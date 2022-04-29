/*
		platform calendar record

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoCalRec.h>
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

#define E_GrP3DoCalRecParaCnt		4

St_GrP3DvlDoPrInfoData	V_GrP3DoCalRecParaInfo[E_GrP3DoCalRecParaCnt]	=	
{
	{	L"GvuDev",				TRUE,		E_GrP3DvlParaTypeAsmGvuAdr,				1,	(void*)&((Ptr_GrP3DoCalRecPara)0)->GvuDev,NULL,	L"select device type GVU"	},
	{	L"GvuTime",				TRUE,		E_GrP3DvlParaTypeAsmGvuAdr,				1,	(void*)&((Ptr_GrP3DoCalRecPara)0)->GvuTime,NULL,	L"select time GVU"	},
	{	L"RelCtrlId",			TRUE,		E_GrP3DvlParaTypeScptIdAny,				1,	(void*)&((Ptr_GrP3DoCalRecPara)0)->RelCtrlId,NULL,	L"Relate control id"	},
	{	L"IsRelFcsMove",	TRUE,		E_GrP3DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP3DoCalRecPara)0)->IsRelFcsMove,NULL,	L"auto focus move relate control"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoCalRec::Cls_GrP3DoCalRec( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoCalBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassCalRec;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaCalRec) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaCalRec, sizeof(m_ParaCalRec) );

		// set type
		m_IsBitMode	=	TRUE;
		m_LvlCnt		=	2;

		// init
		m_DayMap		=	0;
		m_RecType		=	E_GrDvrRecDevNoraml;
		m_SelDay		=	0;

		m_PtrGvu		=	(__s32*)V_GrP3Mng->GvuTblGet();
}
//--------------------------------------------------------------------
Cls_GrP3DoCalRec::~Cls_GrP3DoCalRec( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalRec::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaCalRec);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaCalRec, sizeof(m_ParaCalRec) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaCalRec)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalRec::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoCalBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaCalRec, sizeof(m_ParaCalRec) , m_PtrScpt , E_GrP3DoClassCalRec );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoCalRec::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
		__s32		Tv_Rint[2];
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiUpdate:
				// relate control
				LcValueUpdt();
				LcCalRebuild();
				ReqDraw();
				// relate control
				if ( E_GrP3ScptIdNone != m_ParaCalRec.RelCtrlId )
				{
					Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaCalRec.RelCtrlId );
					if ( NULL != Tv_Obj )
					{
						// update
						Tv_Obj->RtlCommand( E_GrP3UpiUpdate, Tv_Rint, NULL );
					}
				}
				break;
				/*
			case E_GrP3UpiEvtHostIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaCalRec.EvtHostId;
				break;
			case E_GrP3UpiEvtHostIdSet:
				m_ParaCalRec.EvtHostId	=	(__u16)A_PtrInt[1];
				break;
				*/
			default:
				Tv_Result	=	Cls_GrP3DoCalBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalRec::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoCalBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaCalRec, V_GrP3DoCalRecParaInfo, E_GrP3DoCalRecParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalRec::LcCalRebuild( void )
{
	// local -------------------
		__u32	Tv_Time;
		__s32		Tv_Ireg[4];
	// code --------------------
		// inherited
		Cls_GrP3DoCalBase::LcCalRebuild();	

		// get start time
		Tv_Time	=	GrTimeEnc( m_ElemTime[E_GrP3DoCalTimeIdxYear], (__u8)m_ElemTime[E_GrP3DoCalTimeIdxMonth], 1, 
			0, 0, 0 );
		// load day exist map
		Tv_Ireg[1]	=	(__s32)m_RecType;
		Tv_Ireg[2]	=	(__s32)Tv_Time;
		Tv_Ireg[3]	=	(__s32)m_DayCnt;
		GrP3BiosGroupDayMapGet( Tv_Ireg, NULL );
		m_DayMap		=	(__u32)Tv_Ireg[0];
		
		m_SelDay	=	0;
}
//--------------------------------------------------------------------
__u8	Cls_GrP3DoCalRec::LcMarkValueGet( __u8 A_Day )
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_Mask;
	// code --------------------
		Tv_Result	=	0;
		// record data exist
		Tv_Mask		=	1 << (A_Day - 1);
		if ( 0 != (m_DayMap & Tv_Mask) )
		{
			Tv_Result	=	1;
		}
		// select
		if ( m_SelDay == A_Day )
		{
			Tv_Result	=	Tv_Result | 2;
		}
		return	Tv_Result;		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalRec::LcProcTouchDay( void )
{
	// local -------------------
	// code --------------------
		m_SelDay	=	m_CusDay;
		ReqDraw();
		LcNotifyMarkChg();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalRec::RtlInit( void )
{
	// local -------------------
	// code --------------------
		// inherited
		LcValueUpdt();
		Cls_GrP3DoCalBase::RtlInit();

}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalRec::LcNotifyMarkChg( void )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
		__s32	Tv_Rint[2];
	// code --------------------
		// inherited
		Cls_GrP3DoCalBase::LcNotifyMarkChg();

		// update select time
		if ( E_GrP3MngGvuMaxCnt > m_ParaCalRec.GvuTime )
		{
			m_PtrGvu[m_ParaCalRec.GvuTime]	=	GrTimeEnc( m_ElemTime[E_GrP3DoCalTimeIdxYear], 
				(__u8)m_ElemTime[E_GrP3DoCalTimeIdxMonth], m_SelDay, 0, 0, 0 );
		}

		// relate control
		if ( E_GrP3ScptIdNone != m_ParaCalRec.RelCtrlId )
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaCalRec.RelCtrlId );
			if ( NULL != Tv_Obj )
			{
				// update
				Tv_Obj->RtlCommand( E_GrP3UpiUpdate, Tv_Rint, NULL );
				// move focus
				if ( m_ParaCalRec.IsRelFcsMove )
				{
					((Cls_GrP3ConBase*)m_ObjCon)->FocusObjSet( Tv_Obj );
				}
				
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalRec::LcValueUpdt( void )
{
	// local -------------------
		__u8	Tv_RecDev;
		Def_GrTime	Tv_Time;
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
	// code --------------------
		// check able GVU
		if ( (E_GrP3MngGvuMaxCnt > m_ParaCalRec.GvuDev) && (E_GrP3MngGvuMaxCnt > m_ParaCalRec.GvuTime) )
		{
			Tv_RecDev	=	(__u8)m_PtrGvu[m_ParaCalRec.GvuDev];
			if ( E_GrDvrRecDevMirror < Tv_RecDev )
			{
				DbgMsgPrint( "Cls_GrP3DoCalRec::LcValueUpdt - bad device type value ( %d ).\n" , Tv_RecDev );
				Tv_RecDev	=	E_GrDvrRecDevNoraml;
			}
			m_RecType	=	Tv_RecDev;
			Tv_Time	=	(Def_GrTime)m_PtrGvu[m_ParaCalRec.GvuTime];
			if ( 0 == Tv_Time )
			{
				Tv_Time	=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
			}
			// decode data
			GrTimeDec( Tv_Time, &m_ElemTime[E_GrP3DoCalTimeIdxYear], (__u8*)&m_ElemTime[E_GrP3DoCalTimeIdxMonth],
				(__u8*)&m_ElemTime[E_GrP3DoCalTimeIdxDay], 
				(__u8*)&m_ElemTime[E_GrP3DoCalTimeIdxHour], (__u8*)&m_ElemTime[E_GrP3DoCalTimeIdxMin],
				(__u8*)&m_ElemTime[E_GrP3DoCalTimeIdxSec] );
			m_SelDay	=	(__u8)( m_ElemTime[E_GrP3DoCalTimeIdxDay] & 0x1F );
			m_CusDay	=	m_SelDay;
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DoCalRec::LcValueUpdt - ( %d ) bad GUV address.\n" , m_ScptId );
		}
		
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoCalRec::LcViewTimeChg( void )
{
	// local -------------------
	// code --------------------
		// update select time
		if ( E_GrP3MngGvuMaxCnt > m_ParaCalRec.GvuTime )
		{
			m_PtrGvu[m_ParaCalRec.GvuTime]	=	GrTimeEnc( m_ElemTime[E_GrP3DoCalTimeIdxYear], 
				(__u8)m_ElemTime[E_GrP3DoCalTimeIdxMonth], 1, 0, 0, 0 );
		}
}
//--------------------------------------------------------------------
