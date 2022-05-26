/*
	GAUSS DVR file system v6 group

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs6Grp
#define	_EPC_GrDvrFs6Grp

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

#include <Dvr/Fs/V6/GrDvrFs6Base.h>
#include <Dvr/Fs/V6/GrDvrFs6Disk.h>
	
//====================================================================
//constance

#define	E_GrDvrFs6GrpBlkNegGap			(30 * E_GrTimeSecOfMin)

#define E_GrDvrFs6GrpDbgMsgView
//#define	E_GrDvrFs6GrpDbgTailCort


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs6BlkFixInfo
{
	__u8	OvwtId;				// record overwrite count
	__s32	DskIdx;			// record disk internal index
	__s32	BlkIdx;			// record block index
	__u32	SeqId;			// record sequence id
}	*Ptr_GrDvrFs6BlkFixInfo;

typedef struct St_GrDvrFs6RecChInfo
{
	BOOL8	IsDbChg;
	BOOL8	IsDayMapChg;
	BOOL8	IsTimeMapChg;
	__u8	OvwtId;				// record overwrite count

	__s32	DskIdx;			// record disk internal index
	__s32	BlkIdx;			// record block index
	__u32	SeqId;			// record sequence id

	Ptr_GrDvrFs6BlkDbHead		PtrDb;
	St_GrDvrFs6FrmDayMap		DayMap;
	St_GrDvrFs6FrmTimeMap		TimeMap;

}	*Ptr_GrDvrFs6RecChInfo;

//====================================================================
//class

class	Cls_GrDvrFs6Grp
{
	private:

		// disk
		__u8	m_GrpType;			// group type
		__u8	m_DiskMap;			// disk map
		__u8	m_GrpStat;			// group status

		__u32	m_DiskCnt;			// group disk count

		Cls_GrDvrFs6Disk*	m_DiskTbl[E_GrDvrFs6MaxDiskCnt];		// disk table

		__u64	m_DiskSid;			// group disk system id

		BOOL8	m_IsOvwtMode;		// overwrite mode

		//St_GrDvrFs6GrpChInfo	m_ChTbl[E_GrDvrMaxChCnt];

		// record control

		St_GrDvrFs6RecChInfo	m_RecChTbl[E_GrDvrFs6MaxChCnt];

		// read environment
		BOOL8	m_IsReadInTimeMap;
		St_GrDvrFs6FrmTimeMap	m_ReadTimeMap;

	protected:
		//local
		BOOL8	LcGrpChkBlkSeqOvlp(__u32 A_DskIdx, __u32 A_BlkIdx );
		void	LcGrpDskTblBuild(void);
		__u32	LcGrpCalcBlkSeqId(__u32 A_DskIdx, __u32 A_BlkIdx);
		void	LcGrpSeqIdRebuild(void);
		void	LcGrpSeqIdRefresh(void);

		BOOL8	LcGrpMibUpdate(void);

		__s32	LcDskBlkFindEmpty(__u32 A_DskIdx);
		__u8	LcDskBlkTimeOfsFindByTimeFw(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_Time, __u32* A_PtrRtOfs);
		__u8	LcDskBlkTimeOfsFindByTimeBw(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_Time, __u32* A_PtrRtOfs);
		__u32	LcDskBlkGetDayMap(__u32 A_DskIdx, __u32 A_BlkIdx, __u16 A_DaySt, __u16 A_DayEd, Ptr_GrDvrFs6FrmDayMap A_PtrWorkBuf);
		BOOL8	LcDskBlkGetMinMap(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_TimeSt, __u32 A_TimeEd, Ptr_GrDvrRecMinMap A_PtrMinMap);
		__u64	LcDskBlkGetExpSize(__u32 A_DskIdx, __u32 A_BlkIdx, __u32 A_TimeSt, __u32 A_TimeEd, __u32 A_ChMap,
			Ptr_GrDvrFs6FrmTimeMap  A_PtrWorkBuf);

		BOOL8	LcGrpBlkFindEmpty(__s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx);
		BOOL8	LcGrpBlkFindOldest(__s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx);
		BOOL8	LcGrpBlkFindNewest(__s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx);
		BOOL8	LcGrpBlkFindByTimeFw(__u8 A_Ch, __u32 A_Time, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId);
		BOOL8	LcGrpBlkFindByTimeBw(__u8 A_Ch, __u32 A_Time, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId);
		BOOL8	LcGrpBlkFindBySeqFw(__u8 A_Ch, __u32 A_SeqId, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId);
		BOOL8	LcGrpBlkFindBySeqBw(__u8 A_Ch, __u32 A_SeqId, __s32* A_PtrRtDskIdx, __s32* A_PtrRtBlkIdx, __u32* A_PtrRtBlkSeq, __u8* A_PtrRtOvwtId, __u32* A_PtrRtOfs);

		BOOL8	LcGrpRecBlkClose(__u8 A_Ch, BOOL8 A_IsFull = FALSE, BOOL8 A_IsBad = FALSE);
		BOOL8	LcGrpRecBlkOpen(__u8 A_Ch, __u32 A_DskIdx, __u32 A_BlkIdx, BOOL8 A_IsNew = FALSE);

		void	LcGrpRecDayMapInit(__u8 A_Ch, __u32 A_OfsPrv);
		void	LcGrpRecTimeMapInit(__u8 A_Ch, __u32 A_OfsPrv);

		__u8	LcGrpRecTimeMapWrite(__u8 A_Ch);
		__u8	LcGrpRecDayMapWrite(__u8 A_Ch);

		__u8	LcGrpRecAddFrm(__u8 A_Ch, void* A_PtrFrm, __u32 A_Size, __u32* A_PtrRtOfs);
		__u8	LcGrpRecAddTime(__u8 A_Ch, __u32 A_Time, BOOL8 A_IsSm, __u32 A_Ofs, __u8 A_Ratb, __u32 A_BkSize);
		__u8	LcGrpRecAddDay(__u8 A_Ch, __u32 A_Time);
		__u8	LcGrpRecAddTimeToBlk(__u8 A_Ch, __u32 A_Time, BOOL8 A_IsSm);

		__u8	LcGrpDataWriteByHndl(__u64 A_Hndl, void* A_PtrData, __u32 A_Size);
		__u8	LcGrpDataReadByHndl(__u64 A_Hndl, void* A_PtrData, __u32 A_Size);

	public:
		Cls_GrDvrFs6Grp(__u8 A_GroupType, __u8 A_DskMap, BOOL8 A_IsOvrtMode);
    virtual		~Cls_GrDvrFs6Grp();

		// record
		BOOL8	FrmVdoAdd(Ptr_GrDvrFrmHdVdo A_PtrFrm);
		BOOL8	FrmAdoAdd(Ptr_GrDvrFrmHdAdo A_PtrFrm);
		BOOL8	LogAdd(Ptr_GrDvrLogItm A_PtrItem);

		__u8	StatusGet(void);

		__u8	ReadFwGoByTime(Ptr_GrDvrFs6ReadCtrl A_PtrCtrl, Def_GrTime A_Time);
		__u8	ReadFwNext(Ptr_GrDvrFs6ReadCtrl A_PtrCtrl);

		__u8	ReadBwGoByTime(Ptr_GrDvrFs6ReadCtrl A_PtrCtrl, Def_GrTime A_Time);
		__u8	ReadBwNext(Ptr_GrDvrFs6ReadCtrl A_PtrCtrl);

		__u8	GroupInfoGet(__u64* A_PtrRtTotal, __u64* A_PtrRtRec);
		__u8	GroupDataReadByHndl(__u64 A_Hndl, void* A_PtrData, __u32 A_Size);

		BOOL8	RecTimeRangeGet(__u32* A_PtrRtTimeSt, __u32* A_PtrRtTimeEd);
		__u32	RecMonthDayMapGet(__u32 A_TimeMonth);
		BOOL8	RecMinRatbTblGet(__u32 A_TimeMin, __u32 A_MinCnt, Ptr_GrDvrRecMinMap A_PtrMinMap);
		__u64	RecExpectSizeGet(__u32 A_TimeSt, __u32 A_TimeEd, __u32 A_ChMap);

		__u8	LogOpen(Ptr_GrDvrFs6LogCtrl A_PtrCtrl);
		__u8	LogReadNext(Ptr_GrDvrFs6LogCtrl A_PtrCtrl);

		void	RecFlush(void);
};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


