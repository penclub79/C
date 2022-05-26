/*
Platform 5 UI object Button text id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoBtnTid.h>

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
#define	E_GrP5UoBtnTidParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoBtnTidParaInfo[E_GrP5UoBtnTidParaInfoCnt]	=
{
	{L"Button text id", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"TxtId", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoBtnTidPara)0)->TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text ID"},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBtnTidPara)0)->FontId, TRUE, E_GrP5DvlParaInfoActDefault, L"Font ID"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoBtnTid::Cls_GrP5UoBtnTid(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBtnBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdBtnTid, &m_ParaUoBtnTid, sizeof(m_ParaUoBtnTid), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoBtnTid, sizeof(m_ParaUoBtnTid));

		m_ParaUoBtnTid.TxtPara.Atb					=	E_GrP5FontAtbDrawText;
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoBtnTid.TxtPara);
#endif
		m_ParaUoBtnTid.TxtRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoBtnTid.TxtRc.Bottom.Ratio		=	E_GrP5RelRectRatioBaseMax;

		// reset 
		m_StrTxt	=	GrP5TxtBufAlloc(m_ObjId);
		if(NULL != m_StrTxt)
		{
			GrStrClear(m_StrTxt);
		}
		
}
//--------------------------------------------------------------------
Cls_GrP5UoBtnTid::~Cls_GrP5UoBtnTid(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnTid::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoBtnTidParaInfo, E_GrP5UoBtnTidParaInfoCnt, &m_ParaUoBtnTid);
		DvlParaInfoTxtParaAdd(L"Button text para", &m_ParaUoBtnTid.TxtPara);
		DvlParaInfoRelPosAdd(L"Button text rect", &m_ParaUoBtnTid.TxtRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnTid::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnTid::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnTid::TextIdSet(__u16 A_TxtId)
{
	// local -------------------
	// code --------------------
		m_ParaUoBtnTid.TxtId	=	A_TxtId;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------

