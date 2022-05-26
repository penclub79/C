/*
 file system version 1 group

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <Dvr/Fs/V1/GrDvrFs1Grp.h>
#include <GrDebug.h>
#include <GrTaskBase.h>
#include <GrTaskSvr.h>
#include <GrError.h>
#include <GrDumyTool.h>

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs1Grp::Cls_GrDvrFs1Grp( __u8 A_GrpType, BOOL8 A_IsOverWrite, void* A_PtrBuf )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		m_IsOverWrite	=	A_IsOverWrite;
		m_IsDiskFull	=	FALSE;
		m_DskCnt			=	0;
		m_RecDskIdx		=	0;
		m_GrpType			=	A_GrpType;

		m_PtrBuf			=	A_PtrBuf;

		//reset table
		for ( Tv_WkIdx=0;E_GrDvrFs1GrpMaxDiskCnt > Tv_WkIdx;Tv_WkIdx++ )
		{
			m_DskTbl[Tv_WkIdx]	=	NULL;
		}
}
//--------------------------------------------------------------------
Cls_GrDvrFs1Grp::~Cls_GrDvrFs1Grp()
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------

		//release disk
		for ( Tv_WkIdx=0;E_GrDvrFs1GrpMaxDiskCnt > Tv_WkIdx;Tv_WkIdx++ )
		{
			if ( NULL != m_DskTbl[Tv_WkIdx] )
			{
				delete	m_DskTbl[Tv_WkIdx];
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::DiskAdd( Cls_GrDvrFs1Dsk* A_ObjDisk )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrFs1GrpMaxDiskCnt > m_DskCnt )
		{
			m_DskTbl[m_DskCnt]	=	A_ObjDisk;
			m_DskTbl[m_DskCnt]->GroupSubIndexSet( (__u8)m_DskCnt );

			m_DskCnt ++;
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;	
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs1Grp::LcFindDisk( __u8 A_DskIdx )
{
	// local -------------------
		__u8		Tv_Result;
		__u8		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrDvrFs1GrpLocalIdxNon;

		if ( 0 != m_DskCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_DskCnt;Tv_WkIdx++ )
			{
				if ( A_DskIdx == m_DskTbl[Tv_WkIdx]->DiskIdxGet() )
				{
					//finded
					Tv_Result	=	Tv_WkIdx;
					break;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::Format( __u8 A_DskIdx, BOOL8 A_IsWithoutLog )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u8		Tv_LcIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		//find disk
		Tv_LcIdx	=	LcFindDisk( A_DskIdx );
		if ( E_GrDvrFs1GrpLocalIdxNon != Tv_LcIdx )
		{
			Tv_Result	=	m_DskTbl[Tv_LcIdx]->Format( A_IsWithoutLog );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Grp::PacketReserve( __u32 A_Size )
{
	// local -------------------
		__u64	Tv_Result;
		__u32	Tv_Retry;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;
		if ( 0 != m_DskCnt )
		{
			Tv_Retry	=	0;
			while ( Tv_Retry <= m_DskCnt )
			{
				//try now disk recording
				Tv_Result	=	m_DskTbl[m_RecDskIdx]->PacketReserve( A_Size );
				if ( E_GrDvrRecHndlNone != Tv_Result )
				{
					//success
					break;
				}
				//next
				Tv_Retry ++;
				//get next disk index
				m_RecDskIdx ++;
				if ( m_RecDskIdx >= m_DskCnt )
				{
					m_RecDskIdx	=	0;
				}
				//check overwrite mode
				if ( m_IsOverWrite )
				{
					m_DskTbl[m_RecDskIdx]->UnlockFull();
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::PacketAdd( Ptr_GrDvrAvHd A_PtrPkt, __u64* A_PtrRtHndl )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Retry;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( 0 != m_DskCnt )
		{
			//check disk full
			if ( !m_IsDiskFull )
			{
				Tv_Retry	=	0;
				while ( Tv_Retry <= m_DskCnt )
				{
					//try now disk recording
					if ( m_DskTbl[m_RecDskIdx]->PacketAdd( A_PtrPkt, A_PtrRtHndl ) )
					{
						//success
						Tv_Result	=	TRUE;
						break;
					}
					//next
					Tv_Retry ++;
					//get next disk index
					m_RecDskIdx ++;
					if ( m_RecDskIdx >= m_DskCnt )
					{
						m_RecDskIdx	=	0;
					}
					//check overwrite mode
					if ( m_IsOverWrite )
					{
						m_DskTbl[m_RecDskIdx]->UnlockFull();
					}
				}
				//check success
				if ( (!Tv_Result) && (!m_IsOverWrite) )
				{
					m_IsDiskFull	=	TRUE;
					//notify disk full
					GrTaskSvrMsgErrorNotify( E_GrErrDiskFull, m_GrpType );
				}
			}
		}
		//check fail

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::TmaReadCmb( Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( 0 != m_DskCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_DskCnt;Tv_WkIdx++ )
			{
				if ( m_DskTbl[Tv_WkIdx]->TmaReadCmb( A_Time, A_PtrBuf, A_Cnt ) )
				{
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::DirectWrite( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_GrpIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_GrpIdx	=	(__u32)(A_Hndl & E_GrDvrFs1HndlDskIdxMask);
		if ( Tv_GrpIdx < m_DskCnt )
		{
			//write
			Tv_Result	=	m_DskTbl[Tv_GrpIdx]->DirectWrite( A_Hndl, A_PtrBuf, A_Size );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Grp::RecBeginHndlGet( void )
{
	// local -------------------
		__u64	Tv_Result;
		__u32	Tv_GrpIdx;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;
		Tv_GrpIdx	=	m_RecDskIdx;
		//get first work
		if ( m_DskTbl[Tv_GrpIdx]->IsOverwriting() )
		{
			//found
			Tv_Result	=	m_DskTbl[Tv_GrpIdx]->RecOutPosHndlGet();
		}
		else
		{
			for ( Tv_WkIdx=m_DskCnt;0 != Tv_WkIdx;Tv_WkIdx-- )
			{
				//next disk index
				if ( 0 == Tv_GrpIdx )
				{
					Tv_GrpIdx	=	m_DskCnt;
				}
				Tv_GrpIdx --;
				//check hdd
				if ( !m_DskTbl[Tv_GrpIdx]->IsEmpty() )
				{
					Tv_Result	=	m_DskTbl[Tv_GrpIdx]->RecOutPosHndlGet();
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Grp::RecEndHndlGet( void )
{
		return	m_DskTbl[m_RecDskIdx]->RecInPosHndlGet();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::BufSync( void )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		if ( 0 != m_DskCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_DskCnt;Tv_WkIdx++ )
			{
				m_DskTbl[Tv_WkIdx]->BufSync();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::EvtLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
	// code --------------------
		if ( 0 != m_DskCnt )
		{
			m_DskTbl[m_RecDskIdx]->EvtLogAdd( A_PtrItem );
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs1Grp::EvtLogGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	0;
		if ( 0 != m_DskCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_DskCnt;Tv_WkIdx++ )
			{
				if ( Tv_Result >= A_MaxCnt )
				{
					//finish
					break;
				}
				Tv_Result	=	Tv_Result + m_DskTbl[Tv_WkIdx]->EvtLogGet( A_TimeSt, A_TimeEd, A_EvtMask, A_ChMask, &A_PtrRt[Tv_Result], A_MaxCnt - Tv_Result );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::UcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 != m_DskCnt )
		{
			for ( Tv_DskIdx=0;Tv_DskIdx < m_DskCnt;Tv_DskIdx++ )
			{
				//check first hdd
				if ( 0 == m_DskTbl[Tv_DskIdx]->DiskIdxGet() )
				{
					Tv_Result	=	m_DskTbl[Tv_DskIdx]->UcaWrite( A_Pos, A_PtrBuf, A_Size );
					break;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::UcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_DskIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( 0 != m_DskCnt )
		{
			for ( Tv_DskIdx=0;Tv_DskIdx < m_DskCnt;Tv_DskIdx++ )
			{
				//check first hdd
				if ( 0 == m_DskTbl[Tv_DskIdx]->DiskIdxGet() )
				{
					Tv_Result	=	m_DskTbl[Tv_DskIdx]->UcaRead( A_Pos, A_PtrBuf, A_Size );
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::UwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 != m_DskCnt )
		{
			Tv_Result	=	m_DskTbl[0]->UwaWrite( A_Pos, A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::UwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 != m_DskCnt )
		{
			Tv_Result	=	m_DskTbl[0]->UwaRead( A_Pos, A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::IsVssSaved( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 != m_DskCnt )
		{
			Tv_Result	=	m_DskTbl[0]->IsVssSaved();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs1Grp::TemperatureGet( __u8 A_DskIdx )
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 != m_DskCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_DskCnt;Tv_WkIdx++ )
			{
				//check match index
				if ( A_DskIdx == m_DskTbl[Tv_WkIdx]->DiskIdxGet() )
				{
					Tv_Result	=	m_DskTbl[Tv_WkIdx]->TemperatureGet();
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::TemperatureUpdt( void )
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 != m_DskCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_DskCnt;Tv_WkIdx++ )
			{
				//all disk update
				m_DskTbl[Tv_WkIdx]->TemperatureUpdt();
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::WbaWrite( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 != m_DskCnt )
		{
			Tv_Result	=	m_DskTbl[0]->WbaWrite( A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::WbaRead( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 != m_DskCnt )
		{
			Tv_Result	=	m_DskTbl[0]->WbaRead( A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::RecStatusGet( Ptr_GrDvrFs1RecStat A_PtrStatTbl )
{
	// local -------------------
		__u32	Tv_WkIdx;
		__u8	Tv_DskIdx;
	// code --------------------
		if ( 0 != m_DskCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_DskCnt;Tv_WkIdx++ )
			{
				//check match index
				Tv_DskIdx	=	m_DskTbl[Tv_WkIdx]->DiskIdxGet();
				m_DskTbl[Tv_WkIdx]->RecStatusGet( &A_PtrStatTbl[Tv_DskIdx] );
				A_PtrStatTbl[Tv_DskIdx].Type	=	m_GrpType;
				if ( E_GrDvrRecDevMirror >= m_GrpType )
				{
					if ( Tv_WkIdx == m_RecDskIdx )
					{
						A_PtrStatTbl[Tv_DskIdx].IsNowRec	=	TRUE;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Grp::UwaDiskPosGet( void )
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 != m_DskCnt )
		{
			Tv_Result	=	m_DskTbl[0]->UwaDiskPosGet();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Grp::UwaSizeGet( void )
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( 0 != m_DskCnt )
		{
			Tv_Result	=	m_DskTbl[0]->UwaSizeGet();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs1Grp::UwaDiskIndexGet( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 != m_DskCnt )
		{
			Tv_Result	=	m_DskTbl[0]->DiskIdxGet();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs1Grp::RecTimeBeginGet( void )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__u32	Tv_DskIdx;
		__u32	Tv_WkCnt;
		Def_GrTime	Tv_Time;
	// code --------------------
		Tv_Result	=	0;

		//check master record
		Tv_WkCnt	=	m_DskCnt;
		Tv_DskIdx	=	m_RecDskIdx;
		while ( 0 != Tv_WkCnt )
		{
			//try get time
			Tv_Time	=	m_DskTbl[Tv_DskIdx]->RecTimeBeginGet();
			if ( 0 != Tv_Time )
			{
				if ( (0 == Tv_Result) || ( Tv_Result > Tv_Time ) )
				{
					Tv_Result	=	Tv_Time;
				}
			}
			//change disk index
			Tv_DskIdx ++;
			if ( Tv_DskIdx >= m_DskCnt )
			{
				Tv_DskIdx	=	0;
			}
			//next
			Tv_WkCnt --;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs1Grp::RecTimeEndGet( void )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__u32	Tv_DskIdx;
		__u32	Tv_WkCnt;
		Def_GrTime	Tv_Time;
	// code --------------------
		Tv_Result	=	0;

		//check master record
		Tv_WkCnt	=	m_DskCnt;
		Tv_DskIdx	=	m_RecDskIdx;
		while ( 0 != Tv_WkCnt )
		{
			//try get time
			Tv_Time	=	m_DskTbl[Tv_DskIdx]->RecTimeEndGet();
			if ( 0 != Tv_Time )
			{
				if ( Tv_Result < Tv_Time )
				{
					Tv_Result	=	Tv_Time;
				}
			}
			//change disk index
			if ( 0 == Tv_DskIdx )
			{
				Tv_DskIdx	=	m_DskCnt;
			}
			Tv_DskIdx --;
			//next
			Tv_WkCnt --;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::DiskInit( void )
{
	// local -------------------
		BOOL8	Tv_IsOk;
		__u32	Tv_WkIdx;
	// code --------------------
		//check hdd count
		if ( 1 < m_DskCnt )
		{
			Tv_IsOk	=	FALSE;
			//check overwrite mode
			if ( m_IsOverWrite )
			{
				//overwrite mode
				//find overwrited hdd
				for ( Tv_WkIdx=0;Tv_WkIdx < m_DskCnt;Tv_WkIdx++ )
				{
					//find overwriting
					if ( m_DskTbl[Tv_WkIdx]->IsOverwriting() )
					{
						//finded
						m_RecDskIdx	=	Tv_WkIdx;
						Tv_IsOk	=	TRUE;
						break;
					}
				}
			}
			//check finded
			if ( !Tv_IsOk )
			{
				//find writed hdd
				for ( Tv_WkIdx=0;Tv_WkIdx < m_DskCnt;Tv_WkIdx++ )
				{
					if ( ( E_GrDvrFs1DiskStatNormal == m_DskTbl[Tv_WkIdx]->DiskStatGet() ) && 
						(!m_DskTbl[Tv_WkIdx]->IsEmpty()) )
					{
						m_RecDskIdx	=	Tv_WkIdx;
						Tv_IsOk	=	TRUE;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::IsDiskFull( void )
{
		return	m_IsDiskFull;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Grp::CalcDataSize( __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, void* A_PtrTmaBuf )
{
	// local -------------------
		__u64	Tv_Result;
		__u64	Tv_DataSize;
		__u8	Tv_DskIdx;
		Def_GrTime	Tv_TimeLoad;
		__u32	Tv_TmaGap;
		__u32	Tv_TmaRead;
		__u64	Tv_TmaTotal;
		__u64	Tv_TmaSel;
		Def_GrTime	Tv_TimeChk;
		Ptr_GrDvrRecMinThumb	Tv_TmaTbl;
		__u32	Tv_TmaIdx;
		__u32	Tv_ChIdx;
		__u32	Tv_ChMask;
		__u8	Tv_AtbMask;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 != m_DskCnt )
		{
			Tv_DataSize	=	0;
			for ( Tv_DskIdx=0;Tv_DskIdx < (__u8)m_DskCnt;Tv_DskIdx++ )
			{
				Tv_DataSize	=	Tv_DataSize + m_DskTbl[Tv_DskIdx]->DataSizeByMinuteRange( A_TimeSt, A_TimeEd, A_PtrTmaBuf );
			}
			//calculate data
			if ( 0 != Tv_DataSize )
			{
				//tma ratio get
				Tv_TmaTbl		=	(Ptr_GrDvrRecMinThumb)A_PtrTmaBuf;
				Tv_TmaTotal	=	0;
				Tv_TmaSel		=	0;
				Tv_TimeLoad	=	A_TimeSt;
				while ( Tv_TimeLoad < A_TimeEd )
				{
					//load tma
					for ( Tv_DskIdx=0;Tv_DskIdx < (__u8)m_DskCnt;Tv_DskIdx++ )
					{
						Tv_TmaRead	=	m_DskTbl[Tv_DskIdx]->TmaDirRead( Tv_TimeLoad, A_PtrTmaBuf, E_GrDvrFs1TmaTmpCnt, &Tv_TmaGap );
						if ( 0 != Tv_TmaRead )
						{
							//calculate 
							Tv_TimeChk	=	Tv_TimeLoad;
							for ( Tv_TmaIdx=Tv_TmaGap;Tv_TmaIdx < Tv_TmaRead;Tv_TmaIdx++ )
							{
								//check exist handle
								if ( E_GrDvrRecHndlNone != Tv_TmaTbl[Tv_TmaIdx].HndlRec )
								{
									//check count
									Tv_ChMask	=	1;
									for ( Tv_ChIdx=0;Tv_ChIdx < E_GrDvrMaxChCnt;Tv_ChIdx++ )
									{
										Tv_AtbMask	=	0x0F << ((Tv_ChIdx & 1) << 2);
										if ( 0 != (Tv_AtbMask & Tv_TmaTbl[Tv_TmaIdx].ChAtb[ Tv_ChIdx >> 1 ]) )
										{
											Tv_TmaTotal	++;
											if ( 0 != (Tv_ChMask & A_ChMap) )
											{
												Tv_TmaSel ++;
											}
										}
										//next mask
										Tv_ChMask	=	Tv_ChMask << 1;
									}
								}
								//check time over
								Tv_TimeChk	=	Tv_TimeChk + E_GrTimeSecOfMin;
								if ( Tv_TimeChk >= A_TimeEd )
								{
									break;
								}
							}
						}
					}
					//next
					Tv_TimeLoad	=	Tv_TimeLoad + E_GrTimeSecOfDay;
				}
				//calculate ratio
				if ( 0 != Tv_TmaSel )
				{
					Tv_Result	=	( Tv_DataSize * Tv_TmaSel ) /  Tv_TmaTotal;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Grp::IsAllError( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	TRUE;
		if ( 0 != m_DskCnt )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_DskCnt;Tv_WkIdx++ )
			{
				if ( E_GrDvrFs1DiskStatBad != m_DskTbl[Tv_WkIdx]->DiskStatGet() )
				{
					Tv_Result	=	FALSE;
					break;;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::ReadForward( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, BOOL8 A_IsBufChg )
{
	// local -------------------
		__u8	Tv_BufIdx;
		__u8	Tv_StatPos;
		__u8	Tv_IntIdx;
		BOOL8	Tv_IsSuccess;
		BOOL8	Tv_IsOldPart;
		BOOL8	Tv_IsNextHdd;
		BOOL8	Tv_IsFound;

		BOOL8	Tv_IsNewOldPart;
		__u8	Tv_NewIntIdx;

		__u16	Tv_OvwtId;
		__u16	Tv_NewOvwtId;
		
		__u32	Tv_Prgs;
		__u32	Tv_AbleSize;
		__u32	Tv_ReadSize;

		__u64	Tv_Rpos;
		__u64	Tv_NewRpos;
		__u64	Tv_Hndl;
	// code --------------------
		// init
		Tv_IsSuccess	=	FALSE;

		// check control status
		if ( E_GrDvrSeekStatUnknown != A_PtrCtrl->Stat )
		{
			// check disk count able
			if ( 0 != m_DskCnt )
			{
				// check handle
				if ( E_GrDvrRecHndlNone != A_PtrCtrl->Hndl )
				{
					// decode handle information
					GrDvrFs1HndlDec( A_PtrCtrl->Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );

					// check disk index
					if ( Tv_IntIdx < m_DskCnt )
					{
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
							DbgMsgPrint( "Cls_GrDvrFs1Grp::ReadForward - bad progress.\n" );
						}

						// change target buffer
						if ( A_PtrCtrl->IsDblBuf && A_IsBufChg )
						{
							if ( 0 != A_PtrCtrl->Buf[Tv_BufIdx].Readed )
							{
								Tv_BufIdx	=	Tv_BufIdx ^ 1;
								// update buffer index
								A_PtrCtrl->BufIdx	=	Tv_BufIdx;
							}
						}

						// buffer reset
						A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
						A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;

						// calculate new position
						Tv_Rpos	=	Tv_Rpos + (__u64)Tv_Prgs;

						// find next data
						do
						{
							// init
							Tv_IsNextHdd	=	FALSE;
							// check disk position
							Tv_Hndl	=	GrDvrFs1HndlEnc( Tv_Rpos, Tv_IntIdx, Tv_IsOldPart, Tv_OvwtId );
							Tv_StatPos	=	m_DskTbl[Tv_IntIdx]->ChkHndlFw( Tv_Hndl, &Tv_AbleSize );
							if ( E_GrDvrFs1RposOk == Tv_StatPos )
							{
								// read
								Tv_ReadSize	=	A_PtrCtrl->Buf[Tv_BufIdx].Size;
								if ( Tv_AbleSize < Tv_ReadSize )
								{
									Tv_ReadSize	=	Tv_AbleSize;
								}
								if ( m_DskTbl[Tv_IntIdx]->ReadByRpos( Tv_Rpos, A_PtrCtrl->Buf[Tv_BufIdx].Ptr, Tv_ReadSize ) )
								{
									// success
									A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	Tv_ReadSize;
									Tv_IsSuccess	=	TRUE;
								}
								break;
							}

							// check position status
							if ( E_GrDvrFs1RposOld == Tv_StatPos )
							{
								// check this old part
								Tv_IsFound	=	FALSE;
								if ( m_DskTbl[Tv_IntIdx]->IsOldPartExist() )
								{
									Tv_Hndl	=	m_DskTbl[Tv_IntIdx]->RdaBeginHndlGet( TRUE );
									GrDvrFs1HndlDec( Tv_Hndl, &Tv_NewRpos, &Tv_NewIntIdx, &Tv_IsNewOldPart, &Tv_NewOvwtId );
									if ( (Tv_OvwtId + 1) == Tv_NewOvwtId )
									{
										Tv_IsFound	=	TRUE;
									}
								}
								if ( !Tv_IsFound )
								{
									Tv_Hndl	=	m_DskTbl[Tv_IntIdx]->RdaBeginHndlGet( FALSE );
									GrDvrFs1HndlDec( Tv_Hndl, &Tv_NewRpos, &Tv_NewIntIdx, &Tv_IsNewOldPart, &Tv_NewOvwtId );
									if ( (Tv_OvwtId + 1) == Tv_NewOvwtId )
									{
										Tv_IsFound	=	TRUE;
									}
								}

								if ( Tv_IsFound )
								{
									Tv_Rpos				=	Tv_NewRpos;
									Tv_OvwtId			=	Tv_NewOvwtId;
									Tv_IsOldPart	=	Tv_IsNewOldPart;
								}
								else
								{
									Tv_IsNextHdd	=	TRUE;
								}
							}
							else if ( E_GrDvrFs1RposEnd == Tv_StatPos )
							{
								// go next HDD
								Tv_IsNextHdd	=	TRUE;
							}
							else 
							{
								// unknown error
								DbgMsgPrint( "Cls_GrDvrFs1Grp::ReadForward - bad record position.\n" );
								A_PtrCtrl->Stat	=	E_GrDvrSeekStatUnknown;
								break;
							}

							// next HDD
							if ( Tv_IsNextHdd )
							{
								// find next HDD

								if ( 1 == m_DskCnt )
								{
									if ( Tv_IsOldPart )
									{
										// get the first new part position of this disk
										Tv_Hndl	=	m_DskTbl[Tv_IntIdx]->RdaBeginHndlGet( FALSE );
										GrDvrFs1HndlDec( Tv_Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );
									}
									else
									{
										// finish
										A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
										break;
									}
								}
								else
								{
									if ( m_RecDskIdx == Tv_IntIdx )
									{
										if ( !Tv_IsOldPart )
										{
											// finish
											A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
											break;
										}
										// next disk
										Tv_IntIdx	++;
										if ( Tv_IntIdx >= m_DskCnt )
										{
											Tv_IntIdx	=	0;
										}
										Tv_Hndl	=	m_DskTbl[Tv_IntIdx]->RdaBeginHndlGet( TRUE );
										GrDvrFs1HndlDec( Tv_Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );
									}
									else
									{
										if ( Tv_IsOldPart )
										{
											// new part change
											Tv_Hndl	=	m_DskTbl[Tv_IntIdx]->RdaBeginHndlGet( FALSE );
											GrDvrFs1HndlDec( Tv_Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );
										}
										else
										{
											// next disk
											Tv_NewIntIdx	=	Tv_IntIdx + 1;
											if ( Tv_NewIntIdx >= m_DskCnt )
											{
												Tv_NewIntIdx	=	0;
											}
											if ( m_RecDskIdx == Tv_NewIntIdx )
											{
												if ( !m_DskTbl[Tv_NewIntIdx]->IsOldPartExist() )
												{
													// finish
													A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
													break;

												}
												Tv_IntIdx	=	Tv_NewIntIdx;
												Tv_Hndl	=	m_DskTbl[Tv_IntIdx]->RdaBeginHndlGet( FALSE );
												GrDvrFs1HndlDec( Tv_Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );
											}
											else
											{
												Tv_IntIdx	=	Tv_NewIntIdx;
												Tv_Hndl	=	m_DskTbl[Tv_IntIdx]->RdaBeginHndlGet( TRUE );
												GrDvrFs1HndlDec( Tv_Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );
											}
										}
									}
								}
							}
							
						}	while( !Tv_IsSuccess );

						// update handle
						A_PtrCtrl->Hndl	=	GrDvrFs1HndlEnc( Tv_Rpos, Tv_IntIdx, Tv_IsOldPart, Tv_OvwtId );

					}	//	if ( Tv_IntIdx < m_DskCnt )
				}	//	if ( E_GrDvrRecHndlNone != A_PtrCtrl->Hndl )
			}	//	if ( 0 != m_DskCnt )

		}	//	if ( E_GrDvrSeekStatEnd != A_PtrCtrl->Stat )

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::ReadBackward( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, BOOL8 A_IsBufChg )
{
	// local -------------------
		__u8	Tv_BufIdx;
		__u8	Tv_StatPos;
		__u8	Tv_IntIdx;
		BOOL8	Tv_IsSuccess;
		BOOL8	Tv_IsOldPart;
		BOOL8	Tv_IsNextHdd;

		__u8	Tv_NewIntIdx;

		__u16	Tv_OvwtId;

		__u32	Tv_Prgs;
		__u32	Tv_AbleSize;
		__u32	Tv_ReadSize;

		__u64	Tv_Rpos;
		__u64	Tv_Hndl;
	// code --------------------
		// init
		Tv_IsSuccess	=	FALSE;

		// check control status
		if ( (E_GrDvrSeekStatEnd != A_PtrCtrl->Stat) && (E_GrDvrSeekStatUnknown != A_PtrCtrl->Stat) )
		{
			// check disk count able
			if ( 0 != m_DskCnt )
			{
				// check handle
				if ( E_GrDvrRecHndlNone != A_PtrCtrl->Hndl )
				{
					// decode handle information
					GrDvrFs1HndlDec( A_PtrCtrl->Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );

					// check disk index
					if ( Tv_IntIdx < m_DskCnt )
					{
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
							DbgMsgPrint( "Cls_GrDvrFs1Grp::ReadBackward - bad progress.\n" );
						}

						// change target buffer
						if ( A_PtrCtrl->IsDblBuf && A_IsBufChg )
						{
							if ( 0 != A_PtrCtrl->Buf[Tv_BufIdx].Readed )
							{
								Tv_BufIdx	=	Tv_BufIdx ^ 1;
								// update buffer index
								A_PtrCtrl->BufIdx	=	Tv_BufIdx;
							}
						}

						// buffer reset
						A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
						A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;

						// calculate new position
						Tv_Rpos	=	Tv_Rpos + (__u64)Tv_Prgs;

						// find next data
						do
						{
							// init
							Tv_IsNextHdd	=	FALSE;
							// check disk position
							Tv_Hndl	=	GrDvrFs1HndlEnc( Tv_Rpos, Tv_IntIdx, Tv_IsOldPart, Tv_OvwtId );
							Tv_StatPos	=	m_DskTbl[Tv_IntIdx]->ChkHndlBw( Tv_Hndl, &Tv_AbleSize );
							if ( E_GrDvrFs1RposOk == Tv_StatPos )
							{
								// read
								Tv_ReadSize	=	A_PtrCtrl->Buf[Tv_BufIdx].Size;
								if ( Tv_AbleSize < Tv_ReadSize )
								{
									Tv_ReadSize	=	Tv_AbleSize;
								}
								Tv_Rpos			=	Tv_Rpos - Tv_ReadSize;

								if ( m_DskTbl[Tv_IntIdx]->ReadByRpos( Tv_Rpos, A_PtrCtrl->Buf[Tv_BufIdx].Ptr, Tv_ReadSize ) )
								{
									// success
									A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	Tv_ReadSize;
									A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	Tv_ReadSize;
									Tv_IsSuccess	=	TRUE;
								}
								break;
							}

							// check position status
							if ( E_GrDvrFs1RposEnd == Tv_StatPos )
							{
								// go next HDD
								Tv_IsNextHdd	=	TRUE;
							}
							else 
							{
								// unknown error
								DbgMsgPrint( "Cls_GrDvrFs1Grp::ReadBackward - bad record position.\n" );
								A_PtrCtrl->Stat	=	E_GrDvrSeekStatUnknown;
								break;
							}

							// next HDD
							if ( Tv_IsNextHdd )
							{
								// find next HDD

								if ( 1 == m_DskCnt )
								{
									if ( Tv_IsOldPart )
									{
										// finish
										A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;
										break;
									}
									else
									{
										// check exist old part
										if ( !m_DskTbl[Tv_IntIdx]->IsOldPartExist() )
										{
											// finish
											A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;
											break;
										}
										// get old part end position
										Tv_Hndl	=	m_DskTbl[Tv_IntIdx]->RdaEndHndlGet( TRUE );
										GrDvrFs1HndlDec( Tv_Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );
									}
								}	//	if ( 1 == m_DskCnt )
								else
								{
									if ( m_RecDskIdx == Tv_IntIdx )
									{
										if ( Tv_IsOldPart )
										{
											// finish
											A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;
											break;
										}
										// next disk
										if ( 0 == Tv_IntIdx )
										{
											Tv_IntIdx	=	(__u8)m_DskCnt - 1;
										}
										Tv_Hndl	=	m_DskTbl[Tv_IntIdx]->RdaEndHndlGet( FALSE );
										GrDvrFs1HndlDec( Tv_Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );
									}		//	if ( m_RecDskIdx == Tv_IntIdx )
									else
									{
										if ( (!Tv_IsOldPart) && m_DskTbl[Tv_IntIdx]->IsOldPartExist() )
										{
											// new part change
											Tv_Hndl	=	m_DskTbl[Tv_IntIdx]->RdaEndHndlGet( TRUE );
											GrDvrFs1HndlDec( Tv_Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );
										}
										else
										{
											// next disk
											Tv_NewIntIdx	=	Tv_IntIdx;
											if ( 0 == Tv_NewIntIdx )
											{
												Tv_NewIntIdx	=	(__u8)m_DskCnt - 1;
											}
											else
											{
												Tv_NewIntIdx --;
											}
											if ( m_RecDskIdx == Tv_NewIntIdx )
											{
												if ( !m_DskTbl[Tv_NewIntIdx]->IsOldPartExist() )
												{
													// finish
													A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;
													break;
												}
												Tv_IntIdx	=	Tv_NewIntIdx;
												Tv_Hndl	=	m_DskTbl[Tv_IntIdx]->RdaEndHndlGet( TRUE );
												GrDvrFs1HndlDec( Tv_Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );
											}
											else
											{
												Tv_IntIdx	=	Tv_NewIntIdx;
												Tv_Hndl	=	m_DskTbl[Tv_IntIdx]->RdaEndHndlGet( FALSE );
												GrDvrFs1HndlDec( Tv_Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );
											}
										}
									}
								}
							}

						}	while( !Tv_IsSuccess );

						// update handle
						A_PtrCtrl->Hndl	=	GrDvrFs1HndlEnc( Tv_Rpos, Tv_IntIdx, Tv_IsOldPart, Tv_OvwtId );

					}	//	if ( Tv_IntIdx < m_DskCnt )
				}	//	if ( E_GrDvrRecHndlNone != A_PtrCtrl->Hndl )
			}	//	if ( 0 != m_DskCnt )

		}	//	if ( E_GrDvrSeekStatEnd != A_PtrCtrl->Stat )
		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::ReadBegin( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl )
{
	// local -------------------
		__u64	Tv_Hndl;
		BOOL8	Tv_IsFound;
		__u8	Tv_SidIdx;
		__u8	Tv_BufIdx;
	// code --------------------
		if ( 0 != m_DskCnt )
		{
			Tv_IsFound	=	FALSE;
			if ( 1 == m_DskCnt )
			{
				// one disk
				if ( m_DskTbl[m_RecDskIdx]->IsReadAble() )
				{
					Tv_Hndl			=	m_DskTbl[m_RecDskIdx]->RecOutPosHndlGet();
					Tv_IsFound	=	TRUE;
				}
			}
			else
			{
				// two disk
				Tv_SidIdx	=	(__u8)m_RecDskIdx ^ 1;
				if ( m_DskTbl[m_RecDskIdx]->IsReadAble() )
				{
					if ( m_DskTbl[m_RecDskIdx]->IsOldPartExist() )
					{
						Tv_Hndl	=	m_DskTbl[m_RecDskIdx]->RecOutPosHndlGet();
						Tv_IsFound	=	TRUE;
					}
					else
					{
						if ( m_DskTbl[Tv_SidIdx]->IsReadAble() )
						{
							Tv_Hndl	=	m_DskTbl[Tv_SidIdx]->RecOutPosHndlGet();
							Tv_IsFound	=	TRUE;
						}
						else
						{
							Tv_Hndl	=	m_DskTbl[m_RecDskIdx]->RecOutPosHndlGet();
							Tv_IsFound	=	TRUE;
						}
					}
				}
				else
				{
					if ( m_DskTbl[Tv_SidIdx]->IsReadAble() )
					{
						Tv_Hndl	=	m_DskTbl[Tv_SidIdx]->RecOutPosHndlGet();
						Tv_IsFound	=	TRUE;
					}
				}
			}

			if ( Tv_IsFound )
			{
				// change buffer
				Tv_BufIdx	=	0;
				if ( A_PtrCtrl->IsDblBuf )
				{
					if ( 0 != A_PtrCtrl->Buf[Tv_BufIdx].Readed )
					{
						Tv_BufIdx	=	A_PtrCtrl->BufIdx;
						Tv_BufIdx	=	Tv_BufIdx ^ 1;
						// update buffer index
						A_PtrCtrl->BufIdx	=	Tv_BufIdx;
					}
				}
				// reset
				A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
				A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;
				// update handle
				A_PtrCtrl->Hndl	=	Tv_Hndl;
				// try read
				A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;
				ReadForward( A_PtrCtrl, FALSE );
			}
			else
			{
				A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
			}
		}	//	if ( 0 != m_DskCnt )
		else
		{
			// disk not exist
			A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::ReadEnd( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl )
{
	// local -------------------
		__u64	Tv_Hndl;
		BOOL8	Tv_IsFound;
		__u8	Tv_SidIdx;
		__u8	Tv_BufIdx;
	// code --------------------
		if ( 0 != m_DskCnt )
		{
			Tv_IsFound	=	FALSE;
			if ( 1 == m_DskCnt )
			{
				if ( m_DskTbl[m_RecDskIdx]->IsReadAble() )
				{
					Tv_Hndl			=	m_DskTbl[m_RecDskIdx]->RecInPosHndlGet();
					Tv_IsFound	=	TRUE;
				}
			}	//	if ( 1 == m_DskCnt )
			else
			{
				// two disk
				Tv_SidIdx	=	(__u8)m_RecDskIdx ^ 1;
				if ( m_DskTbl[m_RecDskIdx]->IsReadAble() )
				{
					Tv_Hndl	=	m_DskTbl[m_RecDskIdx]->RecInPosHndlGet();
					Tv_IsFound	=	TRUE;
				}	//	if ( m_DskTbl[m_RecDskIdx]->IsReadAble() )
				else
				{
					if ( m_DskTbl[Tv_SidIdx]->IsReadAble() )
					{
						Tv_Hndl	=	m_DskTbl[Tv_SidIdx]->RecInPosHndlGet();
						Tv_IsFound	=	TRUE;
					}
				}

			}	//	if ( 1 == m_DskCnt )

			if ( Tv_IsFound )
			{
				// change buffer
				Tv_BufIdx	=	0;
				if ( A_PtrCtrl->IsDblBuf )
				{
					if ( 0 != A_PtrCtrl->Buf[Tv_BufIdx].Readed )
					{
						Tv_BufIdx	=	A_PtrCtrl->BufIdx;
						Tv_BufIdx	=	Tv_BufIdx ^ 1;
						// update buffer index
						A_PtrCtrl->BufIdx	=	Tv_BufIdx;
					}
				}
				// reset
				A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
				A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;
				// update handle
				A_PtrCtrl->Hndl	=	Tv_Hndl;
				// try read
				A_PtrCtrl->Stat	=	E_GrDvrSeekStatNormal;
				ReadBackward( A_PtrCtrl, FALSE );
			}
			else
			{
				A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
			}

		}	//	if ( 0 != m_DskCnt )
		else
		{
			A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;
		}

}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Grp::LcTimeFindFw( Ptr_GrTime A_PtrTime )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_Time;
		__u32	Tv_WkIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrTmb;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		// init
		Tv_Time	=	*A_PtrTime;
		Tv_Time	=	Tv_Time - (Tv_Time % E_GrTimeSecOfMin);

		// reset buffer
		GrDumyZeroMem( m_PtrBuf, E_GrDvrFs1GrpBufSize );
		// load TMB
		if ( TmaReadCmb( Tv_Time, m_PtrBuf,(E_GrDvrFs1GrpBufSize >> E_GrDvrRecMinThumbSizeShft) ) )
		{
			// find time
			Tv_PtrTmb	=	(Ptr_GrDvrRecMinThumb)m_PtrBuf;
			for (Tv_WkIdx= 0; Tv_WkIdx < (E_GrDvrFs1GrpBufSize >> E_GrDvrRecMinThumbSizeShft); Tv_WkIdx++)
			{
				if ( E_GrDvrRecHndlNone != Tv_PtrTmb->HndlRec )
				{
					// found
					*A_PtrTime	=	Tv_Time;
					Tv_Result		=	Tv_PtrTmb->HndlRec;
					break;
				}
				// next
				Tv_Time		=	Tv_Time + E_GrTimeSecOfMin;
				Tv_PtrTmb ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Grp::LcTimeFindBw( Ptr_GrTime A_PtrTime, Def_GrTime A_TimeLowLim )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_Time;
		Def_GrTime	Tv_Limit;
		__u32	Tv_Cnt;
		__u32	Tv_WkIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrTmb;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		// init
		Tv_Time	=	*A_PtrTime;
		Tv_Time	=	Tv_Time - (Tv_Time % E_GrTimeSecOfMin);

		Tv_Limit	=	A_TimeLowLim - (A_TimeLowLim % E_GrTimeSecOfMin);

		if ( Tv_Time > Tv_Limit )
		{
			Tv_Cnt	=	(E_GrDvrFs1GrpBufSize >> E_GrDvrRecMinThumbSizeShft);
			if ( Tv_Time < ((Tv_Cnt - 1) * E_GrTimeSecOfMin) )
			{
				Tv_Cnt	=	(Tv_Time / E_GrTimeSecOfMin) + 1;
			}
			if ( (Tv_Time - ((Tv_Cnt - 1) * E_GrTimeSecOfMin)) < Tv_Limit )
			{
				Tv_Cnt	=	((Tv_Time - Tv_Limit) / E_GrTimeSecOfMin) + 1;
			}
			// calculate start time
			Tv_Time	=	Tv_Time - ((Tv_Cnt - 1) * E_GrTimeSecOfMin);
			// reset buffer
			GrDumyZeroMem( m_PtrBuf, E_GrDvrFs1GrpBufSize );
			// load TMB
			if ( TmaReadCmb( Tv_Time, m_PtrBuf,Tv_Cnt ) )
			{
				// find time
				Tv_PtrTmb	=	(Ptr_GrDvrRecMinThumb)( (__u32)m_PtrBuf + (sizeof(St_GrDvrRecMinThumb) * (Tv_Cnt - 1)) );
				Tv_Time		=	Tv_Time + ((Tv_Cnt - 1) * E_GrTimeSecOfMin);
				for (Tv_WkIdx= 0; Tv_WkIdx < Tv_Cnt; Tv_WkIdx++)
				{
					if ( E_GrDvrRecHndlNone != Tv_PtrTmb->HndlRec )
					{
						// found
						*A_PtrTime	=	Tv_Time;
						Tv_Result		=	Tv_PtrTmb->HndlRec;
						break;
					}
					// next
					Tv_Time		=	Tv_Time - E_GrTimeSecOfMin;
					Tv_PtrTmb --;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::ReadByTimeFw( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
		__u64	Tv_Hndl;
		Def_GrTime	Tv_Time;
		__u8	Tv_BufIdx;
	// code --------------------
		// check able
		if ( 0 != m_DskCnt )
		{
			// init
			Tv_Time	=	A_Time;
			Tv_Hndl	=	LcTimeFindFw( &Tv_Time );

			if ( E_GrDvrRecHndlNone != Tv_Hndl )
			{
				A_PtrCtrl->Stat	=	E_GrDvrSeekStatNormal;
				// update buffer status
				// buffer change
				Tv_BufIdx	=	0;
				if ( A_PtrCtrl->IsDblBuf )
				{
					if ( 0 != A_PtrCtrl->Buf[Tv_BufIdx].Readed )
					{
						Tv_BufIdx	=	A_PtrCtrl->BufIdx;
						Tv_BufIdx	=	Tv_BufIdx ^ 1;
						// update buffer index
						A_PtrCtrl->BufIdx	=	Tv_BufIdx;
					}
				}
				A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
				A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;
				A_PtrCtrl->Hndl		=	Tv_Hndl;
				if ( E_GrDvrRecHndlNone != Tv_Hndl )
				{
					// try read
					ReadForward( A_PtrCtrl, FALSE );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::ReadByTimeBw( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, Def_GrTime A_Time, Def_GrTime A_TimeLimLow )
{
	// local -------------------
		__u64	Tv_Hndl;
		Def_GrTime	Tv_Time;
		__u8	Tv_BufIdx;
	// code --------------------
		if ( 0 != m_DskCnt )
		{
			// init
			Tv_Time	=	A_Time;
			Tv_Hndl	=	LcTimeFindBw( &Tv_Time, A_TimeLimLow );

			if ( E_GrDvrRecHndlNone != Tv_Hndl )
			{
				A_PtrCtrl->Stat	=	E_GrDvrSeekStatNormal;

				// update buffer status
				// buffer change
				Tv_BufIdx	=	0;
				if ( A_PtrCtrl->IsDblBuf )
				{
					if ( 0 != A_PtrCtrl->Buf[Tv_BufIdx].Readed )
					{
						Tv_BufIdx	=	A_PtrCtrl->BufIdx;
						Tv_BufIdx	=	Tv_BufIdx ^ 1;
						// update buffer index
						A_PtrCtrl->BufIdx	=	Tv_BufIdx;
					}
				}
				A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
				A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;
				A_PtrCtrl->Hndl		=	Tv_Hndl;
				if ( E_GrDvrRecHndlNone != Tv_Hndl )
				{
					// try read
					ReadBackward( A_PtrCtrl, FALSE );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::ReadByHndlFw( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
	// local -------------------
		__u8	Tv_BufIdx;
	// code --------------------
		if ( E_GrDvrRecHndlNone != A_Hndl )
		{
			// buffer change
			Tv_BufIdx	=	0;
			if ( A_PtrCtrl->IsDblBuf )
			{
				if ( 0 != A_PtrCtrl->Buf[Tv_BufIdx].Readed )
				{
					Tv_BufIdx	=	A_PtrCtrl->BufIdx;
					Tv_BufIdx	=	Tv_BufIdx ^ 1;
					// update buffer index
					A_PtrCtrl->BufIdx	=	Tv_BufIdx;
				}
			}

			// init control
			A_PtrCtrl->Hndl		=	A_Hndl;
			A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
			A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;

			// read forward
			A_PtrCtrl->Stat	=	E_GrDvrSeekStatNormal;
			ReadForward( A_PtrCtrl, FALSE );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Grp::ReadByHndlBw( Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
	// local -------------------
		__u8	Tv_BufIdx;
	// code --------------------
		if ( E_GrDvrRecHndlNone != A_Hndl )
		{
			// buffer change
			Tv_BufIdx	=	0;
			if ( A_PtrCtrl->IsDblBuf )
			{
				if ( 0 != A_PtrCtrl->Buf[Tv_BufIdx].Readed )
				{
					Tv_BufIdx	=	A_PtrCtrl->BufIdx;
					Tv_BufIdx	=	Tv_BufIdx ^ 1;
					// update buffer index
					A_PtrCtrl->BufIdx	=	Tv_BufIdx;
				}
			}

			// init control
			A_PtrCtrl->Hndl		=	A_Hndl;
			A_PtrCtrl->Buf[Tv_BufIdx].Prgs		=	0;
			A_PtrCtrl->Buf[Tv_BufIdx].Readed	=	0;

			// read forward
			A_PtrCtrl->Stat	=	E_GrDvrSeekStatNormal;
			ReadBackward( A_PtrCtrl, FALSE );
		}
}
//--------------------------------------------------------------------

