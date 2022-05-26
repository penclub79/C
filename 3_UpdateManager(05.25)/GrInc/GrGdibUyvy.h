/*
	gause dib object - Uyvy
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrGdibUyvy
#define	_EPC_GrGdibUyvy

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<GrImgBase.h>
#include	<GrGdibBase.h>
#include	<GrGdibPk4.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrGdibUyvy	:	public Cls_GrGdibPk4
{
	protected:
		
		//pixel
		virtual	void		ProcSetPixelNpt( __s32 A_X, __s32 A_Y, __u32 A_PxVal );

		//draw
		virtual	void		ProcFillRectNpt( Ptr_GrRect A_PtrRect, __u32 A_PxVal );

		//blt
		virtual	void		ProcBltNrmPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );
		virtual	void		ProcBltSrkPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );

		//font
		virtual	void		ProcFntNrm( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, __u32 A_ClrTxt, Ptr_GrRect A_PtrRectSr );	
		virtual	void		ProcFntSdw( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, __u32 A_ClrTxt, __u32 A_ClrSdw, Ptr_GrRect A_PtrRectSr );		

		//alpha
		virtual	void		ProcAlphaPltRemove( void );
		virtual	void		ProcAlphaPltSet( void );

	public:
		Cls_GrGdibUyvy(	__u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
										__u32 A_Pitch = E_GrGdibPitchAuto, void* A_PtrDib = NULL, 
										__u32 A_PltBitCnt = 0, void* A_PtrPlt = NULL );
		virtual	~Cls_GrGdibUyvy();

};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

