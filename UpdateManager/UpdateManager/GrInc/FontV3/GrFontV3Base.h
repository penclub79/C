/*
Font version 3 base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrFontV3Base
#define	_EPC_GrFontV3Base

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

//====================================================================
//constance

#define	E_GrFontV3ScptFcc			Mac_GrMakeFcc('F','N','T','3')

#define	E_GrFontV3AlignLeft					0
#define	E_GrFontV3AlignRight				1
#define	E_GrFontV3AlignCenterHz			2

#define	E_GrFontV3AlignTop					0
#define	E_GrFontV3AlignBottom				1
#define	E_GrFontV3AlignCenterVt			2

#define	E_GrFontV3AtbDrawText				0x01
#define	E_GrFontV3AtbDrawShadow			0x02
#define	E_GrFontV3AtbWordWarp				0x04
#define	E_GrFontV3AtbFixPitch				0x08
#define	E_GrFontV3AtbPassword				0x10
#define	E_GrFontV3AtbSemiEnter			0x20

#define	E_GrFontV3StatNormal				0
#define	E_GrFontV3StatDisable				1
#define	E_GrFontV3StatOnFocus				0x0100
#define	E_GrFontV3StatOnChecked			0x0200
#define	E_GrFontV3StatOnSelected		0x0400

#define	E_GrFontV3TxtClrNormal			0
#define	E_GrFontV3TxtClrDisable			1
#define	E_GrFontV3TxtClrFocus				2
#define	E_GrFontV3TxtClrSelect			3
#define	E_GrFontV3TxtClrCnt					4

#define	E_FontV3DrawCodeInsteadEnter				0x3B


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrFontV3ScptHead
{
	__u32	Fcc;
	__u32	ClassId;

}	*Ptr_GrFontV3ScptHead;

typedef struct St_GrFontV3DrawStyle
{
	__u8	AlignHz;
	__u8	AlignVt;
	__s8	GapChar;
	__s8	GapLine;

	__u8	Atb;
	__u8	_rsvd0;
	__u16	CodePw;

	__u32	ColorShadow;
	__u32	_rsvd1;

	__u32	ColorTxtTbl[E_GrFontV3TxtClrCnt];

}	*Ptr_GrFontV3DrawStyle;

//====================================================================
//global class


//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

