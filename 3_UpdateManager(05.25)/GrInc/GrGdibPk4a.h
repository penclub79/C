/*
	gause dib object - 4byte pack type
		yuy2 , uyvy , uyvf
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrGdibPk4a
#define	_EPC_GrGdibPk4a

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

class	Cls_GrGdibPk4a	:	public Cls_GrGdibPk4
{
	protected:

		//alpha remove 
		__u32		m_RmAlphaMix;
		__u32		m_PxAlpChk[E_GrGdibPk4PxSubCnt];

		//pixel draw
		virtual	void		ProcAlphaPltRemove( void );

		//remove alpha
		virtual	void		ProcAlphaRemove( Ptr_GrRect A_PtrRect );

	public:
		Cls_GrGdibPk4a(	__u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
										__u32 A_Pitch = E_GrGdibPitchAuto, void* A_PtrDib = NULL, 
										__u32 A_PltBitCnt = 0, void* A_PtrPlt = NULL );
		virtual	~Cls_GrGdibPk4a();

		//alpha	

};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

