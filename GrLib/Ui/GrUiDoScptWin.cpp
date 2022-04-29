/*
 osd object

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoScptWin.h>
#include	<GrStrTool.h>
#include	<GrTimeTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiConsole.h>
#include <GrDebug.h>

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
Cls_GrUiDoScptWin::Cls_GrUiDoScptWin(	__u16 A_ScriptId, __u8 A_Layer, __u8 A_ConIdx, BOOL8 A_IsUpdtParm ):
Cls_GrUiDoWin( NULL, A_Layer, A_ConIdx, FALSE, FALSE )
{
	// local -------------------
		BOOL8		Tv_IsSlaveScpt;
	// code --------------------
		//init
		m_ObjId		=	A_ScriptId;
		
		V_GrUiMngObj->ScptAtbGet( m_ObjId, &m_Layer, &m_Rect, &Tv_IsSlaveScpt  );
		if ( E_GrUiDoWinScptLayer != A_Layer )
		{
			m_Layer		=	A_Layer;
		}

		if ( A_IsUpdtParm )
		{
			UpdateWinParam();
		}
}
//--------------------------------------------------------------------
Cls_GrUiDoScptWin::~Cls_GrUiDoScptWin()
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
void	Cls_GrUiDoScptWin::ProcEvtWinCreate( void )
{
	// local -------------------
		St_GrUiMsgHd	Tv_Msg;
	// code --------------------
		//run script
		if ( !V_GrUiMngObj->ScptDecById( m_ObjId, (Cls_GrUiDoBase*)this ) )
		{
			DbgMsgPrint( "Cls_GrUiDoScptWin::ProcEvtWinCreate - bad script %d.\n", m_ObjId );
		}
		//inherited
		Cls_GrUiDoWin::ProcEvtWinCreate();
		//post message script decoded
		Tv_Msg.Cmd		=	E_GrUiMsgCmdScptDecCpl;
		Tv_Msg.ConIdx	=	E_GrUiConIdxFocused;
		Tv_Msg.WinId	=	m_WinId;
		Tv_Msg.ObjId	=	E_GrUiObjIdBroadcast;
		UiMsgSend( &Tv_Msg );
		//UiMsgPost( &Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------

