/*
 file system version 1 seeker

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Fs/V1Old/GrDvrFsoSeek.h>
#include	<GrDumyTool.h>
#include	<GrTimeTool.h>
#include	<GrStrTool.h>

#include	<stdlib.h>
#include	<Dvr/Fs/V1Old/GrDvrFsoDsk.h>

//====================================================================
//local const

#define E_GrDvrFsoSeekTimeOutCnt	(480 * 60)

//====================================================================
//local type

typedef	struct St_GrDvrFsoSeekMsgBase 
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrDvrFsoSeekMsgBase;


//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFsoSeek::Cls_GrDvrFsoSeek( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFsoMng* A_FsMng, void* A_PtrBuf, 
																		__u32 A_SizeBuf, void* A_PtrDualBuf, BOOL8 A_IsFsSafe ):
Cls_GrTaskCli()
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		m_TaskSvr	=	A_TaskSvr;
		m_MsgAloc	=	GrTaskSvrMsgCmdAlloc();

		//init
		m_FsMng				=	A_FsMng;
		m_FccStrm			=	m_FsMng->FccStreamGet();
		m_IsFsSafe		=	A_IsFsSafe;
		m_IsLimFirst	=	FALSE;
		m_BufIdx			=	0;
		m_BufSize			=	A_SizeBuf;
		GrDumyZeroMem( m_BufTbl, sizeof(m_BufTbl) );
		m_BufTbl[0].PtrBuf	=	A_PtrBuf;
		m_BufTbl[1].PtrBuf	=	A_PtrDualBuf;
		m_BufXor			=	0;
		if ( NULL != m_BufTbl[1].PtrBuf )
		{
			m_BufXor			=	1;
		}
		m_IsDirBack		=	FALSE;

		m_VdoAbleMap	=	0;
		m_VdoIonlyMap	=	0;
		m_VdoPableMap	=	0;
		m_AdoAbleMap	=	0;

		m_DevType			=	E_GrDvrRecDevNoraml;
		m_CliSync			=	NULL;
		m_IsFrmFirst	=	TRUE;
		m_BlkSkipCnt	=	0;

		m_MinJmpStep	=	0;
		m_MinJmpProc	=	0;
		m_AbleChCnt		=	0;

		m_Hndl				=	E_GrDvrFsoHndlNone;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
		{
			m_ChTbl[Tv_WkIdx].Res		=	E_GrDvrVdoResBad;
			m_ChTbl[Tv_WkIdx].VopId	=	0;
		}
		
		//regist task
		m_TaskSvr->RegistMsgClient( this );

}
//--------------------------------------------------------------------
Cls_GrDvrFsoSeek::~Cls_GrDvrFsoSeek()
{
		//unregister timer
		m_TaskSvr->UnregistMsgClient( this );


		GrTaskSvrMsgCmdFree( m_MsgAloc );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoSeek::DevTypeSet( __u8 A_DevType )
{
		if ( m_DevType != A_DevType )
		{
			m_DevType			=	A_DevType;
			m_VdoPableMap	=	0;		//reset pframe able
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoSeek::DevTypeGet( void )
{
		return	m_DevType;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoSeek::SyncClientSet( Cls_GrTaskCli* A_Cli, __u32 A_Id )
{
		m_CliSync	=	A_Cli;
		m_SyncId	=	A_Id;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::LcIsLoadAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFsoSeekCbMsgChkLoad	Tv_Msg;
	// code --------------------
		Tv_Result	=	TRUE;

		if ( NULL != m_CliSync )
		{
			Tv_Msg.Hd.Cmd				=	E_GrDvrFsoSeekCbCmdChkLoadAble;
			Tv_Msg.Id						=	m_SyncId;
			Tv_Msg.IsWait				=	FALSE;
			Tv_Msg.PtrRtIsAble	=	&Tv_Result;
			m_CliSync->RtlCallback( (Ptr_GrTaskMsgHd)&Tv_Msg );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoSeek::LcWaitCanLoad( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFsoSeekCbMsgChkLoad	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_CliSync )
		{
			Tv_Msg.Hd.Cmd				=	E_GrDvrFsoSeekCbCmdChkLoadAble;
			Tv_Msg.Id						=	m_SyncId;
			Tv_Msg.IsWait				=	TRUE;
			Tv_Msg.PtrRtIsAble	=	&Tv_Result;
			m_CliSync->RtlCallback( (Ptr_GrTaskMsgHd)&Tv_Msg );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoSeek::LcSyncReset( void )
{
	// local -------------------
		St_GrDvrFsoSeekCbMsgSyncReset	Tv_Msg;
	// code --------------------
		if ( NULL != m_CliSync )
		{
			Tv_Msg.Hd.Cmd		=	E_GrDvrFsoSeekCbCmdSyncReset;
			Tv_Msg.Id				=	m_SyncId;
			m_CliSync->RtlCallback( (Ptr_GrTaskMsgHd)&Tv_Msg );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::SeekFirst( void )
{
	// local -------------------
		__u64		Tv_Hndl;
	// code --------------------
		//get handle
		if ( m_IsFsSafe )
		{
			Tv_Hndl	=	m_FsMng->SafeRecBeginHndlGet( m_DevType );
		}
		else
		{
			Tv_Hndl	=	m_FsMng->RecBeginHndlGet( m_DevType );
		}
		return	SeekByHndl( Tv_Hndl );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::SeekLast( void )
{
	// local -------------------
		__u64		Tv_Hndl;
	// code --------------------
		//get handle
		if ( m_IsFsSafe )
		{
			Tv_Hndl	=	m_FsMng->SafeRecEndHndlGet( m_DevType );
		}
		else
		{
			Tv_Hndl	=	m_FsMng->RecEndHndlGet( m_DevType );
		}
		return	SeekByHndl( Tv_Hndl );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::SeekRecent( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_GrTime	Tv_TimeSt;
		Def_GrTime	Tv_TimeEd;
		Def_GrTime	Tv_TimeModSec;
		__u64	Tv_Hndl;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsFsSafe )
		{
			if ( m_FsMng->SafeRecTimeRangeGet( m_DevType, &Tv_TimeSt, &Tv_TimeEd ) )
			{
				//check exist end sec by mod minute
				Tv_TimeModSec	=	Tv_TimeEd % E_GrTimeSecOfMin;
				if ( 6 > Tv_TimeModSec )
				{
					Tv_TimeEd	=	Tv_TimeEd - Tv_TimeModSec - E_GrTimeSecOfMin;
				}
				Tv_Hndl	=	m_FsMng->SafeFindHndlByMinBw( m_DevType, Tv_TimeEd, Tv_TimeSt );
				if ( E_GrDvrFsoHndlNone != Tv_Hndl )
				{
					Tv_Result	=	SeekByHndl( Tv_Hndl );
				}
			}
		}
		else
		{
			if ( m_FsMng->RecTimeRangeGet( m_DevType, &Tv_TimeSt, &Tv_TimeEd ) )
			{
				//check exist end sec by mod minute
				Tv_TimeModSec	=	Tv_TimeEd % E_GrTimeSecOfMin;
				if ( 6 > Tv_TimeModSec )
				{
					Tv_TimeEd	=	Tv_TimeEd - Tv_TimeModSec - E_GrTimeSecOfMin;
				}
				Tv_Hndl	=	m_FsMng->FindHndlByMinBw( m_DevType, Tv_TimeEd, Tv_TimeSt );
				if ( E_GrDvrFsoHndlNone != Tv_Hndl )
				{
					Tv_Result	=	SeekByHndl( Tv_Hndl );
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::SeekByMinute( Def_GrTime A_Time )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u64		Tv_Hndl;
		BOOL8		Tv_IsDirBk;
	// code --------------------
		Tv_IsDirBk	=	m_IsDirBack;
		DirectionSet( FALSE );	//direction set
		if ( m_IsFsSafe )
		{
			Tv_Hndl	=	m_FsMng->SafeRecHndlByTimeMin( m_DevType, A_Time );
		}
		else
		{
			Tv_Hndl	=	m_FsMng->RecHndlByTimeMin( m_DevType, A_Time );
		}
		Tv_Result	=	SeekByHndl( Tv_Hndl );
		DirectionSet( Tv_IsDirBk );		// restore direction
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::SeekByRngMin( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u64		Tv_Hndl;
		BOOL8		Tv_IsDirBk;
	// code --------------------
		Tv_IsDirBk	=	m_IsDirBack;
		DirectionSet( FALSE );	//direction set
		if ( m_IsFsSafe )
		{
			Tv_Hndl	=	m_FsMng->SafeFindHndlByMinFw( m_DevType, A_TimeSt, A_TimeEd );
		}
		else
		{
			Tv_Hndl	=	m_FsMng->FindHndlByMinFw( m_DevType, A_TimeSt, A_TimeEd );
		}
		Tv_Result	=	SeekByHndl( Tv_Hndl );
		DirectionSet( Tv_IsDirBk );
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::SeekByHndl( __u64 A_Hndl )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrFsoHndlNone != A_Hndl )
		{
			// Tv_Result	=	E_GrDvrRecErrDataEmpty;
			m_Hndl	=	A_Hndl;
			DbgMsgPrint( "Seek By Hndl = %8X%08X \n", (__u32)( (A_Hndl >> 32) & 0xFFFFFFFFLL ) , (__u32)( A_Hndl & 0xFFFFFFFFLL ) );
			if ( LcReadNow() )
			{
				Tv_Result	=	TRUE;
			}
			m_MinJmpProc	=	0;		//min jump reset
			m_LastOutTime	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::LcLoadJmpBack( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_GrTime	Tv_TimeSt;
		Def_GrTime	Tv_TimeEd;
		Def_GrTime	Tv_TimeTg;
		__u64	Tv_Hndl;
	// code --------------------
		Tv_Result	=	FALSE;

		//get handle
		Tv_TimeTg		=	m_LastOutTime - (E_GrTimeSecOfMin * (__u32)m_MinJmpStep);
		if ( m_IsFsSafe )
		{
			if ( m_FsMng->SafeRecTimeRangeGet( m_DevType, &Tv_TimeSt, &Tv_TimeEd ) )
			{
				Tv_Hndl	=	m_FsMng->SafeFindHndlByMinBw( m_DevType, Tv_TimeTg, Tv_TimeSt );
			}
		}
		else
		{
			if ( m_FsMng->RecTimeRangeGet( m_DevType, &Tv_TimeSt, &Tv_TimeEd ) )
			{
				Tv_Hndl	=	m_FsMng->FindHndlByMinBw( m_DevType, Tv_TimeTg, Tv_TimeSt );
			}
		}
		//check able handle
		if ( E_GrDvrFsoHndlNone != Tv_Hndl )
		{
			m_Hndl	=	Tv_Hndl;
			Tv_Result	=	LcReadBack();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::LcLoadJmpPrev( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_GrTime	Tv_TimeSt;
		Def_GrTime	Tv_TimeEd;
		Def_GrTime	Tv_TimeTg;
		__u64	Tv_Hndl;
	// code --------------------
		Tv_Result	=	FALSE;

		//get handle
		Tv_TimeTg		=	m_LastOutTime - (E_GrTimeSecOfMin * (__u32)m_MinJmpStep);
		if ( m_IsFsSafe )
		{
			if ( m_FsMng->SafeRecTimeRangeGet( m_DevType, &Tv_TimeSt, &Tv_TimeEd ) )
			{
				Tv_Hndl	=	m_FsMng->SafeFindHndlByMinBw( m_DevType, Tv_TimeTg, Tv_TimeSt );
			}
		}
		else
		{
			if ( m_FsMng->RecTimeRangeGet( m_DevType, &Tv_TimeSt, &Tv_TimeEd ) )
			{
				Tv_Hndl	=	m_FsMng->FindHndlByMinBw( m_DevType, Tv_TimeTg, Tv_TimeSt );
			}
		}
		//check able handle
		if ( E_GrDvrFsoHndlNone != Tv_Hndl )
		{
			m_Hndl	=	Tv_Hndl;
			m_BufTbl[m_BufIdx].BufGap	=	0;
			m_BufTbl[m_BufIdx].BufPos	=	0;
			Tv_Result	=	LcReadPrev();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::LcLoadJmpNext( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_GrTime	Tv_TimeSt;
		Def_GrTime	Tv_TimeEd;
		Def_GrTime	Tv_TimeTg;
		__u64	Tv_Hndl;
	// code --------------------
		Tv_Result	=	FALSE;

		//get handle
		Tv_TimeTg		=	m_LastOutTime + (E_GrTimeSecOfMin * (__u32)m_MinJmpStep);
		if ( m_IsFsSafe )
		{
			if ( m_FsMng->SafeRecTimeRangeGet( m_DevType, &Tv_TimeSt, &Tv_TimeEd ) )
			{
				Tv_Hndl	=	m_FsMng->SafeFindHndlByMinFw( m_DevType, Tv_TimeTg, Tv_TimeEd );
			}
		}
		else
		{
			if ( m_FsMng->RecTimeRangeGet( m_DevType, &Tv_TimeSt, &Tv_TimeEd ) )
			{
				Tv_Hndl	=	m_FsMng->FindHndlByMinFw( m_DevType, Tv_TimeTg, Tv_TimeEd );
			}
		}

		//check able handle
		if ( E_GrDvrFsoHndlNone != Tv_Hndl )
		{
			m_Hndl	=	Tv_Hndl;
			Tv_Result	=	LcReadNow();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::LcChkAbleFrm( Ptr_GrDvrAvHd A_PtrFrm )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Ch;
		__u16	Tv_MaskCh;
		__u16	Tv_MaskAble;
	// code --------------------
		Tv_Result	=	FALSE;

		// get frame info
		Tv_Ch				=	A_PtrFrm->Ch;
		Tv_MaskCh		=	1 << (__u16)Tv_Ch;

		//check type
		switch( A_PtrFrm->Type )
		{
			case	E_GrDvrAvTypVdoI:
				m_ChTbl[Tv_Ch].VopId	=	A_PtrFrm->VopId;
				m_ChTbl[Tv_Ch].Res		=	A_PtrFrm->Res;
				Tv_MaskAble	=	Tv_MaskCh & m_VdoAbleMap;
				if ( 0 != Tv_MaskAble )
				{
					Tv_Result	=	TRUE;
					m_VdoPableMap	=	m_VdoPableMap | (Tv_MaskCh & (~m_VdoIonlyMap));	//update pframe able
				}
				else
				{
					//remove p frame able
					m_VdoPableMap	=	m_VdoPableMap & (~Tv_MaskCh);
				}
				break;
			case	E_GrDvrAvTypVdoP:
				Tv_MaskAble	=	Tv_MaskCh & (~m_VdoIonlyMap) & m_VdoPableMap;		//pframe able
				Tv_MaskAble	=	Tv_MaskAble & m_VdoAbleMap;
				if (	(0 != Tv_MaskAble) && (m_ChTbl[Tv_Ch].VopId == A_PtrFrm->VopId) && 
					(m_ChTbl[Tv_Ch].Res == A_PtrFrm->Res)	&& (0 == m_MinJmpStep) && (!m_IsDirBack)	)
				{
					Tv_Result	=	TRUE;
					m_VdoPableMap	=	m_VdoPableMap | (Tv_MaskCh & (~m_VdoIonlyMap));	//update pframe able
				}
				else
				{
					//remove p frame able
					m_VdoPableMap	=	m_VdoPableMap & (~Tv_MaskCh);
				}
				break;
			case	E_GrDvrAvTypAdo:
				//audio data
				if ( (!m_IsDirBack) && (0 != (Tv_MaskCh & m_AdoAbleMap)) )
				{
					Tv_Result	=	TRUE;
				}
				break;
			case E_GrDvrAvTypRsvAdo:
				//DbgMsgPrint("Cls_GrDvrFsoSeek::GetNextFrame - reserve audio type.\n");
				break;
			case E_GrDvrAvTypReserve:
				//DbgMsgPrint("Cls_GrDvrFsoSeek::GetNextFrame - reserve area.\n");
				break;
			case E_GrDvrAvTypNotuse:
				//DbgMsgPrint("Cls_GrDvrFsoSeek::GetNextFrame - not use area.\n");
				break;
			default:
				DbgMsgPrint("Cls_GrDvrFsoSeek::GetNextFrame - unknown type.\n");
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrDvrFsoSeek::LcFrmNext( BOOL8* A_PtrRtIsEnd )
{
	// local -------------------
		void*	Tv_Result;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		__u32	Tv_PktSize;
		BOOL8		Tv_IsReqLoad;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtIsEnd	=	FALSE;

		Tv_IsReqLoad	=	FALSE;

		// check able size
		while ( NULL == Tv_Result )
		{
			if ( m_BufTbl[m_BufIdx].BufInSize < (m_BufTbl[m_BufIdx].BufPos + sizeof(St_GrDvrAvHd) ) )
			{
				Tv_IsReqLoad	=	TRUE;
				break;
			}
			Tv_PktSize		=	32;
			// get AV header
			Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)( (__u32)m_BufTbl[m_BufIdx].PtrBuf + m_BufTbl[m_BufIdx].BufPos );
			// check current AV head
			if ( m_FccStrm == Tv_PtrAvHd->Fcc )
			{
				Tv_PktSize	=	(Tv_PtrAvHd->DataSize + sizeof(St_GrDvrAvHd) + E_GrDvrRecTailMarkSize + E_GrDvrFsoStrmAlignSize - 1) & E_GrDvrFsoStrmAlignMask;
				// check packet size
				if ( m_BufTbl[m_BufIdx].BufInSize < (m_BufTbl[m_BufIdx].BufPos + Tv_PktSize ) )
				{
					Tv_IsReqLoad	=	TRUE;
					break;
				}
				// check able frame
				if ( LcChkAbleFrm( Tv_PtrAvHd ) )
				{
					Tv_Result	=	(void*)Tv_PtrAvHd;
				}
			}
			// next packet
			m_BufTbl[m_BufIdx].BufPos	=	m_BufTbl[m_BufIdx].BufPos + Tv_PktSize;
		}

		// load
		if ( Tv_IsReqLoad )
		{
			if ( LcIsLoadAble() )
			{
				// load
				if ( !LcReadNext() )
				{
					*A_PtrRtIsEnd	=	TRUE;
				}
			}
		}

		//DbgMsgPrint( "Cls_GrDvrFsoSeek::LcFrmNext - now(hndl:%08X%08X , pos: %8X)  \n", (__u32)(m_Hndl >> 32), (__u32)(m_Hndl & 0xFFFFFFFF), m_BufTbl[m_BufIdx].BufPos );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrDvrFsoSeek::LcFrmPrev( BOOL8* A_PtrRtIsEnd )
{
	// local -------------------
		void*	Tv_Result;
		__u32*	Tv_PtrTail;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		__u32	Tv_PktSize;
		__u32	Tv_TailSize;
		BOOL8	Tv_IsReqLoad;
		BOOL8	Tv_IsPktLoss;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtIsEnd	=	FALSE;

		Tv_IsReqLoad	=	FALSE;
		Tv_IsPktLoss	=	FALSE;

		// check able size
		while ( NULL == Tv_Result )
		{
			if ( sizeof(St_GrDvrAvHd) >= m_BufTbl[m_BufIdx].BufPos )
			{
				Tv_IsReqLoad	=	TRUE;
				break;
			}

			Tv_PktSize		=	E_GrDvrFsoStrmAlignSize;

			if ( Tv_IsPktLoss )
			{
				// check header
				Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)( (__u32)m_BufTbl[m_BufIdx].PtrBuf + m_BufTbl[m_BufIdx].BufPos );
				// check FCC
				if ( m_FccStrm == Tv_PtrAvHd->Fcc )
				{
					Tv_IsPktLoss	=	FALSE;
				}
			}

			if ( !Tv_IsPktLoss )
			{
				// get tail
				Tv_PtrTail	=	(__u32*)( (__u32)m_BufTbl[m_BufIdx].PtrBuf + m_BufTbl[m_BufIdx].BufPos - E_GrDvrRecTailMarkSize );
				// check tail mark
				if ( E_GrDvrRecTailMarkId == (E_GrDvrRecTailMarkMask & (*Tv_PtrTail)) )
				{
					Tv_TailSize	=	E_GrDvrRecTailSizeMask & (*Tv_PtrTail);
					// check align
					if ( 0 == ( (E_GrDvrFsoStrmAlignSize - 1) & Tv_TailSize) )
					{
						// check size
						if ( m_BufTbl[m_BufIdx].BufPos < Tv_TailSize )
						{
							Tv_IsReqLoad	=	TRUE;
							break;
						}
						// check header
						Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)( (__u32)m_BufTbl[m_BufIdx].PtrBuf + m_BufTbl[m_BufIdx].BufPos - Tv_TailSize );
						// check FCC
						if ( m_FccStrm == Tv_PtrAvHd->Fcc )
						{
							Tv_PktSize	=	Tv_TailSize;	// update packet size
							// check able frame
							if ( LcChkAbleFrm( Tv_PtrAvHd ) )
							{
								Tv_Result	=	(void*)Tv_PtrAvHd;
							}
						}
						else
						{
							Tv_IsPktLoss	=	TRUE;
							DbgMsgPrint( "Cls_GrDvrFsoSeek::LcFrmPrev - packet stream loss!(hndl:%08X%08X , pos: %8X)  \n", (__u32)(m_Hndl >> 32), (__u32)(m_Hndl & 0xFFFFFFFF), m_BufTbl[m_BufIdx].BufPos );
						}
					}
					else
					{
						Tv_IsPktLoss	=	TRUE;
						DbgMsgPrint( "Cls_GrDvrFsoSeek::LcFrmPrev - packet align loss!(hndl:%08X%08X , pos: %8X)  \n", (__u32)(m_Hndl >> 32), (__u32)(m_Hndl & 0xFFFFFFFF), m_BufTbl[m_BufIdx].BufPos );
					}
				}
				else
				{
					Tv_IsPktLoss	=	TRUE;
					DbgMsgPrint( "Cls_GrDvrFsoSeek::LcFrmPrev - packet id loss!(hndl:%08X%08X , pos: %8X)  \n", (__u32)(m_Hndl >> 32), (__u32)(m_Hndl & 0xFFFFFFFF), m_BufTbl[m_BufIdx].BufPos );
				}
			}

			// next packet
			m_BufTbl[m_BufIdx].BufPos	=	m_BufTbl[m_BufIdx].BufPos - Tv_PktSize;
		}

		// load
		if ( Tv_IsReqLoad )
		{
			if ( LcIsLoadAble() )
			{
				// load
				if ( !LcReadPrev() )
				{
					*A_PtrRtIsEnd	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrDvrFsoSeek::LcFrmMinNext( BOOL8* A_PtrRtIsEnd )
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtIsEnd	=	FALSE;

		if ( (0 != m_LastOutTime) && (m_MinJmpProc >= m_AbleChCnt) )
		{
			if ( LcIsLoadAble() )
			{
				if ( LcLoadJmpNext() )
				{
					m_MinJmpProc	=	0;
				}
				else
				{
					*A_PtrRtIsEnd	=	TRUE;
				}
			}
		}
		else
		{
			Tv_Result	=	LcFrmNext( A_PtrRtIsEnd );
			if ( NULL != Tv_Result )
			{
				m_MinJmpProc ++;
				m_LastOutTime	=	((Ptr_GrDvrAvHd)Tv_Result)->TimeSec;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrDvrFsoSeek::LcFrmMinPrev( BOOL8* A_PtrRtIsEnd )
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtIsEnd	=	FALSE;

		if ( (0 != m_LastOutTime) && (m_MinJmpProc >= m_AbleChCnt) )
		{
			if ( LcIsLoadAble() )
			{
				if ( LcLoadJmpPrev() )
				{
					m_MinJmpProc	=	0;
				}
				else
				{
					*A_PtrRtIsEnd	=	TRUE;
				}
			}
		}
		else
		{
			Tv_Result	=	LcFrmPrev( A_PtrRtIsEnd );
			if ( NULL != Tv_Result )
			{
				m_MinJmpProc ++;
				m_LastOutTime	=	((Ptr_GrDvrAvHd)Tv_Result)->TimeSec;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrDvrFsoSeek::GetNextFrame( BOOL8* A_PtrRtIsEnd )
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtIsEnd	=	FALSE;

		if ( (0 != m_VdoAbleMap) || (0 != m_AdoAbleMap) )
		{
			if ( m_IsDirBack )
			{
				if ( 0 != m_MinJmpStep )
				{
					Tv_Result	=	LcFrmMinPrev( A_PtrRtIsEnd );
				}
				else
				{
					Tv_Result	=	LcFrmPrev( A_PtrRtIsEnd );
				}
			}
			else
			{
				if ( 0 != m_MinJmpStep )
				{
					Tv_Result	=	LcFrmMinNext( A_PtrRtIsEnd );
				}
				else
				{
					Tv_Result	=	LcFrmNext( A_PtrRtIsEnd );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrDvrFsoSeek::GetNextFrameOld( BOOL8* A_PtrRtIsEnd )
{
	// local -------------------
		void*		Tv_Result;
		__u32		Tv_PktSize;
		BOOL8		Tv_IsBadStrm;
		__u8		Tv_Ch;
		__u16		Tv_MaskCh;
		__u16		Tv_MaskAble;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		__u32*	Tv_PtrTail;
		__u32		Tv_ChkCnt;			//check count
	// code --------------------
		//init
		Tv_Result	=	NULL;
		*A_PtrRtIsEnd	=	TRUE;
		//check mask zero
		if ( (0 != m_VdoAbleMap) || (0 != m_AdoAbleMap) )
		{
			//check
			*A_PtrRtIsEnd	=	FALSE;
			for ( Tv_ChkCnt=0;1 > Tv_ChkCnt;Tv_ChkCnt++ )
			{
				//init
				Tv_PtrAvHd	=	NULL;		//reset packet address

				//check next jump
				if ( (0 != m_LastOutTime) && (0 != m_MinJmpStep) && (m_MinJmpProc >= m_AbleChCnt) )
				{
					//check is load able
					if ( LcIsLoadAble() )
					{
						//check direction
						if ( m_IsDirBack )
						{
							//load min jump
							if ( !LcLoadJmpBack() )
							{
								*A_PtrRtIsEnd	=	TRUE;
								break;
							}
						}
						else
						{
							if ( !LcLoadJmpNext() )
							{
								*A_PtrRtIsEnd	=	TRUE;
								break;
							}
						}
						m_MinJmpProc	=	0;
					}
				}
				else
				{
					//check load after first
					if ( m_IsFrmFirst )
					{
						//first time work
						if ( m_IsDirBack )
						{
							//rewind work
							if ( m_BufTbl[m_BufIdx].BufGap < m_BufTbl[m_BufIdx].BufPos )
							{
								// init
								Tv_IsBadStrm	=	TRUE;
								//get packet pointer
								Tv_PtrTail	=	(__u32*)( (__u32)m_BufTbl[m_BufIdx].PtrBuf + m_BufTbl[m_BufIdx].BufPos - E_GrDvrRecTailMarkSize );
								// check current tail mark
								if ( E_GrDvrRecTailMarkId == (E_GrDvrRecTailMarkMask & (*Tv_PtrTail)) )
								{
									Tv_PktSize	=	E_GrDvrRecTailSizeMask & (*Tv_PtrTail);
									if ( (sizeof(St_GrDvrAvHd) <= Tv_PktSize) && (0x40000 > Tv_PktSize) )
									{
										Tv_IsBadStrm	=	FALSE;

										m_BufTbl[m_BufIdx].BufPos	=	m_BufTbl[m_BufIdx].BufPos - Tv_PktSize;
										Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)( (__u32)m_BufTbl[m_BufIdx].PtrBuf + m_BufTbl[m_BufIdx].BufPos );
										//check type
										if ( E_GrDvrAvTypAdo >= Tv_PtrAvHd->Type )
										{
											//check auto delete time
											if ( m_FsMng->AutoDelLimTimeGet() > Tv_PtrAvHd->TimeSec )
											{
												//time over finish
												*A_PtrRtIsEnd	=	TRUE;
												break;
											}
										}

									}
								}
								// check bad stream
								if ( Tv_IsBadStrm )
								{
									// move next position
									m_BufTbl[m_BufIdx].BufPos	=	(m_BufTbl[m_BufIdx].BufPos - E_GrDvrFsoStrmAlignSize) & E_GrDvrFsoStrmAlignMask;
								}
							}
						}
						else
						{
							//forward work
							if ( (m_BufTbl[m_BufIdx].BufGap + m_BufTbl[m_BufIdx].BufInSize) > m_BufTbl[m_BufIdx].BufPos )
							{
								Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)( (__u32)m_BufTbl[m_BufIdx].PtrBuf + m_BufTbl[m_BufIdx].BufPos );
								Tv_PktSize	=	E_GrDvrFsoStrmAlignMask & 
									(Tv_PtrAvHd->DataSize + sizeof(St_GrDvrAvHd) + E_GrDvrRecTailMarkSize + 
									E_GrDvrFsoStrmAlignSize - 1);	//get frame size
							}
						}
						//not first
						m_IsFrmFirst	=	FALSE;
					}
					else
					{
						//not first time work
						//check direction
						if ( m_IsDirBack ) 
						{
							//reverse direction
							//check pointer move able
							if ( m_BufTbl[m_BufIdx].BufGap < m_BufTbl[m_BufIdx].BufPos )
							{
								//get packet pointer
								Tv_PtrTail	=	(__u32*)( (__u32)m_BufTbl[m_BufIdx].PtrBuf + m_BufTbl[m_BufIdx].BufPos - E_GrDvrRecTailMarkSize );
								Tv_IsBadStrm	=	TRUE;
								if ( E_GrDvrRecTailMarkId == (E_GrDvrRecTailMarkMask & (*Tv_PtrTail)) )
								{
									Tv_PktSize	=	E_GrDvrRecTailSizeMask & (*Tv_PtrTail);
									if ( (sizeof(St_GrDvrAvHd) <= Tv_PktSize) && (0x40000 > Tv_PktSize) )
									{
										Tv_IsBadStrm	=	FALSE;
										m_BufTbl[m_BufIdx].BufPos	=	m_BufTbl[m_BufIdx].BufPos - Tv_PktSize;
									}
								}
								if ( Tv_IsBadStrm )
								{
									m_BufTbl[m_BufIdx].BufPos	=	(m_BufTbl[m_BufIdx].BufPos - E_GrDvrFsoStrmAlignSize) & E_GrDvrFsoStrmAlignMask;
#ifndef LINUX_APP
									DbgMsgPrint("Cls_GrDvrFsoSeek::GetNextFrame - bad stream exist! Hndl:%08X%08X \n", (__u32)(m_Hndl >> 32), (__u32)(m_Hndl & 0xFFFFFFFF) );
#endif
								}
								
							}
							else
							{
								//load prev
								if ( LcIsLoadAble() )
								{
									if ( !LcReadBack() )
									{
										*A_PtrRtIsEnd	=	TRUE;
									}
								}
								break;
							}
							//update result
							Tv_PtrAvHd	=	NULL;
							if ( (m_BufTbl[m_BufIdx].BufGap <= m_BufTbl[m_BufIdx].BufPos) && ((m_BufTbl[m_BufIdx].BufGap + m_BufTbl[m_BufIdx].BufInSize) > m_BufTbl[m_BufIdx].BufPos) )
							{
								Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)( (__u32)m_BufTbl[m_BufIdx].PtrBuf + m_BufTbl[m_BufIdx].BufPos );
								Tv_PktSize	=	E_GrDvrFsoStrmAlignMask & 
									(Tv_PtrAvHd->DataSize + sizeof(St_GrDvrAvHd) + E_GrDvrRecTailMarkSize + 
									E_GrDvrFsoStrmAlignSize - 1);	//get frame size
							}
						}
						else
						{
							//normal direction
							//check pointer move able
							if ( (m_BufTbl[m_BufIdx].BufGap + m_BufTbl[m_BufIdx].BufInSize) > m_BufTbl[m_BufIdx].BufPos )
							{
								//get packet pointer
								Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)( (__u32)m_BufTbl[m_BufIdx].PtrBuf + m_BufTbl[m_BufIdx].BufPos );
								// check head
								Tv_IsBadStrm	=	TRUE;
								if ( m_FccStrm == Tv_PtrAvHd->Fcc )
								{
									Tv_PktSize	=	E_GrDvrFsoStrmAlignMask & 
										(Tv_PtrAvHd->DataSize + sizeof(St_GrDvrAvHd) + E_GrDvrRecTailMarkSize + 
										E_GrDvrFsoStrmAlignSize - 1);	//get frame size
									if ( (0 != Tv_PktSize) && (0x40000 > Tv_PktSize) )
									{
										Tv_IsBadStrm	=	FALSE;
										m_BufTbl[m_BufIdx].BufPos	=	m_BufTbl[m_BufIdx].BufPos + Tv_PktSize;
										if ( (0 == Tv_PktSize) || (0x40000 <= Tv_PktSize) )
										{
											DbgMsgPrint("Cls_GrDvrFsoSeek::GetNextFrame - bad stream size ( %d )!\n", Tv_PktSize);
										}
									}
								}
								if ( Tv_IsBadStrm	 )
								{
									m_BufTbl[m_BufIdx].BufPos	=	(m_BufTbl[m_BufIdx].BufPos + E_GrDvrFsoStrmAlignSize) & E_GrDvrFsoStrmAlignMask;
#ifndef LINUX_APP
									DbgMsgPrint("Cls_GrDvrFsoSeek::GetNextFrame - bad stream exist! Hndl:%08X%08X \n", (__u32)(m_Hndl >> 32), (__u32)(m_Hndl & 0xFFFFFFFF) );
#endif
								}
							}
							else
							{
								//load next
								//check able
								//DbgMsgPrint( "[CHK1]\n" );
								if ( LcIsLoadAble() )
								{
									//DbgMsgPrint( "[CHK2]\n" );
									if ( !LcReadNext() )
									{
										//DbgMsgPrint( "[CHK3]\n" );
										*A_PtrRtIsEnd	=	TRUE;
									}
									//DbgMsgPrint( "[CHK4]\n" );
								}
								break;
							}
							//update result
							Tv_PtrAvHd	=	NULL;
							if ( (m_BufTbl[m_BufIdx].BufGap <= m_BufTbl[m_BufIdx].BufPos) && ((m_BufTbl[m_BufIdx].BufGap + m_BufTbl[m_BufIdx].BufInSize) > m_BufTbl[m_BufIdx].BufPos) )
							{
								Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)( (__u32)m_BufTbl[m_BufIdx].PtrBuf + m_BufTbl[m_BufIdx].BufPos );
								Tv_PktSize	=	E_GrDvrFsoStrmAlignMask & 
									(Tv_PtrAvHd->DataSize + sizeof(St_GrDvrAvHd) + E_GrDvrRecTailMarkSize + 
									E_GrDvrFsoStrmAlignSize - 1);	//get frame size
							}
						}
					}

					//check now frame able
					if ( NULL != Tv_PtrAvHd )
					{
						//check stream FCC
						if ( m_FccStrm == Tv_PtrAvHd->Fcc )
						{
							//get frame base info
							Tv_Ch				=	Tv_PtrAvHd->Ch;
							Tv_MaskCh		=	1 << (__u16)Tv_Ch;
							//check type
							switch( Tv_PtrAvHd->Type )
							{
								case	E_GrDvrAvTypVdoI:
									m_ChTbl[Tv_Ch].VopId	=	Tv_PtrAvHd->VopId;
									m_ChTbl[Tv_Ch].Res		=	Tv_PtrAvHd->Res;
									Tv_MaskAble	=	Tv_MaskCh & m_VdoAbleMap;
									if ( 0 != Tv_MaskAble )
									{
										Tv_Result	=	(void*)Tv_PtrAvHd;
										m_VdoPableMap	=	m_VdoPableMap | (Tv_MaskCh & (~m_VdoIonlyMap));	//update pframe able
									}
									else
									{
										//remove p frame able
										m_VdoPableMap	=	m_VdoPableMap & (~Tv_MaskCh);
									}
									break;
								case	E_GrDvrAvTypVdoP:
									Tv_MaskAble	=	Tv_MaskCh & (~m_VdoIonlyMap) & m_VdoPableMap;		//pframe able
									Tv_MaskAble	=	Tv_MaskAble & m_VdoAbleMap;
									if (	(0 != Tv_MaskAble) && (m_ChTbl[Tv_Ch].VopId == Tv_PtrAvHd->VopId) && 
												(m_ChTbl[Tv_Ch].Res == Tv_PtrAvHd->Res)	&& (0 == m_MinJmpStep)	)
									{
										Tv_Result	=	(void*)Tv_PtrAvHd;
										m_VdoPableMap	=	m_VdoPableMap | (Tv_MaskCh & (~m_VdoIonlyMap));	//update pframe able
									}
									else
									{
										//remove p frame able
										m_VdoPableMap	=	m_VdoPableMap & (~Tv_MaskCh);
									}
									break;
								case	E_GrDvrAvTypAdo:
									//audio data
									if ( (!m_IsDirBack) && (0 != (Tv_MaskCh & m_AdoAbleMap)) )
									{
										Tv_Result	=	(void*)Tv_PtrAvHd;
									}
									break;
								case E_GrDvrAvTypRsvAdo:
									//DbgMsgPrint("Cls_GrDvrFsoSeek::GetNextFrame - reserve audio type.\n");
									break;
								case E_GrDvrAvTypReserve:
									//DbgMsgPrint("Cls_GrDvrFsoSeek::GetNextFrame - reserve area.\n");
									break;
								case E_GrDvrAvTypNotuse:
									//DbgMsgPrint("Cls_GrDvrFsoSeek::GetNextFrame - not use area.\n");
									break;
								default:
									DbgMsgPrint("Cls_GrDvrFsoSeek::GetNextFrame - unknown type.\n");
									break;
							}
						}
						else
						{
							//bad stream type
#ifndef LINUX_APP
							DbgMsgPrint("Cls_GrDvrFsoSeek::GetNextFrame - bad stream exist! Hndl:%08X%08X \n", (__u32)(m_Hndl >> 32), (__u32)(m_Hndl & 0xFFFFFFFF) );
#endif
							//*A_PtrRtIsEnd	=	TRUE;
						}
					}
				}

				//check found
				if ( NULL != Tv_Result )
				{
					if ( 0 != m_MinJmpStep )
					{
						m_MinJmpProc ++;
					}
					m_LastOutTime	=	((Ptr_GrDvrAvHd)Tv_Result)->TimeSec;
					break;
				}
			}
		}

		/*
		if ( 0 == m_BufTbl[m_BufIdx].BufInSize )
		{
			DbgMsgPrint( "Seek - Buf Idx %d Pos %8X Buf %8X Gap %8X \n", m_BufIdx, m_BufTbl[m_BufIdx].BufPos, m_BufTbl, m_BufTbl[m_BufIdx].BufGap );
		}
		*/

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::LcReadNow( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ErrCode;
	// code --------------------
		Tv_Result	=	FALSE;
		//check can read
		if ( E_GrDvrFsoHndlNone != m_Hndl )
		{
			//get buffer index
			m_BufIdx	=	m_BufIdx ^ m_BufXor;
			LcSyncReset();
			//load next data
			if ( m_IsFsSafe )
			{
				Tv_ErrCode	=	m_FsMng->SafeReadNormal(	m_DevType, m_Hndl, m_BufTbl[m_BufIdx].PtrBuf, m_BufSize, 
					&m_BufTbl[m_BufIdx].BufInSize, &m_Hndl );
			}
			else
			{
				Tv_ErrCode	=	m_FsMng->ReadNormal(	m_DevType, m_Hndl, m_BufTbl[m_BufIdx].PtrBuf, m_BufSize, 
					&m_BufTbl[m_BufIdx].BufInSize, &m_Hndl );
			}
			if ( E_GrDvrRecErrNone == Tv_ErrCode )
			{
				m_BufTbl[m_BufIdx].BufGap	=	0;
				m_BufTbl[m_BufIdx].BufPos	=	0;
				if ( m_IsDirBack )
				{
					//set back play position
					m_BufTbl[m_BufIdx].BufPos	=	m_BufTbl[m_BufIdx].BufInSize;
				}
				m_IsLimFirst	=	FALSE;
				Tv_Result			=	TRUE;
			}
			else
			{
				//bad next handle
				m_BufTbl[m_BufIdx].BufPos			=	0;
				m_BufTbl[m_BufIdx].BufInSize	=	0;
				m_Hndl	=	E_GrDvrFsoHndlNone;
			}
			//reset frame
			m_IsFrmFirst	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::LcReadNext( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ErrCode;
	// code --------------------
		Tv_Result	=	FALSE;
		//check can read
		if ( E_GrDvrFsoHndlNone != m_Hndl )
		{
			//first end
			if ( m_IsLimFirst )
			{
				if ( m_IsFsSafe )
				{
					m_Hndl	=	m_FsMng->SafeRecBeginHndlGet( m_DevType );
				}
				else
				{
					m_Hndl	=	m_FsMng->RecBeginHndlGet( m_DevType );
				}
			}
			//get prediction handle
			m_Hndl	=	m_Hndl + (__u64)(m_BufTbl[m_BufIdx].BufPos & 0xFFFFFFF8);
			//get buffer index
			m_BufIdx	=	m_BufIdx ^ m_BufXor;
			LcSyncReset();
			//load next data
			if ( m_IsFsSafe )
			{
				Tv_ErrCode	=	m_FsMng->SafeReadNormal(	m_DevType, m_Hndl, m_BufTbl[m_BufIdx].PtrBuf, m_BufSize, 
					&m_BufTbl[m_BufIdx].BufInSize, &m_Hndl );
			}
			else
			{
				Tv_ErrCode	=	m_FsMng->ReadNormal(	m_DevType, m_Hndl, m_BufTbl[m_BufIdx].PtrBuf, m_BufSize, 
					&m_BufTbl[m_BufIdx].BufInSize, &m_Hndl );
			}
			if ( E_GrDvrRecErrNone == Tv_ErrCode )
			{
				m_BufTbl[m_BufIdx].BufGap	=	0;
				m_BufTbl[m_BufIdx].BufPos	=	0;
				Tv_Result	=	TRUE;
				m_IsLimFirst	=	FALSE;		//limit first clear
			}
			else
			{
				//bad next handle
				DbgMsgPrint( "Cls_GrDvrFsoSeek::LcReadNext - error code %d \n", Tv_ErrCode );
				m_BufTbl[m_BufIdx].BufPos			=	0;
				m_BufTbl[m_BufIdx].BufInSize	=	0;
			}
			//reset frame
			m_IsFrmFirst	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::LcReadBack( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ErrCode;
	// code --------------------
		Tv_Result	=	FALSE;

		//check now handle
		if ( (E_GrDvrFsoHndlNone != m_Hndl) && (!m_IsFrmFirst) )
		{
			//get buffer index
			m_BufIdx	=	m_BufIdx ^ m_BufXor;
			LcSyncReset();
			//load previous data
			if ( m_IsFsSafe )
			{
				Tv_ErrCode	=	m_FsMng->SafeReadBack(	m_DevType, m_Hndl, m_BufTbl[m_BufIdx].PtrBuf, m_BufSize, 
					&m_BufTbl[m_BufIdx].BufInSize, &m_Hndl );
			}
			else
			{
				Tv_ErrCode	=	m_FsMng->ReadBack(	m_DevType, m_Hndl, m_BufTbl[m_BufIdx].PtrBuf, m_BufSize, 
					&m_BufTbl[m_BufIdx].BufInSize, &m_Hndl );
			}
			if ( E_GrDvrRecErrNone == Tv_ErrCode )
			{
				m_BufTbl[m_BufIdx].BufGap	=	m_BufSize - m_BufTbl[m_BufIdx].BufInSize;
				m_BufTbl[m_BufIdx].BufPos	=	m_BufSize;
				Tv_Result	=	TRUE;
			}
			else
			{
				//bad next handle
				m_BufTbl[m_BufIdx].BufInSize	=	0;
				m_BufTbl[m_BufIdx].BufPos			=	0;
				m_BufTbl[m_BufIdx].BufGap			=	0;
				m_IsLimFirst	=	TRUE;
			}
			//reset frame
			m_IsFrmFirst	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::LcReadPrev( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ErrCode;
	// code --------------------
		Tv_Result	=	FALSE;

		//check now handle
		if ( E_GrDvrFsoHndlNone != m_Hndl )
		{
			// update handle
			if ( 0 != ((m_Hndl & E_GrDvrFsoHndlPosMask) & (E_GrDvrFsoRdaAlignUnit - 1) ) )
			{
				m_Hndl		=	m_Hndl + m_BufTbl[m_BufIdx].BufPos;
			}
			//get buffer index
			m_BufIdx	=	m_BufIdx ^ m_BufXor;
			LcSyncReset();
			//DbgMsgPrint( "PrevRead-before %08X%08X\n", (__u32)(m_Hndl >> 32), (__u32)(m_Hndl &0xFFFFFFFF));
			//load previous data
			if ( m_IsFsSafe )
			{
				Tv_ErrCode	=	m_FsMng->SafeReadPrev(	m_DevType, m_Hndl, m_BufTbl[m_BufIdx].PtrBuf, m_BufSize, 
					&m_BufTbl[m_BufIdx].BufInSize, &m_Hndl );
			}
			else
			{
				Tv_ErrCode	=	m_FsMng->ReadPrev(	m_DevType, m_Hndl, m_BufTbl[m_BufIdx].PtrBuf, m_BufSize, 
					&m_BufTbl[m_BufIdx].BufInSize, &m_Hndl );
			}
			if ( E_GrDvrRecErrNone == Tv_ErrCode )
			{
				//DbgMsgPrint( "PrevRead-after %08X%08X\n", (__u32)(m_Hndl >> 32), (__u32)(m_Hndl &0xFFFFFFFF));
				//m_BufTbl[m_BufIdx].BufGap	=	m_BufSize - m_BufTbl[m_BufIdx].BufInSize;
				m_BufTbl[m_BufIdx].BufGap	=	0;
				m_BufTbl[m_BufIdx].BufPos	=	m_BufSize;
				Tv_Result	=	TRUE;
			}
			else
			{
				//bad next handle
				m_BufTbl[m_BufIdx].BufInSize	=	0;
				m_BufTbl[m_BufIdx].BufPos			=	0;
				m_BufTbl[m_BufIdx].BufGap			=	0;
				m_IsLimFirst	=	TRUE;
			}
			//reset frame
			m_IsFrmFirst	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::SeekBySec( Def_GrTime A_Time )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsEnd;
		Def_GrTime	Tv_TimeSt;
		Def_GrTime	Tv_TimeEd;
		BOOL8	Tv_IsOk;
	// code --------------------
		Tv_Result	=	FALSE;
		//check exist channel
		if ( (0 != m_VdoAbleMap) || (0 != m_AdoAbleMap) )
		{
			//get time range
			if ( m_IsFsSafe )
			{
				Tv_IsOk	=	m_FsMng->SafeRecTimeRangeGet( m_DevType, &Tv_TimeSt, &Tv_TimeEd );
			}
			else
			{
				Tv_IsOk	=	m_FsMng->RecTimeRangeGet( m_DevType, &Tv_TimeSt, &Tv_TimeEd );
			}
			if ( Tv_IsOk )
			{
				//seek
				if ( Tv_TimeSt < A_Time )
				{
					Tv_TimeSt	=	A_Time;
				}
				if ( SeekByRngSec( Tv_TimeSt, Tv_TimeEd, &Tv_IsEnd ) )
				{
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::SeekByRngSec( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, PBOOL8 A_PtrIsEd )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		BOOL8	Tv_IsEnd;
		BOOL8	Tv_IsDirBk;
		__u32	Tv_CntExit;//121114
	// code --------------------
		Tv_IsDirBk	=	m_IsDirBack;		// backup direction
		DirectionSet( FALSE );
		Tv_CntExit = 40;//121114
		Tv_Result	=	SeekByRngMin( A_TimeSt, A_TimeEd );
		if ( Tv_Result )
		{
			Tv_Result	=	FALSE;
			//go previous
			Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)GetNextFrame( &Tv_IsEnd );
			while ( !Tv_IsEnd )
			{
				if( 0 == Tv_CntExit )
				{
					//break;
				}//121114
				if ( NULL != Tv_PtrAvHd )
				{
					if ( A_TimeSt <= Tv_PtrAvHd->TimeSec )
					{
						//finded
						Tv_Result			=	TRUE;
						m_IsFrmFirst	=	TRUE;		//start position						
						break;
					}
				}

				//next
				Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)GetNextFrame( &Tv_IsEnd );
				Tv_CntExit--;//121114
			}
		}
		DirectionSet( Tv_IsDirBk );	//restore direction
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::CntnByRngSec( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, PBOOL8 A_PtrIsEd )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDvrAvHd	Tv_PtrAvHd;
		BOOL8	Tv_IsEnd;
		BOOL8	Tv_IsDirBk;
		__u32	Tv_CntExit;//121114
	// code --------------------
		Tv_IsDirBk	=	m_IsDirBack;		// backup direction
		DirectionSet( FALSE );
		Tv_Result	=	FALSE;
		Tv_CntExit = 40;//121114
		//go previous
		Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)GetNextFrame( &Tv_IsEnd );
		while ( !Tv_IsEnd )
		{
			if( 0 == Tv_CntExit )
			{
				//break;
			}//121114
			if ( NULL != Tv_PtrAvHd )
			{
				if ( A_TimeSt <= Tv_PtrAvHd->TimeSec )
				{
					//finded
					Tv_Result			=	TRUE;
					m_IsFrmFirst	=	TRUE;		//start position
					break;
				}
			}
			else
			{
				*A_PtrIsEd	=	Tv_IsEnd;
				//bad time
				break;
			}

			//next
			Tv_PtrAvHd	=	(Ptr_GrDvrAvHd)GetNextFrame( &Tv_IsEnd );
			Tv_CntExit--;//121114
		}
		DirectionSet( Tv_IsDirBk );	//restore direction
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoSeek::DirectionSet( BOOL8 A_IsBack )
{
		if ( m_IsDirBack != A_IsBack )
		{
			m_IsDirBack	=	A_IsBack;
			if ( A_IsBack )
			{
				m_VdoIonlyMap	=	0xFFFF;
			}
			else
			{
				m_VdoIonlyMap	=	m_ReqIonlyMap;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::IsDirBack( void )
{
		return	m_IsDirBack;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoSeek::VdoAbleMapSet( __u16 A_Map )
{
	// local -------------------
		__u16	Tv_Mask;
		__u8	Tv_Ch;
		__u8	Tv_AbleCnt;
	// code --------------------
		m_VdoAbleMap	=	A_Map;
		m_VdoPableMap	=	0;
		//update able channel count
		Tv_Mask				=	1;
		Tv_AbleCnt		=	0;
		for ( Tv_Ch=0;Tv_Ch < 16;Tv_Ch++ )
		{
			if ( 0 != (Tv_Mask & m_VdoAbleMap) )
			{
				Tv_AbleCnt ++;
			}
			Tv_Mask	=	Tv_Mask << 1;
		}
		m_AbleChCnt	=	Tv_AbleCnt;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoSeek::VdoIonlyMapSet( __u16 A_Map )
{
		m_ReqIonlyMap	=	A_Map;
		if ( !m_IsDirBack )
		{
			m_VdoIonlyMap	=	A_Map;
			m_VdoPableMap	=	m_VdoPableMap & (~A_Map);
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoSeek::AdoAbleMapSet( __u16 A_Map )
{
		m_AdoAbleMap	=	A_Map;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoSeek::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
	// code --------------------
		return	FALSE;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFsoSeek::BufIdxGet( void )
{
		return	m_BufIdx;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoSeek::VdoChSyncReset( __u8 A_Ch )
{
		m_VdoPableMap	=	m_VdoPableMap &	(~(1 << (__u32)A_Ch));
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoSeek::VopSyncReset( void )
{
		m_VdoPableMap	=	0;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoSeek::MinJumpSet( __u8 A_Val )
{
		m_MinJmpStep	=	A_Val;
		m_MinJmpProc	=	0;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoSeek::MinJumpGet( void )
{
		return	m_MinJmpStep;
}
//--------------------------------------------------------------------



