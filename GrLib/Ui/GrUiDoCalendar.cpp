/*
 calendar

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoCalendar.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrTimeTool.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>
#include	<Ui/GrUiDoSpinNum.h>
#include	<Ui/GrUiDoSpinTxt.h>

//====================================================================
//local const
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*				V_GrUiMngObj;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoCalendar::Cls_GrUiDoCalendar( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		m_TypeId			=	E_GrUiDoTypeIdCalendar;
		m_IsFocusAble	=	TRUE;

		GrDumyZeroMem( &m_TxtPara, sizeof(m_TxtPara) );
		m_TxtPara.ClrShadow	=	m_ObjConsole->ClrBlackGet();
		GrDumyZeroMem( m_CalClrTbl, sizeof(m_CalClrTbl) );
		m_CalClrTbl[E_GrUiDoCalendarClrTxtNrm]	=	m_ObjConsole->ClrWhiteGet();
		m_CalClrTbl[E_GrUiDoCalendarClrTxtCus]	=	m_ObjConsole->ClrWhiteGet();

		m_ItemSizeX			=	E_GrUiDoCalendarDfltItmWidth;
		m_ItemSizeY			=	E_GrUiDoCalendarDfltItmHeight;
		m_ItemGapX			=	0;
		m_ItemGapY			=	0;

		m_SelGap				=	0;
		m_SelDepth			=	0;
		m_CusGap				=	0;
		m_CusDepth			=	0;

		GrDumyZeroMem( m_CalSkinTbl, sizeof(m_CalSkinTbl) );

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoCalendarStatCount;Tv_WkIdx++ )
		{
			m_StatClrTbl[Tv_WkIdx]	=	0;
		}
		
		m_EvtHostId		=	E_GrUiObjIdNone;
		m_SpinYearId	=	E_GrUiObjIdNone;
		m_SpinMonthId	=	E_GrUiObjIdNone;

		//user contorl
		m_Year		=	2009;
		m_Month		=	1;
		m_Cus			=	1;

		m_SelYear		=	0;
		m_SelMonth	=	0;
		m_SelDay		=	0;

		//runtime
		LcMarkTblReset();
#ifdef	GR_OSD_EDIT 
		m_MarkTbl[0]	=	1;
		m_MarkTbl[1]	=	2;
		m_MarkTbl[2]	=	3;
		m_MarkTbl[3]	=	4;
		m_MarkTbl[4]	=	5;
		m_MarkTbl[5]	=	6;
		m_MarkTbl[6]	=	7;
		m_MarkTbl[7]	=	8;
#endif
		
		m_WeekPos	=	0;
		m_MaxDay	=	0;

		LcDateEnvUpdate();
}
//--------------------------------------------------------------------
Cls_GrUiDoCalendar::~Cls_GrUiDoCalendar()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::LcDateEnvUpdate( void )
{
	// local -------------------
	// code --------------------
		//patch year and month
		if ( E_GrTimeBaseYear > m_Year)
		{
			m_Year	=	E_GrTimeBaseYear;
		}
		if ( E_GrTimeMaxYear <= m_Year )
		{
			m_Year	=	E_GrTimeMaxYear - 1;
		}
		if ( 1 > m_Month )
		{
			m_Month	=	1;
		}
		if ( 12 < m_Month )
		{
			m_Month	=	12;
		}

		//get max day count
		m_MaxDay	=	GrTimeGetMonthMaxDay( m_Year, m_Month );
		//get start week pos
		m_WeekPos	=	GrTimeGetWeekByDate( m_Year, m_Month, 1 );

}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::LcPaintContents( void )
{
	// local -------------------
		St_GrRect		Tv_RcItm;
		__u8	Tv_SkipCnt;
		__u8	Tv_Day;
		__u32	Tv_WkX;
		Def_WstrNum	Tv_StrDay;
		__u8	Tv_Digit;
		__u32	Tv_Color;
		St_GrRect	Tv_RcDraw;
		__u8	Tv_WkIdx;
		Ptr_GrGdib	Tv_PtrGdib;
		__u8	Tv_ClrIdx;
	// code --------------------
		Tv_SkipCnt		=	m_WeekPos;
		Tv_RcItm.top		=	m_PntRcClient.top;
		Tv_RcItm.bottom	=	Tv_RcItm.top + (__s32)m_ItemSizeY;
		Tv_Day		=	1;
		Tv_Digit	=	0;
		Tv_StrDay[0]	=	L'1';
		Tv_StrDay[1]	=	0;
		Tv_StrDay[2]	=	0;
		while ( Tv_Day <= m_MaxDay )
		{
			//work x
			Tv_RcItm.left		=	m_PntRcClient.left;
			Tv_RcItm.right	=	Tv_RcItm.left + (__s32)m_ItemSizeX;
			for ( Tv_WkX=0;Tv_WkX < 7;Tv_WkX++ )
			{
				//check skip mode
				if ( 0 == Tv_SkipCnt )
				{
					//check draw able
					if ( Tv_Day <= m_MaxDay )
					{
						//draw
						if ( PaintClipperSet( TRUE, &Tv_RcItm ) )
						{
							//draw marker
							Tv_ClrIdx	=	m_MarkTbl[Tv_Day-1];
							if ( 0 != Tv_ClrIdx )
							{
								//mark color draw
								Tv_ClrIdx	=	Tv_ClrIdx - 1 + E_GrUiDoCalendarClrMark0;
								m_GdibScr->FillRect( &Tv_RcItm, m_CalClrTbl[Tv_ClrIdx] );
								//mark skin draw
								Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_CalSkinTbl[E_GrUiDoCalenderSkinMark].Id );
								if ( NULL != Tv_PtrGdib )
								{
									SkinDrawByPos( m_CalSkinTbl[E_GrUiDoCalenderSkinMark].Atb, Tv_PtrGdib, &Tv_RcItm );
								}
							}

							//draw select skin
							if ( (m_Year == m_SelYear) && (m_Month == m_SelMonth) && (Tv_Day == m_SelDay) )
							{
								Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_CalSkinTbl[E_GrUiDoCalenderSkinSel].Id );
								if ( NULL != Tv_PtrGdib )
								{
									SkinDrawByPos( m_CalSkinTbl[E_GrUiDoCalenderSkinSel].Atb, Tv_PtrGdib, &Tv_RcItm );
								}
							}

							//draw cursor
							if ( Tv_Day == m_Cus )
							{
								//check cursor draw able
								if ( IsFocused() )
								{
									Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_CalSkinTbl[E_GrUiDoCalenderSkinCus].Id );
									if ( NULL != Tv_PtrGdib )
									{
										SkinDrawByPos( m_CalSkinTbl[E_GrUiDoCalenderSkinCus].Atb, Tv_PtrGdib, &Tv_RcItm );
									}
								}
							}

							//calculate 
							Tv_RcDraw		=	Tv_RcItm;
							Tv_RcDraw.left		=	Tv_RcDraw.left + (__s32)m_SelGap;
							Tv_RcDraw.top			=	Tv_RcDraw.top + (__s32)m_SelGap;
							Tv_RcDraw.right		=	Tv_RcDraw.right - (__s32)m_SelGap;
							Tv_RcDraw.bottom	=	Tv_RcDraw.bottom - (__s32)m_SelGap;
							//draw selector
							if ( 0 != m_SelDepth )
							{
								for ( Tv_WkIdx=0;Tv_WkIdx < m_SelDepth;Tv_WkIdx++ )
								{
									//check select
									if ( (m_Year == m_SelYear) && (m_Month == m_SelMonth) && (Tv_Day == m_SelDay) )
									{ 
										m_GdibScr->DrawRect( &Tv_RcDraw, m_CalClrTbl[E_GrUiDoCalendarClrSelect] );
									}
									//next
									Tv_RcDraw.left ++;
									Tv_RcDraw.top ++;
									Tv_RcDraw.right --;
									Tv_RcDraw.bottom --;
								}
							}
							//update gap
							Tv_RcDraw.left		=	Tv_RcDraw.left + (__s32)m_CusGap;
							Tv_RcDraw.top			=	Tv_RcDraw.top + (__s32)m_CusGap;
							Tv_RcDraw.right		=	Tv_RcDraw.right - (__s32)m_CusGap;
							Tv_RcDraw.bottom	=	Tv_RcDraw.bottom - (__s32)m_CusGap;
							//draw cursor
							if ( (Tv_Day == m_Cus) && (0 != m_CusDepth) )
							{
								for ( Tv_WkIdx=0;Tv_WkIdx < m_CusDepth;Tv_WkIdx++ )
								{
									//draw
									m_GdibScr->DrawRect( &Tv_RcDraw, m_CalClrTbl[E_GrUiDoCalendarClrCursor] );
									//next
									Tv_RcDraw.left ++;
									Tv_RcDraw.top ++;
									Tv_RcDraw.right --;
									Tv_RcDraw.bottom --;
								}
							}
							
							//check text color
							Tv_Color	=	m_CalClrTbl[E_GrUiDoCalendarClrTxtNrm];
							if ( Tv_Day == m_Cus )
							{
								Tv_Color	=	m_CalClrTbl[E_GrUiDoCalendarClrTxtCus];
							}
							//draw text
							TextDraw( Tv_StrDay, &m_TxtPara, Tv_Color, &Tv_RcItm );
						}
						//next
						Tv_StrDay[Tv_Digit] ++;
						if ( 0x39 < Tv_StrDay[Tv_Digit] )
						{
							if ( 0 == Tv_Digit )
							{
								Tv_StrDay[0]	=	L'1';
								Tv_StrDay[1]	=	L'0';
								Tv_Digit ++;
							}
							else
							{
								Tv_StrDay[0] ++;
								Tv_StrDay[1]	=	L'0';
							}
						}
					}
					//next
					Tv_Day ++;
				}
				else
				{
					Tv_SkipCnt--;
				}
				//next positon
				Tv_RcItm.left		=	Tv_RcItm.right + (__s32)m_ItemGapX;
				Tv_RcItm.right	=	Tv_RcItm.left + (__s32)m_ItemSizeX;
			}
			//next y position
			Tv_RcItm.top		=	Tv_RcItm.bottom + (__s32)m_ItemGapY;
			Tv_RcItm.bottom	=	Tv_RcItm.top + (__s32)m_ItemSizeY;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::LcStatIdxUpdate( void )
{
		m_StatIdx		=	E_GrUiDoCalendarStatDisable;
		if ( IsCanEnable() )
		{
			m_StatIdx		=	E_GrUiDoCalendarStatNormal;
			if ( IsFocused() || IsOnMouse() )
			{
				m_StatIdx		=	E_GrUiDoCalendarStatFocus;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::ProcBgColorDraw( void )
{
	// local -------------------
	// code --------------------
		//set background color
		LcStatIdxUpdate();
		m_ClrBg		=	m_StatClrTbl[m_StatIdx];
		Cls_GrUiDoSkin::ProcBgColorDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::ProcBgSkinTblDraw( void )
{
	// local -------------------
	// code --------------------
		Cls_GrUiDoSkin::ProcBgSkinTblDraw();
		//draw
		LcPaintContents();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::LcGetDayByPos( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		__u8	Tv_Result;
		__s32		Tv_ItmUnit;
		__s32		Tv_PosX;
		__s32		Tv_PosY;
		__s32		Tv_Day;
	// code --------------------
		Tv_Result	=	0;		
		if ( m_IsPntCliAble )
		{
			//check in client rect
			if (	(m_PntRcClient.left <= A_X) && (m_PntRcClient.right > A_X) && 
						(m_PntRcClient.top <= A_Y) && (m_PntRcClient.bottom > A_Y)	)
			{
				Tv_ItmUnit	=	(__s32)m_ItemSizeX + (__s32)m_ItemGapX;
				Tv_PosX			=	(A_X - m_PntRcClient.left) / Tv_ItmUnit;
				if ( 7 >= Tv_PosX )				// check over range
				{
					Tv_ItmUnit	=	(__s32)m_ItemSizeY + (__s32)m_ItemGapY;
					Tv_PosY			=	(A_Y - m_PntRcClient.top) / Tv_ItmUnit;
					Tv_Day			=	(Tv_PosY * 7) + Tv_PosX - (__s32)m_WeekPos + 1;
					if ( (0 < Tv_Day) && (m_MaxDay >= (__u8)Tv_Day) )
					{
						Tv_Result	=	(__u8)Tv_Day;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoCalendar::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		Cls_GrUiDoSkin::ProcEvtMseLbtnDown( A_X, A_Y, A_BtnMap );
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoCalendar::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__u8	Tv_Day;
	// code --------------------
		//select day
		Tv_Day	=	LcGetDayByPos( (__s32)A_X, (__s32)A_Y );
		if ( 0 != Tv_Day )
		{
			//update date
			m_SelYear		=	m_Year;
			m_SelMonth	=	m_Month;
			m_SelDay		=	Tv_Day;
			//change cursor
			m_Cus				=	Tv_Day;
			//make day click event
			LcMakeEvtDaySelect();
			ReqDraw();
		}
		Cls_GrUiDoSkin::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::LcMakeEvtDaySelect( void )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		if ( V_GrUiMngObj->IsUserEventOn() || (E_GrUiObjIdNone != m_EvtHostId ) )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdCalDaySel;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_EvtHostId;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::LcMakeEvtDayChange( void )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		if ( V_GrUiMngObj->IsUserEventOn() || (E_GrUiObjIdNone != m_EvtHostId ) )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdCalDayChg;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_EvtHostId;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoCalendar::ProcEvtKeyDown( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		switch( A_Key )
		{
			case	E_GrUiKeyLeft:
				Tv_Result	=	LcOnKeyDownLeft();
				break;
			case	E_GrUiKeyRight:
				Tv_Result	=	LcOnKeyDownRight();
				break;
			case	E_GrUiKeyUp:
				Tv_Result	=	LcOnKeyDownUp();
				break;
			case	E_GrUiKeyDown:
				Tv_Result	=	LcOnKeyDownDown();
				break;
			default:
				Tv_Result	=	Cls_GrUiDoSkin::ProcEvtKeyDown( A_Key );
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoCalendar::ProcEvtKeyUp( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		switch( A_Key )
		{
			case	E_GrUiKeyEnter:
				LcOnKeyUpEnter();
				Tv_Result	=	TRUE;
				break;
			case	E_GrUiKeyEsc:
			case	E_GrUiKeyMenu:
				Tv_Result	=	LcOnKeyUpEsc();
				break;
			default:
				Tv_Result	=	Cls_GrUiDoSkin::ProcEvtKeyUp( A_Key );
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::ProcEvtFocusLoss( void )
{
		Cls_GrUiDoSkin::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::ProcEvtMseEnter( void )
{
		//not work
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::ProcEvtMseLeave( void )
{
		//not work
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::LcGetCusPosStat( void )
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_Last;
		__u8	Tv_Line;
	// code --------------------
		Tv_Result	=	0;

		//get line position
		Tv_Last		=	m_WeekPos + m_Cus - 1;
		Tv_Line		=	0;
		while ( 7 <= Tv_Last )
		{
			Tv_Line ++;
			Tv_Last	=	Tv_Last - 7;
		}
		if ( 0 == Tv_Line )
		{
			Tv_Result	=	Tv_Result | E_GrUiDoCalendarCusUpEnd;
		}
		if ( 7 > (m_MaxDay - m_Cus) )
		{
			Tv_Result	=	Tv_Result | E_GrUiDoCalendarCusDownEnd;
		}
		if ( (0 == Tv_Last) || (1 >= m_Cus) )
		{
			Tv_Result	=	Tv_Result | E_GrUiDoCalendarCusLeftEnd;
		}
		if ( (6 == Tv_Last) || ((m_MaxDay <= m_Cus)) )
		{
			Tv_Result	=	Tv_Result | E_GrUiDoCalendarCusRightEnd;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoCalendar::LcOnKeyDownLeft( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_PosStat;
	// code --------------------
		
		Tv_Result	=	FALSE;

		//check left limite
		Tv_PosStat	=	LcGetCusPosStat();
		if ( 0 != (E_GrUiDoCalendarCusLeftEnd & Tv_PosStat) )
		{
			if ( 0 == (E_GrUiDoFocusMapLeft & m_FocusMoveMap) )
			{
				if ( 0 != (E_GrUiDoCalendarCusUpEnd & Tv_PosStat) )
				{
					m_Cus	=	m_MaxDay;
				}
				else
				{
					m_Cus --;
				}
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			m_Cus --;
			Tv_Result	=	TRUE;
		}
		//redraw
		if ( Tv_Result )
		{
			ReqDraw();
			LcMakeEvtDayChange();
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoCalendar::LcOnKeyDownRight( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_PosStat;
	// code --------------------
		Tv_Result	=	FALSE;

		//check right limite
		Tv_PosStat	=	LcGetCusPosStat();
		if ( 0 != (E_GrUiDoCalendarCusRightEnd & Tv_PosStat) )
		{
			if ( 0 == (E_GrUiDoFocusMapRight & m_FocusMoveMap) )
			{
				if ( m_Cus >= m_MaxDay )
				{
					m_Cus	=	1;
				}
				else
				{
					m_Cus ++;
				}
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			m_Cus ++;
			Tv_Result	=	TRUE;
		}
		//redraw
		if ( Tv_Result )
		{
			ReqDraw();
			LcMakeEvtDayChange();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoCalendar::LcOnKeyDownUp( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_PosStat;
	// code --------------------
		Tv_Result	=	FALSE;

		//check right limite
		Tv_PosStat	=	LcGetCusPosStat();
		if ( 0 != (E_GrUiDoCalendarCusUpEnd & Tv_PosStat) )
		{
			if ( 0 == (E_GrUiDoFocusMapUp & m_FocusMoveMap) )
			{
				m_Cus		=	m_MaxDay;
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			m_Cus		=	m_Cus - 7;
			Tv_Result	=	TRUE;
		}
		//redraw
		if ( Tv_Result )
		{
			ReqDraw();
			LcMakeEvtDayChange();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoCalendar::LcOnKeyDownDown( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_PosStat;
	// code --------------------
		Tv_Result	=	FALSE;

		//check right limite
		Tv_PosStat	=	LcGetCusPosStat();
		if ( 0 != (E_GrUiDoCalendarCusDownEnd & Tv_PosStat) )
		{
			if ( 0 == (E_GrUiDoFocusMapDown & m_FocusMoveMap) )
			{
				m_Cus		=	1;
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			m_Cus		=	m_Cus + 7;
			Tv_Result	=	TRUE;
		}
		//redraw
		if ( Tv_Result )
		{
			ReqDraw();
			LcMakeEvtDayChange();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::LcOnKeyUpEnter( void )
{
		m_SelYear		=	m_Year;
		m_SelMonth	=	m_Month;
		m_SelDay		=	m_Cus;
		LcMakeEvtDaySelect();
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoCalendar::LcOnKeyUpEsc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::ProcUiMsg( Ptr_GrUiMsgHd A_PtrMsg )
{
		switch( A_PtrMsg->Cmd )
		{
			case	E_GrUiMsgCmdSpinValChg:
				LcOnSpinChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			default:
				Cls_GrUiDoSkin::ProcUiMsg( A_PtrMsg );
				break;
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::LcGetMonthFromSpin( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		__u8	Tv_Result;
		Cls_GrUiDoSpinNum*	Tv_SpinNum;
		Cls_GrUiDoSpinTxt*	Tv_SpinTxt;
	// code --------------------
		Tv_Result	=	0;

		if ( E_GrUiDoTypeIdSpbtnNum == A_Obj->TypeIdGet() )
		{
			Tv_SpinNum	=	(Cls_GrUiDoSpinNum*)A_Obj;
			Tv_Result	=	(__u8)Tv_SpinNum->ValueGet();
		}
		else if ( E_GrUiDoTypeIdSpbtnTxt == A_Obj->TypeIdGet() )
		{
			Tv_SpinTxt	=	(Cls_GrUiDoSpinTxt*)A_Obj;
			Tv_Result		=	(__u8)Tv_SpinTxt->SelectIdxGet() + 1;
		}
		//check value
		if ( 12 < Tv_Result )
		{
			Tv_Result	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoCalendar::LcGetYearFromSpin( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrUiDoSpinNum*	Tv_SpinNum;
	// code --------------------
		Tv_Result	=	0;

		if ( E_GrUiDoTypeIdSpbtnNum == A_Obj->TypeIdGet() )
		{
			Tv_SpinNum	=	(Cls_GrUiDoSpinNum*)A_Obj;
			Tv_Result	=	(__u16)Tv_SpinNum->ValueGet();
		}
		//check value
		if ( (E_GrTimeBaseYear > Tv_Result) || (E_GrTimeMaxYear <= Tv_Result) )
		{
			Tv_Result	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::LcOnSpinChg( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		BOOL8	Tv_IsChg;
		__u16	Tv_Year;
		__u8	Tv_Month;
	// code --------------------
		//check user data
		Tv_IsChg	=	FALSE;
		Tv_Year		=	m_Year;
		Tv_Month	=	m_Month;
		switch( A_Obj->UserDataGet() )
		{
			case	E_GrUiDoCalendarSpinMonth:
				Tv_Month	=	LcGetMonthFromSpin( A_Obj );
				if ( 0 != Tv_Month )
				{
					Tv_IsChg	=	TRUE;
				}
				break;
			case	E_GrUiDoCalendarSpinYear:
				Tv_Year	=	LcGetYearFromSpin( A_Obj );
				if ( 0 != Tv_Year )
				{
					Tv_IsChg	=	TRUE;
				}
				break;
		}
		//update 
		if ( Tv_IsChg )
		{
			ViewDateSet( Tv_Year, Tv_Month, TRUE );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::LcRelateCtrlUpdt( void )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Obj;
	// code --------------------

		//update year control
		if ( E_GrUiObjIdNone != m_SpinYearId )
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByObjId( m_SpinYearId );
			if ( NULL != Tv_Obj )
			{
				//check type
				if ( E_GrUiDoTypeIdSpbtnNum == Tv_Obj->TypeIdGet() )
				{
					((Cls_GrUiDoSpinNum*)Tv_Obj)->ValueSet( (__s32)m_Year, FALSE );
				}
			}
		}

		//update month control
		if ( E_GrUiObjIdNone != m_SpinMonthId )
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByObjId( m_SpinMonthId );
			if ( NULL != Tv_Obj )
			{
				//check type
				if ( E_GrUiDoTypeIdSpbtnNum == Tv_Obj->TypeIdGet() )
				{
					((Cls_GrUiDoSpinNum*)Tv_Obj)->ValueSet( (__s32)m_Month, FALSE );
				}
				else if ( E_GrUiDoTypeIdSpbtnTxt == Tv_Obj->TypeIdGet() )
				{
					((Cls_GrUiDoSpinTxt*)Tv_Obj)->SelectIdxSet( (__s32)m_Month, FALSE );
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::ViewDateSet( __u16 A_Year, __u8 A_Month, BOOL8 A_IsNotify )
{
	// local -------------------
	// code --------------------
		//check able
		if (	(E_GrTimeBaseYear <= A_Year) && (E_GrTimeMaxYear > A_Year) && 
					(1 <= A_Month) && (12 >= A_Month)	)
		{
			//do change
			m_Year		=	A_Year;
			m_Month		=	A_Month;
			m_Cus			=	1;
			LcMarkTblReset();
			LcDateEnvUpdate();		//update date enviroment

			//update contorls
			LcRelateCtrlUpdt();

			ReqDraw();
			if ( A_IsNotify )
			{
				LcMakeEvtDayChange();
			}
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoCalendar::SelYearGet( void )
{
		return	m_SelYear;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::SelMonthGet( void )
{
		return	m_SelMonth;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::SelDayGet( void )
{
		return	m_SelDay;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::SelDaySet( __u8 A_Day )
{
		//check able
		if ( (1 <= A_Day) && (m_MaxDay >= A_Day) )
		{
			//update
			m_SelYear		=	m_Year;
			m_SelMonth	=	m_Month;
			m_SelDay		=	A_Day;
			//redraw
			ReqDraw();
		}
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrUiDoCalendar::SelTimeGet( void )
{
	// local -------------------
	// code --------------------
		return	GrTimeEnc( m_SelYear, m_SelMonth, m_SelDay, 0, 0, 0 );
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoCalendar::ViewYearGet( void )
{
		return	m_Year;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::ViewMonthGet( void )
{
		return	m_Month;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::MarkValSet( __u8 A_Day, __u8 A_Val )
{
		if ( (1 <= A_Day) && (m_MaxDay >= A_Day)) 
		{
			m_MarkTbl[A_Day - 1]	=	A_Val;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::MarkValGet( __u8 A_Day )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( (1 <= A_Day) && (m_MaxDay >= A_Day))
		{
			Tv_Result	=	m_MarkTbl[A_Day -1];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::MarkMapSet( __u32 A_Map, __u8 A_Val )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u32	Tv_Mask;
	// code --------------------
		Tv_Mask	=	1;
		for ( Tv_WkIdx=0;Tv_WkIdx < m_MaxDay;Tv_WkIdx++ )
		{
			m_MarkTbl[Tv_WkIdx]	=	0;
			if ( 0 != (Tv_Mask & A_Map) )
			{
				m_MarkTbl[Tv_WkIdx]	=	A_Val;
			}
			Tv_Mask	=	Tv_Mask << 1;
		}
		
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::TextParaSet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( &m_TxtPara, A_PtrPara, sizeof(m_TxtPara) );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::TextParaGet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( A_PtrPara, &m_TxtPara, sizeof(m_TxtPara) );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::CalendarClrSet( __u8 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoCalendarClrTblCnt > A_Idx )
		{
			m_CalClrTbl[A_Idx]	=	A_Color;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoCalendar::CalendarClrGet( __u8 A_Idx )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrUiDoCalendarClrTblCnt > A_Idx )
		{
			Tv_Result	=	m_CalClrTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::ItemWidthSet( __u8 A_Size )
{
		m_ItemSizeX		=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::ItemWidthGet( void )
{
		return	m_ItemSizeX;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::ItemHeightSet( __u8 A_Size )
{
		m_ItemSizeY		=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::ItemHeightGet( void )
{
		return	m_ItemSizeY;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::ItemGapxSet( __u8 A_Size )
{
		m_ItemGapX		=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::ItemGapxGet( void )
{
		return	m_ItemGapX;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::ItemGapySet( __u8 A_Size )
{
		m_ItemGapY		=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::ItemGapyGet( void )
{
		return	m_ItemGapY;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::SelBoxGapSet( __u8 A_Size )
{
		m_SelGap		=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::SelBoxGapGet( void )
{
		return	m_SelGap;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::SelBoxDepthSet( __u8 A_Size )
{
		m_SelDepth	=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::SelBoxDepthGet( void )
{
		return	m_SelDepth;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::CusBoxGapSet( __u8 A_Size )
{
		m_CusGap		=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::CusBoxGapGet( void )
{
		return	m_CusGap;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::CusBoxDepthSet( __u8 A_Size )
{
		m_CusDepth	=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoCalendar::CusBoxDepthGet( void )
{
		return	m_CusDepth;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::CalendarSkinSet( __u8 A_Idx, Ptr_GrUiDoSkinImg A_PtrSkin )
{
		//check able image
		if ( E_GrUiDoCalenderSkinCount > A_Idx )
		{
			m_CalSkinTbl[A_Idx]		=	*A_PtrSkin;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::CalendarSkinGet( __u8 A_Idx, Ptr_GrUiDoSkinImg A_PtrRtSkin )
{
		A_PtrRtSkin->Atb	=	0;
		A_PtrRtSkin->Id		=	E_GrUiImgIdNone;
		if (  E_GrUiDoCalenderSkinCount > A_Idx )
		{
			*A_PtrRtSkin	=	m_CalSkinTbl[A_Idx];
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::StatClrSet( __u8 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoCalendarStatCount > A_Idx )
		{
			m_StatClrTbl[A_Idx]	=	A_Color;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoCalendar::StatClrGet( __u8 A_Idx )
{
	// local -------------------
		__u32	Tv_Result;	
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrUiDoCalendarStatCount > A_Idx )
		{
			Tv_Result	=	m_StatClrTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::SpinYearIdSet( __u16 A_Id )
{
		m_SpinYearId	=	A_Id;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoCalendar::SpinYearIdGet( void )
{
		return	m_SpinYearId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::SpinMonthIdSet( __u16 A_Id )
{
		m_SpinMonthId	=	A_Id;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoCalendar::SpinMonthIdGet( void )
{
		return	m_SpinMonthId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::EvtHostIdSet( __u16 A_Id )
{
		m_EvtHostId	=	A_Id;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoCalendar::EvtHostIdGet( void )
{
		return	m_EvtHostId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoCalendar::LcMarkTblReset( void )
{
	// local -------------------
		__u32*	Tv_PtrWk;
		__u8		Tv_WkIdx;
	// code --------------------
		Tv_PtrWk	=	(__u32*)m_MarkTbl;
		for ( Tv_WkIdx=0;Tv_WkIdx < ((sizeof(m_MarkTbl)) >> 2);Tv_WkIdx++ )
		{
			*Tv_PtrWk	=	0;
			Tv_PtrWk ++;
		}
}
//--------------------------------------------------------------------

