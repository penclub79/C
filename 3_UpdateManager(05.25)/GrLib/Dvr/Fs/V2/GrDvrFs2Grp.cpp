/*
 GAUSE file system version 2 group

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
#include <Dvr/Fs/V2/GrDvrFs2Base.h>
#include <Dvr/Fs/V2/GrDvrFs2Grp.h>
#include <Dvr/Fs/V2/GrDvrFs2Mng.h>

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
Cls_GrDvrFs2Grp::Cls_GrDvrFs2Grp( Cls_GrTaskCli* A_ObjMng, __u8 A_DskMap, BOOL8 A_IsOvrtMode )
{
	// local -------------------
		// __u8	Tv_IntIdx;
	// code --------------------
		//init
		m_ObjMng			=	A_ObjMng;
		m_DiskMap			=	A_DskMap;
		GrDumyZeroMem( m_DiskTbl, sizeof(m_DiskTbl) );
		m_DiskCnt			=	0;
		m_RecIntIdx		=	E_GrDvrFs2RecIntIdxBad;
		m_RecId				=	E_GrDvrFs2RecIdNone;
		m_RecBlk			=	0;
		m_IsRecAble		=	FALSE;
		m_IsDefective	=	FALSE;
		m_IsOvwtMode	=	A_IsOvrtMode;

		m_GthAlloc		=	0;
		m_GthIn				=	0;
		m_GthBaseTime	=	0;
		GrDumyZeroMem( &m_GthTmb, sizeof(m_GthTmb) );

		m_AdoSyncCnt	=	((Cls_GrDvrFs2Mng*)m_ObjMng)->AdoSyncCntGet();
		m_AdoUnitSize	=	((Cls_GrDvrFs2Mng*)m_ObjMng)->AdoDataUnitSizeGet();

		GrDumyZeroMem( m_ChTbl, sizeof(m_ChTbl) );

		// build disk list
		do 
		{
			LcDiskTblBuild();
			if ( m_IsDefective )
			{
				// check V1 exist
				if ( 0 != LcV1DiskCount() )
				{
					// upgrade
					LcV1Upgrade();
				}
				else
				{
					// patch record id
					LcRebuildRecId();
				}
			}
			else
			{
				break;
			}
		} while ( 0 != m_DiskCnt );

}
//--------------------------------------------------------------------
Cls_GrDvrFs2Grp::~Cls_GrDvrFs2Grp()
{
	// local -------------------
	// code --------------------
		// flush BDT
		if ( (E_GrDvrFs2RecIntIdxBad != m_RecIntIdx) && (E_GrDvrFs2RecIdNone != m_RecId) )
		{
			m_DiskTbl[m_RecIntIdx]->BlockInfoFlush( m_RecBlk );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::LcDiskTblBuild( void )
{
	// local -------------------
		__u8	Tv_Mask;
		__u8	Tv_WkIdx;
		__u8	Tv_Status;
		//__u8	Tv_SubIdx;
		//__u32	Tv_RecId;
	// code --------------------
		// init
		m_IsDefective		=	FALSE;
		m_IsRecAble			=	FALSE;

		m_RecId					=	E_GrDvrFs2RecIdNone;
		m_RecIntIdx			=	E_GrDvrFs2RecIntIdxBad;

		// add disk by mask
		m_DiskCnt		=	0;
		Tv_Mask			=	1;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2MaxDriveCnt;Tv_WkIdx++ )
		{
			if ( 0 != (Tv_Mask & m_DiskMap) )
			{
				// add disk
				m_DiskTbl[m_DiskCnt]	=	((Cls_GrDvrFs2Mng*)m_ObjMng)->RtlDiskObjGet( Tv_WkIdx );
				if ( NULL != m_DiskTbl[m_DiskCnt] )
				{
					Tv_Status	=	m_DiskTbl[m_DiskCnt]->DiskStatGet();
					if ( E_GrDvrRecStatDefective <= Tv_Status )
					{
						if ( E_GrDvrRecStatDefective == Tv_Status )
						{
							m_IsDefective	=	TRUE;
						}
						// found
						m_DiskCnt ++;
					}
					else
					{
						// remove from list
						m_DiskTbl[m_DiskCnt]	=	NULL;
					}
				}
			}
			// next
			Tv_Mask	=	Tv_Mask << 1;
		}

		// update attribute
		if ( 0 != m_DiskCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_DiskCnt;Tv_WkIdx++ )
			{
				m_DiskTbl[Tv_WkIdx]->GrpIntIdxSet( Tv_WkIdx );
				m_DiskTbl[Tv_WkIdx]->OverwriteSet( m_IsOvwtMode );
			}

			if ( !LcChkRecIdOvlp() )
			{
				m_IsDefective	=	TRUE;
			}

			// check disk unit defective
			if ( m_IsDefective )
			{
				// notify group defective
				GrTaskSvrMsgErrorNotify( E_GrErrRecGrpDefective, m_GrpType );
			}

			// record able update
			m_IsRecAble	=	!m_IsDefective;
		}

		if ( m_IsRecAble )
		{
			LcBlockRecUpdt();
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Grp::LcPacketReplace( __u64 A_Hndl, Ptr_GrDvrAvHd A_PtrPkt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_DskIdx	=	(__u8)(A_Hndl & E_GrDvrFs2HndlGrpIdxMask);
		if ( Tv_DskIdx < m_DiskCnt )
		{
			Tv_Result	=	m_DiskTbl[Tv_DskIdx]->PacketReplace( A_Hndl, A_PtrPkt );
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Grp::LcPacketReplace - bad reserve handle. \n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Grp::AudioAdd( Ptr_GrDvrAvHd A_PtrPkt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Ch;
		__u8	Tv_RecIdx;
		__u8	Tv_RsvIdx;
		void*	Tv_PtrSr;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( (0 != m_DiskCnt) && m_IsRecAble )
		{
			Tv_Ch	=	A_PtrPkt->Ch;
			// check time and data size
			if ( (0 != A_PtrPkt->TimeSec) && (m_AdoUnitSize == A_PtrPkt->DataSize) )
			{
				// check audio sync use
				if ( 0 == m_AdoSyncCnt )
				{
					Tv_Result	=	LcGthPktAdd( A_PtrPkt );
				}
				else
				{
					// reserve
					Tv_RsvIdx	=	m_ChTbl[Tv_Ch].AdoPrgs + m_AdoSyncCnt;
					if ( Tv_RsvIdx >= E_GrDvrFs2AdoSyncMax )
					{
						Tv_RsvIdx	=	Tv_RsvIdx - E_GrDvrFs2AdoSyncMax;
					}

					// packet reserve
					m_ChTbl[Tv_Ch].AdoSync[Tv_RsvIdx].Hndl		=	E_GrDvrRecHndlNone;
					m_ChTbl[Tv_Ch].AdoSync[Tv_RsvIdx].PtrBuf	=	NULL;
					if ( 0 != A_PtrPkt->Evt )
					{
						m_ChTbl[Tv_Ch].AdoSync[Tv_RsvIdx].PtrBuf	=	LcGthAdoRsvd( A_PtrPkt->Ch, A_PtrPkt->Evt, 
							A_PtrPkt->TimeSec, A_PtrPkt->TimeMsec, A_PtrPkt->IsSm );
						if ( NULL != m_ChTbl[Tv_Ch].AdoSync[Tv_RsvIdx].PtrBuf )
						{
							Tv_Result	=	TRUE;
						}
					}

					// write
					Tv_RecIdx	=	m_ChTbl[Tv_Ch].AdoPrgs;
					if ( NULL != m_ChTbl[Tv_Ch].AdoSync[Tv_RecIdx].PtrBuf )
					{
						// buffer update
						Tv_PtrSr	=	(void*)( (__u32)A_PtrPkt + sizeof(St_GrDvrAvHd) );
						GrDumyCopyMem( m_ChTbl[Tv_Ch].AdoSync[Tv_RecIdx].PtrBuf, Tv_PtrSr, A_PtrPkt->DataSize ) ;
						m_ChTbl[Tv_Ch].AdoSync[Tv_RecIdx].PtrBuf	=	NULL;
						Tv_Result	=	TRUE;
					}
					if ( E_GrDvrRecHndlNone != m_ChTbl[Tv_Ch].AdoSync[Tv_RecIdx].Hndl )
					{
						// write
						Tv_Result	=	LcPacketReplace( m_ChTbl[Tv_Ch].AdoSync[Tv_RecIdx].Hndl, A_PtrPkt );
						m_ChTbl[Tv_Ch].AdoSync[Tv_RecIdx].Hndl	=	E_GrDvrRecHndlNone;
					}

					// next
					m_ChTbl[Tv_Ch].AdoPrgs ++;
				}
			}
			else
			{
				if ( m_AdoUnitSize != A_PtrPkt->DataSize )
				{
					DbgMsgPrint( "Cls_GrDvrFs2Grp::AudioAdd - ch ( %d ) data size ( %d ) is bad.\n", Tv_Ch, A_PtrPkt->DataSize );
				}
				else
				{
					DbgMsgPrint( "Cls_GrDvrFs2Grp::AudioAdd - ch ( %d ) data time ( %d ) is bad.\n", Tv_Ch, A_PtrPkt->TimeSec );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Grp::VideoAdd( Ptr_GrDvrAvHd A_PtrPkt )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( (0 != m_DiskCnt) && m_IsRecAble )
		{
			if (0 != A_PtrPkt->Evt)
			{
				Tv_Result	=	LcGthPktAdd( A_PtrPkt );
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs2Grp::DayMapGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
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
				Tv_DskMap	=	m_DiskTbl[Tv_DskIdx]->DayMapGet( A_TimeSt, A_TimeEd );
				Tv_Result	=	Tv_Result | Tv_DskMap;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Grp::GroupStatusGet( void )
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
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::RecSizeGet( __u64* A_PtrRtTotal, __u64* A_PtrRtRec )
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
BOOL8	Cls_GrDvrFs2Grp::RecMinMapGet( Def_GrTime A_TimeSt, void* A_PtrBuf, __u32 A_Cnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_IntIdx;
		__u32	Tv_Size;
	// code --------------------
		Tv_Result	=	FALSE;

		// check able
		if ( 0 != A_Cnt )
		{
			// reset
			Tv_Size	=	A_Cnt * sizeof(St_GrDvrRecMinThumb);
			GrDumyZeroMem( A_PtrBuf, Tv_Size );
			if ( 0 != m_DiskCnt )
			{
				for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
				{
					if ( m_DiskTbl[Tv_IntIdx]->MmdMapCombine( A_TimeSt, A_PtrBuf, A_Cnt ) )
					{
						Tv_Result	=	TRUE;
					}
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::ReadForward( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, BOOL8 A_IsBufChg )
{
	// local -------------------
		__u8	Tv_BufIdx;
		__u32	Tv_Prgs;
		__u8	Tv_IntIdx;
		__u16	Tv_OvwtId;
		__u64	Tv_Dpos;
		__u32	Tv_BlkId;
		__u32	Tv_RecId;
		__u32	Tv_BlkOfs;
		__u32	Tv_RecSize;
		BOOL8	Tv_IsNextBlk;
		BOOL8	Tv_IsSuccess;
		__u32	Tv_ReadSize;
	// code --------------------
		// init
		Tv_IsSuccess	=	FALSE;
		// check disk count able
		if ( 0 != m_DiskCnt )
		{
			// get handle info
			if ( E_GrDvrRecHndlNone != A_PtrCtrl->Hndl )
			{
				// init
				// get now buffer information
				Tv_BufIdx	=	0;
				if ( A_PtrCtrl->IsDblBuf )
				{
					Tv_BufIdx	=	A_PtrCtrl->BufIdx;
				}
				// get info
				Tv_Prgs	=	A_PtrCtrl->Buf[Tv_BufIdx].Prgs;
				if ( 0 != (Tv_Prgs & 0x1F) )		// debug
				{
					DbgMsgPrint( "Cls_GrDvrFs2Grp::ReadForward - bad progress.\n" );
				}

				// change target buffer
				if ( A_PtrCtrl->IsDblBuf && A_IsBufChg )
				{
					Tv_BufIdx	=	Tv_BufIdx ^ 1;
					// update buffer index
					A_PtrCtrl->BufIdx	=	Tv_BufIdx;
				}

				// buffer reset
				A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
				A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;

				// get handle info
				Tv_IsNextBlk	=	TRUE;
				if ( LcHndlInfoGet( A_PtrCtrl->Hndl, &Tv_IntIdx, &Tv_Dpos, &Tv_BlkId, &Tv_RecId, &Tv_OvwtId, &Tv_BlkOfs, &Tv_RecSize ) )
				{
					if ( Tv_RecId == A_PtrCtrl->RecId )
					{
						Tv_BlkOfs	=	Tv_BlkOfs + Tv_Prgs;
						Tv_Dpos		=	Tv_Dpos + Tv_Prgs;
						if ( Tv_BlkOfs < Tv_RecSize )
						{
							Tv_IsNextBlk	=	FALSE;
						}
					}
				}

				do
				{
					// go next block
					if ( Tv_IsNextBlk )
					{
						Tv_RecId ++;
						Tv_RecId	=	LcBlockFwRecIdFind( Tv_RecId, &Tv_IntIdx, &Tv_BlkId, &Tv_OvwtId, &Tv_RecSize );
						if ( E_GrDvrFs2RecIdNone == Tv_RecId )
						{
							// arrive end position
							A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;		// position end
							// finish
							break;
						}
						else
						{
							// update other info
							Tv_BlkOfs	=	0;
							Tv_Dpos		=	E_GrDvrFs2PosRda + ( (__u64)Tv_BlkId << E_GrDvrFs2BlockShift );
						}
					}
					// read
					// make handle and environment
					A_PtrCtrl->Hndl		=	m_DiskTbl[Tv_IntIdx]->HndlFromDpos( Tv_Dpos );
					A_PtrCtrl->RecId	=	Tv_RecId;
					Tv_ReadSize	=	Tv_RecSize - Tv_BlkOfs;
					if ( Tv_ReadSize > A_PtrCtrl->Buf[Tv_BufIdx].Size )
					{
						Tv_ReadSize	=	A_PtrCtrl->Buf[Tv_BufIdx].Size;
					}

					// do read
					if ( m_DiskTbl[Tv_IntIdx]->ReadByHndl( A_PtrCtrl->Hndl, A_PtrCtrl->Buf[Tv_BufIdx].Ptr, Tv_ReadSize ) )
					{
						// update info
						A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	Tv_ReadSize;
						A_PtrCtrl->Stat		=	E_GrDvrSeekStatNormal;
						Tv_IsSuccess	=	TRUE;
						break;
					}
					Tv_IsNextBlk	=	TRUE;		// read next block
				}	while( !Tv_IsSuccess );
					
			}
			else
			{
				DbgMsgPrint( "Cls_GrDvrFs2Grp::ReadForward - bad handle input.\n" );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::ReadBackward( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, BOOL8 A_IsBufChg )
{
	// local -------------------
		__u8	Tv_BufIdx;
		__u32	Tv_Prgs;
		__u32	Tv_Readed;
		__u8	Tv_IntIdx;
		__u16	Tv_OvwtId;
		__u64	Tv_Dpos;
		__u32	Tv_BlkId;
		__u32	Tv_RecId;
		__u32	Tv_BlkOfs;
		__u32	Tv_RecSize;
		BOOL8	Tv_IsNextBlk;
		BOOL8	Tv_IsSuccess;
		__u32	Tv_ReadSize;
	// code --------------------
		// init
		Tv_IsSuccess	=	FALSE;
		// check disk count able
		if ( 0 != m_DiskCnt )
		{
			// get handle info
			if ( E_GrDvrRecHndlNone != A_PtrCtrl->Hndl )
			{
				// init
				// get now buffer information
				Tv_BufIdx	=	0;
				if ( A_PtrCtrl->IsDblBuf )
				{
					Tv_BufIdx	=	A_PtrCtrl->BufIdx;
				}
				// get info
				Tv_Prgs			=	A_PtrCtrl->Buf[Tv_BufIdx].Prgs;
				Tv_Readed		=	A_PtrCtrl->Buf[Tv_BufIdx].Readed;
				if ( 0 != (Tv_Prgs & 0x1F) )		// debug
				{
					DbgMsgPrint( "Cls_GrDvrFs2Grp::ReadBackward - bad progress.\n" );
				}

				// change target buffer
				if ( A_PtrCtrl->IsDblBuf && A_IsBufChg )
				{
					Tv_BufIdx	=	Tv_BufIdx ^ 1;
					// update buffer index
					A_PtrCtrl->BufIdx	=	Tv_BufIdx;
				}

				// buffer reset
				A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
				A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;

				// get handle info
				Tv_IsNextBlk	=	TRUE;
				if ( LcHndlInfoGet( A_PtrCtrl->Hndl, &Tv_IntIdx, &Tv_Dpos, &Tv_BlkId, &Tv_RecId, &Tv_OvwtId, &Tv_BlkOfs, &Tv_RecSize ) )
				{
					if ( Tv_RecId == A_PtrCtrl->RecId )
					{
						Tv_BlkOfs	=	Tv_BlkOfs + Tv_Prgs;
						Tv_Dpos		=	Tv_Dpos + Tv_Prgs;
						if ( 0 < Tv_BlkOfs )
						{
							Tv_IsNextBlk	=	FALSE;
						}
					}
				}

				do
				{
					// go next block
					if ( Tv_IsNextBlk )
					{
						if ( 1 < Tv_RecId )
						{
							Tv_RecId --;
							Tv_RecId	=	LcBlockBwRecIdFind( Tv_RecId, &Tv_IntIdx, &Tv_BlkId, &Tv_OvwtId, &Tv_RecSize );
							if ( E_GrDvrFs2RecIdNone == Tv_RecId )
							{
								// arrive end position
								A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;		// position end
								// finish
								break;
							}
							else
							{
								// update other info
								Tv_BlkOfs	=	Tv_RecSize;
								Tv_Dpos		=	E_GrDvrFs2PosRda + ( (__u64)Tv_BlkId << E_GrDvrFs2BlockShift ) + (__u64)Tv_RecSize;
							}
						}
						else
						{
							// arrive begin position
							A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;		// position end
							// finish
							break;
						}
					}
					// read
					// make handle and environment
					Tv_ReadSize	=	A_PtrCtrl->Buf[Tv_BufIdx].Size;
					if ( Tv_ReadSize > Tv_BlkOfs )
					{
						Tv_ReadSize	=	Tv_BlkOfs;
					}
					Tv_BlkOfs	=	Tv_BlkOfs - Tv_ReadSize;
					Tv_Dpos		=	Tv_Dpos - (__u64)Tv_ReadSize;
					A_PtrCtrl->Hndl		=	m_DiskTbl[Tv_IntIdx]->HndlFromDpos( Tv_Dpos );
					A_PtrCtrl->RecId	=	Tv_RecId;

					// do read
					if ( m_DiskTbl[Tv_IntIdx]->ReadByHndl( A_PtrCtrl->Hndl, A_PtrCtrl->Buf[Tv_BufIdx].Ptr, Tv_ReadSize ) )
					{
						// update info
						A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	Tv_ReadSize;
						A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	Tv_ReadSize;
						A_PtrCtrl->Stat		=	E_GrDvrSeekStatNormal;
						Tv_IsSuccess	=	TRUE;
						break;
					}
					Tv_IsNextBlk	=	TRUE;		// read next block
				}	while( !Tv_IsSuccess );

			}
			else
			{
				DbgMsgPrint( "Cls_GrDvrFs2Grp::ReadForward - bad handle input.\n" );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::ReadBegin( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl )
{
	// local -------------------
		__u8	Tv_BufIdx;
		__u8	Tv_IntIdx;
		__u64	Tv_Dpos;
		__u32	Tv_BlkId;
	// code --------------------
		if ( 0 != m_DiskCnt )
		{
			A_PtrCtrl->RecId	=	LcBlockMinRecIdFind( &Tv_IntIdx, &Tv_BlkId );
			if ( E_GrDvrFs2RecIdNone != A_PtrCtrl->RecId )
			{
				// make handle
				Tv_Dpos	=	E_GrDvrFs2PosRda + ( (__u64)Tv_BlkId << E_GrDvrFs2BlockShift );
				A_PtrCtrl->Hndl	=	m_DiskTbl[Tv_IntIdx]->HndlFromDpos( Tv_Dpos );

				// buffer change
				Tv_BufIdx	=	0;
				if ( A_PtrCtrl->IsDblBuf )
				{
					Tv_BufIdx	=	A_PtrCtrl->BufIdx;
					Tv_BufIdx	=	Tv_BufIdx ^ 1;
					// update buffer index
					A_PtrCtrl->BufIdx	=	Tv_BufIdx;
				}
				// reset
				A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
				A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;
				// try read
				ReadForward( A_PtrCtrl, FALSE );
			}
			else
			{
				// stop
				A_PtrCtrl->Hndl		=	E_GrDvrRecHndlNone;
				A_PtrCtrl->Stat		=	E_GrDvrSeekStatEnd;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::ReadEnd( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl )
{
	// local -------------------
		__u8	Tv_BufIdx;
		__u8	Tv_IntIdx;
		__u64	Tv_Dpos;
		__u32	Tv_BlkId;
		__u32	Tv_RecId;
		__u32	Tv_RecSize;
		__u16	Tv_OvwtId;
	// code --------------------
		if ( 0 != m_DiskCnt )
		{
			A_PtrCtrl->RecId	=	LcBlockMaxRecIdFind( &Tv_IntIdx, &Tv_BlkId );
			if ( E_GrDvrFs2RecIdNone != A_PtrCtrl->RecId )
			{
				// get block info
				if ( m_DiskTbl[Tv_IntIdx]->BlockInfoGet( Tv_BlkId, &Tv_RecId, &Tv_RecSize, &Tv_OvwtId ) )
				{
					// make handle
					Tv_Dpos	=	E_GrDvrFs2PosRda + ( (__u64)Tv_BlkId << E_GrDvrFs2BlockShift ) + (__u64)Tv_RecSize;
					A_PtrCtrl->Hndl	=	m_DiskTbl[Tv_IntIdx]->HndlFromDpos( Tv_Dpos );

					// buffer change
					Tv_BufIdx	=	0;
					if ( A_PtrCtrl->IsDblBuf )
					{
						Tv_BufIdx	=	A_PtrCtrl->BufIdx;
						Tv_BufIdx	=	Tv_BufIdx ^ 1;
						// update buffer index
						A_PtrCtrl->BufIdx	=	Tv_BufIdx;
					}
					// reset
					A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
					A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;
					// try read
					ReadBackward( A_PtrCtrl, FALSE );
				}
				else
				{
					DbgMsgPrint("Cls_GrDvrFs2Grp::ReadEnd - block infomation get error.\n");
					// stop
					A_PtrCtrl->Hndl		=	E_GrDvrRecHndlNone;
					A_PtrCtrl->Stat		=	E_GrDvrSeekStatEnd;
				}
			}
			else
			{
				// stop
				A_PtrCtrl->Hndl		=	E_GrDvrRecHndlNone;
				A_PtrCtrl->Stat		=	E_GrDvrSeekStatEnd;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Grp::ReadByHndlFw( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_BufIdx;
		__u8	Tv_IntIdx;
		__u64	Tv_Dpos;
		__u32	Tv_BlkId;
		__u32	Tv_RecId;
		__u16	Tv_OvwtId;
		__u32	Tv_BlkOfs;
		__u32	Tv_RecSize;
	// code --------------------
		Tv_Result	=	FALSE;
		// check handle
		if ( LcHndlInfoGet( A_Hndl, &Tv_IntIdx, &Tv_Dpos, &Tv_BlkId, &Tv_RecId, &Tv_OvwtId, &Tv_BlkOfs, &Tv_RecSize ) )
		{
			// buffer change
			Tv_BufIdx	=	0;
			if ( A_PtrCtrl->IsDblBuf )
			{
				Tv_BufIdx	=	A_PtrCtrl->BufIdx;
				Tv_BufIdx	=	Tv_BufIdx ^ 1;
				// update buffer index
				A_PtrCtrl->BufIdx	=	Tv_BufIdx;
			}

			// init control
			A_PtrCtrl->Hndl		=	A_Hndl;
			A_PtrCtrl->RecId	=	Tv_RecId;
			A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
			A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;

			// read forward
			ReadForward( A_PtrCtrl, FALSE );

			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Grp::ReadByHndlBw( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_BufIdx;
		__u8	Tv_IntIdx;
		__u64	Tv_Dpos;
		__u32	Tv_BlkId;
		__u32	Tv_RecId;
		__u16	Tv_OvwtId;
		__u32	Tv_BlkOfs;
		__u32	Tv_RecSize;
	// code --------------------
		Tv_Result	=	FALSE;
		// check handle
		if ( LcHndlInfoGet( A_Hndl, &Tv_IntIdx, &Tv_Dpos, &Tv_BlkId, &Tv_RecId, &Tv_OvwtId, &Tv_BlkOfs, &Tv_RecSize ) )
		{
			// buffer change
			Tv_BufIdx	=	0;
			if ( A_PtrCtrl->IsDblBuf )
			{
				Tv_BufIdx	=	A_PtrCtrl->BufIdx;
				Tv_BufIdx	=	Tv_BufIdx ^ 1;
				// update buffer index
				A_PtrCtrl->BufIdx	=	Tv_BufIdx;
			}

			// init control
			A_PtrCtrl->Hndl		=	A_Hndl;
			A_PtrCtrl->RecId	=	Tv_RecId;
			A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
			A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;

			// read forward
			ReadBackward( A_PtrCtrl, FALSE );

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs2Grp::RecDayFindFw( Def_GrTime A_TimeBase )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__u8	Tv_IntIdx;
		Def_GrTime	Tv_Day;
		Def_GrTime	Tv_TimeRt;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 != m_DiskCnt )
		{
			Tv_Day	=	A_TimeBase - (A_TimeBase % E_GrTimeSecOfDay);
			for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx ++ )
			{
				Tv_TimeRt	=	m_DiskTbl[Tv_IntIdx]->RecDayFindFw( Tv_Day );
				if ( 0 != Tv_TimeRt )
				{
					if ( 0 == Tv_Result )
					{
						Tv_Result	=	Tv_TimeRt;
					}
					else
					{
						if ( Tv_Result > Tv_TimeRt )
						{
							Tv_Result	=	Tv_TimeRt;
						}
					}
					// check finished
					if ( Tv_Result == Tv_Day )
					{
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs2Grp::RecDayFindBw( Def_GrTime A_TimeBase )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__u8	Tv_IntIdx;
		Def_GrTime	Tv_Day;
		Def_GrTime	Tv_TimeRt;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 != m_DiskCnt )
		{
			Tv_Day	=	A_TimeBase - (A_TimeBase % E_GrTimeSecOfDay);
			for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx ++ )
			{
				Tv_TimeRt	=	m_DiskTbl[Tv_IntIdx]->RecDayFindBw( Tv_Day );
				if ( 0 != Tv_TimeRt )
				{
					if ( 0 == Tv_Result )
					{
						Tv_Result	=	Tv_TimeRt;
					}
					else
					{
						if ( Tv_Result < Tv_TimeRt )
						{
							Tv_Result	=	Tv_TimeRt;
						}
					}
					// check finished
					if ( Tv_Result == Tv_Day )
					{
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Grp::LcGthPktAdd( Ptr_GrDvrAvHd A_PtrPkt )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsNeedAlloc;
		BOOL8	Tv_IsNeedAdd;
		BOOL8	Tv_IsNeedBdt;
		__u32	Tv_PktSize;
		void*	Tv_PtrBuf;
		__u32	Tv_OccSize;	// occupy size
		__u32*	Tv_PtrTail;
	// code --------------------
		Tv_Result				=	FALSE;
		Tv_IsNeedAdd		=	FALSE;
		Tv_PktSize			=	sizeof(St_GrDvrAvHd) + A_PtrPkt->DataSize;
		Tv_OccSize			=	(Tv_PktSize + E_GrDvrRecTailMarkSize + E_GrDvrFs2StrmAlignSize - 1) & E_GrDvrFs2StrmAlignMask;
		
		// check packet size
		if ( Tv_OccSize < E_GrDvrFs2GrpGthBufSize )
		{
			Tv_IsNeedAlloc	=	TRUE;
			if ( 0 != m_GthAlloc )
			{
				Tv_IsNeedAlloc	=	FALSE;
				Tv_IsNeedAdd		=	TRUE;
				Tv_IsNeedBdt		=	FALSE;
				if ( (m_GthAlloc - m_GthIn) < Tv_OccSize )
				{
					Tv_IsNeedAlloc	=	TRUE;
				}

				if ( 0 != m_GthIn )
				{
					if ( E_GrDvrAvTypRsvAdo >= A_PtrPkt->Type )
					{
						if ( (m_GthBaseTime > A_PtrPkt->TimeSec) || ( (m_GthBaseTime + E_GrTimeSecOfMin) <= A_PtrPkt->TimeSec )  )
						{
							// flush
							Tv_IsNeedAlloc	=	TRUE;
							Tv_IsNeedBdt		=	TRUE;
#ifdef E_GrDvrFs2GrpDbgMsgView
							DbgMsgPrint( "Gth-Time(%X)-", A_PtrPkt->TimeSec );
#endif
						}
					}
				}

				if ( Tv_IsNeedAlloc )
				{
#ifdef E_GrDvrFs2GrpDbgMsgView
					DbgMsgPrint( "Gth-Flush :" );
#endif
					LcGthBufFlush( Tv_IsNeedBdt );
				}
			}

			if ( Tv_IsNeedAlloc )
			{
				Tv_IsNeedAdd	=	FALSE;
				if ( LcGthBufAlloc( Tv_OccSize, E_GrDvrFs2GrpGthBufSize ) )
				{
					Tv_IsNeedAdd	=	TRUE;
				}
			}

			if ( Tv_IsNeedAdd )
			{
				// copy contents
				Tv_PtrBuf	=	(void*)( (__u32)m_GthBuf + m_GthIn );
				GrDumyCopyMem( Tv_PtrBuf, A_PtrPkt, Tv_PktSize );
				// mark tail
				Tv_PtrTail	=	(__u32*)( (__u32)Tv_PtrBuf + Tv_OccSize - E_GrDvrRecTailMarkSize );
				*Tv_PtrTail	=	E_GrDvrRecTailMarkId | Tv_OccSize;
				// move buffer pointer
				m_GthIn		=	m_GthIn + Tv_OccSize;
				// update attribute
				if ( (E_GrDvrAvTypVdoP >= A_PtrPkt->Type) || (E_GrDvrAvTypVdoB == A_PtrPkt->Type))
				{
					m_GthTmb.AtbTbl[A_PtrPkt->Ch]	=	m_GthTmb.AtbTbl[A_PtrPkt->Ch] | A_PtrPkt->Evt;
				}
				// update time
				if ( E_GrDvrAvTypRsvAdo >= A_PtrPkt->Type )
				{
					if ( 0 == m_GthTmb.TimeSt )
					{
						m_GthTmb.TimeSt		=	A_PtrPkt->TimeSec;
						m_GthTmb.TimeEd		=	m_GthTmb.TimeSt;
						m_GthBaseTime			=	m_GthTmb.TimeSt - (m_GthTmb.TimeSt % E_GrTimeSecOfMin);
					}
					if ( m_GthTmb.TimeEd < A_PtrPkt->TimeSec )
					{
						m_GthTmb.TimeEd	=	A_PtrPkt->TimeSec;
					}
				}
				// check need flush
				if ( m_GthIn >= m_GthAlloc )
				{
#ifdef E_GrDvrFs2GrpDbgMsgView
					DbgMsgPrint( "Gth-EFull :" );
#endif
					LcGthBufFlush( FALSE );
				}
				Tv_Result	=	TRUE;
			}

		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Grp::LcGthDskAdd - invalid packet size ( %d ) in.\n",Tv_OccSize );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::LcGthBufFlush( BOOL8 A_IsBdtUpdt )
{
	// local -------------------
		__u8	Tv_Ch;
		__u8	Tv_Sync;
		__u64	Tv_Hndl;
		__u64	Tv_Dpos;
		__u64	Tv_Rpos;
	// code --------------------
		// flush
		if ( E_GrDvrFs2RecIntIdxBad != m_RecIntIdx )
		{
			if ( m_DiskTbl[m_RecIntIdx]->BlockDataAdd( m_RecBlk, m_RecId, m_GthBuf, m_GthIn, &m_GthTmb, A_IsBdtUpdt ) )
			{
#ifdef E_GrDvrFs2GrpDbgMsgView
				DbgMsgPrint( "%d\n", m_GthIn );
#endif
				Tv_Hndl	=	m_DiskTbl[m_RecIntIdx]->RecLastHndlGet();
				if ( E_GrDvrRecHndlNone != Tv_Hndl )
				{
					Tv_Dpos	=	Tv_Hndl & E_GrDvrFs2HndlPosMask;
					// update audio sync
					for ( Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++ )
					{
						for ( Tv_Sync=0;Tv_Sync < E_GrDvrFs2AdoSyncMax;Tv_Sync++ )
						{
							if ( NULL != m_ChTbl[Tv_Ch].AdoSync[Tv_Sync].PtrBuf )
							{
								Tv_Rpos	=	Tv_Dpos + (__u64)( (__u32)m_ChTbl[Tv_Ch].AdoSync[Tv_Sync].PtrBuf - (__u32)m_GthBuf );
								m_ChTbl[Tv_Ch].AdoSync[Tv_Sync].Hndl		=	m_DiskTbl[m_RecIntIdx]->HndlFromDpos( Tv_Rpos );
								m_ChTbl[Tv_Ch].AdoSync[Tv_Sync].PtrBuf	=	NULL;
							}
						}
					}
				}
			}
		}
		
		// reset block
		m_GthAlloc	=	0;
		m_GthIn			=	0;
		GrDumyZeroMem( &m_GthTmb, sizeof(m_GthTmb) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Grp::LcGthBufAlloc( __u32 A_ReqMin, __u32 A_ReqMax )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BlkFree;
		__u32	Tv_RecId;
		__u8	Tv_IntIdx;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result		=	FALSE;
		m_GthAlloc	=	0;
		// check recorded block
		if ( (E_GrDvrFs2RecIntIdxBad == m_RecIntIdx) || (E_GrDvrFs2RecIdNone == m_RecId) )
		{
			LcBlockRecUpdt();
			DbgMsgPrint( "Cls_GrDvrFs2Grp::LcGthBufAlloc - record block not set, find last block.\n" );
		}

		if ( (E_GrDvrFs2RecIntIdxBad != m_RecIntIdx) && (E_GrDvrFs2RecIdNone != m_RecId) )
		{
			// get block size
			Tv_BlkFree	=	m_DiskTbl[m_RecIntIdx]->BlockFreeSizeGet( m_RecBlk );
			if ( A_ReqMin <= Tv_BlkFree )
			{
				m_GthAlloc	=	Tv_BlkFree;
				if ( A_ReqMax <= Tv_BlkFree )
				{
					m_GthAlloc	=	A_ReqMax;
				}
				Tv_Result	=	TRUE;
			}
			else
			{
				// full block
				m_DiskTbl[m_RecIntIdx]->BlockFullSet( m_RecBlk );
				// increase record id
				m_RecId ++;
				m_RecIntIdx	=	E_GrDvrFs2RecIntIdxBad;
				m_RecBlk		=	0;
			}

			// check allocate success
			if ( !Tv_Result )
			{
				// find empty block
				if ( LcBlockEmptyFind( &m_RecIntIdx, &m_RecBlk ) )
				{
					m_GthAlloc	=	A_ReqMax;
					Tv_Result		=	TRUE;
				}
				else
				{
					if ( m_IsOvwtMode )
					{
						// find lower record id
						Tv_RecId	=	LcBlockMinRecIdFind( &Tv_IntIdx, &Tv_BlkIdx );
						while ( E_GrDvrFs2RecIdNone != Tv_RecId )
						{
							m_DiskTbl[Tv_IntIdx]->BlockDelete( Tv_BlkIdx );
							if ( E_GrDvrRecStatEmpty <= m_DiskTbl[Tv_IntIdx]->DiskStatGet() )
							{
								// delete success
								break;
							}
							// next find
							Tv_RecId	=	LcBlockMinRecIdFind( &Tv_IntIdx, &Tv_BlkIdx );
						}
						// check success
						if ( E_GrDvrFs2RecIdNone != Tv_RecId )
						{
							// found
							m_RecIntIdx	=	Tv_IntIdx;
							m_RecBlk		=	Tv_BlkIdx;
							// update allocate
							m_GthAlloc	=	A_ReqMax;
							Tv_Result		=	TRUE;
						}
						else
						{
							// no record able
							m_IsRecAble	=	FALSE;
							DbgMsgPrint( "Cls_GrDvrFs2Grp::LcGthBufAlloc - no have record able block.\n" );
						}
					}
					else
					{
						// disk full notify
						m_IsRecAble	=	FALSE;
						GrTaskSvrMsgErrorNotify( E_GrErrDiskFull, m_GrpType );
						DbgMsgPrint( "Cls_GrDvrFs2Grp::LcGthBufAlloc - disk full.\n" );
					}
				}
			}
			
		}
		
		return	Tv_Result;		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Grp::LcChkRecIdOvlp( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		__u32	Tv_SubIdx;
	// code --------------------
		Tv_Result	=	TRUE;

		// check group defective
		if ( 1 < m_DiskCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < (m_DiskCnt - 1);Tv_WkIdx++ )
			{
				for ( Tv_SubIdx=Tv_WkIdx + 1;Tv_SubIdx < m_DiskCnt;Tv_SubIdx++ )
				{
					if ( !m_DiskTbl[Tv_SubIdx]->IsBdtCrossCheck( m_DiskTbl[Tv_WkIdx] ) )
					{
						// defective found
						Tv_Result	=	FALSE;
						break;
					}
				}
				if ( !Tv_Result )
				{
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::LcBlockRecUpdt( void )
{
	// local -------------------
		__u8	Tv_IntIdx;
		__u32	Tv_BlkIdx;
		__u32	Tv_RecId;
	// code --------------------
		m_RecId			=	E_GrDvrFs2RecIdNone;
		m_RecIntIdx	=	E_GrDvrFs2RecIntIdxBad;
		m_RecBlk		=	0;
		if ( m_IsRecAble )
		{
			if ( 0 != m_DiskCnt )
			{
				for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
				{
					Tv_RecId	=	m_DiskTbl[Tv_IntIdx]->BlockMaxRecIdFind( &Tv_BlkIdx );
					if ( m_RecId < Tv_RecId )
					{
						m_RecId			=	Tv_RecId;
						m_RecBlk		=	Tv_BlkIdx;
						m_RecIntIdx	=	Tv_IntIdx;
					}
				}	//	for ( Tv_IntIdx=1;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
				// record able block not found
				if ( E_GrDvrFs2RecIdNone == m_RecId )
				{
					m_RecId ++;
					m_RecIntIdx	=	0;
				}
			}	//	if ( 0 != m_DiskCnt )
		}	//	if ( m_IsRecAble )
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::LcRebuildRecId( void )
{
	// local -------------------
		__u8	Tv_IntIdx;
		__u8	Tv_DevIdx;
		__u32	Tv_BlkIdx;
		St_GrDvrFs2GrpBdtLst	Tv_BdtLst[E_GrDvrFs2MaxDriveCnt];
		__u32	Tv_Time;
		__u32	Tv_RecId;
		__u32	Tv_RecBlk;
		__u8	Tv_RecIntIdx;
		__u32	Tv_WorkCnt;
		Ptr_GrDvrFs2Bdt	Tv_PtrBdt;
		__u32	Tv_Mili;
		St_GrDvrLogItm	Tv_Log;
	// code --------------------
		// init
		m_RecIntIdx	=	E_GrDvrFs2RecIntIdxBad;
		m_RecBlk		=	0;
		m_RecId			=	E_GrDvrFs2RecIdNone;

		if ( 0 != m_DiskCnt )
		{
			// reset sort mark
			for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx ++ )
			{
				Tv_BdtLst[Tv_IntIdx].PtrBdt	=	m_DiskTbl[Tv_IntIdx]->BdtTableGet();
				Tv_BdtLst[Tv_IntIdx].BlkCnt	=	m_DiskTbl[Tv_IntIdx]->BlockCountGet();
				Tv_PtrBdt	=	Tv_BdtLst[Tv_IntIdx].PtrBdt;
				for ( Tv_BlkIdx=0;Tv_BlkIdx < Tv_BdtLst[Tv_IntIdx].BlkCnt;Tv_BlkIdx++ )
				{
					Tv_BdtLst[Tv_IntIdx].PtrBdt[Tv_BlkIdx].RecId	=	E_GrDvrFs2RecIdNone;
					// check bad time block
					if ( (!Tv_PtrBdt[Tv_BlkIdx].IsBad) && (0 < Tv_PtrBdt[Tv_BlkIdx].RecSize) && (E_GrDvrFs2RecIdNone != Tv_PtrBdt[Tv_BlkIdx].RecId) )
					{
						if ( (0 == Tv_PtrBdt[Tv_BlkIdx].TimeSt) || (0 == Tv_PtrBdt[Tv_BlkIdx].TimeEd) )
						{
							Tv_DevIdx	=	m_DiskTbl[Tv_IntIdx]->DevIdxGet();
							DbgMsgPrint( "Cls_GrDvrFs2Grp::LcRebuildRecId - delete bad time block ( %d : %d ).\n", Tv_DevIdx, Tv_BlkIdx );
							// notify bad time block found
							GrTaskSvrMsgErrorNotify( E_GrErrRecBadTimeBlk, Tv_DevIdx, 0, Tv_BlkIdx );
							// delete block
							Tv_PtrBdt[Tv_BlkIdx].IsFull		=	FALSE;
							Tv_PtrBdt[Tv_BlkIdx].OvwtId		=	0;
							Tv_PtrBdt[Tv_BlkIdx].RecSize	=	0;
							Tv_PtrBdt[Tv_BlkIdx].TimeSt		=	0;
							Tv_PtrBdt[Tv_BlkIdx].TimeEd		=	0;
						}
					}
				}
			}
			// do start sort
			Tv_RecId	=	1;
			do 
			{
				Tv_WorkCnt		=	0;
				Tv_Time				=	0xFFFFFFFF;
				Tv_RecIntIdx	=	E_GrDvrFs2RecIntIdxBad;
				Tv_RecBlk			=	0;
				for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx ++ )
				{
					Tv_PtrBdt	=	Tv_BdtLst[Tv_IntIdx].PtrBdt;
					for ( Tv_BlkIdx=0;Tv_BlkIdx < Tv_BdtLst[Tv_IntIdx].BlkCnt;Tv_BlkIdx++ )
					{
						if ( (!Tv_PtrBdt[Tv_BlkIdx].IsBad) && (0 < Tv_PtrBdt[Tv_BlkIdx].RecSize) && (E_GrDvrFs2RecIdNone != Tv_PtrBdt[Tv_BlkIdx].RecId) )
						{
							// check time
							if ( Tv_Time > Tv_PtrBdt[Tv_BlkIdx].TimeEd )
							{
								Tv_Time	=	Tv_PtrBdt[Tv_BlkIdx].TimeEd;
								Tv_RecIntIdx	=	Tv_RecIntIdx;
								Tv_RecBlk			=	Tv_BlkIdx;
								Tv_WorkCnt ++;
							}
						}
					}
				}

				// update record control
				if ( E_GrDvrFs2RecIntIdxBad != Tv_RecIntIdx )
				{
					// update record block
					m_RecIntIdx	=	Tv_RecIntIdx;
					m_RecId			=	Tv_RecId;
					m_RecBlk		=	Tv_RecBlk;
					// update sorted
					Tv_BdtLst[Tv_RecIntIdx].PtrBdt[Tv_RecBlk].RecId	=	Tv_RecId;
					// next
					Tv_RecId ++;
				}
				
			} while ( 1 < Tv_WorkCnt );

			// save BDT
			for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
			{
				m_DiskTbl[Tv_IntIdx]->BdtSave();
			}

			// add defected log
			Tv_Log.Ch		=	0xFF;
			Tv_Log.Type	=	E_GrDvrLogTypeFsDefect;
			Tv_Log.Time	=	GrTimeSysTimeGet( &Tv_Mili );
			for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
			{
				Tv_Log.ParaByte	=	m_DiskTbl[Tv_IntIdx]->DevIdxGet();
				m_DiskTbl[Tv_IntIdx]->EvtLogAdd( &Tv_Log );
			}
			
			// reset defective
			m_IsDefective	=	FALSE;
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Grp::LcBlockEmptyFind( __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_IntIdx;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
		{
			if ( m_DiskTbl[Tv_IntIdx]->BlockEmptyFind( &Tv_BlkIdx ) )
			{
				Tv_Result	=	TRUE;
				*A_PtrRtIntIdx	=	Tv_IntIdx;
				*A_PtrRtBlkIdx	=	Tv_BlkIdx;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs2Grp::LcBlockMinRecIdFind( __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx )
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_IntIdx;
		__u32	Tv_BlkIdx;
		__u32	Tv_RecId;
	// code --------------------
		Tv_Result	=	E_GrDvrFs2RecIdNone;

		for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
		{
			Tv_RecId	=	m_DiskTbl[Tv_IntIdx]->BlockMinRecIdFind( &Tv_BlkIdx );
			if ( E_GrDvrFs2RecIdNone != Tv_RecId )
			{
				if ( E_GrDvrFs2RecIdNone == Tv_Result )
				{
					Tv_Result	=	Tv_RecId;
					*A_PtrRtIntIdx	=	Tv_IntIdx;
					*A_PtrRtBlkIdx	=	Tv_BlkIdx;
				}
				else if ( Tv_Result > Tv_RecId )
				{
					Tv_Result	=	Tv_RecId;
					*A_PtrRtIntIdx	=	Tv_IntIdx;
					*A_PtrRtBlkIdx	=	Tv_BlkIdx;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs2Grp::LcBlockMaxRecIdFind( __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkIdx )
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_IntIdx;
		__u32	Tv_BlkIdx;
		__u32	Tv_RecId;
	// code --------------------
		Tv_Result	=	E_GrDvrFs2RecIdNone;

		for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
		{
			Tv_RecId	=	m_DiskTbl[Tv_IntIdx]->BlockMaxRecIdFind( &Tv_BlkIdx );
			if ( E_GrDvrFs2RecIdNone != Tv_RecId )
			{
				if ( E_GrDvrFs2RecIdNone == Tv_Result )
				{
					Tv_Result	=	Tv_RecId;
					*A_PtrRtIntIdx	=	Tv_IntIdx;
					*A_PtrRtBlkIdx	=	Tv_BlkIdx;
				}
				else if ( Tv_Result < Tv_RecId )
				{
					Tv_Result	=	Tv_RecId;
					*A_PtrRtIntIdx	=	Tv_IntIdx;
					*A_PtrRtBlkIdx	=	Tv_BlkIdx;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrDvrFs2Grp::LcGthAdoRsvd( __u8 A_Ch, __u8 A_Evt, Def_GrTime A_Time, __u32 A_Mili, BOOL8 A_IsSm )
{
	// local -------------------
		void*	Tv_Result;
		BOOL8	Tv_IsNeedAlloc;
		BOOL8	Tv_IsNeedAdd;
		BOOL8	Tv_IsNeedBdt;
		__u32	Tv_PktSize;
		Ptr_GrDvrAvHd	Tv_PtrHd;
		__u32	Tv_OccSize;	// occupy size
		__u32*	Tv_PtrTail;
	// code --------------------
		Tv_Result	=	NULL;

		Tv_IsNeedAdd		=	FALSE;
		Tv_PktSize			=	sizeof(St_GrDvrAvHd) + m_AdoUnitSize;
		Tv_OccSize			=	(Tv_PktSize + E_GrDvrRecTailMarkSize + E_GrDvrFs2StrmAlignSize - 1) & E_GrDvrFs2StrmAlignMask;

		// check packet size
		if ( Tv_OccSize < E_GrDvrFs2GrpGthBufSize )
		{
			Tv_IsNeedAlloc	=	TRUE;
			if ( 0 != m_GthAlloc )
			{
				Tv_IsNeedAlloc	=	FALSE;
				Tv_IsNeedAdd		=	TRUE;
				Tv_IsNeedBdt	=	FALSE;
				if ( (m_GthAlloc - m_GthIn) < Tv_OccSize )
				{
					Tv_IsNeedAlloc	=	TRUE;
				}
				// check time out
				if ( 0 != m_GthIn )
				{
					if ( (m_GthBaseTime > A_Time) || ( (m_GthBaseTime + E_GrTimeSecOfMin) <= A_Time )  )
					{
						// flush
						Tv_IsNeedBdt		=	TRUE;
						Tv_IsNeedAlloc	=	TRUE;
#ifdef E_GrDvrFs2GrpDbgMsgView
						DbgMsgPrint( "Gth-Time(%X)-", A_Time );
#endif
					}
				}

				if ( Tv_IsNeedAlloc )
				{
					// buffer flushing
#ifdef E_GrDvrFs2GrpDbgMsgView
					DbgMsgPrint( "Gth-Flush :" );
#endif
					LcGthBufFlush( Tv_IsNeedBdt );
				}
			}

			if ( Tv_IsNeedAlloc )
			{
				Tv_IsNeedAdd	=	FALSE;
				if ( LcGthBufAlloc( Tv_OccSize, E_GrDvrFs2GrpGthBufSize ) )
				{
					Tv_IsNeedAdd	=	TRUE;
				}
			}

			if ( Tv_IsNeedAdd )
			{
				// build head
				Tv_PtrHd	=	(Ptr_GrDvrAvHd)( (__u32)m_GthBuf + m_GthIn );
				Tv_PtrHd->Fcc				=	E_GrDvrFs2StrmFcc;
				Tv_PtrHd->Ch				=	A_Ch;
				Tv_PtrHd->Type			=	E_GrDvrAvTypRsvAdo;
				Tv_PtrHd->IsSm			=	A_IsSm;
				Tv_PtrHd->Evt				=	A_Evt;
				Tv_PtrHd->TimeSec		=	A_Time;
				Tv_PtrHd->DataSize	=	m_AdoUnitSize;
				Tv_PtrHd->TimeMsec	=	A_Mili;
				// mark tail
				Tv_PtrTail	=	(__u32*)( (__u32)Tv_PtrHd + Tv_OccSize - E_GrDvrRecTailMarkSize );
				*Tv_PtrTail	=	E_GrDvrRecTailMarkId | Tv_PktSize;
				// move buffer pointer
				m_GthIn		=	m_GthIn + Tv_OccSize;
				// update time
				if ( 0 == m_GthTmb.TimeSt )
				{
					m_GthTmb.TimeSt		=	A_Time;
					m_GthTmb.TimeEd		=	m_GthTmb.TimeSt;
					m_GthBaseTime			=	m_GthTmb.TimeSt - (m_GthTmb.TimeSt % E_GrTimeSecOfMin);
				}
				if ( m_GthTmb.TimeEd < A_Time )
				{
					m_GthTmb.TimeEd	=	A_Time;
				}
				// check need flush
				if ( m_GthIn >= m_GthAlloc )
				{
#ifdef E_GrDvrFs2GrpDbgMsgView
					DbgMsgPrint( "Gth-EFlush :" );
#endif
					LcGthBufFlush( TRUE );
				}
				Tv_Result	=	Tv_PtrHd;
			}

		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Grp::LcGthDskAdd - invalid packet size ( %d ) in.\n",Tv_OccSize );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Grp::LcHndlInfoGet( __u64 A_Hndl, __u8* A_PtrRtIntIdx, 
	__u64* A_PtrRtDpos, __u32* A_PtrRtBlkId, __u32* A_PtrRtRecId, 
	__u16* A_PtrRtOvwtId, __u32* A_PtrRtBlkOfs, __u32* A_PtrRtBlkRecSize )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Dpos;
		__u32	Tv_BlkId;
		__u32	Tv_RecId;
		__u32	Tv_BlkOfs;
		__u16	Tv_HndlOvwtId;
		__u16	Tv_DskOvwtId;
		__u8	Tv_IntIdx;
		__u32	Tv_RecSize;
	// code --------------------
		// init
		Tv_Result	=	FALSE;
			
		// extract basic info
		Tv_Dpos				=	A_Hndl & E_GrDvrFs2HndlPosMask;
		Tv_IntIdx			=	(__u8)( A_Hndl & E_GrDvrFs2HndlGrpIdxMask );
		Tv_HndlOvwtId	=	(__u16)( A_Hndl >> E_GrDvrFs2HndlOvwtIdPos );
		if ( (E_GrDvrFs2PosRda <= Tv_Dpos) && (Tv_IntIdx < m_DiskCnt) )
		{
			Tv_BlkId	=	(__u32)( (Tv_Dpos - E_GrDvrFs2PosRda) >> E_GrDvrFs2BlockShift );
			Tv_BlkOfs	=	(__u32)( (Tv_Dpos - E_GrDvrFs2PosRda) & E_GrDvrFs2BlockOfsMask );
			// get block info
			if ( m_DiskTbl[Tv_IntIdx]->BlockInfoGet( Tv_BlkId, &Tv_RecId, &Tv_RecSize, &Tv_DskOvwtId ) )
			{
				if ( Tv_HndlOvwtId == Tv_DskOvwtId )
				{
					// success
					*A_PtrRtIntIdx	=	Tv_IntIdx;
					*A_PtrRtDpos		=	Tv_Dpos;
					*A_PtrRtBlkId		=	Tv_BlkId;
					*A_PtrRtRecId		=	Tv_RecId;
					*A_PtrRtOvwtId	=	Tv_HndlOvwtId;
					*A_PtrRtBlkOfs	=	Tv_BlkOfs;
					*A_PtrRtBlkRecSize	=	Tv_RecSize;
					Tv_Result	=	TRUE;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs2Grp::LcBlockFwRecIdFind( __u32 A_RecId, __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkId, __u16* A_PtrRtOvwtId, __u32* A_PtrRtRecSize )
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_IntIdx;
		__u32	Tv_RecId;
		__u32 Tv_BlkId;
		__u16	Tv_OvwtId;
		__u32	Tv_RecSize;
	// code --------------------
		Tv_Result	=	E_GrDvrFs2RecIdNone;

		if ( E_GrDvrFs2RecIdNone != A_RecId )
		{
			for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
			{
				Tv_RecId	=	m_DiskTbl[Tv_IntIdx]->BlockFwRecIdFind( A_RecId, &Tv_BlkId, &Tv_OvwtId, &Tv_RecSize );
				if ( E_GrDvrFs2RecIdNone != Tv_RecId )
				{
					if ( Tv_RecId == A_RecId )
					{
						// found
						Tv_Result	=	Tv_RecId;
						*A_PtrRtIntIdx	=	Tv_IntIdx;
						*A_PtrRtBlkId		=	Tv_BlkId;
						*A_PtrRtOvwtId	=	Tv_OvwtId;
						*A_PtrRtRecSize	=	Tv_RecSize;
						break;
					}
					else 
					{
						if ( E_GrDvrFs2RecIdNone == Tv_Result )
						{
							// first
							Tv_Result	=	Tv_RecId;
							*A_PtrRtIntIdx	=	Tv_IntIdx;
							*A_PtrRtBlkId		=	Tv_BlkId;
							*A_PtrRtOvwtId	=	Tv_OvwtId;
							*A_PtrRtRecSize	=	Tv_RecSize;
						}
						else if ( Tv_Result > Tv_RecId )
						{
							// update new
							Tv_Result	=	Tv_RecId;
							*A_PtrRtIntIdx	=	Tv_IntIdx;
							*A_PtrRtBlkId		=	Tv_BlkId;
							*A_PtrRtOvwtId	=	Tv_OvwtId;
							*A_PtrRtRecSize	=	Tv_RecSize;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs2Grp::LcBlockBwRecIdFind( __u32 A_RecId, __u8* A_PtrRtIntIdx, __u32* A_PtrRtBlkId, __u16* A_PtrRtOvwtId, __u32* A_PtrRtRecSize )
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_IntIdx;
		__u32	Tv_RecId;
		__u32 Tv_BlkId;
		__u16	Tv_OvwtId;
		__u32	Tv_RecSize;
	// code --------------------
		Tv_Result	=	E_GrDvrFs2RecIdNone;

		if ( E_GrDvrFs2RecIdNone != A_RecId )
		{
			for ( Tv_IntIdx=0;Tv_IntIdx < m_DiskCnt;Tv_IntIdx++ )
			{
				Tv_RecId	=	m_DiskTbl[Tv_IntIdx]->BlockBwRecIdFind( A_RecId, &Tv_BlkId, &Tv_OvwtId, &Tv_RecSize );
				if ( E_GrDvrFs2RecIdNone != Tv_RecId )
				{
					if ( Tv_RecId == A_RecId )
					{
						// found
						Tv_Result	=	Tv_RecId;
						*A_PtrRtIntIdx	=	Tv_IntIdx;
						*A_PtrRtBlkId		=	Tv_BlkId;
						*A_PtrRtOvwtId	=	Tv_OvwtId;
						*A_PtrRtRecSize	=	Tv_RecSize;
						break;
					}
					else 
					{
						if ( E_GrDvrFs2RecIdNone == Tv_Result )
						{
							// first
							Tv_Result	=	Tv_RecId;
							*A_PtrRtIntIdx	=	Tv_IntIdx;
							*A_PtrRtBlkId		=	Tv_BlkId;
							*A_PtrRtOvwtId	=	Tv_OvwtId;
							*A_PtrRtRecSize	=	Tv_RecSize;
						}
						else if ( Tv_Result < Tv_RecId )
						{
							// update new
							Tv_Result	=	Tv_RecId;
							*A_PtrRtIntIdx	=	Tv_IntIdx;
							*A_PtrRtBlkId		=	Tv_BlkId;
							*A_PtrRtOvwtId	=	Tv_OvwtId;
							*A_PtrRtRecSize	=	Tv_RecSize;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::ReadByTimeFw( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
		__u64	Tv_Hndl;
		Def_GrTime	Tv_Time;
		BOOL8	Tv_IsRead;
		__u8	Tv_IntIdx;
		__u8	Tv_BufIdx;
		__u64	Tv_Dpos;
		__u32	Tv_BlkId;
		__u32	Tv_RecId;
		__u16	Tv_OvwtId;
		__u32	Tv_BlkOfs;
		__u32	Tv_RecSize;
	// code --------------------
		// check able
		if ( 0 != m_DiskCnt )
		{
			// init
			Tv_Time		=	A_Time;
			Tv_IsRead	=	FALSE;

			do 
			{
				Tv_Hndl	=	LcTimeFindFw( &Tv_Time );
				if ( E_GrDvrRecHndlNone == Tv_Hndl )
				{
					// no time exist
					A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
					break;
				}
				// get handle information
				Tv_IsRead	=	LcHndlInfoGet( Tv_Hndl, &Tv_IntIdx, &Tv_Dpos, &Tv_BlkId, &Tv_RecId, &Tv_OvwtId, 
					&Tv_BlkOfs, &Tv_RecSize );
				// next
				Tv_Time	=	Tv_Time + E_GrTimeSecOfMin;
			} while ( !Tv_IsRead );

			// check read
			if ( Tv_RecId )
			{
				// buffer change
				Tv_BufIdx	=	0;
				if ( A_PtrCtrl->IsDblBuf )
				{
					Tv_BufIdx	=	A_PtrCtrl->BufIdx;
					Tv_BufIdx	=	Tv_BufIdx ^ 1;
					// update buffer index
					A_PtrCtrl->BufIdx	=	Tv_BufIdx;
				}
				// reset
				A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
				A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;
				A_PtrCtrl->Hndl		=	Tv_Hndl;
				A_PtrCtrl->RecId	=	Tv_RecId;
				// try read
				ReadForward( A_PtrCtrl, FALSE );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::ReadByTimeBw( Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
		__u64	Tv_Hndl;
		Def_GrTime	Tv_Time;
		BOOL8	Tv_IsRead;
		__u8	Tv_IntIdx;
		__u8	Tv_BufIdx;
		__u64	Tv_Dpos;
		__u32	Tv_BlkId;
		__u32	Tv_RecId;
		__u16	Tv_OvwtId;
		__u32	Tv_BlkOfs;
		__u32	Tv_RecSize;
	// code --------------------
		// check able
		if ( 0 != m_DiskCnt )
		{
			// init
			Tv_Time		=	A_Time;
			Tv_IsRead	=	FALSE;

			do 
			{
				Tv_Hndl	=	LcTimeFindBw( &Tv_Time );
				if ( E_GrDvrRecHndlNone == Tv_Hndl )
				{
					// no time exist
					A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;
					break;
				}
				// get handle information
				Tv_IsRead	=	LcHndlInfoGet( Tv_Hndl, &Tv_IntIdx, &Tv_Dpos, &Tv_BlkId, &Tv_RecId, &Tv_OvwtId, 
					&Tv_BlkOfs, &Tv_RecSize );
				// next
				Tv_Time	=	Tv_Time - E_GrTimeSecOfMin;
			} while ( !Tv_IsRead );

			// check read
			if ( Tv_RecId )
			{
				// buffer change
				Tv_BufIdx	=	0;
				if ( A_PtrCtrl->IsDblBuf )
				{
					Tv_BufIdx	=	A_PtrCtrl->BufIdx;
					Tv_BufIdx	=	Tv_BufIdx ^ 1;
					// update buffer index
					A_PtrCtrl->BufIdx	=	Tv_BufIdx;
				}
				// reset
				A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
				A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;
				A_PtrCtrl->Hndl		=	Tv_Hndl;
				A_PtrCtrl->RecId	=	Tv_RecId;
				// try read
				ReadBackward( A_PtrCtrl, FALSE );
			}
		}
		
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs2Grp::LcTimeFindFw( Def_GrTime* A_PtrTime )
{
	// local -------------------
		__u64	Tv_Result;
		__u64	Tv_Hndl;
		Def_GrTime	Tv_TimeRslt;
		Def_GrTime	Tv_TimeChk;
		__u8	Tv_DskIdx;
	// code --------------------
		Tv_Result		=	E_GrDvrRecHndlNone;
		Tv_TimeRslt	=	*A_PtrTime;

		for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx++ )
		{
			Tv_Hndl	=	m_DiskTbl[Tv_DskIdx]->TimeHndlFindFw( Tv_TimeRslt, &Tv_TimeChk );
			if ( E_GrDvrRecHndlNone != Tv_Hndl )
			{
				if ( E_GrDvrRecHndlNone == Tv_Result )
				{
					Tv_Result		=	Tv_Hndl;
					Tv_TimeRslt	=	Tv_TimeChk;
				}
				else if ( Tv_TimeRslt < Tv_TimeChk )
				{
					Tv_Result		=	Tv_Hndl;
					Tv_TimeRslt	=	Tv_TimeChk;
				}
			}
		}

		// update time
		if ( E_GrDvrRecHndlNone != Tv_Result )
		{
			*A_PtrTime	=	Tv_TimeRslt;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs2Grp::LcTimeFindBw( Def_GrTime* A_PtrTime )
{
	// local -------------------
		__u64	Tv_Result;
		__u64	Tv_Hndl;
		Def_GrTime	Tv_TimeRslt;
		Def_GrTime	Tv_TimeChk;
		__u8	Tv_DskIdx;
	// code --------------------
		Tv_Result		=	E_GrDvrRecHndlNone;
		Tv_TimeRslt	=	*A_PtrTime;

		for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx++ )
		{
			Tv_Hndl	=	m_DiskTbl[Tv_DskIdx]->TimeHndlFindBw( Tv_TimeRslt, &Tv_TimeChk );
			if ( E_GrDvrRecHndlNone != Tv_Hndl )
			{
				if ( E_GrDvrRecHndlNone == Tv_Result )
				{
					Tv_Result		=	Tv_Hndl;
					Tv_TimeRslt	=	Tv_TimeChk;
				}
				else if ( Tv_TimeRslt > Tv_TimeChk )
				{
					Tv_Result		=	Tv_Hndl;
					Tv_TimeRslt	=	Tv_TimeChk;
				}
			}
		}

		// update time
		if ( E_GrDvrRecHndlNone != Tv_Result )
		{
			*A_PtrTime	=	Tv_TimeRslt;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Grp::LcV1DiskCount( void )
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_DskIdx;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 < m_DiskCnt )
		{
			for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx++ )
			{
				if ( m_DiskTbl[Tv_DskIdx]->IsV1System() )
				{
					Tv_Result	++;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::LcV1Upgrade( void )
{
	// local -------------------
		__u8	Tv_DskIdx;
		BOOL8	Tv_IsOk;
		BOOL8	Tv_IsDoNext;
		__u8	Tv_PastIdx;
		__u8	Tv_NearIdx;
		__u32	Tv_RecId;
		St_GrDvrFs2GrpV1UpgdInfo	Tv_DskInfo[E_GrDvrFs2MaxDriveCnt];
	// code --------------------
		// init
		DbgMsgPrint( "Cls_GrDvrFs2Grp::LcV1Upgrade - try upgrade.\n" );
		Tv_IsOk	=	TRUE;
		GrDumyZeroMem( Tv_DskInfo, sizeof(Tv_DskInfo) );
		// status analysis
		for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx++ )
		{
			Tv_DskInfo[Tv_DskIdx].IsExist	=	TRUE;
			Tv_DskInfo[Tv_DskIdx].IsV1		=	m_DiskTbl[Tv_DskIdx]->IsV1System();
			Tv_DskInfo[Tv_DskIdx].BlkPast	=	m_DiskTbl[Tv_DskIdx]->V1PastBlockCnt();
			Tv_DskInfo[Tv_DskIdx].BlkPresent	=	m_DiskTbl[Tv_DskIdx]->V1PresentBlockCnt();
			if ( !Tv_DskInfo[Tv_DskIdx].IsV1 )
			{
				if ( E_GrDvrRecStatEmpty == m_DiskTbl[Tv_DskIdx]->DiskStatGet() )
				{
					Tv_DskInfo[Tv_DskIdx].IsExist	=	FALSE;
					DbgMsgPrint( "Cls_GrDvrFs2Grp::LcV1Upgrade - disk %d empty.\n", Tv_DskIdx );
				}
			}
		}

		// check empty V1 format
		for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx++ )
		{
			if ( Tv_DskInfo[Tv_DskIdx].IsV1 && (0 == (Tv_DskInfo[Tv_DskIdx].BlkPast + Tv_DskInfo[Tv_DskIdx].BlkPresent)) )
			{
				// do format
				if ( m_DiskTbl[Tv_DskIdx]->Format( FALSE ) )
				{
					Tv_DskInfo[Tv_DskIdx].IsV1		=	FALSE;
					Tv_DskInfo[Tv_DskIdx].IsExist	=	FALSE;
					DbgMsgPrint( "Cls_GrDvrFs2Grp::LcV1Upgrade - disk %d format to V2.\n", Tv_DskIdx );
				}
				else
				{
					// appear problem
					Tv_IsOk	=	FALSE;
				}
			}
		}
		
		// check exist V1
		if ( Tv_IsOk )
		{
			// check V1 exist
			Tv_IsOk	=	FALSE;
			for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx++ )
			{
				if ( Tv_DskInfo[Tv_DskIdx].IsExist && Tv_DskInfo[Tv_DskIdx].IsV1 )
				{
					Tv_IsOk	=	TRUE;
					DbgMsgPrint( "Cls_GrDvrFs2Grp::LcV1Upgrade - disk %d is V1 type.\n", Tv_DskIdx );
					break;
				}
			}
		}

		// check convert able
		if ( Tv_IsOk )
		{
			// check disk count
			if ( 1 < m_DiskCnt )
			{
				DbgMsgPrint( "Cls_GrDvrFs2Grp::LcV1Upgrade - two disk mode.\n" );
				// check mixed
				Tv_IsDoNext	=	TRUE;
				for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx++ )
				{
					if ( Tv_DskInfo[Tv_DskIdx].IsExist && (!Tv_DskInfo[Tv_DskIdx].IsV1) )
					{
						DbgMsgPrint( "Cls_GrDvrFs2Grp::LcV1Upgrade - mix disk mode.\n" );
						// build index
						Tv_PastIdx	=	Tv_DskIdx ^ 1;
						Tv_NearIdx	=	Tv_DskIdx;
						// make V1 upgrade
						Tv_RecId	=	1;
						Tv_DskInfo[Tv_PastIdx].RecIdPast	=	Tv_RecId;
						Tv_RecId	=	Tv_RecId + Tv_DskInfo[Tv_PastIdx].BlkPast;
						Tv_DskInfo[Tv_PastIdx].RecIdPresent	=	Tv_RecId;
						Tv_RecId	=	Tv_RecId + Tv_DskInfo[Tv_PastIdx].BlkPresent;

						m_DiskTbl[Tv_PastIdx]->V1FsUpgrade( Tv_DskInfo[Tv_PastIdx].RecIdPast, Tv_DskInfo[Tv_PastIdx].RecIdPresent );
						// rebuild V2 record ID
						m_DiskTbl[Tv_NearIdx]->BdtRecIdGather( Tv_RecId );

						Tv_IsDoNext	=	FALSE;
						break;
					}
				}
				// all V1
				if ( Tv_IsDoNext )
				{
					// check overwrite exist
					Tv_IsDoNext	=	TRUE;
					for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx ++ )
					{
						if ( 0 != Tv_DskInfo[Tv_DskIdx].BlkPast )
						{
							DbgMsgPrint( "Cls_GrDvrFs2Grp::LcV1Upgrade - overwrite disk mode.\n" );

							// exist overwrite
							Tv_PastIdx	=	Tv_DskIdx;
							Tv_NearIdx	=	Tv_DskIdx ^ 1;
							// record id set
							Tv_RecId	=	1;
							Tv_DskInfo[Tv_PastIdx].RecIdPast	=	Tv_RecId;
							Tv_RecId	=	Tv_DskInfo[Tv_PastIdx].BlkPast + 1;
							Tv_DskInfo[Tv_NearIdx].RecIdPast	=	Tv_RecId;
							Tv_RecId	=	Tv_RecId + Tv_DskInfo[Tv_NearIdx].BlkPast;
							Tv_DskInfo[Tv_NearIdx].RecIdPresent	=	Tv_RecId;
							Tv_RecId	=	Tv_RecId + Tv_DskInfo[Tv_NearIdx].BlkPresent;
							Tv_DskInfo[Tv_PastIdx].RecIdPresent	=	Tv_RecId;

							// upgrade
							m_DiskTbl[0]->V1FsUpgrade( Tv_DskInfo[0].RecIdPast, Tv_DskInfo[0].RecIdPresent );
							m_DiskTbl[1]->V1FsUpgrade( Tv_DskInfo[1].RecIdPast, Tv_DskInfo[1].RecIdPresent );

							Tv_IsDoNext	=	FALSE;
						}
					}
				}
				// check full
				if ( Tv_IsDoNext )
				{
					for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx ++ )
					{
						if ( m_DiskTbl[Tv_DskIdx]->IsV1Full() )
						{
							DbgMsgPrint( "Cls_GrDvrFs2Grp::LcV1Upgrade - full disk mode.\n" );
							// found full disk
							Tv_PastIdx	=	Tv_DskIdx;
							Tv_NearIdx	=	Tv_DskIdx ^ 1;
							// build record id
							Tv_RecId	=	1;
							Tv_DskInfo[Tv_PastIdx].RecIdPresent	=	Tv_RecId;
							Tv_RecId	=	Tv_RecId + Tv_DskInfo[Tv_PastIdx].BlkPresent;
							Tv_DskInfo[Tv_NearIdx].RecIdPresent	=	Tv_RecId;

							// upgrade
							m_DiskTbl[0]->V1FsUpgrade( Tv_DskInfo[0].RecIdPast, Tv_DskInfo[0].RecIdPresent );
							m_DiskTbl[1]->V1FsUpgrade( Tv_DskInfo[1].RecIdPast, Tv_DskInfo[1].RecIdPresent );

							Tv_IsDoNext	=	FALSE;
						}
					}
				}
				// all not full
				if ( Tv_IsDoNext )
				{
					DbgMsgPrint( "Cls_GrDvrFs2Grp::LcV1Upgrade - all normal disk mode.\n" );
					// all not full
					Tv_RecId	=	1;
					Tv_DskInfo[0].RecIdPresent	=	Tv_RecId;
					Tv_RecId	=	Tv_RecId + Tv_DskInfo[0].BlkPresent;
					Tv_DskInfo[1].RecIdPresent	=	Tv_RecId;

					// upgrade
					m_DiskTbl[0]->V1FsUpgrade( Tv_DskInfo[0].RecIdPast, Tv_DskInfo[0].RecIdPresent );
					m_DiskTbl[1]->V1FsUpgrade( Tv_DskInfo[1].RecIdPast, Tv_DskInfo[1].RecIdPresent );
				}
			}
			else
			{
				// one disk
				DbgMsgPrint( "Cls_GrDvrFs2Grp::LcV1Upgrade - one disk mode.\n" );
				Tv_RecId	=	1;
				Tv_DskInfo[0].RecIdPast	=	Tv_RecId;
				Tv_RecId	=	Tv_RecId + Tv_DskInfo[0].BlkPast;
				Tv_DskInfo[0].RecIdPresent	=	Tv_RecId;

				m_DiskTbl[0]->V1FsUpgrade( Tv_DskInfo[0].RecIdPast, Tv_DskInfo[0].RecIdPresent );
			}
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Grp::RecTImeRngGet( Def_GrTime* A_PtrRtTimeBegin, Def_GrTime* A_PtrRtTimeEnd )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
		Def_GrTime	Tv_TimeSt;
		Def_GrTime	Tv_TimeEd;
		Def_GrTime	Tv_TimeGet;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_TimeSt		=	0;
		Tv_TimeEd		=	0;
		if ( 0 != m_DiskCnt )
		{
			for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx ++ )
			{
				// begin time
				Tv_TimeGet	=	m_DiskTbl[Tv_DskIdx]->BeginTimeGet();
				if ( 0 != Tv_TimeGet )
				{
					if ( 0 != Tv_TimeSt )
					{
						if ( Tv_TimeSt > Tv_TimeGet )
						{
							Tv_TimeSt	=	Tv_TimeGet;
						}
					}
					else
					{
						Tv_TimeSt	=	Tv_TimeGet;
					}
				}
				// end time
				Tv_TimeGet	=	m_DiskTbl[Tv_DskIdx]->BeginTimeGet();
				if ( 0 != Tv_TimeGet )
				{
					if ( 0 != Tv_TimeEd )
					{
						if ( Tv_TimeEd < Tv_TimeGet )
						{
							Tv_TimeEd	=	Tv_TimeGet;
						}
					}
					else
					{
						Tv_TimeEd	=	Tv_TimeGet;
					}
				}
				
			}
			// update result
			if ( (0 != Tv_TimeSt) && (0 != Tv_TimeEd) )
			{
				Tv_Result	=	TRUE;
			}
		}

		*A_PtrRtTimeBegin	=	Tv_TimeSt;
		*A_PtrRtTimeEnd		=	Tv_TimeEd;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs2Grp::TimeRecSizeCalc( __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		__u8	Tv_DskIdx;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 != m_DiskCnt )
		{
			for ( Tv_DskIdx=0;Tv_DskIdx < m_DiskCnt;Tv_DskIdx++ )
			{
				Tv_Result	=	Tv_Result + m_DiskTbl[Tv_DskIdx]->TimeRecSizeCalc( A_ChMap, A_TimeSt, A_TimeEd );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Grp::EvtLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( 0 != m_DiskCnt )
		{
			if ( E_GrDvrFs2RecIntIdxBad != m_RecIntIdx )
			{
				Tv_Result	=	m_DiskTbl[m_RecIntIdx]->EvtLogAdd( A_PtrItem );
			}
			else
			{
				DbgMsgPrint( "Cls_GrDvrFs2Grp::EvtLogAdd - record device not prepared.\n" );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::EvtLogFindStart( Ptr_GrDvrLogCtrl A_PtrCtrl )
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
		A_PtrCtrl->RtlSt.Ch			=	0;
		A_PtrCtrl->RtlEd.Time		=	0;
		A_PtrCtrl->RtlEd.Type		=	0;
		A_PtrCtrl->RtlEd.Ch			=	0;
		A_PtrCtrl->Base.Time		=	A_PtrCtrl->TimeEd;
		A_PtrCtrl->Base.Type		=	0;
		A_PtrCtrl->Base.Ch			=	0xFF;
		A_PtrCtrl->IsStart			=	TRUE;
		A_PtrCtrl->IsEnd				=	TRUE;

		if ( 0 != m_DiskCnt )
		{
			for ( Tv_DskIdx=0;Tv_DskIdx < (__u8)m_DiskCnt;Tv_DskIdx++ )
			{
				m_DiskTbl[Tv_DskIdx]->EvtLogGetNext( A_PtrCtrl );
			}
			// reference
			if ( (NULL != A_PtrCtrl->PtrRef) && (0 != A_PtrCtrl->RefCnt) )
			{
				LcEvtLogRefNext( A_PtrCtrl );
			}
			LcEvtLogSort( A_PtrCtrl );
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::EvtLogFindNext( Ptr_GrDvrLogCtrl A_PtrCtrl )
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
			A_PtrCtrl->Base.Ch		=	A_PtrCtrl->RtlSt.Ch;

			if ( 0xFF == A_PtrCtrl->Base.Ch )
			{
				A_PtrCtrl->Base.Ch	=	0;
				if ( 0xFF == A_PtrCtrl->Base.Type )
				{
					A_PtrCtrl->Base.Type	=	0;
					A_PtrCtrl->Base.Time --;
				}
				else
				{
					A_PtrCtrl->Base.Type ++;
				}
			}
			else
			{
				A_PtrCtrl->Base.Ch ++;
			}

			// init
			A_PtrCtrl->InCnt		=	0;
			A_PtrCtrl->Total		=	0;
			A_PtrCtrl->UpCnt		=	0;

			A_PtrCtrl->RtlSt.Time	=	0;
			A_PtrCtrl->RtlSt.Type	=	0;
			A_PtrCtrl->RtlSt.Ch		=	0;
			A_PtrCtrl->RtlEd.Time	=	0;
			A_PtrCtrl->RtlEd.Type	=	0;
			A_PtrCtrl->RtlEd.Ch		=	0;

			A_PtrCtrl->IsStart	=	FALSE;
			A_PtrCtrl->IsEnd		=	TRUE;

			for ( Tv_DskIdx=0;Tv_DskIdx < (__u8)m_DiskCnt;Tv_DskIdx++ )
			{
				m_DiskTbl[Tv_DskIdx]->EvtLogGetNext( A_PtrCtrl );
			}
			// reference
			if ( (NULL != A_PtrCtrl->PtrRef) && (0 != A_PtrCtrl->RefCnt) )
			{
				LcEvtLogRefNext( A_PtrCtrl );
			}

			// sort
			LcEvtLogSort( A_PtrCtrl );
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::EvtLogFindPrev( Ptr_GrDvrLogCtrl A_PtrCtrl )
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
			A_PtrCtrl->Base.Ch		=	A_PtrCtrl->RtlEd.Ch;

			if ( 0 == A_PtrCtrl->Base.Ch )
			{
				A_PtrCtrl->Base.Ch	=	0xFF;
				if ( 0 == A_PtrCtrl->Base.Type )
				{
					A_PtrCtrl->Base.Type	=	0xFF;
					A_PtrCtrl->Base.Time ++;
				}
				else
				{
					A_PtrCtrl->Base.Type --;
				}
			}
			else
			{
				A_PtrCtrl->Base.Ch --;
			}

			// init
			A_PtrCtrl->InCnt		=	0;
			A_PtrCtrl->Total		=	0;
			A_PtrCtrl->UpCnt		=	0;

			A_PtrCtrl->RtlSt.Time	=	0;
			A_PtrCtrl->RtlSt.Type	=	0;
			A_PtrCtrl->RtlSt.Ch		=	0;
			A_PtrCtrl->RtlEd.Time	=	0;
			A_PtrCtrl->RtlEd.Type	=	0;
			A_PtrCtrl->RtlEd.Ch		=	0;

			A_PtrCtrl->IsStart	=	TRUE;
			A_PtrCtrl->IsEnd		=	FALSE;

			for ( Tv_DskIdx=0;Tv_DskIdx < (__u8)m_DiskCnt;Tv_DskIdx++ )
			{
				m_DiskTbl[Tv_DskIdx]->EvtLogGetPrev( A_PtrCtrl );
			}

			// reference
			if ( (NULL != A_PtrCtrl->PtrRef) && (0 != A_PtrCtrl->RefCnt) )
			{
				LcEvtLogRefPrev( A_PtrCtrl );
			}
			// sort
			LcEvtLogSort( A_PtrCtrl );
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::LcEvtLogRefNext( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		__u32	Tv_RefIdx;
	// code --------------------
		for ( Tv_RefIdx=0;Tv_RefIdx < A_PtrCtrl->RefCnt;Tv_RefIdx++ )
		{
			if ( 0 != A_PtrCtrl->PtrRef[Tv_RefIdx].Time )
			{
				GrDvrLogV1GetFw( A_PtrCtrl, &A_PtrCtrl->PtrRef[Tv_RefIdx] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::LcEvtLogRefPrev( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		__u32	Tv_RefIdx;
	// code --------------------
		for ( Tv_RefIdx=0;Tv_RefIdx < A_PtrCtrl->RefCnt;Tv_RefIdx++ )
		{
			if ( 0 != A_PtrCtrl->PtrRef[Tv_RefIdx].Time )
			{
				GrDvrLogV1GetBw( A_PtrCtrl, &A_PtrCtrl->PtrRef[Tv_RefIdx] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Grp::LcEvtLogSort( Ptr_GrDvrLogCtrl A_PtrCtrl )
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
				Tv_FindIdx	=	GrDvrLogV1CtlFindHigh( A_PtrCtrl );
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
