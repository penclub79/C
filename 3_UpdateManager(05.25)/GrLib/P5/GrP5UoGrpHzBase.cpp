/*
Platform 5 UI object graph horizontal base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoGrpHzBase.h>

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
#define	E_GrP5UoGrpHzBaseParaInfoCnt							4

St_GrP5DvlScptParaInfo	V_GrP5UoGrpHzBaseParaInfo[E_GrP5UoGrpHzBaseParaInfoCnt]	=
{
	{L"Graph Horizontal base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"LineHeight", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGrpHzBasePara)0)->LineHeight, TRUE, 0, L"Line height"},
	{L"IsDoExistOnly", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoGrpHzBasePara)0)->IsDoExistOnly, TRUE, 0, L"Only works on existing items"},
	{L"IsEvtOutRange", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoGrpHzBasePara)0)->IsEvtOutRange, TRUE, 0, L"Do event when cursor position is on the out of range"},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoGrpHzBase::Cls_GrP5UoGrpHzBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdGrpHzBase, &m_ParaUoGrpHzBase, sizeof(m_ParaUoGrpHzBase), &m_SkinCtlGrpHzBase);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoGrpHzBase, sizeof(m_ParaUoGrpHzBase));
		GrDumyZeroMem(&m_SkinCtlGrpHzBase, sizeof(m_SkinCtlGrpHzBase));
		m_ParaUoGrpHzBase.LineHeight	=	20;

		m_SkinCtlGrpHzBase.SelIdxX			=	-1;
		m_SkinCtlGrpHzBase.SelIdxY			=	-1;

		m_SkinCtlGrpHzBase.CusIdxX			=	0;
		m_SkinCtlGrpHzBase.CusIdxY			=	0;

		m_SkinCtlGrpHzBase.CellCntX				=	0;
		m_SkinCtlGrpHzBase.CellCntY				=	0;

}
//--------------------------------------------------------------------
Cls_GrP5UoGrpHzBase::~Cls_GrP5UoGrpHzBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGrpHzBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoGrpHzBaseParaInfo, E_GrP5UoGrpHzBaseParaInfoCnt, &m_ParaUoGrpHzBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGrpHzBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGrpHzBase::IsKeyAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGrpHzBase::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::EvtKeyFocusGet(__u32 A_Para)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::EvtKeyFocusLoss(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::EvtMseEnter(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::EvtMseLeave(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGrpHzBase::EvtKeyDown(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_CusX;
		__s32	Tv_CusY;
	// code --------------------
		Tv_Result	=	TRUE;

		if(E_GrP5KeyLeft == A_Key)
		{
			if((0 >= m_SkinCtlGrpHzBase.CusIdxX) && m_ParaUoGrpHzBase.IsEvtOutRange)
			{
				// do event
				LcDoEvtCusOver(0);
			}
			else
			{
				// move cursor left
				Tv_CusX		=	m_SkinCtlGrpHzBase.CusIdxX;
				Tv_CusY		=	m_SkinCtlGrpHzBase.CusIdxY;

				if(0 >= Tv_CusX)
				{
					Tv_CusX	=	m_SkinCtlGrpHzBase.CellCntX - 1;
				}
				else
				{
					Tv_CusX --;
				}

				LcDoCusPosChg(Tv_CusX, Tv_CusY);
			}
		}
		else if(E_GrP5KeyRight == A_Key)
		{
			if(((__s32)m_SkinCtlGrpHzBase.CellCntX <= (m_SkinCtlGrpHzBase.CusIdxX + 1)) && m_ParaUoGrpHzBase.IsEvtOutRange)
			{
				// do event
				LcDoEvtCusOver(1);
			}
			else
			{
				// move cursor right
				Tv_CusX		=	m_SkinCtlGrpHzBase.CusIdxX;
				Tv_CusY		=	m_SkinCtlGrpHzBase.CusIdxY;

				if((__s32)m_SkinCtlGrpHzBase.CellCntX <= (Tv_CusX + 1))
				{
					Tv_CusX	=	0;
				}
				else
				{
					Tv_CusX ++;
				}

				LcDoCusPosChg(Tv_CusX, Tv_CusY);
			}
		}
		else
		{
			Tv_Result	=	FALSE;
		}

		if(!Tv_Result)
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyDown(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGrpHzBase::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		
		if(E_GrP5KeyEnter == A_Key)
		{
			// do select
			if((0 <= m_SkinCtlGrpHzBase.CusIdxX) && ((__s32)m_SkinCtlGrpHzBase.CellCntX > m_SkinCtlGrpHzBase.CusIdxX))
			{
				m_SkinCtlGrpHzBase.SelIdxX		=	m_SkinCtlGrpHzBase.CusIdxX;
				DoEvtGrpItemClick();
			}
		}
		else
		{
			Tv_Result	=	FALSE;
		}

		if(!Tv_Result)
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyUp(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGrpHzBase::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
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

		// check position
		Tv_ScrX		=	m_RcScr.left + A_X;
		Tv_ScrY		=	m_RcScr.top + A_Y;

		m_SkinMng->CtlGetItemIdxByPos(this, Tv_ScrX, Tv_ScrY, &Tv_IdxX, &Tv_IdxY);
		if((0 <= Tv_IdxX) && (0 <= Tv_IdxY))
		{
			// do day click event
			m_SkinCtlGrpHzBase.SelIdxX	=	(__s32)Tv_IdxX;
			m_SkinCtlGrpHzBase.SelIdxY	=	(__s32)Tv_IdxY;

			LcDoCusPosChg((__s32)Tv_IdxX, (__s32)Tv_IdxY);

			DoEvtGrpItemClick();
		}
		else
		{
			m_SkinCtlGrpHzBase.SelIdxX	=	-1;
			m_SkinCtlGrpHzBase.SelIdxY	=	-1;
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::DoEvtGrpItemClick(void)
{
	// local -------------------
		//Cls_GrP5UoBase*	Tv_EvtHost;
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		// check mode
		if((!m_ParaUoGrpHzBase.IsDoExistOnly) || (0 != ItemValueGet((__u32)m_SkinCtlGrpHzBase.SelIdxX, (__u32)m_SkinCtlGrpHzBase.SelIdxY)))
		{
			// contorl
			Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
			if(NULL != Tv_Ctrl)
			{
				// calculate time
				Tv_Ctrl->EvtGraphItemSel(m_ParaUoBase.CtrlId, m_ObjId, (__u32)m_SkinCtlGrpHzBase.SelIdxX, (__u32)m_SkinCtlGrpHzBase.SelIdxY);
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGrpHzBase::LcCursorVtCalcRect(__u32 A_IdxX, Ptr_GrRect A_PtrRtRc)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		if(NULL == m_SkinMng)
		{
			return	FALSE;
		}

		Tv_Result	=	FALSE;

		if(A_IdxX < m_SkinCtlGrpHzBase.CellCntX)
		{
			m_SkinMng->CtlGetItemRangeRect(this, (__s32)A_IdxX, 0, (__s32)A_IdxX, m_SkinCtlGrpHzBase.CellCntY - 1, A_PtrRtRc);
			
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::LcRedrawCusArea(__u32 A_IdxX, __u32 A_IdxY)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		if(LcCursorVtCalcRect(A_IdxX,&Tv_Rc))
		{
			LcPntReqByRect(&Tv_Rc);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::SelectIdxGet(__s32* A_PtrRtSelX, __s32* A_PtrRtSelY)
{
	// local -------------------
	// code --------------------
		*A_PtrRtSelX	=	m_SkinCtlGrpHzBase.SelIdxX;
		*A_PtrRtSelY	=	m_SkinCtlGrpHzBase.SelIdxY;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::SelectIdxSet(__s32 A_IdxX, __s32 A_IdxY)
{
		m_SkinCtlGrpHzBase.SelIdxX	=	A_IdxX;
		m_SkinCtlGrpHzBase.SelIdxY	=	A_IdxY;

		if((0 <= A_IdxX) && (0 <= A_IdxY))
		{
			if((m_SkinCtlGrpHzBase.CusIdxX != A_IdxX) || (m_SkinCtlGrpHzBase.CusIdxY != A_IdxY))
			{
				// paint old
				LcRedrawCusArea((__u32)m_SkinCtlGrpHzBase.CusIdxX,(__u32)m_SkinCtlGrpHzBase.CusIdxY);
				m_SkinCtlGrpHzBase.CusIdxX	=	A_IdxX;
				m_SkinCtlGrpHzBase.CusIdxY	=	A_IdxY;
				LcRedrawCusArea((__u32)m_SkinCtlGrpHzBase.CusIdxX, (__u32)m_SkinCtlGrpHzBase.CusIdxY);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::LcDoCusPosChg(__s32 A_X, __s32 A_Y)
{
	// local -------------------
	// code --------------------
		LcRedrawCusArea(m_SkinCtlGrpHzBase.CusIdxX, m_SkinCtlGrpHzBase.CusIdxY);

		m_SkinCtlGrpHzBase.CusIdxX	=	A_X;
		m_SkinCtlGrpHzBase.CusIdxY	=	A_Y;

		LcRedrawCusArea(m_SkinCtlGrpHzBase.CusIdxX, m_SkinCtlGrpHzBase.CusIdxY);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::DoExistOnlySet(BOOL8 A_IsOn)
{
		m_ParaUoGrpHzBase.IsDoExistOnly	=	A_IsOn;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::LcDoEvtCusOver(__u32 A_Dir)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		// contorl
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			// calculate time
			Tv_Ctrl->EvtGraphCusOver(m_ParaUoBase.CtrlId, m_ObjId, A_Dir);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzBase::CusPosGet(__s32* A_PtrRtX, __s32* A_PtrRtY)
{
		*A_PtrRtX		=	m_SkinCtlGrpHzBase.CusIdxX;
		*A_PtrRtY		=	m_SkinCtlGrpHzBase.CusIdxY;
}
//--------------------------------------------------------------------
