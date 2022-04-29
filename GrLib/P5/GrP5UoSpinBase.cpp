/*
Platform 5 UI object Button base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoSpinBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <P5/GrP5Key.h>
#include <P5/GrP5UoLblBase.h>

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
#define	E_GrP5UoSpinBaseParaInfoCnt							6

St_GrP5DvlScptParaInfo	V_GrP5UoSpinBaseParaInfo[E_GrP5UoSpinBaseParaInfoCnt]	=
{
	{L"Spin base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoSpinBasePara)0)->FontId, TRUE, E_GrP5DvlParaInfoActDefault, L"font id"},
	{L"IsKeyCover", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoSpinBasePara)0)->IsKeyCover, TRUE, E_GrP5DvlParaInfoActDefault, L"Key cover use"},
	{L"IsVertical", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoSpinBasePara)0)->IsVertical, TRUE, E_GrP5DvlParaInfoActDefault, L"control direction"},
	{L"IsNotRotate", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoSpinBasePara)0)->IsNotRotate, TRUE, E_GrP5DvlParaInfoActDefault, L"value not rotate"},
	{L"RelLbl", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoSpinBasePara)0)->RelLbl, TRUE, E_GrP5DvlParaInfoActDefault, L"Relate label"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoSpinBase::Cls_GrP5UoSpinBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdSpinBase, &m_ParaUoSpinBase, sizeof(m_ParaUoSpinBase), &m_SkinCtlSpinBase);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoSpinBase, sizeof(m_ParaUoSpinBase));
		GrDumyZeroMem(&m_SkinCtlSpinBase, sizeof(m_SkinCtlSpinBase));

		m_ParaUoSpinBase.TxtRc.Left.Ratio		=	0;
		m_ParaUoSpinBase.TxtRc.Left.Val			=	0;
		m_ParaUoSpinBase.TxtRc.Top.Ratio		=	0;
		m_ParaUoSpinBase.TxtRc.Top.Val			=	0;
		m_ParaUoSpinBase.TxtRc.Right.Ratio	=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoSpinBase.TxtRc.Right.Val		=	0;
		m_ParaUoSpinBase.TxtRc.Bottom.Ratio	=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoSpinBase.TxtRc.Bottom.Val		=	0;

		m_ParaUoSpinBase.TxtPara.Atb		=	E_GrP5FontAtbDrawText;

		m_SkinCtlSpinBase.ItmIdx				=	E_GrP5SkinSpinIdxCont;
		m_SkinCtlSpinBase.IsPush				=	FALSE;
		m_SkinCtlSpinBase.IsKeyCoverOn	=	FALSE;

		m_EvtHostId			=	E_GrP5ObjIdNone;

}
//--------------------------------------------------------------------
Cls_GrP5UoSpinBase::~Cls_GrP5UoSpinBase(void)
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoSpinBaseParaInfo, E_GrP5UoSpinBaseParaInfoCnt, &m_ParaUoSpinBase);
		DvlParaInfoTxtParaAdd(L"Spin text Para", &m_ParaUoSpinBase.TxtPara);
		DvlParaInfoRelPosAdd(L"Spin Text Rect", &m_ParaUoSpinBase.TxtRc);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		// update key cover
		//m_SkinCtlSpinBase.IsKeyCoverOn	=	m_ParaUoSpinBase.IsKeyCover;
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinBase::EvtPaintProc(void)
{
	// local -------------------
	// code --------------------
		ValueRead();

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------

		//m_SkinCtlSpinBase.IsKeyCoverOn	=	m_ParaUoSpinBase.IsKeyCover;
		//ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinBase::IsKeyAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinBase::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinBase::EvtKeyFocusGet(__u32 A_Para)
{
	// local -------------------
		Cls_GrP5UoLblBase*	Tv_Lbl;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoSpinBase.RelLbl)
		{
			Tv_Lbl	=	(Cls_GrP5UoLblBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoSpinBase.RelLbl);
			if(NULL != Tv_Lbl)
			{
				if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblBase))
				{
					Tv_Lbl->RtlStatSet(E_GrP5StatOnFocus);
				}
#ifdef GR_P5_EDITOR
				else
				{
					DbgMsgPrint("Cls_GrP5UoSpinBase::EvtKeyFocusGet - script id (%d) is not label!\n", m_ParaUoSpinBase.RelLbl);
				}
#endif
			}
#ifdef GR_P5_EDITOR
			else
			{
				DbgMsgPrint("Cls_GrP5UoSpinBase::EvtKeyFocusGet - bad relate lable script id (%d)!\n", m_ParaUoSpinBase.RelLbl);
			}
#endif
		}

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinBase::EvtKeyFocusLoss(void)
{
	// local -------------------
		Cls_GrP5UoLblBase*	Tv_Lbl;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoSpinBase.RelLbl)
		{
			Tv_Lbl	=	(Cls_GrP5UoLblBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoSpinBase.RelLbl);
			if(NULL != Tv_Lbl)
			{
				if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblBase))
				{
					Tv_Lbl->RtlStatSet(0);
				}
#ifdef GR_P5_EDITOR
				else
				{
					DbgMsgPrint("Cls_GrP5UoSpinBase::EvtKeyFocusLoss - script id (%d) is not label!\n", m_ParaUoSpinBase.RelLbl);
				}
#endif
			}
#ifdef GR_P5_EDITOR
			else
			{
				DbgMsgPrint("Cls_GrP5UoSpinBase::EvtKeyFocusLoss - bad relate lable script id (%d)!\n", m_ParaUoSpinBase.RelLbl);
			}
#endif
		}

		m_SkinCtlSpinBase.IsKeyCoverOn	=	FALSE;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinBase::EvtMseEnter(void)
{
	// local -------------------
		Cls_GrP5UoLblBase*	Tv_Lbl;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoSpinBase.RelLbl)
		{
			Tv_Lbl	=	(Cls_GrP5UoLblBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoSpinBase.RelLbl);
			if(NULL != Tv_Lbl)
			{
				if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblBase))
				{
					Tv_Lbl->RtlStatSet(E_GrP5StatOnFocus);
				}
#ifdef GR_P5_EDITOR
				else
				{
					DbgMsgPrint("Cls_GrP5UoSpinBase::EvtMseEnter - script id (%d) is not label!\n", m_ParaUoSpinBase.RelLbl);
				}
#endif
			}
#ifdef GR_P5_EDITOR
			else
			{
				DbgMsgPrint("Cls_GrP5UoSpinBase::EvtMseEnter - bad relate lable script id (%d)!\n", m_ParaUoSpinBase.RelLbl);
			}
#endif

		}
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinBase::EvtMseLeave(void)
{
	// local -------------------
		Cls_GrP5UoLblBase*	Tv_Lbl;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoSpinBase.RelLbl)
		{
			Tv_Lbl	=	(Cls_GrP5UoLblBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoSpinBase.RelLbl);
			if(NULL != Tv_Lbl)
			{
				if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblBase))
				{
					Tv_Lbl->RtlStatSet(0);
				}
#ifdef GR_P5_EDITOR
				else
				{
					DbgMsgPrint("Cls_GrP5UoSpinBase::EvtMseLeave - script id (%d) is not label!\n", m_ParaUoSpinBase.RelLbl);
				}
#endif
			}
#ifdef GR_P5_EDITOR
			else
			{
				DbgMsgPrint("Cls_GrP5UoSpinBase::EvtMseLeave - bad relate lable script id (%d)!\n", m_ParaUoSpinBase.RelLbl);
			}
#endif

		}
		ReqDraw(FALSE);

		m_SkinCtlSpinBase.ItmIdx	=	0;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinBase::EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__u8	Tv_BtnIdx;
	// code --------------------
		if(!m_SkinCtlSpinBase.IsPush)
		{
			Tv_BtnIdx		=	BtnIdxGetByScrPos(m_RcScr.left + A_X, m_RcScr.top + A_Y);
			if(Tv_BtnIdx != m_SkinCtlSpinBase.ItmIdx)
			{
				m_SkinCtlSpinBase.ItmIdx	=	Tv_BtnIdx;
				ReqDraw(FALSE);
			}
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinBase::EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		//Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		MseCapture();
		
		m_SkinCtlSpinBase.IsPush		=	TRUE;

		if(E_GrP5SkinSpinIdxBtnDec == m_SkinCtlSpinBase.ItmIdx)
		{
			DoDecrease();
		}
		else if(E_GrP5SkinSpinIdxBtnInc == m_SkinCtlSpinBase.ItmIdx)
		{
			DoIncrease();
		}

		ReqDraw(FALSE);

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinBase::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		//Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------

		MseRelese();
		m_SkinCtlSpinBase.IsPush		=	FALSE;

		if(E_GrP5SkinSpinIdxCont == m_SkinCtlSpinBase.ItmIdx)
		{
			// run event
			ProcSpinClickCont();
			// run control
			DoEvtSpinClickCont();
		}

		ReqDraw(FALSE);

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinBase::EvtMseWheelUp(__u8 A_WheelIdx)
{
	// local -------------------
	// code --------------------
		DoIncrease();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinBase::EvtMseWheelDown(__u8 A_WheelIdx)
{
		DoDecrease();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinBase::EvtKeyDown(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch(A_Key)
		{
			case E_GrP5KeyEnter:
				if(!m_ParaUoSpinBase.IsKeyCover)
				{
					Tv_Result	=	FALSE;
				}
				break;
			case E_GrP5KeyLeft:
				Tv_Result	=	m_SkinCtlSpinBase.IsKeyCoverOn;
				if(!m_ParaUoSpinBase.IsVertical)
				{
					if((!m_ParaUoSpinBase.IsKeyCover) || (m_ParaUoSpinBase.IsKeyCover && m_SkinCtlSpinBase.IsKeyCoverOn))
					{
						DoDecrease();
						Tv_Result	=	TRUE;
					}
				}
				break;
			case E_GrP5KeyRight:
				Tv_Result	=	m_SkinCtlSpinBase.IsKeyCoverOn;
				if(!m_ParaUoSpinBase.IsVertical)
				{
					if((!m_ParaUoSpinBase.IsKeyCover) || (m_ParaUoSpinBase.IsKeyCover && m_SkinCtlSpinBase.IsKeyCoverOn))
					{
						DoIncrease();
						Tv_Result	=	TRUE;
					}
				}
				break;
			case E_GrP5KeyDown:
				Tv_Result	=	m_SkinCtlSpinBase.IsKeyCoverOn;
				if(m_ParaUoSpinBase.IsVertical)
				{
					if((!m_ParaUoSpinBase.IsKeyCover) || (m_ParaUoSpinBase.IsKeyCover && m_SkinCtlSpinBase.IsKeyCoverOn))
					{
						DoDecrease();
						Tv_Result	=	TRUE;
					}
				}
				break;
			case E_GrP5KeyUp:
				Tv_Result	=	m_SkinCtlSpinBase.IsKeyCoverOn;
				if(m_ParaUoSpinBase.IsVertical)
				{
					if((!m_ParaUoSpinBase.IsKeyCover) || (m_ParaUoSpinBase.IsKeyCover && m_SkinCtlSpinBase.IsKeyCoverOn))
					{
						DoIncrease();
						Tv_Result	=	TRUE;
					}
				}
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		if(!Tv_Result)
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyDown(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinBase::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		
		switch(A_Key)
		{
			case E_GrP5KeyEnter:
				if(m_ParaUoSpinBase.IsKeyCover)
				{
					m_SkinCtlSpinBase.IsKeyCoverOn	=	!m_SkinCtlSpinBase.IsKeyCoverOn;
					ReqDraw(FALSE);
				}
				else
				{
					// run event
					ProcSpinClickCont();
					// run control
					DoEvtSpinClickCont();
				}
				break;
			case E_GrP5KeyLeft:
				Tv_Result	=	m_SkinCtlSpinBase.IsKeyCoverOn;
				if(!m_ParaUoSpinBase.IsVertical)
				{
					Tv_Result	=	TRUE;
				}
				break;
			case E_GrP5KeyRight:
				Tv_Result	=	m_SkinCtlSpinBase.IsKeyCoverOn;
				if(!m_ParaUoSpinBase.IsVertical)
				{
					Tv_Result	=	TRUE;
				}
				break;
			case E_GrP5KeyDown:
				Tv_Result	=	m_SkinCtlSpinBase.IsKeyCoverOn;
				if(m_ParaUoSpinBase.IsVertical)
				{
					Tv_Result	=	TRUE;
				}
				break;
			case E_GrP5KeyUp:
				Tv_Result	=	m_SkinCtlSpinBase.IsKeyCoverOn;
				if(m_ParaUoSpinBase.IsVertical)
				{
					Tv_Result	=	TRUE;
				}
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		if(!Tv_Result)
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyUp(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinBase::DoIncrease(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinBase::DoDecrease(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinBase::ValueRead(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinBase::ValueWrite(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinBase::EvtTimerTick(void)
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoSpinBase::BtnIdxGetByScrPos(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		__s32	Tv_IdxX;
		__s32	Tv_IdxY;
	// code --------------------
		if(NULL == m_SkinMng)
		{
			return	E_GrP5SkinSpinIdxCont;
		}

		m_SkinMng->CtlGetItemIdxByPos(this, A_X, A_Y, &Tv_IdxX, &Tv_IdxY);
		return	(__u8)Tv_IdxX;

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinBase::DoEvtSpinValChg(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_EvtHost;
		Cls_GrP5CtrlEvtBase*	Tv_CtrlEvt;
	// code --------------------
		// object event
		if(E_GrP5ObjIdNone == m_EvtHostId)
		{
			Tv_EvtHost	=	m_ObjPrnt;
		}
		else
		{
			Tv_EvtHost	=	(Cls_GrP5UoBase*)GrP5UiObjGet(m_EvtHostId);
		}
		if(NULL != Tv_EvtHost)
		{
			Tv_EvtHost->EvtSpinValChanged(m_ObjId);
		}

		// control event
		Tv_CtrlEvt	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_CtrlEvt)
		{
			Tv_CtrlEvt->EvtSpinValChanged(m_ParaUoBase.CtrlId, m_ObjId);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinBase::DoEvtSpinClickCont(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_EvtHost;
		Cls_GrP5CtrlEvtBase*	Tv_CtrlEvt;
	// code --------------------
		// object event
		if(E_GrP5ObjIdNone == m_EvtHostId)
		{
			Tv_EvtHost	=	m_ObjPrnt;
		}
		else
		{
			Tv_EvtHost	=	(Cls_GrP5UoBase*)GrP5UiObjGet(m_EvtHostId);
		}
		if(NULL != Tv_EvtHost)
		{
			Tv_EvtHost->EvtSpinClickCont(m_ObjId);
		}

		// control event
		Tv_CtrlEvt	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_CtrlEvt)
		{
			Tv_CtrlEvt->EvtSpinClickCont(m_ParaUoBase.CtrlId, m_ObjId);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinBase::ProcSpinClickCont(void)
{

}
//--------------------------------------------------------------------
