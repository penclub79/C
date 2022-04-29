/*
		platform button static

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoBtnStc.h>
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

#define E_GrP3DoBtnStcParaCnt		8

St_GrP3DvlDoPrInfoData	V_GrP3DoBtnStcParaInfo[E_GrP3DoBtnStcParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnStcPara)0)->PntStat[E_GrP3DoBtnStcStatDisable],  NULL,	L"Paint disable status"	},
	{	L"Paint Normal",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnStcPara)0)->PntStat[E_GrP3DoBtnStcStatNormal],	 NULL,L"Paint normal status"	},
	{	L"Paint Focus",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnStcPara)0)->PntStat[E_GrP3DoBtnStcStatFocus],	 NULL,L"Paint focus status"	},
	{	L"Paint Push",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnStcPara)0)->PntStat[E_GrP3DoBtnStcStatPush],	 NULL,L"Paint push status"	},
	{	L"Asm Btn Down",			TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoBtnStcPara)0)->AsmBtnDown,	 NULL,L"assembly button down event code"	},
	{	L"Asm Btn Up",				TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoBtnStcPara)0)->AsmBtnUp,	 NULL,	L"assembly button up event code"	},
	{	L"EvtHostId",					TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoBtnStcPara)0)->EvtHostId,NULL,	L"Notify push event to target object"	},
	{	L"TxtId",							TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoBtnStcPara)0)->TxtId,NULL,	L"Text Script Id"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoBtnStc::Cls_GrP3DoBtnStc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassBtnStc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoBtnStc) + sizeof(St_GrP3ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		m_IsPush		=	FALSE;

		GrDumyZeroMem( &m_ParaDoBtnStc, sizeof(m_ParaDoBtnStc) );
		//m_ParaDoBtnStc.PntBg	=	E_GrP3ScptPstIdDflt;

}
//--------------------------------------------------------------------
Cls_GrP3DoBtnStc::~Cls_GrP3DoBtnStc( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnStc::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoBtnStc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoBtnStc, sizeof(m_ParaDoBtnStc) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoBtnStc)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnStc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoBtnStc, sizeof(m_ParaDoBtnStc) , m_PtrScpt , E_GrP3DoClassBtnStc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnStc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiEvtHostIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaDoBtnStc.EvtHostId;
				break;
			case E_GrP3UpiEvtHostIdSet:
				m_ParaDoBtnStc.EvtHostId	=	(__u16)A_PtrInt[1];
				break;
			case E_GrP3UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaDoBtnStc.TxtId;
				break;
			case E_GrP3UpiTextIdSet:
				m_ParaDoBtnStc.TxtId	=	A_PtrInt[1];
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnStc::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		Tv_Stat	=	E_GrP3DoBtnStcStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP3DoBtnStcStatPush;
			if ( !m_IsPush )
			{
				Tv_Stat	=	E_GrP3DoBtnStcStatNormal;
				if ( m_IsOnMouse || IsFocused() )
				{
					Tv_Stat	=	E_GrP3DoBtnStcStatFocus;
				}
			}
		}
		
		PaintScriptPush( m_ParaDoBtnStc.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnStc::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoBtnStc, V_GrP3DoBtnStcParaInfo, E_GrP3DoBtnStcParaCnt );
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnStc::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
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
BOOL8	Cls_GrP3DoBtnStc::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3KeyEnter == A_Key )
		{
			m_IsPush	=	FALSE;
			ReqDraw();
			LcOdBtnUpEvt();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnStc::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
BOOL8	Cls_GrP3DoBtnStc::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP3DoBase::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		ReqDraw();
		LcOdBtnUpEvt();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnStc::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnStc::LcDoBtnDownEvt( void )
{
	// local -------------------
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		// do push assembly
		V_GrP3Mng->VmRunCode( m_ParaDoBtnStc.AsmBtnDown, this );

		if ( E_GrP3ScptIdNone != m_ParaDoBtnStc.EvtHostId )
		{
			Tv_Msg.Hd.Cmd		=	E_GrP3MsgCmdBtnDown;
			Tv_Msg.Hd.ConIdx	=	E_GrP3ConIdxFocus;
			Tv_Msg.Hd.WinId	=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId	=	E_GrP3ScptIdNone;
			if ( E_GrP3ScptIdSlaveBase <= m_ParaDoBtnStc.EvtHostId )
			{
				Tv_Msg.Hd.ObjId	=	m_ParaDoBtnStc.EvtHostId;
			}
			Tv_Msg.ObjId		=	m_ScptId;
			((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnStc::LcOdBtnUpEvt( void )
{
	// local -------------------
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		// do up assembly
		V_GrP3Mng->VmRunCode( m_ParaDoBtnStc.AsmBtnUp, this );

		// do notify to window
		if ( E_GrP3ScptIdNone != m_ParaDoBtnStc.EvtHostId )
		{
			Tv_Msg.Hd.Cmd		=	E_GrP3MsgCmdBtnUp;
			Tv_Msg.Hd.ConIdx	=	E_GrP3ConIdxFocus;
			Tv_Msg.Hd.WinId	=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId	=	E_GrP3ScptIdNone;
			if ( E_GrP3ScptIdSlaveBase <= m_ParaDoBtnStc.EvtHostId )
			{
				Tv_Msg.Hd.ObjId	=	m_ParaDoBtnStc.EvtHostId;
			}
			Tv_Msg.ObjId		=	m_ScptId;
			((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoBtnStc::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;

		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaDoBtnStc.TxtId );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
