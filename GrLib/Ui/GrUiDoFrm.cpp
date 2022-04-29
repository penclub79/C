/*
 osd text draw

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiDoFrm.h>
#include	<Ui/GrUiDoLbl.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>

#ifdef GR_OSD_EDIT
	#include	<DvlUi/GrUiDvlDoWin.h>
#endif

#include	<Ui/GrUiDoSpinNum.h>
#include	<Ui/GrUiDoSpinTxt.h>

//====================================================================
//local const
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

extern	Cls_GrUiMng*				V_GrUiMngObj;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoFrm::Cls_GrUiDoFrm( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
		//init
		m_TypeId			=	E_GrUiDoTypeIdFrame;

		m_IsNoSlvFcs	=	FALSE;

		GrDumyZeroMem( m_PageObjIdTbl, sizeof(m_PageObjIdTbl) );
		GrDumyZeroMem( m_PageFcsIdTbl, sizeof(m_PageFcsIdTbl) );
		m_PageIdx			=	0;

		m_RelLblId		=	E_GrUiObjIdNone;
		m_LastPageId	=	E_GrUiObjIdNone;
}
//--------------------------------------------------------------------
Cls_GrUiDoFrm::~Cls_GrUiDoFrm()
{
	// local -------------------
		St_GrUiMsgPageFree	Tv_MsgPgChg;
	// code --------------------
		//release lastest page id
		if ( E_GrUiObjIdNone != m_LastPageId )
		{
			//send page release event
			Tv_MsgPgChg.Hd.Cmd			=	E_GrUiMsgCmdPageFree;
			Tv_MsgPgChg.Hd.ConIdx		=	E_GrUiConIdxFocused;
			Tv_MsgPgChg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_MsgPgChg.Hd.ObjId		=	E_GrUiObjIdNone;
			//Tv_MsgPgChg.ObjFrm			=	(void*)this;
			Tv_MsgPgChg.PageId			=	m_LastPageId;
			Tv_MsgPgChg.FrmId				=	m_ObjId;
			UiMsgToWin( (Ptr_GrUiMsgHd)&Tv_MsgPgChg );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::FrmPageObjIdSet( __u8 A_Idx, __u16 A_ScptId )
{
		//check range
		if ( E_GrUiDoFrmMaxPageCnt > A_Idx )
		{
			m_PageObjIdTbl[A_Idx]		=	A_ScptId;
			if ( A_Idx == m_PageIdx )
			{
				LcFrmUpdtContents();
				ReqDraw();
			}
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoFrm::FrmPageObjIdGet( __u8 A_Idx )
{
	// local -------------------
		__u16		Tv_Result;
	// code --------------------
		Tv_Result		=	E_GrUiObjIdNone;
		
		if ( E_GrUiDoFrmMaxPageCnt > A_Idx )
		{
			Tv_Result		=	m_PageObjIdTbl[A_Idx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoFrm::FrmPageNowObjIdGet( void )
{
		return	m_PageObjIdTbl[m_PageIdx];
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::LcPageIndexChg( __u8 A_Index )
{
		//check exist id
		if( E_GrUiObjIdNone != m_PageObjIdTbl[A_Index] )
		{
			//check need focus reset
			if ( m_PageIdx <= A_Index )
			{
				//save now status
				LcSaveLastestStat();
				//reset new page index
				m_PageFcsIdTbl[A_Index]	=	E_GrUiObjIdNone;
			}
			//set index
			m_PageIdx		=	A_Index;
			//update tab draw
			LcFrmUpdtContents();
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoFrm::FrmPageViewIdxSet( __u8 A_Idx, BOOL8 A_IsDirect )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check range
		if ( E_GrUiDoFrmMaxPageCnt > A_Idx )
		{
			if ( A_IsDirect )
			{
				LcPageIndexChg( A_Idx );
			}
			else
			{
				LcReqPageIndexChg( (__u16)A_Idx );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::LcDirectPageGo( __u16 A_PageId, BOOL8 A_IsHisReset )
{
	// local -------------------
	// code --------------------
		if ( A_IsHisReset )
		{
			m_PageIdx	=	0;
			m_PageObjIdTbl[m_PageIdx]	=	A_PageId;
			m_PageFcsIdTbl[m_PageIdx]	=	E_GrUiObjIdNone;
			LcFrmUpdtContents();
			ReqDraw();
		}
		else
		{
			//check move able
			if ( E_GrUiDoFrmMaxPageCnt > (m_PageIdx + 1) )
			{
				//save now status
				LcSaveLastestStat();
				//set new page
				m_PageIdx ++;
				m_PageObjIdTbl[m_PageIdx]	=	A_PageId;
				m_PageFcsIdTbl[m_PageIdx]	=	E_GrUiObjIdNone;
				LcFrmUpdtContents();
				ReqDraw();
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoFrm::FrmPageViewIdxGet( void )
{
		return	m_PageIdx;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::LcRltLblUpdt( void )
{
	// local -------------------
		__u8	Tv_Layer;
		St_GrRect	Tv_Rect;
		Cls_GrUiDoLbl*	Tv_Lbl;
		__u32	Tv_TxtId;
		WCHAR*	Tv_StrRlt;
		BOOL8	Tv_IsSlave;
	// code --------------------
		if ( V_GrUiMngObj->ScptAtbGet( m_PageObjIdTbl[m_PageIdx], &Tv_Layer, &Tv_Rect, &Tv_IsSlave ) )
		{
			if ( Tv_IsSlave )
			{
				if ( E_GrUiObjIdNone != m_RelLblId )
				{
					//get text id
					Tv_TxtId	=	( (0xFFFF & (__u32)Tv_Rect.left) |  ((0xFFFF & (__u32)Tv_Rect.top) << 16) );
					if ( E_GrUiTxtIdNone != Tv_TxtId )
					{
						Tv_StrRlt	=	V_GrUiMngObj->TextPtrGet( Tv_TxtId );
						if ( NULL != Tv_StrRlt )
						{
							Tv_Lbl	=	(Cls_GrUiDoLbl*)m_ObjWin->FindSlaveObjByObjId( m_RelLblId );
							if ( NULL != Tv_Lbl )
							{
								if ( E_GrUiDoTypeIdLabel == Tv_Lbl->TypeIdGet() )
								{
									//change text
									Tv_Lbl->TextSet( Tv_StrRlt );
								}
							}
						}
					}
				}
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoFrm::LcFrmUpdtContents( void )
{
	// local -------------------
		BOOL8	Tv_Result;
#ifdef GR_OSD_EDIT
		Cls_GrUiDvlDoWin*	Tv_WinDvl;
		BOOL8	Tv_IsAble;
#endif
		__u8	Tv_Layer;
		St_GrRect	Tv_Rect;
		BOOL8	Tv_IsSlave;
		Cls_GrUiDoBase*	Tv_ObjFcs;
		St_GrUiMsgPageChg	Tv_MsgPgChg;
		St_GrUiMsgPageFree	Tv_MsgPgFree;
		St_GrUiMsgHd	Tv_MsgScptDec;
		__u16	Tv_FcsId;
	// code --------------------
		Tv_Result	=	FALSE;

		//release lastest page id
		if ( E_GrUiObjIdNone != m_LastPageId )
		{
			//send page release event
			Tv_MsgPgFree.Hd.Cmd			=	E_GrUiMsgCmdPageFree;
			Tv_MsgPgFree.Hd.ConIdx		=	E_GrUiConIdxFocused;
			Tv_MsgPgFree.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_MsgPgFree.Hd.ObjId		=	E_GrUiObjIdNone;
			//Tv_MsgPgFree.ObjFrm			=	(void*)this;
			Tv_MsgPgFree.PageId			=	m_LastPageId;
			Tv_MsgPgFree.FrmId				=	m_ObjId;
			UiMsgToWin( (Ptr_GrUiMsgHd)&Tv_MsgPgFree );
		}

		//release slave objects
		ObjSlaveFreeAll();
		m_LastPageId	=	E_GrUiObjIdNone;

#ifdef GR_OSD_EDIT
		Tv_IsAble		=	FALSE;
		if ( E_GrUiDoWinTypeNormal == ((Cls_GrUiDoWin*)m_ObjWin)->WinTypeGet() )
		{
			Tv_IsAble		=	TRUE;
		}
		else
		{
			Tv_WinDvl	=	(Cls_GrUiDvlDoWin*)m_ObjWin;
			Tv_IsAble	=	!Tv_WinDvl->IsOsdEditMode();
		}
		
		if ( Tv_IsAble )
		{
#endif
			//check script
			if ( V_GrUiMngObj->ScptAtbGet( m_PageObjIdTbl[m_PageIdx], &Tv_Layer, &Tv_Rect, &Tv_IsSlave ) )
			{
				if ( Tv_IsSlave )
				{
					Tv_Result	=	V_GrUiMngObj->ScptDecById( m_PageObjIdTbl[m_PageIdx], (Cls_GrUiDoBase*)this );
					//update new page id
					m_LastPageId	=	m_PageObjIdTbl[m_PageIdx];
					//update relate text
					LcRltLblUpdt();
					//send scripted decoded event
					Tv_MsgScptDec.Cmd		=	E_GrUiMsgCmdScptDecCpl;
					Tv_MsgScptDec.ConIdx	=	E_GrUiConIdxFocused;
					Tv_MsgScptDec.WinId	=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
					Tv_MsgScptDec.ObjId	=	E_GrUiObjIdBroadcast;
					UiMsgSend( &Tv_MsgScptDec );
					//UiMsgPost( &Tv_MsgScptDec, sizeof(Tv_MsgScptDec) );

					//send page change event
					Tv_MsgPgChg.Hd.Cmd			=	E_GrUiMsgCmdPageChange;
					Tv_MsgPgChg.Hd.ConIdx		=	E_GrUiConIdxFocused;
					Tv_MsgPgChg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
					Tv_MsgPgChg.Hd.ObjId		=	E_GrUiObjIdNone;
					//Tv_MsgPgChg.ObjFrm			=	(void*)this;
					//Tv_MsgPgChg.PageId			=	m_PageObjIdTbl[m_PageIdx];
					Tv_MsgPgChg.FrmId				=	m_ObjId;
					UiMsgPost( (Ptr_GrUiMsgHd)&Tv_MsgPgChg, sizeof(Tv_MsgPgChg) );

					//set new focus
					if ( !m_IsNoSlvFcs )
					{
						//get lastest
						Tv_ObjFcs	=	NULL;
						Tv_FcsId	=	m_PageFcsIdTbl[m_PageIdx];
						if ( E_GrUiObjIdNone != Tv_FcsId )
						{
							Tv_ObjFcs	=	FindSlaveObjByObjId( Tv_FcsId );
						}
						//check old focus exist
						if ( NULL == Tv_ObjFcs )
						{
							Tv_ObjFcs	=	FindFocusableSlave();
						}
						if ( NULL != Tv_ObjFcs )
						{
							m_ObjConsole->FocusObjSet( Tv_ObjFcs );
						}
					}
				}
			}
#ifdef GR_OSD_EDIT
		}
#endif
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::ProcUiMsg( Ptr_GrUiMsgHd A_PtrMsg )
{
		//default event handler
		switch( A_PtrMsg->Cmd )
		{
			case	E_GrUiMsgCmdBtnUp:
				ProcEvtBtnUp( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdSpinValChg:
				ProcEvtSpinChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdReqPageChg:
				if ( ((Ptr_GrUiMsgReqPageChg)A_PtrMsg)->IsDir )
				{
					//direct page change
					LcDirectPageGo( ((Ptr_GrUiMsgReqPageChg)A_PtrMsg)->Para, ((Ptr_GrUiMsgReqPageChg)A_PtrMsg)->IsReset );
				}
				else
				{
					LcPageIndexChg( (__u8)((Ptr_GrUiMsgReqPageChg)A_PtrMsg)->Para );
				}
				break;
			default:
				Cls_GrUiDoSkin::ProcUiMsg( A_PtrMsg );
		}

}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::ProcEvtBtnUp( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		__u32		Tv_UserData;
		__u32		Tv_Type;
		__u32		Tv_Code;
	// code --------------------
		//check tab page button click
		Tv_UserData		=	((Cls_GrUiDoBtn*)A_Obj)->UserDataGet();
		Tv_Type				=	Tv_UserData & E_GrUiDoFrmBcTypeMask;
		Tv_Code				=	Tv_UserData & E_GrUiDoFrmBcCodeMask;
		switch ( Tv_Type )
		{
			case	E_GrUiDoFrmBcTypePageNum:
				FrmPageViewIdxSet( (__u8)Tv_Code );
				break;
			case	E_GrUiDoFrmBcTypePageMove:
				LcProcEvtBtnPageMove( Tv_Code );
				break;
			case	E_GrUiDoFrmBcTypePageGo:
				FrmDirectPageGo( (__u16)A_Obj->UserDataGet( 1 ), FALSE );
				break;
			case	E_GrUiDoFrmBcTypePageGoRst:
				FrmDirectPageGo( (__u16)A_Obj->UserDataGet( 1 ), TRUE );
				break;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoFrm::FrmPageViewNext( BOOL8 A_IsDirect )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		//check able
		if ( E_GrUiDoFrmMaxPageCnt > (m_PageIdx + 1) )
		{
			Tv_Result	=	FrmPageViewIdxSet( m_PageIdx + 1, A_IsDirect );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoFrm::FrmPageViewPrev( BOOL8 A_IsDirect )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		//check able
		if ( 0 != m_PageIdx )
		{
			Tv_Result	=	FrmPageViewIdxSet( m_PageIdx - 1, A_IsDirect );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoFrm::FrmPageViewFirst( BOOL8 A_IsDirect )
{
		return	FrmPageViewIdxSet( 0, A_IsDirect );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoFrm::FrmPageViewSecond( BOOL8 A_IsDirect )
{
		return	FrmPageViewIdxSet( 1, A_IsDirect );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoFrm::FrmPageViewLast( BOOL8 A_IsDirect )
{
	// local -------------------
		__u8		Tv_Idx;
		__u8		Tv_Page;
	// code --------------------
		Tv_Page		=	0;
		for ( Tv_Idx=0;Tv_Idx < E_GrUiDoFrmMaxPageCnt;Tv_Idx++ )
		{
			if ( E_GrUiObjIdNone == m_PageObjIdTbl[Tv_Idx] )
			{
				break;
			}
			else
			{
				Tv_Page	=	Tv_Idx;
			}
		}
		return	FrmPageViewIdxSet( Tv_Page, A_IsDirect );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::LcProcEvtBtnPageMove( __u32 A_Code )
{
		switch ( A_Code )
		{
			case	E_GrUiDoFrmBcCodePagePrev:
				FrmPageViewPrev();
				break;
			case	E_GrUiDoFrmBcCodePageNext:
				FrmPageViewNext();
				break;
			case	E_GrUiDoFrmBcCodePageFirst:
				FrmPageViewFirst();
				break;
			case	E_GrUiDoFrmBcCodePageLast:
				FrmPageViewLast();
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::FrmPageClear( void )
{
		GrDumyZeroMem( m_PageObjIdTbl, sizeof(m_PageObjIdTbl) );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::ProcEvtRelate( __u32 A_Para )
{
		switch( A_Para )
		{
			case	E_GrUiDoFrmRelParaPrevPage:
				FrmPageViewPrev();
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::FrmRelLabelIdSet( __u16 A_ScptId )
{
		m_RelLblId	=	A_ScptId;
		LcRltLblUpdt();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoFrm::FrmRelLabelIdGet( void )
{
		return	m_RelLblId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::ProcEvtSpinChg( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		Cls_GrUiDoSpinNum*	Tv_SpinNum;
		Cls_GrUiDoSpinTxt*	Tv_SpinTxt;
	// code --------------------
		//check type
		switch( A_Obj->TypeIdGet() )
		{
			case	E_GrUiDoTypeIdSpbtnNum:
				Tv_SpinNum	=	(Cls_GrUiDoSpinNum*)A_Obj;
				FrmPageViewIdxSet( (__u8)Tv_SpinNum->ValueGet() );
				break;
			case	E_GrUiDoTypeIdSpbtnTxt:
				Tv_SpinTxt	=	(Cls_GrUiDoSpinTxt*)A_Obj;
				FrmPageViewIdxSet( (__u8)Tv_SpinTxt->SelectIdxGet() );
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::ProcEvtMseEnter( void )
{
		//not work
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::ProcEvtMseLeave( void )
{
		//not work
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::ProcEvtScptDecoded( void )
{
		Cls_GrUiDoSkin::ProcEvtScptDecoded();
		LcRltLblUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::LcReqPageIndexChg( __u16 A_Index )
{
	// local -------------------
		St_GrUiMsgReqPageChg	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdReqPageChg;
		Tv_Msg.Hd.ConIdx	=	m_ObjConsole->IndexGet();
		Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
		Tv_Msg.Hd.ObjId		=	m_ObjId;
		Tv_Msg.IsDir			=	FALSE;
		Tv_Msg.Para				=	A_Index;
		UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::LcReqPageDirectChg( __u16 A_PageId, BOOL8 A_IsHisReset )
{
	// local -------------------
		St_GrUiMsgReqPageChg	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdReqPageChg;
		Tv_Msg.Hd.ConIdx	=	m_ObjConsole->IndexGet();
		Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
		Tv_Msg.Hd.ObjId		=	m_ObjId;
		Tv_Msg.IsDir			=	TRUE;
		Tv_Msg.IsReset		=	A_IsHisReset;
		Tv_Msg.Para				=	A_PageId;
		UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::FrmDirectPageGo( __u16 A_PageId, BOOL8 A_IsHisReset, BOOL8 A_IsDirect )
{
		if ( A_IsDirect )
		{
			LcDirectPageGo( A_PageId, A_IsHisReset );
		}
		else
		{
			LcReqPageDirectChg( A_PageId, A_IsHisReset );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoFrm::IsFrmNoMoveFocus( void )
{
		return	m_IsNoSlvFcs;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::FrmNoMoveFocusSet( BOOL8 A_IsOn )
{
		m_IsNoSlvFcs	=	A_IsOn;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoFrm::LcSaveLastestStat( void )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Obj;
		__u16	Tv_Id;
	// code --------------------
		Tv_Obj	=	m_ObjConsole->FocusObjGet();
		Tv_Id		=	E_GrUiObjIdNone;
		if ( NULL != Tv_Obj )
		{
			if ( NULL != FindSlaveObjByObjId( Tv_Obj->ObjIdGet() ) )
			{
				//set
				Tv_Id	=	Tv_Obj->ObjIdGet();
			}
		}
		//save
		m_PageFcsIdTbl[m_PageIdx]	=	Tv_Id;
}
//--------------------------------------------------------------------
