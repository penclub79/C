/*
		platform spin text local

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSpinId.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>
#include <P3/GrP3Cfg.h>

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

#define E_GrP3DoSpinIdParaCnt		9

St_GrP3DvlDoPrInfoData	V_GrP3DoSpinIdParaInfo[E_GrP3DoSpinIdParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinIdPara)0)->PntStat[E_GrP3DoSpinIdStatDisable],NULL,	L"Paint status disable"	},
	{	L"Paint Normal",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinIdPara)0)->PntStat[E_GrP3DoSpinIdStatNormal],NULL,	L"Paint status normal"	},
	{	L"Paint Focus",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinIdPara)0)->PntStat[E_GrP3DoSpinIdStatFocus],NULL,	L"Paint status focus"	},
	{	L"Paint Control",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoSpinIdPara)0)->PntStat[E_GrP3DoSpinIdStatControl],NULL,	L"Paint status control value"	},
	{	L"IsOnKeyCtrl",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinIdPara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsVertical",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinIdPara)0)->IsVertical,NULL,	L"vertical type"	},
	{	L"IsValRotate",				TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSpinIdPara)0)->IsValRotate,NULL,	L"value rotate"	},
	{	L"AsmEvtValChg",			TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoSpinIdPara)0)->AsmEvtValChg,NULL,	L"assembly code id for the value change event"	},
	{	L"EvtHostId",			TRUE,		E_GrP3DvlParaTypeScptIdAny,					1,	(void*)&((Ptr_GrP3DoSpinIdPara)0)->EvtHostId,NULL,	L"event host object id"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSpinId::Cls_GrP3DoSpinId( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
		Ptr_GrP3ScptCfgMdl	Tv_PtrCfgMdl;
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassSpinId;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinId) + sizeof(St_GrP3ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		m_IsCtrlMode	=	FALSE;

		GrDumyZeroMem( &m_ParaSpinId, sizeof(m_ParaSpinId) );

		// get last login id
		m_Value			=	0;
		LcValLoad();

		// get user id count
		m_UserIdCnt		=	E_GrP3UserIdMax;
		Tv_PtrCfgMdl	=	V_GrP3Mng->ConfigMdlAtbGet();
		if ( NULL != Tv_PtrCfgMdl )
		{
			m_UserIdCnt	=	Tv_PtrCfgMdl->UserIdCnt;
			if ( E_GrP3UserIdMax < m_UserIdCnt )
			{
				m_UserIdCnt	=	E_GrP3UserIdMax;
			}
		}

}
//--------------------------------------------------------------------
Cls_GrP3DoSpinId::~Cls_GrP3DoSpinId( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinId::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSpinId;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinId);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinId, sizeof(m_ParaSpinId) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinId)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinId::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinId, sizeof(m_ParaSpinId) , m_PtrScpt , E_GrP3DoClassSpinId );

		LcValLoad();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinId::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiValueGet:
				A_PtrInt[0]	=	(__s32)m_Value;
				break;
			case E_GrP3UpiValueSet:
				if ( m_UserIdCnt > (__u8)A_PtrInt[1] )
				{
					m_Value	=	(__u8)A_PtrInt[1];
					ReqDraw();
				}
				break;
			case E_GrP3UpiEvtHostIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaSpinId.EvtHostId;
				break;
			case E_GrP3UpiEvtHostIdSet:
				m_ParaSpinId.EvtHostId	=	(__u16)A_PtrInt[1];
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinId::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinId, V_GrP3DoSpinIdParaInfo, E_GrP3DoSpinIdParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinId::EvtHostIdSet( __u16 A_ObjId )
{
	m_ParaSpinId.EvtHostId	=	A_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DoSpinId::EvtHostIdGet( void )
{
	return	m_ParaSpinId.EvtHostId;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinId::LcNotifyValChg( void )
{
	// local -------------------
		St_GrP3MngVmRpara	Tv_Rpara;
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		// user event
		if ( V_GrP3Mng->IsUiEventAble() )
		{
			// run event assembly
			Tv_Rpara.Cnt	=	1;
			Tv_Rpara.Ridx[0]	=	1;
			Tv_Rpara.Rval[0]	=	m_Value;
			V_GrP3Mng->VmRunCode( m_ParaSpinId.AsmEvtValChg, this, &Tv_Rpara );
		}

		if ( E_GrP3ScptIdNone != m_ParaSpinId.EvtHostId )
		{
			Tv_Msg.Hd.Cmd				=	E_GrP3MsgCmdSpinValChg;
			Tv_Msg.Hd.ConIdx		=	E_GrP3ConIdxFocus;
			Tv_Msg.Hd.WinId			=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId			=	m_ParaSpinId.EvtHostId;
			Tv_Msg.ObjId				=	m_ScptId;
			((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinId::LcValLoad( void )
{
	// local -------------------
		Ptr_GrP3MngGvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs		=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet( );
		if ( E_GrP3UserIdMax > Tv_PtrGvs->LoginIdx )
		{
			m_Value	=	Tv_PtrGvs->LoginIdx;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinId::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlInit();
		LcValLoad();
		ReqDraw();
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoSpinId::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16		Tv_Ofs;
	// code --------------------
		Tv_Result	=	NULL;

		if ( 0 == A_Idx )
		{
			if ( (0 != m_UserIdCnt) && (m_UserIdCnt > m_Value) )
			{
				Tv_Ofs	=	E_GrP3CfgOfsUserId + ( (__u16)m_Value << 5);
				Tv_Result	=	(WCHAR*)V_GrP3Mng->SetupDataPtrGet( Tv_Ofs );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinId::LcValIncrease( void )
{
	// local -------------------
	// code --------------------
		m_Value ++;
		if ( m_UserIdCnt <= m_Value )
		{
			if ( m_ParaSpinId.IsValRotate )
			{
				m_Value	=	0;
			}
			else
			{
				m_Value	=	m_UserIdCnt;
				if ( 0 != m_Value )
				{
					m_Value --;
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinId::LcValDecrease( void )
{
	// local -------------------
	// code --------------------
		if ( 0 == m_Value  )
		{
			if ( m_ParaSpinId.IsValRotate )
			{
				m_Value	=	m_UserIdCnt;
				if ( 0 != m_Value )
				{
					m_Value --;
				}
			}
		}
		else
		{
			m_Value --;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinId::ProcEvtFocusGet( void )
{
		Cls_GrP3DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinId::ProcEvtFocusLoss( void )
{
		m_IsCtrlMode	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinId::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaSpinId.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
			case E_GrP3KeyLeft:
				if ( !m_ParaSpinId.IsVertical )
				{
					LcValDecrease();
					Tv_Result	=	TRUE;
				}
				break;
			case E_GrP3KeyRight:
				if ( !m_ParaSpinId.IsVertical )
				{
					LcValIncrease();
					Tv_Result	=	TRUE;
				}
				break;
			case E_GrP3KeyUp:
				if ( m_ParaSpinId.IsVertical )
				{
					LcValIncrease();
					Tv_Result	=	TRUE;
				}
				break;
			case E_GrP3KeyDown:
				if ( m_ParaSpinId.IsVertical )
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
BOOL8	Cls_GrP3DoSpinId::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaSpinId.IsOnKeyCtrl )
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
BOOL8	Cls_GrP3DoSpinId::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__s32		Tv_Ct;
	// code --------------------
		// check position
		if ( ScrRectGet( &Tv_Rc ) )
		{
			if ( m_ParaSpinId.IsVertical )
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
BOOL8	Cls_GrP3DoSpinId::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcValIncrease();
		ReqDraw();
		LcNotifyValChg();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinId::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcValDecrease();
		ReqDraw();
		LcNotifyValChg();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinId::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		// value patch
		if ( m_Value >= m_UserIdCnt )
		{
			m_Value	=	m_UserIdCnt;
			if ( 0 != m_Value )
			{
				m_Value --;
			}
		}
		// update status
		Tv_Stat	=	E_GrP3DoSpinIdStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP3DoSpinIdStatNormal;
			if ( m_IsCtrlMode ||  m_IsOnMouse )
			{
				Tv_Stat	=	E_GrP3DoSpinIdStatControl;
			}
			else
			{
				if ( IsFocused() )
				{
					Tv_Stat	=	E_GrP3DoSpinIdStatFocus;
					if ( m_ParaSpinId.IsOnKeyCtrl )
					{
						Tv_Stat	=	E_GrP3DoSpinIdStatControl;
					}
				}
			}
		}
		PaintScriptPush( m_ParaSpinId.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------

