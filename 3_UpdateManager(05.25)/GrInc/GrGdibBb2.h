/*
	gause dib object 2 byte in bit control
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrGdibBb2
#define	_EPC_GrGdibBb2

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<GrImgBase.h>
#include	<GrGdibBase.h>
#include	<GrGdibObj.h>

//====================================================================
//constance

#define E_GrGdibBb2PxEleCnt		3			/* pixel element count */

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrGdibBb2	:	public Cls_GrGdib
{
	protected:
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

		//stretch
		virtual	void		ProcSthNpt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg );	//ni
		virtual	void		ProcSthPlt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg );	//ni

	public:
		Cls_GrGdibBb2(	__u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
										__u32 A_Pitch = E_GrGdibPitchAuto, void* A_PtrDib = NULL, 
										__u32 A_PltBitCnt = 0, void* A_PtrPlt = NULL );
		virtual	~Cls_GrGdibBb2();

		virtual	void	DrawLine(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_Val);

		virtual	void	FontV3Dib2Draw(__u8 A_FontPitch, void* A_PtrData, __u32 A_BitSize,
			__s32 A_PosX, __s32 A_PosY, BOOL8 A_IsDrawTxt, BOOL8 A_IsDrawShadow, __u32* A_PtrClrTbl);

};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

