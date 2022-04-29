/*
		platform time label

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P4/GrP4DoTimeLbl.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <P4/GrP4Script.h>
#include <P4/GrP4ConBase.h>
#include <P4/GrP4AsmBase.h>
#include <P4/GrP4Mng.h>
#include <P4/GrP4UiCmd.h>
#include <P4/GrP4ApiBase.h>

#ifdef GR_P4_EDITOR
#include	<P4/Dvl/GrP4DvlBase.h>
#include	<P4/Dvl/GrP4DvlMng.h>
#endif

//====================================================================
//local const

#define E_GrP4DoTimeLblChkPriod		5

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

#define E_GrP4DoTimeLblParaCnt		39

St_GrP4DvlDoPrInfoData	V_GrP4DoTimeLblParaInfo[E_GrP4DoTimeLblParaCnt]	=	
{
	{	L"Paint BG",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntBg,NULL,	L"Paint background code.\n0 = None, 1 = basic code"	},

	{	L"Paint Year",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxYear],NULL,	L"Paint year code"	},
	{	L"Paint Month",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxMon],NULL,	L"Paint month code"	},
	{	L"Paint day",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxDay],NULL,	L"Paint day code"	},
	{	L"Paint hour",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxHour],NULL,	L"Paint hour code"	},
	{	L"Paint minute",	TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxMin],NULL,	L"Paint minute code"	},
	{	L"Paint sec",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxSec],NULL,	L"Paint second code"	},

	{	L"Paint unit year",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxUyear],NULL,	L"Paint unit year code"	},
	{	L"Paint unit month",	TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxUmon],NULL,	L"Paint unit month code"	},
	{	L"Paint unit day",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxUday],NULL,	L"Paint unit day code"	},
	{	L"Paint unit hour",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxUhour],NULL,	L"Paint unit hour code"	},
	{	L"Paint unit minute",	TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxUmin],NULL,	L"Paint unit minute code"	},
	{	L"Paint unit sec",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxUsec],NULL,	L"Paint unit second code"	},

	{	L"Paint summer time",	TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxSm],NULL,	L"Paint summer time code"	},
	{	L"Paint AM / PM",	TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->PntItem[E_GrP4DoTimeLblIdxApm],NULL,	L"Paint am / pm code"	},

	{	L"width year",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxYear],NULL,	L"width year"	},
	{	L"width month",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxMon],NULL,	L"width month"	},
	{	L"width day",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxDay],NULL,	L"width day"	},
	{	L"width hour",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxHour],NULL,	L"width hour"	},
	{	L"width minute",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxMin],NULL,	L"width minute"	},
	{	L"width sec",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxSec],NULL,	L"width second"	},

	{	L"width unit year",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxUyear],NULL,	L"width unit year"	},
	{	L"width unit month",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxUmon],NULL,	L"width unit month"	},
	{	L"width unit day",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxUday],NULL,	L"width unit day"	},
	{	L"width unit hour",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxUhour],NULL,	L"width unit hour"	},
	{	L"width unit minute",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxUmin],NULL,	L"width unit minute"	},
	{	L"width unit sec",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxUsec],NULL,	L"width unit second"	},

	{	L"width summer time",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxSm],NULL,	L"width summer time"	},
	{	L"width am / pm",	TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->ItemWidth[E_GrP4DoTimeLblIdxApm],NULL,	L"width am / pm"	},

	{	L"text unit year",		TRUE,		E_GrP4DvlParaTypeTxtId,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->TxtUnit[E_GrP4DoTimeLblIdxUyear],NULL,	L"text unit year"	},
	{	L"text unit month",		TRUE,		E_GrP4DvlParaTypeTxtId,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->TxtUnit[E_GrP4DoTimeLblIdxUmon],NULL,	L"text unit month"	},
	{	L"text unit day",			TRUE,		E_GrP4DvlParaTypeTxtId,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->TxtUnit[E_GrP4DoTimeLblIdxUday],NULL,	L"text unit day"	},
	{	L"text unit hour",		TRUE,		E_GrP4DvlParaTypeTxtId,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->TxtUnit[E_GrP4DoTimeLblIdxUhour],NULL,	L"text unit hour"	},
	{	L"text unit minute",	TRUE,		E_GrP4DvlParaTypeTxtId,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->TxtUnit[E_GrP4DoTimeLblIdxUmin],NULL,	L"text unit minute"	},
	{	L"text unit sec",			TRUE,		E_GrP4DvlParaTypeTxtId,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->TxtUnit[E_GrP4DoTimeLblIdxUsec],NULL,	L"text unit second"	},

	{	L"gap head",			TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->GapHead,NULL,	L"gap head"	},
	{	L"gap middle",		TRUE,		E_GrP4DvlParaTypeWord,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->GapMid,NULL,	L"gap middle"	},

	{	L"time type",			TRUE,		E_GrP4DvlParaTypeByte,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->TimeType,NULL,	L"time type \n 0=fix, 1=now time, 2=play time"	},

	{	L"date unit fix",	TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4DoTimeLblPara)0)->IsDateUnitFix,NULL,	L"fix date unit sequence"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoTimeLbl::Cls_GrP4DoTimeLbl(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassTimeLbl;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaTimeLbl) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaTimeLbl, sizeof(m_ParaTimeLbl) );
		m_ParaTimeLbl.PntBg	=	E_GrP4ScptPstIdDflt;

		m_ChkTimer	=	0;
		m_IsSm			=	FALSE;

		m_Time			=	0;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DoTimeLblElemCnt;Tv_WkIdx++ )
		{
			m_ElemVal[Tv_WkIdx]	=	0xFFFF;
			GrStrClear( m_ElemTxt[Tv_WkIdx] );
		}

}
//--------------------------------------------------------------------
Cls_GrP4DoTimeLbl::~Cls_GrP4DoTimeLbl( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoTimeLbl::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassTimeLbl;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaTimeLbl);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaTimeLbl, sizeof(m_ParaTimeLbl) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaTimeLbl)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeLbl::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaTimeLbl, sizeof(m_ParaTimeLbl) , m_PtrScpt , E_GrP4DoClassTimeLbl );

		LcItemPosBuild();
		LcTimeUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoTimeLbl::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiValueSet:
				if ( E_GrP4DoTimeLblFixTime == m_ParaTimeLbl.TimeType )
				{
					m_Time	=	(Def_GrTime)A_PtrInt[1];
					if ( LcTimeUpdt() )
					{
						ReqDraw();
					}
				}
				break;
			case E_GrP4UpiValueGet:
				A_PtrInt[0]	=	(__s32)m_Time;
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeLbl::ProcPaint( void )
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		
		// item
		Tv_Rc.top			=	m_PntRcScr.top;
		Tv_Rc.bottom	=	m_PntRcScr.bottom;
		for ( m_PntTimeMode = 0;m_PntTimeMode < E_GrP4DoTimeLblItemCnt;m_PntTimeMode++ )
		{
			// check paint able
			if ( (0 != m_ParaTimeLbl.ItemWidth[m_PntTimeMode]) && (E_GrP4ScptPstIdNone != m_ParaTimeLbl.PntItem[m_PntTimeMode]) )
			{
				if ( E_GrP4DoTimeLblIdxSm == m_PntTimeMode )
				{
					if ( m_IsSm  )
					{
						Tv_Rc.left	=	m_PntRcScr.left + (__s32)m_ItemPosTbl[m_PntTimeMode];
						Tv_Rc.right	=	Tv_Rc.left + (__s32)m_ParaTimeLbl.ItemWidth[m_PntTimeMode];
						PaintScriptPush( m_ParaTimeLbl.PntItem[m_PntTimeMode], &Tv_Rc );
					}
				}
				else
				{
					Tv_Rc.left	=	m_PntRcScr.left + (__s32)m_ItemPosTbl[m_PntTimeMode];
					Tv_Rc.right	=	Tv_Rc.left + (__s32)m_ParaTimeLbl.ItemWidth[m_PntTimeMode];
					PaintScriptPush( m_ParaTimeLbl.PntItem[m_PntTimeMode], &Tv_Rc );
				}
			}
		}

		// background
		PaintScriptPush( m_ParaTimeLbl.PntBg, &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoTimeLbl::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaTimeLbl, V_GrP4DoTimeLblParaInfo, E_GrP4DoTimeLblParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoTimeLbl::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			if ( E_GrP4DoTimeLblUnitCnt > m_PntTimeMode )
			{
				// unit paint
				Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaTimeLbl.TxtUnit[m_PntTimeMode] );
			}
			else if ( E_GrP4DoTimeLblIdxSec >= m_PntTimeMode )
			{
				// time paint
				Tv_Result	=	m_ElemTxt[m_PntTimeMode - E_GrP4DoTimeLblUnitCnt];
			}
			else if ( E_GrP4DoTimeLblIdxApm == m_PntTimeMode )
			{
				if ( 12 > m_ElemVal[3] )
				{
					Tv_Result	=	V_GrP4Mng->TextPtrGet( E_GrP4ScptTxtIdTimeAm );
				}
				else
				{
					Tv_Result	=	V_GrP4Mng->TextPtrGet( E_GrP4ScptTxtIdTimePm );
				}
				
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoTimeLbl::LcTimeUpdt( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		__u16	Tv_Val[E_GrP4DoTimeLblElemCnt];
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 != m_Time )
		{
			Tv_Val[1]	=	0;
			Tv_Val[2]	=	0;
			Tv_Val[3]	=	0;
			Tv_Val[4]	=	0;
			Tv_Val[5]	=	0;
			GrTimeDec( m_Time, &Tv_Val[0], (__u8*)&Tv_Val[1], (__u8*)&Tv_Val[2],
				(__u8*)&Tv_Val[3], (__u8*)&Tv_Val[4], (__u8*)&Tv_Val[5] );

			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DoTimeLblElemCnt;Tv_WkIdx++ )
			{
				// check different
				if ( Tv_Val[Tv_WkIdx] != m_ElemVal[Tv_WkIdx] )
				{
					// update text
					m_ElemVal[Tv_WkIdx]	=	Tv_Val[Tv_WkIdx];
					if ( (3 == Tv_WkIdx) && (0 != m_ParaTimeLbl.ItemWidth[E_GrP4DoTimeLblIdxApm]) )
					{
						if ( 0 == Tv_Val[Tv_WkIdx] )
						{
							Tv_Val[Tv_WkIdx]	=	12;
						}
						else if ( 12 < Tv_Val[Tv_WkIdx] )
						{
							Tv_Val[Tv_WkIdx]	=	Tv_Val[Tv_WkIdx] - 12;
						}
					}
					if ( 3 > Tv_WkIdx )
					{
						GrStrIntToWstr( m_ElemTxt[Tv_WkIdx], (__s32)Tv_Val[Tv_WkIdx] );
					}
					else
					{
						GrStrIntToWstr( m_ElemTxt[Tv_WkIdx], (__s32)Tv_Val[Tv_WkIdx], 2, TRUE, FALSE );
					}
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeLbl::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
		BOOL8	Tv_IsRedraw;
		__s32		Tv_Rint[2];
		__u32 Tv_Time;
	// code --------------------
		if ( E_GrP4DoTimeLblFixTime != m_ParaTimeLbl.TimeType )
		{
			Tv_IsRedraw = FALSE;
			// get time
			if ( E_GrP4DoTimeLblNowTime == m_ParaTimeLbl.TimeType )
			{
				Tv_Time = GrTimeGet( &Tv_Mili, &Tv_IsSm );
			}
			else 
			{
				// play time get
				GrP4ApiCall(E_GrP4ApiPlayTimeGet, Tv_Rint, NULL, NULL);
				Tv_Time	=	(Def_GrTime)Tv_Rint[0];
				Tv_IsSm	=	(BOOL8)Tv_Rint[1];
			}

			if(m_Time != Tv_Time)
			{
				m_Time = Tv_Time;
				// check redraw
				Tv_IsRedraw	=	LcTimeUpdt();
			}

			//check summer time
			if(m_IsSm != Tv_IsSm)
			{
				m_IsSm	=	Tv_IsSm;
				Tv_IsRedraw		= TRUE;
				LcItemPosBuild();
			}

			if(Tv_IsRedraw)
			{
				ReqDraw();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeLbl::LcItemPosBuild( void )
{
	// local -------------------
		__u8	Tv_DispType;
		__u8	Tv_WkIdx;
		__u8	Tv_ItmIdx;
		__u16	Tv_Pos;
		__u8	Tv_SeqTbl[6];
		
	// code --------------------
		// init
		Tv_Pos	=	m_ParaTimeLbl.GapHead;
		// date
		Tv_DispType	=	GrStrDateDispTypeGet();
		switch ( Tv_DispType )
		{
			case E_GrStrDateDispYyMmDd:
				// yy mm dd
				Tv_SeqTbl[0]	=	E_GrP4DoTimeLblIdxYear;
				Tv_SeqTbl[1]	=	E_GrP4DoTimeLblIdxUyear;
				Tv_SeqTbl[2]	=	E_GrP4DoTimeLblIdxMon;
				Tv_SeqTbl[3]	=	E_GrP4DoTimeLblIdxUmon;
				Tv_SeqTbl[4]	=	E_GrP4DoTimeLblIdxDay;
				Tv_SeqTbl[5]	=	E_GrP4DoTimeLblIdxUday;
				break;
			case E_GrStrDateDispMmDdYy:
				// mm dd yy
				Tv_SeqTbl[0]	=	E_GrP4DoTimeLblIdxMon;
				Tv_SeqTbl[1]	=	E_GrP4DoTimeLblIdxUmon;
				Tv_SeqTbl[2]	=	E_GrP4DoTimeLblIdxDay;
				Tv_SeqTbl[3]	=	E_GrP4DoTimeLblIdxUday;
				Tv_SeqTbl[4]	=	E_GrP4DoTimeLblIdxYear;
				Tv_SeqTbl[5]	=	E_GrP4DoTimeLblIdxUyear;
				break;
			default:
				// dd mm yy
				Tv_SeqTbl[0]	=	E_GrP4DoTimeLblIdxDay;
				Tv_SeqTbl[1]	=	E_GrP4DoTimeLblIdxUday;
				Tv_SeqTbl[2]	=	E_GrP4DoTimeLblIdxMon;
				Tv_SeqTbl[3]	=	E_GrP4DoTimeLblIdxUmon;
				Tv_SeqTbl[4]	=	E_GrP4DoTimeLblIdxYear;
				Tv_SeqTbl[5]	=	E_GrP4DoTimeLblIdxUyear;
				break;
		}
		
		if ( m_ParaTimeLbl.IsDateUnitFix )
		{
			Tv_SeqTbl[1]	=	E_GrP4DoTimeLblIdxUyear;
			Tv_SeqTbl[3]	=	E_GrP4DoTimeLblIdxUmon;
			Tv_SeqTbl[5]	=	E_GrP4DoTimeLblIdxUday;
		}

		for ( Tv_WkIdx=0;Tv_WkIdx < 6;Tv_WkIdx++ )
		{
			Tv_ItmIdx	=	Tv_SeqTbl[Tv_WkIdx];
			m_ItemPosTbl[Tv_ItmIdx]	=	Tv_Pos;
			Tv_Pos	=	Tv_Pos + m_ParaTimeLbl.ItemWidth[Tv_ItmIdx];
		}
		
		// middle gap
		Tv_Pos	=	Tv_Pos + m_ParaTimeLbl.GapMid;

		// summer time
		if(m_IsSm)
		{
			m_ItemPosTbl[E_GrP4DoTimeLblIdxSm]	=	Tv_Pos;
			Tv_Pos	=	Tv_Pos + m_ParaTimeLbl.ItemWidth[E_GrP4DoTimeLblIdxSm];
		}

		// am / pm
		m_ItemPosTbl[E_GrP4DoTimeLblIdxApm]	=	Tv_Pos;
		Tv_Pos	=	Tv_Pos + m_ParaTimeLbl.ItemWidth[E_GrP4DoTimeLblIdxApm];

		// time
		m_ItemPosTbl[E_GrP4DoTimeLblIdxHour]	=	Tv_Pos;
		Tv_Pos	=	Tv_Pos + m_ParaTimeLbl.ItemWidth[E_GrP4DoTimeLblIdxHour];
		m_ItemPosTbl[E_GrP4DoTimeLblIdxUhour]	=	Tv_Pos;
		Tv_Pos	=	Tv_Pos + m_ParaTimeLbl.ItemWidth[E_GrP4DoTimeLblIdxUhour];
		
		m_ItemPosTbl[E_GrP4DoTimeLblIdxMin]	=	Tv_Pos;
		Tv_Pos	=	Tv_Pos + m_ParaTimeLbl.ItemWidth[E_GrP4DoTimeLblIdxMin];
		m_ItemPosTbl[E_GrP4DoTimeLblIdxUmin]	=	Tv_Pos;
		Tv_Pos	=	Tv_Pos + m_ParaTimeLbl.ItemWidth[E_GrP4DoTimeLblIdxUmin];
		
		m_ItemPosTbl[E_GrP4DoTimeLblIdxSec]	=	Tv_Pos;
		Tv_Pos	=	Tv_Pos + m_ParaTimeLbl.ItemWidth[E_GrP4DoTimeLblIdxSec];
		m_ItemPosTbl[E_GrP4DoTimeLblIdxUsec]	=	Tv_Pos;
		Tv_Pos	=	Tv_Pos + m_ParaTimeLbl.ItemWidth[E_GrP4DoTimeLblIdxUsec];

		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeLbl::ProcEvtSysStat( __u8 A_Type )
{
	// local -------------------
	// code --------------------
		// time format change event process
		if ( E_GrP4SysStatTimeFmtChg == A_Type )
		{
			LcItemPosBuild();
			ReqDraw();
		}
		
}
//--------------------------------------------------------------------
