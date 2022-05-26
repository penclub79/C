/*
 GAUSE file system version 3 DEMUX

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include <Dvr/Fs/V4/GrDvrFs4Dmux.h>

#include	<stdlib.h>
#include <Dvr/Fs/V4/GrDvrFs4Disk.h>

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
Cls_GrDvrFs4Dmux::Cls_GrDvrFs4Dmux( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFs4Mng* A_FsMng, BOOL8 A_IsSafe ):
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		//init
		m_TaskSvr	=	A_TaskSvr;
		// m_MsgAloc	=	GrTaskSvrMsgCmdAlloc();

		//init
		m_FsMng						=	A_FsMng;
		m_IsDmuxSafe					=	A_IsSafe;

		m_DmuxMode				=	E_GrDvrFs4DmuxModeAll;

		m_DmuxDevType			=	E_GrDvrRecDevNoraml;

		GrDumyZeroMem(&m_DmuxSeekCtrl, sizeof(m_DmuxSeekCtrl));

		m_DmuxJmpStep			=	0;
		GrDumyZeroMem(m_DmuxChTbl, sizeof(m_DmuxChTbl));
		m_DmuxVdoAbleChCnt	=	0;
		m_DmuxJmpPrgs			=	0;

		m_DmuxTryCnt			=	E_GrDvrFs4DmuxTryDflt;

		m_DmuxJmpTmLast		=	0;
		m_DmuxSeekTmLast	=	0;

}
//--------------------------------------------------------------------
Cls_GrDvrFs4Dmux::~Cls_GrDvrFs4Dmux()
{
	// local -------------------
	// code --------------------

		
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::LcDmuxMoveOneFrmFw(void)
{
	// local -------------------
		__u8	Tv_Result;	// move success
		__s32	Tv_FrmTtSize;
		Ptr_GrDvrFrmBase	Tv_PtrFrmBase;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4DmuxRsltDataEmpty;
		// check data exist
		if (0 != m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Readed)
		{
			Tv_Result	=	E_GrDvrFs4DmuxRsltEndOfData;
			if ((m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs + sizeof(St_GrDvrFrmUnknown)) <= m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Readed)
			{
				// check frame size
				Tv_FrmTtSize	=	E_GrDvrFrmAlignSize;
				Tv_PtrFrmBase	=	(Ptr_GrDvrFrmBase)((__u32)m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Ptr + m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs);
				if (E_GrDvrFrmHeadFcc == Tv_PtrFrmBase->Fcc)
				{
					Tv_FrmTtSize	=	(sizeof(St_GrDvrFrmUnknown)+ Tv_PtrFrmBase->DataSize + sizeof(St_GrDvrFrmTail) + E_GrDvrFrmAlignSize - 1) & E_GrDvrFrmAlignMask;
					if (Tv_FrmTtSize < sizeof(St_GrDvrFrmUnknown))
					{
						Tv_FrmTtSize	=	E_GrDvrFrmAlignSize;
					}
				}
				// check frame size able
				if ((Tv_FrmTtSize + m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs) <= m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Readed)
				{
					// move next position
					m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs	=	m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs + Tv_FrmTtSize;
					Tv_Result	=	E_GrDvrFs4DmuxRsltOk;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::LcDmuxLoadFrmUnitFw(void)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4DmuxRsltBusy;

		if (EvtBeforeLoadFw())
		{
			// try load forward
			if (m_IsDmuxSafe)
			{
				Tv_Rval	=	m_FsMng->SafeReadFw(m_DmuxDevType, &m_DmuxSeekCtrl);
			}
			else
			{
				Tv_Rval	=	m_FsMng->ImmReadFw(m_DmuxDevType, &m_DmuxSeekCtrl);
			}

			if (E_GrDvrFs4RsltOk == Tv_Rval)
			{
				Tv_Result	=	E_GrDvrFs4DmuxRsltOk;
			}
			else
			{
				Tv_Result	=	E_GrDvrFs4DmuxRsltEndOfData;
			}

			EvtAfterLoadFw();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::LcDmuxGetNowFrm(Ptr_GrDvrFrmBase* A_PtrRtFrmPtr)
{
	// local -------------------
		__u8	Tv_Result;
		Ptr_GrDvrFrmBase	Tv_PtrFrmBase;
		__s32	Tv_FrmSize;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4DmuxRsltEndOfData;

		if (m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs < m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Readed)
		{
			Tv_PtrFrmBase	=	(Ptr_GrDvrFrmBase)((__u32)m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Ptr + m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs);
			// check valid frame
			Tv_Result	=	E_GrDvrFs4DmuxRsltBadFrame;
			if ((E_GrDvrFrmHeadFcc == Tv_PtrFrmBase->Fcc) && (0 < Tv_PtrFrmBase->DataSize) && (E_GrDvrMaxChCnt > Tv_PtrFrmBase->Ch))
			{
				// get size
				Tv_Result	=	E_GrDvrFs4DmuxRsltEndOfData;
				Tv_FrmSize	=	(sizeof(St_GrDvrFrmUnknown)+ Tv_PtrFrmBase->DataSize + (E_GrDvrFrmAlignSize - 1) + sizeof(St_GrDvrFrmTail)) & E_GrDvrFrmAlignMask;
				if ((Tv_FrmSize + m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs) <= m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Readed)
				{
					Tv_Result		=	E_GrDvrFs4DmuxRsltOk;
					*A_PtrRtFrmPtr	=	Tv_PtrFrmBase;
					// update last seek time
					if (0 != Tv_PtrFrmBase->TimeSec)
					{
						m_DmuxSeekTmLast	=	Tv_PtrFrmBase->TimeSec;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::LcDmuxSeekFrmUnitFw(void)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_RtCode;
		__u8	Tv_NtSeqId;
		__u8	Tv_IsNrmP;		// normal P frame
		__u32	Tv_TryCnt;
		Ptr_GrDvrFrmBase	Tv_PtrFrmBase;
	// code --------------------
		// init
		Tv_Result	=	E_GrDvrFs4DmuxRsltDataEmpty;

		for (Tv_TryCnt=0; Tv_TryCnt < m_DmuxTryCnt; Tv_TryCnt++)
		{
			// go next frame
			Tv_RtCode	=	LcDmuxMoveOneFrmFw();
			if (E_GrDvrFs4DmuxRsltOk == Tv_RtCode)
			{
				// get now frame
				Tv_RtCode	=	LcDmuxGetNowFrm(&Tv_PtrFrmBase);
				if (E_GrDvrFs4DmuxRsltOk == Tv_RtCode)
				{
					Tv_RtCode	=	E_GrDvrFs4DmuxRsltUselessFrame;
					// check frame type
					if ((E_GrDvrFrmTypeVdoI <= Tv_PtrFrmBase->Type) && (E_GrDvrFrmTypeVdoB >= Tv_PtrFrmBase->Type))
					{
						// mpeg video
						// check match video frame type
						if ((m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoFmt != (E_GrDvrVdoFmtResValMask & ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->VdoFmt)) || 
							(m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSigFps != ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SigFps) || 
							(m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipCnt != ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SkipCnt) ||
							(m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoRefType != ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->RefType) )
						{
							// update video format
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoFmt			=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->VdoFmt & E_GrDvrVdoFmtResValMask;
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSigFps		=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SigFps;
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipCnt	=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SkipCnt;
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoRefType	=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->RefType;
							// reset data
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP		=	FALSE;
							// notify video foramt change
							EvtVdoFmtChg(Tv_PtrFrmBase->Ch);
						}

						// check able video
						if (m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoAble)
						{
							// check frame type
							if (E_GrDvrFrmTypeVdoI == Tv_PtrFrmBase->Type)
							{
								// I frame
								// update skip count
								if (m_DmuxSkipCnt <= m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipPrgs)
								{
									m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipPrgs		=	0;
								}

								if (0 == m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipPrgs)
								{
									m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSeqId		=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SeqId;
									m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP		=	TRUE;
									if (E_GrDvrFs4DmuxModeCntSkip <= m_DmuxMode)
									{
										m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP	=	FALSE;
									}
									m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipPrgs ++;
									m_DmuxJmpPrgs ++;	// increase jump mode progress
									Tv_RtCode	=	E_GrDvrFs4DmuxRsltOk;
								}
								else
								{
									m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP	=	FALSE;
								}
							}
							else // if (E_GrDvrFrmTypeVdoI == Tv_PtrFrmBase->Type)
							{
								// P frame
								if (m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP)
								{
									// check frame type
									Tv_IsNrmP	=	TRUE;

									// interlace frame
									if (0 != (E_GrDvrVdoFmtResInterlace & ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->VdoFmt))
									{
										if (E_GrDvrFs4DmuxModeInterlace <= m_DmuxMode)
										{
											Tv_IsNrmP	=	FALSE;
											if (0 == (E_GrDvrVdoFmtResEvenBit & ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->VdoFmt))
											{
												Tv_NtSeqId	=	m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSeqId + 2;
												if (Tv_NtSeqId == ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SeqId)
												{
													m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSeqId		=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SeqId;
													Tv_RtCode	=	E_GrDvrFs4DmuxRsltOk;
												}
												else
												{
													m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP	=	FALSE;
												}
											}
										}
									}

									// reference 2x frame
									if (2 == ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->RefType)
									{
										if (E_GrDvrFs4DmuxModeRef2x <= m_DmuxMode)
										{
											Tv_IsNrmP		=	FALSE;
											Tv_NtSeqId	=	m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSeqId + 2;
											if (Tv_NtSeqId == ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SeqId)
											{
												m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSeqId		=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SeqId;
												Tv_RtCode	=	E_GrDvrFs4DmuxRsltOk;
											}
										}
									}

									// reference 4x frame
									if (4 == ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->RefType)
									{
										if (E_GrDvrFs4DmuxModeRef2x == m_DmuxMode)
										{
											Tv_IsNrmP		=	FALSE;
											Tv_NtSeqId	=	m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSeqId + 2;
											if (Tv_NtSeqId == ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SeqId)
											{
												m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSeqId		=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SeqId;
												Tv_RtCode	=	E_GrDvrFs4DmuxRsltOk;
											}
										}
										else if (E_GrDvrFs4DmuxModeRef4x == m_DmuxMode)
										{
											Tv_IsNrmP		=	FALSE;
											Tv_NtSeqId	=	m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSeqId + 4;
											if (Tv_NtSeqId == ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SeqId)
											{
												m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSeqId		=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SeqId;
												Tv_RtCode	=	E_GrDvrFs4DmuxRsltOk;
											}
										}
									}

									if (Tv_IsNrmP)
									{
										// all frame mode
										Tv_NtSeqId	=	m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSeqId + 1;
										if (Tv_NtSeqId == ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SeqId)
										{
											m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSeqId		=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SeqId;
											Tv_RtCode	=	E_GrDvrFs4DmuxRsltOk;
										}
										else
										{
											m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP	=	FALSE;
										}
									}

								}	//	if (m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP)
							}	// else .. if (E_GrDvrFrmTypeVdoI == Tv_PtrFrmBase->Type)
						}	
						else  //	if (m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoAble)
						{
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP	=	FALSE;
						}	
						// update skip count
						if (0 != m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipPrgs)
						{
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipPrgs ++;
						}
					}
					else if (E_GrDvrFrmTypeAdo == Tv_PtrFrmBase->Type)
					{
						// audio
						if (m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsAdoAble)
						{
							Tv_RtCode	=	E_GrDvrFs4DmuxRsltOk;
						}
					}
					else if (E_GrDvrFrmTypeMoton == Tv_PtrFrmBase->Type)
					{
						// motion map
						if (m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsMtnAble)
						{
							Tv_RtCode	=	E_GrDvrFs4DmuxRsltOk;
						}
					}
				}	//	Tv_RtCode	=	LcDmuxGetNowFrm(&Tv_PtrFrmBase);
			}	//	Tv_RtCode	=	LcDmuxMoveOneFrmFw();

			// check found
			if (E_GrDvrFs4DmuxRsltOk == Tv_RtCode)
			{
				// found
				m_PtrDmuxFrm	=	Tv_PtrFrmBase;	// update demuxed frame
				break;
			}

			// go next
			if (E_GrDvrFs4DmuxRsltDataEmpty == Tv_RtCode)
			{
				// frame not exist
				break;
			}

			if (E_GrDvrFs4DmuxRsltEndOfData == Tv_RtCode)
			{
				// load next
				Tv_Result	=	LcDmuxLoadFrmUnitFw();
				if ((E_GrDvrFs4DmuxRsltBusy == Tv_Result) || (E_GrDvrFs4DmuxRsltEndOfData == Tv_Result))
				{
					break;
				}
			}
			else 
			{
				// check unknown code
				if (E_GrDvrFs4DmuxRsltUselessFrame != Tv_RtCode)
				{
					DbgMsgPrint("Cls_GrDvrFs4Dmux::LcDmuxSeekFrmUnitFw - Unknown result code(%d).(Hndl:%08X%08X,readed:%8X,prgs:%8X)\n",
						Tv_RtCode, (__u32)(m_DmuxSeekCtrl.Hndl >> 32), (__u32)(m_DmuxSeekCtrl.Hndl),
						m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Readed, m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs);
				}
			}

		}	//	for (Tv_TryCnt=0; Tv_TryCnt < m_DmuxTryCnt;Tv_TryCnt++)

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::DmuxGoFw(void)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		// init
		m_PtrDmuxFrm	=	NULL;
		Tv_Result	=	E_GrDvrFs4DmuxRsltDataEmpty;
		// check mode
		if ((E_GrDvrFs4DmuxModeCntSkip >= m_DmuxMode) || (m_DmuxJmpPrgs < m_DmuxVdoAbleChCnt))
		{
			Tv_Result	=	LcDmuxSeekFrmUnitFw();
		}
		else if (E_GrDvrFs4DmuxModeTimeJmp == m_DmuxMode)
		{
			Tv_Result	=	LcDmuxSeekTimeUnitFw();
		}
		else
		{
			// unknown mode
			DbgMsgPrint("Cls_GrDvrFs4Dmux::DmuxGoFw - unknown demux mode (%d)\n", m_DmuxMode);
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Dmux::EvtBeforeLoadFw(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Dmux::EvtAfterLoadFw(void)
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Dmux::EvtBeforeLoadBw(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Dmux::EvtAfterLoadBw(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Dmux::EvtVdoFmtChg(__u8 A_Ch)
{

}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::LcDmuxSeekTimeUnitFw(void)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_Rval;
		Def_GrTime	Tv_TmTg;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4DmuxRsltDataEmpty;

		if (0 != m_DmuxSeekTmLast)
		{
			Tv_TmTg		=	m_DmuxSeekTmLast + m_DmuxJmpStep;
			// compare jump time
			if (0 != m_DmuxJmpTmLast)
			{
				if (m_DmuxSeekTmLast <= m_DmuxJmpTmLast)
				{
					Tv_TmTg	=	m_DmuxJmpTmLast + (m_DmuxJmpStep * 2);
				}
			}
			// load
			Tv_Result	=	E_GrDvrFs4DmuxRsltBusy;

			if (EvtBeforeLoadFw())
			{
				// try load forward
				if (m_IsDmuxSafe)
				{
					Tv_Rval	=	m_FsMng->SafeReadTimeFw(m_DmuxDevType, &m_DmuxSeekCtrl,Tv_TmTg);
				}
				else
				{
					Tv_Rval	=	m_FsMng->ImmReadTimeFw(m_DmuxDevType, &m_DmuxSeekCtrl, Tv_TmTg);
				}

				if (E_GrDvrFs4RsltOk == Tv_Rval)
				{
					Tv_Result	=	E_GrDvrFs4DmuxRsltOk;
				}
				else
				{
					Tv_Result	=	E_GrDvrFs4DmuxRsltEndOfData;
				}

				EvtAfterLoadFw();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Dmux::DmuxModeSet(__u8 A_Mode, __u32 A_JmpStep, __u32 A_FrmSkipCnt)
{
	// local -------------------
	// code --------------------
		// update mode
		m_DmuxMode			=	A_Mode;
		m_DmuxJmpStep		=	A_JmpStep;
		m_DmuxSkipCnt		=	A_FrmSkipCnt;

		// reset data
		m_DmuxJmpPrgs		=	0;
		m_DmuxJmpTmLast	=	0;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::DmuxGoBegin(void)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		// init
		m_PtrDmuxFrm	=	NULL;
		Tv_Result	=	E_GrDvrFs4DmuxRsltBusy;
		DmuxSyncResetAllCh();

		// check prepare
		if (EvtBeforeLoadFw())
		{
			// try load forward
			if (m_IsDmuxSafe)
			{
				Tv_Rval	=	m_FsMng->SafeReadBegin(m_DmuxDevType, &m_DmuxSeekCtrl);
			}
			else
			{
				Tv_Rval	=	m_FsMng->ImmReadBegin(m_DmuxDevType, &m_DmuxSeekCtrl);
			}

			if (E_GrDvrFs4RsltOk == Tv_Rval)
			{
				Tv_Result	=	E_GrDvrFs4DmuxRsltOk;
			}
			else
			{
				Tv_Result	=	E_GrDvrFs4DmuxRsltEndOfData;
			}

			EvtAfterLoadFw();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Dmux::DmuxSyncResetAllCh(void)
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx =0; Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++)
		{
			m_DmuxChTbl[Tv_WkIdx].IsVdoCanP	=	FALSE;
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::DmuxGoTimeFw(Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		// init
		m_PtrDmuxFrm	=	NULL;
		Tv_Result	=	E_GrDvrFs4DmuxRsltBusy;
		DmuxSyncResetAllCh();

		// check prepare
		if (EvtBeforeLoadFw())
		{
			// try load forward
			if (m_IsDmuxSafe)
			{
				Tv_Rval	=	m_FsMng->SafeReadTimeFw(m_DmuxDevType, &m_DmuxSeekCtrl,A_Time);
			}
			else
			{
				Tv_Rval	=	m_FsMng->ImmReadTimeFw(m_DmuxDevType, &m_DmuxSeekCtrl, A_Time);
			}

			if (E_GrDvrFs4RsltOk == Tv_Rval)
			{
				Tv_Result	=	E_GrDvrFs4DmuxRsltOk;
			}
			else
			{
				Tv_Result	=	E_GrDvrFs4DmuxRsltEndOfData;
			}

			EvtAfterLoadFw();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::DmuxGoBw(void)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		// init
		m_PtrDmuxFrm	=	NULL;
		Tv_Result	=	E_GrDvrFs4DmuxRsltDataEmpty;
		// check mode
		if ((E_GrDvrFs4DmuxModeCntSkip >= m_DmuxMode) || (m_DmuxJmpPrgs < m_DmuxVdoAbleChCnt))
		{
			Tv_Result	=	LcDmuxSeekFrmUnitBw();
		}
		else if (E_GrDvrFs4DmuxModeTimeJmp == m_DmuxMode)
		{
			Tv_Result	=	LcDmuxSeekTimeUnitBw();
		}
		else
		{
			// unknown mode
			DbgMsgPrint("Cls_GrDvrFs4Dmux::DmuxGoBw - unknown demux mode (%d)\n", m_DmuxMode);
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::LcDmuxSeekFrmUnitBw(void)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_RtCode;
		__u32	Tv_TryCnt;
		Ptr_GrDvrFrmBase	Tv_PtrFrmBase;
	// code --------------------
		// init
		Tv_Result	=	E_GrDvrFs4DmuxRsltDataEmpty;

		for (Tv_TryCnt=0; Tv_TryCnt < m_DmuxTryCnt; Tv_TryCnt++)
		{
			// go next frame
			Tv_RtCode	=	LcDmuxMoveOneFrmBw();
			if (E_GrDvrFs4DmuxRsltOk == Tv_RtCode)
			{
				// get now frame
				Tv_RtCode	=	LcDmuxGetNowFrm(&Tv_PtrFrmBase);
				if (E_GrDvrFs4DmuxRsltOk == Tv_RtCode)
				{
					Tv_RtCode	=	E_GrDvrFs4DmuxRsltUselessFrame;
					// check frame type
					if ((E_GrDvrFrmTypeVdoI <= Tv_PtrFrmBase->Type) && (E_GrDvrFrmTypeVdoB >= Tv_PtrFrmBase->Type))
					{
						// mpeg video

						// check match video frame type
						if ((m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoFmt != (E_GrDvrVdoFmtResValMask & ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->VdoFmt)) ||
							(m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSigFps != ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SigFps) || 
							(m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipCnt != ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SkipCnt) ||
							(m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoRefType != ((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->RefType) )
						{
							// update video format
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoFmt			=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->VdoFmt & E_GrDvrVdoFmtResValMask;
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSigFps		=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SigFps;
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipCnt	=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SkipCnt;
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoRefType	=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->RefType;
							// reset data
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP		=	FALSE;
							// notify video foramt change
							EvtVdoFmtChg(Tv_PtrFrmBase->Ch);
						}

						// check able video
						if (m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoAble)
						{
							// check frame type
							if (E_GrDvrFrmTypeVdoI == Tv_PtrFrmBase->Type)
							{
								// I frame
								// update skip count
								if (m_DmuxSkipCnt <= m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipPrgs)
								{
									m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipPrgs		=	0;
								}

								if (0 == m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipPrgs)
								{
									m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSeqId		=	((Ptr_GrDvrFrmVdo)Tv_PtrFrmBase)->SeqId;
									m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP	=	TRUE;
									if (E_GrDvrFs4DmuxModeCntSkip <= m_DmuxMode)
									{
										m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP	=	FALSE;
									}
									m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipPrgs ++;
									m_DmuxJmpPrgs ++;	// increase jump mode progress
									Tv_RtCode	=	E_GrDvrFs4DmuxRsltOk;
								}
								else
								{
									m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP	=	FALSE;
								}
							}
							else // if (E_GrDvrFrmTypeVdoI == Tv_PtrFrmBase->Type)
							{
								// P frame
								m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP	=	FALSE;
							}	// else .. if (E_GrDvrFrmTypeVdoI == Tv_PtrFrmBase->Type)
						}
						else  //	if (m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoAble)
						{
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsVdoCanP	=	FALSE;
						}
						// update skip count
						if (0 != m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipPrgs)
						{
							m_DmuxChTbl[Tv_PtrFrmBase->Ch].VdoSkipPrgs ++;
						}
					}
					else if (E_GrDvrFrmTypeMoton == Tv_PtrFrmBase->Type)
					{
						// motion map
						if (m_DmuxChTbl[Tv_PtrFrmBase->Ch].IsMtnAble)
						{
							Tv_RtCode	=	E_GrDvrFs4DmuxRsltOk;
						}
					}
				}	//	Tv_RtCode	=	LcDmuxGetNowFrm(&Tv_PtrFrmBase);
			}	//	Tv_RtCode	=	LcDmuxMoveOneFrmFw();

			// check found
			if (E_GrDvrFs4DmuxRsltOk == Tv_RtCode)
			{
				// found
				m_PtrDmuxFrm	=	Tv_PtrFrmBase;	// update demuxed frame
				break;
			}

			// go next
			if (E_GrDvrFs4DmuxRsltDataEmpty == Tv_RtCode)
			{
				// frame not exist
				break;
			}

			if (E_GrDvrFs4DmuxRsltEndOfData == Tv_RtCode)
			{
				// load next
				Tv_Result	=	LcDmuxLoadFrmUnitBw();
				if ((E_GrDvrFs4DmuxRsltBusy == Tv_Result) || (E_GrDvrFs4DmuxRsltEndOfData == Tv_Result))
				{
					break;
				}
			}
			else
			{
				// check unknown code
				if (E_GrDvrFs4DmuxRsltUselessFrame != Tv_RtCode)
				{
					DbgMsgPrint("Cls_GrDvrFs4Dmux::LcDmuxSeekFrmUnitFw - Unknown result code(%d).(Hndl:%08X%08X,readed:%8X,prgs:%8X)\n",
						Tv_RtCode, (__u32)(m_DmuxSeekCtrl.Hndl >> 32), (__u32)(m_DmuxSeekCtrl.Hndl),
						m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Readed, m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs);
				}
			}

		}	//	for (Tv_TryCnt=0; Tv_TryCnt < m_DmuxTryCnt;Tv_TryCnt++)

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::LcDmuxMoveOneFrmBw(void)
{
	// local -------------------
		__u8	Tv_Result;	// move success
		__u32	Tv_FrmTtSize;
		Ptr_GrDvrFrmTail	Tv_PtrFrmTail;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4DmuxRsltDataEmpty;
		// check data exist
		if (0 != m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Readed)
		{
			Tv_Result	=	E_GrDvrFs4DmuxRsltEndOfData;
			if (m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs >= sizeof(St_GrDvrFrmUnknown))
			{
				// check frame size
				// get tail
				Tv_FrmTtSize	=	E_GrDvrFrmAlignSize;
				Tv_PtrFrmTail	=	(Ptr_GrDvrFrmTail)((__u32)m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Ptr + m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs - sizeof(St_GrDvrFrmTail));
				if (E_GrDvrFrmTailFcc == Tv_PtrFrmTail->Fcc)
				{
					Tv_FrmTtSize	=	Tv_PtrFrmTail->Size;
					if (Tv_FrmTtSize < sizeof(St_GrDvrFrmUnknown))
					{
						Tv_FrmTtSize	=	E_GrDvrFrmAlignSize;
					}
				}
				// check frame size able
				if (Tv_FrmTtSize <= m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs)
				{
					// move next position
					m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs	=	m_DmuxSeekCtrl.BufTbl[m_DmuxSeekCtrl.BufIdx].Prgs - Tv_FrmTtSize;
					Tv_Result	=	E_GrDvrFs4DmuxRsltOk;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::LcDmuxLoadFrmUnitBw(void)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4DmuxRsltBusy;

		if (EvtBeforeLoadBw())
		{
			// try load forward
			if (m_IsDmuxSafe)
			{
				Tv_Rval	=	m_FsMng->SafeReadBw(m_DmuxDevType, &m_DmuxSeekCtrl);
			}
			else
			{
				Tv_Rval	=	m_FsMng->ImmReadBw(m_DmuxDevType, &m_DmuxSeekCtrl);
			}

			if (E_GrDvrFs4RsltOk == Tv_Rval)
			{
				Tv_Result	=	E_GrDvrFs4DmuxRsltOk;
			}
			else
			{
				Tv_Result	=	E_GrDvrFs4DmuxRsltEndOfData;
			}

			EvtAfterLoadBw();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::LcDmuxSeekTimeUnitBw(void)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_Rval;
		Def_GrTime	Tv_TmTg;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4DmuxRsltDataEmpty;

		if (0 != m_DmuxSeekTmLast)
		{
			Tv_TmTg		=	m_DmuxSeekTmLast - m_DmuxJmpStep;
			// compare jump time
			if (0 != m_DmuxJmpTmLast)
			{
				if (m_DmuxSeekTmLast >= m_DmuxJmpTmLast)
				{
					Tv_TmTg	=	m_DmuxJmpTmLast - (m_DmuxJmpStep * 2);
				}
			}
			// load
			Tv_Result	=	E_GrDvrFs4DmuxRsltBusy;

			if (EvtBeforeLoadBw())
			{
				// try load forward
				if (m_IsDmuxSafe)
				{
					Tv_Rval	=	m_FsMng->SafeReadTimeBw(m_DmuxDevType, &m_DmuxSeekCtrl, Tv_TmTg);
				}
				else
				{
					Tv_Rval	=	m_FsMng->ImmReadTimeBw(m_DmuxDevType, &m_DmuxSeekCtrl, Tv_TmTg);
				}

				if (E_GrDvrFs4RsltOk == Tv_Rval)
				{
					Tv_Result	=	E_GrDvrFs4DmuxRsltOk;
				}
				else
				{
					Tv_Result	=	E_GrDvrFs4DmuxRsltEndOfData;
				}

				EvtAfterLoadBw();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::DmuxGoRecent(void)
{
	// local -------------------
		__u8	Tv_Result;
		Def_GrTime	Tv_TmSt;
		Def_GrTime	Tv_TmEd;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4DmuxRsltEndOfData;
		// get last time
		if (m_IsDmuxSafe)
		{
			m_FsMng->SafeGroupGetRecTimeRng(m_DmuxDevType, &Tv_TmSt, &Tv_TmEd);
		}
		else
		{
			m_FsMng->ImmGroupGetRecTimeRng(m_DmuxDevType, &Tv_TmSt, &Tv_TmEd);
		}
		if (0 != Tv_TmEd)
		{
			Tv_Result	=	DmuxGoTimeBw(Tv_TmEd - 5);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Dmux::DmuxGoTimeBw(Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		// init
		m_PtrDmuxFrm	=	NULL;
		Tv_Result	=	E_GrDvrFs4DmuxRsltBusy;
		DmuxSyncResetAllCh();

		// check prepare
		if (EvtBeforeLoadBw())
		{
			// try load forward
			if (m_IsDmuxSafe)
			{
				Tv_Rval	=	m_FsMng->SafeReadTimeBw(m_DmuxDevType, &m_DmuxSeekCtrl, A_Time);
			}
			else
			{
				Tv_Rval	=	m_FsMng->ImmReadTimeBw(m_DmuxDevType, &m_DmuxSeekCtrl, A_Time);
			}

			if (E_GrDvrFs4RsltOk == Tv_Rval)
			{
				Tv_Result	=	E_GrDvrFs4DmuxRsltOk;
			}
			else
			{
				Tv_Result	=	E_GrDvrFs4DmuxRsltEndOfData;
			}

			EvtAfterLoadBw();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Dmux::DmuxSyncResetCh(__u8 A_Ch)
{
	// local -------------------
	// code --------------------
		if (E_GrDvrMaxChCnt > A_Ch)
		{
			m_DmuxChTbl[A_Ch].IsVdoCanP	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Dmux::DmuxVdoAbleMapSet(__u32 A_Map)
{
	// local -------------------
		__u8	Tv_WkCh;
		__u32	Tv_MaskCh;
	// code --------------------
		// reset channel table
		m_PtrDmuxFrm		=	NULL;
		m_DmuxVdoAbleChCnt	=	0;
		Tv_MaskCh				=	1;
		for (Tv_WkCh=0; Tv_WkCh < E_GrDvrMaxChCnt;Tv_WkCh++)
		{
			if (0 != (Tv_MaskCh & A_Map))
			{
				if (!m_DmuxChTbl[Tv_WkCh].IsVdoAble)
				{
					m_DmuxChTbl[Tv_WkCh].IsVdoAble	=	TRUE;
					m_DmuxChTbl[Tv_WkCh].IsVdoCanP	=	FALSE;	// resync
				}
				m_DmuxVdoAbleChCnt ++;
			}
			else
			{
				m_DmuxChTbl[Tv_WkCh].IsVdoAble	=	FALSE;
				m_DmuxChTbl[Tv_WkCh].IsVdoCanP	=	FALSE;	// resync
			}
			// next
			Tv_MaskCh	=	Tv_MaskCh << 1;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Dmux::DmuxAdoAbleMapSet(__u32 A_Map)
{
	// local -------------------
		__u8	Tv_WkCh;
		__u32	Tv_MaskCh;
	// code --------------------
		// init
		m_PtrDmuxFrm		=	NULL;
		Tv_MaskCh				=	1;
		for (Tv_WkCh=0; Tv_WkCh < E_GrDvrMaxChCnt; Tv_WkCh++)
		{
			if (0 != (Tv_MaskCh & A_Map))
			{
				m_DmuxChTbl[Tv_WkCh].IsAdoAble	=	TRUE;
			}
			else
			{
				m_DmuxChTbl[Tv_WkCh].IsAdoAble	=	FALSE;
			}
			// next
			Tv_MaskCh	=	Tv_MaskCh << 1;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Dmux::DmuxMtnAbleMapSet(__u32 A_Map)
{
	// local -------------------
		__u8	Tv_WkCh;
		__u32	Tv_MaskCh;
	// code --------------------
		// init
		m_PtrDmuxFrm		=	NULL;
		Tv_MaskCh				=	1;
		for (Tv_WkCh=0; Tv_WkCh < E_GrDvrMaxChCnt; Tv_WkCh++)
		{
			if (0 != (Tv_MaskCh & A_Map))
			{
				m_DmuxChTbl[Tv_WkCh].IsMtnAble	=	TRUE;
			}
			else
			{
				m_DmuxChTbl[Tv_WkCh].IsMtnAble	=	FALSE;
			}
			// next
			Tv_MaskCh	=	Tv_MaskCh << 1;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Dmux::DmuxDevSet(__u8 A_DevType)
{
	// local -------------------
	// code --------------------
		m_DmuxDevType	=	A_DevType;
		
		// buffer rest
		m_DmuxSeekCtrl.Hndl		=	E_GrDvrRecHndlNone;
		m_DmuxSeekCtrl.BufTbl[0].Prgs		=	0;
		m_DmuxSeekCtrl.BufTbl[0].Readed	=	0;
		m_DmuxSeekCtrl.BufTbl[1].Prgs		=	0;
		m_DmuxSeekCtrl.BufTbl[1].Readed	=	0;

		// resync
		DmuxSyncResetAllCh();
}
//--------------------------------------------------------------------
