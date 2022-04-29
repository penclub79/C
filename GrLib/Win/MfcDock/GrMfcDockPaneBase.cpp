/*
	Pane base

*/
#include <Win\MfcDock\GrMfcDockBase.h>
#include <Win\MfcDock\GrMfcDockPaneBase.h>


#include <GrDebug.h>
#include <GrDumyTool.h>

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var

//====================================================================
//--------------------------------------------------------------------
Cls_GrMfcDockPaneBase::Cls_GrMfcDockPaneBase(void) :
CDockablePane()
{
	// local -------------------
	// code --------------------
		// init

		m_PaneId		=	E_GrMfcDockPaneIdNone;

		// regist
		if(E_GrErrNone != GrMfcDockPaneRegist(this))
		{
			DbgMsgPrint("Cls_GrMfcDockPaneBase::Cls_GrMfcDockPaneBase - Pane regist error!\n");
		}
		
}
//--------------------------------------------------------------------
Cls_GrMfcDockPaneBase::~Cls_GrMfcDockPaneBase( void )
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockPaneBase::RtlPaneMsgProc(__u32 A_OwnerId, __u32 A_TargetId, __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2)
{

}
//--------------------------------------------------------------------
__u32	Cls_GrMfcDockPaneBase::PaneIdGet(void)
{
		return	m_PaneId;
}
//--------------------------------------------------------------------
