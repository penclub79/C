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

#include <Dvr/Fs/V4/GrDvrFs4Base.h>
#include <Dvr/Fs/V4/GrDvrFs4Disk.h>
#include <Dvr/Fs/V4/GrDvrFs4Mng.h>

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


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs4Disk::Cls_GrDvrFs4Disk(__u8 A_DevIdx, void* A_FsMng)
{
	// local -------------------
		Def_StrGrFnPathShort	Tv_StrDev;
#ifndef LINUX_APP
		Def_StrGrFnPathShort	Tv_StrPath;
#endif
	// code --------------------
		//init
		m_DevIdx				=	A_DevIdx;
		m_FsMng					=	A_FsMng;

		m_GrpIntIdx			=	E_GrDvrFs4GrpIntNone;

		m_DskSize				=	0;
		m_RecTimeBegin	=	0;
		m_RecTimeEnd		=	0;

		m_DskStat				=	E_GrDvrRecStatNotexist;
		m_RecCap				=	0;
		m_RecSize				=	0;
		m_HndlLastRec		=	E_GrDvrRecHndlNone;


#ifdef LINUX_APP
		m_HndlDsk	=	-1;
#else
		m_HndlDsk	=	INVALID_HANDLE_VALUE;
#endif

		GrDumyZeroMem( &m_Mia, sizeof(m_Mia) );
		GrDumyZeroMem( &m_Rca, sizeof(m_Rca) );

		// check structural
		if ( 512 != sizeof(St_GrDvrFs4SmartData) )
		{
			DbgMsgPrint( "Cls_GrDvrFs4Disk: Smart struct size ( %d ) mismatch. \n", sizeof(St_GrDvrFs4SmartData) );
		}

		// check BCS size
		if (24 != sizeof(St_GrDvrFs4Bdt))
		{
			DbgMsgPrint("Cls_GrDvrFs4Disk: BDT struct size ( %d) mismatch.\n", sizeof(St_GrDvrFs4Bdt));
		}

		// check BIT
		if (192 != sizeof(St_GrDvrFs4Bit))
		{
			DbgMsgPrint("Cls_GrDvrFs4Disk: BIT struct size ( %d) mismatch.\n", sizeof(St_GrDvrFs4Bdt));
		}

		if ( GrDvrBiosRecDevNameGet( m_DevIdx, Tv_StrDev ) )
		{
#ifdef LINUX_APP
			m_HndlDsk	=	open( Tv_StrDev, O_RDWR  );
			if ( 0 <= m_HndlDsk )
			{
				DbgMsgPrint( "Cls_GrDvrFs4Disk: device ( %d ) opened. \n", m_DevIdx );
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
					DbgMsgPrint( "Cls_GrDvrFs4Disk: device ( %d ) get size error. \n", m_DevIdx );
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
			if (E_GrDvrFs4DiskMinDskSize <= m_DskSize)
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
Cls_GrDvrFs4Disk::~Cls_GrDvrFs4Disk()
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
BOOL8	Cls_GrDvrFs4Disk::LcSeek( __s64 A_Adr, BOOL8 A_IsBegin )
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
BOOL8	Cls_GrDvrFs4Disk::LcRead( void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFs4Disk::LcWrite( void* A_PtrBuf, __u32 A_Size )
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
void	Cls_GrDvrFs4Disk::LcFlush( void )
{
#ifdef LINUX_APP
		if ( -1 != m_HndlDsk )
		{
			fsync( m_HndlDsk );
		}
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::Format(BOOL8 A_IsWithLog)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsBkElaOvwt;
		BOOL8	Tv_IsWithLog;
		__u32	Tv_BkElaIn;
	// code --------------------
		Tv_Result			=	TRUE;

		Tv_IsWithLog	=	TRUE;
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			Tv_IsWithLog	=	A_IsWithLog;
		}

		// check disk status
		if ( E_GrDvrRecStatBad >= m_DskStat )
		{
			DbgMsgPrint("Cls_GrDvrFs4Disk::Format - disk (%d) status is bad.\n", m_DevIdx);
			Tv_Result	=	FALSE;
		}
		
		// check disk size
		if ((E_GrDvrFs4DiskMinDskSize > m_DskSize))
		{
			DbgMsgPrint("Cls_GrDvrFs4Disk::Format - disk (%d) size is small.\n", m_DevIdx);
			m_DskStat	=	E_GrDvrRecStatBad;
			Tv_Result	=	FALSE;
		}
		
		// MIA
		if ( Tv_Result )
		{
			// init status
			m_DskStat	=	E_GrDvrRecStatUnformated;
			//write MIS
			m_Mia.FsFcc			=	E_GrDvrFs4Fcc;
			m_Mia.FsVer			=	E_GrDvrFs4Ver;
			if (E_GrDvrFs4DiskMaxDskSize <= m_DskSize)
			{
				m_Mia.BlkCnt		=	E_GrDvrFs4BdtMaxCnt;
			}
			else
			{
				m_Mia.BlkCnt		=	(__u32)((m_DskSize - E_GrDvrFs4AdrRda) >> E_GrDvrFs4BlockShift);
			}
			
			if (!LcPwrite(E_GrDvrFs4AdrMia,&m_Mia,sizeof(m_Mia)))
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				DbgMsgPrint("Cls_GrDvrFs4Disk::Format - disk (%d) MIA write error.\n", m_DevIdx);
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
			m_Rca.DvrId	=	((Cls_GrDvrFs4Mng*)m_FsMng)->RtlDvrIdGet();

			// write
			if (!LcPwrite(E_GrDvrFs4AdrRca, &m_Rca, sizeof(m_Rca)))
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				DbgMsgPrint("Cls_GrDvrFs4Disk::Format - disk (%d) RCA write error.\n", m_DevIdx);
				Tv_Result	=	FALSE;
			}
		}
		
		// ELA
		if ( Tv_Result )
		{
			if ( Tv_IsWithLog )
			{
				Tv_Result	=	LcFillZero(E_GrDvrFs4AdrEla, E_GrDvrFs4ElaSize);
			}
			else
			{
				Tv_Result	=	LcElaDisable();
			}

			if (!Tv_Result)
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				DbgMsgPrint("Cls_GrDvrFs4Disk::Format - disk (%d) ELA write error.\n", m_DevIdx);
			}
		}

		// BDT
		if ( Tv_Result )
		{
			GrDumyZeroMem( m_Bdt, sizeof(m_Bdt) );
			if (!LcPwrite(E_GrDvrFs4AdrBdt, &m_Bdt, sizeof(m_Bdt)))
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				DbgMsgPrint("Cls_GrDvrFs4Disk::Format - disk (%d) BDT write error.\n", m_DevIdx);
				Tv_Result	=	FALSE;
			}
		}

		// init
		m_HndlLastRec		=	E_GrDvrRecHndlNone;
		m_RecTimeBegin	=	0;
		m_RecTimeEnd		=	0;

		if ( Tv_Result )
		{
			m_DskStat	=	E_GrDvrRecStatEmpty;
			LcStatUpdtNormal();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Disk::LcInit( void )
{
	// local -------------------
		__u64	Tv_DskSize;
	// code --------------------
		m_DskStat	=	E_GrDvrRecStatNotexist;

		// read MIA
		if (!LcPread(E_GrDvrFs4AdrMia,&m_Mia,sizeof(m_Mia)))
		{
			DbgMsgPrint("Cls_GrDvrFs4Disk::LcInit - MIA read error.\n");
			return;
		}

		// check fourcc code
		m_DskStat	=	E_GrDvrRecStatUnformated;
		if (E_GrDvrFs4Fcc != m_Mia.FsFcc)
		{
			DbgMsgPrint("Cls_GrDvrFs4Disk::LcInit - FCC not match.\n");
			return;
		}

		// check version
		if (E_GrDvrFs4Ver != m_Mia.FsVer)
		{
			DbgMsgPrint("Cls_GrDvrFs4Disk::LcInit - version mismatch.\n");
			return;
		}

		m_DskStat	=	E_GrDvrRecStatNormal;

		// check disk size
		if ((3 > m_Mia.BlkCnt) || (E_GrDvrFs4BdtMaxCnt < m_Mia.BlkCnt))
		{
			m_DskStat	=	E_GrDvrRecStatUnformated;
			DbgMsgPrint("Cls_GrDvrFs4Disk::LcInit - bad block count( %d).\n", m_Mia.BlkCnt);
			return;
		}

		// disk size check
		Tv_DskSize	=	E_GrDvrFs4AdrRda + ((__u64)m_Mia.BlkCnt * E_GrDvrFs4BlockSize);
		if (Tv_DskSize > m_DskSize)
		{
			m_DskStat	=	E_GrDvrRecStatUnformated;
			DbgMsgPrint("Cls_GrDvrFs4Disk::LcInit - block count( %d) was big.\n", m_Mia.BlkCnt);
			return;
		}

		// read RCA
		if (!LcPread(E_GrDvrFs4AdrRca, &m_Rca, sizeof(m_Rca)))
		{
			m_DskStat	=	E_GrDvrRecStatBad;
			DbgMsgPrint("Cls_GrDvrFs4Disk::LcInit - RCA read error.\n");
			return;
		}

		// check event log position
		if (E_GrDvrFs4ElaMaxCnt <= m_Rca.EvtPosIn)
		{
			DbgMsgPrint("Cls_GrDvrFs4Disk::LcInit - bad ELA index(%d), correct it.\n", m_Rca.EvtPosIn);
			m_Rca.EvtPosIn	=	0;
		}

		// read BDT
		if (!LcPread(E_GrDvrFs4AdrBdt, &m_Bdt, sizeof(m_Bdt)))
		{
			m_DskStat	=	E_GrDvrRecStatBad;
			DbgMsgPrint("Cls_GrDvrFs4Disk::LcInit - BDT read error.\n");
			return;
		}

		// check DVR ID
		if (((Cls_GrDvrFs4Mng*)m_FsMng)->RtlDvrIdGet() != m_Rca.DvrId)
		{
			DbgMsgPrint("Cls_GrDvrFs4Disk::LcInit - DVR changed.\n");
			// add event log

		}

		// other init
		LcStatUpdtNormal();
		LcTimeRngUpdt();

		
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs4Disk::RecLastHndlGet( void )
{
	// local -------------------
	// code --------------------
		return	m_HndlLastRec  | (__u64)m_GrpIntIdx;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::IsValidHndl( __u64 A_Hndl )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BlkId;
		__u32	Tv_OvwtId;
		__u64	Tv_DskAdr;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_DskAdr	=	A_Hndl & E_GrDvrFs4HndlRdaPosMask;
		// check Rda position
		if (E_GrDvrFs4AdrRda <= Tv_DskAdr)
		{
			// get block id
			Tv_BlkId		=	(__u32)((Tv_DskAdr - E_GrDvrFs4AdrRda) >> E_GrDvrFs4BlockShift);
			if (Tv_BlkId < m_Mia.BlkCnt)
			{
				Tv_OvwtId	=	(__u32)(A_Hndl >> E_GrDvrFs4HndlOvwtIdPos) & E_GrDvrFs4OvwtIdMask;
				if (m_Bdt[Tv_BlkId].OvwtId != Tv_OvwtId)
				{
					if (0 != m_Bdt[Tv_BlkId].SeqId)
					{
						if (E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkId].Stat)
						{
							Tv_Result	=	TRUE;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Disk::DevIdxGet( void )
{
		return	m_DevIdx;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs4Disk::DayMapGet(Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkId;
		Ptr_GrDvrFs4Bit	Tv_PtrBit;
		__u16	Tv_BitIdx;
		__u32	Tv_DayIdx;
	// code --------------------
		Tv_Result		=	0;
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			// find
			for (Tv_BlkId =0; Tv_BlkId < m_Mia.BlkCnt;Tv_BlkId++)
			{
				if ((E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkId].Stat) && (E_GrDvrFs4SeqIdNone != m_Bdt[Tv_BlkId].SeqId) && (0 != m_Bdt[Tv_BlkId].InSize) )
				{
					// check time range
					if ((0 != m_Bdt[Tv_BlkId].TimeSt) && (0 != m_Bdt[Tv_BlkId].TimeEd) && (0 != m_Bdt[Tv_BlkId].BitCnt))
					{
						// check time range
						if (((A_TimeSt <= m_Bdt[Tv_BlkId].TimeSt) && (A_TimeEd >= m_Bdt[Tv_BlkId].TimeEd)) || 
							((A_TimeSt >= m_Bdt[Tv_BlkId].TimeSt) && (A_TimeSt <= m_Bdt[Tv_BlkId].TimeEd)) || 
							((A_TimeEd >= m_Bdt[Tv_BlkId].TimeSt) && (A_TimeEd <= m_Bdt[Tv_BlkId].TimeEd)) )
						{
							// load BIT
							Tv_PtrBit	=	LcBitCchGet(Tv_BlkId, A_PtrWork);
							if (NULL != Tv_PtrBit)
							{
								// find day map
								for (Tv_BitIdx=0; Tv_BitIdx < m_Bdt[Tv_BlkId].BitCnt;Tv_BitIdx++)
								{
									if ((A_TimeSt <= Tv_PtrBit[Tv_BitIdx].Time) && (A_TimeEd >= Tv_PtrBit[Tv_BitIdx].Time))
									{
										// found, update map
										Tv_DayIdx	=	(Tv_PtrBit[Tv_BitIdx].Time - A_TimeSt) / E_GrTimeSecOfDay;
										Tv_Result	=	Tv_Result | (1 << Tv_DayIdx);
									}
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
BOOL8	Cls_GrDvrFs4Disk::MinMapCombine(Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, Ptr_GrDvrRecMinMap A_PtrRtBuf, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BlkId;
		Ptr_GrDvrFs4Bit	Tv_PtrBit;
		__u16	Tv_BitIdx;
		__u32	Tv_MinIdx;
		__u8	Tv_WkCh;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrDvrRecStatEmpty <= m_DskStat)
		{
			Tv_Result	=	TRUE;
			// find
			for (Tv_BlkId =0; Tv_BlkId < m_Mia.BlkCnt; Tv_BlkId++)
			{
				if ((E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkId].Stat) && (E_GrDvrFs4SeqIdNone != m_Bdt[Tv_BlkId].SeqId) && (0 != m_Bdt[Tv_BlkId].InSize))
				{
					// check time range
					if ((0 != m_Bdt[Tv_BlkId].TimeSt) && (0 != m_Bdt[Tv_BlkId].TimeEd) && (0 != m_Bdt[Tv_BlkId].BitCnt))
					{
						// check time range
						if (((A_TimeSt <= m_Bdt[Tv_BlkId].TimeSt) && (A_TimeEd >= m_Bdt[Tv_BlkId].TimeEd)) ||
							((A_TimeSt >= m_Bdt[Tv_BlkId].TimeSt) && (A_TimeSt <= m_Bdt[Tv_BlkId].TimeEd)) ||
							((A_TimeEd >= m_Bdt[Tv_BlkId].TimeSt) && (A_TimeEd <= m_Bdt[Tv_BlkId].TimeEd)))
						{
							// load BIT
							Tv_PtrBit	=	LcBitCchGet(Tv_BlkId, A_PtrWork);
							if (NULL != Tv_PtrBit)
							{
								// find day map
								for (Tv_BitIdx=0; Tv_BitIdx < m_Bdt[Tv_BlkId].BitCnt; Tv_BitIdx++)
								{
									if ((A_TimeSt <= Tv_PtrBit[Tv_BitIdx].Time) && (A_TimeEd >= Tv_PtrBit[Tv_BitIdx].Time))
									{
										// found, update map
										Tv_MinIdx	=	(Tv_PtrBit[Tv_BitIdx].Time - A_TimeSt) / E_GrTimeSecOfMin;
										for (Tv_WkCh=0; Tv_WkCh < E_GrDvrMaxChCnt;Tv_WkCh++)
										{
											A_PtrRtBuf[Tv_MinIdx].ChAtb[Tv_WkCh]	=	A_PtrRtBuf[Tv_MinIdx].ChAtb[Tv_WkCh] | Tv_PtrBit[Tv_BitIdx].RatbTbl[Tv_WkCh];
										}
									}
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
__u8	Cls_GrDvrFs4Disk::GrpIntIdxGet( void )
{
		return	m_GrpIntIdx;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Disk::GrpIntIdxSet( __u8 A_Idx )
{
		m_GrpIntIdx	=	A_Idx;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::LcFillZero(__u64 A_Adr, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Remain;
		__u32	Tv_WtSize;
		void*	Tv_PtrBuf;
	// code --------------------
		Tv_Result	=	FALSE;

		// allocate TMP buffer
		Tv_PtrBuf	=	((Cls_GrDvrFs4Mng*)m_FsMng)->RtlTmpBufAlloc(E_GrDvrFs4DiskFillTmpSize);
		if (NULL != Tv_PtrBuf)
		{
			if (LcSeek(A_Adr))
			{
				// clear temporary buffer
				Tv_WtSize	=	E_GrDvrFs4DiskFillTmpSize;
				if (E_GrDvrFs4DiskFillTmpSize > A_Size)
				{
					Tv_WtSize	=	A_Size;
				}
				GrDumyZeroMem(Tv_PtrBuf, Tv_WtSize);
				Tv_Remain	=	A_Size;
				while (0 != Tv_Remain)
				{
					// get write size
					Tv_WtSize	=	Tv_Remain;
					if (E_GrDvrFs4DiskFillTmpSize < Tv_WtSize)
					{
						Tv_WtSize	=	E_GrDvrFs4DiskFillTmpSize;
					}
					// write
					if (!LcWrite(Tv_PtrBuf, Tv_WtSize))
					{
						DbgMsgPrint("Cls_GrDvrFs4Disk::LcFillZero - Write fail.\n");
						break;
					}
					// next
					Tv_Remain	=	Tv_Remain - Tv_WtSize;
				}
				// check success
				if (0 == Tv_Remain)
				{
					Tv_Result	=	TRUE;
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrDvrFs4Disk::LcFillZero - Seek fail.\n");
			}

			// release TMP buffer
			((Cls_GrDvrFs4Mng*)m_FsMng)->RtlTmpBufFree(E_GrDvrFs4DiskFillTmpSize);
		}
		else
		{
			DbgMsgPrint("Cls_GrDvrFs4Disk::LcFillZero - TMP buffer was not allocate.\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::LcRcaUpdt( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( LcPwrite( E_GrDvrFs4AdrRca, &m_Rca, sizeof(m_Rca) ) )
		{
			Tv_Result	=	TRUE;
		}
		else
		{
			m_DskStat	=	E_GrDvrRecStatBad;
			DbgMsgPrint( "Cls_GrDvrFs4Disk::LcRcaUpdt - RCA area bad!\n" );
			GrTaskSvrMsgErrorNotify( E_GrErrDiskStatBad, m_DevIdx );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Disk::LcTimeRngUpdt( void )
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
			for ( Tv_BdtIdx=0;Tv_BdtIdx< m_Mia.BlkCnt;Tv_BdtIdx++ )
			{
				if ((0 != m_Bdt[Tv_BdtIdx].SeqId) && (E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BdtIdx].Stat))
				{
					if (0 != m_Bdt[Tv_BdtIdx].InSize)
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
							DbgMsgPrint( "Cls_GrDvrFs4Disk::LcTimeRngUpdt - disk ( %d ) block ( %d ) data exist, but begin time is not.\n", m_DevIdx, Tv_BdtIdx );
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
							DbgMsgPrint( "Cls_GrDvrFs4Disk::LcTimeRngUpdt - disk ( %d ) block ( %d ) data exist, but end time is not.\n", m_DevIdx, Tv_BdtIdx );
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
				DbgMsgPrint( "Cls_GrDvrFs4Disk::LcTimeRngUpdt - disk not empty, but begin time not exist.\n" );
			}
			
			if ( 0 != Tv_TimeEd )
			{
				m_RecTimeEnd	=	Tv_TimeEd;
			}
			else
			{
				m_RecTimeEnd	=	0;
				DbgMsgPrint( "Cls_GrDvrFs4Disk::LcTimeRngUpdt - disk not empty, but time end not exist.\n" );
			}
		}
		else
		{
			m_RecTimeBegin	=	0;
			m_RecTimeEnd	=	0;
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Disk::DiskStatGet( void )
{
		return	m_DskStat;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::LcElaDisable( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDvrLogItm	Tv_PtrLogTbl;
		__u64		Tv_DskAdr;		// disk address
		__u32		Tv_Remain;
		__u32		Tv_RwSize;		// read write size
		__u32		Tv_ItmCnt;
		__u32		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		// allocate TMP buffer
		Tv_PtrLogTbl	=	(Ptr_GrDvrLogItm)((Cls_GrDvrFs4Mng*)m_FsMng)->RtlTmpBufAlloc(E_GrDvrFs4DiskElaTmpSize);
		if (NULL != Tv_PtrLogTbl)
		{
			// init
			Tv_Result	=	TRUE;
			Tv_Remain	=	E_GrDvrFs4ElaSize;
			Tv_DskAdr	=	E_GrDvrFs4AdrEla;

			while (0 != Tv_Remain)
			{
				// calculate read write size
				Tv_RwSize	=	Tv_Remain;
				if (E_GrDvrFs4DiskElaTmpSize < Tv_RwSize)
				{
					Tv_RwSize	=	E_GrDvrFs4DiskElaTmpSize;
				}
				Tv_RwSize	=	Tv_RwSize & E_GrDvrLogAlignMask;	// align

				// read
				if (!LcPread(Tv_DskAdr, Tv_PtrLogTbl, Tv_RwSize))
				{
					Tv_Result	=	FALSE;
					break;
				}

				// disable
				Tv_ItmCnt	=	Tv_RwSize >> E_GrDvrLogShiftSize;
				for (Tv_WkIdx=0; Tv_WkIdx < Tv_ItmCnt; Tv_WkIdx++)
				{
					if (E_GrDvrLogTypeNone != Tv_PtrLogTbl[Tv_WkIdx].Type)
					{
						Tv_PtrLogTbl[Tv_WkIdx].Ch	=	Tv_PtrLogTbl[Tv_WkIdx].Ch | E_GrDvrLogChUnactive;
					}
				}

				// write
				if (!LcPwrite(Tv_DskAdr, Tv_PtrLogTbl, Tv_RwSize))
				{
					Tv_Result	=	FALSE;
					break;
				}

				// next
				Tv_Remain	=	Tv_Remain - Tv_RwSize;
				Tv_DskAdr	=	Tv_DskAdr + (__u64)Tv_RwSize;
			}

			// release TMP buffer
			((Cls_GrDvrFs4Mng*)m_FsMng)->RtlTmpBufFree(E_GrDvrFs4DiskElaTmpSize);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs4Disk::HndlFromRdaPos( __u64 A_RdaPos )
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	LcHndlEncByRdaPos(A_RdaPos);
		Tv_Result	=	Tv_Result | (__u64)m_GrpIntIdx;
		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs4Disk::BeginTimeGet( void )
{
		return	m_RecTimeBegin;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs4Disk::EndTimeGet( void )
{
		return	m_RecTimeEnd;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::RecSizeGet( __u64* A_PtrRtTotal, __u64* A_PtrRtRec )
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
void	Cls_GrDvrFs4Disk::LcStatUpdtNormal(void)
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
			for ( Tv_BdtIdx=0;Tv_BdtIdx < m_Mia.BlkCnt;Tv_BdtIdx++ )
			{
				if (E_GrDvrFs4BdtStatBad == m_Bdt[Tv_BdtIdx].Stat)
				{
					Tv_BadCnt ++;
				}
				else if (E_GrDvrFs4BdtStatFull == m_Bdt[Tv_BdtIdx].Stat)
				{
					Tv_FullCnt ++;
					Tv_Stat	=	E_GrDvrRecStatNormal;
					Tv_RecCap		=	Tv_RecCap + E_GrDvrFs4BlockSize;
					Tv_RecSize	=	Tv_RecSize + E_GrDvrFs4BlockSize;
				}
				else
				{
					if ( 0 != m_Bdt[Tv_BdtIdx].InSize )
					{
						Tv_Stat	=	E_GrDvrRecStatNormal;
						Tv_RecSize	=	Tv_RecSize + (__u64)m_Bdt[Tv_BdtIdx].InSize + E_GrDvrFs4BlockDataAdr;
					}
					Tv_RecCap		=	Tv_RecCap + E_GrDvrFs4BlockSize;
				}
			}
			// check disk bad
			if ( Tv_BadCnt == m_Mia.BlkCnt )
			{
				Tv_Stat	=	E_GrDvrRecStatBad;
			}
			else if ( (m_Mia.BlkCnt - Tv_BadCnt) == Tv_FullCnt )
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
Ptr_GrDvrFs4Bdt	Cls_GrDvrFs4Disk::BdtGetTable(void)
{
		return	m_Bdt;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs4Disk::BlockGetCount(void)
{
		return	m_Mia.BlkCnt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::BdtSave( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BdtSize;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrDvrRecStatEmpty <= m_DskStat)
		{
			if (0 != m_Mia.BlkCnt)
			{
				Tv_BdtSize	=	m_Mia.BlkCnt * sizeof(St_GrDvrFs4Bdt);
				if (LcPwrite(E_GrDvrFs4AdrBdt, m_Bdt, Tv_BdtSize))
				{
					m_DskStat	=	E_GrDvrRecStatEmpty;
					LcStatUpdtNormal();
					Tv_Result	=	TRUE;
				}
				else
				{
					m_DskStat	=	E_GrDvrRecStatBad;
					GrTaskSvrMsgErrorNotify(E_GrErrDiskStatBad, m_DevIdx);
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::BlockFindEmpty(__u32* A_PtrRtBlkId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			for ( Tv_BlkIdx=0;Tv_BlkIdx < m_Mia.BlkCnt;Tv_BlkIdx++ )
			{
				if ((E_GrDvrFs4BdtStatNormal == m_Bdt[Tv_BlkIdx].Stat) && (0 == m_Bdt[Tv_BlkIdx].SeqId))
				{
					// found
					Tv_Result			=	TRUE;
					*A_PtrRtBlkId	=	Tv_BlkIdx;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::LcBdtWriteByBlkId( __u32 A_BlkId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_Adr;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( A_BlkId < m_Mia.BlkCnt )
		{
			Tv_Adr	=	E_GrDvrFs4AdrBdt + (__u64)(A_BlkId * sizeof(St_GrDvrFs4Bdt));
			if (LcPwrite(Tv_Adr, &m_Bdt[A_BlkId], sizeof(St_GrDvrFs4Bdt)))
			{
					Tv_Result	=	TRUE;
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify(E_GrErrDiskStatBad, m_DevIdx);
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs4Disk::BlockFindLast(__u32* A_PtrRtIdx)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4SeqIdNone;

		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			for ( Tv_BlkIdx=0;Tv_BlkIdx < m_Mia.BlkCnt;Tv_BlkIdx++ )
			{
				if ((E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkIdx].Stat) && (0 != m_Bdt[Tv_BlkIdx].InSize))
				{
					if ( Tv_Result < m_Bdt[Tv_BlkIdx].SeqId )
					{
						// found
						Tv_Result		=	m_Bdt[Tv_BlkIdx].SeqId;
						*A_PtrRtIdx	=	Tv_BlkIdx;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs4Disk::BlockFindFirst(__u32* A_PtrRtIdx)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_BlkIdx;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4SeqIdNone;

		if ( E_GrDvrRecStatEmpty < m_DskStat )
		{
			for ( Tv_BlkIdx=0;Tv_BlkIdx < m_Mia.BlkCnt;Tv_BlkIdx++ )
			{
				if ((E_GrDvrFs4BdtStatBad  != m_Bdt[Tv_BlkIdx].Stat) && (0 != m_Bdt[Tv_BlkIdx].InSize) && (E_GrDvrFs4SeqIdNone != m_Bdt[Tv_BlkIdx].SeqId))
				{
					if (E_GrDvrFs4SeqIdNone == Tv_Result)
					{
						// found
						Tv_Result		=	m_Bdt[Tv_BlkIdx].SeqId;
						*A_PtrRtIdx	=	Tv_BlkIdx;
					}
					else 
					{
						if ( Tv_Result > m_Bdt[Tv_BlkIdx].SeqId )
						{
							// found
							Tv_Result		=	m_Bdt[Tv_BlkIdx].SeqId;
							*A_PtrRtIdx	=	Tv_BlkIdx;
						}
					}
					
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::BlockDelete(__u32 A_BlkId, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			if (A_BlkId < m_Mia.BlkCnt)
			{
				// check block status
				if ((E_GrDvrFs4BdtStatBad != m_Bdt[A_BlkId].Stat) && (0 != m_Bdt[A_BlkId].SeqId))
				{
#ifdef E_GrDvrFs4DiskDbgMsgView
					DbgMsgPrint("Cls_GrDvrFs4Disk::BlockDelete - disk(%d) block(%d) seqid(%d) \n", m_DevIdx, A_BlkId, m_Bdt[A_BlkId].SeqId);
#endif
					m_Bdt[A_BlkId].Stat			=	E_GrDvrFs4BdtStatNormal;
					m_Bdt[A_BlkId].SeqId		=	0;
					m_Bdt[A_BlkId].BitCnt		=	0;
					m_Bdt[A_BlkId].InSize		=	0;
					m_Bdt[A_BlkId].IsNrib		=	FALSE;
					m_Bdt[A_BlkId].TimeSt		=	0;
					m_Bdt[A_BlkId].TimeEd		=	0;
					m_Bdt[A_BlkId].OvwtId ++;		// increase overwrite ID

					// delte block cache
					LcBitCchDelByBlkId(A_BlkId, A_PtrWork);

					if (LcBdtWriteByBlkId(A_BlkId))
					{
						// update time range
						LcTimeRngUpdt();
						// update status
						LcStatUpdtNormal();

						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::LcPread(__u64 A_Adr, void* A_PtrBuf, __u32 A_Size)
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
BOOL8	Cls_GrDvrFs4Disk::LcPwrite(__u64 A_Adr, void* A_PtrBuf, __u32 A_Size)
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
BOOL8	Cls_GrDvrFs4Disk::LcIovPread(__u64 A_Adr, iovec* A_PtrIovBuf, __u32 A_IovCnt)
{
	// local -------------------
		BOOL8	Tv_Result;
#ifndef LINUX_APP
		__u32	Tv_WkIdx;
#endif
		// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
		if (0 < preadv64(m_HndlDsk, A_PtrIovBuf, (int)A_IovCnt, (__off64_t)A_Adr))
		{
			Tv_Result	=	TRUE;
		}
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
BOOL8	Cls_GrDvrFs4Disk::LcIovPwrite(__u64 A_Adr, iovec* A_PtrIovBuf, __u32 A_IovCnt)
{
	// local -------------------
		BOOL8	Tv_Result;
#ifndef LINUX_APP
		__u32	Tv_WkIdx;
#endif
	// code --------------------
		Tv_Result	=	FALSE;

#ifdef LINUX_APP
		if (0 < pwritev64(m_HndlDsk,A_PtrIovBuf,(int)A_IovCnt,(__off64_t)A_Adr))
		{
			Tv_Result	=	TRUE;
		}
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

//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::LogAdd( Ptr_GrDvrLogItm A_PtrLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_LogIdx;
		__u64	Tv_DskAdr;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecStatBad != m_DskStat )
		{
			// write item
			//A_PtrLog->IsAble	=	TRUE;		// patch able
			Tv_LogIdx	=	m_Rca.EvtPosIn;
			Tv_DskAdr	=	(__u64)(E_GrDvrFs4AdrEla + (Tv_LogIdx << E_GrDvrLogShiftSize));
			if (LcPwrite(Tv_DskAdr, A_PtrLog, sizeof(St_GrDvrLogItm)))
			{
				Tv_LogIdx ++;
				if (E_GrDvrFs4ElaMaxCnt <= Tv_LogIdx)
				{
					Tv_LogIdx	=	0;
					m_Rca.IsEvtOvwt	=	TRUE;
				}
				// update info
				m_Rca.EvtPosIn	=	Tv_LogIdx;
				// RCA update
				LcRcaUpdt();
				// success
				Tv_Result	=	TRUE;
			}
			else
			{
				m_DskStat	=	E_GrDvrRecStatBad;
				GrTaskSvrMsgErrorNotify( E_GrErrDiskStatBad, m_DevIdx );
				DbgMsgPrint( "Cls_GrDvrFs4Disk::LogAdd - bad sector appear!.\n" );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Disk::LogGetNext( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		__u64	Tv_DskAdr;
		Ptr_GrDvrLogItm	Tv_PtrLogTbl;
		__u32	Tv_Remain;
		__u32	Tv_ReadCnt;
		__u32	Tv_ReadSize;
		BOOL8	Tv_IsOk;
		__u32	Tv_WkIdx;
	// code --------------------
		// check disk status
		if ( E_GrDvrRecStatEmpty <= m_DskStat )
		{
			// allocate buffer
			Tv_PtrLogTbl	=	(Ptr_GrDvrLogItm)((Cls_GrDvrFs4Mng*)m_FsMng)->RtlTmpBufAlloc(E_GrDvrFs4DiskElaTmpSize);
			if (NULL != Tv_PtrLogTbl)
			{
				// read data
				Tv_DskAdr		=	(__u64)E_GrDvrFs4AdrEla;
				Tv_Remain		=	m_Rca.EvtPosIn;
				if (m_Rca.IsEvtOvwt)
				{
					Tv_Remain		=	E_GrDvrFs4ElaMaxCnt;
				}
				while (0 != Tv_Remain)
				{
					Tv_IsOk			=	FALSE;
					Tv_ReadCnt	=	Tv_Remain;
					if ((E_GrDvrFs4DiskElaTmpSize >> E_GrDvrLogShiftSize) < Tv_ReadCnt )
					{
						Tv_ReadCnt	=	E_GrDvrFs4DiskElaTmpSize >> E_GrDvrLogShiftSize;
					}
					Tv_ReadSize	=	Tv_ReadCnt << E_GrDvrLogShiftSize;
					if (LcPread(Tv_DskAdr, Tv_PtrLogTbl, Tv_ReadSize))
					{
						Tv_IsOk	=	TRUE;	// success
						// parsing
						for (Tv_WkIdx=0; Tv_WkIdx < Tv_ReadCnt; Tv_WkIdx++)
						{
							if (0 != Tv_PtrLogTbl[Tv_WkIdx].Time)
							{
								GrDvrLogGetFw( A_PtrCtrl, &Tv_PtrLogTbl[Tv_WkIdx] );
							}
						}
					}

					// check bad
					if (!Tv_IsOk)
					{
						// set bad disk
						m_DskStat	=	E_GrDvrRecStatBad;
						// notify
						GrTaskSvrMsgErrorNotify(E_GrErrDiskStatBad, m_DevIdx);
						break;
					}
					// next
					Tv_Remain	=	Tv_Remain - Tv_ReadCnt;
					Tv_DskAdr	=	Tv_DskAdr + (__u64)Tv_ReadSize;
				}
				// release buffer
				((Cls_GrDvrFs4Mng*)m_FsMng)->RtlTmpBufFree(E_GrDvrFs4DiskElaTmpSize);
			}
			else
			{
				DbgMsgPrint("Cls_GrDvrFs4Disk::LogGetNext - not enough temporarily buffer\n");
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Disk::LogGetPrev( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		__u64	Tv_DskAdr;
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
			// allocate buffer
			Tv_PtrLogTbl	=	(Ptr_GrDvrLogItm)((Cls_GrDvrFs4Mng*)m_FsMng)->RtlTmpBufAlloc(E_GrDvrFs4DiskElaTmpSize);
			if (NULL != Tv_PtrLogTbl)
			{
				// read data
				Tv_DskAdr		=	(__u64)E_GrDvrFs4AdrEla;
				Tv_Remain		=	m_Rca.EvtPosIn;
				if (m_Rca.IsEvtOvwt)
				{
					Tv_Remain		=	E_GrDvrFs4ElaMaxCnt;
				}
				while (0 != Tv_Remain)
				{
					Tv_IsOk			=	FALSE;
					Tv_ReadCnt	=	Tv_Remain;
					if ((E_GrDvrFs4DiskElaTmpSize >> E_GrDvrLogShiftSize) < Tv_ReadCnt)
					{
						Tv_ReadCnt	=	E_GrDvrFs4DiskElaTmpSize >> E_GrDvrLogShiftSize;
					}
					Tv_ReadSize	=	Tv_ReadCnt << E_GrDvrLogShiftSize;
					if (LcPread(Tv_DskAdr, Tv_PtrLogTbl, Tv_ReadSize))
					{
						Tv_IsOk	=	TRUE;	// success
						// parsing
						for (Tv_WkIdx=0; Tv_WkIdx < Tv_ReadCnt; Tv_WkIdx++)
						{
							if (0 != Tv_PtrLogTbl[Tv_WkIdx].Time)
							{
								GrDvrLogGetBw(A_PtrCtrl, &Tv_PtrLogTbl[Tv_WkIdx]);
							}
						}
					}

					// check bad
					if (!Tv_IsOk)
					{
						// set bad disk
						m_DskStat	=	E_GrDvrRecStatBad;
						// notify
						GrTaskSvrMsgErrorNotify(E_GrErrDiskStatBad, m_DevIdx);
						break;
					}
					// next
					Tv_Remain	=	Tv_Remain - Tv_ReadCnt;
					Tv_DskAdr	=	Tv_DskAdr + (__u64)Tv_ReadSize;
				}
				// release buffer
				((Cls_GrDvrFs4Mng*)m_FsMng)->RtlTmpBufFree(E_GrDvrFs4DiskElaTmpSize);
			}
			else
			{
				DbgMsgPrint("Cls_GrDvrFs4Disk::LogGetNext - not enough temporarily buffer\n");
			}
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::LcSmartRead( void )
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
BOOL8	Cls_GrDvrFs4Disk::LcSmartFindRawByte( __u8 A_Id, __u8* A_PtrRtVal )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_BytePos;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_BytePos	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs4DiskSmartValCnt;Tv_WkIdx++ )
		{
			if ( A_Id == m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFs4DiskSmartValIdxId] )
			{
				*A_PtrRtVal	=	m_SmartIoc.Data.Values[Tv_BytePos + E_GrDvrFs4DiskSmartValIdxRawVal];
				Tv_Result	=	TRUE;
				break;
			}
			//next
			Tv_BytePos	=	Tv_BytePos + E_GrDvrFs4DiskSmartValItemSize;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Disk::TempratureRead( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( LcSmartRead() )
		{
			LcSmartFindRawByte( E_GrDvrFs4DiskSmartIdTemp, &Tv_Result );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs4Disk::LcHndlEncByRdaPos(__u64 A_RdaPos)
{
	// local -------------------
		__u32	Tv_BlkId;
	// code --------------------
		Tv_BlkId	=	(__u32)((A_RdaPos - E_GrDvrFs4AdrRda) >> E_GrDvrFs4BlockShift);
		return	(A_RdaPos & E_GrDvrFs4HndlRdaPosMask) | ((__u64)m_Bdt[Tv_BlkId].OvwtId << E_GrDvrFs4HndlOvwtIdPos);
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs4Disk::LcHndlEncByBlkId(__u32 A_BlkId)
{
		return	((__u64)m_Bdt[A_BlkId].OvwtId << E_GrDvrFs4HndlOvwtIdPos) | (((__u64)A_BlkId) << E_GrDvrFs4BlockShift);
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Disk::BlockAddData(__u32 A_BlkId, __u32 A_SeqId, __u32 A_Prgs, Ptr_GrDvrFs4RecReqInfo A_PtrReq, __u32* A_PtrRtPrgs, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork)
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_Prgs;
		__u64	Tv_DskAdr;
		Ptr_GrDvrFs4Bit	Tv_PtrBit;
	// code --------------------
		// init
		Tv_Prgs	=	A_Prgs;
		// update para
		*A_PtrRtPrgs	=	Tv_Prgs;

		Tv_Result	=	LcBlockChkRecAble(A_BlkId, A_SeqId);
		if (E_GrDvrFs4RsltOk == Tv_Result)
		{
			// load BIT table
			Tv_Result	=	E_GrDvrFs4RsltBlockBad;
			Tv_PtrBit	=	LcBitCchGet(A_BlkId, A_PtrWork);
			if (NULL != Tv_PtrBit)
			{

				Tv_Result	=	E_GrDvrFs4RsltOk;
				// write progress
				while (Tv_Prgs < A_PtrReq->FrmCnt)
				{
					// get disk address
					Tv_DskAdr	=	E_GrDvrFs4AdrRda + ((__u64)A_BlkId * E_GrDvrFs4BlockSize) + (__u64)(m_Bdt[A_BlkId].InSize + E_GrDvrFs4BlockDataAdr);
					Tv_Result	=	LcBlockMakeRecIov(A_BlkId, A_PtrReq, Tv_Prgs, &Tv_Prgs, Tv_PtrBit, A_PtrWork);
					if (0 != A_PtrWork->IovCnt)
					{
						// write
						if (!LcIovPwrite(Tv_DskAdr, A_PtrWork->IovBuf, A_PtrWork->IovCnt))
						{
							Tv_Result	=	E_GrDvrFs4RsltBlockBad;
							// set bad block
							m_Bdt[A_BlkId].Stat	=	E_GrDvrFs4BdtStatBad;
							GrTaskSvrMsgErrorNotify(E_GrErrDiskBadSector, m_DevIdx);
							if (!LcBdtWriteByBlkId(A_BlkId))
							{
								Tv_Result	=	E_GrDvrFs4RsltDiskBad;
								break;
							}
						}
					}

					if (E_GrDvrFs4RsltOk != Tv_Result)
					{
						break;
					}

				}	//	while (Tv_Prgs < A_PtrReq->FrmCnt)

				// BIT update
				if ((E_GrDvrFs4RsltOk == Tv_Result) || (E_GrDvrFs4RsltNotEnoughSpace == Tv_Result))
				{
					if (!LcBitWriteByChg(A_BlkId, Tv_PtrBit))
					{
						Tv_Result	=	E_GrDvrFs4RsltBlockBad;
					}
				}

				// BDT update
				if ((E_GrDvrFs4RsltOk == Tv_Result) || (E_GrDvrFs4RsltNotEnoughSpace == Tv_Result))
				{
					m_Bdt[A_BlkId].SeqId	=	A_SeqId;	// upate sequence ID
					if (!LcBdtWriteByBlkId(A_BlkId))
					{
						Tv_Result	=	E_GrDvrFs4RsltDiskBad;
					}
				}
			}
		}

		if (E_GrDvrRecStatEmpty == m_DskStat)
		{
			LcStatUpdtNormal();
		}

		// update progress
		*A_PtrRtPrgs	=	Tv_Prgs;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::BlockSetStatFull(__u32 A_BlkId)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (m_Mia.BlkCnt <= A_BlkId)
		{
			// block bad
			if (E_GrDvrFs4BdtStatBad == m_Bdt[A_BlkId].Stat)
			{
				// check sequence id
				if (0 != m_Bdt[A_BlkId].SeqId)
				{
					Tv_Result	=	TRUE;
					// check already full
					if (E_GrDvrFs4BdtStatFull != m_Bdt[A_BlkId].Stat)
					{
						m_Bdt[A_BlkId].Stat	=	E_GrDvrFs4BdtStatFull;
						Tv_Result	=	LcBdtWriteByBlkId(A_BlkId);

						LcStatUpdtNormal();
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Disk::LcBlockChkRecAble(__u32 A_BlkId, __u32 A_SeqId)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4RsltParamBad;

		if (m_Mia.BlkCnt > A_BlkId)
		{
			// block bad
			Tv_Result	=	E_GrDvrFs4RsltBlockBad;
			if (E_GrDvrFs4BdtStatBad != m_Bdt[A_BlkId].Stat)
			{
				// check new block
				if ((0 == m_Bdt[A_BlkId].SeqId) || (0 == m_Bdt[A_BlkId].InSize))
				{
					m_Bdt[A_BlkId].SeqId	=	0;
					m_Bdt[A_BlkId].Stat		=	E_GrDvrFs4BdtStatNormal;
					m_Bdt[A_BlkId].InSize	=	0;
					m_Bdt[A_BlkId].TimeSt	=	0;
					m_Bdt[A_BlkId].TimeEd	=	0;
					m_Bdt[A_BlkId].IsNrib	=	FALSE;
					m_Bdt[A_BlkId].BitCnt	=	0;
					Tv_Result	=	E_GrDvrFs4RsltOk;
				}
				else
				{
					// check sequence id
					Tv_Result	=	E_GrDvrFs4RsltParamBad;
					if (A_SeqId == m_Bdt[A_BlkId].SeqId)
					{
						// block full
						Tv_Result	=	E_GrDvrFs4RsltNotEnoughSpace;
						if ((E_GrDvrFs4BdtStatFull != m_Bdt[A_BlkId].Stat) && (m_Bdt[A_BlkId].InSize < E_GrDvrFs4BlockRecAreaSize))
						{
							Tv_Result	=	E_GrDvrFs4RsltOk;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrDvrFs4Disk::LcBlockChkRecAble - disk %d block %d sequence id mismatch! (%d) -> (%d)\n", m_DevIdx, A_BlkId, A_SeqId, m_Bdt[A_BlkId].SeqId );
					}
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrDvrFs4Disk::LcBlockChkRecAble - disk %d block id %d out of range\n", m_DevIdx, A_BlkId);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::LcBlockLoadBit(__u32 A_BlkId, void* A_PtrBuf)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u64	Tv_DskAdr;
		__u32	Tv_ReadSize;
	// code --------------------
		Tv_Result		=	FALSE;

		// check block status
		if (E_GrDvrFs4BdtStatBad != m_Bdt[A_BlkId].Stat)
		{
			Tv_Result		=	TRUE;
			// check BIT count
			if (E_GrDvrFs4BlockBitMaxCnt < m_Bdt[A_BlkId].BitCnt)
			{
				DbgMsgPrint("Cls_GrDvrFs4Disk::LcBlockLoadBit - block (%d) bad bit count!\n", A_BlkId);
				m_Bdt[A_BlkId].BitCnt	=	0;	// patch clear
			}

			if ((0 != m_Bdt[A_BlkId].BitCnt) && (0 != m_Bdt[A_BlkId].SeqId))
			{
				Tv_DskAdr		=	E_GrDvrFs4AdrRda + ((__u64)A_BlkId * E_GrDvrFs4BlockSize);
				Tv_ReadSize	=	(__u32)m_Bdt[A_BlkId].BitCnt * sizeof(St_GrDvrFs4Bit);
				if (!LcPread(Tv_DskAdr, A_PtrBuf, Tv_ReadSize))
				{
					// disk error
					m_Bdt[A_BlkId].Stat		=	E_GrDvrFs4BdtStatBad;
					GrTaskSvrMsgErrorNotify(E_GrErrDiskBadSector, m_DevIdx);
					LcBdtWriteByBlkId(A_BlkId);
					Tv_Result	=	FALSE;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Disk::LcBlockMakeRecIov(__u32 A_BlkId, Ptr_GrDvrFs4RecReqInfo A_PtrReq, __u32 A_ReqPrgs, __u32* A_PtrRtPrgs, 
	Ptr_GrDvrFs4Bit A_PtrBitBuf,Ptr_GrDvrFs4GrpWorkEnv A_PtrWork)
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_Prgs;
		__u32	Tv_FreeSize;
		__u32	Tv_MaskCh;
		__u32	Tv_FrmSize;
		BOOL8	Tv_IsAble;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4RsltOk;

		Tv_Prgs		=	A_ReqPrgs;
		A_PtrWork->IovCnt	=	0;

		while (A_PtrWork->IovCnt < E_GrDvrFs4IovTblCnt)
		{
			// init
			Tv_FreeSize	=	E_GrDvrFs4BlockRecAreaSize - m_Bdt[A_BlkId].InSize;
			// check attribute
			Tv_IsAble	=	TRUE;
			if ((E_GrDvrFrmTypeVdoI <= A_PtrReq->FrmTbl[Tv_Prgs].FrmHd.Base.Type) && (E_GrDvrFrmTypeAdo >= A_PtrReq->FrmTbl[Tv_Prgs].FrmHd.Base.Type))
			{
				// update preframe
				if (0 == (E_GrDvrRecAtbRecord & A_PtrReq->FrmTbl[Tv_Prgs].FrmHd.Base.Atb))
				{
					Tv_IsAble	=	FALSE;	// can not record
					Tv_MaskCh	=	1 << A_PtrReq->FrmTbl[Tv_Prgs].FrmHd.Base.Ch;
					if (0 != (Tv_MaskCh & A_PtrReq->PreMap))
					{
						// preframe , do record
						A_PtrReq->FrmTbl[Tv_Prgs].FrmHd.Base.Atb		=	A_PtrReq->FrmTbl[Tv_Prgs].FrmHd.Base.Atb | E_GrDvrRecAtbRecord | E_GrDvrRecAtbPref;
						Tv_IsAble	=	TRUE;
					}
				}
			}

			if (Tv_IsAble)
			{
				// check size
				if (A_PtrReq->FrmTbl[Tv_Prgs].RecSize > Tv_FreeSize)
				{
					// do write
					Tv_Result	=	E_GrDvrFs4RsltNotEnoughSpace;
					break;
				}
				// make header
				A_PtrWork->IovBuf[A_PtrWork->IovCnt].iov_base	=	&A_PtrReq->FrmTbl[Tv_Prgs].FrmHd;
				A_PtrWork->IovBuf[A_PtrWork->IovCnt].iov_len	=	sizeof(St_GrDvrFrmUnknown);
				A_PtrWork->IovCnt ++;
				// make stream area
				A_PtrWork->IovBuf[A_PtrWork->IovCnt].iov_base	=	A_PtrReq->FrmTbl[Tv_Prgs].PtrStrm;
				A_PtrWork->IovBuf[A_PtrWork->IovCnt].iov_len	=	A_PtrReq->FrmTbl[Tv_Prgs].RecSize - sizeof(St_GrDvrFrmUnknown);
				A_PtrWork->IovCnt ++;
				// add BIT
				Tv_FrmSize	=	A_PtrReq->FrmTbl[Tv_Prgs].FrmHd.Base.DataSize + sizeof(St_GrDvrFrmUnknown);
				LcBlockAddRecInfo(A_PtrReq->FrmTbl[Tv_Prgs].FrmHd.Base.Ch, A_PtrReq->FrmTbl[Tv_Prgs].FrmHd.Base.Type, A_PtrReq->FrmTbl[Tv_Prgs].FrmHd.Base.Atb,
					A_PtrReq->FrmTbl[Tv_Prgs].FrmHd.Base.TimeSec, A_PtrReq->FrmTbl[Tv_Prgs].FrmHd.Base.IsSm, Tv_FrmSize, A_BlkId, m_Bdt[A_BlkId].InSize, A_PtrBitBuf);
				// update in size
				m_Bdt[A_BlkId].InSize		=	m_Bdt[A_BlkId].InSize + A_PtrReq->FrmTbl[Tv_Prgs].RecSize;
			}

			// next
			Tv_Prgs ++;
			if (Tv_Prgs >= A_PtrReq->FrmCnt)
			{
				break;
			}
		}

		// update progress
		*A_PtrRtPrgs		=	Tv_Prgs;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Disk::LcBlockAddRecInfo(__u8 A_Ch, __u8 A_Type, __u8 A_Atb, Def_GrTime A_Time, BOOL8 A_IsSm, 
	__u32 A_Size, __u32 A_BlkId, __u32 A_BlkOfs, Ptr_GrDvrFs4Bit A_PtrBitBuf)
{
	// local -------------------
		__s32	Tv_BitIdx;
		__u32	Tv_WkIdx;
		Def_GrTime	Tv_TmMin;
		__u32	Tv_TmIdx;
	// code --------------------

		// update BDT time range
		if ((E_GrDvrFrmTypeVdoI <= A_Type) && (E_GrDvrFrmTypeMoton >= A_Type))
		{
			// begin time
			if ((0 == m_Bdt[A_BlkId].TimeSt) || (m_Bdt[A_BlkId].TimeSt >  A_Time))
			{
				m_Bdt[A_BlkId].TimeSt		=	A_Time;
			}
			// end time
			if ((0 == m_Bdt[A_BlkId].TimeEd) || ((0 != m_Bdt[A_BlkId].TimeEd) && (m_Bdt[A_BlkId].TimeEd < A_Time)))
			{
				m_Bdt[A_BlkId].TimeEd		=	A_Time;
			}
		}
		// add BIT
		if ((E_GrDvrFrmTypeVdoI <= A_Type) && (E_GrDvrFrmTypeAdo >= A_Type))
		{
			// get minute base time
			Tv_TmMin	=	A_Time - (A_Time % E_GrTimeSecOfMin);
			// find exist time
			Tv_BitIdx	=	-1;
			if (0 != m_Bdt[A_BlkId].BitCnt)
			{
				for (Tv_WkIdx=0; Tv_WkIdx < (__u32)m_Bdt[A_BlkId].BitCnt; Tv_WkIdx++)
				{
					if (Tv_TmMin == A_PtrBitBuf[Tv_WkIdx].Time)
					{
						// found
						Tv_BitIdx		=	(__s32)Tv_WkIdx;
						break;
					}
				}
			}
			// new allocate
			if (0 > Tv_BitIdx)
			{
				if (E_GrDvrFs4BlockBitMaxCnt > m_Bdt[A_BlkId].BitCnt)
				{
					Tv_BitIdx	=	(__s32)m_Bdt[A_BlkId].BitCnt;
					m_Bdt[A_BlkId].BitCnt ++;
					// update value
					GrDumyZeroMem(&A_PtrBitBuf[Tv_BitIdx], sizeof(St_GrDvrFs4Bit));
					A_PtrBitBuf[Tv_BitIdx].Time			=	Tv_TmMin;
				}
				else
				{
					DbgMsgPrint("Cls_GrDvrFs4Disk::LcBlockAddRecInfo - not enough BIT slot!\n");
					GrTaskSvrMsgErrorNotify(E_GrErrDiskBitLoss,m_DevIdx,A_IsSm,Tv_TmMin);
				}
			}

			if (0 <= Tv_BitIdx)
			{
				A_PtrBitBuf[Tv_BitIdx].RatbTbl[A_Ch]	=	A_PtrBitBuf[Tv_BitIdx].RatbTbl[A_Ch] | A_Atb;
				A_PtrBitBuf[Tv_BitIdx].SizeTbl[A_Ch]	=	A_PtrBitBuf[Tv_BitIdx].SizeTbl[A_Ch] + A_Size;

				Tv_TmIdx	=	(A_Time - Tv_TmMin) >> E_GrDvrFs4BitOfsTblTimeShft;
				if (0 == A_PtrBitBuf[Tv_BitIdx].OfsTbl[Tv_TmIdx])
				{
					A_PtrBitBuf[Tv_BitIdx].OfsTbl[Tv_TmIdx]	=	A_BlkOfs | E_GrDvrFs4BitOfsTblUseMark;
				}

				A_PtrBitBuf[Tv_BitIdx].IsChg		=	TRUE;
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::LcBitWriteByChg(__u32 A_BlkIdx, Ptr_GrDvrFs4Bit A_PtrBitBuf)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_SrIdx;
		__u64	Tv_DskAdr;
	// code --------------------
		Tv_Result	=	TRUE;

		if (0 != m_Bdt[A_BlkIdx].BitCnt)
		{
			for (Tv_SrIdx=0; Tv_SrIdx < (__u32)m_Bdt[A_BlkIdx].BitCnt; Tv_SrIdx++)
			{
				// check changed
				if (A_PtrBitBuf[Tv_SrIdx].IsChg)
				{
					// remove change
					A_PtrBitBuf[Tv_SrIdx].IsChg	=	FALSE;
					// write
					Tv_DskAdr	=	E_GrDvrFs4AdrRda + ((__u64)A_BlkIdx * E_GrDvrFs4BlockSize) + (__u64)(Tv_SrIdx * sizeof(St_GrDvrFs4Bit));
					if (!LcPwrite(Tv_DskAdr, &A_PtrBitBuf[Tv_SrIdx], sizeof(St_GrDvrFs4Bit)))
					{
						// block error
						m_Bdt[A_BlkIdx].Stat	=	E_GrDvrFs4BdtStatBad;
						GrTaskSvrMsgErrorNotify(E_GrErrDiskBadSector, m_DevIdx);
						LcBdtWriteByBlkId(A_BlkIdx);
						Tv_Result	=	FALSE;
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Disk::ReadByHndlFw(__u64 A_Hndl, __u32 A_Ofs, void* A_PtrBuf, __u32 A_BufSize, __u32* A_PtrRtSize, __u32* A_PtrRtSeqId, __u64* A_PtrRtHndl)
{
	// local -------------------
		__u8	Tv_Result;
		__u64	Tv_RdaAdr;
		__u32	Tv_BlkId;
		__u16	Tv_OvwtId;
		__u32	Tv_BlkAdr;
		__u32	Tv_ReadSize;
		__u64	Tv_DskAdr;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4RsltDiskBad;
		if (E_GrDvrRecStatEmpty <= m_DskStat)
		{
			Tv_Result	=	E_GrDvrFs4RsltParamBad;

			// decode handle
			Tv_RdaAdr	=	A_Hndl & E_GrDvrFs4HndlRdaPosMask;
			Tv_OvwtId	=	(__u16)(A_Hndl >> E_GrDvrFs4HndlOvwtIdPos);
			Tv_BlkId	=	(__u32)(Tv_RdaAdr >> E_GrDvrFs4BlockShift);

			// check block index
			if (Tv_BlkId < m_Mia.BlkCnt)
			{
				if ((E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkId].Stat) && (0 != m_Bdt[Tv_BlkId].SeqId) && (m_Bdt[Tv_BlkId].OvwtId == Tv_OvwtId))
				{
#ifdef E_GrDvrFs4DiskDbgMsgView
					if (0 != (m_Bdt[Tv_BlkId].InSize & (~E_GrDvrFrmAlignMask)))
					{
						DbgMsgPrint("Cls_GrDvrFs4Disk::ReadByHndlFw - disk %d block %d data size %8X bad!\n", m_DevIdx, Tv_BlkId, m_Bdt[Tv_BlkId].InSize);
					}
#endif
					// valid handle, update sequence id
					*A_PtrRtSeqId	=	m_Bdt[Tv_BlkId].SeqId;

					// check read able address
					Tv_Result	=	E_GrDvrFs4RsltEndOfPos;
					Tv_BlkAdr	=	((__u32)Tv_RdaAdr & E_GrDvrFs4BlockOfsMask) + A_Ofs;
					if (Tv_BlkAdr < m_Bdt[Tv_BlkId].InSize)
					{
						// calculate read size
						Tv_ReadSize	=	m_Bdt[Tv_BlkId].InSize - Tv_BlkAdr;
						if (A_BufSize <= Tv_ReadSize)
						{
							Tv_ReadSize	=	A_BufSize;
						}

						// try read
						Tv_DskAdr	=	E_GrDvrFs4AdrRda + (((__u64)Tv_BlkId) << E_GrDvrFs4BlockShift) + (__u64)Tv_BlkAdr;
						if (LcPread(Tv_DskAdr, A_PtrBuf, Tv_ReadSize))
						{
							Tv_Result			=	E_GrDvrFs4RsltOk;
							*A_PtrRtSize	=	Tv_ReadSize;
							*A_PtrRtHndl	=	(Tv_DskAdr - E_GrDvrFs4AdrRda) | (((__u64)Tv_OvwtId) << E_GrDvrFs4HndlOvwtIdPos) | (__u64)m_GrpIntIdx;
#ifdef E_GrDvrFs4DiskDbgMsgView
							if (0 != (Tv_ReadSize & (~E_GrDvrFrmAlignMask)))
							{
								DbgMsgPrint("Cls_GrDvrFs4Disk::ReadByHndlFw - Buffer (%d) read size %d bad!\n", A_BufSize, Tv_ReadSize);
							}
#endif
						}
						else
						{
							GrTaskSvrMsgErrorNotify(E_GrErrDiskBadSector, m_DevIdx);
							m_Bdt[Tv_BlkId].Stat	=	E_GrDvrFs4BdtStatBad;
							LcBdtWriteByBlkId(Tv_BlkId);
							Tv_Result			=	E_GrDvrFs4RsltBlockBad;
						}
					}

				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::HndlFindSeqIdFw(__u32 A_SeqId, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BlkId;
		__u32	Tv_FndSeqId;
		__u32	Tv_FndBlkId;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrDvrRecStatEmpty <= m_DskStat )
		{
			Tv_FndSeqId	=	0;
			for (Tv_BlkId=0; Tv_BlkId < m_Mia.BlkCnt;Tv_BlkId++)
			{
				// check able block
				if ((E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkId].Stat) && (0 != m_Bdt[Tv_BlkId].SeqId) && (0 != m_Bdt[Tv_BlkId].InSize))
				{
					// compare sequence id
					if (A_SeqId ==  m_Bdt[Tv_BlkId].SeqId)
					{
						// found
						Tv_FndBlkId	=	Tv_BlkId;
						Tv_FndSeqId	=	A_SeqId;
						Tv_Result	=	TRUE;
						break;
					}
					else if (A_SeqId <  m_Bdt[Tv_BlkId].SeqId)
					{
						if (Tv_Result)
						{
							if (Tv_FndSeqId > m_Bdt[Tv_BlkId].SeqId)
							{
								Tv_FndBlkId	=	Tv_BlkId;
								Tv_FndSeqId	=	m_Bdt[Tv_BlkId].SeqId;
							}
						}
						else
						{
							// first time
							Tv_FndBlkId	=	Tv_BlkId;
							Tv_FndSeqId	=	m_Bdt[Tv_BlkId].SeqId;
							Tv_Result		=	TRUE;
						}
					}
				}
			}

			// update result
			if (Tv_Result)
			{
				*A_PtrRtHndl	=	LcHndlEncByBlkId(Tv_FndBlkId) | (__u64)m_GrpIntIdx;
				*A_PtrRtSeqId	=	Tv_FndSeqId;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Disk::LcBitCchUpdtLife(Ptr_GrDvrFs4GrpWorkEnv A_PtrWork)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0; Tv_WkIdx < E_GrDvrFs4GrpBitCacheCnt; Tv_WkIdx++)
		{
			if (A_PtrWork->BitCch[Tv_WkIdx].IsUse && (A_PtrWork->BitCch[Tv_WkIdx].LifeCnt != 0xFFFFFFFF))
			{
				A_PtrWork->BitCch[Tv_WkIdx].LifeCnt ++;
			}
		}
}
//--------------------------------------------------------------------
Ptr_GrDvrFs4Bit	Cls_GrDvrFs4Disk::LcBitCchGet(__u32 A_BlkId, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork)
{
	// local -------------------
		Ptr_GrDvrFs4Bit	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_LifeCnt;
		__s32	Tv_CchIdx;
		// code --------------------
		Tv_Result	=	NULL;
		// update life count
		LcBitCchUpdtLife(A_PtrWork);
		// find match cache
		for (Tv_WkIdx=0; Tv_WkIdx < E_GrDvrFs4GrpBitCacheCnt; Tv_WkIdx++)
		{
			if (A_PtrWork->BitCch[Tv_WkIdx].IsUse && (A_PtrWork->BitCch[Tv_WkIdx].DskIdx == m_GrpIntIdx) && (A_PtrWork->BitCch[Tv_WkIdx].BlkId == A_BlkId))
			{
				A_PtrWork->BitCch[Tv_WkIdx].LifeCnt	=	0;
				Tv_Result	=	A_PtrWork->BitCch[Tv_WkIdx].BitBuf;
				break;
			}
		}

		if (NULL == Tv_Result)
		{
			// not exist data
			Tv_CchIdx		=	-1;
			Tv_LifeCnt	=	0;

			// find empty slot
			for (Tv_WkIdx=0; Tv_WkIdx < E_GrDvrFs4GrpBitCacheCnt; Tv_WkIdx++)
			{
				if (!A_PtrWork->BitCch[Tv_WkIdx].IsUse)
				{
					Tv_CchIdx	=	(__s32)Tv_WkIdx;
					break;
				}
			}
			if (0 > Tv_CchIdx)
			{
				// find oldest 
				for (Tv_WkIdx=0; Tv_WkIdx < E_GrDvrFs4GrpBitCacheCnt; Tv_WkIdx++)
				{
					if (A_PtrWork->BitCch[Tv_WkIdx].IsUse)
					{
						if (0 > Tv_CchIdx)
						{
							Tv_CchIdx		=	(__s32)Tv_WkIdx;
							Tv_LifeCnt	=	A_PtrWork->BitCch[Tv_WkIdx].LifeCnt;
						}
						else if (Tv_LifeCnt < A_PtrWork->BitCch[Tv_WkIdx].LifeCnt)
						{
							Tv_CchIdx		=	(__s32)Tv_WkIdx;
							Tv_LifeCnt	=	A_PtrWork->BitCch[Tv_WkIdx].LifeCnt;
						}
					}
				}
			}

			// clear slot
			A_PtrWork->BitCch[Tv_CchIdx].IsUse			=	FALSE;
			A_PtrWork->BitCch[Tv_CchIdx].LifeCnt		=	0;

			// load BIT
			if (LcBlockLoadBit(A_BlkId, A_PtrWork->BitCch[Tv_CchIdx].BitBuf))
			{
				A_PtrWork->BitCch[Tv_CchIdx].DskIdx	=	m_GrpIntIdx;
				A_PtrWork->BitCch[Tv_CchIdx].BlkId		=	A_BlkId;
				A_PtrWork->BitCch[Tv_CchIdx].IsUse		=	TRUE;

				Tv_Result	=	A_PtrWork->BitCch[Tv_CchIdx].BitBuf;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Disk::LcBitCchDelByBlkId(__u32 A_BlkId, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0; Tv_WkIdx < E_GrDvrFs4GrpBitCacheCnt; Tv_WkIdx++)
		{
			if (A_PtrWork->BitCch[Tv_WkIdx].IsUse && (A_PtrWork->BitCch[Tv_WkIdx].DskIdx == m_GrpIntIdx) && (A_PtrWork->BitCch[Tv_WkIdx].BlkId == A_BlkId))
			{
				A_PtrWork->BitCch[Tv_WkIdx].IsUse		=	FALSE;
				A_PtrWork->BitCch[Tv_WkIdx].LifeCnt	=	0;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Disk::LcBitCchDelThisDisk(Ptr_GrDvrFs4GrpWorkEnv A_PtrWork)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0; Tv_WkIdx < E_GrDvrFs4GrpBitCacheCnt; Tv_WkIdx++)
		{
			if (A_PtrWork->BitCch[Tv_WkIdx].IsUse && (A_PtrWork->BitCch[Tv_WkIdx].DskIdx == m_GrpIntIdx))
			{
				A_PtrWork->BitCch[Tv_WkIdx].IsUse		=	FALSE;
				A_PtrWork->BitCch[Tv_WkIdx].LifeCnt	=	0;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::HndlFindTimeFw(Def_GrTime A_Time, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId, __u32* A_PtrRtTimeGap, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsBitFnd;
		BOOL8	Tv_IsSubFnd;
		__u8	Tv_SubIdx;
		__u8	Tv_SubBase;
		__u8	Tv_FndSubId;
		__u32	Tv_BlkId;
		__u32	Tv_FndTmGap;
		__u32	Tv_FndBitId;
		Ptr_GrDvrFs4Bit	Tv_PtrBit;
		__u32	Tv_BitIdx;

	// code --------------------
		Tv_Result	=	FALSE;

		if (0 != m_Mia.BlkCnt)
		{
			for (Tv_BlkId=0; Tv_BlkId < m_Mia.BlkCnt; Tv_BlkId++)
			{
				// check disk status
				if (E_GrDvrRecStatEmpty > m_DskStat)
				{
					break;
				}
				// check block status
				if ((E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkId].Stat) && (0 != m_Bdt[Tv_BlkId].SeqId) && (0 != m_Bdt[Tv_BlkId].BitCnt))
				{
					// check time range
					if (((A_Time >= m_Bdt[Tv_BlkId].TimeSt) && (A_Time <= m_Bdt[Tv_BlkId].TimeEd)) || (A_Time < m_Bdt[Tv_BlkId].TimeSt))
					{
						// load BIT
						Tv_PtrBit	=	LcBitCchGet(Tv_BlkId, A_PtrWork);
						if (NULL != Tv_PtrBit)
						{
							// find time min
							Tv_IsBitFnd	=	FALSE;
							for (Tv_BitIdx=0; Tv_BitIdx < m_Bdt[Tv_BlkId].BitCnt; Tv_BitIdx++)
							{
								// check in time range
								if ((A_Time >= Tv_PtrBit[Tv_BitIdx].Time) && (A_Time < (Tv_PtrBit[Tv_BitIdx].Time + E_GrTimeSecOfMin)))
								{
									// find sub time
									Tv_IsSubFnd	=	FALSE;
									Tv_SubBase	=	(__u8)((A_Time - Tv_PtrBit[Tv_BitIdx].Time) >> E_GrDvrFs4BitOfsTblTimeShft);
									for (Tv_SubIdx=0; Tv_SubIdx < E_GrDvrFs4BitOfsTblCnt; Tv_SubIdx++)
									{
										if (0 != Tv_PtrBit[Tv_BitIdx].OfsTbl[Tv_SubIdx])
										{
											if (Tv_IsSubFnd)
											{
												if (Tv_SubBase < Tv_SubIdx)
												{
													break;
												}
												Tv_FndSubId	=	Tv_SubIdx;
											}
											else
											{
												Tv_IsBitFnd	=	TRUE;
												Tv_FndBitId	=	Tv_BitIdx;
												Tv_FndSubId	=	Tv_SubIdx;
												Tv_FndTmGap	=	0;
												Tv_IsSubFnd	=	TRUE;
											}
										}
									}
									if (Tv_IsSubFnd)
									{
										break;
									}
								}	//	for (Tv_BitIdx=0; Tv_BitIdx < m_Bdt[Tv_FndBlkId].BitCnt;Tv_BitIdx++)
								else if (A_Time < Tv_PtrBit[Tv_BitIdx].Time)
								{
									// find sub time
									for (Tv_SubIdx=0; Tv_SubIdx < E_GrDvrFs4BitOfsTblCnt; Tv_SubIdx++)
									{
										if (0 != Tv_PtrBit[Tv_BitIdx].OfsTbl[Tv_SubIdx])
										{
											Tv_IsBitFnd	=	TRUE;
											Tv_FndBitId	=	Tv_BitIdx;
											Tv_FndSubId	=	Tv_SubIdx;
											Tv_FndTmGap	=	A_Time - Tv_PtrBit[Tv_BitIdx].Time;
											break;
										}
									}
								}	//	else if (A_Time >= Tv_PtrBit[Tv_BitIdx].Time)

							}	//	for (Tv_BitIdx=0; Tv_BitIdx < m_Bdt[Tv_FndBlkId].BitCnt;Tv_BitIdx++)
							// check BIT found
							if (Tv_IsBitFnd)
							{
								// update result
								*A_PtrRtHndl		=	(LcHndlEncByBlkId(Tv_BlkId) + (__u64)(E_GrDvrFrmAlignMask & Tv_PtrBit[Tv_FndBitId].OfsTbl[Tv_FndSubId])) | (__u64)m_GrpIntIdx;
								*A_PtrRtSeqId		=	m_Bdt[Tv_BlkId].SeqId;
								*A_PtrRtTimeGap	=	Tv_FndTmGap;
								Tv_Result	=	TRUE;
							}
							break;
						}	//	if (NULL != Tv_PtrBit)
					}	//	if (((A_Time >= m_Bdt[Tv_BlkId].TimeSt) && (A_Time <= m_Bdt[Tv_BlkId].TimeEd)) || (A_Time < m_Bdt[Tv_BlkId].TimeSt))
				}	//	if ((E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkId].Stat) && (0 != m_Bdt[Tv_BlkId].SeqId) && (0 != m_Bdt[Tv_BlkId].BitCnt))
			}	//	for (Tv_BlkId=0; Tv_BlkId < m_Mia.BlkCnt; Tv_BlkId++)

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::HndlFindFirst(__u64* A_PtrRtHndl, __u32* A_PtrRtSeqId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_SeqId;
		__u32	Tv_BlkId;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_SeqId	=	BlockFindFirst(&Tv_BlkId);
		if (E_GrDvrFs4SeqIdNone != Tv_SeqId)
		{
			*A_PtrRtSeqId	=	Tv_SeqId;
			*A_PtrRtHndl	=	LcHndlEncByBlkId(Tv_BlkId) | (__u64)m_GrpIntIdx;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Disk::ReadByHndlBw(__u64 A_Hndl, __u32 A_Ofs, void* A_PtrBuf, __u32 A_BufSize, __u32* A_PtrRtSize, __u32* A_PtrRtSeqId, __u64* A_PtrRtHndl)
{
	// local -------------------
		__u8	Tv_Result;
		__u64	Tv_RdaAdr;
		__u32	Tv_BlkId;
		__u16	Tv_OvwtId;
		__u32	Tv_BlkAdr;
		__u32	Tv_ReadSize;
		__u64	Tv_DskAdr;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4RsltDiskBad;
		if (E_GrDvrRecStatEmpty <= m_DskStat)
		{
			Tv_Result	=	E_GrDvrFs4RsltParamBad;

			// decode handle
			Tv_RdaAdr	=	A_Hndl & E_GrDvrFs4HndlRdaPosMask;
			Tv_OvwtId	=	(__u16)(A_Hndl >> E_GrDvrFs4HndlOvwtIdPos);
			Tv_BlkId	=	(__u32)(Tv_RdaAdr >> E_GrDvrFs4BlockShift);

			// check block index
			if (Tv_BlkId < m_Mia.BlkCnt)
			{
				if ((E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkId].Stat) && (0 != m_Bdt[Tv_BlkId].SeqId) && (m_Bdt[Tv_BlkId].OvwtId == Tv_OvwtId))
				{
#ifdef E_GrDvrFs4DiskDbgMsgView
					if (0 != (m_Bdt[Tv_BlkId].InSize & (~E_GrDvrFrmAlignMask)))
					{
						DbgMsgPrint("Cls_GrDvrFs4Disk::ReadByHndlFw - disk %d block %d data size %8X bad!\n", m_DevIdx, Tv_BlkId, m_Bdt[Tv_BlkId].InSize);
					}
#endif
					// valid handle, update sequence id
					*A_PtrRtSeqId	=	m_Bdt[Tv_BlkId].SeqId;

					// check read able address
					Tv_Result	=	E_GrDvrFs4RsltEndOfPos;
					Tv_BlkAdr	=	((__u32)Tv_RdaAdr & E_GrDvrFs4BlockOfsMask) + A_Ofs;
					if (Tv_BlkAdr > 0)
					{
						if (m_Bdt[Tv_BlkId].InSize <= Tv_BlkAdr)
						{
							Tv_BlkAdr	=	m_Bdt[Tv_BlkId].InSize;
						}
						// calculate read size
						Tv_ReadSize	=	Tv_BlkAdr;
						if (A_BufSize <= Tv_ReadSize)
						{
							Tv_ReadSize	=	A_BufSize;
						}

						// try read
						Tv_BlkAdr	=	Tv_BlkAdr - Tv_ReadSize;
						Tv_DskAdr	=	E_GrDvrFs4AdrRda + (((__u64)Tv_BlkId) << E_GrDvrFs4BlockShift) + (__u64)Tv_BlkAdr;
						if (LcPread(Tv_DskAdr, A_PtrBuf, Tv_ReadSize))
						{
							Tv_Result			=	E_GrDvrFs4RsltOk;
							*A_PtrRtSize	=	Tv_ReadSize;
							*A_PtrRtHndl	=	(Tv_DskAdr - E_GrDvrFs4AdrRda) | (((__u64)Tv_OvwtId) << E_GrDvrFs4HndlOvwtIdPos) | (__u64)m_GrpIntIdx;
#ifdef E_GrDvrFs4DiskDbgMsgView
							if (0 != (Tv_ReadSize & (~E_GrDvrFrmAlignMask)))
							{
								DbgMsgPrint("Cls_GrDvrFs4Disk::ReadByHndlBw - Buffer (%d) read size %d bad!\n", A_BufSize, Tv_ReadSize);
							}
#endif
						}
						else
						{
							GrTaskSvrMsgErrorNotify(E_GrErrDiskBadSector, m_DevIdx);
							m_Bdt[Tv_BlkId].Stat	=	E_GrDvrFs4BdtStatBad;
							LcBdtWriteByBlkId(Tv_BlkId);
							Tv_Result			=	E_GrDvrFs4RsltBlockBad;
						}
					}

				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::HndlFindSeqIdBw(__u32 A_SeqId, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_BlkId;
		__u32	Tv_FndSeqId;
		__u32	Tv_FndBlkId;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrDvrRecStatEmpty <= m_DskStat)
		{
			Tv_FndSeqId	=	0;
			for (Tv_BlkId=0; Tv_BlkId < m_Mia.BlkCnt; Tv_BlkId++)
			{
				// check able block
				if ((E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkId].Stat) && (0 != m_Bdt[Tv_BlkId].SeqId) && (0 != m_Bdt[Tv_BlkId].InSize))
				{
					// compare sequence id
					if (A_SeqId ==  m_Bdt[Tv_BlkId].SeqId)
					{
						// found
						Tv_FndBlkId	=	Tv_BlkId;
						Tv_FndSeqId	=	A_SeqId;
						Tv_Result	=	TRUE;
						break;
					}
					else if (A_SeqId >  m_Bdt[Tv_BlkId].SeqId)
					{
						if (Tv_Result)
						{
							if (Tv_FndSeqId < m_Bdt[Tv_BlkId].SeqId)
							{
								Tv_FndBlkId	=	Tv_BlkId;
								Tv_FndSeqId	=	m_Bdt[Tv_BlkId].SeqId;
							}
						}
						else
						{
							// first time
							Tv_FndBlkId	=	Tv_BlkId;
							Tv_FndSeqId	=	m_Bdt[Tv_BlkId].SeqId;
							Tv_Result		=	TRUE;
						}
					}
				}
			}

			// update result
			if (Tv_Result)
			{
				*A_PtrRtHndl	=	LcHndlEncByBlkId(Tv_FndBlkId) | (__u64)m_GrpIntIdx;
				*A_PtrRtSeqId	=	Tv_FndSeqId;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Disk::HndlFindTimeBw(Def_GrTime A_Time, __u64* A_PtrRtHndl, __u32* A_PtrRtSeqId, __u32* A_PtrRtTimeGap, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsBitFnd;
		BOOL8	Tv_IsSubFnd;
		__u8	Tv_SubIdx;
		__u8	Tv_SubBase;
		__u8	Tv_FndSubId;
		__u8	Tv_WkIdx;
		__u32	Tv_BlkId;
		__u32	Tv_FndTmGap;
		__u32	Tv_FndBitId;
		Ptr_GrDvrFs4Bit	Tv_PtrBit;
		__u32	Tv_BitIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if (0 != m_Mia.BlkCnt)
		{
			for (Tv_BlkId=0; Tv_BlkId < m_Mia.BlkCnt; Tv_BlkId++)
			{
				// check disk status
				if (E_GrDvrRecStatEmpty > m_DskStat)
				{
					break;
				}
				// check block status
				if ((E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkId].Stat) && (0 != m_Bdt[Tv_BlkId].SeqId) && (0 != m_Bdt[Tv_BlkId].BitCnt))
				{
					// check time range
					if (((A_Time >= m_Bdt[Tv_BlkId].TimeSt) && (A_Time <= m_Bdt[Tv_BlkId].TimeEd)) || (A_Time > m_Bdt[Tv_BlkId].TimeEd))
					{
						// load BIT
						Tv_PtrBit	=	LcBitCchGet(Tv_BlkId, A_PtrWork);
						if (NULL != Tv_PtrBit)
						{
							// find time min
							Tv_IsBitFnd	=	FALSE;
							for (Tv_BitIdx=0; Tv_BitIdx < m_Bdt[Tv_BlkId].BitCnt; Tv_BitIdx++)
							{
								// check in time range
								if ((A_Time >= Tv_PtrBit[Tv_BitIdx].Time) && (A_Time < (Tv_PtrBit[Tv_BitIdx].Time + E_GrTimeSecOfMin)))
								{
									// find sub time
									Tv_IsSubFnd	=	FALSE;
									Tv_SubBase	=	(__u8)((A_Time - Tv_PtrBit[Tv_BitIdx].Time) >> E_GrDvrFs4BitOfsTblTimeShft);
									Tv_SubIdx		=	E_GrDvrFs4BitOfsTblCnt - 1;
									for (Tv_WkIdx=0; Tv_WkIdx < E_GrDvrFs4BitOfsTblCnt; Tv_WkIdx++)
									{
										if (0 != Tv_PtrBit[Tv_BitIdx].OfsTbl[Tv_SubIdx])
										{
											if (Tv_IsSubFnd)
											{
												if (Tv_SubBase < Tv_SubIdx)
												{
													break;
												}
												Tv_FndSubId	=	Tv_SubIdx;
											}
											else
											{
												Tv_IsBitFnd	=	TRUE;
												Tv_FndBitId	=	Tv_BitIdx;
												Tv_FndSubId	=	Tv_SubIdx;
												Tv_FndTmGap	=	0;
												Tv_IsSubFnd	=	TRUE;
											}
										}
										Tv_SubIdx --;
									}
									if (Tv_IsSubFnd)
									{
										break;
									}
								}	//	for (Tv_BitIdx=0; Tv_BitIdx < m_Bdt[Tv_FndBlkId].BitCnt;Tv_BitIdx++)
								else if (A_Time < Tv_PtrBit[Tv_BitIdx].Time)
								{
									// find sub time
									Tv_SubIdx		=	E_GrDvrFs4BitOfsTblCnt - 1;
									for (Tv_WkIdx=0; Tv_WkIdx < E_GrDvrFs4BitOfsTblCnt; Tv_WkIdx++)
									{
										if (0 != Tv_PtrBit[Tv_BitIdx].OfsTbl[Tv_SubIdx])
										{
											Tv_IsBitFnd	=	TRUE;
											Tv_FndBitId	=	Tv_BitIdx;
											Tv_FndSubId	=	Tv_SubIdx;
											Tv_FndTmGap	=	A_Time - Tv_PtrBit[Tv_BitIdx].Time;
											break;
										}
										Tv_SubIdx --;
									}
								}	//	else if (A_Time >= Tv_PtrBit[Tv_BitIdx].Time)

							}	//	for (Tv_BitIdx=0; Tv_BitIdx < m_Bdt[Tv_FndBlkId].BitCnt;Tv_BitIdx++)
							// check BIT found
							if (Tv_IsBitFnd)
							{
								// update result
								*A_PtrRtHndl		=	(LcHndlEncByBlkId(Tv_BlkId) + (__u64)(E_GrDvrFrmAlignMask & Tv_PtrBit[Tv_FndBitId].OfsTbl[Tv_FndSubId])) | (__u64)m_GrpIntIdx;
								*A_PtrRtSeqId		=	m_Bdt[Tv_BlkId].SeqId;
								*A_PtrRtTimeGap	=	Tv_FndTmGap;
								Tv_Result	=	TRUE;
							}
							break;
						}	//	if (NULL != Tv_PtrBit)
					}	//	if (((A_Time >= m_Bdt[Tv_BlkId].TimeSt) && (A_Time <= m_Bdt[Tv_BlkId].TimeEd)) || (A_Time < m_Bdt[Tv_BlkId].TimeSt))
				}	//	if ((E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkId].Stat) && (0 != m_Bdt[Tv_BlkId].SeqId) && (0 != m_Bdt[Tv_BlkId].BitCnt))
			}	//	for (Tv_BlkId=0; Tv_BlkId < m_Mia.BlkCnt; Tv_BlkId++)

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Disk::BkupSizeEstmProc(Ptr_GrDvrFs4BkupSizeEstm A_PtrCtrl, Ptr_GrDvrFs4GrpWorkEnv A_PtrWork)
{
	// local -------------------
		__u32	Tv_BlkId;
		__u32	Tv_WorkCnt;
		Ptr_GrDvrFs4Bit	Tv_PtrBit;
		__u16	Tv_BitIdx;
		__u8	Tv_WkCh;
		__u32	Tv_MaskCh;
	// code --------------------

		if (E_GrDvrRecStatEmpty <= m_DskStat)
		{
			// init
			Tv_WorkCnt	=	0;
			Tv_BlkId		=	A_PtrCtrl->PrgsBdt;
			// find
			while (Tv_BlkId < m_Mia.BlkCnt)
			{
				if ((E_GrDvrFs4BdtStatBad != m_Bdt[Tv_BlkId].Stat) && (E_GrDvrFs4SeqIdNone != m_Bdt[Tv_BlkId].SeqId) && (0 != m_Bdt[Tv_BlkId].InSize))
				{
					// check time range
					if ((0 != m_Bdt[Tv_BlkId].TimeSt) && (0 != m_Bdt[Tv_BlkId].TimeEd) && (0 != m_Bdt[Tv_BlkId].BitCnt))
					{
						// check time range
						if (((A_PtrCtrl->TimeSt <= m_Bdt[Tv_BlkId].TimeSt) && (A_PtrCtrl->TimeEd <= m_Bdt[Tv_BlkId].TimeEd)) ||
							((A_PtrCtrl->TimeSt >= m_Bdt[Tv_BlkId].TimeSt) && (A_PtrCtrl->TimeSt <= m_Bdt[Tv_BlkId].TimeEd)) ||
							((A_PtrCtrl->TimeEd >= m_Bdt[Tv_BlkId].TimeSt) && (A_PtrCtrl->TimeEd <= m_Bdt[Tv_BlkId].TimeEd)))
						{
							// load BIT
							Tv_PtrBit	=	LcBitCchGet(Tv_BlkId, A_PtrWork);
							if (NULL != Tv_PtrBit)
							{
								// find day map
								for (Tv_BitIdx=0; Tv_BitIdx < m_Bdt[Tv_BlkId].BitCnt; Tv_BitIdx++)
								{
									if ((A_PtrCtrl->TimeSt <= Tv_PtrBit[Tv_BitIdx].Time) && (A_PtrCtrl->TimeEd >= Tv_PtrBit[Tv_BitIdx].Time))
									{
										// found, update size
										Tv_MaskCh	=	1;
										for (Tv_WkCh=0; Tv_WkCh < E_GrDvrMaxChCnt; Tv_WkCh++)
										{
											if (0 != (Tv_MaskCh & A_PtrCtrl->ChMap))
											{
												A_PtrCtrl->Rslt	=	A_PtrCtrl->Rslt + (__u64)Tv_PtrBit[Tv_BitIdx].SizeTbl[Tv_WkCh];
											}
											// next
											Tv_MaskCh	=	Tv_MaskCh << 1;
										}
									}
								}
								// update work count
								Tv_WorkCnt ++;
								if (Tv_WorkCnt >= A_PtrCtrl->WorkUnit)
								{
									break;
								}
							}
						}
					}
				}
				// next
				Tv_BlkId ++;
			}		//	while (Tv_BlkId < m_Mia.BlkCnt)

			// check finish
			if (Tv_BlkId >= m_Mia.BlkCnt)
			{
				// next disk
				A_PtrCtrl->PrgsDsk ++;
				Tv_BlkId	=	0;
			}
			// update progress
			A_PtrCtrl->PrgsBdt		=	Tv_BlkId;
		}
		else
		{
			// go next disk
			A_PtrCtrl->PrgsDsk ++;
			A_PtrCtrl->PrgsBdt		=	0;
		}

}
//--------------------------------------------------------------------
