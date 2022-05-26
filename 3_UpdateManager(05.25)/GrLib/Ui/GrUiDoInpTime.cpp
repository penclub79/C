/*
 input time

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoInpTime.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>

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
Cls_GrUiDoInpTime::Cls_GrUiDoInpTime( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
	// local -------------------
	// code --------------------
		//init
		m_TypeId				=	E_GrUiDoTypeIdInpTime;

		m_RelIdYear			=	E_GrUiObjIdNone;				//year
		m_RelIdMon			=	E_GrUiObjIdNone;				//month
		m_RelIdDay			=	E_GrUiObjIdNone;				//day
		m_RelIdUnitYear	=	E_GrUiObjIdNone;				//divide label year 
		m_RelIdUnitMon	=	E_GrUiObjIdNone;				//divide lable month
		m_RelIdUnitMon	=	E_GrUiObjIdNone;				//divide lable month

		m_RelIdHour			=	E_GrUiObjIdNone;				//hour
		m_RelIdMin			=	E_GrUiObjIdNone;				//minute
		m_RelIdSec			=	E_GrUiObjIdNone;				//sec

		m_RelIdDiv1			=	E_GrUiObjIdNone;				//first division
		m_RelIdDiv2			=	E_GrUiObjIdNone;				//second division

		m_MinYear				=	2008;
		m_MinMon				=	1;
		m_MinDay				=	1;
		m_MinHour				=	0;
		m_MinMin				=	0;
		m_MinSec				=	0;
		m_MinTime				=	GrTimeEnc( m_MinYear, m_MinMon, m_MinDay, m_MinHour, m_MinMin, m_MinSec );

		m_MaxYear				=	2036;
		m_MaxMon				=	12;
		m_MaxDay				=	31;
		m_MaxHour				=	23;
		m_MaxMin				=	59;
		m_MaxSec				=	59;
		m_MinTime				=	GrTimeEnc( m_MaxYear, m_MaxMon, m_MaxDay, m_MaxHour, m_MaxMin, m_MaxSec );

		m_ValYear				=	2008;
		m_ValMonth			=	1;
		m_ValDay				=	1;
		m_ValHour				=	0;
		m_ValMin				=	0;
		m_ValSec				=	0;

		m_SpnNumYear		=	NULL;
		m_SpnNumMon			=	NULL;
		m_SpnNumDay			=	NULL;
		m_SpnNumHour		=	NULL;
		m_SpnNumMin			=	NULL;
		m_SpnNumSec			=	NULL;

		m_IsValChgAble	=	TRUE;
}
//--------------------------------------------------------------------
Cls_GrUiDoInpTime::~Cls_GrUiDoInpTime()
{

}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::UpdateDisplay( void )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_ObjTbl[8];
		__s32		Tv_X;
		St_GrRect	Tv_Rc;
		__s32		Tv_CtrlWidth;
		__s32		Tv_WkIdx;
	// code --------------------
		//get contorls
		for ( Tv_WkIdx=0;Tv_WkIdx < 8;Tv_WkIdx++ )
		{
			Tv_ObjTbl[Tv_WkIdx]	=	NULL;
		}

		switch( GrStrDateDispTypeGet() )
		{
			case	E_GrStrDateDispMmDdYy:
				//mmddyy
				if ( E_GrUiObjIdNone != m_RelIdMon )
				{
					Tv_ObjTbl[0]	=	FindSlaveObjByObjId( m_RelIdMon );
				}
				if ( E_GrUiObjIdNone != m_RelIdUnitMon )
				{
					Tv_ObjTbl[1]	=	FindSlaveObjByObjId( m_RelIdUnitMon );
				}
				if ( E_GrUiObjIdNone != m_RelIdDiv1 )
				{
					Tv_ObjTbl[2]	=	FindSlaveObjByObjId( m_RelIdDiv1 );
				}
				if ( E_GrUiObjIdNone != m_RelIdDay )
				{
					Tv_ObjTbl[3]	=	FindSlaveObjByObjId( m_RelIdDay );
				}
				if ( E_GrUiObjIdNone != m_RelIdUnitDay )
				{
					Tv_ObjTbl[4]	=	FindSlaveObjByObjId( m_RelIdUnitDay );
				}
				if ( E_GrUiObjIdNone != m_RelIdDiv2 )
				{
					Tv_ObjTbl[5]	=	FindSlaveObjByObjId( m_RelIdDiv2 );
				}
				if ( E_GrUiObjIdNone != m_RelIdYear )
				{
					Tv_ObjTbl[6]	=	FindSlaveObjByObjId( m_RelIdYear );
				}
				if ( E_GrUiObjIdNone != m_RelIdUnitYear )
				{
					Tv_ObjTbl[7]	=	FindSlaveObjByObjId( m_RelIdUnitYear );
				}
				break;
			case	E_GrStrDateDispDdMmYy:
				//ddmmyy
				if ( E_GrUiObjIdNone != m_RelIdDay )
				{
					Tv_ObjTbl[0]	=	FindSlaveObjByObjId( m_RelIdDay );
				}
				if ( E_GrUiObjIdNone != m_RelIdUnitDay )
				{
					Tv_ObjTbl[1]	=	FindSlaveObjByObjId( m_RelIdUnitDay );
				}
				if ( E_GrUiObjIdNone != m_RelIdDiv1 )
				{
					Tv_ObjTbl[2]	=	FindSlaveObjByObjId( m_RelIdDiv1 );
				}
				if ( E_GrUiObjIdNone != m_RelIdMon )
				{
					Tv_ObjTbl[3]	=	FindSlaveObjByObjId( m_RelIdMon );
				}
				if ( E_GrUiObjIdNone != m_RelIdUnitMon )
				{
					Tv_ObjTbl[4]	=	FindSlaveObjByObjId( m_RelIdUnitMon );
				}
				if ( E_GrUiObjIdNone != m_RelIdDiv2 )
				{
					Tv_ObjTbl[5]	=	FindSlaveObjByObjId( m_RelIdDiv2 );
				}
				if ( E_GrUiObjIdNone != m_RelIdYear )
				{
					Tv_ObjTbl[6]	=	FindSlaveObjByObjId( m_RelIdYear );
				}
				if ( E_GrUiObjIdNone != m_RelIdUnitYear )
				{
					Tv_ObjTbl[7]	=	FindSlaveObjByObjId( m_RelIdUnitYear );
				}
				break;
			default:
				//yymmdd
				if ( E_GrUiObjIdNone != m_RelIdYear )
				{
					Tv_ObjTbl[0]	=	FindSlaveObjByObjId( m_RelIdYear );
				}
				if ( E_GrUiObjIdNone != m_RelIdUnitYear )
				{
					Tv_ObjTbl[1]	=	FindSlaveObjByObjId( m_RelIdUnitYear );
				}
				if ( E_GrUiObjIdNone != m_RelIdDiv1 )
				{
					Tv_ObjTbl[2]	=	FindSlaveObjByObjId( m_RelIdDiv1 );
				}
				if ( E_GrUiObjIdNone != m_RelIdMon )
				{
					Tv_ObjTbl[3]	=	FindSlaveObjByObjId( m_RelIdMon );
				}
				if ( E_GrUiObjIdNone != m_RelIdUnitMon )
				{
					Tv_ObjTbl[4]	=	FindSlaveObjByObjId( m_RelIdUnitMon );
				}
				if ( E_GrUiObjIdNone != m_RelIdDiv2 )
				{
					Tv_ObjTbl[5]	=	FindSlaveObjByObjId( m_RelIdDiv2 );
				}
				if ( E_GrUiObjIdNone != m_RelIdDay )
				{
					Tv_ObjTbl[6]	=	FindSlaveObjByObjId( m_RelIdDay );
				}
				if ( E_GrUiObjIdNone != m_RelIdUnitDay )
				{
					Tv_ObjTbl[7]	=	FindSlaveObjByObjId( m_RelIdUnitDay );
				}
				break;
		}

		//reposition
		Tv_X	=	(__s32)m_CliMgn.left;
		for ( Tv_WkIdx=0;Tv_WkIdx < 8;Tv_WkIdx++ )
		{
			if ( NULL != Tv_ObjTbl[Tv_WkIdx] )
			{
				Tv_Rc	=	*(Tv_ObjTbl[Tv_WkIdx]->RectGet());
				Tv_CtrlWidth	=	Tv_Rc.right - Tv_Rc.left;
				Tv_Rc.left	=	Tv_X;
				Tv_Rc.right	=	Tv_Rc.left + Tv_CtrlWidth;
				Tv_ObjTbl[Tv_WkIdx]->RectSet( &Tv_Rc );
				Tv_X	=	Tv_X + Tv_CtrlWidth;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::LcCtrlListUpdt( void )
{
	// local -------------------
#ifdef GR_OSD_EDIT
		Cls_GrUiDoBase*	Tv_Obj;
		__u8	Tv_TypeId;
#endif
	// code --------------------
		m_SpnNumYear	=	NULL;
		m_ValYear			=	2008;
		if ( E_GrUiObjIdNone != m_RelIdYear )
		{
#ifdef GR_OSD_EDIT
			Tv_Obj	=	FindSlaveObjByObjId( m_RelIdYear );
			if ( NULL != Tv_Obj )
			{
				Tv_TypeId	=	Tv_Obj->TypeIdGet();
				if ( (E_GrUiDoTypeIdSpbtnNum == Tv_TypeId) || (E_GrUiDoTypeIdAcSpNum == Tv_TypeId) )
				{
					m_SpnNumYear	=	(Cls_GrUiDoSpinNum*)Tv_Obj;
				}
				else
				{
					MessageBoxW( NULL, L"InpTime : bad object type - realate year spinbutton.", L"ERROR", MB_ICONHAND | MB_OK );
				}
			}
#else
			m_SpnNumYear	=	(Cls_GrUiDoSpinNum*)FindSlaveObjByObjId( m_RelIdYear );
#endif
			if ( NULL !=	m_SpnNumYear )
			{
				m_SpnNumYear->EvtHostIdSet( m_ObjId );
				m_ValYear	=	(__u16)m_SpnNumYear->ValueGet();
			}
		}
		m_SpnNumMon		=	NULL;
		m_ValMonth		=	1;
		if ( E_GrUiObjIdNone != m_RelIdMon )
		{
#ifdef GR_OSD_EDIT
			Tv_Obj	=	FindSlaveObjByObjId( m_RelIdMon );
			if ( NULL != Tv_Obj )
			{
				Tv_TypeId	=	Tv_Obj->TypeIdGet();
				if ( (E_GrUiDoTypeIdSpbtnNum == Tv_TypeId) || (E_GrUiDoTypeIdAcSpNum == Tv_TypeId) )
				{
					m_SpnNumMon	=	(Cls_GrUiDoSpinNum*)Tv_Obj;
				}
				else
				{
					MessageBoxW( NULL, L"InpTime : bad object type - realate month spinbutton.", L"ERROR", MB_ICONHAND | MB_OK );
				}
			}
#else
			m_SpnNumMon	=	(Cls_GrUiDoSpinNum*)FindSlaveObjByObjId( m_RelIdMon );
#endif
			if ( NULL !=	m_SpnNumMon )
			{
				m_SpnNumMon->EvtHostIdSet( m_ObjId );
				m_ValMonth	=	(__u8)m_SpnNumMon->ValueGet();
			}
		}
		m_SpnNumDay		=	NULL;
		m_ValDay			=	1;
		if ( E_GrUiObjIdNone != m_RelIdDay )
		{
#ifdef GR_OSD_EDIT
			Tv_Obj	=	FindSlaveObjByObjId( m_RelIdDay );
			if ( NULL != Tv_Obj )
			{
				Tv_TypeId	=	Tv_Obj->TypeIdGet();
				if ( (E_GrUiDoTypeIdSpbtnNum == Tv_TypeId) || (E_GrUiDoTypeIdAcSpNum == Tv_TypeId) )
				{
					m_SpnNumDay	=	(Cls_GrUiDoSpinNum*)Tv_Obj;
				}
				else
				{
					MessageBoxW( NULL, L"InpTime : bad object type - realate day spinbutton.", L"ERROR", MB_ICONHAND | MB_OK );
				}
			}
#else
			m_SpnNumDay	=	(Cls_GrUiDoSpinNum*)FindSlaveObjByObjId( m_RelIdDay );
#endif
			if ( NULL != m_SpnNumDay )
			{
				m_SpnNumDay->EvtHostIdSet( m_ObjId );
				m_ValDay	=	(__u8)m_SpnNumDay->ValueGet();
			}
		}
		m_SpnNumHour	=	NULL;
		m_ValHour			=	0;
		if ( E_GrUiObjIdNone != m_RelIdHour )
		{
#ifdef GR_OSD_EDIT
			Tv_Obj	=	FindSlaveObjByObjId( m_RelIdHour );
			if ( NULL != Tv_Obj )
			{
				Tv_TypeId	=	Tv_Obj->TypeIdGet();
				if ( (E_GrUiDoTypeIdSpbtnNum == Tv_TypeId) || (E_GrUiDoTypeIdAcSpNum == Tv_TypeId) )
				{
					m_SpnNumHour	=	(Cls_GrUiDoSpinNum*)Tv_Obj;
				}
				else
				{
					MessageBoxW( NULL, L"InpTime : bad object type - realate hour spinbutton.", L"ERROR", MB_ICONHAND | MB_OK );
				}
			}
#else
			m_SpnNumHour	=	(Cls_GrUiDoSpinNum*)FindSlaveObjByObjId( m_RelIdHour );
#endif
			if ( NULL != m_SpnNumHour )
			{
				m_SpnNumHour->EvtHostIdSet( m_ObjId );
				m_ValHour	=	(__u8)m_SpnNumHour->ValueGet();
			}
		}
		m_SpnNumMin		=	NULL;
		m_ValMin			=	0;
		if ( E_GrUiObjIdNone != m_RelIdMin )
		{
#ifdef GR_OSD_EDIT
			Tv_Obj	=	FindSlaveObjByObjId( m_RelIdMin );
			if ( NULL != Tv_Obj )
			{
				Tv_TypeId	=	Tv_Obj->TypeIdGet();
				if ( (E_GrUiDoTypeIdSpbtnNum == Tv_TypeId) || (E_GrUiDoTypeIdAcSpNum == Tv_TypeId) )
				{
					m_SpnNumMin	=	(Cls_GrUiDoSpinNum*)Tv_Obj;
				}
				else
				{
					MessageBoxW( NULL, L"InpTime : bad object type - realate minute spinbutton.", L"ERROR", MB_ICONHAND | MB_OK );
				}
			}
#else
			m_SpnNumMin	=	(Cls_GrUiDoSpinNum*)FindSlaveObjByObjId( m_RelIdMin );
#endif
			if ( NULL != m_SpnNumMin )
			{
				m_SpnNumMin->EvtHostIdSet( m_ObjId );
				m_ValMin	=	(__u8)m_SpnNumMin->ValueGet();
			}
		}
		m_SpnNumSec		=	NULL;
		m_ValSec			=	0;
		if ( E_GrUiObjIdNone != m_RelIdSec )
		{
#ifdef GR_OSD_EDIT
			Tv_Obj	=	FindSlaveObjByObjId( m_RelIdSec );
			if ( NULL != Tv_Obj )
			{
				Tv_TypeId	=	Tv_Obj->TypeIdGet();
				if ( (E_GrUiDoTypeIdSpbtnNum == Tv_TypeId) || (E_GrUiDoTypeIdAcSpNum == Tv_TypeId) )
				{
					m_SpnNumSec	=	(Cls_GrUiDoSpinNum*)Tv_Obj;
				}
				else
				{
					MessageBoxW( NULL, L"InpTime : bad object type - realate sec spinbutton.", L"ERROR", MB_ICONHAND | MB_OK );
				}
			}
#else
			m_SpnNumSec	=	(Cls_GrUiDoSpinNum*)FindSlaveObjByObjId( m_RelIdSec );
#endif
			if ( NULL != m_SpnNumSec )
			{
				m_SpnNumSec->EvtHostIdSet( m_ObjId );
				m_ValSec	=	(__u8)m_SpnNumSec->ValueGet();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::LcTimeLimUpdate( void )
{
	// local -------------------
		__u8	Tv_Min;
		__u8	Tv_Max;
	// code --------------------
		//year limite
		if ( NULL != m_SpnNumYear )
		{
			m_SpnNumYear->RangeMinSet( m_MinYear );
			m_SpnNumYear->RangeMaxSet( m_MaxYear );
		}
		//month limite
		if ( NULL != m_SpnNumMon )
		{
			Tv_Min		=	1;
			Tv_Max		=	12;
			if (m_MinYear == m_ValYear )
			{
				Tv_Min	=	m_MinMon;
			}
			if ( m_MaxYear == m_ValYear )
			{
				Tv_Max	=	m_MaxMon;
			}
			m_SpnNumMon->RangeMinSet( Tv_Min );
			m_SpnNumMon->RangeMaxSet( Tv_Max );
		}
		//day limite
		if ( NULL != m_SpnNumDay )
		{
			Tv_Min		=	1;
			Tv_Max		=	GrTimeGetMonthMaxDay( m_ValYear, m_ValMonth );
			if ( (m_MinYear == m_ValYear) && (m_MinMon == m_ValMonth) )
			{
				Tv_Min	=	m_MinDay;
			}
			if ( (m_MaxYear == m_ValYear) && (m_MaxMon == m_ValMonth) )
			{
				Tv_Max	=	m_MaxDay;
			}
			m_SpnNumDay->RangeMinSet( Tv_Min );
			m_SpnNumDay->RangeMaxSet( Tv_Max );
		}
		//hour limite
		if ( NULL != m_SpnNumHour )
		{
			Tv_Min		=	0;
			Tv_Max		=	23;
			if ( (m_MinYear == m_ValYear) && (m_MinMon == m_ValMonth) && (m_MinDay == m_ValDay) )
			{
				Tv_Min	=	m_MinHour;
			}
			if ( (m_MaxYear == m_ValYear) && (m_MaxMon == m_ValMonth) && (m_MaxDay == m_ValDay) )
			{
				Tv_Max	=	m_MaxHour;
			}
			m_SpnNumHour->RangeMinSet( Tv_Min );
			m_SpnNumHour->RangeMaxSet( Tv_Max );
		}
		//minute limite
		if ( NULL != m_SpnNumMin )
		{
			Tv_Min		=	0;
			Tv_Max		=	59;
			if ( (m_MinYear == m_ValYear) && (m_MinMon == m_ValMonth) && (m_MinDay == m_ValDay) && (m_MinHour == m_ValHour) )
			{
				Tv_Min	=	m_MinMin;
			}
			if ( (m_MaxYear == m_ValYear) && (m_MaxMon == m_ValMonth) && (m_MaxDay == m_ValDay) && (m_MaxHour == m_ValHour) )
			{
				Tv_Max	=	m_MaxMin;
			}
			m_SpnNumMin->RangeMinSet( Tv_Min );
			m_SpnNumMin->RangeMaxSet( Tv_Max );
		}
		//minute limite
		if ( NULL != m_SpnNumSec )
		{
			Tv_Min		=	0;
			Tv_Max		=	59;
			if ( (m_MinYear == m_ValYear) && (m_MinMon == m_ValMonth) && (m_MinDay == m_ValDay) && (m_MinHour == m_ValHour) && (m_MinMin == m_ValMin) )
			{
				Tv_Min	=	m_MinSec;
			}
			if ( (m_MaxYear == m_ValYear) && (m_MaxMon == m_ValMonth) && (m_MaxDay == m_ValDay) && (m_MaxHour == m_ValHour) && (m_MaxMin == m_ValMin) )
			{
				Tv_Max	=	m_MaxSec;
			}
			m_SpnNumSec->RangeMinSet( Tv_Min );
			m_SpnNumSec->RangeMaxSet( Tv_Max );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::LcLimCorrect( void )
{
	// local -------------------
	// code --------------------
		//lock value
		m_IsValChgAble	=	FALSE;
		//check year
		if ( NULL != m_SpnNumYear )
		{
			if ( ( m_SpnNumYear->RangeMinGet() > (__s32)m_ValYear ) || (m_SpnNumYear->RangeMaxGet() < (__s32)m_ValYear) )
			{
				m_ValYear	=	m_MaxYear;
				m_SpnNumYear->ValueSet( (__s32)m_ValYear, FALSE );
			}
		}
		//check month
		if ( NULL != m_SpnNumMon )
		{
			if ( ( m_SpnNumMon->RangeMinGet() > (__s32)m_ValMonth ) || (m_SpnNumMon->RangeMaxGet() < (__s32)m_ValMonth) )
			{
				m_ValMonth	=	m_MaxMon;
				m_SpnNumMon->ValueSet( (__s32)m_ValMonth, FALSE );
			}
		}
		//check day
		if ( NULL != m_SpnNumDay )
		{
			if ( ( m_SpnNumDay->RangeMinGet() > (__s32)m_ValDay ) || (m_SpnNumDay->RangeMaxGet() < (__s32)m_ValDay) )
			{
				m_ValDay	=	m_MaxDay;
				m_SpnNumDay->ValueSet( (__s32)m_ValDay, FALSE );
			}
		}
		//check hour
		if ( NULL != m_SpnNumHour )
		{
			if ( ( m_SpnNumHour->RangeMinGet() > (__s32)m_ValHour ) || (m_SpnNumHour->RangeMaxGet() < (__s32)m_ValHour) )
			{
				m_ValHour	=	m_MaxHour;
				m_SpnNumHour->ValueSet( (__s32)m_ValHour, FALSE );
			}
		}
		//check min
		if ( NULL != m_SpnNumMin )
		{
			if ( ( m_SpnNumMin->RangeMinGet() > (__s32)m_ValMin ) || (m_SpnNumMin->RangeMaxGet() < (__s32)m_ValMin) )
			{
				m_ValMin	=	m_MaxMin;
				m_SpnNumMin->ValueSet( (__s32)m_ValMin, FALSE );
			}
		}
		//check sec
		if ( NULL != m_SpnNumSec )
		{
			if ( ( m_SpnNumSec->RangeMinGet() > (__s32)m_ValSec ) || (m_SpnNumSec->RangeMaxGet() < (__s32)m_ValSec) )
			{
				m_ValSec	=	m_MaxSec;
				m_SpnNumSec->ValueSet( (__s32)m_ValSec, FALSE );
			}
		}
		//unlock value
		m_IsValChgAble	=	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::TimeRangeSet( Def_GrTime A_TimeMin, Def_GrTime A_TimeMax )
{
	// local -------------------
	// code --------------------
		if ( A_TimeMin <= A_TimeMax )
		{
			m_MinTime	=	A_TimeMin;
			m_MaxTime	=	A_TimeMax;
			GrTimeDec( A_TimeMin, &m_MinYear, &m_MinMon, &m_MinDay, &m_MinHour, &m_MinMin, &m_MinSec );
			GrTimeDec( A_TimeMax, &m_MaxYear, &m_MaxMon, &m_MaxDay, &m_MaxHour, &m_MaxMin, &m_MaxSec );
			//update limite
			LcCtrlListUpdt();
			LcTimeLimUpdate();
			LcLimCorrect();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::LcNotifyValChg( void )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		if ( V_GrUiMngObj->IsUserEventOn() )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdInpTimeChg;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;
			Tv_Msg.Obj				=	(Cls_GrUiDoBase*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::TimeSet( Def_GrTime A_Time )
{
	// local -------------------
	// code --------------------
		m_IsValChgAble	=	FALSE;

		GrTimeDec( A_Time, &m_ValYear, &m_ValMonth, &m_ValDay, &m_ValHour, &m_ValMin, &m_ValSec );
				
		if ( NULL != m_SpnNumYear )
		{
			m_SpnNumYear->ValueSet( (__s32)m_ValYear, FALSE );
		}
		else
		{
			m_ValYear	=	0;
		}
		//check month
		if ( NULL != m_SpnNumMon )
		{
			m_SpnNumMon->ValueSet( (__s32)m_ValMonth, FALSE );
		}
		else
		{
			m_ValMonth	=	1;
		}
		//check day
		if ( NULL != m_SpnNumDay )
		{
			m_SpnNumDay->ValueSet( (__s32)m_ValDay, FALSE );
		}
		else
		{
			m_ValDay	=	1;
		}
		//check hour
		if ( NULL != m_SpnNumHour )
		{
			m_SpnNumHour->ValueSet( (__s32)m_ValHour, FALSE );
		}
		else
		{
			m_ValHour	=	0;
		}
		//check min
		if ( NULL != m_SpnNumMin )
		{
			m_SpnNumMin->ValueSet( (__s32)m_ValMin, FALSE );
		}
		else
		{
			m_ValMin	=	0;
		}
		//check sec
		if ( NULL != m_SpnNumSec )
		{
			m_SpnNumSec->ValueSet( (__s32)m_ValSec, FALSE );
		}
		else
		{
			m_ValSec	=	0;
		}
		m_IsValChgAble	=	TRUE;
		//change event
		LcNotifyValChg();
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrUiDoInpTime::TimeGet( void )
{
	// local -------------------
	// code --------------------
		return	GrTimeEnc( m_ValYear, m_ValMonth, m_ValDay, m_ValHour, m_ValMin, m_ValSec );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::ProcUiMsg( Ptr_GrUiMsgHd A_PtrMsg )
{
	// local -------------------
		Ptr_GrUiMsgCtrl	Tv_PtrMsg;
		__u16	Tv_ObjId;
		BOOL8	Tv_IsChg;
	// code --------------------
		if ( E_GrUiMsgCmdSpinValChg == A_PtrMsg->Cmd )
		{
			Tv_PtrMsg	=	(Ptr_GrUiMsgCtrl)A_PtrMsg;
			Tv_ObjId	=	((Cls_GrUiDoBase*)Tv_PtrMsg->Obj)->ObjIdGet();
			Tv_IsChg	=	TRUE;
			//check contorl
			if ( m_RelIdYear == Tv_ObjId )
			{
				m_ValYear	=	(__u16)((Cls_GrUiDoSpinNum*)Tv_PtrMsg->Obj)->ValueGet();
			}
			else if ( m_RelIdMon == Tv_ObjId )
			{
				m_ValMonth	=	(__u8)((Cls_GrUiDoSpinNum*)Tv_PtrMsg->Obj)->ValueGet();
			}
			else if ( m_RelIdDay == Tv_ObjId )
			{
				m_ValDay	=	(__u8)((Cls_GrUiDoSpinNum*)Tv_PtrMsg->Obj)->ValueGet();
			}
			else if ( m_RelIdHour == Tv_ObjId )
			{
				m_ValHour	=	(__u8)((Cls_GrUiDoSpinNum*)Tv_PtrMsg->Obj)->ValueGet();
			}
			else if ( m_RelIdMin == Tv_ObjId )
			{
				m_ValMin	=	(__u8)((Cls_GrUiDoSpinNum*)Tv_PtrMsg->Obj)->ValueGet();
			}
			else if ( m_RelIdSec == Tv_ObjId )
			{
				m_ValSec	=	(__u8)((Cls_GrUiDoSpinNum*)Tv_PtrMsg->Obj)->ValueGet();
			}
			else
			{
				Tv_IsChg	=	FALSE;
			}
			//event do
			if ( Tv_IsChg )
			{
				LcCtrlListUpdt();
				LcTimeLimUpdate();
				LcLimCorrect();
				LcNotifyValChg();
			}
		}
		else
		{
			//inherited
			Cls_GrUiDoSkin::ProcUiMsg( A_PtrMsg );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::RelIdYearSet( __u16 A_ObjId )
{
		m_RelIdYear	=	A_ObjId;
		LcCtrlListUpdt();
		LcTimeLimUpdate();
		UpdateDisplay();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoInpTime::RelIdYearGet( void )
{
		return	m_RelIdYear;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::RelIdMonthSet( __u16 A_ObjId )
{
		m_RelIdMon	=	A_ObjId;
		LcCtrlListUpdt();
		LcTimeLimUpdate();
		UpdateDisplay();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoInpTime::RelIdMonthGet( void )
{
		return	m_RelIdMon;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::RelIdDaySet( __u16 A_ObjId )
{
		m_RelIdDay	=	A_ObjId;
		LcCtrlListUpdt();
		LcTimeLimUpdate();
		UpdateDisplay();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoInpTime::RelIdDayGet( void )
{
		return	m_RelIdDay;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::RelIdUnitYearSet( __u16 A_ObjId )
{
		m_RelIdUnitYear	=	A_ObjId;
		UpdateDisplay();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoInpTime::RelIdUnitYearGet( void )
{
		return	m_RelIdUnitYear;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::RelIdUnitMonthSet( __u16 A_ObjId )
{
		m_RelIdUnitMon	=	A_ObjId;
		UpdateDisplay();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoInpTime::RelIdUnitMonthGet( void )
{
		return	m_RelIdUnitMon;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::RelIdUnitDaySet( __u16 A_ObjId )
{
		m_RelIdUnitDay	=	A_ObjId;
		UpdateDisplay();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoInpTime::RelIdUnitDayGet( void )
{
		return	m_RelIdUnitDay;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::RelIdHourSet( __u16 A_ObjId )
{
		m_RelIdHour	=	A_ObjId;
		LcCtrlListUpdt();
		LcTimeLimUpdate();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoInpTime::RelIdHourGet( void )
{
		return	m_RelIdHour;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::RelIdMinSet( __u16 A_ObjId )
{
		m_RelIdMin	=	A_ObjId;
		LcCtrlListUpdt();
		LcTimeLimUpdate();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoInpTime::RelIdMinGet( void )
{
		return	m_RelIdMin;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::RelIdSecSet( __u16 A_ObjId )
{
		m_RelIdSec	=	A_ObjId;
		LcCtrlListUpdt();
		LcTimeLimUpdate();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoInpTime::RelIdSecGet( void )
{
		return	m_RelIdSec;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::ProcEvtScptDecoded( void )
{
		Cls_GrUiDoSkin::ProcEvtScptDecoded();
		LcCtrlListUpdt();
		LcTimeLimUpdate();
		UpdateDisplay();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::RelIdDiv1Set( __u16 A_ObjId )
{
		m_RelIdDiv1	=	A_ObjId;
		UpdateDisplay();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoInpTime::RelIdDiv1Get( void )
{
		return	m_RelIdDiv1;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoInpTime::RelIdDiv2Set( __u16 A_ObjId )
{
		m_RelIdDiv2	=	A_ObjId;
		UpdateDisplay();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoInpTime::RelIdDiv2Get( void )
{
		return	m_RelIdDiv2;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrUiDoInpTime::TimeRangeMinGet( void )
{
		return	m_MinTime;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrUiDoInpTime::TimeRangeMaxGet( void )
{
		return	m_MaxTime;
}
//--------------------------------------------------------------------

