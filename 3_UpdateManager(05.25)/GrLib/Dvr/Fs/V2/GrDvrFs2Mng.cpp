/*
 GAUSE DVR file system version 2 manager

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<GrDumyTool.h>
#include	<GrTimeTool.h>
#include	<GrStrTool.h>

#include	<Dvr/Fs/V2/GrDvrFs2Mng.h>

#include	<stdlib.h>

//====================================================================
//local const

#define E_GrDvrFs2MngTimerPriod			300000		/* 5 minute */

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs2Mng::Cls_GrDvrFs2Mng( Cls_GrTaskSvr* A_TaskSvr, __u64 A_DvrId ):
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		//init
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		m_DvrId			=	A_DvrId;

		m_IsInited	=	FALSE;

		m_GrupNrmMap	=	0;
		m_GrupMirMap	=	0;
		m_IsOvwtNrm		=	FALSE;
		m_IsOvwtMir		=	FALSE;

		m_AtdSec			=	0;
		m_AtdTime			=	0;

		GrDumyZeroMem( m_DiskTbl, sizeof(m_DiskTbl) );
		GrDumyZeroMem( m_GrupTbl, sizeof(m_GrupTbl) );
		GrDumyZeroMem( m_TempTbl, sizeof(m_TempTbl) );

		//register task
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrDvrFs2MngTimerPriod, &m_HndlTimer );


}
//--------------------------------------------------------------------
Cls_GrDvrFs2Mng::~Cls_GrDvrFs2Mng()
{
		//unregister timer
		m_TaskSvr->UnregistMsgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );


		GrTaskSvrMsgCmdFree( m_MsgAloc );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check input message
		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			Tv_Result	=	TRUE;
			switch ( ((Ptr_GrDvrFs2MngMsgBase)A_PtrMsg)->Cmd )
			{
				case E_GrDvrFs2MngMsgInit:
					ImmInit();
					break;
				case E_GrDvrFs2MngMsgSetup:
					ImmSetup( ((Ptr_GrDvrFs2MngMsgSetup)A_PtrMsg)->NrmMap, ((Ptr_GrDvrFs2MngMsgSetup)A_PtrMsg)->MirMap,
						((Ptr_GrDvrFs2MngMsgSetup)A_PtrMsg)->IsNrmOvwt, ((Ptr_GrDvrFs2MngMsgSetup)A_PtrMsg)->IsMirOvwt );
					break;
				case E_GrDvrFs2MngMsgFormat:
					*((Ptr_GrDvrFs2MngMsgFormat)A_PtrMsg)->PtrRslt	=	
						ImmFormat( ((Ptr_GrDvrFs2MngMsgFormat)A_PtrMsg)->DevIdx, ((Ptr_GrDvrFs2MngMsgFormat)A_PtrMsg)->IsWithLog );
					break;
				case E_GrDvrFs2MngMsgDiskStatGet:
					*((Ptr_GrDvrFs2MngMsgDiskStatGet)A_PtrMsg)->PtrRslt	=	
						ImmDiskStatGet( ((Ptr_GrDvrFs2MngMsgDiskStatGet)A_PtrMsg)->DevIdx );
					break;
				case E_GrDvrFs2MngMsgDiskRecSizeGet:
					*((Ptr_GrDvrFs2MngMsgDiskRecSizeGet)A_PtrMsg)->PtrRslt	=	
						ImmDiskRecSizeGet( ((Ptr_GrDvrFs2MngMsgDiskRecSizeGet)A_PtrMsg)->DevIdx, ((Ptr_GrDvrFs2MngMsgDiskRecSizeGet)A_PtrMsg)->PtrRtTotal,
						((Ptr_GrDvrFs2MngMsgDiskRecSizeGet)A_PtrMsg)->PtrRtRec );
					break;
				case E_GrDvrFs2MngMsgGroupStatGet:
					*((Ptr_GrDvrFs2MngMsgGroupStatGet)A_PtrMsg)->PtrRslt	=
						ImmGroupStatGet( ((Ptr_GrDvrFs2MngMsgGroupStatGet)A_PtrMsg)->GroupType );
					break;
				case E_GrDvrFs2MngMsgGroupRecSizeGet:
					*((Ptr_GrDvrFs2MngMsgGroupRecSizeGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupRecSizeGet( ((Ptr_GrDvrFs2MngMsgGroupRecSizeGet)A_PtrMsg)->GroupType, ((Ptr_GrDvrFs2MngMsgGroupRecSizeGet)A_PtrMsg)->PtrRtTotal,
						((Ptr_GrDvrFs2MngMsgGroupRecSizeGet)A_PtrMsg)->PtrRtRec );
					break;
				case E_GrDvrFs2MngMsgGroupDayMapGet:
					*((Ptr_GrDvrFs2MngMsgGroupDayMapGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupDayMapGet( ((Ptr_GrDvrFs2MngMsgGroupDayMapGet)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgGroupDayMapGet)A_PtrMsg)->TimeSt,
						((Ptr_GrDvrFs2MngMsgGroupDayMapGet)A_PtrMsg)->DayCnt );
					break;
				case E_GrDvrFs2MngMsgGroupRecThumbGet:
					*((Ptr_GrDvrFs2MngMsgGroupRecThumbGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupRecMinMapGet( ((Ptr_GrDvrFs2MngMsgGroupRecThumbGet)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgGroupRecThumbGet)A_PtrMsg)->TimeSt,
						((Ptr_GrDvrFs2MngMsgGroupRecThumbGet)A_PtrMsg)->ItemCnt, ((Ptr_GrDvrFs2MngMsgGroupRecThumbGet)A_PtrMsg)->PtrBuf );
					break;
				case E_GrDvrFs2MngMsgReadForward:
					ImmReadForward( ((Ptr_GrDvrFs2MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgReadPara)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs2MngMsgReadBackward:
					ImmReadBackward( ((Ptr_GrDvrFs2MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgReadPara)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs2MngMsgReadBegin:
					ImmReadBegin( ((Ptr_GrDvrFs2MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgReadPara)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs2MngMsgReadEnd:
					ImmReadEnd( ((Ptr_GrDvrFs2MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgReadPara)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs2MngMsgReadByHndlFw:
					ImmReadByHndlFw( ((Ptr_GrDvrFs2MngMsgReadHndlPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgReadHndlPara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrDvrFs2MngMsgReadHndlPara)A_PtrMsg)->Hndl );
					break;
				case E_GrDvrFs2MngMsgReadByHndlBw:
					ImmReadByHndlBw( ((Ptr_GrDvrFs2MngMsgReadHndlPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgReadHndlPara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrDvrFs2MngMsgReadHndlPara)A_PtrMsg)->Hndl );
					break;
				case E_GrDvrFs2MngMsgReadByMinFw:
					ImmReadByMinFw( ((Ptr_GrDvrFs2MngMsgReadTimePara)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgReadTimePara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrDvrFs2MngMsgReadTimePara)A_PtrMsg)->Time );
					break;
				case E_GrDvrFs2MngMsgReadByMinBw:
					ImmReadByMinBw( ((Ptr_GrDvrFs2MngMsgReadTimePara)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgReadTimePara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrDvrFs2MngMsgReadTimePara)A_PtrMsg)->Time );
					break;
				case E_GrDvrFs2MngMsgVssReset:
					ImmVssReset();
					break;
				case E_GrDvrFs2MngMsgVssWrite:
					*((Ptr_GrDvrFs2MngMsgVssRwPara)A_PtrMsg)->PtrRslt	=	ImmVssWrite( ((Ptr_GrDvrFs2MngMsgVssRwPara)A_PtrMsg)->Pos, 
						((Ptr_GrDvrFs2MngMsgVssRwPara)A_PtrMsg)->Ptr, ((Ptr_GrDvrFs2MngMsgVssRwPara)A_PtrMsg)->Size );
					break;
				case E_GrDvrFs2MngMsgVssRead:
					*((Ptr_GrDvrFs2MngMsgVssRwPara)A_PtrMsg)->PtrRslt	=	ImmVssRead( ((Ptr_GrDvrFs2MngMsgVssRwPara)A_PtrMsg)->Pos, 
						((Ptr_GrDvrFs2MngMsgVssRwPara)A_PtrMsg)->Ptr, ((Ptr_GrDvrFs2MngMsgVssRwPara)A_PtrMsg)->Size );
					break;
				case E_GrDvrFs2MngMsgUcaWrite:
					*((Ptr_GrDvrFs2MngMsgUcaRwPara)A_PtrMsg)->PtrRslt	=	ImmUcaWrite( ((Ptr_GrDvrFs2MngMsgUcaRwPara)A_PtrMsg)->Pos, 
						((Ptr_GrDvrFs2MngMsgUcaRwPara)A_PtrMsg)->Ptr, ((Ptr_GrDvrFs2MngMsgUcaRwPara)A_PtrMsg)->Size );
					break;
				case E_GrDvrFs2MngMsgUcaRead:
					*((Ptr_GrDvrFs2MngMsgUcaRwPara)A_PtrMsg)->PtrRslt	=	ImmUcaRead( ((Ptr_GrDvrFs2MngMsgUcaRwPara)A_PtrMsg)->Pos, 
						((Ptr_GrDvrFs2MngMsgUcaRwPara)A_PtrMsg)->Ptr, ((Ptr_GrDvrFs2MngMsgUcaRwPara)A_PtrMsg)->Size );
					break;
				case E_GrDvrFs2MngMsgRecTimeRngGet:
					*((Ptr_GrDvrFs2MngMsgRecTimeRngGetPara)A_PtrMsg)->PtrRslt	=	ImmGroupRecTimeRngGet( ((Ptr_GrDvrFs2MngMsgRecTimeRngGetPara)A_PtrMsg)->Group,
						((Ptr_GrDvrFs2MngMsgRecTimeRngGetPara)A_PtrMsg)->PtrRtSt, ((Ptr_GrDvrFs2MngMsgRecTimeRngGetPara)A_PtrMsg)->PtrRtEd );
					break;
				case E_GrDvrFs2MngMsgTimeRecSize:
					*((Ptr_GrDvrFs2MngMsgTimeRecSizePara)A_PtrMsg)->PtrRslt	=	ImmGroupTimeRecSize( ((Ptr_GrDvrFs2MngMsgTimeRecSizePara)A_PtrMsg)->Group,
						((Ptr_GrDvrFs2MngMsgTimeRecSizePara)A_PtrMsg)->ChMap, ((Ptr_GrDvrFs2MngMsgTimeRecSizePara)A_PtrMsg)->TimeSt,
						((Ptr_GrDvrFs2MngMsgTimeRecSizePara)A_PtrMsg)->TimeEd );
					break;
				case E_GrDvrFs2MngMsgLogAdd:
					ImmEvtLogAdd( &((Ptr_GrDvrFs2MngMsgLogAddPara)A_PtrMsg)->Log );
					break;
				case E_GrDvrFs2MngMsgLogFindStart:
					ImmEvtLogFindStart( ((Ptr_GrDvrFs2MngMsgLogFindPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgLogFindPara)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs2MngMsgLogFindNext:
					ImmEvtLogFindNext( ((Ptr_GrDvrFs2MngMsgLogFindPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgLogFindPara)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs2MngMsgLogFindPrev:
					ImmEvtLogFindPrev( ((Ptr_GrDvrFs2MngMsgLogFindPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs2MngMsgLogFindPara)A_PtrMsg)->PtrCtrl );
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
			
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::RtlTimer( __u32 A_Hndl )
{	
		// temperature update
		LcTemperatureUpdt();
		// auto delete time update
		LcAtdTimeUpdt();
}
//--------------------------------------------------------------------
Cls_GrDvrFs2Disk*	Cls_GrDvrFs2Mng::RtlDiskObjGet( __u8 A_Idx )
{
	// local -------------------
		Cls_GrDvrFs2Disk*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( E_GrDvrFs2MaxDriveCnt > A_Idx )
		{
			Tv_Result	=	m_DiskTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Mng::AdoSyncCntGet( void )
{
		return	0;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs2Mng::AdoDataUnitSizeGet( void )
{
		return	0;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::VideoAdd( Ptr_GrDvrAvHd A_PtrPkt )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// try normal
		if ( NULL != m_GrupTbl[E_GrDvrRecDevNoraml] )
		{
			if ( m_GrupTbl[E_GrDvrRecDevNoraml]->VideoAdd( A_PtrPkt ) )
			{
				Tv_Result	=	TRUE;
			}
		}
		// try mirror
		if ( NULL != m_GrupTbl[E_GrDvrRecDevMirror] )
		{
			if ( m_GrupTbl[E_GrDvrRecDevMirror]->VideoAdd( A_PtrPkt ) )
			{
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::AudioAdd( Ptr_GrDvrAvHd A_PtrPkt )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// try normal
		if ( NULL != m_GrupTbl[E_GrDvrRecDevNoraml] )
		{
			if ( m_GrupTbl[E_GrDvrRecDevNoraml]->AudioAdd( A_PtrPkt ) )
			{
				Tv_Result	=	TRUE;
			}
		}
		// try mirror
		if ( NULL != m_GrupTbl[E_GrDvrRecDevMirror] )
		{
			if ( m_GrupTbl[E_GrDvrRecDevMirror]->AudioAdd( A_PtrPkt ) )
			{
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::PacketAdd( Ptr_GrDvrAvHd A_PtrPkt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Group;
	// code --------------------
		Tv_Result	=	FALSE;

		for ( Tv_Group = E_GrDvrRecDevNoraml;Tv_Group <=  E_GrDvrRecDevMirror;Tv_Group++ )
		{
			if ( NULL != m_GrupTbl[Tv_Group] )
			{
				if ( (E_GrDvrAvTypVdoP >= A_PtrPkt->Type) || (E_GrDvrAvTypVdoB == A_PtrPkt->Type) )
				{
					if ( m_GrupTbl[Tv_Group]->VideoAdd( A_PtrPkt ) )
					{
						Tv_Result	=	TRUE;
					}
				}
				else if ( E_GrDvrAvTypAdo == A_PtrPkt->Type )
				{
					if ( m_GrupTbl[Tv_Group]->AudioAdd( A_PtrPkt ) )
					{
						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir )
{
	// local -------------------
		St_GrDvrFs2MngMsgSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs2MngMsgSetup;
		Tv_Msg.NrmMap						=	A_NormalMap;
		Tv_Msg.IsNrmOvwt				=	A_IsOvwtNrm;
		Tv_Msg.MirMap						=	A_MirrorMap;
		Tv_Msg.IsMirOvwt				=	A_IsOvwtMir;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::LcGroupBuild( void )
{
	// local -------------------
		__u8	Tv_GrIdx;
	// code --------------------
		// release group
		for ( Tv_GrIdx=0;Tv_GrIdx < E_GrDvrFs2MngGroupCnt;Tv_GrIdx++ )
		{
			if ( NULL != m_GrupTbl[Tv_GrIdx] )
			{
				delete	m_GrupTbl[Tv_GrIdx];
				m_GrupTbl[Tv_GrIdx]	=	NULL;
			}
		}

		// create normal group
		m_GrupTbl[E_GrDvrRecDevNoraml]	=	(Cls_GrDvrFs2Grp*) new Cls_GrDvrFs2Grp( this, m_GrupNrmMap, m_IsOvwtNrm );

		// create mirror group
		m_GrupTbl[E_GrDvrRecDevMirror]	=	(Cls_GrDvrFs2Grp*) new Cls_GrDvrFs2Grp( this, m_GrupMirMap, m_IsOvwtMir );
		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir )
{
	// local -------------------
	// code --------------------
		// update group info
		m_GrupNrmMap	=	A_NormalMap;
		m_GrupMirMap	=	A_MirrorMap & (~A_NormalMap);
		m_IsOvwtNrm		=	A_IsOvwtNrm;
		m_IsOvwtMir		=	A_IsOvwtMir;
		LcGroupBuild();

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeInit( void )
{
	// local -------------------
		St_GrDvrFs2MngMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Cmd			=	E_GrDvrFs2MngMsgInit;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeVssReset( void )
{
	// local -------------------
		St_GrDvrFs2MngMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Cmd			=	E_GrDvrFs2MngMsgVssReset;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmInit( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		// build disk list
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs2MaxDriveCnt;Tv_WkIdx++ )
		{
			m_DiskTbl[Tv_WkIdx]	=	(Cls_GrDvrFs2Disk*) new Cls_GrDvrFs2Disk( Tv_WkIdx, m_TmpBuf, m_DvrId );
			// check not exist disk
			if ( E_GrDvrRecStatNotexist == m_DiskTbl[Tv_WkIdx]->DiskStatGet() )
			{
				delete	m_DiskTbl[Tv_WkIdx];
				m_DiskTbl[Tv_WkIdx]	=	NULL;
			}
		}
		// temperature update
		LcTemperatureUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::SafeFormat( __u8 A_DevIdx, BOOL8 A_IsWithLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs2MngMsgFormat	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs2MngMsgFormat;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.IsWithLog				=	A_IsWithLog;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::ImmFormat( __u8 A_DevIdx, BOOL8 A_IsWithLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrTaskMsgDiskFormat	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		// check valid device index
		if ( E_GrDvrFs2MaxDriveCnt > A_DevIdx )
		{
			// check exist disk
			if ( NULL != m_DiskTbl[A_DevIdx] )
			{
				// do format
				Tv_Result	=	m_DiskTbl[A_DevIdx]->Format( A_IsWithLog );
				// regroup
				LcGroupBuild();		// regroup
			}
		}
		
		// notify format completed
		Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgDiskFormat;
		Tv_Msg.DiskIdx		=	A_DevIdx;
		Tv_Msg.IsSuccess	=	Tv_Result;
		GrTaskSvrMsgBroadCast( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Mng::SafeDiskStatusGet( __u8 A_DevIdx )
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs2MngMsgDiskStatGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs2MngMsgDiskStatGet;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Mng::ImmDiskStatGet( __u8 A_DevIdx )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		// check valid device index
		if ( E_GrDvrFs2MaxDriveCnt > A_DevIdx )
		{
			// check exist disk
			if ( NULL != m_DiskTbl[A_DevIdx] )
			{
				Tv_Result	=	m_DiskTbl[A_DevIdx]->DiskStatGet();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::SafeDiskRecSizeGet( __u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs2MngMsgDiskRecSizeGet	Tv_Msg;
	// code --------------------
		Tv_Result			=	FALSE;
		*A_PtrRtTotal	=	0;
		*A_PtrRtRec		=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs2MngMsgDiskRecSizeGet;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.PtrRtTotal				=	A_PtrRtTotal;
		Tv_Msg.PtrRtRec					=	A_PtrRtRec;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::ImmDiskRecSizeGet( __u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrFs2MaxDriveCnt > A_DevIdx )
		{
			// check exist disk
			if ( NULL != m_DiskTbl[A_DevIdx] )
			{
				Tv_Result	=	m_DiskTbl[A_DevIdx]->RecSizeGet( A_PtrRtTotal, A_PtrRtRec );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Mng::SafeGroupStatusGet( __u8 A_GroupType )
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs2MngMsgGroupStatGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs2MngMsgGroupStatGet;
		Tv_Msg.GroupType				=	A_GroupType;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::SafeGroupRecSizeGet( __u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs2MngMsgGroupRecSizeGet	Tv_Msg;
	// code --------------------
		Tv_Result			=	FALSE;
		*A_PtrRtTotal	=	0;
		*A_PtrRtRec		=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs2MngMsgGroupRecSizeGet;
		Tv_Msg.GroupType				=	A_GroupType;
		Tv_Msg.PtrRtTotal				=	A_PtrRtTotal;
		Tv_Msg.PtrRtRec					=	A_PtrRtRec;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Mng::ImmGroupStatGet( __u8 A_GroupType )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		if ( E_GrDvrFs2MngGroupCnt > A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				Tv_Result	=	m_GrupTbl[A_GroupType]->GroupStatusGet();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::ImmGroupRecSizeGet( __u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrFs2MngGroupCnt > A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				m_GrupTbl[A_GroupType]->RecSizeGet( A_PtrRtTotal, A_PtrRtRec );
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs2Mng::SafeGroupDayMapGet( __u8 A_GroupType, Def_GrTime A_Time, __u8 A_DayCnt )
{
	// local -------------------
		__u32	Tv_Result;
		St_GrDvrFs2MngMsgGroupDayMapGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs2MngMsgGroupDayMapGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.TimeSt						=	A_Time;
		Tv_Msg.DayCnt						=	A_DayCnt;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs2Mng::ImmGroupDayMapGet( __u8 A_GroupType, Def_GrTime A_Time, __u8 A_DayCnt )
{
	// local -------------------
		__u32	Tv_Result;
		Def_GrTime	Tv_TimeEd;
		Def_GrTime	Tv_TimeSt;
		__u8	Tv_DayCnt;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrDvrFs2MngGroupCnt > A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				Tv_DayCnt	=	A_DayCnt;
				if ( 0 != Tv_DayCnt )
				{
					if ( 31 < Tv_DayCnt )
					{
						Tv_DayCnt	=	31;
					}
					Tv_TimeSt	=	A_Time - (A_Time % E_GrTimeSecOfDay);
					Tv_TimeEd	=	Tv_TimeSt + ( (__u32)Tv_DayCnt * E_GrTimeSecOfDay ) - 1;
					Tv_Result	=	m_GrupTbl[A_GroupType]->DayMapGet( Tv_TimeSt, Tv_TimeEd );
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::SafeGroupRecMinMapGet( __u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs2MngMsgGroupRecThumbGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs2MngMsgGroupRecThumbGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.TimeSt						=	A_TimeSt;
		Tv_Msg.ItemCnt					=	A_ItemCnt;
		Tv_Msg.PtrBuf						=	A_PtrBuf;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::ImmGroupRecMinMapGet( __u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrFs2MngGroupCnt > A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				Tv_Result	=	m_GrupTbl[A_GroupType]->RecMinMapGet( A_TimeSt, A_PtrBuf, A_ItemCnt );
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmReadForward( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl )
{
	// local -------------------
	// code --------------------
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrupTbl[A_DevType] )
			{
				m_GrupTbl[A_DevType]->ReadForward( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmReadBackward( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl )
{
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrupTbl[A_DevType] )
			{
				m_GrupTbl[A_DevType]->ReadBackward( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmReadBegin( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl )
{
	// local -------------------
	// code --------------------
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrupTbl[A_DevType] )
			{
				m_GrupTbl[A_DevType]->ReadBegin( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmReadEnd( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl )
{
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrupTbl[A_DevType] )
			{
				m_GrupTbl[A_DevType]->ReadEnd( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmReadByHndlFw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrupTbl[A_DevType] )
			{
				m_GrupTbl[A_DevType]->ReadByHndlFw( A_PtrCtrl, A_Hndl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmReadByHndlBw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
		//check device type
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrupTbl[A_DevType] )
			{
				m_GrupTbl[A_DevType]->ReadByHndlBw( A_PtrCtrl, A_Hndl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmReadByMinFw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
	// code --------------------
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			m_GrupTbl[A_DevType]->ReadByTimeFw( A_PtrCtrl, A_Time );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmReadByMinBw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
	// code --------------------
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			m_GrupTbl[A_DevType]->ReadByTimeBw( A_PtrCtrl, A_Time );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeReadForward( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs2MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgReadForward;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeReadBackward( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs2MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgReadBackward;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeReadBegin( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs2MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgReadBegin;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeReadEnd( __u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs2MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgReadEnd;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeReadByHndlFw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
	// local -------------------
		St_GrDvrFs2MngMsgReadHndlPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgReadByHndlFw;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.Hndl						=	A_Hndl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeReadByHndlBw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
	// local -------------------
		St_GrDvrFs2MngMsgReadHndlPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgReadByHndlBw;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.Hndl						=	A_Hndl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeReadByMinFw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
		St_GrDvrFs2MngMsgReadTimePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgReadByMinFw;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.Time						=	A_Time;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeReadByMinBw(__u8 A_DevType, Ptr_GrDvrFs2SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
		St_GrDvrFs2MngMsgReadTimePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgReadByMinBw;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.Time						=	A_Time;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::ImmVssReset( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_DiskTbl[0] )
		{
			Tv_Result	=	m_DiskTbl[0]->VssReset();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::ImmVssWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_DiskTbl[0] )
		{
			Tv_Result	=	m_DiskTbl[0]->VssWrite( A_Pos, A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::ImmVssRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_DiskTbl[0] )
		{
			Tv_Result	=	m_DiskTbl[0]->VssRead( A_Pos, A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::SafeVssWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs2MngMsgVssRwPara	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs2MngMsgVssWrite;
		Tv_Msg.Pos							=	A_Pos;
		Tv_Msg.Ptr							=	A_PtrBuf;
		Tv_Msg.Size							=	A_Size;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::SafeVssRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs2MngMsgVssRwPara	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs2MngMsgVssRead;
		Tv_Msg.Pos							=	A_Pos;
		Tv_Msg.Ptr							=	A_PtrBuf;
		Tv_Msg.Size							=	A_Size;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::ImmUcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_DiskTbl[0] )
		{
			Tv_Result	=	m_DiskTbl[0]->UcaWrite( A_Pos, A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::ImmUcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_DiskTbl[0] )
		{
			Tv_Result	=	m_DiskTbl[0]->UcaRead( A_Pos, A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::SafeUcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs2MngMsgUcaRwPara	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs2MngMsgUcaWrite;
		Tv_Msg.Pos							=	A_Pos;
		Tv_Msg.Ptr							=	A_PtrBuf;
		Tv_Msg.Size							=	A_Size;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::SafeUcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs2MngMsgUcaRwPara	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs2MngMsgUcaRead;
		Tv_Msg.Pos							=	A_Pos;
		Tv_Msg.Ptr							=	A_PtrBuf;
		Tv_Msg.Size							=	A_Size;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::LcTemperatureUpdt( void )
{
	// local -------------------
		__u8	Tv_DskIdx;
	// code --------------------
		for ( Tv_DskIdx=0;Tv_DskIdx < E_GrDvrFs2MaxDriveCnt;Tv_DskIdx++ )
		{
			if ( NULL != m_DiskTbl[Tv_DskIdx] )
			{
				m_TempTbl[Tv_DskIdx]	=	m_DiskTbl[Tv_DskIdx]->TemperatureGet();
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Mng::TemperatureGet( __u8 A_DevIdx )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrDvrFs2MaxDriveCnt > A_DevIdx )
		{
			Tv_Result	=	m_TempTbl[A_DevIdx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::ImmGroupRecTimeRngGet( __u8 A_GroupType, Def_GrTime* A_PtrRtTimeBegin, Def_GrTime* A_PtrRtTimeEnd )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrFs2MngGroupCnt > A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				Tv_Result	=	m_GrupTbl[A_GroupType]->RecTImeRngGet( A_PtrRtTimeBegin, A_PtrRtTimeEnd );
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Mng::SafeGroupRecTimeRngGet( __u8 A_GroupType, Def_GrTime* A_PtrRtTimeBegin, Def_GrTime* A_PtrRtTimeEnd )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs2MngMsgRecTimeRngGetPara	Tv_Msg;
	// code --------------------
		Tv_Result		=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgRecTimeRngGet;
		Tv_Msg.Group					=	A_GroupType;
		Tv_Msg.PtrRtSt				=	A_PtrRtTimeBegin;
		Tv_Msg.PtrRtEd				=	A_PtrRtTimeEnd;
		Tv_Msg.PtrRslt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs2Mng::ImmGroupTimeRecSize( __u8 A_GroupType, __u16 A_ChMask, Def_GrTime A_TimeSt, 
	Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrDvrFs2MngGroupCnt > A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				Tv_Result	=	m_GrupTbl[A_GroupType]->TimeRecSizeCalc( A_ChMask, A_TimeSt, A_TimeEd );
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs2Mng::SafeGroupTimeRecSize( __u8 A_GroupType, __u16 A_ChMask, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFs2MngMsgTimeRecSizePara	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgTimeRecSize;
		Tv_Msg.Group					=	A_GroupType;
		Tv_Msg.ChMap					=	A_ChMask;
		Tv_Msg.TimeSt					=	A_TimeSt;
		Tv_Msg.TimeEd					=	A_TimeEd;
		Tv_Msg.PtrRslt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmEvtLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		__u8	Tv_DevType;
	// code --------------------
		//check device type
		for ( Tv_DevType=0;Tv_DevType < E_GrDvrFs2MngGroupCnt;Tv_DevType++ )
		{
			if ( NULL != m_GrupTbl[Tv_DevType] )
			{
				m_GrupTbl[Tv_DevType]->EvtLogAdd( A_PtrItem );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmEvtLogFindStart( __u8 A_DevType, Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
	// code --------------------
		//check device type
		if ( E_GrDvrFs2MngGroupCnt > A_DevType )
		{
			if ( NULL != m_GrupTbl[A_DevType] )
			{
				m_GrupTbl[A_DevType]->EvtLogFindStart( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmEvtLogFindNext( __u8 A_DevType, Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
	// code --------------------
		//check device type
		if ( E_GrDvrFs2MngGroupCnt > A_DevType )
		{
			if ( NULL != m_GrupTbl[A_DevType] )
			{
				m_GrupTbl[A_DevType]->EvtLogFindNext( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmEvtLogFindPrev( __u8 A_DevType, Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
	// code --------------------
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrupTbl[A_DevType] )
			{
				m_GrupTbl[A_DevType]->EvtLogFindPrev( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeEvtLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		St_GrDvrFs2MngMsgLogAddPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgLogAdd;
		GrDumyCopyMem( &Tv_Msg.Log, A_PtrItem, sizeof(St_GrDvrLogItm) );
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeEvtLogFindStart( __u8 A_DevType, Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs2MngMsgLogFindPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgLogFindStart;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeEvtLogFindNext( __u8 A_DevType, Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs2MngMsgLogFindPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgLogFindNext;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeEvtLogFindPrev( __u8 A_DevType, Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs2MngMsgLogFindPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgLogFindPrev;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::ImmAutoDeleteSet( __u32 A_Sec )
{
	// local -------------------
	// code --------------------
		m_AtdSec	=	A_Sec;
		// update auto delete
		LcAtdTimeUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::LcAtdTimeUpdt( void )
{
	// local -------------------
		__u32	Tv_Mili;
	// code --------------------
		if ( 0 == m_AtdSec )
		{
			m_AtdTime	=	0;
		}
		else
		{
			m_AtdTime	=	GrTimeSysTimeGet( &Tv_Mili ) - m_AtdSec;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Mng::SafeAutoDeleteSet( __u32 A_Sec )
{
	// local -------------------
		St_GrDvrFs2MngMsgAutoDeletePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs2MngMsgAutoDelete;
		Tv_Msg.Sec						=	A_Sec;
		
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
