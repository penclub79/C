/*
 GAUSE file system version 4 Play

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include <Dvr/Fs/V4/GrDvrFs4Play.h>

#include	<stdlib.h>

//====================================================================
//local const

#define	E_GrDvrFs4PlayDfltSpdMin		0
#define	E_GrDvrFs4PlayDfltSpdMax		7
#define	E_GrDvrFs4PlayDfltSpdCnt		8

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs4Play::Cls_GrDvrFs4Play( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFs4Mng* A_FsMng, BOOL8 A_IsSafe ):
Cls_GrDvrFs4Dmux( A_TaskSvr, A_FsMng, A_IsSafe )
{
	// local -------------------
	// code --------------------
		// init
		m_PlayMode			=	E_GrDvrFs4PlayModeWait;

		GrBufQueInit( (Ptr_GrBufQue)m_PlayCmdQue, sizeof(St_GrDvrFs4PlayCmdItem), sizeof(m_PlayCmdQue) );

		m_PlayStat			=	E_GrDvrPlayStatStop;
		m_IsPlayDirBack	=	FALSE;

		m_PlaySpdMin		=	E_GrDvrFs4PlayDfltSpdMin;
		m_PlaySpdMax		=	E_GrDvrFs4PlayDfltSpdMax;

		m_PlayVdoMap		=	0;
		m_PlayAdoMap		=	0;

		m_IsPlaying			=	FALSE;

		m_PlayTmSec			=	0;
		m_PlayTmMsec		=	0;
		m_IsPlayTmSm		=	FALSE;
		m_IsPlayTmSync	=	FALSE;
		m_IsPlayNoWait	=	FALSE;

		GrDumyZeroMem(m_PlayChTbl, sizeof(m_PlayChTbl));

		// inherited

		// register
		m_TaskSvr->RegistTimerClient(this, E_GrDvrFs4PlayTimerPeriod, &m_HndlTimer);
		m_TaskSvr->RegistBkgClient(this);
}
//--------------------------------------------------------------------
Cls_GrDvrFs4Play::~Cls_GrDvrFs4Play()
{

		// unregister
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistTimerClient(m_HndlTimer);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		switch ( m_PlayMode )
		{
			case E_GrDvrFs4PlayModeJmpTime:
				Tv_Result	=	LcPlayModeJmpTime();
				break;
			case E_GrDvrFs4PlayModeSeek:
				Tv_Result	=	LcPlayModeSeek();
				break;
			case E_GrDvrFs4PlayModePlay:
				Tv_Result	=	LcPlayModePlay();
				break;
			case E_GrDvrFs4PlayModeNext:
				Tv_Result	=	LcPlayModeNext();
				break;
			case E_GrDvrFs4PlayModePrev:
				Tv_Result	=	LcPlayModePrev();
				break;
			case E_GrDvrFs4PlayModeVdec:
				Tv_Result	=	LcPlayModeVdec();
				break;
			case E_GrDvrFs4PlayModeJmpBegin:
				Tv_Result	=	LcPlayModeJmpBegin();
				break;
			case E_GrDvrFs4PlayModeJmpEnd:
				Tv_Result	=	LcPlayModeJmpEnd();
				break;
			default:
				Tv_Result	=	LcPlayModeWait();
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::RtlTimer(__u32 A_Hndl)
{
	// local -------------------
	// code --------------------
	LcPlayTimerTmUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayTimerTmUpdt(void)
{
	// local -------------------
		__u32	Tv_GapSec;
		__u32	Tv_GapMsec;
		__u32	Tv_TickNow;
		__u32	Tv_TickGap;
		__u32	Tv_MsGap;
	// code --------------------
		// update play time
		if (0 != m_PlayTmSec)
		{
			if ((!m_IsPlayTmSync) && (!m_IsPlayNoWait))
			{
				Tv_GapSec		=	0;
				Tv_GapMsec	=	0;
				// get time gap
				Tv_TickNow	=	GrTimeGetTick();
				Tv_TickGap	=	Tv_TickNow - m_PlayTick;
				if (0 < m_PlaySpeed)
				{
					Tv_TickGap	=	Tv_TickGap << m_PlaySpeed;
				}
				else if (0 > m_PlaySpeed)
				{
					Tv_TickGap	=	Tv_TickGap >> (-m_PlaySpeed);
				}
				// check slow able
				Tv_MsGap		=	GrTimeTickToMs(Tv_TickGap);
				if (0 != Tv_MsGap)
				{
					Tv_GapSec		=	Tv_MsGap / E_GrTimeMiliOfSec;
					Tv_GapMsec	=	Tv_MsGap - (Tv_GapSec * E_GrTimeMiliOfSec);
					m_PlayTick	=	Tv_TickNow;	// update last tick
				}

				// update time by direction
				if (m_IsPlayDirBack)
				{
					if (Tv_GapMsec > m_PlayTmMsec)
					{
						m_PlayTmSec --;
						m_PlayTmMsec	=	(m_PlayTmMsec + E_GrTimeMiliOfSec) - Tv_GapMsec;
					}
					else
					{
						m_PlayTmMsec	=	m_PlayTmMsec - Tv_GapMsec;
					}
					m_PlayTmSec	=	m_PlayTmSec - Tv_GapSec;
				}
				else
				{
					if ((Tv_GapMsec + m_PlayTmMsec) >= E_GrTimeMiliOfSec )
					{
						m_PlayTmSec ++;
						m_PlayTmMsec	=	(m_PlayTmMsec + Tv_GapMsec) - E_GrTimeMiliOfSec;
					}
					else
					{
						m_PlayTmMsec	=	m_PlayTmMsec + Tv_GapMsec;
					}
					m_PlayTmSec	=	m_PlayTmSec + Tv_GapSec;
				}
			}	//	if ((!m_IsPlayTmSync) && (!m_IsPlayNoWait))
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::LcPlayModeWait(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	LcPlayPumpCmd();
		// status update
		if ( !Tv_Result )
		{
			if ( m_IsPlaying )
			{
				LcPlayCmdPlay();
				Tv_Result	=	TRUE;
			}
			else
			{
				m_PlayStat	=	E_GrDvrPlayStatStop;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::LcPlayModePlay(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		// init
		Tv_Result	=	TRUE;
		// parsing command
		if (!LcPlayPumpCmd())
		{
			// init
			Tv_Result		=	FALSE;

			// check direction
			if ( m_IsPlayDirBack )
			{
				Tv_Rval	=	LcPlayRunBw();
			}	//	if ( m_IsDirBack )
			else
			{
				Tv_Rval	=	LcPlayRunFw();
			}	

			if (E_GrDvrFs4PlayRsltOk == Tv_Rval)
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result; 
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::PlayGoTime(Def_GrTime A_Time)
{
	// local -------------------
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PlayCmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd	=	E_GrDvrFs4PlayCmdJmpByTime;
			Tv_PtrCmd->Dw		=	A_Time;
			GrBufQueIn( (Ptr_GrBufQue)m_PlayCmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs4Play::PlayGoTime - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::PlayDo(void)
{
	// local -------------------
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PlayCmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs4PlayCmdDoPlay;
			GrBufQueIn( (Ptr_GrBufQue)m_PlayCmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs4Play::PlayDo - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::PlayDevTypeSet(__u8 A_DevType)
{
	// local -------------------
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PlayCmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs4PlayCmdDevSet;
			Tv_PtrCmd->Bt		=	A_DevType;
			GrBufQueIn( (Ptr_GrBufQue)m_PlayCmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs4Play::PlayDevTypeSet - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::PlayStop(void)
{
	// local -------------------
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PlayCmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs4PlayCmdDoStop;
			GrBufQueIn( (Ptr_GrBufQue)m_PlayCmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs4Play::PlayStop - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::PlayStepNext(void)
{
	// local -------------------
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PlayCmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs4PlayCmdStepNext;
			GrBufQueIn( (Ptr_GrBufQue)m_PlayCmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs4Play::PlayStepNext - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::PlayStepPrev(void)
{
	// local -------------------
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PlayCmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs4PlayCmdStepPrev;
			GrBufQueIn( (Ptr_GrBufQue)m_PlayCmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs4Play::PlayStepPrev - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::PlayVideoChEnable(__u32 A_Map)
{
	// local -------------------
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetInPtr((Ptr_GrBufQue)m_PlayCmdQue);
		if (NULL != Tv_PtrCmd)
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs4PlayCmdVdoChSet;
			Tv_PtrCmd->Dw		=	A_Map;
			GrBufQueIn((Ptr_GrBufQue)m_PlayCmdQue);
		}
		else
		{
			DbgMsgPrint("Cls_GrDvrFs4Play::PlayVideoChEnable - command queue is full!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::PlayAudioChEnable(__u32 A_Map)
{
	// local -------------------
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetInPtr((Ptr_GrBufQue)m_PlayCmdQue);
		if (NULL != Tv_PtrCmd)
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs4PlayCmdAdoChSet;
			Tv_PtrCmd->Dw		=	A_Map;
			GrBufQueIn((Ptr_GrBufQue)m_PlayCmdQue);
		}
		else
		{
			DbgMsgPrint("Cls_GrDvrFs4Play::PlayVideoChEnable - command queue is full!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::PlayDirSet( BOOL8 A_IsBack )
{
	// local -------------------
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PlayCmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs4PlayCmdDirSet;
			Tv_PtrCmd->Bt			=	(__u8)A_IsBack;
			GrBufQueIn( (Ptr_GrBufQue)m_PlayCmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs4Play::PlayDirSet - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::PlaySpeedSet( __s32 A_Speed )
{
	// local -------------------
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PlayCmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs4PlayCmdSpeedSet;
			Tv_PtrCmd->Dw			=	(__u32)A_Speed;
			GrBufQueIn( (Ptr_GrBufQue)m_PlayCmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs4Play::PlaySpeedSet - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::PlayGoBegin(void)
{
	// local -------------------
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PlayCmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs4PlayCmdJmpBegin;
			GrBufQueIn( (Ptr_GrBufQue)m_PlayCmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs4Play::JumpBegin - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::PlayGoEnd(void)
{
	// local -------------------
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_PlayCmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs4PlayCmdJmpEnd;
			GrBufQueIn( (Ptr_GrBufQue)m_PlayCmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs4Play::JumpEnd - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Play::PlayStatusGet( void )
{
		return	m_PlayStat;
}
//--------------------------------------------------------------------
__s32		Cls_GrDvrFs4Play::PlaySpeedGet( void )
{
		return	m_PlaySpeed;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs4Play::PlayTimeGet( void )
{
		//return	m_PlaySec;
		return	m_PlayTmSec;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::IsPlayTimeSm(void)
{
		return	m_IsPlayTmSm;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::IsPlayDirBack(void)
{
		return	m_IsPlayDirBack;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Play::PlayDevTypeGet(void)
{
		return	m_DmuxDevType;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::LcPlayPumpCmd(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDvrFs4PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_Result	=	FALSE;
		// check command queue
		Tv_PtrCmd	=	(Ptr_GrDvrFs4PlayCmdItem)GrBufQueGetOutPtr( (Ptr_GrBufQue)m_PlayCmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			switch ( Tv_PtrCmd->Cmd )
			{
				case E_GrDvrFs4PlayCmdDoPlay:
					LcPlayCmdPlay();
					break;
				case E_GrDvrFs4PlayCmdJmpByTime:
					LcPlayCmdJumpByTime(Tv_PtrCmd->Dw);
					break;
				case E_GrDvrFs4PlayCmdJmpBegin:
					LcPlayCmdJmpBegin();
					break;
				case E_GrDvrFs4PlayCmdJmpEnd:
					LcPlayCmdJmpEnd();
					break;
				case E_GrDvrFs4PlayCmdDirSet:
					LcPlayCmdDirSet( (BOOL8)Tv_PtrCmd->Bt );
					break;
				case E_GrDvrFs4PlayCmdVdoChSet:
					LcPlayCmdVdoChSet(Tv_PtrCmd->Dw);
					break;
				case E_GrDvrFs4PlayCmdAdoChSet:
					LcPlayCmdAdoChSet(Tv_PtrCmd->Dw);
					break;
				case E_GrDvrFs4PlayCmdSpeedSet:
					LcPlayCmdSpeedSet((__s32)Tv_PtrCmd->Dw);
					break;
				case E_GrDvrFs4PlayCmdStepNext:
					LcPlayCmdStepNext();
					break;
				case E_GrDvrFs4PlayCmdStepPrev:
					LcPlayCmdStepPrev();
					break;
				case E_GrDvrFs4PlayCmdDevSet:
					LcPlayCmdDevSet(Tv_PtrCmd->Bt);
					break;
				case E_GrDvrFs4PlayCmdDoStop:
					LcPlayCmdStop();
					break;

			}
			// release command
			GrBufQueOut( (Ptr_GrBufQue)m_PlayCmdQue );
			// command exist
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayCmdPlay(void)
{
	// local -------------------
	// code --------------------
		if (!m_IsPlaying)
		{
			m_PlayMode			=	E_GrDvrFs4PlayModePlay;
			m_IsPlaying			=	TRUE;
			m_IsPlayTmSync	=	TRUE;

			// update dmux mode
			LcPlayDmuxModeUpdt();
			LcPlayDispSkipAllUpdt();
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayCmdJumpByTime(Def_GrTime A_Time)
{
	// local -------------------
	// code --------------------
		m_PlaySeekTgTime	=	A_Time;
		m_PlayMode				=	E_GrDvrFs4PlayModeJmpTime;
		m_IsPlayTmSync		=	TRUE;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Play::DecVdo(Ptr_GrDvrFrmVdo A_PtrHd, BOOL8 A_IsDisp)
{
		return	E_GrDvrFs4PlayRsltFail;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Play::DecAdo(Ptr_GrDvrFrmAdo A_PtrHd)
{
		return	E_GrDvrFs4PlayRsltFail;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Play::LcPlayRunFw(void)
{
	// local -------------------
		__u8	Tv_Result;
		BOOL8	Tv_IsDisp;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4PlayRsltWait;

		if (NULL != m_PtrDmuxFrm)
		{
			// check play time
			if ((0 == m_PlayTmSec) || m_IsPlayTmSync || m_IsPlayNoWait)
			{
				// get first time
				m_IsPlayTmSync	=	TRUE;
				Tv_Result	=	E_GrDvrFs4PlayRsltOk;
			}
			else
			{
				// calculate gap
				if ((m_PlayTmSec > m_PtrDmuxFrm->TimeSec) || ((m_PlayTmSec == m_PtrDmuxFrm->TimeSec) && (m_PlayTmMsec >= m_PtrDmuxFrm->TimeMsec)))
				{
					Tv_Result	=	E_GrDvrFs4PlayRsltOk;
				}
				else if (E_GrDvrFs4PlayTimeGapLimite <= (m_PtrDmuxFrm->TimeSec - m_PlayTmSec))
				{
					m_IsPlayTmSync	=	TRUE;
					Tv_Result	=	E_GrDvrFs4PlayRsltOk;
				}
			}

			// play time sync
			if (m_IsPlayTmSync)
			{
				m_PlayTmSec			=	m_PtrDmuxFrm->TimeSec;
				m_PlayTmMsec		=	m_PtrDmuxFrm->TimeMsec;
				m_PlayTick			=	GrTimeGetTick();
				m_IsPlayTmSync	=	FALSE;
			}
			// update summer time
			m_IsPlayTmSm		=	m_PtrDmuxFrm->IsSm;

			// check cane decode
			if (E_GrDvrFs4PlayRsltOk == Tv_Result)
			{
				// check type
				if ((E_GrDvrFrmTypeVdoI <= m_PtrDmuxFrm->Type) && (E_GrDvrFrmTypeVdoB >= m_PtrDmuxFrm->Type))
				{
					// video
					// check can display
					Tv_IsDisp	=	TRUE;
					if (0 != m_PlayChTbl[m_PtrDmuxFrm->Ch].DispPrgs)
					{
						Tv_IsDisp	=	FALSE;
					}
					// next disp
					m_PlayChTbl[m_PtrDmuxFrm->Ch].DispPrgs ++;
					if (m_PlayChTbl[m_PtrDmuxFrm->Ch].DispPrgs > m_PlayChTbl[m_PtrDmuxFrm->Ch].DispStep)
					{
						m_PlayChTbl[m_PtrDmuxFrm->Ch].DispPrgs		=	0;
					}
					// decode
					Tv_Result	=	DecVdo((Ptr_GrDvrFrmVdo)m_PtrDmuxFrm, Tv_IsDisp);
				}
				else if (E_GrDvrFrmTypeAdo == m_PtrDmuxFrm->Type)
				{
					// audio
					Tv_Result	=	DecAdo((Ptr_GrDvrFrmAdo)m_PtrDmuxFrm);
				}
				/*
					you can implement power search here!
				*/
			}
		}
		// go next
		if ((NULL == m_PtrDmuxFrm) || (E_GrDvrFs4PlayRsltOk == Tv_Result))
		{
			Tv_Rval	=	DmuxGoFw();
			if (E_GrDvrFs4DmuxRsltBusy == Tv_Rval)
			{
				Tv_Result	=	E_GrDvrFs4PlayRsltBusy;
			}
			else if ((E_GrDvrFs4DmuxRsltDataEmpty == Tv_Rval) || (E_GrDvrFs4DmuxRsltEndOfData == Tv_Rval))
			{
				// arrival end position
				m_PlayStat	=	E_GrDvrPlayStatEnd;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Play::LcPlayRunBw(void)
{
	// local -------------------
		__u8	Tv_Result;
		BOOL8	Tv_IsDisp;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4PlayRsltWait;

		if (NULL != m_PtrDmuxFrm)
		{
			// check play time
			if ((0 == m_PlayTmSec) || m_IsPlayTmSync || m_IsPlayNoWait)
			{
				// get first time
				m_IsPlayTmSync	=	TRUE;
				Tv_Result	=	E_GrDvrFs4PlayRsltOk;
			}
			else
			{
				// calculate gap
				if ((m_PlayTmSec < m_PtrDmuxFrm->TimeSec) || ((m_PlayTmSec == m_PtrDmuxFrm->TimeSec) && (m_PlayTmMsec <= m_PtrDmuxFrm->TimeMsec)))
				{
					Tv_Result	=	E_GrDvrFs4PlayRsltOk;
				}
				else if (E_GrDvrFs4PlayTimeGapLimite <= (m_PlayTmSec - m_PtrDmuxFrm->TimeSec))
				{
					m_IsPlayTmSync	=	TRUE;
					Tv_Result	=	E_GrDvrFs4PlayRsltOk;
				}
			}

			// play time sync
			if (m_IsPlayTmSync)
			{
				m_PlayTmSec			=	m_PtrDmuxFrm->TimeSec;
				m_PlayTmMsec		=	m_PtrDmuxFrm->TimeMsec;
				m_PlayTick			=	GrTimeGetTick();
				m_IsPlayTmSync	=	FALSE;
			}
			// update summer time
			m_IsPlayTmSm		=	m_PtrDmuxFrm->IsSm;

			// check cane decode
			if (E_GrDvrFs4PlayRsltOk == Tv_Result)
			{
				// check type
				if (E_GrDvrFrmTypeVdoI == m_PtrDmuxFrm->Type)
				{
					// video
					// check can display
					Tv_IsDisp	=	TRUE;
					if (0 != m_PlayChTbl[m_PtrDmuxFrm->Ch].DispPrgs)
					{
						Tv_IsDisp	=	FALSE;
					}
					// next disp
					m_PlayChTbl[m_PtrDmuxFrm->Ch].DispPrgs ++;
					if (m_PlayChTbl[m_PtrDmuxFrm->Ch].DispPrgs > m_PlayChTbl[m_PtrDmuxFrm->Ch].DispStep)
					{
						m_PlayChTbl[m_PtrDmuxFrm->Ch].DispPrgs		=	0;
					}
					// decode
					Tv_Result	=	DecVdo((Ptr_GrDvrFrmVdo)m_PtrDmuxFrm, Tv_IsDisp);
				}
				/*
				you can implement power search here!
				*/
			}
		}
		// go next
		if ((NULL == m_PtrDmuxFrm) || (E_GrDvrFs4PlayRsltOk == Tv_Result))
		{
			Tv_Rval	=	DmuxGoBw();
			if (E_GrDvrFs4DmuxRsltBusy == Tv_Rval)
			{
				Tv_Result	=	E_GrDvrFs4PlayRsltBusy;
			}
			else if ((E_GrDvrFs4DmuxRsltDataEmpty == Tv_Rval) || (E_GrDvrFs4DmuxRsltEndOfData == Tv_Rval))
			{
				// arrival end position
				m_PlayStat	=	E_GrDvrPlayStatBegin;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs4Play::LcPlayCalcPowerByDmuxMode(__u8 A_DmuxMode)
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_WkCh;
		BOOL8	Tv_IsIntl;
		__u32	Tv_ResX;
		__u32	Tv_ResY;
		__u32	Tv_PwrCh;
	// code --------------------
		Tv_Result	=	0;

		for (Tv_WkCh=0; Tv_WkCh < E_GrDvrMaxChCnt;Tv_WkCh++)
		{
			// check able channel
			if (m_DmuxChTbl[Tv_WkCh].IsVdoAble)
			{
				// calculate channel power
				GrDvrVdoFmtInfoGet(m_DmuxChTbl[Tv_WkCh].VdoFmt, &Tv_ResX, &Tv_ResY, &Tv_IsIntl);
				Tv_PwrCh	=	Tv_ResX * Tv_ResY;
				if (Tv_IsIntl)
				{
					Tv_PwrCh	=	Tv_PwrCh >> 1;
				}

				Tv_PwrCh		=	(Tv_PwrCh * (__u32)m_DmuxChTbl[Tv_WkCh].VdoSigFps) / (__u32)(m_DmuxChTbl[Tv_WkCh].VdoSkipCnt + 1);

				if (E_GrDvrFs4DmuxModeAll != A_DmuxMode)
				{
					// interlace
					if (0 != (E_GrDvrVdoFmtResInterlace & m_DmuxChTbl[Tv_WkCh].VdoFmt))
					{
						Tv_PwrCh	=	Tv_PwrCh >> 1;
					}
					// ref 2x
					if (2 == m_DmuxChTbl[Tv_WkCh].VdoRefType)
					{
						if (E_GrDvrFs4DmuxModeRef2x <= A_DmuxMode)
						{
							Tv_PwrCh	=	Tv_PwrCh >> 1;
						}
					}
					// ref 4x
					if (4 == m_DmuxChTbl[Tv_WkCh].VdoRefType)
					{
						if (E_GrDvrFs4DmuxModeRef2x == A_DmuxMode)
						{
							Tv_PwrCh	=	Tv_PwrCh >> 1;
						}
						else if (E_GrDvrFs4DmuxModeRef4x == A_DmuxMode)
						{
							Tv_PwrCh	=	Tv_PwrCh >> 2;
						}
					}
				}

				// update power
				Tv_Result	=	Tv_Result + Tv_PwrCh;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::EvtVdoFmtChg(__u8 A_Ch)
{
	// local -------------------
	// code --------------------
		LcPlayDispSkipChUpdt(A_Ch);
		LcPlayDmuxModeUpdt();

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayDmuxModeUpdt(void)
{
	// local -------------------
		__u8	Tv_WkMode;
		__u8	Tv_DmMode;
		__u32	Tv_Pwr;
		__u32	Tv_AdoMap;
		__u32	Tv_FrmSkipCnt;
	// code --------------------
		// calculate power
		Tv_DmMode		=	E_GrDvrFs4DmuxModeCntSkip;
		Tv_FrmSkipCnt	=	0;
		Tv_AdoMap			=	m_PlayAdoMap;

		Tv_WkMode	=	E_GrDvrFs4DmuxModeAll;
		if (m_IsPlayDirBack)
		{
			Tv_WkMode	=	E_GrDvrFs4DmuxModeCntSkip;
		}
		while (Tv_WkMode < E_GrDvrFs4DmuxModeCntSkip)
		{
			Tv_Pwr	=	LcPlayCalcPowerByDmuxMode(Tv_WkMode);
			if (0 < m_PlaySpeed)
			{
				Tv_Pwr	=	Tv_Pwr << (__u32)m_PlaySpeed;
			}
			else if (0 > m_PlaySpeed)
			{
				Tv_Pwr	=	Tv_Pwr >> (__u32)(-m_PlaySpeed);
			}
			if (m_PlayPower >= Tv_Pwr)
			{
				Tv_DmMode	=	Tv_WkMode;
				break;
			}
			// next
			Tv_WkMode ++;
		}

		if (E_GrDvrFs4DmuxModeCntSkip == Tv_DmMode)
		{
			// calculate channel skip count
			Tv_Pwr				=	LcPlayCalcPowerByDmuxMode(E_GrDvrFs4DmuxModeAll);
			if (Tv_Pwr > m_PlayPower)
			{
				Tv_FrmSkipCnt	=	(Tv_Pwr / m_PlayPower);

				if (0 != (Tv_Pwr % m_PlayPower))
				{
					Tv_FrmSkipCnt ++;
				}
			}
		}

		if (m_IsPlayDirBack || (0 != m_PlaySpeed))
		{
			Tv_AdoMap	=	0;
		}

		// update demux mode
		DmuxModeSet(Tv_DmMode,0,Tv_FrmSkipCnt);
		DmuxAdoAbleMapSet(Tv_AdoMap);
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayDispSkipChUpdt(__u8 A_Ch)
{
	// local -------------------
		__u32	Tv_ChFps;
		__u32	Tv_Tmp;
		__u8	Tv_DispStep;
	// code --------------------
		// check able channel
		if (m_DmuxChTbl[A_Ch].IsVdoAble && (0 != m_DmuxChTbl[A_Ch].VdoSigFps))
		{
			// get channel speed
			Tv_ChFps	=	(__u32)m_DmuxChTbl[A_Ch].VdoSigFps;

			if (0 < m_PlaySpeed)
			{
				Tv_ChFps	=	(Tv_ChFps << (__u32)m_PlaySpeed) / (__u32)(m_DmuxChTbl[A_Ch].VdoSkipCnt + 1);
			}
			else if (0 > m_PlaySpeed)
			{
				Tv_ChFps	=	(Tv_ChFps / (__u32)(m_DmuxChTbl[A_Ch].VdoSkipCnt + 1)) >> (__u32)(-m_PlaySpeed);
			}

			if (E_GrDvrFs4DmuxModeCntSkip == m_DmuxMode)
			{
				if (0 != m_DmuxSkipCnt)
				{
					Tv_Tmp		=	Tv_ChFps;
					Tv_ChFps	=	Tv_ChFps / m_DmuxSkipCnt;
					if (0 != (Tv_Tmp % m_DmuxSkipCnt))
					{
						Tv_ChFps ++;
					}
				}
			}
			else if (E_GrDvrFs4DmuxModeAll != m_DmuxMode)
			{
				// interlace
				if (0 != (E_GrDvrVdoFmtResInterlace & m_DmuxChTbl[A_Ch].VdoFmt))
				{
					Tv_ChFps	=	Tv_ChFps >> 1;
				}
				// ref 2x
				if (2 == m_DmuxChTbl[A_Ch].VdoRefType)
				{
					if (E_GrDvrFs4DmuxModeRef2x <= m_DmuxMode)
					{
						Tv_ChFps	=	Tv_ChFps >> 1;
					}
				}
				// ref 4x
				if (4 == m_DmuxChTbl[A_Ch].VdoRefType)
				{
					if (E_GrDvrFs4DmuxModeRef2x == m_DmuxMode)
					{
						Tv_ChFps	=	Tv_ChFps >> 1;
					}
					else if (E_GrDvrFs4DmuxModeRef4x == m_DmuxMode)
					{
						Tv_ChFps	=	Tv_ChFps >> 2;
					}
				}
			}

			Tv_DispStep	=	0;
			if (Tv_ChFps > (__u32)m_PlayDispFps)
			{
				Tv_DispStep	=	(__u8)(Tv_ChFps / (__u32)m_PlayDispFps);
				if (0 != (Tv_ChFps % (__u32)m_PlayDispFps))
				{
					Tv_DispStep ++;
				}
			}
			m_PlayChTbl[A_Ch].DispStep	=	Tv_DispStep;	// update

		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayDispSkipAllUpdt(void)
{
	// local -------------------
		__u8	Tv_WkCh;
	// code --------------------
		for (Tv_WkCh=0; Tv_WkCh < E_GrDvrMaxChCnt;Tv_WkCh++)
		{
			LcPlayDispSkipChUpdt(Tv_WkCh);
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::LcPlayModeJmpTime(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;

		if (m_IsPlayDirBack)
		{
			Tv_Rval	=	DmuxGoTimeBw(m_PlaySeekTgTime);
		}
		else
		{
			Tv_Rval	=	DmuxGoTimeFw(m_PlaySeekTgTime);
		}

		if (E_GrDvrFs4DmuxRsltOk == Tv_Rval)
		{
			m_PlayMode			=	E_GrDvrFs4PlayModeSeek;
			m_PlaySeekPrgs	=	0;
			Tv_Result				=	TRUE;
		}
		else if ((E_GrDvrFs4DmuxRsltEndOfData == Tv_Rval) || (E_GrDvrFs4DmuxRsltDataEmpty == Tv_Rval))
		{
			m_PlayMode	=	E_GrDvrFs4PlayModeWait;
			m_IsPlaying	=	FALSE;
			if (m_IsPlayDirBack)
			{
				m_PlayStat	=	E_GrDvrPlayStatBegin;
			}
			else
			{
				m_PlayStat	=	E_GrDvrPlayStatEnd;
			}

			Tv_Result		=	TRUE;
		}
		else
		{
			m_PlayStat	=	E_GrDvrPlayStatSeek;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::LcPlayModeSeek(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_SeekFrmCnt;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result		=	TRUE;
		m_PlayStat	=	E_GrDvrPlayStatSeek;

		for (Tv_SeekFrmCnt=0; Tv_SeekFrmCnt <E_GrDvrFs4PlaySeekWorkUnit;Tv_SeekFrmCnt++)
		{
			// get frame info
			if (NULL != m_PtrDmuxFrm)
			{
				if ( m_PlaySeekTgTime >= m_PtrDmuxFrm->TimeSec )
				{
					// found
					m_PlayMode	=	E_GrDvrFs4PlayModeWait;
					break;
				}
			}
			// go next frame
			Tv_Rval	=	DmuxGoFw();
			if ((E_GrDvrFs4DmuxRsltEndOfData == Tv_Rval) || (E_GrDvrFs4DmuxRsltDataEmpty == Tv_Rval))
			{
				// stop
				m_PlayMode	=	E_GrDvrFs4PlayModeWait;
				m_IsPlaying	=	FALSE;
				if (m_IsPlayDirBack)
				{
					m_PlayStat	=	E_GrDvrPlayStatStop;
				}
				else
				{
					m_PlayStat	=	E_GrDvrPlayStatEnd;
				}
				break;
			}
			else if (E_GrDvrFs4DmuxRsltBusy == Tv_Rval)
			{
				// wait until be ready
				Tv_Result	=	FALSE;
			}
		}

		// check next
		if (E_GrDvrPlayStatSeek == m_PlayStat)
		{
			m_PlaySeekPrgs ++;
			if (E_GrDvrFs4PlaySeekWorkMax >= m_PlaySeekPrgs)
			{
				m_PlaySeekPrgs	=	0;
				m_PlayMode			=	E_GrDvrFs4PlayModeWait;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::LcPlayModeNext(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Rval		=	DmuxGoFw();
		if ( E_GrDvrFs4DmuxRsltOk == Tv_Rval )
		{
			if (NULL != m_PtrDmuxFrm)
			{
				m_PlayMode			=	E_GrDvrFs4PlayModeVdec;
				Tv_Result				=	TRUE;
			}
		}
		else if ((E_GrDvrFs4DmuxRsltDataEmpty == Tv_Rval) && (E_GrDvrFs4DmuxRsltEndOfData == Tv_Rval))
		{
			m_IsPlaying		=	FALSE;
			m_PlayMode		=	E_GrDvrFs4PlayModeWait;
			m_PlayStat		=	E_GrDvrPlayStatEnd;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::LcPlayModePrev(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Rval		=	DmuxGoBw();
		if (E_GrDvrFs4DmuxRsltOk == Tv_Rval)
		{
			if (NULL != m_PtrDmuxFrm)
			{
				m_PlayMode			=	E_GrDvrFs4PlayModeVdec;
				Tv_Result				=	TRUE;
			}
		}
		else if ((E_GrDvrFs4DmuxRsltDataEmpty == Tv_Rval) && (E_GrDvrFs4DmuxRsltEndOfData == Tv_Rval))
		{
			m_IsPlaying		=	FALSE;
			m_PlayMode		=	E_GrDvrFs4PlayModeWait;
			m_PlayStat		=	E_GrDvrPlayStatBegin;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::LcPlayModeVdec(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;

		if (NULL != m_PtrDmuxFrm)
		{
			if ((E_GrDvrFrmTypeVdoI <= m_PtrDmuxFrm->Type) && (E_GrDvrFrmTypeVdoB >= m_PtrDmuxFrm->Type))
			{
				// try decode
				Tv_Rval	=	DecVdo((Ptr_GrDvrFrmVdo)m_PtrDmuxFrm, TRUE);
				if (E_GrDvrFs4PlayRsltOk == Tv_Rval)
				{
					// success
					m_IsPlaying		=	FALSE;
					m_PlayMode		=	E_GrDvrFs4PlayModeWait;
					m_PlayStat		=	E_GrDvrPlayStatStop;
				}
				else if (E_GrDvrFs4PlayRsltBusy != Tv_Rval)
				{
					DbgMsgPrint("Cls_GrDvrFs4Play::LcPlayModeVdec - video frame decode error!\n");
					m_IsPlaying		=	FALSE;
					m_PlayMode		=	E_GrDvrFs4PlayModeWait;
					m_PlayStat		=	E_GrDvrPlayStatStop;
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrDvrFs4Play::LcPlayModeVdec - [BUG] bad frame type(%d)!\n", m_PtrDmuxFrm->Type);
				m_IsPlaying		=	FALSE;
				m_PlayMode		=	E_GrDvrFs4PlayModeWait;
				m_PlayStat		=	E_GrDvrPlayStatStop;
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrDvrFs4Play::LcPlayModeVdec - [BUG] frame not found!\n");
			m_IsPlaying		=	FALSE;
			m_PlayMode		=	E_GrDvrFs4PlayModeWait;
			m_PlayStat		=	E_GrDvrPlayStatStop;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayCmdStepNext(void)
{
	// local -------------------
	// code --------------------
		// dmux mode set
		DmuxModeSet(E_GrDvrFs4DmuxModeAll, 0, 0);
		DmuxAdoAbleMapSet(0);
		// set mode
		m_PlayMode			=	E_GrDvrFs4PlayModeNext;
		m_IsPlayTmSync	=	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayCmdStepPrev(void)
{
	// local -------------------
	// code --------------------
		// dmux mode set
		DmuxModeSet(E_GrDvrFs4DmuxModeAll, 0, 0);
		DmuxAdoAbleMapSet(0);
		// set mode
		m_PlayMode			=	E_GrDvrFs4PlayModePrev;
		m_IsPlayTmSync	=	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayCmdJmpBegin(void)
{
	// local -------------------
	// code --------------------
		m_PlayMode			=	E_GrDvrFs4PlayModeJmpBegin;
		m_IsPlayTmSync	=	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::LcPlayModeJmpBegin(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Rval	=	DmuxGoBegin();
		if (E_GrDvrFs4DmuxRsltOk == Tv_Rval)
		{
			// play speed zero
			LcPlayCmdSpeedSet(0);
			// play direction positive
			LcPlayCmdDirSet(FALSE);

			m_PlayMode			=	E_GrDvrFs4PlayModeWait;
			Tv_Result				=	TRUE;
		}
		else if ((E_GrDvrFs4DmuxRsltEndOfData == Tv_Rval) || (E_GrDvrFs4DmuxRsltDataEmpty == Tv_Rval))
		{
			m_PlayMode	=	E_GrDvrFs4PlayModeWait;
			m_PlayStat	=	E_GrDvrPlayStatBegin;
			m_IsPlaying	=	FALSE;
		}
		else
		{
			m_PlayStat	=	E_GrDvrPlayStatSeek;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayCmdSpeedSet(__s32 A_Speed)
{
	// local -------------------
	// code --------------------
		m_PlaySpeed	=	A_Speed;

		LcPlayDmuxModeUpdt();
		LcPlayDispSkipAllUpdt();

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayCmdDirSet(BOOL8 A_IsBack)
{
	// local -------------------
	// code --------------------
		if (m_IsPlayDirBack	!=	A_IsBack)
		{
			m_IsPlayDirBack	=	A_IsBack;

			LcPlayDmuxModeUpdt();
			LcPlayDispSkipAllUpdt();
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayCmdJmpEnd(void)
{
	// local -------------------
	// code --------------------
		m_PlayMode			=	E_GrDvrFs4PlayModeJmpEnd;
		m_IsPlayTmSync	=	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Play::LcPlayModeJmpEnd(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Rval	=	DmuxGoRecent();
		if (E_GrDvrFs4DmuxRsltOk == Tv_Rval)
		{
			// play speed zero
			LcPlayCmdSpeedSet(0);
			// play direction positive
			LcPlayCmdDirSet(FALSE);

			m_PlayMode			=	E_GrDvrFs4PlayModeWait;
			Tv_Result				=	TRUE;
		}
		else if ((E_GrDvrFs4DmuxRsltEndOfData == Tv_Rval) || (E_GrDvrFs4DmuxRsltDataEmpty == Tv_Rval))
		{
			m_PlayMode	=	E_GrDvrFs4PlayModeWait;
			m_PlayStat	=	E_GrDvrPlayStatBegin;
			m_IsPlaying	=	FALSE;
		}
		else
		{
			m_PlayStat	=	E_GrDvrPlayStatSeek;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayCmdVdoChSet(__u32 A_Map)
{
	// local -------------------
	// code --------------------
		m_PlayVdoMap	=	A_Map;
		LcPlayDmuxModeUpdt();
		LcPlayDispSkipAllUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayCmdAdoChSet(__u32 A_Map)
{
	// local -------------------
	// code --------------------
		m_PlayAdoMap	=	A_Map;
		LcPlayDmuxModeUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayCmdDevSet(__u8 A_DevType)
{
	// local -------------------
	// code --------------------
		DmuxDevSet(A_DevType);
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Play::LcPlayCmdStop(void)
{
	// local -------------------
	// code --------------------
		m_IsPlaying		=	FALSE;
		m_PlayStat		=	E_GrDvrPlayStatStop;
		m_PlayMode		=	E_GrDvrFs4PlayModeWait;
}
//--------------------------------------------------------------------
