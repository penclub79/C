/*
 spin number button

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoEdit.h>
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
Cls_GrUiDoEdit::Cls_GrUiDoEdit( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSmpBtn( A_PtrRect, A_ObjPrnt, A_IsIdAlloc )
{
	// local -------------------
	// code --------------------
		//init
		m_TypeId				=	E_GrUiDoTypeIdEdit;
		m_IsEditNum			=	FALSE;
		m_IsNoKeyVk			=	FALSE;
		m_MaxEdit				=	GrStrMaxLen( m_StrText );

		GrStrClear( m_StrText );
}
//--------------------------------------------------------------------
Cls_GrUiDoEdit::~Cls_GrUiDoEdit()
{
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoEdit::IsEditTypeNumber( void )
{
		return	m_IsEditNum;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::EditTypeSet( BOOL8 A_IsNum )
{
		m_IsEditNum	=	A_IsNum;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::ProcBgSkinTblDraw( void )
{
	// local -------------------
	// code --------------------
		Cls_GrUiDoSmpBtn::ProcBgSkinTblDraw();
		//draw text
		if ( PaintClipperSet( TRUE ) )
		{
			//draw text
			if ( GrStrIsExist(m_StrText) ) 
			{
				TextDraw( m_StrText, &m_TxtPara, m_SmpbTxtClrTbl[m_SmpbStatIdx], &m_PntRcClient );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::EvtHostIdSet( __u16 A_ObjId )
{
		m_EvtHostId	=	A_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoEdit::EvtHostIdGet( void )
{
		return	m_EvtHostId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::TextGet( WCHAR* A_StrRt, __u32 A_LenMax )
{
		GrStrWcopy( A_StrRt, m_StrText, A_LenMax );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::TextSet( WCHAR* A_Str )
{
		GrStrWcopy( m_StrText, A_Str,	GrStrMaxLen(m_StrText) );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::LcEditEventMake( void )
{
	// local -------------------
		St_GrUiMsgCtrl		Tv_Msg;
	// code --------------------
		//run event
		if ( V_GrUiMngObj->IsUserEventOn() || (E_GrUiObjIdNone != m_EvtHostId) )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdEditValChg;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_EvtHostId;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::LcVirtualKbdShow( void )
{
	// local -------------------
		BOOL8	Tv_IsPasswd;
	// code --------------------
		Tv_IsPasswd		=	FALSE;
		if ( 0 != ( E_GrUiDoTextAtbPasswd & m_TxtPara.Atb ) )
		{
			Tv_IsPasswd	=	TRUE;
		}
		m_ObjConsole->VirtKbdCreate(	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet(), m_ObjId, 
																	m_IsEditNum, Tv_IsPasswd, (__u32)m_StrText, 
																	m_StrText, m_MaxEdit, FALSE, m_IsLangLock );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoEdit::ProcEvtKeyUp( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		switch( A_Key )
		{
			case E_GrUiKeyCh0:
				LcEditNumKey(0);
				break;
			case E_GrUiKeyCh1:
				LcEditNumKey(1);
				break;
			case E_GrUiKeyCh2:
				LcEditNumKey(2);
				break;
			case E_GrUiKeyCh3:
				LcEditNumKey(3);
				break;
			case E_GrUiKeyCh4:
				LcEditNumKey(4);
				break;
			case E_GrUiKeyCh5:
				LcEditNumKey(5);
				break;
			case E_GrUiKeyCh6:
				LcEditNumKey(6);
				break;
			case E_GrUiKeyCh7:
				LcEditNumKey(7);
				break;
			case E_GrUiKeyCh8:
				LcEditNumKey(8);
				break;
			case E_GrUiKeyCh9:
				LcEditNumKey(9);
				break;
			case E_GrUiKeyCh10:
				LcEditNumKey(0);
				break;
			case	E_GrUiKeyEnter:
				Tv_Result	=	LcProcKeyUpEnter();
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
BOOL8	Cls_GrUiDoEdit::LcProcKeyUpEnter( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_IsNoKeyVk )
		{
			LcVirtualKbdShow();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoEdit::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		LcVirtualKbdShow();
		Cls_GrUiDoSmpBtn::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::ProcEvtVkbd( __u32 A_VkId	)
{
		//inherited
		Cls_GrUiDoSmpBtn::ProcEvtVkbd( A_VkId );
		//check match text
		if ( A_VkId == (__u32)m_StrText )
		{
			LcEditEventMake();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::EditMaxLenSet( __u32 A_Len )
{
		if ( A_Len <= GrStrMaxLen(m_StrText) )
		{
			m_MaxEdit		=	A_Len;
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoEdit::EditMaxLenGet( void )
{
		return	m_MaxEdit;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::ProcEvtMseEnter( void )
{
		Cls_GrUiDoSmpBtn::ProcEvtMseEnter();	
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::ProcEvtMseLeave( void )
{
		Cls_GrUiDoSmpBtn::ProcEvtMseLeave();
}
//--------------------------------------------------------------------
void Cls_GrUiDoEdit::LcEditNumKey(__u32 A_KeyIdx)
{
	// var --------------------
		__u32			Tv_LenIn;
		//__u8			Tv_PassStat;
		//__u16			Tv_TxtId;
	// code -------------------
		Tv_LenIn	=	GrStrWlen(m_StrText);
		if(m_MaxEdit > Tv_LenIn)
		{
			//insert
			m_StrText[Tv_LenIn]			=	(WCHAR)(L'0' + A_KeyIdx);
			m_StrText[Tv_LenIn + 1]	=	0;

			LcEditEventMake();
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoEdit::IsNoKeyVkbd( void )
{
		return	m_IsNoKeyVk;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::NoKeyVkbdSet( BOOL8 A_IsSet )
{
		m_IsNoKeyVk	=	A_IsSet;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoEdit::LcEditGetSkinIdx( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrUiDoSmpBtn::LcEditGetSkinIdx();
		if ( Tv_Result != E_GrUiDoSmpbImgIdxDisable )
		{
			if ( !GrStrIsExist(m_StrText) )
			{
				Tv_Result	=	Tv_Result + 2;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoEdit::LanguageLockSet( BOOL8 A_IsLock )
{
	// local -------------------
	// code --------------------
		m_IsLangLock	=	A_IsLock;
}
//--------------------------------------------------------------------
