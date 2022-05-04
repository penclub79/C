/*
product regist server
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_GrSvrIocpJob
#define	_GrSvrIocpJob

//====================================================================
// uses

#include <WinSock2.h>

#include <GrTypeBase.h>
#include <GrTaskBase.h>
#include <GrTaskCli.h>
#include <GrTaskSvr.h>

#include <GrSockV2.h>

#include <GrError.h>
#include <GrStrTool.h>

#include <Protocol/GrPrtcDdnsV3.h>


//====================================================================
//constance

#define	E_GrSvrIocpJobTimerPeriod							1000		/* 1 sec */

#define	E_GrSvrIocpJobConnHndlNone						0

#define	E_GrSvrIocpJobOvlpEntCnt							32

#define	E_GrSvrIocpJobHndlShftConnIdx					16
#define	E_GrSvrIocpJobHndlMaskConnIdx					0xFFFF

#define	E_GrSvrIocpJobIocpWaitTime						40

// command
#define	E_GrSvrIocpJobCmdInit									1
#define	E_GrSvrIocpJobCmdFinish								2
#define	E_GrSvrIocpJobCmdConnAdd							3
#define	E_GrSvrIocpJobCmdCapRemainGet					4
#define	E_GrSvrIocpJobCmdConnClose						5

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrSvrIocpJobOvlp
{
	WSAOVERLAPPED	Ovlp;
	WSABUF	BufInfo;
	void*	PtrBuf;
	__u32	Prgs;
	__u32	BufSize;
	BOOL8	IsNtfCpl;		// notify completed
	BOOL8	IsDoing;
}	*Ptr_GrSvrIocpJobOvlp;


typedef struct St_GrSvrIocpJobConn
{
	SOCKET	Sock;
	__u32		Ip;
	__u16		Port;
	__u16		LocalId;

	// receive
	St_GrSvrIocpJobOvlp	Recv;

	// send
	St_GrSvrIocpJobOvlp	Send;

	// timeout
	__u32	ToutLim;
	__u32	ToutPrgs;
}	*Ptr_GrSvrIocpJobConn;

typedef struct St_GrSvrIocpJobCmdBase
{
	St_GrTaskMsgHd	Hd;
	__u32		Cmd;
}	*Ptr_GrSvrIocpJobCmdBase;


typedef struct St_GrSvrIocpJobCmdInit
{
	St_GrSvrIocpJobCmdBase	Hd;
	BOOL8*	PtrRslt;
}	*Ptr_GrSvrIocpJobCmdInit;

typedef struct St_GrSvrIocpJobCmdFinish
{
	St_GrSvrIocpJobCmdBase	Hd;
}	*Ptr_GrSvrIocpJobCmdFinish;

typedef struct St_GrSvrIocpJobCmdConnAdd
{
	St_GrSvrIocpJobCmdBase	Hd;
	BOOL8*	PtrRslt;
	SOCKET	Sock;
	__u32		Ip;
	__u16		Port;
}	*Ptr_GrSvrIocpJobCmdConnAdd;

typedef struct St_GrSvrIocpJobCmdCapRemainGet
{
	St_GrSvrIocpJobCmdBase	Hd;
	__s32*	PtrRslt;
}	*Ptr_GrSvrIocpJobCmdCapRemainGet;

typedef struct St_GrSvrIocpJobCmdConnClose
{
	St_GrSvrIocpJobCmdBase	Hd;
	BOOL8*	PtrRslt;
	__u32		HndlConn;
}	*Ptr_GrSvrIocpJobCmdConnClose;

//====================================================================
//class

class Cls_GrSvrIocpJob : public Cls_GrTaskCli
{
	protected:
		Cls_GrTaskSvr*		m_TskSvr;
		__u32							m_MsgCmdJob;
		__u32							m_TmrSec;

		HANDLE						m_HndlIocp;
		
		void*							m_ConnTbl;
		__u32							m_ConnUnitSize;
		__u32							m_ConnCnt;
		__u32							m_ConnMaxCnt;

#ifndef GR_IOCP_XP
		OVERLAPPED_ENTRY	m_OvlpEntTbl[E_GrSvrIocpJobOvlpEntCnt];
#endif

		// local
		void	LcProcSend(__u32 A_HndlConn, Ptr_GrSvrIocpJobConn A_PtrConn,__u32 A_Transed);
		void	LcProcRecv(__u32 A_HndlConn, Ptr_GrSvrIocpJobConn A_PtrConn, __u32 A_Transed);
		void	LcUpdtTimeOut(void);
		void	LcUpdtTimer(void);

		// api
		__u32	ConnHndlEncode(__u32 A_ConnIdx, __u16 A_LocalId);
		BOOL8	ConnHndlDecode(__u32 A_HndlConn, __u32* A_PtrRtConnIdx, __u16* A_PtrRtLocalId);
		void*	ConnPtrGet(__u32 A_HndlConn);
		BOOL8	ConnDelete(__u32 A_HndlConn);

		BOOL8	ConnSend(__u32 A_HndlConn, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsCplNotify);
		BOOL8	ConnRecv(__u32 A_HndlConn, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsCplNotify);

		BOOL8	ConnTimeOutSet(__u32 A_HndlConn, __u32 A_ToutSec);
		void	ConnTimeOutSingal(__u32 A_HndlConn);
		void	ConnTimeOutKill(__u32 A_HndlConn);

		BOOL8	IsConnSending(__u32 A_HndlConn);

		// event
		virtual	BOOL8	EvtConnAdd(__u32 A_HndlConn);
		virtual	void	EvtSended(__u32 A_HndlConn, void* A_PtrBuf, __u32 A_Size);
		virtual	void	EvtRecved(__u32 A_HndlConn, void* A_PtrBuf, __u32 A_Size);
		virtual	void	EvtClosed(__u32 A_HndlConn);
		virtual	void	EvtTimer(__u32 A_HndlConn);
		virtual	void	EvtTimeOut(__u32 A_HndlConn);

		// background work
		BOOL8	BgwJobProc(void);

		// inherited
		virtual	BOOL8	RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg);		/* return = message processed */
		virtual	BOOL8	RtlBkgProc(void);												/* return = background processed */
		virtual	void	RtlTimer(__u32 A_Hndl);

	public:
		Cls_GrSvrIocpJob(Cls_GrTaskSvr* A_TaskSvr, __u32 A_MaxConnCnt, __u32 A_ConnUnitSize = sizeof(St_GrSvrIocpJobConn));
		virtual	~Cls_GrSvrIocpJob(void);

		// immidatly process
		BOOL8	ImmInit(void);
		void	ImmFinish(void);

		BOOL8	ImmConnAdd(SOCKET A_Sock, __u32 A_Ip, __u16 A_Port);
		__s32	ImmCapRemainGet(void);

		// thead safe process
		BOOL8	ReqInit(void);
		BOOL8	ReqFinish(void);
		BOOL8	ReqConnAdd(SOCKET A_Sock, __u32 A_Ip, __u16 A_Port,BOOL8 A_IsPost = FALSE);
		__s32	ReqCapRemainGet(void);
		BOOL8	ReqConnClose(__u32 A_HndlConn,BOOL8 A_IsPost = FALSE);

};


//====================================================================
//global function


//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif


