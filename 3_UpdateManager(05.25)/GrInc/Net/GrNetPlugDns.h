/*
	GAUSE Network DNS plug-in

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrNetPlugDns
#define	_EPC_GrNetPlugDns

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskCli.h>

#include <GrSockV2.h>
#include <Net/GrNetPlugBase.h>
	
//====================================================================
//constance
#define E_GrNetPlugDnsTimerPeriod		1000

#define E_GrNetPlugDnsRecvPktSize		1024
#define E_GrNetPlugDnsSendPktSize		128
#define E_GrNetPlugDnsMaxReqCnt			16

#define E_GrNetPlugDnsStageNone			0
#define E_GrNetPlugDnsStageTry			1
#define E_GrNetPlugDnsStageDone			2

#define E_GrNetPlugDnsServerCnt			2
#define E_GrNetPlugDnsRsltCnt				5

#define E_GrNetPlugDnsPort					53

#define E_GrNetPlugDnsFlagResponse	0x0080	/* this is response */
#define E_GrNetPlugDnsFlagAuthSvr		0x0004	/* authoritative server */
#define E_GrNetPlugDnsFlagTurncate	0x0002	/* truncate message */
#define E_GrNetPlugDnsFlagRcsDesire	0x0001	/* recursion desired */
#define E_GrNetPlugDnsFlagRcsAble		0x8000	/* recursion available */
#define E_GrNetPlugDnsFlagAhtuAns		0x2000	/* authority answer */
#define E_GrNetPlugDnsFlagTrueData	0x1000	/* authenticate data checked */

#define E_GrNetPlugDnsOpcodeMask		0x0078
#define E_GrNetPlugDnsOpcodeQuery		0x0000

#define E_GrNetPlugDnsAnsCodeMask		0x0F00
#define E_GrNetPlugDnsAnsNoError		0x0000	/* respond code - no error */
#define E_GrNetPlugDnsAnsRspSvFail	0x0100	/* respond code - server fail */
#define E_GrNetPlugDnsAnsRspNotImp	0x0400	/* respond code - not implemented */
#define E_GrNetPlugDnsAnsRspRefuse	0x0500	/* respond code - query refused */

#define E_GrNetPlugDnsTypeHostLb		0x00		/* host address */
#define E_GrNetPlugDnsTypeHostHb		0x01		
#define E_GrNetPlugDnsClassInLb			0x00		/* class internet */
#define E_GrNetPlugDnsClassInHb			0x01		/* class internet */

#define E_GrNetPlugDnsPktMinSize		12
#define E_GrNetPlugDnsSlotBitPos		12
#define E_GrNetPlugDnsSlotValMask		0x000F
#define E_GrNetPlugDnsSeqIdMask			0x0FFF

#define E_GrNetPlugDnsTimeOut				10

#define E_GrNetPlugDnsBadSlot				-1

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrNetPlugDnsItem 
{
	__u8	Stage;					// stage
	__u8	TimeOut;				// time out
	__u8	IpCnt;					// get ip count
	__u8	SendSize;				// send buffer size

	__u16	SeqId;					// sequence id

	__u8	ReqMap;					// request map
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	RsltTbl[E_GrNetPlugDnsRsltCnt];	// result ip
	__u8	SendBuf[E_GrNetPlugDnsSendPktSize];
}	*Ptr_GrNetPlugDnsItem;

typedef struct St_GrNetPlugDnsV4Head	// DNS protocol
{
	__u16	Id;
	__u16	Flag;
	__u16	QstCnt;		// request only
	__u16	AnsCnt;		// response only
	__u16	AuthRrs;	// not use
	__u16	AddRrs;		// not use
}	*Ptr_GrNetPlugDnsV4Head;

//====================================================================
//class
class	Cls_GrNetPlugDns	:	public Cls_GrNetPlugBase
{
	protected:
		// timer
		__u32		m_HndlTimer;

		// local
		__u32		m_SvIpTbl[E_GrNetPlugDnsServerCnt];

		BOOL8		m_IsRecursion;		// recursion request
		St_GrNetPlugDnsItem	m_Slot[E_GrNetPlugDnsMaxReqCnt];

		Def_GrSock	m_Sock;
		// receive
		__u8	m_RecvBuf[E_GrNetPlugDnsRecvPktSize];
		__u32	m_ParsPos;
		__u32	m_RecvSize;

		BOOL8	LcProcUdpRecv( void );
		BOOL8	LcStageRequest( __u8 A_Slot );

		void	LcSlotAddIp( __u8 A_Slot, __u32 A_Ip );

		void	LcParsSkipQuery( void );
		void	LcParsSkipName( void );
		void	LcParsAnswer( __u8 A_Slot );

		BOOL8	LcBldReqPacket( __u8 A_Slot, char* A_StrName );
		__u32	LcBldNameStr( void* A_PtrBuf, char* A_StrName );

		// inherited
		virtual	BOOL8	RtlBkgProc(void);											/* return = background processed */
		virtual	void	RtlTimer(__u32 A_Hndl);

	public:
		Cls_GrNetPlugDns(Cls_GrTaskSvr* A_TaskSvr, Cls_GrTaskCli* A_NetSvs, __u8 A_DevIdx, __u8 A_PlugIdx);
		virtual		~Cls_GrNetPlugDns();

		BOOL8	DnsIpSet(__u8 A_Idx, __u32 A_IpVal);
		__s32	DnsQueryRequest(char* A_StrName);
		BOOL8	DnsQueryGetResult(__s32 A_Hndl, void* A_PtrRtIp, __u8 A_ReqCnt, __u8* A_PtrRtCnt);
		void	DnsFree(__s32 A_Hndl);

};

	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


