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

#include	<Dvr/Fs/V3/GrDvrFs3Mng.h>

#include	<stdlib.h>

//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs3Mng::Cls_GrDvrFs3Mng( Cls_GrTaskSvr* A_TaskSvr, __u64 A_DvrId, __u32 A_AdoUnitSize, __u8 A_AdoSyncCnt ):
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		//init
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		m_DvrId			=	A_DvrId;

		m_IsInited	=	FALSE;

		m_AdoUnitSize	=	A_AdoUnitSize;
		m_AdoSyncCnt	=	A_AdoSyncCnt;

		m_GrupNrmMap	=	0;
		m_GrupMirMap	=	0;
		m_IsOvwtNrm		=	FALSE;
		m_IsOvwtMir		=	FALSE;

		m_TempTimer		=	0;

		m_AtDelTime		=	0;
		m_AtDelBaseTm	=	0;

		m_ScanSizeMode	=	E_GrDvrFs3MngScanSizeModeNone;

		GrDumyZeroMem( m_DiskTbl, sizeof(m_DiskTbl) );
		GrDumyZeroMem( m_GrupTbl, sizeof(m_GrupTbl) );
		GrDumyZeroMem( m_TempTbl, sizeof(m_TempTbl) );

		//register task
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrDvrFs3MngTimerPeriod, &m_HndlTimer );
		m_TaskSvr->RegistBkgClient( this );

}
//--------------------------------------------------------------------
Cls_GrDvrFs3Mng::~Cls_GrDvrFs3Mng()
{
		//unregister timer
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistMsgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );


		GrTaskSvrMsgCmdFree( m_MsgAloc );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check input message
		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			Tv_Result	=	TRUE;
			switch ( ((Ptr_GrDvrFs3MngMsgBase)A_PtrMsg)->Cmd )
			{
				case E_GrDvrFs3MngMsgInit:
					ImmInit();
					break;
				case E_GrDvrFs3MngMsgSetup:
					ImmSetup( ((Ptr_GrDvrFs3MngMsgSetup)A_PtrMsg)->NrmMap, ((Ptr_GrDvrFs3MngMsgSetup)A_PtrMsg)->MirMap,
						((Ptr_GrDvrFs3MngMsgSetup)A_PtrMsg)->IsNrmOvwt, ((Ptr_GrDvrFs3MngMsgSetup)A_PtrMsg)->IsMirOvwt,
						((Ptr_GrDvrFs3MngMsgSetup)A_PtrMsg)->IsAutoFix );
					break;
				case E_GrDvrFs3MngMsgFormat:
					*((Ptr_GrDvrFs3MngMsgFormat)A_PtrMsg)->PtrRslt	=	
						ImmFormat( ((Ptr_GrDvrFs3MngMsgFormat)A_PtrMsg)->DevIdx, ((Ptr_GrDvrFs3MngMsgFormat)A_PtrMsg)->IsWithLog );
					break;
				case E_GrDvrFs3MngMsgDiskStatGet:
					*((Ptr_GrDvrFs3MngMsgDiskStatGet)A_PtrMsg)->PtrRslt	=	
						ImmDiskStatGet( ((Ptr_GrDvrFs3MngMsgDiskStatGet)A_PtrMsg)->DevIdx );
					break;
				case E_GrDvrFs3MngMsgDiskRecSizeGet:
					*((Ptr_GrDvrFs3MngMsgDiskRecSizeGet)A_PtrMsg)->PtrRslt	=	
						ImmDiskRecSizeGet( ((Ptr_GrDvrFs3MngMsgDiskRecSizeGet)A_PtrMsg)->DevIdx, ((Ptr_GrDvrFs3MngMsgDiskRecSizeGet)A_PtrMsg)->PtrRtTotal,
						((Ptr_GrDvrFs3MngMsgDiskRecSizeGet)A_PtrMsg)->PtrRtRec );
					break;
				case E_GrDvrFs3MngMsgGroupStatGet:
					*((Ptr_GrDvrFs3MngMsgGroupStatGet)A_PtrMsg)->PtrRslt	=
						ImmGroupStatGet( ((Ptr_GrDvrFs3MngMsgGroupStatGet)A_PtrMsg)->GroupType );
					break;
				case E_GrDvrFs3MngMsgGroupRecSizeGet:
					*((Ptr_GrDvrFs3MngMsgGroupRecSizeGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupRecSizeGet( ((Ptr_GrDvrFs3MngMsgGroupRecSizeGet)A_PtrMsg)->GroupType, ((Ptr_GrDvrFs3MngMsgGroupRecSizeGet)A_PtrMsg)->PtrRtTotal,
						((Ptr_GrDvrFs3MngMsgGroupRecSizeGet)A_PtrMsg)->PtrRtRec );
					break;
				case E_GrDvrFs3MngMsgGroupDayMapGet:
					*((Ptr_GrDvrFs3MngMsgGroupDayMapGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupDayMapGet( ((Ptr_GrDvrFs3MngMsgGroupDayMapGet)A_PtrMsg)->Group, ((Ptr_GrDvrFs3MngMsgGroupDayMapGet)A_PtrMsg)->TimeSt,
						((Ptr_GrDvrFs3MngMsgGroupDayMapGet)A_PtrMsg)->DayCnt );
					break;
				case E_GrDvrFs3MngMsgGroupMinMapGet:
					*((Ptr_GrDvrFs3MngMsgGroupMinMapGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupMinMapGet( ((Ptr_GrDvrFs3MngMsgGroupMinMapGet)A_PtrMsg)->Group, ((Ptr_GrDvrFs3MngMsgGroupMinMapGet)A_PtrMsg)->TimeSt,
						((Ptr_GrDvrFs3MngMsgGroupMinMapGet)A_PtrMsg)->ItemCnt, ((Ptr_GrDvrFs3MngMsgGroupMinMapGet)A_PtrMsg)->PtrBuf );
					break;
				case E_GrDvrFs3MngMsgReadForward:
					ImmReadForward( ((Ptr_GrDvrFs3MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs3MngMsgReadPara)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs3MngMsgReadBackward:
					ImmReadBackward( ((Ptr_GrDvrFs3MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs3MngMsgReadPara)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs3MngMsgReadBegin:
					ImmReadBegin( ((Ptr_GrDvrFs3MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs3MngMsgReadPara)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs3MngMsgReadEnd:
					ImmReadEnd( ((Ptr_GrDvrFs3MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs3MngMsgReadPara)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs3MngMsgReadByHndlFw:
					ImmReadByHndlFw( ((Ptr_GrDvrFs3MngMsgReadHndlPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs3MngMsgReadHndlPara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrDvrFs3MngMsgReadHndlPara)A_PtrMsg)->Hndl );
					break;
				case E_GrDvrFs3MngMsgReadByHndlBw:
					ImmReadByHndlBw( ((Ptr_GrDvrFs3MngMsgReadHndlPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs3MngMsgReadHndlPara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrDvrFs3MngMsgReadHndlPara)A_PtrMsg)->Hndl );
					break;
				case E_GrDvrFs3MngMsgReadByMinFw:
					ImmReadByMinFw( ((Ptr_GrDvrFs3MngMsgReadTimePara)A_PtrMsg)->Group, ((Ptr_GrDvrFs3MngMsgReadTimePara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrDvrFs3MngMsgReadTimePara)A_PtrMsg)->Time );
					break;
				case E_GrDvrFs3MngMsgReadByMinBw:
					ImmReadByMinBw( ((Ptr_GrDvrFs3MngMsgReadTimePara)A_PtrMsg)->Group, ((Ptr_GrDvrFs3MngMsgReadTimePara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrDvrFs3MngMsgReadTimePara)A_PtrMsg)->Time );
					break;
				case E_GrDvrFs3MngMsgSysInfo:
					ImmSysInfoGet( ((Ptr_GrDvrFs3MngMsgSysInfo)A_PtrMsg)->PtrInfo );
					break;
				case E_GrDvrFs3MngMsgGroupTimeRngGet:
					ImmGroupRecTimeRngGet( ((Ptr_GrDvrFs3MngMsgGroupTimeRngGet)A_PtrMsg)->Group, ((Ptr_GrDvrFs3MngMsgGroupTimeRngGet)A_PtrMsg)->PtrRtTimeSt,
						((Ptr_GrDvrFs3MngMsgGroupTimeRngGet)A_PtrMsg)->PtrRtTimeEd );
					break;
				case E_GrDvrFs3MngMsgLogFindStart:
					ImmLogFindStart( ((Ptr_GrDvrFs3MngMsgLogFind)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs3MngMsgLogFindNext:
					ImmLogFindNext( ((Ptr_GrDvrFs3MngMsgLogFind)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs3MngMsgLogFindPrev:
					ImmLogFindPrev( ((Ptr_GrDvrFs3MngMsgLogFind)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs3MngMsgScanRecSize:
					LcMprcScanSizeReq( (Ptr_GrDvrFs3MngMsgScanRecSize)A_PtrMsg );
					break;
				case E_GrDvrFs3MngMsgLogAdd:
					ImmLogAdd( &((Ptr_GrDvrFs3MngMsgLogAdd)A_PtrMsg)->Log ) ;
					break;
				case E_GrDvrFs3MngMsgAtDelTimeSet:
					ImmAtDelTimeSet( ((Ptr_GrDvrFs3MngMsgAtDelTimeSet)A_PtrMsg)->Sec );
					break;
						
				default:
					Tv_Result	=	FALSE;
					break;
			}
			
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::RtlTimer( __u32 A_Hndl )
{
		LcTempUpdt();
}
//--------------------------------------------------------------------
Cls_GrDvrFs3Disk*	Cls_GrDvrFs3Mng::RtlDiskObjGet( __u8 A_Idx )
{
	// local -------------------
		Cls_GrDvrFs3Disk*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( E_GrDvrFs3MaxDriveCnt > A_Idx )
		{
			Tv_Result	=	m_DiskTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs3Mng::AdoSyncCntGet( void )
{
		return	m_AdoSyncCnt;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs3Mng::AdoDataUnitSizeGet( void )
{
		return	m_AdoUnitSize;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::VideoAdd( Ptr_GrDvrAvHd A_PtrPkt )
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
BOOL8	Cls_GrDvrFs3Mng::AudioAdd( Ptr_GrDvrAvHd A_PtrPkt )
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
BOOL8	Cls_GrDvrFs3Mng::PacketAdd( Ptr_GrDvrAvHd A_PtrPkt )
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
void	Cls_GrDvrFs3Mng::SafeSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir, BOOL8 A_IsAutoFix )
{
	// local -------------------
		St_GrDvrFs3MngMsgSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs3MngMsgSetup;
		Tv_Msg.NrmMap						=	A_NormalMap;
		Tv_Msg.IsNrmOvwt				=	A_IsOvwtNrm;
		Tv_Msg.MirMap						=	A_MirrorMap;
		Tv_Msg.IsMirOvwt				=	A_IsOvwtMir;
		Tv_Msg.IsAutoFix				=	A_IsAutoFix;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::LcGroupBuild( void )
{
	// local -------------------
		__u8	Tv_GrIdx;
	// code --------------------
		// release group
		for ( Tv_GrIdx=0;Tv_GrIdx < E_GrDvrFs3MngGroupCnt;Tv_GrIdx++ )
		{
			if ( NULL != m_GrupTbl[Tv_GrIdx] )
			{
				delete	m_GrupTbl[Tv_GrIdx];
				m_GrupTbl[Tv_GrIdx]	=	NULL;
			}
		}

		// create normal group
		m_GrupTbl[E_GrDvrRecDevNoraml]	=	(Cls_GrDvrFs3Grp*) new Cls_GrDvrFs3Grp( this, m_GrupNrmMap, m_IsOvwtNrm, m_IsAutoFix );

		// create mirror group
		m_GrupTbl[E_GrDvrRecDevMirror]	=	(Cls_GrDvrFs3Grp*) new Cls_GrDvrFs3Grp( this, m_GrupMirMap, m_IsOvwtMir, m_IsAutoFix );
		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::ImmSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir, BOOL8 A_IsAutoFix )
{
	// local -------------------
		St_GrTaskMsgHd	Tv_Msg;
	// code --------------------
		// update group info
		m_GrupNrmMap	=	A_NormalMap;
		m_GrupMirMap	=	A_MirrorMap & (~A_NormalMap);
		m_IsOvwtNrm		=	A_IsOvwtNrm;
		m_IsOvwtMir		=	A_IsOvwtMir;
		m_IsAutoFix		=	A_IsAutoFix;
		LcGroupBuild();

		// file system setup
		Tv_Msg.Cmd		=	E_GrTaskMsgFsSetup;
		GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeInit( void )
{
	// local -------------------
		St_GrDvrFs3MngMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Cmd			=	E_GrDvrFs3MngMsgInit;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::ImmInit( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		// build disk list
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs3MaxDriveCnt;Tv_WkIdx++ )
		{
			m_DiskTbl[Tv_WkIdx]	=	(Cls_GrDvrFs3Disk*) new Cls_GrDvrFs3Disk( Tv_WkIdx, m_TmpBuf, m_DvrId, 0x1000000 );
			// check not exist disk
			if ( E_GrDvrRecStatNotexist == m_DiskTbl[Tv_WkIdx]->DiskStatGet() )
			{
				delete	m_DiskTbl[Tv_WkIdx];
				m_DiskTbl[Tv_WkIdx]	=	NULL;
			}
		}
		// temperature update
		LcTempUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::SafeFormat( __u8 A_DevIdx, BOOL8 A_IsWithLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs3MngMsgFormat	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs3MngMsgFormat;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.IsWithLog				=	A_IsWithLog;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::ImmFormat( __u8 A_DevIdx, BOOL8 A_IsWithLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrTaskMsgDiskFormat	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		// check valid device index
		if ( E_GrDvrFs3MaxDriveCnt > A_DevIdx )
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
__u8	Cls_GrDvrFs3Mng::SafeDiskStatusGet( __u8 A_DevIdx )
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs3MngMsgDiskStatGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs3MngMsgDiskStatGet;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs3Mng::ImmDiskStatGet( __u8 A_DevIdx )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		// check valid device index
		if ( E_GrDvrFs3MaxDriveCnt > A_DevIdx )
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
BOOL8	Cls_GrDvrFs3Mng::SafeDiskRecSizeGet( __u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs3MngMsgDiskRecSizeGet	Tv_Msg;
	// code --------------------
		Tv_Result			=	FALSE;
		*A_PtrRtTotal	=	0;
		*A_PtrRtRec		=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs3MngMsgDiskRecSizeGet;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.PtrRtTotal				=	A_PtrRtTotal;
		Tv_Msg.PtrRtRec					=	A_PtrRtRec;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::ImmDiskRecSizeGet( __u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrFs3MaxDriveCnt > A_DevIdx )
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
__u8	Cls_GrDvrFs3Mng::SafeGroupStatusGet( __u8 A_GroupType )
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs3MngMsgGroupStatGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs3MngMsgGroupStatGet;
		Tv_Msg.GroupType				=	A_GroupType;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::SafeGroupRecSizeGet( __u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs3MngMsgGroupRecSizeGet	Tv_Msg;
	// code --------------------
		Tv_Result			=	FALSE;
		*A_PtrRtTotal	=	0;
		*A_PtrRtRec		=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs3MngMsgGroupRecSizeGet;
		Tv_Msg.GroupType				=	A_GroupType;
		Tv_Msg.PtrRtTotal				=	A_PtrRtTotal;
		Tv_Msg.PtrRtRec					=	A_PtrRtRec;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs3Mng::ImmGroupStatGet( __u8 A_GroupType )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		if ( E_GrDvrFs3MngGroupCnt > A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				Tv_Result	=	m_GrupTbl[A_GroupType]->GroupStatusGet();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::ImmGroupRecSizeGet( __u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrFs3MngGroupCnt > A_GroupType )
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
__u32	Cls_GrDvrFs3Mng::SafeGroupDayMapGet( __u8 A_GroupType, Def_GrTime A_Time, __u8 A_DayCnt )
{
	// local -------------------
		__u32	Tv_Result;
		St_GrDvrFs3MngMsgGroupDayMapGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs3MngMsgGroupDayMapGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.TimeSt						=	A_Time;
		Tv_Msg.DayCnt						=	A_DayCnt;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs3Mng::ImmGroupDayMapGet( __u8 A_GroupType, Def_GrTime A_Time, __u8 A_DayCnt )
{
	// local -------------------
		__u32	Tv_Result;
		Def_GrTime	Tv_TimeEd;
		Def_GrTime	Tv_TimeSt;
		__u8	Tv_DayCnt;
		BOOL8	Tv_IsAble;
		__u32	Tv_DelIdx;
		__u32	Tv_DelMask;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrDvrFs3MngGroupCnt > A_GroupType )
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

					// check auto delete
					Tv_IsAble	=	TRUE;
					if ( 0 != m_AtDelTime )
					{
						LcAtDelLimUpdt();
						if ( Tv_TimeEd < m_AtDelBaseTm )
						{
							Tv_IsAble		=	FALSE;
						}
					}

					if ( Tv_IsAble )
					{
						Tv_Result	=	m_GrupTbl[A_GroupType]->DayMapGet( Tv_TimeSt, Tv_TimeEd );

						// check auto delete
						if ( 0 != m_AtDelTime )
						{
							// mask start time
							if ( Tv_TimeSt < m_AtDelBaseTm )
							{
								Tv_DelIdx		=	((m_AtDelBaseTm - Tv_TimeSt) / E_GrTimeSecOfDay);
								Tv_DelMask	=	(1 << Tv_DelIdx) - 1;
								Tv_Result		=	Tv_Result & (~Tv_DelMask);
							}
						}

					}
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::SafeGroupMinMapGet( __u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs3MngMsgGroupMinMapGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs3MngMsgGroupMinMapGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.TimeSt						=	A_TimeSt;
		Tv_Msg.ItemCnt					=	A_ItemCnt;
		Tv_Msg.PtrBuf						=	A_PtrBuf;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::ImmGroupMinMapGet( __u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, void* A_PtrBuf )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Size;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrFs3MngGroupCnt > A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				Tv_Result	=	TRUE;
				// check auto delete
				if ( 0 != m_AtDelTime )
				{
					LcAtDelLimUpdt();
					if ( A_TimeSt < m_AtDelBaseTm )
					{
						Tv_Size	=	A_ItemCnt << E_GrDvrRecMinMapShft;
						GrDumyZeroMem( A_PtrBuf, Tv_Size );
						Tv_Result	=	FALSE;
					}
				}

				if ( Tv_Result )
				{
					Tv_Result	=	m_GrupTbl[A_GroupType]->MinMapGet( A_TimeSt, A_PtrBuf, A_ItemCnt );
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::ImmGroupRecTimeRngGet( __u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd )
{
	// local -------------------
	// code --------------------
		*A_PtrRtTimeSt	=	0;
		*A_PtrRtTimeEd	=	0;
		if ( E_GrDvrFs3MngGroupCnt > A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				m_GrupTbl[A_GroupType]->RecTimeRngGet( A_PtrRtTimeSt, A_PtrRtTimeEd );
				if ( 0 != m_AtDelTime )
				{
					LcAtDelLimUpdt();
					if ( m_AtDelBaseTm > (*A_PtrRtTimeEd) )
					{
						*A_PtrRtTimeSt	=	0;
						*A_PtrRtTimeEd	=	0;
					}
					else if ( m_AtDelBaseTm > (*A_PtrRtTimeSt) )
					{
						*A_PtrRtTimeSt	=	m_AtDelBaseTm;
					}
				}
				
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeGroupRecTimeRngGet( __u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd )
{
	// local -------------------
		St_GrDvrFs3MngMsgGroupTimeRngGet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs3MngMsgGroupTimeRngGet;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.PtrRtTimeSt			=	A_PtrRtTimeSt;
		Tv_Msg.PtrRtTimeEd			=	A_PtrRtTimeEd;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::ImmReadForward( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl )
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
void	Cls_GrDvrFs3Mng::ImmReadBackward( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl )
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
void	Cls_GrDvrFs3Mng::ImmReadBegin( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl )
{
	// local -------------------
	// code --------------------
		if ( 0 != m_AtDelTime )
		{
			LcAtDelLimUpdt();
			ImmReadByMinFw( A_DevType, A_PtrCtrl, m_AtDelBaseTm );
		}
		else
		{
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
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::ImmReadEnd( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl )
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
void	Cls_GrDvrFs3Mng::ImmReadByHndlFw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, __u64 A_Hndl )
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
void	Cls_GrDvrFs3Mng::ImmReadByHndlBw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, __u64 A_Hndl )
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
void	Cls_GrDvrFs3Mng::ImmReadByMinFw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
		Def_GrTime	Tv_TimeSt;
	// code --------------------
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			Tv_TimeSt	=	A_Time;
			if ( 0 != m_AtDelTime )
			{
				LcAtDelLimUpdt();
				if ( Tv_TimeSt < m_AtDelBaseTm )
				{
					Tv_TimeSt	=	m_AtDelBaseTm;
				}
			}
			m_GrupTbl[A_DevType]->ReadByTimeFw( A_PtrCtrl, Tv_TimeSt );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::ImmReadByMinBw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
		Def_GrTime	Tv_TimeAble;
	// code --------------------
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			Tv_TimeAble	=	0;
			if ( 0 != m_AtDelTime )
			{
				LcAtDelLimUpdt();
				Tv_TimeAble	=	m_AtDelBaseTm;
			}
			m_GrupTbl[A_DevType]->ReadByTimeBw( A_PtrCtrl, A_Time, Tv_TimeAble );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeReadForward( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs3MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgReadForward;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeReadBackward( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs3MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgReadBackward;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeReadBegin( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs3MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgReadBegin;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeReadEnd( __u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs3MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgReadEnd;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeReadByHndlFw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
	// local -------------------
		St_GrDvrFs3MngMsgReadHndlPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgReadByHndlFw;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.Hndl						=	A_Hndl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeReadByHndlBw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
	// local -------------------
		St_GrDvrFs3MngMsgReadHndlPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgReadByHndlBw;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.Hndl						=	A_Hndl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeReadByMinFw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
		St_GrDvrFs3MngMsgReadTimePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgReadByMinFw;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.Time						=	A_Time;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeReadByMinBw(__u8 A_DevType, Ptr_GrDvrFs3SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
		St_GrDvrFs3MngMsgReadTimePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgReadByMinBw;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.Time						=	A_Time;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::ImmLogAdd( Ptr_GrDvrLogV2Item A_PtrItem )
{
	// local -------------------
		__u8	Tv_DevType;
	// code --------------------
		//check device type
		for ( Tv_DevType=0;Tv_DevType < E_GrDvrFs3MngGroupCnt;Tv_DevType++ )
		{
			if ( NULL != m_GrupTbl[Tv_DevType] )
			{
				m_GrupTbl[Tv_DevType]->LogAdd( A_PtrItem );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::ImmLogFindStart( Ptr_GrDvrLogV2Ctrl A_PtrCtrl )
{
		//check device type
		if ( E_GrDvrFs3MngGroupCnt > A_PtrCtrl->DevType )
		{
			if ( NULL != m_GrupTbl[A_PtrCtrl->DevType] )
			{
				m_GrupTbl[A_PtrCtrl->DevType]->LogFindStart( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::ImmLogFindNext( Ptr_GrDvrLogV2Ctrl A_PtrCtrl )
{
		//check device type
		if ( E_GrDvrFs3MngGroupCnt > A_PtrCtrl->DevType )
		{
			if ( NULL != m_GrupTbl[A_PtrCtrl->DevType] )
			{
				m_GrupTbl[A_PtrCtrl->DevType]->LogFindNext( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::ImmLogFindPrev( Ptr_GrDvrLogV2Ctrl A_PtrCtrl )
{
		//check device type
		if ( E_GrDvrRecDevMirror >= A_PtrCtrl->DevType )
		{
			if ( NULL != m_GrupTbl[A_PtrCtrl->DevType] )
			{
				m_GrupTbl[A_PtrCtrl->DevType]->LogFindPrev( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeLogAdd( Ptr_GrDvrLogV2Item A_PtrItem )
{
	// local -------------------
		St_GrDvrFs3MngMsgLogAdd	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgLogAdd;
		GrDumyCopyMem( &Tv_Msg.Log, A_PtrItem, sizeof(St_GrDvrLogV2Item) );
		
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeLogFindStart( Ptr_GrDvrLogV2Ctrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs3MngMsgLogFind	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgLogFindStart;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeLogFindNext( Ptr_GrDvrLogV2Ctrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs3MngMsgLogFind	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgLogFindNext;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeLogFindPrev( Ptr_GrDvrLogV2Ctrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs3MngMsgLogFind	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgLogFindPrev;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::ImmSysInfoGet( Ptr_GrDvrFs3MngSysInfo A_PtrRtInfo )
{
	// local -------------------
		__u8	Tv_DskIdx;
	// code --------------------
		GrDumyZeroMem( A_PtrRtInfo, sizeof(St_GrDvrFs3MngSysInfo) );
		for ( Tv_DskIdx=0;Tv_DskIdx < E_GrDvrFs3MaxDriveCnt;Tv_DskIdx++ )
		{
			if ( NULL != m_DiskTbl[Tv_DskIdx] )
			{
				A_PtrRtInfo->DskStat[Tv_DskIdx]	=	m_DiskTbl[Tv_DskIdx]->DiskStatGet();
				A_PtrRtInfo->DskTemp[Tv_DskIdx]	=	m_TempTbl[Tv_DskIdx];
			}
		}
		// main group info
		if ( NULL != m_GrupTbl[0] )
		{
			A_PtrRtInfo->MainStat	=	m_GrupTbl[0]->GroupStatusGet();
			m_GrupTbl[0]->RecSizeGet( &A_PtrRtInfo->MainCapTot, &A_PtrRtInfo->MainCapRec );
			m_GrupTbl[0]->RecTimeRngGet( &A_PtrRtInfo->MainRecTimeSt, &A_PtrRtInfo->MainRecTimeEd );
		}
		// mirror group info
		if ( NULL != m_GrupTbl[1] )
		{
			A_PtrRtInfo->MirrStat	=	m_GrupTbl[1]->GroupStatusGet();
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::LcTempUpdt( void )
{
	// local -------------------
		__u8	Tv_DskIdx;
	// code --------------------
		m_TempTimer ++;
		if ( E_GrDvrFs3MngTempScanPeriod <= m_TempTimer )
		{
			for ( Tv_DskIdx=0;Tv_DskIdx < E_GrDvrFs3MaxDriveCnt;Tv_DskIdx++ )
			{
				if ( NULL != m_DiskTbl[Tv_DskIdx] )
				{
					m_TempTbl[Tv_DskIdx]	=	m_DiskTbl[Tv_DskIdx]->TempratureRead();
				}
			}
			m_TempTimer	=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeSysInfoGet( Ptr_GrDvrFs3MngSysInfo A_PtrRtInfo )
{
	// local -------------------
		St_GrDvrFs3MngMsgSysInfo	Tv_Msg;
	// code --------------------
		GrDumyZeroMem( A_PtrRtInfo, sizeof(St_GrDvrFs3MngSysInfo) );

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs3MngMsgSysInfo;
		Tv_Msg.PtrInfo					=	A_PtrRtInfo;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs3Mng::TempratureGet( __u8 A_DskIdx )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrDvrFs3MaxDriveCnt > A_DskIdx )
		{
			Tv_Result	=	m_TempTbl[A_DskIdx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::ImmGroupRecAble( __u8 A_GroupType )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrRecDevMirror >= A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				Tv_Result	=	m_GrupTbl[A_GroupType]->IsRecAble();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::LcMprcScanSizeReq( Ptr_GrDvrFs3MngMsgScanRecSize A_PtrMsg )
{
	// local -------------------
	// code --------------------
		// check mode
		if ( E_GrDvrFs3MngScanSizeModeNone == m_ScanSizeMode )
		{
			// build request
			m_ScanSizeReq.DevType	=	A_PtrMsg->Group;
			m_ScanSizeReq.ChMask	=	A_PtrMsg->ChMask;
			m_ScanSizeReq.TimeSt	=	A_PtrMsg->TimeSt;
			m_ScanSizeReq.TimeEd	=	A_PtrMsg->TimeEd;

			m_ScanSizeReq.DskIdx	=	0;
			m_ScanSizeReq.BcsIdx	=	0;
			m_ScanSizeReq.Rslt		=	0;
						
			m_ScanSizeMode				=	E_GrDvrFs3MngScanSizeModeScan;

			*A_PtrMsg->PtrRslt		=	TRUE;
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs3Mng::ImmScanRecSize - busy.\n" );
			*A_PtrMsg->PtrRslt		=	FALSE;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::SafeScanRecSize( __u8 A_GroupType, __u16 A_ChMask, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u64* A_PtrRtSize )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs3MngMsgScanRecSize	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		*A_PtrRtSize	=	0;

		// check device type
		if ( E_GrDvrRecDevMirror >= A_GroupType )
		{
			// check channel mask
			if ( 0 != A_ChMask )
			{
				// check time range
				if ( A_TimeSt <= A_TimeEd )
				{
					// request message
					Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
					Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgScanRecSize;
					Tv_Msg.Group					=	A_GroupType;
					Tv_Msg.ChMask					=	A_ChMask;
					Tv_Msg.TimeSt					=	A_TimeSt;
					Tv_Msg.TimeEd					=	A_TimeEd;
					Tv_Msg.PtrRslt				=	&Tv_Result;

					m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
					if ( Tv_Result )
					{
						// wait finish
						while ( E_GrDvrFs3MngScanSizeModeScan == m_ScanSizeMode )
						{
							// sleep
							GrPrcSleep( 10 );
						}
						*A_PtrRtSize		=	m_ScanSizeReq.Rslt;
						m_ScanSizeMode	=	E_GrDvrFs3MngScanSizeModeNone;
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrDvrFs3Mng::SafeScanRecSize - bad time range.\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrDvrFs3Mng::SafeScanRecSize - channel not selected.\n" );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( LcBgpScanRecSize() )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs3Mng::LcBgpScanRecSize( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrFs3MngScanSizeModeScan == m_ScanSizeMode )
		{
			// scan
			if ( m_GrupTbl[m_ScanSizeReq.DevType]->ScanRecSize( &m_ScanSizeReq ) )
			{
				// finish
				m_ScanSizeMode	=	E_GrDvrFs3MngScanSizeModeFinish;
			}

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::ImmAtDelTimeSet( __u32 A_Sec )
{
		m_AtDelTime	=	A_Sec;
		// update auto delete time
		LcAtDelLimUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::SafeAtDelTimeSet( __u32 A_Sec )
{
	// local -------------------
		St_GrDvrFs3MngMsgAtDelTimeSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs3MngMsgAtDelTimeSet;
		Tv_Msg.Sec						=	A_Sec;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs3Mng::AutoDelTimeGet( void )
{
		return	m_AtDelTime;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs3Mng::LcAtDelLimUpdt( void )
{
	// local -------------------
		BOOL8	Tv_IsSm;
		__u32	Tv_Mili;
		Def_GrTime	Tv_TimeAble;
	// code --------------------
		Tv_TimeAble			=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
		m_AtDelBaseTm		=	(Tv_TimeAble - (Tv_TimeAble % E_GrTimeSecOfDay) - m_AtDelTime) + E_GrTimeSecOfDay;
}
//--------------------------------------------------------------------
