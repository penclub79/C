/*
MFC Pane Base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrMfcDockBase
#define	_EPC_GrMfcDockBase

//====================================================================
// uses

#include <afxframewndex.h>
#include <afxDockablePane.h>

#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

#include <GrTaskCli.h>
#include <GrTaskSvr.h>
#include <GrProcess.h>

//====================================================================
//constance

#define	E_GrMfcDockPaneIdNone						0
#define	E_GrMfcDockPaneIdBroadcast			0xFFFFFFFF

#define	E_GrMfcDockPaneMaxCnt						128

#define	E_GrMfcDockDbgMsgBufSize				0x10000


#define	E_GrMfcDockPaneMsgUserBase			100

// dock system status
#define	E_GrMfcDockPaneStatPrepare			0
#define	E_GrMfcDockPaneStatReady				1

// text align
#define	E_GrMfcDockTxtArrangeLeft				DT_LEFT
#define	E_GrMfcDockTxtArrangeTop				DT_TOP
#define	E_GrMfcDockTxtArrangeRight			DT_RIGHT
#define	E_GrMfcDockTxtArrangeBottom			DT_BOTTOM
#define	E_GrMfcDockTxtArrangeCenterHz		DT_CENTER
#define	E_GrMfcDockTxtArrangeCenterVt		DT_VCENTER
#define	E_GrMfcDockTxtArrangeWordWarp		DT_WORDBREAK

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrMfcDockEnv
{
	// manager
	Cls_GrTaskSvr*	TaskMng;
	Cls_GrTaskCli*	ObjMng;

	CFrameWndEx*		MainFrm;	// main frame
	CWnd*						MainView;

	__u32						Stat;		// dock system status

	// debug
	St_GrPrcCritSct	DmsgCrit;
	__u8						DmsgQue[E_GrMfcDockDbgMsgBufSize];

	// registered pane
	__u32		PaneCnt;
	CDockablePane*	PaneTbl[E_GrMfcDockPaneMaxCnt];

	
}	*Ptr_GrMfcDockEnv;



//====================================================================
//class


//====================================================================
//global function

Def_GrErrCode	GrMfcDockInit(Cls_GrTaskSvr* A_TaskSvr, __u32 A_Version = 0);
Def_GrErrCode	GrMfcDockTaskRun(void);
Def_GrErrCode	GrMfcDockFinish(void);

void	GrMfcDockReqQuit(void);

Def_GrErrCode	GrMfcDockPaneRegist(CDockablePane* A_Pane);
CDockablePane*	GrMfcDockPaneGetByPaneId(__u32 A_PaneId);
BOOL8	GrMfcDockPaneMsgPost(__u32 A_OwnerId, __u32 A_TargetId, __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2);

CMFCToolBarButton*	GrMfcDockToolbarBtnGetByCmd(CMFCToolBar* A_ObjToolbar, UINT A_Cmd);

BOOL8	GrMfcDockIsReady(void);

//====================================================================
//out Enviroment conditional
#endif

