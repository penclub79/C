/*
	gause image

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrGdibBase
#define	_EPC_GrGdibBase

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<GrImgBase.h>

//====================================================================
//constance

#define	E_GrGdibFcc								Mac_GrMakeFcc('G','D','I','B' )

#define E_GrGdibPitchAuto					0			/* auto pitch value */

#define E_GrGdibPltBase						1			/* use base pallet */

#define E_GrGdibBasePltCnt				256

#define	E_GrGdibFontDibBitCnt			2
#define	E_GrGdibFontDibBitMask		3

#define E_GrGdibFontTextPltIdx		1
#define E_GrGdibFontShadowPltIdx	2


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrGdib
{
	__u32			FccGdib;
	__u32			Size;						//total size include header
	__u32			FccImg;					//image FCC
	__u32			ResX;
	__u32			ResY;
	__u32			Pitch;
	__u32			BitCnt;					//pixel bit count
	BOOL8			IsPlt;					//pallet mode
	void*			PtrExtPlt;			//external palleted pointer - null = internal pallet , 1 = base pallet , other use external pallet
	void*			PtrExtDib;			//external DIB pointer - Not null = use external DIB, null = internal DIB
}	*Ptr_GrGdib;

//====================================================================
//class

//====================================================================
//global function
__u32		GrGdibCalcSize( __u32 A_Pitch, __u32 A_ResY, __u32 A_PltBitCnt = 0 );
void		GrGdibInitHead( Ptr_GrGdib A_PtrGdib,
												__u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
												__u32 A_Pitch = E_GrGdibPitchAuto, void* A_PtrDib = NULL, 
												__u32 A_PltBitCnt = 0, void* A_PtrPlt = NULL );
void*		GrGdibGetDibPtr( Ptr_GrGdib A_PtrGdib );
void*		GrGdibGetPalletPtr( Ptr_GrGdib A_PtrGdib );

void	GrGdibAlphaChgArgb8888(Ptr_GrGdib A_PtrGdib, __u8 A_AlphaVal);

//base pallet management
void*	GrGdibGetBasePalletePtr( void );
__u32	GrGdibBasePltColorGet( __u32 A_Idx );
void	GrGdibBasePltColorSet( __u32 A_Idx, __u32 A_Color );


//====================================================================
//out Environment conditional
#endif

