/*
		platform button static

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoBtnRelStc.h>
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

#define E_GrP3DoBtnRelStcParaCnt		12

St_GrP3DvlDoPrInfoData	V_GrP3DoBtnRelStcParaInfo[E_GrP3DoBtnRelStcParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnRelStcPara)0)->PntStat[E_GrP3DoBtnRelStcStatDisable],  NULL,	L"Paint disable status"	},
	{	L"Paint Normal",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnRelStcPara)0)->PntStat[E_GrP3DoBtnRelStcStatNormal],	 NULL,L"Paint normal status"	},
	{	L"Paint Focus",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnRelStcPara)0)->PntStat[E_GrP3DoBtnRelStcStatFocus],	 NULL,L"Paint focus status"	},
	{	L"Paint Select",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnRelStcPara)0)->PntStat[E_GrP3DoBtnRelStcStatSelect],	 NULL,L"Paint select status"	},
	{	L"Paint Fcs Sel",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnRelStcPara)0)->PntStat[E_GrP3DoBtnRelStcStatFcsSel],	 NULL,L"Paint focus select status"	},
	{	L"Paint Push",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnRelStcPara)0)->PntStat[E_GrP3DoBtnRelStcStatPush],	 NULL,L"Paint push status"	},
	{	L"Asm Btn Down",			TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoBtnRelStcPara)0)->AsmBtnDown,	 NULL,L"assembly button down event code"	},
	{	L"Asm Btn Up",				TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoBtnRelStcPara)0)->AsmBtnUp,	 NULL,	L"assembly button up event code"	},
	{	L"EvtHostId",					TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoBtnRelStcPara)0)->EvtHostId,NULL,	L"Notify push event to target object"	},
	{	L"TxtId",							TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoBtnRelStcPara)0)->TxtId,NULL,	L"Text Script Id"	},
	{	L"Value",							TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoBtnRelStcPara)0)->Value,NULL,	L"Relate Value"	},
	{	L"RelType",						TRUE,		E_GrP3DvlParaTypeRelUpi,				1,	(void*)&((Ptr_GrP3DoBtnRelStcPara)0)->RelType,NULL,	L"Relate Upi Type"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoBtnRelStc::Cls_GrP3DoBtnRelStc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassBtnRelStc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaBtnRelStc) + sizeof(St_GrP3ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		m_IsPush		=	FALSE;
		m_IsSel			=	FALSE;

		GrDumyZeroMem( &m_ParaBtnRelStc, sizeof(m_ParaBtnRelStc) );
		//m_ParaBtnRelStc.PntBg	=	E_GrP3ScptPstIdDflt;

		m_Value			=	m_ParaBtnRelStc.Value;

}
//--------------------------------------------------------------------
Cls_GrP3DoBtnRelStc::~Cls_GrP3DoBtnRelStc( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnRelStc::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassBtnStc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaBtnRelStc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaBtnRelStc, sizeof(m_ParaBtnRelStc) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaBtnRelStc)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnRelStc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaBtnRelStc, sizeof(m_ParaBtnRelStc) , m_PtrScpt , E_GrP3DoClassBtnStc );

		// update
		m_Value	=	m_ParaBtnRelStc.Value;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnRelStc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiEvtHostIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaBtnRelStc.EvtHostId;
				break;
			case E_GrP3UpiEvtHostIdSet:
				m_ParaBtnRelStc.EvtHostId	=	(__u16)A_PtrInt[1];
				break;
			case E_GrP3UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaBtnRelStc.TxtId;
				break;
			case E_GrP3UpiTextIdSet:
				m_ParaBtnRelStc.TxtId	=	A_PtrInt[1];
				ReqDraw();
				break;
			case E_GrP3UpiValueGet:
				A_PtrInt[0]	=	(__s32)m_Value;
				break;
			case E_GrP3UpiValueSet:
				m_Value	=	(__u32)A_PtrInt[1];
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnRelStc::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		Tv_Stat	=	E_GrP3DoBtnRelStcStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP3DoBtnRelStcStatPush;
			if ( !m_IsPush )
			{
				if ( m_IsSel )
				{
					Tv_Stat	=	E_GrP3DoBtnRelStcStatSelect;
					if ( m_IsOnMouse || IsFocused() )
					{
						Tv_Stat	=	E_GrP3DoBtnRelStcStatFcsSel;
					}
				}
				else
				{
					Tv_Stat	=	E_GrP3DoBtnRelStcStatNormal;
					if ( m_IsOnMouse || IsFocused() )
					{
						Tv_Stat	=	E_GrP3DoBtnRelStcStatFocus;
					}
				}
			}
		}
		
		PaintScriptPush( m_ParaBtnRelStc.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnRelStc::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaBtnRelStc, V_GrP3DoBtnRelStcParaInfo, E_GrP3DoBtnRelStcParaCnt );
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnRelStc::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3KeyEnter == A_Key )
		{
			m_IsPush	=	TRUE;
			ReqDraw();
			LcDoBtnDownEvt();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnRelStc::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3KeyEnter == A_Key )
		{
			m_IsPush	=	FALSE;
			ReqDraw();
			LcDoBtnUpEvt();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnRelStc::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	TRUE;
		Cls_GrP3DoBase::ProcEvtMseLbtnDown( A_X, A_Y, A_BtnMap );
		ReqDraw();
		LcDoBtnDownEvt();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnRelStc::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP3DoBase::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		ReqDraw();
		LcDoBtnUpEvt();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnRelStc::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnRelStc::LcDoBtnDownEvt( void )
{
	// local -------------------
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		// do push assembly
		V_GrP3Mng->VmRunCode( m_ParaBtnRelStc.AsmBtnDown, this );

		if ( (E_GrP3ScptIdNone != m_ParaBtnRelStc.EvtHostId) && (E_GrP3RelUpiNone == m_ParaBtnRelStc.RelType) )
		{
			Tv_Msg.Hd.Cmd		=	E_GrP3MsgCmdBtnDown;
			Tv_Msg.Hd.ConIdx	=	E_GrP3ConIdxFocus;
			Tv_Msg.Hd.WinId	=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId	=	E_GrP3ScptIdNone;
			if ( E_GrP3ScptIdSlaveBase <= m_ParaBtnRelStc.EvtHostId )
			{
				Tv_Msg.Hd.ObjId	=	m_ParaBtnRelStc.EvtHostId;
			}
			Tv_Msg.ObjId		=	m_ScptId;
			((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnRelStc::LcDoBtnUpEvt( void )
{
	// local -------------------
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		// do up assembly
		V_GrP3Mng->VmRunCode( m_ParaBtnRelStc.AsmBtnUp, this );

		// do notify to window
		if ( (E_GrP3ScptIdNone != m_ParaBtnRelStc.EvtHostId) && (E_GrP3RelUpiNone == m_ParaBtnRelStc.RelType) )
		{
			Tv_Msg.Hd.Cmd		=	E_GrP3MsgCmdBtnUp;
			Tv_Msg.Hd.ConIdx	=	E_GrP3ConIdxFocus;
			Tv_Msg.Hd.WinId	=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId	=	E_GrP3ScptIdNone;
			if ( E_GrP3ScptIdSlaveBase <= m_ParaBtnRelStc.EvtHostId )
			{
				Tv_Msg.Hd.ObjId	=	m_ParaBtnRelStc.EvtHostId;
			}
			Tv_Msg.ObjId		=	m_ScptId;
			((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoBtnRelStc::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;

		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaBtnRelStc.TxtId );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnRelStc::LcSelStatUpdt( void )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
		__u32	Tv_Val;
		BOOL8	Tv_IsChk;
		__s32		Tv_Rval[2];
	// code --------------------
		Tv_IsChk	=	FALSE;
		if ( E_GrP3ScptIdNone != m_ParaBtnRelStc.EvtHostId )
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaBtnRelStc.EvtHostId );
			if ( NULL != Tv_Obj )
			{
				Tv_IsChk	=	TRUE;
				switch ( m_ParaBtnRelStc.RelType )
				{
					case E_GrP3RelUpiValue:
						Tv_Obj->RtlCommand( E_GrP3UpiValueGet, Tv_Rval, NULL );
						Tv_Val	=	(__u32)Tv_Rval[0];
						break;
					case E_GrP3RelUpiRecType:
						Tv_Obj->RtlCommand( E_GrP3UpiRecTypeGet, Tv_Rval, NULL );
						Tv_Val	=	(__u32)Tv_Rval[0];
						break;
					case E_GrP3RelUpiCh:
						Tv_Obj->RtlCommand( E_GrP3UpiChGet, Tv_Rval, NULL );
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
void	Cls_GrP3DoBtnRelStc::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		BOOL8	Tv_IsSel;
	// code --------------------
		if ( (E_GrP3ScptIdNone != m_ParaBtnRelStc.EvtHostId) && (E_GrP3RelUpiNone != m_ParaBtnRelStc.RelType) )
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
