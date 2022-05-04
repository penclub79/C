/*
		platform calendar record

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoCalRec.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>
#include <P4/GrP4ApiBase.h>
#include <P4/GrP4Bios.h>
#include <GrDebug.h>

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

#define E_GrP4DoCalRecParaCnt		4

St_GrP4DvlDoPrInfoData	V_GrP4DoCalRecParaInfo[E_GrP4DoCalRecParaCnt]	=	
{
	{	L"GvuDev",				TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,				1,	(void*)&((Ptr_GrP4DoCalRecPara)0)->GvuDev,NULL,	L"select device type GVU"	},
	{	L"GvuTime",				TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,				1,	(void*)&((Ptr_GrP4DoCalRecPara)0)->GvuTime,NULL,	L"select time GVU"	},
	{	L"RelCtrlId",			TRUE,		E_GrP4DvlParaTypeScptIdAny,				1,	(void*)&((Ptr_GrP4DoCalRecPara)0)->RelCtrlId,NULL,	L"Relate control id"	},
	{	L"IsRelFcsMove",	TRUE,		E_GrP4DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP4DoCalRecPara)0)->IsRelFcsMove,NULL,	L"auto focus move relate control"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoCalRec::Cls_GrP4DoCalRec(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoCalBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassCalRec;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaCalRec) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaCalRec, sizeof(m_ParaCalRec) );

		// set type
		m_IsBitMode	=	TRUE;
		m_LvlCnt		=	2;

		// init
		m_DayMap		=	0;
		m_RecType		=	E_GrDvrRecDevNoraml;
		m_SelDay		=	0;

		m_PtrGvu		=	(__s32*)V_GrP4Mng->GvuTblGet();
}
//--------------------------------------------------------------------
Cls_GrP4DoCalRec::~Cls_GrP4DoCalRec( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoCalRec::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaCalRec);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaCalRec, sizeof(m_ParaCalRec) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaCalRec)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoCalRec::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoCalBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaCalRec, sizeof(m_ParaCalRec) , m_PtrScpt , E_GrP4DoClassCalRec );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoCalRec::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
		__s32		Tv_Rint[2];
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiUpdate:
				// relate control
				LcValueUpdt();
				LcCalRebuild();
				ReqDraw();
				// relate control
				if ( E_GrP4ScptIdNone != m_ParaCalRec.RelCtrlId )
				{
					Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaCalRec.RelCtrlId );
					if ( NULL != Tv_Obj )
					{
						// update
						Tv_Obj->RtlCommand( E_GrP4UpiUpdate, Tv_Rint, NULL );
					}
				}
				break;
				/*
			case E_GrP4UpiEvtHostIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaCalRec.EvtHostId;
				break;
			case E_GrP4UpiEvtHostIdSet:
				m_ParaCalRec.EvtHostId	=	(__u16)A_PtrInt[1];
				break;
				*/
			default:
				Tv_Result	=	Cls_GrP4DoCalBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoCalRec::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoCalBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaCalRec, V_GrP4DoCalRecParaInfo, E_GrP4DoCalRecParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoCalRec::LcCalRebuild( void )
{
	// local -------------------
		__u32	Tv_Time;
		__s32		Tv_Ireg[4];
	// code --------------------
		// inherited
		Cls_GrP4DoCalBase::LcCalRebuild();	

		// get start time
		Tv_Time	=	GrTimeEnc( m_ElemTime[E_GrP4DoCalTimeIdxYear], (__u8)m_ElemTime[E_GrP4DoCalTimeIdxMonth], 1, 
			0, 0, 0 );
		// load day exist map
		Tv_Ireg[1]	=	(__s32)m_RecType;
		Tv_Ireg[2]	=	(__s32)Tv_Time;
		Tv_Ireg[3]	=	(__s32)m_DayCnt;
		GrP4ApiCall(E_GrP4ApiGroupDayMapGet, Tv_Ireg, NULL, NULL);
		m_DayMap		=	(__u32)Tv_Ireg[0];
		
		m_SelDay	=	0;
}
//--------------------------------------------------------------------
__u8	Cls_GrP4DoCalRec::LcMarkValueGet( __u8 A_Day )
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
void	Cls_GrP4DoCalRec::LcProcTouchDay( void )
{
	// local -------------------
	// code --------------------
		m_SelDay	=	m_CusDay;
		ReqDraw();
		LcNotifyMarkChg();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalRec::RtlInit( void )
{
	// local -------------------
	// code --------------------
		// inherited
		LcValueUpdt();
		Cls_GrP4DoCalBase::RtlInit();

}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalRec::LcNotifyMarkChg( void )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
		__s32	Tv_Rint[2];
	// code --------------------
		// inherited
		Cls_GrP4DoCalBase::LcNotifyMarkChg();

		// update select time
		if ( E_GrP4MngGvuMaxCnt > m_ParaCalRec.GvuTime )
		{
			m_PtrGvu[m_ParaCalRec.GvuTime]	=	GrTimeEnc( m_ElemTime[E_GrP4DoCalTimeIdxYear], 
				(__u8)m_ElemTime[E_GrP4DoCalTimeIdxMonth], m_SelDay, 0, 0, 0 );
		}

		// relate control
		if ( E_GrP4ScptIdNone != m_ParaCalRec.RelCtrlId )
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaCalRec.RelCtrlId );
			if ( NULL != Tv_Obj )
			{
				// update
				Tv_Obj->RtlCommand( E_GrP4UpiUpdate, Tv_Rint, NULL );
				// move focus
				if ( m_ParaCalRec.IsRelFcsMove )
				{
					((Cls_GrP4ConBase*)m_ObjCon)->FocusObjSet( Tv_Obj );
				}
				
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalRec::LcValueUpdt( void )
{
	// local -------------------
		__u8	Tv_RecDev;
		Def_GrTime	Tv_Time;
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
	// code --------------------
		// check able GVU
		if ( (E_GrP4MngGvuMaxCnt > m_ParaCalRec.GvuDev) && (E_GrP4MngGvuMaxCnt > m_ParaCalRec.GvuTime) )
		{
			Tv_RecDev	=	(__u8)m_PtrGvu[m_ParaCalRec.GvuDev];
			if ( E_GrDvrRecDevMirror < Tv_RecDev )
			{
				DbgMsgPrint( "Cls_GrP4DoCalRec::LcValueUpdt - bad device type value ( %d ).\n" , Tv_RecDev );
				Tv_RecDev	=	E_GrDvrRecDevNoraml;
			}
			m_RecType	=	Tv_RecDev;
			Tv_Time	=	(Def_GrTime)m_PtrGvu[m_ParaCalRec.GvuTime];
			if ( 0 == Tv_Time )
			{
				Tv_Time	=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
			}
			// decode data
			GrTimeDec( Tv_Time, &m_ElemTime[E_GrP4DoCalTimeIdxYear], (__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxMonth],
				(__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxDay], 
				(__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxHour], (__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxMin],
				(__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxSec] );
			m_SelDay	=	(__u8)( m_ElemTime[E_GrP4DoCalTimeIdxDay] & 0x1F );
			m_CusDay	=	m_SelDay;
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DoCalRec::LcValueUpdt - ( %d ) bad GUV address.\n" , m_ScptId );
		}
		
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalRec::LcViewTimeChg( void )
{
	// local -------------------
	// code --------------------
		// update select time
		if ( E_GrP4MngGvuMaxCnt > m_ParaCalRec.GvuTime )
		{
			m_PtrGvu[m_ParaCalRec.GvuTime]	=	GrTimeEnc( m_ElemTime[E_GrP4DoCalTimeIdxYear], 
				(__u8)m_ElemTime[E_GrP4DoCalTimeIdxMonth], 1, 0, 0, 0 );
		}
}
//--------------------------------------------------------------------
