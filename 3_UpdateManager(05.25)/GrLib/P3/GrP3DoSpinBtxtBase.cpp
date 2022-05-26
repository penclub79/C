/*
		platform spin buffer text

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSpinBtxtBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>
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

#define E_GrP3DoSpinBtxtBaseParaCnt		8

St_GrP3DvlDoPrInfoData	V_GrP3DoSpinBtxtParaInfo[E_GrP3DoSpinBtxtBaseParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinBtxtBasePara)0)->PntStat[E_GrP3DoSpinBtxtStatDisable],NULL,	L"Paint status disable"	},
	{	L"Paint Normal",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinBtxtBasePara)0)->PntStat[E_GrP3DoSpinBtxtStatNormal],NULL,	L"Paint status normal"	},
	{	L"Paint Focus",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinBtxtBasePara)0)->PntStat[E_GrP3DoSpinBtxtStatFocus],NULL,	L"Paint status focus"	},
	{	L"Paint Control",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinBtxtBasePara)0)->PntStat[E_GrP3DoSpinBtxtStatControl],NULL,	L"Paint status control value"	},
	{	L"IsOnKeyCtrl",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinBtxtBasePara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsVertical",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinBtxtBasePara)0)->IsVertical,NULL,	L"vertical type"	},
	{	L"IsValRotate",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinBtxtBasePara)0)->IsValRotate,NULL,	L"value rotate"	},
	{	L"AsmEvtValChg",			TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoSpinBtxtBasePara)0)->AsmEvtValChg,NULL,	L"assembly code id for the value change event"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSpinBtxtBase::Cls_GrP3DoSpinBtxtBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassSpinBtxtBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinBtxtBase) + sizeof(St_GrP3ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaSpinBtxtBase, sizeof(m_ParaSpinBtxtBase) );

		m_IsCtrlMode	=	FALSE;
		m_Value				=	0;
		m_TxtCnt			=	0;
		
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DoSpinBtxtItemMax;Tv_WkIdx++ )
		{
			GrStrClear( m_StrTbl[Tv_WkIdx] );
		}

}
//--------------------------------------------------------------------
Cls_GrP3DoSpinBtxtBase::~Cls_GrP3DoSpinBtxtBase( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtBase::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSpinBtxtBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinBtxtBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinBtxtBase, sizeof(m_ParaSpinBtxtBase) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinBtxtBase)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinBtxtBase, sizeof(m_ParaSpinBtxtBase) , m_PtrScpt , E_GrP3DoClassSpinBtxtBase );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinBtxtBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiValueGet:
				LcValLoad();
				A_PtrInt[0]	=	(__s32)m_Value;
				break;
			case E_GrP3UpiValueSet:
				m_Value	=	(__u32)A_PtrInt[1];
				LcValSave();
				ReqDraw();
				break;
			case E_GrP3UpiItemCntGet:
				A_PtrInt[0]	=	(__s32)m_TxtCnt;
				break;
			case E_GrP3UpiTextAdd:
				if ( E_GrP3DoSpinBtxtItemMax > m_TxtCnt )
				{
					GrStrWcopy( m_StrTbl[m_TxtCnt], A_PtrStr[0], 63 );
					m_TxtCnt ++;
					ReqDraw();
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3DoSpinBtxtBase::RtlCommand - TextAdd - text buffer full!\n" );
				}
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtBase::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		// check value change
		LcValLoad();
		// update status
		Tv_Stat	=	E_GrP3DoSpinBtxtStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP3DoSpinBtxtStatNormal;
			if ( m_IsCtrlMode ||  m_IsOnMouse )
			{
				Tv_Stat	=	E_GrP3DoSpinBtxtStatControl;
			}
			else
			{
				if ( IsFocused() )
				{
					Tv_Stat	=	E_GrP3DoSpinBtxtStatFocus;
					if ( m_ParaSpinBtxtBase.IsOnKeyCtrl )
					{
						Tv_Stat	=	E_GrP3DoSpinBtxtStatControl;
					}
				}
			}
		}
		PaintScriptPush( m_ParaSpinBtxtBase.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtBase::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinBtxtBase, V_GrP3DoSpinBtxtParaInfo, E_GrP3DoSpinBtxtBaseParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoSpinBtxtBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
void	Cls_GrP3DoSpinBtxtBase::LcNotifyValChg( void )
{
	// local -------------------
		//St_GrP3MsgObj	Tv_Msg;
		St_GrP3MngVmRpara	Tv_Rpara;
	// code --------------------
		// relate event

		// user event
		if ( V_GrP3Mng->IsUiEventAble() )
		{
			// run event assembly
			Tv_Rpara.Cnt	=	1;
			Tv_Rpara.Ridx[0]	=	1;
			Tv_Rpara.Rval[0]	=	m_Value;
			V_GrP3Mng->VmRunCode( m_ParaSpinBtxtBase.AsmEvtValChg, this, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtBase::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtBase::LcValIncrease( void )
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
void	Cls_GrP3DoSpinBtxtBase::LcValDecrease( void )
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
BOOL8	Cls_GrP3DoSpinBtxtBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaSpinBtxtBase.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP3KeyLeft:
					if ( !m_ParaSpinBtxtBase.IsVertical )
					{
						LcValDecrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyRight:
					if ( !m_ParaSpinBtxtBase.IsVertical )
					{
						LcValIncrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyUp:
					if ( m_ParaSpinBtxtBase.IsVertical )
					{
						LcValIncrease();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyDown:
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
BOOL8	Cls_GrP3DoSpinBtxtBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaSpinBtxtBase.IsOnKeyCtrl )
		{
			if ( E_GrP3KeyEnter == A_Key )
			{
				m_IsCtrlMode	=	!m_IsCtrlMode;
				ReqDraw();
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinBtxtBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
BOOL8	Cls_GrP3DoSpinBtxtBase::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcValIncrease();
		ReqDraw();
		LcNotifyValChg();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinBtxtBase::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcValDecrease();
		ReqDraw();
		LcNotifyValChg();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtBase::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlInit();
		LcValLoad();
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtBase::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtBase::LcValSave( void )
{

}
//--------------------------------------------------------------------


