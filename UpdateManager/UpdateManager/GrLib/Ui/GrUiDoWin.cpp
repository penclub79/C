/*
 osd object

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoWin.h>
#include	<GrStrTool.h>
#include	<GrTimeTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiConsole.h>
#include	<Ui/GrUiDoFrm.h>
#include	<Ui/GrUiDoSpinNum.h>

//====================================================================
//local const
#define E_GrUiDoWinEvtBrdDepth		16			/* event borad cast depth */

#define E_GrUiDoWinMoveStep				32			/* window move step */

//====================================================================
//local type

//====================================================================
//local macro
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*				V_GrUiMngObj;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoWin::Cls_GrUiDoWin(	Ptr_GrRect A_PtrRect, __u8 A_Layer, __u8 A_ConIdx, BOOL8 A_IsIdAlloc, BOOL8 A_IsUpdtParm ):
Cls_GrUiDoSkin( A_PtrRect, NULL, A_ConIdx, A_IsIdAlloc )
{
	// local -------------------
	// code --------------------
		//init
		//get console
		m_Layer				=	A_Layer;
		m_WinId				=	E_GrUiWinIdNone;			//win id none
		m_TypeId			=	E_GrUiDoTypeIdWin;

		m_IsMoveAble	=	FALSE;
		m_IsMoving		=	FALSE;
		m_IsLayerFocusLock	=	FALSE;
		m_IsFocusAble	=	TRUE;			//focus able
		m_FocusObj		=	NULL;
		m_CloseTime		=	0;
		m_CloseKeyType	=	E_GrUiDoWinKeyCloseNone;

		m_RelFrmId		=	E_GrUiObjIdNone;

		m_WinType			=	E_GrUiDoWinTypeNormal;
		m_PtrModalRslt	=	NULL;
		m_ModalResult	=	0;

		//regist to console
		if ( A_IsUpdtParm )
		{
			UpdateWinParam();
		}
}
//--------------------------------------------------------------------
Cls_GrUiDoWin::~Cls_GrUiDoWin()
{
		if ( NULL != m_PtrModalRslt )
		{
			*m_PtrModalRslt	=	m_ModalResult;
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoWin::LayerGet( void )
{
		return	m_Layer;
}
//--------------------------------------------------------------------
void*	Cls_GrUiDoWin::ConsolGet( void )
{
		return	m_ObjCon;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::MovingModeSet( BOOL8 A_IsMoving )
{
		if ( m_IsMoveAble )
		{
			m_IsMoving	=	A_IsMoving;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoWin::IsMoveable( void )
{
		return	m_IsMoveAble;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::MoveableSet( BOOL8 A_IsMoveAble )
{
		m_IsMoveAble	=	A_IsMoveAble;
		m_IsMoving		=	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoWin::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		//check moveable
		if ( m_IsMoveAble )
		{
			//get now position
			m_MseMvMseX	=	(__s32)A_X;
			m_MseMvMseY	=	(__s32)A_Y;
			m_MseMvWinX	=	m_Rect.left;
			m_MseMvWinY	=	m_Rect.top;
			//set moving
			m_IsMoving		=	TRUE;
		}
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoWin::ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__s32		Tv_MvX;
		__s32		Tv_MvY;
		__s32		Tv_ResX;
		__s32		Tv_ResY;
		St_GrRect	Tv_RcWin;
	// code --------------------
		//mouse moving
		if ( m_IsMoving )
		{
			if ( ScrRectGet( &Tv_RcWin ) )
			{
				//init
				Tv_ResX	=	(__s32)m_ObjConsole->GetResX();
				Tv_ResY	=	(__s32)m_ObjConsole->GetResY();
				//get moving vactor
				Tv_MvX	=	(__s32)A_X - m_MseMvMseX;
				Tv_MvY	=	(__s32)A_Y - m_MseMvMseY;
				//check horizontal
				if ( 0 <= Tv_MvX )
				{
					if ( Tv_ResX < (Tv_RcWin.right + Tv_MvX) )
					{
						Tv_MvX	=	Tv_ResX -	Tv_RcWin.right;
					}
					//patch
					Tv_MvX		=	Tv_MvX & ~(E_GrUiDoWinMoveStep -1);
				}
				else
				{
					if ( 0 > (Tv_RcWin.left + Tv_MvX ) )
					{
						Tv_MvX	=	-Tv_RcWin.left;
					}
					//patch
					Tv_MvX		=	-( (-Tv_MvX) & ~(E_GrUiDoWinMoveStep -1) );
				}
				//check vertical
				if ( 0 <= Tv_MvY )
				{
					if ( Tv_ResY < (Tv_RcWin.bottom + Tv_MvY) )
					{
						Tv_MvY	=	Tv_ResY - Tv_RcWin.bottom;
					}
					//patch
					Tv_MvY		=	Tv_MvY & ~(E_GrUiDoWinMoveStep -1);
				}
				else
				{
					if ( 0 > (Tv_RcWin.top + Tv_MvY) )
					{
						Tv_MvY	=	-Tv_RcWin.top;
					}
					//patch
					Tv_MvY		=	-( (-Tv_MvY) & ~(E_GrUiDoWinMoveStep -1) );
				}
				//check able
				if ( (0 != Tv_MvX) || (0 != Tv_MvY) )
				{
					//calculate rect
					Tv_RcWin.left		=	Tv_RcWin.left + Tv_MvX;
					Tv_RcWin.top		=	Tv_RcWin.top + Tv_MvY;
					Tv_RcWin.right	=	Tv_RcWin.right + Tv_MvX;
					Tv_RcWin.bottom	=	Tv_RcWin.bottom + Tv_MvY;

					//patch exapnd mode
					if ( V_GrUiMngObj->IsOsdExp() )
					{
						Tv_RcWin.top	=	Tv_RcWin.top - (__s32)m_PalExtPosY;
						Tv_RcWin.bottom	=	Tv_RcWin.bottom - (__s32)m_PalExtPosY - (__s32)m_PalExtSizeY;
					}

					//request change rect
					RectSet( &Tv_RcWin );
					//update position
					if ( 0 != Tv_MvX )
					{
						m_MseMvMseX	=	A_X;
					}
					if ( 0 != Tv_MvY )
					{
						m_MseMvMseY	=	A_Y;
					}
				}
			}
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoWin::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		if ( m_IsMoving )
		{
			//moving finished
			m_IsMoving		=	FALSE;
		}

		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoWin::ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrUiDoFrm*	Tv_ObjFrm;
		St_GrUiMsgObjRel	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		//check relate
		if ( E_GrUiObjIdNone != m_RelFrmId )
		{
			//find frame
			Tv_ObjFrm	=	(Cls_GrUiDoFrm*)FindSlaveObjByObjId( m_RelFrmId );
			if ( NULL != Tv_ObjFrm )
			{
				if ( E_GrUiDoTypeIdFrame == ((Cls_GrUiDoBase*)Tv_ObjFrm)->TypeIdGet() )
				{
					if ( 0 != Tv_ObjFrm->FrmPageViewIdxGet() )
					{
						Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdRelate;
						Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
						Tv_Msg.Hd.WinId		=	m_WinId;
						Tv_Msg.Hd.ObjId		=	m_RelFrmId;
						Tv_Msg.Para				=	E_GrUiDoFrmRelParaPrevPage;
						UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
						Tv_Result					=	TRUE;
					}
				}
			}
		}
		
		return	Tv_Result;	
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoWin::CloseKeyTypeGet( void )
{
		return	m_CloseKeyType;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::CloseKeyTypeSet( __u32 A_Type )
{
		m_CloseKeyType	=	A_Type;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoWin::CloseTimeGet( void )
{
		return	GrTimeTickToMs( m_CloseTime );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::CloseTimeSet( __u32 A_Time )
{
		m_CloseTime	=	GrTimeMsToTick( A_Time );
		if ( 0 != m_CloseTime )
		{
			m_TickClose		=	GrTimeGetTick();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoWin::CloseTimeTickGet( void )
{
		return	m_TickClose;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoWin::IsLayerFcsLock( void )
{
		return	m_IsLayerFocusLock;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::LayerFcsLockSet( BOOL8 A_IsLock )
{
		m_IsLayerFocusLock	=	A_IsLock;
		m_ObjConsole->FocusCorrect();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoWin::ProcEvtKeyUp( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrUiDoFrm*	Tv_ObjFrm;
		St_GrUiMsgObjRel	Tv_Msg;
	// code --------------------
		Tv_Result	=	TRUE;
		//check anykey exit
		if ( E_GrUiDoWinKeyCloseAnykey == m_CloseKeyType )
		{
			//close window
			m_ObjConsole->WindowSafeClose( this );
		}
		else
		{
			//other key
			switch( A_Key )
			{
				case	E_GrUiKeyEsc:
				case	E_GrUiKeyMenu:
					//relate 
					Tv_Result	=	FALSE;
					if ( E_GrUiObjIdNone != m_RelFrmId )
					{
						//find frame
						Tv_ObjFrm	=	(Cls_GrUiDoFrm*)FindSlaveObjByObjId( m_RelFrmId );
						if ( NULL != Tv_ObjFrm )
						{
							if ( E_GrUiDoTypeIdFrame == ((Cls_GrUiDoBase*)Tv_ObjFrm)->TypeIdGet() )
							{
								//check process able
								if ( 0 != Tv_ObjFrm->FrmPageViewIdxGet() )
								{
									Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdRelate;
									Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
									Tv_Msg.Hd.WinId		=	m_WinId;
									Tv_Msg.Hd.ObjId		=	m_RelFrmId;
									Tv_Msg.Para				=	E_GrUiDoFrmRelParaPrevPage;
									UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
									Tv_Result	=	TRUE;
								}
							}
						}
					}
					//esc key quit
					if ( !Tv_Result && (E_GrUiDoWinKeyCloseEsc == m_CloseKeyType) )
					{
						//quit
						m_ObjConsole->WindowSafeClose( this );
						Tv_Result	=	TRUE;
					}
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoWin::ProcEvtKeyDown( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		//other key process
		switch ( A_Key )
		{
			case	E_GrUiKeyLeft:
			case	E_GrUiKeyRight:
			case	E_GrUiKeyUp:
			case	E_GrUiKeyDown:
				//focus process
				Tv_Result	=	LcFocusMoveByKey( A_Key );
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoWin::LcFocusMoveByKey( __u8 A_Key )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Dir;
		Cls_GrUiDoBase*	Tv_ObjDo;
		Cls_GrUiDoBase*	Tv_ObjNow;
		__u32		Tv_FocusMap;
		BOOL8		Tv_IsWide;
		BOOL8		Tv_IsSpnNum;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_ObjNow		=	m_ObjConsole->FocusObjGet();
		//check exist focus object
		if ( NULL != Tv_ObjNow )
		{
			//check slave control
			if ( (Cls_GrUiDoBase*)this == Tv_ObjNow->WindowGet() )
			{
				//check spin number
				Tv_IsSpnNum	=	FALSE;
				if ( E_GrUiDoTypeIdSpbtnNum == Tv_ObjNow->TypeIdGet() )
				{
					if ( !((Cls_GrUiDoSpinNum*)Tv_ObjNow)->IsOnKeyCtrl() )
					{
						Tv_IsSpnNum	=	((Cls_GrUiDoSpinNum*)Tv_ObjNow)->IsCtrlMode();
					}
				}
				Tv_FocusMap		=	Tv_ObjNow->FocusMapGet();
				switch( A_Key )
				{
					case	E_GrUiKeyLeft:
						Tv_Dir	=	E_GrUiDoFindLeft;
						break;	
					case	E_GrUiKeyRight:
						Tv_Dir	=	E_GrUiDoFindRight;
						break;
					case	E_GrUiKeyUp:
						Tv_Dir	=	E_GrUiDoFindUp;
						break;
					case	E_GrUiKeyDown:
						Tv_Dir	=	E_GrUiDoFindDown;
						break;
				}
				//check move able
				if ( 0 != (Tv_FocusMap & (1 << Tv_Dir) ) )
				{
					Tv_IsWide	=	FALSE;
					if ( 0 != (Tv_FocusMap & (1 << (Tv_Dir + E_GrUiDoFindWidePos)) ) )
					{
						Tv_IsWide	=	TRUE;
					}
					//find
					Tv_ObjDo	=	Tv_ObjNow->FindFocusObjByDir( Tv_Dir, Tv_IsWide );
					if ( NULL != Tv_ObjDo )
					{
						m_ObjConsole->FocusObjSet( Tv_ObjDo );
						//check spin number
						if ( Tv_IsSpnNum )
						{
							if ( E_GrUiDoTypeIdSpbtnNum == Tv_ObjDo->TypeIdGet() )
							{
								if ( !((Cls_GrUiDoSpinNum*)Tv_ObjDo)->IsOnKeyCtrl() )
								{
									((Cls_GrUiDoSpinNum*)Tv_ObjDo)->CtrlModeSet( TRUE );
								}
							}
						}
						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::FocusAbleSet( BOOL8 A_IsFocusAble )
{
		m_IsFocusAble		=	A_IsFocusAble;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		__u32		Tv_TickPass;
	// code --------------------
		//time close
		if ( 0 != m_CloseTime )
		{
			Tv_TickPass	=	( A_Tick - m_TickClose );
			if ( Tv_TickPass >= m_CloseTime )
			{
				//request close
				m_CloseTime	=	0;
				Close();					//close window
			}
			else
			{
				m_TickClose	=	A_Tick;
				m_CloseTime	=	m_CloseTime - Tv_TickPass;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		//check exist focus object
		if ( (NULL != m_FocusObj) && ((Cls_GrUiDoBase*)this != m_FocusObj) )
		{
			//change focus
			m_ObjConsole->FocusObjSet( m_FocusObj );
		}
}
//--------------------------------------------------------------------
Cls_GrUiDoBase*	Cls_GrUiDoWin::LastFocusObjGet( void )
{
		return	m_FocusObj;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::Close( void )
{
	m_ObjConsole->WindowSafeClose( this );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::CloseAll( __u16 A_ExcpWinId )
{
		m_ObjConsole->WindowAllClose( A_ExcpWinId );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::CloseLayer( __u8 A_Layer, __u16 A_ExcpWinId )
{
		m_ObjConsole->WindowLayerClose( A_Layer, A_ExcpWinId );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcUiMsg( Ptr_GrUiMsgHd A_PtrMsg )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_Obj;
	// code --------------------
		//default event handler
		switch( A_PtrMsg->Cmd )
		{
			case	E_GrUiMsgCmdWinCreate:
				ProcEvtWinCreate();
				break;
			case	E_GrUiMsgCmdBtnDown:
				ProcEvtBtnDown( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdBtnUp:
				ProcEvtBtnUp( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdConChgConFcs:
				ProcEvtConFcsChg( ((Ptr_GrUiMsgConFcsChg)A_PtrMsg)->IsFocus );
				break;
			case	E_GrUiMsgCmdSbarChg:
				ProcEvtSbarChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdLbxItemClick:
				ProcEvtLbxItmClick( (Cls_GrUiDoBase*)((Ptr_GrUiMsgLbxItmIdx)A_PtrMsg)->Obj, ((Ptr_GrUiMsgLbxItmIdx)A_PtrMsg)->ItemIdx );
				break;
			case	E_GrUiMsgCmdLbxTxtChg:
				ProcEvtLbxTxtChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgLbxItmIdx)A_PtrMsg)->Obj, ((Ptr_GrUiMsgLbxItmIdx)A_PtrMsg)->ItemIdx );
				break;
			case	E_GrUiMsgCmdSpinValChg:
				ProcEvtSpinChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdSpinClick:
				ProcEvtSpinClk( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdEditValChg:
				ProcEvtEdtValChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdPageChange:
				Tv_Obj	=	FindSlaveObjByObjId( ((Ptr_GrUiMsgPageChg)A_PtrMsg)->FrmId );
				if ( NULL != Tv_Obj )
				{
					ProcEvtPageChg( Tv_Obj, ((Cls_GrUiDoFrm*)Tv_Obj)->FrmPageNowObjIdGet() );
				}
				break;
			case	E_GrUiMsgCmdPageFree:
				Tv_Obj	=	FindSlaveObjByObjId( ((Ptr_GrUiMsgPageFree)A_PtrMsg)->FrmId );
				if ( NULL != Tv_Obj )
				{
					ProcEvtPageFree( Tv_Obj, ((Ptr_GrUiMsgPageFree)A_PtrMsg)->PageId );
				}
				break;
			case	E_GrUiMsgCmdCalDayChg:
				ProcEvtCalendarDayChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdCalDaySel:
				ProcEvtCalendarDaySel( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdGrpSelCusChg:
				ProcEvtGrpSelCusChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdGrpSelItmSel:
				ProcEvtGrpSelSelect( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdGrpEditCusChg:
				ProcEvtGrpEditCusChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdGrpEditValChg:
				ProcEvtGrpEditValChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdBitGridCusChg:
				ProcEvtBitGridCusChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdBitGridValChg:
				ProcEvtBitGridValChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdInpTimeChg:
				ProcEvtInpTimeChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdQuadChg:
				ProcEvtQuadChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			//case	E_GrUiMsgCmdMosaicGridValChg:
			//	ProcEvtMosaicGridValChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
			//	break;//new-120905~mosaic
			default:
				Cls_GrUiDoSkin::ProcUiMsg( A_PtrMsg );
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtWinCreate( void )
{
		//redraw
		ReqDraw();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoWin::WinIdGet( void )
{
		return	m_WinId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::RtlWinIdSet( __u16 A_WinId )
{
		m_WinId		=	A_WinId;
}
//--------------------------------------------------------------------
__u16	GrUiDoWinMakeWinId( __u8 A_Layer, __u8 A_WinSeq )
{
		return	((__u16)A_Layer << 8) | ((__u16)A_WinSeq);
}
//--------------------------------------------------------------------
__u8	GrUiDoWinGetLayerByWinId( __u16 A_WinId, __u8* A_PtrRtSubIdx )
{
		*A_PtrRtSubIdx	=	(__u8)( 0xFF & A_WinId );
		return	(__u8)( 0xFF & (A_WinId >> 8) );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtBtnDown( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtBtnUp( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtConFcsChg( BOOL8 A_IsOn )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtSbarChg( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtSpinChg( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtSpinClk( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::RtlLastFocusObjSet( Cls_GrUiDoBase* A_Obj )
{
		m_FocusObj	=	A_Obj;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoWin::WinTypeGet( void )
{
		return	m_WinType;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoWin::LastFocusParaGet( __u8 A_Idx )
{
		return	m_FcsPara[A_Idx];
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::RtlLastFocusParaSet( __u8 A_Idx, __u32 A_Para )
{
		m_FcsPara[A_Idx]	=	A_Para;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtMseEnter( void )
{
		//not work
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtMseLeave( void )
{
		//Cls_GrUiDoSkin::ProcEvtMseLeave();
		m_IsMoving	=	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoWin::UpdateWinParam( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrUiDoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	m_ObjConsole->RtlWinRegist( this );
		if ( Tv_Result )
		{
			//do create window
			ProcEvtWinCreate();
			//check focus able
			if ( m_IsFocusAble )
			{
				//update focus
				Tv_Obj	=	FindFocusableSlave();
				if ( NULL != Tv_Obj )
				{
					m_ObjConsole->FocusObjSet( Tv_Obj );
				}
				else
				{
					if ( m_IsLayerFocusLock )
					{
						m_ObjConsole->FocusObjSet( this );
					}
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcBgColorDraw( void )
{
		if ( !m_IsBgSprite || (0 != m_ClrBg) )
		{
			if ( PaintClipperSet( TRUE ) )
			{
				m_GdibScr->FillRect( &m_PntRcClient, m_ClrBg );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcBgSkinTblDraw( void )
{
	// local -------------------
		__u32	Tv_WkIdx;
		Ptr_GrGdib	Tv_PtrGdib;
		St_GrRect		Tv_RcPnt;
		BOOL8	Tv_IsAble;
	// code --------------------
		//draw
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoSkinMaxCnt;Tv_WkIdx++ )
		{
			Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_BgSkinTbl[Tv_WkIdx].Id );
			if ( NULL != Tv_PtrGdib )
			{
				//set clipper
				switch( E_GrUiDoSkinPosValMask & m_BgSkinTbl[Tv_WkIdx].Atb )
				{
					case	E_GrUiDoSkinPosTileAll:
						Tv_IsAble	=	PaintClipperSet( TRUE );
						break;
					case	E_GrUiDoSkinPosTileLeft:
						Tv_RcPnt.left		=	m_PntRcScr.left;
						Tv_RcPnt.top		=	m_PntRcClient.top;
						Tv_RcPnt.right	=	m_PntRcScr.right;
						Tv_RcPnt.bottom	=	m_PntRcClient.bottom;
						Tv_IsAble	=	PaintClipperSet( FALSE, &Tv_RcPnt );
						break;
					case	E_GrUiDoSkinPosTileTop:
						Tv_RcPnt.left		=	m_PntRcClient.left;
						Tv_RcPnt.top		=	m_PntRcScr.top;
						Tv_RcPnt.right	=	m_PntRcClient.right;
						Tv_RcPnt.bottom	=	m_PntRcScr.bottom;
						Tv_IsAble	=	PaintClipperSet( FALSE, &Tv_RcPnt );
						break;
					case	E_GrUiDoSkinPosTileRight:
						Tv_RcPnt.left		=	m_PntRcScr.left;
						Tv_RcPnt.top		=	m_PntRcClient.top;
						Tv_RcPnt.right	=	m_PntRcScr.right;
						Tv_RcPnt.bottom	=	m_PntRcClient.bottom;
						Tv_IsAble	=	PaintClipperSet( FALSE, &Tv_RcPnt );
						break;
					case	E_GrUiDoSkinPosTileBottom:
						Tv_RcPnt.left		=	m_PntRcClient.left;
						Tv_RcPnt.top		=	m_PntRcScr.top;
						Tv_RcPnt.right	=	m_PntRcClient.right;
						Tv_RcPnt.bottom	=	m_PntRcScr.bottom;
						Tv_IsAble	=	PaintClipperSet( FALSE, &Tv_RcPnt );
						break;
					default:
						Tv_IsAble	=	PaintClipperSet( FALSE );
						break;
				}
				if ( Tv_IsAble )
				{
					SkinDrawByPos( m_BgSkinTbl[Tv_WkIdx].Atb, Tv_PtrGdib, &m_PntRcScr );
				}
			}
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoWin::RelateFrameIdGet( void )
{
		return	m_RelFrmId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::RelateFrameIdSet( __u16 A_Id )
{
		m_RelFrmId	=	A_Id;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtPageChg( Cls_GrUiDoBase* A_ObjFrm, __u16 A_PageId )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtPageFree( Cls_GrUiDoBase* A_ObjFrm, __u16 A_PageId )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtGrpSelCusChg( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtGrpSelSelect( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtEdtValChg( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtLbxItmClick( Cls_GrUiDoBase* A_Obj, __s32 A_ItemIdx )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtLbxTxtChg( Cls_GrUiDoBase* A_Obj, __s32 A_ItemIdx )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtCalendarDayChg( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtCalendarDaySel( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtGrpEditCusChg( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtGrpEditValChg( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtBitGridCusChg( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtBitGridValChg( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtInpTimeChg( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::ProcEvtQuadChg( Cls_GrUiDoBase* A_Obj )
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoWin::RtlModalResultSet( __u32* A_PtrRslt )
{
		m_PtrModalRslt	=	A_PtrRslt;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoWin::WindowModalShow( Cls_GrUiDoWin* A_Win )
{
	// local -------------------
		__u32	Tv_Result;
		BOOL8	Tv_IsEnable;
	// code --------------------
		Tv_Result	=	E_GrUiDoWinModalStanby;

		//disable this window
		Tv_IsEnable	=	IsEnable();		//enable stauts backup
		EnableSet( FALSE );

		A_Win->RtlModalResultSet( &Tv_Result );
		//do message pump
		while ( E_GrUiDoWinModalStanby == Tv_Result )
		{
			if ( !V_GrUiMngObj->UiMsgPump() )
			{
				V_GrUiMngObj->UiMsgRest();
			}
		}

		//enable window
		EnableSet( Tv_IsEnable );

		return	Tv_Result;
}
//--------------------------------------------------------------------
//void Cls_GrUiDoWin::ProcEvtMosaicGridValChg( Cls_GrUiDoBase* A_Obj )
//{
//}//new-120905~mosaic
//--------------------------------------------------------------------

