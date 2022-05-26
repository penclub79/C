/*
	GAUSS DVR file system v4 play

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs4Play
#define	_EPC_GrDvrFs4Play

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/Fs/V4/GrDvrFs4Dmux.h>
	
//====================================================================
//constance
#define	E_GrDvrFs4PlayTimerPeriod		20		/* 20 ms 50 Hz */

#define E_GrDvrFs4PlayCmdQueSize		0x1000

#define E_GrDvrFs4PlayTimeGapLimite	3			/* 5 sec time gap limit */

#define	E_GrDvrFs4PlaySeekWorkMax		10		/* max time slice count for seek work */
#define	E_GrDvrFs4PlaySeekWorkUnit	(480*2)		/* max seek frame count while time slice */

#define E_GrDvrFs4PlayModeWait			0
#define E_GrDvrFs4PlayModeJmpTime		1		/* jump target time */
#define E_GrDvrFs4PlayModeSeek			2		/* seek frame unit */
#define E_GrDvrFs4PlayModePlay			3		/* playing */
#define E_GrDvrFs4PlayModeNext			4		/* next frame */
#define E_GrDvrFs4PlayModePrev			5		/* prev frame */
#define E_GrDvrFs4PlayModeVdec			6		/* decode now video frame */
#define E_GrDvrFs4PlayModeJmpBegin	7		/* jump to begin */
#define E_GrDvrFs4PlayModeJmpEnd		8		/* jump to end */

#define E_GrDvrFs4PlayCmdDoPlay			1
#define E_GrDvrFs4PlayCmdJmpByTime	2		/* Dw = time */
#define E_GrDvrFs4PlayCmdJmpBegin		5		
#define E_GrDvrFs4PlayCmdJmpEnd			6		
#define E_GrDvrFs4PlayCmdDirSet			7	
#define E_GrDvrFs4PlayCmdVdoChSet		8		/* Dw = channel map  */
#define E_GrDvrFs4PlayCmdAdoChSet		9		/* Dw = channel map  */
#define E_GrDvrFs4PlayCmdSpeedSet		10	/* Dw = speed  */
#define E_GrDvrFs4PlayCmdStepNext		11
#define E_GrDvrFs4PlayCmdStepPrev		12
#define E_GrDvrFs4PlayCmdDevSet			13	/* Bt = record device type */
#define E_GrDvrFs4PlayCmdDoStop			14	

#define	E_GrDvrFs4PlayRsltOk				0
#define	E_GrDvrFs4PlayRsltWait			1	/* wait play time */
#define	E_GrDvrFs4PlayRsltBusy			2	/* device busy */
#define	E_GrDvrFs4PlayRsltFail			3	/* fail */


//	#define DAMGI_DBG

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs4PlayCmdItem 
{
	__u8	Cmd;
	__u8	Bt;				// byte para
	__u16	Wd;				// word para
	__u32	Dw;				// dword para
	__u64	Hndl;			// handle
}	*Ptr_GrDvrFs4PlayCmdItem;

typedef	struct St_GrDvrFs4PlayChItem 
{
	__u8	DispStep;
	__u8	DispPrgs;

}	*Ptr_GrDvrFs4PlayChItem;


//====================================================================
//class
class	Cls_GrDvrFs4Play	:	public	Cls_GrDvrFs4Dmux
{
	protected:
		// property
		__u32		m_HndlTimer;

		__u8		m_PlayMode;
		BOOL8		m_IsPlayDirBack;	// play direction
		__u8		m_PlayStat;
		BOOL8		m_IsPlaying;

		__u8		m_PlayDispFps;		// play display max fps - owner init
		__u32		m_PlayPower;			// play device power - owner init

		__s32		m_PlaySpeed;			// 0 == normal, positive = fast, negative = slow
		__s32		m_PlaySpdMin;			// 0 ~ -
		__s32		m_PlaySpdMax;			// 1 ~ +

		__u32		m_PlayVdoMap;			// play video map
		__u32		m_PlayAdoMap;			// play audio map
		
		Def_GrTime	m_PlayTmSec;	// running play time
		__u32		m_PlayTmMsec;
		BOOL8		m_IsPlayTmSm;
		BOOL8		m_IsPlayTmSync;	// request play time sync
		BOOL8		m_IsPlayNoWait;	// no wait mode
		__u32		m_PlayTick;			// last tick

		St_GrDvrFs4PlayChItem	m_PlayChTbl[E_GrDvrMaxChCnt];

		__u8		m_PlayCmdQue[E_GrDvrFs4PlayCmdQueSize];

		__u32		m_PlaySeekPrgs;	// seek frame progress
		Def_GrTime	m_PlaySeekTgTime;	// seek target time

	#ifdef	DAMGI_DBG
		__u32		m_DbgSeekCnt;
	#endif

		// method
		void	LcPlayTimerTmUpdt(void);

		__u8	LcPlayRunFw(void);
		__u8	LcPlayRunBw(void);

		__u32	LcPlayCalcPowerByDmuxMode(__u8 A_DmuxMode);

		void	LcPlayDmuxModeUpdt(void);
		void	LcPlayDispSkipChUpdt(__u8 A_Ch);
		void	LcPlayDispSkipAllUpdt(void);

		virtual	BOOL8	LcPlayPumpCmd(void);	// parsing play command
		void	LcPlayCmdPlay(void);
		void	LcPlayCmdJumpByTime(Def_GrTime A_Time);
		void	LcPlayCmdStepNext(void);
		void	LcPlayCmdStepPrev(void);
		void	LcPlayCmdJmpBegin(void);
		void	LcPlayCmdJmpEnd(void);
		void	LcPlayCmdSpeedSet(__s32 A_Speed);
		void	LcPlayCmdDirSet(BOOL8 A_IsBack);
		void	LcPlayCmdVdoChSet(__u32 A_Map);
		void	LcPlayCmdAdoChSet(__u32 A_Map);
		void	LcPlayCmdDevSet(__u8 A_DevType);
		void	LcPlayCmdStop(void);

		BOOL8	LcPlayModeWait(void);
		BOOL8	LcPlayModeJmpTime(void);
		BOOL8	LcPlayModeSeek(void);
		BOOL8	LcPlayModePlay(void);
		BOOL8	LcPlayModeNext(void);
		BOOL8	LcPlayModePrev(void);
		BOOL8	LcPlayModeVdec(void);
		BOOL8	LcPlayModeJmpBegin(void);
		BOOL8	LcPlayModeJmpEnd(void);

		// can inherit
		virtual	__u8	DecVdo(Ptr_GrDvrFrmVdo A_PtrHd, BOOL8 A_IsDisp);
		virtual	__u8	DecAdo(Ptr_GrDvrFrmAdo A_PtrHd);

		// inherited
		virtual	void	EvtVdoFmtChg(__u8 A_Ch);	// video foramt change - from GrDvrFs4Dmux

		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */
		virtual	void	RtlTimer( __u32 A_Hndl );
	public:
		Cls_GrDvrFs4Play( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFs4Mng* A_FsMng, BOOL8 A_IsSafe );
		virtual		~Cls_GrDvrFs4Play();

		// info
		__u8	PlayStatusGet( void );
		__s32		PlaySpeedGet( void );
		Def_GrTime	PlayTimeGet( void );
		BOOL8	IsPlayTimeSm( void );
		BOOL8	IsPlayDirBack( void );
		__u8	PlayDevTypeGet( void );

		// control
		void	PlayDevTypeSet( __u8 A_DevType );
		void	PlayDo( void );
		void	PlayStop( void );
		void	PlayStepNext( void );
		void	PlayStepPrev( void );
		void	PlayVideoChEnable( __u32 A_Map );
		void	PlayAudioChEnable( __u32 A_Map );
		void	PlayDirSet( BOOL8 A_IsBack );
		void	PlaySpeedSet( __s32 A_Speed );

		void	PlayGoBegin( void );
		void	PlayGoEnd( void );
		void	PlayGoTime( Def_GrTime A_Time );

};

	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


