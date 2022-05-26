/*
	free dns client


*/

#include	<GrFdnsCli.h>
#include	<GrSocket.h>
#include	<GrStrTool.h>
#include	<GrProcess.h>
#include	<GrDebug.h>
#include	<GrDumyTool.h>

//--------------------------------------------------------------------
//local const

#define E_GrFdnsCliTimerPriod			1000

#define E_GrFdnsCliTimeOut				30
#define E_GrFdnsCliRecvTimeOut		4
#define E_GrFdnsCliTryCnt					3

//dyndns
#define E_GrFdnsCliDyndnsIpSvName			"checkip.dyndns.org"
#define E_GrFdnsCliDyndnsRegSvName		"members.dyndns.org"
#define E_GrFdnsCliDyndnsDnsSvUrl			"/nic/update?"

//no-ip
#define E_GrFdnsCliNoipIpSvName				"ip1.dynupdate.no-ip.com"
#define E_GrFdnsCliNoipRegSvName			"dynupdate.no-ip.com"
#define E_GrFdnsCliNoipDnsSvUrl				"/nic/update?hostname="

//message
#define E_GrFdnsCliMsgReqUpdt					0	/* request update : St_GrFdnsCliMsgReqUpdt */

//--------------------------------------------------------------------
//local type

typedef	struct St_GrFdnsCliMsgHd 
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrFdnsCliMsgHd;

typedef struct St_GrFdnsCliMsgReqUpdt
{
	St_GrFdnsCliMsgHd	Hd;
	__u8	DevIdx;
	__u8	HostType;
	char* StrId;
	char* StrPass;
	char* StrDomain;
	BOOL8*	PtrRtRslt;
} *Ptr_GrFdnsCliMsgReqUpdt;

//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var


//====================================================================
//--------------------------------------------------------------------
Cls_GrFdnsCli::Cls_GrFdnsCli( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		//init
		m_Stage			=	E_GrFdnsCliStageNone;
		m_TimeOut		=	0;
		m_TryCnt		=	0;

		m_HndlSock	=	E_GrSockBadHandle;

		//regist
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrFdnsCliTimerPriod, &m_HndlTime );
		m_TaskSvr->RegistBkgClient( this );
}
//--------------------------------------------------------------------
Cls_GrFdnsCli::~Cls_GrFdnsCli()
{
	// local -------------------
	// code --------------------
		//release 
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTime );
		m_TaskSvr->UnregistMsgClient( this );

		LcFinish();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcReqUpdt(__u8 A_DevIdx, __u8	A_HostType, char* A_StrId, char* A_StrPass, char* A_StrDomain)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_MyIp;
	// code --------------------
		Tv_Result	=	FALSE;

		//check now working
		if ( E_GrFdnsCliStageNone == m_Stage )
		{
			//check parameter
			if ( ( E_GrFdnsCliHostNoIp >= A_HostType ) && 
						GrStrIsExist(A_StrId) && GrStrIsExist(A_StrPass) && GrStrIsExist(A_StrDomain) )
			{
				//init
				m_TimeOut		=	0;
				m_IpSvr			=	0;
				m_DevIdx		=	A_DevIdx;
				m_HndlDns		=	-1;
				//copy parameter
				m_HostType	=	A_HostType;
				GrStrCopy( m_StrId, A_StrId );
				GrStrCopy( m_StrPass, A_StrPass );
				GrStrCopy( m_StrDomain, A_StrDomain );
				//get my ip
				Tv_MyIp		=	GrSockGetMyIp( m_DevIdx );
				//check local ip
				if ( 192 == (0xFF & Tv_MyIp) )
				{
					m_Stage		=	E_GrFdnsCliStageRtipDns;		//request router ip
					GrStrClear( m_StrMyIp );
				}
				else
				{
					m_Stage		=	E_GrFdnsCliStageStipDns;
					GrStrIpToStr( m_StrMyIp, Tv_MyIp );
					DbgMsgPrint( "Cls_GrFdnsCli::LcReqUpdt - ip select : %s\n", m_StrMyIp );
				}
				//success
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFdnsCli::LcFinish( void )
{
		//release dns
		if ( 0 <= m_HndlDns )
		{
			GrSockDnsFree( m_HndlDns );
			m_HndlDns	=	-1;
		}
		//release handle
		if ( E_GrSockBadHandle != m_HndlSock )
		{
			GrSockClose( m_HndlSock );
			m_HndlSock	=	E_GrSockBadHandle;
		}
		// init
		m_Stage		=	E_GrFdnsCliStageNone;
		m_TimeOut	=	0;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcStageRtipDns( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		char*	Tv_StrName;
		__u8	Tv_IpCnt;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 > m_HndlDns )
		{
			//dns connect
			switch( m_HostType )
			{
				case	E_GrFdnsCliHostDyndDns:
					Tv_StrName	=	E_GrFdnsCliDyndnsIpSvName;
					break;
				case	E_GrFdnsCliHostNoIp:
					Tv_StrName	=	E_GrFdnsCliNoipIpSvName;
					break;
				default:
					Tv_StrName	=	E_GrFdnsCliDyndnsIpSvName;
					break;
			}
			m_HndlDns	=	GrSockDnsReq( Tv_StrName );
			Tv_Result	=	TRUE;
		}
		else
		{
			//wait result
			if ( GrSockDnsGetResult( m_HndlDns, &m_IpSvr, 1, &Tv_IpCnt ) )
			{
				//release dns
				GrSockDnsFree( m_HndlDns );
				m_HndlDns	=	-1;
				//check success
				if ( 0 != m_IpSvr )
				{
					//success, go next
					m_Stage ++;
				}
				else
				{
					//error , stop
					DbgMsgPrint( "Cls_GrFdnsCli::LcStageRtipDns - dns fail\n" );
					LcFinish();
				}
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcStageRtipTryConn( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;
		//try connect
		if ( E_GrSockBadHandle != m_HndlSock )
		{
			//try connect
			Tv_Rval	=	GrSockConnect( m_HndlSock, m_IpSvr, 80 );
			if ( E_GrSockConnOk == Tv_Rval )
			{
				m_Stage				=	E_GrFdnsCliStageRtipContents;
			}
			else if ( E_GrSockConnWaiting != Tv_Rval )
			{
				//connect error
				DbgMsgPrint( "Cls_GrFdnsCli::LcStageRtipTryConn - connect error.\n" );
				//go finish
				LcFinish();
			}
			else
			{
				// connect wait
				m_Stage				=	E_GrFdnsCliStageRtipWaitConn;
			}
			Tv_Result	=	TRUE;
		}
		else
		{
			//create socket
			m_HndlSock	=	GrSockOpen( FALSE, TRUE );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcStageRtipWaitConn( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Rval	=	GrSockWaitConnect( m_HndlSock, 0 );
		if ( E_GrSockConnOk == Tv_Rval )
		{
			// go next
			m_Stage				=	E_GrFdnsCliStageRtipContents;
			Tv_Result			=	TRUE;
		}
		else if ( E_GrSockConnWaiting != Tv_Rval )
		{
			//error
			DbgMsgPrint( "Cls_GrFdnsCli::LcStageRtipWaitConn - connect error.\n" );
			//go finish
			LcFinish();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcStageRtipContets( void )
{
	// local -------------------
		char*	Tv_StrWk;
	// code --------------------
		//build ip request pacekt
		Tv_StrWk	=	GrStrCopy( m_SendBuf, "GET http://" );
		switch( m_HostType )
		{
		case	E_GrFdnsCliHostDyndDns:
			Tv_StrWk	=	GrStrCopy( Tv_StrWk, E_GrFdnsCliDyndnsIpSvName );
			break;
		case	E_GrFdnsCliHostNoIp:
			Tv_StrWk	=	GrStrCopy( Tv_StrWk, E_GrFdnsCliNoipIpSvName );
			break;
		}
		Tv_StrWk		=	GrStrCopy( Tv_StrWk, ":0/ HTTP/1.0\r\n\r\n" );
		m_SendSize	=	GrStrLen( m_SendBuf ) + 1;
		m_Sended		=	0;
		m_RecvSize	=	E_GrFdnsCliRecvBufSize - 1;		//reset recive size
		m_RecvBuf[E_GrFdnsCliRecvBufSize - 1]	=	0;
		m_Recved		=	0;
		m_RcvTimeOut	=	m_TimeOut;
		//go next stage
		m_Stage ++;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcStageSend( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Sended;
		__s32		Tv_SendSize;
		void*	Tv_PtrSend;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrSend	=	(void*)( (__u32)m_SendBuf + m_Sended );
		Tv_SendSize	=	(__s32)( m_SendSize - m_Sended );

		Tv_Sended		=	GrSockSend( m_HndlSock, Tv_PtrSend, Tv_SendSize );
		if ( 0 < Tv_Sended )
		{
			//check send all
			m_Sended	=	m_Sended + (__u32)Tv_Sended;
			if ( m_Sended >= m_SendSize )
			{
				//send completed
				m_Stage ++;
				//proceed
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcStageRecv( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_Readed;
		__s32		Tv_ReadSize;
		void*	Tv_PtrRead;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrRead	=	(void*)( (__u32)m_RecvBuf + m_Recved );
		Tv_ReadSize	=	(__s32)( m_RecvSize - m_Recved );

		Tv_Readed		=	GrSockRecv( m_HndlSock, Tv_PtrRead, Tv_ReadSize );
		if ( 0 < Tv_Readed )
		{
			m_Recved	=	m_Recved + (__u32)Tv_Readed;
			//check finished
			if ( (m_Recved >= m_RecvSize ) || (0 == m_RecvBuf[m_Recved - 1]) || (0x0A == m_RecvBuf[m_Recved - 1]) )
			{
				//mark zero
				m_RecvBuf[m_Recved]	=	0;
				//recived ok
				m_Stage ++;
				//proceed
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			if ( ( (0 != m_Recved) && (E_GrFdnsCliRecvTimeOut < (m_TimeOut - m_RcvTimeOut)) ) )
			{
				//mark zero
				m_RecvBuf[m_Recved]	=	0;
				//recived ok
				m_Stage ++;
				//proceed
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcStageRtipEnd( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u8	Tv_DotCnt;
		char*	Tv_StrIp;
	// code --------------------
		Tv_Result	=	FALSE;

		//close socket
		GrSockClose( m_HndlSock );
		m_HndlSock	=	E_GrSockBadHandle;

		//find ip string
		Tv_StrIp	=	GrStrFindStr( m_RecvBuf, "IP Address: " );
		if ( NULL != Tv_StrIp )
		{
			//copy ip
			Tv_StrIp	=	Tv_StrIp + 12;		//len of "IP Address: "
			Tv_DotCnt	=	0;
			Tv_WkIdx	=	0;
			while ( Tv_WkIdx < m_Recved )
			{
				//check dot
				if ( '.' == Tv_StrIp[Tv_WkIdx] )
				{
					Tv_DotCnt ++;
				}
				//check zero finish
				if ( (0 == Tv_StrIp[Tv_WkIdx]) || ('<' == Tv_StrIp[Tv_WkIdx]) )
				{
					break;
				}
				//copy
				m_StrMyIp[Tv_WkIdx]	=	Tv_StrIp[Tv_WkIdx];
				//next
				Tv_WkIdx ++;
			}
			//mark finish
			m_StrMyIp[Tv_WkIdx]	=	0;

			if ( 3 == Tv_DotCnt )
			{
				DbgMsgPrint( "Cls_GrFdnsCli::LcStageRtipEnd - router ip ok = %s \n", m_StrMyIp );
				m_Stage ++;
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint( "Cls_GrFdnsCli::LcStageRtipEnd - bad router ip = %s \n", m_StrMyIp );
				LcFinish();
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrFdnsCli::LcStageRtipEnd - cannot find router ip\n" );
			LcFinish();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcStageStipDns( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		char*	Tv_StrName;
		__u8	Tv_IpCnt;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 > m_HndlDns )
		{
			//dns connect
			switch( m_HostType )
			{
				case	E_GrFdnsCliHostDyndDns:
					Tv_StrName	=	E_GrFdnsCliDyndnsRegSvName;
					break;
				case	E_GrFdnsCliHostNoIp:
					Tv_StrName	=	E_GrFdnsCliNoipRegSvName;
					break;
				default:
					Tv_StrName	=	E_GrFdnsCliDyndnsRegSvName;
					break;
			}
			m_HndlDns	=	GrSockDnsReq( Tv_StrName );
			Tv_Result	=	TRUE;
		}
		else
		{
			//wait result
			if ( GrSockDnsGetResult( m_HndlDns, &m_IpSvr,1, &Tv_IpCnt ) )
			{
				//release dns
				GrSockDnsFree( m_HndlDns );
				m_HndlDns	=	-1;
				//check success
				if ( 0 != m_IpSvr )
				{
					//success, go next
					m_TryCnt	=	0;
					m_Stage ++;
				}
				else
				{
					//error , stop
					DbgMsgPrint( "Cls_GrFdnsCli::LcStageStipDns - dns fail\n" );
					LcFinish();
				}
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcStageStipTryConn( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;
		//try connect
		if ( E_GrSockBadHandle != m_HndlSock )
		{
			//try connect
			Tv_Rval	=	GrSockConnect( m_HndlSock, m_IpSvr, 80 );
			if ( E_GrSockConnOk == Tv_Rval )
			{
				m_Stage				=	E_GrFdnsCliStageStipContents;
			}
			else if ( E_GrSockConnWaiting != Tv_Rval )
			{
				//connect error
				DbgMsgPrint( "Cls_GrFdnsCli::LcStageStipTryConn - connect error.\n" );
				//go finish
				LcFinish();
			}
			else
			{
				// connect wait
				m_Stage				=	E_GrFdnsCliStageStipWaitConn;
			}
			Tv_Result	=	TRUE;
		}
		else
		{
			//create socket
			m_HndlSock	=	GrSockOpen( FALSE, TRUE );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcStageStipWaitConn( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Rval	=	GrSockWaitConnect( m_HndlSock, 0 );
		if ( E_GrSockConnOk == Tv_Rval )
		{
			// go next
			m_Stage				=	E_GrFdnsCliStageStipContents;
			Tv_Result			=	TRUE;
		}
		else if ( E_GrSockConnWaiting != Tv_Rval )
		{
			//error
			DbgMsgPrint( "Cls_GrFdnsCli::LcStageStipWaitConn - connect error.\n" );
			//go finish
			LcFinish();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcStageStipContents( void )
{
	// local -------------------
		char*	Tv_StrWk;
		Def_StrTag	Tv_StrAcount;
		Def_StrShort	Tv_StrEncAcount;
	// code --------------------
		//make id and password base64 string
		Tv_StrWk	=	GrStrCopy( Tv_StrAcount, m_StrId );
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, ":" );
		Tv_StrWk	=	GrStrCopy( Tv_StrWk, m_StrPass );
		GrStrBase64Enc( Tv_StrAcount, Tv_StrEncAcount );

		//build ip request pacekt
		Tv_StrWk	=	GrStrCopy( m_SendBuf, "GET http://" );
		switch( m_HostType )
		{
			case	E_GrFdnsCliHostDyndDns:
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, E_GrFdnsCliDyndnsRegSvName );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, ":0" );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, E_GrFdnsCliDyndnsDnsSvUrl );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, "system=dyndns&hostname=" );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, m_StrDomain );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, "&myip=" );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, m_StrMyIp );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, "&wildcard=ON&mx=" );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, m_StrDomain );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, "&backmx=NO&offline=NO HTTP/1.0\r\nHost: " );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, E_GrFdnsCliDyndnsRegSvName );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, "\r\nAuthorization: Basic " );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, Tv_StrEncAcount );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, "\r\nUser-Agent: inadyn/1.96.2 inarcis2002@hotpop.com\r\n\r\n" );
				break;
			case	E_GrFdnsCliHostNoIp:
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, E_GrFdnsCliNoipRegSvName );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, ":0" );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, E_GrFdnsCliNoipDnsSvUrl );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, m_StrDomain );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, "&myip=" );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, m_StrMyIp );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, " HTTP/1.0\r\nAuthorization: Basic " );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, Tv_StrEncAcount );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, " \r\nHost: " );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, E_GrFdnsCliNoipRegSvName );
				Tv_StrWk	=	GrStrCopy( Tv_StrWk, " \r\nUser-Agent: inadyn/1.96.2 inarcis2002@hotpop.com\r\n\r\n" );
				break;
		}
		m_SendSize	=	GrStrLen( m_SendBuf );
		m_Sended		=	0;
		m_RecvSize	=	E_GrFdnsCliRecvBufSize - 1;		//reset recive size
		m_RecvBuf[E_GrFdnsCliRecvBufSize - 1]	=	0;	//set end point
		m_Recved		=	0;
		m_RcvTimeOut	=	m_TimeOut;

		//go next stage
		m_Stage ++;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::LcStageStipEnd( void )
{
	// local -------------------
		BOOL8	Tv_IsOk;
	// code --------------------

		//close socket
		GrSockClose( m_HndlSock );
		m_HndlSock	=	E_GrSockBadHandle;

		//check result
		Tv_IsOk		=	FALSE;

		if ( (NULL != GrStrFindStr(m_RecvBuf, "good")) || (NULL != GrStrFindStr(m_RecvBuf, "nochg")) )
		{
			DbgMsgPrint("Cls_GrFdnsCli::LcStageStipEnd - ip update success\n");
			Tv_IsOk	=	TRUE;
		}
		else
		{
			DbgMsgPrint("Cls_GrFdnsCli::LcStageStipEnd - ip update failure\n");
		}
		//finish
		if ( Tv_IsOk || (E_GrFdnsCliTryCnt <= m_TryCnt) )
		{
			LcFinish();
		}
		else
		{
			//retry
			m_TryCnt ++;
			m_Stage	=	E_GrFdnsCliStageStipTryConn;
			DbgMsgPrint("Cls_GrFdnsCli::LcStageStipEnd - retry.\n");
		}

		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			switch( ((Ptr_GrFdnsCliMsgHd)A_PtrMsg)->Cmd )
			{
				case	E_GrFdnsCliMsgReqUpdt:
					*(((Ptr_GrFdnsCliMsgReqUpdt)A_PtrMsg)->PtrRtRslt)	=	LcReqUpdt( 
						((Ptr_GrFdnsCliMsgReqUpdt)A_PtrMsg)->DevIdx, ((Ptr_GrFdnsCliMsgReqUpdt)A_PtrMsg)->HostType,
						((Ptr_GrFdnsCliMsgReqUpdt)A_PtrMsg)->StrId, ((Ptr_GrFdnsCliMsgReqUpdt)A_PtrMsg)->StrPass,
						((Ptr_GrFdnsCliMsgReqUpdt)A_PtrMsg)->StrDomain );
					break;
			}
			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::ReqUpdate(__u8 A_DevIdx, __u8	A_HostType, char* A_StrId, char* A_StrPass, char* A_StrDomain)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrFdnsCliMsgReqUpdt	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrFdnsCliMsgReqUpdt;
		Tv_Msg.DevIdx				=	A_DevIdx;
		Tv_Msg.HostType			=	A_HostType;
		Tv_Msg.StrId				=	A_StrId;
		Tv_Msg.StrPass			=	A_StrPass;
		Tv_Msg.StrDomain		=	A_StrDomain;
		Tv_Msg.PtrRtRslt		=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFdnsCli::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		switch( m_Stage )
		{
			case	E_GrFdnsCliStageRtipDns:
				Tv_Result	=	LcStageRtipDns();
				break;
			case	E_GrFdnsCliStageRtipTryConn:
				Tv_Result	=	LcStageRtipTryConn();
				break;
			case	E_GrFdnsCliStageRtipWaitConn:
				Tv_Result	=	LcStageRtipWaitConn();
				break;
			case E_GrFdnsCliStageRtipContents:
				Tv_Result	=	LcStageRtipContets();
				break;
			case	E_GrFdnsCliStageRtipEnd:
				Tv_Result	=	LcStageRtipEnd();
				break;
			case	E_GrFdnsCliStageStipDns:
				Tv_Result	=	LcStageStipDns();
				break;
			case	E_GrFdnsCliStageStipTryConn:
				Tv_Result	=	LcStageStipTryConn();
				break;
			case E_GrFdnsCliStageStipWaitConn:
				Tv_Result	=	LcStageStipWaitConn();
				break;
			case E_GrFdnsCliStageStipContents:
				Tv_Result	=	LcStageStipContents();
				break;
			case	E_GrFdnsCliStageStipEnd:
				Tv_Result	=	LcStageStipEnd();
				break;
			case	E_GrFdnsCliStageRtipSend:
			case	E_GrFdnsCliStageStipSend:
				Tv_Result	=	LcStageSend();
				break;
			case	E_GrFdnsCliStageRtipRecv:
			case	E_GrFdnsCliStageStipRecv:
				Tv_Result	=	LcStageRecv();
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFdnsCli::RtlTimer( __u32 A_Hndl )
{
		if ( E_GrFdnsCliStageNone != m_Stage )
		{
			m_TimeOut ++;
			if ( E_GrFdnsCliTimeOut < m_TimeOut )
			{
				DbgMsgPrint("Cls_GrFdnsCli::RtlTimer - timeout\n");
				LcFinish();
			}
		}
}
//--------------------------------------------------------------------
