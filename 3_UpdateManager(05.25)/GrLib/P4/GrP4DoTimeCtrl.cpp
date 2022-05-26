/*
		platform time control

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoTimeCtrl.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>
#include	<GrTimeTool.h>

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

#define E_GrP4DoTimeCtrlParaCnt		12

St_GrP4DvlDoPrInfoData	V_GrP4DoTimeCtrlParaInfo[E_GrP4DoTimeCtrlParaCnt]	=	
{
	{	L"Paint BG",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTimeCtrlPara)0)->PntBg,NULL,	L"Paint background code.\n0 = None, 1 = basic code"	},
	{	L"Year Id",				TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoTimeCtrlPara)0)->CtrlTbl[E_GrP4DoTimeCtrlYearIdx],NULL,	L"Year control id"	},
	{	L"Month Id",			TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoTimeCtrlPara)0)->CtrlTbl[E_GrP4DoTimeCtrlMonthIdx],NULL,	L"Month control id"	},
	{	L"Day Id",				TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoTimeCtrlPara)0)->CtrlTbl[E_GrP4DoTimeCtrlDayIdx],NULL,	L"Day control id"	},
	{	L"Hour Id",				TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoTimeCtrlPara)0)->CtrlTbl[E_GrP4DoTimeCtrlHourIdx],NULL,	L"Hour control id"	},
	{	L"Minute Id",			TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoTimeCtrlPara)0)->CtrlTbl[E_GrP4DoTimeCtrlMinIdx],NULL,	L"Minute control id"	},
	{	L"Sec Id",				TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoTimeCtrlPara)0)->CtrlTbl[E_GrP4DoTimeCtrlSecIdx],NULL,	L"Sec control id"	},
	{	L"Date divide 1",	TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoTimeCtrlPara)0)->CtrlTbl[E_GrP4DoTimeCtrlDateDiv1Idx],NULL,	L"Date separator 1 control id"	},
	{	L"Date divide 2",	TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoTimeCtrlPara)0)->CtrlTbl[E_GrP4DoTimeCtrlDateDiv2Idx],NULL,	L"Date separator 2 control id"	},
	{	L"IsDispose",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoTimeCtrlPara)0)->IsDispose,NULL,	L"enable dispose control"	},
	{	L"Date Gap",			TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoTimeCtrlPara)0)->DateGap,NULL,	L"space of between each date element"	},
	{	L"AsmEvtValChg",	TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoTimeCtrlPara)0)->AsmEvtValChg,NULL,	L"asm code for value change event"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoTimeCtrl::Cls_GrP4DoTimeCtrl(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassTimeCtrl;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoTimeCtrl) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaDoTimeCtrl, sizeof(m_ParaDoTimeCtrl) );
		m_ParaDoTimeCtrl.PntBg	=	E_GrP4ScptPstIdDflt;

		GrDumyZeroMem( m_ElemTm, sizeof(m_ElemTm) );

		m_HeadPosX	=	0;
		m_Time			=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
		GrTimeDec( m_Time, &m_ElemTm[E_GrP4DoTimeCtrlYearIdx],
			(__u8*)&m_ElemTm[E_GrP4DoTimeCtrlMonthIdx], (__u8*)&m_ElemTm[E_GrP4DoTimeCtrlDayIdx],
			(__u8*)&m_ElemTm[E_GrP4DoTimeCtrlHourIdx],	(__u8*)&m_ElemTm[E_GrP4DoTimeCtrlMinIdx],
			(__u8*)&m_ElemTm[E_GrP4DoTimeCtrlSecIdx] );	// update info

}
//--------------------------------------------------------------------
Cls_GrP4DoTimeCtrl::~Cls_GrP4DoTimeCtrl( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeCtrl::RtlScptParaSubDump( void )
{
#ifdef GR_P4_EDITOR
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassTimeCtrl;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoTimeCtrl);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoTimeCtrl, sizeof(m_ParaDoTimeCtrl) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoTimeCtrl)	+ sizeof(St_GrP4ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeCtrl::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoTimeCtrl, sizeof(m_ParaDoTimeCtrl) , m_PtrScpt , E_GrP4DoClassTimeCtrl );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoTimeCtrl::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiValueGet:
				A_PtrInt[0]	=	(__s32)m_Time;
				break;
			case E_GrP4UpiValueSet:
				m_Time	=	(__u32)A_PtrInt[1];
				GrTimeDec( m_Time, &m_ElemTm[E_GrP4DoTimeCtrlYearIdx],
					(__u8*)&m_ElemTm[E_GrP4DoTimeCtrlMonthIdx], (__u8*)&m_ElemTm[E_GrP4DoTimeCtrlDayIdx],
					(__u8*)&m_ElemTm[E_GrP4DoTimeCtrlHourIdx],	(__u8*)&m_ElemTm[E_GrP4DoTimeCtrlMinIdx],
					(__u8*)&m_ElemTm[E_GrP4DoTimeCtrlSecIdx] );	// update info
				LcCtrlValUpdt();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeCtrl::ProcPaint( void )
{
		PaintScriptPush( m_ParaDoTimeCtrl.PntBg, &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeCtrl::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P4_EDITOR
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoTimeCtrl, V_GrP4DoTimeCtrlParaInfo, E_GrP4DoTimeCtrlParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoTimeCtrl::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
void	Cls_GrP4DoTimeCtrl::RtlInit( void )
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP4DoBase::RtlInit();

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
void	Cls_GrP4DoTimeCtrl::LcCtrlListUpdt( void )
{
	// local -------------------
		St_GrRect	Tv_RcThis;
		St_GrRect	Tv_RcObj;
		__u8	Tv_WkIdx;
		BOOL8	Tv_IsFound;
		__s32	Tv_Pos;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DoTimeCtrlSubCtrlCnt;Tv_WkIdx++ )
		{
			m_ObjTbl[Tv_WkIdx]	=	FindSlaveObjByScptId( m_ParaDoTimeCtrl.CtrlTbl[Tv_WkIdx] );
		}

		// get date head position
		m_HeadPosX	=	0;
		if (ScrRectGet(&Tv_RcThis, TRUE))
		{
			Tv_IsFound	=	FALSE;
			for (Tv_WkIdx=E_GrP4DoTimeCtrlYearIdx; Tv_WkIdx <= E_GrP4DoTimeCtrlDayIdx; Tv_WkIdx++)
			{
				if (NULL != m_ObjTbl[Tv_WkIdx])
				{
					if (m_ObjTbl[Tv_WkIdx]->ScrRectGet(&Tv_RcObj, TRUE))
					{
						Tv_Pos	=	Tv_RcObj.left - Tv_RcThis.left;
						if (Tv_IsFound)
						{
							if (m_HeadPosX > Tv_Pos)
							{
								m_HeadPosX	=	Tv_Pos;
							}
						}
						else
						{
							m_HeadPosX	=	Tv_Pos;
							Tv_IsFound	=	TRUE;
						}
					}
				}
			}

		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoTimeCtrl::LcDisposeItem( __u8 A_Idx, __s32* A_PtrPosX )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_PosX;
		__s32		Tv_SizeX;
		__s32		Tv_SizeY;
		// code --------------------
		Tv_Result	=	FALSE;
		Tv_PosX		=	*A_PtrPosX;

		if ( NULL != m_ObjTbl[A_Idx] )
		{
			// move position
			m_ObjTbl[A_Idx]->PosMoveAbs(Tv_PosX, 0, TRUE, FALSE);
			// get size
			m_ObjTbl[A_Idx]->SizeGet(&Tv_SizeX, &Tv_SizeY);
			// update result
			*A_PtrPosX	=	Tv_PosX + Tv_SizeX;
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeCtrl::LcDisposeCtrls( void )
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
				if ( LcDisposeItem( E_GrP4DoTimeCtrlMonthIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// divide 1
				if ( LcDisposeItem( E_GrP4DoTimeCtrlDateDiv1Idx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// day
				if ( LcDisposeItem( E_GrP4DoTimeCtrlDayIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// divide 2
				if ( LcDisposeItem( E_GrP4DoTimeCtrlDateDiv2Idx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// year
				if ( LcDisposeItem( E_GrP4DoTimeCtrlYearIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				break;
			case E_GrStrDateDispDdMmYy:
				// day
				if ( LcDisposeItem( E_GrP4DoTimeCtrlDayIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// divid 1
				if ( LcDisposeItem( E_GrP4DoTimeCtrlDateDiv1Idx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// month
				if ( LcDisposeItem( E_GrP4DoTimeCtrlMonthIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// divide 2
				if ( LcDisposeItem( E_GrP4DoTimeCtrlDateDiv2Idx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// year
				if ( LcDisposeItem( E_GrP4DoTimeCtrlYearIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				break;
			default:
				// year
				if ( LcDisposeItem( E_GrP4DoTimeCtrlYearIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// divide 1
				if ( LcDisposeItem( E_GrP4DoTimeCtrlDateDiv1Idx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// month
				if ( LcDisposeItem( E_GrP4DoTimeCtrlMonthIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// divide 2
				if ( LcDisposeItem( E_GrP4DoTimeCtrlDateDiv2Idx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				// day
				if ( LcDisposeItem( E_GrP4DoTimeCtrlDayIdx, &Tv_X ) )
				{
					Tv_X	=	Tv_X + (__s32)m_ParaDoTimeCtrl.DateGap;
				}
				break;

		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeCtrl::LcFocusTgUpdt( void )
{
	// local -------------------
		__s32		Tv_ObjCnt;
		Cls_GrP4DoBase*	Tv_ObjTbl[E_GrP4DoTimeCtrlElementCnt];
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
				if ( NULL != m_ObjTbl[E_GrP4DoTimeCtrlMonthIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP4DoTimeCtrlMonthIdx];
					Tv_ObjCnt ++;
				}
				// day
				if ( NULL != m_ObjTbl[E_GrP4DoTimeCtrlDayIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP4DoTimeCtrlDayIdx];
					Tv_ObjCnt ++;
				}
				// year
				if ( NULL != m_ObjTbl[E_GrP4DoTimeCtrlYearIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP4DoTimeCtrlYearIdx];
					Tv_ObjCnt ++;
				}
				break;
			case E_GrStrDateDispDdMmYy:
				// day
				if ( NULL != m_ObjTbl[E_GrP4DoTimeCtrlDayIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP4DoTimeCtrlDayIdx];
					Tv_ObjCnt ++;
				}
				// month
				if ( NULL != m_ObjTbl[E_GrP4DoTimeCtrlMonthIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP4DoTimeCtrlMonthIdx];
					Tv_ObjCnt ++;
				}
				// year
				if ( NULL != m_ObjTbl[E_GrP4DoTimeCtrlYearIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP4DoTimeCtrlYearIdx];
					Tv_ObjCnt ++;
				}
				break;
			default:
				// year
				if ( NULL != m_ObjTbl[E_GrP4DoTimeCtrlYearIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP4DoTimeCtrlYearIdx];
					Tv_ObjCnt ++;
				}
				// month
				if ( NULL != m_ObjTbl[E_GrP4DoTimeCtrlMonthIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP4DoTimeCtrlMonthIdx];
					Tv_ObjCnt ++;
				}
				// day
				if ( NULL != m_ObjTbl[E_GrP4DoTimeCtrlDayIdx] )
				{
					Tv_ObjTbl[Tv_ObjCnt]	=	m_ObjTbl[E_GrP4DoTimeCtrlDayIdx];
					Tv_ObjCnt ++;
				}
				break;
		}

		// time
		for ( Tv_WkIdx=E_GrP4DoTimeCtrlHourIdx;Tv_WkIdx <= E_GrP4DoTimeCtrlSecIdx;Tv_WkIdx++ )
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
			Tv_TgId		=	m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxLeft];
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ObjCnt;Tv_WkIdx++ )
			{
				Tv_ObjTbl[Tv_WkIdx]->FocusTargetSet( E_GrP4DoFocusMvIdxLeft, Tv_TgId );
				Tv_TgId	=	Tv_ObjTbl[Tv_WkIdx]->ScptIdGet();
			}
			// right
			Tv_TgId		=	m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxRight];
			for ( Tv_WkIdx=Tv_ObjCnt - 1;Tv_WkIdx >= 0;Tv_WkIdx-- )
			{
				Tv_ObjTbl[Tv_WkIdx]->FocusTargetSet( E_GrP4DoFocusMvIdxRight, Tv_TgId );
				Tv_TgId	=	Tv_ObjTbl[Tv_WkIdx]->ScptIdGet();
			}
			// up and down
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ObjCnt;Tv_WkIdx++ )
			{
				Tv_ObjTbl[Tv_WkIdx]->FocusTargetSet( E_GrP4DoFocusMvIdxUp, m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxUp] );
				Tv_ObjTbl[Tv_WkIdx]->FocusTargetSet( E_GrP4DoFocusMvIdxDown, m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxDown] );
			}

		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeCtrl::ProcEvtSysStat( __u8 A_Type )
{
	// local -------------------
	// code --------------------
		// time format change event process
		if ( E_GrP4SysStatTimeFmtChg == A_Type )
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
void	Cls_GrP4DoTimeCtrl::ProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg )
{
	// local -------------------
	// code --------------------
		switch( A_PtrMsg->Cmd )
		{
			case E_GrP4MsgCmdSpinValChg:
				ProcSpinValChg( ((Ptr_GrP4MsgObj)A_PtrMsg)->ObjId );
				break;
			default:
				Cls_GrP4DoBase::ProcUiMsg( A_PtrMsg );
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeCtrl::ProcSpinValChg( __u16 A_ObjId )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
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
			for ( Tv_WkIdx=E_GrP4DoTimeCtrlYearIdx;Tv_WkIdx <= E_GrP4DoTimeCtrlSecIdx;Tv_WkIdx++ )
			{
				if ( A_ObjId == m_ParaDoTimeCtrl.CtrlTbl[Tv_WkIdx] )
				{
					Tv_TimeType	=	Tv_WkIdx;
					break;
				}
			}
			// check found
			if ( E_GrP4DoTimeCtrlSecIdx >= Tv_TimeType )
			{
				if ( !Tv_Obj->RtlCommand( E_GrP4UpiValueGet, (__s32*)&Tv_Value, NULL ) )
				{
					Tv_TimeType	=	0xFF;
				}
			}
			// check exist
			if ( E_GrP4DoTimeCtrlSecIdx >= Tv_TimeType )
			{
				Tv_Value	=	Tv_Value & 0xFFFF;
				if ( E_GrP4DoTimeCtrlYearIdx != Tv_TimeType )
				{
					Tv_Value	=	Tv_Value & 0xFF;
				}
				// update value by time type
				m_ElemTm[Tv_TimeType]	=	(__u16)Tv_Value;
				// check patch
				if ( (E_GrP4DoTimeCtrlMonthIdx == Tv_TimeType) || (E_GrP4DoTimeCtrlDayIdx == Tv_TimeType) )
				{
					LcDayLimiteUpdt();
				}
				// value update
				LcValueUpdt();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeCtrl::LcEvtHostUpdt( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__s32		Tv_Rval[2];
	// code --------------------
		for ( Tv_WkIdx=E_GrP4DoTimeCtrlYearIdx;Tv_WkIdx <= E_GrP4DoTimeCtrlSecIdx;Tv_WkIdx++ )
		{
			if ( NULL != m_ObjTbl[Tv_WkIdx] )
			{
				Tv_Rval[1]	=	(__s32)m_ScptId;
				m_ObjTbl[Tv_WkIdx]->RtlCommand( E_GrP4UpiEvtHostIdSet, Tv_Rval, NULL );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeCtrl::LcCtrlValUpdt( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__s32		Tv_Rval[2];
	// code --------------------
		for ( Tv_WkIdx=E_GrP4DoTimeCtrlYearIdx;Tv_WkIdx <= E_GrP4DoTimeCtrlSecIdx;Tv_WkIdx++ )
		{
			if ( NULL != m_ObjTbl[Tv_WkIdx] )
			{
				Tv_Rval[1]	=	(__s32)m_ElemTm[Tv_WkIdx];
				m_ObjTbl[Tv_WkIdx]->RtlCommand( E_GrP4UpiValueSet, Tv_Rval, NULL );
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeCtrl::LcDayLimiteUpdt( void )
{
	// local -------------------
		__u8	Tv_DayCnt;
		Cls_GrP4DoBase*	Tv_Obj;
		__s32		Tv_Rint[2];
	// code --------------------
		Tv_DayCnt	=	GrTimeGetMonthMaxDay( m_ElemTm[E_GrP4DoTimeCtrlYearIdx], (__u8)m_ElemTm[E_GrP4DoTimeCtrlMonthIdx]	);

		if ( NULL != m_ObjTbl[E_GrP4DoTimeCtrlDayIdx] )
		{
			Tv_Obj	=	m_ObjTbl[E_GrP4DoTimeCtrlDayIdx];
			Tv_Rint[0]	=	0;
			Tv_Obj->RtlCommand( E_GrP4UpiMaxValueGet, Tv_Rint, NULL );
			if ( (__s32)Tv_DayCnt != Tv_Rint[0] )
			{
				Tv_Rint[1]	=	(__s32)Tv_DayCnt;
				Tv_Obj->RtlCommand( E_GrP4UpiMaxValueSet, Tv_Rint, NULL );
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DoTimeCtrl::LcValueUpdt( void )
{
	// local -------------------
	// code --------------------
		m_Time	=	GrTimeEnc( m_ElemTm[E_GrP4DoTimeCtrlYearIdx], (__u8)m_ElemTm[E_GrP4DoTimeCtrlMonthIdx], (__u8)m_ElemTm[E_GrP4DoTimeCtrlDayIdx],
			(__u8)m_ElemTm[E_GrP4DoTimeCtrlHourIdx], (__u8)m_ElemTm[E_GrP4DoTimeCtrlMinIdx], (__u8)m_ElemTm[E_GrP4DoTimeCtrlSecIdx] );

		// value change event
		if ( E_GrP4AsmIdNone != m_ParaDoTimeCtrl.AsmEvtValChg )
		{
			V_GrP4Mng->VmRunCode( m_ParaDoTimeCtrl.AsmEvtValChg, this );
		}
}
//--------------------------------------------------------------------

