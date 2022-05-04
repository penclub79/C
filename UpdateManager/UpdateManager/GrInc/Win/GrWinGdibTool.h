/*
Window gdib tool

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrWinGdibTool
#define	_EPC_GrWinGdibTool

//====================================================================
// uses

#include <atlimage.h>

#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

#include <GrGdibTool.h>

//====================================================================
//constance

#define	E_GrWinGdibToolDfltClrFcc			E_GrFccRGB888

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class


//====================================================================
//global function

CImage*	GrWinGdibToImage(Cls_GrGdib* A_Gdib);
Cls_GrGdib*	GrWinImageToGdib(CImage* A_Image,__u32 A_ColorFcc);

//====================================================================
//out Enviroment conditional
#endif

