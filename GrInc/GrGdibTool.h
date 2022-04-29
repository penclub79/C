/*
	gause image object tool

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrGdibTool
#define	_EPC_GrGdibTool

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<GrGdibObj.h>
#include	<GrImgBase.h>

//====================================================================
//constance

#define E_GrGdibMaxResX		2500
#define E_GrGdibMaxResY		2500

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

//====================================================================
//global function

Cls_GrGdib*	GrGdibCreate(	__u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
													__u32 A_Pitch = E_GrGdibPitchAuto, void* A_PtrDib = NULL, 
													__u32 A_PltBitCnt = 0, void* A_PtrPlt = NULL );
Cls_GrGdib*	GrGdibOvlp( Ptr_GrGdib A_PtrGdib );
Cls_GrGdib*	GrGdibClone( Cls_GrGdib* A_ObjSrc );
Cls_GrGdib*	GrGdibLoadFile( char* A_StrFn, __u32 A_LimResX = E_GrGdibMaxResX, __u32 A_LimResY = E_GrGdibMaxResY );
Cls_GrGdib*	GrGdibLoadBmp(char* A_StrFn, __u32 A_LimResX = E_GrGdibMaxResX, __u32 A_LimResY = E_GrGdibMaxResY);
Cls_GrGdib*	GrGdibLoadBmp( WCHAR* A_StrFn, __u32 A_LimResX = E_GrGdibMaxResX, __u32 A_LimResY = E_GrGdibMaxResY );
Cls_GrGdib*	GrGdibCvt(	Cls_GrGdib* A_ObjSrc, __u32 A_Fcc, 
												__u32 A_Pitch = E_GrGdibPitchAuto, void* A_PtrDib = NULL, 
												__u32 A_PltBitCnt = 0, void* A_PtrPlt = NULL );

//====================================================================
//out Environment conditional
#endif

