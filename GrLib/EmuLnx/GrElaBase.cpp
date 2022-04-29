/*
	emulation LINUX application API base

*/

//====================================================================
//include

#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrStrTool.h>
#include	<GrTimeTool.h>

#include <EmuLnx/GrElaBase.h>
#include <GrDumyTool.h>


//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//global var

extern	St_GrEmuMdlDrv	V_GrEmuDrvTbl[E_GrEmuMdlMaxDrvCnt];

//====================================================================
//functions
//--------------------------------------------------------------------
int open( char* A_StrFn, int flags, int A_Atb )
{
	// local -------------------
		int		Tv_Result;
		Def_StrTag	Tv_StrName;
		int		Tv_WkIdx;
		inode	Tv_Inode;
		file	Tv_File;
	// code --------------------
		Tv_Result	=	-1;

		if ( GrStrFnGetFileName( A_StrFn, Tv_StrName ) )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrEmuMdlMaxDrvCnt;Tv_WkIdx++ )
			{
				if ( V_GrEmuDrvTbl[Tv_WkIdx].IsUse )
				{
					if ( 0 == GrStrCmp( V_GrEmuDrvTbl[Tv_WkIdx].StrName , Tv_StrName, TRUE ) )
					{
						//do open function
						Tv_Inode.Node		=	Tv_WkIdx;
						Tv_File.Handle	=	Tv_WkIdx;
						V_GrEmuDrvTbl[Tv_WkIdx].PtrFo->open( &Tv_Inode, &Tv_File );
						// found
						Tv_Result	=	Tv_WkIdx;
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int ioctl(int A_Hndl, int A_Cmd, void* A_PtrPara )
{
	// local -------------------
		int		Tv_Result;
		inode	Tv_Inode;
		file	Tv_File;
	// code --------------------
		//init
		Tv_Result	=	-1;

		if ( (0 <= A_Hndl) && (E_GrEmuMdlMaxDrvCnt > A_Hndl) )
		{
			if ( V_GrEmuDrvTbl[A_Hndl].IsUse )
			{
				if ( NULL != V_GrEmuDrvTbl[A_Hndl].PtrFo->ioctl )
				{
					Tv_Inode.Node		=	A_Hndl;
					Tv_File.Handle	=	A_Hndl;
					Tv_Result	=	V_GrEmuDrvTbl[A_Hndl].PtrFo->ioctl( &Tv_Inode, &Tv_File, A_Cmd, (DWORD)A_PtrPara );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	mmap( void* A_PtrStart, size_t A_Size, int A_Prot, int A_Flag, int A_Hndl, off_t A_Offset )
{
	// local -------------------
		void*		Tv_Result;
		file		Tv_File;
		vm_area_struct	Tv_VmArea;
	// code --------------------
		Tv_Result	=	NULL;

		if ( V_GrEmuDrvTbl[A_Hndl].IsUse )
		{
			if ( NULL != V_GrEmuDrvTbl[A_Hndl].PtrFo->mmap )
			{
				Tv_File.Handle	=	A_Hndl;
				Tv_VmArea.vm_pgoff	=	A_Offset;
				Tv_VmArea.vm_start	=	0;
				Tv_VmArea.vm_end		=	A_Size;
				V_GrEmuDrvTbl[A_Hndl].PtrFo->mmap( &Tv_File, &Tv_VmArea );
				Tv_Result	=	Tv_VmArea.MapAdr;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int	munmap(void* A_PtrMap, size_t A_Size )
{
		return	0;
}
//--------------------------------------------------------------------
int close( int A_Hndl )
{
	// local -------------------
		inode	Tv_Inode;
		file	Tv_File;
	// code --------------------
		//check exist
		if ( V_GrEmuDrvTbl[A_Hndl].IsUse )
		{
			Tv_Inode.Node		=	A_Hndl;
			Tv_File.Handle	=	A_Hndl;
			V_GrEmuDrvTbl[A_Hndl].PtrFo->release( &Tv_Inode, &Tv_File );
			V_GrEmuDrvTbl[A_Hndl].IsUse	=	FALSE;
		}
		
		return	0;
}
//--------------------------------------------------------------------
int	read( int A_Hndl, void* A_PtrBuf, int A_Size )
{
		return	-1;
}
//--------------------------------------------------------------------
ssize_t write(int fd, const void *buf, size_t count)
{
		return	-1;
}
//--------------------------------------------------------------------
int	mount( char* A_StrDev, char* A_StrMnt, char* A_StrType, DWORD A_Flag, void* A_PtrPara )
{
		return	0;
}
//--------------------------------------------------------------------
int	umount( char* A_StrMnt )
{
		return	0;
}
//--------------------------------------------------------------------
int fstat(int filedes, struct stat *buf)
{
		return	0;
}
//--------------------------------------------------------------------
int fcntl(int fd, int cmd)
{
		return	0;
}
//--------------------------------------------------------------------
int fcntl(int fd, int cmd, long arg)
{
		return	0;
}
//--------------------------------------------------------------------
int fcntl(int fd, int cmd, struct flock *lock)
{
		return	0;
}
//--------------------------------------------------------------------
long lseek(int handle, long offset, int fromwhere)
{
		return	0;
}
//--------------------------------------------------------------------
long sysconf(int name)
{
		return	0;
}
//--------------------------------------------------------------------
int gettimeofday( struct timeval* A_PtrTv, struct timezone* A_PtrTz )
{
		A_PtrTv->tv_sec	=	GrTimeSysTimeGet( (__u32*)&A_PtrTv->tv_usec );
		A_PtrTv->tv_sec	=	GrTimeCvtSysTimeToGtod( A_PtrTv->tv_sec );
		A_PtrTv->tv_usec	=	A_PtrTv->tv_usec << 10;
		return	0;
}
//--------------------------------------------------------------------
int isatty( int handle )
{
		return	0;
}
//--------------------------------------------------------------------
int stat(const char *file_name, struct stat *buf)
{
		return	0;
}
//--------------------------------------------------------------------
pid_t fork(void)
{
		return	0;
}
//--------------------------------------------------------------------
int kill(pid_t pid, int sig)
{
		return	0;
}
//--------------------------------------------------------------------
pid_t wait(int *status)
{
		return	0;
}
//--------------------------------------------------------------------
void usleep(unsigned long used)
{
}
//--------------------------------------------------------------------
unsigned int sleep(unsigned int seconds)
{
		return	0;
}
//--------------------------------------------------------------------
int seteuid(uid_t euid)
{
		return	0;
}
//--------------------------------------------------------------------
int setreuid( uid_t ruid, uid_t euid )
{
		return	0;
}
//--------------------------------------------------------------------
uid_t getuid(void)
{
		return	0;
}
//--------------------------------------------------------------------
uid_t geteuid(void)
{
		return	0;
}
//--------------------------------------------------------------------
int	getpagesize(void)
{
		return	0x1000;
}
//--------------------------------------------------------------------
void*	valloc( int A_Size )
{
		return	malloc( A_Size );
}
//--------------------------------------------------------------------
int		major( int A_val )
{
		return	0;
}
//--------------------------------------------------------------------
int		minor( int A_Val )
{
		return	0;
}
//--------------------------------------------------------------------
sighandler_t signal(int signum, sighandler_t handler)
{
		return	NULL;
}
//--------------------------------------------------------------------
int pause(void)
{
		return	0;
}
//--------------------------------------------------------------------
int getpriority(int which, int who)
{
		return	0;
}
//--------------------------------------------------------------------
int setpriority(int which, int who, int prio)
{
		return	0;
}
//--------------------------------------------------------------------
/*
pid_t getpid(void)
{
		return	0;
}
*/
//--------------------------------------------------------------------
int sched_get_priority_max(int policy)
{
		return	0;
}
//--------------------------------------------------------------------
int sched_get_priority_min(int policy)
{
		return	0;
}
//--------------------------------------------------------------------
int sched_setscheduler(pid_t pid, int policy,
											 const struct sched_param *param)
{
		return	0;
}
//--------------------------------------------------------------------
int sched_getscheduler(pid_t pid)
{
		return	0;
}
//--------------------------------------------------------------------
int  snd_pcm_open (snd_pcm_t **pcm, const char *name, snd_pcm_stream_t stream, int mode)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;
		*pcm	=	(snd_pcm_t *)malloc( sizeof(snd_pcm_t) );
		if ( NULL != *pcm )
		{
			Tv_Result	=	0;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_malloc  ( snd_pcm_hw_params_t **  ptr )
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;
		*ptr	=	(snd_pcm_hw_params_t*)malloc( sizeof(snd_pcm_hw_params_t) );
		if ( NULL != *ptr )
		{
			Tv_Result	=	0;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
int snd_pcm_sw_params_malloc  ( snd_pcm_sw_params_t **  ptr )
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;
		*ptr	=	(snd_pcm_sw_params_t*)malloc( sizeof(snd_pcm_hw_params_t) );
		if ( NULL != *ptr )
		{
			Tv_Result	=	0;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_any  ( snd_pcm_t *  pcm, snd_pcm_hw_params_t* params )
{
		params->flags	=	1234;
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_set_access  ( snd_pcm_t* pcm, snd_pcm_hw_params_t* params, snd_pcm_access_t access )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_set_format  ( snd_pcm_t * pcm, snd_pcm_hw_params_t* params,	snd_pcm_format_t format )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_set_channels  ( snd_pcm_t * pcm, snd_pcm_hw_params_t * params, unsigned int  val )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_set_rate_near  ( snd_pcm_t* pcm, snd_pcm_hw_params_t* params, unsigned int* val, int* dir	)
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_get_buffer_time_max  ( const snd_pcm_hw_params_t* params, unsigned int* val, int* dir	)
{
		*val	=	300000;
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_set_period_time_near  ( snd_pcm_t* pcm,	snd_pcm_hw_params_t* params, unsigned int* val,	int* dir )
{
		*val	=	75000;
		return	0;				
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_set_period_size_near  ( snd_pcm_t* pcm, snd_pcm_hw_params_t* params, snd_pcm_uframes_t* val, int* dir )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_get_buffer_size  (const snd_pcm_hw_params_t* params, snd_pcm_uframes_t* val )
{
		*val			=	0x4000;
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_set_buffer_time_near  ( snd_pcm_t* pcm, snd_pcm_hw_params_t* params,	unsigned int* val, int* dir )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_set_buffer_size_near  ( snd_pcm_t* pcm, snd_pcm_hw_params_t* params, snd_pcm_uframes_t* val )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params ( snd_pcm_t* pcm, snd_pcm_hw_params_t* params )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_hw_params_get_period_size  ( const snd_pcm_hw_params_t* params, snd_pcm_uframes_t* val,	int* dir )
{
		*val	=	0x1000;
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_sw_params_current  ( snd_pcm_t* pcm, snd_pcm_sw_params_t *  params )
{
		GrDumyZeroMem( params, sizeof(snd_pcm_sw_params_t) );
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_sw_params_set_avail_min  ( snd_pcm_t *  pcm,	snd_pcm_sw_params_t *  params, snd_pcm_uframes_t  val	)
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_sw_params_set_start_threshold  ( snd_pcm_t *  pcm, snd_pcm_sw_params_t *  params, snd_pcm_uframes_t  val )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_sw_params_set_stop_threshold  ( snd_pcm_t *  pcm, snd_pcm_sw_params_t *  params, snd_pcm_uframes_t  val )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_sw_params  ( snd_pcm_t *  pcm, snd_pcm_sw_params_t *  params  )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_format_physical_width (	snd_pcm_format_t format )
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;

		switch ( format )
		{
			case SND_PCM_FORMAT_S8:
				Tv_Result	=	8;
				break;
			case SND_PCM_FORMAT_S16_LE:
			case SND_PCM_FORMAT_U16_BE:
				Tv_Result	=	16;
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void snd_pcm_hw_params_free  ( snd_pcm_hw_params_t *  obj )
{
		free( obj );
}
//--------------------------------------------------------------------
void snd_pcm_sw_params_free  ( snd_pcm_sw_params_t *  obj )
{
		free( obj );
}
//--------------------------------------------------------------------
int snd_pcm_drain  ( snd_pcm_t *  pcm   )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_close  ( snd_pcm_t *  pcm   )
{
		free( pcm );
		return	0;
}
//--------------------------------------------------------------------
snd_pcm_sframes_t snd_pcm_readi  ( snd_pcm_t *  pcm, void *  buffer, snd_pcm_uframes_t  size 	)
{
	// local -------------------
		DWORD	Tv_WkIdx;
		__s16*	Tv_PtrTg;
		__s16		Tv_Val;
		__s16		Tv_Step;
	// code --------------------
		Tv_Val		=	(__s16)( (DWORD)buffer & 0xFFFF );
		Tv_PtrTg	=	(__s16*)buffer;
		Tv_Step		=	(__s16)( (DWORD)buffer & 0x3F );
		if ( 0 == Tv_Step )
		{
			Tv_Step	=	10;
		}
		for (Tv_WkIdx = 0;   Tv_WkIdx < size; Tv_WkIdx++)
		{
			*Tv_PtrTg	=	Tv_Val;
			Tv_PtrTg ++;
			
			if ( -20000 > Tv_Val )
			{
				if ( 0 > Tv_Step )
				{
					Tv_Step	=	-Tv_Step;
				}
			}
			else if ( 20000 < Tv_Val )
			{
				if ( 0 < Tv_Step )
				{
					Tv_Step	=	-Tv_Step;
				}
			}
			Tv_Val	=	Tv_Val + Tv_Step;
		}
		//GrDumyFillW( buffer, size, 0x4441 );
		return	size;
}
//--------------------------------------------------------------------
int snd_pcm_recover  ( snd_pcm_t *  pcm, int  err, int  silent )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_prepare  ( snd_pcm_t *  pcm   )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_nonblock  ( snd_pcm_t *  pcm,	int  nonblock )
{
		return	0;
}
//--------------------------------------------------------------------
int snd_pcm_pause ( snd_pcm_t *  pcm, int  enable )
{
		return	0;
}
//--------------------------------------------------------------------
snd_pcm_sframes_t snd_pcm_writei  ( snd_pcm_t *  pcm, const void *  buffer,	snd_pcm_uframes_t  size )
{
	// local -------------------
		__u32	Tv_Tick;
		__u32	Tv_Result;
	// code --------------------
		Tv_Tick	=	GrTimeGetTick();
		Tv_Tick	=	Tv_Tick & 0x03;
		Tv_Result	=	size;
		if ( Tv_Result >= Tv_Tick )
		{
			Tv_Result	=	Tv_Result - Tv_Tick;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
int tcflush(int fd, int queue_selector)
{
		return	0;
}
//--------------------------------------------------------------------
int tcsetattr(int fd, int optional_actions,	const struct termios *termios_p)
{
		return	0;
}
//--------------------------------------------------------------------
int tcdrain(int fd)
{
	return	0;
}
//--------------------------------------------------------------------
int		GrElaDrvAdd( char* A_StrName, Ptr_file_operations A_PtrFop )
{
	// local -------------------
		int		Tv_Result;
		int		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrEmuMdlMaxDrvCnt;Tv_WkIdx++ )
		{
			if ( !V_GrEmuDrvTbl[Tv_WkIdx].IsUse )
			{
				// found
				GrStrCopy( V_GrEmuDrvTbl[Tv_WkIdx].StrName, A_StrName );
				V_GrEmuDrvTbl[Tv_WkIdx].PtrFo	=	A_PtrFop;

				V_GrEmuDrvTbl[Tv_WkIdx].IsUse	=	TRUE;

				Tv_Result	=	Tv_WkIdx;
				break;
			}
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
int epoll_create(int size)
{
		return	1;
}
//--------------------------------------------------------------------
int epoll_create1(int flags)
{
		return	1;
}
//--------------------------------------------------------------------
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
		return	1;
}
//--------------------------------------------------------------------
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
		return	0;
}
//--------------------------------------------------------------------
int sysinfo(struct sysinfo *info)
{
		GrDumyZeroMem(info, sizeof(struct sysinfo));
		info->loads[0]		=	40000;
		info->loads[1]		=	30000;
		info->loads[2]		=	60000;
		info->totalram		=	10000000;
		info->freeram		=	5000000;

		return	0;
}
//--------------------------------------------------------------------
int shmget(key_t A_Key, int A_Size, int A_Shmflg)
{
		return 1;
}
//--------------------------------------------------------------------
void* shmat(int shmid, const void *shmaddr, int shmflg)
{
		return (void*)0x1000;
}
//--------------------------------------------------------------------

