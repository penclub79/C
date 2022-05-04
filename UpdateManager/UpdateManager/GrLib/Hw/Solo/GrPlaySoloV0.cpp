/*

	solo record play

*/

//====================================================================
//include
#include	<Hw/Solo/GrPlaySoloV0.h>
#include	<GrTimeTool.h>
#include	<GrDebug.h>
#include	<GrDumyTool.h>

//====================================================================
//local const
#define E_GrPlaySoloV0RecentFrmCnt		100

#define E_GrPlaySoloV0SpeedJmpBase		4

//reserved work
#define E_GrPlaySoloV0RwkNone					0
#define E_GrPlaySoloV0RwkGoFirst			1
#define E_GrPlaySoloV0RwkGoLast				2
#define E_GrPlaySoloV0RwkGoTimeMin		3
#define E_GrPlaySoloV0RwkGoByHndl			4
#define E_GrPlaySoloV0RwkGoTimeSec		5
#define E_GrPlaySoloV0RwkGoRecent			6

//command
#define E_GrPlaySoloV0CmdSrcDevSet		0x00
#define E_GrPlaySoloV0CmdRsvdWork			0x01
#define E_GrPlaySoloV0CmdPlay					0x02
#define E_GrPlaySoloV0CmdDirSet				0x03
#define E_GrPlaySoloV0CmdSpeedSet			0x04
#define E_GrPlaySoloV0CmdStepNext			0x05		/* play next frame : St_GrPlaySoloV0MsgBase */
#define E_GrPlaySoloV0CmdStepPrev			0x06		/* play previous frame : St_GrPlaySoloV0MsgBase */
#define E_GrPlaySoloV0CmdAdoChSet			0x07		/* audio channel set : St_GrPlaySoloV0MsgAdoChSet */
#define E_GrPlaySoloV0CmdStop					0x08		/* play stop : St_GrPlaySoloV0MsgBase */

//====================================================================
//local type

typedef	struct St_GrPlaySoloV0MsgBase
{
	St_GrTaskMsgHd	MsgHd;
	__u32		PlayCmd;
}	*Ptr_GrPlaySoloV0MsgBase;

typedef	struct St_GrPlaySoloV0MsgRtBool
{
	St_GrPlaySoloV0MsgBase	Hd;
	BOOL8*	PtrRtVal;
}	*Ptr_GrPlaySoloV0MsgRtBool;

typedef	struct St_GrPlaySoloV0MsgSrcSet 
{
	St_GrPlaySoloV0MsgBase	Hd;
	__u8	Src;
}	*Ptr_GrPlaySoloV0MsgSrcSet;

typedef	struct St_GrPlaySoloV0MsgGoTime
{
	St_GrPlaySoloV0MsgBase	Hd;
	Def_GrTime	Time;
	BOOL8*	PtrRtVal;
}	*Ptr_GrPlaySoloV0MsgGoTime;

typedef	struct St_GrPlaySoloV0MsgDirSet
{
	St_GrPlaySoloV0MsgBase	Hd;
	BOOL8	IsBack;
}	*Ptr_GrPlaySoloV0MsgDirSet;

typedef	struct St_GrPlaySoloV0MsgSpdSet
{
	St_GrPlaySoloV0MsgBase	Hd;
	__u32	Speed;
}	*Ptr_GrPlaySoloV0MsgSpdSet;

typedef	struct St_GrPlaySoloV0MsgGoByHndl
{
	St_GrPlaySoloV0MsgBase	Hd;
	__u64	Hndl;
	BOOL8*	PtrRtVal;
}	*Ptr_GrPlaySoloV0MsgGoByHndl;

typedef	struct St_GrPlaySoloV0MsgAdoChSet
{
	St_GrPlaySoloV0MsgBase	Hd;
	__u8	Ch;
}	*Ptr_GrPlaySoloV0MsgAdoChSet;

typedef	struct St_GrPlaySoloV0MsgRsvdWork
{
	St_GrPlaySoloV0MsgBase	Hd;
	__u8	RwkCmd;			//reserved work command
	__u64	Hndl;
	Def_GrTime	Time;
}	*Ptr_GrPlaySoloV0MsgRsvdWork;


//====================================================================
//global var


//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrPlaySoloV0::Cls_GrPlaySoloV0( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDevSolo* A_DevSolo, Cls_GrDvrFsoMng* A_FsMng ):
Cls_GrTaskCli()
{
	// local -------------------
		void*	Tv_PtrBuf;
		void*	Tv_PtrDblBuf;
		__u32	Tv_BufSize;
		__u8	Tv_WkIdx;
	// code --------------------
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();
		m_ObjDev		=	A_DevSolo;
		m_FsMng			=	A_FsMng;

		//make file system seeker
		Tv_PtrBuf			=	m_ObjDev->PlayBufPtrGet();
		Tv_BufSize		=	m_ObjDev->PlayBufSizeGet() >> 1;		//double buffering
		Tv_PtrDblBuf	=	(void*)( (__u32)Tv_PtrBuf + Tv_BufSize );
		m_FsSeek			=	(Cls_GrDvrFsoSeek*)new Cls_GrDvrFsoSeek( A_TaskSvr, m_FsMng, Tv_PtrBuf, Tv_BufSize, Tv_PtrDblBuf, FALSE );
		m_FsSeek->SyncClientSet( this, 0 );

		m_PtrPprFrm		=	NULL;

		m_RwkCmd			=	E_GrPlaySoloV0RwkNone;

		m_TimeSec		=	0;
		m_TimeMsec	=	0;
		m_PlaySpeed	=	0;

		m_DecReqed	=	0;			//decode requested
		m_IsSeek		=	FALSE;

		//m_IsPlaying	=	FALSE;
		m_IsStep		=	FALSE;
		m_PtrPprFrm	=	NULL;

		m_AdoCh			=	0;

		GrDumyZeroMem( m_ChTbl, sizeof(m_ChTbl) );
		for ( Tv_WkIdx=0;Tv_WkIdx < E_SoloMaxChCnt;Tv_WkIdx++ )
		{
			m_ChTbl[Tv_WkIdx].Res		=	E_GrDvrVdoResBad;
		}
		
		m_TimeLastSec		=	0;
		m_TimeLastMsec	=	0;
		m_IsTimeLastSm	=	FALSE;

		m_MaxChCnt			=	(__u8)m_ObjDev->VdoChCnt();
		m_LastStopStat	=	E_GrDvrRecErrNone;

		if ( m_ObjDev->IsVdoPal() )
		{
			m_VdoSigFps		=	E_GrDvrFpsPal;
		}
		else
		{
			m_VdoSigFps		=	E_GrDvrFpsNtsc;
		}
		m_DecPwrFps		=	m_ObjDev->DecPowerGet() * m_VdoSigFps * 4;

		//regist background
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistBkgClient( this );
}
//--------------------------------------------------------------------
Cls_GrPlaySoloV0::~Cls_GrPlaySoloV0()
{

		//unregist background
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistMsgClient( this );
		GrTaskSvrMsgCmdFree( m_MsgAloc );

		//release object
		delete	m_FsSeek;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( LcRtlPlay() )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrPlaySoloV0::SourceType( void )
{
		return	m_FsSeek->DevTypeGet();
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcProcSourceSet( __u8 A_Source )
{
		m_FsSeek->DevTypeSet( A_Source );
		m_PtrPprFrm	=	NULL;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrPlaySoloV0::TimeGet( void )
{	
		return	m_TimeSec;
}
//--------------------------------------------------------------------
__u32	Cls_GrPlaySoloV0::SpeedGet( void )
{
		return	m_PlaySpeed;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::LcProcGoFirst( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_FsSeek->SeekFirst() )
		{
			m_LastStopStat	=	E_GrDvrRecErrNone;
			Tv_Result				=	TRUE;
		}
		m_PtrPprFrm	=	NULL;
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::LcProcGoLast( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_FsSeek->SeekLast() )
		{
			m_LastStopStat	=	E_GrDvrRecErrNone;
			Tv_Result	=	TRUE;
		}
		m_PtrPprFrm	=	NULL;
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::LcProcGoRecent( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		m_FsSeek->DirectionSet( FALSE );
		Tv_Result	=	m_FsSeek->SeekRecent();
		m_PtrPprFrm	=	NULL;
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrPlaySoloV0MsgBase	Tv_PtrMsg;
		__u16	Tv_MapCh;
		__u16	Tv_MaskCh;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrTaskMsgQuadChgReq == A_PtrMsg->Cmd )
		{
			//update 
			Tv_MapCh	=	(__u16)m_ObjDev->QuadViewChMapGet();
			Tv_MaskCh	=	1;
			for ( Tv_WkIdx=0;Tv_WkIdx < m_MaxChCnt;Tv_WkIdx++ )
			{
				if ( 0 == (Tv_MapCh & Tv_MaskCh) )
				{
					m_ChTbl[Tv_WkIdx].Res	=	E_GrDvrVdoResBad;
				}
				//next
				Tv_MaskCh	=	Tv_MaskCh << 1;
			}
			LcChRtlUpdt();
			LcChSpdUpdt();
			m_FsSeek->VdoAbleMapSet( Tv_MapCh );
			//erase 
			if ( 0 != m_ObjDev->PlayChMapGet() )
			{
				LcDoDispErase();
			}
			
			Tv_Result	=	TRUE;
		}
		else if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			//run play contorl message
			Tv_PtrMsg	=	(Ptr_GrPlaySoloV0MsgBase)A_PtrMsg;
			switch( Tv_PtrMsg->PlayCmd )
			{
				case	E_GrPlaySoloV0CmdSrcDevSet:
					LcProcSourceSet( ((Ptr_GrPlaySoloV0MsgSrcSet)Tv_PtrMsg)->Src );
					break;
				case	E_GrPlaySoloV0CmdRsvdWork:
					m_RwkCmd	=	((Ptr_GrPlaySoloV0MsgRsvdWork)Tv_PtrMsg)->RwkCmd;
					m_RwkHndl	=	((Ptr_GrPlaySoloV0MsgRsvdWork)Tv_PtrMsg)->Hndl;
					m_RwkTime	=	((Ptr_GrPlaySoloV0MsgRsvdWork)Tv_PtrMsg)->Time;
					break;
				case	E_GrPlaySoloV0CmdPlay:
					LcProcPlay();
					break;
				case	E_GrPlaySoloV0CmdDirSet:
					LcProcDirSet( ((Ptr_GrPlaySoloV0MsgDirSet)Tv_PtrMsg)->IsBack );
					break;
				case	E_GrPlaySoloV0CmdSpeedSet:
					LcProcSpeedSet( ((Ptr_GrPlaySoloV0MsgSpdSet)Tv_PtrMsg)->Speed );
					break;
				case	E_GrPlaySoloV0CmdStepNext:
					LcProcFrmNext();
					break;
				case	E_GrPlaySoloV0CmdStepPrev:
					LcProcFrmPrev();
					break;
				case	E_GrPlaySoloV0CmdStop:
					LcProcStop();
					break;
			}
			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcChRtlUpdt( void )
{
	// local -------------------
		__u32	Tv_Ch;
		__u8	Tv_Intv;
	// code --------------------

		for ( Tv_Ch=0;Tv_Ch < E_SoloMaxChCnt;Tv_Ch++ )
		{
			//check 
			if ( E_GrDvrVdoResBad != m_ChTbl[Tv_Ch].Res )
			{
				Tv_Intv		=	m_ChTbl[Tv_Ch].Speed;
				m_ChTbl[Tv_Ch].SkipCnt	=	(__u8)((1 << m_PlaySpeed) / Tv_Intv);
				if ( (0 != m_ChTbl[Tv_Ch].SkipCnt) && (0 == ((1 << m_PlaySpeed) % Tv_Intv)) )
				{
					m_ChTbl[Tv_Ch].SkipCnt --;
				}
				m_ChTbl[Tv_Ch].Pwr	=	( m_VdoSigFps << ((3 & (__u32)m_ChTbl[Tv_Ch].Res) + m_PlaySpeed) ) / Tv_Intv;
			}
		}
		//totla play speed update
		LcChSpdUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::LcProcGoTimeMinute( Def_GrTime A_Time )
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_GrTime	Tv_TimeSt;
		Def_GrTime	Tv_TimeEd;
	// code --------------------
		Tv_Result	=	FALSE;

		//get record range
		if ( m_FsMng->RecTimeRangeGet( m_FsSeek->DevTypeGet(), &Tv_TimeSt, &Tv_TimeEd ) )
		{
			if ( A_Time > Tv_TimeEd )
			{
				m_FsSeek->SeekRecent();
			}
			else
			{
				if ( (Tv_TimeSt < A_Time) )
				{
					Tv_TimeSt	=	A_Time;
				}
				if ( m_FsSeek->SeekByRngMin( Tv_TimeSt, Tv_TimeEd ) )
				{
					m_LastStopStat	=	E_GrDvrRecErrNone;
					Tv_Result	=	TRUE;
				}
			}
		}
		m_PtrPprFrm	=	NULL;
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::LcProcGoTimeSec( Def_GrTime A_Time )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_FsSeek->SeekBySec( A_Time ) )
		{
			m_LastStopStat	=	E_GrDvrRecErrNone;
			Tv_Result	=	TRUE;
		}
		m_PtrPprFrm	=	NULL;
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcChSpdUpdt( void )
{
	// local -------------------
		__u32	Tv_Ch;
		__u32	Tv_PwrSum;
	// code --------------------
		Tv_PwrSum		=	0;
		for ( Tv_Ch=0;Tv_Ch < E_SoloMaxChCnt;Tv_Ch++ )
		{
			//check use channel
			if ( E_GrDvrVdoResBad != m_ChTbl[Tv_Ch].Res )
			{
				//calculate fps
				Tv_PwrSum	=	Tv_PwrSum + m_ChTbl[Tv_Ch].Pwr;
			}
		}
		//check overpower
		if ( m_DecPwrFps < Tv_PwrSum  )
		{
			m_FsSeek->VdoIonlyMapSet( 0xFFFF );
		}
		else
		{
			m_FsSeek->VdoIonlyMapSet( 0 );
		}
		//audio update
		LcAdoAbleUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::LcDecFrame( Ptr_GrDvrAvHd A_PtrFrm )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Ch;
		__u32	Tv_Intv;
		BOOL8	Tv_IsNoDisp;
	// code --------------------
		Tv_Result	=	FALSE;

		//check frame type
		Tv_Ch	=	A_PtrFrm->Ch;
		switch( A_PtrFrm->Type )
		{
			case	E_GrDvrAvTypVdoI:
				//check change speed
				if ( (A_PtrFrm->SkipCnt != m_ChTbl[Tv_Ch].Speed) || (A_PtrFrm->Res != m_ChTbl[Tv_Ch].Res) )
				{
					//update speed
					m_ChTbl[Tv_Ch].Res		=	A_PtrFrm->Res;
					m_ChTbl[Tv_Ch].Speed	=	A_PtrFrm->SkipCnt;
					//calculate fps
					Tv_Intv		=	m_ChTbl[Tv_Ch].Speed;
					if ( 0 == Tv_Intv )
					{
						Tv_Intv ++;
					}
					m_ChTbl[Tv_Ch].SkipCnt	=	(__u8)((1 << m_PlaySpeed) / Tv_Intv);
					if ( (0 != m_ChTbl[Tv_Ch].SkipCnt) && (0 == ((1 << m_PlaySpeed) % Tv_Intv)) )
					{
						m_ChTbl[Tv_Ch].SkipCnt --;
					}
					m_ChTbl[Tv_Ch].Pwr	=	( m_VdoSigFps << ((3 & (__u32)m_ChTbl[Tv_Ch].Res) + m_PlaySpeed) ) / Tv_Intv;
					LcChSpdUpdt();
				}
				//check skip
				Tv_IsNoDisp	=	FALSE;
				if ( (m_ChTbl[Tv_Ch].SkipCnt > m_ChTbl[Tv_Ch].SkipPos) && (E_GrPlaySoloV0SpeedJmpBase > m_PlaySpeed) )
				{
					//do skip
					Tv_IsNoDisp	=	!m_IsStep;		//no skip when step mode
				}
				//decode video data
				if ( m_ObjDev->PlayVdoData( A_PtrFrm, Tv_IsNoDisp ) )
				{
					m_ChTbl[Tv_Ch].SkipPos ++;
					if ( m_ChTbl[Tv_Ch].SkipCnt < m_ChTbl[Tv_Ch].SkipPos )
					{
						m_ChTbl[Tv_Ch].SkipPos	=	0;
					}
					//update attribute
					m_ObjDev->PlayAtbSet( Tv_Ch, A_PtrFrm->Evt );
					//success
					Tv_Result	=	TRUE;
				}
				break;
			case	E_GrDvrAvTypVdoP:
				//check skip
				Tv_IsNoDisp	=	FALSE;
				if ( m_ChTbl[Tv_Ch].SkipCnt > m_ChTbl[Tv_Ch].SkipPos )
				{
					//do skip
					Tv_IsNoDisp	=	!m_IsStep;		//no skip when step mode
				}
				//decode video data
				if ( m_ObjDev->PlayVdoData( A_PtrFrm, Tv_IsNoDisp ) )
				{
					m_ChTbl[Tv_Ch].SkipPos ++;
					if ( m_ChTbl[Tv_Ch].SkipCnt < m_ChTbl[Tv_Ch].SkipPos )
					{
						m_ChTbl[Tv_Ch].SkipPos	=	0;
					}
					//update attribute
					m_ObjDev->PlayAtbSet( Tv_Ch, A_PtrFrm->Evt );
					//success
					Tv_Result	=	TRUE;
				}
				break;
			case	E_GrDvrAvTypAdo:
				//audio frame
				Tv_Result	=	m_ObjDev->PlayAdoData( A_PtrFrm );
				break;
		}
		
		//update enviroment
		if ( Tv_Result )
		{
			m_IsSeek	=	FALSE;
			m_DecReqed ++;
			m_TimeLastSec		=	A_PtrFrm->TimeSec;
			m_TimeLastMsec	=	A_PtrFrm->TimeMsec;
			m_IsTimeLastSm	=	A_PtrFrm->IsSm;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::LcIsReadAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_DecReqed >= m_ObjDev->PlayVdoRemainCntGet() )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::LcProcPlayNrm( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsDec;
		BOOL8	Tv_IsEnd;
	// code --------------------
		Tv_Result	=	FALSE;

		//get prepared pointer
		if ( NULL == m_PtrPprFrm )
		{
			m_PtrPprFrm	=	(Ptr_GrDvrAvHd)m_FsSeek->GetNextFrame( &Tv_IsEnd );
			if ( Tv_IsEnd )
			{
				m_PtrPprFrm			=	NULL;
				m_LastStopStat	=	E_GrDvrRecErrLimEnd;
				LcSpeedReset();
				m_ObjDev->PlayingSet( FALSE );
			}
		}
		//try decode
		if ( NULL != m_PtrPprFrm )
		{
			Tv_IsDec	=	TRUE;
			//update time
			if ( m_IsStep || (E_GrPlaySoloV0SpeedJmpBase <= m_PlaySpeed) )
			{
				//update time
				m_TimeSec				=	m_PtrPprFrm->TimeSec;
				m_TimeMsec			=	m_PtrPprFrm->TimeMsec;
			}
			else
			{
				//check time over
				if ( (E_GrPlaySoloV0PlayTimeLimSec <= (m_PtrPprFrm->TimeSec - m_TimeSec)) || (m_PtrPprFrm->TimeSec < m_TimeLastSec) )
				{
					//update direct
					m_TimeSec				=	m_PtrPprFrm->TimeSec;
					m_TimeMsec			=	m_PtrPprFrm->TimeMsec;
				}
				else
				{
					//check time small
					if ( (m_PtrPprFrm->TimeSec > m_TimeSec) || ((m_PtrPprFrm->TimeSec == m_TimeSec) && ( m_PtrPprFrm->TimeMsec > m_TimeMsec ) ) )
					{
						//not enough time
						Tv_IsDec	=	FALSE;
					}
				}
			}
			//decode
			if ( Tv_IsDec )
			{
				//do decode
				if ( LcDecFrame( m_PtrPprFrm ) )
				{
					//update next frame
					m_PtrPprFrm	=	NULL;
					Tv_Result	=	TRUE;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::LcProcPlayBack( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsDec;
		BOOL8	Tv_IsEnd;
	// code --------------------
		Tv_Result	=	FALSE;

		//get prepared pointer
		if ( NULL == m_PtrPprFrm )
		{
			m_PtrPprFrm	=	(Ptr_GrDvrAvHd)m_FsSeek->GetNextFrame( &Tv_IsEnd );
			if ( Tv_IsEnd )
			{
				m_LastStopStat	=	E_GrDvrRecErrLimFirst;
				LcSpeedReset();
				m_ObjDev->PlayingSet( FALSE );
				m_PtrPprFrm			=	NULL;
			}
		}
		//try decode
		if ( NULL != m_PtrPprFrm )
		{
			Tv_IsDec	=	TRUE;
			//update time
			if ( m_IsStep || (E_GrPlaySoloV0SpeedJmpBase <= m_PlaySpeed) )
			{
				//update time
				m_TimeSec				=	m_PtrPprFrm->TimeSec;
				m_TimeMsec			=	m_PtrPprFrm->TimeMsec;
			}
			else
			{
				//check time over
				if ( (E_GrPlaySoloV0PlayTimeLimSec <= (m_TimeSec - m_PtrPprFrm->TimeSec)) || (m_PtrPprFrm->TimeSec > m_TimeLastSec) )
				{
					//update direct
					m_TimeSec				=	m_PtrPprFrm->TimeSec;
					m_TimeMsec			=	m_PtrPprFrm->TimeMsec;
				}
				else
				{
					//check time big
					if ( (m_PtrPprFrm->TimeSec < m_TimeSec) || ((m_PtrPprFrm->TimeSec == m_TimeSec) && ( m_PtrPprFrm->TimeMsec < m_TimeMsec ) ) )
					{
						//not enough time
						Tv_IsDec	=	FALSE;
					}
				}
			}
			//decode
			if ( Tv_IsDec )
			{
				//do decode
				if ( LcDecFrame( m_PtrPprFrm ) )
				{
					//update next frame
					m_PtrPprFrm	=	NULL;
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::LcRtlPlay( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_TickNow;
		__u32	Tv_TickGap;
		__u32	Tv_MsGap;
	// code --------------------
		Tv_Result	=	FALSE;

		//if ( m_IsPlaying )
		if ( m_ObjDev->IsPlaying() )
		{
			//init
			Tv_MsGap		=	0;
			Tv_TickNow	=	0;
			//reserved work process
			LcProcRsvdWork();

			//get tick
			if ( (!m_IsStep) && (E_GrPlaySoloV0SpeedJmpBase > m_PlaySpeed) )
			{
				Tv_TickNow	=	GrTimeGetTick();
				Tv_TickGap	=	(Tv_TickNow - m_TickLast) << m_PlaySpeed;
				Tv_MsGap		=	GrTimeTickToMs( Tv_TickGap );
			}

			if ( m_FsSeek->IsDirBack() )
			{
				Tv_Result	=	LcProcPlayBack();
				//update time
				if ( (!m_IsStep) && (E_GrPlaySoloV0SpeedJmpBase > m_PlaySpeed) )
				{
					if ( m_TimeMsec < Tv_MsGap )
					{
						while ( m_TimeMsec < Tv_MsGap )
						{
							m_TimeSec --;
							m_TimeMsec	=	m_TimeMsec + E_GrTimeMiliOfSec;	
						}
					}
					m_TimeMsec	=	m_TimeMsec - Tv_MsGap;
					m_TickLast	=	Tv_TickNow;
				}
			}
			else
			{
				Tv_Result	=	LcProcPlayNrm();
				//update time
				if ( (!m_IsStep) && (E_GrPlaySoloV0SpeedJmpBase > m_PlaySpeed) )
				{
					m_TimeMsec	=	m_TimeMsec + Tv_MsGap;
					while ( E_GrTimeMiliOfSec <= m_TimeMsec )
					{
						m_TimeSec ++;
						m_TimeMsec	=	m_TimeMsec - E_GrTimeMiliOfSec;	
					}
					m_TickLast	=	Tv_TickNow;
				}
			}
			//update step play
			if ( Tv_Result && m_IsStep )
			{
				m_IsStep		=	FALSE;
				m_ObjDev->PlayingSet( FALSE );
				//m_IsPlaying	=	FALSE;
				m_PtrPprFrm	=	NULL;
			}
		}	//	if ( m_IsPlaying )
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::SourceSet( __u8 A_Source )
{
	// local -------------------
		St_GrPlaySoloV0MsgSrcSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd			=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd				=	E_GrPlaySoloV0CmdSrcDevSet;
		Tv_Msg.Src							=	A_Source;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::GoFirst( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrPlaySoloV0MsgRsvdWork	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Hd.MsgHd.Cmd			=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd				=	E_GrPlaySoloV0CmdRsvdWork;
		Tv_Msg.RwkCmd						=	E_GrPlaySoloV0RwkGoFirst;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::GoLast( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrPlaySoloV0MsgRsvdWork	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Hd.MsgHd.Cmd			=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd				=	E_GrPlaySoloV0CmdRsvdWork;
		Tv_Msg.RwkCmd						=	E_GrPlaySoloV0RwkGoLast;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::GoRecent( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrPlaySoloV0MsgRsvdWork	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Hd.MsgHd.Cmd			=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd				=	E_GrPlaySoloV0CmdRsvdWork;
		Tv_Msg.RwkCmd						=	E_GrPlaySoloV0RwkGoRecent;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::GoTimeMinute( Def_GrTime A_Time )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrPlaySoloV0MsgRsvdWork	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Hd.MsgHd.Cmd			=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd				=	E_GrPlaySoloV0CmdRsvdWork;
		Tv_Msg.RwkCmd						=	E_GrPlaySoloV0RwkGoTimeMin;
		Tv_Msg.Time							=	A_Time;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::GoByHndl( __u64 A_Hnd )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrPlaySoloV0MsgRsvdWork	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Hd.MsgHd.Cmd			=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd				=	E_GrPlaySoloV0CmdRsvdWork;
		Tv_Msg.RwkCmd						=	E_GrPlaySoloV0RwkGoByHndl;
		Tv_Msg.Hndl							=	A_Hnd;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcProcPlay( void )
{
	// local -------------------
	// code --------------------
		//check already play
		if ( !m_ObjDev->IsPlaying() )
		{
			//updaet speed
			if ( E_GrPlaySoloV0SpeedJmpBase > m_PlaySpeed )
			{
				m_FsSeek->MinJumpSet( 0	);
			}
			else
			{
				m_FsSeek->MinJumpSet( (__u8)m_PlaySpeed - E_GrPlaySoloV0SpeedJmpBase + 1 );
			}
			//update last tick
			m_TickLast	=	GrTimeGetTick();
			m_ObjDev->PlayingSet( TRUE );
			LcAdoAbleUpdt();
			//update seek status
			m_IsSeek	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::Play( void )
{
	// local -------------------
		St_GrPlaySoloV0MsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.PlayCmd			=	E_GrPlaySoloV0CmdPlay;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcProcDirSet( BOOL8 A_IsBack )
{
	// local -------------------
	// code --------------------
		m_TickLast	=	GrTimeGetTick();
		m_FsSeek->DirectionSet( A_IsBack );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::IsDirBack( void )
{
		return	m_FsSeek->IsDirBack();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::IsPlaying( void )
{
		//return	m_IsPlaying;
		return	m_ObjDev->IsPlaying();
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::DirectionSet( BOOL8 A_IsBack )
{
	// local -------------------
		St_GrPlaySoloV0MsgDirSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd			=	E_GrPlaySoloV0CmdDirSet;
		Tv_Msg.IsBack					=	A_IsBack;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::Stop( void )
{
	// local -------------------
		St_GrPlaySoloV0MsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.PlayCmd			=	E_GrPlaySoloV0CmdStop;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcProcSpeedSet( __u32 A_Speed )
{
	// local -------------------
	// code --------------------
		//update play speed
		m_PlaySpeed		=	A_Speed;

		//updaet speed
		if ( E_GrPlaySoloV0SpeedJmpBase > m_PlaySpeed )
		{
			m_FsSeek->MinJumpSet( 0	);
		}
		else
		{
			m_FsSeek->MinJumpSet( (__u8)m_PlaySpeed - E_GrPlaySoloV0SpeedJmpBase + 1 );
		}
		//totla play speed update
		LcChRtlUpdt();
		LcChSpdUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcSpeedReset( void )
{
		//jump patch
		if ( E_GrPlaySoloV0SpeedJmpBase <= m_PlaySpeed )
		{
			m_FsSeek->MinJumpSet( 0	);
		}
		//reset play speed
		m_PlaySpeed	=	0;
		//totla play speed update
		LcChRtlUpdt();
		LcChSpdUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::SpeedSet( __u32 A_Speed )
{
	// local -------------------
		St_GrPlaySoloV0MsgSpdSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd			=	E_GrPlaySoloV0CmdSpeedSet;
		Tv_Msg.Speed					=	A_Speed;
		if ( E_GrPlaySoloV0PlaySpeedMax < A_Speed )
		{
			Tv_Msg.Speed					=	E_GrPlaySoloV0PlaySpeedMax;
		}
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::AudioChSet( __u8 A_Ch )
{
	// local -------------------
	// code --------------------
		m_AdoCh	=	A_Ch;
		LcAdoAbleUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::LcProcGoByHndl( __u64 A_Hndl )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		//change play direction
		if ( m_FsSeek->SeekByHndl( A_Hndl ) )
		{
			m_LastStopStat	=	E_GrDvrRecErrNone;
			Tv_Result	=	TRUE;
		}
		m_PtrPprFrm	=	NULL;
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::GoTimeSec( Def_GrTime A_Time )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrPlaySoloV0MsgRsvdWork	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Hd.MsgHd.Cmd			=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd				=	E_GrPlaySoloV0CmdRsvdWork;
		Tv_Msg.RwkCmd						=	E_GrPlaySoloV0RwkGoTimeSec;
		Tv_Msg.Time							=	A_Time;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcPwrReset( void )
{
	// local -------------------
		__u8	Tv_Ch;
	// code --------------------
		for ( Tv_Ch=0;Tv_Ch < m_MaxChCnt;Tv_Ch++ )
		{
			m_ChTbl[Tv_Ch].Speed	=	0xFF;
			m_ChTbl[Tv_Ch].Res	=	E_GrDvrVdoResBad;
			m_ChTbl[Tv_Ch].Pwr	=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcDoDispErase( void )
{
	// local -------------------
	// code --------------------
		//wait not remain play data
		while ( 0 != m_ObjDev->PlayVdoRemainCntGet() )
		{
			GrPrcSleep( 10 );		//waiting process
		}
		//do erase
		m_ObjDev->DispErase( TRUE );
		GrPrcSleep( 10 );
		m_ObjDev->DispErase( FALSE );
		GrPrcSleep( 10 );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::StepNext( void )
{
	// local -------------------
		St_GrPlaySoloV0MsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.PlayCmd			=	E_GrPlaySoloV0CmdStepNext;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::StepPrev( void )
{
	// local -------------------
		St_GrPlaySoloV0MsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.PlayCmd			=	E_GrPlaySoloV0CmdStepPrev;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcProcFrmNext( void )
{
		m_IsStep		=	TRUE;
		m_FsSeek->MinJumpSet( 0 );
		m_FsSeek->DirectionSet( FALSE );
		LcProcPlay();
		m_LastStopStat	=	E_GrDvrRecErrNone;
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcProcFrmPrev( void )
{
		m_IsStep		=	TRUE;
		m_FsSeek->MinJumpSet( 0 );
		m_FsSeek->DirectionSet( TRUE );
		LcProcPlay();
		m_LastStopStat	=	E_GrDvrRecErrNone;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::RtlCallback( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WaitLim;
	// code --------------------
		Tv_Result	=	TRUE;
		switch( A_PtrMsg->Cmd )
		{
			case	E_GrDvrFsoSeekCbCmdChkLoadAble:
				if ( ((Ptr_GrDvrFsoSeekCbMsgChkLoad)A_PtrMsg)->IsWait )
				{
					Tv_WaitLim	=	50;
					while ( !LcIsReadAble() )
					{
						GrPrcSleep( 20 );
						Tv_WaitLim --;
						if ( 0 == Tv_WaitLim )
						{
							DbgMsgPrint( "[BUG]Loadable TimeOut!\n" );
							break;
						}
					}
					*(((Ptr_GrDvrFsoSeekCbMsgChkLoad)A_PtrMsg)->PtrRtIsAble)	=	TRUE;
				}
				else
				{
					*(((Ptr_GrDvrFsoSeekCbMsgChkLoad)A_PtrMsg)->PtrRtIsAble)	=	LcIsReadAble();
				}
				break;
			case	E_GrDvrFsoSeekCbCmdSyncReset:
				//reset fps frame
				if ( 0 == m_DecReqed )
				{
					m_IsSeek	=	TRUE;
				}
				m_DecReqed	=	0;
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrPlaySoloV0::LastStopStatGet( void )
{
		return	m_LastStopStat;
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcProcStop( void )
{
		//m_IsPlaying	=	FALSE;
		m_ObjDev->PlayingSet( FALSE );
 		m_IsSeek		=	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcAdoAbleUpdt( void )
{
	// local -------------------
		__u16	Tv_Map;
	// code --------------------
		Tv_Map	=	0;
		if ( E_SoloMaxChCnt > m_AdoCh  )
		{
			Tv_Map	=	1 << (__u16)m_AdoCh;
			if ( m_IsStep || (0 != m_PlaySpeed) )
			{
				Tv_Map	=	0;
			}
		}
		m_FsSeek->AdoAbleMapSet( Tv_Map );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV0::LcProcRsvdWork( void )
{
	// local -------------------
	// code --------------------
		if ( NULL == m_PtrPprFrm )
		{
			switch( m_RwkCmd )
			{
				case	E_GrPlaySoloV0RwkGoFirst:
					LcProcGoFirst();
					break;
				case	E_GrPlaySoloV0RwkGoLast:
					LcProcGoLast();
					break;
				case	E_GrPlaySoloV0RwkGoTimeMin:
					LcProcGoTimeMinute( m_RwkTime );
					break;
				case	E_GrPlaySoloV0RwkGoByHndl:
					LcProcGoByHndl( m_RwkHndl );
					break;
				case	E_GrPlaySoloV0RwkGoTimeSec:
					LcProcGoTimeSec( m_RwkTime );
					break;
				case	E_GrPlaySoloV0RwkGoRecent:
					LcProcGoRecent();
					break;
			}
			m_RwkCmd	=	E_GrPlaySoloV0RwkNone;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::IsSeeking( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ObjDev->IsPlaying() )
		{
			Tv_Result	=	m_IsSeek;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::IsSummerTime( void )
{
		return	m_IsTimeLastSm;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV0::IsStepMode( void )
{
		return	m_IsStep;
}
//--------------------------------------------------------------------

