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

#include <Dvr/Fs/V6/GrDvrFs6Base.h>
#include <Dvr/Fs/V6/GrDvrFs6Disk.h>
//#include <Dvr/Fs/V6/GrDvrFs6Mng.h>

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


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

extern	St_GrDvrFs6Env	V_GrDvrFs6Env;

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs6Disk::Cls_GrDvrFs6Disk(__u8 A_DevIdx)
{
	// local -------------------
		Def_StrGrFnPathShort	Tv_StrDev;
#ifndef LINUX_APP
		Def_StrGrFnPathShort	Tv_StrPath;
#endif
	// code --------------------
		//init
		m_DevIdx				=	A_DevIdx;

		m_DskSize				=	0;

		m_DskStat				=	E_GrDvrRecStatNotexist;

		m_BlkHdBuf			=	NULL;
		m_BlkCnt				=	0;

#ifdef LINUX_APP
		m_HndlDsk	=	-1;
#else
		m_HndlDsk	=	INVALID_HANDLE_VALUE;
#endif

		GrStrClear(m_StrModel);

		// check structural
		if ( 512 != sizeof(St_GrDvrFs6SmartData) )
		{
			DbgMsgPrint( "Cls_GrDvrFs6Disk: Smart struct size ( %d ) mismatch. \n", sizeof(St_GrDvrFs6SmartData) );
		}
				
		if ( GrDvrBiosRecDevNameGet( m_DevIdx, Tv_StrDev ) )
		{
			DbgMsgPrint( "Cls_GrDvrFs6Disk: device ( %d : %s ) found. \n", m_DevIdx, Tv_StrDev );
#ifdef LINUX_APP
			m_HndlDsk	=	open( Tv_StrDev, O_RDWR | O_SYNC );
			if ( 0 <= m_HndlDsk )
			{
				DbgMsgPrint( "Cls_GrDvrFs6Disk: device ( %d ) open success. \n", m_DevIdx );
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
					DbgMsgPrint( "Cls_GrDvrFs6Disk: device ( %d ) get size error. \n", m_DevIdx );
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

			if(E_GrDvrRecStatNotexist != m_DskStat)
			{
				// update model name
				GrDvrBiosRecDevModelNameGet(m_DevIdx, m_StrModel);
			}

		}
		
		//initialize
		if ( E_GrDvrRecStatUnformated == m_DskStat )
		{
			// check disk size
			if ((E_GrDvrFs6BlkSize * 4) <= m_DskSize)
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
Cls_GrDvrFs6Disk::~Cls_GrDvrFs6Disk()
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

		if(NULL != m_BlkHdBuf)
		{
			free(m_BlkHdBuf);
			m_BlkHdBuf	=	NULL;
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcSeek( __s64 A_Adr, BOOL8 A_IsBegin )
{
	// local -------------------
		BOOL8	Tv_Result;
#ifdef LINUX_APP
		__s32		Tv_Mode;
#else
		__u32	Tv_Mode;
		LARGE_INTEGER		Tv_RtVal;
		LARGE_INTEGER		Tv_Adr;
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
			if ( 0 <= lseek64( m_HndlDsk, A_Adr, Tv_Mode ) )
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
			Tv_Adr.QuadPart		=	A_Adr;
			if (SetFilePointerEx(m_HndlDsk, Tv_Adr, &Tv_RtVal, Tv_Mode))
			{
				Tv_Result	=	TRUE;
			}
		}
#endif

		return		Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcRead( void* A_PtrBuf, __u32 A_Size )
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
			if ( ReadFile(m_HndlDsk, A_PtrBuf, A_Size, (LPDWORD)&Tv_Readed, NULL ) )
			{
				Tv_Result		=	TRUE;
			}
		}
#endif

		return		Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcWrite( void* A_PtrBuf, __u32 A_Size )
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
			if ( WriteFile(m_HndlDsk, A_PtrBuf, A_Size, (LPDWORD)&Tv_Writed, NULL) )
			{
				Tv_Result		=	TRUE;
			}
		}
#endif

		return		Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Disk::LcFlush( void )
{
#ifdef LINUX_APP
		if ( -1 != m_HndlDsk )
		{
			fsync( m_HndlDsk );
		}
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::Format(__u8 A_GroupType, __u32 A_SeqLast, __u32 A_SeqOvwt, BOOL8 A_IsPrgsBrdcst)
{
	// local -------------------
		BOOL8	Tv_Result;

		__u32	Tv_BlkIdx;

		Ptr_GrDvrFs6BlkMibHead	Tv_PtrMib;
		Ptr_GrDvrFs6BlkLogHead	Tv_PtrLog;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
		St_GrTaskMsgDiskFmtPrgs	Tv_Msg;
	// code --------------------
		Tv_Result			=	FALSE;

		// check block count
		if(8 < m_BlkCnt)
		{
			// build MIB block
			Tv_PtrMib	=	(Ptr_GrDvrFs6BlkMibHead)(&m_BlkHdBuf[E_GrDvrFs6BlkIdxMib]);
			Tv_PtrMib->Fcc				=	E_GrDvrFs6Fcc;
			Tv_PtrMib->BlkType		=	E_GrDvrFs6BlkTypeMib;
			Tv_PtrMib->FsVer			=	E_GrDvrFs6Ver;
			Tv_PtrMib->DiskSid		=	V_GrDvrFs6Env.DiskBaseId | ((__u64)A_GroupType << E_GrDvrFs6DiskIdGrpTypePos);
			Tv_PtrMib->_rsvd0			=	0;
			Tv_PtrMib->_rsvd1			=	0;
			Tv_PtrMib->_rsvd2			=	0;
			Tv_PtrMib->_rsvd3[0]		=	0;
			Tv_PtrMib->_rsvd3[1]		=	0;
			Tv_PtrMib->_rsvd3[2]		=	0;
			Tv_PtrMib->_rsvd3[3]		=	0;
			Tv_PtrMib->_rsvd3[4]		=	0;
			Tv_PtrMib->_rsvd3[5]		=	0;
			Tv_PtrMib->_rsvd3[6]		=	0;
			Tv_PtrMib->_rsvd3[7]		=	0;
			Tv_PtrMib->_rsvd3[8]		=	0;

			BlkWriteHeadByIdx(E_GrDvrFs6BlkIdxMib, TRUE);	// write head

			// build LOG block
			Tv_PtrLog	=	(Ptr_GrDvrFs6BlkLogHead)(&m_BlkHdBuf[E_GrDvrFs6BlkIdxLog]);
			Tv_PtrLog->Fcc				=	E_GrDvrFs6Fcc;
			Tv_PtrLog->BlkType		=	E_GrDvrFs6BlkTypeLog;
			Tv_PtrLog->IsFull			=	FALSE;
			Tv_PtrLog->InBlk			=	0;
			Tv_PtrLog->InCnt			=	0;

			Tv_PtrLog->_rsvd0			=	0;
			Tv_PtrLog->_rsvd1			=	0;
			Tv_PtrLog->_rsvd2[0]	=	0;
			Tv_PtrLog->_rsvd2[1]	=	0;
			Tv_PtrLog->_rsvd2[2]	=	0;
			Tv_PtrLog->_rsvd2[3]	=	0;
			Tv_PtrLog->_rsvd2[4]	=	0;
			Tv_PtrLog->_rsvd2[5]	=	0;
			Tv_PtrLog->_rsvd2[6]	=	0;
			Tv_PtrLog->_rsvd2[7]	=	0;
			Tv_PtrLog->_rsvd2[8]	=	0;
			Tv_PtrLog->_rsvd2[9]	=	0;
			Tv_PtrLog->_rsvd2[10]	=	0;
			Tv_PtrLog->_rsvd2[11]	=	0;

			// make message
			Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgDiskFormatPrgs;
			Tv_Msg.DiskIdx		=	m_DevIdx;
			Tv_Msg.Prgs				=	0;

			// build DB block
			Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_BlkHdBuf;
			for( Tv_BlkIdx = E_GrDvrFs6BlkIdxDb; Tv_BlkIdx < m_BlkCnt; Tv_BlkIdx++)
			{
				Tv_PtrDb[Tv_BlkIdx].Fcc			=	E_GrDvrFs6Fcc;
				Tv_PtrDb[Tv_BlkIdx].BlkType	=	E_GrDvrFs6BlkTypeDb;
				Tv_PtrDb[Tv_BlkIdx].InSize	=	0;
				Tv_PtrDb[Tv_BlkIdx].BlkAtb	=	0;
				Tv_PtrDb[Tv_BlkIdx].OfsLastDate	=	0;
				Tv_PtrDb[Tv_BlkIdx].OfsLastTime	=	0;
				Tv_PtrDb[Tv_BlkIdx].OvwtId			=	0;
				Tv_PtrDb[Tv_BlkIdx].SeqId				=	0;
				Tv_PtrDb[Tv_BlkIdx].TimeCreate	=	0;
				Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeSt	=	0;
				Tv_PtrDb[Tv_BlkIdx].TimeRng.TimeEd	=	0;
				Tv_PtrDb[Tv_BlkIdx]._rsvd1[0]		=	0;
				Tv_PtrDb[Tv_BlkIdx]._rsvd1[1]		=	0;
				Tv_PtrDb[Tv_BlkIdx]._rsvd1[2]		=	0;
				Tv_PtrDb[Tv_BlkIdx]._rsvd1[3]		=	0;
				Tv_PtrDb[Tv_BlkIdx]._rsvd1[4]		=	0;
				Tv_PtrDb[Tv_BlkIdx]._rsvd1[5]		=	0;
				Tv_PtrDb[Tv_BlkIdx]._rsvd1[6]		=	0;

				// write head
				BlkWriteHeadByIdx(Tv_BlkIdx, TRUE);

				// update progress
				if(A_IsPrgsBrdcst && (0 == (Tv_BlkIdx & 0xFF)))
				{
					Tv_Msg.Prgs		=	(__u8)((Tv_BlkIdx * 100) / m_BlkCnt);
					GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
				}
			}

			// write all
			//BlkWriteHeadAll(TRUE);

			// update status
			StatusUpdate();

			DbgMsgPrint("Cls_GrDvrFs6Disk::Format - disk %d format success!\n", m_DevIdx);

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Disk::LcInit( void )
{
	// local -------------------
	// code --------------------
		m_DskStat	=	E_GrDvrRecStatUnformated;

		// calculate block count
		m_BlkCnt	=	(__u32)(m_DskSize >> E_GrDvrFs6BlkShift);
		if(E_GrDvrFs6BlkMaxCnt < m_BlkCnt)
		{
			m_BlkCnt	=	E_GrDvrFs6BlkMaxCnt;
		}

		// allocate block buffer
		m_BlkHdBuf	=	(Ptr_GrDvrFs6BlkBaseHead)malloc(sizeof(St_GrDvrFs6BlkBaseHead)* m_BlkCnt);

		// read block info
		if(LcBlkHdLoad())
		{
			m_DskStat	=	E_GrDvrRecStatNormal;
		}
	
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Disk::DevIdxGet( void )
{
		return	m_DevIdx;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Disk::DiskStatGet( void )
{
	// local -------------------
	// code --------------------

		return	m_DskStat;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcPread(__u64 A_Adr, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
		if (((ssize_t)A_Size) == pread64(m_HndlDsk,A_PtrBuf,(size_t)A_Size,(__off64_t)A_Adr))
		{
			Tv_Result	=	TRUE;
		}
#else
		if (LcSeek(A_Adr))
		{
			Tv_Result	=	LcRead( A_PtrBuf, A_Size );
		}
#endif
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcPwrite(__u64 A_Adr, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
		if (((ssize_t)A_Size) == pwrite64(m_HndlDsk,A_PtrBuf,(size_t)A_Size,(__off64_t)A_Adr))
		{
			Tv_Result	=	TRUE;
		}
#else
		if (LcSeek(A_Adr))
		{
			Tv_Result	=	LcWrite( A_PtrBuf, A_Size );
		}
#endif
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcIovPread(__u64 A_Adr, iovec* A_PtrIovBuf, __u32 A_IovCnt)
{
	// local -------------------
		BOOL8	Tv_Result;
#ifndef LINUX_APP
		__u32	Tv_WkIdx;
#endif
		// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP

#if 0		/* uclibc not support */
		if (0 < preadv64(m_HndlDsk, A_PtrIovBuf, (int)A_IovCnt, (__off64_t)A_Adr))
		{
			Tv_Result	=	TRUE;
		}
#endif
		
#else
		if (LcSeek(A_Adr))
		{
			Tv_Result	=	TRUE;
			for (Tv_WkIdx=0; Tv_WkIdx < A_IovCnt; Tv_WkIdx++)
			{
				if (!LcRead(A_PtrIovBuf[Tv_WkIdx].iov_base, A_PtrIovBuf[Tv_WkIdx].iov_len))
				{
					Tv_Result	=	FALSE;
					break;
				}
			}
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcIovPwrite(__u64 A_Adr, iovec* A_PtrIovBuf, __u32 A_IovCnt)
{
	// local -------------------
		BOOL8	Tv_Result;
#ifndef LINUX_APP
		__u32	Tv_WkIdx;
#endif
	// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP

#if 0		/* uclibc not support */
		if (0 < pwritev64(m_HndlDsk,A_PtrIovBuf,(int)A_IovCnt,(__off64_t)A_Adr))
		{
			Tv_Result	=	TRUE;
		}
#endif
		
#else
		if (LcSeek(A_Adr))
		{
			Tv_Result	=	TRUE;
			for (Tv_WkIdx=0; Tv_WkIdx < A_IovCnt;Tv_WkIdx++)
			{
				if (!LcWrite(A_PtrIovBuf[Tv_WkIdx].iov_base, A_PtrIovBuf[Tv_WkIdx].iov_len))
				{
					Tv_Result	=	FALSE;
					break;
				}
			}
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcSmartRead( void )
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
BOOL8	Cls_GrDvrFs6Disk::LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_BytePos;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_BytePos	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs6DiskSmartValCnt;Tv_WkIdx++ )
		{
			if ( A_Id == m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFs6DiskSmartValIdxId] )
			{
				*A_PtrRtVal	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFs6DiskSmartValIdxRawVal];
				Tv_Result	=	TRUE;
				break;
			}
			//next
			Tv_BytePos	=	Tv_BytePos + E_GrDvrFs6DiskSmartValItemSize;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Disk::TempratureRead( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( LcSmartRead() )
		{
			LcSmartFindRawByte( E_GrDvrFs6DiskSmartIdTemp, &Tv_Result );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcBlkHdLoad(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	FALSE;

		// load block
		Tv_Dpos	=	0;
		
		for( Tv_WkIdx = 0; Tv_WkIdx < m_BlkCnt; Tv_WkIdx++)
		{
			//if(!LcPread(Tv_Dpos, &m_BlkHdBuf[Tv_WkIdx], sizeof(St_GrDvrFs6BlkBaseHead)))
			if(!LcCchPread(Tv_Dpos, &m_BlkHdBuf[Tv_WkIdx], sizeof(St_GrDvrFs6BlkBaseHead)))
			{
				// bad clear
				GrDumyZeroMem(&m_BlkHdBuf[Tv_WkIdx], sizeof(St_GrDvrFs6BlkBaseHead));
			}
			// next
			Tv_Dpos	=	Tv_Dpos + E_GrDvrFs6BlkSize;
		}

		// check first block
		if((E_GrDvrFs6Fcc == m_BlkHdBuf[0].Fcc) && (E_GrDvrFs6BlkTypeMib == m_BlkHdBuf[E_GrDvrFs6BlkIdxMib].BlkType))
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrDvrFs6BlkBaseHead	Cls_GrDvrFs6Disk::BlkHeadTableGet(__u32* A_PtrRtCnt)
{
	// local -------------------
		Ptr_GrDvrFs6BlkBaseHead	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCnt	=	0;

		if((E_GrDvrRecStatEmpty <= m_DskStat) && (0 != m_BlkCnt))
		{
			*A_PtrRtCnt	=	m_BlkCnt;
			Tv_Result	=	m_BlkHdBuf;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs6Disk::LcBlkGetDbCnt(void)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	0;

		if(0 != m_BlkCnt)
		{
			for( Tv_BlkIdx = 0; Tv_BlkIdx < m_BlkCnt; Tv_BlkIdx++)
			{
				if(E_GrDvrFs6BlkTypeDb == m_BlkHdBuf[Tv_BlkIdx].BlkType)
				{
					Tv_Result ++;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::BlkWriteHeadByIdx(__u32 A_BlkIdx, BOOL8 A_IsForce)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	FALSE;

		// check disk status
		if(A_IsForce || (E_GrDvrRecStatEmpty <= m_DskStat))
		{
			// check block index range
			if(m_BlkCnt > A_BlkIdx)
			{
				Tv_Dpos		=	(__u64)A_BlkIdx << E_GrDvrFs6BlkShift;
				//Tv_Result	=	LcPwrite(Tv_Dpos, &m_BlkHdBuf[A_BlkIdx], sizeof(St_GrDvrFs6BlkBaseHead));
				Tv_Result	=	LcCchPwrite(Tv_Dpos, &m_BlkHdBuf[A_BlkIdx], sizeof(St_GrDvrFs6BlkBaseHead));
				// check bad block
				if(!Tv_Result)
				{
					m_BlkHdBuf[A_BlkIdx].BlkType	=	E_GrDvrFs6BlkTypeBad;
					// check MIB
					if(0 == A_BlkIdx)
					{
						m_DskStat	=	E_GrDvrRecStatBad;
					}
					else
					{
						// update disk status
						StatusUpdate();
					}
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrDvrFs6Disk::BlkWriteHeadByIdx - bad block index %d (disk:%d)\n",
					A_BlkIdx, m_DevIdx);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::BlkWriteHeadAll(BOOL8 A_IsForce)
{
	// local -------------------
		BOOL8	Tv_Result;
		//__u8	Tv_ErrCode;
		__u32	Tv_BlkIdx;
		__u64	Tv_Dpos;
	// code --------------------

		Tv_Result	=	FALSE;

		if(A_IsForce || (E_GrDvrRecStatEmpty <= m_DskStat))
		{
			if(0 != m_BlkCnt)
			{
				for( Tv_BlkIdx = 0; Tv_BlkIdx < m_BlkCnt; Tv_BlkIdx++)
				{
					Tv_Dpos		=	(__u64)Tv_BlkIdx << E_GrDvrFs6BlkShift;
					//if(LcPwrite(Tv_Dpos, &m_BlkHdBuf[Tv_BlkIdx], sizeof(St_GrDvrFs6BlkBaseHead)))
					if(LcCchPwrite(Tv_Dpos, &m_BlkHdBuf[Tv_BlkIdx], sizeof(St_GrDvrFs6BlkBaseHead)))
					{
						Tv_Result	=	TRUE;
					}
					else
					{
						m_BlkHdBuf[Tv_BlkIdx].BlkType	=	E_GrDvrFs6BlkTypeBad;
						// check MIB
						if(0 == Tv_BlkIdx)
						{
							m_DskStat	=	E_GrDvrRecStatBad;
							break;
						}
					}
				}
				// update disk status
				StatusUpdate();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Disk::BlkWriteDataByOfs(__u32 A_BlkIdx, __u32 A_Ofs, void* A_PtrData, __u32 A_Size)
{
	// local -------------------
		__u8	Tv_Result;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	E_GrErrDiskStatBad;

		if(E_GrDvrRecStatEmpty <= m_DskStat)
		{
			Tv_Result	=	E_GrErrBadParam;
			// check block index
			if(m_BlkCnt > A_BlkIdx)
			{
				// check offset and size
				if(E_GrDvrFs6BlkSize >= (A_Ofs + A_Size))
				{
					// check bad block
					Tv_Result	=	E_GrErrDiskBadSector;
					if(E_GrDvrFs6BlkTypeBad != m_BlkHdBuf[A_BlkIdx].BlkType)
					{
						Tv_Result	=	E_GrErrNone;
						Tv_Dpos		=	((__u64)A_BlkIdx << E_GrDvrFs6BlkShift) + (__u64)A_Ofs;
						
						//if(!LcPwrite(Tv_Dpos, A_PtrData, A_Size))
						if(!LcCchPwrite(Tv_Dpos, A_PtrData, A_Size))
						{
							Tv_Result	=	E_GrErrBadIo;
							// mark bad block
							m_BlkHdBuf[A_BlkIdx].BlkType	=	E_GrDvrFs6BlkTypeBad;
							BlkWriteHeadByIdx(A_BlkIdx);

							// update status
							StatusUpdate();

						}
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrDvrFs6Disk::BlkWriteDataByOfs - bad block offset and size!\n");
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrDvrFs6Disk::BlkWriteDataByOfs - bad block index %d (disk:%d)\n",
					A_BlkIdx, m_DevIdx);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Disk::BlkDbAddData(__u32 A_BlkIdx, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtOfs)
{
	// local -------------------
		__u8	Tv_Result;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result		=	E_GrErrDiskStatBad;
		*A_PtrRtOfs	=	0;

		if(E_GrDvrRecStatEmpty <= m_DskStat)
		{
			Tv_Result	=	E_GrErrBadParam;
			if(m_BlkCnt > A_BlkIdx)
			{
				// check block head
				Tv_Result	=	E_GrErrDiskBadSector;
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)&m_BlkHdBuf[A_BlkIdx];
				if((E_GrDvrFs6Fcc == Tv_PtrDb->Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb->BlkType))
				{
					Tv_Result	=	E_GrErrDiskFull;
					// check full bit
					if(0 == (E_GrDvrFs6BlkAtbFull & Tv_PtrDb->BlkAtb))
					{
						// check able
						if((E_GrDvrFs6BlkSize - sizeof(St_GrDvrFs6BlkDbHead)) >= (Tv_PtrDb->InSize + A_Size))
						{
							Tv_Result	=	E_GrErrBadIo;
							// try write
							Tv_Dpos	=	((__u64)A_BlkIdx << E_GrDvrFs6BlkShift) + (__u64)Tv_PtrDb->InSize + sizeof(St_GrDvrFs6BlkDbHead);
							//if(LcPwrite(Tv_Dpos,A_PtrBuf,A_Size))
							if(LcCchPwrite(Tv_Dpos, A_PtrBuf, A_Size))
							{
								Tv_Result		=	E_GrErrNone;
								*A_PtrRtOfs	=	Tv_PtrDb->InSize + sizeof(St_GrDvrFs6BlkDbHead);
							}
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Disk::StatusUpdate(void)
{
	// local -------------------
		__u32	Tv_BlkIdx;
		Ptr_GrDvrFs6BlkMibHead	Tv_PtrMib;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		if(E_GrDvrRecStatUnformated <= m_DskStat)
		{
			// check MIB
			Tv_PtrMib	=	(Ptr_GrDvrFs6BlkMibHead)m_BlkHdBuf;
			if((E_GrDvrFs6Fcc == Tv_PtrMib->Fcc) && (E_GrDvrFs6BlkTypeMib == Tv_PtrMib->BlkType))
			{
				// check DB data
				m_DskStat	=	E_GrDvrRecStatBad;
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_BlkHdBuf;
				for( Tv_BlkIdx = 1; Tv_BlkIdx < m_BlkCnt; Tv_BlkIdx++)
				{
					if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType))
					{
						// normal
						m_DskStat	=	E_GrDvrRecStatNormal;
						break;
					}
				}
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatUnformated;
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Disk::DiskInfoGet(__u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_BlkIdx;
		__u64	Tv_Total;
		__u64	Tv_RecSize;
		Ptr_GrDvrFs6BlkMibHead	Tv_PtrMib;
		Ptr_GrDvrFs6BlkDbHead	Tv_PtrDb;
	// code --------------------
		Tv_Result		=	m_DskStat;
		Tv_Total		=	0;
		Tv_RecSize	=	0;

		if(E_GrDvrRecStatUnformated <= m_DskStat)
		{
			// check MIB
			Tv_PtrMib	=	(Ptr_GrDvrFs6BlkMibHead)m_BlkHdBuf;
			if((E_GrDvrFs6Fcc == Tv_PtrMib->Fcc) && (E_GrDvrFs6BlkTypeMib == Tv_PtrMib->BlkType))
			{
				// check DB data
				Tv_Result	=	E_GrDvrRecStatBad;
				Tv_PtrDb	=	(Ptr_GrDvrFs6BlkDbHead)m_BlkHdBuf;
				for(Tv_BlkIdx = 1; Tv_BlkIdx < m_BlkCnt; Tv_BlkIdx++)
				{
					if((E_GrDvrFs6Fcc == Tv_PtrDb[Tv_BlkIdx].Fcc) && (E_GrDvrFs6BlkTypeDb == Tv_PtrDb[Tv_BlkIdx].BlkType))
					{
						// normal
						Tv_Result	=	E_GrDvrRecStatNormal;

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
						}

					}
				}
			}
			else
			{
				Tv_Result	=	E_GrDvrRecStatUnformated;
				Tv_Total	=	(__u64)m_BlkCnt * E_GrDvrFs6BlkSize;
			}
		}

		m_DskStat			=	Tv_Result;
		*A_PtrRtTotal	=	Tv_Total;
		*A_PtrRtRec		=	Tv_RecSize;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Disk::BlkReadDataByOfs(__u32 A_BlkIdx, __u32 A_Ofs, void* A_PtrData, __u32 A_Size)
{
	// local -------------------
		__u8	Tv_Result;
		__u64	Tv_Dpos;
	// code --------------------
		Tv_Result	=	E_GrErrDiskStatBad;

		if(E_GrDvrRecStatEmpty <= m_DskStat)
		{
			Tv_Result	=	E_GrErrBadParam;
			// check block index
			if(m_BlkCnt > A_BlkIdx)
			{
				// check offset and size
				if(E_GrDvrFs6BlkSize >= (A_Ofs + A_Size))
				{
					// check bad block
					Tv_Result	=	E_GrErrDiskBadSector;
					if(E_GrDvrFs6BlkTypeBad != m_BlkHdBuf[A_BlkIdx].BlkType)
					{
						Tv_Result	=	E_GrErrNone;
						Tv_Dpos		=	((__u64)A_BlkIdx << E_GrDvrFs6BlkShift) + (__u64)A_Ofs;
						//if(!LcPread(Tv_Dpos, A_PtrData, A_Size))
						if(!LcCchPread(Tv_Dpos, A_PtrData, A_Size))
						{
							Tv_Result	=	E_GrErrBadIo;
							// mark bad block
							m_BlkHdBuf[A_BlkIdx].BlkType	=	E_GrDvrFs6BlkTypeBad;
							BlkWriteHeadByIdx(A_BlkIdx);

							// update status
							StatusUpdate();

						}
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrDvrFs6Disk::BlkReadDataByOfs - bad block offset and size!\n");
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrDvrFs6Disk::BlkReadDataByOfs - bad block index %d (disk:%d)\n",
					A_BlkIdx, m_DevIdx);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Disk::ModelNameGet(WCHAR* A_StrRt)
{
	// local -------------------
	// code --------------------
		GrStrStrToWstr(A_StrRt, m_StrModel);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcCchPread(__u64 A_Adr, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Remain;
		__u32	Tv_Size;
		__u32	Tv_CchOfs;
		__u64	Tv_CchAdr;
		void*	Tv_PtrTg;
	// code --------------------
		Tv_Result	=	TRUE;

		Tv_CchAdr	=	A_Adr & E_GrDvrFs6CchAdrMask;
		Tv_CchOfs	=	(__u32)(A_Adr & E_GrDvrFs6CchOfsMask);
		Tv_Remain	=	A_Size;

		Tv_PtrTg	=	A_PtrBuf;

		while(0 != Tv_Remain)
		{
			// calculate size
			Tv_Size		=	Tv_Remain;
			if((Tv_CchOfs + Tv_Size) >= E_GrDvrFs6CchUnitSize)
			{
				Tv_Size	=	E_GrDvrFs6CchUnitSize - Tv_CchOfs;
			}

			// read cache block
			if(!LcCchBlkRead(Tv_CchAdr,Tv_CchOfs,Tv_PtrTg,Tv_Size))
			{
				Tv_Result	=	FALSE;
				break;
			}

			// next
			Tv_CchOfs		=	0;
			Tv_CchAdr		=	Tv_CchAdr + (__u64)E_GrDvrFs6CchUnitSize;
			Tv_Remain		=	Tv_Remain - Tv_Size;
			Tv_PtrTg		=	(void*)((Def_GrCalPtr)Tv_PtrTg + (Def_GrCalPtr)Tv_Size);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcCchPwrite(__u64 A_Adr, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Remain;
		__u32	Tv_Size;
		__u32	Tv_CchOfs;
		__u64	Tv_CchAdr;
		void*	Tv_PtrSr;
	// code --------------------
		Tv_Result	=	TRUE;

		Tv_CchAdr	=	A_Adr & E_GrDvrFs6CchAdrMask;
		Tv_CchOfs	=	(__u32)(A_Adr & E_GrDvrFs6CchOfsMask);
		Tv_Remain	=	A_Size;

		Tv_PtrSr	=	A_PtrBuf;

		while(0 != Tv_Remain)
		{
			// calculate size
			Tv_Size		=	Tv_Remain;
			if((Tv_CchOfs + Tv_Size) >= E_GrDvrFs6CchUnitSize)
			{
				Tv_Size	=	E_GrDvrFs6CchUnitSize - Tv_CchOfs;
			}

			// write cache block
			if(!LcCchBlkWrite(Tv_CchAdr, Tv_CchOfs, Tv_PtrSr, Tv_Size))
			{
				Tv_Result	=	FALSE;
				break;
			}

			// next
			Tv_CchOfs		=	0;
			Tv_CchAdr		=	Tv_CchAdr + (__u64)E_GrDvrFs6CchUnitSize;
			Tv_Remain		=	Tv_Remain - Tv_Size;
			Tv_PtrSr		=	(void*)((Def_GrCalPtr)Tv_PtrSr + (Def_GrCalPtr)Tv_Size);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcCchBlkRead(__u64 A_BlkAdr, __u32 A_BlkOfs, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_CchIdx;
		void*	Tv_PtrSr;
	// code --------------------
		Tv_Result	=	FALSE;

		// find item index
		Tv_CchIdx	=	LcCchFindByAdr(A_BlkAdr);
		if(0 > Tv_CchIdx)
		{
			// find empty
			Tv_CchIdx	=	LcCchFindEmpty();
			if(0 > Tv_CchIdx)
			{
				// find oldest item
				Tv_CchIdx	=	LcCchFindOldest();
				if(0 <= Tv_CchIdx)
				{
					// flush
					LcCchItemSave(Tv_CchIdx);
				}
			}
			
			// load cache
			if(0 <= Tv_CchIdx)
			{
				if(!LcCchItemLoad(Tv_CchIdx,A_BlkAdr))
				{
					// read fail
					Tv_CchIdx	=	-1;
				}
			}

		}

		if(0 <= Tv_CchIdx)
		{
			// copy
			Tv_PtrSr	=	(void*)((Def_GrCalPtr)V_GrDvrFs6Env.Cch.PtrBuf + (Def_GrCalPtr)(Tv_CchIdx << E_GrDvrFs6CchUnitShft)
				+ (Def_GrCalPtr)A_BlkOfs);
			GrDumyCopyMem(A_PtrBuf, Tv_PtrSr, A_Size);
			// update tick
			V_GrDvrFs6Env.Cch.CtlTbl[Tv_CchIdx].AccTick	=	V_GrDvrFs6Env.TickFs;

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcCchBlkWrite(__u64 A_BlkAdr, __u32 A_BlkOfs, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_CchIdx;
		void*	Tv_PtrTg;
	// code --------------------
		Tv_Result	=	FALSE;

		// find item index
		Tv_CchIdx	=	LcCchFindByAdr(A_BlkAdr);
		if(0 > Tv_CchIdx)
		{
			// write through
			if((0 == A_BlkOfs) && (E_GrDvrFs6CchUnitSize == A_Size))
			{
				// write through
				Tv_Result	=	LcPwrite(A_BlkAdr, A_PtrBuf, E_GrDvrFs6CchUnitSize);
			}
			else
			{
				// find empty
				Tv_CchIdx	=	LcCchFindEmpty();
				if(0 > Tv_CchIdx)
				{
					// find oldest item
					Tv_CchIdx	=	LcCchFindOldest();
					if(0 <= Tv_CchIdx)
					{
						// flush
						LcCchItemSave(Tv_CchIdx);
					}
				}

				// load cache
				if(0 <= Tv_CchIdx)
				{
					if(!LcCchItemLoad(Tv_CchIdx, A_BlkAdr))
					{
						// read fail
						Tv_CchIdx	=	-1;
					}
				}

			}
		}

		if(0 <= Tv_CchIdx)
		{
			// copy
			Tv_PtrTg	=	(void*)((Def_GrCalPtr)V_GrDvrFs6Env.Cch.PtrBuf + (Def_GrCalPtr)(Tv_CchIdx << E_GrDvrFs6CchUnitShft)
				+ (Def_GrCalPtr)A_BlkOfs);
			GrDumyCopyMem(Tv_PtrTg, A_PtrBuf, A_Size);
			// update tick
			V_GrDvrFs6Env.Cch.CtlTbl[Tv_CchIdx].AccTick	=	V_GrDvrFs6Env.TickFs;
			V_GrDvrFs6Env.Cch.CtlTbl[Tv_CchIdx].IsDirt	=	TRUE;		// dirt

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFs6Disk::LcCchFindByAdr(__u64 A_DskAdr)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
	// code --------------------
		Tv_Result		=	-1;

		for( Tv_WkIdx = 0; Tv_WkIdx < V_GrDvrFs6Env.Cch.ItemCnt; Tv_WkIdx++)
		{
			if(V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].IsUse && (V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].DskId == m_DevIdx) &&
				(V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].DskAdr == A_DskAdr))
			{
				// found
				Tv_Result	=	Tv_WkIdx;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFs6Disk::LcCchFindEmpty(void)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for(Tv_WkIdx = 0; Tv_WkIdx < V_GrDvrFs6Env.Cch.ItemCnt; Tv_WkIdx++)
		{
			if(!V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].IsUse)
			{
				// found
				Tv_Result	=	Tv_WkIdx;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFs6Disk::LcCchFindOldest(void)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
		__u32	Tv_FndTick;
	// code --------------------
		Tv_Result	=	-1;

		for(Tv_WkIdx = 0; Tv_WkIdx < V_GrDvrFs6Env.Cch.ItemCnt; Tv_WkIdx++)
		{
			if(V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].IsUse)
			{
				if(0 > Tv_Result)
				{
					Tv_Result		=	Tv_WkIdx;
					Tv_FndTick	=	V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].AccTick;
				}
				else
				{
					// compare oldest
					if(Tv_FndTick > V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].AccTick)
					{
						Tv_Result		=	Tv_WkIdx;
						Tv_FndTick	=	V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].AccTick;
					}
				}

			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcCchItemSave(__s32 A_ItmIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		void*	Tv_PtrBuf;
	// code --------------------
		Tv_Result	=	FALSE;

		if((0 <= A_ItmIdx) && ((__s32)V_GrDvrFs6Env.Cch.ItemCnt > A_ItmIdx))
		{
			if(V_GrDvrFs6Env.Cch.CtlTbl[A_ItmIdx].IsUse && (V_GrDvrFs6Env.Cch.CtlTbl[A_ItmIdx].DskId == m_DevIdx))
			{
				Tv_Result		=	TRUE;
				if(V_GrDvrFs6Env.Cch.CtlTbl[A_ItmIdx].IsDirt)
				{
					// write
					Tv_PtrBuf	=	(void*)((Def_GrCalPtr)V_GrDvrFs6Env.Cch.PtrBuf + (Def_GrCalPtr)(A_ItmIdx << E_GrDvrFs6CchUnitShft));
					Tv_Result	=	LcPwrite(V_GrDvrFs6Env.Cch.CtlTbl[A_ItmIdx].DskAdr, Tv_PtrBuf, E_GrDvrFs6CchUnitSize);
					// reset dirt
					V_GrDvrFs6Env.Cch.CtlTbl[A_ItmIdx].IsDirt	=	FALSE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Disk::LcCchItemLoad(__s32 A_ItmIdx, __u64 A_DskAdr)
{
	// local -------------------
		BOOL8	Tv_Result;
		void*	Tv_PtrBuf;
	// code --------------------
		Tv_Result	=	FALSE;

		if((0 <= A_ItmIdx) && ((__s32)V_GrDvrFs6Env.Cch.ItemCnt > A_ItmIdx))
		{
			// update value
			V_GrDvrFs6Env.Cch.CtlTbl[A_ItmIdx].IsDirt	=	FALSE;
			V_GrDvrFs6Env.Cch.CtlTbl[A_ItmIdx].DskId		=	m_DevIdx;
			V_GrDvrFs6Env.Cch.CtlTbl[A_ItmIdx].DskAdr		=	A_DskAdr;
			V_GrDvrFs6Env.Cch.CtlTbl[A_ItmIdx].AccTick	=	V_GrDvrFs6Env.TickFs;

			// read
			Tv_PtrBuf	=	(void*)((Def_GrCalPtr)V_GrDvrFs6Env.Cch.PtrBuf + (Def_GrCalPtr)(A_ItmIdx << E_GrDvrFs6CchUnitShft));
			Tv_Result	=	LcPread(V_GrDvrFs6Env.Cch.CtlTbl[A_ItmIdx].DskAdr, Tv_PtrBuf, E_GrDvrFs6CchUnitSize);

			// update use
			V_GrDvrFs6Env.Cch.CtlTbl[A_ItmIdx].IsUse	=	Tv_Result;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
