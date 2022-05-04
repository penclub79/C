/*
MFC Pane manager

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrMfcDockMng
#define	_EPC_GrMfcDockMng

//====================================================================
// uses

#include <afxwin.h>

#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrTaskCli.h>
#include <GrTaskSvr.h>
#include <Win\MfcDock\GrMfcDockBase.h>

//====================================================================
//constance

#define	E_GrMfcDockMngMsgCmdReady					1		/* notify ready : St_GrMfcDockMngMsgPaneMsg */
#define	E_GrMfcDockMngMsgCmdPaneMsg				2		/* Pane message post : St_GrMfcDockMngMsgPaneMsg */

//====================================================================
//global macro

//====================================================================
//global type


//message para
typedef struct St_GrMfcDockMngMsgBase
{
	St_GrTaskMsgHd	MsgHd;
	DWORD		Cmd;
}	*Ptr_GrMfcDockMngMsgBase;

typedef struct St_GrMfcDockMngMsgReady
{
	St_GrMfcDockMngMsgBase	Hd;
}	*Ptr_GrMfcDockMngMsgReady;

typedef struct St_GrMfcDockMngMsgPaneMsg
{
	St_GrMfcDockMngMsgBase	Hd;
	__u32		OwnerId;
	__u32		TargetId;
	__u32		Cmd;
	__u32		Para0;
	__u32		Para1;
	__u32		Para2;
	__u32		ExcpMap;		// excpet frame map
}	*Ptr_GrMfcDockMngMsgPaneMsg;



//====================================================================
//class

class Cls_GrMfcDockMng	:	public	Cls_GrTaskCli
{
	private:
		//console
		Cls_GrTaskSvr*	m_TaskSvr;

		DWORD		m_MsgAloc;

	protected:

		void	LcMprcPaneMsg(Ptr_GrMfcDockMngMsgPaneMsg A_PtrMsg);


		//task inherited
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */

	public:
		Cls_GrMfcDockMng( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrMfcDockMng();

		void	DoReady(void);
		BOOL8	PaneMsgPost(__u32 A_OwnerId, __u32 A_TargetId, __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2);

};

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

