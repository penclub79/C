/*
Font version 3 DIB1 type draw

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrFontV3Dib2Draw
#define	_EPC_GrFontV3Dib2Draw

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <FontV3/GrFontV3Base.h>
#include <FontV3/GrFontV3DrawBase.h>
#include <FontV3/Dib2/GrFontV3Dib2Base.h>

#include <GrGdibObj.h>

//====================================================================
//constance
#define	E_GrFontV3Dib2DrawClrTblCnt		5

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrFontV3Dib2DrawCharCtrlItem
{
	void*	PtrData;
}	*Ptr_GrFontV3Dib2DrawCharCtrlItem;

//====================================================================
//global class

class Cls_GrFontV3Dib2Draw	: public Cls_GrFontV3DrawBase
{
	protected:
		Ptr_GrFontV3ScptHead	m_PtrScpt;
		Ptr_GrFontV3Dib2ScptHead	m_PtrDib1Hd;
		Ptr_GrFontV3Dib2CharInfo	m_ChrInfoTbl;
		__u8*	m_PitchTbl;
		void*	m_PtrData;
		__u32	m_ScptSize;

		__u32	m_ClrTbl[E_GrFontV3Dib2DrawClrTblCnt];

		Ptr_GrFontV3Dib2DrawCharCtrlItem	m_ChrCtrlTbl;

		BOOL8	m_IsAutoScptFree;

		void	LcScriptFree(void);
		BOOL8	LcCharCtrlTblBuild(void);

		virtual	void	DrawCtrlInit(WCHAR* A_StrTxt, Cls_GrGdib* A_Gdib, Ptr_GrRect A_PtrRc, Ptr_GrFontV3DrawStyle A_PtrStyle, __u32 A_Stat);

		virtual	BOOL8	CharInfoGetByIndxe(__s32 A_ChrIdx, Ptr_GrFontV3DrawCharInfo A_PtrRtInfo);
		virtual	void	DrawFont(Cls_GrGdib* A_Gdib, __s32 A_ChrIdx, __s32 A_PosX, __s32 A_PosY);

	public:
		Cls_GrFontV3Dib2Draw(void);	
		virtual	~Cls_GrFontV3Dib2Draw(void);

		virtual	BOOL8	ScriptAssign(void* A_PtrScpt, __u32 A_Size, BOOL8 A_IsAutoScptFree);
		virtual	void*	ScriptGet(__u32* A_PtrRtSize);

		virtual	__s32	CodeToCharIdx(__u16 A_Code);
		virtual	__u16	CharIdxToCode(__s32 A_ChrIdx);
		virtual	__u8	HeightGet(void);
		virtual	__u8	WidthGet(void);
		virtual	__u16	CharCountGet(void);

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

