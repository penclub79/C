/*
product regist server

*/

//====================================================================
//include

#include <Win/Net/GrSvrIocpJob.h>

#include <GrDebug.h>
#include <GrError.h>

#include <GrDumyTool.h>


//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//global var


//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrSvrIocpJob::Cls_GrSvrIocpJob(Cls_GrTaskSvr* A_TaskSvr, __u32 A_MaxConnCnt, __u32 A_ConnUnitSize) :
Cls_GrTaskCli()
{
	// local -------------------
		__u32		Tv_ConnIdx;
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
	// code --------------------
		// init
		m_TskSvr				=	A_TaskSvr;
		m_MsgCmdJob			=	GrTaskSvrMsgCmdAlloc();

		// reset
		m_ConnCnt				=	0;
		m_HndlIocp			=	NULL;
		m_ConnMaxCnt		=	A_MaxConnCnt;
		m_ConnUnitSize	=	A_ConnUnitSize;

		m_ConnTbl			=	(void*)malloc(m_ConnMaxCnt * m_ConnUnitSize);
		if(NULL != m_ConnTbl)
		{
			GrDumyZeroMem(m_ConnTbl, m_ConnMaxCnt * m_ConnUnitSize);

			Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)m_ConnTbl;
			for(Tv_ConnIdx = 0; Tv_ConnIdx < m_ConnMaxCnt; Tv_ConnIdx++)
			{
				Tv_PtrConn->Sock		=	INVALID_SOCKET;

				// next
				Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)((Def_GrCalPtr)Tv_PtrConn + (Def_GrCalPtr)m_ConnUnitSize);
			}
		}
		else
		{
			m_ConnMaxCnt		=	0;
		}

		// regist 
		m_TskSvr->RegistMsgClient(this);
		m_TskSvr->RegistTimerClient(this, E_GrSvrIocpJobTimerPeriod, &m_TmrSec);
		m_TskSvr->RegistBkgClient(this);

}
//--------------------------------------------------------------------
Cls_GrSvrIocpJob::~Cls_GrSvrIocpJob()
{
	// local -------------------
	// code --------------------

		ReqFinish();

		m_TskSvr->UnregistBkgClient(this);
		m_TskSvr->UnregistTimerClient(m_TmrSec);
		m_TskSvr->UnregistMsgClient(this);

		GrTaskSvrMsgCmdFree(m_MsgCmdJob);

		if(NULL != m_ConnTbl)
		{
			free(m_ConnTbl);
			m_ConnTbl		=	NULL;
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_RtB8;
	// code --------------------
		Tv_Result	=	FALSE;
	
		if(m_MsgCmdJob == A_PtrMsg->Cmd)
		{
			Tv_Result	=	TRUE;
			switch(((Ptr_GrSvrIocpJobCmdBase)A_PtrMsg)->Cmd)
			{
				case E_GrSvrIocpJobCmdInit:
					*(((Ptr_GrSvrIocpJobCmdInit)A_PtrMsg)->PtrRslt)		=
						ImmInit();
					break;
				case E_GrSvrIocpJobCmdFinish:
					ImmFinish();
					break;
				case E_GrSvrIocpJobCmdConnAdd:
					Tv_RtB8	=	ImmConnAdd(((Ptr_GrSvrIocpJobCmdConnAdd)A_PtrMsg)->Sock, ((Ptr_GrSvrIocpJobCmdConnAdd)A_PtrMsg)->Ip,
						((Ptr_GrSvrIocpJobCmdConnAdd)A_PtrMsg)->Port);
					if(NULL != ((Ptr_GrSvrIocpJobCmdConnAdd)A_PtrMsg)->PtrRslt)
					{
						*(((Ptr_GrSvrIocpJobCmdConnAdd)A_PtrMsg)->PtrRslt)		=	Tv_RtB8;
					}
					break;
				case E_GrSvrIocpJobCmdCapRemainGet:
					*(((Ptr_GrSvrIocpJobCmdCapRemainGet)A_PtrMsg)->PtrRslt)	=	ImmCapRemainGet();
					break;
				case E_GrSvrIocpJobCmdConnClose:
					EvtClosed(((Ptr_GrSvrIocpJobCmdConnClose)A_PtrMsg)->HndlConn);
					Tv_RtB8	=	ConnDelete(((Ptr_GrSvrIocpJobCmdConnClose)A_PtrMsg)->HndlConn);
					if(NULL != ((Ptr_GrSvrIocpJobCmdConnClose)A_PtrMsg)->PtrRslt)
					{
						*(((Ptr_GrSvrIocpJobCmdConnClose)A_PtrMsg)->PtrRslt)		=	Tv_RtB8;
					}
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::RtlBkgProc(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(BgwJobProc())
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::RtlTimer(__u32 A_Hndl)
{
	// local -------------------
	// code --------------------
		if(m_TmrSec == A_Hndl)
		{
			LcUpdtTimeOut();

			LcUpdtTimer();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::ImmInit(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		//__u32	Tv_WkIdx;
		WORD	Tv_WsVer;
		WSADATA	Tv_WsData;
	// code --------------------
		Tv_Result	=	FALSE;

		if(NULL != m_ConnTbl)
		{
			Tv_WsVer	=	MAKEWORD(2, 2);
			if(0 == WSAStartup(Tv_WsVer, &Tv_WsData))
			{
				// create completion port
				m_HndlIocp	=	CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
				if(NULL != m_HndlIocp)
				{
					Tv_Result	=	TRUE;
				}
				else
				{
					DbgMsgPrint("Cls_GrSvrIocpJob::ImmInit - CreateIoCompletionPort port create fail\n");
					WSACleanup();
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrSvrIocpJob::ImmInit - WSAStartup fail\n");
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::ImmFinish(void)
{
	// local -------------------
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
		__u32	Tv_ConnIdx;
		__u32	Tv_HndlConn;
	// code --------------------

		if(NULL != m_HndlIocp)
		{
			// delete all
			Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)m_ConnTbl;
			for( Tv_ConnIdx = 0; Tv_ConnIdx < m_ConnMaxCnt; Tv_ConnIdx++)
			{
				// check exist
				if(INVALID_SOCKET != Tv_PtrConn->Sock)
				{
					Tv_HndlConn	=	ConnHndlEncode(Tv_ConnIdx, Tv_PtrConn->LocalId);
					ConnDelete(Tv_HndlConn);
				}

				// next
				Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)((Def_GrCalPtr)Tv_PtrConn + (Def_GrCalPtr)m_ConnUnitSize);
			}

			CloseHandle(m_HndlIocp);
			m_HndlIocp		=	NULL;

			WSACleanup();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::ReqInit(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrSvrIocpJobCmdInit	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.Hd.Cmd	=	m_MsgCmdJob;
		Tv_Msg.Hd.Cmd			=	E_GrSvrIocpJobCmdInit;
		Tv_Msg.PtrRslt		=	&Tv_Result;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::ReqFinish(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrSvrIocpJobCmdFinish	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.Hd.Cmd	=	m_MsgCmdJob;
		Tv_Msg.Hd.Cmd			=	E_GrSvrIocpJobCmdFinish;

		if(m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg)))
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::ReqConnAdd(SOCKET A_Sock, __u32 A_Ip, __u16 A_Port, BOOL8 A_IsPost)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrSvrIocpJobCmdConnAdd	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.Hd.Cmd	=	m_MsgCmdJob;
		Tv_Msg.Hd.Cmd			=	E_GrSvrIocpJobCmdConnAdd;
		Tv_Msg.Sock				=	A_Sock;
		Tv_Msg.Ip					=	A_Ip;
		Tv_Msg.Port				=	A_Port;
		Tv_Msg.PtrRslt		=	&Tv_Result;
		if(A_IsPost)
		{
			Tv_Msg.PtrRslt		=	NULL;
			Tv_Result	=	m_TskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		}
		else
		{
			m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrSvrIocpJob::ConnHndlEncode(__u32 A_ConnIdx, __u16 A_LocalId)
{
	// local -------------------
	// code --------------------
		return	((A_ConnIdx + 1) << E_GrSvrIocpJobHndlShftConnIdx) | (__u32)A_LocalId;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::ConnHndlDecode(__u32 A_HndlConn, __u32* A_PtrRtConnIdx, __u16* A_PtrRtLocalId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_ConnIdx;
		__u32	Tv_LocalId;
	// code --------------------
		Tv_Result		=	FALSE;

		Tv_LocalId	=	0;

		Tv_ConnIdx	=	((A_HndlConn >> E_GrSvrIocpJobHndlShftConnIdx) & E_GrSvrIocpJobHndlMaskConnIdx);
		if((0 != Tv_ConnIdx) && (m_ConnMaxCnt >= Tv_ConnIdx))
		{
			Tv_ConnIdx --;
			Tv_LocalId	=	A_HndlConn & 0xFFFF;
			Tv_Result		=	TRUE;
		}

		// update result
		*A_PtrRtConnIdx	=	Tv_ConnIdx;
		*A_PtrRtLocalId	=	(__u16)Tv_LocalId;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrSvrIocpJob::ConnPtrGet(__u32 A_HndlConn)
{
	// local -------------------
	Ptr_GrSvrIocpJobConn	Tv_Result;
		__u32	Tv_ConnIdx;
		__u16	Tv_LocalId;
	// code --------------------
		Tv_Result	=	NULL;

		if(ConnHndlDecode(A_HndlConn, &Tv_ConnIdx, &Tv_LocalId))
		{
			Tv_Result	=	(Ptr_GrSvrIocpJobConn)((Def_GrCalPtr)m_ConnTbl + (Def_GrCalPtr)(Tv_ConnIdx * m_ConnUnitSize));
			// check local id
			if(Tv_LocalId != Tv_Result->LocalId)
			{
				Tv_Result	=	NULL;
			}
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::LcProcSend(__u32 A_HndlConn, Ptr_GrSvrIocpJobConn A_PtrConn, __u32 A_Transed)
{
	// local -------------------
		int		Tv_RtCode;
		DWORD	Tv_Trnsed;
	// code --------------------
		// check running
		if(A_PtrConn->Send.IsDoing)
		{
			A_PtrConn->Send.Prgs			=	A_PtrConn->Send.Prgs + A_Transed;
			if(A_PtrConn->Send.Prgs >= A_PtrConn->Send.BufSize)
			{
				// finish
				A_PtrConn->Send.IsDoing		=	FALSE;
				EvtSended(A_HndlConn, A_PtrConn->Send.PtrBuf, A_PtrConn->Send.BufSize);
			}
			else
			{
				// check complete mode
				if(A_PtrConn->Send.IsNtfCpl)
				{
					// do next send
					A_PtrConn->Send.BufInfo.buf		=	(CHAR*)((Def_GrCalPtr)A_PtrConn->Send.PtrBuf + (Def_GrCalPtr)A_PtrConn->Send.Prgs);
					A_PtrConn->Send.BufInfo.len		=	(ULONG)(A_PtrConn->Send.BufSize - A_PtrConn->Send.Prgs);

					GrDumyZeroMem(&A_PtrConn->Send.Ovlp, sizeof(WSAOVERLAPPED));

					Tv_RtCode	=	WSASend(A_PtrConn->Sock, &A_PtrConn->Send.BufInfo, 1, &Tv_Trnsed, 0, &A_PtrConn->Send.Ovlp, NULL);
					if(0 != Tv_RtCode)
					{
						Tv_RtCode	=	WSAGetLastError();
						if(ERROR_IO_PENDING != Tv_RtCode)
						{
							DbgMsgPrint("Cls_GrSvrIocpJob::BgwJobProc - WSASend error(%d).\n", Tv_RtCode);
							// do close
							EvtClosed(A_HndlConn);
							ConnDelete(A_HndlConn);
						}
					}
				}
				else
				{
					// do event and complete
					A_PtrConn->Send.IsDoing		=	FALSE;
					EvtSended(A_HndlConn, A_PtrConn->Send.PtrBuf, A_PtrConn->Send.Prgs);
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::LcProcRecv(__u32 A_HndlConn, Ptr_GrSvrIocpJobConn A_PtrConn, __u32 A_Transed)
{
	// local -------------------
		int		Tv_RtCode;
		DWORD	Tv_Trnsed;
		DWORD	Tv_Flags;
	// code --------------------
		if(0 == A_Transed)
		{
			// closed
			EvtClosed(A_HndlConn);
			ConnDelete(A_HndlConn);
		}
		else
		{
			if(A_PtrConn->Recv.IsDoing)
			{
				A_PtrConn->Recv.Prgs			=	A_PtrConn->Recv.Prgs + A_Transed;
				if(A_PtrConn->Recv.Prgs >= A_PtrConn->Recv.BufSize)
				{
					// finish
					A_PtrConn->Recv.IsDoing		=	FALSE;
					EvtRecved(A_HndlConn, A_PtrConn->Recv.PtrBuf, A_PtrConn->Recv.BufSize);
				}
				else
				{
					// check complete mode
					if(A_PtrConn->Recv.IsNtfCpl)
					{
						// do next Recv
						A_PtrConn->Recv.BufInfo.buf		=	(CHAR*)((Def_GrCalPtr)A_PtrConn->Recv.PtrBuf + (Def_GrCalPtr)A_PtrConn->Recv.Prgs);
						A_PtrConn->Recv.BufInfo.len		=	(ULONG)(A_PtrConn->Recv.BufSize - A_PtrConn->Recv.Prgs);

						GrDumyZeroMem(&A_PtrConn->Recv.Ovlp, sizeof(WSAOVERLAPPED));

						Tv_Flags	=	0;
						Tv_RtCode	=	WSARecv(A_PtrConn->Sock, &A_PtrConn->Recv.BufInfo, 1, &Tv_Trnsed, &Tv_Flags, &A_PtrConn->Recv.Ovlp, NULL);
						if(0 != Tv_RtCode)
						{
							Tv_RtCode	=	WSAGetLastError();
							if(ERROR_IO_PENDING != Tv_RtCode)
							{
								DbgMsgPrint("Cls_GrSvrIocpJob::BgwJobProc - WSARecv error(%d).\n", Tv_RtCode);
								// do close
								EvtClosed(A_HndlConn);
								ConnDelete(A_HndlConn);
							}
						}
					}
					else
					{
						// do event and complete
						A_PtrConn->Recv.IsDoing		=	FALSE;
						EvtRecved(A_HndlConn, A_PtrConn->Recv.PtrBuf, A_PtrConn->Recv.Prgs);
					}
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::BgwJobProc(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
		ULONG_PTR	Tv_HndlConn;
		OVERLAPPED*	Tv_PtrOvlp;
		DWORD	Tv_Trns;
#ifndef GR_IOCP_XP
		ULONG	Tv_NumOut;
		ULONG	Tv_OutIdx;
#endif
	// code --------------------
		// init
		Tv_Result		=	FALSE;

		if(NULL != m_HndlIocp)
		{
#ifdef GR_IOCP_XP
			if(GetQueuedCompletionStatus(m_HndlIocp, &Tv_Trns, &Tv_HndlConn, &Tv_PtrOvlp, E_GrSvrIocpJobIocpWaitTime))
			{
				// get conn ptr
				Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)ConnPtrGet((__u32)Tv_HndlConn);
				if(NULL != Tv_PtrConn)
				{
					// check mode
					if(Tv_PtrOvlp == &Tv_PtrConn->Recv.Ovlp)
					{
						// receive mode
						LcProcRecv((__u32)Tv_HndlConn, Tv_PtrConn, (__u32)Tv_Trns);
					}
					else if(Tv_PtrOvlp == &Tv_PtrConn->Send.Ovlp)
					{
						// send mode
						LcProcSend((__u32)Tv_HndlConn, Tv_PtrConn, (__u32)Tv_Trns);
					}
					else
					{
						DbgMsgPrint("Cls_GrSvrIocpJob::BgwJobProc - bad overlapped pointer.\n");
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrSvrIocpJob::BgwJobProc - bad CONN Handle.\n");
				}
				Tv_Result	=	TRUE;
			}
#else
			Tv_NumOut	=	0;
			if(GetQueuedCompletionStatusEx(m_HndlIocp, m_OvlpEntTbl, E_GrSvrIocpJobOvlpEntCnt, &Tv_NumOut, E_GrSvrIocpJobIocpWaitTime, FALSE))
			{
				if(0 != Tv_NumOut)
				{
					for( Tv_OutIdx = 0; Tv_OutIdx < Tv_NumOut; Tv_OutIdx++)
					{
						// init
						Tv_HndlConn	=	m_OvlpEntTbl[Tv_OutIdx].lpCompletionKey;
						Tv_PtrOvlp	=	m_OvlpEntTbl[Tv_OutIdx].lpOverlapped;
						Tv_Trns			=	m_OvlpEntTbl[Tv_OutIdx].dwNumberOfBytesTransferred;
						// get conn ptr
						Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)ConnPtrGet((__u32)Tv_HndlConn);
						if(NULL != Tv_PtrConn)
						{
							// check mode
							if(Tv_PtrOvlp == &Tv_PtrConn->Recv.Ovlp)
							{
								// receive mode
								LcProcRecv((__u32)Tv_HndlConn, Tv_PtrConn, (__u32)Tv_Trns);
							}
							else if(Tv_PtrOvlp == &Tv_PtrConn->Send.Ovlp)
							{
								// send mode
								LcProcSend((__u32)Tv_HndlConn, Tv_PtrConn, (__u32)Tv_Trns);
							}
							else
							{
								DbgMsgPrint("Cls_GrSvrIocpJob::BgwJobProc - bad overlapped pointer.\n");
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrSvrIocpJob::BgwJobProc - bad CONN Handle.\n");
						}

					}
					Tv_Result	=	TRUE;
				}
			}
#endif
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::ImmConnAdd(SOCKET A_Sock, __u32 A_Ip, __u16 A_Port)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__s32	Tv_ConnIdx;
		__u32	Tv_HndlConn;
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
	// code --------------------
		Tv_Result	=	FALSE;

		// check initialized
		if(NULL != m_HndlIocp)
		{
			Tv_ConnIdx		=	-1;
			// check full
			if(m_ConnMaxCnt > m_ConnCnt)
			{
				// find empty slot
				Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)m_ConnTbl;
				for(Tv_WkIdx = 0; Tv_WkIdx < m_ConnMaxCnt; Tv_WkIdx++)
				{
					if(INVALID_SOCKET == Tv_PtrConn->Sock)
					{
						// found
						Tv_ConnIdx	=	(__s32)Tv_WkIdx;
						break;
					}
					// next
					Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)((Def_GrCalPtr)Tv_PtrConn + (Def_GrCalPtr)m_ConnUnitSize);
				}
			}

			// check found
			if(0 <= Tv_ConnIdx)
			{
				Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)((Def_GrCalPtr)m_ConnTbl + (Def_GrCalPtr)(m_ConnUnitSize * (__u32)Tv_ConnIdx));
				Tv_HndlConn	=	ConnHndlEncode(Tv_ConnIdx, Tv_PtrConn->LocalId);
				if(m_HndlIocp != CreateIoCompletionPort((HANDLE)A_Sock, m_HndlIocp, (ULONG)Tv_HndlConn, 0))
				{
					DbgMsgPrint("Cls_GrSvrIocpJob::ImmConnAdd - CreateIoCompletionPort fail\n");
				}
				else
				{
					// init
					Tv_PtrConn->Sock			=	A_Sock;
					Tv_PtrConn->Ip				=	A_Ip;
					Tv_PtrConn->Port			=	A_Port;

					Tv_PtrConn->Send.IsDoing		=	FALSE;
					Tv_PtrConn->Recv.IsDoing		=	FALSE;

					Tv_PtrConn->ToutLim		=	0;
					Tv_PtrConn->ToutPrgs		=	0;

					// do event
					if(EvtConnAdd(Tv_HndlConn))
					{
						Tv_Result	=	TRUE;
					}
					else
					{
						ConnDelete(Tv_HndlConn);
					}
		
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrSvrIocpJob::ImmJobAdd - job is full\n");
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::ConnDelete(__u32 A_HndlConn)
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
	// code --------------------
		// init
		Tv_Result		=	FALSE;

		Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)ConnPtrGet(A_HndlConn);
		if(NULL != Tv_PtrConn)
		{
			// close socket
			closesocket(Tv_PtrConn->Sock);
			Tv_PtrConn->Sock		=	INVALID_SOCKET;

			Tv_PtrConn->LocalId ++;

			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::IsConnSending(__u32 A_HndlConn)
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
	// code --------------------
		Tv_Result		=	FALSE;

		// check already send
		Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)ConnPtrGet(A_HndlConn);
		if(NULL != Tv_PtrConn)
		{
			if(INVALID_SOCKET != Tv_PtrConn->Sock)
			{
				Tv_Result	=	Tv_PtrConn->Send.IsDoing;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::ConnSend(__u32 A_HndlConn, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsCplNotify)
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
		int		Tv_RtCode;
		DWORD	Tv_Transed;
	// code --------------------
		Tv_Result	=	FALSE;

		// check already send
		Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)ConnPtrGet(A_HndlConn);
		if(NULL != Tv_PtrConn)
		{
			if((INVALID_SOCKET != Tv_PtrConn->Sock) && (!Tv_PtrConn->Send.IsDoing))
			{
				// try send
				Tv_PtrConn->Send.PtrBuf		=	A_PtrBuf;
				Tv_PtrConn->Send.BufSize		=	A_Size;
				Tv_PtrConn->Send.Prgs			=	0;
				Tv_PtrConn->Send.IsNtfCpl	=	A_IsCplNotify;

				Tv_PtrConn->Send.BufInfo.buf		=	(CHAR*)Tv_PtrConn->Send.PtrBuf;
				Tv_PtrConn->Send.BufInfo.len		=	(ULONG)Tv_PtrConn->Send.BufSize;

				GrDumyZeroMem(&Tv_PtrConn->Send.Ovlp, sizeof(WSAOVERLAPPED));

				Tv_RtCode	=	WSASend(Tv_PtrConn->Sock, &Tv_PtrConn->Send.BufInfo, 1, &Tv_Transed, 0, &Tv_PtrConn->Send.Ovlp, NULL);

				Tv_Result	=	TRUE;
				if(0 == Tv_RtCode)
				{
					Tv_PtrConn->Send.IsDoing		=	TRUE;
				}
				else
				{
					Tv_RtCode	=	WSAGetLastError();
					if(ERROR_IO_PENDING == Tv_RtCode)
					{
						Tv_PtrConn->Send.IsDoing		=	TRUE;
					}
					else
					{
						Tv_Result	=	FALSE;
						DbgMsgPrint("Cls_GrSvrIocpJob::ConnSend - WSASend error(%d).\n",Tv_RtCode);
					}
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrSvrIocpJob::ConnSend - bad SOCKET or Already doing.\n");
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrSvrIocpJob::ConnSend - bad CONN handle\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::ConnRecv(__u32 A_HndlConn, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsCplNotify)
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
		int		Tv_RtCode;
		DWORD	Tv_Transed;
		DWORD	Tv_Flags;
	// code --------------------
		Tv_Result	=	FALSE;

		// check already send
		Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)ConnPtrGet(A_HndlConn);
		if(NULL != Tv_PtrConn)
		{
			if((INVALID_SOCKET != Tv_PtrConn->Sock) && (!Tv_PtrConn->Recv.IsDoing))
			{
				// try send
				Tv_PtrConn->Recv.PtrBuf		=	A_PtrBuf;
				Tv_PtrConn->Recv.BufSize		=	A_Size;
				Tv_PtrConn->Recv.Prgs			=	0;
				Tv_PtrConn->Recv.IsNtfCpl	=	A_IsCplNotify;

				Tv_PtrConn->Recv.BufInfo.buf		=	(CHAR*)Tv_PtrConn->Recv.PtrBuf;
				Tv_PtrConn->Recv.BufInfo.len		=	(ULONG)Tv_PtrConn->Recv.BufSize;

				GrDumyZeroMem(&Tv_PtrConn->Recv.Ovlp, sizeof(WSAOVERLAPPED));

				Tv_Flags	=	0;
				Tv_RtCode	=	WSARecv(Tv_PtrConn->Sock, &Tv_PtrConn->Recv.BufInfo, 1, &Tv_Transed, &Tv_Flags, &Tv_PtrConn->Recv.Ovlp, NULL);

				Tv_Result	=	TRUE;
				if(0 == Tv_RtCode)
				{
					Tv_PtrConn->Recv.IsDoing		=	TRUE;
				}
				else
				{
					Tv_RtCode	=	WSAGetLastError();
					if(ERROR_IO_PENDING == Tv_RtCode)
					{
						Tv_PtrConn->Recv.IsDoing		=	TRUE;
					}
					else
					{
						Tv_Result	=	FALSE;
						DbgMsgPrint("Cls_GrSvrIocpJob::ConnRecv - WSARecv error(%d).\n", Tv_RtCode);
					}
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrSvrIocpJob::ConnRecv - bad SOCKET or Already doing.\n");
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrSvrIocpJob::ConnRecv - bad CONN handle\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrSvrIocpJob::ImmCapRemainGet(void)
{
	// local -------------------
	// code --------------------
		return	(__s32)m_ConnMaxCnt - (__s32)m_ConnCnt;
}
//--------------------------------------------------------------------
__s32	Cls_GrSvrIocpJob::ReqCapRemainGet(void)
{
	// local -------------------
		__s32	Tv_Result;
		St_GrSvrIocpJobCmdCapRemainGet	Tv_Msg;
	// code --------------------
		Tv_Result					=	0;

		Tv_Msg.Hd.Hd.Cmd	=	m_MsgCmdJob;
		Tv_Msg.Hd.Cmd			=	E_GrSvrIocpJobCmdCapRemainGet;
		Tv_Msg.PtrRslt		=	&Tv_Result;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::EvtConnAdd(__u32 A_HndlConn)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::EvtSended(__u32 A_HndlConn, void* A_PtrBuf, __u32 A_Size)
{

}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::EvtRecved(__u32 A_HndlConn, void* A_PtrBuf, __u32 A_Size)
{

}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::EvtClosed(__u32 A_HndlConn)
{

}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::EvtTimer(__u32 A_HndlConn)
{

}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::EvtTimeOut(__u32 A_HndlConn)
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::ReqConnClose(__u32 A_HndlConn, BOOL8 A_IsPost)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrSvrIocpJobCmdConnClose	Tv_Msg;
	// code --------------------
		Tv_Result					=	FALSE;

		Tv_Msg.Hd.Hd.Cmd	=	m_MsgCmdJob;
		Tv_Msg.Hd.Cmd			=	E_GrSvrIocpJobCmdConnClose;
		Tv_Msg.PtrRslt		=	&Tv_Result;
		Tv_Msg.HndlConn		=	A_HndlConn;

		if(A_IsPost)
		{
			Tv_Msg.PtrRslt		=	NULL;
			Tv_Result	=	m_TskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		}
		else
		{
			m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpJob::ConnTimeOutSet(__u32 A_HndlConn, __u32 A_ToutSec)
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)ConnPtrGet(A_HndlConn);
		if(NULL != Tv_PtrConn)
		{
			Tv_PtrConn->ToutPrgs		=	0;
			Tv_PtrConn->ToutLim		=	A_ToutSec;

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::ConnTimeOutSingal(__u32 A_HndlConn)
{
	// local -------------------
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
	// code --------------------
		Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)ConnPtrGet(A_HndlConn);
		if(NULL != Tv_PtrConn)
		{
			Tv_PtrConn->ToutPrgs		=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::ConnTimeOutKill(__u32 A_HndlConn)
{
	// local -------------------
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
	// code --------------------
		Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)ConnPtrGet(A_HndlConn);
		if(NULL != Tv_PtrConn)
		{
			Tv_PtrConn->ToutLim		=	0;
			Tv_PtrConn->ToutPrgs		=	0;
		}

}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::LcUpdtTimeOut(void)
{
	// local -------------------
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
		__u32	Tv_ConnIdx;
		__u32	Tv_HndlConn;
	// code --------------------
		if((NULL != m_ConnTbl) && (0 != m_ConnMaxCnt))
		{
			Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)m_ConnTbl;

			for( Tv_ConnIdx = 0; Tv_ConnIdx < m_ConnMaxCnt; Tv_ConnIdx++)
			{
				// check able connection
				if(INVALID_SOCKET != Tv_PtrConn->Sock)
				{
					// check time out use
					if(0 != Tv_PtrConn->ToutLim)
					{
						Tv_PtrConn->ToutPrgs ++;
						if(Tv_PtrConn->ToutLim <= Tv_PtrConn->ToutPrgs)
						{
							// reset timer
							Tv_PtrConn->ToutLim		=	0;
							Tv_PtrConn->ToutPrgs		=	0;
							// do event
							Tv_HndlConn	=	ConnHndlEncode(Tv_ConnIdx, Tv_PtrConn->LocalId);
							EvtTimeOut(Tv_HndlConn);
						}
					}
				}

				// next
				Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)((Def_GrCalPtr)Tv_PtrConn + (Def_GrCalPtr)m_ConnUnitSize);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpJob::LcUpdtTimer(void)
{
	// local -------------------
		Ptr_GrSvrIocpJobConn	Tv_PtrConn;
		__u32	Tv_ConnIdx;
		__u32	Tv_HndlConn;
	// code --------------------
		if((NULL != m_ConnTbl) && (0 != m_ConnMaxCnt))
		{
			Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)m_ConnTbl;

			for(Tv_ConnIdx = 0; Tv_ConnIdx < m_ConnMaxCnt; Tv_ConnIdx++)
			{
				// check able connection
				if(INVALID_SOCKET != Tv_PtrConn->Sock)
				{
					// do event
					Tv_HndlConn	=	ConnHndlEncode(Tv_ConnIdx, Tv_PtrConn->LocalId);
					EvtTimer(Tv_HndlConn);
				}

				// next
				Tv_PtrConn	=	(Ptr_GrSvrIocpJobConn)((Def_GrCalPtr)Tv_PtrConn + (Def_GrCalPtr)m_ConnUnitSize);
			}
		}
}
//--------------------------------------------------------------------

