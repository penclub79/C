/*
Platform 5 UI object grid base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoGridBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>
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
#define	E_GrP5UoGridBaseParaInfoCnt							2

St_GrP5DvlScptParaInfo	V_GrP5UoGridBaseParaInfo[E_GrP5UoGridBaseParaInfoCnt]	=
{
	{L"Grid base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"IsDragSel", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoGridBasePara)0)->IsDragSel, TRUE, 0, L"Drag select mode"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoGridBase::Cls_GrP5UoGridBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdGridBase, &m_ParaUoGridBase, sizeof(m_ParaUoGridBase), &m_SkinCtlGridBase);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoGridBase, sizeof(m_ParaUoGridBase));
		GrDumyZeroMem(&m_SkinCtlGridBase, sizeof(m_SkinCtlGridBase));
		m_ParaUoGridBase.IsDragSel	=	TRUE;

		m_SkinCtlGridBase.CellCntX	=	1;
		m_SkinCtlGridBase.CellCntY	=	1;

		m_SkinCtlGridBase.CusIdxX		=	-1;
		m_SkinCtlGridBase.CusIdxY		=	-1;

		m_SkinCtlGridBase.DragStX		=	-1;
		m_SkinCtlGridBase.DragStY		=	-1;
		m_SkinCtlGridBase.DragEdX		=	-1;
		m_SkinCtlGridBase.DragEdY		=	-1;

}
//--------------------------------------------------------------------
Cls_GrP5UoGridBase::~Cls_GrP5UoGridBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGridBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoGridBaseParaInfo, E_GrP5UoGridBaseParaInfoCnt, &m_ParaUoGridBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGridBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoGridBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGridBase::IsKeyAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGridBase::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridBase::EvtKeyFocusGet(__u32 A_Para)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridBase::EvtKeyFocusLoss(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridBase::EvtMseEnter(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridBase::EvtMseLeave(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridBase::DoEvtGrpItemOneSel(void)
{
	// local -------------------
		//Cls_GrP5UoBase*	Tv_EvtHost;
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------

		// contorl
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			// calculate time
			Tv_Ctrl->EvtGridItemSel(m_ParaUoBase.CtrlId, m_ObjId, (__u32)m_SkinCtlGridBase.CusIdxX, (__u32)m_SkinCtlGridBase.CusIdxY);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridBase::DoEvtGrpItemAreaSel(void)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		// contorl
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			// calculate time
			Tv_Ctrl->EvtGridItemAreaSel(m_ParaUoBase.CtrlId, m_ObjId, (__u32)m_SkinCtlGridBase.DragStX, (__u32)m_SkinCtlGridBase.DragStY,(__u32)m_SkinCtlGridBase.DragEdX,(__u32)m_SkinCtlGridBase.DragEdY);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridBase::CellXyCntGet(__u32* A_PtrRtCntX, __u32* A_PtrRtCntY)
{
		*A_PtrRtCntX	=	m_SkinCtlGridBase.CellCntX;
		*A_PtrRtCntY	=	m_SkinCtlGridBase.CellCntY;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGridBase::EvtKeyDown(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsCusMov;
		__u32	Tv_CntX;
		__u32	Tv_CntY;
		__u32	Tv_NewX;
		__u32	Tv_NewY;
	// code --------------------
		Tv_Result		=	TRUE;
		Tv_IsCusMov	=	TRUE;

		CellXyCntGet(&Tv_CntX, &Tv_CntY);
		Tv_NewX		=	0;
		if(0 <= m_SkinCtlGridBase.CusIdxX)
		{
			Tv_NewX		=	(__u32)m_SkinCtlGridBase.CusIdxX;
		}
		Tv_NewY		=	0;
		if(0 <= m_SkinCtlGridBase.CusIdxY)
		{
			Tv_NewY		=	(__u32)m_SkinCtlGridBase.CusIdxY;
		}

		switch(A_Key)
		{
			case E_GrP5KeyLeft:
				// cursor left
				if(0 <= m_SkinCtlGridBase.CusIdxX)
				{
					if(0 == m_SkinCtlGridBase.CusIdxX)
					{
						Tv_NewX	=	Tv_CntX - 1;
					}
					else
					{
						Tv_NewX	=	(__u32)m_SkinCtlGridBase.CusIdxX - 1;
					}
				}
				break;
			case E_GrP5KeyRight:
				if(0 <= m_SkinCtlGridBase.CusIdxX)
				{
					if(Tv_CntX > ((__u32)m_SkinCtlGridBase.CusIdxX + 1))
					{
						Tv_NewX	=	(__u32)m_SkinCtlGridBase.CusIdxX + 1;
					}
					else
					{
						Tv_NewX	=	0;
					}
				}
				break;
			case E_GrP5KeyUp:
				if(0 <= m_SkinCtlGridBase.CusIdxY)
				{
					if(0 == m_SkinCtlGridBase.CusIdxY)
					{
						Tv_NewY	=	Tv_CntY - 1;
					}
					else
					{
						Tv_NewY	=	(__u32)m_SkinCtlGridBase.CusIdxY - 1;
					}
				}
				break;
			case E_GrP5KeyDown:
				if(0 <= m_SkinCtlGridBase.CusIdxY)
				{
					if(Tv_CntY > ((__u32)m_SkinCtlGridBase.CusIdxY + 1))
					{
						Tv_NewY	=	(__u32)m_SkinCtlGridBase.CusIdxY + 1;
					}
					else
					{
						Tv_NewY	=	0;
					}
				}
				break;
			default:
				Tv_IsCusMov	=	FALSE;
				Tv_Result		=	Cls_GrP5UoBase::EvtKeyDown(A_Key);
				break;
		}

		if(Tv_IsCusMov)
		{
			// repaint old
			DrawReqItemRect((__u32)m_SkinCtlGridBase.CusIdxX, (__u32)m_SkinCtlGridBase.CusIdxY, (__u32)m_SkinCtlGridBase.CusIdxX, (__u32)m_SkinCtlGridBase.CusIdxY);
			// update
			m_SkinCtlGridBase.CusIdxX	=	(__s32)Tv_NewX;
			m_SkinCtlGridBase.CusIdxY	=	(__s32)Tv_NewY;
			// repaint new
			DrawReqItemRect((__u32)m_SkinCtlGridBase.CusIdxX, (__u32)m_SkinCtlGridBase.CusIdxY, (__u32)m_SkinCtlGridBase.CusIdxX, (__u32)m_SkinCtlGridBase.CusIdxY);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGridBase::EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32	Tv_ScrX;
		__s32	Tv_ScrY;
		__s32	Tv_IdxX;
		__s32	Tv_IdxY;
	// code --------------------

		if(NULL == m_SkinMng)
		{
			return	TRUE;
		}

		// check drag mode
		if((0 <= m_SkinCtlGridBase.DragEdX) && (0 <= m_SkinCtlGridBase.DragEdY))
		{
			// get drag end position
			Tv_ScrX		=	m_RcScr.left + A_X;
			Tv_ScrY		=	m_RcScr.top + A_Y;

			// patch rect
			if(Tv_ScrX < m_RcScr.left)
			{
				Tv_ScrX		=	m_RcScr.left;
			}
			if(Tv_ScrX >= m_RcScr.right)
			{
				Tv_ScrX		=	m_RcScr.right - 1;
			}
			if(Tv_ScrY < m_RcScr.top)
			{
				Tv_ScrY		=	m_RcScr.top;
			}
			if(Tv_ScrY >= m_RcScr.bottom)
			{
				Tv_ScrY		=	m_RcScr.bottom - 1;
			}

			m_SkinMng->CtlGetItemIdxByPos(this, Tv_ScrX, Tv_ScrY, &Tv_IdxX, &Tv_IdxY);
			if(0 <= Tv_IdxX)
			{
				// check diffrent last 
				if((Tv_IdxX != m_SkinCtlGridBase.DragEdX) || (Tv_IdxY != m_SkinCtlGridBase.DragEdY))
				{
					DrawReqItemDrag((__u32)m_SkinCtlGridBase.DragStX, (__u32)m_SkinCtlGridBase.DragStY, (__u32)m_SkinCtlGridBase.DragEdX, (__u32)m_SkinCtlGridBase.DragEdY);
					m_SkinCtlGridBase.DragEdX	=	Tv_IdxX;
					m_SkinCtlGridBase.DragEdY	=	Tv_IdxY;
					DrawReqItemDrag((__u32)m_SkinCtlGridBase.DragStX, (__u32)m_SkinCtlGridBase.DragStY, (__u32)m_SkinCtlGridBase.DragEdX, (__u32)m_SkinCtlGridBase.DragEdY);
				}
			}
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGridBase::EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32	Tv_ScrX;
		__s32	Tv_ScrY;
		__s32	Tv_IdxX;
		__s32	Tv_IdxY;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::EvtMseLbtnDown(A_X, A_Y, A_BtnMap);

		if(NULL == m_SkinMng)
		{
			return	TRUE;
		}

		Tv_IdxX	=	-1;
		Tv_IdxY	=	-1;

		if(m_ParaUoGridBase.IsDragSel)
		{
			// get drag start position
			Tv_ScrX		=	m_RcScr.left + A_X;
			Tv_ScrY		=	m_RcScr.top + A_Y;

			m_SkinMng->CtlGetItemIdxByPos(this, Tv_ScrX, Tv_ScrY, &Tv_IdxX, &Tv_IdxY);
			if(0 <= Tv_IdxX)
			{
				// set capture
				MseCapture();
			}
		}

		m_SkinCtlGridBase.DragStX	=	Tv_IdxX;
		m_SkinCtlGridBase.DragStY	=	Tv_IdxY;
		m_SkinCtlGridBase.DragEdX	=	Tv_IdxX;
		m_SkinCtlGridBase.DragEdY	=	Tv_IdxY;

		DrawReqItemRect((__u32)m_SkinCtlGridBase.DragStX, (__u32)m_SkinCtlGridBase.DragStY, (__u32)m_SkinCtlGridBase.DragEdX, (__u32)m_SkinCtlGridBase.DragEdY);

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGridBase::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32	Tv_ScrX;
		__s32	Tv_ScrY;
		__s32	Tv_IdxX;
		__s32	Tv_IdxY;
	// code --------------------

		if(NULL == m_SkinMng)
		{
			return	TRUE;
		}

		// check drag mode
		if((0 <= m_SkinCtlGridBase.DragEdX) && (0 <= m_SkinCtlGridBase.DragEdY))
		{
			MseRelese();
			DrawReqItemRect((__u32)m_SkinCtlGridBase.DragStX, (__u32)m_SkinCtlGridBase.DragStY, (__u32)m_SkinCtlGridBase.DragEdX, (__u32)m_SkinCtlGridBase.DragEdY);
			// do area select event
			if((m_SkinCtlGridBase.DragStX != m_SkinCtlGridBase.DragEdX) || (m_SkinCtlGridBase.DragStY != m_SkinCtlGridBase.DragEdY))
			{
				DoEvtGrpItemAreaSel();
			}
			
		}

		// cursor
		Tv_ScrX		=	m_RcScr.left + A_X;
		Tv_ScrY		=	m_RcScr.top + A_Y;
		m_SkinMng->CtlGetItemIdxByPos(this, Tv_ScrX, Tv_ScrY, &Tv_IdxX, &Tv_IdxY);
		if(0 <= Tv_IdxX)
		{
			// check change cursor
			if((m_SkinCtlGridBase.CusIdxX != Tv_IdxX) || (m_SkinCtlGridBase.CusIdxY != Tv_IdxY))
			{
				// calculate old rect
				DrawReqItemRect((__u32)m_SkinCtlGridBase.CusIdxX, (__u32)m_SkinCtlGridBase.CusIdxY, (__u32)m_SkinCtlGridBase.CusIdxX, (__u32)m_SkinCtlGridBase.CusIdxY);

				// update cursor
				m_SkinCtlGridBase.CusIdxX	=	(__s32)Tv_IdxX;
				m_SkinCtlGridBase.CusIdxY	=	(__s32)Tv_IdxY;

				DrawReqItemRect((__u32)m_SkinCtlGridBase.CusIdxX, (__u32)m_SkinCtlGridBase.CusIdxY, (__u32)m_SkinCtlGridBase.CusIdxX, (__u32)m_SkinCtlGridBase.CusIdxY);
			}

			// update event
			m_SkinCtlGridBase.DragEdX	=	m_SkinCtlGridBase.CusIdxX;
			m_SkinCtlGridBase.DragEdY	=	m_SkinCtlGridBase.CusIdxY;
			if((m_SkinCtlGridBase.DragStX == m_SkinCtlGridBase.DragEdX) && (m_SkinCtlGridBase.DragStY == m_SkinCtlGridBase.DragEdY))
			{
				// do one select event
				DoEvtGrpItemOneSel();
			}
		}
		else
		{
			m_SkinCtlGridBase.CusIdxX	=	-1;
			m_SkinCtlGridBase.CusIdxY	=	-1;
		}

		// reset
		m_SkinCtlGridBase.DragStX	=	-1;
		m_SkinCtlGridBase.DragStY	=	-1;
		m_SkinCtlGridBase.DragEdX	=	-1;
		m_SkinCtlGridBase.DragEdY	=	-1;

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridBase::DrawReqItemRect(__u32 A_IdxStX, __u32 A_IdxStY, __u32 A_IdxEdX, __u32 A_IdxEdY)
{
	// local -------------------
		St_GrRect	Tv_RcPnt;
	// code --------------------
		if(NULL == m_SkinMng)
		{
			return;
		}

		m_SkinMng->CtlGetItemRangeRect(this, (__s32)A_IdxStX, (__s32)A_IdxStY, (__s32)A_IdxEdX, (__s32)A_IdxEdY, &Tv_RcPnt);
		if(0 < Tv_RcPnt.right)
		{
			LcPntReqByRect(&Tv_RcPnt);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridBase::DrawReqItemWaku(__u32 A_IdxStX, __u32 A_IdxStY, __u32 A_IdxEdX, __u32 A_IdxEdY)
{
	// local -------------------
		St_GrRect	Tv_RcPnt;
	// code --------------------

		// left
		m_SkinMng->CtlGetItemRangeRect(this, (__s32)A_IdxStX, (__s32)A_IdxStY, (__s32)A_IdxStX, (__s32)A_IdxEdY, &Tv_RcPnt);
		if(0 < Tv_RcPnt.right)
		{
			LcPntReqByRect(&Tv_RcPnt);
		}

		// right
		if(A_IdxStX != A_IdxEdX)
		{
			m_SkinMng->CtlGetItemRangeRect(this, (__s32)A_IdxEdX, (__s32)A_IdxStY, (__s32)A_IdxEdX, (__s32)A_IdxEdY, &Tv_RcPnt);
			if(0 < Tv_RcPnt.right)
			{
				LcPntReqByRect(&Tv_RcPnt);
			}
		}

		if(2 <= (A_IdxEdX - A_IdxStX))
		{
			// up
			m_SkinMng->CtlGetItemRangeRect(this, (__s32)A_IdxStX + 1, (__s32)A_IdxStY, (__s32)A_IdxEdX - 1, (__s32)A_IdxStY, &Tv_RcPnt);
			if(0 < Tv_RcPnt.right)
			{
				LcPntReqByRect(&Tv_RcPnt);
			}

			if(A_IdxStY != A_IdxEdY)
			{
				m_SkinMng->CtlGetItemRangeRect(this, (__s32)A_IdxStX + 1, (__s32)A_IdxEdY, (__s32)A_IdxEdX - 1, (__s32)A_IdxEdY, &Tv_RcPnt);
				if(0 < Tv_RcPnt.right)
				{
					LcPntReqByRect(&Tv_RcPnt);
				}
			}
		}


}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridBase::DrawReqItemDrag(__u32 A_IdxStX, __u32 A_IdxStY, __u32 A_IdxEdX, __u32 A_IdxEdY)
{
	// local -------------------
		St_GrRect	Tv_RcBase;
		St_GrRect	Tv_RcPnt;
	// code --------------------
		m_SkinMng->CtlGetItemRangeRect(this, (__s32)A_IdxStX, (__s32)A_IdxStY, (__s32)A_IdxEdX, (__s32)A_IdxEdY, &Tv_RcBase);
		if(0 < Tv_RcBase.right)
		{
			// left	
			Tv_RcPnt.left		=	Tv_RcBase.left;
			Tv_RcPnt.top		=	Tv_RcBase.top;
			Tv_RcPnt.right	=	Tv_RcPnt.left + 1;
			Tv_RcPnt.bottom	=	Tv_RcBase.bottom;
			LcPntReqByRect(&Tv_RcPnt);
			// right
			Tv_RcPnt.left		=	Tv_RcBase.right - 1;
			Tv_RcPnt.right	=	Tv_RcPnt.left + 1;
			LcPntReqByRect(&Tv_RcPnt);
			// top
			Tv_RcPnt.left		=	Tv_RcBase.left + 1;
			Tv_RcPnt.right	=	Tv_RcBase.right - 1;
			Tv_RcPnt.bottom	=	Tv_RcPnt.top + 1;
			LcPntReqByRect(&Tv_RcPnt);
			// bottom
			Tv_RcPnt.top		=	Tv_RcBase.bottom - 1;
			Tv_RcPnt.bottom	=	Tv_RcPnt.top + 1;
			LcPntReqByRect(&Tv_RcPnt);

		}

}
//--------------------------------------------------------------------
