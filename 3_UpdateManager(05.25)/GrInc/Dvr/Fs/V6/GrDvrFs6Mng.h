/*
	GAUSE DVR file system v3 manager

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs6Mng
#define	_EPC_GrDvrFs6Mng

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>
#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/Fs/V6/GrDvrFs6Base.h>
#include <Dvr/Fs/V6/GrDvrFs6Disk.h>
#include <Dvr/Fs/V6/GrDvrFs6Grp.h>
	
//====================================================================
//constance

#define E_GrDvrFs6MngGroupCnt								2

#define E_GrDvrFs6MngTimerPeriod						1000

#define	E_GrDvrFs6MngRecFlushPeriod					60

#define E_GrDvrFs6MngTempScanPeriod					30

#define E_GrDvrFs6MngTmpBufSize							0x200000		/* 2 MByte */

#define	E_GrDvrFs6MngRecReqSlotCnt					2

#define	E_GrDvrFs6MngBkupEstmWorkUnit				8

// message command
#define E_GrDvrFs6MngMsgInit								1		/* initialize */
#define E_GrDvrFs6MngMsgFinish							2		/* initialize */
#define E_GrDvrFs6MngMsgSetup								3		/* St_GrDvrFs6MngMsgSetup */
#define E_GrDvrFs6MngMsgFormat							4		/* St_GrDvrFs6MngMsgFormat */
#define E_GrDvrFs6MngMsgDiskStatGet					5		/* St_GrDvrFs6MngMsgDiskStatGet */
#define E_GrDvrFs6MngMsgDiskRecInfoGet			6		/* St_GrDvrFs6MngMsgDiskRecInfoGet */
#define E_GrDvrFs6MngMsgGroupStatGet				7		/* St_GrDvrFs6MngMsgGroupStatGet */
#define E_GrDvrFs6MngMsgGroupRecInfoGet			8		/* St_GrDvrFs6MngMsgGroupRecInfoGet */
#define E_GrDvrFs6MngMsgGroupDayMapGet			9		/* St_GrDvrFs6MngMsgGroupDayMapGet */
#define E_GrDvrFs6MngMsgGroupMinRatbTblGet	10	/* St_GrDvrFs6MngMsgGroupMinRatbTblGet */
#define E_GrDvrFs6MngMsgReadFwNext					11	/* St_GrDvrFs6MngMsgReadPara */
#define E_GrDvrFs6MngMsgReadBwNext					12	/* St_GrDvrFs6MngMsgReadPara */
#define E_GrDvrFs6MngMsgReadBegin						13	/* St_GrDvrFs6MngMsgReadPara */
//#define E_GrDvrFs6MngMsgReadEnd							14	/* St_GrDvrFs6MngMsgReadPara */
#define E_GrDvrFs6MngMsgLogAdd							14	/* St_GrDvrFs6MngMsgLogAdd */
#define E_GrDvrFs6MngMsgLogReadOpen					15	/* St_GrDvrFs6MngMsgLogRead */
#define E_GrDvrFs6MngMsgLogReadNext					16	/* St_GrDvrFs6MngMsgLogRead */
//#define E_GrDvrFs6MngMsgLogFindPrev					17	/* St_GrDvrFs6MngMsgLogFind */
#define E_GrDvrFs6MngMsgSysInfo							18	/* St_GrDvrFs6MngMsgSysInfo */
#define E_GrDvrFs6MngMsgGroupTimeRngGet			19	/* St_GrDvrFs6MngMsgGroupTimeRngGet */
#define E_GrDvrFs6MngMsgRecSizeEstm					20	/* St_GrDvrFs6MngMsgRecSizeEstm */
#define E_GrDvrFs6MngMsgAtDelTimeSet				21	/* St_GrDvrFs6MngMsgAtDelTimeSet */
#define E_GrDvrFs6MngMsgGroupFormat					22	/* St_GrDvrFs6MngMsgGroupFormat */
#define E_GrDvrFs6MngMsgReadFwByTime				23	/* St_GrDvrFs6MngMsgReadTimePara */
#define E_GrDvrFs6MngMsgReadBwByTime				24	/* St_GrDvrFs6MngMsgReadTimePara */
#define	E_GrDvrFs6MngMsgDiskModelGet				25	/* St_GrDvrFs6MngMsgDiskModelGet */
#define	E_GrDvrFs6MngMsgGroupDataReadHndl		26	/* St_GrDvrFs6MngMsgGroupDataReadHndl */
#define	E_GrDvrFs6MngMsgDmuxNew							27	/* St_GrDvrFs6MngMsgDmuxNew */
#define	E_GrDvrFs6MngMsgDmuxDelete					28	/* St_GrDvrFs6MngMsgDmuxDelte */
#define	E_GrDvrFs6MngMsgDmuxGroupSet				29	/* St_GrDvrFs6MngMsgDmuxGroupSet */
#define	E_GrDvrFs6MngMsgDmuxReadCtrlPtrGet	30	/* St_GrDvrFs6MngMsgDmuxReadCtrlPtrGet */
#define	E_GrDvrFs6MngMsgDmuxDo							31	/* St_GrDvrFs6MngMsgDmuxDo */
#define	E_GrDvrFs6MngMsgDmuxRdyFrmRead			32	/* St_GrDvrFs6MngMsgDmuxRdyFrmRead */
#define	E_GrDvrFs6MngMsgDmuxRdyFrmDone			33	/* St_GrDvrFs6MngMsgDmuxRdyFrmDone */
#define	E_GrDvrFs6MngMsgDmuxGoTime					34	/* St_GrDvrFs6MngMsgDmuxGoTime */
#define	E_GrDvrFs6MngMsgDmuxDirSet					35	/* St_GrDvrFs6MngMsgDmuxDirSet */
//#define	E_GrDvrFs6MngMsgDmuxModeSet					36	/* St_GrDvrFs6MngMsgDmuxModeSet */
#define	E_GrDvrFs6MngMsgDmuxVdoChAbleSet		37	/* St_GrDvrFs6MngMsgDmuxVdoChAbleSet */
#define	E_GrDvrFs6MngMsgDmuxAdoChAbleSet		38	/* St_GrDvrFs6MngMsgDmuxAdoChAbleSet */
#define	E_GrDvrFs6MngMsgDmuxVdoFrmMaskSet		39	/* St_GrDvrFs6MngMsgDmuxVdoFrmMaskSet */

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs6MngGrupCtrl
{
	__u8	DskMap;
	BOOL8	IsOvwt;
	__u8	Stat;
	Cls_GrDvrFs6Grp*	Obj;
	__u64	SizeTot;
	__u64	SizeRec;
}	*Ptr_GrDvrFs6MngGrupCtrl;

typedef struct St_GrDvrFs6MngSysInfo
{
	//__u8	DskStat[E_GrDvrFs6MaxDriveCnt];
	//__u8	DskTemp[E_GrDvrFs6MaxDriveCnt];
	__u8	MainStat;
	__u8	MirrStat;
	__u64	MainCapTot;
	__u64	MainCapRec;
	__u32	MainRecTimeSt;
	__u32	MainRecTimeEd;
}	*Ptr_GrDvrFs6MngSysInfo;

// message
typedef	struct St_GrDvrFs6MngMsgBase 
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrDvrFs6MngMsgBase;

typedef	struct St_GrDvrFs6MngMsgSetup
{
	St_GrDvrFs6MngMsgBase	Base;
	__u8	NrmMap;
	BOOL8	IsNrmOvwt;
	__u8	MirMap;
	BOOL8	IsMirOvwt;

}	*Ptr_GrDvrFs6MngMsgSetup;

typedef	struct St_GrDvrFs6MngMsgFormat
{
	St_GrDvrFs6MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs6MngMsgFormat;

typedef	struct St_GrDvrFs6MngMsgDiskStatGet
{
	St_GrDvrFs6MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs6MngMsgDiskStatGet;

typedef	struct St_GrDvrFs6MngMsgDiskRecInfoGet
{
	St_GrDvrFs6MngMsgBase	Base;
	__u8*	PtrRslt;
	__u64*	PtrRtTotal;
	__u64*	PtrRtRec;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs6MngMsgDiskRecInfoGet;

typedef	struct St_GrDvrFs6MngMsgGroupStatGet
{
	St_GrDvrFs6MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs6MngMsgGroupStatGet;

typedef	struct St_GrDvrFs6MngMsgGroupRecInfoGet
{
	St_GrDvrFs6MngMsgBase	Base;
	__u8*	PtrRslt;
	__u64*	PtrRtTotal;
	__u64*	PtrRtRec;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs6MngMsgGroupRecInfoGet;

typedef	struct St_GrDvrFs6MngMsgGroupDayMapGet
{
	St_GrDvrFs6MngMsgBase	Base;
	__u32*	PtrRslt;
	Def_GrTime TimeMonth;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrDvrFs6MngMsgGroupDayMapGet;

typedef struct St_GrDvrFs6MngMsgGroupMinRatbTblGet
{
	St_GrDvrFs6MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	Def_GrTime	TimeSt;
	__u32	ItemCnt;
	Ptr_GrDvrRecMinMap	PtrBuf;

}	*Ptr_GrDvrFs6MngMsgGroupMinRatbTblGet;


typedef struct St_GrDvrFs6MngMsgReadPara
{
	St_GrDvrFs6MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrDvrFs6ReadCtrl	PtrCtrl;

}	*Ptr_GrDvrFs6MngMsgReadPara;

typedef struct St_GrDvrFs6MngMsgReadTimePara
{
	St_GrDvrFs6MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrDvrFs6ReadCtrl	PtrCtrl;
	Def_GrTime	Time;
}	*Ptr_GrDvrFs6MngMsgReadTimePara;

typedef struct St_GrDvrFs6MngMsgLogAdd
{
	St_GrDvrFs6MngMsgBase	Base;
	St_GrDvrLogItm	Log;
}	*Ptr_GrDvrFs6MngMsgLogAdd;

typedef struct St_GrDvrFs6MngMsgLogRead
{
	St_GrDvrFs6MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	DevType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrDvrFs6LogCtrl	PtrCtrl;
}	*Ptr_GrDvrFs6MngMsgLogRead;

typedef struct St_GrDvrFs6MngMsgSysInfo
{
	St_GrDvrFs6MngMsgBase	Base;
	Ptr_GrDvrFs6MngSysInfo	PtrInfo;
}	*Ptr_GrDvrFs6MngMsgSysInfo;

typedef struct St_GrDvrFs6MngMsgGroupTimeRngGet
{
	St_GrDvrFs6MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	Group;
	Def_GrTime*	PtrRtTimeSt;
	Def_GrTime*	PtrRtTimeEd;
}	*Ptr_GrDvrFs6MngMsgGroupTimeRngGet;

typedef struct St_GrDvrFs6MngMsgRecSizeEstm
{
	St_GrDvrFs6MngMsgBase		Base;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	ChMap;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	__u64*	PtrRslt;
}	*Ptr_GrDvrFs6MngMsgRecSizeEstm;

typedef struct St_GrDvrFs6MngMsgAtDelTimeSet
{
	St_GrDvrFs6MngMsgBase		Base;
	__u32	Sec;
}	*Ptr_GrDvrFs6MngMsgAtDelTimeSet;

typedef struct St_GrDvrFs6MngMsgGroupFormat
{
	St_GrDvrFs6MngMsgBase		Base;
	__u8	GroupType;
	BOOL8 IsWithLog;
}	*Ptr_GrDvrFs6MngMsgGroupFormat;

typedef struct St_GrDvrFs6MngMsgDiskModelGet
{
	St_GrDvrFs6MngMsgBase		Base;
	BOOL8*	PtrRslt;
	__u8		DevIdx;
	WCHAR*	StrRt;
}	*Ptr_GrDvrFs6MngMsgDiskModelGet;

typedef struct St_GrDvrFs6MngMsgGroupDataReadHndl
{
	St_GrDvrFs6MngMsgBase		Base;
	BOOL8*	PtrRslt;
	__u8		GroupType;
	__u64		Hndl;
	void*		PtrBuf;
	__u32		Size;

}*Ptr_GrDvrFs6MngMsgGroupDataReadHndl;

typedef struct St_GrDvrFs6MngMsgDmuxNew
{
	St_GrDvrFs6MngMsgBase		Base;
	__s32*	PtrRslt;
	__u8		MaxChCnt;
	__u8		_rsvd0;
	__u8		_rsvd1;
	__u8		_rsvd2;
}*Ptr_GrDvrFs6MngMsgDmuxNew;

typedef struct St_GrDvrFs6MngMsgDmuxDelte
{
	St_GrDvrFs6MngMsgBase		Base;
	BOOL8*	PtrRslt;
	__s32	Hndl;
}*Ptr_GrDvrFs6MngMsgDmuxDelte;

typedef struct St_GrDvrFs6MngMsgDmuxGroupSet
{
	St_GrDvrFs6MngMsgBase		Base;
	__s32	Hndl;
	__u8	RecGrpType;
	__u8		_rsvd0;
	__u8		_rsvd1;
	__u8		_rsvd2;
}*Ptr_GrDvrFs6MngMsgDmuxGroupSet;

typedef struct St_GrDvrFs6MngMsgDmuxReadCtrlPtrGet
{
	St_GrDvrFs6MngMsgBase		Base;
	Ptr_GrDvrFs6ReadCtrl*		PtrRslt;
	__s32	Hndl;
	__u8	Ch;
	__u8		_rsvd0;
	__u8		_rsvd1;
	__u8		_rsvd2;
}*Ptr_GrDvrFs6MngMsgDmuxReadCtrlPtrGet;

typedef struct St_GrDvrFs6MngMsgDmuxDo
{
	St_GrDvrFs6MngMsgBase		Base;
	__u64*	PtrRslt;
	__s32	Hndl;
}*Ptr_GrDvrFs6MngMsgDmuxDo;

typedef struct St_GrDvrFs6MngMsgDmuxRdyFrmRead
{
	St_GrDvrFs6MngMsgBase		Base;
	__u64*	PtrRslt;
	__s32	Hndl;
	Ptr_GrDvrFs6ReadFrameItem	PtrBufTbl;
	__u32		BufCnt;
}*Ptr_GrDvrFs6MngMsgDmuxRdyFrmRead;

typedef struct St_GrDvrFs6MngMsgDmuxRdyFrmDone
{
	St_GrDvrFs6MngMsgBase		Base;
	__s32	Hndl;
	__u64	Map;
}*Ptr_GrDvrFs6MngMsgDmuxRdyFrmDone;

typedef struct St_GrDvrFs6MngMsgDmuxGoTime
{
	St_GrDvrFs6MngMsgBase		Base;
	__s32	Hndl;
	Def_GrTime	Time;
}*Ptr_GrDvrFs6MngMsgDmuxGoTime;

typedef struct St_GrDvrFs6MngMsgDmuxDirSet
{
	St_GrDvrFs6MngMsgBase		Base;
	__s32	Hndl;
	BOOL8	IsDirBack;
	__u8		_rsvd0;
	__u8		_rsvd1;
	__u8		_rsvd2;
}*Ptr_GrDvrFs6MngMsgDmuxDirSet;

typedef struct St_GrDvrFs6MngMsgDmuxVdoChAbleSet
{
	St_GrDvrFs6MngMsgBase		Base;
	__s32	Hndl;
	__u32	Map;
}*Ptr_GrDvrFs6MngMsgDmuxVdoChAbleSet;

typedef struct St_GrDvrFs6MngMsgDmuxAdoChAbleSet
{
	St_GrDvrFs6MngMsgBase		Base;
	__s32	Hndl;
	__u32	Map;
}*Ptr_GrDvrFs6MngMsgDmuxAdoChAbleSet;

typedef struct St_GrDvrFs6MngMsgDmuxVdoFrmMaskSet
{
	St_GrDvrFs6MngMsgBase		Base;
	__s32		Hndl;
	__u8		Ch;
	__u8		Mask;
	__u8		_rsvd0;
	__u8		_rsvd1;
}*Ptr_GrDvrFs6MngMsgDmuxVdoFrmMaskSet;


//====================================================================
//class

class	Cls_GrDvrFs6Mng	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32	m_MsgAloc;
		__u32	m_HndlTimer;

		// 
		BOOL8	m_IsInited;

		St_GrDvrFs6MngGrupCtrl	m_GrupTbl[E_GrDvrFs6MngGroupCnt];

		__u32	m_AtDelTime;
		Def_GrTime	m_AtDelBaseTm;

		__u32	m_RflushTimer;

		__u32	m_CchTimer;

		__u32	m_TempTimer;
		__u8	m_TempTbl[E_GrDvrFs6MaxDiskCnt];

	protected:
		//local
		// build disk table
		void	LcGroupBuild(void);
		void	LcGroupRecFlush(void);
		void	LcTmrRecFlush(void);
		void	LcTmrCchFlush(void);

		void	LcTempUpdt( void );

		BOOL8	LcBgpBkupSizeEstm( void );
		void	LcAtDelLimUpdt( void );

		void	LcGroupInfoUpdate(void);

		//event
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	void	RtlTimer( __u32 A_Hndl );
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */

	public:
		Cls_GrDvrFs6Mng(Cls_GrTaskSvr* A_TaskSvr);
    virtual		~Cls_GrDvrFs6Mng();

		// status
		__u8	DiskTemperatureGet( __u8 A_DskIdx );
		__u32	AutoDelTimeGet( void );

		// thread depended status
		__u32	ImmGroupGetMonthDayMap(__u8 A_GroupType, Def_GrTime A_TimeMonth);
		BOOL8	ImmGroupGetMinRatbTbl(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrRtBuf);
		BOOL8	ImmGroupGetRecTimeRng(__u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd);

		__u8	ImmDiskGetStat( __u8 A_DevIdx );
		__u8	ImmGroupGetStat( __u8 A_GroupType );
		void	ImmSysGetInfo( Ptr_GrDvrFs6MngSysInfo A_PtrRtInfo );
		BOOL8	ImmIsGroupRecAble( __u8 A_GroupType );
		__u64	ImmRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap,Def_GrTime A_TimeSt, Def_GrTime A_TimeEd);

		void	ImmAtDelTimeSet(__u32 A_Sec);
		void	ImmGroupFormat(__u8 A_GroupType);

		__u32	ReqGroupGetMonthDayMap(__u8 A_GroupType, Def_GrTime A_TimeMonth);
		BOOL8	ReqGroupGetMinRatbTbl(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrBuf);	// get record thumb table
		BOOL8	ReqGroupGetRecTimeRng(__u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd);
		__u64	ReqRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd);

		__u8	SafeDiskGetStat( __u8 A_DevIdx );
		__u8	SafeGroupGetStat(__u8 A_GroupType);
		void	SafeSysGetInfo(Ptr_GrDvrFs6MngSysInfo A_PtrRtInfo);

		void	SafeAtDelTimeSet(__u32 A_Sec);
		void	SafeGroupFormat(__u8 A_GroupType, BOOL8 A_IsWithLog);


		// control
		void	ImmInit( void );
		void	ImmFinish(void);

		void	ImmSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir );
		BOOL8	ImmDiskFormat( __u8 A_DevIdx, BOOL8 A_IsPostPrgs );
		__u8	ImmDiskRecInfoGet(__u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec);
		BOOL8	ImmDiskModelNameGet(__u8 A_DevIdx, WCHAR* A_StrRt);
		__u8	ImmGroupRecInfoGet(__u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec);
		__u8	ImmGroupDataReadByHndl(__u8 A_GroupType, __u64 A_Hndl, void* A_PtrData, __u32 A_Size);

		void	ReqInit( void );	// try initialize
		void	ReqFinish(void);	// try finalize

		void	ReqSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir );
		BOOL8	ReqDiskFormat( __u8 A_DevIdx, BOOL8 A_IsPost );		// format request
		__u8	ReqDiskRecInfoGet(__u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec);
		__u8	ReqGroupRecInfoGet(__u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec);
		BOOL8	ReqDiskModelNameGet(__u8 A_DevIdx, WCHAR* A_StrRt);
		__u8	ReqGroupDataReadByHndl(__u8 A_GroupType, __u64 A_Hndl, void* A_PtrData, __u32 A_Size);

		// write
		BOOL8	ImmFrmVdoAdd(Ptr_GrDvrFrmHdVdo A_PtrFrm);
		BOOL8	ImmFrmAdoAdd(Ptr_GrDvrFrmHdAdo A_PtrFrm);

		// read
		__u8	ImmReadFwGoByTime(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl, Def_GrTime A_Time);
		__u8	ImmReadFwNext(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl);
		__u8	ImmReadBwGoByTime(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl, Def_GrTime A_Time);
		__u8	ImmReadBwNext(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl);

		__u8	ReqReadFwGoByTime(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl, Def_GrTime A_Time);
		__u8	ReqReadFwNext(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl);
		__u8	ReqReadBwGoByTime(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl, Def_GrTime A_Time);
		__u8	ReqReadBwNext(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl);

		// dmux
		__s32	ImmDmuxNew(__u8 A_MaxChCnt);
		BOOL8	ImmDmuxDelete(__s32 A_Hndl);
		void	ImmDmuxRecGroupSet(__s32 A_Hndl, __u8 A_RecGrpType);
		Ptr_GrDvrFs6ReadCtrl	ImmDmuxReadCtrlPtrGet(__s32 A_Hndl, __u8 A_Ch);
		__u64	ImmDmuxDo(__s32 A_Hndl);
		__u64	ImmDmuxReadyFrameRead(__s32 A_Hndl, Ptr_GrDvrFs6ReadFrameItem A_PtrBufTbl, __u32 A_BufCnt);
		void	ImmDmuxReadyFrameDone(__s32 A_Hndl, __u64 A_Map);
		void	ImmDmuxGoTime(__s32 A_Hndl, Def_GrTime A_Time);
		void	ImmDmuxDirSet(__s32 A_Hndl, BOOL8 A_IsDirBack);
		void	ImmDmuxVdoChAbleSet(__s32 A_Hndl, __u32 A_Map);
		void	ImmDmuxAdoChAbleSet(__s32 A_Hndl, __u32 A_Map);
		void	ImmDmuxVdoFrmMaskSet(__s32 A_Hndl, __u8 A_Ch, __u8 A_Mask);
		__u8	ImmDmuxRunStaGet(__s32 A_Hndl);

		__s32	ReqDmuxNew(__u8 A_MaxChCnt);
		BOOL8	ReqDmuxDelete(__s32 A_Hndl);
		void	ReqDmuxRecGroupSet(__s32 A_Hndl, __u8 A_RecGrpType);
		Ptr_GrDvrFs6ReadCtrl	ReqDmuxReadCtrlPtrGet(__s32 A_Hndl, __u8 A_Ch);
		__u64	ReqDmuxDo(__s32 A_Hndl);
		__u64	ReqDmuxReadyFrameRead(__s32 A_Hndl, Ptr_GrDvrFs6ReadFrameItem A_PtrBufTbl, __u32 A_BufCnt);
		void	ReqDmuxReadyFrameDone(__s32 A_Hndl, __u64 A_Map);
		void	ReqDmuxGoTime(__s32 A_Hndl, Def_GrTime A_Time);
		void	ReqDmuxDirSet(__s32 A_Hndl, BOOL8 A_IsDirBack);
		void	ReqDmuxVdoChAbleSet(__s32 A_Hndl, __u32 A_Map);
		void	ReqDmuxAdoChAbleSet(__s32 A_Hndl, __u32 A_Map);
		void	ReqDmuxVdoFrmMaskSet(__s32 A_Hndl, __u8 A_Ch, __u8 A_Mask);

		// event log
		void	ImmLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		__u8	ImmLogReadOpen(__u8 A_DevType, Ptr_GrDvrFs6LogCtrl A_PtrCtrl);
		__u8	ImmLogReadNext(__u8 A_DevType, Ptr_GrDvrFs6LogCtrl A_PtrCtrl);

		void	ReqLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		__u8	ReqLogReadOpen(__u8 A_DevType, Ptr_GrDvrFs6LogCtrl A_PtrCtrl);
		__u8	ReqLogReadNext(__u8 A_DevType, Ptr_GrDvrFs6LogCtrl A_PtrCtrl);


		// runtime
		Cls_GrDvrFs6Disk*	RtlDiskObjGet( __u8 A_Idx );
		void*	RtlTmpBufAlloc(__u32 A_Size);
		void	RtlTmpBufFree(__u32 A_Size);

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


