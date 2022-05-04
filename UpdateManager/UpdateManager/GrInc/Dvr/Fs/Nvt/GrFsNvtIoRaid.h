/*
	GAUSE DVR file system v3 manager

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrFsNvtIoRaid
#define	_EPC_GrFsNvtIoRaid

//====================================================================
// option

//#define	E_GrFsNvtIoRaidChkLogSeek

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>
#include <GrTaskSvr.h>
#include <GrTaskCli.h>
#include <GrTimeTool.h>
#include <GrError.h>

#include <Dvr/Fs/Nvt/GrFsNvtBase.h>
#include <Dvr/Fs/Nvt/GrFsNvtHdd.h>
#include <Dvr/Fs/Nvt/GrFsNvtRgrp.h>
	
//====================================================================
//constance

#define E_GrFsNvtIoRaidGroupCnt									E_GrFsNvtRgrpMaxCnt

#define E_GrFsNvtIoRaidTimerPeriod							100
#define	E_GrFsNvtIoRaidRecFlushPeriod						100
#define E_GrFsNvtIoRaidSmartScanPeriod					300

#define	E_GrFsNvtIoRaidFormatPrgsUnit						64

#define	E_GrFsNvtIoRaidAutoDelPeriod						(E_GrTimeSecOfHour * 60)

// run mode
#define	E_GrFsNvtIoRaidModeNotReady							0
#define	E_GrFsNvtIoRaidModeNormal								1
#define	E_GrFsNvtIoRaidModeFormat								2

// log find
#define	E_GrFsNvtIoRaidLogGetCtlMaxCnt					8

// message command
#define E_GrFsNvtIoRaidMsgInit									1		/* initialize */
#define E_GrFsNvtIoRaidMsgFinish								2		/* initialize */
#define E_GrFsNvtIoRaidMsgSetup									3		/* St_GrFsNvtIoRaidMsgSetup */
#define E_GrFsNvtIoRaidMsgFormat								4		/* St_GrFsNvtIoRaidMsgFormat */
//#define E_GrFsNvtIoRaidMsgDiskStatGet					5		/* St_GrFsNvtIoRaidMsgDiskStatGet */
#define E_GrFsNvtIoRaidMsgDiskRecInfoGet				6		/* St_GrFsNvtIoRaidMsgDiskRecInfoGet */
#define E_GrFsNvtIoRaidMsgGroupStatGet					7		/* St_GrFsNvtIoRaidMsgGroupStatGet */
#define E_GrFsNvtIoRaidMsgGroupRecInfoGet				8		/* St_GrFsNvtIoRaidMsgGroupRecInfoGet */
#define E_GrFsNvtIoRaidMsgGroupDayMapGet				9		/* St_GrFsNvtIoRaidMsgGroupDayMapGet */
#define E_GrFsNvtIoRaidMsgGroupMinRatbTblGet		10	/* St_GrFsNvtIoRaidMsgGroupMinRatbTblGet */
#define E_GrFsNvtIoRaidMsgReadFwNext						11	/* St_GrFsNvtIoRaidMsgReadPara */
#define E_GrFsNvtIoRaidMsgReadBwNext						12	/* St_GrFsNvtIoRaidMsgReadPara */
#define E_GrFsNvtIoRaidMsgReadFwStart						13	/* St_GrFsNvtIoRaidMsgReadPara */
#define E_GrFsNvtIoRaidMsgRecBufAdd							14	/* St_GrFsNvtIoRaidMsgRecBufAdd */
#define E_GrFsNvtIoRaidMsgLogAdd								15	/* St_GrFsNvtIoRaidMsgLogAdd */
#define E_GrFsNvtIoRaidMsgLogRead								16	/* St_GrFsNvtIoRaidMsgLogRead			- not use */	
#define E_GrFsNvtIoRaidMsgReadFwByHndl					17	/* St_GrFsNvtIoRaidMsgReadHndlPara */
//#define E_GrFsNvtIoRaidMsgSysInfo							18	/* St_GrFsNvtIoRaidMsgSysInfo */
#define E_GrFsNvtIoRaidMsgGroupTimeRngGet				19	/* St_GrFsNvtIoRaidMsgGroupTimeRngGet */
#define E_GrFsNvtIoRaidMsgRecSizeEstm						20	/* St_GrFsNvtIoRaidMsgRecSizeEstm */
#define E_GrFsNvtIoRaidMsgAtDelTimeSet					21	/* St_GrFsNvtIoRaidMsgAtDelTimeSet */
#define E_GrFsNvtIoRaidMsgGroupFormat						22	/* St_GrFsNvtIoRaidMsgGroupFormat */
#define E_GrFsNvtIoRaidMsgReadFwByTime					23	/* St_GrFsNvtIoRaidMsgReadTimePara */
#define E_GrFsNvtIoRaidMsgReadBwByTime					24	/* St_GrFsNvtIoRaidMsgReadTimePara */
#define	E_GrFsNvtIoRaidMsgDiskModelGet					25	/* St_GrFsNvtIoRaidMsgDiskModelGet */
#define	E_GrFsNvtIoRaidMsgLogGetOpen						26	/* St_GrFsNvtIoRaidMsgLogGetOpen */
#define	E_GrFsNvtIoRaidMsgLogGetChkDone					27	/* St_GrFsNvtIoRaidMsgLogGetChkDone */
#define	E_GrFsNvtIoRaidMsgLogGetClose						28	/* St_GrFsNvtIoRaidMsgLogGetClose */
#define	E_GrFsNvtIoRaidMsgLogGetFrameModeCheck	29	/* St_GrFsNvtIoRaidMsgLogGetPartCheck */
#define	E_GrFsNvtIoRaidMsgLogGetFrameModeResume	30	/* St_GrFsNvtIoRaidMsgLogGetPartResume */
#define	E_GrFsNvtIoRaidMsgDiskInstMapGet				31	/* St_GrFsNvtIoRaidMsgDiskInstMapGet */
#define	E_GrFsNvtIoRaidMsgDiskSmartRead					32	/* St_GrFsNvtIoRaidMsgDiskInstMapGet */
#define	E_GrFsNvtIoRaidMsgGroupMakeFull					33	/* St_GrFsNvtIoRaidMsgGroupMakeFull */
#define	E_GrFsNvtIoRaidMsgSctlClose							34	/* St_GrFsNvtIoRaidMsgSctlClose */


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrFsNvtIoRaidFormatCtrl
{
	__u8	GroupMap;		// bit 0 = normal , bit 1 = mirror
	__u8	DskMap;
	__u8	DskPrgs;
	__u8	DskCnt;

	__u8	DskIdx;		// by map

	__u32	BlkCnt;
	__u32	BlkPrgs;
	__u32	TotPrgs;
}	*Ptr_GrFsNvtIoRaidFormatCtrl;

typedef struct St_GrFsNvtIoRaidGrupCtrl
{
	__u8	DskMap;
	BOOL8	IsOvwt;
	__u8	Stat;
	BOOL8	IsCchRsvAble;

	Cls_GrFsNvtRgrp*	Obj;
	__u64	SizeTot;
	__u64	SizeRec;
}	*Ptr_GrFsNvtIoRaidGrupCtrl;

// message
typedef	struct St_GrFsNvtIoRaidMsgBase 
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrFsNvtIoRaidMsgBase;

typedef	struct St_GrFsNvtIoRaidMsgSetup
{
	St_GrFsNvtIoRaidMsgBase	Base;
	__u8	NrmMap;
	BOOL8	IsNrmOvwt;
	__u8	MirMap;
	BOOL8	IsMirOvwt;

}	*Ptr_GrFsNvtIoRaidMsgSetup;

typedef	struct St_GrFsNvtIoRaidMsgFormat
{
	St_GrFsNvtIoRaidMsgBase	Base;
	Def_GrErrCode*	PtrRslt;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFsNvtIoRaidMsgFormat;

typedef	struct St_GrFsNvtIoRaidMsgDiskRecInfoGet
{
	St_GrFsNvtIoRaidMsgBase	Base;
	__u8*	PtrRslt;
	__u64*	PtrRtTotal;
	__u64*	PtrRtRec;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFsNvtIoRaidMsgDiskRecInfoGet;

typedef	struct St_GrFsNvtIoRaidMsgGroupStatGet
{
	St_GrFsNvtIoRaidMsgBase	Base;
	__u8*	PtrRslt;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFsNvtIoRaidMsgGroupStatGet;

typedef	struct St_GrFsNvtIoRaidMsgGroupRecInfoGet
{
	St_GrFsNvtIoRaidMsgBase	Base;
	__u8*	PtrRslt;
	__u64*	PtrRtTotal;
	__u64*	PtrRtRec;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFsNvtIoRaidMsgGroupRecInfoGet;

typedef	struct St_GrFsNvtIoRaidMsgGroupDayMapGet
{
	St_GrFsNvtIoRaidMsgBase	Base;
	__u32*	PtrRslt;
	Def_GrTime TimeMonth;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrFsNvtIoRaidMsgGroupDayMapGet;

typedef struct St_GrFsNvtIoRaidMsgGroupMinRatbTblGet
{
	St_GrFsNvtIoRaidMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	Def_GrTime	TimeSt;
	__u32	ItemCnt;
	__u32	UserData;
	Ptr_GrDvrRecMinMap	PtrBuf;

}	*Ptr_GrFsNvtIoRaidMsgGroupMinRatbTblGet;


typedef struct St_GrFsNvtIoRaidMsgReadPara
{
	St_GrFsNvtIoRaidMsgBase	Base;
	Def_GrErrCode*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrFsNvtSeekCtrl	PtrCtrl;

}	*Ptr_GrFsNvtIoRaidMsgReadPara;

typedef struct St_GrFsNvtIoRaidMsgReadTimePara
{
	St_GrFsNvtIoRaidMsgBase	Base;
	Def_GrErrCode*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrFsNvtSeekCtrl	PtrCtrl;
	Def_GrTime	Time;
}	*Ptr_GrFsNvtIoRaidMsgReadTimePara;

typedef struct St_GrFsNvtIoRaidMsgReadHndlPara
{
	St_GrFsNvtIoRaidMsgBase	Base;
	Def_GrErrCode*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrFsNvtSeekCtrl	PtrCtrl;
	__u64	Hndl;
}	*Ptr_GrFsNvtIoRaidMsgReadHndlPara;

typedef struct St_GrFsNvtIoRaidMsgLogAdd
{
	St_GrFsNvtIoRaidMsgBase	Base;
	St_GrDvrLogItm	Log;
}	*Ptr_GrFsNvtIoRaidMsgLogAdd;

typedef struct St_GrFsNvtIoRaidMsgGroupTimeRngGet
{
	St_GrFsNvtIoRaidMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	Group;
	Def_GrTime*	PtrRtTimeSt;
	Def_GrTime*	PtrRtTimeEd;
	Def_GrTime*	PtrRtTimeLast;
}	*Ptr_GrFsNvtIoRaidMsgGroupTimeRngGet;

typedef struct St_GrFsNvtIoRaidMsgRecSizeEstm
{
	St_GrFsNvtIoRaidMsgBase		Base;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	ChMap;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	__u64*	PtrRslt;
}	*Ptr_GrFsNvtIoRaidMsgRecSizeEstm;

typedef struct St_GrFsNvtIoRaidMsgAtDelTimeSet
{
	St_GrFsNvtIoRaidMsgBase		Base;
	__u32	Sec;
}	*Ptr_GrFsNvtIoRaidMsgAtDelTimeSet;

typedef struct St_GrFsNvtIoRaidMsgGroupFormat
{
	St_GrFsNvtIoRaidMsgBase	Base;
	Def_GrErrCode*	PtrRslt;
	__u8	GroupMap;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFsNvtIoRaidMsgGroupFormat;

typedef struct St_GrFsNvtIoRaidMsgDiskModelGet
{
	St_GrFsNvtIoRaidMsgBase		Base;
	BOOL8*	PtrRslt;
	__u8		DevIdx;
	WCHAR*	StrRt;
}	*Ptr_GrFsNvtIoRaidMsgDiskModelGet;

typedef struct St_GrFsNvtIoRaidMsgRecBufAdd
{
	St_GrFsNvtIoRaidMsgBase		Base;
	iovec*	PtrIovs;
	__u32		Cnt;
}	*Ptr_GrFsNvtIoRaidMsgRecBufAdd;

typedef struct St_GrFsNvtIoRaidMsgLogGetOpen
{
	St_GrFsNvtIoRaidMsgBase		Base;
	__s32*			PtrRslt;
	__u8				Group;
	BOOL8				IsFrmMode;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	__u32				MaxCnt;
	__u32				ChMask;
	__u64				TypeMask;
	Ptr_GrDvrLogItm	PtrRtBuf;
}	*Ptr_GrFsNvtIoRaidMsgLogGetOpen;

typedef struct St_GrFsNvtIoRaidMsgLogGetChkDone
{
	St_GrFsNvtIoRaidMsgBase		Base;
	BOOL8*	PtrRslt;
	__s32		Hndl;
	__u32*	PtrRtCnt;
}	*Ptr_GrFsNvtIoRaidMsgLogGetChkDone;

typedef struct St_GrFsNvtIoRaidMsgLogGetClose
{
	St_GrFsNvtIoRaidMsgBase		Base;
	__s32		Hndl;

}	*Ptr_GrFsNvtIoRaidMsgLogGetClose;

typedef struct St_GrFsNvtIoRaidMsgLogGetPartCheck
{
	St_GrFsNvtIoRaidMsgBase		Base;
	Def_GrErrCode*	PtrRslt;
	__s32		Hndl;
	__u32*	PtrRtCnt;
}	*Ptr_GrFsNvtIoRaidMsgLogGetPartCheck;

typedef struct St_GrFsNvtIoRaidMsgLogGetPartResume
{
	St_GrFsNvtIoRaidMsgBase		Base;
	BOOL8*	PtrRslt;
	__s32		Hndl;
}	*Ptr_GrFsNvtIoRaidMsgLogGetPartResume;

typedef struct St_GrFsNvtIoRaidMsgDiskInstMapGet
{
	St_GrFsNvtIoRaidMsgBase		Base;
	__u8*	PtrRslt;
	__u8*	PtrRtBadMap;
}	*Ptr_GrFsNvtIoRaidMsgDiskInstMapGet;

typedef struct St_GrFsNvtIoRaidMsgDiskSmartRead
{
	St_GrFsNvtIoRaidMsgBase		Base;
	__u8*	PtrRslt;
	__u8	DevIdx;
	Ptr_GrFsNvtSmart	PtrRtData;
}	*Ptr_GrFsNvtIoRaidMsgDiskSmartRead;

typedef struct St_GrFsNvtIoRaidMsgGroupMakeFull
{
	St_GrFsNvtIoRaidMsgBase		Base;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFsNvtIoRaidMsgGroupMakeFull;

typedef struct St_GrFsNvtIoRaidMsgSctlClose
{
	St_GrFsNvtIoRaidMsgBase		Base;
	Def_GrErrCode*	PtrRslt;
	__u8	SubType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrFsNvtSeekCtrl	PtrCtrl;
}	*Ptr_GrFsNvtIoRaidMsgSctlClose;


//====================================================================
//class

class	Cls_GrFsNvtIoRaid	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;

		__u32	m_MsgMng;
		__u32	m_HndlTimer;

		// 
		__u8	m_RunMode;

		// format mode
		St_GrFsNvtIoRaidFormatCtrl	m_Format;

		// group control
		St_GrFsNvtIoRaidGrupCtrl	m_GrupTbl[E_GrFsNvtIoRaidGroupCnt];

		// flush
		__u32	m_RflushTimer;

		// smart
		__u32	m_SmartTimer;
		St_GrFsNvtSmart	m_SmartTbl[E_GrFsNvtMaxDiskCnt];

		// auto delete
		__u32	m_AtDelTimer;			// auto delete period timer
		__u32	m_AtDelStep;			// auto delete step , sec unit, must be big of day
		Def_GrTime	m_AtDelBaseTm;

		// log
		__u32		m_LogGetCidx;			// now work control index
		St_GrFsNvtIoLogGetRtl		m_LogGetCtls[E_GrFsNvtIoRaidLogGetCtlMaxCnt];
		St_GrFsNvtFrmLogPool		m_LogGetFrmBuf;

#ifdef E_GrFsNvtIoRaidChkLogSeek
		BOOL8	m_DbgIsLogWork;
#endif

	protected:
		//local

		// build disk table
		void	LcGroupMakeByMap(__u8 A_GrpMap);
		void	LcGroupFreeByMap(__u8 A_GrpMap);
		void	LcGroupRemakeByMap(__u8 A_GrpMap);
		void	LcGroupRecFlush(void);

		Def_GrErrCode	LcFormatSetupByDiskmap(__u8 A_GrpMap, __u8 A_DskMap, BOOL8 A_IsPost);
		void	LcFormatFailMsgBradcast(__u8 A_GrpMap, __u8 A_DskMap, Def_GrErrCode A_ErrCode);

		void	LcLogGetSortOutput(__u32 A_Cidx);
		void	LcLogQsort(Ptr_GrDvrLogItm A_PtrBuf, __u32 A_StIdx, __u32 A_EdIdx);

		void	LcTmrRecFlush(void);
		void	LcTmrAutoDelete(void);

		void	LcBkgFormat(void);

		BOOL8	LcBkgLogGet(void);

		void	LcSmartUpdt( void );

		void	LcGroupInfoUpdate(void);

		void	LcLogGetNotifyDone(__u32 A_Idx);
		void	LcLogGetCloseAll(void);

		//event
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	void	RtlTimer( __u32 A_Hndl );
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */

	public:
		Cls_GrFsNvtIoRaid(Cls_GrTaskSvr* A_TaskSvr, __u32 A_CchMinCnt, __u32 A_CchDataCnt);
    virtual		~Cls_GrFsNvtIoRaid();

		// status
		__u8	DiskTemperatureGet( __u8 A_DskIdx );

		// thread depended status
		__u32	ImmGroupGetMonthDayMap(__u8 A_GroupType, Def_GrTime A_TimeMonth);
		BOOL8	ImmGroupGetMinRatbTbl(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrRtBuf);
		BOOL8	ImmGroupGetRecTimeRng(__u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd,Def_GrTime* A_PtrRtTimeLast);
		__u64	ImmRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap,Def_GrTime A_TimeSt, Def_GrTime A_TimeEd);
		void	ImmAtDelTimeSet(__u32 A_Sec);
		__u8	ImmDiskInstMapGet(__u8* A_PtrRtBadMap);

		__u8	ImmGroupGetStat(__u8 A_GroupType);

		__u32	ReqGroupGetMonthDayMap(__u8 A_GroupType, Def_GrTime A_TimeMonth);
		BOOL8	ReqGroupGetMinRatbTbl(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrBuf, BOOL8 A_IsPost = FALSE, __u32 A_UserData = 0);	// get record thumb table
		BOOL8	ReqGroupGetRecTimeRng(__u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd, Def_GrTime* A_PtrRtTimeLast);
		__u64	ReqRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd);
		void	ReqAtDelTimeSet(__u32 A_Sec);
		__u8	ReqDiskInstMapGet(__u8* A_PtrRtBadMap);

		// control
		void	ImmInit( void );
		void	ImmFinish(void);

		void	ImmSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir );
		Def_GrErrCode	ImmDiskFormat(__u8 A_DevIdx, BOOL8 A_IsPost);
		__u8	ImmDiskRecInfoGet(__u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec);
		BOOL8	ImmDiskModelNameGet(__u8 A_DevIdx, WCHAR* A_StrRt);
		__u8	ImmGroupRecInfoGet(__u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec);
		Def_GrErrCode	ImmGroupDiskFormat(__u8 A_GroupMap, BOOL8 A_IsPost);
		BOOL8	ImmDiskSmartRead(__u8 A_DevIdx, Ptr_GrFsNvtSmart A_PtrRtData);

		void	ImmGroupMakeFull(__u8 A_GroupType);

		void	ReqInit( void );	// try initialize
		void	ReqFinish(void);	// try finalize

		void	ReqSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir );
		Def_GrErrCode	ReqDiskFormat(__u8 A_DevIdx, BOOL8 A_IsPost);		// format request
		__u8	ReqDiskRecInfoGet(__u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec);
		__u8	ReqGroupRecInfoGet(__u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec);
		BOOL8	ReqDiskModelNameGet(__u8 A_DevIdx, WCHAR* A_StrRt);
		Def_GrErrCode	ReqGroupDiskFormat(__u8 A_GroupMap, BOOL8 A_IsPost);
		__u8	ReqRecInfoGet(__u8* A_PtrRtGroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec);
		BOOL8	ReqDiskSmartRead(__u8 A_DevIdx, Ptr_GrFsNvtSmart A_PtrRtData);

		void	ReqGroupMakeFull(__u8 A_GroupType);

		// write
		void	ImmRecBufAdd(iovec* A_PtrIovs, __u32 A_Cnt);

		void	ReqRecBufAdd(iovec* A_PtrIovs, __u32 A_Cnt);

		// io base
		Def_GrErrCode	ImmSctlClose(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl);

		Def_GrErrCode	ImmReadFwNext(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl);
		Def_GrErrCode	ImmReadBwNext(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl);
		Def_GrErrCode	ImmReadFwByTime(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time);
		Def_GrErrCode	ImmReadBwByTime(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time);

		Def_GrErrCode	ImmReadFwByHndl(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl);
		Def_GrErrCode	ImmReadFwStart(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl);

		Def_GrErrCode	ReqSctlClose(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl);

		Def_GrErrCode	ReqReadFwNext(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl);
		Def_GrErrCode	ReqReadBwNext(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl);
		Def_GrErrCode	ReqReadFwByTime(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time);
		Def_GrErrCode	ReqReadBwByTime(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time);

		Def_GrErrCode	ReqReadFwStart(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl);
		Def_GrErrCode	ReqReadFwByHndl(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl);

		// event log
		void	ImmLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		__s32	ImmLogGetOpen(__u8 A_Group, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_MaxCnt,
			__u32 A_ChMask, __u64 A_TypeMask, Ptr_GrDvrLogItm A_PtrRtBuf, BOOL8 A_IsFrmMode);
		BOOL8	ImmLogGetChkDone(__s32 A_Hndl, __u32* A_PtrRtCnt);
		Def_GrErrCode	ImmLogGetFrameModeCheck(__s32 A_Hndl, __u32* A_PtrRtCnt);
		BOOL8	ImmLogGetFrameModeResume(__s32 A_Hndl);
		void	ImmLogGetClose(__s32 A_Hndl);

		void	ReqLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		__s32	ReqLogGetOpen(__u8 A_Group, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_MaxCnt,
			__u32 A_ChMask, __u64 A_TypeMask, Ptr_GrDvrLogItm A_PtrRtBuf, BOOL8 A_IsFrmMode);
		BOOL8	ReqLogGetChkDone(__s32 A_Hndl, __u32* A_PtrRtCnt);
		Def_GrErrCode	ReqLogGetFrameModeCheck(__s32 A_Hndl, __u32* A_PtrRtCnt);
		BOOL8	ReqLogGetFrameModeResume(__s32 A_Hndl);
		void	ReqLogGetClose(__s32 A_Hndl);

		// runtime

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


