/*
 solo 6000 chip
 */

//====================================================================
// uses
#include	<GrLnxApp.h>

#include	<GrTypeBase.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>

#include	<stdlib.h>
#include	<W32/GrW32QuadDib.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_GrW32QuadDib::Cls_GrW32QuadDib( __u32 A_DibBitCnt ):
Cls_GrDvrQuad()
{
	// local -------------------
	// code --------------------
		//init
		m_ChCnt				=	E_GrW32QuadDibMaxChCnt;

		m_QdScrResX		=	0;
		m_QdScrResY		=	0;

		m_PipCnt			=	1;

		m_DibBitCnt		=	A_DibBitCnt;
		m_HbtmScr			=	NULL;
		m_HdcScr			=	NULL;
		//m_HbtmOld			=	NULL;
		GrDumyZeroMem( m_StrId, sizeof(m_StrId) );

}
//--------------------------------------------------------------------
Cls_GrW32QuadDib::~Cls_GrW32QuadDib()
{
	// local -------------------
	// code --------------------
		FreeScr();
}
//--------------------------------------------------------------------
void		Cls_GrW32QuadDib::FreeScr( void )
{
		if ( NULL != m_HbtmScr )
		{
			DeleteDC( m_HdcScr );
			m_HdcScr		=	NULL;
			DeleteObject( (HGDIOBJ)m_HbtmScr );
			m_HbtmScr		=	NULL;
		}
}
//--------------------------------------------------------------------
void		Cls_GrW32QuadDib::SetScrRes( __u32 A_ResX, __u32 A_ResY )
{
	// local -------------------
		HDC					Tv_Dc;
	// code --------------------
		//release preview scr
		FreeScr();

		//create bitmap
		if ( (0 != A_ResX) && (0 != A_ResY) )
		{
			//create dc
			Tv_Dc			=	GetDC( NULL );
			m_HdcScr	=	CreateCompatibleDC( Tv_Dc );
			//set font
			SelectObject( m_HdcScr, GetStockObject( DC_PEN ) );
			SelectObject( m_HdcScr, GetStockObject( DC_BRUSH ) );
			SelectObject( m_HdcScr, GetStockObject(DEFAULT_GUI_FONT) );
			SetBkMode( m_HdcScr, TRANSPARENT );
			SetStretchBltMode( m_HdcScr, STRETCH_HALFTONE );

			//create bitmap
			m_QdScrResX	=	A_ResX;
			m_QdScrResY	=	A_ResY;
			m_HbtmScr		=	CreateCompatibleBitmap( Tv_Dc, (__s32)m_QdScrResX, (__s32)m_QdScrResY );
			if ( NULL != m_HbtmScr )
			{
				//set bitmap
				SelectObject( m_HdcScr, (HGDIOBJ)m_HbtmScr );
				//redraw quad
				ClearQuad();
				DrawAllChWaku();
			}
			else
			{
				//SelectObject( m_HbtmScr, m_HfontOld );
				DeleteDC( m_HdcScr );
				m_HdcScr		=	NULL;
			}
			ReleaseDC( NULL, Tv_Dc );
		}
}
//--------------------------------------------------------------------
BOOL8		Cls_GrW32QuadDib::QuadSetMode( __u32 A_Mode, __u32 A_Page )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//update mode
		Tv_Result		=	Cls_GrDvrQuad::QuadSetMode( A_Mode, A_Page );
		ClearQuad();
		DrawAllChWaku();
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void		Cls_GrW32QuadDib::DrawChWaku( __u32 A_Ch )
{
	// local -------------------
		St_GrRect		Tv_RcCh;
		__u32				Tv_LenStr;
		SIZE				Tv_AreaSize;
		__s32					Tv_X;
		__s32					Tv_Y;
	// code --------------------
		//check able
		if ( NULL != m_HbtmScr )
		{
			//get channel rect
			if ( ChGetViewRect( A_Ch, &Tv_RcCh ) )
			{
				//draw out boarder
				SetDCPenColor( m_HdcScr, 0xFFFFFF );
				MoveToEx( m_HdcScr, Tv_RcCh.left, Tv_RcCh.top, NULL );
				LineTo( m_HdcScr, Tv_RcCh.right - 1, Tv_RcCh.top );
				LineTo( m_HdcScr, Tv_RcCh.right - 1, Tv_RcCh.bottom - 1 );
				LineTo( m_HdcScr, Tv_RcCh.left, Tv_RcCh.bottom - 1 );
				LineTo( m_HdcScr, Tv_RcCh.left, Tv_RcCh.top );

				//draw in boarder
				SetDCPenColor( m_HdcScr, 0x000000 );
				MoveToEx( m_HdcScr, Tv_RcCh.left + 1, Tv_RcCh.top + 1, NULL );
				LineTo( m_HdcScr, Tv_RcCh.right - 2, Tv_RcCh.top + 1 );
				LineTo( m_HdcScr, Tv_RcCh.right - 2, Tv_RcCh.bottom - 2 );
				LineTo( m_HdcScr, Tv_RcCh.left + 1, Tv_RcCh.bottom - 2 );
				LineTo( m_HdcScr, Tv_RcCh.left + 1, Tv_RcCh.top + 1 );

				//draw channel text
				Tv_LenStr		=	GrStrLen( m_StrId[A_Ch] );
				if ( GetTextExtentPoint32( m_HdcScr, m_StrId[A_Ch], Tv_LenStr, &Tv_AreaSize  ) )
				{
					//draw bg color
					SetTextColor( m_HdcScr, 0x000000 );
					Tv_X		=	Tv_RcCh.right - Tv_AreaSize.cx - E_GrW32QuadDibTxtGapX;
					Tv_Y		=	Tv_RcCh.bottom - Tv_AreaSize.cy - E_GrW32QuadDibTxtGapY;
					TextOut( m_HdcScr, Tv_X, Tv_Y, m_StrId[A_Ch], Tv_LenStr );
					//draw main
					SetTextColor( m_HdcScr, 0xFFFFFF );
					Tv_X		=	Tv_RcCh.right - Tv_AreaSize.cx - E_GrW32QuadDibTxtGapX - 1;
					Tv_Y		=	Tv_RcCh.bottom - Tv_AreaSize.cy - E_GrW32QuadDibTxtGapY - 1;
					TextOut( m_HdcScr, Tv_X, Tv_Y, m_StrId[A_Ch], Tv_LenStr );
				}

			}
		}
		
}
//--------------------------------------------------------------------
void		Cls_GrW32QuadDib::ClearQuad( void )
{
	// local -------------------
		RECT		Tv_RcScr;
	// code --------------------
		//check able
		if ( NULL != m_HbtmScr )
		{
			//clear dib
			Tv_RcScr.left			=	0;
			Tv_RcScr.top			=	0;
			Tv_RcScr.right		=	(__s32)m_QdScrResX;
			Tv_RcScr.bottom		=	(__s32)m_QdScrResY;
			SetDCBrushColor( m_HdcScr, 0xFF0000 );
			FillRect( m_HdcScr, &Tv_RcScr, (HBRUSH)GetStockObject(DC_BRUSH) );
		}
}
//--------------------------------------------------------------------
void		Cls_GrW32QuadDib::DrawAllChWaku( void )
{
	// local -------------------
		__u32		Tv_Ch;
		BOOL8		Tv_IsPb;
		__u32		Tv_ViewCnt;
		__u32		Tv_ViewIdx;
	// code --------------------
		if ( NULL != m_HbtmScr )
		{
			Tv_ViewCnt	=	QuadGetNowViewCount();
			if ( 0 != Tv_ViewCnt )
			{
				for ( Tv_ViewIdx=0;Tv_ViewIdx < Tv_ViewCnt;Tv_ViewIdx++ )
				{
					//get channel
					if ( QuadGetChByView( Tv_ViewIdx, &Tv_Ch, &Tv_IsPb ) )
					{
						DrawChWaku( Tv_Ch );
					}
				}
			}
		}
		
}
//--------------------------------------------------------------------
HDC			Cls_GrW32QuadDib::GetHdc( void )
{
		return		m_HdcScr;
}
//--------------------------------------------------------------------
HBITMAP	Cls_GrW32QuadDib::GetBitmap( void )
{
		return		m_HbtmScr;
}
//--------------------------------------------------------------------
void		Cls_GrW32QuadDib::SetChDib( __u32 A_Ch, BITMAPINFO* A_PtrBi, 
																		void* A_PtrDib, __u16 A_ResX, __u16 A_ResY )
{
	// local -------------------
		St_GrRect		Tv_RcCh;
	// code --------------------
		if ( NULL != m_HbtmScr )
		{
			if ( ChGetViewRect( A_Ch, &Tv_RcCh ) )
			{
				//draw
				StretchDIBits( m_HdcScr, Tv_RcCh.left, Tv_RcCh.top, 
					Tv_RcCh.right - Tv_RcCh.left, Tv_RcCh.bottom - Tv_RcCh.top,
					0, (-A_PtrBi->bmiHeader.biHeight) - A_ResY, A_ResX, A_ResY, A_PtrDib, A_PtrBi, DIB_RGB_COLORS, SRCCOPY );
				DrawChWaku( A_Ch );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrW32QuadDib::SetChName( __u32 A_Ch, __s8* A_Str )
{
	// local -------------------
	// code --------------------
	//check able
	if ( E_GrW32QuadDibMaxChCnt > A_Ch )
	{
		GrStrCopy( m_StrId[A_Ch], A_Str );
	}
}
//--------------------------------------------------------------------

