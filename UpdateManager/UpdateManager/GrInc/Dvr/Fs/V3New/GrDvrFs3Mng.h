/*
	GAUSE DVR file system v3 manager

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs3Mng
#define	_EPC_GrDvrFs3Mng

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>
#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/Fs/V3/GrDvrFs3Base.h>
#include <Dvr/Fs/V3/GrDvrFs3Disk.h>
#include <Dvr/Fs/V3/GrDvrFs3Grp.h>
	
//====================================================================
//constance

#define E_GrDvrFs3MngGroupCnt								2

#define E_GrDvrFs3MngTimerPeriod						1000

#define E_GrDvrFs3MngTempScanPeriod					60

#define E_GrDvrFs3MngScanSizeModeNone				0
#define E_GrDvrFs3MngScanSizeModeScan				1
#define E_GrDvrFs3MngScanSizeModeFinish			2

// message command
#define E_GrDvrFs3MngMsgInit								1		/* initialize */
#define E_GrDvrFs3MngMsgSetup								2		/* St_GrDvrFs3MngMsgSetup */
#define E_GrDvrFs3MngMsgFormat							3		/* St_GrDvrFs3MngMsgFormat */
#define E_GrDvrFs3MngMsgDiskStatGet					4		/* St_GrDvrFs3MngMsgDiskStatGet */
#define E_GrDvrFs3MngMsgDiskRecSizeGet			5		/* St_GrDvrFs3MngMsgDiskRecSizeGet */
#define E_GrDvrFs3MngMsgGroupStatGet				6		/* St_GrDvrFs3MngMsgGroupStatGet */
#define E_GrDvrFs3MngMsgGroupRecSizeGet			7		/* St_GrDvrFs3MngMsgGroupRecSizeGet */
#define E_GrDvrFs3MngMsgGroupDayMapGet			8		/* St_GrDvrFs3MngMsgGroupDayMapGet */
#define E_GrDvrFs3MngMsgGroupMinMapGet			9		/* St_GrDvrFs3MngMsgGroupMinMapGet */
#define E_GrDvrFs3MngMsgReadForward					10	/* St_GrDvrFs3MngMsgReadPara */
#define E_GrDvrFs3MngMsgReadBackward				11	/* St_GrDvrFs3MngMsgReadPara */
#define E_GrDvrFs3MngMsgReadBegin						12	/* St_GrDvrFs3MngMsgReadPara */
#define E_GrDvrFs3MngMsgReadEnd							13	/* St_GrDvrFs3MngMsgReadPara */
#define E_GrDvrFs3MngMsgReadByHndlFw				14	/* St_GrDvrFs3MngMsgReadHndlPara */
#define E_GrDvrFs3MngMsgReadByHndlBw				15	/* St_GrDvrFs3MngMsgReadHndlPara */
#define E_GrDvrFs3MngMsgReadByMinFw					16	/* St_GrDvrFs3MngMsgReadTimePara */
#define E_GrDvrFs3MngMsgReadByMinBw					17	/* St_GrDvrFs3MngMsgReadTimePara */
#define E_GrDvrFs3MngMsgLogAdd							18	/* St_GrDvrFs3MngMsgLogAdd */
#define E_GrDvrFs3MngMsgLogFindStart				19	/* St_GrDvrFs3MngMsgLogFind */
#define E_GrDvrFs3MngMsgLogFindNext					20	/* St_GrDvrFs3MngMsgLogFind */
#define E_GrDvrFs3MngMsgLogFindPrev					21	/* St_GrDvrFs3MngMsgLogFind */
#define E_GrDvrFs3MngMsgSysInfo							22	/* St_GrDvrFs3MngMsgSysInfo */
#define E_GrDvrFs3MngMsgGroupTimeRngGet			23	/* St_GrDvrFs3MngMsgGroupTimeRngGet */
#define E_GrDvrFs3MngMsgScanRecSize					24	/* St_GrDvrFs3MngMsgScanRecSize */
#define E_GrDvrFs3MngMsgAtDelTimeSet				25	/* St_GrDvrFs3MngMsgAtDelTimeSet */

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs3MngSysInfo
{
	__u8	DskStat[E_GrDvrFs3MaxDriveCnt];
	__u8	DskTemp[E_GrDvrFs3MaxDriveCnt];
	__u8	MainStat;
	__u8	MirrStat;
	__u64	MainCapTot;
	__u64	MainCapRec;
	__u32	MainRecTimeSt;
	__u32	MainRecTimeEd;
}	*Ptr_GrDvrFs3MngSysInfo;

// message
typedef	struct St_GrDvrFs3MngMsgBase 
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrDvrFs3MngMsgBase;

typedef	struct St_GrDvrFs3MngMsgSetup
{
	St_GrDvrFs3MngMsgBase	Base;
	__u8	NrmMap;
	BOOL8	IsNrmOvwt;
	__u8	MirMap;
	BOOL8	IsMirOvwt;
	BOOL8	IsAutoFix;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrDvrFs3MngMsgSetup;

typedef	struct St_GrDvrFs3MngMsgFormat
{
	St_GrDvrFs3MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	DevIdx;
	BOOL8	IsWithLog;
	__u8	_rsvd0;
	__u8	_rsvd1;
}	*Ptr_GrDvrFs3MngMsgFormat;

typedef	struct St_GrDvrFs3MngMsgDiskStatGet
{
	St_GrDvrFs3MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs3MngMsgDiskStatGet;

typedef	struct St_GrDvrFs3MngMsgDiskRecSizeGet
{
	St_GrDvrFs3MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u64*	PtrRtTotal;
	__u64*	PtrRtRec;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs3MngMsgDiskRecSizeGet;

typedef	struct St_GrDvrFs3MngMsgGroupStatGet
{
	St_GrDvrFs3MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs3MngMsgGroupStatGet;

typedef	struct St_GrDvrFs3MngMsgGroupRecSizeGet
{
	St_GrDvrFs3MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u64*	PtrRtTotal;
	__u64*	PtrRtRec;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs3MngMsgGroupRecSizeGet;

typedef	struct St_GrDvrFs3MngMsgGroupDayMapGet
{
	St_GrDvrFs3MngMsgBase	Base;
	__u32*	PtrRslt;
	Def_GrTime TimeSt;
	__u8	DayCnt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
}	*Ptr_GrDvrFs3MngMsgGroupDayMapGet;

typedef struct St_GrDvrFs3MngMsgGroupMinMapGet
{
	St_GrDvrFs3MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	Def_GrTime	TimeSt;
	__u32	ItemCnt;
	void*	PtrBuf;

}	*Ptr_GrDvrFs3MngMsgGroupMinMapGet;


typedef struct St_GrDvrFs3MngMsgReadPara
{
	St_GrDvrFs3MngMsgBase	Base;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrDvrFs3SeekCtrl	PtrCtrl;

}	*Ptr_GrDvrFs3MngMsgReadPara;

typedef struct St_GrDvrFs3MngMsgReadHndlPara
{
	St_GrDvrFs3MngMsgBase	Base;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrDvrFs3SeekCtrl	PtrCtrl;
	__u64	Hndl;
}	*Ptr_GrDvrFs3MngMsgReadHndlPara;

typedef struct St_GrDvrFs3MngMsgReadTimePara
{
	St_GrDvrFs3MngMsgBase	Base;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrDvrFs3SeekCtrl	PtrCtrl;
	Def_GrTime	Time;
}	*Ptr_GrDvrFs3MngMsgReadTimePara;

typedef struct St_GrDvrFs3MngMsgLogAdd
{
	St_GrDvrFs3MngMsgBase	Base;
	St_GrDvrLogV2Item	Log;
}	*Ptr_GrDvrFs3MngMsgLogAdd;

typedef struct St_GrDvrFs3MngMsgLogFind
{
	St_GrDvrFs3MngMsgBase	Base;
	Ptr_GrDvrLogV2Ctrl	PtrCtrl;
}	*Ptr_GrDvrFs3MngMsgLogFind;

typedef struct St_GrDvrFs3MngMsgSysInfo
{
	St_GrDvrFs3MngMsgBase	Base;
	Ptr_GrDvrFs3MngSysInfo	PtrInfo;
}	*Ptr_GrDvrFs3MngMsgSysInfo;

typedef struct St_GrDvrFs3MngMsgGroupTimeRngGet
{
	St_GrDvrFs3MngMsgBase	Base;
	__u8	Group;
	Def_GrTime*	PtrRtTimeSt;
	Def_GrTime*	PtrRtTimeEd;
}	*Ptr_GrDvrFs3MngMsgGroupTimeRngGet;

typedef struct St_GrDvrFs3MngMsgScanRecSize
{
	St_GrDvrFs3MngMsgBase		Base;
	__u8	Group;
	__u8	_rsvd;
	__u16	ChMask;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	BOOL8*	PtrRslt;
}	*Ptr_GrDvrFs3MngMsgScanRecSize;

typedef struct St_GrDvrFs3MngMsgAtDelTimeSet
{
	St_GrDvrFs3MngMsgBase		Base;
	__u32	Sec;
}	*Ptr_GrDvrFs3MngMsgAtDelTimeSet;

//====================================================================
//class

class	Cls_GrDvrFs3Mng	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32	m_MsgAloc;
		__u32	m_HndlTimer;

		// 
		BOOL8	m_IsInited;

		__u64	m_DvrId;
		__u32	m_AdoUnitSize;
		__u8	m_AdoSyncCnt;

		__u8	m_GrupNrmMap;
		BOOL8	m_IsOvwtNrm;
		__u8	m_GrupMirMap;
		BOOL8	m_IsOvwtMir;

		BOOL8	m_IsAutoFix;

		__u32	m_AtDelTime;
		Def_GrTime	m_AtDelBaseTm;

		Cls_GrDvrFs3Disk*	m_DiskTbl[E_GrDvrFs3MaxDriveCnt];
		Cls_GrDvrFs3Grp*	m_GrupTbl[E_GrDvrFs3MngGroupCnt];		// group object

		__u8	m_TmpBuf[E_GrDvrFs3TmpBufSize];

		__u32	m_TempTimer;
				
		__u8	m_TempTbl[E_GrDvrFs3MaxDriveCnt];

		__u8	m_ScanSizeMode;
		St_GrDvrFs3RecSizeReq	m_ScanSizeReq;

	protected:
		//local
		void	LcGroupBuild( void );

		void	LcTempUpdt( void );

		void	LcMprcScanSizeReq( Ptr_GrDvrFs3MngMsgScanRecSize A_PtrMsg );

		BOOL8	LcBgpScanRecSize( void );
		void	LcAtDelLimUpdt( void );

		//event
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	void	RtlTimer( __u32 A_Hndl );
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */

	public:
    Cls_GrDvrFs3Mng( Cls_GrTaskSvr* A_TaskSvr, __u64 A_DvrId, __u32 A_AdoUnitSize, __u8 A_AdoSyncCnt );
    virtual		~Cls_GrDvrFs3Mng();

		// status
		__u8	AdoSyncCntGet( void );
		__u32	AdoDataUnitSizeGet( void );
		__u8	TempratureGet( __u8 A_DskIdx );
		__u32	AutoDelTimeGet( void );

		// thread depended status
		__u8	ImmDiskStatGet( __u8 A_DevIdx );
		BOOL8	ImmDiskRecSizeGet( __u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u8	ImmGroupStatGet( __u8 A_GroupType );
		BOOL8	ImmGroupRecSizeGet( __u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u32	ImmGroupDayMapGet( __u8 A_GroupType, Def_GrTime A_Time, __u8 A_DayCnt );
		BOOL8	ImmGroupMinMapGet( __u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, void* A_PtrBuf );
		void	ImmGroupRecTimeRngGet( __u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd );
		void	ImmSysInfoGet( Ptr_GrDvrFs3MngSysInfo A_PtrRtInfo );
		BOOL8	ImmGroupRecAble( __u8 A_GroupType );

		__u8	SafeDiskStatusGet( __u8 A_DevIdx );
		BOOL8	SafeDiskRecSizeGet( __u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u8	SafeGroupStatusGet( __u8 A_GroupType );
		BOOL8	SafeGroupRecSizeGet( __u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u32	SafeGroupDayMapGet( __u8 A_GroupType, Def_GrTime A_Time, __u8 A_DayCnt );
		BOOL8	SafeGroupMinMapGet( __u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, void* A_PtrBuf );	// get record thumb table
		void	SafeGroupRecTimeRngGet( __u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd );
		void	SafeSysInfoGet( Ptr_GrDvrFs3MngSysInfo A_PtrRtInfo );
		BOOL8	SafeScanRecSize( __u8 A_GroupType, __u16 A_ChMask, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u64* A_PtrRtSize );

		// control
		void	ImmInit( void );
		void	ImmSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir, BOOL8 A_IsAutoFix );
		BOOL8	ImmFormat( __u8 A_DevIdx, BOOL8 A_IsWithLog );
		void	ImmAtDelTimeSet( __u32 A_Sec );

		void	SafeInit( void );	// try initialize
		void	SafeSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir, BOOL8 A_IsAutoFix );
		BOOL8	SafeFormat( __u8 A_DevIdx, BOOL8 A_IsWithLog );		// format request
		void	SafeAtDelTimeSet( __u32 A_Sec );

		// write
		BOOL8	VideoAdd( Ptr_GrDvrAvHd A_PtrPkt );		// video packet add - thread not safe
		BOOL8	AudioAdd( Ptr_GrDvrAvHd A_PtrPkt );		// audio packet add - thread not safe
		BOOL8	PacketAdd( Ptr_GrDvrAvHd A_PtrPkt );	// packet add - thread not safe

		// read
		void	ImmReadForward( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl );
		void	ImmReadBackward( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl );
		void	ImmReadBegin( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl );
		void	ImmReadEnd( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl );
		void	ImmReadByHndlFw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		void	ImmReadByHndlBw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		void	ImmReadByMinFw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, Def_GrTime A_Time );
		void	ImmReadByMinBw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, Def_GrTime A_Time );

		void	SafeReadForward( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl );
		void	SafeReadBackward( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl );
		void	SafeReadBegin( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl );
		void	SafeReadEnd( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl );
		void	SafeReadByHndlFw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		void	SafeReadByHndlBw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		void	SafeReadByMinFw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, Def_GrTime A_Time );
		void	SafeReadByMinBw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, Def_GrTime A_Time );

		// event log
		void	ImmLogAdd( Ptr_GrDvrLogV2Item A_PtrItem );
		void	ImmLogFindStart( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );
		void	ImmLogFindNext( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );
		void	ImmLogFindPrev( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );

		void	SafeLogAdd( Ptr_GrDvrLogV2Item A_PtrItem );
		void	SafeLogFindStart( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );
		void	SafeLogFindNext( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );
		void	SafeLogFindPrev( Ptr_GrDvrLogV2Ctrl A_PtrCtrl );


		// runtime
		Cls_GrDvrFs3Disk*	RtlDiskObjGet( __u8 A_Idx );

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


