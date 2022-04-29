/*
Platform 5 UI object base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5WinBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <P5/GrP5Key.h>

#ifdef GR_P5_EDITOR

#include <P5/Dvl/GrP5DvlBase.h>

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

#define	E_GrP5WinBaseParaInfoCnt							8

St_GrP5DvlScptParaInfo	V_GrP5WinBaseParaInfo[E_GrP5WinBaseParaInfoCnt]	=
{
	{L"Window base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"IsInputLayerLock", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5WinBasePara)0)->IsInputLayerLock, TRUE,0, L"Input layer lock"},
	{L"IsAutoHide", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5WinBasePara)0)->IsAutoHide, TRUE, 0, L"Auto hide window"},
	{L"IsMseSkip", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5WinBasePara)0)->IsMseSkip, TRUE, 0, L"mouse control skip"},
	{L"IsKeySkip", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5WinBasePara)0)->IsKeySkip, TRUE, 0, L"key control skip"},
	{L"IsKeyMseQuit", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5WinBasePara)0)->IsKeyMseQuit, TRUE, 0, L"Quit by key or mouse"},
	{L"IsMseGlb", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5WinBasePara)0)->IsMseGlb, TRUE, 0, L"Global mouse input"},
	{L"FocusScptId", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5WinBasePara)0)->FocusScptId, TRUE, 0, L"Focuse script id"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5WinBase::Cls_GrP5WinBase(__u16 A_ScptId, __u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect, Cls_GrP5UoBase* A_ObjPrnt) :
Cls_GrP5UoBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
		
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdWinBase, &m_ParaWinBase, sizeof(m_ParaWinBase), NULL);

		m_WinKeyFcsObjId	=	E_GrP5ObjIdNone;
		m_Layer				=	A_LayerIdx;
		m_IsWinHide		=	FALSE;

		if(NULL == A_ObjPrnt)
		{
			m_ObjWin		=	this;
			// get console index
			m_ConIdx		=	A_ConIdx;

			// build screen rect
			RtlScrRectBld();

			// allocate object id
			m_ObjId			=	GrP5WinObjRegist(A_ConIdx, A_LayerIdx, this);
			if(E_GrP5ObjIdNone == m_ObjId)
			{
				DbgMsgPrint("[ERROR!] Cls_GrP5WinBase::Cls_GrP5WinBase - Cannot regist window (ScptId:%d) object\n", (int)A_ScptId);
				return;
			}
		}
		
		// default setting
		GrDumyZeroMem(&m_ParaWinBase, sizeof(m_ParaWinBase));
		m_ParaWinBase.IsInputLayerLock	=	TRUE;
		m_ParaUoBase.SkinPara.ParaColor	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 0), GrP5ScriptPxFmtGet());
		m_ParaUoBase.SkinPara.ParaByte0	=	100;

		if(NULL == A_ObjPrnt)
		{
			// request draw
			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------
Cls_GrP5WinBase::~Cls_GrP5WinBase(void)
{
	// local -------------------
	// code --------------------
		
		// redraw alrea
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5WinBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// add parametter
		DvlParaInfoAdd(V_GrP5WinBaseParaInfo, E_GrP5WinBaseParaInfoCnt, &m_ParaWinBase);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinBase::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------

		// delete prepare


		// inherited
		Cls_GrP5UoBase::DvlPreDelete();

}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinBase::IsInputLayerLock(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if(m_ParaUoBase.IsVisible)
		{
			Tv_Result	=	m_ParaWinBase.IsInputLayerLock;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinBase::CloseRequest(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP5MsgConWinCloseReq	Tv_Msg;
	// code --------------------
		Tv_Msg.ConHd.CmdHd.Cmd	=	E_GrP5MsgCmdConWinCloseReq;
		Tv_Msg.ConHd.ConIdx			=	m_ConIdx;
		Tv_Msg.ObjId						=	m_ObjId;

		Tv_Result	=	GrP5MsgPost(&Tv_Msg, sizeof(Tv_Msg));
#ifdef GR_TASKMSG_DEBUG
		if(!Tv_Result)
		{
			DbgMsgPrint("[DBG] %s MsgPost fail!\n", __FUNCTION__);
		}
#endif
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinBase::IsMseSkip(void)
{
		return	m_ParaWinBase.IsMseSkip;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinBase::IsKeySkip(void)
{
		return	m_ParaWinBase.IsKeySkip;
}
//--------------------------------------------------------------------
__u8	Cls_GrP5WinBase::LayerGet(void)
{
		return	m_Layer;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinBase::IsMseGlb(void)
{
		return	m_ParaWinBase.IsMseGlb;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5WinBase::RtlWinKeyFocusSlaveGet(void)
{
		return	m_WinKeyFcsObjId;
}
//--------------------------------------------------------------------
void	Cls_GrP5WinBase::RtlWinKeyFocusSlaveSet(__u16 A_ObjId)
{
		m_WinKeyFcsObjId	=	A_ObjId;
		// focus event
		EvtWinKeyFocusGet();
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5WinBase::RtlUoFindKeyFocusAble(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		Cls_GrP5UoBase*	Tv_ObjSlv;
	// code --------------------
		Tv_Result	=	NULL;

		if((!m_ParaWinBase.IsKeySkip) && m_ParaUoBase.IsEnable && m_ParaUoBase.IsVisible)
		{
			// check exist last key focus
			Tv_Result	=	(Cls_GrP5UoBase*)GrP5UiObjGet(m_WinKeyFcsObjId);
			if(NULL == Tv_Result)
			{
				Tv_ObjSlv	=	m_ObjSlave;
				while(NULL != Tv_ObjSlv)
				{
					Tv_Result	=	Tv_ObjSlv->RtlUoFindKeyFocusAble();
					if(NULL != Tv_Result)
					{
						break;
					}
					// next
					Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
				}
			}

			if(NULL == Tv_Result)
			{
				if(m_ParaUoBase.IsCtlEvtKey)
				{
					Tv_Result	=	(Cls_GrP5UoBase*)this;
				}
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5WinBase::RtlDoEvtScptDecDone(BOOL8 A_IsFocusUpdt)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Uo;
	// code --------------------
		if(A_IsFocusUpdt)
		{
			Tv_Uo	=	RtlUoFindByScptId(m_ParaWinBase.FocusScptId);
			if(NULL != Tv_Uo)
			{
				Tv_Uo->ReqKeyFocusSet();
			}
		}

		Cls_GrP5UoBase::RtlDoEvtScptDecDone();
}
//--------------------------------------------------------------------
void	Cls_GrP5WinBase::RtlDoEvtWinClose(void)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		// do control event
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			Tv_Ctrl->EvtCloseWin(m_ParaUoBase.CtrlId, m_ObjId);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5WinBase::EvtWinKeyFocusGet(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5WinBase::EvtWinKeyFocusLoss(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5WinBase::RtlPaintProc(Cls_GrGdib* A_GdibScr, Ptr_GrRect A_PtrRcScr)
{
	// local -------------------
	// code --------------------
#ifdef GR_P5_EDITOR
		if(((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->DvlIsEdit())
		{
			Cls_GrP5UoBase::RtlPaintProc(A_GdibScr, A_PtrRcScr);
		}
		else
		{
#endif
			if(!m_IsWinHide)
			{
				Cls_GrP5UoBase::RtlPaintProc(A_GdibScr, A_PtrRcScr);
			}
#ifdef GR_P5_EDITOR
		}
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP5WinBase::RtlAutoHideUpdate(BOOL8 A_IsHide)
{
	// local -------------------
	// code --------------------
		if(m_ParaWinBase.IsAutoHide)
		{
			if(m_IsWinHide != A_IsHide)
			{
				m_IsWinHide		=	A_IsHide;
				ReqDraw(FALSE);
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinBase::EvtMseRbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP5UoBase::EvtMseRbtnUp(A_X,A_Y,A_BtnMap);
		if(m_ParaWinBase.IsKeyMseQuit)
		{
			GrP5WindowCloseByObjId(m_ObjId);
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinBase::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP5UoBase::EvtKeyUp(A_Key);
		if(!Tv_Result)
		{
			if(((E_GrP5KeyEsc == A_Key) || (E_GrP5KeyMenu == A_Key)) && (m_ParaWinBase.IsKeyMseQuit))
			{
				GrP5WindowCloseByObjId(m_ObjId);
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinBase::IsKeyAble(void)
{
	// local -------------------
	// code --------------------
		return	m_ParaUoBase.IsCtlEvtKey;
}
//--------------------------------------------------------------------
