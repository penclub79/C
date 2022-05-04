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

#include <Dvr/Fs/Nvt/GrFsNvtBase.h>
#include <Dvr/Fs/Nvt/GrFsNvtHdd.h>

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

#include <errno.h>

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

extern	St_GrFsNvtEnv	V_GrFsNvtEnv;

//====================================================================

//--------------------------------------------------------------------
Cls_GrFsNvtHdd::Cls_GrFsNvtHdd(__u8 A_DevIdx)
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

		m_BlkInfoBuf		=	NULL;
		m_BlkCnt				=	0;

		m_DayInfoTbl		=	NULL;

#ifdef LINUX_APP
		m_HndlDsk	=	-1;
#else
		m_HndlDsk	=	INVALID_HANDLE_VALUE;
#endif

		GrStrClear(m_StrModel);

		// check structural
		if ( 512 != sizeof(St_GrFsNvtSmartData) )
		{
			DbgMsgPrint( "Cls_GrFsNvtHdd: Smart struct size ( %d ) mismatch. \n", sizeof(St_GrFsNvtSmartData) );
		}

		if ( GrDvrBiosRecDevNameGet( m_DevIdx, Tv_StrDev ) )
		{
			DbgMsgPrint( "Cls_GrFsNvtHdd: device ( %d : %s ) found. \n", m_DevIdx, Tv_StrDev );
#ifdef LINUX_APP
			m_HndlDsk	=	open( Tv_StrDev, O_RDWR | O_DIRECT);
			if ( 0 <= m_HndlDsk )
			{
				DbgMsgPrint( "Cls_GrFsNvtHdd: device ( %d ) open success. \n", m_DevIdx );
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
					DbgMsgPrint( "Cls_GrFsNvtHdd: device ( %d ) get size error. \n", m_DevIdx );
				}
			}
#else
			if(('\\' != Tv_StrDev[0]) || '\\' != Tv_StrDev[1])
			{
				GrStrCopy(Tv_StrPath, GrFileCtrlEmuPathGetA());
			}
			else
			{
				GrStrClear(Tv_StrPath);
			}
			
			GrStrCat( Tv_StrPath, Tv_StrDev );
			m_HndlDsk	=	CreateFileA( Tv_StrPath, GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
			if ( INVALID_HANDLE_VALUE != m_HndlDsk )
			{
#ifndef DVR_EMU_PHYDSK
				if ( GetFileSizeEx( m_HndlDsk, (PLARGE_INTEGER)(&m_DskSize) ) )
				{
					m_DskStat		=	E_GrDvrRecStatUnformated;
				}
				else
				{
					CloseHandle( m_HndlDsk );
					m_HndlDsk	=	INVALID_HANDLE_VALUE;				
				}
#else
				{
					DISK_GEOMETRY	Tv_Dgm;
					DWORD	Tv_RtSize;
					BOOL	Tv_Rslt;

					Tv_Rslt = DeviceIoControl(m_HndlDsk,
						IOCTL_DISK_GET_DRIVE_GEOMETRY, 
						NULL, 0,                       
						&Tv_Dgm, sizeof(Tv_Dgm),
						&Tv_RtSize,
						(LPOVERLAPPED)NULL);
					if(Tv_Rslt)
					{
						m_DskSize	=	(__u64)Tv_Dgm.Cylinders.QuadPart * (__u64)Tv_Dgm.TracksPerCylinder *
							(__u64)Tv_Dgm.SectorsPerTrack * (__u64)Tv_Dgm.BytesPerSector;
						m_DskStat		=	E_GrDvrRecStatUnformated;
					}
					else
					{
						CloseHandle(m_HndlDsk);
						m_HndlDsk	=	INVALID_HANDLE_VALUE;
					}
				}
#endif
			}
			else
			{
				__u32	Tv_ErrCode;
				Tv_ErrCode	=	GetLastError();
				DbgMsgPrint("[DSK] error %d \n", Tv_ErrCode);
			}
#endif

			if(E_GrDvrRecStatNotexist != m_DskStat)
			{
				// update model name
				GrDvrBiosRecDevModelNameGet(m_DevIdx, m_StrModel);
				DbgMsgPrint("[HDD] %d model: %s\n", m_DevIdx, m_StrModel);
			}

		}

		//initialize
		if ( E_GrDvrRecStatUnformated == m_DskStat )
		{
			// check disk size
			if ((E_GrFsNvtBlkSize * 4) <= m_DskSize)
			{
				// disk status update
				DbgMsgPrint("[HDD] dev %d try init!\n", m_DevIdx);
				LcInit();
				DbgMsgPrint("[HDD] dev %d end init!\n", m_DevIdx);
			}
			else
			{
				DbgMsgPrint("[HDD] dev %d Stat bad\n", m_DevIdx);
				m_DskStat	=	E_GrDvrRecStatBad;

				{
					// do error notify
					St_GrTaskMsgDskIoErr	Tv_MsgErr;

					Tv_MsgErr.MsgHd.Cmd	=	E_GrTaskMsgHddIoError;
					Tv_MsgErr.DiskIdx		=	m_DevIdx;
					GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgErr, sizeof(Tv_MsgErr));
				}

			}
		}
		
}
//--------------------------------------------------------------------
Cls_GrFsNvtHdd::~Cls_GrFsNvtHdd()
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

		if (NULL != m_BlkInfoBuf)
		{
#ifdef LINUX_APP
			munmap(m_BlkInfoBuf, m_BiBufSize);
#else
			free(m_BlkInfoBuf);
#endif
			m_BlkInfoBuf	=	NULL;
		}

		if(NULL != m_DayInfoTbl)
		{
#ifdef LINUX_APP
			munmap(m_DayInfoTbl, E_GrFsNvtHddDayInfoBufSize);
#else
			free(m_DayInfoTbl);
#endif
			m_DayInfoTbl	=	NULL;
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtHdd::LcFlush( void )
{
#ifdef LINUX_APP
		if ( -1 != m_HndlDsk )
		{
			fsync( m_HndlDsk );
		}
#endif
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtHdd::LcInit( void )
{
	// local -------------------
	// code --------------------
		m_DskStat	=	E_GrDvrRecStatUnformated;

		// calculate block count
		m_BlkCnt	=	(__u32)(m_DskSize >> E_GrFsNvtBlkShift);
		if(E_GrFsNvtBlkMaxCnt < m_BlkCnt)
		{
			m_BlkCnt	=	E_GrFsNvtBlkMaxCnt;
		}

		// allocate block buffer
		m_BiBufSize	=	sizeof(St_GrFsNvtBlkInfoBase)* m_BlkCnt;

		m_BiBufSize	=	(m_BiBufSize + (E_GrFsNvtHddIoUnitSize - 1)) & E_GrFsNvtHddIoAdrMask;

#ifdef LINUX_APP
		m_BlkInfoBuf	=	(Ptr_GrFsNvtBlkInfoBase)mmap(NULL, m_BiBufSize, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, -1, 0);
		if( 0xFFFFFFFF == (__u32)m_BlkInfoBuf)
		{
			DbgMsgPrint("[HDD] blkinfo mmap error %d\n", errno);
			m_BlkInfoBuf	=	NULL;
		}
#else
		m_BlkInfoBuf	=	(Ptr_GrFsNvtBlkInfoBase)malloc(m_BiBufSize);
#endif

		if(NULL == m_BlkInfoBuf)
		{
			DbgMsgPrint("Cls_GrFsNvtHdd::LcInit - block info buffer allocate fail!\n");
			m_DskSize		=	E_GrDvrRecStatBad;
			return;
		}

		// day table allocate
#ifdef LINUX_APP
		m_DayInfoTbl	=	(Ptr_GrFsNvtDayInfo)mmap(NULL, E_GrFsNvtHddDayInfoBufSize, PROT_READ|PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED ,
			-1, 0);
		if(0xFFFFFFFF == (__u32)m_DayInfoTbl)
		{
			DbgMsgPrint("[HDD] day table mmap error %d\n", errno);
			m_DayInfoTbl	=	NULL;
		}
#else
		m_DayInfoTbl	=	(Ptr_GrFsNvtDayInfo)malloc(E_GrFsNvtHddDayInfoBufSize);
#endif

		if(NULL == m_DayInfoTbl)
		{
			DbgMsgPrint("Cls_GrFsNvtHdd::LcInit - day info buffer allocate fail!\n");
			m_DskSize		=	E_GrDvrRecStatBad;
			return;
		}

		// read mib
		if(E_GrErrNone != BlkReadPage(E_GrFsNvtBlkIdxMib, 0, m_BlkInfoBuf, m_BiBufSize))
		{
			// mib bad block
			DbgMsgPrint("[HDD] disk %d mib read fail!\n", m_DevIdx);
			m_DskStat	=	E_GrDvrRecStatBad;

			{
				// do error notify
				St_GrTaskMsgDskIoErr	Tv_MsgErr;

				Tv_MsgErr.MsgHd.Cmd	=	E_GrTaskMsgHddIoError;
				Tv_MsgErr.DiskIdx		=	m_DevIdx;
				GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgErr, sizeof(Tv_MsgErr));
			}

			return;
		}
		
		// read day info
		if(E_GrErrNone != BlkReadPage(E_GrFsNvtBlkIdxMib, E_GrFsNvtBlkOfsDayTbl, m_DayInfoTbl, E_GrFsNvtHddDayInfoBufSize))
		{
			// mib bad block
			m_DskStat	=	E_GrDvrRecStatBad;
			DbgMsgPrint("[HDD] disk %d day info read fail!\n", m_DevIdx);

			{
				// do error notify
				St_GrTaskMsgDskIoErr	Tv_MsgErr;

				Tv_MsgErr.MsgHd.Cmd	=	E_GrTaskMsgHddIoError;
				Tv_MsgErr.DiskIdx		=	m_DevIdx;
				GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgErr, sizeof(Tv_MsgErr));
			}

			return;
		}

		// check formatted
		m_DskStat	=	E_GrDvrRecStatUnformated;
		if(E_GrFsNvtFcc == m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.Fcc)
		{
			// check version
			if( m_BlkCnt == m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.BlkCnt)
			{
				// patch minute table
				if(E_GrFsNvtVer > m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.FsVer)
				{
					// patch minute table
					DbgMsgPrint("Cls_GrFsNvtHdd::LcInit - disk %d patch minute table (old:%d , new:%d)\n", 
						m_DevIdx, m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.FsVer, E_GrFsNvtVer);

					LcCvtMit();

					// check error
					if(E_GrDvrRecStatBad == m_DskStat)
					{
						return;
					}
				}
				// check version
				if(E_GrFsNvtVer == m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.FsVer)
				{
					StatusUpdate();
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrFsNvtHdd::LcInit - disk %d block bad (mib:%d , chk:%d)\n", m_DevIdx, m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.BlkCnt,m_BlkCnt);
			}
		}
		else
		{
			DbgMsgPrint("[HDD] disk %d fcc not match!\n", m_DevIdx);
		}
	
}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtHdd::DevIdxGet( void )
{
		return	m_DevIdx;
}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtHdd::DiskStatGet( void )
{
	// local -------------------
	// code --------------------

		return	m_DskStat;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtHdd::LcSmartRead( void )
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
BOOL8	Cls_GrFsNvtHdd::LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_BytePos;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_BytePos	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrFsNvtHddSmartValCnt;Tv_WkIdx++ )
		{
			if ( A_Id == m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxId] )
			{
				*A_PtrRtVal	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal];
				Tv_Result	=	TRUE;
				break;
			}
			//next
			Tv_BytePos	=	Tv_BytePos + E_GrFsNvtHddSmartValItemSize;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtHdd::LcSmartFindRawDword(__u8 A_Id, __u32* A_PtrRtVal)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_BytePos;
		__u8*	Tv_PtrTg;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_BytePos	=	0;
		for(Tv_WkIdx=0; Tv_WkIdx < E_GrFsNvtHddSmartValCnt; Tv_WkIdx++)
		{
			if(A_Id == m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxId])
			{
				Tv_PtrTg		=	(__u8*)A_PtrRtVal;
				Tv_PtrTg[0]	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal + 0];
				Tv_PtrTg[1]	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal + 1];
				Tv_PtrTg[2]	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal + 2];
				Tv_PtrTg[3]	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal + 3];

				Tv_Result	=	TRUE;
				break;
			}
			//next
			Tv_BytePos	=	Tv_BytePos + E_GrFsNvtHddSmartValItemSize;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtHdd::LcSmartFindRawQword(__u8 A_Id, __u64* A_PtrRtVal)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_BytePos;
		__u8*	Tv_PtrTg;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_BytePos	=	0;
		for(Tv_WkIdx=0; Tv_WkIdx < E_GrFsNvtHddSmartValCnt; Tv_WkIdx++)
		{
			if(A_Id == m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxId])
			{
				Tv_PtrTg		=	(__u8*)A_PtrRtVal;
				Tv_PtrTg[0]	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal + 0];
				Tv_PtrTg[1]	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal + 1];
				Tv_PtrTg[2]	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal + 2];
				Tv_PtrTg[3]	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal + 3];
				Tv_PtrTg[4]	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal + 4];
				Tv_PtrTg[5]	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal + 5];
				Tv_PtrTg[6]	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal + 6];
				Tv_PtrTg[7]	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrFsNvtHddSmartValIdxRawVal + 7];

				Tv_Result	=	TRUE;
				break;
			}
			//next
			Tv_BytePos	=	Tv_BytePos + E_GrFsNvtHddSmartValItemSize;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrFsNvtBlkInfoBase	Cls_GrFsNvtHdd::BlkInfoTableGet(__u32* A_PtrRtCnt, BOOL8 A_IsForce)
{
	// local -------------------
		Ptr_GrFsNvtBlkInfoBase	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCnt	=	0;

		if((E_GrDvrRecStatEmpty <= m_DskStat) || A_IsForce)
		{
			*A_PtrRtCnt	=	m_BlkCnt;
			Tv_Result		=	m_BlkInfoBuf;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtHdd::StatusUpdate(void)
{
	// local -------------------
		__u32	Tv_BlkIdx;
	// code --------------------
		if(E_GrDvrRecStatUnformated <= m_DskStat)
		{
			// check MIB
			if(E_GrFsNvtFcc == m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.Fcc)
			{
				// check DB data
				m_DskStat	=	E_GrDvrRecStatBad;
				for( Tv_BlkIdx = 1; Tv_BlkIdx < m_BlkCnt; Tv_BlkIdx++)
				{
					if(E_GrFsNvtBlkStatBad != m_BlkInfoBuf[Tv_BlkIdx].Db.BlkStat)
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
__u8	Cls_GrFsNvtHdd::DiskInfoGet(__u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_BlkIdx;
		__u64	Tv_Total;
		__u64	Tv_RecSize;
	// code --------------------
		Tv_Result		=	m_DskStat;
		Tv_Total		=	0;
		Tv_RecSize	=	0;

		if(E_GrDvrRecStatUnformated <= m_DskStat)
		{
			// check MIB
			if((E_GrFsNvtFcc == m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.Fcc) && (E_GrFsNvtVer == m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.FsVer))
			{
				// check DB data
				Tv_Result	=	E_GrDvrRecStatBad;
				for(Tv_BlkIdx = 1; Tv_BlkIdx < m_BlkCnt; Tv_BlkIdx++)
				{
					if(E_GrFsNvtBlkStatBad != m_BlkInfoBuf[Tv_BlkIdx].Db.BlkStat)
					{
						// normal
						Tv_Result	=	E_GrDvrRecStatNormal;

						Tv_Total	=	Tv_Total + E_GrFsNvtBlkSize;

						// check full block
						if(E_GrFsNvtBlkStatFull == m_BlkInfoBuf[Tv_BlkIdx].Db.BlkStat)
						{
							Tv_RecSize	=	Tv_RecSize + E_GrFsNvtBlkSize;
						}
						else
						{
							Tv_RecSize	=	Tv_RecSize + (__u64)m_BlkInfoBuf[Tv_BlkIdx].Db.InSize;
						}
					}
				}
			}
			else
			{
				Tv_Result	=	E_GrDvrRecStatUnformated;
				Tv_Total	=	(__u64)m_BlkCnt * E_GrFsNvtBlkSize;
			}
		}

		m_DskStat			=	Tv_Result;
		*A_PtrRtTotal	=	Tv_Total;
		*A_PtrRtRec		=	Tv_RecSize;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtHdd::ModelNameGet(WCHAR* A_StrRt)
{
	// local -------------------
	// code --------------------
		GrStrStrToWstr(A_StrRt, m_StrModel);
}
//--------------------------------------------------------------------
Ptr_GrFsNvtDayInfo	Cls_GrFsNvtHdd::DayInfoTableGet(void)
{
		return	m_DayInfoTbl;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtHdd::DayInfoWrite(void)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		if(E_GrDvrRecStatUnformated > m_DskStat)
		{
			return	E_GrErrDiskStatBad;
		}

		Tv_ErrCode	=	BlkWritePage(E_GrFsNvtBlkIdxMib, E_GrFsNvtBlkOfsDayTbl, m_DayInfoTbl, E_GrFsNvtHddDayInfoBufSize);
		if(E_GrErrNone != Tv_ErrCode)
		{
			// mib bad block
			m_DskStat	=	E_GrDvrRecStatBad;

			{
				// do error notify
				St_GrTaskMsgDskIoErr	Tv_MsgErr;

				Tv_MsgErr.MsgHd.Cmd	=	E_GrTaskMsgHddIoError;
				Tv_MsgErr.DiskIdx		=	m_DevIdx;
				GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgErr, sizeof(Tv_MsgErr));
			}
		}

		return	Tv_ErrCode;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtHdd::SmartRead(Ptr_GrFsNvtSmart A_PtrRtData)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Val64;
		__u8*	Tv_PtrVal;
	// code --------------------
		Tv_Result		=	FALSE;

#ifdef LINUX_EMU
		A_PtrRtData->Poh			=	10000;
		A_PtrRtData->Temp			=	43;
		A_PtrRtData->BadSect	=	2;
		A_PtrRtData->SeekErr	= 11;
		A_PtrRtData->Reallocate	= 100;
		A_PtrRtData->ReadErr	= 22;
		return	TRUE;
#endif
		

		if(LcSmartRead())
		{
			GrDumyZeroMem(A_PtrRtData, sizeof(St_GrFsNvtSmart));

			LcSmartFindRawQword(E_GrFsNvtHddSmartIdSeekErrRate, &Tv_Val64);
			Tv_PtrVal = (__u8*)&Tv_Val64;
			A_PtrRtData->SeekErr	= (Tv_PtrVal[5] << 8) | Tv_PtrVal[4];

			LcSmartFindRawDword(E_GrFsNvtHddSmartIdPoh, &A_PtrRtData->Poh);
			LcSmartFindRawByte(E_GrFsNvtHddSmartIdTemp, &A_PtrRtData->Temp);
			LcSmartFindRawDword(E_GrFsNvtHddSmartIdReallocate, &A_PtrRtData->Reallocate);
			LcSmartFindRawDword(E_GrFsNvtHddSmartIdBadSect, &A_PtrRtData->BadSect);
			LcSmartFindRawDword(E_GrFsNvtHddSmartIdReadErrRate, &A_PtrRtData->ReadErr);
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtHdd::BlkInfoWriteByIdx(__u32 A_BlkIdx)
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Size;
		__u32	Tv_Bofs;
		void*	Tv_PtrBuf;
		Def_GrErrCode	Tv_ErrCode;
	// code --------------------
		// check block buffer exist
		if(NULL == m_BlkInfoBuf)
		{
			return	E_GrErrInitFail;
		}

		// check block index range
		if(m_BlkCnt <= A_BlkIdx)
		{
			DbgMsgPrint("Cls_GrFsNvtHdd::BlkInfoWriteByIdx - bad block index (%d:%d) input!\n", A_BlkIdx,m_BlkCnt);
			return	E_GrErrBadParam;
		}

		// calculate address range
		Tv_Ofs	=	A_BlkIdx * sizeof(St_GrFsNvtBlkInfoBase);

		Tv_Bofs		=	Tv_Ofs &  E_GrFsNvtHddIoAdrMask;
		Tv_Size		=	(Tv_Ofs & E_GrFsNvtHddIoOfsMask) + sizeof(St_GrFsNvtBlkInfoBase);
		Tv_Size		=	(Tv_Size + (E_GrFsNvtHddIoUnitSize - 1)) & E_GrFsNvtHddIoAdrMask;

		Tv_PtrBuf	=	(void*)((Def_GrCalPtr)m_BlkInfoBuf + (Def_GrCalPtr)Tv_Bofs);

		Tv_ErrCode	=	BlkWritePage(E_GrFsNvtBlkIdxMib, Tv_Bofs, Tv_PtrBuf, Tv_Size);
		
		return	Tv_ErrCode;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtHdd::BlkInfoWriteAll(void)
{
	// local -------------------
	// code --------------------

		// check block buffer exist
		if(NULL == m_BlkInfoBuf)
		{
			return	FALSE;
		}

		if(E_GrErrNone != BlkWritePage(E_GrFsNvtBlkIdxMib, 0, m_BlkInfoBuf, m_BiBufSize))
		{
			return	FALSE;
		}

		return	TRUE;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtHdd::BlkReadPage(__u32 A_BlkIdx, __u32 A_Ofs, void* A_PtrData, __u32 A_Size)
{
	// local -------------------
		__u64	Tv_Dpos;
	// code --------------------
		// check disk status
		if(E_GrDvrRecStatUnformated > m_DskStat)
		{
			return	E_GrErrDiskStatBad;
		}

		// check block index
		if(m_BlkCnt <= A_BlkIdx)
		{
			return	E_GrErrBadParam;
		}

		// check offset align
		if(0 != (E_GrFsNvtHddIoOfsMask & A_Ofs))
		{
			DbgMsgPrint("Cls_GrFsNvtHdd::BlkReadPage - bad offset (%08X) input!\n", A_Ofs);
			return	E_GrErrBadParam;
		}

		// check size align
		if((0 == A_Size) || (0 != (E_GrFsNvtHddIoOfsMask & A_Size)))
		{
			DbgMsgPrint("Cls_GrFsNvtHdd::BlkReadPage - bad size (%08X) input!\n", A_Size);
			return	E_GrErrBadParam;
		}

		// check size over
		if((__u32)E_GrFsNvtBlkSize < (A_Ofs + A_Size))
		{
			DbgMsgPrint("Cls_GrFsNvtHdd::BlkReadPage - position over (ofs:%08X size:%08X) input!\n", A_Ofs, A_Size);
			return	E_GrErrBadParam;
		}

		// check bad block
		if((E_GrFsNvtBlkIdxMib != A_BlkIdx) && (E_GrFsNvtBlkStatBad == m_BlkInfoBuf[A_BlkIdx].Db.BlkStat))
		{
			DbgMsgPrint("Cls_GrFsNvtHdd::BlkReadPage - E_GrErrDiskBadSector. BlkIdx = %d blkStat = %d\n", A_BlkIdx, m_BlkInfoBuf[A_BlkIdx].Db.BlkStat);
			return	E_GrErrDiskBadSector;
		}

		// calculate disk address
		Tv_Dpos			=	((__u64)A_BlkIdx << E_GrFsNvtBlkShift) + (__u64)A_Ofs;
#ifdef LINUX_APP
		{
			ssize_t		Tv_ReadSize;
			memset(A_PtrData, 0, A_Size);
			Tv_ReadSize = pread64(m_HndlDsk, A_PtrData, (size_t)A_Size, (__off64_t)Tv_Dpos);

			if(((ssize_t)A_Size) == pread64(m_HndlDsk, A_PtrData, (size_t)A_Size, (__off64_t)Tv_Dpos))
			{
				return	E_GrErrNone;
			}
			else
			{
				DbgMsgPrint("Cls_GrFsNvtHdd::BlkReadPage - adr %08X size = %d Dpos = 0x%x error %d\n", (__u32)A_PtrData, A_Size, Tv_Dpos, errno);
			}
		}
#else
		{
			LARGE_INTEGER		Tv_RtSeek;
			LARGE_INTEGER		Tv_SeekAdr;
			DWORD		Tv_Readed;

			Tv_SeekAdr.QuadPart	=	(LONGLONG)Tv_Dpos;
			if(SetFilePointerEx(m_HndlDsk, Tv_SeekAdr, &Tv_RtSeek, FILE_BEGIN))
			{
				if(ReadFile(m_HndlDsk, A_PtrData, A_Size, (LPDWORD)&Tv_Readed, NULL))
				{
					return	E_GrErrNone;
				}
			}

		}
#endif

		// mark bad block
		// error
		if(E_GrFsNvtBlkIdxMib != A_BlkIdx)
		{
			m_BlkInfoBuf[A_BlkIdx].Db.BlkStat	=	E_GrFsNvtBlkStatBad;
			// write mib by index
			BlkInfoWriteByIdx(A_BlkIdx);
			// update status
			StatusUpdate();
		}
		else
		{
			m_DskStat	=	E_GrDvrRecStatBad;

			{
				// do error notify
				St_GrTaskMsgDskIoErr	Tv_MsgErr;

				Tv_MsgErr.MsgHd.Cmd	=	E_GrTaskMsgHddIoError;
				Tv_MsgErr.DiskIdx		=	m_DevIdx;
				GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgErr, sizeof(Tv_MsgErr));
			}

		}

		return	E_GrErrBadIo;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtHdd::BlkWritePage(__u32 A_BlkIdx, __u32 A_Ofs, void* A_PtrData, __u32 A_Size)
{
	// local -------------------
		__u64	Tv_Dpos;
	// code --------------------

		// check disk status
		if(E_GrDvrRecStatUnformated > m_DskStat)
		{
			return	E_GrErrDiskStatBad;
		}

		// check block index
		if(m_BlkCnt <= A_BlkIdx)
		{
			return	E_GrErrBadParam;
		}

		// check offset align
		if(0 != (E_GrFsNvtHddIoOfsMask & A_Ofs))
		{
			DbgMsgPrint("Cls_GrFsNvtHdd::BlkWritePage - bad offset (%08X) input!\n", A_Ofs);
			return	E_GrErrBadParam;
		}

		// check size align
		if((0 == A_Size) || (0 != (E_GrFsNvtHddIoOfsMask & A_Size)))
		{
			DbgMsgPrint("Cls_GrFsNvtHdd::BlkWritePage - bad size (%08X) input!\n", A_Size);
			return	E_GrErrBadParam;
		}

		// check size over
		if((__u32)E_GrFsNvtBlkSize < (A_Ofs + A_Size))
		{
			DbgMsgPrint("Cls_GrFsNvtHdd::BlkWritePage - position over (ofs:%08X size:%08X) input!\n", A_Ofs, A_Size);
			return	E_GrErrBadParam;
		}

		// check bad block
		if((E_GrFsNvtBlkIdxMib != A_BlkIdx) && (E_GrFsNvtBlkStatBad == m_BlkInfoBuf[A_BlkIdx].Db.BlkStat))
		{
			DbgMsgPrint("Cls_GrFsNvtHdd::BlkWritePage - disk bad sector BlkIdx = %d Stat = %d\n", A_BlkIdx, m_BlkInfoBuf[A_BlkIdx].Db.BlkStat);
			return	E_GrErrDiskBadSector;
		}

		// calculate disk address
		Tv_Dpos			=	((__u64)A_BlkIdx << E_GrFsNvtBlkShift) + (__u64)A_Ofs;
#ifdef LINUX_APP
		{
			ssize_t		Tv_WritedSize;
			Tv_WritedSize	= pwrite64(m_HndlDsk,A_PtrData,(size_t)A_Size,(__off64_t)Tv_Dpos);

			if (((ssize_t)A_Size) == pwrite64(m_HndlDsk,A_PtrData,(size_t)A_Size,(__off64_t)Tv_Dpos))
			{
				return	E_GrErrNone;
			}
			else
			{
				DbgMsgPrint("Cls_GrFsNvtHdd::BlkWritePage - adr %08X size = %d Dpos = 0x%x error %d\n", (__u32)A_PtrData, A_Size, Tv_Dpos, errno);
			}
		}
#else
		{
			LARGE_INTEGER		Tv_RtSeek;
			LARGE_INTEGER		Tv_SeekAdr;
			DWORD		Tv_Writed;

			Tv_SeekAdr.QuadPart	=	(LONGLONG)Tv_Dpos;
			if(SetFilePointerEx(m_HndlDsk, Tv_SeekAdr, &Tv_RtSeek, FILE_BEGIN))
			{
				if(WriteFile(m_HndlDsk, A_PtrData, A_Size, (LPDWORD)&Tv_Writed, NULL))
				{
					return	E_GrErrNone;
				}
			}

		}
#endif

		// mark bad block
		// error
		if(E_GrFsNvtBlkIdxMib != A_BlkIdx)
		{
			m_BlkInfoBuf[A_BlkIdx].Db.BlkStat	=	E_GrFsNvtBlkStatBad;
			// write mib by index
			BlkInfoWriteByIdx(A_BlkIdx);
			// update status
			StatusUpdate();
		}
		else
		{
			m_DskStat	=	E_GrDvrRecStatBad;

			{
				// do error notify
				St_GrTaskMsgDskIoErr	Tv_MsgErr;

				Tv_MsgErr.MsgHd.Cmd	=	E_GrTaskMsgHddIoError;
				Tv_MsgErr.DiskIdx		=	m_DevIdx;
				GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgErr, sizeof(Tv_MsgErr));
			}

		}

		return	E_GrErrBadIo;

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtHdd::LcCvtMit(void)
{
	// local -------------------
		void*	Tv_PtrRd;
		void*	Tv_PtrWt;

		__s32	Tv_DayIdx;

		__u32	Tv_BofsRd;
		__u32	Tv_OfsSr;
		__u32	Tv_RdSize;

		void*	Tv_PtrSr;

		__u32	Tv_BofsWt;

		Ptr_GrFsNvtDayInfo	Tv_PtrDit;		// new day info
	// code --------------------

		// allocate buffer
		Tv_PtrDit	=	(Ptr_GrFsNvtDayInfo)malloc(E_GrFsNvtHddDayInfoBufSize);

#ifdef LINUX_APP
		Tv_PtrRd	=	(void*)mmap(NULL, E_GrFsNvtMitDaySize * 2, PROT_READ|PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,
			-1, 0);

		if(0xFFFFFFFF == (__u32)Tv_PtrRd)
		{
			DbgMsgPrint("[HDD] cvtmit sr mmap error %d\n", errno);
			Tv_PtrRd	=	NULL;
		}
#else
		Tv_PtrRd	=	(void*)malloc(E_GrFsNvtMitDaySize * 2);
#endif

#ifdef LINUX_APP
		Tv_PtrWt	=	(void*)mmap(NULL, E_GrFsNvtMitDaySize , PROT_READ|PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,
			-1, 0);

		if(0xFFFFFFFF == (__u32)Tv_PtrWt)
		{
			DbgMsgPrint("[HDD] cvtmit wt mmap error %d\n", errno);
			Tv_PtrWt	=	NULL;
		}
#else
		Tv_PtrWt	=	(void*)malloc(E_GrFsNvtMitDaySize);
#endif

		// do convert mit table
		if((NULL != Tv_PtrDit) && (NULL != Tv_PtrRd) && (NULL != Tv_PtrWt))
		{
			// mark working
			m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.Fcc	=	0x44332211;	// mark working
			BlkInfoWriteByIdx(E_GrFsNvtBlkIdxMib);


			GrDumyZeroMem(Tv_PtrDit, E_GrFsNvtHddDayInfoBufSize);
			GrDumyZeroMem(Tv_PtrWt, E_GrFsNvtMitDaySize);

			Tv_DayIdx	=	E_GrFsNvtDayInfoOldCntOfDisk;
			while(0 < Tv_DayIdx)
			{
				Tv_DayIdx --;

				// check exist day
				if(0 == m_DayInfoTbl[Tv_DayIdx].TimeDay)
				{
					continue;
				}

				// calculate read address
				Tv_BofsRd	=	E_GrFsNvtBlkOfsMinuteTbl + (Tv_DayIdx * E_GrFsNvtMinuteItemSize * E_GrTimeMinOfDay);

				Tv_OfsSr	=	Tv_BofsRd & E_GrFsNvtHddIoOfsMask;
				Tv_RdSize	=	Tv_OfsSr + (E_GrFsNvtMinuteItemSize * E_GrTimeMinOfDay);
				Tv_RdSize	=	(Tv_RdSize + (E_GrFsNvtHddIoUnitSize - 1)) & E_GrFsNvtHddIoAdrMask;
				Tv_BofsRd	=	Tv_BofsRd & E_GrFsNvtHddIoAdrMask;

				// do read
				if(E_GrErrNone != BlkReadPage(E_GrFsNvtBlkIdxMib, Tv_BofsRd, Tv_PtrRd, Tv_RdSize))
				{
					DbgMsgPrint("Cls_GrFsNvtHdd::LcCvtMit - MIT read fail!\n");
					m_DskStat	=	E_GrDvrRecStatBad;

					{
						// do error notify
						St_GrTaskMsgDskIoErr	Tv_MsgErr;

						Tv_MsgErr.MsgHd.Cmd	=	E_GrTaskMsgHddIoError;
						Tv_MsgErr.DiskIdx		=	m_DevIdx;
						GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgErr, sizeof(Tv_MsgErr));
					}

					break;
				}
				
				// copy contents
				Tv_PtrSr	=	(void*)((Def_GrCalPtr)Tv_PtrRd + (Def_GrCalPtr)Tv_OfsSr);
				GrDumyCopyMem(Tv_PtrWt, Tv_PtrSr, (E_GrFsNvtMinuteItemSize * E_GrTimeMinOfDay));

				// write 
				Tv_BofsWt	=	E_GrFsNvtBlkOfsMinuteTbl + ((Tv_DayIdx + 1) * E_GrFsNvtMitDaySize);
				if(E_GrErrNone != BlkWritePage(E_GrFsNvtBlkIdxMib, Tv_BofsWt, Tv_PtrWt, E_GrFsNvtMitDaySize))
				{
					DbgMsgPrint("Cls_GrFsNvtHdd::LcCvtMit - MIT write fail!\n");
					m_DskStat	=	E_GrDvrRecStatBad;

					{
						// do error notify
						St_GrTaskMsgDskIoErr	Tv_MsgErr;

						Tv_MsgErr.MsgHd.Cmd	=	E_GrTaskMsgHddIoError;
						Tv_MsgErr.DiskIdx		=	m_DevIdx;
						GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgErr, sizeof(Tv_MsgErr));
					}

					break;
				}

				// update 
				Tv_PtrDit[Tv_DayIdx + 1].TimeDay	=	m_DayInfoTbl[Tv_DayIdx].TimeDay;

			}	//	while(0 < Tv_DayIdx)

			// udpate DIT
			GrDumyCopyMem(m_DayInfoTbl, Tv_PtrDit, E_GrFsNvtHddDayInfoBufSize);
			if(E_GrErrNone != BlkWritePage(E_GrFsNvtBlkIdxMib, E_GrFsNvtBlkOfsDayTbl, m_DayInfoTbl, E_GrFsNvtHddDayInfoBufSize))
			{
				DbgMsgPrint("Cls_GrFsNvtHdd::LcCvtMit - DIT write fail!\n");
				m_DskStat	=	E_GrDvrRecStatBad;

				{
					// do error notify
					St_GrTaskMsgDskIoErr	Tv_MsgErr;

					Tv_MsgErr.MsgHd.Cmd	=	E_GrTaskMsgHddIoError;
					Tv_MsgErr.DiskIdx		=	m_DevIdx;
					GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgErr, sizeof(Tv_MsgErr));
				}
			}

			// success patch version
			if(E_GrDvrRecStatBad != m_DskStat)
			{
				St_GrTaskMsgDskFsUpgd	Tv_MsgFsUp;

				Tv_MsgFsUp.OldVer		=	m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.FsVer;	// get old version

				m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.Fcc		=	E_GrFsNvtFcc;
				m_BlkInfoBuf[E_GrFsNvtBlkIdxMib].Mib.FsVer	=	E_GrFsNvtVer;
				BlkInfoWriteByIdx(E_GrFsNvtBlkIdxMib);

				Tv_MsgFsUp.MsgHd.Cmd	=	E_GrTaskMsgDiskFsUpgd;
				Tv_MsgFsUp.Type				=	0;
				Tv_MsgFsUp.Index			=	m_DevIdx;
				Tv_MsgFsUp.NewVer			=	E_GrFsNvtVer;

				GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgFsUp, sizeof(Tv_MsgFsUp));
			}

		}
		else
		{
			DbgMsgPrint("Cls_GrFsNvtHdd::LcCvtMit - not enough memory!\n");
			m_DskStat		=	E_GrDvrRecStatBad;

			{
				// do error notify
				St_GrTaskMsgDskIoErr	Tv_MsgErr;

				Tv_MsgErr.MsgHd.Cmd	=	E_GrTaskMsgHddIoError;
				Tv_MsgErr.DiskIdx		=	m_DevIdx;
				GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgErr, sizeof(Tv_MsgErr));
			}

		}
		
		if(NULL != Tv_PtrDit)
		{
			free(Tv_PtrDit);
		}

		if(NULL != Tv_PtrRd)
		{
#ifdef LINUX_APP
			munmap(Tv_PtrRd,E_GrFsNvtMitDaySize * 2);
#else
			free(Tv_PtrRd);
#endif
		}

		if(NULL != Tv_PtrWt)
		{
#ifdef LINUX_APP
			munmap(Tv_PtrWt, E_GrFsNvtMitDaySize);
#else
			free(Tv_PtrWt);
#endif
		}

}
//--------------------------------------------------------------------
