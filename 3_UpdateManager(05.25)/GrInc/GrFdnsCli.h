/*
	gause free dns client

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrFdnsCli
#define	_EPC_GrFdnsCli

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<GrSockHd.h>

//====================================================================
//const

#define E_GrFdnsCliStageNone					0
#define E_GrFdnsCliStageRtipDns				1
#define E_GrFdnsCliStageRtipTryConn		2
#define E_GrFdnsCliStageRtipWaitConn	3
#define E_GrFdnsCliStageRtipContents	4
#define E_GrFdnsCliStageRtipSend			5
#define E_GrFdnsCliStageRtipRecv			6
#define E_GrFdnsCliStageRtipEnd				7
#define E_GrFdnsCliStageStipDns				8
#define E_GrFdnsCliStageStipTryConn		9
#define E_GrFdnsCliStageStipWaitConn	10
#define E_GrFdnsCliStageStipContents	11
#define E_GrFdnsCliStageStipSend			12
#define E_GrFdnsCliStageStipRecv			13
#define E_GrFdnsCliStageStipEnd				14

#define E_GrFdnsCliHostDyndDns			0
#define E_GrFdnsCliHostNoIp					1

#define E_GrFdnsCliSendBufSize			1024
#define E_GrFdnsCliRecvBufSize			1024

//====================================================================
//type


//====================================================================
//class

class Cls_GrFdnsCli	:	public Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32			m_MsgAloc;
		__u32			m_HndlTime;

		//
		__u8			m_TryCnt;
		__u8			m_Stage;
		__u8			m_HostType;
		__u8			m_TimeOut;
		__u8			m_RcvTimeOut;	//recive timeout
		__u8			m_DevIdx;			//network device index
		Def_GrSock	m_HndlSock;

		Def_StrTag	m_StrId;
		Def_StrTag	m_StrPass;
		Def_StrTag	m_StrDomain;

		Def_StrName	m_StrMyIp;
		__s32				m_HndlDns;	//dns handle
		__u32			m_IpSvr;		//server ip


		__u32			m_SendSize;
		__u32			m_Sended;
		__u32			m_RecvSize;
		__u32			m_Recved;

		char			m_SendBuf[E_GrFdnsCliSendBufSize];
		char			m_RecvBuf[E_GrFdnsCliRecvBufSize];

	protected:
		//local
		void	LcFinish( void );
		BOOL8	LcStageRtipDns( void );			// router ip dns
		BOOL8	LcStageRtipTryConn( void );		// router ip try connect
		BOOL8	LcStageRtipWaitConn( void );
		BOOL8	LcStageRtipContets( void );
		BOOL8	LcStageRtipEnd( void );			// router ip recived
		BOOL8	LcStageStipDns( void );			// set ip dns
		BOOL8	LcStageStipTryConn( void );		// set ip connect
		BOOL8	LcStageStipWaitConn( void );
		BOOL8	LcStageStipContents( void );
		BOOL8	LcStageStipEnd( void );			// set ip result recive

		BOOL8	LcStageSend( void );				// data send
		BOOL8	LcStageRecv( void );				// data recive

		//message process
		BOOL8	LcReqUpdt(__u8 A_DevIdx, __u8	A_HostType, char* A_StrId, char* A_StrPass, char* A_StrDomain);

		//inherited
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */
		virtual	void	RtlTimer( __u32 A_Hndl );
	public:
		Cls_GrFdnsCli( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrFdnsCli();

		//interface
		BOOL8	ReqUpdate(__u8 A_DevIdx, __u8	A_HostType, char* A_StrId, char* A_StrPass, char* A_StrDomain);
};


//====================================================================
//function

//====================================================================
//out Enviroment conditional
#endif
