/*
 quad

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoQuad.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>

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
Cls_GrUiDoQuad::Cls_GrUiDoQuad( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
	// local -------------------
		__u8		Tv_WkIdx;
	// code --------------------
		//init
		m_TypeId			=	E_GrUiDoTypeIdQuad;

		m_IsFocusAble	=	TRUE;

		m_ClrTitle		=	m_ObjConsole->ClrWhiteGet();
		m_ClrGrid			=	m_ClrTitle;
		m_ChDrawGap		=	0;

		GrDumyZeroMem( m_IconTbl, sizeof(m_IconTbl) );
		m_IconSizeX		=	E_GrUiDoQuadDfltIconSizeX;
		m_IconSizeY		=	E_GrUiDoQuadDfltIconSizeY;
		m_IconGapTop	=	E_GrUiDoQuadDfltIconGapTop;
		m_IconGapBtw	=	0;
		m_GridWidth		=	0;

		GrDumyZeroMem( &m_TextPara, sizeof(m_TextPara) );
		m_TextPara.ClrShadow	=	m_ObjConsole->ClrBlackGet();

		GrDumyCopyMem( &m_ExtTxtPara, &m_TextPara, sizeof(m_TextPara) );
 
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
		{
#ifdef	GR_OSD_EDIT
			GrStrIntToWstr( m_StrTitleTbl[Tv_WkIdx], (__s32)Tv_WkIdx + 1 );
			GrStrWcopy( m_StrExtTtTbl[Tv_WkIdx], L"EXT" );
#else
			GrStrClear( m_StrTitleTbl[Tv_WkIdx] );
			GrStrClear( m_StrExtTtTbl[Tv_WkIdx] );
#endif
			m_PntRecAtb[Tv_WkIdx]	=	0;
			m_ExtTtClr[Tv_WkIdx]		=	m_ClrTitle;
		}

		m_PntCvtMap		=	m_ObjConsole->CorvertMapGet();
		m_PntNsgMap		=	m_ObjConsole->NoSigMapGet();
		m_PntViewCh		=	0;

		m_IsGridShow	=	E_GrUiDoQuadDfltGridShow;
		m_IsIconShow	=	E_GrUiDoQuadDfltIconShow;
		m_IsTitleShow	=	E_GrUiDoQuadDfltTitleShow;
		m_IsBgImgShow	=	E_GrUiDoQuadDfltImageShow;
		m_PosMode			=	0;
#ifdef	GR_OSD_EDIT
		m_IsExtTtShow	=	TRUE;
#else
		m_IsExtTtShow	=	FALSE;
#endif

		m_QuadPrvMode	=	E_GrDvrQuadMode1;			//init previous quad mode

		m_GridLineCnt	=	0;
		m_ViewItemCnt	=	0;
		m_IsChItmPntReq	=	FALSE;
		m_IsRecIconLock	=	FALSE;
		m_IsChg	=	FALSE;

		LcQuadParaUpdate();
}
//--------------------------------------------------------------------
Cls_GrUiDoQuad::~Cls_GrUiDoQuad()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::LcPaintGridLine( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__s32		Tv_Add;
	// code --------------------
		if ( (0 != m_GridLineCnt) && (m_IsGridShow) )
		{
			if ( PaintClipperSet( FALSE ) )
			{
				for ( Tv_WkIdx = 0;Tv_WkIdx < m_GridLineCnt;Tv_WkIdx++ )
				{
					Tv_Add	=	0;
					if ( m_GridLineTbl[Tv_WkIdx].St.x == m_GridLineTbl[Tv_WkIdx].Ed.x )
					{
						while ( (__s32)m_GridWidth >= Tv_Add )
						{
							m_GdibScr->DrawLine( m_GridLineTbl[Tv_WkIdx].St.x + Tv_Add, m_GridLineTbl[Tv_WkIdx].St.y, 
								m_GridLineTbl[Tv_WkIdx].Ed.x + Tv_Add, m_GridLineTbl[Tv_WkIdx].Ed.y, m_ClrGrid );
							m_GdibScr->DrawLine(	m_GridLineTbl[Tv_WkIdx].St.x - (Tv_Add + 1), m_GridLineTbl[Tv_WkIdx].St.y, 
								m_GridLineTbl[Tv_WkIdx].Ed.x - (Tv_Add + 1), m_GridLineTbl[Tv_WkIdx].Ed.y, m_ClrGrid );
							Tv_Add ++;
						}
					}
					else
					{
						while ( (__s32)m_GridWidth >= Tv_Add )
						{
							m_GdibScr->DrawLine( m_GridLineTbl[Tv_WkIdx].St.x, m_GridLineTbl[Tv_WkIdx].St.y + Tv_Add, 
								m_GridLineTbl[Tv_WkIdx].Ed.x, m_GridLineTbl[Tv_WkIdx].Ed.y + Tv_Add, m_ClrGrid );
							m_GdibScr->DrawLine(	m_GridLineTbl[Tv_WkIdx].St.x, m_GridLineTbl[Tv_WkIdx].St.y - (Tv_Add + 1), 
								m_GridLineTbl[Tv_WkIdx].Ed.x, m_GridLineTbl[Tv_WkIdx].Ed.y - (Tv_Add + 1), m_ClrGrid );
							Tv_Add ++;
						}
					}

				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::LcChItmDrawMode0( __u8 A_Atb, __u32 A_NosigMap, __u32 A_CvtMap )
{
	// local -------------------
		St_GrRect	Tv_RcIcon;
		Ptr_GrGdib	Tv_PtrGdib;
		Cls_GrUiFontObj*	Tv_ObjFont;
		__s32		Tv_FontGap;
		BOOL8	Tv_IsFullPaint;
		St_GrRect	Tv_RcOrg;
		St_GrRect	Tv_RcFull;
		St_GrRect	Tv_RcClip;
		BOOL8	Tv_IsAble;
	// code --------------------
		Tv_IsFullPaint	=	FALSE;
		if ( !m_ObjConsole->IsPlayMode() )
		{
			if ( m_IsBgImgShow )
			{
				//nosignal
				if ( 0 != ( m_PntChMask & A_NosigMap) )
				{
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadImgNosig].Id );
					if ( NULL != Tv_PtrGdib )
					{
						m_ObjConsole->QuadRectGet( m_PntViewIdx, &Tv_RcFull );
						if ( m_IsChItmPntReq )
						{
							m_ObjConsole->PaintReq( &Tv_RcFull );
							Tv_IsFullPaint	=	TRUE;
						}
						else
						{
							m_GdibScr->ClipperGet( &Tv_RcOrg );
							Tv_IsAble	=	GrDrawCnGetComRect( &Tv_RcClip,	&m_PntRcClip, &Tv_RcFull );
							if ( Tv_IsAble )
							{
								m_GdibScr->ClipperSet( &Tv_RcClip );
								SkinDrawByPos( m_IconTbl[E_GrUiDoQuadImgNosig].Atb, Tv_PtrGdib, &Tv_RcFull );
								m_GdibScr->ClipperSet( &Tv_RcOrg );
							}
						}
					}
				}
				//covert
				if ( !Tv_IsFullPaint )
				{
					if ( 0 != ( m_PntChMask & A_CvtMap) )
					{
						Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadImgCovert].Id );
						if ( NULL != Tv_PtrGdib )
						{
							m_ObjConsole->QuadRectGet( m_PntViewIdx, &Tv_RcFull );
							if ( m_IsChItmPntReq )
							{
								m_ObjConsole->PaintReq( &Tv_RcFull );
								Tv_IsFullPaint	=	TRUE;
							}
							else
							{
								m_GdibScr->ClipperGet( &Tv_RcOrg );
								Tv_IsAble	=	GrDrawCnGetComRect( &Tv_RcClip,	&m_PntRcClip, &Tv_RcFull );
								if ( Tv_IsAble )
								{
									m_GdibScr->ClipperSet( &Tv_RcClip );
									SkinDrawByPos( m_IconTbl[E_GrUiDoQuadImgCovert].Atb, Tv_PtrGdib, &Tv_RcFull );
									m_GdibScr->ClipperSet( &Tv_RcOrg );
									m_RcFull	=	Tv_RcFull;
									m_IsChg		=	TRUE;
								}
							}
						}
					}
				}
			}
		}

		//check paint able
		if ( !Tv_IsFullPaint )
		{
			//draw text
			if ( (!m_IsChItmPntReq) && m_IsTitleShow )
			{
				TextDraw( m_StrTitleTbl[m_PntViewCh], &m_TextPara, m_ClrTitle, &m_PntViewRc );
			}

			//icon draw
			if ( m_IsIconShow )
			{
				//draw record icon
				if ( 0 != ( E_GrDvrRecAtbRecord & A_Atb) )
				{
					Tv_FontGap	=	0;
					Tv_ObjFont	=	V_GrUiMngObj->FontGet( m_TextPara.FontId );
					if ( NULL != Tv_ObjFont )
					{
						Tv_FontGap	=	(__s32)Tv_ObjFont->GetHeight();
					}
					Tv_RcIcon.left		=	m_PntViewRc.right - (__s32)m_IconSizeX;
					Tv_RcIcon.top			=	m_PntViewRc.top + Tv_FontGap;
					Tv_RcIcon.right		=	m_PntViewRc.right;	
					Tv_RcIcon.bottom	=	Tv_RcIcon.top + (__s32)m_IconSizeY;	
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadIconRecord].Id );
					if ( NULL != Tv_PtrGdib )
					{
						if ( m_IsChItmPntReq )
						{
							m_ObjConsole->PaintReq( &Tv_RcIcon );
						}
						else
						{
							SkinDrawByPos( m_IconTbl[E_GrUiDoQuadIconRecord].Atb, Tv_PtrGdib, &Tv_RcIcon );
						}
					}
				}

				//motion icon
				Tv_RcIcon.left		=	m_PntViewRc.left;
				Tv_RcIcon.top			=	m_PntViewRc.top + (__s32)m_IconGapTop;
				Tv_RcIcon.right		=	m_PntViewRc.left + (__s32)m_IconSizeX;
				Tv_RcIcon.bottom	=	Tv_RcIcon.top + (__s32)m_IconSizeY;
				if ( 0 != ( E_GrDvrRecAtbMotion & A_Atb) )
				{
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadIconMotion].Id );
					if ( NULL != Tv_PtrGdib )
					{
						if ( m_IsChItmPntReq )
						{
							m_ObjConsole->PaintReq( &Tv_RcIcon );
						}
						else
						{
							SkinDrawByPos( m_IconTbl[E_GrUiDoQuadIconMotion].Atb, Tv_PtrGdib, &Tv_RcIcon );
						}
					}
				}

				//sensor icon
				Tv_RcIcon.top			=	Tv_RcIcon.bottom + (__s32)m_IconGapBtw;
				Tv_RcIcon.bottom	=	Tv_RcIcon.top + (__s32)m_IconSizeY;
				if ( 0 != ( E_GrDvrRecAtbSensor & A_Atb) )
				{
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadIconSensor].Id );
					if ( NULL != Tv_PtrGdib )
					{
						if ( m_IsChItmPntReq )
						{
							m_ObjConsole->PaintReq( &Tv_RcIcon );
						}
						else
						{
							SkinDrawByPos( m_IconTbl[E_GrUiDoQuadIconSensor].Atb, Tv_PtrGdib, &Tv_RcIcon );
						}
					}
				}

				if ( !m_ObjConsole->IsPlayMode() )
				{
					//nosignal
					Tv_RcIcon.top			=	Tv_RcIcon.bottom + (__s32)m_IconGapBtw;
					Tv_RcIcon.bottom	=	Tv_RcIcon.top + (__s32)m_IconSizeY;
					if ( 0 != ( m_PntChMask & A_NosigMap) )
					{
						Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadIconNosig].Id );
						if ( NULL != Tv_PtrGdib )
						{
							if ( m_IsChItmPntReq )
							{
								m_ObjConsole->PaintReq( &Tv_RcIcon );
							}
							else
							{
								SkinDrawByPos( m_IconTbl[E_GrUiDoQuadIconNosig].Atb, Tv_PtrGdib, &Tv_RcIcon );
							}
						}
					}
					//covert
					Tv_RcIcon.top			=	Tv_RcIcon.bottom + (__s32)m_IconGapBtw;
					Tv_RcIcon.bottom	=	Tv_RcIcon.top + (__s32)m_IconSizeY;
					if ( (0 != ( m_PntChMask & A_CvtMap)) && (!m_ObjConsole->IsPlayMode()) )
					{
						Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadIconCovert].Id );
						if ( NULL != Tv_PtrGdib )
						{
							if ( m_IsChItmPntReq )
							{
								m_ObjConsole->PaintReq( &Tv_RcIcon );
							}
							else
							{
								SkinDrawByPos( m_IconTbl[E_GrUiDoQuadIconCovert].Atb, Tv_PtrGdib, &Tv_RcIcon );
							}
						}
					}
				}		//if ( !m_ObjConsole->IsPlayMode() )

				//image process
				Tv_RcIcon.top			=	Tv_RcIcon.bottom + (__s32)m_IconGapBtw;
				Tv_RcIcon.bottom	=	Tv_RcIcon.top + (__s32)m_IconSizeY;
				if ( 0 != ( E_GrDvrRecAtbExtra & A_Atb) )
				{
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadIconImage].Id );
					if ( NULL != Tv_PtrGdib )
					{
						if ( m_IsChItmPntReq )
						{
							m_ObjConsole->PaintReq( &Tv_RcIcon );
						}
						else
						{
							SkinDrawByPos( m_IconTbl[E_GrUiDoQuadIconImage].Atb, Tv_PtrGdib, &Tv_RcIcon );
						}
					}
				}

			}	//icon show
			//extra title
			if ( m_IsExtTtShow )
			{
				if ( !m_IsChItmPntReq )
				{
					TextDraw( m_StrExtTtTbl[m_PntViewCh], &m_ExtTxtPara, m_ExtTtClr[m_PntViewCh], &m_PntViewRc );
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::LcChItmDrawMode1( __u8 A_Atb, __u32 A_NosigMap, __u32 A_CvtMap )
{
	// local -------------------
		St_GrRect	Tv_RcIcon;
		Ptr_GrGdib	Tv_PtrGdib;
		BOOL8	Tv_IsFullPaint;
		St_GrRect	Tv_RcOrg;
		St_GrRect	Tv_RcFull;
		St_GrRect	Tv_RcClip;
		BOOL8	Tv_IsAble;
	// code --------------------
		Tv_IsFullPaint	=	FALSE;

		if ( !m_ObjConsole->IsPlayMode() )
		{
			if ( m_IsBgImgShow )
			{
				//nosignal
				if ( 0 != ( m_PntChMask & A_NosigMap) )
				{
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadImgNosig].Id );
					if ( NULL != Tv_PtrGdib )
					{
						m_ObjConsole->QuadRectGet( m_PntViewIdx, &Tv_RcFull );
						if ( m_IsChItmPntReq )
						{
							m_ObjConsole->PaintReq( &Tv_RcFull );
							Tv_IsFullPaint	=	TRUE;
						}
						else
						{
							m_GdibScr->ClipperGet( &Tv_RcOrg );
							Tv_IsAble	=	GrDrawCnGetComRect( &Tv_RcClip,	&m_PntRcClip, &Tv_RcFull );
							if ( Tv_IsAble )
							{
								m_GdibScr->ClipperSet( &Tv_RcClip );
								SkinDrawByPos( m_IconTbl[E_GrUiDoQuadImgNosig].Atb, Tv_PtrGdib, &Tv_RcFull );
								m_GdibScr->ClipperSet( &Tv_RcOrg );
							}
						}
					}
				}
				//covert
				if ( (!Tv_IsFullPaint) && (!m_ObjConsole->IsPlayMode()) )
				{
					if ( 0 != ( m_PntChMask & A_CvtMap) )
					{
						Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadImgCovert].Id );
						if ( NULL != Tv_PtrGdib )
						{
							m_ObjConsole->QuadRectGet( m_PntViewIdx, &Tv_RcFull );
							if ( m_IsChItmPntReq )
							{
								m_ObjConsole->PaintReq( &Tv_RcFull );
								Tv_IsFullPaint	=	TRUE;
							}
							else
							{
								m_GdibScr->ClipperGet( &Tv_RcOrg );
								Tv_IsAble	=	GrDrawCnGetComRect( &Tv_RcClip,	&m_PntRcClip, &Tv_RcFull );
								if ( Tv_IsAble )
								{
									m_GdibScr->ClipperSet( &Tv_RcClip );
									SkinDrawByPos( m_IconTbl[E_GrUiDoQuadImgCovert].Atb, Tv_PtrGdib, &Tv_RcFull );
									m_GdibScr->ClipperSet( &Tv_RcOrg );
								}
							}
						}
					}
				}
			}
		}

		//check paint able
		if ( !Tv_IsFullPaint )
		{
			//draw text
			if ( (!m_IsChItmPntReq) && m_IsTitleShow )
			{
				TextDraw( m_StrTitleTbl[m_PntViewCh], &m_TextPara, m_ClrTitle, &m_PntViewRc );
			}

			if ( m_IsIconShow )
			{
				//draw record icon
				Tv_RcIcon.left		=	m_PntViewRc.right - (__s32)m_IconSizeX;
				Tv_RcIcon.top			=	m_PntViewRc.top + (__s32)m_IconGapTop;
				Tv_RcIcon.right		=	m_PntViewRc.right;
				Tv_RcIcon.bottom	=	Tv_RcIcon.top + (__s32)m_IconSizeY;
				if ( 0 != ( E_GrDvrRecAtbRecord & A_Atb) )
				{
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadIconRecord].Id );
					if ( NULL != Tv_PtrGdib )
					{
						if ( m_IsChItmPntReq )
						{
							m_ObjConsole->PaintReq( &Tv_RcIcon );
						}
						else
						{
							SkinDrawByPos( m_IconTbl[E_GrUiDoQuadIconRecord].Atb, Tv_PtrGdib, &Tv_RcIcon );
						}
					}
				}

				//covert
				Tv_RcIcon.right		=	Tv_RcIcon.left - (__s32)m_IconGapBtw;
				Tv_RcIcon.left		=	Tv_RcIcon.right - (__s32)m_IconSizeX;
				if ( (0 != ( m_PntChMask & A_CvtMap)) && (!m_ObjConsole->IsPlayMode()) )
				{
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadIconCovert].Id );
					if ( NULL != Tv_PtrGdib )
					{
						if ( m_IsChItmPntReq )
						{
							m_ObjConsole->PaintReq( &Tv_RcIcon );
						}
						else
						{
							SkinDrawByPos( m_IconTbl[E_GrUiDoQuadIconCovert].Atb, Tv_PtrGdib, &Tv_RcIcon );
						}
					}
				}

				//nosignal
				Tv_RcIcon.right		=	Tv_RcIcon.left - (__s32)m_IconGapBtw;
				Tv_RcIcon.left		=	Tv_RcIcon.right - (__s32)m_IconSizeX;
				if ( (0 != ( m_PntChMask & A_NosigMap)) && (!m_ObjConsole->IsPlayMode()) )
				{
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadIconNosig].Id );
					if ( NULL != Tv_PtrGdib )
					{
						if ( m_IsChItmPntReq )
						{
							m_ObjConsole->PaintReq( &Tv_RcIcon );
						}
						else
						{
							SkinDrawByPos( m_IconTbl[E_GrUiDoQuadIconNosig].Atb, Tv_PtrGdib, &Tv_RcIcon );
						}
					}
				}

				//sensor icon
				Tv_RcIcon.right		=	Tv_RcIcon.left - (__s32)m_IconGapBtw;
				Tv_RcIcon.left		=	Tv_RcIcon.right - (__s32)m_IconSizeX;
				if ( 0 != ( E_GrDvrRecAtbSensor & A_Atb) )
				{
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadIconSensor].Id );
					if ( NULL != Tv_PtrGdib )
					{
						if ( m_IsChItmPntReq )
						{
							m_ObjConsole->PaintReq( &Tv_RcIcon );
						}
						else
						{
							SkinDrawByPos( m_IconTbl[E_GrUiDoQuadIconSensor].Atb, Tv_PtrGdib, &Tv_RcIcon );
						}
					}
				}

				//motion icon
				Tv_RcIcon.right		=	Tv_RcIcon.left - (__s32)m_IconGapBtw;
				Tv_RcIcon.left		=	Tv_RcIcon.right - (__s32)m_IconSizeX;
				if ( 0 != ( E_GrDvrRecAtbMotion & A_Atb) )
				{
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadIconMotion].Id );
					if ( NULL != Tv_PtrGdib )
					{
						if ( m_IsChItmPntReq )
						{
							m_ObjConsole->PaintReq( &Tv_RcIcon );
						}
						else
						{
							SkinDrawByPos( m_IconTbl[E_GrUiDoQuadIconMotion].Atb, Tv_PtrGdib, &Tv_RcIcon );
						}
					}
				}

				//image process
				Tv_RcIcon.right		=	Tv_RcIcon.left - (__s32)m_IconGapBtw;
				Tv_RcIcon.left		=	Tv_RcIcon.right - (__s32)m_IconSizeX;
				if ( 0 != ( E_GrDvrRecAtbExtra & A_Atb) )
				{
					Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_IconTbl[E_GrUiDoQuadIconImage].Id );
					if ( NULL != Tv_PtrGdib )
					{
						if ( m_IsChItmPntReq )
						{
							m_ObjConsole->PaintReq( &Tv_RcIcon );
						}
						else
						{
							SkinDrawByPos( m_IconTbl[E_GrUiDoQuadIconImage].Atb, Tv_PtrGdib, &Tv_RcIcon );
						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::ProcBgSkinTblDraw( void )
{
	// local -------------------
		__u8		Tv_Page;
	// code --------------------
		Cls_GrUiDoSkin::ProcBgSkinTblDraw();

		m_IsChItmPntReq	=	FALSE;

		if ( 0 != m_ViewItemCnt )
		{
			Tv_Page	=	m_ObjConsole->QuadPageGet();
			for ( m_PntViewIdx=0;m_PntViewIdx < m_ViewItemCnt;m_PntViewIdx++ )
			{
				if ( m_ObjConsole->QuadRectGet( m_PntViewIdx, &m_PntViewRc ) )
				{
					//get channel number
					m_PntViewCh	=	m_ObjConsole->QuadGetChByView( m_PntViewIdx );
					m_PntChMask	=	1 << m_PntViewCh;
					//patch rect
					if ( m_PntViewRc.left < m_PntRcClient.left )
					{
						m_PntViewRc.left	=	m_PntRcClient.left;
					}
					if ( m_PntViewRc.top < m_PntRcClient.top )
					{
						m_PntViewRc.top		=	m_PntRcClient.top;
					}
					if ( m_PntViewRc.right > m_PntRcClient.right )
					{
						m_PntViewRc.right	=	 m_PntRcClient.right;
					}
					if ( m_PntViewRc.bottom > m_PntRcClient.bottom )
					{
						m_PntViewRc.bottom	=	m_PntRcClient.bottom;
					}
					//patch draw gap
					m_PntViewRc.left		=	m_PntViewRc.left + (__s32)m_ChDrawGap;
					m_PntViewRc.top			=	m_PntViewRc.top + (__s32)m_ChDrawGap;
					m_PntViewRc.right		=	m_PntViewRc.right - (__s32)m_ChDrawGap;
					m_PntViewRc.bottom	=	m_PntViewRc.bottom - (__s32)m_ChDrawGap;
					//check able rect
					if ( (m_PntViewRc.left < m_PntViewRc.right) && (m_PntViewRc.top < m_PntViewRc.bottom) )
					{
						if ( PaintClipperSet( TRUE, &m_PntViewRc ) )
						{
							//draw view item
							switch( m_PosMode )
							{
								case	1:
#ifdef GR_OSD_EDIT
									LcChItmDrawMode1( 0xFF, 0xFFFF, 0xFFFF );
#else
									LcChItmDrawMode1( m_PntRecAtb[m_PntViewCh], m_PntNsgMap, m_PntCvtMap );
#endif
									break;
								default:
#ifdef GR_OSD_EDIT
									LcChItmDrawMode0( 0xFF, 0xFFFF, 0xFFFF );
#else
									LcChItmDrawMode0( m_PntRecAtb[m_PntViewCh], m_PntNsgMap, m_PntCvtMap );
#endif
									break;
							}
						}
					}
				}
			}
		}
		LcPaintGridLine();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::ProcEvtGlbStatChg( __u8 A_Type )
{
	// local -------------------
	// code --------------------
		switch( A_Type )
		{
			case	E_GrUiChgQuad:
			case	E_GrUiChgOsdExp:
				//change quad mode
				LcQuadParaUpdate();
				break;
		}
		
		//inherited
		Cls_GrUiDoSkin::ProcEvtGlbStatChg( A_Type );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::LcQuadParaUpdate( void )
{
	// local -------------------
	// code --------------------
		m_ViewItemCnt		=	m_ObjConsole->QuadViewCntGet();
		m_GridLineCnt		=	m_ObjConsole->QuadLineTblGet( m_GridLineTbl );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		__u16		Tv_NsgMap;
		__u16		Tv_CvtMap;
		__u16		Tv_ChgNsg;
		__u16		Tv_ChgCvt;
		__u16		Tv_ChUseMap;
		__u8		Tv_RecAtb;
		__u8		Tv_RecChg;
	// code --------------------
		//update para
		m_IsChItmPntReq	=	TRUE;
		Tv_ChUseMap	=	m_ObjConsole->ChUseMapGet();
		Tv_NsgMap	=	m_ObjConsole->NoSigMapGet() & Tv_ChUseMap;
		Tv_CvtMap	=	m_ObjConsole->CorvertMapGet() | (m_ObjConsole->NoSigMapGet() & ~Tv_ChUseMap);
		Tv_ChgNsg	=	m_PntNsgMap ^ Tv_NsgMap;
		Tv_ChgCvt	=	m_PntCvtMap ^ Tv_CvtMap;
		m_PntNsgMap	=	Tv_NsgMap;
		m_PntCvtMap	=	Tv_CvtMap;
		
		// display
		if ( 0 != m_ViewItemCnt )
		{
			if ( ClientRectGet( &m_PntRcClient ) )
			{
				for ( m_PntViewIdx=0;m_PntViewIdx < m_ViewItemCnt;m_PntViewIdx++ )
				{
					//check channel change
					m_PntViewCh	=	m_ObjConsole->QuadGetChByView( m_PntViewIdx );
					m_PntChMask	=	1 << m_PntViewCh;

					if ( m_ObjConsole->QuadRectGet( m_PntViewIdx, &m_PntViewRc ) )
					{
						//patch rect
						if ( m_PntViewRc.left < m_PntRcClient.left )
						{
							m_PntViewRc.left	=	m_PntRcClient.left;
						}
						if ( m_PntViewRc.top < m_PntRcClient.top )
						{
							m_PntViewRc.top		=	m_PntRcClient.top;
						}
						if ( m_PntViewRc.right > m_PntRcClient.right )
						{
							m_PntViewRc.right	=	 m_PntRcClient.right;
						}
						if ( m_PntViewRc.bottom > m_PntRcClient.bottom )
						{
							m_PntViewRc.bottom	=	m_PntRcClient.bottom;
						}
						//patch draw gap
						m_PntViewRc.left		=	m_PntViewRc.left + (__s32)m_ChDrawGap;
						m_PntViewRc.top			=	m_PntViewRc.top + (__s32)m_ChDrawGap;
						m_PntViewRc.right		=	m_PntViewRc.right - (__s32)m_ChDrawGap;
						m_PntViewRc.bottom	=	m_PntViewRc.bottom - (__s32)m_ChDrawGap;
						//check able rect
						if ( (m_PntViewRc.left < m_PntViewRc.right) && (m_PntViewRc.top < m_PntViewRc.bottom) )
						{
							//get record attribute
							if ( m_ObjConsole->IsPlayMode() )
							{
								Tv_RecAtb	=	m_ObjConsole->PlayAtbGet( m_PntViewCh );
							}
							else
							{
								Tv_RecAtb	=	(__u8)(0xFF & m_ObjConsole->RecAtbGet( m_PntViewCh ) );
								if ( m_IsRecIconLock)
								{
									Tv_RecAtb	=	Tv_RecAtb & (~E_GrDvrRecAtbRecord);
								}
								if (0 == (m_PntChMask & Tv_ChUseMap))
								{
									Tv_RecAtb	=	Tv_RecAtb & (~(E_GrDvrRecAtbRecord | E_GrDvrRecAtbMotion | E_GrDvrRecAtbSensor));
								}
							}
							Tv_RecChg	=	Tv_RecAtb ^ m_PntRecAtb[m_PntViewCh];
							m_PntRecAtb[m_PntViewCh]	=	Tv_RecAtb;

							switch( m_PosMode )
							{
								case	1:
									LcChItmDrawMode1( Tv_RecChg, Tv_ChgNsg, Tv_ChgCvt );
									break;
								default:
									LcChItmDrawMode0( Tv_RecChg, Tv_ChgNsg, Tv_ChgCvt );
									break;
							}
						}
					}
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::TextParaSet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( &m_TextPara, A_PtrPara, sizeof(St_GrUiDoTextPara) );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::TextParaGet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( A_PtrPara, &m_TextPara, sizeof(St_GrUiDoTextPara) );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::ExtTxtParaSet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( &m_ExtTxtPara, A_PtrPara, sizeof(St_GrUiDoTextPara) );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::ExtTxtParaGet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( A_PtrPara, &m_ExtTxtPara, sizeof(St_GrUiDoTextPara) );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::TitleGet( __u8 A_Ch, WCHAR* A_StrRt, __u32 A_LenMax )
{
		GrStrWcopy( A_StrRt, m_StrTitleTbl[A_Ch], A_LenMax );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::TitleSet( __u8 A_Ch, WCHAR* A_Str )
{
		GrStrWcopy( m_StrTitleTbl[A_Ch], A_Str );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::ExtTitleGet( __u8 A_Ch, WCHAR* A_StrRt, __u32 A_LenMax )
{
		GrStrWcopy( A_StrRt, m_StrExtTtTbl[A_Ch], A_LenMax );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::ExtTitleSet( __u8 A_Ch, WCHAR* A_Str )
{
		GrStrWcopy( m_StrExtTtTbl[A_Ch], A_Str );
		ReqDraw();
}
//--------------------------------------------------------------------
void		Cls_GrUiDoQuad::TitleColorSet( __u32 A_Color )
{
		m_ClrTitle	=	A_Color;
		ReqDraw();
}
//--------------------------------------------------------------------
__u32		Cls_GrUiDoQuad::TitleColorGet( void )
{
		return	m_ClrTitle;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoQuad::ExtTitleColorGet( __u8 A_Ch )
{
		return	m_ExtTtClr[A_Ch];
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::ExtTitleColorSet(  __u8 A_Ch, __u32 A_Color )
{
		m_ExtTtClr[A_Ch]	=	A_Color;
		ReqDraw();
}
//--------------------------------------------------------------------
void		Cls_GrUiDoQuad::GridColorSet( __u32 A_Color )
{
		m_ClrGrid	=	A_Color;
		ReqDraw();
}
//--------------------------------------------------------------------
__u32		Cls_GrUiDoQuad::GridColorGet( void )
{
		return	m_ClrGrid;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrUiDoQuad::IsGridShow( void )
{
		return	m_IsGridShow;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoQuad::GridShowSet( BOOL8 A_IsShow )
{
		m_IsGridShow	=	A_IsShow;
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8		Cls_GrUiDoQuad::IsTitleShow( void )
{
		return	m_IsTitleShow;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoQuad::TitleShowSet( BOOL8 A_IsShow )
{
		m_IsTitleShow	=	A_IsShow;
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoQuad::IsExtTitleShow( void )
{
		return	m_IsExtTtShow;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::ExtTitleShowSet( BOOL8 A_IsShow )
{
		m_IsExtTtShow	=	A_IsShow;
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8		Cls_GrUiDoQuad::IsIconShow( void )
{
		return	m_IsIconShow;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoQuad::IconShowSet( BOOL8 A_IsShow )
{
		m_IsIconShow	=	A_IsShow;
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8		Cls_GrUiDoQuad::IsBgImgShow( void )
{
		return	m_IsBgImgShow;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoQuad::BgImgShowSet( BOOL8 A_IsShow )
{
		m_IsBgImgShow	=	A_IsShow;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8		Cls_GrUiDoQuad::ChMarginGet( void )
{
		return	m_ChDrawGap;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoQuad::ChMarginSet( __u8 A_Val )
{
		m_ChDrawGap	=	A_Val;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8		Cls_GrUiDoQuad::IconXsizeGet( void )
{
		return	m_IconSizeX;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoQuad::IconXsizeSet( __u8 A_Val )
{
		m_IconSizeX	=	A_Val;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8		Cls_GrUiDoQuad::IconYsizeGet( void )
{
		return	m_IconSizeY;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoQuad::IconYsizeSet( __u8 A_Val )
{
		m_IconSizeY	=	A_Val;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8		Cls_GrUiDoQuad::IconGapTopGet( void )
{
		return	m_IconGapTop;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoQuad::IconGapTopSet( __u8 A_Val )
{
		m_IconGapTop	=	A_Val;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8		Cls_GrUiDoQuad::IconGapBtwGet( void )
{
		return	m_IconGapBtw;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoQuad::IconGapBtwSet( __u8 A_Val )
{
		m_IconGapBtw	=	A_Val;
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoQuad::ProcEvtKeyDown( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;

		switch( A_Key )
		{
			case	E_GrUiKeySplit:
				LcNotifyQuadChg();
				m_ObjConsole->QuadModeNext();
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::IconImgGet( __u32 A_Idx, Ptr_GrUiDoSkinImg A_PtrRtSkin )
{
		if ( E_GrUiDoQuadIconCnt > A_Idx )
		{
			A_PtrRtSkin->Id		=	m_IconTbl[A_Idx].Id;
			A_PtrRtSkin->Atb	=	m_IconTbl[A_Idx].Atb;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::IconImgSet( __u32 A_Idx, Ptr_GrUiDoSkinImg A_PtrSkin )
{
		if ( E_GrUiDoQuadIconCnt > A_Idx )
		{
			m_IconTbl[A_Idx].Id		=	A_PtrSkin->Id;
			m_IconTbl[A_Idx].Atb	=	A_PtrSkin->Atb;
			//reset chage attribute
			GrDumyZeroMem( m_PntRecAtb, sizeof(m_PntRecAtb) );
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoQuad::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__u8		Tv_QdNow;
		__u8		Tv_PgNow;
		__u8		Tv_QdReq;
		__u8		Tv_PgReq;
	// code --------------------
		//inherited
		Cls_GrUiDoSkin::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		
		//check now mode
		Tv_QdReq		=	E_GrDvrQuadMode1;
		Tv_QdNow		=	m_ObjConsole->QuadModeGet();
		Tv_PgNow		=	m_ObjConsole->QuadPageGet();
		if ( E_GrDvrQuadMode1 != Tv_QdNow )
		{
			Tv_QdReq	=	E_GrDvrQuadMode1;
			Tv_PgReq	=	m_ObjConsole->QuadChByPos( A_X, A_Y );
			m_ObjConsole->QuadModeSet( Tv_QdReq, Tv_PgReq );
		}
		else
		{
			if ( E_GrDvrQuadMode1 != m_QuadPrvMode )
			{
				m_ObjConsole->QuadModeSet( m_QuadPrvMode, m_QuadPrvPage );
			}
			else
			{
				m_ObjConsole->QuadMaxModeSet();
			}
		}
		//update previous mode
		m_QuadPrvMode	=	Tv_QdNow;
		m_QuadPrvPage	=	Tv_PgNow;

		LcNotifyQuadChg();

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::ProcEvtMseEnter( void )
{
		//not work
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::ProcEvtMseLeave( void )
{
		//not work
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoQuad::PosModeGet( void )
{
		return	m_PosMode;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::PosModeSet( __u8 A_Mode )
{
		if ( E_GrUiDoQuadPosModeCnt > A_Mode )
		{
			m_PosMode	=	A_Mode;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::IconRecLock( BOOL8 A_IsLock )
{
		m_IsRecIconLock	=	A_IsLock;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::LcNotifyQuadChg( void )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		if ( V_GrUiMngObj->IsUserEventOn() )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdQuadChg;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;
			Tv_Msg.Obj				=	(Cls_GrUiDoBase*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoQuad::GridWidthSet( __u8 A_Width )
{
		m_GridWidth	=	A_Width;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoQuad::GridWidthGet( void )
{
		return	m_GridWidth;
}
//--------------------------------------------------------------------


