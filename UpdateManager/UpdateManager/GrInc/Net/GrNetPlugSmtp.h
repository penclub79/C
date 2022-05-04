/*
	GAUSS Network SMTP plug-in

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrNetPlugSmtp
#define	_EPC_GrNetPlugSmtp

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskCli.h>

#include <GrSockV2.h>
#include <Net/GrNetPlugBase.h>

#ifndef E_GrNetPlugSmtpSslNot
#ifdef	LINUX_APP
	#include <openssl/ssl.h>
#endif
#endif

//====================================================================
//constance
#define	E_GrNetPlugSmtpSslNot

#define	E_GrNetPlugSmtpTimerPeriod		1000

#define E_GrNetPlugSmtpStageTryConn		1
#define E_GrNetPlugSmtpStageWaitConn	2
#define E_GrNetPlugSmtpStageIo				3
#define E_GrNetPlugSmtpStageFinish		4
#define E_GrNetPlugSmtpStageSslInit		5
#define E_GrNetPlugSmtpStageSsl				6
#define E_GrNetPlugSmtpStageSslHello	7
#define E_GrNetPlugSmtpStageSslLogin	8
#define E_GrNetPlugSmtpStageSslId			9
#define E_GrNetPlugSmtpStageSslPass		10
#define E_GrNetPlugSmtpStageSslFrom		11
#define E_GrNetPlugSmtpStageSslTo			12
#define E_GrNetPlugSmtpStageSslData		13
#define E_GrNetPlugSmtpStageSslMail		14
#define E_GrNetPlugSmtpStageSslEnd		15

#define E_GrNetPlugSmtpContMaxSize		0x2000
#define E_GrNetPlugSmtpItemMaxSize		0x4000
#define E_GrNetPlugSmtpQueSize				0x20000

#define E_GrNetPlugSmtpDnsUpdtTime		(4 * 60 * 60)	/* 4 hour */

#define E_GrNetPlugSmtpMailTblCnt			10

//====================================================================
//global macro

//====================================================================
//global type
typedef struct St_GrNetPlugSmtpStageInfo
{
	__u32		Pos;
	__u32		Size;
} *Ptr_GrNetPlugSmtpStageInfo;

typedef struct St_GrNetPlugSmtpItemHd
{
	Def_GrSock	HndlSock;
	__u32	Sent;
	__u32	Size;
	__u8	Stage;
	__u8	_rsvd0;
	__u16	TimeOut;
	St_GrNetPlugSmtpStageInfo Info[E_GrNetPlugSmtpMailTblCnt];
}	*Ptr_GrNetPlugSmtpItemHd;

//====================================================================
//class
class	Cls_GrNetPlugSmtp	:	public Cls_GrNetPlugBase
{
	private:
		__u32	m_HndlTimer;

		__u8	m_MailQue[E_GrNetPlugSmtpQueSize];

		Def_StrTag	m_StrSvName;		// server name
		__u32	m_SvIp;		// server IP address
		__u16	m_SvPort;	// server port
		BOOL8	m_IsAuth;
		Def_StrShort	m_StrSvId;			// base 64 encoded
		Def_StrTag		m_StrSvPass;		// base 64 encoded

		__s32		m_HndlDns;
		__u32	m_DnsRstTimer;

		char	m_StrTmp[E_GrNetPlugSmtpContMaxSize];
		char	m_StrMail[E_GrNetPlugSmtpItemMaxSize];

		BOOL8	m_IsUseSSL;
#ifndef E_GrNetPlugSmtpSslNot
#ifdef	LINUX_APP
		SSL_CTX*    m_PtrCtx;
		SSL*        m_PtrSsl;
#endif
#endif
	protected:
		// local
		BOOL8	LcBgpDns( void );
		BOOL8	LcBgpItem( void );

		BOOL8	LcProcSsl();
		BOOL8	LcSmtpSend(Ptr_GrNetPlugSmtpItemHd A_PtrSmtpItm, __u32 A_SmtpPos);
		BOOL8	LcSmtpRecv(Ptr_GrNetPlugSmtpItemHd A_PtrSmtpItm, char* A_PtrStr);
		BOOL8	LcChkRecvStr(char* A_PtrStr, char* A_PtrCmpStr);

		// inherited
		virtual	BOOL8	RtlBkgProc(void);
		virtual	void	RtlTimer(__u32 A_Hndl);

	public:
		Cls_GrNetPlugSmtp(Cls_GrTaskSvr* A_TaskSvr, Cls_GrTaskCli* A_NetSvs, __u8 A_DevIdx, __u8 A_PlugIdx);
		virtual		~Cls_GrNetPlugSmtp();

		void	SmtpSetup(WCHAR* A_StrSvName, __u16 A_SvPort, WCHAR* A_StrId, WCHAR* A_StrPass, BOOL8 A_IsUseSSL);
		BOOL8	SmtpRequest(WCHAR* A_StrFrom, WCHAR* A_StrTo,
			WCHAR* A_StrSubject, WCHAR*	A_StrCont, void* A_PtrImg, __u32 A_ImgSize);

};

	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


