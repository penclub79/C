/*
 osd skin object

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Ui/GrUiDoSkin.h>
#include	<Ui/GrUiMng.h>
#include	<GrDrawTool.h>
#include	<Ui/GrUiConsole.h>
#include	<GrDumyTool.h>

//====================================================================
//local const

//====================================================================
//local macro
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*	V_GrUiMngObj;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoSkin::Cls_GrUiDoSkin( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrtn, __u8 A_ConIdx, BOOL8 A_IsIdAlloc ) :
Cls_GrUiDoBase( A_PtrRect, A_ObjPrtn, A_ConIdx, A_IsIdAlloc )
{
	// local -------------------
	// code --------------------
		//init
		m_ClrBg			=	0;
		m_BgBrdDepth		=	E_GrUiDoSkinDfltBrdDepth;
		m_BgBrdAtb			=	0;
		m_IsBgSprite		=	FALSE;
		m_BgBrdClrTbl[E_GrGdibClrIdxBrdLight]			=	m_ObjConsole->ClrWhiteGet();
		m_BgBrdClrTbl[E_GrGdibClrIdxBrdDark]			=	m_ObjConsole->ClrBlackGet();
		m_BgBrdClrTbl[E_GrGdibClrIdxBrdMidLight]	=	m_ObjConsole->ClrWhiteGet();
		m_BgBrdClrTbl[E_GrGdibClrIdxBrdMidDark]		=	m_ObjConsole->ClrBlackGet();

		GrDumyZeroMem( m_BgSkinTbl, sizeof(m_BgSkinTbl) );
}
//--------------------------------------------------------------------
Cls_GrUiDoSkin::~Cls_GrUiDoSkin()
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
__u32		Cls_GrUiDoSkin::BgColorGet( void )
{
		return	m_ClrBg;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::BgColorSet( __u32 A_Color )
{
		m_ClrBg	=	A_Color;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::SkinDrawByPos( __u8 A_Atb, Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8	Tv_IsSpr;
	// code --------------------

		//set srpite mode
		Tv_IsSpr	=	FALSE;
		if ( 0 != (E_GrUiDoSkibAtbSprite & A_Atb) )
		{
			Tv_IsSpr	=	TRUE;
		}
		m_GdibScr->SpriteModeSet( Tv_IsSpr );
		
		//draw
		switch( (E_GrUiDoSkinPosValMask & A_Atb) )
		{
			case	E_GrUiDoSkinPosCenter:
				ProcBgSkinCenter( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosMidLeft:
				ProcBgSkinMidLeft( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosMidRight:
				ProcBgSkinMidRight( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosTopCenter:
				ProcBgSkinTopCenter( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosTopLeft:
				ProcBgSkinTopLeft( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosTopRight:
				ProcBgSkinTopRight( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosBtmCenter:
				ProcBgSkinBtmCenter( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosBtmLeft:
				ProcBgSkinBtmLeft( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosBtmRight:
				ProcBgSkinBtmRight( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosTileAll:
				ProcBgSkinTileAll( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosTileLeft:
				ProcBgSkinTileLeft( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosTileTop:
				ProcBgSkinTileTop( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosTileRight:
				ProcBgSkinTileRight( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosTileBottom:
				ProcBgSkinTileBottom( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosTileMidHz:
				ProcBgSkinTileMidHz( A_PtrDib, A_PtrRect );
				break;
			case	E_GrUiDoSkinPosTileMidVt:
				ProcBgSkinTileMidVt( A_PtrDib, A_PtrRect );
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinCenter( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
	// code --------------------
		//calculate center position
		Tv_X	=	A_PtrRect->left +	(((A_PtrRect->right - A_PtrRect->left) - (__s32)A_PtrDib->ResX) / 2);
		Tv_Y	=	A_PtrRect->top + (((A_PtrRect->bottom - A_PtrRect->top) - (__s32)A_PtrDib->ResY) / 2);

		m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinMidLeft( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
	// code --------------------
		//calculate center position
		Tv_X	=	A_PtrRect->left;
		Tv_Y	=	A_PtrRect->top + (((A_PtrRect->bottom - A_PtrRect->top) - (__s32)A_PtrDib->ResY) / 2);
		m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinMidRight( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
	// code --------------------
		//calculate center position
		Tv_X	=	A_PtrRect->right - (__s32)A_PtrDib->ResX;
		Tv_Y	=	A_PtrRect->top + (((A_PtrRect->bottom - A_PtrRect->top) - (__s32)A_PtrDib->ResY) / 2);
		m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinTopCenter( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
	// code --------------------
		//calculate center position
		Tv_X	=	A_PtrRect->left +	(((A_PtrRect->right - A_PtrRect->left) - (__s32)A_PtrDib->ResX) / 2);
		Tv_Y	=	A_PtrRect->top;
		m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinTopLeft( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
	// code --------------------
		//calculate center position
		Tv_X	=	A_PtrRect->left;
		Tv_Y	=	A_PtrRect->top;
		m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinTopRight( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
	// code --------------------
		//calculate center position
		Tv_X	=	A_PtrRect->right -	(__s32)A_PtrDib->ResX;
		Tv_Y	=	A_PtrRect->top;
		m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinBtmCenter( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
	// code --------------------
		//calculate center position
		Tv_X	=	A_PtrRect->left +	(((A_PtrRect->right - A_PtrRect->left) - (__s32)A_PtrDib->ResX) / 2);
		Tv_Y	=	A_PtrRect->bottom - (__s32)A_PtrDib->ResY;
		m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinBtmLeft( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
	// code --------------------
		//calculate center position
		Tv_X	=	A_PtrRect->left;
		Tv_Y	=	A_PtrRect->bottom - (__s32)A_PtrDib->ResY;
		m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinBtmRight( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
	// code --------------------
		//calculate center position
		Tv_X	=	A_PtrRect->right - (__s32)A_PtrDib->ResX;
		Tv_Y	=	A_PtrRect->bottom - (__s32)A_PtrDib->ResY;
		m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinTileAll( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
		__s32		Tv_WkX;
		__s32		Tv_WkY;
	// code --------------------
		//calculate
		Tv_WkY	=	A_PtrRect->top;
		while ( A_PtrRect->bottom > Tv_WkY )
		{
			Tv_WkX	=	A_PtrRect->left;
			while ( A_PtrRect->right > Tv_WkX )
			{
				Tv_X	=	Tv_WkX;
				Tv_Y	=	Tv_WkY;
				m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
				//next
				Tv_WkX	=	Tv_WkX + A_PtrDib->ResX;
			}
			//next
			Tv_WkY	=	Tv_WkY + A_PtrDib->ResY;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinTileLeft( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
		__s32		Tv_WkY;
	// code --------------------
		//calculate
		Tv_WkY	=	A_PtrRect->top;
		while ( A_PtrRect->bottom > Tv_WkY )
		{
			Tv_X	=	A_PtrRect->left;
			Tv_Y	=	Tv_WkY;
			m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
			//next
			Tv_WkY	=	Tv_WkY + A_PtrDib->ResY;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinTileTop( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
		__s32		Tv_WkX;
	// code --------------------
		//calculate
		Tv_WkX	=	A_PtrRect->left;
		while ( A_PtrRect->right > Tv_WkX )
		{
			Tv_X	=	Tv_WkX;
			Tv_Y	=	A_PtrRect->top;
			m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
			//next
			Tv_WkX	=	Tv_WkX + A_PtrDib->ResX;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinTileRight( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
		__s32		Tv_WkY;
	// code --------------------
		//calculate
		Tv_WkY	=	A_PtrRect->top;
		while ( A_PtrRect->bottom > Tv_WkY )
		{
			Tv_X	=	A_PtrRect->right - (__s32)A_PtrDib->ResX;
			Tv_Y	=	Tv_WkY;
			m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
			//next
			Tv_WkY	=	Tv_WkY + A_PtrDib->ResY;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinTileBottom( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
		__s32		Tv_WkX;
	// code --------------------
		//calculate
		Tv_WkX	=	A_PtrRect->left;
		while ( A_PtrRect->right > Tv_WkX )
		{
			Tv_X	=	Tv_WkX;
			Tv_Y	=	A_PtrRect->bottom - (__s32)A_PtrDib->ResY;
			m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
			//next
			Tv_WkX	=	Tv_WkX + A_PtrDib->ResX;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinTileMidHz( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
		__s32		Tv_WkX;
		__s32		Tv_WkY;
	// code --------------------
		//calculate
		Tv_WkX	=	A_PtrRect->left;
		Tv_WkY	=	A_PtrRect->top + (((A_PtrRect->bottom - A_PtrRect->top) - (__s32)A_PtrDib->ResY) / 2);
		while ( A_PtrRect->right > Tv_WkX )
		{
			Tv_X	=	Tv_WkX;
			Tv_Y	=	Tv_WkY;
			m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
			//next
			Tv_WkX	=	Tv_WkX + A_PtrDib->ResX;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinTileMidVt( Ptr_GrGdib A_PtrDib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
		__s32		Tv_WkX;
		__s32		Tv_WkY;
	// code --------------------
		//calculate
		Tv_WkX	=	A_PtrRect->left +	(((A_PtrRect->right - A_PtrRect->left) - (__s32)A_PtrDib->ResX) / 2);
		Tv_WkY	=	A_PtrRect->top;
		while ( A_PtrRect->bottom > Tv_WkY )
		{
			Tv_X	=	Tv_WkX;
			Tv_Y	=	Tv_WkY;
			m_GdibScr->BltGdib( Tv_X, Tv_Y, A_PtrDib );
			//next
			Tv_WkY	=	Tv_WkY + A_PtrDib->ResY;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgBordDraw( void )
{
	// local -------------------
	// code --------------------
		PaintClipperSet(FALSE);
		m_GdibScr->DrawBoarder( m_BgBrdAtb, m_BgBrdDepth, m_BgBrdClrTbl, &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgSkinTblDraw( void )
{
	// local -------------------
		__u32	Tv_WkIdx;
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		//set clipper
		PaintClipperSet( FALSE );
		//draw
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoSkinMaxCnt;Tv_WkIdx++ )
		{
			Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_BgSkinTbl[Tv_WkIdx].Id );
			if ( NULL != Tv_PtrGdib )
			{
				SkinDrawByPos( m_BgSkinTbl[Tv_WkIdx].Atb, Tv_PtrGdib, &m_PntRcScr );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcBgColorDraw( void )
{
		if ( !m_IsBgSprite || (0 != m_ClrBg) )
		{
			PaintClipperSet( FALSE );
			m_GdibScr->FillRect( NULL, m_ClrBg );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::ProcPaint( void )
{
	// local -------------------
	// code --------------------
		ProcBgColorDraw();			//background color draw
		ProcBgSkinTblDraw();		//skin draw
		ProcBgBordDraw();				//border draw
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSkin::IsBgSpriteMode( void )
{
		return	m_IsBgSprite;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::BgSpriteModeSet( BOOL8 A_IsSprite )
{
	// local -------------------
	// code --------------------
		if ( m_IsBgSprite != A_IsSprite )
		{
			//update
			m_IsBgSprite	=	A_IsSprite;
			//redraw
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoSkin::BorderAtbGet( void )
{
		return	m_BgBrdAtb;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::BorderAtbSet( __u8 A_Atb )
{
		if ( m_BgBrdAtb != A_Atb )
		{
			m_BgBrdAtb	=	A_Atb;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSkin::BorderDepthGet( void )
{
		return	m_BgBrdDepth;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::BorderDepthSet( __u32 A_Depth )
{
		if ( m_BgBrdDepth != A_Depth )
		{
			m_BgBrdDepth	=	A_Depth;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::BgSkinGet( __u32 A_Idx, Ptr_GrUiDoSkinImg A_PtrRtSkin )
{
	// local -------------------
	// code --------------------
		A_PtrRtSkin->Id		=	E_GrUiImgIdNone;
		A_PtrRtSkin->Atb	=	0;
		if ( E_GrUiDoSkinMaxCnt > A_Idx )
		{
			*A_PtrRtSkin	=	m_BgSkinTbl[A_Idx];
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::BgSkinSet( __u32 A_Idx, Ptr_GrUiDoSkinImg A_PtrSkin )
{
	// local -------------------
	// code --------------------
		if ( E_GrUiDoSkinMaxCnt > A_Idx )
		{
			m_BgSkinTbl[A_Idx]	=	*A_PtrSkin;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSkin::BorderColorSet( __u8 A_Idx, __u32 A_Color )
{
		if ( E_GrGdibBrdClrTblCnt > A_Idx )
		{
			m_BgBrdClrTbl[A_Idx]	=	A_Color;
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSkin::BorderColorGet( __u8 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		
		if ( E_GrGdibBrdClrTblCnt > A_Idx )
		{
			Tv_Result	=	m_BgBrdClrTbl[A_Idx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------


