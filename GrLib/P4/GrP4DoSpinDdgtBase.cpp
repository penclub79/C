/*
		platform spin decimal digit base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSpinDdgtBase.h>
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

#define E_GrP4DoSpinDdgtBaseParaCnt		10

St_GrP4DvlDoPrInfoData	V_GrP4DoSpinDdgtBaseParaInfo[E_GrP4DoSpinDdgtBaseParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinDdgtBasePara)0)->PntStat[E_GrP4DoSpinDdgtStatDisable],NULL,	L"Paint status disable"	},
	{	L"Paint Normal",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinDdgtBasePara)0)->PntStat[E_GrP4DoSpinDdgtStatNormal],NULL,	L"Paint status normal"	},
	{	L"Paint Focus",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinDdgtBasePara)0)->PntStat[E_GrP4DoSpinDdgtStatFocus],NULL,	L"Paint status focus"	},
	{	L"Paint Control",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinDdgtBasePara)0)->PntStat[E_GrP4DoSpinDdgtStatControl],NULL,	L"Paint status control value"	},
	{	L"IsOnKeyCtrl",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinDdgtBasePara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsValRotate",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinDdgtBasePara)0)->IsValRotate,NULL,	L"value rotate"	},
	{	L"IsCipherRotate",		TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinDdgtBasePara)0)->IsCipherRotate,NULL,	L"cipher rotate"	},
	{	L"MinVal",						TRUE,		E_GrP4DvlParaTypeInt,						1,	(void*)&((Ptr_GrP4DoSpinDdgtBasePara)0)->MinVal,NULL,	L"minimum value limite"	},
	{	L"MaxVal",						TRUE,		E_GrP4DvlParaTypeInt,						1,	(void*)&((Ptr_GrP4DoSpinDdgtBasePara)0)->MaxVal,NULL,	L"maximum value limite"	},
	{	L"AsmEvtValChg",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoSpinDdgtBasePara)0)->AsmEvtValChg,NULL,	L"assembly code id for the value change event"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSpinDdgtBase::Cls_GrP4DoSpinDdgtBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSpinDdgtBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoSpinDdgtBase) + sizeof(St_GrP4ScptParaHd);
		
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
Cls_GrP4DoSpinDdgtBase::~Cls_GrP4DoSpinDdgtBase( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinDdgtBase::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSpinDdgtBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoSpinDdgtBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoSpinDdgtBase, sizeof(m_ParaDoSpinDdgtBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoSpinDdgtBase)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoSpinDdgtBase, sizeof(m_ParaDoSpinDdgtBase) , m_PtrScpt , E_GrP4DoClassSpinDdgtBase );

		if ( m_Value < m_ParaDoSpinDdgtBase.MinVal )
		{
			m_Value	=	m_ParaDoSpinDdgtBase.MinVal;
		}
		

		LcCipherUpdt();
		LcValTxtUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinDdgtBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
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
				LcValTxtUpdt();
				ReqDraw();
				break;
			case E_GrP4UpiMinValueGet:
				A_PtrInt[0]	=	m_ParaDoSpinDdgtBase.MinVal;
				break;
			case E_GrP4UpiMinValueSet:
				m_ParaDoSpinDdgtBase.MinVal	=	A_PtrInt[1];
				if ( m_Value < m_ParaDoSpinDdgtBase.MinVal )
				{
					m_Value	=	m_ParaDoSpinDdgtBase.MinVal;
					LcValSave();
					LcValTxtUpdt();
					ReqDraw();
				}
				break;
			case E_GrP4UpiMaxValueGet:
				A_PtrInt[0]	=	m_ParaDoSpinDdgtBase.MaxVal;
				break;
			case E_GrP4UpiMaxValueSet:
				m_ParaDoSpinDdgtBase.MaxVal	=	A_PtrInt[1];
				if ( m_Value > m_ParaDoSpinDdgtBase.MaxVal )
				{
					m_Value	=	m_ParaDoSpinDdgtBase.MaxVal;
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
void	Cls_GrP4DoSpinDdgtBase::ProcPaint( void )
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
		Tv_Stat	=	E_GrP4DoSpinDdgtStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoSpinDdgtStatNormal;
			if ( m_IsCtrlMode ||  m_IsOnMouse )
			{
				Tv_Stat	=	E_GrP4DoSpinDdgtStatControl;
			}
			else
			{
				if ( IsFocused() )
				{
					Tv_Stat	=	E_GrP4DoSpinDdgtStatFocus;
					if ( m_ParaDoSpinDdgtBase.IsOnKeyCtrl )
					{
						Tv_Stat	=	E_GrP4DoSpinDdgtStatControl;
					}
				}
			}
		}
		PaintScriptPush( m_ParaDoSpinDdgtBase.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinDdgtBase::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoSpinDdgtBase, V_GrP4DoSpinDdgtBaseParaInfo, E_GrP4DoSpinDdgtBaseParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtBase::LcValTxtUpdt( void )
{
	// local -------------------
	// code --------------------
		GrStrIntToWstr( m_StrTxt, (__s32)m_Value, (__u32)m_MaxCipher, TRUE );

}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoSpinDdgtBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
void	Cls_GrP4DoSpinDdgtBase::LcNotifyValChg( void )
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

			V_GrP4Mng->VmRunCode( m_ParaDoSpinDdgtBase.AsmEvtValChg, this, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtBase::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinDdgtBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaDoSpinDdgtBase.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP4KeyLeft:
					Tv_Result	=	LcCusLeft();
					break;
				case E_GrP4KeyRight:
					Tv_Result	=	LcCusRight();
					break;
				case E_GrP4KeyUp:
					LcCipherInc();
					LcNotifyValChg();
					Tv_Result	=	TRUE;
					break;
				case E_GrP4KeyDown:
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
BOOL8	Cls_GrP4DoSpinDdgtBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaDoSpinDdgtBase.IsOnKeyCtrl )
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
BOOL8	Cls_GrP4DoSpinDdgtBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		// do number virtual keyboard
		V_GrP4Mng->VkbdNumRun( this, m_StrTxt, m_MaxCipher, FALSE );
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinDdgtBase::ProcEvtMseMbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
BOOL8	Cls_GrP4DoSpinDdgtBase::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcCipherInc();
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinDdgtBase::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcCipherDec();
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtBase::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlInit();
		LcValTxtUpdt();
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtBase::LcCipherUpdt( void )
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
void	Cls_GrP4DoSpinDdgtBase::LcCipherInc( void )
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
void	Cls_GrP4DoSpinDdgtBase::LcCipherDec( void )
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
BOOL8	Cls_GrP4DoSpinDdgtBase::LcCusRight( void )
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
					Tv_Result	=	MoveFocusByDir( E_GrP4DoFocusMvIdxRight, E_GrP4DoFocusAtbDigitCtrl );
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinDdgtBase::LcCusLeft( void )
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
					Tv_Result	=	MoveFocusByDir( E_GrP4DoFocusMvIdxLeft, E_GrP4DoFocusAtbDigitCtrl );
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtBase::ValControlSet( BOOL8 A_IsCtrl )
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
BOOL8	Cls_GrP4DoSpinDdgtBase::ProcEvtFocusDirection( __u8 A_Direction, __u32 A_Atb )
{
		if ( 0 != (E_GrP4DoFocusAtbDigitCtrl & A_Atb) )
		{
			if ( !m_ParaDoSpinDdgtBase.IsOnKeyCtrl )
			{
				m_IsCtrlMode	=	TRUE;
			}
		}
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtBase::ProcEvtVkbdEditEnd( void )
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
void	Cls_GrP4DoSpinDdgtBase::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinDdgtBase::LcValSave( void )
{

}
//--------------------------------------------------------------------



