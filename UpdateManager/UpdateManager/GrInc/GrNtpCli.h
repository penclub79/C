/*
	gause NTP client

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrNtpCli
#define	_EPC_GrNtpCli

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<GrSockV2.h>

//====================================================================
//const
#define	E_GrNtpCliTimerPeriod				1000

#define E_GrNtpCliErrRetryWait			60

#define	E_GrNtpCliPktLenMinNoMac		(12 * 4)	/* min header length */

#define	E_GrNtpCliMacLenMin					4					/* crypto_NAK */
#define E_GrNtpCliMd5LenMax					(5 * 4)		/* MD5 */
#define	E_GrNtpCliMacLenMax					(6 * 4)		/* SHA */

#define E_GrNtpCliDfltPort					123

// protocol value
#define E_GrNtpCliPrtcLiValNwr			0		/* no warning */
#define E_GrNtpCliPrtcLiValAdd			1		/*  */
#define E_GrNtpCliPrtcLiValDel			2		/*  */
#define E_GrNtpCliPrtcLiValNosync		3		/* overload */
#define E_GrNtpCliPrtcLiBit					6	
#define E_GrNtpCliPrtcLiMask				0x03	

#define E_GrNtpCliPrtcVnVal					4		/* version */
#define E_GrNtpCliPrtcVnBit					3
#define E_GrNtpCliPrtcVnMask				0x07

#define E_GrNtpCliPrtcMdValPas			2		/* passive */
#define E_GrNtpCliPrtcMdValCli			3		/* client */
#define E_GrNtpCliPrtcMdValSvr			4		/* server */
#define E_GrNtpCliPrtcMdValBrd			5		/* broadcast */
#define E_GrNtpCliPrtcMdBit					0
#define E_GrNtpCliPrtcMdMask				0x07

#define E_GrNtpCliPrtcStumDflt			0
#define E_GrNtpCliPrtcStumUnspec		16

#define E_GrNtpCliPrtcPollDflt			4
#define E_GrNtpCliPrtcPrecDflt			-6

#define E_GrNtpCliPrtcRtDelay				0x10000
#define E_GrNtpCliPrtcRtDisp				0x10000

#define E_GrNtpCliPrtcRefIdDflt			0
#define E_GrNtpCliPrtcRefIdDeath		Mac_GrMakeFcc('R','A','T','E')

#define E_GrNtpCliTimeAdder					0x83AA7E80

// stage
#define	E_GrNtpCliStageNone					0
#define	E_GrNtpCliStageDns					1
#define	E_GrNtpCliStageSend					2
#define	E_GrNtpCliStageRecv					3
#define	E_GrNtpCliStageFinish				4
#define	E_GrNtpCliStageWait					5

// message
#define	E_GrNtpCliMsgCmdInit				1		/* init - St_GrNtpCliMsgHd */
#define	E_GrNtpCliMsgCmdFinish			2		/* finish - St_GrNtpCliMsgHd */
#define	E_GrNtpCliMsgCmdSetup				3		/* setup - St_GrNtpCliMsgSetup */
#define	E_GrNtpCliMsgCmdRun					4		/* run - St_GrNtpCliMsgHd */
#define	E_GrNtpCliMsgCmdStop				5		/* stop - St_GrNtpCliMsgHd */
#define	E_GrNtpCliMsgCmdUpdateNow		6		/* update now - St_GrNtpCliMsgUpdateNow */

//====================================================================
//type
typedef struct St_GrNtpCliLfp
{
	__u32	Int;
	__u32	Frc;
}	*Ptr_GrNtpCliLfp;

typedef struct St_GrNtpCliPktNrm
{
	__u8	li_vn_mode;	/* peer leap indicator */
	__u8	stratum;		/* peer stratum */
	__u8	ppoll;		/* peer poll interval */
	__s8	precision;	/* peer clock precision */
	__u32	rootdelay;	/* round trip delay to primary source */
	__u32	rootdisp;	/* dispersion to primary source*/
	__u32	refid;		/* reference id */
	St_GrNtpCliLfp	reftime;	/* last update time */
	St_GrNtpCliLfp	org;		/* originate time stamp */
	St_GrNtpCliLfp	rec;		/* receive time stamp */
	St_GrNtpCliLfp	xmt;		/* transmit time stamp */

}	*Ptr_GrNtpCliPktNrm;

typedef struct St_GrNtpCliPktExt
{
	__u8	li_vn_mode;	/* peer leap indicator */
	__u8	stratum;		/* peer stratum */
	__u8	ppoll;		/* peer poll interval */
	__s8	precision;	/* peer clock precision */
	__u32	rootdelay;	/* round trip delay to primary source */
	__u32	rootdisp;	/* dispersion to primary source*/
	__u32	refid;		/* reference id */
	St_GrNtpCliLfp	reftime;	/* last update time */
	St_GrNtpCliLfp	org;		/* originate time stamp */
	St_GrNtpCliLfp	rec;		/* receive time stamp */
	St_GrNtpCliLfp	xmt;		/* transmit time stamp */

	/*
	* The length of the packet less MAC must be a multiple of 64
	* with an RSA modulus and Diffie-Hellman prime of 256 octets
	* and maximum host name of 128 octets, the maximum auto key
	* command is 152 octets and maximum auto key response is 460
	* octets. A packet can contain no more than one command and one
	* response, so the maximum total extension field length is 864
	* octets. But, to handle humungus certificates, the bank must
	* be broke.
	*/

	__u32	exten[1];	/* misused */
	__u8	mac[E_GrNtpCliMacLenMax]; /* mac */

}	*Ptr_GrNtpCliPktExt;

typedef struct St_GrNtpCliMsgHd
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrNtpCliMsgHd;

typedef struct St_GrNtpCliMsgSetup
{
	St_GrNtpCliMsgHd	Hd;
	char*	StrUrl;
	__u16	Port;
	__u16	_rsvd0;
	__s32	GmtSec;
	__u32	UpdtStepSec;
}	*Ptr_GrNtpCliMsgSetup;

typedef struct St_GrNtpCliMsgUpdateNow
{
	St_GrNtpCliMsgHd	Hd;
	BOOL8*	PtrRslt;
	BOOL8	IsNotify;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}* Ptr_GrNtpCliMsgUpdateNow;


//====================================================================
//class
class Cls_GrNtpCli	:	public Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32			m_MsgAloc;
		__u32			m_HndlTimer;

		__s32			m_Hdns;
		__u32			m_Ip;				// server ip
		__u16			m_Port;

		Def_StrTag	m_StrSvName;

		__s32			m_Gmt;		// sec unit
		__u32			m_UpdtStep;		// update step

		Def_GrSock	m_Hsock;

		BOOL8			m_IsNotify;
		__u8			m_Stage;
		__u8			m_TimeOut;
		__u32			m_WaitTime;

		Def_GrTime	m_TimeRslt;

		St_GrNtpCliPktNrm	m_PktSend;
		St_GrNtpCliPktExt	m_PktRecv;

	protected:
		//local
		BOOL8	LcStart(void);
		void	LcStop(void);

		// stage
		BOOL8	LcStageDns(void);
		BOOL8	LcStageSend(void);
		BOOL8	LcStageRecv(void);
		BOOL8	LcStageFinish(void);

		// local
		void	ImmInit(void);
		void	ImmFinish(void);
		void	ImmSetup(char* A_StrSvUrl, __s32 A_GmtSec, __u32 A_UpdtStepSec, __u16 A_Port);
		void	ImmRun(void);
		void	ImmStop(void);

		BOOL8	ImmUpdateNow(BOOL8 A_IsRsltNotify);

		//inherited
		virtual	void	RtlTimer( __u32 A_Hndl );
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );												/* return = background processed */
	public:
		Cls_GrNtpCli( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrNtpCli( void );

		void	ReqInit(void);
		void	ReqFinish(void);
		void	ReqSetup(char* A_StrSvUrl, __s32 A_GmtSec, __u32 A_UpdtStepSec, __u16 A_Port);
		void	ReqRun(void);
		void	ReqStop(void);
		BOOL8	ReqUpdateNow(BOOL8 A_IsRsltNotify);

};

//====================================================================
//function


//====================================================================
//out Enviroment conditional
#endif
