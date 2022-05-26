/*
	GAUSE Network DHCP plug-in

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrNetPlugDhcp
#define	_EPC_GrNetPlugDhcp

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskCli.h>

#include <GrSockV2.h>
#include <Net/GrNetPlugBase.h>
	
//====================================================================
//constance
#define E_GrNetPlugDhcpDhcpOptLen		312

#define	E_GrNetPlugDhcpTimerPeriod	1000

//====================================================================
//global macro

//====================================================================
//global type

//packet type address
typedef	struct St_GrNetPlugDhcpPktMsg 
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
	__u8		Options[E_GrNetPlugDhcpDhcpOptLen];			//312 - cookie
}	*Ptr_GrNetPlugDhcpPktMsg;

typedef	struct St_GrNetPlugDhcpPktUdp 
{
	St_GrSockIpHead			IpHd;
	St_GrSockUdpHead		UdpHd;
	St_GrNetPlugDhcpPktMsg	Msg;
}	*Ptr_GrNetPlugDhcpPktUdp;


//====================================================================
//class
class	Cls_GrNetPlugDhcp	:	public Cls_GrNetPlugBase
{
	protected:
		__u32			m_HndlTimer;

		// local
		__u8			m_DhcpStage;					//stage
		__u8			m_AdpIdx;							//adapter index
		__u8			m_TimeOutCnt;					//time out counter
		BOOL8			m_IsLeaseExp;					// lease time expand mode
		__u8			m_RetryCnt;						//retry count
		Def_GrSock	m_HndlSend;					//receive socket
		Def_GrSock	m_HndlRecv;					//send socket
		__u32			m_SendId;
		St_GrSockAddrPkt	m_RecvAddr;		//receive address
		St_GrSockAddrPkt	m_SendAddr;		//send address
		St_GrSockCsVtHead		m_CsVtHd;		//checksum virtual header
		St_GrNetPlugDhcpPktUdp	m_SendPkt;	//send packet
		__u64			m_MacAdr;
		St_GrNetPlugDhcpPktUdp	m_RecvPkt;	//receive packet
		__u32			m_RsltIp;
		__u32			m_RsltMask;
		__u32			m_RsltGate;
		__u32			m_RsltDns;
		__u32			m_RsltTime;
		__u32			m_SvrId;			//server ip
		__u32			m_LeaseLim;		//lease limit time

		// UDP
		__u32			m_SvUdpIp;		//received server UDP ip

		// lease manage
		__u32			m_SpendTime;	// lease spend time

		BOOL8	LcSendPkt( void );				//send packet
		BOOL8	LcRecvPkt( void );				//receive packet
		BOOL8	LcRecvUdpPkt( void );			//receive UDP packet
		BOOL8	LcOptAnalysis( void );		//option analysis
		void	LcFinish( void );					//finish work
		__u8	LcGetOptType( void );
		void	LcLeaseExp( void );

		void	LcStageDiscv( void );			//discover stage
		void	LcStageOffer( void );			//receive offer
		void	LcStageReq( void );				//send select
		void	LcStageAck( void );				//receive acknowledge
		void	LcStageExpReq( void );		//lease time expand request
		void	LcStageExpAck( void );		//expand acknowledge

		// inherited
		virtual	BOOL8	RtlBkgProc(void);											/* return = background processed */
		virtual	void	RtlTimer(__u32 A_Hndl);

	public:
		Cls_GrNetPlugDhcp(Cls_GrTaskSvr* A_TaskSvr, Cls_GrTaskCli* A_NetSvs, __u8 A_DevIdx, __u8 A_PlugIdx);
		virtual		~Cls_GrNetPlugDhcp();

		virtual	BOOL8	DhcpRequest(void);
		virtual	BOOL8	DhcpReceive(void);

};

	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


