/*
 GAUSE file system version 5 DEMUX

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>
#include <GrError.h>

#include <Dvr/Fs/Nvt/GrFsNvtBase.h>
#include <Dvr/Fs/Nvt/GrFsNvtDmux.h>
#include <Dvr/Fs/Nvt/GrFsNvtRgrp.h>

#include	<stdlib.h>

//====================================================================
//local const

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

extern	St_GrFsNvtEnv	V_GrFsNvtEnv;

//====================================================================

//--------------------------------------------------------------------
Cls_GrFsNvtDmux::Cls_GrFsNvtDmux(Cls_GrTaskSvr* A_TaskSvr, __u32 A_MaxFrmSize) :
Cls_GrTaskCli()
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		m_TaskSvr	=	A_TaskSvr;

		//init
		m_DmuxIo					=	(Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng;

		// test device fix hdd main
		m_DmuxRunDsub			=	0;

		m_DmuxFrmMaxSize	=	A_MaxFrmSize;

		m_DmuxIskipCnt		=	0;

		GrDumyZeroMem(&m_DmuxSctl, sizeof(m_DmuxSctl));

		m_IsDmuxDirBack		=	FALSE;
		m_DmuxRunStat			=	E_GrDvrPlayStatStop;
		m_DmuxTimeLast		=	0;
		m_DmuxSeekTime		=	0;

		m_PtrDmuxOut			=	NULL;
		m_PtrDmuxPars			=	NULL;

		GrDumyZeroMem(m_DmuxChTbl, sizeof(m_DmuxChTbl));
		
		// test
		for( Tv_WkIdx = 0; Tv_WkIdx < E_GrFsNvtMaxChCnt; Tv_WkIdx++)
		{
			m_DmuxChTbl[Tv_WkIdx].IsVdoAble	=	TRUE;
			m_DmuxChTbl[Tv_WkIdx].IsAdoAble	=	TRUE;
		}

//#ifndef DVR_NVT
#if 1
		// allocate divide frame buffer
		GrDumyZeroMem(&m_DmuxDiv, sizeof(m_DmuxDiv));
		m_DmuxDiv.PtrBuf		=	(Ptr_GrDvrFrmHdBase)malloc(m_DmuxFrmMaxSize);
#endif

}
//--------------------------------------------------------------------
Cls_GrFsNvtDmux::~Cls_GrFsNvtDmux()
{
	// local -------------------
	// code --------------------
		DmuxDevClose();

//#ifndef DVR_NVT
#if 1
		// release divide frame buffer
		if(NULL != m_DmuxDiv.PtrBuf)
		{
			free(m_DmuxDiv.PtrBuf);
			m_DmuxDiv.PtrBuf	=	NULL;
		}
#endif
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::EvtVdoFmtChg(__u8 A_Ch)
{

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::DmuxVdoIfrmSyncAllCh(void)
{
	// local -------------------
		__u32	Tv_ChIdx;
	// code --------------------
		// reset dmuxed frame
		m_PtrDmuxOut		=	NULL;
		m_PtrDmuxPars		=	NULL;

		for( Tv_ChIdx = 0; Tv_ChIdx < E_GrFsNvtMaxChCnt; Tv_ChIdx++)
		{
			m_DmuxChTbl[Tv_ChIdx].IsVdoGopSynced	=	FALSE;
		}
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtDmux::DmuxGoTime(Def_GrTime A_Time)
{
	// local -------------------
	// code --------------------
		// reset dmuxed frame
		m_PtrDmuxOut		=	NULL;
		m_PtrDmuxPars		=	NULL;

		// do seek mode
		if(m_IsDmuxDirBack)
		{
			// try seek backward
			return	LcDmuxBwGoTime(A_Time);
		}
		else
		{
			return	LcDmuxFwGoTime(A_Time);
		}

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtDmux::DmuxGoHndl(__u64 A_Hndl)
{
	// local -------------------
	// code --------------------

		// reset dmuxed frame
		m_PtrDmuxOut		=	NULL;
		m_PtrDmuxPars		=	NULL;

		DmuxDirSet(FALSE);	// direction reset

		return	LcDmuxFwGoHndl(A_Hndl);
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::LcDmuxFwProcPlay(void)
{
	// local -------------------
		__u32	Tv_TryCnt;
		Def_GrErrCode	Tv_ErrCode;

	// code --------------------
		m_PtrDmuxOut	=	NULL;

		Tv_TryCnt	=	E_GrFsNvtDmuxSeekTryCnt;
		while((E_GrDvrRecHndlNone != m_DmuxSctl.Hndl) && (0 != Tv_TryCnt))
		{
			Tv_TryCnt --;

			Tv_ErrCode	=	LcDmuxFwNextFrm();
			if(E_GrErrNone != Tv_ErrCode)
			{
				// check unknown error
				if(E_GrErrEndOfData != Tv_ErrCode)
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwProcPlay - frame parsing unknown error (%d) \n", Tv_ErrCode);
					m_DmuxRunStat	=	E_GrDvrPlayStatEnd;
					break;
				}

				// load data
				if(!IsDataLoadAble())
				{
					break;
				}

				LoadBefore();

				Tv_ErrCode	=	LcDmuxFwNextSctl();
				if(E_GrErrNone == Tv_ErrCode)
				{
					LcDmuxParsFrmUpdt();
				}
				else
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwProcPlay - E_GrDvrPlayStatEnd - 1\n");
					m_DmuxRunStat	=	E_GrDvrPlayStatEnd;
				}
				LoadAfter();

			}

			// check frame type
			if(NULL != m_PtrDmuxPars)
			{
				LcDmuxOutFrmUpdtFw();

				if(NULL != m_PtrDmuxOut)
				{
					m_DmuxTimeLast	=	m_PtrDmuxPars->TimeSec;
					break;
				}
			}

		}	//	while((E_GrDvrRecHndlNone != m_DmuxSctl.Hndl) && (0 != Tv_TryCnt))
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::LcDmuxBwProcPlay(void)
{
	// local -------------------
		__u32	Tv_TryCnt;
		Def_GrErrCode	Tv_ErrCode;

	// code --------------------
		// init
		m_PtrDmuxOut	=	NULL;

		Tv_TryCnt	=	E_GrFsNvtDmuxSeekTryCnt;
		while((E_GrDvrRecHndlNone != m_DmuxSctl.Hndl) && (0 != Tv_TryCnt))
		{
			Tv_TryCnt --;

			Tv_ErrCode	=	LcDmuxBwNextFrm();
			if(E_GrErrNone != Tv_ErrCode)
			{
				// check unknown error
				if(E_GrErrEndOfData != Tv_ErrCode)
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwProcPlay - frame parsing unknown error (%d) \n", Tv_ErrCode);
					m_DmuxRunStat	=	E_GrDvrPlayStatBegin;
					break;
				}

				// load data
				if(!IsDataLoadAble())
				{
					break;
				}

				LoadBefore();

				Tv_ErrCode	=	LcDmuxBwNextSctl();
				if(E_GrErrNone == Tv_ErrCode)
				{
					LcDmuxParsFrmUpdt();
				}
				else
				{
					m_DmuxRunStat	=	E_GrDvrPlayStatBegin;
				}

				LoadAfter();

			}

			// check frame type
			if(NULL != m_PtrDmuxPars)
			{
				LcDmuxOutFrmUpdtBw();
				if(NULL != m_PtrDmuxOut)
				{
					m_DmuxTimeLast	=	m_PtrDmuxPars->TimeSec;
					break;
				}
			}

		}	//	while((E_GrDvrRecHndlNone != m_DmuxSctl.Hndl) && (0 != Tv_TryCnt))

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::DmuxVdoChAbleSet(__u32 A_Map)
{
	// local -------------------
		__u32	Tv_Mask;
		__u32	Tv_ChIdx;
		BOOL8	Tv_IsAble;
	// code --------------------
		Tv_Mask		=	1;
		for( Tv_ChIdx = 0; Tv_ChIdx < E_GrFsNvtMaxChCnt; Tv_ChIdx++)
		{
			Tv_IsAble	=	FALSE;
			if(0 != (Tv_Mask & A_Map))
			{
				Tv_IsAble	=	TRUE;
				if(!m_DmuxChTbl[Tv_ChIdx].IsVdoAble)
				{
					m_DmuxChTbl[Tv_ChIdx].IsVdoGopSynced	=	FALSE;
				}
			}

			// update
			m_DmuxChTbl[Tv_ChIdx].IsVdoAble	=	Tv_IsAble;

			// next
			Tv_Mask	=	Tv_Mask << 1;
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::DmuxAdoChAbleSet(__u32 A_Map)
{
	// local -------------------
		__u32	Tv_Mask;
		__u32	Tv_ChIdx;
	// code --------------------
		Tv_Mask	=	1;
		for(Tv_ChIdx = 0; Tv_ChIdx < E_GrFsNvtMaxChCnt; Tv_ChIdx++)
		{
			if(0 != (Tv_Mask & A_Map))
			{
				m_DmuxChTbl[Tv_ChIdx].IsAdoAble	=	TRUE;
			}
			else
			{
				m_DmuxChTbl[Tv_ChIdx].IsAdoAble	=	FALSE;
			}
			// next
			Tv_Mask	=	Tv_Mask << 1;
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::DmuxVdoFrmMaskSet(__u8 A_Ch, __u8 A_Mask)
{
	// local -------------------
	// code --------------------
		if(m_DmuxChTbl[A_Ch].MaskMode != A_Mask)
		{
			m_DmuxChTbl[A_Ch].MaskMode	=	A_Mask;
			// reset sync
			m_DmuxChTbl[A_Ch].IsVdoGopSynced	=	FALSE;

			if(E_GrFsNvtDmuxFrmMskIonly != m_DmuxChTbl[A_Ch].MaskMode)
			{
				m_DmuxChTbl[A_Ch].VdoMaskPrgs	=	0;		// reset mask progress
			}

		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtDmux::IsDataLoadAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::DmuxDirSet(BOOL8 A_IsDirBack)
{
	// local -------------------
	// code --------------------
		// check diffrent
		if(m_IsDmuxDirBack != A_IsDirBack)
		{
			m_IsDmuxDirBack	=	A_IsDirBack;
			// all video frame resync
			DmuxVdoIfrmSyncAllCh();
		}
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::LoadBefore(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::LoadAfter(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::LcDmuxParsFrmUpdt(void)
{
	// local -------------------
		Ptr_GrDvrFrmHdBase	Tv_PtrHd;
		__u32		Tv_FrmSize;
		void*		Tv_PtrBuf;
	// code --------------------

		m_PtrDmuxPars	=	NULL;
		m_PtrDmuxOut	=	NULL;

		// check load status
		if((E_GrDvrRecHndlNone == m_DmuxSctl.Hndl) || (NULL == m_DmuxDiv.PtrBuf) || (NULL == m_DmuxSctl.PtrBuf))
		{
			return;
		}

		Tv_PtrHd	=	NULL;

		// check divide
		if(m_DmuxDiv.IsOn)
		{
			// check exist divide data
#ifdef E_GrFsDmuxDbgMsg
			if(0 == m_DmuxDiv.FrmSize)
			{
				DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxParsFrmUpdt - on divide , but frame not exist!\n");
				// remove divide frame
				m_DmuxDiv.IsOn	=	FALSE;
				return;
			}
#endif
			// calculate position
			Tv_PtrHd	=	(Ptr_GrDvrFrmHdBase)m_DmuxDiv.PtrBuf;
		}
		else
		{
			// normal position
			// check position
			if((0 > m_DmuxSctl.Prgs) || ((__s32)m_DmuxSctl.InSize <= m_DmuxSctl.Prgs))
			{
				return;
			}
			Tv_PtrHd	=	(Ptr_GrDvrFrmHdBase)((Def_GrCalPtr)m_DmuxSctl.PtrBuf + (Def_GrCalPtr)m_DmuxSctl.Prgs);
		}

		// check header and size
		Tv_FrmSize	=	(Tv_PtrHd->DataSize + sizeof(St_GrDvrFrmHdUnknown)+ sizeof(St_GrDvrFrmTail)+ E_GrDvrFrmAlignSize - 1) & E_GrDvrFrmAlignMask;
		if((E_GrDvrFrmHeadFcc != Tv_PtrHd->Fcc) || (((__u32)m_DmuxSctl.Prgs + Tv_FrmSize) > m_DmuxSctl.InSize))
		{
			return;
		}

		// update progress
		m_PtrDmuxPars	=	Tv_PtrHd;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtDmux::LcDmuxFwNextFrm(void)
{
	// local -------------------
		Ptr_GrDvrFrmHdBase	Tv_PtrHd;
		__u32	Tv_FrmSize;
		__s32	Tv_TryCnt;
	// code --------------------
		// init
		m_PtrDmuxOut	=	NULL;
		m_PtrDmuxPars	=	NULL;

		// check exist buffer
		if((E_GrDvrRecHndlNone == m_DmuxSctl.Hndl) || (NULL == m_DmuxDiv.PtrBuf) || (NULL == m_DmuxSctl.PtrBuf))
		{
			return	E_GrErrInitFail;
		}

		Tv_TryCnt	=	64;
		while(0 < Tv_TryCnt )
		{
			Tv_TryCnt --;

			// prepare move next frame ---------------------------
			if(m_DmuxDiv.IsOn)
			{
				//on divide buffer
				if(m_DmuxDiv.IsTail)
				{
					// tail
					m_DmuxSctl.Prgs	=	m_DmuxSctl.Prgs + (__s32)m_DmuxDiv.FrmSize;
					return	E_GrErrEndOfData;
				}
				else
				{
					// header
					m_DmuxDiv.IsOn		=	FALSE;
					m_DmuxSctl.Prgs		=	m_DmuxSctl.Prgs + (__s32)m_DmuxDiv.FrmSize;
					if(0 > m_DmuxSctl.Prgs)
					{
#ifdef E_GrFsDmuxDbgMsg
						DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwNextFrm - div bad head size (%8X) over!\n", m_DmuxDiv.FrmSize);
#endif
						m_DmuxSctl.Prgs	=	0;
					}
				}
			}
			else
			{
				// normal frame
				// check over position
				if((__s32)m_DmuxSctl.InSize <= m_DmuxSctl.Prgs)
				{
					return	E_GrErrEndOfData;
				}

				Tv_FrmSize		=	E_GrDvrFrmAlignSize;
				// get frame header
				Tv_PtrHd	=	(Ptr_GrDvrFrmHdBase)((Def_GrCalPtr)m_DmuxSctl.PtrBuf + (Def_GrCalPtr)m_DmuxSctl.Prgs);

				// check fcc
				if(E_GrDvrFrmHeadFcc == Tv_PtrHd->Fcc)
				{
					Tv_FrmSize	=	(Tv_PtrHd->DataSize + sizeof(St_GrDvrFrmHdUnknown)+ sizeof(St_GrDvrFrmTail)+ E_GrDvrFrmAlignSize - 1) & E_GrDvrFrmAlignMask;
					if((E_GrDvrFrmAlignSize > Tv_FrmSize) || (m_DmuxFrmMaxSize < Tv_FrmSize))
					{
#ifdef E_GrFsDmuxDbgMsg
						DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwNextFrm - frame size (%8X) over!\n", Tv_FrmSize);
#endif
						Tv_FrmSize	=	E_GrDvrFrmAlignSize;
					}
				}
#ifdef E_GrFsDmuxFccChk
				else
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwNextFrm - bad FCC found!\n");
				}
#endif
				// check over
				if((m_DmuxSctl.Prgs + (__s32)Tv_FrmSize) >= (__s32)m_DmuxSctl.InSize)
				{
					// check divide buffer
					if((0 == m_DmuxDiv.FrmSize) || (!m_DmuxDiv.IsTail))
					{
						// remove divide buffer
						m_DmuxDiv.FrmSize	=	0;
						m_DmuxDiv.IsOn		=	FALSE;
						return	E_GrErrEndOfData;
					}

#ifdef E_GrFsDmuxDbgMsg
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwNextFrm - [BUG] over frame position, but not in divider!\n");
#endif

					// on divide buffer
					m_DmuxDiv.IsOn		=	TRUE;
				}

				// move progress position
				m_DmuxSctl.Prgs		=	m_DmuxSctl.Prgs + (__s32)Tv_FrmSize;
			}

			// check now frame valid  -------------------------
			if(m_DmuxDiv.IsOn)
			{
				// divide frame
				Tv_PtrHd		=	(Ptr_GrDvrFrmHdBase)m_DmuxDiv.PtrBuf;
				if(E_GrDvrFrmHeadFcc == Tv_PtrHd->Fcc)
				{
					Tv_FrmSize	=	(Tv_PtrHd->DataSize + sizeof(St_GrDvrFrmHdUnknown)+ sizeof(St_GrDvrFrmTail)+ E_GrDvrFrmAlignSize - 1) & E_GrDvrFrmAlignMask;
					if((E_GrDvrFrmAlignSize <= Tv_FrmSize) && (m_DmuxDiv.FrmSize >= Tv_FrmSize))
					{
						// found
						m_PtrDmuxPars	=	Tv_PtrHd;
						break;
					}
#ifdef E_GrFsDmuxDbgMsg
					else
					{
						DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwNextFrm - tail divide data size (buf % : frm %) missmatch!\n",m_DmuxDiv.FrmSize, Tv_FrmSize);
					}
#endif
				}
#ifdef E_GrFsDmuxFccChk
				else
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwNextFrm - head bad FCC found!\n");
				}
#endif
			}
			else
			{
				// normal frame
				Tv_PtrHd		=	(Ptr_GrDvrFrmHdBase)((Def_GrCalPtr)m_DmuxSctl.PtrBuf + (Def_GrCalPtr)m_DmuxSctl.Prgs);
				if(E_GrDvrFrmHeadFcc == Tv_PtrHd->Fcc)
				{
					Tv_FrmSize	=	(Tv_PtrHd->DataSize + sizeof(St_GrDvrFrmHdUnknown)+ sizeof(St_GrDvrFrmTail)+ E_GrDvrFrmAlignSize - 1) & E_GrDvrFrmAlignMask;
					if((E_GrDvrFrmAlignSize <= Tv_FrmSize) && (m_DmuxFrmMaxSize >= Tv_FrmSize))
					{
						// check in buffer
						if((m_DmuxSctl.Prgs + Tv_FrmSize) <= m_DmuxSctl.InSize)
						{
							// found
							m_PtrDmuxPars	=	Tv_PtrHd;
							break;
						}

						// check divider buffer
						if((0 != m_DmuxDiv.FrmSize) && m_DmuxDiv.IsTail )
						{
							Tv_PtrHd		=	(Ptr_GrDvrFrmHdBase)m_DmuxDiv.PtrBuf;
							if(E_GrDvrFrmHeadFcc == Tv_PtrHd->Fcc)
							{
								Tv_FrmSize	=	(Tv_PtrHd->DataSize + sizeof(St_GrDvrFrmHdUnknown)+ sizeof(St_GrDvrFrmTail)+ E_GrDvrFrmAlignSize - 1) & E_GrDvrFrmAlignMask;
								if( (E_GrDvrFrmAlignSize <= Tv_FrmSize) && (m_DmuxDiv.FrmSize >= Tv_FrmSize))
								{
									// found
									m_DmuxDiv.IsOn	=	TRUE;
									m_PtrDmuxPars	=	Tv_PtrHd;
#ifdef E_GrFsDmuxDbgMsg
									if(m_DmuxDiv.Prgs != m_DmuxSctl.Prgs)
									{
										DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwNextFrm - divide progress mismatch (sctl % : div %) missmatch!\n", m_DmuxSctl.Prgs, m_DmuxDiv.Prgs);
									}
#endif

									break;
								}
#ifdef E_GrFsDmuxDbgMsg
								else
								{
									DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwNextFrm - tail divide data size (buf % : frm %) missmatch!\n", m_DmuxDiv.FrmSize, Tv_FrmSize);
								}
#endif
							}
#ifdef E_GrFsDmuxFccChk
							else
							{
								DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwNextFrm - head bad FCC found!\n");
							}
#endif
						}	//	if((0 != m_DmuxDiv.FrmSize) && m_DmuxDiv.IsTail && (m_DmuxDiv.Contact == m_DmuxPrgs.Ofs))

						// remove divide buffer
						m_DmuxDiv.FrmSize	=	0;
						m_DmuxDiv.IsOn		=	FALSE;
						return	E_GrErrEndOfData;
					}
#ifdef E_GrFsDmuxDbgMsg
					else
					{
						DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwNextFrm - frame size (%8X) over!\n", Tv_FrmSize);
					}
#endif
				}
#ifdef E_GrFsDmuxFccChk
				else
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwNextFrm - bad FCC found!\n");
				}
#endif

			}

		}	//	while(0 < Tv_TryCnt )

		return	E_GrErrNone;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtDmux::LcDmuxBwNextFrm(void)
{
	// local -------------------
		Ptr_GrDvrFrmHdBase	Tv_PtrHd;
		Ptr_GrDvrFrmTail	Tv_PtrTail;
		__u32	Tv_FrmSize;
		__u32	Tv_TailSize;
		__s32	Tv_TryCnt;
	// code --------------------
		// init
		m_PtrDmuxOut	=	NULL;
		m_PtrDmuxPars	=	NULL;

		// check exist buffer
		if((E_GrDvrRecHndlNone == m_DmuxSctl.Hndl) || (NULL == m_DmuxDiv.PtrBuf) || (NULL == m_DmuxSctl.PtrBuf))
		{
			return	E_GrErrInitFail;
		}

		Tv_TryCnt	=	64;
		while(0 < Tv_TryCnt)
		{
			Tv_TryCnt --;

			// prepare move next frame ---------------------------
			if(m_DmuxDiv.IsOn)
			{
				//on divide buffer
				if(!m_DmuxDiv.IsTail)
				{
					// header
					return	E_GrErrEndOfData;
				}
				else
				{
					// tail
					m_DmuxDiv.IsOn		=	FALSE;
					continue;
				}
			}
			else
			{
				// normal frame
				// check position
				if((__s32)m_DmuxSctl.InSize < m_DmuxSctl.Prgs)
				{
					// patch position
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextFrm - non divide progress positin (%d) bad!\n", m_DmuxSctl.Prgs);
					m_DmuxSctl.Prgs	=	(__s32)m_DmuxSctl.InSize;
				}

				if(E_GrDvrFrmAlignSize > m_DmuxSctl.Prgs)
				{
					return	E_GrErrEndOfData;
				}

				Tv_FrmSize		=	E_GrDvrFrmAlignSize;
				// get tail
				Tv_PtrTail	=	(Ptr_GrDvrFrmTail)((Def_GrCalPtr)m_DmuxSctl.PtrBuf + (Def_GrCalPtr)m_DmuxSctl.Prgs - sizeof(St_GrDvrFrmTail));
				// check fcc
				if(E_GrDvrFrmTailFcc == Tv_PtrTail->Fcc)
				{
					Tv_FrmSize	=	Tv_PtrTail->PktSize;
					if(m_DmuxFrmMaxSize < Tv_FrmSize)
					{
#ifdef E_GrFsDmuxDbgMsg
						DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextFrm - frame size (%8X) over!\n", Tv_FrmSize);
#endif
						Tv_FrmSize	=	E_GrDvrFrmAlignSize;
					}
				}
#ifdef E_GrFsDmuxFccChk
				else
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextFrm - bad tail FCC found!\n");
				}
#endif
				// check under
				if(m_DmuxSctl.Prgs < (__s32)Tv_FrmSize )
				{
					// check exist head divider
					if((0 == m_DmuxDiv.FrmSize) || (m_DmuxDiv.IsTail))
					{
						// remove divide buffer
						m_DmuxDiv.FrmSize	=	0;
						m_DmuxDiv.IsOn		=	FALSE;
						return	E_GrErrEndOfData;
					}
					// on divide buffer
					m_DmuxDiv.IsOn		=	TRUE;
#ifdef E_GrFsDmuxDbgMsg
					if(m_DmuxDiv.Prgs != m_DmuxSctl.Prgs)
					{
						DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextFrm - div mismatch (sctl:%d : div:%d)!\n", m_DmuxSctl.Prgs, m_DmuxDiv.Prgs);
					}
#endif
				}

				// move progress position
				m_DmuxSctl.Prgs		=	m_DmuxSctl.Prgs - (__s32)Tv_FrmSize;
			}

			// check now frame valid  -------------------------
			if(m_DmuxDiv.IsOn)
			{
				// divide frame
				Tv_PtrHd		=	(Ptr_GrDvrFrmHdBase)m_DmuxDiv.PtrBuf;
				if(E_GrDvrFrmHeadFcc == Tv_PtrHd->Fcc)
				{
					Tv_FrmSize	=	(Tv_PtrHd->DataSize + sizeof(St_GrDvrFrmHdUnknown)+ sizeof(St_GrDvrFrmTail)+ E_GrDvrFrmAlignSize - 1) & E_GrDvrFrmAlignMask;
					if(m_DmuxDiv.FrmSize >= Tv_FrmSize)
					{
						// found
						m_PtrDmuxPars	=	Tv_PtrHd;
						break;
					}
#ifdef E_GrFsDmuxDbgMsg
					else
					{
						DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextFrm - tail divide data size (buf % : frm %) missmatch!\n", m_DmuxDiv.FrmSize, Tv_FrmSize);
					}
#endif
				}
#ifdef E_GrFsDmuxFccChk
				else
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextFrm - tail bad FCC found!\n");
				}
#endif
			}
			else
			{
				// normal frame
				Tv_TailSize	=	Tv_FrmSize;		// backup tail size
				Tv_PtrHd		=	(Ptr_GrDvrFrmHdBase)((Def_GrCalPtr)m_DmuxSctl.PtrBuf + (Def_GrCalPtr)m_DmuxSctl.Prgs);
				if(E_GrDvrFrmHeadFcc == Tv_PtrHd->Fcc)
				{
					Tv_FrmSize	=	(Tv_PtrHd->DataSize + sizeof(St_GrDvrFrmHdUnknown)+ sizeof(St_GrDvrFrmTail)+ E_GrDvrFrmAlignSize - 1) & E_GrDvrFrmAlignMask;
					if(m_DmuxFrmMaxSize >= Tv_FrmSize)
					{
						// check in buffer
						if((m_DmuxSctl.Prgs + Tv_FrmSize) <= m_DmuxSctl.InSize)
						{
#ifdef E_GrFsDmuxDbgMsg
							if(Tv_TailSize != Tv_FrmSize)
							{
								DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextFrm - not match tail (head:%d tail:%d) size!\n", Tv_FrmSize, Tv_TailSize);
							}
#endif
							// found
							m_PtrDmuxPars	=	Tv_PtrHd;
							break;
						}
						else
						{
#ifdef E_GrFsDmuxDbgMsg
							DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextFrm - bad frame size (head:%d tail:%d)!\n", Tv_FrmSize,Tv_TailSize);
#endif
						}
					}
#ifdef E_GrFsDmuxDbgMsg
					else
					{
						DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextFrm - frame size (%8X) over!\n", Tv_FrmSize);
					}
#endif
				}
#ifdef E_GrFsDmuxFccChk
				else
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextFrm - bad FCC found!\n");
				}
#endif

			}

		}	//	while(0 < Tv_TryCnt)

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtDmux::DmuxGoNext(void)
{
	// local -------------------
	// code --------------------
		// reset dmuxed frame
		m_PtrDmuxOut		=	NULL;
		m_PtrDmuxPars		=	NULL;
	
		// check seek mode
		if(E_GrDvrPlayStatSeek == m_DmuxRunStat)
		{
			// do seek
			if(m_IsDmuxDirBack)
			{
				LcDumxBwProcSeek();
			}
			else
			{
				LcDumxFwProcSeek();
			}
		}
		else if(E_GrDvrPlayStatPlay == m_DmuxRunStat)
		{
			if(m_IsDmuxDirBack)
			{
				LcDmuxBwProcPlay();
			}
			else
			{
				LcDmuxFwProcPlay();
			}
		}
		else if(E_GrDvrPlayStatBegin == m_DmuxRunStat)
		{
			if(m_IsDmuxDirBack)
			{
				// do not run
			}
			else
			{
				// go first and play
				LcDmuxFwGoStart();
			}
		}
		else if(E_GrDvrPlayStatEnd == m_DmuxRunStat)
		{
			if(m_IsDmuxDirBack)
			{
				m_DmuxRunStat	=	E_GrDvrPlayStatPlay;
				LcDmuxBwProcPlay();
			}
			else
			{
				// continue play
				m_DmuxRunStat	=	E_GrDvrPlayStatPlay;
				LcDmuxFwProcPlay();				
			}
		}

		return	m_DmuxRunStat;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtDmux::LcDmuxFwNextSctl(void)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
		__u32	Tv_DivHdSize;
		__u32	Tv_DivTlSize;
		__u32	Tv_FrmSize;
		void*	Tv_PtrDiv;
		__u32	Tv_PrvSeq;
		__s32	Tv_PrvPrgs;
		Ptr_GrDvrFrmHdBase	Tv_PtrHd;
	// code --------------------
		// init
		m_PtrDmuxOut	=	NULL;
		m_PtrDmuxPars	=	NULL;

		m_DmuxDiv.IsOn		=	FALSE;
		m_DmuxDiv.FrmSize	=	0;

		// backup previous status
		Tv_PrvSeq					=	m_DmuxSctl.BlkSeq;
		Tv_PrvPrgs				=	m_DmuxSctl.Prgs;

		// backup header
		if((m_DmuxSctl.InSize >= E_GrFsNvtRgrpCchDataBufSize) && (0 < m_DmuxSctl.Prgs) && (m_DmuxSctl.Prgs < (__s32)E_GrFsNvtRgrpCchDataBufSize))
		{

			// get frame size
			Tv_PtrHd	=	(Ptr_GrDvrFrmHdBase)((Def_GrCalPtr)m_DmuxSctl.PtrBuf + (Def_GrCalPtr)m_DmuxSctl.Prgs);

			// check fcc
			if(E_GrDvrFrmHeadFcc == Tv_PtrHd->Fcc)
			{
				Tv_FrmSize	=	(Tv_PtrHd->DataSize + sizeof(St_GrDvrFrmHdUnknown)+ sizeof(St_GrDvrFrmTail)+ E_GrDvrFrmAlignSize - 1) & E_GrDvrFrmAlignMask;
				if((E_GrDvrFrmAlignSize <= Tv_FrmSize) && (m_DmuxFrmMaxSize >= Tv_FrmSize))
				{
					// make divide data
					Tv_DivHdSize			=	m_DmuxSctl.InSize - (__u32)m_DmuxSctl.Prgs;
#if 1
					GrDumyCopyMem(m_DmuxDiv.PtrBuf, Tv_PtrHd, Tv_DivHdSize);
					Tv_DivTlSize			=	Tv_FrmSize - Tv_DivHdSize;
#else
					m_DmuxDecBuf.DecBuf	= LcDmuxVdecBufGet(Tv_DivHdSize);- m_DmuxDiv.PtrBuf 에 phy메모리 주소를 직접 넣어주어서 변경이 없도록 한다
					GrDumyCopyMem(m_DmuxDecBuf.DecBuf, Tv_PtrHd, Tv_DivHdSize);
					m_DmuxDiv.PtrBuf	= m_DmuxDecBuf.DecBuf;
					Tv_DivTlSize			=	Tv_FrmSize - Tv_DivHdSize;
#endif
					m_DmuxDiv.IsOn		=	TRUE;
					m_DmuxDiv.IsTail	=	FALSE;		// head
					m_DmuxDiv.FrmSize	=	Tv_FrmSize;
				}
				else
				{
					m_DmuxSctl.Prgs		=	m_DmuxSctl.InSize;
#ifdef E_GrFsDmuxDbgMsg
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxReadNext - bad divide!\n");
#endif
				}
			}
		}

		// read next
		Tv_ErrCode	=	m_DmuxIo->ReqReadFwNext(m_DmuxRunDsub, &m_DmuxSctl);
		if(E_GrErrNone != Tv_ErrCode)
		{
			// reset divider
			m_DmuxDiv.IsOn		=	FALSE;
			m_DmuxDiv.FrmSize	=	0;

			DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxReadNext - read next error - insize = %d Prgs = %d\n", m_DmuxSctl.InSize, m_DmuxSctl.Prgs);
			return	Tv_ErrCode;
		}

		// make divide
		if(m_DmuxDiv.IsOn)
		{
			if(Tv_PrvSeq == m_DmuxSctl.BlkSeq)
			{
#if 1
				// copy remain
				Tv_PtrDiv	=	(void*)((Def_GrCalPtr)m_DmuxDiv.PtrBuf + (Def_GrCalPtr)Tv_DivHdSize);
				GrDumyCopyMem(Tv_PtrDiv, m_DmuxSctl.PtrBuf, Tv_DivTlSize);
#else
				Tv_PtrDiv	=	(void*)((Def_GrCalPtr)m_DmuxDecBuf.DecBuf + (Def_GrCalPtr)Tv_DivHdSize);
				GrDumyCopyMem(Tv_PtrDiv, m_DmuxSctl.PtrBuf, Tv_DivTlSize);
#endif

				// patch progress
				m_DmuxDiv.Prgs		=	-((__s32)Tv_DivHdSize);
				m_DmuxSctl.Prgs		=	-((__s32)Tv_DivHdSize);
				
			}
			else
			{
#ifdef E_GrFsDmuxDbgMsg
				DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxReadNext - invalid divide data sequence!\n");
#endif
				m_DmuxDiv.IsOn		=	FALSE;
				m_DmuxDiv.FrmSize	=	0;

				m_DmuxSctl.Prgs		=	0;
			}
		}
		else
		{
			m_DmuxSctl.Prgs	=	0;
			if((__s32)E_GrFsNvtRgrpCchDataBufSize < Tv_PrvPrgs)
			{
				if(m_DmuxSctl.InSize >= ((__u32)Tv_PrvPrgs - E_GrFsNvtRgrpCchDataBufSize))
				{
					if(Tv_PrvSeq == m_DmuxSctl.BlkSeq)
					{
						m_DmuxSctl.Prgs	=	Tv_PrvPrgs - E_GrFsNvtRgrpCchDataBufSize;
					}
#ifdef E_GrFsDmuxDbgMsg
					else
					{
						DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxReadNext - privious data remain!\n");
					}
#endif
				}
#ifdef E_GrFsDmuxDbgMsg
				else
				{
					if(Tv_PrvSeq == m_DmuxSctl.BlkSeq)
					{
						DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxReadNext - divide size over!\n");
					}
				}
#endif
			}
			else
			{
				//jeff code start
				if(Tv_PrvSeq == m_DmuxSctl.BlkSeq)
				{
					m_DmuxSctl.Prgs	= Tv_PrvPrgs; 
				}
				//jeff code end
			}
		}

		return	E_GrErrNone;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtDmux::LcDmuxBwNextSctl(void)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
		__u32	Tv_DivHdSize;
		__u32	Tv_DivTlSize;
		__u32	Tv_FrmSize;
		void*	Tv_PtrDiv;
		__u32	Tv_PrvSeq;
		__s32	Tv_PrvPrgs;
		Ptr_GrDvrFrmTail		Tv_PtrTail;
	// code --------------------
		// init
		m_PtrDmuxOut	=	NULL;
		m_PtrDmuxPars	=	NULL;

		m_DmuxDiv.IsOn		=	FALSE;
		m_DmuxDiv.FrmSize	=	0;

		// backup previous status
		Tv_PrvSeq					=	m_DmuxSctl.BlkSeq;
		Tv_PrvPrgs				=	m_DmuxSctl.Prgs;

		// backup tail
		if((E_GrDvrFrmAlignSize <= m_DmuxSctl.Prgs) && (m_DmuxSctl.Prgs < (__s32)m_DmuxSctl.InSize))
		{
			// get frame size
			Tv_PtrTail	=	(Ptr_GrDvrFrmTail)((Def_GrCalPtr)m_DmuxSctl.PtrBuf + (Def_GrCalPtr)m_DmuxSctl.Prgs - sizeof(St_GrDvrFrmTail));
			// check tail fcc
			if(E_GrDvrFrmTailFcc == Tv_PtrTail->Fcc)
			{
				Tv_FrmSize	=	Tv_PtrTail->PktSize;
				if((E_GrDvrFrmAlignSize <= Tv_FrmSize) && (m_DmuxFrmMaxSize >= Tv_FrmSize) && ((__u32)m_DmuxSctl.Prgs < Tv_FrmSize))
				{
					// make divide data
					m_DmuxDiv.IsOn		=	TRUE;
					m_DmuxDiv.IsTail	=	TRUE;		// tail
					m_DmuxDiv.FrmSize	=	Tv_FrmSize;

					Tv_DivTlSize			=	(__u32)m_DmuxSctl.Prgs;
					Tv_DivHdSize			=	Tv_FrmSize - Tv_DivTlSize;

#if 1
					Tv_PtrDiv					=	(void*)((Def_GrCalPtr)m_DmuxDiv.PtrBuf + Tv_DivHdSize);
					GrDumyCopyMem(Tv_PtrDiv, m_DmuxSctl.PtrBuf, Tv_DivTlSize);
#else
					Tv_PtrDiv					=	(void*)((Def_GrCalPtr)m_DmuxDecBuf.DecBuf + Tv_DivHdSize);
					GrDumyCopyMem(Tv_PtrDiv, m_DmuxSctl.PtrBuf, Tv_DivTlSize);
#endif
				}
				else
				{
					Tv_PrvPrgs	=	0;
#ifdef E_GrFsDmuxDbgMsg
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextSctl - bad divide!\n");
#endif
				}
			}
		}

		// read next
		Tv_ErrCode	=	m_DmuxIo->ReqReadBwNext(m_DmuxRunDsub, &m_DmuxSctl);
		if(E_GrErrNone != Tv_ErrCode)
		{
			// reset divider
			m_DmuxDiv.IsOn		=	FALSE;
			m_DmuxDiv.FrmSize	=	0;

			return	Tv_ErrCode;
		}

		// make divide
		if(m_DmuxDiv.IsOn)
		{
			if((Tv_PrvSeq == m_DmuxSctl.BlkSeq) && (m_DmuxSctl.InSize == E_GrFsNvtRgrpCchDataBufSize))
			{
#if 1
				// copy remain
				Tv_PtrDiv	=	(void*)((Def_GrCalPtr)m_DmuxSctl.PtrBuf + (Def_GrCalPtr)(m_DmuxSctl.InSize - Tv_DivHdSize));
				GrDumyCopyMem(m_DmuxDiv.PtrBuf, Tv_PtrDiv, Tv_DivHdSize);
#else
				m_DmuxDecBuf.DecBuf	= LcDmuxVdecBufGet(Tv_DivHdSize);
				Tv_PtrDiv	=	(void*)((Def_GrCalPtr)m_DmuxSctl.PtrBuf + (Def_GrCalPtr)(m_DmuxSctl.InSize - Tv_DivHdSize));
				GrDumyCopyMem(m_DmuxDecBuf.DecBuf, Tv_PtrDiv, Tv_DivHdSize);
				m_DmuxDiv.PtrBuf	= m_DmuxDecBuf.DecBuf;
#endif

				// patch progress
				m_DmuxDiv.Prgs		=	(__s32)(m_DmuxSctl.InSize - Tv_DivHdSize);
				m_DmuxSctl.Prgs		=	(__s32)(m_DmuxSctl.InSize - Tv_DivHdSize);

			}
			else
			{
#ifdef E_GrFsDmuxDbgMsg
				DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextSctl - invalid divide data sequence!\n");
#endif
				m_DmuxDiv.IsOn		=	FALSE;
				m_DmuxDiv.FrmSize	=	0;

				m_DmuxSctl.Prgs		=	(__s32)m_DmuxSctl.InSize;
			}
		}
		else
		{
			m_DmuxSctl.Prgs	=	(__s32)m_DmuxSctl.InSize;
			if((0 > Tv_PrvPrgs) && ( 0 <= ((__s32)m_DmuxSctl.InSize + Tv_PrvPrgs)))
			{
				if(Tv_PrvSeq == m_DmuxSctl.BlkSeq)
				{
					m_DmuxSctl.Prgs	=	(__s32)E_GrFsNvtRgrpCchDataBufSize + Tv_PrvPrgs;
				}
#ifdef E_GrFsDmuxDbgMsg
				else
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwNextSctl - previous data remain!\n");
				}
#endif
			}
		}

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::LcDmuxOutFrmUpdtFw(void)
{
	// local -------------------
		__u8	Tv_Ch;
		__u8	Tv_Type;
		BOOL8	Tv_IsAble;
		__u8	Tv_SeqNx;
		Ptr_GrDvrFrmHdVdo		Tv_PtrVdo;
	// code --------------------
		if(NULL == m_PtrDmuxPars)
		{
			return;
		}

		Tv_IsAble	=	FALSE;
		Tv_Ch			=	m_PtrDmuxPars->Ch;
		Tv_Type		=	m_PtrDmuxPars->Type;

		// check frame
		Tv_IsAble	=	FALSE;
		if((E_GrDvrFrmTypeVdoI <= m_PtrDmuxPars->Type) && (E_GrDvrFrmTypeVdoP4x >= m_PtrDmuxPars->Type))
		{
			// vidoe frame
			Tv_PtrVdo	=	(Ptr_GrDvrFrmHdVdo)m_PtrDmuxPars;
			// check video able
			if(m_DmuxChTbl[Tv_Ch].IsVdoAble)
			{
				if(E_GrDvrFrmTypeVdoI == Tv_PtrVdo->Base.Type)
				{
					// check i frame progress
					if((0 == m_DmuxChTbl[Tv_Ch].VdoMaskPrgs) || (E_GrFsNvtDmuxFrmMskIonly != m_DmuxChTbl[Tv_Ch].MaskMode))
					{
						// I frame
						Tv_IsAble	=	TRUE;

						// update GOP synced
						m_DmuxChTbl[Tv_Ch].IsVdoGopSynced	=	Tv_IsAble;
						// update resolution
						if((m_DmuxChTbl[Tv_Ch].VdoFmt != Tv_PtrVdo->VdoFmt) || (m_DmuxChTbl[Tv_Ch].VdoFps != Tv_PtrVdo->Fps))
						{
							// do update
							m_DmuxChTbl[Tv_Ch].VdoFmt	=	Tv_PtrVdo->VdoFmt;
							m_DmuxChTbl[Tv_Ch].VdoFps	=	Tv_PtrVdo->Fps;
							// do event
							EvtVdoFmtChg(Tv_Ch);
						}
					}

					// next mask progress
					if(E_GrFsNvtDmuxFrmMskIonly == m_DmuxChTbl[Tv_Ch].MaskMode)
					{
						m_DmuxChTbl[Tv_Ch].VdoMaskPrgs ++;

						if(m_DmuxIskipCnt < m_DmuxChTbl[Tv_Ch].VdoMaskPrgs)
						{
							m_DmuxChTbl[Tv_Ch].VdoMaskPrgs	=	0;
						}
					}

				}
				else
				{
					// P frame
					// check time jump mode and i only mode
					if(E_GrFsNvtDmuxFrmMskIonly != m_DmuxChTbl[Tv_Ch].MaskMode)
					{
						// check gop sync
						if(m_DmuxChTbl[Tv_Ch].IsVdoGopSynced)
						{
							// check sequence id
							switch(m_DmuxChTbl[Tv_Ch].MaskMode)
							{
							case E_GrFsNvtDmuxFrmMskRef2x:
								Tv_SeqNx	=	m_DmuxChTbl[Tv_Ch].VdoOutSeqId + 2;
								break;
							case E_GrFsNvtDmuxFrmMskRef4x:
								Tv_SeqNx	=	m_DmuxChTbl[Tv_Ch].VdoOutSeqId + 4;
								break;
							default:
								Tv_SeqNx	=	m_DmuxChTbl[Tv_Ch].VdoOutSeqId + 1;
								break;
							}

							Tv_IsAble	=	FALSE;
							if(Tv_SeqNx == Tv_PtrVdo->SeqId)
							{
								// check frame type
								Tv_IsAble	=	TRUE;
								if(E_GrFsNvtDmuxFrmMskRef4x == m_DmuxChTbl[Tv_Ch].MaskMode)
								{
									if(E_GrDvrFrmTypeVdoP4x != Tv_Type)
									{
										Tv_IsAble	=	FALSE;
									}
								}
								else if(E_GrFsNvtDmuxFrmMskRef2x == m_DmuxChTbl[Tv_Ch].MaskMode)
								{
									if(E_GrDvrFrmTypeVdoP == Tv_Type)
									{
										Tv_IsAble	=	FALSE;
									}
								}
							}
							else
							{
								if(E_GrFsNvtDmuxFrmMskAll == m_DmuxChTbl[Tv_Ch].MaskMode)
								{
									// need resync
									m_DmuxChTbl[Tv_Ch].IsVdoGopSynced		=	FALSE;
								}
							}

						}	//	if(m_DmuxChTbl[Tv_PtrFrm->Ch].IsVdoGopSynced)
					}
					else
					{
						// need resync
						m_DmuxChTbl[Tv_Ch].IsVdoGopSynced		=	FALSE;
					}
				}

			}	//	if(m_DmuxChTbl[Tv_PtrFrm->Ch].IsVdoAble)

			// video success update
			if(Tv_IsAble)
			{
				// update sequence 
				m_DmuxChTbl[Tv_Ch].VdoOutSeqId		=	Tv_PtrVdo->SeqId;
			}

		}	//	if((E_GrDvrFrmTypeVdoI <= Tv_PtrFrm->Type) && (E_GrDvrFrmTypeVdoP4x >= Tv_PtrFrm->Type))
		else if(E_GrDvrFrmTypeAdo == Tv_Type)
		{
			// audio
			if(((Ptr_GrDvrFrmHdAdo)m_PtrDmuxPars)->IsAble)
			{
				Tv_IsAble	=	m_DmuxChTbl[Tv_Ch].IsAdoAble;
			}
		}

		if(Tv_IsAble)
		{
			m_PtrDmuxOut	=	m_PtrDmuxPars;
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::LcDmuxOutFrmUpdtBw(void)
{
	// local -------------------
		BOOL8	Tv_IsAble;
		__u8	Tv_Ch;
		__u8	Tv_Type;
		Ptr_GrDvrFrmHdVdo		Tv_PtrVdo;

	// code --------------------
		if(NULL == m_PtrDmuxPars)
		{
			return;
		}

		Tv_Ch			=	m_PtrDmuxPars->Ch;
		Tv_Type		=	m_PtrDmuxPars->Type;

		// check frame
		Tv_IsAble	=	FALSE;
		if((E_GrDvrFrmTypeVdoI <= m_PtrDmuxPars->Type) && (E_GrDvrFrmTypeVdoP4x >= m_PtrDmuxPars->Type))
		{
			// vidoe frame
			Tv_PtrVdo	=	(Ptr_GrDvrFrmHdVdo)m_PtrDmuxPars;
			// check video able
			if(m_DmuxChTbl[Tv_Ch].IsVdoAble)
			{
				if(E_GrDvrFrmTypeVdoI == Tv_PtrVdo->Base.Type)
				{
					// check i frame progress
					if((0 == m_DmuxChTbl[Tv_Ch].VdoMaskPrgs) || (E_GrFsNvtDmuxFrmMskIonly != m_DmuxChTbl[Tv_Ch].MaskMode))
					{
						// I frame
						Tv_IsAble	=	TRUE;
						// update GOP synced
						m_DmuxChTbl[Tv_Ch].IsVdoGopSynced	=	Tv_IsAble;
						// update resolution
						if((m_DmuxChTbl[Tv_Ch].VdoFmt != Tv_PtrVdo->VdoFmt) || (m_DmuxChTbl[Tv_Ch].VdoFps != Tv_PtrVdo->Fps))
						{
							// do update
							m_DmuxChTbl[Tv_Ch].VdoFmt	=	Tv_PtrVdo->VdoFmt;
							m_DmuxChTbl[Tv_Ch].VdoFps	=	Tv_PtrVdo->Fps;
							// do event
							EvtVdoFmtChg(Tv_Ch);
						}

					}

					// next mask progress
					if(E_GrFsNvtDmuxFrmMskIonly == m_DmuxChTbl[Tv_Ch].MaskMode)
					{
						m_DmuxChTbl[Tv_Ch].VdoMaskPrgs ++;

						if(m_DmuxIskipCnt < m_DmuxChTbl[Tv_Ch].VdoMaskPrgs)
						{
							m_DmuxChTbl[Tv_Ch].VdoMaskPrgs	=	0;
						}
					}

				}

			}	//	if(m_DmuxChTbl[Tv_PtrFrm->Ch].IsVdoAble)

			// video success update
			if(Tv_IsAble)
			{
				// update sequence 
				m_DmuxChTbl[Tv_Ch].VdoOutSeqId		=	Tv_PtrVdo->SeqId;
			}

		}	//	if((E_GrDvrFrmTypeVdoI <= Tv_PtrFrm->Type) && (E_GrDvrFrmTypeVdoP4x >= Tv_PtrFrm->Type))

		if(Tv_IsAble)
		{
			m_PtrDmuxOut	=	m_PtrDmuxPars;
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::LcDumxFwProcSeek(void)
{
	// local -------------------
		__u32	Tv_TryCnt;
		Def_GrErrCode	Tv_ErrCode;
		__u8	Tv_Ch;
		__u8	Tv_Type;
		BOOL8	Tv_IsAble;
		Ptr_GrDvrFrmHdVdo		Tv_PtrVdo;

	// code --------------------
		m_PtrDmuxOut	=	NULL;

		Tv_TryCnt	=	E_GrFsNvtDmuxSeekTryCnt;
		while((E_GrDvrRecHndlNone != m_DmuxSctl.Hndl) && (0 != Tv_TryCnt))
		{
			Tv_TryCnt --;

			Tv_ErrCode	=	LcDmuxFwNextFrm();
			if(E_GrErrNone != Tv_ErrCode)
			{
				// check unknown error
				if(E_GrErrEndOfData != Tv_ErrCode)
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDumxFwProcSeek - frame parsing unknown error (%d) \n", Tv_ErrCode);
					m_DmuxRunStat	=	E_GrDvrPlayStatEnd;
					break;
				}

				// load data
				Tv_ErrCode	=	LcDmuxFwNextSctl();
				if(E_GrErrNone != Tv_ErrCode)
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwProcPlay - E_GrDvrPlayStatEnd - 2\n");
					m_DmuxRunStat	=	E_GrDvrPlayStatEnd;
				}
				LcDmuxParsFrmUpdt();
			}

			// check frame type
			if(NULL != m_PtrDmuxPars)
			{
				// check frame time
				Tv_IsAble	=	FALSE;
				Tv_Ch			=	m_PtrDmuxPars->Ch;
				Tv_Type		=	m_PtrDmuxPars->Type;

				// check frame
				Tv_IsAble	=	FALSE;
				if((E_GrDvrFrmTypeVdoI <= m_PtrDmuxPars->Type) && (E_GrDvrFrmTypeVdoP4x >= m_PtrDmuxPars->Type))
				{
					// vidoe frame
					Tv_PtrVdo	=	(Ptr_GrDvrFrmHdVdo)m_PtrDmuxPars;
					// check video able
					if((m_DmuxChTbl[Tv_Ch].IsVdoAble) && (m_DmuxSeekTime <= Tv_PtrVdo->Base.TimeSec))
					{
						if(E_GrDvrFrmTypeVdoI == Tv_PtrVdo->Base.Type)
						{
							// I frame
							Tv_IsAble	=	TRUE;
							// update GOP synced
							m_DmuxChTbl[Tv_Ch].IsVdoGopSynced	=	Tv_IsAble;
							// update resolution
							if((m_DmuxChTbl[Tv_Ch].VdoFmt != Tv_PtrVdo->VdoFmt) || (m_DmuxChTbl[Tv_Ch].VdoFps != Tv_PtrVdo->Fps))
							{
								// do update
								m_DmuxChTbl[Tv_Ch].VdoFmt	=	Tv_PtrVdo->VdoFmt;
								m_DmuxChTbl[Tv_Ch].VdoFps	=	Tv_PtrVdo->Fps;
								// do event
								EvtVdoFmtChg(Tv_Ch);
							}
						}

					}	//	if(m_DmuxChTbl[Tv_PtrFrm->Ch].IsVdoAble)

					// video success update
					if(Tv_IsAble)
					{
						// update sequence 
						m_DmuxChTbl[Tv_Ch].VdoOutSeqId		=	Tv_PtrVdo->SeqId;
					}

				}	//	if((E_GrDvrFrmTypeVdoI <= Tv_PtrFrm->Type) && (E_GrDvrFrmTypeVdoP4x >= Tv_PtrFrm->Type))
				else if(E_GrDvrFrmTypeAdo == Tv_Type)
				{
					// audio
					if( ( ((Ptr_GrDvrFrmHdAdo)m_PtrDmuxPars)->IsAble) && (m_DmuxSeekTime <= m_PtrDmuxPars->TimeSec))
					{
						Tv_IsAble	=	m_DmuxChTbl[Tv_Ch].IsAdoAble;
					}
				}

				if(Tv_IsAble)
				{
					m_DmuxTimeLast	=	m_PtrDmuxPars->TimeSec;
					m_PtrDmuxOut		=	m_PtrDmuxPars;
					m_DmuxRunStat		=	E_GrDvrPlayStatPlay;
					break;
				}
			}

		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::LcDumxBwProcSeek(void)
{
	// local -------------------
		__u32	Tv_TryCnt;
		Def_GrErrCode	Tv_ErrCode;
		__u8	Tv_Ch;
		__u8	Tv_Type;
		BOOL8	Tv_IsAble;
		Ptr_GrDvrFrmHdVdo		Tv_PtrVdo;

	// code --------------------
		m_PtrDmuxOut	=	NULL;

		Tv_TryCnt	=	E_GrFsNvtDmuxSeekTryCnt;
		while((E_GrDvrRecHndlNone != m_DmuxSctl.Hndl) && (0 != Tv_TryCnt))
		{
			Tv_TryCnt --;

			Tv_ErrCode	=	LcDmuxBwNextFrm();
			if(E_GrErrNone != Tv_ErrCode)
			{
				// check unknown error
				if(E_GrErrEndOfData != Tv_ErrCode)
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDumxBwProcSeek - frame parsing unknown error (%d) \n", Tv_ErrCode);
					m_DmuxRunStat	=	E_GrDvrPlayStatBegin;
					break;
				}

				// load data
				Tv_ErrCode	=	LcDmuxFwNextSctl();
				if(E_GrErrNone != Tv_ErrCode)
				{
					DbgMsgPrint("Cls_GrFsNvtDmux::LcDumxBwProcSeek - E_GrDvrPlayStatEnd - 3\n");
					m_DmuxRunStat	=	E_GrDvrPlayStatEnd;
				}
				LcDmuxParsFrmUpdt();
			}

			// check frame type
			if(NULL != m_PtrDmuxPars)
			{
				// check frame time
				Tv_IsAble	=	FALSE;
				Tv_Ch			=	m_PtrDmuxPars->Ch;
				Tv_Type		=	m_PtrDmuxPars->Type;

				// check frame
				Tv_IsAble	=	FALSE;
				if((E_GrDvrFrmTypeVdoI <= m_PtrDmuxPars->Type) && (E_GrDvrFrmTypeVdoP4x >= m_PtrDmuxPars->Type))
				{
					// vidoe frame
					Tv_PtrVdo	=	(Ptr_GrDvrFrmHdVdo)m_PtrDmuxPars;
					// check video able
					if((m_DmuxChTbl[Tv_Ch].IsVdoAble) && (m_DmuxSeekTime <= Tv_PtrVdo->Base.TimeSec))
					{
						if(E_GrDvrFrmTypeVdoI == Tv_PtrVdo->Base.Type)
						{
							// I frame
							Tv_IsAble	=	TRUE;
							// update GOP synced
							m_DmuxChTbl[Tv_Ch].IsVdoGopSynced	=	Tv_IsAble;
							// update resolution
							if((m_DmuxChTbl[Tv_Ch].VdoFmt != Tv_PtrVdo->VdoFmt) || (m_DmuxChTbl[Tv_Ch].VdoFps != Tv_PtrVdo->Fps))
							{
								// do update
								m_DmuxChTbl[Tv_Ch].VdoFmt	=	Tv_PtrVdo->VdoFmt;
								m_DmuxChTbl[Tv_Ch].VdoFps	=	Tv_PtrVdo->Fps;
								// do event
								EvtVdoFmtChg(Tv_Ch);
							}
						}

					}	//	if(m_DmuxChTbl[Tv_PtrFrm->Ch].IsVdoAble)

					// video success update
					if(Tv_IsAble)
					{
						// update sequence 
						m_DmuxChTbl[Tv_Ch].VdoOutSeqId		=	Tv_PtrVdo->SeqId;
					}

				}	//	if((E_GrDvrFrmTypeVdoI <= Tv_PtrFrm->Type) && (E_GrDvrFrmTypeVdoP4x >= Tv_PtrFrm->Type))
				else if(E_GrDvrFrmTypeAdo == Tv_Type)
				{
					// audio
					if((((Ptr_GrDvrFrmHdAdo)m_PtrDmuxPars)->IsAble) && (m_DmuxSeekTime <= m_PtrDmuxPars->TimeSec))
					{
						Tv_IsAble	=	m_DmuxChTbl[Tv_Ch].IsAdoAble;
					}
				}

				if(Tv_IsAble)
				{
					m_DmuxTimeLast	=	m_PtrDmuxPars->TimeSec;
					m_PtrDmuxOut		=	m_PtrDmuxPars;
					m_DmuxRunStat		=	E_GrDvrPlayStatPlay;
					break;
				}
			}

		}

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtDmux::LcDmuxFwGoTime(Def_GrTime A_Time)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		m_DmuxRunStat		=	E_GrDvrPlayStatSeek;
		m_DmuxSeekTime	=	A_Time;	// seek target time

		// reset chanel control
		DmuxVdoIfrmSyncAllCh();

		// try seek forward
		Tv_ErrCode	=	m_DmuxIo->ReqReadFwByTime(m_DmuxRunDsub, &m_DmuxSctl, A_Time);
		if(E_GrErrNone != Tv_ErrCode)
		{
#ifdef E_GrFsNvtDmuxDbgMsgView
			DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwGoTime - fail!\n");
#endif
			m_DmuxRunStat		=	E_GrDvrPlayStatStop;
			m_DmuxSctl.Hndl	=	E_GrDvrRecHndlNone;
		}
#ifdef E_GrFsNvtDmuxDbgMsgView
		else
		{
			Def_StrTag	Tv_StrTime;

			GrStrTimeToStr(Tv_StrTime, A_Time);
			DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwGoTime - try seek to %s, blkseq:%d!\n", Tv_StrTime, m_DmuxSctl.BlkSeq);
		}
#endif

		return	Tv_ErrCode;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtDmux::LcDmuxBwGoTime(Def_GrTime A_Time)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		m_DmuxRunStat		=	E_GrDvrPlayStatSeek;
		m_DmuxSeekTime	=	A_Time;	// seek target time

		// reset chanel control
		DmuxVdoIfrmSyncAllCh();

		// try seek forward
		Tv_ErrCode	=	m_DmuxIo->ReqReadBwByTime(m_DmuxRunDsub, &m_DmuxSctl, A_Time);
		if(E_GrErrNone != Tv_ErrCode)
		{
#ifdef E_GrFsNvtDmuxDbgMsgView
			DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwGoTime - fail!\n");
#endif
			m_DmuxRunStat		=	E_GrDvrPlayStatStop;
			m_DmuxSctl.Hndl	=	E_GrDvrRecHndlNone;
		}
#ifdef E_GrFsNvtDmuxDbgMsgView
		else
		{
			Def_StrTag	Tv_StrTime;

			GrStrTimeToStr(Tv_StrTime, A_Time);
			DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxBwGoTime - try seek to %s, blkseq:%d!\n", Tv_StrTime, m_DmuxSctl.BlkSeq);
		}
#endif

		return	Tv_ErrCode;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtDmux::LcDmuxFwGoStart(void)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		m_DmuxRunStat		=	E_GrDvrPlayStatPlay;

		// reset chanel control
		DmuxVdoIfrmSyncAllCh();

		// try seek start
		Tv_ErrCode	=	m_DmuxIo->ReqReadFwStart(m_DmuxRunDsub, &m_DmuxSctl);
		if(E_GrErrNone != Tv_ErrCode)
		{
#ifdef E_GrFsNvtDmuxDbgMsgView
			DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwGoStart - fail!\n");
#endif
			m_DmuxRunStat		=	E_GrDvrPlayStatStop;
			m_DmuxSctl.Hndl	=	E_GrDvrRecHndlNone;
		}
#ifdef E_GrFsNvtDmuxDbgMsgView
		else
		{
			DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwGoStart - try seek to start!\n");
		}
#endif

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtDmux::LcDmuxFwGoHndl(__u64 A_Hndl)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		m_DmuxRunStat		=	E_GrDvrPlayStatPlay;

		// reset chanel control
		DmuxVdoIfrmSyncAllCh();

		// try seek start
		Tv_ErrCode	=	m_DmuxIo->ReqReadFwByHndl(m_DmuxRunDsub,&m_DmuxSctl, A_Hndl);
		if(E_GrErrNone != Tv_ErrCode)
		{
#ifdef E_GrFsNvtDmuxDbgMsgView
			DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwGoHndl - fail!\n");
#endif
			m_DmuxRunStat		=	E_GrDvrPlayStatStop;
			m_DmuxSctl.Hndl	=	E_GrDvrRecHndlNone;
		}
#ifdef E_GrFsNvtDmuxDbgMsgView
		else
		{
			DbgMsgPrint("Cls_GrFsNvtDmux::LcDmuxFwGoHndl - try seek to start!\n");
		}
#endif

		return	Tv_ErrCode;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtDmux::DmuxDevClose(void)
{
	// local -------------------
	// code --------------------
		m_DmuxIo->ReqSctlClose(m_DmuxRunDsub, &m_DmuxSctl);
		return	E_GrErrNone;
}
//--------------------------------------------------------------------
#ifdef DVR_NVT
void	Cls_GrFsNvtDmux::DmuxDecBufSet(__s32 A_MemOfs, __s32 A_DecOfs, __s32 A_Size)
{
#if 0
		// allocate divide frame buffer
		GrDumyZeroMem(&m_DmuxDiv, sizeof(m_DmuxDiv));
		m_DmuxDiv.PtrBuf		=	(void*)(A_MemOfs + A_DecOfs);
		//m_DmuxFrmMaxSize		= A_Size;

		DbgMsgPrint("Cls_GrFsNvtDmux::DmuxDecBufSet - PtrBuf = %08X Mem = %08X DecOfs = %x size = %08X\n",
			m_DmuxDiv.PtrBuf, A_MemOfs, A_DecOfs, A_Size);
#else
		DbgMsgPrint("Cls_GrFsNvtDmux::DmuxDecBufSet - PtrBuf = %08X Size = %08X\n",
			m_DmuxDiv.PtrBuf, m_DmuxFrmMaxSize);
#endif
}
#endif
//--------------------------------------------------------------------
#ifdef DVR_NVT
void*	Cls_GrFsNvtDmux::LcDmuxVdecBufGet(__s32 A_Size)
{
	// local -------------------
		__s32			Tv_DecOfs;
	// code --------------------
		/*if(0 > m_DmuxDecBuf.DecOfs)
		{
			m_DmuxDecBuf.DecOfs	= m_DmuxDecBuf.StartOfs;
		}

		Tv_DecOfs		= m_DmuxDecBuf.DecOfs;
		if((Tv_DecOfs + A_Size) >= (m_DmuxDecBuf.StartOfs + m_DmuxDecBuf.BufSize))
		{
			Tv_DecOfs		= m_DmuxDecBuf.StartOfs;
		}

		m_DmuxDecBuf.DecOfs	= Tv_DecOfs + A_Size;
		m_DmuxDecBuf.BufOfs	= Tv_DecOfs;

		return (void*)(Tv_DecOfs + m_DmuxDecBuf.PhyOfs);*/
		return 0;
}
#endif
//--------------------------------------------------------------------
