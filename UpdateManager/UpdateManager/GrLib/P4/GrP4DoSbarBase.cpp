/*
		platform slider bar setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSbarBase.h>
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

#define E_GrP4DoSbarBaseParaCnt		23

St_GrP4DvlDoPrInfoData	V_GrP4DoSbarBaseParaInfo[E_GrP4DoSbarBaseParaCnt]	=	
{
	{	L"PntBtnStatDis",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->PntBtnStat[E_GrP4DoSbarBaseStatDis],NULL,			L"Paint button status disable"	},
	{	L"PntBtnStatNrm",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->PntBtnStat[E_GrP4DoSbarBaseStatNrm],NULL,			L"Paint button status normal"	},
	{	L"PntBtnStatCus",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->PntBtnStat[E_GrP4DoSbarBaseStatCus],NULL,			L"Paint button status cursor"	},
	{	L"PntBtnStatCtrl",	TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->PntBtnStat[E_GrP4DoSbarBaseStatCtrl],NULL,			L"Paint button status control"	},

	{	L"PntBgStatDis",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->PntBgStat[E_GrP4DoSbarBaseStatDis],NULL,			L"Paint background status disable"	},
	{	L"PntBgStatNrm",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->PntBgStat[E_GrP4DoSbarBaseStatNrm],NULL,			L"Paint background status normal"	},
	{	L"PntBgStatCus",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->PntBgStat[E_GrP4DoSbarBaseStatCus],NULL,			L"Paint background status cursor"	},
	{	L"PntBgStatCtrl",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->PntBgStat[E_GrP4DoSbarBaseStatCtrl],NULL,			L"Paint background status control"	},

	{	L"BtnSize",					TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->BtnSize,NULL,			L"button size"	},

	{	L"BtnAreaLeft",			TRUE,		E_GrP4DvlParaTypePosBssLt,			1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->BtnBss,NULL,	L"area position left"	},
	{	L"",								FALSE,	E_GrP4DvlParaTypeShort,					1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->BtnArea.left,NULL,		L""	},
	{	L"BtnAreaTop",			TRUE,		E_GrP4DvlParaTypePosBssUp,			1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->BtnBss,NULL,	L"area position top"	},
	{	L"",								FALSE,	E_GrP4DvlParaTypeShort,					1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->BtnArea.top,NULL,			L""	},
	{	L"BtnAreaRight",		TRUE,		E_GrP4DvlParaTypePosBssRt,			1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->BtnBss,NULL,	L"area position right"	},
	{	L"",								FALSE,	E_GrP4DvlParaTypeShort,					1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->BtnArea.right,NULL,		L""	},
	{	L"BtnAreaBottom",		TRUE,		E_GrP4DvlParaTypePosBssDn,			1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->BtnBss,NULL,	L"area position bottom"	},
	{	L"",								FALSE,	E_GrP4DvlParaTypeShort,					1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->BtnArea.bottom,NULL,	L""	},

	{	L"IsVertical",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->IsVertical,NULL,			L"vertical type"	},
	{	L"IsReverse",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->IsReverse,NULL,			L"reverse position"	},
	{	L"IsShowVal",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->IsShowVal,NULL,			L"show value"	},
	{	L"IsOnKyeCtrl",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->IsOnKeyCtrl,NULL,			L"key control able at focus"	},

	{	L"EvtHostId",				TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->EvtHostId,NULL,			L"event host id"	},
	{	L"AsmValChg",				TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoSbarBasePara)0)->AsmValChg,NULL,			L"assembly code for value change event"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSbarBase::Cls_GrP4DoSbarBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId			=	E_GrP4DoClassSbarBase;
		m_ParaSize		=	m_ParaSize + sizeof(m_ParaSbarBase) + sizeof(St_GrP4ScptParaHd);
		m_IsKeyAble		=	TRUE;
		m_IsMouseAble	=	TRUE;
		
		GrDumyZeroMem( &m_ParaSbarBase, sizeof(m_ParaSbarBase) );
		m_ValMin	=	0;
		m_ValMax	=	100;


		m_IsCtrlMode	=	FALSE;
		m_Value				=	50;
		GrStrClear( m_StrVal );
}
//--------------------------------------------------------------------
Cls_GrP4DoSbarBase::~Cls_GrP4DoSbarBase( void )
{
		//
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarBase::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSbarBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSbarBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSbarBase, sizeof(m_ParaSbarBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSbarBase)	+ sizeof(St_GrP4ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSbarBase, sizeof(m_ParaSbarBase) , m_PtrScpt , E_GrP4DoClassSbarBase );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSbarBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
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
				LcValDispUpdt();
				ReqDraw();
				break;
			case E_GrP4UpiMinValueGet:
				A_PtrInt[0]	=	m_ValMin;
				break;
			case E_GrP4UpiMinValueSet:
				if (m_ValMin != A_PtrInt[1])
				{
					m_ValMin	=	A_PtrInt[1];
					// update
					LcValMinMaxChanged();
				}
				break;
			case E_GrP4UpiMaxValueGet:
				A_PtrInt[0]	=	m_ValMax;
				break;
			case E_GrP4UpiMaxValueSet:
				if (m_ValMax != A_PtrInt[1])
				{
					m_ValMax	=	A_PtrInt[1];
					// update
					LcValMinMaxChanged();
				}
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarBase::ProcPaint( void )
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
		Tv_Stat	=	E_GrP4DoSbarBaseStatDis;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoSbarBaseStatNrm;
			if ( m_IsCtrlMode )
			{
				Tv_Stat	=	E_GrP4DoSbarBaseStatCtrl;
			}
			else
			{
				if ( m_IsOnMouse || IsFocused() )
				{
					Tv_Stat	=	E_GrP4DoSbarBaseStatCus;
				}
			}
		}

		// graph
		if ( m_ValMax > m_ValMin )
		{
			if ( (m_Value >= m_ValMin) && (m_Value <= m_ValMax) )
			{
				if ( BasisRectCalc( &m_PntRcScr, m_ParaSbarBase.BtnBss, 
							m_ParaSbarBase.BtnArea.left, m_ParaSbarBase.BtnArea.top, m_ParaSbarBase.BtnArea.right, m_ParaSbarBase.BtnArea.bottom, 
							&Tv_RcArea ) )
				{
					Tv_Vrng	=	m_ValMax - m_ValMin;
					Tv_Vpos	=	m_Value - m_ValMin;
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
void	Cls_GrP4DoSbarBase::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P4_EDITOR
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSbarBase, V_GrP4DoSbarBaseParaInfo, E_GrP4DoSbarBaseParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarBase::LcValDispUpdt( void )
{
	// local -------------------
	// code --------------------
		// update value string
		if ( m_ParaSbarBase.IsShowVal && (m_ValMax > m_ValMin) )
		{
			GrStrIntToWstr( m_StrVal, m_Value );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoSbarBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
void	Cls_GrP4DoSbarBase::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSbarBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaSbarBase.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP4KeyLeft:
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
				case E_GrP4KeyRight:
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
				case E_GrP4KeyUp:
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
				case E_GrP4KeyDown:
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
BOOL8	Cls_GrP4DoSbarBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaSbarBase.IsOnKeyCtrl )
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
void	Cls_GrP4DoSbarBase::LcValLoad( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarBase::LcValSave( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarBase::LcValInc( void )
{
	// local -------------------
	// code --------------------
		// get value
		LcValLoad();
		m_Value ++;
		if ( m_ValMax < m_Value )
		{
			m_Value	=	m_ValMax;
		}
		LcValSave();
		LcValDispUpdt();		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarBase::LcValDec( void )
{
	// local -------------------
	// code --------------------
		LcValLoad();
		m_Value --;
		if ( m_ValMin > m_Value )
		{
			m_Value	=	m_ValMin;
		}
		LcValSave();
		LcValDispUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarBase::LcValChgNotify( void )
{
	// local -------------------
		St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		// notify message
		if ( E_GrP4ScptIdNone != m_ParaSbarBase.EvtHostId )
		{
			Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdSbarValChg;
			Tv_Msg.Hd.ConIdx	=	((Cls_GrP4ConBase*)m_ObjCon)->IndexGet();
			Tv_Msg.Hd.WinId		=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_ParaSbarBase.EvtHostId;
			Tv_Msg.ObjId			=	m_ScptId;
			((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
		
		// call assembly
		if ( E_GrP4AsmIdNone != m_ParaSbarBase.AsmValChg )
		{
			V_GrP4Mng->VmRunCode( m_ParaSbarBase.AsmValChg, this );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSbarBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect	Tv_RcScr;
		St_GrRect	Tv_RcArea;
		__s32		Tv_Vrng;
		__s32		Tv_Len;
		__s32		Tv_Pos;
	// code --------------------
		// check value range
		if ( m_ValMax > m_ValMin )
		{
			// check position
			if ( ScrRectGet( &Tv_RcScr ) )
			{
				// get button basis rect
				if ( BasisRectCalc( &Tv_RcScr, m_ParaSbarBase.BtnBss, 
					m_ParaSbarBase.BtnArea.left, m_ParaSbarBase.BtnArea.top, m_ParaSbarBase.BtnArea.right, m_ParaSbarBase.BtnArea.bottom, 
					&Tv_RcArea ) )
				{
					Tv_Vrng	=	m_ValMax - m_ValMin;
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
						m_Value	=	m_Value + m_ValMin;

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
BOOL8	Cls_GrP4DoSbarBase::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcValInc();
		LcValChgNotify();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSbarBase::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcValDec();
		LcValChgNotify();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarBase::LcValMinMaxChanged(void)
{
	// local -------------------
		BOOL8	Tv_IsChg;
	// code --------------------
		Tv_IsChg	=	FALSE;
		// change position
		LcValLoad();
		if ( m_Value < m_ValMin )
		{
			m_Value		=	m_ValMin;
			Tv_IsChg	=	TRUE;
		}
		if (m_Value > m_ValMax)
		{
			m_Value		=	m_ValMax;
			Tv_IsChg	=	TRUE;
		}

		if (Tv_IsChg)
		{
			LcValChgNotify();
		}

		LcValDispUpdt();
		ReqDraw();
}
//--------------------------------------------------------------------
__s32	Cls_GrP4DoSbarBase::ValueGet(void)
{
		LcValLoad();
		return	m_Value;
}
//--------------------------------------------------------------------
