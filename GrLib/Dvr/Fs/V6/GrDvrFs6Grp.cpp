/*
 GAUSE file system version 6 group

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
#include <Dvr/Fs/V6/GrDvrFs6Base.h>
#include <Dvr/Fs/V6/GrDvrFs6Grp.h>
//#include <Dvr/Fs/V6/GrDvrFs6Mng.h>

#include	<stdlib.h>

//====================================================================
//local const

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

extern	St_GrDvrFs6Env	V_GrDvrFs6Env;

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs6Grp::Cls_GrDvrFs6Grp(__u8 A_GroupType, __u8 A_DskMap, BOOL8 A_IsOvrtMode)
{
	// local -------------------
		// __u8	Tv_IntIdx;
		__u32	Tv_WkIdx;
		__u64	Tv_Total;
		__u64	Tv_RecSize;
	// code --------------------
		//init
		m_DiskMap			=	A_DskMap;
		GrDumyZeroMem( m_DiskTbl, sizeof(m_DiskTbl) );
		m_DiskCnt			=	0;
		m_GrpType			=	A_GroupType;

		m_IsOvwtMode	=	A_IsOvrtMode;

		// read
		m_IsReadInTimeMap	=	FALSE;

		GrDumyZeroMem(m_RecChTbl, sizeof(m_RecChTbl));
		for( Tv_WkIdx = 0; Tv_WkIdx < E_GrDvrFs6MaxChCnt; Tv_WkIdx++)
		{
			m_RecChTbl[Tv_WkIdx].SeqId		=	E_GrDvrFs6SeqIdNone;
			m_RecChTbl[Tv_WkIdx].DskIdx		=	-1;
			m_RecChTbl[Tv_WkIdx].BlkIdx		=	-1;
		}

		m_DiskSid			=	V_GrDvrFs6Env.DiskBaseId | ((__u64)A_GroupType << E_GrDvrFs6DiskIdGrpTypePos);

		// build disk list
		LcGrpDskTblBuild();

		GroupInfoGet(&Tv_Total, &Tv_RecSize);

}
//--------------------------------------------------------------------
Cls_GrDvrFs6Grp::~Cls_GrDvrFs6Grp()
{
	// local -------------------
		__u8	Tv_ChIdx;
	// code --------------------
		
		for( Tv_ChIdx = 0; Tv_ChIdx < E_GrDvrFs6MaxChCnt; Tv_ChIdx++)
		{
			LcGrpRecBlkClose(Tv_ChIdx,FALSE, FALSE);
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::LcGrpChkBlkSeqOvlp(__u32 A_DskIdx, __u32 A_BlkIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Ch;
		__u32	Tv_SeqId;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		__u32	Tv_DskIdx;
		Cls_GrDvrFs6Disk*	Tv_ObjDsk;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result	=	FALSE;

		// get now sequence id
		Tv_ObjDsk	=	(Cls_GrDvrFs6Disk*)m_DiskTbl[A_DskIdx];
		if(NULL != Tv_ObjDsk)
		{
			// check status
			if(E_GrDvrRecStatEmpty <= Tv_ObjDsk->DiskStatGet())
			{
				// check block count
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)Tv_ObjDsk->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrDb)
				{
					if(A_BlkIdx < Tv_BlkCnt)
					{
						// check block status
						if((E_GrDvrFs6Fcc == Tv_PtrDb[A_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[A_BlkIdx].BlkType))
						{
							// get info
							Tv_Ch				=	Tv_PtrDb[A_BlkIdx].Ch;
							Tv_SeqId		=	Tv_PtrDb[A_BlkIdx].SeqId;
							if(E_GrDvrFs6SeqIdMin <= Tv_SeqId)
							{
								// do overlapped start
								Tv_Result	=	TRUE;

								Tv_DskIdx	=	A_DskIdx;
								Tv_BlkIdx	=	A_BlkIdx + 1;
								while(Tv_DskIdx < m_DiskCnt)
								{
									// get disk
									Tv_ObjDsk	=	(Cls_GrDvrFs6Disk*)m_DiskTbl[Tv_DskIdx];
									if(NULL != Tv_ObjDsk)
									{
										// check status
										if(E_GrDvrRecStatEmpty <= Tv_ObjDsk->DiskStatGet())
										{
											// check block count
											Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)Tv_ObjDsk->BlkHeadTableGet(&Tv_BlkCnt);
											if(NULL != Tv_PtrDb)
											{
												while(Tv_BlkIdx < Tv_BlkCnt)
												{
													// check equal
													if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType))
													{
														if((Tv_Ch == Tv_PtrDb[Tv_BlkIdx].Ch) && (Tv_SeqId == Tv_PtrDb[Tv_BlkIdx].SeqId))
														{
															// found
															Tv_Result	=	FALSE;
															break;
														}
													}
													// next
													Tv_BlkIdx ++;
												}	//	while(Tv_BlkIdx < Tv_BlkCnt)
											}
										}
									}

									// check equal sequence id found
									if(!Tv_Result)
									{
										break;
									}

									// next
									Tv_DskIdx ++;
									Tv_BlkIdx	=	E_GrDvrFs6BlkIdxDb;
								}	//	while(Tv_DskIdx < m_DiskCnt)

							}
						}
					}
				}
			}
		}	//	if(NULL != Tv_ObjDsk)

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Grp::LcGrpDskTblBuild(void)
{
	// local -------------------
		__u8	Tv_Mask;
		__u8	Tv_WkIdx;

		BOOL8	Tv_IsSeqRebld;
		BOOL8	Tv_IsSeqRefresh;

		__u32	Tv_DskIdx;
		__u32	Tv_BlkIdx;
		__u32	Tv_BlkCnt;

		//__u8	Tv_SubIdx;
		//__u32	Tv_RecId;
		Cls_GrDvrFs6Disk*	Tv_ObjDsk;
		Ptr_GrDvrFs6BlkMibHead	Tv_PtrMib;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;

		St_GrDvrFs6BlkFixInfo	Tv_BiTbl[E_GrDvrFs6MaxChCnt];

	// code --------------------
		// init
		m_GrpStat		=	E_GrDvrRecStatNotexist;
		Tv_IsSeqRebld	=	FALSE;
		Tv_IsSeqRefresh	=	FALSE;

		// reset record channel
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrDvrFs6MaxChCnt; Tv_WkIdx++)
		{
			Tv_BiTbl[Tv_WkIdx].DskIdx	=	-1;
			Tv_BiTbl[Tv_WkIdx].BlkIdx	=	-1;
			Tv_BiTbl[Tv_WkIdx].OvwtId	=	0;
			Tv_BiTbl[Tv_WkIdx].SeqId	=	0;
		}

		// build disk object table
		m_DiskCnt		=	0;
		Tv_Mask			=	1;
		for(Tv_DskIdx=0; Tv_DskIdx < E_GrDvrFs6MaxDiskCnt; Tv_DskIdx++)
		{
			if(0 != (Tv_Mask & m_DiskMap))
			{
				// add disk
				Tv_ObjDsk	=	(Cls_GrDvrFs6Disk*)V_GrDvrFs6Env.DskTbl[Tv_DskIdx];
				if(NULL != Tv_ObjDsk)
				{
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
		}

		// check able
		if(0 != m_DiskCnt)
		{
			// disk system id check
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				Tv_PtrMib	=	(Ptr_GrDvrFs6BlkMibHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrMib)
				{
					if((E_GrDvrFs6Fcc == Tv_PtrMib[E_GrDvrFs6BlkIdxMib].Fcc) && (E_GrDvrFs6BlkTypeMib == Tv_PtrMib[E_GrDvrFs6BlkIdxMib].BlkType))
					{
						if(m_DiskSid != Tv_PtrMib[E_GrDvrFs6BlkIdxMib].DiskSid)
						{
							// notify
							DbgMsgPrint("Cls_GrDvrFs6Grp::LcDiskTblBuild - Disk %d has system id %08X%08X with %08X%08X missmatch.\n",
								Tv_ObjDsk->DevIdxGet(), (__u32)((m_DiskSid >> 32) & 0xFFFFFFFFLL), (__u32)(m_DiskSid & 0xFFFFFFFFLL),
								(__u32)((Tv_PtrMib[E_GrDvrFs6BlkIdxMib].DiskSid >> 32) & 0xFFFFFFFFLL), (__u32)(Tv_PtrMib[E_GrDvrFs6BlkIdxMib].DiskSid & 0xFFFFFFFFLL));
							// message broadcast - system error
							GrTaskSvrMsgErrorNotify(E_GrTaskSysErrRaidDiskMis, m_GrpType);
							// fix disk id
							Tv_PtrMib[E_GrDvrFs6BlkIdxMib].DiskSid	=	m_DiskSid;
							// write mib
							m_DiskTbl[Tv_DskIdx]->BlkWriteHeadByIdx(E_GrTaskSysErrRaidDiskMis, TRUE);
						}
					}
				}
			}

			// correct
			while(TRUE)
			{
				Tv_IsSeqRefresh	=	FALSE;
				Tv_IsSeqRebld		=	FALSE;
				// build record info
				for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
				{
					Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
					if(NULL != Tv_PtrDb)
					{
						for(Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
						{
							if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType))
							{
								if(E_GrDvrFs6SeqIdTolerance <= Tv_PtrDb[Tv_BlkIdx].SeqId)
								{
									// need rebuild
									Tv_IsSeqRefresh	=	TRUE;
								}

								// update channel record info
								if(E_GrDvrFs6SeqIdMin <= Tv_PtrDb[Tv_BlkIdx].SeqId)
								{
									// check overlapped
									if(!LcGrpChkBlkSeqOvlp(Tv_DskIdx, Tv_BlkIdx))
									{
										DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpDskTblBuild - disk %d sequence id overlapped. rebuild!\n",
											m_DiskTbl[Tv_DskIdx]->DevIdxGet());
										// system error message
										GrTaskSvrMsgErrorNotify(E_GrTaskSysErrRaidBroken, m_GrpType);
										// overlapped id found
										Tv_IsSeqRebld	=	TRUE;
										break;
									}

									// update
									if(Tv_BiTbl[Tv_PtrDb[Tv_BlkIdx].Ch].SeqId < Tv_PtrDb[Tv_BlkIdx].SeqId)
									{
										// update
										Tv_BiTbl[Tv_PtrDb[Tv_BlkIdx].Ch].DskIdx	=	(__s32)Tv_DskIdx;
										Tv_BiTbl[Tv_PtrDb[Tv_BlkIdx].Ch].BlkIdx	=	(__s32)Tv_BlkIdx;
										Tv_BiTbl[Tv_PtrDb[Tv_BlkIdx].Ch].SeqId	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
										Tv_BiTbl[Tv_PtrDb[Tv_BlkIdx].Ch].OvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;
									}
								}
							}
						}	//	for(Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)

						if(Tv_IsSeqRebld)
						{
							break;
						}
					}
				}		//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)	

				// rebuild sequence id
				if(Tv_IsSeqRebld)
				{
					LcGrpSeqIdRebuild();
				}
				else if(Tv_IsSeqRefresh)
				{
					DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpDskTblBuild - group %d sequence id arrived tollerence. rebuild!\n",
						m_GrpType);
					LcGrpSeqIdRefresh();
				}
				else
				{
					// ok, open
					for(Tv_WkIdx = 0; Tv_WkIdx < E_GrDvrFs6MaxChCnt; Tv_WkIdx++)
					{
						if((0 <= Tv_BiTbl[Tv_WkIdx].DskIdx) && (0 <= Tv_BiTbl[Tv_WkIdx].BlkIdx))
						{
							LcGrpRecBlkOpen(Tv_WkIdx, Tv_BiTbl[Tv_WkIdx].DskIdx, Tv_BiTbl[Tv_WkIdx].BlkIdx, FALSE);
						}
					}

					break;
				}
			}	//	while(TRUE)

		}
		
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs6Grp::LcGrpCalcBlkSeqId(__u32 A_DskIdx, __u32 A_BlkIdx)
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_Ch;
		__u32	Tv_Time;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		__u32	Tv_DskIdx;
		Cls_GrDvrFs6Disk*	Tv_ObjDsk;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result	=	0;

		// get now sequence id
		Tv_ObjDsk	=	(Cls_GrDvrFs6Disk*)m_DiskTbl[A_DskIdx];
		if(NULL != Tv_ObjDsk)
		{
			// check status
			if(E_GrDvrRecStatEmpty <= Tv_ObjDsk->DiskStatGet())
			{
				// check block count
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)Tv_ObjDsk->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrDb)
				{
					if(A_BlkIdx < Tv_BlkCnt)
					{
						// check block status
						if((E_GrDvrFs6Fcc == Tv_PtrDb[A_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[A_BlkIdx].BlkType))
						{
							// get info
							Tv_Ch				=	Tv_PtrDb[A_BlkIdx].Ch;
							Tv_Time			=	Tv_PtrDb[A_BlkIdx].TimeCreate;
							if(0 != Tv_Time)
							{
								// init
								Tv_Result ++;

								for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
								{
									Tv_ObjDsk	=	(Cls_GrDvrFs6Disk*)m_DiskTbl[Tv_DskIdx];
									// check status
									if(E_GrDvrRecStatEmpty <= Tv_ObjDsk->DiskStatGet())
									{
										// check block count
										Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)Tv_ObjDsk->BlkHeadTableGet(&Tv_BlkCnt);
										if(NULL != Tv_PtrDb)
										{
											for( Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
											{
												// check self
												if((Tv_DskIdx != A_DskIdx) || (Tv_BlkIdx != A_BlkIdx))
												{
													// check status
													if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType))
													{
														if((Tv_Ch == Tv_PtrDb[Tv_BlkIdx].Ch) && (0 != Tv_PtrDb[Tv_BlkIdx].TimeCreate))
														{
															if(Tv_Time > Tv_PtrDb[Tv_BlkIdx].TimeCreate)
															{
																Tv_Result ++;
															}
															else if(Tv_Time == Tv_PtrDb[Tv_BlkIdx].TimeCreate)
															{
																if((Tv_DskIdx < A_DskIdx) || ((Tv_DskIdx == A_DskIdx) && (Tv_BlkIdx < A_BlkIdx)))
																{
																	Tv_Result ++;
																}
															}
														}

													}
												}
											}	//	for( Tv_BlkIdx = 0; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
										}
									}
								}	//	for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)

							}
						}
					}
				}
			}
		}	//	if(NULL != Tv_ObjDsk)

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Grp::LcGrpSeqIdRebuild(void)
{
	// local -------------------
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		if(0 != m_DiskCnt)
		{

			// calculate seq id
			for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrDb)
				{
					for( Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					{
						if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType))
						{
							// check exist data
							if(0 != Tv_PtrDb[Tv_BlkIdx].TimeCreate)
							{
								// get sequence id
								Tv_PtrDb[Tv_BlkIdx].SeqId	=	LcGrpCalcBlkSeqId(Tv_DskIdx, Tv_BlkIdx);
							}
							else
							{
								Tv_PtrDb[Tv_BlkIdx].SeqId	=	0;
							}
						}
					}
				}
			}

			// update last sequence id and write block head
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				m_DiskTbl[Tv_DskIdx]->BlkWriteHeadAll(FALSE);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Grp::LcGrpSeqIdRefresh(void)
{
	// local -------------------
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		if(0 != m_DiskCnt)
		{
			// calculate seq id
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrDb)
				{
					for(Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					{
						if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType))
						{
							// check exist data
							if(0 != Tv_PtrDb[Tv_BlkIdx].TimeCreate)
							{
								// get sequence id
								Tv_PtrDb[Tv_BlkIdx].SeqId	=	LcGrpCalcBlkSeqId(Tv_DskIdx, Tv_BlkIdx);
							}
						}
					}
				}
			}

			// update last sequence id and write block head
			for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				m_DiskTbl[Tv_DskIdx]->BlkWriteHeadAll(FALSE);
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::LcGrpRecAddFrm(__u8 A_Ch, void* A_PtrFrm, __u32 A_Size, __u32* A_PtrRtOfs)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ErrCode;
		BOOL8	Tv_IsFull;
		BOOL8	Tv_IsBad;
		__s32	Tv_LastDsk;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		//__u32	Tv_BlkCnt;
	// code --------------------
		Tv_Result	=	E_GrErrDiskStatBad;

		Tv_LastDsk	=	m_RecChTbl[A_Ch].DskIdx;	// get last disk

		while(E_GrDvrRecStatEmpty <= m_GrpStat)
		{
			// check opened
			if((0 <= m_RecChTbl[A_Ch].DskIdx) && (0 <= m_RecChTbl[A_Ch].BlkIdx))
			{
				// try add
				Tv_ErrCode	=	m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->BlkDbAddData((__u32)m_RecChTbl[A_Ch].BlkIdx, A_PtrFrm, A_Size, A_PtrRtOfs);
				if(E_GrErrNone == Tv_ErrCode)
				{
					// success
					m_RecChTbl[A_Ch].PtrDb->InSize	=	m_RecChTbl[A_Ch].PtrDb->InSize + A_Size;
					m_RecChTbl[A_Ch].IsDbChg	=	TRUE;

					Tv_Result	=	E_GrErrNone;
					break;
				}

				// close block
				Tv_IsFull		=	FALSE;
				Tv_IsBad		=	FALSE;
				if(E_GrErrBadIo == Tv_ErrCode)
				{
					Tv_IsBad		=	TRUE;
				}
				if(E_GrErrDiskFull == Tv_ErrCode)
				{
					Tv_IsFull		=	TRUE;
				}
				LcGrpRecBlkClose(A_Ch, Tv_IsFull, Tv_IsBad);
			}

			// open empty block now disk
			if(0 <= Tv_LastDsk)
			{
				Tv_BlkIdx	=	LcDskBlkFindEmpty((__u32)Tv_LastDsk);
				if(0 <= Tv_BlkIdx)
				{
					// open record block
					LcGrpRecBlkOpen(A_Ch, (__u32)Tv_LastDsk, (__u32)Tv_BlkIdx, TRUE);
				}
			}

			// check opened
			if((0 > m_RecChTbl[A_Ch].DskIdx) || (0 > m_RecChTbl[A_Ch].BlkIdx))
			{
				// open empty block all disk
				if(LcGrpBlkFindEmpty(&Tv_DskIdx,&Tv_BlkIdx))
				{
					// open record block
					LcGrpRecBlkOpen(A_Ch, (__u32)Tv_DskIdx, (__u32)Tv_BlkIdx, TRUE);
				}
				// reset disk index
				Tv_LastDsk	=	-1;
			}

			// check overwrite mode
			if((0 > m_RecChTbl[A_Ch].DskIdx) || (0 > m_RecChTbl[A_Ch].BlkIdx))
			{
				if(m_IsOvwtMode)
				{
					// find oldest block
					if(LcGrpBlkFindOldest(&Tv_DskIdx,&Tv_BlkIdx))
					{
						// open
						LcGrpRecBlkOpen(A_Ch,(__u32)Tv_DskIdx, (__u32)Tv_BlkIdx, TRUE);
					}
					else
					{
						// block not exist
						m_GrpStat	=	E_GrDvrRecStatBad;
						Tv_Result	=	E_GrErrDiskStatBad;
						break;
					}
				}
				else
				{
					// disk full
					m_GrpStat	=	E_GrDvrRecStatFull;

					// TODO : check all disk bad

					Tv_Result	=	E_GrErrDiskFull;
					break;
				}
			}

		}	//	while(!Tv_Result)

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::FrmVdoAdd(Ptr_GrDvrFrmHdVdo A_PtrFrm)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_FrmSize;
		__u32	Tv_BkSize;
		__u32	Tv_Ofs;
	// code --------------------
		Tv_Result		=	FALSE;

		Tv_FrmSize	=	(A_PtrFrm->Base.DataSize + sizeof(St_GrDvrFrmHdVdo)+ sizeof(St_GrDvrFrmTail)+ E_GrDvrFrmAlignSize - 1) & E_GrDvrFrmAlignMask;

		// check channel
		if(E_GrDvrMaxChCnt > A_PtrFrm->Base.Ch)
		{
			// add frame
			if(E_GrErrNone == LcGrpRecAddFrm(A_PtrFrm->Base.Ch, A_PtrFrm, Tv_FrmSize, &Tv_Ofs))
			{
				// update block time range
				LcGrpRecAddTimeToBlk(A_PtrFrm->Base.Ch ,A_PtrFrm->Base.TimeSec, A_PtrFrm->Base.IsSm);

				// add time min
				Tv_BkSize	=	A_PtrFrm->Base.DataSize + V_GrDvrFs6Env.BkupHdSize;
				LcGrpRecAddTime(A_PtrFrm->Base.Ch, A_PtrFrm->Base.TimeSec, A_PtrFrm->Base.IsSm, Tv_Ofs, A_PtrFrm->Base.Ratb, Tv_BkSize);

				// add time day
				LcGrpRecAddDay(A_PtrFrm->Base.Ch, A_PtrFrm->Base.TimeSec);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrDvrFs6Grp::FrmVdoAdd - bad channel %d input!\n", A_PtrFrm->Base.Ch);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::FrmAdoAdd(Ptr_GrDvrFrmHdAdo A_PtrFrm)
{
	// local -------------------
		BOOL8	Tv_Result;
		//__u8	Tv_ErrCode;
		__u8	Tv_Ch;
		__u32	Tv_FrmSize;
		__u32	Tv_BkSize;
		__u32	Tv_Ofs;
	// code --------------------
		Tv_Result	=	FALSE;

		if(E_GrDvrMaxChCnt > A_PtrFrm->Base.Ch)
		{
			Tv_Ch				=	E_GrDvrFs6AdoChBase + A_PtrFrm->Base.Ch;
			Tv_FrmSize	=	(A_PtrFrm->Base.DataSize + sizeof(St_GrDvrFrmHdAdo)+ sizeof(St_GrDvrFrmTail)+ E_GrDvrFrmAlignSize - 1) & E_GrDvrFrmAlignMask;

			// add frame
			if(E_GrErrNone == LcGrpRecAddFrm(Tv_Ch, A_PtrFrm, Tv_FrmSize, &Tv_Ofs))
			{
				// update block time range
				LcGrpRecAddTimeToBlk(Tv_Ch, A_PtrFrm->Base.TimeSec, A_PtrFrm->Base.IsSm);

				// add time min
				Tv_BkSize	=	A_PtrFrm->Base.DataSize + V_GrDvrFs6Env.BkupHdSize;
				LcGrpRecAddTime(Tv_Ch, A_PtrFrm->Base.TimeSec, A_PtrFrm->Base.IsSm, Tv_Ofs, A_PtrFrm->Base.Ratb, Tv_BkSize);

				// add time day
				LcGrpRecAddDay(Tv_Ch, A_PtrFrm->Base.TimeSec);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::LogAdd(Ptr_GrDvrLogItm A_PtrItem)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ErrCode;
		BOOL8	Tv_IsCreate;
		__u32	Tv_Ofs;
		__u32	Tv_InPos;
		__u64	Tv_TotSize;
		__u64	Tv_RecSize;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		// check open record block
#if 0
		if((0 <= m_RecChTbl[A_Ch].DskIdx) && (0 <= m_RecChTbl[A_Ch].BlkIdx))
		{
			Tv_Result		=	E_GrErrNone;
			Tv_IsCreate	=	TRUE;

			// check add able
			if(0 != m_RecChTbl[A_Ch].PtrDb->OfsLastLog)
			{
				// check enough space
				if(E_GrDvrFrmLogPoolItemCnt > m_RecChTbl[A_Ch].Log.Head.InCnt)
				{
					Tv_InPos	=	m_RecChTbl[A_Ch].Log.Head.InCnt;

					GrDumyCopyMem(&m_RecChTbl[A_Ch].Log.LogTbl[Tv_InPos], A_PtrItem, sizeof(St_GrDvrLogItm));

					m_RecChTbl[A_Ch].Log.Head.InCnt ++;
					m_RecChTbl[A_Ch].IsLogChg	=	TRUE;

					Tv_IsCreate	=	FALSE;
				}
			}

			// check need create log pool frame
			if(Tv_IsCreate)
			{
				// update before log pool
				if(m_RecChTbl[A_Ch].IsLogChg)
				{
					Tv_Result	=	LcGrpRecLogPoolWrite();
					m_RecChTbl[A_Ch].IsLogChg	=	FALSE;
					if(E_GrErrNone != Tv_Result)
					{
						DbgMsgPrint("Cls_GrDvrFs6Grp::LogAdd - Previous log pool write fail!\n");
					}
				}

				if(E_GrErrNone == Tv_Result)
				{
					// build new frame
					LcGrpRecLogPoolInit(m_RecChTbl[A_Ch].PtrDb->OfsLastLog);
					// add log
					GrDumyCopyMem(&m_RecChTbl[A_Ch].Log.LogTbl[0], A_PtrItem, sizeof(St_GrDvrLogItm));
					m_RecChTbl[A_Ch].Log.Head.InCnt	=	1;

					// add new frame
					Tv_ErrCode	=	m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->BlkDbAddData((__u32)m_RecChTbl[A_Ch].BlkIdx,
						&m_RecChTbl[A_Ch].Log, sizeof(St_GrDvrFrmLogPool), &Tv_Ofs);
					if(E_GrErrNone == Tv_ErrCode)
					{
						// success
						m_RecChTbl[A_Ch].PtrDb->OfsLastLog		=	Tv_Ofs;
						m_RecChTbl[A_Ch].PtrDb->InSize				=	m_RecChTbl[A_Ch].PtrDb->InSize + sizeof(St_GrDvrFrmLogPool);
						// update DB
						if(!m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->BlkWriteHeadByIdx((__u32)m_RecChTbl[A_Ch].BlkIdx))
						{
							DbgMsgPrint("Cls_GrDvrFs6Grp::RecFlush - disk %d block %d head write fail!\n",
								m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->DevIdxGet(), m_RecChTbl[A_Ch].BlkIdx);
							// mark bad block
							m_RecChTbl[A_Ch].PtrDb->BlkType		=	E_GrDvrFs6BlkTypeBad;
							// request upate disk status
							m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->StatusUpdate();
							GroupInfoGet(&Tv_TotSize, &Tv_RecSize);
						}
						m_RecChTbl[A_Ch].IsDbChg	=	FALSE;

#ifdef E_GrDvrFs6GrpDbgMsgView
						DbgMsgPrint("Cls_GrDvrFs6Grp::LogAdd - Disk %d block %d log frame (%08X) created.\n",
							m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->DevIdxGet(), m_RecChTbl[A_Ch].BlkIdx, Tv_Ofs);
#endif

						Tv_Result	=	E_GrErrNone;
					}
				}
			}

		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::LcGrpBlkFindOldest(__s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrBlk;
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
					Tv_PtrBlk	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
					if(NULL != Tv_PtrBlk)
					{
						for(Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
						{
							// check block fcc
							if((E_GrDvrFs6Fcc == Tv_PtrBlk[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrBlk[Tv_BlkIdx].BlkType))
							{
								// chekc attribute and size
								if(0 != Tv_PtrBlk[Tv_BlkIdx].SeqId)
								{
									// check found sequence id
									if(E_GrDvrFs6SeqIdNone == Tv_SeqId)
									{
										// found
										*A_PtrRtDskIdx	=	(__s32)Tv_DskIdx;
										*A_PtrRtBlkIdx	=	(__s32)Tv_BlkIdx;
										Tv_SeqId				=	Tv_PtrBlk[Tv_BlkIdx].SeqId;
										Tv_Result	=	TRUE;
									}
									else
									{
										if(Tv_SeqId > Tv_PtrBlk[Tv_BlkIdx].SeqId)
										{
											// update new sequence id
											*A_PtrRtDskIdx	=	(__s32)Tv_DskIdx;
											*A_PtrRtBlkIdx	=	(__s32)Tv_BlkIdx;
											Tv_SeqId				=	Tv_PtrBlk[Tv_BlkIdx].SeqId;
										}
									}
								}
							}
						}	//	for( Tv_BlkIdx = 0; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					}
				}

				if(Tv_Result)
				{
					break;
				}
			}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::LcGrpBlkFindNewest(__s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrBlk;
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
					Tv_PtrBlk	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
					if(NULL != Tv_PtrBlk)
					{
						for(Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
						{
							// check block fcc
							if((E_GrDvrFs6Fcc == Tv_PtrBlk[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrBlk[Tv_BlkIdx].BlkType))
							{
								// chekc attribute and size
								if(0 != Tv_PtrBlk[Tv_BlkIdx].SeqId)
								{
									// check found sequence id
									if(E_GrDvrFs6SeqIdNone == Tv_SeqId)
									{
										// found
										*A_PtrRtDskIdx	=	(__s32)Tv_DskIdx;
										*A_PtrRtBlkIdx	=	(__s32)Tv_BlkIdx;
										Tv_SeqId				=	Tv_PtrBlk[Tv_BlkIdx].SeqId;
										Tv_Result	=	TRUE;
									}
									else
									{
										if(Tv_SeqId < Tv_PtrBlk[Tv_BlkIdx].SeqId)
										{
											// update new sequence id
											*A_PtrRtDskIdx	=	(__s32)Tv_DskIdx;
											*A_PtrRtBlkIdx	=	(__s32)Tv_BlkIdx;
											Tv_SeqId				=	Tv_PtrBlk[Tv_BlkIdx].SeqId;
										}
									}
								}
							}
						}	//	for( Tv_BlkIdx = 0; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					}
				}

				if(Tv_Result)
				{
					break;
				}
			}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::LcGrpMibUpdate(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		Ptr_GrDvrFs6BlkMibHead	Tv_PtrMib;
	// code --------------------
		Tv_Result	=	FALSE;

		if(0 != m_DiskCnt)
		{
			for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				Tv_PtrMib	=	(Ptr_GrDvrFs6BlkMibHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrMib)
				{
					// get MIB
					if((E_GrDvrFs6Fcc == Tv_PtrMib[E_GrDvrFs6BlkIdxMib].Fcc) && (E_GrDvrFs6BlkTypeMib == Tv_PtrMib[E_GrDvrFs6BlkIdxMib].BlkType))
					{
						// update last record sequence
#if 0
						Tv_PtrMib[E_GrDvrFs6BlkIdxMib].SeqLast	=	m_RecChTbl[A_Ch].SeqId;
						Tv_PtrMib[E_GrDvrFs6BlkIdxMib].SeqOvwt	=	m_RecChTbl[A_Ch].OvwtId;
#endif

						// write
						if(m_DiskTbl[Tv_DskIdx]->BlkWriteHeadByIdx(E_GrDvrFs6BlkIdxMib))
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
__s32	Cls_GrDvrFs6Grp::LcDskBlkFindEmpty(__u32 A_DskIdx)
{
	// local -------------------
		__s32	Tv_Result;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result		=	-1;

		if(m_DiskCnt > A_DskIdx)
		{
			Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[A_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
			if(NULL != Tv_PtrDb)
			{
				for( Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
				{
					// check block type
					if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType))
					{
						if((0 == (E_GrDvrFs6BlkAtbFull & Tv_PtrDb[Tv_BlkIdx].BlkAtb)) && (0 == Tv_PtrDb[Tv_BlkIdx].InSize))
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
BOOL8	Cls_GrDvrFs6Grp::LcGrpBlkFindEmpty(__s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx)
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
BOOL8	Cls_GrDvrFs6Grp::LcGrpRecBlkClose(__u8 A_Ch, BOOL8 A_IsFull, BOOL8 A_IsBad)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsDskStat;
		__u8	Tv_ErrCode;
		
		__u64	Tv_TotSize;
		__u64	Tv_RecSize;
	// code --------------------
		Tv_Result	=	FALSE;

		// check opened
		if((0 <= m_RecChTbl[A_Ch].DskIdx) && (0 <= m_RecChTbl[A_Ch].BlkIdx))
		{
			// init
			Tv_IsDskStat	=	FALSE;

			if(!A_IsBad)
			{
				// update day map
				if(m_RecChTbl[A_Ch].IsDayMapChg)
				{
					// write
					if(0 != m_RecChTbl[A_Ch].PtrDb->OfsLastDate)
					{
						Tv_ErrCode	=	m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->BlkWriteDataByOfs((__u32)m_RecChTbl[A_Ch].BlkIdx, m_RecChTbl[A_Ch].PtrDb->OfsLastDate,
							&m_RecChTbl[A_Ch].DayMap, sizeof(St_GrDvrFs6FrmDayMap));
						if(E_GrErrBadIo == Tv_ErrCode)
						{
							DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecBlkClose - Disk %d block %d day map write fail!.\n");
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecBlkClose - day map modified, but OfsLastDate not exist.\n");
					}
				}

				// update min map
				if(m_RecChTbl[A_Ch].IsTimeMapChg)
				{
					LcGrpRecTimeMapWrite(A_Ch);
				}

				if(A_IsFull)
				{
					m_RecChTbl[A_Ch].PtrDb->BlkAtb	=	m_RecChTbl[A_Ch].PtrDb->BlkAtb | E_GrDvrFs6BlkAtbFull;
					m_RecChTbl[A_Ch].IsDbChg	=	TRUE;
				}
			}

			// bad block
			if(A_IsBad)
			{
				m_RecChTbl[A_Ch].PtrDb->BlkType		=	E_GrDvrFs6BlkTypeBad;
				m_RecChTbl[A_Ch].IsDbChg	=	TRUE;
				Tv_IsDskStat	=	TRUE;
			}
			
			// update DB
			if(m_RecChTbl[A_Ch].IsDbChg)
			{
				// update DB
				if(!m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->BlkWriteHeadByIdx((__u32)m_RecChTbl[A_Ch].BlkIdx))
				{
					DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecBlkClose - disk %d block %d head write fail!\n",
						m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->DevIdxGet(),m_RecChTbl[A_Ch].BlkIdx);
					// mark bad block
					m_RecChTbl[A_Ch].PtrDb->BlkType		=	E_GrDvrFs6BlkTypeBad;
					// request upate disk status
					Tv_IsDskStat	=	TRUE;
					
				}
			}

			// update disk status
			if(Tv_IsDskStat)
			{
				m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->StatusUpdate();
				GroupInfoGet(&Tv_TotSize, &Tv_RecSize);
			}

			// reset
			GrDumyZeroMem(&m_RecChTbl[A_Ch], sizeof(m_RecChTbl[A_Ch]));
			m_RecChTbl[A_Ch].DskIdx	=	-1;
			m_RecChTbl[A_Ch].BlkIdx	=	-1;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::LcGrpRecBlkOpen(__u8 A_Ch, __u32 A_DskIdx, __u32 A_BlkIdx, BOOL8 A_IsNew)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BlkCnt;
		__u32	Tv_BkSeqId;
		__u32	Tv_BkSeqOvwt;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result	=	FALSE;

		// check already open
		if((0 > m_RecChTbl[A_Ch].DskIdx) || (0 > m_RecChTbl[A_Ch].BlkIdx))
		{
			// check disk index
			if((__u32)m_DiskCnt > A_DskIdx)
			{
				// get block info
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[A_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrDb)
				{
					// check able block
					if(Tv_BlkCnt > A_BlkIdx)
					{
						// check block status
						if((E_GrDvrFs6Fcc == Tv_PtrDb[A_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[A_BlkIdx].BlkType))
						{
							// try new block open
							if(A_IsNew)
							{
								// new block open
								Tv_BkSeqId		=	m_RecChTbl[A_Ch].SeqId;
								Tv_BkSeqOvwt	=	m_RecChTbl[A_Ch].OvwtId;
								m_RecChTbl[A_Ch].SeqId ++;
								
								// build new sequence id
								Tv_PtrDb[A_BlkIdx].Ch						=	A_Ch;
								Tv_PtrDb[A_BlkIdx].BlkAtb				=	0;
								Tv_PtrDb[A_BlkIdx].InSize				=	0;
								Tv_PtrDb[A_BlkIdx].OfsLastDate	=	0;
								Tv_PtrDb[A_BlkIdx].OfsLastTime	=	0;
								Tv_PtrDb[A_BlkIdx].OvwtId ++;
								Tv_PtrDb[A_BlkIdx].SeqId				=	m_RecChTbl[A_Ch].SeqId;
								Tv_PtrDb[A_BlkIdx].TimeRng.TimeSt	=	0;
								Tv_PtrDb[A_BlkIdx].TimeRng.TimeEd	=	0;
								Tv_PtrDb[A_BlkIdx].TimeCreate		=	0;

								Tv_PtrDb[A_BlkIdx]._rsvd1[0]		=	0;
								Tv_PtrDb[A_BlkIdx]._rsvd1[1]		=	0;
								Tv_PtrDb[A_BlkIdx]._rsvd1[2]		=	0;
								Tv_PtrDb[A_BlkIdx]._rsvd1[3]		=	0;
								Tv_PtrDb[A_BlkIdx]._rsvd1[4]		=	0;
								Tv_PtrDb[A_BlkIdx]._rsvd1[5]		=	0;
								Tv_PtrDb[A_BlkIdx]._rsvd1[6]		=	0;

								// try write
								if(m_DiskTbl[A_DskIdx]->BlkWriteHeadByIdx(A_BlkIdx))
								{
									Tv_Result	=	TRUE;
#ifdef E_GrDvrFs6GrpDbgMsgView
									DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecBlkOpen - block (disk:%d,index:%d,seq id:%d) new allocated.\n",
										m_DiskTbl[A_DskIdx]->DevIdxGet(), A_BlkIdx,m_RecChTbl[A_Ch].SeqId);
#endif
									// update record sequence id
									//LcGrpMibUpdate(); - not need
								}
								else
								{
									// error
									Tv_PtrDb[A_BlkIdx].BlkType	=	E_GrDvrFs6BlkTypeBad;	// mark bad
									// update
									m_DiskTbl[A_DskIdx]->StatusUpdate();
									// restore sequence id
									m_RecChTbl[A_Ch].SeqId		=	Tv_BkSeqId;
									m_RecChTbl[A_Ch].OvwtId		=	Tv_BkSeqOvwt;
								}
							}
							else
							{
								// normal open
								// read day map
								if(0 != Tv_PtrDb[A_BlkIdx].OfsLastDate)
								{
									if(E_GrErrNone != m_DiskTbl[A_DskIdx]->BlkReadDataByOfs(A_BlkIdx, Tv_PtrDb[A_BlkIdx].OfsLastDate, 
										&m_RecChTbl[A_Ch].DayMap, sizeof(St_GrDvrFs6FrmDayMap)))
									{
										// set bad block
										Tv_PtrDb[A_BlkIdx].BlkType	=	E_GrDvrFs6BlkTypeBad;
									}
								}
								// read min map
								if((0 != Tv_PtrDb[A_BlkIdx].OfsLastTime) && (E_GrDvrFs6BlkTypeBad != Tv_PtrDb[A_BlkIdx].BlkType))
								{
									if(E_GrErrNone != m_DiskTbl[A_DskIdx]->BlkReadDataByOfs(A_BlkIdx, Tv_PtrDb[A_BlkIdx].OfsLastTime,
										&m_RecChTbl[A_Ch].TimeMap, sizeof(St_GrDvrFs6FrmTimeMap)))
									{
										// set bad block
										Tv_PtrDb[A_BlkIdx].BlkType	=	E_GrDvrFs6BlkTypeBad;
									}
								}
								// check block bad
								if(E_GrDvrFs6BlkTypeBad != Tv_PtrDb[A_BlkIdx].BlkType)
								{
									// success
									Tv_Result	=	TRUE;
								}
								else
								{
									// bad block
									m_DiskTbl[A_DskIdx]->BlkWriteHeadByIdx(A_BlkIdx);
									m_DiskTbl[A_DskIdx]->StatusUpdate();
#ifdef E_GrDvrFs6GrpDbgMsgView
									DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecBlkOpen - bad block (disk:%d,index:%d) appered.\n",
										m_DiskTbl[A_DskIdx]->DevIdxGet(), A_BlkIdx);
#endif
								}
							}

							// update 
							if(Tv_Result)
							{
								m_RecChTbl[A_Ch].PtrDb		=	&Tv_PtrDb[A_BlkIdx];
								m_RecChTbl[A_Ch].IsDbChg			=	FALSE;
								m_RecChTbl[A_Ch].IsDayMapChg	=	FALSE;
								m_RecChTbl[A_Ch].IsTimeMapChg	=	FALSE;

								m_RecChTbl[A_Ch].DskIdx	=	(__s32)A_DskIdx;
								m_RecChTbl[A_Ch].BlkIdx	=	(__s32)A_BlkIdx;
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecBlkOpen - block (disk:%d,index:%d) status is bad!\n",
								m_DiskTbl[A_DskIdx]->DevIdxGet(),A_BlkIdx);
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecBlkOpen - Bad block index %d (Disk: %d)!\n", 
							A_BlkIdx, m_DiskTbl[A_DskIdx]->DevIdxGet());
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecBlkOpen - Disk %d can not open!\n", m_DiskTbl[A_DskIdx]->DevIdxGet());
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Grp::LcGrpRecTimeMapInit(__u8 A_Ch, __u32 A_OfsPrv)
{
	// local -------------------
	// code --------------------
		m_RecChTbl[A_Ch].TimeMap.Head.Base.Fcc			=	E_GrDvrFrmHeadFcc;
		m_RecChTbl[A_Ch].TimeMap.Head.Base.Ch				=	0xFF;
		m_RecChTbl[A_Ch].TimeMap.Head.Base.DataSize	=	sizeof(St_GrDvrFs6TimeMapFrmItm)* E_GrDvrFs6FrmTimeMapItemCnt;
		m_RecChTbl[A_Ch].TimeMap.Head.Base.IsSm			=	0;	// not use
		m_RecChTbl[A_Ch].TimeMap.Head.Base.TimeSec	=	0;	// not use
		m_RecChTbl[A_Ch].TimeMap.Head.Base.TimeMsec	=	0;
		m_RecChTbl[A_Ch].TimeMap.Head.Base.Ratb			=	0;
		m_RecChTbl[A_Ch].TimeMap.Head.Base.Type			=	E_GrDvrFrmTypeTimeMap;
		m_RecChTbl[A_Ch].TimeMap.Head.InCnt					=	0;
		m_RecChTbl[A_Ch].TimeMap.Head.OfsPrv				=	A_OfsPrv;
		m_RecChTbl[A_Ch].TimeMap.Head._rsvd0				=	0;

		GrDumyZeroMem(m_RecChTbl[A_Ch].TimeMap.TimeTbl, sizeof(St_GrDvrFs6TimeMapFrmItm)* E_GrDvrFs6FrmTimeMapItemCnt);

		m_RecChTbl[A_Ch].TimeMap.Tail.Fcc						=	E_GrDvrFrmTailFcc;
		m_RecChTbl[A_Ch].TimeMap.Tail.PktSize				=	sizeof(St_GrDvrFs6FrmTimeMap);

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Grp::LcGrpRecDayMapInit(__u8 A_Ch, __u32 A_OfsPrv)
{
	// local -------------------
	// code --------------------
		m_RecChTbl[A_Ch].DayMap.Head.Base.Fcc				=	E_GrDvrFrmHeadFcc;
		m_RecChTbl[A_Ch].DayMap.Head.Base.Ch				=	0xFF;
		m_RecChTbl[A_Ch].DayMap.Head.Base.DataSize	=	2 * E_GrDvrFs6FrmDayMapItemCnt;
		m_RecChTbl[A_Ch].DayMap.Head.Base.IsSm			=	0;	// not use
		m_RecChTbl[A_Ch].DayMap.Head.Base.TimeSec		=	0;	// not use
		m_RecChTbl[A_Ch].DayMap.Head.Base.TimeMsec	=	0;
		m_RecChTbl[A_Ch].DayMap.Head.Base.Ratb			=	0;
		m_RecChTbl[A_Ch].DayMap.Head.Base.Type			=	E_GrDvrFrmTypeDayMap;
		m_RecChTbl[A_Ch].DayMap.Head.InCnt					=	0;
		m_RecChTbl[A_Ch].DayMap.Head.OfsPrv					=	A_OfsPrv;
		m_RecChTbl[A_Ch].DayMap.Head._rsvd0					=	0;

		GrDumyZeroMem(m_RecChTbl[A_Ch].DayMap.DayTbl, 2 * E_GrDvrFs6FrmDayMapItemCnt);

		m_RecChTbl[A_Ch].DayMap.Tail.Fcc						=	E_GrDvrFrmTailFcc;
		m_RecChTbl[A_Ch].DayMap.Tail.PktSize				=	sizeof(St_GrDvrFs6FrmDayMap);

}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::LcGrpRecTimeMapWrite(__u8 A_Ch)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		// check opened
		if((0 <= m_RecChTbl[A_Ch].DskIdx) && (0 <= m_RecChTbl[A_Ch].BlkIdx))
		{
			// exist TimeMap
			// write
			if(0 != m_RecChTbl[A_Ch].PtrDb->OfsLastTime)
			{
				Tv_Result	=	m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->BlkWriteDataByOfs((__u32)m_RecChTbl[A_Ch].BlkIdx, m_RecChTbl[A_Ch].PtrDb->OfsLastTime,
					&m_RecChTbl[A_Ch].TimeMap, sizeof(St_GrDvrFs6FrmTimeMap));
				if(E_GrErrBadIo == Tv_Result)
				{
					DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecTimeMapWrite - Disk %d block %d min map write fail!.\n",
						m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->DevIdxGet(),m_RecChTbl[A_Ch].BlkIdx);
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecTimeMapWrite - OfsLastTime not exist.\n");
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::LcGrpRecDayMapWrite(__u8 A_Ch)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		// check opened
		if((0 <= m_RecChTbl[A_Ch].DskIdx) && (0 <= m_RecChTbl[A_Ch].BlkIdx))
		{
			// exist day map
			// write
			if(0 != m_RecChTbl[A_Ch].PtrDb->OfsLastDate)
			{
				Tv_Result	=	m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->BlkWriteDataByOfs((__u32)m_RecChTbl[A_Ch].BlkIdx, m_RecChTbl[A_Ch].PtrDb->OfsLastDate,
					&m_RecChTbl[A_Ch].DayMap, sizeof(St_GrDvrFs6FrmDayMap));
				if(E_GrErrBadIo == Tv_Result)
				{
					DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecDayMapWrite - Disk %d block %d date map write fail!.\n",
						m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->DevIdxGet(), m_RecChTbl[A_Ch].BlkIdx);
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecDayMapWrite - OfsLastDate not exist.\n");
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::LcGrpRecAddTime(__u8 A_Ch, __u32 A_Time, BOOL8 A_IsSm, __u32 A_Ofs, __u8 A_Ratb, __u32 A_BkSize)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ErrCode;
		BOOL8	Tv_IsCreate;
		BOOL8	Tv_IsAddMin;
		__u32	Tv_TimeMin;
		__u32	Tv_Ofs;
		__u32	Tv_InPos;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		// check channel
		if(E_GrDvrMaxChCnt > A_Ch)
		{
			// check open record block
			if((0 <= m_RecChTbl[A_Ch].DskIdx) && (0 <= m_RecChTbl[A_Ch].BlkIdx))
			{
				Tv_Result		=	E_GrErrNone;
				Tv_IsCreate	=	TRUE;
				Tv_TimeMin	=	A_Time - (A_Time % E_GrDvrFs6TimeMapSecUnit);
				Tv_IsAddMin	=	TRUE;

				// check add able
				if(0 != m_RecChTbl[A_Ch].PtrDb->OfsLastTime)
				{
					// compare last time
					if(0 != m_RecChTbl[A_Ch].TimeMap.Head.InCnt)
					{
						Tv_InPos	=	m_RecChTbl[A_Ch].TimeMap.Head.InCnt - 1;
						if(Tv_TimeMin == m_RecChTbl[A_Ch].TimeMap.TimeTbl[Tv_InPos].Time)
						{
							// update
							m_RecChTbl[A_Ch].TimeMap.TimeTbl[Tv_InPos].Ratb	=	m_RecChTbl[A_Ch].TimeMap.TimeTbl[Tv_InPos].Ratb | A_Ratb;
							m_RecChTbl[A_Ch].TimeMap.TimeTbl[Tv_InPos].Size	=	m_RecChTbl[A_Ch].TimeMap.TimeTbl[Tv_InPos].Size + A_BkSize;

							m_RecChTbl[A_Ch].IsTimeMapChg	=	TRUE;

							Tv_IsAddMin	=	FALSE;
							Tv_IsCreate	=	FALSE;
						}
					}

					if(Tv_IsAddMin)
					{
						// check enough space
						if(E_GrDvrFs6FrmTimeMapItemCnt > m_RecChTbl[A_Ch].TimeMap.Head.InCnt)
						{
							Tv_InPos	=	m_RecChTbl[A_Ch].TimeMap.Head.InCnt;

							m_RecChTbl[A_Ch].TimeMap.TimeTbl[Tv_InPos].Time		=	Tv_TimeMin;
							m_RecChTbl[A_Ch].TimeMap.TimeTbl[Tv_InPos].Ofs		=	A_Ofs;
							m_RecChTbl[A_Ch].TimeMap.TimeTbl[Tv_InPos].Size		=	A_BkSize;
							m_RecChTbl[A_Ch].TimeMap.TimeTbl[Tv_InPos].Ratb		=	A_Ratb;

							m_RecChTbl[A_Ch].TimeMap.Head.InCnt ++;
							m_RecChTbl[A_Ch].IsTimeMapChg	=	TRUE;
							Tv_IsCreate	=	FALSE;

							//DbgMsgPrint("[DBG] ch %d time add %d , incnt = %d\n", A_Ch, A_Time, m_RecChTbl[A_Ch].TimeMap.Head.InCnt);
						}
					}
				}

				// check need create min map frame
				if(Tv_IsCreate)
				{
					// update before TimeMap
					if(m_RecChTbl[A_Ch].IsTimeMapChg)
					{
						Tv_Result	=	LcGrpRecTimeMapWrite(A_Ch);
						m_RecChTbl[A_Ch].IsTimeMapChg	=	FALSE;
						if(E_GrErrNone != Tv_Result)
						{
							DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecAddTime - Previous min map write fail!\n");
						}
					}

					if(E_GrErrNone == Tv_Result)
					{
						// build new frame
						LcGrpRecTimeMapInit(A_Ch, m_RecChTbl[A_Ch].PtrDb->OfsLastTime);
						// add first frame
						m_RecChTbl[A_Ch].TimeMap.TimeTbl[0].Time	=	Tv_TimeMin;
						m_RecChTbl[A_Ch].TimeMap.TimeTbl[0].Ofs		=	A_Ofs;
						m_RecChTbl[A_Ch].TimeMap.TimeTbl[0].Size	=	A_BkSize;
						m_RecChTbl[A_Ch].TimeMap.TimeTbl[0].Ratb	=	A_Ratb;

						m_RecChTbl[A_Ch].TimeMap.Head.InCnt	=	1;

						// add new frame
						Tv_ErrCode	=	m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->BlkDbAddData((__u32)m_RecChTbl[A_Ch].BlkIdx,
							&m_RecChTbl[A_Ch].TimeMap, sizeof(St_GrDvrFs6FrmTimeMap), &Tv_Ofs);
						if(E_GrErrNone == Tv_ErrCode)
						{
							// check bad time block offset
							if(m_RecChTbl[A_Ch].PtrDb->OfsLastTime >= Tv_Ofs)
							{
								DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecAddTime - Disk %d block %d  bad time frame pos (%08X -> %08X).\n",
									m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->DevIdxGet(), m_RecChTbl[A_Ch].BlkIdx, m_RecChTbl[A_Ch].PtrDb->OfsLastTime, Tv_Ofs);
							}
							// success
							m_RecChTbl[A_Ch].PtrDb->OfsLastTime		=	Tv_Ofs;
							m_RecChTbl[A_Ch].PtrDb->InSize				=	m_RecChTbl[A_Ch].PtrDb->InSize + sizeof(St_GrDvrFs6FrmTimeMap);
							m_RecChTbl[A_Ch].IsDbChg	=	TRUE;

							Tv_Result	=	E_GrErrNone;

#ifdef E_GrDvrFs6GrpDbgMsgView
							DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecAddTime - Disk %d block %d time frame (%08X) created.\n",
								m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->DevIdxGet(), m_RecChTbl[A_Ch].BlkIdx, Tv_Ofs);
#endif

						}
					}
				}

			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::LcGrpDataWriteByHndl(__u64 A_Hndl, void* A_PtrData, __u32 A_Size)
{
	// local -------------------
		__u8	Tv_Result;
		__s32	Tv_DskIdx;
		__u8	Tv_OvwtId;
		__s32	Tv_BlkIdx;
		__u32	Tv_Ofs;
		__u32	Tv_BlkCnt;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		// decode handle
		GrDvrFs6HndlDec(A_Hndl, &Tv_DskIdx, &Tv_Ofs, &Tv_BlkIdx, &Tv_OvwtId);

		// check disk range
		if(m_DiskCnt > (__u32)Tv_DskIdx)
		{
			// get block info
			Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
			if(NULL != Tv_PtrDb)
			{
				// check block index
				if(Tv_BlkCnt > (__u32)Tv_BlkIdx)
				{
					// check block type
					if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeBad != Tv_PtrDb[Tv_BlkIdx].BlkType))
					{
						// check match overwrite id
						if(Tv_OvwtId == Tv_PtrDb[Tv_BlkIdx].OvwtId)
						{
							// try write
							Tv_Result	=	m_DiskTbl[Tv_DskIdx]->BlkWriteDataByOfs((__u32)Tv_BlkIdx, Tv_Ofs, A_PtrData, A_Size);
						}
						else
						{
							DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpDataWriteByHndl - bad handle (%08X%08X) expired!\n",
								(__u32)((A_Hndl >> 32) & 0xFFFFFFFF), (__u32)(A_Hndl & 0xFFFFFFFF));
							Tv_Result	=	E_GrErrBadParam;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpDataWriteByHndl - bad handle (%08X%08X) bad block operation!\n",
							(__u32)((A_Hndl >> 32) & 0xFFFFFFFF), (__u32)(A_Hndl & 0xFFFFFFFF));
						Tv_Result	=	E_GrErrDiskBadSector;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpDataWriteByHndl - bad handle (%08X%08X) block index!\n",
						(__u32)((A_Hndl >> 32) & 0xFFFFFFFF), (__u32)(A_Hndl & 0xFFFFFFFF));
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpDataWriteByHndl - bad handle (%08X%08X) disk value!\n",
				(__u32)((A_Hndl >> 32) & 0xFFFFFFFF), (__u32)(A_Hndl & 0xFFFFFFFF));
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::LcGrpRecAddDay(__u8 A_Ch, __u32 A_Time)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ErrCode;
		BOOL8	Tv_IsCreate;
		BOOL8	Tv_IsAddItm;
		__u16	Tv_Day;
		__u32	Tv_Ofs;
		__u32	Tv_InPos;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		// check open record block
		if((0 <= m_RecChTbl[A_Ch].DskIdx) && (0 <= m_RecChTbl[A_Ch].BlkIdx))
		{
			Tv_Result		=	E_GrErrNone;
			Tv_IsCreate	=	TRUE;
			Tv_Day			=	(__u16)(A_Time / E_GrTimeSecOfDay);
			Tv_IsAddItm	=	TRUE;

			// check add able
			if(0 != m_RecChTbl[A_Ch].PtrDb->OfsLastDate)
			{
				// compare last time
				if(0 != m_RecChTbl[A_Ch].DayMap.Head.InCnt)
				{
					Tv_InPos	=	m_RecChTbl[A_Ch].DayMap.Head.InCnt - 1;
					if(Tv_Day == m_RecChTbl[A_Ch].DayMap.DayTbl[Tv_InPos])
					{
						Tv_IsAddItm	=	FALSE;
						Tv_IsCreate	=	FALSE;
					}
				}

				if(Tv_IsAddItm)
				{
					// check enough space
					if(E_GrDvrFs6FrmDayMapItemCnt > m_RecChTbl[A_Ch].DayMap.Head.InCnt)
					{
						Tv_InPos	=	m_RecChTbl[A_Ch].DayMap.Head.InCnt;

						m_RecChTbl[A_Ch].DayMap.DayTbl[Tv_InPos]	=	Tv_Day;

						m_RecChTbl[A_Ch].DayMap.Head.InCnt ++;
						m_RecChTbl[A_Ch].IsDayMapChg	=	TRUE;

						Tv_IsCreate	=	FALSE;
					}
				}
			}

			// check need create day map frame
			if(Tv_IsCreate)
			{
				// update before daymap
				if(m_RecChTbl[A_Ch].IsDayMapChg)
				{
					Tv_Result	=	LcGrpRecDayMapWrite(A_Ch);
					m_RecChTbl[A_Ch].IsDayMapChg	=	FALSE;
					if(E_GrErrNone != Tv_Result)
					{
						DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecAddDay - Previous day map write fail!\n");
					}
				}

				if(E_GrErrNone == Tv_Result)
				{
					// build new frame
					LcGrpRecDayMapInit(A_Ch, m_RecChTbl[A_Ch].PtrDb->OfsLastDate);
					// add first day
					m_RecChTbl[A_Ch].DayMap.DayTbl[0]	=	Tv_Day;

					m_RecChTbl[A_Ch].DayMap.Head.InCnt	=	1;

					// add new frame
					Tv_ErrCode	=	m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->BlkDbAddData((__u32)m_RecChTbl[A_Ch].BlkIdx,
						&m_RecChTbl[A_Ch].DayMap, sizeof(St_GrDvrFs6FrmDayMap), &Tv_Ofs);
					if(E_GrErrNone == Tv_ErrCode)
					{
						// success
						m_RecChTbl[A_Ch].PtrDb->OfsLastDate		=	Tv_Ofs;
						m_RecChTbl[A_Ch].PtrDb->InSize				=	m_RecChTbl[A_Ch].PtrDb->InSize + sizeof(St_GrDvrFs6FrmDayMap);
						m_RecChTbl[A_Ch].IsDbChg	=	TRUE;

						Tv_Result	=	E_GrErrNone;

#ifdef E_GrDvrFs6GrpDbgMsgView
						DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpRecAddDay - Disk %d block %d day frame (%08X) created.\n",
							m_DiskTbl[m_RecChTbl[A_Ch].DskIdx]->DevIdxGet(), m_RecChTbl[A_Ch].BlkIdx, Tv_Ofs);
#endif

					}
				}
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::StatusGet(void)
{
		return	m_GrpStat;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::LcGrpRecAddTimeToBlk(__u8 A_Ch, __u32 A_Time, BOOL8 A_IsSm)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		// check open
		if((0 <= m_RecChTbl[A_Ch].DskIdx) && (0 <= m_RecChTbl[A_Ch].BlkIdx))
		{
			// check exist block create time
			if(0 == m_RecChTbl[A_Ch].PtrDb->TimeCreate)
			{
				m_RecChTbl[A_Ch].PtrDb->TimeCreate	=	GrTimeTimeToSysTime(A_Time, A_IsSm);
				m_RecChTbl[A_Ch].IsDbChg	=	TRUE;
			}
			// check time start
			if(0 == m_RecChTbl[A_Ch].PtrDb->TimeRng.TimeSt)
			{
				m_RecChTbl[A_Ch].PtrDb->TimeRng.TimeSt	=	A_Time;
				m_RecChTbl[A_Ch].IsDbChg	=	TRUE;
			}
			else
			{
				if(m_RecChTbl[A_Ch].PtrDb->TimeRng.TimeSt > A_Time)
				{
					m_RecChTbl[A_Ch].PtrDb->TimeRng.TimeSt	=	A_Time;
					m_RecChTbl[A_Ch].IsDbChg	=	TRUE;
				}
			}
			// check time end
			if(0 == m_RecChTbl[A_Ch].PtrDb->TimeRng.TimeEd)
			{
				m_RecChTbl[A_Ch].PtrDb->TimeRng.TimeEd	=	A_Time;
				m_RecChTbl[A_Ch].IsDbChg	=	TRUE;
			}
			else
			{
				if(m_RecChTbl[A_Ch].PtrDb->TimeRng.TimeEd < A_Time)
				{
					m_RecChTbl[A_Ch].PtrDb->TimeRng.TimeEd	=	A_Time;
					m_RecChTbl[A_Ch].IsDbChg	=	TRUE;
				}
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::LcGrpBlkFindByTimeFw(__u8 A_Ch, __u32 A_Time, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsExit;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
		__s32	Tv_FndDskIdx;
		__s32	Tv_FndBlkIdx;
		__u32	Tv_FndBlkSeq;
		__u8	Tv_FndOvwtId;
		__u32	Tv_GapFnd;
		__u32	Tv_GapNow;
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
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrDb)
				{
					// check block
					for( Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					{
						// check block type
						if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType)
							&& (A_Ch == Tv_PtrDb[Tv_BlkIdx].Ch))
						{
							// check time range
							if((0 != Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt) && (0 != Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd))
							{
								Tv_Result	=	TRUE;
								// check time range
								if((A_Time >= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt) && (A_Time <= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd))
								{
									// block found, do exit
									Tv_IsExit	=	TRUE;

									Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
									Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
									Tv_FndBlkSeq	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
									Tv_FndOvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;
									break;
								}
								else
								{
									if(0 > Tv_FndDskIdx)
									{
										Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
										Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
										Tv_FndBlkSeq	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
										Tv_FndOvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;

										if(A_Time > Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt)
										{
											Tv_GapFnd	=	A_Time - Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd;
										}
										else
										{
											Tv_GapFnd	=	Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt - A_Time + E_GrDvrFs6GrpBlkNegGap;
										}
									}
									else
									{
										if(A_Time > Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt)
										{
											Tv_GapNow	=	A_Time - Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd;
										}
										else
										{
											Tv_GapNow	=	Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt - A_Time + E_GrDvrFs6GrpBlkNegGap;
										}
										// compare
										if( Tv_GapFnd > Tv_GapNow )
										{
											// update
											Tv_GapFnd		=	Tv_GapNow;

											Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
											Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
											Tv_FndBlkSeq	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
											Tv_FndOvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;
										}
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
__u8	Cls_GrDvrFs6Grp::LcDskBlkTimeOfsFindByTimeFw(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_Time, __u32* A_PtrRtOfs)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ErrCode;
		__u32	Tv_BlkCnt;
		__u32	Tv_TimeUnit;
		__u32	Tv_FrmOfs;
		__u32	Tv_MinIdx;
		__u32	Tv_FndOfs;
		__u32	Tv_FndGap;
		__u32	Tv_NowGap;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;
		Tv_FndOfs	=	0;

		// check recursive call
		if(m_IsReadInTimeMap)
		{
			DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkTimeOfsFindByTimeFw - recursive call!\n");
			Tv_Result	=	E_GrErrBusy;
			return	Tv_Result;
		}

		m_IsReadInTimeMap	=	TRUE;

		// check disk index
		if((__u32)m_DiskCnt > A_DskIdx)
		{
			// get block info
			Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[A_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
			if(NULL != Tv_PtrDb)
			{
				if(Tv_BlkCnt > A_BlkIdx)
				{
					Tv_Result	=	E_GrErrDiskBadSector;
					// check block status
					if((E_GrDvrFs6Fcc == Tv_PtrDb[A_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[A_BlkIdx].BlkType))
					{
						Tv_FndGap		=	0;
						Tv_Result		=	E_GrErrNotFound;
						Tv_TimeUnit	=	A_Time - (A_Time % E_GrDvrFs6TimeMapSecUnit);
						Tv_FrmOfs		=	Tv_PtrDb[A_BlkIdx].OfsLastTime;

						while(0 != Tv_FrmOfs)
						{
							// load min frame
							Tv_ErrCode	=	m_DiskTbl[A_DskIdx]->BlkReadDataByOfs(A_BlkIdx, Tv_FrmOfs, &m_ReadTimeMap, sizeof(m_ReadTimeMap));
							if(E_GrErrNone != Tv_ErrCode)
							{
								Tv_Result	=	Tv_ErrCode;
								break;
							}
							// check frame type
							if((E_GrDvrFrmHeadFcc != m_ReadTimeMap.Head.Base.Fcc) || (E_GrDvrFrmTypeTimeMap != m_ReadTimeMap.Head.Base.Type))
							{
								DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkMinOfsFindByTimeFw - Disk %d block %d offset %08X Min map data broken!",
									m_DiskTbl[A_DskIdx]->DevIdxGet(), A_BlkIdx, Tv_FrmOfs);
								Tv_Result	=	E_GrErrDataBroken;
								break;
							}

							// get info
							if(Tv_FrmOfs <= m_ReadTimeMap.Head.OfsPrv)
							{
								DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkMinOfsFindByTimeFw - bad frame offset!\n");
								Tv_FrmOfs		=	0;
							}
							else
							{
								Tv_FrmOfs	=	m_ReadTimeMap.Head.OfsPrv;
							}

							// check exist item
							if(0 != m_ReadTimeMap.Head.InCnt)
							{
								// check match time
								for(Tv_MinIdx = 0; Tv_MinIdx < m_ReadTimeMap.Head.InCnt; Tv_MinIdx++)
								{
									// get
									if(0 == Tv_FndOfs)
									{
										Tv_FndOfs	=	m_ReadTimeMap.TimeTbl[Tv_MinIdx].Ofs;
										if(Tv_TimeUnit <= m_ReadTimeMap.TimeTbl[Tv_MinIdx].Time)
										{
											Tv_FndGap	=	m_ReadTimeMap.TimeTbl[Tv_MinIdx].Time - Tv_TimeUnit;
										}
										else
										{
											Tv_FndGap	=	Tv_TimeUnit - m_ReadTimeMap.TimeTbl[Tv_MinIdx].Time  + E_GrDvrFs6GrpBlkNegGap;
										}

										Tv_Result	=	E_GrErrNone;
									}
									else
									{
										// get now gap
										if(Tv_TimeUnit <= m_ReadTimeMap.TimeTbl[Tv_MinIdx].Time)
										{
											Tv_NowGap	=	m_ReadTimeMap.TimeTbl[Tv_MinIdx].Time - Tv_TimeUnit;
										}
										else
										{
											Tv_NowGap	=	Tv_TimeUnit - m_ReadTimeMap.TimeTbl[Tv_MinIdx].Time  + E_GrDvrFs6GrpBlkNegGap;
										}
										// compare gap
										if(Tv_NowGap < Tv_FndGap)
										{
											// update new item
											Tv_FndOfs	=	m_ReadTimeMap.TimeTbl[Tv_MinIdx].Ofs;
											Tv_FndGap	=	Tv_NowGap;
											Tv_Result	=	E_GrErrNone;
										}
									}

									// check finish
									if((0 != Tv_FndOfs) && (0 == Tv_FndGap))
									{
										break;
									}

								}
							}

							if((0 != Tv_FndOfs) && (0 == Tv_FndGap))
							{
								break;
							}

						}	//	while(0 != Tv_FrmOfs)

					}
				}
			}
		}

		m_IsReadInTimeMap	=	FALSE;

		*A_PtrRtOfs	=	Tv_FndOfs;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::LcGrpBlkFindBySeqFw(__u8 A_Ch, __u32 A_SeqId, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsExit;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
		__s32	Tv_FndDskIdx;
		__s32	Tv_FndBlkIdx;
		__u32	Tv_FndBlkSeq;
		__u8	Tv_FndOvwtId;
		__u32	Tv_GapFnd;
		__u32	Tv_GapNow;
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
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrDb)
				{
					// check block
					for(Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					{
						// check block type
						if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType)
							&& (A_Ch == Tv_PtrDb[Tv_BlkIdx].Ch))
						{
							// check sequence id
							if(Tv_PtrDb[Tv_BlkIdx].SeqId == A_SeqId)
							{
								// found
								Tv_Result	=	TRUE;
								Tv_IsExit	=	TRUE;

								Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
								Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
								Tv_FndBlkSeq	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
								Tv_FndOvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;
								break;
							}
							else if(Tv_PtrDb[Tv_BlkIdx].SeqId > A_SeqId)
							{
								if(0 > Tv_FndDskIdx)
								{
									Tv_Result	=	TRUE;
									Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
									Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
									Tv_FndBlkSeq	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
									Tv_FndOvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;

									Tv_GapFnd			=	Tv_PtrDb[Tv_BlkIdx].SeqId - A_SeqId;
								}
								else
								{
									Tv_GapNow		=	Tv_PtrDb[Tv_BlkIdx].SeqId - A_SeqId;
									if(Tv_GapNow > Tv_GapFnd)
									{
										Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
										Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
										Tv_FndBlkSeq	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
										Tv_FndOvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;
										Tv_GapFnd			=	Tv_GapNow;
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
__u8	Cls_GrDvrFs6Grp::ReadFwGoByTime(Ptr_GrDvrFs6ReadCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ErrCode;
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		__u32	Tv_BlkOfs;
		__u64	Tv_Hndl;
	// code --------------------
		Tv_Result	=	E_GrErrNotFound;

		// init control
		A_PtrCtrl->Hndl				=	E_GrDvrRecHndlNone;
		A_PtrCtrl->BlkSeq			=	0;
		A_PtrCtrl->IsFrmRead	=	FALSE;

		// get start block
		if(LcGrpBlkFindByTimeFw(A_PtrCtrl->Ch, A_Time, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq,&Tv_OvwtId))
		{
			// find min offset
			Tv_Hndl	=	E_GrDvrRecHndlNone;
			Tv_ErrCode	=	LcDskBlkTimeOfsFindByTimeFw((__u32)Tv_DskIdx, (__u32)Tv_BlkIdx, A_Time, &Tv_BlkOfs);
			if(E_GrErrNone == Tv_ErrCode)
			{
				// make handle
				Tv_Hndl	=	GrDvrFs6HndlEnc((__u8)Tv_DskIdx, Tv_BlkOfs, (__u16)Tv_BlkIdx, Tv_OvwtId);
			}
			while(E_GrDvrRecStatEmpty <= m_GrpStat)
			{
				// check read handle
				if(E_GrDvrRecHndlNone != Tv_Hndl)
				{
					// read now frame
					Tv_ErrCode	=	LcGrpDataReadByHndl(Tv_Hndl, &A_PtrCtrl->FrmHd, sizeof(St_GrDvrFrmHdUnknown));
					if(E_GrErrNone == Tv_ErrCode)
					{
						// update to control
						A_PtrCtrl->Hndl		=	Tv_Hndl;
						A_PtrCtrl->BlkSeq	=	Tv_BlkSeq;
						A_PtrCtrl->IsFrmRead	=	TRUE;

						Tv_Result	=	E_GrErrNone;
						break;
					}
				}
				// read next block
				Tv_BlkSeq ++;
				if(!LcGrpBlkFindBySeqFw(A_PtrCtrl->Ch, Tv_BlkSeq,&Tv_DskIdx,&Tv_BlkIdx,&Tv_BlkSeq,&Tv_OvwtId))
				{
					// not found
					break;
				}
				// make new handle
				Tv_BlkOfs	=	sizeof(St_GrDvrFs6BlkDbHead);
				Tv_Hndl	=	GrDvrFs6HndlEnc((__u8)Tv_DskIdx, Tv_BlkOfs, (__u16)Tv_BlkIdx, Tv_OvwtId);

			}	//while(E_GrDvrRecStatBad != m_GrpStat)
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::LcGrpDataReadByHndl(__u64 A_Hndl, void* A_PtrData, __u32 A_Size)
{
	// local -------------------
		__u8	Tv_Result;
		__s32	Tv_DskIdx;
		__u8	Tv_OvwtId;
		__s32	Tv_BlkIdx;
		__u32	Tv_Ofs;
		__u32	Tv_BlkCnt;
		__u32	Tv_ReadAble;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		// check read size
		if(0 != A_Size)
		{
			// decode handle
			GrDvrFs6HndlDec(A_Hndl, &Tv_DskIdx, &Tv_Ofs, &Tv_BlkIdx, &Tv_OvwtId);

			// check disk range
			if(m_DiskCnt > (__u32)Tv_DskIdx)
			{
				// get block info
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrDb)
				{
					// check block index
					if(Tv_BlkCnt > (__u32)Tv_BlkIdx)
					{
						// check block type
						if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeBad != Tv_PtrDb[Tv_BlkIdx].BlkType))
						{
							// check match overwrite id
							if(Tv_OvwtId == Tv_PtrDb[Tv_BlkIdx].OvwtId)
							{
								// try read
								Tv_ReadAble	=	(Tv_PtrDb[Tv_BlkIdx].InSize + sizeof(St_GrDvrFs6BlkDbHead)) - Tv_Ofs;
								if(Tv_ReadAble >= A_Size)
								{
									Tv_Result	=	m_DiskTbl[Tv_DskIdx]->BlkReadDataByOfs((__u32)Tv_BlkIdx, Tv_Ofs, A_PtrData, A_Size);
								}
								else
								{
									Tv_Result	=	E_GrErrEndOfData;
								}
							}
							else
							{
								DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpDataReadByHndl - bad handle (%08X%08X) expired!\n",
									(__u32)((A_Hndl >> 32) & 0xFFFFFFFF), (__u32)(A_Hndl & 0xFFFFFFFF));
								Tv_Result	=	E_GrErrNotFound;
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpDataReadByHndl - bad handle (%08X%08X) bad block operation!\n",
								(__u32)((A_Hndl >> 32) & 0xFFFFFFFF), (__u32)(A_Hndl & 0xFFFFFFFF));
							Tv_Result	=	E_GrErrDiskBadSector;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpDataReadByHndl - bad handle (%08X%08X) block index!\n",
							(__u32)((A_Hndl >> 32) & 0xFFFFFFFF), (__u32)(A_Hndl & 0xFFFFFFFF));
					}
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpDataReadByHndl - bad handle (%08X%08X) disk value!\n",
					(__u32)((A_Hndl >> 32) & 0xFFFFFFFF), (__u32)(A_Hndl & 0xFFFFFFFF));
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrDvrFs6Grp::LcGrpDataReadByHndl - Read size is zero!\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::ReadFwNext(Ptr_GrDvrFs6ReadCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ErrCode;
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		__u32	Tv_BlkOfs;
		__u32	Tv_ReadSize;
		__u64	Tv_Hndl;
		__u32	Tv_BlkOrgSeq;
		__u32	Tv_FrmSize;

		St_GrDvrFrmHdUnknown	Tv_HdBk;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		// check handle exist
		if(E_GrDvrRecHndlNone != A_PtrCtrl->Hndl)
		{
			Tv_BlkSeq			=	A_PtrCtrl->BlkSeq;
			Tv_BlkOrgSeq	=	Tv_BlkSeq;

			Tv_FrmSize		=	E_GrDvrFrmAlignSize;
			if(A_PtrCtrl->IsFrmRead)
			{
				Tv_FrmSize	=	(A_PtrCtrl->FrmHd.Base.DataSize + sizeof(St_GrDvrFrmHdUnknown) + sizeof(St_GrDvrFrmTail) + (E_GrDvrFrmAlignSize - 1))
					& E_GrDvrFrmAlignMask;
			}

			// get block info
			GrDvrFs6HndlDec(A_PtrCtrl->Hndl, &Tv_DskIdx, &Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);
			while(E_GrDvrRecStatEmpty <= m_GrpStat)
			{
				// go next position
				Tv_BlkOfs	=	Tv_BlkOfs + Tv_FrmSize;
				Tv_Hndl		=	GrDvrFs6HndlEnc(Tv_DskIdx, Tv_BlkOfs, Tv_BlkIdx, Tv_OvwtId);

				// test
				if(A_PtrCtrl->IsFrmRead)
				{
					GrDumyCopyMem(&Tv_HdBk, &A_PtrCtrl->FrmHd, sizeof(St_GrDvrFrmHdUnknown));
				}

				// read frame header
				A_PtrCtrl->Hndl				=	Tv_Hndl;
				A_PtrCtrl->IsFrmRead	=	FALSE;
				Tv_ErrCode	=	LcGrpDataReadByHndl(Tv_Hndl, &A_PtrCtrl->FrmHd, sizeof(St_GrDvrFrmHdUnknown));
				if(E_GrErrNone == Tv_ErrCode)
				{
					// check frame header
					if((E_GrDvrFrmHeadFcc == A_PtrCtrl->FrmHd.Base.Fcc))
					{
						// success
						A_PtrCtrl->IsFrmRead	=	TRUE;
						Tv_Result	=	E_GrErrNone;
						break;
					}
				}

				if(E_GrErrNone != Tv_ErrCode)
				{
					// go next block
					// check last sequence
					if(Tv_BlkSeq >= m_RecChTbl[A_PtrCtrl->Ch].SeqId)
					{
						// finished
						Tv_Result	=	E_GrErrEndOfData;
						DbgMsgPrint("Cls_GrDvrFs6Grp::ReadFwNext - sequence id (%d) reach last id(%d)\n", Tv_BlkOrgSeq, Tv_BlkSeq);
						break;
					}

					// get next sequence block
					Tv_BlkSeq ++;
					if(!LcGrpBlkFindBySeqFw(A_PtrCtrl->Ch, Tv_BlkSeq, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId))
					{
						// finished
						Tv_Result	=	E_GrErrEndOfData;
						DbgMsgPrint("Cls_GrDvrFs6Grp::ReadFwNext - next sequence id (%d) not found\n", Tv_BlkSeq);
						break;
					}
					// offset move to first
					Tv_BlkOfs	=	sizeof(St_GrDvrFs6BlkDbHead);
				}
				else
				{
					Tv_FrmSize	=	E_GrDvrFrmAlignSize;
				}

			}
		}
		else
		{
			DbgMsgPrint("Cls_GrDvrFs6Grp::ReadFwNext - read control not ready.\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::LcGrpBlkFindByTimeBw(__u8 A_Ch, __u32 A_Time, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsExit;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
		__s32	Tv_FndDskIdx;
		__s32	Tv_FndBlkIdx;
		__u32	Tv_FndBlkSeq;
		__u8	Tv_FndOvwtId;
		__u32	Tv_GapFnd;
		__u32	Tv_GapNow;
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
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrDb)
				{
					// check block
					for(Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					{
						// check block type
						if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType)
							&& (A_Ch == Tv_PtrDb[Tv_BlkIdx].Ch))
						{
							// check time range
							if((0 != Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt) && (0 != Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd))
							{
								Tv_Result	=	TRUE;
								// check time range
								if((A_Time >= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt) && (A_Time <= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd))
								{
									// block found, do exit
									Tv_IsExit	=	TRUE;

									Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
									Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
									Tv_FndBlkSeq	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
									Tv_FndOvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;
									break;
								}
								else
								{
									if(0 > Tv_FndDskIdx)
									{
										Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
										Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
										Tv_FndBlkSeq	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
										Tv_FndOvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;

										if(A_Time < Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd)
										{
											Tv_GapFnd	=	Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd - A_Time;
										}
										else
										{
											Tv_GapFnd	=	A_Time - Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd + E_GrDvrFs6GrpBlkNegGap;
										}
									}
									else
									{
										if(A_Time < Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd)
										{
											Tv_GapNow	=	Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd - A_Time;
										}
										else
										{
											Tv_GapNow	=	A_Time - Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd + E_GrDvrFs6GrpBlkNegGap;
										}
										// compare
										if(Tv_GapFnd > Tv_GapNow)
										{
											// update
											Tv_GapFnd		=	Tv_GapNow;

											Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
											Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
											Tv_FndBlkSeq	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
											Tv_FndOvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;
										}
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
BOOL8	Cls_GrDvrFs6Grp::LcGrpBlkFindBySeqBw(__u8 A_Ch, __u32 A_SeqId, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId, __u32* A_PtrRtOfs)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsExit;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
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
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrDb)
				{
					// check block
					for(Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					{
						// check block type
						if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType)
							&& (A_Ch == Tv_PtrDb[Tv_BlkIdx].Ch) && (0 != Tv_PtrDb[Tv_BlkIdx].SeqId))
						{
							// check sequence id
							if(Tv_PtrDb[Tv_BlkIdx].SeqId == A_SeqId)
							{
								// found
								Tv_Result	=	TRUE;
								Tv_IsExit	=	TRUE;

								Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
								Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
								Tv_FndBlkSeq	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
								Tv_FndOvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;

								break;
							}
							else if(Tv_PtrDb[Tv_BlkIdx].SeqId < A_SeqId)
							{
								if(0 > Tv_FndDskIdx)
								{
									Tv_Result	=	TRUE;
									Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
									Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
									Tv_FndBlkSeq	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
									Tv_FndOvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;
									Tv_FndInSize	=	Tv_PtrDb[Tv_BlkIdx].InSize;
								}
								else
								{
									if(Tv_PtrDb[Tv_BlkIdx].SeqId > Tv_FndBlkSeq)
									{
										Tv_FndDskIdx	=	(__s32)Tv_DskIdx;
										Tv_FndBlkIdx	=	(__s32)Tv_BlkIdx;
										Tv_FndBlkSeq	=	Tv_PtrDb[Tv_BlkIdx].SeqId;
										Tv_FndOvwtId	=	Tv_PtrDb[Tv_BlkIdx].OvwtId;
										Tv_FndInSize	=	Tv_PtrDb[Tv_BlkIdx].InSize;
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
__u8	Cls_GrDvrFs6Grp::LcDskBlkTimeOfsFindByTimeBw(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_Time, __u32* A_PtrRtOfs)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ErrCode;
		__u32	Tv_BlkCnt;
		__u32	Tv_TimeUnit;
		__u32	Tv_FrmOfs;
		__u32	Tv_MinIdx;
		__u32	Tv_FndOfs;
		__u32	Tv_FndGap;
		__u32	Tv_NowGap;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;
		Tv_FndOfs	=	0;

		// check recursive call
		if(m_IsReadInTimeMap)
		{
			DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkTimeOfsFindByTimeBw - recursive call!\n");
			Tv_Result	=	E_GrErrBusy;
			return	Tv_Result;
		}

		m_IsReadInTimeMap	=	TRUE;

		// check disk index
		if((__u32)m_DiskCnt > A_DskIdx)
		{
			// get block info
			Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[A_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
			if(NULL != Tv_PtrDb)
			{
				if(Tv_BlkCnt > A_BlkIdx)
				{
					Tv_Result	=	E_GrErrDiskBadSector;
					// check block status
					if((E_GrDvrFs6Fcc == Tv_PtrDb[A_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[A_BlkIdx].BlkType))
					{

						Tv_FndGap		=	0;
						Tv_Result		=	E_GrErrNotFound;
						Tv_TimeUnit	=	A_Time - (A_Time % E_GrDvrFs6TimeMapSecUnit);
						Tv_FrmOfs		=	Tv_PtrDb[A_BlkIdx].OfsLastTime;

						while(0 != Tv_FrmOfs)
						{
							// load time frame
							Tv_ErrCode	=	m_DiskTbl[A_DskIdx]->BlkReadDataByOfs(A_BlkIdx, Tv_FrmOfs, &m_ReadTimeMap, sizeof(m_ReadTimeMap));
							if(E_GrErrNone == Tv_ErrCode)
							{
								// check frame type
								if((E_GrDvrFrmHeadFcc == m_ReadTimeMap.Head.Base.Fcc) && (E_GrDvrFrmTypeTimeMap == m_ReadTimeMap.Head.Base.Type))
								{
									// get next table position
									if(Tv_FrmOfs <= m_ReadTimeMap.Head.OfsPrv)
									{
										DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkTimeOfsFindByTimeBw - bad frame offset!\n");
										Tv_FrmOfs		=	0;
									}
									else
									{
										Tv_FrmOfs	=	m_ReadTimeMap.Head.OfsPrv;
									}

									// check exist item
									if(0 != m_ReadTimeMap.Head.InCnt)
									{
										// check match time
										for(Tv_MinIdx = 0; Tv_MinIdx < m_ReadTimeMap.Head.InCnt; Tv_MinIdx++)
										{
											// get
											if(0 == Tv_FndOfs)
											{
												Tv_FndOfs	=	m_ReadTimeMap.TimeTbl[Tv_MinIdx].Ofs;
												if(Tv_TimeUnit >= m_ReadTimeMap.TimeTbl[Tv_MinIdx].Time)
												{
													Tv_FndGap	=	Tv_TimeUnit - m_ReadTimeMap.TimeTbl[Tv_MinIdx].Time;
												}
												else
												{
													Tv_FndGap	=	m_ReadTimeMap.TimeTbl[Tv_MinIdx].Time - Tv_TimeUnit  + E_GrDvrFs6GrpBlkNegGap;
												}

												Tv_Result	=	E_GrErrNone;
											}
											else
											{
												// get now gap
												if(Tv_TimeUnit >= m_ReadTimeMap.TimeTbl[Tv_MinIdx].Time)
												{
													Tv_NowGap	=	Tv_TimeUnit - m_ReadTimeMap.TimeTbl[Tv_MinIdx].Time;
												}
												else
												{
													Tv_NowGap	=	m_ReadTimeMap.TimeTbl[Tv_MinIdx].Time - Tv_TimeUnit + E_GrDvrFs6GrpBlkNegGap;
												}
												// compare gap
												if(Tv_NowGap < Tv_FndGap)
												{
													// update new item
													Tv_FndOfs	=	m_ReadTimeMap.TimeTbl[Tv_MinIdx].Ofs;
													Tv_FndGap	=	Tv_NowGap;
													Tv_Result	=	E_GrErrNone;
												}
											}

											// check finish
											if((0 != Tv_FndOfs) && (0 == Tv_FndGap))
											{
												break;
											}

										}
									}
								}
								else
								{
									DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkTimeOfsFindByTimeBw - Disk %d block %d offset %08X Min map data broken!",
										m_DiskTbl[A_DskIdx]->DevIdxGet(), A_BlkIdx, Tv_FrmOfs);
									Tv_Result	=	E_GrErrDataBroken;
									break;
								}
							}
							else
							{
								Tv_Result	=	Tv_ErrCode;
								break;
							}

							if((0 != Tv_FndOfs) && (0 == Tv_FndGap))
							{
								break;
							}

						}	//	while(0 != Tv_FrmOfs)

					}
				}
			}
		}

		m_IsReadInTimeMap	=	FALSE;

		*A_PtrRtOfs	=	Tv_FndOfs;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::ReadBwGoByTime(Ptr_GrDvrFs6ReadCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ErrCode;
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		__u32	Tv_BlkOfs;
		__u64	Tv_Hndl;
	// code --------------------
		Tv_Result	=	E_GrErrNotFound;

		// init control
		A_PtrCtrl->Hndl		=	E_GrDvrRecHndlNone;
		A_PtrCtrl->BlkSeq	=	0;
		A_PtrCtrl->IsFrmRead	=	FALSE;

		// get start block
		if(LcGrpBlkFindByTimeBw(A_PtrCtrl->Ch, A_Time, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId))
		{
			// find min offset
			Tv_Hndl	=	E_GrDvrRecHndlNone;
			Tv_ErrCode	=	LcDskBlkTimeOfsFindByTimeBw((__u32)Tv_DskIdx, (__u32)Tv_BlkIdx, A_Time, &Tv_BlkOfs);
			if(E_GrErrNone == Tv_ErrCode)
			{
				// make handle
				Tv_Hndl	=	GrDvrFs6HndlEnc((__u8)Tv_DskIdx, Tv_BlkOfs, (__u16)Tv_BlkIdx, Tv_OvwtId);
			}
			while(E_GrDvrRecStatEmpty <= m_GrpStat)
			{
				// check read handle
				if(E_GrDvrRecHndlNone != Tv_Hndl)
				{
					Tv_ErrCode	=	LcGrpDataReadByHndl(Tv_Hndl, &A_PtrCtrl->FrmHd, sizeof(St_GrDvrFrmHdUnknown));
					if(E_GrErrNone == Tv_ErrCode)
					{
						// update to control
						A_PtrCtrl->Hndl		=	Tv_Hndl;
						A_PtrCtrl->BlkSeq	=	Tv_BlkSeq;

						Tv_Result	=	E_GrErrNone;
						break;
					}
				}
				// read next block
				Tv_BlkSeq --;
				if(!LcGrpBlkFindBySeqBw(A_PtrCtrl->Ch, Tv_BlkSeq, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId, &Tv_BlkOfs))
				{
					// not found
					break;
				}
				// make new handle
				Tv_Hndl	=	GrDvrFs6HndlEnc((__u8)Tv_DskIdx, Tv_BlkOfs, (__u16)Tv_BlkIdx, Tv_OvwtId);

			}	//while(E_GrDvrRecStatBad != m_GrpStat)
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::ReadBwNext(Ptr_GrDvrFs6ReadCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ErrCode;
		BOOL8	Tv_IsNextBlk;
		__u8	Tv_OvwtId;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		__u32	Tv_BlkOfs;
		__u32	Tv_ReqSize;
		__u32	Tv_ReadSize;
		__u64	Tv_Hndl;
		__u32	Tv_FrmSize;
		St_GrDvrFrmTail	Tv_FrmTail;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		// check handle exist
		if(E_GrDvrRecHndlNone != A_PtrCtrl->Hndl)
		{
			Tv_BlkSeq	=	A_PtrCtrl->BlkSeq;

			// get block info
			GrDvrFs6HndlDec(A_PtrCtrl->Hndl, &Tv_DskIdx, &Tv_BlkOfs, &Tv_BlkIdx, &Tv_OvwtId);

			while(E_GrDvrRecStatEmpty <= m_GrpStat)
			{
				// init
				Tv_IsNextBlk		=	TRUE;
				// check offset
				if((sizeof(St_GrDvrFs6BlkDbHead) + E_GrDvrFrmMinSize) <= Tv_BlkOfs)
				{
					Tv_IsNextBlk		=	FALSE;
					// try read tail
					Tv_Hndl	=	GrDvrFs6HndlEnc(Tv_DskIdx, Tv_BlkOfs - sizeof(St_GrDvrFrmTail), Tv_BlkIdx, Tv_OvwtId);
					Tv_ErrCode	=	LcGrpDataReadByHndl(Tv_Hndl, &Tv_FrmTail, sizeof(St_GrDvrFrmTail));
					if(E_GrErrNone == Tv_ErrCode)
					{
						// get frame size
						Tv_FrmSize	=	Tv_FrmTail.PktSize;
						// check frame size
						if((E_GrDvrFrmMinSize > Tv_FrmSize) || ((Tv_BlkOfs - sizeof(St_GrDvrFs6BlkDbHead)) < Tv_FrmSize) ||
							(0 != ((E_GrDvrFrmAlignSize - 1)& Tv_FrmSize)) || (E_GrDvrFrmTailFcc != Tv_FrmTail.Fcc))
						{
							DbgMsgPrint("Cls_GrDvrFs6Grp::ReadBwNext - Disk %d block %d offset %08X bad tail frame (fcc:%08X size:%08X) found.\n",
								m_DiskTbl[Tv_DskIdx]->DevIdxGet(), Tv_BlkIdx, Tv_BlkOfs - sizeof(St_GrDvrFrmTail), Tv_FrmTail.Fcc, Tv_FrmTail.PktSize);
							Tv_FrmSize	=	E_GrDvrFrmAlignSize;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrDvrFs6Grp::ReadBwNext - Disk %d block %d offset %08X tail read error. go next block!\n",
							m_DiskTbl[Tv_DskIdx]->DevIdxGet(), Tv_BlkIdx, Tv_BlkOfs - sizeof(St_GrDvrFrmTail));
						Tv_IsNextBlk	=	TRUE;
					}

					// read head
					if(!Tv_IsNextBlk)
					{
						Tv_BlkOfs	=	Tv_BlkOfs - Tv_FrmSize;
						Tv_Hndl	=	GrDvrFs6HndlEnc(Tv_DskIdx, Tv_BlkOfs - sizeof(St_GrDvrFrmTail), Tv_BlkIdx, Tv_OvwtId);

						A_PtrCtrl->IsFrmRead	=	FALSE;
						A_PtrCtrl->Hndl				=	Tv_Hndl;
						Tv_ErrCode	=	LcGrpDataReadByHndl(Tv_Hndl, &A_PtrCtrl->FrmHd, sizeof(St_GrDvrFrmHdUnknown));
						if(E_GrErrNone == Tv_ErrCode)
						{
							// check frame header
							if((E_GrDvrFrmHeadFcc == A_PtrCtrl->FrmHd.Base.Fcc))
							{
								A_PtrCtrl->IsFrmRead	=	TRUE;
								A_PtrCtrl->BlkSeq			=	Tv_BlkSeq;
								// found
								Tv_Result	=	E_GrErrNone;
								break;
							}

							// check next block
							if((sizeof(St_GrDvrFs6BlkDbHead) + E_GrDvrFrmMinSize) >= Tv_BlkOfs)
							{
								Tv_IsNextBlk	=	TRUE;
							}
						}
						else
						{
							// go next block
							Tv_IsNextBlk	=	TRUE;
						}
					}

				}

				// go next block
				if(Tv_IsNextBlk)
				{
					// check last sequence
					if(Tv_BlkSeq <= E_GrDvrFs6SeqIdMin)
					{
						// finished
						Tv_Result	=	E_GrErrEndOfData;
						break;
					}

					Tv_BlkSeq --;
					if(!LcGrpBlkFindBySeqBw(A_PtrCtrl->Ch, Tv_BlkSeq, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId, &Tv_BlkOfs))
					{
						// finished
						Tv_Result	=	E_GrErrEndOfData;
						break;
					}
				}	//	if(Tv_IsNextBlk)
			}	//	while(E_GrDvrRecStatEmpty <= m_GrpStat)
		}	//	if(E_GrDvrRecHndlNone != A_PtrCtrl->Hndl)

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::GroupInfoGet(__u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		__u8	Tv_Result;
		BOOL8	Tv_IsAllBlkUse;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkIdx;
		__u32	Tv_BlkCnt;
		__u64	Tv_Total;
		__u64	Tv_RecSize;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result		=	E_GrDvrRecStatNotexist;
		Tv_Total		=	0;
		Tv_RecSize	=	0;

		if(0 != m_DiskCnt)
		{
			Tv_IsAllBlkUse	=	TRUE;
			Tv_Result	=	E_GrDvrRecStatBad;

			for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
				if(NULL != Tv_PtrDb)
				{
					for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
					{
						// check block info
						if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType))
						{
							// add total
							Tv_Total	=	Tv_Total + E_GrDvrFs6BlkSize;

							// check full block
							if(0 != (E_GrDvrFs6BlkAtbFull & Tv_PtrDb[Tv_BlkIdx].BlkAtb))
							{
								Tv_RecSize	=	Tv_RecSize + E_GrDvrFs6BlkSize;
							}
							else
							{
								if(0 != Tv_PtrDb[Tv_BlkIdx].InSize)
								{
									Tv_RecSize	=	Tv_RecSize + (__u64)(Tv_PtrDb[Tv_BlkIdx].InSize + sizeof(St_GrDvrFs6BlkDbHead));
								}
								else
								{
									Tv_IsAllBlkUse	=	FALSE;
								}
							}
						}
					}
				}
			}

			// check status
			if(0 != Tv_Total)
			{
				Tv_Result	=	E_GrDvrRecStatEmpty;

				if(0 != Tv_RecSize)
				{
					Tv_Result	=	E_GrDvrRecStatNormal;

					if(m_IsOvwtMode)
					{
						if(Tv_IsAllBlkUse)
						{
							Tv_Result	=	E_GrDvrRecStatOverwrite;
						}
					}
					else
					{
						if(Tv_RecSize >= Tv_Total)
						{
							Tv_Result	=	E_GrDvrRecStatFull;
						}
					}
				}
			}
		}

		m_GrpStat	=	Tv_Result;
		*A_PtrRtTotal	=	Tv_Total;
		*A_PtrRtRec		=	Tv_RecSize;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::RecTimeRangeGet(__u32* A_PtrRtTimeSt, __u32* A_PtrRtTimeEd)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
		__u32	Tv_TmSt;
		__u32	Tv_TmEd;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_TmSt		=	0;
		Tv_TmEd		=	0;

		if(0 != m_DiskCnt)
		{
			for( Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				// check exist disk
				if(NULL != m_DiskTbl[Tv_DskIdx])
				{
					// get disk time range
					Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
					if(NULL != Tv_PtrDb)
					{
						for( Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
						{
							// check block type
							if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType))
							{
								// check time range
								if((0 != Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt) && (0 != Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd))
								{
									// update start time
									if(0 == Tv_TmSt)
									{
										Tv_TmSt	=	Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt;
									}
									else if(Tv_TmSt > Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt)
									{
										Tv_TmSt	=	Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt;
									}
									// update end time
									if(0 == Tv_TmEd)
									{
										Tv_TmEd	=	Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd;
									}
									else if(Tv_TmEd < Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd)
									{
										Tv_TmEd	=	Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd;
									}
									Tv_Result	=	TRUE;
								}
							}
						}
					}
				}
			}

		}

		// update result
		*A_PtrRtTimeSt	=	Tv_TmSt;
		*A_PtrRtTimeEd	=	Tv_TmEd;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs6Grp::LcDskBlkGetDayMap(__u32 A_DskIdx, __u32 A_BlkIdx, __u16 A_DaySt, __u16 A_DayEd, Ptr_GrDvrFs6FrmDayMap A_PtrWorkBuf)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkCnt;
		__u32	Tv_FrmOfs;
		__u32	Tv_ItmIdx;
		__u32	Tv_DayMask;
		__u8	Tv_ErrCode;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
		Ptr_GrDvrFs6FrmDayMap	Tv_PtrDayMap;
	// code --------------------
		Tv_Result	=	0;

		// check disk index
		if((__u32)m_DiskCnt > A_DskIdx)
		{
			// get block info
			Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[A_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
			if(NULL != Tv_PtrDb)
			{
				if(Tv_BlkCnt > A_BlkIdx)
				{
					// check block status
					if((E_GrDvrFs6Fcc == Tv_PtrDb[A_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[A_BlkIdx].BlkType))
					{
						// allocate work buffer
						Tv_PtrDayMap	=	A_PtrWorkBuf;
						if(NULL == Tv_PtrDayMap)
						{
							Tv_PtrDayMap	=	(Ptr_GrDvrFs6FrmDayMap)malloc(sizeof(St_GrDvrFs6FrmDayMap));
						}
						if(NULL != Tv_PtrDayMap)
						{
							Tv_FrmOfs		=	Tv_PtrDb[A_BlkIdx].OfsLastDate;

							while(0 != Tv_FrmOfs)
							{
								// load time frame
								Tv_ErrCode	=	m_DiskTbl[A_DskIdx]->BlkReadDataByOfs(A_BlkIdx, Tv_FrmOfs, Tv_PtrDayMap, sizeof(St_GrDvrFs6FrmDayMap));
								if(E_GrErrNone != Tv_ErrCode)
								{
									break;
								}
								// check frame type
								if((E_GrDvrFrmHeadFcc == Tv_PtrDayMap->Head.Base.Fcc) && (E_GrDvrFrmTypeDayMap == Tv_PtrDayMap->Head.Base.Type))
								{
									// get info
									if(Tv_FrmOfs > Tv_PtrDayMap->Head.OfsPrv)
									{
										Tv_FrmOfs	=	Tv_PtrDayMap->Head.OfsPrv;
									}
									else
									{
										DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkGetDayMap - bad frame offset!\n");
										Tv_FrmOfs	=	0;
									}

									// build day map
									if(0 != Tv_PtrDayMap->Head.InCnt)
									{
										for( Tv_ItmIdx = 0; Tv_ItmIdx < Tv_PtrDayMap->Head.InCnt; Tv_ItmIdx++)
										{
											if((A_DaySt <= Tv_PtrDayMap->DayTbl[Tv_ItmIdx]) && (A_DayEd >= Tv_PtrDayMap->DayTbl[Tv_ItmIdx]))
											{
												// found
												Tv_DayMask	=	1 << (__u32)(Tv_PtrDayMap->DayTbl[Tv_ItmIdx] - A_DaySt);
												Tv_Result	=	Tv_Result | Tv_DayMask;
											}
										}
									}
								}
							}	//	while(0 != Tv_FrmOfs)

							// release work buffer
							if(NULL == A_PtrWorkBuf)
							{
								free(Tv_PtrDayMap);
							}
							Tv_PtrDayMap	=	NULL;
						}	//	if(NULL != Tv_PtrDayMap)
						else
						{
							DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkGetDayMap - work buffer allocate fail!\n");
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs6Grp::RecMonthDayMapGet(__u32 A_TimeMonth)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		__u32	Tv_Didx;
		__u32	Tv_TimeSt;
		__u32	Tv_TimeEd;
		__u16	Tv_DaySt;
		__u16	Tv_DayEd;

		__u16	Tv_Year;
		__u8	Tv_Month;
		__u8	Tv_Day;
		__u8	Tv_Hour;
		__u8	Tv_Min;
		__u8	Tv_Sec;

		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
		Ptr_GrDvrFs6FrmDayMap	Tv_PtrDayMap;
	// code --------------------
		Tv_Result		=	0;

		if(0 != m_DiskCnt)
		{
			// calculate time of day
			GrTimeDec(A_TimeMonth, &Tv_Year, &Tv_Month, &Tv_Day, &Tv_Hour, &Tv_Min, &Tv_Sec);

			Tv_TimeSt	=	GrTimeEnc(Tv_Year, Tv_Month, 1,0,0,0);
			Tv_Day	=	GrTimeGetMonthMaxDay(Tv_Year, Tv_Month);
			Tv_TimeEd	=	GrTimeEnc(Tv_Year, Tv_Month, Tv_Day, 23, 59, 59);

			Tv_DaySt	=	Tv_TimeSt / E_GrTimeSecOfDay;
			Tv_DayEd	=	Tv_TimeEd / E_GrTimeSecOfDay;

			Tv_PtrDayMap	=	(Ptr_GrDvrFs6FrmDayMap)malloc(sizeof(St_GrDvrFs6FrmDayMap));
			if(NULL != Tv_PtrDayMap)
			{
				for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
				{
					// check exist disk
					if(NULL != m_DiskTbl[Tv_DskIdx])
					{
						Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
						if(NULL != Tv_PtrDb)
						{
							for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
							{
								// check block time range
								if(((Tv_TimeSt <= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt) && (Tv_TimeEd >= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt)) ||
									((Tv_TimeSt <= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd) && (Tv_TimeEd >= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd)) ||
									((Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt <= Tv_TimeSt) && (Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd >= Tv_TimeSt)) ||
									((Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt <= Tv_TimeEd) && (Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd >= Tv_TimeEd))
									)
								{
									// found
									// check block day range
									if((2 * E_GrTimeSecOfDay) > (Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd - Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt))
									{
										if((Tv_TimeSt <= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt) && (Tv_TimeEd >= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt))
										{
											Tv_Didx	=	(Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt - Tv_TimeSt) / E_GrTimeSecOfDay;
											Tv_Result	=	Tv_Result | (1 << Tv_Didx);
										}
										if((Tv_TimeSt <= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd) && (Tv_TimeEd >= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd))
										{
											Tv_Didx	=	(Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd - Tv_TimeSt) / E_GrTimeSecOfDay;
											Tv_Result	=	Tv_Result | (1 << Tv_Didx);
										}
									}
									else
									{
										// do direct check day
										Tv_Result	=	Tv_Result | LcDskBlkGetDayMap(Tv_DskIdx, Tv_BlkIdx, Tv_DaySt, Tv_DayEd, Tv_PtrDayMap);
									}
								}
							}
						}
					}
				}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)

				// release buffer
				free(Tv_PtrDayMap);
				Tv_PtrDayMap	=	NULL;
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::LcDskBlkGetMinMap(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_TimeSt, __u32 A_TimeEd, Ptr_GrDvrRecMinMap A_PtrMinMap)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ErrCode;

		__u32	Tv_BlkCnt;
		__u32	Tv_FrmOfs;
		__u32	Tv_ItmIdx;
		__u32	Tv_Ch;
		__u32	Tv_MinIdx;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result	=	FALSE;

		if(m_IsReadInTimeMap)
		{
			DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkGetMinMap - recursive call.\n");
			return	FALSE;
		}

		m_IsReadInTimeMap	=	TRUE;

		// check disk index
		if((__u32)m_DiskCnt > A_DskIdx)
		{
			// get block info
			Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[A_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
			if(NULL != Tv_PtrDb)
			{
				if(Tv_BlkCnt > A_BlkIdx)
				{
					// check block status
					if((E_GrDvrFs6Fcc == Tv_PtrDb[A_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[A_BlkIdx].BlkType))
					{

						Tv_Ch				=	Tv_PtrDb[A_BlkIdx].Ch;

						Tv_FrmOfs		=	Tv_PtrDb[A_BlkIdx].OfsLastTime;

						while(0 != Tv_FrmOfs)
						{
							// load time frame
							//DbgMsgPrint("[DSK] blk %d ofs %d time frame load.\n", A_BlkIdx,Tv_FrmOfs);
							Tv_ErrCode	=	m_DiskTbl[A_DskIdx]->BlkReadDataByOfs(A_BlkIdx, Tv_FrmOfs, &m_ReadTimeMap, sizeof(m_ReadTimeMap));
							if(E_GrErrNone != Tv_ErrCode)
							{
								break;
							}
							// check frame type
							if((E_GrDvrFrmHeadFcc != m_ReadTimeMap.Head.Base.Fcc) || (E_GrDvrFrmTypeTimeMap != m_ReadTimeMap.Head.Base.Type))
							{
								DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkGetMinMap - bad frame header found! (disk:%d, block:%d, offset:%08X)\n",
									A_DskIdx, A_BlkIdx, Tv_FrmOfs);
								break;
							}
							// get info
							if(Tv_FrmOfs > m_ReadTimeMap.Head.OfsPrv)
							{
								Tv_FrmOfs	=	m_ReadTimeMap.Head.OfsPrv;
							}
							else
							{
								DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkGetMinMap - disk %d block %d frame %d privous offset %d value bad!\n",
									A_DskIdx, A_BlkIdx, Tv_FrmOfs, m_ReadTimeMap.Head.OfsPrv);
								Tv_FrmOfs	=	0;	// set next position
							}

							// build time map
							if(0 != m_ReadTimeMap.Head.InCnt)
							{
								for(Tv_ItmIdx = 0; Tv_ItmIdx < m_ReadTimeMap.Head.InCnt; Tv_ItmIdx++)
								{
									if((A_TimeSt <= m_ReadTimeMap.TimeTbl[Tv_ItmIdx].Time) && (A_TimeEd >= m_ReadTimeMap.TimeTbl[Tv_ItmIdx].Time))
									{
										// found
										Tv_MinIdx	=	(m_ReadTimeMap.TimeTbl[Tv_ItmIdx].Time - A_TimeSt) / E_GrTimeSecOfMin;

										A_PtrMinMap[Tv_MinIdx].ChAtb[Tv_Ch]	=	A_PtrMinMap[Tv_MinIdx].ChAtb[Tv_Ch] | m_ReadTimeMap.TimeTbl[Tv_ItmIdx].Ratb;

										Tv_Result	=	TRUE;
									}
								}
							}
						}	//	while(0 != Tv_FrmOfs)
					}
				}
			}
		}

		m_IsReadInTimeMap	=	FALSE;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs6Grp::LcDskBlkGetExpSize(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_TimeSt, __u32 A_TimeEd, __u32 A_ChMap,
	Ptr_GrDvrFs6FrmTimeMap  A_PtrWorkBuf)
{
	// local -------------------
		__u64	Tv_Result;
		__u32	Tv_BlkCnt;
		__u32	Tv_FrmOfs;
		__u32	Tv_ItmIdx;
		__u32	Tv_ChIdx;
		__u32	Tv_ChMask;
		__u8	Tv_ErrCode;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
		Ptr_GrDvrFs6FrmTimeMap	Tv_PtrTimeMap;
	// code --------------------
		Tv_Result	=	0;

#if 0
		// check disk index
		if((__u32)m_DiskCnt > A_DskIdx)
		{
			// get block info
			Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[A_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
			if(NULL != Tv_PtrDb)
			{
				if(Tv_BlkCnt > A_BlkIdx)
				{
					// check block status
					if((E_GrDvrFs6Fcc == Tv_PtrDb[A_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[A_BlkIdx].BlkType))
					{
						// allocate work buffer
						Tv_PtrTimeMap	=	A_PtrWorkBuf;
						if(NULL == Tv_PtrTimeMap)
						{
							Tv_PtrTimeMap	=	(Ptr_GrDvrFs6FrmTimeMap)malloc(sizeof(St_GrDvrFs6FrmTimeMap));
						}
						if(NULL != Tv_PtrTimeMap)
						{
							Tv_FrmOfs		=	Tv_PtrDb[A_BlkIdx].OfsLastTime;

							while(0 != Tv_FrmOfs)
							{
								// load time frame
								Tv_ErrCode	=	m_DiskTbl[A_DskIdx]->BlkReadDataByOfs(A_BlkIdx, Tv_FrmOfs, Tv_PtrTimeMap, sizeof(St_GrDvrFs6FrmTimeMap));
								if(E_GrErrNone != Tv_ErrCode)
								{
									break;
								}
								// check frame type
								if((E_GrDvrFrmHeadFcc != m_ReadTimeMap.Head.Base.Fcc) || (E_GrDvrFrmTypeTimeMap != m_ReadTimeMap.Head.Base.Type))
								{
									DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkGetExpSize - bad frame header found! (disk:%d, block:%d, offset:%08X,"
										"FCC:%08X,type:%d)\n",
										A_DskIdx, A_BlkIdx, Tv_FrmOfs, m_ReadTimeMap.Head.Base.Fcc, m_ReadTimeMap.Head.Base.Type);
									break;
								}
								// get info
								if(Tv_FrmOfs > m_ReadTimeMap.Head.OfsPrv)
								{
									Tv_FrmOfs	=	m_ReadTimeMap.Head.OfsPrv;
								}
								else
								{
									DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkGetExpSize - disk %d block %d frame %d privous offset %d value bad!\n",
										A_DskIdx, A_BlkIdx, Tv_FrmOfs, m_ReadTimeMap.Head.OfsPrv);
									Tv_FrmOfs	=	0;	// set next position
								}

								// calculate expect size
								if(0 != m_ReadTimeMap.Head.InCnt)
								{
									for(Tv_ItmIdx = 0; Tv_ItmIdx < m_ReadTimeMap.Head.InCnt; Tv_ItmIdx++)
									{
										if((A_TimeSt <= m_ReadTimeMap.TimeTbl[Tv_ItmIdx].Time) && (A_TimeEd >= m_ReadTimeMap.TimeTbl[Tv_ItmIdx].Time))
										{
											// found
											Tv_ChMask	=	1;
											for(Tv_ChIdx = 0; Tv_ChIdx < E_GrDvrMaxChCnt; Tv_ChIdx++)
											{
												if(0 != (Tv_ChMask & A_ChMap))
												{
													Tv_Result	=	Tv_Result + (__u64)m_ReadTimeMap.TimeTbl[Tv_ItmIdx].ChSizeTbl[Tv_ChIdx];
												}
												// next
												Tv_ChMask	=	Tv_ChMask << 1;
											}
										}
									}
								}
							}	//	while(0 != Tv_FrmOfs)

							// release work buffer
							if(NULL == A_PtrWorkBuf)
							{
								free(Tv_PtrTimeMap);
							}
							Tv_PtrTimeMap	=	NULL;
						}	//	if(NULL != Tv_PtrDayMap)
						else
						{
							DbgMsgPrint("Cls_GrDvrFs6Grp::LcDskBlkGetExpSize - work buffer allocate fail!\n");
						}
					}
				}
			}
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Grp::RecMinRatbTblGet(__u32 A_TimeMin, __u32 A_MinCnt, Ptr_GrDvrRecMinMap A_PtrMinMap)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkCnt;
		__u32	Tv_BlkIdx;
		__u32	Tv_TimeSt;
		__u32	Tv_TimeEd;

		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result	=	FALSE;

		// check parametter
		if(0 != A_MinCnt)
		{
			// reset table
			//GrDumyZeroMem(A_PtrMinMap, sizeof(St_GrDvrRecMinMap)* A_MinCnt);

			if(0 != m_DiskCnt)
			{
				Tv_TimeSt	=	A_TimeMin - (A_TimeMin % E_GrTimeSecOfMin);
				Tv_TimeEd	=	Tv_TimeSt + (E_GrTimeSecOfMin * A_MinCnt) - 1;

				for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
				{
					// check exist disk
					if(NULL != m_DiskTbl[Tv_DskIdx])
					{
						//DbgMsgPrint("[DSK] disk %d time map get start\n",Tv_DskIdx);
						Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
						if(NULL != Tv_PtrDb)
						{
							for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
							{
								// check block time range
								if(((Tv_TimeSt <= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt) && (Tv_TimeEd >= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt)) ||
									((Tv_TimeSt <= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd) && (Tv_TimeEd >= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd)) ||
									((Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt <= Tv_TimeSt) && (Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd >= Tv_TimeSt)) ||
									((Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt <= Tv_TimeEd) && (Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd >= Tv_TimeEd))
									)
								{
									// found
									//DbgMsgPrint("[DSK] disk %d blk %d min map get try\n", Tv_DskIdx, Tv_BlkIdx);
									// do check time
									if(LcDskBlkGetMinMap(Tv_DskIdx, Tv_BlkIdx, Tv_TimeSt, Tv_TimeEd, A_PtrMinMap))
									{
										Tv_Result	=	TRUE;
									}

									//DbgMsgPrint("[DSK] disk %d blk %d min map get done\n", Tv_DskIdx, Tv_BlkIdx);
								}
							}
						}
					}
				}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)

			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Grp::RecFlush(void)
{
	// local -------------------
		__u8	Tv_ErrCode;
		__u8	Tv_Ch;
		__u64	Tv_TotSize;
		__u64	Tv_RecSize;
	// code --------------------
		// check disk status
		if(E_GrDvrRecStatEmpty <= m_GrpStat)
		{
			for( Tv_Ch = 0; Tv_Ch < E_GrDvrFs6MaxChCnt; Tv_Ch++)
			{
				// check record status
				if((0 <= m_RecChTbl[Tv_Ch].DskIdx) && (0 <= m_RecChTbl[Tv_Ch].BlkIdx))
				{
					// update day map
					if(m_RecChTbl[Tv_Ch].IsDayMapChg)
					{
						// write
						if(0 != m_RecChTbl[Tv_Ch].PtrDb->OfsLastDate)
						{
							Tv_ErrCode	=	m_DiskTbl[m_RecChTbl[Tv_Ch].DskIdx]->BlkWriteDataByOfs((__u32)m_RecChTbl[Tv_Ch].BlkIdx, 
								m_RecChTbl[Tv_Ch].PtrDb->OfsLastDate, &m_RecChTbl[Tv_Ch].DayMap, sizeof(St_GrDvrFs6FrmDayMap));
							if(E_GrErrBadIo == Tv_ErrCode)
							{
								DbgMsgPrint("Cls_GrDvrFs6Grp::RecFlush - Disk %d block %d day map write fail!.\n");
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrDvrFs6Grp::RecFlush - day map modified, but OfsLastDate not exist.\n");
						}
						m_RecChTbl[Tv_Ch].IsDayMapChg	=	FALSE;
					}

					// update min map
					if(m_RecChTbl[Tv_Ch].IsTimeMapChg)
					{
						LcGrpRecTimeMapWrite(Tv_Ch);
						m_RecChTbl[Tv_Ch].IsTimeMapChg	=	FALSE;
					}

					// update DB
					if(m_RecChTbl[Tv_Ch].IsDbChg)
					{
						// update DB
						if(!m_DiskTbl[m_RecChTbl[Tv_Ch].DskIdx]->BlkWriteHeadByIdx((__u32)m_RecChTbl[Tv_Ch].BlkIdx))
						{
							DbgMsgPrint("Cls_GrDvrFs6Grp::RecFlush - disk %d block %d head write fail!\n",
								m_DiskTbl[m_RecChTbl[Tv_Ch].DskIdx]->DevIdxGet(), m_RecChTbl[Tv_Ch].BlkIdx);
							// mark bad block
							m_RecChTbl[Tv_Ch].PtrDb->BlkType		=	E_GrDvrFs6BlkTypeBad;
							// request upate disk status
							m_DiskTbl[m_RecChTbl[Tv_Ch].DskIdx]->StatusUpdate();
							GroupInfoGet(&Tv_TotSize, &Tv_RecSize);
						}
						m_RecChTbl[Tv_Ch].IsDbChg	=	FALSE;
					}
				}	//	if((0 <= m_RecChTbl[A_Ch].DskIdx) && (0 <= m_RecChTbl[A_Ch].BlkIdx))
			}
		}	//	if(E_GrDvrRecStatEmpty <= m_GrpStat)

}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::LogOpen(Ptr_GrDvrFs6LogCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ErrCode;
		__u8	Tv_OvwtId;
		__u32	Tv_BlkCnt;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result	=	E_GrErrNotFound;

		A_PtrCtrl->IsFinish	=	TRUE;
		A_PtrCtrl->InCnt			=	0;
		A_PtrCtrl->FrmPrgs		=	0;
		A_PtrCtrl->BlkSeq		=	0;
		A_PtrCtrl->DskIdx		=	-1;
		A_PtrCtrl->BlkIdx		=	0;
		A_PtrCtrl->FrmOfs		=	0;

#if 0
		// find block
		if(LcGrpBlkFindByTimeBw(A_PtrCtrl->TimeEd, &Tv_DskIdx, &Tv_BlkIdx, &Tv_BlkSeq, &Tv_OvwtId))
		{
			// check found block time range
			Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
			if(NULL != Tv_PtrDb)
			{
				// check begin time
				if(A_PtrCtrl->TimeSt <= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd)
				{
					// update open info
					A_PtrCtrl->BlkSeq		=	Tv_BlkSeq;
					A_PtrCtrl->DskIdx		=	Tv_DskIdx;
					A_PtrCtrl->BlkIdx		=	Tv_BlkIdx;
					A_PtrCtrl->FrmOfs		=	Tv_PtrDb[Tv_BlkIdx].OfsLastLog;
					A_PtrCtrl->IsFinish	=	FALSE;

					// read first frame
					if(0 != A_PtrCtrl->FrmOfs)
					{
						if((Tv_DskIdx == m_RecChTbl[A_Ch].DskIdx) && (Tv_BlkIdx == m_RecChTbl[A_Ch].BlkIdx) && (Tv_BlkSeq == m_RecChTbl[A_Ch].SeqId))
						{
							GrDumyCopyMem(&A_PtrCtrl->FrmBuf, &m_RecChTbl[A_Ch].Log, sizeof(St_GrDvrFrmLogPool));
							Tv_ErrCode	=	E_GrErrNone;
						}
						else
						{
							Tv_ErrCode	=	m_DiskTbl[Tv_DskIdx]->BlkReadDataByOfs(A_PtrCtrl->BlkIdx, A_PtrCtrl->FrmOfs,
								&A_PtrCtrl->FrmBuf, sizeof(St_GrDvrFrmLogPool));
						}
						if(E_GrErrNone == Tv_ErrCode)
						{
							A_PtrCtrl->FrmPrgs		=	A_PtrCtrl->FrmBuf.Head.InCnt;
						}
						else
						{
							// bad log info
							DbgMsgPrint("Cls_GrDvrFs6Grp::LogOpen - disk %d block %d log frame read fail!\n", 
								m_DiskTbl[Tv_DskIdx]->DevIdxGet(),A_PtrCtrl->BlkIdx);
							A_PtrCtrl->FrmOfs		=	0;
						}
					}
					Tv_Result	=	E_GrErrNone;
				}
				else
				{
					DbgMsgPrint("Cls_GrDvrFs6Grp::LogOpen - out of time range.\n");
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrDvrFs6Grp::LogReadNext - privous sequence disk %d block table get fail!\n",
					m_DiskTbl[Tv_DskIdx]->DevIdxGet());
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrDvrFs6Grp::LogOpen - time block not found.\n");
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::LogReadNext(Ptr_GrDvrFs6LogCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ErrCode;
		__u8	Tv_OvwtId;
		BOOL8	Tv_IsAble;
		__u32	Tv_TypeMask;
		__u32	Tv_ChMask;
		__u32	Tv_BlkCnt;
		__s32	Tv_DskIdx;
		__s32	Tv_BlkIdx;
		__u32	Tv_BlkSeq;
		__u32	Tv_BlkOfs;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

#if 0
		if((0 != A_PtrCtrl->BlkIdx) && (0 != A_PtrCtrl->BlkSeq) && (0 <= A_PtrCtrl->DskIdx))
		{
			Tv_Result	=	E_GrErrNone;
			while((!A_PtrCtrl->IsFinish) && (A_PtrCtrl->InCnt < A_PtrCtrl->MaxCnt))
			{
				// check frame pointer
				if(0 != A_PtrCtrl->FrmOfs)
				{
					// check progress
					while((0 != A_PtrCtrl->FrmPrgs) && (!A_PtrCtrl->IsFinish) && (A_PtrCtrl->InCnt < A_PtrCtrl->MaxCnt))
					{
						Tv_IsAble	=	TRUE;
						// check time range
						if((A_PtrCtrl->TimeSt > A_PtrCtrl->FrmBuf.LogTbl[A_PtrCtrl->FrmPrgs - 1].Time) || (A_PtrCtrl->TimeEd < A_PtrCtrl->FrmBuf.LogTbl[A_PtrCtrl->FrmPrgs - 1].Time))
						{
							Tv_IsAble		=	FALSE;
						}
						// get frame progress
						Tv_TypeMask	=	1 << (A_PtrCtrl->FrmBuf.LogTbl[A_PtrCtrl->FrmPrgs - 1].LogType - 1);
						if(0 == (Tv_TypeMask & A_PtrCtrl->TypeMask))
						{
							Tv_IsAble		=	FALSE;
						}
						// get channel
						Tv_ChMask	=	1 << A_PtrCtrl->FrmBuf.LogTbl[A_PtrCtrl->FrmPrgs - 1].Ch;
						if(0 == (Tv_ChMask & A_PtrCtrl->ChMask))
						{
							Tv_IsAble		=	FALSE;
						}
						// add item
						if(Tv_IsAble)
						{
							GrDumyCopyMem(&A_PtrCtrl->PtrBuf[A_PtrCtrl->InCnt], &A_PtrCtrl->FrmBuf.LogTbl[A_PtrCtrl->FrmPrgs - 1], sizeof(St_GrDvrLogItm));
							A_PtrCtrl->InCnt ++;
							if(A_PtrCtrl->InCnt >= A_PtrCtrl->MaxCnt)
							{
								// finish
								A_PtrCtrl->IsFinish	=	TRUE;
								break;
							}
						}
						A_PtrCtrl->FrmPrgs --;
					}

					// read next frame
					if(0 == A_PtrCtrl->FrmPrgs)
					{
						if(A_PtrCtrl->FrmOfs > A_PtrCtrl->FrmBuf.Head.OfsPrv)
						{
							A_PtrCtrl->FrmOfs	=	A_PtrCtrl->FrmBuf.Head.OfsPrv;
						}
						else
						{
							DbgMsgPrint("Cls_GrDvrFs6Grp::LogReadNext - disk %d block %d log frame offset bad!\n",
								m_DiskTbl[A_PtrCtrl->DskIdx]->DevIdxGet(), A_PtrCtrl->BlkIdx);
							A_PtrCtrl->FrmOfs	=	0;
						}
						if(0 != A_PtrCtrl->FrmOfs)
						{
							// check match block
							Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[A_PtrCtrl->DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
							if(NULL != Tv_PtrDb)
							{
								if((A_PtrCtrl->BlkIdx < (__s32)Tv_BlkCnt) && (A_PtrCtrl->BlkSeq == Tv_PtrDb[A_PtrCtrl->BlkIdx].SeqId))
								{
									Tv_ErrCode	=	m_DiskTbl[A_PtrCtrl->DskIdx]->BlkReadDataByOfs(A_PtrCtrl->BlkIdx, A_PtrCtrl->FrmOfs,
										&A_PtrCtrl->FrmBuf, sizeof(St_GrDvrFrmLogPool));
									if(E_GrErrNone == Tv_ErrCode)
									{
										A_PtrCtrl->FrmPrgs		=	A_PtrCtrl->FrmBuf.Head.InCnt;
									}
									else
									{
										// bad log info
										DbgMsgPrint("Cls_GrDvrFs6Grp::LogReadNext - disk %d block %d log frame read fail!\n",
											m_DiskTbl[A_PtrCtrl->DskIdx]->DevIdxGet(), A_PtrCtrl->BlkIdx);
										A_PtrCtrl->FrmOfs		=	0;
									}
								}
								else
								{
									DbgMsgPrint("Cls_GrDvrFs6Grp::LogReadNext - disk %d block %d block overwrited!\n",
										m_DiskTbl[A_PtrCtrl->DskIdx]->DevIdxGet(), A_PtrCtrl->BlkIdx);
									A_PtrCtrl->FrmOfs		=	0;
									A_PtrCtrl->IsFinish	=	TRUE;
								}
							}
							else
							{
								DbgMsgPrint("Cls_GrDvrFs6Grp::LogReadNext - disk %d block table get fail!\n",
									m_DiskTbl[A_PtrCtrl->DskIdx]->DevIdxGet());
								A_PtrCtrl->FrmOfs		=	0;
							}
						}
					}
				}

				// do next block
				if((0 == A_PtrCtrl->FrmOfs) && (!A_PtrCtrl->IsFinish))
				{
					// read next block
					A_PtrCtrl->BlkSeq --;
					if(0 != A_PtrCtrl->BlkSeq)
					{
						if(LcGrpBlkFindBySeqBw(A_PtrCtrl->BlkSeq,&Tv_DskIdx,&Tv_BlkIdx,&Tv_BlkSeq,&Tv_OvwtId,&Tv_BlkOfs))
						{
							// check found block time range
							Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
							if(NULL != Tv_PtrDb)
							{
								// check begin time
								if(A_PtrCtrl->TimeSt <= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd)
								{
									// update open info
									A_PtrCtrl->BlkSeq		=	Tv_BlkSeq;
									A_PtrCtrl->DskIdx		=	Tv_DskIdx;
									A_PtrCtrl->BlkIdx		=	Tv_BlkIdx;
									A_PtrCtrl->FrmOfs		=	Tv_PtrDb[Tv_BlkIdx].OfsLastLog;
									A_PtrCtrl->IsFinish	=	FALSE;

									// read first frame
									if(0 != A_PtrCtrl->FrmOfs)
									{
										if((Tv_DskIdx == m_RecChTbl[A_Ch].DskIdx) && (Tv_BlkIdx == m_RecChTbl[A_Ch].BlkIdx) && (Tv_BlkSeq == m_RecChTbl[A_Ch].SeqId))
										{
											GrDumyCopyMem(&A_PtrCtrl->FrmBuf, &m_RecChTbl[A_Ch].Log, sizeof(St_GrDvrFrmLogPool));
											Tv_ErrCode	=	E_GrErrNone;
										}
										else
										{
											Tv_ErrCode	=	m_DiskTbl[Tv_DskIdx]->BlkReadDataByOfs(A_PtrCtrl->BlkIdx, A_PtrCtrl->FrmOfs,
												&A_PtrCtrl->FrmBuf, sizeof(St_GrDvrFrmLogPool));
										}
										if(E_GrErrNone == Tv_ErrCode)
										{
											A_PtrCtrl->FrmPrgs		=	A_PtrCtrl->FrmBuf.Head.InCnt;
										}
										else
										{
											// bad log info
											DbgMsgPrint("Cls_GrDvrFs6Grp::LogReadNext - disk %d block %d log frame read fail!\n",
												m_DiskTbl[Tv_DskIdx]->DevIdxGet(), A_PtrCtrl->BlkIdx);
											A_PtrCtrl->FrmOfs		=	0;
										}
									}
								}
								else
								{
									DbgMsgPrint("Cls_GrDvrFs6Grp::LogReadNext - time begin reached.\n");
									A_PtrCtrl->IsFinish	=	TRUE;
								}
							}
							else
							{
								DbgMsgPrint("Cls_GrDvrFs6Grp::LogReadNext - privous sequence disk %d block table get fail!\n",
									m_DiskTbl[Tv_DskIdx]->DevIdxGet());
								A_PtrCtrl->IsFinish	=	TRUE;
							}
						}
						else
						{
							// finished
							DbgMsgPrint("Cls_GrDvrFs6Grp::LogReadNext - privous block not found.\n");
							A_PtrCtrl->IsFinish	=	TRUE;
						}
					}
					else
					{
						// finished
						DbgMsgPrint("Cls_GrDvrFs6Grp::LogReadNext - block sequence begin reached.\n");
						A_PtrCtrl->IsFinish	=	TRUE;
					}
				}

			}

		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs6Grp::RecExpectSizeGet(__u32 A_TimeSt, __u32 A_TimeEd, __u32 A_ChMap)
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeSt;
		Def_GrTime	Tv_TimeEd;
		__u32	Tv_DskIdx;
		__u32	Tv_BlkIdx;
		__u32	Tv_BlkCnt;
		Ptr_GrDvrFs6FrmTimeMap	Tv_PtrTmap;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result	=	0;

		// check time range
		if((0 != A_ChMap) && (A_TimeSt <= A_TimeEd))
		{
			if(0 != m_DiskCnt)
			{
				// calculate time
				Tv_TimeSt	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin);
				Tv_TimeEd	=	A_TimeEd - (A_TimeEd % E_GrTimeSecOfMin) + E_GrTimeSecOfMin - 1;

				Tv_PtrTmap	=	(Ptr_GrDvrFs6FrmTimeMap)malloc(sizeof(St_GrDvrFs6FrmTimeMap));
				if(NULL != Tv_PtrTmap)
				{
					for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
					{
						// check exist disk
						if(NULL != m_DiskTbl[Tv_DskIdx])
						{
							Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_DiskTbl[Tv_DskIdx]->BlkHeadTableGet(&Tv_BlkCnt);
							if(NULL != Tv_PtrDb)
							{
								for(Tv_BlkIdx = 1; Tv_BlkIdx < Tv_BlkCnt; Tv_BlkIdx++)
								{
									// check block time range
									if(((Tv_TimeSt <= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt) && (Tv_TimeEd >= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt)) ||
										((Tv_TimeSt <= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd) && (Tv_TimeEd >= Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd)) ||
										((Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt <= Tv_TimeSt) && (Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd >= Tv_TimeSt)) ||
										((Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt <= Tv_TimeEd) && (Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd >= Tv_TimeEd))
										)
									{
										// found
										// get size
										Tv_Result	=	Tv_Result + LcDskBlkGetExpSize(Tv_DskIdx, Tv_BlkIdx, Tv_TimeSt, Tv_TimeEd, A_ChMap, Tv_PtrTmap);
									}
								}
							}
						}
					}	//	for(Tv_DskIdx = 0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)

					// release buffer
					free(Tv_PtrTmap);
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Grp::GroupDataReadByHndl(__u64 A_Hndl, void* A_PtrData, __u32 A_Size)
{
		return	LcGrpDataReadByHndl(A_Hndl, A_PtrData, A_Size);
}
//--------------------------------------------------------------------

