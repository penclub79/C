/*

	solo record play

*/

//====================================================================
//include
#include	<Hw/Solo/GrPlaySoloV1.h>
#include	<GrTimeTool.h>
#include	<GrDebug.h>
#include	<GrDumyTool.h>

#ifdef LINUX_EMU
#include	<Hw/Solo/GrDrvSolo.h>
#endif

//====================================================================
//local const


//====================================================================
//local type



//====================================================================
//global var


//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrPlaySoloV1::Cls_GrPlaySoloV1( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDevSolo* A_DevSolo, Cls_GrDvrFs1Mng* A_FsMng, __u8 A_Gop ):
	Cls_GrDvrFs1Play( A_TaskSvr, A_FsMng, NULL, FALSE )
{
	// local -------------------
	// code --------------------
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();
		m_ObjDev		=	A_DevSolo;

		GrDumyZeroMem( &m_SeekCtrl, sizeof(m_SeekCtrl) );

		//make file system seeker
		/*
		m_SeekCtrl.IsDblBuf				=	FALSE;
		m_SeekCtrl.Buf[0].Size		=	m_ObjDev->PlayBufSizeGet();
		m_SeekCtrl.Buf[0].Ptr			=	m_ObjDev->PlayBufPtrGet();
		*/

		m_SeekCtrl.IsDblBuf				=	TRUE;
		m_SeekCtrl.Buf[0].Size		=	m_ObjDev->PlayBufSizeGet() >> 1;
		m_SeekCtrl.Buf[0].Ptr			=	m_ObjDev->PlayBufPtrGet();
		m_SeekCtrl.Buf[1].Size		=	m_SeekCtrl.Buf[0].Size;
		m_SeekCtrl.Buf[1].Ptr			=	(void*)( (__u32)m_SeekCtrl.Buf[0].Ptr + m_SeekCtrl.Buf[0].Size );

		m_DecVdoSyncCnt						=	0;
		m_DecAdoSyncCnt						=	0;

		m_PtrSeekCtrl	=	&m_SeekCtrl;

		m_MaxChCnt			=	(__u8)m_ObjDev->VdoChCnt();
		m_IsVdoPal			=	(BOOL8)m_ObjDev->IsVdoPal();	// DAMGI - need implement later
		m_Gop						=	A_Gop;

		if ( m_IsVdoPal )
		{
			m_VdoSigFps		=	E_GrDvrFpsPal;
		}
		else
		{
			m_VdoSigFps		=	E_GrDvrFpsNtsc;
		}
		m_DecPower	=	m_VdoSigFps << (4 + E_GrDvrFs1PlayPwrAmp);

		//regist background
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, 1000, &m_HndlTimer );
}
//--------------------------------------------------------------------
Cls_GrPlaySoloV1::~Cls_GrPlaySoloV1()
{

		//unregist background
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );
		m_TaskSvr->UnregistMsgClient( this );

		GrTaskSvrMsgCmdFree( m_MsgAloc );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcProcSourceSet( __u8 A_Source )
{
		RecDevTypeSet( A_Source );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcProcGoFirst( void )
{
	// local -------------------
	// code --------------------
		JumpBegin();
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcProcGoRecent( void )
{
	// local -------------------
	// code --------------------
		JumpEnd();
		PlayDirSet( TRUE );
		JumpByCnt( E_GrPlaySoloV1RecentFrmCnt );
		PlayDirSet( FALSE );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV1::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrPlaySoloV1MsgBase	Tv_PtrMsg;
		__u16	Tv_MapCh;
		__u16	Tv_MaskCh;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			//run play contorl message
			Tv_PtrMsg	=	(Ptr_GrPlaySoloV1MsgBase)A_PtrMsg;
			switch( Tv_PtrMsg->PlayCmd )
			{
				case	E_GrPlaySoloV1CmdSrcDevSet:
					LcProcSourceSet( ((Ptr_GrPlaySoloV1MsgSrcSet)Tv_PtrMsg)->Src );
					break;
				case	E_GrPlaySoloV1CmdPlay:
					LcProcPlay();
					break;
				case	E_GrPlaySoloV1CmdDirSet:
					LcProcDirSet( ((Ptr_GrPlaySoloV1MsgDirSet)Tv_PtrMsg)->IsBack );
					break;
				case	E_GrPlaySoloV1CmdSpeedSet:
					LcProcSpeedSet( ((Ptr_GrPlaySoloV1MsgSpdSet)Tv_PtrMsg)->Speed );
					break;
				case	E_GrPlaySoloV1CmdStepNext:
					LcProcFrmNext();
					break;
				case	E_GrPlaySoloV1CmdStepPrev:
					LcProcFrmPrev();
					break;
				case	E_GrPlaySoloV1CmdStop:
					LcProcStop();
					break;
				case E_GrPlaySoloV1CmdAdoChSet:
					LcMprcAdoChSet( ((Ptr_GrPlaySoloV1MsgAdoChSet)A_PtrMsg)->Ch );
					break;
				case E_GrPlaySoloV1CmdGoFirst:
					LcProcGoFirst();
					break;
				case E_GrPlaySoloV1CmdGoRecent:
					LcProcGoRecent();
					break;
				case E_GrPlaySoloV1CmdGoTime:
					LcProcGoTime( ((Ptr_GrPlaySoloV1MsgGoTime)A_PtrMsg)->Time );
					break;
				case E_GrPlaySoloV1CmdGoHndl:
					LcProcGoByHndl( ((Ptr_GrPlaySoloV1MsgGoByHndl)A_PtrMsg)->Hndl );
					break;
			}
			Tv_Result	=	TRUE;

		}
		else
		{
			if ( E_GrTaskMsgQuadChgReq == A_PtrMsg->Cmd )
			{
				// video channel update
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
				VideoChEnable( Tv_MapCh );

				// display erase 
				if ( 0 != Tv_MapCh )
				{
					LcDoDispErase();
				}
			
				Tv_Result	=	TRUE;

			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::SourceSet( __u8 A_Source )
{
	// local -------------------
		St_GrPlaySoloV1MsgSrcSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd			=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd				=	E_GrPlaySoloV1CmdSrcDevSet;
		Tv_Msg.Src							=	A_Source;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::GoFirst( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrPlaySoloV1MsgBase	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.MsgHd.Cmd			=	m_MsgAloc;
		Tv_Msg.PlayCmd				=	E_GrPlaySoloV1CmdGoFirst;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::GoRecent( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrPlaySoloV1MsgBase	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.MsgHd.Cmd			=	m_MsgAloc;
		Tv_Msg.PlayCmd				=	E_GrPlaySoloV1CmdGoRecent;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::GoByHndl( __u64 A_Hnd )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrPlaySoloV1MsgGoByHndl	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Hd.MsgHd.Cmd			=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd				=	E_GrPlaySoloV1CmdGoHndl;
		Tv_Msg.Hndl							=	A_Hnd;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcProcPlay( void )
{
	// local -------------------
	// code --------------------
		DoPlay();
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::Play( void )
{
	// local -------------------
		St_GrPlaySoloV1MsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.PlayCmd			=	E_GrPlaySoloV1CmdPlay;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcProcDirSet( BOOL8 A_IsBack )
{
	// local -------------------
	// code --------------------
		PlayDirSet( (BOOL8)A_IsBack );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::DirectionSet( BOOL8 A_IsBack )
{
	// local -------------------
		St_GrPlaySoloV1MsgDirSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd			=	E_GrPlaySoloV1CmdDirSet;
		Tv_Msg.IsBack					=	A_IsBack;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::Stop( void )
{
	// local -------------------
		St_GrPlaySoloV1MsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.PlayCmd			=	E_GrPlaySoloV1CmdStop;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcProcSpeedSet( __u32 A_Speed )
{
	// local -------------------
	// code --------------------
		PlaySpeedSet( (__s32)A_Speed );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::SpeedSet( __u32 A_Speed )
{
	// local -------------------
		St_GrPlaySoloV1MsgSpdSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd			=	E_GrPlaySoloV1CmdSpeedSet;
		Tv_Msg.Speed					=	A_Speed;
		if ( E_GrPlaySoloV1PlaySpeedMax < A_Speed )
		{
			Tv_Msg.Speed					=	E_GrPlaySoloV1PlaySpeedMax;
		}
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::AudioChSet( __u8 A_Ch )
{
	// local -------------------
		St_GrPlaySoloV1MsgAdoChSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd			=	E_GrPlaySoloV1CmdAdoChSet;
		Tv_Msg.Ch							=	A_Ch;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcProcGoByHndl( __u64 A_Hndl )
{
	// local -------------------
	// code --------------------
		JumpByHndl( A_Hndl );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::GoTime( Def_GrTime A_Time )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrPlaySoloV1MsgGoTime	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Hd.MsgHd.Cmd			=	m_MsgAloc;
		Tv_Msg.Hd.PlayCmd				=	E_GrPlaySoloV1CmdGoTime;
		Tv_Msg.Time							=	A_Time;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcDoDispErase( void )
{
	// local -------------------
		__s32	Tv_TimeOut;
	// code --------------------
		//wait not remain play data
		Tv_TimeOut	=	100;
		while ( 0 != m_ObjDev->PlayVdoRemainCntGet() )
		{
			Tv_TimeOut --;
			if ( 0 >= Tv_TimeOut )
			{
				break;
			}
			GrPrcSleep( 10 );		//waiting process
		}
		//do erase
		m_ObjDev->DispErase( TRUE );
		GrPrcSleep( 10 );
		m_ObjDev->DispErase( FALSE );
		GrPrcSleep( 10 );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::StepNext( void )
{
	// local -------------------
		St_GrPlaySoloV1MsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.PlayCmd			=	E_GrPlaySoloV1CmdStepNext;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::StepPrev( void )
{
	// local -------------------
		St_GrPlaySoloV1MsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.PlayCmd			=	E_GrPlaySoloV1CmdStepPrev;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcProcFrmNext( void )
{
		MoveNext();
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcProcFrmPrev( void )
{
		MovePrev();
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcProcStop( void )
{
		DoStop();
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcProcGoTime( Def_GrTime A_Time )
{
		JumpByTime( A_Time );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcMprcAdoChSet( __u8 A_Ch )
{
	// local -------------------
		__u16	Tv_Mask;
	// code --------------------
		Tv_Mask	=	1 << A_Ch;
		AudioChEnable( Tv_Mask );
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::LcChkPlayStatChg( void )
{
	// local -------------------
		//St_GrTaskMsgHd	Tv_Msg;
	// code --------------------
		/*
		if ( (m_PlayStat != m_PtrGvs->PlayStat) || (m_PlaySpeed != m_PtrGvs->PlaySpd) || (m_IsDirBack != m_PtrGvs->IsPlayBack) )
		{
			// update
			m_PtrGvs->PlayStat		=	m_PlayStat;
			m_PtrGvs->PlaySpd			=	m_PlaySpeed;
			m_PtrGvs->IsPlayBack	=	m_IsDirBack;
			// notify
			Tv_Msg.Cmd	=	E_GrTaskMsgPlayStatChg;
			V_DvrRtlEnv.TskSvMain->MsgPost( &Tv_Msg, sizeof(Tv_Msg) );
		}
		*/
}
//--------------------------------------------------------------------
__u8	Cls_GrPlaySoloV1::DecVdo( Ptr_GrDvrAvHd A_PtrHd, BOOL8 A_IsDisp )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrFs1PlayDecBusy;

		if ( m_ObjDev->PlayVdoData( A_PtrHd, (BOOL8)(!A_IsDisp) ) )
		{
			//update attribute
			//m_ObjDev->PlayAtbSet( Tv_Ch, A_PtrFrm->Evt );
			// sync
			m_DecVdoSyncCnt ++;
			//success
			Tv_Result	=	E_GrDvrFs1PlayDecOk;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrPlaySoloV1::DecAdo( Ptr_GrDvrAvHd A_PtrHd )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrFs1PlayDecBusy;

		if ( m_ObjDev->PlayAdoData( A_PtrHd ) )
		{
			// sync
			m_DecAdoSyncCnt ++;
			//success
			Tv_Result	=	E_GrDvrFs1PlayDecOk;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV1::PrepareLoadFw( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;
		if ( (m_DecVdoSyncCnt >= m_ObjDev->PlayVdoRemainCntGet()) && (m_DecAdoSyncCnt >= m_ObjDev->PlayAdoRemainCntGet() ) )
		{
			Tv_Result	=	TRUE;
#ifdef LINUX_EMU
			// check audio buffer
			LcEmuChkAdoQue();
#endif
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::AfterLoadFw( void )
{
		m_DecVdoSyncCnt	=	0;
		m_DecAdoSyncCnt	=	0;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPlaySoloV1::PrepareLoadBw( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( (m_DecVdoSyncCnt >= m_ObjDev->PlayVdoRemainCntGet()) && (m_DecAdoSyncCnt >= m_ObjDev->PlayAdoRemainCntGet() ) )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::AfterLoadBw( void )
{
		m_DecVdoSyncCnt	=	0;
		m_DecAdoSyncCnt	=	0;
}
//--------------------------------------------------------------------
void	Cls_GrPlaySoloV1::RtlTimer( __u32 A_Hndl )
{
		LcChkPlayStatChg();
}
//--------------------------------------------------------------------
#ifdef LINUX_EMU
void	Cls_GrPlaySoloV1::LcEmuChkAdoQue( void )
{
	// local -------------------
		void*	Tv_PtrDrvMem;
		Ptr_GrDrvSoloDevCtrl	Tv_PtrCtrl;
		Ptr_GrBufQue	Tv_PtrQue;
		Ptr_GrDrvSoloAdoPlayItm	Tv_PtrItm;
		__s32		Tv_PosIn;
		__s32		Tv_PosOut;
		__u8	Tv_BufIdx;
		__u32	Tv_BarSt;
		__u32	Tv_BarEd;
	// code --------------------
		Tv_PtrDrvMem	=	m_ObjDev->DrvMemPtrGet();
		Tv_PtrCtrl	=	(Ptr_GrDrvSoloDevCtrl)m_ObjDev->DevCtrlPtrGet();
		// check queue
		Tv_PtrQue		=	(Ptr_GrBufQue)Tv_PtrCtrl->AdecPlayQue;
		Tv_PosIn		=	Tv_PtrQue->InPos;
		Tv_PosOut		=	Tv_PtrQue->OutPos;

		// get barrier range
		Tv_BufIdx		=	m_PtrSeekCtrl->BufIdx;
		if ( m_PtrSeekCtrl->IsDblBuf )
		{
			if ( 0 != m_PtrSeekCtrl->Buf[Tv_BufIdx].Readed )
			{
				Tv_BufIdx	=	Tv_BufIdx ^ 1;
			}
		}
		Tv_BarSt		=	(__u32)m_PtrSeekCtrl->Buf[Tv_BufIdx].Ptr - (__u32)Tv_PtrDrvMem;
		Tv_BarEd		=	Tv_BarSt + m_PtrSeekCtrl->Buf[Tv_BufIdx].Size;

		while ( Tv_PosIn != Tv_PosOut )
		{
			// get item ptr
			Tv_PtrItm		=	(Ptr_GrDrvSoloAdoPlayItm)( (__u32)Tv_PtrQue +	sizeof(struct St_GrBufQue) + Tv_PosOut );
			// check position
			if ( (Tv_PtrItm->BufPos >= Tv_BarSt) && (Tv_PtrItm->BufPos < Tv_BarEd) )
			{
				// found
				DbgMsgPrint( "Play audio data will overlap!\n" );
				break;
			}
			// next
			Tv_PosOut	=	Tv_PosOut + Tv_PtrQue->SizeItem;
			if ( Tv_PosOut >= Tv_PtrQue->BufSize )
			{
				Tv_PosOut	=	0;
			}
		}

}
#endif
//--------------------------------------------------------------------

