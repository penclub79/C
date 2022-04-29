/*
	DVR file system version 1 manager

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs1Mng
#define	_EPC_GrDvrFs1Mng

//====================================================================
// uses
#include <GrTypeBase.h>
#include <Dvr/Fs/V1/GrDvrFs1Grp.h>
#include <Dvr/Fs/V1/GrDvrFs1Base.h>
#include <GrBufTool.h>
#include <GrTaskSvr.h>
#include <GrTaskCli.h>
	
//====================================================================
//constance


//mode
#define E_GrDvrFs1CfgDskNormal		E_GrDvrRecDevNoraml
#define E_GrDvrFs1CfgDskMirror		E_GrDvrRecDevMirror
#define E_GrDvrFs1CfgDskHalt			2
#define E_GrDvrFs1CfgDskNone			3

#define E_GrDvrFs1CfgDskModeCnt		3

#define E_GrDvrFs1VssItmSize			32

#define E_GrDvrFs1FlushPeriod			60000


//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrDvrFs1VssItm
{
	__u64	HndlRec;		//record handle
	Def_GrTime	Time;	//armed time
	__u8	Ch;
	__u8	Cmd;
	BOOL8	IsArmed;
	__u8	Etty;
	__u8	Seq;
	__u8	FrmCnt;
	__u8	_rsvd0[2];
	//__u32	_rsvd1[3];
}	*Ptr_GrDvrFs1VssItm;

//====================================================================
//class

class	Cls_GrDvrFs1Mng	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32		m_MsgAloc;
		__u32		m_HndlTimer;

		__u32		m_VssSize;
		__u32		m_TmpBuf[E_GrDvrFs1ElaSize >> 2];
		__u32		m_GrpBuf[E_GrDvrFs1GrpBufSize >> 2];	// group buffer

		BOOL8		m_IsDiskFull;
		BOOL8		m_IsRecAble;
		BOOL8		m_IsReced;
		BOOL8		m_IsVssAble;
		BOOL8		m_IsVssUse;

		__u8		m_MissMap;			// setup miss hdd map
		__u8		m_NotFmtMap;		// not format hdd map

		BOOL8		m_IsWdgTick;		// watchdog tick

		St_GrDvrFs1DskCfg	m_DskCfg;		//disk configuration
		//auto delete
		__u32		m_AutoDelTime;
		Def_GrTime	m_AtdLimTime;		//autodelete limite time

		Cls_GrDvrFs1Grp*	m_GrpTbl[E_GrDvrFs1CfgDskModeCnt];
		Ptr_GrBufQue	m_PtrVssQue;		//not null = use web contorl

		St_GrDvrRecMinThumb	m_TmaTmp[E_GrDvrFs1TmaTmpCnt];	//tma read tmp
	protected:
		//local
		void	LcFreeAllObject( void );
		BOOL8	LcFormat( __u8 A_DskIdx, BOOL8 A_IsWithoutLog );
		BOOL8	LcTmaRead( __u8 A_DevType, Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt );
		void	LcRecStatusGet( Ptr_GrDvrFs1RecStat A_PtrStatTbl );		//must reset table before excute function , must syncronize
		void	LcSetup( __u8* A_PtrDskTbl, __u8 A_Cnt, BOOL8 A_IsNormalOvwt, BOOL8 A_IsMirrorOvwt );			//disk enable map
		BOOL8	LcUcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcUcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcUwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcUwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );
		//BOOL8	LcRecTimeRangeGet( __u8 A_DevType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd );
		void	LcEvtLogAdd( Ptr_GrDvrLogItm A_PtrItem );
		__u32	LcEvtLogGet( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt );
		void	LcAutoDelTimeUpdt( void );
		void	LcAutoDelTimeSet( __u32 A_DayCnt );
		__u8	LcTempGet( __u8 A_DskIdx );
		BOOL8	LcVssItemGet( __u64* A_PtrRtHndl, Def_GrTime* A_PtrRtTime, __u8* A_PtrRtCh, __u8* A_PtrRtCmd, 
			BOOL8* A_PtrRtIsArmed, __u8*	A_PtrRtEtty, __u8* A_PtrRtSeq, __u8* A_PtrRtFrmCnt );	
		void	LcVssItemPop( void );
		void	LcVssReset( void );

		//event
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	void	RtlTimer( __u32 A_Hndl );

	public:
    Cls_GrDvrFs1Mng( Cls_GrTaskSvr* A_TaskSvr, __u32 A_SysId, __u32 A_FccStrm, Def_GrTime A_TimeBase, 
			__u32 A_TmaCnt, BOOL8 A_IsVssUse, BOOL8 A_IsInstReplay, __u64 A_UwaSize, __u32 A_VssSize );
    virtual		~Cls_GrDvrFs1Mng();

		//stauts
		void	RecStatusGet( Ptr_GrDvrFs1RecStat A_PtrStatTbl );		//must reset table before excute function , thread free
		__u64	RecBeginHndlGet( __u8 A_DevType );
		__u64	SafeRecBeginHndlGet( __u8 A_DevType );		//thread safe
		__u64	RecEndHndlGet( __u8 A_DevType );
		__u64	SafeRecEndHndlGet( __u8 A_DevType );			//thread safe
		__u64	RecHndlByTimeMin( __u8 A_DevType, Def_GrTime A_Time );
		__u64	SafeRecHndlByTimeMin( __u8 A_DevType, Def_GrTime A_Time );	//thread safe
		__u64	FindHndlByMinFw( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );
		__u64	SafeFindHndlByMinFw( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );
		__u64	FindHndlByMinBw( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );
		__u64	SafeFindHndlByMinBw( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );
		__u64	UwaDiskPosGet( void );
		__u8	UwaDiskIndexGet( void );
		BOOL8	RecTimeRangeGet( __u8 A_DevType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd );	//thread safe only
		BOOL8	SafeRecTimeRangeGet( __u8 A_DevType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd );	//thread safe only
		__u32	FccStreamGet( void );
		Def_GrTime	AutoDelLimTimeGet( void );
		BOOL8	IsDiskFull();
		BOOL8	IsRecAble( void );
		BOOL8	IsRecorded( BOOL8 A_IsReset );		//normal recorded
		BOOL8	IsWdgChk( BOOL8 A_IsReset );		// watchdog check
		__u8	MissMapGet( void );
		__u8	NotFormatMapGet( void );
		BOOL8	IsVssAble( void );
		__u64	CalcDataSize( __u8 A_DevType, __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );
		__u64	SafeCalcDataSize( __u8 A_DevType, __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );
		BOOL8	IsMainDskAllErr( void );
		BOOL8	IsSafeMainDskAllErr( void );

		// maintenance
		void	DirvePathSet( __u8 A_Idx, __s8* A_StrPath );
		void	Setup( __u8* A_PtrDskTbl, __u8 A_Cnt, BOOL8 A_IsNormalOvwt, BOOL8 A_IsMirrorOvwt );			//disk enable map, thread free
		BOOL8	Format( __u8 A_DskIdx, BOOL8 A_IsWithoutLog );		//thread free

		//io
		BOOL8	PacketAdd( Ptr_GrDvrAvHd A_PtrPkt, __u64* A_PtrRtHndl = NULL );		//input bi direct packet
		BOOL8	PacketReserve( __u32 A_Size, __u64* A_PtrRtHndlNrm, __u64* A_PtrRtHndlMir );		//packet area reserve - do not use by video packet
		BOOL8	DirectWrite( __u8 A_DevType, __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size );
		void	EvtLogAdd( Ptr_GrDvrLogItm A_PtrItem );	//thread free
		__u32	EvtLogGet( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt );	//thread free
		__u8	TemperatureGet( __u8 A_DskIdx );
		BOOL8	WbaWrite( void* A_PtrBuf, __u32 A_Size );
		BOOL8	WbaRead( void* A_PtrBuf, __u32 A_Size );
		BOOL8	UcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );		//thread free
		BOOL8	UcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size );			//thread free
		BOOL8	UwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );		//thread free
		BOOL8	UwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size );			//thread free
		BOOL8	TmaRead( __u8 A_DevType, Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt );	//thread safe only
		void	AutoDelTimeSet( __u32 A_DayCnt );
		void	VssItemAdd( __u64	A_Hndl, Def_GrTime A_Time, __u8 A_Ch, __u8 A_Cmd, BOOL8 A_IsArmed, __u8	A_Etty,
			__u8 A_Seq, __u8 A_FrmCnt );	//thread not free
		void	SafeVssItemAdd( __u64	A_Hndl, Def_GrTime A_Time, __u8 A_Ch, __u8 A_Cmd, BOOL8 A_IsArmed, __u8	A_Etty,
			__u8 A_Seq, __u8 A_FrmCnt );	//thread free
		BOOL8	VssItemGet( __u64* A_PtrRtHndl, Def_GrTime* A_PtrRtTime, __u8* A_PtrRtCh, __u8* A_PtrRtCmd, 
			BOOL8* A_PtrRtIsArmed, __u8*	A_PtrRtEtty, __u8* A_PtrRtSeq, __u8* A_PtrRtFrmCnt );	//thread free , return frame count
		void	VssItemPop( void );	//thread free
		void	VssReset( void );

		// read
		void	ImmReadForward( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl );
		void	ImmReadBackward( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl );
		void	ImmReadBegin( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl );
		void	ImmReadEnd( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl );
		void	ImmReadByMinFw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, Def_GrTime A_Time );
		void	ImmReadByMinBw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, Def_GrTime A_Time );
		void	ImmReadByHndlFw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		void	ImmReadByHndlBw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, __u64 A_Hndl );

		void	SafeReadForward( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl );
		void	SafeReadBackward( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl );
		void	SafeReadBegin( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl );
		void	SafeReadEnd( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl );
		void	SafeReadByMinFw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, Def_GrTime A_Time );
		void	SafeReadByMinBw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, Def_GrTime A_Time );
		void	SafeReadByHndlFw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, __u64 A_Hndl );
		void	SafeReadByHndlBw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, __u64 A_Hndl );

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


