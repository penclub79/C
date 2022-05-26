/*
Platform 5 UI object Button base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoBtnBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <P5/GrP5UoLblBase.h>
#include <P5/GrP5Key.h>

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
#define	E_GrP5UoBtnBaseParaInfoCnt							5

St_GrP5DvlScptParaInfo	V_GrP5UoBtnBaseParaInfo[E_GrP5UoBtnBaseParaInfoCnt]	=
{
	{L"Button base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"IsSkipKey", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBtnBasePara)0)->IsSkipKey, TRUE, 0, L"Skip key input"},
	{L"PumpPeriod", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBtnBasePara)0)->PumpPeriod, TRUE, 0, L"Pump period , 0 = off"},
	{L"EvtHostId", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnBasePara)0)->EvtHostId, TRUE, 0, L"Event host script id"},
	{L"RelLbl", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnBasePara)0)->RelLbl, TRUE, 0, L"Relate label"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoBtnBase::Cls_GrP5UoBtnBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdBtnBase, &m_ParaUoBtnBase, sizeof(m_ParaUoBtnBase), &m_SkinCtlBtnBase);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoBtnBase, sizeof(m_ParaUoBtnBase));
		GrDumyZeroMem(&m_SkinCtlBtnBase, sizeof(m_SkinCtlBtnBase));

		m_HostObjId		=	E_GrP5ObjIdNone;

		m_PumpPrgs		=	0;
}
//--------------------------------------------------------------------
Cls_GrP5UoBtnBase::~Cls_GrP5UoBtnBase(void)
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoBtnBaseParaInfo, E_GrP5UoBtnBaseParaInfoCnt, &m_ParaUoBtnBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnBase::EvtScptDecDone(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Uo;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();

		// update timer
		if(0 == m_ParaUoBtnBase.PumpPeriod)
		{
			GrP5TimerUnregist(this);
		}
		else
		{
			GrP5TimerRegist(this);
		}

		m_PumpPrgs	=	0;

		if(E_GrP5ScptIdNone != m_ParaUoBtnBase.EvtHostId)
		{
			if(NULL != m_ObjWin)
			{
				Tv_Uo	=	m_ObjWin->RtlUoFindByScptId(m_ParaUoBtnBase.EvtHostId);
				if(NULL != Tv_Uo)
				{
					m_HostObjId	=	Tv_Uo->ObjIdGet();
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBtnBase::IsKeyAble(void)
{
		return	!m_ParaUoBtnBase.IsSkipKey;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBtnBase::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnBase::EvtKeyFocusGet(__u32 A_Para)
{
	// local -------------------
		Cls_GrP5UoLblBase*	Tv_Lbl;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoBtnBase.RelLbl)
		{
			Tv_Lbl	=	(Cls_GrP5UoLblBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoBtnBase.RelLbl);
			if(NULL != Tv_Lbl)
			{
				if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblBase))
				{
					Tv_Lbl->RtlStatSet(E_GrP5StatOnFocus);
				}
#ifdef GR_P5_EDITOR
				else
				{
					DbgMsgPrint("Cls_GrP5UoBtnBase::EvtKeyFocusGet - script id (%d) is not label!\n", m_ParaUoBtnBase.RelLbl);
				}
#endif
			}
#ifdef GR_P5_EDITOR
			else
			{
				DbgMsgPrint("Cls_GrP5UoBtnBase::EvtKeyFocusGet - bad relate lable script id (%d)!\n", m_ParaUoBtnBase.RelLbl);
			}
#endif
		}

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnBase::EvtKeyFocusLoss(void)
{
	// local -------------------
		Cls_GrP5UoLblBase*	Tv_Lbl;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoBtnBase.RelLbl)
		{
			Tv_Lbl	=	(Cls_GrP5UoLblBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoBtnBase.RelLbl);
			if(NULL != Tv_Lbl)
			{
				if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblBase))
				{
					Tv_Lbl->RtlStatSet(0);
				}
#ifdef GR_P5_EDITOR
				else
				{
					DbgMsgPrint("Cls_GrP5UoBtnBase::EvtKeyFocusLoss - script id (%d) is not label!\n", m_ParaUoBtnBase.RelLbl);
				}
#endif
			}
#ifdef GR_P5_EDITOR
			else
			{
				DbgMsgPrint("Cls_GrP5UoBtnBase::EvtKeyFocusLoss - bad relate lable script id (%d)!\n", m_ParaUoBtnBase.RelLbl);
			}
#endif
		}

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnBase::EvtMseEnter(void)
{
	// local -------------------
		Cls_GrP5UoLblBase*	Tv_Lbl;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoBtnBase.RelLbl)
		{
			Tv_Lbl	=	(Cls_GrP5UoLblBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoBtnBase.RelLbl);
			if(NULL != Tv_Lbl)
			{
				if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblBase))
				{
					Tv_Lbl->RtlStatSet(E_GrP5StatOnFocus);
				}
#ifdef GR_P5_EDITOR
				else
				{
					DbgMsgPrint("Cls_GrP5UoBtnBase::EvtMseEnter - script id (%d) is not label!\n", m_ParaUoBtnBase.RelLbl);
				}
#endif
			}
#ifdef GR_P5_EDITOR
			else
			{
				DbgMsgPrint("Cls_GrP5UoBtnBase::EvtMseEnter - bad relate lable script id (%d)!\n", m_ParaUoBtnBase.RelLbl);
			}
#endif
		}

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnBase::EvtMseLeave(void)
{
	// local -------------------
		Cls_GrP5UoLblBase*	Tv_Lbl;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoBtnBase.RelLbl)
		{
			Tv_Lbl	=	(Cls_GrP5UoLblBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoBtnBase.RelLbl);
			if(NULL != Tv_Lbl)
			{
				if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblBase))
				{
					Tv_Lbl->RtlStatSet(0);
				}
#ifdef GR_P5_EDITOR
				else
				{
					DbgMsgPrint("Cls_GrP5UoBtnBase::EvtMseLeave - script id (%d) is not label!\n", m_ParaUoBtnBase.RelLbl);
				}
#endif
			}
#ifdef GR_P5_EDITOR
			else
			{
				DbgMsgPrint("Cls_GrP5UoBtnBase::EvtMseLeave - bad relate lable script id (%d)!\n", m_ParaUoBtnBase.RelLbl);
			}
#endif
		}

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBtnBase::EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
	// code --------------------
		MseCapture();
		
		m_SkinCtlBtnBase.IsPush	=	TRUE;
		ReqDraw(FALSE);

		// event button press
		DoEvtBtnPress();

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBtnBase::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		
	// code --------------------

		MseRelese();
		m_SkinCtlBtnBase.IsPush	=	FALSE;
		ReqDraw(FALSE);

		// event button release
		DoEvtBtnRelease();

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBtnBase::EvtKeyDown(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(E_GrP5KeyEnter == A_Key)
		{
			m_SkinCtlBtnBase.IsPush	=	TRUE;
			ReqDraw(FALSE);

			// event button press
			DoEvtBtnPress();

			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyDown(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBtnBase::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(E_GrP5KeyEnter == A_Key)
		{
			m_SkinCtlBtnBase.IsPush	=	FALSE;
			ReqDraw(FALSE);

			// event button release
			DoEvtBtnRelease();

			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyUp(A_Key);
		}

		return	Tv_Result;
		
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnBase::DoEvtBtnPress(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_EvtHost;
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		// event 
		if(E_GrP5ObjIdNone == m_HostObjId)
		{
			Tv_EvtHost	=	m_ObjPrnt;
		}
		else
		{
			Tv_EvtHost	=	(Cls_GrP5UoBase*)GrP5UiObjGet(m_HostObjId);
		}
		if(NULL != Tv_EvtHost)
		{
			Tv_EvtHost->EvtBtnPress(m_ObjId);
		}

		// contorl
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			Tv_Ctrl->EvtBtnPress(m_ParaUoBase.CtrlId, m_ObjId);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnBase::DoEvtBtnRelease(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_EvtHost;
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		// event 
		if(E_GrP5ObjIdNone == m_HostObjId)
		{
			Tv_EvtHost	=	m_ObjPrnt;
		}
		else
		{
			Tv_EvtHost	=	(Cls_GrP5UoBase*)GrP5UiObjGet(m_HostObjId);
		}
		if(NULL != Tv_EvtHost)
		{
			Tv_EvtHost->EvtBtnRelease(m_ObjId);
		}

		// contorl
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			Tv_Ctrl->EvtBtnRelease(m_ParaUoBase.CtrlId, m_ObjId);
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBtnBase::IsSelected(void)
{
	// local -------------------
	// code --------------------
	
		return	m_SkinCtlBtnBase.IsSelect;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnBase::SelectSet(BOOL8 A_IsSet)
{
		m_SkinCtlBtnBase.IsSelect	=	A_IsSet;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnBase::EvtTimerTick(void)
{
	// local -------------------
	// code --------------------
		if(0 != m_ParaUoBtnBase.PumpPeriod)
		{
			if(m_SkinCtlBtnBase.IsPush)
			{
				m_PumpPrgs ++;
				if(m_ParaUoBtnBase.PumpPeriod <= m_PumpPrgs)
				{
					m_PumpPrgs	=	0;
					// do event
					DoEvtBtnRelease();
					DoEvtBtnPress();
				}
			}
			else
			{
				m_PumpPrgs	=	0;
			}
		}
}
//--------------------------------------------------------------------
