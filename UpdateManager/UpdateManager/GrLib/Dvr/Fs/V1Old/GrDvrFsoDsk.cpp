/*
 file system version 1 disk

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrBufTool.h>
#include <Dvr/Fs/V1Old/GrDvrFsoDsk.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <GrDebug.h>
#include <Dvr/Fs/V1Old/GrDvrFsoMng.h>
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

#define E_GrDvrFsoDskSmartIdTemp		194

#define E_GrDvrFsoDskPktWkBufSize		0x80000

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFsoDsk::Cls_GrDvrFsoDsk( __u8 A_DskIdx, Ptr_GrDvrFsoDskCfg A_PtrCfg, BOOL8 A_IsOverwrite, void* A_PtrLogReadBuf )
{
		m_DskIdx			=	A_DskIdx;
		m_GrpSubIdx		=	0;
		m_DskSize			=	0;
		m_IsFormatted	=	FALSE;
		m_IsOverWrite	=	A_IsOverwrite;
		m_PtrCfg			=	A_PtrCfg;
		m_IsModified	=	FALSE;
		m_PtrLogRdBuf	=	A_PtrLogReadBuf;
		GrDumyZeroMem( &m_CchWtTma, sizeof(m_CchWtTma) );
		m_RecTimeBegin	=	0;
		m_RecTimeEnd		=	0;
		m_ValTemp				=	0;

		GrDumyZeroMem( m_Dmd, sizeof(m_Dmd) );

		m_TmaTmBase			=	0xFFFFFFFF;

		if ( 512 != sizeof(St_GrDvrFsoSmartData) )
		{
			DbgMsgPrint( "GrDvrFsoDsk: Smart struct size ( %d ) mismatch \n", sizeof(St_GrDvrFsoSmartData) );
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
Cls_GrDvrFsoDsk::~Cls_GrDvrFsoDsk()
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
__u8	Cls_GrDvrFsoDsk::DiskIdxGet( void )
{
		return	m_DskIdx;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::DiskSizeGet( void )
{
		return	m_DskSize;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::LcSeek( __s64 A_Pos, BOOL8 A_IsBegin )
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
					m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
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
BOOL8	Cls_GrDvrFsoDsk::LcRead( void* A_PtrBuf, __u32 A_Size )
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
					m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
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
BOOL8	Cls_GrDvrFsoDsk::LcWrite( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
#ifndef LINUX_APP
		__u32		Tv_Writed;
#endif
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( E_GrDvrFsoDiskStatBad != m_Rca.DiskStat )
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
						m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
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
void	Cls_GrDvrFsoDsk::LcFlush( void )
{
#ifdef LINUX_APP
		if ( -1 != m_HndlDsk )
		{
			fsync( m_HndlDsk );
		}
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::IsFormatted( void )
{
		return	m_IsFormatted;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::LcInit( void )
{
	// local -------------------
		__u64	Tv_Pos;
		//__u32	Tv_Size;
		BOOL8	Tv_IsAble;
	// code --------------------
		m_IsFormatted	=	FALSE;
		Tv_IsAble			=	TRUE;

		// load MIS
		if ( LcPread( E_GrDvrFsoDposMis, &m_Mis, sizeof(m_Mis) ) )
		{
			Tv_IsAble	=	TRUE;
		}
		
		// MCA
		if ( Tv_IsAble )
		{
			Tv_IsAble	=	LcPread( E_GrDvrFsoDposMca, &m_Mca, sizeof(m_Mca) );
		}

		// RCA
		if ( Tv_IsAble )
		{
			Tv_Pos		=	(__s64)(E_GrDvrFsoDposRca + (E_GrDvrFsoRcaItmSize * (__u32)m_Mca.RcaIdx) );
			Tv_IsAble	=	LcPread( Tv_Pos, &m_Rca, sizeof(m_Rca) );
			if ( Tv_IsAble )
			{
				// check need upgrade
				if ( 0 != m_Mca.RcaIdx )
				{
					DbgMsgPrint( "Cls_GrDvrFsoDsk::LcInit - RCA address fix.\n" );
					// write RCA
					Tv_IsAble	=	LcPwrite( E_GrDvrFsoDposRca, &m_Rca, sizeof(m_Rca) );
					if ( Tv_IsAble )
					{
						// FIX RCA address
						m_Mca.RcaIdx	=	0;
						Tv_IsAble	=	LcPwrite( E_GrDvrFsoDposMca, &m_Mca, sizeof(m_Mca) );
					}
				}
			}
		}

		// check need upgrade
		if ( Tv_IsAble )
		{
			Tv_IsAble	=	FALSE;
			//check match mis
			if ( (E_GrDvrFsoFcc == m_Mis.Fcc) && (m_PtrCfg->SysId == m_Mis.SysId) )
			{
				if ( E_GrDvrFsoVerNew > m_Mis.Ver )
				{
					// upgrade
					DbgMsgPrint( "Cls_GrDvrFsoDsk::LcInit- file system upgrade.\n" );
					// check version
					Tv_IsAble	=	LcUpgdOldFs( m_Mis.Ver );

					if ( Tv_IsAble )
					{
						// update MIS
						m_Mis.Ver			=	E_GrDvrFsoVerNew;
						m_Mis.UwaPos	=	E_GrDvrFsoDposUwa;
						m_Mis.TmaCnt	=	E_GrDvrFsoTmaItemCnt;
						Tv_IsAble	=	LcPwrite( E_GrDvrFsoDposMis, &m_Mis, sizeof(m_Mis) );
					}
				}
				else
				{
					// load DMD
					Tv_IsAble	=	LcPread( E_GrDvrFsoDposDmd, m_Dmd, sizeof(m_Dmd) );
				}
			}
		}

		// init other value
		if ( Tv_IsAble )
		{
			//init other value
			//m_TimeLimite	=	m_Mis.TimeBase + (60 * m_Mis.TmaCnt);
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
BOOL8	Cls_GrDvrFsoDsk::Format( BOOL8 A_IsWithoutLog )
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
		if ( E_GrDvrFsoSizeLimite <= m_DskSize )
		{
			// init
			Tv_IsAble	=	TRUE;
			//write mis
			m_Mis.Fcc			=	E_GrDvrFsoFcc;
			m_Mis.Ver			=	E_GrDvrFsoVerNew;
			m_Mis.SysId		=	m_PtrCfg->SysId;
			m_Mis.TmaCnt	=	E_GrDvrFsoTmaItemCnt;
			m_Mis.TimeBase	=	GrTimeEnc( 2008, 1, 1, 0, 0, 0 );
			m_Mis.UwaPos	=	E_GrDvrFsoDposUwa;
			m_Mis.UwaSize	=	m_PtrCfg->UwaSize;
			m_Mis.IdaPos	=	(m_Mis.UwaPos + m_Mis.UwaSize + (~E_GrDvrFsoRdaAlignMask)) & E_GrDvrFsoRdaAlignMask;
			m_Mis.IdaSize	=	m_PtrCfg->IdaSize & E_GrDvrFsoRdaAlignMask;
			m_Mis.RdaPos	=	(m_Mis.IdaPos + m_Mis.IdaSize + (~E_GrDvrFsoRdaAlignMask)) & E_GrDvrFsoRdaAlignMask;
			m_Mis.RdaLast	=	m_DskSize & E_GrDvrFsoRdaAlignMask;
			Tv_IsAble	=	LcPwrite( E_GrDvrFsoDposMis, &m_Mis, sizeof(m_Mis) );

			// MCA
			if ( Tv_IsAble )
			{
				GrDumyZeroMem( &m_Mca, sizeof(m_Mca) );
				Tv_IsAble	=	LcPwrite( E_GrDvrFsoDposMca, &m_Mca, sizeof(m_Mca) );
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
				Tv_IsAble	=	LcPwrite( E_GrDvrFsoDposRca, &m_Rca, sizeof(m_Rca) );
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
					GrDumyZeroMem( m_PtrLogRdBuf, E_GrDvrFsoElaSize );
				}
				Tv_IsAble	=	LcPwrite( E_GrDvrFsoDposEla, m_PtrLogRdBuf, E_GrDvrFsoElaSize );
			}

			// DMD
			if ( Tv_IsAble )
			{
				Tv_IsAble	=	LcFillByte( E_GrDvrFsoDposDmd, E_GrDvrFsoDmdAreaSize, 0 );
			}

			// MMD
			if ( Tv_IsAble )
			{
				Tv_IsAble	=	LcFillByte( E_GrDvrFsoDposMmd, E_GrDvrFsoMmdAreaSize, 0 );
			}

			// UWA
			if ( Tv_IsAble )
			{
				LcFillByte( E_GrDvrFsoDposUwa, 0x100, 0 );
			}
			
			// finish
			if ( Tv_IsAble )
			{
				//init runtime data
				//m_TimeLimite	=	m_Mis.TimeBase + (60 * m_Mis.TmaCnt);
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
BOOL8	Cls_GrDvrFsoDsk::LcIsBadBlock( __u64 A_Pos )
{
	/*
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		__u64	Tv_NowBlkPos;
	// code --------------------
		Tv_Result			=	FALSE;
		Tv_NowBlkPos	=	A_Pos & E_GrDvrFsoRdaAlignMask;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoBadBlkCnt;Tv_WkIdx++ )
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
		*/

		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::LcAlignPadAdd( void )
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
		Tv_BlkNext	=	(m_Rca.RecPosIn + E_GrDvrFsoRdaAlignUnit) & E_GrDvrFsoRdaAlignMask;
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
BOOL8	Cls_GrDvrFsoDsk::LcBadBlockAdd( __u64 A_Pos )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		__u64	Tv_PosBlk;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PosBlk	=	E_GrDvrFsoRdaAlignMask & A_Pos;
		//check already exist
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoBadBlkCnt;Tv_WkIdx++ )
		{
			if ( Tv_PosBlk == m_Mca.BadBlkTbl[Tv_WkIdx] )
			{
				//already exist
				Tv_Result	=	TRUE;
				DbgMsgPrint("Cls_GrDvrFsoDsk::LcBadBlockAdd - badblock already added - %08X%08X\n", (__u32)(A_Pos >> 32), (__u32)(A_Pos & 0x00000000FFFFFFFFLL) );
				break;
			}
		}
		//check alreayd exist
		if ( !Tv_Result )
		{
			//find empty bad block
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoBadBlkCnt;Tv_WkIdx++ )
			{
				if ( 0 == m_Mca.BadBlkTbl[Tv_WkIdx] )
				{
					//finded
					m_Mca.BadBlkTbl[Tv_WkIdx]	=	Tv_PosBlk;
					LcMcaUpdt();		//update mca
					DbgMsgPrint("Cls_GrDvrFsoDsk::LcBadBlockAdd - badblock add - %08X%08X\n", (__u32)(A_Pos >> 32), (__u32)(A_Pos & 0x00000000FFFFFFFFLL) );
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
				m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
				DbgMsgPrint("Cls_GrDvrFsoDsk::LcBadBlockAdd - badblock full - %08X%08X\n", (__u32)(A_Pos >> 32), (__u32)(A_Pos & 0x00000000FFFFFFFFLL) );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoDsk::DiskStatGet( void )
{
		return	m_Rca.DiskStat;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoDsk::LcMakeSpace( void )
{
	// local -------------------
	// code --------------------
		if ( m_IsOverWrite )
		{
			//check disk full
			if ( E_GrDvrFsoDiskStatFull != m_Rca.DiskStat )
			{
				//free jam status
				if ( E_GrDvrFsoDiskStatJam == m_Rca.DiskStat )
				{
					m_Rca.DiskStat	=	E_GrDvrFsoDiskStatNormal;
				}
				else
				{
					//check need make space
					if ( m_Rca.RecPosIn < m_Rca.RecPosOut )
					{
						//delete out block
						m_Rca.RecPosOut	=	(m_Rca.RecPosOut + E_GrDvrFsoRdaAlignUnit);	// & E_GrDvrFsoRdaAlignMask;
						//check finish
						if ( m_Mis.RdaLast <= m_Rca.RecPosOut )
						{
							//mark full
							m_Rca.DiskStat	=	E_GrDvrFsoDiskStatFull;
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
			if ( (E_GrDvrFsoDiskStatFull != m_Rca.DiskStat) && (E_GrDvrFsoDiskStatJam != m_Rca.DiskStat) )
			{
				if ( m_Rca.RecPosIn == m_Mis.RdaLast )
				{
					m_Rca.DiskStat	=	E_GrDvrFsoDiskStatFull;
				}
				else
				{
					m_Rca.DiskStat	=	E_GrDvrFsoDiskStatJam;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoDsk::LcMcaUpdt( void )
{
	// local -------------------
		BOOL8	Tv_IsSuccess;
	// code --------------------
		Tv_IsSuccess	=	FALSE;
		if ( LcSeek( E_GrDvrFsoDposMca ) )
		{
			if ( LcWrite( &m_Mca, sizeof(m_Mca)) )
			{
				Tv_IsSuccess	=	TRUE;
			}
		}
		//check bad
		if ( !Tv_IsSuccess )
		{
			m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
			DbgMsgPrint("Cls_GrDvrFsoDsk::LcMcaUpdt - MCA area error!\n");
			GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoDsk::LcRcaWrite( void )
{
	// local -------------------
	// code --------------------
		if ( !LcPwrite( E_GrDvrFsoDposRca, &m_Rca, sizeof(m_Rca) ) )
		{
			m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
			DbgMsgPrint("Cls_GrDvrFsoDsk::LcRcaUpdt - RCA area error!\n");
			GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
		}
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::PacketReserve( __u32 A_Size )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrAvHd	Tv_Hd;
		__u32	Tv_Tail;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;

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
							Tv_Result	=	m_Rca.RecPosIn | (__u64)m_GrpSubIdx | ( (__u64)m_Rca.RecSeqId << E_GrDvrFsoHndlSeqPos );
							//go next position
							m_Rca.RecPosIn	=	m_Rca.RecPosIn + (__u64)A_Size;		//go next position
							m_IsModified	=	TRUE;
						}
					}
				}
			}	//if ( LcSeek( m_Rca.RecPosIn ) )
			if ( E_GrDvrFsoHndlNone == Tv_Result )
			{
				LcBadBlockAdd( m_Rca.RecPosIn );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::DirectWrite( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_DskPos;
		__u16	Tv_HndlSeqId;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_HndlSeqId	=	(__u16)( A_Hndl >> E_GrDvrFsoHndlSeqPos );
		//check bad disk
		if ( E_GrDvrFsoDiskStatBad != m_Rca.DiskStat )
		{
			//get position
			Tv_DskPos			=	A_Hndl & E_GrDvrFsoHndlPosMask;

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
					if ( ((Tv_HndlSeqId + 1) & E_GrDvrFsoHndlSeqLimite) == m_Rca.RecSeqId )
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
BOOL8	Cls_GrDvrFsoDsk::LcPreparePktAdd( __u32 A_Size )
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
				if ( E_GrDvrFsoDiskStatBad == m_Rca.DiskStat )
				{
					break;
				}
				//check disk stauts normal
				if ( E_GrDvrFsoDiskStatNormal == m_Rca.DiskStat )
				{
					//input position is finished
					if ( m_Mis.RdaLast > m_Rca.RecPosIn )
					{
						//check now bad block
						Tv_BlkNext	=	(m_Rca.RecPosIn + E_GrDvrFsoRdaAlignUnit) & E_GrDvrFsoRdaAlignMask;
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
									m_Rca.DiskStat	=	E_GrDvrFsoDiskStatJam;
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
						m_Rca.DiskStat	=	E_GrDvrFsoDiskStatFull;
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
BOOL8	Cls_GrDvrFsoDsk::PacketAdd( Ptr_GrDvrAvHd A_PtrPkt, __u64* A_PtrRtHndl )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_PktSize;
	// code --------------------
		Tv_Result	=	FALSE;

		//check time range
		if (E_GrTimeSecOfDay > A_PtrPkt->TimeSec) 
		{
			//DbgMsgPrint("Cls_GrDvrFsoDsk::PacketAdd -  time limite less\n" );
			A_PtrPkt->TimeSec	=	A_PtrPkt->TimeSec + E_GrTimeSecOfDay;	// patch time
		}
		//check packet size
		Tv_PktSize	=	E_GrDvrFsoStrmAlignMask & (sizeof(St_GrDvrAvHd) + A_PtrPkt->DataSize + E_GrDvrRecTailMarkSize + E_GrDvrFsoStrmAlignSize - 1);
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
					if (0 == A_PtrPkt->Evt)
					{
						A_PtrPkt->Evt	=	A_PtrPkt->Evt | E_GrDvrRecAtbRecord;
					}
					//update TMA
					LcTmaUpdt( A_PtrPkt->TimeSec, A_PtrPkt->Ch, A_PtrPkt->Evt );
					//result handle
					if ( NULL != A_PtrRtHndl )
					{
						*A_PtrRtHndl	=	m_Rca.RecPosIn | (__u64)m_GrpSubIdx | ( (__u64)m_Rca.RecSeqId << E_GrDvrFsoHndlSeqPos );
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
void	Cls_GrDvrFsoDsk::LcTmaUpdt( Def_GrTime A_Time, __u8 A_Ch, __u8 A_Atb )
{
	// local -------------------
		BOOL8		Tv_IsTmChg;
		BOOL8		Tv_isDtChg;
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
			if ( !LcThumdRead( &m_CchWtTma, m_TmaTmBase ) )
			{
				GrDumyZeroMem( &m_CchWtTma, sizeof(m_CchWtTma) );
			}
		}
		
		// check event
		Tv_isDtChg	=	FALSE;
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
			if ( E_GrDvrFsoHndlNone == m_CchWtTma.HndlRec )
			{
				//update value
				m_CchWtTma.HndlRec	=	m_Rca.RecPosIn | ((__u64)m_Rca.RecSeqId << E_GrDvrFsoHndlSeqPos);
			}
		}
		
		// update TMA
		if ( Tv_isDtChg )
		{
			LcThumbWrite( &m_CchWtTma, m_TmaTmBase );
		}

		// update RCA
		if ( Tv_IsTmChg )
		{
			LcRcaWrite();
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoDsk::UnlockFull( void )
{
	// local -------------------
		__u8	Tv_PrvStat;
	// code --------------------
		//check disk status normal
		if ( E_GrDvrFsoDiskStatBad != m_Rca.DiskStat )
		{
			//check foramt
			if ( m_IsFormatted )
			{
				//check is full
				Tv_PrvStat	=	m_Rca.DiskStat;
				if ( E_GrDvrFsoDiskStatFull == Tv_PrvStat )
				{
					m_Rca.RecPosOut	=	m_Mis.RdaPos + E_GrDvrFsoRdaAlignUnit;
					m_Rca.RecPosIn	=	m_Mis.RdaPos;
					m_Rca.RecSeqId ++;
				}
				else
				{
					LcMakeSpace();
				}
				m_Rca.DiskStat	=	E_GrDvrFsoDiskStatNormal;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::LcIsCrntHndl( __u64 A_Hndl )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_SeqId;
		__u64	Tv_DataPos;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_DataPos	=	A_Hndl & E_GrDvrFsoHndlPosMask;
		Tv_SeqId		=	(__u16)(A_Hndl >> E_GrDvrFsoHndlSeqPos);
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
__u32	Cls_GrDvrFsoDsk::TmaDirRead( Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt, __u32* A_PtrRtHeadGap )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsClear;
		Def_GrTime	Tv_TmDay;
		__s32	Tv_MmdSt;
		__s32	Tv_MmdCnt;
		__s32	Tv_MmdRemain;
		__s32	Tv_DmdIdx;
		void*	Tv_PtrTg;
		Ptr_GrDvrRecMinThumb	Tv_PtrMmd;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	0;
		*A_PtrRtHeadGap	=	0;

		//check disk stat
		if ( (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat) && m_IsFormatted )
		{
			// init
			Tv_PtrMmd	=	(Ptr_GrDvrRecMinThumb)m_PtrCfg->TmpBuf;
			// build count
			Tv_MmdRemain	=	A_Cnt;
			Tv_TmDay			=	A_Time - (A_Time % E_GrTimeSecOfDay);
			Tv_MmdSt			=	(A_Time - Tv_TmDay) / E_GrTimeSecOfMin;
			Tv_MmdCnt			=	E_GrTimeMinOfDay - Tv_MmdSt;
			Tv_PtrTg			=	A_PtrBuf;

			while ( 0 != Tv_MmdRemain )
			{
				// init
				Tv_IsClear	=	TRUE;
				// find DMD
				Tv_DmdIdx	=	LcDmdFindTime( Tv_TmDay );
				if ( 0 <= Tv_DmdIdx )
				{
					// load MMD
					Tv_Dpos	=	E_GrDvrFsoDposMmd + (__u64)( Tv_DmdIdx * E_GrDvrFsoMmdDaySize );
					if ( !LcPread( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
					{
						m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
						break;
					}

					// copy
					GrDumyCopyMem( Tv_PtrTg, &Tv_PtrMmd[Tv_MmdSt], Tv_MmdCnt * E_GrDvrRecMinThumbItemSize );
					Tv_IsClear	=	FALSE;
				}

				// clear
				if ( Tv_IsClear )
				{
					GrDumyZeroMem( Tv_PtrTg, Tv_MmdCnt * E_GrDvrRecMinThumbItemSize );
				}

				// next
				Tv_Result			=	Tv_Result + Tv_MmdCnt;
				Tv_MmdRemain	=	Tv_MmdRemain - Tv_MmdCnt;
				Tv_PtrTg			=	(void*)( (__u32)Tv_PtrTg + ((__u32)Tv_MmdCnt * E_GrDvrRecMinThumbItemSize) );
				Tv_MmdSt			=	0;
				Tv_TmDay			=	Tv_TmDay + E_GrTimeSecOfDay;
				Tv_MmdCnt			=	Tv_MmdRemain;
				if ( E_GrTimeMinOfDay < Tv_MmdCnt )
				{
					Tv_MmdCnt			=	E_GrTimeMinOfDay;
				}

			}	//	while ( 0 != Tv_MmdRemain )
		}	//	if ( (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat) && m_IsFormatted )

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::TmaReadCmb( Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt, void* A_PtrTmpBuf )
{
	// local -------------------
		__u32	Tv_ReadCnt;

		BOOL8	Tv_Result;
		__u32	Tv_HeadGap;
		Ptr_GrDvrRecMinThumb	Tv_PtrTg;
		Ptr_GrDvrRecMinThumb	Tv_PtrSr;
		__u32	Tv_WkIdx;
		//__u64	Tv_PosTma;
		//__u64	Tv_PosTmp;

	// code --------------------
		Tv_Result	=	FALSE;

		Tv_ReadCnt	=	TmaDirRead( A_Time, A_PtrTmpBuf, A_Cnt, &Tv_HeadGap );
		if ( 0 != Tv_ReadCnt )
		{
			//combine data
			Tv_PtrTg	=	(Ptr_GrDvrRecMinThumb)( (__u32)A_PtrBuf + (Tv_HeadGap << E_GrDvrRecMinThumbSizeShft) );
			Tv_PtrSr	=	(Ptr_GrDvrRecMinThumb)A_PtrTmpBuf;
			//merge start
			for ( Tv_WkIdx=Tv_ReadCnt;0 != Tv_WkIdx;Tv_WkIdx-- )
			{
				//check accept data
				if ( E_GrDvrFsoHndlNone != Tv_PtrSr->HndlRec )
				{
					//check current tma
					if ( LcIsCrntHndl( Tv_PtrSr->HndlRec ) )
					{
						//check target exist
						if ( E_GrDvrFsoHndlNone == Tv_PtrTg->HndlRec )
						{
							//patch disk id
							*Tv_PtrTg	=	*Tv_PtrSr;
							Tv_PtrTg->HndlRec	=	Tv_PtrTg->HndlRec | m_GrpSubIdx;		//update group index
						}
					}
					else
					{
						//remove and save
						Tv_PtrSr->HndlRec	=	E_GrDvrFsoHndlNone;
						/* patch write
						Tv_PosTmp	=	Tv_PosTma + (__u64)( (__u32)Tv_PtrSr - (__u32)A_PtrTmpBuf );
						LcTmaWrite( Tv_PtrSr, Tv_PosTmp );
						*/
					}
				}

				//next
				Tv_PtrSr ++;
				Tv_PtrTg ++;
			}
			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFsoDsk::LcGetBufFirstTime( void* A_PtrBuf, __u32 A_BufSize )
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
			Tv_PktSize	=	E_GrDvrFsoStrmAlignMask & (sizeof(St_GrDvrAvHd) + Tv_PtrHd->DataSize + E_GrDvrRecTailMarkSize + E_GrDvrFsoStrmAlignSize - 1);
			if ( A_BufSize < (Tv_PrcSize + Tv_PktSize) )
			{
				break;
			}
			//check exist time
			if ( E_GrDvrAvTypAdo >= Tv_PtrHd->Type )
			{
				//found
				Tv_Result	=	Tv_PtrHd->TimeSec;
				break;
			}
			//next
			Tv_PrcSize	=	Tv_PrcSize + Tv_PktSize;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFsoDsk::LcCalcBufFrmsSizeNormal( void* A_PtrBuf, __u32 A_BufSize )
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
			Tv_PktSize	=	E_GrDvrFsoStrmAlignSize;
			if ( m_PtrCfg->FccStrm == Tv_PtrHd->Fcc )
			{
				Tv_PktSize	=	E_GrDvrFsoStrmAlignMask & (sizeof(St_GrDvrAvHd) + Tv_PtrHd->DataSize + E_GrDvrRecTailMarkSize + E_GrDvrFsoStrmAlignSize - 1);
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
__u8	Cls_GrDvrFsoDsk::ReadNormal( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, 
																	__u64* A_PtrRtCrntHndl )
{
	// local -------------------
		__u8	Tv_Result;
		__u16	Tv_HndlSeqId;
		__u64	Tv_DskPos;
		__u64	Tv_NextPos;
		__u32	Tv_ReadSize;
		__u32	Tv_AlignFree;
		BOOL8	Tv_IsOk;
		__u32	Tv_FrmAreaSize;
	// code --------------------
		*A_PtrRtSize			=	0;
		*A_PtrRtCrntHndl	=	A_Hndl;

		//get basic info
		Tv_HndlSeqId	=	(__u16)( A_Hndl >> E_GrDvrFsoHndlSeqPos );
		Tv_DskPos			=	A_Hndl & E_GrDvrFsoHndlPosMask;

		//check data empty
		Tv_Result	=	E_GrDvrRecErrDataEmpty;
		if ( m_Rca.RecPosIn != m_Rca.RecPosOut )
		{
			//check disk status
			Tv_Result	=	E_GrDvrRecErrDskBad;
			while ( E_GrDvrFsoDiskStatBad != m_Rca.DiskStat )
			{
				//check handle and change first position
				Tv_IsOk		=	FALSE;
				if ( Tv_HndlSeqId == m_Rca.RecSeqId )
				{
					//check end position
					if ( Tv_DskPos >= m_Mis.RdaLast )
					{
						//disk end
						Tv_Result	=	E_GrDvrRecErrDskEnd;
						break;
					}
					//match record sequence
					if ( Tv_DskPos < m_Rca.RecPosIn )
					{
						Tv_IsOk	=	TRUE;
						if ((m_Rca.RecPosOut < m_Rca.RecPosIn) && (Tv_DskPos < m_Rca.RecPosOut) )
						{
							Tv_DskPos	=	m_Rca.RecPosOut;
						}
					}
					else
					{
						//disk end
						Tv_Result	=	E_GrDvrRecErrDskEnd;
						break;
					}
				}
				else if ( (Tv_HndlSeqId + 1) == m_Rca.RecSeqId )
				{
					//one time old sequence
					Tv_IsOk	=	TRUE;

					if ( Tv_DskPos >= m_Mis.RdaLast )
					{
						Tv_HndlSeqId ++;		//patch handle
						Tv_DskPos	=	m_Mis.RdaPos;
					}
					else
					{
						//check out
						if (m_Rca.RecPosOut < m_Rca.RecPosIn)
						{
							Tv_HndlSeqId ++;		//patch handle
							Tv_DskPos	=	m_Rca.RecPosOut;
						}
						else
						{
							if ( Tv_DskPos < m_Rca.RecPosOut )
							{
								Tv_DskPos	=	m_Rca.RecPosOut;
							}
						}
					}

				}	//check record sequence

				//check current record sequence
				if ( Tv_IsOk )
				{
					//check bad block
					if ( !LcIsBadBlock( Tv_DskPos ) )
					{
						//check size
						Tv_ReadSize		=	A_Size;
						Tv_AlignFree	=	(__u32)( E_GrDvrFsoRdaAlignUnit - (Tv_DskPos & (~E_GrDvrFsoRdaAlignMask)) );
						if ( Tv_ReadSize >= Tv_AlignFree )
						{
							Tv_ReadSize	=	Tv_AlignFree;
						}
						//check read area out
						Tv_NextPos	=	Tv_DskPos + (__u64)Tv_ReadSize;
						if ( m_Rca.RecPosOut < m_Rca.RecPosIn )
						{
							if ( Tv_NextPos > m_Rca.RecPosIn )
							{
								Tv_ReadSize	=	(__u32)(m_Rca.RecPosIn - Tv_DskPos);
							}
						}
						else
						{
							// inpos < outout mode
							if ( (Tv_DskPos < m_Rca.RecPosIn) && ( Tv_NextPos > m_Rca.RecPosIn ) )
							{
								Tv_ReadSize	=	(__u32)(m_Rca.RecPosIn - Tv_DskPos);
							}
						}
						//move position
						if ( LcSeek( Tv_DskPos ) )
						{
							if ( LcRead( A_PtrBuf, Tv_ReadSize ) )
							{
								//success
								//get frame area size
								//Tv_FrmAreaSize		=	LcCalcBufFrmsSizeNormal( A_PtrBuf, Tv_ReadSize );
								Tv_FrmAreaSize		=	Tv_ReadSize;
								*A_PtrRtSize			=	Tv_FrmAreaSize;
								*A_PtrRtCrntHndl	=	Tv_DskPos  | ( ((__u64)Tv_HndlSeqId) << E_GrDvrFsoHndlSeqPos ) | (__u64)m_GrpSubIdx;
								Tv_Result					=	E_GrDvrRecErrNone;
								break;
							}
						}
						//add bad block
						LcBadBlockAdd( Tv_DskPos );
					}
					else
					{
						//bad block, go next position
						Tv_NextPos	=	(Tv_DskPos + E_GrDvrFsoRdaAlignUnit) & E_GrDvrFsoRdaAlignMask;
						//check finished
						if ( Tv_NextPos >= m_Mis.RdaLast )
						{
							//finish
							Tv_Result	=	E_GrDvrRecErrDskEnd;
							break;
						}
						else
						{
							//check record finish position
							if ( (Tv_DskPos <= m_Rca.RecPosIn) && (Tv_NextPos >= m_Rca.RecPosIn) )
							{
								//finish
								Tv_Result	=	E_GrDvrRecErrDskEnd;
								break;
							}
							else
							{
								//next disk position
								Tv_DskPos = Tv_NextPos;
							}
						}
					}	//if ( !LcIsBadBlock( Tv_DskPos ) )

				}
				else
				{
					//time out
					Tv_Result	=	E_GrDvrRecErrHndlTimeOut;
					break;
				}	//if ( Tv_IsOk )

			}	//while ( E_GrDvrFsoDiskStatBad != m_Rca.DiskStat )

			//update result
			if ( E_GrDvrFsoDiskStatBad == m_Rca.DiskStat )
			{
				Tv_Result	=	E_GrDvrRecErrDskBad;
			}

		}	//check disk empty

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFsoDsk::LcGetBufLastTime( void* A_PtrBuf, __u32 A_BufSize )
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
				DbgMsgPrint( "Cls_GrDvrFsoDsk::LcCalcBufFrmsSizeBack: bad tail code.\n" );
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
__u32	Cls_GrDvrFsoDsk::LcCalcBufFrmsSizeBack( void* A_PtrBuf, __u32 A_BufSize )
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
				DbgMsgPrint( "Cls_GrDvrFsoDsk::LcCalcBufFrmsSizeBack: bad tail code.\n" );
				Tv_PktSize	=	E_GrDvrFsoStrmAlignSize;
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
__u8	Cls_GrDvrFsoDsk::ReadBack( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, 
																__u64* A_PtrRtCrntHndl )
{
	// local -------------------
		__u8	Tv_Result;
		__u16	Tv_HndlSeqId;
		__u64	Tv_DskPos;
		__u64	Tv_NextPos;
		__u32	Tv_ReadSize;
		__u32	Tv_AlignFree;
		void*	Tv_PtrBuf;
		__u32	Tv_FrmAreaSize;
	// code --------------------
		*A_PtrRtSize	=	0;
		*A_PtrRtCrntHndl	=	A_Hndl;

		//get basic info
		Tv_HndlSeqId	=	(__u16)( A_Hndl >> E_GrDvrFsoHndlSeqPos );
		Tv_DskPos			=	A_Hndl & E_GrDvrFsoHndlPosMask;

		//check disk empty
		Tv_Result			=	E_GrDvrRecErrDataEmpty;
		if ( m_Rca.RecPosIn != m_Rca.RecPosOut )
		{
			//check disk status
			Tv_Result	=	E_GrDvrRecErrDskBad;
			while ( E_GrDvrFsoDiskStatBad != m_Rca.DiskStat )
			{
				//check handle and change first postion
				if ( Tv_DskPos == m_Mis.RdaPos )
				{
					//disk
					Tv_Result	=	E_GrDvrRecErrDskEnd;
					break;
				}
				if ( Tv_HndlSeqId == m_Rca.RecSeqId )
				{
					//match record sequence
					if ( m_Rca.RecPosOut < m_Rca.RecPosIn )
					{
						if ( Tv_DskPos <= m_Rca.RecPosOut )
						{
							//disk
							Tv_Result	=	E_GrDvrRecErrDskEnd;
							break;
						}
						else
						{
							if ( Tv_DskPos > m_Rca.RecPosIn )
							{
								Tv_Result	=	E_GrDvrRecErrHndlBad;
								break;
							}
						}
					}
					else
					{
						//record out > record in
						if ( Tv_DskPos > m_Rca.RecPosIn )
						{
							Tv_Result	=	E_GrDvrRecErrHndlBad;
							break;
						}
					}
				}
				else if ( (Tv_HndlSeqId + 1) == m_Rca.RecSeqId )
				{
					//one time old sequence
					//check out
					if (m_Rca.RecPosOut < m_Rca.RecPosIn)
					{
						Tv_Result	=	E_GrDvrRecErrDskEnd;
						break;
					}
					else
					{
						if ( Tv_DskPos < m_Rca.RecPosOut )
						{
							Tv_Result	=	E_GrDvrRecErrDskEnd;
							break;
						}
					}
				}	//check record sequence

				//do record
				//check bad block
				if ( !LcIsBadBlock( Tv_DskPos - 1 ) )
				{
					//check size
					Tv_ReadSize		=	A_Size;
					Tv_AlignFree	=	(__u32)( Tv_DskPos & (~E_GrDvrFsoRdaAlignMask));
					if ( 0 == Tv_AlignFree )
					{
						Tv_AlignFree	=	E_GrDvrFsoRdaAlignUnit;
					}
					if ( Tv_ReadSize >= Tv_AlignFree )
					{
						Tv_ReadSize	=	Tv_AlignFree;
					}
					//check read area out
					Tv_NextPos	=	Tv_DskPos - (__u64)Tv_ReadSize;
					if ( m_Rca.RecPosOut < m_Rca.RecPosIn )
					{
						if ( Tv_NextPos < m_Rca.RecPosOut )
						{
							Tv_ReadSize	=	(__u32)(Tv_DskPos - m_Rca.RecPosOut);
						}
					}
					else
					{
						// inpos < outout mode
						if ( (Tv_NextPos < m_Rca.RecPosOut) && ( Tv_DskPos > m_Rca.RecPosOut ) )
						{
							Tv_ReadSize	=	(__u32)(Tv_DskPos - m_Rca.RecPosOut);
						}
					}
					Tv_NextPos	=	Tv_DskPos - (__u64)Tv_ReadSize;	//recalculate next position
					//move position
					if ( LcSeek( Tv_NextPos ) )
					{
						Tv_PtrBuf	=	(void*)( (__u32)A_PtrBuf + (A_Size - Tv_ReadSize) );
						if ( LcRead( Tv_PtrBuf, Tv_ReadSize ) )
						{
							//success
							//get frame area size
							Tv_FrmAreaSize		=	LcCalcBufFrmsSizeBack( Tv_PtrBuf, Tv_ReadSize );
							//Tv_FrmAreaSize		=	Tv_ReadSize;
							*A_PtrRtSize			=	Tv_FrmAreaSize;
							Tv_NextPos				=	Tv_DskPos - (__u64)Tv_FrmAreaSize;
							*A_PtrRtCrntHndl	=	Tv_NextPos | ( ((__u64)Tv_HndlSeqId) << E_GrDvrFsoHndlSeqPos ) | (__u64)m_GrpSubIdx;
							Tv_Result					=	E_GrDvrRecErrNone;
							break;
						}
					}
					//add bad block
					LcBadBlockAdd( Tv_DskPos - 1 );
				}
				else
				{
					//bad block, go next positon
					Tv_NextPos	=	E_GrDvrFsoRdaAlignMask & (Tv_DskPos - 1);
					//check finished
					if ( Tv_NextPos <= m_Mis.RdaPos )
					{
						//finish
						Tv_Result	=	E_GrDvrRecErrDskEnd;
						break;
					}
					else
					{
						//check record finish position
						if ( (Tv_DskPos >= m_Rca.RecPosOut) && (Tv_NextPos <= m_Rca.RecPosOut) )
						{
							//finish
							Tv_Result	=	E_GrDvrRecErrDskEnd;
							break;
						}
					}
				}	//if ( !LcIsBadBlock( Tv_DskPos ) )

			}	//while ( E_GrDvrFsoDiskStatBad != m_Rca.DiskStat )
			//update result
			if ( E_GrDvrFsoDiskStatBad == m_Rca.DiskStat )
			{
				Tv_Result	=	E_GrDvrRecErrDskBad;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoDsk::ReadPrev( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl )
{
	// local -------------------
		__u8	Tv_Result;
		__u16	Tv_HndlSeqId;
		__u64	Tv_DskPos;
		__u64	Tv_NextPos;
		__u32	Tv_ReadSize;
		__u32	Tv_AlignFree;
		void*	Tv_PtrBuf;
		//__u32	Tv_FrmAreaSize;
	// code --------------------
		*A_PtrRtSize	=	0;
		*A_PtrRtCrntHndl	=	A_Hndl;

		//get basic info
		Tv_HndlSeqId	=	(__u16)( A_Hndl >> E_GrDvrFsoHndlSeqPos );
		Tv_DskPos			=	A_Hndl & E_GrDvrFsoHndlPosMask;

		//check disk empty
		Tv_Result			=	E_GrDvrRecErrDataEmpty;
		if ( m_Rca.RecPosIn != m_Rca.RecPosOut )
		{
			//check disk status
			Tv_Result	=	E_GrDvrRecErrDskBad;
			while ( E_GrDvrFsoDiskStatBad != m_Rca.DiskStat )
			{
				//check handle and change first postion
				if ( Tv_DskPos == m_Mis.RdaPos )
				{
					//disk
					Tv_Result	=	E_GrDvrRecErrDskEnd;
					break;
				}
				if ( Tv_HndlSeqId == m_Rca.RecSeqId )
				{
					//match record sequence
					if ( m_Rca.RecPosOut < m_Rca.RecPosIn )
					{
						if ( Tv_DskPos <= m_Rca.RecPosOut )
						{
							//disk
							Tv_Result	=	E_GrDvrRecErrDskEnd;
							break;
						}
						else
						{
							if ( Tv_DskPos > m_Rca.RecPosIn )
							{
								Tv_Result	=	E_GrDvrRecErrHndlBad;
								break;
							}
						}
					}
					else
					{
						//record out > record in
						if ( Tv_DskPos > m_Rca.RecPosIn )
						{
							Tv_Result	=	E_GrDvrRecErrHndlBad;
							break;
						}
					}
				}
				else if ( (Tv_HndlSeqId + 1) == m_Rca.RecSeqId )
				{
					//one time old sequence
					//check out
					if (m_Rca.RecPosOut < m_Rca.RecPosIn)
					{
						Tv_Result	=	E_GrDvrRecErrDskEnd;
						break;
					}
					else
					{
						if ( Tv_DskPos < m_Rca.RecPosOut )
						{
							Tv_Result	=	E_GrDvrRecErrDskEnd;
							break;
						}
					}
				}	//check record sequence

				//do record
				//check bad block
				//if ( !LcIsBadBlock( Tv_DskPos - 1 ) )
				{
					//check size
					Tv_ReadSize		=	A_Size;
					Tv_AlignFree	=	(__u32)( Tv_DskPos & (~E_GrDvrFsoRdaAlignMask));
					if ( 0 == Tv_AlignFree )
					{
						Tv_AlignFree	=	E_GrDvrFsoRdaAlignUnit;
					}
					if ( Tv_ReadSize >= Tv_AlignFree )
					{
						Tv_ReadSize	=	Tv_AlignFree;
					}
					//check read area out
					Tv_NextPos	=	Tv_DskPos - (__u64)Tv_ReadSize;
					if ( m_Rca.RecPosOut < m_Rca.RecPosIn )
					{
						if ( Tv_NextPos < m_Rca.RecPosOut )
						{
							Tv_ReadSize	=	(__u32)(Tv_DskPos - m_Rca.RecPosOut);
						}
					}
					else
					{
						// inpos < outout mode
						if ( (Tv_NextPos < m_Rca.RecPosOut) && ( Tv_DskPos > m_Rca.RecPosOut ) )
						{
							Tv_ReadSize	=	(__u32)(Tv_DskPos - m_Rca.RecPosOut);
						}
					}
					Tv_NextPos	=	Tv_DskPos - (__u64)Tv_ReadSize;	//recalculate next position
					//DbgMsgPrint( "CHK rpv pos=%08X%08X size=%8X\n", (__u32)(Tv_NextPos >> 32), (__u32)(Tv_NextPos & 0xFFFFFFFF), Tv_ReadSize );
					//move position
					if ( LcSeek( Tv_NextPos ) )
					{
						Tv_PtrBuf	=	(void*)( (__u32)A_PtrBuf + (A_Size - Tv_ReadSize) );
						if ( LcRead( Tv_PtrBuf, Tv_ReadSize ) )
						{
							//success
							//get frame area size
							//Tv_FrmAreaSize		=	LcCalcBufFrmsSizeBack( Tv_PtrBuf, Tv_ReadSize );
							//Tv_FrmAreaSize		=	Tv_ReadSize;
							*A_PtrRtSize			=	Tv_ReadSize;
							Tv_NextPos				=	Tv_DskPos - (__u64)Tv_ReadSize;
							*A_PtrRtCrntHndl	=	Tv_NextPos | ( ((__u64)Tv_HndlSeqId) << E_GrDvrFsoHndlSeqPos ) | (__u64)m_GrpSubIdx;
							Tv_Result					=	E_GrDvrRecErrNone;
							break;
						}
					}
				}

			}	//while ( E_GrDvrFsoDiskStatBad != m_Rca.DiskStat )
			//update result
			if ( E_GrDvrFsoDiskStatBad == m_Rca.DiskStat )
			{
				Tv_Result	=	E_GrDvrRecErrDskBad;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::LcMakeHndlByPos( __u64 A_Pos )
{
	// local -------------------
		__u16	Tv_SeqId;
	// code --------------------
		Tv_SeqId	=	m_Rca.RecSeqId;
		if ( (m_Rca.RecPosIn < m_Rca.RecPosOut) && (A_Pos >= m_Rca.RecPosOut) )
		{
			Tv_SeqId--;
		}
		return	A_Pos | ( ((__u64)Tv_SeqId) << E_GrDvrFsoHndlSeqPos ) | (__u64)m_GrpSubIdx;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::RecOutPosHndlGet( void )
{
		return	LcMakeHndlByPos( m_Rca.RecPosOut );
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::RecInPosHndlGet( void )
{
		return	LcMakeHndlByPos( m_Rca.RecPosIn );
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::RdaPosHndlGet( void )
{
		return	LcMakeHndlByPos( m_Mis.RdaPos );
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::RdaLastHndlGet( void )
{
	return	LcMakeHndlByPos( m_Mis.RdaLast );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoDsk::BufSync( void )
{
	// local -------------------
	// code --------------------
		//check HDD bad
		if ( E_GrDvrFsoDiskStatBad != m_Rca.DiskStat )
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
void	Cls_GrDvrFsoDsk::EvtLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		__u32	Tv_PosIn;
		__u32	Tv_PosOut;
		__u64	Tv_DskPos;
	// code --------------------
		//direct write event LOG
		//calculate disk position

		//check disk status
		if (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat)
		{
			//write item
			A_PtrItem->IsAble	=	TRUE;		//patch able
			Tv_PosIn	=	m_Rca.EvtPosIn;
			Tv_DskPos	=	E_GrDvrFsoDposEla + (__u64)(Tv_PosIn << E_GrDvrFsoElaItmSzSft);
			if ( LcSeek( Tv_DskPos ) )
			{
				if ( LcWrite( A_PtrItem, E_GrDvrFsoElaItmSize ) )
				{
					//update next position
					//get next pos
					Tv_PosIn ++;
					if ( E_GrDvrFsoElaMaxItmCnt <= Tv_PosIn )
					{
						Tv_PosIn	=	0;
					}
					Tv_PosOut	=	m_Rca.EvtPosOut;
					//check last item
					if ( Tv_PosIn == Tv_PosOut )
					{
						//delete item
						Tv_PosOut	++;
						if ( E_GrDvrFsoElaMaxItmCnt <= Tv_PosOut )
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
					m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
					DbgMsgPrint("Cls_GrDvrFsoDsk::EvtLogAdd - log area bad!\n");
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
				}
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFsoDsk::EvtLogGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt )
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

		//check disk staus
		if (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat)
		{
			//load event log
			if ( LcSeek( E_GrDvrFsoDposEla ) )
			{
				if ( LcRead( m_PtrLogRdBuf, E_GrDvrFsoElaSize ) )
				{
					Tv_PtrLogTbl	=	(Ptr_GrDvrLogItm)m_PtrLogRdBuf;
					Tv_ChkIdx			=	(__s32)m_Rca.EvtPosIn;
					for ( Tv_WkIdx=0; E_GrDvrFsoElaMaxItmCnt > Tv_WkIdx;Tv_WkIdx++ )
					{
						//get preview
						Tv_ChkIdx --;
						if ( 0 > Tv_ChkIdx )
						{
							Tv_ChkIdx	=	E_GrDvrFsoElaMaxItmCnt - 1;
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
								//finded, add
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
BOOL8	Cls_GrDvrFsoDsk::UcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result; 
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat)
		{
			if ( E_GrDvrFsoUcaMaxSize >= (A_Pos + A_Size) )
			{
				if ( LcSeek( E_GrDvrFsoDposUca + (__u64)A_Pos ) )
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
BOOL8	Cls_GrDvrFsoDsk::UcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result; 
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat)
		{
			if ( E_GrDvrFsoUcaMaxSize >= (A_Pos + A_Size) )
			{
				if ( LcSeek( E_GrDvrFsoDposUca + (__u64)A_Pos ) )
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
BOOL8	Cls_GrDvrFsoDsk::UwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result; 
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat)
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
BOOL8	Cls_GrDvrFsoDsk::UwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result; 
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat)
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
BOOL8	Cls_GrDvrFsoDsk::LcSmartRead( void )
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
BOOL8	Cls_GrDvrFsoDsk::LcSmartFindVal( __u8 A_Id, __u8* A_PtrRtVal )
{
	// local -------------------	
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_BytePos;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_BytePos	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoSmartValCnt;Tv_WkIdx++ )
		{
			if ( A_Id == m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFsoSmartValIdxId] )
			{
				*A_PtrRtVal	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFsoSmartValIdxValue];
				Tv_Result	=	TRUE;
				break;
			}
			//next
			Tv_BytePos	=	Tv_BytePos + E_GrDvrFsoSmartValItemSize;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal )
{
	// local -------------------	
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_BytePos;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_BytePos	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoSmartValCnt;Tv_WkIdx++ )
		{
			if ( A_Id == m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFsoSmartValIdxId] )
			{
				*A_PtrRtVal	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFsoSmartValIdxRawVal];
				Tv_Result	=	TRUE;
				break;
			}
			//next
			Tv_BytePos	=	Tv_BytePos + E_GrDvrFsoSmartValItemSize;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoDsk::TemperatureGet( void )
{
	// local -------------------
	// code --------------------
		return	m_ValTemp;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::WbaWrite( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result; 
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat)
		{
			if ( E_GrDvrFsoWbaSize >= A_Size )
			{
				if ( LcSeek( E_GrDvrFsoDposWba ) )
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
BOOL8	Cls_GrDvrFsoDsk::WbaRead( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result; 
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat)
		{
			if ( E_GrDvrFsoWbaSize >= A_Size )
			{
				if ( LcSeek( E_GrDvrFsoDposWba ) )
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
void	Cls_GrDvrFsoDsk::LcEvtLogDisable( void )
{
	// local -------------------
		Ptr_GrDvrLogItm	Tv_PtrLogTbl;
		__u32	Tv_Cnt;
	// code --------------------
		//load event log
		if ( LcSeek( E_GrDvrFsoDposEla ) )
		{
			if ( LcRead( m_PtrLogRdBuf, E_GrDvrFsoElaSize ) )
			{
				Tv_PtrLogTbl	=	(Ptr_GrDvrLogItm)m_PtrLogRdBuf;
				for ( Tv_Cnt=E_GrDvrFsoElaMaxItmCnt;0 != Tv_Cnt;Tv_Cnt-- )
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
void	Cls_GrDvrFsoDsk::RecStatusGet( Ptr_GrDvrFsoRecStat A_PtrStat )
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
			if (E_GrDvrFsoDiskStatBad == m_Rca.DiskStat)
			{
				A_PtrStat->IsBad	=	TRUE;
				A_PtrStat->RecSize	=	A_PtrStat->DiskSize;
			}
			else
			{
				//check full
				if ( E_GrDvrFsoDiskStatNormal == m_Rca.DiskStat)
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
void	Cls_GrDvrFsoDsk::GroupSubIndexSet( __u8 A_Idx )
{
		m_GrpSubIdx	=	A_Idx;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::UwaDiskPosGet( void )
{
		return	m_Mis.UwaPos;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::UwaSizeGet( void )
{
		return	m_Mis.UwaSize;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::IsOverwriting( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsFormatted )
		{
			if ( E_GrDvrFsoDiskStatBad != m_Rca.DiskStat )
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
BOOL8	Cls_GrDvrFsoDsk::IsEmpty( void )
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
__u64	Cls_GrDvrFsoDsk::RecOutPosGet( void )
{
		return	m_Rca.RecPosOut;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::RecInPosGet( void )
{
		return	m_Rca.RecPosIn;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::RdaStartPosGet( void )
{
		return	m_Mis.RdaPos;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::RdaLastPosGet( void )
{
		return	m_Mis.RdaLast;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFsoDsk::RecTimeBeginGet( void )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__s32				Tv_DmdIdx;
		__s32				Tv_MmdIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrMmd;
		__u64				Tv_Dpos;
		BOOL8				Tv_IsEmpty;
		BOOL8				Tv_IsChg;
	// code --------------------
		Tv_Result	=	m_RecTimeBegin;
		if ( 0 == Tv_Result )
		{
			Tv_PtrMmd	=	(Ptr_GrDvrRecMinThumb)m_PtrCfg->TmpBuf;
			// find first day
			Tv_DmdIdx	=	LcDmdFindBegin();
			while ( 0 <= Tv_DmdIdx )
			{
				// load mmd
				Tv_Dpos	=	E_GrDvrFsoDposMmd + (__u64)( Tv_DmdIdx * E_GrDvrFsoMmdDaySize );
				if ( !LcPread( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
				{
					// mmd read fail
					DbgMsgPrint( "Cls_GrDvrFsoDsk::RecTimeBeginGet - MMD read fail!\n" );
					m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
					break;
				}
				// check mmd
				Tv_IsEmpty	=	TRUE;
				Tv_IsChg		=	FALSE;
				for (Tv_MmdIdx=0;Tv_MmdIdx < E_GrTimeMinOfDay;Tv_MmdIdx++)
				{
					// check exist
					if ( E_GrDvrFsoHndlNone != Tv_PtrMmd[Tv_MmdIdx].HndlRec )
					{
						// check 
						if ( LcIsCrntHndl( Tv_PtrMmd[Tv_MmdIdx].HndlRec ) )
						{
							// found
							Tv_Result		=	m_Dmd[Tv_DmdIdx] + (Def_GrTime)( Tv_MmdIdx * E_GrTimeSecOfMin );
							Tv_IsEmpty	=	FALSE;
							break;
						}
						else
						{
							// patch
							GrDumyZeroMem( &Tv_PtrMmd[Tv_MmdIdx], E_GrDvrRecMinThumbItemSize );
							Tv_IsChg	=	TRUE;
						}
					}
				}

				// check changed
				if ( Tv_IsChg )
				{
					// write
					LcPwrite( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize );
				}

				// check empty
				if ( Tv_IsEmpty )
				{
					m_Dmd[Tv_DmdIdx]	=	0;		// clear day
					// write DMD
					Tv_Dpos	=	E_GrDvrFsoDposDmd + (__u64)( Tv_DmdIdx * 4 );
					LcPwrite( Tv_Dpos, &m_Dmd[Tv_DmdIdx], 4 );
				}

				// check finish
				if ( 0 != Tv_Result )
				{
					break;
				}

				// next
				Tv_DmdIdx	=	LcDmdFindBegin();
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFsoDsk::RecTimeEndGet( void )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__s32				Tv_DmdIdx;
		__s32				Tv_MmdIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrMmd;
		__u64				Tv_Dpos;
		BOOL8				Tv_IsEmpty;
		BOOL8				Tv_IsChg;
	// code --------------------
		Tv_Result	=	m_RecTimeEnd;
		if ( 0 == Tv_Result )
		{
			Tv_PtrMmd	=	(Ptr_GrDvrRecMinThumb)m_PtrCfg->TmpBuf;
			// find first day
			Tv_DmdIdx	=	LcDmdFindEnd();
			while ( 0 <= Tv_DmdIdx )
			{
				// load mmd
				Tv_Dpos	=	E_GrDvrFsoDposMmd + (__u64)( Tv_DmdIdx * E_GrDvrFsoMmdDaySize );
				if ( !LcPread( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
				{
					// mmd read fail
					DbgMsgPrint( "Cls_GrDvrFsoDsk::RecTimeBeginGet - MMD read fail!\n" );
					m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
					break;
				}
				// check mmd
				Tv_IsEmpty	=	TRUE;
				Tv_IsChg		=	FALSE;
				for (Tv_MmdIdx=E_GrTimeMinOfDay - 1;Tv_MmdIdx >= 0;Tv_MmdIdx--)
				{
					// check exist
					if ( E_GrDvrFsoHndlNone != Tv_PtrMmd[Tv_MmdIdx].HndlRec )
					{
						// check 
						if ( LcIsCrntHndl( Tv_PtrMmd[Tv_MmdIdx].HndlRec ) )
						{
							// found
							Tv_Result		=	m_Dmd[Tv_DmdIdx] + (Def_GrTime)( Tv_MmdIdx * E_GrTimeSecOfMin );
							Tv_IsEmpty	=	FALSE;
							break;
						}
						else
						{
							// patch
							GrDumyZeroMem( &Tv_PtrMmd[Tv_MmdIdx], E_GrDvrRecMinThumbItemSize );
							Tv_IsChg	=	TRUE;
						}
					}
				}

				// check changed
				if ( Tv_IsChg )
				{
					// write
					LcPwrite( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize );
				}

				// check empty
				if ( Tv_IsEmpty )
				{
					m_Dmd[Tv_DmdIdx]	=	0;		// clear day
					// write DMD
					Tv_Dpos	=	E_GrDvrFsoDposDmd + (__u64)( Tv_DmdIdx * 4 );
					if ( !LcPwrite( Tv_Dpos, &m_Dmd[Tv_DmdIdx], 4 ) )
					{

					}
				}

				// check finish
				if ( 0 != Tv_Result )
				{
					break;
				}

				// next
				Tv_DmdIdx	=	LcDmdFindEnd();
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoDsk::TemperatureUpdt( void )
{
	// local -------------------
	// code --------------------
		if ( LcSmartRead() )
		{
			LcSmartFindRawByte( E_GrDvrFsoDskSmartIdTemp, &m_ValTemp );
		}
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::FindHndlByMinFw( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TmScan;
		Def_GrTime	Tv_TmDay;
		Def_GrTime	Tv_TmWk;
		__s32	Tv_WkIdx;
		__s32	Tv_DmdIdx;
		__s32	Tv_MmdIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrMmd;
		__u64	Tv_Dpos;
		BOOL8	Tv_IsEmpty;
		BOOL8	Tv_IsChg;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;

		//check formatted
		if ( m_IsFormatted && (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat) )
		{
			// init
			Tv_TmScan	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin);
			Tv_PtrMmd	=	(Ptr_GrDvrRecMinThumb)m_PtrCfg->TmpBuf;

			while ( (E_GrDvrFsoHndlNone == Tv_Result) && (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat) )
			{
				// find day
				Tv_DmdIdx	=	-1;
				Tv_TmDay	=	Tv_TmScan - (Tv_TmScan % E_GrTimeSecOfDay);
				Tv_TmWk		=	0;
				// find DMD
				for (Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoDmdCnt;Tv_WkIdx++)
				{
					if ( (0 != m_Dmd[Tv_WkIdx]) && (Tv_TmDay <= m_Dmd[Tv_WkIdx]) )
					{
						if ( (0 == Tv_TmWk) || (Tv_TmWk > m_Dmd[Tv_WkIdx]) )
						{
							Tv_TmWk		=	m_Dmd[Tv_WkIdx];
							Tv_DmdIdx	=	Tv_WkIdx;
						}
					}
				}

				// check not found
				if ( 0 > Tv_DmdIdx )
				{
					break;
				}

				// patch scan time
				if ( Tv_TmDay != m_Dmd[Tv_DmdIdx] )
				{
					Tv_TmScan	=	m_Dmd[Tv_DmdIdx];
					Tv_TmDay	=	m_Dmd[Tv_DmdIdx];
				}

				// read MMD
				Tv_Dpos	=	E_GrDvrFsoDposMmd + (__u64)( Tv_DmdIdx * E_GrDvrFsoMmdDaySize );
				if ( !LcPread( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
				{
					m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
					break;
				}

				// find
				Tv_IsEmpty	=	TRUE;
				Tv_IsChg		=	FALSE;
				Tv_TmWk			=	Tv_TmDay;
				for (Tv_MmdIdx=0;Tv_MmdIdx < E_GrTimeMinOfDay;Tv_MmdIdx++)
				{
					// check time
					if ( Tv_TmWk > A_TimeEd )
					{
						Tv_IsEmpty	=	FALSE;
						break;
					}

					// check exist
					if ( E_GrDvrRecHndlNone != Tv_PtrMmd[Tv_MmdIdx].HndlRec )
					{
						// check valid handle
						if ( LcIsCrntHndl( Tv_PtrMmd[Tv_MmdIdx].HndlRec ) )
						{
							// exist
							Tv_IsEmpty	=	FALSE;
							// check scantime
							if ( Tv_TmScan <= Tv_TmWk )
							{
								// found
								Tv_Result		=	Tv_PtrMmd[Tv_MmdIdx].HndlRec;
								break;
							}
						}
						else
						{
							// invalid
							GrDumyZeroMem( &Tv_PtrMmd[Tv_MmdIdx], E_GrDvrRecMinThumbItemSize );
							Tv_IsChg		=	TRUE;
						}
					}
					// next
					Tv_TmWk		=	Tv_TmWk + E_GrTimeSecOfMin;
				}

				// check change
				if ( Tv_IsChg )
				{
					if ( !LcPwrite( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
					{
						m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
						break;
					}
				}

				// check empty
				if ( Tv_IsEmpty )
				{
					m_Dmd[Tv_DmdIdx]	=	0;	// clear
					Tv_Dpos	=	E_GrDvrFsoDposDmd + (__u64)( Tv_DmdIdx * 4 );
					if ( !LcPwrite( Tv_Dpos, &m_Dmd[Tv_DmdIdx], 4 ) )
					{
						m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
						break;
					}
				}

				// next
				Tv_TmScan	=	Tv_TmDay + E_GrTimeSecOfDay;

			}	//	while ( (E_GrDvrFsoHndlNone == Tv_Result) && (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat) )

		}	//	if ( m_IsFormatted && (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat) )

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::FindHndlByMinBw( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TmScan;
		Def_GrTime	Tv_TmDay;
		Def_GrTime	Tv_TmWk;
		__s32	Tv_WkIdx;
		__s32	Tv_DmdIdx;
		__s32	Tv_MmdIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrMmd;
		__u64	Tv_Dpos;
		BOOL8	Tv_IsEmpty;
		BOOL8	Tv_IsChg;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;

		//check formatted
		if ( m_IsFormatted && (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat) )
		{
			// init
			Tv_TmScan	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin);
			Tv_PtrMmd	=	(Ptr_GrDvrRecMinThumb)m_PtrCfg->TmpBuf;

			while ( (E_GrDvrFsoHndlNone == Tv_Result) && (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat) )
			{
				// find day
				Tv_DmdIdx	=	-1;
				Tv_TmDay	=	Tv_TmScan - (Tv_TmScan % E_GrTimeSecOfDay);
				Tv_TmWk		=	0;
				// find DMD
				for (Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoDmdCnt;Tv_WkIdx++)
				{
					if ( (0 != m_Dmd[Tv_WkIdx]) && (Tv_TmDay >= m_Dmd[Tv_WkIdx]) )
					{
						if ( (0 == Tv_TmWk) || (Tv_TmWk < m_Dmd[Tv_WkIdx]) )
						{
							Tv_TmWk		=	m_Dmd[Tv_WkIdx];
							Tv_DmdIdx	=	Tv_WkIdx;
						}
					}
				}

				// check not found
				if ( 0 > Tv_DmdIdx )
				{
					break;
				}

				// patch scan time
				if ( Tv_TmDay != m_Dmd[Tv_DmdIdx] )
				{
					Tv_TmScan	=	m_Dmd[Tv_DmdIdx] + E_GrTimeSecOfDay - E_GrTimeSecOfMin;
					Tv_TmDay	=	m_Dmd[Tv_DmdIdx];
				}

				// read MMD
				Tv_Dpos	=	E_GrDvrFsoDposMmd + (__u64)( Tv_DmdIdx * E_GrDvrFsoMmdDaySize );
				if ( !LcPread( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
				{
					m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
					break;
				}

				// find
				Tv_IsEmpty	=	TRUE;
				Tv_IsChg		=	FALSE;
				Tv_TmWk			=	Tv_TmDay + E_GrTimeSecOfDay - E_GrTimeSecOfMin;
				for (Tv_MmdIdx=E_GrTimeMinOfDay - 1;Tv_MmdIdx >= 0;Tv_MmdIdx--)
				{
					// check time
					if ( Tv_TmWk < A_TimeEd )
					{
						Tv_IsEmpty	=	FALSE;
						break;
					}
					// check exist
					if ( E_GrDvrRecHndlNone != Tv_PtrMmd[Tv_MmdIdx].HndlRec )
					{
						// check valid handle
						if ( LcIsCrntHndl( Tv_PtrMmd[Tv_MmdIdx].HndlRec ) )
						{
							// exist
							Tv_IsEmpty	=	FALSE;
							// check scantime
							if ( Tv_TmScan >= Tv_TmWk )
							{
								// found
								Tv_Result		=	Tv_PtrMmd[Tv_MmdIdx].HndlRec;
								break;
							}
						}
						else
						{
							// invalid
							GrDumyZeroMem( &Tv_PtrMmd[Tv_MmdIdx], E_GrDvrRecMinThumbItemSize );
							Tv_IsChg		=	TRUE;
						}
					}
					// next
					Tv_TmWk		=	Tv_TmWk - E_GrTimeSecOfMin;
				}

				// check change
				if ( Tv_IsChg )
				{
					if ( !LcPwrite( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
					{
						m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
						break;
					}
				}

				// check empty
				if ( Tv_IsEmpty )
				{
					m_Dmd[Tv_DmdIdx]	=	0;	// clear
					Tv_Dpos	=	E_GrDvrFsoDposDmd + (__u64)( Tv_DmdIdx * 4 );
					if ( !LcPwrite( Tv_Dpos, &m_Dmd[Tv_DmdIdx], 4 ) )
					{
						m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
						break;
					}
				}

				// next
				Tv_TmScan	=	Tv_TmDay - E_GrTimeSecOfMin;

			}	//	while ( (E_GrDvrFsoHndlNone == Tv_Result) && (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat) )

		}	//	if ( m_IsFormatted && (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat) )

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoDsk::DataSizeByMinuteRange( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TmRecSt;
		Def_GrTime	Tv_TmRecEd;
		Def_GrTime	Tv_TmChkSt;
		Def_GrTime	Tv_TmChkEd;
		__u64	Tv_PosSt;
		__u64	Tv_PosEd;
	// code --------------------
		Tv_Result	=	0;

		if ( m_IsFormatted && ( E_GrDvrFsoDiskStatBad != m_Rca.DiskStat ) )
		{
			//get local hdd time range
			Tv_TmRecSt	=	RecTimeBeginGet();
			Tv_TmRecEd	=	RecTimeEndGet();

			//check current time range
			if ( (A_TimeSt < Tv_TmRecEd) && (A_TimeEd > Tv_TmRecSt ) )
			{
				//get start handle
				if ( A_TimeSt <= Tv_TmRecSt )
				{
					Tv_PosSt	=	RecOutPosGet();
				}
				else
				{
					Tv_TmChkSt	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin);
					Tv_PosSt		=	E_GrDvrFsoHndlPosMask & FindHndlByMinFw( Tv_TmChkSt, A_TimeEd );
				}
				//get end handl
				if ( A_TimeEd >= Tv_TmRecEd )
				{
					Tv_PosEd	=	RecInPosGet();
				}
				else
				{
					Tv_TmChkEd	=	A_TimeEd + E_GrTimeSecOfMin - 1;
					Tv_TmChkEd	=	Tv_TmChkEd - (Tv_TmChkEd % E_GrTimeSecOfMin);
					Tv_PosEd	=	E_GrDvrFsoHndlPosMask & FindHndlByMinBw( Tv_TmChkEd, A_TimeSt );
				}
				//check calculate able
				if ( (0 != Tv_PosSt) && (0 != Tv_PosEd) )
				{
					if ( m_Rca.RecPosIn != m_Rca.RecPosOut )
					{
						// check overwrite mode
						if ( m_Rca.RecPosIn > m_Rca.RecPosOut )
						{
							if ( Tv_PosSt < Tv_PosEd )
							{
								Tv_Result	=	Tv_PosEd - Tv_PosSt;
							}
						}
						else
						{
							if ( ((m_Rca.RecPosIn > Tv_PosSt) && (m_Rca.RecPosIn >= Tv_PosEd)) || ((m_Rca.RecPosOut <= Tv_PosSt) && (m_Rca.RecPosIn < Tv_PosEd))  )
							{
								if ( Tv_PosSt < Tv_PosEd )
								{
									Tv_Result	=	Tv_PosEd - Tv_PosSt;
								}
							}
							else
							{
								if ( (m_Rca.RecPosOut <= Tv_PosSt ) && ( m_Rca.RecPosIn >= Tv_PosEd ) )
								{
									Tv_Result	=	(m_Mis.RdaLast - Tv_PosSt) + (Tv_PosEd - m_Mis.RdaPos);
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
BOOL8	Cls_GrDvrFsoDsk::IsVssSaved( void )
{
		return	m_Rca.IsVss;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::LcFillByte( __u64 A_PosTg, __u32 A_Size, __u8 A_Fill )
{
	// local -------------------
		BOOL8	Tv_Result;
		void*	Tv_PtrBuf;
		__u64	Tv_PosTg;
		__u32	Tv_WkSize;
		__u32	Tv_Remain;
	// code --------------------
		Tv_Result	=	TRUE;
		// allocate buffer
		Tv_PtrBuf	=	(void*)m_PtrCfg->TmpBuf;
		// fill buffer
		GrDumyFillB( Tv_PtrBuf, E_GrDvrFsoTmpBufSize, A_Fill );
		// init
		Tv_Remain	=	A_Size;
		Tv_PosTg	=	A_PosTg;

		while ( 0 != Tv_Remain )
		{
			Tv_WkSize	=	Tv_Remain;
			if ( E_GrDvrFsoTmpBufSize < Tv_WkSize )
			{
				Tv_WkSize	=	E_GrDvrFsoTmpBufSize;
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

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::LcCopy( __u64 A_PosTg, __u64 A_PosSr, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		void*	Tv_PtrBuf;
		__u64	Tv_PosSr;
		__u64	Tv_PosTg;
		__u32	Tv_CpSize;
		__u32	Tv_Remain;
	// code --------------------
		Tv_Result	=	TRUE;
		Tv_PtrBuf	=	(void*)m_PtrCfg->TmpBuf;

		Tv_Remain	=	A_Size;
		Tv_PosSr	=	A_PosSr;
		Tv_PosTg	=	A_PosTg;

		while ( 0 != Tv_Remain )
		{
			Tv_CpSize	=	Tv_Remain;
			if ( E_GrDvrFsoTmpBufSize < Tv_CpSize )
			{
				Tv_CpSize	=	E_GrDvrFsoTmpBufSize;
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

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::LcPread( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFsoDsk::LcPwrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFsoDsk::LcUpgdOldFs( __u32 A_OldVer )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_ScanCnt;
		__u32	Tv_WkCnt;
		__u32	Tv_WkIdx;
		Def_GrTime	Tv_TmPrc;
		Ptr_GrDvrRecMinThumb	Tv_PtrTma;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	FALSE;

		// move UWA
		if ( E_GrDvrFsoVerBigTma > A_OldVer )
		{
			LcCopy( E_GrDvrFsoDposUwa, m_Mis.UwaPos, (__u32)m_Mis.UwaSize );
		}

		// clear new area
		LcFillByte( E_GrDvrFsoDposDmd, E_GrDvrFsoDmdAreaSize + E_GrDvrFsoMmdAreaSize, 0 );

		// rebuild tma
		Tv_ScanCnt	=	m_Mis.TmaCnt;
		if ( E_GrDvrFsoTmaScanCnt < Tv_ScanCnt )
		{
			Tv_ScanCnt	=	E_GrDvrFsoTmaScanCnt;
		}

		// init
		Tv_Result	=	TRUE;
		Tv_PtrTma	= (Ptr_GrDvrRecMinThumb)m_PtrCfg->TmpBuf;
		// scan TMA
		Tv_Dpos		=	E_GrDvrFsoDposTma;
		Tv_TmPrc	=	GrTimeEnc( 2008, 1, 1, 0, 0, 0 );
			
		while ( Tv_Result && (0 != Tv_ScanCnt) )
		{
			Tv_WkCnt	=	E_GrDvrFsoUpgdBufSize >> E_GrDvrRecMinThumbSizeShft;
			if ( Tv_ScanCnt < Tv_WkCnt )
			{
				Tv_WkCnt	=	Tv_ScanCnt;
			}

			// read data
			if ( !LcPread( Tv_Dpos, Tv_PtrTma, E_GrDvrFsoUpgdBufSize ) )
			{
				Tv_Result	=	FALSE;
				break;
			}

			// scan
			for (Tv_WkIdx=0;Tv_WkIdx < Tv_WkCnt;Tv_WkIdx++)
			{
				if ( E_GrDvrRecHndlNone != Tv_PtrTma[Tv_WkIdx].HndlRec )
				{
					if ( LcIsCrntHndl(Tv_PtrTma[Tv_WkIdx].HndlRec) )
					{
						// TMA add
						if ( !LcThumbWrite( &Tv_PtrTma[Tv_WkIdx], Tv_TmPrc ) )
						{
							Tv_Result	=	FALSE;
							break;
						}
					}
					else
					{
						DbgMsgPrint("TMA Convert - Bad handle found!\n");
					}
				}
				// next
				Tv_TmPrc	=	Tv_TmPrc + E_GrTimeSecOfMin;
			}
				
			// next
			Tv_Dpos	=	Tv_Dpos + E_GrDvrFsoUpgdBufSize;
			Tv_ScanCnt	=	Tv_ScanCnt - Tv_WkCnt;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFsoDsk::LcDmdFindTime( Def_GrTime A_TimeDay )
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for (Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoDmdCnt;Tv_WkIdx++)
		{
			if ( A_TimeDay == m_Dmd[Tv_WkIdx] )
			{
				Tv_Result	=	Tv_WkIdx;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFsoDsk::LcDmdFindBegin( void )
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
		Def_GrTime	Tv_TmFound;
	// code --------------------
		Tv_Result		=	-1;
		Tv_TmFound	=	0;

		for (Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoDmdCnt;Tv_WkIdx++)
		{
			if ( 0 != m_Dmd[Tv_WkIdx] )
			{
				if ( 0 == Tv_TmFound )
				{
					Tv_TmFound	=	m_Dmd[Tv_WkIdx];
					Tv_Result		=	Tv_WkIdx;
				}
				else
				{
					if ( Tv_TmFound > m_Dmd[Tv_WkIdx] )
					{
						Tv_TmFound	=	m_Dmd[Tv_WkIdx];
						Tv_Result		=	Tv_WkIdx;
					}
					else if ( Tv_TmFound == m_Dmd[Tv_WkIdx] )
					{
						DbgMsgPrint( "Cls_GrDvrFsoDsk::LcDmdFindBegin - [BUG!] DMD ( %d ) time is overlapped!\n", Tv_WkIdx );
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFsoDsk::LcDmdFindEnd( void )
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
		Def_GrTime	Tv_TmFound;
	// code --------------------
		Tv_Result		=	-1;
		Tv_TmFound	=	0;

		for (Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoDmdCnt;Tv_WkIdx++)
		{
			if ( 0 != m_Dmd[Tv_WkIdx] )
			{
				if ( 0 == Tv_TmFound )
				{
					Tv_TmFound	=	m_Dmd[Tv_WkIdx];
					Tv_Result		=	Tv_WkIdx;
				}
				else
				{
					if ( Tv_TmFound < m_Dmd[Tv_WkIdx] )
					{
						Tv_TmFound	=	m_Dmd[Tv_WkIdx];
						Tv_Result		=	Tv_WkIdx;
					}
					else if ( Tv_TmFound == m_Dmd[Tv_WkIdx] )
					{
						DbgMsgPrint( "Cls_GrDvrFsoDsk::LcDmdFindEnd - [BUG!] DMD ( %d ) time is overlapped!\n", Tv_WkIdx );
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFsoDsk::LcDmdCreate( Def_GrTime A_TimeDay )
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
		__u32	Tv_MmdIdx;
		BOOL8	Tv_IsChg;
		BOOL8	Tv_IsEmpty;
		__u64	Tv_Dpos;
		Ptr_GrDvrRecMinThumb	Tv_PtrMmd;
	// code --------------------
		Tv_Result	=	-1;

		// init
		Tv_PtrMmd	=	(Ptr_GrDvrRecMinThumb)&m_PtrCfg->TmpBuf[E_GrDvrFsoBufOfsDmdCr];

		// find empty DMD
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoDmdCnt;Tv_WkIdx++)
		{
			if ( 0 == m_Dmd[Tv_WkIdx] )
			{
				// found empty
				Tv_Result	=	Tv_WkIdx;
				// erase MMD
				GrDumyZeroMem( Tv_PtrMmd, E_GrDvrFsoMmdDaySize );
				Tv_Dpos	=	E_GrDvrFsoDposMmd + (__u64)(Tv_WkIdx * E_GrDvrFsoMmdDaySize);
				if ( !LcPwrite( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
				{
					m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
				}
				break;
			}
		}

		if ( 0 > Tv_Result )
		{
			// find invalid day
			for (Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoDmdCnt;Tv_WkIdx++)
			{
				if ( 0 != m_Dmd[Tv_WkIdx] )
				{
					// read 
					Tv_Dpos	=	E_GrDvrFsoDposMmd + (__u64)(Tv_WkIdx * E_GrDvrFsoMmdDaySize);
					if ( LcPread( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
					{
						// check MMD
						Tv_IsChg		=	FALSE;
						Tv_IsEmpty	=	TRUE;
						for (Tv_MmdIdx=0;Tv_MmdIdx < E_GrTimeMinOfDay;Tv_MmdIdx++)
						{
							if ( E_GrDvrRecHndlNone != Tv_PtrMmd[Tv_MmdIdx].HndlRec )
							{
								if ( LcIsCrntHndl(Tv_PtrMmd[Tv_MmdIdx].HndlRec) )
								{
									Tv_IsEmpty	=	FALSE;
								}
								else
								{
									GrDumyZeroMem( &Tv_PtrMmd[Tv_MmdIdx], E_GrDvrRecMinThumbItemSize );
									Tv_IsChg		=	TRUE;
								}
							}
						}

						if ( Tv_IsChg )
						{
							// write
							if ( !LcPwrite( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
							{
								m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
								GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
							}
						}

						// check empty
						if ( Tv_IsEmpty )
						{
							// found
							Tv_Result	=	Tv_WkIdx;
							break;
						}
					}	//	if ( LcPread( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
					else
					{
						m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
						GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
					}
				}	//	if ( 0 != m_Dmd[Tv_WkIdx] )
			}	//	for (Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFsoDmdCnt;Tv_WkIdx++)
		}

		if ( 0 <= Tv_Result )
		{
			m_Dmd[Tv_Result]	=	A_TimeDay;
			Tv_Dpos	=	E_GrDvrFsoDposDmd + (__u64)(Tv_WkIdx * 4);
			// write DMD
			if ( !LcPwrite( Tv_Dpos, &m_Dmd[Tv_Result], 4 ) )
			{
				m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::LcThumbWrite( Ptr_GrDvrRecMinThumb A_PtrThumb, Def_GrTime A_Time )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_DmdIdx;
		Def_GrTime	Tv_TmDay;
		__u64	Tv_Dpos;
		__s32	Tv_MmdIdx;
	// code --------------------
		// init
		Tv_Result	=	FALSE;

		Tv_TmDay	=	A_Time - (A_Time % E_GrTimeSecOfDay);

		// check match DMD
		Tv_DmdIdx	=	LcDmdFindTime( Tv_TmDay );
		if ( 0 > Tv_DmdIdx )
		{
			// create new DMD
			Tv_DmdIdx	=	LcDmdCreate( Tv_TmDay );
		}

		// found
		if ( 0 <= Tv_DmdIdx )
		{
			// write MMD
			Tv_MmdIdx	=	(A_Time - Tv_TmDay) / E_GrTimeSecOfMin;
			Tv_Dpos	=	E_GrDvrFsoDposMmd + (__u64)(Tv_DmdIdx * E_GrDvrFsoMmdDaySize) + (__u64)(Tv_MmdIdx * E_GrDvrRecMinThumbItemSize);
			if ( LcPwrite( Tv_Dpos, A_PtrThumb, E_GrDvrRecMinThumbItemSize ) )
			{
				Tv_Result	=	TRUE;
			}
			else
			{
				m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoDsk::LcThumdRead( Ptr_GrDvrRecMinThumb A_PtrThumb, Def_GrTime A_Time )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_DmdIdx;
		Def_GrTime	Tv_TmDay;
		__u64	Tv_Dpos;
		__s32	Tv_MmdIdx;
	// code --------------------

		// init
		Tv_Result	=	FALSE;

		Tv_TmDay	=	A_Time - (A_Time % E_GrTimeSecOfDay);

		// check match DMD
		Tv_DmdIdx	=	LcDmdFindTime( Tv_TmDay );

		// found
		if ( 0 <= Tv_DmdIdx )
		{
			// write MMD
			Tv_MmdIdx	=	(A_Time - Tv_TmDay) / E_GrTimeSecOfMin;
			Tv_Dpos	=	E_GrDvrFsoDposMmd + (__u64)(Tv_DmdIdx * E_GrDvrFsoMmdDaySize) + (__u64)(Tv_MmdIdx * E_GrDvrRecMinThumbItemSize);
			if ( LcPread( Tv_Dpos, A_PtrThumb, E_GrDvrRecMinThumbItemSize ) )
			{
				// check current handle
				if ( (E_GrDvrRecHndlNone == A_PtrThumb->HndlRec) || (!LcIsCrntHndl(A_PtrThumb->HndlRec)) )
				{
					// reset
					GrDumyZeroMem( A_PtrThumb, E_GrDvrRecMinThumbItemSize );
				}
				Tv_Result	=	TRUE;
			}
			else
			{
				m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DskIdx );
			}
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFsoDsk::DayMapGet( Def_GrTime A_TimeDay, __u32 A_DayCnt )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_DayMask;
		__u32	Tv_WkIdx;
		__s32	Tv_DmdIdx;
		__s32	Tv_MmdIdx;
		Def_GrTime	Tv_TmDay;
		BOOL8	Tv_IsEmpty;
		BOOL8	Tv_IsChg;
		__u64	Tv_Dpos;
		Ptr_GrDvrRecMinThumb	Tv_PtrMmd;
	// code --------------------
		Tv_Result	=	0;

		if ( (E_GrDvrFsoDiskStatBad != m_Rca.DiskStat) && m_IsFormatted )
		{
			Tv_PtrMmd		=	(Ptr_GrDvrRecMinThumb)m_PtrCfg->TmpBuf;
			Tv_TmDay		=	A_TimeDay - (A_TimeDay % E_GrTimeSecOfDay);

			Tv_DayMask	=	1;
			for (Tv_WkIdx=0;Tv_WkIdx < A_DayCnt;Tv_WkIdx++)
			{
				// find DMD
				Tv_DmdIdx	=	LcDmdFindTime( Tv_TmDay );
				if ( 0 <= Tv_DmdIdx )
				{
					// read MMD
					Tv_Dpos	=	E_GrDvrFsoDposMmd + (__u64)(Tv_DmdIdx * E_GrDvrFsoMmdDaySize);
					if ( !LcPread( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
					{
						m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
						break;
					}

					// check exist
					Tv_IsEmpty	=	TRUE;
					Tv_IsChg		=	FALSE;
					for (Tv_MmdIdx=0;Tv_MmdIdx < E_GrTimeMinOfDay;Tv_MmdIdx++)
					{
						if ( E_GrDvrRecHndlNone != Tv_PtrMmd[Tv_MmdIdx].HndlRec )
						{
							// check valid handle
							if ( LcIsCrntHndl(Tv_PtrMmd[Tv_MmdIdx].HndlRec) )
							{
								Tv_IsEmpty	=	FALSE;
							}
							else
							{
								// change
								GrDumyZeroMem( &Tv_PtrMmd[Tv_MmdIdx], E_GrDvrRecMinThumbItemSize );
								Tv_IsChg	=	TRUE;
							}
						}
					}

					// check changed
					if ( Tv_IsChg )
					{
						if ( !LcPwrite( Tv_Dpos, Tv_PtrMmd, E_GrDvrFsoMmdDaySize ) )
						{
							m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
							break;
						}
					}

					// check empty
					if ( Tv_IsEmpty )
					{
						// reset DMD
						m_Dmd[Tv_DmdIdx]	=	0;
						Tv_Dpos	=	E_GrDvrFsoDposDmd + (__u64)(Tv_DmdIdx * 4);
						if ( !LcPwrite( Tv_Dpos, &m_Dmd[Tv_DmdIdx], 4) )
						{
							m_Rca.DiskStat	=	E_GrDvrFsoDiskStatBad;
							break;
						}
					}
					else
					{
						// update day mask
						Tv_Result	=	Tv_Result | Tv_DayMask;
					}
				}	//	if ( 0 <= Tv_DmdIdx )

				//next
				Tv_TmDay		=	Tv_TmDay + E_GrTimeSecOfDay;
				Tv_DayMask	=	Tv_DayMask << 1;
			}	//	for (Tv_WkIdx=0;Tv_WkIdx < A_DayCnt;Tv_WkIdx++)
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
