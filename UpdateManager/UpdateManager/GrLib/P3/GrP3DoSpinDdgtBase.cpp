/*
		platform spin decimal digit base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSpinDdgtBase.h>
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

#define E_GrP3DoSpinDdgtBaseParaCnt		10

St_GrP3DvlDoPrInfoData	V_GrP3DoSpinDdgtBaseParaInfo[E_GrP3DoSpinDdgtBaseParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinDdgtBasePara)0)->PntStat[E_GrP3DoSpinDdgtStatDisable],NULL,	L"Paint status disable"	},
	{	L"Paint Normal",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinDdgtBasePara)0)->PntStat[E_GrP3DoSpinDdgtStatNormal],NULL,	L"Paint status normal"	},
	{	L"Paint Focus",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinDdgtBasePara)0)->PntStat[E_GrP3DoSpinDdgtStatFocus],NULL,	L"Paint status focus"	},
	{	L"Paint Control",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinDdgtBasePara)0)->PntStat[E_GrP3DoSpinDdgtStatControl],NULL,	L"Paint status control value"	},
	{	L"IsOnKeyCtrl",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinDdgtBasePara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsValRotate",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinDdgtBasePara)0)->IsValRotate,NULL,	L"value rotate"	},
	{	L"IsCipherRotate",		TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinDdgtBasePara)0)->IsCipherRotate,NULL,	L"cipher rotate"	},
	{	L"MinVal",						TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoSpinDdgtBasePara)0)->MinVal,NULL,	L"minimum value limite"	},
	{	L"MaxVal",						TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoSpinDdgtBasePara)0)->MaxVal,NULL,	L"maximum value limite"	},
	{	L"AsmEvtValChg",			TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoSpinDdgtBasePara)0)->AsmEvtValChg,NULL,	L"assembly code id for the value change event"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSpinDdgtBase::Cls_GrP3DoSpinDdgtBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassSpinDdgtBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoSpinDdgtBase) + sizeof(St_GrP3ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaDoSpinDdgtBase, sizeof(m_ParaDoSpinDdgtBase) );
		m_ParaDoSpinDdgtBase.MaxVal	=	100;

		m_IsCtrlMode	=	FALSE;
		m_Value				=	0;
		m_CusPos			=	0;

		LcCipherUpdt();

		LcValTxtUpdt();
}
//--------------------------------------------------------------------
Cls_GrP3DoSpinDdgtBase::~Cls_GrP3DoSpinDdgtBase( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSpinDdgtBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoSpinDdgtBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoSpinDdgtBase, sizeof(m_ParaDoSpinDdgtBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoSpinDdgtBase)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoSpinDdgtBase, sizeof(m_ParaDoSpinDdgtBase) , m_PtrScpt , E_GrP3DoClassSpinDdgtBase );

		if ( m_Value < m_ParaDoSpinDdgtBase.MinVal )
		{
			m_Value	=	m_ParaDoSpinDdgtBase.MinVal;
		}
		

		LcCipherUpdt();
		LcValTxtUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinDdgtBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
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
				LcValTxtUpdt();
				ReqDraw();
				break;
			case E_GrP3UpiMinValueGet:
				A_PtrInt[0]	=	m_ParaDoSpinDdgtBase.MinVal;
				break;
			case E_GrP3UpiMinValueSet:
				m_ParaDoSpinDdgtBase.MinVal	=	A_PtrInt[1];
				if ( m_Value < m_ParaDoSpinDdgtBase.MinVal )
				{
					m_Value	=	m_ParaDoSpinDdgtBase.MinVal;
					LcValSave();
					LcValTxtUpdt();
					ReqDraw();
				}
				break;
			case E_GrP3UpiMaxValueGet:
				A_PtrInt[0]	=	m_ParaDoSpinDdgtBase.MaxVal;
				break;
			case E_GrP3UpiMaxValueSet:
				m_ParaDoSpinDdgtBase.MaxVal	=	A_PtrInt[1];
				if ( m_Value > m_ParaDoSpinDdgtBase.MaxVal )
				{
					m_Value	=	m_ParaDoSpinDdgtBase.MaxVal;
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
void	Cls_GrP3DoSpinDdgtBase::ProcPaint( void )
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
		Tv_Stat	=	E_GrP3DoSpinDdgtStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP3DoSpinDdgtStatNormal;
			if ( m_IsCtrlMode ||  m_IsOnMouse )
			{
				Tv_Stat	=	E_GrP3DoSpinDdgtStatControl;
			}
			else
			{
				if ( IsFocused() )
				{
					Tv_Stat	=	E_GrP3DoSpinDdgtStatFocus;
					if ( m_ParaDoSpinDdgtBase.IsOnKeyCtrl )
					{
						Tv_Stat	=	E_GrP3DoSpinDdgtStatControl;
					}
				}
			}
		}
		PaintScriptPush( m_ParaDoSpinDdgtBase.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoSpinDdgtBase, V_GrP3DoSpinDdgtBaseParaInfo, E_GrP3DoSpinDdgtBaseParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::LcValTxtUpdt( void )
{
	// local -------------------
	// code --------------------
		GrStrIntToWstr( m_StrTxt, (__s32)m_Value, (__u32)m_MaxCipher, TRUE );

}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoSpinDdgtBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			*A_PtrRtCus	=	(__s32)m_CusPos;
			Tv_Result	=	m_StrTxt;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::LcNotifyValChg( void )
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
			V_GrP3Mng->VmRunCode( m_ParaDoSpinDdgtBase.AsmEvtValChg, this, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinDdgtBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaDoSpinDdgtBase.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP3KeyLeft:
					Tv_Result	=	LcCusLeft();
					break;
				case E_GrP3KeyRight:
					Tv_Result	=	LcCusRight();
					break;
				case E_GrP3KeyUp:
					LcCipherInc();
					LcNotifyValChg();
					Tv_Result	=	TRUE;
					break;
				case E_GrP3KeyDown:
					LcCipherDec();
					LcNotifyValChg();
					Tv_Result	=	TRUE;
					break;

			}
		}

		// change event
		if ( Tv_Result )
		{
			ReqDraw();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinDdgtBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaDoSpinDdgtBase.IsOnKeyCtrl )
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
BOOL8	Cls_GrP3DoSpinDdgtBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		// do number virtual keyboard
		V_GrP3Mng->VkbdNumRun( this, m_StrTxt, m_MaxCipher, FALSE );
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinDdgtBase::ProcEvtMseMbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		if ( (m_CusPos + 1) < m_MaxCipher )
		{
			m_CusPos ++;
		}
		else
		{
			m_CusPos	=	0;
		}
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinDdgtBase::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcCipherInc();
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinDdgtBase::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcCipherDec();
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlInit();
		LcValTxtUpdt();
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::LcCipherUpdt( void )
{
	// local -------------------
		__u32	Tv_Val;
	// code --------------------
		m_MaxCipher	=	1;
		Tv_Val			=	m_ParaDoSpinDdgtBase.MaxVal;
		while ( 10 <= Tv_Val )
		{
			m_MaxCipher ++;
			Tv_Val	=	Tv_Val / 10;
		}
		// check cursor position
		if ( m_CusPos >= m_MaxCipher )
		{
			m_CusPos	=	m_MaxCipher - 1;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::LcCipherInc( void )
{
	// local -------------------
		__s32		Tv_Val;
	// code --------------------
		// patch cursor position
		if ( m_CusPos >= m_MaxCipher )
		{
			m_CusPos	=	m_MaxCipher - 1;
		}
		// increase value on cursor
		if ( (WCHAR)'9' == m_StrTxt[m_CusPos] )
		{
			m_StrTxt[m_CusPos]	=	(WCHAR)'0';
		}
		else
		{
			m_StrTxt[m_CusPos] ++;
		}
		// check over value
		if ( GrStrWstrToInt( m_StrTxt, &Tv_Val ) )
		{
			m_Value	=	(__u32)Tv_Val;
			if ( m_Value > m_ParaDoSpinDdgtBase.MaxVal )
			{
				m_Value	=	m_ParaDoSpinDdgtBase.MaxVal;
				if ( m_ParaDoSpinDdgtBase.IsValRotate )
				{
					m_Value	=	m_ParaDoSpinDdgtBase.MinVal;
				}
			}
			else if ( m_Value < m_ParaDoSpinDdgtBase.MinVal )
			{
				m_Value	=	m_ParaDoSpinDdgtBase.MinVal;
				if ( m_ParaDoSpinDdgtBase.IsValRotate )
				{
					m_Value	=	m_ParaDoSpinDdgtBase.MaxVal;
				}
			}
		}
		else
		{
			m_Value	=	m_ParaDoSpinDdgtBase.MaxVal;
			if ( m_ParaDoSpinDdgtBase.IsValRotate )
			{
				m_Value	=	m_ParaDoSpinDdgtBase.MinVal;
			}
		}
		LcValTxtUpdt();
		LcValSave();
		LcNotifyValChg();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::LcCipherDec( void )
{
	// local -------------------
		__s32		Tv_Val;
	// code --------------------
		// patch cursor position
		if ( m_CusPos >= m_MaxCipher )
		{
			m_CusPos	=	m_MaxCipher - 1;
		}
		// decrease value on cursor
		if ( (WCHAR)'0' == m_StrTxt[m_CusPos] )
		{
			m_StrTxt[m_CusPos]	=	(WCHAR)'9';
		}
		else
		{
			m_StrTxt[m_CusPos] --;
		}
		// check over value
		if ( GrStrWstrToInt( m_StrTxt, &Tv_Val ) )
		{
			m_Value	=	(__u32)Tv_Val;
			if ( m_Value < m_ParaDoSpinDdgtBase.MinVal )
			{
				m_Value	=	m_ParaDoSpinDdgtBase.MinVal;
				if ( m_ParaDoSpinDdgtBase.IsValRotate )
				{
					m_Value	=	m_ParaDoSpinDdgtBase.MaxVal;
				}
			}
			else	if ( m_Value > m_ParaDoSpinDdgtBase.MaxVal )
			{
				m_Value	=	m_ParaDoSpinDdgtBase.MaxVal;
				if ( m_ParaDoSpinDdgtBase.IsValRotate )
				{
					m_Value	=	m_ParaDoSpinDdgtBase.MinVal;
				}
			}
		}
		else
		{
			m_Value	=	m_ParaDoSpinDdgtBase.MinVal;
			if ( m_ParaDoSpinDdgtBase.IsValRotate )
			{
				m_Value	=	m_ParaDoSpinDdgtBase.MaxVal;
			}
		}
		LcValSave();
		LcValTxtUpdt();
		LcNotifyValChg();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinDdgtBase::LcCusRight( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( (m_CusPos + 1) < m_MaxCipher )
		{
			m_CusPos ++;
			Tv_Result	=	TRUE;
		}
		else
		{
			if ( m_ParaDoSpinDdgtBase.IsCipherRotate )
			{
				m_CusPos	=	0;
				Tv_Result	=	TRUE;
			}
			else
			{
				if ( m_ParaDoSpinDdgtBase.IsRelExtDigit )
				{
					// move focus
					Tv_Result	=	MoveFocusByDir( E_GrP3DoFocusMvIdxRight, E_GrP3DoFocusAtbDigitCtrl );
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinDdgtBase::LcCusLeft( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 != m_CusPos )
		{
			m_CusPos --;
			Tv_Result	=	TRUE;
		}
		else
		{
			if ( m_ParaDoSpinDdgtBase.IsCipherRotate )
			{
				m_CusPos	=	m_MaxCipher - 1;
				Tv_Result	=	TRUE;
			}
			else
			{
				if ( m_ParaDoSpinDdgtBase.IsRelExtDigit )
				{
					// move focus
					Tv_Result	=	MoveFocusByDir( E_GrP3DoFocusMvIdxLeft, E_GrP3DoFocusAtbDigitCtrl );
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::ValControlSet( BOOL8 A_IsCtrl )
{
	// local -------------------
	// code --------------------
		if ( !m_ParaDoSpinDdgtBase.IsOnKeyCtrl )
		{
			m_IsCtrlMode	=	A_IsCtrl;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinDdgtBase::ProcEvtFocusDirection( __u8 A_Direction, __u32 A_Atb )
{
		if ( 0 != (E_GrP3DoFocusAtbDigitCtrl & A_Atb) )
		{
			if ( !m_ParaDoSpinDdgtBase.IsOnKeyCtrl )
			{
				m_IsCtrlMode	=	TRUE;
			}
		}
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::ProcEvtVkbdEditEnd( void )
{
	// local -------------------
		__s32		Tv_Val;
	// code --------------------
		// patch value
		if ( GrStrWstrToInt( m_StrTxt, &Tv_Val ) )
		{
			m_Value	=	(__u32)Tv_Val;
			if ( m_Value > m_ParaDoSpinDdgtBase.MaxVal )
			{
				m_Value	=	m_ParaDoSpinDdgtBase.MaxVal;
				LcValTxtUpdt();
			}
			else if ( m_Value < m_ParaDoSpinDdgtBase.MinVal )
			{
				m_Value	=	m_ParaDoSpinDdgtBase.MinVal;
				LcValTxtUpdt();
			}
		}
		else
		{
			m_Value	=	m_ParaDoSpinDdgtBase.MinVal;
			LcValTxtUpdt();
		}
		LcValSave();
		LcNotifyValChg();

		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinDdgtBase::LcValSave( void )
{

}
//--------------------------------------------------------------------



