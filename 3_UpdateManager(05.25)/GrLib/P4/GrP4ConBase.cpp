/*
		platform console base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrDebug.h>
#include	<P4/GrP4ConBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4DoWin.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4Cfg.h>
#include	<P4/GrP4DoFrame.h>
#include <P4/GrP4ApiBase.h>

#ifdef GR_P4_EDITOR
#include	<P4/Dvl/GrP4DvlMng.h>
#endif

#include <GrTaskBase.h>
#include <GrTaskSvr.h>

//====================================================================
//local const

#define	E_GrP4ConCusShift		2


//====================================================================
//local type

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var

extern	Cls_GrP4Mng*	V_GrP4Mng;

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4ConBase::Cls_GrP4ConBase( BOOL8 A_IsCusSw )
{
	// local -------------------
		__u8	Tv_VdoChCnt;
	// code --------------------
		//init
		m_GdibScr		=	NULL;
		m_ObjOnMse	=	NULL;
		m_GdibDbl		=	NULL;

		m_PtrGdibCus	=	NULL;

		m_ObjFocus	=	NULL;

		m_WinSeq			=	0;
		m_PntSp				=	0;
		m_PntSlotCnt	=	0;

		m_QuadMode		=	E_GrDvrQuadMode1;
		m_QuadPage		=	0;

		m_QuadGridWidth	=	0;
		m_IsQuadGridView	=	TRUE;

		m_IsMseCap		=	FALSE;
		m_IsShutDown	=	FALSE;

		m_CusImgId		=	E_GrP4ImgIdNone;
		m_IsCusSw			=	A_IsCusSw;

		// init text paint
		m_PntTxtClrShadow			=	0;	// shadow color
		m_PntTxtGapPitch			=	0;		// pitch gap
		m_PntTxtGapLine				=	0;		// line gap
		m_PntTxtAlignHz				=	0;
		m_PntTxtAlignVt				=	0;		
		m_PntTxtAtb						=	0;
		m_PntTxtCus						=	0;				// cursor position
		m_IsPntTxtShadow			=	FALSE;		// paint text shadow - need update

		m_IsFocusCrtReq				=	FALSE;

		m_CusPosX							=	0;
		m_CusPosY							=	0;
		m_CusSizeX						=	0;
		m_CusSizeY						=	0;

		GrDumyZeroMem(&m_QuadRect, sizeof(m_QuadRect));

		Tv_VdoChCnt		=	V_GrP4Mng->VdoMaxChCntGet();
		m_QuadAbleMap	=	E_GrDvrQuadAbleMask1 | E_GrDvrQuadAbleMask4;
		if ( 8 <= Tv_VdoChCnt )
		{
			m_QuadAbleMap	=	m_QuadAbleMap | E_GrDvrQuadAbleMask6 | E_GrDvrQuadAbleMask8 | E_GrDvrQuadAbleMask9;
		}
		if ( 16 <= Tv_VdoChCnt )
		{
			m_QuadAbleMap	=	m_QuadAbleMap | E_GrDvrQuadAbleMask10 | E_GrDvrQuadAbleMask13 | E_GrDvrQuadAbleMask16;
		}

		GrDumyZeroMem( m_WinTbl, sizeof(m_WinTbl) );
		GrDumyZeroMem( m_PntSlotTbl, sizeof(m_PntSlotTbl) );

		m_PtrQuadChMap				=	V_GrP4Mng->SetupDataPtrGet( E_GrP4CfgOfsQuadMap );

		LcCusImgUpdt();			// cursor info update
}
//--------------------------------------------------------------------
Cls_GrP4ConBase::~Cls_GrP4ConBase( void )
{
	// local -------------------
	// code --------------------
		m_IsShutDown	=	TRUE;		// set shutdown mode
		// release all window
		WindowAllFree();
}
//--------------------------------------------------------------------
Cls_GrGdib*	Cls_GrP4ConBase::GdibGet( void )
{
		return	m_GdibScr;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4ConBase::MouseOnObjGet( void )
{
		return	m_ObjOnMse;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::MouseOnObjReset( void )
{
		m_ObjOnMse	=	NULL;
		m_IsMseCap	=	FALSE;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4ConBase::FocusKeyObjGet( void )
{
		return	m_ObjFocus;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::FocusReset( void )
{
		m_ObjFocus	=	NULL;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::FocusCorrect( __u8 A_BaseLayer )
{
	// local -------------------
		St_GrP4MsgHd	Tv_Msg;
		BOOL8	Tv_IsAble;
		Cls_GrP4DoWin*	Tv_Win;
	// code --------------------
		// check requested
		if ( !m_IsFocusCrtReq )
		{
			Tv_IsAble	=	TRUE;
			// check now 
			if ( 0 != A_BaseLayer )
			{
				if ( NULL != m_ObjFocus )
				{
					Tv_Win	=	(Cls_GrP4DoWin*)m_ObjFocus->WindowGet();
					if ( Tv_Win->LayerGet() >= A_BaseLayer )
					{
						// not need change
						Tv_IsAble	=	FALSE;
					}
				}
			}
			// check able
			if ( Tv_IsAble )
			{
				Tv_Msg.Cmd			=	E_GrP4MsgCmdConFocusCrt;
				Tv_Msg.ConIdx		=	E_GrP4ConIdxFocus;
				Tv_Msg.WinId		=	E_GrP4WinIdNone;
				Tv_Msg.ObjId		=	E_GrP4ScptIdNone;
				m_IsFocusCrtReq	=	UiMsgPost( &Tv_Msg, sizeof(Tv_Msg) );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4ConBase::UiMsgPost( Ptr_GrP4MsgHd A_PtrMsg, __u32 A_Size )
{
	// local -------------------
	// code --------------------
		// patch console index
		if ( E_GrP4ConIdxFocus == A_PtrMsg->ConIdx )
		{
			A_PtrMsg->ConIdx	=	m_ConIdx;
		}
		return	V_GrP4Mng->UiMsgPost( A_PtrMsg, A_Size );
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4ConBase::FocusObjGet( void )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	m_ObjFocus;
		if ( m_IsMseCusOn )
		{
			Tv_Result	=	NULL;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::FocusObjSet( Cls_GrP4DoBase* A_Obj )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
		BOOL8	Tv_IsAble;
	// code --------------------
		Tv_Obj		=	A_Obj;
		Tv_IsAble	=	FALSE;
		if ( m_ObjFocus != Tv_Obj )
		{
			Tv_IsAble	=	TRUE;
			if ( NULL != Tv_Obj )
			{
				// check target object is frame
				if ( E_GrP4DoClassFrame == Tv_Obj->ClassIdGet() )
				{
					Tv_Obj	=	((Cls_GrP4DoFrame*)Tv_Obj)->LastFocusObjGet();
				}
				if ( NULL != Tv_Obj )
				{
					Tv_IsAble	=	Tv_Obj->IsCanKey();
				}
			}
		}
		//check changed
		if ( Tv_IsAble )
		{
			//check old focus exist
			if ( NULL != m_ObjFocus )
			{
				//focus out event
				LcPostFocusLossMsg();
			}
			//set new focus
			m_ObjFocus	=	Tv_Obj;
			//check new object
			if ( NULL != m_ObjFocus )
			{
				//focus on event
				LcPostFocusGetMsg();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcPostFocusGetMsg( void )
{
	// local -------------------
		St_GrP4MsgHd		Tv_Msg;
		Cls_GrP4DoWin*		Tv_Win;
	// code --------------------
		Tv_Win					=	(Cls_GrP4DoWin*)m_ObjFocus->WindowGet();
		Tv_Msg.Cmd			=	E_GrP4MsgCmdFocusGet;
		Tv_Msg.ConIdx		=	m_ConIdx;
		Tv_Msg.WinId		=	Tv_Win->WinIdGet();
		Tv_Msg.ObjId		=	E_GrP4ScptIdNone;
		if ( !m_ObjFocus->IsWindow() )
		{
			Tv_Msg.ObjId	=	m_ObjFocus->ScptIdGet();
		}
		UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcPostFocusLossMsg( void )
{
	// local -------------------
		St_GrP4MsgHd	Tv_Msg;
		Cls_GrP4DoWin*	Tv_Win;
	// code --------------------
		// check exist focus object
		if ( NULL != m_ObjFocus )
		{
			//focus out event
			Tv_Win					=	(Cls_GrP4DoWin*)m_ObjFocus->WindowGet();
			Tv_Msg.Cmd			=	E_GrP4MsgCmdFocusLoss;
			Tv_Msg.ConIdx		=	m_ConIdx;
			Tv_Msg.WinId		=	Tv_Win->WinIdGet();
			Tv_Msg.ObjId		=	E_GrP4ScptIdNone;
			if ( !m_ObjFocus->IsWindow() )
			{
				Tv_Msg.ObjId	=	m_ObjFocus->ScptIdGet();
			}
			UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::PaintReq( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__u8		Tv_WkIdx;
		BOOL8		Tv_IsRpl;			// replace mode
		BOOL8		Tv_IsDid;			// did
	// code --------------------
		// init
		Tv_IsRpl	=	FALSE;
		// patch rect
		if ( A_PtrRect->left <	m_ScrRect.left )
		{
			A_PtrRect->left	=	m_ScrRect.left;
		}
		if ( A_PtrRect->top	< m_ScrRect.top )
		{
			A_PtrRect->top	=	m_ScrRect.top;
		}
		if ( A_PtrRect->right > m_ScrRect.right )
		{
			A_PtrRect->right	=	m_ScrRect.right;
		}
		if ( A_PtrRect->bottom > m_ScrRect.bottom )
		{
			A_PtrRect->bottom	=	m_ScrRect.bottom;
		}
		// check current rect
		if ( (A_PtrRect->left < A_PtrRect->right) && (A_PtrRect->top < A_PtrRect->bottom) )
		{
			Tv_IsDid	=	FALSE;
			// find already rect exist
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4ConPaintMaxCnt;Tv_WkIdx++ )
			{
				//check exist item
				if ( 0 != m_PntSlotTbl[Tv_WkIdx].right )
				{
					//check replace mode
					if ( Tv_IsRpl )
					{
						// check replace
						if (	(A_PtrRect->left <= m_PntSlotTbl[Tv_WkIdx].left) && (A_PtrRect->top <= m_PntSlotTbl[Tv_WkIdx].top) && 
									(A_PtrRect->right >= m_PntSlotTbl[Tv_WkIdx].right) && (A_PtrRect->bottom >= m_PntSlotTbl[Tv_WkIdx].bottom) )
						{
							// remove
							m_PntSlotTbl[Tv_WkIdx].right	=	0;
							m_PntSlotCnt --;
						}
					}
					else
					{
						// check corvered by paint
						if (	(A_PtrRect->left >= m_PntSlotTbl[Tv_WkIdx].left) && (A_PtrRect->top >= m_PntSlotTbl[Tv_WkIdx].top) && 
									(A_PtrRect->right <= m_PntSlotTbl[Tv_WkIdx].right) && (A_PtrRect->bottom <= m_PntSlotTbl[Tv_WkIdx].bottom) )
						{
							// corvered
							Tv_IsDid	=	TRUE;
							break;
						}
						else if ( (A_PtrRect->left <= m_PntSlotTbl[Tv_WkIdx].left) && (A_PtrRect->top <= m_PntSlotTbl[Tv_WkIdx].top) && 
											(A_PtrRect->right >= m_PntSlotTbl[Tv_WkIdx].right) && (A_PtrRect->bottom >= m_PntSlotTbl[Tv_WkIdx].bottom) )
						{
							// cover rect, replace other rect
							m_PntSlotTbl[Tv_WkIdx]	=	*A_PtrRect;
							Tv_IsDid	=	TRUE;
							Tv_IsRpl	=	TRUE;
						}
					}
				}
			}
			// check did
			if ( !Tv_IsDid )
			{
				// insert new area
				// check full
				if ( E_GrP4ConPaintMaxCnt <= m_PntSlotCnt )
				{
					// full mode
					m_PntSlotTbl[0]	=	m_ScrRect;
					m_PntSlotCnt		=	1;
					// delete other item
					for ( Tv_WkIdx=1;Tv_WkIdx < E_GrP4ConPaintMaxCnt;Tv_WkIdx++ )
					{
						m_PntSlotTbl[Tv_WkIdx].right	=	0;
					}
				}
				else
				{
					// find empty slot
					for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4ConPaintMaxCnt;Tv_WkIdx++ )
					{
						// check not use
						if ( 0 == m_PntSlotTbl[Tv_WkIdx].right )
						{
							// insert
							m_PntSlotTbl[Tv_WkIdx]	=	*A_PtrRect;
							m_PntSlotCnt ++;
							break;
						}
					}
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::PaintAllReq( void )
{
		PaintReq( &m_ScrRect );
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::RtlScptUpdt( void )
{
	// local -------------------
		__u8	Tv_VdoChCnt;
	// code --------------------
		LcCusImgUpdt();

		Tv_VdoChCnt		=	V_GrP4Mng->VdoMaxChCntGet();
		m_QuadAbleMap	=	E_GrDvrQuadAbleMask1 | E_GrDvrQuadAbleMask4;
		if ( 8 <= Tv_VdoChCnt )
		{
			m_QuadAbleMap	=	m_QuadAbleMap | E_GrDvrQuadAbleMask6 | E_GrDvrQuadAbleMask8 | E_GrDvrQuadAbleMask9;
		}
		if ( 16 <= Tv_VdoChCnt )
		{
			m_QuadAbleMap	=	m_QuadAbleMap | E_GrDvrQuadAbleMask10 | E_GrDvrQuadAbleMask13 | E_GrDvrQuadAbleMask16;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg )
{
	// local -------------------
	// code --------------------
		switch ( A_PtrMsg->Cmd )
		{
			case E_GrP4MsgCmdConInpMouse:
				LcProcMsgInpMouse( (Ptr_GrP4MsgMouse)A_PtrMsg );
				break;
			case E_GrP4MsgCmdConInpWheel:
				LcProcMsgInpWheel( (Ptr_GrP4MsgWheel)A_PtrMsg );
				break;
			case E_GrP4MsgCmdConWinClose:
				LcProcMsgWinClose( (Ptr_GrP4MsgWinClose)A_PtrMsg );
				break;
			case E_GrP4MsgCmdConInpKey:
				LcProcMsgInpKey( (Ptr_GrP4MsgKey)A_PtrMsg );
				break;
			case E_GrP4MsgCmdConFocusCrt:
				LcProcMsgFcsCrt();
				break;
			case E_GrP4MsgCmdSysStat:
				LcProcMsgSysStat( (Ptr_GrP4MsgSysStat)A_PtrMsg );
				break;
			case E_GrP4MsgCmdConLayUpClose:
				LcProcMsgLayUpClose( (Ptr_GrP4MsgLayer)A_PtrMsg );
				break;

		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcProcMsgFcsCrt( void )
{
	// local -------------------
		__s32	Tv_LayIdx;
		__s32	Tv_WinIdx;
		Cls_GrP4DoBase*	Tv_ObjFcs;
	// code --------------------
		// check focus is null
		if ( NULL == m_ObjFocus )
		{
			Tv_ObjFcs	=	NULL;
			for ( Tv_LayIdx = E_GrP4ConLayerMaxCnt - 1;Tv_LayIdx >= 0;Tv_LayIdx -- )
			{
				for ( Tv_WinIdx=0;Tv_WinIdx < E_GrP4ConLayerWinCnt;Tv_WinIdx++ )
				{
					//check exist win
					if ( NULL != m_WinTbl[Tv_LayIdx][Tv_WinIdx] )
					{
						//check can focus
						if ( m_WinTbl[Tv_LayIdx][Tv_WinIdx]->IsWinKeyAble() )
						{
							//exist last focus object
							Tv_ObjFcs	=	m_WinTbl[Tv_LayIdx][Tv_WinIdx]->LastFocusObjGet();
							if ( NULL != Tv_ObjFcs )
							{
								break;
							}
						}
					}
				}
				if ( NULL != Tv_ObjFcs )
				{
					break;
				}
			}
			//update
			FocusObjSet( Tv_ObjFcs );
		}
		// reset request
		m_IsFocusCrtReq	=	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcProcMsgInpKey( Ptr_GrP4MsgKey A_PtrMsg )
{
	// local -------------------
	// code --------------------
		// mouse cursor off
		MouseCusOnOff( FALSE );

		// check exist focus object
		if ( NULL != m_ObjFocus )
		{
			// check can focus
			if (m_ObjFocus->IsCanKey())
			{
				if (A_PtrMsg->IsPush)
				{
					m_ObjFocus->RtlKeyDownProc(A_PtrMsg->Dev, A_PtrMsg->Key);
				}
				else
				{
					m_ObjFocus->RtlKeyUpProc(A_PtrMsg->Dev, A_PtrMsg->Key);
				}
			}
			else
			{
				FocusReset();
				FocusCorrect();
			}
		}
		else
		{
			FocusCorrect();
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::RtlUiMsgProc( Ptr_GrP4MsgHd A_PtrMsg )
{
	// local -------------------
		__u8	Tv_LayIdx;
		__u8	Tv_WinIdx;
	// code --------------------
		// do tick
		if ( E_GrP4MsgCmdTickPeriod == A_PtrMsg->Cmd )
		{
			// do timer
			LcProcTimePeriod();
		}
		// check console message
		if ( E_GrP4WinIdNone == A_PtrMsg->WinId )
		{
			// console message
			LcProcUiMsg( A_PtrMsg );
		}
		else if ( E_GrP4WinIdAll == A_PtrMsg->WinId )
		{
			// broad cast window
			for ( Tv_LayIdx=0;Tv_LayIdx < E_GrP4ConLayerMaxCnt;Tv_LayIdx++ )
			{
				for ( Tv_WinIdx=0;Tv_WinIdx < E_GrP4ConLayerWinCnt;Tv_WinIdx++ )
				{
					// check exist window
					if ( NULL != m_WinTbl[Tv_LayIdx][Tv_WinIdx] )
					{
						// check window message
						if ( E_GrP4ScptIdNone != A_PtrMsg->ObjId )
						{
							m_WinTbl[Tv_LayIdx][Tv_WinIdx]->RtlUiMsgProc( A_PtrMsg );
						}
						// window message
						if ( (E_GrP4ScptIdNone == A_PtrMsg->ObjId) || (E_GrP4ScptIdAll == A_PtrMsg->ObjId) )
						{
							m_WinTbl[Tv_LayIdx][Tv_WinIdx]->ProcUiMsg( A_PtrMsg );
						}
					}
				}
			}
		}
		else
		{
			// one window message
			GrP4MngGetWinIdInfo( A_PtrMsg->WinId, &Tv_LayIdx, &Tv_WinIdx );
			// check exist window
			if ( NULL != m_WinTbl[Tv_LayIdx][Tv_WinIdx] )
			{
				// check window message
				if ( E_GrP4ScptIdNone != A_PtrMsg->ObjId )
				{
					m_WinTbl[Tv_LayIdx][Tv_WinIdx]->RtlUiMsgProc( A_PtrMsg );
				}
				if ( (E_GrP4ScptIdNone == A_PtrMsg->ObjId) || (E_GrP4ScptIdAll == A_PtrMsg->ObjId) )
				{
					m_WinTbl[Tv_LayIdx][Tv_WinIdx]->ProcUiMsg( A_PtrMsg );
				}

			}
		}
		
}
//--------------------------------------------------------------------
__u32	Cls_GrP4ConBase::GetResX( void )
{
		return	m_ScrRect.right - m_ScrRect.left;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4ConBase::GetResY( void )
{
		return	m_ScrRect.bottom - m_ScrRect.top;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::MouseCusOnOff( BOOL8 A_IsOn )
{
	// local -------------------
		BOOL8	Tv_IsOld;
		__u16	Tv_X;
		__u16	Tv_Y;
		St_GrRect	Tv_RcPnt;
	// code --------------------
		Tv_IsOld			=	m_IsMseCusOn;		//backup old
		m_IsMseCusOn	=	A_IsOn;					//update
		//check change to on
		if ( m_IsMseCusOn )
		{
			//check on
			if ( !Tv_IsOld )
			{
				//redraw focused object
				if ( NULL != m_ObjFocus )
				{
					//focus leave message
					LcPostFocusLossMsg();
				}
				//get mouse position
				//V_GrP4Mng->MousePosGet( &Tv_X, &Tv_Y );
				Tv_X	=	(__u16)m_CusPosX;
				Tv_Y	=	(__u16)m_CusPosY;
				//update mouse object
				LcMseObjUpdt( Tv_X, Tv_Y );
			}
		}
		else
		{
			//check change
			if ( Tv_IsOld )
			{
				if ( NULL != m_ObjFocus )
				{
					LcPostFocusGetMsg();
				}
			}
			//mouse object free
			LcMseObjFree();
		}
		//update hardware mouse cursor
		if ( Tv_IsOld != m_IsMseCusOn )
		{
			// software cursor
			if ( E_GrP4ImgIdNone != m_CusImgId )
			{
				Tv_RcPnt.left		=	m_CusPosX;
				Tv_RcPnt.top		=	m_CusPosY;
				Tv_RcPnt.right	=	m_CusPosX + m_CusSizeX;
				Tv_RcPnt.bottom	=	m_CusPosY + m_CusSizeY;
				PaintReq( &Tv_RcPnt );
			}
			HalMouseCusOnOff();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::MouseCusPosSet( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		St_GrRect	Tv_RcPnt;
	// code --------------------
		// soft cursor draw
		if ( m_IsCusSw && m_IsMseCusOn && (E_GrP4ImgIdNone != m_CusImgId) )
		{
			// double buffering
			if ( m_IsMseCusOn && (NULL != m_GdibDbl) && (NULL != m_PtrGdibCus) )
			{
				m_GdibDbl->SpriteModeSet( FALSE );
				// paint old position
				Tv_RcPnt.left		=	m_CusPosX;
				Tv_RcPnt.top		=	m_CusPosY;
				Tv_RcPnt.right	=	m_CusPosX + m_CusSizeX;
				Tv_RcPnt.bottom	=	m_CusPosY + m_CusSizeY;
				m_GdibDbl->BltGdib( Tv_RcPnt.left, Tv_RcPnt.top, m_GdibScr->GetGdibPtr(), &Tv_RcPnt );
				HalPntReqTransfer( &Tv_RcPnt );		// do hardware transfer
				// paint new position
				Tv_RcPnt.left		=	A_X;
				Tv_RcPnt.top		=	A_Y;
				Tv_RcPnt.right	=	A_X + m_CusSizeX;
				Tv_RcPnt.bottom	=	A_Y + m_CusSizeY;
				m_GdibDbl->BltGdib( Tv_RcPnt.left, Tv_RcPnt.top, m_GdibScr->GetGdibPtr(), &Tv_RcPnt );
				// draw cursor
				m_GdibDbl->SpriteModeSet( TRUE );
				m_GdibDbl->BltGdib( A_X, A_Y, m_PtrGdibCus );
				HalPntReqTransfer( &Tv_RcPnt );		// do hardware transfer
			}
			else
			{
				// paint old position
				Tv_RcPnt.left		=	m_CusPosX;
				Tv_RcPnt.top		=	m_CusPosY;
				Tv_RcPnt.right	=	m_CusPosX + m_CusSizeX;
				Tv_RcPnt.bottom	=	m_CusPosY + m_CusSizeY;
				PaintReq( &Tv_RcPnt );
				// paint new position
				Tv_RcPnt.left		=	A_X;
				Tv_RcPnt.top		=	A_Y;
				Tv_RcPnt.right	=	A_X + m_CusSizeX;
				Tv_RcPnt.bottom	=	A_Y + m_CusSizeY;
				PaintReq( &Tv_RcPnt );
			}
		}

		// update value
		m_CusPosX			=	A_X;
		m_CusPosY			=	A_Y;
		
		// h/w operation
		HalMousePosSet( A_X, A_Y );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4ConBase::HalPntReqTransfer( Ptr_GrRect A_PtrRect )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::HalMouseCusOnOff( void )
{
		// hardware implement
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcProcMsgInpMouse( Ptr_GrP4MsgMouse A_PtrMsg )
{
	// local -------------------
	// code --------------------
		// reset mouse suspend time
		m_MseSusTime	=	0;
		// mouse cursor on
		MouseCusOnOff( TRUE );
		// mouse position set
		MouseCusPosSet( A_PtrMsg->X, A_PtrMsg->Y );
		// update mouse object
		LcMseObjUpdt( A_PtrMsg->X, A_PtrMsg->Y );
		// send object to message
		if ( NULL != m_ObjOnMse )
		{
			if (0 != (E_GrP4MouseAtbPosChg & ((Ptr_GrP4MsgMouse)A_PtrMsg)->Atb))
			{
				//mouse move
				m_ObjOnMse->RtlMseMoveProc(A_PtrMsg->X,A_PtrMsg->Y,A_PtrMsg->BtnMap);
			}
			else if (0 != (E_GrP4MouseAtbLbtnChg & ((Ptr_GrP4MsgMouse)A_PtrMsg)->Atb))
			{
				if (0 != (E_GrP4MouseBmkLeft & ((Ptr_GrP4MsgMouse)A_PtrMsg)->BtnMap))
				{
					m_ObjOnMse->RtlMseLbtnDownProc(A_PtrMsg->X, A_PtrMsg->Y, A_PtrMsg->BtnMap);
				}
				else
				{
					m_ObjOnMse->RtlMseLbtnUpProc(A_PtrMsg->X, A_PtrMsg->Y, A_PtrMsg->BtnMap);
				}
			}
			else if (0 != (E_GrP4MouseAtbRbtnChg & ((Ptr_GrP4MsgMouse)A_PtrMsg)->Atb))
			{
				if (0 != (E_GrP4MouseBmkRight & ((Ptr_GrP4MsgMouse)A_PtrMsg)->BtnMap))
				{
					m_ObjOnMse->RtlMseRbtnDownProc(A_PtrMsg->X, A_PtrMsg->Y, A_PtrMsg->BtnMap);
				}
				else
				{
					m_ObjOnMse->RtlMseRbtnUpProc(A_PtrMsg->X, A_PtrMsg->Y, A_PtrMsg->BtnMap);
				}
			}
			else if (0 != (E_GrP4MouseAtbMbtnChg & ((Ptr_GrP4MsgMouse)A_PtrMsg)->Atb))
			{
				if (0 != (E_GrP4MouseBmkMiddle & ((Ptr_GrP4MsgMouse)A_PtrMsg)->BtnMap))
				{
					m_ObjOnMse->RtlMseMbtnDownProc(A_PtrMsg->X, A_PtrMsg->Y, A_PtrMsg->BtnMap);
				}
				else
				{
					m_ObjOnMse->RtlMseMbtnUpProc(A_PtrMsg->X, A_PtrMsg->Y, A_PtrMsg->BtnMap);
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcProcMsgInpWheel( Ptr_GrP4MsgWheel A_PtrMsg )
{
	// local -------------------
	// code --------------------
		// reset mouse suspend time
		m_MseSusTime	=	0;
		// mouse cursor on
		MouseCusOnOff( TRUE );
		// send object to message
		if ( NULL != m_ObjOnMse )
		{
			if (A_PtrMsg->IsDown)
			{
				m_ObjOnMse->RtlWheelDownProc();
			}
			else
			{
				m_ObjOnMse->RtlWheelUpProc();
			}

		}
}
//--------------------------------------------------------------------
Ptr_GrP4ConPntStackItm	Cls_GrP4ConBase::PaintStackGetPushPtr( void )
{
	// local -------------------
		Ptr_GrP4ConPntStackItm	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( m_IsPstCanPush )
		{
			// check stack full
			if ( E_GrP4ConPntStackCnt > m_PntSp )
			{
				Tv_Result	=	&m_PntStack[m_PntSp];
			}
			else
			{
				DbgMsgPrint("Cls_GrP4ConBase::PaintStackGetPushPtr - paint stack is full!\n");
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::PaintStackPush( void )
{
	// local -------------------
	// code --------------------
		if ( m_IsPstCanPush )
		{
			if ( E_GrP4ConPntStackCnt > m_PntSp )
			{
				// update continue able
				if (	(E_GrP4ScptPstTypeClrFill == m_PntStack[m_PntSp].Type) || 
							( (E_GrP4ScptPstTypeImage == m_PntStack[m_PntSp].Type) && (1 == m_PntStack[m_PntSp].Para0) )	
						)
				{
					// check rect corver paint request
					if (	(m_PntStack[m_PntSp].Rect.left <= m_PtrPntRect->left) && (m_PntStack[m_PntSp].Rect.right >= m_PtrPntRect->right) && 
								(m_PntStack[m_PntSp].Rect.top <= m_PtrPntRect->top) && (m_PntStack[m_PntSp].Rect.bottom >= m_PtrPntRect->bottom)
						 )
					{
						// do not need paint continue
						m_IsPstCanPush	=	FALSE;
					}
				}
				// stack pointer increase
				m_PntSp ++;
			}
			else
			{
				DbgMsgPrint("Cls_GrP4ConBase::PaintStackPush - paint stack is already full!\n");
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4ConBase::IsPstCanPush( void )
{
		return	m_IsPstCanPush;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::PstPushBlock( void )
{
		m_IsPstCanPush	=	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcSoftCusPaint( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		St_GrRect	Tv_RcCus;
		Ptr_GrP4ConPntStackItm	Tv_PtrItm;
	// code --------------------
		// check area
		Tv_RcCus.left		=	m_CusPosX;
		Tv_RcCus.top		=	m_CusPosY;
		Tv_RcCus.right	=	m_CusPosX + m_CusSizeX;
		Tv_RcCus.bottom	=	m_CusPosY + m_CusSizeY;
		// patch cursor area
		if ( Tv_RcCus.left > A_PtrRect->left )
		{
			Tv_RcCus.left	=	A_PtrRect->left;
		}
		if ( Tv_RcCus.right < A_PtrRect->right )
		{
			Tv_RcCus.right	=	A_PtrRect->right;
		}
		if ( Tv_RcCus.top > A_PtrRect->top )
		{
			Tv_RcCus.top	=	A_PtrRect->top;
		}
		if ( Tv_RcCus.bottom < A_PtrRect->bottom )
		{
			Tv_RcCus.bottom	=	A_PtrRect->bottom;
		}
		// check bad area
		if ( (Tv_RcCus.left < Tv_RcCus.right) && (Tv_RcCus.top < Tv_RcCus.bottom) )
		{
			// paint
			Tv_PtrItm	=	PaintStackGetPushPtr();
			if ( NULL != Tv_PtrItm )
			{
				Tv_PtrItm->Rect.left		=	m_CusPosX;
				Tv_PtrItm->Rect.top			=	m_CusPosY;
				Tv_PtrItm->Rect.right		=	m_CusPosX + m_CusSizeX;
				Tv_PtrItm->Rect.bottom	=	m_CusPosY + m_CusSizeY;
				Tv_PtrItm->Type		=	E_GrP4ScptPstTypeImage;
				Tv_PtrItm->Para0	=	E_GrP4ScptPstImgAtbSprite;		// sprite attribute
				Tv_PtrItm->Para1	=	0;
				Tv_PtrItm->Value	=	(__u32)m_CusImgId;
				PaintStackPush();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcPaintByRect( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__u8	Tv_LayIdx;
		__u8	Tv_WinIdx;
	// code --------------------
		// init
		m_PtrPntRect		=	A_PtrRect;
		m_IsPstCanPush	=	TRUE;
		m_PntSp					=	0;		// reset paint stack

#ifdef GR_P4_EDITOR
		if ( ((Cls_GrP4DvlMng*)V_GrP4Mng)->DvlIsDbgRun() )
		{
#endif
		// soft cursor
		if ( m_IsCusSw && m_IsMseCusOn && (NULL == m_GdibDbl) && (E_GrP4ImgIdNone != m_CusImgId) )
		{
			LcSoftCusPaint( A_PtrRect );
		}
#ifdef GR_P4_EDITOR
		}
#endif

		Tv_LayIdx	=	E_GrP4ConLayerMaxCnt;
		while (0 != Tv_LayIdx)
		{
			Tv_LayIdx --;
			// do paint window
			for ( Tv_WinIdx=0;Tv_WinIdx < E_GrP4ConLayerWinCnt;Tv_WinIdx++ )
			{
				// check exsit window
				if ( NULL != m_WinTbl[Tv_LayIdx][Tv_WinIdx] )
				{
					m_WinTbl[Tv_LayIdx][Tv_WinIdx]->RtlPaint( m_PtrPntRect );
					// check not need continue
					if ( !m_IsPstCanPush )
					{
						break;
					}
				}
			}
			// check not need continue
			if ( !m_IsPstCanPush )
			{
				break;
			}
		}
		LcDoPaintStack( A_PtrRect );
#ifdef GR_P4_EDITOR
		if ( 0 != m_PntSp )
		{
			DbgMsgPrint( "Cls_GrP4ConBase::LcPaintByRect - paint stack remained.\n" );
		}
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcPaintBySlot( void )
{
	// local -------------------
		__u32	Tv_SlotIdx;
	// code --------------------
		m_PntFont	=	NULL;		// reset font
		// check exist paint slot
		if ( (NULL != m_GdibScr) && (0 != m_PntSlotCnt) )
		{
			// find possible item
			Tv_SlotIdx	=	0;
			while ( (E_GrP4ConPaintMaxCnt > Tv_SlotIdx) && (0 != m_PntSlotCnt) )
			{
				// check exist item
				if ( 0 != m_PntSlotTbl[Tv_SlotIdx].right )
				{
					// patch rect
					/*
					if ( m_PntSlotTbl[Tv_SlotIdx].left < m_ScrRect.left )
					{
						m_PntSlotTbl[Tv_SlotIdx].left	=	m_ScrRect.left;
					}
					if ( m_PntSlotTbl[Tv_SlotIdx].top < m_ScrRect.top )
					{
						m_PntSlotTbl[Tv_SlotIdx].top	=	m_ScrRect.top;
					}
					if ( m_PntSlotTbl[Tv_SlotIdx].right > m_ScrRect.right )
					{
						m_PntSlotTbl[Tv_SlotIdx].right	=	m_ScrRect.right;
					}
					if ( m_PntSlotTbl[Tv_SlotIdx].bottom > m_ScrRect.bottom )
					{
						m_PntSlotTbl[Tv_SlotIdx].bottom	=	m_ScrRect.bottom;
					}
					*/
					// check able rect
					/*
					if ( (m_PntSlotTbl[Tv_SlotIdx].left < m_PntSlotTbl[Tv_SlotIdx].right) && (m_PntSlotTbl[Tv_SlotIdx].top < m_PntSlotTbl[Tv_SlotIdx].bottom) )
					{
					*/
						// rect clipping
						m_GdibScr->ClipperSet( &m_PntSlotTbl[Tv_SlotIdx] );
						// back ground clear - editor only
#ifdef GR_P4_EDITOR
						m_GdibScr->FillRect( &m_PntSlotTbl[Tv_SlotIdx], 0 );
#endif
						LcPaintByRect( &m_PntSlotTbl[Tv_SlotIdx] );
						// double bufer
						if ( NULL != m_GdibDbl )
						{
							m_GdibDbl->SpriteModeSet( FALSE );
							m_GdibDbl->BltGdib( m_PntSlotTbl[Tv_SlotIdx].left, m_PntSlotTbl[Tv_SlotIdx].top, 
								m_GdibScr->GetGdibPtr(), &m_PntSlotTbl[Tv_SlotIdx] );
						}
						HalPntReqTransfer( &m_PntSlotTbl[Tv_SlotIdx] );		// do hardware transfer
						/*
					}
					*/
					// delete slot
					m_PntSlotTbl[Tv_SlotIdx].right	=	0;
					m_PntSlotCnt --;
				}
				Tv_SlotIdx ++;
			}
			// reset paint slot
			if ( 0 != m_PntSlotCnt )
			{
				DbgMsgPrint("Cls_GrP4ConBase::LcPaintBySlot - paint slot count ( %d ) mismatch\n", m_PntSlotCnt );
				m_PntSlotCnt	=	0;
			}
			
		}
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4ConBase::ObjFindMseByPos( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
		BOOL8	Tv_IsLayLock;
		__s32		Tv_LayIdx;
		__u8	Tv_WinIdx;
		Cls_GrP4DoWin*	Tv_Win;
	// code --------------------
		Tv_Result			=	NULL;
		Tv_IsLayLock	=	FALSE;
		// find layer
		for ( Tv_LayIdx=E_GrP4ConLayerMaxCnt-1;Tv_LayIdx >= 0;Tv_LayIdx-- )
		{
			// find window
			for ( Tv_WinIdx=0;Tv_WinIdx < E_GrP4ConLayerWinCnt;Tv_WinIdx++  )
			{
				// check exist win
				if ( NULL != m_WinTbl[Tv_LayIdx][Tv_WinIdx] )
				{
					Tv_Win	=	m_WinTbl[Tv_LayIdx][Tv_WinIdx];
					// find positino item
					Tv_Result	=	Tv_Win->FindMseObjByPos( A_X, A_Y );
					if ( NULL != Tv_Result )
					{
						break;
					}
					if ( Tv_Win->IsWinLayerLock() )
					{
						Tv_IsLayLock	=	TRUE;
						Tv_Result			=	(Cls_GrP4DoBase*)Tv_Win;
						break;
					}
				}
			}
			// layer lock
			if ( Tv_IsLayLock || (NULL != Tv_Result) )
			{
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4ConBase::ObjFindByPos( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
		__s32		Tv_LayIdx;
		__u8	Tv_WinIdx;
		Cls_GrP4DoWin*	Tv_Win;
	// code --------------------
		Tv_Result			=	NULL;
		// find layer
		for ( Tv_LayIdx=E_GrP4ConLayerMaxCnt-1;Tv_LayIdx >= 0;Tv_LayIdx-- )
		{
			// find window
			for ( Tv_WinIdx=0;Tv_WinIdx < E_GrP4ConLayerWinCnt;Tv_WinIdx++  )
			{
				// check exist win
				if ( NULL != m_WinTbl[Tv_LayIdx][Tv_WinIdx] )
				{
					Tv_Win	=	m_WinTbl[Tv_LayIdx][Tv_WinIdx];
					// find positino item
					Tv_Result	=	Tv_Win->FindObjByPos( A_X, A_Y );
					if ( NULL != Tv_Result )
					{
						break;
					}
				}
			}
			// layer lock
			if ( NULL != Tv_Result )
			{
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4ConBase::ObjListByPos( __s32 A_X, __s32 A_Y, __u32* A_PtrBuf )
{
	// local -------------------
		__u32	Tv_Result;
		__s32		Tv_LayIdx;
		__u8	Tv_WinIdx;
		Cls_GrP4DoWin*	Tv_Win;
	// code --------------------
		Tv_Result	=	0;

		for ( Tv_LayIdx=E_GrP4ConLayerMaxCnt-1;Tv_LayIdx >= 0;Tv_LayIdx-- )
		{
			// find window
			for ( Tv_WinIdx=0;Tv_WinIdx < E_GrP4ConLayerWinCnt;Tv_WinIdx++  )
			{
				// check exist win
				if ( NULL != m_WinTbl[Tv_LayIdx][Tv_WinIdx] )
				{
					Tv_Win	=	m_WinTbl[Tv_LayIdx][Tv_WinIdx];
					// find positino item
					Tv_Win->RtlListObjByPos( A_X, A_Y, &m_ScrRect, &Tv_Result, A_PtrBuf );
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcMseObjUpdt( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
		Cls_GrP4DoWin*	Tv_Win;
		St_GrP4MsgHd		Tv_Msg;
	// code --------------------
		// check mouse capture
		if ( (!m_IsMseCap) || (NULL == m_ObjOnMse) )
		{
			// find object
			Tv_Obj	=	ObjFindMseByPos( A_X, A_Y );
			if ( Tv_Obj != m_ObjOnMse )
			{
				// mouse object release
				LcMseObjFree();
				// new regist mouse object
				m_ObjOnMse	=	Tv_Obj;
				if ( NULL != m_ObjOnMse )
				{
					// mouse enter message
					// leave focus message 
					Tv_Win	=	(Cls_GrP4DoWin*)m_ObjOnMse->WindowGet();
					Tv_Msg.Cmd		=	E_GrP4MsgCmdMouseEnter;
					Tv_Msg.ConIdx	=	E_GrP4ConIdxFocus;
					Tv_Msg.WinId	=	Tv_Win->WinIdGet();
					Tv_Msg.ObjId	=	E_GrP4ScptIdNone;
					if ( !m_ObjOnMse->IsWindow() )
					{
						Tv_Msg.ObjId	=	m_ObjOnMse->ScptIdGet();
					}
					UiMsgPost( &Tv_Msg, sizeof(Tv_Msg) );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcMseObjFree( void )
{
	// local -------------------
		Cls_GrP4DoWin*	Tv_Win;
		St_GrP4MsgHd		Tv_Msg;
	// code --------------------
		if ( NULL != m_ObjOnMse )
		{
			// leave focus message 
			Tv_Win	=	(Cls_GrP4DoWin*)m_ObjOnMse->WindowGet();
			Tv_Msg.Cmd		=	E_GrP4MsgCmdMouseLeave;
			Tv_Msg.ConIdx	=	E_GrP4ConIdxFocus;
			Tv_Msg.WinId	=	Tv_Win->WinIdGet();
			Tv_Msg.ObjId	=	E_GrP4ScptIdNone;
			if ( !m_ObjOnMse->IsWindow() )
			{
				Tv_Msg.ObjId	=	m_ObjOnMse->ScptIdGet();
			}
			UiMsgPost( &Tv_Msg, sizeof(Tv_Msg) );
			// release object
			m_ObjOnMse	=	NULL;
			m_IsMseCap	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcMseHideUpdt( void )
{
	// local -------------------
	// code --------------------
		if ( m_IsMseCusOn && (!m_IsMseCap) )
		{
			m_MseSusTime ++;
			if ( E_GrP4ConMseSusTimeOut < m_MseSusTime )
			{
				MouseCusOnOff( FALSE );
				m_MseSusTime	=	0;
			}
		}
		/*
		else
		{
			m_MseSusTime	=	0;
		}
		*/
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcProcTimePeriod( void )
{
	// local -------------------
	// code --------------------
		// update paint
		LcPaintBySlot();
		// update auto mouse cursor off
		LcMseHideUpdt();
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4ConBase::DvlDispSetup( void )
{
	// local -------------------
	// code --------------------
		
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
Cls_GrGdib*	Cls_GrP4ConBase::DvlGdibOutGet( void )
{
		return	NULL;
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::IndexSet( __u8 A_Idx )
{
		m_ConIdx	=	A_Idx;
}
//--------------------------------------------------------------------
__u8	Cls_GrP4ConBase::IndexGet( void )
{
		return	m_ConIdx;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::WindowAllFree( void )
{
	// local -------------------
		__u8	Tv_LayIdx;
		__u8	Tv_WkIdx;
		__u8	Tv_Layer;
	// code --------------------
		for ( Tv_LayIdx=0;Tv_LayIdx < E_GrP4ConLayerMaxCnt;Tv_LayIdx++ )
		{
			for (Tv_WkIdx=0;Tv_WkIdx < E_GrP4ConLayerWinCnt;Tv_WkIdx++)
			{
				Tv_Layer	=	E_GrP4ConLayerMaxCnt - Tv_LayIdx - 1;
				if ( NULL != m_WinTbl[Tv_Layer][Tv_WkIdx] )
				{
					delete	m_WinTbl[Tv_Layer][Tv_WkIdx];
					m_WinTbl[Tv_Layer][Tv_WkIdx]	=	NULL;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::WindowFree( __u16 A_WinId )
{
	// local -------------------
		__u16	Tv_Layer;
		__u16	Tv_Slot;
	// code --------------------
		if ( LcFindWinByWinId( A_WinId, &Tv_Layer, &Tv_Slot ) )
		{
			delete	m_WinTbl[Tv_Layer][Tv_Slot];
			m_WinTbl[Tv_Layer][Tv_Slot]	=	NULL;
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP4ConBase::RtlWinRegist( Cls_GrP4DoWin* A_Win, __u8 A_Layer )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_SlotIdx;
	// code --------------------
		Tv_Result	=	E_GrP4WinIdNone;
		if ( E_GrP4ConLayerMaxCnt > A_Layer )
		{
			//find empty slot
			for (Tv_SlotIdx=0;Tv_SlotIdx < E_GrP4ConLayerWinCnt;Tv_SlotIdx++)
			{
				if ( NULL == m_WinTbl[A_Layer][Tv_SlotIdx] )
				{
					// found
					m_WinTbl[A_Layer][Tv_SlotIdx]	=	A_Win;
					// make window id
					m_WinSeq ++;
					Tv_Result	=	(__u16)m_WinSeq | (((__u16)A_Layer) << E_GrP4WinIdLayerBitPos) | (Tv_SlotIdx << E_GrP4WinIdSlotBitPos);
					break;
				}
			}
			if ( E_GrP4WinIdNone == Tv_Result )
			{
				DbgMsgPrint( "Cls_GrP4ConBase::RtlWinRegist - layer is full!\n" );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP4ConBase::RtlWinRegist - bad layer ( %d ) requested!\n", (__s32)A_Layer );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::RtlAsmStatEvt( __u8 A_Stat, __u8 A_BpIdx, __u8 A_VmIdx )
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4ConBase::LcFindWinByWinId( __u16 A_WinId, __u16* A_PtrRtLayer, __u16* A_PtrRtSlot )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Layer;
		__u16	Tv_Slot;
		Cls_GrP4DoWin*	Tv_Win;
	// code --------------------
		Tv_Result	=	FALSE;
		// check window id
		Tv_Layer	=	(A_WinId >> E_GrP4WinIdLayerBitPos) & E_GrP4WinIdLayerBitMask;
		Tv_Slot		=	(A_WinId >> E_GrP4WinIdSlotBitPos) & E_GrP4WinIdSlotBitMask;
		if ( NULL != m_WinTbl[Tv_Layer][Tv_Slot] )
		{
			Tv_Win	=	m_WinTbl[Tv_Layer][Tv_Slot];
			if ( A_WinId == Tv_Win->WinIdGet() )
			{
				*A_PtrRtLayer	=	Tv_Layer;
				*A_PtrRtSlot	=	Tv_Slot;
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP4DoWin*	Cls_GrP4ConBase::WinFindByWinId( __u16 A_WinId )
{
	// local -------------------
		Cls_GrP4DoWin*	Tv_Result;
		__u16	Tv_Layer;
		__u16	Tv_Slot;
	// code --------------------
		Tv_Result	=	NULL;
		if ( LcFindWinByWinId( A_WinId, &Tv_Layer, &Tv_Slot ) )
		{
			Tv_Result	=	m_WinTbl[Tv_Layer][Tv_Slot];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP4DoWin*	Cls_GrP4ConBase::WinFindByScptId( __u16 A_ScptId )
{
	// local -------------------
		Cls_GrP4DoWin*	Tv_Result;
		__u16	Tv_Layer;
		__u16	Tv_Slot;
	// code --------------------
		Tv_Result	=	NULL;
		for ( Tv_Layer=0;Tv_Layer < E_GrP4ConLayerMaxCnt;Tv_Layer++ )
		{
			for ( Tv_Slot=0;Tv_Slot < E_GrP4ConLayerWinCnt;Tv_Slot++ )
			{
				if ( NULL != m_WinTbl[Tv_Layer][Tv_Slot] )
				{
					if ( A_ScptId == m_WinTbl[Tv_Layer][Tv_Slot]->ScptIdGet() )
					{
						Tv_Result	=	m_WinTbl[Tv_Layer][Tv_Slot];
						break;
					}
				}
			}
			if ( NULL != Tv_Result )
			{
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcDoWinClose( __u16 A_Layer, __u16 A_Slot )
{
	// local -------------------
	// code --------------------
		if ( NULL != m_WinTbl[A_Layer][A_Slot] )
		{
			// release
			delete	m_WinTbl[A_Layer][A_Slot];
			m_WinTbl[A_Layer][A_Slot]	=	NULL;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcProcMsgWinClose( Ptr_GrP4MsgWinClose A_PtrMsg )
{
	// local -------------------
		__u16	Tv_Layer;
		__u16	Tv_Slot;
	// code --------------------
		// find window
		if ( LcFindWinByWinId( A_PtrMsg->WinId, &Tv_Layer, &Tv_Slot ) )
		{
			LcDoWinClose( Tv_Layer, Tv_Slot );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcProcMsgLayUpClose( Ptr_GrP4MsgLayer A_PtrMsg )
{
	// local -------------------
		__u8	Tv_Layer;
		__u16	Tv_Slot;
	// code --------------------
		Tv_Layer	=	A_PtrMsg->Layer;
		while ( E_GrP4ConLayerMaxCnt > Tv_Layer ) 
		{
			for ( Tv_Slot=0;Tv_Slot < E_GrP4ConLayerWinCnt;Tv_Slot++ )
			{
				LcDoWinClose( (__u16)Tv_Layer, Tv_Slot );
			}
			// next
			Tv_Layer ++;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcDoPstLine( void )
{
	// local -------------------
		__s32		Tv_StX;
		__s32		Tv_StY;
		__s32		Tv_EdX;
		__s32		Tv_EdY;
		__s32		Tv_AddSt;
		__s32		Tv_AddEd;
		__u8	Tv_Atb;
	// code --------------------
		// check box
		if ( 0 == m_PntStack[m_PntSp].Para1 )
		{
			// box
			m_GdibScr->FillRect( &m_PntStack[m_PntSp].Rect, m_PntStack[m_PntSp].Value );
		}
		else
		{
			// line
			Tv_StX	=	m_PntStack[m_PntSp].Rect.left;
			Tv_StY	=	m_PntStack[m_PntSp].Rect.top;
			Tv_AddSt	=	0;
			Tv_AddEd	=	0;
			if ( 0 == m_PntStack[m_PntSp].Para0 )
			{
				// horizontal line
				Tv_EdX	=	m_PntStack[m_PntSp].Rect.right - 1;
				// left
				Tv_Atb	=	(m_PntStack[m_PntSp].Para1 >> 4) & 0x0F;
				if ( 1 == Tv_Atb )
				{
					Tv_AddSt	=	1;
				}
				else if ( 2 == Tv_Atb )
				{
					Tv_AddSt	=	-1;
					Tv_StX		=	Tv_StX + (m_PntStack[m_PntSp].Rect.bottom - m_PntStack[m_PntSp].Rect.top);
				}
				// right
				Tv_Atb	=	m_PntStack[m_PntSp].Para1 & 0x0F;
				if ( 1 == Tv_Atb )
				{
					Tv_AddEd	=	-1;
				}
				else if ( 2 == Tv_Atb )
				{
					Tv_AddEd	=	1;
					Tv_EdX	=	Tv_EdX - (m_PntStack[m_PntSp].Rect.bottom - m_PntStack[m_PntSp].Rect.top) + 1;
				}
				// draw
				do 
				{
					// check over position
					if ( Tv_StX >= Tv_EdX )
					{
						break;
					}
					m_GdibScr->DrawLine( Tv_StX, Tv_StY, Tv_EdX, Tv_StY, m_PntStack[m_PntSp].Value );
					// next
					Tv_StX	=	Tv_StX + Tv_AddSt;
					Tv_EdX	=	Tv_EdX + Tv_AddEd;
					Tv_StY ++;
				} while ( Tv_StY < m_PntStack[m_PntSp].Rect.bottom );
			}
			else
			{
				// vertical line
				Tv_EdY	=	m_PntStack[m_PntSp].Rect.bottom - 1;
				// up
				Tv_Atb	=	(m_PntStack[m_PntSp].Para1 >> 4) & 0x0F;
				if ( 1 == Tv_Atb )
				{
					Tv_AddSt	=	1;
				}
				else if ( 2 == Tv_Atb )
				{
					Tv_AddSt	=	-1;
					Tv_StY		=	Tv_StY + (m_PntStack[m_PntSp].Rect.right - m_PntStack[m_PntSp].Rect.left );
				}
				// down
				Tv_Atb	=	m_PntStack[m_PntSp].Para1 & 0x0F;
				if ( 1 == Tv_Atb )
				{
					Tv_AddEd	=	-1;
				}
				else if ( 2 == Tv_Atb )
				{
					Tv_AddEd	=	1;
					Tv_EdY	=	Tv_EdY - (m_PntStack[m_PntSp].Rect.right - m_PntStack[m_PntSp].Rect.left) + 1;
				}
				// draw
				do 
				{
					// check over position
					if ( Tv_StY >= Tv_EdY )
					{
						break;
					}
					m_GdibScr->DrawLine( Tv_StX, Tv_StY, Tv_StX, Tv_EdY, m_PntStack[m_PntSp].Value );
					// next
					Tv_StY	=	Tv_StY + Tv_AddSt;
					Tv_EdY	=	Tv_EdY + Tv_AddEd;
					Tv_StX ++;
				} while ( Tv_StX < m_PntStack[m_PntSp].Rect.right );
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcDoPstImage( void )
{
	// local -------------------
		Ptr_GrGdib	Tv_PtrGdib;
		BOOL8		Tv_IsSpr;
		__s32			Tv_ImgSizeX;
		__s32			Tv_ImgSizeY;
		__s32			Tv_X;
		__s32			Tv_Y;
		__s32			Tv_LimX;
		__s32			Tv_LimY;
		St_GrRect	Tv_RcOrg;
		St_GrRect	Tv_RcClip;
	// code --------------------
		// check exist image
		Tv_PtrGdib	=	V_GrP4Mng->ImgGet( (__u16)m_PntStack[m_PntSp].Value );
		if ( NULL != Tv_PtrGdib )
		{
			// get orignal clipper rect
			m_GdibScr->ClipperGet( &Tv_RcOrg );
			// set image clipper
			if ( GrDrawCnGetComRect( &Tv_RcClip, &Tv_RcOrg, &m_PntStack[m_PntSp].Rect ) )
			{
				// set clipping
				m_GdibScr->ClipperSet( &Tv_RcClip );
				// set srite mode
				Tv_IsSpr	=	FALSE;
				if ( 0 != (E_GrP4ScptPstImgAtbSprite & m_PntStack[m_PntSp].Para0) )
				{
					Tv_IsSpr	=	TRUE;
				}
				m_GdibScr->SpriteModeSet( Tv_IsSpr );
				// check fill mode
				if ( 0 != (E_GrP4ScptPstImgAtbFill & m_PntStack[m_PntSp].Para0) )
				{
					// fill mode
					Tv_ImgSizeX	=	(__s32)Tv_PtrGdib->ResX;
					Tv_ImgSizeY	=	(__s32)Tv_PtrGdib->ResY;
					Tv_LimX			=	m_PntStack[m_PntSp].Rect.right;
					Tv_LimY			=	m_PntStack[m_PntSp].Rect.bottom;
					Tv_Y	=	m_PntStack[m_PntSp].Rect.top;
					while( Tv_LimY > Tv_Y )
					{
						Tv_X	=	m_PntStack[m_PntSp].Rect.left;
						while( Tv_LimX > Tv_X )
						{
							m_GdibScr->BltGdib( Tv_X, Tv_Y, Tv_PtrGdib );
							// next
							Tv_X	=	Tv_X + Tv_ImgSizeX;
						}
						// next
						Tv_Y	=	Tv_Y + Tv_ImgSizeY;
					}
				}
				else
				{
					// single mode
					m_GdibScr->BltGdib( m_PntStack[m_PntSp].Rect.left, m_PntStack[m_PntSp].Rect.top, Tv_PtrGdib );
				}
			}
			// restore clipper
			m_GdibScr->ClipperSet( &Tv_RcOrg );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcDoPstFont( void )
{
	// local -------------------
	// code --------------------
		m_PntFont	=	V_GrP4Mng->FontGet( m_PntStack[m_PntSp].Para0 );
		m_PntTxtClrShadow	=	m_PntStack[m_PntSp].Value;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcDoPstStyle( void )
{
	// local -------------------
		__u8*	Tv_PtrBt;
	// code --------------------
		m_PntTxtGapPitch	=	(__s32)( (__s8)m_PntStack[m_PntSp].Para0 );
		m_PntTxtGapLine		=	(__s32)( (__s8)m_PntStack[m_PntSp].Para1 );
		Tv_PtrBt	=	(__u8*)&m_PntStack[m_PntSp].Value;
		m_PntTxtAlignHz		=	Tv_PtrBt[0];
		m_PntTxtAlignVt		=	Tv_PtrBt[1];
		m_PntTxtAtb				=	Tv_PtrBt[2];
		m_IsPntTxtShadow	=	FALSE;
		if ( 0 != (E_GrP4ScptPstTxtAtbDrawShadow & m_PntTxtAtb) )
		{
			m_IsPntTxtShadow	=	TRUE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcDoPstText( void )
{
	// local -------------------
	// code --------------------
		// check exist font
		if ( NULL != m_PntFont )
		{
			// init runtime
			m_PntTxtStr	=	(WCHAR*)m_PntStack[m_PntSp].PtrOpt;
			m_PntTxtRc	=	m_PntStack[m_PntSp].Rect;
			m_PntTxtClrTxt	=	m_PntStack[m_PntSp].Value;
			m_PntTxtCus	=	m_PntStack[m_PntSp].IntOpt;

			LcPntTxtAreaBuild();
			LcPntTxtDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcDoPaintStack( Ptr_GrRect A_PtrRect )
{
	// local -------------------
	// code --------------------
		// check exist stack item
		if ( 0 != m_PntSp )
		{
			// clipper set
			//m_GdibScr->ClipperSet( A_PtrRect );
			// do working
			while ( 0 != m_PntSp )
			{
				m_PntSp --;
				// clip
				//m_GdibScr->ClipperSet( &m_PntStack[m_PntSp].Rect );
				// check item
				switch ( m_PntStack[m_PntSp].Type )
				{
					case E_GrP4ScptPstTypeClrFill:
						m_GdibScr->FillRect( &m_PntStack[m_PntSp].Rect, m_PntStack[m_PntSp].Value );
						break;
					case E_GrP4ScptPstTypeLine:
						LcDoPstLine();
						break;
					case E_GrP4ScptPstTypeImage:
						LcDoPstImage();
						break;
					case E_GrP4ScptPstTypeText:
						LcDoPstText();
						break;
					case E_GrP4ScptPstTypeFont:
						LcDoPstFont();
						break;
					case E_GrP4ScptPstTypeStyle:
						LcDoPstStyle();
						break;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcPntTxtParsProc( void )
{
	// local -------------------
		WCHAR	Tv_Code;
		__s32		Tv_SizeX;
		__u32	Tv_FntIdx;
		St_GrRect	Tv_RcFnt;
		__s32		Tv_FntPitch;
		__s32		Tv_FntGap;
	// code --------------------
		// init
		// check able
		if ( m_IsPntTxtProc )
		{
			// get code
			Tv_Code		=	m_PntTxtStr[m_PntTxtIdx];
			// analysis text
			if ( 0 == Tv_Code )
			{
				// check cursor position
				if ( m_PntTxtCus ==  m_PntTxtIdx)
				{
					// request draw cursor
					// calculate cursor size
					Tv_SizeX	=	m_PntFont->GetWidth() >> E_GrP4ConCusShift;
					// check end 
					if ( (m_PntTxtLineTbl[m_PntTxtLnCnt].SzX + Tv_SizeX) > (m_PntTxtRc.right - m_PntTxtRc.left)  )
					{
						// do word break
						if ( (0 != (E_GrP4ScptPstTxtAtbMultiLine & m_PntTxtAtb)) && (0 != (E_GrP4ScptPstTxtAtbWarp & m_PntTxtAtb)) )
						{
							// move next line able
							if (	(E_GrP4ConPntTxtLineMax > m_PntTxtLnCnt) &&
										( (m_PntTxtAreaY + m_PntTxtGapLine + (__s32)m_PntFont->GetHeight()) <= (m_PntTxtRc.bottom - m_PntTxtRc.top) ) 
								 )
							{
								// word break
								m_PntTxtLnCnt ++;
								m_PntTxtLineTbl[m_PntTxtLnCnt].Cnt ++;
								m_PntTxtLineTbl[m_PntTxtLnCnt].SzX	=	m_PntTxtLineTbl[m_PntTxtLnCnt].SzX + Tv_SizeX;
								m_PntTxtAreaY	=	m_PntTxtAreaY + m_PntTxtGapLine + (__s32)m_PntFont->GetHeight();
							}
						}
					}
					else
					{
						// add process
						m_PntTxtLineTbl[m_PntTxtLnCnt].Cnt ++;
						m_PntTxtLineTbl[m_PntTxtLnCnt].SzX	=	m_PntTxtLineTbl[m_PntTxtLnCnt].SzX + Tv_SizeX;
					}
				}
				// zero finish
				m_IsPntTxtProc	=	FALSE;
			}
			else 
			{
				// normal text
				// check gap pitch
				if ( 0 != m_PntTxtLineTbl[m_PntTxtLnCnt].Cnt )
				{
					m_PntTxtLineTbl[m_PntTxtLnCnt].SzX	=	m_PntTxtLineTbl[m_PntTxtLnCnt].SzX + m_PntTxtGapPitch;
				}
				// check special code - enter or space or none text
				if (	(0x0A == Tv_Code) || ( (0x3B == Tv_Code) && (0 != (E_GrP4ScptPstTxtAtbSemiEnter & m_PntTxtAtb)) ) ||
							(0x0D == Tv_Code) || (0x20 == Tv_Code)
						)
				{
					Tv_SizeX	=	m_PntFont->GetWidth() >> E_GrP4ConCusShift;		// display size
				}
				else
				{
					// normal text
					// check password
					if ( 0 != (E_GrP4ScptPstTxtAtbPasswd & m_PntTxtAtb) )
					{
						Tv_Code	=	0x2A;
					}
					// get font info
					if ( m_PntFont->GetCharInfo( Tv_Code, &Tv_FntIdx, &Tv_RcFnt, &Tv_FntPitch, &Tv_FntGap ) )
					{
						Tv_SizeX	=	Tv_FntPitch;
						if ( 0 != (E_GrP4ScptPstTxtAtbFixPitch & m_PntTxtAtb) )
						{
							Tv_SizeX	=	m_PntFont->GetWidth();
						}
					}
					else
					{
						Tv_SizeX	=	m_PntFont->GetWidth() >> E_GrP4ConCusShift;		// display size
					}
				}
				// check end 
				if ( (m_PntTxtLineTbl[m_PntTxtLnCnt].SzX + Tv_SizeX) > (m_PntTxtRc.right - m_PntTxtRc.left)  )
				{
					// do word break
					if ( (0 != (E_GrP4ScptPstTxtAtbMultiLine & m_PntTxtAtb)) && (0 != (E_GrP4ScptPstTxtAtbWarp & m_PntTxtAtb)) )
					{
						// move next line able
						if (	(E_GrP4ConPntTxtLineMax > m_PntTxtLnCnt) &&
							( (m_PntTxtAreaY + m_PntTxtGapLine + (__s32)m_PntFont->GetHeight()) <= (m_PntTxtRc.bottom - m_PntTxtRc.top) ) 
							)
						{
							// word break
							m_PntTxtLnCnt ++;
							m_PntTxtLineTbl[m_PntTxtLnCnt].Cnt ++;
							m_PntTxtLineTbl[m_PntTxtLnCnt].SzX	=	m_PntTxtLineTbl[m_PntTxtLnCnt].SzX + Tv_SizeX;
							m_PntTxtAreaY	=	m_PntTxtAreaY + m_PntTxtGapLine + (__s32)m_PntFont->GetHeight();
						}
						else
						{
							// stop
							m_IsPntTxtProc	=	FALSE;
						}
					}
					else
					{
						// stop
						m_IsPntTxtProc	=	FALSE;
					}
				}
				else
				{
					// add process
					m_PntTxtLineTbl[m_PntTxtLnCnt].Cnt ++;
					m_PntTxtLineTbl[m_PntTxtLnCnt].SzX	=	m_PntTxtLineTbl[m_PntTxtLnCnt].SzX + Tv_SizeX;
				}
				// check enter key mode
				if ( (0x0A == Tv_Code) || ( (0x3B == Tv_Code) && (0 != (E_GrP4ScptPstTxtAtbSemiEnter & m_PntTxtAtb)) ) )
				{
					// check do new line process
					if ( m_IsPntTxtProc )
					{
						// check multi line
						if (0 != (E_GrP4ScptPstTxtAtbMultiLine & m_PntTxtAtb))
						{
							// do next line
							// move next line able
							if (	(E_GrP4ConPntTxtLineMax > m_PntTxtLnCnt) &&
								( (m_PntTxtAreaY + m_PntTxtGapLine + (__s32)m_PntFont->GetHeight()) <= (m_PntTxtRc.bottom - m_PntTxtRc.top) ) 
								)
							{
								// do enter - not increase code count
								m_PntTxtLnCnt ++;
								m_PntTxtAreaY	=	m_PntTxtAreaY + m_PntTxtGapLine + (__s32)m_PntFont->GetHeight();
							}
							else
							{
								// stop
								m_IsPntTxtProc	=	FALSE;
							}
						}
					}
				}
			}
			// go next
			if ( m_IsPntTxtProc )
			{
				m_PntTxtIdx ++;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcPntTxtAreaBuild( void )
{
	// local -------------------
		__u8		Tv_WkIdx;
	// code --------------------
		// init
		m_PntTxtIdx			=	0;
		m_IsPntTxtProc	=	TRUE;
		m_PntTxtLnCnt		=	0;
		GrDumyZeroMem( m_PntTxtLineTbl, sizeof(m_PntTxtLineTbl) );
		m_PntTxtAreaY		=	(__s32)m_PntFont->GetHeight();
		// check area height
		if ( m_PntTxtAreaY <= (m_PntTxtRc.bottom - m_PntTxtRc.top) )
		{
			// do calculate
			while ( m_IsPntTxtProc )
			{
				LcPntTxtParsProc();
			}
			// check last line exist
			if ( 0 != m_PntTxtLineTbl[m_PntTxtLnCnt].Cnt )
			{
				m_PntTxtLnCnt ++;		// line count patch
			}
			else
			{
				// paint area height patch
				if ( 0 != m_PntTxtLnCnt )
				{
					m_PntTxtAreaY	=	m_PntTxtAreaY - (__s32)m_PntFont->GetHeight() - m_PntTxtGapLine;
				}
			}
			// check line exist
			if ( 0 != m_PntTxtLnCnt )
			{
				// calculate align y
				if ( E_GrP4ScptPstTxtAlignVtTop == m_PntTxtAlignVt )
				{
					m_PntTxtStY	=	m_PntTxtRc.top;
				}
				else if ( E_GrP4ScptPstTxtAlignVtBottom == m_PntTxtAlignVt )
				{
					m_PntTxtStY	=	m_PntTxtRc.bottom - m_PntTxtAreaY;
				}
				else
				{
					m_PntTxtStY	=	m_PntTxtRc.top + ( ( (m_PntTxtRc.bottom - m_PntTxtRc.top) - m_PntTxtAreaY ) >> 1 ) ;
				}
				// horizontal align
				for ( Tv_WkIdx=0;Tv_WkIdx < m_PntTxtLnCnt;Tv_WkIdx++ )
				{
					if ( E_GrP4ScptPstTxtAlignHzLeft == m_PntTxtAlignHz )
					{
						m_PntTxtLineTbl[Tv_WkIdx].StX	=	m_PntTxtRc.left;
					}
					else if ( E_GrP4ScptPstTxtAlignHzRight == m_PntTxtAlignHz )
					{
						m_PntTxtLineTbl[Tv_WkIdx].StX	=	m_PntTxtRc.right - m_PntTxtLineTbl[Tv_WkIdx].SzX;
					}
					else
					{
						m_PntTxtLineTbl[Tv_WkIdx].StX	=	m_PntTxtRc.left + 
							( ( (m_PntTxtRc.right - m_PntTxtRc.left) - m_PntTxtLineTbl[Tv_WkIdx].SzX ) >> 1 ) ;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcPntTxtDraw( void )
{
	// local -------------------
		WCHAR	Tv_Code;
		__u8	Tv_LnIdx;
		__s32		Tv_CdIdx;
		__s32		Tv_SizeX;
		__s32		Tv_PosX;
		__s32		Tv_PosY;
		__u32	Tv_FntIdx;
		St_GrRect	Tv_RcFnt;
		__s32		Tv_FntPitch;
		__s32		Tv_FntGap;
		BOOL8	Tv_IsFont;
		__u32	Tv_ClrTxt;
	// code --------------------
		// init
		m_PntTxtIdx		=	0;
		// check line count
		if ( 0 != m_PntTxtLnCnt )
		{
			// loop line
			Tv_PosY		=	m_PntTxtStY;
			for ( Tv_LnIdx=0;Tv_LnIdx < m_PntTxtLnCnt;Tv_LnIdx++ )
			{
				// get code
				if ( 0 != m_PntTxtLineTbl[Tv_LnIdx].Cnt )
				{
					Tv_PosX		=	m_PntTxtLineTbl[Tv_LnIdx].StX;
					for ( Tv_CdIdx=0;Tv_CdIdx < m_PntTxtLineTbl[Tv_LnIdx].Cnt;Tv_CdIdx ++ )
					{
						// init
						Tv_IsFont	=	FALSE;
						// check pitch gap
						if ( 0 != Tv_CdIdx )
						{
							Tv_PosX	=	Tv_PosX + m_PntTxtGapPitch;
						}
						// get text size
						Tv_Code	=	m_PntTxtStr[m_PntTxtIdx];
						// check special code - enter or space or none text
						if (	(0x00 == Tv_Code) ||	(0x0A == Tv_Code) || 
									( (0x3B == Tv_Code) && (0 != (E_GrP4ScptPstTxtAtbSemiEnter & m_PntTxtAtb)) ) ||
									(0x0D == Tv_Code) || (0x20 == Tv_Code)
							)
						{
							Tv_SizeX	=	m_PntFont->GetWidth() >> E_GrP4ConCusShift;		// display size
						}
						else
						{
							// normal text
							// check password
							if ( 0 != (E_GrP4ScptPstTxtAtbPasswd & m_PntTxtAtb) )
							{
								Tv_Code	=	0x2A;
							}
							// get font info
							if ( m_PntFont->GetCharInfo( Tv_Code, &Tv_FntIdx, &Tv_RcFnt, &Tv_FntPitch, &Tv_FntGap ) )
							{
								Tv_SizeX	=	Tv_FntPitch;
								if ( 0 != (E_GrP4ScptPstTxtAtbFixPitch & m_PntTxtAtb) )
								{
									Tv_SizeX	=	m_PntFont->GetWidth();
								}
								Tv_IsFont	=	TRUE;
							}
							else
							{
								Tv_SizeX	=	m_PntFont->GetWidth() >> E_GrP4ConCusShift;		// display size
							}
						}
						// draw font
						if ( Tv_IsFont )
						{
							Tv_ClrTxt	=	m_PntTxtClrTxt;
							// check cursor position
							if ( m_PntTxtCus == m_PntTxtIdx )
							{
								Tv_ClrTxt	=	V_GrP4Mng->CursorColorGet();
							}
							m_GdibScr->FontBlt( Tv_PosX, Tv_PosY, m_PntFont->GetGdibPtr(), Tv_ClrTxt, m_PntTxtClrShadow, 
								m_IsPntTxtShadow, &Tv_RcFnt );
						}
						else
						{
							// check draw cursor
							if ( m_PntTxtCus == m_PntTxtIdx )
							{
								Tv_RcFnt.left			=	Tv_PosX;
								Tv_RcFnt.top			=	Tv_PosY;
								Tv_RcFnt.right		=	Tv_PosX + Tv_SizeX;
								Tv_RcFnt.bottom		=	Tv_PosY + (__s32)m_PntFont->GetHeight();
								m_GdibScr->FillRect( &Tv_RcFnt, V_GrP4Mng->CursorColorGet() );
							}
						}
						// next
						Tv_PosX		=	Tv_PosX + Tv_SizeX;
						m_PntTxtIdx ++;
					}
				}
				// next
				Tv_PosY		=	Tv_PosY + (__s32)m_PntFont->GetHeight() + m_PntTxtGapLine;
			}
		}
}
//--------------------------------------------------------------------
__u8 Cls_GrP4ConBase::QuadPageCntByMode( __u8 A_Mode )
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_MaxChCnt;
		__u8	Tv_ViewCnt;
	// code --------------------
		Tv_MaxChCnt	=	V_GrP4Mng->VdoMaxChCntGet();
		Tv_ViewCnt	=	GrDvrQuadViewCount( A_Mode );

		Tv_Result	=	Tv_MaxChCnt / Tv_ViewCnt;
		if ( 0 != (Tv_MaxChCnt % Tv_ViewCnt) )
		{
			Tv_Result ++;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4ConBase::QuadModeSet( __u8 A_Mode, __u8 A_Page, __u32 A_Fcc )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_PageMax;
		St_GrTaskMsgQuadChgReq	Tv_Msg;
		__u32	Tv_Mask;
		__s32		Tv_Rint[4];
		Ptr_GrP4RgstSys	Tv_PtrRgstSys;
	// code --------------------
		Tv_Result	=	FALSE;

		// check mode count
		if ( E_GrDvrQuadModeCnt > A_Mode )
		{
			// check able mode
			Tv_Mask	=	1 << (__u32)A_Mode;
			if ( 0 != (Tv_Mask & m_QuadAbleMap) )
			{
				// check able mode
				Tv_PageMax	=	QuadPageCntByMode( A_Mode );
				if ( 0 != Tv_PageMax )
				{
					if ( Tv_PageMax > A_Page )
					{
						// update mode
						m_QuadMode	=	A_Mode;
						m_QuadPage	=	A_Page;
						Tv_Result		=	TRUE;
						// change quad message
						Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgQuadChgReq;
						Tv_Msg.ConIdx			=	m_ConIdx;
						Tv_Msg.QuadMode		=	A_Mode;
						Tv_Msg.QuadPage		=	A_Page;
						Tv_Msg.Fcc				=	A_Fcc;
						GrTaskSvrMsgBroadCast( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

						/*
						Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdSysStat;
						Tv_Msg.Hd.ConIdx	=	m_ConIdx;
						Tv_Msg.Hd.WinId		=	E_GrP4WinIdAll;
						Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdAll;
						Tv_Msg.Type				=	E_GrP4SysStatQuadMode;
						V_GrP4Mng->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
						*/

						// registry save
						if ( 0 == m_ConIdx )
						{
							Tv_PtrRgstSys	=	(Ptr_GrP4RgstSys)V_GrP4Mng->RegistryPtrGet();
							Tv_PtrRgstSys->LiveQdMode	=	A_Mode;
							Tv_PtrRgstSys->LiveQdPage	=	A_Page;
							GrP4ApiCall(E_GrP4ApiRgstSave, Tv_Rint, NULL, NULL);
						}

					}
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::QuadModeNext( __u32 A_Fcc )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_Mode;
		__u32	Tv_Mask;
	// code --------------------
		Tv_Mode	=	m_QuadMode;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrQuadModeCnt;Tv_WkIdx++ )
		{
			Tv_Mode ++;
			if ( Tv_Mode >= E_GrDvrQuadModeCnt )
			{
				Tv_Mode	=	E_GrDvrQuadMode1;
			}
			Tv_Mask	=	1 << (__u32)Tv_Mode;
			if ( 0 != (Tv_Mask & m_QuadAbleMap) )
			{
				//found
				QuadModeSet( Tv_Mode, 0, A_Fcc );
				break;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::QuadModePrev( __u32 A_Fcc )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_Mode;
		__u32	Tv_Mask;
	// code --------------------
		Tv_Mode	=	m_QuadMode;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrQuadModeCnt;Tv_WkIdx++ )
		{
			if ( 0 == Tv_Mode )
			{
				Tv_Mode	=	E_GrDvrQuadModeCnt - 1;
			}
			else
			{
				Tv_Mode --;
			}
			Tv_Mask	=	1 << (__u32)Tv_Mode;
			if ( 0 != (Tv_Mask & m_QuadAbleMap) )
			{
				//found
				QuadModeSet( Tv_Mode, 0, A_Fcc );
				break;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::QuadRectSet(Ptr_GrRect A_PtrRc)
{
		m_QuadRect	=	*A_PtrRc;
}
//--------------------------------------------------------------------
__u8*	Cls_GrP4ConBase::QuadViewChMapPtrGet( void )
{
		return	GrDvrQuadMapPtr( m_PtrQuadChMap , m_QuadMode, m_QuadPage );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4ConBase::DvlDebugPrepare( void )
{
	// local -------------------
		__u8	Tv_VdoChCnt;
	// code --------------------
		LcCusImgUpdt();

		Tv_VdoChCnt		=	V_GrP4Mng->VdoMaxChCntGet();
		m_QuadAbleMap	=	E_GrDvrQuadAbleMask1 | E_GrDvrQuadAbleMask4;
		if ( 8 <= Tv_VdoChCnt )
		{
			m_QuadAbleMap	=	m_QuadAbleMap | E_GrDvrQuadAbleMask6 | E_GrDvrQuadAbleMask8 | E_GrDvrQuadAbleMask9;
		}
		if ( 16 <= Tv_VdoChCnt )
		{
			m_QuadAbleMap	=	m_QuadAbleMap | E_GrDvrQuadAbleMask10 | E_GrDvrQuadAbleMask13 | E_GrDvrQuadAbleMask16;
		}
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP4ConBase::IsShutDown( void )
{
		return	m_IsShutDown;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcCusImgUpdt( void )
{
	// local -------------------
		__u16	Tv_ImgId;
	// code --------------------
		m_CusImgId	=	E_GrP4ImgIdNone;

		Tv_ImgId		=	V_GrP4Mng->SoftCusImgIdGet();
		if ( E_GrP4ImgIdNone != Tv_ImgId )
		{
			m_PtrGdibCus	=	V_GrP4Mng->ImgGet( Tv_ImgId );
			if ( NULL != m_PtrGdibCus )
			{
				// update
				m_CusImgId	=	Tv_ImgId;
				m_CusSizeX	=	m_PtrGdibCus->ResX;
				m_CusSizeY	=	m_PtrGdibCus->ResY;
				// HAL cursor image update
				HalMouseCusImgSet( m_PtrGdibCus );
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::HalMousePosSet( __s32 A_X, __s32 A_Y )
{

}
//--------------------------------------------------------------------
__u32	Cls_GrP4ConBase::QuadDispChMapGet(void)
{
	// local -------------------
		__u32	Tv_Result;
		__u8*	Tv_PtrChMap;
		__u8	Tv_ChCnt;
		__u8	Tv_WkIdx;
		__u8	Tv_Val;
	// code --------------------
		Tv_Result	=	0;

		Tv_PtrChMap	=	QuadViewChMapPtrGet();
		Tv_ChCnt		=	GrDvrQuadViewCount( m_QuadMode );

		if ( 0 != Tv_ChCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ChCnt;Tv_WkIdx ++ )
			{
				Tv_Val	=	*Tv_PtrChMap;
				if ( E_GrDvrMaxChCnt > Tv_Val )
				{
					Tv_Result	=	Tv_Result | (1 << Tv_Val);
				}
				// next
				Tv_PtrChMap ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::LcProcMsgSysStat( Ptr_GrP4MsgSysStat A_PtrMsg )
{
	// local -------------------
		St_GrTaskMsgQuadChgDone	Tv_Msg;
		__u16*	Tv_PtrEvt;
		Ptr_GrP4ScptCfgMdl	Tv_PtrCfgMdl;
	// code --------------------
		if ( E_GrP4SysStatQuadMode == A_PtrMsg->Type )
		{
			Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgQuadChgDone;
			Tv_Msg.ConIdx			=	m_ConIdx;
			Tv_Msg.QuadMode		=	m_QuadMode;
			Tv_Msg.QuadPage		=	m_QuadPage;
			Tv_Msg.ChAbleMap	=	QuadDispChMapGet();
			GrTaskSvrMsgBroadCast( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
			// call assembly quad event
			Tv_PtrCfgMdl	=	V_GrP4Mng->ConfigMdlAtbGet();
			Tv_PtrEvt			=	&Tv_PtrCfgMdl->AsmQuad0Chg;
			if ( E_GrP4AsmIdNone != Tv_PtrEvt[m_ConIdx] )
			{
				V_GrP4Mng->VmRunCode( Tv_PtrEvt[m_ConIdx], NULL, NULL );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::QuadModeGet( __u8* A_PtrRtMode, __u8* A_PtrRtPage )
{
	// local -------------------
	// code --------------------
		*A_PtrRtMode	=	m_QuadMode;
		*A_PtrRtPage	=	m_QuadPage;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::QuadPageNext( __u32 A_Fcc )
{
	// local -------------------
		__u8		Tv_QuadPage;
		__u8		Tv_MaxPage;
	// code --------------------
		Tv_MaxPage		= QuadPageCntByMode(m_QuadMode);
		Tv_QuadPage		= m_QuadPage + 1;

		if(Tv_MaxPage <= Tv_QuadPage)
		{
			Tv_QuadPage = 0;
		}

		QuadModeSet( m_QuadMode, Tv_QuadPage, A_Fcc );
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::HalMouseCusImgSet( Ptr_GrGdib A_PtrGdib )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::MouseCapture( void )
{
	// local -------------------
	// code --------------------
		m_IsMseCap	=	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::MouseRelease( void )
{
		m_IsMseCap	=	FALSE;
}
//--------------------------------------------------------------------
Ptr_GrRect	Cls_GrP4ConBase::QuadGridPosTblPtrGet(__u8* A_PtrRtCnt)
{
		*A_PtrRtCnt	=	0;
		return	NULL;
}
//--------------------------------------------------------------------
Ptr_GrRect	Cls_GrP4ConBase::QuadViewRectTblPtrGet(__u8* A_PtrRtCnt)
{
		*A_PtrRtCnt	=	0;
		return	NULL;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4ConBase::IsQuadGridHw(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4ConBase::IsQuadCovertHw(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::QuadGridWidthSet(__u8 A_Width)
{
		m_QuadGridWidth	=	A_Width;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4ConBase::IsQuadGridView(void)
{
		return	m_IsQuadGridView;
}
//--------------------------------------------------------------------
void	Cls_GrP4ConBase::QuadGridViewSet(BOOL8 A_IsView)
{
		m_IsQuadGridView	=	A_IsView;
}
//--------------------------------------------------------------------
