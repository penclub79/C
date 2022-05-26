/*
		platform slider bar setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSbarBase.h>
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

#define E_GrP3DoSbarBaseParaCnt		25

St_GrP3DvlDoPrInfoData	V_GrP3DoSbarBaseParaInfo[E_GrP3DoSbarBaseParaCnt]	=	
{
	{	L"PntBtnStatDis",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->PntBtnStat[E_GrP3DoSbarBaseStatDis],NULL,			L"Paint button status disable"	},
	{	L"PntBtnStatNrm",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->PntBtnStat[E_GrP3DoSbarBaseStatNrm],NULL,			L"Paint button status normal"	},
	{	L"PntBtnStatCus",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->PntBtnStat[E_GrP3DoSbarBaseStatCus],NULL,			L"Paint button status cursor"	},
	{	L"PntBtnStatCtrl",	TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->PntBtnStat[E_GrP3DoSbarBaseStatCtrl],NULL,			L"Paint button status control"	},

	{	L"PntBgStatDis",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->PntBgStat[E_GrP3DoSbarBaseStatDis],NULL,			L"Paint background status disable"	},
	{	L"PntBgStatNrm",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->PntBgStat[E_GrP3DoSbarBaseStatNrm],NULL,			L"Paint background status normal"	},
	{	L"PntBgStatCus",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->PntBgStat[E_GrP3DoSbarBaseStatCus],NULL,			L"Paint background status cursor"	},
	{	L"PntBgStatCtrl",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->PntBgStat[E_GrP3DoSbarBaseStatCtrl],NULL,			L"Paint background status control"	},

	{	L"BtnSize",					TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->BtnSize,NULL,			L"button size"	},

	{	L"BtnAreaLeft",			TRUE,		E_GrP3DvlParaTypePosBssLt,			1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->BtnBss,NULL,	L"area position left"	},
	{	L"",								FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->BtnArea.left,NULL,		L""	},
	{	L"BtnAreaTop",			TRUE,		E_GrP3DvlParaTypePosBssUp,			1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->BtnBss,NULL,	L"area position top"	},
	{	L"",								FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->BtnArea.top,NULL,			L""	},
	{	L"BtnAreaRight",		TRUE,		E_GrP3DvlParaTypePosBssRt,			1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->BtnBss,NULL,	L"area position right"	},
	{	L"",								FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->BtnArea.right,NULL,		L""	},
	{	L"BtnAreaBottom",		TRUE,		E_GrP3DvlParaTypePosBssDn,			1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->BtnBss,NULL,	L"area position bottom"	},
	{	L"",								FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->BtnArea.bottom,NULL,	L""	},

	{	L"IsVertical",			TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->IsVertical,NULL,			L"vertical type"	},
	{	L"IsReverse",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->IsReverse,NULL,			L"reverse position"	},
	{	L"IsShowVal",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->IsShowVal,NULL,			L"show value"	},
	{	L"IsOnKyeCtrl",			TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->IsOnKeyCtrl,NULL,			L"key control able at focus"	},

	{	L"Value Min",				TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->ValMin,NULL,			L"Value range minimal"	},
	{	L"Value Max",				TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->ValMax,NULL,			L"Value range maximal"	},

	{	L"EvtHostId",				TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->EvtHostId,NULL,			L"event host id"	},
	{	L"AsmValChg",				TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoSbarBasePara)0)->AsmValChg,NULL,			L"assembly code for value change event"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSbarBase::Cls_GrP3DoSbarBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId			=	E_GrP3DoClassSbarBase;
		m_ParaSize		=	m_ParaSize + sizeof(m_ParaSbarBase) + sizeof(St_GrP3ScptParaHd);
		m_IsKeyAble		=	TRUE;
		m_IsMouseAble	=	TRUE;
		
		GrDumyZeroMem( &m_ParaSbarBase, sizeof(m_ParaSbarBase) );
		m_ParaSbarBase.ValMax	=	100;

		m_IsCtrlMode	=	FALSE;
		m_Value				=	50;
		GrStrClear( m_StrVal );
}
//--------------------------------------------------------------------
Cls_GrP3DoSbarBase::~Cls_GrP3DoSbarBase( void )
{
		//
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarBase::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSbarBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSbarBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSbarBase, sizeof(m_ParaSbarBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSbarBase)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSbarBase, sizeof(m_ParaSbarBase) , m_PtrScpt , E_GrP3DoClassSbarBase );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSbarBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
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
				LcValDispUpdt();
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarBase::ProcPaint( void )
{
	// local -------------------
		St_GrRect	Tv_RcArea;
		__s32		Tv_Pos;
		__s32		Tv_Len;
		__s32		Tv_Vpos;
		__s32		Tv_Vrng;
		__s32		Tv_Rpos;
		St_GrRect	Tv_RcBtn;
		BOOL8	Tv_IsBtnAble;
		__u8	Tv_Stat;
	// code --------------------
		// init
		Tv_IsBtnAble	=	FALSE;
		// status
		Tv_Stat	=	E_GrP3DoSbarBaseStatDis;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP3DoSbarBaseStatNrm;
			if ( m_IsCtrlMode )
			{
				Tv_Stat	=	E_GrP3DoSbarBaseStatCtrl;
			}
			else
			{
				if ( m_IsOnMouse || IsFocused() )
				{
					Tv_Stat	=	E_GrP3DoSbarBaseStatCus;
				}
			}
		}

		// graph
		if ( m_ParaSbarBase.ValMax > m_ParaSbarBase.ValMin )
		{
			if ( (m_Value >= m_ParaSbarBase.ValMin) && (m_Value <= m_ParaSbarBase.ValMax) )
			{
				if ( BasisRectCalc( &m_PntRcScr, m_ParaSbarBase.BtnBss, 
							m_ParaSbarBase.BtnArea.left, m_ParaSbarBase.BtnArea.top, m_ParaSbarBase.BtnArea.right, m_ParaSbarBase.BtnArea.bottom, 
							&Tv_RcArea ) )
				{
					Tv_Vrng	=	m_ParaSbarBase.ValMax - m_ParaSbarBase.ValMin;
					Tv_Vpos	=	m_Value - m_ParaSbarBase.ValMin;
					if ( m_ParaSbarBase.IsVertical )
					{
						// vertical
						if ( m_ParaSbarBase.BtnSize < (Tv_RcArea.bottom - Tv_RcArea.top) )
						{
							Tv_IsBtnAble	=	TRUE;
							Tv_Len	=	Tv_RcArea.bottom - Tv_RcArea.top - m_ParaSbarBase.BtnSize;
							Tv_Rpos	=	0;
							if ( 0 != Tv_Vrng )
							{
								Tv_Rpos	=	((Tv_Len * Tv_Vpos) / Tv_Vrng);
							}
							Tv_Pos	=	Tv_Rpos + Tv_RcArea.top;
							if ( m_ParaSbarBase.IsReverse )
							{
								Tv_Pos	=	Tv_RcArea.bottom - Tv_Rpos - m_ParaSbarBase.BtnSize;
							}
							//make button rect
							Tv_RcBtn.left		=	Tv_RcArea.left;
							Tv_RcBtn.right	=	Tv_RcArea.right;
							Tv_RcBtn.top		=	Tv_Pos;
							Tv_RcBtn.bottom	=	Tv_Pos + m_ParaSbarBase.BtnSize;
						}
					}
					else
					{
						// horizontal
						if ( m_ParaSbarBase.BtnSize < (Tv_RcArea.right - Tv_RcArea.left) )
						{
							Tv_IsBtnAble	=	TRUE;
							Tv_Len	=	Tv_RcArea.right - Tv_RcArea.left - m_ParaSbarBase.BtnSize;
							Tv_Rpos	=	0;
							if ( 0 != Tv_Vrng )
							{
								Tv_Rpos	=	((Tv_Len * Tv_Vpos) / Tv_Vrng);
							}
							Tv_Pos	=	Tv_Rpos + Tv_RcArea.left;
							if ( m_ParaSbarBase.IsReverse )
							{
								Tv_Pos	=	Tv_RcArea.right - Tv_Rpos - m_ParaSbarBase.BtnSize;
							}
							//make button rect
							Tv_RcBtn.left		=	Tv_Pos;
							Tv_RcBtn.right	=	Tv_Pos + m_ParaSbarBase.BtnSize;
							Tv_RcBtn.top		=	Tv_RcArea.top;
							Tv_RcBtn.bottom	=	Tv_RcArea.bottom;
						}
					}
					if ( Tv_IsBtnAble )
					{
						PaintScriptPush( m_ParaSbarBase.PntBtnStat[Tv_Stat], &Tv_RcBtn );
					}
				}
			}
		}
		
		// back ground
		PaintScriptPush( m_ParaSbarBase.PntBgStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarBase::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSbarBase, V_GrP3DoSbarBaseParaInfo, E_GrP3DoSbarBaseParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarBase::LcValDispUpdt( void )
{
	// local -------------------
	// code --------------------
		// update value string
		if ( m_ParaSbarBase.IsShowVal && (m_ParaSbarBase.ValMax > m_ParaSbarBase.ValMin) )
		{
			GrStrIntToWstr( m_StrVal, m_Value );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoSbarBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;

		if ( 0 == A_Idx )
		{
			Tv_Result	=	m_StrVal;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarBase::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSbarBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaSbarBase.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP3KeyLeft:
					if ( !m_ParaSbarBase.IsVertical )
					{
						if ( m_ParaSbarBase.IsReverse )
						{
							LcValInc();
						}
						else
						{
							LcValDec();
						}
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyRight:
					if ( !m_ParaSbarBase.IsVertical )
					{
						if ( m_ParaSbarBase.IsReverse )
						{
							LcValDec();
						}
						else
						{
							LcValInc();
						}
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyUp:
					if ( m_ParaSbarBase.IsVertical )
					{
						if ( m_ParaSbarBase.IsReverse )
						{
							LcValDec();
						}
						else
						{
							LcValInc();
						}
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyDown:
					if ( m_ParaSbarBase.IsVertical )
					{
						if ( m_ParaSbarBase.IsReverse )
						{
							LcValInc();
						}
						else
						{
							LcValDec();
						}
						Tv_Result	=	TRUE;
					}
					break;

			}
		}

		// change event
		if ( Tv_Result )
		{
			ReqDraw();
			LcValChgNotify();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSbarBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaSbarBase.IsOnKeyCtrl )
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
void	Cls_GrP3DoSbarBase::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarBase::LcValSave( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarBase::LcValInc( void )
{
	// local -------------------
	// code --------------------
		// get value
		LcValLoad();
		m_Value ++;
		if ( m_ParaSbarBase.ValMax < m_Value )
		{
			m_Value	=	m_ParaSbarBase.ValMax;
		}
		LcValSave();
		LcValDispUpdt();		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarBase::LcValDec( void )
{
	// local -------------------
	// code --------------------
		LcValLoad();
		m_Value --;
		if ( m_ParaSbarBase.ValMin > m_Value )
		{
			m_Value	=	m_ParaSbarBase.ValMin;
		}
		LcValSave();
		LcValDispUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarBase::LcValChgNotify( void )
{
	// local -------------------
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		// notify message
		if ( E_GrP3ScptIdNone != m_ParaSbarBase.EvtHostId )
		{
			Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdSbarValChg;
			Tv_Msg.Hd.ConIdx	=	((Cls_GrP3ConBase*)m_ObjCon)->IndexGet();
			Tv_Msg.Hd.WinId		=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_ParaSbarBase.EvtHostId;
			Tv_Msg.ObjId			=	m_ScptId;
			((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		
		// call assembly
		if ( E_GrP3AsmIdNone != m_ParaSbarBase.AsmValChg )
		{
			V_GrP3Mng->VmRunCode( m_ParaSbarBase.AsmValChg, this );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSbarBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect	Tv_RcScr;
		St_GrRect	Tv_RcArea;
		__s32		Tv_Vrng;
		__s32		Tv_Len;
		__s32		Tv_Pos;
	// code --------------------
		// check value range
		if ( m_ParaSbarBase.ValMax > m_ParaSbarBase.ValMin )
		{
			// check position
			if ( ScrRectGet( &Tv_RcScr ) )
			{
				// get button basis rect
				if ( BasisRectCalc( &Tv_RcScr, m_ParaSbarBase.BtnBss, 
					m_ParaSbarBase.BtnArea.left, m_ParaSbarBase.BtnArea.top, m_ParaSbarBase.BtnArea.right, m_ParaSbarBase.BtnArea.bottom, 
					&Tv_RcArea ) )
				{
					Tv_Vrng	=	m_ParaSbarBase.ValMax - m_ParaSbarBase.ValMin;
					// get mouse position
					if ( m_ParaSbarBase.IsVertical )
					{
						// vertical
						Tv_Len	=	Tv_RcArea.bottom - Tv_RcArea.top - m_ParaSbarBase.BtnSize;
						Tv_Pos	=	A_Y - Tv_RcArea.top - ((__s32)m_ParaSbarBase.BtnSize >> 1);
						if ( m_ParaSbarBase.IsReverse )
						{
							Tv_Pos	=	Tv_RcArea.bottom - A_Y - ((__s32)m_ParaSbarBase.BtnSize >> 1);
						}
					}
					else
					{
						// horizontal
						Tv_Len	=	Tv_RcArea.right - Tv_RcArea.left - m_ParaSbarBase.BtnSize;
						Tv_Pos	=	A_X - Tv_RcArea.left - ((__s32)m_ParaSbarBase.BtnSize >> 1);
						if ( m_ParaSbarBase.IsReverse )
						{
							Tv_Pos	=	Tv_RcArea.right - A_X - ((__s32)m_ParaSbarBase.BtnSize >> 1);
						}
					}
					// check able
					if ( 0 < Tv_Len )
					{
						// patch range
						if ( 0 > Tv_Pos )
						{
							Tv_Pos	=	0;
						}
						if ( Tv_Len < Tv_Pos )
						{
							Tv_Pos	=	Tv_Len;
						}
						// calculate value
						m_Value	=	(Tv_Pos * Tv_Vrng) / Tv_Len;
						m_Value	=	m_Value + m_ParaSbarBase.ValMin;

						// update value
						LcValSave();
						ReqDraw();
						LcValChgNotify();

					}
				}
			}
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSbarBase::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcValInc();
		LcValChgNotify();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSbarBase::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcValDec();
		LcValChgNotify();
		return	TRUE;
}
//--------------------------------------------------------------------
