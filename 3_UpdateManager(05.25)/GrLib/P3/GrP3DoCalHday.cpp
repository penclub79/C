/*
		platform calendar holiday

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoCalHday.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>
#include <P3/GrP3Bios.h>
#include <GrDebug.h>
#include <P3/GrP3ApiBase.h>
#include <P3/GrP3Cfg.h>

#ifdef GR_P3_EDITOR
#include	<P3/Dvl/GrP3DvlBase.h>
#include	<P3/Dvl/GrP3DvlMng.h>
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
extern	Cls_GrP3Mng*	V_GrP3Mng;

#ifdef GR_P3_EDITOR

#define E_GrP3DoCalHdayParaCnt		1

St_GrP3DvlDoPrInfoData	V_GrP3DoCalHdayParaInfo[E_GrP3DoCalHdayParaCnt]	=	
{
	{	L"SchdCnt",				TRUE,		E_GrP3DvlParaTypeByte,				1,	(void*)&((Ptr_GrP3DoCalHdayPara)0)->SchdCnt,NULL,	L"max holiday schedule count"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoCalHday::Cls_GrP3DoCalHday( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoCalBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
		__u32	Tv_Time;
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassCalHday;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaCalHday) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaCalHday, sizeof(m_ParaCalHday) );
		m_ParaCalHday.SchdCnt	=	1;

		// set type
		m_IsBitMode	=	FALSE;
		m_LvlCnt		=	1;

		// update now time
		Tv_Time	=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
		GrTimeDec( Tv_Time, &m_ElemTime[E_GrP3DoCalTimeIdxYear], (__u8*)&m_ElemTime[E_GrP3DoCalTimeIdxMonth],
			(__u8*)&m_ElemTime[E_GrP3DoCalTimeIdxDay], (__u8*)&m_ElemTime[E_GrP3DoCalTimeIdxHour],
			(__u8*)&m_ElemTime[E_GrP3DoCalTimeIdxMin], (__u8*)&m_ElemTime[E_GrP3DoCalTimeIdxSec] );

		m_PtrYear	=	(__u16*)V_GrP3Mng->SetupDataPtrGet( E_GrP3CfgOfsHolyBaseYear );
		m_PtrTbl	=	(__u8*)V_GrP3Mng->SetupDataPtrGet( E_GrP3CfgOfsRecHoly );

		// init
		GrDumyZeroMem( m_DayTbl, sizeof(m_DayTbl) );

}
//--------------------------------------------------------------------
Cls_GrP3DoCalHday::~Cls_GrP3DoCalHday( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalHday::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoCalBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassCalRec;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaCalHday);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaCalHday, sizeof(m_ParaCalHday) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaCalHday)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalHday::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoCalBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaCalHday, sizeof(m_ParaCalHday) , m_PtrScpt , E_GrP3DoClassCalRec );

		// patch holiday count
		if ( 0 == m_ParaCalHday.SchdCnt )
		{
			m_ParaCalHday.SchdCnt	=	1;
		}
		else if ( E_GrP3CfgHoliSchdCnt < m_ParaCalHday.SchdCnt )
		{
			m_ParaCalHday.SchdCnt	=	E_GrP3CfgHoliSchdCnt;
		}
		m_LvlCnt	=	m_ParaCalHday.SchdCnt;
				
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoCalHday::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiUpdate:
				// relate control
				LcElemTmUpdtByCtrl();
				LcValueUpdt();
				LcCalRebuild();
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoCalBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalHday::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoCalBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaCalHday, V_GrP3DoCalHdayParaInfo, E_GrP3DoCalHdayParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalHday::LcCalRebuild( void )
{
	// local -------------------
		__u16	Tv_BaseYear;
		__u16	Tv_ByIdx;
		__u8	Tv_WkIdx;
		__u8	Tv_Val;
		__u8	Tv_DayIdx;

	// code --------------------
		// inherited
		Cls_GrP3DoCalBase::LcCalRebuild();	

		GrDumyZeroMem( m_DayTbl, sizeof(m_DayTbl) );

		// check year range
		Tv_BaseYear	=	E_GrP3CfgHdayBaseYear + (*m_PtrYear);
		if ( (Tv_BaseYear <= m_ElemTime[E_GrP3DoCalTimeIdxYear]) && ((Tv_BaseYear + E_GrP3CfgHdayYearCnt) > m_ElemTime[E_GrP3DoCalTimeIdxYear]) )
		{
			Tv_ByIdx	=	(m_ElemTime[E_GrP3DoCalTimeIdxYear] - Tv_BaseYear) * (12*16);
			Tv_ByIdx	=	Tv_ByIdx + ( (m_ElemTime[E_GrP3DoCalTimeIdxMonth] - 1) * 16);
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
			DbgMsgPrint( "Cls_GrP3DoCalHday::LcCalRebuild - out of day range.\n" );
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrP3DoCalHday::LcMarkValueGet( __u8 A_Day )
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
void	Cls_GrP3DoCalHday::LcProcTouchDay( void )
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
		Tv_BaseYear	=	E_GrP3CfgHdayBaseYear + (*m_PtrYear);
		if ( (Tv_BaseYear <= m_ElemTime[E_GrP3DoCalTimeIdxYear]) && ((Tv_BaseYear + E_GrP3CfgHdayYearCnt) > m_ElemTime[E_GrP3DoCalTimeIdxYear]) )
		{
			// update configuration
			Tv_ByIdx	=	(m_ElemTime[E_GrP3DoCalTimeIdxYear] - Tv_BaseYear) * (12*16);
			Tv_ByIdx	=	Tv_ByIdx + ( (m_ElemTime[E_GrP3DoCalTimeIdxMonth] - 1) * 16);
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
void	Cls_GrP3DoCalHday::RtlInit( void )
{
	// local -------------------
	// code --------------------
		// inherited
		LcValueUpdt();
		Cls_GrP3DoCalBase::RtlInit();

}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalHday::LcNotifyMarkChg( void )
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP3DoCalBase::LcNotifyMarkChg();
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalHday::LcValueUpdt( void )
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
