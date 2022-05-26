/*
		platform calendar holiday

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoCalHday.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>
#include <P4/GrP4Bios.h>
#include <GrDebug.h>
#include <P4/GrP4ApiBase.h>
#include <P4/GrP4Cfg.h>

#ifdef GR_P4_EDITOR
#include	<P4/Dvl/GrP4DvlBase.h>
#include	<P4/Dvl/GrP4DvlMng.h>
#endif

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrP4Mng*	V_GrP4Mng;

#ifdef GR_P4_EDITOR

#define E_GrP4DoCalHdayParaCnt		1

St_GrP4DvlDoPrInfoData	V_GrP4DoCalHdayParaInfo[E_GrP4DoCalHdayParaCnt]	=	
{
	{	L"SchdCnt",				TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4DoCalHdayPara)0)->SchdCnt,NULL,	L"max holiday schedule count"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoCalHday::Cls_GrP4DoCalHday(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoCalBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
		__u32	Tv_Time;
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassCalHday;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaCalHday) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaCalHday, sizeof(m_ParaCalHday) );
		m_ParaCalHday.SchdCnt	=	1;

		// set type
		m_IsBitMode	=	FALSE;
		m_LvlCnt		=	1;

		// update now time
		Tv_Time	=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
		GrTimeDec( Tv_Time, &m_ElemTime[E_GrP4DoCalTimeIdxYear], (__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxMonth],
			(__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxDay], (__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxHour],
			(__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxMin], (__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxSec] );

		m_PtrYear	=	(__u16*)V_GrP4Mng->SetupDataPtrGet( E_GrP4CfgOfsHolyBaseYear );
		m_PtrTbl	=	(__u8*)V_GrP4Mng->SetupDataPtrGet( E_GrP4CfgOfsRecHoly );

		// init
		GrDumyZeroMem( m_DayTbl, sizeof(m_DayTbl) );

}
//--------------------------------------------------------------------
Cls_GrP4DoCalHday::~Cls_GrP4DoCalHday( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoCalHday::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoCalBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassCalRec;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaCalHday);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaCalHday, sizeof(m_ParaCalHday) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaCalHday)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoCalHday::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoCalBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaCalHday, sizeof(m_ParaCalHday) , m_PtrScpt , E_GrP4DoClassCalRec );

		// patch holiday count
		if ( 0 == m_ParaCalHday.SchdCnt )
		{
			m_ParaCalHday.SchdCnt	=	1;
		}
		else if ( E_GrP4CfgHoliSchdCnt < m_ParaCalHday.SchdCnt )
		{
			m_ParaCalHday.SchdCnt	=	E_GrP4CfgHoliSchdCnt;
		}
		m_LvlCnt	=	m_ParaCalHday.SchdCnt;
				
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoCalHday::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiUpdate:
				// relate control
				LcElemTmUpdtByCtrl();
				LcValueUpdt();
				LcCalRebuild();
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoCalBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoCalHday::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoCalBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaCalHday, V_GrP4DoCalHdayParaInfo, E_GrP4DoCalHdayParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoCalHday::LcCalRebuild( void )
{
	// local -------------------
		__u16	Tv_BaseYear;
		__u16	Tv_ByIdx;
		__u8	Tv_WkIdx;
		__u8	Tv_Val;
		__u8	Tv_DayIdx;

	// code --------------------
		// inherited
		Cls_GrP4DoCalBase::LcCalRebuild();	

		GrDumyZeroMem( m_DayTbl, sizeof(m_DayTbl) );

		// check year range
		Tv_BaseYear	=	E_GrP4CfgHdayBaseYear + (*m_PtrYear);
		if ( (Tv_BaseYear <= m_ElemTime[E_GrP4DoCalTimeIdxYear]) && ((Tv_BaseYear + E_GrP4CfgHdayYearCnt) > m_ElemTime[E_GrP4DoCalTimeIdxYear]) )
		{
			Tv_ByIdx	=	(m_ElemTime[E_GrP4DoCalTimeIdxYear] - Tv_BaseYear) * (12*16);
			Tv_ByIdx	=	Tv_ByIdx + ( (m_ElemTime[E_GrP4DoCalTimeIdxMonth] - 1) * 16);
			for ( Tv_WkIdx=0;Tv_WkIdx < 16;Tv_WkIdx++ )
			{
				Tv_Val		=	m_PtrTbl[Tv_ByIdx];
				Tv_DayIdx	=	Tv_WkIdx << 1;
				// even
				m_DayTbl[Tv_DayIdx]	=	Tv_Val & 0x0F;
				// odd
				m_DayTbl[Tv_DayIdx + 1]	=	(Tv_Val >> 4) & 0x0F;
				// next
				Tv_ByIdx ++;
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DoCalHday::LcCalRebuild - out of day range.\n" );
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrP4DoCalHday::LcMarkValueGet( __u8 A_Day )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( (0 < A_Day) && (31 >= A_Day )	)
		{
			Tv_Result	=	m_DayTbl[A_Day-1];
			if ( m_LvlCnt < Tv_Result )
			{
				Tv_Result	=	0;
			}
		}
		
		return	Tv_Result;		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalHday::LcProcTouchDay( void )
{
	// local -------------------
		__u8	Tv_Val;
		__u16	Tv_BaseYear;
		__u16	Tv_ByIdx;
	// code --------------------
		// update select day
		Tv_Val	=	0;
		if ( (0 < m_CusDay) && (31 >= m_CusDay) )
		{
			Tv_Val	=	m_DayTbl[m_CusDay-1];
			Tv_Val ++;
			if ( m_LvlCnt < Tv_Val )
			{
				Tv_Val	=	0;
			}
			m_DayTbl[m_CusDay-1]	=	Tv_Val;
		}

		// check year range
		Tv_BaseYear	=	E_GrP4CfgHdayBaseYear + (*m_PtrYear);
		if ( (Tv_BaseYear <= m_ElemTime[E_GrP4DoCalTimeIdxYear]) && ((Tv_BaseYear + E_GrP4CfgHdayYearCnt) > m_ElemTime[E_GrP4DoCalTimeIdxYear]) )
		{
			// update configuration
			Tv_ByIdx	=	(m_ElemTime[E_GrP4DoCalTimeIdxYear] - Tv_BaseYear) * (12*16);
			Tv_ByIdx	=	Tv_ByIdx + ( (m_ElemTime[E_GrP4DoCalTimeIdxMonth] - 1) * 16);
			Tv_ByIdx	=	Tv_ByIdx + (__u16)( (m_CusDay-1) >> 1);
			if ( 0 == (1 & (m_CusDay-1)) )
			{
				m_PtrTbl[Tv_ByIdx]	=	(m_PtrTbl[Tv_ByIdx] & 0xF0) | Tv_Val;
			}
			else
			{
				m_PtrTbl[Tv_ByIdx]	=	(m_PtrTbl[Tv_ByIdx] & 0x0F) | (Tv_Val << 4);
			}
		}
		else
		{
			DbgMsgPrint( "" );
		}
		ReqDraw();
		LcNotifyMarkChg();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalHday::RtlInit( void )
{
	// local -------------------
	// code --------------------
		// inherited
		LcValueUpdt();
		Cls_GrP4DoCalBase::RtlInit();

}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalHday::LcNotifyMarkChg( void )
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP4DoCalBase::LcNotifyMarkChg();
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalHday::LcValueUpdt( void )
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
