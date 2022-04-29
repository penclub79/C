/*
 progress bar

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoPrgsbar.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>
#include	<Ui/GrUiDoLbl.h>

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
Cls_GrUiDoPrgsbar::Cls_GrUiDoPrgsbar( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
		//init
		m_TypeId			=	E_GrUiDoTypeIdPrgsbar;

		m_ClrPrgs			=	0;
		m_ValMax			=	E_GrUiDoPrgsbarDfltMaxVal;
		m_IsVertical	=	FALSE;
		m_IsRelTypeCnt	=	FALSE;		//relate value type count
		m_RltLblId		=	E_GrUiObjIdNone;

		m_ImgPrgs.Id	=	E_GrUiImgIdNone;
		m_ImgPrgs.Atb	=	0;

#ifdef GR_OSD_EDIT 
		m_ValNow			=	(E_GrUiDoPrgsbarDfltMaxVal >> 1);
#else
		m_ValNow			=	0;
#endif
		m_LastBaseSize	=	0;
		m_LastValue			=	0;
		m_LastMax				=	0;
		m_PntPrgsSize		=	0;

}
//--------------------------------------------------------------------
Cls_GrUiDoPrgsbar::~Cls_GrUiDoPrgsbar()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoPrgsbar::ProcBgSkinTblDraw( void )
{
	// local -------------------
		__u32	Tv_BaseSize;
		St_GrRect	Tv_Rc;
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		Cls_GrUiDoSkin::ProcBgSkinTblDraw();
		//draw text
		if ( PaintClipperSet( TRUE ) )
		{
			Tv_BaseSize	=	(__u32)(m_PntRcClient.right - m_PntRcClient.left);
			if ( m_IsVertical )
			{
				Tv_BaseSize	=	(__u32)(m_PntRcClient.bottom - m_PntRcClient.top);
			}
			if ( (Tv_BaseSize != m_LastBaseSize) || (m_ValNow != m_LastValue) || (m_ValMax != m_LastMax) )
			{
				//recompute
				m_LastBaseSize	=	Tv_BaseSize;
				m_LastValue			=	m_ValNow;
				m_LastMax				=	m_ValMax;
				m_PntPrgsSize		=	(m_LastValue * m_LastBaseSize) / m_LastMax;
			}
			Tv_Rc.left		=	m_PntRcClient.left;
			Tv_Rc.bottom	=	m_PntRcClient.bottom;
			if ( m_IsVertical )
			{
				Tv_Rc.top		=	m_PntRcClient.bottom - (__s32)m_PntPrgsSize;
				Tv_Rc.right	=	m_PntRcClient.right;
			}
			else
			{
				Tv_Rc.top		=	m_PntRcClient.top;
				Tv_Rc.right	=	m_PntRcClient.left + (__s32)m_PntPrgsSize;
			}
			
			if ( E_GrUiImgIdNone != m_ImgPrgs.Id )
			{
				//image fill
				Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_ImgPrgs.Id );
				if ( NULL != Tv_PtrGdib )
				{
					SkinDrawByPos( m_ImgPrgs.Atb, Tv_PtrGdib, &Tv_Rc );
				}
			}
			else
			{
				//color bar
				m_GdibScr->FillRect( &Tv_Rc, m_ClrPrgs );
			}
		}
}
//--------------------------------------------------------------------
__u32		Cls_GrUiDoPrgsbar::BarColorGet( void )
{
		return	m_ClrPrgs;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoPrgsbar::BarColorSet( __u32 A_Color )
{
		m_ClrPrgs	=	A_Color;
		ReqDraw();
}
//--------------------------------------------------------------------
void		Cls_GrUiDoPrgsbar::BarImageGet( Ptr_GrUiDoSkinImg A_PtrRt )
{
		*A_PtrRt	=	m_ImgPrgs;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoPrgsbar::BarImageSet( Ptr_GrUiDoSkinImg A_PtrImg )
{
		m_ImgPrgs	=	*A_PtrImg;
		ReqDraw();
}
//--------------------------------------------------------------------
__u32		Cls_GrUiDoPrgsbar::ValueMaxGet( void )
{
		return	m_ValMax;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoPrgsbar::LcRltLblUpdt( void )
{
	// local -------------------
		Cls_GrUiDoLbl*	Tv_Lbl;
		Def_WstrName	Tv_StrRlt;
		WCHAR*	Tv_StrWk;
		__u32		Tv_Percent;
	// code --------------------
		if ( E_GrUiObjIdNone != m_RltLblId )
		{
			Tv_Lbl	=	(Cls_GrUiDoLbl*)m_ObjWin->FindSlaveObjByObjId( m_RltLblId );
			if ( NULL != Tv_Lbl )
			{
				//check type
				if ( E_GrUiDoTypeIdLabel == Tv_Lbl->TypeIdGet() )
				{
					//generate text
					if ( m_IsRelTypeCnt )
					{
						Tv_StrWk	=	Tv_StrRlt;
						GrStrIntToWstr( Tv_StrWk, (__s32)m_ValNow );
						Tv_StrWk	=	Tv_StrWk + GrStrWlen( Tv_StrWk );
						*Tv_StrWk	=	L' ';		//insert division mark
						Tv_StrWk ++;
						*Tv_StrWk	=	L'/';
						Tv_StrWk ++;
						*Tv_StrWk	=	L' ';
						Tv_StrWk ++;
						GrStrIntToWstr( Tv_StrWk, (__s32)m_ValMax );
					}
					else
					{
						Tv_Percent	=	(100 * m_ValNow) / m_ValMax;
						GrStrIntToWstr( Tv_StrRlt, (__s32)Tv_Percent );
					}
					Tv_Lbl->TextSet( Tv_StrRlt );
				}
			}
		}
}
//--------------------------------------------------------------------
void		Cls_GrUiDoPrgsbar::ValueMaxSet( __u32 A_Max )
{
		if ( 0 != A_Max )
		{
			m_ValMax	=	A_Max;
			ReqDraw();
			//update relate
			LcRltLblUpdt();
		}
}
//--------------------------------------------------------------------
BOOL8		Cls_GrUiDoPrgsbar::IsVerticalType( void )
{
		return	m_IsVertical;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoPrgsbar::VerticalTypeSet( BOOL8 A_IsVert )
{
		m_IsVertical	=	A_IsVert;
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8		Cls_GrUiDoPrgsbar::IsRltLblTypeCnt( void )
{
		return	m_IsRelTypeCnt;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoPrgsbar::RltLblTypeCntSet( BOOL8 A_IsCntType )
{
		m_IsRelTypeCnt	=	A_IsCntType;
		LcRltLblUpdt();
}
//--------------------------------------------------------------------
__u16		Cls_GrUiDoPrgsbar::RltLblIdGet( void )
{
		return	m_RltLblId;
}
//--------------------------------------------------------------------
void		Cls_GrUiDoPrgsbar::RltLblIdSet( __u16 A_ObjId )
{
		m_RltLblId	=	A_ObjId;
		LcRltLblUpdt();
}
//--------------------------------------------------------------------
__u32		Cls_GrUiDoPrgsbar::ValueNowGet( void )
{
		return	m_ValNow;		
}
//--------------------------------------------------------------------
void		Cls_GrUiDoPrgsbar::ValueNowSet( __u32 A_Value )
{
		if ( m_ValMax >= A_Value )
		{
			m_ValNow	=	A_Value;
			ReqDraw();
			LcRltLblUpdt();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoPrgsbar::ProcEvtScptDecoded( void )
{
		Cls_GrUiDoSkin::ProcEvtScptDecoded();
		LcRltLblUpdt();
}
//--------------------------------------------------------------------

