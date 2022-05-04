/*
	GAUSE DVR file system v3 manager

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrFs5IoRaid
#define	_EPC_GrFs5IoRaid

//====================================================================
// option

//#define	E_GrFs5IoRaidChkLogSeek

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>
#include <GrTaskSvr.h>
#include <GrTaskCli.h>
#include <GrTimeTool.h>
#include <GrError.h>

#include <Dvr/Fs/V5/GrFs5Base.h>
#include <Dvr/Fs/V5/GrFs5Hdd.h>
#include <Dvr/Fs/V5/GrFs5Rgrp.h>
	
//====================================================================
//constance

#define E_GrFs5IoRaidGroupCnt									E_GrFs5RgrpMaxCnt

#define E_GrFs5IoRaidTimerPeriod							100
#define	E_GrFs5IoRaidRecFlushPeriod						100
#define E_GrFs5IoRaidSmartScanPeriod					300

#define	E_GrFs5IoRaidFormatPrgsUnit						64

#define	E_GrFs5IoRaidAutoDelPeriod						(E_GrTimeSecOfHour * 60)

// run mode
#define	E_GrFs5IoRaidModeNotReady							0
#define	E_GrFs5IoRaidModeNormal								1
#define	E_GrFs5IoRaidModeFormat								2

// log find
#define	E_GrFs5IoRaidLogGetCtlMaxCnt					8

// message command
#define E_GrFs5IoRaidMsgInit									1		/* initialize */
#define E_GrFs5IoRaidMsgFinish								2		/* initialize */
#define E_GrFs5IoRaidMsgSetup									3		/* St_GrFs5IoRaidMsgSetup */
#define E_GrFs5IoRaidMsgFormat								4		/* St_GrFs5IoRaidMsgFormat */
//#define E_GrFs5IoRaidMsgDiskStatGet					5		/* St_GrFs5IoRaidMsgDiskStatGet */
#define E_GrFs5IoRaidMsgDiskRecInfoGet				6		/* St_GrFs5IoRaidMsgDiskRecInfoGet */
#define E_GrFs5IoRaidMsgGroupStatGet					7		/* St_GrFs5IoRaidMsgGroupStatGet */
#define E_GrFs5IoRaidMsgGroupRecInfoGet				8		/* St_GrFs5IoRaidMsgGroupRecInfoGet */
#define E_GrFs5IoRaidMsgGroupDayMapGet				9		/* St_GrFs5IoRaidMsgGroupDayMapGet */
#define E_GrFs5IoRaidMsgGroupMinRatbTblGet		10	/* St_GrFs5IoRaidMsgGroupMinRatbTblGet */
#define E_GrFs5IoRaidMsgReadFwNext						11	/* St_GrFs5IoRaidMsgReadPara */
#define E_GrFs5IoRaidMsgReadBwNext						12	/* St_GrFs5IoRaidMsgReadPara */
#define E_GrFs5IoRaidMsgReadFwStart						13	/* St_GrFs5IoRaidMsgReadPara */
#define E_GrFs5IoRaidMsgRecBufAdd							14	/* St_GrFs5IoRaidMsgRecBufAdd */
#define E_GrFs5IoRaidMsgLogAdd								15	/* St_GrFs5IoRaidMsgLogAdd */
#define E_GrFs5IoRaidMsgLogRead								16	/* St_GrFs5IoRaidMsgLogRead			- not use */	
#define E_GrFs5IoRaidMsgReadFwByHndl					17	/* St_GrFs5IoRaidMsgReadHndlPara */
//#define E_GrFs5IoRaidMsgSysInfo							18	/* St_GrFs5IoRaidMsgSysInfo */
#define E_GrFs5IoRaidMsgGroupTimeRngGet				19	/* St_GrFs5IoRaidMsgGroupTimeRngGet */
#define E_GrFs5IoRaidMsgRecSizeEstm						20	/* St_GrFs5IoRaidMsgRecSizeEstm */
#define E_GrFs5IoRaidMsgAtDelTimeSet					21	/* St_GrFs5IoRaidMsgAtDelTimeSet */
#define E_GrFs5IoRaidMsgGroupFormat						22	/* St_GrFs5IoRaidMsgGroupFormat */
#define E_GrFs5IoRaidMsgReadFwByTime					23	/* St_GrFs5IoRaidMsgReadTimePara */
#define E_GrFs5IoRaidMsgReadBwByTime					24	/* St_GrFs5IoRaidMsgReadTimePara */
#define	E_GrFs5IoRaidMsgDiskModelGet					25	/* St_GrFs5IoRaidMsgDiskModelGet */
#define	E_GrFs5IoRaidMsgLogGetOpen						26	/* St_GrFs5IoRaidMsgLogGetOpen */
#define	E_GrFs5IoRaidMsgLogGetChkDone					27	/* St_GrFs5IoRaidMsgLogGetChkDone */
#define	E_GrFs5IoRaidMsgLogGetClose						28	/* St_GrFs5IoRaidMsgLogGetClose */
#define	E_GrFs5IoRaidMsgLogGetFrameModeCheck	29	/* St_GrFs5IoRaidMsgLogGetPartCheck */
#define	E_GrFs5IoRaidMsgLogGetFrameModeResume	30	/* St_GrFs5IoRaidMsgLogGetPartResume */
#define	E_GrFs5IoRaidMsgDiskInstMapGet				31	/* St_GrFs5IoRaidMsgDiskInstMapGet */
#define	E_GrFs5IoRaidMsgDiskSmartRead					32	/* St_GrFs5IoRaidMsgDiskInstMapGet */
#define	E_GrFs5IoRaidMsgGroupMakeFull					33	/* St_GrFs5IoRaidMsgGroupMakeFull */
#define	E_GrFs5IoRaidMsgSctlClose							34	/* St_GrFs5IoRaidMsgSctlClose */


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrFs5IoRaidFormatCtrl
{
	__u8	GroupMap;		// bit 0 = normal , bit 1 = mirror
	__u8	DskMap;
	__u8	DskPrgs;
	__u8	DskCnt;

	__u8	DskIdx;		// by map

	__u32	BlkCnt;
	__u32	BlkPrgs;
	__u32	TotPrgs;
}	*Ptr_GrFs5IoRaidFormatCtrl;

typedef struct St_GrFs5IoRaidGrupCtrl
{
	__u8	DskMap;
	BOOL8	IsOvwt;
	__u8	Stat;
	BOOL8	IsCchRsvAble;

	Cls_GrFs5Rgrp*	Obj;
	__u64	SizeTot;
	__u64	SizeRec;
}	*Ptr_GrFs5IoRaidGrupCtrl;

// message
typedef	struct St_GrFs5IoRaidMsgBase 
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrFs5IoRaidMsgBase;

typedef	struct St_GrFs5IoRaidMsgSetup
{
	St_GrFs5IoRaidMsgBase	Base;
	__u8	NrmMap;
	BOOL8	IsNrmOvwt;
	__u8	MirMap;
	BOOL8	IsMirOvwt;

}	*Ptr_GrFs5IoRaidMsgSetup;

typedef	struct St_GrFs5IoRaidMsgFormat
{
	St_GrFs5IoRaidMsgBase	Base;
	Def_GrErrCode*	PtrRslt;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFs5IoRaidMsgFormat;

typedef	struct St_GrFs5IoRaidMsgDiskRecInfoGet
{
	St_GrFs5IoRaidMsgBase	Base;
	__u8*	PtrRslt;
	__u64*	PtrRtTotal;
	__u64*	PtrRtRec;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFs5IoRaidMsgDiskRecInfoGet;

typedef	struct St_GrFs5IoRaidMsgGroupStatGet
{
	St_GrFs5IoRaidMsgBase	Base;
	__u8*	PtrRslt;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFs5IoRaidMsgGroupStatGet;

typedef	struct St_GrFs5IoRaidMsgGroupRecInfoGet
{
	St_GrFs5IoRaidMsgBase	Base;
	__u8*	PtrRslt;
	__u64*	PtrRtTotal;
	__u64*	PtrRtRec;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFs5IoRaidMsgGroupRecInfoGet;

typedef	struct St_GrFs5IoRaidMsgGroupDayMapGet
{
	St_GrFs5IoRaidMsgBase	Base;
	__u32*	PtrRslt;
	Def_GrTime TimeMonth;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrFs5IoRaidMsgGroupDayMapGet;

typedef struct St_GrFs5IoRaidMsgGroupMinRatbTblGet
{
	St_GrFs5IoRaidMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	Def_GrTime	TimeSt;
	__u32	ItemCnt;
	__u32	UserData;
	Ptr_GrDvrRecMinMap	PtrBuf;

}	*Ptr_GrFs5IoRaidMsgGroupMinRatbTblGet;


typedef struct St_GrFs5IoRaidMsgReadPara
{
	St_GrFs5IoRaidMsgBase	Base;
	Def_GrErrCode*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrFs5SeekCtrl	PtrCtrl;

}	*Ptr_GrFs5IoRaidMsgReadPara;

typedef struct St_GrFs5IoRaidMsgReadTimePara
{
	St_GrFs5IoRaidMsgBase	Base;
	Def_GrErrCode*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrFs5SeekCtrl	PtrCtrl;
	Def_GrTime	Time;
}	*Ptr_GrFs5IoRaidMsgReadTimePara;

typedef struct St_GrFs5IoRaidMsgReadHndlPara
{
	St_GrFs5IoRaidMsgBase	Base;
	Def_GrErrCode*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrFs5SeekCtrl	PtrCtrl;
	__u64	Hndl;
}	*Ptr_GrFs5IoRaidMsgReadHndlPara;

typedef struct St_GrFs5IoRaidMsgLogAdd
{
	St_GrFs5IoRaidMsgBase	Base;
	St_GrDvrLogItm	Log;
}	*Ptr_GrFs5IoRaidMsgLogAdd;

typedef struct St_GrFs5IoRaidMsgGroupTimeRngGet
{
	St_GrFs5IoRaidMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	Group;
	Def_GrTime*	PtrRtTimeSt;
	Def_GrTime*	PtrRtTimeEd;
	Def_GrTime*	PtrRtTimeLast;
}	*Ptr_GrFs5IoRaidMsgGroupTimeRngGet;

typedef struct St_GrFs5IoRaidMsgRecSizeEstm
{
	St_GrFs5IoRaidMsgBase		Base;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	ChMap;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	__u64*	PtrRslt;
}	*Ptr_GrFs5IoRaidMsgRecSizeEstm;

typedef struct St_GrFs5IoRaidMsgAtDelTimeSet
{
	St_GrFs5IoRaidMsgBase		Base;
	__u32	Sec;
}	*Ptr_GrFs5IoRaidMsgAtDelTimeSet;

typedef struct St_GrFs5IoRaidMsgGroupFormat
{
	St_GrFs5IoRaidMsgBase	Base;
	Def_GrErrCode*	PtrRslt;
	__u8	GroupMap;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFs5IoRaidMsgGroupFormat;

typedef struct St_GrFs5IoRaidMsgDiskModelGet
{
	St_GrFs5IoRaidMsgBase		Base;
	BOOL8*	PtrRslt;
	__u8		DevIdx;
	WCHAR*	StrRt;
}	*Ptr_GrFs5IoRaidMsgDiskModelGet;

typedef struct St_GrFs5IoRaidMsgRecBufAdd
{
	St_GrFs5IoRaidMsgBase		Base;
	iovec*	PtrIovs;
	__u32		Cnt;
}	*Ptr_GrFs5IoRaidMsgRecBufAdd;

typedef struct St_GrFs5IoRaidMsgLogGetOpen
{
	St_GrFs5IoRaidMsgBase		Base;
	__s32*			PtrRslt;
	__u8				Group;
	BOOL8				IsFrmMode;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	__u32				MaxCnt;
	__u32				ChMask;
	__u64				TypeMask;
	Ptr_GrDvrLogItm	PtrRtBuf;
}	*Ptr_GrFs5IoRaidMsgLogGetOpen;

typedef struct St_GrFs5IoRaidMsgLogGetChkDone
{
	St_GrFs5IoRaidMsgBase		Base;
	BOOL8*	PtrRslt;
	__s32		Hndl;
	__u32*	PtrRtCnt;
}	*Ptr_GrFs5IoRaidMsgLogGetChkDone;

typedef struct St_GrFs5IoRaidMsgLogGetClose
{
	St_GrFs5IoRaidMsgBase		Base;
	__s32		Hndl;

}	*Ptr_GrFs5IoRaidMsgLogGetClose;

typedef struct St_GrFs5IoRaidMsgLogGetPartCheck
{
	St_GrFs5IoRaidMsgBase		Base;
	Def_GrErrCode*	PtrRslt;
	__s32		Hndl;
	__u32*	PtrRtCnt;
}	*Ptr_GrFs5IoRaidMsgLogGetPartCheck;

typedef struct St_GrFs5IoRaidMsgLogGetPartResume
{
	St_GrFs5IoRaidMsgBase		Base;
	BOOL8*	PtrRslt;
	__s32		Hndl;
}	*Ptr_GrFs5IoRaidMsgLogGetPartResume;

typedef struct St_GrFs5IoRaidMsgDiskInstMapGet
{
	St_GrFs5IoRaidMsgBase		Base;
	__u8*	PtrRslt;
	__u8*	PtrRtBadMap;
}	*Ptr_GrFs5IoRaidMsgDiskInstMapGet;

typedef struct St_GrFs5IoRaidMsgDiskSmartRead
{
	St_GrFs5IoRaidMsgBase		Base;
	__u8*	PtrRslt;
	__u8	DevIdx;
	Ptr_GrFs5Smart	PtrRtData;
}	*Ptr_GrFs5IoRaidMsgDiskSmartRead;

typedef struct St_GrFs5IoRaidMsgGroupMakeFull
{
	St_GrFs5IoRaidMsgBase		Base;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFs5IoRaidMsgGroupMakeFull;

typedef struct St_GrFs5IoRaidMsgSctlClose
{
	St_GrFs5IoRaidMsgBase		Base;
	Def_GrErrCode*	PtrRslt;
	__u8	SubType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrFs5SeekCtrl	PtrCtrl;
}	*Ptr_GrFs5IoRaidMsgSctlClose;


//====================================================================
//class

class	Cls_GrFs5IoRaid	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;

		__u32	m_MsgMng;
		__u32	m_HndlTimer;

		// 
		__u8	m_RunMode;

		// format mode
		St_GrFs5IoRaidFormatCtrl	m_Format;

		// group control
		St_GrFs5IoRaidGrupCtrl	m_GrupTbl[E_GrFs5IoRaidGroupCnt];

		// flush
		__u32	m_RflushTimer;

		// smart
		__u32	m_SmartTimer;
		St_GrFs5Smart	m_SmartTbl[E_GrFs5MaxDiskCnt];

		// auto delete
		__u32	m_AtDelTimer;			// auto delete period timer
		__u32	m_AtDelStep;			// auto delete step , sec unit, must be big of day
		Def_GrTime	m_AtDelBaseTm;

		// log
		__u32		m_LogGetCidx;			// now work control index
		St_GrFs5IoLogGetRtl		m_LogGetCtls[E_GrFs5IoRaidLogGetCtlMaxCnt];
		St_GrFs5FrmLogPool		m_LogGetFrmBuf;

#ifdef E_GrFs5IoRaidChkLogSeek
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
		Cls_GrFs5IoRaid(Cls_GrTaskSvr* A_TaskSvr, __u32 A_CchMinCnt, __u32 A_CchDataCnt);
    virtual		~Cls_GrFs5IoRaid();

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
		BOOL8	ImmDiskSmartRead(__u8 A_DevIdx, Ptr_GrFs5Smart A_PtrRtData);

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
		BOOL8	ReqDiskSmartRead(__u8 A_DevIdx, Ptr_GrFs5Smart A_PtrRtData);

		void	ReqGroupMakeFull(__u8 A_GroupType);

		// write
		void	ImmRecBufAdd(iovec* A_PtrIovs, __u32 A_Cnt);

		void	ReqRecBufAdd(iovec* A_PtrIovs, __u32 A_Cnt);

		// io base
		Def_GrErrCode	ImmSctlClose(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl);

		Def_GrErrCode	ImmReadFwNext(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl);
		Def_GrErrCode	ImmReadBwNext(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl);
		Def_GrErrCode	ImmReadFwByTime(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl, Def_GrTime A_Time);
		Def_GrErrCode	ImmReadBwByTime(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl, Def_GrTime A_Time);

		Def_GrErrCode	ImmReadFwByHndl(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl, __u64 A_Hndl);
		Def_GrErrCode	ImmReadFwStart(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl);

		Def_GrErrCode	ReqSctlClose(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl);

		Def_GrErrCode	ReqReadFwNext(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl);
		Def_GrErrCode	ReqReadBwNext(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl);
		Def_GrErrCode	ReqReadFwByTime(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl, Def_GrTime A_Time);
		Def_GrErrCode	ReqReadBwByTime(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl, Def_GrTime A_Time);

		Def_GrErrCode	ReqReadFwStart(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl);
		Def_GrErrCode	ReqReadFwByHndl(__u8 A_SubType, Ptr_GrFs5SeekCtrl A_PtrCtrl, __u64 A_Hndl);

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


