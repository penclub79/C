/*
product regist server
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_GrSvrIocpMng
#define	_GrSvrIocpMng

//====================================================================
// uses

#include <WinSock2.h>

#include <GrTypeBase.h>
#include <GrTaskBase.h>
#include <GrTaskCli.h>
#include <GrTaskSvr.h>

#include <GrError.h>
#include <GrStrTool.h>

//====================================================================
//constance

#define	E_GrSvrIocpMngJobMaxCnt							8

#define	E_GrSvrIocpMngListenQueue						64

#define	E_GrSvrIocpMngWaitEvtTime						40			/* milisecond unit */

//#define	E_GrSvrIocpMngTimerPeriod						1000		/* 1 sec */

// command
#define	E_GrSvrIocpMngCmdInit								1
#define	E_GrSvrIocpMngCmdFinish							2

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrSvrIocpMngJobCtrl
{
	Cls_GrTaskSvr*	TskSvr;
	Cls_GrTaskCli*	ObjJob;
}	*Ptr_GrSvrIocpMngJobCtrl;

typedef struct St_GrSvrIocpMngCmdBase
{
	St_GrTaskMsgHd	Hd;
	__u32		Cmd;
}	*Ptr_GrSvrIocpMngCmdBase;

typedef struct St_GrSvrIocpMngCmdInit
{
	St_GrSvrIocpMngCmdBase	Hd;
	BOOL8*	PtrRslt;
	__u16		Port;
	__u32		JobCnt;
	__u32		TaskIdBase;
}	*Ptr_GrSvrIocpMngCmdInit;

typedef struct St_GrSvrIocpMngCmdFinish
{
	St_GrSvrIocpMngCmdBase	Hd;
}	*Ptr_GrSvrIocpMngCmdFinish;


//====================================================================
//class

class Cls_GrSvrIocpMng : public Cls_GrTaskCli
{
	protected:

		Cls_GrTaskSvr*		m_TskSvr;
		__u32							m_MsgCmdMng;
		//__u32							m_TmrSec;

		__u16							m_PortListen;
		SOCKET						m_SockListen;		// listen socket
		WSAEVENT					m_EvtListen;

		St_GrSvrIocpMngJobCtrl	m_JobTbl[E_GrSvrIocpMngJobMaxCnt];
		__u32		m_JobCnt;

		// local
		Cls_GrTaskCli*	JobCapableGet(void);

		// background work
		BOOL8	BgwListen(void);

		// event
		virtual	Cls_GrTaskCli*	EvtJobAdd(Cls_GrTaskSvr* A_TaskSvr);
		virtual	BOOL8	EvtConnAdd(SOCKET A_Sock,__u32 A_Ip,__u16 A_Port);

		// inherited
		virtual	BOOL8	RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg);		/* return = message processed */
		virtual	BOOL8	RtlBkgProc(void);												/* return = background processed */
		//virtual	void	RtlTimer(__u32 A_Hndl);

	public:
		Cls_GrSvrIocpMng(Cls_GrTaskSvr* A_TaskSvr);
		virtual	~Cls_GrSvrIocpMng(void);

		// immidatly process
		BOOL8	ImmInit(__u16 A_Port,__u32 A_JobCnt,__u32 A_TaskIdBase);
		void	ImmFinish(void);

		// thead safe process
		BOOL8	ReqInit(__u16 A_Port, __u32 A_JobCnt, __u32 A_TaskIdBase);
		BOOL8	ReqFinish(void);

};


//====================================================================
//global function


//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif


