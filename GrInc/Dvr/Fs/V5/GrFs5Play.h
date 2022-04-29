/*
File system 5 play
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrFs5Play
#define	_EPC_GrFs5Play

//====================================================================
// option
//#define	E_GrFs5PlayDbgMsg			/* show debug message */
#define	E_GrFs5PlayErrMsg			/* show error message */
//#define	E_GrFs5PlayTimeDirect	/* play time display by now frame time mode */
// #define E_GrFs5PlayShowFps		/* display decode fraem rate - not implemented */ 


//====================================================================
// uses
#ifdef LINUX_EMU
#include <Win/MfcDock/GrMfcDockBase.h>
#endif

#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrTimeTool.h>
#include <Dvr/Fs/V5/GrFs5Dmux.h>

//====================================================================
//const
#define	E_GrFs5PlayTimerPeriod							100		/* 100 ms */

#define	E_GrFs5PlayGapTimeLimite						5

#define	E_GrFs5PlayPwrResShift							8		/* resolution power right shifter */

#define	E_GrFs5PlayBufMaxCnt								2

#define	E_GrFs5PlayCacheIdAdder							1000							

// play sync mode
#define	E_GrFs5PlaySyncOff									0
#define	E_GrFs5PlaySyncAuto									1
#define	E_GrFs5PlaySyncAll									2
#define	E_GrFs5PlaySync2x										3
#define	E_GrFs5PlaySync4x										4
#define	E_GrFs5PlaySyncIonly								5

// message command
#define	E_GrFs5PlayMsgCmdVdoChMapSet				1
#define	E_GrFs5PlayMsgCmdIsPlaying					2
#define	E_GrFs5PlayMsgCmdPlay								3
#define	E_GrFs5PlayMsgCmdPause							4
#define	E_GrFs5PlayMsgCmdGoByTime						5
#define	E_GrFs5PlayMsgCmdSpeedSet						6
#define	E_GrFs5PlayMsgCmdSpeedGet						7
#define	E_GrFs5PlayMsgCmdDirSet							8
#define	E_GrFs5PlayMsgCmdDirGet							9
#define	E_GrFs5PlayMsgCmdAdoChSet						10
#define	E_GrFs5PlayMsgCmdStepForward				11
#define	E_GrFs5PlayMsgCmdStepBackward				12
#define	E_GrFs5PlayMsgCmdStatGet						13
#define	E_GrFs5PlayMsgCmdAdoChMapSet				15
#define	E_GrFs5PlayMsgCmdSyncSet						16

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrFs5PlayChCtl
{
	// display skip count
	__u8	DskipStep;	// display skip step 0 == not use
	__u8	DskipPrgs;	// display skip progress
	__u32	CorFps;			// correction frame rate
	__u32	NsPwr;			// normal speed power
	__u32	IfPwr;			// I frame power
}	*Ptr_GrFs5PlayChCtl;

typedef struct St_GrFs5PlayMsgBase
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrFs5PlayMsgBase;

typedef struct St_GrFs5PlayMsgVdoChMapSet
{
	St_GrFs5PlayMsgBase	Base;
	__u32	Map;
}	*Ptr_GrFs5PlayMsgVdoChMapSet;

typedef struct St_GrFs5PlayMsgIsPlaying
{
	St_GrFs5PlayMsgBase	Base;
	BOOL8*	PtrRslt;
}	*Ptr_GrFs5PlayMsgIsPlaying;

typedef struct St_GrFs5PlayMsgStatGet
{
	St_GrFs5PlayMsgBase	Base;
	__u8*	PtrRslt;
}	*Ptr_GrFs5PlayMsgStatGet;

typedef struct St_GrFs5PlayMsgGoByTime
{
	St_GrFs5PlayMsgBase	Base;
	Def_GrErrCode*	PtrRslt;
	Def_GrTime	Time;
}	*Ptr_GrFs5PlayMsgGoByTime;

typedef struct St_GrFs5PlayMsgGoByLog
{
	St_GrFs5PlayMsgBase	Base;
	Ptr_GrDvrLogItm	PtrLog;
}	*Ptr_GrFs5PlayMsgGoByLog;

typedef struct St_GrFs5PlayMsgSpeedSet
{
	St_GrFs5PlayMsgBase	Base;
	__s32	Val;
}	*Ptr_GrFs5PlayMsgSpeedSet;

typedef struct St_GrFs5PlayMsgSpeedGet
{
	St_GrFs5PlayMsgBase	Base;
	__s32*	PtrRslt;
}	*Ptr_GrFs5PlayMsgSpeedGet;

typedef struct St_GrFs5PlayMsgDirSet
{
	St_GrFs5PlayMsgBase	Base;
	BOOL8	IsBack;
}	*Ptr_GrFs5PlayMsgDirSet;

typedef struct St_GrFs5PlayMsgDirGet
{
	St_GrFs5PlayMsgBase	Base;
	BOOL8*	PtrRslt;
}	*Ptr_GrFs5PlayMsgDirGet;

typedef struct St_GrFs5PlayMsgAdoChSet
{
	St_GrFs5PlayMsgBase	Base;
	__s32		AdoCh;
}	*Ptr_GrFs5PlayMsgAdoChSet;

typedef struct St_GrFs5PlayMsgAdoChMapSet
{
	St_GrFs5PlayMsgBase	Base;
	__u32	Map;
}	*Ptr_GrFs5PlayMsgAdoChMapSet;

typedef struct St_GrFs5PlayMsgSyncSet
{
	St_GrFs5PlayMsgBase	Base;
	__u8	Mode;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrFs5PlayMsgSyncSet;


//====================================================================
//class

class Cls_GrFs5Play	:	public	Cls_GrFs5Dmux
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

#ifdef E_GrFs5PlayTimeDirect
		Def_GrTime	m_PlayTmLast;
#endif

		__u32	m_PlayTickLast;
		__u32	m_PlayTickBefore;
		__u32	m_PlaySpdShif;

		St_GrFs5PlayChCtl	m_PlayChTbl[E_GrFs5MaxChCnt];

#ifdef E_GrFs5PlayShowFps
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

		// need override
		virtual	__u8	PlayVdoDec(__u8 A_Ch, void* A_PtrStrm, __u32 A_StrmSize, BOOL8 A_IsDisp) = 0;
		virtual	__u8	PlayAdoDec(__u8 A_Ch, void* A_PtrStrm, __u32 A_StrmSize) = 0;
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
		Cls_GrFs5Play( Cls_GrTaskSvr* A_TaskSvr, __u32 A_TryCnt, __u32 A_MaxFrmSize);
		virtual	~Cls_GrFs5Play();

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

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

