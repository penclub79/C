/*
	Font version 3 DIB1 type base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrFontV3Dib1Base
#define	_EPC_GrFontV3Dib1Base

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrFontV3Dib1ClassId				Mac_GrMakeFcc('D','I','B','1')

#define	E_GrFontV3Dib1Version				1

#define	E_GrFontV3Dib1WidthMax			255
#define	E_GrFontV3Dib1HeightMax			255

#define	E_GrFontV3Dib1CmdBitPos			6
#define	E_GrFontV3Dib1CmdBitMask		0x03

#define	E_GrFontV3Dib1CmdPxCntMask	0x3F

#define	E_GrFontV3Dib1CmdSkipPixel	0
#define	E_GrFontV3Dib1CmdText				1
#define	E_GrFontV3Dib1CmdShadow			2
#define	E_GrFontV3Dib1CmdSkipLine		3

#define	E_GrFontV3Dib1DbgMsg

//====================================================================
//global macro


//====================================================================
//global type

typedef struct St_GrFontV3Dib1ScptHead
{
	__u32	Version;
	__u8	FontWidth;
	__u8	FontHeight;
	__u16	FontCnt;
}	*Ptr_GrFontV3Dib1ScptHead;

typedef struct St_GrFontV3Dib1CharInfo
{
	__u16	Code;
	__u16	DataSize;
}	*Ptr_GrFontV3Dib1CharInfo;

//====================================================================
//global class


//====================================================================
//global function

void	GrFontV3Dib1Draw(Cls_GrGdib* A_Gdib, __u8 A_Pitch, void* A_PtrData, __u32 A_DataSize, 
	__s32 A_PosX, __s32 A_PosY, BOOL8 A_IsDrawTxt, BOOL8 A_IsDrawShadow, __u32 A_ClrTxt, __u32 A_ClrShadow);

//====================================================================
//out Environment conditional
#endif

