/*
Platform 5 UI object Label Text Id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoLblTimeView.h>

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


#define	E_Cls_GrP5UoLblTimeViewParaInfoCnt							4

St_GrP5DvlScptParaInfo	V_Cls_GrP5UoLblTimeViewParaInfo[E_Cls_GrP5UoLblTimeViewParaInfoCnt]	=
{
	{L"Label time view", E_GrP5DvlValTypeSeparator, 1, NULL, TRUE, 0, NULL},
	{L"IsPrgs", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoLblTimeViewPara)0)->IsPrgs, TRUE, 0, L"run progress"},
	{L"IsSpriteImg", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoLblTimeViewPara)0)->IsSpriteImg, TRUE, 0, L"Sprite image"},
	{L"ImgDst", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoLblTimeViewPara)0)->ImgDst, TRUE, 0, L"DST image id"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoLblTimeView::Cls_GrP5UoLblTimeView(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoLblTimeBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdLblTimeView, &m_ParaUoLblTimeView, sizeof(m_ParaUoLblTimeView), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoLblTimeView, sizeof(m_ParaUoLblTimeView));

		m_TimeVal	=	0;
		m_IsDst		=	FALSE;

		// regist timer
		GrP5TimerRegist(this);
}
//--------------------------------------------------------------------
Cls_GrP5UoLblTimeView::~Cls_GrP5UoLblTimeView(void)
{
	// local -------------------
	// code --------------------
		
		// unregist
		GrP5TimerUnregist(this);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblTimeView::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoLblTimeBase::EvtScptDecDone();

		LcTimeValueUpdate();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblTimeView::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoLblTimeBase::DvlParaInfoBuild();

		// separator basic info
		DvlParaInfoAdd(V_Cls_GrP5UoLblTimeViewParaInfo, E_Cls_GrP5UoLblTimeViewParaInfoCnt, &m_ParaUoLblTimeView);

		DvlParaInfoRelPosAdd(L"Lbl Time View Dst Icon Rect", &m_ParaUoLblTimeView.RcIconDst);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblTimeView::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoLblTimeBase::DvlParaInfoAction(A_Action);

		// custom action
		LcTimeValueUpdate();
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblTimeView::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
	// delete prepare


		// inherited
		Cls_GrP5UoLblTimeBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoLblTimeView::LcTimeValueUpdate(void)
{
	// local -------------------
		__u32	Tv_Mili;
		Def_GrTime	Tv_TimeVal;
	// code --------------------
		// get value
		Tv_TimeVal	=	GrTimeSysTimeGet(&Tv_Mili);
		TimeSet(Tv_TimeVal);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblTimeView::EvtTimerTick(void)
{
	// local -------------------
	// code --------------------
		if(m_ParaUoLblTimeView.IsPrgs)
		{
			// update value
			LcTimeValueUpdate();
		}
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrP5UoLblTimeView::TimeGet(void)
{
		return	m_TimeVal;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblTimeView::TimeSet(Def_GrTime A_Time)
{
	// local -------------------
		Def_GrTime	Tv_TimeView;
	// code --------------------
		if(m_TimeVal != A_Time)
		{
			// update value
			m_TimeVal	=	A_Time;
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
					1, 1, m_ParaUoLblTimeBase.GapDateTime, TRUE, TRUE, m_IsApmShow,GrP5TextGet(E_GrP5TxtIdTimeAm), 
					GrP5TextGet(E_GrP5TxtIdTimePm));
			}
			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------
