/*
	linux on win32 emulation
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_GrSocket
#define	_GrSocket

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrSockHd.h>

#ifdef LINUX_APP
	#include <unistd.h>
#endif
#include	<GrProcess.h>

//====================================================================
//constance

#define E_GrSockConnOk				0		/* connect ok */
#define E_GrSockConnWaiting		1		/* waiting non block socket */
#define E_GrSockConnTimeOut		2		/* time out */
#define E_GrSockConnBroken		3		/* socket broken */

//====================================================================
//global macro


#ifdef WIN32
	#define GrSockClose(A_Hndl)	closesocket((A_Hndl))
	#define GrSockSend(A_Hndl,A_PtrBuf,A_Len)	send( (SOCKET)(A_Hndl), (char*)(A_PtrBuf), (int)(A_Len) , 0 )
	#define GrSockRecv(A_Hndl,A_PtrBuf,A_Len)	recv( (SOCKET)(A_Hndl), (char*)(A_PtrBuf), (int)(A_Len) , 0 )
#else
	#define GrSockClose(A_Hndl)	close((A_Hndl))
	#define GrSockSend(A_Hndl,A_PtrBuf,A_Len)	write( (A_Hndl), (A_PtrBuf), (A_Len) )
	#define GrSockRecv(A_Hndl,A_PtrBuf,A_Len)	read( (A_Hndl), (A_PtrBuf), (A_Len) )
#endif

//====================================================================
//global type

//====================================================================
//class

//====================================================================
//global function

void		GrSockInit( void );
void		GrSockFinish( void );
Def_GrSock	GrSockOpen( BOOL8 A_IsUdp, BOOL8 A_IsNonblock );
BOOL8		GrSockSetListenMode( Def_GrSock A_Hndl, __u16 A_Port );
void		GrSockSetCloseTimeOut( Def_GrSock A_Hndl, __u16 A_TimeOutSec = 0xFFFF );
void		GrSockSetBlockMode( Def_GrSock A_Hndl, BOOL8 A_IsNonBlock );
Def_GrSock	GrSockAccept( Def_GrSock A_HndlListen, BOOL8 A_IsNonblock, 
													__u32* A_PtrRtIp, __u16* A_PtrRtPort );
__u8		GrSockConnect( Def_GrSock A_Hndl, __u32 A_Ip, __u16 A_Port );
__u32		GrSockGetErrCode( void );
__u64		GrSockGetMacAddr( __u8 A_DevIdx );
__u32		GrSockGetMyIp( __u8 A_DevIdx = 0 );
__u32		GrSockGetMyNetmask( __u8 A_DevIdx = 0 );
__u32		GrSockGetMyGateway( __u8 A_DevIdx = 0 );
BOOL8		GrSockChkCloseByRecv( __s32 A_RecvVal );
void		GrSockTcpNodelySet( Def_GrSock A_Hndl, BOOL8 A_IsOn );
__u8		GrSockWaitConnect( Def_GrSock A_Hndl, __u32 A_TimeOut );

//BOOL8		GrSockGetIpByName( char* A_StrAdr, __u32* A_PtrRtIp );
//dns - new
void		GrSockSetMacAddr( __u8 A_DevIdx, void* A_PtrMac );

BOOL8		GrSockUdpPortAssign( Def_GrSock A_Hndl, __u16 A_Port );
BOOL8		GrSockUdpIpAssign( Def_GrSock A_Hndl, __u32 A_Ip );
__u32		GrSockUdpSend( Def_GrSock A_Hndl, __u32 A_Ip, __u16 A_Port, void* A_PtrData, __u32 A_SizeData );
__u32		GrSockUdpRead( Def_GrSock A_Hndl, __u32 *A_PtrIp, __u16 A_Port, void* A_PtrData, __u32 A_SizeData );
BOOL8		GrSockUdpBroad( Def_GrSock A_Hndl );

void		GrSockSetNetAdr( __u8 A_DevIdx, __u32 A_Ip, __u32 A_NetMask, __u32 A_GateWay );
void		GrSockSetDns( __u32 A_Dns );

BOOL8		GrSockSetupByDhcp( __u8 A_NetIf = 0 );
BOOL8		GrSockDhcpReq( __u8 A_NetIf = 0 );			//request dhcp
BOOL8		GrSockDhcpResult( __u32* A_PtrRtIp, __u32* A_PtrRtMask, __u32* A_PtrRtGate, __u32* A_PtrRtDns );
void		GrSockDhcpStop( void );

BOOL8		GrSockIsLanCableOk( __s8 A_ConnectorIdx );
BOOL8		GrSockNetIfActive( __s8 A_DevIdx, BOOL8 A_IsOn );
BOOL8		GrSockNetIfMtuSet( __s8 A_DevIdx, __s32 A_Mtu );
__u8		GrSockNetIfAdaptarIdGet( __u8 A_DevIdx );

BOOL8		GrSockFreeDnsUpdt(__u8 A_DevIdx, __u8	A_HostType, char* A_StrId, char* A_StrPass, char* A_StrDomain);
BOOL8		GrSockFreeDnsUpdt( __u8 A_DevIdx, __u8	A_HostType, WCHAR* A_StrId, WCHAR* A_StrPass, WCHAR* A_StrDomain );

__u16		GrSockCalcChksum( void* A_PtrBuf, __u32 A_Size, void* A_PtrExt = NULL, __u32 A_ExtSize = NULL );

//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif


