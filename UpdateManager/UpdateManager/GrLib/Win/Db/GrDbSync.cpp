/*
product regist server

*/

//====================================================================
//include

#include <Win/Db/GrDbSync.h>

#include <GrDebug.h>
#include <GrError.h>

#include <GrDumyTool.h>
#include <Win/Db/GrDbCtrl.h>


//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//global var

//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrDbSync::Cls_GrDbSync(Cls_GrTaskSvr* A_TaskSvr, void* A_ObjDbCtl, BOOL8 A_IsSvr, __u32 A_IpSv, __u16 A_Port) :
Cls_GrTaskCli()
{
	// local -------------------
		//__u32		Tv_WkIdx;
	// code --------------------
		// init
		m_TskSvr			=	A_TaskSvr;
		m_MsgCmd			=	GrTaskSvrMsgCmdAlloc();

		m_IsInited		=	FALSE;

		m_DbCtl				=	A_ObjDbCtl;

		m_IsServer		=	A_IsSvr;

		m_IpSv				=	A_IpSv;
		m_Port				=	A_Port;

		m_SockListen	=	E_GrSockBadHandle;
		m_SockConn		=	E_GrSockBadHandle;

		m_Stage				=	E_GrDbSyncStageWaitConn;

		m_IsCdsRun		=	FALSE;
		m_CdsGrpId		=	0;
		m_CdsKeyIdx		=	0;
		m_CdsBlkId		=	0;

		GrDumyZeroMem(m_GrpChgMap, sizeof(m_GrpChgMap));

		m_QuitCode		=	E_GrErrNone;

		m_PtrSendItm	=	NULL;

		((Cls_GrDbCtrl*)m_DbCtl)->ImmGrpInfoGet(m_GrpInfo);

		GrBufVqInit((Ptr_GrBufVarQue)m_SendQue, E_GrDbSyncSendBufSize, 16);

		m_Recved			=	0;

		// regist sock
		m_TskSvr->RegistMsgClient(this);
		m_TskSvr->RegistTimerClient(this, E_GrDbSyncTimerPeriod, &m_TmrSec);
		m_TskSvr->RegistBkgClient(this);


}
//--------------------------------------------------------------------
Cls_GrDbSync::~Cls_GrDbSync()
{
	// local -------------------
	// code --------------------

		ReqFinish();

		m_TskSvr->UnregistBkgClient(this);
		m_TskSvr->UnregistTimerClient(m_TmrSec);
		m_TskSvr->UnregistMsgClient(this);

		GrTaskSvrMsgCmdFree(m_MsgCmd);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbSync::RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(m_MsgCmd == A_PtrMsg->Cmd)
		{
			Tv_Result	=	TRUE;
			switch(((Ptr_GrDbSyncCmdBase)A_PtrMsg)->Cmd)
			{
				case E_GrDbSyncCmdInit:
					ImmInit();
					break;
				case E_GrDbSyncCmdFinish:
					ImmFinish();
					break;
				case E_GrDbSyncCmdChgDataAdd:
					ImmChgDataAdd(((Ptr_GrDbSyncCmdChgDataAdd)A_PtrMsg)->GrpId, ((Ptr_GrDbSyncCmdChgDataAdd)A_PtrMsg)->KeyId,
						((Ptr_GrDbSyncCmdChgDataAdd)A_PtrMsg)->BlkMap);
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbSync::RtlBkgProc(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		
		if(m_IsInited)
		{
			// stage work
			if(LcStageProc())
			{
				Tv_Result	=	TRUE;
			}

			// check do send
			if(E_GrDbSyncStageWaitConn < m_Stage)
			{
				if(LcSendProc())
				{
					Tv_Result	=	TRUE;
				}
			}

			// check do receive
			if(E_GrDbSyncStageWaitConn < m_Stage)
			{
				if(LcRecvProc())
				{
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDbSync::RtlTimer(__u32 A_Hndl)
{
	// local -------------------
	// code --------------------
		if(m_IsInited)
		{
			// check connected 
			if((E_GrDbSyncStageWaitConn < m_Stage) || ((!m_IsServer) && (E_GrDbSyncStageWaitConn == m_Stage)))
			{
				if(0 != m_TmOut)
				{
					m_TmOut --;
					if(0 == m_TmOut)
					{
						LcConnClose();
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDbSync::ImmInit(void)
{
	// local -------------------
	// code --------------------

		GrSockV2Init();

		m_IsInited	=	TRUE;

}
//--------------------------------------------------------------------
void	Cls_GrDbSync::ImmFinish(void)
{
	// local -------------------
	// code --------------------
		
		// close
		if(E_GrSockBadHandle != m_SockConn)
		{
			GrSockV2Close(m_SockConn);
			m_SockConn	=	E_GrSockBadHandle;
		}

		if(E_GrSockBadHandle != m_SockListen)
		{
			GrSockV2Close(m_SockListen);
			m_SockListen	=	E_GrSockBadHandle;
		}
		
		GrSockV2Finish();

		m_IsInited	=	FALSE;

}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcCliStageWaitConn(void)
{
	// local -------------------
		__u8	Tv_RtCode;
	// code --------------------
		// make socket
		if(E_GrSockBadHandle == m_SockConn)
		{
			m_SockConn	=	GrSockV2Open(FALSE, TRUE, FALSE);
			// try connect
			if(E_GrSockBadHandle != m_SockConn)
			{
				m_TmOut		=	20;
				Tv_RtCode	=	GrSockV2Connect(m_SockConn, &m_IpSv, m_Port, FALSE);
				if(E_GrSockV2ConnOk == Tv_RtCode)
				{
					// success go next stage
					m_TmOut		=	0;
					m_Stage	=	E_GrDbSyncStageReady;
				}
				else if(E_GrSockV2ConnWaiting != Tv_RtCode)
				{
					// connect fail
					m_TmOut				=	0;
					GrSockV2Close(m_SockConn);
					m_SockConn		=	E_GrSockBadHandle;

					DbgMsgPrint("Cls_GrDbSync::LcCliStageWaitConn - connect fail!\n");
				}
			}
		}
		else
		{
			// connecting..
			// check connected
			Tv_RtCode	=	GrSockV2WaitConnect(m_SockConn, 0);
			if(E_GrSockV2ConnOk == Tv_RtCode)
			{
				// go ok stage
				m_Stage	=	E_GrDbSyncStageReady;
			}
			else if(E_GrSockV2ConnWaiting != Tv_RtCode)
			{
				// connect fail
				GrSockV2Close(m_SockConn);
				m_SockConn	=	E_GrSockBadHandle;

				DbgMsgPrint("Cls_GrDbSync::LcCliStageWaitConn - connect fail!\n");
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbSync::LcRecvProc(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		void*	Tv_PtrTg;
		__u32	Tv_ReqSize;
		__s32	Tv_Recved;
		__u32	Tv_Pos;
		Ptr_GrDbSyncPrtcHead	Tv_PtrPktHd;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrTg		=	(void*)((Def_GrCalPtr)m_RecvBuf + (Def_GrCalPtr)m_Recved);

		// check head received
		if(sizeof(St_GrDbSyncPrtcHead) > m_Recved)
		{
			// receive head
			Tv_ReqSize	=	sizeof(St_GrDbSyncPrtcHead) - m_Recved;
			Tv_Recved		=	GrSockV2Recv(m_SockConn, Tv_PtrTg, Tv_ReqSize);
			if(0 < Tv_Recved)
			{
				m_Recved	=	m_Recved + (__u32)Tv_Recved;
				if(sizeof(St_GrDbSyncPrtcHead) <= m_Recved)
				{
					// check exist fcc
					if(E_GrDbSyncPrtcHdFcc != ((Ptr_GrDbSyncPrtcHead)m_RecvBuf)->Fcc)
					{
						// realign
						Tv_Pos	=	1;
						while((Tv_Pos + 4) <= m_Recved)
						{
							// check found
							if(('D' == m_RecvBuf[Tv_Pos]) && ('S' == m_RecvBuf[Tv_Pos + 1]) &&
								('Y' == m_RecvBuf[Tv_Pos + 2]) && ('N' == m_RecvBuf[Tv_Pos + 3]))
							{
								// found
								break;
							}
							// next
							Tv_Pos ++;
						}
						// move
						Tv_ReqSize	=	m_Recved - Tv_Pos;
						Tv_PtrTg		=	(void*)((Def_GrCalPtr)m_RecvBuf + (Def_GrCalPtr)Tv_Pos);
						GrDumyMoveMem(m_RecvBuf, Tv_PtrTg, Tv_ReqSize);
						// chage size
						m_Recved 		=	Tv_ReqSize;
					}
					else if((sizeof(St_GrDbSyncPrtcHead) > ((Ptr_GrDbSyncPrtcHead)m_RecvBuf)->TotSize) || 
						(E_GrDbSyncRecvBufSize < ((Ptr_GrDbSyncPrtcHead)m_RecvBuf)->TotSize))
					{
						DbgMsgPrint("Cls_GrDbSync::LcRecvProc - bad packet size %d, cmd %d\n", 
							((Ptr_GrDbSyncPrtcHead)m_RecvBuf)->TotSize, ((Ptr_GrDbSyncPrtcHead)m_RecvBuf)->Cmd);
						m_Recved	=	0;
					}
				}
				// update time out
				m_TmOut			=	E_GrDbSyncTimeOut;
				Tv_Result		=	TRUE;
			}
			else
			{
				if(GrSockV2ChkCloseByRecv(Tv_Recved))
				{
					// stage move to close
					LcConnClose();
				}
			}
		}
		else
		{
			// receive payload
			Tv_PtrPktHd	=	(Ptr_GrDbSyncPrtcHead)m_RecvBuf;
			Tv_ReqSize	=	Tv_PtrPktHd->TotSize - m_Recved;
			Tv_Recved		=	GrSockV2Recv(m_SockConn, Tv_PtrTg, Tv_ReqSize);
			if(0 < Tv_Recved)
			{
				m_Recved	=	m_Recved + (__u32)Tv_Recved;
				// update time out
				m_TmOut			=	E_GrDbSyncTimeOut;
				Tv_Result		=	TRUE;
			}
			else
			{
				if(GrSockV2ChkCloseByRecv(Tv_Recved))
				{
					// stage move to close
					LcConnClose();
				}
			}
		}

		// process command
		Tv_PtrPktHd	=	(Ptr_GrDbSyncPrtcHead)m_RecvBuf;
		if((sizeof(St_GrDbSyncPrtcHead) <= m_Recved) && (Tv_PtrPktHd->TotSize <= m_Recved))
		{
			// do parsing
			switch(Tv_PtrPktHd->Cmd)
			{
			case E_GrDbSyncPrtcCmdGrpInfo:
				LcRcvCmdGrpInfo();
				break;
			case E_GrDbSyncPrtcCmdKeyInfo:
				LcRcvCmdKeyInfo();
				break;
			case E_GrDbSyncPrtcCmdReqUpdt:
				LcRcvCmdReqUpdt();
				break;
			case E_GrDbSyncPrtcCmdData:
				LcRcvCmdDataSet();
				break;
			case E_GrDbSyncPrtcCmdSwitchHost:
				LcRcvCmdSwitchHost();
				break;
			case  E_GrDbSyncPrtcCmdError:
				LcRcvCmdError();
				break;
			default:
				DbgMsgPrint("Cls_GrDbSync::LcRecvProc - unknown command %d received.\n", Tv_PtrPktHd->Cmd);
				break;
			}

			// finish
			m_Recved	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbSync::LcSendProc(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		void*		Tv_PtrSend;
		__u32		Tv_ReqSize;
		__s32		Tv_Sended;
		Ptr_GrDbSyncSendPktItem	Tv_PtrItm;
		Ptr_GrDbSyncPrtcHead	Tv_PtrPktHd;
	// code --------------------

		// get now item
		Tv_PtrItm	=	(Ptr_GrDbSyncSendPktItem)GrBufVqGetOutPtr((Ptr_GrBufVarQue)m_SendQue);
		if(NULL == Tv_PtrItm)
		{
			return	FALSE;
		}

		Tv_Result	=	FALSE;

		// check send data
		Tv_ReqSize	=	Tv_PtrItm->Size - Tv_PtrItm->Sended;
		Tv_PtrSend	=	(void*)((Def_GrCalPtr)Tv_PtrItm + sizeof(St_GrDbSyncSendPktItem) + (Def_GrCalPtr)Tv_PtrItm->Sended);

		Tv_Sended		=	GrSockV2Send(m_SockConn, Tv_PtrSend, Tv_ReqSize);
		if(0 < Tv_Sended)
		{
			// worked
			Tv_Result	=	TRUE;
			m_TmOut		=	10;

			// update sended
			Tv_PtrItm->Sended	=	Tv_PtrItm->Sended + (__u32)Tv_Sended;
			if(Tv_PtrItm->Sended >= Tv_PtrItm->Size)
			{
				// send completed
				Tv_PtrPktHd	=	(Ptr_GrDbSyncPrtcHead)((Def_GrCalPtr)Tv_PtrItm + sizeof(St_GrDbSyncSendPktItem));
				switch(Tv_PtrPktHd->Cmd)
				{
					case E_GrDbSyncPrtcCmdKeyInfo:
						LcSendCplKeyInfo();
						break;
					case E_GrDbSyncPrtcCmdData:
						LcSendCplData();
						break;
					case E_GrDbSyncPrtcCmdError:
						LcSendCplError();
						break;
				}

				// next
				GrBufVqOut((Ptr_GrBufVarQue)m_SendQue);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcSvrStageWaitConn(void)
{
	// local -------------------
		__u16	Tv_PortCli;
		BOOL8	Tv_IsIpv6;
	// code --------------------

		// create listen socket
		if(E_GrSockBadHandle == m_SockListen)
		{
			m_SockListen	=	GrSockV2Open(FALSE, TRUE, FALSE);
			if(E_GrSockBadHandle != m_SockListen)
			{
				if(!GrSockV2SetListenMode(m_SockListen, m_Port, FALSE))
				{
					GrSockV2Close(m_SockListen);
					m_SockListen	=	E_GrSockBadHandle;
				}
			}
		}

		// wait accept
		if((E_GrSockBadHandle != m_SockListen) && (E_GrSockBadHandle == m_SockConn))
		{
			// try accept
			m_SockConn	=	GrSockV2Accept(m_SockListen, TRUE, &m_IpRmt, &Tv_PortCli, &Tv_IsIpv6);
			if(E_GrSockBadHandle != m_SockConn)
			{
				// do accepted
				m_Stage		=	E_GrDbSyncStageSendGroupInfo;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrDbSync::ImmChgDataAdd(__u32 A_GrpId, __u32 A_KeyId, __u32 A_BlkMap)
{
	// local -------------------
	// code --------------------
		// check group id
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return;
		}
		
		// check key id
		if((0 == A_KeyId) || (E_GrDbKeyMaxCnt < A_KeyId))
		{
			return;
		}

		m_GrpChgMap[A_GrpId][A_KeyId]	=	m_GrpChgMap[A_GrpId][A_KeyId] | A_BlkMap;

}
//--------------------------------------------------------------------
void	Cls_GrDbSync::ReqInit(Cls_GrTaskCli* A_DbCtl)
{
	// local -------------------
		St_GrDbSyncCmdInit	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Hd.Cmd			=	E_GrDbSyncCmdInit;

		Tv_Msg.DbCtl			=	A_DbCtl;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg,sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrDbSync::ReqFinish(void)
{
	// local -------------------
		St_GrDbSyncCmdFinish	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Hd.Cmd			=	E_GrDbSyncCmdFinish;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrDbSync::ReqChgDataAdd(__u32 A_GrpId, __u32 A_KeyId, __u32 A_BlkMap)
{
	// local -------------------
		St_GrDbSyncCmdChgDataAdd	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Hd.Cmd			=	E_GrDbSyncCmdChgDataAdd;
		Tv_Msg.GrpId			=	A_GrpId;
		Tv_Msg.KeyId			=	A_KeyId;
		Tv_Msg.BlkMap			=	A_BlkMap;

		m_TskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcStageSendGroupInfo(void)
{
	// local -------------------
		Ptr_GrDbSyncPrtcGrpInfo	Tv_PtrPrtc;
	// code --------------------
		// initialize
		m_ScanGrpId		=	0;
		m_ScanKeyId		=	0;

		// send prepare
		Tv_PtrPrtc	=	(Ptr_GrDbSyncPrtcGrpInfo)LcSendBufAlloc(sizeof(St_GrDbSyncPrtcGrpInfo));
		if(NULL == Tv_PtrPrtc)
		{
			DbgMsgPrint("Cls_GrDbSync::LcStageSendGroupInfo - not enough send buffer!\n");
			return;
		}

		// build head
		Tv_PtrPrtc->Hd.Fcc				=	E_GrDbSyncPrtcHdFcc;
		Tv_PtrPrtc->Hd.Cmd				=	E_GrDbSyncPrtcCmdGrpInfo;
		Tv_PtrPrtc->Hd.TotSize		=	sizeof(St_GrDbSyncPrtcGrpInfo);

		// copy group info
		GrDumyCopyMem(Tv_PtrPrtc->GrpInfo, m_GrpInfo, sizeof(St_GrDbGrpInfo) * E_GrDbGrpMaxCnt);

		// request
		LcSendDo(Tv_PtrPrtc->Hd.Cmd, sizeof(St_GrDbSyncPrtcGrpInfo));

		m_Stage		=	E_GrDbSyncStageSendKeyInfo;

}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcStageSendKeyInfo(void)
{
	// local -------------------
		Ptr_GrDbSyncPrtcKeyInfo	Tv_PtrHd;
		Ptr_GrDbKeyInfo	Tv_PtrKeyInfo;
		__u32	Tv_KeyIdProc;
		__u32	Tv_ReqSize;
	// code --------------------

		Tv_ReqSize	=	sizeof(St_GrDbSyncPrtcKeyInfo) + (sizeof(St_GrDbKeyInfo) * E_GrDbSyncKeyInfoSendUnitCnt);
		Tv_PtrHd		=	(Ptr_GrDbSyncPrtcKeyInfo)LcSendBufAlloc(Tv_ReqSize);
		if(NULL == Tv_PtrHd)
		{
			DbgMsgPrint("Cls_GrDbSync::LcStageSendKeyInfo - not enough send buffer!\n");
			return;
		}

		Tv_PtrHd->Hd.Fcc					=	E_GrDbSyncPrtcHdFcc;
		Tv_PtrHd->Hd.Cmd					=	E_GrDbSyncPrtcCmdKeyInfo;
		Tv_PtrHd->Hd.TotSize			=	sizeof(St_GrDbSyncPrtcKeyInfo);

		Tv_PtrHd->IsLast					=	FALSE;
		Tv_PtrHd->GrpId						=	0;
		Tv_PtrHd->Cnt							=	0;

		Tv_PtrKeyInfo							=	(Ptr_GrDbKeyInfo)((Def_GrCalPtr)Tv_PtrHd + (Def_GrCalPtr)sizeof(St_GrDbSyncPrtcKeyInfo));

		// check exist key and group
		while(E_GrDbSyncKeyInfoSendUnitCnt > Tv_PtrHd->Cnt)
		{
			// check group
			if(E_GrDbGrpMaxCnt <= m_ScanGrpId)
			{
				break;
			}
			// check exist group
			if(0 == m_GrpInfo[m_ScanGrpId].KeyValCnt)
			{
				// go next group
				m_ScanGrpId ++;
				m_ScanKeyId	=	0;
				continue;
			}
		
			// get key id table
			if(0 == m_ScanKeyId)
			{
				m_ScanKeyId ++;
			}
			Tv_PtrHd->GrpId	=	m_ScanGrpId;
			Tv_PtrHd->Cnt	=	((Cls_GrDbCtrl*)m_DbCtl)->ReqKeyInfosGet(m_ScanGrpId, m_ScanKeyId, E_GrDbSyncKeyInfoSendUnitCnt,
				Tv_PtrKeyInfo, &Tv_KeyIdProc);
			m_ScanKeyId	=	Tv_KeyIdProc;
			if(E_GrDbKeyMaxCnt < Tv_KeyIdProc)
			{
				// go next group
				m_ScanGrpId ++;
				m_ScanKeyId	=	0;
			}

			if(0 != Tv_PtrHd->Cnt)
			{
				break;
			}
		}

		// check end mark
		if(E_GrDbGrpMaxCnt <= m_ScanGrpId)
		{
			Tv_PtrHd->IsLast	=	TRUE;
		}
		// update size
		Tv_PtrHd->Hd.TotSize	=	Tv_PtrHd->Hd.TotSize + (sizeof(St_GrDbKeyInfo) * Tv_PtrHd->Cnt);

		// go send
		LcSendDo(Tv_PtrHd->Hd.Cmd,Tv_PtrHd->Hd.TotSize);

		// go wait
		m_Stage	=	E_GrDbSyncStageReady;

}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcSendCplKeyInfo(void)
{
	// local -------------------
	// code --------------------
		// go wait mode
		m_Stage		=	E_GrDbSyncStageSendKeyInfo;
		// check send completed
		if(E_GrDbGrpMaxCnt <= m_ScanGrpId)
		{
			m_Stage		=	E_GrDbSyncStageReady;
		}
		else
		{
			if(((m_ScanGrpId + 1) >= E_GrDbGrpMaxCnt) && (m_ScanKeyId > E_GrDbKeyMaxCnt))
			{
				m_Stage		=	E_GrDbSyncStageReady;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcRcvCmdGrpInfo(void)
{
	// local -------------------
		Ptr_GrDbSyncPrtcGrpInfo	Tv_PtrPtGrpInfo;
		__u32	Tv_GrpIdx;
		BOOL8	Tv_IsMismatch;
	// code --------------------
		// init
		Tv_PtrPtGrpInfo	=	(Ptr_GrDbSyncPrtcGrpInfo)m_RecvBuf;
		// check match group info
		Tv_IsMismatch	=	FALSE;
		for(Tv_GrpIdx = 0; Tv_GrpIdx < E_GrDbGrpMaxCnt; Tv_GrpIdx++)
		{
			if((m_GrpInfo[Tv_GrpIdx].KeyValType != Tv_PtrPtGrpInfo->GrpInfo[Tv_GrpIdx].KeyValType) || 
				(m_GrpInfo[Tv_GrpIdx].KeyValCnt!= Tv_PtrPtGrpInfo->GrpInfo[Tv_GrpIdx].KeyValCnt))
			{
				DbgMsgPrint("Cls_GrDbSync::LcRcvCmdGrpInfo - group %d key val type %d:%d key size %d:%d mismatch!\n",
					Tv_GrpIdx,
					m_GrpInfo[Tv_GrpIdx].KeyValType, Tv_PtrPtGrpInfo->GrpInfo[Tv_GrpIdx].KeyValType,
					m_GrpInfo[Tv_GrpIdx].KeyValCnt, Tv_PtrPtGrpInfo->GrpInfo[Tv_GrpIdx].KeyValCnt);
				Tv_IsMismatch	=	TRUE;
				break;
			}
		}

		// check need send problem occure
		if(Tv_IsMismatch)
		{
			m_QuitCode	=	E_GrDbSyncQuitGrpMismatch;
			m_Stage			=	E_GrDbSyncStageQuit;
		}

}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcRcvCmdKeyInfo(void)
{
	// local -------------------
		Ptr_GrDbSyncPrtcKeyInfo	Tv_PtrPtKeyInfo;
		Ptr_GrDbSyncPrtcReqUpdt	Tv_PtrPtReqUpdt;
		Ptr_GrDbKeyInfo					Tv_PtrKeyInfo;
		Ptr_GrDbDataChgInfo			Tv_PtrDataChgInfo;
		__u32	Tv_ReqSize;
	// code --------------------
		// init
		Tv_PtrPtKeyInfo	=	(Ptr_GrDbSyncPrtcKeyInfo)m_RecvBuf;
		// calculate packet size
		//Tv_ReqSize			=	sizeof(St_GrDbSyncPrtcReqUpdt) + (sizeof(St_GrDbDataChgInfo) * Tv_PtrPtKeyInfo->Cnt);E_GrDbSyncKeyInfoSendUnitCnt
		Tv_ReqSize			=	sizeof(St_GrDbSyncPrtcReqUpdt) + (sizeof(St_GrDbDataChgInfo) * E_GrDbSyncKeyInfoSendUnitCnt);
		// allocate packet
		Tv_PtrPtReqUpdt	=	(Ptr_GrDbSyncPrtcReqUpdt)LcSendBufAlloc(Tv_ReqSize);
		if(NULL == Tv_PtrPtReqUpdt)
		{
			DbgMsgPrint("Cls_GrDbSync::LcRcvCmdKeyInfo - send buffer allocate fail!\n");
		}
		GrDumyZeroMem(Tv_PtrPtReqUpdt, Tv_ReqSize);
		// init header
		Tv_PtrPtReqUpdt->Hd.Fcc				=	E_GrDbSyncPrtcHdFcc;
		Tv_PtrPtReqUpdt->Hd.Cmd				=	E_GrDbSyncPrtcCmdReqUpdt;
		Tv_PtrPtReqUpdt->Hd.TotSize		=	sizeof(St_GrDbSyncPrtcReqUpdt);
		Tv_PtrPtReqUpdt->IsLast				=	Tv_PtrPtKeyInfo->IsLast;
		Tv_PtrPtReqUpdt->GrpId				=	Tv_PtrPtKeyInfo->GrpId;
		Tv_PtrPtReqUpdt->Cnt					=	0;

		// check key info
		if(0 != Tv_PtrPtKeyInfo->Cnt)
		{
			Tv_PtrKeyInfo									=	(Ptr_GrDbKeyInfo)((Def_GrCalPtr)Tv_PtrPtKeyInfo + sizeof(St_GrDbSyncPrtcKeyInfo));
			Tv_PtrDataChgInfo							=	(Ptr_GrDbDataChgInfo)((Def_GrCalPtr)Tv_PtrPtReqUpdt + sizeof(St_GrDbSyncPrtcReqUpdt));
			Tv_PtrPtReqUpdt->Cnt					=	((Cls_GrDbCtrl*)m_DbCtl)->ReqDataChgInfoMake(Tv_PtrPtKeyInfo->GrpId,
				Tv_PtrKeyInfo, Tv_PtrPtKeyInfo->Cnt, Tv_PtrDataChgInfo);

			Tv_PtrPtReqUpdt->Hd.TotSize	=	Tv_PtrPtReqUpdt->Hd.TotSize + (sizeof(St_GrDbDataChgInfo) * Tv_PtrPtReqUpdt->Cnt);
		}

		// do send
		LcSendDo(Tv_PtrPtReqUpdt->Hd.Cmd, Tv_PtrPtReqUpdt->Hd.TotSize);

}
//--------------------------------------------------------------------
void*	Cls_GrDbSync::LcSendBufAlloc(__u32 A_Size)
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		// allocate buffer
		m_PtrSendItm	=	(Ptr_GrDbSyncSendPktItem)GrBufVqGetInPtr((Ptr_GrBufVarQue)m_SendQue, A_Size);
		if(NULL == m_PtrSendItm)
		{
			return	NULL;
		}
		// allocate success
		m_PtrSendItm->Size	=	A_Size;
		Tv_Result	=	(void*)((Def_GrCalPtr)m_PtrSendItm + sizeof(St_GrDbSyncSendPktItem));
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbSync::LcSendDo(__u32 A_Cmd, __u32 A_Size)
{
	// local -------------------
	// code --------------------
		if(NULL == m_PtrSendItm)
		{
			return	FALSE;
		}
		// check request size
		if(m_PtrSendItm->Size < A_Size)
		{
			DbgMsgPrint("Cls_GrDbSync::LcSendDo - cmd %d bad send size (%d : %d)\n", 
				A_Cmd ,m_PtrSendItm->Size, A_Size);
			m_PtrSendItm	=	NULL;
			return	FALSE;
		}

		// set
		m_PtrSendItm->Sended	=	0;
		m_PtrSendItm->Size		=	A_Size;

		// insert to queue
		GrBufVqSizeIn((Ptr_GrBufVarQue)m_SendQue, A_Size);

		// done
		m_PtrSendItm	=	NULL;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbSync::LcDataKeyAtbSend(__u32 A_GrpId, __u32 A_KeyId)
{
	// local -------------------
		Ptr_GrDbSyncPrtcDataKeyAtb	Tv_PtrPtData;
		__u32	Tv_AlocSize;
	// code --------------------
		// allocate send buffer
		Tv_AlocSize		=	sizeof(St_GrDbSyncPrtcDataKeyAtb);
		Tv_PtrPtData	=	(Ptr_GrDbSyncPrtcDataKeyAtb)LcSendBufAlloc(Tv_AlocSize);
		if (NULL == Tv_PtrPtData)
		{
			return	FALSE;
		}
		
		// get key information
		if(((Cls_GrDbCtrl*)m_DbCtl)->ReqKeyAtbGetByKeyId(A_GrpId, A_KeyId, &Tv_PtrPtData->KeyAtb))
		{
			// build header
			Tv_PtrPtData->Hd.Fcc			=	E_GrDbSyncPrtcHdFcc;
			Tv_PtrPtData->Hd.Cmd			=	E_GrDbSyncPrtcCmdData;
			Tv_PtrPtData->Hd.TotSize	=	sizeof(St_GrDbSyncPrtcDataKeyAtb);

			Tv_PtrPtData->GrpId				=	A_GrpId;
			Tv_PtrPtData->BlkId				=	0;

			// do send
			return	LcSendDo(Tv_PtrPtData->Hd.Cmd,Tv_AlocSize);
		}

		// key data not exist, do skip
		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbSync::LcDataBlockSend(__u32 A_GrpId, __u32 A_KeyId, __u32 A_BlkIdx)
{
	// local -------------------
		Ptr_GrDbSyncPrtcDataBlkVal	Tv_PtrPtData;
		__u32	Tv_AlocSize;
	// code --------------------
		// check group id
		if (E_GrDbGrpMaxCnt <= A_GrpId)
		{
			// bad group index, go next
			return	TRUE;
		}
		// check exist block
		if(E_GrDbDatBlkMaxCnt <= A_BlkIdx)
		{
			// skip block , go next
			return	TRUE;
		}

		// allocate send buffer
		Tv_AlocSize		=	sizeof(St_GrDbSyncPrtcDataBlkVal);
		Tv_PtrPtData	=	(Ptr_GrDbSyncPrtcDataBlkVal)LcSendBufAlloc(Tv_AlocSize);
		if (NULL == Tv_PtrPtData)
		{
			return	FALSE;
		}

		// get key value and block data
		if( ((Cls_GrDbCtrl*)m_DbCtl)->ReqReadBlkDataAndInfoByKeyId(A_GrpId,A_KeyId,A_BlkIdx,Tv_PtrPtData->Data,
			Tv_PtrPtData->Key, &Tv_PtrPtData->ChkSum, &Tv_PtrPtData->TimeUpdt))
		{
			// build head
			Tv_PtrPtData->Hd.Fcc			=	E_GrDbSyncPrtcHdFcc;
			Tv_PtrPtData->Hd.Cmd			=	E_GrDbSyncPrtcCmdData;
			Tv_PtrPtData->Hd.TotSize	=	sizeof(St_GrDbSyncPrtcDataBlkVal);

			Tv_PtrPtData->GrpId				=	A_GrpId;
			Tv_PtrPtData->BlkId				=	A_BlkIdx;

			// do send
			return	LcSendDo(Tv_PtrPtData->Hd.Cmd,Tv_AlocSize);
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbSync::LcSwitchHostSend(void)
{
	// local -------------------
		Ptr_GrDbSyncPrtcSwitchHost	Tv_PtrPtSwHost;
		__u32	Tv_AlocSize;
	// code --------------------
		// allocate send buffer
		Tv_AlocSize			=	sizeof(St_GrDbSyncPrtcSwitchHost);
		Tv_PtrPtSwHost	=	(Ptr_GrDbSyncPrtcSwitchHost)LcSendBufAlloc(Tv_AlocSize);
		if(NULL == Tv_PtrPtSwHost)
		{
			return	FALSE;
		}

		// build head
		Tv_PtrPtSwHost->Hd.Fcc			=	E_GrDbSyncPrtcHdFcc;
		Tv_PtrPtSwHost->Hd.Cmd			=	E_GrDbSyncPrtcCmdSwitchHost;
		Tv_PtrPtSwHost->Hd.TotSize	=	sizeof(St_GrDbSyncPrtcSwitchHost);

		// do send
		return	LcSendDo(Tv_PtrPtSwHost->Hd.Cmd,Tv_AlocSize);

}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcInitSend()
{
		m_IsCdsRun	=	FALSE;
		m_Stage			=	E_GrDbSyncStageSendGroupInfo;
		DbgMsgPrint("[SYNC] : init\n");
}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcSyncDataWork(void)
{
	// local -------------------
		__u16	Tv_BlkMask;
		__s32	Tv_FndGrpId;
	// code --------------------
		// init
		Tv_FndGrpId		=	-1;
		// check now group
		while (E_GrDbGrpMaxCnt > m_CdsGrpId)
		{
			// check key
			while(E_GrDbKeyMaxCnt > m_CdsKeyIdx)
			{
				// check changed data
				if(0 != m_GrpChgMap[m_CdsGrpId][m_CdsKeyIdx])
				{
					// exist
					if(0 == m_CdsBlkId)
					{
						// found - header send
						Tv_FndGrpId		=	(__s32)m_CdsGrpId;
						break;
					}
					else
					{
						// block work
						while(m_CdsBlkId <= E_GrDbDatBlkMaxCnt)
						{
							Tv_BlkMask = 1 << m_CdsBlkId;
							if (0 != (Tv_BlkMask & m_GrpChgMap[m_CdsGrpId][m_CdsKeyIdx]))
							{
								// found
								Tv_FndGrpId		= (__s32)m_CdsGrpId;
								break;
							}
							// next
							m_CdsBlkId ++;

						}	// while (m_CdsBlkId <= m_GrpBlkCnt[m_CdsGrpId])
					}
				}
				else
				{
					m_CdsBlkId	=	E_GrDbDatBlkMaxCnt + 1;
				}
				// check found
				if (0 <= Tv_FndGrpId)
				{
					break;
				}

				// check next
				if(E_GrDbDatBlkMaxCnt < m_CdsBlkId)
				{
					m_CdsKeyIdx ++;
					m_CdsBlkId	=	0;
				}
			}	// while (m_GrpInfo[m_CdsGrpId].KeyMaxCnt > m_CdsKeyIdx)

			// check found
			if(0 <= Tv_FndGrpId)
			{
				break;
			}
			
			// check go next group
			if(E_GrDbKeyMaxCnt <= m_CdsKeyIdx)
			{
				m_CdsGrpId ++;
				m_CdsKeyIdx	=	0;
				m_CdsBlkId	=	0;
			}
		}	//	while (E_GrDbGrpMaxCnt > m_CdsGrpId)

		// check found
		if (0 <= Tv_FndGrpId)
		{
			// check type
			if (0 == m_CdsBlkId)
			{
				// send key info
				if (LcDataKeyAtbSend(Tv_FndGrpId,m_CdsKeyIdx + 1))
				{
					// clear change mark
					m_GrpChgMap[Tv_FndGrpId][m_CdsKeyIdx]	=	m_GrpChgMap[Tv_FndGrpId][m_CdsKeyIdx] & 0xFFFFFFFE;
					// go next progress
					m_CdsBlkId ++;
				}
			}
			else
			{
				// send block
				if(LcDataBlockSend(Tv_FndGrpId,m_CdsKeyIdx + 1,m_CdsBlkId))
				{
					// clear change mark
					m_GrpChgMap[Tv_FndGrpId][m_CdsKeyIdx]	=	m_GrpChgMap[Tv_FndGrpId][m_CdsKeyIdx] & (~(1 << m_CdsBlkId));
					// increase work count
					m_CdsCnt ++;
					// go next progress
					m_CdsBlkId ++;
				}
			}
		}

		// check send finish
		if(E_GrDbGrpMaxCnt <= m_CdsGrpId)
		{
			// send switch host
			/*if(LcSwitchHostSend())
			{
				// do count print
				if(0 != m_CdsCnt)
				{
					DbgMsgPrint("[SYNC] %d block synced!\n", m_CdsCnt);
				}
				// switch host
				m_IsCdsRun	=	FALSE;
			}*/

			LcInitSend(); //test code
		}

}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcRcvCmdReqUpdt(void)
{
	// local -------------------
		Ptr_GrDbSyncPrtcReqUpdt	Tv_PtrPtReqUpdt;
		Ptr_GrDbDataChgInfo	Tv_PtrChgInfo;
	// code --------------------
		// init
		Tv_PtrPtReqUpdt	=	(Ptr_GrDbSyncPrtcReqUpdt)m_RecvBuf;

		// update change data
		if(E_GrDbGrpMaxCnt > Tv_PtrPtReqUpdt->GrpId)
		{
			// check exist change buffer
			// update
			Tv_PtrChgInfo	=	(Ptr_GrDbDataChgInfo)((Def_GrCalPtr)Tv_PtrPtReqUpdt + sizeof(St_GrDbSyncPrtcReqUpdt));
			if(0 != Tv_PtrPtReqUpdt->Cnt)
			{
				// mark change map
				((Cls_GrDbCtrl*)m_DbCtl)->ReqDataChgInfoToChgMap(Tv_PtrPtReqUpdt->GrpId, Tv_PtrChgInfo, Tv_PtrPtReqUpdt->Cnt,
					m_GrpChgMap[Tv_PtrPtReqUpdt->GrpId]);
			}
		}
		
		// do send first frame
		if(Tv_PtrPtReqUpdt->IsLast)
		{
			// do send sync data
			if(!m_IsCdsRun)
			{
				m_CdsGrpId	=	0;
				m_CdsKeyIdx	=	0;
				m_CdsBlkId	=	0;
				m_CdsCnt		=	0;
				m_IsCdsRun	=	TRUE;
				LcSyncDataWork();
			}
			else
			{
				DbgMsgPrint("Cls_GrDbSync::LcRcvCmdReqUpdt - change data send already!\n");
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcSendCplData(void)
{
	// local -------------------
	// code --------------------
		if(m_IsCdsRun)
		{
			LcSyncDataWork();
		}
}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcStageQuit(void)
{
	// local -------------------
		Ptr_GrDbSyncPrtcError	Tv_PtrPtErr;
		__u32	Tv_AlocSize;
	// code --------------------
		// send quit
		// allocate send buffer
		Tv_AlocSize			=	sizeof(St_GrDbSyncPrtcError);
		Tv_PtrPtErr			=	(Ptr_GrDbSyncPrtcError)LcSendBufAlloc(Tv_AlocSize);
		if(NULL == Tv_PtrPtErr)
		{
			return;
		}

		// build head
		Tv_PtrPtErr->Hd.Fcc				=	E_GrDbSyncPrtcHdFcc;
		Tv_PtrPtErr->Hd.Cmd				=	E_GrDbSyncPrtcCmdError;
		Tv_PtrPtErr->Hd.TotSize		=	sizeof(St_GrDbSyncPrtcError);
		Tv_PtrPtErr->ErrCode			=	m_QuitCode;

		// do send
		if(LcSendDo(Tv_PtrPtErr->Hd.Cmd,Tv_AlocSize))
		{
			m_Stage	=	E_GrDbSyncStageReady;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcSendCplError(void)
{
	// local -------------------
	// code --------------------
		// close socket
		GrSockV2Close(m_SockConn);
		m_SockConn	=	E_GrSockBadHandle;

		m_Stage			=	E_GrDbSyncStageWaitConn;
}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcRcvCmdError(void)
{
	// local -------------------
	// code --------------------
		LcConnClose();
}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcConnClose(void)
{
	// local -------------------
	// code --------------------
		// close socket
		if(E_GrSockBadHandle != m_SockConn)
		{
			GrSockV2Close(m_SockConn);
			m_SockConn	=	E_GrSockBadHandle;
		}

		// reset send buffer
		GrBufVqReset((Ptr_GrBufVarQue)m_SendQue);
		m_PtrSendItm	=	NULL;

		// reset receive buffer
		m_Recved		=	0;

		// reset
		m_IsCdsRun		=	FALSE;

		// go connect stage
		m_Stage			=	E_GrDbSyncStageWaitConn;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbSync::LcStageProc(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch(m_Stage)
		{
			case E_GrDbSyncStageWaitConn:
				if(m_IsServer)
				{
					LcSvrStageWaitConn();
				}
				else
				{
					LcCliStageWaitConn();
				}
				break;
			case E_GrDbSyncStageReady:
				Tv_Result	=	FALSE;
				break;
			case E_GrDbSyncStageSendGroupInfo:
				LcStageSendGroupInfo();
				break;
			case E_GrDbSyncStageSendKeyInfo:
				LcStageSendKeyInfo();
				break;
			case E_GrDbSyncStageQuit:
				LcStageQuit();
				break;

			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcRcvCmdDataSet(void)
{
	// local -------------------
		Ptr_GrDbSyncPrtcDataKeyAtb	Tv_PtrPtDataAtb;
		Ptr_GrDbSyncPrtcDataBlkVal	Tv_PtrPtDataVal;
	// code --------------------
		// init
		Tv_PtrPtDataAtb	=	(Ptr_GrDbSyncPrtcDataKeyAtb)m_RecvBuf;

		// check type
		/*if(0 == Tv_PtrPtDataAtb->BlkId)
		{
			// find key
			if(!((Cls_GrDbCtrl*)m_DbCtl)->ReqKeyAtbUpdate(Tv_PtrPtDataAtb->GrpId, &Tv_PtrPtDataAtb->KeyAtb,FALSE))
			{
				DbgMsgPrint("Cls_GrDbSync::LcRcvCmdDataSet - key attribute set fail!\n");
			}
		}
		else
		{
			Tv_PtrPtDataVal	=	(Ptr_GrDbSyncPrtcDataBlkVal)m_RecvBuf;
			// write block with update time
			if(!((Cls_GrDbCtrl*)m_DbCtl)->ReqWriteBlkAndTimeByKeyVal(Tv_PtrPtDataVal->GrpId, Tv_PtrPtDataVal->Key,
				Tv_PtrPtDataVal->BlkId, Tv_PtrPtDataVal->TimeUpdt, Tv_PtrPtDataVal->Data,FALSE))
			{
				DbgMsgPrint("Cls_GrDbSync::LcRcvCmdDataSet - block data write fail!\n");
			}
		}*/

		if(0 == Tv_PtrPtDataAtb->BlkId)
		{
			// find key
			if(!((Cls_GrDbCtrl*)m_DbCtl)->ReqKeyAtbUpdate(Tv_PtrPtDataAtb->GrpId, &Tv_PtrPtDataAtb->KeyAtb, TRUE))
			{
				DbgMsgPrint("Cls_GrDbSync::LcRcvCmdDataSet - key attribute set fail!\n");
			}
		}
		else
		{
			Tv_PtrPtDataVal	=	(Ptr_GrDbSyncPrtcDataBlkVal)m_RecvBuf;
			// write block with update time
			if(!((Cls_GrDbCtrl*)m_DbCtl)->ReqWriteBlkAndTimeByKeyVal(Tv_PtrPtDataVal->GrpId, Tv_PtrPtDataVal->Key,
				Tv_PtrPtDataVal->BlkId, Tv_PtrPtDataVal->TimeUpdt, Tv_PtrPtDataVal->Data, TRUE))
			{
				DbgMsgPrint("Cls_GrDbSync::LcRcvCmdDataSet - block data write fail!\n");
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDbSync::LcRcvCmdSwitchHost(void)
{
	// local -------------------
	// code --------------------
		m_Stage	=	E_GrDbSyncStageSendGroupInfo;
}
//--------------------------------------------------------------------
