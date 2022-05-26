/*
 GAUSE Network NTP plug-in

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <Net/GrNetPlugNtp.h>
#include <Net/GrNetSvs.h>

#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include	<stdlib.h>

#include	<GrError.h>

#ifdef LINUX_APP

#include	<arpa/inet.h>

#endif

//====================================================================
//local const

#define E_GrNetPlugNtpHostAddrField		1					/* host address field */
#define E_GrNetPlugNtpArpaIntnField		1					/* ARPA internet field */

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrNetPlugNtp::Cls_GrNetPlugNtp(Cls_GrTaskSvr* A_TaskSvr, Cls_GrTaskCli* A_NetSvs, __u8 A_DevIdx, __u8 A_PlugIdx) :
Cls_GrNetPlugBase(A_TaskSvr, A_NetSvs, A_DevIdx, A_PlugIdx)
{
	// local -------------------
	// code --------------------
		// init
		m_Hdns		=	-1;
		m_Ip			=	0;
		GrStrClear( m_StrSvName );
		m_DnsUpdtPeriod	=	0;
		m_Gmt			=	0;

		m_Hsock		=	E_GrSockBadHandle;
		m_Stage		=	E_GrNetPlugNtpStageNone;

		GrDumyZeroMem( m_ReqTbl, sizeof(m_ReqTbl) );

		// init send packet
		GrDumyZeroMem( &m_PktSend, sizeof(m_PktSend) );
		m_PktSend.li_vn_mode	=	(E_GrNetPlugNtpPrtcLiValNwr << E_GrNetPlugNtpPrtcLiBit) |	
			(E_GrNetPlugNtpPrtcVnVal << E_GrNetPlugNtpPrtcVnBit) |
			(E_GrNetPlugNtpPrtcMdValCli << E_GrNetPlugNtpPrtcMdBit);
		m_PktSend.refid				=	E_GrNetPlugNtpPrtcRefIdDflt;
		m_PktSend.ppoll				=	E_GrNetPlugNtpPrtcPollDflt;
		m_PktSend.precision		=	E_GrNetPlugNtpPrtcPrecDflt;

		m_TaskSvr->RegistTimerClient(this, E_GrNetPlugNtpTimerPeriod, &m_HndlTimer);
		m_TaskSvr->RegistBkgClient(this);
}
//--------------------------------------------------------------------
Cls_GrNetPlugNtp::~Cls_GrNetPlugNtp()
{
	// local -------------------
	// code --------------------

		m_TaskSvr->UnregistBkgClient(this);
		m_TaskSvr->UnregistTimerClient(m_HndlTimer);

		//release socket
		if ( E_GrSockBadHandle != m_Hsock )
		{
			GrSockV2Close( m_Hsock );
		}
		// release DNS
		if ( 0 <= m_Hdns )
		{
			((Cls_GrNetSvs*)m_NetSvs)->ImmDnsFree( m_Hdns, FALSE );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugNtp::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		switch ( m_Stage )
		{
			case E_GrNetPlugNtpStageDns:
				if ( LcStageDns() )
				{
					Tv_Result	=	TRUE;
				}
				break;
			case E_GrNetPlugNtpStageSend:
				if ( LcStageSend() )
				{
					Tv_Result	=	TRUE;
				}
				break;
			case E_GrNetPlugNtpStageRecv:
				if ( LcStageRecv() )
				{
					Tv_Result	=	TRUE;
				}
				break;
			case E_GrNetPlugNtpStageFinish:
				if ( LcStageFinish() )
				{
					Tv_Result	=	TRUE;
				}
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugNtp::RtlTimer(__u32 A_Hndl)
{
	// local -------------------
	// code --------------------
		// check time out
		if ( (E_GrNetPlugNtpStageNone != m_Stage) && (E_GrNetPlugNtpStageFinish != m_Stage) && (0 != m_TimeOut) )
		{
			m_TimeOut --;
			if ( 0 == m_TimeOut )
			{
				// time out
				LcRsltFail();
				m_Stage	=	E_GrNetPlugNtpStageFinish;
				DbgMsgPrint( "Cls_GrNetPlugNtp::RtlTimer - time out!\n" );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugNtp::NtpSetup(WCHAR* A_StrSvName, __s32 A_Gmt)
{
	// local -------------------
	// code --------------------
		m_Gmt		=	A_Gmt;
		GrStrWstrToStr( m_StrSvName, A_StrSvName );
		m_Ip	=	0;
		m_DnsUpdtPeriod	=	0;
}
//--------------------------------------------------------------------
__s32	Cls_GrNetPlugNtp::NtpRequest(void)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
	// code --------------------
		// init
		Tv_Result	=	-1;
		// find empty request
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrNetPlugNtpReqMax;Tv_WkIdx++ )
		{
			if ( E_GrNetPlugNtpReqStatNone == m_ReqTbl[Tv_WkIdx].Stat )
			{
				// found
				Tv_Result	=	Tv_WkIdx;
				m_ReqTbl[Tv_WkIdx].Stat	=	E_GrNetPlugNtpReqStatWait;
				m_ReqTbl[Tv_WkIdx].RecvTime	=	0;
				break;
			}
		}
		// start
		LcStart();

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugNtp::LcStart( void )
{
	// local -------------------
	// code --------------------
		// check stage
		if ( E_GrNetPlugNtpReqStatNone == m_Stage) 
		{
			if ((0 == m_Ip) || (E_GrNetPlugNtpDnsUpdtPeriod < m_DnsUpdtPeriod))
			{
				// DNS request
				m_DnsUpdtPeriod	=	0;
				m_Ip						=	0;
				// check exist
				if ( GrStrIsExist(m_StrSvName) )
				{
					// DNS request
					m_Hdns	=	((Cls_GrNetSvs*)m_NetSvs)->ImmDnsReq( m_StrSvName, 1, FALSE );
					if ( 0 <= m_Hdns )
					{
						m_Stage		=	E_GrNetPlugNtpStageDns;
						m_TimeOut	=	10;
					}
					else
					{
						DbgMsgPrint( "Cls_GrNetPlugNtp::LcRequest - DNS request fail.\n" );
					}
				}
				// check success
				if ( E_GrNetPlugNtpStageDns != m_Stage )
				{
					// request all error
					LcRsltFail();
				}
			}
			else
			{
				// direct request
				m_Stage	=	E_GrNetPlugNtpStageSend;
				m_TimeOut	=	10;
			}

		}
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugNtp::LcRsltFail( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrNetPlugNtpReqMax;Tv_WkIdx++ )
		{
			if ( E_GrNetPlugNtpReqStatWait == m_ReqTbl[Tv_WkIdx].Stat )
			{
				m_ReqTbl[Tv_WkIdx].RecvTime	=	0;
				m_ReqTbl[Tv_WkIdx].Stat			=	E_GrNetPlugNtpReqStatFail;
			}
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugNtp::LcStageDns( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_IpCnt;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( ((Cls_GrNetSvs*)m_NetSvs)->ImmDnsResult( m_Hdns, &m_Ip, 1, &Tv_IpCnt, FALSE ) )
		{
			// check IP
			if ( 0 != m_Ip )
			{
				// send
				m_Stage		=	E_GrNetPlugNtpStageSend;
				Tv_Result	=	TRUE;
			}
			else
			{
				LcRsltFail();
				m_Stage		=	E_GrNetPlugNtpStageNone;
				DbgMsgPrint( "Cls_GrNetPlugNtp::LcStageDns - DNS fail.\n" );
			}
			// release DNS
			((Cls_GrNetSvs*)m_NetSvs)->ImmDnsFree( m_Hdns, FALSE );
			m_Hdns	=	-1;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugNtp::LcStageSend( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_GrTime	Tv_TimeNow;
		__u32	Tv_Mili;
		__u32	Tv_Val;
	// code --------------------
		// init
		Tv_Result	=	FALSE;
		// stage send
		m_Hsock	=	GrSockV2Open( TRUE, TRUE, FALSE );
		if ( E_GrSockBadHandle != m_Hsock )
		{
			// setup
			GrSockV2UdpPortAssign( m_Hsock, E_GrNetPlugNtpPort );

			Tv_TimeNow	=	GrTimeSysTimeGet( &Tv_Mili );
			Tv_TimeNow	=	(Def_GrTime)( (__s32)Tv_TimeNow - m_Gmt );
			Tv_Val			=	GrTimeCvtSysTimeToGtod( Tv_TimeNow ) + E_GrNetPlugNtpTimeAdder;
			GrDumyEndian4byteSet( &m_PktSend.xmt.Int, Tv_Val );

			Tv_Mili			=	Tv_Mili << 10;
			Tv_Val			=	( 4294 * (Tv_Mili) + ((1981 * (Tv_Mili)) >> 11) );
			GrDumyEndian4byteSet( &m_PktSend.xmt.Frc, Tv_Val );

			// send
			if ( sizeof(m_PktSend) == GrSockV2UdpSend( m_Hsock, &m_Ip, E_GrNetPlugNtpPort, &m_PktSend, sizeof(m_PktSend), FALSE ) )
			{
				m_Stage		=	E_GrNetPlugNtpStageRecv;
				m_TimeOut	=	10;
				Tv_Result	=	TRUE;
			}
			else
			{
				GrSockV2Close( m_Hsock );
				m_Hsock		=	E_GrSockBadHandle;
				m_Stage		=	E_GrNetPlugNtpStageNone;
				DbgMsgPrint( "Cls_GrNetPlugNtp::LcStageSend - Send fail.\n" );
				LcRsltFail();
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrNetPlugNtp::LcStageSend - Handle create fail.\n" );
		}
	
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugNtp::LcStageRecv( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsOk;
		__u8	Tv_ValBt;
		__u8	Tv_WkIdx;
		__u16	Tv_Port;
		__u32	Tv_Read;
		__u32	Tv_Ip;
		__u32	Tv_ValDw;
		Def_GrTime	Tv_Time;
		Def_StrTag	Tv_StrTime;
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Ip		=	m_Ip;
		Tv_Read	=	GrSockV2UdpRead(m_Hsock, &Tv_Ip, &Tv_Port, &m_PktRecv, sizeof(m_PktRecv), FALSE, &Tv_ErrCode);
		if ( E_GrNetPlugNtpPktLenMinNoMac <= Tv_Read )
		{
			// check packet authenticity
			Tv_IsOk	=	TRUE;
			// check version
			Tv_ValBt	=	(m_PktRecv.li_vn_mode >> E_GrNetPlugNtpPrtcVnBit) & E_GrNetPlugNtpPrtcVnMask;
			if ( (1 > Tv_ValBt) || (E_GrNetPlugNtpPrtcVnVal < Tv_ValBt) )
			{
				DbgMsgPrint( "Cls_GrNetPlugNtp::LcStageRecv - version (%d) is bad.\n", Tv_ValBt );
				Tv_IsOk	=	FALSE;
			}
			// mode
			Tv_ValBt	=	(m_PktRecv.li_vn_mode >> E_GrNetPlugNtpPrtcMdBit) & E_GrNetPlugNtpPrtcMdMask;
			if ( (E_GrNetPlugNtpPrtcMdValPas != Tv_ValBt) && (E_GrNetPlugNtpPrtcMdValSvr != Tv_ValBt) )
			{
				DbgMsgPrint( "Cls_GrNetPlugNtp::LcStageRecv - mode (%d) is bad.\n", Tv_ValBt );
				Tv_IsOk	=	FALSE;
			}
			// stratum
			if ( E_GrNetPlugNtpPrtcStumUnspec <= m_PktRecv.stratum )
			{
				DbgMsgPrint( "Cls_GrNetPlugNtp::LcStageRecv - stratum (%d) is bad.\n", m_PktRecv.stratum );
				Tv_IsOk	=	FALSE;
			}
			// check special
			Tv_ValBt	=	(m_PktRecv.li_vn_mode >> E_GrNetPlugNtpPrtcLiBit) & E_GrNetPlugNtpPrtcLiMask;
			if ( (E_GrNetPlugNtpPrtcLiValNosync == Tv_ValBt) && (0 == m_PktRecv.stratum) && (E_GrNetPlugNtpPrtcRefIdDeath == m_PktRecv.refid) )
			{
				DbgMsgPrint( "Cls_GrNetPlugNtp::LcStageRecv - death code.\n" );
				Tv_IsOk	=	FALSE;
			}

			// check OK
			if ( Tv_IsOk )
			{
				// notify included authenticate data
				if ( E_GrNetPlugNtpPktLenMinNoMac < Tv_Read )
				{
					DbgMsgPrint( "Cls_GrNetPlugNtp::LcStageRecv - packet authenticate data included.\n" );
				}

				// get time
				Tv_ValDw	=	GrDumyEndian4byteGet( &m_PktRecv.xmt.Int );
				Tv_ValDw	=	Tv_ValDw - E_GrNetPlugNtpTimeAdder;
				Tv_Time		=	GrTimeCvtGtodToSysTime( Tv_ValDw );
				Tv_Time		=	(Def_GrTime)( (__s32)Tv_Time + m_Gmt );

				GrStrTimeToStr( Tv_StrTime, Tv_Time );
				DbgMsgPrint( "NTP:time receive success! ip = %08X, refid = %08X, time = %s \n", m_Ip, m_PktRecv.refid, Tv_StrTime );

				// update success
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrNetPlugNtpReqMax;Tv_WkIdx++ )
				{
					if ( E_GrNetPlugNtpReqStatWait == m_ReqTbl[Tv_WkIdx].Stat )
					{
						m_ReqTbl[Tv_WkIdx].Stat	=	E_GrNetPlugNtpReqStatRecv;
						m_ReqTbl[Tv_WkIdx].RecvTime	=	Tv_Time;
					}
				}
				
				// go next stage
				m_DnsUpdtPeriod ++;
				m_Stage		=	E_GrNetPlugNtpStageFinish;

			}
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugNtp::LcStageFinish( void )
{
	// local -------------------
	// code --------------------
		GrSockV2Close( m_Hsock );
		m_Hsock	=	E_GrSockBadHandle;

		m_Stage	=	E_GrNetPlugNtpStageNone;

		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugNtp::LcTmrStart( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		if ( E_GrNetPlugNtpStageNone == m_Stage )
		{
			// check exist requested slot
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrNetPlugNtpReqMax;Tv_WkIdx++ )
			{
				if ( E_GrNetPlugNtpReqStatWait == m_ReqTbl[Tv_WkIdx].Stat )
				{
					//found
					LcStart();
					break;
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNetPlugNtp::NtpTimeGet(__s32 A_Hndl, Def_GrTime* A_PtrRtTime)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result			=	FALSE;
		*A_PtrRtTime	=	0;
		if ( E_GrNetPlugNtpReqMax > (__u32)A_Hndl )
		{
			if (E_GrNetPlugNtpReqStatRecv <= m_ReqTbl[A_Hndl].Stat)
			{
				if (E_GrNetPlugNtpReqStatRecv == m_ReqTbl[A_Hndl].Stat)
				{
					*A_PtrRtTime	=	m_ReqTbl[A_Hndl].RecvTime;
				}
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNetPlugNtp::NtpFree(__s32 A_Hndl)
{
	// local -------------------
	// code --------------------
		if ( E_GrNetPlugNtpReqMax > (__u32)A_Hndl )
		{
			if (E_GrNetPlugNtpReqStatNone != m_ReqTbl[A_Hndl].Stat)
			{
				m_ReqTbl[A_Hndl].Stat			=	E_GrNetPlugNtpReqStatNone;
				m_ReqTbl[A_Hndl].RecvTime	=	0;
			}
		}
}
//--------------------------------------------------------------------
