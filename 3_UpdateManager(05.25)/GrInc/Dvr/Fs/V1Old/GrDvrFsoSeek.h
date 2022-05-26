/*
	dvr file system version 1 seeker

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrFsoSeek
#define	_EPC_GrDvrFsoSeek

//====================================================================
// uses
#include <GrTypeBase.h>
#include <Dvr/Fs/V1Old/GrDvrFsoMng.h>
#include <GrBufTool.h>
#include <GrTaskSvr.h>
#include <GrTaskCli.h>
	
//====================================================================
//constance
#define E_GrDvrFsoSeekBufMaxCnt		2

#define E_GrDvrFsoSeekCbCmdChkLoadAble		Mac_GrMakeFcc('F','1','C','L')	//St_GrDvrFsoSeekCbMsgChkLoad
#define E_GrDvrFsoSeekCbCmdSyncReset			Mac_GrMakeFcc('F','1','S','R')	//St_GrDvrFsoSeekCbMsgSyncReset

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct St_GrDvrFsoSeekBufItm 
{
	void*	PtrBuf;
	__u32	BufGap;
	__u32	BufPos;
	__u32	BufInSize;
}	*Ptr_GrDvrFsoSeekBufItm;

typedef	struct St_GrDvrFsoSeekChCtrl
{
	__u8	VopId;
	__u8	Res;
}	*Ptr_GrDvrFsoSeekChCtrl;

typedef	struct St_GrDvrFsoSeekCbMsgSyncReset
{
	St_GrTaskMsgHd	Hd;
	__u32	Id;
}	*Ptr_GrDvrFsoSeekCbMsgSyncReset;

typedef	struct St_GrDvrFsoSeekCbMsgChkLoad
{
	St_GrTaskMsgHd	Hd;
	__u32	Id;
	BOOL8	IsWait;
	BOOL8*	PtrRtIsAble;		//able
}	*Ptr_GrDvrFsoSeekCbMsgChkLoad;

//====================================================================
//class

class	Cls_GrDvrFsoSeek	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32		m_MsgAloc;

		Cls_GrDvrFsoMng*	m_FsMng;
		__u32	m_FccStrm;

		__u8	m_DevType;
		BOOL8	m_IsFsSafe;
		BOOL8	m_IsDirBack;
		BOOL8	m_IsLimFirst;

		__u8	m_BufIdx;				//buffer index
		__u8	m_BufXor;				//buffer xor - dual buffer

		BOOL8	m_IsFrmFirst;		//first frame
		__u8	m_BlkSkipCnt;		//block skip count

		__u16	m_VdoAbleMap;
		__u16	m_VdoIonlyMap;
		__u16	m_VdoPableMap;
		__u16	m_AdoAbleMap;

		__u16	m_ReqIonlyMap;

		__u8	m_MinJmpStep;		//min unit step count
		__u8	m_MinJmpProc;		//min jump frame count
		__u8	m_AbleChCnt;		//able channel count

		Def_GrTime	m_LastOutTime;

		St_GrDvrFsoSeekBufItm	m_BufTbl[E_GrDvrFsoSeekBufMaxCnt];
		__u32	m_BufSize;
		St_GrDvrFsoSeekChCtrl	m_ChTbl[E_GrDvrMaxChCnt];

		//handle cache
		__u64	m_Hndl;		//loaded handle

		Cls_GrTaskCli*	m_CliSync;		//sync client
		__u32	m_SyncId;

		__u32	m_DmngId;		//disk manager id


	protected:
		//local
		BOOL8	LcReadNow( void );
		BOOL8	LcReadNext( void );
		BOOL8	LcReadBack( void );
		BOOL8	LcReadPrev( void );
		BOOL8	LcIsLoadAble( void );
		void	LcSyncReset( void );
		void	LcWaitCanLoad( void );
		BOOL8	LcLoadJmpNext( void );
		BOOL8	LcLoadJmpBack( void );
		BOOL8	LcLoadJmpPrev( void );

		BOOL8	LcChkAbleFrm( Ptr_GrDvrAvHd A_PtrFrm );

		void*	LcFrmNext( BOOL8* A_PtrRtIsEnd );
		void*	LcFrmPrev( BOOL8* A_PtrRtIsEnd );
		void*	LcFrmMinNext( BOOL8* A_PtrRtIsEnd );
		void*	LcFrmMinPrev( BOOL8* A_PtrRtIsEnd );

		//void*	LcNextFrm

		//event
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */

	public:
    Cls_GrDvrFsoSeek( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFsoMng* A_FsMng, void* A_PtrBuf, __u32 A_SizeBuf, void* A_PtrDualBuf, BOOL8 A_IsFsSafe );
    virtual		~Cls_GrDvrFsoSeek();
		
		void	SyncClientSet( Cls_GrTaskCli* A_Cli, __u32 A_Id );
		void	DevTypeSet( __u8 A_DevType );
		__u8	DevTypeGet( void );
		__u32	BufIdxGet( void );

		BOOL8	SeekFirst( void );
		BOOL8	SeekLast( void );
		BOOL8	SeekRecent( void );
		BOOL8	SeekByMinute( Def_GrTime A_Time );
		BOOL8	SeekBySec( Def_GrTime A_Time );
		BOOL8	SeekByRngMin( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd );
		BOOL8	SeekByRngSec( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, PBOOL8 A_PtrIsEd );
		BOOL8	CntnByRngSec( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, PBOOL8 A_PtrIsEd );
		BOOL8	SeekByHndl( __u64 A_Hndl );
		void	DirectionSet( BOOL8 A_IsBack );
		BOOL8	IsDirBack( void );

		void*	GetNextFrame( BOOL8* A_PtrRtIsEnd );
		void*	GetNextFrameOld( BOOL8* A_PtrRtIsEnd );
		void	VdoAbleMapSet( __u16 A_Map );
		void	VdoIonlyMapSet( __u16 A_Map );
		void	AdoAbleMapSet( __u16 A_Map );
		void	VdoChSyncReset( __u8 A_Ch );
		void	VopSyncReset( void );
		void	MinJumpSet( __u8 A_Val );
		__u8	MinJumpGet( void );

};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


