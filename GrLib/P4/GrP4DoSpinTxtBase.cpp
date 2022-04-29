/*
		platform spin text base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSpinTxtBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>

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

#define E_GrP4DoSpinTxtBaseParaCnt		41

St_GrP4DvlDoPrInfoData	V_GrP4DoSpinTxtBaseParaInfo[E_GrP4DoSpinTxtBaseParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->PntStat[E_GrP4DoSpinTxtStatDisable],NULL,	L"Paint status disable"	},
	{	L"Paint Normal",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->PntStat[E_GrP4DoSpinTxtStatNormal],NULL,	L"Paint status normal"	},
	{	L"Paint Focus",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->PntStat[E_GrP4DoSpinTxtStatFocus],NULL,	L"Paint status focus"	},
	{	L"Paint Control",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->PntStat[E_GrP4DoSpinTxtStatControl],NULL,	L"Paint status control value"	},
	{	L"IsOnKeyCtrl",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsVertical",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->IsVertical,NULL,	L"vertical type"	},
	{	L"IsValRotate",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->IsValRotate,NULL,	L"value rotate"	},
	{	L"AsmEvtValChg",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->AsmEvtValChg,NULL,	L"assembly code id for the value change event"	},
	{	L"TxtCnt",						TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtCnt,NULL,	L"text count"	},
	{	L"TxtId0",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[0],NULL,	L"text id 0"	},
	{	L"TxtId1",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[1],NULL,	L"text id 1"	},
	{	L"TxtId2",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[2],NULL,	L"text id 2"	},
	{	L"TxtId3",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[3],NULL,	L"text id 3"	},
	{	L"TxtId4",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[4],NULL,	L"text id 4"	},
	{	L"TxtId5",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[5],NULL,	L"text id 5"	},
	{	L"TxtId6",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[6],NULL,	L"text id 6"	},
	{	L"TxtId7",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[7],NULL,	L"text id 7"	},
	{	L"TxtId8",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[8],NULL,	L"text id 8"	},
	{	L"TxtId9",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[9],NULL,	L"text id 9"	},
	{	L"TxtId10",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[10],NULL,	L"text id 10"	},
	{	L"TxtId11",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[11],NULL,	L"text id 11"	},
	{	L"TxtId12",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[12],NULL,	L"text id 12"	},
	{	L"TxtId13",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[13],NULL,	L"text id 13"	},
	{	L"TxtId14",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[14],NULL,	L"text id 14"	},
	{	L"TxtId15",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[15],NULL,	L"text id 15"	},
	{	L"TxtId16",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[16],NULL,	L"text id 16"	},
	{	L"TxtId17",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[17],NULL,	L"text id 17"	},
	{	L"TxtId18",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[18],NULL,	L"text id 18"	},
	{	L"TxtId19",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[19],NULL,	L"text id 19"	},
	{	L"TxtId20",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[20],NULL,	L"text id 20"	},
	{	L"TxtId21",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[21],NULL,	L"text id 21"	},
	{	L"TxtId22",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[22],NULL,	L"text id 22"	},
	{	L"TxtId23",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[23],NULL,	L"text id 23"	},
	{	L"TxtId24",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[24],NULL,	L"text id 24"	},
	{	L"TxtId25",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[25],NULL,	L"text id 25"	},
	{	L"TxtId26",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[26],NULL,	L"text id 26"	},
	{	L"TxtId27",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[27],NULL,	L"text id 27"	},
	{	L"TxtId28",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[28],NULL,	L"text id 28"	},
	{	L"TxtId29",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[29],NULL,	L"text id 29"	},
	{	L"TxtId30",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[30],NULL,	L"text id 30"	},
	{	L"TxtId31",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinTxtBasePara)0)->TxtTbl[31],NULL,	L"text id 31"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSpinTxtBase::Cls_GrP4DoSpinTxtBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSpinTxtBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoSpinTxtBase) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaDoSpinTxtBase, sizeof(m_ParaDoSpinTxtBase) );

		m_IsCtrlMode	=	FALSE;
		m_Value				=	0;

}
//--------------------------------------------------------------------
Cls_GrP4DoSpinTxtBase::~Cls_GrP4DoSpinTxtBase( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinTxtBase::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSpinTxtBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoSpinTxtBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoSpinTxtBase, sizeof(m_ParaDoSpinTxtBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoSpinTxtBase)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoSpinTxtBase, sizeof(m_ParaDoSpinTxtBase) , m_PtrScpt , E_GrP4DoClassSpinTxtBase );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinTxtBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Index;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiValueGet:
				LcValLoad();
				A_PtrInt[0]	=	m_Value;
				break;
			case E_GrP4UpiValueSet:
				m_Value	=	A_PtrInt[1];
				LcValSave();
				ReqDraw();
				break;
			case E_GrP4UpiItemTxtIdSet:
				Tv_Index	=	(__u32)A_PtrInt[1];
				if ( E_GrP4DoSpinTxtItemMax > Tv_Index )
				{
					m_ParaDoSpinTxtBase.TxtTbl[Tv_Index]	=	(__u16)A_PtrInt[2];
					ReqDraw();
				}
				break;
			case E_GrP4UpiItemCntSet:
				if ( E_GrP4DoSpinTxtItemMax > (__u32)A_PtrInt[1] )
				{
					m_ParaDoSpinTxtBase.TxtCnt	=	(__u8)A_PtrInt[1];
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DoSpinTxtBase::RtlCommand - [ItemCntSet] bad input count value\n", A_PtrInt[1] );
				}
				
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtBase::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		// check value change
		LcValLoad();
		// value patch
		if ( (m_Value >= (__u32)m_ParaDoSpinTxtBase.TxtCnt) && (0 != m_ParaDoSpinTxtBase.TxtCnt) )
		{
			m_Value	=	m_ParaDoSpinTxtBase.TxtCnt - 1;
		}
		// update status
		Tv_Stat	=	E_GrP4DoSpinTxtStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoSpinTxtStatNormal;
			if ( m_IsCtrlMode ||  m_IsOnMouse )
			{
				Tv_Stat	=	E_GrP4DoSpinTxtStatControl;
			}
			else
			{
				if ( IsFocused() )
				{
					Tv_Stat	=	E_GrP4DoSpinTxtStatFocus;
					if ( m_ParaDoSpinTxtBase.IsOnKeyCtrl )
					{
						Tv_Stat	=	E_GrP4DoSpinTxtStatControl;
					}
				}
			}
		}
		PaintScriptPush( m_ParaDoSpinTxtBase.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinTxtBase::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoSpinTxtBase, V_GrP4DoSpinTxtBaseParaInfo, E_GrP4DoSpinTxtBaseParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoSpinTxtBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaDoSpinTxtBase.TxtTbl[m_Value] );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtBase::LcNotifyValChg( void )
{
	// local -------------------
		//St_GrP4MsgObj	Tv_Msg;
		St_GrP4MngVmRpara	Tv_Rpara;
	// code --------------------
		// relate event

		// user event
		if ( V_GrP4Mng->IsUiEventAble() )
		{
			// run event assembly
			Tv_Rpara.In.Cnt		=	0;

			Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	1;
			Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	(__s32)m_Value;
			Tv_Rpara.In.Cnt ++;

			Tv_Rpara.Out.Cnt	=	0;

			V_GrP4Mng->VmRunCode( m_ParaDoSpinTxtBase.AsmEvtValChg, this, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtBase::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtBase::LcValIncrease( void )
{
	// local -------------------
	// code --------------------
		LcValLoad();
		m_Value ++;
		if ( (E_GrP4DoSpinTxtItemMax <= m_Value) || ((__u32)m_ParaDoSpinTxtBase.TxtCnt <= m_Value) )
		{
			if ( m_ParaDoSpinTxtBase.IsValRotate )
			{
				m_Value	=	0;
			}
			else
			{
				m_Value	=	(__u32)m_ParaDoSpinTxtBase.TxtCnt;
				if ( 0 != m_Value )
				{
					m_Value --;
					if ( E_GrP4DoSpinTxtItemMax <= m_Value )
					{
						m_Value	=	E_GrP4DoSpinTxtItemMax - 1;
					}
				}
			}
		}
		LcValSave();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtBase::LcValDecrease( void )
{
	// local -------------------
	// code --------------------
		LcValLoad();
		if ( 0 == m_Value  )
		{
			if ( m_ParaDoSpinTxtBase.IsValRotate )
			{
				m_Value	=	(__u32)m_ParaDoSpinTxtBase.TxtCnt;
				if ( 0 != m_Value )
				{
					m_Value --;
					if ( E_GrP4DoSpinTxtItemMax <= m_Value ) 
					{
						m_Value	=	E_GrP4DoSpinTxtItemMax - 1;
					}
				}
			}
		}
		else
		{
			m_Value --;
		}
		LcValSave();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinTxtBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaDoSpinTxtBase.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP4KeyLeft:
					if ( !m_ParaDoSpinTxtBase.IsVertical )
					{
						LcValDecrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyRight:
					if ( !m_ParaDoSpinTxtBase.IsVertical )
					{
						LcValIncrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyUp:
					if ( m_ParaDoSpinTxtBase.IsVertical )
					{
						LcValIncrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyDown:
					if ( m_ParaDoSpinTxtBase.IsVertical )
					{
						LcValDecrease();
						Tv_Result	=	TRUE;
					}
					break;

			}
		}

		// change event
		if ( Tv_Result )
		{
			ReqDraw();
			LcNotifyValChg();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinTxtBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaDoSpinTxtBase.IsOnKeyCtrl )
		{
			if ( E_GrP4KeyEnter == A_Key )
			{
				m_IsCtrlMode	=	!m_IsCtrlMode;
				ReqDraw();
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinTxtBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__s32		Tv_Ct;
	// code --------------------
		// check position
		if ( ScrRectGet( &Tv_Rc ) )
		{
			if ( m_ParaDoSpinTxtBase.IsVertical )
			{
				// vertical area calculate
				Tv_Ct	=	((Tv_Rc.bottom - Tv_Rc.top) >> 1) + Tv_Rc.top;
				if ( Tv_Ct > (__s32)A_Y )
				{
					LcValIncrease();
				}
				else
				{
					LcValDecrease();
				}
			}
			else
			{
				// horizontal area calculate
				Tv_Ct	=	((Tv_Rc.right - Tv_Rc.left) >> 1) + Tv_Rc.left;
				if ( Tv_Ct > (__s32)A_X )
				{
					LcValDecrease();
				}
				else
				{
					LcValIncrease();
				}
			}
			ReqDraw();
			LcNotifyValChg();
		}
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinTxtBase::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcValIncrease();
		ReqDraw();
		LcNotifyValChg();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinTxtBase::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcValDecrease();
		ReqDraw();
		LcNotifyValChg();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtBase::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlInit();
		LcValLoad();
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtBase::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtBase::LcValSave( void )
{

}
//--------------------------------------------------------------------


