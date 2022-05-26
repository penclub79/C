/*
 listbox

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoListbox.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiConsole.h>
#include	<Ui/GrUiMng.h>
#include	<stdlib.h>

//====================================================================
//local const
#define E_GrUiDoListboxTestItemCnt		16

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
Cls_GrUiDoListbox::Cls_GrUiDoListbox( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
	// local -------------------
		__u32	Tv_WkIdx;
		__u32	Tv_ClrBlack;
		__u32	Tv_ClrWhite;
#ifdef GR_OSD_EDIT
		__s32		Tv_ItmIdx;
		Def_WstrTag		Tv_StrTest;
		__u8	Tv_IconIdx;
#endif
	// code --------------------
		//init
		m_TypeId			=	E_GrUiDoTypeIdListbox;
		m_IsFocusAble	=	TRUE;

		//runtime
		m_IsLbxCtrl				=	FALSE;
		m_VwsIdx					=	0;
		m_CusIdx					=	0;
		m_ItemCnt					=	0;

		m_IsEditAble			=	FALSE;
		m_IsEditNumType		=	FALSE;

		m_ItemHeight			=	E_GrUiDoListboxDfltItemHeight;
		m_ItemMgn.left		=	0;
		m_ItemMgn.top			=	0;
		m_ItemMgn.right		=	0;
		m_ItemMgn.bottom	=	0;
		m_ItemBrdDepth		=	E_GrUiDoSkinDfltBrdDepth;
		m_ItemBrdAtb			=	0;
		m_ItemGap					=	0;

		m_IsKeyDelAble		=	FALSE;
		m_IsKeyDelQuest		=	FALSE;
		
		m_IconWidth				=	E_GrUiDoListboxDfltIconWidth;
		m_IconCnt					=	0;
		m_IconPos					=	E_GrUiDoListboxIconLeft;
		m_IconGap					=	E_GrUiDoListboxDfltIconGap;

		m_LbxSbarId				=	E_GrUiObjIdNone;
		m_IsKeyFocusCtrl	=	FALSE;

		GrDumyZeroMem( &m_TextPrar, sizeof(m_TextPrar) );
		GrDumyZeroMem( m_ItemTbl, sizeof(m_ItemTbl) );
		GrDumyZeroMem( m_LbxImgTbl, sizeof(m_LbxImgTbl) );
		GrDumyZeroMem( m_LbxIconTbl, sizeof(m_LbxIconTbl) );
		GrDumyZeroMem( m_LbxBgClrTbl, sizeof(m_LbxBgClrTbl) );

		//default value
		Tv_ClrBlack		=	m_ObjConsole->ClrBlackGet();
		Tv_ClrWhite		=	m_ObjConsole->ClrWhiteGet();
		m_TextPrar.ClrShadow	=	Tv_ClrBlack;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoListboxStatCnt;Tv_WkIdx++ )
		{
			m_LbxClrTbl[Tv_WkIdx]			=	0;
			m_LbxTxtClrTbl[Tv_WkIdx]	=	Tv_ClrWhite;
		}

		//runtime
		m_IsLbxCtrl		=	FALSE;
		m_VwsIdx			=	0;
		m_ItemCnt			=	0;
		m_PageCnt			=	0;
		m_IsSbarLock	=	FALSE;

		//update page count
		LcPageCntUpdta();

		//sample item
#ifdef GR_OSD_EDIT
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoListboxTestItemCnt;Tv_WkIdx++ )
		{
			GrStrIntToWstr( Tv_StrTest, (__s32)Tv_WkIdx + 1 );
			GrStrWcat( Tv_StrTest, L" - Test" );
			Tv_ItmIdx	=	ItemAdd( Tv_StrTest );
			//set icon
			for ( Tv_IconIdx = 0;Tv_IconIdx < E_GrUiDoListboxIconMax;Tv_IconIdx++ )
			{
				ItemIconSet( Tv_ItmIdx, Tv_IconIdx,Tv_IconIdx  );
			}
		}
#endif
}
//--------------------------------------------------------------------
Cls_GrUiDoListbox::~Cls_GrUiDoListbox()
{
		//release item buffer
		ItemClear();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ProcBgColorDraw( void )
{
	// local -------------------
	// code --------------------
		//change color
		m_ClrBg		=	m_LbxBgClrTbl[E_GrUiDoListboxBgDisable];
		if ( IsCanEnable() )
		{
			//set normal
			m_ClrBg		=	m_LbxBgClrTbl[E_GrUiDoListboxBgNormal];
			//check in focus
			if ( IsFocused() || IsOnMouse() )
			{
				m_ClrBg		=	m_LbxBgClrTbl[E_GrUiDoListboxBgFocus];
			}
			//check enter mode
			if ( m_IsLbxCtrl || (IsFocused() && m_IsKeyFocusCtrl) )
			{
				m_ClrBg		=	m_LbxBgClrTbl[E_GrUiDoListboxBgCtrl];
			}
		}

		//inherited
		Cls_GrUiDoSkin::ProcBgColorDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ProcBgBordDraw( void )
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__s32				Tv_WkIdx;
		__s32				Tv_ItmIdx;
	// code --------------------
		//check draw able
		if ( m_IsPntCliAble )
		{
			//check client item drawable
			if ( 0 != m_PageCnt )
			{
				//draw items
				Tv_Rc.left		=	m_PntRcClient.left;
				Tv_Rc.top			=	m_PntRcClient.top;
				Tv_Rc.right		=	m_PntRcClient.right;
				Tv_Rc.bottom	=	Tv_Rc.top + (__s32)m_ItemHeight;
				Tv_ItmIdx		=	m_VwsIdx;
				for ( Tv_WkIdx = 0;Tv_WkIdx < m_PageCnt;Tv_WkIdx++ )
				{
					//draw item
					LcPaintItem( Tv_ItmIdx, &Tv_Rc );
					//next
					Tv_Rc.top			=	Tv_Rc.bottom + (__s32)m_ItemGap;
					Tv_Rc.bottom	=	Tv_Rc.top + (__s32)m_ItemHeight;
					Tv_ItmIdx ++;
				}
			}
		}

		//inherited
		Cls_GrUiDoSkin::ProcBgBordDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::LcSbarUpdate( void )
{
	// local -------------------
		Cls_GrUiDoSbar*	Tv_Sbar;
		__s32		Tv_Min;
		__s32		Tv_Max;
		__s32		Tv_Pos;
	// code --------------------
		//check exist sbar
		if ( (E_GrUiObjIdNone != m_LbxSbarId) && (!m_IsSbarLock) )
		{
			//find sbar
			Tv_Sbar	=	(Cls_GrUiDoSbar*)m_ObjWin->FindSlaveObjByObjId( m_LbxSbarId );
			if ( NULL != Tv_Sbar )
			{
				//check match type
				if ( E_GrUiDoTypeIdSbar == Tv_Sbar->TypeIdGet() )
				{
					Tv_Pos	=	0;
					if ( 1 < m_ItemCnt )
					{
						//calculate position ratio
						Tv_Min	=	Tv_Sbar->RangeMinGet();
						Tv_Max	=	Tv_Sbar->RangeMaxGet();
						Tv_Pos	=	((m_CusIdx * (Tv_Max - Tv_Min)) / m_ItemCnt ) + Tv_Min;
					}
					//update position
					Tv_Sbar->PosSet( Tv_Pos, FALSE );
				}
			}
		}
}
//--------------------------------------------------------------------
void		Cls_GrUiDoListbox::LcSubPntItemBg( __s32 A_Idx, Ptr_GrRect A_PtrRc, __s32 A_Stat )
{
	// local -------------------
		__u32	Tv_Color;
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		//set clipper
		PaintClipperSet( TRUE, A_PtrRc );
		//draw background color
		Tv_Color	=	m_LbxClrTbl[A_Stat];
		if ( (0 != Tv_Color) || (0 == (E_GrUiDoSkibAtbSprite & m_LbxImgTbl[A_Stat].Atb)) )
		{
			m_GdibScr->FillRect( A_PtrRc, Tv_Color );
		}
		//draw background skin
		Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_LbxImgTbl[A_Stat].Id );
		if ( NULL != Tv_PtrGdib )
		{
			SkinDrawByPos( m_LbxImgTbl[A_Stat].Atb, Tv_PtrGdib, A_PtrRc );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::LcSubPntIcon( __s32 A_Idx, Ptr_GrRect A_PtrRc )
{
	// local -------------------
		__s32		Tv_WkIdx;
		St_GrRect		Tv_Rc;
		__u8	Tv_IconIdx;
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		if ( NULL != m_ItemTbl[A_Idx] )
		{
			PaintClipperSet( TRUE, A_PtrRc );
			Tv_Rc.left			=	A_PtrRc->left;
			Tv_Rc.top				=	A_PtrRc->top;
			Tv_Rc.right			=	Tv_Rc.left + (__s32)m_IconWidth;
			Tv_Rc.bottom		=	A_PtrRc->bottom;
			for ( Tv_WkIdx = 0;Tv_WkIdx < m_IconCnt;Tv_WkIdx ++ )
			{
				Tv_IconIdx	=	m_ItemTbl[A_Idx]->Icons[Tv_WkIdx];
				if ( 0 != Tv_IconIdx )
				{
					Tv_IconIdx --;
					//check exist icon
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_LbxIconTbl[Tv_IconIdx].Id );
					if ( NULL != Tv_PtrGdib )
					{
						//draw
						SkinDrawByPos( m_LbxIconTbl[Tv_IconIdx].Atb, Tv_PtrGdib, &Tv_Rc );
					}
				}
				//next
				Tv_Rc.left		=	Tv_Rc.right + (__s32)m_IconGap;
				Tv_Rc.right		=	Tv_Rc.left + (__s32)m_IconWidth;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::LcSubPntText( __s32 A_Idx, Ptr_GrRect A_PtrRc, __s32 A_Stat )
{
	// local -------------------
	// code --------------------
		PaintClipperSet( TRUE, A_PtrRc );
		//draw text
		TextDraw( m_ItemTbl[A_Idx]->Str, &m_TextPrar, m_LbxTxtClrTbl[A_Stat], A_PtrRc );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::LcPaintItem( __s32 A_Idx, Ptr_GrRect A_PtrRc )
{
	// local -------------------
		__u32	Tv_Stat;
		St_GrRect	Tv_RcItmCli;
		St_GrRect	Tv_RcIcon;
		St_GrRect	Tv_RcText;
	// code --------------------
		//get item status
		Tv_Stat		=	0;
		if ( A_Idx == m_CusIdx )
		{
			Tv_Stat	=	E_GrUiDoListboxStatCusNofcs;
			if ( IsOnMouse() || IsFocused() )
			{
				Tv_Stat --;
			}
		}

		//draw background
		LcSubPntItemBg( A_Idx, A_PtrRc, Tv_Stat );

		//calculate item client rect
		Tv_RcItmCli.left		=	A_PtrRc->left + (__s32)m_ItemMgn.left;
		Tv_RcItmCli.top			=	A_PtrRc->top + (__s32)m_ItemMgn.top;
		Tv_RcItmCli.right		=	A_PtrRc->right - (__s32)m_ItemMgn.right;
		Tv_RcItmCli.bottom	=	A_PtrRc->bottom - (__s32)m_ItemMgn.bottom;
		//check draw able
		if ( (Tv_RcItmCli.right > Tv_RcItmCli.left) && (Tv_RcItmCli.bottom > Tv_RcItmCli.top) )
		{
			//calculate rect
			switch ( m_IconPos )
			{
				case E_GrUiDoListboxIconTop:
					Tv_RcIcon.left			=	Tv_RcItmCli.left;
					Tv_RcIcon.top				=	Tv_RcItmCli.top;
					Tv_RcIcon.right			=	Tv_RcItmCli.right;
					Tv_RcIcon.bottom		=	Tv_RcIcon.top + ((Tv_RcItmCli.bottom - Tv_RcItmCli.top)/2);
					Tv_RcText.left			=	Tv_RcItmCli.left;
					Tv_RcText.top				=	Tv_RcIcon.bottom;
					Tv_RcText.right			=	Tv_RcItmCli.right;
					Tv_RcText.bottom		=	Tv_RcItmCli.bottom;
					break;
				case E_GrUiDoListboxIconRight:
					Tv_RcIcon.left			=	Tv_RcItmCli.right - ( ((__s32)m_IconWidth + (__s32)m_IconGap) * (__s32)m_IconCnt );
					Tv_RcIcon.top				=	Tv_RcItmCli.top;
					Tv_RcIcon.right			=	Tv_RcItmCli.right;
					Tv_RcIcon.bottom		=	Tv_RcItmCli.bottom;
					Tv_RcText.left			=	Tv_RcItmCli.left;
					Tv_RcText.top				=	Tv_RcItmCli.top;
					Tv_RcText.right			=	Tv_RcIcon.left;
					Tv_RcText.bottom		=	Tv_RcItmCli.bottom;
					break;
				case E_GrUiDoListboxIconBottom:
					Tv_RcIcon.left			=	Tv_RcItmCli.left;
					Tv_RcIcon.top				=	Tv_RcItmCli.top + ((Tv_RcItmCli.bottom - Tv_RcItmCli.top)/2);
					Tv_RcIcon.right			=	Tv_RcItmCli.right;
					Tv_RcIcon.bottom		=	Tv_RcItmCli.bottom;
					Tv_RcText.left			=	Tv_RcItmCli.left;
					Tv_RcText.top				=	Tv_RcIcon.bottom;
					Tv_RcText.right			=	Tv_RcItmCli.right;
					Tv_RcText.bottom		=	Tv_RcItmCli.bottom;
					break;
				default:
					Tv_RcIcon.left			=	Tv_RcItmCli.left;
					Tv_RcIcon.top				=	Tv_RcItmCli.top;
					Tv_RcIcon.right			=	Tv_RcItmCli.left + ( ((__s32)m_IconWidth + (__s32)m_IconGap) * (__s32)m_IconCnt );
					Tv_RcIcon.bottom		=	Tv_RcItmCli.bottom;
					Tv_RcText.left			=	Tv_RcIcon.right;
					Tv_RcText.top				=	Tv_RcItmCli.top;
					Tv_RcText.right			=	Tv_RcItmCli.right;
					Tv_RcText.bottom		=	Tv_RcItmCli.bottom;
					break;
			}

			//draw icon
			LcSubPntIcon( A_Idx, &Tv_RcIcon );

			//draw text
			LcSubPntText( A_Idx, &Tv_RcText, Tv_Stat );
		}

		//draw boarder
		PaintClipperSet( TRUE, A_PtrRc );
		m_GdibScr->DrawBoarder( m_ItemBrdAtb, m_ItemBrdDepth, m_BgBrdClrTbl, A_PtrRc );
		
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::LcScrlPageUp( void )
{
	// local -------------------
		__s32		Tv_CusIdx;
	// code --------------------
		//scroll up
		Tv_CusIdx		=	m_CusIdx - m_PageCnt;
		if ( 0 > Tv_CusIdx )
		{
			Tv_CusIdx	=	0;
		}
		ItemCusIdxSet( Tv_CusIdx );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::LcScrlPageDown( void )
{
	// local -------------------
		__s32		Tv_CusIdx;
	// code --------------------
		//scroll up
		Tv_CusIdx		=	m_CusIdx + m_PageCnt;
		if ( m_ItemCnt <= Tv_CusIdx )
		{
			Tv_CusIdx	=	m_ItemCnt - 1;		
		}
		if ( 0 > Tv_CusIdx )
		{
			Tv_CusIdx		=	0;
		}
		ItemCusIdxSet( Tv_CusIdx );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::LcCusUp( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		//scroll up
		if ( 0 < m_CusIdx )
		{
			ItemCusIdxSet( m_CusIdx - 1 );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::LcCusDown( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( (0 <= m_CusIdx) && ( m_ItemCnt > (m_CusIdx + 1) ) )
		{
			ItemCusIdxSet( m_CusIdx + 1 );
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrUiDoListbox::LcGetItemIdxByScrPos( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		__s32		Tv_Result;
		St_GrRect	Tv_RcCli;
		__s32		Tv_WkY;
		__s32		Tv_ItmIdx;
		__s32		Tv_VwIdx;
	// code --------------------
		//init
		Tv_Result	=	-1;

		//check exist item
		if ( 0 != m_PageCnt )
		{
			//get client area
			if ( ClientRectGet( &Tv_RcCli ) )
			{
				//check x position
				if ( ( A_X >= Tv_RcCli.left) && (A_X < Tv_RcCli.right) )
				{
					//find
					Tv_ItmIdx		=	m_VwsIdx;
					Tv_VwIdx		=	0;
					Tv_WkY			=	Tv_RcCli.top;
					while ( (Tv_ItmIdx < m_ItemCnt) && (Tv_VwIdx < m_PageCnt) )
					{
						//check in range
						if ( (A_Y >= Tv_WkY) && (A_Y < (Tv_WkY + (__s32)m_ItemHeight)) )
						{
							//finded
							Tv_Result		=	Tv_ItmIdx;
							break;
						}

						//next
						Tv_ItmIdx ++;
						Tv_VwIdx ++;
						Tv_WkY	=	Tv_WkY + (__s32)m_ItemHeight + (__s32)m_ItemGap;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ItemGapSet( __u8 A_Gap )
{
		if ( m_ItemGap != A_Gap )
		{
			m_ItemGap	=	A_Gap;
			LcPageCntUpdta();
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoListbox::ItemGapGet( void )
{
		return	m_ItemGap;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ItemColorSet( __u32 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoListboxStatCnt > A_Idx )
		{
			if ( m_LbxClrTbl[A_Idx] !=	A_Color )
			{
				m_LbxClrTbl[A_Idx]		=	A_Color;
				ReqDraw();
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoListbox::ItemColorGet( __u32 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrUiDoListboxStatCnt > A_Idx )
		{
			Tv_Result	=	m_LbxClrTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::LbxColorSet( __u32 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoListboxBgCnt > A_Idx )
		{
			if ( m_LbxBgClrTbl[A_Idx] !=	A_Color )
			{
				m_LbxBgClrTbl[A_Idx]		=	A_Color;
				ReqDraw();
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoListbox::LbxColorGet( __u32 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrUiDoListboxBgCnt > A_Idx )
		{
			Tv_Result	=	m_LbxBgClrTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::TextColorSet( __u32 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoListboxStatCnt > A_Idx )
		{
			if ( m_LbxTxtClrTbl[A_Idx] !=	A_Color )
			{
				m_LbxTxtClrTbl[A_Idx]		=	A_Color;
				ReqDraw();
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoListbox::TextColorGet( __u32 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrUiDoListboxStatCnt > A_Idx )
		{
			Tv_Result	=	m_LbxTxtClrTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::IconGapSet( __u8 A_Gap )
{
		if ( m_IconGap != A_Gap )
		{
			m_IconGap	=	A_Gap;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoListbox::IconGapGet( void )
{
		return	m_IconGap;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ScrlSbarIdSet( __u16 A_Id )
{
	// local -------------------
	// code --------------------
		m_LbxSbarId		=	A_Id;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoListbox::ScrlSbarIdGet( void )
{
		return	m_LbxSbarId;
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoListbox::ItemAdd( WCHAR* A_Str, __u32 A_UserData )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	-1;

		if ( E_GrUiDoListboxMaxItemCnt > m_ItemCnt )
		{
			//allocate new item
			m_ItemTbl[m_ItemCnt]	=	(Ptr_GrUiDoListboxItem)malloc( sizeof(St_GrUiDoListboxItem) );
			if ( NULL != m_ItemTbl[m_ItemCnt] )
			{
				Tv_Result	=	m_ItemCnt;
				//reset icon map
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoListboxIconMax;Tv_WkIdx++ )
				{
					m_ItemTbl[Tv_Result]->Icons[Tv_WkIdx]	=	E_GrUiDoListboxIconIdxNone;
				}
				m_ItemTbl[Tv_Result]->UserData	=	A_UserData;
				GrStrWcopy( m_ItemTbl[Tv_Result]->Str, A_Str, E_GrUiDoListboxLineStrSize - 1 );
				m_ItemCnt ++;
				//update scroll bar
				LcSbarUpdate();
				ReqDraw();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ItemStrGet( __s32 A_Idx, WCHAR* A_StrRt )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( (__u32)m_ItemCnt > (__u32)A_Idx )
		{
			GrStrWcopy( A_StrRt, m_ItemTbl[A_Idx]->Str );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ItemStrSet( __s32 A_Idx, WCHAR* A_Str )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( (__u32)m_ItemCnt > (__u32)A_Idx )
		{
			GrStrWcopy( m_ItemTbl[A_Idx]->Str, A_Str, E_GrUiDoListboxLineStrSize - 1 );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ItemClear( void )
{
	// local -------------------
		__s32		Tv_WkIdx;
	// code --------------------
		if ( 0 != m_ItemCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_ItemCnt;Tv_WkIdx++ )
			{
				free( m_ItemTbl[Tv_WkIdx] );
				m_ItemTbl[Tv_WkIdx]		=	NULL;
			}
			m_CusIdx		=	0;
			m_VwsIdx		=	0;
			m_ItemCnt		=	0;
			LcSbarUpdate();
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ItemIconSet( __s32 A_Idx, __u8 A_PosIdx, __u8 A_IconIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( (__u32)m_ItemCnt > (__u32)A_Idx )
		{
			//check icon index
			if ( (E_GrUiDoListboxIconMax > A_PosIdx) && (E_GrUiDoListboxIconMax >= A_IconIdx) )
			{
				m_ItemTbl[A_Idx]->Icons[A_PosIdx]	=	A_IconIdx;
				Tv_Result	=	TRUE;
				ReqDraw();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoListbox::ItemIconGet( __s32 A_Idx, __u8 A_PosIdx )
{
	// local -------------------
		__u8		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	E_GrUiDoListboxIconIdxNone;

		if ( (__u32)m_ItemCnt > (__u32)A_Idx )
		{
			//check icon index
			if (E_GrUiDoListboxIconMax > A_PosIdx)
			{
				Tv_Result	=	m_ItemTbl[A_Idx]->Icons[A_PosIdx];
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ItemViewStartSet( __s32 A_Idx )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		if ( (__u32)m_ItemCnt > (__u32)A_Idx )
		{
			m_VwsIdx	=	A_Idx;
			Tv_Result	=	TRUE;
			//update scroll bar
			LcSbarUpdate();
			ReqDraw();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoListbox::ItemViewStartGet( void )
{
		return	m_VwsIdx;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ItemCusIdxSet( __s32 A_Idx )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		if ( (__u32)m_ItemCnt > (__u32)A_Idx )
		{
			//update cursor idx
			m_CusIdx	=	A_Idx;
			//check change view area
			if ( (m_VwsIdx > A_Idx) || ( (m_VwsIdx + m_PageCnt) <= A_Idx )  )
			{
				ItemViewStartSet( A_Idx );
			}
			Tv_Result	=	TRUE;
			//redraw
			ReqDraw();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoListbox::ItemCusIdxGet( void )
{
		return	m_CusIdx;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoListbox::ItemCount( void )
{
		return	(__u32)m_ItemCnt;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ItemHeightSet( __u8 A_Size )
{
		if ( m_ItemHeight != A_Size )
		{
			m_ItemHeight	=	A_Size;
			LcPageCntUpdta();
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoListbox::ItemHeightGet( void )
{
		return	m_ItemHeight;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ItemMarginSet( Ptr_GrByteRect A_PtrRc )
{
		m_ItemMgn		=	*A_PtrRc;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ItemMarginGet( Ptr_GrByteRect A_PtrRc )
{
		*A_PtrRc	=	m_ItemMgn;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ItemBrdDepthSet( __u8 A_Depth )
{
		if ( m_ItemBrdDepth != A_Depth )
		{
			m_ItemBrdDepth	=	A_Depth;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoListbox::ItemBrdDepthGet( void )
{
		return	m_ItemBrdDepth;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ItemBrdAtbSet( __u8 A_Atb )
{
		if ( m_ItemBrdAtb != A_Atb )
		{
			m_ItemBrdAtb		=	A_Atb;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoListbox::ItemBrdAtbGet( void )
{
		return	m_ItemBrdAtb;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::IconWidthSet( __u8 A_Width )
{
		if ( m_IconWidth != A_Width )
		{
			m_IconWidth		=	A_Width;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoListbox::IconWidthGet( void )
{
		return	m_IconWidth;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::IconCountSet( __u8 A_Cnt )
{
		if ( (m_IconCnt != A_Cnt) && (E_GrUiDoListboxIconMax > A_Cnt) )
		{
			m_IconCnt		=	A_Cnt;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoListbox::IconCountGet( void )
{
		return	m_IconCnt;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::IconPosSet( __u8 A_Pos )
{
		if ( m_IconPos != A_Pos )
		{
			m_IconPos		=	A_Pos;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoListbox::IconPosGet( void )
{
		return	m_IconPos;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ItemSkinSet( __u32 A_Idx, Ptr_GrUiDoSkinImg A_PtrSkin )
{
		if ( E_GrUiDoListboxStatCnt > A_Idx )
		{
			m_LbxImgTbl[A_Idx]	=	*A_PtrSkin;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ItemSkinGet( __u32 A_Idx, Ptr_GrUiDoSkinImg A_PtrRtSkin )
{
	// local -------------------
	// code --------------------
		//init
		A_PtrRtSkin->Id		=	E_GrUiImgIdNone;
		A_PtrRtSkin->Atb	=	0;
		if ( E_GrUiDoListboxStatCnt > A_Idx )
		{
			*A_PtrRtSkin	=	m_LbxImgTbl[A_Idx];
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::IconSet( __u32 A_Idx, Ptr_GrUiDoSkinImg A_PtrSkin )
{
		if ( E_GrUiDoListboxIconMax > A_Idx )
		{
			m_LbxIconTbl[A_Idx]		=	*A_PtrSkin;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::IconGet( __u32 A_Idx, Ptr_GrUiDoSkinImg A_PtrRtSkin )
{
	// local -------------------
	// code --------------------
		//init
		A_PtrRtSkin->Id			=	E_GrUiImgIdNone;
		A_PtrRtSkin->Atb		=	0;
		if ( E_GrUiDoListboxIconMax > A_Idx )
		{
			*A_PtrRtSkin	=	m_LbxIconTbl[A_Idx];
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ItemUserdataSet( __s32 A_Idx, __u32 A_Val )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check item index
		if ( (__u32)m_ItemCnt > (__u32)A_Idx )
		{
			//set value
			m_ItemTbl[A_Idx]->UserData	=	A_Val;
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoListbox::ItemUserdataGet( __s32 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	0;

		//check item index
		if ( (__u32)m_ItemCnt > (__u32)A_Idx )
		{
			//set value
			Tv_Result		=	m_ItemTbl[A_Idx]->UserData;
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ItemInsert( __s32 A_Idx, WCHAR* A_Str, __u32 A_UserData )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check item index
		if ( ((__u32)m_ItemCnt >= (__u32)A_Idx) && (E_GrUiDoListboxMaxItemCnt > m_ItemCnt) )
		{
			//leave space
			if ( (__u32)m_ItemCnt > (__u32)A_Idx )
			{
				for ( Tv_WkIdx = m_ItemCnt;Tv_WkIdx > A_Idx;Tv_WkIdx-- )
				{
					m_ItemTbl[Tv_WkIdx]	=	m_ItemTbl[Tv_WkIdx-1];
				}
			}
			//reset item
			m_ItemTbl[A_Idx]	=	NULL;
			//allocate new item
			m_ItemTbl[A_Idx]	=	(Ptr_GrUiDoListboxItem)malloc( sizeof(St_GrUiDoListboxItem) );
			if ( NULL != m_ItemTbl[A_Idx] )
			{
				//reset icon map
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoListboxIconMax;Tv_WkIdx++ )
				{
					m_ItemTbl[A_Idx]->Icons[Tv_WkIdx]	=	E_GrUiDoListboxIconIdxNone;
				}
				m_ItemTbl[A_Idx]->UserData	=	A_UserData;
				GrStrWcopy( m_ItemTbl[A_Idx]->Str, A_Str, E_GrUiDoListboxLineStrSize - 1 );
				m_ItemCnt ++;
				//update scroll bar
				LcSbarUpdate();
				ReqDraw();
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ItemDelete( __s32 A_Idx )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check item index
		if ( (0 != m_ItemCnt) && ((__u32)m_ItemCnt > (__u32)A_Idx ) )
		{
			//release item
			free( m_ItemTbl[A_Idx] );
			m_ItemTbl[A_Idx]	=	NULL;

			//gather item
			if ( (m_ItemCnt - 1) != A_Idx )
			{
				for ( Tv_WkIdx = A_Idx+1;Tv_WkIdx < m_ItemCnt;Tv_WkIdx++ )
				{
					m_ItemTbl[Tv_WkIdx-1]	=	m_ItemTbl[Tv_WkIdx];
				}
				//patch last
				m_ItemTbl[m_ItemCnt-1]	=	NULL;
			}

			//count descreate
			m_ItemCnt --;
			//patch cursor
			if ( (__u32)m_ItemCnt <= (__u32)m_CusIdx )
			{
				m_CusIdx	=	m_ItemCnt - 1;
				if ( 0 > m_CusIdx )
				{
					m_CusIdx	=	0;
				}
			}
			//patch view positon
			if ( (__u32)m_ItemCnt <= (__u32)m_VwsIdx )
			{
				m_VwsIdx	=	m_ItemCnt - 1;
				if ( 0 > m_VwsIdx )
				{
					m_VwsIdx	=	0;
				}
			}
			//update scroll bar
			LcSbarUpdate();
			ReqDraw();
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::LcPageCntUpdta( void )
{
	// local -------------------
		St_GrRect		Tv_RcCli;
		__s32					Tv_CliHeight;
	// code --------------------
		//get client size
		m_PageCnt		=	0;
		if ( ClientRectGet( &Tv_RcCli ) )
		{
			//calculate client area height
			Tv_CliHeight	=	Tv_RcCli.bottom - Tv_RcCli.top;
			//loop
			while ( Tv_CliHeight >= (__s32)m_ItemHeight )
			{
				m_PageCnt ++;
				//next
				Tv_CliHeight	=	Tv_CliHeight - ( (__s32)m_ItemHeight + (__s32)m_ItemGap );
			}
			//check over
			if ( E_GrUiDoListboxMaxItemCnt < m_PageCnt )
			{
				m_PageCnt	=	E_GrUiDoListboxMaxItemCnt;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::RectSet( Ptr_GrRect A_PtrRect )
{
		//inherited
		Cls_GrUiDoSkin::RectSet( A_PtrRect );
		//update rect
		LcPageCntUpdta();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ProcUiMsg( Ptr_GrUiMsgHd A_PtrMsg )
{
		//default event handler
		switch( A_PtrMsg->Cmd )
		{
			case	E_GrUiMsgCmdSbarChg:
				ProcEvtSbarChg( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			case	E_GrUiMsgCmdBtnUp:
				ProcEvtBtnUp( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
				break;
			default:
				Cls_GrUiDoSkin::ProcUiMsg( A_PtrMsg );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ProcEvtSbarChg( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		Cls_GrUiDoSbar*	Tv_Sbar;
		__s32		Tv_CusPos;
		__s32		Tv_Min;
		__s32		Tv_Max;
		__s32		Tv_Pos;
		__s32		Tv_Rng;
	// code --------------------
		//check need calculate
		Tv_CusPos	=	0;
		if ( 1 < m_ItemCnt )
		{
			Tv_Sbar	=	(Cls_GrUiDoSbar*)A_Obj;
			Tv_Max	=	Tv_Sbar->RangeMaxGet();
			Tv_Min	=	Tv_Sbar->RangeMinGet();
			Tv_Pos	=	Tv_Sbar->PosGet();
			Tv_Rng	=	Tv_Max - Tv_Min;
			if ( 0 < Tv_Rng )
			{
				Tv_CusPos	=	((Tv_Pos - Tv_Min) * (m_ItemCnt - 1)) / Tv_Rng;
			}
			//update position
			m_IsSbarLock	=	TRUE;
			ItemCusIdxSet( Tv_CusPos );
			m_IsSbarLock	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ProcEvtBtnUp( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		__u32		Tv_UserData;
	// code --------------------
		Tv_UserData	=	A_Obj->UserDataGet();
		switch ( Tv_UserData )
		{
			case E_GrUiDoListboxBudScrlUp:
				LcCusUp();
				break;
			case E_GrUiDoListboxBudScrlDown:
				LcCusDown();
				break;
			case E_GrUiDoListboxBudDelete:
				ItemDelete( m_CusIdx );
				break;
			case E_GrUiDoListboxBudClear:
				ItemClear();
				break;
			case E_GrUiDoListboxBudScrlHome:
				ItemCusIdxSet( 0 );
				break;
			case E_GrUiDoListboxBudScrlEnd:
				if ( 0 != m_ItemCnt )
				{
					ItemCusIdxSet( m_ItemCnt -1 );
				}
				break;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ProcEvtKeyDown( __u8 A_Key )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		//check enter key
		switch( A_Key )
		{
			case	E_GrUiKeyUp:
				Tv_Result	=	LcProcKeyDownUp();
				break;
			case	E_GrUiKeyDown:
				Tv_Result	=	LcProcKeyDownDown();
				break;
			case	E_GrUiKeyLeft:
			case	E_GrUiKeyRight:
				Tv_Result	=	m_IsLbxCtrl;
				if ( m_IsKeyFocusCtrl )
				{
					Tv_Result	=	FALSE;
				}
				break;
			case	E_GrUiKeyValMinus:
				LcScrlPageUp();
				Tv_Result	=	TRUE;
				break;
			case	E_GrUiKeyValPlus:
				LcScrlPageDown();
				Tv_Result	=	TRUE;
				break;

		}
		//default process
		if ( !Tv_Result )
		{
			Tv_Result	=	Cls_GrUiDoSkin::ProcEvtKeyDown( A_Key );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ProcEvtKeyUp( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		switch( A_Key )
		{
			case	E_GrUiKeyEnter:
				Tv_Result	=	LcProcKeyUpEnter();
				break;
			case	E_GrUiKeyMenu:
			case	E_GrUiKeyEsc:
				Tv_Result	=	LcProcKeyUpEsc();
				break;
			case	E_GrUiKeyDelete:
				Tv_Result	=	LcProcKeyDownDel();
				break;
		}
				
		//default process
		if ( !Tv_Result )
		{
			Tv_Result	=	Cls_GrUiDoSkin::ProcEvtKeyUp( A_Key );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::LcProcKeyUpEnter( void )
{
		if ( m_IsLbxCtrl || m_IsKeyFocusCtrl )
		{
			if ( m_IsEditAble )
			{
				LcVirtualKbdShow();
			}
			else
			{
				LcNotifyItemClick();
			}
		}
		else
		{
			m_IsLbxCtrl		=	TRUE;
			ReqDraw();
		}
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::LcProcKeyUpEsc( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( m_IsLbxCtrl )
		{
			m_IsLbxCtrl	=	FALSE;
			ReqDraw();
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::LcProcKeyDownUp( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		if ( m_IsLbxCtrl || m_IsKeyFocusCtrl )
		{
			Tv_Result	=	LcCusUp();
			if ( !m_IsKeyFocusCtrl )
			{
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::LcProcKeyDownDown( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		if ( m_IsLbxCtrl || m_IsKeyFocusCtrl )
		{
			Tv_Result	=	LcCusDown();
			if ( !m_IsKeyFocusCtrl )
			{
				Tv_Result		=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__s32		Tv_ItmIdx;
	// code --------------------
		//release control mode
		Tv_ItmIdx	=	LcGetItemIdxByScrPos( A_X, A_Y );
		if ( -1 != Tv_ItmIdx )
		{
			m_IsLbxCtrl		=	FALSE;
			//process
			ItemCusIdxSet( Tv_ItmIdx );
			if ( m_IsEditAble )
			{
				LcVirtualKbdShow();
			}
			else
			{
				LcNotifyItemClick();
			}
		}
		else
		{
			if ( !m_IsLbxCtrl )
			{
				m_IsLbxCtrl		=	TRUE;
				ReqDraw();
			}
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::EditAbleSet( BOOL8 A_IsAble )
{
		m_IsEditAble		=	A_IsAble;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::IsEditAble( void )
{
		return	m_IsEditAble;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::EditTypeNum( BOOL8 A_IsNum )
{
		m_IsEditNumType		=	A_IsNum;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::IsEditNum( void )
{
		return	m_IsEditNumType;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::LcNotifyItemClick( void )
{
	// local -------------------
		St_GrUiMsgLbxItmIdx		Tv_Msg;
	// code --------------------
		//check exist item
		if ( m_ItemCnt > m_CusIdx  )
		{
			if ( V_GrUiMngObj->IsUserEventOn() )
			{
				Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdLbxItemClick;
				Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
				Tv_Msg.Obj				=	(Cls_GrUiDoBase*)this;
				Tv_Msg.ItemIdx		=	m_CusIdx;
				UiMsgToWin( (Ptr_GrUiMsgHd)&Tv_Msg );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::LcNotifyTextChange( void )
{
	// local -------------------
		St_GrUiMsgLbxItmIdx		Tv_Msg;
	// code --------------------
		if ( V_GrUiMngObj->IsUserEventOn() )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdLbxTxtChg;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Obj				=	(Cls_GrUiDoBase*)this;
			Tv_Msg.ItemIdx		=	m_CusIdx;
			UiMsgToWin( (Ptr_GrUiMsgHd)&Tv_Msg );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::LcVirtualKbdShow( void )
{
	// local -------------------
		BOOL8	Tv_IsPasswd;
	// code --------------------
		//check able
		if ( (__u32)m_ItemCnt > (__u32)m_CusIdx )
		{
			Tv_IsPasswd		=	FALSE;
			if ( 0 != ( E_GrUiDoTextAtbPasswd & m_TextPrar.Atb ) )
			{
				Tv_IsPasswd	=	TRUE;
			}
			m_ObjConsole->VirtKbdCreate( ((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet(), m_ObjId, 
				m_IsEditNumType, Tv_IsPasswd, (__u32)m_ItemTbl[m_CusIdx], m_ItemTbl[m_CusIdx]->Str  );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ProcEvtFocusGet( void )
{
	// local -------------------
		Cls_GrUiDoWin*	Tv_Win;
	// code --------------------
		//check restore parameter
		Tv_Win	=	(Cls_GrUiDoWin*)m_ObjWin;
		if ( this == Tv_Win->LastFocusObjGet() )
		{
			//restore parameter
			m_IsLbxCtrl	=	(BOOL8)Tv_Win->LastFocusParaGet( 0 );
		}
		Cls_GrUiDoSkin::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::ProcEvtFocusLoss( void )
{
		//update focus parameter
		((Cls_GrUiDoWin*)m_ObjWin)->RtlLastFocusParaSet( 0, (__u32)m_IsLbxCtrl );
		m_IsLbxCtrl		=	FALSE;
		Cls_GrUiDoSkin::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ProcEvtWheelUp( void )
{
		LcCusUp();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::ProcEvtWheelDown( void )
{
		LcCusDown();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::LcProcKeyDownDel( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		WCHAR*	Tv_Str;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check edit able
		if ( m_IsLbxCtrl || m_IsKeyFocusCtrl )
		{
			if ( m_IsKeyDelAble )
			{
				if ( m_IsKeyDelQuest )
				{
					//get delete message
					Tv_Str	=	V_GrUiMngObj->TextPtrGet( E_GrUiTxtIdLbxDelMsg );
					if ( NULL != Tv_Str )
					{
						//check quest delete
						if ( E_GrUiDoMsgBoxRsltYes == MsgBox( L"", Tv_Str, E_GrUiDoMsgBoxTypeYesNo ) )
						{
							//check exist item
							ItemDelete( (__s32)m_CusIdx );
						}
					}
					Tv_Result	=	TRUE;
				}
				else
				{
					//check exist item
					ItemDelete( m_CusIdx );
					Tv_Result	=	TRUE;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::KeyDelAbleSet( BOOL8 A_IsAble )
{
		m_IsKeyDelAble	=	A_IsAble;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::IsKeyDelAble( void )
{
		return	m_IsKeyDelAble;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::KeyDelQuestSet( BOOL8 A_IsQuest )
{
		m_IsKeyDelQuest		=	A_IsQuest;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::IsKeyDelQuest( void )
{
		return	m_IsKeyDelQuest;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::TextParaSet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( &m_TextPrar, A_PtrPara, sizeof(St_GrUiDoTextPara) );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::TextParaGet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( A_PtrPara, &m_TextPrar, sizeof(St_GrUiDoTextPara) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoListbox::IsKeyFocusCtrl( void )
{
		return	m_IsKeyFocusCtrl;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoListbox::KeyFocusCtrlSet( BOOL8 A_IsSet )
{
		m_IsKeyFocusCtrl	=	A_IsSet;
}
//--------------------------------------------------------------------

