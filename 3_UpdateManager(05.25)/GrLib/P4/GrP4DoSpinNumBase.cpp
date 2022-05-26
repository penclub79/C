/*
		platform spin number base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSpinNumBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>

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

#define E_GrP4DoSpinNumBaseParaCnt		16

St_GrP4DvlDoPrInfoData	V_GrP4DoSpinNumBaseParaInfo[E_GrP4DoSpinNumBaseParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->PntStat[E_GrP4DoSpinNumStatDisable],NULL,	L"Paint status disable"	},
	{	L"Paint Normal",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->PntStat[E_GrP4DoSpinNumStatNormal],NULL,	L"Paint status normal"	},
	{	L"Paint Focus",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->PntStat[E_GrP4DoSpinNumStatFocus],NULL,	L"Paint status focus"	},
	{	L"Paint Control",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->PntStat[E_GrP4DoSpinNumStatControl],NULL,	L"Paint status control value"	},
	{	L"IsOnKeyCtrl",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsVertical",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->IsVertical,NULL,	L"vertical type"	},
	{	L"IsValRotate",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->IsValRotate,NULL,	L"value rotate"	},
	{	L"Cipher",						TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->Cipher,NULL,	L"display cipher"	},
	{	L"MinVal",						TRUE,		E_GrP4DvlParaTypeInt,						1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->MinVal,NULL,	L"minimum value limite"	},
	{	L"MaxVal",						TRUE,		E_GrP4DvlParaTypeInt,						1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->MaxVal,NULL,	L"maximum value limite"	},
	{	L"Step",							TRUE,		E_GrP4DvlParaTypeInt,						1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->Step,NULL,	L"control value step"	},
	{	L"ZeroTxtId",					TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->ZeroTxtId,NULL,	L"show text when value is zero"	},
	{	L"PrefixTxtId",				TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->PrefixTxtId,NULL,	L"prefix text id"	},
	{	L"SuffixTxtId",				TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->SuffixTxtId,NULL,	L"suffix text id"	},
	{	L"AsmEvtValChg",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->AsmEvtValChg,NULL,	L"assembly code id for the value change event"	},
	{	L"DispAddVal",				TRUE,		E_GrP4DvlParaTypeInt,						1,	(void*)&((Ptr_GrP4DoSpinNumBasePara)0)->DispAddVal,NULL,	L"display add value"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSpinNumBase::Cls_GrP4DoSpinNumBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSpinNumBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoSpinNumBase) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaDoSpinNumBase, sizeof(m_ParaDoSpinNumBase) );
		m_ParaDoSpinNumBase.MaxVal	=	100;
		m_ParaDoSpinNumBase.Step		=	1;

		m_IsCtrlMode	=	FALSE;
		m_Value				=	0;

		LcValTxtUpdt();
}
//--------------------------------------------------------------------
Cls_GrP4DoSpinNumBase::~Cls_GrP4DoSpinNumBase( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinNumBase::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSpinNumBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoSpinNumBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoSpinNumBase, sizeof(m_ParaDoSpinNumBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoSpinNumBase)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoSpinNumBase, sizeof(m_ParaDoSpinNumBase) , m_PtrScpt , E_GrP4DoClassSpinNumBase );

		if ( m_Value < m_ParaDoSpinNumBase.MinVal )
		{
			m_Value	=	m_ParaDoSpinNumBase.MinVal;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinNumBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
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
				LcValTxtUpdt();
				ReqDraw();
				break;
			case E_GrP4UpiMinValueGet:
				A_PtrInt[0]	=	m_ParaDoSpinNumBase.MinVal;
				break;
			case E_GrP4UpiMinValueSet:
				m_ParaDoSpinNumBase.MinVal	=	A_PtrInt[1];
				if ( m_Value < m_ParaDoSpinNumBase.MinVal )
				{
					m_Value	=	m_ParaDoSpinNumBase.MinVal;
					LcValSave();
					LcValTxtUpdt();
					ReqDraw();
				}
				break;
			case E_GrP4UpiMaxValueGet:
				A_PtrInt[0]	=	m_ParaDoSpinNumBase.MaxVal;
				break;
			case E_GrP4UpiMaxValueSet:
				m_ParaDoSpinNumBase.MaxVal	=	A_PtrInt[1];
				if ( m_Value > m_ParaDoSpinNumBase.MaxVal )
				{
					m_Value	=	m_ParaDoSpinNumBase.MaxVal;
					LcValSave();
					LcValTxtUpdt();
					ReqDraw();
				}
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumBase::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
		__s32		Tv_Value;
	// code --------------------
		// check value change
		Tv_Value	=	m_Value;
		LcValLoad();
		if ( Tv_Value != m_Value )
		{
			LcValTxtUpdt();
		}
		// update status
		Tv_Stat	=	E_GrP4DoSpinNumStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoSpinNumStatNormal;
			if ( m_IsCtrlMode ||  m_IsOnMouse )
			{
				Tv_Stat	=	E_GrP4DoSpinNumStatControl;
			}
			else
			{
				if ( IsFocused() )
				{
					Tv_Stat	=	E_GrP4DoSpinNumStatFocus;
					if ( m_ParaDoSpinNumBase.IsOnKeyCtrl )
					{
						Tv_Stat	=	E_GrP4DoSpinNumStatControl;
					}
				}
			}
		}
		PaintScriptPush( m_ParaDoSpinNumBase.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinNumBase::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoSpinNumBase, V_GrP4DoSpinNumBaseParaInfo, E_GrP4DoSpinNumBaseParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumBase::LcValTxtUpdt( void )
{
	// local -------------------
		BOOL8	Tv_IsFix;
		WCHAR*	Tv_StrWk;
		WCHAR*	Tv_StrAdd;
	// code --------------------
		Tv_StrWk	=	m_StrTxt;

		// check value
		if ( (0 == m_Value) && (E_GrP4ScptTxtIdNone != m_ParaDoSpinNumBase.ZeroTxtId) )
		{
			Tv_StrAdd	=	V_GrP4Mng->TextPtrGet( m_ParaDoSpinNumBase.ZeroTxtId );
			Tv_StrWk	=	GrStrWcopy( Tv_StrWk, Tv_StrAdd );
		}
		else
		{
			// add prefix
			if ( E_GrP4ScptTxtIdNone != m_ParaDoSpinNumBase.PrefixTxtId )
			{
				Tv_StrAdd	=	V_GrP4Mng->TextPtrGet( m_ParaDoSpinNumBase.PrefixTxtId );
				Tv_StrWk	=	GrStrWcopy( Tv_StrWk, Tv_StrAdd, 12 );
			}

			Tv_IsFix	=	FALSE;
			if ( 0 != m_ParaDoSpinNumBase.Cipher )
			{
				Tv_IsFix	=	TRUE;
			}
			GrStrIntToWstr( Tv_StrWk, m_Value + m_ParaDoSpinNumBase.DispAddVal, (__u32)m_ParaDoSpinNumBase.Cipher, Tv_IsFix );

			// add suffix
			if ( E_GrP4ScptTxtIdNone != m_ParaDoSpinNumBase.SuffixTxtId )
			{
				Tv_StrAdd	=	V_GrP4Mng->TextPtrGet( m_ParaDoSpinNumBase.SuffixTxtId );
				Tv_StrWk	=	GrStrWcat( Tv_StrWk, Tv_StrAdd, 12 );
			}
		}

}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoSpinNumBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	m_StrTxt;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumBase::LcNotifyValChg( void )
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
			Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	m_Value;
			Tv_Rpara.In.Cnt ++;

			Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	2;
			Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	m_Orignal;
			Tv_Rpara.In.Cnt ++;

			Tv_Rpara.Out.Cnt	=	0;

			V_GrP4Mng->VmRunCode( m_ParaDoSpinNumBase.AsmEvtValChg, this, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumBase::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumBase::LcValIncrease( void )
{
	// local -------------------
	// code --------------------
		LcValLoad();
		m_Orignal	=	m_Value;
		m_Value		=	m_Value + m_ParaDoSpinNumBase.Step;
		if ( m_ParaDoSpinNumBase.MaxVal < m_Value )
		{
			if ( m_ParaDoSpinNumBase.IsValRotate )
			{
				m_Value	=	m_ParaDoSpinNumBase.MinVal;
			}
			else
			{
				m_Value	=	m_ParaDoSpinNumBase.MaxVal;
			}
		}
		LcValSave();
		LcValTxtUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumBase::LcValDecrease( void )
{
	// local -------------------
	// code --------------------
		LcValLoad();
		m_Orignal	=	m_Value;
		m_Value		=	m_Value - m_ParaDoSpinNumBase.Step;
		if ( m_ParaDoSpinNumBase.MinVal > m_Value )
		{
			if ( m_ParaDoSpinNumBase.IsValRotate )
			{
				m_Value	=	m_ParaDoSpinNumBase.MaxVal;
			}
			else
			{
				m_Value	=	m_ParaDoSpinNumBase.MinVal;
			}
		}
		LcValSave();
		LcValTxtUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinNumBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaDoSpinNumBase.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP4KeyLeft:
					if ( !m_ParaDoSpinNumBase.IsVertical )
					{
						LcValDecrease();
						LcNotifyValChg();
						ReqDraw();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyRight:
					if ( !m_ParaDoSpinNumBase.IsVertical )
					{
						LcValIncrease();
						LcNotifyValChg();
						ReqDraw();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyUp:
					if ( m_ParaDoSpinNumBase.IsVertical )
					{
						LcValIncrease();
						LcNotifyValChg();
						ReqDraw();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyDown:
					if ( m_ParaDoSpinNumBase.IsVertical )
					{
						LcValDecrease();
						LcNotifyValChg();
						ReqDraw();
						Tv_Result	=	TRUE;
					}
					break;

			}
		}

		// change event
		/*
		if ( Tv_Result )
		{
			ReqDraw();
			LcNotifyValChg();
		}
		*/

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinNumBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaDoSpinNumBase.IsOnKeyCtrl )
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
BOOL8	Cls_GrP4DoSpinNumBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__s32		Tv_Ct;
	// code --------------------
		// check position
		if ( ScrRectGet( &Tv_Rc ) )
		{
			if ( m_ParaDoSpinNumBase.IsVertical )
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
BOOL8	Cls_GrP4DoSpinNumBase::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcValIncrease();
		LcNotifyValChg();
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinNumBase::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcValDecrease();
		LcNotifyValChg();
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumBase::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlInit();
		LcValLoad();
		LcValTxtUpdt();
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumBase::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinNumBase::LcValSave( void )
{

}
//--------------------------------------------------------------------

