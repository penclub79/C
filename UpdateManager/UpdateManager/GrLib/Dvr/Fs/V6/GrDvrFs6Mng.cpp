/*
 GAUSE DVR file system version 2 manager

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>
#include <GrError.h>

#include <Dvr/Fs/V6/GrDvrFs6Mng.h>
#include <Dvr/Fs/V6/GrDvrFs6Dmux.h>

#include <stdlib.h>

//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

extern	St_GrDvrFs6Env	V_GrDvrFs6Env;

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs6Mng::Cls_GrDvrFs6Mng(Cls_GrTaskSvr* A_TaskSvr) :
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		//init
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		m_IsInited	=	FALSE;

		m_RflushTimer	=	0;

		m_TempTimer		=	0;

		m_CchTimer		=	0;

		m_AtDelTime		=	0;
		m_AtDelBaseTm	=	0;

		GrDumyZeroMem( m_GrupTbl, sizeof(m_GrupTbl) );
		GrDumyZeroMem( m_TempTbl, sizeof(m_TempTbl) );

		//register task
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrDvrFs6MngTimerPeriod, &m_HndlTimer );
		m_TaskSvr->RegistBkgClient( this );

}
//--------------------------------------------------------------------
Cls_GrDvrFs6Mng::~Cls_GrDvrFs6Mng()
{
		// request finish
		ReqFinish();

		//unregister timer
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistMsgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );


		GrTaskSvrMsgCmdFree( m_MsgAloc );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check input message
		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			Tv_Result	=	TRUE;
			switch ( ((Ptr_GrDvrFs6MngMsgBase)A_PtrMsg)->Cmd )
			{
				case E_GrDvrFs6MngMsgInit:
					ImmInit();
					break;
				case E_GrDvrFs6MngMsgFinish:
					ImmFinish();
					break;
				case E_GrDvrFs6MngMsgSetup:
					ImmSetup( ((Ptr_GrDvrFs6MngMsgSetup)A_PtrMsg)->NrmMap, ((Ptr_GrDvrFs6MngMsgSetup)A_PtrMsg)->MirMap,
						((Ptr_GrDvrFs6MngMsgSetup)A_PtrMsg)->IsNrmOvwt, ((Ptr_GrDvrFs6MngMsgSetup)A_PtrMsg)->IsMirOvwt );
					break;
				case E_GrDvrFs6MngMsgFormat:
					if(NULL != ((Ptr_GrDvrFs6MngMsgFormat)A_PtrMsg)->PtrRslt)
					{
						*((Ptr_GrDvrFs6MngMsgFormat)A_PtrMsg)->PtrRslt	=
							ImmDiskFormat(((Ptr_GrDvrFs6MngMsgFormat)A_PtrMsg)->DevIdx,FALSE);
					}
					else
					{
						// post mode
						ImmDiskFormat(((Ptr_GrDvrFs6MngMsgFormat)A_PtrMsg)->DevIdx,TRUE);
					}
					break;
				case E_GrDvrFs6MngMsgDiskStatGet:
					*((Ptr_GrDvrFs6MngMsgDiskStatGet)A_PtrMsg)->PtrRslt	=	
						ImmDiskGetStat( ((Ptr_GrDvrFs6MngMsgDiskStatGet)A_PtrMsg)->DevIdx );
					break;
				case E_GrDvrFs6MngMsgDiskRecInfoGet:
					*((Ptr_GrDvrFs6MngMsgDiskRecInfoGet)A_PtrMsg)->PtrRslt	=	
						ImmDiskRecInfoGet(((Ptr_GrDvrFs6MngMsgDiskRecInfoGet)A_PtrMsg)->DevIdx, ((Ptr_GrDvrFs6MngMsgDiskRecInfoGet)A_PtrMsg)->PtrRtTotal,
						((Ptr_GrDvrFs6MngMsgDiskRecInfoGet)A_PtrMsg)->PtrRtRec);
					break;
				case E_GrDvrFs6MngMsgGroupStatGet:
					*((Ptr_GrDvrFs6MngMsgGroupStatGet)A_PtrMsg)->PtrRslt	=
						ImmGroupGetStat( ((Ptr_GrDvrFs6MngMsgGroupStatGet)A_PtrMsg)->GroupType );
					break;
				case E_GrDvrFs6MngMsgGroupRecInfoGet:
					*((Ptr_GrDvrFs6MngMsgGroupRecInfoGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupRecInfoGet(((Ptr_GrDvrFs6MngMsgGroupRecInfoGet)A_PtrMsg)->GroupType, ((Ptr_GrDvrFs6MngMsgGroupRecInfoGet)A_PtrMsg)->PtrRtTotal,
						((Ptr_GrDvrFs6MngMsgGroupRecInfoGet)A_PtrMsg)->PtrRtRec);
					break;
				case E_GrDvrFs6MngMsgGroupDayMapGet:
					*((Ptr_GrDvrFs6MngMsgGroupDayMapGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupGetMonthDayMap( ((Ptr_GrDvrFs6MngMsgGroupDayMapGet)A_PtrMsg)->Group, ((Ptr_GrDvrFs6MngMsgGroupDayMapGet)A_PtrMsg)->TimeMonth);
					break;
				case E_GrDvrFs6MngMsgGroupMinRatbTblGet:
					*((Ptr_GrDvrFs6MngMsgGroupMinRatbTblGet)A_PtrMsg)->PtrRslt	=
						ImmGroupGetMinRatbTbl(((Ptr_GrDvrFs6MngMsgGroupMinRatbTblGet)A_PtrMsg)->Group, ((Ptr_GrDvrFs6MngMsgGroupMinRatbTblGet)A_PtrMsg)->TimeSt,
						((Ptr_GrDvrFs6MngMsgGroupMinRatbTblGet)A_PtrMsg)->ItemCnt, ((Ptr_GrDvrFs6MngMsgGroupMinRatbTblGet)A_PtrMsg)->PtrBuf);
					break;
				case E_GrDvrFs6MngMsgReadFwNext:
					*((Ptr_GrDvrFs6MngMsgReadPara)A_PtrMsg)->PtrRslt		=	
						ImmReadFwNext(((Ptr_GrDvrFs6MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs6MngMsgReadPara)A_PtrMsg)->PtrCtrl);
					break;
				case E_GrDvrFs6MngMsgReadBwNext:
					*((Ptr_GrDvrFs6MngMsgReadPara)A_PtrMsg)->PtrRslt		=
						ImmReadBwNext(((Ptr_GrDvrFs6MngMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrDvrFs6MngMsgReadPara)A_PtrMsg)->PtrCtrl);
					break;
				case E_GrDvrFs6MngMsgReadFwByTime:
					*((Ptr_GrDvrFs6MngMsgReadTimePara)A_PtrMsg)->PtrRslt		=
						ImmReadFwGoByTime(((Ptr_GrDvrFs6MngMsgReadTimePara)A_PtrMsg)->Group, ((Ptr_GrDvrFs6MngMsgReadTimePara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrDvrFs6MngMsgReadTimePara)A_PtrMsg)->Time);
					break;
				case E_GrDvrFs6MngMsgReadBwByTime:
					*((Ptr_GrDvrFs6MngMsgReadTimePara)A_PtrMsg)->PtrRslt		=
						ImmReadBwGoByTime(((Ptr_GrDvrFs6MngMsgReadTimePara)A_PtrMsg)->Group, ((Ptr_GrDvrFs6MngMsgReadTimePara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrDvrFs6MngMsgReadTimePara)A_PtrMsg)->Time);
					break;
				case E_GrDvrFs6MngMsgSysInfo:
					ImmSysGetInfo( ((Ptr_GrDvrFs6MngMsgSysInfo)A_PtrMsg)->PtrInfo );
					break;
				case E_GrDvrFs6MngMsgGroupTimeRngGet:
					*((Ptr_GrDvrFs6MngMsgGroupTimeRngGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupGetRecTimeRng( ((Ptr_GrDvrFs6MngMsgGroupTimeRngGet)A_PtrMsg)->Group, ((Ptr_GrDvrFs6MngMsgGroupTimeRngGet)A_PtrMsg)->PtrRtTimeSt,
						((Ptr_GrDvrFs6MngMsgGroupTimeRngGet)A_PtrMsg)->PtrRtTimeEd );
					break;
				case E_GrDvrFs6MngMsgLogReadOpen:
					*((Ptr_GrDvrFs6MngMsgLogRead)A_PtrMsg)->PtrRslt	=
						ImmLogReadOpen(((Ptr_GrDvrFs6MngMsgLogRead)A_PtrMsg)->DevType, ((Ptr_GrDvrFs6MngMsgLogRead)A_PtrMsg)->PtrCtrl);
					break;
				case E_GrDvrFs6MngMsgLogReadNext:
					*((Ptr_GrDvrFs6MngMsgLogRead)A_PtrMsg)->PtrRslt	=
						ImmLogReadNext(((Ptr_GrDvrFs6MngMsgLogRead)A_PtrMsg)->DevType, ((Ptr_GrDvrFs6MngMsgLogRead)A_PtrMsg)->PtrCtrl);
					break;
				case E_GrDvrFs6MngMsgRecSizeEstm:
					*(((Ptr_GrDvrFs6MngMsgRecSizeEstm)A_PtrMsg)->PtrRslt)		=	
						ImmRecSizeEstmate(((Ptr_GrDvrFs6MngMsgRecSizeEstm)A_PtrMsg)->Group, ((Ptr_GrDvrFs6MngMsgRecSizeEstm)A_PtrMsg)->ChMap,
						((Ptr_GrDvrFs6MngMsgRecSizeEstm)A_PtrMsg)->TimeSt, ((Ptr_GrDvrFs6MngMsgRecSizeEstm)A_PtrMsg)->TimeEd);
					break;
				case E_GrDvrFs6MngMsgLogAdd:
					ImmLogAdd( &((Ptr_GrDvrFs6MngMsgLogAdd)A_PtrMsg)->Log ) ;
					break;
				case E_GrDvrFs6MngMsgAtDelTimeSet:
					ImmAtDelTimeSet( ((Ptr_GrDvrFs6MngMsgAtDelTimeSet)A_PtrMsg)->Sec );
					break;
				case E_GrDvrFs6MngMsgGroupFormat:
					ImmGroupFormat(((Ptr_GrDvrFs6MngMsgGroupFormat)A_PtrMsg)->GroupType);
					break;
				case E_GrDvrFs6MngMsgDiskModelGet:
					*((Ptr_GrDvrFs6MngMsgDiskModelGet)A_PtrMsg)->PtrRslt		=	
						ImmDiskModelNameGet(((Ptr_GrDvrFs6MngMsgDiskModelGet)A_PtrMsg)->DevIdx, ((Ptr_GrDvrFs6MngMsgDiskModelGet)A_PtrMsg)->StrRt);
					break;
				case  E_GrDvrFs6MngMsgGroupDataReadHndl:
					*((Ptr_GrDvrFs6MngMsgGroupDataReadHndl)A_PtrMsg)->PtrRslt		=
						ImmGroupDataReadByHndl(((Ptr_GrDvrFs6MngMsgGroupDataReadHndl)A_PtrMsg)->GroupType,
						((Ptr_GrDvrFs6MngMsgGroupDataReadHndl)A_PtrMsg)->Hndl,
						((Ptr_GrDvrFs6MngMsgGroupDataReadHndl)A_PtrMsg)->PtrBuf,
						((Ptr_GrDvrFs6MngMsgGroupDataReadHndl)A_PtrMsg)->Size);
					break;
				case E_GrDvrFs6MngMsgDmuxNew:
					*((Ptr_GrDvrFs6MngMsgDmuxNew)A_PtrMsg)->PtrRslt		=
						ImmDmuxNew(((Ptr_GrDvrFs6MngMsgDmuxNew)A_PtrMsg)->MaxChCnt);
					break;
				case E_GrDvrFs6MngMsgDmuxDelete:
					*((Ptr_GrDvrFs6MngMsgDmuxDelte)A_PtrMsg)->PtrRslt		=
						ImmDmuxDelete(((Ptr_GrDvrFs6MngMsgDmuxDelte)A_PtrMsg)->Hndl);
					break;
				case E_GrDvrFs6MngMsgDmuxGroupSet:
					ImmDmuxRecGroupSet(((Ptr_GrDvrFs6MngMsgDmuxGroupSet)A_PtrMsg)->Hndl, ((Ptr_GrDvrFs6MngMsgDmuxGroupSet)A_PtrMsg)->RecGrpType);
					break;
				case E_GrDvrFs6MngMsgDmuxReadCtrlPtrGet:
					*((Ptr_GrDvrFs6MngMsgDmuxReadCtrlPtrGet)A_PtrMsg)->PtrRslt		=
						ImmDmuxReadCtrlPtrGet(((Ptr_GrDvrFs6MngMsgDmuxReadCtrlPtrGet)A_PtrMsg)->Hndl,
						((Ptr_GrDvrFs6MngMsgDmuxReadCtrlPtrGet)A_PtrMsg)->Ch);
					break;
				case E_GrDvrFs6MngMsgDmuxDo:
					*((Ptr_GrDvrFs6MngMsgDmuxDo)A_PtrMsg)->PtrRslt		=
						ImmDmuxDo(((Ptr_GrDvrFs6MngMsgDmuxDo)A_PtrMsg)->Hndl);
					break;
				case E_GrDvrFs6MngMsgDmuxRdyFrmRead:
					*((Ptr_GrDvrFs6MngMsgDmuxRdyFrmRead)A_PtrMsg)->PtrRslt		=
						ImmDmuxReadyFrameRead(((Ptr_GrDvrFs6MngMsgDmuxRdyFrmRead)A_PtrMsg)->Hndl,
						((Ptr_GrDvrFs6MngMsgDmuxRdyFrmRead)A_PtrMsg)->PtrBufTbl,
						((Ptr_GrDvrFs6MngMsgDmuxRdyFrmRead)A_PtrMsg)->BufCnt);
					break;
				case E_GrDvrFs6MngMsgDmuxRdyFrmDone:
					ImmDmuxReadyFrameDone(((Ptr_GrDvrFs6MngMsgDmuxRdyFrmDone)A_PtrMsg)->Hndl, ((Ptr_GrDvrFs6MngMsgDmuxRdyFrmDone)A_PtrMsg)->Map);
					break;
				case E_GrDvrFs6MngMsgDmuxGoTime:
					ImmDmuxGoTime(((Ptr_GrDvrFs6MngMsgDmuxGoTime)A_PtrMsg)->Hndl, ((Ptr_GrDvrFs6MngMsgDmuxGoTime)A_PtrMsg)->Time);
					break;
				case E_GrDvrFs6MngMsgDmuxDirSet:
					ImmDmuxDirSet(((Ptr_GrDvrFs6MngMsgDmuxDirSet)A_PtrMsg)->Hndl, ((Ptr_GrDvrFs6MngMsgDmuxDirSet)A_PtrMsg)->IsDirBack);
					break;
				case E_GrDvrFs6MngMsgDmuxVdoChAbleSet:
					ImmDmuxVdoChAbleSet(((Ptr_GrDvrFs6MngMsgDmuxVdoChAbleSet)A_PtrMsg)->Hndl,
						((Ptr_GrDvrFs6MngMsgDmuxVdoChAbleSet)A_PtrMsg)->Map);
					break;
				case E_GrDvrFs6MngMsgDmuxAdoChAbleSet:
					ImmDmuxAdoChAbleSet(((Ptr_GrDvrFs6MngMsgDmuxAdoChAbleSet)A_PtrMsg)->Hndl,
						((Ptr_GrDvrFs6MngMsgDmuxAdoChAbleSet)A_PtrMsg)->Map);
					break;
				case E_GrDvrFs6MngMsgDmuxVdoFrmMaskSet:
					ImmDmuxVdoFrmMaskSet(((Ptr_GrDvrFs6MngMsgDmuxVdoFrmMaskSet)A_PtrMsg)->Hndl,
						((Ptr_GrDvrFs6MngMsgDmuxVdoFrmMaskSet)A_PtrMsg)->Ch, ((Ptr_GrDvrFs6MngMsgDmuxVdoFrmMaskSet)A_PtrMsg)->Mask);
					break;

				default:
					Tv_Result	=	FALSE;
					break;
			}
			
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::RtlTimer( __u32 A_Hndl )
{
		LcTempUpdt();

		LcTmrRecFlush();

		// filesystem tick
		V_GrDvrFs6Env.TickFs ++;
}
//--------------------------------------------------------------------
Cls_GrDvrFs6Disk*	Cls_GrDvrFs6Mng::RtlDiskObjGet( __u8 A_Idx )
{
	// local -------------------
		Cls_GrDvrFs6Disk*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
#if 0
		if ( E_GrDvrFs6MaxDriveCnt > A_Idx )
		{
			Tv_Result	=	m_DiskTbl[A_Idx];
		}
#endif


		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ReqSetup(__u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir)
{
	// local -------------------
		St_GrDvrFs6MngMsgSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgSetup;
		Tv_Msg.NrmMap						=	A_NormalMap;
		Tv_Msg.IsNrmOvwt				=	A_IsOvwtNrm;
		Tv_Msg.MirMap						=	A_MirrorMap;
		Tv_Msg.IsMirOvwt				=	A_IsOvwtMir;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::LcGroupBuild( void )
{
	// local -------------------
		__u8	Tv_GrIdx;
	// code --------------------
		// release group
		for ( Tv_GrIdx=0;Tv_GrIdx < E_GrDvrFs6MngGroupCnt;Tv_GrIdx++ )
		{
			if(NULL != m_GrupTbl[Tv_GrIdx].Obj)
			{
				delete	m_GrupTbl[Tv_GrIdx].Obj;
				m_GrupTbl[Tv_GrIdx].Obj	=	NULL;
			}
		}

		// create normal group
		for(Tv_GrIdx=0; Tv_GrIdx < E_GrDvrFs6MngGroupCnt; Tv_GrIdx++)
		{
			if(0 != m_GrupTbl[Tv_GrIdx].DskMap)
			{
				m_GrupTbl[Tv_GrIdx].Obj	=	(Cls_GrDvrFs6Grp*) new Cls_GrDvrFs6Grp(Tv_GrIdx, 
					m_GrupTbl[Tv_GrIdx].DskMap, m_GrupTbl[Tv_GrIdx].IsOvwt);
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir )
{
	// local -------------------
		St_GrTaskMsgHd	Tv_Msg;
	// code --------------------
		// update group info
		m_GrupTbl[E_GrDvrRecDevNoraml].DskMap	=	A_NormalMap;
		m_GrupTbl[E_GrDvrRecDevNoraml].IsOvwt	=	A_IsOvwtNrm;

		m_GrupTbl[E_GrDvrRecDevMirror].DskMap	=	A_MirrorMap;
		m_GrupTbl[E_GrDvrRecDevMirror].IsOvwt	=	A_IsOvwtMir;

		LcGroupBuild();

		LcGroupInfoUpdate();

		// file system setup
		Tv_Msg.Cmd		=	E_GrTaskMsgFsSetup;
		GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ReqInit(void)
{
	// local -------------------
		St_GrDvrFs6MngMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Cmd			=	E_GrDvrFs6MngMsgInit;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ReqFinish(void)
{
	// local -------------------
		St_GrDvrFs6MngMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Cmd			=	E_GrDvrFs6MngMsgFinish;
		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmInit( void )
{
	// local -------------------
		__u32	Tv_DskIdx;
	// code --------------------
		// build disk list
		for(Tv_DskIdx=0; Tv_DskIdx < V_GrDvrFs6Env.DiskMaxCnt; Tv_DskIdx++)
		{
			V_GrDvrFs6Env.DskTbl[Tv_DskIdx]	=	(void*) new Cls_GrDvrFs6Disk(Tv_DskIdx);
			// check not exist disk
			if(E_GrDvrRecStatNotexist == ((Cls_GrDvrFs6Disk*)V_GrDvrFs6Env.DskTbl[Tv_DskIdx])->DiskStatGet())
			{
				delete	(Cls_GrDvrFs6Disk*)V_GrDvrFs6Env.DskTbl[Tv_DskIdx];
				V_GrDvrFs6Env.DskTbl[Tv_DskIdx]	=	NULL;
			}
		}
		// temperature update
		m_TempTimer	=	E_GrDvrFs6MngTempScanPeriod;
		LcTempUpdt();

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmFinish(void)
{
	// local -------------------
		__u32	Tv_DskIdx;
		__u32	Tv_GrpIdx;
		__u32	Tv_DmxIdx;
	// code --------------------
		// release dmux
		for(Tv_DmxIdx = 0; Tv_DmxIdx < E_GrDvrFs6MaxDmuxCnt; Tv_DmxIdx++)
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[Tv_DmxIdx].Obj)
			{
				delete	(Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[Tv_DmxIdx].Obj;
				V_GrDvrFs6Env.DmuxTbl[Tv_DmxIdx].Obj	=	NULL;
			}
		}

		// release group
		for(Tv_GrpIdx = 0; Tv_GrpIdx < E_GrDvrFs6MngGroupCnt; Tv_GrpIdx++)
		{
			if(NULL != m_GrupTbl[Tv_GrpIdx].Obj)
			{
				delete	m_GrupTbl[Tv_GrpIdx].Obj;
				m_GrupTbl[Tv_GrpIdx].Obj	=	NULL;
			}
		}

		// release disk
		for(Tv_DskIdx=0; Tv_DskIdx < E_GrDvrFs6MaxDiskCnt; Tv_DskIdx++)
		{
			if(NULL != V_GrDvrFs6Env.DskTbl[Tv_DskIdx])
			{
				delete	(Cls_GrDvrFs6Disk*)V_GrDvrFs6Env.DskTbl[Tv_DskIdx];
				V_GrDvrFs6Env.DskTbl[Tv_DskIdx]	=	NULL;
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ReqDiskFormat(__u8 A_DevIdx, BOOL8 A_IsPost)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs6MngMsgFormat	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgFormat;
		Tv_Msg.DevIdx						=	A_DevIdx;

		if( A_IsPost)
		{
			Tv_Msg.PtrRslt					=	NULL;
			Tv_Result	=	m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		}
		else
		{
			Tv_Msg.PtrRslt					=	&Tv_Result;
			m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ImmDiskFormat(__u8 A_DevIdx, BOOL8 A_IsPostPrgs)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_GrpType;
		__s32	Tv_GrpIdx;
		St_GrTaskMsgDiskFmtCpl	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		// check valid device index
		if ( E_GrDvrFs6MaxDiskCnt > A_DevIdx )
		{
			// check exist disk
			if(NULL != V_GrDvrFs6Env.DskTbl[A_DevIdx])
			{
				// check group
				Tv_GrpIdx		=	-1;
				Tv_GrpType	=	0;
				if(0 != (m_GrupTbl[E_GrDvrRecDevNoraml].DskMap & (1 << A_DevIdx)))
				{
					Tv_GrpIdx	=	0;
				}
				if(0 != (m_GrupTbl[E_GrDvrRecDevMirror].DskMap & (1 << A_DevIdx)))
				{
					Tv_GrpIdx	=	1;
					Tv_GrpType ++;
				}
				// stop group
				if(0 <= Tv_GrpIdx)
				{
					delete	m_GrupTbl[Tv_GrpIdx].Obj;
					m_GrupTbl[Tv_GrpIdx].Obj	=	NULL;
				}

				// do format
				Tv_Result	=	((Cls_GrDvrFs6Disk*)V_GrDvrFs6Env.DskTbl[A_DevIdx])->Format(Tv_GrpType, 0, 0, A_IsPostPrgs);

				// regroup
				if(0 <= Tv_GrpIdx)
				{
					m_GrupTbl[Tv_GrpIdx].Obj	=	(Cls_GrDvrFs6Grp*) new Cls_GrDvrFs6Grp((__u8)Tv_GrpIdx, m_GrupTbl[Tv_GrpIdx].DskMap,
						m_GrupTbl[Tv_GrpIdx].IsOvwt);
				}
			}
		}
		
		LcGroupInfoUpdate();

		// notify format completed
		Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgDiskFormatCpl;
		Tv_Msg.DiskIdx		=	A_DevIdx;
		Tv_Msg.IsSuccess	=	Tv_Result;
		GrTaskSvrMsgBroadCast( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::SafeDiskGetStat(__u8 A_DevIdx)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs6MngMsgDiskStatGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDiskStatGet;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ImmDiskGetStat(__u8 A_DevIdx)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		// check valid device index
#if 0
		if ( E_GrDvrFs6MaxDriveCnt > A_DevIdx )
		{
			// check exist disk
			if ( NULL != m_DiskTbl[A_DevIdx] )
			{
				Tv_Result	=	m_DiskTbl[A_DevIdx]->DiskStatGet();
			}
		}
#endif
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ReqDiskRecInfoGet(__u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs6MngMsgDiskRecInfoGet	Tv_Msg;
	// code --------------------
		Tv_Result			=	E_GrDvrRecStatNotexist;
		*A_PtrRtTotal	=	0;
		*A_PtrRtRec		=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDiskRecInfoGet;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.PtrRtTotal				=	A_PtrRtTotal;
		Tv_Msg.PtrRtRec					=	A_PtrRtRec;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ImmDiskRecInfoGet(__u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;

		// check valid device index
		if ( E_GrDvrFs6MaxDiskCnt > A_DevIdx )
		{
			// check exist disk
			if(NULL != V_GrDvrFs6Env.DskTbl[A_DevIdx])
			{
				Tv_Result	=	((Cls_GrDvrFs6Disk*)V_GrDvrFs6Env.DskTbl[A_DevIdx])->DiskInfoGet(A_PtrRtTotal, A_PtrRtRec);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ImmDiskModelNameGet(__u8 A_DevIdx, WCHAR* A_StrRt)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check valid device index
		if(E_GrDvrFs6MaxDiskCnt > A_DevIdx)
		{
			// check exist disk
			if(NULL != V_GrDvrFs6Env.DskTbl[A_DevIdx])
			{
				((Cls_GrDvrFs6Disk*)V_GrDvrFs6Env.DskTbl[A_DevIdx])->ModelNameGet(A_StrRt);
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ReqDiskModelNameGet(__u8 A_DevIdx, WCHAR* A_StrRt)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs6MngMsgDiskModelGet	Tv_Msg;
	// code --------------------
		Tv_Result		=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDiskModelGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.StrRt						=	A_StrRt;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::SafeGroupGetStat(__u8 A_GroupType)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs6MngMsgGroupStatGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgGroupStatGet;
		Tv_Msg.GroupType				=	A_GroupType;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ReqGroupRecInfoGet(__u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
#if 0
		__u8	Tv_Result;
		St_GrDvrFs6MngMsgGroupRecInfoGet	Tv_Msg;
#endif
	// code --------------------
#if 0
		Tv_Result			=	E_GrDvrRecStatNotexist;
		*A_PtrRtTotal	=	0;
		*A_PtrRtRec		=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgGroupRecInfoGet;
		Tv_Msg.GroupType				=	A_GroupType;
		Tv_Msg.PtrRtTotal				=	A_PtrRtTotal;
		Tv_Msg.PtrRtRec					=	A_PtrRtRec;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
#endif
		*A_PtrRtTotal	=	m_GrupTbl[A_GroupType].SizeTot;
		*A_PtrRtRec		=	m_GrupTbl[A_GroupType].SizeRec;
		return	m_GrupTbl[A_GroupType].Stat;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ImmGroupGetStat(__u8 A_GroupType)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		if ( E_GrDvrFs6MngGroupCnt > A_GroupType )
		{
			if(NULL != m_GrupTbl[A_GroupType].Obj)
			{
				Tv_Result	=	m_GrupTbl[A_GroupType].Obj->StatusGet();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ImmGroupRecInfoGet(__u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result			=	E_GrDvrRecStatNotexist;
		*A_PtrRtTotal	=	0;
		*A_PtrRtRec		=	0;
		if ( E_GrDvrFs6MngGroupCnt > A_GroupType )
		{
			if(NULL != m_GrupTbl[A_GroupType].Obj)
			{
				Tv_Result	=	((Cls_GrDvrFs6Grp*)m_GrupTbl[A_GroupType].Obj)->GroupInfoGet(A_PtrRtTotal, A_PtrRtRec);
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs6Mng::ReqGroupGetMonthDayMap(__u8 A_GroupType, Def_GrTime A_TimeMonth)
{
	// local -------------------
		__u32	Tv_Result;
		St_GrDvrFs6MngMsgGroupDayMapGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgGroupDayMapGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.TimeMonth				=	A_TimeMonth;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs6Mng::ImmGroupGetMonthDayMap(__u8 A_GroupType, Def_GrTime A_TimeMonth)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrDvrFs6MngGroupCnt > A_GroupType )
		{
			if(NULL != m_GrupTbl[A_GroupType].Obj)
			{
				Tv_Result	=	m_GrupTbl[A_GroupType].Obj->RecMonthDayMapGet(A_TimeMonth);
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ReqGroupGetMinRatbTbl(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrBuf)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs6MngMsgGroupMinRatbTblGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgGroupMinRatbTblGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.TimeSt						=	A_TimeSt;
		Tv_Msg.ItemCnt					=	A_ItemCnt;
		Tv_Msg.PtrBuf						=	A_PtrBuf;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ImmGroupGetMinRatbTbl(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrRtBuf)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Size;
	// code --------------------
		Tv_Result	=	FALSE;

		if (0 != A_ItemCnt)
		{
			// reset
			Tv_Size	=	sizeof(St_GrDvrRecMinMap)* A_ItemCnt;
			GrDumyZeroMem(A_PtrRtBuf, Tv_Size);

			if (E_GrDvrFs6MngGroupCnt > A_GroupType)
			{
				if(NULL != m_GrupTbl[A_GroupType].Obj)
				{
					Tv_Result	=	m_GrupTbl[A_GroupType].Obj->RecMinRatbTblGet(A_TimeSt, A_ItemCnt, A_PtrRtBuf);
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ImmGroupGetRecTimeRng(__u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		*A_PtrRtTimeSt	=	0;
		*A_PtrRtTimeEd	=	0;
		if ( E_GrDvrFs6MngGroupCnt > A_GroupType )
		{
			if(NULL != m_GrupTbl[A_GroupType].Obj)
			{
				Tv_Result	=	m_GrupTbl[A_GroupType].Obj->RecTimeRangeGet(A_PtrRtTimeSt, A_PtrRtTimeEd);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ReqGroupGetRecTimeRng(__u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs6MngMsgGroupTimeRngGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgGroupTimeRngGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.PtrRtTimeSt			=	A_PtrRtTimeSt;
		Tv_Msg.PtrRtTimeEd			=	A_PtrRtTimeEd;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ImmReadFwNext(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if(NULL != m_GrupTbl[A_DevType].Obj)
			{
				Tv_Result	=	m_GrupTbl[A_DevType].Obj->ReadFwNext(A_PtrCtrl);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ImmReadBwNext(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;
		//check device type
		if(E_GrDvrRecDevMirror >= A_DevType)
		{
			if(NULL != m_GrupTbl[A_DevType].Obj)
			{
				Tv_Result	=	m_GrupTbl[A_DevType].Obj->ReadBwNext(A_PtrCtrl);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ReqReadFwNext(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs6MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs6MngMsgReadFwNext;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		__u8	Tv_DevType;
	// code --------------------
		//check device type
		for ( Tv_DevType=0;Tv_DevType < E_GrDvrFs6MngGroupCnt;Tv_DevType++ )
		{
			if(NULL != m_GrupTbl[Tv_DevType].Obj)
			{
				m_GrupTbl[Tv_DevType].Obj->LogAdd(A_PtrItem);
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ImmLogReadOpen(__u8 A_DevType, Ptr_GrDvrFs6LogCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		//check device type
		if ( E_GrDvrFs6MngGroupCnt > A_DevType )
		{
			if(NULL != m_GrupTbl[A_DevType].Obj)
			{
				Tv_Result	=	m_GrupTbl[A_DevType].Obj->LogOpen(A_PtrCtrl);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ImmLogReadNext(__u8 A_DevType, Ptr_GrDvrFs6LogCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;

		//check device type
		if(E_GrDvrFs6MngGroupCnt > A_DevType)
		{
			if(NULL != m_GrupTbl[A_DevType].Obj)
			{
				Tv_Result	=	m_GrupTbl[A_DevType].Obj->LogReadNext(A_PtrCtrl);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ReqLogAdd(Ptr_GrDvrLogItm A_PtrItem)
{
	// local -------------------
		St_GrDvrFs6MngMsgLogAdd	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs6MngMsgLogAdd;
		GrDumyCopyMem( &Tv_Msg.Log, A_PtrItem, sizeof(St_GrDvrLogItm) );
		
#ifdef GR_TASKMSG_DEBUG
		if(!m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg)))
		{
			DbgMsgPrint("[DBG] %s MsgPost fail!\n", __FUNCTION__);
		}
#else
		m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
#endif
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ReqLogReadOpen(__u8 A_DevType, Ptr_GrDvrFs6LogCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs6MngMsgLogRead	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs6MngMsgLogReadOpen;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.DevType				=	A_DevType;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ReqLogReadNext(__u8 A_DevType, Ptr_GrDvrFs6LogCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs6MngMsgLogRead	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs6MngMsgLogReadNext;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.DevType				=	A_DevType;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmSysGetInfo(Ptr_GrDvrFs6MngSysInfo A_PtrRtInfo)
{
	// local -------------------
		//__u8	Tv_DskIdx;
	// code --------------------
#if 0
		GrDumyZeroMem( A_PtrRtInfo, sizeof(St_GrDvrFs6MngSysInfo) );
		for ( Tv_DskIdx=0;Tv_DskIdx < E_GrDvrFs6MaxDriveCnt;Tv_DskIdx++ )
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
#endif
		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::LcTempUpdt( void )
{
	// local -------------------
		__u8	Tv_DskIdx;
	// code --------------------
		m_TempTimer ++;
		if ( E_GrDvrFs6MngTempScanPeriod <= m_TempTimer )
		{
			for(Tv_DskIdx=0; Tv_DskIdx < E_GrDvrFs6MaxDiskCnt; Tv_DskIdx++)
			{
				if(NULL != V_GrDvrFs6Env.DskTbl[Tv_DskIdx])
				{
					m_TempTbl[Tv_DskIdx]	=	((Cls_GrDvrFs6Disk*)V_GrDvrFs6Env.DskTbl[Tv_DskIdx])->TempratureRead();
				}
				else
				{
					m_TempTbl[Tv_DskIdx]	=	0;
				}
			}
			m_TempTimer	=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::SafeSysGetInfo(Ptr_GrDvrFs6MngSysInfo A_PtrRtInfo)
{
	// local -------------------
		St_GrDvrFs6MngMsgSysInfo	Tv_Msg;
	// code --------------------
		GrDumyZeroMem( A_PtrRtInfo, sizeof(St_GrDvrFs6MngSysInfo) );

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgSysInfo;
		Tv_Msg.PtrInfo					=	A_PtrRtInfo;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::DiskTemperatureGet(__u8 A_DskIdx)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if(E_GrDvrFs6MaxDiskCnt > A_DskIdx)
		{
			Tv_Result	=	m_TempTbl[A_DskIdx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ImmIsGroupRecAble(__u8 A_GroupType)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

#if 0
		if ( E_GrDvrRecDevMirror >= A_GroupType )
		{
			if ( NULL != m_GrupTbl[A_GroupType] )
			{
				Tv_Result	=	m_GrupTbl[A_GroupType]->IsRecAble();
			}
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs6Mng::ImmRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd)
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if (E_GrDvrRecDevMirror >= A_GroupType)
		{
			if ( (0 != A_ChMap) && (A_TimeSt <= A_TimeEd))
			{
				if (NULL != m_GrupTbl[A_GroupType].Obj)
				{
					Tv_Result	=	m_GrupTbl[A_GroupType].Obj->RecExpectSizeGet(A_TimeSt,A_TimeEd,A_ChMap);
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs6Mng::ReqRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd)
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFs6MngMsgRecSizeEstm	Tv_Msg;
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
					Tv_Msg.Base.Cmd				=	E_GrDvrFs6MngMsgRecSizeEstm;
					Tv_Msg.Group					=	A_GroupType;
					Tv_Msg.ChMap					=	A_ChMap;
					Tv_Msg.TimeSt					=	A_TimeSt;
					Tv_Msg.TimeEd					=	A_TimeEd;
					Tv_Msg.PtrRslt				=	&Tv_Result;

					m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				}
				else
				{
					DbgMsgPrint( "Cls_GrDvrFs6Mng::ReqRecSizeEstmate - bad time range.\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrDvrFs6Mng::ReqRecSizeEstmate - channel not selected.\n" );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::RtlBkgProc( void )
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
BOOL8	Cls_GrDvrFs6Mng::LcBgpBkupSizeEstm(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

#if 0
		if (m_BkupSizeEstm.IsRun)
		{
			m_GrupTbl[m_BkupSizeEstm.DevType]->BkupSizeEstmProc(&m_BkupSizeEstm);
			Tv_Result	=	TRUE;
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmAtDelTimeSet( __u32 A_Sec )
{
		m_AtDelTime	=	A_Sec;
		// update auto delete time
		LcAtDelLimUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::SafeAtDelTimeSet( __u32 A_Sec )
{
	// local -------------------
		St_GrDvrFs6MngMsgAtDelTimeSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs6MngMsgAtDelTimeSet;
		Tv_Msg.Sec						=	A_Sec;

#ifdef GR_TASKMSG_DEBUG
		if(!m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg)))
		{
			DbgMsgPrint("[DBG] %s MsgPost fail!\n", __FUNCTION__);
		}
#else
		m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
#endif
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs6Mng::AutoDelTimeGet( void )
{
		return	m_AtDelTime;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::LcAtDelLimUpdt( void )
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
void	Cls_GrDvrFs6Mng::ImmGroupFormat( __u8 A_GroupType)
{
	// local -------------------
	// code --------------------
		if(E_GrDvrFs6MngGroupCnt > A_GroupType)
		{
			if(NULL != m_GrupTbl[A_GroupType].Obj)
			{
				//TODO : implement
				//m_GrupTbl[A_GroupType]->GroupFormat(A_IsWithLog);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::SafeGroupFormat( __u8 A_GroupType, BOOL8 A_IsWithLog)
{
	// local -------------------
		St_GrDvrFs6MngMsgGroupFormat	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgGroupFormat;
		Tv_Msg.GroupType				=	A_GroupType;
		Tv_Msg.IsWithLog				=	A_IsWithLog;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void*	Cls_GrDvrFs6Mng::RtlTmpBufAlloc(__u32 A_Size)
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

#if 0
		if (E_GrDvrFs6MngTmpBufSize >= (m_TmpUsed + A_Size))
		{
			Tv_Result	=	(void*)((__u32)m_TmpBuf + m_TmpUsed);
			m_TmpUsed	=	m_TmpUsed + A_Size;
		}
		else
		{
			DbgMsgPrint("Cls_GrDvrFs6Mng::RtlTmpBufAlloc - not enough TMP buffer.\n");
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::RtlTmpBufFree(__u32 A_Size)
{
	// local -------------------
	// code --------------------

#if 0
		if (m_TmpUsed < A_Size)
		{
			m_TmpUsed	=	0;
			DbgMsgPrint("Cls_GrDvrFs6Mng::RtlTmpBufFree - bad size required.\n");
		}
		else
		{
			m_TmpUsed	=	m_TmpUsed + A_Size;
		}
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ImmFrmVdoAdd(Ptr_GrDvrFrmHdVdo A_PtrFrm)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		// init
		Tv_Result	=	FALSE;

		// check exist main
		if(NULL != m_GrupTbl[E_GrDvrRecDevNoraml].Obj)
		{
			if(m_GrupTbl[E_GrDvrRecDevNoraml].Obj->FrmVdoAdd(A_PtrFrm))
			{
				Tv_Result	=	TRUE;
			}
		}

		// check exist mirror
		if(NULL != m_GrupTbl[E_GrDvrRecDevMirror].Obj)
		{
			if(m_GrupTbl[E_GrDvrRecDevMirror].Obj->FrmVdoAdd(A_PtrFrm))
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ImmReadFwGoByTime(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;
		//check device type
		if (E_GrDvrRecDevMirror >= A_DevType)
		{
			if(NULL != m_GrupTbl[A_DevType].Obj)
			{
				Tv_Result	=	m_GrupTbl[A_DevType].Obj->ReadFwGoByTime(A_PtrCtrl, A_Time);
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ImmReadBwGoByTime(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;
		//check device type
		if(E_GrDvrRecDevMirror >= A_DevType)
		{
			if(NULL != m_GrupTbl[A_DevType].Obj)
			{
				Tv_Result	=	m_GrupTbl[A_DevType].Obj->ReadBwGoByTime(A_PtrCtrl, A_Time);
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ReqReadFwGoByTime(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs6MngMsgReadTimePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs6MngMsgReadFwByTime;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.Time						=	A_Time;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ImmFrmAdoAdd(Ptr_GrDvrFrmHdAdo A_PtrFrm)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		// init
		Tv_Result	=	TRUE;

		// check exist main
		if(NULL != m_GrupTbl[E_GrDvrRecDevNoraml].Obj)
		{
			if(m_GrupTbl[E_GrDvrRecDevNoraml].Obj->FrmAdoAdd(A_PtrFrm))
			{
				Tv_Result	=	TRUE;
			}
		}

		// check exist mirror
		if(NULL != m_GrupTbl[E_GrDvrRecDevMirror].Obj)
		{
			if(m_GrupTbl[E_GrDvrRecDevMirror].Obj->FrmAdoAdd(A_PtrFrm))
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::LcGroupRecFlush(void)
{
	// local -------------------
		__u32	Tv_GrpType;
	// code --------------------
		for(Tv_GrpType = 0; Tv_GrpType < E_GrDvrFs6MngGroupCnt; Tv_GrpType++)
		{
			if(NULL != m_GrupTbl[Tv_GrpType].Obj)
			{
				m_GrupTbl[Tv_GrpType].Obj->RecFlush();
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::LcTmrRecFlush(void)
{
	// local -------------------
	// code --------------------
		m_RflushTimer ++;
		if(E_GrDvrFs6MngRecFlushPeriod <= m_RflushTimer)
		{
			LcGroupRecFlush();

			// update group info
			LcGroupInfoUpdate();

			m_RflushTimer	=	0;
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ReqReadBwGoByTime(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs6MngMsgReadTimePara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs6MngMsgReadBwByTime;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.Time						=	A_Time;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ReqReadBwNext(__u8 A_DevType, Ptr_GrDvrFs6ReadCtrl A_PtrCtrl)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs6MngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs6MngMsgReadBwNext;
		Tv_Msg.Group					=	A_DevType;
		Tv_Msg.PtrCtrl				=	A_PtrCtrl;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::LcGroupInfoUpdate(void)
{
	// local -------------------
		__u8	Tv_GrpIdx;
	// code --------------------
		for( Tv_GrpIdx = 0; Tv_GrpIdx < E_GrDvrFs6MngGroupCnt; Tv_GrpIdx++)
		{
			m_GrupTbl[Tv_GrpIdx].Stat	=	ImmGroupRecInfoGet(Tv_GrpIdx, &m_GrupTbl[Tv_GrpIdx].SizeTot, &m_GrupTbl[Tv_GrpIdx].SizeRec);
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ImmGroupDataReadByHndl(__u8 A_GroupType, __u64 A_Hndl, void* A_PtrData, __u32 A_Size)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadParam;
		
		if(E_GrDvrFs6MngGroupCnt > A_GroupType)
		{
			if(NULL != m_GrupTbl[A_GroupType].Obj)
			{
				Tv_Result	=	((Cls_GrDvrFs6Grp*)m_GrupTbl[A_GroupType].Obj)->GroupDataReadByHndl(A_Hndl,A_PtrData,A_Size);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ReqGroupDataReadByHndl(__u8 A_GroupType, __u64 A_Hndl, void* A_PtrData, __u32 A_Size)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs6MngMsgGroupDataReadHndl	Tv_Msg;
	
	// code --------------------
		Tv_Result	=	E_GrErrReqSendFail;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Base.Cmd				=	E_GrDvrFs6MngMsgGroupDataReadHndl;
		Tv_Msg.PtrRslt				=	&Tv_Result;
		Tv_Msg.GroupType			=	A_GroupType;
		Tv_Msg.Hndl						=	A_Hndl;
		Tv_Msg.PtrBuf					=	A_PtrData;
		Tv_Msg.Size						=	A_Size;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFs6Mng::ImmDmuxNew(__u8 A_MaxChCnt)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;
		for( Tv_WkIdx = 0; Tv_WkIdx < E_GrDvrFs6MaxDmuxCnt; Tv_WkIdx++)
		{
			if(NULL == V_GrDvrFs6Env.DmuxTbl[Tv_WkIdx].Obj)
			{
				// create
				V_GrDvrFs6Env.DmuxTbl[Tv_WkIdx].Obj	=	(void*) new Cls_GrDvrFs6Dmux(A_MaxChCnt);
				if(NULL != V_GrDvrFs6Env.DmuxTbl[Tv_WkIdx].Obj)
				{
					Tv_Result	=	Tv_WkIdx;
				}
				else
				{
					DbgMsgPrint("Cls_GrDvrFs6Mng::ImmDmuxNew - not enough memory!\n");
				}
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ImmDmuxDelete(__s32 A_Hndl)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if((0 <= A_Hndl) && (E_GrDvrFs6MaxDmuxCnt > A_Hndl))
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)
			{
				delete	(Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj;
				V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj	=	NULL;
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmDmuxRecGroupSet(__s32 A_Hndl, __u8 A_RecGrpType)
{
	// local -------------------

	// code --------------------
		if((0 <= A_Hndl) && (E_GrDvrFs6MaxDmuxCnt > A_Hndl))
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)
			{
				((Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)->DmuxRecGroupSet(A_RecGrpType);
			}
		}

}
//--------------------------------------------------------------------
Ptr_GrDvrFs6ReadCtrl	Cls_GrDvrFs6Mng::ImmDmuxReadCtrlPtrGet(__s32 A_Hndl, __u8 A_Ch)
{
	// local -------------------
		Ptr_GrDvrFs6ReadCtrl	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if((0 <= A_Hndl) && (E_GrDvrFs6MaxDmuxCnt > A_Hndl))
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)
			{
				Tv_Result	=	((Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)->DmuxReadCtrlPtrGet(A_Ch);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs6Mng::ImmDmuxDo(__s32 A_Hndl)
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		
		if((0 <= A_Hndl) && (E_GrDvrFs6MaxDmuxCnt > A_Hndl))
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)
			{
				Tv_Result	=	((Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)->DmuxDo();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs6Mng::ImmDmuxReadyFrameRead(__s32 A_Hndl, Ptr_GrDvrFs6ReadFrameItem A_PtrBufTbl, __u32 A_BufCnt)
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if((0 <= A_Hndl) && (E_GrDvrFs6MaxDmuxCnt > A_Hndl))
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)
			{
				Tv_Result	=	((Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)->DmuxReadyFrameRead(A_PtrBufTbl,A_BufCnt);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmDmuxReadyFrameDone(__s32 A_Hndl, __u64 A_Map)
{
	// local -------------------
	// code --------------------
		if((0 <= A_Hndl) && (E_GrDvrFs6MaxDmuxCnt > A_Hndl))
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)
			{
				((Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)->DmuxReadyFrameDone(A_Map);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmDmuxGoTime(__s32 A_Hndl, Def_GrTime A_Time)
{
	// local -------------------
	// code --------------------
		if((0 <= A_Hndl) && (E_GrDvrFs6MaxDmuxCnt > A_Hndl))
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)
			{
				((Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)->DmuxGoTime(A_Time);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmDmuxDirSet(__s32 A_Hndl, BOOL8 A_IsDirBack)
{
	// local -------------------
	// code --------------------
		if((0 <= A_Hndl) && (E_GrDvrFs6MaxDmuxCnt > A_Hndl))
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)
			{
				((Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)->DmuxDirSet(A_IsDirBack);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmDmuxVdoChAbleSet(__s32 A_Hndl, __u32 A_Map)
{
	// local -------------------
	// code --------------------
		if((0 <= A_Hndl) && (E_GrDvrFs6MaxDmuxCnt > A_Hndl))
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)
			{
				((Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)->DmuxVdoChAbleSet(A_Map);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmDmuxAdoChAbleSet(__s32 A_Hndl, __u32 A_Map)
{
	// local -------------------
	// code --------------------
		if((0 <= A_Hndl) && (E_GrDvrFs6MaxDmuxCnt > A_Hndl))
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)
			{
				((Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)->DmuxAdoChAbleSet(A_Map);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ImmDmuxVdoFrmMaskSet(__s32 A_Hndl, __u8 A_Ch, __u8 A_Mask)
{
	// local -------------------
	// code --------------------
		if((0 <= A_Hndl) && (E_GrDvrFs6MaxDmuxCnt > A_Hndl))
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)
			{
				((Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)->DmuxVdoFrmMaskSet(A_Ch,A_Mask);
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Mng::ImmDmuxRunStaGet(__s32 A_Hndl)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrPlayStatStop;

		if((0 <= A_Hndl) && (E_GrDvrFs6MaxDmuxCnt > A_Hndl))
		{
			if(NULL != V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)
			{
				Tv_Result	=	((Cls_GrDvrFs6Dmux*)V_GrDvrFs6Env.DmuxTbl[A_Hndl].Obj)->DmuxRunStatGet();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrDvrFs6Mng::ReqDmuxNew(__u8 A_MaxChCnt)
{
	// local -------------------
		__s32	Tv_Result;
		St_GrDvrFs6MngMsgDmuxNew	Tv_Msg;
	// code --------------------
		Tv_Result	=	-1;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDmuxNew;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.MaxChCnt					=	A_MaxChCnt;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs6Mng::ReqDmuxDelete(__s32 A_Hndl)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs6MngMsgDmuxDelte	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDmuxDelete;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Hndl							=	A_Hndl;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ReqDmuxRecGroupSet(__s32 A_Hndl, __u8 A_RecGrpType)
{
	// local -------------------
		St_GrDvrFs6MngMsgDmuxGroupSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDmuxGroupSet;
		Tv_Msg.Hndl							=	A_Hndl;
		Tv_Msg.RecGrpType				=	A_RecGrpType;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
Ptr_GrDvrFs6ReadCtrl	Cls_GrDvrFs6Mng::ReqDmuxReadCtrlPtrGet(__s32 A_Hndl, __u8 A_Ch)
{
	// local -------------------
		Ptr_GrDvrFs6ReadCtrl	Tv_Result;
		St_GrDvrFs6MngMsgDmuxReadCtrlPtrGet	Tv_Msg;
	// code --------------------
		Tv_Result		=	NULL;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDmuxReadCtrlPtrGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Hndl							=	A_Hndl;
		Tv_Msg.Ch								=	A_Ch;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs6Mng::ReqDmuxDo(__s32 A_Hndl)
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFs6MngMsgDmuxDo	Tv_Msg;
	// code --------------------
		Tv_Result		=	NULL;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDmuxDo;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Hndl							=	A_Hndl;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs6Mng::ReqDmuxReadyFrameRead(__s32 A_Hndl, Ptr_GrDvrFs6ReadFrameItem A_PtrBufTbl, __u32 A_BufCnt)
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFs6MngMsgDmuxRdyFrmRead	Tv_Msg;
	// code --------------------
		Tv_Result		=	NULL;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDmuxRdyFrmRead;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Hndl							=	A_Hndl;
		Tv_Msg.PtrBufTbl				=	A_PtrBufTbl;
		Tv_Msg.BufCnt						=	A_BufCnt;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ReqDmuxReadyFrameDone(__s32 A_Hndl, __u64 A_Map)
{
	// local -------------------
		St_GrDvrFs6MngMsgDmuxRdyFrmDone	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDmuxRdyFrmDone;
		Tv_Msg.Hndl							=	A_Hndl;
		Tv_Msg.Map							=	A_Map;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ReqDmuxGoTime(__s32 A_Hndl, Def_GrTime A_Time)
{
	// local -------------------
		St_GrDvrFs6MngMsgDmuxGoTime	Tv_Msg;
	// code --------------------

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDmuxGoTime;
		Tv_Msg.Hndl							=	A_Hndl;
		Tv_Msg.Time							=	A_Time;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ReqDmuxDirSet(__s32 A_Hndl, BOOL8 A_IsDirBack)
{
	// local -------------------
		St_GrDvrFs6MngMsgDmuxDirSet	Tv_Msg;
	// code --------------------

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDmuxDirSet;
		Tv_Msg.Hndl							=	A_Hndl;
		Tv_Msg.IsDirBack				=	A_IsDirBack;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ReqDmuxVdoChAbleSet(__s32 A_Hndl, __u32 A_Map)
{
	// local -------------------
		St_GrDvrFs6MngMsgDmuxVdoChAbleSet	Tv_Msg;
	// code --------------------

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDmuxVdoChAbleSet;
		Tv_Msg.Hndl							=	A_Hndl;
		Tv_Msg.Map							=	A_Map;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ReqDmuxAdoChAbleSet(__s32 A_Hndl, __u32 A_Map)
{
	// local -------------------
		St_GrDvrFs6MngMsgDmuxAdoChAbleSet	Tv_Msg;
	// code --------------------

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDmuxAdoChAbleSet;
		Tv_Msg.Hndl							=	A_Hndl;
		Tv_Msg.Map							=	A_Map;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::ReqDmuxVdoFrmMaskSet(__s32 A_Hndl, __u8 A_Ch, __u8 A_Mask)
{
	// local -------------------
		St_GrDvrFs6MngMsgDmuxVdoFrmMaskSet	Tv_Msg;
	// code --------------------

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Base.Cmd					=	E_GrDvrFs6MngMsgDmuxVdoFrmMaskSet;
		Tv_Msg.Hndl							=	A_Hndl;
		Tv_Msg.Ch								=	A_Ch;
		Tv_Msg.Mask							=	A_Mask;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Mng::LcTmrCchFlush(void)
{
	// local -------------------
		__u32	Tv_WkIdx;
		__u8	Tv_DskIdx;
		__u32	Tv_BlkIdx;
		__u32	Tv_BlkOfs;
		void*	Tv_PtrBuf;
	// code --------------------
		m_CchTimer ++;
		if(10 <= m_CchTimer)
		{
			// check time out dirty
			for( Tv_WkIdx = 0; Tv_WkIdx < V_GrDvrFs6Env.Cch.ItemCnt; Tv_WkIdx++)
			{
				if(V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].IsUse  && V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].IsDirt && 
					(E_GrDvrFs6CchFlushPeriod <= (V_GrDvrFs6Env.TickFs - V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].AccTick)))
				{
					// do flush
					Tv_DskIdx	=	V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].DskId;
					if(NULL != V_GrDvrFs6Env.DskTbl[Tv_DskIdx])
					{
						Tv_BlkIdx	=	(__u32)(V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].DskAdr >> E_GrDvrFs6BlkShift) & 0xFFFF;
						Tv_BlkOfs	=	(__u32)V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].DskAdr & E_GrDvrFs6BlkOfsMask;
						Tv_PtrBuf	=	(void*)((Def_GrCalPtr)V_GrDvrFs6Env.Cch.PtrBuf + (Def_GrCalPtr)(Tv_WkIdx << E_GrDvrFs6CchUnitShft));
						((Cls_GrDvrFs6Disk*)V_GrDvrFs6Env.DskTbl[Tv_DskIdx])->BlkWriteDataByOfs(Tv_BlkIdx, Tv_BlkOfs, Tv_PtrBuf,
							E_GrDvrFs6CchUnitSize);
						V_GrDvrFs6Env.Cch.CtlTbl[Tv_WkIdx].IsDirt	=	FALSE;
					}
				}
			}

			// reset
			m_CchTimer		=	0;
		}
}
//--------------------------------------------------------------------
