/*
	gause Network base

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrNetBase
#define	_EPC_GrNetBase

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<GrSockV2.h>

//====================================================================
//const

#define	E_GrNetThrdFcc			3000

#define	E_GrNetNtpPortDflt	123

#define	E_GrNetUpnpDevTypeGateway			1
#define	E_GrNetUpnpDevTypeIpCamera		2

//====================================================================
//type

typedef struct St_GrNetEnv
{
	Cls_GrTaskSvr*	TskSvr;

	Cls_GrTaskCli*	ObjDns;
	Cls_GrTaskCli*	ObjDhcp;
	Cls_GrTaskCli*	ObjNtp;
	Cls_GrTaskCli*	ObjUpnp;

}	*Ptr_GrNetEnv;

//====================================================================
//class


//====================================================================
//function

void	GrNetInit(BOOL8 A_IsDbgMode);
void	GrNetFinish(void);

// dns
void	GrNetDnsSetup(__u32 A_DnsIp);
__s32	GrNetDnsOpen(char* A_StrName);
__u8	GrNetDnsRead(__s32 A_Hndl, void* A_PtrRtIp, __u8 A_MaxCnt, __u8* A_PtrRtCnt);
__u8	GrNetDnsClose(__s32 A_Hndl);
__u32	GrNetDnsIpGet(void);

// dhcp
void	GrNetDhcpSetup(__u8 A_NetIf);
void	GrNetDhcpRun(void);
void	GrNetDhcpStop(void);

// ntp
void	GrNetNtpSetup(char* A_StrSvUrl, __s32 A_GmtSec, __u32 A_UpdtStepSec, __u16 A_Port = E_GrNetNtpPortDflt);
void	GrNetNtpRun(void);
void	GrNetNtpStop(void);
BOOL8	GrNetNtpUpdateNow(BOOL8 A_IsNotify);


// upnp


//====================================================================
//out Enviroment conditional
#endif
