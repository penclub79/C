/*
	dhcp client


*/

#include <GrDhcpCli.h>
#include <GrSockV2.h>
#include <GrStrTool.h>
#include <GrProcess.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrError.h>

#include <GrNetBase.h>

#ifdef LINUX_APP

#include	<asm/types.h>

#include	<errno.h>
#include	<arpa/inet.h>
#include	<fcntl.h>
#include	<errno.h>
#include	<netdb.h>

#include	<netinet/in.h>
#include	<netinet/ether.h> 
#include	<net/ethernet.h> 

#include	<sys/socket.h>
#include	<sys/types.h> 
#include	<sys/ioctl.h> 
#include	<sys/utsname.h>

#include	<linux/sockios.h>
#include	<linux/route.h>
#include	<linux/netlink.h>
#include	<linux/rtnetlink.h>

#include	<stdlib.h>

#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE	500
#include <signal.h>

#include <resolv.h>

#else

//#include "winsock2.h"
//#include "ws2tcpip.h"
#include <EmuLnx/GrElaBase.h>

#endif


//--------------------------------------------------------------------
//local const

//--------------------------------------------------------------------
//local type


//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var


//====================================================================
//--------------------------------------------------------------------
Cls_GrDhcpCli::Cls_GrDhcpCli(Cls_GrTaskSvr* A_TaskSvr, BOOL8 A_IsDbgMode) :
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		//init
		m_IsDbgMode	=	A_IsDbgMode;
		m_DhcpStage	=	E_GrDhcpCliStageNone;
		m_HndlRecv	=	E_GrSockBadHandle;
		m_HndlSend	=	E_GrSockBadHandle;

		m_TimeOutCnt	=	0;
		m_SpendTime		=	0;
		m_RetryWait		=	0;

		m_NetIf				=	0;

		//virtual header init
		m_CsVtHd.SrcIp						=	INADDR_ANY;
		m_CsVtHd.DstIp						=	INADDR_BROADCAST;
		m_CsVtHd.Zero							=	0;
		m_CsVtHd.Protocol					=	IPPROTO_UDP;
		//GrDumyEndian2byteSet( &m_CsVtHd.Len, sizeof(St_GrSockUdpHead) + sizeof(St_GrDhcpCliPktMsg) );
		//send packet init
		GrDumyZeroMem( &m_SendPkt, sizeof(m_SendPkt) );
		m_SendPkt.IpHd.HlenVer		=	0x45;					//low 4bit = ipv4, hight 4bit = 5 dword (20byte)
		m_SendPkt.IpHd.Service		=	0;
		//GrDumyEndian2byteSet( &m_SendPkt.IpHd.TtLen, sizeof(m_SendPkt) );
		m_SendPkt.IpHd.Id					=	0;
		m_SendPkt.IpHd.FragOfs		=	0;
		m_SendPkt.IpHd.Ttl				=	64;						//defatul ttl
		m_SendPkt.IpHd.Protocol		=	IPPROTO_UDP;
		//m_SendPkt.IpHd.SrcIp			=	INADDR_ANY;
		//m_SendPkt.IpHd.DstIp			=	INADDR_BROADCAST;
		m_SendPkt.IpHd.ChkSum			=	0;		//prepare calculate checksum
		//m_SendPkt.IpHd.ChkSum			=	GrSockV2CalcIpV4Chksum(&m_SendPkt.IpHd, sizeof(St_GrSockIpHead));
		GrDumyEndian2byteSet( &m_SendPkt.UdpHd.SrcPort, E_GrDhcpCliPortClient );		//
		GrDumyEndian2byteSet( &m_SendPkt.UdpHd.DstPort, E_GrDhcpCliPortServer );		//server port
		//GrDumyEndian2byteSet( &m_SendPkt.UdpHd.Len, sizeof(St_GrSockUdpHead) + sizeof(St_GrDhcpCliPktMsg) );

		//dhcp packet initalize
		m_SendPkt.Msg.MsgType			=	E_GrDhcpCliMsgTypeRequest;
		m_SendPkt.Msg.HwType			=	1;	//ethernet mac address
		m_SendPkt.Msg.HwLen				=	E_GrDhcpCliMacAdrLen;
		m_SendPkt.Msg.HwZeroCnt		=	0;
		//m_SendPkt.Msg.SendId			=	E_GrDhcpCliSendId;
		m_SendPkt.Msg.Sec					=	0;
		//m_SendPkt.Msg.Flags				=	0x0080;	/* broadcast */
		m_SendPkt.Msg.Flags				=	0x0000;		/* unicast */
		m_SendPkt.Msg.ClientAddr	=	0;
		m_SendPkt.Msg.ReqAddr			=	0;
		m_SendPkt.Msg.Siaddr			=	0;
		m_SendPkt.Msg.Giaddr			=	0;
		m_SendPkt.Msg.Cookie			=	E_GrDhcpCliMsgFcc;

		//regist
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrDhcpCliTimerPriod, &m_HndlTime );
		m_TaskSvr->RegistBkgClient( this );
}
//--------------------------------------------------------------------
Cls_GrDhcpCli::~Cls_GrDhcpCli()
{
	// local -------------------
	// code --------------------
		
		ReqFinish();

		//release 
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTime );
		m_TaskSvr->UnregistMsgClient( this );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDhcpCli::LcRequest( void )
{
	// local -------------------
		BOOL8	Tv_Result;
#ifdef LINUX_APP
		__s32			Tv_Atb;
#endif
	// code --------------------
		Tv_Result			=	FALSE;

		//init
		m_AdpIdx			=	GrSockV2NetIfAdaptarIdGet(m_NetIf);
		m_MacAdr			=	0;
#ifdef LINUX_APP
		GrSockV2GetMacAddr( m_NetIf,(__u8*)&m_MacAdr );
#else
		m_MacAdr			=	0x4368006ADA90LL;
#endif
		m_TimeOutCnt	=	0;		//reset time out counter
		m_RetryWait		=	0;		//reset wait time
		m_SendId			=	(__u32)((m_MacAdr >> 16) & 0xFFFFFFFFLL);
		m_RsltIp			=	0;
		m_RsltDns			=	0;
		m_RsltMask		=	0;
		m_RsltGate		=	0;
		m_RsltTime		=	0;
		m_SvrId				=	0;
		m_LeaseLim		=	0;

		//init send packet
		*((__u64*)m_SendPkt.Msg.MacAddr)	=	m_MacAdr;

		//send address init
			
		GrDumyZeroMem( &m_SendAddr, sizeof(m_SendAddr) );
		m_SendAddr.Family			= AF_PACKET;
		m_SendAddr.Protocol		= E_GrDhcpCliSockPrtcIp;
		m_SendAddr.IfIdx			= m_AdpIdx;
		m_SendAddr.AdrLen			= E_GrDhcpCliMacAdrLen;
		m_SendAddr.AdrBuf[0]	=	0xFF;
		m_SendAddr.AdrBuf[1]	=	0xFF;
		m_SendAddr.AdrBuf[2]	=	0xFF;
		m_SendAddr.AdrBuf[3]	=	0xFF;
		m_SendAddr.AdrBuf[4]	=	0xFF;
		m_SendAddr.AdrBuf[5]	=	0xFF;

		//make socket
#ifdef LINUX_APP
		//recive socket
#ifndef E_GrDhcpCliRcvUdp
		m_HndlRecv = socket( PF_PACKET, SOCK_DGRAM, E_GrDhcpCliSockPrtcIp );
		if ( E_GrSockBadHandle != m_HndlRecv )
		{
			//non blocking mode set
			Tv_Atb	=	fcntl(m_HndlRecv, F_GETFL, 0 );
			Tv_Atb	=	Tv_Atb | O_NONBLOCK;
			fcntl(m_HndlRecv, F_SETFL, Tv_Atb);
			//address bind
			GrDumyZeroMem( &m_RecvAddr, sizeof(m_RecvAddr) );
			m_RecvAddr.Family		=	AF_PACKET;
			m_RecvAddr.Protocol	=	E_GrDhcpCliSockPrtcIp;
			m_RecvAddr.IfIdx		=	m_AdpIdx;
			if ( 0 > bind(m_HndlRecv, (struct sockaddr*)&m_RecvAddr, sizeof(m_RecvAddr)) )
			{
				GrSockV2Close( m_HndlRecv );
				m_HndlRecv	=	E_GrSockBadHandle;
			}
		}
#endif
		//send socket
		m_HndlSend	= socket( AF_PACKET, SOCK_DGRAM, E_GrDhcpCliSockPrtcIp );
		if ( E_GrSockBadHandle != m_HndlSend )
		{
			//non blocking mode set
			Tv_Atb	=	fcntl(m_HndlSend, F_GETFL, 0 );
			Tv_Atb	=	Tv_Atb | O_NONBLOCK;
			fcntl(m_HndlSend, F_SETFL, Tv_Atb);
			//address bind
			if ( 0 > bind(m_HndlSend, (struct sockaddr*)&m_SendAddr, sizeof(m_SendAddr)) )
			{
				GrSockV2Close( m_HndlSend );
				m_HndlSend	=	E_GrSockBadHandle;
			}
		}

#endif

#ifdef E_GrDhcpCliRcvUdp
		//recive socket make
		m_HndlRecv	=	GrSockV2Open( TRUE, TRUE );
		if ( E_GrSockBadHandle != m_HndlRecv )
		{
			//set port address
			GrSockV2UpdPortAssign( m_HndlRecv, E_GrDhcpCliPortClient );
		}
#endif
		//check success
#ifdef LINUX_APP
		if ( (E_GrSockBadHandle != m_HndlRecv) && (E_GrSockBadHandle != m_HndlSend) )
#else
		if(TRUE )
#endif
		{
			//set stage
			m_DhcpStage	=	E_GrDhcpCliStageDiscv;
			//success
			Tv_Result		=	TRUE;
		}
		else
		{
			//releas socket
			if ( E_GrSockBadHandle != m_HndlSend )
			{
				GrSockV2Close( m_HndlSend );
				m_HndlSend	=	E_GrSockBadHandle;
			}
			if ( E_GrSockBadHandle != m_HndlRecv )
			{
				GrSockV2Close( m_HndlRecv );
				m_HndlRecv	=	E_GrSockBadHandle;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDhcpCli::LcSendPkt(__u32 A_OptSize)
{
	// local -------------------
		BOOL8	Tv_Result;
		int		Tv_SendSize;
	// code --------------------
		Tv_Result	=	FALSE;
		// init
		Tv_SendSize	=	sizeof(St_GrSockIpHead) + sizeof(St_GrSockUdpHead) + sizeof(St_GrDhcpCliPktMsg) + A_OptSize;
		// setup virtual header
		GrDumyEndian2byteSet(&m_CsVtHd.Len, sizeof(St_GrSockUdpHead) + sizeof(St_GrDhcpCliPktMsg) + A_OptSize);
		// setup ip header
		GrDumyEndian2byteSet(&m_SendPkt.IpHd.TtLen, (__u16)Tv_SendSize);
		m_SendPkt.IpHd.ChkSum		=	0;		// clear checksum field
		m_SendPkt.IpHd.ChkSum		=	GrSockV2CalcIpV4Chksum(&m_SendPkt.IpHd, sizeof(St_GrSockIpHead));
		DbgMsgPrint("[DHCP] IP checksum = %08X\n",m_SendPkt.IpHd.ChkSum);
		// setup udp header
		GrDumyEndian2byteSet(&m_SendPkt.UdpHd.Len, sizeof(St_GrSockUdpHead) + sizeof(St_GrDhcpCliPktMsg) + A_OptSize);
		//calculate checksum
		m_SendPkt.Msg.SendId		=	m_SendId;
		m_SendPkt.UdpHd.ChkSum	=	0;
		m_SendPkt.UdpHd.ChkSum	=	GrSockV2CalcIpV4Chksum(&m_SendPkt.UdpHd,
			sizeof(St_GrSockUdpHead) + sizeof(St_GrDhcpCliPktMsg) + A_OptSize, &m_CsVtHd, sizeof(m_CsVtHd) );
		
#ifdef LINUX_APP
		if(0 < sendto(m_HndlSend, &m_SendPkt, Tv_SendSize, 0, (sockaddr*)&m_SendAddr, sizeof(m_SendAddr)))
#else
		//if(0 < sendto(m_HndlSend, (char*)&m_SendPkt, Tv_SendSize, 0, (sockaddr*)&m_SendAddr, sizeof(m_SendAddr)))
		if(TRUE)
#endif
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::LcStageDiscv( void )
{
	// local -------------------
		__u8*	Tv_PtrSr;
		__u32	Tv_MacVal;
	// code --------------------
		//set ip
		m_SendPkt.IpHd.SrcIp			=	INADDR_ANY;
		m_SendPkt.IpHd.DstIp			=	INADDR_BROADCAST;

		//reset option
		GrDumyZeroMem( m_SendPkt.Options, E_GrDhcpCliDhcpOptLen );
		//discover option
		m_SendPkt.Options[0]	=	E_GrDhcpCliOptCodeMsgType;
		m_SendPkt.Options[1]	=	1;
		m_SendPkt.Options[2]	=	E_GrDhcpCliOptTypeDiscover;
		//client identifier
		m_SendPkt.Options[3]	=	E_GrDhcpCliOptCodeCliId;
		m_SendPkt.Options[4]	=	7;
		m_SendPkt.Options[5]	=	1;		//ethernet type
		Tv_PtrSr	=	(__u8*)&m_MacAdr;
		m_SendPkt.Options[6]	=	Tv_PtrSr[0];
		m_SendPkt.Options[7]	=	Tv_PtrSr[1];
		m_SendPkt.Options[8]	=	Tv_PtrSr[2];
		m_SendPkt.Options[9]	=	Tv_PtrSr[3];
		m_SendPkt.Options[10]	=	Tv_PtrSr[4];
		m_SendPkt.Options[11]	=	Tv_PtrSr[5];
		// maximum DHCP message size
		m_SendPkt.Options[12]	=	E_GrDhcpCliOptCodeMaxMsgSize;
		m_SendPkt.Options[13]	=	2;	// len
		GrDumyEndian2byteSet(&m_SendPkt.Options[14], 1500);		// DHCP message max 1500 byte
		//vendor class identifier
		m_SendPkt.Options[16]	=	E_GrDhcpCliOptCodeClassId;
		m_SendPkt.Options[17]	=	5;
		m_SendPkt.Options[18]	=	'D';
		m_SendPkt.Options[19]	=	'A';
		m_SendPkt.Options[20]	=	'Y';
		m_SendPkt.Options[21]	=	'O';
		m_SendPkt.Options[22]	=	'U';
		// host name - my name
		m_SendPkt.Options[23]	=	E_GrDhcpCliOptCodeClassId;
		m_SendPkt.Options[24]	=	12;	// length
		m_SendPkt.Options[25]	=	'D';
		m_SendPkt.Options[26]	=	'A';
		m_SendPkt.Options[27]	=	'Y';
		m_SendPkt.Options[28]	=	'O';
		m_SendPkt.Options[29]	=	'U';
		m_SendPkt.Options[30]	=	'-';
		Tv_PtrSr	=	(__u8*)&m_MacAdr;
		Tv_MacVal	=	((__u32)Tv_PtrSr[3] << 16) | ((__u32)Tv_PtrSr[4] << 8) | (__u32)Tv_PtrSr[5];
		GrStrDwordToHexStr((char*)&m_SendPkt.Options[31], Tv_MacVal, 6, TRUE);
		//request list
		m_SendPkt.Options[37]	=	E_GrDhcpCliOptCodeReqList;
		m_SendPkt.Options[38]	=	4;
		m_SendPkt.Options[39]	=	E_GrDhcpCliOptCodeNetMask;
		m_SendPkt.Options[40]	=	E_GrDhcpCliOptCodeRouter;
		m_SendPkt.Options[41]	=	E_GrDhcpCliOptCodeDnsServer;
		m_SendPkt.Options[42]	=	E_GrDhcpCliOptCodeLeaseTm;
		//end
		m_SendPkt.Options[43]	=	E_GrDhcpCliOptCodeEnd;

		//send request
		if ( LcSendPkt(44) )
		{
			DbgMsgPrint("Cls_GrDhcpCli::LcStageDiscv - send discover..\n");
			//go next stage
			m_TimeOutCnt	=	0;	//time out reset
#ifdef LINUX_APP
			m_DhcpStage ++;
#else
			m_DhcpStage	=	E_GrDhcpCliStageReq;
#endif
		}
		else
		{
			DbgMsgPrint("Cls_GrDhcpCli::LcStageDiscv - discover send failure.\n");
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDhcpCli::LcRecvPkt( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_RecvLen;
#ifdef LINUX_APP
		socklen_t		Tv_AdrSize;
#endif
	// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
		//Tv_RecvLen	=	read( m_HndlRecv, &m_RecvPkt, sizeof(m_RecvPkt) );
		Tv_AdrSize	=	sizeof(m_RecvAddr);
		Tv_RecvLen	=	recvfrom( m_HndlRecv, &m_RecvPkt, sizeof(m_RecvPkt), 0, (sockaddr*)&m_RecvAddr, &Tv_AdrSize );
#else
		Tv_RecvLen	=	0;
#endif
		if ( 0 < Tv_RecvLen )
		{
			//check size
			if ( (sizeof(St_GrSockIpHead) + sizeof(St_GrSockUdpHead)) < Tv_RecvLen )
			{
				//check useable packet
				if ( (IPPROTO_UDP == m_RecvPkt.IpHd.Protocol) && (0x45 == m_RecvPkt.IpHd.HlenVer) )
				{
					//check port
					if (E_GrDhcpCliPortClient == GrDumyEndian2byteGet( &m_RecvPkt.UdpHd.DstPort) ) 
					{
						//check length
						if ((__u32)Tv_RecvLen >= (GrDumyEndian2byteGet( &m_RecvPkt.UdpHd.Len) + sizeof(St_GrSockIpHead)))
						{
							//check sequence id
							if ( m_SendId == m_RecvPkt.Msg.SendId )
							{
								Tv_Result	=	TRUE;
							}
							else
							{
								DbgMsgPrint( "Cls_GrDhcpCli::LcRecvPkt - bad send id = %8X : %8X \n" , m_SendId, m_RecvPkt.Msg.SendId );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrDhcpCli::LcRecvPkt - bad packet length = %d\n" , Tv_RecvLen );
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDhcpCli::LcOptAnalysis( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Pos;
		__u8*	Tv_PtrOpt;
		__u8*	Tv_PtrRt;
		BOOL8	Tv_IsQuit;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check message type
		if ( E_GrDhcpCliMsgTypeReply == m_RecvPkt.Msg.MsgType )
		{
			DbgMsgPrint( "Cls_GrDhcpCli::LcOptAnalysis - update value...\n" );
			Tv_IsQuit	=	FALSE;
			Tv_PtrOpt	=	m_RecvPkt.Options;
			Tv_Pos		=	0;
			while ( E_GrDhcpCliDhcpOptLen > Tv_Pos )
			{
				switch( Tv_PtrOpt[Tv_Pos + E_GrDhcpCliOptOfsCode] )
				{
					case	E_GrDhcpCliOptCodeNetMask:
						Tv_PtrRt		=	(__u8*)&m_RsltMask;
						Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 2];
						Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 3];
						Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 4];
						Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 5];
						DbgMsgPrint( "Cls_GrDhcpCli::LcValueUpdt - Netmask = %d.%d.%d.%d \n", 
							Tv_PtrOpt[Tv_Pos + 2], Tv_PtrOpt[Tv_Pos + 3], Tv_PtrOpt[Tv_Pos + 4], Tv_PtrOpt[Tv_Pos + 5] );
						break;
					case	E_GrDhcpCliOptCodeRouter:
						Tv_PtrRt		=	(__u8*)&m_RsltGate;
						Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 2];
						Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 3];
						Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 4];
						Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 5];
						DbgMsgPrint( "Cls_GrDhcpCli::LcValueUpdt - gateway = %d.%d.%d.%d \n", 
							Tv_PtrOpt[Tv_Pos + 2], Tv_PtrOpt[Tv_Pos + 3], Tv_PtrOpt[Tv_Pos + 4], Tv_PtrOpt[Tv_Pos + 5] );
						break;
					case	E_GrDhcpCliOptCodeDnsServer:
						Tv_PtrRt		=	(__u8*)&m_RsltDns;
						Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 2];
						Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 3];
						Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 4];
						Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 5];
						DbgMsgPrint( "Cls_GrDhcpCli::LcValueUpdt - dns = %d.%d.%d.%d \n", 
							Tv_PtrOpt[Tv_Pos + 2], Tv_PtrOpt[Tv_Pos + 3], Tv_PtrOpt[Tv_Pos + 4], Tv_PtrOpt[Tv_Pos + 5] );
						break;
					case	E_GrDhcpCliOptCodeServerId:
						Tv_PtrRt		=	(__u8*)&m_SvrId;
						Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 2];
						Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 3];
						Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 4];
						Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 5];
						DbgMsgPrint( "Cls_GrDhcpCli::LcValueUpdt - server ip = %d.%d.%d.%d \n", 
							Tv_PtrOpt[Tv_Pos + 2], Tv_PtrOpt[Tv_Pos + 3], Tv_PtrOpt[Tv_Pos + 4], Tv_PtrOpt[Tv_Pos + 5] );
						break;
					case	E_GrDhcpCliOptCodeLeaseTm:
						Tv_PtrRt		=	(__u8*)&m_RsltTime;
						Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 5];		//change endian
						Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 4];
						Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 3];
						Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 2];
						DbgMsgPrint( "Cls_GrDhcpCli::LcValueUpdt - lease time = %d sec.\n", m_RsltTime );
						break;
					case	E_GrDhcpCliOptCodeEnd:
						Tv_IsQuit	=	TRUE;
						break;
					case	E_GrDhcpCliOptCodeMsgType:
						//get result
						if ( E_GrDhcpCliOptTypeOffer == Tv_PtrOpt[Tv_Pos + 2] )
						{
							Tv_Result	=	TRUE;
						}
						break;
						/*
					default:
						Tv_Len	=	Tv_PtrOpt[Tv_Pos + E_GrDhcpCliOptOfsLen];
						Tv_Val	=	0;
						Tv_PtrRt		=	(__u8*)&Tv_Val;
						Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 2];		
						Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 3];
						Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 4];
						Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 5];
						DbgMsgPrint("Cls_GrDhcpCli::LcValueUpdt : unknown option code - %d ,len = %d, value - %X .\n",
							Tv_PtrOpt[Tv_Pos + E_GrDhcpCliOptOfsCode], Tv_Len, Tv_Val );
						break;
						*/
				}
				//next
				if ( E_GrDhcpCliOptCodePadding == Tv_PtrOpt[Tv_Pos + E_GrDhcpCliOptOfsCode] )
				{
					//padding
					Tv_Pos ++;
				}
				else
				{
					//normal packet
					Tv_Pos	=	Tv_Pos + (__u32)Tv_PtrOpt[Tv_Pos + E_GrDhcpCliOptOfsLen] + 2;
				}
				//check quit
				if ( Tv_IsQuit )
				{
					break;
				}
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrDhcpCli::LcOptAnalysis - bad message type = %d \n", m_RecvPkt.Msg.MsgType ); 
		}

		if ( Tv_Result )
		{
			//get ip
			m_RsltIp	=	m_RecvPkt.Msg.ReqAddr;
			Tv_PtrOpt	=	(__u8*)&m_RsltIp;
			DbgMsgPrint( "Cls_GrDhcpCli::LcOptAnalysis - myip = %d.%d.%d.%d \n", 
				Tv_PtrOpt[0], Tv_PtrOpt[1], Tv_PtrOpt[2], Tv_PtrOpt[3] );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDhcpCli::LcOptAnalysis - bad option msg type. \n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDhcpCli::LcGetOptType( void )
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_Pos;
		__u8*	Tv_PtrOpt;
		BOOL8	Tv_IsQuit;
	// code --------------------
		Tv_Result	=	0;
		Tv_IsQuit	=	FALSE;
		Tv_PtrOpt	=	m_RecvPkt.Options;
		Tv_Pos		=	0;
		while ( E_GrDhcpCliDhcpOptLen > Tv_Pos )
		{
			switch( Tv_PtrOpt[Tv_Pos + E_GrDhcpCliOptOfsCode] )
			{
				case	E_GrDhcpCliOptCodeEnd:
					Tv_IsQuit	=	TRUE;
					break;
				case	E_GrDhcpCliOptCodeMsgType:
					//get result
					Tv_Result	=	Tv_PtrOpt[Tv_Pos + 2];
					Tv_IsQuit	=	TRUE;
					break;
					/*
				default:
					Tv_Len	=	Tv_PtrOpt[Tv_Pos + E_GrDhcpCliOptOfsLen];
					Tv_Val	=	0;
					Tv_PtrRt		=	(__u8*)&Tv_Val;
					Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 2];		
					Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 3];
					Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 4];
					Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 5];
					DbgMsgPrint("Cls_GrDhcpCli::LcValueUpdt : unknown option code - %d ,len = %d, value - %X .\n",
						Tv_PtrOpt[Tv_Pos + E_GrDhcpCliOptOfsCode], Tv_Len, Tv_Val );
					break;
					*/
			}
			//next
			if ( E_GrDhcpCliOptCodePadding == Tv_PtrOpt[Tv_Pos + E_GrDhcpCliOptOfsCode] )
			{
				//padding
				Tv_Pos ++;
			}
			else
			{
				//normal packet
				Tv_Pos	=	Tv_Pos + (__u32)Tv_PtrOpt[Tv_Pos + E_GrDhcpCliOptOfsLen] + 2;
			}
			//check quit
			if ( Tv_IsQuit )
			{
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::LcStageOffer( void )
{
	// local -------------------
	// code --------------------
#ifdef E_GrDhcpCliRcvUdp
		if ( LcRecvUdpPkt() )
#else
		if ( LcRecvPkt() )
#endif
		{
#ifdef E_GrDhcpCliDbgMsg
			DbgMsgPrint("[DHCP] stage offer - data received.\n");
#endif
			//DbgMemDump( m_RecvPkt.Msg.Options, E_GrDhcpCliDhcpOptLen );
			//check option
			if ( LcOptAnalysis() )
			{
				//go next stage
				m_DhcpStage ++;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::LcStageReq( void )
{
	// local -------------------
		__u8*	Tv_PtrBt;
		__u8*	Tv_PtrSr;
		__u32	Tv_MacVal;
	// code --------------------
		//reset option
		GrDumyZeroMem( m_SendPkt.Options, E_GrDhcpCliDhcpOptLen );
		//discover option
		m_SendPkt.Options[0]	=	E_GrDhcpCliOptCodeMsgType;
		m_SendPkt.Options[1]	=	1;
		m_SendPkt.Options[2]	=	E_GrDhcpCliOptTypeRequest;
		//client identifier
		m_SendPkt.Options[3]	=	E_GrDhcpCliOptCodeCliId;
		m_SendPkt.Options[4]	=	7;
		m_SendPkt.Options[5]	=	1;		//ethernet type
		Tv_PtrBt	=	(__u8*)&m_MacAdr;
		m_SendPkt.Options[6]	=	Tv_PtrBt[0];
		m_SendPkt.Options[7]	=	Tv_PtrBt[1];
		m_SendPkt.Options[8]	=	Tv_PtrBt[2];
		m_SendPkt.Options[9]	=	Tv_PtrBt[3];
		m_SendPkt.Options[10]	=	Tv_PtrBt[4];
		m_SendPkt.Options[11]	=	Tv_PtrBt[5];
		//request ip
		m_SendPkt.Options[12]	=	E_GrDhcpCliOptCodeIpAddr;
		m_SendPkt.Options[13]	=	4;
		Tv_PtrBt	=	(__u8*)&m_RsltIp;
		m_SendPkt.Options[14]	=	Tv_PtrBt[0];
		m_SendPkt.Options[15]	=	Tv_PtrBt[1];
		m_SendPkt.Options[16]	=	Tv_PtrBt[2];
		m_SendPkt.Options[17]	=	Tv_PtrBt[3];
		//add server id
		m_SendPkt.Options[18]	=	E_GrDhcpCliOptCodeServerId;
		m_SendPkt.Options[19]	=	4;
		Tv_PtrBt	=	(__u8*)&m_SvrId;
		m_SendPkt.Options[20]	=	Tv_PtrBt[0];
		m_SendPkt.Options[21]	=	Tv_PtrBt[1];
		m_SendPkt.Options[22]	=	Tv_PtrBt[2];
		m_SendPkt.Options[23]	=	Tv_PtrBt[3];
		// maximum DHCP message size
		m_SendPkt.Options[24]	=	E_GrDhcpCliOptCodeMaxMsgSize;
		m_SendPkt.Options[25]	=	2;	// len
		GrDumyEndian2byteSet(&m_SendPkt.Options[26], 1500);		// DHCP message max 1500 byte
		//class identifier
		m_SendPkt.Options[28]	=	E_GrDhcpCliOptCodeClassId;
		m_SendPkt.Options[29]	=	5;
		m_SendPkt.Options[30]	=	'D';
		m_SendPkt.Options[31]	=	'A';
		m_SendPkt.Options[32]	=	'Y';
		m_SendPkt.Options[33]	=	'O';
		m_SendPkt.Options[34]	=	'U';
		// host name - my name
		m_SendPkt.Options[35]	=	E_GrDhcpCliOptCodeClassId;
		m_SendPkt.Options[36]	=	12;	// length
		m_SendPkt.Options[37]	=	'D';
		m_SendPkt.Options[38]	=	'A';
		m_SendPkt.Options[39]	=	'Y';
		m_SendPkt.Options[40]	=	'O';
		m_SendPkt.Options[41]	=	'U';
		m_SendPkt.Options[42]	=	'-';
		Tv_PtrSr	=	(__u8*)&m_MacAdr;
		Tv_MacVal	=	((__u32)Tv_PtrSr[3] << 16) | ((__u32)Tv_PtrSr[4] << 8) | (__u32)Tv_PtrSr[5];
		GrStrDwordToHexStr((char*)&m_SendPkt.Options[43], Tv_MacVal, 6, TRUE);
		//request list
		m_SendPkt.Options[49]	=	E_GrDhcpCliOptCodeReqList;
		m_SendPkt.Options[50]	=	4;
		m_SendPkt.Options[51]	=	E_GrDhcpCliOptCodeNetMask;
		m_SendPkt.Options[52]	=	E_GrDhcpCliOptCodeRouter;
		m_SendPkt.Options[53]	=	E_GrDhcpCliOptCodeDnsServer;
		m_SendPkt.Options[54]	=	E_GrDhcpCliOptCodeLeaseTm;
		//end
		m_SendPkt.Options[55]	=	E_GrDhcpCliOptCodeEnd;

		//send request
		if ( LcSendPkt(56) )
		{
			DbgMsgPrint("Cls_GrDhcpCli::LcStageReq - send select..\n");
			//go next stage
			m_TimeOutCnt	=	0;	//time out reset
			m_DhcpStage ++;
		}
		else
		{
			DbgMsgPrint("Cls_GrDhcpCli::LcStageReq - select send failure.\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::LcStageAck( void )
{
	// local -------------------
		__u8	Tv_OptType;
	// code --------------------
#ifdef E_GrDhcpCliRcvUdp
		if ( LcRecvUdpPkt() )
#else
		if ( LcRecvPkt() )
#endif
		{
			//check option
			Tv_OptType	=	LcGetOptType();
			//if((E_GrDhcpCliOptTypeAck == Tv_OptType) || (E_GrDhcpCliOptTypeOffer == Tv_OptType))
			if(E_GrDhcpCliOptTypeAck == Tv_OptType)
			{
				//success
				m_SpendTime		=	0;		//lease timer reset
				m_LeaseLim		=	m_RsltTime - (m_RsltTime >> 2);
				//release handle
				if ( E_GrSockBadHandle != m_HndlSend )
				{
					GrSockV2Close( m_HndlSend );
					m_HndlSend	=	E_GrSockBadHandle;
				}
				if ( E_GrSockBadHandle != m_HndlRecv )
				{
					GrSockV2Close( m_HndlRecv );
					m_HndlRecv	=	E_GrSockBadHandle;
				}
				//update value
				if ( 0 != m_RsltIp )
				{
					if(!m_IsDbgMode)
					{
						GrSockV2SetNetAdr(m_NetIf, &m_RsltIp, &m_RsltMask, &m_RsltGate, FALSE);
					}
					DbgMsgPrint( "Cls_GrDhcpCli::LcStageAck - net addr update(ip:%d.%d.%d.%d mask:%d.%d.%d.%d gate:%d.%d.%d.%d).\n",
						(m_RsltIp & 0xFF), ((m_RsltIp >> 8) & 0xFF), ((m_RsltIp >> 16) & 0xFF), ((m_RsltIp >> 24) & 0xFF),
						(m_RsltMask & 0xFF), ((m_RsltMask >> 8) & 0xFF), ((m_RsltMask >> 16) & 0xFF), ((m_RsltMask >> 24) & 0xFF),
						(m_RsltGate & 0xFF), ((m_RsltGate >> 8) & 0xFF), ((m_RsltGate >> 16) & 0xFF), ((m_RsltGate >> 24) & 0xFF)
						);
					if ( 0 != m_RsltDns )
					{
						GrNetDnsSetup(m_RsltDns);
						DbgMsgPrint( "Cls_GrDhcpCli::LcStageAck - dns update.\n" );
					}
				}
				//go next stage
				m_DhcpStage ++;
				DbgMsgPrint( "Cls_GrDhcpCli::LcStageAck - dhcp completed.\n" );
			}
			else
			{
				DbgMsgPrint( "Cls_GrDhcpCli::LcStageAck - bad opt type - %d.\n", Tv_OptType );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::ReqRun(void)
{
	// local -------------------
		St_GrDhcpCliMsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrDhcpCliCmdRun;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::ReqStop(void)
{
	// local -------------------
		St_GrDhcpCliMsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrDhcpCliCmdStop;
		m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::ReqInit(void)
{
	// local -------------------
		St_GrDhcpCliMsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrDhcpCliCmdInit;
		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::ReqFinish(void)
{
	// local -------------------
		St_GrDhcpCliMsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrDhcpCliCmdFinish;
		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::ReqSetup(__u8 A_NetIf)
{
	// local -------------------
		St_GrDhcpCliMsgSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDhcpCliCmdSetup;
		Tv_Msg.NetIf				=	A_NetIf;

		m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDhcpCli::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			switch( ((Ptr_GrDhcpCliMsgHd)A_PtrMsg)->Cmd )
			{
				case E_GrDhcpCliCmdInit:
					ImmInit();
					break;
				case E_GrDhcpCliCmdFinish:
					ImmFinish();
					break;
				case	E_GrDhcpCliCmdSetup:
					ImmSetup(((Ptr_GrDhcpCliMsgSetup)A_PtrMsg)->NetIf);
					break;
				case E_GrDhcpCliCmdRun:
					ImmRun();
					break;
				case E_GrDhcpCliCmdStop:
					ImmStop();
					break;
			}
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDhcpCli::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch( m_DhcpStage )
		{
			case	E_GrDhcpCliStageDiscv:
				LcStageDiscv();
				break;
			case	E_GrDhcpCliStageOffer:
				LcStageOffer();
				break;
			case	E_GrDhcpCliStageReq:
				LcStageReq();
				break;
			case	E_GrDhcpCliStageAck:
				LcStageAck();
				break;
			case E_GrDhcpCliStageExpReq:
				LcStageExpReq();
				break;
			case E_GrDhcpCliStageExpAck:
				LcStageExpAck();
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
	// code --------------------
		if ( m_HndlTime == A_Hndl )
		{
			//check working
			if ( E_GrDhcpCliStageNone != m_DhcpStage)
			{
				if(E_GrDhcpCliStageRetryWait == m_DhcpStage)
				{
					m_RetryWait ++;
					if(E_GrDhcpCliRetryWait <= m_RetryWait)
					{
						m_RetryWait	=	0;
						LcRequest();
					}
				}
				else if(E_GrDhcpCliStageWaitLease == m_DhcpStage)
				{
					m_SpendTime ++;
					if(m_SpendTime >= m_LeaseLim)
					{
						m_SpendTime		=	0;
						// try expand
						if(!LcLeaseExp())
						{
							//finish
							LcFinish();
							// retry mode
							m_DhcpStage	=	E_GrDhcpCliStageRetryWait;
							m_RetryWait	=	0;
						}
					}
				}
				else
				{
					m_TimeOutCnt ++;
					if(E_GrDhcpCliTimeout < m_TimeOutCnt)
					{
						//finish
						LcFinish();
						// retry mode
						m_DhcpStage	=	E_GrDhcpCliStageRetryWait;
						m_RetryWait	=	0;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::LcFinish( void )
{
		//stop and reset
		m_RsltIp			=	0;
		m_RsltMask		=	0;
		m_RsltGate		=	0;
		m_RsltDns			=	0;
		m_RsltTime		=	0;
		m_SpendTime		=	0;
		m_TimeOutCnt	=	0;
		m_RetryWait		=	0;
		//release handle
		if ( E_GrSockBadHandle != m_HndlSend )
		{
			GrSockV2Close( m_HndlSend );
			m_HndlSend	=	E_GrSockBadHandle;
		}
		if ( E_GrSockBadHandle != m_HndlRecv )
		{
			GrSockV2Close( m_HndlRecv );
			m_HndlRecv	=	E_GrSockBadHandle;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDhcpCli::LcLeaseExp(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	#ifdef LINUX_APP
		__s32			Tv_Atb;
	#endif
	// code --------------------
		Tv_Result			=	FALSE;
		//init
		m_SpendTime		=	0;
		m_TimeOutCnt	=	0;		//reset time out counter
		m_RetryWait		=	0;		//reset wait reset

		//init send packet
		*((__u64*)m_SendPkt.Msg.MacAddr)	=	m_MacAdr;

		//send address init
		GrDumyZeroMem( &m_SendAddr, sizeof(m_SendAddr) );
#ifdef LINUX_APP
		m_SendAddr.Family			= AF_PACKET;
#endif
		m_SendAddr.Protocol		= E_GrDhcpCliSockPrtcIp;
		m_SendAddr.IfIdx			= m_AdpIdx;
		m_SendAddr.AdrLen			= E_GrDhcpCliMacAdrLen;
		m_SendAddr.AdrBuf[0]	=	0xFF;
		m_SendAddr.AdrBuf[1]	=	0xFF;
		m_SendAddr.AdrBuf[2]	=	0xFF;
		m_SendAddr.AdrBuf[3]	=	0xFF;
		m_SendAddr.AdrBuf[4]	=	0xFF;
		m_SendAddr.AdrBuf[5]	=	0xFF;

		//make socket
#ifdef LINUX_APP
		//recive socket
#ifndef E_GrDhcpCliRcvUdp
		m_HndlRecv = socket( PF_PACKET, SOCK_DGRAM, E_GrDhcpCliSockPrtcIp );
		if ( E_GrSockBadHandle != m_HndlRecv )
		{
			//non blocking mode set
			Tv_Atb	=	fcntl(m_HndlRecv, F_GETFL, 0 );
			Tv_Atb	=	Tv_Atb | O_NONBLOCK;
			fcntl(m_HndlRecv, F_SETFL, Tv_Atb);
			//address bind
			GrDumyZeroMem( &m_RecvAddr, sizeof(m_RecvAddr) );
			m_RecvAddr.Family		=	AF_PACKET;
			m_RecvAddr.Protocol	=	E_GrDhcpCliSockPrtcIp;
			m_RecvAddr.IfIdx		=	m_AdpIdx;
			if ( 0 > bind(m_HndlRecv, (struct sockaddr*)&m_RecvAddr, sizeof(m_RecvAddr)) )
			{
				GrSockV2Close( m_HndlRecv );
				m_HndlRecv	=	E_GrSockBadHandle;
			}
		}
#endif
		//send socket
		m_HndlSend	= socket( AF_PACKET, SOCK_DGRAM, E_GrDhcpCliSockPrtcIp );
		if ( E_GrSockBadHandle != m_HndlSend )
		{
			//non blocking mode set
			Tv_Atb	=	fcntl(m_HndlSend, F_GETFL, 0 );
			Tv_Atb	=	Tv_Atb | O_NONBLOCK;
			fcntl(m_HndlSend, F_SETFL, Tv_Atb);
			//address bind
			if ( 0 > bind(m_HndlSend, (struct sockaddr*)&m_SendAddr, sizeof(m_SendAddr)) )
			{
				GrSockV2Close( m_HndlSend );
				m_HndlSend	=	E_GrSockBadHandle;
			}
		}
#endif
#ifdef E_GrDhcpCliRcvUdp
			//recive socket make
			m_HndlRecv	=	GrSockOpen( TRUE, TRUE );
			if ( E_GrSockBadHandle != m_HndlRecv )
			{
				//set port address
				GrSockV2UpdPortAssign( m_HndlRecv, E_GrDhcpCliPortClient );
			}
#endif

		//check success
		if ( (E_GrSockBadHandle != m_HndlRecv) && (E_GrSockBadHandle != m_HndlSend) )
		{
			//set stage
			m_DhcpStage	=	E_GrDhcpCliStageDiscv;
			//m_DhcpStage	=	E_GrDhcpCliStageReq;
			//m_SendPkt.IpHd.SrcIp			=	m_RsltIp;
			//m_SendPkt.IpHd.DstIp			=	m_SvrId;
			DbgMsgPrint("Cls_GrDhcpCli::LcLeaseExp - try lease extend..\n");
			//success
			Tv_Result		=	TRUE;
		}
		else
		{
			//releas socket
			if ( E_GrSockBadHandle != m_HndlSend )
			{
				GrSockV2Close( m_HndlSend );
				m_HndlSend	=	E_GrSockBadHandle;
			}
			if ( E_GrSockBadHandle != m_HndlRecv )
			{
				GrSockV2Close( m_HndlRecv );
				m_HndlRecv	=	E_GrSockBadHandle;
			}
			DbgMsgPrint("Cls_GrDhcpCli::LcLeaseExp - socket allocate failure.\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::LcStageExpReq( void )
{
	// local -------------------
		__u8*	Tv_PtrBt;
	// code --------------------
		//reset option
		GrDumyZeroMem( m_SendPkt.Options, E_GrDhcpCliDhcpOptLen );
		//discover option
		m_SendPkt.Options[0]	=	E_GrDhcpCliOptCodeMsgType;
		m_SendPkt.Options[1]	=	1;
		m_SendPkt.Options[2]	=	E_GrDhcpCliOptTypeRequest;
		//client identifier
		m_SendPkt.Options[3]	=	E_GrDhcpCliOptCodeCliId;
		m_SendPkt.Options[4]	=	7;
		m_SendPkt.Options[5]	=	1;		//ethernet type
		Tv_PtrBt	=	(__u8*)&m_MacAdr;
		m_SendPkt.Options[6]	=	Tv_PtrBt[0];
		m_SendPkt.Options[7]	=	Tv_PtrBt[1];
		m_SendPkt.Options[8]	=	Tv_PtrBt[2];
		m_SendPkt.Options[9]	=	Tv_PtrBt[3];
		m_SendPkt.Options[10]	=	Tv_PtrBt[4];
		m_SendPkt.Options[11]	=	Tv_PtrBt[5];
		//class identifier
		m_SendPkt.Options[12]	=	E_GrDhcpCliOptCodeClassId;
		m_SendPkt.Options[13]	=	5;
		m_SendPkt.Options[14]	=	'D';
		m_SendPkt.Options[15]	=	'A';
		m_SendPkt.Options[16]	=	'Y';
		m_SendPkt.Options[17]	=	'O';
		m_SendPkt.Options[18]	=	'U';
		//request ip
		m_SendPkt.Options[19]	=	E_GrDhcpCliOptCodeIpAddr;
		m_SendPkt.Options[20]	=	4;
		Tv_PtrBt	=	(__u8*)&m_RsltIp;
		m_SendPkt.Options[21]	=	Tv_PtrBt[0];
		m_SendPkt.Options[22]	=	Tv_PtrBt[1];
		m_SendPkt.Options[23]	=	Tv_PtrBt[2];
		m_SendPkt.Options[24]	=	Tv_PtrBt[3];
		//add server id
		m_SendPkt.Options[25]	=	E_GrDhcpCliOptCodeServerId;
		m_SendPkt.Options[26]	=	4;
		Tv_PtrBt	=	(__u8*)&m_SvrId;
		m_SendPkt.Options[27]	=	Tv_PtrBt[0];
		m_SendPkt.Options[28]	=	Tv_PtrBt[1];
		m_SendPkt.Options[29]	=	Tv_PtrBt[2];
		m_SendPkt.Options[30]	=	Tv_PtrBt[3];
		//request list
		m_SendPkt.Options[31]	=	E_GrDhcpCliOptCodeReqList;
		m_SendPkt.Options[32]	=	3;
		m_SendPkt.Options[33]	=	E_GrDhcpCliOptCodeNetMask;
		m_SendPkt.Options[34]	=	E_GrDhcpCliOptCodeRouter;
		m_SendPkt.Options[35]	=	E_GrDhcpCliOptCodeDnsServer;
		//end
		m_SendPkt.Options[36]	=	E_GrDhcpCliOptCodeEnd;

		//send request
		if ( 0 != GrSockV2UdpSend( m_HndlSend, &m_SvrId, E_GrDhcpCliPortServer, &m_SendPkt.Msg, sizeof(St_GrDhcpCliPktMsg),FALSE ) )
		{
			DbgMsgPrint("Cls_GrDhcpCli::LcStageExpReq - send lease extend reqset..\n");
			//go next stage
			m_TimeOutCnt	=	0;	//time out reset
			m_DhcpStage ++;
		}
		else
		{
			DbgMsgPrint("Cls_GrDhcpCli::LcStageExpReq - lease extend  send failure.\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::LcStageExpAck( void )
{
	// local -------------------
		__u32	Tv_Ip;
		__u16	Tv_Port;
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		Tv_Ip		=	m_SvrId;
		Tv_Port	=	E_GrDhcpCliPortServer;
		if(0 != GrSockV2UdpRead(m_HndlRecv, &Tv_Ip, &Tv_Port, &m_RecvPkt.Msg, sizeof(St_GrDhcpCliPktMsg),FALSE,&Tv_ErrCode))
		{
			//check match data
			if ( (m_SvrId == Tv_Ip) && (m_SendId == m_RecvPkt.Msg.SendId) )
			{
				//check option
				if ( E_GrDhcpCliOptTypeAck == LcGetOptType() )
				{
					//success
					//release handle
					if ( E_GrSockBadHandle != m_HndlSend )
					{
						GrSockV2Close( m_HndlSend );
						m_HndlSend	=	E_GrSockBadHandle;
					}
					if ( E_GrSockBadHandle != m_HndlRecv )
					{
						GrSockV2Close( m_HndlRecv );
						m_HndlRecv	=	E_GrSockBadHandle;
					}
					//update lease
					m_SpendTime	=	0;		//lease time reset
					//go finish stage
					m_DhcpStage	=	E_GrDhcpCliStageWaitLease;
					DbgMsgPrint( "Cls_GrDhcpCli::LcStageExpAck - lease extend completed.\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrDhcpCli::LcStageExpAck - recive bad msg.\n" );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDhcpCli::LcRecvUdpPkt( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Readed;
		__u16	Tv_Port;
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Port		=	E_GrDhcpCliPortServer;
		Tv_Readed	=	GrSockV2UdpRead( m_HndlRecv, &m_SvUdpIp, &Tv_Port, &m_RecvPkt.Msg, sizeof(St_GrDhcpCliPktMsg),FALSE,&Tv_ErrCode );
		if ( 0 < Tv_Readed )
		{
			//check sendid
			if ( m_SendId == m_RecvPkt.Msg.SendId )
			{
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrDhcpCli::LcRecvUdpPkt - send id mismath - %d : %d  pktsize= %d  ip=%08X.\n" , 
					m_SendId, m_RecvPkt.Msg.SendId, Tv_Readed, m_SvUdpIp );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::ImmSetup(__u8 A_NetIf)
{
	// local -------------------
	// code --------------------
		m_NetIf		=	A_NetIf;
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::ImmRun(void)
{
	// local -------------------
	// code --------------------
		// reset
		if(E_GrDhcpCliStageNone != m_DhcpStage)
		{
			// close socket
			LcFinish();
			m_DhcpStage	=	E_GrDhcpCliStageNone;
		}

		if(!LcRequest())
		{
			m_RetryWait	=	0;
			m_DhcpStage	=	E_GrDhcpCliStageRetryWait;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::ImmStop(void)
{
		if(E_GrDhcpCliStageNone != m_DhcpStage)
		{
			LcFinish();
			m_TimeOutCnt	=	0;
			m_SpendTime		=	0;
			m_RetryWait		=	0;
			m_DhcpStage		=	E_GrDhcpCliStageNone;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::ImmInit(void)
{
		GrSockV2Init();
}
//--------------------------------------------------------------------
void	Cls_GrDhcpCli::ImmFinish(void)
{
		LcFinish();
		GrSockV2Finish();
}
//--------------------------------------------------------------------


