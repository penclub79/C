/*
 spin number button

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoSmpBtn.h>
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
Cls_GrUiDoSmpBtn::Cls_GrUiDoSmpBtn( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
	// local -------------------
		__u32		Tv_ClrWhite;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		m_IsFocusAble		=	TRUE;			//focusable control
		GrDumyZeroMem( &m_TxtPara, sizeof(m_TxtPara) );
		m_TxtPara.ClrShadow	=	m_ObjConsole->ClrBlackGet();
		m_EvtHostId			=	E_GrUiObjIdNone;

		Tv_ClrWhite		=	m_ObjConsole->ClrWhiteGet();
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoSmpbImgCnt;Tv_WkIdx++ )
		{
			m_SmpbSkinTbl[Tv_WkIdx].Atb	=	0;
			m_SmpbSkinTbl[Tv_WkIdx].Id	=	E_GrUiImgIdNone;
			m_SmpbClrTbl[Tv_WkIdx]			=	0;
			m_SmpbTxtClrTbl[Tv_WkIdx]		=	Tv_ClrWhite;
		}

}
//--------------------------------------------------------------------
Cls_GrUiDoSmpBtn::~Cls_GrUiDoSmpBtn()
{
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoSmpBtn::LcEditGetSkinIdx( void )
{
	// local -------------------
		__u8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	E_GrUiDoSmpbImgIdxDisable;
		//check disable
		if ( IsCanEnable()	)
		{
			//normal mode
			Tv_Result		=	E_GrUiDoSmpbImgIdxNoraml;	
			if ( IsFocused() || IsOnMouse() )
			{
				//check focused
				Tv_Result		=	E_GrUiDoSmpbImgIdxNrmFcs;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSmpBtn::ProcBgColorDraw( void )
{
	// local -------------------
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		//check draw background
		m_SmpbStatIdx	=	LcEditGetSkinIdx();
		//select background color
		m_ClrBg		=	m_SmpbClrTbl[m_SmpbStatIdx];
		//draw background color
		Cls_GrUiDoSkin::ProcBgColorDraw();
		//set clipper
		if ( PaintClipperSet( TRUE ) )
		{
			//edit status skin draw
			Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_SmpbSkinTbl[m_SmpbStatIdx].Id );
			if ( NULL != Tv_PtrGdib )
			{
				//blt
				SkinDrawByPos( m_SmpbSkinTbl[m_SmpbStatIdx].Atb, Tv_PtrGdib, &m_PntRcClient );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSmpBtn::TextParaSet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( &m_TxtPara, A_PtrPara, sizeof(m_TxtPara) );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSmpBtn::TextParaGet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( A_PtrPara, &m_TxtPara, sizeof(m_TxtPara) );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSmpBtn::StatSkinGet( __u8 A_Idx, Ptr_GrUiDoSkinImg A_PtrRtSkin )
{
		//init
		A_PtrRtSkin->Id			=	E_GrUiImgIdNone;
		A_PtrRtSkin->Atb		=	0;
		//check able image
		if ( E_GrUiDoSmpbImgCnt > A_Idx )
		{
			*A_PtrRtSkin	=	m_SmpbSkinTbl[A_Idx];
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSmpBtn::StatSkinSet( __u8 A_Idx, Ptr_GrUiDoSkinImg A_PtrSkin )
{
		//check able image
		if ( E_GrUiDoSmpbImgCnt > A_Idx )
		{
			m_SmpbSkinTbl[A_Idx]		=	*A_PtrSkin;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSmpBtn::StatClrGet( __u8 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result		=	0;
		if ( E_GrUiDoSmpbImgCnt > A_Idx )
		{
			Tv_Result	=	m_SmpbClrTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSmpBtn::StatClrSet( __u8 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoSmpbImgCnt > A_Idx )
		{
			m_SmpbClrTbl[A_Idx]	=	A_Color;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSmpBtn::TxtClrGet( __u8 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result		=	0;
		if ( E_GrUiDoSmpbImgCnt > A_Idx )
		{
			Tv_Result	=	m_SmpbTxtClrTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSmpBtn::TxtClrSet( __u8 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoSmpbImgCnt > A_Idx )
		{
			m_SmpbTxtClrTbl[A_Idx]	=	A_Color;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSmpBtn::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		Cls_GrUiDoSkin::ProcEvtMseLbtnDown( A_X, A_Y, A_BtnMap );
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSmpBtn::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		Cls_GrUiDoSkin::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		return	TRUE;
}
//--------------------------------------------------------------------

