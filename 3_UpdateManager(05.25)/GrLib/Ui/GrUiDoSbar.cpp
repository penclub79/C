/*
	slider bar
*/

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoSbar.h>
#include	<Ui/GrUiConsole.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<Ui/GrUiMng.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local macro
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*	V_GrUiMngObj;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoSbar::Cls_GrUiDoSbar( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
		//init
		m_TypeId				=	E_GrUiDoTypeIdSbar;
		m_IsFocusAble		=	TRUE;

		//local date init
		m_IsSbarHz			=	FALSE;
		m_IsOnKeyCtrl		=	FALSE;
		m_SbtnBrdAtb		=	0;
		m_SbtnBrdDepth	=	E_GrUiDoSkinDfltBrdDepth;
		m_SbtnSize			=	E_GrUiDoSbarBtnDfltSize;
		m_SbarEvtHostId	=	E_GrUiObjIdNone;

		m_RangeMin			=	E_GrUiDoSbarDfltMin;
		m_RangeMax			=	E_GrUiDoSbarDfltMax;
		m_ValPos				=	0;
		m_PageSize			=	1;		//default page size

		m_IsSbarCtrl		=	FALSE;

		GrDumyZeroMem( m_SbtnImgTbl, sizeof(m_SbtnImgTbl) );
		GrDumyZeroMem( m_SbtnClrTbl, sizeof(m_SbtnClrTbl) );

}
//--------------------------------------------------------------------
Cls_GrUiDoSbar::~Cls_GrUiDoSbar()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::BarDirSet( BOOL8 A_IsHorizontal )
{
		if ( m_IsSbarHz != A_IsHorizontal )
		{
			m_IsSbarHz	=	A_IsHorizontal;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSbar::IsBarDirVt( void )
{
		return	m_IsSbarHz;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::SbtnBrdDepthSet( __u8 A_Depth )
{
		if ( m_SbtnBrdDepth != A_Depth )
		{
			m_SbtnBrdDepth	=	A_Depth;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoSbar::SbtnBrdDepthGet( void )
{
		return	m_SbtnBrdDepth;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::SbtnBrdAtbSet( __u8 A_Atb )
{
		if ( m_SbtnBrdAtb != A_Atb )
		{
			m_SbtnBrdAtb	=	A_Atb;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoSbar::SbtnBrdAtbGet( void )
{
		return	 m_SbtnBrdAtb;
}
//--------------------------------------------------------------------
__u32		Cls_GrUiDoSbar::LcSbarBtnImgIdxGet( void )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	E_GrUiDoSbarStatDisable;

		if ( IsCanEnable() )
		{
			Tv_Result	=	E_GrUiDoSbarStatNormal;
			if ( m_IsSbarCtrl )
			{
				Tv_Result		=	E_GrUiDoSbarStatControl;
			}
			else if ( IsFocused() )
			{
				Tv_Result		=	E_GrUiDoSbarStatFocus;
				if ( m_IsOnKeyCtrl )
				{
					Tv_Result		=	E_GrUiDoSbarStatControl;
				}
			}
			else if ( IsOnMouse() )
			{
				Tv_Result		=	E_GrUiDoSbarStatFocus;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSbar::LcSbarDrawInfoGet( Ptr_GrRect A_PtrRcScr, __s32* A_PtrRtAreaSize )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_RngSize;
		__s32			Tv_BrdSize;
		__s32			Tv_AreaSize;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check able value
		if ( m_RangeMin < m_RangeMax )
		{
			//calculate value 
			Tv_RngSize			=	m_RangeMax - m_RangeMin;

			//check direction
			if ( m_IsSbarHz )
			{
				//horizontal
				Tv_BrdSize	=	A_PtrRcScr->right - A_PtrRcScr->left;
			}
			else
			{
				//vertical
				Tv_BrdSize	=	A_PtrRcScr->bottom - A_PtrRcScr->top;
			}
			//check draw able
			if ( m_SbtnSize < Tv_BrdSize )
			{	
				Tv_AreaSize		=	Tv_BrdSize - (__s32)m_SbtnSize;
				//success
				*A_PtrRtAreaSize	=	Tv_AreaSize;
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSbar::LcSbarBtnRectGet( Ptr_GrRect A_PtrRcScr )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_RngPos;
		__s32			Tv_AreaSize;
		__s32			Tv_AreaPos;
		__s32			Tv_RngSize;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( LcSbarDrawInfoGet( &m_PntRcClient, &Tv_AreaSize ) )
		{
			//init
			Tv_RngSize	=	m_RangeMax - m_RangeMin;
			Tv_RngPos		=	m_ValPos - m_RangeMin;
			Tv_AreaPos	=	Tv_RngPos;
			if ( Tv_RngSize != Tv_AreaSize )
			{
				//area pos patch
				Tv_AreaPos	=	(Tv_RngPos * Tv_AreaSize) / Tv_RngSize;
			}
			//calculate rect
			if ( m_IsSbarHz )
			{
				//horizontal
				A_PtrRcScr->top			=	m_PntRcClient.top;
				A_PtrRcScr->bottom	=	m_PntRcClient.bottom;
				A_PtrRcScr->left		=	m_PntRcClient.left + Tv_AreaPos;
				A_PtrRcScr->right		=	A_PtrRcScr->left + (__s32)m_SbtnSize;
			}
			else
			{
				//vertical
				A_PtrRcScr->left		=	m_PntRcClient.left;
				A_PtrRcScr->right		=	m_PntRcClient.right;
				A_PtrRcScr->top			=	m_PntRcClient.top + Tv_AreaPos;
				A_PtrRcScr->bottom	=	A_PtrRcScr->top + (__s32)m_SbtnSize;
			}
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::ProcBgBordDraw( void )
{
	// local -------------------
		__u32		Tv_ImgIdx;
		St_GrRect	Tv_RcBtn;
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		//inherited
		Cls_GrUiDoSkin::ProcBgBordDraw();

		//draw button
		//calculate rect
		if ( LcSbarBtnRectGet( &Tv_RcBtn ) )
		{
			//clip set
			if ( PaintClipperSet( TRUE, &Tv_RcBtn ) )
			{
				Tv_ImgIdx	=	LcSbarBtnImgIdxGet();
				//check sprite mode
				if ( (0 != m_SbtnClrTbl[Tv_ImgIdx]) || ( 0 != (E_GrUiDoSkibAtbSprite & m_SbtnImgTbl[Tv_ImgIdx].Atb) ) )
				{
					//draw background color
					m_GdibScr->FillRect( &Tv_RcBtn, m_SbtnClrTbl[Tv_ImgIdx] );
				}
				//draw image
				Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_SbtnImgTbl[Tv_ImgIdx].Id );
				if ( NULL != Tv_PtrGdib )
				{
					SkinDrawByPos( m_SbtnImgTbl[Tv_ImgIdx].Atb , Tv_PtrGdib, &Tv_RcBtn );
				}

				//draw boarder
				m_GdibScr->DrawBoarder( m_SbtnBrdAtb, m_SbtnBrdDepth, m_BgBrdClrTbl, &Tv_RcBtn );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::SbtnStatImgSet( __u32 A_Idx, Ptr_GrUiDoSkinImg A_PtrSkin )
{
		//check range
		if ( E_GrUiDoSbarBtnImgCnt > A_Idx )
		{
			m_SbtnImgTbl[A_Idx]	=	*A_PtrSkin;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::SbtnStatImgGet( __u32 A_Idx, Ptr_GrUiDoSkinImg A_PtrRtSkin )
{
	// local -------------------
	// code --------------------
		A_PtrRtSkin->Atb	=	0;
		A_PtrRtSkin->Id		=	E_GrUiImgIdNone;
		//check range
		if ( E_GrUiDoSbarBtnImgCnt > A_Idx )
		{
			*A_PtrRtSkin	=	m_SbtnImgTbl[A_Idx];
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSbar::SbtnStatClrGet( __u32 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	0;

		if ( E_GrUiDoSbarBtnImgCnt > A_Idx )
		{
			Tv_Result		=	m_SbtnClrTbl[A_Idx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::SbtnStatClrSet( __u32 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoSbarBtnImgCnt > A_Idx )
		{
			m_SbtnClrTbl[A_Idx]	=	A_Color;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::RangeMinSet( __s32 A_Val )
{
		if ( m_RangeMin != A_Val )
		{
			m_RangeMin	=	A_Val;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSbar::RangeMinGet( void )
{
		return	m_RangeMin;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::RangeMaxSet( __s32 A_Val )
{
		if ( m_RangeMax != A_Val )
		{
			m_RangeMax	=	A_Val;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSbar::RangeMaxGet( void )
{
		return	m_RangeMax;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::PosSet( __s32 A_Pos, BOOL8 A_IsNotify )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
		__s32		Tv_Pos;
	// code --------------------
		Tv_Pos		=	A_Pos;
		//check range
		if ( m_RangeMin > Tv_Pos )
		{
			Tv_Pos	=	m_RangeMin;
		}
		if ( m_RangeMax < Tv_Pos )
		{
			Tv_Pos	=	m_RangeMax;
		}

		if ( m_ValPos !=  Tv_Pos  )
		{
			m_ValPos	=	Tv_Pos;
			ReqDraw();
			if ( A_IsNotify )
			{
				//event make
				if ( V_GrUiMngObj->IsUserEventOn() || (E_GrUiObjIdNone != m_SbarEvtHostId) )
				{
					Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdSbarChg;
					Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
					Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
					Tv_Msg.Hd.ObjId		=	m_SbarEvtHostId;
					Tv_Msg.Obj				=	(void*)this;
					UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				}
			}
		}
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSbar::PosGet( void )
{
		return	m_ValPos;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSbar::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__s32		Tv_Val;
	// code --------------------
		//set contorl mode
		m_IsSbarCtrl	=	TRUE;
		//calculate position
		Tv_Val	=	LcSbarValByPos( A_X, A_Y );
		PosSet( Tv_Val );
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSbar::ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Val;
	// code --------------------
		//init
		//check button pushed
		if ( 0 != (E_GrUiMouseBmkkLeft & A_BtnMap) )
		{
			//release contorl mode
			m_IsSbarCtrl	=	FALSE;
			//calculate position
			Tv_Val	=	LcSbarValByPos( A_X, A_Y );
			PosSet( Tv_Val );
			Tv_Result		=	TRUE;
		}
		else
		{
			Tv_Result	=	Cls_GrUiDoSkin::ProcEvtMseMove( A_X, A_Y, A_BtnMap );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSbar::ProcEvtKeyDown( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;
		switch( A_Key )
		{
			case	E_GrUiKeyEnter:
				if ( m_IsOnKeyCtrl )
				{
					m_IsSbarCtrl	=	FALSE;
				}
				else
				{
					m_IsSbarCtrl	=	!m_IsSbarCtrl;
				}
				ReqDraw();
				Tv_Result	=	TRUE;
				break;
			case	E_GrUiKeyUp:
				if ( m_IsSbarCtrl || m_IsOnKeyCtrl )
				{
					Tv_Result	=	m_IsSbarCtrl;
					if ( !m_IsSbarHz )
					{
						PosPageDown();
						Tv_Result	=	TRUE;
					}
				}
				break;
			case	E_GrUiKeyDown:
				if ( m_IsSbarCtrl || m_IsOnKeyCtrl  )
				{
					Tv_Result	=	m_IsSbarCtrl;
					if ( !m_IsSbarHz )
					{
						PosPageUp();
						Tv_Result	=	TRUE;
					}
				}
				break;
			case	E_GrUiKeyLeft:
				if ( m_IsSbarCtrl || m_IsOnKeyCtrl )
				{
					Tv_Result	=	m_IsSbarCtrl;
					if ( m_IsSbarHz )
					{
						PosPageDown();
						Tv_Result	=	TRUE;
					}
				}
				break;
			case	E_GrUiKeyRight:
				if ( m_IsSbarCtrl || m_IsOnKeyCtrl )
				{
					Tv_Result	=	m_IsSbarCtrl;
					if ( m_IsSbarHz )
					{
						PosPageUp();
						Tv_Result	=	TRUE;
					}
				}
				break;
		}
		//inherited
		if ( !Tv_Result )
		{
			Tv_Result	=	Cls_GrUiDoSkin::ProcEvtKeyDown( A_Key );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSbar::ProcEvtWheelUp( void )
{
		PosPageDown();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSbar::ProcEvtWheelDown( void )
{
		PosPageUp();
		return	TRUE;
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSbar::LcSbarValByPos( __s32 A_ScrX, __s32 A_SrcY )
{
	// local -------------------
		__s32		Tv_Result;
		St_GrRect	Tv_RcScr;
		__s32		Tv_AreaSize;
		__s32		Tv_RngSize;
		__s32		Tv_AreaPos;
		__s32		Tv_AreaBase;
		__s32		Tv_Value;
	// code --------------------
		//init
		Tv_Result		=	m_RangeMin;

		//check able
		if ( m_RangeMin < m_RangeMax )
		{
			//get screen rect
			if ( ClientRectGet( &Tv_RcScr ) )
			{
				//get draw info
				if ( LcSbarDrawInfoGet( &Tv_RcScr, &Tv_AreaSize ) )
				{
					//get area position
					if ( m_IsSbarHz )
					{
						Tv_AreaBase	=	Tv_RcScr.left + ( ((Tv_RcScr.right - Tv_RcScr.left) - Tv_AreaSize) >> 1 );
						Tv_AreaPos	= A_ScrX - Tv_AreaBase;
					}
					else
					{
						Tv_AreaBase	=	Tv_RcScr.top + ( ((Tv_RcScr.bottom - Tv_RcScr.top) - Tv_AreaSize) >> 1 );
						Tv_AreaPos	= A_SrcY - Tv_AreaBase;
					}
					//patch area pos
					if ( 0 > Tv_AreaPos )
					{
						Tv_AreaPos	=	0;
					}
					else if ( Tv_AreaSize < Tv_AreaPos )
					{
						Tv_AreaPos	=	Tv_AreaSize;
					}
					//calculate value
					Tv_RngSize	=	m_RangeMax - m_RangeMin;
					Tv_Value		=	Tv_AreaPos;
					if ( Tv_RngSize != Tv_AreaSize )
					{
						//patch value
						Tv_Value	=	(Tv_AreaPos * Tv_RngSize) / Tv_AreaSize;
					}
					//update result
					Tv_Result	=	Tv_Result + Tv_Value;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSbar::PageUnitGet( void )
{
		return	m_PageSize;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::PageUnitSet( __s32 A_Page )
{
		if ( 0 < A_Page )
		{
			m_PageSize	=	A_Page;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::EvtHostIdSet( __u16 A_ObjId )
{
		m_SbarEvtHostId		=	A_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoSbar::EvtHostIdGet( void )
{
		return	m_SbarEvtHostId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::PosPageDown( void )
{
		PosSet( m_ValPos - m_PageSize );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::PosPageUp( void )
{
		PosSet( m_ValPos + m_PageSize );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::SbtnSizeSet( __u8 A_Size )
{
		m_SbtnSize		=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoSbar::SbtnSizeGet( void )
{
		return	m_SbtnSize;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::ProcEvtFocusLoss( void )
{
		m_IsSbarCtrl	=	FALSE;
		Cls_GrUiDoSkin::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSbar::OnKeyCtrlSet( BOOL8 A_IsSet )
{
		m_IsOnKeyCtrl	=	A_IsSet;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSbar::IsOnKyeCtrl( void )
{
		return	m_IsOnKeyCtrl;
}
//--------------------------------------------------------------------

