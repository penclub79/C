/*
 GAUSE Network service

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <Net/GrNetSvs.h>

#include <Net/GrNetPlugDns.h>
#include <Net/GrNetPlugSmtp.h>
#include <Net/GrNetPlugNtp.h>
#include <Net/GrNetPlugWebSvr.h>
#include <Net/GrNetPlugDhcp.h>

#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include	<stdlib.h>

//====================================================================
//local const

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrNetSvs::Cls_GrNetSvs(Cls_GrTaskSvr* A_TaskSvr, __u8 A_DevIdx) :
Cls_GrTaskCli( )
{
	// local -------------------
		//__u8	Tv_WkIdx;
	// code --------------------
		// init
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		= GrTaskSvrMsgCmdAlloc();

		m_InitLvl		=	0;

		m_DevIdx		=	A_DevIdx;
		m_Stage			=	E_GrNetSvsStageOff;
		m_IsLink		=	FALSE;

		m_IsIpV4Req	=	FALSE;
		m_IsIpV6Req	=	FALSE;

		GrDumyZeroMem( m_PlugInTbl, sizeof(m_PlugInTbl) );

		//register background 
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrNetSvsTimerPeriod, &m_HndlTimer );
		m_TaskSvr->RegistBkgClient( this );

		m_InitLvl ++;
}
//--------------------------------------------------------------------
Cls_GrNetSvs::~Cls_GrNetSvs()
{
		//unregister background
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );
		m_TaskSvr->UnregistMsgClient( this );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetSvs::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
		//__u32	Tv_PlgIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			switch ( ((Ptr_GrNetSvsMsgBase)A_PtrMsg)->Cmd )
			{
				case E_GrNetSvsMsgPlugRegist:
					LcMprocPlugRegist((Ptr_GrNetSvsMsgPlugRegist)A_PtrMsg);
					break;
				case E_GrNetSvsMsgPlugUnregist:
					LcMprocPlugUnregist((Ptr_GrNetSvsMsgPlugUnregist)A_PtrMsg);
					break;
				case	E_GrNetSvsMsgDnsReq:
					*((Ptr_GrNetSvsMsgDnsReq)A_PtrMsg)->PtrRslt	=	ImmDnsReq( ((Ptr_GrNetSvsMsgDnsReq)A_PtrMsg)->StrName,
						((Ptr_GrNetSvsMsgDnsReq)A_PtrMsg)->SvCnt, ((Ptr_GrNetSvsMsgDnsReq)A_PtrMsg)->IsV6 );
					break;
				case E_GrNetSvsMsgDnsFree:
					ImmDnsFree( ((Ptr_GrNetSvsMsgDnsFree)A_PtrMsg)->Hndl, ((Ptr_GrNetSvsMsgDnsFree)A_PtrMsg)->IsV6 );
					break;
				case E_GrNetSvsMsgSmtpSetup:
					ImmSmtpSetup( ((Ptr_GrNetSvsMsgSmtpSetup)A_PtrMsg)->StrSvName, ((Ptr_GrNetSvsMsgSmtpSetup)A_PtrMsg)->SvPort,
						((Ptr_GrNetSvsMsgSmtpSetup)A_PtrMsg)->StrId, ((Ptr_GrNetSvsMsgSmtpSetup)A_PtrMsg)->StrPass , TRUE);
					break;
				case E_GrNetSvsMsgSmtpReq:
					*((Ptr_GrNetSvsMsgSmtpReq)A_PtrMsg)->PtrRslt	=	ImmSmtpReq( ((Ptr_GrNetSvsMsgSmtpReq)A_PtrMsg)->StrFrom,
						((Ptr_GrNetSvsMsgSmtpReq)A_PtrMsg)->StrTo, ((Ptr_GrNetSvsMsgSmtpReq)A_PtrMsg)->StrSubject, 
						((Ptr_GrNetSvsMsgSmtpReq)A_PtrMsg)->StrCont, ((Ptr_GrNetSvsMsgSmtpReq)A_PtrMsg)->PtrImg, 
						((Ptr_GrNetSvsMsgSmtpReq)A_PtrMsg)->ImgSize );
					break;
				case E_GrNetSvsMsgNtpSetup:
					ImmNtpSetup( ((Ptr_GrNetSvsMsgNtpSetup)A_PtrMsg)->StrSvName, ((Ptr_GrNetSvsMsgNtpSetup)A_PtrMsg)->Gmt );
					break;
				case E_GrNetSvsMsgNtpReq:
					*((Ptr_GrNetSvsMsgNtpReq)A_PtrMsg)->PtrRslt	=	ImmNtpRequest();
					break;
				case E_GrNetSvsMsgNtpGet:
					*((Ptr_GrNetSvsMsgNtpGet)A_PtrMsg)->PtrRslt	=	ImmNtpTimeGet( ((Ptr_GrNetSvsMsgNtpGet)A_PtrMsg)->Hndl,
						((Ptr_GrNetSvsMsgNtpGet)A_PtrMsg)->PtrRtTime );
					break;
				case E_GrNetSvsMsgNtpFree:
					ImmNtpFree( ((Ptr_GrNetSvsMsgNtpFree)A_PtrMsg)->Hndl );
					break;
			}

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetSvs::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_PlgIdx;
	// code --------------------

		// stage process
		Tv_Result	=	TRUE;
		switch ( m_Stage )
		{
			case E_GrNetSvsStageIpPrepare:
				StageIpPrep();
				break;
			case E_GrNetSvsStageDhcpReq:
				StageDhcpReq();
				break;
			case E_GrNetSvsStageDhcpRecv:
				StageDhcpRecv();
				break;
			case E_GrNetSvsStageIpChg:
				StageIpChg();
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		// background process
		for ( Tv_PlgIdx=0;Tv_PlgIdx < E_GrNetSvsMaxPluginCnt;Tv_PlgIdx++ )
		{
			if ( NULL != m_PlugInTbl[Tv_PlgIdx] )
			{
				if ( m_PlugInTbl[Tv_PlgIdx]->RtlBkgProc() )
				{
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
	// code --------------------
		if (E_GrNetSvsStageOff != m_Stage)
		{
			LcLinkUpdt();
		}

}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::LcLinkUpdt( void )
{
	// local -------------------
		BOOL8	Tv_IsAct;
		BOOL8	Tv_IsChg;
		St_GrTaskMsgNetIf	Tv_Msg;
	// code --------------------
		Tv_IsChg	=	FALSE;
		Tv_IsAct	=	GrSockV2IsLink( m_DevIdx );
		if ( Tv_IsAct != m_IsLink )
		{
			Tv_IsChg	=	TRUE;
		}
		m_IsLink	=	Tv_IsAct;

		if ( Tv_IsChg )
		{
			// request DHCP
			if (m_IsDhcpUse && m_IsLink && (E_GrNetSvsStageNone == m_Stage))
			{
				m_Stage	=	E_GrNetSvsStageDhcpReq;
			}

			// notify message interface status
			Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgNetOff;
			if ( Tv_IsAct )
			{
				Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgNetOn;
			}
			Tv_Msg.IfIdx	=	m_DevIdx;
			GrTaskSvrMsgBroadCast( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::StageIpPrep( void )
{
	// local -------------------
		St_GrTaskMsgNetIf	Tv_Msg;
	// code --------------------

		// need update DHCP use, and static IP address
		if ( m_IsDhcpUse )
		{
			m_Stage	=	E_GrNetSvsStageDhcpReq;
		}
		else
		{
			m_IsIpV4Req	=	TRUE;		// static IP update request
			m_Stage			=	E_GrNetSvsStageIpChg;
		}

		// ip release event
		Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgIpFree;
		Tv_Msg.IfIdx			=	m_DevIdx;
		GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::StageDhcpReq( void )
{
	// local -------------------
	// code --------------------

		if (NULL != m_PlugInTbl[E_GrNetSvsPlugDhcp])
		{
			if (((Cls_GrNetPlugDhcp*)m_PlugInTbl[E_GrNetSvsPlugDhcp])->DhcpRequest())
			{
				m_Stage	=	E_GrNetSvsStageDhcpRecv;
				DbgMsgPrint("Cls_GrNetSvs::StageDhcpReq - go DHCP receive stage.\n");
			}
		}
		else
		{
			m_Stage	=	E_GrNetSvsStageNone;
			DbgMsgPrint("Cls_GrNetSvs::StageDhcpReq - DHCP plug-in not found.\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::StageDhcpRecv( void )
{
	// local -------------------
	// code --------------------
		if (NULL != m_PlugInTbl[E_GrNetSvsPlugDhcp])
		{
			if (((Cls_GrNetPlugDhcp*)m_PlugInTbl[E_GrNetSvsPlugDhcp])->DhcpReceive())
			{
				m_Stage	=	E_GrNetSvsStageIpChg;
				DbgMsgPrint("Cls_GrNetSvs::StageDhcpReq - go DHCP receive stage.\n");
			}
		}
		else
		{
			m_Stage	=	E_GrNetSvsStageNone;
			DbgMsgPrint("Cls_GrNetSvs::StageDhcpRecv - DHCP plug-in not found.\n");
		}

}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::StageIpChg( void )
{
	// local -------------------
		St_GrTaskMsgNetIf	Tv_Msg;
	// code --------------------
		// update IPV4
		if ( m_IsIpV4Req )
		{
			GrSockV2SetNetAdr( m_DevIdx, &m_MyV4Ip, &m_MyV4NetMask, &m_MyV4GateWay, FALSE );
			DbgMsgPrint( "Cls_GrNetSvs::StageIpChg - request change ip %d.%d.%d.%d \n", 
				m_MyV4Ip & 0xFF, (m_MyV4Ip >> 8) & 0xFF, (m_MyV4Ip >> 16) & 0xFF, (m_MyV4Ip >> 24) & 0xFF );

			m_IsIpV4Req	=	FALSE;
		}
		
		Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgIpSet;
		Tv_Msg.IfIdx			=	m_DevIdx;
		GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		m_Stage	=	E_GrNetSvsStageNone;
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::RtlV4NetAdrSet( __u32 A_Ip, __u32 A_Mask, __u32 A_Gate )
{
	// local -------------------
	// code --------------------
		m_IsIpV4Req		=	TRUE;
		m_MyV4Ip			=	A_Ip;
		m_MyV4GateWay	=	A_Gate;
		m_MyV4NetMask	=	A_Mask;

}
//--------------------------------------------------------------------
__s32	Cls_GrNetSvs::ImmDnsReq( char* A_StrName, __u8 A_SvCnt, BOOL8 A_IsV6 )
{
	// local -------------------
		__s32		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;

		// check exist
		if (NULL != m_PlugInTbl[E_GrNetSvsPlugDns])
		{
			Tv_Result	=	((Cls_GrNetPlugDns*)m_PlugInTbl[E_GrNetSvsPlugDns])->DnsQueryRequest(A_StrName);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetSvs::ImmDnsResult( __s32 A_Hndl, void* A_PtrRtIp, __u8 A_ReqCnt, __u8* A_PtrRtCnt, BOOL8 A_IsV6 )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result			=	FALSE;
		*A_PtrRtCnt		=	0;

		// check exist
		if (NULL != m_PlugInTbl[E_GrNetSvsPlugDns])
		{
			Tv_Result	=	((Cls_GrNetPlugDns*)m_PlugInTbl[E_GrNetSvsPlugDns])->DnsQueryGetResult(A_Hndl,A_PtrRtIp,A_ReqCnt,A_PtrRtCnt);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::ImmDnsFree( __s32 A_Hndl, BOOL8 A_IsV6 )
{
		if (NULL != m_PlugInTbl[E_GrNetSvsPlugDns])
		{
			((Cls_GrNetPlugDns*)m_PlugInTbl[E_GrNetSvsPlugDns])->DnsFree(A_Hndl);
		}
}
//--------------------------------------------------------------------
__s32		Cls_GrNetSvs::SafeDnsReq( char* A_StrName, __u8 A_SvCnt, BOOL8 A_IsV6 )
{
	// local -------------------
		__s32		Tv_Result;
		St_GrNetSvsMsgDnsReq	Tv_Msg;
	// code --------------------
		Tv_Result	=	-1;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd			=	E_GrNetSvsMsgDnsReq;
		Tv_Msg.StrName			=	A_StrName;
		Tv_Msg.SvCnt				=	A_SvCnt;
		Tv_Msg.IsV6					=	A_IsV6;
		Tv_Msg.PtrRslt			=	&Tv_Result;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetSvs::SafeDnsResult( __s32 A_Hndl, void* A_PtrRtIp, __u8 A_ReqCnt, __u8* A_PtrRtCnt, BOOL8 A_IsV6 )
{
		return	ImmDnsResult( A_Hndl, A_PtrRtIp, A_ReqCnt, A_PtrRtCnt, A_IsV6 );		// not need safe
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::SafeDnsFree( __s32 A_Hndl, BOOL8 A_IsV6 )
{
	// local -------------------
		St_GrNetSvsMsgDnsFree	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd			=	E_GrNetSvsMsgDnsFree;
		Tv_Msg.Hndl					=	A_Hndl;
		Tv_Msg.IsV6					=	A_IsV6;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::ImmSmtpSetup( WCHAR* A_StrSvName, __u16 A_SvPort, WCHAR* A_StrId, WCHAR* A_StrPass, BOOL8 A_IsUseSSL )
{
	// local -------------------
	// code --------------------
		// SMTP setup
		if (NULL != m_PlugInTbl[E_GrNetSvsPlugSmtp])
		{
			((Cls_GrNetPlugSmtp*)m_PlugInTbl[E_GrNetSvsPlugSmtp])->SmtpSetup(A_StrSvName, A_SvPort, A_StrId, A_StrPass, A_IsUseSSL);
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetSvs::ImmSmtpReq( WCHAR* A_StrFrom, WCHAR* A_StrTo, 
	WCHAR* A_StrSubject, WCHAR*	A_StrCont, void* A_PtrImg, __u32 A_ImgSize )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (NULL != m_PlugInTbl[E_GrNetSvsPlugSmtp])
		{
			Tv_Result	=	((Cls_GrNetPlugSmtp*)m_PlugInTbl[E_GrNetSvsPlugSmtp])->SmtpRequest(A_StrFrom, A_StrTo, A_StrSubject, A_StrCont, A_PtrImg,A_ImgSize );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::SafeSmtpSetup( WCHAR* A_StrSvName, __u16 A_SvPort, WCHAR* A_StrId, WCHAR* A_StrPass )
{
	// local -------------------
		St_GrNetSvsMsgSmtpSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd			=	E_GrNetSvsMsgSmtpSetup;
		Tv_Msg.StrSvName		=	A_StrSvName;
		Tv_Msg.SvPort				=	A_SvPort;
		Tv_Msg.StrId				=	A_StrId;
		Tv_Msg.StrPass			=	A_StrPass;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetSvs::SafeSmtpReq( WCHAR* A_StrFrom, WCHAR* A_StrTo, 
	WCHAR* A_StrSubject, WCHAR*	A_StrCont, void* A_PtrImg, __u32 A_ImgSize )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrNetSvsMsgSmtpReq	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd			=	E_GrNetSvsMsgSmtpReq;
		Tv_Msg.StrFrom			=	A_StrFrom;
		Tv_Msg.StrTo				=	A_StrTo;
		Tv_Msg.StrSubject		=	A_StrSubject;
		Tv_Msg.StrCont			=	A_StrCont;
		Tv_Msg.PtrImg				=	A_PtrImg;
		Tv_Msg.ImgSize			=	A_ImgSize;
		Tv_Msg.PtrRslt			=	&Tv_Result;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::ImmNtpSetup(WCHAR* A_StrSvName, __s32 A_Gmt)
{
		if (NULL != m_PlugInTbl[E_GrNetSvsPlugNtp])
		{
			((Cls_GrNetPlugNtp*)m_PlugInTbl[E_GrNetSvsPlugNtp])->NtpSetup(A_StrSvName, A_Gmt);
		}
}
//--------------------------------------------------------------------
__s32		Cls_GrNetSvs::ImmNtpRequest( void )
{
	// local -------------------
		__s32		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;

		if (NULL != m_PlugInTbl[E_GrNetSvsPlugNtp])
		{
			Tv_Result	=	((Cls_GrNetPlugNtp*)m_PlugInTbl[E_GrNetSvsPlugNtp])->NtpRequest();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetSvs::ImmNtpTimeGet( __s32 A_Hndl, Def_GrTime* A_PtrRtTime )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result				=	FALSE;

		if (NULL != m_PlugInTbl[E_GrNetSvsPlugNtp])
		{
			Tv_Result	=	((Cls_GrNetPlugNtp*)m_PlugInTbl[E_GrNetSvsPlugNtp])->NtpTimeGet(A_Hndl,A_PtrRtTime);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::ImmNtpFree( __s32 A_Hndl )
{
		if (NULL != m_PlugInTbl[E_GrNetSvsPlugNtp])
		{
			((Cls_GrNetPlugNtp*)m_PlugInTbl[E_GrNetSvsPlugNtp])->NtpFree(A_Hndl);
		}
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::SafeNtpSetup(WCHAR* A_StrSvName, __s32 A_Gmt)
{
	// local -------------------
		St_GrNetSvsMsgNtpSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd			=	E_GrNetSvsMsgNtpSetup;
		Tv_Msg.StrSvName		=	A_StrSvName;
		Tv_Msg.Gmt					=	A_Gmt;
		
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
__s32		Cls_GrNetSvs::SafeNtpRequest( void )
{
	// local -------------------
		__s32		Tv_Result;
		St_GrNetSvsMsgNtpReq	Tv_Msg;
	// code --------------------
		Tv_Result	=	-1;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd			=	E_GrNetSvsMsgNtpReq;
		Tv_Msg.PtrRslt			=	&Tv_Result;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetSvs::SafeNtpTimeGet( __s32 A_Hndl, Def_GrTime* A_PtrRtTime )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrNetSvsMsgNtpGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		*A_PtrRtTime	=	0;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd			=	E_GrNetSvsMsgNtpGet;
		Tv_Msg.Hndl					=	A_Hndl;
		Tv_Msg.PtrRtTime		=	A_PtrRtTime;
		Tv_Msg.PtrRslt			=	&Tv_Result;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::SafeNtpFree( __s32 A_Hndl )
{
	// local -------------------
		St_GrNetSvsMsgNtpFree	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd			=	E_GrNetSvsMsgNtpGet;
		Tv_Msg.Hndl					=	A_Hndl;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetSvs::RtlPluginRegist(Cls_GrNetPlugBase* A_ObjPlugin, __u8 A_PlugIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrNetSvsMsgPlugRegist	Tv_Msg;
	// code --------------------
		Tv_Result						=	FALSE;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd			=	E_GrNetSvsMsgPlugRegist;
		Tv_Msg.ObjPlugin		=	A_ObjPlugin;
		Tv_Msg.PlugIdx			=	A_PlugIdx;
		Tv_Msg.PtrRslt			=	&Tv_Result;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::LcMprocPlugRegist(Ptr_GrNetSvsMsgPlugRegist A_PtrMsg)
{
	// local -------------------
	// code --------------------
		// check plugin range
		if (E_GrNetSvsMaxPluginCnt > A_PtrMsg->PlugIdx)
		{
			// check already exist
			if (NULL == m_PlugInTbl[A_PtrMsg->PlugIdx])
			{
				// regist
				m_PlugInTbl[A_PtrMsg->PlugIdx]		=	A_PtrMsg->ObjPlugin;
				*(A_PtrMsg->PtrRslt)		=	TRUE;
			}
			else
			{
				DbgMsgPrint("Cls_GrNetSvs::LcMprocPlugRegist - Plug-in slot (%d) is not empty\n", A_PtrMsg->PlugIdx);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrNetSvs::LcMprocPlugRegist - Plugin index (%d) is out of range\n", A_PtrMsg->PlugIdx);
		}
}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::RtlPluginUnregist(__u8 A_PlugIdx)
{
	// local -------------------
		St_GrNetSvsMsgPlugUnregist	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd			=	E_GrNetSvsMsgPlugUnregist;
		Tv_Msg.PlugIdx			=	A_PlugIdx;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrNetSvs::LcMprocPlugUnregist(Ptr_GrNetSvsMsgPlugUnregist A_PtrMsg)
{
	// local -------------------
	// code --------------------
		// check plugin range
		if (E_GrNetSvsMaxPluginCnt > A_PtrMsg->PlugIdx)
		{
			// check already exist
			if (NULL != m_PlugInTbl[A_PtrMsg->PlugIdx])
			{
				// regist
				m_PlugInTbl[A_PtrMsg->PlugIdx]		=	NULL;
			}
			else
			{
				DbgMsgPrint("Cls_GrNetSvs::LcMprocPlugUnregist - Plug-in slot (%d) is empty\n", A_PtrMsg->PlugIdx);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrNetSvs::LcMprocPlugUnregist - Plugin index (%d) is out of range\n", A_PtrMsg->PlugIdx);
		}

}
//--------------------------------------------------------------------
__u8	Cls_GrNetSvs::NetIfIdxGet(void)
{
		return	m_DevIdx;
}
//--------------------------------------------------------------------

