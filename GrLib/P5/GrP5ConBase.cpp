/*
Platform 5 UI console base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5ConBase.h>
#include <P5/GrP5Mng.h>

#include <GrDumyTool.h>

#include <P5/GrP5UoBase.h>
#include <P5/GrP5WinBase.h>

#include <GrDebug.h>
#include <GrMath.h>

#include <P5/GrP5Paint.h>

//--------------------------------------------------------------------
// local constant


//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5ConBase::Cls_GrP5ConBase(__u8 A_ConIdx)
{
	// local -------------------
	// code --------------------
		// init
		m_ConIdx				=	A_ConIdx;

		m_IsMseObjUpdt	=	FALSE;
		m_IsPntExist		=	FALSE;

		m_GdibScr				=	NULL;

		m_RcScr.left		=	0;
		m_RcScr.top			=	0;
		m_RcScr.right		=	0;
		m_RcScr.bottom	=	0;

		GrDumyZeroMem(m_PntTgTbl, sizeof(m_PntTgTbl));

		GrDumyZeroMem(&m_Tooltip, sizeof(m_Tooltip));

}
//--------------------------------------------------------------------
Cls_GrP5ConBase::~Cls_GrP5ConBase(void)
{
		// release object
		RtlUiObjReleaseAll();
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::ScrRectGet(Ptr_GrRect A_PtrRtRect)
{
		*A_PtrRtRect	=	m_RcScr;
}
//--------------------------------------------------------------------
Cls_GrGdib*	Cls_GrP5ConBase::ScrGdibGet(void)
{
		return	m_GdibScr;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::IsCanLeave(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		
		if(V_GrP5Env.IsMseCap && (E_GrP5ObjIdNone != V_GrP5Env.MseObjId))
		{
			Tv_Result	=	FALSE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::PaintReq(Ptr_GrRect A_PtrRect)
{
	// local -------------------
	// code --------------------

#ifdef E_GrP5ConPntReqChk
		if ((0 > A_PtrRect->left) || (0 > A_PtrRect->top))
		{
			DbgMsgPrint("Cls_GrP5ConBase::PaintReq - bad rect (l:%d t:%d r:%d b:%d) input!\n",
				A_PtrRect->left,A_PtrRect->top,A_PtrRect->right,A_PtrRect->bottom);
		}
#endif

		m_PntSp		=	0;
		// insert stack
		m_PntSrStk[m_PntSp]	=	*A_PtrRect;

		// do
		while(0 <= m_PntSp)
		{
			LcPntArngProc();
		}

		m_IsPntExist	=	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::PaintAllReq(void)
{
	// local -------------------
	// code --------------------
		PaintReq(&m_RcScr);
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::EvtTimer(void)
{
	// local -------------------
	// code --------------------
		if(m_IsMseObjUpdt)
		{
			if(V_GrP5Env.IsMseCusOn && (m_ConIdx == V_GrP5Env.MseConIdx))
			{
				LcMseObjUpdate();
			}
			m_IsMseObjUpdt	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::LcPntBldByRect(Ptr_GrRect A_PtrRect)
{
	// local -------------------
		__u8	Tv_Layer;
		__u8	Tv_WinIdx;
		Ptr_GrP5ConCtrl	Tv_PtrConCtrl;
	// code --------------------
		// init
		Tv_PtrConCtrl	=	&V_GrP5Env.ConTbl[m_ConIdx];

		// build clip
		if(!GrP5PntSectionStart(A_PtrRect))
		{
			DbgMsgPrint("Cls_GrP5ConBase::LcPntBldByRect - section start fail!\n");
			return;
		}

		for(Tv_Layer = 0; Tv_Layer < E_GrP5LayerMaxCnt; Tv_Layer++)
		{
			// try paint
			for(Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
			{
				if(NULL != Tv_PtrConCtrl->WinTbl[Tv_Layer][Tv_WinIdx].UiObj)
				{
					((Cls_GrP5UoBase*)Tv_PtrConCtrl->WinTbl[Tv_Layer][Tv_WinIdx].UiObj)->RtlPaintProc(m_GdibScr, A_PtrRect);
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlTimer(void)
{
	// local -------------------
	// code --------------------
		// timer event
		EvtTimer();

		if(m_IsPntExist)
		{
			V_GrP5Env.ConTbl[m_ConIdx].IsPntNeed	=	TRUE;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::EvtPaintPrepare(Ptr_GrRect A_PtrRect)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::EvtPaintHwTransfer(Ptr_GrRect A_PtrRect)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::EvtMseEnter(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::EvtMseLeave(void)
{
	// local -------------------
	// code --------------------
		// reset mouse object
		
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::EvtVdoFormatChanged(void)
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::EvtMseMove(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::EvtMseLbtnDown(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::EvtMseLbtnUp(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::EvtMseLbtnDblclk(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::EvtMseRbtnDown(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::EvtMseRbtnUp(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::EvtMseMbtnDown(void)
{
	return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::EvtMseMbtnUp(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::EvtMseWheelUp(__u8 A_WheelIdx)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::EvtMseWheelDown(__u8 A_WheelIdx)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlMseEnter(void)
{
	// local -------------------
	// code --------------------
		// event
		EvtMseEnter();
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlMseLeave(void)
{
	// local -------------------
	// code --------------------
		// event
		EvtMseLeave();

		// mouse on object release

		// redraw all

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlMseMove(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoMse;
		St_GrRect	Tv_RcUo;
		__s32	Tv_UoInX;
		__s32	Tv_UoInY;
	// code --------------------
		// call event
		if(EvtMseMove())
		{
			// check capture
			if(V_GrP5Env.IsMseCap  && (E_GrP5ObjIdNone != V_GrP5Env.MseObjId))
			{
				// capture mode
				Tv_UoMse	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.MseObjId);
				if (NULL != Tv_UoMse)
				{
					Tv_UoMse->ScrRectGet(&Tv_RcUo);
					Tv_UoInX	=	V_GrP5Env.MseX - Tv_RcUo.left;
					Tv_UoInY	=	V_GrP5Env.MseY - Tv_RcUo.top;
					Tv_UoMse->EvtMseMove(Tv_UoInX, Tv_UoInY, V_GrP5Env.MseBtnMap);
				}
				else
				{
#ifdef E_GrP5DbgMsgWarnning
					DbgMsgPrint("Cls_GrP5ConBase::RtlMseMove - Capture object (%4X) not found!\n", V_GrP5Env.MseObjId);
#endif
					// release capture
					V_GrP5Env.MseObjId	=	E_GrP5ObjIdNone;
					V_GrP5Env.IsMseCap	=	FALSE;
				}
			}
			else
			{
				// none capture mode
				LcMseObjUpdate();

				// mouse move event
				if(E_GrP5ScptIdNone != V_GrP5Env.MseObjId)
				{
					Tv_UoMse	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.MseObjId);
					if(NULL != Tv_UoMse)
					{
						Tv_UoMse->ScrRectGet(&Tv_RcUo);
						Tv_UoInX	=	V_GrP5Env.MseX - Tv_RcUo.left;
						Tv_UoInY	=	V_GrP5Env.MseY - Tv_RcUo.top;

						Tv_UoMse->EvtMseMove(Tv_UoInX, Tv_UoInY, V_GrP5Env.MseBtnMap);
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlMseLbtnDown(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoMse;
		St_GrRect	Tv_RcUo;
		__s32	Tv_UoInX;
		__s32	Tv_UoInY;
		__s32	Tv_PopIdx;
	// code --------------------
		// call event
		if(EvtMseLbtnDown())
		{
			// get control
			if(E_GrP5ObjIdNone == V_GrP5Env.MseObjId)
			{
				LcMseObjUpdate();
			}

			Tv_UoMse	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.MseObjId);
			if (NULL != Tv_UoMse)
			{
				// get popup index
				Tv_PopIdx	=	GrP5PopWinIdxByUo(Tv_UoMse);
				if(0 > Tv_PopIdx)
				{
					// close all popup
					GrP5PopupCloseAll();
				}

				// try key set focus
				GrP5KeyFocusSet(Tv_UoMse->ObjIdGet(),0);
			}
			else
			{
				GrP5PopupCloseAll();
			}

			if(NULL != Tv_UoMse)
			{
				// left button down event
				Tv_UoMse->ScrRectGet(&Tv_RcUo);
				Tv_UoInX	=	V_GrP5Env.MseX - Tv_RcUo.left;
				Tv_UoInY	=	V_GrP5Env.MseY - Tv_RcUo.top;
				Tv_UoMse->EvtMseLbtnDown(Tv_UoInX, Tv_UoInY, V_GrP5Env.MseBtnMap);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlMseLbtnUp(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoMse;
		St_GrRect	Tv_RcUo;
		__s32	Tv_UoInX;
		__s32	Tv_UoInY;
		__s32	Tv_PopIdx;
	// code --------------------
		// call event
		if(EvtMseLbtnUp())
		{
			// get control
			Tv_UoMse	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.MseObjId);
			if(NULL != Tv_UoMse)
			{
				// get popup index
				Tv_PopIdx	=	GrP5PopWinIdxByUo(Tv_UoMse);
				if(0 > Tv_PopIdx)
				{
					// close all popup
					GrP5PopupCloseAll();
				}

				// event
				Tv_UoMse->ScrRectGet(&Tv_RcUo);
				Tv_UoInX	=	V_GrP5Env.MseX - Tv_RcUo.left;
				Tv_UoInY	=	V_GrP5Env.MseY - Tv_RcUo.top;
				Tv_UoMse->EvtMseLbtnUp(Tv_UoInX, Tv_UoInY, V_GrP5Env.MseBtnMap);
			}
			else
			{
				// close all popup
				GrP5PopupCloseAll();
			}
		}
}
//--------------------------------------------------------------------
#ifdef P5_DBLCLK
void	Cls_GrP5ConBase::RtlMseLbtnDblclk(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoMse;
		St_GrRect	Tv_RcUo;
		__s32	Tv_UoInX;
		__s32	Tv_UoInY;
	// code --------------------
		// call event
		if (EvtMseLbtnDblclk())
		{
			// get control
			Tv_UoMse	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.MseObjId);
			if (NULL != Tv_UoMse)
			{
				// event
				Tv_UoMse->ScrRectGet(&Tv_RcUo);
				Tv_UoInX	=	V_GrP5Env.MseX - Tv_RcUo.left;
				Tv_UoInY	=	V_GrP5Env.MseY - Tv_RcUo.top;
				Tv_UoMse->EvtMseLbtnDblclk(Tv_UoInX, Tv_UoInY, V_GrP5Env.MseBtnMap);

			}
		}
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlMseRbtnDown(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoMse;
		St_GrRect	Tv_RcUo;
		__s32	Tv_UoInX;
		__s32	Tv_UoInY;
		__s32	Tv_PopIdx;
	// code --------------------
		// call event
		if(EvtMseRbtnDown())
		{
			// get control
			Tv_UoMse	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.MseObjId);
			if(NULL != Tv_UoMse)
			{
				// get popup index
				Tv_PopIdx	=	GrP5PopWinIdxByUo(Tv_UoMse);
				if(0 > Tv_PopIdx)
				{
					// close all popup
					GrP5PopupCloseAll();
				}

				// try key set focus
				GrP5KeyFocusSet(Tv_UoMse->ObjIdGet(),0);

				// event
				Tv_UoMse->ScrRectGet(&Tv_RcUo);
				Tv_UoInX	=	V_GrP5Env.MseX - Tv_RcUo.left;
				Tv_UoInY	=	V_GrP5Env.MseY - Tv_RcUo.top;
				Tv_UoMse->EvtMseRbtnDown(Tv_UoInX, Tv_UoInY, V_GrP5Env.MseBtnMap);

			}
			else
			{
				GrP5PopupCloseAll();
			}

		}

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlMseRbtnUp(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoMse;
		St_GrRect	Tv_RcUo;
		__s32	Tv_UoInX;
		__s32	Tv_UoInY;
		__s32	Tv_PopIdx;
	// code --------------------
		// call event
		if(EvtMseRbtnUp())
		{
			// get control
			Tv_UoMse	=	(Cls_GrP5UoBase*)LcMseGlbAbleObjGetByPos(V_GrP5Env.MseX, V_GrP5Env.MseY);
			if(NULL != Tv_UoMse)
			{
				// get popup index
				Tv_PopIdx	=	GrP5PopWinIdxByUo(Tv_UoMse);
				if(0 > Tv_PopIdx)
				{
					// close all popup
					GrP5PopupCloseAll();
				}

				// do button
				while(NULL != Tv_UoMse)
				{
					// event
					Tv_UoMse->ScrRectGet(&Tv_RcUo);
					Tv_UoInX	=	V_GrP5Env.MseX - Tv_RcUo.left;
					Tv_UoInY	=	V_GrP5Env.MseY - Tv_RcUo.top;
					if(Tv_UoMse->EvtMseRbtnUp(Tv_UoInX, Tv_UoInY, V_GrP5Env.MseBtnMap))
					{
						break;
					}
					// get next
					Tv_UoMse	=	Tv_UoMse->ParentObjGet();
				}
			}
			else
			{
				Cls_GrP5WinBase* Tv_Win;
				__u16	Tv_ObjId;

				GrP5PopupCloseAll();

				Tv_Win	=	(Cls_GrP5WinBase*)LcMseGlbWinFind();
				if(NULL != Tv_Win)
				{
					// get focused object
					Tv_ObjId	=	Tv_Win->RtlWinKeyFocusSlaveGet();
					Tv_UoMse	=	(Cls_GrP5UoBase*)GrP5UiObjGet(Tv_ObjId);
					if(NULL == Tv_UoMse)
					{
						Tv_UoMse	=	(Cls_GrP5UoBase*)Tv_Win;
					}

					while(NULL != Tv_UoMse)
					{
						// event
						if(Tv_UoMse->EvtMseRbtnUp(0, 0, V_GrP5Env.MseBtnMap))
						{
							break;
						}
						// get next
						Tv_UoMse	=	Tv_UoMse->ParentObjGet();
					}
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlMseMbtnDown(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoMse;
		St_GrRect	Tv_RcUo;
		__s32	Tv_UoInX;
		__s32	Tv_UoInY;
		__s32	Tv_PopIdx;
	// code --------------------
		// call event
		if(EvtMseMbtnDown())
		{
			// get control
			Tv_UoMse	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.MseObjId);
			if(NULL != Tv_UoMse)
			{
				// get popup index
				Tv_PopIdx	=	GrP5PopWinIdxByUo(Tv_UoMse);
				if(0 > Tv_PopIdx)
				{
					// close all popup
					GrP5PopupCloseAll();
				}

				// try key set focus
				GrP5KeyFocusSet(Tv_UoMse->ObjIdGet(),0);

				// event
				Tv_UoMse->ScrRectGet(&Tv_RcUo);
				Tv_UoInX	=	V_GrP5Env.MseX - Tv_RcUo.left;
				Tv_UoInY	=	V_GrP5Env.MseY - Tv_RcUo.top;
				Tv_UoMse->EvtMseMbtnDown(Tv_UoInX, Tv_UoInY, V_GrP5Env.MseBtnMap);

			}
			else
			{
				GrP5PopupCloseAll();
			}

		}
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlMseMbtnUp(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoMse;
		St_GrRect	Tv_RcUo;
		__s32	Tv_UoInX;
		__s32	Tv_UoInY;
		__s32	Tv_PopIdx;
	// code --------------------
		// call event
		if(EvtMseMbtnDown())
		{
			// get control
			Tv_UoMse	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.MseObjId);
			if(NULL != Tv_UoMse)
			{
				// get popup index
				Tv_PopIdx	=	GrP5PopWinIdxByUo(Tv_UoMse);
				if(0 > Tv_PopIdx)
				{
					// close all popup
					GrP5PopupCloseAll();
				}

				// try key set focus
				//GrP5KeyFocusSet(Tv_UoMse->ObjIdGet());

				// event
				Tv_UoMse->ScrRectGet(&Tv_RcUo);
				Tv_UoInX	=	V_GrP5Env.MseX - Tv_RcUo.left;
				Tv_UoInY	=	V_GrP5Env.MseY - Tv_RcUo.top;
				Tv_UoMse->EvtMseMbtnUp(Tv_UoInX, Tv_UoInY, V_GrP5Env.MseBtnMap);

			}
			else
			{
				GrP5PopupCloseAll();
			}

		}
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlMseWheelUp(__u8 A_WheelIdx)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoMse;
	// code --------------------
		// call event
		if (EvtMseWheelUp(A_WheelIdx))
		{
			// get control
			Tv_UoMse	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.MseObjId);
			while(NULL != Tv_UoMse)
			{
				// event
				if(Tv_UoMse->EvtMseWheelUp(A_WheelIdx))
				{
					break;
				}
				// get next
				Tv_UoMse	=	Tv_UoMse->ParentObjGet();
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlMseWheelDown(__u8 A_WheelIdx)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoMse;
	// code --------------------
		// call event
		if (EvtMseWheelDown(A_WheelIdx))
		{
			// get control
			Tv_UoMse	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.MseObjId);
			while(NULL != Tv_UoMse)
			{
				// event
				if(Tv_UoMse->EvtMseWheelDown(A_WheelIdx))
				{
					break;
				}
				// get next
				Tv_UoMse	=	Tv_UoMse->ParentObjGet();
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::EvtKeyInput(__u8 A_DevId, __u8 A_Key, BOOL8 A_IsPush)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::RtlMsgProc(Ptr_GrP5MsgHd A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;

		switch(A_PtrMsg->Cmd)
		{
			case E_GrP5MsgCmdConWinCloseReq:
				LcMprcCloseReq((Ptr_GrP5MsgConWinCloseReq)A_PtrMsg);
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::LcMprcCloseReq(Ptr_GrP5MsgConWinCloseReq A_PtrMsg)
{
	// local -------------------
		__u8	Tv_ConIdx;
		__u8	Tv_Layer;
		__u8	Tv_WinIdx;
		__u8	Tv_LocId;
	// code --------------------
		// get window slot
		if (GrP5WinObjIdDecode(A_PtrMsg->ObjId, &Tv_ConIdx, &Tv_Layer, &Tv_WinIdx, &Tv_LocId))
		{
			// check match console id
			if (m_ConIdx == Tv_ConIdx)
			{
				// check exist window
				if (NULL != V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_Layer][Tv_WinIdx].UiObj)
				{
					if (A_PtrMsg->ObjId == ((Cls_GrP5WinBase*)V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_Layer][Tv_WinIdx].UiObj)->ObjIdGet())
					{
						((Cls_GrP5WinBase*)V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_Layer][Tv_WinIdx].UiObj)->RtlDoEvtWinClose();
						delete	((Cls_GrP5WinBase*)V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_Layer][Tv_WinIdx].UiObj);
						V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_Layer][Tv_WinIdx].UiObj	=	NULL;
					}
				}
			}
		}
		
		
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
BOOL8	Cls_GrP5ConBase::DvlIsEdit(void)
{
		return	FALSE;
}
#endif
//--------------------------------------------------------------------
__s8	Cls_GrP5ConBase::QuadChGetByPos(__s32 A_X, __s32 A_Y)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::QuadModeSet(__u8 A_Mode, __u8 A_Page)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::QuadModeGet(__u8* A_PtrRtMode, __u8* A_PtrRtPage)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::QuadModeNext(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::QuadModePrev(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::QuadPageNext(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::QuadPagePrev(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::QuadModeExchange(__u8 A_RefCh)
{
		return	FALSE;
}
//--------------------------------------------------------------------
Ptr_GrRect	Cls_GrP5ConBase::QuadLineRcPtrGet(__u32* A_PtrRtCnt)
{
		*A_PtrRtCnt	=	0;
		return	NULL;
}
//--------------------------------------------------------------------
__u32	Cls_GrP5ConBase::QuadLineWidthGet(void)
{
		return	0;
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::QuadLineWidthSet(__u32)
{
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::QuadRectSet(Ptr_GrRect A_PtrRc)
{
		return	FALSE;
}
//--------------------------------------------------------------------
Ptr_GrRect	Cls_GrP5ConBase::QuadViewRcPtrGet(__u32* A_PtrRtCnt)
{
		*A_PtrRtCnt	=	0;
		return	NULL;
}
//--------------------------------------------------------------------
__s8	Cls_GrP5ConBase::QuadChGetByViewIdx(__u8 A_Idx)
{
		return	-1;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP5ConBase::QuadChNameGet(__u8 A_Ch)
{
		return	NULL;
}
//--------------------------------------------------------------------
__u8	Cls_GrP5ConBase::QuadChRecTypeGet(__u8 A_Ch)
{
		return	0;
}
//--------------------------------------------------------------------
__u32	Cls_GrP5ConBase::QuadRecChMapGet(void)
{
	return	0;
}
//--------------------------------------------------------------------
__u32	Cls_GrP5ConBase::QuadMotionChMapGet(void)
{
	return	0;
}
//--------------------------------------------------------------------
__u32	Cls_GrP5ConBase::QuadSensorChMapGet(void)
{
	return	0;
}
//--------------------------------------------------------------------
__u32	Cls_GrP5ConBase::QuadPtzChMapGet(void)
{
	return	0;
}
//--------------------------------------------------------------------
__u32	Cls_GrP5ConBase::QuadAdoChMapGet(void)
{
	return	0;
}
//--------------------------------------------------------------------
__u32	Cls_GrP5ConBase::QuadIaChMapGet(void)
{
	return	0;
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlUiObjReleaseAll(void)
{
	// local -------------------
		__u8	Tv_Layer;
		__u8	Tv_WinIdx;
	// code --------------------
		for( Tv_Layer = 0; Tv_Layer < E_GrP5LayerMaxCnt; Tv_Layer++)
		{
			for( Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
			{
				// check exist window
				if(NULL != V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_Layer][Tv_WinIdx].UiObj)
				{
					//((Cls_GrP5WinBase*)V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_Layer][Tv_WinIdx].UiObj)->RtlDoEvtWinClose();
					delete	((Cls_GrP5WinBase*)V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_Layer][Tv_WinIdx].UiObj);
					V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_Layer][Tv_WinIdx].UiObj	=	NULL;
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::CursorOnSet(BOOL8 A_IsOn)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::UiObjScrRectRebuild(void)
{
	// local -------------------
		__u32	Tv_LayIdx;
		__u32	Tv_WinIdx;
	// code --------------------
		for( Tv_LayIdx = 0; Tv_LayIdx < E_GrP5LayerMaxCnt; Tv_LayIdx++)
		{
			for( Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
			{
				if(NULL != V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)
				{
					((Cls_GrP5UoBase*)V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)->RtlScrRectBld();
				}
			}

		}
		
}
//--------------------------------------------------------------------
void*	Cls_GrP5ConBase::LcMseGlbWinFind(void)
{
	// local -------------------
		void*	Tv_Result;
		Cls_GrP5WinBase*	Tv_Obj;
		__u8	Tv_LayIdx;
		__u8	Tv_WinIdx;
		BOOL8	Tv_IsLayLock;
	// code --------------------
		Tv_Result	=	NULL;

		Tv_LayIdx	=	E_GrP5LayerMaxCnt;
		Tv_IsLayLock	=	FALSE;

		do
		{
			Tv_LayIdx --;

			for( Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
			{
				Tv_Obj	=	(Cls_GrP5WinBase*)V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj;
				if(NULL != Tv_Obj)
				{
					// check mouse able
					if(Tv_Obj->IsVisible() && Tv_Obj->IsEnable() && (!Tv_Obj->IsMseSkip()) && (Tv_Obj->IsMseGlb()) )
					{
						// found
						Tv_Result	=	(void*)Tv_Obj;
						break;
					}
					// check layer lock
					if(Tv_Obj->IsInputLayerLock())
					{
						Tv_IsLayLock	=	TRUE;
					}
				}
			}
			
			if((NULL != Tv_Result) || Tv_IsLayLock)
			{
				break;
			}

		} while(0 != Tv_LayIdx);

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrP5ConBase::LcMseGlbAbleObjGetByPos(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		void*	Tv_Result;
		Cls_GrP5WinBase*	Tv_ObjWin;
		St_GrRect	Tv_RcWin;
		__u8	Tv_LayIdx;
		__u8	Tv_WinIdx;
		BOOL8	Tv_IsLayLock;
	// code --------------------
		Tv_Result	=	NULL;

		Tv_LayIdx	=	E_GrP5LayerMaxCnt;
		Tv_IsLayLock	=	FALSE;

		do
		{
			Tv_LayIdx --;

			for(Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
			{
				Tv_ObjWin	=	(Cls_GrP5WinBase*)V_GrP5Env.ConTbl[m_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj;
				if(NULL != Tv_ObjWin)
				{
					// check mouse able
					if(Tv_ObjWin->IsVisible())
					{
						if(Tv_ObjWin->IsEnable() && (!Tv_ObjWin->IsMseSkip()))
						{
							// check position
							if(Tv_ObjWin->ScrRectGet(&Tv_RcWin))
							{
								if((A_X >= Tv_RcWin.left) && (A_X < Tv_RcWin.right) && (A_Y >= Tv_RcWin.top) && (A_Y < Tv_RcWin.bottom))
								{
									// find by pos
									Tv_Result	=	(void*)Tv_ObjWin->RtlUoVisibleFindByPos(A_X, A_Y);
									break;
								}
							}
						}
						// check layer lock
						if(Tv_ObjWin->IsInputLayerLock())
						{
							Tv_IsLayLock	=	TRUE;
						}
					}
				}
			}

			if((NULL != Tv_Result) || Tv_IsLayLock)
			{
				break;
			}

		} while(0 != Tv_LayIdx);

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::LcMseObjUpdate(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoMse;
		Cls_GrP5UoBase*	Tv_UoOld;
		__u16	Tv_ObjIdNew;
	// code --------------------
		Tv_ObjIdNew		=	E_GrP5ObjIdNone;
		Tv_UoMse			=	(Cls_GrP5UoBase*)GrP5UiObjCanMseFindByPos(m_ConIdx, V_GrP5Env.MseX, V_GrP5Env.MseY);
		if(NULL != Tv_UoMse)
		{
			Tv_ObjIdNew	=	Tv_UoMse->ObjIdGet();
		}
		// check different object
		if(V_GrP5Env.MseObjId != Tv_ObjIdNew)
		{
			// update new mouse object
			Tv_UoOld	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.MseObjId);
			V_GrP5Env.MseObjId	=	Tv_ObjIdNew;

			// leave event
			if(NULL != Tv_UoOld)
			{
				Tv_UoOld->EvtMseLeave();
			}

			// enter event
			if(NULL != Tv_UoMse)
			{
				Tv_UoMse->EvtMseEnter();
			}

			// update tool tip
#ifdef GR_P5_EDITOR
			if(!DvlIsEdit())
			{
				((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->RtlTooltipUpdate(V_GrP5Env.MseObjId);
			}
#else
			((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->RtlTooltipUpdate(V_GrP5Env.MseObjId);
#endif
			
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::ReqMouseObjUpdate(void)
{
	// local -------------------
	// code --------------------
		m_IsMseObjUpdt	=	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::LcPntSrAdd(Ptr_GrRect A_PtrRcAdd)
{
	// local -------------------
	// code --------------------
#ifdef E_GrP5ConPntChk
		// check 
		if((A_PtrRcAdd->left >= A_PtrRcAdd->right) || (A_PtrRcAdd->top >= A_PtrRcAdd->bottom))
		{
			DbgMsgPrint("Cls_GrP5ConBase::LcPntSrAdd - bug! bad rect input l:%d t:%d r:%d b:%d \n",
				A_PtrRcAdd->left, A_PtrRcAdd->top, A_PtrRcAdd->right, A_PtrRcAdd->bottom);
		}
#endif
		// check stack is full
		if((m_PntSp + 1) >= E_GrP5ConPntSrStkCnt)
		{
			// full
			DbgMsgPrint("Cls_GrP5ConBase::LcPntSrAdd - source stack is full\n");
			return	FALSE;
		}

		// insert
		m_PntSp ++;
		m_PntSrStk[m_PntSp]	=	*A_PtrRcAdd;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5ConBase::LcPntTgAdd(Ptr_GrRect A_PtrRect)
{
	// local -------------------
		__s32	Tv_FndIdx;
		__u32	Tv_WkIdx;
	// code --------------------
#ifdef E_GrP5ConPntChk
		// check 
		if((A_PtrRect->left >= A_PtrRect->right) || (A_PtrRect->top >= A_PtrRect->bottom))
		{
			DbgMsgPrint("Cls_GrP5ConBase::LcPntTgAdd - bug! bad rect input l:%d t:%d r:%d b:%d \n",
				A_PtrRect->left, A_PtrRect->top, A_PtrRect->right, A_PtrRect->bottom);
		}
#endif

		Tv_FndIdx	=	-1;
		// find empty slot
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5ConPntRcTgCnt; Tv_WkIdx++)
		{
			if(0 == m_PntTgTbl[Tv_WkIdx].right)
			{
				// found
				Tv_FndIdx	=	(__s32)Tv_WkIdx;
				break;
			}
		}

		// check full
		if(0 > Tv_FndIdx)
		{
			//DbgMsgPrint("Cls_GrP5ConBase::LcPntTgAdd - taget buffer full!\n");
			return	FALSE;
		}

		m_PntTgTbl[Tv_FndIdx]	=	*A_PtrRect;
		return	TRUE;

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::LcPntRcMerge(Ptr_GrRect A_PtrTg, Ptr_GrRect A_PtrRef)
{
	// local -------------------
	// code --------------------
		if(A_PtrTg->left >= A_PtrRef->left)
		{
			A_PtrTg->left		=	A_PtrRef->left;
		}
		if(A_PtrTg->top >= A_PtrRef->top)
		{
			A_PtrTg->top		=	A_PtrRef->top;
		}
		if(A_PtrTg->right <= A_PtrRef->right)
		{
			A_PtrTg->right		=	A_PtrRef->right;
		}
		if(A_PtrTg->bottom <= A_PtrRef->bottom)
		{
			A_PtrTg->bottom		=	A_PtrRef->bottom;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::LcPntArngProc(void)
{
	// local -------------------
		__u32				Tv_TgIdx;
		BOOL8				Tv_IsAddTg;
		__u8				Tv_Ovlp;
		St_GrRect		Tv_RcTmp;
	// code --------------------
		// compare source stack with target
		Tv_TgIdx		=	0;
		Tv_IsAddTg	=	TRUE;

		while(Tv_TgIdx < E_GrP5ConPntRcTgCnt)
		{
			// check tareget exist
			if(0 == m_PntTgTbl[Tv_TgIdx].right)
			{
				Tv_TgIdx ++;
				continue;
			}

#ifdef E_GrP5ConPntChk
			LcDbgPntSrTgChk(); // debug
#endif

			// get status
			Tv_Ovlp	=	GrDrawCnChkOvlpRect(&m_PntTgTbl[Tv_TgIdx], &m_PntSrStk[m_PntSp]);

			// check not overlapped
			if(E_GrDrawChkOvlpNotOvlp == Tv_Ovlp)
			{
				// not overlapped =============================

				// check horizontal attached
				if((m_PntSrStk[m_PntSp].top == m_PntTgTbl[Tv_TgIdx].top) && (m_PntSrStk[m_PntSp].bottom == m_PntTgTbl[Tv_TgIdx].bottom))
				{
					// attached
					if(m_PntSrStk[m_PntSp].right == m_PntTgTbl[Tv_TgIdx].left)
					{
						// target left side attached
						// merge target by source
						m_PntSrStk[m_PntSp].right	=	m_PntTgTbl[Tv_TgIdx].right;
						// remove target
						m_PntTgTbl[Tv_TgIdx].left		=	0;
						m_PntTgTbl[Tv_TgIdx].right	=	0;
						m_PntTgTbl[Tv_TgIdx].top		=	0;
						m_PntTgTbl[Tv_TgIdx].bottom	=	0;

						// continue
						Tv_TgIdx ++;
						continue;
					}
					else if(m_PntSrStk[m_PntSp].left == m_PntTgTbl[Tv_TgIdx].right)
					{
						// target right side attached
						// merge target by source
						m_PntSrStk[m_PntSp].left	=	m_PntTgTbl[Tv_TgIdx].left;
						// remove target
						m_PntTgTbl[Tv_TgIdx].left		=	0;
						m_PntTgTbl[Tv_TgIdx].right	=	0;
						m_PntTgTbl[Tv_TgIdx].top		=	0;
						m_PntTgTbl[Tv_TgIdx].bottom	=	0;

						// continue
						Tv_TgIdx ++;
						continue;
					}
				}

				// check vertical attached
				if((m_PntSrStk[m_PntSp].left == m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right == m_PntTgTbl[Tv_TgIdx].right))
				{
					// attached
					if(m_PntSrStk[m_PntSp].bottom == m_PntTgTbl[Tv_TgIdx].top)
					{
						// target top side attached
						// merge target by source
						m_PntSrStk[m_PntSp].bottom	=	m_PntTgTbl[Tv_TgIdx].bottom;
						// remove target
						m_PntTgTbl[Tv_TgIdx].left		=	0;
						m_PntTgTbl[Tv_TgIdx].right	=	0;
						m_PntTgTbl[Tv_TgIdx].top		=	0;
						m_PntTgTbl[Tv_TgIdx].bottom	=	0;

#ifdef E_GrP5ConPntChk
						LcDbgPntSrTgChk(); // debug
#endif

						// continue
						Tv_TgIdx ++;
						continue;
					}
					else if(m_PntSrStk[m_PntSp].top == m_PntTgTbl[Tv_TgIdx].bottom)
					{
						// target bottom side attached
						// merge target by source
						m_PntSrStk[m_PntSp].top	=	m_PntTgTbl[Tv_TgIdx].top;
						// remove target
						m_PntTgTbl[Tv_TgIdx].left		=	0;
						m_PntTgTbl[Tv_TgIdx].right	=	0;
						m_PntTgTbl[Tv_TgIdx].top		=	0;
						m_PntTgTbl[Tv_TgIdx].bottom	=	0;

#ifdef E_GrP5ConPntChk
						LcDbgPntSrTgChk(); // debug
#endif

						// continue
						Tv_TgIdx ++;
						continue;
					}
				}

				// not matched
				// go next
				Tv_TgIdx ++;
				continue;
			}	//	if(E_GrDrawChkOvlpNotOvlp == Tv_Ovlp)

			// overlapped ========================================

			// check under target -------------------------------
			if( E_GrDrawChkOvlpCoverPara1 == Tv_Ovlp)
			{
				// finish
				Tv_IsAddTg	=	FALSE;
				break;
			}

			// check target under source ------------------------
			if(E_GrDrawChkOvlpCoverPara2 == Tv_Ovlp)
			{
				// remove target
				m_PntTgTbl[Tv_TgIdx].left		=	0;
				m_PntTgTbl[Tv_TgIdx].right	=	0;
				m_PntTgTbl[Tv_TgIdx].top		=	0;
				m_PntTgTbl[Tv_TgIdx].bottom	=	0;

				// continue
				Tv_TgIdx ++;
				continue;
			}

			// check target horizontal same  -------------------------------  0
			if ((m_PntSrStk[m_PntSp].left == m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right == m_PntTgTbl[Tv_TgIdx].right))
			{
				// merge by source
				LcPntRcMerge(&m_PntSrStk[m_PntSp], &m_PntTgTbl[Tv_TgIdx]);
				// remove target
				m_PntTgTbl[Tv_TgIdx].left		=	0;
				m_PntTgTbl[Tv_TgIdx].right	=	0;
				m_PntTgTbl[Tv_TgIdx].top		=	0;
				m_PntTgTbl[Tv_TgIdx].bottom	=	0;

#ifdef E_GrP5ConPntChk
				LcDbgPntSrTgChk(); // debug
#endif

				// continue
				Tv_TgIdx ++;
				continue;
			}

			// check equal top and bottom ----------------------- 5
			if((m_PntSrStk[m_PntSp].top == m_PntTgTbl[Tv_TgIdx].top) && (m_PntSrStk[m_PntSp].bottom == m_PntTgTbl[Tv_TgIdx].bottom))
			{
				// merge to source
				LcPntRcMerge(&m_PntSrStk[m_PntSp], &m_PntTgTbl[Tv_TgIdx]);
				// remove target
				m_PntTgTbl[Tv_TgIdx].left		=	0;
				m_PntTgTbl[Tv_TgIdx].right	=	0;
				m_PntTgTbl[Tv_TgIdx].top		=	0;
				m_PntTgTbl[Tv_TgIdx].bottom	=	0;
				// continue
				Tv_TgIdx ++;
				continue;
			}

			// check target horizontal same or bigger -------------------------------  1
			if((m_PntSrStk[m_PntSp].left >= m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right <= m_PntTgTbl[Tv_TgIdx].right))
			{
				// check source vertical side
				if(m_PntSrStk[m_PntSp].bottom <= m_PntTgTbl[Tv_TgIdx].bottom)
				{
					// souce upper side
					// patch source
					m_PntSrStk[m_PntSp].bottom	=	m_PntTgTbl[Tv_TgIdx].top;

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					// continue
					Tv_TgIdx ++;
					continue;
				}
				else if(m_PntSrStk[m_PntSp].top >= m_PntTgTbl[Tv_TgIdx].top)
				{
					// source bottom side
					// patch source
					m_PntSrStk[m_PntSp].top	=	m_PntTgTbl[Tv_TgIdx].bottom;

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					// continue
					Tv_TgIdx ++;
					continue;
				}
				// source penetrate
				Tv_RcTmp.left		=	m_PntSrStk[m_PntSp].left;
				Tv_RcTmp.top		=	m_PntSrStk[m_PntSp].top;
				Tv_RcTmp.right	=	m_PntSrStk[m_PntSp].right;
				Tv_RcTmp.bottom	=	m_PntTgTbl[Tv_TgIdx].top;

				// patch source
				m_PntSrStk[m_PntSp].top		=	m_PntTgTbl[Tv_TgIdx].bottom;

#ifdef E_GrP5ConPntChk
				LcDbgPntSrTgChk(); // debug
#endif

				if(LcPntSrAdd(&Tv_RcTmp))
				{
					// continue
					Tv_TgIdx ++;
					continue;
				}

				// source divide fail
				// restore source
				m_PntSrStk[m_PntSp].top		=	Tv_RcTmp.top;

#ifdef E_GrP5ConPntChk
				LcDbgPntSrTgChk(); // debug
#endif

				// check attached
				if(m_PntSrStk[m_PntSp].left == m_PntTgTbl[Tv_TgIdx].left)
				{
					// patch target
					m_PntTgTbl[Tv_TgIdx].left	=	m_PntSrStk[m_PntSp].right;
					// continue
					Tv_TgIdx ++;
					continue;
				}
				else if(m_PntSrStk[m_PntSp].right == m_PntTgTbl[Tv_TgIdx].right)
				{
					// patch target
					m_PntTgTbl[Tv_TgIdx].right	=	m_PntSrStk[m_PntSp].left;
					// continue
					Tv_TgIdx ++;
					continue;
				}

				// source and target merge
				LcPntRcMerge(&m_PntSrStk[m_PntSp], &m_PntTgTbl[Tv_TgIdx]);

#ifdef E_GrP5ConPntChk
				LcDbgPntSrTgChk(); // debug
#endif

				// remove target
				m_PntTgTbl[Tv_TgIdx].left		=	0;
				m_PntTgTbl[Tv_TgIdx].right	=	0;
				m_PntTgTbl[Tv_TgIdx].top		=	0;
				m_PntTgTbl[Tv_TgIdx].bottom	=	0;

				// rescan
				Tv_TgIdx		=	0;
				continue;
			}

			// source and target bottom same ------------------------------- 2
			if((m_PntSrStk[m_PntSp].bottom == m_PntTgTbl[Tv_TgIdx].bottom) && (m_PntSrStk[m_PntSp].top < m_PntTgTbl[Tv_TgIdx].top))
			{
				// check left side over
				if((m_PntSrStk[m_PntSp].left < m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right < m_PntTgTbl[Tv_TgIdx].right))
				{
					// left side over
					// make new source
					Tv_RcTmp.left		=	m_PntSrStk[m_PntSp].left;
					Tv_RcTmp.top		=	m_PntTgTbl[Tv_TgIdx].top;
					Tv_RcTmp.right	=	m_PntTgTbl[Tv_TgIdx].right;
					Tv_RcTmp.bottom	=	m_PntTgTbl[Tv_TgIdx].bottom;

					// patch source
					m_PntSrStk[m_PntSp].bottom	=	m_PntTgTbl[Tv_TgIdx].top;

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					// try add source
					if(LcPntSrAdd(&Tv_RcTmp))
					{
						// remove target
						m_PntTgTbl[Tv_TgIdx].left		=	0;
						m_PntTgTbl[Tv_TgIdx].right	=	0;
						m_PntTgTbl[Tv_TgIdx].top		=	0;
						m_PntTgTbl[Tv_TgIdx].bottom	=	0;

						// continue
						Tv_TgIdx ++;
						continue;
					}

					// fail add
					// restore source
					m_PntSrStk[m_PntSp].bottom	=	Tv_RcTmp.bottom;

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					// patch target
					m_PntTgTbl[Tv_TgIdx].left		=	m_PntSrStk[m_PntSp].right;

					// continue
					Tv_TgIdx ++;
					continue;
				}
				else if((m_PntSrStk[m_PntSp].left > m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right > m_PntTgTbl[Tv_TgIdx].right))
				{
					// right side over
					// make new source
					Tv_RcTmp.left		=	m_PntTgTbl[Tv_TgIdx].left;
					Tv_RcTmp.top		=	m_PntTgTbl[Tv_TgIdx].top;
					Tv_RcTmp.right	=	m_PntSrStk[m_PntSp].right;
					Tv_RcTmp.bottom	=	m_PntTgTbl[Tv_TgIdx].bottom;

					// patch source
					m_PntSrStk[m_PntSp].bottom	=	m_PntTgTbl[Tv_TgIdx].top;

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					// try add source
					if(LcPntSrAdd(&Tv_RcTmp))
					{
						// remove target
						m_PntTgTbl[Tv_TgIdx].left		=	0;
						m_PntTgTbl[Tv_TgIdx].right	=	0;
						m_PntTgTbl[Tv_TgIdx].top		=	0;
						m_PntTgTbl[Tv_TgIdx].bottom	=	0;

						// continue
						Tv_TgIdx ++;
						continue;
					}

					// fail add
					// restore source
					m_PntSrStk[m_PntSp].bottom	=	Tv_RcTmp.bottom;

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					// patch target
					m_PntTgTbl[Tv_TgIdx].right		=	m_PntSrStk[m_PntSp].left;

					// continue
					Tv_TgIdx ++;
					continue;
				}
			}

			// source and target top same ------------------------------- 3
			if((m_PntSrStk[m_PntSp].top == m_PntTgTbl[Tv_TgIdx].top) && (m_PntSrStk[m_PntSp].bottom > m_PntTgTbl[Tv_TgIdx].bottom))
			{
				// check left side over
				if((m_PntSrStk[m_PntSp].left < m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right < m_PntTgTbl[Tv_TgIdx].right))
				{
					// left side over
					// make new source
					Tv_RcTmp.left		=	m_PntSrStk[m_PntSp].left;
					Tv_RcTmp.top		=	m_PntTgTbl[Tv_TgIdx].top;
					Tv_RcTmp.right	=	m_PntTgTbl[Tv_TgIdx].right;
					Tv_RcTmp.bottom	=	m_PntTgTbl[Tv_TgIdx].bottom;

					// patch source
					m_PntSrStk[m_PntSp].top	=	m_PntTgTbl[Tv_TgIdx].bottom;

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					// try add source
					if(LcPntSrAdd(&Tv_RcTmp))
					{
						// remove target
						m_PntTgTbl[Tv_TgIdx].left		=	0;
						m_PntTgTbl[Tv_TgIdx].right	=	0;
						m_PntTgTbl[Tv_TgIdx].top		=	0;
						m_PntTgTbl[Tv_TgIdx].bottom	=	0;

						// continue
						Tv_TgIdx ++;
						continue;
					}

					// fail add
					// restore source
					m_PntSrStk[m_PntSp].top	=	Tv_RcTmp.top;

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					// patch target
					m_PntTgTbl[Tv_TgIdx].left		=	m_PntSrStk[m_PntSp].right;

					// continue
					Tv_TgIdx ++;
					continue;
				}
				else if((m_PntSrStk[m_PntSp].left > m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right > m_PntTgTbl[Tv_TgIdx].right))
				{
					// right side over
					// make new source
					Tv_RcTmp.left		=	m_PntTgTbl[Tv_TgIdx].left;
					Tv_RcTmp.top		=	m_PntTgTbl[Tv_TgIdx].top;
					Tv_RcTmp.right	=	m_PntSrStk[m_PntSp].right;
					Tv_RcTmp.bottom	=	m_PntTgTbl[Tv_TgIdx].bottom;

					// patch source
					m_PntSrStk[m_PntSp].top	=	m_PntTgTbl[Tv_TgIdx].bottom;

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					// try add source
					if(LcPntSrAdd(&Tv_RcTmp))
					{
						// remove target
						m_PntTgTbl[Tv_TgIdx].left		=	0;
						m_PntTgTbl[Tv_TgIdx].right	=	0;
						m_PntTgTbl[Tv_TgIdx].top		=	0;
						m_PntTgTbl[Tv_TgIdx].bottom	=	0;

						// continue
						Tv_TgIdx ++;
						continue;
					}

					// fail add
					// restore source
					m_PntSrStk[m_PntSp].top	=	Tv_RcTmp.top;

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					// patch target
					m_PntTgTbl[Tv_TgIdx].right		=	m_PntSrStk[m_PntSp].left;

					// continue
					Tv_TgIdx ++;
					continue;
				}
			}

			// target height smaller than source ---------------- 4
			if((m_PntSrStk[m_PntSp].top <= m_PntTgTbl[Tv_TgIdx].top) && (m_PntSrStk[m_PntSp].bottom >= m_PntTgTbl[Tv_TgIdx].bottom))
			{
				// check over side
				if(m_PntSrStk[m_PntSp].left < m_PntTgTbl[Tv_TgIdx].left)
				{
					// source left side over
					// divide source top side
					Tv_RcTmp.left		=	m_PntSrStk[m_PntSp].left;
					Tv_RcTmp.top		=	m_PntSrStk[m_PntSp].top;
					Tv_RcTmp.right	=	m_PntSrStk[m_PntSp].right;
					Tv_RcTmp.bottom	=	m_PntTgTbl[Tv_TgIdx].top;

					// patch soruce 
					m_PntSrStk[m_PntSp].top		=	m_PntTgTbl[Tv_TgIdx].bottom;

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					if(!LcPntSrAdd(&Tv_RcTmp))
					{
						// source add fail
						// restore source
						m_PntSrStk[m_PntSp].top	=	Tv_RcTmp.top;

#ifdef E_GrP5ConPntChk
						LcDbgPntSrTgChk(); // debug
#endif

						// patch target 
						m_PntTgTbl[Tv_TgIdx].left		=	m_PntSrStk[m_PntSp].right;
						// continue
						Tv_TgIdx ++;
						continue;
					}
					// source divide success
					// target to new soruce
					Tv_RcTmp.left		=	m_PntSrStk[m_PntSp].left;
					Tv_RcTmp.top		=	m_PntTgTbl[Tv_TgIdx].top;
					Tv_RcTmp.right	=	m_PntTgTbl[Tv_TgIdx].right;
					Tv_RcTmp.bottom	=	m_PntTgTbl[Tv_TgIdx].bottom;
					if(LcPntSrAdd(&Tv_RcTmp))
					{
						// remove target
						m_PntTgTbl[Tv_TgIdx].left		=	0;
						m_PntTgTbl[Tv_TgIdx].right	=	0;
						m_PntTgTbl[Tv_TgIdx].top		=	0;
						m_PntTgTbl[Tv_TgIdx].bottom	=	0;
						// continue
						Tv_TgIdx ++;
						continue;
					}
					// fail add new source
					// merget to source
					LcPntRcMerge(&m_PntSrStk[m_PntSp], &Tv_RcTmp);

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					// remove target
					m_PntTgTbl[Tv_TgIdx].left		=	0;
					m_PntTgTbl[Tv_TgIdx].right	=	0;
					m_PntTgTbl[Tv_TgIdx].top		=	0;
					m_PntTgTbl[Tv_TgIdx].bottom	=	0;
					// rescan
					Tv_TgIdx	=	0;
					continue;

				}
				else if(m_PntSrStk[m_PntSp].right > m_PntTgTbl[Tv_TgIdx].right)
				{
					// source right side over
					// divide source top side
					Tv_RcTmp.left		=	m_PntSrStk[m_PntSp].left;
					Tv_RcTmp.top		=	m_PntSrStk[m_PntSp].top;
					Tv_RcTmp.right	=	m_PntSrStk[m_PntSp].right;
					Tv_RcTmp.bottom	=	m_PntTgTbl[Tv_TgIdx].top;

					// patch soruce 
					m_PntSrStk[m_PntSp].top		=	m_PntTgTbl[Tv_TgIdx].bottom;

#ifdef E_GrP5ConPntChk
					LcDbgPntSrTgChk(); // debug
#endif

					if(!LcPntSrAdd(&Tv_RcTmp))
					{
						// source add fail
						// restore source
						m_PntSrStk[m_PntSp].top	=	Tv_RcTmp.top;

#ifdef E_GrP5ConPntChk
						LcDbgPntSrTgChk(); // debug
#endif

						// patch target 
						m_PntTgTbl[Tv_TgIdx].right		=	m_PntSrStk[m_PntSp].left;
						// continue
						Tv_TgIdx ++;
						continue;
					}
					// source divide success
					// target to new soruce
					Tv_RcTmp.left		=	m_PntTgTbl[Tv_TgIdx].left;
					Tv_RcTmp.top		=	m_PntTgTbl[Tv_TgIdx].top;
					Tv_RcTmp.right	=	m_PntSrStk[m_PntSp].right;
					Tv_RcTmp.bottom	=	m_PntTgTbl[Tv_TgIdx].bottom;
					if(LcPntSrAdd(&Tv_RcTmp))
					{
						// remove target
						m_PntTgTbl[Tv_TgIdx].left		=	0;
						m_PntTgTbl[Tv_TgIdx].right	=	0;
						m_PntTgTbl[Tv_TgIdx].top		=	0;
						m_PntTgTbl[Tv_TgIdx].bottom	=	0;
						// continue
						Tv_TgIdx ++;
						continue;
					}
					// fail add new source
					// merget to source
					LcPntRcMerge(&m_PntSrStk[m_PntSp], &Tv_RcTmp);
					// remove target
					m_PntTgTbl[Tv_TgIdx].left		=	0;
					m_PntTgTbl[Tv_TgIdx].right	=	0;
					m_PntTgTbl[Tv_TgIdx].top		=	0;
					m_PntTgTbl[Tv_TgIdx].bottom	=	0;
					// rescan
					Tv_TgIdx	=	0;
					continue;

				}
			}

			// check equal top --------------------------- 6
			if(m_PntSrStk[m_PntSp].top == m_PntTgTbl[Tv_TgIdx].top)
			{
				// patch source
				if(m_PntSrStk[m_PntSp].left >= m_PntTgTbl[Tv_TgIdx].left)
				{
					m_PntSrStk[m_PntSp].left	=	m_PntTgTbl[Tv_TgIdx].left;
				}
				if(m_PntSrStk[m_PntSp].right <= m_PntTgTbl[Tv_TgIdx].right)
				{
					m_PntSrStk[m_PntSp].right	=	m_PntTgTbl[Tv_TgIdx].right;
				}

				// patch target
				m_PntTgTbl[Tv_TgIdx].top	=	m_PntSrStk[m_PntSp].bottom;
				// continue
				Tv_TgIdx ++;
				continue;
			}

			// check equal bottom ----------------------- 7
			if(m_PntSrStk[m_PntSp].bottom == m_PntTgTbl[Tv_TgIdx].bottom)
			{
				// patch source
				if(m_PntSrStk[m_PntSp].left >= m_PntTgTbl[Tv_TgIdx].left)
				{
					m_PntSrStk[m_PntSp].left	=	m_PntTgTbl[Tv_TgIdx].left;
				}
				if(m_PntSrStk[m_PntSp].right <= m_PntTgTbl[Tv_TgIdx].right)
				{
					m_PntSrStk[m_PntSp].right	=	m_PntTgTbl[Tv_TgIdx].right;
				}

				// patch target
				m_PntTgTbl[Tv_TgIdx].bottom	=	m_PntSrStk[m_PntSp].top;
				// continue
				Tv_TgIdx ++;
				continue;
			}

			// source over upper -------------------------  8
			if((m_PntSrStk[m_PntSp].top < m_PntTgTbl[Tv_TgIdx].top) && (m_PntSrStk[m_PntSp].bottom < m_PntTgTbl[Tv_TgIdx].bottom))
			{
				// check side over
				if((m_PntSrStk[m_PntSp].left <= m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right >= m_PntTgTbl[Tv_TgIdx].right))
				{
					// patch target
					m_PntTgTbl[Tv_TgIdx].top	=	m_PntSrStk[m_PntSp].bottom;
					// continue
					Tv_TgIdx ++;
					continue;
				}
			}

			// source under bottom ----------------------- 9
			if((m_PntSrStk[m_PntSp].top > m_PntTgTbl[Tv_TgIdx].top) && (m_PntSrStk[m_PntSp].bottom > m_PntTgTbl[Tv_TgIdx].bottom))
			{
				// check side over
				if((m_PntSrStk[m_PntSp].left <= m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right >= m_PntTgTbl[Tv_TgIdx].right))
				{
					// patch target
					m_PntTgTbl[Tv_TgIdx].bottom	=	m_PntSrStk[m_PntSp].top;
					// continue
					Tv_TgIdx ++;
					continue;
				}
			}

			// target height over than source ---------------- 10
			if((m_PntSrStk[m_PntSp].top > m_PntTgTbl[Tv_TgIdx].top) && (m_PntSrStk[m_PntSp].bottom < m_PntTgTbl[Tv_TgIdx].bottom))
			{
				// divide target
				Tv_RcTmp.left			=	m_PntTgTbl[Tv_TgIdx].left;
				Tv_RcTmp.top			=	m_PntTgTbl[Tv_TgIdx].top;
				Tv_RcTmp.right		=	m_PntTgTbl[Tv_TgIdx].right;
				Tv_RcTmp.bottom		=	m_PntSrStk[m_PntSp].top;

				// patch source
				if (m_PntSrStk[m_PntSp].left >= m_PntTgTbl[Tv_TgIdx].left)
				{
					m_PntSrStk[m_PntSp].left	=	m_PntTgTbl[Tv_TgIdx].left;
				}

				if (m_PntSrStk[m_PntSp].right <= m_PntTgTbl[Tv_TgIdx].right)
				{
					m_PntSrStk[m_PntSp].right	=	m_PntTgTbl[Tv_TgIdx].right;
				}

				// patch target
				m_PntTgTbl[Tv_TgIdx].top	=	m_PntSrStk[m_PntSp].bottom;

				// add target
				if (LcPntTgAdd(&Tv_RcTmp))
				{
					// success
					// continue
					Tv_TgIdx ++;
					continue;
				}
				// add fail
				// check source bi-side over
				if ((m_PntSrStk[m_PntSp].left < m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right > m_PntTgTbl[Tv_TgIdx].right))
				{
					// merge source with adder
					LcPntRcMerge(&m_PntSrStk[m_PntSp], &Tv_RcTmp);
					// rescan
					Tv_TgIdx	=	0;
					continue;
				}
				else if (m_PntSrStk[m_PntSp].left < m_PntTgTbl[Tv_TgIdx].left)
				{
					// left side over
					// target restore
					m_PntTgTbl[Tv_TgIdx].top	=	Tv_RcTmp.top;
					// patch source
					m_PntSrStk[m_PntSp].right	=	m_PntTgTbl[Tv_TgIdx].left;
					// continue
					Tv_TgIdx ++;
					continue;
				}
				// right side over
				// target restore
				m_PntTgTbl[Tv_TgIdx].top	=	Tv_RcTmp.top;
				// patch source
				m_PntSrStk[m_PntSp].left	=	m_PntTgTbl[Tv_TgIdx].right;
				// continue
				Tv_TgIdx ++;
				continue;
			}

			// source upper -------------------- 11
			if ((m_PntSrStk[m_PntSp].top < m_PntTgTbl[Tv_TgIdx].top) && (m_PntSrStk[m_PntSp].bottom < m_PntTgTbl[Tv_TgIdx].bottom))
			{
				// check horizontal side
				if ((m_PntSrStk[m_PntSp].left < m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right < m_PntTgTbl[Tv_TgIdx].right))
				{
					// uppder left
					// divide source bottom side
					Tv_RcTmp.left		=	m_PntSrStk[m_PntSp].left;
					Tv_RcTmp.top		=	m_PntTgTbl[Tv_TgIdx].top;
					Tv_RcTmp.right	=	m_PntTgTbl[Tv_TgIdx].right;
					Tv_RcTmp.bottom	=	m_PntSrStk[m_PntSp].bottom;

					// patch source
					m_PntSrStk[m_PntSp].bottom	=	m_PntTgTbl[Tv_TgIdx].top;

					// patch target
					m_PntTgTbl[Tv_TgIdx].top	=	Tv_RcTmp.bottom;

					// add source
					if (LcPntSrAdd(&Tv_RcTmp))
					{
						// success add
						// continue
						Tv_TgIdx ++;
						continue;
					}

					// fail add
					// merge source 
					LcPntRcMerge(&m_PntSrStk[m_PntSp], &Tv_RcTmp);

					// rescan
					Tv_TgIdx		=	0;
					continue;

				}
				else if ((m_PntSrStk[m_PntSp].left > m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right > m_PntTgTbl[Tv_TgIdx].right))
				{
					// upper right
					// divide source bottom side
					Tv_RcTmp.left		=	m_PntTgTbl[Tv_TgIdx].left;
					Tv_RcTmp.top		=	m_PntTgTbl[Tv_TgIdx].top;
					Tv_RcTmp.right	=	m_PntSrStk[m_PntSp].right;
					Tv_RcTmp.bottom	=	m_PntSrStk[m_PntSp].bottom;

					// patch source
					m_PntSrStk[m_PntSp].bottom	=	m_PntTgTbl[Tv_TgIdx].top;

					// patch target
					m_PntTgTbl[Tv_TgIdx].top	=	Tv_RcTmp.bottom;

					// add source
					if (LcPntSrAdd(&Tv_RcTmp))
					{
						// success add
						// continue
						Tv_TgIdx ++;
						continue;
					}

					// fail add
					// merge source 
					LcPntRcMerge(&m_PntSrStk[m_PntSp], &Tv_RcTmp);

					// rescan
					Tv_TgIdx		=	0;
					continue;

				}
			}

			// source under -------------------- 12
			if ((m_PntSrStk[m_PntSp].top > m_PntTgTbl[Tv_TgIdx].top) && (m_PntSrStk[m_PntSp].bottom > m_PntTgTbl[Tv_TgIdx].bottom))
			{
				// check horizontal side
				if ((m_PntSrStk[m_PntSp].left < m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right < m_PntTgTbl[Tv_TgIdx].right))
				{
					// under left
					// divide source top side
					Tv_RcTmp.left		=	m_PntSrStk[m_PntSp].left;
					Tv_RcTmp.top		=	m_PntSrStk[m_PntSp].top;
					Tv_RcTmp.right	=	m_PntTgTbl[Tv_TgIdx].right;
					Tv_RcTmp.bottom	=	m_PntTgTbl[Tv_TgIdx].bottom;

					// patch source
					m_PntSrStk[m_PntSp].top		=	m_PntTgTbl[Tv_TgIdx].bottom;

					// patch target
					m_PntTgTbl[Tv_TgIdx].bottom	=	Tv_RcTmp.top;

					// add source
					if (LcPntSrAdd(&Tv_RcTmp))
					{
						// success add
						// continue
						Tv_TgIdx ++;
						continue;
					}

					// fail add
					// merge source 
					LcPntRcMerge(&m_PntSrStk[m_PntSp], &Tv_RcTmp);

					// rescan
					Tv_TgIdx		=	0;
					continue;

				}
				else if ((m_PntSrStk[m_PntSp].left > m_PntTgTbl[Tv_TgIdx].left) && (m_PntSrStk[m_PntSp].right > m_PntTgTbl[Tv_TgIdx].right))
				{
					// under right
					// divide source top side
					Tv_RcTmp.left		=	m_PntTgTbl[Tv_TgIdx].left;
					Tv_RcTmp.top		=	m_PntSrStk[m_PntSp].top;
					Tv_RcTmp.right	=	m_PntSrStk[m_PntSp].right;
					Tv_RcTmp.bottom	=	m_PntTgTbl[Tv_TgIdx].bottom;

					// patch source
					m_PntSrStk[m_PntSp].top		=	m_PntTgTbl[Tv_TgIdx].bottom;

					// patch target
					m_PntTgTbl[Tv_TgIdx].bottom	=	Tv_RcTmp.top;

					// add source
					if (LcPntSrAdd(&Tv_RcTmp))
					{
						// success add
						// continue
						Tv_TgIdx ++;
						continue;
					}

					// fail add
					// merge source 
					LcPntRcMerge(&m_PntSrStk[m_PntSp], &Tv_RcTmp);

					// rescan
					Tv_TgIdx		=	0;
					continue;

				}
			}

			// overlapped unknown ------------------------------
			// unknown situation
			DbgMsgPrint("Cls_GrP5ConBase::LcPntArngProc - unknonw situaltion : overlapped\n");
			// not matched go next
			Tv_TgIdx ++;

		}	//	while(Tv_TgIdx < E_GrP5ConPntRcTgCnt)

		// check target add
		if(Tv_IsAddTg)
		{
			if (!LcPntTgAdd(&m_PntSrStk[m_PntSp]))
			{
				// fail add (target full..)
				// merge all

				// soruce merge
				for(Tv_TgIdx = 0; Tv_TgIdx < E_GrP5ConPntRcTgCnt; Tv_TgIdx++)
				{
					// check exist 
					if(0 != m_PntTgTbl[Tv_TgIdx].right)
					{
						// merge
						if(m_PntSrStk[m_PntSp].left > m_PntTgTbl[Tv_TgIdx].left)
						{
							m_PntSrStk[m_PntSp].left	=	m_PntTgTbl[Tv_TgIdx].left;
						}
						if(m_PntSrStk[m_PntSp].top > m_PntTgTbl[Tv_TgIdx].top)
						{
							m_PntSrStk[m_PntSp].top		=	m_PntTgTbl[Tv_TgIdx].top;
						}
						if(m_PntSrStk[m_PntSp].right < m_PntTgTbl[Tv_TgIdx].right)
						{
							m_PntSrStk[m_PntSp].right		=	m_PntTgTbl[Tv_TgIdx].right;
						}
						if(m_PntSrStk[m_PntSp].bottom < m_PntTgTbl[Tv_TgIdx].bottom)
						{
							m_PntSrStk[m_PntSp].bottom		=	m_PntTgTbl[Tv_TgIdx].bottom;
						}
					}
				}
				return;
			}
		}

		// pop source
		m_PntSp --;

}
//--------------------------------------------------------------------
__s32	Cls_GrP5ConBase::LcPntTgFindByRect(Ptr_GrRect A_PtrRc)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_FndWv;
		__u32	Tv_TgIdx;
		__s32	Tv_SrCtX;
		__s32	Tv_SrCtY;
		__s32	Tv_WkCtX;
		__s32	Tv_WkCtY;
		__s32	Tv_WkWv;
	// code --------------------
		Tv_Result	=	-1;

		// calculate source center
		Tv_SrCtX	=	(A_PtrRc->right + A_PtrRc->left) >> 1;
		Tv_SrCtY	=	(A_PtrRc->bottom + A_PtrRc->top) >> 1;

		for (Tv_TgIdx = 0; Tv_TgIdx < E_GrP5ConPntRcTgCnt; Tv_TgIdx++)
		{
			// check exist target 
			if (0 != m_PntTgTbl[Tv_TgIdx].right)
			{
				// calculate center
				Tv_WkCtX	=	(m_PntTgTbl[Tv_TgIdx].right + m_PntTgTbl[Tv_TgIdx].left) >> 1;
				Tv_WkCtY	=	(m_PntTgTbl[Tv_TgIdx].bottom + m_PntTgTbl[Tv_TgIdx].top) >> 1;
				// calculate weight value
				Tv_WkWv		=	GrAbs32(Tv_SrCtX - Tv_WkCtX) + GrAbs32(Tv_SrCtY - Tv_WkCtY);
				// check exist found
				if (0 > Tv_Result)
				{
					// update this
					Tv_Result	=	(__s32)Tv_TgIdx;
					Tv_FndWv	=	Tv_WkWv;
				}
				else
				{
					if (Tv_FndWv > Tv_WkWv)
					{
						// update this
						Tv_Result	=	(__s32)Tv_TgIdx;
						Tv_FndWv	=	Tv_WkWv;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlPaintCodeBuild(void)
{
	// local -------------------
		__u32	Tv_SltIdx;
		Cls_GrP5SkinBase*	Tv_ObjSkin;
	// code --------------------

		Tv_ObjSkin	=	GrP5SkinGet();

		// reset paint table
		GrP5PntReset();

		// build paint code
		for(Tv_SltIdx = 0; Tv_SltIdx < E_GrP5ConPntRcTgCnt; Tv_SltIdx++)
		{
			if(0 != m_PntTgTbl[Tv_SltIdx].right)
			{
				// contents
				LcPntBldByRect(&m_PntTgTbl[Tv_SltIdx]);

				// tooltip
				if(NULL != Tv_ObjSkin)
				{
					// check overlapped tooltip area
					if(E_GrP5TxtIdNone != m_Tooltip.TxtId)
					{
						if(0 != GrDrawCnChkOvlpRect(&m_Tooltip.RcDraw, &m_PntTgTbl[Tv_SltIdx]))
						{
							St_GrRect	Tv_RcClipOld;
							if(GrP5PntClipSet(&m_RcScr, &Tv_RcClipOld))
							{
								Tv_ObjSkin->TooltipDraw(&m_Tooltip);

								GrP5PntClipRestore(&Tv_RcClipOld);
							}
						}

					}
				}

				// clear
				m_PntTgTbl[Tv_SltIdx].left		=	0;
				m_PntTgTbl[Tv_SltIdx].top			=	0;
				m_PntTgTbl[Tv_SltIdx].right		=	0;
				m_PntTgTbl[Tv_SltIdx].bottom	=	0;
			}
		}

#if 0
		// tooltip
		if(NULL != Tv_ObjSkin)
		{
			// check overlapped tooltip area
			if(E_GrP5TxtIdNone != m_Tooltip.TxtId)
			{
				if(GrP5PntSectionStart(&m_Tooltip.RcDraw))
				{
					St_GrRect	Tv_RcClipOld;
					if(GrP5PntClipSet(&m_RcScr, &Tv_RcClipOld))
					{
						Tv_ObjSkin->TooltipDraw(&m_Tooltip);

						GrP5PntClipRestore(&Tv_RcClipOld);
					}
				}

			}
		}
#endif
		// reset need
		V_GrP5Env.ConTbl[m_ConIdx].IsPntNeed	=	FALSE;
		m_IsPntExist	=	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::LcDbgPntSrTgChk(void)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		if(0 <= m_PntSp)
		{
			for( Tv_WkIdx = 0; Tv_WkIdx < (__u32)(m_PntSp + 1); Tv_WkIdx++)
			{
				if((m_PntSrStk[Tv_WkIdx].left >m_PntSrStk[Tv_WkIdx].right) || (m_PntSrStk[Tv_WkIdx].top >m_PntSrStk[Tv_WkIdx].bottom))
				{
					DbgMsgPrint("[BUG] src promlem!\n");
				}
				
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5ConBase::RtlTooltipSet(Ptr_GrRect A_PtrRcBase, __u16 A_TxtId)
{
	// local -------------------
		Cls_GrP5SkinBase*	Tv_ObjSkin;
	// code --------------------

		// redraw old tooltip
		if(E_GrP5TxtIdNone != m_Tooltip.TxtId)
		{
			PaintReq(&m_Tooltip.RcDraw);
			m_Tooltip.TxtId	=	E_GrP5TxtIdNone;
		}

		Tv_ObjSkin	=	GrP5SkinGet();
		if(NULL == Tv_ObjSkin)
		{
			return;
		}

		Tv_ObjSkin->TooltipSetup( &m_RcScr,A_PtrRcBase, A_TxtId, &m_Tooltip);
		if(E_GrP5TxtIdNone != m_Tooltip.TxtId)
		{
			PaintReq(&m_Tooltip.RcDraw);
		}
}
//--------------------------------------------------------------------
