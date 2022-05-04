/*
Platform 5 UI object Button text id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoChkImgTiBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5ConBase.h>

#ifdef GR_P5_EDITOR

#include <P5\Dvl\GrP5DvlBase.h>

#endif

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;

#ifdef GR_P5_EDITOR

// relate position
#define	E_GrP5UoChkImgTiBaseParaInfoCnt							8

St_GrP5DvlScptParaInfo	V_GrP5UoChkImgTiBaseParaInfo[E_GrP5UoChkImgTiBaseParaInfoCnt]	=
{
	{L"Check Image text id base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"ImgAlignHz", E_GrVarTypeAlignHz, 1, (void*)&((Ptr_GrP5UoChkImgTiBasePara)0)->ImgAlignHz, TRUE, E_GrP5DvlParaInfoActDefault, L"Image align horizontal"},
	{L"ImgAlignVt", E_GrVarTypeAlignVt, 1, (void*)&((Ptr_GrP5UoChkImgTiBasePara)0)->ImgAlignVt, TRUE, E_GrP5DvlParaInfoActDefault, L"Image align vertical"},
	{L"IsImgSprite", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoChkImgTiBasePara)0)->IsImgSprite, TRUE, E_GrP5DvlParaInfoActDefault, L"Image sprite draw"},
	{L"ImgIdNormal", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoChkImgTiBasePara)0)->ImgIdNormal, TRUE, E_GrP5DvlParaInfoActDefault, L"Image normal status"},
	{L"ImgIdPush", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoChkImgTiBasePara)0)->ImgIdPush, TRUE, E_GrP5DvlParaInfoActDefault, L"Image push status"},
	{L"ImgIdChecked", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoChkImgTiBasePara)0)->ImgIdChecked, TRUE, E_GrP5DvlParaInfoActDefault, L"Image checked status"},
	{L"ImgIdDisable", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoChkImgTiBasePara)0)->ImgIdDisable, TRUE, E_GrP5DvlParaInfoActDefault, L"Image disabled status"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoChkImgTiBase::Cls_GrP5UoChkImgTiBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBtnTid(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdChkImgTiBase, &m_ParaUoChkImgTiBase, sizeof(m_ParaUoChkImgTiBase), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoChkImgTiBase, sizeof(m_ParaUoChkImgTiBase));

		m_ParaUoChkImgTiBase.ImgRc.Right.Ratio		=	128;
		m_ParaUoChkImgTiBase.ImgRc.Bottom.Ratio		=	128;


		m_SkinCtlBtnBase.IsChked		=	FALSE;
}
//--------------------------------------------------------------------
Cls_GrP5UoChkImgTiBase::~Cls_GrP5UoChkImgTiBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoChkImgTiBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnTid::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoChkImgTiBaseParaInfo, E_GrP5UoChkImgTiBaseParaInfoCnt, &m_ParaUoChkImgTiBase);
		DvlParaInfoRelPosAdd(L"Check image rect", &m_ParaUoChkImgTiBase.ImgRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoChkImgTiBase::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBtnTid::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoChkImgTiBase::EvtPaintProc(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnTid::EvtPaintProc();

		// update checked
		LcValueRead();

}
//--------------------------------------------------------------------
void	Cls_GrP5UoChkImgTiBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnTid::EvtScptDecDone();

}
//--------------------------------------------------------------------
void	Cls_GrP5UoChkImgTiBase::DoEvtBtnRelease(void)
{
	// local -------------------
	// code --------------------
		// 
		m_SkinCtlBtnBase.IsChked	=	!m_SkinCtlBtnBase.IsChked;
		LcValueWrite();
		// inherited
		Cls_GrP5UoBtnTid::DoEvtBtnRelease();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoChkImgTiBase::ValueSet(BOOL8 A_IsSet)
{
		m_SkinCtlBtnBase.IsChked	=	A_IsSet;
		LcValueWrite();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoChkImgTiBase::ValueGet(void)
{
		LcValueRead();
		return	m_SkinCtlBtnBase.IsChked;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoChkImgTiBase::LcValueWrite(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoChkImgTiBase::LcValueRead(void)
{

}
//--------------------------------------------------------------------

