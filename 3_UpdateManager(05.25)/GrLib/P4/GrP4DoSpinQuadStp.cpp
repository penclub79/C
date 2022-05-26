/*
		platform spin QUAD setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSpinQuadStp.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>
#include <P4/GrP4Cfg.h>

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

extern	__u8		V_GrDvrQuadPageCntTbl[E_GrDvrQuadModeCnt];

#ifdef GR_P4_EDITOR

#define E_GrP4DoSpinQuadStpParaCnt		13

St_GrP4DvlDoPrInfoData	V_GrP4DoSpinQuadStpParaInfo[E_GrP4DoSpinQuadStpParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->PntStat[E_GrP4DoSpinQuadStpStatDisable],NULL,	L"Paint status disable"	},
	{	L"Paint Normal",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->PntStat[E_GrP4DoSpinQuadStpStatNormal],NULL,	L"Paint status normal"	},
	{	L"Paint Focus",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->PntStat[E_GrP4DoSpinQuadStpStatFocus],NULL,	L"Paint status focus"	},
	{	L"Paint Control",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->PntStat[E_GrP4DoSpinQuadStpStatControl],NULL,	L"Paint status control value"	},
	{	L"IsOnKeyCtrl",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsVertical",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->IsVertical,NULL,	L"vertical type"	},
	{	L"Cipher",						TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->Cipher,NULL,	L"display cipher"	},
	{	L"PrefixTxtId",				TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->PrefixTxtId,NULL,	L"prefix text id"	},
	{	L"SuffixTxtId",				TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->SuffixTxtId,NULL,	L"suffix text id"	},
	{	L"AsmEvtValChg",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->AsmEvtValChg,NULL,	L"assembly code id for the value change event"	},
	{	L"QuadMode",					TRUE,		E_GrP4DvlParaTypeQuadMode,			1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->QuadMode,NULL,	L"Quad mode"	},
	{	L"ViewIdx",						TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->ViewIdx,NULL,	L"Quad View Index"	},
	{	L"RelPaint",					TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinQuadStpPara)0)->RelPnt,NULL,	L"Relate Paint object"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSpinQuadStp::Cls_GrP4DoSpinQuadStp(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSpinQuadStp;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinQuadStp) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaSpinQuadStp, sizeof(m_ParaSpinQuadStp) );

		m_PtrCfgPage	=	NULL;

		m_VdoChCnt	=	V_GrP4Mng->VdoMaxChCntGet();

		m_IsCtrlMode	=	FALSE;
		m_Value				=	0;

		LcValTxtUpdt();

}
//--------------------------------------------------------------------
Cls_GrP4DoSpinQuadStp::~Cls_GrP4DoSpinQuadStp( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinQuadStp::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSpinQuadStp;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinQuadStp);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinQuadStp, sizeof(m_ParaSpinQuadStp) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinQuadStp)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinQuadStp::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinQuadStp, sizeof(m_ParaSpinQuadStp) , m_PtrScpt , E_GrP4DoClassSpinQuadStp );

		LcCfgPosUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinQuadStp::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
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
				m_Value	=	(__u8)A_PtrInt[1];
				LcValSave();
				LcValTxtUpdt();
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinQuadStp::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinQuadStp, V_GrP4DoSpinQuadStpParaInfo, E_GrP4DoSpinQuadStpParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinQuadStp::LcValLoad( void )
{
	// local -------------------
	// code --------------------
		if ( NULL != m_PtrCfgPage )
		{
			m_Value	=	m_PtrCfgPage[m_CfgViewOfs];
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinQuadStp::LcValSave( void )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		// update value
		LcChgVal( m_Value );
		// relate redraw
		if ( E_GrP4ScptIdNone != m_ParaSpinQuadStp.RelPnt )
		{
			if ( NULL != m_ObjWin )
			{
				Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaSpinQuadStp.RelPnt );
				if ( NULL != Tv_Obj )
				{
					Tv_Obj->ReqDraw();
				}
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinQuadStp::LcCfgPosUpdt( void )
{
	// local -------------------
		void*	Tv_PtrCfg;
		__u8	Tv_PageIdx;
		__u8	Tv_ViewTotal;
		__u8	Tv_PageCnt;
	// code --------------------
		// init
		m_PtrCfgPage	=	NULL;
		if ( E_GrDvrQuadModeCnt > m_ParaSpinQuadStp.QuadMode )
		{
			Tv_PtrCfg			=	V_GrP4Mng->SetupDataPtrGet( E_GrP4CfgOfsQuadMap );
			m_ViewCnt			=	GrDvrQuadViewCount( m_ParaSpinQuadStp.QuadMode );
			Tv_PageCnt		=	V_GrDvrQuadPageCntTbl[m_ParaSpinQuadStp.QuadMode];
			Tv_ViewTotal	=	Tv_PageCnt * m_ViewCnt;
			if ( Tv_ViewTotal > m_ParaSpinQuadStp.ViewIdx )
			{
				Tv_PageIdx		=	m_ParaSpinQuadStp.ViewIdx / m_ViewCnt;
				m_CfgViewOfs	=	m_ParaSpinQuadStp.ViewIdx % m_ViewCnt;
				m_PtrCfgPage	=	GrDvrQuadMapPtr( Tv_PtrCfg, m_ParaSpinQuadStp.QuadMode, Tv_PageIdx );

				m_ChSt				=	(m_ViewCnt * Tv_PageIdx) % m_VdoChCnt;
				m_ChRmCnt			=	0;
				if ( m_VdoChCnt < (m_ChSt + m_ViewCnt) )
				{
					m_ChRmCnt	=	(m_ChSt + m_ViewCnt) - m_VdoChCnt;
				}
				
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinQuadStp::LcValTxtUpdt( void )
{
	// local -------------------
		BOOL8	Tv_IsFix;
		WCHAR*	Tv_StrWk;
		WCHAR*	Tv_StrAdd;
	// code --------------------
		Tv_StrWk	=	m_StrTxt;

		// add prefix
		if ( E_GrP4ScptTxtIdNone != m_ParaSpinQuadStp.PrefixTxtId )
		{
			Tv_StrAdd	=	V_GrP4Mng->TextPtrGet( m_ParaSpinQuadStp.PrefixTxtId );
			Tv_StrWk	=	GrStrWcopy( Tv_StrWk, Tv_StrAdd, 12 );
		}

		Tv_IsFix	=	FALSE;
		if ( 0 != m_ParaSpinQuadStp.Cipher )
		{
			Tv_IsFix	=	TRUE;
		}
		GrStrIntToWstr( Tv_StrWk, m_Value + 1, (__u32)m_ParaSpinQuadStp.Cipher, Tv_IsFix );

		// add suffix
		if ( E_GrP4ScptTxtIdNone != m_ParaSpinQuadStp.SuffixTxtId )
		{
			Tv_StrAdd	=	V_GrP4Mng->TextPtrGet( m_ParaSpinQuadStp.SuffixTxtId );
			Tv_StrWk	=	GrStrWcat( Tv_StrWk, Tv_StrAdd, 12 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinQuadStp::LcChgVal( __u8 A_NewVal )
{
	// local -------------------
		__u8	Tv_NowVal;
		__u8	Tv_WkIdx;
	// code --------------------
		if ( NULL != m_PtrCfgPage )
		{
			// get now value
			Tv_NowVal	=	m_PtrCfgPage[m_CfgViewOfs];
			for ( Tv_WkIdx=0;Tv_WkIdx < m_ViewCnt;Tv_WkIdx++ )
			{
				if ( A_NewVal == m_PtrCfgPage[Tv_WkIdx] )
				{
					// change
					m_PtrCfgPage[Tv_WkIdx]	=	Tv_NowVal;
				}
			}
			// update new value
			m_PtrCfgPage[m_CfgViewOfs]	=	A_NewVal;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinQuadStp::LcNotifyValChg( void )
{
	// local -------------------
		St_GrP4MngVmRpara	Tv_Rpara;
	// code --------------------
		// user event
		if ( V_GrP4Mng->IsUiEventAble() )
		{
			// run event assembly
			Tv_Rpara.In.Cnt		=	0;

			Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	1;
			Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	(__s32)((__u32)m_Value);
			Tv_Rpara.In.Cnt ++;

			Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	2;
			Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	(__s32)((__u32)m_Orignal);
			Tv_Rpara.In.Cnt ++;

			Tv_Rpara.Out.Cnt	=	0;

			V_GrP4Mng->VmRunCode( m_ParaSpinQuadStp.AsmEvtValChg, this, &Tv_Rpara );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinQuadStp::LcValIncrease( void )
{
	// local -------------------
	// code --------------------
		LcValLoad();
		m_Orignal	=	m_Value;
		m_Value	++;
		if ( 0 != m_ChRmCnt )
		{
			if ( m_Value < m_ChSt )
			{
				if ( m_Value >= m_ChRmCnt )
				{
					m_Value	=	m_ChSt;
				}
			}
			else
			{
				if ( m_VdoChCnt <= m_Value )
				{
					m_Value	=	0;
				}
			}
		}
		else
		{
			if ( (m_ChSt + m_ViewCnt) <= m_Value )
			{
				m_Value	=	m_ChSt;
			}
		}

		LcValSave();
		LcValTxtUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinQuadStp::LcValDecrease( void )
{
	// local -------------------
	// code --------------------
		LcValLoad();
		m_Orignal	=	m_Value;

		if ( 0 == m_Value )
		{
			if ( 0 != m_ChRmCnt )
			{
				m_Value	=	m_ChSt;
			}
			else
			{
				m_Value	=	m_ChSt + m_ViewCnt - 1;
			}
		}
		else
		{
			m_Value --;
			if ( 0 != m_ChRmCnt )
			{
				if ( (m_ChRmCnt <= m_Value) && (m_ChSt > m_Value) )
				{
					m_Value	=	m_ChRmCnt - 1;
				}
			}
			else
			{
				if ( m_ChSt > m_Value )
				{
					m_Value	=	m_ChSt + m_ViewCnt - 1;
				}
			}
		}

		LcValSave();
		LcValTxtUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinQuadStp::ProcEvtFocusGet( void )
{
		Cls_GrP4DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinQuadStp::ProcEvtFocusLoss( void )
{
		m_IsCtrlMode	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinQuadStp::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaSpinQuadStp.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP4KeyLeft:
					if ( !m_ParaSpinQuadStp.IsVertical )
					{
						LcValDecrease();
						LcNotifyValChg();
						ReqDraw();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyRight:
					if ( !m_ParaSpinQuadStp.IsVertical )
					{
						LcValIncrease();
						LcNotifyValChg();
						ReqDraw();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyUp:
					if ( m_ParaSpinQuadStp.IsVertical )
					{
						LcValIncrease();
						LcNotifyValChg();
						ReqDraw();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyDown:
					if ( m_ParaSpinQuadStp.IsVertical )
					{
						LcValDecrease();
						LcNotifyValChg();
						ReqDraw();
						Tv_Result	=	TRUE;
					}
					break;

			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinQuadStp::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaSpinQuadStp.IsOnKeyCtrl )
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
BOOL8	Cls_GrP4DoSpinQuadStp::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__s32		Tv_Ct;
	// code --------------------
		// check position
		if ( ScrRectGet( &Tv_Rc ) )
		{
			if ( m_ParaSpinQuadStp.IsVertical )
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
BOOL8	Cls_GrP4DoSpinQuadStp::ProcEvtWheelUp( void )
{
		LcValIncrease();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinQuadStp::ProcEvtWheelDown( void )
{
		LcValDecrease();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinQuadStp::ProcPaint( void )
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
		Tv_Stat	=	E_GrP4DoSpinQuadStpStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoSpinQuadStpStatNormal;
			if ( m_IsCtrlMode ||  m_IsOnMouse )
			{
				Tv_Stat	=	E_GrP4DoSpinQuadStpStatControl;
			}
			else
			{
				if ( IsFocused() )
				{
					Tv_Stat	=	E_GrP4DoSpinQuadStpStatFocus;
					if ( m_ParaSpinQuadStp.IsOnKeyCtrl )
					{
						Tv_Stat	=	E_GrP4DoSpinQuadStpStatControl;
					}
				}
			}
		}
		PaintScriptPush( m_ParaSpinQuadStp.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoSpinQuadStp::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
void	Cls_GrP4DoSpinQuadStp::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlInit();
		LcValLoad();
		LcValTxtUpdt();
		ReqDraw();
}
//--------------------------------------------------------------------

