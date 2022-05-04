/*
	solo record play
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrPlaySoloV0
#define	_EPC_GrPlaySoloV0

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<Dvr/GrDvrBase.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<Hw/Solo/GrDevSolo.h>
#include	<GrBufTool.h>

#include	<Dvr/Fs/V1Old/GrDvrFsoMng.h>
#include	<Dvr/Fs/V1Old/GrDvrFsoSeek.h>

//====================================================================
//constance
#define E_GrPlaySoloV0PlaySrcNormal		0			//normal hdd
#define E_GrPlaySoloV0PlaySrcMirror		1			//mirror hdd
#define E_GrPlaySoloV0PlaySrcInst			2			//instance replay

#define E_GrPlaySoloV0PlayTimeLimSec	10			//over time sec
#define E_GrPlaySoloV0PlayTimeLimMsec	(E_GrPlaySoloV0PlayTimeLimSec * E_GrTimeMiliOfSec)	//over time milsec

#define E_GrPlaySoloV0PlaySpeedMax		4			//max 16 x

//====================================================================
//global macro


//====================================================================
//global type

typedef	struct St_GrPlaySoloV0ChItm 
{
	//BOOL8	IsPlayAble;		//play able
	//BOOL8	IsPfrmPlay;		//p frame play able
	//__u8	VopId;
	//__u8	IfrmSkip;			//i frame skip count
	__u8	SkipCnt;			//display skip count
	__u8	SkipPos;			//display skip pos
	__u8	Res;					//last frame resolution
	__u8	Speed;				//record skip count
	__u32	Pwr;					//power
}	*Ptr_GrPlaySoloV0ChItm;


//====================================================================
//class

class Cls_GrPlaySoloV0	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*		m_TaskSvr;
		Cls_GrDevSolo*		m_ObjDev;
		__u32	m_MsgAloc;

		Cls_GrDvrFsoMng*	m_FsMng;
		Cls_GrDvrFsoSeek*	m_FsSeek;
		Ptr_GrDvrAvHd			m_PtrPprFrm;		//prepared frame pointer
		__u32	m_DecReqed;		//video decode request count
		BOOL8	m_IsSeek;

		//reserved work
		__u8	m_RwkCmd;					//reserved work command
		__u64	m_RwkHndl;				//reserved work handle
		Def_GrTime	m_RwkTime;	//reserved work time

		__u8	m_MaxChCnt;			//max channel count
		__u8	m_AdoCh;				//

		//BOOL8	m_IsPlaying;
		__u32	m_PlaySpeed;		//play shifter
		BOOL8	m_IsStep;
		Def_GrTime	m_TimeSec;
		__u32	m_TimeMsec;

		__u8	m_LastStopStat;

		BOOL8	m_IsTimeLastSm;					//last frame summer time
		Def_GrTime	m_TimeLastSec;		//last frame sec
		__u32	m_TimeLastMsec;					//last frame msec
		__u32	m_TickLast;

		__u32	m_DecPwrFps;		//deocde power fps unit
		__u32	m_VdoSigFps;				//video signal fps

		St_GrPlaySoloV0ChItm	m_ChTbl[E_SoloMaxChCnt];


	protected:
		//local
		void	LcChRtlUpdt( void );		//channel update infomation update
		BOOL8	LcRtlPlay( void );
		BOOL8	LcDecFrame( Ptr_GrDvrAvHd A_PtrFrm );
		BOOL8	LcProcPlayNrm( void );	//normal direction play
		void	LcChSpdUpdt( void );		//channel speed update
		BOOL8	LcProcPlayBack( void );	//normal direction play
		BOOL8	LcIsReadAble( void );	//read able waiting
		void	LcPwrReset( void );			//play power reset
		void	LcAdoAbleUpdt( void );
		void	LcProcRsvdWork( void );
		void	LcSpeedReset( void );

		void	LcProcSourceSet( __u8 A_Source );
		BOOL8	LcProcGoFirst( void );
		BOOL8	LcProcGoLast( void );
		BOOL8	LcProcGoRecent( void );
		BOOL8	LcProcGoTimeMinute( Def_GrTime A_Time );
		BOOL8	LcProcGoByHndl( __u64 A_Hndl );
		BOOL8	LcProcGoTimeSec( Def_GrTime A_Time );
		void	LcProcPlay( void );
		void	LcProcDirSet( BOOL8 A_IsBack );
		void	LcProcSpeedSet( __u32 A_Speed );
		void	LcProcStop( void );

		void	LcProcFrmNext( void );
		void	LcProcFrmPrev( void );

		void	LcDoDispErase( void );

		//inherited
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */
		virtual	BOOL8	RtlCallback( Ptr_GrTaskMsgHd A_PtrMsg );

	public:
		Cls_GrPlaySoloV0( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDevSolo* A_DevSolo, Cls_GrDvrFsoMng* A_FsMng );
		virtual	~Cls_GrPlaySoloV0();

		//info
		__u8	SourceType( void );
		Def_GrTime	TimeGet( void );
		BOOL8	IsSummerTime( void );		//playing data is summer time
		__u32	SpeedGet( void );
		BOOL8	IsDirBack( void );
		BOOL8	IsPlaying( void );
		__u8	LastStopStatGet( void );
		BOOL8	IsSeeking( void );
		BOOL8	IsStepMode( void );

		//control
		void	SourceSet( __u8 A_Source );
		void	GoFirst( void );
		void	GoLast( void );
		void	GoRecent( void );
		void	GoTimeMinute( Def_GrTime A_Time );
		void	GoByHndl( __u64 A_Hnd );
		void	GoTimeSec( Def_GrTime A_Time );

		void	Play( void );
		void	DirectionSet( BOOL8 A_IsBack );
		void	Stop( void );
		void	SpeedSet( __u32 A_Speed );
		void	AudioChSet( __u8 A_Ch );

		void	StepNext( void );
		void	StepPrev( void );

};


//====================================================================
//global function
	
//====================================================================
//out Enviroment conditional
#endif
