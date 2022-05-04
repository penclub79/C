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

#include	<Dvr/Fs/Nvt/GrFsNvtIoRaid.h>

#include	<stdlib.h>

//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

extern	St_GrFsNvtEnv	V_GrFsNvtEnv;

//====================================================================

//--------------------------------------------------------------------
Cls_GrFsNvtIoRaid::Cls_GrFsNvtIoRaid(Cls_GrTaskSvr* A_TaskSvr, __u32 A_CchMinCnt, __u32 A_CchDataCnt) :
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		//init
		m_TaskSvr			=	A_TaskSvr;

		m_MsgMng			=	GrTaskSvrMsgCmdAlloc();

		m_RunMode			=	E_GrFsNvtIoRaidModeNotReady;

		m_RflushTimer	=	0;

		m_SmartTimer	=	E_GrFsNvtIoRaidSmartScanPeriod - 2;

		m_AtDelTimer	=	0;
		m_AtDelStep		=	0;
		m_AtDelBaseTm	=	0;

		m_LogGetCidx	=	0;
		GrDumyZeroMem(m_LogGetCtls, sizeof(m_LogGetCtls));

		GrDumyZeroMem( m_GrupTbl, sizeof(m_GrupTbl) );
		GrDumyZeroMem(m_SmartTbl, sizeof(m_SmartTbl));

		m_GrupTbl[E_GrDvrRecDevNoraml].IsCchRsvAble	=	TRUE;
				
#ifdef E_GrFsNvtIoRaidChkLogSeek
		m_DbgIsLogWork	=	FALSE;
#endif

		//register task
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrFsNvtIoRaidTimerPeriod, &m_HndlTimer );
		m_TaskSvr->RegistBkgClient( this );

}
//--------------------------------------------------------------------
Cls_GrFsNvtIoRaid::~Cls_GrFsNvtIoRaid()
{
		// request finish
		ReqFinish();

		//unregister timer
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistMsgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );


		GrTaskSvrMsgCmdFree( m_MsgMng );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrTaskMsgRecTimeMapDone	Tv_MsgTmDone;
	// code --------------------
		Tv_Result	=	FALSE;
		// check input message
		if ( m_MsgMng == A_PtrMsg->Cmd )
		{
			Tv_Result	=	TRUE;
			switch ( ((Ptr_GrFsNvtIoRaidMsgBase)A_PtrMsg)->Cmd )
			{
				case E_GrFsNvtIoRaidMsgInit:
					ImmInit();
					break;
				case E_GrFsNvtIoRaidMsgFinish:
					ImmFinish();
					break;
				case E_GrFsNvtIoRaidMsgSetup:
					ImmSetup( ((Ptr_GrFsNvtIoRaidMsgSetup)A_PtrMsg)->NrmMap, ((Ptr_GrFsNvtIoRaidMsgSetup)A_PtrMsg)->MirMap,
						((Ptr_GrFsNvtIoRaidMsgSetup)A_PtrMsg)->IsNrmOvwt, ((Ptr_GrFsNvtIoRaidMsgSetup)A_PtrMsg)->IsMirOvwt );
					break;
				case E_GrFsNvtIoRaidMsgFormat:
					if(NULL != ((Ptr_GrFsNvtIoRaidMsgFormat)A_PtrMsg)->PtrRslt)
					{
						*((Ptr_GrFsNvtIoRaidMsgFormat)A_PtrMsg)->PtrRslt	=
							ImmDiskFormat(((Ptr_GrFsNvtIoRaidMsgFormat)A_PtrMsg)->DevIdx,FALSE);
					}
					else
					{
						// post mode
						ImmDiskFormat(((Ptr_GrFsNvtIoRaidMsgFormat)A_PtrMsg)->DevIdx,TRUE);
					}
					break;
				case E_GrFsNvtIoRaidMsgDiskRecInfoGet:
					*((Ptr_GrFsNvtIoRaidMsgDiskRecInfoGet)A_PtrMsg)->PtrRslt	=	
						ImmDiskRecInfoGet(((Ptr_GrFsNvtIoRaidMsgDiskRecInfoGet)A_PtrMsg)->DevIdx, ((Ptr_GrFsNvtIoRaidMsgDiskRecInfoGet)A_PtrMsg)->PtrRtTotal,
						((Ptr_GrFsNvtIoRaidMsgDiskRecInfoGet)A_PtrMsg)->PtrRtRec);
					break;
				case E_GrFsNvtIoRaidMsgGroupStatGet:
					*((Ptr_GrFsNvtIoRaidMsgGroupStatGet)A_PtrMsg)->PtrRslt	=
						ImmGroupGetStat( ((Ptr_GrFsNvtIoRaidMsgGroupStatGet)A_PtrMsg)->GroupType );
					break;
				case E_GrFsNvtIoRaidMsgGroupRecInfoGet:
					*((Ptr_GrFsNvtIoRaidMsgGroupRecInfoGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupRecInfoGet(((Ptr_GrFsNvtIoRaidMsgGroupRecInfoGet)A_PtrMsg)->GroupType, ((Ptr_GrFsNvtIoRaidMsgGroupRecInfoGet)A_PtrMsg)->PtrRtTotal,
						((Ptr_GrFsNvtIoRaidMsgGroupRecInfoGet)A_PtrMsg)->PtrRtRec);
					break;
				case E_GrFsNvtIoRaidMsgGroupDayMapGet:
					*((Ptr_GrFsNvtIoRaidMsgGroupDayMapGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupGetMonthDayMap( ((Ptr_GrFsNvtIoRaidMsgGroupDayMapGet)A_PtrMsg)->Group, ((Ptr_GrFsNvtIoRaidMsgGroupDayMapGet)A_PtrMsg)->TimeMonth);
					break;
				case E_GrFsNvtIoRaidMsgGroupMinRatbTblGet:
					if(NULL != ((Ptr_GrFsNvtIoRaidMsgGroupMinRatbTblGet)A_PtrMsg)->PtrRslt)
					{
						*((Ptr_GrFsNvtIoRaidMsgGroupMinRatbTblGet)A_PtrMsg)->PtrRslt	=
							ImmGroupGetMinRatbTbl(((Ptr_GrFsNvtIoRaidMsgGroupMinRatbTblGet)A_PtrMsg)->Group, ((Ptr_GrFsNvtIoRaidMsgGroupMinRatbTblGet)A_PtrMsg)->TimeSt,
							((Ptr_GrFsNvtIoRaidMsgGroupMinRatbTblGet)A_PtrMsg)->ItemCnt, ((Ptr_GrFsNvtIoRaidMsgGroupMinRatbTblGet)A_PtrMsg)->PtrBuf);
					}
					else
					{
						ImmGroupGetMinRatbTbl(((Ptr_GrFsNvtIoRaidMsgGroupMinRatbTblGet)A_PtrMsg)->Group, ((Ptr_GrFsNvtIoRaidMsgGroupMinRatbTblGet)A_PtrMsg)->TimeSt,
							((Ptr_GrFsNvtIoRaidMsgGroupMinRatbTblGet)A_PtrMsg)->ItemCnt, ((Ptr_GrFsNvtIoRaidMsgGroupMinRatbTblGet)A_PtrMsg)->PtrBuf);
						// notify done
						Tv_MsgTmDone.MsgHd.Cmd	=	E_GrTaskMsgRecTimeMapDone;
						Tv_MsgTmDone.UserData		=	((Ptr_GrFsNvtIoRaidMsgGroupMinRatbTblGet)A_PtrMsg)->UserData;
						GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgTmDone, sizeof(Tv_MsgTmDone));
					}
					break;
				case E_GrFsNvtIoRaidMsgReadFwNext:
					*((Ptr_GrFsNvtIoRaidMsgReadPara)A_PtrMsg)->PtrRslt		=	
						ImmReadFwNext(((Ptr_GrFsNvtIoRaidMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrFsNvtIoRaidMsgReadPara)A_PtrMsg)->PtrCtrl);
					break;
				case E_GrFsNvtIoRaidMsgReadBwNext:
					*((Ptr_GrFsNvtIoRaidMsgReadPara)A_PtrMsg)->PtrRslt		=
						ImmReadBwNext(((Ptr_GrFsNvtIoRaidMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrFsNvtIoRaidMsgReadPara)A_PtrMsg)->PtrCtrl);
					break;
				case E_GrFsNvtIoRaidMsgReadFwByTime:
					*((Ptr_GrFsNvtIoRaidMsgReadTimePara)A_PtrMsg)->PtrRslt		=
						ImmReadFwByTime(((Ptr_GrFsNvtIoRaidMsgReadTimePara)A_PtrMsg)->Group, ((Ptr_GrFsNvtIoRaidMsgReadTimePara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrFsNvtIoRaidMsgReadTimePara)A_PtrMsg)->Time);
					break;
				case E_GrFsNvtIoRaidMsgReadBwByTime:
					*((Ptr_GrFsNvtIoRaidMsgReadTimePara)A_PtrMsg)->PtrRslt		=
						ImmReadBwByTime(((Ptr_GrFsNvtIoRaidMsgReadTimePara)A_PtrMsg)->Group, ((Ptr_GrFsNvtIoRaidMsgReadTimePara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrFsNvtIoRaidMsgReadTimePara)A_PtrMsg)->Time);
					break;
				case E_GrFsNvtIoRaidMsgGroupTimeRngGet:
					*((Ptr_GrFsNvtIoRaidMsgGroupTimeRngGet)A_PtrMsg)->PtrRslt	=	
						ImmGroupGetRecTimeRng( ((Ptr_GrFsNvtIoRaidMsgGroupTimeRngGet)A_PtrMsg)->Group, ((Ptr_GrFsNvtIoRaidMsgGroupTimeRngGet)A_PtrMsg)->PtrRtTimeSt,
						((Ptr_GrFsNvtIoRaidMsgGroupTimeRngGet)A_PtrMsg)->PtrRtTimeEd, ((Ptr_GrFsNvtIoRaidMsgGroupTimeRngGet)A_PtrMsg)->PtrRtTimeLast);
					break;
				case E_GrFsNvtIoRaidMsgRecSizeEstm:
					*(((Ptr_GrFsNvtIoRaidMsgRecSizeEstm)A_PtrMsg)->PtrRslt)		=	
						ImmRecSizeEstmate(((Ptr_GrFsNvtIoRaidMsgRecSizeEstm)A_PtrMsg)->Group, ((Ptr_GrFsNvtIoRaidMsgRecSizeEstm)A_PtrMsg)->ChMap,
						((Ptr_GrFsNvtIoRaidMsgRecSizeEstm)A_PtrMsg)->TimeSt, ((Ptr_GrFsNvtIoRaidMsgRecSizeEstm)A_PtrMsg)->TimeEd);
					break;
				case E_GrFsNvtIoRaidMsgLogAdd:
					ImmLogAdd( &((Ptr_GrFsNvtIoRaidMsgLogAdd)A_PtrMsg)->Log ) ;
					break;
				case E_GrFsNvtIoRaidMsgAtDelTimeSet:
					ImmAtDelTimeSet( ((Ptr_GrFsNvtIoRaidMsgAtDelTimeSet)A_PtrMsg)->Sec );
					break;
				case E_GrFsNvtIoRaidMsgGroupFormat:
					if(NULL != ((Ptr_GrFsNvtIoRaidMsgGroupFormat)A_PtrMsg)->PtrRslt)
					{
						*((Ptr_GrFsNvtIoRaidMsgGroupFormat)A_PtrMsg)->PtrRslt	=
							ImmGroupDiskFormat(((Ptr_GrFsNvtIoRaidMsgGroupFormat)A_PtrMsg)->GroupMap, FALSE);
					}
					else
					{
						// post mode
						ImmGroupDiskFormat(((Ptr_GrFsNvtIoRaidMsgGroupFormat)A_PtrMsg)->GroupMap, TRUE);
					}
					break;
				case E_GrFsNvtIoRaidMsgDiskModelGet:
					*((Ptr_GrFsNvtIoRaidMsgDiskModelGet)A_PtrMsg)->PtrRslt		=	
						ImmDiskModelNameGet(((Ptr_GrFsNvtIoRaidMsgDiskModelGet)A_PtrMsg)->DevIdx, ((Ptr_GrFsNvtIoRaidMsgDiskModelGet)A_PtrMsg)->StrRt);
					break;
				case E_GrFsNvtIoRaidMsgReadFwStart:
					*((Ptr_GrFsNvtIoRaidMsgReadPara)A_PtrMsg)->PtrRslt		=
						ImmReadFwStart(((Ptr_GrFsNvtIoRaidMsgReadPara)A_PtrMsg)->Group, ((Ptr_GrFsNvtIoRaidMsgReadPara)A_PtrMsg)->PtrCtrl);
					break;
				case E_GrFsNvtIoRaidMsgReadFwByHndl:
					*((Ptr_GrFsNvtIoRaidMsgReadHndlPara)A_PtrMsg)->PtrRslt		=
						ImmReadFwByHndl(((Ptr_GrFsNvtIoRaidMsgReadHndlPara)A_PtrMsg)->Group, ((Ptr_GrFsNvtIoRaidMsgReadHndlPara)A_PtrMsg)->PtrCtrl,
						((Ptr_GrFsNvtIoRaidMsgReadHndlPara)A_PtrMsg)->Hndl);
					break;
				case E_GrFsNvtIoRaidMsgRecBufAdd:
					ImmRecBufAdd(((Ptr_GrFsNvtIoRaidMsgRecBufAdd)A_PtrMsg)->PtrIovs, ((Ptr_GrFsNvtIoRaidMsgRecBufAdd)A_PtrMsg)->Cnt);
					break;
				case E_GrFsNvtIoRaidMsgLogGetOpen:
					*((Ptr_GrFsNvtIoRaidMsgLogGetOpen)A_PtrMsg)->PtrRslt	=
						ImmLogGetOpen(((Ptr_GrFsNvtIoRaidMsgLogGetOpen)A_PtrMsg)->Group, ((Ptr_GrFsNvtIoRaidMsgLogGetOpen)A_PtrMsg)->TimeSt,
						((Ptr_GrFsNvtIoRaidMsgLogGetOpen)A_PtrMsg)->TimeEd, ((Ptr_GrFsNvtIoRaidMsgLogGetOpen)A_PtrMsg)->MaxCnt,
						((Ptr_GrFsNvtIoRaidMsgLogGetOpen)A_PtrMsg)->ChMask, ((Ptr_GrFsNvtIoRaidMsgLogGetOpen)A_PtrMsg)->TypeMask,
						((Ptr_GrFsNvtIoRaidMsgLogGetOpen)A_PtrMsg)->PtrRtBuf, ((Ptr_GrFsNvtIoRaidMsgLogGetOpen)A_PtrMsg)->IsFrmMode);
					break;
				case E_GrFsNvtIoRaidMsgLogGetChkDone:
					*((Ptr_GrFsNvtIoRaidMsgLogGetChkDone)A_PtrMsg)->PtrRslt	=
						ImmLogGetChkDone(((Ptr_GrFsNvtIoRaidMsgLogGetChkDone)A_PtrMsg)->Hndl, ((Ptr_GrFsNvtIoRaidMsgLogGetChkDone)A_PtrMsg)->PtrRtCnt);
					break;
				case E_GrFsNvtIoRaidMsgLogGetClose:
					ImmLogGetClose(((Ptr_GrFsNvtIoRaidMsgLogGetClose)A_PtrMsg)->Hndl);
					break;
				case E_GrFsNvtIoRaidMsgLogGetFrameModeCheck:
					*((Ptr_GrFsNvtIoRaidMsgLogGetPartCheck)A_PtrMsg)->PtrRslt	=
						ImmLogGetFrameModeCheck(((Ptr_GrFsNvtIoRaidMsgLogGetPartCheck)A_PtrMsg)->Hndl, ((Ptr_GrFsNvtIoRaidMsgLogGetPartCheck)A_PtrMsg)->PtrRtCnt);
					break;
				case E_GrFsNvtIoRaidMsgLogGetFrameModeResume:
					*((Ptr_GrFsNvtIoRaidMsgLogGetPartResume)A_PtrMsg)->PtrRslt	=
						ImmLogGetFrameModeResume(((Ptr_GrFsNvtIoRaidMsgLogGetPartResume)A_PtrMsg)->Hndl);
					break;
				case E_GrFsNvtIoRaidMsgDiskInstMapGet:
					*((Ptr_GrFsNvtIoRaidMsgDiskInstMapGet)A_PtrMsg)->PtrRslt	=
						ImmDiskInstMapGet(((Ptr_GrFsNvtIoRaidMsgDiskInstMapGet)A_PtrMsg)->PtrRtBadMap);
					break;
				case  E_GrFsNvtIoRaidMsgDiskSmartRead:
					*((Ptr_GrFsNvtIoRaidMsgDiskSmartRead)A_PtrMsg)->PtrRslt	=
						ImmDiskSmartRead(((Ptr_GrFsNvtIoRaidMsgDiskSmartRead)A_PtrMsg)->DevIdx, ((Ptr_GrFsNvtIoRaidMsgDiskSmartRead)A_PtrMsg)->PtrRtData);
					break;
				case E_GrFsNvtIoRaidMsgGroupMakeFull:
					ImmGroupMakeFull(((Ptr_GrFsNvtIoRaidMsgGroupMakeFull)A_PtrMsg)->GroupType);
					break;
				case E_GrFsNvtIoRaidMsgSctlClose:
					*((Ptr_GrFsNvtIoRaidMsgSctlClose)A_PtrMsg)->PtrRslt	=
						ImmSctlClose(((Ptr_GrFsNvtIoRaidMsgSctlClose)A_PtrMsg)->SubType, ((Ptr_GrFsNvtIoRaidMsgSctlClose)A_PtrMsg)->PtrCtrl);
					break;

				default:
					Tv_Result	=	FALSE;
					break;
			}
			
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::RtlTimer( __u32 A_Hndl )
{
		// update tick
		V_GrFsNvtEnv.Tick100ms ++;

		LcSmartUpdt();

		LcTmrRecFlush();

		LcTmrAutoDelete();

#ifdef E_GrFsNvtIoRaidChkLogSeek
		if(m_DbgIsLogWork)
		{
			DbgMsgPrint("[DBG] log work!\n");
			m_DbgIsLogWork	=	FALSE;
		}
#endif

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ReqSetup(__u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir)
{
	// local -------------------
		St_GrFsNvtIoRaidMsgSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgSetup;
		Tv_Msg.NrmMap						=	A_NormalMap;
		Tv_Msg.IsNrmOvwt				=	A_IsOvwtNrm;
		Tv_Msg.MirMap						=	A_MirrorMap;
		Tv_Msg.IsMirOvwt				=	A_IsOvwtMir;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcGroupMakeByMap(__u8 A_GrpMap)
{
	// local -------------------
		__u8	Tv_GrIdx;
		__u8	Tv_GrpMask;
	// code --------------------
		Tv_GrpMask	=	1;

		// create group
		for(Tv_GrIdx=0; Tv_GrIdx < E_GrFsNvtIoRaidGroupCnt; Tv_GrIdx++)
		{
			// check group selected
			if (0 != (Tv_GrpMask & A_GrpMap))
			{
				if (0 != m_GrupTbl[Tv_GrIdx].DskMap)
				{
					DbgMsgPrint("Cls_GrFsNvtIoRaid::LcGroupMakeByMap - new Cls_GrFsNvtRgrp\n");
					m_GrupTbl[Tv_GrIdx].Obj	=	(Cls_GrFsNvtRgrp*) new Cls_GrFsNvtRgrp(Tv_GrIdx,
						m_GrupTbl[Tv_GrIdx].DskMap, m_GrupTbl[Tv_GrIdx].IsOvwt, m_GrupTbl[Tv_GrIdx].IsCchRsvAble);
					// check exist
					if (NULL != m_GrupTbl[Tv_GrIdx].Obj)
					{
						if (E_GrDvrRecStatUnformated >= m_GrupTbl[Tv_GrIdx].Obj->StatusGet(FALSE))
						{
							delete	m_GrupTbl[Tv_GrIdx].Obj;
							m_GrupTbl[Tv_GrIdx].Obj		=	NULL;
						}
					}
				}
			}
			// next
			Tv_GrpMask	=	Tv_GrpMask << 1;
		}

		// file system changed
		{
			St_GrTaskMsgHd	Tv_Msg;

			Tv_Msg.Cmd		=	E_GrTaskMsgFsStatChg;
			GrTaskSvrMsgBroadCast(&Tv_Msg, sizeof(Tv_Msg));
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcGroupFreeByMap(__u8 A_GrpMap)
{
	// local -------------------
		__u8	Tv_GrIdx;
		__u8	Tv_GrpMask;
	// code --------------------
		Tv_GrpMask	=	1;

		// release group
		for (Tv_GrIdx=0; Tv_GrIdx < E_GrFsNvtIoRaidGroupCnt; Tv_GrIdx++)
		{
			// check selected
			if (0 != (Tv_GrpMask & A_GrpMap))
			{
				if (NULL != m_GrupTbl[Tv_GrIdx].Obj)
				{
					delete	m_GrupTbl[Tv_GrIdx].Obj;
					m_GrupTbl[Tv_GrIdx].Obj	=	NULL;
				}
			}
			// next
			Tv_GrpMask	=	Tv_GrpMask << 1;
		}

		// file system changed
		{
			St_GrTaskMsgHd	Tv_Msg;

			Tv_Msg.Cmd		=	E_GrTaskMsgFsStatChg;
			GrTaskSvrMsgBroadCast(&Tv_Msg, sizeof(Tv_Msg));
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcGroupRemakeByMap(__u8 A_GrpMap)
{
	// local -------------------
		__u8	Tv_GrIdx;
		__u8	Tv_GrpMask;
	// code --------------------
		Tv_GrpMask	=	1;

		for (Tv_GrIdx=0; Tv_GrIdx < E_GrFsNvtIoRaidGroupCnt; Tv_GrIdx++)
		{
			// check selected
			if (0 != (Tv_GrpMask & A_GrpMap))
			{
				// release
				if (NULL != m_GrupTbl[Tv_GrIdx].Obj)
				{
					delete	m_GrupTbl[Tv_GrIdx].Obj;
					m_GrupTbl[Tv_GrIdx].Obj	=	NULL;
				}
				// make
				if (0 != m_GrupTbl[Tv_GrIdx].DskMap)	// check exsit disk map
				{
					DbgMsgPrint("Cls_GrFsNvtIoRaid::LcGroupRemakeByMap - new Cls_GrFsNvtRgrp\n");
					m_GrupTbl[Tv_GrIdx].Obj	=	(Cls_GrFsNvtRgrp*) new Cls_GrFsNvtRgrp(Tv_GrIdx,
						m_GrupTbl[Tv_GrIdx].DskMap, m_GrupTbl[Tv_GrIdx].IsOvwt, m_GrupTbl[Tv_GrIdx].IsCchRsvAble);
					// check exist
					if (NULL != m_GrupTbl[Tv_GrIdx].Obj)
					{
						if (E_GrDvrRecStatUnformated >= m_GrupTbl[Tv_GrIdx].Obj->StatusGet(FALSE))
						{
							delete	m_GrupTbl[Tv_GrIdx].Obj;
							m_GrupTbl[Tv_GrIdx].Obj		=	NULL;
						}
					}
				}
			}
			// next
			Tv_GrpMask	=	Tv_GrpMask << 1;
		}

		// file system changed
		{
			St_GrTaskMsgHd	Tv_Msg;

			Tv_Msg.Cmd		=	E_GrTaskMsgFsStatChg;
			GrTaskSvrMsgBroadCast(&Tv_Msg, sizeof(Tv_Msg));
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ImmSetup( __u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir )
{
	// local -------------------
	// code --------------------
		// check progress format
		if(E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			DbgMsgPrint("Cls_GrFsNvtIoRaid::ImmSetup - not initialized or in format mode!\n");
			return;
		}

		// close all log get contorl
		LcLogGetCloseAll();

		// update group info
		m_GrupTbl[E_GrDvrRecDevNoraml].DskMap	=	A_NormalMap;
		m_GrupTbl[E_GrDvrRecDevNoraml].IsOvwt	=	A_IsOvwtNrm;

		m_GrupTbl[E_GrDvrRecDevMirror].DskMap	=	A_MirrorMap;
		m_GrupTbl[E_GrDvrRecDevMirror].IsOvwt	=	A_IsOvwtMir;

		LcGroupRemakeByMap(3);

		LcGroupInfoUpdate();

		m_AtDelBaseTm	=	E_GrFsNvtIoRaidAutoDelPeriod;		// auto delete flush


}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ReqInit(void)
{
	// local -------------------
		St_GrFsNvtIoRaidMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Cmd			=	E_GrFsNvtIoRaidMsgInit;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ReqFinish(void)
{
	// local -------------------
		St_GrFsNvtIoRaidMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Cmd			=	E_GrFsNvtIoRaidMsgFinish;
		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ImmInit( void )
{
	// local -------------------
		__u32	Tv_DskIdx;
	// code --------------------
		if(E_GrFsNvtIoRaidModeNotReady != m_RunMode)
		{
			return;
		}

		// build disk list
		for(Tv_DskIdx=0; Tv_DskIdx < V_GrFsNvtEnv.DiskMaxCnt; Tv_DskIdx++)
		{
			V_GrFsNvtEnv.DskTbl[Tv_DskIdx]	=	(void*) new Cls_GrFsNvtHdd(Tv_DskIdx);
			// check not exist disk
			if(E_GrDvrRecStatNotexist == ((Cls_GrFsNvtHdd*)V_GrFsNvtEnv.DskTbl[Tv_DskIdx])->DiskStatGet())
			{
				delete	(Cls_GrFsNvtHdd*)V_GrFsNvtEnv.DskTbl[Tv_DskIdx];
				V_GrFsNvtEnv.DskTbl[Tv_DskIdx]	=	NULL;
			}
		}
		// temperature update
		m_SmartTimer	=	E_GrFsNvtIoRaidSmartScanPeriod;
		LcSmartUpdt();

		// ready
		m_RunMode		=	E_GrFsNvtIoRaidModeNormal;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ImmFinish(void)
{
	// local -------------------
		__u32	Tv_DskIdx;
		__u32	Tv_GrpIdx;
	// code --------------------
		// release group
		for(Tv_GrpIdx = 0; Tv_GrpIdx < E_GrFsNvtIoRaidGroupCnt; Tv_GrpIdx++)
		{
			if(NULL != m_GrupTbl[Tv_GrpIdx].Obj)
			{
				delete	m_GrupTbl[Tv_GrpIdx].Obj;
				m_GrupTbl[Tv_GrpIdx].Obj	=	NULL;
			}
		}

		// release disk
		for(Tv_DskIdx=0; Tv_DskIdx < E_GrFsNvtMaxDiskCnt; Tv_DskIdx++)
		{
			if(NULL != V_GrFsNvtEnv.DskTbl[Tv_DskIdx])
			{
				delete	(Cls_GrFsNvtHdd*)V_GrFsNvtEnv.DskTbl[Tv_DskIdx];
				V_GrFsNvtEnv.DskTbl[Tv_DskIdx]	=	NULL;
			}
		}

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ReqDiskFormat(__u8 A_DevIdx, BOOL8 A_IsPost)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrFsNvtIoRaidMsgFormat	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrErrBusy;
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgFormat;
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
Def_GrErrCode	Cls_GrFsNvtIoRaid::ImmDiskFormat(__u8 A_DevIdx, BOOL8 A_IsPost)
{
	// local -------------------
		__u8	Tv_DskMap;
		__u8	Tv_GrpMap;
	// code --------------------
		Tv_DskMap	=	1 << A_DevIdx;

		// close all log get contorl
		LcLogGetCloseAll();

		if (E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			DbgMsgPrint("Cls_GrFsNvtIoRaid::ImmGroupDiskFormat - in format mode\n");
			if (A_IsPost)
			{
				LcFormatFailMsgBradcast(0, Tv_DskMap, E_GrErrAlreadyExist);
			}
			return	E_GrErrAlreadyExist;
		}

		if (E_GrFsNvtMaxDiskCnt <= A_DevIdx)
		{
			if (A_IsPost)
			{
				LcFormatFailMsgBradcast(0, 0, E_GrErrBadParam);
			}
			return	E_GrErrBadParam;
		}

		// check exist disl
		if (NULL == V_GrFsNvtEnv.DskTbl[A_DevIdx])
		{
			if (A_IsPost)
			{
				LcFormatFailMsgBradcast(0, 0, E_GrErrBadParam);
			}
			return	E_GrErrBadParam;
		}

		// check group
		Tv_GrpMap		=	0;
		if (0 != (m_GrupTbl[E_GrDvrRecDevNoraml].DskMap & (1 << A_DevIdx)))
		{
			Tv_GrpMap		=	 1 << E_GrDvrRecDevNoraml;
		}
		if (0 != (m_GrupTbl[E_GrDvrRecDevMirror].DskMap & (1 << A_DevIdx)))
		{
			Tv_GrpMap		=	 1 << E_GrDvrRecDevMirror;
		}

		return	LcFormatSetupByDiskmap(Tv_GrpMap, Tv_DskMap, A_IsPost);

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ImmGroupDiskFormat(__u8 A_GroupMap, BOOL8 A_IsPost)
{
	// local -------------------
		__u8	Tv_DskMap;
		__u32	Tv_GrpMask;
		__u32	Tv_GrpIdx;

	// code --------------------
		// close all log get contorl
		LcLogGetCloseAll();

		if(E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			DbgMsgPrint("Cls_GrFsNvtIoRaid::ImmGroupDiskFormat - in format mode\n");
			if (A_IsPost)
			{
				LcFormatFailMsgBradcast(A_GroupMap, 0, E_GrErrAlreadyExist);
			}
			return	E_GrErrAlreadyExist;
		}

		// check group map
		if (0 == (3 & A_GroupMap))
		{
			if (A_IsPost)
			{
				LcFormatFailMsgBradcast(A_GroupMap, 0, E_GrErrBadParam);
			}
			return	E_GrErrBadParam;
		}

		Tv_DskMap		=	0;
		Tv_GrpMask	=	1;
		for (Tv_GrpIdx = 0; Tv_GrpIdx < E_GrFsNvtIoRaidGroupCnt; Tv_GrpIdx++)
		{
			// check exist group
			if (0 != (Tv_GrpMask & A_GroupMap))
			{
				Tv_DskMap		=	Tv_DskMap | m_GrupTbl[Tv_GrpIdx].DskMap;
			}

			// next
			Tv_GrpMask	=	Tv_GrpMask << 1;
		}

		return	LcFormatSetupByDiskmap(A_GroupMap, Tv_DskMap, A_IsPost);

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::LcFormatSetupByDiskmap(__u8 A_GrpMap, __u8 A_DskMap, BOOL8 A_IsPost)
{
	// local -------------------
		__u8	Tv_DskCnt;
		__u8	Tv_DskMap;
		__u32	Tv_DskMask;
		__u32	Tv_DskIdx;

	// code --------------------
		Tv_DskMap			=	A_DskMap;
		// check and patch disk map
		Tv_DskCnt			=	0;
		Tv_DskMask		=	1;
		for (Tv_DskIdx = 0; Tv_DskIdx < E_GrFsNvtMaxDiskCnt; Tv_DskIdx++)
		{
			if (NULL != V_GrFsNvtEnv.DskTbl[Tv_DskIdx])
			{
				if(0 != (Tv_DskMap & Tv_DskMask))
				{
					Tv_DskCnt ++;
				}
			}
			else
			{
				Tv_DskMap	=	Tv_DskMap & (~Tv_DskMask);
			}

			// next
			Tv_DskMask	=	Tv_DskMask << 1;
		}

		// check disk exist
		if (0 == Tv_DskMap)
		{
			if (A_IsPost)
			{
				LcFormatFailMsgBradcast(A_GrpMap, 0, E_GrErrBadParam);
			}
			return	E_GrErrBadParam;
		}

		// do start
		LcGroupFreeByMap(A_GrpMap);

		// do format
		m_Format.GroupMap		=	A_GrpMap;
		m_Format.DskMap			=	Tv_DskMap;

		m_Format.DskPrgs		=	0;
		m_Format.DskCnt			=	Tv_DskCnt;
		m_Format.DskIdx			=	0;

		m_Format.BlkCnt			=	0;
		m_Format.BlkPrgs		=	0;
		m_Format.TotPrgs		=	0;

		m_RunMode	=	E_GrFsNvtIoRaidModeFormat;

		return	E_GrErrNone;

}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtIoRaid::ReqDiskRecInfoGet(__u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrFsNvtIoRaidMsgDiskRecInfoGet	Tv_Msg;
	// code --------------------
		Tv_Result			=	E_GrDvrRecStatNotexist;
		*A_PtrRtTotal	=	0;
		*A_PtrRtRec		=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgDiskRecInfoGet;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.PtrRtTotal				=	A_PtrRtTotal;
		Tv_Msg.PtrRtRec					=	A_PtrRtRec;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtIoRaid::ImmDiskRecInfoGet(__u8 A_DevIdx, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;

		// check valid device index
		if ( E_GrFsNvtMaxDiskCnt > A_DevIdx )
		{
			// check exist disk
			if(NULL != V_GrFsNvtEnv.DskTbl[A_DevIdx])
			{
				Tv_Result	=	((Cls_GrFsNvtHdd*)V_GrFsNvtEnv.DskTbl[A_DevIdx])->DiskInfoGet(A_PtrRtTotal, A_PtrRtRec);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::ImmDiskModelNameGet(__u8 A_DevIdx, WCHAR* A_StrRt)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check valid device index
		if(E_GrFsNvtMaxDiskCnt > A_DevIdx)
		{
			// check exist disk
			if(NULL != V_GrFsNvtEnv.DskTbl[A_DevIdx])
			{
				((Cls_GrFsNvtHdd*)V_GrFsNvtEnv.DskTbl[A_DevIdx])->ModelNameGet(A_StrRt);
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::ReqDiskModelNameGet(__u8 A_DevIdx, WCHAR* A_StrRt)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrFsNvtIoRaidMsgDiskModelGet	Tv_Msg;
	// code --------------------
		Tv_Result		=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgDiskModelGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.DevIdx						=	A_DevIdx;
		Tv_Msg.StrRt						=	A_StrRt;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtIoRaid::ReqGroupRecInfoGet(__u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
#if 0
		__u8	Tv_Result;
		St_GrFsNvtIoRaidMsgGroupRecInfoGet	Tv_Msg;
#endif
	// code --------------------
#if 0
		Tv_Result			=	E_GrDvrRecStatNotexist;
		*A_PtrRtTotal	=	0;
		*A_PtrRtRec		=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgGroupRecInfoGet;
		Tv_Msg.GroupType				=	A_GroupType;
		Tv_Msg.PtrRtTotal				=	A_PtrRtTotal;
		Tv_Msg.PtrRtRec					=	A_PtrRtRec;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
#else
		return	ImmGroupRecInfoGet(A_GroupType, A_PtrRtTotal, A_PtrRtRec);
#endif
}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtIoRaid::ReqRecInfoGet(__u8* A_PtrRtGroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_GrpIdx;
	// code --------------------
		Tv_GrpIdx	=	0;
		Tv_Result	=	m_GrupTbl[Tv_GrpIdx].Stat;
		if(E_GrDvrRecStatEmpty > Tv_Result)
		{
			// check mirror
			if(E_GrDvrRecStatEmpty <= m_GrupTbl[1].Stat)
			{
				Tv_GrpIdx ++;
				Tv_Result	=	m_GrupTbl[Tv_GrpIdx].Stat;
			}
		}

		*A_PtrRtGroupType	=	Tv_GrpIdx;
		*A_PtrRtTotal			=	m_GrupTbl[Tv_GrpIdx].SizeTot;
		*A_PtrRtRec				=	m_GrupTbl[Tv_GrpIdx].SizeRec;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtIoRaid::ImmGroupGetStat(__u8 A_GroupType)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecStatNotexist;
		if ( E_GrFsNvtIoRaidGroupCnt > A_GroupType )
		{
			if(NULL != m_GrupTbl[A_GroupType].Obj)
			{
				Tv_Result	=	m_GrupTbl[A_GroupType].Obj->StatusGet(FALSE);
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtIoRaid::ImmGroupRecInfoGet(__u8 A_GroupType, __u64* A_PtrRtTotal, __u64* A_PtrRtRec)
{
#if 0
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result			=	E_GrDvrRecStatNotexist;
		*A_PtrRtTotal	=	0;
		*A_PtrRtRec		=	0;
		if ( E_GrFsNvtIoRaidGroupCnt > A_GroupType )
		{
			if(NULL != m_GrupTbl[A_GroupType].Obj)
			{
				Tv_Result	=	((Cls_GrFsNvtRgrp*)m_GrupTbl[A_GroupType].Obj)->GroupInfoGet(A_PtrRtTotal, A_PtrRtRec,FALSE);
			}
		}
		return	Tv_Result;
#else
	*A_PtrRtTotal	=	m_GrupTbl[A_GroupType].SizeTot;
	*A_PtrRtRec		=	m_GrupTbl[A_GroupType].SizeRec;
	return	m_GrupTbl[A_GroupType].Stat;
#endif
}
//--------------------------------------------------------------------
__u32	Cls_GrFsNvtIoRaid::ReqGroupGetMonthDayMap(__u8 A_GroupType, Def_GrTime A_TimeMonth)
{
	// local -------------------
		__u32	Tv_Result;
		St_GrFsNvtIoRaidMsgGroupDayMapGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgGroupDayMapGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.TimeMonth				=	A_TimeMonth;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrFsNvtIoRaid::ImmGroupGetMonthDayMap(__u8 A_GroupType, Def_GrTime A_TimeMonth)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrFsNvtIoRaidGroupCnt > A_GroupType )
		{
			if(NULL != m_GrupTbl[A_GroupType].Obj)
			{
				Tv_Result	=	m_GrupTbl[A_GroupType].Obj->RecMonthDayMapGet(A_TimeMonth);
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::ReqGroupGetMinRatbTbl(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrBuf,
	BOOL8 A_IsPost, __u32 A_UserData)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrFsNvtIoRaidMsgGroupMinRatbTblGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgGroupMinRatbTblGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.TimeSt						=	A_TimeSt;
		Tv_Msg.ItemCnt					=	A_ItemCnt;
		Tv_Msg.PtrBuf						=	A_PtrBuf;
		Tv_Msg.UserData					=	A_UserData;

		if(A_IsPost)
		{
			Tv_Msg.PtrRslt			=	NULL;
			Tv_Result	=	m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		}
		else
		{
			m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::ImmGroupGetMinRatbTbl(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrRtBuf)
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

			if (E_GrFsNvtIoRaidGroupCnt > A_GroupType)
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
BOOL8	Cls_GrFsNvtIoRaid::ImmGroupGetRecTimeRng(__u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd, Def_GrTime* A_PtrRtTimeLast)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		*A_PtrRtTimeSt		=	0;
		*A_PtrRtTimeEd		=	0;
		if(NULL != A_PtrRtTimeLast)
		{
			*A_PtrRtTimeLast	=	0;
		}
		
		if ( E_GrFsNvtIoRaidGroupCnt > A_GroupType )
		{
			if(NULL != m_GrupTbl[A_GroupType].Obj)
			{
				m_GrupTbl[A_GroupType].Obj->RecTimeRangeGet(A_PtrRtTimeSt, A_PtrRtTimeEd);
				if(NULL != A_PtrRtTimeLast)
				{
					*A_PtrRtTimeLast	=	m_GrupTbl[A_GroupType].Obj->RecTimeLastGet();
				}
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::ReqGroupGetRecTimeRng(__u8 A_GroupType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd, Def_GrTime* A_PtrRtTimeLast)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrFsNvtIoRaidMsgGroupTimeRngGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgGroupTimeRngGet;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_GroupType;
		Tv_Msg.PtrRtTimeSt			=	A_PtrRtTimeSt;
		Tv_Msg.PtrRtTimeEd			=	A_PtrRtTimeEd;
		Tv_Msg.PtrRtTimeLast		=	A_PtrRtTimeLast;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ImmReadFwNext(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
	// local -------------------
	// code --------------------
		if (E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrPermisDenied;
		}


		if(E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrFail;
		}

		if(E_GrDvrRecDevMirror < A_SubType)
		{
			return	E_GrErrBadParam;
		}

		if(NULL == m_GrupTbl[A_SubType].Obj)
		{
			return	E_GrErrBadParam;
		}

		return	m_GrupTbl[A_SubType].Obj->ReadFwNext(A_PtrCtrl);

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ImmReadFwStart(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
	// local -------------------
	// code --------------------
		if (E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrPermisDenied;
		}

		if(E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrBusy;
		}

		if(E_GrDvrRecDevMirror < A_SubType)
		{
			return	E_GrErrBadParam;
		}

		if(NULL == m_GrupTbl[A_SubType].Obj)
		{
			return	E_GrErrBadParam;
		}

		return	m_GrupTbl[A_SubType].Obj->ReadFwStart(A_PtrCtrl);

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ImmReadBwNext(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
	// local -------------------
	// code --------------------
		if (E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrPermisDenied;
		}

		if(E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrFail;
		}

		if(E_GrDvrRecDevMirror < A_SubType)
		{
			return	E_GrErrBadParam;
		}

		if(NULL == m_GrupTbl[A_SubType].Obj)
		{
			return	E_GrErrBadParam;
		}

		return	m_GrupTbl[A_SubType].Obj->ReadBwNext(A_PtrCtrl);

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ImmLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		__u8	Tv_DevType;
	// code --------------------
		if(E_GrFsNvtIoRaidModeNormal == m_RunMode)
		{
			//check device type
			for ( Tv_DevType=0;Tv_DevType < E_GrFsNvtIoRaidGroupCnt;Tv_DevType++ )
			{
				if(NULL != m_GrupTbl[Tv_DevType].Obj)
				{
					m_GrupTbl[Tv_DevType].Obj->LogAdd(A_PtrItem);
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ReqLogAdd(Ptr_GrDvrLogItm A_PtrItem)
{
	// local -------------------
		St_GrFsNvtIoRaidMsgLogAdd	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgMng;
		Tv_Msg.Base.Cmd				=	E_GrFsNvtIoRaidMsgLogAdd;
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
void	Cls_GrFsNvtIoRaid::LcSmartUpdt(void)
{
	// local -------------------
		__u8	Tv_DskIdx;
	// code --------------------
		m_SmartTimer ++;
		if(E_GrFsNvtIoRaidSmartScanPeriod <= m_SmartTimer)
		{
			for(Tv_DskIdx=0; Tv_DskIdx < E_GrFsNvtMaxDiskCnt; Tv_DskIdx++)
			{
				if(NULL != V_GrFsNvtEnv.DskTbl[Tv_DskIdx])
				{
					((Cls_GrFsNvtHdd*)V_GrFsNvtEnv.DskTbl[Tv_DskIdx])->SmartRead(&m_SmartTbl[Tv_DskIdx]);
				}
			}
			m_SmartTimer	=	0;
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtIoRaid::DiskTemperatureGet(__u8 A_DskIdx)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if(E_GrFsNvtMaxDiskCnt > A_DskIdx)
		{
			Tv_Result	=	m_SmartTbl[A_DskIdx].Temp;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrFsNvtIoRaid::ImmRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd)
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
__u64	Cls_GrFsNvtIoRaid::ReqRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd)
{
	// local -------------------
		__u64	Tv_Result;
		St_GrFsNvtIoRaidMsgRecSizeEstm	Tv_Msg;
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
					Tv_Msg.Base.MsgHd.Cmd	=	m_MsgMng;
					Tv_Msg.Base.Cmd				=	E_GrFsNvtIoRaidMsgRecSizeEstm;
					Tv_Msg.Group					=	A_GroupType;
					Tv_Msg.ChMap					=	A_ChMap;
					Tv_Msg.TimeSt					=	A_TimeSt;
					Tv_Msg.TimeEd					=	A_TimeEd;
					Tv_Msg.PtrRslt				=	&Tv_Result;

					m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				}
				else
				{
					DbgMsgPrint( "Cls_GrFsNvtIoRaid::ReqRecSizeEstmate - bad time range.\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrFsNvtIoRaid::ReqRecSizeEstmate - channel not selected.\n" );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(E_GrFsNvtIoRaidModeFormat == m_RunMode)
		{
			LcBkgFormat();
			Tv_Result	=	TRUE;
		}
		else if (E_GrFsNvtIoRaidModeNormal == m_RunMode)
		{
			// cache process
			if(NULL != m_GrupTbl[0].Obj)
			{
				if(m_GrupTbl[0].Obj->RtlCchDataRsvProc())
				{
					Tv_Result	=	TRUE;
				}
			}

			// log process
			if (LcBkgLogGet())
			{
				Tv_Result	=	TRUE;
#ifdef E_GrFsNvtIoRaidChkLogSeek
				m_DbgIsLogWork	=	TRUE;
#endif
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ImmAtDelTimeSet( __u32 A_Sec )
{
		m_AtDelStep		=	A_Sec;
		m_AtDelTimer	=	E_GrFsNvtIoRaidAutoDelPeriod;

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ReqAtDelTimeSet(__u32 A_Sec)
{
	// local -------------------
		St_GrFsNvtIoRaidMsgAtDelTimeSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgMng;
		Tv_Msg.Base.Cmd				=	E_GrFsNvtIoRaidMsgAtDelTimeSet;
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
Def_GrErrCode	Cls_GrFsNvtIoRaid::ReqGroupDiskFormat(__u8 A_GroupMap, BOOL8 A_IsPost)
{
	// local -------------------
		Def_GrErrCode	Tv_ErrCode;
		St_GrFsNvtIoRaidMsgGroupFormat	Tv_Msg;
	// code --------------------
		Tv_ErrCode	=	E_GrErrBusy;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgGroupFormat;
		Tv_Msg.GroupMap				=	A_GroupMap;

		if(A_IsPost)
		{
			Tv_Msg.PtrRslt					=	NULL;
			Tv_ErrCode	=	m_TaskSvr->MsgPost((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		}
		else
		{
			Tv_Msg.PtrRslt					=	&Tv_ErrCode;
			m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		}

		return	Tv_ErrCode;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ImmReadFwByTime(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
	// code --------------------
		if (E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrPermisDenied;
		}

		if(E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrBusy;
		}
		
		if(E_GrDvrRecDevMirror < A_SubType)
		{
			return	E_GrErrBadParam;
		}

		if(NULL == m_GrupTbl[A_SubType].Obj)
		{
			return	E_GrErrBadParam;
		}

		return	m_GrupTbl[A_SubType].Obj->ReadFwByTime(A_PtrCtrl, A_Time);

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ImmReadFwByHndl(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl)
{
	// local -------------------
	// code --------------------
		if (E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrPermisDenied;
		}

		if(E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrBusy;
		}

		if(E_GrDvrRecDevMirror < A_SubType)
		{
			return	E_GrErrBadParam;
		}

		if(NULL == m_GrupTbl[A_SubType].Obj)
		{
			return	E_GrErrBadParam;
		}

		return	m_GrupTbl[A_SubType].Obj->ReadFwByHndl(A_PtrCtrl, A_Hndl);

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ImmReadBwByTime(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
	// code --------------------
		if (E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrPermisDenied;
		}

		if(E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrBusy;
		}

		if(E_GrDvrRecDevMirror < A_SubType)
		{
			return	E_GrErrBadParam;
		}

		if(NULL == m_GrupTbl[A_SubType].Obj)
		{
			return	E_GrErrBadParam;
		}

		return	m_GrupTbl[A_SubType].Obj->ReadBwByTime(A_PtrCtrl, A_Time);

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ImmRecBufAdd(iovec* A_PtrIovs, __u32 A_Cnt)
{
	// local -------------------
	// code --------------------
		if (E_GrFsNvtIoRaidModeNormal == m_RunMode)
		{
			// check exist main
			if (NULL != m_GrupTbl[E_GrDvrRecDevNoraml].Obj)
			{
				m_GrupTbl[E_GrDvrRecDevNoraml].Obj->RecBufAdd(A_PtrIovs, A_Cnt);
			}

			// check exist mirror
			if (NULL != m_GrupTbl[E_GrDvrRecDevMirror].Obj)
			{
				m_GrupTbl[E_GrDvrRecDevMirror].Obj->RecBufAdd(A_PtrIovs, A_Cnt);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcGroupRecFlush(void)
{
	// local -------------------
		__u32	Tv_GrpType;
	// code --------------------
		for(Tv_GrpType = 0; Tv_GrpType < E_GrFsNvtIoRaidGroupCnt; Tv_GrpType++)
		{
			if(NULL != m_GrupTbl[Tv_GrpType].Obj)
			{
				m_GrupTbl[Tv_GrpType].Obj->RecFlush();
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcTmrRecFlush(void)
{
	// local -------------------
	// code --------------------
		m_RflushTimer ++;
		if(E_GrFsNvtIoRaidRecFlushPeriod <= m_RflushTimer)
		{
			LcGroupRecFlush();

			// update group info
			LcGroupInfoUpdate();

			m_RflushTimer	=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcGroupInfoUpdate(void)
{
	// local -------------------
		__u8	Tv_GrpIdx;
	// code --------------------
		for( Tv_GrpIdx = 0; Tv_GrpIdx < E_GrFsNvtIoRaidGroupCnt; Tv_GrpIdx++)
		{
			if(NULL != m_GrupTbl[Tv_GrpIdx].Obj)
			{
				m_GrupTbl[Tv_GrpIdx].Stat	=	m_GrupTbl[Tv_GrpIdx].Obj->GroupInfoGet(&m_GrupTbl[Tv_GrpIdx].SizeTot, &m_GrupTbl[Tv_GrpIdx].SizeRec, TRUE);
			}
			else
			{
				m_GrupTbl[Tv_GrpIdx].SizeTot	=	0;
				m_GrupTbl[Tv_GrpIdx].SizeRec	=	0;
				m_GrupTbl[Tv_GrpIdx].Stat			=	E_GrDvrRecStatNotexist;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcBkgFormat(void)
{
	// local -------------------
		__u8	Tv_DskMask;
		__u32	Tv_WkIdx;
		__u32	Tv_PrgsBlk;
		//__u32	Tv_BlkOfs;

		Cls_GrFsNvtHdd*	Tv_ObjDsk;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
		Ptr_GrFsNvtDayInfo	Tv_DayTbl;
		St_GrTaskMsgDiskFmtPrgs	Tv_MsgPrg;
		St_GrTaskMsgDiskFmtCpl	Tv_MsgCpl;
	// code --------------------

		// check find disk
		if(0 == m_Format.BlkCnt)
		{
			// init
			m_Format.BlkPrgs		=	0;

			// find disk
			Tv_DskMask		=	(1 << m_Format.DskIdx);
			while (E_GrFsNvtMaxDiskCnt > m_Format.DskIdx)
			{
				// check match
				if (0 != (Tv_DskMask & m_Format.DskMap))
				{
					// found , check exist disk
					if (NULL != V_GrFsNvtEnv.DskTbl[m_Format.DskIdx])
					{
						// get disk header table
						Tv_ObjDsk	=	(Cls_GrFsNvtHdd*)V_GrFsNvtEnv.DskTbl[m_Format.DskIdx];
						Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)Tv_ObjDsk->BlkInfoTableGet(&m_Format.BlkCnt, TRUE);
						if((NULL != Tv_PtrBi) && (1 < m_Format.BlkCnt))
						{
							// DO block information
							// clear
							GrDumyZeroMem(Tv_PtrBi, sizeof(St_GrFsNvtBlkInfoBase) * m_Format.BlkCnt);
							// build MIB block
							Tv_PtrBi[E_GrFsNvtBlkIdxMib].Mib.Fcc			=	E_GrFsNvtFcc;
							Tv_PtrBi[E_GrFsNvtBlkIdxMib].Mib.DiskId		=	V_GrFsNvtEnv.DiskBaseId;
							Tv_PtrBi[E_GrFsNvtBlkIdxMib].Mib.BlkCnt		=	m_Format.BlkCnt;
							Tv_PtrBi[E_GrFsNvtBlkIdxMib].Mib.FsVer		=	E_GrFsNvtVer;
							Tv_PtrBi[E_GrFsNvtBlkIdxMib].Mib.SeqLast	=	0;
							Tv_PtrBi[E_GrFsNvtBlkIdxMib].Mib.SeqOvwt	=	0;

							// write db
							for( Tv_WkIdx = 1; Tv_WkIdx < m_Format.BlkCnt; Tv_WkIdx++)
							{
								Tv_PtrBi[Tv_WkIdx].Db.OfsLastLog	=	-1;
							}

							Tv_ObjDsk->BlkInfoWriteAll();

							// date information clear
							Tv_DayTbl	=	Tv_ObjDsk->DayInfoTableGet();
							GrDumyZeroMem(Tv_DayTbl, E_GrFsNvtHddDayInfoBufSize);
							Tv_ObjDsk->DayInfoWrite();

							// update disk info
							Tv_ObjDsk->StatusUpdate();

							// no minute clear mode only
							m_Format.DskPrgs ++;
							m_Format.DskIdx ++;
							m_Format.BlkCnt	=	0;
							break;
						}
					}
					m_Format.DskPrgs ++;	// bad disk skip
				}
				// next
				Tv_DskMask	=	Tv_DskMask << 1;
				m_Format.DskIdx ++;
				m_Format.BlkCnt	=	0;
			}	//	while (E_GrFsNvtMaxDiskCnt > m_Format.DskPrgs)
		}

#if 0
		if(0 != m_Format.BlkCnt)
		{
			// do minute table format
			if(NULL != V_GrFsNvtEnv.DskTbl[m_Format.DskIdx])
			{
				Tv_ObjDsk	=	(Cls_GrFsNvtHdd*)V_GrFsNvtEnv.DskTbl[m_Format.DskIdx];
				// fill day map
				Tv_BlkOfs	=	E_GrFsNvtBlkOfsMinuteTbl + (m_Format.Prgs * E_GrFsNvtMinuteTblSizeOfDay);
				if(E_GrErrNone == Tv_ObjDsk->BlkFillZeroByOfs(E_GrFsNvtBlkIdxMib, Tv_BlkOfs, (E_GrFsNvtMinuteTblSizeOfDay * 8)))
				{
					m_Format.Prgs	=	m_Format.Prgs + 8;
					// check disk finish
					if(m_Format.Prgs >= E_GrFsNvtDayInfoCntOfDisk)
					{
						// set next
						m_Format.BlkCnt		=	0;
						m_Format.Prgs			=	0;
						m_Format.DskIdx ++;
						m_Format.DskPrgs ++;
					}
				}
				else
				{
					// go next
					m_Format.BlkCnt		=	0;
					m_Format.Prgs			=	0;
					m_Format.DskIdx ++;
					m_Format.DskPrgs ++;
				}
			}
			// update progress
			Tv_MsgPrg.MsgHd.Cmd		=	E_GrTaskMsgDiskFormatPrgs;
			Tv_MsgPrg.DiskMap			=	m_Format.DskMap;
			Tv_MsgPrg.GroupType		=	m_Format.GroupType;

			Tv_PrgsBlk	=	0;
			if(0 != m_Format.Prgs)
			{
				Tv_PrgsBlk	=	(m_Format.Prgs * 100) / E_GrFsNvtDayInfoCntOfDisk;
			}

			Tv_PrgsDsk	=	((((__u32)m_Format.DskPrgs * 100) + Tv_PrgsBlk) * 100) / ((__u32)m_Format.DskCnt * 100);

			Tv_MsgPrg.Prgs	=	(__u8)Tv_PrgsDsk;

			GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgPrg, sizeof(Tv_MsgPrg));
		}
#endif

		// update progress
		if((m_Format.DskCnt <= m_Format.DskPrgs) || (E_GrFsNvtMaxDiskCnt <= m_Format.DskIdx))
		{
			m_Format.TotPrgs	=	100;
		}
		else
		{
			// calcupate progress block
			Tv_PrgsBlk	=	0;
			if (0 != m_Format.BlkCnt)
			{
				Tv_PrgsBlk	=	(m_Format.BlkPrgs << 4) / m_Format.BlkCnt;
			}
			// calculate total progress
			m_Format.TotPrgs	=	(((m_Format.DskPrgs << 4) + Tv_PrgsBlk) * 100) / (m_Format.DskCnt << 4);
		}
		// update progress message
		Tv_MsgPrg.MsgHd.Cmd		=	E_GrTaskMsgDiskFormatPrgs;
		Tv_MsgPrg.DiskMap			=	m_Format.DskMap;
		Tv_MsgPrg.GroupMap		=	m_Format.GroupMap;
		Tv_MsgPrg.Prgs				=	m_Format.TotPrgs;

		GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgPrg, sizeof(Tv_MsgPrg));

		// check finish
		if((m_Format.DskCnt <= m_Format.DskPrgs) || (E_GrFsNvtMaxDiskCnt <= m_Format.DskIdx))
		{
			// finish
			// regroup
			LcGroupMakeByMap(m_Format.GroupMap);
			LcGroupInfoUpdate();

			// notify format completed
			Tv_MsgCpl.MsgHd.Cmd	=	E_GrTaskMsgDiskFormatCpl;
			Tv_MsgCpl.DiskMap		=	m_Format.DskMap;
			Tv_MsgCpl.GroupMap	=	m_Format.GroupMap;
			Tv_MsgCpl.IsSuccess	=	TRUE;
			Tv_MsgCpl.ErrCode		=	E_GrErrNone;
			GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgCpl, sizeof(Tv_MsgCpl));

			// change mode
			m_RunMode		=	E_GrFsNvtIoRaidModeNormal;
		}


}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ReqRecBufAdd(iovec* A_PtrIovs, __u32 A_Cnt)
{
	// local -------------------
		St_GrFsNvtIoRaidMsgRecBufAdd	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgRecBufAdd;
		Tv_Msg.PtrIovs					=	A_PtrIovs;
		Tv_Msg.Cnt							=	A_Cnt;

		if(!m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg,sizeof(Tv_Msg)))
		{
			DbgMsgPrint("Cls_GrFsNvtIoRaid::ReqRecBufAdd - request fail!\n");
		}

}
//--------------------------------------------------------------------
__s32	Cls_GrFsNvtIoRaid::ImmLogGetOpen(__u8 A_Group, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_MaxCnt,
	__u32 A_ChMask, __u64 A_TypeMask, Ptr_GrDvrLogItm A_PtrRtBuf, BOOL8 A_IsFrmMode)
{
	// local -------------------
		__s32	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		// check parametter
		if ((NULL == A_PtrRtBuf) || (0 == A_ChMask) || (0 == A_TypeMask) || (0 == A_MaxCnt) || (E_GrFsNvtIoRaidGroupCnt <= A_Group))
		{
			DbgMsgPrint("Cls_GrFsNvtIoRaid::ImmLogGetOpen - bad parametter input\n");
			return	-1;
		}

		if (E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			// now foramtting
			DbgMsgPrint("Cls_GrFsNvtIoRaid::ImmLogGetOpen - now formatting..\n");
			return	-1;
		}

		// check mode and max count
		if(A_IsFrmMode && (E_GrFsNvtFrmLogPoolItemCnt > A_MaxCnt))
		{
			// not enough buffer size
			DbgMsgPrint("Cls_GrFsNvtIoRaid::ImmLogGetOpen - frame mode not enough buffer size %d\n", A_MaxCnt);
			return -1;
		}

		// find empty slot
		Tv_Result	=	-1;
		for (Tv_WkIdx = 0; Tv_WkIdx < E_GrFsNvtIoRaidLogGetCtlMaxCnt; Tv_WkIdx++)
		{
			if (!m_LogGetCtls[Tv_WkIdx].IsOpen)
			{
				// found
				Tv_Result	=	(__s32)Tv_WkIdx;
				break;
			}
		}

		if (0 > Tv_Result)
		{
			DbgMsgPrint("Cls_GrFsNvtIoRaid::ImmLogGetOpen - not enough slot!\n");
			return	Tv_Result;
		}

		// initialize slot
		m_LogGetCtls[Tv_Result].PtrRtBuf			=	A_PtrRtBuf;
		m_LogGetCtls[Tv_Result].IsFrmUnit			= A_IsFrmMode;
		m_LogGetCtls[Tv_Result].ChMask				=	A_ChMask;
		m_LogGetCtls[Tv_Result].DevSub				=	A_Group;
		m_LogGetCtls[Tv_Result].MaxCnt				=	A_MaxCnt;
		m_LogGetCtls[Tv_Result].TimeSt				=	A_TimeSt;
		m_LogGetCtls[Tv_Result].TimeEd				=	A_TimeEd;
		m_LogGetCtls[Tv_Result].TypeMask			=	A_TypeMask;

		m_LogGetCtls[Tv_Result].DskIdx				=	0;
		m_LogGetCtls[Tv_Result].LogOfs				=	-1;
		m_LogGetCtls[Tv_Result].BlkIdx				=	1;

		m_LogGetCtls[Tv_Result].InCnt					=	0;
		m_LogGetCtls[Tv_Result].IsDone				=	FALSE;
		m_LogGetCtls[Tv_Result].IsOpen				=	TRUE;
		m_LogGetCtls[Tv_Result].IsWait				=	FALSE;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcLogGetNotifyDone(__u32 A_Idx)
{
	// local -------------------
		St_GrTaskMsgFsLogGetDone	Tv_Msg;
	// code --------------------
		if (!m_LogGetCtls[A_Idx].IsOpen)
		{
			return;
		}
	
#ifdef E_GrFsNvtIoRaidChkLogSeek
		DbgMsgPrint("[DBG-DvrFsNvt] log seek done.\n");
#endif

		Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgFsLogGetDone;
		Tv_Msg.Hndl				=	A_Idx;

		GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::LcBkgLogGet(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
		Cls_GrFsNvtRgrp*	Tv_ObjGrp;
	// code --------------------
		Tv_Result	=	FALSE;

		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrFsNvtIoRaidLogGetCtlMaxCnt; Tv_WkIdx++)
		{
			// check rtl opened
			if(m_LogGetCtls[m_LogGetCidx].IsOpen && (!m_LogGetCtls[m_LogGetCidx].IsDone))
			{
				// check run able
				if((!m_LogGetCtls[m_LogGetCidx].IsFrmUnit) || (m_LogGetCtls[m_LogGetCidx].IsFrmUnit && (!m_LogGetCtls[m_LogGetCidx].IsWait)))
				{
					// run
					Tv_ObjGrp	=	m_GrupTbl[m_LogGetCtls[m_LogGetCidx].DevSub].Obj;
					if(NULL == Tv_ObjGrp)
					{
						// do finish - mybe changed file system setting
						m_LogGetCtls[m_LogGetCidx].IsDone	=	TRUE;
						if(!m_LogGetCtls[m_LogGetCidx].IsFrmUnit)
						{
							//DbgMsgPrint("[LOG] %d out!\n", m_LogGetCtls[m_LogGetCidx].InCnt);
							// do sort
							LcLogGetSortOutput(m_LogGetCidx);
							LcLogGetNotifyDone(m_LogGetCidx);
						}
					}
					else
					{
						// run rtl
						Tv_ObjGrp->RtlLogReadProc(&m_LogGetCtls[m_LogGetCidx], &m_LogGetFrmBuf);
						// check done
						if(m_LogGetCtls[m_LogGetCidx].IsDone && (!m_LogGetCtls[m_LogGetCidx].IsFrmUnit))
						{
							//DbgMsgPrint("[LOG] %d out!\n", m_LogGetCtls[m_LogGetCidx].InCnt);
							// do sort
							LcLogGetSortOutput(m_LogGetCidx);
							LcLogGetNotifyDone(m_LogGetCidx);
						}
						Tv_Result	=	TRUE;
					}
				}
			}
			// next
			m_LogGetCidx ++;
			if(E_GrFsNvtIoRaidLogGetCtlMaxCnt <= m_LogGetCidx)
			{
				m_LogGetCidx	=	0;
			}

			if(Tv_Result)
			{
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::ImmLogGetChkDone(__s32 A_Hndl, __u32* A_PtrRtCnt)
{
	// local -------------------
	// code --------------------
		*A_PtrRtCnt		=	0;

		// check handle range
		if ((0 > A_Hndl) || (E_GrFsNvtIoRaidLogGetCtlMaxCnt <= A_Hndl))
		{
			return	TRUE;
		}

		// check opened
		if (!m_LogGetCtls[A_Hndl].IsOpen)
		{
			return	TRUE;
		}

		// update result
		*A_PtrRtCnt	=	m_LogGetCtls[A_Hndl].InCnt;
		return	m_LogGetCtls[A_Hndl].IsDone;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ImmLogGetFrameModeCheck(__s32 A_Hndl, __u32* A_PtrRtCnt)
{
	// local -------------------
	// code --------------------

		*A_PtrRtCnt		=	0;

		// check handle range
		if((0 > A_Hndl) || (E_GrFsNvtIoRaidLogGetCtlMaxCnt <= A_Hndl))
		{
			DbgMsgPrint("Cls_GrFsNvtIoRaid::ImmLogGetPartCheck - bad handle %d input!\n", A_Hndl);
			return	E_GrErrBadParam;
		}

		// check opened
		if(!m_LogGetCtls[A_Hndl].IsOpen)
		{
			DbgMsgPrint("Cls_GrFsNvtIoRaid::ImmLogGetPartCheck - handle %d not opened!\n", A_Hndl);
			return	E_GrErrBadParam;
		}

		// chceck parted mode
		if(!m_LogGetCtls[A_Hndl].IsFrmUnit)
		{
			DbgMsgPrint("Cls_GrFsNvtIoRaid::ImmLogGetPartCheck - handle %d not frame mode!\n", A_Hndl);
			return	E_GrErrBadParam;
		}

		// check wait
		if(m_LogGetCtls[A_Hndl].IsWait)
		{
			*A_PtrRtCnt	=	m_LogGetCtls[A_Hndl].InCnt;
			return	E_GrErrNone;
		}

		// check finished
		if(m_LogGetCtls[A_Hndl].IsDone)
		{
			*A_PtrRtCnt	=	m_LogGetCtls[A_Hndl].InCnt;
			return	E_GrErrEndOfData;
		}

		return	E_GrErrBusy;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::ImmLogGetFrameModeResume(__s32 A_Hndl)
{
	// local -------------------
	// code --------------------
		// check handle range
		if((0 > A_Hndl) || (E_GrFsNvtIoRaidLogGetCtlMaxCnt <= A_Hndl))
		{
			return	FALSE;
		}

		// check opened
		if(!m_LogGetCtls[A_Hndl].IsOpen)
		{
			return	FALSE;
		}

		// chceck parted mode
		if(!m_LogGetCtls[A_Hndl].IsFrmUnit)
		{
			return	FALSE;
		}

		// check finished
		if(m_LogGetCtls[A_Hndl].IsDone)
		{
			return	FALSE;
		}

		// check not wait mode
		if(!m_LogGetCtls[A_Hndl].IsWait)
		{
			return	FALSE;
		}

		// reset in count
		m_LogGetCtls[A_Hndl].InCnt		=	0;
		m_LogGetCtls[A_Hndl].IsWait		=	FALSE;

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ImmLogGetClose(__s32 A_Hndl)
{
	// local -------------------
	// code --------------------
		// check handle range
		if ((0 > A_Hndl) || (E_GrFsNvtIoRaidLogGetCtlMaxCnt <= A_Hndl))
		{
			return;
		}

		m_LogGetCtls[A_Hndl].IsOpen	=	FALSE;

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcLogGetCloseAll(void)
{
	// local -------------------
		__u32	Tv_CtlIdx;
	// code --------------------
		for (Tv_CtlIdx = 0; Tv_CtlIdx < E_GrFsNvtIoRaidLogGetCtlMaxCnt; Tv_CtlIdx++)
		{
			if (m_LogGetCtls[Tv_CtlIdx].IsOpen && (!m_LogGetCtls[Tv_CtlIdx].IsDone))
			{
				m_LogGetCtls[Tv_CtlIdx].IsDone		=	TRUE;
				LcLogGetNotifyDone(Tv_CtlIdx);
			}
			m_LogGetCtls[Tv_CtlIdx].IsOpen		=	FALSE;
		}
}
//--------------------------------------------------------------------
__s32	Cls_GrFsNvtIoRaid::ReqLogGetOpen(__u8 A_Group, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_MaxCnt,
	__u32 A_ChMask, __u64 A_TypeMask, Ptr_GrDvrLogItm A_PtrRtBuf, BOOL8 A_IsFrmMode)
{
	// local -------------------
		__s32		Tv_Result;
		St_GrFsNvtIoRaidMsgLogGetOpen	Tv_Msg;
	// code --------------------
		Tv_Result	=	-1;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgMng;
		Tv_Msg.Base.Cmd				=	E_GrFsNvtIoRaidMsgLogGetOpen;
		Tv_Msg.PtrRslt				=	&Tv_Result;
		Tv_Msg.Group					=	A_Group;
		Tv_Msg.IsFrmMode			=	A_IsFrmMode;
		Tv_Msg.TimeSt					=	A_TimeSt;
		Tv_Msg.TimeEd					=	A_TimeEd;
		Tv_Msg.MaxCnt					=	A_MaxCnt;
		Tv_Msg.ChMask					=	A_ChMask;
		Tv_Msg.TypeMask				=	A_TypeMask;
		Tv_Msg.PtrRtBuf				=	A_PtrRtBuf;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

#ifdef E_GrFsNvtIoRaidChkLogSeek
		DbgMsgPrint("[DBG] log req grp:%d , md:%d hndl:%d \n", A_Group,A_Mode,Tv_Result );
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::ReqLogGetChkDone(__s32 A_Hndl, __u32* A_PtrRtCnt)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrFsNvtIoRaidMsgLogGetChkDone	Tv_Msg;
	// code --------------------
		Tv_Result		=	TRUE;
		*A_PtrRtCnt	=	0;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgMng;
		Tv_Msg.Base.Cmd				=	E_GrFsNvtIoRaidMsgLogGetChkDone;
		Tv_Msg.PtrRslt				=	&Tv_Result;
		Tv_Msg.Hndl						=	A_Hndl;
		Tv_Msg.PtrRtCnt				=	A_PtrRtCnt;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ReqLogGetClose(__s32 A_Hndl)
{
	// local -------------------
		St_GrFsNvtIoRaidMsgLogGetClose	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgMng;
		Tv_Msg.Base.Cmd				=	E_GrFsNvtIoRaidMsgLogGetClose;
		Tv_Msg.Hndl						=	A_Hndl;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ReqLogGetFrameModeCheck(__s32 A_Hndl, __u32* A_PtrRtCnt)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrFsNvtIoRaidMsgLogGetPartCheck	Tv_Msg;
	// code --------------------
		Tv_Result		=	E_GrErrBusy;
		*A_PtrRtCnt	=	0;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgMng;
		Tv_Msg.Base.Cmd				=	E_GrFsNvtIoRaidMsgLogGetFrameModeCheck;
		Tv_Msg.PtrRslt				=	&Tv_Result;
		Tv_Msg.Hndl						=	A_Hndl;
		Tv_Msg.PtrRtCnt				=	A_PtrRtCnt;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::ReqLogGetFrameModeResume(__s32 A_Hndl)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrFsNvtIoRaidMsgLogGetPartResume	Tv_Msg;
	// code --------------------
		Tv_Result		=	TRUE;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgMng;
		Tv_Msg.Base.Cmd				=	E_GrFsNvtIoRaidMsgLogGetFrameModeResume;
		Tv_Msg.PtrRslt				=	&Tv_Result;
		Tv_Msg.Hndl						=	A_Hndl;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcTmrAutoDelete(void)
{
	// local -------------------
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
	// code --------------------
		m_AtDelTimer ++;
		if(E_GrFsNvtIoRaidAutoDelPeriod <= m_AtDelTimer)
		{
			// do auto delete
			if(E_GrTimeSecOfDay <= m_AtDelStep)
			{
				// calculate base time
				m_AtDelBaseTm	=	GrTimeGet(&Tv_Mili, &Tv_IsSm);

				if(m_AtDelBaseTm > m_AtDelStep)
				{
					m_AtDelBaseTm	=	m_AtDelBaseTm - m_AtDelStep;

					// do delete - normal only
					if(NULL != m_GrupTbl[0].Obj)
					{
						m_GrupTbl[0].Obj->DeleteStaleData(m_AtDelBaseTm);
					}
				}
			}

			m_AtDelTimer		=	0;
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtIoRaid::ImmDiskInstMapGet(__u8* A_PtrRtBadMap)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_BadMap;
		__u8	Tv_DskMask;
		__u32	Tv_DskIdx;
	// code --------------------
		Tv_Result	=	0;
		Tv_BadMap	=	0;

		Tv_DskMask	=	1;
		for(Tv_DskIdx = 0; Tv_DskIdx < V_GrFsNvtEnv.DiskMaxCnt; Tv_DskIdx++)
		{
			if(NULL != V_GrFsNvtEnv.DskTbl[Tv_DskIdx])
			{
				Tv_Result	=	Tv_Result | Tv_DskMask;
				if(E_GrDvrRecStatBad == ((Cls_GrFsNvtHdd*)V_GrFsNvtEnv.DskTbl[Tv_DskIdx])->DiskStatGet())
				{
					Tv_BadMap	=	Tv_BadMap | Tv_DskMask;
				}
			}
			// next
			Tv_DskMask	=	Tv_DskMask << 1;
		}

		*A_PtrRtBadMap	=	Tv_BadMap;
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrFsNvtIoRaid::ReqDiskInstMapGet(__u8* A_PtrRtBadMap)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrFsNvtIoRaidMsgDiskInstMapGet	Tv_Msg;
	// code --------------------
		Tv_Result				=	0;
		*A_PtrRtBadMap	=	0;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgMng;
		Tv_Msg.Base.Cmd				=	E_GrFsNvtIoRaidMsgDiskInstMapGet;
		Tv_Msg.PtrRslt				=	&Tv_Result;
		Tv_Msg.PtrRtBadMap		=	A_PtrRtBadMap;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::ImmDiskSmartRead(__u8 A_DevIdx, Ptr_GrFsNvtSmart A_PtrRtData)
{
	// local -------------------

	// code --------------------
#if 0
		// check valid device index
		if(E_GrFsNvtMaxDiskCnt <= A_DevIdx)
		{
			return	FALSE;
		}

		return	((Cls_GrFsNvtHdd*)V_GrFsNvtEnv.DskTbl[A_DevIdx])->SmartRead(A_PtrRtData);
#else
	if(E_GrFsNvtMaxDiskCnt <= A_DevIdx)
	{
		return	FALSE;
	}

	GrDumyCopyMem(A_PtrRtData, &m_SmartTbl[A_DevIdx], sizeof(St_GrFsNvtSmart));

	return	TRUE;
#endif

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoRaid::ReqDiskSmartRead(__u8 A_DevIdx, Ptr_GrFsNvtSmart A_PtrRtData)
{
#if 0
	// local -------------------
		BOOL8	Tv_Result;
		St_GrFsNvtIoRaidMsgDiskSmartRead	Tv_Msg;
	// code --------------------
		
		Tv_Result				=	FALSE;

		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgMng;
		Tv_Msg.Base.Cmd				=	E_GrFsNvtIoRaidMsgDiskSmartRead;
		Tv_Msg.PtrRslt				=	&Tv_Result;
		Tv_Msg.DevIdx					=	A_DevIdx;
		Tv_Msg.PtrRtData			=	A_PtrRtData;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;

#else
		return	ImmDiskSmartRead(A_DevIdx,A_PtrRtData);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ImmGroupMakeFull(__u8 A_GroupType)
{
	// local -------------------
		__u32	Tv_DskMask;
		__u32	Tv_DskIdx;
		__u32	Tv_BiCnt;
		__u32	Tv_BiIdx;
		Def_GrTime	Tv_Time;
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
		__u32	Tv_SeqIdNew;
		Cls_GrFsNvtHdd*	Tv_ObjDsk;
		Ptr_GrFsNvtBlkInfoBase	Tv_PtrBi;
	// code --------------------
		if(E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return;
		}

		if(0 == m_GrupTbl[A_GroupType].DskMap)
		{
			return;
		}

		// stop group
		if(NULL != m_GrupTbl[A_GroupType].Obj)
		{
			delete	m_GrupTbl[A_GroupType].Obj;
			m_GrupTbl[A_GroupType].Obj	=	NULL;
		}

		Tv_SeqIdNew	=	1;

		// build full
		Tv_DskMask		=	1;
		for( Tv_DskIdx = 0; Tv_DskIdx < E_GrFsNvtMaxDiskCnt; Tv_DskIdx++)
		{
			// check exist disk
			if(0 == (Tv_DskMask & m_GrupTbl[A_GroupType].DskMap))
			{
				Tv_DskMask		=	Tv_DskMask << 1;
				continue;
			}
			if(NULL == V_GrFsNvtEnv.DskTbl[Tv_DskIdx])
			{
				Tv_DskMask		=	Tv_DskMask << 1;
				continue;
			}

			// get disk object
			Tv_ObjDsk	=	(Cls_GrFsNvtHdd*)V_GrFsNvtEnv.DskTbl[Tv_DskIdx];
			Tv_PtrBi	=	(Ptr_GrFsNvtBlkInfoBase)Tv_ObjDsk->BlkInfoTableGet(&Tv_BiCnt, FALSE);
			if((NULL == Tv_PtrBi) || (0 == Tv_BiCnt))
			{
				Tv_DskMask		=	Tv_DskMask << 1;
				continue;
			}

			Tv_Time	=	GrTimeGet(&Tv_Mili, &Tv_IsSm);
			Tv_Time	=	Tv_Time - (2 * E_GrTimeSecOfYear);
			
			// check empty block
			for(Tv_BiIdx = 1; Tv_BiIdx < Tv_BiCnt; Tv_BiIdx++)
			{
				if(E_GrFsNvtBlkStatBad == Tv_PtrBi[Tv_BiIdx].Db.BlkStat)
				{
					continue;
				}
				if((E_GrFsNvtBlkStatFull == Tv_PtrBi[Tv_BiIdx].Db.BlkStat) || (0 != Tv_PtrBi[Tv_BiIdx].Db.InSize))
				{
					// patch sequence id
					Tv_PtrBi[Tv_BiIdx].Db.SeqId	=	Tv_PtrBi[Tv_BiIdx].Db.SeqId + Tv_BiCnt;
					continue;
				}

				Tv_PtrBi[Tv_BiIdx].Db.SeqId		=	Tv_SeqIdNew;
				Tv_PtrBi[Tv_BiIdx].Db.InSize	=	0x100000;
				Tv_PtrBi[Tv_BiIdx].Db.BlkStat	=	E_GrFsNvtBlkStatFull;
				Tv_PtrBi[Tv_BiIdx].Db.TimeRng.TimeSt	=	Tv_Time;
				Tv_PtrBi[Tv_BiIdx].Db.TimeRng.TimeEd	=	Tv_Time + E_GrTimeSecOfHour - 1;
				Tv_SeqIdNew ++;

				// next
				Tv_Time	=	Tv_Time + E_GrTimeSecOfHour;
			}

			Tv_ObjDsk->BlkInfoWriteAll();

			// next disk
			Tv_DskMask		=	Tv_DskMask << 1;
		}

		// regroup
		DbgMsgPrint("Cls_GrFsNvtIoRaid::ImmGroupMakeFull - new Cls_GrFsNvtRgrp\n");
		m_GrupTbl[A_GroupType].Obj	=	(Cls_GrFsNvtRgrp*) new Cls_GrFsNvtRgrp((__u8)A_GroupType,
			m_GrupTbl[A_GroupType].DskMap, m_GrupTbl[A_GroupType].IsOvwt, m_GrupTbl[A_GroupType].IsCchRsvAble);

		LcGroupInfoUpdate();

		DbgMsgPrint("[TEST] make full finished!\n");

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::ReqGroupMakeFull(__u8 A_GroupType)
{
	// local -------------------
		St_GrFsNvtIoRaidMsgGroupMakeFull	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.MsgHd.Cmd	=	m_MsgMng;
		Tv_Msg.Base.Cmd				=	E_GrFsNvtIoRaidMsgGroupMakeFull;
		Tv_Msg.GroupType			=	A_GroupType;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ImmSctlClose(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
		if (E_GrFsNvtIoRaidModeNormal != m_RunMode)
		{
			return	E_GrErrPermisDenied;
		}

		if(E_GrDvrRecDevMirror < A_SubType)
		{
			return	E_GrErrBadParam;
		}

		if(NULL == m_GrupTbl[A_SubType].Obj)
		{
			return	E_GrErrBadParam;
		}

		return	m_GrupTbl[A_SubType].Obj->SctlFree(A_PtrCtrl);
		
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcFormatFailMsgBradcast(__u8 A_GrpMap, __u8 A_DskMap, Def_GrErrCode A_ErrCode)
{
	// local -------------------
		St_GrTaskMsgDiskFmtCpl	Tv_MsgCpl;
	// code --------------------
		// notify format completed
		Tv_MsgCpl.MsgHd.Cmd	=	E_GrTaskMsgDiskFormatCpl;
		Tv_MsgCpl.DiskMap		=	A_DskMap;
		Tv_MsgCpl.GroupMap	=	A_GrpMap;
		Tv_MsgCpl.IsSuccess	=	FALSE;
		Tv_MsgCpl.ErrCode		=	A_ErrCode;
		GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_MsgCpl, sizeof(Tv_MsgCpl));

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcLogGetSortOutput(__u32 A_Cidx)
{
	// local -------------------
		
	// code --------------------
		// check count
		if(1 >= m_LogGetCtls[A_Cidx].InCnt)
		{
			return;
		}

		// quick sort
		LcLogQsort(m_LogGetCtls[A_Cidx].PtrRtBuf, 0, m_LogGetCtls[A_Cidx].InCnt - 1);

}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoRaid::LcLogQsort(Ptr_GrDvrLogItm A_PtrBuf, __u32 A_StIdx, __u32 A_EdIdx)
{
	// local -------------------
		__u32	Tv_PvIdx;
		__u32	Tv_LoIdx;
		__u32	Tv_HiIdx;
		__u32	Tv_NewSt;
		__u32	Tv_NewEd;
		St_GrDvrLogItm	Tv_Tmp;
	// code --------------------
		Tv_PvIdx	=	(A_StIdx + A_EdIdx) >> 1;
		Tv_LoIdx	=	A_StIdx;
		Tv_HiIdx	=	A_EdIdx;

		do
		{
			// compare lo
			while((A_PtrBuf[Tv_LoIdx].Time > A_PtrBuf[Tv_PvIdx].Time) || 
				((A_PtrBuf[Tv_LoIdx].Time == A_PtrBuf[Tv_PvIdx].Time) && (A_PtrBuf[Tv_LoIdx].Ch < A_PtrBuf[Tv_PvIdx].Ch)))
			{
				Tv_LoIdx ++;
			}
			// compare hi
			while((A_PtrBuf[Tv_HiIdx].Time < A_PtrBuf[Tv_PvIdx].Time) ||
				((A_PtrBuf[Tv_HiIdx].Time == A_PtrBuf[Tv_PvIdx].Time) && (A_PtrBuf[Tv_HiIdx].Ch > A_PtrBuf[Tv_PvIdx].Ch)))
			{
				Tv_HiIdx --;
			}

			// check need move
			if(Tv_LoIdx < Tv_HiIdx)
			{
				// do swap
				GrDumyCopyMem(&Tv_Tmp, &A_PtrBuf[Tv_LoIdx], sizeof(St_GrDvrLogItm));	// lo to tmp
				GrDumyCopyMem(&A_PtrBuf[Tv_LoIdx], &A_PtrBuf[Tv_HiIdx], sizeof(St_GrDvrLogItm));	// hi to lo
				GrDumyCopyMem(&A_PtrBuf[Tv_HiIdx], &Tv_Tmp, sizeof(St_GrDvrLogItm));	// tmp to hi
				// check move pivot
				if(Tv_PvIdx == Tv_LoIdx)
				{
					Tv_PvIdx	=	Tv_HiIdx;
				}
				else if(Tv_PvIdx == Tv_HiIdx)
				{
					Tv_PvIdx	=	Tv_LoIdx;
				}
			}

		} while(Tv_LoIdx < Tv_HiIdx);

		// recursion left
		if(2 <= (Tv_PvIdx - A_StIdx))
		{
			Tv_NewSt		=	A_StIdx;
			Tv_NewEd		=	Tv_PvIdx - 1;
			LcLogQsort(A_PtrBuf, Tv_NewSt, Tv_NewEd);
		}

		// recursion right
		if(2 <= (A_EdIdx - Tv_PvIdx))
		{
			Tv_NewSt		=	Tv_PvIdx + 1;
			Tv_NewEd		=	A_EdIdx;
			LcLogQsort(A_PtrBuf, Tv_NewSt, Tv_NewEd);
		}

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ReqSctlClose(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrFsNvtIoRaidMsgSctlClose	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrErrBusy;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgSctlClose;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.SubType					=	A_SubType;
		Tv_Msg.PtrCtrl					=	A_PtrCtrl;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ReqReadFwNext(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrFsNvtIoRaidMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrErrBusy;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgReadFwNext;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_SubType;
		Tv_Msg.PtrCtrl					=	A_PtrCtrl;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ReqReadBwNext(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrFsNvtIoRaidMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrErrBusy;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgReadBwNext;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_SubType;
		Tv_Msg.PtrCtrl					=	A_PtrCtrl;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ReqReadFwByTime(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrFsNvtIoRaidMsgReadTimePara	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrErrBusy;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgReadFwByTime;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_SubType;
		Tv_Msg.Time							=	A_Time;
		Tv_Msg.PtrCtrl					=	A_PtrCtrl;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ReqReadBwByTime(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrFsNvtIoRaidMsgReadTimePara	Tv_Msg;

	// code --------------------
		Tv_Result	=	E_GrErrBusy;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgReadBwByTime;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_SubType;
		Tv_Msg.Time							=	A_Time;
		Tv_Msg.PtrCtrl					=	A_PtrCtrl;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ReqReadFwStart(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrFsNvtIoRaidMsgReadPara	Tv_Msg;

	// code --------------------
		Tv_Result	=	E_GrErrBusy;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgReadFwStart;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_SubType;
		Tv_Msg.PtrCtrl					=	A_PtrCtrl;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoRaid::ReqReadFwByHndl(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrFsNvtIoRaidMsgReadHndlPara	Tv_Msg;

	// code --------------------
		Tv_Result	=	E_GrErrBusy;

		Tv_Msg.Base.MsgHd.Cmd		=	m_MsgMng;
		Tv_Msg.Base.Cmd					=	E_GrFsNvtIoRaidMsgReadFwByHndl;
		Tv_Msg.PtrRslt					=	&Tv_Result;
		Tv_Msg.Group						=	A_SubType;
		Tv_Msg.PtrCtrl					=	A_PtrCtrl;
		Tv_Msg.Hndl							=	A_Hndl;

		m_TaskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;

}
//--------------------------------------------------------------------
