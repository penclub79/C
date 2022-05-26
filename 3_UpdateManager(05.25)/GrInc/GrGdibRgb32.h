/*
	gause dib object rgb32
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrGdibRgb32
#define	_EPC_GrGdibRgb32

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<GrImgBase.h>
#include	<GrGdibBase.h>
#include	<GrGdibObj.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrGdibRgb32	:	public Cls_GrGdib
{
	protected:

		//pixel byte pos

		//pixel
		virtual	void		ProcSetPixelNpt( __s32 A_X, __s32 A_Y, __u32 A_PxVal );
		virtual	__u32		ProcGetPixelNpt( __s32 A_X, __s32 A_Y );

		//draw
		virtual	void		ProcFillRectNpt( Ptr_GrRect A_PtrRect, __u32 A_PxVal );

		//blt
		virtual	void		ProcBltNrmNpt2Npt( __s32 A_X, __s32 A_Y,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );
		virtual	void		ProcBltSrkNpt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );

		virtual	void		ProcBltNrmPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );
		virtual	void		ProcBltSrkPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );

		//font
		virtual	void		ProcFntNrm(__s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, __u32 A_ClrTxt, Ptr_GrRect A_PtrRectSr);
		virtual	void		ProcFntSdw(__s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, __u32 A_ClrTxt, __u32 A_ClrSdw, Ptr_GrRect A_PtrRectSr);

		//stretch
		virtual	void		ProcSthNpt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg );	//ni
		virtual	void		ProcSthPlt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg );	//ni

	public:
		Cls_GrGdibRgb32(	__u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
										__u32 A_Pitch = E_GrGdibPitchAuto, void* A_PtrDib = NULL, 
										__u32 A_PltBitCnt = 0, void* A_PtrPlt = NULL );
		virtual	~Cls_GrGdibRgb32();

};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

