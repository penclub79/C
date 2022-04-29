/*
File system 5 play
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrFsNvtPlay
#define	_EPC_GrFsNvtPlay

//====================================================================
// option
//#define	E_GrFsNvtPlayDbgMsg			/* show debug message */
#define	E_GrFsNvtPlayErrMsg			/* show error message */
//#define	E_GrFsNvtPlayTimeDirect	/* play time display by now frame time mode */
// #define E_GrFsNvtPlayShowFps		/* display decode fraem rate - not implemented */ 


//====================================================================
// uses
#ifdef LINUX_EMU
#include <Win/MfcDock/GrMfcDockBase.h>
#endif

#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrTimeTool.h>
#include <Dvr/Fs/Nvt/GrFsNvtDmux.h>

//====================================================================
//const
#define	E_GrFsNvtPlayTimerPeriod							100		/* 100 ms */

#define	E_GrFsNvtPlayGapTimeLimite						5

#define	E_GrFsNvtPlayPwrResShift							8		/* resolution power right shifter */

#define	E_GrFsNvtPlayBufMaxCnt								2

#define	E_GrFsNvtPlayCacheIdAdder							1000							

// play sync mode
#define	E_GrFsNvtPlaySyncOff									0
#define	E_GrFsNvtPlaySyncAuto									1
#define	E_GrFsNvtPlaySyncAll									2
#define	E_GrFsNvtPlaySync2x										3
#define	E_GrFsNvtPlaySync4x										4
#define	E_GrFsNvtPlaySyncIonly								5

// message command
#define	E_GrFsNvtPlayMsgCmdVdoChMapSet				1
#define	E_GrFsNvtPlayMsgCmdIsPlaying					2
#define	E_GrFsNvtPlayMsgCmdPlay								3
#define	E_GrFsNvtPlayMsgCmdPause							4
#define	E_GrFsNvtPlayMsgCmdGoByTime						5
#define	E_GrFsNvtPlayMsgCmdSpeedSet						6
#define	E_GrFsNvtPlayMsgCmdSpeedGet						7
#define	E_GrFsNvtPlayMsgCmdDirSet							8
#define	E_GrFsNvtPlayMsgCmdDirGet							9
#define	E_GrFsNvtPlayMsgCmdAdoChSet						10
#define	E_GrFsNvtPlayMsgCmdStepForward				11
#define	E_GrFsNvtPlayMsgCmdStepBackward				12
#define	E_GrFsNvtPlayMsgCmdStatGet						13
#define	E_GrFsNvtPlayMsgCmdAdoChMapSet				15
#define	E_GrFsNvtPlayMsgCmdSyncSet						16

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrFsNvtPlayChCtl
{
	// display skip count
	__u8	DskipStep;	// display skip step 0 == not use
	__u8	DskipPrgs;	// display skip progress
	__u32	CorFps;			// correction frame rate
	__u32	NsPwr;			// normal speed power
	__u32	IfPwr;			// I frame power
}	*Ptr_GrFsNvtPlayChCtl;

typedef struct St_GrFsNvtPlayMsgBase
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrFsNvtPlayMsgBase;

typedef struct St_GrFsNvtPlayMsgVdoChMapSet
{
	St_GrFsNvtPlayMsgBase	Base;
	__u32	Map;
}	*Ptr_GrFsNvtPlayMsgVdoChMapSet;

typedef struct St_GrFsNvtPlayMsgIsPlaying
{
	St_GrFsNvtPlayMsgBase	Base;
	BOOL8*	PtrRslt;
}	*Ptr_GrFsNvtPlayMsgIsPlaying;

typedef struct St_GrFsNvtPlayMsgStatGet
{
	St_GrFsNvtPlayMsgBase	Base;
	__u8*	PtrRslt;
}	*Ptr_GrFsNvtPlayMsgStatGet;

typedef struct St_GrFsNvtPlayMsgGoByTime
{
	St_GrFsNvtPlayMsgBase	Base;
	Def_GrErrCode*	PtrRslt;
	Def_GrTime	Time;
}	*Ptr_GrFsNvtPlayMsgGoByTime;

typedef struct St_GrFsNvtPlayMsgGoByLog
{
	St_GrFsNvtPlayMsgBase	Base;
	Ptr_GrDvrLogItm	PtrLog;
}	*Ptr_GrFsNvtPlayMsgGoByLog;

typedef struct St_GrFsNvtPlayMsgSpeedSet
{
	St_GrFsNvtPlayMsgBase	Base;
	__s32	Val;
}	*Ptr_GrFsNvtPlayMsgSpeedSet;

typedef struct St_GrFsNvtPlayMsgSpeedGet
{
	St_GrFsNvtPlayMsgBase	Base;
	__s32*	PtrRslt;
}	*Ptr_GrFsNvtPlayMsgSpeedGet;

typedef struct St_GrFsNvtPlayMsgDirSet
{
	St_GrFsNvtPlayMsgBase	Base;
	BOOL8	IsBack;
}	*Ptr_GrFsNvtPlayMsgDirSet;

typedef struct St_GrFsNvtPlayMsgDirGet
{
	St_GrFsNvtPlayMsgBase	Base;
	BOOL8*	PtrRslt;
}	*Ptr_GrFsNvtPlayMsgDirGet;

typedef struct St_GrFsNvtPlayMsgAdoChSet
{
	St_GrFsNvtPlayMsgBase	Base;
	__s32		AdoCh;
}	*Ptr_GrFsNvtPlayMsgAdoChSet;

typedef struct St_GrFsNvtPlayMsgAdoChMapSet
{
	St_GrFsNvtPlayMsgBase	Base;
	__u32	Map;
}	*Ptr_GrFsNvtPlayMsgAdoChMapSet;

typedef struct St_GrFsNvtPlayMsgSyncSet
{
	St_GrFsNvtPlayMsgBase	Base;
	__u8	Mode;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFsNvtPlayMsgSyncSet;


//====================================================================
//class

class Cls_GrFsNvtPlay	:	public	Cls_GrFsNvtDmux
{
	protected:

		__u32	m_MsgAloc;
		__u32	m_HndlTimer;

		__u32	m_PlayChCnt;				// need set
		__u32	m_PlayTryCnt;				// work try count

		__u8	m_PlaySyncMode;
		BOOL8	m_IsPlayRun;
		BOOL8	m_IsPlayFrmUsed;	// used frame
		Def_GrTime	m_PlayTimeSec;
		__u32	m_PlayTimeMili;

#ifdef E_GrFsNvtPlayTimeDirect
		Def_GrTime	m_PlayTmLast;
#endif

		__u32	m_PlayTickLast;
		__u32	m_PlayTickBefore;
		__u32	m_PlaySpdShif;

		St_GrFsNvtPlayChCtl	m_PlayChTbl[E_GrFsNvtMaxChCnt];

#ifdef E_GrFsNvtPlayShowFps
		__u32	m_Fps;
#endif
		//local
		BOOL8	LcProcPlay(void);

		// normal speed
		BOOL8	LcSubPlayFw(void);
		BOOL8	LcSubPlayBw(void);

		// 30 sec step
		BOOL8	LcSubPlayS1Fw(void);

		BOOL8	LcPlayDecNowFrmFw(BOOL8 A_IsImmUpdt);
		BOOL8	LcPlayDecNowFrmBw(BOOL8 A_IsImmUpdt);

		void	LcPlayNowFrmFlush(void);

		void	LcPlayPrgsTimeSet(Def_GrTime A_Time, __u32 A_Mili);

		void	LcPlayPwrChUpdate(__u8 A_Ch);

		void	LcTmrPlayTime(void);

		void	LcPlayTimeGet(void);

		// need override
#ifndef DVR_NVT
		virtual	__u8	PlayVdoDec(__u8 A_Ch, void* A_PtrStrm, __u32 A_StrmSize, BOOL8 A_IsDisp) = 0;
		virtual	__u8	PlayAdoDec(__u8 A_Ch, void* A_PtrStrm, __u32 A_StrmSize) = 0;
#else
		virtual	__u8	PlayVdoDec(__u8 A_Ch, void* A_PtrStrm, __u32 A_StrmSize, BOOL8 A_IsDisp, __s32 A_StrmBufOfs = 0) = 0;
		virtual	__u8	PlayAdoDec(__u8 A_Ch, void* A_PtrStrm, __u32 A_StrmSize, __s32 A_StrmBufOfs = 0) = 0;
#endif
		
		virtual	void	PlayTimeUpdate(Def_GrTime A_Time);
		virtual	void	PlayPwrCtlUpdate(void);

		//inherited
		virtual	void	EvtVdoFmtChg(__u8 A_Ch);	// video foramt change
		virtual	void	LoadBefore(void);
		virtual	void	LoadAfter(void);

		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	void	RtlTimer( __u32 A_Hndl );
		virtual	BOOL8	RtlBkgProc(void);											/* return = background processed */
	public:
		Cls_GrFsNvtPlay( Cls_GrTaskSvr* A_TaskSvr, __u32 A_TryCnt, __u32 A_MaxFrmSize);
		virtual	~Cls_GrFsNvtPlay();

		Def_GrErrCode	ImmGoByTime(Def_GrTime A_Time);
		void	ImmPlay(void);
		void	ImmPause(void);
		void	ImmVdoChMapSet(__u32 A_Map);
		void	ImmAdoChMapSet(__u32 A_Map);
		void	ImmAdoChSet(__s32 A_Ch);
		BOOL8	ImmIsPlaying(void);
		__u8	ImmPlayStatGet(void);
		void	ImmPlaySpeedSet(__s32 A_Spd);
		__s32	ImmPlaySpeedGet(void);
		void	ImmPlayDirSet(BOOL8 A_IsBack);
		BOOL8	ImmPlayDirGet(void);
		void	ImmStepForward(void);
		void	ImmStepBackward(void);
		void	ImmPlaySyncSet(__u8 A_Mode);
		
		void	ReqVdoChMapSet(__u32 A_Map);
		void	ReqAdoChMapSet(__u32 A_Map);
		BOOL8	ReqIsPlaying(void);
		__u8	ReqPlayStatGet(void);
		void	ReqPlay(void);
		void	ReqPause(void);
		Def_GrErrCode	ReqGoByTime(Def_GrTime A_Time);
		void	ReqPlaySpeedSet(__s32 A_Spd);
		__s32	ReqPlaySpeedGet(void);
		void	ReqPlayDirSet(BOOL8 A_IsBack);
		BOOL8	ReqPlayDirGet(void);
		void	ReqAdoChSet(__s32 A_Ch);
		void	ReqStepForward(void);
		void	ReqStepBackward(void);
		void	ReqPlaySyncSet(__u8 A_Mode);

		//void	DecBufSet(__s32 A_MemOfs, __s32 A_DecOfs, __s32 A_Size);
};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

