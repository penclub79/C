


#include <Win\MfcDock\GrMfcDockMainFrm.h>
#include <Win\MfcDock\GrMfcDockPaneBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>

#include <Win\MfcDock\GrMfcDockBase.h>
#include <Win\MfcDock\GrMfcDockMng.h>


//--------------------------------------------------------------------
//const


//--------------------------------------------------------------------
//global var

extern	St_GrMfcDockEnv	V_GrMfcDockEnv;

//--------------------------------------------------------------------
IMPLEMENT_DYNAMIC(Cls_GrMfcDockMainFrm, CFrameWndEx)

//--------------------------------------------------------------------
Cls_GrMfcDockMainFrm::Cls_GrMfcDockMainFrm() :
CFrameWndEx()
{
		m_PaneId	=	E_GrMfcDockPaneIdNone;

		V_GrMfcDockEnv.MainFrm	=	this;		// update main frame
}
//--------------------------------------------------------------------
Cls_GrMfcDockMainFrm::~Cls_GrMfcDockMainFrm()
{
		V_GrMfcDockEnv.MainFrm	=	NULL;
}
//--------------------------------------------------------------------
BEGIN_MESSAGE_MAP(Cls_GrMfcDockMainFrm, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()
//--------------------------------------------------------------------
int Cls_GrMfcDockMainFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// local -------------------
	// code --------------------

		if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		{
			return -1;
		}

		((Cls_GrMfcDockMng*)V_GrMfcDockEnv.ObjMng)->DoReady();

		return	0;
}
//--------------------------------------------------------------------
void Cls_GrMfcDockMainFrm::OnClose()
{
	// local -------------------
	// code --------------------
		V_GrMfcDockEnv.TaskMng->ReqQuit();
		CFrameWndEx::OnClose();
}
//--------------------------------------------------------------------
__u32	Cls_GrMfcDockMainFrm::PaneIdGet(void)
{
		return	m_PaneId;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockMainFrm::RtlDbgMsgAdd(WCHAR* A_StrMsg)
{

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockMainFrm::RtlPaneMsgProc(__u32 A_OwnerId, __u32 A_TargetId, __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2)
{

}
//--------------------------------------------------------------------

