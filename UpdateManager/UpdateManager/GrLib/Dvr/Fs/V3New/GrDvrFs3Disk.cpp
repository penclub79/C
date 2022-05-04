/*
 GAUSE file system version 3 disk I/O

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
#include <Dvr/GrDvrBios.h>

#include <Dvr/Fs/V3/GrDvrFs3Base.h>
#include <Dvr/Fs/V3/GrDvrFs3Disk.h>

#include <GrTaskBase.h>
#include <GrTaskSvr.h>
#include <GrError.h>

#ifdef	LINUX_APP

#define LINUX_FILE_CTRL
#define _LARGEF64_SOURCE

#include <linux/fs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/hdreg.h>

#include <stdio.h>
#include <stdlib.h>

#else

#include <GrFileCtrl.h>

#endif

//====================================================================
//local const

#define E_GrFsV2DiskIoTmpBufSize			0x40000		/* 256 K byte */

#define E_GrFsV2DiskIoThumbBufCnt			60

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs3Disk::Cls_GrDvrFs3Disk( __u8 A_DevIdx, void* A_PtrTmpBuf, __u64 A_DvrId, __u32 A_ReqVsaSize )
{
	// local -------------------
		Def_StrGrFnPathShort	Tv_StrDev;
#ifndef LINUX_APP
		Def_StrGrFnPathShort	Tv_StrPath;
#endif
	// code --------------------
		//init
		m_DevIdx				=	A_DevIdx;
		m_DvrId					=	A_DvrId;
		m_IsOvwtAble		=	FALSE;
		m_PtrTmpBuf			=	A_PtrTmpBuf;
		m_GrpIntIdx			=	E_GrDvrFs3GrpIntNone;

		m_DskSize				=	0;
		m_RecTimeBegin	=	0;
		m_RecTimeEnd		=	0;

		m_DskStat				=	E_GrDvrRecStatNotexist;
		m_RecCap				=	0;
		m_RecSize				=	0;
		m_HndlLastRec		=	E_GrDvrRecHndlNone;

		m_ThumbTime			=	0;
		m_ThumbDmdIdx		=	-1;
		m_ThumbMmdIdx		=	0;

		m_BcsBlk				=	-1;
		GrDumyZeroMem( &m_BcsData, sizeof(m_BcsData) );

		// request VSA size
		m_ReqVsaSize		=	A_ReqVsaSize;
		m_ReqRdaPos			=	( (__u64)(E_GrDvrFs3PosVsa + A_ReqVsaSize) + E_GrDvrFs3DiskRdaPosAlgMask ) & (~E_GrDvrFs3DiskRdaPosAlgMask);

#ifdef LINUX_APP
		m_HndlDsk	=	-1;
#else
		m_HndlDsk	=	INVALID_HANDLE_VALUE;
#endif

		GrDumyZeroMem( &m_Mis, sizeof(m_Mis) );
		GrDumyZeroMem( &m_Rca, sizeof(m_Rca) );
		GrDumyZeroMem( m_Dmd, sizeof(m_Dmd) );
		GrDumyZeroMem( &m_ThumbData, sizeof(m_ThumbData) );

		// check structural
		if ( 512 != sizeof(St_GrDvrFs3SmartData) )
		{
			DbgMsgPrint( "Cls_GrDvrFs3Disk: Smart struct size ( %d ) mismatch. \n", sizeof(St_GrDvrFs3SmartData) );
		}

		// check BCS size
		if ( E_GrDvrFs3BcsUnitSize < sizeof(St_GrDvrFs3Bcs) )
		{
			DbgMsgPrint( "Cls_GrDvrFs3Disk: BCS struct size ( %d) mismatch.\n" , sizeof(St_GrDvrFs3Bcs) );
		}

		if ( GrDvrBiosRecDevNameGet( m_DevIdx, Tv_StrDev ) )
		{
#ifdef LINUX_APP
			m_HndlDsk	=	open( Tv_StrDev, O_RDWR  );
			if ( 0 <= m_HndlDsk )
			{
				DbgMsgPrint( "Cls_GrDvrFs3Disk: device ( %d ) opened. \n", m_DevIdx );
				// get disk identify
				if ( 0 == ioctl( m_HndlDsk, BLKGETSIZE64, &m_DskSize ) )
				{
					m_DskStat		=	E_GrDvrRecStatUnformated;
				}
				else
				{
					//size get failure, stop run
					close( m_HndlDsk );
					m_HndlDsk	=	-1;
					DbgMsgPrint( "Cls_GrDvrFs3Disk: device ( %d ) get size error. \n", m_DevIdx );
				}
			}
#else
			GrStrCopy( Tv_StrPath, GrFileCtrlEmuPathGetA() );
			GrStrCat( Tv_StrPath, Tv_StrDev );
			m_HndlDsk	=	CreateFileA( Tv_StrPath, GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
			if ( INVALID_HANDLE_VALUE != m_HndlDsk )
			{
				if ( GetFileSizeEx( m_HndlDsk, (PLARGE_INTEGER)(&m_DskSize) ) )
				{
					m_DskStat		=	E_GrDvrRecStatUnformated;
				}
				else
				{
					CloseHandle( m_HndlDsk );
					m_HndlDsk	=	INVALID_HANDLE_VALUE;				
				}
			}
#endif
		}
		
		//initialize
		if ( E_GrDvrRecStatUnformated == m_DskStat )
		{
			// check disk size
			if ( (E_GrDvrFs3DiskMinRdaSize + m_ReqRdaPos) <= m_DskSize )
			{
				// disk status update
				LcInit();
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
			}
		}
		
}
//--------------------------------------------------------------------
Cls_GrDvrFs3Disk::~Cls_GrDvrFs3Disk()
{
	// local -------------------
	// code --------------------
#ifdef LINUX_APP
		if ( 0 <= m_HndlDsk )
		{
			//flush all
			close( m_HndlDsk );
		}
#else
		if ( INVALID_HANDLE_VALUE != m_HndlDsk )
		{
			//flush all
			CloseHandle( m_HndlDsk );
		}
#endif

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcSeek( __s64 A_Pos, BOOL8 A_IsBegin )
{
	// local -------------------
		BOOL8	Tv_Result;
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
			/*
			else
			{
				//check hdd status
				if ( !LcSmartRead() )
				{
					m_Rca.DiskStat	=	E_GrDvrFs1DiskStatBad;
				}
			}
			*/
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
BOOL8	Cls_GrDvrFs3Disk::LcRead( void* A_PtrBuf, __u32 A_Size )
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
		//check opened
		if ( -1 != m_HndlDsk )
		{
			if ( A_Size == read( m_HndlDsk, A_PtrBuf, A_Size ) )
			{
				Tv_Result	=	TRUE;
			}
			/*
			else
			{
				//check HDD status
				if ( !LcSmartRead() )
				{
					m_Rca.DiskStat	=	E_GrDvrFs1DiskStatBad;
				}
			}
			*/
		}
#else
		//check opened
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
BOOL8	Cls_GrDvrFs3Disk::LcWrite( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
#ifndef LINUX_APP
		__u32		Tv_Writed;
#endif
	// code --------------------
		//init
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
		//check opened
		if ( -1 != m_HndlDsk )
		{
			if ( A_Size == write( m_HndlDsk, A_PtrBuf, A_Size ) )
			{
				Tv_Result	=	TRUE;
			}
			/*
			else
			{
				//check HDD status
				if ( !LcSmartRead() )
				{
					m_Rca.DiskStat	=	E_GrDvrFs1DiskStatBad;
				}
			}
			*/
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

		return		Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::LcFlush( void )
{
#ifdef LINUX_APP
		if ( -1 != m_HndlDsk )
		{
			fsync( m_HndlDsk );
		}
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::Format( BOOL8 A_IsWithLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsBkElaOvwt;
		BOOL8	Tv_IsFormated;
		BOOL8	Tv_IsWithLog;
		__u32	Tv_BkElaIn;
	// code --------------------
		Tv_Result	=	TRUE;
		Tv_IsFormated	=	FALSE;
		Tv_IsWithLog	=	TRUE;
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			Tv_IsFormated	=	TRUE;
			Tv_IsWithLog	=	A_IsWithLog;
		}

		// check disk status
		if ( E_GrDvrRecStatBad == m_DskStat )
		{
			Tv_Result	=	FALSE;
		}
		
		// check disk size
		if ( ((E_GrDvrFs3DiskMinRdaSize + m_ReqRdaPos) > m_DskSize) )
		{
			Tv_Result	=	FALSE;
			m_DskStat	=	E_GrDvrRecStatBad;
		}
		
		// MIS
		if ( Tv_Result )
		{
			// init status
			m_DskStat	=	E_GrDvrRecStatUnformated;
			//write MIS
			m_Mis.FsFcc			=	E_GrDvrFs3Fcc;
			m_Mis.FsVer			=	E_GrDvrFs3Ver;
			m_Mis.StrmFcc		=	E_GrDvrFs3StrmFcc;
			m_Mis.ElaSize		=	E_GrDvrFs3ElaDfltSize;
			m_Mis.VsaSize		=	m_ReqVsaSize;
			m_Mis.RdaPos		=	m_ReqRdaPos;
			if ( (E_GrDvrFs3DiskMaxRdaSize + m_ReqRdaPos) <= m_DskSize  )
			{
				m_Mis.BlkCnt		=	(__u32)(E_GrDvrFs3DiskMaxRdaSize >> E_GrDvrFs3BlockShift);
			}
			else
			{
				m_Mis.BlkCnt		=	(__u32)((m_DskSize - m_Mis.RdaPos) >> E_GrDvrFs3BlockShift);
			}
			
			if ( LcSeek( E_GrDvrFs3PosMis ) )
			{
				Tv_Result	=	LcWrite( &m_Mis, sizeof(m_Mis) );
			}
			else
			{
				Tv_Result	=	FALSE;
			}
		}
		
		// RCA
		if ( Tv_Result )
		{
			if ( !Tv_IsWithLog )
			{
				Tv_BkElaIn			=	m_Rca.EvtInIdx;
				Tv_IsBkElaOvwt	=	m_Rca.IsEvtOvwt;
			}
			GrDumyZeroMem( &m_Rca, sizeof(m_Rca) );
			if ( !Tv_IsWithLog )
			{
				m_Rca.EvtInIdx	=	Tv_BkElaIn;
				m_Rca.IsEvtOvwt	=	Tv_IsBkElaOvwt;
			}
			// update DVR id
			m_Rca.DvrId	=	m_DvrId;

			// write
			if ( LcSeek( E_GrDvrFs3PosRca ) )
			{
				Tv_Result	=	LcWrite( &m_Rca, sizeof(m_Rca) );
			}
			else
			{
				Tv_Result	=	FALSE;
			}
			
		}

		// DMD
		if ( Tv_Result )
		{
			GrDumyZeroMem( m_Dmd, sizeof(m_Dmd) );
			LcDmdWrite();
			if ( E_GrDvrRecStatBad == m_DskStat )
			{
				Tv_Result	=	FALSE;
			}
		}
		
		// ELA
		if ( Tv_Result )
		{
			if ( Tv_IsWithLog )
			{
				Tv_Result	=	LcFillZero( E_GrDvrFs3PosEla, E_GrDvrFs3ElaDfltSize );
			}
			else
			{
				LcElaDisable();
			}
		}

		// BDT
		if ( Tv_Result )
		{
			GrDumyZeroMem( m_Bdt, sizeof(m_Bdt) );
			Tv_Result	=	LcFillZero( E_GrDvrFs3PosBdt, sizeof(m_Bdt) );
		}

		// BCS
		if ( Tv_Result )
		{
			GrDumyZeroMem( &m_BcsData, sizeof(m_BcsData) );
			Tv_Result	=	LcFillZero( E_GrDvrFs3PosBcs, E_GrDvrFs3BcsAreaSize );
		}

		// init
		m_HndlLastRec		=	E_GrDvrRecHndlNone;
		m_ThumbTime			=	0;
		m_ThumbDmdIdx		=	-1;
		m_ThumbMmdIdx		=	0;
		m_RecTimeBegin	=	0;
		m_RecTimeEnd		=	0;

		m_BcsBlk				=	-1;

		if ( Tv_Result )
		{
			m_DskStat	=	E_GrDvrRecStatEmpty;
			LcNormalStatUpdt();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::LcInit( void )
{
	// local -------------------
		BOOL8	Tv_IsAble;
		__u64	Tv_DskSize;
	// code --------------------
		m_DskStat	=	E_GrDvrRecStatUnformated;
		Tv_IsAble	=	TRUE;

		// load mis
		if ( Tv_IsAble )
		{
			// load
			Tv_IsAble	=	FALSE;
			if ( LcPread( E_GrDvrFs3PosMis, &m_Mis, sizeof(m_Mis) ) )
			{
				if ( (E_GrDvrFs3Fcc == m_Mis.FsFcc) && (E_GrDvrFs3Ver == m_Mis.FsVer) && (E_GrDvrFs3StrmFcc == m_Mis.StrmFcc) )
				{
					// check event log size
					if ( (E_GrDvrFs3ElaMinSize <= m_Mis.ElaSize) && (E_GrDvrFs3ElaMaxSize >= m_Mis.ElaSize) )
					{
						// check event log size fix
						if ( 0 != ( (~E_GrDvrLogAlignMask) & m_Mis.ElaSize ) )
						{
							DbgMsgPrint( "Cls_GrDvrFs3Disk::LcInit - ELA size align bad. do fix... \n" );
							m_Mis.ElaSize	=	m_Mis.ElaSize & E_GrDvrLogAlignMask;
						}
						
						// check RDA size
						if ( (2 <= m_Mis.BlkCnt) && (E_GrDvrFs3BdtMaxCnt >= m_Mis.BlkCnt) )
						{
							Tv_DskSize	=	m_Mis.RdaPos + ((__u64)m_Mis.BlkCnt * E_GrDvrFs3BlockSize);
							if ( Tv_DskSize <= m_DskSize )
							{
								Tv_IsAble	=	TRUE;
							}
							else
							{
								DbgMsgPrint( "Cls_GrDvrFs3Disk::LcInit - record area bigger than disksize.\n" );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrDvrFs3Disk::LcInit - BDT block count ( %d ) is bad.\n", m_Mis.BlkCnt );
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrDvrFs3Disk::LcInit - ELA bad.\n" );
					}
				}
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
			}
		}

		// load RCA
		if ( Tv_IsAble )
		{
			// load
			Tv_IsAble	=	FALSE;
			if ( LcPread( E_GrDvrFs3PosRca,  &m_Rca, sizeof(m_Rca) ) )
			{
				// check DVR id
				if ( m_Rca.DvrId != m_DvrId )
				{
					// m_DskStat	=	E_GrDvrRecStatDefective;
					DbgMsgPrint( "Cls_GrDvrFs3Disk::LcInit - disk %d invalid DVR id.\n", m_DevIdx );
				}
				// event log position
				if ( m_Mis.ElaSize > (m_Rca.EvtInIdx << E_GrDvrLogShiftSize) )
				{
					// success
					Tv_IsAble	=	TRUE;
				}
				else
				{
					DbgMsgPrint( "Cls_GrDvrFs3Disk::LcInit - ELA item pointer bad.\n" );
				}
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
			}
		}

		// load DMD
		if ( Tv_IsAble )
		{
			// load
			Tv_IsAble	=	FALSE;
			if ( LcPread( E_GrDvrFs3PosDmd, m_Dmd, sizeof(m_Dmd) ) )
			{
				Tv_IsAble	=	TRUE;
				// check DMD time
				LcDmdCheck();
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
			}
		}

		// check BDT
		if ( Tv_IsAble )
		{
			// load
			Tv_IsAble	=	FALSE;
			if ( LcPread( E_GrDvrFs3PosBdt, m_Bdt, sizeof(m_Bdt) ) )
			{
				Tv_IsAble	=	TRUE;
				if ( !LcBdtChkIntra() )
				{
					DbgMsgPrint( "Cls_GrDvrFs3Disk::LcInit - disk %d record id overlapped.\n", m_DevIdx );
					m_DskStat	=	E_GrDvrRecStatDefective;
				}
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
			}
		}

		// update other info
		if ( Tv_IsAble && (E_GrDvrRecStatBad != m_DskStat) && (E_GrDvrRecStatDefective != m_DskStat) )
		{
			// set empty status
			m_DskStat	=	E_GrDvrRecStatEmpty;
			// update status
			LcNormalStatUpdt();
			// time range update
			LcTimeRngUpdt();
		}

		// other init
		m_BcsBlk		=	-1;
		
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs3Disk::RecLastHndlGet( void )
{
	// local -------------------
	// code --------------------
		return	m_HndlLastRec  | (__u64)m_GrpIntIdx;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::IsValidHndl( __u64 A_Hndl )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_OvwtId;
		__u32	Tv_BlkId;
		__u64	Tv_RdaPos;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_RdaPos	=	A_Hndl & E_GrDvrFs3HndlRdaPosMask;
		Tv_OvwtId		=	(__u16)(A_Hndl >> E_GrDvrFs3HndlOvwtIdPos);
		// get block id
		Tv_BlkId		=	(__u32)(Tv_RdaPos >> E_GrDvrFs3BlockShift);
		// check bad block
		if ( Tv_BlkId < m_Mis.BlkCnt )
		{
			if ( !m_Bdt[Tv_BlkId].IsBad )
			{
				if ( Tv_OvwtId == m_Bdt[Tv_BlkId].OvwtId )
				{
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcThumbWrite( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Pos;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( 0 <= m_ThumbDmdIdx )
		{
			// write MMD
			// calculate position
			Tv_Pos	=	E_GrDvrFs3PosMmd + (__u64)( m_ThumbDmdIdx * E_GrDvrFs3MmdDaySize ) + (__u64)( m_ThumbMmdIdx * sizeof(St_GrDvrRecMinThumb) );
			if ( LcSeek( Tv_Pos ) )
			{
				if ( LcWrite( &m_ThumbData, sizeof(m_ThumbData) ) )
				{
					Tv_Result	=	TRUE;
				}
				else
				{
					m_DskStat	=	E_GrDvrRecStatBad;
				}
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcThumbUpdt( Ptr_GrDvrFs3BlkAddInfo A_PtrInfo )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsTmChg;
		BOOL8	Tv_IsNewDay;
		BOOL8	Tv_IsDtChg;
		__u64	Tv_Pos;
		__u8	Tv_ThumbEvt;
		__u8	Tv_WkIdx;
		__u8	Tv_WkCh;
		__u8	Tv_Tmp;
		Def_GrTime	Tv_ThumbDay;
	// code --------------------
		Tv_Result			=	FALSE;
		Tv_IsDtChg		=	FALSE;

		// check time
		Tv_IsTmChg		=	TRUE;
		if ( 0 != m_ThumbTime )
		{
			if ( (m_ThumbTime <= A_PtrInfo->TimeSt) && ((m_ThumbTime + E_GrTimeSecOfMin) > A_PtrInfo->TimeSt) )
			{
				Tv_IsTmChg	=	FALSE;
			}
		}

		// reload thumb
		if ( Tv_IsTmChg && (E_GrDvrRecStatBad != m_DskStat) )
		{
			// check need allocate
			Tv_IsNewDay	=	TRUE;
			if ( 0 != m_ThumbTime )
			{
				// check day change
				if ( (m_Dmd[m_ThumbDmdIdx] <= A_PtrInfo->TimeSt) && ( (m_Dmd[m_ThumbDmdIdx] + E_GrTimeSecOfDay) > A_PtrInfo->TimeSt )  )
				{
					Tv_IsNewDay	=	FALSE;
				}
			}
			
			// new day
			if ( Tv_IsNewDay )
			{
				// find current day
				Tv_ThumbDay		=	A_PtrInfo->TimeSt - (A_PtrInfo->TimeSt % E_GrTimeSecOfDay);
				m_ThumbDmdIdx	=	LcDmdFind( Tv_ThumbDay );
				if ( 0 > m_ThumbDmdIdx )
				{
					// create new DMD
					m_ThumbDmdIdx	=	LcDmdAlloc();
				}
				if ( (0 <= m_ThumbDmdIdx) && (E_GrDvrRecStatBad != m_DskStat) )
				{
					// DMD update
					m_Dmd[m_ThumbDmdIdx]	=	Tv_ThumbDay;
					LcDmdWrite();
				}
			}
			// check new minute
			if ( (0 <= m_ThumbDmdIdx) && (E_GrDvrRecStatBad != m_DskStat) )
			{
				m_ThumbTime		=	A_PtrInfo->TimeSt - (A_PtrInfo->TimeSt % E_GrTimeSecOfMin);
				m_ThumbMmdIdx	=	(m_ThumbTime - m_Dmd[m_ThumbDmdIdx]) / E_GrTimeSecOfMin;
				// load data
				Tv_Pos	=	E_GrDvrFs3PosMmd + (__u64)( m_ThumbDmdIdx * E_GrDvrFs3MmdDaySize ) + (__u64)( m_ThumbMmdIdx * sizeof(St_GrDvrRecMinThumb) );
				if ( LcPread( Tv_Pos, &m_ThumbData, sizeof(m_ThumbData) ) )
				{
					// check handle
					if ( !IsValidHndl( m_ThumbData.HndlRec ) )
					{
						// reset data
						GrDumyZeroMem( &m_ThumbData, sizeof(m_ThumbData) );
						Tv_IsDtChg	=	TRUE;
					}
				}
				else
				{
					// notify disk bad
					m_DskStat	=	E_GrDvrRecStatBad;
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				}
			}
		}

		// update data
		if ( ( 0 <= m_ThumbDmdIdx) && (E_GrDvrRecStatBad != m_DskStat) )
		{
			// check handle
			if ( E_GrDvrRecHndlNone == m_ThumbData.HndlRec )
			{
				m_ThumbData.HndlRec	=	m_HndlLastRec;
				Tv_IsDtChg	=	TRUE;
			}

			Tv_WkCh	=	0;
			for ( Tv_WkIdx=0;Tv_WkIdx< (E_GrDvrMaxChCnt >> 1);Tv_WkIdx++ )
			{
				Tv_ThumbEvt	=	GrDvrRecAtbToThumb( A_PtrInfo->AtbTbl[Tv_WkCh] );
				Tv_Tmp			=	GrDvrRecAtbToThumb( A_PtrInfo->AtbTbl[Tv_WkCh + 1] );
				Tv_ThumbEvt	=	Tv_ThumbEvt | (Tv_Tmp << 4);
				Tv_ThumbEvt	=	Tv_ThumbEvt | m_ThumbData.ChAtb[Tv_WkIdx]; 
				// check change
				if ( Tv_ThumbEvt != m_ThumbData.ChAtb[Tv_WkIdx] )
				{
					m_ThumbData.ChAtb[Tv_WkIdx]	=	Tv_ThumbEvt;
					Tv_IsDtChg	=	TRUE;
				}
				// next
				Tv_WkCh			=	Tv_WkCh + 2;
			}

			// update
			Tv_Result	=	Tv_IsTmChg;
		}

		// write data
		if ( Tv_IsDtChg && (0 <= m_ThumbDmdIdx) && (E_GrDvrRecStatBad != m_DskStat) )
		{
			LcThumbWrite();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::OverwriteSet( BOOL8 A_IsOvwt )
{
		m_IsOvwtAble	=	A_IsOvwt;
		// update status
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			// status update
			LcNormalStatUpdt();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs3Disk::DevIdxGet( void )
{
		return	m_DevIdx;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::PacketReplace( __u64 A_Hndl, Ptr_GrDvrAvHd A_PtrPkt )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsBad;
		__u32	Tv_BlkId;
		__u64	Tv_RdaPos;
		__u64	Tv_DskPos;
		St_GrDvrAvHd	Tv_AvHd;
	// code --------------------
		Tv_Result	=	FALSE;

		// check disk status
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			// check handle range
			if ( IsValidHndl( A_Hndl ) )
			{
				// load header
				Tv_RdaPos			=	A_Hndl & E_GrDvrFs3HndlRdaPosMask;
				Tv_DskPos			=	Tv_RdaPos + m_Mis.RdaPos;
				Tv_IsBad	=	TRUE;
				if ( LcSeek( Tv_DskPos ) )
				{
					if ( LcRead( &Tv_AvHd, sizeof(Tv_AvHd) ) )
					{
						Tv_IsBad	=	FALSE;

						// check header and reserved size
						if ( E_GrDvrFs3StrmFcc == Tv_AvHd.Fcc )
						{
							// check size
							if ( Tv_AvHd.DataSize == A_PtrPkt->DataSize )
							{
								// write
								Tv_IsBad	=	TRUE;
								Tv_DskPos	=	Tv_DskPos + (__u64)(sizeof(St_GrDvrAvHd));
								if ( LcSeek( Tv_DskPos ) )
								{
									if ( LcWrite( A_PtrPkt, A_PtrPkt->DataSize ) )
									{
										// success
										Tv_IsBad	=	FALSE;
										Tv_Result	=	TRUE;
									}
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrDvrFs3Disk::PacketReplace - reserved size mismatch.\n" );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrDvrFs3Disk::PacketReplace - bad reserved packet.\n" );
						}
					}
				}
				// check bad appear
				if ( Tv_IsBad )
				{
					// bad block appear
					Tv_BlkId	=	(__u32)( (Tv_DskPos - m_Mis.RdaPos) >> E_GrDvrFs3BlockShift );
					m_Bdt[Tv_BlkId].IsBad	=	TRUE;
					LcBdtWriteByBlkId( Tv_BlkId );
					GrTaskSvrMsgErrorNotify( E_GrErrDiskBadSector, m_DevIdx );
					DbgMsgPrint( "Cls_GrDvrFs3Disk::PacketReplace - bad block (%d:%d) appear.\n", m_DevIdx, Tv_BlkId );
				}

			}	//	if ( IsValidHndl( A_Hndl ) )
			else
			{
				DbgMsgPrint( "Cls_GrDvrFs3Disk::PacketReplace - bad handle value.\n" );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs3Disk::DayMapGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_DmdIdx;
		__u32	Tv_MmdIdx;
		__u32	Tv_Pos;
		Ptr_GrDvrRecMinThumb	Tv_PtrSr;
		BOOL8	Tv_IsDmdDirt;
	// code --------------------
		Tv_Result		=	0;
		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			// init
			Tv_IsDmdDirt	=	FALSE;
			Tv_PtrSr		=	(Ptr_GrDvrRecMinThumb)( (__u32)m_PtrTmpBuf + E_GrDvrFs3DiskMmdCmbBufPos );
			for ( Tv_DmdIdx=0;Tv_DmdIdx < E_GrDvrFs3DmdItemCnt;Tv_DmdIdx++ )
			{
				// check exist day
				if ( (m_Dmd[Tv_DmdIdx] >= A_TimeSt) && (m_Dmd[Tv_DmdIdx] <= A_TimeEd) )
				{
					// check validity
					if ( LcMmdLoadByDmdIdx( (__u32)Tv_DmdIdx, Tv_PtrSr ) )
					{
						for ( Tv_MmdIdx=0;Tv_MmdIdx < E_GrDvrFs3MmdDayItemCnt;Tv_MmdIdx++ )
						{
							// check exist
							if ( E_GrDvrRecHndlNone != Tv_PtrSr[Tv_MmdIdx].HndlRec )
							{
								Tv_Pos		=	(m_Dmd[Tv_DmdIdx] - A_TimeSt) / E_GrTimeSecOfDay;
								Tv_Result	=	Tv_Result | (1 << Tv_Pos);
								break;
							}
						}
					}
					else
					{
						m_Dmd[Tv_DmdIdx]	=	0;
						Tv_IsDmdDirt			=	TRUE;
					}
				}
			}

			// update DMD
			if ( Tv_IsDmdDirt )
			{
				LcDmdWrite();
			}
			
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::MinMapCombine( Def_GrTime A_TimeSt, void* A_PtrBuf, __u32 A_Cnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_GrTime	Tv_TimeDay;
		__s32		Tv_DmdIdx;
		PBYTE	Tv_PtrTg;
		Ptr_GrDvrRecMinThumb	Tv_PtrSr;
		__u32	Tv_SrIdx;
		__u32	Tv_Cnt;
		__u32	Tv_WkIdx;

		PBYTE	Tv_PtrSrAtb;
		PBYTE	Tv_PtrTgAtb;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_TimeDay	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfDay);
		Tv_PtrSr		=	(Ptr_GrDvrRecMinThumb)( (__u32)m_PtrTmpBuf + E_GrDvrFs3DiskMmdCmbBufPos );

		// find DMD index
		Tv_DmdIdx		=	LcDmdFind( Tv_TimeDay );
		if ( 0 <= Tv_DmdIdx )
		{
			// load MMD to temporary buffer
			if ( LcMmdLoadByDmdIdx( (__u32)Tv_DmdIdx, Tv_PtrSr ) )
			{
				// calculate combine range
				Tv_SrIdx	=	(A_TimeSt - Tv_TimeDay) / E_GrTimeSecOfMin;
				Tv_Cnt		=	E_GrDvrFs3MmdDayItemCnt - Tv_SrIdx;
				if ( Tv_Cnt > A_Cnt )
				{
					Tv_Cnt	=	A_Cnt;
				}
				// combine
				Tv_PtrTg	=	(PBYTE)A_PtrBuf;
				for ( Tv_WkIdx=0;Tv_WkIdx < Tv_Cnt;Tv_WkIdx++ )
				{
					// check combine
					if ( E_GrDvrRecHndlNone != Tv_PtrSr[Tv_SrIdx].HndlRec )
					{
						Tv_PtrTgAtb = (PBYTE)Tv_PtrTg;
						Tv_PtrSrAtb	= (PBYTE)Tv_PtrSr[Tv_SrIdx].ChAtb;
						for(__u8 Tv_WkCnt = 0;Tv_WkCnt < E_GrDvrMaxChCnt;Tv_WkCnt++)
						{
							if(0 != (Tv_WkCnt & 1))
							{
								Tv_PtrTgAtb[Tv_WkCnt] = Tv_PtrSrAtb[Tv_WkCnt >> 1] >> 4;
							}
							else
							{
								Tv_PtrTgAtb[Tv_WkCnt] = Tv_PtrSrAtb[Tv_WkCnt >> 1] & 0x0F;
							}
						}
					}
					// next
					Tv_SrIdx ++;
					Tv_PtrTg += E_GrDvrMaxChCnt;
				}
				// success
				Tv_Result	=	TRUE;
			}
			else
			{
				// garbage remove
				m_Dmd[Tv_DmdIdx]	=	0;
				LcDmdWrite();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs3Disk::GrpIntIdxGet( void )
{
		return	m_GrpIntIdx;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::GrpIntIdxSet( __u8 A_Idx )
{
		m_GrpIntIdx	=	A_Idx;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcFillZero( __u64 A_Pos, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Remain;
		__u32	Tv_WtSize;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( LcSeek( A_Pos ) )
		{
			// clear temporary buffer
			Tv_WtSize	=	E_GrDvrFs3TmpBufSize;
			if ( E_GrDvrFs3TmpBufSize > A_Size )
			{
				Tv_WtSize	=	A_Size;
			}
			GrDumyZeroMem( m_PtrTmpBuf, Tv_WtSize );
			Tv_Remain	=	A_Size;
			while ( 0 != Tv_Remain )
			{
				// get write size
				Tv_WtSize	=	Tv_Remain;
				if ( E_GrDvrFs3TmpBufSize < Tv_WtSize )
				{
					Tv_WtSize	=	E_GrDvrFs3TmpBufSize;
				}
				// write
				if ( !LcWrite( m_PtrTmpBuf, Tv_WtSize ) )
				{
					break;
				}
				// next
				Tv_Remain	=	Tv_Remain - Tv_WtSize;
			}
			// check success
			if ( 0 == Tv_Remain )
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcRcaUpdt( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( LcPwrite( E_GrDvrFs3PosRca, &m_Rca, sizeof(m_Rca) ) )
		{
			Tv_Result	=	TRUE;
		}
		else
		{
			m_DskStat	=	E_GrDvrRecStatBad;
			DbgMsgPrint( "Cls_GrDvrFs3Disk::LcRcaUpdt - RCA area bad!\n" );
			GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcMmdLoadByDmdIdx( __u32 A_DmdIdx, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Pos;
		__u32	Tv_WkIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrThumb;
		BOOL8	Tv_IsModified;
		BOOL8	Tv_IsBad;
	// code --------------------
		Tv_Result	=	FALSE;
		// check DMD index
		if ( E_GrDvrFs3DmdItemCnt > A_DmdIdx )
		{
			// init
			Tv_IsBad	=	TRUE;
			Tv_Pos	=	(__u64)( E_GrDvrFs3PosMmd + ( A_DmdIdx * E_GrDvrFs3MmdDaySize ) );
			// load 
			if ( LcSeek( Tv_Pos ) )
			{
				if ( LcRead( A_PtrBuf, E_GrDvrFs3MmdDaySize ) )
				{
					Tv_IsBad	=	FALSE;
				}
			}
			// check read success
			if ( !Tv_IsBad )
			{
				// check current handle
				Tv_IsModified	=	FALSE;
				Tv_PtrThumb	=	(Ptr_GrDvrRecMinThumb)A_PtrBuf;
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs3MmdDayItemCnt;Tv_WkIdx++ )
				{
					if ( E_GrDvrRecHndlNone != Tv_PtrThumb->HndlRec )
					{
						if ( IsValidHndl( Tv_PtrThumb->HndlRec ) )
						{
							Tv_Result	=	TRUE;
						}
						else
						{
							Tv_PtrThumb->HndlRec	=	E_GrDvrRecHndlNone;
							Tv_PtrThumb->ChAtb[0]	=	0;
							Tv_PtrThumb->ChAtb[1]	=	0;
							Tv_PtrThumb->ChAtb[2]	=	0;
							Tv_PtrThumb->ChAtb[3]	=	0;
							Tv_PtrThumb->ChAtb[4]	=	0;
							Tv_PtrThumb->ChAtb[5]	=	0;
							Tv_PtrThumb->ChAtb[6]	=	0;
							Tv_PtrThumb->ChAtb[7]	=	0;
							Tv_IsModified	=	TRUE;
						}
					}
					// next
					Tv_PtrThumb ++;
				}
				// check modified
				if ( Tv_IsModified )
				{
					Tv_IsBad	=	TRUE;
					if ( LcSeek( Tv_Pos ) )
					{
						if ( LcWrite( A_PtrBuf, E_GrDvrFs3MmdDaySize ) )
						{
							Tv_IsBad	=	FALSE;
						}
					}
				}

			}	//	if ( !Tv_IsBad )

			if ( Tv_IsBad )
			{
				if ( E_GrDvrRecStatBad != m_DskStat )
				{
					// disk error
					m_DskStat	=	E_GrDvrRecStatBad;
					// notify disk bad
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				}
			}
			
		}	//	if ( E_GrDvrFs3DmdItemCnt > A_DmdIdx )

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::LcDmdWrite( void )
{
	// local -------------------
		BOOL8	Tv_IsOk;
	// code --------------------
		Tv_IsOk	=	FALSE;

		if ( LcSeek( E_GrDvrFs3PosDmd ) )
		{
			if ( LcWrite( m_Dmd, sizeof(m_Dmd) ) )
			{
				Tv_IsOk	=	TRUE;
			}
		}

		if ( (!Tv_IsOk) && (E_GrDvrRecStatBad != m_DskStat) )
		{
			// disk error
			m_DskStat	=	E_GrDvrRecStatBad;
			// notify disk bad
			GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
		}
		
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFs3Disk::LcMmdFindOldestThumb( __u32 A_DmdIdx, Ptr_GrDvrRecMinThumb A_PtrRtThumb )
{
	// local -------------------
		__s32	Tv_Result;
		__u64	Tv_Pos;
		__u32	Tv_WkIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrThumb;
		BOOL8	Tv_IsModified;
		BOOL8	Tv_IsBad;
	// code --------------------
		Tv_Result	=	-1;
		// check DMD index
		if ( E_GrDvrFs3DmdItemCnt > A_DmdIdx )
		{
			// init
			Tv_IsBad	=	TRUE;
			Tv_Pos	=	(__u64)( E_GrDvrFs3PosMmd + ( A_DmdIdx * E_GrDvrFs3MmdDaySize ) );
			// load 
			if ( LcSeek( Tv_Pos ) )
			{
				if ( LcRead( m_PtrTmpBuf, E_GrDvrFs3MmdDaySize ) )
				{
					Tv_IsBad	=	FALSE;
				}
			}
			// check read success
			if ( !Tv_IsBad )
			{
				// check current handle and time
				Tv_PtrThumb	=	(Ptr_GrDvrRecMinThumb)m_PtrTmpBuf;
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs3MmdDayItemCnt;Tv_WkIdx++ )
				{
					if ( E_GrDvrRecHndlNone != Tv_PtrThumb->HndlRec )
					{
						if ( IsValidHndl( Tv_PtrThumb->HndlRec ) )
						{
							if ( -1 == Tv_Result )
							{
								// update value
								Tv_Result	=	(__s32)Tv_WkIdx;

								A_PtrRtThumb->HndlRec		=	Tv_PtrThumb->HndlRec;
								A_PtrRtThumb->ChAtb[0]	=	Tv_PtrThumb->ChAtb[0];
								A_PtrRtThumb->ChAtb[1]	=	Tv_PtrThumb->ChAtb[1];
								A_PtrRtThumb->ChAtb[2]	=	Tv_PtrThumb->ChAtb[2];
								A_PtrRtThumb->ChAtb[3]	=	Tv_PtrThumb->ChAtb[3];
								A_PtrRtThumb->ChAtb[4]	=	Tv_PtrThumb->ChAtb[4];
								A_PtrRtThumb->ChAtb[5]	=	Tv_PtrThumb->ChAtb[5];
								A_PtrRtThumb->ChAtb[6]	=	Tv_PtrThumb->ChAtb[6];
								A_PtrRtThumb->ChAtb[7]	=	Tv_PtrThumb->ChAtb[7];
							}
						}
						else
						{
							// process bad thumb
							Tv_PtrThumb->HndlRec	=	E_GrDvrRecHndlNone;
							Tv_PtrThumb->ChAtb[0]	=	0;
							Tv_PtrThumb->ChAtb[1]	=	0;
							Tv_PtrThumb->ChAtb[2]	=	0;
							Tv_PtrThumb->ChAtb[3]	=	0;
							Tv_PtrThumb->ChAtb[4]	=	0;
							Tv_PtrThumb->ChAtb[5]	=	0;
							Tv_PtrThumb->ChAtb[6]	=	0;
							Tv_PtrThumb->ChAtb[7]	=	0;
							Tv_IsModified	=	TRUE;
						}
					}
					// next
					Tv_PtrThumb ++;
				}
				// check modified
				if ( Tv_IsModified )
				{
					Tv_IsBad	=	TRUE;
					if ( LcSeek( Tv_Pos ) )
					{
						if ( LcWrite( m_PtrTmpBuf, E_GrDvrFs3MmdDaySize ) )
						{
							Tv_IsBad	=	FALSE;
						}
					}
				}

			}	//	if ( !Tv_IsBad )

			if ( Tv_IsBad )
			{
				if ( E_GrDvrRecStatBad != m_DskStat )
				{
					// disk error
					m_DskStat	=	E_GrDvrRecStatBad;
					// notify disk bad
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				}
			}

		}	//	if ( E_GrDvrFs3DmdItemCnt > A_DmdIdx )

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFs3Disk::LcMmdFindNewestThumb( __u32 A_DmdIdx, Ptr_GrDvrRecMinThumb A_PtrRtThumb )
{
	// local -------------------
		__s32	Tv_Result;
		__u64	Tv_Pos;
		__s32		Tv_WkIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrThumb;
		BOOL8	Tv_IsModified;
		BOOL8	Tv_IsBad;
	// code --------------------
		Tv_Result	=	-1;
		// check DMD index
		if ( E_GrDvrFs3DmdItemCnt > A_DmdIdx )
		{
			// init
			Tv_IsBad	=	TRUE;
			Tv_Pos	=	(__u64)( E_GrDvrFs3PosMmd + ( A_DmdIdx * E_GrDvrFs3MmdDaySize ) );
			// load 
			if ( LcSeek( Tv_Pos ) )
			{
				if ( LcRead( m_PtrTmpBuf, E_GrDvrFs3MmdDaySize ) )
				{
					Tv_IsBad	=	FALSE;
				}
			}
			// check read success
			if ( !Tv_IsBad )
			{
				// check current handle and time
				Tv_PtrThumb	=	(Ptr_GrDvrRecMinThumb)( (__u32)m_PtrTmpBuf + (E_GrDvrFs3MmdDaySize - sizeof(St_GrDvrRecMinThumb)) );
				for ( Tv_WkIdx=E_GrDvrFs3MmdDayItemCnt - 1;0 <= Tv_WkIdx;Tv_WkIdx-- )
				{
					if ( E_GrDvrRecHndlNone != Tv_PtrThumb->HndlRec )
					{
						if ( IsValidHndl( Tv_PtrThumb->HndlRec ) )
						{
							if ( -1 == Tv_Result )
							{
								// update value
								Tv_Result	=	Tv_WkIdx;

								A_PtrRtThumb->HndlRec		=	Tv_PtrThumb->HndlRec;
								A_PtrRtThumb->ChAtb[0]	=	Tv_PtrThumb->ChAtb[0];
								A_PtrRtThumb->ChAtb[1]	=	Tv_PtrThumb->ChAtb[1];
								A_PtrRtThumb->ChAtb[2]	=	Tv_PtrThumb->ChAtb[2];
								A_PtrRtThumb->ChAtb[3]	=	Tv_PtrThumb->ChAtb[3];
								A_PtrRtThumb->ChAtb[4]	=	Tv_PtrThumb->ChAtb[4];
								A_PtrRtThumb->ChAtb[5]	=	Tv_PtrThumb->ChAtb[5];
								A_PtrRtThumb->ChAtb[6]	=	Tv_PtrThumb->ChAtb[6];
								A_PtrRtThumb->ChAtb[7]	=	Tv_PtrThumb->ChAtb[7];
							}
						}
						else
						{
							// process bad thumb
							Tv_PtrThumb->HndlRec	=	E_GrDvrRecHndlNone;
							Tv_PtrThumb->ChAtb[0]	=	0;
							Tv_PtrThumb->ChAtb[1]	=	0;
							Tv_PtrThumb->ChAtb[2]	=	0;
							Tv_PtrThumb->ChAtb[3]	=	0;
							Tv_PtrThumb->ChAtb[4]	=	0;
							Tv_PtrThumb->ChAtb[5]	=	0;
							Tv_PtrThumb->ChAtb[6]	=	0;
							Tv_PtrThumb->ChAtb[7]	=	0;
							Tv_IsModified	=	TRUE;
						}
					}
					// next
					Tv_PtrThumb --;
				}
				// check modified
				if ( Tv_IsModified )
				{
					Tv_IsBad	=	TRUE;
					if ( LcSeek( Tv_Pos ) )
					{
						if ( LcWrite( m_PtrTmpBuf, E_GrDvrFs3MmdDaySize ) )
						{
							Tv_IsBad	=	FALSE;
						}
					}
				}

			}	//	if ( !Tv_IsBad )

			if ( Tv_IsBad )
			{
				if ( E_GrDvrRecStatBad != m_DskStat )
				{
					// disk error
					m_DskStat	=	E_GrDvrRecStatBad;
					// notify disk bad
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				}
			}

		}	//	if ( E_GrDvrFs3DmdItemCnt > A_DmdIdx )

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::LcTimeRngUpdt( void )
{
	// local -------------------
		__u32	Tv_BdtIdx;
		__u32	Tv_TimeSt;
		__u32	Tv_TimeEd;
	// code --------------------
		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			Tv_TimeSt	=	0xFFFFFFFF;
			Tv_TimeEd	=	0;
			for ( Tv_BdtIdx=0;Tv_BdtIdx< m_Mis.BlkCnt;Tv_BdtIdx++ )
			{
				if ( !m_Bdt[Tv_BdtIdx].IsBad )
				{
					if ( 0 != m_Bdt[Tv_BdtIdx].RecSize )
					{
						// check begin time
						if ( 0 != m_Bdt[Tv_BdtIdx].TimeSt )
						{
							if ( m_Bdt[Tv_BdtIdx].TimeSt < Tv_TimeSt )
							{
								Tv_TimeSt	=	m_Bdt[Tv_BdtIdx].TimeSt;
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrDvrFs3Disk::LcTimeRngUpdt - disk ( %d ) block ( %d ) data exist, but time is empty.\n", m_DevIdx, Tv_BdtIdx );
						}

						// check end time
						if ( 0 != m_Bdt[Tv_BdtIdx].TimeEd )
						{
							if ( m_Bdt[Tv_BdtIdx].TimeEd > Tv_TimeEd )
							{
								Tv_TimeEd	=	m_Bdt[Tv_BdtIdx].TimeEd;
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrDvrFs3Disk::LcTimeRngUpdt - disk ( %d ) block ( %d ) data exist, but time is empty.\n", m_DevIdx, Tv_BdtIdx );
						}
					}
				}
			}
			// check found
			if ( 0xFFFFFFFF != Tv_TimeSt )
			{
				m_RecTimeBegin	=	Tv_TimeSt;
			}
			else
			{
				m_RecTimeBegin	=	0;
				DbgMsgPrint( "Cls_GrDvrFs3Disk::LcTimeRngUpdt - disk not empty, but begin time not exist.\n" );
			}
			
			if ( 0 != Tv_TimeEd )
			{
				m_RecTimeEnd	=	Tv_TimeEd;
			}
			else
			{
				m_RecTimeEnd	=	0;
				DbgMsgPrint( "Cls_GrDvrFs3Disk::LcTimeRngUpdt - disk not empty, but time end not exist.\n" );
			}
		}
		else
		{
			m_RecTimeBegin	=	0;
			m_RecTimeEnd	=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::LcDmdCheck( void )
{
	// local -------------------
		__u32	Tv_WkIdx;
		Def_GrTime	Tv_Remain;
		BOOL8	Tv_IsModified;
		__u32	Tv_ChkIdx;
	// code --------------------
		Tv_IsModified	=	FALSE;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs3DmdItemCnt;Tv_WkIdx++ )
		{
			// check time exist
			if ( 0 != m_Dmd[Tv_WkIdx] )
			{
				Tv_Remain	=	m_Dmd[Tv_WkIdx] % E_GrTimeSecOfDay;
				if ( 0 != Tv_Remain )
				{
					DbgMsgPrint( "Cls_GrDvrFs3Disk::LcDmdCheck - Bad DMD time found. fix..\n" );
					m_Dmd[Tv_WkIdx]	=	m_Dmd[Tv_WkIdx] - Tv_Remain;
					Tv_IsModified	=	TRUE;
				}
			}
		}

		// check overlapped date
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs3DmdItemCnt;Tv_WkIdx++ )
		{
			if ( 0 != m_Dmd[Tv_WkIdx] )
			{
				Tv_ChkIdx	=	Tv_WkIdx + 1;
				while ( E_GrDvrFs3DmdItemCnt > Tv_ChkIdx )
				{
					// check equal
					if ( m_Dmd[Tv_WkIdx] == m_Dmd[Tv_ChkIdx] )
					{
						// match DMD found
						DbgMsgPrint( "Cls_GrDvrFs3Disk::LcDmdCheck - overlapped date found!, remove overlapped date.\n" );
						m_Dmd[Tv_ChkIdx]	=	0;	// remove
						Tv_IsModified			=	TRUE;
					}
					// next
					Tv_ChkIdx ++;
				}
			}
		}
		
		// update modified
		if ( Tv_IsModified )
		{
			LcDmdWrite();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs3Disk::DiskStatGet( void )
{
		return	m_DskStat;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcElaDisable( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDvrLogV2Item	Tv_PtrLogTbl;
		__u64		Tv_DskPos;		// disk position
		__u32		Tv_Remain;
		__u32		Tv_RwSize;		// read write size
		__u32		Tv_ItmCnt;
		__u32		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		// init
		Tv_Remain	=	m_Mis.ElaSize;
		Tv_DskPos	=	E_GrDvrFs3PosEla;
		Tv_PtrLogTbl	=	(Ptr_GrDvrLogV2Item)m_PtrTmpBuf;

		while ( 0 != Tv_Remain )
		{
			// calculate read write size
			Tv_RwSize	=	Tv_Remain;
			if ( E_GrDvrFs3TmpBufSize < Tv_RwSize )
			{
				Tv_RwSize	=	E_GrDvrFs3TmpBufSize;
			}
			Tv_RwSize	=	Tv_RwSize & E_GrDvrLogAlignMask;	// align

			// read
			if ( !LcSeek( Tv_DskPos ) )
			{
				break;
			}
			if ( !LcRead( Tv_PtrLogTbl, Tv_RwSize ) )
			{
				break;
			}

			// disable
			Tv_ItmCnt	=	Tv_RwSize >> E_GrDvrLogShiftSize;
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ItmCnt;Tv_WkIdx++ )
			{
				Tv_PtrLogTbl[Tv_WkIdx].IsAble	=	FALSE;
			}

			// write
			if ( !LcSeek( Tv_DskPos ) )
			{
				break;
			}
			if ( !LcWrite( Tv_PtrLogTbl, Tv_RwSize ) )
			{
				break;
			}
			// next
			Tv_Remain	=	Tv_Remain - Tv_RwSize;
			Tv_DskPos	=	Tv_DskPos + (__u64)Tv_RwSize;
		}

		// check success
		if ( 0 == Tv_Remain )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFs3Disk::LcDmdFind( Def_GrTime A_Day )
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for (Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs3DmdItemCnt;Tv_WkIdx++)
		{
			if ( m_Dmd[Tv_WkIdx] == A_Day )
			{
				// found
				Tv_Result	=	Tv_WkIdx;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFs3Disk::LcDmdAlloc( void )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_WkIdx;
		BOOL8	Tv_IsOk;
		__u64	Tv_Pos;
	// code --------------------
		Tv_Result	=	-1;

		// find empty
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs3DmdItemCnt;Tv_WkIdx++ )
		{
			if ( 0 == m_Dmd[Tv_WkIdx] )
			{
				// found
				Tv_Result	=	Tv_WkIdx;
				// MMD clear
				Tv_Pos		=	E_GrDvrFs3PosMmd + (__u64)( (__u32)Tv_WkIdx * E_GrDvrFs3MmdDaySize );
				LcFillZero( Tv_Pos, E_GrDvrFs3MmdDaySize );
				break;
			}
		}

		// check success
		if ( 0 > Tv_Result )
		{
			// garbage collection
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs3DmdItemCnt;Tv_WkIdx++ )
			{
				if ( 0 != m_Dmd[Tv_WkIdx] )
				{
					Tv_IsOk	=	LcMmdLoadByDmdIdx( Tv_WkIdx, m_PtrTmpBuf );
					// check bad
					if ( E_GrDvrRecStatBad != m_DskStat )
					{
						break;
					}
					if ( !Tv_IsOk )
					{
						// garbage found
						m_Dmd[Tv_WkIdx]	=	0;
						Tv_Result	=	Tv_WkIdx;
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs3Disk::HndlFromRdaPos( __u64 A_RdaPos )
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	LcLocalHndlByRdaPos( A_RdaPos );
		Tv_Result	=	Tv_Result | (__u64)m_GrpIntIdx;
		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs3Disk::BeginTimeGet( void )
{
		return	m_RecTimeBegin;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs3Disk::EndTimeGet( void )
{
		return	m_RecTimeEnd;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::RecSizeGet( __u64* A_PtrRtTotal, __u64* A_PtrRtRec )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			*A_PtrRtTotal	=	m_RecCap;
			*A_PtrRtRec		=	m_RecSize;
			Tv_Result	=	TRUE;
		}
		else
		{
			*A_PtrRtTotal	=	m_DskSize;
			*A_PtrRtRec		=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFs3Disk::LcDmdNearFw( Def_GrTime A_Time, Def_GrTime* A_PtrRtTime )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs3DmdItemCnt;Tv_WkIdx++ )
		{
			if ( A_Time == m_Dmd[Tv_WkIdx] )
			{
				Tv_Result	=	Tv_WkIdx;
				*A_PtrRtTime	=	m_Dmd[Tv_WkIdx];
				break;
			}
			else if ( A_Time < m_Dmd[Tv_WkIdx] )
			{
				// check found
				if ( 0 <= Tv_Result )
				{
					if ( m_Dmd[Tv_Result] > m_Dmd[Tv_WkIdx] )
					{
						// update
						Tv_Result	=	Tv_WkIdx;
						*A_PtrRtTime	=	m_Dmd[Tv_WkIdx];
					}
				}
				else
				{
					Tv_Result			=	Tv_WkIdx;
					*A_PtrRtTime	=	m_Dmd[Tv_WkIdx];
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs3Disk::RecDayFindFw( Def_GrTime A_TimeBase )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__s32		Tv_DmdIdx;
	// code --------------------
		// find nearest DMD
		Tv_DmdIdx	=	LcDmdNearFw( A_TimeBase, &Tv_Result );
		if ( 0 > Tv_DmdIdx )
		{
			Tv_Result	=	0;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32		Cls_GrDvrFs3Disk::LcDmdNearBw( Def_GrTime A_Time, Def_GrTime* A_PtrRtTime )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs3DmdItemCnt;Tv_WkIdx++ )
		{
			if ( A_Time == m_Dmd[Tv_WkIdx] )
			{
				Tv_Result	=	Tv_WkIdx;
				*A_PtrRtTime	=	m_Dmd[Tv_WkIdx];
				break;
			}
			else if ( (0 != m_Dmd[Tv_WkIdx]) && (A_Time > m_Dmd[Tv_WkIdx]) )
			{
				// check found
				if ( 0 <= Tv_Result )
				{
					if ( m_Dmd[Tv_Result] < m_Dmd[Tv_WkIdx] )
					{
						// update
						Tv_Result	=	Tv_WkIdx;
						*A_PtrRtTime	=	m_Dmd[Tv_WkIdx];
					}
				}
				else
				{
					Tv_Result	=	Tv_WkIdx;
					*A_PtrRtTime	=	m_Dmd[Tv_WkIdx];
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs3Disk::RecDayFindBw( Def_GrTime A_TimeBase )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__s32		Tv_DmdIdx;
	// code --------------------
		// find nearest DMD
		Tv_DmdIdx	=	LcDmdNearBw( A_TimeBase, &Tv_Result );
		if ( 0 > Tv_DmdIdx )
		{
			Tv_Result	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcBdtChkIntra( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_ChkIdx;
		__u32	Tv_SubIdx;
	// code --------------------
		Tv_Result	=	TRUE;

		for ( Tv_ChkIdx=0;Tv_ChkIdx < m_Mis.BlkCnt;Tv_ChkIdx++ )
		{
			// check bad block
			if ( !m_Bdt[Tv_ChkIdx].IsBad )
			{
				// check recorded size
				if ( E_GrDvrFs3BlockSize < m_Bdt[Tv_ChkIdx].RecSize )
				{
					Tv_Result	=	FALSE;
					break;
				}
				// check overlapped record id 
				if ( 0 != m_Bdt[Tv_ChkIdx].RecSize )
				{
					Tv_SubIdx	=	Tv_ChkIdx + 1;
					while ( Tv_SubIdx < m_Mis.BlkCnt )
					{
						if ( !m_Bdt[Tv_SubIdx].IsBad )
						{
							if ( 0 != m_Bdt[Tv_SubIdx].RecSize )
							{
								if ( m_Bdt[Tv_ChkIdx].RecId == m_Bdt[Tv_SubIdx].RecId )
								{
									Tv_Result	=	FALSE;
									break;
								}
							}
						}
						
						// next
						Tv_SubIdx ++;
					}
					// check finished
					if ( !Tv_Result )
					{
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::LcNormalStatUpdt( void )
{
	// local -------------------
		__u8	Tv_Stat;
		__u32	Tv_BdtIdx;
		__u32	Tv_BadCnt;
		__u32	Tv_FullCnt;
		__u64	Tv_RecCap;
		__u64	Tv_RecSize;
	// code --------------------
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			Tv_Stat			=	E_GrDvrRecStatEmpty;
			Tv_BadCnt		=	0;
			Tv_FullCnt	=	0;
			Tv_RecCap		=	0;
			Tv_RecSize	=	0;
			for ( Tv_BdtIdx=0;Tv_BdtIdx < m_Mis.BlkCnt;Tv_BdtIdx++ )
			{
				if ( m_Bdt[Tv_BdtIdx].IsBad )
				{
					Tv_BadCnt ++;
				}
				else if ( m_Bdt[Tv_BdtIdx].IsFull )
				{
					Tv_FullCnt ++;
					Tv_Stat	=	E_GrDvrRecStatNormal;
					Tv_RecCap		=	Tv_RecCap + (__u64)E_GrDvrFs3BlockSize;
					Tv_RecSize	=	Tv_RecSize + (__u64)E_GrDvrFs3BlockSize;
				}
				else
				{
					if ( 0 != m_Bdt[Tv_BdtIdx].RecSize )
					{
						Tv_Stat	=	E_GrDvrRecStatNormal;
						Tv_RecSize	=	Tv_RecSize + (__u64)m_Bdt[Tv_BdtIdx].RecSize;
					}
					Tv_RecCap		=	Tv_RecCap + (__u64)E_GrDvrFs3BlockSize;
				}
			}
			// check disk bad
			if ( Tv_BadCnt == m_Mis.BlkCnt )
			{
				Tv_Stat	=	E_GrDvrRecStatBad;
			}
			else if ( (m_Mis.BlkCnt - Tv_BadCnt) == Tv_FullCnt )
			{
				Tv_Stat	=	E_GrDvrRecStatFull;
			}
			// update
			m_DskStat	=	Tv_Stat;
			m_RecCap	=	Tv_RecCap;
			m_RecSize	=	Tv_RecSize;
		}
}
//--------------------------------------------------------------------
Ptr_GrDvrFs3Bdt	Cls_GrDvrFs3Disk::BdtTableGet( void )
{
		return	m_Bdt;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs3Disk::BlockCountGet( void )
{
		return	m_Mis.BlkCnt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::IsBdtOverlap( __u32 A_RecId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BdtIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		// check can check
		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			for ( Tv_BdtIdx=0;Tv_BdtIdx < m_Mis.BlkCnt;Tv_BdtIdx++ )
			{
				// check bad block
				if ( !m_Bdt[Tv_BdtIdx].IsBad )
				{
					// check record data exist
					if ( 0 != m_Bdt[Tv_BdtIdx].RecSize )
					{
						if ( A_RecId == m_Bdt[Tv_BdtIdx].RecId )
						{
							// overlapped
							Tv_Result	=	TRUE;
							break;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::IsBdtCrossCheck( Cls_GrDvrFs3Disk* A_Disk )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDvrFs3Bdt	Tv_PtrSr;
		__u32	Tv_SrBlkCnt;
		__u32	Tv_SrIdx;
		__u32	Tv_TgIdx;
	// code --------------------
		Tv_Result	=	TRUE;

		// check disk status
		if ( (E_GrDvrRecStatDefective <= m_DskStat) && (E_GrDvrRecStatDefective <= A_Disk->DiskStatGet()) )
		{
			Tv_SrBlkCnt	=	A_Disk->BlockCountGet();
			if ( 0 != Tv_SrBlkCnt )
			{
				Tv_PtrSr		=	A_Disk->BdtTableGet();
				for ( Tv_SrIdx=0;Tv_SrIdx < Tv_SrBlkCnt;Tv_SrIdx ++ )
				{
					// check append able
					if ( !Tv_PtrSr[Tv_SrIdx].IsBad )
					{
						if ( 0 != Tv_PtrSr[Tv_SrIdx].RecSize )
						{
							// check target
							for ( Tv_TgIdx=0;Tv_TgIdx < m_Mis.BlkCnt;Tv_TgIdx++ )
							{
								if ( !m_Bdt[Tv_TgIdx].IsBad )
								{
									if ( 0 != m_Bdt[Tv_TgIdx].RecSize )
									{
										if ( Tv_PtrSr[Tv_SrIdx].RecId == m_Bdt[Tv_TgIdx].RecId )
										{
											// overlapped
											Tv_Result	=	FALSE;
											break;
										}
									}
								}
								
							}
							if ( !Tv_Result )
							{
								break;
							}
						}
					}
					
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::BdtSave( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BdtSize;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecStatDefective <= m_DskStat )
		{
			if ( LcSeek( E_GrDvrFs3PosBdt ) )
			{
				Tv_BdtSize	=	m_Mis.BlkCnt * sizeof(St_GrDvrFs3Bdt);
				if ( LcWrite( m_Bdt, Tv_BdtSize ) )
				{
					Tv_Result	=	TRUE;
					// reinitialize
					if ( LcBdtChkIntra() )
					{
						m_DskStat	=	E_GrDvrRecStatEmpty;
						LcNormalStatUpdt();
					}
					else
					{
						Tv_Result	=	FALSE;
					}

				}
			}
			if ( !Tv_Result )
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::BlockEmptyFind( __u32* A_PtrRtIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			for ( Tv_BlkIdx=0;Tv_BlkIdx < m_Mis.BlkCnt;Tv_BlkIdx++ )
			{
				if ( (!m_Bdt[Tv_BlkIdx].IsBad) && (!m_Bdt[Tv_BlkIdx].IsFull) )
				{
					if ( 0 == m_Bdt[Tv_BlkIdx].RecSize )
					{
						// found
						Tv_Result		=	TRUE;
						*A_PtrRtIdx	=	Tv_BlkIdx;
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs3Disk::BlockFreeSizeGet( __u32 A_BlkIdx )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			if ( A_BlkIdx < m_Mis.BlkCnt )
			{
				if ( (!m_Bdt[A_BlkIdx].IsBad) && (!m_Bdt[A_BlkIdx].IsFull) )
				{
					Tv_Result	=	E_GrDvrFs3BlockSize - m_Bdt[A_BlkIdx].RecSize;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::BlockFullSet( __u32 A_BlkIdx )
{
	// local -------------------
	// code --------------------
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			if ( A_BlkIdx < m_Mis.BlkCnt )
			{
				if ( (!m_Bdt[A_BlkIdx].IsBad) && (!m_Bdt[A_BlkIdx].IsFull) )
				{
					// mark full
					m_Bdt[A_BlkIdx].IsFull	=	TRUE;
					// save BDT
					LcBdtWriteByBlkId( A_BlkIdx );
					// update information
					LcNormalStatUpdt();
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcBdtWriteByBlkId( __u32 A_BlkId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Pos;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( A_BlkId < m_Mis.BlkCnt )
		{
			Tv_Pos	=	E_GrDvrFs3PosBdt + (__u64)( A_BlkId * sizeof(St_GrDvrFs3Bdt) );
			if ( LcSeek( Tv_Pos ) )
			{
				if ( LcWrite( &m_Bdt[A_BlkId], sizeof(St_GrDvrFs3Bdt) ) )
				{
					Tv_Result	=	TRUE;
				}
			}
			// check error
			if ( !Tv_Result )
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs3Disk::BlockMaxRecIdFind( __u32* A_PtrRtIdx )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	E_GrDvrFs3RecIdNone;

		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			for ( Tv_BlkIdx=0;Tv_BlkIdx < m_Mis.BlkCnt;Tv_BlkIdx++ )
			{
				if ( (!m_Bdt[Tv_BlkIdx].IsBad) && (0 != m_Bdt[Tv_BlkIdx].RecSize) )
				{
					if ( Tv_Result < m_Bdt[Tv_BlkIdx].RecId )
					{
						// found
						Tv_Result		=	m_Bdt[Tv_BlkIdx].RecId;
						*A_PtrRtIdx	=	Tv_BlkIdx;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs3Disk::BlockMinRecIdFind( __u32* A_PtrRtIdx )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	E_GrDvrFs3RecIdNone;

		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			for ( Tv_BlkIdx=0;Tv_BlkIdx < m_Mis.BlkCnt;Tv_BlkIdx++ )
			{
				if ( (!m_Bdt[Tv_BlkIdx].IsBad) && (0 != m_Bdt[Tv_BlkIdx].RecSize) )
				{
					if ( E_GrDvrFs3RecIdNone != m_Bdt[Tv_BlkIdx].RecId )
					{
						if ( E_GrDvrFs3RecIdNone == Tv_Result)
						{
							// found
							Tv_Result		=	m_Bdt[Tv_BlkIdx].RecId;
							*A_PtrRtIdx	=	Tv_BlkIdx;
						}
						else 
						{
							if ( Tv_Result > m_Bdt[Tv_BlkIdx].RecId )
							{
								// found
								Tv_Result		=	m_Bdt[Tv_BlkIdx].RecId;
								*A_PtrRtIdx	=	Tv_BlkIdx;
							}
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrDvrFs3Disk::BlockMinRecIdFind - bad recid block (%d:%d) found. delete.\n", m_DevIdx, Tv_BlkIdx );
						BlockDelete( Tv_BlkIdx );
						GrTaskSvrMsgErrorNotify( E_GrErrRecBadRecIdBlk,m_DevIdx,0,Tv_BlkIdx );
					}
					
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::BlockDelete( __u32 A_BlkIdx )
{
	// local -------------------
	// code --------------------
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			if ( A_BlkIdx < m_Mis.BlkCnt )
			{
#ifdef E_GrDvrFs3DiskDbgMsgView
				DbgMsgPrint( "Cls_GrDvrFs3Disk::BlockDelete - disk(%d) block(%d) recid(%d) \n", m_DevIdx, A_BlkIdx, m_Bdt[A_BlkIdx].RecId );
#endif
				m_Bdt[A_BlkIdx].IsFull		=	FALSE;
				m_Bdt[A_BlkIdx].OvwtId ++;
				m_Bdt[A_BlkIdx].RecId		=	E_GrDvrFs3RecIdNone;
				m_Bdt[A_BlkIdx].RecSize	=	0;
				m_Bdt[A_BlkIdx].TimeSt		=	0;
				m_Bdt[A_BlkIdx].TimeEd		=	0;
				LcBdtWriteByBlkId( A_BlkIdx );
				// update time range
				LcTimeRngUpdt();
				// update status
				LcNormalStatUpdt();
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::BlockDataAdd( __u32 A_BlkId, __u32 A_RecId, void* A_PtrBuf, __u32 A_Size, 
	Ptr_GrDvrFs3BlkAddInfo A_PtrInfo, BOOL8 A_IsBdtUpdt )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsBdtWrite;
		__u32	Tv_FreeSize;
	// code --------------------
		Tv_Result	=	FALSE;

		// check disk status
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			if ( A_BlkId < m_Mis.BlkCnt )
			{
				if ( 0 != A_Size )
				{
					if ( !m_Bdt[A_BlkId].IsBad  )
					{
						if (!m_Bdt[A_BlkId].IsFull)
						{
							Tv_FreeSize	=	E_GrDvrFs3BlockSize - m_Bdt[A_BlkId].RecSize;
							if ( Tv_FreeSize >= A_Size )
							{
								// write data
								if ( LcBlockWrite( A_BlkId, m_Bdt[A_BlkId].RecSize, A_PtrBuf, A_Size, TRUE ) )
								{
									// update info
									m_Bdt[A_BlkId].RecSize	=	m_Bdt[A_BlkId].RecSize + A_Size;
									// check valid time
									if ( (0 != A_PtrInfo->TimeSt) && (0 != A_PtrInfo->TimeEd) )
									{
										// update block time
										if ( 0 == m_Bdt[A_BlkId].TimeSt )
										{
											m_Bdt[A_BlkId].TimeSt	=	A_PtrInfo->TimeSt;
										}
										else if ( m_Bdt[A_BlkId].TimeSt > A_PtrInfo->TimeSt )
										{
											m_Bdt[A_BlkId].TimeSt	=	A_PtrInfo->TimeSt;
										}
										if ( 0 == m_Bdt[A_BlkId].TimeEd )
										{
											m_Bdt[A_BlkId].TimeEd	=	A_PtrInfo->TimeEd;
										}
										else if ( m_Bdt[A_BlkId].TimeEd	< A_PtrInfo->TimeEd )
										{
											m_Bdt[A_BlkId].TimeEd	=	A_PtrInfo->TimeEd;
										}
										// update disk time
										if ( 0 == m_RecTimeBegin )
										{
											m_RecTimeBegin	=	A_PtrInfo->TimeSt;
										}
										else if ( m_RecTimeBegin > A_PtrInfo->TimeSt )
										{
											m_RecTimeBegin	=	A_PtrInfo->TimeSt;
										}
										if ( 0 == m_RecTimeEnd )
										{
											m_RecTimeEnd	=	A_PtrInfo->TimeEd;
										}
										else if ( m_RecTimeEnd < A_PtrInfo->TimeEd )
										{
											m_RecTimeEnd	=	A_PtrInfo->TimeEd;
										}
									}
									else
									{
										DbgMsgPrint( "Cls_GrDvrFs3Disk::BlockDataAdd - bad time input.\n" );
									}

									// update record id
									Tv_IsBdtWrite	=	A_IsBdtUpdt;
									if ( E_GrDvrFs3RecIdNone == m_Bdt[A_BlkId].RecId )
									{
										m_Bdt[A_BlkId].RecId	=	A_RecId;
										Tv_IsBdtWrite	=	TRUE;
									}

									// check full
									if ( E_GrDvrFs3BlockSize <= m_Bdt[A_BlkId].RecSize )
									{
										m_Bdt[A_BlkId].IsFull	=	TRUE;
										Tv_IsBdtWrite	=	TRUE;
									}
									// update BCS
									LcBcsUpdt( A_BlkId, A_PtrInfo );

									// update TMA
									if ( LcThumbUpdt( A_PtrInfo ) )
									{
										Tv_IsBdtWrite	=	TRUE;
									}

									if ( Tv_IsBdtWrite )
									{
										LcBdtWriteByBlkId( A_BlkId );
									}

									// update status
									LcNormalStatUpdt();
									// success
									Tv_Result	=	TRUE;
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrDvrFs3Disk::BlockDataAdd - Expect size miss (%d - > %d).\n",A_Size, Tv_FreeSize );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrDvrFs3Disk::BlockDataAdd - Block is full (%d:%d).\n",m_DevIdx, A_BlkId );
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrDvrFs3Disk::BlockDataAdd - Bad block (%d:%d).\n",m_DevIdx, A_BlkId );
					}
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrDvrFs3Disk::BlockDataAdd - bad block id (%d:%d).\n",m_DevIdx, A_BlkId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcBlockWrite( __u32 A_BlkId, __u32 A_Ofs, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsLhndlUpdt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_RdaPos;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_RdaPos	=	( (__u64)A_BlkId << E_GrDvrFs3BlockShift ) + (__u64)A_Ofs;
		Tv_Dpos		=	Tv_RdaPos + m_Mis.RdaPos;
		if ( LcSeek( Tv_Dpos ) )
		{
			if ( LcWrite( A_PtrBuf, A_Size ) )
			{
				Tv_Result	=	TRUE;
				// update last handle
				if ( A_IsLhndlUpdt )
				{
					m_HndlLastRec	=	LcLocalHndlByRdaPos( Tv_RdaPos );
				}
			}
		}
		if ( !Tv_Result )
		{
			m_Bdt[A_BlkId].IsBad	=	TRUE;
			DbgMsgPrint( "Cls_GrDvrFs3Disk::LcBlockWrite - bad block (%d:%d) appear.\n", m_DevIdx, A_BlkId );
			LcBdtWriteByBlkId( A_BlkId );
			GrTaskSvrMsgErrorNotify( E_GrErrDiskBadSector, m_DevIdx );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::BlockInfoGet( __u32 A_BlkId, __u32* A_PtrRtRecId, __u32* A_PtrRtDataSize, __u16* A_PtrRtOvwtId )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check disk status
		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			if ( A_BlkId < m_Mis.BlkCnt )
			{
				if ( !m_Bdt[A_BlkId].IsBad )
				{
					if ( 0 != m_Bdt[A_BlkId].RecSize )
					{
						*A_PtrRtRecId			=	m_Bdt[A_BlkId].RecId;
						*A_PtrRtDataSize	=	m_Bdt[A_BlkId].RecSize;
						*A_PtrRtOvwtId		=	m_Bdt[A_BlkId].OvwtId;
						Tv_Result					=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs3Disk::BlockFwRecIdFind( __u32 A_RecId, __u32* A_PtrRtBlkId, __u16* A_PtrRtOvwtId, __u32* A_PtrRtRecSize )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkId;
	// code --------------------
		Tv_Result	=	E_GrDvrFs3RecIdNone;

		if ( (E_GrDvrRecStatEmpty < m_DskStat) && (E_GrDvrFs3RecIdNone != A_RecId) )
		{
			for ( Tv_BlkId=0;Tv_BlkId < m_Mis.BlkCnt;Tv_BlkId++ )
			{
				// check able
				if ( (!m_Bdt[Tv_BlkId].IsBad) && (0 != m_Bdt[Tv_BlkId].RecSize) )
				{
					if ( E_GrDvrFs3RecIdNone != m_Bdt[Tv_BlkId].RecId )
					{
						if ( A_RecId == m_Bdt[Tv_BlkId].RecId )
						{
							Tv_Result				=	m_Bdt[Tv_BlkId].RecId;
							*A_PtrRtBlkId		=	Tv_BlkId;
							*A_PtrRtOvwtId	=	m_Bdt[Tv_BlkId].OvwtId;
							*A_PtrRtRecSize	=	m_Bdt[Tv_BlkId].RecSize;
							break;
						}
						else if ( A_RecId < m_Bdt[Tv_BlkId].RecId )
						{
							if ( E_GrDvrFs3RecIdNone == Tv_Result )
							{
								// update first
								Tv_Result				=	m_Bdt[Tv_BlkId].RecId;
								*A_PtrRtBlkId		=	Tv_BlkId;
								*A_PtrRtOvwtId	=	m_Bdt[Tv_BlkId].OvwtId;
								*A_PtrRtRecSize	=	m_Bdt[Tv_BlkId].RecSize;
							}
							else if ( Tv_Result > m_Bdt[Tv_BlkId].RecId )
							{
								// new found
								Tv_Result				=	m_Bdt[Tv_BlkId].RecId;
								*A_PtrRtBlkId		=	Tv_BlkId;
								*A_PtrRtOvwtId	=	m_Bdt[Tv_BlkId].OvwtId;
								*A_PtrRtRecSize	=	m_Bdt[Tv_BlkId].RecSize;
							}
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrDvrFs3Disk::BlockFwRecIdFind - bad RecId found on block %d:%d!\n", m_DevIdx, Tv_BlkId );
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs3Disk::BlockBwRecIdFind( __u32 A_RecId, __u32* A_PtrRtBlkId, __u16* A_PtrRtOvwtId, __u32* A_PtrRtRecSize )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkId;
	// code --------------------
		Tv_Result	=	E_GrDvrFs3RecIdNone;

		if ( (E_GrDvrRecStatEmpty < m_DskStat) && (E_GrDvrFs3RecIdNone != A_RecId) )
		{
			for ( Tv_BlkId=0;Tv_BlkId < m_Mis.BlkCnt;Tv_BlkId++ )
			{
				// check able
				if ( (!m_Bdt[Tv_BlkId].IsBad) && (0 != m_Bdt[Tv_BlkId].RecSize) )
				{
					if ( E_GrDvrFs3RecIdNone != m_Bdt[Tv_BlkId].RecId )
					{
						if ( A_RecId == m_Bdt[Tv_BlkId].RecId )
						{
							Tv_Result				=	m_Bdt[Tv_BlkId].RecId;
							*A_PtrRtBlkId		=	Tv_BlkId;
							*A_PtrRtOvwtId	=	m_Bdt[Tv_BlkId].OvwtId;
							*A_PtrRtRecSize	=	m_Bdt[Tv_BlkId].RecSize;
							break;
						}
						else if ( A_RecId > m_Bdt[Tv_BlkId].RecId )
						{
							if ( E_GrDvrFs3RecIdNone == Tv_Result )
							{
								// update first
								Tv_Result				=	m_Bdt[Tv_BlkId].RecId;
								*A_PtrRtBlkId		=	Tv_BlkId;
								*A_PtrRtOvwtId	=	m_Bdt[Tv_BlkId].OvwtId;
								*A_PtrRtRecSize	=	m_Bdt[Tv_BlkId].RecSize;
							}
							else if ( Tv_Result < m_Bdt[Tv_BlkId].RecId )
							{
								// new found
								Tv_Result				=	m_Bdt[Tv_BlkId].RecId;
								*A_PtrRtBlkId		=	Tv_BlkId;
								*A_PtrRtOvwtId	=	m_Bdt[Tv_BlkId].OvwtId;
								*A_PtrRtRecSize	=	m_Bdt[Tv_BlkId].RecSize;
							}
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrDvrFs3Disk::BlockBwRecIdFind - bad RecId found on block %d:%d!\n", m_DevIdx, Tv_BlkId );
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::ReadByHndl( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_RdaPos;
		__u64	Tv_Dpos;
		__u32	Tv_BlkId;
	// code --------------------
		Tv_Result	=	FALSE;

		// check disk status
		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			Tv_RdaPos	=	A_Hndl & E_GrDvrFs3HndlRdaPosMask;
			Tv_Dpos		=	Tv_RdaPos + m_Mis.RdaPos;
			if ( LcSeek( Tv_Dpos ) )
			{
				if ( LcRead( A_PtrBuf, A_Size ) )
				{
					Tv_Result	=	TRUE;
				}
			}
			if ( !Tv_Result )
			{
				// mark bad
				Tv_BlkId	=	(__u32)( Tv_RdaPos >> E_GrDvrFs3BlockShift );
				if ( Tv_BlkId < m_Mis.BlkCnt )
				{
					if ( m_Bdt[Tv_BlkId].IsBad )
					{
						DbgMsgPrint( "Cls_GrDvrFs3Disk::ReadByHndl - bad block (%d:%d) access.\n", m_DevIdx, Tv_BlkId );
					}
					else
					{
						m_Bdt[Tv_BlkId].IsBad	=	TRUE;
						GrTaskSvrMsgErrorNotify( E_GrErrDiskBadSector, m_DevIdx );
						LcBdtWriteByBlkId( Tv_BlkId );
						LcNormalStatUpdt();
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrDvrFs3Disk::ReadByHndl - bad handle input.\n" );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs3Disk::TimeHndlFindFw( Def_GrTime A_TimeSt, Def_GrTime* A_PtrRtTime )
{
	// local -------------------
		__u64	Tv_Result;
		__s32		Tv_DmdIdx;
		Def_GrTime	Tv_TimeDay;
		Def_GrTime	Tv_TimeDmd;
		Def_GrTime	Tv_TimeRslt;
		__u32	Tv_MmdIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrTmb;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		Tv_PtrTmb		=	(Ptr_GrDvrRecMinThumb)( (__u32)m_PtrTmpBuf + E_GrDvrFs3DiskMmdCmbBufPos );

		Tv_TimeRslt	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin);

		while ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			Tv_TimeDay	=	Tv_TimeRslt - (Tv_TimeRslt % E_GrTimeSecOfDay);
			Tv_DmdIdx	=	LcDmdNearFw( Tv_TimeDay, &Tv_TimeDmd );
			if ( 0 > Tv_DmdIdx )
			{
				break;
			}
			// check change day
			if ( Tv_TimeDay != Tv_TimeDmd )
			{
				// update new day
				Tv_TimeRslt	=	Tv_TimeDmd;
				Tv_TimeDay	=	Tv_TimeDmd;
			}
			// load
			if ( LcMmdLoadByDmdIdx( (__u32)Tv_DmdIdx, Tv_PtrTmb ) )
			{
				Tv_MmdIdx	=	(Tv_TimeRslt - Tv_TimeDay) / E_GrTimeSecOfMin;
				while ( E_GrDvrFs3MmdDayItemCnt > Tv_MmdIdx )
				{
					// check handle
					if ( E_GrDvrRecHndlNone != Tv_PtrTmb[Tv_MmdIdx].HndlRec )
					{
						Tv_Result			=	Tv_PtrTmb[Tv_MmdIdx].HndlRec;
						*A_PtrRtTime	=	Tv_TimeRslt;
						break;
					}
					// next
					Tv_MmdIdx ++;
					Tv_TimeRslt	=	Tv_TimeRslt + E_GrTimeSecOfMin;
				}
				// check found
				if ( E_GrDvrRecHndlNone != Tv_Result )
				{
					break;
				}
			}
			// next
			Tv_TimeRslt	=	Tv_TimeDay + E_GrTimeSecOfDay;
		}
		
		if ( E_GrDvrRecHndlNone != Tv_Result )
		{
			// global handle
			Tv_Result	=	(Tv_Result & (~E_GrDvrFs3HndlGrpIdxMask) ) | (__u64)m_GrpIntIdx ;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs3Disk::TimeHndlFindBw( Def_GrTime A_TimeSt, Def_GrTime* A_PtrRtTime )
{
	// local -------------------
		__u64	Tv_Result;
		__s32		Tv_DmdIdx;
		Def_GrTime	Tv_TimeDay;
		Def_GrTime	Tv_TimeDmd;
		Def_GrTime	Tv_TimeRslt;
		__s32	Tv_MmdIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrTmb;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		Tv_PtrTmb		=	(Ptr_GrDvrRecMinThumb)( (__u32)m_PtrTmpBuf + E_GrDvrFs3DiskMmdCmbBufPos );

		Tv_TimeRslt	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin);

		while ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			Tv_TimeDay	=	Tv_TimeRslt - (Tv_TimeRslt % E_GrTimeSecOfDay);
			Tv_DmdIdx	=	LcDmdNearBw( Tv_TimeDay, &Tv_TimeDmd );
			if ( 0 > Tv_DmdIdx )
			{
				// quit
				break;
			}
			// check change day
			if ( Tv_TimeDay != Tv_TimeDmd )
			{
				Tv_TimeRslt	=	Tv_TimeDmd + E_GrTimeSecOfDay - E_GrTimeSecOfMin;
				Tv_TimeDay	=	Tv_TimeDmd;
			}
			// load
			if ( LcMmdLoadByDmdIdx( (__u32)Tv_DmdIdx, Tv_PtrTmb ) )
			{
				Tv_MmdIdx	=	(__s32)((Tv_TimeRslt - Tv_TimeDay) / E_GrTimeSecOfMin);
				while ( 0 <= Tv_MmdIdx )
				{
					// check handle
					if ( E_GrDvrRecHndlNone != Tv_PtrTmb[Tv_MmdIdx].HndlRec )
					{
						Tv_Result			=	Tv_PtrTmb[Tv_MmdIdx].HndlRec;
						*A_PtrRtTime	=	Tv_TimeRslt;
						break;
					}
					// next
					Tv_MmdIdx --;
					Tv_TimeRslt	=	Tv_TimeRslt - E_GrTimeSecOfMin;
				}
				// check found
				if ( E_GrDvrRecHndlNone != Tv_Result )
				{
					break;
				}
			}
			// go next
			Tv_TimeRslt	=	Tv_TimeDay - E_GrTimeSecOfMin;
		}

		if ( E_GrDvrRecHndlNone != Tv_Result )
		{
			// global handle
			Tv_Result	=	(Tv_Result & (~E_GrDvrFs3HndlGrpIdxMask) ) | (__u64)m_GrpIntIdx ;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::BlockInfoFlush( __u32 A_BlkId )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			Tv_Result	=	LcBdtWriteByBlkId( A_BlkId );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcPread( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( LcSeek( A_Pos ) )
		{
			Tv_Result	=	LcRead( A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcPwrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( LcSeek( A_Pos ) )
		{
			Tv_Result	=	LcWrite( A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcBcsWrite( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Pos;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0  <= m_BcsBlk )
		{
			Tv_Pos	=	E_GrDvrFs3PosBcs + ( E_GrDvrFs3BcsUnitSize * (__u32)m_BcsBlk );
			if ( LcPwrite( Tv_Pos, &m_BcsData, sizeof(m_BcsData) ) )
			{
				Tv_Result	=	TRUE;
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcBcsUpdt( __u32 A_BlkIdx, Ptr_GrDvrFs3BlkAddInfo A_PtrInfo )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Pos;
		__s32		Tv_TmIdx;
		Def_GrTime	Tv_TmMin;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		// check equal block index
		if ( (__s32)A_BlkIdx != m_BcsBlk )
		{
			// load BCS
			if ( E_GrDvrRecStatBad != m_DskStat )
			{
				Tv_Pos	=	E_GrDvrFs3PosBcs + ( E_GrDvrFs3BcsUnitSize * A_BlkIdx );
				if ( LcPread( Tv_Pos, &m_BcsData, sizeof(m_BcsData) ) )
				{
					m_BcsBlk	=	(__s32)A_BlkIdx;
				}
				else
				{
					m_BcsBlk	=	-1;
					m_DskStat	=	E_GrDvrRecStatBad;
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				}
			}
		}
		// update
		if ( E_GrDvrRecStatBad != m_DskStat )
		{
			// check current block
			if ( 0 <= m_BcsBlk )
			{
				// check empty
				if ( 0 == m_BcsData.Cnt )
				{
					Tv_TmIdx	=	0;
					Tv_TmMin	=	A_PtrInfo->TimeSt - (A_PtrInfo->TimeSt % E_GrTimeSecOfMin);
					m_BcsData.TimeTbl[Tv_TmIdx]	=	Tv_TmMin;
					m_BcsData.Cnt ++;
				}
				else
				{
					// check match time
					Tv_TmIdx	=	(__s32)m_BcsData.Cnt - 1;
					if ( (m_BcsData.TimeTbl[Tv_TmIdx] > A_PtrInfo->TimeSt) || ((m_BcsData.TimeTbl[Tv_TmIdx] + E_GrTimeSecOfMin) <= A_PtrInfo->TimeSt ) )
					{
						Tv_TmIdx	=	-1;
						// find exist match time
						if ( 1 < m_BcsData.Cnt )
						{
							for ( Tv_WkIdx=0;Tv_WkIdx < (m_BcsData.Cnt - 1);Tv_WkIdx++ )
							{
								if ( (m_BcsData.TimeTbl[Tv_WkIdx] <= A_PtrInfo->TimeSt) && ((m_BcsData.TimeTbl[Tv_WkIdx] + E_GrTimeSecOfMin) > A_PtrInfo->TimeSt ) )
								{
									// found
									Tv_TmIdx	=	(__s32)Tv_WkIdx;
									break;
								}
							}
						}
						// new allocate
						if ( 0 > Tv_TmIdx )
						{
							if ( E_GrDvrFs3BcsMaxCnt > m_BcsData.Cnt )
							{
								Tv_TmIdx	=	(__s32)m_BcsData.Cnt;
								m_BcsData.Cnt ++;
								Tv_TmMin	=	A_PtrInfo->TimeSt - (A_PtrInfo->TimeSt % E_GrTimeSecOfMin);
								m_BcsData.TimeTbl[Tv_TmIdx]	=	Tv_TmMin;
							}
							else
							{
								DbgMsgPrint( "Cls_GrDvrFs3Disk::LcBcsUpdt - Channel size overflow.\n" );
							}
						}
					}
				}
				// update size
				if ( 0 <= Tv_TmIdx )
				{
					for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
					{
						m_BcsData.SizeTbl[Tv_TmIdx][Tv_WkIdx]	=	m_BcsData.SizeTbl[Tv_TmIdx][Tv_WkIdx] + A_PtrInfo->ChSize[Tv_WkIdx];
					}
					// write
					LcBcsWrite();
				}

			}	//	if ( 0 <= m_BcsBlk )
		}	//	if ( E_GrDvrRecStatBad != m_DskStat )

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LogAdd( Ptr_GrDvrLogV2Item A_PtrLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_LogIdx;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecStatBad != m_DskStat )
		{
			// write item
			A_PtrLog->IsAble	=	TRUE;		// patch able

			Tv_LogIdx	=	m_Rca.EvtInIdx;
			Tv_Dpos		=	(__u64)( E_GrDvrFs3PosEla + (Tv_LogIdx << E_GrDvrLogShiftSize) );
			if ( LcPwrite( Tv_Dpos, A_PtrLog, sizeof(St_GrDvrLogV2Item) ) )
			{
				Tv_LogIdx ++;
				if ( (m_Mis.ElaSize >> E_GrDvrLogShiftSize) <= Tv_LogIdx )
				{
					Tv_LogIdx	=	0;
					m_Rca.IsEvtOvwt	=	TRUE;
				}
				// update info
				m_Rca.EvtInIdx	=	Tv_LogIdx;
				// RCA update
				LcRcaUpdt();
				// success
				Tv_Result	=	TRUE;
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				DbgMsgPrint( "Cls_GrDvrFs3Disk::LogAdd - bad sector appear!.\n" );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::LogGetNext( Ptr_GrDvrLogV2Ctrl A_PtrCtrl )
{
	// local -------------------
		__u64	Tv_DskPos;
		__u32	Tv_Remain;
		__u32	Tv_ReadCnt;
		__u32	Tv_ReadSize;
		BOOL8	Tv_IsOk;
		Ptr_GrDvrLogV2Item	Tv_PtrLogTbl;
		__u32	Tv_WkIdx;
	// code --------------------
		// check disk status
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			// init
			Tv_DskPos		=	E_GrDvrFs3PosEla;
			Tv_PtrLogTbl	=	(Ptr_GrDvrLogV2Item)m_PtrTmpBuf;
			Tv_Remain		=	m_Mis.ElaSize >> E_GrDvrLogShiftSize;

			while ( 0 != Tv_Remain )
			{
				Tv_IsOk	=	FALSE;
				Tv_ReadCnt	=	E_GrDvrFs3TmpBufSize >> E_GrDvrLogShiftSize;
				if ( Tv_ReadCnt > Tv_Remain )
				{
					Tv_ReadCnt	=	Tv_Remain;
				}
				Tv_ReadSize	=	Tv_ReadCnt << E_GrDvrLogShiftSize;
				if ( LcPread( Tv_DskPos, Tv_PtrLogTbl, Tv_ReadSize ) )
				{
					Tv_IsOk	=	TRUE;	// success
					// parsing
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ReadCnt;Tv_WkIdx++ )
					{
						if ( 0 != Tv_PtrLogTbl[Tv_WkIdx].Time )
						{
							GrDvrLogV2GetFw( A_PtrCtrl, &Tv_PtrLogTbl[Tv_WkIdx] );
						}
					}
				}
				// check bad
				if ( !Tv_IsOk )
				{
					// set bad disk
					m_DskStat	=	E_GrDvrRecStatBad;
					// notify
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
					break;
				}
				// next
				Tv_Remain	=	Tv_Remain - Tv_ReadCnt;
				Tv_DskPos	=	Tv_DskPos + (__u64)Tv_ReadSize;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::LogGetPrev( Ptr_GrDvrLogV2Ctrl A_PtrCtrl )
{
	// local -------------------
		__u64	Tv_DskPos;
		__u32	Tv_Remain;
		__u32	Tv_ReadCnt;
		__u32	Tv_ReadSize;
		BOOL8	Tv_IsOk;
		Ptr_GrDvrLogV2Item	Tv_PtrLogTbl;
		__u32	Tv_WkIdx;
	// code --------------------
		// check disk status
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			// init
			Tv_DskPos		=	E_GrDvrFs3PosEla;
			Tv_PtrLogTbl	=	(Ptr_GrDvrLogV2Item)m_PtrTmpBuf;
			Tv_Remain		=	m_Mis.ElaSize >> E_GrDvrLogShiftSize;

			while ( 0 != Tv_Remain )
			{
				Tv_IsOk	=	FALSE;
				Tv_ReadCnt	=	E_GrDvrFs3TmpBufSize >> E_GrDvrLogShiftSize;
				if ( Tv_ReadCnt > Tv_Remain )
				{
					Tv_ReadCnt	=	Tv_Remain;
				}
				Tv_ReadSize	=	Tv_ReadCnt << E_GrDvrLogShiftSize;
				if ( LcPread( Tv_DskPos, Tv_PtrLogTbl, Tv_ReadSize ) )
				{
					Tv_IsOk	=	TRUE;	// success
					// parsing
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ReadCnt;Tv_WkIdx++ )
					{
						if ( 0 != Tv_PtrLogTbl[Tv_WkIdx].Time )
						{
							GrDvrLogV2GetBw( A_PtrCtrl, &Tv_PtrLogTbl[Tv_WkIdx] );
						}
					}
				}
				// check bad
				if ( !Tv_IsOk )
				{
					// set bad disk
					m_DskStat	=	E_GrDvrRecStatBad;
					// notify
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
					break;
				}
				// next
				Tv_Remain	=	Tv_Remain - Tv_ReadCnt;
				Tv_DskPos	=	Tv_DskPos + (__u64)Tv_ReadSize;
			}
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::LcSmartRead( void )
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
BOOL8	Cls_GrDvrFs3Disk::LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_BytePos;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_BytePos	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs3DiskSmartValCnt;Tv_WkIdx++ )
		{
			if ( A_Id == m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFs3DiskSmartValIdxId] )
			{
				*A_PtrRtVal	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFs3DiskSmartValIdxRawVal];
				Tv_Result	=	TRUE;
				break;
			}
			//next
			Tv_BytePos	=	Tv_BytePos + E_GrDvrFs3DiskSmartValItemSize;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs3Disk::TempratureRead( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( LcSmartRead() )
		{
			LcSmartFindRawByte( E_GrDvrFs3DiskSmartIdTemp, &Tv_Result );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Disk::LcBcsRecSizeScan( Ptr_GrDvrFs3RecSizeReq A_PtrReq )
{
	// local -------------------
		__u64	Tv_RecSize;
		Ptr_GrDvrFs3Bcs	Tv_PtrBcs;
		__u32	Tv_Pos;
		__u32	Tv_WkIdx;
		__u16	TV_ChIdx;
		__u16	Tv_Mask;
	// code --------------------
		// check over Block index
		if ( m_Mis.BlkCnt > A_PtrReq->BcsIdx )
		{
			// check exist BCS
			if ( (0 != m_Bdt[A_PtrReq->BcsIdx].RecSize) && (!m_Bdt[A_PtrReq->BcsIdx].IsBad) )
			{
				// check time range
				if (	((m_Bdt[A_PtrReq->BcsIdx].TimeSt <= A_PtrReq->TimeSt) && (m_Bdt[A_PtrReq->BcsIdx].TimeEd >= A_PtrReq->TimeEd)) ||
							( (A_PtrReq->TimeSt <= m_Bdt[A_PtrReq->BcsIdx].TimeSt) && (A_PtrReq->TimeEd >= m_Bdt[A_PtrReq->BcsIdx].TimeSt)) ||
							( (A_PtrReq->TimeSt <= m_Bdt[A_PtrReq->BcsIdx].TimeEd) && (A_PtrReq->TimeEd >= m_Bdt[A_PtrReq->BcsIdx].TimeEd))
						)
				{
					// load BCS
					Tv_PtrBcs	=	(Ptr_GrDvrFs3Bcs)m_PtrTmpBuf;
					Tv_Pos		=	E_GrDvrFs3PosBcs + ( E_GrDvrFs3BcsUnitSize * (__u32)A_PtrReq->BcsIdx );
					if ( LcPread( Tv_Pos, Tv_PtrBcs, sizeof(St_GrDvrFs3Bcs) ) )
					{
						if ( (0 != Tv_PtrBcs->Cnt) && (E_GrDvrFs3BcsMaxCnt > Tv_PtrBcs->Cnt) )
						{
							Tv_RecSize	=	0;
							for ( Tv_WkIdx=0;Tv_WkIdx < Tv_PtrBcs->Cnt;Tv_WkIdx++ )
							{
								// check time
								if ( (A_PtrReq->TimeSt <= Tv_PtrBcs->TimeTbl[Tv_WkIdx]) && (A_PtrReq->TimeEd >= Tv_PtrBcs->TimeTbl[Tv_WkIdx]) )
								{
									Tv_Mask	=	1;
									for ( TV_ChIdx=0;TV_ChIdx < E_GrDvrMaxChCnt;TV_ChIdx++ )
									{
										if ( 0 != (Tv_Mask & A_PtrReq->ChMask) )
										{
											// add size
											Tv_RecSize	=	Tv_RecSize + (__u64)Tv_PtrBcs->SizeTbl[Tv_WkIdx][TV_ChIdx];
										}
										// next
										Tv_Mask	=	Tv_Mask << 1;
									}
								}
							}
							// update size
							A_PtrReq->Rslt	=	A_PtrReq->Rslt + Tv_RecSize;
						}
						else
						{
							DbgMsgPrint( "Cls_GrDvrFs3Disk::LcBcsRecSizeScan - bad BCS count %d.\n", Tv_PtrBcs->Cnt );
						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Disk::ScanRecSize( Ptr_GrDvrFs3RecSizeReq A_PtrReq )
{
	// local -------------------
		BOOL8	Tv_Result;	// done
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs3RecSizeWorkUnit;Tv_WkIdx++ )
		{
			// check block over
			if ( m_Mis.BlkCnt <= A_PtrReq->BcsIdx )
			{
				Tv_Result	=	TRUE;
				break;
			}
			// try read
			LcBcsRecSizeScan( A_PtrReq );
			// next
			A_PtrReq->BcsIdx ++;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs3Disk::LcLocalHndlByRdaPos( __u64 A_RdaPos )
{
	// local -------------------
		__u16	Tv_OvwtId;
		__u32	Tv_BlkId;
	// code --------------------
		Tv_BlkId	=	(__u32)(A_RdaPos >> E_GrDvrFs3BlockShift);
		Tv_OvwtId	=	m_Bdt[Tv_BlkId].OvwtId;
		return	A_RdaPos | ((__u64)Tv_OvwtId << E_GrDvrFs3HndlOvwtIdPos) | E_GrDvrFs3HndlMarkBit;
}
//--------------------------------------------------------------------
