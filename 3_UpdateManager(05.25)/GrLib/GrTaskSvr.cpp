/*
	task server

*/

//====================================================================
//include
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <GrTaskSvr.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrProcess.h>

#ifdef LINUX_APP
	#undef _XOPEN_SOURCE
	#define _XOPEN_SOURCE	500
	#include <signal.h>
	#include <stdlib.h>
	#include <stdio.h>
#endif

//====================================================================
// local const


//====================================================================
// local type

//====================================================================
// variable

extern	St_GrTaskEnv	V_GrTaskEnv;

//====================================================================
//functions
Def_GrPrcThrdRslt	Def_GrPrcThrdClcb	GrTaskSvrThrdProc( Def_GrPrcThrdPara A_Para );

//--------------------------------------------------------------------
Cls_GrTaskSvr::Cls_GrTaskSvr( __u32 A_Fcc, BOOL8 A_IsNewThrd, __u32 A_MsgBufSize )
{
	// local -------------------
	// code --------------------
		// check debug mode
#ifdef E_GrTaskSvrDbgApi
#ifdef LINUX_APP
		printf("****************************************\n");
		printf("*** option E_GrTaskSvrDbgApi is on!! ***\n");
		printf("****************************************\n");
#endif
#endif

		//init
		m_IsAble			=	FALSE;
		m_IsThrdRun		=	FALSE;
		m_IsCliRun		=	TRUE;
		m_IsReqQuit		=	FALSE;
		m_IsNewThrd		=	A_IsNewThrd;
		m_PtrMsgBuf		=	NULL;
		m_MsgCliAreaCnt	=	0;
		GrDumyZeroMem( m_MsgCliTbl, sizeof(m_MsgCliTbl) );
		m_BkgCliAreaCnt	=	0;
		GrDumyZeroMem( m_BkgCliTbl, sizeof(m_BkgCliTbl) );
		m_TimerAreaCnt	=	0;
		GrDumyZeroMem( m_TimerTbl, sizeof(m_TimerTbl) );

		m_MsgPumpCnt	=	E_GrTaskSvrDfltMsgPump;

		//make critical section
		GrPrcCritInit( &m_Crit );

		//create message queue
		m_PtrMsgBuf	=	(Ptr_GrBufVarQue)malloc( A_MsgBufSize );
		if ( NULL != m_PtrMsgBuf )
		{
			//register
			m_SvrId	=	GrTaskSvrReg(this);

			if(0 <= m_SvrId)
			{
				V_GrTaskEnv.SvrRegTbl[m_SvrId].Fcc	=	A_Fcc;

				GrBufVqInit(m_PtrMsgBuf, A_MsgBufSize, 4);
				PrioritySet(E_GrTaskSvrPrioGap1);			//default priority
				//check create thread
				if(m_IsNewThrd)
				{
					GrPrcThrdCreate(&m_ThrdMng, GrTaskSvrThrdProc, (Def_GrPrcThrdPara)this);
				}
				else
				{
					m_IsAble			=	TRUE;
					m_IsThrdRun		=	TRUE;

					V_GrTaskEnv.SvrRegTbl[m_SvrId].ThrdId	=	GrPrcThrdGetCurId();
				}

			}
		}
		
}
//--------------------------------------------------------------------
Cls_GrTaskSvr::~Cls_GrTaskSvr()
{
		//unregister
		GrTaskSvrUnreg( this );
		//stop thread
		if ( m_IsNewThrd && m_IsAble )
		{
			ReqQuit();

			while ( m_IsThrdRun )
			{
				GrPrcSleep( E_GrTaskSvrPrioQuatZero );
			}
			GrPrcThrdRelease( &m_ThrdMng );
		}
		
		//free message queue
		if ( NULL	!= m_PtrMsgBuf )
		{
			free( m_PtrMsgBuf );
		}

		//releas critical
		GrPrcCritDel( &m_Crit );
		
}
//--------------------------------------------------------------------
Def_GrPrcThrdRslt	Def_GrPrcThrdClcb	GrTaskSvrThrdProc( Def_GrPrcThrdPara A_Para )
{
	// local -------------------
		Cls_GrTaskSvr*	Tv_ObjSvr;
	// code --------------------
		Tv_ObjSvr		=	(Cls_GrTaskSvr*)A_Para;
		Tv_ObjSvr->RtlRun();

		return	0;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::DoStep( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		// check registered
		if(0 > m_SvrId)
		{
			return	FALSE;
		}

		//init
		Tv_Result		=	FALSE;
		//message
		if(RtlMsgPump())
		{
			Tv_Result	=	TRUE;
		}

		//background
		if ( LcProcBkg() )
		{
			Tv_Result		=	TRUE;
		}

		//timer
		if ( LcProcTimer() )
		{
			Tv_Result		=	TRUE;
		}
		
		// update step
		V_GrTaskEnv.SvrRegTbl[m_SvrId].PrgsCnt ++;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::RtlMsgPump(void)
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
		Ptr_GrTaskMsgHd	Tv_PtrMsg;
		__u8		Tv_MsgBuf[E_GrTaskSvrMsgMaxSize];
	// code --------------------
		Tv_Result	=	FALSE;

		for(Tv_WkIdx = 0; Tv_WkIdx < m_MsgPumpCnt; Tv_WkIdx++)
		{
			Tv_PtrMsg	=	(Ptr_GrTaskMsgHd)GrBufVqGetOutPtr(m_PtrMsgBuf);
			if(NULL == Tv_PtrMsg)
			{
				break;
			}
			if(E_GrTaskSvrMsgMaxSize < Tv_PtrMsg->Size)
			{
				DbgMsgPrint("Cls_GrTaskSvr::RtlMsgPump: message size over! - %d\n", Tv_PtrMsg->Size);
				GrBufVqOut(m_PtrMsgBuf);		//release message
				continue;
			}
			GrDumyCopyMem(Tv_MsgBuf, Tv_PtrMsg, Tv_PtrMsg->Size);
			GrBufVqOut(m_PtrMsgBuf);		//release message

			if(LcProcMsg((Ptr_GrTaskMsgHd)Tv_MsgBuf))
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcProcMsg( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//process basic message
		if ( LcProcSvrMsg( A_PtrMsg ) )
		{
			Tv_Result		=	TRUE;
		}
		else
		{
			//check exist client
			if ( (0 != m_MsgCliAreaCnt) && m_IsCliRun )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < m_MsgCliAreaCnt;Tv_WkIdx++ )
				{
					if ( NULL != m_MsgCliTbl[Tv_WkIdx] )
					{
						if ( m_MsgCliTbl[Tv_WkIdx]->RtlMsgProc( A_PtrMsg ) )
						{
							Tv_Result		=	TRUE;
						}
					}
				}
			}
		}
		//mark finish code
		if ( NULL != A_PtrMsg->PtrRtl )
		{
			if ( Tv_Result )
			{
				A_PtrMsg->PtrRtl->Stat	=	E_GrTaskSvrMsgFcodeOk;		//mark finish
			}
			else
			{
				A_PtrMsg->PtrRtl->Stat	=	E_GrTaskSvrMsgFcodeNoCli;		//not exist client
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcProcBkg( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE; 

		//check exist client
		if ( (0 != m_BkgCliAreaCnt) && m_IsCliRun )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_BkgCliAreaCnt;Tv_WkIdx++ )
			{
				if ( NULL != m_BkgCliTbl[Tv_WkIdx] )
				{
					if ( m_BkgCliTbl[Tv_WkIdx]->RtlBkgProc() )
					{
						Tv_Result		=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrTaskSvr::LcTimerIdxToHndl( __u32 A_PriodIdx, __u32 A_SlotIdx )
{
		return	( ((A_PriodIdx & 0xFF) << E_GrTaskSvrTmHndlPdPos) | (A_SlotIdx & 0xFF) );
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::LcTimerHndlToIdx( __u32 A_Hndl, __u32* A_PtrRtPriodIdx, __u32* A_PtrRtSlotIdx )
{
		*A_PtrRtPriodIdx	=	(A_Hndl >> E_GrTaskSvrTmHndlPdPos) & 0xFF;
		*A_PtrRtSlotIdx		=	(A_Hndl & 0xFF);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcProcTimer( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Tick;
		__u32		Tv_TmIdx;
		__u32		Tv_CliIdx;
		__u32		Tv_Hndl;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( (0 != m_TimerAreaCnt) && m_IsCliRun )
		{
			Tv_Tick		=	GrTimeGetTick();
			for (Tv_TmIdx=0;Tv_TmIdx < m_TimerAreaCnt;Tv_TmIdx++	)
			{
				//check time slot
				if ( 0 != m_TimerTbl[Tv_TmIdx].AreaCnt )
				{
					//check tick
					if ( m_TimerTbl[Tv_TmIdx].Priod <= (Tv_Tick - m_TimerTbl[Tv_TmIdx].TickLast) )
					{
						Tv_Hndl		=	Tv_TmIdx << E_GrTaskSvrTmHndlPdPos;
						for ( Tv_CliIdx=0;Tv_CliIdx < m_TimerTbl[Tv_TmIdx].AreaCnt;Tv_CliIdx++ )
						{
							if ( NULL != m_TimerTbl[Tv_TmIdx].CliTbl[Tv_CliIdx] )
							{
								m_TimerTbl[Tv_TmIdx].CliTbl[Tv_CliIdx]->RtlTimer( Tv_Hndl );
								Tv_Result		=	TRUE;
							}
							//next
							Tv_Hndl ++;
						}
						//update time
						m_TimerTbl[Tv_TmIdx].TickLast	=	Tv_Tick;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::PrioritySet( __u32 A_Priority )
{
	// local -------------------
	// code --------------------
		if ( E_GrTaskSvrPrioAlways == A_Priority )
		{
			m_IsPrioRest	=	FALSE;
		}
		else
		{
			m_IsPrioRest		=	TRUE;
			m_IsPrioGap			=	FALSE;
			if ( (E_GrTaskSvrPrioTerm0 <= A_Priority) && (E_GrTaskSvrPrioTerm3 >= A_Priority) )
			{
				//term
				m_IsPrioQuat		=	FALSE;
				m_PrioBreak			= (A_Priority - E_GrTaskSvrPrioTerm0) * E_GrTaskSvrPrioTermStep;
			}
			else if ( (E_GrTaskSvrPrioGap0 <= A_Priority) && (E_GrTaskSvrPrioGap3 >= A_Priority) )
			{
				//gap
				m_IsPrioGap			=	TRUE;
				m_IsPrioQuat		=	FALSE;
				m_PrioBreak			= (A_Priority - E_GrTaskSvrPrioGap0) * E_GrTaskSvrPrioTermStep;
			}
			else if ( (E_GrTaskSvrPrioQuat0 <= A_Priority) && (E_GrTaskSvrPrioQuat3 >= A_Priority) )
			{
				//quarter
				m_IsPrioQuat		=	TRUE;
				m_PrioBreak			= (A_Priority - E_GrTaskSvrPrioQuat0) * E_GrTaskSvrPrioQuatStep;
				if ( 0 == m_PrioBreak )
				{
					m_PrioBreak		=	E_GrTaskSvrPrioQuatZero;
				}
				m_PrioPriod			=	GrTimeMsToTick( E_GrTimeMiliOfSec - m_PrioBreak );
				m_PrioTick			=	GrTimeGetTick();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::Run( void )
{
	// local -------------------
		BOOL8		Tv_IsBreak;
		__u32		Tv_Tick;
	// code --------------------
		if ( m_IsAble )
		{
			m_IsThrdRun		=	TRUE;
			//m_IsCliRun		=	TRUE;

			while ( !m_IsReqQuit )
			{
				//process
				Tv_IsBreak		=	TRUE;
				if ( DoStep() )
				{
					if ( m_IsPrioGap )
					{
						Tv_IsBreak	=	FALSE;
					}
				}

				//break time
				if ( Tv_IsBreak )
				{
					if ( m_IsPrioQuat )
					{
						//quarter
						Tv_Tick	=	GrTimeGetTick();
						if ( m_PrioPriod <= (Tv_Tick - m_PrioTick) )
						{
							GrPrcSleep( m_PrioBreak );
							//update
							m_PrioTick		=	Tv_Tick;
						}
					}
					else
					{
						//term
						GrPrcSleep( m_PrioBreak );
					}
				}
			}
			//mark finish
			m_IsThrdRun		=	FALSE;
			m_IsCliRun		=	FALSE;
		}
}
//--------------------------------------------------------------------
#ifndef E_GrTaskSvrSendPump
#ifdef E_GrTaskSvrDbgApi
BOOL8	Cls_GrTaskSvr::MsgSend(Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_Size, BOOL8 A_IsDbg)
#else
BOOL8	Cls_GrTaskSvr::MsgSend(	Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_Size )
#endif
{
	// local -------------------
		BOOL8		Tv_Result;
		Def_GrPrcThrdId	Tv_ThrdId;
		St_GrTaskMsgRtl	Tv_Mrtl;
		Ptr_GrTaskMsgHd	Tv_PtrMsg;
		BOOL8		Tv_IsDirect;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check direct message
		Tv_IsDirect	=	FALSE;
		//check thread
		Tv_ThrdId		=	GrPrcThrdGetCurId();
		if ( GrPrcThrdIsEqual( Tv_ThrdId, m_ThrdId ) )
		{
			Tv_IsDirect		=	TRUE;
		}

		//init
		A_PtrMsg->Size	=	A_Size;
		Tv_Mrtl.Stat		=	E_GrTaskSvrMsgFcodeNone;

		//check direct mode
		if ( Tv_IsDirect )
		{
#ifdef E_GrTaskSvrDbgApi
			if( A_IsDbg)
			{
#ifdef LINUX_APP
				printf("Cls_GrTaskSvr::MsgSend - direct mode!\n");
#endif
			}
#endif
			//direct process
			A_PtrMsg->PtrRtl	=	NULL;
			Tv_Result		=	LcProcMsg( A_PtrMsg );
		}
		else
		{
			//post 
			if ( E_GrTaskSvrMsgMaxSize >= A_Size )
			{
				//critical
				if ( GrPrcCritEnter( &m_Crit ) )
				{
					//allocate queue
					Tv_PtrMsg	=	(Ptr_GrTaskMsgHd)GrBufVqGetInPtr( m_PtrMsgBuf, A_Size  );
					if ( NULL != Tv_PtrMsg )
					{
						GrDumyCopyMem( (void*)Tv_PtrMsg, (void*)A_PtrMsg, A_Size );
						Tv_PtrMsg->PtrRtl		=	&Tv_Mrtl;
						GrBufVqIn( m_PtrMsgBuf );
						Tv_Result		=	TRUE;

#ifdef E_GrTaskSvrDbgApi
						if( A_IsDbg)
						{
#ifdef LINUX_APP
							printf("Cls_GrTaskSvr::MsgSend - message queu in!\n");
#endif
						}
#endif
					}
					else
					{
#ifdef LINUX_APP
						printf("Cls_GrTaskSvr::MsgSend - Message buffer full\n");
#else
						DbgMsgPrint("Cls_GrTaskSvr::MsgSend - Message buffer full\n");
#endif
					}
					GrPrcCritLeave( &m_Crit );

					//wait result
					if ( Tv_Result )
					{
						while ( E_GrTaskSvrMsgFcodeNone == Tv_Mrtl.Stat )
						{
							GrPrcSleep( E_GrTaskSvrMsgWaitStep );
							if(m_IsReqQuit)
							{
								break;
							}
						}
						if ( E_GrTaskSvrMsgFcodeOk != Tv_Mrtl.Stat )
						{
							Tv_Result		=	FALSE;
						}
					}
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrTaskSvr::MsgSend - Bad size message\n");
			}
		}
		
		return	Tv_Result;
}
#else
BOOL8	Cls_GrTaskSvr::MsgSend(Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_Size)
{
	// local -------------------
		Cls_GrTaskSvr*	Tv_TskNow;
		Def_GrPrcThrdId	Tv_ThrdId;
		St_GrTaskMsgRtl	Tv_Mrtl;
		Ptr_GrTaskMsgHd	Tv_PtrMsg;
	// code --------------------

		if(0 > m_SvrId)
		{
			DbgMsgPrint("Cls_GrTaskSvr::MsgSend - [BUG!] task not registered!\n");
			return	FALSE;
		}

		if(E_GrTaskSvrMsgMaxSize < A_Size)
		{
			DbgMsgPrint("Cls_GrTaskSvr::MsgSend - [BUG!] message size (%d) over \n", A_Size);
			return	FALSE;
		}

		// init
		A_PtrMsg->Size	=	A_Size;
		Tv_Mrtl.Stat		=	E_GrTaskSvrMsgFcodeNone;

		// get my thread
		Tv_ThrdId		=	GrPrcThrdGetCurId();

		// check my task
		if(Tv_ThrdId == V_GrTaskEnv.SvrRegTbl[m_SvrId].ThrdId)
		{
			//direct process
			A_PtrMsg->PtrRtl	=	NULL;
			return	LcProcMsg(A_PtrMsg);
		}

		// far task
		Tv_TskNow		=	(Cls_GrTaskSvr*)GrTaskSvrGetByThrdId(Tv_ThrdId);

		//critical
		Tv_PtrMsg		=	NULL;
		if(GrPrcCritEnter(&m_Crit))
		{
			//allocate queue
			Tv_PtrMsg	=	(Ptr_GrTaskMsgHd)GrBufVqGetInPtr(m_PtrMsgBuf, A_Size);
			if(NULL != Tv_PtrMsg)
			{
				GrDumyCopyMem((void*)Tv_PtrMsg, (void*)A_PtrMsg, A_Size);
				Tv_PtrMsg->PtrRtl		=	&Tv_Mrtl;
				GrBufVqIn(m_PtrMsgBuf);
			}
			GrPrcCritLeave(&m_Crit);
		}	//if(GrPrcCritEnter(&m_Crit))

		// check message allocate fail
		if(NULL == Tv_PtrMsg)
		{
			return	FALSE;
		}

		//wait result
		while(E_GrTaskSvrMsgFcodeNone == Tv_Mrtl.Stat)
		{
			if(NULL != Tv_TskNow)
			{

			}
			if(!Tv_TskNow->RtlMsgPump())
			{
				GrPrcSleep(E_GrTaskSvrMsgWaitStep);
			}

			// check finish
			if(m_IsReqQuit)
			{
				break;
			}
		}

		// check fail
		if(E_GrTaskSvrMsgFcodeOk != Tv_Mrtl.Stat)
		{
			return	FALSE;
		}

		return	TRUE;

}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::MsgPost(Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_Size, BOOL8 A_IsFast, BOOL8 A_ShowMsg)
{
	// local -------------------
		BOOL8		Tv_Result;
		Def_GrPrcThrdId	Tv_ThrdId;
		Ptr_GrTaskMsgHd	Tv_PtrMsg;
		BOOL8		Tv_IsDirect;
	// code --------------------
		// check registered
		if(0 > m_SvrId)
		{
			DbgMsgPrint("Cls_GrTaskSvr::MsgPost - task not registered!\n");
			return	FALSE;
		}

		//init
		Tv_Result		=	FALSE;

		A_PtrMsg->Size	=	A_Size;
		//check direct message
		Tv_IsDirect	=	FALSE;
		//check thread
		if ( A_IsFast )
		{
			Tv_ThrdId		=	GrPrcThrdGetCurId();
			if ( GrPrcThrdIsEqual( Tv_ThrdId, V_GrTaskEnv.SvrRegTbl[m_SvrId].ThrdId ) )
			{
				Tv_IsDirect		=	TRUE;
			}
		}

		//not need wait
		A_PtrMsg->PtrRtl	=	NULL;
		//check direct mode
		if ( Tv_IsDirect )
		{
			//direct process
			Tv_Result		=	LcProcMsg( A_PtrMsg );
		}
		else
		{
			//post 
			//check message size
			if ( E_GrTaskSvrMsgMaxSize >= A_Size )
			{
				//critical
				if ( GrPrcCritEnter( &m_Crit ) )
				{
					//allocate queue
					Tv_PtrMsg	=	(Ptr_GrTaskMsgHd)GrBufVqGetInPtr( m_PtrMsgBuf, A_Size  );
					if ( NULL != Tv_PtrMsg )
					{
						GrDumyCopyMem( (void*)Tv_PtrMsg, (void*)A_PtrMsg, A_Size );
						GrBufVqIn( m_PtrMsgBuf );
						Tv_Result		=	TRUE;
					}
					else
					{
						if(A_ShowMsg)
						{
							DbgMsgPrint("Cls_GrTaskSvr::MsgPost - Task %d msg %d buffer full\n",
								V_GrTaskEnv.SvrRegTbl[m_SvrId].Fcc, A_PtrMsg->Cmd);
						}
					}
					GrPrcCritLeave( &m_Crit );
				}

			}
			else
			{
				if(A_ShowMsg)
				{
					DbgMsgPrint("Cls_GrTaskSvr::MsgPost - Bad size message\n");
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::MsgExPost( __u32 A_Cmd , void* A_PtrData, __u32 A_SizeData )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrTaskMsgHd	Tv_PtrMsg;
		__u32		Tv_Size;
		void*		Tv_PtrPara;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//post 
		//check message size
		if ( E_GrTaskSvrMsgMaxSize >= (A_SizeData + sizeof(St_GrTaskMsgHd)) )
		{

			//critical
			if ( GrPrcCritEnter( &m_Crit ) )
			{
				Tv_Size	=	A_SizeData + sizeof(St_GrTaskMsgHd);
				//allocate queue
				Tv_PtrMsg	=	(Ptr_GrTaskMsgHd)GrBufVqGetInPtr( m_PtrMsgBuf, Tv_Size  );
				if ( NULL != Tv_PtrMsg )
				{
					//init head
					Tv_PtrMsg->Cmd		=	A_Cmd;
					Tv_PtrMsg->Size		=	Tv_Size;
					Tv_PtrMsg->PtrRtl	=	NULL;
					if ( 0 != A_SizeData )
					{
						Tv_PtrPara	=	(void*)( (Def_GrCalPtr)Tv_PtrMsg + sizeof(St_GrTaskMsgHd) );
						GrDumyCopyMem( Tv_PtrPara, (void*)A_PtrData, A_SizeData );
					}
					GrBufVqIn( m_PtrMsgBuf );
					Tv_Result		=	TRUE;
				}
				else
				{
					DbgMsgPrint("Cls_GrTaskSvr::MsgExPost - Message buffer full\n");
				}
				GrPrcCritLeave( &m_Crit );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrTaskSvr::MsgExPost - Bad size message\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::RegistMsgClient( Cls_GrTaskCli* A_Obj )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrTaskSvrMsgRegCli	Tv_Msg;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//set message
		Tv_Msg.Hd.Cmd		=	E_GrTaskSvrMsgRegCli;
		Tv_Msg.ObjCli		=	A_Obj;
		Tv_Msg.Type			=	E_GrTaskSvrCliTypeMsg;
		Tv_Msg.PtrRslt	=	&Tv_Result;
		MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::UnregistMsgClient( Cls_GrTaskCli* A_Obj )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrTaskSvrMsgUnregCli	Tv_Msg;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//set message
		Tv_Msg.Hd.Cmd		=	E_GrTaskSvrMsgUnregCli;
		Tv_Msg.ObjCli		=	A_Obj;
		Tv_Msg.Type			=	E_GrTaskSvrCliTypeMsg;
		Tv_Msg.PtrRslt	=	&Tv_Result;
		MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::RegistBkgClient( Cls_GrTaskCli* A_Obj )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrTaskSvrMsgRegCli	Tv_Msg;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//set message
		Tv_Msg.Hd.Cmd		=	E_GrTaskSvrMsgRegCli;
		Tv_Msg.ObjCli		=	A_Obj;
		Tv_Msg.Type			=	E_GrTaskSvrCliTypeBkg;
		Tv_Msg.PtrRslt	=	&Tv_Result;
		MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::UnregistBkgClient( Cls_GrTaskCli* A_Obj )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrTaskSvrMsgUnregCli	Tv_Msg;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//set message
		Tv_Msg.Hd.Cmd		=	E_GrTaskSvrMsgUnregCli;
		Tv_Msg.ObjCli		=	A_Obj;
		Tv_Msg.Type			=	E_GrTaskSvrCliTypeBkg;
		Tv_Msg.PtrRslt	=	&Tv_Result;
		MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::RegistTimerClient( Cls_GrTaskCli* A_Obj, __u32 A_Priod, __u32* A_PtrRtHndl )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrTaskSvrMsgRegCli	Tv_Msg;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//set message
		Tv_Msg.Hd.Cmd		=	E_GrTaskSvrMsgRegCli;
		Tv_Msg.ObjCli		=	A_Obj;
		Tv_Msg.Type			=	E_GrTaskSvrCliTypeTimer;
		Tv_Msg.Priod		=	A_Priod;
		Tv_Msg.PtrRtHndl	=	A_PtrRtHndl;
		Tv_Msg.PtrRslt	=	&Tv_Result;
		MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::UnregistTimerClient( __u32 A_Hndl )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrTaskSvrMsgUnregCli	Tv_Msg;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//set message
		Tv_Msg.Hd.Cmd		=	E_GrTaskSvrMsgUnregCli;
		Tv_Msg.Hndl			=	A_Hndl;
		Tv_Msg.Type			=	E_GrTaskSvrCliTypeTimer;
		Tv_Msg.PtrRslt	=	&Tv_Result;
		MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcProcSvrMsg( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	TRUE;
		switch( A_PtrMsg->Cmd )
		{
			case E_GrTaskSvrMsgRegCli:
				LcPmsgRegCli( (Ptr_GrTaskSvrMsgRegCli)A_PtrMsg );
				break;
			case E_GrTaskSvrMsgUnregCli:
				LcPmsgUnregCli( (Ptr_GrTaskSvrMsgUnregCli)A_PtrMsg );
				break;
			default:
				Tv_Result		=	FALSE;
		    break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::LcPmsgRegCli( Ptr_GrTaskSvrMsgRegCli A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		switch( A_PtrMsg->Type )
		{
			case E_GrTaskSvrCliTypeMsg:
				Tv_Result	=	LcRegCliMsg( A_PtrMsg->ObjCli );
				break;
			case E_GrTaskSvrCliTypeBkg:
				Tv_Result	=	LcRegCliBkg( A_PtrMsg->ObjCli );
				break;
			case E_GrTaskSvrCliTypeTimer:
				Tv_Result	=	LcRegCliTimer( A_PtrMsg->ObjCli, A_PtrMsg->Priod, A_PtrMsg->PtrRtHndl );
				break;
			default:
				//bad client type
				Tv_Result	=	FALSE;
		    break;
		}

		//update result - check send
		if ( NULL != A_PtrMsg->PtrRslt )
		{
			*A_PtrMsg->PtrRslt	=	Tv_Result;
		}
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::LcPmsgUnregCli( Ptr_GrTaskSvrMsgUnregCli A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		switch( A_PtrMsg->Type )
		{
			case E_GrTaskSvrCliTypeMsg:
				Tv_Result	=	LcUnregCliMsg( A_PtrMsg->ObjCli );
				break;
			case E_GrTaskSvrCliTypeBkg:
				Tv_Result	=	LcUnregCliBkg( A_PtrMsg->ObjCli );
				break;
			case E_GrTaskSvrCliTypeTimer:
				Tv_Result	=	LcUnregCliTimer( A_PtrMsg->Hndl );
				break;
			default:
				//bad client type
				Tv_Result	=	FALSE;
				break;
		}

		//update result - check send
		if ( NULL != A_PtrMsg->PtrRslt )
		{
			*A_PtrMsg->PtrRslt	=	Tv_Result;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcFindMsgTblEmpty( __u32* A_PtrRtIdx )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTaskSvrMaxCli;Tv_WkIdx++ )
		{
			if ( NULL == m_MsgCliTbl[Tv_WkIdx] )
			{
				*A_PtrRtIdx	=	Tv_WkIdx;
				Tv_Result		=	TRUE;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::LcMsgTblAreaUpdt( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		m_MsgCliAreaCnt	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTaskSvrMaxCli;Tv_WkIdx++ )
		{
			if ( NULL != m_MsgCliTbl[Tv_WkIdx] )
			{
				m_MsgCliAreaCnt	=	Tv_WkIdx + 1;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcRegCliMsg( Cls_GrTaskCli* A_Obj )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_SlotIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check already exist
		if ( !LcFindMsgTblByObj( A_Obj, &Tv_SlotIdx ) )
		{
			//find empty slot
			if ( LcFindMsgTblEmpty( &Tv_SlotIdx ) )
			{
				m_MsgCliTbl[Tv_SlotIdx]	=	A_Obj;
				if ( Tv_SlotIdx >= m_MsgCliAreaCnt )
				{
					//patch area
					m_MsgCliAreaCnt	=	Tv_SlotIdx + 1;
				}

				Tv_Result		=	TRUE;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcFindMsgTblByObj( Cls_GrTaskCli* A_Obj, __u32* A_PtrRtIdx )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( 0 != m_MsgCliAreaCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_MsgCliAreaCnt;Tv_WkIdx++ )
			{
				if ( A_Obj == m_MsgCliTbl[Tv_WkIdx] )
				{
					*A_PtrRtIdx	=	Tv_WkIdx;
					Tv_Result		=	TRUE;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcUnregCliMsg( Cls_GrTaskCli* A_Obj )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_SlotIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check already exist
		if ( LcFindMsgTblByObj( A_Obj, &Tv_SlotIdx ) )
		{
			m_MsgCliTbl[Tv_SlotIdx]	=	NULL;
			if ( (Tv_SlotIdx + 1) == m_MsgCliAreaCnt )
			{
				LcMsgTblAreaUpdt();
			}
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcFindBkgTblEmpty( __u32* A_PtrRtIdx )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTaskSvrMaxCli;Tv_WkIdx++ )
		{
			if ( NULL == m_BkgCliTbl[Tv_WkIdx] )
			{
				*A_PtrRtIdx	=	Tv_WkIdx;
				Tv_Result		=	TRUE;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::LcBkgTblAreaUpdt( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		m_BkgCliAreaCnt	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTaskSvrMaxCli;Tv_WkIdx++ )
		{
			if ( NULL != m_BkgCliTbl[Tv_WkIdx] )
			{
				m_BkgCliAreaCnt	=	Tv_WkIdx + 1;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcFindBkgTblByObj( Cls_GrTaskCli* A_Obj, __u32* A_PtrRtIdx )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( 0 != m_BkgCliAreaCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_BkgCliAreaCnt;Tv_WkIdx++ )
			{
				if ( A_Obj == m_BkgCliTbl[Tv_WkIdx] )
				{
					*A_PtrRtIdx	=	Tv_WkIdx;
					Tv_Result		=	TRUE;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcRegCliBkg( Cls_GrTaskCli* A_Obj )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_SlotIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check exist
		if ( !LcFindBkgTblByObj( A_Obj, &Tv_SlotIdx) )
		{
			//find empty slot
			if ( LcFindBkgTblEmpty( &Tv_SlotIdx ) )
			{
				m_BkgCliTbl[Tv_SlotIdx]	=	A_Obj;
				if ( Tv_SlotIdx >= m_BkgCliAreaCnt )
				{
					//patch area
					m_BkgCliAreaCnt	=	Tv_SlotIdx + 1;
				}

				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcUnregCliBkg( Cls_GrTaskCli* A_Obj )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_SlotIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check already exist
		if ( LcFindBkgTblByObj( A_Obj, &Tv_SlotIdx ) )
		{
			m_BkgCliTbl[Tv_SlotIdx]	=	NULL;
			if ( (Tv_SlotIdx + 1) == m_BkgCliAreaCnt )
			{
				LcBkgTblAreaUpdt();
			}
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcFindTmPdEmpty( __u32* A_PtrRtIdx )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTaskSvrMaxTimer;Tv_WkIdx++ )
		{
			if ( 0 == m_TimerTbl[Tv_WkIdx].AreaCnt	)
			{
				*A_PtrRtIdx	=	Tv_WkIdx;
				Tv_Result		=	TRUE;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::LcTmPdAreaUpdt( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		m_TimerAreaCnt	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTaskSvrMaxTimer;Tv_WkIdx++ )
		{
			if ( 0 != m_TimerTbl[Tv_WkIdx].AreaCnt	)
			{
				m_TimerAreaCnt	=	Tv_WkIdx + 1;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcFindTmPdByPriod( __u32 A_Priod, __u32* A_PtrRtIdx )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
		__u32		Tv_Priod;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( 0 != m_TimerAreaCnt )
		{
			Tv_Priod	=	GrTimeMsToTick( A_Priod );
			for ( Tv_WkIdx=0;Tv_WkIdx < m_TimerAreaCnt;Tv_WkIdx++ )
			{
				if ( (0 != m_TimerTbl[Tv_WkIdx].AreaCnt) && ( Tv_Priod == m_TimerTbl[Tv_WkIdx].Priod) )
				{
					*A_PtrRtIdx	=	Tv_WkIdx;
					Tv_Result		=	TRUE;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcFindTmCliEmpty( __u32 A_PdIdx, __u32* A_PtrRtIdx )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( A_PdIdx < E_GrTaskSvrMaxTimer )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTaskSvrMaxCli;Tv_WkIdx++ )
			{
				if ( NULL == m_TimerTbl[A_PdIdx].CliTbl[Tv_WkIdx] )
				{
					*A_PtrRtIdx	=	Tv_WkIdx;
					Tv_Result		=	TRUE;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::LcTmCliAreaUpdt( __u32 A_PdIdx )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		if ( A_PdIdx < E_GrTaskSvrMaxTimer )
		{
			m_TimerTbl[A_PdIdx].AreaCnt	=	0;
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrTaskSvrMaxCli;Tv_WkIdx++ )
			{
				if ( NULL != m_TimerTbl[A_PdIdx].CliTbl[Tv_WkIdx]	)
				{
					m_TimerTbl[A_PdIdx].AreaCnt	=	Tv_WkIdx + 1;
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcFindTmCliByObj( __u32 A_PdIdx, Cls_GrTaskCli* A_Obj, __u32* A_PtrRtIdx )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( A_PdIdx < m_TimerAreaCnt )
		{
			if ( 0 != m_TimerTbl[A_PdIdx].AreaCnt )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < m_TimerTbl[A_PdIdx].AreaCnt;Tv_WkIdx++ )
				{
					if (A_Obj == m_TimerTbl[A_PdIdx].CliTbl[Tv_WkIdx])
					{
						*A_PtrRtIdx	=	Tv_WkIdx;
						Tv_Result		=	TRUE;
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcRegCliTimer( Cls_GrTaskCli* A_Obj, __u32 A_Priod, __u32* A_PtrRtHndl )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Pid;			//priod index
		__u32		Tv_Cid;			//client index
	// code --------------------

		if(0 > m_SvrId)
		{
			return	FALSE;
		}

		//init
		Tv_Result		=	TRUE;

		//check exist pid
		if ( !LcFindTmPdByPriod( A_Priod, &Tv_Pid) )
		{
			Tv_Result	=	LcFindTmPdEmpty( &Tv_Pid );
		}
		//find client
		if ( Tv_Result )
		{
			//init
			Tv_Result		=	FALSE;

			//check exist object
			if ( !LcFindTmCliByObj( Tv_Pid, A_Obj, &Tv_Cid ) )
			{
				//find empty
				if ( LcFindTmCliEmpty( Tv_Pid, &Tv_Cid ) )
				{
					//regist pid
					if ( Tv_Pid >= m_TimerAreaCnt )
					{
						m_TimerAreaCnt	=	Tv_Pid + 1;
					}
					//update new pid
					if ( 0 == m_TimerTbl[Tv_Pid].AreaCnt )
					{
						m_TimerTbl[Tv_Pid].TickLast	=	GrTimeGetTick();
						m_TimerTbl[Tv_Pid].Priod		=	GrTimeMsToTick(A_Priod);
					}
					//update client area count
					if ( Tv_Cid >= m_TimerTbl[Tv_Pid].AreaCnt )
					{
						m_TimerTbl[Tv_Pid].AreaCnt	=	Tv_Cid + 1;
					}
					//regist client
					m_TimerTbl[Tv_Pid].CliTbl[Tv_Cid]	=	A_Obj;
					*A_PtrRtHndl	=	LcTimerIdxToHndl( Tv_Pid, Tv_Cid );
					Tv_Result		=	TRUE;
				}
				else
				{
					DbgMsgPrint("Cls_GrTaskSvr::LcRegCliTimer - task %d empty slot not found!\n", 
						V_GrTaskEnv.SvrRegTbl[m_SvrId].Fcc);
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrTaskSvr::LcRegCliTimer - task %d client already exist!\n", 
					V_GrTaskEnv.SvrRegTbl[m_SvrId].Fcc);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrTaskSvr::LcRegCliTimer - task %d PID not found!\n", 
				V_GrTaskEnv.SvrRegTbl[m_SvrId].Fcc);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::LcUnregCliTimer( __u32 A_Hndl )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Pid;
		__u32		Tv_Cid;
	// code --------------------
		Tv_Result	=	FALSE;
		LcTimerHndlToIdx( A_Hndl, &Tv_Pid, &Tv_Cid );

		//check current
		if ( Tv_Pid < m_TimerAreaCnt )
		{
			if ( Tv_Cid < m_TimerTbl[Tv_Pid].AreaCnt )
			{
				//delete
				m_TimerTbl[Tv_Pid].CliTbl[Tv_Cid]		=	NULL;
				//check client area
				if ( (Tv_Cid + 1) == m_TimerTbl[Tv_Pid].AreaCnt )
				{
					LcTmCliAreaUpdt( Tv_Pid );
				}
				//check period area
				if ( 0 == m_TimerTbl[Tv_Pid].AreaCnt )
				{
					if ( (Tv_Pid + 1) == m_TimerAreaCnt )
					{
						LcTmPdAreaUpdt();
					}
				}
				//success
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::ReqQuit( void )
{
		m_IsReqQuit	=	TRUE;
}
//--------------------------------------------------------------------
__u32	Cls_GrTaskSvr::Fcc( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if(0 <= m_SvrId)
		{
			Tv_Result	=	V_GrTaskEnv.SvrRegTbl[m_SvrId].Fcc;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::RtlRun( void )
{
		if(0 > m_SvrId)
		{
			DbgMsgPrint("Cls_GrTaskSvr::RtlRun - task not registered!\n");
			return;
		}
		//LINUX signal block
		#ifdef LINUX_APP
			sigset(SIGPIPE, SIG_IGN); 
		#endif
		V_GrTaskEnv.SvrRegTbl[m_SvrId].ThrdId	=	GrPrcThrdGetCurId();
		m_IsAble		=	TRUE;
		Run();
}
//--------------------------------------------------------------------
Def_GrPrcThrdId	Cls_GrTaskSvr::ThreadId( void )
{
	// local -------------------
		Def_GrPrcThrdId	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if(0 <= m_SvrId)
		{
			Tv_Result	=	V_GrTaskEnv.SvrRegTbl[m_SvrId].ThrdId;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::CliRunSet( BOOL8 A_IsRun )
{
		m_IsCliRun	=	A_IsRun;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrTaskSvr::IsRun( void )
{
		return	m_IsThrdRun;
}
//--------------------------------------------------------------------
void	Cls_GrTaskSvr::MsgPumpCntSet(__u32 A_Cnt)
{
	// local -------------------
	// code --------------------
		if( 0 != A_Cnt)
		{
			m_MsgPumpCnt	=	A_Cnt;
		}

}
//--------------------------------------------------------------------
