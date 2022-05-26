/*
Platform 5 UI object Label Text Id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoLblTimeReg.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>

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


WCHAR	V_Cls_GrP5UoLblTimeRegStrParaSepLabelBase[]			=	L"Label Time by register";

#define	E_Cls_GrP5UoLblTimeRegParaInfoCnt							4

St_GrP5DvlScptParaInfo	V_Cls_GrP5UoLblTimeRegParaInfo[E_Cls_GrP5UoLblTimeRegParaInfoCnt]	=
{
	{L"RegTime", E_GrP5DvlValTypeRintId, 1, (void*)&((Ptr_GrP5UoLblTimeRegPara)0)->RegTime, TRUE, 0, L"Register integer time"},
	{L"IsObserve", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoLblTimeRegPara)0)->IsObserve, TRUE, 0, L"Keep observe"},
	{L"IsSpriteImg", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoLblTimeRegPara)0)->IsSpriteImg, TRUE, 0, L"Sprite DST image"},
	{L"ImgDstId", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoLblTimeRegPara)0)->ImgDstId, TRUE, 0, L"DST image id"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoLblTimeReg::Cls_GrP5UoLblTimeReg(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoLblTimeBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdLblTimeReg, &m_ParaUoLblTimeReg, sizeof(m_ParaUoLblTimeReg),NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoLblTimeReg, sizeof(m_ParaUoLblTimeReg));

		m_TimeVal	=	0;
		m_IsDst		=	FALSE;

		// regist timer
		GrP5TimerRegist(this);

}
//--------------------------------------------------------------------
Cls_GrP5UoLblTimeReg::~Cls_GrP5UoLblTimeReg(void)
{
	// local -------------------
	// code --------------------
		
		// unregist
		GrP5TimerUnregist(this);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblTimeReg::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoLblTimeBase::EvtScptDecDone();

		LcTimeValueUpdate();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblTimeReg::DvlParaInfoBuild(void)
{
	// local -------------------
		St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoLblTimeBase::DvlParaInfoBuild();

		// separator basic info
		Tv_ParaInfo.StrName		=	V_Cls_GrP5UoLblTimeRegStrParaSepLabelBase;
		Tv_ParaInfo.StrDesc		=	NULL;
		Tv_ParaInfo.ValType		=	E_GrP5DvlValTypeSeparator;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.Act				=	0;
		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);

		DvlParaInfoAdd(V_Cls_GrP5UoLblTimeRegParaInfo, E_Cls_GrP5UoLblTimeRegParaInfoCnt, &m_ParaUoLblTimeReg);

		DvlParaInfoRelPosAdd(L"Label time register DST Image rect", &m_ParaUoLblTimeReg.RcIconDst);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblTimeReg::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoLblTimeBase::DvlParaInfoAction(A_Action);

		// custom action
		LcTimeValueUpdate();
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblTimeReg::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
	// delete prepare


		// inherited
		Cls_GrP5UoLblTimeBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoLblTimeReg::LcTimeValueUpdate(void)
{
	// local -------------------
		__u32	Tv_Val;
#ifdef GR_P5_EDITOR
		__u32	Tv_Mili;
#endif
	// code --------------------
		// get value
#ifdef GR_P5_EDITOR
		Tv_Val	=	GrTimeSysTimeGet(&Tv_Mili);
#else
		Tv_Val		=	GrP5RintGet(m_ParaUoLblTimeReg.RegTime);
#endif
		LcTimeDispUpdate((Def_GrTime)Tv_Val);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblTimeReg::EvtTimerTick(void)
{
	// local -------------------
	// code --------------------
		if(m_ParaUoLblTimeReg.IsObserve)
		{
			// update value
			LcTimeValueUpdate();
		}
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrP5UoLblTimeReg::TimeGet(void)
{
		LcTimeValueUpdate();
		return	m_TimeVal;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblTimeReg::LcTimeDispUpdate(Def_GrTime A_Time)
{
	// local -------------------
		Def_GrTime	Tv_TimeView;
	// code --------------------
		if(m_TimeVal != A_Time)
		{
			// update value
			m_TimeVal	=	A_Time;
			GrP5RintSet(m_ParaUoLblTimeReg.RegTime,(__s32)m_TimeVal);
			if(0 == m_TimeVal)
			{
				m_IsDst	=	FALSE;
				GrStrClear(m_SkinCtlLblBase.Str);
			}
			else
			{
				Tv_TimeView	=	m_TimeVal;
				m_IsDst	=	GrTimeIsSmTime(m_TimeVal);
				if(m_IsDst)
				{
					Tv_TimeView	=	Tv_TimeView + E_GrTimeSecOfHour;
				}
				GrStrTimeToWstr(m_SkinCtlLblBase.Str, Tv_TimeView, m_ParaUoLblTimeBase.IsNoTime, m_ParaUoLblTimeBase.IsNoDate, m_ParaUoLblTimeBase.IsNoSec,
					1, 1, m_ParaUoLblTimeBase.GapDateTime, TRUE, TRUE, m_IsApmShow,GrP5TextGet(E_GrP5TxtIdTimeAm), GrP5TextGet(E_GrP5TxtIdTimePm));
			}
			ReqDraw(FALSE);
		}

}
//--------------------------------------------------------------------

