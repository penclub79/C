/*
Platform 5 UI object Label base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoPnlAim.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5Mng.h>
#include <P5/GrP5CtrlEvtBase.h>

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

#define	E_GrP5UoPnlAimParaInfoCnt							2

St_GrP5DvlScptParaInfo	V_GrP5UoPnlAimParaInfo[E_GrP5UoPnlAimParaInfoCnt]	=
{
	{L"Pannel Aim", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL},
	{L"Level", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlAimPara)0)->Level, TRUE, 0, L"Position divide level"},
};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoPnlAim::Cls_GrP5UoPnlAim(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdPnlAim, &m_ParaUoPnlAim, sizeof(m_ParaUoPnlAim), NULL);

		// default setting
		GrDumyZeroMem(&m_ParaUoPnlAim, sizeof(m_ParaUoPnlAim));
		m_ParaUoPnlAim.Level	=	10;

		// reset
		m_UpdtStep	=	0;
		m_IsPosChg	=	FALSE;
		m_IsMsePush	=	FALSE;
		m_PosX			=	0;
		m_PosY			=	0;

		// request timer
		GrP5TimerRegist(this);
}
//--------------------------------------------------------------------
Cls_GrP5UoPnlAim::~Cls_GrP5UoPnlAim(void)
{
	// local -------------------
	// code --------------------
		
		//unregist timer
		GrP5TimerUnregist(this);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlAim::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlAim::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// separator basic info
		DvlParaInfoAdd(V_GrP5UoPnlAimParaInfo, E_GrP5UoPnlAimParaInfoCnt, &m_ParaUoPnlAim);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlAim::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		// custom action
		ReqDraw(FALSE);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlAim::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlAim::IsKeyAble(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlAim::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlAim::EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
	// code --------------------

		if(m_IsMsePush)
		{
			m_PosX	=	A_X;
			m_PosY	=	A_Y;
			m_IsPosChg	=	TRUE;
		}

		Cls_GrP5UoBase::EvtMseMove(A_X, A_Y, A_BtnMap);

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlAim::EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
	// code --------------------
		m_IsMsePush	=	TRUE;
		LcNotifyAimEvt(A_X, A_Y, TRUE);
		m_IsPosChg	=	FALSE;

		// inherited
		Cls_GrP5UoBase::EvtMseLbtnDown(A_X, A_Y, A_BtnMap);

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlAim::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
	// code --------------------
		m_IsMsePush	=	FALSE;
		LcNotifyAimEvt(A_X, A_Y, FALSE);
		m_IsPosChg	=	FALSE;

		// inherited
		Cls_GrP5UoBase::EvtMseLbtnUp(A_X, A_Y, A_BtnMap);
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlAim::EvtTimerTick(void)
{
	// local -------------------
	// code --------------------
		m_UpdtStep ++;
		if(E_GrP5UoPnlAimUpdtTimeStep <= m_UpdtStep)
		{
			if(m_IsPosChg)
			{
				// notificate event
				LcNotifyAimEvt(m_PosX, m_PosY, m_IsMsePush);
				
				m_IsPosChg	=	FALSE;
			}
			// reset
			m_UpdtStep	=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlAim::LcNotifyAimEvt(__s32 A_PosX, __s32 A_PosY, BOOL8 A_IsBtnPush)
{
	// local -------------------
		__s32	Tv_PosX;
		__s32	Tv_PosY;
		__s32	Tv_SizeX;
		__s32	Tv_SizeY;
		__s32	Tv_CtX;
		__s32	Tv_CtY;
		__s32	Tv_RtX;
		__s32	Tv_RtY;
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		// calculate x
		Tv_SizeX		=	(m_RcScr.right - m_RcScr.left) >> 1;
		Tv_CtX			=	m_RcScr.left + Tv_SizeX;
		Tv_PosX			=	A_PosX - Tv_CtX;
		if(0 > Tv_PosX)
		{
			Tv_PosX	=	-Tv_PosX;
		}
		Tv_RtX			=	(Tv_PosX * (__s32)(__u32)m_ParaUoPnlAim.Level) / Tv_SizeX;
		if(A_PosX < Tv_CtX)
		{
			Tv_RtX	=	-Tv_RtX;
		}

		// calculate y
		Tv_SizeY		=	(m_RcScr.bottom - m_RcScr.top) >> 1;
		Tv_CtY			=	m_RcScr.top + Tv_SizeY;
		Tv_PosY			=	A_PosY - Tv_CtY;
		if(0 > Tv_PosY)
		{
			Tv_PosY	=	-Tv_PosY;
		}
		Tv_RtY			=	(Tv_PosY * (__s32)(__u32)m_ParaUoPnlAim.Level) / Tv_SizeY;
		if(A_PosY < Tv_CtY)
		{
			Tv_RtY	=	-Tv_RtY;
		}

		// do control event
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			Tv_Ctrl->EvtAim(m_ParaUoBase.CtrlId, m_ObjId, Tv_RtX, Tv_RtY, A_IsBtnPush);
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlAim::EvtMseWheelUp(__u8 A_WheelIdx)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			Tv_Ctrl->EvtMseWheelUp(m_ParaUoBase.CtrlId, m_ObjId, 0);
		}

		return TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlAim::EvtMseWheelDown(__u8 A_WheelIdx)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			Tv_Ctrl->EvtMseWheelDown(m_ParaUoBase.CtrlId, m_ObjId, 0);
		}

		return TRUE;
}
//--------------------------------------------------------------------
