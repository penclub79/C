/*
Platform 5 UI object Button time

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoBtnTime.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
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
#define	E_GrP5UoBtnTimeParaInfoCnt							2

St_GrP5DvlScptParaInfo	V_GrP5UoBtnTimeParaInfo[E_GrP5UoBtnTimeParaInfoCnt]	=
{
	{L"Button Time", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBtnTimePara)0)->FontId, TRUE, E_GrP5DvlParaInfoActDefault, L"Font ID"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoBtnTime::Cls_GrP5UoBtnTime(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBtnBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdBtnTime, &m_ParaUoBtnTime, sizeof(m_ParaUoBtnTime), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoBtnTime, sizeof(m_ParaUoBtnTime));

		m_ParaUoBtnTime.TxtPara.Atb					=	E_GrP5FontAtbDrawText;
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoBtnTime.TxtPara);
#endif
		m_ParaUoBtnTime.TxtRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoBtnTime.TxtRc.Bottom.Ratio		=	E_GrP5RelRectRatioBaseMax;

		// reset 
		m_SkinCtlBtnBase.Str	=	GrP5TxtBufAlloc(m_ObjId);
		if(NULL != m_SkinCtlBtnBase.Str)
		{
			GrStrClear(m_SkinCtlBtnBase.Str);
		}
		m_TimeLast		=	0;

		GrP5TimerRegist(this);
}
//--------------------------------------------------------------------
Cls_GrP5UoBtnTime::~Cls_GrP5UoBtnTime(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnTime::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoBtnTimeParaInfo, E_GrP5UoBtnTimeParaInfoCnt, &m_ParaUoBtnTime);
		DvlParaInfoTxtParaAdd(L"Button text para", &m_ParaUoBtnTime.TxtPara);
		DvlParaInfoRelPosAdd(L"Button text rect", &m_ParaUoBtnTime.TxtRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnTime::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoAction(A_Action);

		// local action
		LcBtnTextBld();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnTime::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::EvtScptDecDone();

		// update text
		//LcTabBtnTextBld();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnTime::LcBtnTextBld(void)
{
	// local -------------------
	// code --------------------
		if( NULL != m_SkinCtlBtnBase.Str)
		{
			GrStrTimeToWstr(m_SkinCtlBtnBase.Str, m_TimeLast);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnTime::EvtTimerTick(void)
{
	// local -------------------
		Def_GrTime	Tv_Time;
		BOOL8	Tv_IsSm;
		__u32	Tv_Mili;
	// code --------------------
		Tv_Time	=	GrTimeGet(&Tv_Mili, &Tv_IsSm);
		if(m_TimeLast != Tv_Time)
		{
			// update time
			m_TimeLast	=	Tv_Time;
			LcBtnTextBld();
			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------
