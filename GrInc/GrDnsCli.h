/*
	gause dns client

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDnsCli
#define	_EPC_GrDnsCli

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>

#include <GrSocket.h>

//====================================================================
//const
#define E_GrDnsCliTimerPriod			1000

#define E_GrDnsCliRecvPktSize			1024
#define E_GrDnsCliSendPktSize			128
#define E_GrDnsCliMaxItemCnt			16

#define E_GrDnsCliStageNone			0
#define E_GrDnsCliStageTry			1
#define E_GrDnsCliStageDone			2

#define E_GrDnsCliRsltCnt				5

#define E_GrDnsCliPort					53

#define E_GrDnsCliFlagResponse	0x0080	/* this is response */
#define E_GrDnsCliFlagAuthSvr		0x0004	/* authoritative server */
#define E_GrDnsCliFlagTurncate	0x0002	/* truncate message */
#define E_GrDnsCliFlagRcsDesire	0x0001	/* recursion desired */
#define E_GrDnsCliFlagRcsAble		0x8000	/* recursion available */
#define E_GrDnsCliFlagAhtuAns		0x2000	/* authority answer */
#define E_GrDnsCliFlagTrueData	0x1000	/* authenticate data checked */

#define E_GrDnsCliOpcodeMask		0x0078
#define E_GrDnsCliOpcodeQuery		0x0000

#define E_GrDnsCliAnsCodeMask		0x0F00
#define E_GrDnsCliAnsNoError		0x0000	/* respond code - no error */
#define E_GrDnsCliAnsRspSvFail	0x0100	/* respond code - server fail */
#define E_GrDnsCliAnsRspNotImp	0x0400	/* respond code - not implemented */
#define E_GrDnsCliAnsRspRefuse	0x0500	/* respond code - query refused */

#define E_GrDnsCliTypeHostLb		0x00		/* host address */
#define E_GrDnsCliTypeHostHb		0x01		
#define E_GrDnsCliClassInLb			0x00		/* class internet */
#define E_GrDnsCliClassInHb			0x01		/* class internet */

#define E_GrDnsCliPktMinSize		12

#define E_GrDnsCliSlotBitPos		12
#define E_GrDnsCliSlotValMask		0x000F
#define E_GrDnsCliSeqIdMask			0x0FFF

#define E_GrDnsCliHostAddrField		1					/* host address field */
#define E_GrDnsCliArpaIntnField		1					/* ARPA internet field */

#define E_GrDnsCliTimeOut				10

#define E_GrDnsCliBadSlot				-1

//message command
#define E_GrDnsCliMsgInit					1					/* init - St_GrDnsCliMsgHd */
#define E_GrDnsCliMsgFinish				2					/* finish - St_GrDnsCliMsgHd */
#define E_GrDnsCliMsgSetup				3					/* setup - St_GrDnsCliMsgSetup */
#define E_GrDnsCliMsgOpen					4					/* open - St_GrDnsCliMsgOpen */
#define E_GrDnsCliMsgRead					5					/* read - St_GrDnsCliMsgRead */
#define E_GrDnsCliMsgClose				6					/* close - St_GrDnsCliMsgClose */

//====================================================================
//type

typedef	struct St_GrDnsCliItem 
{
	__u8	Stage;					// stage
	__u8	TimeOut;				// time out
	__u8	IpCnt;					// get ip count
	__u8	_rsvd0;

	__u16	SeqId;					// sequence id
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	RsltTbl[E_GrDnsCliRsltCnt];	// result ip
}	*Ptr_GrDnsCliItem;

typedef struct St_GrDnsCliV4Head	// DNS protocol
{
	__u16	Id;
	__u16	Flag;
	__u16	QstCnt;		// request only
	__u16	AnsCnt;		// response only
	__u16	AuthRrs;	// not use
	__u16	AddRrs;		// not use
}	*Ptr_GrDnsCliV4Head;

typedef	struct St_GrDnsCliMsgHd 
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrDnsCliMsgHd;

typedef	struct St_GrDnsCliMsgSetup 
{
	St_GrDnsCliMsgHd	Hd;
	__u32	DnsIp;
}	*Ptr_GrDnsCliMsgSetup;

typedef	struct St_GrDnsCliMsgOpen
{
	St_GrDnsCliMsgHd	Hd;
	char*	StrName;
	__s32*	PtrRt;
}	*Ptr_GrDnsCliMsgOpen;

typedef	struct St_GrDnsCliMsgClose
{
	St_GrDnsCliMsgHd	Hd;
	__u8*	PtrRtRslt;
	__s32		Hndl;
}	*Ptr_GrDnsCliMsgClose;

typedef	struct St_GrDnsCliMsgRead
{
	St_GrDnsCliMsgHd	Hd;
	__u8*	PtrRtRslt;
	__s32	Hndl;
	__u8	MaxCnt;			// init by user
	void*	PtrRtIp;
	__u8*	PtrRtCnt;
}	*Ptr_GrDnsCliMsgRead;



//====================================================================
//class

class Cls_GrDnsCli	:	public Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32			m_MsgAloc;

		__u32			m_HndlTime;

		__u32			m_SvIpV4;

		BOOL8		m_IsRecursion;		// recursion request

		__s32		m_ItemCnt;
		__s32		m_RunCnt;					// standby item count
		St_GrDnsCliItem	m_ItemTbl[E_GrDnsCliMaxItemCnt];

		Def_GrSock	m_Sock;

		// send
		__u8	m_SendBuf[E_GrDnsCliSendPktSize];

		// receive
		__u8	m_RecvBuf[E_GrDnsCliRecvPktSize];
		__u32	m_ParsPos;
		__u32	m_RecvSize;

	protected:

		BOOL8	LcProcUdpRecv( void );

		__u32	LcBldReqPacket(char* A_StrName,__u16 A_SeqId);
		__u32	LcBldNameStr(void* A_PtrBuf, char* A_StrName);

		void	LcSlotAddIp( __u8 A_Slot, __u32 A_Ip );

		void	LcParsSkipQuery( void );
		void	LcParsSkipName( void );
		void	LcParsAnswer( __u8 A_Slot );

		// 
		void	ImmInit(void);
		void	ImmFinish(void);
		void	ImmSetup(__u32 A_DnsIp);
		__s32	ImmOpen(char* A_StrName);
		__u8	ImmRead(__s32 A_Hndl, void* A_PtrRtIp, __u8 A_MaxCnt, __u8* A_PtrRtCnt);
		__u8	ImmClose(__s32 A_Hndl);

		//inherited
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */
		virtual	void	RtlTimer( __u32 A_Hndl );
	public:
		Cls_GrDnsCli( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrDnsCli();

		//interface
		void	ReqInit(void);
		void	ReqFinish(void);
		void	ReqSetup(__u32 A_DnsIp);
		__s32	ReqOpen(char* A_StrDns);
		__u8	ReqRead(__s32 A_Hndl, void* A_PtrRtIp, __u8 A_MaxCnt, __u8* A_PtrRtCnt);
		__u8	ReqClose(__s32 A_Hndl);
		__u32	ReqDnsIpGet(void);
};


//====================================================================
//function

//====================================================================
//out Enviroment conditional
#endif
