/*
	DVR play main

*/

//====================================================================
// uses
#include <Dvr/Fs/V5/GrFs5Play.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <GrDebug.h>
#include <GrProcess.h>
#include <GrError.h>

#ifdef LINUX_APP

#include <stdlib.h>

#include <sys/mman.h>


#endif

//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//local function


//====================================================================
// global var

//====================================================================
//--------------------------------------------------------------------
Cls_GrFs5Play::Cls_GrFs5Play(Cls_GrTaskSvr* A_TaskSvr, __u32 A_TryCnt, __u32 A_MaxFrmSize) :
Cls_GrFs5Dmux(A_TaskSvr, A_MaxFrmSize)
{
	// local -------------------
		//__u8*	Tv_PtrBt;
	// code --------------------

		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		// init
		m_PlayTryCnt			=	A_TryCnt;

#ifdef E_GrFs5PlayTimeDirect
		m_PlayTmLast					=	0;
#endif

		m_PlayChCnt						=	E_GrDvrMaxChCnt;

		m_PlaySyncMode				=	E_GrFs5PlaySyncAuto;

		m_IsPlayRun						=	FALSE;
		m_IsPlayFrmUsed				=	FALSE;
		m_PlayTimeSec					=	0;
		m_PlayTimeMili				=	0;

		m_PlaySpdShif					=	0;

		GrDumyZeroMem(m_PlayChTbl,sizeof(m_PlayChTbl));

#ifdef E_GrFs5PlayShowFps
		m_Fps	=	0;
		//GrDumyZeroMem( m_Fps, sizeof(m_Fps) );
#endif

		//register background
		m_TaskSvr->RegistMsgClient(this);
		m_TaskSvr->RegistBkgClient(this);
		m_TaskSvr->RegistTimerClient(this, E_GrFs5PlayTimerPeriod, &m_HndlTimer);
}
//--------------------------------------------------------------------
Cls_GrFs5Play::~Cls_GrFs5Play()
{
	// local -------------------
		//__u32	Tv_WkIdx;
	// code --------------------

		//unregister background
		m_TaskSvr->UnregistBkgClient(this);
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );
		m_TaskSvr->UnregistMsgClient(this);

		GrTaskSvrMsgCmdFree(m_MsgAloc);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5Play::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// system message
		if(m_MsgAloc == A_PtrMsg->Cmd)
		{
			// local command
			Tv_Result	=	TRUE;
			switch(((Ptr_GrFs5PlayMsgBase)A_PtrMsg)->Cmd)
			{
				case E_GrFs5PlayMsgCmdVdoChMapSet:
					ImmVdoChMapSet(((Ptr_GrFs5PlayMsgVdoChMapSet)A_PtrMsg)->Map);
					break;
				case E_GrFs5PlayMsgCmdAdoChMapSet:
					ImmAdoChMapSet(((Ptr_GrFs5PlayMsgAdoChMapSet)A_PtrMsg)->Map);
					break;
				case  E_GrFs5PlayMsgCmdIsPlaying:
					*((Ptr_GrFs5PlayMsgIsPlaying)A_PtrMsg)->PtrRslt	=
						ImmIsPlaying();
					break;
				case E_GrFs5PlayMsgCmdPlay:
					ImmPlay();
					break;
				case E_GrFs5PlayMsgCmdPause:
					ImmPause();
					break;
				case E_GrFs5PlayMsgCmdGoByTime:
					*((Ptr_GrFs5PlayMsgGoByTime)A_PtrMsg)->PtrRslt	=	
						ImmGoByTime(((Ptr_GrFs5PlayMsgGoByTime)A_PtrMsg)->Time);
					break;
				case E_GrFs5PlayMsgCmdSpeedSet:
					ImmPlaySpeedSet(((Ptr_GrFs5PlayMsgSpeedSet)A_PtrMsg)->Val);
					break;
				case E_GrFs5PlayMsgCmdSpeedGet:
					*((Ptr_GrFs5PlayMsgSpeedGet)A_PtrMsg)->PtrRslt		=
						ImmPlaySpeedGet();
					break;
				case E_GrFs5PlayMsgCmdDirSet:
					ImmPlayDirSet(((Ptr_GrFs5PlayMsgDirSet)A_PtrMsg)->IsBack);
					break;
				case E_GrFs5PlayMsgCmdDirGet:
					*((Ptr_GrFs5PlayMsgDirGet)A_PtrMsg)->PtrRslt		=
						ImmPlayDirGet();
					break;
				case E_GrFs5PlayMsgCmdAdoChSet:
						ImmAdoChSet(((Ptr_GrFs5PlayMsgAdoChSet)A_PtrMsg)->AdoCh);
					break;
				case E_GrFs5PlayMsgCmdStepForward:
					ImmStepForward();
					break;
				case E_GrFs5PlayMsgCmdStepBackward:
					ImmStepBackward();
					break;
				case E_GrFs5PlayMsgCmdStatGet:
					*((Ptr_GrFs5PlayMsgStatGet)A_PtrMsg)->PtrRslt	=
						ImmPlayStatGet();
					break;
				case E_GrFs5PlayMsgCmdSyncSet:
					ImmPlaySyncSet(((Ptr_GrFs5PlayMsgSyncSet)A_PtrMsg)->Mode);
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}
		else 
		{
			// system message
			if (E_GrTaskMsgFsStatChg == A_PtrMsg->Cmd)
			{
				// udpate dmux info
				DmuxDevClose();
				// stop playing
				m_DmuxRunStat	=	E_GrDvrPlayStatStop;
				m_IsPlayRun		=	FALSE;
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5Play::RtlBkgProc(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check able
		if(m_IsPlayRun || (E_GrDvrPlayStatSeek == m_DmuxRunStat))
		{
			Tv_Result	=	LcProcPlay();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
	// code --------------------
		//DbgMsgPrint( "DecCall- %d\n", m_Fps );
#ifdef E_GrFs5PlayShowFps
		m_Fps	=	0;
#endif

		// upate play time
		LcTmrPlayTime();
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::EvtVdoFmtChg(__u8 A_Ch)
{
	// local -------------------
	// code --------------------
		LcPlayPwrChUpdate(A_Ch);
		PlayPwrCtlUpdate();
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFs5Play::ImmGoByTime(Def_GrTime A_Time)
{
	// local -------------------
	// code --------------------
		// reset decoded
		m_IsPlayFrmUsed	=	FALSE;

		// reset time
#ifdef E_GrFs5PlayTimeDirect
		m_PlayTmLast		=	A_Time;
#endif
		m_PlayTimeSec		=	A_Time;
		m_PlayTimeMili	=	0;	

		m_PlayTimeTick	= m_PlayTimeSec << 32 | m_PlayTimeMili;

		return	DmuxGoTime(A_Time);
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ImmPlay(void)
{
	// local -------------------
	// code --------------------
		m_IsPlayRun		=	TRUE;

		// get play tick
		m_PlayTickLast	=	GrTimeGetTick();
		m_PlayTickTest	= GrTimeGetTick();
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ImmPause(void)
{
	// local -------------------
	// code --------------------

		LcPlayNowFrmFlush();

		m_IsPlayRun		=	FALSE;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5Play::LcProcPlay(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_TryCnt;
	// code --------------------
		Tv_Result	=	FALSE;

		// check status
		for(Tv_TryCnt = 0; Tv_TryCnt < m_PlayChCnt; Tv_TryCnt++)
		{
			if(E_GrDvrPlayStatStop == m_DmuxRunStat)
			{
				m_IsPlayRun	=	FALSE;
				break;
			}
			// check direction
			if(m_IsDmuxDirBack)
			{
				if(E_GrDvrPlayStatBegin == m_DmuxRunStat)
				{
					break;
				}
				if(LcSubPlayBw())
				{
					Tv_Result	=	TRUE;
				}
			}
			else
			{
				// forward
				if(LcSubPlayFw())
				{
					Tv_Result	=	TRUE;
				}
			}
			if(!Tv_Result)
			{
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5Play::LcPlayDecNowFrmFw(BOOL8 A_IsImmUpdt)
{
	// local -------------------
		BOOL8	Tv_IsDisp;
		BOOL8	Tv_IsPlay;
		void*	Tv_PtrStrm;
		char	Tv_StrTime[64];
	// code --------------------

		if((NULL == m_PtrDmuxOut) || m_IsPlayFrmUsed)
		{
			return	TRUE;
		}

		Tv_IsPlay	=	FALSE;

		// check video
		if((E_GrDvrFrmTypeVdoI <= m_PtrDmuxOut->Type) && (E_GrDvrFrmTypeVdoP4x >= m_PtrDmuxOut->Type))
		{
			Tv_IsDisp = FALSE;
			if(A_IsImmUpdt || (0 == m_PlayChTbl[m_PtrDmuxOut->Ch].DskipPrgs))
			{
				Tv_IsDisp = TRUE;
			}
			Tv_PtrStrm = (void*)((Def_GrCalPtr)m_PtrDmuxOut + sizeof(St_GrDvrFrmHdVdo));
			if(E_GrErrNone != PlayVdoDec(m_PtrDmuxOut->Ch, Tv_PtrStrm, m_PtrDmuxOut->DataSize, Tv_IsDisp))
			{
				return	FALSE;
			}

			if(A_IsImmUpdt)
			{
				// update new time
				LcPlayPrgsTimeSet(m_PtrDmuxOut->TimeSec, m_PtrDmuxOut->TimeMsec);
			}

			//test
			if(m_PlayTimeSec > m_PtrDmuxOut->TimeSec + 3)
			{
				GrStrTimeToStr(Tv_StrTime, m_PtrDmuxOut->TimeSec);
				DbgMsgPrint("[DEC:%s]\n", Tv_StrTime);
			}
			

			// update last frame time
#ifdef E_GrFs5PlayTimeDirect
			m_PlayTmLast	=	m_PtrPlayFrm->TimeSec;
#endif

			// next progress
			m_PlayChTbl[m_PtrDmuxOut->Ch].DskipPrgs ++;
			if(A_IsImmUpdt || (m_PlayChTbl[m_PtrDmuxOut->Ch].DskipPrgs >= m_PlayChTbl[m_PtrDmuxOut->Ch].DskipStep))
			{
				m_PlayChTbl[m_PtrDmuxOut->Ch].DskipPrgs	=	0;
			}

			// played
			Tv_IsPlay	=	TRUE;
#if 0
			GrStrTimeToStr(Tv_StrTime, m_PtrDmuxOut->TimeSec);
			DbgMsgPrint("[DEC:%s]\n", Tv_StrTime);
#endif
		}
		else if(E_GrDvrFrmTypeAdo == m_PtrDmuxOut->Type)
		{
			// do decode audio
			if((!A_IsImmUpdt) && ((0 == m_PlaySpdShif) || (E_GrFs5PlaySyncOff == m_PlaySyncMode)))
			{
				Tv_PtrStrm	=	(void*)((Def_GrCalPtr)m_PtrDmuxOut + sizeof(St_GrDvrFrmHdAdo));
				if(E_GrErrNone != PlayAdoDec(m_PtrDmuxOut->Ch, Tv_PtrStrm, m_PtrDmuxOut->DataSize))
				{
#if 0
					{
						char	Tv_StrTime[64];
						GrStrTimeToStr(Tv_StrTime, m_PtrDmuxOut->TimeSec);
						DbgMsgPrint("[DEC: ADO %s - wait]\n", Tv_StrTime);
					}
#endif
					return	FALSE;
				}

			}
		}
		// reset
		m_IsPlayFrmUsed	=	TRUE;		// frame used

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5Play::LcSubPlayFw(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_TryCnt;
		//__u32	Tv_MiliGap;
		__u32	Tv_GapLimit;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_GapLimit	=	E_GrFs5PlayGapTimeLimite << m_PlaySpdShif;


		// check status
		if((E_GrDvrPlayStatBegin == m_DmuxRunStat) || (E_GrDvrPlayStatEnd == m_DmuxRunStat))
		{
			DmuxGoNext();
			m_IsPlayFrmUsed	=	FALSE;
		}

		if(E_GrDvrPlayStatStop == m_DmuxRunStat)
		{
			return	FALSE;
		}

		LcPlayTimeGet();//test

		// normal mode
		for(Tv_TryCnt = 0; Tv_TryCnt < m_PlayTryCnt; Tv_TryCnt++)
		{
			// check play time
			if((NULL != m_PtrDmuxOut) && (!m_IsPlayFrmUsed))
			{
				// check play time
				if(E_GrFs5PlaySyncOff == m_PlaySyncMode)
				{
					LcPlayPrgsTimeSet(m_PtrDmuxOut->TimeSec, m_PtrDmuxOut->TimeMsec);
				}
				else
				{
					if(m_PtrDmuxOut->TimeSec > m_PlayTimeSec)
					{
						// check gap limite over
						if(Tv_GapLimit > (m_PtrDmuxOut->TimeSec - m_PlayTimeSec))
						{
							// do wait
							Tv_Result	=	FALSE;
							//DbgMsgPrint("Time wait %08X - %08X! TEST - 1\n", m_PtrDmuxOut->TimeSec, m_PlayTimeSec);
							break;
						}
						// update new time
						LcPlayPrgsTimeSet(m_PtrDmuxOut->TimeSec, m_PtrDmuxOut->TimeMsec);
					}
					else if((m_PtrDmuxOut->TimeSec == m_PlayTimeSec) && (m_PtrDmuxOut->TimeMsec > m_PlayTimeMili))
					{
						// wait
						Tv_Result	=	FALSE;
						//DbgMsgPrint("Time wait %08X - %08X! TEST - 2\n", m_PtrDmuxOut->TimeSec, m_PlayTimeSec);
						break;
					}
					// do decode
					// update over time
					if(Tv_GapLimit < (m_PlayTimeSec - m_PtrDmuxOut->TimeSec))
					{
						LcPlayPrgsTimeSet(m_PtrDmuxOut->TimeSec, m_PtrDmuxOut->TimeMsec);
					}
				}

				if(LcPlayDecNowFrmFw(FALSE))
				{
					Tv_Result	=	TRUE;
					// check play run
					if(!m_IsPlayRun)
					{
						break;
					}
				}
				else
				{
					break;
				}

			}	//	if(NULL != m_PtrPlayFrm)

			// go next frame
			DmuxGoNext();
			m_IsPlayFrmUsed	=	FALSE;
			if((E_GrDvrPlayStatStop == m_DmuxRunStat) || (E_GrDvrPlayStatEnd == m_DmuxRunStat))
			{
				m_IsPlayRun	=	FALSE;	// do stop
				Tv_Result	=	FALSE;
				break;
			}

#if 0
			else
			{
				DbgMsgPrint("[PLAY] prg:%08X\n", m_DmuxSctl.Prgs);
			}
#endif
		}	//	for(Tv_TryCnt = 0; Tv_TryCnt < E_GrFs5PlayTryCnt; Tv_TryCnt++)

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5Play::LcPlayDecNowFrmBw(BOOL8 A_IsImmUpdt)
{
	// local -------------------
		BOOL8	Tv_IsPlay;
		//__u32	Tv_MiliGap;
		void*	Tv_PtrStrm;
	// code --------------------

		if((NULL == m_PtrDmuxOut) || m_IsPlayFrmUsed)
		{
			return	TRUE;
		}

		Tv_IsPlay	=	FALSE;

		// check video
		if(E_GrDvrFrmTypeVdoI == m_PtrDmuxOut->Type)
		{

			Tv_PtrStrm	=	(void*)((Def_GrCalPtr)m_PtrDmuxOut + sizeof(St_GrDvrFrmHdVdo));
			if(E_GrErrNone != PlayVdoDec(m_PtrDmuxOut->Ch, Tv_PtrStrm, m_PtrDmuxOut->DataSize, TRUE))
			{
				return	FALSE;
			}

			if(A_IsImmUpdt)
			{
				// update new time
				LcPlayPrgsTimeSet(m_PtrDmuxOut->TimeSec, m_PtrDmuxOut->TimeMsec);
			}

			// update last frame time
#ifdef E_GrFs5PlayTimeDirect
			m_PlayTmLast	=	m_PtrDmuxOut->TimeSec;
#endif

			// next progress
			m_PlayChTbl[m_PtrDmuxOut->Ch].DskipPrgs	=	0;
			Tv_IsPlay	=	TRUE;

#if 0
			GrStrTimeToStr(Tv_StrTime, m_PtrDmuxOut->TimeSec);
			DbgMsgPrint("[DEC:%s]\n", Tv_StrTime);
#endif
		}
		else if(E_GrDvrFrmTypeAdo <= m_PtrDmuxOut->Type)
		{
			// do decode audio
			// none


		}

		// reset
		m_IsPlayFrmUsed	=	TRUE;

		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5Play::LcSubPlayBw(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_TryCnt;
		__u32	Tv_GapLimit;

#if 0
		Def_StrTag	Tv_StrTime;
#endif
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_GapLimit	=	E_GrFs5PlayGapTimeLimite << m_PlaySpdShif;

		if(E_GrDvrPlayStatEnd == m_DmuxRunStat)
		{
			DmuxGoNext();
			m_IsPlayFrmUsed	=	FALSE;
		}

		if((E_GrDvrPlayStatStop == m_DmuxRunStat) || (E_GrDvrPlayStatBegin == m_DmuxRunStat))
		{
			return	FALSE;
		}

		LcPlayTimeGet();//test

		// normal mode
		for(Tv_TryCnt = 0; Tv_TryCnt < m_PlayTryCnt; Tv_TryCnt++)
		{
			// check play time
			if((NULL != m_PtrDmuxOut) && (!m_IsPlayFrmUsed))
			{

				if(E_GrFs5PlaySyncOff == m_PlaySyncMode)
				{
					LcPlayPrgsTimeSet(m_PtrDmuxOut->TimeSec, m_PtrDmuxOut->TimeMsec);
				}
				else
				{
					if(m_PtrDmuxOut->TimeSec < m_PlayTimeSec)
					{
						// check gap limite over
						if(Tv_GapLimit >(m_PlayTimeSec - m_PtrDmuxOut->TimeSec))
						{
							// do wait
							Tv_Result	=	FALSE;
							break;
						}
						// update new time
						LcPlayPrgsTimeSet(m_PtrDmuxOut->TimeSec, m_PtrDmuxOut->TimeMsec);
					}
					else if((m_PtrDmuxOut->TimeSec == m_PlayTimeSec) && (m_PtrDmuxOut->TimeMsec < m_PlayTimeMili))
					{
						// wait
						Tv_Result	=	FALSE;
						break;
					}
					// do decode
					// update over time
					if(Tv_GapLimit < (m_PtrDmuxOut->TimeSec - m_PlayTimeSec))
					{
						LcPlayPrgsTimeSet(m_PtrDmuxOut->TimeSec, m_PtrDmuxOut->TimeMsec);
					}
				}

				if(LcPlayDecNowFrmBw(FALSE))
				{
					Tv_Result	=	TRUE;
					// check play run
					if(!m_IsPlayRun)
					{
						break;
					}
				}
				else
				{
					break;
				}

			}	//	if(NULL != m_PtrPlayFrm)

			// go next
			DmuxGoNext();
			m_IsPlayFrmUsed	=	FALSE;

			if((E_GrDvrPlayStatStop == m_DmuxRunStat) || (E_GrDvrPlayStatBegin == m_DmuxRunStat))
			{
				m_IsPlayRun	=	FALSE;		// do stop
				Tv_Result	=	FALSE;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::LcTmrPlayTime(void)
{
	// local -------------------
		__s32	Tv_Mili;
		__u32	Tv_Tick;
		__u32	Tv_Gap;
	// code --------------------
		if(m_IsPlayRun && (E_GrFs5PlaySyncOff != m_PlaySyncMode))
		{
			//test
			/* 
			if(E_GrDvrPlayStatPlay == m_DmuxRunStat)
			{
				// check time skip mode
				//if(E_GrFs5DmuxModeTimeJmp != m_DmuxMode)
				{
					Tv_Tick	=	GrTimeGetTick();
					Tv_Gap	=	GrTimeTickDifMs(m_PlayTickLast, Tv_Tick);
					Tv_Gap	=	Tv_Gap << m_PlaySpdShif;

					if(m_IsDmuxDirBack)
					{
						// backward
						Tv_Mili	=	(__s32)m_PlayTimeMili - (__s32)Tv_Gap;
						while(0 > Tv_Mili)
						{
							m_PlayTimeSec --;
							Tv_Mili	=	Tv_Mili + 1000;
						}
						m_PlayTimeMili	=	(__u32)Tv_Mili;
					}
					else
					{
						// forward
						Tv_Mili	=	(__s32)(Tv_Gap + m_PlayTimeMili);
						while(1000 <= Tv_Mili)
						{
							m_PlayTimeSec ++;
							Tv_Mili	=	Tv_Mili - 1000;
						}
						m_PlayTimeMili	=	(__u32)Tv_Mili;
					}
					// update tick
					m_PlayTickLast	=	Tv_Tick;
				}
			}
			// update play time
#ifdef E_GrFs5PlayTimeDirect
			PlayTimeUpdate(m_PlayTmLast);
#else
			PlayTimeUpdate(m_PlayTimeSec);
#endif*/
		}
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ImmVdoChMapSet(__u32 A_Map)
{
	// local -------------------
	// code --------------------
		// update seek map
		DmuxVdoChAbleSet(A_Map);
		// update decode power
		PlayPwrCtlUpdate();
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ImmAdoChMapSet(__u32 A_Map)
{
		DmuxAdoChAbleSet(A_Map);
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ImmAdoChSet(__s32 A_Ch)
{
	// local -------------------
		__u32	Tv_Map;
	// code --------------------
		Tv_Map	=	0;
		if(0 <= Tv_Map)
		{
			Tv_Map	=	1 << A_Ch;
			ImmAdoChMapSet(Tv_Map);
		}
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ReqVdoChMapSet(__u32 A_Map)
{
	// local -------------------
		St_GrFs5PlayMsgVdoChMapSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrFs5PlayMsgCmdVdoChMapSet;
		Tv_Msg.Map						=	A_Map;

		//m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ReqAdoChMapSet(__u32 A_Map)
{
	// local -------------------
		St_GrFs5PlayMsgAdoChMapSet	Tv_Msg;
	// code --------------------
	Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
	Tv_Msg.Base.Cmd				=	E_GrFs5PlayMsgCmdAdoChMapSet;
	Tv_Msg.Map						=	A_Map;

	//m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
	m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5Play::ImmIsPlaying(void)
{
	// local -------------------
	// code --------------------
		return	m_IsPlayRun;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5Play::ReqIsPlaying(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrFs5PlayMsgIsPlaying	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrFs5PlayMsgCmdIsPlaying;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrFs5Play::ReqPlayStatGet(void)
{
	// local -------------------
#if 0
		__u8	Tv_Result;
		St_GrFs5PlayMsgStatGet	Tv_Msg;
#endif
	// code --------------------
#if 0
		Tv_Result	=	E_GrDvrPlayStatStop;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrFs5PlayMsgCmdStatGet;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
#endif
		return	ImmPlayStatGet();
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ReqPlay(void)
{
	// local -------------------
		St_GrFs5PlayMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrFs5PlayMsgCmdPlay;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ReqPause(void)
{
	// local -------------------
		St_GrFs5PlayMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrFs5PlayMsgCmdPause;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		//m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFs5Play::ReqGoByTime(Def_GrTime A_Time)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrFs5PlayMsgGoByTime	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrErrBusy;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrFs5PlayMsgCmdGoByTime;
		Tv_Msg.PtrRslt				=	&Tv_Result;
		Tv_Msg.Time						=	A_Time;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::LcPlayPrgsTimeSet(Def_GrTime A_Time, __u32 A_Mili)
{
	// local -------------------
	// code --------------------
		//m_PlayTickLast	=	GrTimeGetTick();//test

		m_PlayTimeSec		=	A_Time;
		m_PlayTimeMili	=	A_Mili;
		
		// update time
#ifdef E_GrFs5PlayTimeDirect
		PlayTimeUpdate(m_PlayTmLast);
#else
		PlayTimeUpdate(m_PlayTimeSec);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ImmPlaySpeedSet(__s32 A_Spd)
{
	// local -------------------
	// code --------------------
		if(0 <= A_Spd)
		{
			if(5 >= A_Spd)
			{
				m_PlaySpdShif	=	(__u32)A_Spd;

				// update speed control
				PlayPwrCtlUpdate();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::LcPlayPwrChUpdate(__u8 A_Ch)
{
	// local -------------------
		BOOL8	Tv_IsIntl;
		__u32	Tv_ResX;
		__u32	Tv_ResY;
	// code --------------------
		// calculate channel power
		GrDvrVdoFmtInfoGet(m_DmuxChTbl[A_Ch].VdoFmt, &Tv_ResX, &Tv_ResY, &Tv_IsIntl);
		m_PlayChTbl[A_Ch].IfPwr	=	((Tv_ResX * Tv_ResY) >> E_GrFs5PlayPwrResShift);
		m_PlayChTbl[A_Ch].NsPwr	=	m_PlayChTbl[A_Ch].IfPwr * (__u32)m_DmuxChTbl[A_Ch].VdoFps;

		//DbgMsgPrint("[PLAY] ch %d resx %d resy %d fps %d\n", A_Ch, Tv_ResX, Tv_ResY, m_DmuxChTbl[A_Ch].VdoFps);
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::PlayPwrCtlUpdate(void)
{

}
//--------------------------------------------------------------------
__s32	Cls_GrFs5Play::ImmPlaySpeedGet(void)
{
		return	m_PlaySpdShif;
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ImmPlayDirSet(BOOL8 A_IsBack)
{
		DmuxDirSet(A_IsBack);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5Play::ImmPlayDirGet(void)
{
		return	m_IsDmuxDirBack;
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ReqPlaySpeedSet(__s32 A_Spd)
{
	// local -------------------
		St_GrFs5PlayMsgSpeedSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrFs5PlayMsgCmdSpeedSet;
		Tv_Msg.Val						=	A_Spd;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
__s32	Cls_GrFs5Play::ReqPlaySpeedGet(void)
{
	// local -------------------
		__s32	Tv_Result;
		St_GrFs5PlayMsgSpeedGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrFs5PlayMsgCmdSpeedGet;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ReqPlayDirSet(BOOL8 A_IsBack)
{
	// local -------------------
		St_GrFs5PlayMsgDirSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrFs5PlayMsgCmdDirSet;
		Tv_Msg.IsBack					=	A_IsBack;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5Play::ReqPlayDirGet(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrFs5PlayMsgDirGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrFs5PlayMsgCmdDirGet;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
void Cls_GrFs5Play::ReqAdoChSet(__s32 A_AdoCh)
{
	// local -------------------
		St_GrFs5PlayMsgAdoChSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrFs5PlayMsgCmdAdoChSet;
		Tv_Msg.AdoCh					=	A_AdoCh;

		m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ImmStepForward(void)
{
	// local -------------------
		__u32	Tv_TryCnt;
	// code --------------------
		// check seek mode
		if((E_GrDvrPlayStatSeek == m_DmuxRunStat) || (E_GrDvrPlayStatStop == m_DmuxRunStat))
		{
			return;
		}

		// check now play mode
		if(m_IsPlayRun)
		{
			// stop
			m_IsPlayRun	=	FALSE;
			if(m_IsDmuxDirBack)
			{
				ImmPlayDirSet(FALSE);
			}

			// flush now frame
			LcPlayNowFrmFlush();
			return;
		}

		if(m_IsDmuxDirBack)
		{
			ImmPlayDirSet(FALSE);
			LcPlayNowFrmFlush();
			return;
		}

		if(0 != m_PlaySpdShif)
		{
			ImmPlaySpeedSet(0);
		}

		// do step play
		if((NULL == m_PtrDmuxOut) || m_IsPlayFrmUsed)
		{
			m_IsPlayFrmUsed	=	FALSE;

			for( Tv_TryCnt = 0; Tv_TryCnt < m_PlayTryCnt; Tv_TryCnt++)
			{
				DmuxGoNext();
				// check play status
				if(E_GrDvrPlayStatStop == m_DmuxRunStat)
				{
					return;
				}
				if(NULL != m_PtrDmuxOut)
				{
					break;
				}
			}
		}

		// decode frame
		LcPlayNowFrmFlush();

}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ImmStepBackward(void)
{
	// local -------------------
		__u32	Tv_TryCnt;
	// code --------------------
		// check seek mode
		if((E_GrDvrPlayStatSeek == m_DmuxRunStat) || (E_GrDvrPlayStatStop == m_DmuxRunStat))
		{
			return;
		}

		// check now play mode
		if(m_IsPlayRun)
		{
			// stop
			m_IsPlayRun	=	FALSE;
			if(!m_IsDmuxDirBack)
			{
				ImmPlayDirSet(TRUE);
			}

			// flush now frame
			LcPlayNowFrmFlush();
			return;
		}

		if(!m_IsDmuxDirBack)
		{
			ImmPlayDirSet(TRUE);
			LcPlayNowFrmFlush();
			return;
		}

		if(0 != m_PlaySpdShif)
		{
			ImmPlaySpeedSet(0);
		}

		// check play status
		if((E_GrDvrPlayStatStop == m_DmuxRunStat) || (E_GrDvrPlayStatBegin == m_DmuxRunStat))
		{
			return;
		}

		// do step play
		if((NULL == m_PtrDmuxOut) || m_IsPlayFrmUsed)
		{
			m_IsPlayFrmUsed	=	FALSE;

			for(Tv_TryCnt = 0; Tv_TryCnt < m_PlayTryCnt; Tv_TryCnt++)
			{
				DmuxGoNext();
				// check play status
				if((E_GrDvrPlayStatStop == m_DmuxRunStat) || (E_GrDvrPlayStatBegin == m_DmuxRunStat))
				{
					return;
				}
				if(NULL != m_PtrDmuxOut)
				{
					break;
				}
			}
		}

		// decode frame
		LcPlayNowFrmFlush();

}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ReqStepForward(void)
{
	// local -------------------
		St_GrFs5PlayMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrFs5PlayMsgCmdStepForward;

		m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ReqStepBackward(void)
{
	// local -------------------
		St_GrFs5PlayMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrFs5PlayMsgCmdStepBackward;

		m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::LoadBefore(void)
{
#if 0 //test
		m_PlayTickBefore	=	GrTimeGetTick();
		//DbgMsgPrint("[Play load Before!]\n");
#endif
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::LoadAfter(void)
{
	// local -------------------
		__u32	Tv_Tick;
	// code --------------------
#if 0 //test
		Tv_Tick	=	GrTimeGetTick();

		// patch play tick
		m_PlayTickLast	=	m_PlayTickLast + (Tv_Tick - m_PlayTickBefore);
		//DbgMsgPrint("[Play loaded size = %d prgs = %d After!]\n", m_DmuxSctl.Readed, m_DmuxSctl.Prgs);
#endif
}
//--------------------------------------------------------------------
__u8	Cls_GrFs5Play::ImmPlayStatGet(void)
{
	// local -------------------
	// code --------------------
		return	m_DmuxRunStat;
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::PlayTimeUpdate(Def_GrTime A_Time)
{

}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ImmPlaySyncSet(__u8 A_Mode)
{
		m_PlaySyncMode	=	A_Mode;
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::ReqPlaySyncSet(__u8 A_Mode)
{
	// local -------------------
		St_GrFs5PlayMsgSyncSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrFs5PlayMsgCmdSyncSet;
		Tv_Msg.Mode						=	A_Mode;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::LcPlayNowFrmFlush(void)
{
	// local -------------------
	// code --------------------
		if((NULL != m_PtrDmuxOut) && (!m_IsPlayFrmUsed))
		{
			if(m_IsDmuxDirBack)
			{
				LcPlayDecNowFrmBw(TRUE);
			}
			else
			{
				LcPlayDecNowFrmFw(TRUE);
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5Play::LcSubPlayS1Fw(void)
{
	// local -------------------
	// code --------------------
		// 
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrFs5Play::LcPlayTimeGet(void)
{
	// local -------------------
		__s32	Tv_Mili;
		__u32	Tv_Tick;
		__u32	Tv_Gap;
	// code --------------------
		// check time skip mode
		Tv_Tick	=	GrTimeGetTick();
		Tv_Gap	=	GrTimeTickDifMs(m_PlayTickTest, Tv_Tick);
		Tv_Gap	=	Tv_Gap << m_PlaySpdShif;

		if(m_IsDmuxDirBack)
		{
			// backward
			Tv_Mili	=	(__s32)m_PlayTimeMili - (__s32)Tv_Gap;
			while(0 > Tv_Mili)
			{
				m_PlayTimeSec --;
				Tv_Mili	=	Tv_Mili + 1000;
			}
			m_PlayTimeMili	=	(__u32)Tv_Mili;
		}
		else
		{
			// forward
			Tv_Mili	=	(__s32)(Tv_Gap + m_PlayTimeMili);
			while(1000 <= Tv_Mili)
			{
				m_PlayTimeSec ++;
				Tv_Mili	=	Tv_Mili - 1000;
			}
			m_PlayTimeMili	=	(__u32)Tv_Mili;
		}
		// update tick
		m_PlayTickTest	=	Tv_Tick;

		// update play time
		PlayTimeUpdate(m_PlayTimeSec);
}
//--------------------------------------------------------------------

