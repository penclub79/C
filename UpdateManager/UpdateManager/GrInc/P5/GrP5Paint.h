/*
	Platform 5 Paint

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5Paint
#define	_EPC_GrP5Paint

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrGdibBase.h>

//====================================================================
//constance

#define	E_GrP5PntItemSize				16

#define	E_GrP5PntTxtClrCnt			4

#define	E_GrP5PntCmdSection			1
#define	E_GrP5PntCmdClip				2
#define	E_GrP5PntCmdFillRect		3
#define	E_GrP5PntCmdLine				4
#define	E_GrP5PntCmdPixel				5
#define	E_GrP5PntCmdImage				6
#define	E_GrP5PntCmdTxtStyle0		7
#define	E_GrP5PntCmdTxtStyle1		8
#define	E_GrP5PntCmdTxtCus			9
#define	E_GrP5PntCmdTxtDraw			10
#define	E_GrP5PntCmdDrawRect		11

//====================================================================
//global macro


//====================================================================
//global type

typedef struct St_GrP5PntCmdBase
{
	__u8	Cmd;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	_rsvd3[3];
}	*Ptr_GrP5PntCmdBase;

typedef struct St_GrP5PntCmdSection
{
	__u8	Cmd;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	SctIdx;

	__u32	_rsvd2;

	St_GrSwordRect	Rect;
}	*Ptr_GrP5PntCmdSection;

typedef struct St_GrP5PntCmdClip
{
	__u8	Cmd;
	BOOL8	IsValid;
	__u8	_rsvd0;
	__u8	_rsvd1;

	__u32	ClipIdx;

	St_GrSwordRect	Rect;
}	*Ptr_GrP5PntCmdClip;

typedef struct St_GrP5PntCmdFillRect
{
	__u8	Cmd;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	Color;
	St_GrSwordRect	Rect;
}	*Ptr_GrP5PntCmdFillRect;

typedef struct St_GrP5PntCmdLine
{
	__u8	Cmd;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	Color;

	St_GrSwordPos	St;
	St_GrSwordPos	Ed;

}	*Ptr_GrP5PntCmdLine;

typedef struct St_GrP5PntCmdPixel
{
	__u8	Cmd;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	Color;

	__s32	X;
	__s32	Y;

}	*Ptr_GrP5PntCmdPixel;

typedef struct St_GrP5PntCmdImage
{
	__u8	Cmd;
	BOOL8	IsSprite;
	__u8	_rsvd0;
	__u8	_rsvd1;

	St_GrSwordPos	Pos;

	Ptr_GrGdib	PtrGdib;

	__u32	_rsvd2;
}	*Ptr_GrP5PntCmdImage;

typedef struct St_GrP5PntCmdTxtStyle0
{
	__u8	Cmd;
	__u8	Atb;
	__u8	AlignHz;
	__u8	AlignVt;

	__s8	GapChar;
	__s8	GapLine;
	__u16	CodePw;

	__u32	ClrShadow;
	__u32	ClrTxt0;
}	*Ptr_GrP5PntCmdTxtStyle0;

typedef struct St_GrP5PntCmdTxtStyle1
{
	__u8	Cmd;
	__u8	FontId;
	__u8	_rsvd0;
	__u8	_rsvd1;

	__u32	ClrTxt1;
	__u32	ClrTxt2;
	__u32	ClrTxt3;

}	*Ptr_GrP5PntCmdTxtStyle1;

typedef struct St_GrP5PntCmdTxtCus
{
	__u8	Cmd;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__s32	Pos;
	__u32	Color;

	__u32	_rsvd3;

}	*Ptr_GrP5PntCmdTxtCus;

typedef struct St_GrP5PntCmdTxtDraw
{
	__u8	Cmd;
	__u8	_rsvd0;
	__u16	Stat;
	
	WCHAR*	Str;

	St_GrSwordRect	Rect;
}	*Ptr_GrP5PntCmdTxtDraw;

typedef struct St_GrP5PntCmdDrawRect
{
	__u8	Cmd;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	Color;
	St_GrSwordRect	Rect;
}	*Ptr_GrP5PntCmdDrawRect;


//====================================================================
//global class


//====================================================================
//global function

void	GrP5PntReset(void);

BOOL8	GrP5PntSectionStart(Ptr_GrRect A_PtrRect);

BOOL8	GrP5PntClipSet(Ptr_GrRect A_PtrRect, Ptr_GrRect A_PtrRtRcOrg);
void	GrP5PntClipRestore(Ptr_GrRect A_PtrRect);

void	GrP5PntArrangeByRect(Ptr_GrRect A_PtrRect);

BOOL8	GrP5PntReqFillRect(Ptr_GrRect A_PtrRect, __u32 A_Color);
BOOL8	GrP5PntReqLine(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_Color);
BOOL8	GrP5PntReqPixel(__s32 A_X, __s32 A_Y, __u32 A_Color);
BOOL8	GrP5PntReqImage(Ptr_GrGdib A_PtrGdib, __s32 A_X, __s32 A_Y, BOOL8 A_IsSprite);
BOOL8	GrP5PntReqTxtStyle(void* A_PtrFontDrawStyle, __u8 A_FontId);
BOOL8	GrP5PntReqTxtCus(__s32 A_CusPos = -1, __u32 A_ClrCus = 0);
BOOL8	GrP5PntReqTxtDraw(Ptr_GrRect A_PtrRect, WCHAR* A_Str, __u16 A_Stat);
BOOL8	GrP5PntReqDrawRect(Ptr_GrRect A_PtrRect, __u32 A_Color);

//====================================================================
//out Environment conditional
#endif

