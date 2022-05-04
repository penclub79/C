/*
		platform spin number base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSpinNumBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>

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

#define E_GrP3DoSpinNumBaseParaCnt		16

St_GrP3DvlDoPrInfoData	V_GrP3DoSpinNumBaseParaInfo[E_GrP3DoSpinNumBaseParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->PntStat[E_GrP3DoSpinNumStatDisable],NULL,	L"Paint status disable"	},
	{	L"Paint Normal",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->PntStat[E_GrP3DoSpinNumStatNormal],NULL,	L"Paint status normal"	},
	{	L"Paint Focus",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->PntStat[E_GrP3DoSpinNumStatFocus],NULL,	L"Paint status focus"	},
	{	L"Paint Control",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->PntStat[E_GrP3DoSpinNumStatControl],NULL,	L"Paint status control value"	},
	{	L"IsOnKeyCtrl",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsVertical",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->IsVertical,NULL,	L"vertical type"	},
	{	L"IsValRotate",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->IsValRotate,NULL,	L"value rotate"	},
	{	L"Cipher",						TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->Cipher,NULL,	L"display cipher"	},
	{	L"MinVal",						TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->MinVal,NULL,	L"minimum value limite"	},
	{	L"MaxVal",						TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->MaxVal,NULL,	L"maximum value limite"	},
	{	L"Step",							TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->Step,NULL,	L"control value step"	},
	{	L"ZeroTxtId",					TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->ZeroTxtId,NULL,	L"show text when value is zero"	},
	{	L"PrefixTxtId",				TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->PrefixTxtId,NULL,	L"prefix text id"	},
	{	L"SuffixTxtId",				TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->SuffixTxtId,NULL,	L"suffix text id"	},
	{	L"AsmEvtValChg",			TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->AsmEvtValChg,NULL,	L"assembly code id for the value change event"	},
	{	L"DispAddVal",				TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoSpinNumBasePara)0)->DispAddVal,NULL,	L"display add value"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSpinNumBase::Cls_GrP3DoSpinNumBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassSpinNumBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoSpinNumBase) + sizeof(St_GrP3ScptParaHd);
		
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
Cls_GrP3DoSpinNumBase::~Cls_GrP3DoSpinNumBase( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumBase::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSpinNumBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoSpinNumBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoSpinNumBase, sizeof(m_ParaDoSpinNumBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoSpinNumBase)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoSpinNumBase, sizeof(m_ParaDoSpinNumBase) , m_PtrScpt , E_GrP3DoClassSpinNumBase );

		if ( m_Value < m_ParaDoSpinNumBase.MinVal )
		{
			m_Value	=	m_ParaDoSpinNumBase.MinVal;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinNumBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiValueGet:
				LcValLoad();
				A_PtrInt[0]	=	m_Value;
				break;
			case E_GrP3UpiValueSet:
				m_Value	=	A_PtrInt[1];
				LcValSave();
				LcValTxtUpdt();
				ReqDraw();
				break;
			case E_GrP3UpiMinValueGet:
				A_PtrInt[0]	=	m_ParaDoSpinNumBase.MinVal;
				break;
			case E_GrP3UpiMinValueSet:
				m_ParaDoSpinNumBase.MinVal	=	A_PtrInt[1];
				if ( m_Value < m_ParaDoSpinNumBase.MinVal )
				{
					m_Value	=	m_ParaDoSpinNumBase.MinVal;
					LcValSave();
					LcValTxtUpdt();
					ReqDraw();
				}
				break;
			case E_GrP3UpiMaxValueGet:
				A_PtrInt[0]	=	m_ParaDoSpinNumBase.MaxVal;
				break;
			case E_GrP3UpiMaxValueSet:
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
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumBase::ProcPaint( void )
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
		Tv_Stat	=	E_GrP3DoSpinNumStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP3DoSpinNumStatNormal;
			if ( m_IsCtrlMode ||  m_IsOnMouse )
			{
				Tv_Stat	=	E_GrP3DoSpinNumStatControl;
			}
			else
			{
				if ( IsFocused() )
				{
					Tv_Stat	=	E_GrP3DoSpinNumStatFocus;
					if ( m_ParaDoSpinNumBase.IsOnKeyCtrl )
					{
						Tv_Stat	=	E_GrP3DoSpinNumStatControl;
					}
				}
			}
		}
		PaintScriptPush( m_ParaDoSpinNumBase.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumBase::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoSpinNumBase, V_GrP3DoSpinNumBaseParaInfo, E_GrP3DoSpinNumBaseParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumBase::LcValTxtUpdt( void )
{
	// local -------------------
		BOOL8	Tv_IsFix;
		WCHAR*	Tv_StrWk;
		WCHAR*	Tv_StrAdd;
	// code --------------------
		Tv_StrWk	=	m_StrTxt;

		// check value
		if ( (0 == m_Value) && (E_GrP3ScptTxtIdNone != m_ParaDoSpinNumBase.ZeroTxtId) )
		{
			Tv_StrAdd	=	V_GrP3Mng->TextPtrGet( m_ParaDoSpinNumBase.ZeroTxtId );
			Tv_StrWk	=	GrStrWcopy( Tv_StrWk, Tv_StrAdd );
		}
		else
		{
			// add prefix
			if ( E_GrP3ScptTxtIdNone != m_ParaDoSpinNumBase.PrefixTxtId )
			{
				Tv_StrAdd	=	V_GrP3Mng->TextPtrGet( m_ParaDoSpinNumBase.PrefixTxtId );
				Tv_StrWk	=	GrStrWcopy( Tv_StrWk, Tv_StrAdd, 12 );
			}

			Tv_IsFix	=	FALSE;
			if ( 0 != m_ParaDoSpinNumBase.Cipher )
			{
				Tv_IsFix	=	TRUE;
			}
			GrStrIntToWstr( Tv_StrWk, m_Value + m_ParaDoSpinNumBase.DispAddVal, (__u32)m_ParaDoSpinNumBase.Cipher, Tv_IsFix );

			// add suffix
			if ( E_GrP3ScptTxtIdNone != m_ParaDoSpinNumBase.SuffixTxtId )
			{
				Tv_StrAdd	=	V_GrP3Mng->TextPtrGet( m_ParaDoSpinNumBase.SuffixTxtId );
				Tv_StrWk	=	GrStrWcat( Tv_StrWk, Tv_StrAdd, 12 );
			}
		}

}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoSpinNumBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
void	Cls_GrP3DoSpinNumBase::LcNotifyValChg( void )
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
			Tv_Rpara.Cnt	=	2;
			Tv_Rpara.Ridx[0]	=	1;
			Tv_Rpara.Rval[0]	=	m_Value;
			Tv_Rpara.Ridx[1]	=	2;
			Tv_Rpara.Rval[1]	=	m_Orignal;

			V_GrP3Mng->VmRunCode( m_ParaDoSpinNumBase.AsmEvtValChg, this, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumBase::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumBase::LcValIncrease( void )
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
void	Cls_GrP3DoSpinNumBase::LcValDecrease( void )
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
BOOL8	Cls_GrP3DoSpinNumBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaDoSpinNumBase.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP3KeyLeft:
					if ( !m_ParaDoSpinNumBase.IsVertical )
					{
						LcValDecrease();
						LcNotifyValChg();
						ReqDraw();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyRight:
					if ( !m_ParaDoSpinNumBase.IsVertical )
					{
						LcValIncrease();
						LcNotifyValChg();
						ReqDraw();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyUp:
					if ( m_ParaDoSpinNumBase.IsVertical )
					{
						LcValIncrease();
						LcNotifyValChg();
						ReqDraw();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyDown:
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
BOOL8	Cls_GrP3DoSpinNumBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaDoSpinNumBase.IsOnKeyCtrl )
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
BOOL8	Cls_GrP3DoSpinNumBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
BOOL8	Cls_GrP3DoSpinNumBase::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcValIncrease();
		LcNotifyValChg();
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinNumBase::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcValDecrease();
		LcNotifyValChg();
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumBase::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlInit();
		LcValLoad();
		LcValTxtUpdt();
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumBase::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinNumBase::LcValSave( void )
{

}
//--------------------------------------------------------------------

