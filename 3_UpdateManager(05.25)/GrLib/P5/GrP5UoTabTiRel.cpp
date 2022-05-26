/*
Platform 5 UI object Label Text Id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoTabTiRel.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5Mng.h>

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

WCHAR	V_Cls_GrP5UoTabTiRelStrLaebl[]	=	L"LAEBL";

WCHAR	V_Cls_GrP5UoTabTiRelStrParaSepTabTiRel[]			=	L"Tab text icon relate";
WCHAR	V_Cls_GrP5UoTabTiRelStrParaSepTextPara[]			=	L"Label text parameter";

#define	E_Cls_GrP5UoTabTiRelParaInfoCnt							4

St_GrP5DvlScptParaInfo	V_Cls_GrP5UoTabTiRelParaInfo[E_Cls_GrP5UoTabTiRelParaInfoCnt]	=
{
	{L"LblFontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTabTiRelPara)0)->LblFontId, TRUE, 0, L"Label font ID"},
	{L"IconAlignHz", E_GrVarTypeAlignHz, 1, (void*)&((Ptr_GrP5UoTabTiRelPara)0)->IconAlignHz, TRUE, 0, L"Icon align Horizontal"},
	{L"IconAlignVt", E_GrVarTypeAlignVt, 1, (void*)&((Ptr_GrP5UoTabTiRelPara)0)->IconAlignVt, TRUE, 0, L"Icon align Vertical"},
	{L"IsIconSprite", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTabTiRelPara)0)->IsIconSprite, TRUE, 0, L"Icon draw sprite mode"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoTabTiRel::Cls_GrP5UoTabTiRel(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoTabBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdTabTiRel, &m_ParaUoTabTiRel, sizeof(m_ParaUoTabTiRel), &m_SkinCtlTabTiRel);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoTabTiRel, sizeof(m_ParaUoTabTiRel));
		GrDumyZeroMem(&m_SkinCtlTabTiRel, sizeof(m_SkinCtlTabTiRel));

		m_ParaUoTabTiRel.LblTxtPara.Atb				=	E_GrP5FontAtbDrawText;
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoTabTiRel.LblTxtPara);
#endif

		m_ParaUoTabTiRel.LblBgRc.Right.Ratio	=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoTabTiRel.LblBgRc.Bottom.Val		=	40;

		m_ParaUoTabTiRel.LblTxtRc.Left.Val		=	60;
		m_ParaUoTabTiRel.LblTxtRc.Right.Ratio	=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoTabTiRel.LblTxtRc.Bottom.Val	=	40;

		m_ParaUoTabTiRel.IconRc.Right.Val			=	60;
		m_ParaUoTabTiRel.IconRc.Bottom.Val		=	40;

}
//--------------------------------------------------------------------
Cls_GrP5UoTabTiRel::~Cls_GrP5UoTabTiRel(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5UoTabTiRel::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoTabBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoTabTiRel::DvlParaInfoBuild(void)
{
	// local -------------------
		St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoTabBase::DvlParaInfoBuild();

		// separator basic info
		Tv_ParaInfo.StrName		=	V_Cls_GrP5UoTabTiRelStrParaSepTabTiRel;
		Tv_ParaInfo.StrDesc		=	NULL;
		Tv_ParaInfo.ValType		=	E_GrP5DvlValTypeSeparator;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.Act				=	0;
		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);

		DvlParaInfoAdd(V_Cls_GrP5UoTabTiRelParaInfo, E_Cls_GrP5UoTabTiRelParaInfoCnt, &m_ParaUoTabTiRel);

		DvlParaInfoTxtParaAdd(V_Cls_GrP5UoTabTiRelStrParaSepTextPara, &m_ParaUoTabTiRel.LblTxtPara);
		DvlParaInfoRelPosAdd(L"Label text position", &m_ParaUoTabTiRel.LblTxtRc);
		DvlParaInfoRelPosAdd(L"Label background position", &m_ParaUoTabTiRel.LblBgRc);
		DvlParaInfoRelPosAdd(L"Icon position", &m_ParaUoTabTiRel.IconRc);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoTabTiRel::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoTabBase::DvlParaInfoAction(A_Action);

		// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoTabTiRel::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
	// delete prepare


	// inherited
	Cls_GrP5UoTabBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
