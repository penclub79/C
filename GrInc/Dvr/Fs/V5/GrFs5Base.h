/*
	GAUSS DVR file system V5 base

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrFs5Base
#define	_EPC_GrFs5Base

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

#define E_GrFs5MaxDiskCnt									8

#define	E_GrFs5MaxChCnt										32

#define E_GrFs5HndlOvwtIdPos							56					/* overwrite id bit position */
#define E_GrFs5HndlGrpIdxMask							0x07LL			/* group disk idx mask */
#define E_GrFs5HndlOfsMask								0x000000001FFFFFE0LL					/* record data address position mask */
#define	E_GrFs5HndlBlkIdxPos							29
#define	E_GrFs5BlkOfsLimit								0x1FFFFFE0

// tiem base
#define	E_GrFs5TimeMin										1325376000		/* 2012 , 1, 1 */

// disk
#define E_GrFs5Fcc												Mac_GrMakeFcc('G','D','F','5')

#define E_GrFs5Ver												5		/* sub version */

// group

#define	E_GrFs5RgrpMaxCnt									2			/* raid group max count */
#define	E_GrFs5HwBufMinMaxCnt							32
#define	E_GrFs5HwBufDataMaxCnt						32


// block info
#define E_GrFs5BlkSize										0x0000000020000000LL
#define E_GrFs5BlkShift										E_GrFs5HndlBlkIdxPos

#define	E_GrFs5BlkMaxCnt									0xFFFF

#define	E_GrFs5BlkInfoSize								32
#define	E_GrFs5MinuteItemSize							176

// block status
#define	E_GrFs5BlkStatNormal							0
#define	E_GrFs5BlkStatFull								1
#define	E_GrFs5BlkStatBad									0xFF

// block offset
#define	E_GrFs5BlkOfsDayTbl								0x00200000		/* block offset of day table */
#define	E_GrFs5BlkOfsMinuteTbl						0x00201000		/* block offset of minute table */

// block index
#define	E_GrFs5BlkIdxMib									0		/* master information block */

// disk id
#define	E_GrFs5DiskIdMaskMac							0x0000FFFFFFFFFFFFLL
//#define	E_GrFs5DiskIdGrpTypePos						48	/* not use */

// sequence id	
#define E_GrFs5SeqIdNone									0
#define	E_GrFs5SeqIdMin										1
#define	E_GrFs5SeqIdMax										0xFFFFFFFF
#define	E_GrFs5SeqIdTolerance							0xFFE00000

// day map
#define	E_GrFs5DayInfoOldCntOfDisk				768			// V4
#define	E_GrFs5DayTblCntOfDisk						1024		// V5

// time map
//#define	E_GrFs5MinuteTblSizeOfDay					(E_GrFs5MinuteItemSize * 60 * 24)
#define	E_GrFs5TimeMapSecUnit							30		/* 30 sec */
#define	E_GrFs5MitDaySize									0x3E000

// log pool
#define	E_GrFs5FrmLogPoolItemCnt					2046	/* 0x10000 size  */

//====================================================================
//global macro


//====================================================================
//global type

typedef struct St_GrFs5MibInfo
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
}	*Ptr_GrFs5MibInfo;

typedef struct St_GrFs5DbBlkInfo
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
}	*Ptr_GrFs5DbBlkInfo;

typedef struct St_GrFs5DayInfo
{
	__u32	TimeDay;		// day base time
}	*Ptr_GrFs5DayInfo;

typedef struct St_GrFs5BlkAdr
{
	__u8	OvwtId;
	__u8	_rsvd0;
	__u16	Idx;
	__u32	Ofs;
}	*Ptr_GrFs5BlkAdr;

typedef struct St_GrFs5MinuteInfo
{
	St_GrFs5BlkAdr	AdrMin;	// minute offset
	St_GrFs5BlkAdr	Adr30s; // 30 sec offset
	__u8	ChRatbTbl[E_GrDvrMaxChCnt];
	__u32	ChSizeTbl[E_GrDvrMaxChCnt];
}	*Ptr_GrFs5MinuteInfo;


typedef struct St_GrFs5BlkInfoBase
{
	union 
	{
		St_GrFs5MibInfo	Mib;
		St_GrFs5DbBlkInfo	Db;
	};
}	*Ptr_GrFs5BlkInfoBase;

typedef	struct St_GrFs5SeekCtrl
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
}	*Ptr_GrFs5SeekCtrl;

typedef struct St_GrFs5HwBuf
{
	__u32	CchMinCnt;
	void*	CchMinBuf[E_GrFs5HwBufMinMaxCnt];

	__u32	CchDataCnt;
	void*	CchDataBuf[E_GrFs5HwBufDataMaxCnt];
}	*Ptr_GrFs5HwBuf;

typedef struct St_GrFs5Env
{
	__u64	DiskBaseId;
	//__u32	IoUnitSize;	// data buffer unit size
	//__u32	IoAdrMask;
	//__u32	IoOfsMask;

	__u32	BkupHdSize;
	__u32	DiskMaxCnt;
	__u32	Tick100ms;		// 100 ms tick
	void*	DskTbl[E_GrFs5MaxDiskCnt];
	Cls_GrTaskCli*	ObjMng;

	// cache buffer
	St_GrFs5HwBuf	HwBufs[E_GrFs5RgrpMaxCnt];
}	*Ptr_GrFs5Env;

typedef struct St_GrFs5BkupSizeEstm
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
}	*Ptr_GrFs5BkupSizeEstm;

typedef struct St_GrFs5FrmHdLogPool
{
	St_GrDvrFrmHdBase	Base;
	__u32	InCnt;
	__s32	OfsPrv;
	__u32	_rsvd0;
}	*Ptr_GrFs5FrmHdLogPool;

typedef struct St_GrFs5FrmLogPool
{
	St_GrFs5FrmHdLogPool	Head;

	St_GrDvrLogItm	LogTbl[E_GrFs5FrmLogPoolItemCnt];

	__u32	_rsvd0[6];

	St_GrDvrFrmTail	Tail;

}	*Ptr_GrFs5FrmLogPool;


typedef struct St_GrFs5LogFindReq
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

}	*Ptr_GrFs5LogFindReq;

typedef struct St_GrFs5LogFindRslt
{
	BOOL8				IsDone;		// finish
	BOOL8				IsOpen;		// opened
	BOOL8				IsPart;		// part in
	__u32				InCnt;		// input count
	Def_GrErrCode	ErrCode;	// error code
}	*Ptr_GrFs5LogFindRslt;

typedef struct St_GrFs5IoLogGetRtl
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

}	*Ptr_GrFs5IoLogGetRtl;

typedef struct St_GrFs5Smart
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
}	*Ptr_GrFs5Smart;

//====================================================================
//class

	
//====================================================================
//global function

void	GrFs5Prepare(Cls_GrTaskSvr* A_TskSvr, __u64 A_MacAdr, __u32 A_BkupHdSize,__u32 A_DiskMaxCnt,
	__u32 A_CchMinCnt, __u32 A_CchDataCnt);
void	GrFs5Init(void);
void	GrFs5Stop(void);
void	GrFs5Finish(void);

void	GrFs5HndlDec(__u64 A_Hndl, __s32* A_PtrRtDskIdx, __u32* A_PtrRtOffset, __s32* A_PtrRtBlkIdx, __u8* A_PtrRtOvwtId);
__u64	GrFs5HndlEnc(__s32 A_DskIdx, __u32 A_Offset, __s32 A_BlkIdx, __u8 A_OvwtId);

BOOL8	GrFs5LogAdd(Ptr_GrDvrLogItm A_PtrLog);
void	GrFs5RecBufAdd(iovec* A_PtrIovs, __u32 A_Cnt);

void	GrFs5Setup(__u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir);
void	GrFs5AutoDelSet(__u32 A_Sec);
BOOL8	GrFs5DiskFormatReq(__u32 A_DiskIdx, BOOL8 A_IsPost);
__u8	GrFs5DiskRecInfoGet(__u32 A_DiskIdx, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize);
BOOL8	GrFs5DiskModelNameGet(__u32 A_DiskIdx, WCHAR* A_StrRt);
__u8	GrFs5GroupRecInfoGet(__u8 A_Group, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize);
BOOL8	GrFs5GroupRecTimeRngGet(__u8 A_Group, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd, Def_GrTime* A_PtrRtTimeLast = NULL);
__u8	GrFs5DiskTemperatureGet(__u8 A_DiskIdx);
__u64	GrFs5GroupRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd);
BOOL8	GrFs5GroupDiskFormatReq(__u8 A_GroupMap, BOOL8 A_IsPost);
__u8	GrFs5DiskInstMapGet(__u8* A_PtrRtBadMap);
__u8	GrFs5RecInfoGet(__u8* A_PtrRtGroup, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize);

__u32	GrFs5MonthDayMapGet(__u8 A_Group, __u32 A_TimeMonth);
BOOL8	GrFs5MinRatbTblGet(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrBuf, BOOL8 A_IsPost = FALSE, __u32 A_UserData = 0);

__s32	GrFs5LogGetOpen(__u8 A_Group, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_MaxCnt,
	__u32 A_ChMask, __u64 A_TypeMask, Ptr_GrDvrLogItm A_PtrRtBuf, BOOL8 A_IsFrmMode = FALSE);
BOOL8	GrFs5LogGetChkDone(__s32 A_Hndl, __u32* A_PtrRtCnt);
Def_GrErrCode	GrFs5LogGetFrameModeCheck(__s32 A_Hndl, __u32* A_PtrRtCnt);
BOOL8	GrFs5LogGetFrameModeResume(__s32 A_Hndl);
void	GrFs5LogGetClose(__s32 A_Hndl);

BOOL8	GrFs5SmartRead(__u32 A_DskIdx, Ptr_GrFs5Smart A_PtrRtData);

void	GrFs5GroupMakeFull(__u8 A_Group);

//====================================================================
//out Environment conditional
#endif


