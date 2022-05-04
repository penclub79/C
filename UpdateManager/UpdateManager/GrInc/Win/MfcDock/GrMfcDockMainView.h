/*
MFC Docking main view

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrMfcDockMainView
#define	_EPC_GrMfcDockMainView

//====================================================================
// uses

#include <Win\MfcDock\GrMfcDockBase.h>

#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrTaskCli.h>
#include <GrTaskSvr.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type




//====================================================================
//class

class Cls_GrMfcDockMainView : public CWnd
{
	protected:
		__u32	m_PaneId;

		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		DECLARE_MESSAGE_MAP()
	public:
		Cls_GrMfcDockMainView();
		virtual ~Cls_GrMfcDockMainView();

		__u32	PaneIdGet(void);

		virtual	void	RtlPaneMsgProc(__u32 A_OwnerId, __u32 A_TargetId, __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2);
};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif



