/*
Platform 5 UI object Button tab text id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoBtnImgTid.h>

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
#define	E_GrP5UoBtnImgTidParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoBtnImgTidParaInfo[E_GrP5UoBtnImgTidParaInfoCnt]	=
{
	{L"Button image text id", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"TxtId", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoBtnImgTidPara)0)->TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text ID"},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBtnImgTidPara)0)->FontId, TRUE, E_GrP5DvlParaInfoActDefault, L"Font ID"},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoBtnImgTid::Cls_GrP5UoBtnImgTid(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBtnImgBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdBtnImgTid, &m_ParaUoBtnImgTid, sizeof(m_ParaUoBtnImgTid),NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoBtnImgTid, sizeof(m_ParaUoBtnImgTid));

		m_ParaUoBtnImgTid.TxtPara.Atb	=	E_GrP5FontAtbDrawText;
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoBtnImgTid.TxtPara);
#endif
		m_ParaUoBtnImgTid.TxtRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoBtnImgTid.TxtRc.Bottom.Ratio	=	E_GrP5RelRectRatioBaseMax;

}
//--------------------------------------------------------------------
Cls_GrP5UoBtnImgTid::~Cls_GrP5UoBtnImgTid(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnImgTid::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnImgBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoBtnImgTidParaInfo, E_GrP5UoBtnImgTidParaInfoCnt, &m_ParaUoBtnImgTid);
		DvlParaInfoTxtParaAdd(L"Tab button text para", &m_ParaUoBtnImgTid.TxtPara);
		DvlParaInfoRelPosAdd(L"Tab button text rect", &m_ParaUoBtnImgTid.TxtRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnImgTid::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBtnImgBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnImgTid::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnImgBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
