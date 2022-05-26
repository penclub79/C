/*
Platform 5 UI object Button tab text id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoBtnTabTid.h>

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
#define	E_GrP5UoBtnTabTidParaInfoCnt							11

St_GrP5DvlScptParaInfo	V_GrP5UoBtnTabTidParaInfo[E_GrP5UoBtnTabTidParaInfoCnt]	=
{
	{L"Tab button text id", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"TxtId", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoBtnTabTidPara)0)->TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text ID"},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBtnTabTidPara)0)->FontId, TRUE, E_GrP5DvlParaInfoActDefault, L"Font ID"},
	{ L"ImgNormal", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnTabTidPara)0)->ImgNormal, TRUE, E_GrP5DvlParaInfoActDefault, L"Image normal status" },
	{ L"ImgDisable", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnTabTidPara)0)->ImgDisable, TRUE, E_GrP5DvlParaInfoActDefault, L"Image disable status" },
	{ L"ImgPush", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnTabTidPara)0)->ImgPush, TRUE, E_GrP5DvlParaInfoActDefault, L"Image push status" },
	{ L"ImgOnFocus", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnTabTidPara)0)->ImgOnFocus, TRUE, E_GrP5DvlParaInfoActDefault, L"Image on focus status" },
	{ L"ImgOnSelected", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnTabTidPara)0)->ImgOnSelected, TRUE, E_GrP5DvlParaInfoActDefault, L"Image selected status" },
	{ L"IsImgSprite", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBtnTabTidPara)0)->IsImgSprite, TRUE, E_GrP5DvlParaInfoActDefault, L"Image draw sprite" },
	{ L"ImgAlignHz", E_GrVarTypeAlignHz, 1, (void*)&((Ptr_GrP5UoBtnTabTidPara)0)->ImgAlignHz, TRUE, E_GrP5DvlParaInfoActDefault, L"Image align horizontal" },
	{ L"ImgAlignVt", E_GrVarTypeAlignVt, 1, (void*)&((Ptr_GrP5UoBtnTabTidPara)0)->ImgAlignVt, TRUE, E_GrP5DvlParaInfoActDefault, L"Image align vertical" },

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoBtnTabTid::Cls_GrP5UoBtnTabTid(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBtnTabBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdBtnTabTid, &m_ParaUoBtnTabTid, sizeof(m_ParaUoBtnTabTid), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoBtnTabTid, sizeof(m_ParaUoBtnTabTid));

		m_ParaUoBtnTabTid.TxtPara.Atb	=	E_GrP5FontAtbDrawText;
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoBtnTabTid.TxtPara);
#endif
		m_ParaUoBtnTabTid.TxtRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoBtnTabTid.TxtRc.Bottom.Ratio	=	E_GrP5RelRectRatioBaseMax;

		m_ParaUoBtnTabTid.ImgRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoBtnTabTid.ImgRc.Bottom.Ratio	=	E_GrP5RelRectRatioBaseMax;


}
//--------------------------------------------------------------------
Cls_GrP5UoBtnTabTid::~Cls_GrP5UoBtnTabTid(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnTabTid::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnTabBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoBtnTabTidParaInfo, E_GrP5UoBtnTabTidParaInfoCnt, &m_ParaUoBtnTabTid);
		DvlParaInfoTxtParaAdd(L"Tab button text para", &m_ParaUoBtnTabTid.TxtPara);
		DvlParaInfoRelPosAdd(L"Tab button text rect", &m_ParaUoBtnTabTid.TxtRc);
		DvlParaInfoRelPosAdd(L"Tab button image rect", &m_ParaUoBtnTabTid.ImgRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnTabTid::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBtnTabBase::DvlParaInfoAction(A_Action);

		// local action
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnTabTid::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnTabBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
