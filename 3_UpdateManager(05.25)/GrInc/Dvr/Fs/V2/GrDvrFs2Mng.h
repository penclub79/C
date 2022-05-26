/*
	GAUSE DVR file system v3 manager

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs2Mng
#define	_EPC_GrDvrFs2Mng

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>
#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/Fs/V2/GrDvrFs2Base.h>
#include <Dvr/Fs/V2/GrDvrFs2Disk.h>
#include <Dvr/Fs/V2/GrDvrFs2Grp.h>
	
//====================================================================
//constance

#define E_GrDvrFs2MngGroupCnt								2

// message command
#define E_GrDvrFs2MngMsgInit								1		/* St_GrDvrFs2MngMsgBase */
#define E_GrDvrFs2MngMsgSetup								2		/* St_GrDvrFs2MngMsgSetup */
#define E_GrDvrFs2MngMsgFormat							3		/* St_GrDvrFs2MngMsgFormat */
#define E_GrDvrFs2MngMsgDiskStatGet					4		/* St_GrDvrFs2MngMsgDiskStatGet */
#define E_GrDvrFs2MngMsgDiskRecSizeGet			5		/* St_GrDvrFs2MngMsgDiskRecSizeGet */
#define E_GrDvrFs2MngMsgGroupStatGet				6		/* St_GrDvrFs2MngMsgGroupStatGet */
#define E_GrDvrFs2MngMsgGroupRecSizeGet			7		/* St_GrDvrFs2MngMsgGroupRecSizeGet */
#define E_GrDvrFs2MngMsgGroupDayMapGet			8		/* St_GrDvrFs2MngMsgGroupDayMapGet */
#define E_GrDvrFs2MngMsgGroupRecThumbGet		9		/* St_GrDvrFs2MngMsgGroupRecThumbGet */
#define E_GrDvrFs2MngMsgReadForward					10	/* St_GrDvrFs2MngMsgReadPara */
#define E_GrDvrFs2MngMsgReadBackward				11	/* St_GrDvrFs2MngMsgReadPara */
#define E_GrDvrFs2MngMsgReadBegin						12	/* St_GrDvrFs2MngMsgReadPara */
#define E_GrDvrFs2MngMsgReadEnd							13	/* St_GrDvrFs2MngMsgReadPara */
#define E_GrDvrFs2MngMsgReadByHndlFw				14	/* St_GrDvrFs2MngMsgReadHndlPara */
#define E_GrDvrFs2MngMsgReadByHndlBw				15	/* St_GrDvrFs2MngMsgReadHndlPara */
#define E_GrDvrFs2MngMsgReadByMinFw					16	/* St_GrDvrFs2MngMsgReadTimePara */
#define E_GrDvrFs2MngMsgReadByMinBw					17	/* St_GrDvrFs2MngMsgReadTimePara */
#define E_GrDvrFs2MngMsgVssReset						18	/* St_GrDvrFs2MngMsgBase */
#define E_GrDvrFs2MngMsgVssWrite						19	/* St_GrDvrFs2MngMsgVssRwPara */
#define E_GrDvrFs2MngMsgVssRead							20	/* St_GrDvrFs2MngMsgVssRwPara */
#define E_GrDvrFs2MngMsgUcaWrite						21	/* St_GrDvrFs2MngMsgUcaRwPara */
#define E_GrDvrFs2MngMsgUcaRead							22	/* St_GrDvrFs2MngMsgUcaRwPara */
#define	E_GrDvrFs2MngMsgRecTimeRngGet				23	/* St_GrDvrFs2MngMsgTempGetPara */
#define	E_GrDvrFs2MngMsgTimeRecSize					24	/* St_GrDvrFs2MngMsgTimeRecSizePara */
#define	E_GrDvrFs2MngMsgLogAdd							25	/* St_GrDvrFs2MngMsgLogAddPara */
#define	E_GrDvrFs2MngMsgLogFindStart				26	/* St_GrDvrFs2MngMsgLogFindPara */
#define	E_GrDvrFs2MngMsgLogFindNext					27	/* St_GrDvrFs2MngMsgLogFindPara */
#define	E_GrDvrFs2MngMsgLogFindPrev					28	/* St_GrDvrFs2MngMsgLogFindPara */
#define	E_GrDvrFs2MngMsgAutoDelete					29	/* St_GrDvrFs2MngMsgAutoDeletePara */	

//====================================================================
//global macro

//====================================================================
//global type

// message
typedef	struct St_GrDvrFs2MngMsgBase 
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrDvrFs2MngMsgBase;

typedef	struct St_GrDvrFs2MngMsgSetup
{
	St_GrDvrFs2MngMsgBase	Base;
	__u8	NrmMap;
	BOOL8	IsNrmOvwt;
	__u8	MirMap;
	BOOL8	IsMirOvwt;

}	*Ptr_GrDvrFs2MngMsgSetup;

typedef	struct St_GrDvrFs2MngMsgFormat
{
	St_GrDvrFs2MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	DevIdx;
	BOOL8	IsWithLog;
	__u8	_rsvd0;
	__u8	_rsvd1;
}	*Ptr_GrDvrFs2MngMsgFormat;

typedef	struct St_GrDvrFs2MngMsgDiskStatGet
{
	St_GrDvrFs2MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs2MngMsgDiskStatGet;

typedef	struct St_GrDvrFs2MngMsgDiskRecSizeGet
{
	St_GrDvrFs2MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u64*	PtrRtTotal;
	__u64*	PtrRtRec;
	__u8	DevIdx;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs2MngMsgDiskRecSizeGet;

typedef	struct St_GrDvrFs2MngMsgGroupStatGet
{
	St_GrDvrFs2MngMsgBase	Base;
	__u8*	PtrRslt;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs2MngMsgGroupStatGet;

typedef	struct St_GrDvrFs2MngMsgGroupRecSizeGet
{
	St_GrDvrFs2MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u64*	PtrRtTotal;
	__u64*	PtrRtRec;
	__u8	GroupType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrDvrFs2MngMsgGroupRecSizeGet;

typedef	struct St_GrDvrFs2MngMsgGroupDayMapGet
{
	St_GrDvrFs2MngMsgBase	Base;
	__u32*	PtrRslt;
	Def_GrTime TimeSt;
	__u8	DayCnt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
}	*Ptr_GrDvrFs2MngMsgGroupDayMapGet;

typedef struct St_GrDvrFs2MngMsgGroupRecThumbGet
{
	St_GrDvrFs2MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	Def_GrTime	TimeSt;
	__u32	ItemCnt;
	void*	PtrBuf;

}	*Ptr_GrDvrFs2MngMsgGroupRecThumbGet;


typedef struct St_GrDvrFs2MngMsgReadPara
{
	St_GrDvrFs2MngMsgBase	Base;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrDvrFs2SeekCtrl	PtrCtrl;

}	*Ptr_GrDvrFs2MngMsgReadPara;

typedef struct St_GrDvrFs2MngMsgReadHndlPara
{
	St_GrDvrFs2MngMsgBase	Base;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrDvrFs2SeekCtrl	PtrCtrl;
	__u64	Hndl;
}	*Ptr_GrDvrFs2MngMsgReadHndlPara;

typedef struct St_GrDvrFs2MngMsgReadTimePara
{
	St_GrDvrFs2MngMsgBase	Base;
	__u8	Group;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	Ptr_GrDvrFs2SeekCtrl	PtrCtrl;
	Def_GrTime	Time;
}	*Ptr_GrDvrFs2MngMsgReadTimePara;

typedef struct St_GrDvrFs2MngMsgVssRwPara
{
	St_GrDvrFs2MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u32	Pos;
	void*	Ptr;
	__u32	Size;
}	*Ptr_GrDvrFs2MngMsgVssRwPara;

typedef struct St_GrDvrFs2MngMsgUcaRwPara
{
	St_GrDvrFs2MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u32	Pos;
	void*	Ptr;
	__u32	Size;
}	*Ptr_GrDvrFs2MngMsgUcaRwPara;

typedef struct St_GrDvrFs2MngMsgRecTimeRngGetPara
{
	St_GrDvrFs2MngMsgBase	Base;
	BOOL8*	PtrRslt;
	__u8	Group;
	Def_GrTime* PtrRtSt;
	Def_GrTime*	PtrRtEd;
}	*Ptr_GrDvrFs2MngMsgRecTimeRngGetPara;

typedef struct St_GrDvrFs2MngMsgTimeRecSizePara
{
	St_GrDvrFs2MngMsgBase	Base;
	__u64*	PtrRslt;
	__u8	Group;
	__u16	ChMap;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
}	*Ptr_GrDvrFs2MngMsgTimeRecSizePara;

typedef struct St_GrDvrFs2MngMsgLogAddPara
{
	St_GrDvrFs2MngMsgBase	Base;
	St_GrDvrLogItm	Log;
}	*Ptr_GrDvrFs2MngMsgLogAddPara;

typedef struct St_GrDvrFs2MngMsgLogFindPara
{
	St_GrDvrFs2MngMsgBase	Base;
	__u8	Group;
	Ptr_GrDvrLogCtrl	PtrCtrl;
}	*Ptr_GrDvrFs2MngMsgLogFindPara;

typedef struct St_GrDvrFs2MngMsgAutoDeletePara
{
	St_GrDvrFs2MngMsgBase	Base;
	__u32	Sec;
}	*Ptr_GrDvrFs2MngMsgAutoDeletePara;

//====================================================================
//class

class	Cls_GrDvrFs2Mng	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32	m_MsgAloc;
		__u32	m_HndlTimer;

		// 
		BOOL8	m_IsInited;

		__u64	m_DvrId;

		__u8	m_GrupNrmMap;
		BOOL8	m_IsOvwtNrm;
		__u8	m_GrupMirMap;
		BOOL8	m_IsOvwtMir;

		__u32	m_AtdSec;		// auto delete second
		Def_GrTime	m_AtdTime;	// auto delete limit time

		Cls_GrDvrFs2Disk*	m_DiskTbl[E_GrDvrFs2MaxDriveCnt];
		Cls_GrDvrFs2Grp*	m_GrupTbl[E_GrDvrFs2MngGroupCnt];		// group object

		__u8	m_TempTbl[E_GrDvrFs2MaxDriveCnt];

		__u8	m_TmpBuf[E_GrDvrFs2TmpBufSize];

	protected:
		//local
		void	LcGroupBuild( void );

		void	LcTemperatureUpdt( void );
		void	LcAtdTimeUpdt( void );

		//event
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	void	RtlTimer( __u32 A_Hndl );

	public:
    Cls_GrDvrFs2Mng( Cls_GrTaskSvr* A_TaskSvr, __u64 A_DvrId );
    virtual		~Cls_GrDvrFs2Mng();

		// status
		__u8	AdoSyncCntGet( void );
		__u32	AdoDataUnitSizeGet( void );
		__u8	TemperatureGet( __u8 A_DevIdx );

		// thread depended status
		__u8	ImmDiskStatGet( __u8 A_DevIdx );
		BOOL8	ImmDiskRecSizeGet( __u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u8	ImmGroupStatGet( __u8 A_GroupType );
		BOOL8	ImmGroupRecSizeGet( __u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u32	ImmGroupDayMapGet( __u8 A_GroupType, Def_GrTime A_Time, __u8 A_DayCnt );
		BOOL8	ImmGroupRecMinMapGet( __u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, void* A_PtrBuf );
		BOOL8	ImmGroupRecTimeRngGet( __u8 A_GroupType, Def_GrTime* A_PtrRtTimeBegin, Def_GrTime* A_PtrRtTimeEnd );
		__u64	ImmGroupTimeRecSize( __u8 A_GroupType, __u16 A_ChMask, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );

		__u8	SafeDiskStatusGet( __u8 A_DevIdx );
		BOOL8	SafeDiskRecSizeGet( __u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u8	SafeGroupStatusGet( __u8 A_GroupType );
		BOOL8	SafeGroupRecSizeGet( __u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec );
		__u32	SafeGroupDayMapGet( __u8 A_GroupType, Def_GrTime A_Time, __u8 A_DayCnt );
		BOOL8	SafeGroupRecMinMapGet( __u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, void* A_PtrBuf );	// get record thumb table
		BOOL8	SafeGroupRecTimeRngGet( __u8 A_GroupType, Def_GrTime* A_PtrRtTimeBegin, Def_GrTime* A_PtrRtTimeEnd );
		__u64	SafeGroupTimeRecSize( __u8 A_GroupType, __u16 A_ChMask, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );

		// control
		void	ImmInit( void );
		void	ImmSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir );
		BOOL8	ImmFormat( __u8 A_DevIdx, BOOL8 A_IsWithLog );
		BOOL8	ImmVssReset( void );
		BOOL8	ImmVssWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	ImmVssRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	ImmUcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	ImmUcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		void	ImmEvtLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		void	ImmEvtLogFindStart( __u8 A_DevType, Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	ImmEvtLogFindNext( __u8 A_DevType, Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	ImmEvtLogFindPrev( __u8 A_DevType, Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	ImmAutoDeleteSet( __u32 A_Sec );

		void	SafeInit( void );	// try initialize
		void	SafeSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir );
		BOOL8	SafeFormat( __u8 A_DevIdx, BOOL8 A_IsWithLog );		// format request
		void	SafeVssReset( void );
		BOOL8	SafeVssWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	SafeVssRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	SafeUcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	SafeUcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		void	SafeEvtLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		void	SafeEvtLogFindStart( __u8 A_DevType, Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	SafeEvtLogFindNext( __u8 A_DevType, Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	SafeEvtLogFindPrev( __u8 A_DevType, Ptr_GrDvrLogCtrl A_PtrCtrl );
		void	SafeAutoDeleteSet( __u32 A_Sec );

		// write
		BOOL8	VideoAdd( Ptr_GrDvrAvHd A_PtrPkt );		// video packet add - thread not safe
		BOOL8	AudioAdd( Ptr_GrDvrAvHd A_PtrPkt );		// audio packet add - thread not safe
		BOOL8	PacketAdd( Ptr_GrDvrAvHd A_PtrPkt );	// packet add - thread not safe

		// read
		void	ImmReadForward( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl );
		void	ImmReadBackward( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl );
		void	ImmReadBegin( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl );
		void	ImmReadEnd( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl );
		void	ImmReadByHndlFw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		void	ImmReadByHndlBw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		void	ImmReadByMinFw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, Def_GrTime A_Time );
		void	ImmReadByMinBw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, Def_GrTime A_Time );

		void	SafeReadForward( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl );
		void	SafeReadBackward( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl );
		void	SafeReadBegin( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl );
		void	SafeReadEnd( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl );
		void	SafeReadByHndlFw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		void	SafeReadByHndlBw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		void	SafeReadByMinFw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, Def_GrTime A_Time );
		void	SafeReadByMinBw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, Def_GrTime A_Time );

		// runtime
		Cls_GrDvrFs2Disk*	RtlDiskObjGet( __u8 A_Idx );

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


