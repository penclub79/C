/*
	GAUSS Network NTP

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrNetPlugNtp
#define	_EPC_GrNetPlugNtp

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskCli.h>

#include <GrSockV2.h>
#include <Net/GrNetPlugBase.h>
	
//====================================================================
//constance
#define	E_GrNetPlugNtpTimerPeriod			1000

#define	E_GrNetPlugNtpPktLenMinNoMac	(12 * 4)	/* min header length */

#define	E_GrNetPlugNtpMacLenMin				4					/* crypto_NAK */
#define E_GrNetPlugNtpMd5LenMax				(5 * 4)		/* MD5 */
#define	E_GrNetPlugNtpMacLenMax				(6 * 4)		/* SHA */

#define E_GrNetPlugNtpPort						123

#define E_GrNetPlugNtpReqMax					8
#define E_GrNetPlugNtpDnsUpdtPeriod		4

// protocol value
#define E_GrNetPlugNtpPrtcLiValNwr		0		/* no warning */
#define E_GrNetPlugNtpPrtcLiValAdd		1		/*  */
#define E_GrNetPlugNtpPrtcLiValDel		2		/*  */
#define E_GrNetPlugNtpPrtcLiValNosync	3		/* overload */
#define E_GrNetPlugNtpPrtcLiBit				6	
#define E_GrNetPlugNtpPrtcLiMask			0x03	

#define E_GrNetPlugNtpPrtcVnVal				4		/* version */
#define E_GrNetPlugNtpPrtcVnBit				3
#define E_GrNetPlugNtpPrtcVnMask			0x07

#define E_GrNetPlugNtpPrtcMdValPas		2		/* passive */
#define E_GrNetPlugNtpPrtcMdValCli		3		/* client */
#define E_GrNetPlugNtpPrtcMdValSvr		4		/* server */
#define E_GrNetPlugNtpPrtcMdValBrd		5		/* broadcast */
#define E_GrNetPlugNtpPrtcMdBit				0
#define E_GrNetPlugNtpPrtcMdMask			0x07

#define E_GrNetPlugNtpPrtcStumDflt		0
#define E_GrNetPlugNtpPrtcStumUnspec	16

#define E_GrNetPlugNtpPrtcPollDflt		4
#define E_GrNetPlugNtpPrtcPrecDflt		-6

#define E_GrNetPlugNtpPrtcRtDelay			0x10000
#define E_GrNetPlugNtpPrtcRtDisp			0x10000

#define E_GrNetPlugNtpPrtcRefIdDflt		0
#define E_GrNetPlugNtpPrtcRefIdDeath	Mac_GrMakeFcc('R','A','T','E')

#define E_GrNetPlugNtpTimeAdder				0x83AA7E80


// stage
#define	E_GrNetPlugNtpStageNone				0
#define	E_GrNetPlugNtpStageDns				1
#define	E_GrNetPlugNtpStageSend				2
#define	E_GrNetPlugNtpStageRecv				3
#define	E_GrNetPlugNtpStageFinish			4

// request status
#define E_GrNetPlugNtpReqStatNone			0
#define E_GrNetPlugNtpReqStatWait			1
#define E_GrNetPlugNtpReqStatRecv			2
#define E_GrNetPlugNtpReqStatFail			3

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrNetPlugNtpLfp
{
	__u32	Int;
	__u32	Frc;
}	*Ptr_GrNetPlugNtpLfp;

typedef struct St_GrNetPlugNtpPktNrm 
{
	__u8	li_vn_mode;	/* peer leap indicator */
	__u8	stratum;		/* peer stratum */
	__u8	ppoll;		/* peer poll interval */
	__s8	precision;	/* peer clock precision */
	__u32	rootdelay;	/* round trip delay to primary source */
	__u32	rootdisp;	/* dispersion to primary source*/
	__u32	refid;		/* reference id */
	St_GrNetPlugNtpLfp	reftime;	/* last update time */
	St_GrNetPlugNtpLfp	org;		/* originate time stamp */
	St_GrNetPlugNtpLfp	rec;		/* receive time stamp */
	St_GrNetPlugNtpLfp	xmt;		/* transmit time stamp */

}	*Ptr_GrNetPlugNtpPktNrm;

typedef struct St_GrNetPlugNtpPktExt
{
	__u8	li_vn_mode;	/* peer leap indicator */
	__u8	stratum;		/* peer stratum */
	__u8	ppoll;		/* peer poll interval */
	__s8	precision;	/* peer clock precision */
	__u32	rootdelay;	/* round trip delay to primary source */
	__u32	rootdisp;	/* dispersion to primary source*/
	__u32	refid;		/* reference id */
	St_GrNetPlugNtpLfp	reftime;	/* last update time */
	St_GrNetPlugNtpLfp	org;		/* originate time stamp */
	St_GrNetPlugNtpLfp	rec;		/* receive time stamp */
	St_GrNetPlugNtpLfp	xmt;		/* transmit time stamp */

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
	__u8	mac[E_GrNetPlugNtpMacLenMax]; /* mac */

}	*Ptr_GrNetPlugNtpPktExt;

typedef struct St_GrNetPlugNtpReq
{
	__u8	Stat;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Def_GrTime	RecvTime;
}	*Ptr_GrNetPlugNtpReq;

//====================================================================
//class
class	Cls_GrNetPlugNtp	:	public Cls_GrNetPlugBase
{
	protected:
		__u32		m_HndlTimer;

		__s32		m_Hdns;
		__u32	m_Ip;
		Def_StrTag	m_StrSvName;
		__u8	m_DnsUpdtPeriod;

		__s32		m_Gmt;

		Def_GrSock	m_Hsock;

		__u8	m_Stage;
		__u8	m_TimeOut;

		St_GrNetPlugNtpPktNrm	m_PktSend;
		St_GrNetPlugNtpPktExt	m_PktRecv;

		St_GrNetPlugNtpReq	m_ReqTbl[E_GrNetPlugNtpReqMax];

		// local
		void	LcStart( void );
		void	LcRsltFail( void );
		void	LcTmrStart( void );

		// stage
		BOOL8	LcStageDns( void );
		BOOL8	LcStageSend( void );
		BOOL8	LcStageRecv( void );
		BOOL8	LcStageFinish( void );

		// inherited
		virtual	BOOL8	RtlBkgProc(void);
		virtual	void	RtlTimer(__u32 A_Hndl);

	public:
		Cls_GrNetPlugNtp(Cls_GrTaskSvr* A_TaskSvr, Cls_GrTaskCli* A_NetSvs, __u8 A_DevIdx, __u8 A_PlugIdx);
		virtual		~Cls_GrNetPlugNtp();

		void	NtpSetup(WCHAR* A_StrSvName, __s32 A_Gmt);
		__s32	NtpRequest(void);
		BOOL8	NtpTimeGet(__s32 A_Hndl, Def_GrTime* A_PtrRtTime);
		void	NtpFree(__s32 A_Hndl);

};

	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


