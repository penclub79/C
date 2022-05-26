/*
	GAUSE UART control


*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrUart
#define	_EPC_GrUart

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//const
#define E_GrUartMaxCh								4				/* UART max channel */

#define E_GrUartCfgStopBit1					0
#define E_GrUartCfgStopBit2					1
#define E_GrUartCfgStopBit1_5				2

#define E_GrUartCfgParityBitNone		0
#define E_GrUartCfgParityBitEven		1
#define E_GrUartCfgParityBitOdd			2

//====================================================================
//type


#ifndef LINUX_APP

typedef	BOOL8(*Fnc_GrUartSetup)(HANDLE, __u32, __u8, __u8, __u8);
typedef	__u32(*Fnc_GrUartSend)(HANDLE, void*, __u32, BOOL8);
typedef	__u32(*Fnc_GrUartRecv)(HANDLE, void*, __u32, __u32);

#else
typedef	BOOL8	(*Fnc_GrUartSetup)( __u32, __u32, __u8, __u8, __u8 );
typedef	__u32	(*Fnc_GrUartSend)( __u32, void*, __u32, BOOL8 );
typedef	__u32	(*Fnc_GrUartRecv)( __u32, void*, __u32, __u32 );
#endif

typedef	struct St_GrUartOps 
{
	Fnc_GrUartSetup	Setup;
	Fnc_GrUartSend	Send;
	Fnc_GrUartRecv	Recv;
}	*Ptr_GrUartOps;

typedef	struct St_GrUartChItm 
{
	BOOL8	IsExist;
	BOOL8	IsOsPort;	// OS port function
	__u8	DataBit;
	__u8	Parity;
#ifndef LINUX_APP
	HANDLE	Id;
#else
	__u32	Id;				// user define value - OS port mode = handle
#endif
	__u32	Buad;
	__u8	StopBit;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	St_GrUartOps	Ops;
}	*Ptr_GrUartChItm;

//====================================================================
//class


//====================================================================
//function

BOOL8	GrUartIsRegisted( __u8 A_Ch );

#if defined(_WIN64)
BOOL8	GrUartRegist( __u8 A_Ch, Ptr_GrUartOps A_PtrOps, __u64 A_Id );
#elif defined(_WIN32)
BOOL8	GrUartRegist(__u8 A_Ch, Ptr_GrUartOps A_PtrOps, __u32 A_Id);
#else
BOOL8	GrUartRegist(__u8 A_Ch, Ptr_GrUartOps A_PtrOps, __u32 A_Id);
#endif

BOOL8	GrUartOsOpsRegist( __u8 A_Ch, __u8 A_Port );
BOOL8	GrUartUnregist( __u8 A_Ch );
BOOL8	GrUartSetup( __u8 A_Ch, __u32 A_Speed, __u8 A_DataBit, __u8 A_ParityBit, __u8 A_StopBit	);
__u32	GrUartSend( __u8 A_Ch, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsWait = FALSE );
__u32	GrUartRecv( __u8 A_Ch, void* A_PtrBuf, __u32 A_Size, __u32 A_TimeOut = 0 );

//====================================================================
//out Environment conditional
#endif
