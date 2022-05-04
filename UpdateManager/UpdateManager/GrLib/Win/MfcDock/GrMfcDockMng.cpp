

#include <Win\MfcDock\GrMfcDockMng.h>

#include <Win\MfcDock\GrMfcDockMainFrm.h>
#include <Win\MfcDock\GrMfcDockMainView.h>
#include <Win\MfcDock\GrMfcDockPaneBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>


//--------------------------------------------------------------------
//const


//--------------------------------------------------------------------
//global var

extern	St_GrMfcDockEnv	V_GrMfcDockEnv;

//--------------------------------------------------------------------
//--------------------------------------------------------------------
Cls_GrMfcDockMng::Cls_GrMfcDockMng( Cls_GrTaskSvr* A_TaskSvr ):
	Cls_GrTaskCli()
{
		m_TaskSvr	=	A_TaskSvr;
		m_MsgAloc				=	GrTaskSvrMsgCmdAlloc();

		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistBkgClient( this );
}
//--------------------------------------------------------------------
Cls_GrMfcDockMng::~Cls_GrMfcDockMng()
{
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistMsgClient( this );

		GrTaskSvrMsgCmdFree( m_MsgAloc );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockMng::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		MSG		Tv_Msg;
		WCHAR*	Tv_StrMsg;
	// code --------------------
		Tv_Result	=	FALSE;
		// message basic process
		if ( PeekMessage( &Tv_Msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if ( !AfxGetApp()->PumpMessage() )
			{
				PostQuitMessage( 0 );
				m_TaskSvr->ReqQuit();
			}
			Tv_Result	=	TRUE;
		}
		// debug message
		if(NULL != V_GrMfcDockEnv.MainFrm)
		{
			Tv_StrMsg	=	(WCHAR*)GrBufVqGetOutPtr((struct St_GrBufVarQue*)V_GrMfcDockEnv.DmsgQue);
			if(NULL != Tv_StrMsg)
			{
				((Cls_GrMfcDockMainFrm*)V_GrMfcDockEnv.MainFrm)->RtlDbgMsgAdd(Tv_StrMsg);

				GrBufVqOut((struct St_GrBufVarQue*)V_GrMfcDockEnv.DmsgQue);
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockMng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			switch ( ((Ptr_GrMfcDockMngMsgBase)A_PtrMsg)->Cmd )
			{
				case E_GrMfcDockMngMsgCmdReady:
					V_GrMfcDockEnv.Stat	=	E_GrMfcDockPaneStatReady;
					break;
				case E_GrMfcDockMngMsgCmdPaneMsg:
					LcMprcPaneMsg((Ptr_GrMfcDockMngMsgPaneMsg)A_PtrMsg);
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockMng::LcMprcPaneMsg(Ptr_GrMfcDockMngMsgPaneMsg A_PtrMsg)
{
	// local -------------------
		__u32	Tv_PneIdx;
		__u32	Tv_PaneId;
		BOOL8	Tv_IsAble;
	// code --------------------
		Tv_IsAble	=	TRUE;

		// check main frame
		if(NULL != V_GrMfcDockEnv.MainFrm)
		{
			Tv_PaneId	=	((Cls_GrMfcDockMainFrm*)V_GrMfcDockEnv.MainFrm)->PaneIdGet();
			if((Tv_PaneId == A_PtrMsg->TargetId) ||
				((Tv_PaneId != A_PtrMsg->OwnerId) && (E_GrMfcDockPaneIdBroadcast == A_PtrMsg->TargetId)))
			{
				((Cls_GrMfcDockMainFrm*)V_GrMfcDockEnv.MainFrm)->RtlPaneMsgProc(A_PtrMsg->OwnerId, A_PtrMsg->TargetId,
					A_PtrMsg->Cmd, A_PtrMsg->Para0, A_PtrMsg->Para1, A_PtrMsg->Para2);
				if(E_GrMfcDockPaneIdBroadcast != A_PtrMsg->TargetId)
				{
					Tv_IsAble	=	FALSE;
				}
			}
		}

		// check main view
		if(Tv_IsAble && (NULL != V_GrMfcDockEnv.MainView))
		{
			Tv_PaneId	=	((Cls_GrMfcDockMainView*)V_GrMfcDockEnv.MainView)->PaneIdGet();
			if((Tv_PaneId == A_PtrMsg->TargetId) ||
				((Tv_PaneId != A_PtrMsg->OwnerId) && (E_GrMfcDockPaneIdBroadcast == A_PtrMsg->TargetId)))
			{
				((Cls_GrMfcDockMainView*)V_GrMfcDockEnv.MainView)->RtlPaneMsgProc(A_PtrMsg->OwnerId, A_PtrMsg->TargetId,
					A_PtrMsg->Cmd, A_PtrMsg->Para0, A_PtrMsg->Para1, A_PtrMsg->Para2);
				if(E_GrMfcDockPaneIdBroadcast != A_PtrMsg->TargetId)
				{
					Tv_IsAble	=	FALSE;
				}
			}
		}

		// pane broad cast
		if(Tv_IsAble)
		{
			for(Tv_PneIdx = 0; Tv_PneIdx < E_GrMfcDockPaneMaxCnt; Tv_PneIdx++)
			{
				if(NULL != V_GrMfcDockEnv.PaneTbl[Tv_PneIdx])
				{
					// init
					Tv_PaneId	=	((Cls_GrMfcDockPaneBase*)V_GrMfcDockEnv.PaneTbl[Tv_PneIdx])->PaneIdGet();
					Tv_IsAble	=	TRUE;

					if(A_PtrMsg->OwnerId != A_PtrMsg->TargetId)
					{
						if(A_PtrMsg->OwnerId == Tv_PaneId)
						{
							Tv_IsAble	=	FALSE;
						}
						else if(E_GrMfcDockPaneIdBroadcast != A_PtrMsg->TargetId)
						{
							if(Tv_PaneId != A_PtrMsg->TargetId)
							{
								Tv_IsAble	=	FALSE;
							}
						}
					}
					else
					{
						if(Tv_PaneId != A_PtrMsg->TargetId)
						{
							Tv_IsAble	=	FALSE;
						}
					}

					if(Tv_IsAble)
					{
						((Cls_GrMfcDockPaneBase*)V_GrMfcDockEnv.PaneTbl[Tv_PneIdx])->RtlPaneMsgProc(A_PtrMsg->OwnerId, A_PtrMsg->TargetId,
							A_PtrMsg->Cmd, A_PtrMsg->Para0, A_PtrMsg->Para1, A_PtrMsg->Para2);
					}

				}
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockMng::PaneMsgPost(__u32 A_OwnerId, __u32 A_TargetId, __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2)
{
	// local -------------------
		St_GrMfcDockMngMsgPaneMsg	Tv_Msg;
	// code --------------------

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrMfcDockMngMsgCmdPaneMsg;
		Tv_Msg.OwnerId			=	A_OwnerId;
		Tv_Msg.TargetId			=	A_TargetId;
		Tv_Msg.Cmd					=	A_Cmd;
		Tv_Msg.Para0				=	A_Para0;
		Tv_Msg.Para1				=	A_Para1;
		Tv_Msg.Para2				=	A_Para2;

		return	m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockMng::DoReady(void)
{
	// local -------------------
		St_GrMfcDockMngMsgReady	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrMfcDockMngMsgCmdReady;

		m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
