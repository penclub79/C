/*
	socket library
		ip struct : 192.168.0.1   [0]192[1]168[2]0[3]1
*/

#ifndef		LINUX_APP
	#include <WinSock2.h>
#include <WS2tcpip.h>
#endif
#include	<GrTypeBase.h>
#include	<GrSockV2.h>
#ifdef LINUX_APP

	#include	<asm/types.h>

#include	<sys/types.h> 
#include	<sys/socket.h>
#include	<net/if.h>

	#include	<errno.h>
	#include	<arpa/inet.h>
	#include	<fcntl.h>
	#include	<errno.h>
	#include	<netdb.h>

	#include	<netinet/in.h>
	#include	<netinet/ether.h> 
	#include	<net/ethernet.h> 

	#include	<sys/time.h> 
	#include	<unistd.h> 
	#include	<sys/ioctl.h> 
	#include	<sys/utsname.h>

	#include	<linux/sockios.h>
	#include	<net/route.h>
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
#include <GrTaskBase.h>
#include <GrTaskSvr.h>

#include <GrError.h>

//====================================================================
//local const

//====================================================================
//local type

#ifdef LINUX_APP
struct in6_ifreq {
	struct in6_addr ifr6_addr;
	__u32 ifr6_prefixlen;
	unsigned int ifr6_ifindex;
};
#endif

//====================================================================
//global var

//====================================================================
//functions

//--------------------------------------------------------------------
__u32		GrSockV2GetErrCode( void )
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
				case WSAECONNRESET:
					Tv_Result		=	E_GrSockErrConnBroken;
					break;
				default:
					Tv_Result		=	E_GrSockErrEtc;
			    break;
			}
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrSock	GrSockV2Open( BOOL8 A_IsUdp, BOOL8 A_IsNonblock, BOOL8 A_IsV6 )
{
	// local -------------------
		Def_GrSock	Tv_Result;
		__s32				Tv_Type;
		__s32				Tv_Af;
		#ifdef LINUX_APP
			__s32			Tv_Atb;
		#else
		u_long		Tv_Val;
		#endif
	// code --------------------
		Tv_Type			=	SOCK_STREAM;
		if ( A_IsUdp )
		{
			Tv_Type			=	SOCK_DGRAM;
		}
		Tv_Af			=	AF_INET;
		if ( A_IsV6 )
		{
			Tv_Af			=	AF_INET6;
		}
		Tv_Result	=	socket( Tv_Af, Tv_Type, 0 );
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
					ioctlsocket( Tv_Result, FIONBIO, &Tv_Val );
				#endif
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrSockV2SetBlockMode( Def_GrSock A_Hndl, BOOL8 A_IsNonBlock )
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
BOOL8		GrSockV2UdpPortAssign( Def_GrSock A_Hndl, __u16 A_Port )
{
	// local -------------------
		BOOL8		Tv_Result;
		sockaddr_in	Tv_NetAdr;
		BOOL		Tv_IsReuse;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//set mode
		Tv_IsReuse	=	TRUE;
		//setsockopt( A_Hndl, SOL_SOCKET, SO_REUSEADDR, (char*)&Tv_IsReuse, sizeof(Tv_IsReuse) );
#ifdef LINUX_APP
		setsockopt( A_Hndl, SOL_SOCKET, SO_REUSEADDR, (char*)&Tv_IsReuse, sizeof(Tv_IsReuse) );
		setsockopt( A_Hndl, SOL_SOCKET, SO_BINDTODEVICE, "eth0", 4 );
#else
		setsockopt( A_Hndl, SOL_SOCKET, SO_REUSEADDR, (char*)&Tv_IsReuse, sizeof(Tv_IsReuse) );
#endif

		GrDumyZeroMem(&Tv_NetAdr, sizeof(Tv_NetAdr));
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
BOOL8		GrSockV2UdpIpAndPortAssign(Def_GrSock A_Hndl, void* A_PtrIp, __u16 A_Port, BOOL8 A_IsV6)
{
	// local -------------------
		BOOL8		Tv_Result;
		sockaddr_in	Tv_NetAdr;
		BOOL		Tv_IsReuse;
#ifndef LINUX_APP
		int			Tv_Err;
#endif
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//set mode
		Tv_IsReuse	=	TRUE;
		setsockopt( A_Hndl, SOL_SOCKET, SO_REUSEADDR, (char*)&Tv_IsReuse, sizeof(Tv_IsReuse) );
		GrDumyZeroMem(&Tv_NetAdr, sizeof(Tv_NetAdr));
		Tv_NetAdr.sin_family			=	AF_INET;
		Tv_NetAdr.sin_addr.s_addr	=	*((__u32*)A_PtrIp);
		Tv_NetAdr.sin_port				=	A_Port;

		if ( 0 <= bind( A_Hndl, (sockaddr*)&Tv_NetAdr, sizeof(Tv_NetAdr) ) )
		{
			Tv_Result		=	TRUE;
		}
#ifndef LINUX_APP
		else
		{
			Tv_Err	=	WSAGetLastError();
			DbgMsgPrint("GrSockV2UdpIpAssign - bind error %d\n",Tv_Err);
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrSockV2SetListenMode(Def_GrSock A_Hndl, __u16 A_Port, BOOL8 A_IsV6, __u32 A_QueCnt)
{
	// local -------------------
		BOOL8		Tv_Result;
		BOOL		Tv_IsReuse;
		int			Tv_RtCode;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//set mode
		Tv_IsReuse	=	TRUE;
		setsockopt( A_Hndl, SOL_SOCKET, SO_REUSEADDR, (char*)&Tv_IsReuse, sizeof(Tv_IsReuse) );
		if ( A_IsV6 )
		{
			// IPV6
			sockaddr_in6	Tv_NetAdr;
			
			GrDumyZeroMem(&Tv_NetAdr, sizeof(Tv_NetAdr));

			Tv_NetAdr.sin6_family			=	AF_INET6;
			Tv_NetAdr.sin6_flowinfo		= 0;
			Tv_NetAdr.sin6_port				= htons(A_Port);
			Tv_NetAdr.sin6_addr				= in6addr_any;
			Tv_NetAdr.sin6_scope_id		= 0;		//if_nametoindex("eth0");

			Tv_RtCode	=	bind(A_Hndl, (sockaddr*)&Tv_NetAdr, sizeof(Tv_NetAdr));
		}
		else
		{
			// IPV4
			sockaddr_in	Tv_NetAdr;

			Tv_NetAdr.sin_family			=	AF_INET;
			Tv_NetAdr.sin_addr.s_addr	=	htonl(INADDR_ANY);
			Tv_NetAdr.sin_port				=	htons(A_Port);
			Tv_RtCode	=	bind(A_Hndl, (sockaddr*)&Tv_NetAdr, sizeof(Tv_NetAdr));
		}

		if ( 0 <=  Tv_RtCode)
		{
			if(0 <= listen(A_Hndl, A_QueCnt))
			{
				Tv_Result		=	TRUE;
			}
#ifdef LINUX_APP
			else
			{
				DbgMsgPrint("GrSockV2SetListenMode - listen error %d\n", errno);
			}
#endif
		}
#ifdef LINUX_APP
		else
		{
			DbgMsgPrint("GrSockV2SetListenMode - bind error %d\n", errno);
		}
#endif
		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrSock	GrSockV2Accept( Def_GrSock A_HndlListen, BOOL8 A_IsNonblock, 
	void* A_PtrRtIp, __u16* A_PtrRtPort, BOOL8* A_PtrRtIsV6)
{
	// local -------------------
		Def_GrSock	Tv_Result;
		sockaddr_in6	Tv_AdrConn;
		socklen_t		Tv_RtSize;
		#ifdef LINUX_APP
			__s32			Tv_Atb;
			linger	Tv_Linger;
		#else
		u_long		Tv_Val;
		#endif
	// code --------------------
		*A_PtrRtIsV6	=	FALSE;
		Tv_RtSize			=	sizeof(Tv_AdrConn);
		GrDumyZeroMem(&Tv_AdrConn, sizeof(Tv_AdrConn));
		Tv_Result			=	accept( A_HndlListen, (sockaddr*)&Tv_AdrConn, &Tv_RtSize );
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
			if(AF_INET == Tv_AdrConn.sin6_family)
			{
				sockaddr_in*	Tv_PtrAdrV4;

				Tv_PtrAdrV4		=	(sockaddr_in*)&Tv_AdrConn;

				*((__u32*)A_PtrRtIp)	=	Tv_PtrAdrV4->sin_addr.s_addr;
				*A_PtrRtPort	=	htons(Tv_PtrAdrV4->sin_port);
			}
			else if(AF_INET6 == Tv_AdrConn.sin6_family)
			{
				GrDumyCopyMem(A_PtrRtIp, &Tv_AdrConn.sin6_addr, 16);
				*A_PtrRtPort	=	htons(Tv_AdrConn.sin6_port);
				*A_PtrRtIsV6	=	TRUE;
			}
			else
			{
				*((__u32*)A_PtrRtIp)	=	0;
				*A_PtrRtPort					=	0;
				DbgMsgPrint("GrSockV2Accept - unknown family type %d\n", Tv_AdrConn.sin6_family);
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8		GrSockV2Connect( Def_GrSock A_Hndl, void* A_PtrIp, __u16 A_Port, BOOL8 A_IsV6 )
{
	// local -------------------
		__u8		Tv_Result;
		int			Tv_AdrSize;
		int			Tv_Err;
	// code --------------------
		//init
		Tv_Result		=	E_GrSockV2ConnTimeOut;

		//try connect
		if(!A_IsV6)
		{
			// IPv4
			sockaddr_in	Tv_AdrConn;

			Tv_AdrConn.sin_family				=	AF_INET;
			Tv_AdrConn.sin_addr.s_addr	=	*((__u32*)A_PtrIp);
			Tv_AdrConn.sin_port					=	htons((__u16)A_Port);
			Tv_AdrSize									=	sizeof(Tv_AdrConn);
			Tv_Err	= connect(A_Hndl, (sockaddr*)&Tv_AdrConn, Tv_AdrSize);
			if(0 <= Tv_Err)
			{
				return	E_GrSockV2ConnOk;
			}
		}
		else
		{
			// IPv6
			sockaddr_in6	Tv_AdrConn;

			GrDumyZeroMem(&Tv_AdrConn, sizeof(Tv_AdrConn));

			Tv_AdrConn.sin6_family			=	AF_INET6;
			Tv_AdrConn.sin6_port				= htons(A_Port);
			GrDumyCopyMem(&Tv_AdrConn.sin6_addr, A_PtrIp,16);

			Tv_AdrSize									=	sizeof(Tv_AdrConn);

			if(0 <= connect(A_Hndl, (sockaddr*)&Tv_AdrConn, Tv_AdrSize))
			{
				return	E_GrSockV2ConnOk;
			}
		}

#ifdef LINUX_APP
		Tv_Err	=	errno;
		if ( (EINPROGRESS == Tv_Err) || (EALREADY == Tv_Err) )
		{
			Tv_Result	=	E_GrSockV2ConnWaiting;
		}
		else if ( (EISCONN == Tv_Err) )
		{
			Tv_Result		=	E_GrSockV2ConnOk;
		}
		else
		{
			DbgMsgPrint( "GrSockConnect - connect error - ip=%8X , err = %d \n", *((__u32*)A_PtrIp), Tv_Err );
		}
#else
		Tv_Err	=	WSAGetLastError();
		if ( (WSAEWOULDBLOCK == Tv_Err)  )
		{
			Tv_Result	=	E_GrSockV2ConnWaiting;
		}
		else if ( WSAEISCONN == Tv_Err )
		{
			Tv_Result		=	E_GrSockV2ConnOk;
		}
		else
		{
			DbgMsgPrint( "GrSockConnect - connect error - ip=%8X , err = %d \n", *((__u32*)A_PtrIp), Tv_Err );
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrSockV2GetMacAddr( __u8 A_DevIdx, __u8* A_PtrRt )
{
	// local -------------------
		BOOL8			Tv_Result;
		#ifdef LINUX_APP
			__s32			Tv_Hndl;
			struct ifreq	Tv_IfReq; 
			__u8*		Tv_PtrData;
			__u32		Tv_WkIdx;
			__u8*		Tv_PtrTg;
		#endif
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		
#ifdef LINUX_APP
			Tv_Hndl		=	socket( AF_INET, SOCK_STREAM, 0 );
			if ( 0 <= Tv_Hndl )
			{
				GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
				Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
				if ( 0 <= ioctl( Tv_Hndl, SIOCGIFHWADDR, &Tv_IfReq ) )
				{
					Tv_PtrData		=	(__u8*)( &Tv_IfReq.ifr_hwaddr.sa_data );
					Tv_PtrTg			=	A_PtrRt;
					for ( Tv_WkIdx=0;Tv_WkIdx < 6;Tv_WkIdx++ )
					{
						*Tv_PtrTg	=	*Tv_PtrData;
						Tv_PtrData ++;
						Tv_PtrTg ++;
					}
					Tv_Result	=	TRUE;
				}
				close( Tv_Hndl );
			}
#else
		A_PtrRt[0]	=	0x00;
		A_PtrRt[1]	=	0x18;
		A_PtrRt[2]	=	0xCF;
		A_PtrRt[3]	=	0xE8;
		A_PtrRt[4]	=	0x00;
		A_PtrRt[5]	=	0x10;

		Tv_Result		=	TRUE;
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrSockV2SetMacAddr(__u8 A_DevIdx, __u8* A_PtrMac)
{
#ifdef LINUX_APP
	// local -------------------
		BOOL8	Tv_Result;
		__s32			Tv_Hndl;
		struct ifreq	Tv_IfReq; 
		__u8*		Tv_PtrData;
		__u32		Tv_WkIdx;
		__u8*		Tv_PtrTg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Hndl		=	socket(AF_INET, SOCK_STREAM, 0);
		if(0 <= Tv_Hndl)
		{
			GrStrCopy(Tv_IfReq.ifr_name, "eth0");
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
			GrDumyCopyMem(Tv_IfReq.ifr_hwaddr.sa_data, A_PtrMac, 6);
			Tv_IfReq.ifr_hwaddr.sa_family = ARPHRD_ETHER;

			if(0 > ioctl(Tv_Hndl, SIOCSIFHWADDR, &Tv_IfReq))
			{
				DbgMsgPrint("GrSockV2SetMacAddr - change mac addres failure(%d)\n", errno);
			}
			else
			{
				Tv_Result	=	TRUE;
			}
			close(Tv_Hndl);
		}

		return	Tv_Result;
#else

		return	TRUE;
#endif
}
//--------------------------------------------------------------------
__u32		GrSockV2UdpSend( Def_GrSock A_Hndl, void* A_PtrIp, __u16 A_Port, void* A_PtrData, __u32 A_SizeData, BOOL8 A_IsV6 )
{
	// local -------------------
		__s32			Tv_Result;
		sockaddr_in	Tv_AdrConn;
		__u32		Tv_AdrSize;
	// code --------------------
		//set address
		GrDumyZeroMem(&Tv_AdrConn, sizeof(Tv_AdrConn));
		Tv_AdrConn.sin_family				=	AF_INET;
		Tv_AdrConn.sin_addr.s_addr	=	*((__u32*)A_PtrIp);
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
			DbgMsgPrint("GrSockV2UdpSend() - send to error. ret = %d\n", Tv_Result);
			Tv_Result		=	0;
		}
		
		return	(__u32)Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrSockV2UdpRead(Def_GrSock A_Hndl, void* A_PtrRtIp, __u16* A_PtrRtPort, void* A_PtrData, __u32 A_SizeData,
	BOOL8 A_IsV6, Def_GrErrCode* A_PtrRtErrCode)
{
	// local -------------------
		__s32			Tv_Result;
		sockaddr_in	Tv_AdrConn;
		socklen_t		Tv_AdrSize;
	// code --------------------
		// init
		*A_PtrRtErrCode	=	E_GrErrNone;
		//set address
		Tv_AdrConn.sin_family				=	AF_INET;
		Tv_AdrConn.sin_addr.s_addr	=	INADDR_ANY;
		Tv_AdrConn.sin_port					=	0;
		Tv_AdrSize		=	sizeof(Tv_AdrConn);

		#ifdef LINUX_APP
			Tv_Result		=	recvfrom( A_Hndl, A_PtrData, (size_t)A_SizeData, 0, 
										(sockaddr*)&Tv_AdrConn, &Tv_AdrSize );
			if(0 > Tv_Result)
			{
				*A_PtrRtErrCode	=	E_GrErrBadParam;
			}
		#else
			Tv_Result		=	recvfrom( A_Hndl, (char*)A_PtrData, (int)A_SizeData, 0, 
										(sockaddr*)&Tv_AdrConn, (int*)&Tv_AdrSize );
			if(0 > Tv_Result)
			{
				int	Tv_RtCode;

				Tv_RtCode	=	WSAGetLastError();
				switch( Tv_RtCode )
				{
					case WSANOTINITIALISED:
						*A_PtrRtErrCode	=	E_GrErrInitFail;
						break;
					case WSAEMSGSIZE:
						*A_PtrRtErrCode	=	E_GrErrBufFull;
						break;
					default:
						*A_PtrRtErrCode	=	E_GrErrBadParam;
						break;
				}
			}
		#endif

		//patch result
		if ( 0 > Tv_Result )
		{
			Tv_Result		=	0;
		}
		else
		{
			//update ip
			*((__u32*)A_PtrRtIp)		=	(__u32)Tv_AdrConn.sin_addr.s_addr;
			// update port
			*A_PtrRtPort	=	(__u16)htons(Tv_AdrConn.sin_port);
		}

		return	(__u32)Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrSockV2GetMyIp( __u8 A_DevIdx, void* A_PtrRt, BOOL8 A_IsV6 )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

#ifdef LINUX_APP
		{
			__u32		Tv_WkIdx;
			__s32		Tv_Hndl;
			char		Tv_StrIfn[8];

			if(A_IsV6)
			{
				// IPV6
				struct in6_ifreq	Tv_IfReq;
				struct sockaddr_in6*	Tv_PtrIpAdr;

				Tv_Hndl		=	socket(AF_INET6, SOCK_STREAM, 0);

				if(0 <= Tv_Hndl)
				{

					GrDumyZeroMem(&Tv_IfReq, sizeof(Tv_IfReq));
					Tv_PtrIpAdr	=	(struct sockaddr_in6 *) &Tv_IfReq.ifr6_addr;
					Tv_PtrIpAdr->sin6_family		=	AF_INET6;

					GrStrCopy(Tv_StrIfn, "eth0");
					Tv_StrIfn[3]	=	Tv_StrIfn[3] + A_DevIdx;

					Tv_PtrIpAdr->sin6_scope_id	=	if_nametoindex(Tv_StrIfn);

					if(0 <= ioctl(Tv_Hndl, SIOCGIFADDR, &Tv_IfReq))
					{
						GrDumyCopyMem(A_PtrRt, &Tv_PtrIpAdr->sin6_addr, 16);
						Tv_Result	=	TRUE;
					}
					close(Tv_Hndl);
				}

			}
			else
			{
				// IPV4
				struct ifreq	Tv_IfReq;
				struct sockaddr_in*	Tv_PtrIpAdr;

				Tv_Hndl		=	socket(AF_INET, SOCK_STREAM, 0);

				if(0 <= Tv_Hndl)
				{
					GrStrCopy(Tv_IfReq.ifr_name, "eth0");
					Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
					if(0 <= ioctl(Tv_Hndl, SIOCGIFADDR, &Tv_IfReq))
					{
						Tv_PtrIpAdr		=	(struct sockaddr_in*)&Tv_IfReq.ifr_ifru.ifru_addr;
						*((__u32*)A_PtrRt)	=	Tv_PtrIpAdr->sin_addr.s_addr;
						Tv_Result	=	TRUE;
					}
					close(Tv_Hndl);
				}

			}

		}

#else
		{

#if 1
			struct addrinfo	Tv_Hints;
			struct addrinfo*	Tv_PtrRes;
			struct addrinfo*	Tv_PtrWk;

			Def_StrTag	Tv_StrName;
			int		Tv_Err;

			gethostname(Tv_StrName, sizeof(Tv_StrName)- 1);

			GrDumyZeroMem(&Tv_Hints, sizeof(Tv_Hints));
			Tv_Hints.ai_socktype	=	SOCK_STREAM;
			Tv_Hints.ai_family		=	AF_INET;
			if( A_IsV6)
			{
				Tv_Hints.ai_family		=	AF_INET6;
			}

			Tv_Err	=	getaddrinfo(Tv_StrName, NULL, &Tv_Hints, &Tv_PtrRes);
			if(0 != Tv_Err)
			{
				return	FALSE;
			}

			Tv_PtrWk	=	Tv_PtrRes;

			while(NULL != Tv_PtrWk)
			{
				if(AF_INET == Tv_PtrWk->ai_family)
				{
					if(!A_IsV6)
					{
						sockaddr_in*	Tv_PtrAdr;
						Tv_PtrAdr		=	(sockaddr_in*)Tv_PtrWk->ai_addr;
						*((__u32*)A_PtrRt)	=	Tv_PtrAdr->sin_addr.S_un.S_addr;
						Tv_Result	=	TRUE;
						break;
					}
				}
				else if(AF_INET6 == Tv_PtrWk->ai_family)
				{
					if(A_IsV6)
					{
						sockaddr_in6*	Tv_PtrAdr;
						Tv_PtrAdr		=	(sockaddr_in6*)Tv_PtrWk->ai_addr;
						GrDumyCopyMem(A_PtrRt, Tv_PtrAdr->sin6_addr.u.Byte, 16);
						Tv_Result	=	TRUE;
						break;
					}
				}

				// next
				Tv_PtrWk	=	Tv_PtrWk->ai_next;
			}


#else
			Def_StrTag	Tv_StrName;
			hostent*	Tv_PtrEnt;
			char**		Tv_PtrAdr;
			__u32*		Tv_PtrIp;

			gethostname( Tv_StrName, sizeof(Tv_StrName) - 1 );
			Tv_PtrEnt	=	gethostbyname( Tv_StrName );
			if ( NULL != Tv_PtrEnt )
			{
				Tv_PtrAdr	=	Tv_PtrEnt->h_addr_list;
				if ( NULL != *Tv_PtrAdr )
				{
					Tv_PtrIp		=	(__u32*)*Tv_PtrAdr;
					*((__u32*)A_PtrRt)	=	*Tv_PtrIp;
					Tv_Result		=	TRUE;
				}
			}
#endif
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrSockV2GetMyNetmask( __u8 A_DevIdx, void* A_PtrRt, BOOL8 A_IsV6 )
{
	// local -------------------
		BOOL8		Tv_Result;
#ifdef LINUX_APP
		__s32			Tv_Hndl;
		struct ifreq	Tv_IfReq; 
		__u32		Tv_WkIdx;
		struct sockaddr_in*	Tv_PtrIpAdr;
#endif
	// code --------------------
		//init
		Tv_Result		=	FALSE;

	#ifdef LINUX_APP
		Tv_Hndl		=	socket( AF_INET, SOCK_STREAM, 0 );

		if ( 0 <= Tv_Hndl )
		{
			GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
			if ( 0 <= ioctl( Tv_Hndl, SIOCGIFNETMASK, &Tv_IfReq ) )
			{
				Tv_PtrIpAdr		=	(struct sockaddr_in*)&Tv_IfReq.ifr_ifru.ifru_addr;
				*((__u32*)A_PtrRt)	=	Tv_PtrIpAdr->sin_addr.s_addr;
				Tv_Result		=	TRUE;
			}
			close( Tv_Hndl );
		}
	#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrSockV2IsLink( __s8 A_DevIdx )
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
BOOL8		GrSockV2NetIfActive( __s8 A_DevIdx, BOOL8 A_IsOn )
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
BOOL8		GrSockV2NetIfMtuSet( __s8 A_DevIdx, __s32 A_Mtu )
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
BOOL8		GrSockV2SetNetAdr( __u8 A_DevIdx, void* A_PtrIp, void* A_PtrNetMask, void* A_PtrGateWay, BOOL8 A_IsV6 )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrTaskMsgNetIf	Tv_Msg;
#ifdef LINUX_APP
		__s32		Tv_Rslt;
		__s32		Tv_Sock;
		struct  ifreq	Tv_IfReq;
		struct sockaddr_in*	Tv_PtrIpAdr;
		struct rtentry Tv_Route;
		struct sockaddr_in*	Tv_PtrRtDst;
		struct sockaddr_in*	Tv_PtrRtGw;
		struct sockaddr_in*	Tv_PtrRtMask;
#endif
	// code --------------------
		Tv_Result	=	FALSE;
#ifdef LINUX_APP
		Tv_Sock	=	socket(AF_INET, SOCK_DGRAM,0);
		if ( 0 <= Tv_Sock )
		{
			Tv_Result	=	TRUE;
			//ip set
			GrDumyZeroMem( &Tv_IfReq, sizeof(Tv_IfReq) );
			GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
			Tv_PtrIpAdr	=	(struct sockaddr_in*)&Tv_IfReq.ifr_ifru.ifru_addr;
			Tv_PtrIpAdr->sin_family = AF_INET;
			Tv_PtrIpAdr->sin_addr.s_addr = *((__u32*)A_PtrIp);
			Tv_PtrIpAdr->sin_port = 0;
			if ( ioctl(Tv_Sock, SIOCSIFADDR, &Tv_IfReq) < 0 )
			{
				DbgMsgPrint("GrSockV2SetNetAdr: IP ( %d.%d.%d.%d ) setting error\n", 
					*((__u8*)A_PtrIp), *((__u8*)((__u32)A_PtrIp + 1)), *((__u8*)((__u32)A_PtrIp + 2)), *((__u8*)((__u32)A_PtrIp + 3)) );
				Tv_Result	=	FALSE;
			}
			else
			{
				DbgMsgPrint("GrSockV2SetNetAdr: IP ( %d.%d.%d.%d ) set.\n", 
					*((__u8*)A_PtrIp), *((__u8*)((__u32)A_PtrIp + 1)), *((__u8*)((__u32)A_PtrIp + 2)), *((__u8*)((__u32)A_PtrIp + 3)) );
			}

			//NETMASK set
			GrDumyZeroMem( &Tv_IfReq, sizeof(Tv_IfReq) );
			GrStrCopy( Tv_IfReq.ifr_name, "eth0" );
			Tv_IfReq.ifr_name[3]	=	Tv_IfReq.ifr_name[3] + A_DevIdx;
			Tv_PtrIpAdr	=	(struct sockaddr_in*)&Tv_IfReq.ifr_ifru.ifru_addr;
			Tv_PtrIpAdr->sin_family = AF_INET;
			Tv_PtrIpAdr->sin_addr.s_addr = *((__u32*)A_PtrNetMask);
			Tv_PtrIpAdr->sin_port = 0;
			if ( ioctl(Tv_Sock, SIOCSIFNETMASK, &Tv_IfReq) < 0 )
			{
				DbgMsgPrint("GrSockV2SetNetAdr: NETMASK ( %d.%d.%d.%d ) setting error\n" , 
					*((__u8*)A_PtrNetMask), *((__u8*)((__u32)A_PtrNetMask + 1)), *((__u8*)((__u32)A_PtrNetMask + 2)), *((__u8*)((__u32)A_PtrNetMask + 3)) );
				Tv_Result	=	FALSE;
			}
			else
			{
				DbgMsgPrint("GrSockV2SetNetAdr: NETMASK ( %d.%d.%d.%d ) set\n" , 
					*((__u8*)A_PtrNetMask), *((__u8*)((__u32)A_PtrNetMask + 1)), *((__u8*)((__u32)A_PtrNetMask + 2)), *((__u8*)((__u32)A_PtrNetMask + 3)) );
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
			Tv_PtrRtGw->sin_addr.s_addr		= *((__u32*)A_PtrGateWay);
			Tv_PtrRtMask->sin_addr.s_addr = 0;
			Tv_Route.rt_metric				=	1;
			Tv_Route.rt_flags = RTF_UP | RTF_GATEWAY;
			ioctl(Tv_Sock,SIOCDELRT,&Tv_Route);			//remove already exist gateway

			Tv_Rslt	=	ioctl(Tv_Sock,SIOCADDRT,&Tv_Route);
			if( 0 > Tv_Rslt ) 
			{
				DbgMsgPrint("GrSockV2SetNetAdr: gateway ( %d.%d.%d.%d ) setting error %d \n", 
					*((__u8*)A_PtrGateWay), *((__u8*)((__u32)A_PtrGateWay + 1)), *((__u8*)((__u32)A_PtrGateWay + 2)), *((__u8*)((__u32)A_PtrGateWay + 3)),
						Tv_Rslt );
				Tv_Result	=	FALSE;
			}
			else
			{
				DbgMsgPrint("GrSockV2SetNetAdr: gateway ( %d.%d.%d.%d ) set\n", 
					*((__u8*)A_PtrGateWay), *((__u8*)((__u32)A_PtrGateWay + 1)), *((__u8*)((__u32)A_PtrGateWay + 2)), *((__u8*)((__u32)A_PtrGateWay + 3)) );
			}

			if ( Tv_Result )
			{
				DbgMsgPrint("GrSockV2SetNetAdr: network address change success.\n");
			}

			//close socket
			close( Tv_Sock );
		}
		else
		{
			DbgMsgPrint("GrSockV2SetNetAdr: socket create error.\n");
		}
#endif

		// notify network change
		Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgIpUpdt;
		Tv_Msg.IfIdx			=	A_DevIdx;
		GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrSockV2GetMyGateway( __u8 A_DevIdx, void* A_PtrRt, BOOL8 A_IsV6 )
{
	// local -------------------
		BOOL8	Tv_Result;
#ifdef LINUX_APP
		__s32		Tv_Sock;
		__s8	Tv_MsgBuf[8192];	
		struct nlmsghdr*	Tv_PtrNlMsg;		//NET LINK message
		struct rtmsg*			Tv_PtrRtMsg;		//route message
		__s32		Tv_MsgSeq;
		__s32		Tv_Pid;
		struct nlmsghdr*	Tv_PtrNlHdr;
		__s32		Tv_ReadLen;
		__s32		Tv_MsgLen;
		__s8*	Tv_PtrBuf;
#endif
	// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
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
					/* receive response from the kernel */
					Tv_ReadLen	=	recv(Tv_Sock, Tv_PtrBuf, sizeof(Tv_MsgBuf) - Tv_MsgLen, 0);
					if( 0 <= Tv_ReadLen )
					{
						Tv_PtrNlHdr = (struct nlmsghdr *)Tv_PtrBuf;
						/* Check if the header is valid */
						if( (NLMSG_OK(Tv_PtrNlHdr, Tv_ReadLen) == 0) || (Tv_PtrNlHdr->nlmsg_type == NLMSG_ERROR))
						{
							DbgMsgPrint("GrSockV2GetMyGateway:Error in packet!\n");
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
						DbgMsgPrint("GrSockV2GetMyGateway:SockNlReadError\n");
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
						/* get the RTATTR field */
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
									*((__u32*)A_PtrRt)	=	*(__u32*)RTA_DATA(Tv_PtrRtAttr);
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
					DbgMsgPrint("GrSockV2GetMyGateway:Message read failure!\n");
				}
			}
			else
			{
				DbgMsgPrint("GrSockV2GetMyGateway: write socket fail!\n");
			}
			//close socket
			close(Tv_Sock);
		}
		else
		{
			DbgMsgPrint("GrSockV2GetMyGateway:socket open error!\n");
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrSockV2ChkCloseByRecv( __s32 A_RecvVal )
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
void		GrSockV2SetCloseTimeOut( Def_GrSock A_Hndl, __u16 A_TimeOutSec )
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
void		GrSockV2TcpNodelySet( Def_GrSock A_Hndl, BOOL8 A_IsOn )
{
	// local -------------------
		__s32		Tv_IsOn;
	// code --------------------
		Tv_IsOn	=	(__s32)A_IsOn;
	#ifdef LINUX_APP
		setsockopt( A_Hndl, SOL_SOCKET, SO_LINGER, &Tv_IsOn, sizeof(Tv_IsOn) );	//no delay
	#else
		setsockopt( A_Hndl, SOL_SOCKET, SO_LINGER, (char*)&Tv_IsOn, sizeof(Tv_IsOn) );	//no delay
	#endif
}
//--------------------------------------------------------------------
__u16		GrSockV2CalcIpV4Chksum( void* A_PtrBuf, __u32 A_Size, void* A_PtrExt, __u32 A_ExtSize )
{
	// local -------------------
		__u32	Tv_Result;
#if 0
		__u16*	Tv_PtrWk;
		__s32		Tv_Last;
		__u8*	Tv_PtrBt;
#endif
		__u32		Tv_ValAdd;
		__u8*		Tv_PtrSr;
		__u8*		Tv_PtrAdd;
		__u32		Tv_Remain;
	// code --------------------
		Tv_Result	=	0;

		Tv_ValAdd	=	0;
		Tv_PtrAdd	=	(__u8*)&Tv_ValAdd;

		// check extention exist
		if((NULL != A_PtrExt) && (0 != A_ExtSize))
		{
			Tv_Remain	=	A_ExtSize;
			Tv_PtrSr	=	(__u8*)A_PtrExt;
			while(1 < Tv_Remain)
			{
				Tv_PtrAdd[1]	=	*Tv_PtrSr;
				Tv_PtrSr ++;
				Tv_PtrAdd[0]	=	*Tv_PtrSr;
				Tv_PtrSr ++;

				Tv_Result		=	Tv_Result + Tv_ValAdd;

				// next
				Tv_Remain	=	Tv_Remain - 2;
			}
		}

		// contents
		Tv_Remain	=	A_Size;
		Tv_PtrSr	=	(__u8*)A_PtrBuf;
		while(1 < Tv_Remain)
		{
			Tv_PtrAdd[1]	=	*Tv_PtrSr;
			Tv_PtrSr ++;
			Tv_PtrAdd[0]	=	*Tv_PtrSr;
			Tv_PtrSr ++;

			Tv_Result		=	Tv_Result + Tv_ValAdd;

			// next
			Tv_Remain	=	Tv_Remain - 2;
		}

		// sum upper 16bit
		while (0 != (Tv_Result & 0xFFFF0000))
		{
			Tv_Result	=	((Tv_Result >> 16) & 0xFFFF) + (Tv_Result & 0xFFFF);
		}
		Tv_Result	=	~Tv_Result;

		Tv_Result	=	GrDumyEndian2byteGet(&Tv_Result);

#if 0
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
#endif
		return	(__u16)Tv_Result;
}
//--------------------------------------------------------------------
__u8		GrSockV2NetIfAdaptarIdGet( __u8 A_DevIdx )
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

		GrSockV2Close( Tv_Hndl );
#endif
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8		GrSockV2WaitConnect( Def_GrSock A_Hndl, __u32 A_TimeOut )
{
	// local -------------------
		__u8	Tv_Result;
		struct timeval	Tv_TmVal;
		fd_set	Tv_FdSet;
		__s32		Tv_Rval;
		__s32		Tv_Err;
	// code --------------------
		Tv_Result	=	E_GrSockV2ConnWaiting;
		
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
#if defined(_WIN32) || defined(_WIN64)
		Tv_Rval	=	select( 0, NULL, &Tv_FdSet, NULL, &Tv_TmVal );	// Window is not used any more nfds.
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
					DbgMsgPrint( "GrSockV2WaitConnect : error - err = %d \n", Tv_Err );
				}
				Tv_Result	=	E_GrSockV2ConnTimeOut;
			}
#else
			Tv_Err	=	WSAGetLastError();
			if ( WSAEWOULDBLOCK != Tv_Err )
			{
				DbgMsgPrint( "GrSockV2WaitConnect : error - err = %d \n", Tv_Err );
				Tv_Result	=	E_GrSockV2ConnTimeOut;
			}
#endif
		}
		else if ( 0 != Tv_Rval )
		{
			Tv_Result	=	E_GrSockV2ConnOk;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrSockV2UdpBroad(Def_GrSock A_Hndl, void* A_PtrIpBrd, __u8 A_DevIdx, BOOL8 A_IsV6)
{
	// local -------------------
		//int			Tv_IsBroad;
		BOOL		Tv_IsReuse;
		ip_mreq	Tv_Mreq;
		__u32		Tv_Ttl;
		sockaddr_in	Tv_NetAdr;
	// code --------------------
		//set mode
#if 0
		Tv_IsBroad	=	1;
		if(0 > setsockopt(A_Hndl, SOL_SOCKET, SO_BROADCAST, (char*)&Tv_IsBroad, sizeof(Tv_IsBroad)))
		{
			return	FALSE;
		}
#endif

		Tv_IsReuse	=	TRUE;
		if(0 > setsockopt(A_Hndl, SOL_SOCKET, SO_REUSEADDR, (char*)&Tv_IsReuse, sizeof(Tv_IsReuse)))
		{
			return	FALSE;
		}

		GrDumyZeroMem(&Tv_Mreq, sizeof(Tv_Mreq));

#ifdef LINUX_APP
		Tv_Mreq.imr_multiaddr.s_addr	=	*((__u32*)A_PtrIpBrd);
		Tv_Mreq.imr_interface.s_addr	=	INADDR_ANY;
#else
		Tv_Mreq.imr_multiaddr.S_un.S_addr	=	*((__u32*)A_PtrIpBrd);
		Tv_Mreq.imr_interface.S_un.S_addr	=	INADDR_ANY;
#endif
		if(0 > setsockopt(A_Hndl, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&Tv_Mreq, sizeof(Tv_Mreq)))
		{
			return	FALSE;
		}

		Tv_Ttl		=	1;
		if(0 > setsockopt(A_Hndl, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&Tv_Ttl, sizeof(Tv_Ttl)))
		{
			return	FALSE;
		}

		GrDumyZeroMem(&Tv_NetAdr, sizeof(Tv_NetAdr));
		Tv_NetAdr.sin_family			=	AF_INET;
		GrSockV2GetMyIp(A_DevIdx, &Tv_NetAdr.sin_addr.s_addr, FALSE);
		//Tv_NetAdr.sin_addr.s_addr	=	*((__u32*)A_PtrIpBrd);
		Tv_NetAdr.sin_port				=	0;
		if(0 > bind(A_Hndl, (sockaddr*)&Tv_NetAdr, sizeof(Tv_NetAdr)))
		{
			return	FALSE;
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void		GrSockV2Init( void )
{
	// local -------------------
#ifndef LINUX_APP
		WSADATA		Tv_WsaData;
#endif
	// code --------------------
#ifdef LINUX_APP
		sigset(SIGPIPE, SIG_IGN);
#else
		//WSAStartup(MAKEWORD(1,0), &Tv_WsaData);
		WSAStartup(MAKEWORD(2, 2), &Tv_WsaData);
#endif

}
//--------------------------------------------------------------------
void		GrSockV2Finish( void )
{
#ifndef	LINUX_APP
		WSACleanup();
#endif
}
//--------------------------------------------------------------------
__u32	GrSockV2DnsRrNameInfo(Ptr_GrSockV2DnsRrParsEnv A_PtrEnv, void** A_PtrRtPtr)
{
	// local -------------------
		__u32	Tv_Result;
		__u8*	Tv_PtrWp;		// work position
		__u32	Tv_Remain;
		__u32	Tv_LenThis;
	// code --------------------
		// init
		Tv_Result		=	0;
		*A_PtrRtPtr	=	NULL;

		Tv_PtrWp	=	(__u8*)((Def_GrCalPtr)A_PtrEnv->PtrRr + (Def_GrCalPtr)A_PtrEnv->Prgs);
		Tv_Remain	=	A_PtrEnv->RrSize - A_PtrEnv->Prgs;

		// check pointer
		if(0xC0 == (*Tv_PtrWp))
		{
			// reference addrss
			*A_PtrRtPtr	=	(void*)((Def_GrCalPtr)A_PtrEnv->PtrRr + (Def_GrCalPtr)Tv_PtrWp[1]);
			Tv_Result	=	Tv_Result + 2;
			Tv_PtrWp	=	(__u8*)((Def_GrCalPtr)Tv_PtrWp + 2);
		}
		else
		{
			// direct address
			*A_PtrRtPtr	=	(void*)Tv_PtrWp;
			while(0 != Tv_PtrWp[0])
			{
				// check size
				Tv_LenThis	=	(__u32)Tv_PtrWp[0] + 1;
				if(Tv_Remain <= Tv_LenThis)
				{
					// break data
					Tv_Result	=	Tv_Result + Tv_Remain;
					break;
				}
				// next
				Tv_PtrWp	=	(__u8*)((Def_GrCalPtr)Tv_PtrWp + (Def_GrCalPtr)Tv_LenThis);
				Tv_Result	=	Tv_Result + Tv_LenThis;
				Tv_Remain	=	Tv_Remain - Tv_LenThis;
			}
			// mark finish
			Tv_Result ++;
			Tv_PtrWp	++;
			Tv_Remain --;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrSockV2DnsRrParsQuery(Ptr_GrSockV2DnsRrParsEnv A_PtrEnv, Ptr_GrSockV2DnsRrParsQueryInfo A_PtrRt)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_FitLen;
		__u16*	Tv_PtrU16;
	// code --------------------
		Tv_Result	=	FALSE;

		A_PtrRt->Class			=	0;
		A_PtrRt->Type			=	0;
		A_PtrRt->PtrName		=	NULL;

		// get name position
		Tv_FitLen	=	GrSockV2DnsRrNameInfo(A_PtrEnv, &A_PtrRt->PtrName);
		if((Tv_FitLen + 4 + A_PtrEnv->Prgs) <= A_PtrEnv->RrSize)
		{
			// go next pointer
			A_PtrEnv->Prgs		=	A_PtrEnv->Prgs + Tv_FitLen;
			
			// get type and class
			Tv_PtrU16	=	(__u16*)((Def_GrCalPtr)A_PtrEnv->PtrRr + (Def_GrCalPtr)A_PtrEnv->Prgs);
			A_PtrRt->Type		=	Tv_PtrU16[0];	// type
			A_PtrRt->Class		=	Tv_PtrU16[1];	// class

			// go next pointer
			A_PtrEnv->Prgs		=	A_PtrEnv->Prgs + 4;

			// success
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
