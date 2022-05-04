/*
	NET base


*/

#include <GrNetBase.h>
#include <GrStrTool.h>
#include <GrProcess.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrDebug.h>

#include <GrDnsCli.h>
#include <GrDhcpCli.h>
#include <GrNtpCli.h>
#include <GrUpnpCli.h>

#ifdef LINUX_APP

#include	<arpa/inet.h>

#endif

//--------------------------------------------------------------------
//local const

//--------------------------------------------------------------------
//local type


//--------------------------------------------------------------------
//local macro

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

St_GrNetEnv	V_GrNetEnv	=	
{	NULL };

//====================================================================
//--------------------------------------------------------------------
void	GrNetInit(BOOL8 A_IsDbgMode)
{
	// local -------------------
	// code --------------------
		// create thread
		V_GrNetEnv.TskSvr	=	(Cls_GrTaskSvr*)new Cls_GrTaskSvr(E_GrNetThrdFcc);

		V_GrNetEnv.ObjDns	=	(Cls_GrTaskCli*) new Cls_GrDnsCli(V_GrNetEnv.TskSvr);
		((Cls_GrDnsCli*)V_GrNetEnv.ObjDns)->ReqInit();

		V_GrNetEnv.ObjDhcp	=	(Cls_GrTaskCli*) new Cls_GrDhcpCli(V_GrNetEnv.TskSvr,A_IsDbgMode);
		((Cls_GrDhcpCli*)V_GrNetEnv.ObjDhcp)->ReqInit();

		V_GrNetEnv.ObjNtp	=	(Cls_GrTaskCli*) new Cls_GrNtpCli(V_GrNetEnv.TskSvr);
		((Cls_GrNtpCli*)V_GrNetEnv.ObjNtp)->ReqInit();

		//V_GrNetEnv.ObjUpnp	=	(Cls_GrTaskCli*) new Cls_GrUpnpCli(V_GrNetEnv.TskSvr);
		//((Cls_GrUpnpCli*)V_GrNetEnv.ObjUpnp)->ReqInit();

}
//--------------------------------------------------------------------
void	GrNetFinish(void)
{
#if 0
		if(NULL != V_GrNetEnv.ObjUpnp)
		{
			delete	V_GrNetEnv.ObjUpnp;
			V_GrNetEnv.ObjUpnp	=	NULL;
		}
#endif

		if(NULL != V_GrNetEnv.ObjDhcp)
		{
			delete	V_GrNetEnv.ObjDhcp;
			V_GrNetEnv.ObjDhcp	=	NULL;
		}

		if(NULL != V_GrNetEnv.ObjDns)
		{
			delete	V_GrNetEnv.ObjDns;
			V_GrNetEnv.ObjDns	=	NULL;
		}

		if(NULL != V_GrNetEnv.TskSvr)
		{
			delete	V_GrNetEnv.TskSvr;
			V_GrNetEnv.TskSvr	=	NULL;
		}
}
//--------------------------------------------------------------------
__s32	GrNetDnsOpen(char* A_StrName)
{
		return	((Cls_GrDnsCli*)V_GrNetEnv.ObjDns)->ReqOpen(A_StrName);
}
//--------------------------------------------------------------------
__u8	GrNetDnsRead(__s32 A_Hndl, void* A_PtrRtIp, __u8 A_MaxCnt, __u8* A_PtrRtCnt)
{
		return	((Cls_GrDnsCli*)V_GrNetEnv.ObjDns)->ReqRead(A_Hndl, A_PtrRtIp, A_MaxCnt, A_PtrRtCnt);
}
//--------------------------------------------------------------------
__u8	GrNetDnsClose(__s32 A_Hndl)
{
		return	((Cls_GrDnsCli*)V_GrNetEnv.ObjDns)->ReqClose(A_Hndl);
}
//--------------------------------------------------------------------
__u32	GrNetDnsIpGet(void)
{
	return	((Cls_GrDnsCli*)V_GrNetEnv.ObjDns)->ReqDnsIpGet();
}
//--------------------------------------------------------------------
void	GrNetDnsSetup(__u32 A_DnsIp)
{
		((Cls_GrDnsCli*)V_GrNetEnv.ObjDns)->ReqSetup(A_DnsIp);
}
//--------------------------------------------------------------------
void	GrNetDhcpSetup(__u8 A_NetIf)
{
		((Cls_GrDhcpCli*)V_GrNetEnv.ObjDhcp)->ReqSetup(A_NetIf);
}
//--------------------------------------------------------------------
void	GrNetDhcpRun(void)
{
		((Cls_GrDhcpCli*)V_GrNetEnv.ObjDhcp)->ReqRun();
}
//--------------------------------------------------------------------
void	GrNetDhcpStop(void)
{
		((Cls_GrDhcpCli*)V_GrNetEnv.ObjDhcp)->ReqStop();
}
//--------------------------------------------------------------------
void	GrNetNtpSetup(char* A_StrSvUrl, __s32 A_GmtSec, __u32 A_UpdtStepSec, __u16 A_Port)
{
	((Cls_GrNtpCli*)V_GrNetEnv.ObjNtp)->ReqSetup(A_StrSvUrl, A_GmtSec, A_UpdtStepSec, A_Port);
}
//--------------------------------------------------------------------
void	GrNetNtpRun(void)
{
		((Cls_GrNtpCli*)V_GrNetEnv.ObjNtp)->ReqRun();
}
//--------------------------------------------------------------------
void	GrNetNtpStop(void)
{
		((Cls_GrNtpCli*)V_GrNetEnv.ObjNtp)->ReqStop();
}
//--------------------------------------------------------------------
BOOL8	GrNetNtpUpdateNow(BOOL8 A_IsNotify)
{
		return	((Cls_GrNtpCli*)V_GrNetEnv.ObjNtp)->ReqUpdateNow(A_IsNotify);
}
//--------------------------------------------------------------------

