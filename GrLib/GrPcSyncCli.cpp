/*
	NTP Client


*/

#include	<GrPcSyncCli.h>
#include	<GrStrTool.h>
#include	<GrProcess.h>
#include	<GrDumyTool.h>
#include	<GrTimeTool.h>
#include	<GrDebug.h>

#ifdef LINUX_APP

#include	<arpa/inet.h>

#endif

//--------------------------------------------------------------------
//local const
#define E_GrPcSyncCliPort					445
#define E_GrPcSyncCliSendSize			0xc2
#define E_GrPcSyncCliSmbSize			39
#define E_GrPcSyncCliTimeFixUp		11644473600LL
#define E_GrPcSyncCliSmbNegProt		0x72		//negotiate protocol

#define E_GrPcSyncCliMsgCmdReqGetTime		0		/* request time : St_GrPcSyncCliMsgReqTime */

//--------------------------------------------------------------------
//local type
enum	protocol_types
{
	PROTOCOL_NONE,
	PROTOCOL_CORE,
	PROTOCOL_COREPLUS,
	PROTOCOL_LANMAN1,
	PROTOCOL_LANMAN2,
	PROTOCOL_NT1
};

typedef	struct St_GrPcSyncCliMsgBase 
{
	St_GrTaskMsgHd	Hd;
	__u32	Cmd;
}	*Ptr_GrPcSyncCliMsgBase;

typedef	struct St_GrPcSyncCliMsgReqTime 
{
	St_GrPcSyncCliMsgBase	PcSyncHd;
	Cls_GrTaskCli* CbObj;
	__u32 Para;
	__u32	TimeOut;
	__u32	Ip;
	__u16	Port;
	BOOL8*	PtrResult;
}	*Ptr_GrPcSyncCliMsgReqTime;

//--------------------------------------------------------------------
//local macro

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

//====================================================================
//--------------------------------------------------------------------
Cls_GrPcSyncCli::Cls_GrPcSyncCli( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		GrSockInit();

		m_CbCli			=	NULL;
		m_CbPara		=	0;
		m_HsockSend		=	E_GrSockBadHandle;

		m_TaskSvr->RegistBkgClient( this );
		m_TaskSvr->RegistMsgClient( this );
}
//--------------------------------------------------------------------
Cls_GrPcSyncCli::~Cls_GrPcSyncCli( void )
{
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistMsgClient( this );

		GrTaskSvrMsgCmdFree( m_MsgAloc );

		GrSockFinish();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPcSyncCli::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( LcBgpTimeRecv() )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPcSyncCli::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			//check command
			if ( E_GrPcSyncCliMsgCmdReqGetTime == ((Ptr_GrPcSyncCliMsgBase)A_PtrMsg)->Cmd )
			{
				//request time
				*((Ptr_GrPcSyncCliMsgReqTime)A_PtrMsg)->PtrResult	=	LcProcMsgReqTime( ((Ptr_GrPcSyncCliMsgReqTime)A_PtrMsg)->Ip,
					((Ptr_GrPcSyncCliMsgReqTime)A_PtrMsg)->CbObj, ((Ptr_GrPcSyncCliMsgReqTime)A_PtrMsg)->Para,
					((Ptr_GrPcSyncCliMsgReqTime)A_PtrMsg)->TimeOut );
			}

			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPcSyncCli::LcProcMsgReqTime( __u32 A_Ip, Cls_GrTaskCli* A_CbObj, __u32 A_Para, __u32 A_TickTimeOut )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Pkt[E_GrPcSyncCliSendSize];
		St_GrPcSyncCliCbMsgTimeGet	Tv_Msg;
		__s32		Tv_Sended;
	// code --------------------
		Tv_Result	=	TRUE;

		//check already requested, and able para
		if ( (NULL == m_CbCli) && (NULL != A_CbObj) )
		{
			//listen mode
			m_HsockSend	=	GrSockOpen( FALSE, TRUE );
			if ( (E_GrSockBadHandle != m_HsockSend) )
			{
				m_Ip	=	A_Ip;
				if ( E_GrSockConnOk != GrSockConnect(m_HsockSend, m_Ip, E_GrPcSyncCliPort) )
				{
					GrPrcSleep(1);
					if ( E_GrSockConnOk != GrSockConnect(m_HsockSend, m_Ip, E_GrPcSyncCliPort) )
					{
						GrPrcSleep(1);
						if ( E_GrSockConnOk != GrSockConnect(m_HsockSend, m_Ip, E_GrPcSyncCliPort) )
						{
							GrPrcSleep(1);
							if ( E_GrSockConnOk != GrSockConnect(m_HsockSend, m_Ip, E_GrPcSyncCliPort) )
							{
								Tv_Result	=	FALSE;
							}
						}
					}
				}

				if ( Tv_Result )
				{
					//init
					m_Sended	=	0;
					Tv_Result	=	FALSE;
					//make packet
					LcMakePacket( Tv_Pkt );

					Tv_Sended	= GrSockSend( m_HsockSend, Tv_Pkt, E_GrPcSyncCliSendSize );
					if ( E_GrPcSyncCliSendSize <= Tv_Sended )
					{
						m_CbCli				=	A_CbObj;
						m_CbPara			=	A_Para;
						m_TickTmOut		=	A_TickTimeOut;
						m_TickLast		=	GrTimeGetTick();
						Tv_Result			=	TRUE;
					}
					else
					{
						DbgMsgPrint("Cls_GrPcSyncCli::LcProcMsgReqTime - packet send fail. - ip:%d:%d:%d:%d\n",
							((m_Ip>>0)&0xFF), ((m_Ip>>8)&0xFF), ((m_Ip>>16)&0xFF), ((m_Ip>>24)&0xFF) );
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrPcSyncCli::LcProcMsgReqTime - socket connect fail. - ip:%d:%d:%d:%d\n",
						((m_Ip>>0)&0xFF), ((m_Ip>>8)&0xFF), ((m_Ip>>16)&0xFF), ((m_Ip>>24)&0xFF) );
				}

				//check not success
				if ( !Tv_Result )
				{
					//socket close
					GrSockClose( m_HsockSend );
					m_HsockSend		=	E_GrSockBadHandle;
					//return failure
					//send command
					Tv_Msg.Hd.Cmd			=	E_GrPcSyncCliCbCmdTimeGet;
					Tv_Msg.Para				=	m_CbPara;
					Tv_Msg.SysTime		=	0;
					Tv_Msg.IsSuccess	=	FALSE;
					A_CbObj->RtlCallback( (Ptr_GrTaskMsgHd)&Tv_Msg );
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrPcSyncCli::LcProcMsgReqTime - socket create fail\n");
				if ( E_GrSockBadHandle != m_HsockSend )
				{
					GrSockClose( m_HsockSend );
					m_HsockSend		=	E_GrSockBadHandle;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrPcSyncCli::LcMakePacket( PBYTE A_Pkt )
{
	// var --------------------------
		__s32			Tv_ProtsLen;
		__u32		Tv_WkCnt;
		PBYTE		Tv_PtrBuf;
		__u32		Tv_CopyLen;
	// code -------------------------
	//init
		GrDumyZeroMem( A_Pkt, E_GrPcSyncCliSmbSize );
		m_Prots[0].ProtType	=	PROTOCOL_CORE;
		m_Prots[0].ProtName	=	"PC NETWORK PROGRAM 1.0";
		m_Prots[1].ProtType	=	PROTOCOL_COREPLUS;
		m_Prots[1].ProtName	=	"MICROSOFT NETWORKS 1.03";
		m_Prots[2].ProtType	=	PROTOCOL_LANMAN1;
		m_Prots[2].ProtName	=	"MICROSOFT NETWORKS 3.0";
		m_Prots[3].ProtType	=	PROTOCOL_LANMAN1;
		m_Prots[3].ProtName	=	"LANMAN1.0";
		m_Prots[4].ProtType	=	PROTOCOL_LANMAN2;
		m_Prots[4].ProtName	=	"LM1.2X002";
		m_Prots[5].ProtType	=	PROTOCOL_LANMAN2;
		m_Prots[5].ProtName	=	"DOS LANMAN2.1";
		m_Prots[6].ProtType	=	PROTOCOL_LANMAN2;
		m_Prots[6].ProtName	=	"LANMAN2.1";
		m_Prots[7].ProtType	=	PROTOCOL_LANMAN2;
		m_Prots[7].ProtName	=	"Samba";
		m_Prots[8].ProtType	=	PROTOCOL_NT1;
		m_Prots[8].ProtName	=	"NT LANMAN 1.0";
		m_Prots[9].ProtType	=	PROTOCOL_NT1;
		m_Prots[9].ProtName	=	"NT LM 0.12";

		Tv_ProtsLen	=	0;
		for ( Tv_WkCnt = 0; Tv_WkCnt < E_GrPcSyncCliProtsCnt; Tv_WkCnt++ )
		{
			Tv_ProtsLen	+=	GrStrLen(m_Prots[Tv_WkCnt].ProtName) + 2;
		}

		A_Pkt[0]	= 0;
		A_Pkt[1]	= ((E_GrPcSyncCliSmbSize + Tv_ProtsLen - 4) & 0x10000) >> 16;
		A_Pkt[2]	= ((E_GrPcSyncCliSmbSize + Tv_ProtsLen - 4) & 0xFF00) >> 8;
		A_Pkt[3]	= (E_GrPcSyncCliSmbSize + Tv_ProtsLen - 4) & 0xFF;
		A_Pkt[4]	=	0xFF;
		A_Pkt[5]	=	'S';
		A_Pkt[6]	=	'M';
		A_Pkt[7]	=	'B';
		A_Pkt[8]	=	E_GrPcSyncCliSmbNegProt;

		A_Pkt[36]	=	0;
		A_Pkt[37]	=	(__u8)(Tv_ProtsLen & 0xFF);
		A_Pkt[38]	=	(__u8)(Tv_ProtsLen >> 8);
		A_Pkt[39]	=	2;

		GrDumyZeroMem( A_Pkt + 39, Tv_ProtsLen );
		Tv_PtrBuf = A_Pkt + 39;
		for ( Tv_WkCnt = 0; Tv_WkCnt < E_GrPcSyncCliProtsCnt; Tv_WkCnt++ )
		{
			*Tv_PtrBuf++	= 2;
			Tv_CopyLen		= GrStrLen( m_Prots[Tv_WkCnt].ProtName ) + 1;
			GrDumyCopyMem( Tv_PtrBuf, m_Prots[Tv_WkCnt].ProtName, Tv_CopyLen );

			//next
			Tv_PtrBuf	+=	Tv_CopyLen;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPcSyncCli::ReqTimeGet( __u32 A_Ip, Cls_GrTaskCli* A_CbObj, __u32 A_Para, __u32 A_TimeOutMs, __u16 A_Port )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrPcSyncCliMsgReqTime	Tv_Msg;
	// code --------------------
		Tv_Result			=	FALSE;

		Tv_Msg.PcSyncHd.Hd.Cmd	=	m_MsgAloc;
		Tv_Msg.PcSyncHd.Cmd			=	E_GrPcSyncCliMsgCmdReqGetTime;
		Tv_Msg.Ip							=	A_Ip;
		Tv_Msg.CbObj					=	A_CbObj;
		Tv_Msg.Para						=	A_Para;
		Tv_Msg.TimeOut				=	GrTimeMsToTick( A_TimeOutMs );
		Tv_Msg.Port						=	A_Port;
		Tv_Msg.PtrResult			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPcSyncCli::LcBgpTimeRecv( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Pkt[1024];
		Def_GrTime	Tv_Time;
		St_GrPcSyncCliCbMsgTimeGet	Tv_Msg;
		__u32	Tv_TickGap;
		__s32	Tv_Recved;
		__s32	Tv_PktSize;
		__u16	Tv_ProtsIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		//check exist request
		if ( NULL != m_CbCli )
		{
			//receive packet
			GrPrcSleep( 100 );		//wait receiving data
			GrDumyZeroMem( Tv_Pkt, sizeof(Tv_Pkt) );

			Tv_Result	=	FALSE;
			Tv_Recved	=	GrSockRecv( m_HsockSend, Tv_Pkt, 4 );
			if ( 0 < Tv_Recved )
			{
				Tv_PktSize = (__u32)( (Tv_Pkt[3]&0xFF) | ((Tv_Pkt[2]&0xFF) << 8) | ((Tv_Pkt[1]&0x01) << 16) );
				if ( Tv_PktSize > 0 )
				{
					Tv_Pkt[Tv_PktSize]			=	0;
					Tv_Pkt[Tv_PktSize + 1]	=	0;
					Tv_Recved	=	GrSockRecv( m_HsockSend, Tv_Pkt + 4, Tv_PktSize );
					if ( 0 < Tv_Recved )
					{
						GrDumyCopyMem( &Tv_ProtsIdx, Tv_Pkt + 37, sizeof(__u16) );
						if ( PROTOCOL_NT1 <= m_Prots[Tv_ProtsIdx].ProtType )
						{
							Tv_Time		=	LcGetPacketToTime( Tv_Pkt + 59 + 1 );
							Tv_Result	=	TRUE;
						}
					}
				}

				if ( Tv_Result )
				{
					//send command
					Tv_Msg.IsSuccess	=	TRUE;
					Tv_Msg.Hd.Cmd			=	E_GrPcSyncCliCbCmdTimeGet;
					Tv_Msg.Para				=	m_CbPara;
					Tv_Msg.SysTime		=	Tv_Time;
					m_CbCli->RtlCallback( (Ptr_GrTaskMsgHd)&Tv_Msg );

					//clear 
					GrSockClose( m_HsockSend );
					m_HsockSend		=	E_GrSockBadHandle;
					m_CbCli		=	NULL;
					m_CbPara	=	0;
				}
				else
				{
					Tv_Msg.IsSuccess	=	FALSE;
					DbgMsgPrint("NTP:receive data fail.\n");
				}
			}
			else
			{
				//check time over
				Tv_TickGap	=	GrTimeGetTick() - m_TickLast;
				if ( Tv_TickGap >= m_TickTmOut )
				{
					//finish
					Tv_Msg.Hd.Cmd			=	E_GrPcSyncCliCbCmdTimeGet;
					Tv_Msg.Para				=	m_CbPara;
					Tv_Msg.SysTime		=	0;
					Tv_Msg.IsSuccess	=	FALSE;
					m_CbCli->RtlCallback( (Ptr_GrTaskMsgHd)&Tv_Msg );

					//clear 
					GrSockClose( m_HsockSend );
					m_HsockSend		=	E_GrSockBadHandle;
					m_CbCli		=	NULL;
					m_CbPara	=	0;
					Tv_Result	=	TRUE;

					DbgMsgPrint("pc sync: time out. ip = %d:%d:%d:%d\n",
						((m_Ip>>0)&0xFF), ((m_Ip>>8)&0xFF), ((m_Ip>>16)&0xFF), ((m_Ip>>24)&0xFF) );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrPcSyncCli::LcGetPacketToTime( PBYTE A_PtrBuf )
{
	// var -----------------------
		Def_GrTime		Tv_Time;
		__u64		Tv_QTime;
		__u16		Tv_Year;
		__u8		Tv_Month, Tv_Day, Tv_Hour, Tv_Min, Tv_Sec;
	// code ----------------------
		GrDumyCopyMem( &Tv_QTime, A_PtrBuf, sizeof(__u64) );

		//convert to seconds
		Tv_QTime	= Tv_QTime / (1000 * 1000 * 10);
		//now adjust by 369 years to make the secs since 1970
		Tv_QTime	= Tv_QTime - E_GrPcSyncCliTimeFixUp;
		//convert to grtime
		Tv_Time		= (__u32)Tv_QTime;
		Tv_Time		=	GrTimeCvtGtodToSysTime( Tv_Time );

		//debug
		GrTimeDec( Tv_Time, &Tv_Year, &Tv_Month, &Tv_Day,	&Tv_Hour,&Tv_Min, &Tv_Sec );
		DbgMsgPrint( "pc sync:time recive success! ip = %d:%d:%d:%d, time = %d/%02d/%02d %02d:%02d:%02d\n",
			((m_Ip>>0)&0xFF), ((m_Ip>>8)&0xFF), ((m_Ip>>16)&0xFF), ((m_Ip>>24)&0xFF),
			Tv_Year, Tv_Month, Tv_Day, Tv_Hour, Tv_Min, Tv_Sec);

		return	Tv_Time;
}
//--------------------------------------------------------------------


