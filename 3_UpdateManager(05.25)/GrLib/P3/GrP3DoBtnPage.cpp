/*
		platform button page

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoBtnPage.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>
#include	<GrDebug.h>
#include	<P3/GrP3DoFrame.h>

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

#define E_GrP3DoBtnPageParaCnt		12

St_GrP3DvlDoPrInfoData	V_GrP3DoBtnPageParaInfo[E_GrP3DoBtnPageParaCnt]	=	
{
	{	L"Disable Paint",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnPagePara)0)->PntStatTbl[E_GrP3DoBtnPageStatDisable],NULL,				L"Paint disable status"	},
	{	L"Normal Paint",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnPagePara)0)->PntStatTbl[E_GrP3DoBtnPageStatNormal],NULL,				L"Paint normal status"	},
	{	L"OnCus Paint",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnPagePara)0)->PntStatTbl[E_GrP3DoBtnPageStatOnCus],NULL,				L"Paint on cursor status"	},
	{	L"Select Paint",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnPagePara)0)->PntStatTbl[E_GrP3DoBtnPageStatSelect],NULL,				L"Paint selected status"	},
	{	L"SelCus Paint",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnPagePara)0)->PntStatTbl[E_GrP3DoBtnPageStatSelCus],NULL,				L"Paint selected and on cursor status"	},
	{	L"Push Paint",					TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoBtnPagePara)0)->PntStatTbl[E_GrP3DoBtnPageStatPush],NULL,				L"Paint push status"	},
	{	L"IsUpLayer",						TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoBtnPagePara)0)->IsUpLayer,NULL,				L"go to upper frame layer, Don't use frame id and Frame layer when this parametter trun on"	},
	{	L"FrameId",							TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoBtnPagePara)0)->FrameId,NULL,				L"relate frame script id"	},
	{	L"PageId",							TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoBtnPagePara)0)->PageId,NULL,				L"target page id"	},
	{	L"FrameLayer",					TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoBtnPagePara)0)->FrmLayer,NULL,				L"target frame layer"	},
	{	L"IsMovFcs",						TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoBtnPagePara)0)->IsMovFcs,NULL,				L"move focus to frame when after change page"	},
	{	L"TxtId",								TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoBtnPagePara)0)->TxtId,NULL,				L"display text id"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoBtnPage::Cls_GrP3DoBtnPage( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassBtnPage;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoBtnPage) + sizeof(St_GrP3ScptParaHd);

		m_IsKeyAble	=	TRUE;		// key able
		m_IsMouseAble	=	TRUE;
		
		GrDumyZeroMem( &m_ParaDoBtnPage, sizeof(m_ParaDoBtnPage) );

		m_IsSelect	=	FALSE;
		m_IsPush		=	FALSE;

}
//--------------------------------------------------------------------
Cls_GrP3DoBtnPage::~Cls_GrP3DoBtnPage( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnPage::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassBtnPage;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoBtnPage);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoBtnPage, sizeof(m_ParaDoBtnPage) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoBtnPage)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnPage::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoBtnPage, sizeof(m_ParaDoBtnPage) , m_PtrScpt , E_GrP3DoClassBtnPage );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnPage::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnPage::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		// get status
		Tv_Stat	=	E_GrP3DoBtnPageStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP3DoBtnPageStatPush;
			if ( !m_IsPush )
			{
				Tv_Stat	=	E_GrP3DoBtnPageStatNormal;
				if ( m_IsSelect )
				{
					Tv_Stat	=	E_GrP3DoBtnPageStatSelect;
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
void	Cls_GrP3DoBtnPage::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoBtnPage, V_GrP3DoBtnPageParaInfo, E_GrP3DoBtnPageParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnPage::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlInit();

		LcSelStatUpdt();		// select status update
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnPage::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
	// code --------------------
#ifdef GR_P3_EDITOR
		if ( !((Cls_GrP3DvlMng*)V_GrP3Mng)->DvlIsDbgRun() )
		{
			return;
		}
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnPage::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Key )
		{
			case E_GrP3KeyEnter:
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
BOOL8	Cls_GrP3DoBtnPage::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Key )
		{
			case E_GrP3KeyEnter:
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
BOOL8	Cls_GrP3DoBtnPage::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::ProcEvtMseLbtnDown( A_X, A_Y, A_BtnMap );
		m_IsPush	=	TRUE;
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoBtnPage::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		Cls_GrP3DoBase::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		m_IsPush	=	FALSE;
		ReqDraw();
		// do change
		LcReqFrmPageChg();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnPage::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnPage::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnPage::LcSelStatUpdt( void )
{
	// local -------------------
		Cls_GrP3DoFrame*	Tv_Frm;
		BOOL8	Tv_IsSel;
	// code --------------------
		Tv_IsSel	=	FALSE;
		if ( m_IsVisible && m_ParaDoBase.IsEnable )
		{
			// find exist control
			if ( (E_GrP3ScptIdNone != m_ParaDoBtnPage.FrameId) && (E_GrP3ScptIdNone != m_ParaDoBtnPage.PageId) )
			{
				Tv_Frm	=	(Cls_GrP3DoFrame*)m_ObjWin->FindSlaveObjByScptId( m_ParaDoBtnPage.FrameId );
				if ( NULL != Tv_Frm )
				{
					if ( E_GrP3DoClassFrame == Tv_Frm->ClassIdGet() )
					{
						// get page id
						if ( m_ParaDoBtnPage.PageId == Tv_Frm->DispPageIdGet() )
						{
							Tv_IsSel	=	TRUE;
						}
					}
#ifdef GR_P3_EDITOR
					else
					{
						DbgMsgPrint( "Cls_GrP3DoBtnPage ( %d ) - incorrect frame type ( %d ).\n", m_ScptId, m_ParaDoBtnPage.FrameId );
					}
#endif
				}
#ifdef GR_P3_EDITOR
				else
				{
					DbgMsgPrint( "Cls_GrP3DoBtnPage ( %d ) - bad frame id ( %d ).\n", m_ScptId, m_ParaDoBtnPage.FrameId );
				}
#endif
			}
		}
		// update select
		m_IsSelect	=	Tv_IsSel;

}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnPage::LcReqFrmPageChg( void )
{
	// local -------------------
		Cls_GrP3DoFrame*	Tv_Frm;
		St_GrP3MsgFrmReqPageChg	Tv_Msg;
	// code --------------------
		// find exist control
		if ( (E_GrP3ScptIdNone != m_ParaDoBtnPage.FrameId) && (E_GrP3ScptIdNone != m_ParaDoBtnPage.PageId) )
		{
			Tv_Frm	=	(Cls_GrP3DoFrame*)m_ObjWin->FindSlaveObjByScptId( m_ParaDoBtnPage.FrameId );
			if ( NULL != Tv_Frm )
			{
				if ( E_GrP3DoClassFrame == Tv_Frm->ClassIdGet() )
				{
					// request page change
					Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdFrameReqPageChg;
					Tv_Msg.Hd.ConIdx	=	E_GrP3ConIdxFocus;
					Tv_Msg.Hd.WinId		=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
					Tv_Msg.Hd.ObjId		=	m_ParaDoBtnPage.FrameId;
					Tv_Msg.PageId			=	m_ParaDoBtnPage.PageId;
					Tv_Msg.Layer			=	m_ParaDoBtnPage.FrmLayer;
					Tv_Msg.IsBack			=	m_ParaDoBtnPage.IsUpLayer;
					Tv_Msg.IsMovFcs		=	m_ParaDoBtnPage.IsMovFcs;
					((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				}
#ifdef GR_P3_EDITOR
				else
				{
					DbgMsgPrint( "Cls_GrP3DoBtnPage ( %d ) - incorrect frame type ( %d ).\n", m_ScptId, m_ParaDoBtnPage.FrameId );
				}
#endif
			}
#ifdef GR_P3_EDITOR
			else
			{
				DbgMsgPrint( "Cls_GrP3DoBtnPage ( %d ) - bad frame id ( %d ).\n", m_ScptId, m_ParaDoBtnPage.FrameId );
			}
#endif
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoBtnPage::ProcEvtFramePageChged( __u16 A_FrmadId, __u16 A_PageId )
{
	// local -------------------
	// code --------------------
		LcSelStatUpdt();
		ReqDraw();
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoBtnPage::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaDoBtnPage.TxtId );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------

