/*
	gause dib object byte unit control
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrGdibByt
#define	_EPC_GrGdibByt

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<GrImgBase.h>
#include	<GrGdibBase.h>
#include	<GrGdibObj.h>

//====================================================================
//constance

#define E_GrGdibBytPxEleCnt		3			/* pixel element count */

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrGdibByt	:	public Cls_GrGdib
{
	protected:

		//pixel byte pos
		__u8		m_PxBpos[E_GrGdibBytPxEleCnt];
		__u32		m_PxByteCnt;			//pixel byte count

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
		virtual	void		ProcFntNrmNpt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );		//ni
		virtual	void		ProcFntSdwNpt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );		//ni

		//stretch
		virtual	void		ProcSthNpt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg );	//ni
		virtual	void		ProcSthPlt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg );	//ni

	public:
		Cls_GrGdibByt(	__u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
										__u32 A_Pitch = E_GrGdibPitchAuto, void* A_PtrDib = NULL, 
										__u32 A_PltBitCnt = 0, void* A_PtrPlt = NULL );
		virtual	~Cls_GrGdibByt();

};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

