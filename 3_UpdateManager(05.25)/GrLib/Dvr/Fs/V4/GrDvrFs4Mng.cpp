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

#include	<Dvr/Fs/V4/GrDvrFs4Mng.h>

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
Cls_GrDvrFs4Mng::Cls_GrDvrFs4Mng(Cls_GrTaskSvr* A_TaskSvr, __u64 A_DvrId, Ptr_GrBufTsMng A_PtrStrmBuf, __u8 A_BufThrdIdx, __u8 A_BufOwnerIdx) :
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		//init
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		m_DvrId			=	A_DvrId;

		m_PtrBufStrm	=	A_PtrStrmBuf;
		m_BufThrdIdx	=	A_BufThrdIdx;
		m_BufOwnerMap	=	3 << A_BufOwnerIdx;
		m_IsInited	=	FALSE;

		m_RecReqIdx		=	0;
		m_RecReqCnt		=	0;
		GrDumyZeroMem(m_RecReqTbl, sizeof(m_RecReqTbl));

		m_GrupNrmMap	=	0;
		m_GrupMirMap	=	0;
		m_IsOvwtNrm		=	FALSE;
		m_IsOvwtMir		=	FALSE;

		m_TempTimer		=	0;

		m_AtDelTime		=	0;
		m_AtDelBaseTm	=	0;

		m_TmpUsed			=	0;

		GrDumyZeroMem(&m_BkupSizeEstm, sizeof(m_BkupSizeEstm));

		GrDumyZeroMem( m_DiskTbl, sizeof(m_DiskTbl) );
		GrDumyZeroMem( m_GrupTbl, sizeof(m_GrupTbl) );
		GrDumyZeroMem( m_TempTbl, sizeof(m_TempTbl) );

		//register task
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrDvrFs4MngTimerPeriod, &m_HndlTimer );
		m_TaskSvr->RegistBkgClient( this );

}
//--------------------------------------------------------------------
Cls_GrDvrFs4Mng::~Cls_GrDvrFs4Mng()
{
		//unregister timer
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistMsgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );


		GrTaskSvrMsgCmdFree( m_MsgAloc );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check input message
		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			Tv_Result	=	TRUE;
			switch ( ((Ptr_GrDvrFs4MngMsgBase)A_PtrMsg)->Cmd )
			{
				case E_GrDvrFs4MngMsgInit:
					ImmInit();
					break;
				case E_GrDvrFs4MngMsgSetup:
					ImmSetup( ((Ptr_GrDvrFs4MngMsgSetup)A_PtrMsg)->NrmMap, ((Ptr_GrDvrFs4MngMsgSetup)A_PtrMsg)->MirMap,
						((Ptr_GrDvrFs4MngMsgSetup)A_PtrMsg)->IsNrmOvwt, ((Ptr_GrDvrFs4MngMsgSetup)A_PtrMsg)->IsMirOvwt,
						((Ptr_GrDvrFs4MngMsgSetup)A_PtrMsg)->IsAutoFix );
					break;
				case E_GrDvrFs4MngMsgFormat:
					*((Ptr_GrDvrFs4MngMsgFormat)A_PtrMsg)->PtrRslt	=	
						ImmFormat( ((Ptr_GrDvrFs4MngMsgFormat)A_PtrMsg)->DevIdx, ((Ptr_GrDvrFs4MngMsgFormat)A_PtrMsg)->IsWithLog );
					break;
				case E_GrDvrFs4MngMsgDiskStatGet:
					*((Ptr_GrDvrFs4MngMsgDiskStatGet)A_PtrMsg)->PtrRslt	=	
						ImmDiskGetStat( ((Ptr_GrDvrFs4MngMsgDiskStatGet)A_PtrMsg)->DevIdx );
					break;
				case E_GrDvrFs4MngMsgDiskRecSizeGet:
					*((Ptr_GrDvrFs4MngMsgDiskRecSizeGet)A_PtrMsg)->PtrRslt	=	
						ImmDiskGetRecSize( ((Ptr_GrDvrFs4MngMsgDiskRecSizeGet)A_PtrMsg)->DevIdx, ((Ptr_GrDvrFs4MngMsgDiskRecSizeGet)A_PtrMsg)->PtrRtTotal,
						((Ptr_GrDvrFs4MngMsgDiskRecSizeGet)A_PtrMsg)->PtrRtRec );
					break;
				case E_GrDvrFs4MngMsgGroupStatGet:
					*((Ptr_GrDvrFs4MngMsgGroupStatGet)A_PtrMsg)->PtrRslt	=
						ImmGroupGetStat( ((Ptr_GrDvrFs4MngMsgGroupStatGet)A_PtrMsg)->GroupType );
					break;
				case E_GrDvrFs4MngMsgGroupRecSizeGet:
					*((Ptr_GrDvrFs4MngMsgGroupRecSizeGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupGetRecSize( ((Ptr_GrDvrFs4MngMsgGroupRecSizeGet)A_PtrMsg)->GroupType, ((Ptr_GrDvrFs4MngMsgGroupRecSizeGet)A_PtrMsg)->PtrRtTotal,
						((Ptr_GrDvrFs4MngMsgGroupRecSizeGet)A_PtrMsg)->PtrRtRec );
					break;
				case E_GrDvrFs4MngMsgGroupDayMapGet:
					*((Ptr_GrDvrFs4MngMsgGroupDayMapGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupGetDayMap( ((Ptr_GrDvrFs4MngMsgGroupDayMapGet)A_PtrMsg)->Group, ((Ptr_GrDvrFs4MngMsgGroupDayMapGet)A_PtrMsg)->TimeSt,
						((Ptr_GrDvrFs4MngMsgGroupDayMapGet)A_PtrMsg)->DayCnt );
					break;
				case E_GrDvrFs4MngMsgGroupMinMapGet:
					*((Ptr_GrDvrFs4MngMsgGroupMinMapGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupGetMinMap( ((Ptr_GrDvrFs4MngMsgGroupMinMapGet)A_PtrMsg)->Group, ((Ptr_GrDvrFs4MngMsgGroupMinMapGet)A_PtrMsg)->TimeSt,
						((Ptr_GrDvrFs4MngMsgGroupMinMapGet)A_PtrMsg)->ItemCnt, ((Ptr_GrDvrFs4MngMsgGroupMinMapGet)A_PtrMsg)->PtrBuf );
					break;
				case E_GrDvrFs4MngMsgReadFw:
					*((Ptr_GrDvrFs4MngMsgReadPara)A_PtrMsg)->PtrRslt		=	
						ImmReadFw(((Ptr_GrDvrFs4MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs4MngMsgReadPara)A_PtrMsg)->PtrCtrl);
					break;
				case E_GrDvrFs4MngMsgReadTimeFw:
					*((Ptr_GrDvrFs4MngMsgReadTimePara)A_PtrMsg)->PtrRslt		=
						ImmReadTimeFw(((Ptr_GrDvrFs4MngMsgReadTimePara)A_PtrMsg)->Group, ((Ptr_GrDvrFs4MngMsgReadTimePara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrDvrFs4MngMsgReadTimePara)A_PtrMsg)->Time);
					break;
				case E_GrDvrFs4MngMsgReadBw:
					*((Ptr_GrDvrFs4MngMsgReadTimePara)A_PtrMsg)->PtrRslt		=
						ImmReadBw(((Ptr_GrDvrFs4MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs4MngMsgReadPara)A_PtrMsg)->PtrCtrl);
					break;
				case E_GrDvrFs4MngMsgReadBegin:
					ImmReadBegin( ((Ptr_GrDvrFs4MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs4MngMsgReadPara)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs4MngMsgSysInfo:
					ImmSysGetInfo( ((Ptr_GrDvrFs4MngMsgSysInfo)A_PtrMsg)->PtrInfo );
					break;
				case E_GrDvrFs4MngMsgGroupTimeRngGet:
					ImmGroupGetRecTimeRng( ((Ptr_GrDvrFs4MngMsgGroupTimeRngGet)A_PtrMsg)->Group, ((Ptr_GrDvrFs4MngMsgGroupTimeRngGet)A_PtrMsg)->PtrRtTimeSt,
						((Ptr_GrDvrFs4MngMsgGroupTimeRngGet)A_PtrMsg)->PtrRtTimeEd );
					break;
				case E_GrDvrFs4MngMsgLogFindStart:
					ImmLogFindStart( ((Ptr_GrDvrFs4MngMsgLogFind)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs4MngMsgLogFindNext:
					ImmLogFindNext( ((Ptr_GrDvrFs4MngMsgLogFind)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs4MngMsgLogFindPrev:
					ImmLogFindPrev( ((Ptr_GrDvrFs4MngMsgLogFind)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs4MngMsgBkupSizeEstm:
					*(((Ptr_GrDvrFs4MngMsgBkupSizeEstm)A_PtrMsg)->PtrRslt)		=	
						ImmBkupSizeEstmate(((Ptr_GrDvrFs4MngMsgBkupSizeEstm)A_PtrMsg)->Group, ((Ptr_GrDvrFs4MngMsgBkupSizeEstm)A_PtrMsg)->ChMap,
						((Ptr_GrDvrFs4MngMsgBkupSizeEstm)A_PtrMsg)->TimeSt, ((Ptr_GrDvrFs4MngMsgBkupSizeEstm)A_PtrMsg)->TimeEd);
					break;
				case E_GrDvrFs4MngMsgLogAdd:
					ImmLogAdd( &((Ptr_GrDvrFs4MngMsgLogAdd)A_PtrMsg)->Log ) ;
					break;
				case E_GrDvrFs4MngMsgAtDelTimeSet:
					ImmAtDelTimeSet( ((Ptr_GrDvrFs4MngMsgAtDelTimeSet)A_PtrMsg)->Sec );
					break;
				case E_GrDvrFs4MngMsgGroupFormat:
					ImmGroupFormat(((Ptr_GrDvrFs4MngMsgGroupFormat)A_PtrMsg)->GroupType, ((Ptr_GrDvrFs4MngMsgGroupFormat)A_PtrMsg)->IsWithLog);
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
			
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::RtlTimer( __u32 A_Hndl )
{
		LcTempUpdt();
}
//--------------------------------------------------------------------
Cls_GrDvrFs4Disk*	Cls_GrDvrFs4Mng::RtlDiskObjGet( __u8 A_Idx )
{
	// local -------------------
		Cls_GrDvrFs4Disk*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( E_GrDvrFs4MaxDriveCnt > A_Idx )
		{
			Tv_Result	=	m_DiskTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::SafeSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir, BOOL8 A_IsAutoFix )
{
	// local -------------------
		St_GrDvrFs4MngMsgSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs4MngMsgSetup;
		Tv_Msg.NrmMap						=	A_NormalMap;
		Tv_Msg.IsNrmOvwt				=	A_IsOvwtNrm;
		Tv_Msg.MirMap						=	A_MirrorMap;
		Tv_Msg.IsMirOvwt				=	A_IsOvwtMir;
		Tv_Msg.IsAutoFix				=	A_IsAutoFix;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::LcGroupBuild( void )
{
	// local -------------------
		__u8	Tv_GrIdx;
	// code --------------------
		// release group
		for ( Tv_GrIdx=0;Tv_GrIdx < E_GrDvrFs4MngGroupCnt;Tv_GrIdx++ )
		{
			if ( NULL != m_GrupTbl[Tv_GrIdx] )
			{
				delete	m_GrupTbl[Tv_GrIdx];
				m_GrupTbl[Tv_GrIdx]	=	NULL;
			}
		}

		// create normal group
		m_GrupTbl[E_GrDvrRecDevNoraml]	=	(Cls_GrDvrFs4Grp*) new Cls_GrDvrFs4Grp( this, m_GrupNrmMap, m_IsOvwtNrm );

		// create mirror group
		m_GrupTbl[E_GrDvrRecDevMirror]	=	(Cls_GrDvrFs4Grp*) new Cls_GrDvrFs4Grp(this, m_GrupMirMap, m_IsOvwtMir );
		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::ImmSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir, BOOL8 A_IsAutoFix )
{
	// local -------------------
		St_GrTaskMsgHd	Tv_Msg;
	// code --------------------
		// update group info
		m_GrupNrmMap	=	A_NormalMap;
		m_GrupMirMap	=	A_MirrorMap & (~A_NormalMap);
		m_IsOvwtNrm		=	A_IsOvwtNrm;
		m_IsOvwtMir		=	A_IsOvwtMir;
		LcGroupBuild();

		// file system setup
		Tv_Msg.Cmd		=	E_GrTaskMsgFsSetup;
		GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::SafeInit( void )
{
	// local -------------------
		St_GrDvrFs4MngMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Cmd			=	E_GrDvrFs4MngMsgInit;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::ImmInit( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		// build disk list
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrFs4MaxDriveCnt;Tv_WkIdx++ )
		{
			m_DiskTbl[Tv_WkIdx]	=	(Cls_GrDvrFs4Disk*) new Cls_GrDvrFs4Disk( Tv_WkIdx, (void*)this );
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
BOOL8	Cls_GrDvrFs4Mng::SafeFormat( __u8 A_DevIdx, BOOL8 A_IsWithLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs4MngMsgFormat	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs4MngMsgFormat;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.IsWithLog				=	A_IsWithLog;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::ImmFormat( __u8 A_DevIdx, BOOL8 A_IsWithLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrTaskMsgDiskFormat	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		// check valid device index
		if ( E_GrDvrFs4MaxDriveCnt > A_DevIdx )
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
__u8	Cls_GrDvrFs4Mng::SafeDiskGetStat(__u8 A_DevIdx)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs4MngMsgDiskStatGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs4MngMsgDiskStatGet;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::ImmDiskGetStat(__u8 A_DevIdx)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		// check valid device index
		if ( E_GrDvrFs4MaxDriveCnt > A_DevIdx )
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
BOOL8	Cls_GrDvrFs4Mng::SafeDiskGetRecSize(__u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs4MngMsgDiskRecSizeGet	Tv_Msg;
	// code --------------------
		Tv_Result			=	FALSE;
		*A_PtrRtTotal	=	0;
		*A_PtrRtRec		=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs4MngMsgDiskRecSizeGet;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.PtrRtTotal				=	A_PtrRtTotal;
		Tv_Msg.PtrRtRec					=	A_PtrRtRec;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::ImmDiskGetRecSize(__u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrDvrFs4MaxDriveCnt > A_DevIdx )
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
__u8	Cls_GrDvrFs4Mng::SafeGroupGetStat(__u8 A_GroupType)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs4MngMsgGroupStatGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs4MngMsgGroupStatGet;
		Tv_Msg.GroupType				=	A_GroupType;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::SafeGroupGetRecSize(__u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs4MngMsgGroupRecSizeGet	Tv_Msg;
	// code --------------------
		Tv_Result			=	FALSE;
		*A_PtrRtTotal	=	0;
		*A_PtrRtRec		=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs4MngMsgGroupRecSizeGet;
		Tv_Msg.GroupType				=	A_GroupType;
		Tv_Msg.PtrRtTotal				=	A_PtrRtTotal;
		Tv_Msg.PtrRtRec					=	A_PtrRtRec;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::ImmGroupGetStat(__u8 A_GroupType)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		if ( E_GrDvrFs4MngGroupCnt > A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				Tv_Result	=	m_GrupTbl[A_GroupType]->GroupGetStatus();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::ImmGroupGetRecSize(__u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrFs4MngGroupCnt > A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				m_GrupTbl[A_GroupType]->RecGetSize( A_PtrRtTotal, A_PtrRtRec );
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs4Mng::SafeGroupGetDayMap(__u8 A_GroupType, Def_GrTime A_Time, __u8 A_DayCnt)
{
	// local -------------------
		__u32	Tv_Result;
		St_GrDvrFs4MngMsgGroupDayMapGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs4MngMsgGroupDayMapGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.TimeSt						=	A_Time;
		Tv_Msg.DayCnt						=	A_DayCnt;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs4Mng::ImmGroupGetDayMap(__u8 A_GroupType, Def_GrTime A_Time, __u8 A_DayCnt)
{
	// local -------------------
		__u32	Tv_Result;
		Def_GrTime	Tv_TimeEd;
		Def_GrTime	Tv_TimeSt;
		__u8	Tv_DayCnt;
		__u32	Tv_DelIdx;
		__u32	Tv_DelMask;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrDvrFs4MngGroupCnt > A_GroupType )
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
BOOL8	Cls_GrDvrFs4Mng::SafeGroupGetMinMap(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrBuf)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs4MngMsgGroupMinMapGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs4MngMsgGroupMinMapGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.TimeSt						=	A_TimeSt;
		Tv_Msg.ItemCnt					=	A_ItemCnt;
		Tv_Msg.PtrBuf						=	A_PtrBuf;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::ImmGroupGetMinMap(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrRtBuf)
{
	// local -------------------
		BOOL8	Tv_Result;
		Def_GrTime	Tv_TimeEd;
		__u32	Tv_Size;
	// code --------------------
		Tv_Result	=	FALSE;

		// reset
		Tv_Size	=	sizeof(St_GrDvrRecMinMap)* A_ItemCnt;
		GrDumyZeroMem(A_PtrRtBuf, Tv_Size);

		if (0 != A_ItemCnt)
		{
			if (E_GrDvrFs4MngGroupCnt > A_GroupType)
			{
				if (NULL != m_GrupTbl[A_GroupType])
				{
					Tv_TimeEd	=	A_TimeSt + ((A_ItemCnt - 1) * E_GrTimeSecOfMin);
					Tv_Result	=	m_GrupTbl[A_GroupType]->MinMapGet(A_TimeSt,Tv_TimeEd,A_PtrRtBuf);
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::ImmGroupGetRecTimeRng(__u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd)
{
	// local -------------------
	// code --------------------
		*A_PtrRtTimeSt	=	0;
		*A_PtrRtTimeEd	=	0;
		if ( E_GrDvrFs4MngGroupCnt > A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				m_GrupTbl[A_GroupType]->RecGetTimeRng( A_PtrRtTimeSt, A_PtrRtTimeEd );
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
void	Cls_GrDvrFs4Mng::SafeGroupGetRecTimeRng(__u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd)
{
	// local -------------------
		St_GrDvrFs4MngMsgGroupTimeRngGet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs4MngMsgGroupTimeRngGet;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.PtrRtTimeSt			=	A_PtrRtTimeSt;
		Tv_Msg.PtrRtTimeEd			=	A_PtrRtTimeEd;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::ImmReadFw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4RsltParamBad;
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrupTbl[A_DevType] )
			{
				Tv_Result	=	m_GrupTbl[A_DevType]->ReadFw(A_PtrCtrl,A_PtrCtrl->BufTbl[A_PtrCtrl->BufIdx].Prgs);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::ImmReadBegin(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4RsltParamBad;
		//check device type
		if (E_GrDvrRecDevMirror >= A_DevType)
		{
			if (NULL != m_GrupTbl[A_DevType])
			{
				Tv_Result	=	m_GrupTbl[A_DevType]->ReadBegin(A_PtrCtrl);
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::SafeReadFw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs4MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs4MngMsgReadFw;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::SafeReadBw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs4MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs4MngMsgReadBw;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::SafeReadBegin(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs4MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs4MngMsgReadBegin;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::ImmLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		__u8	Tv_DevType;
	// code --------------------
		//check device type
		for ( Tv_DevType=0;Tv_DevType < E_GrDvrFs4MngGroupCnt;Tv_DevType++ )
		{
			if ( NULL != m_GrupTbl[Tv_DevType] )
			{
				m_GrupTbl[Tv_DevType]->LogAdd( A_PtrItem );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::ImmLogFindStart( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
		//check device type
		if ( E_GrDvrFs4MngGroupCnt > A_PtrCtrl->DevType )
		{
			if ( NULL != m_GrupTbl[A_PtrCtrl->DevType] )
			{
				m_GrupTbl[A_PtrCtrl->DevType]->LogFindStart( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::ImmLogFindNext( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
		//check device type
		if ( E_GrDvrFs4MngGroupCnt > A_PtrCtrl->DevType )
		{
			if ( NULL != m_GrupTbl[A_PtrCtrl->DevType] )
			{
				m_GrupTbl[A_PtrCtrl->DevType]->LogFindNext( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::ImmLogFindPrev( Ptr_GrDvrLogCtrl A_PtrCtrl )
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
void	Cls_GrDvrFs4Mng::SafeLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		St_GrDvrFs4MngMsgLogAdd	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs4MngMsgLogAdd;
		GrDumyCopyMem( &Tv_Msg.Log, A_PtrItem, sizeof(St_GrDvrLogItm) );
		
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::SafeLogFindStart( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs4MngMsgLogFind	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs4MngMsgLogFindStart;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::SafeLogFindNext( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs4MngMsgLogFind	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs4MngMsgLogFindNext;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::SafeLogFindPrev( Ptr_GrDvrLogCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs4MngMsgLogFind	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs4MngMsgLogFindPrev;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::ImmSysGetInfo(Ptr_GrDvrFs4MngSysInfo A_PtrRtInfo)
{
	// local -------------------
		__u8	Tv_DskIdx;
	// code --------------------
		GrDumyZeroMem( A_PtrRtInfo, sizeof(St_GrDvrFs4MngSysInfo) );
		for ( Tv_DskIdx=0;Tv_DskIdx < E_GrDvrFs4MaxDriveCnt;Tv_DskIdx++ )
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
			A_PtrRtInfo->MainStat	=	m_GrupTbl[0]->GroupGetStatus();
			m_GrupTbl[0]->RecGetSize( &A_PtrRtInfo->MainCapTot, &A_PtrRtInfo->MainCapRec );
			m_GrupTbl[0]->RecGetTimeRng( &A_PtrRtInfo->MainRecTimeSt, &A_PtrRtInfo->MainRecTimeEd );
		}
		// mirror group info
		if ( NULL != m_GrupTbl[1] )
		{
			A_PtrRtInfo->MirrStat	=	m_GrupTbl[1]->GroupGetStatus();
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::LcTempUpdt( void )
{
	// local -------------------
		__u8	Tv_DskIdx;
	// code --------------------
		m_TempTimer ++;
		if ( E_GrDvrFs4MngTempScanPeriod <= m_TempTimer )
		{
			for ( Tv_DskIdx=0;Tv_DskIdx < E_GrDvrFs4MaxDriveCnt;Tv_DskIdx++ )
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
void	Cls_GrDvrFs4Mng::SafeSysGetInfo(Ptr_GrDvrFs4MngSysInfo A_PtrRtInfo)
{
	// local -------------------
		St_GrDvrFs4MngMsgSysInfo	Tv_Msg;
	// code --------------------
		GrDumyZeroMem( A_PtrRtInfo, sizeof(St_GrDvrFs4MngSysInfo) );

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs4MngMsgSysInfo;
		Tv_Msg.PtrInfo					=	A_PtrRtInfo;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::TempratureGet( __u8 A_DskIdx )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrDvrFs4MaxDriveCnt > A_DskIdx )
		{
			Tv_Result	=	m_TempTbl[A_DskIdx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::ImmIsGroupRecAble(__u8 A_GroupType)
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
BOOL8	Cls_GrDvrFs4Mng::ImmBkupSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check mode
		if (!m_BkupSizeEstm.IsRun)
		{
			// init
			m_BkupSizeEstm.DevType	=	A_GroupType;
			m_BkupSizeEstm.ChMap		=	A_ChMap;
			m_BkupSizeEstm.TimeSt		=	A_TimeSt;
			m_BkupSizeEstm.TimeEd		=	A_TimeEd;
			m_BkupSizeEstm.PrgsDsk	=	0;
			m_BkupSizeEstm.PrgsBdt	=	0;
			m_BkupSizeEstm.Rslt			=	0;
			m_BkupSizeEstm.WorkUnit	=	E_GrDvrFs4MngBkupEstmWorkUnit;

			m_BkupSizeEstm.IsRun		=	TRUE;
			Tv_Result	=	TRUE;
		}
		else
		{
			DbgMsgPrint( "Cls_GrDvrFs4Mng::ImmBkupSizeEstmate - busy.\n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::SafeBkupSizeEstmate(__u8 A_GroupType, __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs4MngMsgBkupSizeEstm	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		// check device type
		if ( E_GrDvrRecDevMirror >= A_GroupType )
		{
			// check channel mask
			if (0 != A_ChMap)
			{
				// check time range
				if ( A_TimeSt <= A_TimeEd )
				{
					// request message
					Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
					Tv_Msg.Base.Cmd				=	E_GrDvrFs4MngMsgBkupSizeEstm;
					Tv_Msg.Group					=	A_GroupType;
					Tv_Msg.ChMap					=	A_ChMap;
					Tv_Msg.TimeSt					=	A_TimeSt;
					Tv_Msg.TimeEd					=	A_TimeEd;
					Tv_Msg.PtrRslt				=	&Tv_Result;

					m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				}
				else
				{
					DbgMsgPrint( "Cls_GrDvrFs4Mng::SafeBkupSizeEstmate - bad time range.\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrDvrFs4Mng::SafeBkupSizeEstmate - channel not selected.\n" );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (LcBgpBkupSizeEstm())
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::LcBgpBkupSizeEstm(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (m_BkupSizeEstm.IsRun)
		{
			m_GrupTbl[m_BkupSizeEstm.DevType]->BkupSizeEstmProc(&m_BkupSizeEstm);
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::ImmAtDelTimeSet( __u32 A_Sec )
{
		m_AtDelTime	=	A_Sec;
		// update auto delete time
		LcAtDelLimUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::SafeAtDelTimeSet( __u32 A_Sec )
{
	// local -------------------
		St_GrDvrFs4MngMsgAtDelTimeSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs4MngMsgAtDelTimeSet;
		Tv_Msg.Sec						=	A_Sec;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs4Mng::AutoDelTimeGet( void )
{
		return	m_AtDelTime;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::LcAtDelLimUpdt( void )
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
void	Cls_GrDvrFs4Mng::ImmGroupFormat( __u8 A_GroupType, BOOL8 A_IsWithLog)
{
	// local -------------------
	// code --------------------
		if(E_GrDvrFs4MngGroupCnt > A_GroupType)
		{
			if(NULL != m_GrupTbl[A_GroupType])
			{
				m_GrupTbl[A_GroupType]->GroupFormat(A_IsWithLog);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::SafeGroupFormat( __u8 A_GroupType, BOOL8 A_IsWithLog)
{
	// local -------------------
		St_GrDvrFs4MngMsgGroupFormat	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs4MngMsgGroupFormat;
		Tv_Msg.GroupType				=	A_GroupType;
		Tv_Msg.IsWithLog				=	A_IsWithLog;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs4Mng::RtlDvrIdGet(void)
{
		return	m_DvrId;
}
//--------------------------------------------------------------------
void*	Cls_GrDvrFs4Mng::RtlTmpBufAlloc(__u32 A_Size)
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if (E_GrDvrFs4MngTmpBufSize >= (m_TmpUsed + A_Size))
		{
			Tv_Result	=	(void*)((__u32)m_TmpBuf + m_TmpUsed);
			m_TmpUsed	=	m_TmpUsed + A_Size;
		}
		else
		{
			DbgMsgPrint("Cls_GrDvrFs4Mng::RtlTmpBufAlloc - not enough TMP buffer.\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs4Mng::RtlTmpBufFree(__u32 A_Size)
{
	// local -------------------
	// code --------------------
		if (m_TmpUsed < A_Size)
		{
			m_TmpUsed	=	0;
			DbgMsgPrint("Cls_GrDvrFs4Mng::RtlTmpBufFree - bad size required.\n");
		}
		else
		{
			m_TmpUsed	=	m_TmpUsed + A_Size;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::VdoAdd(void* A_PtrStrm, __u32 A_StrmSize, Def_GrTime A_TimeSec, __u32 A_TimeMili, BOOL8 A_IsSm,
	__u8 A_Ch, __u8 A_Type,
	__u8 A_Ratb, __u32 A_VdoFmt, __u8 A_SeqId, __u8 A_SigFps, __u8 A_SkipCnt, __u8 A_RefType, __u32 A_WtMark)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_CchFrmIdx;
		Ptr_GrDvrFrmVdo		Tv_PtrFrmHd;
		Ptr_GrDvrFrmTail	Tv_PtrFrmTl;
	// code --------------------
		// init
		Tv_Result	=	TRUE;
		// add to cache
		Tv_CchFrmIdx	=	m_RecReqTbl[m_RecReqIdx].FrmCnt;

		// build frame header
		Tv_PtrFrmHd	=	(Ptr_GrDvrFrmVdo)&m_RecReqTbl[m_RecReqIdx].FrmTbl[Tv_CchFrmIdx].FrmHd;
		Tv_PtrFrmHd->Base.Fcc				=	E_GrDvrFrmHeadFcc;
		Tv_PtrFrmHd->Base.Type				=	A_Type;
		Tv_PtrFrmHd->Base.Ch					=	A_Ch;
		Tv_PtrFrmHd->Base.IsSm				=	A_IsSm;
		Tv_PtrFrmHd->Base.Atb				=	A_Ratb;
		Tv_PtrFrmHd->Base.TimeSec		=	A_TimeSec;
		Tv_PtrFrmHd->Base.TimeMsec		=	A_TimeMili;
		Tv_PtrFrmHd->Base.DataSize		=	A_StrmSize;
		Tv_PtrFrmHd->SeqId						=	A_SeqId;
		Tv_PtrFrmHd->SigFps					=	A_SigFps;
		Tv_PtrFrmHd->SkipCnt					=	A_SkipCnt;
		Tv_PtrFrmHd->RefType					=	A_RefType;
		Tv_PtrFrmHd->VdoFmt					=	A_VdoFmt;
		Tv_PtrFrmHd->Wtmk						=	A_WtMark;

		// stream pointer
		m_RecReqTbl[m_RecReqIdx].FrmTbl[Tv_CchFrmIdx].PtrStrm	=	A_PtrStrm;

		// record data size
		m_RecReqTbl[m_RecReqIdx].FrmTbl[Tv_CchFrmIdx].RecSize	=	(A_StrmSize + sizeof(St_GrDvrFrmUnknown) + sizeof(St_GrDvrFrmTail)+ (E_GrDvrFrmAlignSize - 1)) & E_GrDvrFrmAlignMask;

		// make tail
		Tv_PtrFrmTl	=	(Ptr_GrDvrFrmTail)((__u32)A_PtrStrm + m_RecReqTbl[m_RecReqIdx].FrmTbl[Tv_CchFrmIdx].RecSize - sizeof(St_GrDvrFrmTail));
		Tv_PtrFrmTl->Fcc		=	E_GrDvrFrmTailFcc;
		Tv_PtrFrmTl->Size	=	m_RecReqTbl[m_RecReqIdx].FrmTbl[Tv_CchFrmIdx].RecSize;

		// insert
		m_RecReqTbl[m_RecReqIdx].FrmCnt ++;

		// check need flush
		if (E_GrDvrFs4RecReqFrmCnt <= m_RecReqTbl[m_RecReqIdx].FrmCnt)
		{
			// check full
			LcRecReqNext();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::LcRecReqNext(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_SlotIdx;
	// code --------------------
		Tv_Result	=	TRUE;

		// check full
		if ((m_RecReqCnt + 1) >= E_GrDvrFs4MngRecReqSlotCnt)
		{
			// do flush
			Tv_SlotIdx	=	m_RecReqIdx - m_RecReqCnt;
			if (m_RecReqIdx < m_RecReqCnt)
			{
				Tv_SlotIdx	=	E_GrDvrFs4MngRecReqSlotCnt + m_RecReqIdx - m_RecReqCnt;
			}
			Tv_Result		=	LcRecReqFlush(Tv_SlotIdx);
			m_RecReqCnt --;
		}
		// next
		m_RecReqCnt ++;
		m_RecReqIdx ++;
		if (E_GrDvrFs4MngRecReqSlotCnt <= m_RecReqIdx)
		{
			m_RecReqIdx	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::LcRecReqFlush(__u8 A_SlotIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	TRUE;

		if (0 != m_RecReqTbl[A_SlotIdx].FrmCnt)
		{
			// send to group
			if (NULL != m_GrupTbl[E_GrDvrRecDevNoraml])
			{
				Tv_Result	=	m_GrupTbl[E_GrDvrRecDevNoraml]->RecAddData(&m_RecReqTbl[A_SlotIdx]);
			}

			if (NULL != m_GrupTbl[E_GrDvrRecDevMirror])
			{
				m_GrupTbl[E_GrDvrRecDevMirror]->RecAddData(&m_RecReqTbl[A_SlotIdx]);
			}

			// release buffer
			for (Tv_WkIdx=0; Tv_WkIdx < m_RecReqTbl[A_SlotIdx].FrmCnt;Tv_WkIdx++)
			{
				GrBufTsMapFree(m_PtrBufStrm, m_RecReqTbl[A_SlotIdx].FrmTbl[Tv_WkIdx].PtrStrm, m_BufOwnerMap, (__u16)m_BufThrdIdx);
			}
			// reset
			m_RecReqTbl[A_SlotIdx].FrmCnt	=	0;
			m_RecReqTbl[A_SlotIdx].PreMap	=	0;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::ImmReadTimeFw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4RsltParamBad;
		//check device type
		if (E_GrDvrRecDevMirror >= A_DevType)
		{
			if (NULL != m_GrupTbl[A_DevType])
			{
				Tv_Result	=	m_GrupTbl[A_DevType]->ReadTimeFw(A_PtrCtrl, A_Time);
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::SafeReadTimeFw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs4MngMsgReadTimePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs4MngMsgReadTimeFw;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.Time						=	A_Time;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::ImmReadBw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4RsltParamBad;
		//check device type
		if (E_GrDvrRecDevMirror >= A_DevType)
		{
			if (NULL != m_GrupTbl[A_DevType])
			{
				Tv_Result	=	m_GrupTbl[A_DevType]->ReadBw(A_PtrCtrl, A_PtrCtrl->BufTbl[A_PtrCtrl->BufIdx].Prgs);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::ImmReadTimeBw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrFs4RsltParamBad;
		//check device type
		if (E_GrDvrRecDevMirror >= A_DevType)
		{
			if (NULL != m_GrupTbl[A_DevType])
			{
				Tv_Result	=	m_GrupTbl[A_DevType]->ReadTimeBw(A_PtrCtrl, A_Time);
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs4Mng::SafeReadTimeBw(__u8 A_DevType, Ptr_GrDvrFs4SeekCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs4MngMsgReadTimePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs4MngMsgReadTimeBw;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.Time						=	A_Time;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs4Mng::AdoAdd(void* A_PtrStrm, __u32 A_StrmSize, Def_GrTime A_TimeSec, __u32 A_TimeMili, BOOL8 A_IsSm,
	__u8 A_Ch, __u8 A_Type,
	__u8 A_Ratb)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_CchFrmIdx;
		Ptr_GrDvrFrmAdo		Tv_PtrFrmHd;
		Ptr_GrDvrFrmTail	Tv_PtrFrmTl;
	// code --------------------
		// init
		Tv_Result	=	TRUE;
		// add to cache
		Tv_CchFrmIdx	=	m_RecReqTbl[m_RecReqIdx].FrmCnt;

		// build frame header
		Tv_PtrFrmHd	=	(Ptr_GrDvrFrmAdo)&m_RecReqTbl[m_RecReqIdx].FrmTbl[Tv_CchFrmIdx].FrmHd;
		Tv_PtrFrmHd->Base.Fcc				=	E_GrDvrFrmHeadFcc;
		Tv_PtrFrmHd->Base.Type				=	A_Type;
		Tv_PtrFrmHd->Base.Ch					=	A_Ch;
		Tv_PtrFrmHd->Base.IsSm				=	A_IsSm;
		Tv_PtrFrmHd->Base.Atb				=	A_Ratb;
		Tv_PtrFrmHd->Base.TimeSec		=	A_TimeSec;
		Tv_PtrFrmHd->Base.TimeMsec		=	A_TimeMili;
		Tv_PtrFrmHd->Base.DataSize		=	A_StrmSize;

		// stream pointer
		m_RecReqTbl[m_RecReqIdx].FrmTbl[Tv_CchFrmIdx].PtrStrm	=	A_PtrStrm;

		// record data size
		m_RecReqTbl[m_RecReqIdx].FrmTbl[Tv_CchFrmIdx].RecSize	=	(A_StrmSize + sizeof(St_GrDvrFrmTail)+ (E_GrDvrFrmAlignSize - 1)) & E_GrDvrFrmAlignMask;

		// make tail
		Tv_PtrFrmTl	=	(Ptr_GrDvrFrmTail)((__u32)A_PtrStrm + m_RecReqTbl[m_RecReqIdx].FrmTbl[Tv_CchFrmIdx].RecSize - sizeof(St_GrDvrFrmTail));
		Tv_PtrFrmTl->Fcc		=	E_GrDvrFrmTailFcc;
		Tv_PtrFrmTl->Size	=	m_RecReqTbl[m_RecReqIdx].FrmTbl[Tv_CchFrmIdx].RecSize;

		// insert
		m_RecReqTbl[m_RecReqIdx].FrmCnt ++;

		// check need flush
		if (E_GrDvrFs4RecReqFrmCnt <= m_RecReqTbl[m_RecReqIdx].FrmCnt)
		{
			// check full
			Tv_Result	=	LcRecReqNext();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

