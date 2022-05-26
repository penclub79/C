/*
	platform font

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3Font
#define	_EPC_GrP3Font

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<GrGdibTool.h>

//====================================================================
//constance

#define	E_GrP3FontVer2Id					Mac_GrMakeFcc('F','N','T','2')	/*	ver 2	*/

#define E_GrP3FontTextPltIdx			1
#define E_GrP3FontShadowPltIdx		2

#define E_GrP3FontDibBitCnt				2

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrP3FontMapItm 
{
	__u16		Code;
	__u8		Pitch;
	__u8		StGap;
}	*Ptr_GrP3FontMapItm;

// font data header
typedef	struct St_GrP3FontHead
{
	__u32		Fcc;
	__u32		DibPitch;
	__u32		DibWidth;
	__u16		Width;
	__u16		Height;
	__u32		CharCnt;
}	*Ptr_GrP3FontHead;

//====================================================================
//class

class	Cls_GrP3Font
{
	protected:
		__u32		m_Width;
		__u32		m_CharCnt;

		Ptr_GrP3FontMapItm	m_PtrMapTbl;						//pointer of mapping table 
		void*		m_PtrDib;								//dib ptr
		St_GrGdib	m_Gdib;
		//function

	public:
		Cls_GrP3Font( Ptr_GrP3FontHead A_PtrFont );
		virtual	~Cls_GrP3Font();

		//info
		__u32	GetWidth( void );
		__u32	GetHeight( void );
		__u32	GetCharCnt( void );
		Ptr_GrP3FontMapItm	GetMapTbl( void );

		//get gdib
		Ptr_GrGdib	GetGdibPtr( void );
		
		//get charactor info
		BOOL8	IsCodeExist( __u16 A_Code );
		BOOL8	GetCharInfo( __u16 A_Code, __u32* A_PtrRtIdx, Ptr_GrRect A_PtrRtRcDib, __s32* A_PtrRtPitch, __s32* A_PtrRtGap );
		BOOL8	GetItemInfo( __u32 A_Idx, __u16* A_PtrRtCode, Ptr_GrRect A_PtrRtRcDib, __s32* A_PtrRtPitch, __s32* A_PtrRtGap );

};


//====================================================================
//global function
__u32	GrP3FontCalcSize( Ptr_GrP3FontHead A_PtrFont );

//====================================================================
//out Enviroment conditional
#endif

