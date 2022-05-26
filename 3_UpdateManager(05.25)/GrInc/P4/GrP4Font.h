/*
	platform font

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4Font
#define	_EPC_GrP4Font

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<GrGdibTool.h>

//====================================================================
//constance

#define	E_GrP4FontVer2Id					Mac_GrMakeFcc('F','N','T','2')	/*	ver 2	*/

#define	E_GrP4FontCharTblBase			0x21
#define	E_GrP4FontCharTblEnd			0xD7FF
#define	E_GrP4FontCharTblCnt			0xD7E0

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrP4FontMapItm 
{
	__u16		Code;
	__u8		Pitch;
	__u8		StGap;
}	*Ptr_GrP4FontMapItm;

// font data header
typedef	struct St_GrP4FontHead
{
	__u32		Fcc;
	__u32		DibPitch;
	__u32		DibWidth;
	__u16		Width;
	__u16		Height;
	__u32		CharCnt;
}	*Ptr_GrP4FontHead;


//====================================================================
//class

class	Cls_GrP4Font
{
	protected:
		BOOL8		m_IsCodeTbl;		// use code table
		__u32		m_Width;
		__u32		m_CharCnt;

		Ptr_GrP4FontMapItm	m_PtrMapTbl;						//pointer of mapping table 
		void*		m_PtrDib;								//dib ptr
		St_GrGdib	m_Gdib;
		//function
		__u16		m_CdIdxTbl[E_GrP4FontCharTblCnt];
		__u32		m_CdPosTbl[E_GrP4FontCharTblCnt];

	public:
		Cls_GrP4Font( Ptr_GrP4FontHead A_PtrFont );
		virtual	~Cls_GrP4Font();

		//info
		__u32	GetWidth(void);
		__u32	GetHeight(void);
		__u32	GetCharCnt(void);
		Ptr_GrP4FontMapItm	GetMapTbl( void );

		//get gdib
		Ptr_GrGdib	GetGdibPtr( void );
		
		// control
		void	CodeTblBuild(void);

		//get charactor info
		BOOL8	IsCodeExist(__u16 A_Code);
		BOOL8	GetCharInfo(__u16 A_Code, __u32* A_PtrRtIdx, Ptr_GrRect A_PtrRtRcDib, __s32* A_PtrRtPitch, __s32* A_PtrRtGap);
		BOOL8	GetItemInfo(__u32 A_Idx, __u16* A_PtrRtCode, Ptr_GrRect A_PtrRtRcDib, __s32* A_PtrRtPitch, __s32* A_PtrRtGap);

};


//====================================================================
//global function
__u32	GrP4FontCalcSize( Ptr_GrP4FontHead A_PtrFont );

//====================================================================
//out Enviroment conditional
#endif

