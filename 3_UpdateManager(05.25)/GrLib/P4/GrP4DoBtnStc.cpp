/*
		platform button static

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoBtnStc.h>
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

#define E_GrP4DoBtnStcParaCnt		8

St_GrP4DvlDoPrInfoData	V_GrP4DoBtnStcParaInfo[E_GrP4DoBtnStcParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnStcPara)0)->PntStat[E_GrP4DoBtnStcStatDisable],  NULL,	L"Paint disable status"	},
	{	L"Paint Normal",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnStcPara)0)->PntStat[E_GrP4DoBtnStcStatNormal],	 NULL,L"Paint normal status"	},
	{	L"Paint Focus",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnStcPara)0)->PntStat[E_GrP4DoBtnStcStatFocus],	 NULL,L"Paint focus status"	},
	{	L"Paint Push",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnStcPara)0)->PntStat[E_GrP4DoBtnStcStatPush],	 NULL,L"Paint push status"	},
	{	L"Asm Btn Down",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoBtnStcPara)0)->AsmBtnDown,	 NULL,L"assembly button down event code"	},
	{	L"Asm Btn Up",				TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoBtnStcPara)0)->AsmBtnUp,	 NULL,	L"assembly button up event code"	},
	{	L"EvtHostId",					TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoBtnStcPara)0)->EvtHostId,NULL,	L"Notify push event to target object"	},
	{	L"TxtId",							TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoBtnStcPara)0)->TxtId,NULL,	L"Text Script Id"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoBtnStc::Cls_GrP4DoBtnStc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassBtnStc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoBtnStc) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		m_IsPush		=	FALSE;

		GrDumyZeroMem( &m_ParaDoBtnStc, sizeof(m_ParaDoBtnStc) );
		//m_ParaDoBtnStc.PntBg	=	E_GrP4ScptPstIdDflt;

}
//--------------------------------------------------------------------
Cls_GrP4DoBtnStc::~Cls_GrP4DoBtnStc( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoBtnStc::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoBtnStc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoBtnStc, sizeof(m_ParaDoBtnStc) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoBtnStc)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnStc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoBtnStc, sizeof(m_ParaDoBtnStc) , m_PtrScpt , E_GrP4DoClassBtnStc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnStc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiEvtHostIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaDoBtnStc.EvtHostId;
				break;
			case E_GrP4UpiEvtHostIdSet:
				m_ParaDoBtnStc.EvtHostId	=	(__u16)A_PtrInt[1];
				break;
			case E_GrP4UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaDoBtnStc.TxtId;
				break;
			case E_GrP4UpiTextIdSet:
				m_ParaDoBtnStc.TxtId	=	(__u16)A_PtrInt[1];
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnStc::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		Tv_Stat	=	E_GrP4DoBtnStcStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoBtnStcStatPush;
			if ( !m_IsPush )
			{
				Tv_Stat	=	E_GrP4DoBtnStcStatNormal;
				if ( m_IsOnMouse || IsFocused() )
				{
					Tv_Stat	=	E_GrP4DoBtnStcStatFocus;
				}
			}
		}
		
		PaintScriptPush( m_ParaDoBtnStc.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoBtnStc::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoBtnStc, V_GrP4DoBtnStcParaInfo, E_GrP4DoBtnStcParaCnt );
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnStc::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
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
BOOL8	Cls_GrP4DoBtnStc::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4KeyEnter == A_Key )
		{
			m_IsPush	=	FALSE;
			ReqDraw();
			LcOdBtnUpEvt();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnStc::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
BOOL8	Cls_GrP4DoBtnStc::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP4DoBase::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		ReqDraw();
		LcOdBtnUpEvt();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnStc::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnStc::LcDoBtnDownEvt( void )
{
	// local -------------------
		St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		// do push assembly
		if (E_GrP4AsmIdNone != m_ParaDoBtnStc.AsmBtnDown)
		{
			V_GrP4Mng->VmRunCode(m_ParaDoBtnStc.AsmBtnDown, this);
		}

		if ( E_GrP4ScptIdNone != m_ParaDoBtnStc.EvtHostId )
		{
			Tv_Msg.Hd.Cmd		=	E_GrP4MsgCmdBtnDown;
			Tv_Msg.Hd.ConIdx	=	E_GrP4ConIdxFocus;
			Tv_Msg.Hd.WinId	=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId	=	E_GrP4ScptIdNone;
			if ( E_GrP4ScptIdSlaveBase <= m_ParaDoBtnStc.EvtHostId )
			{
				Tv_Msg.Hd.ObjId	=	m_ParaDoBtnStc.EvtHostId;
			}
			Tv_Msg.ObjId		=	m_ScptId;
			((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnStc::LcOdBtnUpEvt( void )
{
	// local -------------------
		St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		// do up assembly
		V_GrP4Mng->VmRunCode( m_ParaDoBtnStc.AsmBtnUp, this );

		// do notify to window
		if ( E_GrP4ScptIdNone != m_ParaDoBtnStc.EvtHostId )
		{
			Tv_Msg.Hd.Cmd		=	E_GrP4MsgCmdBtnUp;
			Tv_Msg.Hd.ConIdx	=	E_GrP4ConIdxFocus;
			Tv_Msg.Hd.WinId	=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId	=	E_GrP4ScptIdNone;
			if ( E_GrP4ScptIdSlaveBase <= m_ParaDoBtnStc.EvtHostId )
			{
				Tv_Msg.Hd.ObjId	=	m_ParaDoBtnStc.EvtHostId;
			}
			Tv_Msg.ObjId		=	m_ScptId;
			((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoBtnStc::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;

		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaDoBtnStc.TxtId );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
