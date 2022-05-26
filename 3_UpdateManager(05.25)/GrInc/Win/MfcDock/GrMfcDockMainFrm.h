/*
MFC Docking main frame

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrMfcDockMainFrm
#define	_EPC_GrMfcDockMainFrm

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

class Cls_GrMfcDockMainFrm : public CFrameWndEx
{
	protected:
		__u32		m_PaneId;

		DECLARE_DYNAMIC(Cls_GrMfcDockMainFrm)

		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg	void OnClose();
		DECLARE_MESSAGE_MAP()
	public:
		Cls_GrMfcDockMainFrm();
		virtual ~Cls_GrMfcDockMainFrm();

		__u32	PaneIdGet(void);

		// runtime
		virtual	void	RtlPaneMsgProc(__u32 A_OwnerId, __u32 A_TargetId, __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2);
		virtual	void	RtlDbgMsgAdd(WCHAR* A_StrMsg);

};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif



