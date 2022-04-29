

#include <Win\MfcDock\GrMfcDockMainView.h>


#include <GrDebug.h>
#include <GrStrTool.h>


//--------------------------------------------------------------------
//const


//--------------------------------------------------------------------
//global var

extern	St_GrMfcDockEnv	V_GrMfcDockEnv;

//--------------------------------------------------------------------
Cls_GrMfcDockMainView::Cls_GrMfcDockMainView() :
CWnd()
{
		V_GrMfcDockEnv.MainView	=	this;
}
//--------------------------------------------------------------------
Cls_GrMfcDockMainView::~Cls_GrMfcDockMainView()
{
		V_GrMfcDockEnv.MainView	=	NULL;
}
//--------------------------------------------------------------------
BEGIN_MESSAGE_MAP(Cls_GrMfcDockMainView, CWnd)

	ON_WM_ERASEBKGND()

END_MESSAGE_MAP()
//--------------------------------------------------------------------
__u32	Cls_GrMfcDockMainView::PaneIdGet(void)
{
		return	m_PaneId;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockMainView::RtlPaneMsgProc(__u32 A_OwnerId, __u32 A_TargetId, __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2)
{

}
//--------------------------------------------------------------------
BOOL Cls_GrMfcDockMainView::OnEraseBkgnd(CDC* pDC)
{
		return	TRUE;
}
//--------------------------------------------------------------------
