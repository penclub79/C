/*
	Font version 3 DIB2 type base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrFontV3Dib2Base
#define	_EPC_GrFontV3Dib2Base

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrFontV3Dib2ClassId				Mac_GrMakeFcc('D','I','B','2')

#define	E_GrFontV3Dib2Version				1

#define	E_GrFontV3Dib2WidthMax			127
#define	E_GrFontV3Dib2HeightMax			127

#define	E_GrFontV3Dib2CmdLnSkip			0
#define	E_GrFontV3Dib2CmdPxSkip			1
#define	E_GrFontV3Dib2CmdPxShadow		2
#define	E_GrFontV3Dib2CmdPxText			3
#define	E_GrFontV3Dib2CmdPxIntp2		4
#define	E_GrFontV3Dib2CmdPxIntp4		5
#define	E_GrFontV3Dib2CmdPxIntp8		6

#define	E_GrFontV3Dib2AtbNext				8

#define	E_GrFontV3Dib2DbgMsg

//====================================================================
//global macro


//====================================================================
//global type

typedef struct St_GrFontV3Dib2ScptHead
{
	__u32	Version;
	__u8	FontWidth;
	__u8	FontHeight;
	__u16	FontCnt;
}	*Ptr_GrFontV3Dib2ScptHead;

typedef struct St_GrFontV3Dib2CharInfo
{
	__u16	Code;
	__u16	BitSize;
}	*Ptr_GrFontV3Dib2CharInfo;

//====================================================================
//global class


//====================================================================
//global function

void	GrFontV3Dib2Draw(Cls_GrGdib* A_Gdib, __u8 A_Pitch, void* A_PtrData, __u32 A_BitSize, 
	__s32 A_PosX, __s32 A_PosY, BOOL8 A_IsDrawTxt, BOOL8 A_IsDrawShadow, __u32* A_PtrClrTbl);

//====================================================================
//out Environment conditional
#endif

