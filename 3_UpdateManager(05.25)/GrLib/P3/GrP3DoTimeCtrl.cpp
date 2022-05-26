/*
		platform time control

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoTimeCtrl.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>
#include	<GrTimeTool.h>

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

#define E_GrP3DoTimeCtrlParaCnt		12

St_GrP3DvlDoPrInfoData	V_GrP3DoTimeCtrlParaInfo[E_GrP3DoTimeCtrlParaCnt]	=	
{
	{	L"Paint BG",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoTimeCtrlPara)0)->PntBg,NULL,	L"Paint background code.\n0 = None, 1 = basic code"	},
	{	L"Year Id",				TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoTimeCtrlPara)0)->CtrlTbl[E_GrP3DoTimeCtrlYearIdx],NULL,	L"Year control id"	},
	{	L"Month Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoTimeCtrlPara)0)->CtrlTbl[E_GrP3DoTimeCtrlMonthIdx],NULL,	L"Month control id"	},
	{	L"Day Id",				TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoTimeCtrlPara)0)->CtrlTbl[E_GrP3DoTimeCtrlDayIdx],NULL,	L"Day control id"	},
	{	L"Hour Id",				TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoTimeCtrlPara)0)->CtrlTbl[E_GrP3DoTimeCtrlHourIdx],NULL,	L"Hour control id"	},
	{	L"Minute Id",			TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoTimeCtrlPara)0)->CtrlTbl[E_GrP3DoTimeCtrlMinIdx],NULL,	L"Minute control id"	},
	{	L"Sec Id",				TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoTimeCtrlPara)0)->CtrlTbl[E_GrP3DoTimeCtrlSecIdx],NULL,	L"Sec control id"	},
	{	L"Date divide 1",	TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoTimeCtrlPara)0)->CtrlTbl[E_GrP3DoTimeCtrlDateDiv1Idx],NULL,	L"Date separator 1 control id"	},
	{	L"Date divide 2",	TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoTimeCtrlPara)0)->CtrlTbl[E_GrP3DoTimeCtrlDateDiv2Idx],NULL,	L"Date separator 2 control id"	},
	{	L"IsDispose",			TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoTimeCtrlPara)0)->IsDispose,NULL,	L"enable dispose control"	},
	{	L"Date Gap",			TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoTimeCtrlPara)0)->DateGap,NULL,	L"space of between each date element"	},
	{	L"AsmEvtValChg",	TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoTimeCtrlPara)0)->AsmEvtValChg,NULL,	L"asm code for value change event"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoTimeCtrl::Cls_GrP3DoTimeCtrl( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassTimeCtrl;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoTimeCtrl) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaDoTimeCtrl, sizeof(m_ParaDoTimeCtrl) );
		m_ParaDoTimeCtrl.PntBg	=	E_GrP3ScptPstIdDflt;

		GrDumyZeroMem( m_ElemTm, sizeof(m_ElemTm) );

		m_HeadPosX	=	0;
		m_Time			=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
		GrTimeDec( m_Time, &m_ElemTm[E_GrP3DoTimeCtrlYearIdx],
			(__u8*)&m_ElemTm[E_GrP3DoTimeCtrlMonthIdx], (__u8*)&m_ElemTm[E_GrP3DoTimeCtrlDayIdx],
			(__u8*)&m_ElemTm[E_GrP3DoTimeCtrlHourIdx],	(__u8*)&m_ElemTm[E_GrP3DoTimeCtrlMinIdx],
			(__u8*)&m_ElemTm[E_GrP3DoTimeCtrlSecIdx] );	// update info

}
//--------------------------------------------------------------------
Cls_GrP3DoTimeCtrl::~Cls_GrP3DoTimeCtrl( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassTimeCtrl;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoTimeCtrl);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoTimeCtrl, sizeof(m_ParaDoTimeCtrl) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoTimeCtrl)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoTimeCtrl, sizeof(m_ParaDoTimeCtrl) , m_PtrScpt , E_GrP3DoClassTimeCtrl );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoTimeCtrl::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiValueGet:
				A_PtrInt[0]	=	(__s32)m_Time;
				break;
			case E_GrP3UpiValueSet:
				m_Time	=	(__u32)A_PtrInt[1];
				GrTimeDec( m_Time, &m_ElemTm[E_GrP3DoTimeCtrlYearIdx],
					(__u8*)&m_ElemTm[E_GrP3DoTimeCtrlMonthIdx], (__u8*)&m_ElemTm[E_GrP3DoTimeCtrlDayIdx],
					(__u8*)&m_ElemTm[E_GrP3DoTimeCtrlHourIdx],	(__u8*)&m_ElemTm[E_GrP3DoTimeCtrlMinIdx],
					(__u8*)&m_ElemTm[E_GrP3DoTimeCtrlSecIdx] );	// update info
				LcCtrlValUpdt();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::ProcPaint( void )
{
		PaintScriptPush( m_ParaDoTimeCtrl.PntBg, &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoTimeCtrl, V_GrP3DoTimeCtrlParaInfo, E_GrP3DoTimeCtrlParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoTimeCtrl::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;

		/*
		if ( 0 == A_Idx )
		{
			Tv_Result	=	m_StrTxt;
		}
		*/

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::RtlInit( void )
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP3DoBase::RtlInit();

		// update control
		LcCtrlListUpdt();
		// update format
		if ( m_ParaDoTimeCtrl.IsDispose )
		{
			LcDisposeCtrls();
		}

		LcFocusTgUpdt();

		LcEvtHostUpdt();		// event host id update

		// day limite update
		LcDayLimiteUpdt();

		// update control value
		LcCtrlValUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::LcCtrlListUpdt( void )
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__u8	Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DoTimeCtrlSubCtrlCnt;Tv_WkIdx++ )
		{
			m_ObjTbl[Tv_WkIdx]	=	FindSlaveObjByScptId( m_ParaDoTimeCtrl.CtrlTbl[Tv_WkIdx] );
		}

		// get date head position
		m_HeadPosX	=	m_Rect.right;
		for ( Tv_WkIdx=E_GrP3DoTimeCtrlYearIdx;Tv_WkIdx <= E_GrP3DoTimeCtrlDayIdx;Tv_WkIdx++ )
		{
			if ( NULL != m_ObjTbl[Tv_WkIdx] )
			{
				m_ObjTbl[Tv_WkIdx]->RectGet(&Tv_Rc);
				if ( m_HeadPosX > Tv_Rc.left )
				{
					m_HeadPosX	=	Tv_Rc.left;
				}
			}
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoTimeCtrl::LcDisposeItem( __u8 A_Idx, __s32* A_PtrPosX )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_PosX;
		St_GrRect	Tv_Rc;
		__s32		Tv_SizeX;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_PosX		=	*A_PtrPosX;

		if ( NULL != m_ObjTbl[A_Idx] )
		{
			// get info
			m_ObjTbl[A_Idx]->RectGet(&Tv_Rc);
			Tv_SizeX	=	Tv_Rc.right - Tv_Rc.left;
			// patch
			Tv_Rc.left	=	Tv_PosX;
			Tv_Rc.right	=	Tv_PosX + Tv_SizeX;
			// update rect
			m_ObjTbl[A_Idx]->RectSet( &Tv_Rc );
			// update result
			*A_PtrPosX	=	Tv_Rc.right;
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::LcDisposeCtrls( void )
{
	// local -------------------
		__s32		Tv_X;
	// code --------------------
		// get time type
		Tv_X		=	m_HeadPosX;
		switch ( GrStrDateDispTypeGet() )
		{
			case E_GrStrDateDispMmDdYy:
				// month
				if ( LcDisposeItem( E_GrP3DoTimeCtrlMonthIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// divide 1
				if ( LcDisposeItem( E_GrP3DoTimeCtrlDateDiv1Idx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// day
				if ( LcDisposeItem( E_GrP3DoTimeCtrlDayIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// divide 2
				if ( LcDisposeItem( E_GrP3DoTimeCtrlDateDiv2Idx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// year
				if ( LcDisposeItem( E_GrP3DoTimeCtrlYearIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				break;
			case E_GrStrDateDispDdMmYy:
				// day
				if ( LcDisposeItem( E_GrP3DoTimeCtrlDayIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// divid 1
				if ( LcDisposeItem( E_GrP3DoTimeCtrlDateDiv1Idx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// month
				if ( LcDisposeItem( E_GrP3DoTimeCtrlMonthIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// divide 2
				if ( LcDisposeItem( E_GrP3DoTimeCtrlDateDiv2Idx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// year
				if ( LcDisposeItem( E_GrP3DoTimeCtrlYearIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				break;
			default:
				// year
				if ( LcDisposeItem( E_GrP3DoTimeCtrlYearIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// divide 1
				if ( LcDisposeItem( E_GrP3DoTimeCtrlDateDiv1Idx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// month
				if ( LcDisposeItem( E_GrP3DoTimeCtrlMonthIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// divide 2
				if ( LcDisposeItem( E_GrP3DoTimeCtrlDateDiv2Idx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// day
				if ( LcDisposeItem( E_GrP3DoTimeCtrlDayIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				break;

		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::LcFocusTgUpdt( void )
{
	// local -------------------
		__s32		Tv_ObjCnt;
		Cls_GrP3DoBase*	Tv_ObjTbl[E_GrP3DoTimeCtrlElementCnt];
		__s32		Tv_WkIdx;
		__u16	Tv_TgId;
	// code --------------------
		// init
		Tv_ObjCnt	=	0;

		// date sequence
		switch ( GrStrDateDispTypeGet() )
		{
			case E_GrStrDateDispMmDdYy:
				// month
				if ( NULL != m_ObjTbl[E_GrP3DoTimeCtrlMonthIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP3DoTimeCtrlMonthIdx];
					Tv_ObjCnt ++;
				}
				// day
				if ( NULL != m_ObjTbl[E_GrP3DoTimeCtrlDayIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP3DoTimeCtrlDayIdx];
					Tv_ObjCnt ++;
				}
				// year
				if ( NULL != m_ObjTbl[E_GrP3DoTimeCtrlYearIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP3DoTimeCtrlYearIdx];
					Tv_ObjCnt ++;
				}
				break;
			case E_GrStrDateDispDdMmYy:
				// day
				if ( NULL != m_ObjTbl[E_GrP3DoTimeCtrlDayIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP3DoTimeCtrlDayIdx];
					Tv_ObjCnt ++;
				}
				// month
				if ( NULL != m_ObjTbl[E_GrP3DoTimeCtrlMonthIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP3DoTimeCtrlMonthIdx];
					Tv_ObjCnt ++;
				}
				// year
				if ( NULL != m_ObjTbl[E_GrP3DoTimeCtrlYearIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP3DoTimeCtrlYearIdx];
					Tv_ObjCnt ++;
				}
				break;
			default:
				// year
				if ( NULL != m_ObjTbl[E_GrP3DoTimeCtrlYearIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP3DoTimeCtrlYearIdx];
					Tv_ObjCnt ++;
				}
				// month
				if ( NULL != m_ObjTbl[E_GrP3DoTimeCtrlMonthIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP3DoTimeCtrlMonthIdx];
					Tv_ObjCnt ++;
				}
				// day
				if ( NULL != m_ObjTbl[E_GrP3DoTimeCtrlDayIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP3DoTimeCtrlDayIdx];
					Tv_ObjCnt ++;
				}
				break;
		}

		// time
		for ( Tv_WkIdx=E_GrP3DoTimeCtrlHourIdx;Tv_WkIdx <= E_GrP3DoTimeCtrlSecIdx;Tv_WkIdx++ )
		{
			if ( NULL != m_ObjTbl[Tv_WkIdx] )
			{
				Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[Tv_WkIdx];
				Tv_ObjCnt ++;
			}
		}

		// check able
		if ( 0 != Tv_ObjCnt )
		{
			// left 
			Tv_TgId		=	m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxLeft];
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ObjCnt;Tv_WkIdx++ )
			{
				Tv_ObjTbl[Tv_WkIdx]->FocusTargetSet( E_GrP3DoFocusMvIdxLeft, Tv_TgId );
				Tv_TgId	=	Tv_ObjTbl[Tv_WkIdx]->ScptIdGet();
			}
			// right
			Tv_TgId		=	m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxRight];
			for ( Tv_WkIdx=Tv_ObjCnt - 1;Tv_WkIdx >= 0;Tv_WkIdx-- )
			{
				Tv_ObjTbl[Tv_WkIdx]->FocusTargetSet( E_GrP3DoFocusMvIdxRight, Tv_TgId );
				Tv_TgId	=	Tv_ObjTbl[Tv_WkIdx]->ScptIdGet();
			}
			// up and down
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ObjCnt;Tv_WkIdx++ )
			{
				Tv_ObjTbl[Tv_WkIdx]->FocusTargetSet( E_GrP3DoFocusMvIdxUp, m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxUp] );
				Tv_ObjTbl[Tv_WkIdx]->FocusTargetSet( E_GrP3DoFocusMvIdxDown, m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxDown] );
			}

		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::ProcEvtSysStat( __u8 A_Type )
{
	// local -------------------
	// code --------------------
		// time format change event process
		if ( E_GrP3SysStatTimeFmtChg == A_Type )
		{
			// change time format
			if ( m_ParaDoTimeCtrl.IsDispose )
			{
				LcDisposeCtrls();
				LcFocusTgUpdt();
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::ProcUiMsg( Ptr_GrP3MsgHd A_PtrMsg )
{
	// local -------------------
	// code --------------------
		switch( A_PtrMsg->Cmd )
		{
			case E_GrP3MsgCmdSpinValChg:
				ProcSpinValChg( ((Ptr_GrP3MsgObj)A_PtrMsg)->ObjId );
				break;
			default:
				Cls_GrP3DoBase::ProcUiMsg( A_PtrMsg );
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::ProcSpinValChg( __u16 A_ObjId )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
		__u8	Tv_WkIdx;
		__u8	Tv_TimeType;
		__u32	Tv_Value;
	// code --------------------
		// check exist object
		Tv_Obj	=	FindSlaveObjByScptId( A_ObjId );
		if ( NULL != Tv_Obj )
		{
			// find type
			Tv_TimeType		=	0xFF;
			for ( Tv_WkIdx=E_GrP3DoTimeCtrlYearIdx;Tv_WkIdx <= E_GrP3DoTimeCtrlSecIdx;Tv_WkIdx++ )
			{
				if ( A_ObjId == m_ParaDoTimeCtrl.CtrlTbl[Tv_WkIdx] )
				{
					Tv_TimeType	=	Tv_WkIdx;
					break;
				}
			}
			// check found
			if ( E_GrP3DoTimeCtrlSecIdx >= Tv_TimeType )
			{
				if ( !Tv_Obj->RtlCommand( E_GrP3UpiValueGet, (__s32*)&Tv_Value, NULL ) )
				{
					Tv_TimeType	=	0xFF;
				}
			}
			// check exist
			if ( E_GrP3DoTimeCtrlSecIdx >= Tv_TimeType )
			{
				Tv_Value	=	Tv_Value & 0xFFFF;
				if ( E_GrP3DoTimeCtrlYearIdx != Tv_TimeType )
				{
					Tv_Value	=	Tv_Value & 0xFF;
				}
				// update value by time type
				m_ElemTm[Tv_TimeType]	=	(__u16)Tv_Value;
				// check patch
				if ( (E_GrP3DoTimeCtrlMonthIdx == Tv_TimeType) || (E_GrP3DoTimeCtrlDayIdx == Tv_TimeType) )
				{
					LcDayLimiteUpdt();
				}
				// value update
				LcValueUpdt();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::LcEvtHostUpdt( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__s32		Tv_Rval[2];
	// code --------------------
		for ( Tv_WkIdx=E_GrP3DoTimeCtrlYearIdx;Tv_WkIdx <= E_GrP3DoTimeCtrlSecIdx;Tv_WkIdx++ )
		{
			if ( NULL != m_ObjTbl[Tv_WkIdx] )
			{
				Tv_Rval[1]	=	(__s32)m_ScptId;
				m_ObjTbl[Tv_WkIdx]->RtlCommand( E_GrP3UpiEvtHostIdSet, Tv_Rval, NULL );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::LcCtrlValUpdt( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__s32		Tv_Rval[2];
	// code --------------------
		for ( Tv_WkIdx=E_GrP3DoTimeCtrlYearIdx;Tv_WkIdx <= E_GrP3DoTimeCtrlSecIdx;Tv_WkIdx++ )
		{
			if ( NULL != m_ObjTbl[Tv_WkIdx] )
			{
				Tv_Rval[1]	=	(__s32)m_ElemTm[Tv_WkIdx];
				m_ObjTbl[Tv_WkIdx]->RtlCommand( E_GrP3UpiValueSet, Tv_Rval, NULL );
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::LcDayLimiteUpdt( void )
{
	// local -------------------
		__u8	Tv_DayCnt;
		Cls_GrP3DoBase*	Tv_Obj;
		__s32		Tv_Rint[2];
	// code --------------------
		Tv_DayCnt	=	GrTimeGetMonthMaxDay( m_ElemTm[E_GrP3DoTimeCtrlYearIdx], (__u8)m_ElemTm[E_GrP3DoTimeCtrlMonthIdx]	);

		if ( NULL != m_ObjTbl[E_GrP3DoTimeCtrlDayIdx] )
		{
			Tv_Obj	=	m_ObjTbl[E_GrP3DoTimeCtrlDayIdx];
			Tv_Rint[0]	=	0;
			Tv_Obj->RtlCommand( E_GrP3UpiMaxValueGet, Tv_Rint, NULL );
			if ( (__s32)Tv_DayCnt != Tv_Rint[0] )
			{
				Tv_Rint[1]	=	(__s32)Tv_DayCnt;
				Tv_Obj->RtlCommand( E_GrP3UpiMaxValueSet, Tv_Rint, NULL );
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3DoTimeCtrl::LcValueUpdt( void )
{
	// local -------------------
	// code --------------------
		m_Time	=	GrTimeEnc( m_ElemTm[E_GrP3DoTimeCtrlYearIdx], (__u8)m_ElemTm[E_GrP3DoTimeCtrlMonthIdx], (__u8)m_ElemTm[E_GrP3DoTimeCtrlDayIdx],
			(__u8)m_ElemTm[E_GrP3DoTimeCtrlHourIdx], (__u8)m_ElemTm[E_GrP3DoTimeCtrlMinIdx], (__u8)m_ElemTm[E_GrP3DoTimeCtrlSecIdx] );

		// value change event
		if ( E_GrP3AsmIdNone != m_ParaDoTimeCtrl.AsmEvtValChg )
		{
			V_GrP3Mng->VmRunCode( m_ParaDoTimeCtrl.AsmEvtValChg, this );
		}
}
//--------------------------------------------------------------------

