/*
	GAUSS Network service

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrNetSvs
#define	_EPC_GrNetSvs

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <GrSockV2.h>
#include <Net/GrNetPlugBase.h>
#include <Dvr/GrDvrBase.h>
	
//====================================================================
//constance

#define E_GrNetSvsMaxPluginCnt		8

#define	E_GrNetSvsPlugUser				5

#define	E_GrNetSvsPlugDhcp				0
#define	E_GrNetSvsPlugDns					1
#define	E_GrNetSvsPlugSmtp				2
#define	E_GrNetSvsPlugNtp					4

#define E_GrNetSvsTimerPeriod			1000

#define E_GrNetSvsStageNone				0		/* normal mode */
#define E_GrNetSvsStageIpPrepare	1		/* IP change prepare */
#define E_GrNetSvsStageDhcpReq		2		/* DHCP request */
#define E_GrNetSvsStageDhcpRecv		3		/* DHCP receive */
#define E_GrNetSvsStageIpChg			4		/* IP change */

#define E_GrNetSvsStageOff				0xFF	/* off */

#define E_GrNetSvsDnsRsltWait			0		/* DNS waiting */
#define E_GrNetSvsDnsRsltOk				1		/* DNS receive ok */
#define E_GrNetSvsDnsRsltErr			2		/* DNS error */

// message command
#define E_GrNetSvsMsgPlugRegist		1		/* plugin regist	:	St_GrNetSvsMsgPlugRegist */
#define E_GrNetSvsMsgPlugUnregist	2		/* plugin unregist	:	St_GrNetSvsMsgPlugUnregist */

#define E_GrNetSvsMsgDnsReq				3		/* DNS request	:	St_GrNetSvsMsgDnsReq */
#define E_GrNetSvsMsgDnsResult		4		/* DNS get result	:	St_GrNetSvsMsgDnsResult */
#define E_GrNetSvsMsgDnsFree			5		/* DNS free	:	St_GrNetSvsMsgDnsFree */

#define E_GrNetSvsMsgSmtpSetup		6		/* SMTP setup	:	St_GrNetSvsMsgSmtpSetup */
#define E_GrNetSvsMsgSmtpReq			7		/* SMTP request	:	St_GrNetSvsMsgSmtpReq */

#define E_GrNetSvsMsgNtpSetup			8		/* NTP setup	:	St_GrNetSvsMsgNtpSetup */
#define E_GrNetSvsMsgNtpReq				9		/* NTP request	:	St_GrNetSvsMsgNtpReq */
#define E_GrNetSvsMsgNtpGet				10	/* NTP time get	:	St_GrNetSvsMsgNtpGet */
#define E_GrNetSvsMsgNtpFree			11	/* NTP free	:	St_GrNetSvsMsgNtpFree */


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrNetSvsMsgBase 
{
	St_GrTaskMsgHd	Hd;
	__u32	Cmd;
}	*Ptr_GrNetSvsMsgBase;

typedef struct St_GrNetSvsMsgPlugRegist
{
	St_GrNetSvsMsgBase	Base;
	BOOL8*	PtrRslt;
	Cls_GrNetPlugBase* ObjPlugin;
	__u8	PlugIdx;
}	*Ptr_GrNetSvsMsgPlugRegist;

typedef struct St_GrNetSvsMsgPlugUnregist
{
	St_GrNetSvsMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	PlugIdx;
}	*Ptr_GrNetSvsMsgPlugUnregist;


typedef struct St_GrNetSvsMsgDnsReq 
{
	St_GrNetSvsMsgBase	Base;
	__s32*	PtrRslt;
	char*	StrName;
	__u8	SvCnt;
	BOOL8	IsV6;
}	*Ptr_GrNetSvsMsgDnsReq;

typedef struct  St_GrNetSvsMsgDnsResult
{
	St_GrNetSvsMsgBase	Base;
	__u8*	PtrRslt;
	__s32		Hndl;
	void*	PtrRtIp;
	BOOL8	IsV6;
}	*Ptr_GrNetSvsMsgDnsResult;

typedef struct  St_GrNetSvsMsgDnsFree
{
	St_GrNetSvsMsgBase	Base;
	__s32		Hndl;
	BOOL8	IsV6;
}	*Ptr_GrNetSvsMsgDnsFree;

typedef struct St_GrNetSvsMsgSmtpSetup
{
	St_GrNetSvsMsgBase	Base;
	WCHAR*	StrSvName;
	__u16		SvPort;
	WCHAR*	StrId;
	WCHAR*	StrPass;
}	*Ptr_GrNetSvsMsgSmtpSetup;

typedef struct St_GrNetSvsMsgSmtpReq
{
	St_GrNetSvsMsgBase	Base;
	BOOL8*	PtrRslt;
	WCHAR*	StrFrom;
	WCHAR*	StrTo;
	WCHAR*	StrSubject;
	WCHAR*	StrCont;
	void*		PtrImg;
	__u32		ImgSize;
}	*Ptr_GrNetSvsMsgSmtpReq;

typedef struct St_GrNetSvsMsgNtpSetup
{
	St_GrNetSvsMsgBase	Base;
	WCHAR*	StrSvName;
	__s32		Gmt;
}	*Ptr_GrNetSvsMsgNtpSetup;

typedef struct St_GrNetSvsMsgNtpReq
{
	St_GrNetSvsMsgBase	Base;
	__s32*	PtrRslt;
}	*Ptr_GrNetSvsMsgNtpReq;

typedef struct St_GrNetSvsMsgNtpGet
{
	St_GrNetSvsMsgBase	Base;
	BOOL8*	PtrRslt;
	__s32			Hndl;
	Def_GrTime*	PtrRtTime;
}	*Ptr_GrNetSvsMsgNtpGet;

typedef struct St_GrNetSvsMsgNtpFree
{
	St_GrNetSvsMsgBase	Base;
	__s32			Hndl;
}	*Ptr_GrNetSvsMsgNtpFree;

//====================================================================
//class
class	Cls_GrNetSvs	:	public	Cls_GrTaskCli
{
	protected:
		// property
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32		m_MsgAloc;
		__u32		m_HndlTimer;

		__u8		m_InitLvl;			// init completed level

		__u8		m_DevIdx;				// network device index
		BOOL8		m_IsIpV4Req;
		BOOL8		m_IsIpV6Req;

		__u32		m_MyV4Ip;				// ip
		__u32		m_MyV4GateWay;	// gate way
		__u32		m_MyV4NetMask;	// net mask
		__u32		m_MyV4Dns;			// DNS

		BOOL8		m_IsDhcpUse;		// DHCP use

		__u8		m_Stage;
		BOOL8		m_IsLink;				// linked

		Cls_GrNetPlugBase*	m_PlugInTbl[E_GrNetSvsMaxPluginCnt];

		// local
		void	LcLinkUpdt( void );

		void	LcMprocPlugRegist(Ptr_GrNetSvsMsgPlugRegist A_PtrMsg);
		void	LcMprocPlugUnregist(Ptr_GrNetSvsMsgPlugUnregist A_PtrMsg);

		virtual	void	StageIpPrep( void );
		virtual	void	StageDhcpReq( void );
		virtual	void	StageDhcpRecv( void );
		virtual	void	StageIpChg( void );

		// inherited
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */
		virtual	void	RtlTimer( __u32 A_Hndl );
	public:
		Cls_GrNetSvs( Cls_GrTaskSvr* A_TaskSvr, __u8 A_DevIdx );
		virtual		~Cls_GrNetSvs();

		// status
		__u8	NetIfIdxGet(void);

		// DNS
		virtual	__s32	ImmDnsReq( char* A_StrName, __u8 A_SvCnt, BOOL8 A_IsV6 );
		virtual	BOOL8	ImmDnsResult( __s32 A_Hndl, void* A_PtrRtIp, __u8 A_ReqCnt, __u8* A_PtrRtCnt, BOOL8 A_IsV6 );
		virtual	void	ImmDnsFree( __s32 A_Hndl, BOOL8 A_IsV6 );

		__s32		SafeDnsReq( char* A_StrName, __u8 A_SvCnt, BOOL8 A_IsV6 );
		BOOL8	SafeDnsResult( __s32 A_Hndl, void* A_PtrRtIp, __u8 A_ReqCnt, __u8* A_PtrRtCnt, BOOL8 A_IsV6 );
		void	SafeDnsFree( __s32 A_Hndl, BOOL8 A_IsV6 );

		// SMTP
		virtual	void	ImmSmtpSetup( WCHAR* A_StrSvName, __u16 A_SvPort, WCHAR* A_StrId, WCHAR* A_StrPass, BOOL8 A_IsUseSSL );
		virtual	BOOL8	ImmSmtpReq( WCHAR* A_StrFrom, WCHAR* A_StrTo, 
			WCHAR* A_StrSubject, WCHAR*	A_StrCont, void* A_PtrImg, __u32 A_ImgSize );

		void	SafeSmtpSetup( WCHAR* A_StrSvName, __u16 A_SvPort, WCHAR* A_StrId, WCHAR* A_StrPass );
		BOOL8	SafeSmtpReq( WCHAR* A_StrFrom, WCHAR* A_StrTo, 
			WCHAR* A_StrSubject, WCHAR*	A_StrCont, void* A_PtrImg, __u32 A_ImgSize );

		// NTP
		void	ImmNtpSetup( WCHAR* A_StrSvName, __s32 A_Gmt );
		__s32		ImmNtpRequest( void );
		BOOL8	ImmNtpTimeGet( __s32 A_Hndl, Def_GrTime* A_PtrRtTime );
		void	ImmNtpFree( __s32 A_Hndl );

		void	SafeNtpSetup( WCHAR* A_StrSvName, __s32 A_Gmt );
		__s32		SafeNtpRequest( void );
		BOOL8	SafeNtpTimeGet( __s32 A_Hndl, Def_GrTime* A_PtrRtTime );
		void	SafeNtpFree( __s32 A_Hndl );

		// runtime
		BOOL8	RtlPluginRegist(Cls_GrNetPlugBase* A_ObjPlugin, __u8 A_PlugIdx);
		void	RtlPluginUnregist(__u8 A_PlugIdx);

		void	RtlV4NetAdrSet( __u32 A_Ip, __u32 A_Mask, __u32 A_Gate );

};

	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


