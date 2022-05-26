/*
Font version 3 draw base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrFontV3DrawBase
#define	_EPC_GrFontV3DrawBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <FontV3/GrFontV3Base.h>
#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrFontV3DrawCusSizeShift			1

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrFontV3DrawCtrl
{
	Cls_GrGdib*	Gdib;
	St_GrFontV3DrawStyle	Style;
	St_GrRect	RcTarget;
	WCHAR*	StrTxt;

	__u32	ClrTxt;

	__s32	MaxSizeX;
	__s32	SizeX;
	__s32	SizeY;
	__s32	DrawX;		// draw position x
	__s32	DrawY;		// draw position y

	__u32	TxtLen;

	__s32	CusPos;
	__u32	ClrCus;

	BOOL8	IsGapChr;
	BOOL8	IsGapLine;

	BOOL8	IsDrawTxt;
	BOOL8	IsDrawShadow;
	__s32	Height;
	__s32	Width;

}	*Ptr_GrFontV3DrawCtrl;


typedef struct St_GrFontV3DrawCharInfo
{
	__u8	Pitch;
	__u8	_rsvd0;
	__u16	DataSize;
	void*	PtrData;
}	*Ptr_GrFontV3DrawCharInfo;

//====================================================================
//global class

class Cls_GrFontV3DrawBase
{
	protected:
		St_GrFontV3DrawCtrl	m_Ctrl;		// draw control

		virtual	BOOL8	CharInfoGetByIndxe(__s32 A_ChrIdx, Ptr_GrFontV3DrawCharInfo A_PtrRtInfo);
		virtual	void	DrawFont(Cls_GrGdib* A_Gdib, __s32 A_ChrIdx, __s32 A_PosX, __s32 A_PosY);

		BOOL8	DrawCtrlSubLinefeed(void);

		virtual	void	DrawCtrlInit(WCHAR* A_StrTxt, Cls_GrGdib* A_Gdib, Ptr_GrRect A_PtrRc, Ptr_GrFontV3DrawStyle A_PtrStyle, __u32 A_Stat);
		void	DrawCtrlRun(BOOL8 A_IsDraw);

	public:
		Cls_GrFontV3DrawBase(void);	
		virtual	~Cls_GrFontV3DrawBase(void);

		// info
		virtual	__s32	CodeToCharIdx(__u16 A_Code);
		virtual	__u16	CharIdxToCode(__s32 A_ChrIdx);
		virtual	__u8	HeightGet(void);
		virtual	__u8	WidthGet(void);
		virtual	__u16	CharCountGet(void);

		// control
		virtual	BOOL8	ScriptAssign(void* A_PtrScpt, __u32 A_Size, BOOL8 A_IsAutoScptFree);
		virtual	void*	ScriptGet(__u32* A_PtrRtSize);
		void	DrawString(WCHAR* A_StrTxt, Cls_GrGdib* A_Gdib, Ptr_GrRect A_PtrRc, Ptr_GrFontV3DrawStyle A_PtrStyle, __u32 A_Stat,
			__s32 A_CusPos = -1,__u32 A_ClrCus = 0);
		void	CalcStringDimension(WCHAR* A_StrTxt, Ptr_GrRect A_PtrRcCon, Ptr_GrFontV3DrawStyle A_PtrStyle, __u32 A_Stat, __u32* A_PtrRtWidth, __u32* A_PtrRtHeight);
};



//====================================================================
//global function

Cls_GrFontV3DrawBase*	GrFontV3DrawCreateByScript(void* A_PtrScpt, __u32 A_Size, BOOL8 A_IsAutoScptFree);

//====================================================================
//out Enviroment conditional
#endif

