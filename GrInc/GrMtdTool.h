/*
	DAMGI MTD tool


*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrMtdTool
#define	_EPC_GrMtdTool

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#ifdef	LINUX_APP

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/ioctl.h>
#include	<sys/time.h>
#include	<time.h>
#include	<pthread.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<sys/mman.h>

#define _LARGEF64_SOURCE

#include <mtd/mtd-user.h>
#include <stdio.h>

#else

#include <GrFileCtrl.h>

#include <EmuLnx/GrElaBase.h>

#endif

//====================================================================
//const

#define E_GrMtdNandRetryCnt			4

//====================================================================
//type

typedef struct St_GrMtdNandCtrl
{
#ifdef LINUX_APP
	__s32	Hndl;
#else
	Cls_GrFileCtrl*	Fc;
#endif
	struct  mtd_info_user	MemInfo;
	//struct	mtd_oob_buf	Oob;
	__u32	BlkOfs;		// write block offset
	__u32	InOfs;		// write offset
	//__u32	Prgs;
	__u32	ReadOfs;
	__u32	ReadBlk;
	//BOOL8	IsOob;
}	*Ptr_GrMtdNandCtrl;

//====================================================================
//class


//====================================================================
//function

BOOL8	GrMtdNandOpen( Ptr_GrMtdNandCtrl A_PtrCtrl, __u8 A_MtdIdx );
BOOL8	GrMtdNandClose( Ptr_GrMtdNandCtrl A_PtrCtrl );
__u32	GrMtdNandUnitSizeGet( Ptr_GrMtdNandCtrl A_PtrCtrl );
BOOL8	GrMtdNandEraseAll( Ptr_GrMtdNandCtrl A_PtrCtrl );
BOOL8	GrMtdNandErase( Ptr_GrMtdNandCtrl A_PtrCtrl, __u32 A_Size );
BOOL8	GrMtdNandWrite( Ptr_GrMtdNandCtrl A_PtrCtrl, void* A_PtrData );
__u32	GrMtdNandTotalSizeGet( Ptr_GrMtdNandCtrl A_PtrCtrl );
BOOL8	GrMtdNandSeqRead( Ptr_GrMtdNandCtrl A_PtrCtrl, void* A_PtrBuf, __u32 A_Size );
BOOL8	GrMtdNandReadPosReset( Ptr_GrMtdNandCtrl A_PtrCtrl );

BOOL8	GrMtdReadFromNand(__u8 A_MtdIdx, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf);
BOOL8	GrMtdWriteToNand(__u8 A_MtdIdx, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf);

//====================================================================
//out Environment conditional
#endif
