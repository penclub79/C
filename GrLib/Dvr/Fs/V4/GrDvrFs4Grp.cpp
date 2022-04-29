/*
 GAUSE file system version 3 group

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
#include <Dvr/Fs/V4/GrDvrFs4Base.h>
#include <Dvr/Fs/V4/GrDvrFs4Grp.h>
#include <Dvr/Fs/V4/GrDvrFs4Mng.h>

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
Cls_GrDvrFs4Grp::Cls_GrDvrFs4Grp(void* A_FsMng, __u8 A_DskMap, BOOL8 A_IsOvrtMode)
{
	// local -------------------
		// __u8	Tv_IntIdx;
	// code --------------------
		//init
		m_FsMng				=	A_FsMng;
		m_DiskMap			=	A_DskMap;
		GrDumyZeroMem( m_DiskTbl, sizeof(m_DiskTbl) );
		m_DiskCnt			=	0;

		m_RecSeqId		=	E_GrDvrFs4SeqIdNone;
		m_RecDskIdx		=	0;
		m_RecBdtIdx		=	0;

		m_IsRecAble		=	FALSE;
		m_IsOvwtMode	=	A_IsOvrtMode;

		m_IsOvwting		=	FALSE;

		GrDumyZeroMem( m_ChTbl, sizeof(m_ChTbl) );

		GrDumyZeroMem(&m_GrpWorkEnv, sizeof(m_GrpWorkEnv));

		// build disk list
		LcDiskTblBuild();
		if ((!LcRecIdChkSelfOvlp()) || (!LcRecIdChkRefOvlp()) )
		{
			// defect notify
			GrTaskSvrMsgErrorNotify(E_GrErrRecGrpDefective,m_GrpType);
			DbgMsgPrint("Cls_GrDvrFs4Grp::Cls_GrDvrFs4Grp - disk have a sequence id problem.\n");
			while (0 != m_DiskCnt)
			{
				// correct
				if (LcRecIdRebuild())
				{
					break;
				}
				// rebuild table
				LcDiskTblBuild();
			}
		}

		if (0 != m_DiskCnt)
		{
			m_IsRecAble	=	TRUE;
		}
}
//--------------------------------------------------------------------
Cls_GrDvrFs4Grp::~Cls_GrDvrFs4Grp()
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Grp::LcDiskTblBuild( void )
{
	// local -------------------
		__u8	Tv_Mask;
		__u8	Tv_WkIdx;
		__u8	Tv_Status;
		//__u8	Tv_SubIdx;
		//__u32	Tv_RecId;
	// code --------------------
		// init
		m_IsRecAble			=	FALSE;

		m_RecSeqId			=	E_GrDvrFs4SeqIdNone;
		m_RecBdtIdx			=	0;
		m_RecDskIdx			=	E_GrDvrFs4RecDskIdxBad;

		// build disk table
		m_DiskCnt		=	0;
		Tv_Mask			=	1;
		for (Tv_WkIdx=0; Tv_WkIdx < E_GrDvrFs4MaxDriveCnt; Tv_WkIdx++)
		{
			if (0 != (Tv_Mask & m_DiskMap))
			{
				// add disk
				m_DiskTbl[m_DiskCnt]	=	((Cls_GrDvrFs4Mng*)m_FsMng)->RtlDiskObjGet(Tv_WkIdx);
				if (NULL != m_DiskTbl[m_DiskCnt])
				{
					Tv_Status	=	m_DiskTbl[m_DiskCnt]->DiskStatGet();
					// check status
					if (E_GrDvrRecStatEmpty <= Tv_Status)
					{
						m_DiskTbl[m_DiskCnt]->GrpIntIdxSet(m_DiskCnt);
						// regist
						m_DiskCnt ++;
					}
					else
					{
						m_DiskTbl[m_DiskCnt]	=	NULL;
					}
				}
				Tv_Mask	=	Tv_Mask << 1;
			}
		}
		
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs4Grp::DayMapGet(Def_GrTime A_TimeSt, Def_GrTime A_TimeEd)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_DskMap;
		__u8	Tv_DskIdx;
	// code --------------------
		Tv_Result	=	0;
		if ( 0 != m_DiskCnt )
		{
			for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx++ )
			{
				Tv_DskMap	=	m_DiskTbl[Tv_DskIdx]->DayMapGet(A_TimeSt, A_TimeEd,&m_GrpWorkEnv);
				Tv_Result	=	Tv_Result | Tv_DskMap;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Grp::GroupGetStatus(void)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_BadCnt;
		__u8	Tv_FullCnt;
		__u8	Tv_EmptyCnt;
		__u8	Tv_IntIdx;
		__u8	Tv_Stat;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;

		// check disk exist
		if ( 0 != m_DiskCnt )
		{
			if ( m_IsOvwtMode && !m_IsRecAble )
			{
				Tv_Result	=	E_GrDvrRecStatBad;
			}
			else
			{
				// check overwrite mode
				if ( m_IsOvwting )
				{
					Tv_Result	=	E_GrDvrRecStatOverwrite;
				}
				else
				{
					Tv_BadCnt		=	0;
					Tv_FullCnt	=	0;
					Tv_EmptyCnt	=	0;

					Tv_Result	=	E_GrDvrRecStatNormal;
					for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
					{
						Tv_Stat	=	m_DiskTbl[Tv_IntIdx]->DiskStatGet();
						if ( E_GrDvrRecStatEmpty > Tv_Stat )
						{
							Tv_BadCnt	++;
						}
						else if ( E_GrDvrRecStatEmpty == Tv_Stat )
						{
							Tv_EmptyCnt ++;
						}
						else if ( E_GrDvrRecStatFull == Tv_Stat )
						{
							Tv_FullCnt ++;
						}
					}
					// sum status
					if ( Tv_BadCnt == m_DiskCnt )
					{
						Tv_Result	=	E_GrDvrRecStatBad;
					}
					else if ( Tv_EmptyCnt == m_DiskCnt )
					{
						Tv_Result	=	E_GrDvrRecStatEmpty;
					}
					else if ( Tv_FullCnt == m_DiskCnt )
					{ 
						Tv_Result	=	E_GrDvrRecStatFull;
						if ( m_IsOvwtMode )
						{
							Tv_Result	=	E_GrDvrRecStatOverwrite;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Grp::RecGetSize(__u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		__u64	Tv_Total;
		__u64	Tv_RecSize;
		__u64	Tv_DskTot;
		__u64	Tv_DskRec;
		__u8	Tv_IntIdx;
	// code --------------------
		Tv_Total		=	0;
		Tv_RecSize	=	0;
		
		if ( 0 != m_DiskCnt )
		{
			for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
			{
				if ( m_DiskTbl[Tv_IntIdx]->RecSizeGet( &Tv_DskTot, &Tv_DskRec ) )
				{
					Tv_Total		=	Tv_Total + Tv_DskTot;
					Tv_RecSize	=	Tv_RecSize + Tv_DskRec;
				}
			}
		}

		*A_PtrRtTotal	=	Tv_Total;
		*A_PtrRtRec		=	Tv_RecSize;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::MinMapGet(Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, Ptr_GrDvrRecMinMap A_PtrRtBuf)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_IntIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( 0 != m_DiskCnt )
		{
			for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
			{
				if ( m_DiskTbl[Tv_IntIdx]->MinMapCombine( A_TimeSt,A_TimeEd,A_PtrRtBuf,&m_GrpWorkEnv) )
				{
					Tv_Result	=	TRUE;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Grp::ReadFw(Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, __u32 A_Ofs)
{
	// local -------------------
		__u8	Tv_Result;

		__u8	Tv_BufIdx;
		__u32	Tv_Ofs;
		__u32	Tv_BufSize;
		__u32	Tv_SeqId;
		__u32	Tv_Read;
		__u8	Tv_DskIdx;
		BOOL8	Tv_IsValid;
		__u8	Tv_RtCode;
		void*	Tv_PtrBuf;

	// code --------------------
		// init
		Tv_Result	=	E_GrDvrFs4RsltParamBad;

		// check disk count able
		if (0 != m_DiskCnt)
		{
			// get handle info
			if (E_GrDvrRecHndlNone != A_PtrCtrl->Hndl)
			{
				// init
				Tv_SeqId		=	0;
				// get now buffer information
				Tv_BufIdx		=	A_PtrCtrl->BufIdx;
				Tv_BufSize	=	A_PtrCtrl->Size;
				// get info
				Tv_Ofs			=	A_Ofs;
#ifdef E_GrDvrFs4GrpDbgMsgView
				if ( 0 != (Tv_Prgs & (~E_GrDvrFs4StrmAlignMask)) )		// debug
				{
					DbgMsgPrint( "Cls_GrDvrFs4Grp::ReadFw - bad progress(%8X).\n", Tv_Prgs );
				}
#endif
				// buffer change
				if (A_PtrCtrl->IsDblBuf)
				{
					Tv_BufIdx	=	Tv_BufIdx ^ 1;
				}
				
				Tv_PtrBuf		=	A_PtrCtrl->BufTbl[Tv_BufIdx].Ptr;

				do
				{
					// try read next data
					Tv_RtCode		=	E_GrDvrFs4RsltDiskBad;
					Tv_IsValid	=	FALSE;
					Tv_DskIdx		=	(__u8)(A_PtrCtrl->Hndl & E_GrDvrFs4HndlGrpIdxMask);
					if (Tv_DskIdx < m_DiskCnt)
					{
						Tv_RtCode	=	m_DiskTbl[Tv_DskIdx]->ReadByHndlFw(A_PtrCtrl->Hndl, Tv_Ofs, Tv_PtrBuf, Tv_BufSize, &Tv_Read, &Tv_SeqId, &A_PtrCtrl->Hndl);
					}

					if (E_GrDvrFs4RsltOk == Tv_RtCode)
					{
						// success
						A_PtrCtrl->BufIdx	=	Tv_BufIdx;
						A_PtrCtrl->BufTbl[Tv_BufIdx].Prgs		=	0;
						A_PtrCtrl->BufTbl[Tv_BufIdx].Readed	=	Tv_Read;

						Tv_Result	=	E_GrDvrFs4RsltOk;
						break;
					}

					// find next block
					Tv_SeqId ++;
					Tv_Ofs		=	0;

					if (!LcHndlFindSeqIdFw(Tv_SeqId, &A_PtrCtrl->Hndl, &Tv_SeqId))
					{
						Tv_Result		=	E_GrDvrFs4RsltEndOfPos;
						break;
					}

				} while (TRUE);

			}	//	if ( E_GrDvrRecHndlNone != A_PtrCtrl->Hndl )
		}	//	if ( 0 != m_DiskCnt )

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Grp::ReadBegin(Ptr_GrDvrFs4SeekCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
		__u64	Tv_Hndl;
		__u32	Tv_SeqId;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4RsltParamBad;

		if (LcHndlFindFirst(&Tv_Hndl, &Tv_SeqId))
		{
			A_PtrCtrl->Hndl	=	Tv_Hndl;
			Tv_Result	=	ReadFw(A_PtrCtrl, 0);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::LcRecIdChkSelfOvlp(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u32	Tv_BdtSr;
		__u32	Tv_BdtTg;
		Ptr_GrDvrFs4Bdt	Tv_PtrBdt;
	// code --------------------
		Tv_Result	=	TRUE;

		if (0 < m_DiskCnt)
		{
			for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				Tv_PtrBdt	=	m_DiskTbl[Tv_DskIdx]->BdtGetTable();
				for (Tv_BdtSr = 0; Tv_BdtSr < (E_GrDvrFs4BdtMaxCnt - 1);Tv_BdtSr++)
				{
					// check able
					if ((0 != Tv_PtrBdt[Tv_BdtSr].SeqId) && (E_GrDvrFs4BdtStatBad != Tv_PtrBdt[Tv_BdtSr].Stat))
					{
						for (Tv_BdtTg =Tv_BdtSr + 1; Tv_BdtTg < E_GrDvrFs4BdtMaxCnt;Tv_BdtTg++)
						{
							if ((0 != Tv_PtrBdt[Tv_BdtTg].SeqId) && (E_GrDvrFs4BdtStatBad != Tv_PtrBdt[Tv_BdtTg].Stat))
							{
								// check compare
								if (Tv_PtrBdt[Tv_BdtTg].SeqId == Tv_PtrBdt[Tv_BdtSr].SeqId)
								{
									// overlapped found
									Tv_Result	=	FALSE;
									break;
								}
							}
						}
					}
					if (!Tv_Result)
					{
						break;
					}
				}
				if (!Tv_Result)
				{
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::LcRecIdChkRefOvlp(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskSr;
		__u8	Tv_DskTg;
		__u32	Tv_BdtSr;
		__u32	Tv_BdtTg;
		Ptr_GrDvrFs4Bdt	Tv_PtrBdtTbl[E_GrDvrFs4MaxDriveCnt];
	// code --------------------
		Tv_Result	=	TRUE;

		// check group defective
		if ( 1 < m_DiskCnt )
		{
			// get BDT table list
			for (Tv_DskSr=0; Tv_DskSr < m_DiskCnt; Tv_DskSr++)
			{
				Tv_PtrBdtTbl[Tv_DskSr]	=	m_DiskTbl[Tv_DskSr]->BdtGetTable();
			}
			// check overlapped
			for (Tv_DskSr=0; Tv_DskSr < (m_DiskCnt - 1); Tv_DskSr++)
			{
				for (Tv_DskTg=Tv_DskSr + 1; Tv_DskTg < m_DiskCnt; Tv_DskTg++)
				{
					for (Tv_BdtSr = 0; Tv_BdtSr < E_GrDvrFs4BdtMaxCnt; Tv_BdtSr++)
					{
						// check able source
						if ((0 != Tv_PtrBdtTbl[Tv_DskSr][Tv_BdtSr].SeqId) && (E_GrDvrFs4BdtStatBad != Tv_PtrBdtTbl[Tv_DskSr][Tv_BdtSr].Stat))
						{
							for (Tv_BdtTg =0; Tv_BdtTg < E_GrDvrFs4BdtMaxCnt; Tv_BdtTg++)
							{
								if ((0 != Tv_PtrBdtTbl[Tv_DskTg][Tv_BdtTg].SeqId) && (E_GrDvrFs4BdtStatBad != Tv_PtrBdtTbl[Tv_DskTg][Tv_BdtTg].Stat))
								{
									// check compare
									if (Tv_PtrBdtTbl[Tv_DskTg][Tv_BdtTg].SeqId == Tv_PtrBdtTbl[Tv_DskSr][Tv_BdtSr].SeqId)
									{
										// overlapped found
										Tv_Result	=	FALSE;
										break;
									}
								}
							}	//	for (Tv_BdtTg =0; Tv_BdtTg < E_GrDvrFs4BdtMaxCnt; Tv_BdtTg++)
						}
						if (!Tv_Result)
						{
							break;
						}
					}	//	for (Tv_BdtSr == 0; Tv_BdtSr < E_GrDvrFs4BdtMaxCnt; Tv_BdtSr++)
					if (!Tv_Result)
					{
						break;
					}

				}	//	for (Tv_DskTg=Tv_DskSr + 1; Tv_DskTg < m_DiskCnt; Tv_DskTg++)
				if (!Tv_Result)
				{
					break;
				}

			}	//	for (Tv_DskSr=0; Tv_DskSr < (m_DiskCnt - 1); Tv_DskSr++)
		}	//	if ( 1 < m_DiskCnt )

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Grp::LcRecBlockUpdt(void)
{
	// local -------------------
	// code --------------------
		m_RecSeqId	=	E_GrDvrFs4SeqIdNone;
		m_RecDskIdx	=	0;
		m_RecBdtIdx	=	0;
		if ( m_IsRecAble )
		{
			// check disk count
			if ( 0 != m_DiskCnt )
			{
				// get last record id
				m_RecSeqId	=	LcBlockFindLast(&m_RecDskIdx, &m_RecBdtIdx);
				// record able block not found
				if ( E_GrDvrFs4SeqIdNone == m_RecSeqId)
				{
					// find empty block
					if (LcBlockFindEmpty(&m_RecDskIdx,&m_RecBdtIdx))
					{
						m_RecSeqId ++;
					}
				}
			}	//	if ( 0 != m_DiskCnt )
		}	//	if ( m_IsRecAble )
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::LcRecIdRebuild(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsWork;
		Ptr_GrDvrFs4Bdt	Tv_PtrBdtTbl[E_GrDvrFs4MaxDriveCnt];
		__u32	Tv_BdtCntTbl[E_GrDvrFs4MaxDriveCnt];
		__u8	Tv_DskIdx;
		__u32	Tv_BdtIdx;

		__u32	Tv_FndTime;	// fonud time
		__u32	Tv_FndBdt;	// found bdt
		__u8	Tv_FndDsk;	// found disk

		__u32	Tv_LastTime;	// last time
		__u32	Tv_LastBdt;		// last bdt
		__u8	Tv_LastDsk;		// last disk
		__u32	Tv_LastSeqId;

		__u32	Tv_SeqId;

	// code --------------------
		// init
		Tv_Result		=	TRUE;

		m_RecSeqId	=	0;

		if (0 != m_DiskCnt)
		{
			// get BDT table list
			for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				Tv_PtrBdtTbl[Tv_DskIdx]	=	m_DiskTbl[Tv_DskIdx]->BdtGetTable();
				Tv_BdtCntTbl[Tv_DskIdx]	=	m_DiskTbl[Tv_DskIdx]->BlockGetCount();
			}

			// mark need rebuild block
			for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				if (0 != Tv_BdtCntTbl[Tv_DskIdx])
				{
					for (Tv_BdtIdx=0; Tv_BdtIdx < Tv_BdtCntTbl[Tv_DskIdx]; Tv_BdtIdx++)
					{
						if ((0 != Tv_PtrBdtTbl[Tv_DskIdx][Tv_BdtIdx].SeqId) && (E_GrDvrFs4BdtStatBad != Tv_PtrBdtTbl[Tv_DskIdx][Tv_BdtIdx].Stat))
						{
							if ((0 != Tv_PtrBdtTbl[Tv_DskIdx][Tv_BdtIdx].TimeSt) && (0 != Tv_PtrBdtTbl[Tv_DskIdx][Tv_BdtIdx].TimeEd))
							{
								Tv_PtrBdtTbl[Tv_DskIdx][Tv_BdtIdx].IsNrib	=	TRUE;
							}
							else
							{
								DbgMsgPrint("Cls_GrDvrFs4Grp::LcRecIdRebuild - disk %d block %d time bad.\n", m_DiskTbl[Tv_DskIdx]->DevIdxGet(), Tv_BdtIdx);
								m_DiskTbl[Tv_DskIdx]->BlockDelete(Tv_BdtIdx,&m_GrpWorkEnv);
							}
						}
						else
						{
							Tv_PtrBdtTbl[Tv_DskIdx][Tv_BdtIdx].IsNrib	=	FALSE;
						}
					}
				}
			}

			// sort and rebuild
			Tv_LastTime	=	0;
			Tv_SeqId	=	1;
			do
			{
				Tv_IsWork		=	FALSE;
				Tv_FndTime	=	0;
				// find lowest time
				for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
				{
					if (0 != Tv_BdtCntTbl[Tv_DskIdx])
					{
						for (Tv_BdtIdx=0; Tv_BdtIdx < Tv_BdtCntTbl[Tv_DskIdx]; Tv_BdtIdx++)
						{
							if (Tv_PtrBdtTbl[Tv_DskIdx][Tv_BdtIdx].IsNrib)
							{
								// check first
								if (0 == Tv_FndTime)
								{
									Tv_FndTime	=	Tv_PtrBdtTbl[Tv_DskIdx][Tv_BdtIdx].TimeSt;
									Tv_FndDsk		=	Tv_DskIdx;
									Tv_FndBdt		=	Tv_BdtIdx;
								}
								else 
								{
									if (Tv_PtrBdtTbl[Tv_DskIdx][Tv_BdtIdx].TimeSt < Tv_FndTime)
									{
										Tv_FndTime	=	Tv_PtrBdtTbl[Tv_DskIdx][Tv_BdtIdx].TimeSt;
										Tv_FndDsk		=	Tv_DskIdx;
										Tv_FndBdt		=	Tv_BdtIdx;
									}
									Tv_IsWork		=	TRUE;
								}
							}
						}
					}
				}

				// update 
				if (0 != Tv_FndTime)
				{
					Tv_PtrBdtTbl[Tv_FndDsk][Tv_FndBdt].SeqId	=	Tv_SeqId;
					Tv_PtrBdtTbl[Tv_FndDsk][Tv_FndBdt].IsNrib	=	FALSE;

					Tv_LastTime		=	Tv_FndTime;
					Tv_LastDsk		=	Tv_FndDsk;
					Tv_LastBdt		=	Tv_FndBdt;
					Tv_LastSeqId	=	Tv_SeqId;
				}

				// next
				Tv_SeqId ++;
			} while (Tv_IsWork);

			// save BDT
			for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				if (!m_DiskTbl[Tv_DskIdx]->BdtSave())
				{
					Tv_Result	=	FALSE;
					break;
				}
			}

			if (Tv_Result && (0 != Tv_LastTime))
			{
				m_RecDskIdx	=	Tv_LastDsk;
				m_RecBdtIdx	=	Tv_LastBdt;
				m_RecSeqId	=	Tv_LastSeqId;
			}
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::LcBlockFindEmpty( __u8* A_PtrRtDskIdx, __u32* A_PtrRtBlkIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if (0 != m_DiskCnt)
		{
			for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				if (m_DiskTbl[Tv_DskIdx]->BlockFindEmpty(&Tv_BlkIdx))
				{
					Tv_Result	=	TRUE;
					*A_PtrRtDskIdx	=	Tv_DskIdx;
					*A_PtrRtBlkIdx	=	Tv_BlkIdx;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs4Grp::LcBlockFindFirst(__u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx)
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_IntIdx;
		__u32	Tv_BlkIdx;
		__u32	Tv_SeqId;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4SeqIdNone;

		if (0 != m_DiskCnt)
		{
			for (Tv_IntIdx=0; Tv_IntIdx < m_DiskCnt; Tv_IntIdx++)
			{
				Tv_SeqId	=	m_DiskTbl[Tv_IntIdx]->BlockFindFirst(&Tv_BlkIdx);
				if (E_GrDvrFs4SeqIdNone != Tv_SeqId)
				{
					if (E_GrDvrFs4SeqIdNone == Tv_Result)
					{
						Tv_Result				=	Tv_SeqId;
						*A_PtrRtBlkIdx	=	Tv_BlkIdx;
						*A_PtrRtIntIdx	=	Tv_IntIdx;
					}
					else if (Tv_Result > Tv_SeqId)
					{
						Tv_Result				=	Tv_SeqId;
						*A_PtrRtBlkIdx	=	Tv_BlkIdx;
						*A_PtrRtIntIdx	=	Tv_IntIdx;
					}
				}
			}	//	for ( Tv_IntIdx=1;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs4Grp::LcBlockFindLast(__u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx)
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_IntIdx;
		__u32	Tv_BlkIdx;
		__u32	Tv_SeqId;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4SeqIdNone;

		if (0 != m_DiskCnt)
		{
			for (Tv_IntIdx=0; Tv_IntIdx < m_DiskCnt; Tv_IntIdx++)
			{
				Tv_SeqId	=	m_DiskTbl[Tv_IntIdx]->BlockFindLast(&Tv_BlkIdx);
				if (Tv_Result < Tv_SeqId)
				{
					Tv_Result				=	Tv_SeqId;
					*A_PtrRtBlkIdx	=	Tv_BlkIdx;
					*A_PtrRtIntIdx	=	Tv_IntIdx;
				}
			}	//	for ( Tv_IntIdx=1;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Grp::LcLogRefNext( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		__u32	Tv_RefIdx;
	// code --------------------
		for ( Tv_RefIdx=0;Tv_RefIdx < A_PtrCtrl->RefCnt;Tv_RefIdx++ )
		{
			if ( 0 != A_PtrCtrl->PtrRef[Tv_RefIdx].Time )
			{
				GrDvrLogGetFw( A_PtrCtrl, &A_PtrCtrl->PtrRef[Tv_RefIdx] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Grp::LcLogRefPrev( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		__u32	Tv_RefIdx;
	// code --------------------
		for ( Tv_RefIdx=0;Tv_RefIdx < A_PtrCtrl->RefCnt;Tv_RefIdx++ )
		{
			if ( 0 != A_PtrCtrl->PtrRef[Tv_RefIdx].Time )
			{
				GrDvrLogGetBw( A_PtrCtrl, &A_PtrCtrl->PtrRef[Tv_RefIdx] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Grp::LcLogSort( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		Ptr_GrDvrLogItm	Tv_PtrBufOrg;
		__u32	Tv_OrgInCnt;
		__u32	Tv_FindIdx;
		St_GrDvrLogItm	Tv_TmpBuf;
	// code --------------------
		// check sort able
		if ( A_PtrCtrl->IsSort && (1 < A_PtrCtrl->InCnt) )
		{
			// backup original buffer
			Tv_PtrBufOrg	=	A_PtrCtrl->PtrBuf;
			Tv_OrgInCnt		=	A_PtrCtrl->InCnt;

			// check
			while ( 1 < A_PtrCtrl->InCnt )
			{
				// find highest
				Tv_FindIdx	=	GrDvrLogCtlFindHigh( A_PtrCtrl );
				// move position
				if ( 0 != Tv_FindIdx )
				{
					GrDumyCopyMem( &Tv_TmpBuf, A_PtrCtrl->PtrBuf, sizeof(St_GrDvrLogItm) );	// backup top item
					GrDumyCopyMem( A_PtrCtrl->PtrBuf, &A_PtrCtrl->PtrBuf[Tv_FindIdx], sizeof(St_GrDvrLogItm) );	// backup sort item
					GrDumyCopyMem( &A_PtrCtrl->PtrBuf[Tv_FindIdx], &Tv_TmpBuf, sizeof(St_GrDvrLogItm) );	// exchange old item
				}
				// go next
				A_PtrCtrl->InCnt --;
				A_PtrCtrl->PtrBuf	=	(Ptr_GrDvrLogItm)( (__u32)A_PtrCtrl->PtrBuf + sizeof(St_GrDvrLogItm) );
			}

			// restore original pointer
			A_PtrCtrl->PtrBuf	=	Tv_PtrBufOrg;
			A_PtrCtrl->InCnt	=	Tv_OrgInCnt;
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Grp::LogFindStart( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		__u8	Tv_DskIdx;
	// code --------------------
		// init
		A_PtrCtrl->InCnt				=	0;
		A_PtrCtrl->Total				=	0;
		A_PtrCtrl->UpCnt				=	0;
		A_PtrCtrl->RtlSt.Time		=	0;
		A_PtrCtrl->RtlSt.Type		=	0;
		A_PtrCtrl->RtlEd.Time		=	0;
		A_PtrCtrl->RtlEd.Type		=	0;
		A_PtrCtrl->Base.Time		=	A_PtrCtrl->TimeEd;
		A_PtrCtrl->Base.Type		=	0;
		A_PtrCtrl->IsStart			=	TRUE;
		A_PtrCtrl->IsEnd				=	TRUE;

		if ( 0 != m_DiskCnt )
		{
			for ( Tv_DskIdx=0;Tv_DskIdx < (__u8)m_DiskCnt;Tv_DskIdx++ )
			{
				m_DiskTbl[Tv_DskIdx]->LogGetNext( A_PtrCtrl );
			}
			// reference
			if ( (NULL != A_PtrCtrl->PtrRef) && (0 != A_PtrCtrl->RefCnt) )
			{
				LcLogRefNext( A_PtrCtrl );
			}
			LcLogSort( A_PtrCtrl );
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Grp::LogFindNext( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		__u8	Tv_DskIdx;
	// code --------------------
		// check next able
		if ( !A_PtrCtrl->IsEnd )
		{
			// get base
			A_PtrCtrl->Base.Time	=	A_PtrCtrl->RtlSt.Time;
			A_PtrCtrl->Base.Type	=	A_PtrCtrl->RtlSt.Type;

			if ( 0xFF == A_PtrCtrl->Base.Type )
			{
				A_PtrCtrl->Base.Type	=	0;
				A_PtrCtrl->Base.Time --;
			}
			else
			{
				A_PtrCtrl->Base.Type ++;
			}

			// init
			A_PtrCtrl->InCnt		=	0;
			A_PtrCtrl->Total		=	0;
			A_PtrCtrl->UpCnt		=	0;

			A_PtrCtrl->RtlSt.Time	=	0;
			A_PtrCtrl->RtlSt.Type	=	0;
			A_PtrCtrl->RtlEd.Time	=	0;
			A_PtrCtrl->RtlEd.Type	=	0;

			A_PtrCtrl->IsStart	=	FALSE;
			A_PtrCtrl->IsEnd		=	TRUE;

			for ( Tv_DskIdx=0;Tv_DskIdx < (__u8)m_DiskCnt;Tv_DskIdx++ )
			{
				m_DiskTbl[Tv_DskIdx]->LogGetNext( A_PtrCtrl );
			}
			// reference
			if ( (NULL != A_PtrCtrl->PtrRef) && (0 != A_PtrCtrl->RefCnt) )
			{
				LcLogRefNext( A_PtrCtrl );
			}

			// sort
			LcLogSort( A_PtrCtrl );
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Grp::LogFindPrev( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		__u8	Tv_DskIdx;
	// code --------------------
		// check next able
		if ( !A_PtrCtrl->IsStart )
		{
			// get base
			A_PtrCtrl->Base.Time	=	A_PtrCtrl->RtlEd.Time;
			A_PtrCtrl->Base.Type	=	A_PtrCtrl->RtlEd.Type;

			if ( 0 == A_PtrCtrl->Base.Type )
			{
				A_PtrCtrl->Base.Type	=	0xFF;
				A_PtrCtrl->Base.Time ++;
			}
			else
			{
				A_PtrCtrl->Base.Type --;
			}

			// init
			A_PtrCtrl->InCnt		=	0;
			A_PtrCtrl->Total		=	0;
			A_PtrCtrl->UpCnt		=	0;

			A_PtrCtrl->RtlSt.Time	=	0;
			A_PtrCtrl->RtlSt.Type	=	0;
			A_PtrCtrl->RtlEd.Time	=	0;
			A_PtrCtrl->RtlEd.Type	=	0;

			A_PtrCtrl->IsStart	=	TRUE;
			A_PtrCtrl->IsEnd		=	FALSE;

			for ( Tv_DskIdx=0;Tv_DskIdx < (__u8)m_DiskCnt;Tv_DskIdx++ )
			{
				m_DiskTbl[Tv_DskIdx]->LogGetPrev( A_PtrCtrl );
			}

			// reference
			if ( (NULL != A_PtrCtrl->PtrRef) && (0 != A_PtrCtrl->RefCnt) )
			{
				LcLogRefPrev( A_PtrCtrl );
			}
			// sort
			LcLogSort( A_PtrCtrl );
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::LogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( 0 != m_DiskCnt )
		{
			Tv_DskIdx	=	0;
			if ( E_GrDvrFs4SeqIdNone != m_RecSeqId )
			{
				Tv_DskIdx	=	m_RecDskIdx;
			}
			Tv_Result	=	m_DiskTbl[Tv_DskIdx]->LogAdd(A_PtrItem);

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Grp::RecGetTimeRng(Ptr_GrTime A_PtrRtTimeSt, Ptr_GrTime A_PtrRtTimeEd)
{
	// local -------------------
		Def_GrTime	Tv_TimeSt;
		Def_GrTime	Tv_TimeEd;
		__u8	Tv_DskIdx;
		Def_GrTime	Tv_LcSt;
		Def_GrTime	Tv_LcEd;
	// code --------------------
		Tv_TimeSt	=	0;
		Tv_TimeEd	=	0;
		if ( 0 != m_DiskCnt )
		{
			for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx++ )
			{
				// get disk time
				Tv_LcSt	=	m_DiskTbl[Tv_DskIdx]->BeginTimeGet();
				Tv_LcEd	=	m_DiskTbl[Tv_DskIdx]->EndTimeGet();
				// update begin
				if ( 0 != Tv_LcSt )
				{
					if ( 0 == Tv_TimeSt )
					{
						Tv_TimeSt	=	Tv_LcSt;
					}
					else
					{
						if ( Tv_LcSt < Tv_TimeSt )
						{
							Tv_TimeSt	=	Tv_LcSt;
						}
					}
				}
				// update end time
				if ( 0 != Tv_LcEd )
				{
					if ( 0 == Tv_TimeEd )
					{
						Tv_TimeEd	=	Tv_LcEd;
					}
					else
					{
						if ( Tv_LcEd > Tv_TimeEd )
						{
							Tv_TimeEd	=	Tv_LcEd;
						}
					}
				}
			}
		}

		// update result
		*A_PtrRtTimeSt	=	Tv_TimeSt;
		*A_PtrRtTimeEd	=	Tv_TimeEd;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::IsRecAble( void )
{
		return	m_IsRecAble;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Grp::GroupFormat(BOOL8 A_IsWithLog)
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_MaskDsk;
		Cls_GrDvrFs4Disk*	Tv_ObjDsk;
	// code --------------------
		
		if (0 != m_DiskMap)
		{
			Tv_MaskDsk	=	1;
			for (Tv_WkIdx = 0; Tv_WkIdx < E_GrDvrFs4MaxDriveCnt; Tv_WkIdx++)
			{
				if (0 != (Tv_MaskDsk & m_DiskMap) )
				{
					Tv_ObjDsk	=	((Cls_GrDvrFs4Mng*)m_FsMng)->RtlDiskObjGet(Tv_WkIdx);
					if (NULL != Tv_ObjDsk)
					{
						Tv_ObjDsk->Format(A_IsWithLog);
					}
				}
				// next
				Tv_MaskDsk	=	Tv_MaskDsk << 1;
			}
			GrDumyZeroMem(&m_GrpWorkEnv, sizeof(m_GrpWorkEnv));	// reset cache
			LcDiskTblBuild();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::LcBlockDeleteOldest(__u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_SeqId;
		__u8	Tv_DskIdx;
		__u32	Tv_BlkId;
	// code --------------------
		Tv_Result	=	FALSE;

		do
		{
			Tv_SeqId	=	LcBlockFindFirst(&Tv_DskIdx, &Tv_BlkId);
			if (E_GrDvrFs4SeqIdNone == Tv_SeqId)
			{
				break;
			}
			// delete block
			if (m_DiskTbl[Tv_DskIdx]->BlockDelete(Tv_BlkId,&m_GrpWorkEnv))
			{
				// update value
				*A_PtrRtIntIdx	=	Tv_DskIdx;
				*A_PtrRtBlkIdx	=	Tv_BlkId;
				Tv_Result	=	TRUE;
			}
		} while (!Tv_Result);

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::RecAddData(Ptr_GrDvrFs4RecReqInfo A_PtrReq)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
		BOOL8	Tv_IsAble;
		__u8	Tv_DskIdx;
		__u32	Tv_BlkId;
		__u32	Tv_FrmIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if (m_IsRecAble)
		{
			// check record block
			if (E_GrDvrFs4SeqIdNone == m_RecSeqId)
			{
				LcRecBlockUpdt();
			}

			// check record able
			if (E_GrDvrFs4SeqIdNone != m_RecSeqId)
			{
				Tv_FrmIdx	=	0;
				while (Tv_FrmIdx < A_PtrReq->FrmCnt)
				{
					// write last block
					Tv_Rval	=	m_DiskTbl[m_RecDskIdx]->BlockAddData(m_RecBdtIdx, m_RecSeqId, Tv_FrmIdx, A_PtrReq, &Tv_FrmIdx, &m_GrpWorkEnv);
					if (E_GrDvrFs4RsltOk == Tv_Rval)
					{
						Tv_Result	=	TRUE;
						if (Tv_FrmIdx != A_PtrReq->FrmCnt)
						{
							DbgMsgPrint("Cls_GrDvrFs4Grp::RecDataAdd - [BUG!] incompleted requirement process.\n");
						}
						break;
					}
					// check block capacity is full
					if ((E_GrDvrFs4RsltNotEnoughSpace == Tv_Rval) || (E_GrDvrFs4RsltParamBad == Tv_Rval))
					{
						if (E_GrDvrFs4RsltParamBad == Tv_Rval)
						{
							DbgMsgPrint("Cls_GrDvrFs4Grp::RecDataAdd - [BUG!] Unknown parametter error.\n");
						}
						// set block status to full
						m_DiskTbl[m_RecDskIdx]->BlockSetStatFull(m_RecBdtIdx);
					}
					// go next sequence id
					if (m_RecSeqId >= 0xFFFFFFFE)
					{
						// rebuild record id
						if (!LcRecIdRebuild())
						{
							break;
						}
						if (E_GrDvrFs4SeqIdNone == m_RecSeqId)
						{
							LcRecBlockUpdt();
						}
						if (E_GrDvrFs4SeqIdNone == m_RecSeqId)
						{
							break;
						}
					}
					else
					{
						// next sequence id
						m_RecSeqId ++;
					}
					// find record able block
					Tv_IsAble	=	FALSE;
					// find empty block
					if (LcBlockFindEmpty(&Tv_DskIdx, &Tv_BlkId))
					{
						// update record index
						m_RecDskIdx	=	Tv_DskIdx;
						m_RecBdtIdx	=	Tv_BlkId;
						Tv_IsAble		=	TRUE;
					}
					else 
					{
						// disk full
						if (m_IsOvwtMode)
						{
							// delete oldest block
							if (LcBlockDeleteOldest(&Tv_DskIdx, &Tv_BlkId))
							{
								m_RecDskIdx	=	Tv_DskIdx;
								m_RecBdtIdx	=	Tv_BlkId;
								m_IsOvwting	=	TRUE;				// over writing
								Tv_IsAble		=	TRUE;
							}
						}
						else
						{
							// notify disk full
							m_IsRecAble	=	FALSE;
							GrTaskSvrMsgErrorNotify(E_GrErrDiskFull, m_GrpType);
						}
					}

					// check write able
					if (!Tv_IsAble)
					{
						// can not record
						break;
					}
				}	//	while (Tv_FrmIdx < A_PtrReq->FrmCnt)

			}
			else	//	if (E_GrDvrFs4SeqIdNone != m_RecSeqId)
			{
				DbgMsgPrint("Cls_GrDvrFs4Grp::RecDataAdd - can not find record able block.\n");
			}
		}	//	if (m_IsRecAble)

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::LcHndlFindSeqIdFw(__u32 A_SeqId, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u64	Tv_FndHndl;
		__u32	Tv_FndSeqId;
		__u32	Tv_NowSeqId;
		__u64	Tv_NowHndl;

	// code --------------------
		Tv_Result		=	FALSE;

		if ((0 != m_DiskCnt) && (E_GrDvrFs4SeqIdNone != A_SeqId))
		{
			Tv_FndSeqId	=	0;
			Tv_FndHndl	=	E_GrDvrRecHndlNone;

			for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt;Tv_DskIdx++)
			{
				if (m_DiskTbl[Tv_DskIdx]->HndlFindSeqIdFw(A_SeqId,&Tv_NowHndl,&Tv_NowSeqId))
				{
					if (A_SeqId == Tv_NowSeqId)
					{
						// found match id
						Tv_FndSeqId		=	Tv_NowSeqId;
						Tv_FndHndl		=	Tv_NowHndl;
						Tv_Result			=	TRUE;
						break;
					}
					if (Tv_Result)
					{
						if (Tv_FndSeqId > Tv_NowSeqId)
						{
							Tv_FndSeqId		=	Tv_NowSeqId;
							Tv_FndHndl		=	Tv_NowHndl;
						}
					}
					else
					{
						Tv_FndSeqId		=	Tv_NowSeqId;
						Tv_FndHndl		=	Tv_NowHndl;
						Tv_Result			=	TRUE;
					}
				}
			}

			if (Tv_Result)
			{
				*A_PtrRtHndl	=	Tv_FndHndl;
				*A_PtrRtSeqId	=	Tv_FndSeqId;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Grp::ReadTimeFw(Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
		__u64	Tv_Hndl;
		__u32	Tv_SeqId;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4RsltParamBad;

		if (LcHndlFindTimeFw(A_Time,&Tv_Hndl,&Tv_SeqId))
		{
			A_PtrCtrl->Hndl	=	Tv_Hndl;
			Tv_Result	=	ReadFw(A_PtrCtrl, 0);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::LcHndlFindFirst(__u64* A_PtrRtHndl, __u32* A_PtrRtSeqId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u64	Tv_FndHndl;
		__u32	Tv_FndSeqId;
		__u32	Tv_NowSeqId;
		__u64	Tv_NowHndl;
	// code --------------------
		Tv_Result		=	FALSE;

		if (0 != m_DiskCnt)
		{
			Tv_FndSeqId	=	0;
			Tv_FndHndl	=	E_GrDvrRecHndlNone;

			for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				if (m_DiskTbl[Tv_DskIdx]->HndlFindFirst(&Tv_NowHndl, &Tv_NowSeqId))
				{
					if (Tv_Result)
					{
						if (Tv_FndSeqId > Tv_NowSeqId)
						{
							Tv_FndSeqId		=	Tv_NowSeqId;
							Tv_FndHndl		=	Tv_NowHndl;
						}
					}
					else
					{
						Tv_FndSeqId		=	Tv_NowSeqId;
						Tv_FndHndl		=	Tv_NowHndl;
						Tv_Result			=	TRUE;
					}
				}
			}

			if (Tv_Result)
			{
				*A_PtrRtHndl	=	Tv_FndHndl;
				*A_PtrRtSeqId	=	Tv_FndSeqId;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::LcHndlFindTimeFw(Def_GrTime A_Time, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u64	Tv_NowHndl;
		__u32	Tv_NowSeqId;
		__u32	Tv_NowTmGap;
		__u64	Tv_FndHndl;
		__u32	Tv_FndSeqId;
		__u32	Tv_FndTmGap;
	// code --------------------
		// init
		Tv_Result	=	FALSE;
		// check disk count able
		if (0 != m_DiskCnt)
		{
			// init
			Tv_FndSeqId	=	0;
			// find time
			for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				if (m_DiskTbl[Tv_DskIdx]->HndlFindTimeFw(A_Time, &Tv_NowHndl, &Tv_NowSeqId, &Tv_NowTmGap, &m_GrpWorkEnv))
				{
					// check found
					if (0 == Tv_NowTmGap)
					{
						Tv_FndHndl	=	Tv_NowHndl;
						Tv_FndSeqId	=	Tv_NowSeqId;
						Tv_FndTmGap	=	Tv_NowTmGap;
						break;
					}
					if (0 == Tv_FndSeqId)
					{
						Tv_FndHndl	=	Tv_NowHndl;
						Tv_FndSeqId	=	Tv_NowSeqId;
						Tv_FndTmGap	=	Tv_NowTmGap;
					}
					else if (Tv_NowTmGap < Tv_FndTmGap)
					{
						Tv_FndHndl	=	Tv_NowHndl;
						Tv_FndSeqId	=	Tv_NowSeqId;
						Tv_FndTmGap	=	Tv_NowTmGap;
					}
				}
			}	//	for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt;Tv_DskIdx++)

			if (0 != Tv_FndSeqId)
			{
				*A_PtrRtHndl	=	Tv_FndHndl;
				*A_PtrRtSeqId	=	Tv_FndSeqId;
				Tv_Result			=	TRUE;
			}

		}	//	if ( 0 != m_DiskCnt )

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Grp::ReadBw(Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, __u32 A_Ofs)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_BufIdx;
		__u32	Tv_Ofs;
		__u32	Tv_BufSize;
		__u32	Tv_SeqId;
		__u32	Tv_Read;
		__u8	Tv_DskIdx;
		BOOL8	Tv_IsValid;
		__u8	Tv_RtCode;
		void*	Tv_PtrBuf;
	// code --------------------
		// init
		Tv_Result	=	E_GrDvrFs4RsltParamBad;

		// check disk count able
		if (0 != m_DiskCnt)
		{
			// get handle info
			if (E_GrDvrRecHndlNone != A_PtrCtrl->Hndl)
			{
				// init
				Tv_SeqId		=	0xFFFFFFFF;
				// get now buffer information
				Tv_BufIdx		=	A_PtrCtrl->BufIdx;
				Tv_BufSize	=	A_PtrCtrl->Size;
				// get info
				Tv_Ofs			=	A_Ofs;
#ifdef E_GrDvrFs4GrpDbgMsgView
				if (0 != (Tv_Prgs & (~E_GrDvrFs4StrmAlignMask)))		// debug
				{
					DbgMsgPrint("Cls_GrDvrFs4Grp::ReadBw - bad progress(%8X).\n", Tv_Prgs);
				}
#endif
				// buffer change
				if (A_PtrCtrl->IsDblBuf)
				{
					Tv_BufIdx	=	Tv_BufIdx ^ 1;
				}
				Tv_PtrBuf		=	A_PtrCtrl->BufTbl[Tv_BufIdx].Ptr;

				do
				{
					// try read next data
					Tv_RtCode		=	E_GrDvrFs4RsltDiskBad;
					Tv_IsValid	=	FALSE;
					Tv_DskIdx		=	(__u8)(A_PtrCtrl->Hndl & E_GrDvrFs4HndlGrpIdxMask);
					if (Tv_DskIdx < m_DiskCnt)
					{
						Tv_RtCode	=	m_DiskTbl[Tv_DskIdx]->ReadByHndlBw(A_PtrCtrl->Hndl, Tv_Ofs, Tv_PtrBuf, Tv_BufSize, &Tv_Read, &Tv_SeqId,&A_PtrCtrl->Hndl);
					}

					if (E_GrDvrFs4RsltOk == Tv_RtCode)
					{
						// success
						A_PtrCtrl->BufIdx	=	Tv_BufIdx;
						A_PtrCtrl->BufTbl[Tv_BufIdx].Prgs		=	Tv_Read;
						A_PtrCtrl->BufTbl[Tv_BufIdx].Readed	=	Tv_Read;
						Tv_Result	=	E_GrDvrFs4RsltOk;
						break;
					}

					// find next block
					Tv_SeqId --;
					Tv_Ofs		=	E_GrDvrFs4BlockRecAreaSize;

					if (!LcHndlFindSeqIdBw(Tv_SeqId, &A_PtrCtrl->Hndl, &Tv_SeqId))
					{
						Tv_Result		=	E_GrDvrFs4RsltEndOfPos;
						break;
					}

				} while (TRUE);

			}	//	if ( E_GrDvrRecHndlNone != A_PtrCtrl->Hndl )
		}	//	if ( 0 != m_DiskCnt )

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::LcHndlFindSeqIdBw(__u32 A_SeqId, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u64	Tv_FndHndl;
		__u32	Tv_FndSeqId;
		__u32	Tv_NowSeqId;
		__u64	Tv_NowHndl;
	// code --------------------
		Tv_Result		=	FALSE;

		if ((0 != m_DiskCnt) && (E_GrDvrFs4SeqIdNone != A_SeqId) && (E_GrDvrFs4SeqIdMax >= A_SeqId))
		{
			Tv_FndSeqId	=	0;
			Tv_FndHndl	=	E_GrDvrRecHndlNone;

			for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				if (m_DiskTbl[Tv_DskIdx]->HndlFindSeqIdBw(A_SeqId, &Tv_NowHndl, &Tv_NowSeqId))
				{
					if (A_SeqId == Tv_NowSeqId)
					{
						// found match id
						Tv_FndSeqId		=	Tv_NowSeqId;
						Tv_FndHndl		=	Tv_NowHndl;
						Tv_Result			=	TRUE;
						break;
					}
					if (Tv_Result)
					{
						if (Tv_FndSeqId < Tv_NowSeqId)
						{
							Tv_FndSeqId		=	Tv_NowSeqId;
							Tv_FndHndl		=	Tv_NowHndl;
						}
					}
					else
					{
						Tv_FndSeqId		=	Tv_NowSeqId;
						Tv_FndHndl		=	Tv_NowHndl;
						Tv_Result			=	TRUE;
					}
				}
			}

			if (Tv_Result)
			{
				*A_PtrRtHndl	=	Tv_FndHndl;
				*A_PtrRtSeqId	=	Tv_FndSeqId;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Grp::ReadTimeBw(Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
		__u64	Tv_Hndl;
		__u32	Tv_SeqId;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4RsltParamBad;

		if (LcHndlFindTimeBw(A_Time, &Tv_Hndl, &Tv_SeqId))
		{
			A_PtrCtrl->Hndl	=	Tv_Hndl;
			Tv_Result	=	ReadFw(A_PtrCtrl, 0);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Grp::LcHndlFindTimeBw(Def_GrTime A_Time, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		__u64	Tv_NowHndl;
		__u32	Tv_NowSeqId;
		__u32	Tv_NowTmGap;
		__u64	Tv_FndHndl;
		__u32	Tv_FndSeqId;
		__u32	Tv_FndTmGap;
	// code --------------------
		// init
		Tv_Result	=	FALSE;
		// check disk count able
		if (0 != m_DiskCnt)
		{
			// init
			Tv_FndSeqId	=	0;
			// find time
			for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt; Tv_DskIdx++)
			{
				if (m_DiskTbl[Tv_DskIdx]->HndlFindTimeBw(A_Time, &Tv_NowHndl, &Tv_NowSeqId, &Tv_NowTmGap, &m_GrpWorkEnv))
				{
					// check found
					if (0 == Tv_NowTmGap)
					{
						Tv_FndHndl	=	Tv_NowHndl;
						Tv_FndSeqId	=	Tv_NowSeqId;
						Tv_FndTmGap	=	Tv_NowTmGap;
						break;
					}
					if (0 == Tv_FndSeqId)
					{
						Tv_FndHndl	=	Tv_NowHndl;
						Tv_FndSeqId	=	Tv_NowSeqId;
						Tv_FndTmGap	=	Tv_NowTmGap;
					}
					else if (Tv_NowTmGap < Tv_FndTmGap)
					{
						Tv_FndHndl	=	Tv_NowHndl;
						Tv_FndSeqId	=	Tv_NowSeqId;
						Tv_FndTmGap	=	Tv_NowTmGap;
					}
				}
			}	//	for (Tv_DskIdx=0; Tv_DskIdx < m_DiskCnt;Tv_DskIdx++)

			if (0 != Tv_FndSeqId)
			{
				*A_PtrRtHndl	=	Tv_FndHndl;
				*A_PtrRtSeqId	=	Tv_FndSeqId;
				Tv_Result			=	TRUE;
			}

		}	//	if ( 0 != m_DiskCnt )

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Grp::BkupSizeEstmProc(Ptr_GrDvrFs4BkupSizeEstm A_PtrCtrl)
{
	// local -------------------
		BOOL8	Tv_IsFinish;
		St_GrTaskMsgBkupSizeEstm	Tv_Msg;
	// code --------------------
		Tv_IsFinish	=	FALSE;
		// check finish
		if ( A_PtrCtrl->PrgsDsk < m_DiskCnt )
		{
			m_DiskTbl[A_PtrCtrl->PrgsDsk]->BkupSizeEstmProc(A_PtrCtrl,&m_GrpWorkEnv);
			// check finish
			if (A_PtrCtrl->PrgsDsk >= m_DiskCnt)
			{
				Tv_IsFinish	=	TRUE;
			}
		}
		else
		{
			Tv_IsFinish	=	TRUE;
		}

		// notify finish
		if (Tv_IsFinish)
		{
			Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgBkupSizeEstm;
			Tv_Msg.RsltSize		=	A_PtrCtrl->Rslt;
			A_PtrCtrl->IsRun		=	FALSE;
		}

}
//--------------------------------------------------------------------
