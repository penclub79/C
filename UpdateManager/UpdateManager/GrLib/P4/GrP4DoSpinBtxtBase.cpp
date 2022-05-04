/*
		platform spin buffer text

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSpinBtxtBase.h>
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

#define E_GrP4DoSpinBtxtBaseParaCnt		8

St_GrP4DvlDoPrInfoData	V_GrP4DoSpinBtxtParaInfo[E_GrP4DoSpinBtxtBaseParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinBtxtBasePara)0)->PntStat[E_GrP4DoSpinBtxtStatDisable],NULL,	L"Paint status disable"	},
	{	L"Paint Normal",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinBtxtBasePara)0)->PntStat[E_GrP4DoSpinBtxtStatNormal],NULL,	L"Paint status normal"	},
	{	L"Paint Focus",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinBtxtBasePara)0)->PntStat[E_GrP4DoSpinBtxtStatFocus],NULL,	L"Paint status focus"	},
	{	L"Paint Control",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinBtxtBasePara)0)->PntStat[E_GrP4DoSpinBtxtStatControl],NULL,	L"Paint status control value"	},
	{	L"IsOnKeyCtrl",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinBtxtBasePara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsVertical",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinBtxtBasePara)0)->IsVertical,NULL,	L"vertical type"	},
	{	L"IsValRotate",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinBtxtBasePara)0)->IsValRotate,NULL,	L"value rotate"	},
	{	L"AsmEvtValChg",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoSpinBtxtBasePara)0)->AsmEvtValChg,NULL,	L"assembly code id for the value change event"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSpinBtxtBase::Cls_GrP4DoSpinBtxtBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect, A_PosRel, A_ObjPrnt )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSpinBtxtBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinBtxtBase) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaSpinBtxtBase, sizeof(m_ParaSpinBtxtBase) );

		m_IsCtrlMode	=	FALSE;
		m_Value				=	0;
		m_TxtCnt			=	0;
		
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DoSpinBtxtItemMax;Tv_WkIdx++ )
		{
			GrStrClear( m_StrTbl[Tv_WkIdx] );
		}

}
//--------------------------------------------------------------------
Cls_GrP4DoSpinBtxtBase::~Cls_GrP4DoSpinBtxtBase( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinBtxtBase::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSpinBtxtBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinBtxtBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinBtxtBase, sizeof(m_ParaSpinBtxtBase) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinBtxtBase)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinBtxtBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinBtxtBase, sizeof(m_ParaSpinBtxtBase) , m_PtrScpt , E_GrP4DoClassSpinBtxtBase );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinBtxtBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiValueGet:
				LcValLoad();
				A_PtrInt[0]	=	(__s32)m_Value;
				break;
			case E_GrP4UpiValueSet:
				m_Value	=	(__u32)A_PtrInt[1];
				LcValSave();
				ReqDraw();
				break;
			case E_GrP4UpiItemCntGet:
				A_PtrInt[0]	=	(__s32)m_TxtCnt;
				break;
			case E_GrP4UpiTextAdd:
				if ( E_GrP4DoSpinBtxtItemMax > m_TxtCnt )
				{
					GrStrWcopy( m_StrTbl[m_TxtCnt], A_PtrStr[0], 63 );
					m_TxtCnt ++;
					ReqDraw();
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DoSpinBtxtBase::RtlCommand - TextAdd - text buffer full!\n" );
				}
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinBtxtBase::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		// check value change
		LcValLoad();
		// update status
		Tv_Stat	=	E_GrP4DoSpinBtxtStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoSpinBtxtStatNormal;
			if ( m_IsCtrlMode ||  m_IsOnMouse )
			{
				Tv_Stat	=	E_GrP4DoSpinBtxtStatControl;
			}
			else
			{
				if ( IsFocused() )
				{
					Tv_Stat	=	E_GrP4DoSpinBtxtStatFocus;
					if ( m_ParaSpinBtxtBase.IsOnKeyCtrl )
					{
						Tv_Stat	=	E_GrP4DoSpinBtxtStatControl;
					}
				}
			}
		}
		PaintScriptPush( m_ParaSpinBtxtBase.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinBtxtBase::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinBtxtBase, V_GrP4DoSpinBtxtParaInfo, E_GrP4DoSpinBtxtBaseParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoSpinBtxtBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			if ( m_TxtCnt > m_Value )
			{
				Tv_Result	=	m_StrTbl[m_Value];
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinBtxtBase::LcNotifyValChg( void )
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

			V_GrP4Mng->VmRunCode( m_ParaSpinBtxtBase.AsmEvtValChg, this, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinBtxtBase::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinBtxtBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinBtxtBase::LcValIncrease( void )
{
	// local -------------------
	// code --------------------
		LcValLoad();

		if ( 0 == m_TxtCnt )
		{
			m_Value	=	0;
		}
		else
		{
			m_Value ++;
			if ( m_TxtCnt <= m_Value )
			{
				if ( m_ParaSpinBtxtBase.IsValRotate )
				{
					m_Value	=	0;
				}
				else
				{
					m_Value	=	m_TxtCnt - 1;
				}
			}
		}
		LcValSave();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinBtxtBase::LcValDecrease( void )
{
	// local -------------------
	// code --------------------
		LcValLoad();

		if ( 0 == m_TxtCnt )
		{
			m_Value	=	0;
		}
		else
		{
			if ( 0 == m_Value  )
			{
				if ( m_ParaSpinBtxtBase.IsValRotate )
				{
					m_Value	=	m_TxtCnt - 1;
				}
			}
			else
			{
				m_Value --;
			}
			// patch
			if ( m_Value >= m_TxtCnt )
			{
				m_Value	=	m_TxtCnt - 1;
			}
		}

		LcValSave();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinBtxtBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaSpinBtxtBase.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP4KeyLeft:
					if ( !m_ParaSpinBtxtBase.IsVertical )
					{
						LcValDecrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyRight:
					if ( !m_ParaSpinBtxtBase.IsVertical )
					{
						LcValIncrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyUp:
					if ( m_ParaSpinBtxtBase.IsVertical )
					{
						LcValIncrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyDown:
					if ( m_ParaSpinBtxtBase.IsVertical )
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
BOOL8	Cls_GrP4DoSpinBtxtBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaSpinBtxtBase.IsOnKeyCtrl )
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
BOOL8	Cls_GrP4DoSpinBtxtBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__s32		Tv_Ct;
	// code --------------------
		// check position
		if ( ScrRectGet( &Tv_Rc ) )
		{
			if ( m_ParaSpinBtxtBase.IsVertical )
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
BOOL8	Cls_GrP4DoSpinBtxtBase::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcValIncrease();
		ReqDraw();
		LcNotifyValChg();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinBtxtBase::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcValDecrease();
		ReqDraw();
		LcNotifyValChg();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinBtxtBase::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlInit();
		LcValLoad();
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinBtxtBase::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinBtxtBase::LcValSave( void )
{

}
//--------------------------------------------------------------------


