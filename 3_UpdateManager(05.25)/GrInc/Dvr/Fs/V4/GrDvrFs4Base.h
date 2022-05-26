/*
	GAUSS DVR file system V4 base

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs4Base
#define	_EPC_GrDvrFs4Base

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <Dvr/GrDvrBase.h>

#ifdef LINUX_APP
#include <sys/uio.h>
#else
#include <EmuLnx\GrElaBase.h>
#endif
	
//====================================================================
//constance

#define E_GrDvrFs4MaxDriveCnt					8

#define	E_GrDvrFs4MaxChCnt						32

#define E_GrDvrFs4HndlOvwtIdPos				48					/* overwrite id bit position */
#define E_GrDvrFs4HndlGrpIdxMask			0x07LL			/* disk idx mask */
#define E_GrDvrFs4HndlRdaPosMask			0x00000FFFFFFFFFE0LL					/* record data address position mask */

#define	E_GrDvrFs4OvwtIdMask					0x0000FFFF

#define E_GrDvrFs4GrpIntNone					0xFF				/* not group appertain */

#define	E_GrDvrFs4RecReqFrmCnt				1024

#define E_GrDvrFs4SeqIdNone						0
#define	E_GrDvrFs4SeqIdMin						1
#define	E_GrDvrFs4SeqIdMax						0xFFFFFFFE

#define E_GrDvrFs4RecDskIdxBad				0xFF

#define	E_GrDvrFs4BlockBitMaxCnt			4096
#define	E_GrDvrFs4IovTblCnt						UIO_MAXIOV

#define	E_GrDvrFs4BitOfsTblTimeStep		16
#define	E_GrDvrFs4BitOfsTblTimeShft		4
#define	E_GrDvrFs4BitOfsTblCnt				4
#define	E_GrDvrFs4BitOfsTblUseMark		0x1F	/* useable mark bit */

#define	E_GrDvrFs4GrpBitCacheCnt			16

// return code
#define	E_GrDvrFs4RsltOk							0		/* no problem */
#define	E_GrDvrFs4RsltNotEnoughSpace	1
#define	E_GrDvrFs4RsltBlockBad				2
#define	E_GrDvrFs4RsltParamBad				3		/* bad parametter */
#define	E_GrDvrFs4RsltWorkBufErr			4		/* work buffer error */
#define	E_GrDvrFs4RsltDiskBad					5		/* disk bad */
#define	E_GrDvrFs4RsltEndOfPos				6		/* end of position */


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs4Bit		/* 192 byte */
{
	BOOL8	IsChg;		// modified - runtime only
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	Time;			// sec unit
	__u32	OfsTbl[E_GrDvrFs4BitOfsTblCnt];		// 16 sec unit offset in block

	__u32	_rsvd3;
	__u32	_rsvd4;

	__u8	RatbTbl[E_GrDvrFs4MaxChCnt];
	__u32	SizeTbl[E_GrDvrFs4MaxChCnt];
}	*Ptr_GrDvrFs4Bit;

typedef struct St_GrDvrFs4RecFrmInfo
{
	St_GrDvrFrmUnknown	FrmHd;	// frame header
	void*	PtrStrm;		// buffer pointer
	__u32	RecSize;		// record size
}	*Ptr_GrDvrFs4RecFrmInfo;

typedef struct St_GrDvrFs4RecReqInfo
{
	St_GrDvrFs4RecFrmInfo	FrmTbl[E_GrDvrFs4RecReqFrmCnt];
	__u32	FrmCnt;
	__u32	PreMap;				// pre frame map
}	*Ptr_GrDvrFs4RecReqInfo;

typedef struct St_GrDvrFs4GrpBitCchItem
{
	BOOL8	IsUse;
	__u8	DskIdx;
	__u32	BlkId;
	__u32	LifeCnt;
	St_GrDvrFs4Bit	BitBuf[E_GrDvrFs4BlockBitMaxCnt];
}	*Ptr_GrDvrFs4GrpBitCchItem;

typedef struct St_GrDvrFs4GrpWorkEnv
{
	struct iovec	IovBuf[E_GrDvrFs4IovTblCnt];
	__u32	IovCnt;

	St_GrDvrFs4GrpBitCchItem	BitCch[E_GrDvrFs4GrpBitCacheCnt];
}	*Ptr_GrDvrFs4GrpWorkEnv;

typedef struct St_GrDvrFs4SeekBuf
{
	void*	Ptr;
	__u32	Readed;
	__u32	Prgs;
}	*Ptr_GrDvrFs4SeekBuf;

typedef	struct St_GrDvrFs4SeekCtrl
{
	St_GrDvrFs4SeekBuf	BufTbl[2];
	__u64	Hndl;					// disk handle
	__u32	Size;
	BOOL8	IsDblBuf;
	__u8	BufIdx;
}	*Ptr_GrDvrFs4SeekCtrl;

typedef struct St_GrDvrFs4BkupSizeEstm
{
	__u8	DevType;				// input
	__u8	PrgsDsk;				// disk progress
	BOOL8	IsRun;					// now running
	__u8	_rsvd1;
	Def_GrTime	TimeSt;		// input
	Def_GrTime	TimeEd;		// input
	__u32	WorkUnit;				// input - work BDT unit
	__u32	ChMap;
	__u32	PrgsBdt;
	__u64	Rslt;
}	*Ptr_GrDvrFs4BkupSizeEstm;


//====================================================================
//class

	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


