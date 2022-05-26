/*
	gause UPNP

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrUpnpCli
#define	_EPC_GrUpnpCli

//====================================================================
// option

#define	E_GrUpnpCliDebug

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<GrSockV2.h>
#include	<GrBufTool.h>


//====================================================================
//const


#define E_GrUpnpCliDevMaxCnt					4
#define	E_GrUpnpCliReqMaxCnt					4

#define	E_GrUpnpCliDevCgiMaxLen				127
#define	E_GrUpnpCliDevUrnMaxLen				127
#define	E_GrUpnpCliDevUuidMaxLen			127

#define	E_GrUpnpCliCgiHeadMax					1024
#define E_GrUpnpCliRecvBufLen					2048

#define E_GrUpnpCliBroadAddr					Mac_GrSockMakeIp(239,255,255,250)
#define E_GrUpnpCliBroadPort					1900

#define E_GrUpnpCliApplyCilCnt				4			/* not use */
#define	E_GrUpnpCliMaxSvCnt						4			/* not use */

#define E_GrUpnpCliAckMsg							"HTTP/1.1 200 OK"
#define E_GrUpnpCliNakMsg							"HTTP/1.1 500 Internal Server Error"

// request command
#define E_GrUpnpCliReqNone								0
#define E_GrUpnpCliReqPortForward					1

// stage
#define E_GrUpnpCliStageNone							0
#define E_GrUpnpCliStageDiscverV4					1
#define E_GrUpnpCliStageReadDiscvV4				2

#define E_GrUpnpCliStagePortFwPrepare			100
#define E_GrUpnpCliStagePortFwReqEntry		101
#define E_GrUpnpCliStagePortFwParsAns			102


#define	E_GrUpnpCliStageCgiTryConn				900
#define	E_GrUpnpCliStageCgiConnWait				901
#define	E_GrUpnpCliStageCgiSendHead				902
#define	E_GrUpnpCliStageCgiSendCont				903
#define	E_GrUpnpCliStageCgiRecvAns				904

#define E_GrUpnpCliStageFinish						1000

// return code
#define	E_GrUpnpCliRtSuccess							0
#define	E_GrUpnpCliRtNotEnoughRsrc				1
#define	E_GrUpnpCliRtSocketProblem				2
#define	E_GrUpnpCliRtNotFound							3

// message command
#define	E_GrUpnpCliCmdInit								1
#define	E_GrUpnpCliCmdFinish							2

//====================================================================
//type

typedef struct St_GrUpnpCliDevCtl
{
	__u16	Port;
	__u32	IpV4;
	char	StrCgi[E_GrUpnpCliDevCgiMaxLen + 1];
	char	StrUrn[E_GrUpnpCliDevUrnMaxLen + 1];
	char	StrUuid[E_GrUpnpCliDevUuidMaxLen + 1];
}	*Ptr_GrUpnpCliDevCtl;

typedef struct St_GrUpnpCliReqPortFw
{
	__u16	PortLc;
	__u16	PortOut;
}	*Ptr_GrUpnpCliReqPortFw;

typedef struct St_GrUpnpCliReqItem
{
	union 
	{
		St_GrUpnpCliReqPortFw	PortFw;
	};
}	*Ptr_GrUpnpCliReqItem;

typedef struct St_GrUpnpCliRtlPortFw
{
	__u32	FindIdx;	// find entry index
}	*Ptr_GrUpnpCliRtlPortFw;

typedef struct St_GrUpnpCliRuntime
{
	union 
	{
		St_GrUpnpCliRtlPortFw	PortFw;
	};
}	*Ptr_GrUpnpCliRuntime;

typedef struct St_NetCtrlSvInfo
{
	BOOL8			IsUse;
	__s32				Dns;
	__u32			Ip;
	__u32			Port;
	Def_StrName		StrIp;
	Def_StrTag		CtrlUrl;
	Def_StrTag		Url;
} *Ptr_NetCtrlSvInfo;

typedef struct St_GrUpnpCliCliInfo
{
	__u16		ExtrPort;
	__u16		InterPort;
	__u32		Protocol;				//tcp:0, udp:1
	__u32		ClientIp;
	__u32		LeaseTime;
	Def_StrShort	Tiltle;
} *Ptr_GrUpnpCliCliInfo;

// message command
typedef struct St_GrUpnpCliMsgHd
{
	St_GrTaskMsgHd	Hd;
	__u32	Cmd;
}	*Ptr_GrUpnpCliMsgHd;


//====================================================================
//class
class Cls_GrUpnpCli	:	public Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		Cls_GrTaskCli*	m_CbCli;		//event client
		__u32			m_MsgAloc;
		__u32			m_TimerSec;


		Def_GrSock		m_Sock;		// borad cast socket

		__u8		m_IntfIdx;			// interface device index

		__u8		m_Stage;			// stage
		__u8		m_RtCode;			// return code

		__u32		m_TimeOut;		// keep timer

		__u32		m_SendReq;			// send request size
		__s32		m_SendLen;

		__s32		m_RecvLen;
		char		m_Buf[E_GrUpnpCliRecvBufLen];

		char		m_CgiHdBuf[E_GrUpnpCliCgiHeadMax];
		__u32		m_CgiHdLen;
		__u32		m_CgiNtStage;

		// parsing
		__u32		m_PrsPos;

		// line parsing
		char*		m_StrLine;
		__u32		m_LineLen;
		__u32		m_LinePrgs;

		// word parsing
		char*		m_StrWord;
		__u32		m_WordLen;

		__u32		m_TimeOutCnt;	//time out count
		//Def_StrTag		m_DevList[E_GrUpnpCliDevListCnt];

		__u32		m_DevType;
		__u32		m_DevCnt;
		St_GrUpnpCliDevCtl	m_DevTbl[E_GrUpnpCliDevMaxCnt];

		__u32		m_ReqCmd;
		__u32		m_ReqCnt;
		__u32		m_ReqDevIdx;
		St_GrUpnpCliReqItem	m_ReqTbl[E_GrUpnpCliReqMaxCnt];

		// runtime
		St_GrUpnpCliRuntime	m_Rtl;


		__u8	m_CliIdx;
		__u32	m_SvIdx;
		St_GrUpnpCliCliInfo		m_CliInfo[E_GrUpnpCliApplyCilCnt];
		St_NetCtrlSvInfo		m_SvTbl[E_GrUpnpCliMaxSvCnt];
		Ptr_NetCtrlSvInfo		m_PtrSv;

		__u32		m_ListCnt;
		__u8		m_FindIdx;
		BOOL8		m_IsAllDel;
		__u8		m_ErrCnt;

	protected:
		//local
		// misc
		__s32	LcDevFindByUuid(char* A_StrUuid);
		__s32	LcDevFindByIpV4(__u32 A_IpV4);

		// make message
		__u32	LcMakeDiscvMsgV4(void*	A_PtrBuf);	// make UDP discover message IPV4
		__u32	LcMakePortFwListMsgV4(__u32 A_DevIdx, __u32 A_EntryIdx);

		// parse message
		BOOL8	LcSubParsGetOneLine(void);

		BOOL8	LcSubParsWordSkipSpace(void);
		BOOL8	LcSubParsWordSkipHttp(void);
		BOOL8	LcSubParsWordLabel(void);
		BOOL8	LcSubParsWordAll(void);
		BOOL8	LcSubParsWordDblColon(void);
		BOOL8	LcSubParsWordIpAndPort(__u32* A_PtrRtIp, __u16* A_PtrRtPort);

		void	LcSubDiscCmdSt(void);
		void	LcSubDiscCmdUsn(void);
		void	LcSubDiscCmdLocation(void);
		void	LcParsDiscv(void);

		// stage work
		BOOL8	LcStageDiscoverV4(void);
		BOOL8	LcStageReadDiscvV4(void);

		BOOL8	LcStagePortFwPrepare(void);
		BOOL8	LcStagePortFwReqEntry(void);

		BOOL8	LcStageFinish(void);

		BOOL8	LcBkgStage(void);





		BOOL8	LcReqUpnpApply( void );
		void	LcUpdtClientTable( void );

		BOOL8	LcStageDnsUpdt( void );
		BOOL8	LcStageAddPortApply( void );
		BOOL8	LcStageAddPortPktSend( void );
		BOOL8	LcStageAddPortPktRecv( void );
		BOOL8	LcStageDelPortApply( void );
		BOOL8	LcStageDelPortPktSend( void );
		BOOL8	LcStageDelPortPktRecv( void );
		BOOL8	LcStageGetPortPktSend( void );
		BOOL8	LcStageGetPortPktRecv( void );

		BOOL8	LcTcpOpen( Ptr_NetCtrlSvInfo A_PtrSv );
		BOOL8	LcParseGetAnswer( char* A_SrBuf, BOOL8* A_PtrIsAck );
		//get server URL
		BOOL8	LcParseHttpUrl( char* A_SrBuf, Ptr_NetCtrlSvInfo A_PtrSv );	
		//set port
		void	LcMakeAddPortMsg( char*	A_SendBuf, Ptr_NetCtrlSvInfo A_PtrSv, Ptr_GrUpnpCliCliInfo A_CliInfo );
		//delete port
		void	LcMakeDelPortMsg( char* A_SendBuf, Ptr_NetCtrlSvInfo A_PtrSv, Ptr_GrUpnpCliCliInfo A_CliInfo );
		//get port info
		__u8	LcParseIsResigtPort( char* A_SrBuf );
		// get model info
		BOOL8	LcParseGetModel( char* A_SrBuf, Ptr_NetCtrlSvInfo A_PtrSv );
		__u32	LcMakeGetModelMsg( void* A_PtrBuf, Ptr_NetCtrlSvInfo A_PtrSv );
		BOOL8	LcStageGetModelInfo( void );
		BOOL8	LcStageGetModelPktSend( void );
		BOOL8	LcStageGetModelPktRecv( void );

		void	LcDbgDump( void* A_PtrBuf, __u32 A_Size );

		//inherited
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );												/* return = background processed */
		virtual	void	RtlTimer( __u32 A_Hndl );

	public:
		Cls_GrUpnpCli( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrUpnpCli( void );

		// 
		void	ImmInit(void);
		void	ImmFinish(void);


		//
		void	ReqInit(void);
		void	ReqFinish(void);


		
		BOOL8	ReqUpnpApply( void );
		BOOL8	ReqUpnpCancel( void );
		__u32	GetUpnpStage( void );
};

//====================================================================
//function


//====================================================================
//out Enviroment conditional
#endif
