/*
product regist server

*/

//====================================================================
//include

#include <Win/Net/GrSvrIocpMng.h>
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
Cls_GrSvrIocpMng::Cls_GrSvrIocpMng(Cls_GrTaskSvr* A_TaskSvr) :
Cls_GrTaskCli()
{
	// local -------------------
		//__u32		Tv_WkIdx;
	// code --------------------
		// init
		m_TskSvr			=	A_TaskSvr;
		m_MsgCmdMng		=	GrTaskSvrMsgCmdAlloc();

		// reset
		m_PortListen	=	0;
		m_SockListen	=	INVALID_SOCKET;
		m_EvtListen		=	WSA_INVALID_EVENT;

		m_JobCnt			=	0;

		GrDumyZeroMem(m_JobTbl, sizeof(m_JobTbl));

		// regist sock
		m_TskSvr->RegistMsgClient(this);
		//m_TskSvr->RegistTimerClient(this, E_GrSvrIocpMngTimerPeriod, &m_TmrSec);
		m_TskSvr->RegistBkgClient(this);


}
//--------------------------------------------------------------------
Cls_GrSvrIocpMng::~Cls_GrSvrIocpMng()
{
	// local -------------------
	// code --------------------

		ReqFinish();

		m_TskSvr->UnregistBkgClient(this);
		//m_TskSvr->UnregistTimerClient(m_TmrSec);
		m_TskSvr->UnregistMsgClient(this);

		GrTaskSvrMsgCmdFree(m_MsgCmdMng);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpMng::RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(m_MsgCmdMng == A_PtrMsg->Cmd)
		{
			Tv_Result	=	TRUE;
			switch(((Ptr_GrSvrIocpMngCmdBase)A_PtrMsg)->Cmd)
			{
				case E_GrSvrIocpMngCmdInit:
					*(((Ptr_GrSvrIocpMngCmdInit)A_PtrMsg)->PtrRslt)		=
						ImmInit(((Ptr_GrSvrIocpMngCmdInit)A_PtrMsg)->Port, ((Ptr_GrSvrIocpMngCmdInit)A_PtrMsg)->JobCnt,
						((Ptr_GrSvrIocpMngCmdInit)A_PtrMsg)->TaskIdBase);
					break;
				case E_GrSvrIocpMngCmdFinish:
					ImmFinish();
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpMng::RtlBkgProc(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(BgwListen())
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpMng::ImmInit(__u16 A_Port, __u32 A_JobCnt, __u32 A_TaskIdBase)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		WORD	Tv_WsVer;
		WSADATA	Tv_WsData;
		sockaddr_in	Tv_NetAdr;

	// code --------------------
		Tv_Result		=	FALSE;

		// check already initialized
		if(INVALID_SOCKET == m_SockListen)
		{
			Tv_WsVer	=	MAKEWORD(2, 2);
			if(0 == WSAStartup(Tv_WsVer, &Tv_WsData))
			{
				// listen socket
				m_SockListen	=	WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
				if(INVALID_SOCKET != m_SockListen)
				{
					m_EvtListen		=	WSACreateEvent();
					if(WSA_INVALID_EVENT != m_EvtListen)
					{
						Tv_NetAdr.sin_family			=	AF_INET;
						Tv_NetAdr.sin_addr.s_addr	=	htonl(INADDR_ANY);
						Tv_NetAdr.sin_port				=	htons(A_Port);
						if(0 <= bind(m_SockListen, (sockaddr*)&Tv_NetAdr, sizeof(Tv_NetAdr)))
						{
							if(0 == WSAEventSelect(m_SockListen, m_EvtListen, FD_ACCEPT))
							{
								if(0 <= listen(m_SockListen, E_GrSvrIocpMngListenQueue))
								{
									if((0 != A_JobCnt) && (E_GrSvrIocpMngJobMaxCnt >= A_JobCnt))
									{

										for( Tv_WkIdx = 0; Tv_WkIdx < A_JobCnt; Tv_WkIdx++)
										{
											m_JobTbl[m_JobCnt].TskSvr	=	(Cls_GrTaskSvr*)new Cls_GrTaskSvr(A_TaskIdBase + m_JobCnt, TRUE);
											if(NULL != m_JobTbl[m_JobCnt].TskSvr)
											{
												m_JobTbl[m_JobCnt].ObjJob	=	EvtJobAdd(m_JobTbl[m_JobCnt].TskSvr);
												if(NULL != m_JobTbl[m_JobCnt].ObjJob)
												{
													if(((Cls_GrSvrIocpJob*)m_JobTbl[m_JobCnt].ObjJob)->ReqInit())
													{
														m_JobCnt ++;
													}
													else
													{
														delete	m_JobTbl[m_JobCnt].TskSvr;
														m_JobTbl[m_JobCnt].TskSvr	=	NULL;
													}
												}
												else
												{
													delete	m_JobTbl[m_JobCnt].TskSvr;
													m_JobTbl[m_JobCnt].TskSvr	=	NULL;
												}
											}
										}

										if(0 != m_JobCnt)
										{
											Tv_Result		=	TRUE;
										}
									}
									else
									{
										DbgMsgPrint("Cls_GrSvrIocpMng::ImmInit - bad job count(%d).\n", A_JobCnt);
									}
								}
								else
								{
									DbgMsgPrint("Cls_GrSvrIocpMng::ImmInit - listen fail!\n");
								}
							}
							else
							{
								DbgMsgPrint("Cls_GrSvrIocpMng::WSAEventSelect - bind fail!\n");
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrSvrIocpMng::ImmInit - bind fail!\n");
						}

						if(!Tv_Result)
						{
							WSACloseEvent(m_EvtListen);
							m_EvtListen	=	WSA_INVALID_EVENT;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrSvrIocpMng::ImmInit - WSACreateEvent fail!\n");
					}
					if(!Tv_Result)
					{
						closesocket(m_SockListen);
						m_SockListen	=	INVALID_SOCKET;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrSvrIocpMng::ImmInit - WSASocket fail!\n");
				}

				if(!Tv_Result)
				{
					WSACleanup();
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrSvrIocpMng::ImmInit - WSAStartup fail!\n");
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrSvrIocpMng::ImmFinish(void)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		// release job
		if(0 != m_JobCnt)
		{
			for(Tv_WkIdx = 0; Tv_WkIdx < m_JobCnt; Tv_WkIdx++)
			{
				delete	m_JobTbl[Tv_WkIdx].ObjJob;
				m_JobTbl[Tv_WkIdx].ObjJob	=	NULL;

				delete	m_JobTbl[Tv_WkIdx].TskSvr;
				m_JobTbl[Tv_WkIdx].TskSvr	=	NULL;
			}
			m_JobCnt		=	0;
		}

		// release socket
		if(INVALID_SOCKET != m_SockListen)
		{
			closesocket(m_SockListen);
			m_SockListen	=	INVALID_SOCKET;
		}

		// release event
		if(WSA_INVALID_EVENT != m_EvtListen)
		{
			WSACloseEvent(m_EvtListen);
			m_EvtListen	=	WSA_INVALID_EVENT;
		}

		WSACleanup();

}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpMng::ReqInit(__u16 A_Port, __u32 A_JobCnt, __u32 A_TaskIdBase)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrSvrIocpMngCmdInit	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.Hd.Cmd	=	m_MsgCmdMng;
		Tv_Msg.Hd.Cmd			=	E_GrSvrIocpMngCmdInit;
		Tv_Msg.PtrRslt		=	&Tv_Result;
		Tv_Msg.Port				=	A_Port;
		Tv_Msg.JobCnt			=	A_JobCnt;
		Tv_Msg.TaskIdBase	=	A_TaskIdBase;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpMng::ReqFinish(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrSvrIocpMngCmdFinish	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.Hd.Cmd	=	m_MsgCmdMng;
		Tv_Msg.Hd.Cmd			=	E_GrSvrIocpMngCmdFinish;

		if(m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg)))
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrTaskCli*	Cls_GrSvrIocpMng::JobCapableGet(void)
{
	// local -------------------
		Cls_GrTaskCli*	Tv_Result;
		__s32	Tv_JobFnd;
		__s32	Tv_CapFnd;
		__u32	Tv_JobIdx;
		__s32	Tv_CapNow;
	// code --------------------
		Tv_Result	=	NULL;

		if(0 != m_JobCnt)
		{
			Tv_JobFnd		=	-1;
			Tv_CapFnd		=	0;
			for(Tv_JobIdx = 0; Tv_JobIdx < m_JobCnt; Tv_JobIdx++)
			{
				Tv_CapNow	=	((Cls_GrSvrIocpJob*)m_JobTbl[Tv_JobIdx].ObjJob)->ImmCapRemainGet();
				if(0 < Tv_CapNow)
				{
					if(0 > Tv_JobFnd)
					{
						Tv_JobFnd	=	(__s32)Tv_JobIdx;
						Tv_CapFnd	=	Tv_CapNow;
					}
					else if(Tv_CapFnd < Tv_CapNow)
					{
						Tv_JobFnd	=	(__s32)Tv_JobIdx;
						Tv_CapFnd	=	Tv_CapNow;
					}
				}
			}

			if(0 <= Tv_JobFnd)
			{
				Tv_Result	=	m_JobTbl[Tv_JobFnd].ObjJob;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpMng::BgwListen(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsAdded;
		DWORD	Tv_EvtVal;
		sockaddr_in	Tv_AddrIn;
		int		Tv_AddrSize;
		SOCKET	Tv_SockAcpt;
		__u32	Tv_AcptIp;
		__u16	Tv_AcptPort;
		Cls_GrSvrIocpJob*	Tv_ObjJob;
		int		Tv_ErrCode;
	// code --------------------

		Tv_Result		=	FALSE;

		// check able
		if((INVALID_SOCKET != m_SockListen) && (WSA_INVALID_EVENT != m_EvtListen))
		{
			Tv_EvtVal	=	WSAWaitForMultipleEvents(1, &m_EvtListen, FALSE, E_GrSvrIocpMngWaitEvtTime,FALSE);
			if(WSA_WAIT_EVENT_0 == Tv_EvtVal)
			{
				// reset event
				WSAResetEvent(m_EvtListen);
				// do accept
				GrDumyZeroMem(&Tv_AddrIn, sizeof(Tv_AddrIn));
				Tv_AddrSize	=	sizeof(Tv_AddrIn);

				Tv_SockAcpt	=	WSAAccept(m_SockListen, (sockaddr*)&Tv_AddrIn, &Tv_AddrSize, NULL, NULL);
				if(INVALID_SOCKET != Tv_SockAcpt)
				{
					// get socket info
					Tv_AcptIp		=	Tv_AddrIn.sin_addr.s_addr;
					Tv_AcptPort	=	(__u16)htons(Tv_AddrIn.sin_port);
					// find good capability job
					Tv_IsAdded	=	FALSE;
					Tv_ObjJob	=	(Cls_GrSvrIocpJob*)JobCapableGet();
					if(NULL != Tv_ObjJob)
					{
						if(EvtConnAdd(Tv_SockAcpt,Tv_AcptIp,Tv_AcptPort))
						{
							Tv_IsAdded	=	Tv_ObjJob->ReqConnAdd(Tv_SockAcpt, Tv_AcptIp, Tv_AcptPort);
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrSvrIocpMng::BgwListen - Job is full\n");
					}

					if(!Tv_IsAdded)
					{
						// close socket
						closesocket(Tv_SockAcpt);
					}

					// thread do
					Tv_Result	=	TRUE;
				}
				else
				{
					Tv_ErrCode	=	WSAGetLastError();
					DbgMsgPrint("ACP ERR %d ip = 0x%x\n",Tv_ErrCode, Tv_AddrIn.sin_addr.s_addr);
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrTaskCli*	Cls_GrSvrIocpMng::EvtJobAdd(Cls_GrTaskSvr* A_TaskSvr)
{
		return	NULL;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrSvrIocpMng::EvtConnAdd(SOCKET A_Sock, __u32 A_Ip, __u16 A_Port)
{
		return	TRUE;
}
//--------------------------------------------------------------------
