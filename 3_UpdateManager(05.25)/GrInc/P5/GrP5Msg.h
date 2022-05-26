/*
	Platform 5 message

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5Msg
#define	_EPC_GrP5Msg

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrTaskBase.h>

//====================================================================
//constance

#define Mac_GrP5MsgMake(ch0, ch1)	\
	((__u32)(ch0) | ((__u32)(ch1) << 24))

// system
#define	E_GrP5MsgCmdTypeSys								1
#define	E_GrP5MsgCmdConRegist							Mac_GrP5MsgMake(0,E_GrP5MsgCmdTypeSys)	/* St_GrP5MsgConRegist */
#define	E_GrP5MsgCmdConUnregist						Mac_GrP5MsgMake(1,E_GrP5MsgCmdTypeSys)	/* St_GrP5MsgConUnregist */

#define	E_GrP5MsgCmdInpKey								Mac_GrP5MsgMake(2,E_GrP5MsgCmdTypeSys)	/* St_GrP5MsgInpKey */
#define	E_GrP5MsgCmdInpMsePosRel					Mac_GrP5MsgMake(3,E_GrP5MsgCmdTypeSys)	/* St_GrP5MsgInpMsePosRel */
#define	E_GrP5MsgCmdInpMsePosAbs					Mac_GrP5MsgMake(4,E_GrP5MsgCmdTypeSys)	/* St_GrP5MsgInpMsePosAbs */
#define	E_GrP5MsgCmdInpMseBtn							Mac_GrP5MsgMake(5,E_GrP5MsgCmdTypeSys)	/* St_GrP5MsgInpMseBtn */
#define	E_GrP5MsgCmdInpMseWheel						Mac_GrP5MsgMake(6,E_GrP5MsgCmdTypeSys)	/* St_GrP5MsgInpMseWheel */

#define	E_GrP5MsgCmdPntCodeBld						Mac_GrP5MsgMake(7,E_GrP5MsgCmdTypeSys)	/* St_GrP5MsgPntCodeBld */

#define	E_GrP5MsgCmdKeyFcsUoSet						Mac_GrP5MsgMake(8,E_GrP5MsgCmdTypeSys)	/* St_GrP5MsgKeyFcsUoSet */
#define	E_GrP5MsgCmdKeyFcsCorrect					Mac_GrP5MsgMake(9,E_GrP5MsgCmdTypeSys)	/* St_GrP5MsgHd */

#define	E_GrP5MsgCmdAutoLogoutSet					Mac_GrP5MsgMake(10,E_GrP5MsgCmdTypeSys)	/* St_GrP5MsgAtLogoutSet */


// console

#define	E_GrP5MsgCmdTypeCon								2

#define	E_GrP5MsgCmdConWinCloseReq				Mac_GrP5MsgMake(0,E_GrP5MsgCmdTypeCon)

// ui
#define	E_GrP5MsgCmdTypeUiObj							100
#define	E_GrP5MsgCmdUiTabPageSet					Mac_GrP5MsgMake(0,E_GrP5MsgCmdTypeUiObj)		/* St_GrP5MsgUiTabPageSet */
#define	E_GrP5MsgCmdUiTabPagePop					Mac_GrP5MsgMake(1,E_GrP5MsgCmdTypeUiObj)		/* St_GrP5MsgHd */


//====================================================================
//global macro

#define Mac_GrP5MsgTypeGet(ch0)	\
	(((__u32)(ch0) >> 24) & 0xFF)

//====================================================================
//global type

typedef struct St_GrP5MsgHd
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrP5MsgHd;

typedef struct St_GrP5MsgConRegist
{
	St_GrP5MsgHd	CmdHd;
	BOOL8*	PtrRslt;
	__u8	ConIdx;
	void*	ObjCon;
}	*Ptr_GrP5MsgConRegist;

typedef struct St_GrP5MsgConUnregist
{
	St_GrP5MsgHd	CmdHd;
	BOOL8*	PtrRslt;
	__u8	ConIdx;
}	*Ptr_GrP5MsgConUnregist;

typedef struct St_GrP5MsgInpKey
{
	St_GrP5MsgHd	CmdHd;
	__u8	DevId;
	__u8	Key;
	BOOL8	IsPush;
}	*Ptr_GrP5MsgInpKey;

typedef struct St_GrP5MsgInpMsePosRel
{
	St_GrP5MsgHd	CmdHd;
	__s32	X;
	__s32	Y;
}	*Ptr_GrP5MsgInpMsePosRel;

typedef struct St_GrP5MsgInpMsePosAbs
{
	St_GrP5MsgHd	CmdHd;
	__s32	X;
	__s32	Y;
}	*Ptr_GrP5MsgInpMsePosAbs;

typedef struct St_GrP5MsgInpMseBtn
{
	St_GrP5MsgHd	CmdHd;
	__u8	BtnIdx;
	BOOL8	IsPush;
}	*Ptr_GrP5MsgInpMseBtn;

typedef struct St_GrP5MsgInpMseWheel
{
	St_GrP5MsgHd	CmdHd;
	__u8	WheelIdx;
	BOOL8	IsDown;
}	*Ptr_GrP5MsgInpMseWheel;

typedef struct St_GrP5MsgConHd
{
	St_GrP5MsgHd	CmdHd;
	__u8	ConIdx;
}	*Ptr_GrP5MsgConHd;

typedef struct St_GrP5MsgConWinCloseReq
{
	St_GrP5MsgConHd	ConHd;
	__u16	ObjId;
}	*Ptr_GrP5MsgConWinCloseReq;

typedef struct St_GrP5MsgUiHd
{
	St_GrP5MsgHd	CmdHd;
	__u16	ObjId;
}	*Ptr_GrP5MsgUiHd;

typedef struct St_GrP5MsgUiTabPageSet
{
	St_GrP5MsgUiHd	UiHd;
	__u16	ScptId;
	BOOL8	IsPush;
	__u8	_rsvd0;
}	*Ptr_GrP5MsgUiTabPageSet;

typedef struct St_GrP5MsgPntCodeBld
{
	St_GrP5MsgHd	CmdHd;
	BOOL8*	PtrRtRslt;
	__u8		ConIdx;
}	*Ptr_GrP5MsgPntCodeBld;

typedef struct St_GrP5MsgKeyFcsUoSet
{
	St_GrP5MsgHd	CmdHd;
	__u16	ObjId;
	__u16	_rsvd0;
	__u32	Para;
}	*Ptr_GrP5MsgKeyFcsUoSet;

typedef struct St_GrP5MsgAtLogoutSet
{
	St_GrP5MsgHd	CmdHd;
	__u32	ExpSec;
}	*Ptr_GrP5MsgAtLogoutSet;



//====================================================================
//global class


//====================================================================
//global function



//====================================================================
//out Environment conditional
#endif

