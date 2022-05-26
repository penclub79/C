/*
	NTP Client


*/

#include <GrNtpCli.h>
#include <GrStrTool.h>
#include <GrProcess.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrDebug.h>
#include <GrNetBase.h>
#include <GrMath.h>

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

//====================================================================
//--------------------------------------------------------------------
Cls_GrNtpCli::Cls_GrNtpCli( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		m_Hdns			=	-1;
		m_Ip				=	0;
		m_Port			=	E_GrNtpCliDfltPort;
		GrStrClear(m_StrSvName);
		m_TimeRslt	=	0;
		m_Hsock			=	E_GrSockBadHandle;

		m_Gmt				=	9 * E_GrTimeSecOfHour;

		m_Stage			=	E_GrNtpCliStageNone;
		m_TimeOut		=	0;
		m_WaitTime	=	0;
		m_UpdtStep	=	24 * E_GrTimeSecOfHour;

		m_IsNotify	=	FALSE;

		// build packet
		GrDumyZeroMem(&m_PktSend, sizeof(m_PktSend));
		m_PktSend.li_vn_mode	=	(E_GrNtpCliPrtcLiValNwr << E_GrNtpCliPrtcLiBit) |
			(E_GrNtpCliPrtcVnVal << E_GrNtpCliPrtcVnBit) |
			(E_GrNtpCliPrtcMdValCli << E_GrNtpCliPrtcMdBit);
		m_PktSend.refid				=	E_GrNtpCliPrtcRefIdDflt;
		m_PktSend.ppoll				=	E_GrNtpCliPrtcPollDflt;
		m_PktSend.precision		=	E_GrNtpCliPrtcPrecDflt;

		m_TaskSvr->RegistBkgClient( this );
		m_TaskSvr->RegistTimerClient(this, E_GrNtpCliTimerPeriod, &m_HndlTimer);
		m_TaskSvr->RegistMsgClient( this );
}
//--------------------------------------------------------------------
Cls_GrNtpCli::~Cls_GrNtpCli( void )
{
		ReqFinish();

		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistTimerClient(m_HndlTimer);
		m_TaskSvr->UnregistMsgClient( this );

		GrTaskSvrMsgCmdFree( m_MsgAloc );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrNtpCli::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(E_GrNtpCliStageNone != m_Stage)
		{
			switch(m_Stage)
			{
				case E_GrNtpCliStageDns:
					//DbgMsgPrint("[NTP] stage dns\n");
					Tv_Result	=	LcStageDns();
					break;
				case E_GrNtpCliStageSend:
					//DbgMsgPrint("[NTP] stage send\n");
					Tv_Result	=	LcStageSend();
					break;
				case E_GrNtpCliStageRecv:
					//DbgMsgPrint("[NTP] stage recv\n");
					Tv_Result	=	LcStageRecv();
					break;
				case E_GrNtpCliStageFinish:
					//DbgMsgPrint("[NTP] stage finish\n");
					Tv_Result	=	LcStageFinish();
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNtpCli::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			//check command
			Tv_Result	=	TRUE;
			switch(((Ptr_GrNtpCliMsgHd)A_PtrMsg)->Cmd)
			{
				case E_GrNtpCliMsgCmdInit:
					ImmInit();
					break;
				case E_GrNtpCliMsgCmdFinish:
					ImmFinish();
					break;
				case E_GrNtpCliMsgCmdSetup:
					ImmSetup(((Ptr_GrNtpCliMsgSetup)A_PtrMsg)->StrUrl, ((Ptr_GrNtpCliMsgSetup)A_PtrMsg)->GmtSec,
						((Ptr_GrNtpCliMsgSetup)A_PtrMsg)->UpdtStepSec, ((Ptr_GrNtpCliMsgSetup)A_PtrMsg)->Port);
					break;
				case E_GrNtpCliMsgCmdRun:
					ImmRun();
					break;
				case E_GrNtpCliMsgCmdStop:
					ImmStop();
					break;
				case E_GrNtpCliMsgCmdUpdateNow:
					*((Ptr_GrNtpCliMsgUpdateNow)A_PtrMsg)->PtrRslt	=
						ImmUpdateNow(((Ptr_GrNtpCliMsgUpdateNow)A_PtrMsg)->IsNotify);
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrNtpCli::RtlTimer(__u32 A_Hndl)
{
	// local -------------------
	// code --------------------
		if(E_GrNtpCliStageNone != m_Stage)
		{
			if(E_GrNtpCliStageWait == m_Stage)
			{
				if(0 == m_WaitTime)
				{
					// retry
					ImmRun();
				}
				else
				{
					m_WaitTime --;
				}
			}
			else
			{
				if(0 == m_TimeOut )
				{
					LcStop();
					DbgMsgPrint("Cls_GrNtpCli::RtlTimer - NTP work time out! Stage = %d\n", m_Stage);
					m_Stage			=	E_GrNtpCliStageWait;
					m_WaitTime	=	2 * E_GrTimeSecOfMin;
				}
				else
				{
					m_TimeOut --;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrNtpCli::ImmInit(void)
{
	// local -------------------
	// code --------------------
		GrSockV2Init();
}
//--------------------------------------------------------------------
void	Cls_GrNtpCli::ImmFinish(void)
{
	// local -------------------
	// code --------------------
		LcStop();
		m_Stage	=	E_GrNtpCliStageNone;

		GrSockV2Finish();
}
//--------------------------------------------------------------------
void	Cls_GrNtpCli::ReqInit(void)
{
	// local -------------------
		St_GrNtpCliMsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrNtpCliMsgCmdInit;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrNtpCli::ReqFinish(void)
{
	// local -------------------
		St_GrNtpCliMsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrNtpCliMsgCmdFinish;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
BOOL8	Cls_GrNtpCli::LcStart(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(GrStrIsExist(m_StrSvName))
		{
			// init
			m_Ip		=	0;

			// DNS request
			m_Hdns	=	GrNetDnsOpen(m_StrSvName);
			if(0 <= m_Hdns)
			{
				m_Stage		=	E_GrNtpCliStageDns;
				m_TimeOut	=	20;
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint("Cls_GrNtpCli::LcStart - DNS open fail!\n");
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNtpCli::LcStageDns(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Cnt;
		__u8	Tv_ErrCode;
	// code --------------------
		Tv_Result	=	FALSE;

		// receive success
		Tv_ErrCode	=	GrNetDnsRead(m_Hdns, &m_Ip, 1, &Tv_Cnt);
		if((E_GrErrNone == Tv_ErrCode) || (E_GrErrBusy != Tv_ErrCode))
		{
			if((0 != Tv_Cnt) && (0 != m_Ip))
			{
				// go next stage
				m_Stage		=	E_GrNtpCliStageSend;
				Tv_Result	=	TRUE;
			}
			else
			{
				// fail
				DbgMsgPrint("Cls_GrNtpCli::LcStageDns - DNS read fail!\n");

				m_Stage			=	E_GrNtpCliStageWait;
				m_WaitTime	=	E_GrTimeSecOfMin;
			}

			// finish DNS
			GrNetDnsClose(m_Hdns);
			m_Hdns	=	-1;
			// check network off
			if(!GrSockV2IsLink(0))
			{
				DbgMsgPrint("Cls_GrNtpCli::LcStageDns - NET Cable disconnected!\n");
				m_Stage			=	E_GrNtpCliStageWait;
				m_WaitTime	=	m_UpdtStep;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNtpCli::LcStageSend(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_GrTime	Tv_TimeNow;
		__u32	Tv_Mili;
		__u32	Tv_Val;
	// code --------------------
		Tv_Result	=	FALSE;

		m_Hsock	=	GrSockV2Open(TRUE, TRUE, FALSE);
		if(E_GrSockBadHandle != m_Hsock)
		{
			// setup
			//GrSockV2UdpPortAssign(m_Hsock, m_Port);
			GrSockV2UdpIpAndPortAssign(m_Hsock, &m_Ip, m_Port, FALSE);

			Tv_TimeNow	=	GrTimeSysTimeGet(&Tv_Mili);
			Tv_TimeNow	=	(Def_GrTime)((__s32)Tv_TimeNow - m_Gmt);
			Tv_Val			=	GrTimeCvtSysTimeToGtod(Tv_TimeNow) + E_GrNtpCliTimeAdder;
			GrDumyEndian4byteSet(&m_PktSend.xmt.Int, Tv_Val);

			Tv_Mili			=	Tv_Mili << 10;
			Tv_Val			=	(4294 * (Tv_Mili)+ ((1981 * (Tv_Mili)) >> 11));
			GrDumyEndian4byteSet(&m_PktSend.xmt.Frc, Tv_Val);

			// send
			if(sizeof(m_PktSend) == GrSockV2UdpSend(m_Hsock, &m_Ip, m_Port, &m_PktSend, sizeof(m_PktSend), FALSE))
			{
				m_Stage		=	E_GrNtpCliStageRecv;
				m_TimeOut	=	10;
				Tv_Result	=	TRUE;
			}
			else
			{
				GrSockV2Close(m_Hsock);
				m_Hsock			=	E_GrSockBadHandle;
				m_Stage			=	E_GrNtpCliStageWait;
				m_WaitTime	=	60;
				DbgMsgPrint("Cls_GrNtpCli::LcStageSend - Send fail.\n");
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrNtpCli::LcStageSend - Handle create fail.\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNtpCli::LcStageRecv(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsOk;
		__u8	Tv_ValBt;
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
		if(E_GrNtpCliPktLenMinNoMac <= Tv_Read)
//		if(0 < Tv_Read)
		{
			// check packet authenticity
			Tv_IsOk	=	TRUE;
			// check version
			Tv_ValBt	=	(m_PktRecv.li_vn_mode >> E_GrNtpCliPrtcVnBit) & E_GrNtpCliPrtcVnMask;
			if((1 > Tv_ValBt) || (E_GrNtpCliPrtcVnVal < Tv_ValBt))
			{
				DbgMsgPrint("Cls_GrNtpCli::LcStageRecv - version (%d) is bad.\n", Tv_ValBt);
				Tv_IsOk	=	FALSE;
			}
			// mode
			Tv_ValBt	=	(m_PktRecv.li_vn_mode >> E_GrNtpCliPrtcMdBit) & E_GrNtpCliPrtcMdMask;
			if((E_GrNtpCliPrtcMdValPas != Tv_ValBt) && (E_GrNtpCliPrtcMdValSvr != Tv_ValBt))
			{
				DbgMsgPrint("Cls_GrNtpCli::LcStageRecv - mode (%d) is bad.\n", Tv_ValBt);
				Tv_IsOk	=	FALSE;
			}
			// stratum
			if(E_GrNtpCliPrtcStumUnspec <= m_PktRecv.stratum)
			{
				DbgMsgPrint("Cls_GrNtpCli::LcStageRecv - stratum (%d) is bad.\n", m_PktRecv.stratum);
				Tv_IsOk	=	FALSE;
			}
			// check special
			Tv_ValBt	=	(m_PktRecv.li_vn_mode >> E_GrNtpCliPrtcLiBit) & E_GrNtpCliPrtcLiMask;
			if((E_GrNtpCliPrtcLiValNosync == Tv_ValBt) && (0 == m_PktRecv.stratum) && (E_GrNtpCliPrtcRefIdDeath == m_PktRecv.refid))
			{
				DbgMsgPrint("Cls_GrNtpCli::LcStageRecv - death code.\n");
				Tv_IsOk	=	FALSE;
			}

			// check OK
			if(Tv_IsOk)
			{
				// notify included authenticate data
				if(E_GrNtpCliPktLenMinNoMac < Tv_Read)
				{
					DbgMsgPrint("Cls_GrNtpCli::LcStageRecv - packet authenticate data included.\n");
				}

				// get time
				Tv_ValDw	=	GrDumyEndian4byteGet(&m_PktRecv.xmt.Int);
				Tv_ValDw	=	Tv_ValDw - E_GrNtpCliTimeAdder;
				Tv_Time		=	GrTimeCvtGtodToSysTime(Tv_ValDw);
				Tv_Time		=	(Def_GrTime)((__s32)Tv_Time + m_Gmt);

				GrStrTimeToStr(Tv_StrTime, Tv_Time);
				DbgMsgPrint("NTP:time receive success! ip = %08X, refid = %08X, time = %s \n", m_Ip, m_PktRecv.refid, Tv_StrTime);

				// update success
				m_TimeRslt		=	Tv_Time;
				m_TimeOut			=	10;

				// go next stage
				m_Stage		=	E_GrNtpCliStageFinish;

			}
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNtpCli::LcStageFinish(void)
{
	// local -------------------
		__u32	Tv_TmSec;
		__u32	Tv_TmMili;
		St_GrTaskMsgNtpRslt	Tv_Msg;
	// code --------------------
		// upate time
		Tv_TmSec	= GrTimeSysTimeGet(&Tv_TmMili);
		if(2 <= GrAbs32((__s32)Tv_TmSec - (__s32)m_TimeRslt))
		{
			GrTimeSysTimeSet(m_TimeRslt);
		}

		// notify
		if(m_IsNotify)
		{
			Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgNtpRslt;
			Tv_Msg.IsOk				=	TRUE;
			GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

			m_IsNotify	=	FALSE;
		}

		// go wait stage
		m_WaitTime	=	m_UpdtStep;
		m_Stage			=	E_GrNtpCliStageWait;
		
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrNtpCli::LcStop(void)
{
	// local -------------------
		St_GrTaskMsgNtpRslt	Tv_Msg;
	// code --------------------
		if(m_IsNotify)
		{
			Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgNtpRslt;
			Tv_Msg.IsOk				=	FALSE;
			GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		}

		m_IsNotify	=	FALSE;

		// close dns
		if(0 <= m_Hdns)
		{
			GrNetDnsClose(m_Hdns);
			m_Hdns	=	-1;
		}

		// close socket
		if(E_GrSockBadHandle != m_Hsock)
		{
			GrSockV2Close(m_Hsock);
			m_Hsock	=	E_GrSockBadHandle;
		}

		m_Stage		=	E_GrNtpCliStageNone;
}
//--------------------------------------------------------------------
void	Cls_GrNtpCli::ImmSetup(char* A_StrSvUrl, __s32 A_GmtSec, __u32 A_UpdtStepSec, __u16 A_Port)
{
	// local -------------------
	// code --------------------
		// update url
		GrStrCopy(m_StrSvName, A_StrSvUrl);
		m_Port		=	A_Port;
		// update GMT
		m_Gmt		=	A_GmtSec;
		// update step
		m_UpdtStep	=	A_UpdtStepSec;
		if(E_GrTimeSecOfHour > m_UpdtStep)
		{
			m_UpdtStep	=	24 * E_GrTimeSecOfHour;
		}
}
//--------------------------------------------------------------------
void	Cls_GrNtpCli::ImmRun(void)
{
	// local -------------------
	// code --------------------
		LcStop();

		// check network linked
		if(GrSockV2IsLink(0))
		{
			if(!LcStart())
			{
				if(GrStrIsExist(m_StrSvName))
				{
					m_WaitTime	=	E_GrTimeSecOfMin;
					m_Stage			=	E_GrNtpCliStageWait;
				}
				else
				{
					m_Stage			=	E_GrNtpCliStageNone;
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrNtpCli::ImmRun - network not linked\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrNtpCli::ImmStop(void)
{
		LcStop();
}
//--------------------------------------------------------------------
void	Cls_GrNtpCli::ReqSetup(char* A_StrSvUrl, __s32 A_GmtSec, __u32 A_UpdtStepSec, __u16 A_Port)
{
	// local -------------------
		St_GrNtpCliMsgSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrNtpCliMsgCmdSetup;
		Tv_Msg.StrUrl				=	A_StrSvUrl;
		Tv_Msg.GmtSec				=	A_GmtSec;
		Tv_Msg.UpdtStepSec	=	A_UpdtStepSec;
		Tv_Msg.Port					=	A_Port;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrNtpCli::ReqRun(void)
{
	// local -------------------
		St_GrNtpCliMsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrNtpCliMsgCmdRun;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrNtpCli::ReqStop(void)
{
	// local -------------------
		St_GrNtpCliMsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrNtpCliMsgCmdStop;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNtpCli::ImmUpdateNow(BOOL8 A_IsRsltNotify)
{
	// local -------------------
		__u8	Tv_Stage;
	// code --------------------
		// check network status
		if(!GrSockV2IsLink(0))
		{
			return	FALSE;
		}

		// check stage
		if((E_GrNtpCliStageNone != m_Stage) && (E_GrNtpCliStageWait != m_Stage))
		{
			return	FALSE;
		}

		m_IsNotify	=	A_IsRsltNotify;
		
		Tv_Stage		=	m_Stage;
		if(!LcStart())
		{
			m_Stage		=	Tv_Stage;
			m_IsNotify	=	FALSE;
			return	FALSE;
		}

		return	TRUE;
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrNtpCli::ReqUpdateNow(BOOL8 A_IsRsltNotify)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrNtpCliMsgUpdateNow	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrNtpCliMsgCmdUpdateNow;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.IsNotify			=	A_IsRsltNotify;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------

