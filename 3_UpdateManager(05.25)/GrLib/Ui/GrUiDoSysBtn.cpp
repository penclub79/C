/*
 spin number button

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoSysBtn.h>
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
Cls_GrUiDoSysBtn::Cls_GrUiDoSysBtn( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSmpBtn( A_PtrRect, A_ObjPrnt, A_IsIdAlloc )
{
	// local -------------------
	// code --------------------
		//init
		m_TypeId				=	E_GrUiDoTypeIdSysBtn;

		m_TextId				=	E_GrUiTxtIdNone;
		GrStrClear( m_StrText );
}
//--------------------------------------------------------------------
Cls_GrUiDoSysBtn::~Cls_GrUiDoSysBtn()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSysBtn::ProcBgSkinTblDraw( void )
{
		Cls_GrUiDoSmpBtn::ProcBgSkinTblDraw();
		
		if ( PaintClipperSet( TRUE ) )
		{
			if (E_GrUiTxtIdNone != m_TextId) 
			{
				RosTextDraw( m_TextId, &m_TxtPara, m_SmpbTxtClrTbl[m_SmpbStatIdx], &m_PntRcClient );
			}
			else if ( GrStrIsExist(m_StrText) ) 
			{
				TextDraw( m_StrText, &m_TxtPara, m_SmpbTxtClrTbl[m_SmpbStatIdx], &m_PntRcClient );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSysBtn::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		Cls_GrUiDoSmpBtn::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		//send close event
		((Cls_GrUiDoWin*)m_ObjWin)->Close();		//request close
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSysBtn::ProcEvtKeyUp( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		switch( A_Key )
		{
			case	E_GrUiKeyEnter:
				//send close event
				((Cls_GrUiDoWin*)m_ObjWin)->Close();		//request close
				Tv_Result	=	TRUE;
				break;
		}

		//default process
		if ( !Tv_Result )
		{
			Tv_Result	=	Cls_GrUiDoSmpBtn::ProcEvtKeyUp( A_Key );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSysBtn::TextIdSet( __u32 A_Id )
{
		m_TextId	=	A_Id;
		ReqDraw();
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSysBtn::TextIdGet( void )
{
		return	m_TextId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSysBtn::TextGet( WCHAR* A_StrRt, __u32 A_LenMax )
{
		GrStrWcopy( A_StrRt, m_StrText, A_LenMax );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSysBtn::TextSet( WCHAR* A_Str )
{
		GrStrWcopy( m_StrText, A_Str, sizeof(m_StrText) - 1 );
		ReqDraw();
}
//--------------------------------------------------------------------

