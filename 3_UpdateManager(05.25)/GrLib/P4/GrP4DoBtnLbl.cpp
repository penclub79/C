/*
		platform label

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P4/GrP4DoBtnLbl.h>
#include <GrDebug.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <P4/GrP4Script.h>
#include <P4/GrP4ConBase.h>
#include <P4/GrP4AsmBase.h>
#include <P4/GrP4Mng.h>
#include <P4/GrP4UiCmd.h>

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

#define E_GrP4DoBtnLblParaCnt		7

St_GrP4DvlDoPrInfoData	V_GrP4DoBtnLblParaInfo[E_GrP4DoBtnLblParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnLblPara)0)->PntStat[E_GrP4DoBtnLblStatDisable],  NULL,	L"Paint disable status"	},
	{	L"Paint Normal",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnLblPara)0)->PntStat[E_GrP4DoBtnLblStatNormal],	 NULL,L"Paint normal status"	},
	{	L"Paint Focus",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnLblPara)0)->PntStat[E_GrP4DoBtnLblStatFocus],	 NULL,L"Paint focus status"	},
	{	L"Paint Push",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnLblPara)0)->PntStat[E_GrP4DoBtnLblStatPush],	 NULL,L"Paint push status"	},
	{	L"Asm Btn Down",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoBtnLblPara)0)->AsmBtnDown,	 NULL,L"assembly button down event code"	},
	{	L"Asm Btn Up",				TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoBtnLblPara)0)->AsmBtnUp,	 NULL,	L"assembly button up event code"	},
	{	L"EvtHostId",					TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoBtnLblPara)0)->EvtHostId,NULL,	L"Notify event to target object"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoBtnLbl::Cls_GrP4DoBtnLbl(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassBtnLbl;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoBtnLbl) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		m_IsPush		=	FALSE;

		GrDumyZeroMem( &m_ParaDoBtnLbl, sizeof(m_ParaDoBtnLbl) );
		//m_ParaDoBtnLbl.PntBg	=	E_GrP4ScptPstIdDflt;

		GrStrClear(m_StrTxt);

}
//--------------------------------------------------------------------
Cls_GrP4DoBtnLbl::~Cls_GrP4DoBtnLbl( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoBtnLbl::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassBtnLbl;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoBtnLbl);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoBtnLbl, sizeof(m_ParaDoBtnLbl) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoBtnLbl)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnLbl::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoBtnLbl, sizeof(m_ParaDoBtnLbl) , m_PtrScpt , E_GrP4DoClassBtnLbl );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnLbl::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Tidx;
		WCHAR*	Tv_StrWk;
	// code --------------------
		Tv_Result	=	TRUE;
		switch (A_Cmd)
		{
			case E_GrP4UpiTextGet:
				Tv_Tidx	=	A_PtrInt[1];
				if ((0 <= Tv_Tidx) && (E_GrP4AsmMaxTxtRegCnt > Tv_Tidx))
				{
					GrStrWcopy(A_PtrStr[Tv_Tidx], m_StrTxt);
				}
				else
				{
					DbgMsgPrint("Cls_GrP4DoBtnLbl::RtlCommand - [UPI:TextGet] text register index over.\n");
				}
				break;
			case E_GrP4UpiTextSet:
				Tv_Tidx	=	A_PtrInt[1];
				if ((0 <= Tv_Tidx) && (E_GrP4AsmMaxTxtRegCnt > Tv_Tidx))
				{
					GrStrWcopy(m_StrTxt, A_PtrStr[Tv_Tidx], E_GrP4DoBtnLblStrMaxLen  - 1);
				}
				else
				{
					DbgMsgPrint("Cls_GrP4DoBtnLbl::RtlCommand - [UPI:TextSet] text register index over.\n");
				}
				break;
			case E_GrP4UpiTextIdSet:
				Tv_StrWk	=	V_GrP4Mng->TextPtrGet((__u16)A_PtrInt[1]);
				if (NULL != Tv_StrWk)
				{
					GrStrWcopy(m_StrTxt, Tv_StrWk, E_GrP4DoBtnLblStrMaxLen  - 1);
				}
				break;
			default:
					Tv_Result	=	Cls_GrP4DoBase::RtlCommand(A_Cmd, A_PtrInt, A_PtrStr);
					break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnLbl::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		Tv_Stat	=	E_GrP4DoBtnLblStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoBtnLblStatPush;
			if ( !m_IsPush )
			{
				Tv_Stat	=	E_GrP4DoBtnLblStatNormal;
				if ( m_IsOnMouse || IsFocused() )
				{
					Tv_Stat	=	E_GrP4DoBtnLblStatFocus;
				}
			}
		}
		
		PaintScriptPush( m_ParaDoBtnLbl.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoBtnLbl::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoBtnLbl, V_GrP4DoBtnLblParaInfo, E_GrP4DoBtnLblParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnLbl::TextSet( WCHAR* A_Str )
{
	// local -------------------
	// code --------------------
		GrStrWcopy( m_StrTxt, A_Str, GrStrMaxLen(m_StrTxt) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnLbl::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
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
BOOL8	Cls_GrP4DoBtnLbl::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
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
BOOL8	Cls_GrP4DoBtnLbl::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
BOOL8	Cls_GrP4DoBtnLbl::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
void	Cls_GrP4DoBtnLbl::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnLbl::LcDoBtnDownEvt( void )
{
	// local -------------------
		St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		if ( V_GrP4Mng->IsUiEventAble() )
		{
			// do push assembly
			V_GrP4Mng->VmRunCode( m_ParaDoBtnLbl.AsmBtnDown, this );
			// do notify to window
			if ( E_GrP4ScptIdNone != m_ParaDoBtnLbl.EvtHostId )
			{
				Tv_Msg.Hd.Cmd		=	E_GrP4MsgCmdBtnDown;
				Tv_Msg.Hd.ConIdx	=	E_GrP4ConIdxFocus;
				Tv_Msg.Hd.WinId	=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
				Tv_Msg.Hd.ObjId	=	E_GrP4ScptIdNone;
				if ( E_GrP4ScptIdSlaveBase <= m_ParaDoBtnLbl.EvtHostId )
				{
					Tv_Msg.Hd.ObjId	=	m_ParaDoBtnLbl.EvtHostId;
				}
				Tv_Msg.ObjId		=	m_ScptId;
				((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnLbl::LcDoBtnUpEvt( void )
{
	// local -------------------
		St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		if ( V_GrP4Mng->IsUiEventAble() )
		{
			// run event assembly
			V_GrP4Mng->VmRunCode( m_ParaDoBtnLbl.AsmBtnUp, this );

			// do notify to window
			if ( E_GrP4ScptIdNone != m_ParaDoBtnLbl.EvtHostId )
			{
				Tv_Msg.Hd.Cmd		=	E_GrP4MsgCmdBtnUp;
				Tv_Msg.Hd.ConIdx	=	E_GrP4ConIdxFocus;
				Tv_Msg.Hd.WinId	=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
				Tv_Msg.Hd.ObjId	=	E_GrP4ScptIdNone;
				if ( E_GrP4ScptIdSlaveBase <= m_ParaDoBtnLbl.EvtHostId )
				{
					Tv_Msg.Hd.ObjId	=	m_ParaDoBtnLbl.EvtHostId;
				}
				Tv_Msg.ObjId		=	m_ScptId;
				((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
			}
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoBtnLbl::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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


