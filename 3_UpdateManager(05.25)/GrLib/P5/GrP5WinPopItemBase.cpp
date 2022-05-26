/*
Platform 5 UI window popup

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5WinPopItemBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>

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


#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5WinPopItemBase::Cls_GrP5WinPopItemBase(__u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5WinBase(E_GrP5ScptIdNone, A_ConIdx, A_LayerIdx, A_PtrRelRect, 0)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdWinPopItemBase, NULL, 0, &m_SkinCtlWinPopItemBase);

		GrDumyZeroMem(&m_SkinCtlWinPopItemBase, sizeof(m_SkinCtlWinPopItemBase));

		m_SkinCtlWinPopItemBase.CusMseIdx	=	-1;
		m_SkinCtlWinPopItemBase.CusKeyIdx	=	0;

		m_MseMode		=	E_GrP5WinPopItemMseNormal;

}
//--------------------------------------------------------------------
Cls_GrP5WinPopItemBase::~Cls_GrP5WinPopItemBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5WinPopItemBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5WinBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinPopItemBase::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5WinBase::DvlParaInfoBuild();
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinPopItemBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

	// inherited
	Cls_GrP5WinBase::DvlParaInfoAction(A_Action);

	// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinPopItemBase::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// not delete

		// delete prepare

		// inherited
		//Cls_GrP5WinBase::DvlPreDelete();

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5WinPopItemBase::EvtWinKeyFocusLoss(void)
{
	// local -------------------
	// code --------------------
		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
void	Cls_GrP5WinPopItemBase::EvtMseLeave(void)
{
	ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinPopItemBase::EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32	Tv_ScrX;
		__s32	Tv_ScrY;

		__s32	Tv_IdxX;
		__s32	Tv_IdxY;

		St_GrRect	Tv_Rc;
	// code --------------------
		// check skin able
		if(NULL == m_SkinMng)
		{
			return	TRUE;
		}

		// calculate screen position
		Tv_ScrX		=	A_X + m_RcScr.left;
		Tv_ScrY		=	A_Y + m_RcScr.top;

		// check mouse mode
		if(E_GrP5WinPopItemMseNormal != m_MseMode)
		{
			if(E_GrP5WinPopItemMsePushThumb == m_MseMode)
			{
				// change scroll bar position
				LcSclBarSetByMsePos(Tv_ScrY);
			}

			return	TRUE;
		}

		// normal mode

		// check scroll bar able
		if(0 < m_SkinCtlWinPopItemBase.Sbar.RcArea.right)
		{
			// init
			m_SkinCtlWinPopItemBase.Sbar.BtnDecStat		=	0;
			m_SkinCtlWinPopItemBase.Sbar.BtnIncStat		=	0;
			m_SkinCtlWinPopItemBase.Sbar.ThumbStat		=	0;

			// check scroll bar position
			if((Tv_ScrX <= m_SkinCtlWinPopItemBase.Sbar.RcArea.left) && (Tv_ScrX > m_SkinCtlWinPopItemBase.Sbar.RcArea.right) &&
				(Tv_ScrY <= m_SkinCtlWinPopItemBase.Sbar.RcArea.top) && (Tv_ScrY > m_SkinCtlWinPopItemBase.Sbar.RcArea.bottom))
			{
				// do scroll bar mode
				if((Tv_ScrX <= m_SkinCtlWinPopItemBase.Sbar.RcBtnDec.left) && (Tv_ScrX > m_SkinCtlWinPopItemBase.Sbar.RcBtnDec.right) &&
					(Tv_ScrY <= m_SkinCtlWinPopItemBase.Sbar.RcBtnDec.top) && (Tv_ScrY > m_SkinCtlWinPopItemBase.Sbar.RcBtnDec.bottom))
				{
					// button decrease
					m_SkinCtlWinPopItemBase.Sbar.BtnDecStat		=	E_GrP5StatOnCtrlCus;
				}
				else if((Tv_ScrX <= m_SkinCtlWinPopItemBase.Sbar.RcBtnDec.left) && (Tv_ScrX > m_SkinCtlWinPopItemBase.Sbar.RcBtnDec.right) &&
					(Tv_ScrY <= m_SkinCtlWinPopItemBase.Sbar.RcBtnDec.top) && (Tv_ScrY > m_SkinCtlWinPopItemBase.Sbar.RcBtnDec.bottom))
				{
					// button increase
					m_SkinCtlWinPopItemBase.Sbar.BtnIncStat		=	E_GrP5StatOnCtrlCus;
				}
				else
				{
					// on track
					m_SkinMng->CtlGetSbarVtThumbRc(this, &m_SkinCtlWinPopItemBase.Sbar, &Tv_Rc);
					if(0 < Tv_Rc.right)
					{
						// button thumb nail
						if((Tv_ScrX <= Tv_Rc.left) && (Tv_ScrX > Tv_Rc.right) && (Tv_ScrY <= Tv_Rc.top) && (Tv_ScrY > Tv_Rc.bottom))
						{
							m_SkinCtlWinPopItemBase.Sbar.ThumbStat		=	E_GrP5StatOnCtrlCus;
						}
					}
				}

				if(0 <= m_SkinCtlWinPopItemBase.CusMseIdx)
				{
					m_SkinCtlWinPopItemBase.CusMseIdx	=	-1;
					ReqDraw(FALSE);
				}
				else
				{
					LcPntReqByRect(&m_SkinCtlWinPopItemBase.Sbar.RcArea);
				}

				return	TRUE;
			}
		}

		// get item index
		m_SkinMng->CtlGetItemIdxByPos(this, Tv_ScrX, Tv_ScrY, &Tv_IdxX, &Tv_IdxY);

		if((0 < m_SkinCtlWinPopItemBase.Sbar.RcArea.right) &&
			((0 != m_SkinCtlWinPopItemBase.Sbar.BtnDecStat) || (0 != m_SkinCtlWinPopItemBase.Sbar.BtnIncStat) || (0 != m_SkinCtlWinPopItemBase.Sbar.ThumbStat)))
		{
			// reset
			m_SkinCtlWinPopItemBase.Sbar.BtnDecStat		=	0;
			m_SkinCtlWinPopItemBase.Sbar.BtnIncStat		=	0;
			m_SkinCtlWinPopItemBase.Sbar.ThumbStat		=	0;

			ReqDraw(FALSE);
		}
		else
		{
			if(m_SkinCtlWinPopItemBase.CusMseIdx != Tv_IdxY)
			{
				// redraw
				// old rect
				if(0 <= m_SkinCtlWinPopItemBase.CusMseIdx)
				{
					m_SkinMng->CtlGetItemOneRect(this, 0, m_SkinCtlWinPopItemBase.CusMseIdx, &Tv_Rc);
					LcPntReqByRect(&Tv_Rc);
				}

				// new rect
				if(0 <= Tv_IdxY)
				{
					m_SkinMng->CtlGetItemOneRect(this, 0, Tv_IdxY, &Tv_Rc);
					LcPntReqByRect(&Tv_Rc);
				}
				
			}
		}

		// update
		m_SkinCtlWinPopItemBase.CusMseIdx		=	Tv_IdxY;

		return	TRUE;

}
//--------------------------------------------------------------------
void	Cls_GrP5WinPopItemBase::LcSclBarSetByMsePos(__s32 A_ScrY)
{
	// local -------------------
		__s32	Tv_Pos;
	// code --------------------
		Tv_Pos		=	A_ScrY - m_SbarVtBasePos;
		m_SkinMng->CtlCalcSbarVtByPos(this, &m_SkinCtlWinPopItemBase.Sbar, Tv_Pos);
		// redraw 
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinPopItemBase::EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32	Tv_ScrX;
		__s32	Tv_ScrY;

		St_GrRect	Tv_RcThumb;
	// code --------------------
		// check skin able
		if(NULL == m_SkinMng)
		{
			return	TRUE;
		}

		// calculate screen position
		Tv_ScrX		=	A_X + m_RcScr.left;
		Tv_ScrY		=	A_Y + m_RcScr.top;

		// check scroll bar able
		if(0 < m_SkinCtlWinPopItemBase.Sbar.RcArea.right)
		{
			// init
			m_SkinCtlWinPopItemBase.Sbar.BtnDecStat		=	0;
			m_SkinCtlWinPopItemBase.Sbar.BtnIncStat		=	0;
			m_SkinCtlWinPopItemBase.Sbar.ThumbStat		=	0;

			// check scroll bar position
			if((Tv_ScrX >= m_SkinCtlWinPopItemBase.Sbar.RcArea.left) && (Tv_ScrX < m_SkinCtlWinPopItemBase.Sbar.RcArea.right) &&
				(Tv_ScrY >= m_SkinCtlWinPopItemBase.Sbar.RcArea.top) && (Tv_ScrY < m_SkinCtlWinPopItemBase.Sbar.RcArea.bottom))
			{
				// mouse capture
				MseCapture();

				// do scroll bar mode
				if((Tv_ScrX >= m_SkinCtlWinPopItemBase.Sbar.RcBtnDec.left) && (Tv_ScrX < m_SkinCtlWinPopItemBase.Sbar.RcBtnDec.right) &&
					(Tv_ScrY >= m_SkinCtlWinPopItemBase.Sbar.RcBtnDec.top) && (Tv_ScrY < m_SkinCtlWinPopItemBase.Sbar.RcBtnDec.bottom))
				{
					// button decrease
					m_MseMode	=	E_GrP5WinPopItemMsePushSclUp;
					m_SkinCtlWinPopItemBase.Sbar.BtnDecStat		=	E_GrP5StatPush;
				}
				else if((Tv_ScrX >= m_SkinCtlWinPopItemBase.Sbar.RcBtnInc.left) && (Tv_ScrX < m_SkinCtlWinPopItemBase.Sbar.RcBtnInc.right) &&
					(Tv_ScrY >= m_SkinCtlWinPopItemBase.Sbar.RcBtnInc.top) && (Tv_ScrY < m_SkinCtlWinPopItemBase.Sbar.RcBtnInc.bottom))
				{
					// button increase
					m_MseMode	=	E_GrP5WinPopItemMsePushSclDn;
					m_SkinCtlWinPopItemBase.Sbar.BtnIncStat		=	E_GrP5StatPush;
				}
				else
				{
					// on track
					m_MseMode	=	E_GrP5WinPopItemMsePushTrack;
					m_SkinMng->CtlGetSbarVtThumbRc(this, &m_SkinCtlWinPopItemBase.Sbar, &Tv_RcThumb);
					if(0 < Tv_RcThumb.right)
					{
						// button thumb nail
						if((Tv_ScrX >= Tv_RcThumb.left) && (Tv_ScrX < Tv_RcThumb.right) && (Tv_ScrY >= Tv_RcThumb.top) && (Tv_ScrY < Tv_RcThumb.bottom))
						{
							m_MseMode	=	E_GrP5WinPopItemMsePushThumb;
							m_SkinCtlWinPopItemBase.Sbar.ThumbStat		=	E_GrP5StatPush;
							m_SbarVtBasePos		=	Tv_ScrY - m_SkinCtlWinPopItemBase.Sbar.Pos;
						}
					}
				}

				if(0 <= m_SkinCtlWinPopItemBase.CusMseIdx)
				{
					m_SkinCtlWinPopItemBase.CusMseIdx	=	-1;
					ReqDraw(FALSE);
				}
				else
				{
					LcPntReqByRect(&m_SkinCtlWinPopItemBase.Sbar.RcArea);
				}
			}
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinPopItemBase::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32	Tv_ScrX;
		__s32	Tv_ScrY;

		__s32	Tv_Pos;

	// code --------------------

		// check skin able
		if(NULL == m_SkinMng)
		{
			return	TRUE;
		}

		// calculate screen position
		Tv_ScrX		=	A_X + m_RcScr.left;
		Tv_ScrY		=	A_Y + m_RcScr.top;

		if(E_GrP5WinPopItemMseNormal != m_MseMode)
		{
			MseRelese();
		}

		// check item mode
		if(E_GrP5WinPopItemMsePushSclUp == m_MseMode)
		{
			// scroll up
			m_SkinCtlWinPopItemBase.Sbar.BtnDecStat	=	0;
			if(0 != m_SkinCtlWinPopItemBase.Sbar.Val)
			{
				m_SkinMng->CtlCalcSbarVtByValue(this, &m_SkinCtlWinPopItemBase.Sbar, (__u32)m_SkinCtlWinPopItemBase.Sbar.Val - 1);
				ReqDraw(FALSE);
			}
			else
			{
				LcPntReqByRect(&m_SkinCtlWinPopItemBase.Sbar.RcBtnDec);
			}
		}
		else if(E_GrP5WinPopItemMsePushSclDn == m_MseMode)
		{
			// scroll down
			m_SkinCtlWinPopItemBase.Sbar.BtnIncStat	=	0;
			if(m_SkinCtlWinPopItemBase.Sbar.Max > m_SkinCtlWinPopItemBase.Sbar.Val)
			{
				m_SkinMng->CtlCalcSbarVtByValue(this, &m_SkinCtlWinPopItemBase.Sbar, (__u32)m_SkinCtlWinPopItemBase.Sbar.Val + 1);
				ReqDraw(FALSE);
			}
			else
			{
				LcPntReqByRect(&m_SkinCtlWinPopItemBase.Sbar.RcBtnInc);
			}
		}
		else if(E_GrP5WinPopItemMsePushTrack == m_MseMode)
		{
			// track

		}
		else if(E_GrP5WinPopItemMsePushThumb == m_MseMode)
		{
			// thumb nail
			Tv_Pos		=	Tv_ScrY - m_SbarVtBasePos;
			m_SkinMng->CtlCalcSbarVtByPos(this, &m_SkinCtlWinPopItemBase.Sbar, Tv_Pos);
			m_SkinCtlWinPopItemBase.Sbar.ThumbStat	=	0;
			ReqDraw(FALSE);
		}
		else
		{
			// normal mode
			if(0 <= m_SkinCtlWinPopItemBase.CusMseIdx)
			{
				// event item click
				EvtClickItem((__u32)m_SkinCtlWinPopItemBase.CusMseIdx);
			}
		}

		// reset mode
		m_MseMode		=	E_GrP5WinPopItemMseNormal;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinPopItemBase::IsKeyAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinPopItemBase::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------


