/*
	GAUSE DVR file system v3 manager

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs4Mng
#define	_EPC_GrDvrFs4Mng

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>
#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/Fs/V4/GrDvrFs4Base.h>
#include <Dvr/Fs/V4/GrDvrFs4Disk.h>
#include <Dvr/Fs/V4/GrDvrFs4Grp.h>
	
//====================================================================
//constance

#define E_GrDvrFs4MngGroupCnt								2

#define E_GrDvrFs4MngTimerPeriod						1000

#define E_GrDvrFs4MngTempScanPeriod					60


#define E_GrDvrFs4MngTmpBufSize							0x200000		/* 2 MByte */

#define	E_GrDvrFs4MngRecReqSlotCnt					2

#define	E_GrDvrFs4MngBkupEstmWorkUnit				8

// message command
#define E_GrDvrFs4MngMsgInit								1		/* initialize */
#define E_GrDvrFs4MngMsgSetup								2		/* St_GrDvrFs4MngMsgSetup */
#define E_GrDvrFs4MngMsgFormat							3		/* St_GrDvrFs4MngMsgFormat */
#define E_GrDvrFs4MngMsgDiskStatGet					4		/* St_GrDvrFs4MngMsgDiskStatGet */
#define E_GrDvrFs4MngMsgDiskRecSizeGet			5		/* St_GrDvrFs4MngMsgDiskRecSizeGet */
#define E_GrDvrFs4MngMsgGroupStatGet				6		/* St_GrDvrFs4MngMsgGroupStatGet */
#define E_GrDvrFs4MngMsgGroupRecSizeGet			7		/* St_GrDvrFs4MngMsgGroupRecSizeGet */
#define E_GrDvrFs4MngMsgGroupDayMapGet			8		/* St_GrDvrFs4MngMsgGroupDayMapGet */
#define E_GrDvrFs4MngMsgGroupMinMapGet			9		/* St_GrDvrFs4MngMsgGroupMinMapGet */
#define E_GrDvrFs4MngMsgReadFw							10	/* St_GrDvrFs4MngMsgReadPara */
#define E_GrDvrFs4MngMsgReadBw							11	/* St_GrDvrFs4MngMsgReadPara */
#define E_GrDvrFs4MngMsgReadBegin						12	/* St_GrDvrFs4MngMsgReadPara */
//#define E_GrDvrFs4MngMsgReadEnd							13	/* St_GrDvrFs4MngMsgReadPara */
#define E_GrDvrFs4MngMsgLogAdd							14	/* St_GrDvrFs4MngMsgLogAdd */
#define E_GrDvrFs4MngMsgLogFindStart				15	/* St_GrDvrFs4MngMsgLogFind */
#define E_GrDvrFs4MngMsgLogFindNext					16	/* St_GrDvrFs4MngMsgLogFind */
#define E_GrDvrFs4MngMsgLogFindPrev					17	/* St_GrDvrFs4MngMsgLogFind */
#define E_GrDvrFs4MngMsgSysInfo							18	/* St_GrDvrFs4MngMsgSysInfo */
#define E_GrDvrFs4MngMsgGroupTimeRngGet			19	/* St_GrDvrFs4MngMsgGroupTimeRngGet */
#define E_GrDvrFs4MngMsgBkupSizeEstm				20	/* St_GrDvrFs4MngMsgBkupSizeEstm */
#define E_GrDvrFs4MngMsgAtDelTimeSet				21	/* St_GrDvrFs4MngMsgAtDelTimeSet */
#define E_GrDvrFs4MngMsgGroupFormat					22	/* St_GrDvrFs4MngMsgGroupFormat */
#define E_GrDvrFs4MngMsgReadTimeFw					23	/* St_GrDvrFs4MngMsgReadTimePara */
#define E_GrDvrFs4MngMsgReadTimeBw					24	/* St_GrDvrFs4MngMsgReadTimePara */

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs4MngSysInfo
{
	__u8	DskStat[E_GrDvrFs4MaxDriveCnt];
	__u8	DskTemp[E_GrDvrFs4MaxDriveCnt];
	__u8	MainStat;
	__u8	MirrStat;
	__u64	MainCapTot;
	__u64	MainCapRec;
	__u32	MainRecTimeSt;
	__u32	MainRecTimeEd;
}	*Ptr_GrDvrFs4MngSysInfo;

// message
typedef	struct St_GrDvrFs4MngMsgBase 
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrDvrFs4MngMsgBase;

typedef	struct St_GrDvrFs4MngMsgSetup
{
	St_GrDvrFs4MngMsgBase	Base;
	__u8	NrmMap;
	BOOL8	IsNrmOvwt;
	__u8	MirMap;
	BOOL8	IsMirOvwt;
	BOOL8	IsAutoFix;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrDvrFs4MngMsgSetup;

typedef	struct St_GrDvrFs4MngMsgFormat
{
	St_GrDvrFs4MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	DevIdx;
	BOOL8	IsWithLog;
	__u8	_rsvd0;
	__u8	_rsvd1;
}	*Ptr_GrDvrFs4MngMsgFormat;

typedef	struct St_GrDvrFs4MngMsgDiskStatGet
{
	St_GrDvrFs4MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs4MngMsgDiskStatGet;

typedef	struct St_GrDvrFs4MngMsgDiskRecSizeGet
{
	St_GrDvrFs4MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u64*	PtrRtTotal;
	__u64*	PtrRtRec;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs4MngMsgDiskRecSizeGet;

typedef	struct St_GrDvrFs4MngMsgGroupStatGet
{
	St_GrDvrFs4MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs4MngMsgGroupStatGet;

typedef	struct St_GrDvrFs4MngMsgGroupRecSizeGet
{
	St_GrDvrFs4MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u64*	PtrRtTotal;
	__u64*	PtrRtRec;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs4MngMsgGroupRecSizeGet;

typedef	struct St_GrDvrFs4MngMsgGroupDayMapGet
{
	St_GrDvrFs4MngMsgBase	Base;
	__u32*	PtrRslt;
	Def_GrTime TimeSt;
	__u8	DayCnt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
}	*Ptr_GrDvrFs4MngMsgGroupDayMapGet;

typedef struct St_GrDvrFs4MngMsgGroupMinMapGet
{
	St_GrDvrFs4MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	Def_GrTime	TimeSt;
	__u32	ItemCnt;
	Ptr_GrDvrRecMinMap	PtrBuf;

}	*Ptr_GrDvrFs4MngMsgGroupMinMapGet;


typedef struct St_GrDvrFs4MngMsgReadPara
{
	St_GrDvrFs4MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrDvrFs4SeekCtrl	PtrCtrl;

}	*Ptr_GrDvrFs4MngMsgReadPara;

typedef struct St_GrDvrFs4MngMsgReadTimePara
{
	St_GrDvrFs4MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrDvrFs4SeekCtrl	PtrCtrl;
	Def_GrTime	Time;
}	*Ptr_GrDvrFs4MngMsgReadTimePara;

typedef struct St_GrDvrFs4MngMsgLogAdd
{
	St_GrDvrFs4MngMsgBase	Base;
	St_GrDvrLogItm	Log;
}	*Ptr_GrDvrFs4MngMsgLogAdd;

typedef struct St_GrDvrFs4MngMsgLogFind
{
	St_GrDvrFs4MngMsgBase	Base;
	Ptr_GrDvrLogCtrl	PtrCtrl;
}	*Ptr_GrDvrFs4MngMsgLogFind;

typedef struct St_GrDvrFs4MngMsgSysInfo
{
	St_GrDvrFs4MngMsgBase	Base;
	Ptr_GrDvrFs4MngSysInfo	PtrInfo;
}	*Ptr_GrDvrFs4MngMsgSysInfo;

typedef struct St_GrDvrFs4MngMsgGroupTimeRngGet
{
	St_GrDvrFs4MngMsgBase	Base;
	__u8	Group;
	Def_GrTime*	PtrRtTimeSt;
	Def_GrTime*	PtrRtTimeEd;
}	*Ptr_GrDvrFs4MngMsgGroupTimeRngGet;

typedef struct St_GrDvrFs4MngMsgBkupSizeEstm
{
	St_GrDvrFs4MngMsgBase		Base;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	ChMap;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	BOOL8*	PtrRslt;
}	*Ptr_GrDvrFs4MngMsgBkupSizeEstm;

typedef struct St_GrDvrFs4MngMsgAtDelTimeSet
{
	St_GrDvrFs4MngMsgBase		Base;
	__u32	Sec;
}	*Ptr_GrDvrFs4MngMsgAtDelTimeSet;

typedef struct St_GrDvrFs4MngMsgGroupFormat
{
	St_GrDvrFs4MngMsgBase		Base;
	__u8	GroupType;
	BOOL8 IsWithLog;
}	*Ptr_GrDvrFs4MngMsgGroupFormat;

//====================================================================
//class

class	Cls_GrDvrFs4Mng	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32	m_MsgAloc;
		__u32	m_HndlTimer;

		// 
		BOOL8	m_IsInited;

		Ptr_GrBufTsMng	m_PtrBufStrm;
		__u8	m_BufOwnerMap;
		__u8	m_BufThrdIdx;

		__u64	m_DvrId;

		St_GrDvrFs4RecReqInfo	m_RecReqTbl[E_GrDvrFs4MngRecReqSlotCnt];
		__u8	m_RecReqIdx;
		__u8	m_RecReqCnt;

		__u8	m_GrupNrmMap;
		BOOL8	m_IsOvwtNrm;
		__u8	m_GrupMirMap;
		BOOL8	m_IsOvwtMir;

		__u32	m_AtDelTime;
		Def_GrTime	m_AtDelBaseTm;

		Cls_GrDvrFs4Disk*	m_DiskTbl[E_GrDvrFs4MaxDriveCnt];
		Cls_GrDvrFs4Grp*	m_GrupTbl[E_GrDvrFs4MngGroupCnt];		// group object

		__u8	m_TmpBuf[E_GrDvrFs4MngTmpBufSize];
		__u32	m_TmpUsed;

		__u32	m_TempTimer;
				
		__u8	m_TempTbl[E_GrDvrFs4MaxDriveCnt];

		St_GrDvrFs4BkupSizeEstm	m_BkupSizeEstm;

	protected:
		//local
		void	LcGroupBuild( void );

		void	LcTempUpdt( void );

		BOOL8	LcBgpBkupSizeEstm( void );
		void	LcAtDelLimUpdt( void );
		BOOL8	LcRecReqNext(void);
		BOOL8	LcRecReqFlush(__u8 A_SlotIdx);

		//event
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	void	RtlTimer( __u32 A_Hndl );
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */

	public:
		Cls_GrDvrFs4Mng(Cls_GrTaskSvr* A_TaskSvr, __u64 A_DvrId, Ptr_GrBufTsMng A_PtrStrmBuf, __u8 A_BufThrdIdx, __u8 A_BufOwnerIdx);
    virtual		~Cls_GrDvrFs4Mng();

		// status
		__u8	TempratureGet( __u8 A_DskIdx );
		__u32	AutoDelTimeGet( void );

		// thread depended status
		__u8	ImmDiskGetStat( __u8 A_DevIdx );
		BOOL8	ImmDiskGetRecSize( __u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u8	ImmGroupGetStat( __u8 A_GroupType );
		BOOL8	ImmGroupGetRecSize( __u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u32	ImmGroupGetDayMap( __u8 A_GroupType, Def_GrTime A_Time, __u8 A_DayCnt );
		BOOL8	ImmGroupGetMinMap(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrRtBuf);
		void	ImmGroupGetRecTimeRng( __u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd );
		void	ImmSysGetInfo( Ptr_GrDvrFs4MngSysInfo A_PtrRtInfo );
		BOOL8	ImmIsGroupRecAble( __u8 A_GroupType );
		BOOL8	ImmBkupSizeEstmate(__u8 A_GroupType, __u32 A_ChMap,Def_GrTime A_TimeSt, Def_GrTime A_TimeEd);

		__u8	SafeDiskGetStat( __u8 A_DevIdx );
		BOOL8	SafeDiskGetRecSize(__u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec);
		__u8	SafeGroupGetStat(__u8 A_GroupType);
		BOOL8	SafeGroupGetRecSize(__u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec);
		__u32	SafeGroupGetDayMap(__u8 A_GroupType, Def_GrTime A_Time, __u8 A_DayCnt);
		BOOL8	SafeGroupGetMinMap(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrBuf);	// get record thumb table
		void	SafeGroupGetRecTimeRng(__u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd);
		void	SafeSysGetInfo(Ptr_GrDvrFs4MngSysInfo A_PtrRtInfo);
		BOOL8	SafeBkupSizeEstmate(__u8 A_GroupType, __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd);

		// control
		void	ImmInit( void );
		void	ImmSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir, BOOL8 A_IsAutoFix );
		BOOL8	ImmFormat( __u8 A_DevIdx, BOOL8 A_IsWithLog );
		void	ImmAtDelTimeSet( __u32 A_Sec );
		void	ImmGroupFormat(__u8 A_GroupType, BOOL8 A_IsWithLog);

		void	SafeInit( void );	// try initialize
		void	SafeSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir, BOOL8 A_IsAutoFix );
		BOOL8	SafeFormat( __u8 A_DevIdx, BOOL8 A_IsWithLog );		// format request
		void	SafeAtDelTimeSet( __u32 A_Sec );
		void	SafeGroupFormat(__u8 A_GroupType, BOOL8 A_IsWithLog);

		// write
		BOOL8	VdoAdd(void* A_PtrStrm, __u32 A_StrmSize, Def_GrTime A_TimeSec, __u32 A_TimeMili, BOOL8 A_IsSm, 
			__u8 A_Ch, __u8 A_Type, 
			__u8 A_Ratb,__u32 A_VdoFmt, __u8 A_SeqId, __u8 A_SigFps, __u8 A_SkipCnt, __u8 A_RefType, __u32 A_WtMark);

		BOOL8	AdoAdd(void* A_PtrStrm, __u32 A_StrmSize, Def_GrTime A_TimeSec, __u32 A_TimeMili, BOOL8 A_IsSm,
			__u8 A_Ch, __u8 A_Type,
			__u8 A_Ratb );

		// read
		__u8	ImmReadFw( __u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl );
		__u8	ImmReadBw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl);
		__u8	ImmReadTimeFw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, Def_GrTime A_Time);
		__u8	ImmReadTimeBw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, Def_GrTime A_Time);
		__u8	ImmReadBegin(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl);

		__u8	SafeReadFw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl);
		__u8	SafeReadBw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl);
		__u8	SafeReadTimeFw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, Def_GrTime A_Time);
		__u8	SafeReadTimeBw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, Def_GrTime A_Time);
		__u8	SafeReadBegin(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl);

		


		// event log
		void	ImmLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		void	ImmLogFindStart( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	ImmLogFindNext( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	ImmLogFindPrev( Ptr_GrDvrLogCtrl A_PtrCtrl );

		void	SafeLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		void	SafeLogFindStart( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	SafeLogFindNext( Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	SafeLogFindPrev( Ptr_GrDvrLogCtrl A_PtrCtrl );


		// runtime
		Cls_GrDvrFs4Disk*	RtlDiskObjGet( __u8 A_Idx );
		__u64	RtlDvrIdGet(void);
		void*	RtlTmpBufAlloc(__u32 A_Size);
		void	RtlTmpBufFree(__u32 A_Size);

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


