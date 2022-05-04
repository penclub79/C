/*
Platform 5 UI object tab base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoTabBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5Mng.h>
#include <P5/GrP5WinBase.h>
#include <P5/GrP5WinChild.h>
#include <P5/GrP5Key.h>

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

WCHAR	V_GrP5UoTabBaseStrParaSepTabBase[]				=	L"Tab base";

#define	E_GrP5UoTabBaseParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoTabBaseParaInfo[E_GrP5UoTabBaseParaInfoCnt]	=
{
	{L"ScptIdStart", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTabBasePara)0)->ScptIdStart, TRUE, 0, L"Script id start window"},
	{L"ContClr", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoTabBasePara)0)->ContClr, TRUE, 0, L"Contents background color"},
	{L"IsContNoSkin", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTabBasePara)0)->IsContNoSkin, TRUE, 0, L"Contents background not draw"},
};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoTabBase::Cls_GrP5UoTabBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdTabBase, &m_ParaUoTabBase, sizeof(m_ParaUoTabBase), &m_SkinCtlTabBase);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoTabBase, sizeof(m_ParaUoTabBase));
		GrDumyZeroMem(&m_SkinCtlTabBase, sizeof(m_SkinCtlTabBase));
		m_ParaUoTabBase.ContRc.Right.Ratio	=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoTabBase.ContRc.Bottom.Ratio	=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoTabBase.ContClr	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 180, 180, 180), GrP5ScriptPxFmtGet());

		// reset
		m_WinChild	=	NULL;

		GrDumyZeroMem(m_PageStack, sizeof(m_PageStack));
		m_StackIdx	=	0;

}
//--------------------------------------------------------------------
Cls_GrP5UoTabBase::~Cls_GrP5UoTabBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5UoTabBase::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();

		// update start script
#ifdef GR_P5_EDITOR
		if(!((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->DvlIsEdit())
		{
#endif
			
			if(E_GrP5ScptIdNone != m_ParaUoTabBase.ScptIdStart)
			{
				DoTabPageSet(m_ParaUoTabBase.ScptIdStart, E_GrP5ScptIdNone);
			}

#ifdef GR_P5_EDITOR
		}
#endif
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoTabBase::DvlParaInfoBuild(void)
{
	// local -------------------
		St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// separator basic info
		Tv_ParaInfo.StrName		=	V_GrP5UoTabBaseStrParaSepTabBase;
		Tv_ParaInfo.StrDesc		=	NULL;
		Tv_ParaInfo.ValType		=	E_GrP5DvlValTypeSeparator;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.Act				=	0;
		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);

		DvlParaInfoAdd(V_GrP5UoTabBaseParaInfo, E_GrP5UoTabBaseParaInfoCnt, &m_ParaUoTabBase);

		DvlParaInfoRelPosAdd(L"Child position", &m_ParaUoTabBase.ContRc);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoTabBase::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoTabBase::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoTabBase::TabPageSet(__u16 A_ScptId, BOOL8 A_IsPush)
{
	// local -------------------
		St_GrP5MsgUiTabPageSet	Tv_Msg;
	// code --------------------
		Tv_Msg.UiHd.CmdHd.Cmd	=	E_GrP5MsgCmdUiTabPageSet;
		Tv_Msg.UiHd.ObjId			=	m_ObjId;
		Tv_Msg.ScptId					=	A_ScptId;
		Tv_Msg.IsPush					=	A_IsPush;

#ifdef GR_TASKMSG_DEBUG
		if(!GrP5MsgPost(&Tv_Msg, sizeof(Tv_Msg)))
		{
			DbgMsgPrint("[DBG] %s MsgPost fail!\n", __FUNCTION__);
		}
#else
		GrP5MsgPost(&Tv_Msg, sizeof(Tv_Msg));
#endif
				
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTabBase::RtlProcUiMsg(Ptr_GrP5MsgUiHd A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		
		switch(A_PtrMsg->CmdHd.Cmd)
		{
			case E_GrP5MsgCmdUiTabPageSet:
				MprcTabPageSet((Ptr_GrP5MsgUiTabPageSet)A_PtrMsg);
				break;
			case E_GrP5MsgCmdUiTabPagePop:
				MprcTabPagePop();
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTabBase::MprcTabPageSet(Ptr_GrP5MsgUiTabPageSet A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_IsAble;
	// code --------------------

		if(QueryPageChange())
		{
			Tv_IsAble	=	TRUE;

			if(A_PtrMsg->IsPush)
			{
				Tv_IsAble	=	FALSE;
				if(E_GrP5UoTabLvlCnt > (m_StackIdx + 1))
				{
					// backup focused object id
					// get focuse object script id
					m_PageStack[m_StackIdx].FocusId	=	E_GrP5ScptIdNone;
					{
						Cls_GrP5UoBase*	Tv_ObjFcs;
						Tv_ObjFcs	=	(Cls_GrP5UoBase*)GrP5KeyFocusUiObjGet();
						if(NULL != Tv_ObjFcs)
						{
							m_PageStack[m_StackIdx].FocusId	=	Tv_ObjFcs->ScptIdGet();
						}
					}
					

					m_StackIdx ++;
					Tv_IsAble	=	TRUE;
				}
				else
				{
					DbgMsgPrint("Cls_GrP5UoTabBase::MprcTabPageSet - stack is full!\n");
				}
			}

			if(Tv_IsAble)
			{
				DoTabPageSet(A_PtrMsg->ScptId, E_GrP5ScptIdNone);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoTabBase::MprcTabPagePop(void)
{
	// local -------------------
	// code --------------------
		if(QueryPageChange())
		{
			if(0 != m_StackIdx)
			{
				m_StackIdx --;
				DoTabPageSet(m_PageStack[m_StackIdx].ScptId, m_PageStack[m_StackIdx].FocusId);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTabBase::RtlDoEvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// push start page
		if(E_GrP5ScptIdNone != m_ParaUoTabBase.ScptIdStart)
		{
			TabPageSet(m_ParaUoTabBase.ScptIdStart,FALSE);
		}

		// inherited
		Cls_GrP5UoBase::RtlDoEvtScptDecDone();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTabBase::DoTabPageSet(__u16 A_ScptId, __u16 A_FocusId)
{
	// local -------------------
		__u32	Tv_Deced;
		void*	Tv_PtrScpt;
	// code --------------------
		// filter opposite mirror effect
		if(NULL != m_ObjWin->RtlUoFindByScptId(A_ScptId))
		{
			DbgMsgPrint("Cls_GrP5UoTabBase::DoTabPageSet - can not push same parent script.\n");
			return;
		}

		// notify close event
		if(NULL != m_WinChild)
		{
			((Cls_GrP5WinChild*)m_WinChild)->RtlDoEvtWinClose();
		}

		// delete child
		delete	m_WinChild;
		m_WinChild		=	NULL;
		m_PageStack[m_StackIdx].ScptId	=	E_GrP5ScptIdNone;
		m_PageStack[m_StackIdx].FocusId	=	E_GrP5ScptIdNone;

		// get script
		Tv_PtrScpt	=	GrP5ScriptGet(A_ScptId);
		if(NULL != Tv_PtrScpt)
		{
			// decode
			m_WinChild	=	(Cls_GrP5UoBase*)GrP5ScriptDecChild(Tv_PtrScpt, this, &Tv_Deced, &m_ParaUoTabBase.ContRc);
			if(NULL != m_WinChild)
			{
				// replace stack
				m_PageStack[m_StackIdx].ScptId	=	A_ScptId;
				// change focus
				if(E_GrP5ScptIdNone != A_FocusId)
				{
					Cls_GrP5UoBase*	Tv_ObjFcs;

					Tv_ObjFcs	=	RtlUoFindByScptId(A_FocusId);
					if(NULL != Tv_ObjFcs)
					{
						GrP5KeyFocusReq(Tv_ObjFcs->ObjIdGet(), 0);
					}
				}
				// do event
				DoEvtPageSet(A_ScptId);
			}
			else
			{
				DbgMsgPrint("Cls_GrP5UoTabBase::DoTabPageSet - script (%d) decode error!\n", A_ScptId);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP5UoTabBase::DoTabPageSet - script (%d) not found!", A_ScptId);
		}

		// redraw this control
		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoTabBase::TabNowPageScptIdGet(void)
{
	// local -------------------
	// code --------------------
		return	m_PageStack[m_StackIdx].ScptId;

}
//--------------------------------------------------------------------
void	Cls_GrP5UoTabBase::DoEvtPageSet(__u16 A_ScptId)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		// get event control
		Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctl)
		{
			Tv_Ctl->EvtPageDecDone(m_ParaUoBase.CtrlId, m_ObjId, A_ScptId);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTabBase::TabPagePop(void)
{
	// local -------------------
		St_GrP5MsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgUiMsg;
		Tv_Msg.Cmd				=	E_GrP5MsgCmdUiTabPagePop;

		GrP5MsgPost(&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTabBase::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if((E_GrP5KeyEsc == A_Key) || (E_GrP5KeyMenu == A_Key))
		{
			// check stack position
			if(0 != m_StackIdx)
			{
				if(QueryPageChange())
				{
					m_StackIdx --;
					DoTabPageSet(m_PageStack[m_StackIdx].ScptId, m_PageStack[m_StackIdx].FocusId);
				}
				Tv_Result	=	TRUE;
			}
		}

		if(!Tv_Result)
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyUp(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTabBase::EvtMseRbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check stack position
		if(0 != m_StackIdx)
		{
			if(QueryPageChange())
			{
				m_StackIdx --;
				DoTabPageSet(m_PageStack[m_StackIdx].ScptId, m_PageStack[m_StackIdx].FocusId);
			}
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTabBase::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTabBase::QueryPageChange(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		Tv_Result	=	TRUE;

		Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctl)
		{
			Tv_Result	=	Tv_Ctl->EvtPageQueryChange(m_ParaUoBase.CtrlId, m_ObjId);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoTabBase::ChildWinGet(void)
{
		return	m_WinChild;
}
//--------------------------------------------------------------------
