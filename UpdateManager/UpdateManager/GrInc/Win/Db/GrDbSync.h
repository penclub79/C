/*
DB sync
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_GrDbSync
#define	_GrDbSync

//====================================================================
// debug

#define	E_GrDbSyncDbgMsg

//====================================================================
// uses
#include <afxwin.h>

#include <WinSock2.h>

#include <GrTypeBase.h>
#include <GrTaskBase.h>
#include <GrTaskCli.h>
#include <GrTaskSvr.h>

#include <GrSockV2.h>

#include <GrTimeTool.h>
#include <GrError.h>
#include <GrStrTool.h>

#include <Win/Db/GrDbBase.h>
#include <Win/Db/GrDbGrp.h>

//====================================================================
//constance

#define	E_GrDbSyncTimerPeriod							1000		/* 1 sec */

#define	E_GrDbSyncSendBufSize							(E_GrDbDatBlkSize * 8)
#define	E_GrDbSyncRecvBufSize							(E_GrDbDatBlkSize * 3)

#define	E_GrDbSyncKeyInfoSendUnitCnt			256


#define	E_GrDbSyncScanLatency							30
#define	E_GrDbSyncScanMaxCnt							4096

#define	E_GrDbSyncReqMaxCnt								(E_GrDbSyncScanMaxCnt * 2)

#define	E_GrDbSyncChgMapDwCnt							(E_GrDbKeyMaxCnt >> 5)


#define	E_GrDbSyncTimeOut									40

#define	E_GrDbSyncChgTimeMargin						(E_GrTimeSecOfMin * 30)

// protocol
#define	E_GrDbSyncPrtcHdFcc								Mac_GrMakeFcc('D','S','Y','N')

#define	E_GrDbSyncPrtcCmdGrpInfo					1
#define	E_GrDbSyncPrtcCmdKeyInfo					2
#define	E_GrDbSyncPrtcCmdReqUpdt					3
#define	E_GrDbSyncPrtcCmdData							4
#define	E_GrDbSyncPrtcCmdSwitchHost				5
#define	E_GrDbSyncPrtcCmdError						6

// common stage
#define	E_GrDbSyncStageWaitConn						0
#define	E_GrDbSyncStageReady							1
#define	E_GrDbSyncStageSendGroupInfo			2
#define	E_GrDbSyncStageSendKeyInfo				3
#define	E_GrDbSyncStageQuit								4

// quit code
#define	E_GrDbSyncQuitGrpMismatch					1		/* group infomation mismatch */

// command
#define	E_GrDbSyncCmdInit									1
#define	E_GrDbSyncCmdFinish								2
#define	E_GrDbSyncCmdChgDataAdd						3

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDbSyncSendPktItem
{
	__u32	Size;
	__u32	Sended;
}	*Ptr_GrDbSyncSendPktItem;

// protocol
#pragma	pack(1)

typedef struct St_GrDbSyncPrtcHead
{
	__u32	Fcc;
	__u32	Cmd;
	__u32	TotSize;			// total size
}	*Ptr_GrDbSyncPrtcHead;

typedef struct St_GrDbSyncPrtcGrpInfo
{
	St_GrDbSyncPrtcHead	Hd;
	__u32	_rsvd0;
	St_GrDbGrpInfo	GrpInfo[E_GrDbGrpMaxCnt];
}	*Ptr_GrDbSyncPrtcGrpInfo;

typedef struct St_GrDbSyncPrtcKeyInfo
{
	St_GrDbSyncPrtcHead	Hd;
	BOOL8	IsLast;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	GrpId;
	__u32	Cnt;
}	*Ptr_GrDbSyncPrtcKeyInfo;

typedef struct St_GrDbSyncPrtcReqUpdt
{
	St_GrDbSyncPrtcHead	Hd;
	BOOL8	IsLast;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	GrpId;
	__u32	Cnt;
}	*Ptr_GrDbSyncPrtcReqUpdt;

typedef struct St_GrDbSyncPrtcDataKeyAtb
{
	St_GrDbSyncPrtcHead	Hd;
	__u32	GrpId;
	__u32	BlkId;
	__u32	_rsvd0;

	St_GrDbKeyAtb	KeyAtb;
}	*Ptr_GrDbSyncPrtcDataKeyAtb;

typedef struct St_GrDbSyncPrtcDataBlkVal
{
	St_GrDbSyncPrtcHead	Hd;
	__u32	GrpId;
	__u32	BlkId;
	__u32	_rsvd0;
	__u64	Key[E_GrDbKeyQwCnt];
	__u32	ChkSum;
	__u32	TimeUpdt;
	__u8	Data[E_GrDbDatBlkSize];
}	*Ptr_GrDbSyncPrtcDataBlkVal;

typedef struct St_GrDbSyncPrtcSwitchHost
{
	St_GrDbSyncPrtcHead	Hd;
}	*Ptr_GrDbSyncPrtcSwitchHost;

typedef struct St_GrDbSyncPrtcError
{
	St_GrDbSyncPrtcHead	Hd;
	__u32		ErrCode;
}	*Ptr_GrDbSyncPrtcError;

#pragma	pack()

typedef struct St_GrDbSyncCmdBase
{
	St_GrTaskMsgHd	Hd;
	__u32		Cmd;
}	*Ptr_GrDbSyncCmdBase;


typedef struct St_GrDbSyncCmdInit
{
	St_GrDbSyncCmdBase	Hd;
	Cls_GrTaskCli*	DbCtl;
}	*Ptr_GrDbSyncCmdInit;

typedef struct St_GrDbSyncCmdFinish
{
	St_GrDbSyncCmdBase	Hd;
}	*Ptr_GrDbSyncCmdFinish;

typedef struct St_GrDbSyncCmdChgDataAdd
{
	St_GrDbSyncCmdBase	Hd;
	__u32		GrpId;
	__u32		KeyId;
	__u32		BlkMap;
}	*Ptr_GrDbSyncCmdChgDataAdd;

//====================================================================
//class

class Cls_GrDbSync : public Cls_GrTaskCli
{
	protected:
		Cls_GrTaskSvr*		m_TskSvr;
		__u32							m_MsgCmd;
		__u32							m_TmrSec;

		// common
		BOOL8							m_IsInited;

		St_GrDbGrpInfo		m_GrpInfo[E_GrDbGrpMaxCnt];

		BOOL8							m_IsServer;

		__u8							m_Stage;

		__u32							m_IpSv;
		__u16							m_Port;

		__u32							m_TmOut;		// time out timer

		void*							m_DbCtl;		// database control

		Def_GrSock				m_SockListen;
		Def_GrSock				m_SockConn;

		__u32							m_IpRmt;		// remote ip

		// key scan
		__u32							m_ScanGrpId;
		__u32							m_ScanKeyId;

		// changed data send
		BOOL8							m_IsCdsRun;
		__u32							m_CdsGrpId;
		__u32							m_CdsKeyIdx;
		__u32							m_CdsBlkId;		// 0 = key info , 1 ~ block index
		__u32							m_GrpChgMap[E_GrDbGrpMaxCnt][E_GrDbKeyMaxCnt];
		__u32							m_CdsCnt;

		// send
		Ptr_GrDbSyncSendPktItem	m_PtrSendItm;
		__u8							m_SendQue[E_GrDbSyncSendBufSize];

		// receive
		__u32							m_Recved;
		__u8							m_RecvBuf[E_GrDbSyncRecvBufSize];
		
		// error code
		__u32							m_QuitCode;

		// normal work
		BOOL8	LcRecvProc(void);
		BOOL8	LcSendProc(void);

		// receive
		void	LcRcvCmdGrpInfo(void);
		void	LcRcvCmdKeyInfo(void);
		void	LcRcvCmdReqUpdt(void);
		void	LcRcvCmdDataSet(void);
		void	LcRcvCmdSwitchHost(void);
		void	LcRcvCmdError(void);

		// client
		void	LcCliStageWaitConn(void);

		// server
		void	LcSvrStageWaitConn(void);

		// common stage
		void	LcStageSendGroupInfo(void);
		void	LcStageSendKeyInfo(void);
		void	LcStageQuit(void);

		// send completed work
		void	LcSendCplKeyInfo(void);
		void	LcSendCplData(void);
		void	LcSendCplError(void);

		// common work
		void*	LcSendBufAlloc(__u32 A_Size);
		BOOL8	LcSendDo(__u32 A_Cmd, __u32 A_Size);

		void	LcSyncDataWork(void);

		BOOL8	LcDataKeyAtbSend(__u32 A_GrpId, __u32 A_KeyId);
		BOOL8	LcDataBlockSend(__u32 A_GrpId, __u32 A_KeyId,__u32 A_BlkIdx);

		BOOL8	LcSwitchHostSend(void);
		void	LcInitSend();

		void	LcConnClose(void);

		BOOL8	LcStageProc(void);

		// inherited
		virtual	BOOL8	RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg);		/* return = message processed */
		virtual	BOOL8	RtlBkgProc(void);												/* return = background processed */
		virtual	void	RtlTimer(__u32 A_Hndl);

	public:
		Cls_GrDbSync(Cls_GrTaskSvr* A_TaskSvr, void* A_ObjDbCtl, BOOL8 A_IsSvr, __u32 A_IpSv, __u16 A_Port);
		virtual	~Cls_GrDbSync(void);

		// immidatly process
		void	ImmInit(void);
		void	ImmFinish(void);

		void	ImmChgDataAdd(__u32 A_GrpId, __u32 A_KeyId, __u32 A_BlkMap);

		// request
		void	ReqInit(Cls_GrTaskCli* A_DbCtl);
		void	ReqFinish(void);

		void	ReqChgDataAdd(__u32 A_GrpId, __u32 A_KeyId, __u32 A_BlkMap);


};


//====================================================================
//global function


//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif


