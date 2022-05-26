/*
 GAUSE Network DHCP plug-in

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <Net/GrNetPlugDhcp.h>
#include <Net/GrNetSvs.h>

#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include <GrError.h>

#include	<stdlib.h>

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

#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE	500
#include <signal.h>

#include <resolv.h>

#endif


//====================================================================
//local const
#define E_GrNetPlugDhcpTimeout			10		/* time out time */
#define E_GrNetPlugDhcpRetry				3			/* retry count */
#define E_GrNetPlugDhcpTimerPriod		1000	/* sec timer */

#define E_GrNetPlugDhcpStageNone		0			/* not work or failure stage */
#define E_GrNetPlugDhcpStageDiscv		1			/* discover stage */
#define E_GrNetPlugDhcpStageOffer		2			/* wait offer stage */
#define E_GrNetPlugDhcpStageReq			3			/* request stage */
#define E_GrNetPlugDhcpStageAck			4			/* wait ACK stage */
#define E_GrNetPlugDhcpStageOk			5			/* receive success */
#define E_GrNetPlugDhcpStageExpReq	6			/* lease time expand request */
#define E_GrNetPlugDhcpStageExpAck	7			/* lease time expand acknowledge */

#define E_GrNetPlugDhcpSockPrtcIp		0x08	/* protocol type ip */
#define E_GrNetPlugDhcpMacAdrLen		6
#define E_GrNetPlugDhcpPortServer		67
#define E_GrNetPlugDhcpPortClient		68

#define E_GrNetPlugDhcpMsgTypeRequest	1
#define E_GrNetPlugDhcpMsgTypeReply		2

#define E_GrNetPlugDhcpMsgFcc				0x63538263

#define E_GrNetPlugDhcpOptCodePadding		0x00
#define E_GrNetPlugDhcpOptCodeNetMask		1
#define E_GrNetPlugDhcpOptCodeRouter		3
#define E_GrNetPlugDhcpOptCodeDnsServer	6
#define E_GrNetPlugDhcpOptCodeOptOver		0x34
#define E_GrNetPlugDhcpOptCodeMsgType		0x35
#define E_GrNetPlugDhcpOptCodeServerId	0x36
#define E_GrNetPlugDhcpOptCodeClassId		0x3C		/* class identifier */
#define E_GrNetPlugDhcpOptCodeCliId			0x3D		/* client identifier */
#define E_GrNetPlugDhcpOptCodeIpAddr		50
#define E_GrNetPlugDhcpOptCodeLeaseTm		51
#define E_GrNetPlugDhcpOptCodeReqList		55
#define E_GrNetPlugDhcpOptCodeEnd				0xFF

#define E_GrNetPlugDhcpOptTypeDiscover	1
#define E_GrNetPlugDhcpOptTypeOffer			2
#define E_GrNetPlugDhcpOptTypeRequest		3
#define E_GrNetPlugDhcpOptTypeAck				5
#define E_GrNetPlugDhcpOptTypeNack			6

#define E_GrNetPlugDhcpOptOfsCode			0
#define E_GrNetPlugDhcpOptOfsLen			1

#define E_GrNetPlugDhcpOptFldOpt					0
#define E_GrNetPlugDhcpOptFldFile					1
#define E_GrNetPlugDhcpOptFldSvName				2

//#define	E_GrNetPlugDhcpRcvUdp							//receive UDP mode


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrNetPlugDhcp::Cls_GrNetPlugDhcp(Cls_GrTaskSvr* A_TaskSvr, Cls_GrTaskCli* A_NetSvs, __u8 A_DevIdx, __u8 A_PlugIdx) :
Cls_GrNetPlugBase(A_TaskSvr, A_NetSvs, A_DevIdx, A_PlugIdx)
{
	// local -------------------
	// code --------------------
		// init
		m_DhcpStage	=	E_GrNetPlugDhcpStageNone;
		m_HndlRecv	=	E_GrSockBadHandle;
		m_HndlSend	=	E_GrSockBadHandle;

		m_TimeOutCnt	=	0;
		m_IsLeaseExp	=	FALSE;
		m_SpendTime		=	0;

		//virtual header init
		m_CsVtHd.SrcIp						=	INADDR_ANY;
		m_CsVtHd.DstIp						=	INADDR_BROADCAST;
		m_CsVtHd.Zero							=	0;
		m_CsVtHd.Protocol					=	IPPROTO_UDP;
		GrDumyEndian2byteSet( &m_CsVtHd.Len, sizeof(St_GrSockUdpHead) + sizeof(St_GrNetPlugDhcpPktMsg) );
		//send packet init
		GrDumyZeroMem( &m_SendPkt, sizeof(m_SendPkt) );
		m_SendPkt.IpHd.HlenVer		=	0x45;					//low 4bit = ipv4, hight 4bit = 5 dword (20byte)
		m_SendPkt.IpHd.Service		=	0;
		GrDumyEndian2byteSet( &m_SendPkt.IpHd.TtLen, sizeof(m_SendPkt) );
		m_SendPkt.IpHd.Id					=	0;
		m_SendPkt.IpHd.FragOfs		=	0;
		m_SendPkt.IpHd.Ttl				=	64;						//default
		m_SendPkt.IpHd.Protocol		=	IPPROTO_UDP;
		//m_SendPkt.IpHd.SrcIp			=	INADDR_ANY;
		//m_SendPkt.IpHd.DstIp			=	INADDR_BROADCAST;
		m_SendPkt.IpHd.ChkSum			=	0;		//prepare calculate checksum
		m_SendPkt.IpHd.ChkSum			=	GrSockV2CalcIpV4Chksum( &m_SendPkt.IpHd, sizeof(St_GrSockIpHead) );
		GrDumyEndian2byteSet( &m_SendPkt.UdpHd.SrcPort, E_GrNetPlugDhcpPortClient );		//
		GrDumyEndian2byteSet( &m_SendPkt.UdpHd.DstPort, E_GrNetPlugDhcpPortServer );		//server port
		GrDumyEndian2byteSet( &m_SendPkt.UdpHd.Len, sizeof(St_GrSockUdpHead) + sizeof(St_GrNetPlugDhcpPktMsg) );

		//DHCP packet initialize
		m_SendPkt.Msg.MsgType			=	E_GrNetPlugDhcpMsgTypeRequest;
		m_SendPkt.Msg.HwType			=	1;	//ETHERNET mac address
		m_SendPkt.Msg.HwLen				=	E_GrNetPlugDhcpMacAdrLen;
		m_SendPkt.Msg.HwZeroCnt		=	0;
		//m_SendPkt.Msg.SendId			=	E_GrNetPlugDhcpSendId;
		m_SendPkt.Msg.Sec					=	0;
		//m_SendPkt.Msg.Flags				=	0x0080;	/* broadcast */
		m_SendPkt.Msg.Flags				=	0x0000;		/* UNICAST */
		m_SendPkt.Msg.ClientAddr	=	0;
		m_SendPkt.Msg.ReqAddr			=	0;
		m_SendPkt.Msg.Siaddr			=	0;
		m_SendPkt.Msg.Giaddr			=	0;
		m_SendPkt.Msg.Cookie			=	E_GrNetPlugDhcpMsgFcc;

		m_TaskSvr->RegistTimerClient(this, E_GrNetPlugDhcpTimerPeriod, &m_HndlTimer);
		m_TaskSvr->RegistBkgClient(this);

}
//--------------------------------------------------------------------
Cls_GrNetPlugDhcp::~Cls_GrNetPlugDhcp()
{
		LcFinish();

		m_TaskSvr->UnregistBkgClient(this);
		m_TaskSvr->UnregistTimerClient(m_HndlTimer);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugDhcp::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch( m_DhcpStage )
		{
			case	E_GrNetPlugDhcpStageDiscv:
				LcStageDiscv();
				break;
			case	E_GrNetPlugDhcpStageOffer:
				LcStageOffer();
				break;
			case	E_GrNetPlugDhcpStageReq:
				LcStageReq();
				break;
			case	E_GrNetPlugDhcpStageAck:
				LcStageAck();
				break;
			case E_GrNetPlugDhcpStageExpReq:
				LcStageExpReq();
				break;
			case E_GrNetPlugDhcpStageExpAck:
				LcStageExpAck();
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugDhcp::RtlTimer(__u32 A_Hndl)
{
	// local -------------------
	// code --------------------
		//check working
		if ( (E_GrNetPlugDhcpStageNone != m_DhcpStage) && (E_GrNetPlugDhcpStageOk != m_DhcpStage) )
		{
			m_TimeOutCnt ++;
			if ( E_GrNetPlugDhcpTimeout < m_TimeOutCnt )
			{
				//finish
				LcFinish();
				//check lease mode
				if ( m_IsLeaseExp )
				{
					LcLeaseExp( );
				}
				else
				{
					m_DhcpStage	=	E_GrNetPlugDhcpStageOk;
				}
			}
		}
		else
		{
			//check lease extend
			if ( m_IsLeaseExp )
			{
				m_SpendTime ++;
				if ( m_SpendTime == m_LeaseLim )
				{
					//try lease extend
					LcLeaseExp();
				}
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugDhcp::DhcpRequest(void)
{
	// local -------------------
		BOOL8	Tv_Result;
#ifdef LINUX_APP
		__s32			Tv_Atb;
#endif
	// code --------------------
		Tv_Result	=	FALSE;
		//check stage
		if ( (E_GrNetPlugDhcpStageNone == m_DhcpStage) || (E_GrNetPlugDhcpStageOk == m_DhcpStage) )
		{
			//init
			m_IsLeaseExp	=	FALSE;
			m_AdpIdx			=	GrSockV2NetIfAdaptarIdGet( m_DevIdx );
			m_MacAdr			=	0;
			GrSockV2GetMacAddr( m_DevIdx, (__u8*)&m_MacAdr );
			m_TimeOutCnt	=	0;		//reset time out counter
			m_RetryCnt		=	0;		//reset retry count
			m_SendId			=	(__u32)((m_MacAdr >> 16) & 0xFFFFFFFFLL);
			m_RsltIp			=	0;
			m_RsltDns			=	0;
			m_RsltMask		=	0;
			m_RsltGate		=	0;
			m_RsltTime		=	0;
			m_SvrId				=	0;
			m_LeaseLim		=	0;

			m_DhcpStage		=	E_GrNetPlugDhcpStageNone;

			//init send packet
			*((__u64*)m_SendPkt.Msg.MacAddr)	=	m_MacAdr;

			//send address init
			GrDumyZeroMem( &m_SendAddr, sizeof(m_SendAddr) );
#ifdef LINUX_APP
			m_SendAddr.Family			= AF_PACKET;
#endif
			m_SendAddr.Protocol		= E_GrNetPlugDhcpSockPrtcIp;
			m_SendAddr.IfIdx			= m_AdpIdx;
			m_SendAddr.AdrLen			= E_GrNetPlugDhcpMacAdrLen;
			m_SendAddr.AdrBuf[0]	=	0xFF;
			m_SendAddr.AdrBuf[1]	=	0xFF;
			m_SendAddr.AdrBuf[2]	=	0xFF;
			m_SendAddr.AdrBuf[3]	=	0xFF;
			m_SendAddr.AdrBuf[4]	=	0xFF;
			m_SendAddr.AdrBuf[5]	=	0xFF;

			//make socket
#ifdef LINUX_APP
			//receive socket
#ifndef E_GrNetPlugDhcpRcvUdp
			m_HndlRecv = socket( PF_PACKET, SOCK_DGRAM, E_GrNetPlugDhcpSockPrtcIp );
			if ( E_GrSockBadHandle != m_HndlRecv )
			{
				//non blocking mode set
				Tv_Atb	=	fcntl(m_HndlRecv, F_GETFL, 0 );
				Tv_Atb	=	Tv_Atb | O_NONBLOCK;
				fcntl(m_HndlRecv, F_SETFL, Tv_Atb);
				//address bind
				GrDumyZeroMem( &m_RecvAddr, sizeof(m_RecvAddr) );
				m_RecvAddr.Family		=	AF_PACKET;
				m_RecvAddr.Protocol	=	E_GrNetPlugDhcpSockPrtcIp;
				m_RecvAddr.IfIdx		=	m_AdpIdx;
				if ( 0 > bind(m_HndlRecv, (struct sockaddr*)&m_RecvAddr, sizeof(m_RecvAddr)) )
				{
					GrSockV2Close( m_HndlRecv );
					m_HndlRecv	=	E_GrSockBadHandle;
				}
			}
#endif
			//send socket
			m_HndlSend	= socket( AF_PACKET, SOCK_DGRAM, E_GrNetPlugDhcpSockPrtcIp );
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

#ifdef E_GrNetPlugDhcpRcvUdp
			//receive socket make
			m_HndlRecv	=	GrSockOpen( TRUE, TRUE );
			if ( E_GrSockBadHandle != m_HndlRecv )
			{
				//set port address
				GrSockV2UdpPortAssign( m_HndlRecv, E_GrNetPlugDhcpPortClient );
			}
#endif
			//check success
			if ( (E_GrSockBadHandle != m_HndlRecv) && (E_GrSockBadHandle != m_HndlSend) )
			{
				//set stage
				m_DhcpStage	=	E_GrNetPlugDhcpStageDiscv;
				//success
				Tv_Result		=	TRUE;
			}
			else
			{
				//release socket
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
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugDhcp::DhcpReceive(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrNetPlugDhcpStageOk == m_DhcpStage )
		{
			if ( 0 != m_RsltIp )
			{
				// update result
				((Cls_GrNetSvs*)m_NetSvs)->RtlV4NetAdrSet( m_RsltIp, m_RsltMask, m_RsltGate );
			}
			Tv_Result	=	TRUE;
			m_DhcpStage	=	E_GrNetPlugDhcpStageNone;
		}
		else if ( E_GrNetPlugDhcpStageNone == m_DhcpStage )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugDhcp::LcSendPkt( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		//calculate checksum
		m_SendPkt.Msg.SendId		=	m_SendId;
		m_SendPkt.UdpHd.ChkSum	=	0;
		m_SendPkt.UdpHd.ChkSum	=	GrSockV2CalcIpV4Chksum( &m_SendPkt.UdpHd , 
			sizeof(St_GrSockUdpHead) + sizeof(St_GrNetPlugDhcpPktMsg), &m_CsVtHd, sizeof(m_CsVtHd) );
#ifdef LINUX_APP
		if ( 0 < sendto( m_HndlSend, &m_SendPkt, sizeof(m_SendPkt), 0, (sockaddr*)&m_SendAddr, sizeof(m_SendAddr) ) )
#else
		if ( 0 < sendto( m_HndlSend, (char*)&m_SendPkt, sizeof(m_SendPkt), 0, (sockaddr*)&m_SendAddr, sizeof(m_SendAddr) ) )
#endif
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugDhcp::LcStageDiscv( void )
{
	// local -------------------
		__u8*	Tv_PtrSr;
	// code --------------------
		//set ip
		m_SendPkt.IpHd.SrcIp			=	INADDR_ANY;
		m_SendPkt.IpHd.DstIp			=	INADDR_BROADCAST;

		//reset option
		GrDumyZeroMem( m_SendPkt.Msg.Options, E_GrNetPlugDhcpDhcpOptLen );
		//discover option
		m_SendPkt.Msg.Options[0]	=	E_GrNetPlugDhcpOptCodeMsgType;
		m_SendPkt.Msg.Options[1]	=	1;
		m_SendPkt.Msg.Options[2]	=	E_GrNetPlugDhcpOptTypeDiscover;
		//client identifier
		m_SendPkt.Msg.Options[3]	=	E_GrNetPlugDhcpOptCodeCliId;
		m_SendPkt.Msg.Options[4]	=	7;
		m_SendPkt.Msg.Options[5]	=	1;		//ETHERNET type
		Tv_PtrSr	=	(__u8*)&m_MacAdr;
		m_SendPkt.Msg.Options[6]	=	Tv_PtrSr[0];
		m_SendPkt.Msg.Options[7]	=	Tv_PtrSr[1];
		m_SendPkt.Msg.Options[8]	=	Tv_PtrSr[2];
		m_SendPkt.Msg.Options[9]	=	Tv_PtrSr[3];
		m_SendPkt.Msg.Options[10]	=	Tv_PtrSr[4];
		m_SendPkt.Msg.Options[11]	=	Tv_PtrSr[5];
		//class identifier
		m_SendPkt.Msg.Options[12]	=	E_GrNetPlugDhcpOptCodeClassId;
		m_SendPkt.Msg.Options[13]	=	5;
		m_SendPkt.Msg.Options[14]	=	'G';
		m_SendPkt.Msg.Options[15]	=	'A';
		m_SendPkt.Msg.Options[16]	=	'U';
		m_SendPkt.Msg.Options[17]	=	'S';
		m_SendPkt.Msg.Options[18]	=	'E';
		//request list
		m_SendPkt.Msg.Options[19]	=	E_GrNetPlugDhcpOptCodeReqList;
		m_SendPkt.Msg.Options[20]	=	3;
		m_SendPkt.Msg.Options[21]	=	E_GrNetPlugDhcpOptCodeNetMask;
		m_SendPkt.Msg.Options[22]	=	E_GrNetPlugDhcpOptCodeRouter;
		m_SendPkt.Msg.Options[23]	=	E_GrNetPlugDhcpOptCodeDnsServer;
		//end
		m_SendPkt.Msg.Options[24]	=	E_GrNetPlugDhcpOptCodeEnd;

		//send request
		if ( LcSendPkt() )
		{
			DbgMsgPrint("Cls_GrNetPlugDhcp::LcStageDiscv - send discover..\n");
			//go next stage
			m_TimeOutCnt	=	0;	//time out reset
			m_DhcpStage ++;
		}
		else
		{
			DbgMsgPrint("Cls_GrNetPlugDhcp::LcStageDiscv - discover send failure.\n");
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugDhcp::LcRecvPkt( void )
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
				//check usable packet
				if ( (IPPROTO_UDP == m_RecvPkt.IpHd.Protocol) && (0x45 == m_RecvPkt.IpHd.HlenVer) )
				{
					//check port
					if (E_GrNetPlugDhcpPortClient == GrDumyEndian2byteGet( &m_RecvPkt.UdpHd.DstPort) ) 
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
								DbgMsgPrint( "Cls_GrNetPlugDhcp::LcRecvPkt - bad send id = %8X : %8X \n" , m_SendId, m_RecvPkt.Msg.SendId );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrNetPlugDhcp::LcRecvPkt - bad packet length = %8X\n" , Tv_RecvLen );
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugDhcp::LcOptAnalysis( void )
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
		if ( E_GrNetPlugDhcpMsgTypeReply == m_RecvPkt.Msg.MsgType )
		{
			DbgMsgPrint( "Cls_GrNetPlugDhcp::LcOptAnalysis - update value...\n" );
			Tv_IsQuit	=	FALSE;
			Tv_PtrOpt	=	m_RecvPkt.Msg.Options;
			Tv_Pos		=	0;
			while ( E_GrNetPlugDhcpDhcpOptLen > Tv_Pos )
			{
				switch( Tv_PtrOpt[Tv_Pos + E_GrNetPlugDhcpOptOfsCode] )
				{
					case	E_GrNetPlugDhcpOptCodeNetMask:
						Tv_PtrRt		=	(__u8*)&m_RsltMask;
						Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 2];
						Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 3];
						Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 4];
						Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 5];
						DbgMsgPrint( "Cls_GrNetPlugDhcp::LcValueUpdt - Netmask = %d.%d.%d.%d \n", 
							Tv_PtrOpt[Tv_Pos + 2], Tv_PtrOpt[Tv_Pos + 3], Tv_PtrOpt[Tv_Pos + 4], Tv_PtrOpt[Tv_Pos + 5] );
						break;
					case	E_GrNetPlugDhcpOptCodeRouter:
						Tv_PtrRt		=	(__u8*)&m_RsltGate;
						Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 2];
						Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 3];
						Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 4];
						Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 5];
						DbgMsgPrint( "Cls_GrNetPlugDhcp::LcValueUpdt - gateway = %d.%d.%d.%d \n", 
							Tv_PtrOpt[Tv_Pos + 2], Tv_PtrOpt[Tv_Pos + 3], Tv_PtrOpt[Tv_Pos + 4], Tv_PtrOpt[Tv_Pos + 5] );
						break;
					case	E_GrNetPlugDhcpOptCodeDnsServer:
						Tv_PtrRt		=	(__u8*)&m_RsltDns;
						Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 2];
						Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 3];
						Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 4];
						Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 5];
						DbgMsgPrint( "Cls_GrNetPlugDhcp::LcValueUpdt - dns = %d.%d.%d.%d \n", 
							Tv_PtrOpt[Tv_Pos + 2], Tv_PtrOpt[Tv_Pos + 3], Tv_PtrOpt[Tv_Pos + 4], Tv_PtrOpt[Tv_Pos + 5] );
						break;
					case	E_GrNetPlugDhcpOptCodeServerId:
						Tv_PtrRt		=	(__u8*)&m_SvrId;
						Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 2];
						Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 3];
						Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 4];
						Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 5];
						DbgMsgPrint( "Cls_GrNetPlugDhcp::LcValueUpdt - server ip = %d.%d.%d.%d \n", 
							Tv_PtrOpt[Tv_Pos + 2], Tv_PtrOpt[Tv_Pos + 3], Tv_PtrOpt[Tv_Pos + 4], Tv_PtrOpt[Tv_Pos + 5] );
						break;
					case	E_GrNetPlugDhcpOptCodeLeaseTm:
						Tv_PtrRt		=	(__u8*)&m_RsltTime;
						Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 5];		//change endian
						Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 4];
						Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 3];
						Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 2];
						DbgMsgPrint( "Cls_GrNetPlugDhcp::LcValueUpdt - lease time = %d sec.\n", m_RsltTime );
						break;
					case	E_GrNetPlugDhcpOptCodeEnd:
						Tv_IsQuit	=	TRUE;
						break;
					case	E_GrNetPlugDhcpOptCodeMsgType:
						//get result
						if ( E_GrNetPlugDhcpOptTypeOffer == Tv_PtrOpt[Tv_Pos + 2] )
						{
							Tv_Result	=	TRUE;
						}
						break;
						/*
					default:
						Tv_Len	=	Tv_PtrOpt[Tv_Pos + E_GrNetPlugDhcpOptOfsLen];
						Tv_Val	=	0;
						Tv_PtrRt		=	(__u8*)&Tv_Val;
						Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 2];		
						Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 3];
						Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 4];
						Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 5];
						DbgMsgPrint("Cls_GrNetPlugDhcp::LcValueUpdt : unknown option code - %d ,len = %d, value - %X .\n",
							Tv_PtrOpt[Tv_Pos + E_GrNetPlugDhcpOptOfsCode], Tv_Len, Tv_Val );
						break;
						*/
				}
				//next
				if ( E_GrNetPlugDhcpOptCodePadding == Tv_PtrOpt[Tv_Pos + E_GrNetPlugDhcpOptOfsCode] )
				{
					//padding
					Tv_Pos ++;
				}
				else
				{
					//normal packet
					Tv_Pos	=	Tv_Pos + (__u32)Tv_PtrOpt[Tv_Pos + E_GrNetPlugDhcpOptOfsLen] + 2;
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
			DbgMsgPrint( "Cls_GrNetPlugDhcp::LcOptAnalysis - bad message type = %d \n", m_RecvPkt.Msg.MsgType ); 
		}

		if ( Tv_Result )
		{
			//get ip
			m_RsltIp	=	m_RecvPkt.Msg.ReqAddr;
			Tv_PtrOpt	=	(__u8*)&m_RsltIp;
			DbgMsgPrint( "Cls_GrNetPlugDhcp::LcOptAnalysis - myip = %d.%d.%d.%d \n", 
				Tv_PtrOpt[0], Tv_PtrOpt[1], Tv_PtrOpt[2], Tv_PtrOpt[3] );
		}
		else
		{
			DbgMsgPrint( "Cls_GrNetPlugDhcp::LcOptAnalysis - bad option msg type. \n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrNetPlugDhcp::LcGetOptType( void )
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_Pos;
		__u8*	Tv_PtrOpt;
		BOOL8	Tv_IsQuit;
	// code --------------------
				Tv_Result	=	0;
		Tv_IsQuit	=	FALSE;
		Tv_PtrOpt	=	m_RecvPkt.Msg.Options;
		Tv_Pos		=	0;
		while ( E_GrNetPlugDhcpDhcpOptLen > Tv_Pos )
		{
			switch( Tv_PtrOpt[Tv_Pos + E_GrNetPlugDhcpOptOfsCode] )
			{
				case	E_GrNetPlugDhcpOptCodeEnd:
					Tv_IsQuit	=	TRUE;
					break;
				case	E_GrNetPlugDhcpOptCodeMsgType:
					//get result
					Tv_Result	=	Tv_PtrOpt[Tv_Pos + 2];
					Tv_IsQuit	=	TRUE;
					break;
					/*
				default:
					Tv_Len	=	Tv_PtrOpt[Tv_Pos + E_GrNetPlugDhcpOptOfsLen];
					Tv_Val	=	0;
					Tv_PtrRt		=	(__u8*)&Tv_Val;
					Tv_PtrRt[0]	=	Tv_PtrOpt[Tv_Pos + 2];		
					Tv_PtrRt[1]	=	Tv_PtrOpt[Tv_Pos + 3];
					Tv_PtrRt[2]	=	Tv_PtrOpt[Tv_Pos + 4];
					Tv_PtrRt[3]	=	Tv_PtrOpt[Tv_Pos + 5];
					DbgMsgPrint("Cls_GrNetPlugDhcp::LcValueUpdt : unknown option code - %d ,len = %d, value - %X .\n",
						Tv_PtrOpt[Tv_Pos + E_GrNetPlugDhcpOptOfsCode], Tv_Len, Tv_Val );
					break;
					*/
			}
			//next
			if ( E_GrNetPlugDhcpOptCodePadding == Tv_PtrOpt[Tv_Pos + E_GrNetPlugDhcpOptOfsCode] )
			{
				//padding
				Tv_Pos ++;
			}
			else
			{
				//normal packet
				Tv_Pos	=	Tv_Pos + (__u32)Tv_PtrOpt[Tv_Pos + E_GrNetPlugDhcpOptOfsLen] + 2;
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
void	Cls_GrNetPlugDhcp::LcStageOffer( void )
{
	// local -------------------
	// code --------------------
#ifdef E_GrNetPlugDhcpRcvUdp
		if ( LcRecvUdpPkt() )
#else
		if ( LcRecvPkt() )
#endif
		{
			//DbgMemDump( m_RecvPkt.Msg.Options, E_GrNetPlugDhcpDhcpOptLen );

			//check option
			if ( LcOptAnalysis() )
			{
				//go next stage
				m_DhcpStage ++;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugDhcp::LcStageReq( void )
{
	// local -------------------
		__u8*	Tv_PtrBt;
	// code --------------------
		//reset option
		GrDumyZeroMem( m_SendPkt.Msg.Options, E_GrNetPlugDhcpDhcpOptLen );
		//discover option
		m_SendPkt.Msg.Options[0]	=	E_GrNetPlugDhcpOptCodeMsgType;
		m_SendPkt.Msg.Options[1]	=	1;
		m_SendPkt.Msg.Options[2]	=	E_GrNetPlugDhcpOptTypeRequest;
		//client identifier
		m_SendPkt.Msg.Options[3]	=	E_GrNetPlugDhcpOptCodeCliId;
		m_SendPkt.Msg.Options[4]	=	7;
		m_SendPkt.Msg.Options[5]	=	1;		//ethernet type
		Tv_PtrBt	=	(__u8*)&m_MacAdr;
		m_SendPkt.Msg.Options[6]	=	Tv_PtrBt[0];
		m_SendPkt.Msg.Options[7]	=	Tv_PtrBt[1];
		m_SendPkt.Msg.Options[8]	=	Tv_PtrBt[2];
		m_SendPkt.Msg.Options[9]	=	Tv_PtrBt[3];
		m_SendPkt.Msg.Options[10]	=	Tv_PtrBt[4];
		m_SendPkt.Msg.Options[11]	=	Tv_PtrBt[5];
		//class identifier
		m_SendPkt.Msg.Options[12]	=	E_GrNetPlugDhcpOptCodeClassId;
		m_SendPkt.Msg.Options[13]	=	5;
		m_SendPkt.Msg.Options[14]	=	'G';
		m_SendPkt.Msg.Options[15]	=	'A';
		m_SendPkt.Msg.Options[16]	=	'U';
		m_SendPkt.Msg.Options[17]	=	'S';
		m_SendPkt.Msg.Options[18]	=	'E';
		//request ip
		m_SendPkt.Msg.Options[19]	=	E_GrNetPlugDhcpOptCodeIpAddr;
		m_SendPkt.Msg.Options[20]	=	4;
		Tv_PtrBt	=	(__u8*)&m_RsltIp;
		m_SendPkt.Msg.Options[21]	=	Tv_PtrBt[0];
		m_SendPkt.Msg.Options[22]	=	Tv_PtrBt[1];
		m_SendPkt.Msg.Options[23]	=	Tv_PtrBt[2];
		m_SendPkt.Msg.Options[24]	=	Tv_PtrBt[3];
		//add server id
		m_SendPkt.Msg.Options[25]	=	E_GrNetPlugDhcpOptCodeServerId;
		m_SendPkt.Msg.Options[26]	=	4;
		Tv_PtrBt	=	(__u8*)&m_SvrId;
		m_SendPkt.Msg.Options[27]	=	Tv_PtrBt[0];
		m_SendPkt.Msg.Options[28]	=	Tv_PtrBt[1];
		m_SendPkt.Msg.Options[29]	=	Tv_PtrBt[2];
		m_SendPkt.Msg.Options[30]	=	Tv_PtrBt[3];
		//request list
		m_SendPkt.Msg.Options[31]	=	E_GrNetPlugDhcpOptCodeReqList;
		m_SendPkt.Msg.Options[32]	=	3;
		m_SendPkt.Msg.Options[33]	=	E_GrNetPlugDhcpOptCodeNetMask;
		m_SendPkt.Msg.Options[34]	=	E_GrNetPlugDhcpOptCodeRouter;
		m_SendPkt.Msg.Options[35]	=	E_GrNetPlugDhcpOptCodeDnsServer;
		//end
		m_SendPkt.Msg.Options[36]	=	E_GrNetPlugDhcpOptCodeEnd;

		//send request
		if ( LcSendPkt() )
		{
			DbgMsgPrint("Cls_GrNetPlugDhcp::LcStageReq - send select..\n");
			//go next stage
			m_TimeOutCnt	=	0;	//time out reset
			m_DhcpStage ++;
		}
		else
		{
			DbgMsgPrint("Cls_GrNetPlugDhcp::LcStageReq - select send failure.\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugDhcp::LcStageAck( void )
{
	// local -------------------
		__u8	Tv_OptType;
	// code --------------------
#ifdef E_GrNetPlugDhcpRcvUdp
		if ( LcRecvUdpPkt() )
#else
		if ( LcRecvPkt() )
#endif
		{
			//check option
			Tv_OptType	=	LcGetOptType();
			if ( E_GrNetPlugDhcpOptTypeAck == Tv_OptType )
			{
				//success
				m_IsLeaseExp	=	TRUE;		//lease expand mode
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
				//go next stage
				m_DhcpStage ++;
				DbgMsgPrint( "Cls_GrNetPlugDhcp::LcStageAck - dhcp completed.\n" );
			}
			else
			{
				DbgMsgPrint( "Cls_GrNetPlugDhcp::LcStageAck - bad opt type - %d.\n", Tv_OptType );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugDhcp::LcFinish( void )
{
	// local -------------------
	// code --------------------
		//stop and reset
		m_RsltIp			=	0;
		m_RsltMask		=	0;
		m_RsltGate		=	0;
		m_RsltDns			=	0;
		m_RsltTime		=	0;
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
void	Cls_GrNetPlugDhcp::LcLeaseExp( void )
{
	// local -------------------
#ifdef LINUX_APP
		__s32			Tv_Atb;
#endif
	// code --------------------
		//init
		m_SpendTime		=	0;
		m_TimeOutCnt	=	0;		//reset time out counter
		m_RetryCnt		=	0;		//reset retry count

		//init send packet
		*((__u64*)m_SendPkt.Msg.MacAddr)	=	m_MacAdr;

		//send address init
		GrDumyZeroMem( &m_SendAddr, sizeof(m_SendAddr) );
#ifdef LINUX_APP
		m_SendAddr.Family			= AF_PACKET;
#endif
		m_SendAddr.Protocol		= E_GrNetPlugDhcpSockPrtcIp;
		m_SendAddr.IfIdx			= m_AdpIdx;
		m_SendAddr.AdrLen			= E_GrNetPlugDhcpMacAdrLen;
		m_SendAddr.AdrBuf[0]	=	0xFF;
		m_SendAddr.AdrBuf[1]	=	0xFF;
		m_SendAddr.AdrBuf[2]	=	0xFF;
		m_SendAddr.AdrBuf[3]	=	0xFF;
		m_SendAddr.AdrBuf[4]	=	0xFF;
		m_SendAddr.AdrBuf[5]	=	0xFF;

		//make socket
#ifdef LINUX_APP
		//recive socket
#ifndef E_GrNetPlugDhcpRcvUdp
		m_HndlRecv = socket( PF_PACKET, SOCK_DGRAM, E_GrNetPlugDhcpSockPrtcIp );
		if ( E_GrSockBadHandle != m_HndlRecv )
		{
			//non blocking mode set
			Tv_Atb	=	fcntl(m_HndlRecv, F_GETFL, 0 );
			Tv_Atb	=	Tv_Atb | O_NONBLOCK;
			fcntl(m_HndlRecv, F_SETFL, Tv_Atb);
			//address bind
			GrDumyZeroMem( &m_RecvAddr, sizeof(m_RecvAddr) );
			m_RecvAddr.Family		=	AF_PACKET;
			m_RecvAddr.Protocol	=	E_GrNetPlugDhcpSockPrtcIp;
			m_RecvAddr.IfIdx		=	m_AdpIdx;
			if ( 0 > bind(m_HndlRecv, (struct sockaddr*)&m_RecvAddr, sizeof(m_RecvAddr)) )
			{
				GrSockV2Close( m_HndlRecv );
				m_HndlRecv	=	E_GrSockBadHandle;
			}
		}
#endif
		//send socket
		m_HndlSend	= socket( AF_PACKET, SOCK_DGRAM, E_GrNetPlugDhcpSockPrtcIp );
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
#ifdef E_GrNetPlugDhcpRcvUdp
		//receive socket make
		m_HndlRecv	=	GrSockOpen( TRUE, TRUE );
		if ( E_GrSockBadHandle != m_HndlRecv )
		{
			//set port address
			GrSockUpdPortAssign( m_HndlRecv, E_GrNetPlugDhcpPortClient );
		}
#endif

		//check success
		if ( (E_GrSockBadHandle != m_HndlRecv) && (E_GrSockBadHandle != m_HndlSend) )
		{
			//set stage
			m_DhcpStage	=	E_GrNetPlugDhcpStageDiscv;
			//m_DhcpStage	=	E_GrNetPlugDhcpStageReq;
			//m_SendPkt.IpHd.SrcIp			=	m_RsltIp;
			//m_SendPkt.IpHd.DstIp			=	m_SvrId;
			DbgMsgPrint("Cls_GrNetPlugDhcp::LcLeaseExp - try lease extend..\n");
			//success
		}
		else
		{
			// release socket
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
			DbgMsgPrint("Cls_GrNetPlugDhcp::LcLeaseExp - socket allocate failure.\n");
		}

}
//--------------------------------------------------------------------
void	Cls_GrNetPlugDhcp::LcStageExpReq( void )
{
	// local -------------------
		__u8*	Tv_PtrBt;
	// code --------------------
		//reset option
		GrDumyZeroMem( m_SendPkt.Msg.Options, E_GrNetPlugDhcpDhcpOptLen );
		//discover option
		m_SendPkt.Msg.Options[0]	=	E_GrNetPlugDhcpOptCodeMsgType;
		m_SendPkt.Msg.Options[1]	=	1;
		m_SendPkt.Msg.Options[2]	=	E_GrNetPlugDhcpOptTypeRequest;
		//client identifier
		m_SendPkt.Msg.Options[3]	=	E_GrNetPlugDhcpOptCodeCliId;
		m_SendPkt.Msg.Options[4]	=	7;
		m_SendPkt.Msg.Options[5]	=	1;		//ETHERNET type
		Tv_PtrBt	=	(__u8*)&m_MacAdr;
		m_SendPkt.Msg.Options[6]	=	Tv_PtrBt[0];
		m_SendPkt.Msg.Options[7]	=	Tv_PtrBt[1];
		m_SendPkt.Msg.Options[8]	=	Tv_PtrBt[2];
		m_SendPkt.Msg.Options[9]	=	Tv_PtrBt[3];
		m_SendPkt.Msg.Options[10]	=	Tv_PtrBt[4];
		m_SendPkt.Msg.Options[11]	=	Tv_PtrBt[5];
		//class identifier
		m_SendPkt.Msg.Options[12]	=	E_GrNetPlugDhcpOptCodeClassId;
		m_SendPkt.Msg.Options[13]	=	5;
		m_SendPkt.Msg.Options[14]	=	'G';
		m_SendPkt.Msg.Options[15]	=	'A';
		m_SendPkt.Msg.Options[16]	=	'U';
		m_SendPkt.Msg.Options[17]	=	'S';
		m_SendPkt.Msg.Options[18]	=	'E';
		//request ip
		m_SendPkt.Msg.Options[19]	=	E_GrNetPlugDhcpOptCodeIpAddr;
		m_SendPkt.Msg.Options[20]	=	4;
		Tv_PtrBt	=	(__u8*)&m_RsltIp;
		m_SendPkt.Msg.Options[21]	=	Tv_PtrBt[0];
		m_SendPkt.Msg.Options[22]	=	Tv_PtrBt[1];
		m_SendPkt.Msg.Options[23]	=	Tv_PtrBt[2];
		m_SendPkt.Msg.Options[24]	=	Tv_PtrBt[3];
		//add server id
		m_SendPkt.Msg.Options[25]	=	E_GrNetPlugDhcpOptCodeServerId;
		m_SendPkt.Msg.Options[26]	=	4;
		Tv_PtrBt	=	(__u8*)&m_SvrId;
		m_SendPkt.Msg.Options[27]	=	Tv_PtrBt[0];
		m_SendPkt.Msg.Options[28]	=	Tv_PtrBt[1];
		m_SendPkt.Msg.Options[29]	=	Tv_PtrBt[2];
		m_SendPkt.Msg.Options[30]	=	Tv_PtrBt[3];
		//request list
		m_SendPkt.Msg.Options[31]	=	E_GrNetPlugDhcpOptCodeReqList;
		m_SendPkt.Msg.Options[32]	=	3;
		m_SendPkt.Msg.Options[33]	=	E_GrNetPlugDhcpOptCodeNetMask;
		m_SendPkt.Msg.Options[34]	=	E_GrNetPlugDhcpOptCodeRouter;
		m_SendPkt.Msg.Options[35]	=	E_GrNetPlugDhcpOptCodeDnsServer;
		//end
		m_SendPkt.Msg.Options[36]	=	E_GrNetPlugDhcpOptCodeEnd;

		//send request
		if ( 0 != GrSockV2UdpSend( m_HndlSend, &m_SvrId, E_GrNetPlugDhcpPortServer, &m_SendPkt.Msg, sizeof(St_GrNetPlugDhcpPktMsg), FALSE ) )
		{
			DbgMsgPrint("Cls_GrNetPlugDhcp::LcStageExpReq - send lease extend reqset..\n");
			//go next stage
			m_TimeOutCnt	=	0;	//time out reset
			m_DhcpStage ++;
		}
		else
		{
			DbgMsgPrint("Cls_GrNetPlugDhcp::LcStageExpReq - lease extend  send failure.\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugDhcp::LcStageExpAck( void )
{
	// local -------------------
		__u32	Tv_Ip;
		__u16	Tv_Port;
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		Tv_Ip		=	m_SvrId;
		if(0 != GrSockV2UdpRead(m_HndlRecv, &Tv_Ip, &Tv_Port, &m_RecvPkt.Msg, sizeof(St_GrNetPlugDhcpPktMsg), FALSE, &Tv_ErrCode))
		{
			//check match data
			if ( (m_SvrId == Tv_Ip) && (m_SendId == m_RecvPkt.Msg.SendId) )
			{
				//check option
				if ( E_GrNetPlugDhcpOptTypeAck == LcGetOptType() )
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
					m_DhcpStage	=	E_GrNetPlugDhcpStageNone;
					DbgMsgPrint( "Cls_GrNetPlugDhcp::LcStageExpAck - lease extend completed.\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrNetPlugDhcp::LcStageExpAck - recive bad msg.\n" );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugDhcp::LcRecvUdpPkt( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Readed;
		__u32	Tv_Ip;
		__u16	Tv_Port;
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Readed	=	GrSockV2UdpRead(m_HndlRecv, &Tv_Ip, &Tv_Port, &m_RecvPkt.Msg, sizeof(St_GrNetPlugDhcpPktMsg), FALSE, &Tv_ErrCode);
		if ( 0 < Tv_Readed )
		{
			//check sent
			if ( m_SendId == m_RecvPkt.Msg.SendId )
			{
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrNetPlugDhcp::LcRecvUdpPkt - send id mismath - %d : %d  pktsize= %d  ip=%08X.\n" , 
					m_SendId, m_RecvPkt.Msg.SendId, Tv_Readed, m_SvUdpIp );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

