/*
	gause dib object
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrGdibObj
#define	_EPC_GrGdibObj

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<GrImgBase.h>
#include	<GrGdibBase.h>
#include	<GrDrawTool.h>

//====================================================================
//constance

//color boarder table index
#define E_GrGdibClrIdxBrdLight		0
#define E_GrGdibClrIdxBrdDark			1
#define E_GrGdibClrIdxBrdMidLight	2
#define E_GrGdibClrIdxBrdMidDark	3

#define E_GrGdibBrdClrTblCnt			4

//border attribute
#define E_GrGdibBrdAtbFlat				(1 << 0)		/* flat border */
#define E_GrGdibBrdAtbInner				(1 << 1)		/* inner border */

//blt mode
#define	E_GrGdibBltModeNormal			0
#define	E_GrGdibBltModeSrcKey			1
#define E_GrGdibBltModeDstKey			2
#define E_GrGdibBltModeBothKey		3

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrGdib
{
	protected:
		St_GrGdib		m_Gdib;				//gdib header

		__u32				m_DibSize;		//dib size
		void*				m_PtrDib;			//dib allocated pointer
		void*				m_PtrPlt;			//pallete allocated pointer

		Ptr_GrDrawPxStInfo	m_PtrPxSt;	//pixel struct
		St_GrRect		m_Rect;				//blt rect

		BOOL8				m_IsSprite;		// sprite mode
		BOOL8				m_IsRtlPack;		//dib pack
		__u8				m_RtlPltMask;		//pallete index mask

		//__u32				m_ClrKey;			// color key

		//runtime
		__u32*			m_PtrRtlPlt;		//pallete runtime
		__u32				m_RtlPltCnt;
		//funciton

		//pixel
		virtual	void		ProcSetPixelOrd( __s32 A_X, __s32 A_Y, __u32 A_PxVal );
		virtual	void		ProcSetPixelNpt( __s32 A_X, __s32 A_Y, __u32 A_PxVal );		//ni
		virtual	void		ProcSetPixelPlt( __s32 A_X, __s32 A_Y, __u32 A_PxVal );

		virtual	__u32		ProcGetPixelOrd( __s32 A_X, __s32 A_Y );
		virtual	__u32		ProcGetPixelNpt( __s32 A_X, __s32 A_Y );		//ni
		virtual	__u32		ProcGetPixelPlt( __s32 A_X, __s32 A_Y );

		//draw
		virtual	void		ProcFillRectOrd( Ptr_GrRect A_PtrRect, __u32 A_PxVal );
		virtual	void		ProcFillRectNpt( Ptr_GrRect A_PtrRect, __u32 A_PxVal );
		virtual	void		ProcFillRectPlt( Ptr_GrRect A_PtrRect, __u32 A_PxVal );

		//blt
		virtual	void		ProcBltOrd( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );

		virtual	void		ProcBltNpt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );

		virtual	void		ProcBltNrmNpt2Npt( __s32 A_X, __s32 A_Y,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );		//ni
		virtual	void		ProcBltSrkNpt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );	//ni		

		virtual	void		ProcBltPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );

		virtual	void		ProcBltNrmPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );	//ni
		virtual	void		ProcBltSrkPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );	//ni

		virtual	void		ProcBltPlt2Plt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );

		virtual	void		ProcBltNrmPlt2Plt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );
		virtual	void		ProcBltSrkPlt2Plt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr );

		//font 

		//stretch
		virtual	void		ProcSthOrd( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg );
		virtual	void		ProcSthNpt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg );	//ni
		virtual	void		ProcSthPlt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg );	//ni
		virtual	void		ProcSthPlt2Plt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg );

		//remove alpha
		virtual	void		ProcAlphaRemove( Ptr_GrRect A_PtrRect );
		virtual	void		ProcAlphaPltRemove( void );
		virtual	void		ProcAlphaSet( Ptr_GrRect A_PtrRect );
		virtual	void		ProcAlphaPltSet( void );

		//cvtto
		virtual	void		ProcCvtOrd( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect );
		virtual	void		ProcCvtNpt2Npt( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect );
		virtual	void		ProcCvtPlt2Npt( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect );
		virtual	void		ProcCvtNpt2Plt( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect );
		virtual	void		ProcCvtPlt2Plt( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect );

		//color swap
		void	ProcSwapColorNpt( __u32 A_Clr1, __u32 A_Clr2 );		//ni
		void	ProcSwapColorPlt( __u32 A_Clr1, __u32 A_Clr2 );	

	public:
		Cls_GrGdib( __u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
								__u32 A_Pitch = E_GrGdibPitchAuto, void* A_PtrDib = NULL, 
								__u32 A_PltBitCnt = 0, void* A_PtrPlt = NULL );
		virtual	~Cls_GrGdib();

		//info
		__u32		GetResX( void );
		__u32		GetResY( void );
		__u32		GetFcc( void );
		Ptr_GrGdib	GetGdibPtr( void );		//gdib header pointer
		void*		GetDibPtr( void );		//dib pointer
		void*		GetPltPtr( void );		//pallete pointer
		BOOL8		IsPalleteUse( void );	//pallete dib
		__u32		PalletCount( void );	//pallete count
		__u32		GetPixelBit( void );
		__u32		GetDibPitch( void );
		BOOL8		FindPltIdxByColor( __u32 A_Color, __u32* A_PtrRtIdx );
		__u32		GetColorByPltIdx( __u32 A_PltIdx, __u32* A_PtrRtColor );
		__u32		GetDibSize( void );

		//maintanence
		void		ClipperSet( Ptr_GrRect A_PtrRc );
		void		ClipperGet( Ptr_GrRect A_PtrRc );

		//pallete
		__u32		PalleteGet( __u32 A_Idx );
		void		PalleteSet( __u32 A_Idx, __u32 A_Color );
		void		PalleteDefault( void );		//release pallete to default

		//pixel
		virtual	BOOL8		SetPixelVal( __s32 A_X, __s32 A_Y, __u32 A_Val );	//pallete mode = color index
		virtual	BOOL8		GetPixelVal( __s32 A_X, __s32 A_Y, __u32* A_PtrRtVal );
		virtual	BOOL8		GetPixelColor( __s32 A_X, __s32 A_Y, __u32* A_PtrRtColor );

		//draw
		virtual	void		DrawLine( __s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_Val );
		virtual	void		DrawRect( Ptr_GrRect A_PtrRect, __u32 A_PxVal );
		virtual	void		DrawWaku( Ptr_GrRect A_PtrRect, __u32 A_PxValTopLeft, __u32 A_PxValBottomRight );
		virtual	void		DrawBoarder( __u8 A_Atb, __u32 A_Depth, __u32* A_PtrClrTbl, Ptr_GrRect A_PtrRect = NULL );
		virtual	void		FillRect( Ptr_GrRect A_PtrRect, __u32 A_PxVal );
			 
		//blt
		virtual	void		BltGdib( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr = NULL );
		virtual	void		NakedBlt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr = NULL );
		virtual	void		SpriteModeSet( BOOL8 A_IsSprite );

		//font
		virtual	void	FontV3Dib2Draw(__u8 A_FontPitch, void* A_PtrData, __u32 A_BitSize,
			__s32 A_PosX, __s32 A_PosY, BOOL8 A_IsDrawTxt, BOOL8 A_IsDrawShadow, __u32* A_PtrClrTbl);

		//stretch
		virtual	void		StretchGdib( Ptr_GrRect A_PtrRcTg, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr );
		virtual	void		NakedStretch( Ptr_GrRect A_PtrRcTg, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg  );

		//skin


		//alpha	
		virtual	BOOL8		AlphaRemove( Ptr_GrRect A_PtrRect = NULL );
		virtual	BOOL8		AlphaSet( Ptr_GrRect A_PtrRect = NULL );

		//convert
		virtual	BOOL8		ChgPalletForamt( __u32 A_Fcc );
		virtual	BOOL8		CvtTo( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect = NULL );

		//file
		virtual	BOOL8		SaveToFile( char* A_StrFn );
		virtual	BOOL8		SaveToBmp(char* A_StrFn);
		virtual	__u32		SaveToMem( void* A_PtrBuf );		//A_PtrBuf = NULL : get size only

		//color swap
		void		SwapColor( __u32 A_Clr1, __u32 A_Clr2 );

};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

