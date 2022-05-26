/*
		platform button page

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoBtnPage.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>
#include	<GrDebug.h>
#include	<P4/GrP4DoFrame.h>

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

#define E_GrP4DoBtnPageParaCnt		12

St_GrP4DvlDoPrInfoData	V_GrP4DoBtnPageParaInfo[E_GrP4DoBtnPageParaCnt]	=	
{
	{	L"Disable Paint",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnPagePara)0)->PntStatTbl[E_GrP4DoBtnPageStatDisable],NULL,				L"Paint disable status"	},
	{	L"Normal Paint",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnPagePara)0)->PntStatTbl[E_GrP4DoBtnPageStatNormal],NULL,				L"Paint normal status"	},
	{	L"OnCus Paint",					TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnPagePara)0)->PntStatTbl[E_GrP4DoBtnPageStatOnCus],NULL,				L"Paint on cursor status"	},
	{	L"Select Paint",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnPagePara)0)->PntStatTbl[E_GrP4DoBtnPageStatSelect],NULL,				L"Paint selected status"	},
	{	L"SelCus Paint",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnPagePara)0)->PntStatTbl[E_GrP4DoBtnPageStatSelCus],NULL,				L"Paint selected and on cursor status"	},
	{	L"Push Paint",					TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoBtnPagePara)0)->PntStatTbl[E_GrP4DoBtnPageStatPush],NULL,				L"Paint push status"	},
	{	L"IsUpLayer",						TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoBtnPagePara)0)->IsUpLayer,NULL,				L"go to upper frame layer, Don't use frame id and Frame layer when this parametter trun on"	},
	{	L"FrameId",							TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoBtnPagePara)0)->FrameId,NULL,				L"relate frame script id"	},
	{	L"PageId",							TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoBtnPagePara)0)->PageId,NULL,				L"target page id"	},
	{	L"FrameLayer",					TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoBtnPagePara)0)->FrmLayer,NULL,				L"target frame layer"	},
	{	L"IsMovFcs",						TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoBtnPagePara)0)->IsMovFcs,NULL,				L"move focus to frame when after change page"	},
	{	L"TxtId",								TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoBtnPagePara)0)->TxtId,NULL,				L"display text id"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoBtnPage::Cls_GrP4DoBtnPage(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassBtnPage;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoBtnPage) + sizeof(St_GrP4ScptParaHd);

		m_IsKeyAble	=	TRUE;		// key able
		m_IsMouseAble	=	TRUE;
		
		GrDumyZeroMem( &m_ParaDoBtnPage, sizeof(m_ParaDoBtnPage) );

		m_IsSelect	=	FALSE;
		m_IsPush		=	FALSE;

}
//--------------------------------------------------------------------
Cls_GrP4DoBtnPage::~Cls_GrP4DoBtnPage( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoBtnPage::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassBtnPage;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoBtnPage);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoBtnPage, sizeof(m_ParaDoBtnPage) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoBtnPage)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnPage::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoBtnPage, sizeof(m_ParaDoBtnPage) , m_PtrScpt , E_GrP4DoClassBtnPage );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnPage::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnPage::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		// get status
		Tv_Stat	=	E_GrP4DoBtnPageStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoBtnPageStatPush;
			if ( !m_IsPush )
			{
				Tv_Stat	=	E_GrP4DoBtnPageStatNormal;
				if ( m_IsSelect )
				{
					Tv_Stat	=	E_GrP4DoBtnPageStatSelect;
				}
				if ( m_IsOnMouse || IsFocused() )
				{
					Tv_Stat ++;
				}
			}
		}
		// do paint
		PaintScriptPush( m_ParaDoBtnPage.PntStatTbl[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoBtnPage::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoBtnPage, V_GrP4DoBtnPageParaInfo, E_GrP4DoBtnPageParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnPage::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlInit();

		LcSelStatUpdt();		// select status update
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnPage::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
	// code --------------------
#ifdef GR_P4_EDITOR
		if ( !((Cls_GrP4DvlMng*)V_GrP4Mng)->DvlIsDbgRun() )
		{
			return;
		}
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnPage::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Key )
		{
			case E_GrP4KeyEnter:
				m_IsPush	=	TRUE;
				ReqDraw();
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnPage::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Key )
		{
			case E_GrP4KeyEnter:
				m_IsPush	=	FALSE;
				ReqDraw();
				// do change
				LcReqFrmPageChg();
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnPage::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtMseLbtnDown( A_X, A_Y, A_BtnMap );
		m_IsPush	=	TRUE;
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoBtnPage::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		Cls_GrP4DoBase::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		m_IsPush	=	FALSE;
		ReqDraw();
		// do change
		LcReqFrmPageChg();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnPage::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnPage::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnPage::LcSelStatUpdt( void )
{
	// local -------------------
		Cls_GrP4DoFrame*	Tv_Frm;
		BOOL8	Tv_IsSel;
	// code --------------------
		Tv_IsSel	=	FALSE;
		if ( m_IsVisible && m_ParaDoBase.IsEnable )
		{
			// find exist control
			if ( (E_GrP4ScptIdNone != m_ParaDoBtnPage.FrameId) && (E_GrP4ScptIdNone != m_ParaDoBtnPage.PageId) )
			{
				Tv_Frm	=	(Cls_GrP4DoFrame*)m_ObjWin->FindSlaveObjByScptId( m_ParaDoBtnPage.FrameId );
				if ( NULL != Tv_Frm )
				{
					if ( E_GrP4DoClassFrame == Tv_Frm->ClassIdGet() )
					{
						// get page id
						if ( m_ParaDoBtnPage.PageId == Tv_Frm->DispPageIdGet() )
						{
							Tv_IsSel	=	TRUE;
						}
					}
#ifdef GR_P4_EDITOR
					else
					{
						DbgMsgPrint( "Cls_GrP4DoBtnPage ( %d ) - incorrect frame type ( %d ).\n", m_ScptId, m_ParaDoBtnPage.FrameId );
					}
#endif
				}
#ifdef GR_P4_EDITOR
				else
				{
					DbgMsgPrint( "Cls_GrP4DoBtnPage ( %d ) - bad frame id ( %d ).\n", m_ScptId, m_ParaDoBtnPage.FrameId );
				}
#endif
			}
		}
		// update select
		m_IsSelect	=	Tv_IsSel;

}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnPage::LcReqFrmPageChg( void )
{
	// local -------------------
		Cls_GrP4DoFrame*	Tv_Frm;
		St_GrP4MsgFrmReqPageChg	Tv_Msg;
	// code --------------------
		// find exist control
		if ( (E_GrP4ScptIdNone != m_ParaDoBtnPage.FrameId) && (E_GrP4ScptIdNone != m_ParaDoBtnPage.PageId) )
		{
			Tv_Frm	=	(Cls_GrP4DoFrame*)m_ObjWin->FindSlaveObjByScptId( m_ParaDoBtnPage.FrameId );
			if ( NULL != Tv_Frm )
			{
				if ( E_GrP4DoClassFrame == Tv_Frm->ClassIdGet() )
				{
					// request page change
					Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdFrameReqPageChg;
					Tv_Msg.Hd.ConIdx	=	E_GrP4ConIdxFocus;
					Tv_Msg.Hd.WinId		=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
					Tv_Msg.Hd.ObjId		=	m_ParaDoBtnPage.FrameId;
					Tv_Msg.PageId			=	m_ParaDoBtnPage.PageId;
					Tv_Msg.Layer			=	m_ParaDoBtnPage.FrmLayer;
					Tv_Msg.IsBack			=	m_ParaDoBtnPage.IsUpLayer;
					Tv_Msg.IsMovFcs		=	m_ParaDoBtnPage.IsMovFcs;
					((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				}
#ifdef GR_P4_EDITOR
				else
				{
					DbgMsgPrint( "Cls_GrP4DoBtnPage ( %d ) - incorrect frame type ( %d ).\n", m_ScptId, m_ParaDoBtnPage.FrameId );
				}
#endif
			}
#ifdef GR_P4_EDITOR
			else
			{
				DbgMsgPrint( "Cls_GrP4DoBtnPage ( %d ) - bad frame id ( %d ).\n", m_ScptId, m_ParaDoBtnPage.FrameId );
			}
#endif
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoBtnPage::ProcEvtFramePageChged( __u16 A_FrmadId, __u16 A_PageId )
{
	// local -------------------
	// code --------------------
		LcSelStatUpdt();
		ReqDraw();
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoBtnPage::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaDoBtnPage.TxtId );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------

