/*
	GAUSS DVR file system v5 raid group

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrFsNvtRgrp
#define	_EPC_GrFsNvtRgrp

//====================================================================
// option

#define E_GrFsNvtRgrpDbgMsgView
//#define	E_GrFsNvtRgrpDbgTailCort


//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskBase.h>
#include <GrTaskCli.h>

#ifdef LINUX_APP
#include <sys/uio.h>
#else
#include <EmuLnx\GrElaBase.h>
#endif

#include <GrError.h>

#include <Dvr/Fs/Nvt/GrFsNvtBase.h>
#include <Dvr/Fs/Nvt/GrFsNvtHdd.h>
	
//====================================================================
//constance

#define	E_GrFsNvtRgrpCchMinMaxCnt		E_GrFsNvtHwBufMinMaxCnt

#define	E_GrFsNvtRgrpCchDataMaxCnt	E_GrFsNvtHwBufDataMaxCnt
#define	E_GrFsNvtRgrpCchDataMaxRsvd	8						/* max reserved count */
#define	E_GrFsNvtRgrpCchDataBufSize	0x400000		/* 4MB */
#define	E_GrFsNvtRgrpCchDataAdrMask	0xFFC00000
#define	E_GrFsNvtRgrpCchDataOfsMask	0x003FFFFF

//====================================================================
//global macro

//====================================================================
//global type


typedef struct St_GrFsNvtRecBlkEnv
{
	BOOL8	IsDbChg;
	BOOL8	IsMinuteChg;
	BOOL8	IsLogChg;
	__u32	TimeMinute;

	__s32	OfsLog;

	Ptr_GrFsNvtDbBlkInfo	PtrDb;
	St_GrFsNvtMinuteInfo	MinuteInfo;
	St_GrFsNvtFrmLogPool	Log;
}	*Ptr_GrFsNvtRecBlkEnv;

typedef struct St_GrFsNvtRgrpRbwChCtrl
{
	__u8	Ratb;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	BkSize;
}	*Ptr_GrFsNvtRgrpRbwChCtrl;

typedef struct St_GrFsNvtRgrpRbwCtrl
{
	iovec*	IocTbl;
	__u32	IocPrgs;
	__u32	IocCnt;

	St_GrFsNvtRgrpRbwChCtrl	ChTbl[E_GrDvrMaxChCnt];
}	*Ptr_GrFsNvtRgrpRbwCtrl;

typedef struct St_GrFsNvtRgrpCchMinItem
{
	__s32	DskIdx;		// 0 > not exist
	__u32	DayIdx;
	__u32	AccTime;	// lst access time
	BOOL8	IsDirt;		// data dirt
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrFsNvtMinuteInfo	PtrBuf;
}	*Ptr_GrFsNvtRgrpCchMinItem;

typedef struct St_GrFsNvtRgrpCchMinCtl
{
	__u32	Cnt;		// cache count
	St_GrFsNvtRgrpCchMinItem	Items[E_GrFsNvtRgrpCchMinMaxCnt];
}	*Ptr_GrFsNvtRgrpCchMinCtl;

typedef struct St_GrFsNvtRgrpCchDataItem
{
	BOOL8	IsDirt;		// data dirt
	__u8	Locked;		// locked count
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	AccTime;		// last access time

	void*	PtrBuf;

	__u32	BlkSeq;
	__u64	Hndl;			// handle
}	*Ptr_GrFsNvtRgrpCchDataItem;

typedef struct St_GrFsNvtRgrpCchDataRsvdItem
{
	__u64	Hndl;
}	*Ptr_GrFsNvtRgrpCchDataRsvdItem;

typedef struct St_GrFsNvtRgrpCchDataRsvdCtl
{
	BOOL8	IsDirBack;	// cache direcition
	__u8	ReqCnt;			// reseved request count
	__u8	Prgs;				// now reserved progressed count
	__u8	_rsvd0;

	__u64	HndlBase;		// base handle

	St_GrFsNvtRgrpCchDataRsvdItem	Items[E_GrFsNvtRgrpCchDataMaxRsvd];

}	*Ptr_GrFsNvtRgrpCchDataRsvdCtl;

typedef struct St_GrFsNvtRgrpCchDataCtl
{
	__u32	Cnt;
	St_GrFsNvtRgrpCchDataRsvdCtl	Rsvd;
	St_GrFsNvtRgrpCchDataItem			Items[E_GrFsNvtRgrpCchDataMaxCnt];
}	*Ptr_GrFsNvtRgrpCchDataCtl;

//====================================================================
//class

class	Cls_GrFsNvtRgrp
{
	private:

		// disk
		__u8	m_GrpType;			// group type
		__u8	m_DiskMap;			// disk map
		__u8	m_GrpStat;			// group status

		BOOL8	m_IsCchRsvAble;	// cache reserveable

		__u32	m_DiskCnt;			// group disk count
		Cls_GrFsNvtHdd*					m_DiskTbl[E_GrFsNvtMaxDiskCnt];		// disk table

		__u64	m_DiskId;				// group disk id

		BOOL8	m_IsOvwtMode;		// overwrite mode

		// group info
		Def_GrTime	m_RecTimeSt;
		Def_GrTime	m_RecTimeEd;
		Def_GrTime	m_RecTimeLast;
		__u64	m_GrpTotalSize;
		__u64	m_GrpRecSize;

		// record control
		__s32	m_RecDskIdx;		// record disk internal index
		__s32	m_RecBlkIdx;		// record block index
		__u32	m_RecSeqId;			// record sequence id
		__u32	m_RecSeqOvwt;		// record sequence overwrite count

		St_GrFsNvtRecBlkEnv	m_RecBlkEnv;

		// record buffer write
		St_GrFsNvtRgrpRbwCtrl	m_RbwCtl;

		// cache
		St_GrFsNvtRgrpCchMinCtl		m_CchMin;		// minute cache
		St_GrFsNvtRgrpCchDataCtl	m_CchData;	// data cache


	protected:
		//local
		void	LcInit(void);

		void	LcGrpDskTblBuild(void);
		__u32	LcGrpCalcSeqId(__u8 A_DskIdx, __u32 A_BlkIdx, __u32 A_TimeSt);
		void	LcGrpSeqIdRebuild(void);
		void	LcGrpRecInfoInit(void);

		BOOL8	LcGrpChkBlkInfoBroken(void);
		BOOL8	LcBlkChkSeqOverlapped(__u32 A_DskIdx, __u32 A_BlkIdx,__u32 A_SeqId);

		BOOL8	LcGrpMibUpdate(void);

		// minmap
		BOOL8	LcMinuteTblChkAndFix(__u32 A_DskIdx, Ptr_GrFsNvtMinuteInfo A_PtrTbl, BOOL8 A_IsFastChk, BOOL8* A_PtrRtIsFixed); // reseult = data exist
		BOOL8	LcDskDayMinMapGet(__u32 A_DskIdx, __u32 A_TimeDay, Ptr_GrDvrRecMinMap A_PtrMinMap);
		Ptr_GrFsNvtMinuteInfo	LcDskMinuteTblGetByIdx(__u32 A_DskIdx, __u32 A_DayIdx, Def_GrErrCode* A_PtrRtErrCode, BOOL8 A_IsReadSkip);
		Def_GrErrCode	LcDskMinuteItemRead(__u32 A_DskIdx, __u32 A_TimeMinute, Ptr_GrFsNvtMinuteInfo A_PtrRtBuf);
		Def_GrErrCode	LcDskMinuteItemWrite(__u32 A_DskIdx, __u32 A_TimeMinute, Ptr_GrFsNvtMinuteInfo A_PtrBuf);

		// minmap cache
		void	LcCchMinInit(void);
		void	LcCchMinFinish(void);
		__s32	LcCchMinFindInCache(__u32 A_DskIdx, __u32 A_DayIdx);
		__s32	LcCchMinLoadToCache(__u32 A_DskIdx, __u32 A_DayIdx, Def_GrErrCode* A_PtrRtErrCode, BOOL8 A_IsReadSkip);
		Def_GrErrCode	LcCchMinFlushByCchIdx(__u32 A_CchIdx);
		BOOL8	LcCchMinAccessUpdate(__u32 A_DskIdx, __u32 A_DayIdx, BOOL8 A_IsDirt);
		void	LcCchMinFlushAll(void);

		// data cache
		void	LcCchDataInit(void);
		void	LcCchDataFinish(void);

		__s32	LcCchDataGetByHndl(__u64 A_Hndl, BOOL8 A_IsReadSkip, __u32* A_PtrRtInSize, Def_GrErrCode* A_PtrRtErr);	// return - cache index
		Def_GrErrCode	LcCchDataGetByHndlToSctl(Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl);
		//void	LcCchDataLockByHndl(__u64 A_Hndl);

		void	LcCchDataUnlockByIdx(__u32 A_CchIdx);
		void	LcCchDataUnlockByHndl(__u64 A_Hndl);
		void	LcCchDataUnlockBySctl(Ptr_GrFsNvtSeekCtrl A_PtrCtrl);
		void	LcCchDataMarkAccByIdx(__u32 A_CchIdx, BOOL8 A_IsWrite);
		void	LcCchDataMarkAccByHndl(__u64 A_Hndl, BOOL8 A_IsWrite);

		__s32	LcCchDataFindByHndl(__u64 A_Hndl);
		__s32	LcCchDataFindEmpty(void);
		__s32	LcCchDataFindLoadable(void);
		BOOL8	LcCchDataChkRsvdByIdx(__u32 A_CchIdx);

		void	LcCchDataFlushByIdx(__u32 A_CchIdx);
		void	LcCchDataFreeByIdx(__u32 A_CchIdx);
		void	LcCchDataFlushAll(void);

		__u32	LcCchDataCalSizeByHndl(__u64 A_Hndl);

		void	LcCchDataReserve(BOOL8 A_IsDirBack, __u8 A_ReqCnt, __u64 A_HndlBase);

		// disk
		//BOOL8	LcDskLoadByHndl

		__s32	LcDskBlkFindEmpty(__u32 A_DskIdx);
		__u64	LcDskExpSizeGet(__u32 A_DskIdx, __u32 A_TimeSt, __u32 A_TimeEd, __u32 A_ChMap);
		BOOL8	LcDskDayCheckByIdx(__u32 A_DskIdx, __u32 A_DayIdx, BOOL8 A_IsFastChk, BOOL8 A_IsBadDelete);
		__s32	LcDskDayFind(__u32 A_DskIdx, __u32 A_TimeDay);
		Def_GrErrCode	LcDskDayDelByIdx(__u32 A_DskIdx, __u32 A_DayIdx);
		__s32	LcDskDayCreate(__u32 A_DskIdx, __u32 A_TimeDay);
		BOOL8	LcDskBlkDbErase(__u32 A_DskIdx, __u32 A_BlkIdx);
		Ptr_GrFsNvtDbBlkInfo	LcDskDbBlkInfoGet(__u32 A_DskIdx, __u32 A_BlkIdx);
		BOOL8	LcDskDayFindByTimeFw( __u32 A_DskIdx, __u32 A_Time, __s32* A_PtrRtDayIdx, __u32* A_PtrRtTime);
		BOOL8	LcDskDayFindByTimeBw(__u32 A_DskIdx, __u32 A_Time,  __s32* A_PtrRtDayIdx, __u32* A_PtrRtTime);

		Def_GrErrCode	LcDskReadToBuf(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_Offset, __u32 A_Size, void* A_PtrBuf);
		Def_GrErrCode	LcDskWriteFromBuf(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_Offset, __u32 A_Size, void* A_PtrBuf);

		// group
		Def_GrErrCode	LcGrpReadAlignByHndl(__u64 A_Hndl, void* A_PtrBuf, __u32* A_PtrRtSize);
		Def_GrErrCode	LcGrpWriteAlignByHndl(__u64 A_Hndl, void* A_PtrBuf, __u32* A_PtrRtSize);

		void	LcGrpStatUpdate(void);		// group status update

		Ptr_GrFsNvtDbBlkInfo	LcGrpDbBlkInfoGetByHndl(__u64 A_Hndl);

		BOOL8	LcGrpBlkAdrFindByTimeFw(__u32 A_Time, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtSeqId, __u8* A_PtrRtOvwtId, __s32* A_PtrRtOfs);
		BOOL8	LcGrpBlkAdrFindByTimeBw(__u32 A_Time, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtSeqId, __u8* A_PtrRtOvwtId, __s32* A_PtrRtOfs);

		Def_GrErrCode	LcGrpReadSctlFwByHndl(Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl);
		Def_GrErrCode	LcGrpReadSctlBwByHndl(Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl);

		BOOL8	LcGrpRecMinuteOpen(__u32 A_TimeMinute);
		BOOL8	LcGrpRecMinuteClose(void);
		Def_GrErrCode	LcGrpRecMinuteFlush(void);

		Def_GrErrCode	LcGrpRbwWriteToRecBlk(void);		// block write

		Def_GrErrCode	LcGrpRecBlkOpen(__u32 A_DskIdx, __u32 A_BlkIdx);
		Def_GrErrCode	LcGrpRecBlkNew(__u32 A_DskIdx, __u32 A_BlkIdx);
		Def_GrErrCode	LcGrpRecBlkClose(BOOL8 A_IsFull = FALSE, BOOL8 A_IsBad = FALSE);

		void	LcGrpRecLogPoolPktBuild(__s32 A_OfsPrv);
		Def_GrErrCode	LcGrpRecLogPoolOpen(__u32 A_DskIdx, __u32 A_BlkIdx);
		Def_GrErrCode	LcGrpRecLogPoolWrite(void);
		Def_GrErrCode	LcGrpRecLogPoolAdd(void);

		BOOL8	LcGrpBlkFindEmpty(__s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx);
		BOOL8	LcGrpBlkFindOldest(__s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtSeqId, __u8* A_PtrRtOvwtId);
		BOOL8	LcGrpBlkFindNewest(__s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtSeqId);

		BOOL8	LcGrpBlkFindByTimeRngBw(__u32 A_TimeSt, __u32 A_TimeEd, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId);

		BOOL8	LcGrpBlkFindBySeqFw(__u32 A_SeqId, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId);
		BOOL8	LcGrpBlkFindBySeqBw(__u32 A_SeqId, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId, __u32* A_PtrRtOfs);


		BOOL8	LcGrpDayFindByTimeFw(__u32 A_Time, __s32* A_PtrRtDskIdx, __s32* A_PtrRtDayIdx,__u32* A_PtrRtTime);
		BOOL8	LcGrpDayFindByTimeBw(__u32 A_Time, __s32* A_PtrRtDskIdx, __s32* A_PtrRtDayIdx, __u32* A_PtrRtTime);

		BOOL8	LcGrpDayRangeGet(__u32* A_PtrRtDaySt, __u32* A_PtrRtDayEd);

		__u32	LcGrpRecFreeSizeGet(void);

		Def_GrErrCode	LcGrpRecAddTimeByRbw(__u32 A_Time, __u32 A_Ofs, Ptr_GrFsNvtRgrpRbwChCtrl A_PtrRbwCi);
		Def_GrErrCode	LcGrpRecAddBlkTimeRange(__u32 A_TimeMin, __u32 A_TimeMax);

		void	LcGrpDelSlateBlk(__u32 A_TimeBase);
		void	LcGrpDelSlateTimeMap(__u32 A_TimeBase);

	public:
		Cls_GrFsNvtRgrp(__u8 A_GroupType, __u8 A_DskMap, BOOL8 A_IsOvrtMode, BOOL8 A_IsCchRsvAble );
    virtual		~Cls_GrFsNvtRgrp();

		// record
		Def_GrErrCode	LogAdd(Ptr_GrDvrLogItm A_PtrItem);
		void	RecBufAdd(iovec* A_PtrIovs, __u32 A_Cnt);

		__u8	StatusGet(BOOL8 A_IsRefresh);

		Def_GrErrCode	ReadFwByHndl(Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl);
		Def_GrErrCode	ReadFwByTime(Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time);
		Def_GrErrCode	ReadFwStart(Ptr_GrFsNvtSeekCtrl A_PtrCtrl);

		Def_GrErrCode	ReadBwByTime(Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time);

		Def_GrErrCode	ReadFwNext(Ptr_GrFsNvtSeekCtrl A_PtrCtrl);
		Def_GrErrCode	ReadBwNext(Ptr_GrFsNvtSeekCtrl A_PtrCtrl);

		Def_GrErrCode	SctlFree(Ptr_GrFsNvtSeekCtrl A_PtrCtrl);

		__u8	GroupInfoGet(__u64* A_PtrRtTotal, __u64* A_PtrRtRec,BOOL8 A_IsRefresh);

		__u32	RecTimeLastGet(void);
		void	RecTimeRangeGet(__u32* A_PtrRtTimeSt, __u32* A_PtrRtTimeEd);
		__u32	RecMonthDayMapGet(__u32 A_TimeMonth);
		BOOL8	RecMinRatbTblGet(__u32 A_TimeDay, __u32 A_MinCnt, Ptr_GrDvrRecMinMap A_PtrMinMap);
		__u64	RecExpectSizeGet(__u32 A_TimeSt, __u32 A_TimeEd, __u32 A_ChMap);

		void	RecFlush(void);

		void	DeleteStaleData(__u32 A_TimeBase);

		BOOL8	RtlCchDataRsvProc(void);		// do runtime
		void	RtlLogReadProc(Ptr_GrFsNvtIoLogGetRtl A_PtrCtl, Ptr_GrFsNvtFrmLogPool A_PtrFrmBuf);

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


