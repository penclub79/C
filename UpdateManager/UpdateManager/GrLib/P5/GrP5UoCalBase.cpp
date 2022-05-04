/*
Platform 5 UI object calendar base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoCalBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>

#include <P5/GrP5Key.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
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

// relate position
#define	E_GrP5UoCalBaseParaInfoCnt							2

St_GrP5DvlScptParaInfo	V_GrP5UoCalBaseParaInfo[E_GrP5UoCalBaseParaInfoCnt]	=
{
	{L"Calendar base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoCalBasePara)0)->FontId, TRUE, 0, L"Font ID"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoCalBase::Cls_GrP5UoCalBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdCalBase, &m_ParaUoCalBase, sizeof(m_ParaUoCalBase), &m_SkinCtlCalBase);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoCalBase, sizeof(m_ParaUoCalBase));
		m_ParaUoCalBase.TxtPara.Atb					=	E_GrP5FontAtbDrawText;
		m_ParaUoCalBase.TxtRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoCalBase.TxtRc.Bottom.Ratio	=	E_GrP5RelRectRatioBaseMax;

		GrDumyZeroMem(&m_SkinCtlCalBase, sizeof(m_SkinCtlCalBase));

		m_SkinCtlCalBase.CusIdxX		=	-1;
		m_SkinCtlCalBase.CusIdxY		=	-1;

		m_StartWeek	=	0;
		m_DayCnt		=	0;

}
//--------------------------------------------------------------------
Cls_GrP5UoCalBase::~Cls_GrP5UoCalBase(void)
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCalBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoCalBaseParaInfo, E_GrP5UoCalBaseParaInfoCnt, &m_ParaUoCalBase);

		DvlParaInfoTxtParaAdd(L"Text para", &m_ParaUoCalBase.TxtPara);
		DvlParaInfoRelPosAdd(L"Text Position", &m_ParaUoCalBase.TxtRc);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCalBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoCalBase::EvtScptDecDone(void)
{
	// local -------------------
#ifdef GR_P5_EDITOR
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
#endif
	// code --------------------
	
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();

#ifdef GR_P5_EDITOR
		if(((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->DvlIsEdit())
		{
			CalendarSetTime(GrTimeGet(&Tv_Mili,&Tv_IsSm));
		}
#endif

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCalBase::IsKeyAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCalBase::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCalBase::EvtKeyFocusGet(__u32 A_Para)
{
	// local -------------------
		__s32	Tv_Idx;
	// code --------------------
		if(0 != (E_GrP5FocusParaByLeft & A_Para))
		{
			m_SkinCtlCalBase.CusIdxX	=	E_GrTimeDayOfWeek - 1;
			if(0 > m_SkinCtlCalBase.CusIdxY)
			{
				m_SkinCtlCalBase.CusIdxY	=	0;
			}
			if((0 == m_SkinCtlCalBase.DayTbl[m_SkinCtlCalBase.CusIdxY][m_SkinCtlCalBase.CusIdxX].Day) && (0 != m_SkinCtlCalBase.CusIdxY))
			{
				m_SkinCtlCalBase.CusIdxY --;
			}
		}
		if(0 != (E_GrP5FocusParaByRight & A_Para))
		{
			m_SkinCtlCalBase.CusIdxX	=	0;
			if(0 > m_SkinCtlCalBase.CusIdxY)
			{
				m_SkinCtlCalBase.CusIdxY	=	0;
			}
			if((0 == m_SkinCtlCalBase.DayTbl[m_SkinCtlCalBase.CusIdxY][m_SkinCtlCalBase.CusIdxX].Day) && (0 == m_SkinCtlCalBase.CusIdxY))
			{
				m_SkinCtlCalBase.CusIdxY ++;
			}
		}
		if(0 != (E_GrP5FocusParaByUp & A_Para))
		{
			Tv_Idx		=	(__s32)m_StartWeek + (__s32)m_DayCnt - 1;
			m_SkinCtlCalBase.CusIdxY	=	Tv_Idx / E_GrTimeDayOfWeek;
			m_SkinCtlCalBase.CusIdxX	=	Tv_Idx % E_GrTimeDayOfWeek;
		}
		if(0 != (E_GrP5FocusParaByDown & A_Para))
		{
			m_SkinCtlCalBase.CusIdxY	=	0;
			m_SkinCtlCalBase.CusIdxX	=	(__s32)m_StartWeek;
		}

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCalBase::EvtKeyFocusLoss(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCalBase::EvtMseEnter(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCalBase::EvtMseLeave(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCalBase::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32	Tv_ScrX;
		__s32	Tv_ScrY;
		__s32	Tv_IdxX;
		__s32	Tv_IdxY;
		St_GrRect	Tv_Rc;
		__u8	Tv_Day;
	// code --------------------

		if(NULL == m_SkinMng)
		{
			return	FALSE;
		}

		// check position
		Tv_ScrX		=	m_RcScr.left + A_X;
		Tv_ScrY		=	m_RcScr.top + A_Y;

		m_SkinMng->CtlGetItemIdxByPos(this, Tv_ScrX, Tv_ScrY, &Tv_IdxX, &Tv_IdxY);
		if((0 > Tv_IdxX) || (0 > Tv_IdxY))
		{
			return	FALSE;
		}

		// get day
		Tv_Day	=	m_SkinCtlCalBase.DayTbl[Tv_IdxY][Tv_IdxX].Day;
		// old area clear
		if( (m_SkinCtlCalBase.ViewYear == m_SkinCtlCalBase.SelYear) && (m_SkinCtlCalBase.ViewMonth == m_SkinCtlCalBase.SelMonth) && 
			(Tv_Day != m_SkinCtlCalBase.SelDay) )
		{
			// redraw old
			if(LcDayToItemIdx(m_SkinCtlCalBase.SelDay,&Tv_IdxX,&Tv_IdxY))
			{
				m_SkinMng->CtlGetItemOneRect(this, Tv_IdxX, Tv_IdxY, &Tv_Rc);
				if(0 < Tv_Rc.right)
				{
					LcPntReqByRect(&Tv_Rc);
				}
			}
		}

		// update new value
		if((m_SkinCtlCalBase.ViewYear != m_SkinCtlCalBase.SelYear) || (m_SkinCtlCalBase.ViewMonth != m_SkinCtlCalBase.SelMonth) || (Tv_Day != m_SkinCtlCalBase.SelDay))
		{
			// get index
			if(LcDayToItemIdx(Tv_Day, &Tv_IdxX, &Tv_IdxY))
			{
				m_SkinMng->CtlGetItemOneRect(this, Tv_IdxX, Tv_IdxY, &Tv_Rc);
				if(0 < Tv_Rc.right)
				{
					LcPntReqByRect(&Tv_Rc);
					// upate select value
					m_SkinCtlCalBase.SelYear		=	m_SkinCtlCalBase.ViewYear;
					m_SkinCtlCalBase.SelMonth	=	m_SkinCtlCalBase.ViewMonth;
					m_SkinCtlCalBase.SelDay		=	Tv_Day;
					// update cursor
					m_SkinCtlCalBase.CusIdxX		=	(__s32)Tv_IdxX;
					m_SkinCtlCalBase.CusIdxY		=	(__s32)Tv_IdxY;
				}
			}
		}
		// do event
		DoEvtCalDayClick();

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCalBase::DoEvtCalDayClick(void)
{
	// local -------------------
		//Cls_GrP5UoBase*	Tv_EvtHost;
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
		__u32	Tv_Time;
	// code --------------------

		// contorl
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			// calculate time
			Tv_Time	=	GrTimeEnc(m_SkinCtlCalBase.SelYear, m_SkinCtlCalBase.SelMonth, m_SkinCtlCalBase.SelDay , 0, 0, 0);
			Tv_Ctrl->EvtCalDaySel(m_ParaUoBase.CtrlId, m_ObjId, Tv_Time);
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCalBase::LcDayToItemIdx(__u8 A_Day, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
	// code --------------------
		Tv_Result	=	FALSE;

		// check day
		if((0 != A_Day) && (31 >= A_Day))
		{
			// calculate index
			Tv_IdxX		=	(__u32)m_StartWeek + (__u32)A_Day - 1;
			Tv_IdxY		=	Tv_IdxX / E_GrTimeDayOfWeek;
			Tv_IdxX		=	Tv_IdxX % E_GrTimeDayOfWeek;

			// check match day
			if(A_Day == m_SkinCtlCalBase.DayTbl[Tv_IdxY][Tv_IdxX].Day)
			{
				// found
				*A_PtrRtIdxX	=	(__s32)Tv_IdxX;
				*A_PtrRtIdxY	=	(__s32)Tv_IdxY;
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCalBase::CalendarSetTime(__u32 A_Time)
{
	// local -------------------
		__u16	Tv_Year;
		__u8	Tv_Month;
		__u8	Tv_Day;
		__u8	Tv_Hour;
		__u8	Tv_Min;
		__u8	Tv_Sec;
	// code --------------------
		GrTimeDec(A_Time, &Tv_Year, &Tv_Month, &Tv_Day, &Tv_Hour, &Tv_Min, &Tv_Sec);
		CalendarSetDate(Tv_Year, Tv_Month);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCalBase::CalendarSetDate(__u16 A_Year, __u8 A_Month)
{
	// local -------------------
		__u8	Tv_DayIdx;
		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
	// code --------------------
		// reset table
		GrDumyZeroMem(m_SkinCtlCalBase.DayTbl, sizeof(m_SkinCtlCalBase.DayTbl));

		m_SkinCtlCalBase.ViewYear		=	A_Year;
		m_SkinCtlCalBase.ViewMonth	=	A_Month;

		// build calendar
		m_StartWeek	=	GrTimeGetWeekByDate(A_Year, A_Month, 1);
		m_DayCnt		=	GrTimeGetMonthMaxDay(A_Year, A_Month);

		Tv_IdxY			=	0;
		Tv_IdxX			=	(__u32)m_StartWeek;

		for(Tv_DayIdx = 0; Tv_DayIdx < m_DayCnt; Tv_DayIdx++)
		{
			// update 
			m_SkinCtlCalBase.DayTbl[Tv_IdxY][Tv_IdxX].Day	=	Tv_DayIdx + 1;

			Tv_IdxX ++;
			if(E_GrTimeDayOfWeek <= Tv_IdxX)
			{
				Tv_IdxX	=	0;
				Tv_IdxY ++;
			}
		}

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCalBase::CalendarGetDate(__u16* A_PtrRtYear, __u8* A_PtrRtMonth)
{
	// local -------------------
	// code --------------------
		*A_PtrRtYear	=	m_SkinCtlCalBase.ViewYear;
		*A_PtrRtMonth	=	m_SkinCtlCalBase.ViewMonth;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCalBase::EvtKeyDown(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_NewCusX;
		__s32	Tv_NewCusY;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;
		// check direction key
		switch(A_Key)
		{
			case E_GrP5KeyLeft:
				if((0 > m_SkinCtlCalBase.CusIdxX) || (0> m_SkinCtlCalBase.CusIdxY))
				{
					Tv_Result	=	LcDayToItemIdx(1, &Tv_NewCusX, &Tv_NewCusY);
				}
				else if(0 < m_SkinCtlCalBase.CusIdxX)
				{
					Tv_NewCusX	=	m_SkinCtlCalBase.CusIdxX - 1;
					Tv_NewCusY	=	m_SkinCtlCalBase.CusIdxY;
					Tv_Result		=	TRUE;
				}
				break;
			case E_GrP5KeyUp:
				if((0 > m_SkinCtlCalBase.CusIdxX) || (0> m_SkinCtlCalBase.CusIdxY))
				{
					Tv_Result	=	LcDayToItemIdx(1, &Tv_NewCusX, &Tv_NewCusY);
				}
				else if(0 < m_SkinCtlCalBase.CusIdxY)
				{
					Tv_NewCusX	=	m_SkinCtlCalBase.CusIdxX;
					Tv_NewCusY	=	m_SkinCtlCalBase.CusIdxY - 1;
					Tv_Result		=	TRUE;
				}
				break;
			case E_GrP5KeyRight:
				if((0 > m_SkinCtlCalBase.CusIdxX) || (0> m_SkinCtlCalBase.CusIdxY))
				{
					Tv_Result	=	LcDayToItemIdx(1, &Tv_NewCusX, &Tv_NewCusY);
				}
				else if(E_GrTimeDayOfWeek > (m_SkinCtlCalBase.CusIdxX + 1))
				{
					Tv_NewCusX	=	m_SkinCtlCalBase.CusIdxX + 1;
					Tv_NewCusY	=	m_SkinCtlCalBase.CusIdxY;
					Tv_Result		=	TRUE;
				}
				break;
			case E_GrP5KeyDown:
				if((0 > m_SkinCtlCalBase.CusIdxX) || (0> m_SkinCtlCalBase.CusIdxY))
				{
					Tv_Result	=	LcDayToItemIdx(1, &Tv_NewCusX, &Tv_NewCusY);
				}
				else if(E_GrP5SkinCalDayLineCnt > (m_SkinCtlCalBase.CusIdxY + 1))
				{
					Tv_NewCusX	=	m_SkinCtlCalBase.CusIdxX;
					Tv_NewCusY	=	m_SkinCtlCalBase.CusIdxY + 1;
					Tv_Result		=	TRUE;
				}
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		if(Tv_Result && (NULL != m_SkinMng))
		{
				// cursor
				if(0 != m_SkinCtlCalBase.DayTbl[Tv_NewCusY][Tv_NewCusX].Day)
				{
					// paint old
					m_SkinMng->CtlGetItemOneRect(this, m_SkinCtlCalBase.CusIdxX, m_SkinCtlCalBase.CusIdxY, &Tv_Rc);
					if(0 < Tv_Rc.right)
					{
						LcPntReqByRect(&Tv_Rc);
					}
					// update new cursor
					m_SkinCtlCalBase.CusIdxX	=	Tv_NewCusX;
					m_SkinCtlCalBase.CusIdxY	=	Tv_NewCusY;
					// paint new
					m_SkinMng->CtlGetItemOneRect(this, m_SkinCtlCalBase.CusIdxX, m_SkinCtlCalBase.CusIdxY, &Tv_Rc);
					if(0 < Tv_Rc.right)
					{
						LcPntReqByRect(&Tv_Rc);
					}
				}
				else
				{
					Tv_Result	=	FALSE;
				}
			}
		
		if(!Tv_Result)
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyDown(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCalBase::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_IdxX;
		__s32	Tv_IdxY;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;
		// check direction key
		switch(A_Key)
		{
			case E_GrP5KeyEnter:
				if((0 <= m_SkinCtlCalBase.CusIdxX) && (0 <= m_SkinCtlCalBase.CusIdxY))
				{
					Tv_Result			=	TRUE;
				}
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		if(Tv_Result && (NULL != m_SkinMng))
		{
				// select change
				// draw old select
				if((m_SkinCtlCalBase.SelYear == m_SkinCtlCalBase.ViewYear) && (m_SkinCtlCalBase.SelMonth == m_SkinCtlCalBase.ViewMonth) && LcDayToItemIdx(m_SkinCtlCalBase.SelDay, &Tv_IdxX, &Tv_IdxY))
				{
					m_SkinMng->CtlGetItemOneRect(this, Tv_IdxX, Tv_IdxY, &Tv_Rc);
					if(0 < Tv_Rc.right)
					{
						LcPntReqByRect(&Tv_Rc);
					}
				}
				// update new select
				m_SkinCtlCalBase.SelYear		= m_SkinCtlCalBase.ViewYear;
				m_SkinCtlCalBase.SelMonth	=	m_SkinCtlCalBase.ViewMonth;
				m_SkinCtlCalBase.SelDay		=	m_SkinCtlCalBase.DayTbl[m_SkinCtlCalBase.CusIdxY][m_SkinCtlCalBase.CusIdxX].Day;
				// darw new
				m_SkinMng->CtlGetItemOneRect(this, m_SkinCtlCalBase.CusIdxX, m_SkinCtlCalBase.CusIdxY, &Tv_Rc);
				if(0 < Tv_Rc.right)
				{
					LcPntReqByRect(&Tv_Rc);
				}

				DoEvtCalDayClick();
		}
		
		if(!Tv_Result)
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyUp(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s8	Cls_GrP5UoCalBase::SelectDayGet(void)
{
	// local -------------------
	// code --------------------
		return	m_SkinCtlCalBase.SelDay;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCalBase::SelectDaySet(__u8 A_Day)
{
	// local -------------------
		__s32	Tv_IdxX;
		__s32	Tv_IdxY;
		St_GrRect	Tv_Rc;
	// code --------------------
		if(NULL == m_SkinMng)
		{
			return;
		}

		// calculate day
		if((0 != m_SkinCtlCalBase.ViewYear) && (0 != m_SkinCtlCalBase.ViewMonth) && (0 != A_Day) && (31 >= A_Day))
		{
			// old area clear
			if((m_SkinCtlCalBase.ViewYear == m_SkinCtlCalBase.SelYear) && (m_SkinCtlCalBase.ViewMonth == m_SkinCtlCalBase.SelMonth) && (A_Day != m_SkinCtlCalBase.SelDay))
			{
				// redraw old
				if(LcDayToItemIdx(m_SkinCtlCalBase.SelDay, &Tv_IdxX, &Tv_IdxY))
				{
					m_SkinMng->CtlGetItemOneRect(this, Tv_IdxX, Tv_IdxY, &Tv_Rc);
					if(0 < Tv_Rc.right)
					{
						LcPntReqByRect(&Tv_Rc);
					}
				}
			}

			// update new value
			if(A_Day != m_SkinCtlCalBase.SelDay)
			{
				// get index
				if(LcDayToItemIdx(A_Day, &Tv_IdxX, &Tv_IdxY))
				{
					m_SkinMng->CtlGetItemOneRect(this, Tv_IdxX, Tv_IdxY, &Tv_Rc);
					if(0 < Tv_Rc.right)
					{
						LcPntReqByRect(&Tv_Rc);
						// upate select value
						m_SkinCtlCalBase.SelYear		=	m_SkinCtlCalBase.ViewYear;
						m_SkinCtlCalBase.SelMonth	=	m_SkinCtlCalBase.ViewMonth;
						m_SkinCtlCalBase.SelDay		=	A_Day;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrP5UoCalBase::SelectTimeGet(void)
{
	// local -------------------
	// code --------------------
		return	GrTimeEnc(m_SkinCtlCalBase.SelYear, m_SkinCtlCalBase.SelMonth, m_SkinCtlCalBase.SelDay, 0, 0, 0);
}
//--------------------------------------------------------------------
