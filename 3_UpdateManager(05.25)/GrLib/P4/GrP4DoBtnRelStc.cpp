/*
		platform button static

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoBtnRelStc.h>
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

#define E_GrP4DoBtnRelStcParaCnt		12

St_GrP4DvlDoPrInfoData	V_GrP4DoBtnRelStcParaInfo[E_GrP4DoBtnRelStcParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnRelStcPara)0)->PntStat[E_GrP4DoBtnRelStcStatDisable],  NULL,	L"Paint disable status"	},
	{	L"Paint Normal",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnRelStcPara)0)->PntStat[E_GrP4DoBtnRelStcStatNormal],	 NULL,L"Paint normal status"	},
	{	L"Paint Focus",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnRelStcPara)0)->PntStat[E_GrP4DoBtnRelStcStatFocus],	 NULL,L"Paint focus status"	},
	{	L"Paint Select",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnRelStcPara)0)->PntStat[E_GrP4DoBtnRelStcStatSelect],	 NULL,L"Paint select status"	},
	{	L"Paint Fcs Sel",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnRelStcPara)0)->PntStat[E_GrP4DoBtnRelStcStatFcsSel],	 NULL,L"Paint focus select status"	},
	{	L"Paint Push",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnRelStcPara)0)->PntStat[E_GrP4DoBtnRelStcStatPush],	 NULL,L"Paint push status"	},
	{	L"Asm Btn Down",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoBtnRelStcPara)0)->AsmBtnDown,	 NULL,L"assembly button down event code"	},
	{	L"Asm Btn Up",				TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoBtnRelStcPara)0)->AsmBtnUp,	 NULL,	L"assembly button up event code"	},
	{	L"EvtHostId",					TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoBtnRelStcPara)0)->EvtHostId,NULL,	L"Notify push event to target object"	},
	{	L"TxtId",							TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoBtnRelStcPara)0)->TxtId,NULL,	L"Text Script Id"	},
	{	L"Value",							TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4DoBtnRelStcPara)0)->Value,NULL,	L"Relate Value"	},
	{	L"RelType",						TRUE,		E_GrP4DvlParaTypeRelUpi,				1,	(void*)&((Ptr_GrP4DoBtnRelStcPara)0)->RelType,NULL,	L"Relate Upi Type"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoBtnRelStc::Cls_GrP4DoBtnRelStc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassBtnRelStc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaBtnRelStc) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		m_IsPush		=	FALSE;
		m_IsSel			=	FALSE;

		GrDumyZeroMem( &m_ParaBtnRelStc, sizeof(m_ParaBtnRelStc) );
		//m_ParaBtnRelStc.PntBg	=	E_GrP4ScptPstIdDflt;

		m_Value			=	m_ParaBtnRelStc.Value;

}
//--------------------------------------------------------------------
Cls_GrP4DoBtnRelStc::~Cls_GrP4DoBtnRelStc( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoBtnRelStc::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassBtnStc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaBtnRelStc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaBtnRelStc, sizeof(m_ParaBtnRelStc) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaBtnRelStc)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnRelStc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaBtnRelStc, sizeof(m_ParaBtnRelStc) , m_PtrScpt , E_GrP4DoClassBtnStc );

		// update
		m_Value	=	m_ParaBtnRelStc.Value;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnRelStc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiEvtHostIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaBtnRelStc.EvtHostId;
				break;
			case E_GrP4UpiEvtHostIdSet:
				m_ParaBtnRelStc.EvtHostId	=	(__u16)A_PtrInt[1];
				break;
			case E_GrP4UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaBtnRelStc.TxtId;
				break;
			case E_GrP4UpiTextIdSet:
				m_ParaBtnRelStc.TxtId	=	(__u16)A_PtrInt[1];
				ReqDraw();
				break;
			case E_GrP4UpiValueGet:
				A_PtrInt[0]	=	(__s32)m_Value;
				break;
			case E_GrP4UpiValueSet:
				m_Value	=	(__u32)A_PtrInt[1];
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnRelStc::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		Tv_Stat	=	E_GrP4DoBtnRelStcStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoBtnRelStcStatPush;
			if ( !m_IsPush )
			{
				if ( m_IsSel )
				{
					Tv_Stat	=	E_GrP4DoBtnRelStcStatSelect;
					if ( m_IsOnMouse || IsFocused() )
					{
						Tv_Stat	=	E_GrP4DoBtnRelStcStatFcsSel;
					}
				}
				else
				{
					Tv_Stat	=	E_GrP4DoBtnRelStcStatNormal;
					if ( m_IsOnMouse || IsFocused() )
					{
						Tv_Stat	=	E_GrP4DoBtnRelStcStatFocus;
					}
				}
			}
		}
		
		PaintScriptPush( m_ParaBtnRelStc.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoBtnRelStc::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaBtnRelStc, V_GrP4DoBtnRelStcParaInfo, E_GrP4DoBtnRelStcParaCnt );
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnRelStc::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4KeyEnter == A_Key )
		{
			m_IsPush	=	TRUE;
			ReqDraw();
			LcDoBtnDownEvt();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnRelStc::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4KeyEnter == A_Key )
		{
			m_IsPush	=	FALSE;
			ReqDraw();
			LcDoBtnUpEvt();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnRelStc::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	TRUE;
		Cls_GrP4DoBase::ProcEvtMseLbtnDown( A_X, A_Y, A_BtnMap );
		ReqDraw();
		LcDoBtnDownEvt();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnRelStc::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP4DoBase::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		ReqDraw();
		LcDoBtnUpEvt();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnRelStc::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnRelStc::LcDoBtnDownEvt( void )
{
	// local -------------------
		St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		// do push assembly
		V_GrP4Mng->VmRunCode( m_ParaBtnRelStc.AsmBtnDown, this );

		if ( (E_GrP4ScptIdNone != m_ParaBtnRelStc.EvtHostId) && (E_GrP4RelUpiNone == m_ParaBtnRelStc.RelType) )
		{
			Tv_Msg.Hd.Cmd		=	E_GrP4MsgCmdBtnDown;
			Tv_Msg.Hd.ConIdx	=	E_GrP4ConIdxFocus;
			Tv_Msg.Hd.WinId	=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId	=	E_GrP4ScptIdNone;
			if ( E_GrP4ScptIdSlaveBase <= m_ParaBtnRelStc.EvtHostId )
			{
				Tv_Msg.Hd.ObjId	=	m_ParaBtnRelStc.EvtHostId;
			}
			Tv_Msg.ObjId		=	m_ScptId;
			((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnRelStc::LcDoBtnUpEvt( void )
{
	// local -------------------
		St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		// do up assembly
		V_GrP4Mng->VmRunCode( m_ParaBtnRelStc.AsmBtnUp, this );

		// do notify to window
		if ( (E_GrP4ScptIdNone != m_ParaBtnRelStc.EvtHostId) && (E_GrP4RelUpiNone == m_ParaBtnRelStc.RelType) )
		{
			Tv_Msg.Hd.Cmd		=	E_GrP4MsgCmdBtnUp;
			Tv_Msg.Hd.ConIdx	=	E_GrP4ConIdxFocus;
			Tv_Msg.Hd.WinId	=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId	=	E_GrP4ScptIdNone;
			if ( E_GrP4ScptIdSlaveBase <= m_ParaBtnRelStc.EvtHostId )
			{
				Tv_Msg.Hd.ObjId	=	m_ParaBtnRelStc.EvtHostId;
			}
			Tv_Msg.ObjId		=	m_ScptId;
			((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoBtnRelStc::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;

		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaBtnRelStc.TxtId );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnRelStc::LcSelStatUpdt( void )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
		__u32	Tv_Val;
		BOOL8	Tv_IsChk;
		__s32		Tv_Rval[2];
	// code --------------------
		Tv_IsChk	=	FALSE;
		if ( E_GrP4ScptIdNone != m_ParaBtnRelStc.EvtHostId )
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaBtnRelStc.EvtHostId );
			if ( NULL != Tv_Obj )
			{
				Tv_IsChk	=	TRUE;
				switch ( m_ParaBtnRelStc.RelType )
				{
					case E_GrP4RelUpiValue:
						Tv_Obj->RtlCommand( E_GrP4UpiValueGet, Tv_Rval, NULL );
						Tv_Val	=	(__u32)Tv_Rval[0];
						break;
					case E_GrP4RelUpiRecType:
						Tv_Obj->RtlCommand( E_GrP4UpiRecTypeGet, Tv_Rval, NULL );
						Tv_Val	=	(__u32)Tv_Rval[0];
						break;
					case E_GrP4RelUpiCh:
						Tv_Obj->RtlCommand( E_GrP4UpiChGet, Tv_Rval, NULL );
						Tv_Val	=	(__u32)Tv_Rval[0];
						break;
					default:
						Tv_IsChk	=	FALSE;
						break;
				}
				// check able
				if ( Tv_IsChk )
				{
					// do compare
					if ( m_Value == Tv_Val )
					{
						m_IsSel	=	TRUE;
					}
					else
					{
						m_IsSel	=	FALSE;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnRelStc::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		BOOL8	Tv_IsSel;
	// code --------------------
		if ( (E_GrP4ScptIdNone != m_ParaBtnRelStc.EvtHostId) && (E_GrP4RelUpiNone != m_ParaBtnRelStc.RelType) )
		{
			Tv_IsSel	=	m_IsSel;
			LcSelStatUpdt();
			if ( Tv_IsSel != m_IsSel )
			{
				ReqDraw();
			}
		}
}
//--------------------------------------------------------------------
