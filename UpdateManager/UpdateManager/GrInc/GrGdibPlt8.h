/*
	gause dib object byte unit control
	Pallete 8bit
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrGdibPlt8
#define	_EPC_GrGdibPlt8

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

class	Cls_GrGdibPlt8	:	public Cls_GrGdib
{
	protected:
		__u32		m_CvtPlt[256];			//convert pallete
		//inherited
		virtual	void		ProcCvtPlt2Npt( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect );

		virtual	void		ProcBltNrmPlt2Plt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );
		virtual	void		ProcBltSrkPlt2Plt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );

		//alpha
		virtual	void		ProcAlphaPltRemove( void );
		virtual	void		ProcAlphaPltSet( void );

	public:
		Cls_GrGdibPlt8(	__u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
										__u32 A_Pitch = E_GrGdibPitchAuto, void* A_PtrDib = NULL, 
										void* A_PtrPlt = NULL );
		virtual	~Cls_GrGdibPlt8();

};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

