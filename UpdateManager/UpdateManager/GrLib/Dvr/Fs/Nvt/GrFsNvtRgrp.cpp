/*
 GAUSE file system version 5 raid group

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrError.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include <Dvr/GrDvrBase.h>
#include <Dvr/Fs/Nvt/GrFsNvtBase.h>
#include <Dvr/Fs/Nvt/GrFsNvtRgrp.h>
//#include <Dvr/Fs/V5/GrFsNvtMng.h>

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
Cls_GrFsNvtRgrp::Cls_GrFsNvtRgrp(__u8 A_GroupType, __u8 A_DskMap, BOOL8 A_IsOvrtMode, BOOL8 A_IsCchRsvAble)
{
	// local -------------------
	// code --------------------
		//init
		m_DiskMap			=	A_DskMap;
		//GrDumyZeroMem( m_DiskTbl, sizeof(m_DiskTbl) );
		m_DiskCnt			=	0;
		m_GrpType			=	A_GroupType;

		m_IsCchRsvAble	=	A_IsCchRsvAble;

		m_RecSeqId		=	E_GrFsNvtSeqIdNone;
		m_RecDskIdx		=	-1;
		m_RecBlkIdx		=	-1;
		m_RecTimeLast	=	0;

		m_IsOvwtMode	=	A_IsOvrtMode;

		GrDumyZeroMem(&m_RecBlkEnv, sizeof(m_RecBlkEnv));
		m_RecBlkEnv.OfsLog	=	-1;

		m_DiskId			=	V_GrFsNvtEnv.DiskBaseId;

		LcCchMinInit();
		LcCchDataInit();

		// initialize group
		LcInit();

}
//--------------------------------------------------------------------
Cls_GrFsNvtRgrp::~Cls_GrFsNvtRgrp()
{
	// local -------------------
	// code --------------------
		LcGrpRecBlkClose(FALSE, FALSE);

		LcCchMinFlushAll();
		LcCchDataFlushAll();

		LcCchMinFinish();
		LcCchDataFinish();

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchMinInit(void)
{
	// local -------------------
		__u32		Tv_ItmIdx;
	// code --------------------
		GrDumyZeroMem(&m_CchMin, sizeof(m_CchMin));

		m_CchMin.Cnt		=	V_GrFsNvtEnv.HwBufs[m_GrpType].CchMinCnt;
		if(0 == m_CchMin.Cnt)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcCchMinInit - min cache count is zero!\n");
			return;
		}

		for(Tv_ItmIdx = 0; Tv_ItmIdx < m_CchMin.Cnt; Tv_ItmIdx++)
		{
			m_CchMin.Items[Tv_ItmIdx].DskIdx	=	-1;
			m_CchMin.Items[Tv_ItmIdx].PtrBuf	=	(Ptr_GrFsNvtMinuteInfo)V_GrFsNvtEnv.HwBufs[m_GrpType].CchMinBuf[Tv_ItmIdx];
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchMinFinish(void)
{
	// local -------------------
		__u32	Tv_ItmIdx;
	// code --------------------
		if(0 != m_CchMin.Cnt)
		{
			for( Tv_ItmIdx = 0; Tv_ItmIdx < m_CchMin.Cnt; Tv_ItmIdx++)
			{
				m_CchMin.Items[Tv_ItmIdx].PtrBuf	=	NULL;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchDataInit(void)
{
	// local -------------------
		__u32		Tv_ItmIdx;
	// code --------------------
		GrDumyZeroMem(&m_CchData, sizeof(m_CchData));

		// build data cache
		m_CchData.Cnt	=	V_GrFsNvtEnv.HwBufs[m_GrpType].CchDataCnt;
		if(0 == m_CchData.Cnt)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcCchDataInit - data cache count is zero!\n");
			return;
		}

		for(Tv_ItmIdx = 0; Tv_ItmIdx < m_CchData.Cnt; Tv_ItmIdx++)
		{
			m_CchData.Items[Tv_ItmIdx].Hndl		=	E_GrDvrRecHndlNone;
			m_CchData.Items[Tv_ItmIdx].PtrBuf	=	V_GrFsNvtEnv.HwBufs[m_GrpType].CchDataBuf[Tv_ItmIdx];
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchDataFinish(void)
{
	// local -------------------
		__u32		Tv_ItmIdx;
	// code --------------------

		
		// release buffer
		if (0 !=  m_CchData.Cnt)
		{
			for (Tv_ItmIdx = 0; Tv_ItmIdx < m_CchData.Cnt; Tv_ItmIdx++)
			{
				m_CchData.Items[Tv_ItmIdx].PtrBuf	=	NULL;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcGrpDskTblBuild(void)
{
	// local -------------------
		__u8	Tv_Mask;
		__u8	Tv_DskIdx;
		Cls_GrFsNvtHdd*	Tv_ObjDsk;
		
	// code --------------------
		// init
		m_GrpStat			=	E_GrDvrRecStatNotexist;
		// build disk table
		m_DiskCnt		=	0;

		Tv_Mask			=	1;
		for(Tv_DskIdx=0; Tv_DskIdx < E_GrFsNvtMaxDiskCnt; Tv_DskIdx++)
		{
			if (0 != (Tv_Mask & m_DiskMap))
			{
				// add disk
				if(NULL != V_GrFsNvtEnv.DskTbl[Tv_DskIdx])
				{
					Tv_ObjDsk	=	(Cls_GrFsNvtHdd*)V_GrFsNvtEnv.DskTbl[Tv_DskIdx];
					// check status
					if(E_GrDvrRecStatEmpty <= Tv_ObjDsk->DiskStatGet())
					{
						// add
						m_DiskTbl[m_DiskCnt]	=	Tv_ObjDsk;
						m_GrpStat	=	E_GrDvrRecStatEmpty;
						m_DiskCnt ++;
					}
				}
			}
			Tv_Mask	=	Tv_Mask << 1;
		}
		
}
//--------------------------------------------------------------------
__u32	Cls_GrFsNvtRgrp::LcGrpCalcSeqId(__u8 A_DskIdx, __u32 A_BlkIdx, __u32 A_TimeSt)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
	// code --------------------
		Tv_Result	=	1;

		for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		{
			Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
			if(NULL != Tv_PtrBi)
			{
				for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
				{
					if(E_GrFsNvtBlkStatBad != Tv_PtrBi[Tv_BlkIdx].Db.BlkStat)
					{
						if((0 != Tv_PtrBi[Tv_BlkIdx].Db.InSize) && (0 != Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeSt) && (0 != Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeEd))
						{
							// chekc sequence id
							if(A_TimeSt > Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeSt)
							{
								Tv_Result ++;
							}
							else if(A_TimeSt == Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeSt)
							{
								if((A_DskIdx > Tv_DskIdx) || ((A_DskIdx == Tv_DskIdx) && (A_BlkIdx > Tv_BlkIdx)))
								{
									Tv_Result ++;
								}
							}
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcGrpSeqIdRebuild(void)
{
	// local -------------------
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		__u32	Tv_SeqId;
		__u8	Tv_OvwtId;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
		St_GrTaskMsgGrpDskFsFix	Tv_MsgFix;
	// code --------------------
		Tv_SeqId		=	E_GrFsNvtSeqIdNone;
		Tv_OvwtId		=	0;

		if(0 != m_DiskCnt)
		{
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				if(E_GrDvrRecStatEmpty <= m_DiskTbl[Tv_DskIdx]->DiskStatGet())
				{
					Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
					if(NULL != Tv_PtrBi)
					{
						for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
						{
							if(E_GrFsNvtBlkStatBad != Tv_PtrBi[Tv_BlkIdx].Db.BlkStat)
							{
								// check data exist
								if(0 != Tv_PtrBi[Tv_BlkIdx].Db.InSize)
								{
									if((0 != Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeSt) && (0 != Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeEd))
									{
										// get sequence id
										Tv_PtrBi[Tv_BlkIdx].Db.SeqId	=	LcGrpCalcSeqId(Tv_DskIdx, Tv_BlkIdx, Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeSt);
										if(Tv_SeqId < Tv_PtrBi[Tv_BlkIdx].Db.SeqId)
										{
											Tv_SeqId	=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
											Tv_OvwtId	=	Tv_PtrBi[Tv_BlkIdx].Db.OvwtId;
										}
									}
									else
									{
										Tv_PtrBi[Tv_BlkIdx].Db.SeqId	=	0;
										Tv_PtrBi[Tv_BlkIdx].Db.InSize	=	0;
										Tv_PtrBi[Tv_BlkIdx].Db.OvwtId	=	0;
										DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpSeqIdRebuild - disk %d block %d info bad\n",
											m_DiskTbl[Tv_DskIdx]->DevIdxGet(), Tv_BlkIdx);
									}
								}
								else
								{
									// reset squence id
									Tv_PtrBi[Tv_BlkIdx].Db.SeqId	=	0;
									Tv_PtrBi[Tv_BlkIdx].Db.InSize	=	0;
									Tv_PtrBi[Tv_BlkIdx].Db.OvwtId	=	0;
								}
							}
						}	//	for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)

						// write header
						m_DiskTbl[Tv_DskIdx]->BlkInfoWriteAll();
					}
				}
			}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)

			// error fix notify
			Tv_MsgFix.MsgHd.Cmd	=	E_GrTaskMsgGroupFsFix;
			Tv_MsgFix.Type			=	1;
			Tv_MsgFix.Index			=	m_GrpType;
			GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgFix, sizeof(Tv_MsgFix));
		}

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LogAdd(Ptr_GrDvrLogItm A_PtrItem)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
		__u32	Tv_InPos;
	// code --------------------
		// check record open
		if((0 > m_RecDskIdx) || (0 > m_RecBlkIdx))
		{
			return	E_GrErrInitFail;
		}

		if((0 > m_RecBlkEnv.OfsLog) || (E_GrFsNvtFrmLogPoolItemCnt <= m_RecBlkEnv.Log.Head.InCnt))
		{
			// try create
			Tv_ErrCode	=	LcGrpRecLogPoolAdd();
			if(E_GrErrNone != Tv_ErrCode)
			{
				DbgMsgPrint("[BUG] log pool create fail!(err:%d)\n", Tv_ErrCode);
				return	Tv_ErrCode;
			}	
		}

		Tv_InPos	=	m_RecBlkEnv.Log.Head.InCnt;

		GrDumyCopyMem(&m_RecBlkEnv.Log.LogTbl[Tv_InPos], A_PtrItem, sizeof(St_GrDvrLogItm));

		m_RecBlkEnv.Log.Head.InCnt ++;
		m_RecBlkEnv.IsLogChg	=	TRUE;

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpBlkFindOldest(__s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtSeqId, __u8* A_PtrRtOvwtId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
		__u32	Tv_SeqId;
	// code --------------------
		Tv_Result	=	FALSE;
		*A_PtrRtDskIdx	=	-1;
		*A_PtrRtBlkIdx	=	-1;

		Tv_SeqId	=	0;

		if(0 != m_DiskCnt)
		{
			// find sequence id
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				// check disk status
				if(E_GrDvrRecStatEmpty <= m_DiskTbl[Tv_DskIdx]->DiskStatGet())
				{
					Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
					if(NULL != Tv_PtrBi)
					{
						for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
						{
							// check block fcc
							if(E_GrFsNvtBlkStatBad != Tv_PtrBi[Tv_BlkIdx].Db.BlkStat)
							{
								// chekc attribute and size
								if(0 != Tv_PtrBi[Tv_BlkIdx].Db.SeqId)
								{
									// check found sequence id
									if(E_GrFsNvtSeqIdNone == Tv_SeqId)
									{
										// found
										*A_PtrRtDskIdx	=	(__s32)Tv_DskIdx;
										*A_PtrRtBlkIdx	=	(__s32)Tv_BlkIdx;
										*A_PtrRtSeqId		=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
										*A_PtrRtOvwtId	=	Tv_PtrBi[Tv_BlkIdx].Db.OvwtId;
										Tv_SeqId				=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
										Tv_Result	=	TRUE;
									}
									else
									{
										if(Tv_SeqId > Tv_PtrBi[Tv_BlkIdx].Db.SeqId)
										{
											// update new sequence id
											*A_PtrRtDskIdx	=	(__s32)Tv_DskIdx;
											*A_PtrRtBlkIdx	=	(__s32)Tv_BlkIdx;
											*A_PtrRtSeqId		=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
											*A_PtrRtOvwtId	=	Tv_PtrBi[Tv_BlkIdx].Db.OvwtId;
											Tv_SeqId				=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
										}
									}
								}
							}
						}	//	for( Tv_BlkIdx = 0; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					}
				}
			}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpBlkFindNewest(__s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtSeqId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
		__u32	Tv_SeqId;
	// code --------------------
		Tv_Result	=	FALSE;
		*A_PtrRtDskIdx	=	-1;
		*A_PtrRtBlkIdx	=	-1;

		Tv_SeqId	=	0;

		if(0 != m_DiskCnt)
		{
			// find sequence id
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				// check disk status
				if(E_GrDvrRecStatEmpty <= m_DiskTbl[Tv_DskIdx]->DiskStatGet())
				{
					Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
					if(NULL != Tv_PtrBi)
					{
						for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
						{
							// check block fcc
							if(E_GrFsNvtBlkStatBad != Tv_PtrBi[Tv_BlkIdx].Db.BlkStat)
							{
								// chekc attribute and size
								if(0 != Tv_PtrBi[Tv_BlkIdx].Db.SeqId)
								{
									// check found sequence id
									if(E_GrFsNvtSeqIdNone == Tv_SeqId)
									{
										// found
										*A_PtrRtDskIdx	=	(__s32)Tv_DskIdx;
										*A_PtrRtBlkIdx	=	(__s32)Tv_BlkIdx;
										*A_PtrRtSeqId		=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
										Tv_SeqId				=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
										Tv_Result	=	TRUE;
									}
									else
									{
										if(Tv_SeqId < Tv_PtrBi[Tv_BlkIdx].Db.SeqId)
										{
											// update new sequence id
											*A_PtrRtDskIdx	=	(__s32)Tv_DskIdx;
											*A_PtrRtBlkIdx	=	(__s32)Tv_BlkIdx;
											*A_PtrRtSeqId		=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
											Tv_SeqId				=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
										}
									}
								}
							}
						}	//	for( Tv_BlkIdx = 0; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					}
				}
			}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpMibUpdate(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
	// code --------------------
		Tv_Result	=	FALSE;

		if(0 != m_DiskCnt)
		{
			for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrBi)
				{
					// get MIB
					if(E_GrFsNvtFcc == Tv_PtrBi[E_GrFsNvtBlkIdxMib].Mib.Fcc)
					{
						// update last record sequence
						Tv_PtrBi[E_GrFsNvtBlkIdxMib].Mib.SeqLast	=	m_RecSeqId;
						Tv_PtrBi[E_GrFsNvtBlkIdxMib].Mib.SeqOvwt	=	m_RecSeqOvwt;

						// write
						if(E_GrErrNone == m_DiskTbl[Tv_DskIdx]->BlkInfoWriteByIdx(E_GrFsNvtBlkIdxMib))
						{
							Tv_Result	=	TRUE;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrFsNvtRgrp::LcDskBlkFindEmpty(__u32 A_DskIdx)
{
	// local -------------------
		__s32	Tv_Result;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
	// code --------------------
		Tv_Result		=	-1;

		if(m_DiskCnt > A_DskIdx)
		{
			Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[A_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
			if(NULL != Tv_PtrBi)
			{
				for( Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
				{
					// check block status
					if(E_GrFsNvtBlkStatNormal == Tv_PtrBi[Tv_BlkIdx].Db.BlkStat)
					{
						if(0 == Tv_PtrBi[Tv_BlkIdx].Db.InSize)
						{
							// found
							Tv_Result	=	(__s32)Tv_BlkIdx;
							break;
						}
					}
				}	//	for( Tv_BlkIdx = 0; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpBlkFindEmpty(__s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__s32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		*A_PtrRtDskIdx	=	-1;
		*A_PtrRtBlkIdx	=	-1;

		if(0 != m_DiskCnt)
		{
			for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				Tv_BlkIdx	=	LcDskBlkFindEmpty((__u32)Tv_DskIdx);
				if(0 <= Tv_BlkIdx)
				{
					// found
					*A_PtrRtDskIdx	=	(__s32)Tv_DskIdx;
					*A_PtrRtBlkIdx	=	Tv_BlkIdx;
					Tv_Result	=	TRUE;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpRecBlkClose(BOOL8 A_IsFull, BOOL8 A_IsBad)
{
	// local -------------------
		BOOL8	Tv_IsDskStat;
		
	// code --------------------
		// check not open
		if((0 > m_RecDskIdx) || (0 > m_RecBlkIdx))
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkClose - record not opened.\n");
			return	E_GrErrBadParam;
		}

		// check not bad
		if(!A_IsBad)
		{
			// update log
			if(m_RecBlkEnv.IsLogChg)
			{
				// write
				LcGrpRecLogPoolWrite();
			}

			if(A_IsFull)
			{
				m_RecBlkEnv.PtrDb->BlkStat		=	E_GrFsNvtBlkStatFull;
				m_RecBlkEnv.IsDbChg	=	TRUE;
			}
		}

		// bad block
		Tv_IsDskStat	=	FALSE;
		if(A_IsBad)
		{
			m_RecBlkEnv.PtrDb->BlkStat		=	E_GrFsNvtBlkStatBad;
			m_RecBlkEnv.IsDbChg	=	TRUE;
			Tv_IsDskStat	=	TRUE;
		}

		// update DB
		if(m_RecBlkEnv.IsDbChg)
		{
			// update DB
			if(E_GrErrNone != m_DiskTbl[m_RecDskIdx]->BlkInfoWriteByIdx((__u32)m_RecBlkIdx))
			{
				DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkClose - disk %d block %d block info write fail!\n",
					m_DiskTbl[m_RecDskIdx]->DevIdxGet(), m_RecBlkIdx);
				// request upate disk status
				Tv_IsDskStat	=	TRUE;
			}
			m_RecBlkEnv.IsDbChg	=	FALSE;
		}

		// update disk status
		if(Tv_IsDskStat)
		{
			LcGrpStatUpdate();
		}

		// reset
		GrDumyZeroMem(&m_RecBlkEnv, sizeof(m_RecBlkEnv));
		m_RecBlkEnv.OfsLog	=	-1;
		m_RecDskIdx	=	-1;
		m_RecBlkIdx	=	-1;

		return	E_GrErrNone;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpRecBlkOpen(__u32 A_DskIdx, __u32 A_BlkIdx)
{
	// local -------------------
		__u32	Tv_BlkCnt;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
	// code --------------------
		// check already open
		if((0 <= m_RecDskIdx) || (0 <= m_RecBlkIdx))
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkCreate - record already open!\n");
			return	E_GrErrBadParam;
		}

		// init
		m_RecBlkEnv.PtrDb			=	NULL;
		m_RecBlkEnv.IsDbChg		=	FALSE;
		m_RecBlkEnv.IsLogChg	=	FALSE;

		// check disk count
		if((__u32)m_DiskCnt <= A_DskIdx)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkCreate - Bad disk index %d value!\n", A_DskIdx);
			return	E_GrErrBadParam;
		}

		// get block info
		Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[A_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
		if((NULL == Tv_PtrBi) || (0 == Tv_BlkCnt))
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkCreate - Disk %d can not open!\n", m_DiskTbl[A_DskIdx]->DevIdxGet());
			return	E_GrErrDiskStatBad;
		}

		// check block index
		if(Tv_BlkCnt <= A_BlkIdx)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkCreate - Bad block index %d (Disk: %d)!\n",
				A_BlkIdx, m_DiskTbl[A_DskIdx]->DevIdxGet());
			return	E_GrErrBadParam;
		}

		// check block status
		if(E_GrFsNvtBlkStatBad == Tv_PtrBi[A_BlkIdx].Db.BlkStat)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkCreate - block (disk:%d,index:%d) status is bad!\n",
				m_DiskTbl[A_DskIdx]->DevIdxGet(), A_BlkIdx);
			return	E_GrErrDiskBadSector;
		}

		// load last log
		LcGrpRecLogPoolOpen(A_DskIdx, A_BlkIdx);

		// success
		m_RecDskIdx		=	(__s32)A_DskIdx;
		m_RecBlkIdx		=	(__s32)A_BlkIdx;
		m_RecBlkEnv.PtrDb	=	&Tv_PtrBi[A_BlkIdx].Db;

		// get record overwrite id
		m_RecSeqId		=	Tv_PtrBi[A_BlkIdx].Db.SeqId;
		m_RecSeqOvwt	=	Tv_PtrBi[A_BlkIdx].Db.OvwtId;

		return	E_GrErrNone;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpRecBlkNew(__u32 A_DskIdx, __u32 A_BlkIdx)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
		__u32	Tv_BlkCnt;
		__u32	Tv_BkSeqId;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
	// code --------------------
		// check already open
		if((0 <= m_RecDskIdx) || (0 <= m_RecBlkIdx))
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkNew - record already open!\n");
			return	E_GrErrBadParam;
		}

		// init
		m_RecBlkEnv.PtrDb			=	NULL;
		m_RecBlkEnv.IsDbChg		=	FALSE;
		m_RecBlkEnv.IsLogChg	=	FALSE;
		m_RecBlkEnv.OfsLog		=	-1;

		LcGrpRecLogPoolPktBuild(-1);

		// check disk count
		if((__u32)m_DiskCnt <= A_DskIdx)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkNew - Bad disk index %d value!\n", A_DskIdx);
			return	E_GrErrBadParam;
		}

		// get block info
		Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[A_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
		if((NULL == Tv_PtrBi) || (0 == Tv_BlkCnt))
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkNew - Disk %d can not open!\n", m_DiskTbl[A_DskIdx]->DevIdxGet());
			return	E_GrErrDiskStatBad;
		}

		// check block index
		if(Tv_BlkCnt <= A_BlkIdx)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkNew - Bad block index %d (Disk: %d)!\n",
				A_BlkIdx, m_DiskTbl[A_DskIdx]->DevIdxGet());
			return	E_GrErrBadParam;
		}

		// check block status
		if(E_GrFsNvtBlkStatBad == Tv_PtrBi[A_BlkIdx].Db.BlkStat)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkNew - block (disk:%d,index:%d) status is bad!\n",
				m_DiskTbl[A_DskIdx]->DevIdxGet(), A_BlkIdx);
			return	E_GrErrDiskBadSector;
		}

		Tv_BkSeqId		=	m_RecSeqId;				//sequence id backup
		m_RecSeqId ++;

		// update overwrite id
		if((E_GrFsNvtBlkStatNormal != Tv_PtrBi[A_BlkIdx].Db.BlkStat) || (0 != Tv_PtrBi[A_BlkIdx].Db.InSize))
		{
			Tv_PtrBi[A_BlkIdx].Db.OvwtId ++;
		}

		// build new sequence id
		Tv_PtrBi[A_BlkIdx].Db.BlkStat					=	E_GrFsNvtBlkStatNormal;
		Tv_PtrBi[A_BlkIdx].Db.InSize					=	0;
		Tv_PtrBi[A_BlkIdx].Db.OfsLastLog			=	-1;
		Tv_PtrBi[A_BlkIdx].Db.SeqId						=	m_RecSeqId;
		Tv_PtrBi[A_BlkIdx].Db.TimeRng.TimeSt	=	0;
		Tv_PtrBi[A_BlkIdx].Db.TimeRng.TimeEd	=	0;

		// get record overwrite id
		m_RecSeqOvwt	=	Tv_PtrBi[A_BlkIdx].Db.OvwtId;

		// try write
		Tv_ErrCode	=	m_DiskTbl[A_DskIdx]->BlkInfoWriteByIdx(A_BlkIdx);
		if(E_GrErrNone != Tv_ErrCode)
		{
			// update
			LcGrpStatUpdate();

			return	E_GrErrDiskStatBad;
		}

		// update record sequence id
		LcGrpMibUpdate();
		if(E_GrDvrRecStatEmpty > m_DiskTbl[A_DskIdx]->DiskStatGet())
		{
			return	E_GrErrDiskStatBad;
		}

		// success
#if 0
#ifdef E_GrFsNvtRgrpDbgMsgView
		DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecBlkNew - block (disk:%d,index:%d,seq id:%d) new allocated.\n",
			m_DiskTbl[A_DskIdx]->DevIdxGet(), A_BlkIdx, m_RecSeqId);
#endif
#endif

		m_RecDskIdx		=	(__s32)A_DskIdx;
		m_RecBlkIdx		=	(__s32)A_BlkIdx;

		m_RecBlkEnv.PtrDb	=	&Tv_PtrBi[A_BlkIdx].Db;

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcGrpRecLogPoolPktBuild(__s32 A_OfsPrv)
{
	// local -------------------
	// code --------------------
		m_RecBlkEnv.Log.Head.Base.Fcc				=	E_GrDvrFrmHeadFcc;
		m_RecBlkEnv.Log.Head.Base.Ch				=	0xFF;
		m_RecBlkEnv.Log.Head.Base.DataSize	=	sizeof(St_GrDvrLogItm)* E_GrFsNvtFrmLogPoolItemCnt;
		m_RecBlkEnv.Log.Head.Base.IsSm			=	0;	// not use
		m_RecBlkEnv.Log.Head.Base.TimeSec		=	0;	// not use
		m_RecBlkEnv.Log.Head.Base.TimeMsec	=	0;
		m_RecBlkEnv.Log.Head.Base.Ratb			=	0;
		m_RecBlkEnv.Log.Head.Base.Type			=	E_GrDvrFrmTypeLogPool;
		m_RecBlkEnv.Log.Head.InCnt					=	0;
		m_RecBlkEnv.Log.Head.OfsPrv					=	A_OfsPrv;
		m_RecBlkEnv.Log.Head._rsvd0					=	0;

		GrDumyZeroMem(m_RecBlkEnv.Log.LogTbl, sizeof(St_GrDvrLogItm)* E_GrFsNvtFrmLogPoolItemCnt);

		m_RecBlkEnv.Log.Tail.Fcc						=	E_GrDvrFrmTailFcc;
		m_RecBlkEnv.Log.Tail.PktSize				=	sizeof(St_GrFsNvtFrmLogPool);

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpRecLogPoolOpen(__u32 A_DskIdx, __u32 A_BlkIdx)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
		Ptr_GrFsNvtDbBlkInfo	Tv_PtrBi;
	// code --------------------
		// get block info
		Tv_PtrBi	=	LcDskDbBlkInfoGet(A_DskIdx, A_BlkIdx);
		if(NULL == Tv_PtrBi)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecLogPoolOpen - can not get block info!\n");
			return	E_GrErrBadParam;
		}

		// check exist pool
		if(0 > Tv_PtrBi->OfsLastLog)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecLogPoolOpen - last logblock not found, create new.\n");
			LcGrpRecLogPoolPktBuild(-1);
			return	E_GrErrNone;
		}
		
		// load
		Tv_ErrCode	=	LcDskReadToBuf(A_DskIdx, A_BlkIdx, (__u32)Tv_PtrBi->OfsLastLog,
			sizeof(St_GrFsNvtFrmLogPool), &m_RecBlkEnv.Log);
		if(E_GrErrNone == Tv_ErrCode)
		{
			// check log packet
			if((E_GrDvrFrmHeadFcc != m_RecBlkEnv.Log.Head.Base.Fcc) || (E_GrDvrFrmTypeLogPool != m_RecBlkEnv.Log.Head.Base.Type))
			{
				DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecLogPoolOpen - bad log frame found!\n");
				// delete log
				Tv_PtrBi->OfsLastLog	=	-1;
				m_DiskTbl[A_DskIdx]->BlkInfoWriteByIdx(A_BlkIdx);
				LcGrpRecLogPoolPktBuild(-1);
			}

			// update log position
			m_RecBlkEnv.OfsLog	=	Tv_PtrBi->OfsLastLog;
		}

		return	Tv_ErrCode;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpRecLogPoolWrite(void)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		// reset change info
		m_RecBlkEnv.IsLogChg	=	FALSE;

		// check open
		if((0 > m_RecDskIdx) || (0 > m_RecBlkIdx))
		{
			return	E_GrErrBadParam;
		}

		// check registed
		if((0 > m_RecBlkEnv.PtrDb->OfsLastLog) || (m_RecBlkEnv.PtrDb->OfsLastLog != m_RecBlkEnv.OfsLog))
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecLogPoolWrite - log pool not registed!\n");
			return	E_GrErrFail;
		}

		// try write
		Tv_ErrCode	=	LcDskWriteFromBuf((__u32)m_RecDskIdx, (__u32)m_RecBlkIdx, (__u32)m_RecBlkEnv.PtrDb->OfsLastLog,
			sizeof(St_GrFsNvtFrmLogPool), &m_RecBlkEnv.Log);

#if 0
		DbgMsgPrint("[LOG] write dsk %d blk %d ofs %d cnt %d err %d\n", m_RecDskIdx, m_RecBlkIdx, m_RecBlkEnv.PtrDb->OfsLastLog,
			m_RecBlkEnv.Log.Head.InCnt,Tv_ErrCode);
#endif

		return	Tv_ErrCode;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpRecLogPoolAdd(void)
{
	// local -------------------
		__s32	Tv_PrvOfs;
	// code --------------------
		// check open
		if((0 > m_RecDskIdx) || (0 > m_RecBlkIdx))
		{
			return	E_GrErrBadParam;
		}

		// close previous data
		if(0 <= m_RecBlkEnv.PtrDb->OfsLastLog)
		{
			LcGrpRecLogPoolWrite();
		}

		// init
		Tv_PrvOfs	=	m_RecBlkEnv.PtrDb->OfsLastLog;
		LcGrpRecLogPoolPktBuild(Tv_PrvOfs);

		// check block full
		if(E_GrDvrRecStatFull == m_RecBlkEnv.PtrDb->BlkStat)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecLogPoolAdd - block full!\n");
			m_RecBlkEnv.OfsLog	=	-1;
			return	E_GrErrDiskFull;
		}

		// check allocate able
		if(((__u32)E_GrFsNvtBlkSize - m_RecBlkEnv.PtrDb->InSize) < sizeof(St_GrFsNvtFrmLogPool))
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecLogPoolAdd - block full!\n");
			m_RecBlkEnv.OfsLog	=	-1;
			return	E_GrErrDiskFull;
		}

		// set new log
		m_RecBlkEnv.OfsLog	=	(__s32)m_RecBlkEnv.PtrDb->InSize;

		// change block in size
		m_RecBlkEnv.PtrDb->InSize	=	m_RecBlkEnv.PtrDb->InSize + sizeof(St_GrFsNvtFrmLogPool);
		if((__u32)E_GrFsNvtBlkSize < m_RecBlkEnv.PtrDb->InSize)
		{
			m_RecBlkEnv.PtrDb->BlkStat	=	E_GrDvrRecStatFull;
		}

		// do write
		m_RecBlkEnv.PtrDb->OfsLastLog	=	m_RecBlkEnv.OfsLog;

		if(E_GrErrNone != LcGrpRecLogPoolWrite())
		{
			m_RecBlkEnv.PtrDb->OfsLastLog	=	Tv_PrvOfs;
			m_RecBlkEnv.OfsLog	=	-1;
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRecLogPoolAdd - data write fail!\n");
		}
		
		// chaange DB info
		m_RecBlkEnv.IsDbChg	=	TRUE;

		return	E_GrErrNone;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpRecAddTimeByRbw(__u32 A_Time, __u32 A_Ofs, Ptr_GrFsNvtRgrpRbwChCtrl A_PtrRbwCi)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		BOOL8	Tv_IsAdrChg;
		__u32	Tv_TimeMin;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		// check open record block
		if ((0 <= m_RecDskIdx) && (0 <= m_RecBlkIdx))
		{
			Tv_Result		=	E_GrErrNone;
			Tv_TimeMin	=	A_Time - (A_Time % E_GrTimeSecOfMin);

			// check need open
			if((0 == m_RecBlkEnv.TimeMinute) || ((m_RecBlkEnv.TimeMinute > A_Time) || ((m_RecBlkEnv.TimeMinute + E_GrTimeSecOfMin) <= A_Time)))
			{
				// close old data
				LcGrpRecMinuteClose();
				// open new
				LcGrpRecMinuteOpen(Tv_TimeMin);
			}

			// check add able
			Tv_IsAdrChg	=	FALSE;
			if(0 != m_RecBlkEnv.TimeMinute)
			{
				// update handle
				if(30 > (A_Time - m_RecBlkEnv.TimeMinute))
				{
					if(0 == m_RecBlkEnv.MinuteInfo.AdrMin.Idx)
					{
						m_RecBlkEnv.MinuteInfo.AdrMin.Idx			=	(__u16)m_RecBlkIdx;
						m_RecBlkEnv.MinuteInfo.AdrMin.Ofs			=	A_Ofs;
						m_RecBlkEnv.MinuteInfo.AdrMin.OvwtId	=	m_RecSeqOvwt;
						Tv_IsAdrChg	=	TRUE;
					}
				}
				else
				{
					if(0 == m_RecBlkEnv.MinuteInfo.Adr30s.Idx)
					{
						m_RecBlkEnv.MinuteInfo.Adr30s.Idx			=	(__u16)m_RecBlkIdx;
						m_RecBlkEnv.MinuteInfo.Adr30s.Ofs			=	A_Ofs;
						m_RecBlkEnv.MinuteInfo.Adr30s.OvwtId	=	m_RecSeqOvwt;
						Tv_IsAdrChg	=	TRUE;
					}
				}
			
				// update record info
				for(Tv_WkIdx = 0; Tv_WkIdx < E_GrDvrMaxChCnt; Tv_WkIdx++)
				{
					m_RecBlkEnv.MinuteInfo.ChRatbTbl[Tv_WkIdx]	=	m_RecBlkEnv.MinuteInfo.ChRatbTbl[Tv_WkIdx] |
						A_PtrRbwCi[Tv_WkIdx].Ratb;
					m_RecBlkEnv.MinuteInfo.ChSizeTbl[Tv_WkIdx]	=	m_RecBlkEnv.MinuteInfo.ChSizeTbl[Tv_WkIdx] +
						A_PtrRbwCi[Tv_WkIdx].BkSize;
				}

				m_RecBlkEnv.IsMinuteChg		=	TRUE;
				// check changed address info
				if (Tv_IsAdrChg)
				{
					LcGrpRecMinuteFlush();
				}
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtRgrp::StatusGet(BOOL8 A_IsRefresh)
{
		if(A_IsRefresh)
		{
			LcGrpStatUpdate();
		}

		return	m_GrpStat;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpRecAddBlkTimeRange(__u32 A_TimeMin, __u32 A_TimeMax)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		// check open
		if ((0 <= m_RecDskIdx) && (0 <= m_RecBlkIdx))
		{
			// check time start
			if (0 == m_RecBlkEnv.PtrDb->TimeRng.TimeSt)
			{
				m_RecBlkEnv.PtrDb->TimeRng.TimeSt	=	A_TimeMin;
				m_RecBlkEnv.IsDbChg	=	TRUE;
			}
			else
			{
				if (m_RecBlkEnv.PtrDb->TimeRng.TimeSt > A_TimeMin)
				{
					m_RecBlkEnv.PtrDb->TimeRng.TimeSt	=	A_TimeMin;
					m_RecBlkEnv.IsDbChg	=	TRUE;
				}
			}
			// check time end
			if (0 == m_RecBlkEnv.PtrDb->TimeRng.TimeEd)
			{
				m_RecBlkEnv.PtrDb->TimeRng.TimeEd	=	A_TimeMax;
				m_RecBlkEnv.IsDbChg	=	TRUE;
			}
			else
			{
				if (m_RecBlkEnv.PtrDb->TimeRng.TimeEd < A_TimeMax)
				{
					m_RecBlkEnv.PtrDb->TimeRng.TimeEd	=	A_TimeMax;
					m_RecBlkEnv.IsDbChg	=	TRUE;
				}
			}

			Tv_Result	=	E_GrErrNone;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrFsNvtRgrp::LcGrpRecFreeSizeGet(void)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result				=	0;

		// check open
		if ((0 <= m_RecDskIdx) && (0 <= m_RecBlkIdx))
		{
			// check full
			if (E_GrFsNvtBlkStatNormal == m_RecBlkEnv.PtrDb->BlkStat)
			{
				Tv_Result				=	(__u32)E_GrFsNvtBlkSize - m_RecBlkEnv.PtrDb->InSize;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpBlkFindBySeqFw(__u32 A_SeqId, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsExit;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
		__s32	Tv_FndDskIdx;
		__s32	Tv_FndBlkIdx;
		__u32	Tv_FndBlkSeq;
		__u8	Tv_FndOvwtId;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_IsExit	=	FALSE;
		Tv_FndDskIdx	=	-1;
		Tv_FndBlkIdx	=	-1;
		Tv_FndBlkSeq	=	0;
		Tv_FndOvwtId	=	0;

		if(0 != m_DiskCnt)
		{
			// check time range
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				// get disk block head table
				Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrBi)
				{
					// check block
					for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					{
						// check block type
						if(E_GrFsNvtBlkStatBad != Tv_PtrBi[Tv_BlkIdx].Db.BlkStat)
						{
							// check sequence id
							if(Tv_PtrBi[Tv_BlkIdx].Db.SeqId == A_SeqId)
							{
								// found
								Tv_Result	=	TRUE;
								Tv_IsExit	=	TRUE;

								Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
								Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
								Tv_FndBlkSeq	=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
								Tv_FndOvwtId	=	Tv_PtrBi[Tv_BlkIdx].Db.OvwtId;
								break;
							}
							else if(Tv_PtrBi[Tv_BlkIdx].Db.SeqId > A_SeqId)
							{
								if(0 > Tv_FndDskIdx)
								{
									Tv_Result	=	TRUE;
									Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
									Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
									Tv_FndBlkSeq	=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
									Tv_FndOvwtId	=	Tv_PtrBi[Tv_BlkIdx].Db.OvwtId;
								}
								else
								{
									if(Tv_PtrBi[Tv_BlkIdx].Db.SeqId < Tv_FndBlkSeq)
									{
										Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
										Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
										Tv_FndBlkSeq	=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
										Tv_FndOvwtId	=	Tv_PtrBi[Tv_BlkIdx].Db.OvwtId;
									}
								}
							}
						}
					}	//	for( Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
				}
				if(Tv_IsExit)
				{
					break;
				}
			}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		}

		// update
		*A_PtrRtDskIdx	=	Tv_FndDskIdx;
		*A_PtrRtBlkIdx	=	Tv_FndBlkIdx;
		*A_PtrRtBlkSeq	=	Tv_FndBlkSeq;
		*A_PtrRtOvwtId	=	Tv_FndOvwtId;

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpReadSctlFwByHndl(Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl)
{
	// local -------------------
		Def_GrCalPtr	Tv_ErrCode;
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		__s32	Tv_BlkOfs;
		__u32	Tv_CchOfs;
		__u64	Tv_Hndl;

		Ptr_GrFsNvtDbBlkInfo	Tv_PtrBi;
	// code --------------------
		if(E_GrDvrRecHndlNone == A_Hndl)
		{
			return	E_GrErrBadParam;
		}

		// check exist block
		Tv_PtrBi	=	LcGrpDbBlkInfoGetByHndl(A_Hndl);
		if(NULL == Tv_PtrBi)
		{
			return	E_GrErrBadParam;
		}

		Tv_BlkSeq	=	Tv_PtrBi->SeqId;

		// decode handle
		GrFsNvtHndlDec(A_Hndl, &Tv_DskIdx, (__u32*)&Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);

		// patch offset
		Tv_CchOfs	=	Tv_BlkOfs & E_GrFsNvtRgrpCchDataOfsMask;
		Tv_BlkOfs	=	Tv_BlkOfs & E_GrFsNvtRgrpCchDataAdrMask;

		// read 
		Tv_Hndl		=	GrFsNvtHndlEnc(Tv_DskIdx, Tv_BlkOfs, Tv_BlkIdx, Tv_OvwtId);
		// get cache
		Tv_ErrCode	=	LcCchDataGetByHndlToSctl(A_PtrCtrl, Tv_Hndl);
		if(E_GrErrNone == Tv_ErrCode)
		{
			// found
			// patch offset
			A_PtrCtrl->Prgs	=	0;
			if(A_PtrCtrl->InSize >= Tv_CchOfs)
			{
				A_PtrCtrl->Prgs	=	Tv_CchOfs;
			}

			return	E_GrErrNone;
		}

		// go next address
		Tv_BlkOfs	=	Tv_BlkOfs + E_GrFsNvtRgrpCchDataBufSize;

		while(TRUE)
		{
			// check group status
			if(E_GrDvrRecStatEmpty > m_GrpStat)
			{
				return	E_GrErrDiskStatBad;
			}

			// check exist block
			if(E_GrFsNvtBlkOfsLimit > Tv_BlkOfs)
			{
				Tv_Hndl	=	GrFsNvtHndlEnc(Tv_DskIdx, Tv_BlkOfs, Tv_BlkIdx, Tv_OvwtId);
				// read cache block
				Tv_ErrCode	=	LcCchDataGetByHndlToSctl(A_PtrCtrl, Tv_Hndl);
				if(E_GrErrNone == Tv_ErrCode)
				{
					// reset progress
					A_PtrCtrl->Prgs		=	0;
					// update cache reserve
					LcCchDataReserve(FALSE, A_PtrCtrl->CchRsv, Tv_Hndl);
					break;
				}
			}

			// next block
			Tv_BlkOfs		=	0;
			if(Tv_BlkSeq >= m_RecSeqId)
			{
				return	E_GrErrEndOfData;
			}

			if(!LcGrpBlkFindBySeqFw(Tv_BlkSeq, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId))
			{
				// finished
#ifdef E_GrFsNvtRgrpDbgMsgView
				DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpReadSctlFwByHndl - next sequence id (%d) not found\n", Tv_BlkSeq);
#endif
				return	E_GrErrEndOfData;
			}

		}

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpReadSctlBwByHndl(Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl)
{
	// local -------------------
		Def_GrCalPtr	Tv_ErrCode;
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		__s32	Tv_BlkOfs;
		__u32	Tv_CchOfs;
		__u64	Tv_Hndl;

		Ptr_GrFsNvtDbBlkInfo	Tv_PtrBi;
	// code --------------------

		if(E_GrDvrRecHndlNone == A_Hndl)
		{
			return	E_GrErrBadParam;
		}

		// check exist block
		Tv_PtrBi	=	LcGrpDbBlkInfoGetByHndl(A_Hndl);
		if(NULL == Tv_PtrBi)
		{
			return	E_GrErrBadParam;
		}

		Tv_BlkSeq	=	Tv_PtrBi->SeqId;

		// decode handle
		GrFsNvtHndlDec(A_Hndl, &Tv_DskIdx, (__u32*)&Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);

		// patch offset
		Tv_CchOfs	=	Tv_BlkOfs & E_GrFsNvtRgrpCchDataOfsMask;
		Tv_BlkOfs	=	Tv_BlkOfs & E_GrFsNvtRgrpCchDataAdrMask;

		// read 
		Tv_Hndl		=	GrFsNvtHndlEnc(Tv_DskIdx, Tv_BlkOfs, Tv_BlkIdx, Tv_OvwtId);
		// get cache
		Tv_ErrCode	=	LcCchDataGetByHndlToSctl(A_PtrCtrl, Tv_Hndl);
		if(E_GrErrNone == Tv_ErrCode)
		{
			// found
			// patch offset
			A_PtrCtrl->Prgs	=	0;
			if(A_PtrCtrl->InSize >= Tv_CchOfs)
			{
				A_PtrCtrl->Prgs	=	Tv_CchOfs;
			}

			return	E_GrErrNone;
		}

		// go next address
		Tv_BlkOfs	=	Tv_BlkOfs - E_GrFsNvtRgrpCchDataBufSize;

		while(TRUE)
		{
			// check group status
			if(E_GrDvrRecStatEmpty > m_GrpStat)
			{
				return	E_GrErrDiskStatBad;
			}

			// check exist block
			if(0 <= Tv_BlkOfs)
			{
				Tv_Hndl	=	GrFsNvtHndlEnc(Tv_DskIdx, Tv_BlkOfs, Tv_BlkIdx, Tv_OvwtId);
				// read cache block
				Tv_ErrCode	=	LcCchDataGetByHndlToSctl(A_PtrCtrl, Tv_Hndl);
				if(E_GrErrNone == Tv_ErrCode)
				{
					// update cache reserve
					LcCchDataReserve(TRUE, A_PtrCtrl->CchRsv, Tv_Hndl);
					break;
				}
			}

			// next block
			if(1 >= Tv_BlkSeq)
			{
				return	E_GrErrEndOfData;
			}

			if(!LcGrpBlkFindBySeqBw(Tv_BlkSeq, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId, (__u32*)&Tv_BlkOfs))
			{
				// finished
#ifdef E_GrFsNvtRgrpDbgMsgView
				DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpReadSctlBwByHndl - next sequence id (%d) not found\n", Tv_BlkSeq);
#endif
				return	E_GrErrEndOfData;
			}

			// patch block offset
			Tv_BlkOfs	=	Tv_BlkOfs & E_GrFsNvtRgrpCchDataAdrMask;

		}

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::ReadFwByTime(Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		__s32	Tv_BlkOfs;
		__u64	Tv_Hndl;
	// code --------------------
		// check time
		if(0 == A_Time)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::ReadFwByTime - input zero time!\n");
			return	E_GrErrBadParam;
		}

		// find time
		if(!LcGrpBlkAdrFindByTimeFw(A_Time, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId, &Tv_BlkOfs))
		{
			return	E_GrErrNotFound;
		}

		// read 
		Tv_Hndl		=	GrFsNvtHndlEnc(Tv_DskIdx, Tv_BlkOfs, Tv_BlkIdx, Tv_OvwtId);
		return	LcGrpReadSctlFwByHndl(A_PtrCtrl, Tv_Hndl);
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::ReadFwStart(Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		__u64	Tv_Hndl;
	// code --------------------

		if(!LcGrpBlkFindOldest(&Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId))
		{
			return	E_GrErrNotFound;
		}

		// read 
		Tv_Hndl		=	GrFsNvtHndlEnc(Tv_DskIdx, 0, Tv_BlkIdx, Tv_OvwtId);

		return	LcGrpReadSctlFwByHndl(A_PtrCtrl, Tv_Hndl);

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchDataUnlockByIdx(__u32 A_CchIdx)
{
	// local -------------------
	// code --------------------
		if(m_CchData.Cnt <= A_CchIdx)
		{
			return;
		}

		if(0 >= m_CchData.Items[A_CchIdx].Locked)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcCchDataUnlockByIdx - cache already unlocked!\n");
			return;
		}

		m_CchData.Items[A_CchIdx].Locked --;

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchDataUnlockByHndl(__u64 A_Hndl)
{
	// local -------------------
		__s32	Tv_CchIdx;
	// code --------------------
		// check valid handle
		if(E_GrDvrRecHndlNone == A_Hndl)
		{
			return;
		}

		// find cache by handle
		Tv_CchIdx	=	LcCchDataFindByHndl(A_Hndl);
		if(0 > Tv_CchIdx)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcCchDataUnlockByHndl - handle not found!\n");
			return;
		}

		LcCchDataUnlockByIdx((__u32)Tv_CchIdx);
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchDataUnlockBySctl(Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
	// local -------------------
	// code --------------------
		// check valid cache
		if(E_GrDvrRecHndlNone == A_PtrCtrl->Hndl)
		{
			return;
		}

		// unlock 
		LcCchDataUnlockByHndl(A_PtrCtrl->Hndl);
		A_PtrCtrl->Hndl		=	E_GrDvrRecHndlNone;
		A_PtrCtrl->PtrBuf	=	NULL;
		A_PtrCtrl->InSize	=	0;
		A_PtrCtrl->Prgs		=	0;

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchDataMarkAccByIdx(__u32 A_CchIdx, BOOL8 A_IsWrite)
{
	// local -------------------
	// code --------------------
		if(m_CchData.Cnt <= A_CchIdx)
		{
			return;
		}

		m_CchData.Items[A_CchIdx].AccTime	=	V_GrFsNvtEnv.Tick100ms;

		if(A_IsWrite)
		{
			m_CchData.Items[A_CchIdx].IsDirt	=	TRUE;
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchDataMarkAccByHndl(__u64 A_Hndl, BOOL8 A_IsWrite)
{
	// local -------------------
		__s32	Tv_CchIdx;
	// code --------------------
		if(E_GrDvrRecHndlNone == A_Hndl)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcCchDataMarkAccByHndl - none handle input!\n");
			return;
		}

		Tv_CchIdx	=	LcCchDataFindByHndl(A_Hndl);
		if(0 > Tv_CchIdx)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcCchDataMarkAccByHndl - bad handle input!\n");
			return;
		}

		LcCchDataMarkAccByIdx((__u32)Tv_CchIdx, A_IsWrite);
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::ReadFwByHndl(Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl)
{
	// local -------------------
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkOfs;

		Def_GrErrCode	Tv_ErrCode;

		__u32	Tv_CchOfs;

		__u64	Tv_Hndl;
	// code --------------------
		if(E_GrDvrRecHndlNone == A_Hndl)
		{
			return	E_GrErrBadParam;
		}

		// make cache handle
		GrFsNvtHndlDec(A_Hndl, &Tv_DskIdx, &Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);
		Tv_CchOfs	=	Tv_BlkOfs & E_GrFsNvtRgrpCchDataOfsMask;
		Tv_BlkOfs	=	Tv_BlkOfs & E_GrFsNvtRgrpCchDataAdrMask;

		Tv_Hndl		=	GrFsNvtHndlEnc(Tv_DskIdx, Tv_BlkOfs, Tv_BlkIdx, Tv_OvwtId);

		// get cache
		Tv_ErrCode	=	LcCchDataGetByHndlToSctl(A_PtrCtrl, Tv_Hndl);
		if(E_GrErrNone == Tv_ErrCode)
		{
			// patch offset
			A_PtrCtrl->Prgs	=	0;
			if(A_PtrCtrl->InSize >= Tv_CchOfs)
			{
				A_PtrCtrl->Prgs	=	Tv_CchOfs;
			}
		}

		return	Tv_ErrCode;

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchDataReserve(BOOL8 A_IsDirBack, __u8 A_ReqCnt, __u64 A_HndlBase)
{
	// local -------------------
		__u8	Tv_ReqCnt;
		__u8	Tv_ReqIdx;
		__u8	Tv_OvwtId;
		BOOL8	Tv_IsAble;
		__s32	Tv_DskIdx;
		__u32	Tv_BlkOfs;
		__s32	Tv_BlkIdx;
	// code --------------------
		if((0 == A_ReqCnt) || (!m_IsCchRsvAble))
		{
			return;
		}

		if(E_GrDvrRecHndlNone == A_HndlBase)
		{
			return;
		}

		// patch request count
		Tv_ReqCnt	=	A_ReqCnt;
		if(E_GrFsNvtRgrpCchDataMaxRsvd <= Tv_ReqCnt)
		{
			Tv_ReqCnt	=	E_GrFsNvtRgrpCchDataMaxRsvd;
		}

		// get base block info
		GrFsNvtHndlDec(A_HndlBase, &Tv_DskIdx, &Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);

		// reset reserved
		m_CchData.Rsvd.IsDirBack	=	A_IsDirBack;
		m_CchData.Rsvd.Prgs				=	0;
		m_CchData.Rsvd.ReqCnt			=	Tv_ReqCnt;
		m_CchData.Rsvd.HndlBase		=	A_HndlBase;

		Tv_IsAble	=	TRUE;

		for( Tv_ReqIdx = 0; Tv_ReqIdx < Tv_ReqCnt; Tv_ReqIdx++)
		{
			if(Tv_IsAble)
			{
				if(A_IsDirBack)
				{
					// back direction
					if(E_GrFsNvtRgrpCchDataBufSize < Tv_BlkOfs)
					{
						Tv_BlkOfs	=	Tv_BlkOfs - E_GrFsNvtRgrpCchDataBufSize;
					}
					else
					{
						Tv_IsAble	=	FALSE;
					}
				}
				else
				{
					// forward direction
					if((__u32)E_GrFsNvtBlkSize > Tv_BlkOfs)
					{
						Tv_BlkOfs	=	Tv_BlkOfs + E_GrFsNvtRgrpCchDataBufSize;
					}
					else
					{
						Tv_IsAble	=	FALSE;
					}
				}
			}

			if(Tv_IsAble)
			{
				m_CchData.Rsvd.Items[Tv_ReqIdx].Hndl	=	GrFsNvtHndlEnc(Tv_DskIdx, Tv_BlkOfs, Tv_BlkIdx, Tv_OvwtId);
			}
			else
			{
				m_CchData.Rsvd.Items[Tv_ReqIdx].Hndl	=	E_GrDvrRecHndlNone;
			}
		}

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::ReadFwNext(Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		__u32	Tv_BlkOfs;
		__u32	Tv_ReadSize;
		__u64	Tv_Hndl;

	// code --------------------

		// check handle
		if(E_GrDvrRecHndlNone == A_PtrCtrl->Hndl)
		{
#ifdef E_GrFsNvtRgrpDbgMsgView
			DbgMsgPrint("Cls_GrFsNvtRgrp::ReadFwNext - bad handle input!\n");
#endif
			return	E_GrErrBadParam;
		}

		// get block info
		GrFsNvtHndlDec(A_PtrCtrl->Hndl, &Tv_DskIdx, &Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);
		Tv_BlkSeq		=	A_PtrCtrl->BlkSeq;

		// check block offset
#ifdef E_GrFsNvtRgrpDbgMsgView
		if(0 != (E_GrFsNvtRgrpCchDataOfsMask & Tv_BlkOfs))
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::ReadFwNext - handle address not aligned!\n");
			Tv_BlkOfs	=	Tv_BlkOfs & E_GrFsNvtRgrpCchDataAdrMask;
		}
#endif

		// check changed now block
		if(E_GrFsNvtRgrpCchDataBufSize > A_PtrCtrl->InSize)
		{
			Tv_ReadSize	=	LcCchDataCalSizeByHndl(A_PtrCtrl->Hndl);
			if(Tv_ReadSize  > A_PtrCtrl->InSize)
			{
				// patch size
				A_PtrCtrl->InSize		=	Tv_ReadSize;
				LcCchDataMarkAccByHndl(A_PtrCtrl->Hndl, FALSE);
				return	E_GrErrNone;
			}
		}

		// go next address
		Tv_BlkOfs	=	Tv_BlkOfs + E_GrFsNvtRgrpCchDataBufSize;

		while(TRUE)
		{
			// check group status
			if(E_GrDvrRecStatEmpty > m_GrpStat)
			{
				DbgMsgPrint("Cls_GrFsNvtRgrp::ReadFwNext - disk stat bad\n", Tv_BlkSeq);
				return	E_GrErrDiskStatBad;
			}

			// check exist block
			if(E_GrFsNvtBlkOfsLimit > Tv_BlkOfs)
			{
				Tv_Hndl	=	GrFsNvtHndlEnc(Tv_DskIdx, Tv_BlkOfs, Tv_BlkIdx, Tv_OvwtId);
				// read cache block
				Tv_ErrCode	=	LcCchDataGetByHndlToSctl(A_PtrCtrl, Tv_Hndl);
				if(E_GrErrNone == Tv_ErrCode)
				{
					// reset progress
					A_PtrCtrl->Prgs		=	0;
					// update cache reserve
					LcCchDataReserve(FALSE, A_PtrCtrl->CchRsv, Tv_Hndl);
					break;
				}
			}

			// next block
			Tv_BlkOfs		=	0;
			if(Tv_BlkSeq >= m_RecSeqId)
			{
				return	E_GrErrEndOfData;
			}

			Tv_BlkSeq ++;
			if(!LcGrpBlkFindBySeqFw(Tv_BlkSeq, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId))
			{
				// finished
#ifdef E_GrFsNvtRgrpDbgMsgView
				DbgMsgPrint("Cls_GrFsNvtRgrp::ReadFwNext - next sequence id (%d) not found\n", Tv_BlkSeq);
#endif
				return	E_GrErrEndOfData;
			}

		}
								
		return	E_GrErrNone;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpBlkFindBySeqBw(__u32 A_SeqId, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId, __u32* A_PtrRtOfs)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsExit;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
		__s32	Tv_FndDskIdx;
		__s32	Tv_FndBlkIdx;
		__u32	Tv_FndBlkSeq;
		__u32	Tv_FndInSize;
		__u8	Tv_FndOvwtId;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_IsExit	=	FALSE;
		Tv_FndDskIdx	=	-1;
		Tv_FndBlkIdx	=	-1;
		Tv_FndBlkSeq	=	0;
		Tv_FndOvwtId	=	0;
		Tv_FndInSize	=	0;

		if(0 != m_DiskCnt)
		{
			// check time range
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				// get disk block head table
				Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrBi)
				{
					// check block
					for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					{
						// check block type
						if((E_GrFsNvtBlkStatBad != Tv_PtrBi[Tv_BlkIdx].Db.BlkStat) && (0 != Tv_PtrBi[Tv_BlkIdx].Db.SeqId))
						{
							// check sequence id
							if(Tv_PtrBi[Tv_BlkIdx].Db.SeqId == A_SeqId)
							{
								// found
								Tv_Result	=	TRUE;
								Tv_IsExit	=	TRUE;

								Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
								Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
								Tv_FndBlkSeq	=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
								Tv_FndOvwtId	=	Tv_PtrBi[Tv_BlkIdx].Db.OvwtId;

								break;
							}
							else if(Tv_PtrBi[Tv_BlkIdx].Db.SeqId < A_SeqId)
							{
								if(0 > Tv_FndDskIdx)
								{
									Tv_Result	=	TRUE;
									Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
									Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
									Tv_FndBlkSeq	=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
									Tv_FndOvwtId	=	Tv_PtrBi[Tv_BlkIdx].Db.OvwtId;
									Tv_FndInSize	=	Tv_PtrBi[Tv_BlkIdx].Db.InSize;
								}
								else
								{
									if(Tv_PtrBi[Tv_BlkIdx].Db.SeqId > Tv_FndBlkSeq)
									{
										Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
										Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
										Tv_FndBlkSeq	=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
										Tv_FndOvwtId	=	Tv_PtrBi[Tv_BlkIdx].Db.OvwtId;
										Tv_FndInSize	=	Tv_PtrBi[Tv_BlkIdx].Db.InSize;
									}
								}
							}
						}
					}	//	for( Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
				}
				if(Tv_IsExit)
				{
					break;
				}
			}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		}

		// update
		*A_PtrRtDskIdx	=	Tv_FndDskIdx;
		*A_PtrRtBlkIdx	=	Tv_FndBlkIdx;
		*A_PtrRtBlkSeq	=	Tv_FndBlkSeq;
		*A_PtrRtOvwtId	=	Tv_FndOvwtId;
		*A_PtrRtOfs			=	Tv_FndInSize;

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::ReadBwByTime(Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		__s32	Tv_BlkOfs;
		__u64	Tv_Hndl;
	// code --------------------

		// check time
		if(0 == A_Time)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::ReadBwByTime - input zero time!\n");
			return	E_GrErrBadParam;
		}

		// find time
		if(!LcGrpBlkAdrFindByTimeBw(A_Time, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId, &Tv_BlkOfs))
		{
			return	E_GrErrNotFound;
		}

		// read 
		Tv_Hndl		=	GrFsNvtHndlEnc(Tv_DskIdx, Tv_BlkOfs, Tv_BlkIdx, Tv_OvwtId);
		return	LcGrpReadSctlBwByHndl(A_PtrCtrl, Tv_Hndl);

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::ReadBwNext(Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		__s32	Tv_BlkOfs;
		__u64	Tv_Hndl;

	// code --------------------

		// check handle
		if(E_GrDvrRecHndlNone == A_PtrCtrl->Hndl)
		{
#ifdef E_GrFsNvtRgrpDbgMsgView
			DbgMsgPrint("Cls_GrFsNvtRgrp::ReadBwNext - bad handle input!\n");
#endif
			return	E_GrErrBadParam;
		}

		// get block info
		GrFsNvtHndlDec(A_PtrCtrl->Hndl, &Tv_DskIdx, (__u32*)&Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);
		Tv_BlkSeq		=	A_PtrCtrl->BlkSeq;

		// check block offset
#ifdef E_GrFsNvtRgrpDbgMsgView
		if(0 != (E_GrFsNvtRgrpCchDataOfsMask & Tv_BlkOfs))
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::ReadBwNext - handle address not aligned!\n");
			Tv_BlkOfs	=	Tv_BlkOfs & E_GrFsNvtRgrpCchDataAdrMask;
		}
#endif

		// go next address
		Tv_BlkOfs	=	Tv_BlkOfs - E_GrFsNvtRgrpCchDataBufSize;

		while(TRUE)
		{
			// check group status
			if(E_GrDvrRecStatEmpty > m_GrpStat)
			{
				return	E_GrErrDiskStatBad;
			}

			// check exist block
			if(0 <= Tv_BlkOfs)
			{
				Tv_Hndl	=	GrFsNvtHndlEnc(Tv_DskIdx, Tv_BlkOfs, Tv_BlkIdx, Tv_OvwtId);
				// read cache block
				Tv_ErrCode	=	LcCchDataGetByHndlToSctl(A_PtrCtrl, Tv_Hndl);
				if(E_GrErrNone == Tv_ErrCode)
				{
					// update cache reserve
					LcCchDataReserve(TRUE, A_PtrCtrl->CchRsv, Tv_Hndl);
					break;
				}
			}

			// next block
			if(1 >= m_RecSeqId)
			{
				return	E_GrErrEndOfData;
			}

			Tv_BlkSeq --;
			if(!LcGrpBlkFindBySeqBw(Tv_BlkSeq, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId, (__u32*)&Tv_BlkOfs))
			{
				// finished
#ifdef E_GrFsNvtRgrpDbgMsgView
				DbgMsgPrint("Cls_GrFsNvtRgrp::ReadFwNext - next sequence id (%d) not found\n", Tv_BlkSeq);
#endif
				return	E_GrErrEndOfData;
			}

			// patch block offset
			Tv_BlkOfs	=	Tv_BlkOfs & E_GrFsNvtRgrpCchDataAdrMask;

		}

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtRgrp::GroupInfoGet(__u64* A_PtrRtTotal, __u64* A_PtrRtRec, BOOL8 A_IsRefresh)
{
	// local -------------------
	// code --------------------
		if(A_IsRefresh)
		{
			LcGrpStatUpdate();
		}
		*A_PtrRtTotal	=	m_GrpTotalSize;
		*A_PtrRtRec		=	m_GrpRecSize;

		return	m_GrpStat;

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::RecTimeRangeGet(__u32* A_PtrRtTimeSt, __u32* A_PtrRtTimeEd)
{
	// local -------------------
	// code --------------------
		*A_PtrRtTimeSt		=	m_RecTimeSt;
		*A_PtrRtTimeEd		=	m_RecTimeEd;

}
//--------------------------------------------------------------------
__u32	Cls_GrFsNvtRgrp::RecTimeLastGet(void)
{
		return	m_RecTimeLast;
}
//--------------------------------------------------------------------
__u32	Cls_GrFsNvtRgrp::RecMonthDayMapGet(__u32 A_TimeMonth)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_DskIdx;
		__u32	Tv_TimeSt;
		__u32	Tv_TimeEd;

		__u16	Tv_Year;
		__u8	Tv_Month;
		__u8	Tv_Day;
		__u8	Tv_Hour;
		__u8	Tv_Min;
		__u8	Tv_Sec;

		BOOL8	Tv_IsFix;

		__u32	Tv_DayMask;
		__u32	Tv_DayIdx;
		Ptr_GrFsNvtDayInfo	Tv_DayTbl;
		Ptr_GrFsNvtMinuteInfo	Tv_MnutTbl;
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------

		if(0 == m_DiskCnt)
		{
			return	0;
		}

		Tv_Result		=	0;

		// calculate time of day
		GrTimeDec(A_TimeMonth, &Tv_Year, &Tv_Month, &Tv_Day, &Tv_Hour, &Tv_Min, &Tv_Sec);

		Tv_TimeSt	=	GrTimeEnc(Tv_Year, Tv_Month, 1,0,0,0);
		Tv_Day	=	GrTimeGetMonthMaxDay(Tv_Year, Tv_Month);
		Tv_TimeEd	=	GrTimeEnc(Tv_Year, Tv_Month, Tv_Day, 0, 0, 0);

		for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		{
			// get disk day table
			Tv_DayTbl	=	m_DiskTbl[Tv_DskIdx]->DayInfoTableGet();
			for(Tv_DayIdx = 0; Tv_DayIdx < E_GrFsNvtDayTblCntOfDisk; Tv_DayIdx++)
			{
				// day
				if((Tv_DayTbl[Tv_DayIdx].TimeDay >= Tv_TimeSt) && (Tv_DayTbl[Tv_DayIdx].TimeDay <= Tv_TimeEd))
				{
					Tv_DayMask	=	1 << ((Tv_DayTbl[Tv_DayIdx].TimeDay - Tv_TimeSt) / E_GrTimeSecOfDay);
					if(0 == (Tv_DayMask & Tv_Result))
					{
						// load minute table
						Tv_MnutTbl	=	LcDskMinuteTblGetByIdx(Tv_DskIdx, Tv_DayIdx, &Tv_ErrCode,FALSE);
						if(E_GrErrNone == Tv_ErrCode)
						{
							// check minute info
							if(LcMinuteTblChkAndFix(Tv_DskIdx,Tv_MnutTbl,TRUE,&Tv_IsFix))
							{
								Tv_Result	=	Tv_Result | Tv_DayMask;
							}
							if(Tv_IsFix)
							{
								LcCchMinAccessUpdate(Tv_DskIdx, Tv_DayIdx, TRUE);
							}
						}
						else
						{
							// bad disk
							break;
						}
					}
				}

			}	//	for( Tv_DayIdx = 0; Tv_DayIdx < Tv_DayCnt; Tv_DayIdx++)
		}	//	for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcDskDayMinMapGet(__u32 A_DskIdx, __u32 A_TimeDay, Ptr_GrDvrRecMinMap A_PtrMinMap)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_ChIdx;
		BOOL8	Tv_IsFix;
		__s32	Tv_DayIdx;
		__u32	Tv_MinIdx;
		Def_GrErrCode	Tv_ErrCode;
		Ptr_GrFsNvtMinuteInfo	Tv_MnutTbl;
	// code --------------------
		// check disk index
		if((__u32)m_DiskCnt <= A_DskIdx)
		{
			return	FALSE;
		}

		// check disk status
		if(E_GrDvrRecStatEmpty > m_DiskTbl[A_DskIdx]->DiskStatGet())
		{
			return	FALSE;
		}

		Tv_DayIdx	=	LcDskDayFind(A_DskIdx, A_TimeDay);
		if(0 > Tv_DayIdx)
		{
			return	FALSE;
		}

		// get minmap
		Tv_MnutTbl	=	LcDskMinuteTblGetByIdx(A_DskIdx, (__u32)Tv_DayIdx, &Tv_ErrCode,FALSE);
		if(NULL == Tv_MnutTbl)
		{
			return	FALSE;
		}

		// fix
		Tv_Result	=	FALSE;
		if(LcMinuteTblChkAndFix(A_DskIdx, Tv_MnutTbl, FALSE, &Tv_IsFix))
		{
			// do check
			for(Tv_MinIdx = 0; Tv_MinIdx < E_GrTimeMinOfDay; Tv_MinIdx++)
			{
				// check exist
				if((0 != Tv_MnutTbl[Tv_MinIdx].AdrMin.Idx) || (0 != Tv_MnutTbl[Tv_MinIdx].Adr30s.Idx))
				{
					// add data
					for(Tv_ChIdx = 0; Tv_ChIdx < E_GrDvrMaxChCnt; Tv_ChIdx++)
					{
						A_PtrMinMap[Tv_MinIdx].ChAtb[Tv_ChIdx]	=	A_PtrMinMap[Tv_MinIdx].ChAtb[Tv_ChIdx] | Tv_MnutTbl[Tv_MinIdx].ChRatbTbl[Tv_ChIdx];
					}
				}
			}
			Tv_Result	=	TRUE;
		}

		// cache access upate
		if(Tv_IsFix)
		{
			LcCchMinAccessUpdate(A_DskIdx, (__u32)Tv_DayIdx, Tv_IsFix);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrFsNvtRgrp::LcDskExpSizeGet(__u32 A_DskIdx, __u32 A_TimeSt, __u32 A_TimeEd, __u32 A_ChMap)
{
	// local -------------------
		__u64	Tv_Result;
		__u32	Tv_ChIdx;
		__u32	Tv_ChMask;
		BOOL8	Tv_IsFix;
		__u32	Tv_DayIdx;
		__u32	Tv_MinEd;
		__u32	Tv_MinIdx;

		Def_GrErrCode	Tv_ErrCode;
		Ptr_GrFsNvtDayInfo	Tv_DayTbl;
		Ptr_GrFsNvtMinuteInfo	Tv_MnutTbl;
	// code --------------------

		// check disk index
		if((__u32)m_DiskCnt <= A_DskIdx)
		{
			return	0;
		}

		// check disk status
		if(E_GrDvrRecStatEmpty > m_DiskTbl[A_DskIdx]->DiskStatGet())
		{
			return	0;
		}

		Tv_Result	=	0;

		// get day info
		Tv_DayTbl	=	m_DiskTbl[A_DskIdx]->DayInfoTableGet();
		for(Tv_DayIdx = 0; Tv_DayIdx < E_GrFsNvtDayTblCntOfDisk; Tv_DayIdx++)
		{
			if(0 == Tv_DayTbl[Tv_DayIdx].TimeDay)
			{
				continue;
			}
			// check in range
			if(((Tv_DayTbl[Tv_DayIdx].TimeDay >= A_TimeSt) && (Tv_DayTbl[Tv_DayIdx].TimeDay <= A_TimeEd)) ||
				(((Tv_DayTbl[Tv_DayIdx].TimeDay + E_GrTimeSecOfDay - 1) >= A_TimeSt) && ((Tv_DayTbl[Tv_DayIdx].TimeDay + E_GrTimeSecOfDay - 1) <= A_TimeEd)) ||
				((Tv_DayTbl[Tv_DayIdx].TimeDay >= A_TimeSt) && ((Tv_DayTbl[Tv_DayIdx].TimeDay + E_GrTimeSecOfDay - 1) <= A_TimeEd)) ||
				((Tv_DayTbl[Tv_DayIdx].TimeDay <= A_TimeSt) && ((Tv_DayTbl[Tv_DayIdx].TimeDay + E_GrTimeSecOfDay - 1) >= A_TimeEd)))
			{
				// get minut table
				Tv_MnutTbl	=	LcDskMinuteTblGetByIdx(A_DskIdx, Tv_DayIdx, &Tv_ErrCode,FALSE);
				if(E_GrErrNone != Tv_ErrCode)
				{
					continue;
				}
				// patch
				if(LcMinuteTblChkAndFix(A_DskIdx,Tv_MnutTbl,FALSE,&Tv_IsFix))
				{
					// do check
					Tv_MinIdx		=	0;
					if(Tv_DayTbl[Tv_DayIdx].TimeDay < A_TimeSt)
					{
						Tv_MinIdx	=	(A_TimeSt - Tv_DayTbl[Tv_DayIdx].TimeDay) / E_GrTimeSecOfMin;
					}
					Tv_MinEd		=	E_GrTimeMinOfDay;
					if((Tv_DayTbl[Tv_DayIdx].TimeDay + E_GrTimeSecOfDay) > A_TimeEd)
					{
						Tv_MinEd	=	(A_TimeEd - Tv_DayTbl[Tv_DayIdx].TimeDay) / E_GrTimeSecOfMin;
					}

					while(Tv_MinIdx < Tv_MinEd)
					{
						// check exist
						if((0 != Tv_MnutTbl[Tv_MinIdx].AdrMin.Idx) || (0 != Tv_MnutTbl[Tv_MinIdx].Adr30s.Idx))
						{
							// add data
							Tv_ChMask	=	1;
							for(Tv_ChIdx = 0; Tv_ChIdx < E_GrDvrMaxChCnt; Tv_ChIdx++)
							{
								if(0 != (Tv_ChMask & A_ChMap))
								{
									Tv_Result	=	Tv_Result + (__u64)Tv_MnutTbl[Tv_MinIdx].ChSizeTbl[Tv_ChIdx];
								}
								// next
								Tv_ChMask	=	Tv_ChMask << 1;
							}
						}

						// next
						Tv_MinIdx ++;
					}

				}
				// update access
				LcCchMinAccessUpdate(A_DskIdx, Tv_DayIdx, Tv_IsFix);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrFsNvtDbBlkInfo	Cls_GrFsNvtRgrp::LcDskDbBlkInfoGet(__u32 A_DskIdx, __u32 A_BlkIdx)
{
	// local -------------------
		Ptr_GrFsNvtDbBlkInfo		Tv_Result;
		Ptr_GrFsNvtBlkInfoBase	Tv_BiTbl;
		__u32	Tv_BlkCnt;
	// code --------------------
		Tv_Result	=	NULL;

		if(A_DskIdx < m_DiskCnt)
		{
			Tv_BiTbl	=	m_DiskTbl[A_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
			if((1 < Tv_BlkCnt) && (NULL != Tv_BiTbl))
			{
				if((0 < A_BlkIdx) && (A_BlkIdx < Tv_BlkCnt))
				{
					Tv_Result	=	&Tv_BiTbl[A_BlkIdx].Db;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::RecMinRatbTblGet(__u32 A_TimeDay, __u32 A_MinCnt, Ptr_GrDvrRecMinMap A_PtrMinMap)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_DskIdx;
	// code --------------------
		// check parametter
		if(0 == A_MinCnt)
		{
			return	FALSE;
		}
		// reset table
		//GrDumyZeroMem(A_PtrMinMap, sizeof(St_GrDvrRecMinMap)* A_MinCnt);

		if(0 == m_DiskCnt)
		{
			return	FALSE;
		}

		Tv_Result	=	FALSE;
		for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		{
			// check exist disk
			if(NULL != m_DiskTbl[Tv_DskIdx])
			{
				if(LcDskDayMinMapGet(Tv_DskIdx, A_TimeDay, A_PtrMinMap))
				{
					Tv_Result	=	TRUE;
				}
			}
		}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::RecFlush(void)
{
	// local -------------------
	// code --------------------

		LcCchMinFlushAll();

		if(NULL != m_RecBlkEnv.PtrDb)
		{
			if(0 <= m_RecBlkEnv.PtrDb->OfsLastLog)
			{
				LcGrpRecLogPoolWrite();
			}

			if(m_RecBlkEnv.IsDbChg)
			{
				if((0 <= m_RecDskIdx) && (0 <= m_RecBlkIdx))
				{
					// update DB
					if(E_GrErrNone != m_DiskTbl[m_RecDskIdx]->BlkInfoWriteByIdx((__u32)m_RecBlkIdx))
					{
						DbgMsgPrint("Cls_GrFsNvtRgrp::RecFlush - disk %d block %d block info write fail!\n",
							m_DiskTbl[m_RecDskIdx]->DevIdxGet(), m_RecBlkIdx);
					}
				}
				m_RecBlkEnv.IsDbChg	=	FALSE;
			}
		}
		
		LcCchDataFlushAll();

		LcGrpStatUpdate();

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::RtlLogReadProc(Ptr_GrFsNvtIoLogGetRtl A_PtrCtl, Ptr_GrFsNvtFrmLogPool A_PtrFrmBuf)
{
	// local -------------------
		Ptr_GrFsNvtBlkInfoBase Tv_PtrBi;
		__u32	Tv_BlkCnt;
		Def_GrErrCode	Tv_ErrCode;
		__u32	Tv_ItmIdx;
		BOOL8	Tv_IsAble;
		__u64	Tv_TypeMask;
		__u32	Tv_ChMask;

	// code --------------------

		// check done
		if(A_PtrCtl->IsDone)
		{
			return;
		}

		while(TRUE)
		{
			// check disk position
			if(m_DiskCnt <= A_PtrCtl->DskIdx)
			{
				A_PtrCtl->IsDone	=	TRUE;
				return;
			}

			// find disk and block
			Tv_PtrBi	=	m_DiskTbl[A_PtrCtl->DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
			if(NULL == Tv_PtrBi)
			{
				// go next disk
				A_PtrCtl->BlkIdx	=	1;
				A_PtrCtl->LogOfs	=	-1;
				A_PtrCtl->DskIdx ++;
				continue;
			}

			// check block time range
			while(A_PtrCtl->BlkIdx < Tv_BlkCnt)
			{
				// check block time range
				if(((A_PtrCtl->TimeSt <= Tv_PtrBi[A_PtrCtl->BlkIdx].Db.TimeRng.TimeSt) && (A_PtrCtl->TimeEd >= Tv_PtrBi[A_PtrCtl->BlkIdx].Db.TimeRng.TimeSt)) ||
					((A_PtrCtl->TimeSt <= Tv_PtrBi[A_PtrCtl->BlkIdx].Db.TimeRng.TimeEd) && (A_PtrCtl->TimeEd >= Tv_PtrBi[A_PtrCtl->BlkIdx].Db.TimeRng.TimeEd)) ||
					((A_PtrCtl->TimeSt <= Tv_PtrBi[A_PtrCtl->BlkIdx].Db.TimeRng.TimeSt) && (A_PtrCtl->TimeEd >= Tv_PtrBi[A_PtrCtl->BlkIdx].Db.TimeRng.TimeEd)) ||
					((A_PtrCtl->TimeSt >= Tv_PtrBi[A_PtrCtl->BlkIdx].Db.TimeRng.TimeSt) && (A_PtrCtl->TimeEd <= Tv_PtrBi[A_PtrCtl->BlkIdx].Db.TimeRng.TimeEd)))
				{
					// found
					break;
				}
				// next
				A_PtrCtl->BlkIdx ++;
			}

			// check found
			if(A_PtrCtl->BlkIdx >= Tv_BlkCnt)
			{
				// go next disk
				A_PtrCtl->BlkIdx	=	1;
				A_PtrCtl->LogOfs	=	-1;
				A_PtrCtl->DskIdx ++;
				continue;
			}

			// load next log
			if(0 > A_PtrCtl->LogOfs)
			{
				// fisrt log
				A_PtrCtl->LogOfs	=	Tv_PtrBi[A_PtrCtl->BlkIdx].Db.OfsLastLog;
				if(0 > A_PtrCtl->LogOfs)
				{
					// not exist log, go next block
					A_PtrCtl->LogOfs	=	-1;
					A_PtrCtl->BlkIdx ++;
					continue;
				}
			}

			// load frame
			Tv_ErrCode	=	LcDskReadToBuf(A_PtrCtl->DskIdx, A_PtrCtl->BlkIdx, (__u32)A_PtrCtl->LogOfs, sizeof(St_GrFsNvtFrmLogPool), A_PtrFrmBuf);
			if(E_GrErrNone != Tv_ErrCode)
			{
				// go next block
				DbgMsgPrint("[LogGet] frame read fail(err:%d)\n", Tv_ErrCode);
				A_PtrCtl->LogOfs	=	-1;
				A_PtrCtl->BlkIdx ++;
				continue;
			}

			// check frame
			if((E_GrDvrFrmHeadFcc != A_PtrFrmBuf->Head.Base.Fcc) || (E_GrDvrFrmTypeLogPool != A_PtrFrmBuf->Head.Base.Type))
			{
				// go next block
				DbgMsgPrint("Cls_GrFsNvtRgrp::LogReadByCtrl - bad frame(d:%d b:%d) head, go next block!\n",
					m_DiskTbl[A_PtrCtl->DskIdx]->DevIdxGet(), A_PtrCtl->BlkIdx);
				A_PtrCtl->LogOfs	=	-1;
				A_PtrCtl->BlkIdx ++;
				continue;
			}

			// check item is full
			if(A_PtrCtl->InCnt >= A_PtrCtl->MaxCnt)
			{
				// full, finish
				A_PtrCtl->IsDone	=	TRUE;
				if(A_PtrCtl->IsFrmUnit)
				{
					DbgMsgPrint("[BUG] Impossible! frame mode buffer is full!\n");
				}
				return;
			}

			// extract current time item
			if(0 != A_PtrFrmBuf->Head.InCnt)
			{
				for(Tv_ItmIdx = 0; Tv_ItmIdx < A_PtrFrmBuf->Head.InCnt; Tv_ItmIdx++)
				{
					// find item
					Tv_IsAble	=	TRUE;
					// check time range
					if((A_PtrCtl->TimeSt > A_PtrFrmBuf->LogTbl[Tv_ItmIdx].Time) || (A_PtrCtl->TimeEd < A_PtrFrmBuf->LogTbl[Tv_ItmIdx].Time))
					{
						Tv_IsAble		=	FALSE;
					}
					// check frame ty[e
					Tv_TypeMask	=	(__u64)1 << (A_PtrFrmBuf->LogTbl[Tv_ItmIdx].LogType - 1);
					if(0 == (Tv_TypeMask & A_PtrCtl->TypeMask))
					{
						Tv_IsAble		=	FALSE;
					}
					// check channel
					Tv_ChMask	=	1 << A_PtrFrmBuf->LogTbl[Tv_ItmIdx].Ch;
					if(0 == (Tv_ChMask & A_PtrCtl->ChMask))
					{
						Tv_IsAble		=	FALSE;
					}
					// add item
					if(Tv_IsAble)
					{
						GrDumyCopyMem(&A_PtrCtl->PtrRtBuf[A_PtrCtl->InCnt], &A_PtrFrmBuf->LogTbl[Tv_ItmIdx],
							sizeof(St_GrDvrLogItm));
						A_PtrCtl->InCnt ++;
						if(A_PtrCtl->InCnt >= A_PtrCtl->MaxCnt)
						{
							// check frame mode
							if(!A_PtrCtl->IsFrmUnit)
							{
								// do finish
								A_PtrCtl->IsDone	=	TRUE;
								return;
							}
							break;
						}
					}
				}
			}

			// update next frame
			if(0 > A_PtrFrmBuf->Head.OfsPrv)
			{
				// go next block
				A_PtrCtl->LogOfs	=	-1;
				A_PtrCtl->BlkIdx ++;

				// check fraem mode
				if(A_PtrCtl->IsFrmUnit)
				{
					if(0 != A_PtrCtl->InCnt)
					{
						A_PtrCtl->IsWait	=	TRUE;	// wait copy
					}
				}
				return;
			}

			// check next pointer current
			if(A_PtrFrmBuf->Head.OfsPrv >= A_PtrCtl->LogOfs)
			{
				DbgMsgPrint("Cls_GrFsNvtRgrp::LogReadByCtrl - bad previous address (now:%08X , prv:%08X)!\n", A_PtrCtl->LogOfs, A_PtrFrmBuf->Head.OfsPrv);
				// go next block
				A_PtrCtl->LogOfs	=	-1;
				A_PtrCtl->BlkIdx ++;

				// check fraem mode
				if(A_PtrCtl->IsFrmUnit)
				{
					if(0 != A_PtrCtl->InCnt)
					{
						A_PtrCtl->IsWait	=	TRUE;	// wait copy
					}
				}
				return;
			}

			// update frame address
			A_PtrCtl->LogOfs	=	A_PtrFrmBuf->Head.OfsPrv;

			// check fraem mode
			if(A_PtrCtl->IsFrmUnit)
			{
				if(0 != A_PtrCtl->InCnt)
				{
					A_PtrCtl->IsWait	=	TRUE;	// wait copy
				}
			}
			break;

		}	// while(TRUE)

}
//--------------------------------------------------------------------
__u64	Cls_GrFsNvtRgrp::RecExpectSizeGet(__u32 A_TimeSt, __u32 A_TimeEd, __u32 A_ChMap)
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeSt;
		Def_GrTime	Tv_TimeEd;
		__u32	Tv_DskIdx;
	// code --------------------

		// check time range
		if((0 == A_ChMap) || (A_TimeSt > A_TimeEd))
		{
			return	0;
		}
		if(0 == m_DiskCnt)
		{
			return	0;
		}

		Tv_Result	=	0;

		// calculate time
		Tv_TimeSt	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin);
		Tv_TimeEd	=	A_TimeEd - (A_TimeEd % E_GrTimeSecOfMin) + E_GrTimeSecOfMin - 1;

		for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		{
			// check exist disk
			if(NULL != m_DiskTbl[Tv_DskIdx])
			{
				Tv_Result	=	Tv_Result + LcDskExpSizeGet(Tv_DskIdx, Tv_TimeSt, Tv_TimeEd, A_ChMap);
			}
		}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::RecBufAdd(iovec* A_PtrIovs, __u32 A_Cnt)
{
	// local -------------------
		__u8	Tv_ErrCode;
		BOOL8	Tv_IsFull;
		BOOL8	Tv_IsBad;
		__u8	Tv_OvwtId;

		__s32	Tv_LastDsk;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;

		__u32	Tv_SeqId;

	// code --------------------
		// check count
		if (0 == A_Cnt)
		{
			return;
		}

		// initialize
		m_RbwCtl.IocPrgs		=	0;
		m_RbwCtl.IocCnt			=	A_Cnt;
		m_RbwCtl.IocTbl			=	A_PtrIovs;

		Tv_LastDsk	=	m_RecDskIdx;	// get last disk

		while (E_GrDvrRecStatEmpty <= m_GrpStat)
		{
			// check finished
			if (m_RbwCtl.IocPrgs >= m_RbwCtl.IocCnt)
			{
				// work completed
				break;
			}

			// check opened
			if ((0 <= m_RecDskIdx) && (0 <= m_RecBlkIdx))
			{
				// write to block
				Tv_ErrCode	=	LcGrpRbwWriteToRecBlk();
				if (E_GrErrNone == Tv_ErrCode)
				{
					continue;
				}

				// close block
				Tv_IsFull		=	FALSE;
				Tv_IsBad		=	TRUE;
				if (E_GrErrDiskFull == Tv_ErrCode)
				{
					Tv_IsFull		=	TRUE;
					Tv_IsBad		=	FALSE;
				}

				LcGrpRecBlkClose(Tv_IsFull, Tv_IsBad);
			}

			// open empty block now disk
			if (0 <= Tv_LastDsk)
			{
				Tv_BlkIdx	=	LcDskBlkFindEmpty((__u32)Tv_LastDsk);
				if (0 <= Tv_BlkIdx)
				{
					// create record block
					LcGrpRecBlkNew((__u32)Tv_LastDsk, (__u32)Tv_BlkIdx);
				}
			}

			// check opened
			if ((0 > m_RecDskIdx) || (0 > m_RecBlkIdx))
			{
				// open empty block all disk
				if (LcGrpBlkFindEmpty(&Tv_DskIdx, &Tv_BlkIdx))
				{
					// create record block
					LcGrpRecBlkNew((__u32)Tv_DskIdx, (__u32)Tv_BlkIdx);
				}
				// reset disk index
				Tv_LastDsk	=	-1;
			}

			// check overwrite mode
			if ((0 > m_RecDskIdx) || (0 > m_RecBlkIdx))
			{
				if (m_IsOvwtMode)
				{
					// find oldest block
					if(LcGrpBlkFindOldest(&Tv_DskIdx, &Tv_BlkIdx, &Tv_SeqId, &Tv_OvwtId))
					{
						// open
						LcGrpRecBlkNew((__u32)Tv_DskIdx, (__u32)Tv_BlkIdx);
					}
					else
					{
						// block not exist
						m_GrpStat	=	E_GrDvrRecStatBad;
						break;
					}
				}
				else
				{
					// disk full
					if(E_GrDvrRecStatFull != m_GrpStat)
					{
						St_GrTaskMsgRecGrpFull	Tv_Msg;

						m_GrpStat	=	E_GrDvrRecStatFull;

						// braodcast record group full message
						Tv_Msg.MsgHd.Cmd		=	E_GrTaskMsgRecGrpFull;
						Tv_Msg.GrpType			=	m_GrpType;

						GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_Msg,sizeof(Tv_Msg));
					}
					break;
				}
			}

		}	//	while (E_GrDvrRecStatEmpty <= m_GrpStat)

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpRbwWriteToRecBlk(void)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		__u8	Tv_Ch;

		__u32	Tv_BlkOfs;
		__u32	Tv_InSize;

		__u32	Tv_OfsBegin;		// backup start offset

		__u32	Tv_TmRecMin;
		__u32	Tv_TmRecMax;
		__u32	Tv_TmCanMin;
		__u32	Tv_WkIdx;

		St_GrDvrLogItm	Tv_Log;

		__u32	Tv_CchIns;
		__u32	Tv_CchOfs;
		__s32	Tv_CchIdx;

		void*	Tv_PtrSr;
		void*	Tv_PtrTg;
		__u32	Tv_CpTail;
		__u32	Tv_CpHead;

		__u64	Tv_Hndl;

		__u32	Tv_DbgOldInSize;

		Ptr_GrDvrFrmHdBase	Tv_PtrFrmUnk;
		Ptr_GrDvrFrmHdVdo	Tv_PtrFrmVdo;
		Ptr_GrDvrFrmHdAdo	Tv_PtrFrmAdo;
	// code --------------------
		// check able
		if (m_RbwCtl.IocPrgs >= m_RbwCtl.IocCnt)
		{
			return	E_GrErrNone;
		}
		// init
		Tv_CchIdx			=	-1;

		// init channel table
		Tv_TmRecMin		=	0;
		Tv_TmRecMax		=	0;
		Tv_TmCanMin		=	0;

		for ( Tv_WkIdx = 0; Tv_WkIdx < E_GrDvrMaxChCnt; Tv_WkIdx++)
		{
			m_RbwCtl.ChTbl[Tv_WkIdx].Ratb		=	0;
			m_RbwCtl.ChTbl[Tv_WkIdx].BkSize	=	0;
		}

		// get record block free space
		Tv_InSize		=	0;

		// backup start offset
		Tv_OfsBegin	=	m_RecBlkEnv.PtrDb->InSize;

		// find current cache
		Tv_BlkOfs	=	m_RecBlkEnv.PtrDb->InSize & E_GrFsNvtRgrpCchDataAdrMask;
		Tv_CchOfs	=	m_RecBlkEnv.PtrDb->InSize & E_GrFsNvtRgrpCchDataOfsMask;
		Tv_Hndl		=	GrFsNvtHndlEnc(m_RecDskIdx, Tv_BlkOfs, m_RecBlkIdx, m_RecBlkEnv.PtrDb->OvwtId);

		Tv_CchIdx	=	LcCchDataGetByHndl(Tv_Hndl, FALSE, &Tv_CchIns, &Tv_Result);
		if(0 > Tv_CchIdx)
		{
			return	Tv_Result;
		}

		// check offset and insize
		if(Tv_CchOfs != Tv_CchIns)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRbwWriteToRecBlk - cin (%08X) and cof (%08X) mismatch!\n", Tv_CchIns, Tv_CchOfs);
		}

		// do progress
		Tv_Result			=	E_GrErrNone;
		while (m_RbwCtl.IocPrgs < m_RbwCtl.IocCnt)
		{
			// get frame
			Tv_PtrFrmUnk	=	(Ptr_GrDvrFrmHdBase)m_RbwCtl.IocTbl[m_RbwCtl.IocPrgs].iov_base;

			// check add able
			if((__u32)E_GrFsNvtBlkSize < (m_RecBlkEnv.PtrDb->InSize + (__u32)m_RbwCtl.IocTbl[m_RbwCtl.IocPrgs].iov_len))
			{
				// finish
				Tv_Result		=	E_GrErrDiskFull;
				break;
			}

			// calculate block offset and cache offset
			Tv_BlkOfs	=	m_RecBlkEnv.PtrDb->InSize & E_GrFsNvtRgrpCchDataAdrMask;
			Tv_CchOfs	=	m_RecBlkEnv.PtrDb->InSize & E_GrFsNvtRgrpCchDataOfsMask;

			// do copy
			if(E_GrFsNvtRgrpCchDataBufSize > (Tv_CchOfs + (__u32)m_RbwCtl.IocTbl[m_RbwCtl.IocPrgs].iov_len))
			{
				// direct copy
				Tv_PtrTg	=	(void*)((Def_GrCalPtr)m_CchData.Items[Tv_CchIdx].PtrBuf + (Def_GrCalPtr)Tv_CchOfs);
				GrDumyCopyMem(Tv_PtrTg, Tv_PtrFrmUnk, m_RbwCtl.IocTbl[m_RbwCtl.IocPrgs].iov_len);

				// info update
				m_CchData.Items[Tv_CchIdx].IsDirt		=	TRUE;		// mark dirty
				m_RecBlkEnv.PtrDb->InSize	=	m_RecBlkEnv.PtrDb->InSize + (__u32)m_RbwCtl.IocTbl[m_RbwCtl.IocPrgs].iov_len;
			}
			else
			{
				// debug
				{
					Tv_DbgOldInSize	=	m_RecBlkEnv.PtrDb->InSize;
				}

				// divide
				Tv_CpHead		=	E_GrFsNvtRgrpCchDataBufSize - Tv_CchOfs;
				Tv_CpTail		=	(__u32)m_RbwCtl.IocTbl[m_RbwCtl.IocPrgs].iov_len - Tv_CpHead;

				// copy head
				Tv_PtrTg	=	(void*)((Def_GrCalPtr)m_CchData.Items[Tv_CchIdx].PtrBuf + (Def_GrCalPtr)Tv_CchOfs);
				GrDumyCopyMem(Tv_PtrTg, Tv_PtrFrmUnk, Tv_CpHead);

				// info update
				m_CchData.Items[Tv_CchIdx].IsDirt		=	TRUE;		// mark dirty
				m_RecBlkEnv.PtrDb->InSize	=	m_RecBlkEnv.PtrDb->InSize + Tv_CpHead;

				// unlock cache
				LcCchDataUnlockByIdx((__u32)Tv_CchIdx);

				// change cache
				Tv_BlkOfs	=	Tv_BlkOfs + E_GrFsNvtRgrpCchDataBufSize;
				Tv_Hndl		=	GrFsNvtHndlEnc(m_RecDskIdx, Tv_BlkOfs, m_RecBlkIdx, m_RecBlkEnv.PtrDb->OvwtId);

				Tv_CchIdx	=	LcCchDataGetByHndl(Tv_Hndl, FALSE, &Tv_CchIns, &Tv_Result);
				if(0 > Tv_CchIdx)
				{
					return	Tv_Result;
				}

				// check offset and insize
				if(0 != Tv_CchIns)
				{
					DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpRbwWriteToRecBlk - cin (%08X) is not zero!\n", Tv_CchIns);
					DbgMsgPrint("[DBG] oldin:%08X nowin:%08X bofs:%08X cofs:%08X frmsize:%08X, hd:%08X, tail:%08X \n", 
						Tv_DbgOldInSize, m_RecBlkEnv.PtrDb->InSize, Tv_BlkOfs, Tv_CchOfs,
						m_RbwCtl.IocTbl[m_RbwCtl.IocPrgs].iov_len,Tv_CpHead,Tv_CpTail);
				}

				// copy tail
				Tv_PtrSr	=	(void*)((Def_GrCalPtr)Tv_PtrFrmUnk + (Def_GrCalPtr)Tv_CpHead);
				GrDumyCopyMem(m_CchData.Items[Tv_CchIdx].PtrBuf, Tv_PtrSr, Tv_CpTail);

				// info update
				m_CchData.Items[Tv_CchIdx].IsDirt		=	TRUE;		// mark dirty
				m_RecBlkEnv.PtrDb->InSize	=	m_RecBlkEnv.PtrDb->InSize + Tv_CpTail;
			}

			// check frame type
			if ((E_GrDvrFrmTypeVdoP4x >= Tv_PtrFrmUnk->Type) || (E_GrDvrFrmTypeAdo == Tv_PtrFrmUnk->Type))
			{
				// update time
				if (0 == Tv_TmCanMin )
				{
					// build new time base
					Tv_TmCanMin		=	Tv_PtrFrmUnk->TimeSec - (Tv_PtrFrmUnk->TimeSec % E_GrFsNvtTimeMapSecUnit);
					Tv_TmRecMin		=	Tv_PtrFrmUnk->TimeSec;
					Tv_TmRecMax		=	Tv_PtrFrmUnk->TimeSec;
				}
				else
				{
					// check range
					if ((Tv_TmCanMin > Tv_PtrFrmUnk->TimeSec) || ((Tv_TmCanMin + E_GrFsNvtTimeMapSecUnit) <= Tv_PtrFrmUnk->TimeSec))
					{
						// out of range
						break;
					}
					// update time range
					if (Tv_PtrFrmUnk->TimeSec < Tv_TmRecMin)
					{
						Tv_TmRecMin	=	Tv_PtrFrmUnk->TimeSec;
					}
					if (Tv_PtrFrmUnk->TimeSec > Tv_TmRecMax)
					{
						Tv_TmRecMax	=	Tv_PtrFrmUnk->TimeSec;
					}
				}

				// get channel
				Tv_Ch		=	Tv_PtrFrmUnk->Ch;

				// check detail type
				if (E_GrDvrFrmTypeVdoP4x >= Tv_PtrFrmUnk->Type)
				{
					// video
					Tv_PtrFrmVdo	=	(Ptr_GrDvrFrmHdVdo)m_RbwCtl.IocTbl[m_RbwCtl.IocPrgs].iov_base;
					// update channel info
					m_RbwCtl.ChTbl[Tv_Ch].Ratb		=	m_RbwCtl.ChTbl[Tv_Ch].Ratb | (Tv_PtrFrmVdo->Base.Ratb & E_GrDvrRecAtbNoramlMask);
					// check add log
					if(0 != (Tv_PtrFrmVdo->Base.Ratb & (E_GrDvrRecAtbLogMtn | E_GrDvrRecAtbLogSen | E_GrDvrRecAtbLogSpc)))
					{
						Tv_Log.Ch				=	Tv_Ch;
						Tv_Log.Time			=	Tv_PtrFrmVdo->Base.TimeSec;
						Tv_Log.ParaBt0	=	m_GrpType;
						Tv_Log.ParaWd0	=	m_RecSeqOvwt;
						Tv_Log.ParaWd1	=	0;
						Tv_Log.ParaDw0	=	m_RecDskIdx;
						Tv_Log.ParaDw1	=	m_RecBlkIdx;
						Tv_Log.ParaDw2	=	m_RecBlkEnv.PtrDb->InSize;
						Tv_Log.ParaDw3	=	0;
						Tv_Log.ParaDw4	=	0;
						// add motion
						if(0 != (E_GrDvrRecAtbLogMtn & Tv_PtrFrmVdo->Base.Ratb))
						{
							Tv_Log.LogType	=	E_GrDvrLogTypeMotion;
							LogAdd(&Tv_Log);
						}
						// add sensor
						if(0 != (E_GrDvrRecAtbLogSen & Tv_PtrFrmVdo->Base.Ratb))
						{
							Tv_Log.LogType	=	E_GrDvrLogTypeSensor;
							LogAdd(&Tv_Log);
						}
						// add special sensor
						if(0 != (E_GrDvrRecAtbLogSpc & Tv_PtrFrmVdo->Base.Ratb))
						{
							Tv_Log.LogType	=	E_GrDvrLogTypeSpcSen;
							LogAdd(&Tv_Log);
						}
					}
				}
				else if (E_GrDvrFrmTypeAdo == Tv_PtrFrmUnk->Type)
				{
					// audio
					Tv_PtrFrmAdo	=	(Ptr_GrDvrFrmHdAdo)m_RbwCtl.IocTbl[m_RbwCtl.IocPrgs].iov_base;
				}
				// add backup size
				m_RbwCtl.ChTbl[Tv_Ch].BkSize	=	m_RbwCtl.ChTbl[Tv_Ch].BkSize + Tv_PtrFrmUnk->DataSize + V_GrFsNvtEnv.BkupHdSize;
			}

			// add size
			Tv_InSize		=	Tv_InSize + (__u32)m_RbwCtl.IocTbl[m_RbwCtl.IocPrgs].iov_len;

			// next
			m_RbwCtl.IocPrgs ++;
		}	//	while (m_RbwCtl.IocPrgs < m_RbwCtl.IocCnt)

		// do write
		if (0 != Tv_InSize)
		{
			// update DB change
			m_RecBlkEnv.IsDbChg	=	TRUE;

			// check time info able
			if(0 != Tv_TmCanMin)
			{
				// update block time range
				LcGrpRecAddBlkTimeRange(Tv_TmRecMin, Tv_TmRecMax);

				// add time 30 sec
				LcGrpRecAddTimeByRbw(Tv_TmRecMin, Tv_OfsBegin, m_RbwCtl.ChTbl);
			}
		}

		// unlock cache
		LcCchDataUnlockByIdx(Tv_CchIdx);

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrFsNvtRgrp::LcCchMinFindInCache(__u32 A_DskIdx, __u32 A_DayIdx)
{
	// local -------------------
		__s32	Tv_Result;
		__u32	Tv_ItmIdx;
	// code --------------------
		// check available cache
		if((NULL == m_CchMin.Items) || (0 == m_CchMin.Cnt))
		{
			return	-1;
		}
		
		Tv_Result	=	-1;

		for( Tv_ItmIdx = 0; Tv_ItmIdx < m_CchMin.Cnt; Tv_ItmIdx++)
		{
			// check exist
			if(((__s32)A_DskIdx == m_CchMin.Items[Tv_ItmIdx].DskIdx) && (A_DayIdx == m_CchMin.Items[Tv_ItmIdx].DayIdx))
			{
				// found
				Tv_Result	=	(__s32)Tv_ItmIdx;
				break;
			}

		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
__s32	Cls_GrFsNvtRgrp::LcCchMinLoadToCache(__u32 A_DskIdx, __u32 A_DayIdx, Def_GrErrCode* A_PtrRtErrCode, BOOL8 A_IsReadSkip)
{
	// local -------------------
		__s32	Tv_Result;
		__u32	Tv_ItmIdx;
		__u32	Tv_Tick;
		__u32	Tv_BlkOfs;
	// code --------------------
		// check available cache
		if((NULL == m_CchMin.Items) || (0 == m_CchMin.Cnt))
		{
			*A_PtrRtErrCode	=	E_GrErrInitFail;
			return	-1;
		}

		// check disk status
		if(E_GrDvrRecStatEmpty > m_DiskTbl[A_DskIdx]->DiskStatGet())
		{
			*A_PtrRtErrCode	=	E_GrErrDiskStatBad;
			return	-1;
		}

		Tv_Result	=	-1;

		// find empty cache
		for(Tv_ItmIdx = 0; Tv_ItmIdx < m_CchMin.Cnt; Tv_ItmIdx++)
		{
			// check exist
			if(0 > m_CchMin.Items[Tv_ItmIdx].DskIdx)
			{
				// found
				Tv_Result	=	(__s32)Tv_ItmIdx;
				break;
			}
		}

		// check empty not found
		if(0 > Tv_Result)
		{
			// find oldest cache
			for(Tv_ItmIdx = 0; Tv_ItmIdx < m_CchMin.Cnt; Tv_ItmIdx++)
			{
				// check exist result
				if(0 > Tv_Result)
				{
					Tv_Result	=	(__s32)Tv_ItmIdx;
					Tv_Tick		=	m_CchMin.Items[Tv_ItmIdx].AccTime;
					continue;
				}

				// compare tick
				if(Tv_Tick > m_CchMin.Items[Tv_ItmIdx].AccTime)
				{
					Tv_Result	=	(__s32)Tv_ItmIdx;
					Tv_Tick		=	m_CchMin.Items[Tv_ItmIdx].AccTime;
				}
			}

			// check need flush
			if(m_CchMin.Items[Tv_Result].IsDirt)
			{
				DbgMsgPrint("Cls_GrFsNvtRgrp::LcCchMinLoadToCache() - Tv_Result = %d\n", Tv_Result);
				// flush cache
				LcCchMinFlushByCchIdx((__u32)Tv_Result);
			}
		}

		if(A_IsReadSkip)
		{
			*A_PtrRtErrCode	=	E_GrErrNone;
		}
		else
		{
			// do load
			Tv_BlkOfs	=	E_GrFsNvtBlkOfsMinuteTbl + (A_DayIdx * E_GrFsNvtMitDaySize);
			*A_PtrRtErrCode	=	m_DiskTbl[A_DskIdx]->BlkReadPage(E_GrFsNvtBlkIdxMib, Tv_BlkOfs, m_CchMin.Items[Tv_Result].PtrBuf,
				E_GrFsNvtMitDaySize);
			if(E_GrErrNone != (*A_PtrRtErrCode))
			{
				// update group info
				LcGrpStatUpdate();
			}
		}

		if(E_GrErrNone == (*A_PtrRtErrCode))
		{
			// patch cache
			m_CchMin.Items[Tv_Result].AccTime	=	V_GrFsNvtEnv.Tick100ms;		// update tick
			m_CchMin.Items[Tv_Result].DayIdx	=	A_DayIdx;
			m_CchMin.Items[Tv_Result].DskIdx	=	(__s32)A_DskIdx;
			m_CchMin.Items[Tv_Result].IsDirt	=	FALSE;
		}
		else
		{
			// reset cache
			m_CchMin.Items[Tv_Result].DskIdx	=	-1;
			m_CchMin.Items[Tv_Result].IsDirt	=	FALSE;
			Tv_Result		=	-1;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrFsNvtMinuteInfo	Cls_GrFsNvtRgrp::LcDskMinuteTblGetByIdx(__u32 A_DskIdx, __u32 A_DayIdx, Def_GrErrCode* A_PtrRtErrCode, BOOL8 A_IsReadSkip)
{
	// local -------------------
		__s32	Tv_CchIdx;

	// code --------------------
		// check disk index
		if((E_GrFsNvtDayTblCntOfDisk <= A_DayIdx) || ((__u32)m_DiskCnt <= A_DskIdx))
		{
			*A_PtrRtErrCode	=	E_GrErrBadParam;
			return	NULL;
		}

		// check exist cache
		*A_PtrRtErrCode	=	E_GrErrNone;
		Tv_CchIdx	=	LcCchMinFindInCache(A_DskIdx, A_DayIdx);
		if(0 > Tv_CchIdx)
		{
			// load disk to cache
			Tv_CchIdx	=	LcCchMinLoadToCache(A_DskIdx, A_DayIdx, A_PtrRtErrCode,A_IsReadSkip);
			if(E_GrErrNone != (*A_PtrRtErrCode))
			{
				return	NULL;
			}
		}

		// update access time
		m_CchMin.Items[Tv_CchIdx].AccTime	=	V_GrFsNvtEnv.Tick100ms;
		
		return	(Ptr_GrFsNvtMinuteInfo)m_CchMin.Items[Tv_CchIdx].PtrBuf;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcCchMinFlushByCchIdx(__u32 A_CchIdx)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		__u32	Tv_BlkOfs;
	// code --------------------
		// check current cache index
		if((NULL == m_CchMin.Items) || (0 == m_CchMin.Cnt))
		{
			return	E_GrErrInitFail;
		}

		// check cache index
		if(A_CchIdx >= m_CchMin.Cnt)
		{
			return	E_GrErrBadParam;
		}

		// check exist cache item
		if(0 > m_CchMin.Items[A_CchIdx].DskIdx)
		{
			return	E_GrErrBadParam;
		}

		// check dirty
		if(!m_CchMin.Items[A_CchIdx].IsDirt)
		{
			// success 
			return	E_GrErrNone;
		}

		// check disk status
		if(E_GrDvrRecStatEmpty > m_DiskTbl[m_CchMin.Items[A_CchIdx].DskIdx]->DiskStatGet())
		{
			return	E_GrErrDiskStatBad;
		}

		// save
		Tv_BlkOfs	=	E_GrFsNvtBlkOfsMinuteTbl + (m_CchMin.Items[A_CchIdx].DayIdx * E_GrFsNvtMitDaySize);

		Tv_Result	=	m_DiskTbl[m_CchMin.Items[A_CchIdx].DskIdx]->BlkWritePage(E_GrFsNvtBlkIdxMib, Tv_BlkOfs,
			m_CchMin.Items[A_CchIdx].PtrBuf, E_GrFsNvtMitDaySize);

		if(E_GrErrNone != Tv_Result)
		{
			// update group info
			LcGrpStatUpdate();
		}

		// reset dirt
		m_CchMin.Items[A_CchIdx].IsDirt	=	FALSE;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcMinuteTblChkAndFix(__u32 A_DskIdx, Ptr_GrFsNvtMinuteInfo A_PtrTbl, BOOL8 A_IsFastChk, BOOL8* A_PtrRtIsFixed)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsFix;
		BOOL8	Tv_IsDelMnut;
		__u32	Tv_MnutIdx;
		__u32	Tv_BlkCnt;
		Ptr_GrFsNvtBlkInfoBase	Tv_BiTbl;
		Ptr_GrFsNvtMinuteInfo	Tv_MnutTbl;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_IsFix	=	FALSE;

		Tv_BiTbl	=	m_DiskTbl[A_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
		if((NULL != Tv_BiTbl) && (1 < Tv_BlkCnt))
		{
			Tv_MnutTbl	=	A_PtrTbl;

			for(Tv_MnutIdx = 0; Tv_MnutIdx < E_GrTimeMinOfDay; Tv_MnutIdx++)
			{
				// check exist item
				if((0 != Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx) || ((0 != Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx)))
				{
					Tv_IsDelMnut	=	TRUE;
					// check minute address
					if(0 != Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx)
					{
						// check cureent address
						if(Tv_BlkCnt > Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx)
						{
							if((Tv_BiTbl[Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx].Db.OvwtId == Tv_MnutTbl[Tv_MnutIdx].AdrMin.OvwtId) &&
								(E_GrFsNvtBlkStatBad != Tv_BiTbl[Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx].Db.BlkStat) && 
								(Tv_BiTbl[Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx].Db.InSize > Tv_MnutTbl[Tv_MnutIdx].AdrMin.Ofs) )
							{
								Tv_IsDelMnut	=	FALSE;
								Tv_Result			=	TRUE;
								if(A_IsFastChk)
								{
									break;
								}
							}
							else
							{
								// fix
								Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx			=	0;
								Tv_MnutTbl[Tv_MnutIdx].AdrMin.Ofs			=	0;
								Tv_MnutTbl[Tv_MnutIdx].AdrMin.OvwtId	=	0;
								Tv_IsFix	=	TRUE;
							}
						}
						else
						{
							// fix
							Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx			=	0;
							Tv_MnutTbl[Tv_MnutIdx].AdrMin.Ofs			=	0;
							Tv_MnutTbl[Tv_MnutIdx].AdrMin.OvwtId	=	0;
							Tv_IsFix	=	TRUE;
						}

					}	// if(0 != Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx)

					// check 30s address
					if(0 != Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx)
					{
						// check cureent address
						if(Tv_BlkCnt > Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx)
						{
							if((Tv_BiTbl[Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx].Db.OvwtId == Tv_MnutTbl[Tv_MnutIdx].Adr30s.OvwtId) &&
								(E_GrFsNvtBlkStatBad != Tv_BiTbl[Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx].Db.BlkStat) && 
								(Tv_BiTbl[Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx].Db.InSize > Tv_MnutTbl[Tv_MnutIdx].Adr30s.Ofs))
							{
								Tv_IsDelMnut	=	FALSE;
								Tv_Result			=	TRUE;
								if(A_IsFastChk)
								{
									break;
								}
							}
							else
							{
								// fix
								Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx			=	0;
								Tv_MnutTbl[Tv_MnutIdx].Adr30s.Ofs			=	0;
								Tv_MnutTbl[Tv_MnutIdx].Adr30s.OvwtId	=	0;
								Tv_IsFix	=	TRUE;
							}
						}
						else
						{
							// fix
							Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx			=	0;
							Tv_MnutTbl[Tv_MnutIdx].Adr30s.Ofs			=	0;
							Tv_MnutTbl[Tv_MnutIdx].Adr30s.OvwtId	=	0;
							Tv_IsFix	=	TRUE;
						}
					}	// if(0 != Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx)

					// check minute delete
					if(Tv_IsDelMnut)
					{
						GrDumyZeroMem(&Tv_MnutTbl[Tv_MnutIdx], sizeof(St_GrFsNvtMinuteInfo));
					}
				}	// if(0 != Tv_MnutTbl[Tv_MnutIdx].TimeMin)
			}	// for(Tv_MnutIdx = 0; Tv_MnutIdx < E_GrTimeMinOfDay; Tv_MnutIdx++)
		}

		*A_PtrRtIsFixed	=	Tv_IsFix;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcDskDayCheckByIdx(__u32 A_DskIdx, __u32 A_DayIdx, BOOL8 A_IsFastChk, BOOL8 A_IsBadDelete)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsFix;
		Def_GrErrCode	Tv_ErrCode;
		Ptr_GrFsNvtMinuteInfo	Tv_MnutTbl;
	// code --------------------

		Tv_MnutTbl	=	LcDskMinuteTblGetByIdx(A_DskIdx, A_DayIdx, &Tv_ErrCode,FALSE);
		if(E_GrErrNone != Tv_ErrCode)
		{
			return	FALSE;
		}

		Tv_Result	=	LcMinuteTblChkAndFix(A_DskIdx, Tv_MnutTbl, A_IsFastChk, &Tv_IsFix);
		if(!Tv_Result)
		{
			// delete day
			if(A_IsBadDelete)
			{
				LcDskDayDelByIdx(A_DskIdx, A_DayIdx);
			}
		}
		else if(Tv_IsFix)
		{
			// do write access
			LcCchMinAccessUpdate(A_DskIdx, A_DayIdx, TRUE);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcDskMinuteItemRead(__u32 A_DskIdx, __u32 A_TimeMinute, Ptr_GrFsNvtMinuteInfo A_PtrRtBuf)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		__s32	Tv_DayIdx;
		Ptr_GrFsNvtMinuteInfo	Tv_MnutTbl;
		__u32	Tv_TimeDay;
		__u32	Tv_MnutIdx;

		BOOL8	Tv_IsFix;
		__u32	Tv_BlkCnt;
		Ptr_GrFsNvtBlkInfoBase	Tv_BiTbl;

	// code --------------------
		// calculate time day
		Tv_TimeDay	=	A_TimeMinute - (A_TimeMinute % E_GrTimeSecOfDay);
		
		Tv_DayIdx			=	LcDskDayFind(A_DskIdx, Tv_TimeDay);
		if(0 > Tv_DayIdx)
		{
			return	E_GrErrNotFound;
		}

		Tv_MnutTbl		=	LcDskMinuteTblGetByIdx(A_DskIdx, (__u32)Tv_DayIdx, &Tv_Result,FALSE);
		if(E_GrErrNone != Tv_Result)
		{
			return	Tv_Result;
		}

		// check value
		// patch
		Tv_BiTbl	=	m_DiskTbl[A_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
		if((NULL == Tv_BiTbl) || (1 >= Tv_BlkCnt))
		{
			return	E_GrErrNotFound;
		}

		// calculate minute index
		Tv_MnutIdx	=	(A_TimeMinute - Tv_TimeDay) / E_GrTimeSecOfMin;

		Tv_IsFix	=	FALSE;
		// check minute address
		if(0 != Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx)
		{
			// check cureent address
			if(Tv_BlkCnt > Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx)
			{
				if((Tv_BiTbl[Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx].Db.OvwtId != Tv_MnutTbl[Tv_MnutIdx].AdrMin.OvwtId) ||
					(E_GrFsNvtBlkStatBad == Tv_BiTbl[Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx].Db.BlkStat))
				{
					// fix
					Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx		=	0;
					Tv_MnutTbl[Tv_MnutIdx].AdrMin.Ofs		=	0;
					Tv_MnutTbl[Tv_MnutIdx].AdrMin.OvwtId	=	0;
					Tv_IsFix		=	TRUE;
				}
			}
			else
			{
				// fix
				Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx		=	0;
				Tv_MnutTbl[Tv_MnutIdx].AdrMin.Ofs		=	0;
				Tv_MnutTbl[Tv_MnutIdx].AdrMin.OvwtId	=	0;
				Tv_IsFix		=	TRUE;
			}

		}	// if(0 != A_PtrRtBuf->AdrMin.Idx)

		// check 30s address
		if(0 != Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx)
		{
			// check cureent address
			if(Tv_BlkCnt > Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx)
			{
				if((Tv_BiTbl[Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx].Db.OvwtId != Tv_MnutTbl[Tv_MnutIdx].Adr30s.OvwtId) ||
					(E_GrFsNvtBlkStatBad == Tv_BiTbl[Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx].Db.BlkStat))
				{
					// fix
					Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx		=	0;
					Tv_MnutTbl[Tv_MnutIdx].Adr30s.Ofs		=	0;
					Tv_MnutTbl[Tv_MnutIdx].Adr30s.OvwtId	=	0;
					Tv_IsFix		=	TRUE;
				}
			}
			else
			{
				// fix
				Tv_MnutTbl[Tv_MnutIdx].Adr30s.Idx		=	0;
				Tv_MnutTbl[Tv_MnutIdx].Adr30s.Ofs		=	0;
				Tv_MnutTbl[Tv_MnutIdx].Adr30s.OvwtId	=	0;
				Tv_IsFix		=	TRUE;
			}
		}	// if(0 != Tv_MnutTbl[Tv_MnutIdx].AdrMin.Idx)

		// update access
		LcCchMinAccessUpdate(A_DskIdx, (__u32)Tv_DayIdx, Tv_IsFix);

		// copy data
		GrDumyCopyMem(A_PtrRtBuf, &Tv_MnutTbl[Tv_MnutIdx], E_GrFsNvtMinuteItemSize);

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcDskMinuteItemWrite(__u32 A_DskIdx, __u32 A_TimeMinute, Ptr_GrFsNvtMinuteInfo A_PtrBuf)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		__s32	Tv_DayIdx;
		__u32	Tv_MnutIdx;
		__u32	Tv_TimeDay;
		Ptr_GrFsNvtMinuteInfo	Tv_MnutTbl;
	// code --------------------

		// calculate time day
		Tv_TimeDay	=	A_TimeMinute - (A_TimeMinute % E_GrTimeSecOfDay);

		Tv_DayIdx			=	LcDskDayFind(A_DskIdx, Tv_TimeDay);
		if(0 > Tv_DayIdx)
		{
			Tv_DayIdx	=	LcDskDayCreate(A_DskIdx, Tv_TimeDay);
			// check day create faile
			if(0 > Tv_DayIdx)
			{
				return	E_GrErrNotFound;
			}
		}

		Tv_MnutTbl		=	LcDskMinuteTblGetByIdx(A_DskIdx, (__u32)Tv_DayIdx, &Tv_Result,FALSE);
		if(E_GrErrNone != Tv_Result)
		{
			return	Tv_Result;
		}

		// calculate minute index
		Tv_MnutIdx	=	(A_TimeMinute - Tv_TimeDay) / E_GrTimeSecOfMin;
		GrDumyCopyMem(&Tv_MnutTbl[Tv_MnutIdx], A_PtrBuf, E_GrFsNvtMinuteItemSize);

		LcCchMinAccessUpdate(A_DskIdx, (__u32)Tv_DayIdx, TRUE);
		
		return	E_GrErrNone;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpRecMinuteOpen(__u32 A_TimeMinute)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check parametter
		if(0 <= m_RecDskIdx)
		{
			// load minute
			if(E_GrErrNone != LcDskMinuteItemRead((__u32)m_RecDskIdx, A_TimeMinute, &m_RecBlkEnv.MinuteInfo))
			{
				// clear
				GrDumyZeroMem(&m_RecBlkEnv.MinuteInfo, sizeof(E_GrFsNvtMinuteItemSize));
			}

			// check cear
			if((0 == m_RecBlkEnv.MinuteInfo.AdrMin.Idx) && (0 == m_RecBlkEnv.MinuteInfo.Adr30s.Idx))
			{
				// reset record atb and predict size
				GrDumyZeroMem(&m_RecBlkEnv.MinuteInfo, sizeof(St_GrFsNvtMinuteInfo));
			}

			// update
			m_RecBlkEnv.TimeMinute	=	A_TimeMinute;
			m_RecTimeLast						=	A_TimeMinute;
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpRecMinuteClose(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check parametter
		if((0 <= m_RecDskIdx) && (0 != m_RecBlkEnv.TimeMinute))
		{
			Tv_Result	=	TRUE;
			if(m_RecBlkEnv.IsMinuteChg)
			{
				Tv_Result	=	FALSE;
				// save minute
				if(E_GrErrNone == LcDskMinuteItemWrite((__u32)m_RecDskIdx, m_RecBlkEnv.TimeMinute, &m_RecBlkEnv.MinuteInfo))
				{
					Tv_Result	=	TRUE;
				}
			}
		}
		// update
		m_RecBlkEnv.TimeMinute	=	0;
		m_RecBlkEnv.IsMinuteChg	=	FALSE;

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpRecMinuteFlush(void)
{
	// local -------------------
	// code --------------------
		if ((0 > m_RecDskIdx) || (0 == m_RecBlkEnv.TimeMinute))
		{
			return	E_GrErrNone;
		}

		if (!m_RecBlkEnv.IsMinuteChg)
		{
			return	E_GrErrNone;
		}

		m_RecBlkEnv.IsMinuteChg	=	FALSE;

		// save minute
		return	LcDskMinuteItemWrite((__u32)m_RecDskIdx, m_RecBlkEnv.TimeMinute, &m_RecBlkEnv.MinuteInfo);
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcDskDayDelByIdx(__u32 A_DskIdx, __u32 A_DayIdx)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
		
		Ptr_GrFsNvtDayInfo	Tv_DayTbl;
		//Ptr_GrFsNvtMinuteInfo	Tv_PtrMinTbl;
	// code --------------------
		if((E_GrFsNvtDayTblCntOfDisk <= A_DayIdx) && ((__u32)m_DiskCnt <= A_DskIdx))
		{
			return	E_GrErrBadParam;
		}

		// check disk info
		if(E_GrDvrRecStatEmpty > m_DiskTbl[A_DskIdx]->DiskStatGet())
		{
			return	E_GrErrNotFound;
		}

		Tv_DayTbl	=	m_DiskTbl[A_DskIdx]->DayInfoTableGet();
		if(NULL == Tv_DayTbl)
		{
			return	E_GrErrDiskStatBad;
		}

#if 0
		Tv_PtrMinTbl	=	LcDskMinuteTblGetByIdx(A_DskIdx, A_DayIdx, &Tv_ErrCode);
		if(E_GrErrNone != Tv_ErrCode)
		{
			return	Tv_ErrCode;
		}

		// clear data
		GrDumyZeroMem(Tv_PtrMinTbl, E_GrFsNvtMitDaySize);
		LcCchMinAccessUpdate(A_DskIdx, A_DayIdx, TRUE);
#endif

		// day delete
		Tv_DayTbl[A_DayIdx].TimeDay	=	0;
		// do write day map
		Tv_ErrCode	=	m_DiskTbl[A_DskIdx]->DayInfoWrite();
		if(E_GrErrNone != Tv_ErrCode)
		{
			LcGrpStatUpdate();
		}

		return	Tv_ErrCode;

}
//--------------------------------------------------------------------
__s32	Cls_GrFsNvtRgrp::LcDskDayCreate(__u32 A_DskIdx, __u32 A_TimeDay)
{
	// local -------------------
		__s32	Tv_Result;		// day index
		Def_GrErrCode	Tv_ErrCode;
		__u32	Tv_DayIdx;
		Ptr_GrFsNvtDayInfo	Tv_DayTbl;
		Ptr_GrFsNvtMinuteInfo	Tv_PtrMinTbl;
	// code --------------------

		if(E_GrDvrRecStatEmpty > m_DiskTbl[A_DskIdx]->DiskStatGet())
		{
			return	-1;
		}

		Tv_Result	=	-1;

		// get date table
		Tv_DayTbl	=	m_DiskTbl[A_DskIdx]->DayInfoTableGet();
		// find empty date slot
		for(Tv_DayIdx = 0; Tv_DayIdx < E_GrFsNvtDayTblCntOfDisk; Tv_DayIdx++)
		{
			if(0 == Tv_DayTbl[Tv_DayIdx].TimeDay )
			{
				// found
				Tv_Result	=	(__s32)Tv_DayIdx;
				break;
			}
		}

		if(0 > Tv_Result)
		{
			// check daylist
			for(Tv_DayIdx = 0; Tv_DayIdx < E_GrFsNvtDayTblCntOfDisk; Tv_DayIdx++)
			{
				if(!LcDskDayCheckByIdx(A_DskIdx, Tv_DayIdx, TRUE, FALSE))
				{
					// check disk status
					if(E_GrDvrRecStatEmpty > m_DiskTbl[A_DskIdx]->DiskStatGet())
					{
						// disk error
						return	-1;
					}
					// delete
					if(E_GrErrNone == LcDskDayDelByIdx(A_DskIdx, Tv_DayIdx))
					{
						Tv_Result	=	(__s32)Tv_DayIdx;
					}
					break;
				}
			}
		}

		// create
		if(0 <= Tv_Result)
		{
			// get minute info
			Tv_PtrMinTbl	=	LcDskMinuteTblGetByIdx(A_DskIdx, Tv_Result, &Tv_ErrCode,TRUE);
			if (E_GrErrNone == Tv_ErrCode)
			{
				// clear data
				GrDumyZeroMem(Tv_PtrMinTbl, E_GrFsNvtMitDaySize);
				LcCchMinAccessUpdate(A_DskIdx, Tv_Result, TRUE);
			}
			else
			{
				DbgMsgPrint("Cls_GrFsNvtRgrp::LcDskDayCreate - disk %d day index %d data get fail!\n", A_DskIdx,Tv_Result);
				return	Tv_Result;
			}

			Tv_DayTbl[Tv_Result].TimeDay	=	A_TimeDay;
			if(E_GrErrNone != m_DiskTbl[A_DskIdx]->DayInfoWrite())
			{
				Tv_Result	=	-1;
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrFsNvtRgrp::LcDskDayFind(__u32 A_DskIdx, __u32 A_TimeDay)
{
	// local -------------------
		__s32	Tv_Result;
		__u32	Tv_DayIdx;
		Ptr_GrFsNvtDayInfo	Tv_DayTbl;
	// code --------------------
		Tv_Result	=	-1;
		if(E_GrDvrRecStatEmpty <= m_DiskTbl[A_DskIdx]->DiskStatGet())
		{
			Tv_DayTbl	=	m_DiskTbl[A_DskIdx]->DayInfoTableGet();
			for(Tv_DayIdx = 0; Tv_DayIdx < E_GrFsNvtDayTblCntOfDisk; Tv_DayIdx++)
			{
				if(A_TimeDay == Tv_DayTbl[Tv_DayIdx].TimeDay)
				{
					// found
					Tv_Result	=	(__s32)Tv_DayIdx;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcDskDayFindByTimeFw(__u32 A_DskIdx, __u32 A_Time, __s32* A_PtrRtDayIdx, __u32* A_PtrRtTime)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_FndDay;
		__u32	Tv_FndTime;
		__u32	Tv_DayIdx;
		__u32	Tv_TimeDay;
		Ptr_GrFsNvtDayInfo	Tv_DayTbl;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_FndDay	=	-1;
		Tv_FndTime	=	0;

		Tv_TimeDay	=	A_Time - (A_Time % E_GrTimeSecOfDay);

		if((A_DskIdx < m_DiskCnt) && (0 != Tv_TimeDay))
		{
			if(E_GrDvrRecStatEmpty <= m_DiskTbl[A_DskIdx]->DiskStatGet())
			{
				Tv_DayTbl	=	m_DiskTbl[A_DskIdx]->DayInfoTableGet();
				for(Tv_DayIdx = 0; Tv_DayIdx < E_GrFsNvtDayTblCntOfDisk; Tv_DayIdx++)
				{
					if(Tv_TimeDay == Tv_DayTbl[Tv_DayIdx].TimeDay)
					{
						Tv_FndDay		=	(__s32)Tv_DayIdx;
						Tv_FndTime	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
						Tv_Result	=	TRUE;
						break;
					}
					else if(Tv_TimeDay < Tv_DayTbl[Tv_DayIdx].TimeDay)
					{
						if(0 > Tv_FndDay)
						{
							Tv_FndDay		=	(__s32)Tv_DayIdx;
							Tv_FndTime	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
							Tv_Result	=	TRUE;
						}
						else
						{
							if((Tv_FndTime > Tv_DayTbl[Tv_DayIdx].TimeDay))
							{
								Tv_FndDay		=	(__s32)Tv_DayIdx;
								Tv_FndTime	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
							}
						}
					}
				}	//	for( Tv_DayIdx = 0; Tv_DayIdx < Tv_DayCnt; Tv_DayIdx++)
			}
		}

		*A_PtrRtDayIdx	=	Tv_FndDay;
		*A_PtrRtTime		=	Tv_FndTime;
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpDayFindByTimeFw(__u32 A_Time, __s32* A_PtrRtDskIdx, __s32* A_PtrRtDayIdx, __u32* A_PtrRtTime)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_FndDsk;
		__s32	Tv_FndDay;
		__u32	Tv_FndTime;
		__u32	Tv_DskIdx;
		__u32	Tv_DayIdx;
		__u32	Tv_TimeDay;
		Ptr_GrFsNvtDayInfo	Tv_DayTbl;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_FndDsk	=	-1;
		Tv_FndDay	=	-1;
		Tv_FndTime	=	0;

		Tv_TimeDay	=	A_Time - (A_Time % E_GrTimeSecOfDay);

		if((0 != m_DiskCnt) && (0 != Tv_TimeDay))
		{
			for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				if(E_GrDvrRecStatEmpty <= m_DiskTbl[Tv_DskIdx]->DiskStatGet())
				{
					Tv_DayTbl	=	m_DiskTbl[Tv_DskIdx]->DayInfoTableGet();
					for(Tv_DayIdx = 0; Tv_DayIdx < E_GrFsNvtDayTblCntOfDisk; Tv_DayIdx++)
					{
						if(Tv_TimeDay == Tv_DayTbl[Tv_DayIdx].TimeDay)
						{
							Tv_FndDay		=	(__s32)Tv_DayIdx;
							Tv_FndDsk		=	(__s32)Tv_DskIdx;
							Tv_FndTime	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
							Tv_Result	=	TRUE;
							break;
						}
						else if(Tv_TimeDay < Tv_DayTbl[Tv_DayIdx].TimeDay)
						{
							if(0 > Tv_FndDsk)
							{
								Tv_FndDay		=	(__s32)Tv_DayIdx;
								Tv_FndDsk		=	(__s32)Tv_DskIdx;
								Tv_FndTime	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
								Tv_Result	=	TRUE;
							}
							else
							{
								if((Tv_FndTime > Tv_DayTbl[Tv_DayIdx].TimeDay))
								{
									Tv_FndDay		=	(__s32)Tv_DayIdx;
									Tv_FndDsk		=	(__s32)Tv_DskIdx;
									Tv_FndTime	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
								}
							}
						}
					}	//	for( Tv_DayIdx = 0; Tv_DayIdx < Tv_DayCnt; Tv_DayIdx++)
				}
				// check found
				if(Tv_FndTime == Tv_TimeDay)
				{
					break;
				}
			}	//	for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		}

		*A_PtrRtDskIdx	=	Tv_FndDsk;
		*A_PtrRtDayIdx	=	Tv_FndDay;
		*A_PtrRtTime		=	Tv_FndTime;
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcDskDayFindByTimeBw(__u32 A_DskIdx, __u32 A_Time, __s32* A_PtrRtDayIdx, __u32* A_PtrRtTime)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_FndDay;
		__u32	Tv_FndTime;
		__u32	Tv_DayIdx;
		__u32	Tv_TimeDay;
		Ptr_GrFsNvtDayInfo	Tv_DayTbl;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_FndDay		=	-1;
		Tv_FndTime	=	0;

		Tv_TimeDay	=	A_Time - (A_Time % E_GrTimeSecOfDay);

		if((A_DskIdx < m_DiskCnt) && (0 != Tv_TimeDay))
		{
			if(E_GrDvrRecStatEmpty <= m_DiskTbl[A_DskIdx]->DiskStatGet())
			{
				Tv_DayTbl	=	m_DiskTbl[A_DskIdx]->DayInfoTableGet();
				for(Tv_DayIdx = 0; Tv_DayIdx < E_GrFsNvtDayTblCntOfDisk; Tv_DayIdx++)
				{
					if(Tv_TimeDay == Tv_DayTbl[Tv_DayIdx].TimeDay)
					{
						Tv_FndDay		=	(__s32)Tv_DayIdx;
						Tv_FndTime	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
						Tv_Result		=	TRUE;
						break;
					}
					else if((0 != Tv_DayTbl[Tv_DayIdx].TimeDay) && (Tv_TimeDay > Tv_DayTbl[Tv_DayIdx].TimeDay))
					{
						if(0 > Tv_FndDay)
						{
							Tv_FndDay		=	(__s32)Tv_DayIdx;
							Tv_FndTime	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
							Tv_Result		=	TRUE;
						}
						else
						{
							if((Tv_FndTime < Tv_DayTbl[Tv_DayIdx].TimeDay))
							{
								Tv_FndDay		=	(__s32)Tv_DayIdx;
								Tv_FndTime	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
							}
						}
					}
				}	//	for( Tv_DayIdx = 0; Tv_DayIdx < Tv_DayCnt; Tv_DayIdx++)
			}
		}

		*A_PtrRtDayIdx	=	Tv_FndDay;
		*A_PtrRtTime		=	Tv_FndTime;
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpDayFindByTimeBw(__u32 A_Time, __s32* A_PtrRtDskIdx, __s32* A_PtrRtDayIdx, __u32* A_PtrRtTime)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_FndDsk;
		__s32	Tv_FndDay;
		__u32	Tv_FndTime;
		__u32	Tv_DskIdx;
		__u32	Tv_DayIdx;
		__u32	Tv_TimeDay;
		Ptr_GrFsNvtDayInfo	Tv_DayTbl;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_FndDsk		=	-1;
		Tv_FndDay		=	-1;
		Tv_FndTime	=	0;

		Tv_TimeDay	=	A_Time - (A_Time % E_GrTimeSecOfDay);

		if((0 != m_DiskCnt) && (0 != Tv_TimeDay))
		{
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				if(E_GrDvrRecStatEmpty <= m_DiskTbl[Tv_DskIdx]->DiskStatGet())
				{
					Tv_DayTbl	=	m_DiskTbl[Tv_DskIdx]->DayInfoTableGet();
					for(Tv_DayIdx = 0; Tv_DayIdx < E_GrFsNvtDayTblCntOfDisk; Tv_DayIdx++)
					{
						if(Tv_TimeDay == Tv_DayTbl[Tv_DayIdx].TimeDay)
						{
							Tv_FndDay		=	(__s32)Tv_DayIdx;
							Tv_FndDsk		=	(__s32)Tv_DskIdx;
							Tv_FndTime	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
							Tv_Result		=	TRUE;
							break;
						}
						else if((0 != Tv_DayTbl[Tv_DayIdx].TimeDay) && (Tv_TimeDay > Tv_DayTbl[Tv_DayIdx].TimeDay))
						{
							if(0 > Tv_FndDsk)
							{
								Tv_FndDay		=	(__s32)Tv_DayIdx;
								Tv_FndDsk		=	(__s32)Tv_DskIdx;
								Tv_FndTime	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
								Tv_Result		=	TRUE;
							}
							else
							{
								if((Tv_FndTime < Tv_DayTbl[Tv_DayIdx].TimeDay))
								{
									Tv_FndDay		=	(__s32)Tv_DayIdx;
									Tv_FndDsk		=	(__s32)Tv_DskIdx;
									Tv_FndTime	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
								}
							}
						}
					}	//	for( Tv_DayIdx = 0; Tv_DayIdx < Tv_DayCnt; Tv_DayIdx++)
				}
				// check found
				if(Tv_FndTime == Tv_TimeDay)
				{
					break;
				}
			}	//	for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		}

		*A_PtrRtDskIdx	=	Tv_FndDsk;
		*A_PtrRtDayIdx	=	Tv_FndDay;
		*A_PtrRtTime		=	Tv_FndTime;
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpBlkAdrFindByTimeFw(__u32 A_Time, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtSeqId, __u8* A_PtrRtOvwtId, __s32* A_PtrRtOfs)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsFix;
		BOOL8	Tv_Is30s;
		__u32	Tv_DskIdx;
		__s32	Tv_DayIdx;
		__u32	Tv_MinIdx;
		__u32	Tv_TimeDay;
		__u32	Tv_BlkCnt;
		__s32	Tv_FndDsk;
		__s32	Tv_FndBlk;
		__u32	Tv_FndSeqId;
		__u8	Tv_FndOvwtId;
		__s32	Tv_FndOfs;
		__u32	Tv_FndDay;
		__u32	Tv_FndTime;

		Def_GrErrCode	Tv_ErrCode;

		// rtl
		__u32	Tv_DayLoaded;
		__u32	Tv_TimeReq;			// required base time
		__u32	Tv_TimeDskWk;		// time on disk work

		Ptr_GrFsNvtMinuteInfo	Tv_MnutTbl;
		Ptr_GrFsNvtBlkInfoBase	Tv_BiTbl;
	// code --------------------
		// init
		Tv_Result			=	FALSE;
		Tv_FndDsk			=	-1;
		Tv_FndBlk			=	-1;
		Tv_FndSeqId		=	0;
		Tv_FndOvwtId	=	0;
		Tv_FndOfs			=	-1;
		Tv_FndDay			=	0;
		Tv_FndTime		=	0;

		Tv_TimeReq		=	A_Time;

		do
		{
			// parpare
			Tv_DayLoaded	=	0;
			// check disk
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				// check day find
				if(!LcDskDayFindByTimeFw(Tv_DskIdx, Tv_TimeReq, &Tv_DayIdx, &Tv_TimeDay))
				{
					continue;
				}

				// check block exist
				Tv_BiTbl	=	m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
				if((NULL == Tv_BiTbl) || (0 == Tv_BlkCnt))
				{
					continue;
				}

				// update day loaded
				if(0 == Tv_DayLoaded)
				{
					Tv_DayLoaded	=	Tv_TimeDay;
				}
				else if(Tv_DayLoaded > Tv_TimeDay )
				{
					Tv_DayLoaded	=	Tv_TimeDay;
				}

				// compare previous found day
				if(Tv_Result)
				{
					if(Tv_FndDay < Tv_TimeDay)
					{
						continue;
					}
				}

				// get minute table
				Tv_MnutTbl	=	LcDskMinuteTblGetByIdx((__u32)Tv_DskIdx, (__u32)Tv_DayIdx, &Tv_ErrCode,FALSE);
				if(E_GrErrNone != Tv_ErrCode)
				{
					continue;
				}

				// check table
				if(!LcMinuteTblChkAndFix((__u32)Tv_DskIdx, Tv_MnutTbl, FALSE, &Tv_IsFix))
				{
					if(Tv_IsFix)
					{
						LcCchMinAccessUpdate((__u32)Tv_DskIdx, (__u32)Tv_DayIdx, TRUE);
					}
					continue;
				}

				// calculate start minute index
				Tv_TimeDskWk	=	Tv_TimeReq;
				if(Tv_TimeDskWk < Tv_TimeDay)
				{
					Tv_TimeDskWk	=	Tv_TimeDay;
				}
				// get time block
				Tv_Is30s	=	FALSE;
				Tv_MinIdx	=	(Tv_TimeDskWk - Tv_TimeDay) / 30;
				if(0 != (1 & Tv_MinIdx))
				{
					Tv_Is30s	=	TRUE;
				}

				Tv_MinIdx	=	Tv_MinIdx >> 1;

				Tv_TimeDskWk	=	Tv_TimeDay + (Tv_MinIdx * 60);	// recalculate time disk work

				while(Tv_MinIdx < E_GrTimeMinOfDay)
				{
					// check already found
					if(Tv_Result)
					{
						if(Tv_FndTime < Tv_TimeDskWk)
						{
							break;
						}
					}

					// check exist time
					if(0 != Tv_MnutTbl[Tv_MinIdx].AdrMin.Idx)
					{
						// found
						Tv_FndDsk			=	(__s32)Tv_DskIdx;
						Tv_FndBlk			=	(__s32)((__u32)Tv_MnutTbl[Tv_MinIdx].AdrMin.Idx);
						Tv_FndOvwtId	=	Tv_MnutTbl[Tv_MinIdx].AdrMin.OvwtId;
						Tv_FndOfs			=	(__s32)Tv_MnutTbl[Tv_MinIdx].AdrMin.Ofs;
						Tv_FndSeqId		=	Tv_BiTbl[Tv_FndBlk].Db.SeqId;
						Tv_FndDay			=	Tv_TimeDay;
						Tv_FndTime		=	Tv_TimeDskWk;
						Tv_Result	=	TRUE;
					}

					if((!Tv_Result) || (Tv_Is30s))
					{
						if(0 != Tv_MnutTbl[Tv_MinIdx].Adr30s.Idx)
						{
							// found
							Tv_FndDsk				=	(__s32)Tv_DskIdx;
							Tv_FndBlk				=	(__s32)((__u32)Tv_MnutTbl[Tv_MinIdx].Adr30s.Idx);
							Tv_FndOvwtId		=	Tv_MnutTbl[Tv_MinIdx].Adr30s.OvwtId;
							Tv_FndOfs				=	(__s32)Tv_MnutTbl[Tv_MinIdx].Adr30s.Ofs;
							Tv_FndSeqId			=	Tv_BiTbl[Tv_FndBlk].Db.SeqId;
							Tv_FndDay				=	Tv_TimeDay;
							Tv_FndTime			=	Tv_TimeDskWk + 30;
							Tv_Result	=	TRUE;
						}
					}

					if(Tv_Result)
					{
						break;
					}
					// next
					Tv_TimeDskWk	=	Tv_TimeDskWk + 60;
					Tv_Is30s	=	FALSE;
					Tv_MinIdx ++;
				}	//	while(Tv_MinIdx < E_GrTimeMinOfDay)

				// update access
				LcCchMinAccessUpdate((__u32)Tv_DskIdx, (__u32)Tv_DayIdx, Tv_IsFix);

			}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)

			// check found
			if(Tv_Result)
			{
				break;
			}

			// next day
			Tv_TimeReq	=	Tv_DayLoaded + E_GrTimeSecOfDay;

		} while(0 != Tv_DayLoaded);	// check day loaded

		*A_PtrRtDskIdx	=	Tv_FndDsk;
		*A_PtrRtBlkIdx	=	Tv_FndBlk;
		*A_PtrRtSeqId		=	Tv_FndSeqId;
		*A_PtrRtOvwtId	=	Tv_FndOvwtId;
		*A_PtrRtOfs			=	Tv_FndOfs;

		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpBlkAdrFindByTimeBw(__u32 A_Time, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtSeqId, __u8* A_PtrRtOvwtId, __s32* A_PtrRtOfs)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsFix;
		BOOL8	Tv_Is30s;
		__u32	Tv_DskIdx;
		__s32	Tv_DayIdx;
		__s32	Tv_MinIdx;
		__u32	Tv_TimeDay;
		__s32	Tv_FndDsk;
		__s32	Tv_FndBlk;
		__u32	Tv_FndSeqId;
		__u8	Tv_FndOvwtId;
		__s32	Tv_FndOfs;
		__u32	Tv_BlkCnt;
		__u32	Tv_FndDay;
		__u32	Tv_FndTime;
		Def_GrErrCode	Tv_ErrCode;

		// rtl
		__u32	Tv_DayLoaded;
		__u32	Tv_TimeReq;			// required base time
		__u32	Tv_TimeDskWk;		// time on disk work

		Ptr_GrFsNvtMinuteInfo	Tv_MnutTbl;
		Ptr_GrFsNvtBlkInfoBase	Tv_BiTbl;
	// code --------------------

		// init
		Tv_Result			=	FALSE;
		Tv_FndDsk			=	-1;
		Tv_FndBlk			=	-1;
		Tv_FndSeqId		=	0;
		Tv_FndOvwtId	=	0;
		Tv_FndOfs			=	-1;
		Tv_FndDay			=	0;
		Tv_FndTime		=	0;

		Tv_TimeReq		=	A_Time - 29;	// patch time

		do
		{
			// parpare
			Tv_DayLoaded	=	0;
			// check disk
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				// check day find
				if(!LcDskDayFindByTimeBw(Tv_DskIdx, Tv_TimeReq, &Tv_DayIdx, &Tv_TimeDay))
				{
					continue;
				}

				// check block exist
				Tv_BiTbl	=	m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
				if((NULL == Tv_BiTbl) || (0 == Tv_BlkCnt))
				{
					continue;
				}

				// update day loaded
				if(0 == Tv_DayLoaded)
				{
					Tv_DayLoaded	=	Tv_TimeDay;
				}
				else if(Tv_DayLoaded < Tv_TimeDay)
				{
					Tv_DayLoaded	=	Tv_TimeDay;
				}

				// compare previous found day
				if(Tv_Result)
				{
					if(Tv_FndDay > Tv_TimeDay)
					{
						continue;
					}
				}

				// load minute table
				Tv_MnutTbl	=	LcDskMinuteTblGetByIdx((__u32)Tv_DskIdx, (__u32)Tv_DayIdx, &Tv_ErrCode,FALSE);
				if(E_GrErrNone != Tv_ErrCode)
				{
					continue;
				}

				// check table
				if(!LcMinuteTblChkAndFix((__u32)Tv_DskIdx, Tv_MnutTbl, FALSE, &Tv_IsFix))
				{
					if(Tv_IsFix)
					{
						LcCchMinAccessUpdate((__u32)Tv_DskIdx, (__u32)Tv_DayIdx, TRUE);
					}
					continue;
				}

				// calculate start minute index
				Tv_TimeDskWk	=	Tv_TimeReq;
				if(E_GrTimeSecOfDay <= (Tv_TimeDskWk - Tv_TimeDay))
				{
					Tv_TimeDskWk	=	Tv_TimeDay + E_GrTimeSecOfDay - 1;
				}
				// get time block
				Tv_Is30s	=	FALSE;
				Tv_MinIdx	=	(Tv_TimeDskWk - Tv_TimeDay) / 30;
				if(0 != (1 & Tv_MinIdx))
				{
					Tv_Is30s	=	TRUE;
				}

				Tv_MinIdx	=	Tv_MinIdx >> 1;

				Tv_TimeDskWk	=	Tv_TimeDay + (Tv_MinIdx * 60);	// recalculate time disk work

				while(0 <= Tv_MinIdx)
				{
					// check already found
					if(Tv_Result)
					{
						if(Tv_FndTime > Tv_TimeDskWk)
						{
							break;
						}
					}

					if(Tv_Is30s)
					{
						if(0 != Tv_MnutTbl[Tv_MinIdx].Adr30s.Idx)
						{
							// found
							Tv_FndDsk				=	(__s32)Tv_DskIdx;
							Tv_FndBlk				=	(__s32)((__u32)Tv_MnutTbl[Tv_MinIdx].Adr30s.Idx);
							Tv_FndOvwtId		=	Tv_MnutTbl[Tv_MinIdx].Adr30s.OvwtId;
							Tv_FndOfs				=	(__s32)Tv_MnutTbl[Tv_MinIdx].Adr30s.Ofs;
							Tv_FndSeqId			=	Tv_BiTbl[Tv_FndBlk].Db.SeqId;
							Tv_FndDay				=	Tv_TimeDay;
							Tv_FndTime			=	Tv_TimeDskWk + 30;
							Tv_Result	=	TRUE;
						}
					}

					// check exist time
					if(!Tv_Result)
					{
						if(0 != Tv_MnutTbl[Tv_MinIdx].AdrMin.Idx)
						{
							// found
							Tv_FndDsk			=	(__s32)Tv_DskIdx;
							Tv_FndBlk			=	(__s32)((__u32)Tv_MnutTbl[Tv_MinIdx].AdrMin.Idx);
							Tv_FndOvwtId	=	Tv_MnutTbl[Tv_MinIdx].AdrMin.OvwtId;
							Tv_FndOfs			=	(__s32)Tv_MnutTbl[Tv_MinIdx].AdrMin.Ofs;
							Tv_FndSeqId			=	Tv_BiTbl[Tv_FndBlk].Db.SeqId;
							Tv_FndDay			=	Tv_TimeDay;
							Tv_FndTime		=	Tv_TimeDskWk;
							Tv_Result	=	TRUE;
						}
					}

					if(Tv_Result)
					{
						break;
					}
					// next
					Tv_TimeDskWk	=	Tv_TimeDskWk - 60;
					Tv_Is30s	=	TRUE;
					Tv_MinIdx --;
				}	//	while(0 <= Tv_MinIdx)

				// check fxed
				LcCchMinAccessUpdate((__u32)Tv_DskIdx, (__u32)Tv_DayIdx, Tv_IsFix);

			}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)

			// check found
			if(Tv_Result)
			{
				break;
			}

			// next day
			Tv_TimeReq	=	Tv_DayLoaded - E_GrTimeSecOfDay;

		} while((0 != Tv_DayLoaded) && (E_GrTimeSecOfDay <= Tv_TimeReq));	// check day loaded

		// update
		*A_PtrRtDskIdx	=	Tv_FndDsk;
		*A_PtrRtBlkIdx	=	Tv_FndBlk;
		*A_PtrRtSeqId		=	Tv_FndSeqId;
		*A_PtrRtOvwtId	=	Tv_FndOvwtId;
		*A_PtrRtOfs			=	Tv_FndOfs;

		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpBlkFindByTimeRngBw(__u32 A_TimeSt, __u32 A_TimeEd, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
		__s32	Tv_FndDskIdx;
		__s32	Tv_FndBlkIdx;
		__u32	Tv_FndBlkSeq;
		__u8	Tv_FndOvwtId;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_FndDskIdx	=	-1;
		Tv_FndBlkIdx	=	-1;
		Tv_FndBlkSeq	=	0;
		Tv_FndOvwtId	=	0;

		if(0 != m_DiskCnt)
		{
			// check time range
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				// get disk block head table
				Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrBi)
				{
					// check block
					for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					{
						// check block type
						if(E_GrFsNvtBlkStatBad != Tv_PtrBi[Tv_BlkIdx].Db.BlkStat)
						{
							// check time range
							if((0 != Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeSt) && (0 != Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeEd))
							{
								// check time range
								if((A_TimeSt <= Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeEd) && (A_TimeEd >= Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeSt))
								{
									// check already exist
									if(0 > Tv_FndDskIdx)
									{
										Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
										Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
										Tv_FndBlkSeq	=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
										Tv_FndOvwtId	=	Tv_PtrBi[Tv_BlkIdx].Db.OvwtId;
										Tv_Result	=	TRUE;
									}
									else
									{
										if(Tv_FndBlkSeq < Tv_PtrBi[Tv_BlkIdx].Db.SeqId)
										{
											Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
											Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
											Tv_FndBlkSeq	=	Tv_PtrBi[Tv_BlkIdx].Db.SeqId;
											Tv_FndOvwtId	=	Tv_PtrBi[Tv_BlkIdx].Db.OvwtId;
										}
									}
								}
							}
						}
					}	//	for( Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
				}
			}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		}

		// update
		*A_PtrRtDskIdx	=	Tv_FndDskIdx;
		*A_PtrRtBlkIdx	=	Tv_FndBlkIdx;
		*A_PtrRtBlkSeq	=	Tv_FndBlkSeq;
		*A_PtrRtOvwtId	=	Tv_FndOvwtId;

		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcBlkChkSeqOverlapped(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_SeqId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_DskIdx;
		Ptr_GrFsNvtBlkInfoBase	Tv_BiTbl;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_DskIdx	=	A_DskIdx;
		Tv_BlkIdx	=	A_BlkIdx + 1;

		while(Tv_DskIdx < m_DiskCnt)
		{
			Tv_BiTbl	=	m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
			if((1 < Tv_BlkCnt) && (NULL != Tv_BiTbl))
			{
				while(Tv_BlkIdx < Tv_BlkCnt)
				{
					// check exist block
					if((0 != Tv_BiTbl[Tv_BlkIdx].Db.SeqId) && (E_GrFsNvtBlkStatBad != Tv_BiTbl[Tv_BlkIdx].Db.BlkStat))
					{
						// check overlapped
						if(Tv_BiTbl[Tv_BlkIdx].Db.SeqId == A_SeqId)
						{
							// found
							Tv_Result	=	TRUE;
							break;
						}
					}
					// next
					Tv_BlkIdx ++;
				}	//	while(Tv_BlkIdx < Tv_BlkCnt)
			}
			// check finished
			if(Tv_Result)
			{
				break;
			}

			// next
			Tv_BlkIdx	=	1;
			Tv_DskIdx ++;
		}	//	while(Tv_DskIdx < m_DiskCnt)
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcDskBlkDbErase(__u32 A_DskIdx, __u32 A_BlkIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrFsNvtBlkInfoBase	Tv_BiTbl;
		__u32	Tv_BlkCnt;
	// code --------------------
		Tv_Result	=	FALSE;

		if(A_DskIdx < m_DiskCnt)
		{
			Tv_BiTbl	=	m_DiskTbl[A_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
			if((1 < Tv_BlkCnt) && (NULL != Tv_BiTbl))
			{
				if((0 < A_BlkIdx) && (A_BlkIdx < Tv_BlkCnt))
				{
					Tv_BiTbl[A_BlkIdx].Db.BlkStat			=	E_GrFsNvtBlkStatNormal;
					Tv_BiTbl[A_BlkIdx].Db.InSize			=	0;
					Tv_BiTbl[A_BlkIdx].Db.OfsLastLog	=	-1;
					Tv_BiTbl[A_BlkIdx].Db.OvwtId			=	0;
					Tv_BiTbl[A_BlkIdx].Db.SeqId				=	0;
					Tv_BiTbl[A_BlkIdx].Db.TimeRng.TimeSt	=	0;
					Tv_BiTbl[A_BlkIdx].Db.TimeRng.TimeEd	=	0;
					Tv_BiTbl[A_BlkIdx].Db._rsvd0			=	0;
					Tv_BiTbl[A_BlkIdx].Db._rsvd1			=	0;
					Tv_BiTbl[A_BlkIdx].Db._rsvd2			=	0;
					Tv_BiTbl[A_BlkIdx].Db._rsvd3			=	0;

					// write
					if(E_GrErrNone == m_DiskTbl[A_DskIdx]->BlkInfoWriteByIdx(A_BlkIdx))
					{
						Tv_Result	=	TRUE;
					}
					
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpChkBlkInfoBroken(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsErase;
		__u32	Tv_DskIdx;
		Ptr_GrFsNvtBlkInfoBase	Tv_BiTbl;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if(0 != m_DiskCnt)
		{
			for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				Tv_BiTbl	=	m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
				if((1 < Tv_BlkCnt) && (NULL != Tv_BiTbl))
				{
					for( Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					{
						// check overlapped sequence block
						if(E_GrFsNvtBlkStatBad != Tv_BiTbl[Tv_BlkIdx].Db.BlkStat)
						{
							// init
							Tv_IsErase	=	FALSE;
							// check size zero 
							if(0 == Tv_BiTbl[Tv_BlkIdx].Db.InSize)
							{
								// check zero full
								if(E_GrFsNvtBlkStatFull == Tv_BiTbl[Tv_BlkIdx].Db.BlkStat)
								{
									DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpChkBlkInfoBroken - zero full block found!\n");
									Tv_IsErase	=	TRUE;
								}
								// check log offset
								if(0 <= Tv_BiTbl[Tv_BlkIdx].Db.OfsLastLog)
								{
									DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpChkBlkInfoBroken - bad log offset found!\n");
									Tv_IsErase	=	TRUE;
								}
								// exist sequence id
								if(E_GrFsNvtSeqIdNone != Tv_BiTbl[Tv_BlkIdx].Db.SeqId)
								{
									DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpChkBlkInfoBroken - empty block exist sequence id!\n");
									Tv_IsErase	=	TRUE;
								}
								// check exist time range
								if((0 != Tv_BiTbl[Tv_BlkIdx].Db.TimeRng.TimeSt) || (0 != Tv_BiTbl[Tv_BlkIdx].Db.TimeRng.TimeEd))
								{
									DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpChkBlkInfoBroken - empty block exist time range!\n");
									Tv_IsErase	=	TRUE;
								}

								// erase
								if(Tv_IsErase)
								{
									// erase block
									if(!LcDskBlkDbErase(Tv_DskIdx, Tv_BlkIdx))
									{
										// bad disk , go next disk
										break;
									}
								}
								// go next block
								continue;
							}

							// exist data
							// check bad time range
							if((0 == Tv_BiTbl[Tv_BlkIdx].Db.TimeRng.TimeSt) || (0 == Tv_BiTbl[Tv_BlkIdx].Db.TimeRng.TimeEd))
							{
								DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpChkBlkInfoBroken - bad time range!\n");
								Tv_IsErase	=	TRUE;
							}

							// bad sequence id
							if(E_GrFsNvtSeqIdNone == Tv_BiTbl[Tv_BlkIdx].Db.SeqId)
							{
								DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpChkBlkInfoBroken - bad sequence id!\n");
								Tv_IsErase	=	TRUE;
							}

							// erase
							if(Tv_IsErase)
							{
								// erase block
								if(!LcDskBlkDbErase(Tv_DskIdx, Tv_BlkIdx))
								{
									// bad disk , go next disk
									break;
								}
								// go next block
								continue;
							}

							// normal block
							// check block sequence id overlapped
							if(!Tv_Result)
							{
								if(LcBlkChkSeqOverlapped(Tv_DskIdx, Tv_BlkIdx, Tv_BiTbl[Tv_BlkIdx].Db.SeqId))
								{
									// overlapped id found
									DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpChkSeqOverlapped - overlapped sequence id found!\n");
									Tv_Result	=	TRUE;
								}
							}
						}
					}	//	for( Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
				}
				if(Tv_Result)
				{
					break;
				}
			}	//	for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcGrpRecInfoInit(void)
{
	// local -------------------
		__s32	Tv_FndDsk;
		__s32	Tv_FndBlk;
	// code --------------------
		// init
		m_RecDskIdx	=	-1;
		m_RecBlkIdx	=	-1;
		m_RecSeqId	=	0;

		if(0 == m_DiskCnt)
		{
			return;
		}

		// find newest block
		if(!LcGrpBlkFindNewest(&Tv_FndDsk,&Tv_FndBlk,&m_RecSeqId))
		{
			return;
		}

		// load record info
		LcGrpRecBlkOpen((__u32)Tv_FndDsk, (__u32)Tv_FndBlk);	

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcInit(void)
{
	// local -------------------
	// code --------------------
		// build disk table
		LcGrpDskTblBuild();
		
		// chekc sequence number
		if(LcGrpChkBlkInfoBroken())
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcInit - overlapped id found!\n");
			// rebuild sequence number
			LcGrpSeqIdRebuild();
		}
		// find record info
		LcGrpRecInfoInit();

		// group information update
		LcGrpStatUpdate();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcGrpDayRangeGet(__u32* A_PtrRtDaySt, __u32* A_PtrRtDayEd)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_DskIdx;
		__u32	Tv_DaySt;
		__u32	Tv_DayEd;
		__u32	Tv_DayIdx;
		Ptr_GrFsNvtDayInfo	Tv_DayTbl;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_DaySt	=	0;
		Tv_DayEd	=	0;

		if(0 != m_DiskCnt)
		{
			for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				if(E_GrDvrRecStatEmpty <= m_DiskTbl[Tv_DskIdx]->DiskStatGet())
				{
					Tv_DayTbl	=	m_DiskTbl[Tv_DskIdx]->DayInfoTableGet();
					if(NULL != Tv_DayTbl)
					{
						for(Tv_DayIdx = 0; Tv_DayIdx < E_GrFsNvtDayTblCntOfDisk; Tv_DayIdx++)
						{
							if(0 != Tv_DayTbl[Tv_DayIdx].TimeDay)
							{
								if(0 == Tv_DaySt)
								{
									Tv_DaySt	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
								}
								else if(Tv_DaySt >= Tv_DayTbl[Tv_DayIdx].TimeDay)
								{
									Tv_DaySt	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
								}

								if(0 == Tv_DayEd)
								{
									Tv_DayEd	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
								}
								else if(Tv_DayEd <= Tv_DayTbl[Tv_DayIdx].TimeDay)
								{
									Tv_DayEd	=	Tv_DayTbl[Tv_DayIdx].TimeDay;
								}

								Tv_Result	=	TRUE;
							}
						}
					}
				}
			}
		}

		// update result
		*A_PtrRtDaySt	=	Tv_DaySt;
		*A_PtrRtDayEd	=	Tv_DayEd;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcGrpDelSlateBlk(__u32 A_TimeBase)
{
	// local -------------------
		__u32	Tv_DskIdx;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
	// code --------------------
		// check disk count
		if(0 == m_DiskCnt)
		{
			return;
		}

		for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		{
			// get disk block head table
			Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
			if(NULL == Tv_PtrBi)
			{
				continue;
			}

			for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
			{
				// check block type
				if(E_GrFsNvtBlkStatBad == Tv_PtrBi[Tv_BlkIdx].Db.BlkStat)
				{
					continue;
				}

				// check data exist
				if((0 == Tv_PtrBi[Tv_BlkIdx].Db.InSize) || (0 == Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeSt) || 
					(0 == Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeEd))
				{
					continue;
				}

				// check time range
				if(A_TimeBase < Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeEd )
				{
					continue;
				}

				LcDskBlkDbErase(Tv_DskIdx, Tv_BlkIdx);

			}	//	for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
		}	//	for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcGrpDelSlateTimeMap(__u32 A_TimeBase)
{
	// local -------------------
		__u32	Tv_DskIdx;
		__u32	Tv_DayIdx;
		Ptr_GrFsNvtDayInfo	Tv_DayTbl;
	// code --------------------
		// check disk count
		if(0 == m_DiskCnt)
		{
			return;
		}

		for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		{
			Tv_DayTbl	=	m_DiskTbl[Tv_DskIdx]->DayInfoTableGet();
			for(Tv_DayIdx = 0; Tv_DayIdx < E_GrFsNvtDayTblCntOfDisk; Tv_DayIdx++)
			{
				if(0 == Tv_DayTbl[Tv_DayIdx].TimeDay)
				{
					continue;
				}
				// check time
				if(A_TimeBase < Tv_DayTbl[Tv_DayIdx].TimeDay)
				{
					continue;
				}

				// do delete
				LcDskDayDelByIdx(Tv_DskIdx, Tv_DayIdx);

			}	//	for(Tv_DayIdx = 0; Tv_DayIdx < Tv_DayCnt; Tv_DayIdx++)

		}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::DeleteStaleData(__u32 A_TimeBase)
{
	// local -------------------
		
	// code --------------------
		// delete block
		LcGrpDelSlateBlk(A_TimeBase);

		// delete time map
		LcGrpDelSlateTimeMap(A_TimeBase);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcCchMinAccessUpdate(__u32 A_DskIdx, __u32 A_DayIdx, BOOL8 A_IsDirt)
{
	// local -------------------
		__s32	Tv_CchIdx;
	// code --------------------
		Tv_CchIdx	=	LcCchMinFindInCache(A_DskIdx, A_DayIdx);
		if(0 > Tv_CchIdx)
		{
			return	FALSE;
		}

		// update access time
		m_CchMin.Items[Tv_CchIdx].AccTime	=	V_GrFsNvtEnv.Tick100ms;
		if(A_IsDirt)
		{
			m_CchMin.Items[Tv_CchIdx].IsDirt	=	TRUE;
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchMinFlushAll(void)
{
	// local -------------------
		__u32	Tv_CchIdx;
	// code --------------------
		if((NULL == m_CchMin.Items) || (0 == m_CchMin.Cnt))
		{
			return;
		}

		for( Tv_CchIdx = 0; Tv_CchIdx < m_CchMin.Cnt; Tv_CchIdx++)
		{
			LcCchMinFlushByCchIdx(Tv_CchIdx);
		}

}
//--------------------------------------------------------------------
__s32	Cls_GrFsNvtRgrp::LcCchDataFindByHndl(__u64 A_Hndl)
{
	// local -------------------
		__u32	Tv_ItmIdx;
	// code --------------------
		for ( Tv_ItmIdx = 0; Tv_ItmIdx < m_CchData.Cnt; Tv_ItmIdx++)
		{
			if (A_Hndl == m_CchData.Items[Tv_ItmIdx].Hndl)
			{
				// found
				return	(__s32)Tv_ItmIdx;
			}
		}

		return	-1;
}
//--------------------------------------------------------------------
__s32	Cls_GrFsNvtRgrp::LcCchDataFindEmpty(void)
{
	// local -------------------
		__u32	Tv_ItmIdx;
	// code --------------------
		for (Tv_ItmIdx = 0; Tv_ItmIdx < m_CchData.Cnt; Tv_ItmIdx++)
		{
			if (E_GrDvrRecHndlNone == m_CchData.Items[Tv_ItmIdx].Hndl)
			{
				// found
				return	(__s32)Tv_ItmIdx;
			}
		}

		return	-1;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::LcCchDataChkRsvdByIdx(__u32 A_CchIdx)
{
	// local -------------------
		__u32	Tv_RsvIdx;
	// code --------------------
		// check reserved exist
		if (0 == m_CchData.Rsvd.ReqCnt)
		{
			return	FALSE;
		}

		// check reserved
		for (Tv_RsvIdx = 0; Tv_RsvIdx < (__u32)m_CchData.Rsvd.ReqCnt; Tv_RsvIdx++)
		{
			if (m_CchData.Rsvd.Items[Tv_RsvIdx].Hndl == m_CchData.Items[A_CchIdx].Hndl)
			{
				return	TRUE;
			}
		}

		return	FALSE;
}
//--------------------------------------------------------------------
__s32	Cls_GrFsNvtRgrp::LcCchDataFindLoadable(void)
{
	// local -------------------
		__u32	Tv_ItmIdx;
		__s32	Tv_FndIdx;
		__u32	Tv_FndTick;
	// code --------------------
		Tv_FndIdx	=	-1;

		for (Tv_ItmIdx = 0; Tv_ItmIdx < m_CchData.Cnt; Tv_ItmIdx++)
		{
			// check exsit
			if (E_GrDvrRecHndlNone != m_CchData.Items[Tv_ItmIdx].Hndl)
			{
				// check unlocked
				if (0 == m_CchData.Items[Tv_ItmIdx].Locked)
				{
					// check reserved
					if(!LcCchDataChkRsvdByIdx(Tv_ItmIdx))
					{
						// found
						if (0 > Tv_FndIdx)
						{
							Tv_FndIdx		=	(__s32)Tv_ItmIdx;
							Tv_FndTick	=	m_CchData.Items[Tv_ItmIdx].AccTime;
						}
						else
						{
							if (Tv_FndTick > m_CchData.Items[Tv_ItmIdx].AccTime)
							{
								// relpace
								Tv_FndIdx		=	(__s32)Tv_ItmIdx;
								Tv_FndTick	=	m_CchData.Items[Tv_ItmIdx].AccTime;
							}
						}
					}
				}
			}
		}

		return	Tv_FndIdx;

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchDataFlushByIdx(__u32 A_CchIdx)
{
	// local -------------------
		__u32	Tv_WriteSize;
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		// check index
		if(m_CchData.Cnt <= A_CchIdx)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcCchDataFlushByIdx - bad index (idx:%d, total:%d) input!\n", A_CchIdx, m_CchData.Cnt);
			return;
		}
		// check valid cache
		if(E_GrDvrRecHndlNone == m_CchData.Items[A_CchIdx].Hndl)
		{
			return;
		}

		// check dirted
		if(!m_CchData.Items[A_CchIdx].IsDirt)
		{
			return;
		}

		// do write
		Tv_ErrCode	=	LcGrpWriteAlignByHndl(m_CchData.Items[A_CchIdx].Hndl, m_CchData.Items[A_CchIdx].PtrBuf, &Tv_WriteSize);

		m_CchData.Items[A_CchIdx].IsDirt	=	FALSE;

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchDataFlushAll(void)
{
	// local -------------------
		__u32	Tv_CchIdx;
	// code --------------------
		if(0 == m_CchData.Cnt)
		{
			return;
		}

		for(Tv_CchIdx = 0; Tv_CchIdx < m_CchData.Cnt; Tv_CchIdx++)
		{
			LcCchDataFlushByIdx(Tv_CchIdx);
		}
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcCchDataFreeByIdx(__u32 A_CchIdx)
{
	// local -------------------
	// code --------------------
		// check index
		if(m_CchData.Cnt <= A_CchIdx)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcCchDataFreeByIdx - bad index (idx:%d, total:%d) input!\n", A_CchIdx, m_CchData.Cnt);
			return;
		}
		// check valid cache
		if(E_GrDvrRecHndlNone == m_CchData.Items[A_CchIdx].Hndl)
		{
			return;
		}

		// check dirted
		if(m_CchData.Items[A_CchIdx].IsDirt)
		{
			// flush
			LcCchDataFlushByIdx(A_CchIdx);
		}

		// check locked
		if(0 < m_CchData.Items[A_CchIdx].Locked)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcCchDataFreeByIdx - cache %d locked!\n", A_CchIdx);
			return;
		}

		// release cache
		m_CchData.Items[A_CchIdx].BlkSeq	=	0;
		m_CchData.Items[A_CchIdx].Hndl		=	E_GrDvrRecHndlNone;
		m_CchData.Items[A_CchIdx].IsDirt	=	FALSE;
		m_CchData.Items[A_CchIdx].Locked	=	0;

}
//--------------------------------------------------------------------
__u32	Cls_GrFsNvtRgrp::LcCchDataCalSizeByHndl(__u64 A_Hndl)
{
	// local -------------------
		__u32	Tv_Result;

		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkOfs;

		Ptr_GrFsNvtDbBlkInfo	Tv_PtrBi;
	// code --------------------
		// check handle
		if(E_GrDvrRecHndlNone == A_Hndl)
		{
			return	0;
		}

		// get block info
		GrFsNvtHndlDec(A_Hndl, &Tv_DskIdx, &Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);

		// getblock info
		Tv_PtrBi	=	LcDskDbBlkInfoGet(Tv_DskIdx, Tv_BlkIdx);
		if(NULL == Tv_PtrBi)
		{
			return	0;
		}

		if(Tv_PtrBi->InSize <= Tv_BlkOfs)
		{
			return	0;
		}
	
		Tv_Result	=	Tv_PtrBi->InSize - Tv_BlkOfs;
		if(E_GrFsNvtRgrpCchDataBufSize < Tv_Result)
		{
			Tv_Result	=	E_GrFsNvtRgrpCchDataBufSize;
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtRgrp::LcGrpStatUpdate(void)
{
	// local -------------------
		__u8	Tv_Stat;
		BOOL8	Tv_IsAllBlkUse;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkIdx;
		__u32	Tv_BlkCnt;
		__u64	Tv_Total;
		__u64	Tv_RecSize;

		Def_GrTime	Tv_TmSt;
		Def_GrTime	Tv_TmEd;

		Ptr_GrFsNvtBlkInfoBase Tv_PtrBi;

	// code --------------------
		Tv_Stat			=	E_GrDvrRecStatNotexist;
		Tv_Total		=	0;
		Tv_RecSize	=	0;
		Tv_TmSt			=	0;
		Tv_TmEd			=	0;

		if(0 != m_DiskCnt)
		{
			Tv_IsAllBlkUse	=	TRUE;
			Tv_Stat		=	E_GrDvrRecStatBad;

			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrBi)
				{
					for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					{
						// check block info
						if(E_GrFsNvtBlkStatBad != Tv_PtrBi[Tv_BlkIdx].Db.BlkStat)
						{
							// add total
							Tv_Total	=	Tv_Total + E_GrFsNvtBlkSize;

							// check full block
							if(E_GrFsNvtBlkStatFull == Tv_PtrBi[Tv_BlkIdx].Db.BlkStat)
							{
								Tv_RecSize	=	Tv_RecSize + E_GrFsNvtBlkSize;
							}
							else
							{
								if(0 != Tv_PtrBi[Tv_BlkIdx].Db.InSize)
								{
									Tv_RecSize	=	Tv_RecSize + (__u64)Tv_PtrBi[Tv_BlkIdx].Db.InSize;
								}
								else
								{
									Tv_IsAllBlkUse	=	FALSE;
								}
							}

							// check time range
							if((0 != Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeSt) && (0 != Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeEd))
							{
								// update start time
								if(E_GrFsNvtTimeMin > Tv_TmSt)
								{
									Tv_TmSt	=	Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeSt;
								}
								else if(Tv_TmSt > Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeSt)
								{
									Tv_TmSt	=	Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeSt;
								}
								// update end time
								if(0 == Tv_TmEd)
								{
									Tv_TmEd	=	Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeEd;
								}
								else if(Tv_TmEd < Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeEd)
								{
									Tv_TmEd	=	Tv_PtrBi[Tv_BlkIdx].Db.TimeRng.TimeEd;
								}
							}

						}	//	if(E_GrFsNvtBlkStatBad != Tv_PtrBi[Tv_BlkIdx].Db.BlkStat)
					}
				}
			}

			// check status
			if(0 != Tv_Total)
			{
				Tv_Stat	=	E_GrDvrRecStatEmpty;

				if(0 != Tv_RecSize)
				{
					Tv_Stat	=	E_GrDvrRecStatNormal;

					if(m_IsOvwtMode)
					{
						if(Tv_IsAllBlkUse)
						{
							Tv_Stat	=	E_GrDvrRecStatOverwrite;
						}
					}
					else
					{
						if(Tv_RecSize >= Tv_Total)
						{
							Tv_Stat	=	E_GrDvrRecStatFull;
						}
					}
				}
			}
		}

		// update
		m_GrpStat				=	Tv_Stat;
		m_GrpTotalSize	=	Tv_Total;
		m_GrpRecSize		=	Tv_RecSize;
		m_RecTimeSt			=	Tv_TmSt;
		m_RecTimeEd			=	Tv_TmEd;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpReadAlignByHndl(__u64 A_Hndl, void* A_PtrBuf, __u32* A_PtrRtSize)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;

		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkOfs;

		__u32	Tv_InSize;
		__u32	Tv_ReadSize;

		Ptr_GrFsNvtDbBlkInfo	Tv_PtrBi;
	// code --------------------
		// check handle
		if(E_GrDvrRecHndlNone == A_Hndl)
		{
			return	E_GrErrBadParam;
		}

		// decode handle
		GrFsNvtHndlDec(A_Hndl, &Tv_DskIdx, &Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);

		// check offset
		if(0 != (Tv_BlkOfs & E_GrFsNvtRgrpCchDataOfsMask))
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpReadAlignByHndl - bad handle offset input!\n");
			return	E_GrErrBadParam;
		}

		// check disk index
		if((__s32)m_DiskCnt <= Tv_DskIdx)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpReadAlignByHndl - bad handle disk index input!\n");
			return	E_GrErrBadParam;
		}

		// get block info
		Tv_PtrBi	=	LcDskDbBlkInfoGet(Tv_DskIdx, Tv_BlkIdx);
		if(NULL == Tv_PtrBi)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpReadAlignByHndl - block info get fail!\n");
			return	E_GrErrInitFail;
		}

		if(Tv_PtrBi->InSize <= Tv_BlkOfs)
		{
			*A_PtrRtSize	=	0;
			return	E_GrErrNone;
		}

		Tv_InSize	=	Tv_PtrBi->InSize - Tv_BlkOfs;
		if(E_GrFsNvtRgrpCchDataBufSize < Tv_InSize)
		{
			Tv_InSize	=	E_GrFsNvtRgrpCchDataBufSize;
		}

		// patch
		Tv_ReadSize	=	(Tv_InSize + (E_GrFsNvtHddIoUnitSize - 1)) & E_GrFsNvtHddIoAdrMask;

		// upate return size
		*A_PtrRtSize	=	Tv_InSize;

		// request read
		Tv_ErrCode	=	m_DiskTbl[Tv_DskIdx]->BlkReadPage(Tv_BlkIdx, Tv_BlkOfs, A_PtrBuf, Tv_ReadSize);
		if(E_GrErrNone != Tv_ErrCode)
		{
			// fail
			A_PtrRtSize	=	0;
			// update group info
			LcGrpStatUpdate();
		}

		return	Tv_ErrCode;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcGrpWriteAlignByHndl(__u64 A_Hndl, void* A_PtrBuf, __u32* A_PtrRtSize)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;

		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkOfs;

		__u32	Tv_InSize;
		__u32	Tv_WriteSize;

		Ptr_GrFsNvtDbBlkInfo	Tv_PtrBi;

	// code --------------------
		// check handle
		if(E_GrDvrRecHndlNone == A_Hndl)
		{
			return	E_GrErrBadParam;
		}

		// decode handle
		GrFsNvtHndlDec(A_Hndl, &Tv_DskIdx, &Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);

		// check offset
		if(0 != (Tv_BlkOfs & E_GrFsNvtRgrpCchDataOfsMask ))
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpWriteAlignByHndl - bad handle offset input!\n");
			return	E_GrErrBadParam;
		}

		// check disk index
		if((__s32)m_DiskCnt <= Tv_DskIdx)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpWriteAlignByHndl - bad handle disk index input!\n");
			return	E_GrErrBadParam;
		}

		// get block info
		Tv_PtrBi	=	LcDskDbBlkInfoGet(Tv_DskIdx, Tv_BlkIdx);
		if(NULL == Tv_PtrBi)
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcGrpWriteAlignByHndl - block info get fail!\n");
			return	E_GrErrInitFail;
		}

		if(Tv_PtrBi->InSize <= Tv_BlkOfs)
		{
			*A_PtrRtSize	=	0;
			return	E_GrErrNone;
		}

		Tv_InSize	=	Tv_PtrBi->InSize - Tv_BlkOfs;
		if(E_GrFsNvtRgrpCchDataBufSize < Tv_InSize)
		{
			Tv_InSize	=	E_GrFsNvtRgrpCchDataBufSize;
		}

		// patch
		Tv_WriteSize	=	(Tv_InSize + (E_GrFsNvtHddIoUnitSize - 1)) & E_GrFsNvtHddIoAdrMask;

		// upate return size
		*A_PtrRtSize	=	Tv_InSize;

		// request read
		Tv_ErrCode	=	m_DiskTbl[Tv_DskIdx]->BlkWritePage(Tv_BlkIdx, Tv_BlkOfs, A_PtrBuf, Tv_WriteSize);
		if(E_GrErrNone != Tv_ErrCode)
		{
			// fail
			A_PtrRtSize	=	0;
			// update group info
			LcGrpStatUpdate();
		}

		return	Tv_ErrCode;

}
//--------------------------------------------------------------------
Ptr_GrFsNvtDbBlkInfo	Cls_GrFsNvtRgrp::LcGrpDbBlkInfoGetByHndl(__u64 A_Hndl)
{
	// local -------------------
		Ptr_GrFsNvtDbBlkInfo	Tv_PtrBi;

		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__u32	Tv_BlkOfs;
		__s32	Tv_BlkIdx;

		__u32	Tv_BlkCnt;
	// code --------------------
		if(E_GrDvrRecHndlNone == A_Hndl)
		{
			return	NULL;
		}

		GrFsNvtHndlDec(A_Hndl, &Tv_DskIdx, &Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);

		// check disk index
		if((__s32)m_DiskCnt <= Tv_DskIdx)
		{
			return	NULL;
		}

		// get block info
		Tv_PtrBi	=	(Ptr_GrFsNvtDbBlkInfo)m_DiskTbl[Tv_DskIdx]->BlkInfoTableGet(&Tv_BlkCnt);
		if(NULL == Tv_PtrBi)
		{
			return	NULL;
		}

		// check block index
		if((0 >= Tv_BlkIdx) || ((__s32)Tv_BlkCnt <= Tv_BlkIdx))
		{
			return	NULL;
		}

		return	&Tv_PtrBi[Tv_BlkIdx];
		
}
//--------------------------------------------------------------------
__s32	Cls_GrFsNvtRgrp::LcCchDataGetByHndl(__u64 A_Hndl, BOOL8 A_IsReadSkip, __u32* A_PtrRtInSize, Def_GrErrCode* A_PtrRtErr)
{
	// local -------------------
		__s32	Tv_ItmIdx;
		__u32	Tv_ReadSize;
		Ptr_GrFsNvtDbBlkInfo	Tv_PtrBi;
	// code --------------------
		// check exist cache buffer
		if (0 == m_CchData.Cnt)
		{
			*A_PtrRtErr			=	E_GrErrInitFail;
			*A_PtrRtInSize	=	0;
			return	-1;
		}

		// check handle
		if (E_GrDvrRecHndlNone == A_Hndl)
		{
			*A_PtrRtErr			=	E_GrErrBadParam;
			*A_PtrRtInSize	=	0;
			return	-1;
		}

		// check exist cached data
		Tv_ItmIdx	=	LcCchDataFindByHndl(A_Hndl);
		if (0 <= Tv_ItmIdx)
		{
			// increase lock count
			*A_PtrRtErr			=	E_GrErrNone;
			*A_PtrRtInSize	=	LcCchDataCalSizeByHndl(A_Hndl);
			m_CchData.Items[Tv_ItmIdx].Locked ++;
			m_CchData.Items[Tv_ItmIdx].AccTime	=	V_GrFsNvtEnv.Tick100ms;
			return	(__s32)Tv_ItmIdx;
		}

		// find empty item slot
		Tv_ItmIdx	=	LcCchDataFindEmpty();

		// check found
		if (0 > Tv_ItmIdx)
		{
			// find oldest unlocked , unreserved cache
			Tv_ItmIdx	=	LcCchDataFindLoadable();
		}

		// check loadable found
		if (0 > Tv_ItmIdx)
		{
			DbgMsgPrint("[Cache Data] all locked!\n");
			GrTaskSvrMsgSysErrNotify(100);
			*A_PtrRtErr			=	E_GrErrBufFull;
			*A_PtrRtInSize	=	0;
			return	-1;
		}

		// release cache
		LcCchDataFreeByIdx(Tv_ItmIdx);

		// check read skip
		if (A_IsReadSkip)
		{
			*A_PtrRtErr			=	E_GrErrNone;
			*A_PtrRtInSize	=	0;

			m_CchData.Items[Tv_ItmIdx].Locked		=	1;
			m_CchData.Items[Tv_ItmIdx].AccTime	=	V_GrFsNvtEnv.Tick100ms;
			m_CchData.Items[Tv_ItmIdx].Hndl			=	A_Hndl;
			m_CchData.Items[Tv_ItmIdx].BlkSeq		=	0;
			Tv_PtrBi	=	LcGrpDbBlkInfoGetByHndl(A_Hndl);
			if(NULL != Tv_PtrBi)
			{
				m_CchData.Items[Tv_ItmIdx].BlkSeq		=	Tv_PtrBi->SeqId;
			}
			else
			{
				DbgMsgPrint("Cls_GrFsNvtRgrp::LcCchDataGetByHndl - block info get fail!\n");
			}

			return	(__s32)Tv_ItmIdx;
		}

		// load by handle
		*A_PtrRtErr	=	LcGrpReadAlignByHndl(A_Hndl, m_CchData.Items[Tv_ItmIdx].PtrBuf, &Tv_ReadSize);
		if(E_GrErrNone != (*A_PtrRtErr))
		{
			*A_PtrRtInSize	=	0;
			return	-1;
		}

		m_CchData.Items[Tv_ItmIdx].Locked		=	1;
		m_CchData.Items[Tv_ItmIdx].AccTime	=	V_GrFsNvtEnv.Tick100ms;
		m_CchData.Items[Tv_ItmIdx].Hndl			=	A_Hndl;
		m_CchData.Items[Tv_ItmIdx].BlkSeq		=	0;

		Tv_PtrBi	=	LcGrpDbBlkInfoGetByHndl(A_Hndl);
		if(NULL != Tv_PtrBi)
		{
			m_CchData.Items[Tv_ItmIdx].BlkSeq		=	Tv_PtrBi->SeqId;
		}
		else
		{
			DbgMsgPrint("Cls_GrFsNvtRgrp::LcCchDataGetByHndl - block info get fail!\n");
		}

		*A_PtrRtInSize	=	Tv_ReadSize;
		return	(__s32)Tv_ItmIdx;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcCchDataGetByHndlToSctl(Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
		__s32	Tv_CchIdx;
	// code --------------------
		
		// unlock cache
		LcCchDataUnlockBySctl(A_PtrCtrl);

		Tv_CchIdx	=	LcCchDataGetByHndl(A_Hndl, FALSE, &A_PtrCtrl->InSize, &Tv_ErrCode);
		if(0 <= Tv_CchIdx)
		{
			A_PtrCtrl->Hndl		=	A_Hndl;
			A_PtrCtrl->BlkSeq	=	m_CchData.Items[Tv_CchIdx].BlkSeq;
			A_PtrCtrl->Prgs		=	0;
			A_PtrCtrl->PtrBuf	=	m_CchData.Items[Tv_CchIdx].PtrBuf;
		}
		
		return	Tv_ErrCode;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcDskReadToBuf(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_Offset, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
		__u64	Tv_Hndl;
		__u32	Tv_CchOfs;
		__s32	Tv_CchIdx;
		__u32	Tv_HeadSize;
		__u32	Tv_TailSize;
		__u32	Tv_BlkOfs;

		__u32	Tv_InSize;

		void*	Tv_PtrSr;
		void*	Tv_PtrTg;

		Def_GrErrCode	Tv_ErrCode;

		Ptr_GrFsNvtDbBlkInfo	Tv_PtrBi;
	// code --------------------
		// check disk index
		if((m_DiskCnt <= A_DskIdx) || (0 == A_Size) || (E_GrFsNvtRgrpCchDataBufSize < A_Size))
		{
			return	E_GrErrBadParam;
		}

		// calculate info
		Tv_CchOfs		=	A_Offset & E_GrFsNvtRgrpCchDataOfsMask;
		Tv_BlkOfs		=	A_Offset & E_GrFsNvtRgrpCchDataAdrMask;
		Tv_HeadSize	=	A_Size;
		Tv_TailSize	=	0;
		if(E_GrFsNvtRgrpCchDataBufSize < (Tv_CchOfs + Tv_HeadSize))
		{
			Tv_TailSize	=	(Tv_CchOfs + Tv_HeadSize) - E_GrFsNvtRgrpCchDataBufSize;
			Tv_HeadSize	=	Tv_HeadSize - Tv_TailSize;
		}

		// get block info
		Tv_PtrBi	=	LcDskDbBlkInfoGet(A_DskIdx, A_BlkIdx);
		if(NULL == Tv_PtrBi)
		{
			return	E_GrErrFail;
		}

		// get cache
		Tv_Hndl	=	GrFsNvtHndlEnc((__s32)A_DskIdx, Tv_BlkOfs, (__s32)A_BlkIdx, Tv_PtrBi->OvwtId);
		Tv_CchIdx	=	LcCchDataGetByHndl(Tv_Hndl, FALSE, &Tv_InSize, &Tv_ErrCode);
		if(0 > Tv_CchIdx)
		{
			return	Tv_ErrCode;
		}

		// copy head
		Tv_PtrSr	=	(void*)((Def_GrCalPtr)m_CchData.Items[Tv_CchIdx].PtrBuf + (Def_GrCalPtr)Tv_CchOfs);
		GrDumyCopyMem(A_PtrBuf, Tv_PtrSr, Tv_HeadSize);

		// done
		LcCchDataUnlockByIdx(Tv_CchIdx);

		// check exist tail
		if(0 == Tv_TailSize)
		{
			return	E_GrErrNone;
		}

		// get next cache
		Tv_BlkOfs	=	Tv_BlkOfs + E_GrFsNvtRgrpCchDataBufSize;
		if((__u32)E_GrFsNvtBlkSize <= Tv_BlkOfs)
		{
			return	E_GrErrBadParam;
		}

		Tv_Hndl	=	GrFsNvtHndlEnc((__s32)A_DskIdx, Tv_BlkOfs, (__s32)A_BlkIdx, Tv_PtrBi->OvwtId);
		Tv_CchIdx	=	LcCchDataGetByHndl(Tv_Hndl, FALSE, &Tv_InSize, &Tv_ErrCode);
		if(0 > Tv_CchIdx)
		{
			return	Tv_ErrCode;
		}

		// copy tail
		Tv_PtrTg	=	(void*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_HeadSize);
		GrDumyCopyMem(Tv_PtrTg, m_CchData.Items[Tv_CchIdx].PtrBuf, Tv_TailSize);

		// done
		LcCchDataUnlockByIdx(Tv_CchIdx);

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::LcDskWriteFromBuf(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_Offset, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
		__u64	Tv_Hndl;
		__u32	Tv_CchOfs;
		__s32	Tv_CchIdx;
		__u32	Tv_HeadSize;
		__u32	Tv_TailSize;
		__u32	Tv_BlkOfs;

		__u32	Tv_InSize;

		void*	Tv_PtrSr;
		void*	Tv_PtrTg;

		Def_GrErrCode	Tv_ErrCode;

		Ptr_GrFsNvtDbBlkInfo	Tv_PtrBi;

	// code --------------------
		// check disk index
		if((m_DiskCnt <= A_DskIdx) || (0 == A_Size) || (E_GrFsNvtRgrpCchDataBufSize < A_Size))
		{
			return	E_GrErrBadParam;
		}

		// calculate info
		Tv_CchOfs		=	A_Offset & E_GrFsNvtRgrpCchDataOfsMask;
		Tv_BlkOfs		=	A_Offset & E_GrFsNvtRgrpCchDataAdrMask;
		Tv_HeadSize	=	A_Size;
		Tv_TailSize	=	0;
		if(E_GrFsNvtRgrpCchDataBufSize < (Tv_CchOfs + Tv_HeadSize))
		{
			Tv_TailSize	=	(Tv_CchOfs + Tv_HeadSize) - E_GrFsNvtRgrpCchDataBufSize;
			Tv_HeadSize	=	Tv_HeadSize - Tv_TailSize;
		}

		// get block info
		Tv_PtrBi	=	LcDskDbBlkInfoGet(A_DskIdx, A_BlkIdx);
		if(NULL == Tv_PtrBi)
		{
			return	E_GrErrFail;
		}

		// get cache
		Tv_Hndl	=	GrFsNvtHndlEnc((__s32)A_DskIdx, Tv_BlkOfs, (__s32)A_BlkIdx, Tv_PtrBi->OvwtId);
		Tv_CchIdx	=	LcCchDataGetByHndl(Tv_Hndl, FALSE, &Tv_InSize, &Tv_ErrCode);
		if(0 > Tv_CchIdx)
		{
			return	Tv_ErrCode;
		}

		// copy head
		Tv_PtrTg	=	(void*)((Def_GrCalPtr)m_CchData.Items[Tv_CchIdx].PtrBuf + (Def_GrCalPtr)Tv_CchOfs);
		GrDumyCopyMem(Tv_PtrTg, A_PtrBuf, Tv_HeadSize);

		// done
		m_CchData.Items[Tv_CchIdx].IsDirt	=	TRUE;		// mark dirty
		LcCchDataUnlockByIdx(Tv_CchIdx);

		// check exist tail
		if(0 == Tv_TailSize)
		{
			return	E_GrErrNone;
		}

		// get next cache
		Tv_BlkOfs	=	Tv_BlkOfs + E_GrFsNvtRgrpCchDataBufSize;
		if((__u32)E_GrFsNvtBlkSize <= Tv_BlkOfs)
		{
			return	E_GrErrBadParam;
		}

		Tv_Hndl	=	GrFsNvtHndlEnc((__s32)A_DskIdx, Tv_BlkOfs, (__s32)A_BlkIdx, Tv_PtrBi->OvwtId);
		Tv_CchIdx	=	LcCchDataGetByHndl(Tv_Hndl, FALSE, &Tv_InSize, &Tv_ErrCode);
		if(0 > Tv_CchIdx)
		{
			return	Tv_ErrCode;
		}

		// copy tail
		Tv_PtrSr	=	(void*)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_HeadSize);
		GrDumyCopyMem(m_CchData.Items[Tv_CchIdx].PtrBuf,Tv_PtrSr	, Tv_TailSize);

		// done
		m_CchData.Items[Tv_CchIdx].IsDirt	=	TRUE;		// mark dirty
		LcCchDataUnlockByIdx(Tv_CchIdx);

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtRgrp::RtlCchDataRsvProc(void)
{
	// local -------------------
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__u32	Tv_BlkOfs;
		__s32	Tv_BlkIdx;

		__s32	Tv_CchIdx;
		__u32	Tv_BlkSeq;

		__u32	Tv_InSize;

		Def_GrErrCode	Tv_ErrCode;
		Ptr_GrFsNvtDbBlkInfo	Tv_PtrBi;

		__u64	Tv_Hndl;

	// code --------------------
		// check cache reserve
		if(0 == m_CchData.Rsvd.ReqCnt)
		{
			return	FALSE;
		}

		// reserve completed
		if(m_CchData.Rsvd.ReqCnt <= m_CchData.Rsvd.Prgs)
		{
			return	FALSE;
		}

		// get previous status
		if(0 == m_CchData.Rsvd.Prgs)
		{
			Tv_Hndl	=	m_CchData.Rsvd.HndlBase;
		}
		else
		{
			Tv_Hndl	=	m_CchData.Rsvd.Items[m_CchData.Rsvd.Prgs - 1].Hndl;
		}
		GrFsNvtHndlDec(Tv_Hndl, &Tv_DskIdx, &Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);

		// check direction
		if(!m_CchData.Rsvd.IsDirBack)
		{
			// forward
			Tv_BlkOfs	=	Tv_BlkOfs + E_GrFsNvtRgrpCchDataBufSize;
			if((__u32)E_GrFsNvtBlkSize <= Tv_BlkOfs)
			{
				// go next block
				Tv_PtrBi	=	LcGrpDbBlkInfoGetByHndl(Tv_Hndl);
				if(NULL == Tv_PtrBi)
				{
					// disk bad occure
					// remove all cache
					m_CchData.Rsvd.ReqCnt	=	0;
					m_CchData.Rsvd.Prgs		=	0;
					return	FALSE;
				}
				Tv_BlkSeq	=	Tv_PtrBi->SeqId;

				// check record seq
				if(Tv_BlkSeq >= m_RecSeqId)
				{
					// finish
					m_CchData.Rsvd.ReqCnt	=	0;
					m_CchData.Rsvd.Prgs		=	0;
					return	FALSE;
				}

				Tv_BlkSeq ++;
				if(!LcGrpBlkFindBySeqFw(Tv_BlkSeq, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId))
				{
					// finish
					m_CchData.Rsvd.ReqCnt	=	0;
					m_CchData.Rsvd.Prgs		=	0;
					return	FALSE;
				}
				Tv_BlkOfs		=	0;
			}
		}
		else
		{
			// backward
			if(E_GrFsNvtRgrpCchDataBufSize > Tv_BlkOfs)
			{
				// go next block
				Tv_PtrBi	=	LcGrpDbBlkInfoGetByHndl(Tv_Hndl);
				if(NULL == Tv_PtrBi)
				{
					// disk bad occure
					// remove all cache
					m_CchData.Rsvd.ReqCnt	=	0;
					m_CchData.Rsvd.Prgs		=	0;
					return	FALSE;
				}
				Tv_BlkSeq	=	Tv_PtrBi->SeqId;

				// check record seq
				if( 1 >= Tv_BlkSeq)
				{
					// finish
					m_CchData.Rsvd.ReqCnt	=	0;
					m_CchData.Rsvd.Prgs		=	0;
					return	FALSE;
				}

				Tv_BlkSeq --;
				if(!LcGrpBlkFindBySeqBw(Tv_BlkSeq, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId,&Tv_BlkOfs))
				{
					// finish
					m_CchData.Rsvd.ReqCnt	=	0;
					m_CchData.Rsvd.Prgs		=	0;
					return	FALSE;
				}

				if(0 == Tv_BlkOfs)
				{
					// finish
					m_CchData.Rsvd.ReqCnt	=	0;
					m_CchData.Rsvd.Prgs		=	0;
					return	FALSE;
				}
				Tv_BlkOfs		=	(Tv_BlkOfs - 1) & E_GrFsNvtRgrpCchDataAdrMask;
			}
			else
			{
				Tv_BlkOfs	=	Tv_BlkOfs - E_GrFsNvtRgrpCchDataBufSize;
			}
		}
		
		// do cache read
		Tv_Hndl	=	GrFsNvtHndlEnc(Tv_DskIdx, Tv_BlkOfs, Tv_BlkIdx, Tv_OvwtId);
		Tv_CchIdx	= LcCchDataGetByHndl(Tv_Hndl, FALSE, &Tv_InSize, &Tv_ErrCode);

		if(0 > Tv_CchIdx)
		{
			// finish
			m_CchData.Rsvd.ReqCnt	=	0;
			m_CchData.Rsvd.Prgs		=	0;
			return	FALSE;
		}

		// update cache info
		m_CchData.Rsvd.Items[m_CchData.Rsvd.Prgs].Hndl	=	Tv_Hndl;

		//DbgMsgPrint("[FS] caching %08X%08X !\n", (__u32)(Tv_Hndl >> 32), (__u32)Tv_Hndl);

		// unlock data
		LcCchDataUnlockByIdx((__u32)Tv_CchIdx);

		// go next
		m_CchData.Rsvd.Prgs ++;
		if(m_CchData.Rsvd.ReqCnt <= m_CchData.Rsvd.Prgs)
		{
			// finish
			m_CchData.Rsvd.ReqCnt	=	0;
			m_CchData.Rsvd.Prgs		=	0;
		}

		return	TRUE;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtRgrp::SctlFree(Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
	// local -------------------
	// code --------------------
		LcCchDataUnlockBySctl(A_PtrCtrl);

		return	E_GrErrNone;
}
//--------------------------------------------------------------------
