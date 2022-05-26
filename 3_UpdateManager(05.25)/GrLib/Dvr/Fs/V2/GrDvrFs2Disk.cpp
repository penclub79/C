/*
 GAUSE file system version 2 disk I/O

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

#include <Dvr/Fs/V2/GrDvrFs2Base.h>
#include <Dvr/Fs/V2/GrDvrFs2Disk.h>

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

#define E_GrDvrFs1Fcc									Mac_GrMakeFcc('F','S','D','1')


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs2Disk::Cls_GrDvrFs2Disk( __u8 A_DevIdx, void* A_PtrTmpBuf, __u64 A_DvrId )
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
		m_GrpIntIdx			=	E_GrDvrFs2GrpIntNone;

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

		m_IsNeedCvt			=	FALSE;

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
		if ( 512 != sizeof(St_GrDvrFs2SmartData) )
		{
			DbgMsgPrint( "Cls_GrDvrFs2Disk: Smart struct size ( %d ) mismatch. \n", sizeof(St_GrDvrFs2SmartData) );
		}

		if ( GrDvrBiosRecDevNameGet( m_DevIdx, Tv_StrDev ) )
		{
#ifdef LINUX_APP
			m_HndlDsk	=	open( Tv_StrDev, O_RDWR  );
			if ( 0 <= m_HndlDsk )
			{
				DbgMsgPrint( "Cls_GrDvrFs2Disk: device ( %d ) opened. \n", m_DevIdx );
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
					DbgMsgPrint( "Cls_GrDvrFs2Disk: device ( %d ) get size error. \n", m_DevIdx );
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
			if ( E_GrDvrFs2DiskMinSize <= m_DskSize )
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
Cls_GrDvrFs2Disk::~Cls_GrDvrFs2Disk()
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
BOOL8	Cls_GrDvrFs2Disk::LcSeek( __s64 A_Pos, BOOL8 A_IsBegin )
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
				//check HDD status
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
BOOL8	Cls_GrDvrFs2Disk::LcRead( void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFs2Disk::LcWrite( void* A_PtrBuf, __u32 A_Size )
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
void	Cls_GrDvrFs2Disk::LcFlush( void )
{
#ifdef LINUX_APP
		if ( -1 != m_HndlDsk )
		{
			fsync( m_HndlDsk );
		}
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::Format( BOOL8 A_IsWithLog )
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
		if ( (E_GrDvrFs2DiskMinSize > m_DskSize) )
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
			m_Mis.FsFcc			=	E_GrDvrFs2Fcc;
			m_Mis.FsVer			=	E_GrDvrFs2Ver;
			m_Mis.ElaSize		=	E_GrDvrFs2ElaDfltSize;
			m_Mis.VsaSize		=	E_GrDvrFs2VsaDfltSize;
			if ( E_GrDvrFs2DiskMaxSize <= m_DskSize  )
			{
				m_Mis.BlkCnt		=	(__u32)((E_GrDvrFs2DiskMaxSize - E_GrDvrFs2PosRda) >> E_GrDvrFs2BlockShift);
			}
			else
			{
				m_Mis.BlkCnt		=	(__u32)((m_DskSize - E_GrDvrFs2PosRda) >> E_GrDvrFs2BlockShift);
			}
			
			if ( LcSeek( E_GrDvrFs2PosMis ) )
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
				Tv_BkElaIn			=	m_Rca.EvtPosIn;
				Tv_IsBkElaOvwt	=	m_Rca.IsEvtOvwt;
			}
			GrDumyZeroMem( &m_Rca, sizeof(m_Rca) );
			if ( !Tv_IsWithLog )
			{
				m_Rca.EvtPosIn	=	Tv_BkElaIn;
				m_Rca.IsEvtOvwt	=	Tv_IsBkElaOvwt;
			}
			// update DVR id
			m_Rca.DvrId	=	m_DvrId;

			// write
			if ( LcSeek( E_GrDvrFs2PosRca ) )
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
				Tv_Result	=	LcFillByte( E_GrDvrFs2PosEla, E_GrDvrFs2ElaDfltSize, 0 );
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
			Tv_Result	=	LcFillByte( E_GrDvrFs2PosBdt, sizeof(m_Bdt), 0 );
		}

		// VSS
		if ( Tv_Result )
		{
			Tv_Result	=	VssReset();
		}

		// init
		m_HndlLastRec		=	E_GrDvrRecHndlNone;
		m_ThumbTime			=	0;
		m_ThumbDmdIdx		=	-1;
		m_ThumbMmdIdx		=	0;
		m_RecTimeBegin	=	0;
		m_RecTimeEnd		=	0;

		if ( Tv_Result )
		{
			m_DskStat	=	E_GrDvrRecStatEmpty;
			LcNormalStatUpdt();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Disk::LcInit( void )
{
	// local -------------------
		BOOL8	Tv_IsAble;
		__u64	Tv_DskSize;
		__u64	Tv_Dpos;
		St_GrDvrFs2V1Mca	Tv_Mca;
	// code --------------------
		m_DskStat	=	E_GrDvrRecStatUnformated;
		Tv_IsAble	=	TRUE;

		// load mis
		if ( LcPread( E_GrDvrFs2PosMis, &m_Mis, sizeof(m_Mis) ) )
		{
			Tv_IsAble	=	FALSE;
			if ( (E_GrDvrFs2Fcc == m_Mis.FsFcc) && (E_GrDvrFs2Ver == m_Mis.FsVer) )
			{
				// check BDT count
				if ( E_GrDvrFs2BdtMaxCnt >= m_Mis.BlkCnt )
				{
					// check event log size
					if ( (E_GrDvrFs2ElaMinSize <= m_Mis.ElaSize) && (E_GrDvrFs2ElaMaxSize >= m_Mis.ElaSize) )
					{
						// check event log size fix
						if ( 0 != ( (~E_GrDvrLogAlignMask) & m_Mis.ElaSize ) )
						{
							DbgMsgPrint( "Cls_GrDvrFs2Disk::LcInit - ELA size align bad. do fix... \n" );
							m_Mis.ElaSize	=	m_Mis.ElaSize & E_GrDvrLogAlignMask;
						}
						// check VSA size
						if ( (E_GrDvrFs2VsaMinSize <= m_Mis.VsaSize) && (E_GrDvrFs2VsaMaxSize >= m_Mis.VsaSize) )
						{
							// check RDA size
							if ( (2 <= m_Mis.BlkCnt) && (E_GrDvrFs2BdtMaxCnt >= m_Mis.BlkCnt) )
							{
								Tv_DskSize	=	E_GrDvrFs2PosRda + ((__u64)m_Mis.BlkCnt * E_GrDvrFs2BlockSize);
								if ( Tv_DskSize <= m_DskSize )
								{
									Tv_IsAble	=	TRUE;
								}
								else
								{
									DbgMsgPrint( "Cls_GrDvrFs2Disk::LcInit - record area bigger than disksize.\n" );
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrDvrFs2Disk::LcInit - BDT block count ( %d ) is bad.\n", m_Mis.BlkCnt );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrDvrFs2Disk::LcInit - VSA size bad.\n" );
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrDvrFs2Disk::LcInit - ELA bad.\n" );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrDvrFs2Disk::LcInit - Block count ( %d ) over.\n", m_Mis.BlkCnt );
				}
			}
			else if ( E_GrDvrFs1Fcc == m_Mis.FsFcc )
			{
				if ( E_GrDvrFs2PosRda == m_Mis.V1RdaPos )
				{
					m_IsNeedCvt	=	TRUE;
					m_DskStat		=	E_GrDvrRecStatDefective;
					DbgMsgPrint( "Cls_GrDvrFs2Disk::LcInit - V1 type disk, need upgrade.\n" );
				}
				else
				{
					DbgMsgPrint( "Cls_GrDvrFs2Disk::LcInit - bad V1 type disk,.\n" );
				}
			}
		}
		else
		{
			m_DskStat	=	E_GrDvrRecStatBad;
			GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
		}

		// load V1 RCA
		if ( (E_GrDvrRecStatDefective == m_DskStat) && m_IsNeedCvt )
		{
			// read MCA
			if ( LcPread( E_GrDvrFs2PosV1Mca, &Tv_Mca, sizeof(Tv_Mca) ) )
			{
				// read V1 RCA
				Tv_Dpos	=	(__u64)(E_GrDvrFs2PosV1Rca + (E_GrDvrFs2V1RcaItmSize * (__u32)Tv_Mca.RcaIdx));
				if ( !LcPread( Tv_Dpos, &m_V1Rca, sizeof(m_V1Rca) ) )
				{
					m_IsNeedCvt	=	FALSE;
					m_DskStat		=	E_GrDvrRecStatBad;
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				}
			}
			else
			{
				m_IsNeedCvt	=	FALSE;
				m_DskStat		=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
			}
		}
		 
		// load RCA
		if ( Tv_IsAble )
		{
			Tv_IsAble	=	FALSE;
			// check loaded
			if ( LcPread( E_GrDvrFs2PosRca, &m_Rca, sizeof(m_Rca) ) )
			{
				Tv_IsAble	=	FALSE;
				// check DVR id
				if ( m_Rca.DvrId != m_DvrId )
				{
					// m_DskStat	=	E_GrDvrRecStatDefective;
					DbgMsgPrint( "Cls_GrDvrFs2Disk::LcInit - disk %d invalid DVR id.\n", m_DevIdx );
				}
				// event log position
				if ( m_Mis.ElaSize > m_Rca.EvtPosIn )
				{
					// success
					Tv_IsAble	=	TRUE;
					// check event log pointer align
					if ( 0 != ( (~E_GrDvrLogAlignMask) & m_Rca.EvtPosIn ) )
					{
						DbgMsgPrint( "Cls_GrDvrFs2Disk::LcInit - event log pointer align bad.\n" );
						m_Rca.EvtPosIn	=	m_Rca.EvtPosIn & E_GrDvrLogAlignMask;	// patch
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrDvrFs2Disk::LcInit - ELA item pointer bad.\n" );
				}
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
			}
		}

		// load DMD
		if ( Tv_IsAble )
		{
			Tv_IsAble	=	FALSE;
			if ( LcPread( E_GrDvrFs2PosDmd, m_Dmd, sizeof(m_Dmd) ) )
			{
				// check DMD time
				LcDmdCheck();
				Tv_IsAble	=	TRUE;
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
			}
		}

		// check BDT
		if ( Tv_IsAble )
		{
			Tv_IsAble	=	FALSE;
			if ( LcPread( E_GrDvrFs2PosBdt, m_Bdt, sizeof(m_Bdt) ) )
			{
				Tv_IsAble	=	TRUE;
				if ( !LcBdtChkIntra() )
				{
					m_DskStat	=	E_GrDvrRecStatDefective;
				}
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
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
			LcTimeBeginUpdt();
			LcTimeEndUpdt();
		}
		
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs2Disk::RecLastHndlGet( void )
{
	// local -------------------
	// code --------------------
		return	m_HndlLastRec;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::IsValidHndl( __u64 A_Hndl )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_OvwtId;
		__u32	Tv_BlkId;
		__u64	Tv_DposHndl;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_DposHndl	=	A_Hndl & E_GrDvrFs2HndlPosMask;
		Tv_OvwtId		=	(__u16)(A_Hndl >> E_GrDvrFs2HndlOvwtIdPos);
		// get block id
		Tv_BlkId		=	(__u32)((Tv_DposHndl - E_GrDvrFs2PosRda) >> E_GrDvrFs2BlockShift);
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
BOOL8	Cls_GrDvrFs2Disk::LcThumbWrite( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Pos;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( 0 != m_ThumbTime )
		{
			// write MMD
			// calculate position
			Tv_Pos	=	E_GrDvrFs2PosMmd + (__u64)( m_ThumbDmdIdx * E_GrDvrFs2MmdDaySize ) + (__u64)( m_ThumbMmdIdx * sizeof(St_GrDvrRecMinThumb) );
			if ( LcSeek( Tv_Pos ) )
			{
				if ( LcWrite( &m_ThumbData, sizeof(m_ThumbData) ) )
				{
					Tv_Result	=	TRUE;
					// write RCA
					LcRcaWrite();
				}
				else
				{
					m_DskStat	=	E_GrDvrRecStatBad;
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				}
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
BOOL8	Cls_GrDvrFs2Disk::LcThumbUpdt( Ptr_GrDvrFs2BlkAddInfo A_PtrInfo )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsDayChg;
		BOOL8	Tv_IsBad;
		__u32	Tv_ThumbDay;
		__u64	Tv_Pos;
		__u8	Tv_ThumbEvt;
		__u8	Tv_WkIdx;
		__u8	Tv_WkCh;
		__u8	Tv_Tmp;
	// code --------------------
		Tv_Result			=	TRUE;
		Tv_IsDayChg		=	TRUE;

		// check time
		if ( 0 != m_ThumbTime )
		{
			if ( (m_ThumbTime <= A_PtrInfo->TimeSt) && ((m_ThumbTime + E_GrTimeSecOfMin) > A_PtrInfo->TimeSt) )
			{
				Tv_Result		=	FALSE;
				Tv_IsDayChg	=	FALSE;
			}
		}

		// update previous table
		if ( Tv_Result )
		{
			// flush previous time
			LcThumbWrite();
		}

		// DMD index recalculate
		if ( Tv_Result && (E_GrDvrRecStatBad != m_DskStat) )
		{
			// init
			Tv_ThumbDay		=	A_PtrInfo->TimeSt - (A_PtrInfo->TimeSt % E_GrTimeSecOfDay);
			// new MMD calculate
			m_ThumbMmdIdx	=	(A_PtrInfo->TimeSt - Tv_ThumbDay) / E_GrTimeSecOfMin;
			// check day change
			if ( 0 != m_ThumbTime )
			{
				if ( Tv_ThumbDay == m_Dmd[m_ThumbDmdIdx] )
				{
					Tv_IsDayChg	=	FALSE;
				}
			}

			// get day index
			if ( Tv_IsDayChg )
			{
				// find current day
				m_ThumbDmdIdx	=	LcDmdFind( Tv_ThumbDay );
				if ( 0 > m_ThumbDmdIdx )
				{
					// not found, create new DMD
					m_ThumbDmdIdx	=	LcDmdAlloc();
					if ( (0 <= m_ThumbDmdIdx) && (E_GrDvrRecStatBad != m_DskStat) )
					{
						// change DMD
						m_Dmd[m_ThumbDmdIdx]	=	Tv_ThumbDay;
						LcDmdWrite();
					}
				}
			}
		}

		// load thumb data
		if ( Tv_Result && ( 0 <= m_ThumbDmdIdx) && (E_GrDvrRecStatBad != m_DskStat) )
		{
			Tv_IsBad	=	TRUE;
			Tv_Pos	=	E_GrDvrFs2PosMmd + (__u64)( m_ThumbDmdIdx * E_GrDvrFs2MmdDaySize ) + (__u64)( m_ThumbMmdIdx * sizeof(St_GrDvrRecMinThumb) );
			if ( LcSeek( Tv_Pos ) )
			{
				if ( LcRead( &m_ThumbData, sizeof(m_ThumbData) ) )
				{
					Tv_IsBad	=	FALSE;
					// update thumb time
					m_ThumbTime	=	m_Dmd[m_ThumbDmdIdx] + (m_ThumbMmdIdx * E_GrTimeSecOfMin);
				}
			}
			if ( Tv_IsBad && (E_GrDvrRecStatBad != m_DskStat) )
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
			}
			// load so do not change
			Tv_Result	=	FALSE;
		}
		
		// update data
		if ( ( 0 <= m_ThumbDmdIdx) && (E_GrDvrRecStatBad != m_DskStat) )
		{
			// check handle
			if ( E_GrDvrRecHndlNone == m_ThumbData.HndlRec )
			{
				m_ThumbData.HndlRec	=	m_HndlLastRec;
				Tv_Result	=	TRUE;
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
					Tv_Result	=	TRUE;
				}
				// next
				Tv_WkCh			=	Tv_WkCh + 2;
			}

			// check changed
			if ( Tv_Result )
			{
				LcThumbWrite();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Disk::OverwriteSet( BOOL8 A_IsOvwt )
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
__u8	Cls_GrDvrFs2Disk::DevIdxGet( void )
{
		return	m_DevIdx;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::PacketReplace( __u64 A_Hndl, Ptr_GrDvrAvHd A_PtrPkt )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsBad;
		__u32	Tv_BlkId;
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
				Tv_DskPos			=	A_Hndl & E_GrDvrFs2HndlPosMask;
				Tv_IsBad	=	TRUE;
				if ( LcSeek( Tv_DskPos ) )
				{
					if ( LcRead( &Tv_AvHd, sizeof(Tv_AvHd) ) )
					{
						Tv_IsBad	=	FALSE;

						// check header and reserved size
						if ( E_GrDvrFs2StrmFcc == Tv_AvHd.Fcc )
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
								DbgMsgPrint( "Cls_GrDvrFs2Disk::PacketReplace - reserved size mismatch.\n" );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrDvrFs2Disk::PacketReplace - bad reserved packet.\n" );
						}
					}
				}
				// check bad appear
				if ( Tv_IsBad )
				{
					// bad block appear
					Tv_BlkId	=	(__u32)( (Tv_DskPos - E_GrDvrFs2PosRda) >> E_GrDvrFs2BlockShift );
					m_Bdt[Tv_BlkId].IsBad	=	TRUE;
					LcBdtWriteByBlkId( Tv_BlkId );
					GrTaskSvrMsgErrorNotify( E_GrErrDiskBadSector, m_DevIdx );
					DbgMsgPrint( "Cls_GrDvrFs2Disk::PacketReplace - bad block (%d:%d) appear.\n", m_DevIdx, Tv_BlkId );
				}

			}	//	if ( IsValidHndl( A_Hndl ) )
			else
			{
				DbgMsgPrint( "Cls_GrDvrFs2Disk::PacketReplace - bad handle value.\n" );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs2Disk::DayMapGet( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_Pos;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2DmdItemCnt;Tv_WkIdx++ )
			{
				if ( (m_Dmd[Tv_WkIdx] >= A_TimeSt) && (m_Dmd[Tv_WkIdx] <= A_TimeEd) )
				{
					// found
					Tv_Pos		=	(m_Dmd[Tv_WkIdx] - A_TimeSt) / E_GrTimeSecOfDay;
					Tv_Result	=	Tv_Result | (1 << Tv_Pos);
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::MmdMapCombine( Def_GrTime A_TimeSt, void* A_PtrBuf, __u32 A_Cnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_GrTime	Tv_TimeDay;
		__s32		Tv_DmdIdx;
		__u8*	Tv_PtrTg;
		Ptr_GrDvrRecMinThumb	Tv_PtrSr;
		__u32	Tv_SrIdx;
		__u32	Tv_Cnt;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_TimeDay	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfDay);
		Tv_PtrSr		=	(Ptr_GrDvrRecMinThumb)m_PtrTmpBuf;

		// find DMD index
		Tv_DmdIdx		=	LcDmdFind( Tv_TimeDay );
		if ( 0 <= Tv_DmdIdx )
		{
			// load MMD to temporary buffer
			if ( LcMmdLoadByDmdIdx( (__u32)Tv_DmdIdx, Tv_PtrSr ) )
			{
				// calculate combine range
				Tv_SrIdx	=	(A_TimeSt - Tv_TimeDay) / E_GrTimeSecOfMin;
				Tv_Cnt		=	E_GrDvrFs2MmdDayItemCnt - Tv_SrIdx;
				if ( Tv_Cnt > A_Cnt )
				{
					Tv_Cnt	=	A_Cnt;
				}
				// combine
				Tv_PtrTg	=	(__u8*)A_PtrBuf;
				for ( Tv_WkIdx=0;Tv_WkIdx < Tv_Cnt;Tv_WkIdx++ )
				{
					// check combine
					if ( E_GrDvrRecHndlNone != Tv_PtrSr[Tv_SrIdx].HndlRec )
					{
						Tv_PtrTg[0]	=	Tv_PtrTg[0] | Tv_PtrSr[Tv_SrIdx].ChAtb[0];
						Tv_PtrTg[1]	=	Tv_PtrTg[1] | Tv_PtrSr[Tv_SrIdx].ChAtb[1];
						Tv_PtrTg[2]	=	Tv_PtrTg[2] | Tv_PtrSr[Tv_SrIdx].ChAtb[2];
						Tv_PtrTg[3]	=	Tv_PtrTg[3] | Tv_PtrSr[Tv_SrIdx].ChAtb[3];
						Tv_PtrTg[4]	=	Tv_PtrTg[4] | Tv_PtrSr[Tv_SrIdx].ChAtb[4];
						Tv_PtrTg[5]	=	Tv_PtrTg[5] | Tv_PtrSr[Tv_SrIdx].ChAtb[5];
						Tv_PtrTg[6]	=	Tv_PtrTg[6] | Tv_PtrSr[Tv_SrIdx].ChAtb[6];
						Tv_PtrTg[7]	=	Tv_PtrTg[7] | Tv_PtrSr[Tv_SrIdx].ChAtb[7];
					}
					// next
					Tv_SrIdx ++;
					Tv_PtrTg	=	Tv_PtrTg + E_GrDvrRecMinMapSize;
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
__u8	Cls_GrDvrFs2Disk::GrpIntIdxGet( void )
{
		return	m_GrpIntIdx;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Disk::GrpIntIdxSet( __u8 A_Idx )
{
		m_GrpIntIdx	=	A_Idx;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Disk::LcRcaWrite( void )
{
	// local -------------------
		BOOL8	Tv_IsOk;
	// code --------------------
		Tv_IsOk	=	FALSE;
		if ( LcSeek( E_GrDvrFs2PosRca ) )
		{
			if ( LcWrite( &m_Rca, sizeof(m_Rca)) )
			{
				Tv_IsOk	=	TRUE;
			}
		}
		// check bad
		if ( (!Tv_IsOk) && (E_GrDvrRecStatBad != m_DskStat) )
		{
			m_DskStat	=	E_GrDvrRecStatBad;
			DbgMsgPrint( "Cls_GrDvrFs2Disk::LcRcaWrite - RCA area bad!\n" );
			GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
		}

		// flush
		//LcFlush();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcMmdLoadByDmdIdx( __u32 A_DmdIdx, void* A_PtrBuf )
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
		if ( E_GrDvrFs2DmdItemCnt > A_DmdIdx )
		{
			// init
			Tv_IsBad	=	TRUE;
			Tv_Pos	=	(__u64)( E_GrDvrFs2PosMmd + ( A_DmdIdx * E_GrDvrFs2MmdDaySize ) );
			// load 
			if ( LcSeek( Tv_Pos ) )
			{
				if ( LcRead( A_PtrBuf, E_GrDvrFs2MmdDaySize ) )
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
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2MmdDayItemCnt;Tv_WkIdx++ )
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
						if ( LcWrite( A_PtrBuf, E_GrDvrFs2MmdDaySize ) )
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
			
		}	//	if ( E_GrDvrFs2DmdItemCnt > A_DmdIdx )

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Disk::LcDmdWrite( void )
{
	// local -------------------
		BOOL8	Tv_IsOk;
	// code --------------------
		Tv_IsOk	=	FALSE;

		if ( LcSeek( E_GrDvrFs2PosDmd ) )
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
__s32	Cls_GrDvrFs2Disk::LcMmdFindOldestThumb( __u32 A_DmdIdx, Ptr_GrDvrRecMinThumb A_PtrRtThumb )
{
	// local -------------------
		__s32	Tv_Result;
		__u64	Tv_Pos;
		Ptr_GrDvrRecMinThumb	Tv_PtrThumb;
		BOOL8	Tv_IsModified;
		__s32		Tv_MmdIdx;
	// code --------------------
		Tv_Result	=	-1;
		// check DMD index
		if ( E_GrDvrFs2DmdItemCnt > A_DmdIdx )
		{
			Tv_Pos				=	(__u64)( E_GrDvrFs2PosMmd + ( A_DmdIdx * E_GrDvrFs2MmdDaySize ) );
			if ( LcPread( Tv_Pos, m_PtrTmpBuf, E_GrDvrFs2MmdDaySize ) )
			{
				// check current handle and time
				Tv_IsModified	=	FALSE;
				Tv_PtrThumb	=	(Ptr_GrDvrRecMinThumb)m_PtrTmpBuf;
				for ( Tv_MmdIdx=0;Tv_MmdIdx < E_GrDvrFs2MmdDayItemCnt;Tv_MmdIdx++ )
				{
					if ( E_GrDvrRecHndlNone != Tv_PtrThumb->HndlRec )
					{
						if ( IsValidHndl( Tv_PtrThumb->HndlRec ) )
						{
							// update value
							Tv_Result	=	Tv_MmdIdx;

							A_PtrRtThumb->HndlRec		=	Tv_PtrThumb->HndlRec;
							A_PtrRtThumb->ChAtb[0]	=	Tv_PtrThumb->ChAtb[0];
							A_PtrRtThumb->ChAtb[1]	=	Tv_PtrThumb->ChAtb[1];
							A_PtrRtThumb->ChAtb[2]	=	Tv_PtrThumb->ChAtb[2];
							A_PtrRtThumb->ChAtb[3]	=	Tv_PtrThumb->ChAtb[3];
							A_PtrRtThumb->ChAtb[4]	=	Tv_PtrThumb->ChAtb[4];
							A_PtrRtThumb->ChAtb[5]	=	Tv_PtrThumb->ChAtb[5];
							A_PtrRtThumb->ChAtb[6]	=	Tv_PtrThumb->ChAtb[6];
							A_PtrRtThumb->ChAtb[7]	=	Tv_PtrThumb->ChAtb[7];
							break;
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
					if ( !LcPwrite( Tv_Pos, m_PtrTmpBuf, E_GrDvrFs2MmdDaySize ) )
					{
						m_DskStat	=	E_GrDvrRecStatBad;
						GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
						Tv_Result	=	-1;
					}
				}
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				Tv_Result	=	-1;
			}

		}	//	if ( E_GrDvrFs2DmdItemCnt > A_DmdIdx )
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Disk::LcMmdFindOldestThumb - bad DMD index.\n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFs2Disk::LcMmdFindNewestThumb( __u32 A_DmdIdx, Ptr_GrDvrRecMinThumb A_PtrRtThumb )
{
	// local -------------------
		__s32	Tv_Result;
		__u64	Tv_Pos;
		__u32	Tv_WkIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrThumb;
		BOOL8	Tv_IsModified;
		__s32		Tv_MmdIdx;
	// code --------------------
		Tv_Result	=	-1;
		// check DMD index
		if ( E_GrDvrFs2DmdItemCnt > A_DmdIdx )
		{
			Tv_Pos				=	(__u64)( E_GrDvrFs2PosMmd + ( A_DmdIdx * E_GrDvrFs2MmdDaySize ) );
			if ( LcPread( Tv_Pos, m_PtrTmpBuf, E_GrDvrFs2MmdDaySize ) )
			{
				// check current handle and time
				Tv_MmdIdx			=	E_GrDvrFs2MmdDayItemCnt;
				Tv_IsModified	=	FALSE;
				Tv_PtrThumb	=	(Ptr_GrDvrRecMinThumb)( (__u32)m_PtrTmpBuf + E_GrDvrFs2MmdDaySize);
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2MmdDayItemCnt;Tv_WkIdx++ )
				{
					// next
					Tv_PtrThumb --;
					Tv_MmdIdx --;
					// check
					if ( E_GrDvrRecHndlNone != Tv_PtrThumb->HndlRec )
					{
						if ( IsValidHndl( Tv_PtrThumb->HndlRec ) )
						{
							// update value
							Tv_Result	=	Tv_MmdIdx;

							A_PtrRtThumb->HndlRec		=	Tv_PtrThumb->HndlRec;
							A_PtrRtThumb->ChAtb[0]	=	Tv_PtrThumb->ChAtb[0];
							A_PtrRtThumb->ChAtb[1]	=	Tv_PtrThumb->ChAtb[1];
							A_PtrRtThumb->ChAtb[2]	=	Tv_PtrThumb->ChAtb[2];
							A_PtrRtThumb->ChAtb[3]	=	Tv_PtrThumb->ChAtb[3];
							A_PtrRtThumb->ChAtb[4]	=	Tv_PtrThumb->ChAtb[4];
							A_PtrRtThumb->ChAtb[5]	=	Tv_PtrThumb->ChAtb[5];
							A_PtrRtThumb->ChAtb[6]	=	Tv_PtrThumb->ChAtb[6];
							A_PtrRtThumb->ChAtb[7]	=	Tv_PtrThumb->ChAtb[7];
							break;
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
				}	//	for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ReadCnt;Tv_WkIdx++ )
				// check modified
				if ( Tv_IsModified )
				{
					if ( !LcPwrite( Tv_Pos, m_PtrTmpBuf, E_GrDvrFs2MmdDaySize ) )
					{
						m_DskStat	=	E_GrDvrRecStatBad;
						GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
						Tv_Result	=	-1;
					}
				}
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				Tv_Result	=	-1;
			}

		}	//	if ( E_GrDvrFs2DmdItemCnt > A_DmdIdx )
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs2Disk::LcMmdFindNewestThumb - bad DMD index.\n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Disk::LcDmdCheck( void )
{
	// local -------------------
		__u32	Tv_WkIdx;
		Def_GrTime	Tv_Remain;
		BOOL8	Tv_IsModified;
		__u32	Tv_ChkIdx;
	// code --------------------
		Tv_IsModified	=	FALSE;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2DmdItemCnt;Tv_WkIdx++ )
		{
			// check time exist
			if ( 0 != m_Dmd[Tv_WkIdx] )
			{
				Tv_Remain	=	m_Dmd[Tv_WkIdx] % E_GrTimeSecOfDay;
				if ( 0 != Tv_Remain )
				{
					DbgMsgPrint( "Cls_GrDvrFs2Disk::LcDmdCheck - Bad DMD time found. fix..\n" );
					m_Dmd[Tv_WkIdx]	=	m_Dmd[Tv_WkIdx] - Tv_Remain;
					Tv_IsModified	=	TRUE;
				}
			}
		}

		// check overlapped date
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2DmdItemCnt;Tv_WkIdx++ )
		{
			if ( 0 != m_Dmd[Tv_WkIdx] )
			{
				Tv_ChkIdx	=	Tv_WkIdx + 1;
				while ( E_GrDvrFs2DmdItemCnt > Tv_ChkIdx )
				{
					// check equal
					if ( m_Dmd[Tv_WkIdx] == m_Dmd[Tv_ChkIdx] )
					{
						// match DMD found
						DbgMsgPrint( "Cls_GrDvrFs2Disk::LcDmdCheck - overlapped date found!, remove overlapped date.\n" );
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
__u8	Cls_GrDvrFs2Disk::DiskStatGet( void )
{
		return	m_DskStat;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcElaDisable( void )
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
		Tv_DskPos	=	E_GrDvrFs2PosEla;
		Tv_PtrLogTbl	=	(Ptr_GrDvrLogV2Item)m_PtrTmpBuf;

		while ( 0 != Tv_Remain )
		{
			// calculate read write size
			Tv_RwSize	=	Tv_Remain;
			if ( E_GrDvrFs2TmpBufSize < Tv_RwSize )
			{
				Tv_RwSize	=	E_GrDvrFs2TmpBufSize;
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
__s32	Cls_GrDvrFs2Disk::LcDmdFind( Def_GrTime A_Day )
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for (Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2DmdItemCnt;Tv_WkIdx++)
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
__s32	Cls_GrDvrFs2Disk::LcDmdAlloc( void )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_WkIdx;
		BOOL8	Tv_IsOk;
		__u64	Tv_Pos;
	// code --------------------
		Tv_Result	=	-1;

		// find empty
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2DmdItemCnt;Tv_WkIdx++ )
		{
			if ( 0 == m_Dmd[Tv_WkIdx] )
			{
				// found
				Tv_Result	=	Tv_WkIdx;
				// MMD clear
				Tv_Pos		=	E_GrDvrFs2PosMmd + (__u64)( (__u32)Tv_WkIdx * E_GrDvrFs2MmdDaySize );
				LcFillByte( Tv_Pos, E_GrDvrFs2MmdDaySize, 0 );
				break;
			}
		}

		// check success
		if ( 0 > Tv_Result )
		{
			// garbage collection
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2DmdItemCnt;Tv_WkIdx++ )
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
__u64	Cls_GrDvrFs2Disk::HndlFromDpos( __u64 A_DiskPos )
{
	// local -------------------
		__u16	Tv_OvwtId;
		__u32	Tv_BlkId;
	// code --------------------
		Tv_BlkId	=	(__u32)((A_DiskPos - E_GrDvrFs2PosRda) >> E_GrDvrFs2BlockShift);
		Tv_OvwtId	=	m_Bdt[Tv_BlkId].OvwtId;
		return	A_DiskPos | (__u64)m_GrpIntIdx | ((__u64)Tv_OvwtId << E_GrDvrFs2HndlOvwtIdPos);
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs2Disk::BeginTimeGet( void )
{
		return	m_RecTimeBegin;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs2Disk::EndTimeGet( void )
{
		return	m_RecTimeEnd;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::RecSizeGet( __u64* A_PtrRtTotal, __u64* A_PtrRtRec )
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
__s32	Cls_GrDvrFs2Disk::LcDmdNearFw( Def_GrTime A_Time )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2DmdItemCnt;Tv_WkIdx++ )
		{
			if ( A_Time == m_Dmd[Tv_WkIdx] )
			{
				Tv_Result	=	Tv_WkIdx;
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
					}
				}
				else
				{
					Tv_Result	=	Tv_WkIdx;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs2Disk::RecDayFindFw( Def_GrTime A_TimeBase )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__s32		Tv_DmdIdx;
	// code --------------------
		Tv_Result	=	0;
		// find nearest DMD
		Tv_DmdIdx	=	LcDmdNearFw( A_TimeBase );
		if ( 0 <= Tv_DmdIdx )
		{
			Tv_Result	=	m_Dmd[Tv_DmdIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32		Cls_GrDvrFs2Disk::LcDmdNearBw( Def_GrTime A_Time )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2DmdItemCnt;Tv_WkIdx++ )
		{
			if ( A_Time == m_Dmd[Tv_WkIdx] )
			{
				Tv_Result	=	Tv_WkIdx;
				break;
			}
			else if ( A_Time > m_Dmd[Tv_WkIdx] )
			{
				// check found
				if ( 0 <= Tv_Result )
				{
					if ( m_Dmd[Tv_Result] < m_Dmd[Tv_WkIdx] )
					{
						// update
						Tv_Result	=	Tv_WkIdx;
					}
				}
				else
				{
					Tv_Result	=	Tv_WkIdx;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs2Disk::RecDayFindBw( Def_GrTime A_TimeBase )
{
	// local -------------------
		Def_GrTime	Tv_Result;
		__s32		Tv_DmdIdx;
	// code --------------------
		Tv_Result	=	0;
		// find nearest DMD
		Tv_DmdIdx	=	LcDmdNearBw( A_TimeBase );
		if ( 0 <= Tv_DmdIdx )
		{
			Tv_Result	=	m_Dmd[Tv_DmdIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcBdtChkIntra( void )
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
				if ( E_GrDvrFs2BlockSize < m_Bdt[Tv_ChkIdx].RecSize )
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
void	Cls_GrDvrFs2Disk::LcNormalStatUpdt( void )
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
					Tv_RecCap		=	Tv_RecCap + (__u64)E_GrDvrFs2BlockSize;
					Tv_RecSize	=	Tv_RecSize + (__u64)E_GrDvrFs2BlockSize;
				}
				else
				{
					if ( 0 != m_Bdt[Tv_BdtIdx].RecSize )
					{
						Tv_Stat	=	E_GrDvrRecStatNormal;
						Tv_RecSize	=	Tv_RecSize + (__u64)m_Bdt[Tv_BdtIdx].RecSize;
					}
					Tv_RecCap		=	Tv_RecCap + (__u64)E_GrDvrFs2BlockSize;
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
Ptr_GrDvrFs2Bdt	Cls_GrDvrFs2Disk::BdtTableGet( void )
{
		return	m_Bdt;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs2Disk::BlockCountGet( void )
{
		return	m_Mis.BlkCnt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::IsBdtOverlap( __u32 A_RecId )
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
BOOL8	Cls_GrDvrFs2Disk::IsBdtCrossCheck( Cls_GrDvrFs2Disk* A_Disk )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDvrFs2Bdt	Tv_PtrSr;
		__u32	Tv_SrBlkCnt;
		__u32	Tv_SrIdx;
		__u32	Tv_TgIdx;
	// code --------------------
		Tv_Result	=	TRUE;

		// check disk status
		if ( (E_GrDvrRecStatEmpty < m_DskStat) && (E_GrDvrRecStatEmpty < A_Disk->DiskStatGet()) )
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
BOOL8	Cls_GrDvrFs2Disk::BdtSave( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BdtSize;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecStatUnformated < m_DskStat )
		{
			if ( LcSeek( E_GrDvrFs2PosBdt ) )
			{
				Tv_BdtSize	=	m_Mis.BlkCnt * sizeof(St_GrDvrFs2Bdt);
				if ( LcWrite( m_Bdt, Tv_BdtSize ) )
				{
					Tv_Result	=	TRUE;
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
BOOL8	Cls_GrDvrFs2Disk::BlockEmptyFind( __u32* A_PtrRtIdx )
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
__u32	Cls_GrDvrFs2Disk::BlockFreeSizeGet( __u32 A_BlkIdx )
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
					Tv_Result	=	E_GrDvrFs2BlockSize - m_Bdt[A_BlkIdx].RecSize;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Disk::BlockFullSet( __u32 A_BlkIdx )
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
BOOL8	Cls_GrDvrFs2Disk::LcBdtWriteByBlkId( __u32 A_BlkId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Pos;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( A_BlkId < m_Mis.BlkCnt )
		{
			Tv_Pos	=	E_GrDvrFs2PosBdt + (__u64)( A_BlkId * sizeof(St_GrDvrFs2Bdt) );
			if ( LcSeek( Tv_Pos ) )
			{
				if ( LcWrite( &m_Bdt[A_BlkId], sizeof(St_GrDvrFs2Bdt) ) )
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
__u32	Cls_GrDvrFs2Disk::BlockMaxRecIdFind( __u32* A_PtrRtIdx )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	E_GrDvrFs2RecIdNone;

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
__u32	Cls_GrDvrFs2Disk::BlockMinRecIdFind( __u32* A_PtrRtIdx )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	E_GrDvrFs2RecIdNone;

		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			for ( Tv_BlkIdx=0;Tv_BlkIdx < m_Mis.BlkCnt;Tv_BlkIdx++ )
			{
				if ( (!m_Bdt[Tv_BlkIdx].IsBad) && (0 != m_Bdt[Tv_BlkIdx].RecSize) )
				{
					if ( E_GrDvrFs2RecIdNone != m_Bdt[Tv_BlkIdx].RecId )
					{
						if ( E_GrDvrFs2RecIdNone == Tv_Result)
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
						DbgMsgPrint( "Cls_GrDvrFs2Disk::BlockMinRecIdFind - bad recid block (%d:%d) found. delete.\n", m_DevIdx, Tv_BlkIdx );
						BlockDelete( Tv_BlkIdx );
						GrTaskSvrMsgErrorNotify( E_GrErrRecBadRecIdBlk,m_DevIdx,0,Tv_BlkIdx );
					}
					
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Disk::BlockDelete( __u32 A_BlkIdx )
{
	// local -------------------
	// code --------------------
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			if ( A_BlkIdx < m_Mis.BlkCnt )
			{
#ifdef E_GrDvrFs2DiskDbgMsgView
				DbgMsgPrint( "Cls_GrDvrFs2Disk::BlockDelete - disk(%d) block(%d) recid(%d) \n", m_DevIdx, A_BlkIdx, m_Bdt[A_BlkIdx].RecId );
#endif
				m_Bdt[A_BlkIdx].IsFull		=	FALSE;
				m_Bdt[A_BlkIdx].OvwtId ++;
				m_Bdt[A_BlkIdx].RecId		=	E_GrDvrFs2RecIdNone;
				m_Bdt[A_BlkIdx].RecSize	=	0;
				m_Bdt[A_BlkIdx].TimeSt		=	0;
				m_Bdt[A_BlkIdx].TimeEd		=	0;
				LcBdtWriteByBlkId( A_BlkIdx );
				// update time range
				LcTimeBeginUpdt();
				// update status
				LcNormalStatUpdt();
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::BlockDataAdd( __u32 A_BlkId, __u32 A_RecId, void* A_PtrBuf, __u32 A_Size, 
	Ptr_GrDvrFs2BlkAddInfo A_PtrInfo, BOOL8 A_IsBdtUpdt )
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
							Tv_FreeSize	=	E_GrDvrFs2BlockSize - m_Bdt[A_BlkId].RecSize;
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
										DbgMsgPrint( "Cls_GrDvrFs2Disk::BlockDataAdd - bad time input.\n" );
									}

									// update record id
									Tv_IsBdtWrite	=	A_IsBdtUpdt;
									if ( E_GrDvrFs2RecIdNone == m_Bdt[A_BlkId].RecId )
									{
										m_Bdt[A_BlkId].RecId	=	A_RecId;
										Tv_IsBdtWrite	=	TRUE;
									}

									// check full
									if ( E_GrDvrFs2BlockSize <= m_Bdt[A_BlkId].RecSize )
									{
										m_Bdt[A_BlkId].IsFull	=	TRUE;
										Tv_IsBdtWrite	=	TRUE;
									}

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
								DbgMsgPrint( "Cls_GrDvrFs2Disk::BlockDataAdd - Expect size miss (%d - > %d).\n",A_Size, Tv_FreeSize );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrDvrFs2Disk::BlockDataAdd - Block is full (%d:%d).\n",m_DevIdx, A_BlkId );
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrDvrFs2Disk::BlockDataAdd - Bad block (%d:%d).\n",m_DevIdx, A_BlkId );
					}
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrDvrFs2Disk::BlockDataAdd - bad block id (%d:%d).\n",m_DevIdx, A_BlkId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcBlockWrite( __u32 A_BlkId, __u32 A_Ofs, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsLhndlUpdt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Dpos	=	E_GrDvrFs2PosRda + ( (__u64)A_BlkId << E_GrDvrFs2BlockShift ) + (__u64)A_Ofs;
		if ( LcSeek( Tv_Dpos ) )
		{
			if ( LcWrite( A_PtrBuf, A_Size ) )
			{
				Tv_Result	=	TRUE;
				// update last handle
				if ( A_IsLhndlUpdt )
				{
					m_HndlLastRec	=	HndlFromDpos( Tv_Dpos );
				}
			}
		}
		if ( !Tv_Result )
		{
			m_Bdt[A_BlkId].IsBad	=	TRUE;
			DbgMsgPrint( "Cls_GrDvrFs2Disk::LcBlockWrite - bad block (%d:%d) appear.\n", m_DevIdx, A_BlkId );
			LcBdtWriteByBlkId( A_BlkId );
			GrTaskSvrMsgErrorNotify( E_GrErrDiskBadSector, m_DevIdx );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::BlockInfoGet( __u32 A_BlkId, __u32* A_PtrRtRecId, __u32* A_PtrRtDataSize, __u16* A_PtrRtOvwtId )
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
__u32	Cls_GrDvrFs2Disk::BlockFwRecIdFind( __u32 A_RecId, __u32* A_PtrRtBlkId, __u16* A_PtrRtOvwtId, __u32* A_PtrRtRecSize )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkId;
	// code --------------------
		Tv_Result	=	E_GrDvrFs2RecIdNone;

		if ( (E_GrDvrRecStatEmpty < m_DskStat) && (E_GrDvrFs2RecIdNone != A_RecId) )
		{
			for ( Tv_BlkId=0;Tv_BlkId < m_Mis.BlkCnt;Tv_BlkId++ )
			{
				// check able
				if ( (!m_Bdt[Tv_BlkId].IsBad) && (0 != m_Bdt[Tv_BlkId].RecSize) )
				{
					if ( E_GrDvrFs2RecIdNone != m_Bdt[Tv_BlkId].RecId )
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
							if ( E_GrDvrFs2RecIdNone == Tv_Result )
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
						DbgMsgPrint( "Cls_GrDvrFs2Disk::BlockFwRecIdFind - bad RecId found on block %d:%d!\n", m_DevIdx, Tv_BlkId );
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs2Disk::BlockBwRecIdFind( __u32 A_RecId, __u32* A_PtrRtBlkId, __u16* A_PtrRtOvwtId, __u32* A_PtrRtRecSize )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkId;
	// code --------------------
		Tv_Result	=	E_GrDvrFs2RecIdNone;

		if ( (E_GrDvrRecStatEmpty < m_DskStat) && (E_GrDvrFs2RecIdNone != A_RecId) )
		{
			for ( Tv_BlkId=0;Tv_BlkId < m_Mis.BlkCnt;Tv_BlkId++ )
			{
				// check able
				if ( (!m_Bdt[Tv_BlkId].IsBad) && (0 != m_Bdt[Tv_BlkId].RecSize) )
				{
					if ( E_GrDvrFs2RecIdNone != m_Bdt[Tv_BlkId].RecId )
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
							if ( E_GrDvrFs2RecIdNone == Tv_Result )
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
						DbgMsgPrint( "Cls_GrDvrFs2Disk::BlockBwRecIdFind - bad RecId found on block %d:%d!\n", m_DevIdx, Tv_BlkId );
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::ReadByHndl( __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Dpos;
		__u32	Tv_BlkId;
	// code --------------------
		Tv_Result	=	FALSE;

		// check disk status
		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			Tv_Dpos	=	A_Hndl & E_GrDvrFs2HndlPosMask;
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
				if ( E_GrDvrFs2PosRda <= Tv_Dpos   )
				{
					Tv_BlkId	=	(__u32)( (Tv_Dpos - E_GrDvrFs2PosRda) >> E_GrDvrFs2BlockShift );
					if ( Tv_BlkId < m_Mis.BlkCnt )
					{
						if ( m_Bdt[Tv_BlkId].IsBad )
						{
							DbgMsgPrint( "Cls_GrDvrFs2Disk::ReadByHndl - bad block (%d:%d) access.\n", m_DevIdx, Tv_BlkId );
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
						DbgMsgPrint( "Cls_GrDvrFs2Disk::ReadByHndl - bad handle input.\n" );
					}
				}
				else
				{
					// system area
					m_DskStat	=	E_GrDvrRecStatBad;
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs2Disk::TimeHndlFindFw( Def_GrTime A_TimeSt, Def_GrTime* A_PtrRtTime )
{
	// local -------------------
		__u64	Tv_Result;
		__s32		Tv_DmdIdx;
		Def_GrTime	Tv_TimeDay;
		Def_GrTime	Tv_TimeRslt;
		__u32	Tv_MmdIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrTmb;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		Tv_PtrTmb		=	(Ptr_GrDvrRecMinThumb)m_PtrTmpBuf;

		Tv_TimeRslt	=	A_TimeSt;

		while ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			Tv_TimeDay	=	Tv_TimeRslt - (Tv_TimeRslt % E_GrTimeSecOfDay);
			Tv_DmdIdx	=	LcDmdNearFw( Tv_TimeDay );
			if ( 0 <= Tv_DmdIdx )
			{
				// load
				if ( LcMmdLoadByDmdIdx( (__u32)Tv_DmdIdx, Tv_PtrTmb ) )
				{
					Tv_MmdIdx	=	(Tv_TimeRslt - Tv_TimeDay) / E_GrTimeSecOfMin;
					while ( E_GrDvrFs2MmdDayItemCnt > Tv_MmdIdx )
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
					// check finished
					if ( E_GrDvrRecHndlNone != Tv_Result )
					{
						break;
					}
				}
			}
			Tv_TimeRslt	=	Tv_TimeDay + E_GrTimeSecOfDay;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs2Disk::TimeHndlFindBw( Def_GrTime A_TimeSt, Def_GrTime* A_PtrRtTime )
{
	// local -------------------
		__u64	Tv_Result;
		__s32		Tv_DmdIdx;
		Def_GrTime	Tv_TimeDay;
		Def_GrTime	Tv_TimeRslt;
		__s32	Tv_MmdIdx;
		Ptr_GrDvrRecMinThumb	Tv_PtrTmb;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		Tv_PtrTmb		=	(Ptr_GrDvrRecMinThumb)m_PtrTmpBuf;

		Tv_TimeRslt	=	A_TimeSt;

		while ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			Tv_TimeDay	=	Tv_TimeRslt - (Tv_TimeRslt % E_GrTimeSecOfDay);
			Tv_DmdIdx	=	LcDmdNearBw( Tv_TimeDay );
			if ( 0 <= Tv_DmdIdx )
			{
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
					// check finished
					if ( E_GrDvrRecHndlNone != Tv_Result )
					{
						break;
					}
				}
			}
			Tv_TimeRslt	=	Tv_TimeDay - E_GrTimeSecOfMin;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::BlockInfoFlush( __u32 A_BlkId )
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
BOOL8	Cls_GrDvrFs2Disk::LcCopy( __u64 A_PosTg, __u64 A_PosSr, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_PosSr;
		__u64	Tv_PosTg;
		__u32	Tv_CpSize;
		__u32	Tv_Remain;
	// code --------------------
		Tv_Result	=	TRUE;
		Tv_Remain	=	A_Size;
		Tv_PosSr	=	A_PosSr;
		Tv_PosTg	=	A_PosTg;

		while ( 0 != Tv_Remain )
		{
			Tv_CpSize	=	Tv_Remain;
			if ( E_GrDvrFs2TmpBufSize < Tv_CpSize )
			{
				Tv_CpSize	=	E_GrDvrFs2TmpBufSize;
			}
			// read
			if ( !LcSeek( Tv_PosSr ) )
			{
				Tv_Result	=	FALSE;
				break;
			}
			if ( !LcRead( m_PtrTmpBuf, Tv_CpSize ) )
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
			if ( !LcWrite( m_PtrTmpBuf, Tv_CpSize ) )
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
BOOL8	Cls_GrDvrFs2Disk::LcFillByte( __u64 A_PosTg, __u32 A_Size, __u8 A_Fill )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_PosTg;
		__u32	Tv_WkSize;
		__u32	Tv_Remain;
	// code --------------------
		Tv_Result	=	TRUE;
		// fill buffer
		GrDumyFillB( m_PtrTmpBuf, E_GrDvrFs2TmpBufSize, A_Fill );
		// init
		Tv_Remain	=	A_Size;
		Tv_PosTg	=	A_PosTg;

		while ( 0 != Tv_Remain )
		{
			Tv_WkSize	=	Tv_Remain;
			if ( E_GrDvrFs2TmpBufSize < Tv_WkSize )
			{
				Tv_WkSize	=	E_GrDvrFs2TmpBufSize;
			}

			// write
			if ( !LcSeek( Tv_PosTg ) )
			{
				Tv_Result	=	FALSE;
				break;
			}
			if ( !LcWrite( m_PtrTmpBuf, Tv_WkSize ) )
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
BOOL8	Cls_GrDvrFs2Disk::LcPread( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFs2Disk::LcPwrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFs2Disk::IsV1System( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	m_IsNeedCvt;
		if ( E_GrDvrRecStatBad == m_DskStat )
		{
			Tv_Result	=	FALSE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::V1FsUpgrade( __u32 A_PastRecId, __u32 A_NearRecId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_RecId;
		__u32	Tv_BlkIdx;
		__u32	Tv_WkCnt;
		__u32	Tv_WkIdx;
		__u32	Tv_Mili;
		St_GrDvrLogItm	Tv_Log;
	// code --------------------
		// check disk status
		if ( E_GrDvrRecStatDefective == m_DskStat )
		{
			// convert mis
			m_Mis.FsFcc		=	E_GrDvrFs2Fcc;
			m_Mis.FsVer		=	E_GrDvrFs2Ver;
			m_Mis.ElaSize	=	0x00200000;			// V1 ELA size
			m_Mis.VsaSize	=	(__u32)m_Mis.V1UwaSize;
			m_Mis.BlkCnt	=	(__u32)( (m_Mis.V1RdaLast - m_Mis.V1RdaPos) >> E_GrDvrFs2BlockShift);

			// make RCA
			m_Rca.DvrId			=	m_DvrId;
			m_Rca.EvtPosIn	=	m_V1Rca.EvtPosIn;
			m_Rca.IsEvtOvwt	=	FALSE;
			if ( (m_V1Rca.EvtPosIn < m_V1Rca.EvtPosOut) || ( 0 != m_V1Rca.EvtPosOut ) )
			{
				m_Rca.IsEvtOvwt	=	TRUE;
			}

			// move VSS data
			if ( m_V1Rca.IsVss )
			{
				if ( !LcCopy( E_GrDvrFs2PosVsa, m_Mis.V1UwaPos, (__u32)m_Mis.V1UwaSize ) )
				{
					m_DskStat	=	E_GrDvrRecStatBad;
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				}
			}

			// make BDT
			if ( E_GrDvrRecStatBad != m_DskStat )
			{
				GrDumyZeroMem( m_Bdt, sizeof(m_Bdt) );
				// overwrite mode
				if ( m_V1Rca.RecPosIn < m_V1Rca.RecPosOut )
				{
					// build past
					Tv_WkCnt	=	(__u32)( (m_V1Rca.RecPosIn - E_GrDvrFs2PosRda) >> E_GrDvrFs2BlockShift );
					Tv_BlkIdx	=	0;
					Tv_RecId	=	A_PastRecId;
					if ( 0 != Tv_WkCnt )
					{
						for ( Tv_WkIdx=0;Tv_WkIdx < Tv_WkCnt;Tv_WkIdx++ )
						{
							m_Bdt[Tv_BlkIdx].RecId		=	Tv_RecId;
							m_Bdt[Tv_BlkIdx].IsFull		=	TRUE;
							m_Bdt[Tv_BlkIdx].RecSize	=	E_GrDvrFs2BlockSize;
							// next
							Tv_BlkIdx ++;
							Tv_RecId ++;
						}
					}
					// last block
					Tv_WkCnt	=	(__u32)( m_V1Rca.RecPosIn & E_GrDvrFs2DiskBlkAlignMask );
					if ( 0 != Tv_WkCnt )
					{
						m_Bdt[Tv_BlkIdx].RecId		=	Tv_RecId;
						m_Bdt[Tv_BlkIdx].RecSize	=	Tv_WkCnt;
					}
					// build present
					Tv_WkCnt	=	(__u32)( (m_Mis.V1RdaLast - m_V1Rca.RecPosOut) >> E_GrDvrFs2BlockShift );
					if ( 0 != Tv_WkCnt )
					{
						Tv_BlkIdx	=	(__u32)( (m_V1Rca.RecPosOut - E_GrDvrFs2PosRda) >> E_GrDvrFs2BlockShift);
						Tv_RecId	=	A_NearRecId;
						for ( Tv_WkIdx=0;Tv_WkIdx < Tv_WkCnt;Tv_WkIdx++ )
						{
							m_Bdt[Tv_BlkIdx].RecId		=	Tv_RecId;
							m_Bdt[Tv_BlkIdx].IsFull		=	TRUE;
							m_Bdt[Tv_BlkIdx].RecSize	=	E_GrDvrFs2BlockSize;
							// next
							Tv_BlkIdx ++;
							Tv_RecId ++;
						}
					}
				}
				else
				{
					// build preset only
					Tv_WkCnt	=	(__u32)( (m_V1Rca.RecPosIn - m_V1Rca.RecPosOut) >> E_GrDvrFs2BlockShift );
					Tv_BlkIdx	=	(__u32)( (m_V1Rca.RecPosOut - E_GrDvrFs2PosRda) >> E_GrDvrFs2BlockShift);
					Tv_RecId	=	A_NearRecId;
					if ( 0 != Tv_WkCnt )
					{
						for ( Tv_WkIdx=0;Tv_WkIdx < Tv_WkCnt;Tv_WkIdx++ )
						{
							m_Bdt[Tv_BlkIdx].RecId		=	Tv_RecId;
							m_Bdt[Tv_BlkIdx].IsFull		=	TRUE;
							m_Bdt[Tv_BlkIdx].RecSize	=	E_GrDvrFs2BlockSize;
							// next
							Tv_BlkIdx ++;
							Tv_RecId ++;
						}
					}
					// last block
					Tv_WkCnt	=	(__u32)( m_V1Rca.RecPosIn & E_GrDvrFs2DiskBlkAlignMask );
					if ( 0 != Tv_WkCnt )
					{
						m_Bdt[Tv_BlkIdx].RecId		=	Tv_RecId;
						m_Bdt[Tv_BlkIdx].RecSize	=	Tv_WkCnt;
					}
				}
				// BDT write
				BdtSave();
			}

			// TMA to DMD/MMD
			if ( E_GrDvrRecStatBad != m_DskStat )
			{
				GrDumyZeroMem( m_Dmd, sizeof(m_Dmd) );	// reset DMD
				LcFillByte( E_GrDvrFs2PosMmd, E_GrDvrFs2MmdTotalSize, 0 );	// reset MMD
				LcCvtTma( );		// convert
			}

			// write UCA
			if ( E_GrDvrRecStatBad != m_DskStat )
			{
				LcCopy( E_GrDvrFs2PosUca, E_GrDvrFs2PosV1Uca, E_GrDvrFs2V1UcaSize );
			}

			// write RCA
			if ( E_GrDvrRecStatBad != m_DskStat )
			{
				LcRcaWrite();
			}

			// write MIS
			if ( E_GrDvrRecStatBad != m_DskStat )
			{
				if ( !LcPwrite( E_GrDvrFs2PosMis, &m_Mis, sizeof(m_Mis) ) )
				{
					m_DskStat	=	E_GrDvrRecStatBad;
					GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				}
			}
			
			// update success
			if ( E_GrDvrRecStatBad != m_DskStat )
			{
				// add log
				Tv_Log.Ch		=	0xFF;
				Tv_Log.Type	=	E_GrDvrLogTypeFsUpgrade;
				Tv_Log.ParaByte	=	m_DevIdx;
				Tv_Log.Time	=	GrTimeSysTimeGet( &Tv_Mili );
				if ( EvtLogAdd( &Tv_Log ) )
				{
					m_DskStat	=	E_GrDvrRecStatEmpty;
					LcNormalStatUpdt();
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcCvtTma( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Remain;
		__u32	Tv_WkIdx;
		__u64	Tv_Dpos;
		__u32	Tv_TmaTime;
		__u32	Tv_ReadCnt;
		__u32	Tv_ReqTime;
		Ptr_GrDvrRecMinThumb	Tv_PtrTma;
	// code --------------------
		Tv_Result	=	TRUE;

		Tv_Dpos			=	E_GrDvrFs2PosV1Tma;
		Tv_TmaTime	=	0;
		Tv_Remain		=	m_Mis.V1TmaCnt;
		Tv_PtrTma		=	(Ptr_GrDvrRecMinThumb)m_PtrTmpBuf;

		while ( 0 != Tv_Remain )
		{
			Tv_ReadCnt	=	Tv_Remain;
			if ( E_GrDvrFs2CvtTmaWkCnt < Tv_ReadCnt )
			{
				Tv_ReadCnt	=	E_GrDvrFs2CvtTmaWkCnt;
			}
			// read
			if ( LcPread( Tv_Dpos, Tv_PtrTma, (Tv_ReadCnt << E_GrDvrRecMinThumbSizeShft) ) )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ReadCnt;Tv_WkIdx++ )
				{
					// check time
					Tv_ReqTime	=	Tv_TmaTime;
					if ( E_GrTimeSecOfDay > Tv_ReqTime )
					{
						Tv_ReqTime	=	Tv_ReqTime + E_GrTimeSecOfDay;
					}
					// check able handle
					if ( LcIsV1ValidHndl( Tv_PtrTma[Tv_WkIdx].HndlRec ) )
					{
						LcThumbDirWrite( Tv_ReqTime, &Tv_PtrTma[Tv_WkIdx] );
					}
					// next
					Tv_TmaTime	=	Tv_TmaTime + E_GrTimeSecOfMin;
				}
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				Tv_Result	=	FALSE;
				break;
			}
			// next
			Tv_Dpos		=	Tv_Dpos + (__u64)(Tv_ReadCnt << E_GrDvrRecMinThumbSizeShft);
			Tv_Remain	=	Tv_Remain - Tv_ReadCnt;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcIsV1ValidHndl( __u64 A_V1Hndl )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_SeqId;
		__u64	Tv_DataPos;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_DataPos	=	A_V1Hndl & E_GrDvrFs2HndlPosMask;
		Tv_SeqId		=	(__u16)(A_V1Hndl >> E_GrDvrFs2HndlOvwtIdPos);
		if ( m_V1Rca.RecPosOut < m_V1Rca.RecPosIn )
		{
			if (	(Tv_SeqId == m_V1Rca.RecSeqId) && (m_V1Rca.RecPosOut <= Tv_DataPos) && 
						(m_V1Rca.RecPosIn > Tv_DataPos)
				)
			{
				Tv_Result	=	TRUE;
			}
		}
		else if ( m_V1Rca.RecPosOut > m_V1Rca.RecPosIn )
		{
			if (	(((Tv_SeqId+1) == m_V1Rca.RecSeqId) && (m_V1Rca.RecPosOut <= Tv_DataPos)) || 
						((Tv_SeqId == m_V1Rca.RecSeqId) && (m_V1Rca.RecPosIn > Tv_DataPos))
				)
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcThumbDirWrite( Def_GrTime A_Time, Ptr_GrDvrRecMinThumb A_PtrThumb )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_DmdIdx;
		Def_GrTime	Tv_Day;
		__u64	Tv_Dpos;
		__u32	Tv_Min;
	// code --------------------
		Tv_Result	=	FALSE;

		// check exist DMD
		Tv_Day		=	A_Time - (A_Time % E_GrTimeSecOfDay);
		Tv_DmdIdx	=	LcDmdFind( Tv_Day );
		if ( 0 > Tv_DmdIdx  )
		{
			// need create
			Tv_DmdIdx	=	LcDmdAlloc();
			if ( 0 <= Tv_DmdIdx )
			{
				// allocate new
				m_Dmd[Tv_DmdIdx]	=	Tv_Day;
				LcDmdWrite();
			}
		}

		// MMD
		if ( (0 <= Tv_DmdIdx) && (E_GrDvrRecStatBad != m_DskStat) )
		{
			// calculate disk position
			Tv_Min	=	(A_Time - Tv_Day) / E_GrTimeSecOfMin;
			Tv_Dpos	=	(__u64)(E_GrDvrFs2PosMmd + (Tv_DmdIdx * E_GrDvrFs2MmdDaySize) + (Tv_Min * sizeof(St_GrDvrRecMinThumb)));
			if ( LcPwrite( Tv_Dpos, A_PtrThumb, sizeof(St_GrDvrRecMinThumb) ) )
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
__u32	Cls_GrDvrFs2Disk::V1PastBlockCnt( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( m_IsNeedCvt )
		{
			if ( m_V1Rca.RecPosOut > m_V1Rca.RecPosIn )
			{
				Tv_Result	=	(__u32)((m_Mis.V1RdaLast - m_V1Rca.RecPosOut) >> E_GrDvrFs2BlockShift);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs2Disk::V1PresentBlockCnt( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( m_IsNeedCvt )
		{
			if ( m_V1Rca.RecPosIn < m_V1Rca.RecPosOut )
			{
				Tv_Result	=	(__u32)( (m_V1Rca.RecPosIn - m_Mis.V1RdaPos) >> E_GrDvrFs2BlockShift );
			}
			else
			{
				Tv_Result	=	(__u32)( (m_V1Rca.RecPosIn - m_V1Rca.RecPosOut) >> E_GrDvrFs2BlockShift );
			}
			if ( 0 != (E_GrDvrFs2DiskBlkAlignMask & m_V1Rca.RecPosIn) )
			{
				Tv_Result ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::IsV1Full( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_IsNeedCvt )
		{
			if ( ( E_GrDvrFs2PosRda == m_V1Rca.RecPosOut ) && ( m_Mis.V1RdaLast == m_V1Rca.RecPosIn ) )
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::BdtRecIdGather( __u32 A_BaseId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_ChkIdx;
		__u32	Tv_SortIdx;
		__u32	Tv_SortRid;
		__u32	Tv_RecId;
	// code --------------------
		Tv_Result	=	FALSE;

		// check disk status
		if ( E_GrDvrRecStatUnformated < m_DskStat )
		{
			// reset sort field
			for ( Tv_WkIdx=0;Tv_WkIdx < m_Mis.BlkCnt;Tv_WkIdx++ )
			{
				m_Bdt[Tv_WkIdx].IsCanSort	=	TRUE;
			}
			// do sort
			Tv_RecId	=	A_BaseId;
			for ( Tv_WkIdx=0;Tv_WkIdx < m_Mis.BlkCnt;Tv_WkIdx++ )
			{
				Tv_SortIdx	=	0xFFFFFFFF;
				Tv_SortRid	=	0xFFFFFFFF;
				for ( Tv_ChkIdx=0;Tv_ChkIdx < m_Mis.BlkCnt;Tv_WkIdx++ )
				{
					if ( (0 != m_Bdt[Tv_ChkIdx].RecId) && (m_Bdt[Tv_ChkIdx].IsCanSort) )
					{
						// check small
						if ( Tv_SortRid > m_Bdt[Tv_ChkIdx].RecId )
						{
							// found
							Tv_SortRid	=	m_Bdt[Tv_ChkIdx].RecId;
							Tv_SortIdx	=	Tv_ChkIdx;
						}
					}
				}
				// check found
				if ( Tv_SortIdx == 0xFFFFFFFF )
				{
					// finish
					break;
				}
				// update sorted
				m_Bdt[Tv_SortIdx].RecId			=	Tv_RecId;
				m_Bdt[Tv_SortIdx].IsCanSort	=	FALSE;
				Tv_RecId ++;
			}
			// BDT write
			Tv_Result	=	BdtSave();

		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcEvtLogDisable( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDvrLogItm	Tv_PtrLogTbl;
		__s64	Tv_DskPos;		// disk position
		__u32		Tv_Remain;
		__u32		Tv_RwSize;		// read write size
		__u32		Tv_ItmCnt;
		__u32		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		// init
		Tv_Remain	=	m_Mis.ElaSize;
		Tv_DskPos	=	E_GrDvrFs2PosEla;
		Tv_PtrLogTbl	=	(Ptr_GrDvrLogItm)m_PtrTmpBuf;

		while ( 0 != Tv_Remain )
		{
			// calculate read write size
			Tv_RwSize	=	Tv_Remain;
			if ( E_GrDvrFs2TmpBufSize < Tv_RwSize )
			{
				Tv_RwSize	=	E_GrDvrFs2TmpBufSize;
			}
			Tv_ItmCnt	=	Tv_RwSize >> E_GrDvrRecMinThumbSizeShft;
			Tv_RwSize	=	Tv_ItmCnt << E_GrDvrRecMinThumbSizeShft;

			// read
			if ( !LcPread( Tv_DskPos, Tv_PtrLogTbl, Tv_RwSize ) )
			{
				// disk error
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				break;
			}

			// disable
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ItmCnt;Tv_WkIdx++ )
			{
				Tv_PtrLogTbl[Tv_WkIdx].IsAble	=	FALSE;
			}

			// write
			if ( !LcPwrite( Tv_DskPos, Tv_PtrLogTbl, Tv_RwSize ) )
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				break;
			}
			
			// next
			Tv_Remain	=	Tv_Remain - Tv_RwSize;
			Tv_DskPos	=	Tv_DskPos + (__s64)Tv_RwSize;
		}

		// check success
		if ( 0 == Tv_Remain )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::EvtLogAdd( Ptr_GrDvrLogItm A_PtrLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_PosIn;
		__u64	Tv_DskPos;
	// code --------------------
		Tv_Result	=	FALSE;

		//check disk status
		if ( E_GrDvrRecStatBad != m_DskStat )
		{
			//write item
			A_PtrLog->IsAble	=	TRUE;		//patch able
			Tv_PosIn	=	m_Rca.EvtPosIn;
			Tv_DskPos	=	E_GrDvrFs2PosEla + (__u64)(Tv_PosIn << E_GrDvrLogShiftSize);
			if ( LcPwrite( Tv_DskPos, A_PtrLog, sizeof(St_GrDvrLogItm) ) )
			{
				// update next position
				Tv_PosIn ++;
				if ( (m_Mis.ElaSize >> E_GrDvrLogShiftSize) <= Tv_PosIn )
				{
					Tv_PosIn	=	0;
					m_Rca.IsEvtOvwt	=	TRUE;
				}
				// update info
				m_Rca.EvtPosIn	=	Tv_PosIn;
				// success
				Tv_Result	=	TRUE;
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskOperateBad, m_DevIdx );
				DbgMsgPrint( "Cls_GrDvrFs2Disk::EvtLogAdd - bad sector appear!.\n" );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcTimeBeginByBdt( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BdtIdx;
		__u32	Tv_TimeSt;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			Tv_Result	=	TRUE;
			Tv_TimeSt	=	0xFFFFFFFF;
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
							break;
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
				DbgMsgPrint( "Cls_GrDvrFs2Disk::LcTimeBeginByBdt - BDT time not properly, may be this HDD is converted.\n" );
				Tv_Result	=	FALSE;
			}
		}
		else
		{
			m_RecTimeBegin	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcTimeEndByBdt( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BdtIdx;
		__u32	Tv_TimeEd;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			Tv_TimeEd	=	0;
			for ( Tv_BdtIdx=0;Tv_BdtIdx< m_Mis.BlkCnt;Tv_BdtIdx++ )
			{
				if ( !m_Bdt[Tv_BdtIdx].IsBad )
				{
					if ( 0 != m_Bdt[Tv_BdtIdx].RecSize )
					{
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
							break;
						}
					}
				}
			}
			// check found
			if ( 0 != Tv_TimeEd )
			{
				m_RecTimeEnd	=	Tv_TimeEd;
			}
			else
			{
				DbgMsgPrint( "Cls_GrDvrFs2Disk::LcTimeEndByBdt - BDT time not properly, may be this HDD is converted.\n" );
				Tv_Result	=	FALSE;
			}
		}
		else
		{
			m_RecTimeEnd	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32		Cls_GrDvrFs2Disk::LcDmdLowest( void )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_WkIdx;
		Def_GrTime	Tv_Time;
	// code --------------------
		Tv_Result	=	-1;

		Tv_Time		=	0xFFFFFFFF;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2DmdItemCnt;Tv_WkIdx++ )
		{
			if ( 0 != m_Dmd[Tv_WkIdx] )
			{
				if ( Tv_Time > m_Dmd[Tv_WkIdx] )
				{
					Tv_Time		=	m_Dmd[Tv_WkIdx];
					Tv_Result	=	Tv_WkIdx;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32		Cls_GrDvrFs2Disk::LcDmdHighest( void )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_WkIdx;
		Def_GrTime	Tv_Time;
	// code --------------------
		Tv_Result	=	-1;

		Tv_Time		=	0x0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2DmdItemCnt;Tv_WkIdx++ )
		{
			if ( 0 != m_Dmd[Tv_WkIdx] )
			{
				if ( Tv_Time < m_Dmd[Tv_WkIdx] )
				{
					Tv_Time		=	m_Dmd[Tv_WkIdx];
					Tv_Result	=	Tv_WkIdx;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcTimeBeginByMmd( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_DmdIdx;
		__s32		Tv_MmdIdx;
		St_GrDvrRecMinThumb	Tv_Thumb;
	// code --------------------
		Tv_Result	=	FALSE;

		while ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			// find lowest
			Tv_DmdIdx	=	LcDmdLowest();
			if ( 0 > Tv_DmdIdx )
			{
				break;
			}
			// read MMD
			Tv_MmdIdx	=	LcMmdFindOldestThumb( (__u32)Tv_DmdIdx, &Tv_Thumb );
			if ( 0 != Tv_MmdIdx )
			{
				m_RecTimeBegin	=	m_Dmd[Tv_DmdIdx] + ( (__u32)Tv_MmdIdx * E_GrTimeSecOfMin );
				Tv_Result	=	TRUE;
				break;
			}
		}
		// check bad disk
		if ( E_GrDvrRecStatEmpty >= m_DskStat )
		{
			m_RecTimeBegin	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcTimeEndByMMd( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_DmdIdx;
		__s32		Tv_MmdIdx;
		St_GrDvrRecMinThumb	Tv_Thumb;
	// code --------------------
		Tv_Result	=	FALSE;

		while ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			// find lowest
			Tv_DmdIdx	=	LcDmdHighest();
			if ( 0 > Tv_DmdIdx )
			{
				break;
			}
			// read MMD
			Tv_MmdIdx	=	LcMmdFindNewestThumb( (__u32)Tv_DmdIdx, &Tv_Thumb );
			if ( 0 != Tv_MmdIdx )
			{
				m_RecTimeEnd	=	m_Dmd[Tv_DmdIdx] + ( (__u32)Tv_MmdIdx * E_GrTimeSecOfMin );
				Tv_Result	=	TRUE;
				break;
			}
		}
		// check bad disk
		if ( E_GrDvrRecStatEmpty >= m_DskStat )
		{
			m_RecTimeEnd	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcTimeBeginUpdt( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			Tv_Result	=	LcTimeBeginByBdt();
			if ( !Tv_Result && (E_GrDvrRecStatEmpty < m_DskStat) )
			{
				Tv_Result	=	LcTimeBeginByMmd();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcTimeEndUpdt( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			Tv_Result	=	LcTimeBeginByBdt();
			if ( !Tv_Result && (E_GrDvrRecStatEmpty < m_DskStat) )
			{
				Tv_Result	=	LcTimeBeginByMmd();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::VssReset( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			Tv_Result	=	LcFillByte( E_GrDvrFs2PosVsa, m_Mis.VsaSize, 0 );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::VssWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			// check VSA over
			if ( m_Mis.VsaSize >= (A_Pos + A_Size) )
			{
				Tv_Dpos	=	(__u64)( E_GrDvrFs2PosVsa + A_Pos );
				Tv_Result	=	LcPwrite( Tv_Dpos, A_PtrBuf, A_Size );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::VssRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			// check VSA over
			if ( m_Mis.VsaSize >= (A_Pos + A_Size) )
			{
				Tv_Dpos	=	(__u64)( E_GrDvrFs2PosVsa + A_Pos );
				Tv_Result	=	LcPread( Tv_Dpos, A_PtrBuf, A_Size );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::UcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			// check VSA over
			if ( E_GrDvrFs2UcaMaxSize >= (A_Pos + A_Size) )
			{
				Tv_Dpos	=	(__u64)( E_GrDvrFs2PosUca + A_Pos );
				Tv_Result	=	LcPwrite( Tv_Dpos, A_PtrBuf, A_Size );
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::UcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			// check VSA over
			if ( E_GrDvrFs2UcaMaxSize >= (A_Pos + A_Size) )
			{
				Tv_Dpos	=	(__u64)( E_GrDvrFs2PosUca + A_Pos );
				Tv_Result	=	LcPread( Tv_Dpos, A_PtrBuf, A_Size );
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Disk::LcSmartRead( void )
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
BOOL8	Cls_GrDvrFs2Disk::LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_BytePos;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_BytePos	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2SmartValCnt;Tv_WkIdx++ )
		{
			if ( A_Id == m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFs2SmartValIdxId] )
			{
				*A_PtrRtVal	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFs2SmartValIdxRawVal];
				Tv_Result	=	TRUE;
				break;
			}
			//next
			Tv_BytePos	=	Tv_BytePos + E_GrDvrFs2SmartValItemSize;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Disk::TemperatureGet( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrDvrRecStatBad < m_DskStat )
		{
			if ( LcSmartRead() )
			{
				LcSmartFindRawByte( E_GrDvrFs2SmartIdTemp, &Tv_Result );
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs2Disk::TimeRecSizeCalc( __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		__u64	Tv_SizeTot;
		__u64	Tv_SizeRec;
		__u32	Tv_DmdTot;
		__u32	Tv_DmdIdx;
		__u32	Tv_DmdCrt;
		Def_GrTime	Tv_TimeSt;
		Def_GrTime	Tv_TimeEd;
		__u16	Tv_ChMask;
		__u16	Tv_ChIdx;
		__u32	Tv_ChCnt;
	// code --------------------
		Tv_Result	=	0;
		// check channel exist
		if ( 0 != A_ChMap )
		{
			// get total disk size
			if ( RecSizeGet( &Tv_SizeTot, &Tv_SizeRec ) )
			{
				Tv_TimeSt	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfDay);
				Tv_TimeEd	=	A_TimeEd - (A_TimeEd % E_GrTimeSecOfDay); 
				// get total DMD count
				Tv_DmdTot	=	0;
				Tv_DmdCrt	=	0;
				for ( Tv_DmdIdx=0;Tv_DmdIdx < E_GrDvrFs2DmdItemCnt;Tv_DmdIdx++ )
				{
					if ( 0 != m_Dmd[Tv_DmdIdx] )
					{
						// increase DMD count
						Tv_DmdTot ++;
						if ( (Tv_TimeSt <= m_Dmd[Tv_DmdIdx]) && (Tv_TimeEd >= m_Dmd[Tv_DmdIdx]) )
						{
							Tv_DmdCrt ++;
						}
					}
				}
				// check exist
				if ( 0 != Tv_DmdCrt )
				{
					Tv_Result	=	(Tv_SizeRec * (__u64)Tv_DmdCrt) / (__u64)Tv_DmdTot;
					// get channel count
					Tv_ChCnt	=	0;
					Tv_ChMask	=	1;
					for ( Tv_ChIdx=0;Tv_ChIdx < E_GrDvrMaxChCnt;Tv_ChIdx++ )
					{
						if ( 0 != (Tv_ChMask & A_ChMap) )
						{
							Tv_ChCnt ++;
						}
						// next
						Tv_ChMask	=	Tv_ChMask << 1;
					}
					// get ratio
					Tv_Result	=	((Tv_Result * (__u64)Tv_ChCnt) >> 4) + 4000000;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Disk::EvtLogGetNext( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		__u64	Tv_DskPos;
		__u32	Tv_Remain;
		__u32	Tv_ReadCnt;
		__u32	Tv_ReadSize;
		BOOL8	Tv_IsOk;
		Ptr_GrDvrLogItm	Tv_PtrLogTbl;
		__u32	Tv_WkIdx;
	// code --------------------
		// check disk status
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			// init
			Tv_DskPos		=	E_GrDvrFs2PosEla;
			Tv_Remain		=	m_Mis.ElaSize;
			Tv_ReadCnt	=	E_GrDvrFs2TmpBufSize >> E_GrDvrLogShiftSize;
			Tv_ReadSize	=	Tv_ReadCnt << E_GrDvrLogShiftSize;
			Tv_PtrLogTbl	=	(Ptr_GrDvrLogItm)m_PtrTmpBuf;

			while ( 0 != Tv_Remain )
			{
				Tv_IsOk	=	FALSE;
				if ( LcPread( Tv_DskPos, Tv_PtrLogTbl, Tv_ReadSize ) )
				{
					Tv_IsOk	=	TRUE;	// success
					// parsing
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ReadCnt;Tv_WkIdx++ )
					{
						if ( 0 != Tv_PtrLogTbl[Tv_WkIdx].Time )
						{
							GrDvrLogV1GetFw( A_PtrCtrl, &Tv_PtrLogTbl[Tv_WkIdx] );
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
				Tv_Remain	=	Tv_Remain - Tv_ReadSize;
				Tv_DskPos	=	Tv_DskPos + (__u64)Tv_ReadSize;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Disk::EvtLogGetPrev( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		__u64	Tv_DskPos;
		__u32	Tv_Remain;
		__u32	Tv_ReadCnt;
		__u32	Tv_ReadSize;
		BOOL8	Tv_IsOk;
		Ptr_GrDvrLogItm	Tv_PtrLogTbl;
		__u32	Tv_WkIdx;
	// code --------------------
		// check disk status
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			// init
			Tv_DskPos		=	E_GrDvrFs2PosEla;
			Tv_Remain		=	m_Mis.ElaSize;
			Tv_ReadCnt	=	E_GrDvrFs2TmpBufSize >> E_GrDvrLogShiftSize;
			Tv_ReadSize	=	Tv_ReadCnt << E_GrDvrLogShiftSize;
			Tv_PtrLogTbl	=	(Ptr_GrDvrLogItm)m_PtrTmpBuf;

			while ( 0 != Tv_Remain )
			{
				Tv_IsOk	=	FALSE;
				if ( LcPread( Tv_DskPos, Tv_PtrLogTbl, Tv_ReadSize ) )
				{
					Tv_IsOk	=	TRUE;	// success
					// parsing
					for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ReadCnt;Tv_WkIdx++ )
					{
						if ( 0 != Tv_PtrLogTbl[Tv_WkIdx].Time )
						{
							GrDvrLogV1GetBw( A_PtrCtrl, &Tv_PtrLogTbl[Tv_WkIdx] );
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
				Tv_Remain	=	Tv_Remain - Tv_ReadSize;
				Tv_DskPos	=	Tv_DskPos + (__u64)Tv_ReadSize;
			}
		}

}
//--------------------------------------------------------------------
