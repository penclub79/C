/*
 label

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoLbl.h>
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
Cls_GrUiDoLbl::Cls_GrUiDoLbl( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
		//init
		m_TypeId			=	E_GrUiDoTypeIdLabel;
		GrStrClear( m_StrText );
		m_TextId			=	E_GrUiTxtIdNone;
		GrDumyZeroMem( &m_TxtPara, sizeof(m_TxtPara) );
		m_TxtPara.ClrShadow	=	m_ObjConsole->ClrBlackGet();
		m_ClrTxt			=	m_ObjConsole->ClrWhiteGet();

		m_IsCusOn			=	FALSE;
		m_CusPos			=	0;
}
//--------------------------------------------------------------------
Cls_GrUiDoLbl::~Cls_GrUiDoLbl()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoLbl::ProcBgSkinTblDraw( void )
{
	// local -------------------
	// code --------------------
		Cls_GrUiDoSkin::ProcBgSkinTblDraw();
		//draw text
		if ( PaintClipperSet( TRUE ) )
		{
			if (E_GrUiTxtIdNone != m_TextId) 
			{
				RosTextDraw( m_TextId, &m_TxtPara, m_ClrTxt, &m_PntRcClient );
			}
			else if ( GrStrIsExist(m_StrText) ) 
			{
				TextDraw( m_StrText, &m_TxtPara, m_ClrTxt, &m_PntRcClient, m_IsCusOn, m_CusPos );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoLbl::TextGet( WCHAR* A_StrRt, __u32 A_LenMax )
{
	// local -------------------
	// code --------------------
		if ( E_GrUiTxtIdNone == m_TextId )
		{
			GrStrWcopy( A_StrRt, m_StrText, A_LenMax );
		}
		else
		{
			V_GrUiMngObj->TextGet( m_TextId, A_StrRt, A_LenMax );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoLbl::TextSet( WCHAR* A_Str )
{
		if ( E_GrUiTxtIdNone == m_TextId )
		{
			GrStrWcopy( m_StrText, A_Str, GrStrMaxLen(m_StrText) );
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoLbl::TextIdGet( void )
{
	// local -------------------
	// code --------------------
		return	m_TextId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoLbl::TextIdSet( __u32 A_Id )
{
	// local -------------------
	// code --------------------
		if ( m_TextId != A_Id )
		{
			m_TextId	=	A_Id;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoLbl::TextColorSet( __u32 A_Color )
{
		if ( m_ClrTxt != A_Color )
		{
			m_ClrTxt	=	A_Color;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoLbl::TextColorGet( void )
{
		return	m_ClrTxt;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoLbl::TextParaSet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( &m_TxtPara, A_PtrPara, sizeof(St_GrUiDoTextPara) );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoLbl::TextParaGet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( A_PtrPara, &m_TxtPara, sizeof(St_GrUiDoTextPara) );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoLbl::CusOn( BOOL8 A_IsOn )
{
		m_IsCusOn		=	A_IsOn;
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoLbl::IsCusOn( void )
{
		return	m_IsCusOn;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoLbl::CusPosSet( __s32 A_Pos )
{
		m_CusPos		=	A_Pos;
		ReqDraw();
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoLbl::CusPosGet( void )
{
		return	m_CusPos;
}
//--------------------------------------------------------------------


