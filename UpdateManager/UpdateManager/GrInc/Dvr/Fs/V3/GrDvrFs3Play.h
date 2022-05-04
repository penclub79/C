/*
	GAUSE DVR file system v2 play

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs3Play
#define	_EPC_GrDvrFs3Play

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/Fs/V3/GrDvrFs3Dmux.h>
	
//====================================================================
//constance
#define E_GrDvrFs3PlayCmdQueSize		0x1000

#define E_GrDvrFs3PlayDecOk					0		/* decode success */
#define E_GrDvrFs3PlayDecBusy				1		/* decoder busy */
#define E_GrDvrFs3PlayDecFail				2		/* decode failure */

#define E_GrDvrFs3PlayTimeGapLimite	5		/* 5 sec time gap limit */

#define E_GrDvrFs3PlayRunModeWait		0
#define E_GrDvrFs3PlayRunModeSeek		1
#define E_GrDvrFs3PlayRunModePlay		2
#define E_GrDvrFs3PlayRunModeNext		3		/* next frame */
#define E_GrDvrFs3PlayRunModePrev		4		/* prev frame */

#define E_GrDvrFs3PlayCmdDoPlay			1
#define E_GrDvrFs3PlayCmdJmpByTime	2		/* Dw = time, Bt = seek after play */
#define E_GrDvrFs3PlayCmdJmpByHndl	3		/* Hndl = handle, Bt = seek after play */
#define E_GrDvrFs3PlayCmdJmpByCnt		4		/* Dw = frame count, Bt = seek after play */
#define E_GrDvrFs3PlayCmdJmpBegin		5		/* Bt = seek after play */
#define E_GrDvrFs3PlayCmdJmpEnd			6		/* Bt = seek after play */
#define E_GrDvrFs3PlayCmdDirSet			7	
#define E_GrDvrFs3PlayCmdVdoChSet		8		/* Wd = channel map  */
#define E_GrDvrFs3PlayCmdAdoChSet		9		/* Wd = channel map  */
#define E_GrDvrFs3PlayCmdSpeedSet		10	/* Dw = speed  */
#define E_GrDvrFs3PlayCmdStepNext		11
#define E_GrDvrFs3PlayCmdStepPrev		12
#define E_GrDvrFs3PlayCmdDevSet			13	/* Bt = record device type */
#define E_GrDvrFs3PlayCmdDoStop			14	
#define E_GrDvrFs3PlayCmdIfrmOnly		15	/* Wd = channel map */
#define E_GrDvrFs3PlayCmdNoTimeSync	16	/* Bt = no time sync mode set */

#define E_GrDvrFs3PlaySpeedMjBase		4

#define E_GrDvrFs3PlaySpeedMax			6

//	#define DAMGI_DBG

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs3PlayCmdItem 
{
	__u8	Cmd;
	__u8	Bt;				// byte para
	__u16	Wd;				// word para
	__u32	Dw;				// dword para
	__u64	Hndl;			// handle
}	*Ptr_GrDvrFs3PlayCmdItem;

typedef	struct St_GrDvrFs3PlayChItem 
{
	BOOL8	IsVdoAble;
	BOOL8	IsAdoAble;
	BOOL8	IsIonly;			// i frame only

	// runtime
	BOOL8	IsPable;			// video P-frame able
	__u8	DispPos;
	__u8	DropPos;
	__u8	SeqId;

	// now status
	__u8	Res;
	__u8	Speed;				// skip count
	__u8	DispStep;
	__u32	Pwr;					// channel power
}	*Ptr_GrDvrFs3PlayChItem;


//====================================================================
//class
class	Cls_GrDvrFs3Play	:	public	Cls_GrDvrFs3Dmux
{
	protected:
		// property

		__u8		m_RunMode;
		BOOL8		m_IsDirBack;		// play direction
		__u8		m_PlayStat;
		BOOL8		m_IsSeekCnt;

		__u8		m_VdoEnChCnt;		// video enable channel count
		BOOL8		m_IsDecoded;		// decoded
		__u8		m_DropCnt;			// drop count
		__u8		m_PlayMaxPos;		// max speed play position

		__u8		m_Gop;					// GOP
		BOOL8		m_IsSmTime;			// play time is summer time
		BOOL8		m_IsNoTimeSync;		// time sync not use
		BOOL8		m_IsJumpAfterSeek;

		BOOL8		m_IsPlaying;

		__u32		m_DecPower;			// decoding power
		__u32		m_VdoSigFps;		// video signal fps

		__s32			m_PlaySpeed;		// 0 == normal, positive = fast, negative = slow

		__u32		m_SeekFrmCnt;
		Def_GrTime	m_SeekTime;

	#ifdef	DAMGI_DBG
		__u32		m_DbgSeekCnt;
	#endif

		Def_GrTime	m_PlayLast;		// last play time

		Def_GrTime	m_PlaySec;
		__u32	m_PlayMsec;
		__u32		m_PlayTick;		// last tick

		__u8		m_CmdQue[E_GrDvrFs3PlayCmdQueSize];

		St_GrDvrFs3PlayChItem	m_ChTbl[E_GrDvrMaxChCnt];

		// method
		void	LcPableReset( void );

		virtual	BOOL8	LcParsPlayCmd( void );	// parsing play command

		void	LcSpdChUpdt( __u8 A_Ch, __u8 A_Res, __u8 A_Speed );
		void	LcSpdParaUpdt( void );
		void	LcDispParaUpdt( void );
		void	LcVdoChEnSet( __u16 A_Map );
		void	LcAdoChEnSet( __u16 A_Map );
		void	LcIonlyChEnSet( __u16 A_Map );
		void	LcJumpByTime( Def_GrTime A_Time );
		void	LcDoPlay( void );

		BOOL8	LcPlayForward( void );
		BOOL8	LcPlayBackward( void );
		BOOL8	LcPlayMaxFw( void );
		BOOL8	LcPlayMaxBw( void );

		BOOL8	LcRunModeWait( void );
		BOOL8	LcRunModeSeek( void );
		BOOL8	LcRunModePlay( void );
		BOOL8	LcRunModeNext( void );
		BOOL8	LcRunModePrev( void );

		virtual	__u8	DecVdo( Ptr_GrDvrAvHd A_PtrHd, BOOL8 A_IsDisp );
		virtual	__u8	DecAdo( Ptr_GrDvrAvHd A_PtrHd );

		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */
		// virtual	void	RtlTimer( __u32 A_Hndl );
	public:
		Cls_GrDvrFs3Play( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFs3Mng* A_FsMng, Ptr_GrDvrFs3SeekCtrl A_PtrSeekCtrl, BOOL8 A_IsSafe );
		virtual		~Cls_GrDvrFs3Play();

		// info
		__u8	PlayStatusGet( void );
		__s32		PlaySpeedGet( void );
		Def_GrTime	PlayTimeGet( void );
		BOOL8	IsSummerTime( void );
		BOOL8	IsDirBack( void );
		__u8	RecDevTypeGet( void );

		// control
		void	RecDevTypeSet( __u8 A_DevType );
		void	DoPlay( void );
		void	DoStop( void );
		void	MoveNext( void );
		void	MovePrev( void );
		void	VideoChEnable( __u16 A_Map );
		void	AudioChEnable( __u16 A_Map );
		void	IframeOnly( __u16 A_Map );
		void	PlayDirSet( BOOL8 A_IsBack );
		void	PlaySpeedSet( __s32 A_Speed );

		void	JumpBegin( void );
		void	JumpEnd( void );
		void	JumpByTime( Def_GrTime A_Time );
		void	JumpByHndl( __u64 A_Hndl );
		void	JumpByCnt( __u32 A_Cnt );

		void	NoTimeSyncSet( BOOL8 A_IsNoSync );
};

	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


