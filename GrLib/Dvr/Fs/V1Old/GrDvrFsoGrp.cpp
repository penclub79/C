/*
 file system version 1 group

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <Dvr/Fs/V1Old/GrDvrFsoGrp.h>
#include <GrDebug.h>
#include <GrTaskBase.h>
#include <GrTaskSvr.h>
#include <GrError.h>

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFsoGrp::Cls_GrDvrFsoGrp( __u8 A_GrpType, BOOL8 A_IsOverWrite )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		m_IsOverWrite	=	A_IsOverWrite;
		m_IsDiskFull	=	FALSE;
		m_DskCnt			=	0;
		m_RecDskIdx		=	0;
		m_GrpType			=	A_GrpType;
		//reset table
		for ( Tv_WkIdx=0;E_GrDvrFsoGrpMaxDiskCnt > Tv_WkIdx;Tv_WkIdx++ )
		{
			m_DskTbl[Tv_WkIdx]	=	NULL;
		}
}
//--------------------------------------------------------------------
Cls_GrDvrFsoGrp::~Cls_GrDvrFsoGrp()
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------

		//release disk
		for ( Tv_WkIdx=0;E_GrDvrFsoGrpMaxDiskCnt > Tv_WkIdx;Tv_WkIdx++ )
		{
			if ( NULL != m_DskTbl[Tv_WkIdx] )
			{
				delete	m_DskTbl[Tv_WkIdx];
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoGrp::DiskAdd( Cls_GrDvrFsoDsk* A_ObjDisk )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrFsoGrpMaxDiskCnt > m_DskCnt )
		{
			m_DskTbl[m_DskCnt]	=	A_ObjDisk;
			m_DskTbl[m_DskCnt]->GroupSubIndexSet( (__u8)m_DskCnt );

			m_DskCnt ++;
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;	
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoGrp::LcFindDisk( __u8 A_DskIdx )
{
	// local -------------------
		__u8		Tv_Result;
		__u8		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoGrpLocalIdxNon;

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
BOOL8	Cls_GrDvrFsoGrp::Format( __u8 A_DskIdx, BOOL8 A_IsWithoutLog )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u8		Tv_LcIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		//find disk
		Tv_LcIdx	=	LcFindDisk( A_DskIdx );
		if ( E_GrDvrFsoGrpLocalIdxNon != Tv_LcIdx )
		{
			Tv_Result	=	m_DskTbl[Tv_LcIdx]->Format( A_IsWithoutLog );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoGrp::PacketReserve( __u32 A_Size )
{
	// local -------------------
		__u64	Tv_Result;
		__u32	Tv_Retry;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;
		if ( 0 != m_DskCnt )
		{
			Tv_Retry	=	0;
			while ( Tv_Retry <= m_DskCnt )
			{
				//try now disk recording
				Tv_Result	=	m_DskTbl[m_RecDskIdx]->PacketReserve( A_Size );
				if ( E_GrDvrFsoHndlNone != Tv_Result )
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
BOOL8	Cls_GrDvrFsoGrp::PacketAdd( Ptr_GrDvrAvHd A_PtrPkt, __u64* A_PtrRtHndl )
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
BOOL8	Cls_GrDvrFsoGrp::TmaReadCmb( Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt, void* A_PtrTmpBuf )
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
				if ( m_DskTbl[Tv_WkIdx]->TmaReadCmb( A_Time, A_PtrBuf, A_Cnt, A_PtrTmpBuf ) )
				{
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoGrp::DirectWrite( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_GrpIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_GrpIdx	=	(__u32)(A_Hndl & E_GrDvrFsoHndlGrpIdxMask);
		if ( Tv_GrpIdx < m_DskCnt )
		{
			//write
			Tv_Result	=	m_DskTbl[Tv_GrpIdx]->DirectWrite( A_Hndl, A_PtrBuf, A_Size );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoGrp::ReadNormal( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, 
																	__u64* A_PtrRtCrntHndl )
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_GrpIdx;
		__u8	Tv_RtDsk;
		__u64	Tv_Hndl;
		__u64	Tv_DskPos;
		BOOL8	Tv_IsLastHdd;
	// code --------------------
		Tv_Hndl	=	A_Hndl;
		//check disk index
		Tv_Result	=	E_GrDvrRecErrHndlBad;

		Tv_GrpIdx	=	(__u32)(Tv_Hndl & E_GrDvrFsoHndlGrpIdxMask);
		//check over disk
		if ( Tv_GrpIdx < m_DskCnt )
		{
			while ( TRUE )
			{
				//read
				Tv_RtDsk	=	m_DskTbl[Tv_GrpIdx]->ReadNormal( Tv_Hndl, A_PtrBuf, A_Size, A_PtrRtSize, A_PtrRtCrntHndl );
				if ( E_GrDvrRecErrNone == Tv_RtDsk )
				{
					//success
					Tv_Result	=	E_GrDvrRecErrNone;
					break;
				}
				else 
				{
					//error process
					if ( E_GrDvrRecErrHndlTimeOut == Tv_RtDsk )
					{
						Tv_Result	=	E_GrDvrRecErrHndlTimeOut;		//very old or futher handle
						DbgMsgPrint("Cls_GrDvrFsoGrp::ReadNormal - Handl is time out %08X%08X\n", 
							(__u32)(Tv_Hndl >> 32), (__u32)(0x00000000FFFFFFFFLL & Tv_Hndl) );
						break;
					}
					else if ( E_GrDvrRecErrHndlBad == Tv_RtDsk )
					{
						Tv_Result	=	E_GrDvrRecErrHndlBad;
						DbgMsgPrint("Cls_GrDvrFsoGrp::ReadNormal - Handl is bad %08X%08X\n", 
							(__u32)(Tv_Hndl >> 32), (__u32)(0x00000000FFFFFFFFLL & Tv_Hndl) );
						break;
					}
					//check last process
					Tv_IsLastHdd	=	FALSE;
					if ( m_RecDskIdx == Tv_GrpIdx )
					{
						//check recording or last bad
						if ( E_GrDvrRecErrDskBad == Tv_RtDsk )
						{
							//finish end
							Tv_Result	=	E_GrDvrRecErrDskEnd;
							DbgMsgPrint("Cls_GrDvrFsoGrp::ReadNormal - Record disk bad\n" );
							break;
						}
						//check overwrite hdd
						if ( m_DskTbl[Tv_GrpIdx]->IsOverwriting() )
						{
							Tv_DskPos	=	E_GrDvrFsoHndlPosMask & Tv_Hndl;
							if ( Tv_DskPos <= m_DskTbl[Tv_GrpIdx]->RecInPosGet() )
							{
								Tv_IsLastHdd	=	TRUE;
							}
						}
					}
					//check last hdd
					if ( Tv_IsLastHdd )
					{
						if ( E_GrDvrRecErrDskEnd != Tv_RtDsk )
						{
							DbgMsgPrint("Cls_GrDvrFsoGrp::ReadNormal - unexpected error type - %d\n", Tv_RtDsk);
						}
						Tv_Result	=	E_GrDvrRecErrDskEnd;
						break;
					}
					//go next disk
					Tv_GrpIdx ++;
					if ( Tv_GrpIdx >= m_DskCnt  )
					{
						Tv_GrpIdx	=	0;
					}
					//check next hdd is end
					if ( (Tv_GrpIdx == m_RecDskIdx) && (!m_DskTbl[Tv_GrpIdx]->IsOverwriting()) )
					{
						//disk end
						Tv_Result	=	E_GrDvrRecErrDskEnd;
						break;
					}
					//next hdd read
					Tv_Hndl	=	m_DskTbl[Tv_GrpIdx]->RecOutPosHndlGet();
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoGrp::ReadBack( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, 
																__u64* A_PtrRtCrntHndl )
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_GrpIdx;
		__u8	Tv_RtDsk;
		__u64	Tv_Hndl;
		__u64	Tv_DskPos;
		BOOL8	Tv_IsLastHdd;
	// code --------------------
		Tv_Hndl	=	A_Hndl;
		//check disk index
		Tv_Result	=	E_GrDvrRecErrHndlBad;
		Tv_GrpIdx	=	(__u32)(Tv_Hndl & E_GrDvrFsoHndlGrpIdxMask);
		//check over disk
		if ( Tv_GrpIdx < m_DskCnt )
		{
			while ( TRUE )
			{
				//read
				Tv_RtDsk	=	m_DskTbl[Tv_GrpIdx]->ReadBack( Tv_Hndl, A_PtrBuf, A_Size, A_PtrRtSize, A_PtrRtCrntHndl );
				if ( E_GrDvrRecErrNone == Tv_RtDsk )
				{
					//success
					Tv_Result	=	E_GrDvrRecErrNone;
					break;
				}
				else 
				{
					//error process
					if ( E_GrDvrRecErrHndlTimeOut == Tv_RtDsk )
					{
						Tv_Result	=	E_GrDvrRecErrHndlTimeOut;		//very old or futher handle
						DbgMsgPrint("Cls_GrDvrFsoGrp::ReadNormal - Handl is time out %08X%08X\n", 
							(__u32)(Tv_Hndl >> 32), (__u32)(0x00000000FFFFFFFFLL & Tv_Hndl) );
						break;
					}
					else if ( E_GrDvrRecErrHndlBad == Tv_RtDsk )
					{
						Tv_Result	=	E_GrDvrRecErrHndlBad;
						DbgMsgPrint("Cls_GrDvrFsoGrp::ReadNormal - Handl is bad %08X%08X\n", 
							(__u32)(Tv_Hndl >> 32), (__u32)(0x00000000FFFFFFFFLL & Tv_Hndl) );
						break;
					}
					//check last process
					Tv_IsLastHdd	=	FALSE;
					if ( m_RecDskIdx == Tv_GrpIdx )
					{
						//check recording or last bad
						if ( E_GrDvrRecErrDskBad == Tv_RtDsk )
						{
							//finish end
							Tv_Result	=	E_GrDvrRecErrDskEnd;
							DbgMsgPrint("Cls_GrDvrFsoGrp::ReadNormal - Record disk bad\n" );
							break;
						}
						//check overwrite hdd
						if ( m_DskTbl[Tv_GrpIdx]->IsOverwriting() )
						{
							Tv_DskPos	=	E_GrDvrFsoHndlPosMask & Tv_Hndl;
							if ( Tv_DskPos >= m_DskTbl[Tv_GrpIdx]->RecOutPosGet() )
							{
								Tv_IsLastHdd	=	TRUE;
							}
						}
					}
					//check last hdd
					if ( Tv_IsLastHdd )
					{
						if ( E_GrDvrRecErrDskEnd != Tv_RtDsk )
						{
							DbgMsgPrint("Cls_GrDvrFsoGrp::ReadNormal - unexpected error type - %d\n", Tv_RtDsk);
						}
						Tv_Result	=	E_GrDvrRecErrDskEnd;
						break;
					}
					//go next disk
					if ( 0 == Tv_GrpIdx )
					{
						Tv_GrpIdx	=	m_DskCnt;
					}
					Tv_GrpIdx --;
					//check next hdd is end
					if ( (Tv_GrpIdx == m_RecDskIdx) && (!m_DskTbl[Tv_GrpIdx]->IsOverwriting()) )
					{
						//disk end
						Tv_Result	=	E_GrDvrRecErrDskEnd;
						break;
					}
					//get next handle
					Tv_Hndl	=	m_DskTbl[Tv_GrpIdx]->RecInPosHndlGet();
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoGrp::ReadPrev( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl )
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_GrpIdx;
		__u8	Tv_RtDsk;
		__u64	Tv_Hndl;
		__u64	Tv_DskPos;
		BOOL8	Tv_IsLastHdd;
	// code --------------------
		Tv_Hndl	=	A_Hndl;
		//check disk index
		Tv_Result	=	E_GrDvrRecErrHndlBad;
		Tv_GrpIdx	=	(__u32)(Tv_Hndl & E_GrDvrFsoHndlGrpIdxMask);
		//check over disk
		if ( Tv_GrpIdx < m_DskCnt )
		{
			while ( TRUE )
			{
				//read
				Tv_RtDsk	=	m_DskTbl[Tv_GrpIdx]->ReadPrev( Tv_Hndl, A_PtrBuf, A_Size, A_PtrRtSize, A_PtrRtCrntHndl );
				if ( E_GrDvrRecErrNone == Tv_RtDsk )
				{
					//success
					Tv_Result	=	E_GrDvrRecErrNone;
					break;
				}
				else 
				{
					//error process
					if ( E_GrDvrRecErrHndlTimeOut == Tv_RtDsk )
					{
						Tv_Result	=	E_GrDvrRecErrHndlTimeOut;		//very old or futher handle
						DbgMsgPrint("Cls_GrDvrFsoGrp::ReadPrev - Handl is time out %08X%08X\n", 
							(__u32)(Tv_Hndl >> 32), (__u32)(0x00000000FFFFFFFFLL & Tv_Hndl) );
						break;
					}
					else if ( E_GrDvrRecErrHndlBad == Tv_RtDsk )
					{
						Tv_Result	=	E_GrDvrRecErrHndlBad;
						DbgMsgPrint("Cls_GrDvrFsoGrp::ReadPrev - Handl is bad %08X%08X\n", 
							(__u32)(Tv_Hndl >> 32), (__u32)(0x00000000FFFFFFFFLL & Tv_Hndl) );
						break;
					}
					//check last process
					Tv_IsLastHdd	=	FALSE;
					if ( m_RecDskIdx == Tv_GrpIdx )
					{
						//check recording or last bad
						if ( E_GrDvrRecErrDskBad == Tv_RtDsk )
						{
							//finish end
							Tv_Result	=	E_GrDvrRecErrDskEnd;
							DbgMsgPrint("Cls_GrDvrFsoGrp::ReadPrev - Record disk bad\n" );
							break;
						}
						//check overwrite hdd
						if ( m_DskTbl[Tv_GrpIdx]->IsOverwriting() )
						{
							Tv_DskPos	=	E_GrDvrFsoHndlPosMask & Tv_Hndl;
							if ( Tv_DskPos >= m_DskTbl[Tv_GrpIdx]->RecOutPosGet() )
							{
								Tv_IsLastHdd	=	TRUE;
							}
						}
					}
					//check last hdd
					if ( Tv_IsLastHdd )
					{
						if ( E_GrDvrRecErrDskEnd != Tv_RtDsk )
						{
							DbgMsgPrint("Cls_GrDvrFsoGrp::ReadPrev - unexpected error type - %d\n", Tv_RtDsk);
						}
						Tv_Result	=	E_GrDvrRecErrDskEnd;
						break;
					}
					//go next disk
					if ( 0 == Tv_GrpIdx )
					{
						Tv_GrpIdx	=	m_DskCnt;
					}
					Tv_GrpIdx --;
					//check next hdd is end
					if ( (Tv_GrpIdx == m_RecDskIdx) && (!m_DskTbl[Tv_GrpIdx]->IsOverwriting()) )
					{
						//disk end
						Tv_Result	=	E_GrDvrRecErrDskEnd;
						break;
					}
					//get next handle
					Tv_Hndl	=	m_DskTbl[Tv_GrpIdx]->RecInPosHndlGet();
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoGrp::RecBeginHndlGet( void )
{
	// local -------------------
		__u64	Tv_Result;
		__u32	Tv_GrpIdx;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;
		Tv_GrpIdx	=	m_RecDskIdx;
		//get first work
		if ( m_DskTbl[Tv_GrpIdx]->IsOverwriting() )
		{
			//finded
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
__u64	Cls_GrDvrFsoGrp::RecEndHndlGet( void )
{
		return	m_DskTbl[m_RecDskIdx]->RecInPosHndlGet();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoGrp::BufSync( void )
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
void	Cls_GrDvrFsoGrp::EvtLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
	// code --------------------
		if ( 0 != m_DskCnt )
		{
			m_DskTbl[m_RecDskIdx]->EvtLogAdd( A_PtrItem );
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFsoGrp::EvtLogGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt )
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
BOOL8	Cls_GrDvrFsoGrp::UcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFsoGrp::UcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFsoGrp::UwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFsoGrp::UwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFsoGrp::IsVssSaved( void )
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
__u8	Cls_GrDvrFsoGrp::TemperatureGet( __u8 A_DskIdx )
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
void	Cls_GrDvrFsoGrp::TemperatureUpdt( void )
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
BOOL8	Cls_GrDvrFsoGrp::WbaWrite( void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFsoGrp::WbaRead( void* A_PtrBuf, __u32 A_Size )
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
__u32	Cls_GrDvrFsoGrp::DayMapGet( Def_GrTime A_TimeDay, __u32 A_DayCnt )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 != m_DskCnt )
		{
			for (Tv_WkIdx=0;Tv_WkIdx < m_DskCnt;Tv_WkIdx++)
			{
				Tv_Result	=	Tv_Result | m_DskTbl[Tv_WkIdx]->DayMapGet( A_TimeDay, A_DayCnt );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoGrp::RecStatusGet( Ptr_GrDvrFsoRecStat A_PtrStatTbl )
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
__u64	Cls_GrDvrFsoGrp::UwaDiskPosGet( void )
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
__u64	Cls_GrDvrFsoGrp::UwaSizeGet( void )
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
__u8	Cls_GrDvrFsoGrp::UwaDiskIndexGet( void )
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
Def_GrTime	Cls_GrDvrFsoGrp::RecTimeBeginGet( void )
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
Def_GrTime	Cls_GrDvrFsoGrp::RecTimeEndGet( void )
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
void	Cls_GrDvrFsoGrp::DiskInit( void )
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
					if ( ( E_GrDvrFsoDiskStatNormal == m_DskTbl[Tv_WkIdx]->DiskStatGet() ) && 
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
BOOL8	Cls_GrDvrFsoGrp::IsDiskFull( void )
{
		return	m_IsDiskFull;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoGrp::CalcDataSize( __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, void* A_PtrTmaBuf )
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
				Tv_DataSize	=	Tv_DataSize + m_DskTbl[Tv_DskIdx]->DataSizeByMinuteRange( A_TimeSt, A_TimeEd );
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
						Tv_TmaRead	=	m_DskTbl[Tv_DskIdx]->TmaDirRead( Tv_TimeLoad, A_PtrTmaBuf, E_GrDvrFsoTmaTmpCnt, &Tv_TmaGap );
						if ( 0 != Tv_TmaRead )
						{
							//calculate 
							Tv_TimeChk	=	Tv_TimeLoad;
							for ( Tv_TmaIdx=Tv_TmaGap;Tv_TmaIdx < Tv_TmaRead;Tv_TmaIdx++ )
							{
								//check exist handle
								if ( E_GrDvrFsoHndlNone != Tv_TmaTbl[Tv_TmaIdx].HndlRec )
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
BOOL8	Cls_GrDvrFsoGrp::IsAllError( void )
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
				if ( E_GrDvrFsoDiskStatBad != m_DskTbl[Tv_WkIdx]->DiskStatGet() )
				{
					Tv_Result	=	FALSE;
					break;;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------

