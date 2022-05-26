/*
 ui font modification object

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<DvlUi/GrUiDvlFont.h>
#include	<GrDrawTool.h>
#include	<GrFileCtrl.h>
#include	<GrDumyTool.h>
#include	<GrFileTool.h>
#include	<stdlib.h>

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
Cls_GrUiDvlFont::Cls_GrUiDvlFont( __u32 A_Fcc, __u32 A_Width, __u32 A_Height ):
Cls_GrUiFontObj( NULL )
{
		//init
		m_FccImg		=	A_Fcc;
		m_Width			=	A_Width;
		
		m_Gdib.ResY		=	A_Height;
		m_Gdib.Pitch	=	0;

		m_CharCnt		=	0;
		m_PtrDib		=	NULL;
		m_PtrMapTbl	=	m_MapTbl;
		m_GdibFont	=	NULL;
		m_GdibPad		=	NULL;

		//make gdib
		GrGdibInitHead( &m_Gdib, E_GrFccFont, A_Width, A_Height, 0,
										m_PtrDib, E_GrUiFontDibBitCnt, (void*)E_GrGdibPltBase );

		//create workpad
		RebuildWorkPad();

}
//--------------------------------------------------------------------
Cls_GrUiDvlFont::~Cls_GrUiDvlFont()
{
		if ( NULL != m_GdibPad )
		{
			delete	m_GdibPad;
		}
		if ( NULL != m_GdibFont )
		{
			delete	m_GdibFont;
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::LoadFontV2( Ptr_GrUiFontV2Head A_PtrFont )
{
	// local -------------------
		BOOL8		Tv_Result;
		void*		Tv_PtrMap;
		__u32		Tv_MapSize;
		void*		Tv_PtrDib;
		__u32		Tv_SizeDib;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		if ( NULL != m_GdibFont )
		{
			delete	m_GdibFont;
			m_GdibFont	=	NULL;
			m_PtrDib		=	NULL;
		}
		//update para		
		m_Width				=	A_PtrFont->Width;
		m_Gdib.ResY		=	A_PtrFont->Height;
		m_Gdib.Pitch	=	A_PtrFont->DibPitch;
		m_Gdib.ResX		=	A_PtrFont->DibWidth;
		m_CharCnt			=	A_PtrFont->CharCnt;

		if ( 0 != m_CharCnt )
		{
			//get map table
			Tv_PtrMap		=	(void*)( (__u32)A_PtrFont + sizeof(St_GrUiFontV2Head) );
			Tv_MapSize	=	m_CharCnt * sizeof(St_GrUiFontV2MapItm);
			GrDumyCopyMem( (void*)m_MapTbl, Tv_PtrMap, Tv_MapSize );

			//create object
			m_GdibFont	=	GrGdibCreate( m_FccImg, A_PtrFont->DibWidth, A_PtrFont->Height, A_PtrFont->DibPitch, NULL, 
										E_GrUiFontDibBitCnt );			
			m_PtrDib		=	m_GdibFont->GetDibPtr();
			//copy dib
			Tv_PtrDib		=	(void*)( (__u32)Tv_PtrMap + Tv_MapSize );
			Tv_SizeDib	=	A_PtrFont->DibPitch * A_PtrFont->Height;
			GrDumyCopyMem( m_PtrDib, Tv_PtrDib, Tv_SizeDib );

			//success
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrGdib*	Cls_GrUiDvlFont::GetWorkPad( void )
{
		return	m_GdibPad;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlFont::PadMakeShadow( void )
{
	// local -------------------
		__s32		Tv_WkX;
		__s32		Tv_WkY;
		__u32	Tv_PxVal;
		__u32	Tv_PxChk[4];
		__u32	Tv_WkIdx;
	// code --------------------
		//check exsit workpad
		if ( NULL != m_GdibPad )
		{
			//clear shadow
			for ( Tv_WkY=0;Tv_WkY < (__s32)m_Gdib.ResY;Tv_WkY++ )
			{
				for ( Tv_WkX=0;Tv_WkX < (__s32)m_Width;Tv_WkX++ )
				{
					if ( m_GdibPad->GetPixelVal( Tv_WkX, Tv_WkY, &Tv_PxVal ) )
					{
						if ( 2 == Tv_PxVal )
						{
							m_GdibPad->SetPixelVal( Tv_WkX, Tv_WkY, 0 );
						}
					}
				}
			}
			//make shadow
			for ( Tv_WkY=0;Tv_WkY < (__s32)m_Gdib.ResY;Tv_WkY++ )
			{
				for ( Tv_WkX=0;Tv_WkX < (__s32)m_Width;Tv_WkX++ )
				{
					if ( m_GdibPad->GetPixelVal( Tv_WkX, Tv_WkY, &Tv_PxVal ) )
					{
						if ( 0 == Tv_PxVal )
						{
							//check 
							Tv_PxChk[0]	=	0;
							Tv_PxChk[1]	=	0;
							Tv_PxChk[2]	=	0;
							Tv_PxChk[3]	=	0;
							m_GdibPad->GetPixelVal( Tv_WkX -1, Tv_WkY, &Tv_PxChk[0] );
							m_GdibPad->GetPixelVal( Tv_WkX , Tv_WkY -1, &Tv_PxChk[1] );
							m_GdibPad->GetPixelVal( Tv_WkX + 1, Tv_WkY, &Tv_PxChk[2] );
							m_GdibPad->GetPixelVal( Tv_WkX , Tv_WkY + 1, &Tv_PxChk[3] );
							//write
							for ( Tv_WkIdx=0;Tv_WkIdx < 4;Tv_WkIdx++ )
							{
								if ( 1 == Tv_PxChk[Tv_WkIdx] )
								{
									m_GdibPad->SetPixelVal( Tv_WkX, Tv_WkY, 2 );
									break;
								}
							}
						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::PadGetFontImage( __u16 A_Code, __s32 A_MovX, __s32 A_MovY )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrRect	Tv_RcFont;
		__s32			Tv_FontPitch;
		__s32			Tv_FontGap;
		__u32		Tv_MapIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check pand and font image exist
		if ( (NULL != m_GdibPad) && (NULL != m_GdibFont) )
		{
			if ( GetCharInfo( A_Code, &Tv_MapIdx, &Tv_RcFont, &Tv_FontPitch, &Tv_FontGap ) )
			{
				m_GdibPad->SetBltMode( FALSE, FALSE );
				m_GdibPad->FillRect( NULL, 0 );		//clear
				m_GdibPad->BltGdib( Tv_FontGap + A_MovX, A_MovY, m_GdibFont->GetGdibPtr(), &Tv_RcFont );
				m_GdibPad->FontBlt( Tv_FontGap + A_MovX, A_MovY, m_GdibFont->GetGdibPtr(), 0x7FFF, 0x
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::FontDel( __u16 A_Code )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrRect	Tv_RcFont;
		__s32			Tv_FontPitch;
		__s32			Tv_FontGap;
		Cls_GrGdib*	Tv_GdibNew;
		__u32		Tv_ResX;
		St_GrRect	Tv_RcCopy;
		__u32		Tv_MapIdx;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check exist
		if ( GetCharInfo( A_Code, &Tv_MapIdx, &Tv_RcFont, &Tv_FontPitch, &Tv_FontGap ) )
		{
			//check empty
			if ( 1 == m_CharCnt )
			{
				//change font
				delete	m_GdibFont;
				m_GdibFont	=	NULL;
				m_PtrDib		=	NULL;

				//update info
				m_Gdib.ResX		=	0;
				m_Gdib.Pitch	=	0;
				m_CharCnt			=	0;
				//success
				Tv_Result		=	TRUE;
			}
			else
			{
				//create new font image
				Tv_ResX			=	m_GdibFont->GetResX() - (__u32)Tv_FontPitch;
				Tv_GdibNew	=	GrGdibCreate( m_FccImg, Tv_ResX, m_GdibFont->GetResY(), 0, NULL, 
																		E_GrUiFontDibBitCnt );			
				if ( NULL != Tv_GdibNew )
				{
					Tv_GdibNew->SetBltMode( FALSE, FALSE );
					//copy head
					if ( 0 != Tv_RcFont.left )
					{
						Tv_RcCopy.left		=	0;
						Tv_RcCopy.top			=	0;
						Tv_RcCopy.right		=	Tv_RcFont.left;
						Tv_RcCopy.bottom	=	m_GdibFont->GetResY();
						Tv_GdibNew->BltGdib( 0, 0, m_GdibFont->GetGdibPtr(), &Tv_RcCopy );
					}
					//copy tail
					if ( m_GdibFont->GetResX() != Tv_RcFont.right )
					{
						Tv_RcCopy.left		=	Tv_RcFont.right;
						Tv_RcCopy.top			=	0;
						Tv_RcCopy.right		=	m_GdibFont->GetResX();
						Tv_RcCopy.bottom	=	m_GdibFont->GetResY();
						Tv_GdibNew->BltGdib( Tv_RcFont.left, 0, m_GdibFont->GetGdibPtr(), &Tv_RcCopy );
						//collect map
						for ( Tv_WkIdx=Tv_MapIdx;Tv_WkIdx < m_CharCnt - 1;Tv_WkIdx++ )
						{
							m_MapTbl[Tv_WkIdx].Code		=	m_MapTbl[Tv_WkIdx+1].Code;
							m_MapTbl[Tv_WkIdx].Pitch	=	m_MapTbl[Tv_WkIdx+1].Pitch;
							m_MapTbl[Tv_WkIdx].StGap	=	m_MapTbl[Tv_WkIdx+1].StGap;
						}
					}

					//change font
					delete	m_GdibFont;
					m_GdibFont	=	Tv_GdibNew;
					m_PtrDib		=	m_GdibFont->GetDibPtr();

					//update info
					m_Gdib.ResX	=	m_GdibFont->GetResX();
					m_Gdib.Pitch	=	m_GdibFont->GetDibPitch();
					m_CharCnt	--;
					//success
					Tv_Result		=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::FontAdd( __u16 A_Code, Ptr_GrRect A_PtrRcPad )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_ContWidth;
		Cls_GrGdib*	Tv_GdibNew;
		__u32		Tv_ResX;
	// code --------------------
		//init
		Tv_Result			=	FALSE;
		Tv_ContWidth	=	A_PtrRcPad->right - A_PtrRcPad->left;

		//check allocated
		if ( NULL == m_GdibFont )
		{
			//new allocate
			m_GdibFont	=	GrGdibCreate( m_FccImg, Tv_ContWidth, m_Gdib.ResY, 0, NULL, 
										E_GrUiFontDibBitCnt );
			if ( NULL != m_GdibFont )
			{
				//init
				m_Gdib.ResX		=	0;
				m_Gdib.Pitch	=	0;
				//copy contents
				m_GdibFont->SetBltMode( FALSE, FALSE );
				m_GdibFont->BltGdib( 0, 0, m_GdibPad->GetGdibPtr(), A_PtrRcPad );
				//success
				Tv_Result		=	TRUE;
			}
		}
		else
		{
			//create new font
			Tv_ResX		=	m_Gdib.ResX + (__u32)Tv_ContWidth;
			Tv_GdibNew	=	GrGdibCreate( m_FccImg, Tv_ResX, m_Gdib.ResY, 0, NULL, 
										E_GrUiFontDibBitCnt );
			if ( NULL != Tv_GdibNew )
			{
				//copy old data
				Tv_GdibNew->SetBltMode( FALSE, FALSE );
				Tv_GdibNew->BltGdib( 0, 0, m_GdibFont->GetGdibPtr() );
				//add new data
				Tv_GdibNew->BltGdib( m_Gdib.ResX, 0, m_GdibPad->GetGdibPtr(), A_PtrRcPad );
				//change 
				delete	m_GdibFont;
				m_GdibFont	=	Tv_GdibNew;
				//success
				Tv_Result		=	TRUE;
			}
		}
		//check success
		if ( Tv_Result )
		{
			//update map
			m_MapTbl[m_CharCnt].Code		=	A_Code;
			m_MapTbl[m_CharCnt].Pitch		=	(__u8)Tv_ContWidth;
			m_MapTbl[m_CharCnt].StGap		=	A_PtrRcPad->left;
			m_CharCnt ++;
			//update dib
			m_Gdib.ResX		=	m_GdibFont->GetResX();
			m_Gdib.Pitch	=	m_GdibFont->GetDibPitch();
			m_PtrDib			=	m_GdibFont->GetDibPtr();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlFont::FontClear( void )
{
		if ( NULL != m_GdibFont )
		{
			delete	m_GdibFont;
			m_GdibFont	=	NULL;
		}
		m_Gdib.Pitch	=	0;
		m_Gdib.ResX		=	0;
		m_CharCnt			=	0;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::PadGetRect( Ptr_GrRect A_PtrRtRc )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_WkX;
		__s32			Tv_WkY;
		__u32		Tv_PxVal;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		A_PtrRtRc->top		=	0;
		A_PtrRtRc->bottom	=	m_Gdib.ResY;

		//exist pad
		if ( NULL != m_GdibPad )
		{
			//left area
			for ( Tv_WkX = 0;Tv_WkX < (__s32)m_Width;Tv_WkX++ )
			{
				for ( Tv_WkY = 0;Tv_WkY < (__s32)m_Gdib.ResY;Tv_WkY++ )
				{
					if ( m_GdibPad->GetPixelVal( Tv_WkX, Tv_WkY, &Tv_PxVal ) )
					{
						if ( 0 != Tv_PxVal )
						{
							//finded
							A_PtrRtRc->left	=	Tv_WkX;
							Tv_Result		=	TRUE;
							break;
						}
					}
				}
				if ( Tv_Result )
				{
					break;
				}
			}
			//check finded
			if ( Tv_Result )
			{
				//right area
				Tv_Result	=	FALSE;
				for ( Tv_WkX = m_Width - 1;Tv_WkX >= 0;Tv_WkX-- )
				{
					for ( Tv_WkY = 0;Tv_WkY < (__s32)m_Gdib.ResY;Tv_WkY++ )
					{
						if ( m_GdibPad->GetPixelVal( Tv_WkX, Tv_WkY, &Tv_PxVal ) )
						{
							if ( 0 != Tv_PxVal )
							{
								//finded
								A_PtrRtRc->right	=	Tv_WkX + 1;
								Tv_Result		=	TRUE;
								break;
							}
						}
					}
					if ( Tv_Result )
					{
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::PadSaveToFont( __u16 A_Code )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrRect	Tv_RcPad;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check exist pad
		if ( NULL != m_GdibPad )
		{
			//make shadow
			PadMakeShadow();
			//get font rect
			if ( PadGetRect( &Tv_RcPad ) )
			{
				if ( IsCodeExist(A_Code) )
				{
					//relpace font
					Tv_Result	=	FontOvwt( A_Code, &Tv_RcPad );
				}
				else
				{
					//add font
					Tv_Result	=	FontAdd( A_Code, &Tv_RcPad );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::LoadFontV1( Ptr_GrUiFontV1Head A_PtrFont )
{
	// local -------------------
		BOOL8		Tv_Result;
		void*		Tv_PtrDib;
		__u32		Tv_SizeDib;
		__u32		Tv_PltCnt;
		Cls_GrGdib*	Tv_GdibTmp;
		__u32		Tv_WkIdx;
		Ptr_GrUiFontV1MapItm	Tv_PtrV1Map;
		__u32		Tv_MapSize;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		if ( NULL != m_GdibFont )
		{
			delete	m_GdibFont;
			m_GdibFont	=	NULL;
		}
		//update para		
		m_Width			=	A_PtrFont->Width;
		m_Gdib.ResX	=	m_Width;
		m_Gdib.ResY	=	A_PtrFont->Height;
		m_Gdib.ResX	=	A_PtrFont->DibWidth;
		m_Gdib.Pitch	=	A_PtrFont->DibPitch;
		m_CharCnt		=	A_PtrFont->CharCnt;

		if ( 0 != m_CharCnt )
		{
			//get map table
			Tv_PltCnt	=	(1 << A_PtrFont->PxBit) - 1;
			Tv_PtrV1Map	=	(Ptr_GrUiFontV1MapItm)( (__u32)A_PtrFont + sizeof(St_GrUiFontV1Head) + (4 * Tv_PltCnt) );
			for (Tv_WkIdx=0;Tv_WkIdx < m_CharCnt;Tv_WkIdx++)
			{
				m_MapTbl[Tv_WkIdx].Code		=	Tv_PtrV1Map[Tv_WkIdx].Code;
				m_MapTbl[Tv_WkIdx].Pitch	=	Tv_PtrV1Map[Tv_WkIdx].Pitch;
				m_MapTbl[Tv_WkIdx].StGap	=	Tv_PtrV1Map[Tv_WkIdx].StGap;
			}
			//patch pitch
			if ( E_GrUiFontDibBitCnt != A_PtrFont->PxBit )
			{
				m_Gdib.Pitch	=	0;
			}
			
			//create object
			m_GdibFont	=	GrGdibCreate( m_FccImg, m_Gdib.ResX, m_Gdib.ResY, m_Gdib.Pitch, NULL, 
										E_GrUiFontDibBitCnt );			
			m_PtrDib		=	m_GdibFont->GetDibPtr();
			//check match bit count
			Tv_MapSize	=	m_CharCnt * sizeof(St_GrUiFontV1MapItm);
			Tv_PtrDib		=	(void*)( (__u32)Tv_PtrV1Map + Tv_MapSize );
			if ( 1 == A_PtrFont->PxBit )
			{
				//convert
				Tv_GdibTmp	=	GrGdibCreate( m_FccImg, m_Gdib.ResX, m_Gdib.ResY, A_PtrFont->DibPitch, Tv_PtrDib, 
											A_PtrFont->PxBit );
				if ( NULL != Tv_GdibTmp )
				{
					Tv_GdibTmp->CvtTo( m_GdibFont );
					delete	Tv_GdibTmp;
				}
			}
			else
			{
				//direct copying
				Tv_SizeDib	=	A_PtrFont->DibPitch * m_Gdib.ResY;
				GrDumyCopyMem( m_GdibFont->GetDibPtr(), Tv_PtrDib, Tv_SizeDib );
			}

			//update dib
			m_Gdib.Pitch	=	m_GdibFont->GetDibPitch();
			//success
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlFont::BuildShadowAll( __s32 A_MovX, __s32 A_MovY )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u16		Tv_Code;
		St_GrRect	Tv_RcCont;
		__s32			Tv_FontPitch;
		__s32			Tv_StGap;
	// code --------------------
		if ( 0 != m_CharCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_CharCnt;Tv_WkIdx++ )
			{
				//get info
				if ( !GetItemInfo( Tv_WkIdx, &Tv_Code, &Tv_RcCont, &Tv_FontPitch, &Tv_StGap ) )
				{
					break;
				}
				//load font image
				if ( !PadGetFontImage( Tv_Code, A_MovX, A_MovY ) )
				{
					break;
				}
				//make shadow
				PadMakeShadow();
				//get contents range
				if ( PadGetRect( &Tv_RcCont ) )
				{
					//overwrite font
					if ( !FontOvwt( Tv_Code, &Tv_RcCont ) )
					{
						break;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::FontOvwt( __u16 A_Code, Ptr_GrRect A_PtrRcPad )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrRect	Tv_RcFont;
		__s32			Tv_FontPitch;
		__s32			Tv_FontGap;
		Cls_GrGdib*	Tv_GdibNew;
		__u32		Tv_ResX;
		St_GrRect	Tv_RcCopy;
		__u32		Tv_MapIdx;
		__s32			Tv_NewPitch;			//new font pitch
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check exist
		if ( GetCharInfo( A_Code, &Tv_MapIdx, &Tv_RcFont, &Tv_FontPitch, &Tv_FontGap ) )
		{
			//calculate new size
			Tv_NewPitch	=	A_PtrRcPad->right - A_PtrRcPad->left;
			//check change size
			Tv_Result		=	TRUE;
			if ( Tv_NewPitch != Tv_FontPitch )
			{
				//init
				Tv_Result		=	FALSE;
				//create new font image
				Tv_ResX			=	m_Gdib.ResX - (__u32)Tv_FontPitch + (__u32)Tv_NewPitch;
				Tv_GdibNew	=	GrGdibCreate( m_FccImg, Tv_ResX, m_Gdib.ResY, 0, NULL, 
											E_GrUiFontDibBitCnt );			
				if ( NULL != Tv_GdibNew )
				{
					//set blt mode
					Tv_GdibNew->SetBltMode( FALSE, FALSE );
					//copy head
					if ( 0 != Tv_RcFont.left )
					{
						Tv_RcCopy.left		=	0;
						Tv_RcCopy.top			=	0;
						Tv_RcCopy.right		=	Tv_RcFont.left;
						Tv_RcCopy.bottom	=	m_Gdib.ResY;
						Tv_GdibNew->BltGdib( 0, 0, m_GdibFont->GetGdibPtr(), &Tv_RcCopy );
					}
					//copy tail
					if ( m_Gdib.ResX != (__u32)Tv_RcFont.right )
					{
						Tv_RcCopy.left		=	Tv_RcFont.right;
						Tv_RcCopy.top			=	0;
						Tv_RcCopy.right		=	m_Gdib.ResX;
						Tv_RcCopy.bottom	=	m_Gdib.ResY;
						Tv_GdibNew->BltGdib( Tv_RcFont.left + Tv_NewPitch, 0, m_GdibFont->GetGdibPtr(), &Tv_RcCopy );
					}

					//change font
					delete	m_GdibFont;
					m_GdibFont	=	Tv_GdibNew;

					//update info
					m_Gdib.ResX	=	m_GdibFont->GetResX();
					m_Gdib.Pitch	=	m_GdibFont->GetDibPitch();
					m_PtrDib		=	m_GdibFont->GetDibPtr();
					//success
					Tv_Result		=	TRUE;
				}
			}
			//update font
			if ( Tv_Result )
			{
				m_GdibFont->SetBltMode( FALSE, FALSE );
				m_GdibFont->BltGdib( Tv_RcFont.left, 0, m_GdibPad->GetGdibPtr(), A_PtrRcPad );
				//update info
				m_MapTbl[Tv_MapIdx].Pitch		=	(__u8)Tv_NewPitch;
				m_MapTbl[Tv_MapIdx].StGap		=	(__u8)A_PtrRcPad->left;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::LoadFromFile( __s8* A_StrFn )
{
	// local -------------------
		BOOL8		Tv_Result;
		void*		Tv_PtrBuf;
		__u32		Tv_Size;
		Cls_GrFileCtrl*	Tv_ObjFl;
		__u32*	Tv_PtrFccId;

	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//create file control
		Tv_ObjFl	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE );
		if ( Tv_ObjFl->IsOpened() )
		{
			//get size
			Tv_Size		=	(__u32)Tv_ObjFl->GetSize();
			Tv_PtrBuf	=	malloc( Tv_Size );
			if ( NULL != Tv_PtrBuf )
			{
				//load
				Tv_ObjFl->Seek( 0 );
				if ( Tv_Size == Tv_ObjFl->Read( Tv_PtrBuf, Tv_Size ) )
				{
					//check id
					Tv_PtrFccId	=	(__u32*)Tv_PtrBuf;
					if ( E_GrUiFontVer1Id == *Tv_PtrFccId )
					{
						//old type
						Tv_Result		=	LoadFontV1( (Ptr_GrUiFontV1Head)Tv_PtrBuf );
						//build pad
						if ( NULL != m_GdibPad )
						{
							delete	m_GdibPad;
							m_GdibPad		=	NULL;
						}

						RebuildWorkPad();
						//correct font
						FontCorrect();
						//build shadow
						BuildShadowAll();
					}
					else if ( E_GrUiFontVer2Id == *Tv_PtrFccId )
					{
						//new type
						Tv_Result		=	LoadFontV2( (Ptr_GrUiFontV2Head)Tv_PtrBuf );
						//build pad
						if ( NULL != m_GdibPad )
						{
							delete	m_GdibPad;
							m_GdibPad		=	NULL;
						}
						RebuildWorkPad();
					}
				}
				//release memory
				free( Tv_PtrBuf );
			}
		}
		delete	Tv_ObjFl;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::LoadFromFile( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8		Tv_Result;
		void*		Tv_PtrBuf;
		__u32		Tv_Size;
		Cls_GrFileCtrl*	Tv_ObjFl;
		__u32*	Tv_PtrFccId;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//create file control
		Tv_ObjFl	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE );
		if ( Tv_ObjFl->IsOpened() )
		{
			//get size
			Tv_Size		=	(__u32)Tv_ObjFl->GetSize();
			Tv_PtrBuf	=	malloc( Tv_Size );
			if ( NULL != Tv_PtrBuf )
			{
				//load
				Tv_ObjFl->Seek( 0 );
				if ( Tv_Size == Tv_ObjFl->Read( Tv_PtrBuf, Tv_Size ) )
				{
					//check id
					Tv_PtrFccId	=	(__u32*)Tv_PtrBuf;
					if ( E_GrUiFontVer1Id == *Tv_PtrFccId )
					{
						//old type
						Tv_Result		=	LoadFontV1( (Ptr_GrUiFontV1Head)Tv_PtrBuf );
						//build pad
						if ( NULL != m_GdibPad )
						{
							delete	m_GdibPad;
							m_GdibPad		=	NULL;
						}

						RebuildWorkPad();
						//correct font
						FontCorrect();
						//build shadow
						BuildShadowAll();
					}
					else if ( E_GrUiFontVer2Id == *Tv_PtrFccId )
					{
						//new type
						Tv_Result		=	LoadFontV2( (Ptr_GrUiFontV2Head)Tv_PtrBuf );
						//build pad
						if ( NULL != m_GdibPad )
						{
							delete	m_GdibPad;
							m_GdibPad		=	NULL;
						}
						RebuildWorkPad();
					}
				}
				//release memory
				free( Tv_PtrBuf );
			}
		}
		delete	Tv_ObjFl;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::SaveToFile( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrFileCtrl*	Tv_ObjFl;
		St_GrUiFontV2Head	Tv_Fh;
		__u32	Tv_Size;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check exist font
		if ( 0 != m_CharCnt )
		{
			//delete exist file
			if ( GrFileIsExist( A_StrFn ) )
			{
				GrFileDelete( A_StrFn );
			}

			//check file name
			Tv_ObjFl	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, TRUE, TRUE );
			if ( Tv_ObjFl->IsOpened() )
			{
				//write header
				Tv_Fh.Fcc			=	E_GrUiFontVer2Id;
				Tv_Fh.Width		=	(__u16)m_Width;
				Tv_Fh.Height	=	(__u16)m_Gdib.ResY;
				Tv_Fh.DibWidth	=	m_Gdib.ResX;
				Tv_Fh.DibPitch	=	m_Gdib.Pitch;
				Tv_Fh.CharCnt		=	m_CharCnt;
				if ( sizeof(Tv_Fh) == Tv_ObjFl->Write( &Tv_Fh, sizeof(Tv_Fh) ) )
				{
					//write map table
					Tv_Size		=	sizeof(St_GrUiFontV2MapItm) * m_CharCnt;
					if ( Tv_Size == Tv_ObjFl->Write( (void*)m_MapTbl, Tv_Size ) )
					{
						//write dib
						if ( m_GdibFont->GetDibSize() == Tv_ObjFl->Write( m_GdibFont->GetDibPtr(), m_GdibFont->GetDibSize() ) )
						{
							//success
							Tv_Result	=	TRUE;
						}
					}
				}
			}
			delete	Tv_ObjFl;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlFont::RebuildWorkPad( void )
{
		if ( NULL != m_GdibPad )
		{
			delete	m_GdibPad;
			m_GdibPad		=	NULL;
		}
		m_GdibPad		=	GrGdibCreate( m_FccImg, m_Width, m_Gdib.ResY, 0, NULL, E_GrUiFontDibBitCnt );
		m_GdibPad->FillRect( NULL, 0 );
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlFont::CalcChangedDibWidth( __u32 A_Width )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_WkIdx;
		__u32		Tv_Pitch;
	// code --------------------
		//init
		Tv_Result	=	0;

		//change size
		if ( 0 != m_CharCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_CharCnt;Tv_WkIdx++ )
			{
				Tv_Pitch	=	m_MapTbl[Tv_WkIdx].Pitch;
				if ( A_Width < Tv_Pitch )
				{
					Tv_Pitch	=	A_Width;
				}
				//add
				Tv_Result	=	Tv_Result + Tv_Pitch;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlFont::ChangeSize( __u32 A_Width, __u32 A_Height )
{
	// local -------------------
		Cls_GrGdib*	Tv_GdibNew;
		__u32		Tv_DibWidthNew;
		Ptr_GrUiFontV2MapItm	Tv_PtrMapNew;
		__u32		Tv_WkIdx;
		St_GrRect	Tv_RcCopy;
		__s32			Tv_NewX;
		__s32			Tv_OldX;
	// code --------------------
		//check font exist
		if ( (NULL != m_GdibFont) && (0 != m_CharCnt) )
		{
			//create new font
			Tv_DibWidthNew	=	CalcChangedDibWidth( A_Width );
			Tv_GdibNew	=	GrGdibCreate( m_FccImg, Tv_DibWidthNew, A_Height, 0, NULL, 
										E_GrUiFontDibBitCnt );
			Tv_PtrMapNew	=	(Ptr_GrUiFontV2MapItm)malloc( sizeof(St_GrUiFontV2MapItm) * 0x10000 );
			//convert
			Tv_GdibNew->FillRect( NULL, 0 );
			Tv_GdibNew->SetBltMode( FALSE, FALSE );
			Tv_RcCopy.top			=	0;
			Tv_RcCopy.bottom	=	m_Gdib.ResY;
			Tv_NewX		=	0;
			Tv_OldX		=	0;
			for ( Tv_WkIdx=0;Tv_WkIdx < m_CharCnt;Tv_WkIdx++ )
			{
				//update map
				Tv_PtrMapNew[Tv_WkIdx].Code		=	m_MapTbl[Tv_WkIdx].Code;
				Tv_PtrMapNew[Tv_WkIdx].StGap	=	m_MapTbl[Tv_WkIdx].StGap;
				Tv_PtrMapNew[Tv_WkIdx].Pitch	=	m_MapTbl[Tv_WkIdx].Pitch;
				if ( A_Width < Tv_PtrMapNew[Tv_WkIdx].Pitch )
				{
					Tv_PtrMapNew[Tv_WkIdx].Pitch	=	(__u8)A_Width;
				}

				//copy image
				Tv_RcCopy.left		=	Tv_OldX;
				Tv_RcCopy.right		=	Tv_OldX + (__s32)Tv_PtrMapNew[Tv_WkIdx].Pitch;
				Tv_GdibNew->BltGdib( Tv_NewX, 0, m_GdibFont->GetGdibPtr(), &Tv_RcCopy );
				//next
				Tv_NewX	=	Tv_NewX + (__s32)Tv_PtrMapNew[Tv_WkIdx].Pitch;
				Tv_OldX	=	Tv_OldX + (__s32)m_MapTbl[Tv_WkIdx].Pitch;
			}
			
			//change font
			delete	m_GdibFont;
			m_GdibFont	=	Tv_GdibNew;

			//update map
			GrDumyCopyMem( (void*)m_MapTbl, Tv_PtrMapNew, sizeof(St_GrUiFontV2MapItm) * m_CharCnt );
			free( (void*)Tv_PtrMapNew );
		}
		else
		{
			//release font
			delete	m_GdibFont;
			m_GdibFont	=	NULL;
		}

		//update spec
		m_Width		=	A_Width;
		m_Gdib.ResY	=	A_Height;
		if ( NULL != m_GdibFont )
		{
			m_Gdib.Pitch	=	m_GdibFont->GetDibPitch();
			m_Gdib.ResX		=	m_GdibFont->GetResX();
			m_PtrDib			=	m_GdibFont->GetDibPtr();
		}
		//update pad
		RebuildWorkPad();
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlFont::FontCorrect( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_MapIdx;
		St_GrRect	Tv_RcFont;
		__s32			Tv_FontPitch;
		__s32			Tv_FontGap;
	// code --------------------
		//check count
		if ( 0 != m_CharCnt )
		{
			Tv_WkIdx		=	0;
			while ( m_CharCnt > Tv_WkIdx )
			{
				//find already exist
				if ( GetCharInfo( m_MapTbl[Tv_WkIdx].Code, &Tv_MapIdx, &Tv_RcFont, &Tv_FontPitch, &Tv_FontGap ) )
				{
					//check already exist
					if ( Tv_MapIdx != Tv_WkIdx )
					{
						//delete 
						FontDel( m_MapTbl[Tv_WkIdx].Code );
						//retry
						Tv_WkIdx	=	0;
						continue;
					}
				}
				//next
				Tv_WkIdx ++;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::GdibIsBgVtLine( __s32 A_X, Cls_GrGdib* A_Gdib )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_WkY;
		__u32		Tv_PxVal;
	// code --------------------
		//init
		Tv_Result	=	TRUE;

		//check
		for ( Tv_WkY = 0;Tv_WkY < (__s32)A_Gdib->GetResY();Tv_WkY++ )
		{
			if ( A_Gdib->GetPixelVal( A_X, Tv_WkY, &Tv_PxVal ) )
			{
				if ( 0 != Tv_PxVal )
				{
					Tv_Result	=	FALSE;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlFont::GdibFindFontPos( __s32 A_StartX, __s32 A_MinGapX, Cls_GrGdib* A_Gdib, __s32* A_PtrRtStX, __s32* A_PtrRtEdX )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_WkX;
		__s32		Tv_GapCnt;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check first line
		Tv_WkX		=	A_StartX;
		while ( Tv_WkX < (__s32)A_Gdib->GetResX() )
		{
			if ( !GdibIsBgVtLine(Tv_WkX, A_Gdib ) )
			{
				Tv_Result	=	TRUE;
				*A_PtrRtStX	=	Tv_WkX;
				*A_PtrRtEdX	=	Tv_WkX;
				break;
			}
			//next
			Tv_WkX ++;
		}

		//check contents
		if ( Tv_Result )
		{
			Tv_WkX ++;
			Tv_GapCnt		=	0;
			while ( Tv_WkX < (__s32)A_Gdib->GetResX() )
			{
				if ( GdibIsBgVtLine(Tv_WkX, A_Gdib ) )
				{
					Tv_GapCnt ++;
					if ( Tv_GapCnt > A_MinGapX )
					{
						break;
					}
				}
				else
				{
					//update end positon
					*A_PtrRtEdX	=	Tv_WkX;
					Tv_GapCnt		=	0;
				}
				//next
				Tv_WkX ++;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlFont::GdibCvtByFont( Cls_GrGdib* A_GdibTg, Cls_GrGdib* A_GdibSr )
{
	// local -------------------
		__s32		Tv_WkX;
		__s32		Tv_WkY;
		__u32	Tv_PxBg;
		__u32	Tv_PxVal;
		__u32	Tv_PxWt;
	// code --------------------
		//get background pixel
		if ( A_GdibSr->GetPixelVal( 0, 0, &Tv_PxBg ) )
		{
			for ( Tv_WkY=0;Tv_WkY < (__s32)A_GdibTg->GetResY();Tv_WkY++ )
			{
				for ( Tv_WkX=0;Tv_WkX < (__s32)A_GdibTg->GetResX();Tv_WkX++ )
				{
					if ( A_GdibSr->GetPixelVal( Tv_WkX, Tv_WkY, &Tv_PxVal ) )
					{
						Tv_PxWt		=	1;
						if ( Tv_PxBg == Tv_PxVal )
						{
							Tv_PxWt		=	0;
						}
						//write
						A_GdibTg->SetPixelVal( Tv_WkX, Tv_WkY, Tv_PxWt );
					}
				}
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlFont::AddFromBmp( WCHAR* A_StrFn, __u16 A_StCode, __s32 A_MinXGap )
{
	// local -------------------
		__u32		Tv_Result;
		Cls_GrGdib*	Tv_GdibBmp;
		Cls_GrGdib*	Tv_GdibCvt;
		__u16		Tv_WkCode;
		__s32			Tv_StX;
		__s32			Tv_EdX;
		__s32			Tv_WkX;
		St_GrRect	Tv_RcCopy;
		__s32			Tv_GetSizeX;
		__s32			Tv_StGap;
	// code --------------------
		//init
		Tv_Result		=	0;
		Tv_GdibBmp	=	NULL;
		Tv_GdibCvt	=	NULL;

		//load bmp
		Tv_GdibBmp	=	GrGdibLoadBmp( A_StrFn, 0xFFFF, 80 );
		if ( NULL != Tv_GdibBmp )
		{
			Tv_GdibCvt	=	GrGdibCreate( m_FccImg, Tv_GdibBmp->GetResX(), Tv_GdibBmp->GetResY(), 0, NULL, 
																	E_GrUiFontDibBitCnt, NULL );
			if ( NULL != Tv_GdibCvt )
			{
				//convert
				GdibCvtByFont( Tv_GdibCvt, Tv_GdibBmp );
				//get work pad
				if ( NULL != m_GdibPad )
				{
					//start
					m_GdibPad->SetBltMode( FALSE, FALSE );
					Tv_WkCode	=	A_StCode;
					Tv_WkX		=	0;
					//get background color
					while ( GdibFindFontPos( Tv_WkX, A_MinXGap, Tv_GdibCvt, &Tv_StX, &Tv_EdX ) )
					{
						Tv_GetSizeX	=	(Tv_EdX - Tv_StX) + 1;
						//check range
						if ( (__s32)m_GdibPad->GetResX() < Tv_GetSizeX )
						{
							break;
						}
						
						//add
						m_GdibPad->FillRect( NULL, 0 );
						Tv_RcCopy.left		=	Tv_StX;
						Tv_RcCopy.top			=	0;
						Tv_RcCopy.right		=	Tv_EdX + 1;
						Tv_RcCopy.bottom	=	m_GdibPad->GetResY();
						Tv_StGap					= (m_GdibPad->GetResX() -	Tv_GetSizeX) / 2;
						m_GdibPad->BltGdib( Tv_StGap, 0, Tv_GdibCvt->GetGdibPtr(), &Tv_RcCopy );

						//make shadow
						PadMakeShadow();
						//write
						PadSaveToFont( Tv_WkCode );

						//next
						Tv_Result ++;
						Tv_WkX	=	Tv_EdX + 1;
						if ( 127 == Tv_WkCode )
						{
							//byte code finish
							break;
						}
						Tv_WkCode	++;
					}
				}
			}
			//release dib
			if ( NULL != Tv_GdibCvt )
			{
				delete	Tv_GdibCvt;
			}
			delete	Tv_GdibBmp;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDvlFont::AddFromFont( Cls_GrUiFontObj* A_ObjFont )
{
	// local -------------------
		__u32	Tv_Result;
		Ptr_GrGdib	Tv_GdibFont;
		__u16		Tv_Code;
		St_GrRect	Tv_RcFont;
		__s32			Tv_FontPitch;
		__s32			Tv_FontGap;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	0;

		//check pad exist
		if ( NULL != m_GdibPad )
		{
			//make gdib
			Tv_GdibFont	=	A_ObjFont->GetGdibPtr();
			if ( NULL != Tv_GdibFont )
			{
				if ( 0 != A_ObjFont->GetCharCnt() )
				{
					m_GdibPad->SetBltMode( FALSE, FALSE );
					for ( Tv_WkIdx=0;Tv_WkIdx < A_ObjFont->GetCharCnt();Tv_WkIdx++ )
					{
						if ( A_ObjFont->GetItemInfo( Tv_WkIdx, &Tv_Code, &Tv_RcFont, &Tv_FontPitch, &Tv_FontGap ) )
						{
							m_GdibPad->BltGdib( Tv_FontGap, 0, Tv_GdibFont, &Tv_RcFont );
							PadSaveToFont( Tv_Code );
							Tv_Result ++;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------


