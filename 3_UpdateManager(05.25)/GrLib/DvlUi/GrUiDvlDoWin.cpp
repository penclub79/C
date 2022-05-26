/*
osd object

*/

//====================================================================
// uses
#include	<DvlUi/GrUiDvlDoWin.h>
#include	<GrStrTool.h>
#include	<GrTimeTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiConsole.h>
#include	<DvlUi/GrUiDvlMng.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local macro
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*				V_GrUiMngObj;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDvlDoWin::Cls_GrUiDvlDoWin(	__u16 A_ScriptId, BOOL8 A_IsEdit, BOOL8 A_IsUpdtParm ):
Cls_GrUiDoWin( NULL, 0, 0, FALSE, FALSE )
{
	// local -------------------
		__u8		Tv_Layer;
		BOOL8		Tv_IsSlave;
	// code --------------------
		//init
		m_IsOsdEdit		=	A_IsEdit;
		m_IsOsdDel		=	FALSE;

		m_WinType			=	E_GrUiDvlDoWintTypeDevel;

		m_ObjId				=	A_ScriptId;
		V_GrUiMngObj->ScptAtbGet( m_ObjId, &Tv_Layer, &m_Rect, &Tv_IsSlave  );
		//check page
		if ( Tv_IsSlave )
		{
			m_Rect.left		=	0;
			m_Rect.top		=	0;
			m_Rect.right	=	E_GrUiDvlMaxScrSizeX;
			m_Rect.bottom	=	E_GrUiDvlMaxScrSizeY;
		}

		if ( A_IsUpdtParm )
		{
			UpdateWinParam();
		}
}
//--------------------------------------------------------------------
Cls_GrUiDvlDoWin::~Cls_GrUiDvlDoWin()
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlDoWin::ProcEvtWinCreate( void )
{
	// local -------------------
		St_GrRect	Tv_Rect;
		__u8		Tv_Layer;
		BOOL8		Tv_IsSlave;
		St_GrUiMsgHd	Tv_Msg;
	// code --------------------
		//inherited
		Cls_GrUiDoWin::ProcEvtWinCreate();
		//loading script
		V_GrUiMngObj->ScptDecById( m_ObjId, (Cls_GrUiDoBase*)this );
		//check page
		if ( V_GrUiMngObj->ScptAtbGet( m_ObjId, &Tv_Layer, &Tv_Rect, &Tv_IsSlave ) )
		{
			if ( Tv_IsSlave )
			{
				//update text page
				m_UserDataTbl[0]	=	(0xFFFF &(__u32)Tv_Rect.left) | ( (0xFFFF & (__u32)Tv_Rect.top) << 16);
			}
		}

		//post message script decoded
		Tv_Msg.Cmd		=	E_GrUiMsgCmdScptDecCpl;
		Tv_Msg.ConIdx	=	E_GrUiConIdxFocused;
		Tv_Msg.WinId	=	m_WinId;
		Tv_Msg.ObjId	=	E_GrUiObjIdBroadcast;
		UiMsgPost( &Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlDoWin::ProcEvtTimeTick( __u32 A_Tick )
{
		if ( !m_IsOsdEdit )
		{
			Cls_GrUiDoWin::ProcEvtTimeTick( A_Tick );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlDoWin::IsOsdEditMode( void )
{
		return	m_IsOsdEdit;
}
//--------------------------------------------------------------------
void	Cls_GrUiDvlDoWin::OsdDeleteModeSet( BOOL8 A_IsDel )
{
		m_IsOsdDel	=	A_IsDel;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDvlDoWin::IsOsdDelMode( void )
{
		return	m_IsOsdDel;
}
//--------------------------------------------------------------------


