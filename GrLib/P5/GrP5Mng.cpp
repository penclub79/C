/*
	Platform 5

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5Mng.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5Key.h>
#include <P5/GrP5Script.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5WinBase.h>

#include <GrDumyTool.h>
#include <GrLayerData.h>


//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;
extern	St_GrP5SkinCfg	V_GrP5SkinCfg;

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5Mng::Cls_GrP5Mng(Cls_GrTaskSvr* A_TaskSvr) :
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		m_TskSvr	=	A_TaskSvr;

		//m_MsgAloc	=	GrTaskSvrMsgCmdAlloc();

		m_TskSvr->RegistMsgClient(this);
		m_TskSvr->RegistTimerClient(this, E_GrP5MngTimerPeriod, &m_TmrMain);

		m_PtrUiInfoBasic	=	NULL;

		m_MseHideTimer		=	0;
		m_AtLogoutTimer		=	0;
		m_AtLogoutExpire	=	0;

#if P5_DBLCLK
		m_IsLbtnDblClkChk	=	FALSE;
		m_LbtnDblClkTimer	=	0;
#endif

		GrDumyZeroMem(m_FontTbl, sizeof(m_FontTbl));
		GrDumyZeroMem(m_ImgScptTbl, sizeof(m_ImgScptTbl));
		GrDumyZeroMem(m_ImgUserTbl, sizeof(m_ImgUserTbl));
		GrDumyZeroMem(m_ScptWinTbl, sizeof(m_ScptWinTbl));

		GrDumyZeroMem(m_TxtFixTbl, sizeof(m_TxtFixTbl));
		GrDumyZeroMem(m_TxtUserTbl, sizeof(m_TxtUserTbl));
		
		GrDumyZeroMem(&m_Tooltip, sizeof(m_Tooltip));

}
//--------------------------------------------------------------------
Cls_GrP5Mng::~Cls_GrP5Mng(void)
{
	// local -------------------
	// code --------------------
		
		m_TskSvr->UnregistTimerClient(m_TmrMain);
		m_TskSvr->UnregistMsgClient(this);

		//GrTaskSvrMsgCmdFree(m_MsgAloc);

		FontFreeAll();

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Mng::RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Cmd;
		__u32	Tv_Type;
	// code --------------------
		Tv_Result	=	FALSE;

		// check message type
		if (E_GrTaskMsgUiMsg == A_PtrMsg->Cmd)
		{
			// UI message
			Tv_Cmd	=	((Ptr_GrP5MsgHd)A_PtrMsg)->Cmd;
			Tv_Type	=	Mac_GrP5MsgTypeGet(Tv_Cmd);
			// check message type
			if(E_GrP5MsgCmdTypeCon == Tv_Type)
			{
				Tv_Result	=	ProcMsgTypeCon((Ptr_GrP5MsgConHd)A_PtrMsg);
			}
			else if(E_GrP5MsgCmdTypeUiObj == Tv_Type)
			{
				Tv_Result	=	ProcMsgTypeUi((Ptr_GrP5MsgUiHd)A_PtrMsg);
			}
			else
			{
				Tv_Result	=	TRUE;
				switch(Tv_Cmd)
				{
					case E_GrP5MsgCmdConRegist:
						ProcMsgConRegist((Ptr_GrP5MsgConRegist)A_PtrMsg);
						break;
					case E_GrP5MsgCmdConUnregist:
						ProcMsgConUnregist((Ptr_GrP5MsgConUnregist)A_PtrMsg);
						break;
					case E_GrP5MsgCmdInpKey:
						ProcMsgInpKey((Ptr_GrP5MsgInpKey)A_PtrMsg);
						break;
					case E_GrP5MsgCmdInpMsePosRel:
						ProcMsgInpMsePosRel((Ptr_GrP5MsgInpMsePosRel)A_PtrMsg);
						break;
					case E_GrP5MsgCmdInpMsePosAbs:
						ProcMsgInpMsePosAbs((Ptr_GrP5MsgInpMsePosAbs)A_PtrMsg);
						break;
					case E_GrP5MsgCmdInpMseBtn:
						ProcMsgInpMseBtn((Ptr_GrP5MsgInpMseBtn)A_PtrMsg);
						break;
					case E_GrP5MsgCmdInpMseWheel:
						ProcMsgInpMseWheel((Ptr_GrP5MsgInpMseWheel)A_PtrMsg);
						break;
					case E_GrP5MsgCmdPntCodeBld:
						*((Ptr_GrP5MsgPntCodeBld)A_PtrMsg)->PtrRtRslt	=
							ImmPntCodeBld(((Ptr_GrP5MsgPntCodeBld)A_PtrMsg)->ConIdx);
						break;
					case E_GrP5MsgCmdKeyFcsUoSet:
						ProcMsgKeyFocusUoSet((Ptr_GrP5MsgKeyFcsUoSet)A_PtrMsg);
						break;
					case E_GrP5MsgCmdKeyFcsCorrect:
						GrP5KeyFocusCorrectDo();
						break;
					case E_GrP5MsgCmdAutoLogoutSet:
						ProcMsgAtLogoutSet((Ptr_GrP5MsgAtLogoutSet)A_PtrMsg);
						break;
					default:
						Tv_Result	=	FALSE;
						break;
				}
			}
		}
		else if(E_GrTaskMsgQuadChgDone == A_PtrMsg->Cmd)
		{
			// do change quad
			ProcMsgQuadChanged((Ptr_GrTaskMsgQuadChgDone)A_PtrMsg);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::RtlTimer(__u32 A_Hndl)
{
	// local -------------------
		
	// code --------------------
		// do timer input
		ProcTimerInput();
		// do control timer
		ProcTimerCtrl();
		// do window timer
		ProcTimerWin();
		// do object timer
		ProcTimerObj();
		// do console timer
		ProcTimerCon();
		// do tooltip
		LcTimerTooltip();
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcTimerCon(void)
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------
		for( Tv_ConIdx = 0; Tv_ConIdx < E_GrP5ConMaxCnt; Tv_ConIdx++)
		{
			if(NULL != V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)
			{
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)->RtlTimer();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::InputMsePosRel(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		St_GrP5MsgInpMsePosRel	Tv_Msg;
	// code --------------------
		Tv_Msg.CmdHd.Cmd	=	E_GrP5MsgCmdInpMsePosRel;
		Tv_Msg.X					=	A_X;
		Tv_Msg.Y					=	A_Y;

#ifdef GR_TASKMSG_DEBUG
		if(!UiMsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg)))
		{
			DbgMsgPrint("[DBG] %s MsgPost fail!\n", __FUNCTION__);
		}
#else
		UiMsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
#endif

}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::InputMsePosAbs(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		St_GrP5MsgInpMsePosAbs	Tv_Msg;
	// code --------------------
		Tv_Msg.CmdHd.Cmd	=	E_GrP5MsgCmdInpMsePosAbs;
		Tv_Msg.X					=	A_X;
		Tv_Msg.Y					=	A_Y;

#ifdef GR_TASKMSG_DEBUG
		if(!UiMsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg)))
		{
			DbgMsgPrint("[DBG] %s MsgPost fail!\n", __FUNCTION__);
		}
#else
		UiMsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
#endif

		
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::InputMseBtn(__u8 A_BtnIdx, BOOL8 A_IsPush)
{
	// local -------------------
		St_GrP5MsgInpMseBtn	Tv_Msg;
	// code --------------------
		Tv_Msg.CmdHd.Cmd	=	E_GrP5MsgCmdInpMseBtn;
		Tv_Msg.BtnIdx			=	A_BtnIdx;
		Tv_Msg.IsPush			=	A_IsPush;

#ifdef GR_TASKMSG_DEBUG
		if(!UiMsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg)))
		{
			DbgMsgPrint("[DBG] %s MsgPost fail!\n", __FUNCTION__);
		}
#else
		UiMsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
#endif
				
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::InputMseWheel(__u8 A_WheelIdx, BOOL8 A_IsDown)
{
	// local -------------------
		St_GrP5MsgInpMseWheel	Tv_Msg;
	// code --------------------
		Tv_Msg.CmdHd.Cmd	=	E_GrP5MsgCmdInpMseWheel;
		Tv_Msg.WheelIdx		=	A_WheelIdx;
		Tv_Msg.IsDown			=	A_IsDown;

#ifdef GR_TASKMSG_DEBUG
		if(!UiMsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg)))
		{
			DbgMsgPrint("[DBG] %s MsgPost fail!\n", __FUNCTION__);
		}
#else
		MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
#endif
				
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::InputKey(__u8 A_DevId, __u8 A_Key, BOOL8 A_IsPush)
{
	// local -------------------
		St_GrP5MsgInpKey	Tv_Msg;
	// code --------------------
		Tv_Msg.CmdHd.Cmd	=	E_GrP5MsgCmdInpKey;
		Tv_Msg.DevId			=	A_DevId;
		Tv_Msg.Key				=	A_Key;
		Tv_Msg.IsPush			=	A_IsPush;

#ifdef GR_TASKMSG_DEBUG
		if(!UiMsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg)))
		{
			DbgMsgPrint("[DBG] %s MsgPost fail!\n", __FUNCTION__);
		}
#else
		UiMsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
#endif
				
}
//--------------------------------------------------------------------
void*	Cls_GrP5Mng::ScriptGet(__u16 A_ScptId)
{
	// local -------------------
		void*	Tv_Result;
		__u16	Tv_ScpIdx;
	// code --------------------
		Tv_Result	=	NULL;

		// check window
		if((E_GrP5ScptIdWinBase <= A_ScptId) && ((E_GrP5ScptIdWinBase + E_GrP5ScptWinMaxCnt) > A_ScptId))
		{
			// window
			Tv_ScpIdx	=	A_ScptId - E_GrP5ScptIdWinBase;
			Tv_Result	=	m_ScptWinTbl[Tv_ScpIdx];
		}
		else
		{
			// unknown
			DbgMsgPrint("Cls_GrP5Mng::ScriptGet - bad script id (%d)\n", A_ScptId);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Mng::ProcMsgTypeCon(Ptr_GrP5MsgConHd A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		// init
		Tv_Result		=	FALSE;
		// go current console
		if(E_GrP5ConMaxCnt > A_PtrMsg->ConIdx)
		{
			// check exist console
			if(NULL != V_GrP5Env.ConTbl[A_PtrMsg->ConIdx].ObjCon)
			{
				// do message
				Tv_Result	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_PtrMsg->ConIdx].ObjCon)->RtlMsgProc((Ptr_GrP5MsgHd)A_PtrMsg);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Mng::ProcMsgTypeUi(Ptr_GrP5MsgUiHd A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5UoBase*	Tv_Uo;
	// code --------------------
		Tv_Result	=	FALSE;

		// find object
		Tv_Uo	=	(Cls_GrP5UoBase*)GrP5UiObjGet(A_PtrMsg->ObjId);
		if(NULL != Tv_Uo)
		{
			Tv_Result	=	Tv_Uo->RtlProcUiMsg(A_PtrMsg);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Mng::UiMsgPost(Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_MsgSize)
{
	// local -------------------
	// code --------------------
		A_PtrMsg->Cmd		=	E_GrTaskMsgUiMsg;
		return	m_TskSvr->MsgPost(A_PtrMsg, A_MsgSize);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Mng::UiMsgSend(Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_MsgSize)
{
		A_PtrMsg->Cmd		=	E_GrTaskMsgUiMsg;
		return	m_TskSvr->MsgSend(A_PtrMsg, A_MsgSize);
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcMsgConRegist(Ptr_GrP5MsgConRegist A_PtrMsg)
{
	// local -------------------
	// code --------------------
		if(NULL != A_PtrMsg->PtrRslt )
		{
			*A_PtrMsg->PtrRslt		=	FALSE;
		}
		// check console range
		if(E_GrP5ConMaxCnt > A_PtrMsg->ConIdx)
		{
			// check already exist
			if(NULL == V_GrP5Env.ConTbl[A_PtrMsg->ConIdx].ObjCon)
			{
				// regist
				V_GrP5Env.ConTbl[A_PtrMsg->ConIdx].ObjCon	=	A_PtrMsg->ObjCon;
				V_GrP5Env.ConTbl[A_PtrMsg->ConIdx].ConIdx	=	A_PtrMsg->ConIdx;
				// success
				if(NULL != A_PtrMsg->PtrRslt)
				{
					*A_PtrMsg->PtrRslt		=	TRUE;
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcMsgConUnregist(Ptr_GrP5MsgConUnregist A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_Rslt;
	// code --------------------
		Tv_Rslt		=	FALSE;

		if(E_GrP5ConMaxCnt > A_PtrMsg->ConIdx)
		{
			// check exist
			if(NULL != V_GrP5Env.ConTbl[A_PtrMsg->ConIdx].ObjCon)
			{
				// remove
				V_GrP5Env.ConTbl[A_PtrMsg->ConIdx].ObjCon	=	NULL;
				GrDumyZeroMem(V_GrP5Env.ConTbl[A_PtrMsg->ConIdx].WinTbl, sizeof(St_GrP5UoCtrl)* E_GrP5LayerMaxCnt * E_GrP5WinCntForLayer);
				GrDumyZeroMem(V_GrP5Env.ConTbl[A_PtrMsg->ConIdx].SlaveObjTbl, sizeof(St_GrP5UoCtrl)* E_GrP5SlaveObjMaxCnt);

				Tv_Rslt		=	TRUE;
			}
		}

		if(NULL != A_PtrMsg->PtrRslt)
		{
			*A_PtrMsg->PtrRslt		=	Tv_Rslt;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcMsgInpKey(Ptr_GrP5MsgInpKey A_PtrMsg)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoFcs;
		__u32	Tv_KeyMask;
		__u8	Tv_KtbIdx;
		__u8	Tv_ConIdx;
	// code --------------------
		// mouse cursor off
		MseCursorOnSet(FALSE);
		
		// auto logout
		m_AtLogoutTimer	=	0;

		// check able
		if(E_GrP5KeyNone != A_PtrMsg->Key)
		{
			// get focus object
			Tv_UoFcs	=	(Cls_GrP5UoBase*)GrP5KeyFocusUiObjGet();
			if(NULL != Tv_UoFcs)
			{
				// console event
				Tv_ConIdx	=	GrP5ConIdxByObjId(V_GrP5Env.KeyObjId);
				if(NULL != V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)
				{
					if(((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)->EvtKeyInput(A_PtrMsg->DevId, A_PtrMsg->Key, A_PtrMsg->IsPush))
					{
						Tv_KtbIdx		=	A_PtrMsg->Key >> 5;
						Tv_KeyMask	=	1 << (A_PtrMsg->Key & 0x1F);
						if(A_PtrMsg->IsPush)
						{
							// push
							// update key map
							V_GrP5Env.KeyMap[Tv_KtbIdx]	=	V_GrP5Env.KeyMap[Tv_KtbIdx] | Tv_KeyMask;

							// call event
							while(NULL != Tv_UoFcs)
							{
								// key event
								if(Tv_UoFcs->EvtKeyDown(A_PtrMsg->Key))
								{
									break;
								}
								// next
								Tv_UoFcs	=	Tv_UoFcs->ParentObjGet();
							}
						}
						else
						{
							// release
							// update key map
							V_GrP5Env.KeyMap[Tv_KtbIdx]	=	V_GrP5Env.KeyMap[Tv_KtbIdx] & (~Tv_KeyMask);

							// call event
							while(NULL != Tv_UoFcs)
							{
								// key event
								if(Tv_UoFcs->EvtKeyUp(A_PtrMsg->Key))
								{
									break;
								}
								// next
								Tv_UoFcs	=	Tv_UoFcs->ParentObjGet();
							}
						}

					}
				}

			}

#if P5_DBLCLK
			// release dobuleclick mode 
			m_IsLbtnDblClkChk	=	FALSE;
			m_LbtnDblClkTimer	=	0;
#endif

		}
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcPosMse(__s32 A_X, __s32 A_Y)
{
	// local -------------------
#if 0
		BOOL8	Tv_IsChg;
		__u8	Tv_ChgIdx;
		BOOL8	Tv_IsCanChg;
#endif
		St_GrRect	Tv_RcScr;
		__s32	Tv_X;
		__s32	Tv_Y;
	// code --------------------
		// auto logout
		m_AtLogoutTimer	=	0;

		// cursor on
		MseCursorOnSet(TRUE);

		Tv_X				=	A_X;
		Tv_Y				=	A_Y;

#if 1
		if(NULL == V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)
		{
			return;
		}

		// get screen rect
		((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)->ScrRectGet(&Tv_RcScr);
		// check out of range
		if(Tv_X <= Tv_RcScr.left)
		{
			Tv_X		=	Tv_RcScr.left;
		}
		if(Tv_X >= Tv_RcScr.right)
		{
			Tv_X	=	Tv_RcScr.right - 1;
		}
		if (Tv_Y <= Tv_RcScr.top)
		{
			Tv_Y	=	Tv_RcScr.top;
		}
		if(Tv_Y >= Tv_RcScr.bottom)
		{
			Tv_Y	=	Tv_RcScr.bottom - 1;
		}

		// update position
		V_GrP5Env.MseX		=	Tv_X;
		V_GrP5Env.MseY		=	Tv_Y;

		// notify mouse move event
		((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)->RtlMseMove();

#else
		Tv_IsChg		=	FALSE;
		Tv_IsCanChg	=	TRUE;
		Tv_ChgIdx		=	V_GrP5Env.MseConIdx ^ 1;
		// check out of range
		if(NULL != V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)
		{
			// get can leave
			Tv_IsCanChg	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)->IsCanLeave();
			// get screen rect
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)->ScrRectGet(&Tv_RcScr);
			// check out of range
			if(Tv_X < Tv_RcScr.left)
			{
				Tv_X		=	Tv_RcScr.left;

				if(Tv_IsCanChg)
				{
					if(((E_GrP5ConExpLeft == V_GrP5Env.ConExpType) && (0 == V_GrP5Env.MseConIdx)) ||
						((E_GrP5ConExpRight == V_GrP5Env.ConExpType) && (1 == V_GrP5Env.MseConIdx))
						)
					{
						if(NULL != V_GrP5Env.ConTbl[Tv_ChgIdx].ObjCon)
						{
							// change console device
							Tv_IsChg	=	TRUE;
							((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ChgIdx].ObjCon)->ScrRectGet(&Tv_RcScr);
							Tv_X		=	Tv_RcScr.right - 1;
						}
					}
				}

			}
			if(Tv_X >= Tv_RcScr.right)
			{
				Tv_X	=	Tv_RcScr.right - 1;

				if(Tv_IsCanChg)
				{
					if(((E_GrP5ConExpRight == V_GrP5Env.ConExpType) && (0 == V_GrP5Env.MseConIdx)) ||
						((E_GrP5ConExpLeft == V_GrP5Env.ConExpType) && (1 == V_GrP5Env.MseConIdx))
						)
					{
						if(NULL != V_GrP5Env.ConTbl[Tv_ChgIdx].ObjCon)
						{
							// change console device
							Tv_IsChg	=	TRUE;
							((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ChgIdx].ObjCon)->ScrRectGet(&Tv_RcScr);
							Tv_X		=	Tv_RcScr.left;
						}
					}
				}
			}
			if(Tv_Y < Tv_RcScr.top)
			{
				Tv_Y	=	Tv_RcScr.top;

				if(Tv_IsCanChg)
				{
					if(((E_GrP5ConExpUp == V_GrP5Env.ConExpType) && (0 == V_GrP5Env.MseConIdx)) ||
						((E_GrP5ConExpDown == V_GrP5Env.ConExpType) && (1 == V_GrP5Env.MseConIdx))
						)
					{
						if(NULL != V_GrP5Env.ConTbl[Tv_ChgIdx].ObjCon)
						{
							// change console device
							Tv_IsChg	=	TRUE;
							((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ChgIdx].ObjCon)->ScrRectGet(&Tv_RcScr);
							Tv_X		=	Tv_RcScr.bottom - 1;
						}
					}
				}
			}
			if(Tv_Y >= Tv_RcScr.bottom)
			{
				Tv_Y	=	Tv_RcScr.bottom - 1;

				if(Tv_IsCanChg)
				{
					if(((E_GrP5ConExpDown == V_GrP5Env.ConExpType) && (0 == V_GrP5Env.MseConIdx)) ||
						((E_GrP5ConExpUp == V_GrP5Env.ConExpType) && (1 == V_GrP5Env.MseConIdx))
						)
					{
						if(NULL != V_GrP5Env.ConTbl[Tv_ChgIdx].ObjCon)
						{
							// change console device
							Tv_IsChg	=	TRUE;
							((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ChgIdx].ObjCon)->ScrRectGet(&Tv_RcScr);
							Tv_X		=	Tv_RcScr.top;
						}
					}
				}
			}

			// update position
			V_GrP5Env.MseX		=	Tv_X;
			V_GrP5Env.MseY		=	Tv_Y;

			// check change
#if 0
			// not need - only desktop extend mode
			if(Tv_IsChg)
			{
				// update
				V_GrP5Env.MseConIdx	=	Tv_ChgIdx;
				// notify leave event
				Tv_ChgIdx	=	Tv_ChgIdx ^ 1;
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ChgIdx].ObjCon)->RtlMseLeave();
				// notify enter event
				Tv_ChgIdx	=	Tv_ChgIdx ^ 1;
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[Tv_ChgIdx].ObjCon)->RtlMseEnter();
			}
#endif

			// notify mouse move event
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)->RtlMseMove();

		}
		else
		{
			// console status bad
			DbgMsgPrint("[BUG!] Cls_GrP5Mng::ProcPosMse - console %d not exist\n", (int)V_GrP5Env.MseConIdx);
		}
#endif

}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcMsgInpMsePosRel(Ptr_GrP5MsgInpMsePosRel A_PtrMsg)
{
	// local -------------------
	// code --------------------
		ProcPosMse(V_GrP5Env.MseX + A_PtrMsg->X, V_GrP5Env.MseY + A_PtrMsg->Y);
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcMsgInpMsePosAbs(Ptr_GrP5MsgInpMsePosAbs A_PtrMsg)
{
	// local -------------------
	// code --------------------
		ProcPosMse(A_PtrMsg->X, A_PtrMsg->Y);
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcMsgInpMseBtn(Ptr_GrP5MsgInpMseBtn A_PtrMsg)
{
	// local -------------------
		Cls_GrP5ConBase*	Tv_ObjCon;
		BOOL8	Tv_IsDblClk;
	// code --------------------
		m_AtLogoutTimer	=	0;
		MseCursorOnSet(TRUE);

		// get console
		Tv_ObjCon	=	(Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon;
		if(NULL != Tv_ObjCon)
		{
			// call event
			if(A_PtrMsg->IsPush)
			{
				// push
				Tv_IsDblClk	=	FALSE;
				switch(A_PtrMsg->BtnIdx)
				{
					case 0:
#if P5_DBLCLK
						if(m_IsLbtnDblClkChk)
						{
							// check timer
							if (E_GrP5MngDblClkTimeCnt > m_LbtnDblClkTimer)
							{
								Tv_IsDblClk	=	TRUE;
							}
						}

						if (Tv_IsDblClk)
						{
							m_IsLbtnDblClkChk	=	FALSE;
							m_LbtnDblClkTimer	=	0;
							Tv_ObjCon->RtlMseLbtnDblclk();
						}
						else
						{
							m_IsLbtnDblClkChk	=	TRUE;
							m_LbtnDblClkTimer	=	0;
							Tv_ObjCon->RtlMseLbtnDown();
						}
#else
						Tv_ObjCon->RtlMseLbtnDown();
#endif
						break;
					case 1:
#if P5_DBLCLK
						m_IsLbtnDblClkChk	=	FALSE;
						m_LbtnDblClkTimer	=	0;
#endif
						Tv_ObjCon->RtlMseRbtnDown();
						break;
					case 3:
#if P5_DBLCLK
						m_IsLbtnDblClkChk	=	FALSE;
						m_LbtnDblClkTimer	=	0;
#endif
						Tv_ObjCon->RtlMseMbtnDown();
						break;
#ifdef E_GrP5DbgMsgWarnning
					default:
						DbgMsgPrint("[WARNNING] Cls_GrP5Mng::ProcMsgInpMseBtn - button %d not support\n", (int)A_PtrMsg->BtnIdx);
						break;
#endif
				}
			}
			else
			{
				// release
				switch(A_PtrMsg->BtnIdx)
				{
					case 0:
						Tv_ObjCon->RtlMseLbtnUp();
						break;
					case 1:
#if P5_DBLCLK
						m_IsLbtnDblClkChk	=	FALSE;
						m_LbtnDblClkTimer	=	0;
#endif
						Tv_ObjCon->RtlMseRbtnUp();
						break;
					case 3:
#if P5_DBLCLK
						m_IsLbtnDblClkChk	=	FALSE;
						m_LbtnDblClkTimer	=	0;
#endif
						Tv_ObjCon->RtlMseMbtnUp();
						break;
#ifdef E_GrP5DbgMsgWarnning
					default:
						DbgMsgPrint("[WARNNING] Cls_GrP5Mng::ProcMsgInpMseBtn - button %d not support\n", (int)A_PtrMsg->BtnIdx);
						break;
#endif
				}
			}
		}
		else
		{
			DbgMsgPrint("[BUG!] Cls_GrP5Mng::ProcMsgInpMseBtn - console %d not exist.\n", (int)V_GrP5Env.MseConIdx);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcMsgInpMseWheel(Ptr_GrP5MsgInpMseWheel A_PtrMsg)
{
	// local -------------------
		Cls_GrP5ConBase*	Tv_ObjCon;
	// code --------------------
		m_AtLogoutTimer	=	0;
		MseCursorOnSet(TRUE);
		// get console
		Tv_ObjCon	=	(Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon;
		if(NULL != Tv_ObjCon)
		{
			if(A_PtrMsg->IsDown)
			{
				Tv_ObjCon->RtlMseWheelDown(A_PtrMsg->WheelIdx);
			}
			else
			{
				Tv_ObjCon->RtlMseWheelUp(A_PtrMsg->WheelIdx);
			}
		}

#if P5_DBLCLK
		// release double click mode
		m_IsLbtnDblClkChk	=	FALSE;
		m_LbtnDblClkTimer	=	0;
#endif

}
//--------------------------------------------------------------------
__u32	Cls_GrP5Mng::PixelFormatGet(void)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if (NULL != m_PtrUiInfoBasic)
		{
			Tv_Result	=	m_PtrUiInfoBasic->FccScr;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP5Font*	Cls_GrP5Mng::FontGet(__u8 A_FontId)
{
	// local -------------------
		Cls_GrFontV3DrawBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if(E_GrP5ScptFontMaxCnt > A_FontId)
		{
			Tv_Result	=	m_FontTbl[A_FontId];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::FontFreeAll(void)
{
	// local -------------------
		__u8	Tv_FntIdx;
	// code --------------------
		for( Tv_FntIdx = 0; Tv_FntIdx < E_GrP5ScptFontMaxCnt; Tv_FntIdx++)
		{
			if(NULL != m_FontTbl[Tv_FntIdx])
			{
				delete	m_FontTbl[Tv_FntIdx];
				m_FontTbl[Tv_FntIdx]	=	NULL;
			}
		}
}
//--------------------------------------------------------------------
Ptr_GrGdib	Cls_GrP5Mng::ImageGet(__u16 A_ImgId)
{
	// local -------------------
		Ptr_GrGdib	Tv_Result;
		__u16	Tv_ImgIdx;
	// code --------------------
		Tv_Result	=	NULL;

		if ((E_GrP5ImgIdBase <= A_ImgId) && ((E_GrP5ImgIdBase + E_GrP5ScptImgMaxCnt) > A_ImgId))
		{
			Tv_ImgIdx	=	A_ImgId - E_GrP5ImgIdBase;
			Tv_Result	=	m_ImgScptTbl[Tv_ImgIdx];
		}
		else if ((E_GrP5ImgUserIdBase <= A_ImgId) && ((E_GrP5ImgUserIdBase + E_GrP5ImgUserMaxCnt) > A_ImgId))
		{
			Tv_ImgIdx	=	A_ImgId - E_GrP5ImgUserIdBase;
			Tv_Result	=	m_ImgUserTbl[Tv_ImgIdx];
		}
#if 0
		else
		{
			DbgMsgPrint("Cls_GrP5Mng::ImageGet - bad image id (%d) requirement\n", A_ImgId);
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Mng::ImageUserSet(__u16 A_ImgId, Ptr_GrGdib A_PtrGdib)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_ImgIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ((E_GrP5ImgUserIdBase <= A_ImgId) && ((E_GrP5ImgUserIdBase + E_GrP5ImgUserMaxCnt) > A_ImgId))
		{
			Tv_ImgIdx	=	A_ImgId - E_GrP5ImgUserIdBase;
			m_ImgUserTbl[Tv_ImgIdx]	=	A_PtrGdib;
			Tv_Result	=	TRUE;
		}
		else
		{
			DbgMsgPrint("Cls_GrP5Mng::ImageUserSet - bad image id (%d) requirement\n", A_ImgId);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP5Mng::TextGet(__u16 A_TxtId, __u8 A_LanguageId)
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u8		Tv_LngId;
		__u16		Tv_TxtIdx;
	// code --------------------
		Tv_Result	=	NULL;
		
		Tv_LngId	=	A_LanguageId;
		if (E_GrP5ScptLanguageMaxCnt <= Tv_LngId)
		{
			Tv_LngId	=	V_GrP5Env.Language;
			if (E_GrP5ScptLanguageMaxCnt <= Tv_LngId)
			{
				DbgMsgPrint("Cls_GrP5Mng::TextGet - bad langulage id (%d)\n", Tv_LngId);
				Tv_LngId	=	0;
			}
		}

		// check text id
		if ((E_GrP5TxtIdFixBase <= A_TxtId) && ((E_GrP5TxtIdFixBase + E_GrP5TxtDedicatedCnt) > A_TxtId))
		{
			// deidicated text
			Tv_TxtIdx	=	A_TxtId - E_GrP5TxtIdFixBase;
			Tv_Result	=	m_TxtFixTbl[Tv_LngId][Tv_TxtIdx];
		}
		else if ((E_GrP5TxtIdUserBase <= A_TxtId) && ((E_GrP5TxtIdUserBase + E_GrP5ScptTxtUserMaxCnt) > A_TxtId))
		{
			// user text
			Tv_TxtIdx	=	A_TxtId - E_GrP5TxtIdUserBase;
			Tv_Result	=	m_TxtUserTbl[Tv_LngId][Tv_TxtIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcMsgKeyFocusUoSet(Ptr_GrP5MsgKeyFcsUoSet A_PtrMsg)
{
	// local -------------------
	// code --------------------
		GrP5KeyFocusSet(A_PtrMsg->ObjId,A_PtrMsg->Para);
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::Reset(void)
{
	// local -------------------
		
	// code --------------------
		// release font
		FontFreeAll();

		// reset table
		m_PtrUiInfoBasic	=	NULL;

		m_PtrVkbdCfg			=	NULL;

		GrDumyZeroMem(m_ImgScptTbl, sizeof(m_ImgScptTbl));
		GrDumyZeroMem(m_ImgUserTbl, sizeof(m_ImgUserTbl));
		GrDumyZeroMem(m_ScptWinTbl, sizeof(m_ScptWinTbl));

		GrDumyZeroMem(m_TxtFixTbl, sizeof(m_TxtFixTbl));
		GrDumyZeroMem(m_TxtUserTbl, sizeof(m_TxtUserTbl));

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Mng::ExcScriptSet(void* A_PtrExc)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_DataId;
		__u32	Tv_Size;
		__u32	Tv_Para0;
		__u32	Tv_Para1;
		St_GrLayerDataParsRtl	Tv_Pars;
		void*	Tv_PtrData;
	// code --------------------
		Tv_Result	=	FALSE;

		// reset script
		Reset();

		if (GrLayerDataParsInit(&Tv_Pars,A_PtrExc))
		{
			m_PtrUiInfoBasic	=	(Ptr_GrP5ScptUiInfoBasic)GrLayerDataParsGetUserInfo(&Tv_Pars, &Tv_Size);
			if ((NULL != m_PtrUiInfoBasic) && (8 <= Tv_Size))
			{
				if (E_GrP5ScriptFcc == m_PtrUiInfoBasic->FccScpt)
				{
					Tv_PtrData	=	GrLayerDataParsDo(&Tv_Pars, &Tv_DataId, &Tv_Size, &Tv_Para0, &Tv_Para1);
					while (NULL != Tv_PtrData)
					{
						switch (Tv_DataId)
						{
							case E_GrP5DataIdFont:
								ExcFontSet(Tv_Para0, Tv_PtrData, Tv_Size);
								break;
							case E_GrP5DataIdImage:
								ExcImageSet(Tv_Para0, Tv_PtrData, Tv_Size);
								break;
							case E_GrP5DataIdWinScript:
								ExcWinScptSet(Tv_Para0, Tv_PtrData, Tv_Size);
								Tv_Result	=	TRUE;
								break;
							case E_GrP5DataIdSkinCfg:
								ExcSkinCfgSet(Tv_PtrData, Tv_Size);
								break;
							case  E_GrP5DataIdText:
								ExcTextSet(Tv_Para0, Tv_Para1, Tv_PtrData, Tv_Size);
								break;
							case E_GrP5DataIdVkbdCfg:
								ExcVkbdCfg(Tv_PtrData);
								break;
							default:
								// unknown data
								DbgMsgPrint("Cls_GrP5Mng::ExcScriptSet - unknown data id (%d) found.so skip.\n",Tv_DataId);
								break;
						}
						// next
						Tv_PtrData	=	GrLayerDataParsDo(&Tv_Pars, &Tv_DataId, &Tv_Size, &Tv_Para0, &Tv_Para1);
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP5Mng::ExcScriptSet - bad script basic info!\n");
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP5Mng::ExcScriptSet - script basic info read fail!\n");
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP5Mng::ExcScriptSet - parser initialize fail!\n");
		}

		if (Tv_Result)
		{
			DbgMsgPrint("Cls_GrP5Mng::ExcScriptSet - success!\n");
		}
		else
		{
			// reset
			Reset();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ExcFontSet(__u32 A_FontId, void* A_PtrData, __u32 A_DataSize)
{
	// local -------------------
	// code --------------------
		// check font id range
		if (E_GrP5ScptFontMaxCnt > A_FontId)
		{
			// release old font
			if (NULL != m_FontTbl[A_FontId])
			{
				DbgMsgPrint("Cls_GrP5Mng::ExcFontSet - font id (%d) overlapped\n", A_FontId);
				delete	m_FontTbl[A_FontId];
				m_FontTbl[A_FontId]	=	NULL;
			}

			// create new font
			m_FontTbl[A_FontId]	=	GrFontV3DrawCreateByScript(A_PtrData, A_DataSize, FALSE);
			if (NULL == m_FontTbl[A_FontId])
			{
				DbgMsgPrint("Cls_GrP5Mng::ExcFontSet - bad font (%d) script\n", A_FontId);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP5Mng::ExcFontSet - bad font id (%d)\n", A_FontId);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ExcImageSet(__u32 A_ImgId, void* A_PtrData, __u32 A_DataSize)
{
	// local -------------------
		__u32	Tv_Idx;
	// code --------------------
		// check image id range
		if ((E_GrP5ImgIdBase <= A_ImgId) && ((E_GrP5ImgIdBase + E_GrP5ScptImgMaxCnt) > A_ImgId))
		{
			Tv_Idx	=	A_ImgId - E_GrP5ImgIdBase;
			if (NULL != m_ImgScptTbl[Tv_Idx])
			{
				DbgMsgPrint("Cls_GrP5Mng::ExcImageSet - image id (%d) overlapped\n", A_ImgId);
				m_ImgScptTbl[Tv_Idx]	=	NULL;
			}

			if (E_GrGdibFcc == ((Ptr_GrGdib)A_PtrData)->FccGdib)
			{
				m_ImgScptTbl[Tv_Idx]	=	(Ptr_GrGdib)A_PtrData;
			}
			else
			{
				DbgMsgPrint("Cls_GrP5Mng::ExcImageSet - bad image (%d) data\n", A_ImgId);
			}
			
		}
		else
		{
			DbgMsgPrint("Cls_GrP5Mng::ExcImageSet - bad image id (%d)\n", A_ImgId);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ExcWinScptSet(__u32 A_ScptId, void* A_PtrData, __u32 A_DataSize)
{
	// local -------------------
		__u32	Tv_Idx;
	// code --------------------
		// check id range
		if ((E_GrP5ScptIdWinBase <= A_ScptId) && ((E_GrP5ScptIdWinBase + E_GrP5ScptWinMaxCnt) > A_ScptId))
		{
			Tv_Idx	=	A_ScptId - E_GrP5ScptIdWinBase;
			if (NULL != m_ScptWinTbl[Tv_Idx])
			{
				DbgMsgPrint("Cls_GrP5Mng::ExcWinScptSet - window id (%d) overlapped\n", A_ScptId);
				m_ScptWinTbl[Tv_Idx]	=	NULL;
			}

			if (E_GrP5ScptObjHdTcc == ((Ptr_GrP5ScptObjHead)A_PtrData)->Tcc)
			{
				m_ScptWinTbl[Tv_Idx]	=	(Ptr_GrP5ScptObjHead)A_PtrData;
			}
			else
			{
				DbgMsgPrint("Cls_GrP5Mng::ExcWinScptSet - bad window (%d) script data\n", A_ScptId);
			}

		}
		else
		{
			DbgMsgPrint("Cls_GrP5Mng::ExcWinScptSet - bad window id (%d)\n", A_ScptId);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ExcSkinCfgSet(void* A_PtrData, __u32 A_DataSize)
{
	// local -------------------
		__u32	Tv_CpSize;
	// code --------------------
		Tv_CpSize	=	A_DataSize;
		if (Tv_CpSize >= sizeof(V_GrP5SkinCfg))
		{
			Tv_CpSize	=	sizeof(V_GrP5SkinCfg);
		}
		GrDumyCopyMem(&V_GrP5SkinCfg, A_PtrData, Tv_CpSize);

		// rebuild script
		GrP5SkinSetByCfg();
		
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ExcTextSet(__u32 A_TxtId, __u32 A_Language, void* A_PtrData, __u32 A_DataSize)
{
	// local -------------------
		__u32	Tv_Idx;
	// code --------------------
		// check language range
		if (E_GrP5ScptLanguageMaxCnt > A_Language)
		{
			// check id range
			if ((E_GrP5TxtIdFixBase <= A_TxtId) && ((E_GrP5TxtIdFixBase + E_GrP5TxtDedicatedCnt) > A_TxtId))
			{
				Tv_Idx	=	A_TxtId - E_GrP5TxtIdFixBase;
				m_TxtFixTbl[A_Language][Tv_Idx]	=	(WCHAR*)A_PtrData;
			}
			else if ((E_GrP5TxtIdUserBase <= A_TxtId) && ((E_GrP5TxtIdUserBase + E_GrP5ScptTxtUserMaxCnt) > A_TxtId))
			{
				Tv_Idx	=	A_TxtId - E_GrP5TxtIdUserBase;
				m_TxtUserTbl[A_Language][Tv_Idx]	=	(WCHAR*)A_PtrData;
			}
			else
			{
				DbgMsgPrint("Cls_GrP5Mng::ExcTextSet - bad text id (%d)\n", A_TxtId);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP5Mng::ExcTextSet - bad text (%d) language (%d)\n", A_TxtId,A_Language);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ExcVkbdCfg(void* A_PtrData)
{
	// local -------------------
	// code --------------------
		m_PtrVkbdCfg		=	(Ptr_GrP5VkbdCfg)A_PtrData;
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcTimerInput(void)
{
	// local -------------------
		St_GrTaskMsgHd	Tv_Msg;
	// code --------------------

		// auto hide
		if(V_GrP5Env.IsMseCusOn)
		{
			if (E_GrP5MngMseHideTimeCnt < m_MseHideTimer)
			{
				// mouse cursor off
				MseCursorOnSet(FALSE);
			}
			else
			{
				m_MseHideTimer ++;
			}
		}
		else
		{
			m_MseHideTimer	=	0;
		}

#if P5_DBLCLK
		// check mouse doble click
		if (m_IsLbtnDblClkChk)
		{
			m_LbtnDblClkTimer ++;
		}
#endif

		// auto logout
		if(0 != m_AtLogoutExpire)
		{
			m_AtLogoutTimer ++;
			if(m_AtLogoutTimer >= m_AtLogoutExpire)
			{
				// broadcast message
				Tv_Msg.Cmd	=	E_GrTaskMsgAutoLogout;
				GrTaskSvrMsgBroadCast(&Tv_Msg, sizeof(Tv_Msg));
				// done
				m_AtLogoutExpire	=	0;
				m_AtLogoutTimer		=	0;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcTimerCtrl(void)
{
	// local -------------------
		//__u32	Tv_CtlIdx;

	// code --------------------
		/*
		if(NULL != V_GrP5Env.CtrlEvtTbl[Tv_CtlIdx])
		{
			((Cls_GrP5CtrlE))
		}
		*/
		
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcTimerObj(void)
{
	// local -------------------
		__u32	Tv_TmrIdx;
	// code --------------------
		if(0 != V_GrP5Env.TimerCnt )
		{
			for( Tv_TmrIdx = 0; Tv_TmrIdx < V_GrP5Env.TimerCnt; Tv_TmrIdx++)
			{
				if(NULL != V_GrP5Env.TimerTbl[Tv_TmrIdx].UiObj)
				{
					((Cls_GrP5UoBase*)V_GrP5Env.TimerTbl[Tv_TmrIdx].UiObj)->EvtTimerTick();
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcMsgQuadChanged(Ptr_GrTaskMsgQuadChgDone A_PtrMsg)
{
	// local -------------------
		__u8	Tv_ConIdx;
		__u8	Tv_LayIdx;
		__u32	Tv_WkIdx;
	// code --------------------
		// get console
		if(E_GrP5ConMaxCnt > A_PtrMsg->ConIdx)
		{
			Tv_ConIdx	=	A_PtrMsg->ConIdx;
			// check exist console
			if(NULL != V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)
			{
				// event window
				for( Tv_LayIdx = 0; Tv_LayIdx < E_GrP5LayerMaxCnt; Tv_LayIdx++)
				{
					for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5WinCntForLayer; Tv_WkIdx++)
					{
						if(NULL != V_GrP5Env.ConTbl[Tv_ConIdx].WinTbl[Tv_LayIdx][Tv_WkIdx].UiObj)
						{
							((Cls_GrP5UoBase*)V_GrP5Env.ConTbl[Tv_ConIdx].WinTbl[Tv_LayIdx][Tv_WkIdx].UiObj)->EvtQuadChanged();
						}
					}
				}
				// event slave
				for( Tv_WkIdx = 0; Tv_WkIdx < E_GrP5SlaveObjMaxCnt; Tv_WkIdx++)
				{
					if(NULL != V_GrP5Env.ConTbl[Tv_ConIdx].SlaveObjTbl[Tv_WkIdx].UiObj)
					{
						((Cls_GrP5UoBase*)V_GrP5Env.ConTbl[Tv_ConIdx].SlaveObjTbl[Tv_WkIdx].UiObj)->EvtQuadChanged();
					}
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP5Mng::ProcMsgQuadChanged - console (%d) is not exist.\n", A_PtrMsg->ConIdx);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP5Mng::ProcMsgQuadChanged - console index (%d) is big.\n", A_PtrMsg->ConIdx);
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP5Mng::VkbdGetScptId(__u32 A_Type)
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrP5ScptIdNone;

		if(NULL != m_PtrVkbdCfg)
		{
			switch(A_Type)
			{
				case E_GrP5VkbdTypeNormal:
					Tv_Result	=	m_PtrVkbdCfg->WinIdNormal;
					break;
				case E_GrP5VkbdTypeDec:
					Tv_Result	=	m_PtrVkbdCfg->WinIdDec;
					break;
				case E_GrP5VkbdTypeHex:
					Tv_Result	=	m_PtrVkbdCfg->WinIdHex;
					break;
				case E_GrP5VkbdTypePassword:
					Tv_Result	=	m_PtrVkbdCfg->WinIdPassword;
					break;
				case E_GrP5VkbdTypeUrl:
					Tv_Result	=	m_PtrVkbdCfg->WinIdUrl;
					break;
				case E_GrP5VkbdTypeEmail:
					Tv_Result	=	m_PtrVkbdCfg->WinIdEmail;
					break;
			}
			
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcTimerWin(void)
{
	// local -------------------
		__u8	Tv_ConIdx;
		__u8	Tv_LayIdx;
		__u8	Tv_SubIdx;
		BOOL8	Tv_IsHide;
		Cls_GrP5WinBase*	Tv_Win;
		St_GrRect	Tv_Rc;
	// code --------------------
		//for(Tv_ConIdx = 0; Tv_ConIdx < E_GrP5ConMaxCnt; Tv_ConIdx++)
		{
			Tv_ConIdx	=	V_GrP5Env.MseConIdx;
			// update auto hide
			if(Tv_ConIdx == V_GrP5Env.MseConIdx)
			{
				for( Tv_LayIdx = 0; Tv_LayIdx < E_GrP5LayerMaxCnt; Tv_LayIdx++)
				{
					for( Tv_SubIdx = 0; Tv_SubIdx < E_GrP5WinCntForLayer; Tv_SubIdx++)
					{

						Tv_Win	=	(Cls_GrP5WinBase*)V_GrP5Env.ConTbl[Tv_ConIdx].WinTbl[Tv_LayIdx][Tv_SubIdx].UiObj;
						if(NULL != Tv_Win)
						{
							if(Tv_Win->ScrRectGet(&Tv_Rc))
							{
								Tv_IsHide	=	TRUE;
								if((Tv_Rc.left <= V_GrP5Env.MseX) && (Tv_Rc.right > V_GrP5Env.MseX) && 
									(Tv_Rc.top <= V_GrP5Env.MseY) && (Tv_Rc.bottom > V_GrP5Env.MseY))
								{
									Tv_IsHide	=	FALSE;
								}
								Tv_Win->RtlAutoHideUpdate(Tv_IsHide);
							}
						}
					}
				}
				
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::MseCursorOnSet(BOOL8 A_IsOn)
{
	// local -------------------
	// code --------------------
		if(A_IsOn != V_GrP5Env.IsMseCusOn)
		{
			if(A_IsOn)
			{
				// get console index
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)->CursorOnSet(TRUE);
			}
			else
			{
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)->CursorOnSet(FALSE);
			}
			V_GrP5Env.IsMseCusOn	=	A_IsOn;
			GrP5ConRedraw(V_GrP5Env.MseConIdx);
		}
		// reset mouse timer
		if(A_IsOn)
		{
			m_MseHideTimer	=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::AutoLogoutReq(__u32 A_ExpiredSec)
{
	// local -------------------
		St_GrP5MsgAtLogoutSet	Tv_Msg;
	// code --------------------
		Tv_Msg.CmdHd.MsgHd.Cmd	=	E_GrTaskMsgUiMsg;
		Tv_Msg.CmdHd.Cmd				=	E_GrP5MsgCmdAutoLogoutSet;
		Tv_Msg.ExpSec						=	A_ExpiredSec;

		UiMsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::ProcMsgAtLogoutSet(Ptr_GrP5MsgAtLogoutSet A_PtrMsg)
{
	// local -------------------
	// code --------------------
		if(0 == A_PtrMsg->ExpSec)
		{
			m_AtLogoutExpire	=	0;
		}
		else
		{
			m_AtLogoutExpire	=	A_PtrMsg->ExpSec * (1000 / E_GrP5MngTimerPeriod);
		}
		m_AtLogoutTimer		=	0;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Mng::ImmPntCodeBld(__u8 A_ConIdx)
{
	// local -------------------
	// code --------------------
		// check console number
		if(E_GrP5ConMaxCnt <= A_ConIdx)
		{
			return	FALSE;
		}
		// check console exist
		if(NULL == V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
		{
			return	FALSE;
		}

		// build paint code
		((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon)->RtlPaintCodeBuild();

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5Mng::ReqPntCodeBld(__u8 A_ConIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP5MsgPntCodeBld	Tv_Msg;
	// code --------------------

		Tv_Result	=	FALSE;

		Tv_Msg.CmdHd.Cmd	=	E_GrP5MsgCmdPntCodeBld;
		Tv_Msg.PtrRtRslt	=	&Tv_Result;
		Tv_Msg.ConIdx			=	A_ConIdx;

		UiMsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::RtlTooltipUpdate(__u16 A_ObjId)
{
	// local -------------------
	// code --------------------
		m_Tooltip.ObjIdNew	=	A_ObjId;
}
//--------------------------------------------------------------------
void	Cls_GrP5Mng::LcTimerTooltip(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Uo;
		__u16	Tv_TxtId;
		St_GrRect	Tv_RcBase;
	// code --------------------
		// check not popup
		if(E_GrP5TxtIdNone == m_Tooltip.TxtId)
		{
			// check change
			if((m_Tooltip.ObjIdNew != m_Tooltip.ObjIdNow) || (E_GrP5ObjIdNone == m_Tooltip.ObjIdNew))
			{
				// update
				m_Tooltip.ObjIdNow	=	m_Tooltip.ObjIdNew;
				// reset pop timer
				m_Tooltip.TimePop		=	0;
				return;
			}

			// do popup time
			if(E_GrP5MngTooltipPopTime <= m_Tooltip.TimePop)
			{
				return;
			}

			m_Tooltip.TimePop ++;
			if(E_GrP5MngTooltipPopTime > m_Tooltip.TimePop)
			{
				return;
			}

			// do popup
			// get exist tooltip
			Tv_Uo	=	(Cls_GrP5UoBase*)GrP5UiObjGet(m_Tooltip.ObjIdNow);
			if(NULL == Tv_Uo)
			{
				return;
			}

			m_Tooltip.TxtId	=	Tv_Uo->TooltipTextIdGet();
			if(E_GrP5TxtIdNone == m_Tooltip.TxtId)
			{
				return;
			}

			// check popupable object
			if(!Tv_Uo->ScrRectGet(&Tv_RcBase))
			{
				m_Tooltip.ObjIdPop	=	m_Tooltip.ObjIdNow;
				m_Tooltip.TxtId	=	E_GrP5TxtIdNone;
				return;
			}
			
			// do popup
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)->RtlTooltipSet(&Tv_RcBase, m_Tooltip.TxtId);
			m_Tooltip.TimePop		=	0;
			m_Tooltip.TimeKeep	=	0;
			return;
		}

		// on popup

		// check mouse off
		if(!V_GrP5Env.IsMseCusOn)
		{
			// close tooltip
			m_Tooltip.TimePop		=	0;
			m_Tooltip.TimeKeep	=	0;
			m_Tooltip.TxtId	=	E_GrP5TxtIdNone;
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)->RtlTooltipSet(&Tv_RcBase, m_Tooltip.TxtId);

			return;
		}

		if((E_GrP5ObjIdNone != m_Tooltip.ObjIdNew) && (m_Tooltip.ObjIdNew == m_Tooltip.ObjIdPop))
		{
			// refresh keep time
			m_Tooltip.TimeKeep	=	0;
			return;
		}

		// changed
		// update new popup
		// update new object id
		m_Tooltip.ObjIdNow	=	m_Tooltip.ObjIdNew;

		// get exist tooltip
		Tv_Uo	=	(Cls_GrP5UoBase*)GrP5UiObjGet(m_Tooltip.ObjIdNow);
		if(NULL != Tv_Uo)
		{
			Tv_TxtId	=	Tv_Uo->TooltipTextIdGet();
			if(E_GrP5TxtIdNone != Tv_TxtId)
			{
				// check popupable object
				if(Tv_Uo->ScrRectGet(&Tv_RcBase))
				{
					// do change tooltip
					m_Tooltip.ObjIdPop	=	m_Tooltip.ObjIdNow;
					m_Tooltip.TxtId	=	Tv_TxtId;

					((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)->RtlTooltipSet(&Tv_RcBase, m_Tooltip.TxtId);
					m_Tooltip.TimePop		=	0;
					m_Tooltip.TimeKeep	=	0;
					return;
				}
			}
		}

		// close timer check
		m_Tooltip.TimeKeep ++;
		if(E_GrP5MngTooltipKeepTime <= m_Tooltip.TimeKeep)
		{
			// close tooltip
			m_Tooltip.ObjIdPop	=	0;
			m_Tooltip.TimePop		=	0;
			m_Tooltip.TimeKeep	=	0;
			m_Tooltip.TxtId	=	E_GrP5TxtIdNone;
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[V_GrP5Env.MseConIdx].ObjCon)->RtlTooltipSet(&Tv_RcBase, m_Tooltip.TxtId);
		}

}
//--------------------------------------------------------------------
