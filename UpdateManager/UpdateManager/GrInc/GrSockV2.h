/*
	GAUSE socket version 2
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_GrSockV2
#define	_GrSockV2

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrSockHd.h>

#ifdef LINUX_APP
	#include <unistd.h>
#endif

#include <GrError.h>

#if defined(_WIN32) || defined(_WIN64)

#pragma	comment(lib, "ws2_32.lib")

#endif

//====================================================================
//constance

#define E_GrSockV2ConnOk				0		/* connect ok */
#define E_GrSockV2ConnWaiting		1		/* waiting non block socket */
#define E_GrSockV2ConnTimeOut		2		/* time out */
#define E_GrSockV2ConnBroken		3		/* socket broken */

//====================================================================
//global macro


#if defined(_WIN32) || defined(_WIN64)
	#define GrSockV2Close(A_Hndl)	closesocket((A_Hndl))
	#define GrSockV2Send(A_Hndl,A_PtrBuf,A_Len)	send( (SOCKET)(A_Hndl), (char*)(A_PtrBuf), (int)(A_Len) , 0 )
	#define GrSockV2Recv(A_Hndl,A_PtrBuf,A_Len)	recv( (SOCKET)(A_Hndl), (char*)(A_PtrBuf), (int)(A_Len) , 0 )
#else
	#define GrSockV2Close(A_Hndl)	close((A_Hndl))
	#define GrSockV2Send(A_Hndl,A_PtrBuf,A_Len)	write( (A_Hndl), (A_PtrBuf), (A_Len) )
	#define GrSockV2Recv(A_Hndl,A_PtrBuf,A_Len)	read( (A_Hndl), (A_PtrBuf), (A_Len) )
#endif

//====================================================================
//global type

typedef struct St_GrSockV2DnsRrParsEnv
{
	void*	PtrRr;
	__u32	RrSize;
	__u32	Prgs;
}	*Ptr_GrSockV2DnsRrParsEnv;

typedef struct St_GrSockV2DnsRrParsQueryInfo
{
	void*	PtrName;
	__u16	Type;
	__u16	Class;
}	*Ptr_GrSockV2DnsRrParsQueryInfo;

//====================================================================
//class

//====================================================================
//global function
void		GrSockV2Init( void );
void		GrSockV2Finish( void );

Def_GrSock	GrSockV2Open( BOOL8 A_IsUdp, BOOL8 A_IsNonblock, BOOL8 A_IsV6 );
BOOL8		GrSockV2SetListenMode( Def_GrSock A_Hndl, __u16 A_Port, BOOL8 A_IsV6, __u32 A_QueCnt = 5 );
void		GrSockV2SetCloseTimeOut( Def_GrSock A_Hndl, __u16 A_TimeOutSec = 0xFFFF );
void		GrSockV2SetBlockMode( Def_GrSock A_Hndl, BOOL8 A_IsNonBlock );
Def_GrSock	GrSockV2Accept( Def_GrSock A_HndlListen, BOOL8 A_IsNonblock, 
													void* A_PtrRtIp, __u16* A_PtrRtPort, BOOL8* A_PtrRtIsV6 );
__u8		GrSockV2Connect( Def_GrSock A_Hndl, void* A_PtrIp, __u16 A_Port, BOOL8 A_IsV6 );
__u32		GrSockV2GetErrCode( void );
BOOL8		GrSockV2GetMacAddr( __u8 A_DevIdx, __u8* A_PtrRt );
BOOL8		GrSockV2SetMacAddr(__u8 A_DevIdx, __u8* A_PtrMac);
BOOL8		GrSockV2GetMyIp( __u8 A_DevIdx, void* A_PtrRt, BOOL8 A_IsV6 );
BOOL8		GrSockV2GetMyNetmask( __u8 A_DevIdx, void* A_PtrRt, BOOL8 A_IsV6 );
BOOL8		GrSockV2GetMyGateway( __u8 A_DevIdx, void* A_PtrRt, BOOL8 A_IsV6 );
BOOL8		GrSockV2ChkCloseByRecv( __s32 A_RecvVal );
void		GrSockV2TcpNodelySet( Def_GrSock A_Hndl, BOOL8 A_IsOn );
__u8		GrSockV2WaitConnect( Def_GrSock A_Hndl, __u32 A_TimeOut );

BOOL8		GrSockV2UdpPortAssign( Def_GrSock A_Hndl, __u16 A_Port );
BOOL8		GrSockV2UdpIpAndPortAssign( Def_GrSock A_Hndl, void* A_PtrIp, __u16 A_Port, BOOL8 A_IsV6 );
__u32		GrSockV2UdpSend( Def_GrSock A_Hndl, void* A_PtrIp, __u16 A_Port, void* A_PtrData, __u32 A_SizeData, BOOL8 A_IsV6 );
__u32		GrSockV2UdpRead(Def_GrSock A_Hndl, void* A_PtrRtIp, __u16* A_PtrRtPort, void* A_PtrData, __u32 A_SizeData,
	 BOOL8 A_IsV6, Def_GrErrCode* A_PtrRtErrCode);
BOOL8		GrSockV2UdpBroad(Def_GrSock A_Hndl, void* A_PtrIpBrd, __u8 A_DevIdx, BOOL8 A_IsV6);

BOOL8		GrSockV2SetNetAdr( __u8 A_DevIdx, void* A_PtrIp, void* A_PtrNetMask, void* A_PtrGateWay, BOOL8 A_IsV6 );

BOOL8		GrSockV2IsLink( __s8 A_DevIdx );
BOOL8		GrSockV2NetIfActive( __s8 A_DevIdx, BOOL8 A_IsOn );
BOOL8		GrSockV2NetIfMtuSet( __s8 A_DevIdx, __s32 A_Mtu );
__u8		GrSockV2NetIfAdaptarIdGet( __u8 A_DevIdx );

__u16		GrSockV2CalcIpV4Chksum( void* A_PtrBuf, __u32 A_Size, void* A_PtrExt = NULL, __u32 A_ExtSize = 0 );

// DNS parse
__u32	GrSockV2DnsRrNameInfo(Ptr_GrSockV2DnsRrParsEnv A_PtrEnv, void** A_PtrRtPtr);
BOOL8	GrSockV2DnsRrParsQuery(Ptr_GrSockV2DnsRrParsEnv A_PtrEnv, Ptr_GrSockV2DnsRrParsQueryInfo A_PtrRt);


//====================================================================
//global variable

//====================================================================

//out Environment conditional
#endif


