/*
	GAUSS DVR file system V6 base

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs6Base
#define	_EPC_GrDvrFs6Base

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/GrDvrBase.h>

#ifdef LINUX_APP
#include <sys/uio.h>
#else
#include <EmuLnx\GrElaBase.h>
#endif
	
//====================================================================
//constance

#define E_GrDvrFs6MaxDiskCnt							8

#define	E_GrDvrFs6AdoChBase								E_GrDvrMaxChCnt
#define	E_GrDvrFs6MaxChCnt								(E_GrDvrMaxChCnt * 2)		/* include audio channel */

#define E_GrDvrFs6HndlOvwtIdPos						56					/* overwrite id bit position */
#define E_GrDvrFs6HndlGrpIdxMask					0x07LL			/* group disk idx mask */
#define E_GrDvrFs6HndlOfsMask							0x000000000FFFFFE0LL					/* record data address position mask */
#define	E_GrDvrFs6HndlBlkIdxPos						28

#define E_GrDvrFs6GrpIntNone							0xFF				/* not group appertain */

#define	E_GrDvrFs6DayMapCntOfFrm					(512 - 5)	/* align 4Kbyte */

#define	E_GrDvrFs6MinMapOfsCnt						4		/* minute offset table count */
#define	E_GrDvrFs6MinMapCntOfFrm					1024

#define	E_GrDvrFs6LogPoolCntOfFrm					(2048 - 2)

// disk
#define E_GrDvrFs6Fcc											Mac_GrMakeFcc('G','D','F','6')

#define E_GrDvrFs6Ver											1		/* sub version */

// block info
#define E_GrDvrFs6BlkSize									0x0000000010000000LL
#define E_GrDvrFs6BlkOfsMask							0x0FFFFFE0
#define E_GrDvrFs6BlkShift								28

#define	E_GrDvrFs6BlkMaxCnt								0xFFFF

#define	E_GrDvrFs6BlkHeadSize							64

#define	E_GrDvrFs6BlkRecAbleSize					(E_GrDvrFs6BlkSize - E_GrDvrFs6BlkHeadSize)

// block type
#define	E_GrDvrFs6BlkTypeNone							0
#define	E_GrDvrFs6BlkTypeMib							1
#define	E_GrDvrFs6BlkTypeDb								2
#define	E_GrDvrFs6BlkTypeLog							3

#define	E_GrDvrFs6BlkTypeBad							255

// block DB attribute
#define	E_GrDvrFs6BlkAtbFull							0x01

// block index
#define	E_GrDvrFs6BlkIdxMib								0		/* master information block */
#define	E_GrDvrFs6BlkIdxLog								1		/* log block */
#define	E_GrDvrFs6BlkIdxDb								2		/* database block */

// disk id
#define	E_GrDvrFs6DiskIdMaskMac						0x0000FFFFFFFFFFFFLL
#define	E_GrDvrFs6DiskIdGrpTypePos				48

// sequence id	
#define E_GrDvrFs6SeqIdNone								0
#define	E_GrDvrFs6SeqIdMin								1
#define	E_GrDvrFs6SeqIdMax								0xFFFFFFFF
#define	E_GrDvrFs6SeqIdTolerance					0xFFE00000

// day map
#define	E_GrDvrFs6FrmDayMapItemCnt				108		/* (256 - 32(head) - 8(tail)) / 2 =  */


// time map
#define	E_GrDvrFs6TimeMapSecUnit					30		/* 30 sec */
#define	E_GrDvrFs6TimeMapMinElemCnt				(E_GrTimeSecOfMin / E_GrDvrFs6TimeMapSecUnit)

#define	E_GrDvrFs6FrmTimeMapItemCnt				256

// dmux
#define E_GrDvrFs6MaxDmuxCnt							34					/* max client + main play + backup */

// cache
#define	E_GrDvrFs6CchFlushPeriod					10

#define	E_GrDvrFs6CchUnitShft							16
#define	E_GrDvrFs6CchUnitSize							0x10000
#define	E_GrDvrFs6CchItemMaxCnt						512
#define	E_GrDvrFs6CchAdrMask							0xFFFFFFFFFFFF0000LL
#define	E_GrDvrFs6CchOfsMask							0x000000000000FFFFLL

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs6CchItem
{
	__u8	DskId;
	BOOL8	IsUse;
	BOOL8	IsDirt;
	__u8	_rsvd0;
	__u32	AccTick;
	__u64	DskAdr;
}	*Ptr_GrDvrFs6CchItem;

typedef struct St_GrDvrFs6CchCtrl
{
	St_GrDvrFs6CchItem	CtlTbl[E_GrDvrFs6CchItemMaxCnt];
	void*	PtrBuf;
	__u32	ItemCnt;
}	*Ptr_GrDvrFs6CchCtrl;

typedef struct St_GrDvrFs6BlkBaseHead
{
	__u32	Fcc;
	__u8	BlkType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	_rsvd3[14];

}	*Ptr_GrDvrFs6BlkBaseHead;

typedef struct St_GrDvrFs6BlkMibHead
{
	__u32	Fcc;
	__u8	BlkType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u64	DiskSid;	// disk system id
	__u32	FsVer;

	__u32	_rsvd3[11];
}	*Ptr_GrDvrFs6BlkMibHead;

typedef struct St_GrDvrFs6BlkLogHead
{
	__u32	Fcc;
	__u8	BlkType;
	BOOL8	IsFull;
	__u8	_rsvd0;
	__u8	_rsvd1;

	__u32	InBlk;
	__u32	InCnt;

	__u32	_rsvd2[12];
}	*Ptr_GrDvrFs6BlkLogHead;

typedef struct St_GrDvrFs6BlkDbHead
{
	__u32	Fcc;
	__u8	BlkType;
	__u8	BlkAtb;
	__u8	OvwtId;
	__u8	Ch;
	__u32	SeqId;
	__u32	InSize;

	St_GrTimeRng	TimeRng;
	__u32	OfsLastDate;
	__u32	OfsLastTime;
	__u32	TimeCreate;		// when use re align

	__u32	_rsvd1[7];
}	*Ptr_GrDvrFs6BlkDbHead;

typedef struct St_GrDvrFs6ReadFrameItem
{
	__u8	Ch;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	void*	PtrBuf;
}	*Ptr_GrDvrFs6ReadFrameItem;

typedef	struct St_GrDvrFs6ReadCtrl
{
	__u8	Ch;
	BOOL8	IsFrmRead;
	__u8	Stat;
	__u8	_rsvd1;
	__u32	BlkSeq;				// block sequence id
	__u64	Hndl;					// disk handle
	St_GrDvrFrmHdUnknown	FrmHd;

}	*Ptr_GrDvrFs6ReadCtrl;

typedef struct St_GrDvrFs6DmuxCtrl
{
	void*	Obj;
}	*Ptr_GrDvrFs6DmuxCtrl;

typedef struct St_GrDvrFs6LogCtrl
{
	// initialize area
	Ptr_GrDvrLogItm	PtrBuf;
	Def_GrTime TimeSt;
	Def_GrTime TimeEd;
	__u32	MaxCnt;
	__u32	ChMask;
	__u64	TypeMask;

	// runtime area
	BOOL8	IsFinish;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	InCnt;				// data in count

	__u32	FrmPrgs;			// frame progress
	__s32	DskIdx;
	__u32	BlkSeq;				// now loaded block sequence id
	__u32	BlkIdx;
	__u32	FrmOfs;

	//St_GrDvrFrmLogPool	FrmBuf;
}	*Ptr_GrDvrFs6LogCtrl;

typedef struct St_GrDvrFs6Env
{
	__u64	DiskBaseId;
	__u32	BkupHdSize;
	__u32	DiskMaxCnt;
	void*	DskTbl[E_GrDvrFs6MaxDiskCnt];
	St_GrDvrFs6DmuxCtrl	DmuxTbl[E_GrDvrFs6MaxDmuxCnt];
	Cls_GrTaskCli*	ObjMng;

	__u32	TickFs;		// file system tick - sec unit

	St_GrDvrFs6CchCtrl	Cch;	// cache

}	*Ptr_GrDvrFs6Env;

typedef struct St_GrDvrFs6BkupSizeEstm
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
}	*Ptr_GrDvrFs6BkupSizeEstm;

typedef struct St_GrDvrFs6FrmHdDayMap
{
	St_GrDvrFrmHdBase	Base;
	__u32	InCnt;
	__u32	OfsPrv;
	__u32	_rsvd0;
}	*Ptr_GrDvrFs6FrmHdDayMap;

typedef struct St_GrDvrFs6FrmDayMap
{
	St_GrDvrFs6FrmHdDayMap	Head;

	__u16	DayTbl[E_GrDvrFs6FrmDayMapItemCnt];

	St_GrDvrFrmTail	Tail;
}	*Ptr_GrDvrFs6FrmDayMap;

typedef struct St_GrDvrFs6FrmHdTimeMap
{
	St_GrDvrFrmHdBase	Base;
	__u32	InCnt;
	__u32	OfsPrv;
	__u32	_rsvd0;
}	*Ptr_GrDvrFs6FrmHdTimeMap;

typedef struct St_GrDvrFs6TimeMapFrmItm
{
	__u32	Time;
	__u32	Ofs;

	__u8	Ratb;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	Size;
}	*Ptr_GrDvrFs6TimeMapFrmItm;

typedef struct St_GrDvrFs6FrmTimeMap
{
	St_GrDvrFs6FrmHdTimeMap	Head;

	St_GrDvrFs6TimeMapFrmItm	TimeTbl[E_GrDvrFs6FrmTimeMapItemCnt];

	__u32	_rsvd0[6];

	St_GrDvrFrmTail	Tail;
}	*Ptr_GrDvrFs6FrmTimeMap;



//====================================================================
//class

	
//====================================================================
//global function

void	GrDvrFs6Prepare(Cls_GrTaskSvr* A_TskSvr, __u64 A_MacAdr, __u32 A_BkupHdSize,__u32 A_DiskMaxCnt,__u32 A_CacheSize);
void	GrDvrFs6Init(void);
void	GrDvrFs6Finish(void);

void	GrDvrFs6HndlDec(__u64 A_Hndl, __s32* A_PtrRtDskIdx, __u32* A_PtrRtOffset, __s32* A_PtrRtBlkIdx, __u8* A_PtrRtOvwtId);
__u64	GrDvrFs6HndlEnc(__s32 A_DskIdx, __u32 A_Offset, __s32 A_BlkIdx, __u8 A_OvwtId);

BOOL8	GrDvrFs6FrmVdoAdd(Ptr_GrDvrFrmHdVdo A_PtrFrm);
BOOL8	GrDvrFs6FrmAdoAdd(Ptr_GrDvrFrmHdAdo A_PtrFrm);
BOOL8	GrDvrFs6LogAdd(Ptr_GrDvrLogItm A_PtrLog);

void	GrDvrFs6Setup(__u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir);
BOOL8	GrDvrFs6DiskFormatReq(__u32 A_DiskIdx, BOOL8 A_IsPost);
__u8	GrDvrFs6DiskRecInfoGet(__u32 A_DiskIdx, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize);
BOOL8	GrDvrFs6DiskModelNameGet(__u32 A_DiskIdx, WCHAR* A_StrRt);
__u8	GrDvrFs6GroupRecInfoGet(__u8 A_Group, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize);
BOOL8	GrDvrFs6GroupRecTimeRngGet(__u8 A_Group, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd);
__u8	GrDvrFs6DiskTemperatureGet(__u8 A_DiskIdx);
__u64	GrDvrFs6GroupRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd);

__u32	GrDvrFs6MonthDayMapGet(__u8 A_Group, __u32 A_TimeMonth);
BOOL8	GrDvrFs6MinRatbTblGet(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrBuf);

__u32	GrDvrFs6LogDayGet(__u8 A_Group,Def_GrTime A_Time, __u64 A_TypeMask, Ptr_GrDvrLogItm A_PtrBuf, __u32 A_MaxCnt);
__u32	GrDvrFs6LogAdvGet(__u8 A_Group, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_ChMask, __u64 A_TypeMask, Ptr_GrDvrLogItm A_PtrBuf, __u32 A_MaxCnt);

__s32	GrDvrFs6DmuxNew(__u8 A_MaxChCnt);
BOOL8	GrDvrFs6DmuxDelete(__s32 A_Hndl);
void	GrDvrFs6DmuxRecGroupSet(__s32 A_Hndl, __u8 A_RecGrpType);
Ptr_GrDvrFs6ReadCtrl	GrDvrFs6DmuxReadCtrlPtrGet(__s32 A_Hndl, __u8 A_Ch);
__u64	GrDvrFs6DmuxDo(__s32 A_Hndl);
__u64	GrDvrFs6DmuxReadyFrameRead(__s32 A_Hndl, Ptr_GrDvrFs6ReadFrameItem A_PtrBufTbl, __u32 A_BufCnt);
void	GrDvrFs6DmuxReadyFrameDone(__s32 A_Hndl, __u64 A_Map);
void	GrDvrFs6DmuxGoTime(__s32 A_Hndl, Def_GrTime A_Time);
void	GrDvrFs6DmuxDirSet(__s32 A_Hndl, BOOL8 A_IsDirBack);
void	GrDvrFs6DmuxVdoChAbleSet(__s32 A_Hndl, __u32 A_Map);
void	GrDvrFs6DmuxAdoChAbleSet(__s32 A_Hndl, __u32 A_Map);
void	GrDvrFs6DmuxVdoFrmMaskSet(__s32 A_Hndl, __u8 A_Ch, __u8 A_Mask);
__u8	GrDvrFs6DmuxRunStatGet(__s32 A_Hndl);


//====================================================================
//out Environment conditional
#endif


