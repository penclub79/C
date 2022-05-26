/*
	solo manager

	EMU_SOLO_DUAL			= 2 chip emulation
	EMU_SOLO_BUFSIZE	=	buffer size

*/

//====================================================================
//include
#include	<Hw/Solo/GrHwSolo.h>
#include	<GrDumyTool.h>

#include	<stdlib.h>

#ifdef LINUX_APP

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/ioctl.h>
#include	<sys/time.h>
#include	<time.h>
#include	<pthread.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<sys/mman.h>

#else

#include <EmuLnx/GrElaBase.h>

#endif

//====================================================================
//local const

#define E_GrHwSoloDrvName			"/dev/gs-solo"

//====================================================================
//local type

//====================================================================
//global var

//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrHwSolo::Cls_GrHwSolo( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
	// local -------------------
		__s32		Tv_Rslt;
		__s32		Tv_WkIdx;
	// code --------------------
		//init
		m_TaskSvr			=	A_TaskSvr;
		m_HndlDrv			=	-1;
		m_PtrMmap			=	NULL;
		GrDumyZeroMem( &m_SysInfo, sizeof(m_SysInfo) );
		GrDumyZeroMem( m_DevTbl, sizeof(m_DevTbl) );

		//open
		m_HndlDrv	=	open( E_GrHwSoloDrvName, O_RDWR|O_NDELAY );
		if ( 0 <= m_HndlDrv )
		{
			//get information
			Tv_Rslt	=	ioctl( m_HndlDrv, E_GrDrvSoloIoctlGetSysInfo, &m_SysInfo );
			if ( 0 <= Tv_Rslt )
			{
				//mmap
				m_PtrMmap	=	mmap( NULL, m_SysInfo.MemSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_HndlDrv, 0 );
				if ( NULL != m_PtrMmap )
				{
					//setup control pointer
					m_PtrMmap	=	(void*)m_PtrMmap;
					//check device count
					if ( 0 != m_SysInfo.ChipCnt )
					{
						//device create
						for ( Tv_WkIdx=0;Tv_WkIdx < (__s32)m_SysInfo.ChipCnt;Tv_WkIdx++ )
						{
							m_DevTbl[Tv_WkIdx]	=	(Cls_GrDevSolo*)new Cls_GrDevSolo( m_HndlDrv, Tv_WkIdx, m_SysInfo.PhysMem, m_PtrMmap, m_SysInfo.PllMhz );
						}

					}
					
				}
			}
		}
		
}
//--------------------------------------------------------------------
Cls_GrHwSolo::~Cls_GrHwSolo()
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		//release device
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrHwSoloMaxDevCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_DevTbl[Tv_WkIdx] )
			{
				delete	m_DevTbl[Tv_WkIdx];
				m_DevTbl[Tv_WkIdx]	=	NULL;
			}
		}

		if ( 0 <= m_HndlDrv )
		{
			if ( NULL != m_PtrMmap )
			{
				munmap( m_PtrMmap, m_SysInfo.MemSize );
			}
			close( m_HndlDrv );	
		}
}
//--------------------------------------------------------------------
__u32		Cls_GrHwSolo::DrvBufSizeGet( void )
{
		return	m_SysInfo.MemSize;
}
//--------------------------------------------------------------------
Cls_GrDevSolo*	Cls_GrHwSolo::DeviceGet( __u8 A_Idx )
{
	// local -------------------
		Cls_GrDevSolo*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( E_GrHwSoloMaxDevCnt > A_Idx )
		{
			Tv_Result	=	m_DevTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrHwSolo::DeviceCount( void )
{
		return	m_SysInfo.ChipCnt;
}
//--------------------------------------------------------------------
