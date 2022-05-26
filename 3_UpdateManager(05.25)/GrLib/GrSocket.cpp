/*
	socket library
		ip struct : 192.168.0.1   [0]192[1]168[2]0[3]1
*/

#ifndef		LINUX_APP
	#include <WinSock2.h>
#endif
#include	<GrTypeBase.h>
#include	<GrSocket.h>
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
	#include	<sys/time.h> 
	#include	<sys/types.h> 
	#include	<unistd.h> 
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

#endif

#include <GrStrTool.h>
#include <GrFileCtrl.h>
#include <GrFileTool.h>
#include <GrTimeTool.h>
#include <GrDumyTool.h>
#include <GrDebug.h>
#include <GrTaskSvr.h>
#include <GrDnsCli.h>
#include <GrDhcpCli.h>
#include <GrFdnsCli.h>

//====================================================================
//local const
#ifdef LINUX_APP
#define E_GrSockStrDnsFile		"/etc/resolv.conf"
#else
#define E_GrSockStrDnsFile		"e:\\EmuGh\\Ramdisk\\etc\\resolv.conf"
#endif
#define E_GrSockStrFnNtpCli		"/ntpclient"
#define E_GrSockStrCmdNtpCli	"/ntpclient -c 1 -s -h "

#define	E_GrSockTaskFcc				Mac_GrMakeFcc('S','O','C','K')

#define E_GrSockNtpTimeBase		0x3680C02EB400LL

//====================================================================
//local type

//====================================================================
//global var

__u32	V_GrSocketInitCnt		=	0;		//initialize count
Cls_GrTaskSvr*	V_GrSocketTask	=	NULL;
Cls_GrDnsCli*		V_GrSocketDnsCli	=	NULL;
Cls_GrDhcpCli*	V_GrSocketDhcpCli	= NULL;
Cls_GrFdnsCli*	V_GrSocketFdnsCli	=	NULL;

//====================================================================
//functions

//--------------------------------------------------------------------
__u32		GrSockGetErrCode( void )
{
	// local -------------------
		__u32		Tv_Result;
		__s32		Tv_Err;
	// code --------------------
		//init
		Tv_Result		=	E_GrSockErrNone;

		#ifdef LINUX_APP
			Tv_Err	=	errno;
			switch( Tv_Err )
			{
				case EWOULDBLOCK:
					Tv_Result		=	E_GrSockErrWouldblock;
					break;
				case	EPIPE:
					Tv_Result		=	E_GrSockErrConnBroken;
					DbgMsgPrint("GrSockGetErrCode: epipe broken.\n" );
					break;
				case	ECONNRESET:
					Tv_Result		=	E_GrSockErrConnBroken;
					DbgMsgPrint("GrSockGetErrCode: reset peer conn.\n" );
					break;
				default:
					DbgMsgPrint("GrSockGetErrCode: unknown error code = %d\n", Tv_Err );
					Tv_Result		=	E_GrSockErrEtc;
			    break;
			}
		#else
			Tv_Err	=	WSAGetLastError();
			switch ( Tv_Err )
			{
				case WSAEWOULDBLOCK:
					Tv_Result		=	E_GrSockErrWouldblock;
					break;
				default:
					Tv_Result		=	E_GrSockErrEtc;
			    break;
			}
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrSockInit( void )
{
	// local -------------------
		#ifndef LINUX_APP
			WSADATA		Tv_WsaData;
		#endif
	// code --------------------
		#ifdef LINUX_APP
			sigset(SIGPIPE, SIG_IGN);
		#else
			WSAStartup(MAKEWORD(1,0), &Tv_WsaData);
		#endif
			//init control
			if ( (0 == V_GrSocketInitCnt) && (NULL == V_GrSocketTask) )
			{
				//create task
				V_GrSocketTask	=	(Cls_GrTaskSvr*)new Cls_GrTaskSvr( E_GrSockTaskFcc );
				//regist dns
				V_GrSocketDnsCli	=	(Cls_GrDnsCli*) new Cls_GrDnsCli( V_GrSocketTask );
				//register dhcp
				V_GrSocketDhcpCli	=	(Cls_GrDhcpCli*) new Cls_GrDhcpCli( V_GrSocketTask );
				//register fdns
				V_GrSocketFdnsCli	=	(Cls_GrFdnsCli*) new Cls_GrFdnsCli( V_GrSocketTask );
			}
			V_GrSocketInitCnt ++;
}
//--------------------------------------------------------------------
void		GrSockFinish( void )
{
		#ifndef	LINUX_APP
			WSACleanup();
		#endif
			V_GrSocketInitCnt --;
			if ( (0 == V_GrSocketInitCnt) && (NULL != V_GrSocketTask) )
			{
				//release fdns
				delete	V_GrSocketFdnsCli;
				V_GrSocketFdnsCli	=	NULL;
				//release dhcp
				delete	V_GrSocketDhcpCli;
				V_GrSocketDhcpCli	=	NULL;
				//release dns
				delete	V_GrSocketDnsCli;
				V_GrSocketDnsCli	=	NULL;
				//release task
				delete	V_GrSocketTask;
				V_GrSocketTask	=	NULL;
			}
}
//--------------------------------------------------------------------
Def_GrSock	GrSockOpen( BOOL8 A_IsUdp, BOOL8 A_IsNonblock )
{
	// local -------------------
		Def_GrSock	Tv_Result;
		__s32				Tv_Type;
		#ifdef LINUX_APP
			__s32			Tv_Atb;
		#else
			__u32		Tv_Val;
		#endif
	// code --------------------
		Tv_Type			=	SOCK_STREAM;
		if ( A_IsUdp )
		{
			Tv_Type			=	SOCK_DGRAM;
		}
		Tv_Result		=	socket( AF_INET, Tv_Type, 0 );
		if ( E_GrSockBadHandle != Tv_Result )
		{
			if ( A_IsNonblock )
			{
				//set non blocking socket
				#ifdef LINUX_APP
					Tv_Atb	=	fcntl(Tv_Result, F_GETFL, 0 );
					Tv_Atb	=	Tv_Atb | O_NONBLOCK;
					fcntl(Tv_Result, F_SETFL, Tv_Atb);
				#else
					Tv_Val		=	TRUE;
					ioctlsocket( Tv_Result, FIONBIO, (u_long*)&Tv_Val );
				#endif
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrSockSetBlockMode( Def_GrSock A_Hndl, BOOL8 A_IsNonBlock )
{
	// local -------------------
#ifdef LINUX_APP
		__s32			Tv_Atb;
#else
		__u32		Tv_Val;
#endif
	// code --------------------

		if ( A_IsNonBlock )
		{
			//set non blocking socket
#ifdef LINUX_APP
			Tv_Atb	=	fcntl(A_Hndl, F_GETFL, 0 );
			Tv_Atb	=	Tv_Atb | O_NONBLOCK;
			fcntl(A_Hndl, F_SETFL, Tv_Atb);
#else
			Tv_Val		=	TRUE;
			ioctlsocket( A_Hndl, FIONBIO, (u_long*)&Tv_Val );
#endif
		}
		else
		{
			//set blocking socket
#ifdef LINUX_APP
			Tv_Atb	=	fcntl(A_Hndl, F_GETFL, 0 );
			Tv_Atb	=	Tv_Atb & ~O_NONBLOCK;
			fcntl(A_Hndl, F_SETFL, Tv_Atb);
#else
			Tv_Val		=	FALSE;
			ioctlsocket( A_Hndl, FIONBIO, (u_long*)&Tv_Val );
#endif
		}
}
//--------------------------------------------------------------------
BOOL8		GrSockUdpPortAssign( Def_GrSock A_Hndl, __u16 A_Port )
{
	// local -------------------
		BOOL8		Tv_Result;
		sockaddr_in	Tv_NetAdr;
		BOOL8		Tv_IsReuse;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//set mode
		Tv_IsReuse	=	TRUE;
		setsockopt( A_Hndl, SOL_SOCKET, SO_REUSEADDR, (char*)&Tv_IsReuse, sizeof(Tv_IsReuse) );
		Tv_NetAdr.sin_family			=	AF_INET;
		Tv_NetAdr.sin_addr.s_addr	=	htonl(INADDR_ANY);
		Tv_NetAdr.sin_port				=	htons(A_Port);
		if ( 0 <= bind( A_Hndl, (sockaddr*)&Tv_NetAdr, sizeof(Tv_NetAdr) ) )
		{
				Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrSockUdpIpAssign( Def_GrSock A_Hndl, __u32 A_Ip )
{
	// local -------------------
		BOOL8		Tv_Result;
		sockaddr_in	Tv_NetAdr;
		BOOL8		Tv_IsReuse;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//set mode
		Tv_IsReuse	=	TRUE;
		setsockopt( A_Hndl, SOL_SOCKET, SO_REUSEADDR, (char*)&Tv_IsReuse, sizeof(Tv_IsReuse) );
		Tv_NetAdr.sin_family			=	AF_INET;
		Tv_NetAdr.sin_addr.s_addr	=	A_Ip;
		Tv_NetAdr.sin_port				=	0;
		if ( 0 <= bind( A_Hndl, (sockaddr*)&Tv_NetAdr, sizeof(Tv_NetAdr) ) )
		{
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrSockSetListenMode( Def_GrSock A_Hndl, __u16 A_Port )
{
	// local -------------------
		BOOL8		Tv_Result;
		sockaddr_in	Tv_NetAdr;
		BOOL8		Tv_IsReuse;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//set mode
		Tv_IsReuse	=	TRUE;
		setsockopt( A_Hndl, SOL_SOCKET, SO_REUSEADDR, (char*)&Tv_IsReuse, sizeof(Tv_IsReuse) );
		Tv_NetAdr.sin_family			=	AF_INET;
		Tv_NetAdr.sin_addr.s_addr	=	htonl(INADDR_ANY);
		Tv_NetAdr.sin_port				=	htons(A_Port);
		if ( 0 <= bind( A_Hndl, (sockaddr*)&Tv_NetAdr, sizeof(Tv_NetAdr) ) )
		{
			if ( 0 <= listen( A_Hndl, 5 ) )
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrSock	GrSockAccept( Def_GrSock A_HndlListen, BOOL8 A_IsNonblock, 
												 __u32* A_PtrRtIp, __u16* A_PtrRtPort )
{
	// local -------------------
		Def_GrSock	Tv_Result;
		sockaddr_in	Tv_AdrConn;
		__s32			Tv_RtSize;
#ifdef LINUX_APP
			__s32			Tv_Atb;
			linger	Tv_Linger;
#else
			__u32		Tv_Val;
#endif
	// code --------------------
		Tv_RtSize		=	sizeof(Tv_AdrConn);
		Tv_Result		=	accept( A_HndlListen, (sockaddr*)&Tv_AdrConn, (socklen_t*)&Tv_RtSize );
		if ( E_GrSockBadHandle != Tv_Result )
		{
			if ( A_IsNonblock )
			{
				//set non blocking socket
				#ifdef LINUX_APP
					Tv_Atb	=	fcntl(Tv_Result, F_GETFL, 0 );
					Tv_Atb	=	Tv_Atb | O_NONBLOCK;
					fcntl(Tv_Result, F_SETFL, Tv_Atb);
					/*
					Tv_Linger.l_onoff		=	TRUE;
					Tv_Linger.l_linger	=	0;
					setsockopt( Tv_Result, SOL_SOCKET, SO_LINGER, &Tv_Linger, sizeof(Tv_Linger) );	//fast close option
					*/
				#else
					Tv_Val		=	TRUE;
					ioctlsocket( Tv_Result, FIONBIO, (u_long*)&Tv_Val );
				#endif
			}

			//get ip address
			*A_PtrRtIp		=	Tv_AdrConn.sin_addr.s_addr;
			*A_PtrRtPort	=	htons(Tv_AdrConn.sin_port);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8		GrSockConnect( Def_GrSock A_Hndl, __u32 A_Ip, __u16 A_Port )
{
	// local -------------------
		__u8		Tv_Result;
		sockaddr_in	Tv_AdrConn;
		__s32			Tv_AdrSize;
		__s32			Tv_Err;
	// code --------------------
		//init
		Tv_Result		=	E_GrSockConnTimeOut;

		//try connetc
		Tv_AdrConn.sin_family				=	AF_INET;
		Tv_AdrConn.sin_addr.s_addr	=	A_Ip;
		Tv_AdrConn.sin_port					=	htons((__u16)A_Port);
		Tv_AdrSize		=	sizeof(Tv_AdrConn);
		if ( 0 <= connect( A_Hndl, (sockaddr*)&Tv_AdrConn, Tv_AdrSize ) )
		{
			Tv_Result		=	E_GrSockConnOk;
		}
		else
		{
#ifdef LINUX_APP
			Tv_Err	=	errno;
			if ( (EINPROGRESS == Tv_Err) || (EALREADY == Tv_Err) )
			{
				Tv_Result	=	E_GrSockConnWaiting;
			}
			else if ( (EISCONN == Tv_Err) )
			{
				Tv_Result		=	E_GrSockConnOk;
			}
			else
			{
				DbgMsgPrint( "GrSockConnect - connect error - ip=%8X , err = %d \n", A_Ip, Tv_Err );
			}
#else
			Tv_Err	=	WSAGetLastError();
			if ( (WSAEWOULDBLOCK == Tv_Err)  )
			{
				Tv_Result	=	E_GrSockConnWaiting;
			}
			else if ( WSAEISCONN == Tv_Err )
			{
				Tv_Result		=	E_GrSockConnOk;
			}
			else
			{
				DbgMsgPrint( "GrSockConnect - connect error - ip=%8X , err = %d \n", A_Ip, Tv_Err );
			}
#endif
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64		GrSockGetMacAddr( __u8 A_DevIdx )
{
	// local -------------------
		__u64		Tv_Result;
		#ifdef LINUX_APP
			__s32			Tv_Hndl;
			struct ifreq	Tv_IfReq; 
			__u8*		Tv_PtrData;
			__u32		Tv_WkIdx;
			__u8*		Tv_PtrTg;
		#endif
	// code --------------------
		//init
		Tv_Result		=	0xFFFFFFFFFFFFLL;
		
		#ifdef LINUX_APP
			Tv_Hndl		=	socket( AF_INET, SOCK_STREAM, 0 );
			if ( 0 <= Tv_Hndl )
			{
				GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
				Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
				if ( 0 <= ioctl( Tv_Hndl, SIOCGIFHWADDR, &Tv_IfReq ) )
				{
					Tv_PtrData		=	(__u8*)( &Tv_IfReq.ifr_hwaddr.sa_data );
					Tv_Result			=	0;
					Tv_PtrTg			=	(__u8*)&Tv_Result;
					for ( Tv_WkIdx=0;Tv_WkIdx < 6;Tv_WkIdx++ )
					{
						*Tv_PtrTg	=	*Tv_PtrData;
						Tv_PtrData ++;
						Tv_PtrTg ++;
					}
				}
				close( Tv_Hndl );
			}
			
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrSockSetMacAddr( __u8 A_DevIdx, void* A_PtrMac )
{
	// local -------------------
	#ifdef LINUX_APP
		__s32			Tv_Hndl;
		struct ifreq	Tv_IfReq; 
		__u8*		Tv_PtrData;
		__u32		Tv_WkIdx;
		__u8*		Tv_PtrTg;
	#endif
	// code --------------------
	#ifdef LINUX_APP
		Tv_Hndl		=	socket( AF_INET, SOCK_STREAM, 0 );
		if ( 0 <= Tv_Hndl )
		{
			GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
			GrDumyCopyMem( Tv_IfReq.ifr_hwaddr.sa_data, A_PtrMac, 6 );
			Tv_IfReq.ifr_hwaddr.sa_family = ARPHRD_ETHER;

			if ( 0 > ioctl( Tv_Hndl, SIOCSIFHWADDR, &Tv_IfReq ) )
			{
				DbgMsgPrint( "GrSockSetMacAddr - change mac addres failure\n" );
			}
			close( Tv_Hndl );
		}

	#endif
}
//--------------------------------------------------------------------
/*
BOOL8		GrSockGetIpByName( char* A_StrAdr, __u32* A_PtrRtIp )
{
	// local -------------------
		BOOL8			Tv_Result;
		hostent*	Tv_PtrEnt;
		char**		Tv_PtrAdr;
		__u32*		Tv_PtrIp;
#ifdef LINUX_APP
		char			Tv_Buf[1024];
		hostent		Tv_EntTmp;
		__s32				Tv_Err;
		__s32				Tv_Rt;
#endif
	// code --------------------
		//init
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
		Tv_Rt	=	gethostbyname_r( A_StrAdr, &Tv_EntTmp, Tv_Buf, sizeof(Tv_Buf), &Tv_PtrEnt, &Tv_Err );
		if ( NULL != Tv_PtrEnt )
		{
			Tv_PtrAdr		=	Tv_PtrEnt->h_addr_list;
			Tv_PtrIp		=	(__u32*)*Tv_PtrAdr;
			*A_PtrRtIp	=	*Tv_PtrIp;
			Tv_Result		=	TRUE;
		}
		else
		{
			DbgMsgPrint( "GrSockGetIpByName: error- %s \n", hstrerror(h_errno) );
		}
#else
		Tv_PtrEnt	=	gethostbyname( A_StrAdr );
		if ( NULL != Tv_PtrEnt )
		{
			Tv_PtrAdr	=	Tv_PtrEnt->h_addr_list;
			if ( NULL != *Tv_PtrAdr )
			{
				Tv_PtrIp		=	(__u32*)*Tv_PtrAdr;
				*A_PtrRtIp	=	*Tv_PtrIp;
				Tv_Result		=	TRUE;
			}
		}
#endif

		return	Tv_Result;
}
*/
//--------------------------------------------------------------------
__u32		GrSockUdpSend( Def_GrSock A_Hndl, __u32 A_Ip, __u16 A_Port, void* A_PtrData, __u32 A_SizeData )
{
	// local -------------------
		__s32			Tv_Result;
		sockaddr_in	Tv_AdrConn;
		__u32		Tv_AdrSize;
	// code --------------------
		//set address
		Tv_AdrConn.sin_family				=	AF_INET;
		Tv_AdrConn.sin_addr.s_addr	=	A_Ip;
		Tv_AdrConn.sin_port					=	htons((__u16)A_Port);
		Tv_AdrSize		=	sizeof(Tv_AdrConn);
		
		#ifdef LINUX_APP
			Tv_Result		=	sendto( A_Hndl, A_PtrData, (size_t)A_SizeData, 0, 
										(sockaddr*)&Tv_AdrConn, (socklen_t)Tv_AdrSize );
		#else
			Tv_Result		=	sendto( A_Hndl, (char*)A_PtrData, (__s32)A_SizeData, 0, 
										(sockaddr*)&Tv_AdrConn, (__s32)Tv_AdrSize );
		#endif

		//patch result
		if ( 0 > Tv_Result )
		{
			Tv_Result		=	0;
		}
		
		return	(__u32)Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrSockUdpRead( Def_GrSock A_Hndl, __u32* A_PtrIp, __u16 A_Port, void* A_PtrData, __u32 A_SizeData )
{
	// local -------------------
		__s32			Tv_Result;
		sockaddr_in	Tv_AdrConn;
		socklen_t		Tv_AdrSize;
	// code --------------------
		//set address
		Tv_AdrConn.sin_family				=	AF_INET;
		Tv_AdrConn.sin_addr.s_addr	=	*A_PtrIp;
		Tv_AdrConn.sin_port					=	htons((__u16)A_Port);
		Tv_AdrSize		=	sizeof(Tv_AdrConn);

		#ifdef LINUX_APP
			Tv_Result		=	recvfrom( A_Hndl, A_PtrData, (size_t)A_SizeData, 0, 
										(sockaddr*)&Tv_AdrConn, &Tv_AdrSize );
		#else
			Tv_Result		=	recvfrom( A_Hndl, (char*)A_PtrData, (__s32)A_SizeData, 0, 
										(sockaddr*)&Tv_AdrConn, (int*)&Tv_AdrSize );
		#endif

		//patch result
		if ( 0 > Tv_Result )
		{
			Tv_Result		=	0;
		}
		else
		{
			//update ip
			*A_PtrIp		=	Tv_AdrConn.sin_addr.s_addr;
		}

		return	(__u32)Tv_Result;
}
//--------------------------------------------------------------------
void	GrSockSetDns( __u32 A_Dns )
{
	// local -------------------
		Cls_GrFileCtrl*	Tv_ObjFile;
		Def_StrTag	Tv_StrCmd;
		Def_StrName	Tv_StrIp;
		__u32		Tv_LenCmd;
	// code --------------------
		//delete old dns
		GrFileDelete( E_GrSockStrDnsFile, TRUE );
		Tv_ObjFile	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( E_GrSockStrDnsFile, TRUE, TRUE, FALSE, TRUE, TRUE );
		if ( Tv_ObjFile->IsOpened() )
		{
			GrStrCopy( Tv_StrCmd, "nameserver " );
			GrStrIpToStr( Tv_StrIp, A_Dns );
			GrStrCat( Tv_StrCmd, Tv_StrIp );
			GrStrCat( Tv_StrCmd, "\n" );
			Tv_LenCmd		=	GrStrLen( Tv_StrCmd );
			Tv_ObjFile->Seek( 0 );
			Tv_ObjFile->Write( (void*)Tv_StrCmd, Tv_LenCmd );
		}
		delete	Tv_ObjFile;		
#ifdef LINUX_APP
		res_init();
#endif
		//update dns client
#if 0
		if ( NULL != V_GrSocketDnsCli )
		{
			V_GrSocketDnsCli->Setup( A_Dns );
		}
#endif
}
//--------------------------------------------------------------------
BOOL8	GrSockSetupByDhcp( __u8 A_NetIf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Ip;
		__u32	Tv_Mask;
		__u32	Tv_Gate;
		__u32	Tv_Dns;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( V_GrSocketDhcpCli->Request( A_NetIf, FALSE, FALSE ) )
		{
			Tv_Ip		=	0;
			while ( !V_GrSocketDhcpCli->GetResult( &Tv_Ip, &Tv_Mask, &Tv_Gate, &Tv_Dns ) )
			{
				GrPrcSleep( 10 );
			}
			//check able
			if ( 0 != Tv_Ip )
			{
				GrSockSetNetAdr( A_NetIf, Tv_Ip, Tv_Mask, Tv_Gate );
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrSockGetMyIp( __u8 A_DevIdx )
{
	// local -------------------
		__u32		Tv_Result;
#ifdef LINUX_APP
		__s32			Tv_Hndl;
		struct ifreq	Tv_IfReq; 
		__u32		Tv_WkIdx;
		struct sockaddr_in*	Tv_PtrIpAdr;
#else
		Def_StrTag	Tv_StrName;
		hostent*	Tv_PtrEnt;
		char**		Tv_PtrAdr;
		__u32*		Tv_PtrIp;
#endif
	// code --------------------
		//init
		Tv_Result		=	0;

#ifdef LINUX_APP
		Tv_Hndl		=	socket( AF_INET, SOCK_STREAM, 0 );

		if ( 0 <= Tv_Hndl )
		{
			GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
			if ( 0 <= ioctl( Tv_Hndl, SIOCGIFADDR, &Tv_IfReq ) )
			{
				Tv_PtrIpAdr		=	(struct sockaddr_in*)&Tv_IfReq.ifr_ifru.ifru_addr;
				Tv_Result			=	Tv_PtrIpAdr->sin_addr.s_addr;
			}
			close( Tv_Hndl );
		}
#else
		gethostname( Tv_StrName, sizeof(Tv_StrName) - 1 );
		Tv_PtrEnt	=	gethostbyname( Tv_StrName );
		if ( NULL != Tv_PtrEnt )
		{
			Tv_PtrAdr	=	Tv_PtrEnt->h_addr_list;
			if ( NULL != *Tv_PtrAdr )
			{
				Tv_PtrIp		=	(__u32*)*Tv_PtrAdr;
				Tv_Result		=	*Tv_PtrIp;
			}
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrSockGetMyNetmask( __u8 A_DevIdx )
{
	// local -------------------
		__u32		Tv_Result;
#ifdef LINUX_APP
		__s32			Tv_Hndl;
		struct ifreq	Tv_IfReq; 
		__u32		Tv_WkIdx;
		struct sockaddr_in*	Tv_PtrIpAdr;
#endif
	// code --------------------
		//init
		Tv_Result		=	0;

	#ifdef LINUX_APP
		Tv_Hndl		=	socket( AF_INET, SOCK_STREAM, 0 );

		if ( 0 <= Tv_Hndl )
		{
			GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
			if ( 0 <= ioctl( Tv_Hndl, SIOCGIFNETMASK, &Tv_IfReq ) )
			{
				Tv_PtrIpAdr		=	(struct sockaddr_in*)&Tv_IfReq.ifr_ifru.ifru_addr;
				Tv_Result			=	Tv_PtrIpAdr->sin_addr.s_addr;
			}
			close( Tv_Hndl );
		}
	#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrSockIsLanCableOk( __s8 A_ConnectorIdx )
{
#ifdef LINUX_APP
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Sock;
		struct  ifreq	Tv_IfReq;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Sock	=	socket(AF_INET, SOCK_DGRAM,0);
		if ( 0 <= Tv_Sock )
		{
			//make name
			GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_ConnectorIdx;
			if ( ioctl(Tv_Sock, SIOCGIFFLAGS, &Tv_IfReq) >= 0 )
			{
				//check on
				if ( 0 != ( Tv_IfReq.ifr_flags & IFF_RUNNING ) )
				{
					Tv_Result	=	TRUE;
				}
			}

			//close socket
			close( Tv_Sock );
		}

		return	Tv_Result;
#else
		return	TRUE;
#endif
}
//--------------------------------------------------------------------
BOOL8		GrSockNetIfActive( __s8 A_DevIdx, BOOL8 A_IsOn )
{
#ifdef LINUX_APP
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Sock;
		struct  ifreq	Tv_IfReq;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Sock	=	socket(AF_INET, SOCK_DGRAM,0);
		if ( 0 <= Tv_Sock )
		{
			//make name
			GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
			if ( ioctl(Tv_Sock, SIOCGIFFLAGS, &Tv_IfReq) >= 0 )
			{
				if ( A_IsOn )
				{
					Tv_IfReq.ifr_flags |= (IFF_UP | IFF_RUNNING);
				}
				else
				{
					Tv_IfReq.ifr_flags &= ~IFF_UP;
				}
				GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
				Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
				ioctl(Tv_Sock, SIOCSIFFLAGS, &Tv_IfReq);
				Tv_Result	=	TRUE;
			}

			//close socket
			close( Tv_Sock );
		}

		return	Tv_Result;
#else
	return	TRUE;
#endif
}
//--------------------------------------------------------------------
BOOL8		GrSockNetIfMtuSet( __s8 A_DevIdx, __s32 A_Mtu )
{
	#ifdef LINUX_APP
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Sock;
		struct  ifreq	Tv_IfReq;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Sock	=	socket(AF_INET, SOCK_DGRAM,0);
		if ( 0 <= Tv_Sock )
		{
			//make name
			GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
			Tv_IfReq.ifr_mtu	=	A_Mtu;
			if ( 0 <= ioctl(Tv_Sock, SIOCSIFMTU, &Tv_IfReq) )
			{
				Tv_Result	=	TRUE;
			}
			//close socket
			close( Tv_Sock );
		}

		return	Tv_Result;
	#else
		return	TRUE;
	#endif
}
//--------------------------------------------------------------------
void	GrSockSetNetAdr( __u8 A_DevIdx, __u32 A_Ip, __u32 A_NetMask, __u32 A_GateWay )
{
#ifdef LINUX_APP
	// local -------------------
		__s32		Tv_Sock;
		struct  ifreq	Tv_IfReq;
		struct sockaddr_in*	Tv_PtrIpAdr;
		struct rtentry Tv_Route;
		struct sockaddr_in*	Tv_PtrRtDst;
		struct sockaddr_in*	Tv_PtrRtGw;
		struct sockaddr_in*	Tv_PtrRtMask;
	// code --------------------
		Tv_Sock	=	socket(AF_INET, SOCK_DGRAM,0);
		if ( 0 <= Tv_Sock )
		{
			//ip set
			GrDumyZeroMem( &Tv_IfReq, sizeof(Tv_IfReq) );
			GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
			Tv_PtrIpAdr	=	(struct sockaddr_in*)&Tv_IfReq.ifr_ifru.ifru_addr;
			Tv_PtrIpAdr->sin_family = AF_INET;
			Tv_PtrIpAdr->sin_addr.s_addr = A_Ip;
			Tv_PtrIpAdr->sin_port = 0;
			if ( ioctl(Tv_Sock, SIOCSIFADDR, &Tv_IfReq) < 0 )
			{
				DbgMsgPrint("GrSockSetNetAdr: Ip setting error\n");
			}

			//netmask set
			GrDumyZeroMem( &Tv_IfReq, sizeof(Tv_IfReq) );
			GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
			Tv_PtrIpAdr	=	(struct sockaddr_in*)&Tv_IfReq.ifr_ifru.ifru_addr;
			Tv_PtrIpAdr->sin_family = AF_INET;
			Tv_PtrIpAdr->sin_addr.s_addr = A_NetMask;
			Tv_PtrIpAdr->sin_port = 0;
			if ( ioctl(Tv_Sock, SIOCSIFNETMASK, &Tv_IfReq) < 0 )
			{
				DbgMsgPrint("GrSockSetNetAdr: Netmask setting error\n");
			}

			//gateway set
			GrDumyZeroMem( &Tv_Route, sizeof(Tv_Route) );
			Tv_PtrRtDst		=	(struct sockaddr_in*)(&(Tv_Route.rt_dst));
			Tv_PtrRtGw		= (struct sockaddr_in *)(&(Tv_Route.rt_gateway));
			Tv_PtrRtMask	= (struct sockaddr_in *)(&(Tv_Route.rt_genmask));
			Tv_PtrRtDst->sin_family		=	AF_INET;
			Tv_PtrRtGw->sin_family		=	AF_INET;
			Tv_PtrRtMask->sin_family	=	AF_INET;
			Tv_PtrRtDst->sin_addr.s_addr	=	0;
			Tv_PtrRtGw->sin_addr.s_addr		=	0;
			Tv_PtrRtMask->sin_addr.s_addr	=	0;
			Tv_Route.rt_flags = RTF_UP | RTF_GATEWAY;
			ioctl(Tv_Sock,SIOCDELRT,&Tv_Route);			//remove default gateway

			Tv_PtrRtDst->sin_addr.s_addr	= 0;
			Tv_PtrRtGw->sin_addr.s_addr		= A_GateWay;
			Tv_PtrRtMask->sin_addr.s_addr = 0;
			Tv_Route.rt_metric				=	1;
			Tv_Route.rt_flags = RTF_UP | RTF_GATEWAY;
			ioctl(Tv_Sock,SIOCDELRT,&Tv_Route);			//remove already exist gateway

			if( ioctl(Tv_Sock,SIOCADDRT,&Tv_Route) < 0 ) 
			{
				DbgMsgPrint("GrSockSetNetAdr: gateway setting error\n");
			}

			//close socket
			close( Tv_Sock );
		}
#endif
}
//--------------------------------------------------------------------
__u32		GrSockGetMyGateway( __u8 A_DevIdx )
{
#ifdef LINUX_APP
	// local -------------------
		__u32	Tv_Result;
		__s32		Tv_Sock;
		__s8	Tv_MsgBuf[8192];	
		struct nlmsghdr*	Tv_PtrNlMsg;		//netlink message
		struct rtmsg*			Tv_PtrRtMsg;		//route message
		__s32		Tv_MsgSeq;
		__s32		Tv_Pid;
		struct nlmsghdr*	Tv_PtrNlHdr;
		__s32		Tv_ReadLen;
		__s32		Tv_MsgLen;
		__s8*	Tv_PtrBuf;
	// code --------------------
		Tv_Result	=	0;

		Tv_Sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
		if ( 0 <= Tv_Sock )
		{
			GrDumyZeroMem( Tv_MsgBuf, sizeof(Tv_MsgBuf) );
			Tv_PtrNlMsg	=	(struct nlmsghdr*)Tv_MsgBuf;
			Tv_PtrRtMsg	=	(struct rtmsg*)NLMSG_DATA(Tv_PtrNlMsg);

			/* Fill in the Tv_PtrNlMsg header*/
			Tv_MsgSeq		=	0;
			Tv_Pid			=	getpid();
			Tv_PtrNlMsg->nlmsg_len		= NLMSG_LENGTH(sizeof(struct rtmsg)); // Length of message.
			Tv_PtrNlMsg->nlmsg_type		= RTM_GETROUTE;		// Get the routes from kernel routing table .
			Tv_PtrNlMsg->nlmsg_flags	= NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump.
			Tv_PtrNlMsg->nlmsg_seq		= Tv_MsgSeq++; // Sequence of the message packet.
			Tv_PtrNlMsg->nlmsg_pid		= Tv_Pid; // PID of process sending the request.

			/* Send the request */
			if( 0 <= send(Tv_Sock, Tv_PtrNlMsg, Tv_PtrNlMsg->nlmsg_len, 0))
			{
				/* Read the response */
				Tv_ReadLen	=	0;
				Tv_MsgLen		=	0;
				Tv_PtrBuf		=	Tv_MsgBuf;

				do
				{
					/* Recieve response from the kernel */
					Tv_ReadLen	=	recv(Tv_Sock, Tv_PtrBuf, sizeof(Tv_MsgBuf) - Tv_MsgLen, 0);
					if( 0 <= Tv_ReadLen )
					{
						Tv_PtrNlHdr = (struct nlmsghdr *)Tv_PtrBuf;
						/* Check if the header is valid */
						if( (NLMSG_OK(Tv_PtrNlHdr, Tv_ReadLen) == 0) || (Tv_PtrNlHdr->nlmsg_type == NLMSG_ERROR))
						{
							DbgMsgPrint("GrSockGetMyGateway:Error in packet!\n");
							Tv_MsgLen	=	-1;
							break;
						}
						/* Check if the its the last message */
						if(Tv_PtrNlHdr->nlmsg_type == NLMSG_DONE) 
						{
							break;
						}
						else
						{
							/* Else move the pointer to buffer appropriately */
							Tv_PtrBuf += Tv_ReadLen;
							Tv_MsgLen += Tv_ReadLen;
						}

						/* Check if its a multi part message */
						if((Tv_PtrNlHdr->nlmsg_flags & NLM_F_MULTI) == 0) 
						{
							/* return if its not */
							break;
						}
					}
					else
					{
						DbgMsgPrint("GrSockGetMyGateway:SockNlReadError\n");
						Tv_MsgLen	=	-1;
						break;
					}
				} while((Tv_PtrNlHdr->nlmsg_seq != Tv_MsgSeq) || (Tv_PtrNlHdr->nlmsg_pid != Tv_Pid));

				//check message read success
				if ( 0 <= Tv_MsgLen )
				{
					/* Parse the response */
					for(;NLMSG_OK(Tv_PtrNlMsg,Tv_MsgLen);Tv_PtrNlMsg = NLMSG_NEXT(Tv_PtrNlMsg,Tv_MsgLen))
					{
						struct rtmsg*		Tv_PtrWkRtMsg;
						struct rtattr*	Tv_PtrRtAttr;
						__s32			Tv_RtLen;
						__s8		Tv_TmpBuf[100];
						__u32		Tv_DstAdr;
						//__u32		Tv_GateWay;

						Tv_DstAdr		=	0xFFFFFFFF;
						//Tv_GateWay	=	0;

						Tv_PtrWkRtMsg = (struct rtmsg*)NLMSG_DATA(Tv_PtrNlMsg);

						/* If the route is not for AF_INET or does not belong to main routing table
						then return. */
						if((Tv_PtrWkRtMsg->rtm_family != AF_INET) || (Tv_PtrWkRtMsg->rtm_table != RT_TABLE_MAIN))
						{
							break;
						}
						/* get the rtattr field */
						Tv_PtrRtAttr = (struct rtattr*)RTM_RTA(Tv_PtrWkRtMsg);
						Tv_RtLen = RTM_PAYLOAD(Tv_PtrNlMsg);
						for(;RTA_OK(Tv_PtrRtAttr,Tv_RtLen);Tv_PtrRtAttr = RTA_NEXT(Tv_PtrRtAttr,Tv_RtLen))
						{
							switch(Tv_PtrRtAttr->rta_type) 
							{
								/*
								case RTA_OIF:
									if_indextoname(*(__s32 *)RTA_DATA(rtAttr), rtInfo->ifName);
									break;
									*/
								case RTA_GATEWAY:
									Tv_Result	=	*(__u32*)RTA_DATA(Tv_PtrRtAttr);
									break;
									/*
								case RTA_PREFSRC:
									rtInfo->srcAddr = *(u_int *)RTA_DATA(Tv_PtrRtAttr);
									break;
									*/
								case RTA_DST:
									Tv_DstAdr		=	*(__u32*)RTA_DATA(Tv_PtrRtAttr);
									break;
							}
						}
					}
				}
				else
				{
					DbgMsgPrint("GrSockGetMyGateway:Message read failure!\n");
				}
			}
			else
			{
				DbgMsgPrint("GrSockGetMyGateway: write socket fail!\n");
			}
			//close socket
			close(Tv_Sock);
		}
		else
		{
			DbgMsgPrint("GrSockGetMyGateway:socket open error!\n");
		}
		return	Tv_Result;
#else
		return	0;
#endif

}
//--------------------------------------------------------------------
BOOL8	GrSockChkCloseByRecv( __s32 A_RecvVal )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 == A_RecvVal )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrSockSetCloseTimeOut( Def_GrSock A_Hndl, __u16 A_TimeOutSec )
{
	// local -------------------
		linger	Tv_Linger;
	// code --------------------
		Tv_Linger.l_onoff		=	TRUE;
		Tv_Linger.l_linger	=	A_TimeOutSec;
		if ( 0xFFFF == A_TimeOutSec )
		{
			Tv_Linger.l_onoff		=	FALSE;
			Tv_Linger.l_linger	=	0;
		}
#ifdef LINUX_APP
		setsockopt( A_Hndl, SOL_SOCKET, SO_LINGER, &Tv_Linger, sizeof(Tv_Linger) );	//fast close option
#else
		setsockopt( A_Hndl, SOL_SOCKET, SO_LINGER, (char*)&Tv_Linger, sizeof(Tv_Linger) );	//fast close option
#endif
}
//--------------------------------------------------------------------
void		GrSockTcpNodelySet( Def_GrSock A_Hndl, BOOL8 A_IsOn )
{
	// local -------------------
	// code --------------------
	#ifdef LINUX_APP
		setsockopt( A_Hndl, SOL_SOCKET, SO_LINGER, &A_IsOn, sizeof(A_IsOn) );	//no delay
	#else
		setsockopt( A_Hndl, SOL_SOCKET, SO_LINGER, (char*)&A_IsOn, sizeof(A_IsOn) );	//no delay
	#endif
}
//--------------------------------------------------------------------
__u16		GrSockCalcChksum( void* A_PtrBuf, __u32 A_Size, void* A_PtrExt, __u32 A_ExtSize )
{
	// local -------------------
		__u32	Tv_Result;
		__u16*	Tv_PtrWk;
		__s32		Tv_Last;
		__u8*	Tv_PtrBt;
	// code --------------------
		Tv_Result	=	0;

		if ( (NULL != A_PtrExt) && (0 != A_ExtSize) )
		{
			Tv_Last		=	(__s32)A_ExtSize;
			Tv_PtrWk	=	(__u16*)A_PtrExt;
			while ( 1 < Tv_Last )
			{
				Tv_Result	=	Tv_Result + (__u32)(*Tv_PtrWk);
				//next
				Tv_PtrWk ++;
				Tv_Last	=	Tv_Last - 2;
			}
		}

		Tv_Last		=	(__s32)A_Size;
		Tv_PtrWk	=	(__u16*)A_PtrBuf;
		while (1 < Tv_Last)
		{
			Tv_Result	=	Tv_Result + (__u32)(*Tv_PtrWk);
			//next
			Tv_PtrWk ++;
			Tv_Last	=	Tv_Last - 2;
		}
		
		//check last byte
		if ( 0 != Tv_Last )
		{
			Tv_PtrBt	=	(__u8*)Tv_PtrWk;
			Tv_Result	=	Tv_Result + (__u32)(*Tv_PtrBt);
		}
		//fold 32bit sum to 16bit
		while ( 0 != (0xFFFF0000 & Tv_Result) )
		{
			Tv_Result	=	(Tv_Result & 0xFFFF) + (Tv_Result >> 16);
		}

		Tv_Result	=	(~Tv_Result) & 0xFFFF;
		return	(__u16)Tv_Result;
}
//--------------------------------------------------------------------
__u8		GrSockNetIfAdaptarIdGet( __u8 A_DevIdx )
{
	// var -------------------
		__u8		Tv_Result;
#ifdef LINUX_APP
		Def_GrSock	Tv_Hndl;
		struct ifreq		Tv_IfReq;
		struct sockaddr_in*		Tv_PtrNetAddr;
#endif
	// code ------------------
		//init
		Tv_Result		=	0xFF;
#ifdef LINUX_APP
		GrDumyZeroMem( &Tv_IfReq, sizeof(Tv_IfReq) );

		Tv_Hndl	=	socket( AF_INET, SOCK_RAW, IPPROTO_RAW );
		if ( E_GrSockBadHandle != Tv_Hndl )
		{
			Tv_IfReq.ifr_addr.sa_family = AF_INET;
			GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
			//get network interface index
			if ( 0 == ioctl(Tv_Hndl, SIOCGIFINDEX, &Tv_IfReq) )
			{
				Tv_Result	= Tv_IfReq.ifr_ifindex;
			}
		}

		GrSockClose( Tv_Hndl );
#endif
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrSockDhcpReq( __u8 A_NetIf )
{
		return	V_GrSocketDhcpCli->Request( A_NetIf, FALSE, FALSE );
}
//--------------------------------------------------------------------
void		GrSockDhcpStop( void )
{
		V_GrSocketDhcpCli->StopLeaseExpand();
}
//--------------------------------------------------------------------
BOOL8		GrSockDhcpResult( __u32* A_PtrRtIp, __u32* A_PtrRtMask, __u32* A_PtrRtGate, __u32* A_PtrRtDns )
{
		return	V_GrSocketDhcpCli->GetResult( A_PtrRtIp, A_PtrRtMask, A_PtrRtGate, A_PtrRtDns );
}
//--------------------------------------------------------------------
BOOL8	GrSockFreeDnsUpdt(__u8 A_DevIdx, __u8	A_HostType, char* A_StrId, char* A_StrPass, char* A_StrDomain)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != V_GrSocketFdnsCli )
		{
			Tv_Result	=	V_GrSocketFdnsCli->ReqUpdate( A_DevIdx, A_HostType, A_StrId, A_StrPass, A_StrDomain );
		}
		return	Tv_Result;		
}
//--------------------------------------------------------------------
BOOL8	GrSockFreeDnsUpdt( __u8 A_DevIdx, __u8	A_HostType, WCHAR* A_StrId, WCHAR* A_StrPass, WCHAR* A_StrDomain )
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_StrTag	Tv_StrId;
		Def_StrTag	Tv_StrPass;
		Def_StrTag	Tv_StrDomain;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != V_GrSocketFdnsCli )
		{
			GrStrWstrToStr( Tv_StrId, A_StrId );
			GrStrWstrToStr( Tv_StrPass, A_StrPass );
			GrStrWstrToStr( Tv_StrDomain, A_StrDomain );
			Tv_Result	=	V_GrSocketFdnsCli->ReqUpdate( A_DevIdx, A_HostType, Tv_StrId, Tv_StrPass, Tv_StrDomain );
		}
		return	Tv_Result;		
}
//--------------------------------------------------------------------
__u8		GrSockWaitConnect( Def_GrSock A_Hndl, __u32 A_TimeOut )
{
	// local -------------------
		__u8	Tv_Result;
		struct timeval	Tv_TmVal;
		fd_set	Tv_FdSet;
		__s32		Tv_Rval;
		__s32		Tv_Err;
	// code --------------------
		Tv_Result	=	E_GrSockConnWaiting;
		
		//set timeout
		Tv_TmVal.tv_sec		=	0;
		Tv_TmVal.tv_usec	=	0;	
		if ( 0 != A_TimeOut )
		{
			Tv_TmVal.tv_sec		=	A_TimeOut / 1000;
			Tv_TmVal.tv_usec	=	(A_TimeOut % 1000) * 1000;	
		}
		FD_ZERO( &Tv_FdSet );
		FD_SET( A_Hndl, &Tv_FdSet );
#ifdef _WINDOWS
		Tv_Rval = select(0, NULL, &Tv_FdSet, NULL, &Tv_TmVal);	// windows is not used any more nfds
#else
		Tv_Rval	=	select( A_Hndl + 1, NULL, &Tv_FdSet, NULL, &Tv_TmVal );
#endif
		if ( 0 > Tv_Rval )
		{
			//check error
#ifdef LINUX_APP
			Tv_Err	=	errno;
			if ( (EINTR != Tv_Err) && (EINPROGRESS != Tv_Err) )
			{
				/* if (EINPROGRESS != Tv_Err) */
				{
					DbgMsgPrint( "GrSockWaitConnect : error - err = %d \n", Tv_Err );
				}
				Tv_Result	=	E_GrSockConnTimeOut;
			}
#else
			Tv_Err	=	WSAGetLastError();
			if ( WSAEWOULDBLOCK != Tv_Err )
			{
				DbgMsgPrint( "GrSockWaitConnect : error - err = %d \n", Tv_Err );
				Tv_Result	=	E_GrSockConnTimeOut;
			}
#endif
		}
		else if ( 0 != Tv_Rval )
		{
			Tv_Result	=	E_GrSockConnOk;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrSockUdpBroad( Def_GrSock A_Hndl )
{
	// local -------------------
		BOOL8		Tv_Result;
		BOOL8		Tv_IsReuse;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//set mode
		Tv_IsReuse	=	TRUE;
		if ( 0 <= setsockopt(A_Hndl, SOL_SOCKET, SO_BROADCAST, (char*)&Tv_IsReuse, sizeof(Tv_IsReuse)) )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

