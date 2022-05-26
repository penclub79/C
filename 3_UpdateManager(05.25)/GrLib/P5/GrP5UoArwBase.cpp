/*
Platform 5 UI object arrow base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoArwBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5Mng.h>
#include <P5/GrP5Key.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <GrMath.h>

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

#define	E_GrP5UoArwBaseParaInfoCnt							4

St_GrP5DvlScptParaInfo	V_GrP5UoArwBaseParaInfo[E_GrP5UoArwBaseParaInfoCnt]	=
{
	{L"Pannel Standard", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL},
	{L"Margin", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoArwBasePara)0)->Margin, TRUE, 0, L"Margin input area"},
	{L"MinSize", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoArwBasePara)0)->MinSize, TRUE, 0, L"Minimum arrow size"},
	{L"IsKeyCover", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoArwBasePara)0)->IsKeyCover, TRUE, 0, L"key cover mode set"},
};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoArwBase::Cls_GrP5UoArwBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdArwBase, &m_ParaUoArwBase, sizeof(m_ParaUoArwBase), &m_SkinCtlArwBase);

		// default setting
		GrDumyZeroMem(&m_ParaUoArwBase, sizeof(m_ParaUoArwBase));
		m_ParaUoArwBase.Margin	=	1;
		m_ParaUoArwBase.MinSize	=	4;

		// reset
		GrDumyZeroMem(m_ArwTbl, sizeof(m_ArwTbl));
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5SkinArwMaxCnt; Tv_WkIdx++)
		{
			m_SkinCtlArwBase.ArwItms[Tv_WkIdx].St.x		=	-1;
			m_SkinCtlArwBase.ArwItms[Tv_WkIdx].St.y		=	-1;
			m_SkinCtlArwBase.ArwItms[Tv_WkIdx].Ed.x		=	-1;
			m_SkinCtlArwBase.ArwItms[Tv_WkIdx].Ed.y		=	-1;
			//m_ArwTbl[Tv_WkIdx].Val		=	3;		// test
		}

		m_EditIdx		=	0;

		m_SkinCtlArwBase.CellxCnt	=	45;
		m_SkinCtlArwBase.CellyCnt	=	30;

		m_SkinCtlArwBase.CusIdxX	=	(__s32)m_ParaUoArwBase.Margin;
		m_SkinCtlArwBase.CusIdxY	=	(__s32)m_ParaUoArwBase.Margin;

		m_RcCus.left		=	0;
		m_RcCus.top			=	0;
		m_RcCus.right		=	0;
		m_RcCus.bottom	=	0;

		m_SkinCtlArwBase.IsKeyCvrOn	=	FALSE;

}
//--------------------------------------------------------------------
Cls_GrP5UoArwBase::~Cls_GrP5UoArwBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5UoArwBase::EvtScptDecDone(void)
{
		// upate cursor position
		m_SkinCtlArwBase.CusIdxX		=	(__s32)m_ParaUoArwBase.Margin;
		m_SkinCtlArwBase.CusIdxY		=	(__s32)m_ParaUoArwBase.Margin;

		// redraw cursor
		if((0 <= m_SkinCtlArwBase.CusIdxX) && (0 <= m_SkinCtlArwBase.CusIdxY))
		{
			LcDrawCusRect(m_SkinCtlArwBase.CusIdxX, m_SkinCtlArwBase.CusIdxY);
		}

		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoArwBase::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// separator basic info
		DvlParaInfoAdd(V_GrP5UoArwBaseParaInfo, E_GrP5UoArwBaseParaInfoCnt, &m_ParaUoArwBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoArwBase::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		// custom action
		ReqDraw(FALSE);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoArwBase::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoArwBase::IsKeyAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoArwBase::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoArwBase::EvtKeyFocusGet(__u32 A_Para)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoArwBase::EvtKeyFocusLoss(void)
{
		m_SkinCtlArwBase.IsKeyCvrOn	=	FALSE;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoArwBase::EvtMseEnter(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoArwBase::EvtMseLeave(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoArwBase::EvtKeyDown(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsCusMov;
		__s32	Tv_NewX;
		__s32	Tv_NewY;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_IsCusMov	=	FALSE;

		Tv_NewX	=	m_SkinCtlArwBase.CusIdxX;
		Tv_NewY	=	m_SkinCtlArwBase.CusIdxY;

		switch(A_Key)
		{
			case E_GrP5KeyLeft:
				// cursor left
				Tv_Result	=	FALSE;
				if((!m_ParaUoArwBase.IsKeyCover) || (m_ParaUoArwBase.IsKeyCover && m_SkinCtlArwBase.IsKeyCvrOn))
				{
					Tv_Result		=	TRUE;
					Tv_IsCusMov	=	TRUE;
					if((__s32)m_ParaUoArwBase.Margin >= m_SkinCtlArwBase.CusIdxX)
					{
						Tv_NewX	=	m_SkinCtlArwBase.CellxCnt - (__s32)m_ParaUoArwBase.Margin - 1;
						
					}
					else
					{
						Tv_NewX	=	m_SkinCtlArwBase.CusIdxX - 1;
					}
				}
				break;
			case E_GrP5KeyRight:
				Tv_Result	=	FALSE;
				if((!m_ParaUoArwBase.IsKeyCover) || (m_ParaUoArwBase.IsKeyCover && m_SkinCtlArwBase.IsKeyCvrOn))
				{
					Tv_Result		=	TRUE;
					Tv_IsCusMov	=	TRUE;
					if((__s32)(m_SkinCtlArwBase.CellxCnt - m_ParaUoArwBase.Margin) > (m_SkinCtlArwBase.CusIdxX + 1))
					{
						Tv_NewX	=	m_SkinCtlArwBase.CusIdxX + 1;
					}
					else
					{
						Tv_NewX		=	(__s32)m_ParaUoArwBase.Margin;
					}
				}
				break;
			case E_GrP5KeyUp:
				Tv_Result	=	FALSE;
				if((!m_ParaUoArwBase.IsKeyCover) || (m_ParaUoArwBase.IsKeyCover && m_SkinCtlArwBase.IsKeyCvrOn))
				{
					Tv_Result		=	TRUE;
					Tv_IsCusMov	=	TRUE;
					if((__s32)m_ParaUoArwBase.Margin >= m_SkinCtlArwBase.CusIdxY)
					{
						Tv_NewY	=	m_SkinCtlArwBase.CellyCnt - (__s32)m_ParaUoArwBase.Margin - 1;
					}
					else
					{
						Tv_NewY	=	m_SkinCtlArwBase.CusIdxY - 1;
					}
				}
				break;
			case E_GrP5KeyDown:
				Tv_Result	=	FALSE;
				if((!m_ParaUoArwBase.IsKeyCover) || (m_ParaUoArwBase.IsKeyCover && m_SkinCtlArwBase.IsKeyCvrOn))
				{
					Tv_Result		=	TRUE;
					Tv_IsCusMov	=	TRUE;
					if(0 <= m_SkinCtlArwBase.CusIdxY)
					{
						if((__s32)(m_SkinCtlArwBase.CellyCnt - m_ParaUoArwBase.Margin) > (m_SkinCtlArwBase.CusIdxY + 1))
						{
							Tv_NewY	=	m_SkinCtlArwBase.CusIdxY + 1;
						}
						else
						{
							Tv_NewY	=	(__s32)m_ParaUoArwBase.Margin;
						}
					}
				}
				break;
		}

		if(Tv_IsCusMov)
		{
			// update
			m_SkinCtlArwBase.CusIdxX	=	(__s32)Tv_NewX;
			m_SkinCtlArwBase.CusIdxY	=	(__s32)Tv_NewY;
			// repaint new
			LcDrawCusRect((__u32)m_SkinCtlArwBase.CusIdxX, (__u32)m_SkinCtlArwBase.CusIdxY);
		}

		if(!Tv_Result)
		{
			Tv_Result		=	Cls_GrP5UoBase::EvtKeyDown(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoArwBase::LcDrawCusRect(__s32 A_IdxX, __s32 A_IdxY)
{
	// local -------------------
	// code --------------------
		
#if 0
		// repaint old
		if(0 < m_RcCus.right)
		{
			LcPntReqByRect(&m_RcCus);
		}

		if(NULL == m_SkinMng)
		{
			return;
		}

		// calculate position
		m_SkinMng->CtlGetItemOneRect(this, A_IdxX, A_IdxY, &m_RcCus);

		LcPntReqByRect(&m_RcCus);
#endif

		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoArwBase::LcDrawItemRect(__u32 A_ArwIdx)
{
	// local -------------------
	// code --------------------
		// check item index
		if(E_GrP5SkinArwMaxCnt <= A_ArwIdx)
		{
			return;
		}

		// redraw
		if(0 < m_ArwTbl[A_ArwIdx].RcPnt.right)
		{
			// paint previous
			LcPntReqByRect(&m_ArwTbl[A_ArwIdx].RcPnt);
		}

		if(NULL == m_SkinMng)
		{
			return;
		}

		// check item exist
		if((0 > m_SkinCtlArwBase.ArwItms[A_ArwIdx].St.x) || (0 > m_SkinCtlArwBase.ArwItms[A_ArwIdx].St.y))
		{
			return;
		}

		if((0 > m_SkinCtlArwBase.ArwItms[A_ArwIdx].Ed.x) || (0 > m_SkinCtlArwBase.ArwItms[A_ArwIdx].Ed.y))
		{
			//start only
			m_SkinMng->CtlGetItemOneRect(this, (__s32)m_SkinCtlArwBase.ArwItms[A_ArwIdx].St.x, (__s32)m_SkinCtlArwBase.ArwItms[A_ArwIdx].St.y,
				&m_ArwTbl[A_ArwIdx].RcPnt);
		}
		else
		{
			// range
			m_SkinMng->CtlGetItemRangeRect(this, (__s32)m_SkinCtlArwBase.ArwItms[A_ArwIdx].St.x, (__s32)m_SkinCtlArwBase.ArwItms[A_ArwIdx].St.y,
				(__s32)m_SkinCtlArwBase.ArwItms[A_ArwIdx].Ed.x, (__s32)m_SkinCtlArwBase.ArwItms[A_ArwIdx].Ed.y,
				&m_ArwTbl[A_ArwIdx].RcPnt);
		}

		LcPntReqByRect(&m_ArwTbl[A_ArwIdx].RcPnt);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoArwBase::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(E_GrP5KeyEnter == A_Key)
		{
			if((!m_ParaUoArwBase.IsKeyCover) || (m_ParaUoArwBase.IsKeyCover && m_SkinCtlArwBase.IsKeyCvrOn))
			{
				// do item work
				LcDoItemWork();
			}
			else if(m_ParaUoArwBase.IsKeyCover)
			{
				m_SkinCtlArwBase.IsKeyCvrOn	=	TRUE;
				ReqDraw(FALSE);
			}
			Tv_Result	=	TRUE;
		}
		else if((E_GrP5KeyEsc == A_Key) || (E_GrP5KeyMenu == A_Key))
		{
			if(m_ParaUoArwBase.IsKeyCover && m_SkinCtlArwBase.IsKeyCvrOn)
			{
				m_SkinCtlArwBase.IsKeyCvrOn	=	FALSE;
				ReqDraw(FALSE);
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyUp(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoArwBase::LcDoItemWork(void)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		// check mode
		if(0 > m_EditIdx)
		{
			return;
		}
		
		if((((__s32)m_ParaUoArwBase.Margin > m_SkinCtlArwBase.ArwItms[m_EditIdx].St.x) || ((__s32)m_ParaUoArwBase.Margin > m_SkinCtlArwBase.ArwItms[m_EditIdx].St.y)) ||
			(((__s32)m_ParaUoArwBase.Margin <= m_SkinCtlArwBase.ArwItms[m_EditIdx].St.x) && ((__s32)m_ParaUoArwBase.Margin <= m_SkinCtlArwBase.ArwItms[m_EditIdx].St.y) &&
			((__s32)m_ParaUoArwBase.Margin <= m_SkinCtlArwBase.ArwItms[m_EditIdx].Ed.x) && ((__s32)m_ParaUoArwBase.Margin <= m_SkinCtlArwBase.ArwItms[m_EditIdx].Ed.y))
			)
		{
			// start position
			m_SkinCtlArwBase.ArwItms[m_EditIdx].St.x		=	(__s16)m_SkinCtlArwBase.CusIdxX;
			m_SkinCtlArwBase.ArwItms[m_EditIdx].St.y		=	(__s16)m_SkinCtlArwBase.CusIdxY;

			m_SkinCtlArwBase.ArwItms[m_EditIdx].Ed.x		=	-1;
			m_SkinCtlArwBase.ArwItms[m_EditIdx].Ed.y		=	-1;

		}
		else
		{
			// check range
			if((m_ParaUoArwBase.MinSize > (__u32)GrAbs32((__s32)m_SkinCtlArwBase.ArwItms[m_EditIdx].St.x - m_SkinCtlArwBase.CusIdxX)) &&
				(m_ParaUoArwBase.MinSize > (__u32)GrAbs32((__s32)m_SkinCtlArwBase.ArwItms[m_EditIdx].St.y - m_SkinCtlArwBase.CusIdxY))
				)
			{
				// not enough size
				return;
			}

			// end point
			m_SkinCtlArwBase.ArwItms[m_EditIdx].Ed.x		=	(__s16)m_SkinCtlArwBase.CusIdxX;
			m_SkinCtlArwBase.ArwItms[m_EditIdx].Ed.y		=	(__s16)m_SkinCtlArwBase.CusIdxY;
		}

		// redraw
		LcDrawItemRect((__u32)m_EditIdx);

		// contorl
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			// do 
			Tv_Ctrl->EvtArwValChanged(m_ParaUoBase.CtrlId, m_ObjId);
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoArwBase::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32		Tv_IdxX;
		__s32		Tv_IdxY;
		__s32		Tv_ScrX;
		__s32		Tv_ScrY;
	// code --------------------

		if(NULL == m_SkinMng)
		{
			return	FALSE;
		}

		Tv_ScrX		=	A_X + m_RcScr.left;
		Tv_ScrY		=	A_Y + m_RcScr.top;

		m_SkinMng->CtlGetItemIdxByPos(this, Tv_ScrX, Tv_ScrY, &Tv_IdxX, &Tv_IdxY);

		// check able
		if(((__s32)m_ParaUoArwBase.Margin <= Tv_IdxX) && ((__s32)(m_SkinCtlArwBase.CellxCnt - m_ParaUoArwBase.Margin) > Tv_IdxX) && 
			((__s32)m_ParaUoArwBase.Margin <= Tv_IdxY) && ((__s32)(m_SkinCtlArwBase.CellyCnt - m_ParaUoArwBase.Margin) > Tv_IdxY)
			)
		{
			// check cursor redraw
			if((m_SkinCtlArwBase.CusIdxX != Tv_IdxX) || (m_SkinCtlArwBase.CusIdxY != Tv_IdxY))
			{
				m_SkinCtlArwBase.CusIdxX		=	Tv_IdxX;
				m_SkinCtlArwBase.CusIdxY		=	Tv_IdxY;
				LcDrawCusRect(m_SkinCtlArwBase.CusIdxX, m_SkinCtlArwBase.CusIdxY);
			}

			// work item
			LcDoItemWork();
		}

		return	TRUE;
}
//--------------------------------------------------------------------
