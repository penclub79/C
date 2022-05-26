/*
	gause dhcp client

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDhcpCli
#define	_EPC_GrDhcpCli

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<GrSockHd.h>

//====================================================================
//const
#define E_GrDhcpCliDhcpOptLen				312

#define E_GrDhcpCliTimeout					10		/* time out time */
#define E_GrDhcpCliRetry						3			/* retry count */
#define E_GrDhcpCliTimerPriod				1000	/* sec timer */

#define	E_GrDhcpCliRetryWait				60

#define E_GrDhcpCliStageNone				0			/* not work or failure stage */
#define E_GrDhcpCliStageDiscv				1			/* discover stage */
#define E_GrDhcpCliStageOffer				2			/* wait offer stage */
#define E_GrDhcpCliStageReq					3			/* request stage */
#define E_GrDhcpCliStageAck					4			/* wait ack stage */
#define	E_GrDhcpCliStageWaitLease		5			/* wait lease expired */
#define	E_GrDhcpCliStageRetryWait		6			/* wait retry time */
#define E_GrDhcpCliStageExpReq			7			/* lease time expand request - maybe not use */
#define E_GrDhcpCliStageExpAck			8			/* lease time expand acknologe - maybe not use */

#define E_GrDhcpCliSockPrtcIp				0x08	/* protocol type ip */
#define E_GrDhcpCliMacAdrLen				6
#define E_GrDhcpCliPortServer				67
#define E_GrDhcpCliPortClient				68

#define E_GrDhcpCliMsgTypeRequest		1
#define E_GrDhcpCliMsgTypeReply			2

//#define E_GrDhcpCliMsgFcc						0x69596930
#define E_GrDhcpCliMsgFcc						0x63538263

#define E_GrDhcpCliOptCodePadding			0x00
#define E_GrDhcpCliOptCodeNetMask			0x01
#define E_GrDhcpCliOptCodeRouter			0x03
#define E_GrDhcpCliOptCodeDnsServer		0x06
#define E_GrDhcpCliOptCodeHostName		0x0C	/* my name */
#define E_GrDhcpCliOptCodeIpAddr			0x32
#define E_GrDhcpCliOptCodeLeaseTm			0x33
#define E_GrDhcpCliOptCodeOptOver			0x34
#define E_GrDhcpCliOptCodeMsgType			0x35
#define E_GrDhcpCliOptCodeServerId		0x36
#define E_GrDhcpCliOptCodeReqList			0x37
#define E_GrDhcpCliOptCodeMaxMsgSize	0x39		
#define E_GrDhcpCliOptCodeClassId			0x3C		
#define E_GrDhcpCliOptCodeCliId				0x3D		
#define E_GrDhcpCliOptCodeEnd					0xFF


#define E_GrDhcpCliOptTypeDiscover	1
#define E_GrDhcpCliOptTypeOffer			2
#define E_GrDhcpCliOptTypeRequest		3
#define E_GrDhcpCliOptTypeAck				5
#define E_GrDhcpCliOptTypeNack			6

#define E_GrDhcpCliOptOfsCode				0
#define E_GrDhcpCliOptOfsLen				1

#define E_GrDhcpCliOptFldOpt				0
#define E_GrDhcpCliOptFldFile				1
#define E_GrDhcpCliOptFldSvName			2

//#define	E_GrDhcpCliRcvUdp							//recive udp mode

#define	E_GrDhcpCliDbgMsg

//message command

#define E_GrDhcpCliCmdInit				1		/* run : St_GrDhcpCliMsgHd */
#define E_GrDhcpCliCmdFinish			2		/* run : St_GrDhcpCliMsgHd */
#define E_GrDhcpCliCmdSetup				3		/* run : St_GrDhcpCliMsgSetup */
#define E_GrDhcpCliCmdRun					4		/* run : St_GrDhcpCliMsgHd */
#define E_GrDhcpCliCmdStop				5		/* stop : St_GrDhcpCliMsgHd */

//====================================================================
//type

//packet type address
typedef	struct St_GrDhcpCliPktMsg 
{
	__u8		MsgType;			//message type
	__u8		HwType;				//hardware address type
	__u8		HwLen;				//hardware address length
	__u8		HwZeroCnt;		//should be zero in client message
	__u32		SendId;				//transaction id
	__u16		Sec;					//elapsed time in sec. from boot
	__u16		Flags;		
	__u32		ClientAddr;		//client IP
	__u32		ReqAddr;			//request ip address
	__u32		Siaddr;				//should be zero in client's messages
	__u32		Giaddr;				//should be zero in client's messages
	__u8		MacAddr[16];	//mac-address
	__u8		SvName[64];		//server host name
	__u8		BootName[128];
	__u32		Cookie;
	
}	*Ptr_GrDhcpCliPktMsg;

typedef	struct St_GrDhcpCliPktUdp 
{
	St_GrSockIpHead			IpHd;
	St_GrSockUdpHead		UdpHd;
	St_GrDhcpCliPktMsg	Msg;
	__u8		Options[E_GrDhcpCliDhcpOptLen];			//312 - cookie
}	*Ptr_GrDhcpCliPktUdp;

typedef	struct St_GrDhcpCliMsgHd
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrDhcpCliMsgHd;

typedef struct St_GrDhcpCliMsgSetup
{
	St_GrDhcpCliMsgHd	Hd;
	__u8	NetIf;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrDhcpCliMsgSetup;

//====================================================================
//class

class Cls_GrDhcpCli	:	public Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32			m_MsgAloc;

		__u32			m_HndlTime;

		BOOL8			m_IsDbgMode;

		//dhcp
		__u8			m_DhcpStage;					//stage
		__u8			m_NetIf;							//net interface
		__u8			m_AdpIdx;							//adaptor index
		__u8			m_TimeOutCnt;					//time out counter
		Def_GrSock	m_HndlSend;					//recive socket
		Def_GrSock	m_HndlRecv;					//send socket
		__u32			m_SendId;
		St_GrSockAddrPkt		m_RecvAddr;		//recive address
		St_GrSockAddrPkt		m_SendAddr;		//send address
		St_GrSockCsVtHead		m_CsVtHd;		//checksum virtual header
		St_GrDhcpCliPktUdp	m_SendPkt;	//send packet
		__u64			m_MacAdr;
		St_GrDhcpCliPktUdp	m_RecvPkt;	//recive packet
		__u32			m_RsltIp;
		__u32			m_RsltMask;
		__u32			m_RsltGate;
		__u32			m_RsltDns;
		__u32			m_RsltTime;
		__u32			m_SvrId;			//server ip
		__u32			m_LeaseLim;		//lease limite time

		//udp
		__u32			m_SvUdpIp;		//recived server udp ip

		// lease manage
		__u32			m_SpendTime;	// lease spend time
		__u32			m_RetryWait;	// retry wait
				
	protected:
		//local
		BOOL8	LcSendPkt( __u32 A_OptSize );				//send pacekt
		BOOL8	LcRecvPkt( void );				//recive packet
		BOOL8	LcRecvUdpPkt( void );			//recive udp packet
		BOOL8	LcOptAnalysis( void );		//option analysis
		void	LcFinish( void );					//finish work
		__u8	LcGetOptType( void );

		void	LcStageDiscv( void );			//discorver stage
		void	LcStageOffer( void );			//recive offer
		void	LcStageReq( void );				//send select
		void	LcStageAck( void );				//recive ack
		void	LcStageExpReq( void );		//lease time expand request
		void	LcStageExpAck( void );		//expand ack

		// local function
		BOOL8	LcRequest( void );
		BOOL8	LcLeaseExp( void );

		void	ImmInit(void);
		void	ImmFinish(void);
		void	ImmSetup(__u8 A_NetIf);
		void	ImmRun(void);
		void	ImmStop(void);

		//inherited
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */
		virtual	void	RtlTimer( __u32 A_Hndl );
	public:
		Cls_GrDhcpCli( Cls_GrTaskSvr* A_TaskSvr, BOOL8 A_IsDbgMode );
		virtual	~Cls_GrDhcpCli();

		//interface
		void	ReqInit(void);
		void	ReqFinish(void);
		void	ReqSetup(__u8 A_NetIf);
		void	ReqRun( void );
		void	ReqStop( void );
};


//====================================================================
//function

//====================================================================
//out Enviroment conditional
#endif
