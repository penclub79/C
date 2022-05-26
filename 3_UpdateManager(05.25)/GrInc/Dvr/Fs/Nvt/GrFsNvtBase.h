/*
	GAUSS DVR file system V5 base

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrFsNvtBase
#define	_EPC_GrFsNvtBase

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/GrDvrBase.h>
#include <GrBufTool.h>

#ifdef LINUX_APP
#include <sys/uio.h>
#else
#include <EmuLnx\GrElaBase.h>
#endif
	
//====================================================================
//constance

#define E_GrFsNvtMaxDiskCnt									8

#define	E_GrFsNvtMaxChCnt										32

#define E_GrFsNvtHndlOvwtIdPos							56					/* overwrite id bit position */
#define E_GrFsNvtHndlGrpIdxMask							0x07LL			/* group disk idx mask */
#define E_GrFsNvtHndlOfsMask								0x000000001FFFFFE0LL					/* record data address position mask */
#define	E_GrFsNvtHndlBlkIdxPos							29
#define	E_GrFsNvtBlkOfsLimit								0x1FFFFFE0

// tiem base
#define	E_GrFsNvtTimeMin										1325376000		/* 2012 , 1, 1 */

// disk
#define E_GrFsNvtFcc												Mac_GrMakeFcc('G','D','F','5')

#define E_GrFsNvtVer												5		/* sub version */

// group

#define	E_GrFsNvtRgrpMaxCnt									2			/* raid group max count */
#define	E_GrFsNvtHwBufMinMaxCnt							32
#define	E_GrFsNvtHwBufDataMaxCnt						32


// block info
#define E_GrFsNvtBlkSize										0x0000000020000000LL
#define E_GrFsNvtBlkShift										E_GrFsNvtHndlBlkIdxPos

#define	E_GrFsNvtBlkMaxCnt									0xFFFF

#define	E_GrFsNvtBlkInfoSize								32
#define	E_GrFsNvtMinuteItemSize							176

// block status
#define	E_GrFsNvtBlkStatNormal							0
#define	E_GrFsNvtBlkStatFull								1
#define	E_GrFsNvtBlkStatBad									0xFF

// block offset
#define	E_GrFsNvtBlkOfsDayTbl								0x00200000		/* block offset of day table */
#define	E_GrFsNvtBlkOfsMinuteTbl						0x00201000		/* block offset of minute table */

// block index
#define	E_GrFsNvtBlkIdxMib									0		/* master information block */

// disk id
#define	E_GrFsNvtDiskIdMaskMac							0x0000FFFFFFFFFFFFLL
//#define	E_GrFsNvtDiskIdGrpTypePos						48	/* not use */

// sequence id	
#define E_GrFsNvtSeqIdNone									0
#define	E_GrFsNvtSeqIdMin										1
#define	E_GrFsNvtSeqIdMax										0xFFFFFFFF
#define	E_GrFsNvtSeqIdTolerance							0xFFE00000

// day map
#define	E_GrFsNvtDayInfoOldCntOfDisk				768			// V4
#define	E_GrFsNvtDayTblCntOfDisk						1024		// V5

// time map
//#define	E_GrFsNvtMinuteTblSizeOfDay					(E_GrFsNvtMinuteItemSize * 60 * 24)
#define	E_GrFsNvtTimeMapSecUnit							30		/* 30 sec */
#define	E_GrFsNvtMitDaySize									0x3E000

// log pool
#define	E_GrFsNvtFrmLogPoolItemCnt					2046	/* 0x10000 size  */

//====================================================================
//global macro


//====================================================================
//global type

typedef struct St_GrFsNvtMibInfo
{
	__u32	Fcc;
	__u8	SeqLast;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u64	DiskId;

	__u32	FsVer;
	__u32	SeqOvwt;
	__u32	BlkCnt;
	__u32	_rsvd3;
}	*Ptr_GrFsNvtMibInfo;

typedef struct St_GrFsNvtDbBlkInfo
{
	__u8	BlkStat;
	__u8	OvwtId;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u32	SeqId;
	__u32	InSize;
	__u32	_rsvd2;

	St_GrTimeRng	TimeRng;
	__s32	OfsLastLog;
	__u32	_rsvd3;
}	*Ptr_GrFsNvtDbBlkInfo;

typedef struct St_GrFsNvtDayInfo
{
	__u32	TimeDay;		// day base time
}	*Ptr_GrFsNvtDayInfo;

typedef struct St_GrFsNvtBlkAdr
{
	__u8	OvwtId;
	__u8	_rsvd0;
	__u16	Idx;
	__u32	Ofs;
}	*Ptr_GrFsNvtBlkAdr;

typedef struct St_GrFsNvtMinuteInfo
{
	St_GrFsNvtBlkAdr	AdrMin;	// minute offset
	St_GrFsNvtBlkAdr	Adr30s; // 30 sec offset
	__u8	ChRatbTbl[E_GrDvrMaxChCnt];
	__u32	ChSizeTbl[E_GrDvrMaxChCnt];
}	*Ptr_GrFsNvtMinuteInfo;


typedef struct St_GrFsNvtBlkInfoBase
{
	union 
	{
		St_GrFsNvtMibInfo	Mib;
		St_GrFsNvtDbBlkInfo	Db;
	};
}	*Ptr_GrFsNvtBlkInfoBase;

typedef	struct St_GrFsNvtSeekCtrl
{
	void*	PtrBuf;				// buffer pointer
	__u32	InSize;				// in data size
	__u32	BlkSeq;				// block sequence id
	__s32	Prgs;					// progress
	__u8	CchRsv;				// cache reserve count
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u64	Hndl;					// disk handle
}	*Ptr_GrFsNvtSeekCtrl;

typedef struct St_GrFsNvtHwBuf
{
	__u32	CchMinCnt;
	void*	CchMinBuf[E_GrFsNvtHwBufMinMaxCnt];

	__u32	CchDataCnt;
	void*	CchDataBuf[E_GrFsNvtHwBufDataMaxCnt];
}	*Ptr_GrFsNvtHwBuf;

typedef struct St_GrFsNvtEnv
{
	__u64	DiskBaseId;
	//__u32	IoUnitSize;	// data buffer unit size
	//__u32	IoAdrMask;
	//__u32	IoOfsMask;

	__u32	BkupHdSize;
	__u32	DiskMaxCnt;
	__u32	Tick100ms;		// 100 ms tick
	void*	DskTbl[E_GrFsNvtMaxDiskCnt];
	Cls_GrTaskCli*	ObjMng;

	// cache buffer
	St_GrFsNvtHwBuf	HwBufs[E_GrFsNvtRgrpMaxCnt];
}	*Ptr_GrFsNvtEnv;

typedef struct St_GrFsNvtBkupSizeEstm
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
}	*Ptr_GrFsNvtBkupSizeEstm;

typedef struct St_GrFsNvtFrmHdLogPool
{
	St_GrDvrFrmHdBase	Base;
	__u32	InCnt;
	__s32	OfsPrv;
	__u32	_rsvd0;
}	*Ptr_GrFsNvtFrmHdLogPool;

typedef struct St_GrFsNvtFrmLogPool
{
	St_GrFsNvtFrmHdLogPool	Head;

	St_GrDvrLogItm	LogTbl[E_GrFsNvtFrmLogPoolItemCnt];

	__u32	_rsvd0[6];

	St_GrDvrFrmTail	Tail;

}	*Ptr_GrFsNvtFrmLogPool;


typedef struct St_GrFsNvtLogFindReq
{
	Ptr_GrDvrLogItm	PtrLog;		// result buffer
	__u8				Group;				// disk group
	__u8				Mode;					// log mode
	__u8				_rsvd1;
	__u8				_rsvd2;

	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;

	__u32				MaxCnt;
	__u32				ChMask;

	__u64				TypeMask;

}	*Ptr_GrFsNvtLogFindReq;

typedef struct St_GrFsNvtLogFindRslt
{
	BOOL8				IsDone;		// finish
	BOOL8				IsOpen;		// opened
	BOOL8				IsPart;		// part in
	__u32				InCnt;		// input count
	Def_GrErrCode	ErrCode;	// error code
}	*Ptr_GrFsNvtLogFindRslt;

typedef struct St_GrFsNvtIoLogGetRtl
{
	// common
	BOOL8	IsOpen;				// now open
	BOOL8	IsFrmUnit;		// false = all, true = frame unit
	__u8	DevSub;				// device sub type
	BOOL8	IsDone;

	BOOL8	IsWait;				// wait continue - frame mode only

	__u32	ChMask;
	__u64	TypeMask;

	Def_GrTime			TimeSt;
	Def_GrTime			TimeEd;

	__u32	MaxCnt;		// max counter
	__u32	InCnt;		// in counter
	Ptr_GrDvrLogItm	PtrRtBuf;

	__u32	DskIdx;
	__u32	BlkIdx;
	__s32	LogOfs;

}	*Ptr_GrFsNvtIoLogGetRtl;

typedef struct St_GrFsNvtSmart
{
	__u32	Poh;		// power on hour
	__u8	Temp;		// temprature
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	BadSect;	// bad sector count
	__u32	SeekErr;	//write error rate
	__u32 ReadErr;	//read error rate
	__u32 Reallocate;	//read error rate
}	*Ptr_GrFsNvtSmart;

typedef struct St_GrFsNvtDecBuf
{
	__s32 MemHndl;
	void* PtrBuf;
	__s32 BufSize;
	__s32 BufOfs;
} *Ptr_GrFsNvtDecBuf;

//====================================================================
//class

	
//====================================================================
//global function

void	GrFsNvtPrepare(Cls_GrTaskSvr* A_TskSvr, __u64 A_MacAdr, __u32 A_BkupHdSize,__u32 A_DiskMaxCnt,
	__u32 A_CchMinCnt, __u32 A_CchDataCnt);
void	GrFsNvtInit(void);
void	GrFsNvtStop(void);
void	GrFsNvtFinish(void);

void	GrFsNvtHndlDec(__u64 A_Hndl, __s32* A_PtrRtDskIdx, __u32* A_PtrRtOffset, __s32* A_PtrRtBlkIdx, __u8* A_PtrRtOvwtId);
__u64	GrFsNvtHndlEnc(__s32 A_DskIdx, __u32 A_Offset, __s32 A_BlkIdx, __u8 A_OvwtId);

BOOL8	GrFsNvtLogAdd(Ptr_GrDvrLogItm A_PtrLog);
void	GrFsNvtRecBufAdd(iovec* A_PtrIovs, __u32 A_Cnt);

void	GrFsNvtSetup(__u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir);
void	GrFsNvtAutoDelSet(__u32 A_Sec);
BOOL8	GrFsNvtDiskFormatReq(__u32 A_DiskIdx, BOOL8 A_IsPost);
__u8	GrFsNvtDiskRecInfoGet(__u32 A_DiskIdx, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize);
BOOL8	GrFsNvtDiskModelNameGet(__u32 A_DiskIdx, WCHAR* A_StrRt);
__u8	GrFsNvtGroupRecInfoGet(__u8 A_Group, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize);
BOOL8	GrFsNvtGroupRecTimeRngGet(__u8 A_Group, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd, Def_GrTime* A_PtrRtTimeLast = NULL);
__u8	GrFsNvtDiskTemperatureGet(__u8 A_DiskIdx);
__u64	GrFsNvtGroupRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd);
BOOL8	GrFsNvtGroupDiskFormatReq(__u8 A_GroupMap, BOOL8 A_IsPost);
__u8	GrFsNvtDiskInstMapGet(__u8* A_PtrRtBadMap);
__u8	GrFsNvtRecInfoGet(__u8* A_PtrRtGroup, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize);

__u32	GrFsNvtMonthDayMapGet(__u8 A_Group, __u32 A_TimeMonth);
BOOL8	GrFsNvtMinRatbTblGet(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrBuf, BOOL8 A_IsPost = FALSE, __u32 A_UserData = 0);

__s32	GrFsNvtLogGetOpen(__u8 A_Group, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_MaxCnt,
	__u32 A_ChMask, __u64 A_TypeMask, Ptr_GrDvrLogItm A_PtrRtBuf, BOOL8 A_IsFrmMode = FALSE);
BOOL8	GrFsNvtLogGetChkDone(__s32 A_Hndl, __u32* A_PtrRtCnt);
Def_GrErrCode	GrFsNvtLogGetFrameModeCheck(__s32 A_Hndl, __u32* A_PtrRtCnt);
BOOL8	GrFsNvtLogGetFrameModeResume(__s32 A_Hndl);
void	GrFsNvtLogGetClose(__s32 A_Hndl);

BOOL8	GrFsNvtSmartRead(__u32 A_DskIdx, Ptr_GrFsNvtSmart A_PtrRtData);

void	GrFsNvtGroupMakeFull(__u8 A_Group);

void	GrFsNvtDecBufInit();
void	GrFsNvtDecBufFinish();
void*	GrFsNvtDecBufGet(__s32 A_Size, __s32* A_PtrRtDecOfs);


//====================================================================
//out Environment conditional
#endif


