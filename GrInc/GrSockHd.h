/*
	socket header
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_GrSockHd
#define	_GrSockHd

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>


//====================================================================
//constance

#define E_GrSockErrNone						0
#define E_GrSockErrWouldblock			1
#define E_GrSockErrConnBroken			2
#define E_GrSockErrEtc						0x70000000

#define E_GrSockPrtcTypeIp				8		/* protocol type ip */

#if defined(_WIN64)
#define E_GrSockBadHandle				0xFFFFFFFFFFFFFFFF
#elif defined(_WIN32)
#define E_GrSockBadHandle				0xFFFFFFFF
#else
#define E_GrSockBadHandle				-1
#endif

#define E_GrSockIpv4AddrAny				INADDR_ANY
#define E_GrSockIpv4AddrSelf			0x0100007F
#define E_GrSockIpv4AddrBroad			0xFFFFFFFF

//====================================================================
//global macro

#define Mac_GrSockMakeIp(ch0, ch1, ch2, ch3)	\
	((__u32)(__u8)(ch0) | ((__u32)(__u8)(ch1) << 8) | \
	((__u32)(__u8)(ch2) << 16) | ((__u32)(__u8)(ch3) << 24 ))


//====================================================================
//global type
#if defined(_WIN32) || defined(_WIN64)

typedef	int	socklen_t;
typedef	SOCKET		Def_GrSock;			/* socket */

#else

typedef	__s32			Def_GrSock;			/* socket */

#endif

typedef	struct St_GrSockAddrPkt		/* address packet type  */
{
	__u16	Family;			/* Always AF_PACKET */
	__u16	Protocol;		/* Physical layer protocol */
	__s32   IfIdx;			/* Interface number */
	__u16	HdType;			/* Header type */
	__u8	PktType;		/* Packet type */
	__u8	AdrLen;     /* Length of address */
	__u8  AdrBuf[8];  /* Physical layer address */
}	*Ptr_GrSockAddrPkt;

typedef	struct St_GrSockIpHead
{
	__u8	HlenVer;		//high 4bit = ipv4(4) , low 4bit = dword count (5 = 20byte)
	__u8  Service;		//service type
	__u16  TtLen;			//total length
	__u16  Id;
	__u16  FragOfs;		//fragment offset
	__u8  Ttl;			
	__u8  Protocol;
	__u16  ChkSum;
	__u32 SrcIp;
	__u32 DstIp;
}	*Ptr_GrSockIpHead;

typedef	struct St_GrSockUdpHead 
{
	__u16	SrcPort;
	__u16	DstPort;
	__u16	Len;
	__u16	ChkSum;
}	*Ptr_GrSockUdpHead;

typedef	struct St_GrSockCsVtHead		//checksum virtual header 
{
	__u32	SrcIp;
	__u32	DstIp;
	__u8	Zero;
	__u8	Protocol;
	__u16	Len;				//length
}	*Ptr_GrSockCsVtHead;


//====================================================================
//class

//====================================================================
//global function


//====================================================================
//global variable

//====================================================================

//out Environment conditional
#endif


