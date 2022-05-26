/*
 osd skin object

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrGdibObj.h>
#include	<GrImgBase.h>
#include	<GrDumyTool.h>
#include	<GrMath.h>
#include	<stdlib.h>
#include	<GrFileCtrl.h>
#include	<GrFileTool.h>
#include	<GrBmpTool.h>

#include <FontV3/Dib2/GrFontV3Dib2Base.h>

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
Cls_GrGdib::Cls_GrGdib( __u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
												__u32 A_Pitch, void* A_PtrDib, 
												__u32 A_PltBitCnt, void* A_PtrPlt )
{
	// local -------------------
	// code --------------------
		//init header
		m_IsSprite		=	FALSE;
		m_Rect.left		=	0;
		m_Rect.top		=	0;
		m_Rect.right	=	A_ResX;
		m_Rect.bottom	=	A_ResY;
		m_PtrDib			=	NULL;
		m_PtrPlt			=	NULL;
		GrGdibInitHead( &m_Gdib, A_Fcc, A_ResX, A_ResY, A_Pitch, A_PtrDib, A_PltBitCnt, A_PtrPlt );
		m_DibSize	=	(m_Gdib.Pitch * m_Gdib.ResY);
		if ( NULL == A_PtrDib )
		{
			m_PtrDib	=	malloc( m_DibSize );
			m_Gdib.PtrExtDib	=	m_PtrDib;
		}
		if ( m_Gdib.IsPlt && (NULL == A_PtrPlt) )
		{
			//allocate pallete buffer
			m_PtrPlt	=	malloc( 4 * (1 << m_Gdib.BitCnt) );
			m_Gdib.PtrExtPlt	=	m_PtrPlt;
		}
		//build pixel struct
		m_PtrPxSt	=	GrDrawFindPxStByFcc( m_Gdib.FccImg );
		//runtime
		m_PtrRtlPlt	=	(__u32*)GrGdibGetPalletPtr( &m_Gdib );
		m_RtlPltCnt	=	0;
		if ( NULL != m_PtrRtlPlt )
		{
			m_RtlPltCnt		=	1 << m_Gdib.BitCnt;
		}
		m_IsRtlPack		=	m_PtrPxSt->IsPacked;
		if ( m_Gdib.IsPlt )
		{
			m_IsRtlPack		=	FALSE;
			m_RtlPltMask	=	(__u8)((__u16)(1 << m_Gdib.BitCnt) -1 );
		}
}
//--------------------------------------------------------------------
Cls_GrGdib::~Cls_GrGdib()
{
		if ( NULL != m_PtrDib )
		{
			free( m_PtrDib );
		}

		if ( NULL != m_PtrPlt )
		{
			free( m_PtrPlt );
		}
}
//--------------------------------------------------------------------
__u32		Cls_GrGdib::GetResX( void )
{
		return	m_Gdib.ResX;
}
//--------------------------------------------------------------------
__u32		Cls_GrGdib::GetResY( void )
{
		return	m_Gdib.ResY;
}
//--------------------------------------------------------------------
__u32		Cls_GrGdib::GetFcc( void )
{
		return	m_Gdib.FccImg;
}
//--------------------------------------------------------------------
Ptr_GrGdib	Cls_GrGdib::GetGdibPtr( void )
{
		return	&m_Gdib;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrGdib::SetPixelVal( __s32 A_X, __s32 A_Y, __u32 A_Val )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( (m_Rect.left <= A_X) && (m_Rect.right > A_X) && (m_Rect.top <= A_Y) && (m_Rect.bottom > A_Y) )
		{
			ProcSetPixelOrd( A_X, A_Y, A_Val );
			//success
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrGdib::FindPltIdxByColor( __u32 A_Color, __u32* A_PtrRtIdx )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check pallete mode
		if ( NULL != m_PtrRtlPlt )
		{
			//find
			for ( Tv_WkIdx=0;Tv_WkIdx < m_RtlPltCnt;Tv_WkIdx++ )
			{
				if ( A_Color == m_PtrRtlPlt[Tv_WkIdx] )
				{
					//found
					*A_PtrRtIdx		=	Tv_WkIdx;
					Tv_Result			=	TRUE;
					break;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		Cls_GrGdib::GetColorByPltIdx(__u32 A_PltIdx, __u32* A_PtrRtColor)
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check pallete range
		if ( NULL != m_PtrRtlPlt )
		{
			if ( A_PltIdx < m_RtlPltCnt )
			{
				//ok
				*A_PtrRtColor		=	m_PtrRtlPlt[A_PltIdx];
				Tv_Result				=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrGdib::GetPixelVal( __s32 A_X, __s32 A_Y, __u32* A_PtrRtVal )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( (m_Rect.left <= A_X) && (m_Rect.right > A_X) && (m_Rect.top <= A_Y) && (m_Rect.bottom > A_Y) )
		{
			*A_PtrRtVal	=	ProcGetPixelOrd( A_X, A_Y );
			//success
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrGdib::GetPixelColor( __s32 A_X, __s32 A_Y, __u32* A_PtrRtColor )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( (m_Rect.left <= A_X) && (m_Rect.right > A_X) && (m_Rect.top <= A_Y) && (m_Rect.bottom > A_Y) )
		{
			*A_PtrRtColor	=	ProcGetPixelOrd( A_X, A_Y );
			if ( m_Gdib.IsPlt )
			{
				*A_PtrRtColor	=	m_PtrRtlPlt[*A_PtrRtColor];
			}
			
			//success
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrGdib::IsPalleteUse( void )
{
		return	m_Gdib.IsPlt;
}
//--------------------------------------------------------------------
void		Cls_GrGdib::DrawLine( __s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_Val )
{
	// local -------------------
		St_GrDrawLineD2	Tv_LineCtrl;
	// code --------------------
		if ( GrDrawLineD2Init(	&Tv_LineCtrl, A_StX, A_StY, A_EdX, A_EdY, &m_Rect ) )
		{
			//draw first pixel
			SetPixelVal( Tv_LineCtrl.NowX, Tv_LineCtrl.NowY, A_Val );
			//draw next 
			while ( GrDrawLineD2CalNext( &Tv_LineCtrl ) )
			{
				if ( Tv_LineCtrl.IsPxShow )
				{
					SetPixelVal( Tv_LineCtrl.NowX, Tv_LineCtrl.NowY, A_Val );
				}
			}
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdib::DrawRect( Ptr_GrRect A_PtrRect, __u32 A_PxVal )
{
		//top
		DrawLine( A_PtrRect->left, A_PtrRect->top, A_PtrRect->right - 1, A_PtrRect->top, A_PxVal );
		//left
		DrawLine( A_PtrRect->left, A_PtrRect->top + 1, A_PtrRect->left, A_PtrRect->bottom - 1, A_PxVal );
		//right
		DrawLine( A_PtrRect->right - 1, A_PtrRect->top + 1, A_PtrRect->right - 1, A_PtrRect->bottom - 1, A_PxVal );
		//bottom
		DrawLine( A_PtrRect->left, A_PtrRect->bottom - 1, A_PtrRect->right - 2, A_PtrRect->bottom - 1, A_PxVal );
}
//--------------------------------------------------------------------
void		Cls_GrGdib::DrawWaku( Ptr_GrRect A_PtrRect, __u32 A_PxValTopLeft, __u32 A_PxValBottomRight )
{
		//top
		DrawLine( A_PtrRect->left, A_PtrRect->top, A_PtrRect->right - 2, A_PtrRect->top, A_PxValTopLeft );
		//left
		DrawLine( A_PtrRect->left, A_PtrRect->top + 1, A_PtrRect->left, A_PtrRect->bottom - 2, A_PxValTopLeft );
		//right
		DrawLine( A_PtrRect->right - 1, A_PtrRect->top, A_PtrRect->right - 1, A_PtrRect->bottom - 1, A_PxValBottomRight );
		//bottom
		DrawLine( A_PtrRect->left, A_PtrRect->bottom - 1, A_PtrRect->right - 2, A_PtrRect->bottom - 1, A_PxValBottomRight );
}
//--------------------------------------------------------------------
void		Cls_GrGdib::DrawBoarder( __u8 A_Atb, __u32 A_Depth, __u32* A_PtrClrTbl, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__u32		Tv_ClrTopLeft;			//top left
		__u32		Tv_ClrBottomRight;	//bottom right
		St_GrRect	Tv_RcWk;
		__u32		Tv_WkIdx;
	// code --------------------
		if ( 0 != A_Depth )
		{
			//init
			if ( NULL == A_PtrRect )
			{
				Tv_RcWk.left		=	0;
				Tv_RcWk.top			=	0;
				Tv_RcWk.right		=	(__s32)m_Gdib.ResX;
				Tv_RcWk.bottom	=	(__s32)m_Gdib.ResY;
			}
			else
			{
				Tv_RcWk		=	*A_PtrRect;
			}
			
			//check type
			if ( 0 != (E_GrGdibBrdAtbFlat & A_Atb) )
			{
				//flat
				//get color
				Tv_ClrTopLeft	=	A_PtrClrTbl[E_GrGdibClrIdxBrdLight];
				if ( 0 != (E_GrGdibBrdAtbInner & A_Atb) )
				{
					Tv_ClrTopLeft	=	A_PtrClrTbl[E_GrGdibClrIdxBrdDark];
				}
				//get rect
				for ( Tv_WkIdx=0;Tv_WkIdx < A_Depth;Tv_WkIdx++ )
				{
					DrawRect( &Tv_RcWk, Tv_ClrTopLeft );
					//next
					Tv_RcWk.left ++;
					Tv_RcWk.top	++;
					Tv_RcWk.right	--;
					Tv_RcWk.bottom --;
				}
			}
			else
			{
				//normal
				//get color
				if ( 0 != (E_GrGdibBrdAtbInner & A_Atb) )
				{
					Tv_ClrTopLeft			=	A_PtrClrTbl[E_GrGdibClrIdxBrdDark];
					Tv_ClrBottomRight	=	A_PtrClrTbl[E_GrGdibClrIdxBrdLight];
				}
				else
				{
					Tv_ClrTopLeft			=	A_PtrClrTbl[E_GrGdibClrIdxBrdLight];
					Tv_ClrBottomRight	=	A_PtrClrTbl[E_GrGdibClrIdxBrdDark];
				}
				//draw edge
				DrawWaku( &Tv_RcWk, Tv_ClrTopLeft, Tv_ClrBottomRight );
				//check more draw able
				if ( 1 < A_Depth )
				{
					//get color
					if ( 0 != (E_GrGdibBrdAtbInner & A_Atb) )
					{
						Tv_ClrTopLeft			=	A_PtrClrTbl[E_GrGdibClrIdxBrdMidDark];
						Tv_ClrBottomRight	=	A_PtrClrTbl[E_GrGdibClrIdxBrdMidLight];
					}
					else
					{
						Tv_ClrTopLeft			=	A_PtrClrTbl[E_GrGdibClrIdxBrdMidLight];
						Tv_ClrBottomRight	=	A_PtrClrTbl[E_GrGdibClrIdxBrdMidDark];
					}
					//get rect
					Tv_RcWk.left ++;
					Tv_RcWk.top ++;
					Tv_RcWk.right --;
					Tv_RcWk.bottom	--;
					for ( Tv_WkIdx=0;Tv_WkIdx < A_Depth - 1;Tv_WkIdx++ )
					{
						DrawWaku( &Tv_RcWk, Tv_ClrTopLeft, Tv_ClrBottomRight );
						//next
						Tv_RcWk.left ++;
						Tv_RcWk.top	++;
						Tv_RcWk.right	--;
						Tv_RcWk.bottom --;
					}
				}
			}

		}

}
//--------------------------------------------------------------------
void		Cls_GrGdib::FillRect( Ptr_GrRect A_PtrRect, __u32 A_PxVal )
{
	// local -------------------
		St_GrRect		Tv_Rect;	
	// code --------------------
		//check all
		if ( NULL == A_PtrRect )
		{
				ProcFillRectOrd( &m_Rect, A_PxVal );
		}
		else
		{
			//check common rect
			if ( GrDrawCnGetComRect( &Tv_Rect, &m_Rect, A_PtrRect ) )
			{
					ProcFillRectOrd( &Tv_Rect, A_PxVal );
			}
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdib::BltGdib( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		St_GrRect		Tv_RectSr;	
		__s32					Tv_X;
		__s32					Tv_Y;
	// code --------------------
		//init
		Tv_X		=	A_X;
		Tv_Y		=	A_Y;
		Tv_RectSr.left		=	0;
		Tv_RectSr.top			=	0;
		Tv_RectSr.right		=	A_PtrGdibSr->ResX;
		Tv_RectSr.bottom	=	A_PtrGdibSr->ResY;
		if ( NULL != A_PtrRectSr )
		{
			Tv_RectSr		=	*A_PtrRectSr;
		}

		//check match fcc
		if ( A_PtrGdibSr->FccImg == m_Gdib.FccImg )
		{
			//patch source rect
			if ( GrDrawPatchPaintBlt( &m_Rect, &Tv_X, &Tv_Y, &Tv_RectSr ) )
			{
				//check blt type
				ProcBltOrd( Tv_X, Tv_Y, A_PtrGdibSr, &Tv_RectSr );
			}
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdib::NakedBlt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
		//check match fcc
		if ( A_PtrGdibSr->FccImg == m_Gdib.FccImg )
		{
			//check blt type
			ProcBltOrd( A_X, A_Y, A_PtrGdibSr, A_PtrRectSr );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdib::SpriteModeSet( BOOL8 A_IsSprite )
{
		m_IsSprite	=	A_IsSprite;
}
//--------------------------------------------------------------------
void		Cls_GrGdib::StretchGdib( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr )
{
	// local -------------------
		St_GrRect		Tv_RcCrntTg;
		St_GrRect		Tv_RcSrMask;
		St_GrRect		Tv_RcTgMask;
	// code --------------------
		if ( A_PtrGdibSr->FccImg == m_Gdib.FccImg )
		{
			Tv_RcSrMask.left		=	0;
			Tv_RcSrMask.top			=	0;
			Tv_RcSrMask.right		=	A_PtrGdibSr->ResX;
			Tv_RcSrMask.bottom	=	A_PtrGdibSr->ResY;

			Tv_RcTgMask					=	m_Rect;

			if ( GrDrawPatchStretchPara( &Tv_RcTgMask, A_PtrRcTg, &Tv_RcSrMask, A_PtrRcSr, &Tv_RcCrntTg ) )
			{
				ProcSthOrd( A_PtrRcTg, A_PtrGdibSr, A_PtrRcSr, &Tv_RcCrntTg );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::NakedStretch( Ptr_GrRect A_PtrRcTg, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg  )
{
		if ( A_PtrGdibSr->FccImg == m_Gdib.FccImg )
		{
			ProcSthOrd( A_PtrRcTg, A_PtrGdibSr, A_PtrRcSr, A_PtrRcCrntTg );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrGdib::AlphaRemove( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrRect	Tv_PtrRc;
		St_GrRect		Tv_RcCom;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check pallete mode
		if ( m_Gdib.IsPlt )
		{
			if ( NULL == A_PtrRect )
			{
				ProcAlphaPltRemove();
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			//check rect
			Tv_PtrRc		=	A_PtrRect;
			if ( NULL == Tv_PtrRc )
			{
				Tv_PtrRc		=	&m_Rect;
				Tv_Result		=	TRUE;
			}
			else
			{
				if ( GrDrawCnGetComRect( &Tv_RcCom, &m_Rect, A_PtrRect ) )
				{
					Tv_PtrRc		=	&Tv_RcCom;
					Tv_Result		=	TRUE;
				}
			}
			//check able
			if ( Tv_Result )
			{
				ProcAlphaRemove( Tv_PtrRc );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrGdib::AlphaSet( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrRect	Tv_PtrRc;
		St_GrRect		Tv_RcCom;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check pallete mode
		if ( m_Gdib.IsPlt )
		{
			if (NULL == A_PtrRect)
			{
				ProcAlphaPltSet();
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			//check rect
			Tv_PtrRc		=	A_PtrRect;
			if ( NULL == Tv_PtrRc )
			{
				Tv_PtrRc		=	&m_Rect;
				Tv_Result		=	TRUE;
			}
			else
			{
				if ( GrDrawCnGetComRect( &Tv_RcCom, &m_Rect, A_PtrRect ) )
				{
					Tv_PtrRc		=	&Tv_RcCom;
					Tv_Result		=	TRUE;
				}
			}
			//check able
			if ( Tv_Result )
			{
				ProcAlphaSet( Tv_PtrRc );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcAlphaSet( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_WkX;
		__s32		Tv_WkY;
		__u32	Tv_Clr;
	// code --------------------
		for ( Tv_WkY=A_PtrRect->top;Tv_WkY < A_PtrRect->bottom;Tv_WkY++ )
		{
			for ( Tv_WkX= A_PtrRect->left;Tv_WkX < A_PtrRect->right;Tv_WkX++ )
			{
				GetPixelVal( Tv_WkX, Tv_WkY, &Tv_Clr );
				GrDrawColorAlpahSet( m_Gdib.FccImg, &Tv_Clr, TRUE );
				SetPixelVal( Tv_WkX, Tv_WkY, Tv_Clr );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcAlphaPltSet( void )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=1;Tv_WkIdx < m_RtlPltCnt;Tv_WkIdx++ )
		{
			GrDrawColorAlpahSet( m_Gdib.FccImg, &m_PtrRtlPlt[Tv_WkIdx], TRUE );
		}
}
//--------------------------------------------------------------------
BOOL8		Cls_GrGdib::ChgPalletForamt( __u32 A_Fcc )
{
	// local -------------------
	BOOL8		Tv_Result;
	__u32*	Tv_PtrPal;
	__u32		Tv_WkIdx;
	__u32		Tv_ClrCvt;
	// code --------------------
	//init
	Tv_Result		=	FALSE;

	if ( m_Gdib.IsPlt )
	{
		//change pallet formate
		Tv_PtrPal			=	(__u32*)m_Gdib.PtrExtPlt;
		for ( Tv_WkIdx=0;Tv_WkIdx < m_RtlPltCnt;Tv_WkIdx++ )
		{
			Tv_ClrCvt	=	GrDrawCvtColorByFcc( m_Gdib.FccImg, *Tv_PtrPal, A_Fcc );
			*Tv_PtrPal	=	Tv_ClrCvt;
			//next
			Tv_PtrPal ++;
		}
		//success
		m_Gdib.FccImg		=	A_Fcc;
		Tv_Result		=	TRUE;
	}

	return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrGdib::CvtTo( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		if ( NULL == A_PtrRect )
		{
			Tv_Rc.left		=	0;
			Tv_Rc.top			=	0;
			Tv_Rc.right		=	m_Gdib.ResX;
			Tv_Rc.bottom	=	m_Gdib.ResY;
			ProcCvtOrd( A_ObjGdib, &Tv_Rc );
		}
		else
		{
			ProcCvtOrd( A_ObjGdib, A_PtrRect );
		}
		
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrGdib::SaveToFile(char* A_StrFn)
{
	// local -------------------
		BOOL8		Tv_Result;
		Cls_GrFileCtrl*	Tv_ObjFl;
		St_GrGdib	Tv_GdibHd;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//exist file
		if ( GrFileIsExist( A_StrFn ) )
		{
			GrFileDelete( A_StrFn );
		}

		//create file
		Tv_ObjFl	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, TRUE, TRUE );
		if ( Tv_ObjFl->IsOpened() )
		{
			//write header
			Tv_GdibHd	=	m_Gdib;
			Tv_GdibHd.PtrExtDib		=	NULL;
			Tv_GdibHd.PtrExtPlt		=	NULL;
			Tv_GdibHd.Size				=	sizeof(Tv_GdibHd) +	(m_RtlPltCnt * 4) + m_DibSize;
			if ( sizeof(Tv_GdibHd) == Tv_ObjFl->Write( &Tv_GdibHd, sizeof(Tv_GdibHd) ) )
			{
				//write pallete
				if ( m_Gdib.IsPlt )
				{
					if ( (4 * m_RtlPltCnt) == Tv_ObjFl->Write( m_Gdib.PtrExtPlt, (4 * m_RtlPltCnt) ) )
					{
						Tv_Result	=	TRUE;
					}
				}
				else
				{
					Tv_Result	=	TRUE;
				}
				//write contents
				if ( Tv_Result )
				{
					if ( m_DibSize != Tv_ObjFl->Write( m_Gdib.PtrExtDib, m_DibSize ) )
					{
						Tv_Result		=	FALSE;
					}
				}
			}
		}
		delete	Tv_ObjFl;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcSetPixelOrd( __s32 A_X, __s32 A_Y, __u32 A_PxVal )
{
		if ( m_Gdib.IsPlt )
		{
			ProcSetPixelPlt( A_X, A_Y, A_PxVal );
		}
		else
		{
			ProcSetPixelNpt( A_X, A_Y, A_PxVal );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcSetPixelNpt( __s32 A_X, __s32 A_Y, __u32 A_PxVal )
{
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcSetPixelPlt( __s32 A_X, __s32 A_Y, __u32 A_PxVal )
{
	// local -------------------
		__u8*		Tv_PtrPx;
		__u32		Tv_Subp;
	// code --------------------
		//calculate address
		Tv_PtrPx	=	(__u8*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( ( A_X * m_Gdib.BitCnt ) >> 3 ) );
		Tv_Subp		=	0x07 & (A_X * m_Gdib.BitCnt);

		*Tv_PtrPx	=	(*Tv_PtrPx & ~(m_RtlPltMask << Tv_Subp) );
		*Tv_PtrPx	=	*Tv_PtrPx | ((__u8)A_PxVal << Tv_Subp);
}
//--------------------------------------------------------------------
__u32		Cls_GrGdib::ProcGetPixelOrd( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result		=	0;

		if ( m_Gdib.IsPlt )
		{
			Tv_Result	=	ProcGetPixelPlt( A_X, A_Y );
		}
		else
		{
			Tv_Result	=	ProcGetPixelNpt( A_X, A_Y );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrGdib::ProcGetPixelNpt( __s32 A_X, __s32 A_Y )
{
		return	0;
}
//--------------------------------------------------------------------
__u32	Cls_GrGdib::ProcGetPixelPlt( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		__u32		Tv_Result;
		__u8*		Tv_PtrPx;
		__u32		Tv_Subp;
	// code --------------------
		//calculate address
		Tv_PtrPx	=	(__u8*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( ( A_X * m_Gdib.BitCnt ) >> 3 ) );
		Tv_Subp		=	0x07 & (A_X * m_Gdib.BitCnt);

		Tv_Result	=	(__u32)( (*Tv_PtrPx >> Tv_Subp) & m_RtlPltMask );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcFillRectOrd( Ptr_GrRect A_PtrRect, __u32 A_PxVal )
{
		if ( m_Gdib.IsPlt )
		{
			ProcFillRectPlt( A_PtrRect, A_PxVal );		
		}
		else
		{
			ProcFillRectNpt( A_PtrRect, A_PxVal );		
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcFillRectNpt( Ptr_GrRect A_PtrRect, __u32 A_PxVal )
{
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcFillRectPlt( Ptr_GrRect A_PtrRect, __u32 A_PxVal )
{
	// local -------------------
		void*		Tv_PtrY;
		__u8*		Tv_PtrPx;
		__u32		Tv_Subp;
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		__u8		Tv_PxFull;
		__u32		Tv_PxCntOnByte;
		__u32		Tv_BitHd;
		__u32		Tv_BitTail;
		__u32		Tv_BitMid;
		__u32		Tv_ByteMid;
		__u8		Tv_HdMask;
		__u8		Tv_TlMask;
	// code --------------------
		//make pixel full
		Tv_PxCntOnByte	=	8 / m_Gdib.BitCnt;
		Tv_PxFull	=	0;
		Tv_Subp		=	0;
		for ( Tv_WkX=0;Tv_WkX < Tv_PxCntOnByte;Tv_WkX++ )
		{
			Tv_PxFull	=	Tv_PxFull | ( ( (__u8)A_PxVal & m_RtlPltMask ) << Tv_Subp );
			//next
			Tv_Subp		=	Tv_Subp + m_Gdib.BitCnt;
		}
		Tv_BitMid		=	(A_PtrRect->right - A_PtrRect->left) * m_Gdib.BitCnt;
		//get bit head
		Tv_BitHd		=	(A_PtrRect->left * m_Gdib.BitCnt) & 0x07;
		Tv_HdMask		=	~( (__u8)( (__u16)(1 << Tv_BitHd) -1) );
		Tv_BitMid		=	Tv_BitMid - ((8 - Tv_BitHd) & 0x07);
		//get bit tail
		Tv_BitTail	=	Tv_BitMid & 0x07;
		Tv_TlMask		=	(__u8)( (__u16)(1 << Tv_BitTail) -1 );
		//get mid byte
		Tv_ByteMid	=	Tv_BitMid >> 3;

		//calculate address
		Tv_PtrY		=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_PtrRect->top ) +
													( ( A_PtrRect->left * m_Gdib.BitCnt ) >> 3 ) );
		//work
		for ( Tv_WkY = (__u32)A_PtrRect->top;Tv_WkY < (__u32)A_PtrRect->bottom;Tv_WkY++ )
		{
			//set pixel address
			Tv_PtrPx	=	(__u8*)Tv_PtrY;

			//draw head
			if ( 0 != Tv_BitHd )
			{
				*Tv_PtrPx	=	*Tv_PtrPx & (~Tv_HdMask);
				*Tv_PtrPx	=	*Tv_PtrPx | (Tv_PxFull & Tv_HdMask); 
				Tv_PtrPx ++;
			}

			//middle
			if ( 0 != Tv_ByteMid )
			{
				Tv_PtrPx	=	(__u8*)GrDumyFillB( (void*)Tv_PtrPx, Tv_ByteMid, Tv_PxFull );
			}

			//draw tail
			if ( 0 != Tv_BitTail )
			{
				*Tv_PtrPx	=	( *Tv_PtrPx & ~Tv_TlMask );
				*Tv_PtrPx	=	*Tv_PtrPx | (Tv_PxFull & Tv_TlMask); 
			}
			
			//next
			Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcBltOrd( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
	// code --------------------
		if ( m_Gdib.IsPlt && A_PtrGdibSr->IsPlt )
		{
			//src , dest pallete
			if ( A_PtrGdibSr->BitCnt == m_Gdib.BitCnt )
			{
				ProcBltPlt2Plt( A_X, A_Y, A_PtrGdibSr, A_PtrRectSr );
			}
		}
		else if ( A_PtrGdibSr->IsPlt )
		{
			//source pallete only
			ProcBltPlt2Npt( A_X, A_Y, A_PtrGdibSr, A_PtrRectSr );
		}
		else
		{
			//non pallete mode
			ProcBltNpt2Npt( A_X, A_Y, A_PtrGdibSr, A_PtrRectSr );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdib::ProcBltNpt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
		if ( m_IsSprite )
		{
			ProcBltSrkNpt2Npt( A_X, A_Y, A_PtrGdibSr, A_PtrRectSr );
		}
		else
		{
			ProcBltNrmNpt2Npt( A_X, A_Y, A_PtrGdibSr, A_PtrRectSr );
		}
 }
//--------------------------------------------------------------------
void		Cls_GrGdib::ProcBltPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
		if ( m_IsSprite )
		{
			ProcBltSrkPlt2Npt( A_X, A_Y, A_PtrGdibSr, A_PtrRectSr );
		}
		else
		{
			ProcBltNrmPlt2Npt( A_X, A_Y, A_PtrGdibSr, A_PtrRectSr );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdib::ProcBltPlt2Plt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
		if ( m_IsSprite )
		{
			ProcBltSrkPlt2Plt( A_X, A_Y, A_PtrGdibSr, A_PtrRectSr );
		}
		else
		{
			ProcBltNrmPlt2Plt( A_X, A_Y, A_PtrGdibSr, A_PtrRectSr );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdib::ProcBltNrmNpt2Npt( __s32 A_X, __s32 A_Y,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
}
//--------------------------------------------------------------------
void		Cls_GrGdib::ProcBltSrkNpt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
}
//--------------------------------------------------------------------
void		Cls_GrGdib::ProcBltNrmPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
}
//--------------------------------------------------------------------
void		Cls_GrGdib::ProcBltSrkPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcBltNrmPlt2Plt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		__u32		Tv_CntHz;
		__u32		Tv_CntVt;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SrHzBitPos;
		__u32		Tv_TgHzBitPos;

		__u8		Tv_PltIdx;

		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//acc count
		Tv_CntHz		=	(__u32)(A_PtrRectSr->right - A_PtrRectSr->left);
		Tv_CntVt		=	(__u32)A_PtrRectSr->bottom;
		if ( A_PtrGdibSr->ResY < Tv_CntVt )
		{
			Tv_CntVt	=	A_PtrGdibSr->ResY;
		}
		if ( m_Gdib.ResY < Tv_CntVt )
		{
			Tv_CntVt	=	m_Gdib.ResY;
		}
		
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
													( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( ( A_X * m_Gdib.BitCnt ) >> 3 ) );
		Tv_SrHzBitPos	=	0x07 & (A_PtrRectSr->left * A_PtrGdibSr->BitCnt);
		Tv_TgHzBitPos	=	0x07 & (A_X * m_Gdib.BitCnt);
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < Tv_CntVt;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			Tv_SubpSr			=	Tv_SrHzBitPos;
			Tv_SubpTg			=	Tv_TgHzBitPos;
			for ( Tv_WkX=0;Tv_WkX < Tv_CntHz;Tv_WkX++ )
			{
				//write pixel
				Tv_PltIdx		=	((*Tv_PtrSrP >> Tv_SubpSr) & m_RtlPltMask);
				*Tv_PtrTgP	=	(*Tv_PtrTgP & ~(m_RtlPltMask << Tv_SubpTg) );
				*Tv_PtrTgP	=	*Tv_PtrTgP | (Tv_PltIdx << Tv_SubpTg);

				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrP	=	Tv_PtrSrP + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_SubpTg	=	Tv_SubpTg + m_Gdib.BitCnt;
				Tv_PtrTgP	=	Tv_PtrTgP + ( (Tv_SubpTg >> 3) & 1 );
				Tv_SubpTg	=	Tv_SubpTg & 0x07;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdib::ProcBltSrkPlt2Plt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		__u32		Tv_CntHz;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SrHzBitPos;
		__u32		Tv_TgHzBitPos;

		__u8		Tv_PltIdx;

		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//acc count
		Tv_CntHz		=	A_PtrRectSr->right - A_PtrRectSr->left;
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
													( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( ( A_X * m_Gdib.BitCnt ) >> 3 ) );
		Tv_SrHzBitPos	=	0x07 & (A_PtrRectSr->left * A_PtrGdibSr->BitCnt);
		Tv_TgHzBitPos	=	0x07 & (A_X * m_Gdib.BitCnt);
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			Tv_SubpSr			=	Tv_SrHzBitPos;
			Tv_SubpTg			=	Tv_TgHzBitPos;
			for ( Tv_WkX=0;Tv_WkX < Tv_CntHz;Tv_WkX++ )
			{
				//write pixel
				Tv_PltIdx		=	((*Tv_PtrSrP >> Tv_SubpSr) & m_RtlPltMask);
				//check source key
				if ( 0 != Tv_PltIdx )
				{
					*Tv_PtrTgP	=	(*Tv_PtrTgP & ~(m_RtlPltMask << Tv_SubpTg) );
					*Tv_PtrTgP	=	*Tv_PtrTgP | (Tv_PltIdx << Tv_SubpTg);
				}

				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrP	=	Tv_PtrSrP + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_SubpTg	=	Tv_SubpTg + m_Gdib.BitCnt;
				Tv_PtrTgP	=	Tv_PtrTgP + ( (Tv_SubpTg >> 3) & 1 );
				Tv_SubpTg	=	Tv_SubpTg & 0x07;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcSthOrd( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg )
{
		if ( m_Gdib.IsPlt && A_PtrGdibSr->IsPlt )
		{
			//src , dest pallete
			if ( A_PtrGdibSr->BitCnt == m_Gdib.BitCnt )
			{
				ProcSthPlt2Plt( A_PtrRcTg, A_PtrGdibSr, A_PtrRcSr, A_PtrRcCrntTg );
			}
		}
		else if ( A_PtrGdibSr->IsPlt )
		{
			//source pallete only
			ProcSthPlt2Npt( A_PtrRcTg, A_PtrGdibSr, A_PtrRcSr, A_PtrRcCrntTg );
		}
		else
		{
			//non pallete mode
			ProcSthNpt2Npt( A_PtrRcTg, A_PtrGdibSr, A_PtrRcSr, A_PtrRcCrntTg );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcSthNpt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg )
{
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcSthPlt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg )
{
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcSthPlt2Plt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg )
{
	// local -------------------
		__u8		Tv_PxVal;
		__s32			Tv_SrRtX;
		__s32			Tv_SrRtY;
		__s32			Tv_TgRtX;
		__s32			Tv_TgRtY;
		__s32			Tv_WkX;
		__s32			Tv_WkY;
		__s32			Tv_SrX;
		__s32			Tv_SrY;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;
		__u8*		Tv_PtrSrPx;
		__u8*		Tv_PtrTgPx;
		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;
		void*		Tv_PtrDibSr;
		__u32		Tv_SubpBaseTg;

	// code --------------------
		//calculate current target
		Tv_SrRtX		=	A_PtrRcSr->right - A_PtrRcSr->left;
		Tv_SrRtY		=	A_PtrRcSr->bottom - A_PtrRcSr->top;
		Tv_TgRtX		=	A_PtrRcTg->right - A_PtrRcTg->left;
		Tv_TgRtY		=	A_PtrRcTg->bottom - A_PtrRcTg->top;

		Tv_PtrDibSr	=	GrGdibGetDibPtr( A_PtrGdibSr );
		//address target
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_PtrRcCrntTg->top ) +
													( ( A_PtrRcCrntTg->left * m_Gdib.BitCnt ) >> 3 ) );
		Tv_SubpBaseTg	=	0x07 & (A_PtrRcCrntTg->left * m_Gdib.BitCnt);

		for ( Tv_WkY = A_PtrRcCrntTg->top;Tv_WkY < A_PtrRcCrntTg->bottom;Tv_WkY ++ )
		{
			//get x positon
			Tv_SubpTg		=	Tv_SubpBaseTg;
			Tv_PtrTgPx	=	(__u8*)Tv_PtrTgY;
			//calculate source y
			Tv_SrY	=	A_PtrRcSr->top + ( ((Tv_WkY - A_PtrRcTg->top) * Tv_SrRtY ) / Tv_TgRtY );
			Tv_PtrSrY	=	(void*)(	(__u32)Tv_PtrDibSr + ( A_PtrGdibSr->Pitch * Tv_SrY ) );
			for ( Tv_WkX = A_PtrRcCrntTg->left;Tv_WkX < A_PtrRcCrntTg->right;Tv_WkX ++ )
			{
				//get source x position
				Tv_SrX	=	A_PtrRcSr->left + ( ((Tv_WkX - A_PtrRcTg->left) * Tv_SrRtX ) / Tv_TgRtX );
				//get address source
				Tv_PtrSrPx	=	(__u8*)( (__u32)Tv_PtrSrY + ( ( Tv_SrX * A_PtrGdibSr->BitCnt ) >> 3 ) );
				Tv_SubpSr		=	0x07 & (Tv_SrX * m_Gdib.BitCnt);
				//get pixel value
				Tv_PxVal		=	((*Tv_PtrSrPx >> Tv_SubpSr) & m_RtlPltMask);
				//write pixel				
				*Tv_PtrTgPx	=	(*Tv_PtrTgPx & ~(m_RtlPltMask << Tv_SubpTg) );
				*Tv_PtrTgPx	=	*Tv_PtrTgPx | (Tv_PxVal << Tv_SubpTg);
				//next
				Tv_SubpTg		=	Tv_SubpTg + m_Gdib.BitCnt;
				Tv_PtrTgPx	=	Tv_PtrTgPx + ( (Tv_SubpTg >> 3) & 1 );
				Tv_SubpTg		=	Tv_SubpTg & 0x07;
			}
			//next
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcAlphaRemove( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__s32		Tv_WkX;
		__s32		Tv_WkY;
		__u32	Tv_Clr;
	// code --------------------
		for ( Tv_WkY=A_PtrRect->top;Tv_WkY < A_PtrRect->bottom;Tv_WkY++ )
		{
			for ( Tv_WkX= A_PtrRect->left;Tv_WkX < A_PtrRect->right;Tv_WkX++ )
			{
				GetPixelVal( Tv_WkX, Tv_WkY, &Tv_Clr );
				GrDrawColorAlpahSet( m_Gdib.FccImg, &Tv_Clr, FALSE );
				SetPixelVal( Tv_WkX, Tv_WkY, Tv_Clr );
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrGdib::GetPixelBit( void )
{
		return	m_Gdib.BitCnt;
}
//--------------------------------------------------------------------
__u32	Cls_GrGdib::GetDibPitch( void )
{
		return	m_Gdib.Pitch;
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcCvtOrd( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect )
{
		if ( m_Gdib.IsPlt && A_ObjGdib->IsPalleteUse() )
		{
			//pallete to pallete
			ProcCvtPlt2Plt( A_ObjGdib, A_PtrRect );
		}
		else if ( m_Gdib.IsPlt )
		{
			//source pallete only
			ProcCvtPlt2Npt( A_ObjGdib, A_PtrRect );
		}
		else if ( A_ObjGdib->IsPalleteUse() )
		{
			//target pallete only
			ProcCvtNpt2Plt( A_ObjGdib, A_PtrRect );
		}
		else
		{
			//non pallete mode
			ProcCvtNpt2Npt( A_ObjGdib, A_PtrRect );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcCvtNpt2Npt( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__u32	Tv_WkX;
		__u32	Tv_WkY;
		__u32	Tv_PxVal;
		__u8	Tv_ClrEleSr[E_GrDrawClrElemCnt];
		__u8	Tv_ClrEleTg[E_GrDrawClrElemCnt];
		Ptr_GrDrawPxStInfo		Tv_PtrPxStTg;
		Ptr_GrDrawClrCvtPara	Tv_PtrCvtPara;
		__u32		Tv_TgResX;
		__u32		Tv_TgResY;
	// code --------------------
		//init
		Tv_PtrPxStTg	=	GrDrawFindPxStByFcc( A_ObjGdib->GetFcc() );
		if ( NULL != Tv_PtrPxStTg )
		{
			Tv_PtrCvtPara	=	GrDrawFindCvtParaByPxSt( m_PtrPxSt, Tv_PtrPxStTg );
			
			Tv_TgResX		=	A_ObjGdib->GetResX();
			Tv_TgResY		=	A_ObjGdib->GetResY();
			Tv_WkY	=	A_PtrRect->top;
			while ( ((__u32)A_PtrRect->bottom > Tv_WkY) && (Tv_TgResY > Tv_WkY) )
			{
				Tv_WkX	=	A_PtrRect->left;
				while ( ((__u32)A_PtrRect->right > Tv_WkX) && (Tv_TgResX > Tv_WkX) )
				{
					Tv_PxVal	=	ProcGetPixelOrd( Tv_WkX, Tv_WkY );
					GrDrawGetClrElemByPxst( Tv_PxVal, m_PtrPxSt, Tv_ClrEleSr );
					GrDrawCvtEleByCvtPara( Tv_PtrCvtPara, Tv_ClrEleSr, Tv_ClrEleTg );
					Tv_PxVal	=	GrDrawMakeClrValByByPxst( Tv_PtrPxStTg, Tv_ClrEleTg );
					A_ObjGdib->SetPixelVal( Tv_WkX, Tv_WkY, Tv_PxVal );
					//next
					Tv_WkX ++;
				}
				//next
				Tv_WkY ++;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcCvtPlt2Npt( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__u32	Tv_WkX;
		__u32	Tv_WkY;
		__u32	Tv_PltIdx;
		__u32	Tv_PxVal;
		__u8	Tv_ClrEleSr[E_GrDrawClrElemCnt];
		__u8	Tv_ClrEleTg[E_GrDrawClrElemCnt];
		Ptr_GrDrawPxStInfo		Tv_PtrPxStTg;
		Ptr_GrDrawClrCvtPara	Tv_PtrCvtPara;
		__u32		Tv_TgResX;
		__u32		Tv_TgResY;
	// code --------------------
		Tv_PtrPxStTg	=	GrDrawFindPxStByFcc( A_ObjGdib->GetFcc() );
		if ( NULL != Tv_PtrPxStTg )
		{
			Tv_PtrCvtPara	=	GrDrawFindCvtParaByPxSt( m_PtrPxSt, Tv_PtrPxStTg );

			Tv_TgResX		=	A_ObjGdib->GetResX();
			Tv_TgResY		=	A_ObjGdib->GetResY();
			Tv_WkY	=	A_PtrRect->top;
			while ( ((__u32)A_PtrRect->bottom > Tv_WkY) && (Tv_TgResY > Tv_WkY) )
			{
				Tv_WkX	=	A_PtrRect->left;
				while ( ((__u32)A_PtrRect->right > Tv_WkX) && (Tv_TgResX > Tv_WkX) )
				{
					Tv_PltIdx	=	ProcGetPixelOrd( Tv_WkX, Tv_WkY );
					Tv_PxVal	=	m_PtrRtlPlt[Tv_PltIdx];

					GrDrawGetClrElemByPxst( Tv_PxVal, m_PtrPxSt, Tv_ClrEleSr );
					GrDrawCvtEleByCvtPara( Tv_PtrCvtPara, Tv_ClrEleSr, Tv_ClrEleTg );
					Tv_PxVal	=	GrDrawMakeClrValByByPxst( Tv_PtrPxStTg, Tv_ClrEleTg );

					A_ObjGdib->SetPixelVal( Tv_WkX, Tv_WkY, Tv_PxVal );
					//next
					Tv_WkX ++;
				}
				//next
				Tv_WkY ++;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcCvtNpt2Plt( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__u32		Tv_WkX;
		__u32		Tv_WkY;
		__u32		Tv_PltIdx;
		__u32		Tv_PxVal;
		__u32		Tv_PltCnt;
		__u32*	Tv_PtrPltTg;
		__u32		Tv_MaxPltCnt;
		__u32		Tv_WkIdx;
		BOOL8		Tv_IsPltFind;
		__u8	Tv_ClrEleSr[E_GrDrawClrElemCnt];
		__u8	Tv_ClrEleTg[E_GrDrawClrElemCnt];
		Ptr_GrDrawPxStInfo		Tv_PtrPxStTg;
		Ptr_GrDrawClrCvtPara	Tv_PtrCvtPara;
		__u32		Tv_TgResX;
		__u32		Tv_TgResY;
	// code --------------------
		Tv_PltCnt			=	0;
		Tv_PtrPltTg		=	(__u32*)A_ObjGdib->GetPltPtr();
		Tv_MaxPltCnt	=	1 << A_ObjGdib->GetPixelBit();

		Tv_PtrPxStTg	=	GrDrawFindPxStByFcc( A_ObjGdib->GetFcc() );
		if ( NULL != Tv_PtrPxStTg )
		{
			Tv_PtrCvtPara	=	GrDrawFindCvtParaByPxSt( m_PtrPxSt, Tv_PtrPxStTg );
			Tv_TgResX		=	A_ObjGdib->GetResX();
			Tv_TgResY		=	A_ObjGdib->GetResY();
			Tv_WkY	=	A_PtrRect->top;
			while ( ((__u32)A_PtrRect->bottom > Tv_WkY) && (Tv_TgResY > Tv_WkY) )
			{
				Tv_WkX	=	A_PtrRect->left;
				while ( ((__u32)A_PtrRect->right > Tv_WkX) && (Tv_TgResX > Tv_WkX) )
				{
					Tv_PxVal	=	ProcGetPixelOrd( Tv_WkX, Tv_WkY );

					GrDrawGetClrElemByPxst( Tv_PxVal, m_PtrPxSt, Tv_ClrEleSr );
					GrDrawCvtEleByCvtPara( Tv_PtrCvtPara, Tv_ClrEleSr, Tv_ClrEleTg );
					Tv_PxVal	=	GrDrawMakeClrValByByPxst( Tv_PtrPxStTg, Tv_ClrEleTg );

					//add pallete
					Tv_PltIdx		=	0;
					//find match pallete
					Tv_IsPltFind	=	FALSE;
					if ( 0 != Tv_PltCnt )
					{
						for ( Tv_WkIdx = 0;Tv_WkIdx < Tv_PltCnt;Tv_WkIdx++ )
						{
							if ( Tv_PxVal == Tv_PtrPltTg[Tv_WkIdx] )
							{
								//found
								Tv_PltIdx			=	Tv_WkIdx;
								Tv_IsPltFind	=	TRUE;
								break;
							}
						}
					}
					//add pallete
					if ( !Tv_IsPltFind && (Tv_PltCnt < Tv_MaxPltCnt ) )
					{
						Tv_PtrPltTg[Tv_PltCnt]	=	Tv_PxVal;
						Tv_PltCnt++;
					}
					//write value
					A_ObjGdib->SetPixelVal( Tv_WkX, Tv_WkY, Tv_PltIdx );
					//next
					Tv_WkX ++;
				}
				//next
				Tv_WkY ++;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcCvtPlt2Plt( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__u32		Tv_WkX;
		__u32		Tv_WkY;
		__u32		Tv_PltIdx;
		__u32		Tv_PxVal;
		__u32*	Tv_PtrPltTg;
		__u32		Tv_MaxPltCnt;
		__u32		Tv_WkIdx;
		__u32		Tv_ValMask;
		__u8	Tv_ClrEleSr[E_GrDrawClrElemCnt];
		__u8	Tv_ClrEleTg[E_GrDrawClrElemCnt];
		Ptr_GrDrawPxStInfo		Tv_PtrPxStTg;
		Ptr_GrDrawClrCvtPara	Tv_PtrCvtPara;
		__u32		Tv_TgResX;
		__u32		Tv_TgResY;
	// code --------------------
		Tv_PtrPltTg		=	(__u32*)A_ObjGdib->GetPltPtr();
		Tv_MaxPltCnt	=	1 << A_ObjGdib->GetPixelBit();
		if ( m_RtlPltCnt < Tv_MaxPltCnt )
		{
			Tv_MaxPltCnt	=	m_RtlPltCnt;
		}
		Tv_ValMask	=	Tv_MaxPltCnt - 1;

		Tv_PtrPxStTg	=	GrDrawFindPxStByFcc( A_ObjGdib->GetFcc() );
		if ( NULL != Tv_PtrPxStTg )
		{
			Tv_PtrCvtPara	=	GrDrawFindCvtParaByPxSt( m_PtrPxSt, Tv_PtrPxStTg );

			//converting pallete
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_MaxPltCnt;Tv_WkIdx++ )
			{
				GrDrawGetClrElemByPxst( m_PtrRtlPlt[Tv_WkIdx], m_PtrPxSt, Tv_ClrEleSr );
				GrDrawCvtEleByCvtPara( Tv_PtrCvtPara, Tv_ClrEleSr, Tv_ClrEleTg );
				Tv_PxVal	=	GrDrawMakeClrValByByPxst( Tv_PtrPxStTg, Tv_ClrEleTg );

				Tv_PtrPltTg[Tv_WkIdx]	=	Tv_PxVal;
			}
			
			//copy value
			Tv_TgResX		=	A_ObjGdib->GetResX();
			Tv_TgResY		=	A_ObjGdib->GetResY();
			Tv_WkY		=	A_PtrRect->top;
			while ( ((__u32)A_PtrRect->bottom > Tv_WkY) && (Tv_TgResY > Tv_WkY) )
			{
				Tv_WkX		=	A_PtrRect->left;
				while ( ((__u32)A_PtrRect->right > Tv_WkX) && (Tv_TgResX > Tv_WkX) )
				{
					Tv_PltIdx	=	ProcGetPixelOrd( Tv_WkX, Tv_WkY );
					Tv_PltIdx	=	Tv_PltIdx & Tv_ValMask;
					A_ObjGdib->SetPixelVal( Tv_WkX, Tv_WkY, Tv_PltIdx );
					//next
					Tv_WkX ++;
				}
				//next
				Tv_WkY ++;
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrGdib::GetDibSize( void )
{
		return	m_DibSize;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrGdib::SaveToBmp(char* A_StrFn)
{
	// local -------------------
		BOOL8		Tv_Result;
		BOOL8		Tv_IsAble;
		Cls_GrFileCtrl*	Tv_ObjFl;
		St_GrBmpFlHead	Tv_BmpHd;
		void*		Tv_PtrExtDib;
		__u32		Tv_BmpPitch;
		__u32		Tv_BmpDibSize;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check bmpable
		Tv_IsAble		=	FALSE;
		if ( m_Gdib.IsPlt )
		{
			if (	(E_GrFccRGB == m_Gdib.FccImg) || (E_GrFccRGB888 == m_Gdib.FccImg) ||  
						(E_GrFccARGB8888 == m_Gdib.FccImg)  )
			{
				if ( (1 == m_Gdib.BitCnt) || (4 == m_Gdib.BitCnt) || (8 == m_Gdib.BitCnt) )
				{
					Tv_IsAble		=	TRUE;
				}
			}
		}
		else
		{
			if (	(E_GrFccRGB == m_Gdib.FccImg) || (E_GrFccRGB555 == m_Gdib.FccImg) ||  (E_GrFccARGB1555 == m_Gdib.FccImg) || 
						(E_GrFccRGB888 == m_Gdib.FccImg) || (E_GrFccARGB8888 == m_Gdib.FccImg) )
			{
				Tv_IsAble	=	TRUE;
			}
		}
		//bmp make able type
		if ( Tv_IsAble )
		{
			//exist file
			if ( GrFileIsExist( A_StrFn ) )
			{
				GrFileDelete( A_StrFn );
			}

			//create file
			Tv_ObjFl	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, TRUE, TRUE );
			if ( Tv_ObjFl->IsOpened() )
			{
				//init
				Tv_BmpPitch		=	GrDrawCalcPitchByBitCnt( m_Gdib.BitCnt, m_Gdib.ResX );
				Tv_BmpDibSize	=	Tv_BmpPitch * m_Gdib.ResY;
				//make header
				GrBmpSetHeader( &Tv_BmpHd, m_Gdib.ResX, m_Gdib.ResY, m_Gdib.BitCnt );		
				//write header
				if ( sizeof(Tv_BmpHd) == Tv_ObjFl->Write( &Tv_BmpHd, sizeof(Tv_BmpHd) ) )
				{
					//write pallete
					if ( m_Gdib.IsPlt )
					{
						if ( (4 * m_RtlPltCnt) == Tv_ObjFl->Write( m_Gdib.PtrExtPlt, (4 * m_RtlPltCnt) ) )
						{
							Tv_Result	=	TRUE;
						}
					}
					else
					{
						Tv_Result	=	TRUE;
					}
					//write contents
					if ( Tv_Result )
					{
						//make contents
						Tv_PtrExtDib	=	malloc( Tv_BmpDibSize );
						if ( NULL != Tv_PtrExtDib )
						{
							//make dib
							GrDrawDibCpyRsvVt( m_Gdib.PtrExtDib, m_Gdib.Pitch, Tv_PtrExtDib, Tv_BmpPitch, m_Gdib.ResY );
							if ( 1 == m_Gdib.BitCnt )
							{
								GrDumySwapByte1Bit( Tv_PtrExtDib, Tv_BmpDibSize );
							}
							else if ( 4 == m_Gdib.BitCnt )
							{
								GrDumySwapByte4Bit( Tv_PtrExtDib, Tv_BmpDibSize );
							}

							if ( m_DibSize != Tv_ObjFl->Write( Tv_PtrExtDib, Tv_BmpDibSize ) )
							{
								Tv_Result		=	FALSE;
							}
							free( Tv_PtrExtDib );
						}
					}
				}
			}
			delete	Tv_ObjFl;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrGdib::GetDibPtr( void )
{
		return	m_Gdib.PtrExtDib;
}
//--------------------------------------------------------------------
void*	Cls_GrGdib::GetPltPtr( void )
{
		return	m_PtrRtlPlt;
}
//--------------------------------------------------------------------
__u32	Cls_GrGdib::PalleteGet( __u32 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	0;
		if ( m_RtlPltCnt > A_Idx )
		{
			Tv_Result	=	m_PtrRtlPlt[A_Idx];
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrGdib::PalleteSet( __u32 A_Idx, __u32 A_Color )
{
		if ( m_RtlPltCnt > A_Idx )
		{
			m_PtrRtlPlt[A_Idx]	=	A_Color;
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrGdib::PalletCount( void )
{
		return	m_RtlPltCnt;
}
//--------------------------------------------------------------------
__u32	Cls_GrGdib::SaveToMem( void* A_PtrBuf )
{
	// local -------------------
		__u32		Tv_Result;
		Ptr_GrGdib	Tv_PtrHd;
		void*		Tv_PtrPlt;
		void*		Tv_PtrDib;
	// code --------------------
		//calculate size
		Tv_Result		=	sizeof(St_GrGdib);
		Tv_Result		=	Tv_Result + (m_RtlPltCnt * 4);
		Tv_Result		=	Tv_Result + GetDibSize();

		if ( NULL != A_PtrBuf )
		{
			Tv_PtrHd	=	(Ptr_GrGdib)A_PtrBuf;
			//copy head
			GrDumyCopyMem( Tv_PtrHd, &m_Gdib, sizeof(m_Gdib) );
			Tv_PtrHd->PtrExtDib		=	NULL;
			Tv_PtrHd->PtrExtPlt		=	NULL;
			Tv_PtrHd->Size				=	Tv_Result;
			//copy pallete
			if ( 0 != m_RtlPltCnt )
			{
				Tv_PtrPlt	=	GrGdibGetPalletPtr( Tv_PtrHd );
				GrDumyCopyMem( Tv_PtrPlt, GetPltPtr(), m_RtlPltCnt * 4 );
			}
			//copy dib
			Tv_PtrDib		=	GrGdibGetDibPtr( Tv_PtrHd );
			GrDumyCopyMem( Tv_PtrDib, GetDibPtr(), GetDibSize() );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcAlphaPltRemove( void )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=1;Tv_WkIdx < m_RtlPltCnt;Tv_WkIdx++ )
		{
			GrDrawColorAlpahSet( m_Gdib.FccImg, &m_PtrRtlPlt[Tv_WkIdx], FALSE );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ClipperSet( Ptr_GrRect A_PtrRc )
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		//check release clipper
		if ( NULL == A_PtrRc )
		{
			m_Rect.left		=	0;
			m_Rect.top		=	0;
			m_Rect.right	=	(__s32)m_Gdib.ResX;
			m_Rect.bottom	=	(__s32)m_Gdib.ResY;
		}
		else
		{
			Tv_Rc.left		=	0;
			Tv_Rc.top			=	0;
			Tv_Rc.right		=	(__s32)m_Gdib.ResX;
			Tv_Rc.bottom	=	(__s32)m_Gdib.ResY;
			if ( !GrDrawCnGetComRect( &m_Rect, &Tv_Rc, A_PtrRc ) )
			{
				m_Rect.left		=	0;
				m_Rect.top		=	0;
				m_Rect.right	=	0;
				m_Rect.bottom	=	0;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ClipperGet( Ptr_GrRect A_PtrRc )
{
		*A_PtrRc		=	m_Rect;
}
//--------------------------------------------------------------------
void	Cls_GrGdib::PalleteDefault( void )
{
		//release allocated pallete
		if ( NULL != m_PtrPlt )
		{
			free( m_PtrPlt );
			m_PtrPlt	=	NULL;
		}
		m_Gdib.PtrExtPlt	=	(void*)E_GrGdibPltBase;
		m_PtrRtlPlt	=	(__u32*)GrGdibGetPalletPtr( &m_Gdib );
}
//--------------------------------------------------------------------
void	Cls_GrGdib::SwapColor( __u32 A_Clr1, __u32 A_Clr2 )
{
		//check able
		if ( A_Clr1 != A_Clr2 )
		{
			if ( m_Gdib.IsPlt )
			{
				ProcSwapColorPlt( A_Clr1, A_Clr2 );
			}
			else
			{
				ProcSwapColorNpt( A_Clr1, A_Clr2 );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcSwapColorNpt( __u32 A_Clr1, __u32 A_Clr2 )
{
		//not implement
}
//--------------------------------------------------------------------
void	Cls_GrGdib::ProcSwapColorPlt( __u32 A_Clr1, __u32 A_Clr2 )
{
	// local -------------------
		__u32		Tv_WkX;
		__u32		Tv_WkY;
		__u32		Tv_PxVal;
	// code --------------------
		//change pallete
		Tv_PxVal	=	m_PtrRtlPlt[A_Clr1];
		m_PtrRtlPlt[A_Clr1]	=	m_PtrRtlPlt[A_Clr2];
		m_PtrRtlPlt[A_Clr2]	=	Tv_PxVal;
		//change pixel
		for ( Tv_WkY=0;Tv_WkY < m_Gdib.ResY;Tv_WkY++ )
		{
			for ( Tv_WkX=0;Tv_WkX < m_Gdib.ResX;Tv_WkX++ )
			{
				if ( GetPixelVal( Tv_WkX, Tv_WkY, &Tv_PxVal ) )
				{
					if ( Tv_PxVal == A_Clr1 )
					{
						SetPixelVal( Tv_WkX, Tv_WkY, A_Clr2 );
					}
					else if ( Tv_PxVal == A_Clr2 )
					{
						SetPixelVal( Tv_WkX, Tv_WkY, A_Clr1 );
					}
				}
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrGdib::FontV3Dib2Draw(__u8 A_FontPitch, void* A_PtrData, __u32 A_BitSize,
	__s32 A_PosX, __s32 A_PosY, BOOL8 A_IsDrawTxt, BOOL8 A_IsDrawShadow, __u32* A_PtrClrTbl)
{
	// local -------------------
		__u8*		Tv_PtrCode;
		__u32		Tv_Deced;
		__u32		Tv_Total;

		BOOL8		Tv_IsAble[5];

		__u8		Tv_Bit;
		__u8		Tv_Tmp;

		__u8		Tv_TblIdx;

		__u8		Tv_Cmd;
		__u32		Tv_Val;

		__u32		Tv_VbPos;		// value bit pos

		__s32		Tv_WkX;
		__s32		Tv_WkY;

		St_GrRect	Tv_RcLn;
	// code --------------------
		Tv_PtrCode		=	(__u8*)A_PtrData;
		Tv_Deced			=	0;
		Tv_Total			=	A_BitSize;
		Tv_Bit				=	0;

		Tv_IsAble[0]	=	A_IsDrawShadow;
		Tv_IsAble[1]	=	A_IsDrawTxt;
		Tv_IsAble[2]	=	A_IsDrawTxt;
		Tv_IsAble[3]	=	A_IsDrawTxt;
		Tv_IsAble[4]	=	A_IsDrawTxt;

		Tv_WkX				=	A_PosX;
		Tv_WkY				=	A_PosY;

		while(Tv_WkY < m_Rect.bottom)
		{
			// check completed
			if(Tv_Deced >= Tv_Total)
			{
				break;
			}
			// get command
			Tv_Tmp	=	((*Tv_PtrCode) >> Tv_Bit) & 0x0F;
			// go next code
			Tv_Deced	=	Tv_Deced + 4;
			Tv_Bit		=	Tv_Bit ^ 4;
			if(0 == Tv_Bit)
			{
				Tv_PtrCode ++;
			}
			Tv_Cmd	=	Tv_Tmp & 7;

			// get value
			Tv_Val		=	0;
			Tv_VbPos	=	0;
			while(0 != (E_GrFontV3Dib2AtbNext & Tv_Tmp))
			{
				// check completed
				if(Tv_Deced >= Tv_Total)
				{
					break;
				}
				// get value
				Tv_Tmp	=	((*Tv_PtrCode) >> Tv_Bit) & 0x0F;
				// go next code
				Tv_Deced	=	Tv_Deced + 4;
				Tv_Bit		=	Tv_Bit ^ 4;
				if(0 == Tv_Bit)
				{
					Tv_PtrCode ++;
				}

				// update value
				Tv_Val	=	Tv_Val | (((__u32)(Tv_Tmp & 7)) << Tv_VbPos);
				// next
				Tv_VbPos	=	Tv_VbPos + 3;
			}
			Tv_Val ++;

			// run command
			if(E_GrFontV3Dib2CmdLnSkip == Tv_Cmd)
			{
				Tv_WkX	=	A_PosX;
				Tv_WkY	=	Tv_WkY + (__s32)Tv_Val;
			}
			else if(E_GrFontV3Dib2CmdPxIntp8 >= Tv_Cmd)
			{
				if(E_GrFontV3Dib2CmdPxSkip != Tv_Cmd)
				{
					Tv_TblIdx	=	Tv_Cmd - E_GrFontV3Dib2CmdPxShadow;
					if(Tv_IsAble[Tv_TblIdx])
					{
						// draw able
						if(1 == Tv_Val)
						{
							// do pixel
							SetPixelVal(Tv_WkX, Tv_WkY, A_PtrClrTbl[Tv_TblIdx]);
						}
						else
						{
							// do line
							Tv_RcLn.left		=	Tv_WkX;
							Tv_RcLn.top			=	Tv_WkY;
							Tv_RcLn.right		=	Tv_WkX + (__s32)Tv_Val;
							Tv_RcLn.bottom	=	Tv_WkY + 1;
							FillRect(&Tv_RcLn, A_PtrClrTbl[Tv_TblIdx]);
						}
					}
				}
				// go next
				Tv_WkX	=	Tv_WkX + (__s32)Tv_Val;
			}
		}	//	while(Tv_WkY < Tv_RcClip.bottom)

}
//--------------------------------------------------------------------

