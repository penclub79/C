/*
 GAUSE file system version 2 Play

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include <Dvr/Fs/V2/GrDvrFs2Play.h>

#include	<stdlib.h>

//====================================================================
//local const

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs2Play::Cls_GrDvrFs2Play( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFs2Mng* A_FsMng, Ptr_GrDvrFs2SeekCtrl A_PtrSeekCtrl, BOOL8 A_IsSafe ):
Cls_GrDvrFs2Dmux( A_TaskSvr, A_FsMng, A_PtrSeekCtrl, A_IsSafe )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		// init
		m_RunMode		=	E_GrDvrFs2PlayRunModeWait;

		GrBufQueInit( (Ptr_GrBufQue)m_CmdQue, sizeof(St_GrDvrFs2PlayCmdItem), sizeof(m_CmdQue) );

		m_PlayStat	=	E_GrDvrPlayStatStop;
		m_IsDirBack	=	FALSE;

		m_IsDecoded	=	FALSE;

		m_IsSmTime	=	FALSE;

		m_IsNoTimeSync	=	FALSE;
		m_IsSeekAfterPlay	=	FALSE;

		m_DropCnt		=	0;

		GrDumyZeroMem( m_ChTbl, sizeof(m_ChTbl) );
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
		{
			m_ChTbl[Tv_WkIdx].Res		=	0xFF;
			m_ChTbl[Tv_WkIdx].Speed	=	0xFF;
		}

		// DAMGI - debug
		LcVdoChEnSet( 0xFFFF );

		// register
		m_TaskSvr->RegistBkgClient( this );
}
//--------------------------------------------------------------------
Cls_GrDvrFs2Play::~Cls_GrDvrFs2Play()
{

		// unregister
		m_TaskSvr->UnregistBkgClient( this );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		switch ( m_RunMode )
		{
			case E_GrDvrFs2PlayRunModeSeek:
				Tv_Result	=	LcRunModeSeek();
				break;
			case E_GrDvrFs2PlayRunModePlay:
				Tv_Result	=	LcRunModePlay();
				break;
			case E_GrDvrFs2PlayRunModeNext:
				Tv_Result	=	LcRunModeNext();
				break;
			case E_GrDvrFs2PlayRunModePrev:
				Tv_Result	=	LcRunModePrev();
				break;
			default:
				Tv_Result	=	LcRunModeWait();
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::LcPableReset( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
		{
			m_ChTbl[Tv_WkIdx].IsPable	=	FALSE;
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::LcRunModeWait( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	LcParsPlayCmd();
		// status update
		if ( !Tv_Result )
		{
			m_PlayStat	=	E_GrDvrPlayStatStop;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::LcRunModeSeek( void )
{
	// local -------------------
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		BOOL8	Tv_IsNxtFrm;
		__u8	Tv_Ch;
	// code --------------------
		// init
		m_PlayStat	=	E_GrDvrPlayStatSeek;
		Tv_IsNxtFrm	=	FALSE;
		// get frame data
		Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)m_PtrFrm;
		if ( m_IsDirBack )
		{
			// backward
			if ( m_IsSeekCnt )
			{
				// seek by count
				FrmPrev();
				m_SeekFrmCnt --;
				if (E_GrDvrSeekStatNormal != m_PtrSeekCtrl->Stat)
				{
					m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
				}
				else if (0 == m_SeekFrmCnt)
				{
					// seek finish
					if ( m_IsSeekAfterPlay && (E_GrDvrSeekStatNormal == m_PtrSeekCtrl->Stat) )
					{
						LcDoPlay();
					}
					else
					{
						m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
					}
				}
			}	//	if ( m_IsSeekCnt )
			else
			{
				// seek by time
				Tv_IsNxtFrm	=	TRUE;
				if ( NULL != Tv_PtrAvHd )
				{
					// init
					Tv_Ch	=	Tv_PtrAvHd->Ch;
					// check frame type , i frame only
					if ( (E_GrDvrAvTypVdoI == Tv_PtrAvHd->Type) && (E_GrDvrMaxChCnt > Tv_Ch) )
					{
						// video only
						if ( m_ChTbl[Tv_Ch].IsVdoAble )
						{
							// check time
							if ( m_SeekTime >= Tv_PtrAvHd->TimeSec )
							{
								// valid frame found
								Tv_IsNxtFrm	=	FALSE;
								// found
								// seek finish
								if ( m_IsSeekAfterPlay )
								{
									LcDoPlay();
								}
								else
								{
									m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
								}
							}
						}
					}
#ifdef DAMGI_DBG
					if ( 0 == (0x3F & m_DbgSeekCnt) )
					{
						/*
						DbgMsgPrint( "frm bidx= %d, read=%d, prgs=%d frmsize=%d\n", m_PtrSeekCtrl->BufIdx, m_PtrSeekCtrl->Buf[m_PtrSeekCtrl->BufIdx].Readed,
							m_PtrSeekCtrl->Buf[m_PtrSeekCtrl->BufIdx].Prgs, Tv_PtrAvHd->DataSize );
							*/
						DbgMsgPrint( "seektime= %8x frmtime = %8x \n", m_SeekTime, Tv_PtrAvHd->TimeSec );
					}
#endif
				}	//	if ( NULL != Tv_PtrAvHd )

				// next frame
				if ( Tv_IsNxtFrm )
				{
					FrmPrev();
#ifdef DAMGI_DBG
					/*
					if ( 0 == (0x3F & m_DbgSeekCnt) )
					{
						DbgMsgPrint( "PlayFrmMove\n" );
					}
					*/
					m_DbgSeekCnt ++;
#endif
				}
				// check end frame
				if ( E_GrDvrSeekStatNormal != m_PtrSeekCtrl->Stat )
				{
					// seek finish
					m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
				}
			}	//	if ( m_IsSeekCnt )
		}	//	if ( m_IsDirBack )
		else
		{
			// forward
			if ( m_IsSeekCnt )
			{
				// seek count mode
				FrmNext();
				m_SeekFrmCnt --;
				if ( E_GrDvrSeekStatNormal != m_PtrSeekCtrl->Stat )
				{
					m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
				}
				else if ( 0 == m_SeekFrmCnt )
				{
					// seek finish
					if ( m_IsSeekAfterPlay && (E_GrDvrSeekStatNormal == m_PtrSeekCtrl->Stat) )
					{
						LcDoPlay();
					}
					else
					{
						m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
					}
				}
			}	//	if ( m_IsSeekCnt )
			else
			{
				// seek time mode
				Tv_IsNxtFrm	=	TRUE;
				if ( NULL != Tv_PtrAvHd )
				{
					// init
					Tv_Ch	=	Tv_PtrAvHd->Ch;
					// check frame type
					if ( ((E_GrDvrAvTypVdoI == Tv_PtrAvHd->Type) ||(E_GrDvrAvTypAdo == Tv_PtrAvHd->Type)) && (E_GrDvrMaxChCnt > Tv_Ch) )
					{
						// check type
						if ( E_GrDvrAvTypAdo == Tv_PtrAvHd->Type)
						{
							// audio
							if ( m_ChTbl[Tv_Ch].IsAdoAble && (0 == m_PlaySpeed) )
							{
								// check time
								if ( m_SeekTime <= Tv_PtrAvHd->TimeSec )
								{
									// valid frame found
									Tv_IsNxtFrm	=	FALSE;
									// seek finish
									if ( m_IsSeekAfterPlay )
									{
										LcDoPlay();
									}
									else
									{
										m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
									}
								}
							}
						}
						else
						{
							// video
							if ( m_ChTbl[Tv_Ch].IsVdoAble )
							{
								// check time
								if ( m_SeekTime <= Tv_PtrAvHd->TimeSec )
								{
									// valid frame found
									Tv_IsNxtFrm	=	FALSE;
									// seek finish
									if ( m_IsSeekAfterPlay )
									{
										LcDoPlay();
									}
									else
									{
										m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
									}
								}
							}
						}
					}
				}	//	if ( NULL != Tv_PtrAvHd )

				// next frame
				if ( Tv_IsNxtFrm )
				{
					FrmNext();
				}
				// check end frame
				if ( E_GrDvrSeekStatNormal != m_PtrSeekCtrl->Stat )
				{
					m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
				}

			}	//	if ( m_IsSeekCnt )
		}	//	if ( m_IsDirBack )

		// prepare wait mode
		if ( E_GrDvrFs2PlayRunModeSeek !=  m_RunMode )
		{
			LcPableReset();		// P frame reset
			m_PlaySec		=	0;	// play time reset
			m_IsSeekAfterPlay	=	FALSE;	// seek after play reset
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::LcPlayBackward( void )
{
	// local -------------------
		BOOL8	Tv_IsNext;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		__u32	Tv_PlayGap;
		BOOL8	Tv_IsPableReset;
		BOOL8	Tv_IsDropFrm;
		BOOL8	Tv_IsDisp;
		__u8	Tv_Ch;
		__u8	Tv_DecRslt;
	// code --------------------
		// init
		Tv_IsNext	=	TRUE;		// next frame able 
		// get frame pointer
		Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)m_PtrFrm;
		// check frame exist
		if ( NULL != Tv_PtrAvHd )
		{
			// patch time
			if ( m_IsNoTimeSync || (0 == m_PlaySec))
			{
				m_PlaySec		=	Tv_PtrAvHd->TimeSec;
				m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
			}
			else if (Tv_PtrAvHd->TimeSec > m_PlaySec) 				
			{
				Tv_PlayGap	=	Tv_PtrAvHd->TimeSec - m_PlaySec;
				if (E_GrDvrFs2PlayTimeGapLimite < Tv_PlayGap)
				{
					m_PlaySec		=	Tv_PtrAvHd->TimeSec;
					m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
				}
			}
			else
			{
				Tv_PlayGap	=	m_PlaySec - Tv_PtrAvHd->TimeSec;
				if (E_GrDvrFs2PlayTimeGapLimite < Tv_PlayGap)
				{
					m_PlaySec		=	Tv_PtrAvHd->TimeSec;
					m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
				}
			}

			m_IsSmTime	=	Tv_PtrAvHd->IsSm;		// update summer time

			// check stream type
			if ( (E_GrDvrAvTypVdoP >= Tv_PtrAvHd->Type) || (E_GrDvrAvTypVdoB == Tv_PtrAvHd->Type) )
			{
				Tv_Ch						=	Tv_PtrAvHd->Ch;
				Tv_IsPableReset	=	TRUE;
				// check able channel
				if ( m_ChTbl[Tv_Ch].IsVdoAble )
				{
					// check frame drop
					Tv_IsDropFrm	=	TRUE;
					// check i frame only
					if ( E_GrDvrAvTypVdoI == Tv_PtrAvHd->Type )
					{
						if ( 0 == m_ChTbl[Tv_Ch].DropPos )
						{
							Tv_IsDropFrm		=	FALSE;
							Tv_IsNext				=	FALSE;
							// check time
							if ( m_IsNoTimeSync || (Tv_PtrAvHd->TimeSec > m_PlaySec) ||( (Tv_PtrAvHd->TimeSec == m_PlaySec) && (Tv_PtrAvHd->TimeMsec >= m_PlayMsec) )  )
							{
								// update display count
								Tv_IsDisp	=	TRUE;
								if ( 0 != m_ChTbl[Tv_Ch].DispPos )
								{
									Tv_IsDisp	=	FALSE;
								}
								
								// decode
								Tv_DecRslt	=	DecVdo(Tv_PtrAvHd, Tv_IsDisp);
								if ( E_GrDvrFs2PlayDecOk == Tv_DecRslt )
								{
									// update
									m_ChTbl[Tv_Ch].SeqId	=	Tv_PtrAvHd->SeqId;
									//m_PlaySec				=	Tv_PtrAvHd->TimeSec;
									//m_PlayMsec			=	Tv_PtrAvHd->TimeMsec;
									Tv_IsPableReset	=	FALSE;
									m_ChTbl[Tv_Ch].IsPable	=	TRUE;
									m_IsDecoded	=	TRUE;
								}

								// check not busy
								if ( E_GrDvrFs2PlayDecBusy != Tv_DecRslt )
								{
									// go next frame
									Tv_IsNext	=	TRUE;
									m_ChTbl[Tv_Ch].DropPos	=	m_DropCnt;	// update drop
									m_PlayStat	=	E_GrDvrPlayStatPlay;	// update status

									// display status update
									if ( 0 == m_ChTbl[Tv_Ch].DispPos )
									{
										m_ChTbl[Tv_Ch].DispPos	=	m_ChTbl[Tv_Ch].DispStep;
									}
									else
									{
										m_ChTbl[Tv_Ch].DispPos --;
									}

									// update parameter
									if ( (Tv_PtrAvHd->SkipCnt != m_ChTbl[Tv_Ch].Speed) || (Tv_PtrAvHd->Res != m_ChTbl[Tv_Ch].Res) )
									{
										// update play speed
										LcSpdChUpdt( Tv_Ch, Tv_PtrAvHd->Res, Tv_PtrAvHd->SkipCnt );
										LcSpdParaUpdt();
									}
								}
							}
						}
					}	//	if ( E_GrDvrAvTypVdoI == Tv_PtrAvHd->Type )

					// check drop
					if ( Tv_IsDropFrm && (0 != m_ChTbl[Tv_Ch].DropPos) )
					{
						m_ChTbl[Tv_Ch].DropPos --;
					}

				}
				// p frame able reset
				if ( Tv_IsPableReset )
				{
					m_ChTbl[Tv_PtrAvHd->Ch].IsPable	=	FALSE;
				}
			}

		}	//	if ( NULL != Tv_PtrAvHd )
		// check go next 
		if ( Tv_IsNext )
		{
			FrmPrev();
			// check status
			if ( E_GrDvrSeekStatNormal != m_PtrSeekCtrl->Stat )
			{
				m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
			}
		}

		return	(BOOL8)Tv_IsNext;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::LcPlayForward( void )
{
	// local -------------------
		BOOL8	Tv_IsNext;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		__u32	Tv_PlayGap;
		BOOL8	Tv_IsPableReset;
		BOOL8	Tv_IsDecAble;
		__u8	Tv_Ch;
		BOOL8	Tv_IsDisp;
		__u8	Tv_SeqId;
		__u8	Tv_DecRslt;
		Def_StrTag	Tv_StrDmsg;
	// code --------------------
		// init
		Tv_IsNext	=	TRUE;		// next frame able 
		// get frame pointer
		Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)m_PtrFrm;

		// check frame exist
		if ( NULL != Tv_PtrAvHd )
		{

			m_IsSmTime	=	Tv_PtrAvHd->IsSm;		// update summer time

			// check stream type
			if ( (E_GrDvrAvTypVdoP >= Tv_PtrAvHd->Type) || (E_GrDvrAvTypVdoB == Tv_PtrAvHd->Type) )
			{
				// video stream
				Tv_Ch						=	Tv_PtrAvHd->Ch;
				Tv_IsPableReset	=	TRUE;
				// check able channel
				if ( m_ChTbl[Tv_Ch].IsVdoAble ) 
				{

					// check frame drop
					Tv_IsDecAble		=	FALSE;
					if ( E_GrDvrAvTypVdoI == Tv_PtrAvHd->Type )
					{
						Tv_IsDecAble	=	TRUE;
					}
					else if (m_ChTbl[Tv_Ch].IsPable && (!m_ChTbl[Tv_Ch].IsIonly))
					{
						// check sequence id
						Tv_SeqId	=	m_ChTbl[Tv_Ch].SeqId + 1;
						if ( Tv_SeqId == Tv_PtrAvHd->SeqId )
						{
							Tv_IsDecAble	=	TRUE;
						}
						else
						{
							GrStrTimeToStr( Tv_StrDmsg, Tv_PtrAvHd->TimeSec );
							DbgMsgPrint( "Cls_GrDvrFs2Play::LcPlayForward - invaild sequence number at %s.\n", Tv_StrDmsg );
						}
					}

					// check decode able
					if ( Tv_IsDecAble )
					{
						// check drop
						if ( 0 == m_ChTbl[Tv_Ch].DropPos )
						{

							// patch time
							if ((0 == m_PlaySec) || m_IsNoTimeSync)
							{
								m_PlaySec		=	Tv_PtrAvHd->TimeSec;
								m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
							}
							else if (Tv_PtrAvHd->TimeSec < m_PlaySec) 
							{
								Tv_PlayGap	=	m_PlaySec - Tv_PtrAvHd->TimeSec;
								if (1 < Tv_PlayGap)
								{
									m_PlaySec		=	Tv_PtrAvHd->TimeSec;
									m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
								}
							}
							else
							{
								Tv_PlayGap	=	Tv_PtrAvHd->TimeSec - m_PlaySec;
								if (E_GrDvrFs2PlayTimeGapLimite < Tv_PlayGap)
								{
									m_PlaySec		=	Tv_PtrAvHd->TimeSec;
									m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
								}
							}

							Tv_IsPableReset	=	FALSE;
							Tv_IsNext	=	FALSE;	//	wait time reach
							// check time
							if ( m_IsNoTimeSync || (Tv_PtrAvHd->TimeSec < m_PlaySec) ||( (Tv_PtrAvHd->TimeSec == m_PlaySec) && (Tv_PtrAvHd->TimeMsec <= m_PlayMsec) )  )
							{
								Tv_IsPableReset	=	TRUE;
								// update display count
								Tv_IsDisp	=	TRUE;
								if ( 0 != m_ChTbl[Tv_Ch].DispPos )
								{
									Tv_IsDisp	=	FALSE;
								}
								
								// decode
								Tv_DecRslt	=	DecVdo(Tv_PtrAvHd, Tv_IsDisp);
								if ( E_GrDvrFs2PlayDecOk == Tv_DecRslt )
								{
									// update
									m_ChTbl[Tv_Ch].SeqId	=	Tv_PtrAvHd->SeqId;
									//m_PlaySec				=	Tv_PtrAvHd->TimeSec;
									//m_PlayMsec			=	Tv_PtrAvHd->TimeMsec;
									Tv_IsPableReset	=	FALSE;
									m_ChTbl[Tv_Ch].IsPable	=	TRUE;
									m_IsDecoded	=	TRUE;
								}

								if ( E_GrDvrFs2PlayDecBusy == Tv_DecRslt )
								{
									Tv_IsPableReset	=	FALSE;
								}
								else
								{
									if ( 0 == m_ChTbl[Tv_Ch].DispPos )
									{
										m_ChTbl[Tv_Ch].DispPos	=	m_ChTbl[Tv_Ch].DispStep;
									}
									else
									{
										m_ChTbl[Tv_Ch].DispPos --;
									}

									// go next frame
									Tv_IsNext	=	TRUE;
									m_ChTbl[Tv_Ch].DropPos	=	m_DropCnt;	// update drop
									m_PlayStat	=	E_GrDvrPlayStatPlay;	// update status

									// update parameter
									if ( (Tv_PtrAvHd->SkipCnt != m_ChTbl[Tv_Ch].Speed) || (Tv_PtrAvHd->Res != m_ChTbl[Tv_Ch].Res) )
									{
										// update play speed
										LcSpdChUpdt( Tv_Ch, Tv_PtrAvHd->Res, Tv_PtrAvHd->SkipCnt );
										LcSpdParaUpdt();
									}
								}
								
							}
						}
						else
						{
							// frame drop
							m_ChTbl[Tv_Ch].DropPos --;
						}
					}

				}	//	if ( m_ChTbl[Tv_Ch].IsVdoAble )
				// p frame able reset
				if ( Tv_IsPableReset )
				{
					m_ChTbl[Tv_PtrAvHd->Ch].IsPable	=	FALSE;
				}

			}	//	if ( (E_GrDvrAvTypVdoP >= Tv_PtrAvHd->Type) || (E_GrDvrAvTypVdoB == Tv_PtrAvHd->Type) )
			else if ( E_GrDvrAvTypAdo == Tv_PtrAvHd->Type )
			{
				// audio stream
				Tv_Ch						=	Tv_PtrAvHd->Ch;
				if ( (0 == m_PlaySpeed) && m_ChTbl[Tv_Ch].IsAdoAble )
				{
					// decode audio
					DecAdo( Tv_PtrAvHd );
				}
			}

		}	//	if ( NULL != Tv_PtrAvHd )
		// check go next 
		if ( Tv_IsNext )
		{
			FrmNext();
			// check status
			if ( E_GrDvrSeekStatNormal != m_PtrSeekCtrl->Stat )
			{
				m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
			}
		}

		return	(BOOL8)Tv_IsNext;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::LcRunModePlay( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_TickNow;
		__u32	Tv_TickGap;
		__u32	Tv_MsGap;
		__u32	Tv_GapMsec;
		__u32	Tv_GapSec;
	// code --------------------
		// init
		Tv_Result	=	TRUE;
		// parsing command
		if ( !LcParsPlayCmd() )
		{
			// init
			Tv_Result		=	FALSE;
			Tv_GapSec		=	0;
			Tv_GapMsec	=	0;
			// get time gap
			if ( (!m_IsNoTimeSync) && (0 != m_PlaySec) )
			{
				Tv_TickNow	=	GrTimeGetTick();
				Tv_TickGap	=	Tv_TickNow - m_PlayTick;
				if ( 0 < m_PlaySpeed )
				{
					if (E_GrDvrFs2PlaySpeedMjBase > m_PlaySpeed)
					{
						Tv_TickGap	=	Tv_TickGap << m_PlaySpeed;
					}
					else
					{
						Tv_TickGap	=	Tv_TickGap << (E_GrDvrFs2PlaySpeedMjBase - 1);
					}
				}
				else if ( 0 > m_PlaySpeed )
				{
					Tv_TickGap	=	Tv_TickGap >> (-m_PlaySpeed);
				}
				// check slow able
				Tv_MsGap		=	GrTimeTickToMs( Tv_TickGap );
				if ( 0 != Tv_MsGap )
				{
					Tv_GapSec		=	Tv_MsGap / E_GrTimeMiliOfSec;
					Tv_GapMsec	=	Tv_MsGap - (Tv_GapSec * E_GrTimeMiliOfSec);
					m_PlayTick	=	Tv_TickNow;	// update last tick
				}
			}	//	if ( (0 != m_PlaySec) && (E_GrDvrFs2PlaySpeedMax > m_PlaySpeed) )
			else
			{
				m_PlayTick	=	GrTimeGetTick();
			}
			// check direction
			if ( m_IsDirBack )
			{
				// backward
				// update time
				if ( (0 != m_PlaySec) && (!m_IsNoTimeSync) )
				{
					if ( Tv_GapMsec > m_PlayMsec  )
					{
						m_PlaySec --;
						m_PlayMsec	=	(m_PlayMsec + E_GrTimeMiliOfSec) - Tv_GapMsec;
					}
					else
					{
						m_PlayMsec	=	m_PlayMsec - Tv_GapMsec;
					}
					m_PlaySec	=	m_PlaySec - Tv_GapSec;
				}
				// check play speed
				if ( E_GrDvrFs2PlaySpeedMjBase <= m_PlaySpeed )
				{
					// max speed
					Tv_Result	=	LcPlayMaxBw();
				}
				else
				{
					// normal speed
					Tv_Result	=	LcPlayBackward();
				}
			}	//	if ( m_IsDirBack )
			else
			{
				// forward
				// update time
				if ( (0 != m_PlaySec) && (!m_IsNoTimeSync) )
				{
					m_PlayMsec	=	m_PlayMsec + Tv_GapMsec;
					if ( E_GrTimeMiliOfSec <= m_PlayMsec  )
					{
						m_PlaySec ++;
						m_PlayMsec	=	m_PlayMsec - E_GrTimeMiliOfSec;
					}
					m_PlaySec	=	m_PlaySec + Tv_GapSec;
				}
				// check play speed
				if ( E_GrDvrFs2PlaySpeedMjBase <= m_PlaySpeed )
				{
					// max speed
					Tv_Result	=	LcPlayMaxFw();
				}
				else
				{
					Tv_Result	=	LcPlayForward();
				}

			}	//	if ( m_IsDirBack )

		}

		return	Tv_Result; 
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::LcSpdChUpdt( __u8 A_Ch, __u8 A_Res, __u8 A_Speed )
{
	// local -------------------
		__s32		Tv_Spd;
		__u32	Tv_Sval;
		__u32	Tv_Step;
		__u32	Tv_Dval;
	// code --------------------
		m_ChTbl[A_Ch].Speed		=	A_Speed;
		m_ChTbl[A_Ch].Res			=	A_Res;
		// speed patch
		if ( 0 == m_ChTbl[A_Ch].Speed )
		{
			m_ChTbl[A_Ch].Speed ++;
		}
		// calculate channel power
		m_ChTbl[A_Ch].Pwr	=	m_VdoSigFps;
		Tv_Spd	=	(__s32)m_ChTbl[A_Ch].Res + m_PlaySpeed;
		if ( 0 < Tv_Spd )
		{
			m_ChTbl[A_Ch].Pwr	=	m_ChTbl[A_Ch].Pwr << Tv_Spd;
		}
		else if ( 0 > Tv_Spd )
		{
			m_ChTbl[A_Ch].Pwr	=	m_ChTbl[A_Ch].Pwr >> (-Tv_Spd);
		}
		m_ChTbl[A_Ch].Pwr	=	m_ChTbl[A_Ch].Pwr / (__u32)m_ChTbl[A_Ch].Speed;
		// calculate display step
		Tv_Step	=	0;
		if ( 0 < m_PlaySpeed )
		{
			Tv_Sval	=	1 << m_PlaySpeed;
			Tv_Dval	=	(__u32)m_ChTbl[A_Ch].Speed;
			if ( 0 != m_DropCnt )
			{
				Tv_Dval	=	Tv_Spd * (__u32)m_Gop;
			}
			if ( Tv_Sval > Tv_Dval )
			{
				Tv_Step	=	Tv_Sval / Tv_Dval;
			}
			if ( Tv_Sval > (__u32)m_ChTbl[A_Ch].Speed )
			{
				Tv_Step	=	Tv_Sval / (__u32)m_ChTbl[A_Ch].Speed;
			}
		}
		m_ChTbl[A_Ch].DispStep	=	(__u8)Tv_Step;
		m_ChTbl[A_Ch].DispPos		=	(__u8)Tv_Step;

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::LcDispParaUpdt( void )
{
	// local -------------------
		__u8	Tv_Ch;
		__u32	Tv_Sval;
		__u32	Tv_Step;
		__u32	Tv_Spd;
	// code --------------------
		for ( Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++ )
		{
			if ( m_ChTbl[Tv_Ch].IsVdoAble )
			{
				Tv_Step	=	0;
				if ( 0 < m_PlaySpeed )
				{
					Tv_Sval	=	1 << m_PlaySpeed;
					Tv_Spd	=	(__u32)m_ChTbl[Tv_Ch].Speed;
					if ( 0 != m_DropCnt )
					{
						Tv_Spd	=	Tv_Spd * (__u32)m_Gop;
					}
					if ( Tv_Sval > Tv_Spd )
					{
						Tv_Step	=	Tv_Sval / Tv_Spd;
					}
				}
				m_ChTbl[Tv_Ch].DispStep	=	(__u8)Tv_Step;
				m_ChTbl[Tv_Ch].DispPos		=	(__u8)Tv_Step;
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::LcSpdParaUpdt( void )
{
	// local -------------------
		__u8	Tv_Ch;
		__u32	Tv_TotalPwr;
		__u32	Tv_DropCnt;
	// code --------------------
		// init
		Tv_TotalPwr	=	0;
		// calculate total power
		for ( Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++ )
		{
			if ( m_ChTbl[Tv_Ch].IsVdoAble )
			{
				Tv_TotalPwr	=	Tv_TotalPwr + m_ChTbl[Tv_Ch].Pwr;
			}
		}
		// check skip count
		if ( 0 < m_PlaySpeed )
		{
			Tv_TotalPwr	=	Tv_TotalPwr << m_PlaySpeed;
		}
		else if ( 0 > m_PlaySpeed )
		{
			Tv_TotalPwr	=	Tv_TotalPwr >> (-m_PlaySpeed);
		}
		Tv_DropCnt	=	0;
		while ( 0 != Tv_TotalPwr )
		{
			if ( Tv_TotalPwr > m_DecPower )
			{
				Tv_DropCnt ++;
				Tv_TotalPwr	=	Tv_TotalPwr - m_DecPower;
			}
			else
			{
				Tv_TotalPwr	=	0;
			}
		}
		m_DropCnt	=	(__u8)Tv_DropCnt;
		// update drop position
		for ( Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++ )
		{
			if ( m_ChTbl[Tv_Ch].IsVdoAble )
			{
				m_ChTbl[Tv_Ch].DropPos	=	m_DropCnt;
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::LcPlayMaxFw( void )
{
	// local -------------------
		BOOL8	Tv_IsNext;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		__u32	Tv_PlayGap;
		BOOL8	Tv_IsPableReset;
		BOOL8	Tv_IsDecAble;
		__u8	Tv_Ch;
		__u8	Tv_DecRslt;
		Def_GrTime	Tv_ReqTime;
	// code --------------------
		// init
		Tv_IsNext	=	TRUE;		// next frame able 
		// get frame pointer
		Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)m_PtrFrm;

		// check frame exist
		if ( NULL != Tv_PtrAvHd )
		{
			// patch time
			if ((0 == m_PlaySec) || m_IsNoTimeSync)
			{
				m_PlaySec		=	Tv_PtrAvHd->TimeSec;
				m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
			}
			else if (Tv_PtrAvHd->TimeSec < m_PlaySec) 
			{
				Tv_PlayGap	=	m_PlaySec - Tv_PtrAvHd->TimeSec;
				if (E_GrDvrFs2PlayTimeGapLimite < Tv_PlayGap)
				{
					m_PlaySec		=	Tv_PtrAvHd->TimeSec;
					m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
				}
			}
			else
			{
				Tv_PlayGap	=	Tv_PtrAvHd->TimeSec - m_PlaySec;
				if (E_GrDvrFs2PlayTimeGapLimite < Tv_PlayGap)
				{
					m_PlaySec		=	Tv_PtrAvHd->TimeSec;
					m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
				}
			}

			m_IsSmTime	=	Tv_PtrAvHd->IsSm;		// update summer time

			// check stream type
			if ( (E_GrDvrAvTypVdoP >= Tv_PtrAvHd->Type) || (E_GrDvrAvTypVdoB == Tv_PtrAvHd->Type) )
			{
				// video stream
				Tv_Ch						=	Tv_PtrAvHd->Ch;
				Tv_IsPableReset	=	TRUE;
				// check able channel
				if ( m_ChTbl[Tv_Ch].IsVdoAble )
				{
					// check frame drop
					Tv_IsDecAble		=	FALSE;
					if ( E_GrDvrAvTypVdoI == Tv_PtrAvHd->Type )
					{
						Tv_IsDecAble	=	TRUE;
					}
					// check decode able
					if ( Tv_IsDecAble )
					{
						Tv_IsPableReset	=	FALSE;
						Tv_IsNext	=	FALSE;	//	wait time reach
						// check time
						if ( m_IsNoTimeSync || (Tv_PtrAvHd->TimeSec < m_PlaySec) ||( (Tv_PtrAvHd->TimeSec == m_PlaySec) && (Tv_PtrAvHd->TimeMsec <= m_PlayMsec) )  )
						{
							Tv_IsPableReset	=	TRUE;

							// decode
							Tv_DecRslt	=	DecVdo(Tv_PtrAvHd, TRUE);
							if ( E_GrDvrFs2PlayDecOk == Tv_DecRslt )
							{
								// update
								m_ChTbl[Tv_Ch].SeqId	=	Tv_PtrAvHd->SeqId;
								//m_PlaySec				=	Tv_PtrAvHd->TimeSec;
								//m_PlayMsec			=	Tv_PtrAvHd->TimeMsec;
								Tv_IsPableReset	=	FALSE;
								m_ChTbl[Tv_Ch].IsPable	=	TRUE;
								m_IsDecoded	=	TRUE;
							}

							// update parameter
							if ( (Tv_PtrAvHd->SkipCnt != m_ChTbl[Tv_Ch].Speed) || (Tv_PtrAvHd->Res != m_ChTbl[Tv_Ch].Res) )
							{
								// update play speed
								LcSpdChUpdt( Tv_Ch, Tv_PtrAvHd->Res, Tv_PtrAvHd->SkipCnt );
								LcSpdParaUpdt();
							}

							// go next frame
							m_PlayMaxPos ++;
							Tv_IsNext	=	TRUE;
							m_ChTbl[Tv_Ch].DropPos	=	m_DropCnt;	// update drop
							m_PlayStat	=	E_GrDvrPlayStatPlay;	// update status
						}
					}

				}	//	if ( m_ChTbl[Tv_Ch].IsVdoAble )
				// p frame able reset
				if ( Tv_IsPableReset )
				{
					m_ChTbl[Tv_PtrAvHd->Ch].IsPable	=	FALSE;
				}

			}	//	( (E_GrDvrAvTypVdoP >= Tv_PtrAvHd->Type) || (E_GrDvrAvTypVdoB == Tv_PtrAvHd->Type) )

		}	//	if ( NULL != Tv_PtrAvHd )
		// check go next 
		if ( Tv_IsNext )
		{
			// check play max position
			if ( m_PlayMaxPos >= m_VdoEnChCnt )
			{
				// one minute next
				m_PlayMaxPos	=	0;
				Tv_ReqTime	=	m_PlaySec + (E_GrTimeSecOfMin * ((m_PlaySpeed - E_GrDvrFs2PlaySpeedMjBase) + 1));
				Tv_ReqTime	=	Tv_ReqTime - (Tv_ReqTime % E_GrTimeSecOfMin);
				JumpByTime( Tv_ReqTime, TRUE );
			}
			else
			{
				FrmNext();
				// check status
				if ( E_GrDvrSeekStatNormal != m_PtrSeekCtrl->Stat )
				{
					m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
				}
			}
		}

		return	(BOOL8)Tv_IsNext;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::LcPlayMaxBw( void )
{
	// local -------------------
		BOOL8	Tv_IsNext;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		__u32	Tv_PlayGap;
		BOOL8	Tv_IsPableReset;
		BOOL8	Tv_IsDropFrm;
		BOOL8	Tv_IsDisp;
		__u8	Tv_Ch;
		__u8	Tv_DecRslt;
		Def_GrTime	Tv_ReqTime;
	// code --------------------
		// init
		Tv_IsNext	=	TRUE;		// next frame able 
		// get frame pointer
		Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)m_PtrFrm;
		// check frame exist
		if ( NULL != Tv_PtrAvHd )
		{
			// patch time
			if ( m_IsNoTimeSync || (0 == m_PlaySec))
			{
				m_PlaySec		=	Tv_PtrAvHd->TimeSec;
				m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
			}
			else if (Tv_PtrAvHd->TimeSec > m_PlaySec) 				
			{
				Tv_PlayGap	=	Tv_PtrAvHd->TimeSec - m_PlaySec;
				if (E_GrDvrFs2PlayTimeGapLimite < Tv_PlayGap)
				{
					m_PlaySec		=	Tv_PtrAvHd->TimeSec;
					m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
				}
			}
			else
			{
				Tv_PlayGap	=	m_PlaySec - Tv_PtrAvHd->TimeSec;
				if (E_GrDvrFs2PlayTimeGapLimite < Tv_PlayGap)
				{
					m_PlaySec		=	Tv_PtrAvHd->TimeSec;
					m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
				}
			}

			m_IsSmTime	=	Tv_PtrAvHd->IsSm;		// update summer time

			// check stream type
			if ( (E_GrDvrAvTypVdoP >= Tv_PtrAvHd->Type) || (E_GrDvrAvTypVdoB == Tv_PtrAvHd->Type) )
			{
				Tv_Ch						=	Tv_PtrAvHd->Ch;
				Tv_IsPableReset	=	TRUE;
				// check able channel
				if ( m_ChTbl[Tv_Ch].IsVdoAble )
				{
					// check frame drop
					Tv_IsDropFrm	=	TRUE;
					// check i frame only
					if ( E_GrDvrAvTypVdoI == Tv_PtrAvHd->Type )
					{
						if ( 0 == m_ChTbl[Tv_Ch].DropPos )
						{
							Tv_IsDropFrm	=	FALSE;
							Tv_IsNext			=	FALSE;
							// check time
							if ( m_IsNoTimeSync || (Tv_PtrAvHd->TimeSec > m_PlaySec) ||( (Tv_PtrAvHd->TimeSec == m_PlaySec) && (Tv_PtrAvHd->TimeMsec >= m_PlayMsec) )  )
							{
								// update display count
								Tv_IsDisp	=	TRUE;
								if ( 0 == m_ChTbl[Tv_Ch].DispPos )
								{
									m_ChTbl[Tv_Ch].DispPos	=	m_ChTbl[Tv_Ch].DispStep;
								}
								else
								{
									Tv_IsDisp	=	FALSE;
									m_ChTbl[Tv_Ch].DispPos --;
								}
								// decode
								Tv_DecRslt	=	DecVdo(Tv_PtrAvHd, Tv_IsDisp);
								if ( E_GrDvrFs2PlayDecOk == Tv_DecRslt )
								{
									// update
									m_ChTbl[Tv_Ch].SeqId	=	Tv_PtrAvHd->SeqId;
									//m_PlaySec				=	Tv_PtrAvHd->TimeSec;
									//m_PlayMsec			=	Tv_PtrAvHd->TimeMsec;
									Tv_IsPableReset	=	FALSE;
									m_ChTbl[Tv_Ch].IsPable	=	TRUE;
									m_IsDecoded	=	TRUE;
								}
								// update parameter
								if ( (Tv_PtrAvHd->SkipCnt != m_ChTbl[Tv_Ch].Speed) || (Tv_PtrAvHd->Res != m_ChTbl[Tv_Ch].Res) )
								{
									// update play speed
									LcSpdChUpdt( Tv_Ch, Tv_PtrAvHd->Res, Tv_PtrAvHd->SkipCnt );
									LcSpdParaUpdt();
								}
								// go next frame
								m_PlayMaxPos ++;
								Tv_IsNext	=	TRUE;
								m_ChTbl[Tv_Ch].DropPos	=	m_DropCnt;	// update drop
								m_PlayStat	=	E_GrDvrPlayStatPlay;	// update status
							}
						}
					}	//	if ( E_GrDvrAvTypVdoI == Tv_PtrAvHd->Type )

					// check drop
					if ( Tv_IsDropFrm && (0 != m_ChTbl[Tv_Ch].DropPos) )
					{
						m_ChTbl[Tv_Ch].DropPos --;
					}

				}
				// p frame able reset
				if ( Tv_IsPableReset )
				{
					m_ChTbl[Tv_PtrAvHd->Ch].IsPable	=	FALSE;
				}
			}

		}	//	if ( NULL != Tv_PtrAvHd )
		// check go next 
		if ( Tv_IsNext )
		{
			// check play max position
			if ( m_PlayMaxPos >= m_VdoEnChCnt )
			{
				// one minute next
				m_PlayMaxPos	=	0;
				Tv_ReqTime		=	m_PlaySec - (E_GrTimeSecOfMin * ((m_PlaySpeed - E_GrDvrFs2PlaySpeedMjBase) + 1));
				Tv_ReqTime		=	Tv_ReqTime - (Tv_ReqTime % E_GrTimeSecOfMin);
				JumpByTime( Tv_ReqTime, TRUE );
			}
			else
			{
				FrmPrev();
				// check status
				if ( E_GrDvrSeekStatNormal != m_PtrSeekCtrl->Stat )
				{
					m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
				}
			}
		}

		return	(BOOL8)Tv_IsNext;

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::JumpByTime( Def_GrTime A_Time, BOOL8 A_IsAfterPlay )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd	=	E_GrDvrFs2PlayCmdJmpByTime;
			Tv_PtrCmd->Dw		=	A_Time;
			Tv_PtrCmd->Bt		=	(__u8)A_IsAfterPlay;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::LcReqCmdSeekByTime - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::DoPlay( void )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdDoPlay;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::LcReqCmdPlay - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::RecDevTypeSet( __u8 A_DevType )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdDevSet;
			Tv_PtrCmd->Bt			=	A_DevType;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::RecDevTypeSet - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::DoStop( void )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdDoStop;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::DoStop - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::MoveNext( void )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdStepNext;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::MoveNext - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::MovePrev( void )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdStepPrev;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::MovePrev - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::VideoChEnable( __u16 A_Map )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdVdoChSet;
			Tv_PtrCmd->Wd			=	A_Map;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::VideoChEnable - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::AudioChEnable( __u16 A_Map )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdAdoChSet;
			Tv_PtrCmd->Wd			=	A_Map;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::AudioChEnable - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::IframeOnly( __u16 A_Map )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdIfrmOnly;
			Tv_PtrCmd->Wd			=	A_Map;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::IframeOnly - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::PlayDirSet( BOOL8 A_IsBack )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdDirSet;
			Tv_PtrCmd->Bt			=	(__u8)A_IsBack;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::DirectionSet - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::PlaySpeedSet( __s32 A_Speed )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdSpeedSet;
			Tv_PtrCmd->Dw			=	(__u32)A_Speed;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::SpeedSet - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::JumpBegin( void )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdJmpBegin;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::JumpBegin - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::JumpEnd( void )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdJmpEnd;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::JumpEnd - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::JumpByHndl( __u64 A_Hndl )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdJmpByHndl;
			Tv_PtrCmd->Hndl		=	A_Hndl;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::JumpByHndl - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::JumpByCnt( __u32 A_Cnt )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdJmpByCnt;
			Tv_PtrCmd->Dw			=	A_Cnt;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::JumpByCnt - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Play::PlayStatusGet( void )
{
		return	m_PlayStat;
}
//--------------------------------------------------------------------
__s32		Cls_GrDvrFs2Play::PlaySpeedGet( void )
{
		return	m_PlaySpeed;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs2Play::PlayTimeGet( void )
{
		return	m_PlaySec;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::IsSummerTime( void )
{
		return	m_IsSmTime;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::IsDirBack( void )
{
		return	m_IsDirBack;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Play::RecDevTypeGet( void )
{
		return	m_DevType;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::LcParsPlayCmd( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_Result	=	FALSE;
		// check command queue
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetOutPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			switch ( Tv_PtrCmd->Cmd )
			{
				case E_GrDvrFs2PlayCmdDoPlay:
					LcDoPlay();
					break;
				case E_GrDvrFs2PlayCmdJmpByTime:
					LcJumpByTime( Tv_PtrCmd->Dw, (BOOL8)Tv_PtrCmd->Bt );
					break;
				case E_GrDvrFs2PlayCmdJmpByHndl:
					if ( m_IsDirBack )
					{
						SeekByHndlBw( Tv_PtrCmd->Hndl );
					}
					else
					{
						SeekByHndlFw( Tv_PtrCmd->Hndl );
					}
					m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
					LcPableReset();
					m_PlaySec			=	0;
					m_PlayTick		=	GrTimeGetTick();
					break;
				case E_GrDvrFs2PlayCmdJmpByCnt:
					m_IsSeekCnt		=	TRUE;
					m_SeekFrmCnt	=	Tv_PtrCmd->Dw;
					m_RunMode		=	E_GrDvrFs2PlayRunModeSeek;
					LcPableReset();
					m_PlaySec			=	0;
					m_PlayTick		=	GrTimeGetTick();
					break;
				case E_GrDvrFs2PlayCmdJmpBegin:
					SeekBegin();
					m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
					LcPableReset();
					m_PlaySec			=	0;
					m_PlayTick		=	GrTimeGetTick();
					break;
				case E_GrDvrFs2PlayCmdJmpEnd:
					SeekEnd();
					m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
					LcPableReset();
					m_PlaySec			=	0;
					m_PlayTick		=	GrTimeGetTick();
					break;
				case E_GrDvrFs2PlayCmdDirSet:
					m_IsDirBack	=	(BOOL8)Tv_PtrCmd->Bt;
					break;
				case E_GrDvrFs2PlayCmdVdoChSet:
					LcVdoChEnSet( Tv_PtrCmd->Wd );
					break;
				case E_GrDvrFs2PlayCmdAdoChSet:
					LcAdoChEnSet( Tv_PtrCmd->Wd );
					break;
				case E_GrDvrFs2PlayCmdSpeedSet:
					if ( E_GrDvrFs2PlaySpeedMax >= (__s32)Tv_PtrCmd->Dw )
					{
						m_PlaySpeed	=	(__s32)Tv_PtrCmd->Dw;
						LcSpdParaUpdt();
						LcDispParaUpdt();
					}
					break;
				case E_GrDvrFs2PlayCmdStepNext:
					m_RunMode			=	E_GrDvrFs2PlayRunModeNext;
					break;
				case E_GrDvrFs2PlayCmdStepPrev:
					m_RunMode			=	E_GrDvrFs2PlayRunModePrev;
					break;
				case E_GrDvrFs2PlayCmdDevSet:
					m_DevType			=	Tv_PtrCmd->Bt;
					m_RunMode			=	E_GrDvrFs2PlayRunModeWait;
					break;
				case E_GrDvrFs2PlayCmdDoStop:
					m_RunMode			=	E_GrDvrFs2PlayRunModeWait;
					break;
				case E_GrDvrFs2PlayCmdIfrmOnly:
					LcIonlyChEnSet( Tv_PtrCmd->Wd );
					break;
				case E_GrDvrFs2PlayCmdNoTimeSync:
					m_IsNoTimeSync	=	(BOOL8)Tv_PtrCmd->Bt;
					break;

			}
			// release command
			GrBufQueOut( (Ptr_GrBufQue)m_CmdQue );
			// command exist
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::LcDoPlay( void )
{
		m_RunMode		=	E_GrDvrFs2PlayRunModePlay;
		m_PlaySec		=	0;		// play time reset
		m_PlayTick	=	GrTimeGetTick();		// update play tick
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::LcJumpByTime( Def_GrTime A_Time, BOOL8 A_IsAfterPlay )
{
		m_IsSeekCnt	=	FALSE;
		m_IsSeekAfterPlay	=	A_IsAfterPlay;
		m_SeekTime	=	A_Time;
		if ( m_IsDirBack )
		{
			SeekByMinBw( m_SeekTime );
		}
		else
		{
			SeekByMinFw( m_SeekTime );
		}
		m_RunMode		=	E_GrDvrFs2PlayRunModeSeek;
		LcPableReset();
		m_PlaySec			=	0;
		m_PlayTick		=	GrTimeGetTick();

#ifdef DAMGI_DBG
		m_DbgSeekCnt	=	0;
#endif
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::LcVdoChEnSet( __u16 A_Map )
{
	// local -------------------
		__u8	Tv_Ch;
		__u16	Tv_MaskCh;
		__u8	Tv_EnChCnt;
	// code --------------------
		Tv_MaskCh		=	1;
		Tv_EnChCnt	=	0;
		for(Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++)
		{
			if ( 0 != (Tv_MaskCh & A_Map) )
			{
				m_ChTbl[Tv_Ch].IsVdoAble	=	TRUE;
				Tv_EnChCnt ++;
			}
			else
			{
				m_ChTbl[Tv_Ch].IsVdoAble	=	FALSE;
				m_ChTbl[Tv_Ch].IsPable		=	FALSE;
				m_ChTbl[Tv_Ch].Res				=	0xFF;
				m_ChTbl[Tv_Ch].Speed			=	0xFF;
				m_ChTbl[Tv_Ch].Pwr				=	0;
			}
			// next
			Tv_MaskCh	=	Tv_MaskCh << 1;
		}

		m_VdoEnChCnt	=	Tv_EnChCnt;
		// update speed parameter
		LcSpdParaUpdt();
		LcDispParaUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::LcAdoChEnSet( __u16 A_Map )
{
	// local -------------------
		__u8	Tv_Ch;
		__u16	Tv_MaskCh;
	// code --------------------
		Tv_MaskCh	=	1;
		for(Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++)
		{
			if ( 0 != (Tv_MaskCh & A_Map) )
			{
				m_ChTbl[Tv_Ch].IsAdoAble	=	TRUE;
			}
			else
			{
				m_ChTbl[Tv_Ch].IsAdoAble	=	FALSE;
			}
			// next
			Tv_MaskCh	=	Tv_MaskCh << 1;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::LcIonlyChEnSet( __u16 A_Map )
{
	// local -------------------
		__u8	Tv_Ch;
		__u16	Tv_MaskCh;
	// code --------------------
		Tv_MaskCh	=	1;
		for(Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++)
		{
			if ( 0 != (Tv_MaskCh & A_Map) )
			{
				m_ChTbl[Tv_Ch].IsIonly	=	TRUE;
				m_ChTbl[Tv_Ch].IsPable	=	FALSE;
			}
			else
			{
				m_ChTbl[Tv_Ch].IsIonly	=	FALSE;
			}
			// next
			Tv_MaskCh	=	Tv_MaskCh << 1;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::LcRunModeNext( void )
{
	// local -------------------
		BOOL8	Tv_IsNext;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		BOOL8	Tv_IsPableReset;
		BOOL8	Tv_IsDecAble;
		__u8	Tv_Ch;
		__u8	Tv_SeqId;
		__u8	Tv_DecRslt;
	// code --------------------
		// init
		Tv_IsNext	=	TRUE;		// next frame able 
		// get frame pointer
		Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)m_PtrFrm;

		// check frame exist
		if ( NULL != Tv_PtrAvHd )
		{
			// update time
			m_PlaySec		=	Tv_PtrAvHd->TimeSec;
			m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
			m_IsSmTime	=	Tv_PtrAvHd->IsSm;		// update summer time

			// check stream type
			if ( (E_GrDvrAvTypVdoP >= Tv_PtrAvHd->Type) || (E_GrDvrAvTypVdoB == Tv_PtrAvHd->Type) )
			{
				// video stream
				Tv_Ch						=	Tv_PtrAvHd->Ch;
				Tv_IsPableReset	=	TRUE;
				Tv_IsDecAble		=	FALSE;

				// check able channel
				if ( m_ChTbl[Tv_Ch].IsVdoAble )
				{
					// check decode able
					if ( E_GrDvrAvTypVdoI == Tv_PtrAvHd->Type )
					{
						Tv_IsDecAble	=	TRUE;
					}
					else if ( m_ChTbl[Tv_Ch].IsPable )
					{
						// check sequence id
						Tv_SeqId	=	m_ChTbl[Tv_Ch].SeqId + 1;
						if ( Tv_SeqId == Tv_PtrAvHd->SeqId )
						{
							Tv_IsDecAble	=	TRUE;
						}
					}

					// check decode able
					if ( Tv_IsDecAble )
					{
						Tv_IsPableReset	=	FALSE;
						// decode
						Tv_DecRslt	=	DecVdo(Tv_PtrAvHd, TRUE);
						if ( E_GrDvrFs2PlayDecOk == Tv_DecRslt )
						{
							// update
							m_ChTbl[Tv_Ch].SeqId	=	Tv_PtrAvHd->SeqId;
							//m_PlaySec				=	Tv_PtrAvHd->TimeSec;
							//m_PlayMsec			=	Tv_PtrAvHd->TimeMsec;
							Tv_IsPableReset	=	FALSE;
							m_ChTbl[Tv_Ch].IsPable	=	TRUE;
							m_IsDecoded	=	TRUE;
						}

						if ( E_GrDvrFs2PlayDecFail == Tv_DecRslt )
						{
							Tv_IsPableReset	=	TRUE;
						}

						if ( E_GrDvrFs2PlayDecBusy != Tv_DecRslt )
						{
							// update parameter
							if ( (Tv_PtrAvHd->SkipCnt != m_ChTbl[Tv_Ch].Speed) || (Tv_PtrAvHd->Res != m_ChTbl[Tv_Ch].Res) )
							{
								// update play speed
								LcSpdChUpdt( Tv_Ch, Tv_PtrAvHd->Res, Tv_PtrAvHd->SkipCnt );
								LcSpdParaUpdt();
							}

							// go next frame
							Tv_IsNext	=	TRUE;
							m_ChTbl[Tv_Ch].DropPos	=	m_DropCnt;	// update drop
							m_PlayStat	=	E_GrDvrPlayStatPlay;	// update status
						}
					}

				}	//	if ( m_ChTbl[Tv_Ch].IsVdoAble )
				// p frame able reset
				if ( Tv_IsPableReset )
				{
					m_ChTbl[Tv_PtrAvHd->Ch].IsPable	=	FALSE;
				}

			}	//	if ( (E_GrDvrAvTypVdoP >= Tv_PtrAvHd->Type) || (E_GrDvrAvTypVdoB == Tv_PtrAvHd->Type) )

		}	//	if ( NULL != Tv_PtrAvHd )
		// check go next 
		if ( Tv_IsNext )
		{
			FrmNext();
			m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
		}

		return	(BOOL8)Tv_IsNext;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Play::LcRunModePrev( void )
{
	// local -------------------
		BOOL8	Tv_IsNext;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		BOOL8	Tv_IsPableReset;
		BOOL8	Tv_IsDropFrm;
		__u8	Tv_Ch;
		__u8	Tv_DecRslt;
	// code --------------------
		// init
		Tv_IsNext	=	TRUE;		// next frame able 
		// get frame pointer
		Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)m_PtrFrm;
		// check frame exist
		if ( NULL != Tv_PtrAvHd )
		{
			// update time
			m_PlaySec		=	Tv_PtrAvHd->TimeSec;
			m_PlayMsec	=	Tv_PtrAvHd->TimeMsec;
			m_IsSmTime	=	Tv_PtrAvHd->IsSm;		// update summer time

			// check stream type
			if ( (E_GrDvrAvTypVdoP >= Tv_PtrAvHd->Type) || (E_GrDvrAvTypVdoB == Tv_PtrAvHd->Type) )
			{
				Tv_Ch						=	Tv_PtrAvHd->Ch;
				Tv_IsPableReset	=	TRUE;
				// check able channel
				if ( m_ChTbl[Tv_Ch].IsVdoAble )
				{
					// check frame drop
					Tv_IsDropFrm	=	TRUE;
					// check i frame only
					if ( E_GrDvrAvTypVdoI == Tv_PtrAvHd->Type )
					{
						// decode
						Tv_DecRslt	=	DecVdo(Tv_PtrAvHd, TRUE);
						if ( E_GrDvrFs2PlayDecOk == Tv_DecRslt )
						{
							// update
							m_ChTbl[Tv_Ch].SeqId	=	Tv_PtrAvHd->SeqId;
							//m_PlaySec				=	Tv_PtrAvHd->TimeSec;
							//m_PlayMsec			=	Tv_PtrAvHd->TimeMsec;
							Tv_IsPableReset	=	FALSE;
							m_ChTbl[Tv_Ch].IsPable	=	TRUE;
							m_IsDecoded	=	TRUE;
						}

						if ( E_GrDvrFs2PlayDecBusy != Tv_DecRslt )
						{
							// update parameter
							if ( (Tv_PtrAvHd->SkipCnt != m_ChTbl[Tv_Ch].Speed) || (Tv_PtrAvHd->Res != m_ChTbl[Tv_Ch].Res) )
							{
								// update play speed
								LcSpdChUpdt( Tv_Ch, Tv_PtrAvHd->Res, Tv_PtrAvHd->SkipCnt );
								LcSpdParaUpdt();
							}
							// go next frame
							Tv_IsNext	=	TRUE;
							m_ChTbl[Tv_Ch].DropPos	=	m_DropCnt;	// update drop
							m_PlayStat	=	E_GrDvrPlayStatPlay;	// update status
						}
						
					}	//	if ( E_GrDvrAvTypVdoI == Tv_PtrAvHd->Type )
				}
				// p frame able reset
				if ( Tv_IsPableReset )
				{
					m_ChTbl[Tv_PtrAvHd->Ch].IsPable	=	FALSE;
				}
			}

		}	//	if ( NULL != Tv_PtrAvHd )
		// check go next 
		if ( Tv_IsNext )
		{
			FrmPrev();
			m_RunMode	=	E_GrDvrFs2PlayRunModeWait;
		}

		return	(BOOL8)Tv_IsNext;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Play::DecVdo( Ptr_GrDvrAvHd A_PtrHd, BOOL8 A_IsDisp )
{
		return	E_GrDvrFs2PlayDecFail;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Play::DecAdo( Ptr_GrDvrAvHd A_PtrHd )
{
		return	E_GrDvrFs2PlayDecFail;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Play::NoTimeSyncSet( BOOL8 A_IsNoSync )
{
	// local -------------------
		Ptr_GrDvrFs2PlayCmdItem	Tv_PtrCmd;
	// code --------------------
		Tv_PtrCmd	=	(Ptr_GrDvrFs2PlayCmdItem)GrBufQueGetInPtr( (Ptr_GrBufQue)m_CmdQue );
		if ( NULL != Tv_PtrCmd )
		{
			Tv_PtrCmd->Cmd		=	E_GrDvrFs2PlayCmdNoTimeSync;
			Tv_PtrCmd->Bt			=	A_IsNoSync;
			GrBufQueIn( (Ptr_GrBufQue)m_CmdQue );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Play::NoTimeSyncSet - command queue is full!\n" );
		}
}
//--------------------------------------------------------------------
