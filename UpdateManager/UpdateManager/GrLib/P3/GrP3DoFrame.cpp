/*
		platform label

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoFrame.h>
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

#define E_GrP3DoFrameParaCnt		4

St_GrP3DvlDoPrInfoData	V_GrP3DoFrameParaInfo[E_GrP3DoFrameParaCnt]	=	
{
	{	L"Paint BG",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoFramePara)0)->PntBg,NULL,	L"Paint background code.\n0 = None, 1 = basic code"	},
	{	L"Start Page",		TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoFramePara)0)->StartPage,NULL,	L"Start page script id"	},
	{	L"EscKeyFocus",		TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoFramePara)0)->EscKeyFocus,NULL,	L"Move focus when press ESC or MENU key"	},
	{	L"QuitRelateFrm",	TRUE,		E_GrP3DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP3DoFramePara)0)->QuitRelateFrm,NULL,	L"quit action relate frame"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoFrame::Cls_GrP3DoFrame( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassFrame;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoFrame) + sizeof(St_GrP3ScptParaHd);

		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaDoFrame, sizeof(m_ParaDoFrame) );
		m_ParaDoFrame.PntBg	=	E_GrP3ScptPstIdDflt;

		GrDumyZeroMem( m_PageCch, sizeof(m_PageCch) );
		m_CchLayer	=	0;

		m_ObjFrm		=	this;
}
//--------------------------------------------------------------------
Cls_GrP3DoFrame::~Cls_GrP3DoFrame( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoFrame::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassFrame;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoFrame);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoFrame, sizeof(m_ParaDoFrame) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoFrame)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoFrame::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoFrame, sizeof(m_ParaDoFrame) , m_PtrScpt , E_GrP3DoClassFrame );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoFrame::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiFrmPageSet:
				ProcEvtFrmReqPageChg( FALSE, (__u8)A_PtrInt[2], (__u16)A_PtrInt[1], (BOOL8)A_PtrInt[3] );
				break;
			case E_GrP3UpiFrmPageBack:
				ProcEvtFrmReqPageChg( TRUE, 0, 0, (BOOL8)A_PtrInt[1] );
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoFrame::ProcPaint( void )
{
		PaintScriptPush( m_ParaDoFrame.PntBg, &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoFrame::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherit
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoFrame, V_GrP3DoFrameParaInfo, E_GrP3DoFrameParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoFrame::ProcUiMsg( Ptr_GrP3MsgHd A_PtrMsg )
{
	// local -------------------
	// code --------------------
		switch ( A_PtrMsg->Cmd )
		{
			case E_GrP3MsgCmdFrameReqPageChg:
				ProcEvtFrmReqPageChg( ((Ptr_GrP3MsgFrmReqPageChg)A_PtrMsg)->IsBack, ((Ptr_GrP3MsgFrmReqPageChg)A_PtrMsg)->Layer,
					((Ptr_GrP3MsgFrmReqPageChg)A_PtrMsg)->PageId, ((Ptr_GrP3MsgFrmReqPageChg)A_PtrMsg)->IsMovFcs );
				break;
			default:
				Cls_GrP3DoBase::ProcUiMsg( A_PtrMsg );
				break;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoFrame::ProcEvtFrmReqPageChg( BOOL8 A_IsBack, __u8 A_Layer, __u16 A_PageId, BOOL8 A_IsMoveFcs )
{
	// local -------------------
		BOOL8	Tv_IsChg;
		__u8	Tv_ItmIdx;
		__u16	Tv_OldPage;
	// code --------------------
		Tv_IsChg		=	FALSE;
		Tv_OldPage	=	m_PageCch[m_CchLayer].ItmTbl[m_PageCch[m_CchLayer].ItmIdx].PagdId;
		// check back
		if ( A_IsBack )
		{
			if ( 0 != m_CchLayer )
			{
				// decrease layer
				m_CchLayer --;
				Tv_IsChg	=	TRUE;
			}
		}
		else
		{
			if ( E_GrP3DoFrmCchLayerCnt > A_Layer )
			{
				// update layer
				m_CchLayer	=	A_Layer;
				// find exist page
				for ( Tv_ItmIdx=0;Tv_ItmIdx < E_GrP3DoFrmCchPageCnt;Tv_ItmIdx++ )
				{
					if ( A_PageId == m_PageCch[A_Layer].ItmTbl[Tv_ItmIdx].PagdId )
					{
						// found
						m_PageCch[A_Layer].ItmIdx	=	(__u32)Tv_ItmIdx;
						Tv_IsChg	=	TRUE;
						break;
					}
				}
				// check need new
				if ( !Tv_IsChg )
				{
					for ( Tv_ItmIdx=0;Tv_ItmIdx < E_GrP3DoFrmCchPageCnt;Tv_ItmIdx++ )
					{
						if ( E_GrP3ScptIdNone == m_PageCch[A_Layer].ItmTbl[Tv_ItmIdx].PagdId )
						{
							// found
							m_PageCch[A_Layer].ItmIdx	=	(__u32)Tv_ItmIdx;
							m_PageCch[A_Layer].ItmTbl[Tv_ItmIdx].PagdId		=	A_PageId;
							m_PageCch[A_Layer].ItmTbl[Tv_ItmIdx].FocusId	=	E_GrP3ScptIdNone;
							Tv_IsChg	=	TRUE;
							break;
						}
					}
				}
				// check need replace
				if ( !Tv_IsChg )
				{
					Tv_ItmIdx	=	(__u8)m_PageCch[A_Layer].ItmIdx + 1;
					if ( E_GrP3DoFrmCchPageCnt <= Tv_ItmIdx )
					{
						Tv_ItmIdx	=	0;
					}
					// found
					m_PageCch[A_Layer].ItmIdx	=	(__u32)Tv_ItmIdx;
					m_PageCch[A_Layer].ItmTbl[Tv_ItmIdx].PagdId		=	A_PageId;
					m_PageCch[A_Layer].ItmTbl[Tv_ItmIdx].FocusId	=	E_GrP3ScptIdNone;
					Tv_IsChg	=	TRUE;
				}
				
			}
		}
		// update display
		if ( Tv_IsChg )
		{
			LcDoPageChange( Tv_OldPage, A_IsMoveFcs );
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoFrame::LcDoPageChange( __u16 A_OldPageId, BOOL8 A_IsMoveFocus )
{
	// local -------------------
		__u16	Tv_AsmId;
	// code --------------------
		// run page end code
		if ( E_GrP3ScptIdNone != A_OldPageId )
		{
			Tv_AsmId	=	V_GrP3Mng->PageAsmCloseGet( A_OldPageId );
			V_GrP3Mng->VmRunCode( Tv_AsmId, this );
		}

		// release old
		ObjSlaveFreeAll();

		if ( LcCchShow( A_IsMoveFocus ) )
		{
			// notify change
			LcNotifyPageChange();
			// redraw
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoFrame::LcCchShow( BOOL8 A_IsChgFcs )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_PageId;
		__u16	Tv_FocusId;
		Cls_GrP3DoBase*	Tv_Obj;
		__u16	Tv_AsmId;
	// code --------------------
		Tv_Result	=	FALSE;

		// check exist script
		Tv_PageId		=	m_PageCch[m_CchLayer].ItmTbl[m_PageCch[m_CchLayer].ItmIdx].PagdId;
		if ( V_GrP3Mng->PageDec( Tv_PageId, this ) )
		{
			// check change focus
			if ( A_IsChgFcs )
			{
				// update focus
				Tv_FocusId	=	m_PageCch[m_CchLayer].ItmTbl[m_PageCch[m_CchLayer].ItmIdx].FocusId;
				// set focus
				Tv_Obj	=	FindSlaveObjByScptId( Tv_FocusId );
				if ( NULL == Tv_Obj )
				{
					Tv_Obj	=	FindFocusableSlave();
				}
				if ( NULL != Tv_Obj )
				{
					((Cls_GrP3ConBase*)m_ObjCon)->FocusObjSet( Tv_Obj );
				}
			}
			
			// slave init
			// do slave
			Tv_Obj	=	m_ObjSlave;
			while ( NULL != Tv_Obj )
			{
				// init
				Tv_Obj->RtlInit();
				// next
				Tv_Obj	=	Tv_Obj->NextObjGet();
			}

			// call open assembly
			Tv_AsmId	=	V_GrP3Mng->PageAsmOpenGet( Tv_PageId );
			V_GrP3Mng->VmRunCode( Tv_AsmId, this );
			// success
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DoFrame::DispPageIdGet( void )
{
	// local -------------------
	// code --------------------
		return	m_PageCch[m_CchLayer].ItmTbl[m_PageCch[m_CchLayer].ItmIdx].PagdId;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoFrame::LcNotifyPageChange( void )
{
	// local -------------------
		St_GrP3MsgFrmPageChged	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Cmd					=	E_GrP3MsgCmdFramePageChged;
		Tv_Msg.Hd.ConIdx			=	E_GrP3ConIdxFocus;
		Tv_Msg.Hd.WinId				=	((Cls_GrP3DoWin*)m_ObjWin)->WinIdGet();
		Tv_Msg.Hd.ObjId				=	E_GrP3ScptIdAll;
		Tv_Msg.FramdId				=	m_ScptId;
		Tv_Msg.PageId					=	m_PageCch[m_CchLayer].ItmTbl[m_PageCch[m_CchLayer].ItmIdx].PagdId;
		((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoFrame::RtlInit( void )
{
	// local -------------------
	// code --------------------
		// Cls_GrP3DoBase::RtlInit();			// do not inherited
		m_PageCch[0].ItmTbl[0].PagdId	=	m_ParaDoFrame.StartPage;
		if ( LcCchShow( TRUE ) )
		{
			// notify change
			LcNotifyPageChange();
			// redraw
			ReqDraw();
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoFrame::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
		__u16	Tv_ScptId;
		__u16	Tv_EscObjId;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3ScptIdNone != m_ParaDoFrame.EscKeyFocus )
		{
			if ( (E_GrP3KeyEsc == A_Key) || (E_GrP3KeyMenu == A_Key) )
			{
				// check now layer
				if ( 0 != m_CchLayer )
				{
					// decrease layer
					V_GrP3Mng->DpcUpiCall( ((Cls_GrP3ConBase*)m_ObjCon)->IndexGet() , m_ObjWin->ScptIdGet(), m_ScptId,E_GrP3UpiFrmPageBack,
						TRUE );
				}
				else
				{
					// try move focus
					Tv_EscObjId	=	E_GrP3ScptIdNone;
					// check page exist focus
					Tv_ScptId	=	m_PageCch[m_CchLayer].ItmTbl[m_PageCch[m_CchLayer].ItmIdx].PagdId;
					if ( E_GrP3ScptIdNone != Tv_ScptId )
					{
						Tv_EscObjId	=	V_GrP3Mng->PageEscKeyFcsIdGet( Tv_ScptId );
					}
					if ( E_GrP3ScptIdNone == Tv_EscObjId )
					{
						Tv_EscObjId	=	m_ParaDoFrame.EscKeyFocus;
					}
					if ( E_GrP3ScptIdNone != Tv_EscObjId )
					{
						Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaDoFrame.EscKeyFocus );
						if ( NULL != Tv_Obj )
						{
							((Cls_GrP3ConBase*)m_ObjCon)->FocusObjSet( Tv_Obj );
						}
					}

					// quit relate action
					if ( E_GrP3ScptIdNone != m_ParaDoFrame.QuitRelateFrm )
					{
						V_GrP3Mng->DpcUpiCall( ((Cls_GrP3ConBase*)m_ObjCon)->IndexGet() , m_ObjWin->ScptIdGet(), m_ParaDoFrame.QuitRelateFrm,E_GrP3UpiFrmPageBack,
								TRUE );
						Tv_Result	=	TRUE;
					}

				}
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3DoFrame::LastFocusObjGet( void )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Result;
		__u16	Tv_FcsId;
	// code --------------------
		Tv_Result	=	NULL;
		Tv_FcsId	=	m_PageCch[m_CchLayer].ItmTbl[m_PageCch[m_CchLayer].ItmIdx].FocusId;
		if ( E_GrP3ScptIdNone != Tv_FcsId )
		{
			Tv_Result	=	FindSlaveObjByScptId( Tv_FcsId );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoFrame::RtlLastFocusObjIdSet( __u16 A_ScptId )
{
		m_PageCch[m_CchLayer].ItmTbl[m_PageCch[m_CchLayer].ItmIdx].FocusId	=	A_ScptId;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoFrame::ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( 0 != m_CchLayer )
		{
			// decrease layer
			V_GrP3Mng->DpcUpiCall( ((Cls_GrP3ConBase*)m_ObjCon)->IndexGet() , m_ObjWin->ScptIdGet(), m_ScptId,E_GrP3UpiFrmPageBack,
					TRUE );
			Tv_Result	=	TRUE;
		}
		else if ( E_GrP3ScptIdNone != m_ParaDoFrame.QuitRelateFrm )
		{
			V_GrP3Mng->DpcUpiCall( ((Cls_GrP3ConBase*)m_ObjCon)->IndexGet() , m_ObjWin->ScptIdGet(), m_ParaDoFrame.QuitRelateFrm,E_GrP3UpiFrmPageBack,
					TRUE );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
