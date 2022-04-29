/*
Platform 5 UI object Button tab text id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoBtnImgBase.h>

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
#define	E_GrP5UoBtnImgBaseParaInfoCnt							10

St_GrP5DvlScptParaInfo	V_GrP5UoBtnImgBaseParaInfo[E_GrP5UoBtnImgBaseParaInfoCnt]	=
{
	{L"Button Image base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{ L"IsSkinDraw", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBtnImgBasePara)0)->IsSkinDraw, TRUE, E_GrP5DvlParaInfoActDefault, L"Draw skin" },
	{ L"ImgNormal", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnImgBasePara)0)->ImgNormal, TRUE, E_GrP5DvlParaInfoActDefault, L"Image normal status" },
	{ L"ImgDisable", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnImgBasePara)0)->ImgDisable, TRUE, E_GrP5DvlParaInfoActDefault, L"Image disable status" },
	{ L"ImgPush", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnImgBasePara)0)->ImgPush, TRUE, E_GrP5DvlParaInfoActDefault, L"Image push status" },
	{ L"ImgOnFocus", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnImgBasePara)0)->ImgOnFocus, TRUE, E_GrP5DvlParaInfoActDefault, L"Image on focus status" },
	{ L"ImgOnSelected", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnImgBasePara)0)->ImgOnSelected, TRUE, E_GrP5DvlParaInfoActDefault, L"Image selected status" },
	{ L"IsImgSprite", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBtnImgBasePara)0)->IsImgSprite, TRUE, E_GrP5DvlParaInfoActDefault, L"Image draw sprite" },
	{ L"ImgAlignHz", E_GrVarTypeAlignHz, 1, (void*)&((Ptr_GrP5UoBtnImgBasePara)0)->ImgAlignHz, TRUE, E_GrP5DvlParaInfoActDefault, L"Image align horizontal" },
	{ L"ImgAlignVt", E_GrVarTypeAlignVt, 1, (void*)&((Ptr_GrP5UoBtnImgBasePara)0)->ImgAlignVt, TRUE, E_GrP5DvlParaInfoActDefault, L"Image align vertical" },
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoBtnImgBase::Cls_GrP5UoBtnImgBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBtnBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdBtnImgBase, &m_ParaUoBtnImgBase, sizeof(m_ParaUoBtnImgBase), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoBtnImgBase, sizeof(m_ParaUoBtnImgBase));

		m_ParaUoBtnImgBase.IsSkinDraw	=	TRUE;

		m_ParaUoBtnImgBase.ImgRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoBtnImgBase.ImgRc.Bottom.Ratio		=	E_GrP5RelRectRatioBaseMax;

}
//--------------------------------------------------------------------
Cls_GrP5UoBtnImgBase::~Cls_GrP5UoBtnImgBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnImgBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoBtnImgBaseParaInfo, E_GrP5UoBtnImgBaseParaInfoCnt, &m_ParaUoBtnImgBase);
		DvlParaInfoRelPosAdd(L"Tab button image rect", &m_ParaUoBtnImgBase.ImgRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnImgBase::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnImgBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::EvtScptDecDone();

}
//--------------------------------------------------------------------

