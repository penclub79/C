/*
		platform label

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoBtnLbl.h>
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

#define E_GrP3DoBtnLblParaCnt		7

St_GrP3DvlDoPrInfoData	V_GrP3DoBtnLblParaInfo[E_GrP3DoBtnLblParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnLblPara)0)->PntStat[E_GrP3DoBtnLblStatDisable],  NULL,	L"Paint disable status"	},
	{	L"Paint Normal",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnLblPara)0)->PntStat[E_GrP3DoBtnLblStatNormal],	 NULL,L"Paint normal status"	},
	{	L"Paint Focus",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnLblPara)0)->PntStat[E_GrP3DoBtnLblStatFocus],	 NULL,L"Paint focus status"	},
	{	L"Paint Push",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnLblPara)0)->PntStat[E_GrP3DoBtnLblStatPush],	 NULL,L"Paint push status"	},
	{	L"Asm Btn Down",			TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoBtnLblPara)0)->AsmBtnDown,	 NULL,L"assembly button down event code"	},
	{	L"Asm Btn Up",				TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoBtnLblPara)0)->AsmBtnUp,	 NULL,	L"assembly button up event code"	},
	{	L"EvtHostId",					TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoBtnLblPara)0)->EvtHostId,NULL,	L"Notify event to target object"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoBtnLbl::Cls_GrP3DoBtnLbl( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassBtnLbl;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoBtnLbl) + sizeof(St_GrP3ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		m_IsPush		=	FALSE;

		GrDumyZeroMem( &m_ParaDoBtnLbl, sizeof(m_ParaDoBtnLbl) );
		//m_ParaDoBtnLbl.PntBg	=	E_GrP3ScptPstIdDflt;

		GrStrClear(m_StrTxt);

}
//--------------------------------------------------------------------
Cls_GrP3DoBtnLbl::~Cls_GrP3DoBtnLbl( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnLbl::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassBtnLbl;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoBtnLbl);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoBtnLbl, sizeof(m_ParaDoBtnLbl) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoBtnLbl)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnLbl::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoBtnLbl, sizeof(m_ParaDoBtnLbl) , m_PtrScpt , E_GrP3DoClassBtnLbl );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnLbl::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnLbl::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		Tv_Stat	=	E_GrP3DoBtnLblStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP3DoBtnLblStatPush;
			if ( !m_IsPush )
			{
				Tv_Stat	=	E_GrP3DoBtnLblStatNormal;
				if ( m_IsOnMouse || IsFocused() )
				{
					Tv_Stat	=	E_GrP3DoBtnLblStatFocus;
				}
			}
		}
		
		PaintScriptPush( m_ParaDoBtnLbl.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnLbl::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoBtnLbl, V_GrP3DoBtnLblParaInfo, E_GrP3DoBtnLblParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnLbl::TextSet( WCHAR* A_Str )
{
	// local -------------------
	// code --------------------
		GrStrWcopy( m_StrTxt, A_Str, GrStrMaxLen(m_StrTxt) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnLbl::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
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
BOOL8	Cls_GrP3DoBtnLbl::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
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
BOOL8	Cls_GrP3DoBtnLbl::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
BOOL8	Cls_GrP3DoBtnLbl::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
void	Cls_GrP3DoBtnLbl::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnLbl::LcDoBtnDownEvt( void )
{
	// local -------------------
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		if ( V_GrP3Mng->IsUiEventAble() )
		{
			// do push assembly
			V_GrP3Mng->VmRunCode( m_ParaDoBtnLbl.AsmBtnDown, this );
			// do notify to window
			if ( E_GrP3ScptIdNone != m_ParaDoBtnLbl.EvtHostId )
			{
				Tv_Msg.Hd.Cmd		=	E_GrP3MsgCmdBtnDown;
				Tv_Msg.Hd.ConIdx	=	E_GrP3ConIdxFocus;
				Tv_Msg.Hd.WinId	=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
				Tv_Msg.Hd.ObjId	=	E_GrP3ScptIdNone;
				if ( E_GrP3ScptIdSlaveBase <= m_ParaDoBtnLbl.EvtHostId )
				{
					Tv_Msg.Hd.ObjId	=	m_ParaDoBtnLbl.EvtHostId;
				}
				Tv_Msg.ObjId		=	m_ScptId;
				((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnLbl::LcDoBtnUpEvt( void )
{
	// local -------------------
		St_GrP3MsgObj	Tv_Msg;
	// code --------------------
		if ( V_GrP3Mng->IsUiEventAble() )
		{
			// run event assembly
			V_GrP3Mng->VmRunCode( m_ParaDoBtnLbl.AsmBtnUp, this );

			// do notify to window
			if ( E_GrP3ScptIdNone != m_ParaDoBtnLbl.EvtHostId )
			{
				Tv_Msg.Hd.Cmd		=	E_GrP3MsgCmdBtnUp;
				Tv_Msg.Hd.ConIdx	=	E_GrP3ConIdxFocus;
				Tv_Msg.Hd.WinId	=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
				Tv_Msg.Hd.ObjId	=	E_GrP3ScptIdNone;
				if ( E_GrP3ScptIdSlaveBase <= m_ParaDoBtnLbl.EvtHostId )
				{
					Tv_Msg.Hd.ObjId	=	m_ParaDoBtnLbl.EvtHostId;
				}
				Tv_Msg.ObjId		=	m_ScptId;
				((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
			}
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoBtnLbl::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	m_StrTxt;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------


