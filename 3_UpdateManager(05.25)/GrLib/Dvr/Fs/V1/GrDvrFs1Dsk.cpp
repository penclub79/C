/*
 file system version 1 disk

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrBufTool.h>
#include <Dvr/Fs/V1/GrDvrFs1Dsk.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <GrDebug.h>
#include <Dvr/Fs/V1/GrDvrFs1Mng.h>
#include <GrTaskBase.h>
#include <GrTaskSvr.h>
#include <GrError.h>

#ifdef	LINUX_APP

#define LINUX_FILE_CTRL
#define _LARGEF64_SOURCE

#include	<linux/fs.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/ioctl.h>
#include	<sys/time.h>
#include	<time.h>
#include	<pthread.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<sys/mman.h>
#include	<linux/hdreg.h>

#include	<stdio.h>
#include	<stdlib.h>

#endif

//====================================================================
//local const

#define E_GrDvrFs1DskSmartIdTemp		194

#define E_GrDvrFs1DskPktWkBufSize		0x80000

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs1Dsk::Cls_GrDvrFs1Dsk( __u8 A_DskIdx, Ptr_GrDvrFs1DskCfg A_PtrCfg, BOOL8 A_IsOverwrite, void* A_PtrTmpBuf )
{
		m_DskIdx			=	A_DskIdx;
		m_GrpSubIdx		=	0;
		m_DskSize			=	0;
		m_IsFormatted	=	FALSE;
		m_IsOverWrite	=	A_IsOverwrite;
		m_PtrCfg			=	A_PtrCfg;
		m_IsModified	=	FALSE;
		m_PtrTmpBuf		=	A_PtrTmpBuf;
		GrDumyZeroMem( &m_CchWtTma, sizeof(m_CchWtTma) );
		m_RecTimeBegin	=	0;
		m_RecTimeEnd		=	0;
		m_ValTemp				=	0;

		m_TmaTmBase			=	0xFFFFFFFF;

		if ( 512 != sizeof(St_GrDvrFs1SmartData) )
		{
			DbgMsgPrint( "GrDvrFs1Dsk: Smart struct size ( %d ) mismatch \n", sizeof(St_GrDvrFs1SmartData) );
		}

#ifdef LINUX_APP
		m_HndlDsk	=	open( m_PtrCfg->DevFnTbl[m_DskIdx], O_RDWR  );
		if ( 0 <= m_HndlDsk )
		{
			//get disk identify
			if ( 0 == ioctl( m_HndlDsk, BLKGETSIZE64, &m_DskSize ) )
			{
				LcInit();
			}
			else
			{
				//size get failure, stop run
				close( m_HndlDsk );
				m_HndlDsk	=	-1;
			}
		}
#else
		m_HndlDsk	=	CreateFileA( m_PtrCfg->DevFnTbl[m_DskIdx], GENERIC_READ | GENERIC_WRITE, 
								FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
		if ( INVALID_HANDLE_VALUE != m_HndlDsk )
		{
			if ( GetFileSizeEx( m_HndlDsk, (PLARGE_INTEGER)(&m_DskSize) ) )
			{
				LcInit();
			}
			else
			{
				CloseHandle( m_HndlDsk );
				m_HndlDsk	=	INVALID_HANDLE_VALUE;				
			}
		}
#endif
}
//--------------------------------------------------------------------
Cls_GrDvrFs1Dsk::~Cls_GrDvrFs1Dsk()
{
#ifdef LINUX_APP
		if ( 0 <= m_HndlDsk )
		{
#else
		if ( INVALID_HANDLE_VALUE != m_HndlDsk )
		{
#endif
			
			//flush all
			BufSync();

#ifdef LINUX_APP
			close( m_HndlDsk );
		}
#else
			CloseHandle( m_HndlDsk );
		}
#endif
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs1Dsk::DiskIdxGet( void )
{
		return	m_DskIdx;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::DiskSizeGet( void )
{
		return	m_DskSize;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcSeek( __s64 A_Pos, BOOL8 A_IsBegin )
{
	// local -------------------
		BOOL8			Tv_Result;
#ifdef LINUX_APP
		__s32		Tv_Mode;
#else
		__u32	Tv_Mode;
		LARGE_INTEGER		Tv_RtVal;
		LARGE_INTEGER		Tv_Pos;
#endif
	// code --------------------
		//init
		Tv_Result		=	FALSE;
#ifdef LINUX_APP
		//check open
		if ( -1 != m_HndlDsk )
		{
			Tv_Mode		=	SEEK_CUR;
			if ( A_IsBegin )
			{
				Tv_Mode		=	SEEK_SET;
			}
			if ( 0 <= lseek64( m_HndlDsk, A_Pos, Tv_Mode ) )
			{
				Tv_Result	=	TRUE;
			}
			else
			{
				//check hdd status
				if ( !LcSmartRead() )
				{
					m_Rca.DiskStat	=	E_GrDvrFs1DiskStatBad;
				}
			}
		}
#else
		//init
		Tv_Result		=	-1;

		//check open
		if ( INVALID_HANDLE_VALUE != m_HndlDsk )
		{
			//move position
			Tv_Mode	=	FILE_CURRENT;
			if ( A_IsBegin )
			{
				Tv_Mode	=	FILE_BEGIN;
			}
			Tv_Pos.QuadPart		=	A_Pos;
			if ( SetFilePointerEx( m_HndlDsk, Tv_Pos, &Tv_RtVal, Tv_Mode ) )
			{
				Tv_Result	=	TRUE;
			}
		}
#endif

		return		Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcRead( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
#ifndef LINUX_APP
		__u32		Tv_Readed;
#endif
	// code --------------------
		//init
		Tv_Result	=	FALSE;
#ifdef LINUX_APP
		//check opend
		if ( -1 != m_HndlDsk )
		{
			if ( A_Size == read( m_HndlDsk, A_PtrBuf, A_Size ) )
			{
				Tv_Result	=	TRUE;
			}
			else
			{
				//check hdd status
				if ( !LcSmartRead() )
				{
					m_Rca.DiskStat	=	E_GrDvrFs1DiskStatBad;
				}
			}
		}
#else
		//check opend
		if ( INVALID_HANDLE_VALUE != m_HndlDsk )
		{
			if ( ReadFile(m_HndlDsk, A_PtrBuf, A_Size, &Tv_Readed, NULL ) )
			{
				Tv_Result		=	TRUE;
			}
		}
#endif

		return		Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcWrite( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
#ifndef LINUX_APP
		__u32		Tv_Writed;
#endif
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( E_GrDvrFs1DiskStatBad != m_Rca.DiskStat )
		{
#ifdef LINUX_APP
			//check opened
			if ( -1 != m_HndlDsk )
			{
				if ( A_Size == write( m_HndlDsk, A_PtrBuf, A_Size ) )
				{
					Tv_Result	=	TRUE;
				}
				else
				{
					//check hdd status
					if ( !LcSmartRead() )
					{
						m_Rca.DiskStat	=	E_GrDvrFs1DiskStatBad;
					}
				}
			}
#else
			//check opened
			if ( INVALID_HANDLE_VALUE != m_HndlDsk )
			{
				if ( WriteFile(m_HndlDsk, A_PtrBuf, A_Size, &Tv_Writed, NULL) )
				{
					Tv_Result		=	TRUE;
				}
			}
#endif
		}

		return		Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Dsk::LcFlush( void )
{
#ifdef LINUX_APP
		if ( -1 != m_HndlDsk )
		{
			fsync( m_HndlDsk );
		}
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::IsFormatted( void )
{
		return	m_IsFormatted;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcInit( void )
{
	// local -------------------
		__u64	Tv_Pos;
		__u32	Tv_Size;
		BOOL8	Tv_IsAble;
	// code --------------------
		m_IsFormatted	=	FALSE;
		Tv_IsAble			=	TRUE;

		// load MIS
		if ( LcPread( E_GrDvrFs1DposMis, &m_Mis, sizeof(m_Mis) ) )
		{
			Tv_IsAble	=	FALSE;
			//check match mis
			if ( (E_GrDvrFs1Fcc == m_Mis.Fcc) && (m_PtrCfg->SysId == m_Mis.SysId) )
			{
				if ( E_GrDvrFs1Ver > m_Mis.Ver )
				{
					// upgrade
					DbgMsgPrint( "Cls_GrDvrFs1Dsk::LcInit- file system upgrade.\n" );
					// move UWA area
					LcCopy( E_GrDvrFs1DposUwa, m_Mis.UwaPos, (__u32)m_Mis.UwaSize );
					// fill zero area
					Tv_Pos	=	(__u64)(E_GrDvrFs1DposTma + (m_Mis.TmaCnt << E_GrDvrRecMinThumbSizeShft));
					Tv_Size	=	(E_GrDvrFs1TmaItemCnt - m_Mis.TmaCnt) << E_GrDvrRecMinThumbSizeShft;
					LcFillByte( Tv_Pos, Tv_Size, 0 );

					// update MIS
					m_Mis.Ver			=	E_GrDvrFs1Ver;
					m_Mis.UwaPos	=	E_GrDvrFs1DposUwa;
					m_Mis.TmaCnt	=	E_GrDvrFs1TmaItemCnt;
					Tv_IsAble	=	LcPwrite( E_GrDvrFs1DposMis, &m_Mis, sizeof(m_Mis) );
				}
				else
				{
					Tv_IsAble	=	TRUE;
				}
			}
		}
		
		// MCA
		if ( Tv_IsAble )
		{
			Tv_IsAble	=	LcPread( E_GrDvrFs1DposMca, &m_Mca, sizeof(m_Mca) );
		}

		// RCA
		if ( Tv_IsAble )
		{
			Tv_Pos		=	(__s64)(E_GrDvrFs1DposRca + (E_GrDvrFs1RcaItmSize * (__u32)m_Mca.RcaIdx) );
			Tv_IsAble	=	LcPread( Tv_Pos, &m_Rca, sizeof(m_Rca) );
			if ( Tv_IsAble )
			{
				// check need upgrade
				if ( 0 != m_Mca.RcaIdx )
				{
					DbgMsgPrint( "Cls_GrDvrFs1Dsk::LcInit - RCA address fix.\n" );
					// write RCA
					Tv_IsAble	=	LcPwrite( E_GrDvrFs1DposRca, &m_Rca, sizeof(m_Rca) );
					if ( Tv_IsAble )
					{
						// FIX RCA address
						m_Mca.RcaIdx	=	0;
						Tv_IsAble	=	LcPwrite( E_GrDvrFs1DposMca, &m_Mca, sizeof(m_Mca) );
					}
				}
			}
		}

		// init other value
		if ( Tv_IsAble )
		{
			// patch RCA status
			if ( E_GrDvrFs1DiskStatBad == m_Rca.DiskStat )
			{
				m_Rca.DiskStat	=	E_GrDvrFs1DiskStatNormal;
			}
			
			//init other value
			m_TimeLimite	=	m_Mis.TimeBase + (60 * m_Mis.TmaCnt);
			//bug fix
			if ( m_Rca.RecPosOut >= m_Mis.RdaLast )
			{
				m_Rca.RecPosOut	=	m_Mis.RdaPos;
			}
			//update temperature
			TemperatureUpdt();
			//success
			m_IsFormatted	=	TRUE;
		}

		return	m_IsFormatted;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::Format( BOOL8 A_IsWithoutLog )
{
	// local -------------------
		BOOL8	Tv_IsAble;
		BOOL8	Tv_IsFormatted;
		__u32	Tv_EvtLogPosIn;
		__u32	Tv_EvtLogPosOut;
	// code --------------------
		Tv_IsFormatted	=	m_IsFormatted;
		m_IsFormatted	=	FALSE;
		Tv_IsAble			=	FALSE;

		//check disk size
		if ( E_GrDvrFs1SizeMin <= m_DskSize )
		{
			// init
			Tv_IsAble	=	TRUE;
			//write mis
			m_Mis.Fcc			=	E_GrDvrFs1Fcc;
			m_Mis.Ver			=	E_GrDvrFs1Ver;
			m_Mis.SysId		=	m_PtrCfg->SysId;
			m_Mis.TmaCnt	=	E_GrDvrFs1TmaItemCnt;
			m_Mis.TimeBase	=	m_PtrCfg->TimeBase;
			m_Mis.UwaPos	=	E_GrDvrFs1DposTma + ( (__u64)m_Mis.TmaCnt << E_GrDvrRecMinThumbSizeShft );
			m_Mis.UwaSize	=	m_PtrCfg->UwaSize;
			m_Mis.IdaPos	=	(m_Mis.UwaPos + m_Mis.UwaSize + (~E_GrDvrFs1RdaAlignMask)) & E_GrDvrFs1RdaAlignMask;
			m_Mis.IdaSize	=	m_PtrCfg->IdaSize & E_GrDvrFs1RdaAlignMask;
			m_Mis.RdaPos	=	(m_Mis.IdaPos + m_Mis.IdaSize + (~E_GrDvrFs1RdaAlignMask)) & E_GrDvrFs1RdaAlignMask;
			m_Mis.RdaLast	=	m_DskSize & E_GrDvrFs1RdaAlignMask;
			Tv_IsAble	=	LcPwrite( E_GrDvrFs1DposMis, &m_Mis, sizeof(m_Mis) );

			// MCA
			if ( Tv_IsAble )
			{
				GrDumyZeroMem( &m_Mca, sizeof(m_Mca) );
				Tv_IsAble	=	LcPwrite( E_GrDvrFs1DposMca, &m_Mca, sizeof(m_Mca) );
			}
			
			// RCA
			if ( Tv_IsAble )
			{
				if ( Tv_IsFormatted && A_IsWithoutLog )
				{
					//backup event log value
					Tv_EvtLogPosIn	=	m_Rca.EvtPosIn;
					Tv_EvtLogPosOut	=	m_Rca.EvtPosOut;
				}
				GrDumyZeroMem( &m_Rca, sizeof(m_Rca) );
				m_Rca.RecPosIn	=	m_Mis.RdaPos;
				m_Rca.RecPosOut	=	m_Mis.RdaPos;
				if ( Tv_IsFormatted && A_IsWithoutLog )
				{
					//restore event log value
					m_Rca.EvtPosIn	=	Tv_EvtLogPosIn;
					m_Rca.EvtPosOut	=	Tv_EvtLogPosOut;
				}
				Tv_IsAble	=	LcPwrite( E_GrDvrFs1DposRca, &m_Rca, sizeof(m_Rca) );
			}

			// ELA
			if ( Tv_IsAble )
			{
				if ( Tv_IsFormatted && A_IsWithoutLog )
				{
					//disable event log data
					LcEvtLogDisable();
				}
				else
				{
					//check not formatted event log
					GrDumyZeroMem( m_PtrTmpBuf, E_GrDvrFs1ElaSize );
				}
				Tv_IsAble	=	LcPwrite( E_GrDvrFs1DposEla, m_PtrTmpBuf, E_GrDvrFs1ElaSize );
			}

			// TMA
			if ( Tv_IsAble )
			{
				Tv_IsAble	=	LcFillByte( E_GrDvrFs1DposTma, E_GrDvrFs1TmaSize, 0 );
			}

			// UWA
			if ( Tv_IsAble )
			{
				LcFillByte( E_GrDvrFs1DposUwa, 0x100, 0 );
			}
			
			// finish
			if ( Tv_IsAble )
			{
				//init runtime data
				m_TimeLimite	=	m_Mis.TimeBase + (60 * m_Mis.TmaCnt);
				m_IsModified	=	FALSE;
				m_RecTimeBegin	=	0;
				m_RecTimeEnd		=	0;
				//format completed
				m_IsFormatted	=	TRUE;
			}
		}
		// TMA cache clear
		GrDumyZeroMem( &m_CchWtTma, sizeof(m_CchWtTma) );
		m_TmaTmBase	=	0xFFFFFFFF;
		
		return	m_IsFormatted;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcIsBadBlock( __u64 A_Pos )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		__u64	Tv_NowBlkPos;
	// code --------------------
		Tv_Result			=	FALSE;
		Tv_NowBlkPos	=	A_Pos & E_GrDvrFs1RdaAlignMask;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs1BadBlkCnt;Tv_WkIdx++ )
		{
			//check current bad area
			if ( Tv_NowBlkPos == m_Mca.BadBlkTbl[Tv_WkIdx] )
			{
				//finded
				Tv_Result	=	TRUE;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcAlignPadAdd( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrAvHd	Tv_Hd;
		__u32	Tv_TailMark;
		__u32	Tv_Size;
		__u64	Tv_Pos;
		__u64	Tv_BlkNext;
	// code --------------------
		Tv_Result	=	FALSE;

		//build start id
		Tv_BlkNext	=	(m_Rca.RecPosIn + E_GrDvrFs1RdaAlignUnit) & E_GrDvrFs1RdaAlignMask;
		Tv_Size	=	( (__u32)( Tv_BlkNext - m_Rca.RecPosIn ) );
		//write head
		Tv_Hd.Fcc				=	m_PtrCfg->FccStrm;
		Tv_Hd.Type			=	E_GrDvrAvTypNotuse;
		Tv_Hd.DataSize	=	Tv_Size - sizeof(St_GrDvrAvHd) - E_GrDvrRecTailMarkSize;
		if ( LcSeek( m_Rca.RecPosIn ) )
		{
			if ( LcWrite( &Tv_Hd, sizeof(Tv_Hd) ) )
			{
				//write tail mark
				Tv_TailMark	=	E_GrDvrRecTailMarkId | Tv_Size;
				Tv_Pos		=	m_Rca.RecPosIn + (__u64)Tv_Size - E_GrDvrRecTailMarkSize;
				if ( LcSeek( Tv_Pos ) )
				{
					if ( LcWrite( &Tv_TailMark, sizeof(Tv_TailMark) ) )
					{
						//success
						m_Rca.RecPosIn	=	Tv_BlkNext;	//go next block
						Tv_Result	=	TRUE;
					}
				}
			}
			else
			{
				//add bad block
				LcBadBlockAdd( m_Rca.RecPosIn );
				m_Rca.RecPosIn	=	Tv_BlkNext;	//go next block
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcBadBlockAdd( __u64 A_Pos )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		__u64	Tv_PosBlk;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PosBlk	=	E_GrDvrFs1RdaAlignMask & A_Pos;
		//check already exist
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs1BadBlkCnt;Tv_WkIdx++ )
		{
			if ( Tv_PosBlk == m_Mca.BadBlkTbl[Tv_WkIdx] )
			{
				//already exist
				Tv_Result	=	TRUE;
				DbgMsgPrint("Cls_GrDvrFs1Dsk::LcBadBlockAdd - badblock already added - %08X%08X\n", (__u32)(A_Pos >> 32), (__u32)(A_Pos & 0x00000000FFFFFFFFLL) );
				break;
			}
		}
		//check alreayd exist
		if ( !Tv_Result )
		{
			//find empty bad block
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs1BadBlkCnt;Tv_WkIdx++ )
			{
				if ( 0 == m_Mca.BadBlkTbl[Tv_WkIdx] )
				{
					//finded
					m_Mca.BadBlkTbl[Tv_WkIdx]	=	Tv_PosBlk;
					LcMcaUpdt();		//update mca
					DbgMsgPrint("Cls_GrDvrFs1Dsk::LcBadBlockAdd - badblock add - %08X%08X\n", (__u32)(A_Pos >> 32), (__u32)(A_Pos & 0x00000000FFFFFFFFLL) );
					Tv_Result	=	TRUE;
					//disk error task message notify
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
					break;
				}
			}
			//check bad list is full
			if ( !Tv_Result )
			{
				//hdd status set to bad
				m_Rca.DiskStat	=	E_GrDvrFs1DiskStatBad;
				DbgMsgPrint("Cls_GrDvrFs1Dsk::LcBadBlockAdd - badblock full - %08X%08X\n", (__u32)(A_Pos >> 32), (__u32)(A_Pos & 0x00000000FFFFFFFFLL) );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs1Dsk::DiskStatGet( void )
{
		return	m_Rca.DiskStat;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Dsk::LcMakeSpace( void )
{
	// local -------------------
	// code --------------------
		if ( m_IsOverWrite )
		{
			//check disk full
			if ( E_GrDvrFs1DiskStatFull != m_Rca.DiskStat )
			{
				//free jam status
				if ( E_GrDvrFs1DiskStatJam == m_Rca.DiskStat )
				{
					m_Rca.DiskStat	=	E_GrDvrFs1DiskStatNormal;
				}
				else
				{
					//check need make space
					if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
					{
						//delete out block
						m_Rca.RecPosOut	=	(m_Rca.RecPosOut + E_GrDvrFs1RdaAlignUnit);	// & E_GrDvrFs1RdaAlignMask;
						//check finish
						if ( m_Mis.RdaLast <= m_Rca.RecPosOut )
						{
							//mark full
							m_Rca.DiskStat	=	E_GrDvrFs1DiskStatFull;
							m_Rca.RecPosOut	=	m_Mis.RdaPos;	//reset out position
						}
						m_RecTimeBegin	=	0;		//record time begin reset
						LcRcaWrite();
					}
				}
			}
		}
		else
		{
			//non over write mode
			//check already full
			if ( (E_GrDvrFs1DiskStatFull != m_Rca.DiskStat) && (E_GrDvrFs1DiskStatJam != m_Rca.DiskStat) )
			{
				if ( m_Rca.RecPosIn == m_Mis.RdaLast )
				{
					m_Rca.DiskStat	=	E_GrDvrFs1DiskStatFull;
				}
				else
				{
					m_Rca.DiskStat	=	E_GrDvrFs1DiskStatJam;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Dsk::LcMcaUpdt( void )
{
	// local -------------------
		BOOL8	Tv_IsSuccess;
	// code --------------------
		Tv_IsSuccess	=	FALSE;
		if ( LcSeek( E_GrDvrFs1DposMca ) )
		{
			if ( LcWrite( &m_Mca, sizeof(m_Mca)) )
			{
				Tv_IsSuccess	=	TRUE;
			}
		}
		//check bad
		if ( !Tv_IsSuccess )
		{
			m_Rca.DiskStat	=	E_GrDvrFs1DiskStatBad;
			DbgMsgPrint("Cls_GrDvrFs1Dsk::LcMcaUpdt - MCA area error!\n");
			GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Dsk::LcRcaWrite( void )
{
	// local -------------------
	// code --------------------
		if ( !LcPwrite( E_GrDvrFs1DposRca, &m_Rca, sizeof(m_Rca) ) )
		{
			m_Rca.DiskStat	=	E_GrDvrFs1DiskStatBad;
			DbgMsgPrint("Cls_GrDvrFs1Dsk::LcRcaUpdt - RCA area error!\n");
			GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
		}
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::PacketReserve( __u32 A_Size )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrAvHd	Tv_Hd;
		__u32	Tv_Tail;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		//prepare
		if ( LcPreparePktAdd( A_Size ) )
		{
			//head write
			if ( LcSeek( m_Rca.RecPosIn ) )
			{
				//make head
				Tv_Hd.Fcc		=	m_PtrCfg->FccStrm;
				Tv_Hd.Type	=	E_GrDvrAvTypReserve;
				Tv_Hd.DataSize	=	A_Size - sizeof(St_GrDvrAvHd) - E_GrDvrRecTailMarkSize;
				if ( LcWrite( &Tv_Hd, sizeof(St_GrDvrAvHd) ) )
				{
					//tail write
					if ( LcSeek( m_Rca.RecPosIn + (__u64)(A_Size - E_GrDvrRecTailMarkSize) ) )
					{
						Tv_Tail	=	E_GrDvrRecTailMarkId | A_Size;
						if ( LcWrite( &Tv_Tail, E_GrDvrRecTailMarkSize ) )
						{
							//success
							//result handle
							Tv_Result	=	m_Rca.RecPosIn | (__u64)m_GrpSubIdx | ( (__u64)m_Rca.RecSeqId << E_GrDvrFs1HndlSeqPos );
							//go next position
							m_Rca.RecPosIn	=	m_Rca.RecPosIn + (__u64)A_Size;		//go next position
							m_IsModified	=	TRUE;
						}
					}
				}
			}	//if ( LcSeek( m_Rca.RecPosIn ) )
			if ( E_GrDvrRecHndlNone == Tv_Result )
			{
				LcBadBlockAdd( m_Rca.RecPosIn );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::DirectWrite( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_DskPos;
		__u16	Tv_HndlSeqId;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_HndlSeqId	=	(__u16)( A_Hndl >> E_GrDvrFs1HndlSeqPos );
		//check bad disk
		if ( E_GrDvrFs1DiskStatBad != m_Rca.DiskStat )
		{
			//get position
			Tv_DskPos			=	A_Hndl & E_GrDvrFs1HndlPosMask;

			//check write able
			Tv_Result	=	TRUE;
			if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
			{
				if ( Tv_HndlSeqId == m_Rca.RecSeqId )
				{
					if ( Tv_DskPos >= m_Rca.RecPosIn )
					{
						Tv_Result	=	FALSE;
					}
				}
				else	//if ( Tv_HndlSeqId == m_Rca.RecSeqId )
				{
					if ( ((Tv_HndlSeqId + 1) & E_GrDvrFs1HndlSeqLimite) == m_Rca.RecSeqId )
					{
						if ( Tv_DskPos >= m_Rca.RecPosOut )
						{
							if ( Tv_DskPos >= m_Mis.RdaLast )
							{
								Tv_Result	=	FALSE;
							}
						}
						else	//if ( Tv_DskPos >= m_Rca.RecPosOut )
						{
							Tv_Result	=	FALSE;
						}
					}
					else	//if ( (Tv_HndlSeqId + 1) == m_Rca.RecSeqId )
					{
						Tv_Result	=	FALSE;
					}
				}
			}
			else	//if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
			{
				if ( Tv_HndlSeqId == m_Rca.RecSeqId )
				{
					if ( Tv_DskPos < m_Rca.RecPosIn )
					{
						if ( Tv_DskPos < m_Rca.RecPosOut )
						{
							Tv_Result	=	FALSE;
						}
					}
					else
					{
						Tv_Result	=	FALSE;
					}
				}
				else	//if ( Tv_HndlSeqId == m_Rca.RecSeqId )
				{
					Tv_Result	=	FALSE;
				}
			}
			//check second stage
			//check able
			if ( Tv_Result )
			{
				Tv_Result	=	FALSE;
				if ( !LcIsBadBlock( Tv_DskPos ) )
				{
					//write
					if ( LcSeek( Tv_DskPos ) )
					{
						//make tail
						if ( LcWrite( A_PtrBuf, A_Size ) )
						{
							//success
							Tv_Result	=	TRUE;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcPreparePktAdd( __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_BlkNext;
		__u64	Tv_FpNext;
	// code --------------------
		Tv_Result	=	FALSE;

		//check formatted
		if ( m_IsFormatted )
		{
			//check success
			while ( !Tv_Result )
			{
				//check disk bad
				if ( E_GrDvrFs1DiskStatBad == m_Rca.DiskStat )
				{
					break;
				}
				//check disk stauts normal
				if ( E_GrDvrFs1DiskStatNormal == m_Rca.DiskStat )
				{
					//input position is finished
					if ( m_Mis.RdaLast > m_Rca.RecPosIn )
					{
						//check now bad block
						Tv_BlkNext	=	(m_Rca.RecPosIn + E_GrDvrFs1RdaAlignUnit) & E_GrDvrFs1RdaAlignMask;
						if ( !LcIsBadBlock( m_Rca.RecPosIn ) )
						{
							//check block align
							Tv_FpNext		=	m_Rca.RecPosIn + (__u64)A_Size;
							if ( Tv_FpNext < Tv_BlkNext )
							{
								//success
								Tv_Result	=	TRUE;
								break;
							}
							else
							{
								//gap area set
								if ( Tv_BlkNext == m_Rca.RecPosOut )
								{
									//make space
									LcMakeSpace();
								}
								else
								{
									//block align gap build
									LcAlignPadAdd();
								}
							}	//check block align area
						}
						else
						{
							//process badblock
							//go next block
							if ( Tv_BlkNext == m_Rca.RecPosOut )
							{
								if ( m_IsOverWrite )
								{
									LcMakeSpace();
								}
								else
								{
									//disk jamm
									m_Rca.DiskStat	=	E_GrDvrFs1DiskStatJam;
									break;
								}
							}
							//move next block
							m_Rca.RecPosIn	=	Tv_BlkNext;
						}	//check bad block
					}
					else	//input position is last
					{
						//hard disk full
						m_Rca.RecPosIn	=	m_Mis.RdaLast;
						m_Rca.DiskStat	=	E_GrDvrFs1DiskStatFull;
						break;
					}
				}		
				else	// hard disk status normal check
				{
					//hard disk stauts unnormal , finish
					break;
				}
			}		//while (!Tv_Result)
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::PacketAdd( Ptr_GrDvrAvHd A_PtrPkt, __u64* A_PtrRtHndl )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_PktSize;
	// code --------------------
		Tv_Result	=	FALSE;

		//check time range
		if (m_Mis.TimeBase > A_PtrPkt->TimeSec) 
		{
			//DbgMsgPrint("Cls_GrDvrFs1Dsk::PacketAdd -  time limite less\n" );
			A_PtrPkt->TimeSec	=	m_Mis.TimeBase + (A_PtrPkt->TimeSec & 0xFFFF);	// patch time
		}
		else if (m_TimeLimite <= A_PtrPkt->TimeSec)
		{
			//DbgMsgPrint("Cls_GrDvrFs1Dsk::PacketAdd -  time limite big\n" );
			A_PtrPkt->TimeSec	=	m_Mis.TimeBase + (A_PtrPkt->TimeSec & 0xFFFF);	// patch time
		}
		//check packet size
		Tv_PktSize	=	E_GrDvrFs1StrmAlignMask & (sizeof(St_GrDvrAvHd) + A_PtrPkt->DataSize + E_GrDvrRecTailMarkSize + E_GrDvrFs1StrmAlignSize - 1);
		if ( LcPreparePktAdd( Tv_PktSize ) )
		{
			//packet write
			if ( LcSeek( m_Rca.RecPosIn ) )
			{
				if ( LcWrite( A_PtrPkt, Tv_PktSize ) )
				{
					//success
					//update last time
					if ( m_RecTimeEnd < A_PtrPkt->TimeSec)
					{
						m_RecTimeEnd	=	A_PtrPkt->TimeSec;
					}
					//update status
					if ( 0 != A_PtrPkt->Evt )
					{
						//update TMA
						LcTmaUpdt( A_PtrPkt->TimeSec, A_PtrPkt->Ch, A_PtrPkt->Evt );
					}
					//result handle
					if ( NULL != A_PtrRtHndl )
					{
						*A_PtrRtHndl	=	m_Rca.RecPosIn | (__u64)m_GrpSubIdx | ( (__u64)m_Rca.RecSeqId << E_GrDvrFs1HndlSeqPos );
					}
					m_Rca.RecPosIn	=	m_Rca.RecPosIn + (__u64)Tv_PktSize;		//go next position
					Tv_Result			=	TRUE;
					m_IsModified	=	TRUE;
				}
			}
			//not success add bad block
			if ( !Tv_Result )
			{
				LcBadBlockAdd( m_Rca.RecPosIn );
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcTmaWrite( Ptr_GrDvrRecMinThumb A_PtrTma, __u64 A_Pos )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( LcSeek( A_Pos ) )
		{
			if ( LcWrite( (void*)A_PtrTma, E_GrDvrRecMinThumbItemSize ) )
			{
				//success
				Tv_Result	=	TRUE;
			}
		}
		//check failure
		if ( !Tv_Result )
		{
			//disk status set to bad
			m_Rca.DiskStat	=	E_GrDvrFs1DiskStatBad;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Dsk::LcTmaUpdt( Def_GrTime A_Time, __u8 A_Ch, __u8 A_Atb )
{
	// local -------------------
		BOOL8		Tv_IsTmChg;
		BOOL8		Tv_isDtChg;
		__u64		Tv_Pos;
		__u8		Tv_EvtVal;
		__u8		Tv_WkIdx;
		__u8		Tv_Shft;
	// code --------------------
		// init
		Tv_IsTmChg	=	FALSE;
		// check change time
		if ( (m_TmaTmBase > A_Time) || ((m_TmaTmBase + E_GrTimeSecOfMin) <= A_Time ) )
		{
			// reload new TMA
			Tv_IsTmChg	=	TRUE;
			// reload new TMA
			m_TmaTmBase	=	A_Time - (A_Time % E_GrTimeSecOfMin);
			Tv_Pos	=	E_GrDvrFs1DposTma + ((m_TmaTmBase / E_GrTimeSecOfMin) << E_GrDvrRecMinThumbSizeShft);
			LcPread( Tv_Pos, &m_CchWtTma, sizeof(m_CchWtTma) );
			// check current handle
			if ( !LcIsCrntHndl( m_CchWtTma.HndlRec ) )
			{
				GrDumyZeroMem( &m_CchWtTma, sizeof(m_CchWtTma) );
			}
		}
		
		// check event
		Tv_isDtChg	=	FALSE;
		if ( E_GrDvrRecHndlNone == m_CchWtTma.HndlRec )
		{
			//update value
			m_CchWtTma.HndlRec	=	m_Rca.RecPosIn | ((__u64)m_Rca.RecSeqId << E_GrDvrFs1HndlSeqPos);
			Tv_isDtChg	=	TRUE;
		}
		// check event
		Tv_EvtVal	=	E_GrDvrRecAtbNoramlMask & A_Atb;
		if ( 0 != (E_GrDvrRecAtbSpcMask & A_Atb) )
		{
			Tv_EvtVal	=	Tv_EvtVal | E_GrDvrRecAtbSpcMask;
		}
		Tv_WkIdx	=	A_Ch >> 1;
		Tv_Shft		=	(A_Ch & 1) << 2;
		Tv_EvtVal	=	Tv_EvtVal << Tv_Shft;		//make 4bit attribute
		Tv_EvtVal	=	Tv_EvtVal | m_CchWtTma.ChAtb[Tv_WkIdx];
		// check change
		if ( Tv_EvtVal != m_CchWtTma.ChAtb[Tv_WkIdx] )
		{
			Tv_isDtChg	=	TRUE;
			m_CchWtTma.ChAtb[Tv_WkIdx]	=	Tv_EvtVal;
		}
		
		// update TMA
		if ( Tv_isDtChg )
		{
			Tv_Pos	=	E_GrDvrFs1DposTma + ((m_TmaTmBase / E_GrTimeSecOfMin) << E_GrDvrRecMinThumbSizeShft);
			LcPwrite( Tv_Pos, &m_CchWtTma, sizeof(m_CchWtTma) );
		}

		// update RCA
		if ( Tv_IsTmChg )
		{
			LcRcaWrite();
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Dsk::UnlockFull( void )
{
	// local -------------------
		__u8	Tv_PrvStat;
	// code --------------------
		//check disk status normal
		if ( E_GrDvrFs1DiskStatBad != m_Rca.DiskStat )
		{
			//check foramt
			if ( m_IsFormatted )
			{
				//check is full
				Tv_PrvStat	=	m_Rca.DiskStat;
				if ( E_GrDvrFs1DiskStatFull == Tv_PrvStat )
				{
					m_Rca.RecPosOut	=	m_Mis.RdaPos + E_GrDvrFs1RdaAlignUnit;
					m_Rca.RecPosIn	=	m_Mis.RdaPos;
					m_Rca.RecSeqId ++;
				}
				else
				{
					LcMakeSpace();
				}
				m_Rca.DiskStat	=	E_GrDvrFs1DiskStatNormal;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcIsCrntHndl( __u64 A_Hndl )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_SeqId;
		__u64	Tv_DataPos;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_DataPos	=	A_Hndl & E_GrDvrFs1HndlPosMask;
		Tv_SeqId		=	(__u16)(A_Hndl >> E_GrDvrFs1HndlSeqPos);
		if ( m_Rca.RecPosOut < m_Rca.RecPosIn )
		{
			if (	(Tv_SeqId == m_Rca.RecSeqId) && (m_Rca.RecPosOut <= Tv_DataPos) && 
						(m_Rca.RecPosIn > Tv_DataPos)
				 )
			{
				Tv_Result	=	TRUE;
			}
		}
		else if ( m_Rca.RecPosOut > m_Rca.RecPosIn )
		{
			if (	(((Tv_SeqId+1) == m_Rca.RecSeqId) && (m_Rca.RecPosOut <= Tv_DataPos)) || 
						((Tv_SeqId == m_Rca.RecSeqId) && (m_Rca.RecPosIn > Tv_DataPos))
					)
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs1Dsk::TmaDirRead( Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt, __u32* A_PtrRtHeadGap )
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_GrTime	Tv_TimeSt;
		__u32	Tv_TmaIdx;
		__u64	Tv_PosTma;
		__u32	Tv_ReadCnt;
		__u32	Tv_ReadSize;
		__u32	Tv_HeadGap;
	// code --------------------
		Tv_Result	=	0;
		*A_PtrRtHeadGap	=	0;

		//check disk stat
		if ( (E_GrDvrFs1DiskStatBad != m_Rca.DiskStat) && m_IsFormatted )
		{
			//check count
			if ( 0 != A_Cnt )
			{
				//patch time
				Tv_HeadGap	=	0;
				Tv_TimeSt		=	A_Time;
				Tv_ReadCnt	=	A_Cnt;
				if ( m_Mis.TimeBase > Tv_TimeSt )
				{
					Tv_TimeSt		=	m_Mis.TimeBase;
					Tv_HeadGap	=	(m_Mis.TimeBase - A_Time) / 60;
					Tv_ReadCnt	=	Tv_ReadCnt - Tv_HeadGap;
				}
				//check head position over
				if ( Tv_HeadGap < A_Cnt )
				{
					Tv_TmaIdx	=	(Tv_TimeSt - m_Mis.TimeBase) / 60;
					//patch read count
					if ( m_Mis.TmaCnt < (Tv_TmaIdx + Tv_ReadCnt) )
					{
						Tv_ReadCnt	=	m_Mis.TmaCnt - Tv_TmaIdx;
					}
					//get tma position
					Tv_PosTma	=	E_GrDvrFs1DposTma +	(__u64)( Tv_TmaIdx << E_GrDvrRecMinThumbSizeShft );
					if ( LcSeek( Tv_PosTma ) )
					{
						Tv_ReadSize	=	Tv_ReadCnt << E_GrDvrRecMinThumbSizeShft;
						if ( LcRead( A_PtrBuf, Tv_ReadSize ) )
						{
							Tv_Result	=	Tv_ReadCnt;
							*A_PtrRtHeadGap	=	Tv_HeadGap;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::TmaReadCmb( Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Cnt;
		__u32	Tv_HeadGap;
		Ptr_GrDvrRecMinThumb	Tv_PtrTg;
		Ptr_GrDvrRecMinThumb	Tv_PtrSr;
		__u32	Tv_WkIdx;
		__u32	Tv_ReadCnt;

		__u64	Tv_Rpos;
		__u8	Tv_IntIdx;
		BOOL8	Tv_IsOldPart;
		__u16	Tv_OvwtId;

		BOOL8	Tv_IsAble;

	// code --------------------
		Tv_Result	=	FALSE;

		// check read able
		if ( (E_GrDvrFs1DiskStatBad != m_Rca.DiskStat) && ( m_Rca.RecPosIn != m_Rca.RecPosOut ) )
		{
			// patch count
			Tv_Cnt		=	A_Cnt;
			if ( (E_GrDvrFs1TmpBufSize >> E_GrDvrRecMinThumbSizeShft) < Tv_Cnt )
			{
				Tv_Cnt	=	(E_GrDvrFs1TmpBufSize >> E_GrDvrRecMinThumbSizeShft);
			}

			Tv_ReadCnt	=	TmaDirRead( A_Time, m_PtrTmpBuf, Tv_Cnt, &Tv_HeadGap );
			if ( 0 != Tv_ReadCnt )
			{
				//combine data
				Tv_PtrTg	=	(Ptr_GrDvrRecMinThumb)( (__u32)A_PtrBuf + (Tv_HeadGap << E_GrDvrRecMinThumbSizeShft) );
				Tv_PtrSr	=	(Ptr_GrDvrRecMinThumb)m_PtrTmpBuf;
				//merge start
				for ( Tv_WkIdx=Tv_ReadCnt;0 != Tv_WkIdx;Tv_WkIdx-- )
				{
					//check accept data
					if ( E_GrDvrRecHndlNone != Tv_PtrSr->HndlRec )
					{
						// parse handle
						GrDvrFs1HndlDec( Tv_PtrSr->HndlRec, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );
						//check current TMA
						Tv_IsOldPart	=	FALSE;
						Tv_IsAble	=	FALSE;
						if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
						{
							if ( Tv_OvwtId == m_Rca.RecSeqId )
							{
								if ( Tv_Rpos < m_Rca.RecPosIn )
								{
									Tv_IsAble	=	TRUE;
								}
							}
							else
							{
								if ( ((Tv_OvwtId +1) == m_Rca.RecSeqId) && (Tv_Rpos >= m_Rca.RecPosOut) )
								{
									Tv_IsOldPart	=	TRUE;
									Tv_IsAble	=	TRUE;
								}
							}
						}	//	if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
						else
						{
							if ( (Tv_OvwtId == m_Rca.RecSeqId) && ( (Tv_Rpos >= m_Rca.RecPosOut) && (Tv_Rpos < m_Rca.RecPosIn) ) )
							{
								Tv_IsAble	=	TRUE;
							}
						}

						if ( Tv_IsAble )
						{
							//check target exist
							if ( E_GrDvrRecHndlNone == Tv_PtrTg->HndlRec )
							{
								// set handle
								Tv_PtrTg->HndlRec	=	GrDvrFs1HndlEnc( Tv_Rpos, m_GrpSubIdx, Tv_IsOldPart, Tv_OvwtId );
								*((__u64*)Tv_PtrTg->ChAtb)	=	*((__u64*)Tv_PtrSr->ChAtb);
							}
						}
					}

					//next
					Tv_PtrSr ++;
					Tv_PtrTg ++;
				}
				Tv_Result	=	TRUE;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs1Dsk::LcGetBufFirstTime( void* A_PtrBuf, __u32 A_BufSize )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		Ptr_GrDvrAvHd	Tv_PtrHd;
		__u32	Tv_PktSize;
		__u32	Tv_PrcSize;
	// code --------------------
		Tv_Result			=	0;
		Tv_PrcSize		=	0;
		while ( A_BufSize > Tv_PrcSize )
		{
			Tv_PtrHd		=	(Ptr_GrDvrAvHd)( (__u32)A_PtrBuf + Tv_PrcSize );
			// check head
			if ( E_GrDvrFs1StrmFcc == Tv_PtrHd->Fcc )
			{
				//check exist time
				if ( E_GrDvrAvTypAdo >= Tv_PtrHd->Type )
				{
					//found
					Tv_Result	=	Tv_PtrHd->TimeSec;
					break;
				}
				else
				{
					Tv_PktSize	=	E_GrDvrFs1StrmAlignMask & (sizeof(St_GrDvrAvHd) + Tv_PtrHd->DataSize + E_GrDvrRecTailMarkSize + E_GrDvrFs1StrmAlignSize - 1);
					if ( 0 == Tv_PktSize )
					{
						Tv_PktSize	=	E_GrDvrFs1StrmAlignSize;
					}
				}
			}
			else
			{
				Tv_PktSize	=	E_GrDvrFs1StrmAlignSize;
			}

			//next
			Tv_PrcSize	=	Tv_PrcSize + Tv_PktSize;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs1Dsk::LcCalcBufFrmsSizeNormal( void* A_PtrBuf, __u32 A_BufSize )
{
	// local -------------------
		__u32	Tv_Result;
		Ptr_GrDvrAvHd	Tv_PtrHd;
		__u32	Tv_PktSize;
	// code --------------------
		Tv_Result		=	0;
		while ( A_BufSize > Tv_Result )
		{
			Tv_PtrHd		=	(Ptr_GrDvrAvHd)( (__u32)A_PtrBuf + Tv_Result );
			Tv_PktSize	=	E_GrDvrFs1StrmAlignSize;
			if ( m_PtrCfg->FccStrm == Tv_PtrHd->Fcc )
			{
				Tv_PktSize	=	E_GrDvrFs1StrmAlignMask & (sizeof(St_GrDvrAvHd) + Tv_PtrHd->DataSize + E_GrDvrRecTailMarkSize + E_GrDvrFs1StrmAlignSize - 1);
			}
			if ( A_BufSize < (Tv_Result + Tv_PktSize) )
			{
				break;
			}
			//next
			Tv_Result	=	Tv_Result + Tv_PktSize;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs1Dsk::LcGetBufLastTime( void* A_PtrBuf, __u32 A_BufSize )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__u32		Tv_PktSize;
		__u32*	Tv_PtrTail;
		Ptr_GrDvrAvHd	Tv_PtrHd;
		__u32		Tv_PrcSize;
	// code --------------------
		Tv_Result			=	0;
		Tv_PrcSize		=	0;
		while ( A_BufSize > Tv_PrcSize )
		{
			Tv_PtrTail	=	(__u32*)( (__u32)A_PtrBuf + A_BufSize - Tv_PrcSize - E_GrDvrRecTailMarkSize );
			Tv_PktSize	=	E_GrDvrRecTailSizeMask & (*Tv_PtrTail);
	#ifndef LINUX_APP
			if ( E_GrDvrRecTailMarkId != (E_GrDvrRecTailMarkMask & (*Tv_PtrTail)) )
			{
				DbgMsgPrint( "Cls_GrDvrFs1Dsk::LcCalcBufFrmsSizeBack: bad tail code.\n" );
			}
	#endif
			if ( A_BufSize < (Tv_PrcSize + Tv_PktSize) )
			{
				break;
			}
			//check time
			Tv_PtrHd	=	(Ptr_GrDvrAvHd)( (__u32)Tv_PtrTail - Tv_PktSize + E_GrDvrRecTailMarkSize );
			if ( E_GrDvrAvTypAdo >= Tv_PtrHd->Type )
			{
				// found
				Tv_Result	=	Tv_PtrHd->TimeSec;
				break;
			}
			//next
			Tv_PrcSize	=	Tv_PrcSize + Tv_PktSize;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs1Dsk::LcCalcBufFrmsSizeBack( void* A_PtrBuf, __u32 A_BufSize )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_PktSize;
		__u32*	Tv_PtrTail;
	// code --------------------
		Tv_Result		=	0;
		while ( A_BufSize > Tv_Result )
		{
			Tv_PtrTail	=	(__u32*)( (__u32)A_PtrBuf + A_BufSize - Tv_Result - E_GrDvrRecTailMarkSize );
			Tv_PktSize	=	E_GrDvrRecTailSizeMask & (*Tv_PtrTail);
#ifndef LINUX_APP
			if ( E_GrDvrRecTailMarkId != (E_GrDvrRecTailMarkMask & (*Tv_PtrTail)) )
			{
				DbgMsgPrint( "Cls_GrDvrFs1Dsk::LcCalcBufFrmsSizeBack: bad tail code.\n" );
				Tv_PktSize	=	E_GrDvrFs1StrmAlignSize;
			}
#endif
			if ( A_BufSize < (Tv_Result + Tv_PktSize) )
			{
				break;
			}
			//next
			Tv_Result	=	Tv_Result + Tv_PktSize;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::LcMakeHndlByPos( __u64 A_Pos )
{
	// local -------------------
		__u16	Tv_SeqId;
		BOOL8	Tv_IsOldPart;
	// code --------------------
		Tv_IsOldPart	=	FALSE;
		Tv_SeqId	=	m_Rca.RecSeqId;
		if ( (m_Rca.RecPosIn < m_Rca.RecPosOut) && (A_Pos >= m_Rca.RecPosOut) )
		{
			Tv_SeqId--;
			Tv_IsOldPart	=	TRUE;
		}
		
		return	GrDvrFs1HndlEnc( A_Pos, m_GrpSubIdx, Tv_IsOldPart, Tv_SeqId );
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::RecOutPosHndlGet( void )
{
		return	LcMakeHndlByPos( m_Rca.RecPosOut );
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::RecInPosHndlGet( void )
{
		return	LcMakeHndlByPos( m_Rca.RecPosIn );
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::RdaPosHndlGet( void )
{
		return	LcMakeHndlByPos( m_Mis.RdaPos );
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::RdaLastHndlGet( void )
{
	return	LcMakeHndlByPos( m_Mis.RdaLast );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Dsk::BufSync( void )
{
	// local -------------------
	// code --------------------
		//check HDD bad
		if ( E_GrDvrFs1DiskStatBad != m_Rca.DiskStat )
		{
			//check modified
			if ( m_IsModified )
			{
				//update RCA
				LcRcaWrite();
			}
			LcFlush();
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Dsk::EvtLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		__u32	Tv_PosIn;
		__u32	Tv_PosOut;
		__u64	Tv_DskPos;
	// code --------------------
		//direct write event LOG
		//calculate disk position

		//check disk status
		if (E_GrDvrFs1DiskStatBad != m_Rca.DiskStat)
		{
			//write item
			A_PtrItem->IsAble	=	TRUE;		//patch able
			Tv_PosIn	=	m_Rca.EvtPosIn;
			Tv_DskPos	=	E_GrDvrFs1DposEla + (__u64)(Tv_PosIn << E_GrDvrFs1ElaItmSzSft);
			if ( LcSeek( Tv_DskPos ) )
			{
				if ( LcWrite( A_PtrItem, E_GrDvrFs1ElaItmSize ) )
				{
					//update next position
					//get next pos
					Tv_PosIn ++;
					if ( E_GrDvrFs1ElaMaxItmCnt <= Tv_PosIn )
					{
						Tv_PosIn	=	0;
					}
					Tv_PosOut	=	m_Rca.EvtPosOut;
					//check last item
					if ( Tv_PosIn == Tv_PosOut )
					{
						//delete item
						Tv_PosOut	++;
						if ( E_GrDvrFs1ElaMaxItmCnt <= Tv_PosOut )
						{
							Tv_PosOut	=	0;
						}
					}

					//update info
					m_Rca.EvtPosOut	=	Tv_PosOut;
					m_Rca.EvtPosIn	=	Tv_PosIn;
					m_IsModified		=	TRUE;		//rca modified
				}
				else
				{
					m_Rca.DiskStat	=	E_GrDvrFs1DiskStatBad;
					DbgMsgPrint("Cls_GrDvrFs1Dsk::EvtLogAdd - log area bad!\n");
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
				}
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs1Dsk::EvtLogGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_WkIdx;
		__s32		Tv_ChkIdx;
		__u32	Tv_Mask;
		__u8	Tv_TypeId;
		Ptr_GrDvrLogItm	Tv_PtrLogTbl;
		__u32	Tv_ChMask;
	// code --------------------
		Tv_Result	=	0;

		//check disk status
		if (E_GrDvrFs1DiskStatBad != m_Rca.DiskStat)
		{
			//load event log
			if ( LcSeek( E_GrDvrFs1DposEla ) )
			{
				if ( LcRead( m_PtrTmpBuf, E_GrDvrFs1ElaSize ) )
				{
					Tv_PtrLogTbl	=	(Ptr_GrDvrLogItm)m_PtrTmpBuf;
					Tv_ChkIdx			=	(__s32)m_Rca.EvtPosIn;
					for ( Tv_WkIdx=0; E_GrDvrFs1ElaMaxItmCnt > Tv_WkIdx;Tv_WkIdx++ )
					{
						//get preview
						Tv_ChkIdx --;
						if ( 0 > Tv_ChkIdx )
						{
							Tv_ChkIdx	=	E_GrDvrFs1ElaMaxItmCnt - 1;
						}
						//check over
						/*
						if ( 0 == Tv_PtrLogTbl[Tv_ChkIdx].Time )
						{
							//finish
							break;
						}
						*/
						//check count over
						if ( A_MaxCnt <= Tv_Result )
						{
							break;
						}
						
						//check event
						Tv_TypeId	=	Tv_PtrLogTbl[Tv_ChkIdx].Type >> E_GrDvrLogTypeMjShft;
						Tv_Mask		=	(1 << (__u32)Tv_TypeId);
						Tv_ChMask	=	1 << Tv_PtrLogTbl[Tv_ChkIdx].Ch;
						if ( (0 != (Tv_Mask & A_EvtMask )) && ((0xFF == Tv_PtrLogTbl[Tv_ChkIdx].Ch) || ( 0 != (Tv_ChMask & A_ChMask) )  ) )
						{
							//check time range
							if ( (A_TimeSt <= Tv_PtrLogTbl[Tv_ChkIdx].Time) && (A_TimeEd >= Tv_PtrLogTbl[Tv_ChkIdx].Time) )
							{
								//found, add
								A_PtrRt[Tv_Result]	=	Tv_PtrLogTbl[Tv_ChkIdx];
								Tv_Result ++;
							}
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::UcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result; 
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if (E_GrDvrFs1DiskStatBad != m_Rca.DiskStat)
		{
			if ( E_GrDvrFs1UcaMaxSize >= (A_Pos + A_Size) )
			{
				if ( LcSeek( E_GrDvrFs1DposUca + (__u64)A_Pos ) )
				{
					if ( LcWrite( A_PtrBuf, A_Size ) )
					{
						Tv_Result	=	TRUE;
#ifdef	LINUX_APP
						sync();
#endif
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::UcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result; 
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if (E_GrDvrFs1DiskStatBad != m_Rca.DiskStat)
		{
			if ( E_GrDvrFs1UcaMaxSize >= (A_Pos + A_Size) )
			{
				if ( LcSeek( E_GrDvrFs1DposUca + (__u64)A_Pos ) )
				{
					if ( LcRead( A_PtrBuf, A_Size ) )
					{
						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::UwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result; 
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if (E_GrDvrFs1DiskStatBad != m_Rca.DiskStat)
		{
			if ( m_Mis.UwaSize >= (A_Pos + (__u64)A_Size) )
			{
				if ( LcSeek( m_Mis.UwaPos + A_Pos ) )
				{
					if ( LcWrite( A_PtrBuf, A_Size ) )
					{
						Tv_Result	=	TRUE;
						//update VSS use
						if ( !m_Rca.IsVss )
						{
							m_Rca.IsVss	=	TRUE;
							LcRcaWrite();
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::UwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result; 
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if (E_GrDvrFs1DiskStatBad != m_Rca.DiskStat)
		{
			if ( m_Mis.UwaSize >= (A_Pos + (__u64)A_Size) )
			{
				if ( LcSeek( m_Mis.UwaPos + A_Pos ) )
				{
					if ( LcRead( A_PtrBuf, A_Size ) )
					{
						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcSmartRead( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
		if ( 0 <= m_HndlDsk )
		{
			m_SmartIoc.Cmds[0]	=	WIN_SMART;
			m_SmartIoc.Cmds[1]	=	0;
			m_SmartIoc.Cmds[2]	=	SMART_READ_VALUES;
			m_SmartIoc.Cmds[3]	=	1;
			if ( 0 == ioctl(m_HndlDsk, HDIO_DRIVE_CMD, &m_SmartIoc) ) 
			{
				Tv_Result	=	TRUE;
			}
		}
#endif
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcSmartFindVal( __u8 A_Id, __u8* A_PtrRtVal )
{
	// local -------------------	
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_BytePos;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_BytePos	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs1SmartValCnt;Tv_WkIdx++ )
		{
			if ( A_Id == m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFs1SmartValIdxId] )
			{
				*A_PtrRtVal	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFs1SmartValIdxValue];
				Tv_Result	=	TRUE;
				break;
			}
			//next
			Tv_BytePos	=	Tv_BytePos + E_GrDvrFs1SmartValItemSize;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal )
{
	// local -------------------	
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_BytePos;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_BytePos	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs1SmartValCnt;Tv_WkIdx++ )
		{
			if ( A_Id == m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFs1SmartValIdxId] )
			{
				*A_PtrRtVal	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFs1SmartValIdxRawVal];
				Tv_Result	=	TRUE;
				break;
			}
			//next
			Tv_BytePos	=	Tv_BytePos + E_GrDvrFs1SmartValItemSize;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs1Dsk::TemperatureGet( void )
{
	// local -------------------
	// code --------------------
		return	m_ValTemp;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::WbaWrite( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result; 
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if (E_GrDvrFs1DiskStatBad != m_Rca.DiskStat)
		{
			if ( E_GrDvrFs1WbaSize >= A_Size )
			{
				if ( LcSeek( E_GrDvrFs1DposWba ) )
				{
					if ( LcWrite( A_PtrBuf, A_Size ) )
					{
						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::WbaRead( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result; 
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if (E_GrDvrFs1DiskStatBad != m_Rca.DiskStat)
		{
			if ( E_GrDvrFs1WbaSize >= A_Size )
			{
				if ( LcSeek( E_GrDvrFs1DposWba ) )
				{
					if ( LcRead( A_PtrBuf, A_Size ) )
					{
						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Dsk::LcEvtLogDisable( void )
{
	// local -------------------
		Ptr_GrDvrLogItm	Tv_PtrLogTbl;
		__u32	Tv_Cnt;
	// code --------------------
		//load event log
		if ( LcSeek( E_GrDvrFs1DposEla ) )
		{
			if ( LcRead( m_PtrTmpBuf, E_GrDvrFs1ElaSize ) )
			{
				Tv_PtrLogTbl	=	(Ptr_GrDvrLogItm)m_PtrTmpBuf;
				for ( Tv_Cnt=E_GrDvrFs1ElaMaxItmCnt;0 != Tv_Cnt;Tv_Cnt-- )
				{
					//disable
					Tv_PtrLogTbl->IsAble	=	FALSE;
					//next
					Tv_PtrLogTbl ++;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Dsk::RecStatusGet( Ptr_GrDvrFs1RecStat A_PtrStat )
{
	// local -------------------
	// code --------------------
		A_PtrStat->IsFormatted	=	m_IsFormatted;
		A_PtrStat->IsNowRec			=	FALSE;
		A_PtrStat->IsBad				=	FALSE;
		A_PtrStat->IsOvwt				=	FALSE;
		A_PtrStat->Temp					=	m_ValTemp;
		A_PtrStat->DiskSize			=	m_DskSize;
		if ( m_IsFormatted )
		{
			A_PtrStat->DiskSize	=	m_Mis.RdaLast - m_Mis.RdaPos;
			if (E_GrDvrFs1DiskStatBad == m_Rca.DiskStat)
			{
				A_PtrStat->IsBad	=	TRUE;
				A_PtrStat->RecSize	=	A_PtrStat->DiskSize;
			}
			else
			{
				//check full
				if ( E_GrDvrFs1DiskStatNormal == m_Rca.DiskStat)
				{
					A_PtrStat->RecSize	=	m_Rca.RecPosIn - m_Rca.RecPosOut;
					if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
					{
						A_PtrStat->RecSize	=	A_PtrStat->DiskSize +	m_Rca.RecPosIn - m_Rca.RecPosOut;
						A_PtrStat->IsOvwt		=	TRUE;
					}
				}
				else
				{
					//full
					A_PtrStat->IsFull		=	TRUE;
					A_PtrStat->RecSize	=	A_PtrStat->DiskSize;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Dsk::GroupSubIndexSet( __u8 A_Idx )
{
		m_GrpSubIdx	=	A_Idx;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::UwaDiskPosGet( void )
{
		return	m_Mis.UwaPos;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::UwaSizeGet( void )
{
		return	m_Mis.UwaSize;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::IsOverwriting( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsFormatted )
		{
			if ( E_GrDvrFs1DiskStatBad != m_Rca.DiskStat )
			{
				if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
				{
					Tv_Result	=	TRUE;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::IsEmpty( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsFormatted )
		{
			if ( m_Rca.RecPosIn == m_Rca.RecPosOut )
			{
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::RecOutPosGet( void )
{
		return	m_Rca.RecPosOut;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::RecInPosGet( void )
{
		return	m_Rca.RecPosIn;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::RdaStartPosGet( void )
{
		return	m_Mis.RdaPos;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::RdaLastPosGet( void )
{
		return	m_Mis.RdaLast;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs1Dsk::RecTimeBeginGet( void )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__u64				Tv_Rpos;
		__u64				Tv_ReadAble;
	// code --------------------
		Tv_Result	=	m_RecTimeBegin;
		// check need recalculate
		if ( 0 == Tv_Result )
		{
			// disk data exist
			if ( m_Rca.RecPosIn != m_Rca.RecPosOut )
			{
				// check bad disk
				Tv_Rpos	=	m_Rca.RecPosOut;

				while ( E_GrDvrFs1DiskStatBad != m_Rca.DiskStat )
				{
					// calculate read size
					if ( Tv_Rpos > m_Rca.RecPosIn )
					{
						Tv_ReadAble	=	m_Mis.RdaLast - Tv_Rpos;
					}
					else
					{
						Tv_ReadAble	=	m_Rca.RecPosIn - Tv_Rpos;
					}
					
					if ( E_GrDvrFs1DskPktWkBufSize < Tv_ReadAble )
					{
						Tv_ReadAble	=	E_GrDvrFs1DskPktWkBufSize;
					}

					if ( 0 != Tv_ReadAble )
					{
						if ( !ReadByRpos( Tv_Rpos, m_PtrTmpBuf, (__u32)Tv_ReadAble ) )
						{
							// error
							DbgMsgPrint( "Cls_GrDvrFs1Dsk::RecTimeBeginGet - read error!\n" );
							break;
						}
						// get first packet
						Tv_Result	=	LcGetBufFirstTime( m_PtrTmpBuf, (__u32)Tv_ReadAble );
						if ( 0 != Tv_Result )
						{
							break;
						}
					}
					// next
					Tv_Rpos	=	Tv_Rpos + Tv_ReadAble;

					if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
					{
						if ( Tv_Rpos == m_Mis.RdaLast )
						{
							Tv_Rpos	=	m_Mis.RdaPos;
						}
						else if ( Tv_Rpos == m_Rca.RecPosIn )
						{
							break;
						}
					}
					else
					{
						if ( Tv_Rpos == m_Rca.RecPosIn )
						{
							break;
						}
					}

				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs1Dsk::RecTimeEndGet( void )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__u64				Tv_Rpos;
		__u64				Tv_ReadAble;
	// code --------------------
		Tv_Result	=	m_RecTimeEnd;
		if ( 0 == Tv_Result )
		{

			// disk data exist
			if ( m_Rca.RecPosIn != m_Rca.RecPosOut )
			{
				// check bad disk
				Tv_Rpos	=	m_Rca.RecPosIn;

				while ( E_GrDvrFs1DiskStatBad != m_Rca.DiskStat )
				{
					// calculate read size
					if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
					{
						Tv_ReadAble	=	Tv_Rpos;
						if ( Tv_Rpos > m_Rca.RecPosIn )
						{
							Tv_ReadAble	=	Tv_ReadAble - m_Rca.RecPosOut;
						}
					}
					else
					{
						Tv_ReadAble	=	Tv_Rpos - m_Rca.RecPosOut;
					}
					
					if ( E_GrDvrFs1DskPktWkBufSize < Tv_ReadAble )
					{
						Tv_ReadAble	=	E_GrDvrFs1DskPktWkBufSize;
					}

					if ( 0 != Tv_ReadAble )
					{
						Tv_Rpos	=	Tv_Rpos - Tv_ReadAble;
						if ( !ReadByRpos( Tv_Rpos, m_PtrTmpBuf, (__u32)Tv_ReadAble ) )
						{
							// error
							DbgMsgPrint( "Cls_GrDvrFs1Dsk::RecTimeBeginGet - read error!\n" );
							break;
						}
						// get first packet
						Tv_Result	=	LcGetBufFirstTime( m_PtrTmpBuf, (__u32)Tv_ReadAble );
						if ( 0 != Tv_Result )
						{
							break;
						}
					}
					// next
					if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
					{
						if ( 0 == Tv_Rpos )
						{
							Tv_Rpos	=	m_Mis.RdaLast;
						}
						else if ( m_Rca.RecPosOut == Tv_Rpos )
						{
							break;
						}
					}
					else
					{
						if ( m_Rca.RecPosOut == Tv_Rpos )
						{
							break;
						}
					}

				}
			}	//	if ( m_Rca.RecPosIn != m_Rca.RecPosOut )
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Dsk::TemperatureUpdt( void )
{
	// local -------------------
	// code --------------------
		if ( LcSmartRead() )
		{
			LcSmartFindRawByte( E_GrDvrFs1DskSmartIdTemp, &m_ValTemp );
		}
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::FindHndlByMinFw( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, void* A_PtrTmaBuf )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeMin;
		__u32	Tv_TmaIdx;
		BOOL8	Tv_IsEnd;
		Ptr_GrDvrRecMinThumb	Tv_TmaTbl;
		__u32	Tv_HeadGap;
		__u32	Tv_ReadCnt;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		//check formatted
		if ( m_IsFormatted && (E_GrDvrFs1DiskStatBad != m_Rca.DiskStat) )
		{
			//read current date
			Tv_TmaTbl		=	(Ptr_GrDvrRecMinThumb)A_PtrTmaBuf;
			Tv_IsEnd		=	FALSE;
			Tv_TimeMin	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin);
			while ( Tv_TimeMin < A_TimeEd )
			{
				Tv_ReadCnt	=	TmaDirRead( Tv_TimeMin, A_PtrTmaBuf, E_GrDvrFs1TmaTmpCnt, &Tv_HeadGap );
				//find current time	
				if ( 0 != Tv_ReadCnt )
				{
					for ( Tv_TmaIdx=Tv_HeadGap;Tv_TmaIdx < Tv_ReadCnt;Tv_TmaIdx++ )
					{
						//check find current
						if ( E_GrDvrRecHndlNone != Tv_TmaTbl[Tv_TmaIdx].HndlRec )
						{
							//finded
							Tv_Result	=	Tv_TmaTbl[Tv_TmaIdx].HndlRec;
							Tv_IsEnd	=	TRUE;
							break;
						}
						//next
						Tv_TimeMin	=	Tv_TimeMin + E_GrTimeSecOfMin;
						//check time over
						if ( Tv_TimeMin > A_TimeEd  )
						{
							Tv_IsEnd	=	TRUE;
							break;
						}
					}
				}
				if ( Tv_IsEnd )
				{
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::FindHndlByMinBw( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, void* A_PtrTmaBuf )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeMin;
		__u32	Tv_TmaIdx;
		BOOL8	Tv_IsEnd;
		Ptr_GrDvrRecMinThumb	Tv_TmaTbl;
		__u32	Tv_HeadGap;
		__u32	Tv_ReadCnt;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		//check formatted
		if ( m_IsFormatted && (E_GrDvrFs1DiskStatBad != m_Rca.DiskStat) )
		{
			//read current date
			Tv_TmaTbl		=	(Ptr_GrDvrRecMinThumb)A_PtrTmaBuf;
			Tv_IsEnd		=	FALSE;
			Tv_TimeMin	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin);
			while ( Tv_TimeMin >= A_TimeEd )
			{
				Tv_ReadCnt	=	TmaDirRead( Tv_TimeMin - E_GrTimeSecOfDay + E_GrTimeSecOfMin, A_PtrTmaBuf, E_GrDvrFs1TmaTmpCnt, &Tv_HeadGap );
				if ( 0 == Tv_ReadCnt )
				{
					break;
				}
				//find current time	
				Tv_TmaIdx	=	E_GrDvrFs1TmaTmpCnt;
				do 
				{
					//index
					Tv_TmaIdx --;
					//check find current
					if ( E_GrDvrRecHndlNone != Tv_TmaTbl[Tv_TmaIdx].HndlRec )
					{
						//finded
						Tv_Result	=	Tv_TmaTbl[Tv_TmaIdx].HndlRec;
						Tv_IsEnd	=	TRUE;
						break;
					}
					//next
					Tv_TimeMin	=	Tv_TimeMin - E_GrTimeSecOfMin;
					//check time over
					if ( Tv_TimeMin < A_TimeEd )
					{
						Tv_IsEnd	=	TRUE;
						break;
					}
				} while( Tv_HeadGap < Tv_TmaIdx );
				if ( Tv_IsEnd )
				{
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::DataSizeByMinuteRange( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, void* A_PtrTmaBuf )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeSt;
		Def_GrTime	Tv_TimeEd;
		__u64	Tv_PosSt;
		__u64	Tv_PosEd;
	// code --------------------
		Tv_Result	=	0;

		if ( m_IsFormatted && ( E_GrDvrFs1DiskStatBad != m_Rca.DiskStat ) )
		{
			//get local hdd time range
			Tv_TimeSt	=	RecTimeBeginGet();
			Tv_TimeEd	=	RecTimeEndGet();

			//check current time range
			if ( (A_TimeSt < Tv_TimeEd) && (A_TimeEd > Tv_TimeSt ) )
			{
				//get start handle
				if ( A_TimeSt <= Tv_TimeSt )
				{
					Tv_PosSt	=	RecOutPosGet();
				}
				else
				{
					Tv_PosSt	=	E_GrDvrFs1HndlPosMask & FindHndlByMinFw( A_TimeSt, A_TimeEd, A_PtrTmaBuf );
				}
				//get end handl
				if ( A_TimeEd >= Tv_TimeEd )
				{
					Tv_PosEd	=	RecInPosGet();
				}
				else
				{
					Tv_PosEd	=	E_GrDvrFs1HndlPosMask & FindHndlByMinBw( A_TimeEd, A_TimeSt, A_PtrTmaBuf );
				}
				//check calculate able
				if ( (0 != Tv_PosSt) && (0 != Tv_PosEd) )
				{
					//calculate
					if ( Tv_PosSt < Tv_PosEd )
					{
						Tv_Result	=	Tv_PosEd - Tv_PosSt;
					}
					else
					{
						Tv_Result	=	(m_Mis.RdaLast - Tv_PosEd) + (Tv_PosSt - m_Mis.RdaPos);
					}
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::IsVssSaved( void )
{
		return	m_Rca.IsVss;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcFillByte( __u64 A_PosTg, __u32 A_Size, __u8 A_Fill )
{
	// local -------------------
		BOOL8	Tv_Result;
		void*	Tv_PtrBuf;
		__u64	Tv_PosTg;
		__u32	Tv_WkSize;
		__u32	Tv_Remain;
	// code --------------------
		Tv_Result	=	FALSE;
		// allocate buffer
		Tv_PtrBuf	=	malloc( E_GrDvrFs1TmpBufSize );
		if ( NULL != Tv_PtrBuf )
		{
			// fill buffer
			GrDumyFillB( Tv_PtrBuf, E_GrDvrFs1TmpBufSize, A_Fill );
			// init
			Tv_Remain	=	A_Size;
			Tv_PosTg	=	A_PosTg;

			while ( 0 != Tv_Remain )
			{
				Tv_WkSize	=	Tv_Remain;
				if ( E_GrDvrFs1TmpBufSize < Tv_WkSize )
				{
					Tv_WkSize	=	E_GrDvrFs1TmpBufSize;
				}

				// write
				if ( !LcSeek( Tv_PosTg ) )
				{
					Tv_Result	=	FALSE;
					break;
				}
				if ( !LcWrite( Tv_PtrBuf, Tv_WkSize ) )
				{
					Tv_Result	=	FALSE;
					break;
				}

				// next
				Tv_PosTg	=	Tv_PosTg + (__u64)Tv_WkSize;
				Tv_Remain	=	Tv_Remain - Tv_WkSize;
			}

			free( Tv_PtrBuf );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcCopy( __u64 A_PosTg, __u64 A_PosSr, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		void*	Tv_PtrBuf;
		__u64	Tv_PosSr;
		__u64	Tv_PosTg;
		__u32	Tv_CpSize;
		__u32	Tv_Remain;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_PtrBuf	=	malloc( E_GrDvrFs1TmpBufSize );
		if ( NULL != Tv_PtrBuf )
		{
			Tv_Remain	=	A_Size;
			Tv_PosSr	=	A_PosSr;
			Tv_PosTg	=	A_PosTg;

			while ( 0 != Tv_Remain )
			{
				Tv_CpSize	=	Tv_Remain;
				if ( E_GrDvrFs1TmpBufSize < Tv_CpSize )
				{
					Tv_CpSize	=	E_GrDvrFs1TmpBufSize;
				}
				// read
				if ( !LcSeek( Tv_PosSr ) )
				{
					Tv_Result	=	FALSE;
					break;
				}
				if ( !LcRead( Tv_PtrBuf, Tv_CpSize ) )
				{
					Tv_Result	=	FALSE;
					break;
				}
				// write
				if ( !LcSeek( Tv_PosTg ) )
				{
					Tv_Result	=	FALSE;
					break;
				}
				if ( !LcWrite( Tv_PtrBuf, Tv_CpSize ) )
				{
					Tv_Result	=	FALSE;
					break;
				}
				// next
				Tv_PosSr	=	Tv_PosSr + (__u64)Tv_CpSize;
				Tv_PosTg	=	Tv_PosTg + (__u64)Tv_CpSize;
				Tv_Remain	=	Tv_Remain - Tv_CpSize;
			}

			free( Tv_PtrBuf );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcPread( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( LcSeek( A_Pos ) )
		{
			if ( LcRead( A_PtrBuf, A_Size ) )
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::LcPwrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( LcSeek( A_Pos ) )
		{
			if ( LcWrite( A_PtrBuf, A_Size ) )
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs1Dsk::ChkHndlFw( __u64 A_Hndl, __u32* A_PtrRtAbleSize )
{
	// local -------------------
		__u8	Tv_Result;
		BOOL8	Tv_IsOldPart;
		__u8	Tv_IntIdx;
		__u16	Tv_OvwtId;
		__u64	Tv_Rpos;
		__u64	Tv_AbleSize;
	// code --------------------
		Tv_Result				=	E_GrDvrFs1RposBad;
		Tv_AbleSize			=	0;

		// check HDD is bad
		if ( m_Rca.RecPosIn != m_Rca.RecPosOut )
		{
			GrDvrFs1HndlDec( A_Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );

			if ( Tv_IsOldPart )
			{
				Tv_Result				=	E_GrDvrFs1RposOld;
				if ( m_Rca.RecPosOut > m_Rca.RecPosIn )
				{
					if ( (Tv_OvwtId + 1) == m_Rca.RecSeqId )
					{
						if ( Tv_Rpos == m_Mis.RdaLast )
						{
							// end position
							Tv_Result	=	E_GrDvrFs1RposEnd;
						}
						else if ( Tv_Rpos >= m_Rca.RecPosOut )
						{
							Tv_Result		=	E_GrDvrFs1RposOk;
							Tv_AbleSize	=	m_Mis.RdaLast - Tv_Rpos;
						}
					}
				}
			}
			else
			{
				if ( Tv_OvwtId == m_Rca.RecSeqId )
				{
					if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
					{
						if ( Tv_Rpos == m_Rca.RecPosIn )
						{
							Tv_Result	=	E_GrDvrFs1RposEnd;
						}
						else if ( Tv_Rpos < m_Rca.RecPosIn )
						{
							Tv_Result		=	E_GrDvrFs1RposOk;
							Tv_AbleSize	=	m_Rca.RecPosIn - Tv_Rpos;
						}
					}
					else
					{
						if ( Tv_Rpos < m_Rca.RecPosOut )
						{
							Tv_Result				=	E_GrDvrFs1RposOld;
						}
						else if ( (Tv_Rpos >= m_Rca.RecPosOut) && (Tv_Rpos < m_Rca.RecPosIn) )
						{
							Tv_Result		=	E_GrDvrFs1RposOk;
							Tv_AbleSize	=	m_Rca.RecPosIn - Tv_Rpos;
						}
						else if ( Tv_Rpos == m_Rca.RecPosIn )
						{
							Tv_Result				=	E_GrDvrFs1RposEnd;
						}
					}
				}
				else if ( (Tv_OvwtId + 1) == m_Rca.RecSeqId )
				{
					Tv_Result				=	E_GrDvrFs1RposOld;
				}
			}
		}
		
		// patch size
		if ( E_GrDvrFs1RdaAlignUnit < Tv_AbleSize )
		{
			Tv_AbleSize	=	E_GrDvrFs1RdaAlignUnit;
		}
		
		*A_PtrRtAbleSize	=	(__u32)Tv_AbleSize;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs1Dsk::ChkHndlBw( __u64 A_Hndl, __u32* A_PtrRtAbleSize )
{
	// local -------------------
		__u8	Tv_Result;
		BOOL8	Tv_IsOldPart;
		__u8	Tv_IntIdx;
		__u16	Tv_OvwtId;
		__u64	Tv_Rpos;
		__u64	Tv_AbleSize;
	// code --------------------
		Tv_Result				=	E_GrDvrFs1RposBad;
		Tv_AbleSize			=	0;

		// check HDD is bad
		if ( m_Rca.RecPosIn != m_Rca.RecPosOut )
		{
			GrDvrFs1HndlDec( A_Hndl, &Tv_Rpos, &Tv_IntIdx, &Tv_IsOldPart, &Tv_OvwtId );

			if ( Tv_IsOldPart )
			{
				if ( m_Rca.RecPosOut > m_Rca.RecPosIn )
				{
					if ( (Tv_OvwtId + 1) == m_Rca.RecSeqId )
					{
						Tv_Result	=	E_GrDvrFs1RposEnd;
						if ( m_Rca.RecPosOut < Tv_Rpos )
						{
							Tv_Result		=	E_GrDvrFs1RposOk;
							Tv_AbleSize	=	Tv_Rpos - m_Rca.RecPosOut;
						}
					}
				}	// if ( m_Rca.RecPosOut > m_Rca.RecPosIn )

			}	//	if ( Tv_IsOldPart )
			else
			{
				if ( m_Rca.RecPosOut > m_Rca.RecPosIn )
				{
					if ( (Tv_OvwtId + 1) == m_Rca.RecSeqId )
					{
						Tv_Result	=	E_GrDvrFs1RposEnd;
						if ( m_Rca.RecPosOut < Tv_Rpos )
						{
							Tv_Result		=	E_GrDvrFs1RposOk;
							Tv_AbleSize	=	Tv_Rpos - m_Rca.RecPosOut;
						}
					}
					else if ( m_Rca.RecSeqId == Tv_OvwtId )
					{
						if ( m_Mis.RdaPos >= Tv_Rpos )
						{
							Tv_Result	=	E_GrDvrFs1RposEnd;
						}
						else if ( Tv_Rpos <= m_Rca.RecPosIn )
						{
							Tv_Result		=	E_GrDvrFs1RposOk;
							Tv_AbleSize	=	Tv_Rpos - m_Mis.RdaPos;
						}
					}
				}
				else
				{
					if ( m_Rca.RecSeqId == Tv_OvwtId )
					{
						if ( (Tv_Rpos > m_Rca.RecPosOut) && (Tv_Rpos <= m_Rca.RecPosIn ) )
						{
							Tv_Result		=	E_GrDvrFs1RposOk;
							Tv_AbleSize	=	Tv_Rpos - m_Rca.RecPosOut;
						}
						else
						{
							Tv_Result	=	E_GrDvrFs1RposEnd;
						}
					}
					else
					{
						Tv_Result	=	E_GrDvrFs1RposEnd;
					}
				}
			}	//	if ( Tv_IsOldPart )
		}	//	if ( m_Rca.RecPosIn != m_Rca.RecPosOut )

		// patch size
		if ( E_GrDvrFs1RdaAlignUnit < Tv_AbleSize )
		{
			Tv_AbleSize	=	E_GrDvrFs1RdaAlignUnit;
		}

		*A_PtrRtAbleSize	=	(__u32)Tv_AbleSize;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::ReadByRpos( __u64 A_RdaPos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
	// code --------------------
		return	LcPread( A_RdaPos, A_PtrBuf, A_Size );
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::RdaBeginHndlGet( BOOL8 A_IsOldPart )
{
	// local -------------------
		__u64	Tv_Rpos;
		BOOL8	Tv_IsOldPart;
		__u16	Tv_OvwtId;
	// code --------------------
		Tv_Rpos				=	m_Rca.RecPosOut;
		Tv_OvwtId			=	m_Rca.RecSeqId;
		Tv_IsOldPart	=	FALSE;
		if (m_Rca.RecPosIn < m_Rca.RecPosOut)
		{
			if ( A_IsOldPart )
			{
				Tv_OvwtId	--;
				Tv_IsOldPart	=	TRUE;
			}
			else
			{
				Tv_Rpos	=	m_Mis.RdaPos;
			}
		}

		return	GrDvrFs1HndlEnc( Tv_Rpos, m_GrpSubIdx, Tv_IsOldPart, Tv_OvwtId );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::IsOldPartExist( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Dsk::RdaEndHndlGet( BOOL8 A_IsOldPart )
{
	// local -------------------
		__u64	Tv_Rpos;
		BOOL8	Tv_IsOldPart;
		__u16	Tv_OvwtId;
	// code --------------------
		Tv_Rpos				=	m_Rca.RecPosIn;
		Tv_OvwtId			=	m_Rca.RecSeqId;
		Tv_IsOldPart	=	FALSE;
		if (m_Rca.RecPosIn < m_Rca.RecPosOut)
		{
			if ( A_IsOldPart )
			{
				Tv_OvwtId	--;
				Tv_IsOldPart	=	TRUE;
				Tv_Rpos		=	m_Mis.RdaLast;
			}
		}

		return	GrDvrFs1HndlEnc( Tv_Rpos, m_GrpSubIdx, Tv_IsOldPart, Tv_OvwtId );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Dsk::IsReadAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrFs1DiskStatBad != m_Rca.DiskStat )
		{
			if ( m_Rca.RecPosIn != m_Rca.RecPosOut )
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
