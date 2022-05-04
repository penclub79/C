/*
product regist server

*/

//====================================================================
//include

#include <Win/Db/GrDbCtrl.h>

#include <GrDebug.h>
#include <GrError.h>

#include <GrDumyTool.h>
#include <GrTimeTool.h>

#include <Win/Db/GrDbSync.h>

//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//global var


//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrDbCtrl::Cls_GrDbCtrl(Cls_GrTaskSvr* A_TaskSvr, WCHAR* A_StrDbPath, Ptr_GrDbGrpInfo A_PtrGrpInfo) :
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		// init
		m_TskSvr				=	A_TaskSvr;
		m_MsgCmd				=	GrTaskSvrMsgCmdAlloc();

		m_TskSync				=	NULL;
		m_DbSync				=	NULL;

		GrDumyZeroMem(m_GrpInfo, sizeof(m_GrpInfo));
		GrDumyZeroMem(m_ObjGrp, sizeof(m_ObjGrp));

		GrStrWcopy(m_StrPath, A_StrDbPath);

		// set group table
		GrDumyCopyMem(m_GrpInfo, A_PtrGrpInfo, sizeof(St_GrDbGrpInfo) * E_GrDbGrpMaxCnt);

		// regist 
		m_TskSvr->RegistMsgClient(this);
		m_TskSvr->RegistTimerClient(this, E_GrDbCtrlTimerPeriod, &m_TmrSec);
		m_TskSvr->RegistBkgClient(this);

}
//--------------------------------------------------------------------
Cls_GrDbCtrl::~Cls_GrDbCtrl()
{
	// local -------------------
	// code --------------------

		ReqFinish();

		m_TskSvr->UnregistBkgClient(this);
		m_TskSvr->UnregistTimerClient(m_TmrSec);
		m_TskSvr->UnregistMsgClient(this);

		GrTaskSvrMsgCmdFree(m_MsgCmd);


}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
	
		if(m_MsgCmd == A_PtrMsg->Cmd)
		{
			Tv_Result	=	TRUE;
			switch(((Ptr_GrDbCtrlCmdBase)A_PtrMsg)->Cmd)
			{
				case E_GrDbCtrlCmdInit:
					ImmInit();
					break;
				case E_GrDbCtrlCmdFinish:
					ImmFinish();
					break;
				case E_GrDbCtrlCmdGrpInfoGet:
					ImmGrpInfoGet(((Ptr_GrDbCtrlCmdGrpInfoGet)A_PtrMsg)->PtrRtBuf);
					break;
				case  E_GrDbCtrlCmdKeyInfosGet:
					*(((Ptr_GrDbCtrlCmdKeyInfosGet)A_PtrMsg)->PtrRslt)	=
						ImmKeyInfosGet(((Ptr_GrDbCtrlCmdKeyInfosGet)A_PtrMsg)->GrpId, ((Ptr_GrDbCtrlCmdKeyInfosGet)A_PtrMsg)->KeyIdSt,
						((Ptr_GrDbCtrlCmdKeyInfosGet)A_PtrMsg)->MaxCnt, ((Ptr_GrDbCtrlCmdKeyInfosGet)A_PtrMsg)->PtrBuf,
						((Ptr_GrDbCtrlCmdKeyInfosGet)A_PtrMsg)->PtrRtKeyIdProc);
					break;
				case E_GrDbCtrlCmdDataChgInfoMake:
					*(((Ptr_GrDbCtrlCmdDataChgInfoMake)A_PtrMsg)->PtrRslt)	=
						ImmDataChgInfoMake(((Ptr_GrDbCtrlCmdDataChgInfoMake)A_PtrMsg)->GrpId, ((Ptr_GrDbCtrlCmdDataChgInfoMake)A_PtrMsg)->PtrKeyInfos,
						((Ptr_GrDbCtrlCmdDataChgInfoMake)A_PtrMsg)->KeyInfoCnt, ((Ptr_GrDbCtrlCmdDataChgInfoMake)A_PtrMsg)->PtrRtChgInfo);
					break;
				case E_GrDbCtrlCmdDataChgInfoToChgMap:
					ImmDataChgInfoToChgMap(((Ptr_GrDbCtrlCmdDataChgInfoToChgMap)A_PtrMsg)->GrpId, ((Ptr_GrDbCtrlCmdDataChgInfoToChgMap)A_PtrMsg)->PtrChgInfo,
						((Ptr_GrDbCtrlCmdDataChgInfoToChgMap)A_PtrMsg)->InfoCnt, ((Ptr_GrDbCtrlCmdDataChgInfoToChgMap)A_PtrMsg)->PtrMap);
					break;
				case E_GrDbCtrlCmdKeyInfoGetByKeyVal:
					*((Ptr_GrDbCtrlCmdKeyInfoGetByKeyVal)A_PtrMsg)->PtrRslt =
						ImmKeyInfoGetByKeyVal(((Ptr_GrDbCtrlCmdKeyInfoGetByKeyVal)A_PtrMsg)->GrpId,
						((Ptr_GrDbCtrlCmdKeyInfoGetByKeyVal)A_PtrMsg)->PtrKeyVal,
						((Ptr_GrDbCtrlCmdKeyInfoGetByKeyVal)A_PtrMsg)->PtrRtInfo);
					break;
				case E_GrDbCtrlCmdKeyInfoGetByKeyId:
					*((Ptr_GrDbCtrlCmdKeyInfoGetByKeyId)A_PtrMsg)->PtrRslt	=
						ImmKeyInfoGetByKeyId(((Ptr_GrDbCtrlCmdKeyInfoGetByKeyId)A_PtrMsg)->GrpId,
						((Ptr_GrDbCtrlCmdKeyInfoGetByKeyId)A_PtrMsg)->KeyId,
						((Ptr_GrDbCtrlCmdKeyInfoGetByKeyId)A_PtrMsg)->PtrRtBuf);
					break;
				case E_GrDbCtrlCmdKeyAtbUpdate:
					*((Ptr_GrDbCtrlCmdKeyAtbUpdate)A_PtrMsg)->PtrRslt	=
						ImmKeyAtbUpdate(((Ptr_GrDbCtrlCmdKeyAtbUpdate)A_PtrMsg)->GrpId,
						((Ptr_GrDbCtrlCmdKeyAtbUpdate)A_PtrMsg)->PtrKeyAtb, ((Ptr_GrDbCtrlCmdKeyAtbUpdate)A_PtrMsg)->IsForce);
					break;
				case E_GrDbCtrlCmdWriteBlkAndTimeByKeyVal:
					*((Ptr_GrDbCtrlCmdWriteBlkAndTimeByKeyVal)A_PtrMsg)->PtrRslt	=
						ImmWriteBlkAndTimeByKeyVal(((Ptr_GrDbCtrlCmdWriteBlkAndTimeByKeyVal)A_PtrMsg)->GrpId,
						((Ptr_GrDbCtrlCmdWriteBlkAndTimeByKeyVal)A_PtrMsg)->PtrKeyVal, ((Ptr_GrDbCtrlCmdWriteBlkAndTimeByKeyVal)A_PtrMsg)->BlkId,
						((Ptr_GrDbCtrlCmdWriteBlkAndTimeByKeyVal)A_PtrMsg)->TmUpdt, ((Ptr_GrDbCtrlCmdWriteBlkAndTimeByKeyVal)A_PtrMsg)->PtrBlkData,
						((Ptr_GrDbCtrlCmdWriteBlkAndTimeByKeyVal)A_PtrMsg)->IsForce);
					break;
				case E_GrDbCtrlCmdKeyAtbGetByKeyId:
					*((Ptr_GrDbCtrlCmdKeyAtbGetByKeyId)A_PtrMsg)->PtrRslt	=
						ImmKeyAtbGetByKeyId(((Ptr_GrDbCtrlCmdKeyAtbGetByKeyId)A_PtrMsg)->GrpId, ((Ptr_GrDbCtrlCmdKeyAtbGetByKeyId)A_PtrMsg)->KeyId,
						((Ptr_GrDbCtrlCmdKeyAtbGetByKeyId)A_PtrMsg)->PtrRtKeyAtb);
					break;
				case E_GrDbCtrlCmdReadBlkDataAndInfoByKeyId:
					*((Ptr_GrDbCtrlCmdReadBlkDataAndInfoByKeyId)A_PtrMsg)->PtrRslt	=
						ImmReadBlkDataAndInfoByKeyId(((Ptr_GrDbCtrlCmdReadBlkDataAndInfoByKeyId)A_PtrMsg)->GrpId, ((Ptr_GrDbCtrlCmdReadBlkDataAndInfoByKeyId)A_PtrMsg)->KeyId,
						((Ptr_GrDbCtrlCmdReadBlkDataAndInfoByKeyId)A_PtrMsg)->BlkId, ((Ptr_GrDbCtrlCmdReadBlkDataAndInfoByKeyId)A_PtrMsg)->PtrRtBlkBuf,
						((Ptr_GrDbCtrlCmdReadBlkDataAndInfoByKeyId)A_PtrMsg)->PtrRtKeyVal, ((Ptr_GrDbCtrlCmdReadBlkDataAndInfoByKeyId)A_PtrMsg)->PtrRtChkSum,
						((Ptr_GrDbCtrlCmdReadBlkDataAndInfoByKeyId)A_PtrMsg)->PtrRtTmUpdt);
					break;
				case E_GrDbCtrlCmdSyncSet:
					ImmSyncSet(((Ptr_GrDbCtrlCmdSyncSet)A_PtrMsg)->IsServer, ((Ptr_GrDbCtrlCmdSyncSet)A_PtrMsg)->IpSv, ((Ptr_GrDbCtrlCmdSyncSet)A_PtrMsg)->Port);
					break;
				case E_GrDbCtrlCmdDataReadByOfs:
					*((Ptr_GrDbCtrlCmdDataReadByOfs)A_PtrMsg)->PtrRslt	=
						ImmDataReadByOfs(((Ptr_GrDbCtrlCmdDataReadByOfs)A_PtrMsg)->GrpId, ((Ptr_GrDbCtrlCmdDataReadByOfs)A_PtrMsg)->PtrKeyVal,
						((Ptr_GrDbCtrlCmdDataReadByOfs)A_PtrMsg)->Ofs, ((Ptr_GrDbCtrlCmdDataReadByOfs)A_PtrMsg)->Size,
						((Ptr_GrDbCtrlCmdDataReadByOfs)A_PtrMsg)->PtrBuf);
					break;
				case E_GrDbCtrlCmdDataWriteByOfs:
					*((Ptr_GrDbCtrlCmdDataWriteByOfs)A_PtrMsg)->PtrRslt	=
						ImmDataWriteByOfs(((Ptr_GrDbCtrlCmdDataWriteByOfs)A_PtrMsg)->GrpId, ((Ptr_GrDbCtrlCmdDataWriteByOfs)A_PtrMsg)->PtrKeyVal,
						((Ptr_GrDbCtrlCmdDataWriteByOfs)A_PtrMsg)->Ofs, ((Ptr_GrDbCtrlCmdDataWriteByOfs)A_PtrMsg)->Size,
						((Ptr_GrDbCtrlCmdDataWriteByOfs)A_PtrMsg)->PtrBuf);
					break;
				case E_GrDbCtrlCmdKeyCreate:
					*((Ptr_GrDbCtrlCmdKeyCreate)A_PtrMsg)->PtrRslt	=
						ImmKeyCreate(((Ptr_GrDbCtrlCmdKeyCreate)A_PtrMsg)->GrpId, ((Ptr_GrDbCtrlCmdKeyCreate)A_PtrMsg)->PtrKeyVal);
					break;
				case E_GrDbCtrlCmdKeyFindByRng:
					*((Ptr_GrDbCtrlCmdKeyFindByValRng)A_PtrMsg)->PtrRslt	=
						ImmKeyFindByValRng(((Ptr_GrDbCtrlCmdKeyFindByValRng)A_PtrMsg)->GrpId, ((Ptr_GrDbCtrlCmdKeyFindByValRng)A_PtrMsg)->PtrKeySt,
						((Ptr_GrDbCtrlCmdKeyFindByValRng)A_PtrMsg)->PtrKeyEd, ((Ptr_GrDbCtrlCmdKeyFindByValRng)A_PtrMsg)->PtrRtBuf,
						((Ptr_GrDbCtrlCmdKeyFindByValRng)A_PtrMsg)->MaxCnt);
					break;
				case E_GrDbCtrlCmdKeyDelByKeyVal:
					*((Ptr_GrDbCtrlCmdKeyDelByKeyVal)A_PtrMsg)->PtrRslt	=
						ImmKeyDelByKeyVal(((Ptr_GrDbCtrlCmdKeyDelByKeyVal)A_PtrMsg)->GrpId, ((Ptr_GrDbCtrlCmdKeyDelByKeyVal)A_PtrMsg)->PtrKeyVal);;
					break;

				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::RtlBkgProc(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//if(BgwJobProc())
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDbCtrl::RtlTimer(__u32 A_Hndl)
{
	// local -------------------
		__u32	Tv_GrpIdx;
	// code --------------------
		if(m_TmrSec == A_Hndl)
		{
			for( Tv_GrpIdx = 0; Tv_GrpIdx < E_GrDbGrpMaxCnt; Tv_GrpIdx++)
			{
				if(NULL != m_ObjGrp[Tv_GrpIdx])
				{
					m_ObjGrp[Tv_GrpIdx]->RtlTimer();
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDbCtrl::ImmInit(void)
{
	// local -------------------
		__u32	Tv_GrpIdx;
	// code --------------------
		// group create
		for(Tv_GrpIdx = 0; Tv_GrpIdx < E_GrDbGrpMaxCnt; Tv_GrpIdx++)
		{
			if(0 != m_GrpInfo[Tv_GrpIdx].KeyValCnt)
			{
				// create
				m_ObjGrp[Tv_GrpIdx]	=	(Cls_GrDbGrp*)new Cls_GrDbGrp(m_StrPath, Tv_GrpIdx, 
					m_GrpInfo[Tv_GrpIdx].KeyValCnt, m_GrpInfo[Tv_GrpIdx].KeyValType);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrDbCtrl::ImmFinish(void)
{
	// local -------------------
		__u32	Tv_GrpIdx;
	// code --------------------
		for(Tv_GrpIdx = 0; Tv_GrpIdx < E_GrDbGrpMaxCnt; Tv_GrpIdx++)
		{
			if(NULL != m_ObjGrp[Tv_GrpIdx])
			{
				delete	m_ObjGrp[Tv_GrpIdx];
				m_ObjGrp[Tv_GrpIdx]	=	NULL;
			}
		}

		// sysnc object release
		if(NULL != m_DbSync)
		{
			delete	m_DbSync;
			m_DbSync	=	NULL;
		}

		// task sync delete
		if(NULL != m_TskSync)
		{
			delete	m_TskSync;
			m_TskSync	=	NULL;
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::ReqInit(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDbCtrlCmdInit	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdInit;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::ReqFinish(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDbCtrlCmdFinish	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdFinish;

		if(m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg)))
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::ImmKeyInfoGetByKeyId(__u32 A_GrpId, __u32 A_KeyId, Ptr_GrDbKeyInfo A_PtrRtBuf)
{
	// local -------------------
	// code --------------------
		// check group 
		if (E_GrDbGrpMaxCnt > A_GrpId)
		{
			return	FALSE;
		}

		if (NULL == m_ObjGrp[A_GrpId])
		{
			return	FALSE;
		}

		return	m_ObjGrp[A_GrpId]->KeyInfoGetByKeyId(A_KeyId, A_PtrRtBuf);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::ReqKeyInfoGetByKeyId(__u32 A_GrpId, __u32 A_KeyId, Ptr_GrDbKeyInfo A_PtrRtBuf)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDbCtrlCmdKeyInfoGetByKeyId	Tv_Msg;
	// code --------------------
		Tv_Result						=	FALSE;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdKeyInfoGetByKeyId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.GrpId				=	A_GrpId;
		Tv_Msg.KeyId				=	A_KeyId;
		Tv_Msg.PtrRtBuf			=	A_PtrRtBuf;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;

}
//--------------------------------------------------------------------
__u32	Cls_GrDbCtrl::ReqKeyInfoGetByKeyVal(__u32 A_GrpId, void* A_PtrKeyVal, Ptr_GrDbKeyInfo A_PtrRtKeyInfo)
{
	// local -------------------
		__u32	Tv_Result;
		St_GrDbCtrlCmdKeyInfoGetByKeyVal	Tv_Msg;
	// code --------------------
		Tv_Result = E_GrDbKeyIdNone;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdKeyInfoGetByKeyVal;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.GrpId				= A_GrpId;
		Tv_Msg.PtrKeyVal		=	A_PtrKeyVal;
		Tv_Msg.PtrRtInfo		=	A_PtrRtKeyInfo;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDbCtrl::ImmGrpInfoGet(Ptr_GrDbGrpInfo A_PtrBuf)
{
	// local -------------------
	// code --------------------
		GrDumyCopyMem(A_PtrBuf, m_GrpInfo, sizeof(m_GrpInfo));
}
//--------------------------------------------------------------------
void	Cls_GrDbCtrl::ReqGrpInfoGet(Ptr_GrDbGrpInfo A_PtrBuf)
{
	// local -------------------
		St_GrDbCtrlCmdGrpInfoGet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.Hd.Cmd		=	m_MsgCmd;
		Tv_Msg.Base.Cmd				=	E_GrDbCtrlCmdGrpInfoGet;
		Tv_Msg.PtrRtBuf				=	A_PtrBuf;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
__u32	Cls_GrDbCtrl::ImmKeyInfosGet(__u32 A_GrpId, __u32 A_KeyIdSt, __u32 A_MaxCnt, Ptr_GrDbKeyInfo A_PtrBuf, __u32* A_PtrRtKeyIdProc)
{
	// local -------------------
	// code --------------------
		// check group
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			*A_PtrRtKeyIdProc	=	E_GrDbKeyMaxCnt + 1;
			return	0;
		}

		// check exist group
		if(NULL == m_ObjGrp[A_GrpId])
		{
			*A_PtrRtKeyIdProc	=	E_GrDbKeyMaxCnt + 1;
			return	0;
		}

		return	m_ObjGrp[A_GrpId]->KeyInfosGet(A_KeyIdSt,A_MaxCnt,A_PtrBuf,A_PtrRtKeyIdProc);
}
//--------------------------------------------------------------------
__u32	Cls_GrDbCtrl::ReqKeyInfosGet(__u32 A_GrpId, __u32 A_KeyIdSt, __u32 A_MaxCnt, Ptr_GrDbKeyInfo A_PtrBuf, __u32* A_PtrRtKeyIdProc)
{
	// local -------------------
		__u32	Tv_Result;
		St_GrDbCtrlCmdKeyInfosGet	Tv_Msg;
	// code --------------------
		Tv_Result							=	0;
		*A_PtrRtKeyIdProc			=	E_GrDbKeyMaxCnt + 1;

		Tv_Msg.Base.Hd.Cmd		=	m_MsgCmd;
		Tv_Msg.Base.Cmd				=	E_GrDbCtrlCmdKeyInfosGet;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		Tv_Msg.GrpId					=	A_GrpId;
		Tv_Msg.KeyIdSt				=	A_KeyIdSt;
		Tv_Msg.MaxCnt					=	A_MaxCnt;
		Tv_Msg.PtrBuf					=	A_PtrBuf;
		Tv_Msg.PtrRtKeyIdProc	=	A_PtrRtKeyIdProc;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDbCtrl::ImmDataChgInfoMake(__u32 A_GrpId, Ptr_GrDbKeyInfo A_PtrKeyInfos, __u32 A_KeyInfoCnt, Ptr_GrDbDataChgInfo A_PtrRtChgInfo)
{
	// local -------------------
	// code --------------------
		// check group
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return	0;
		}

		// check exist group
		if(NULL == m_ObjGrp[A_GrpId])
		{
			return	0;
		}

		return	m_ObjGrp[A_GrpId]->DataChgInfoMake(A_PtrKeyInfos,A_KeyInfoCnt,A_PtrRtChgInfo);

}
//--------------------------------------------------------------------
__u32	Cls_GrDbCtrl::ReqDataChgInfoMake(__u32 A_GrpId, Ptr_GrDbKeyInfo A_PtrKeyInfos, __u32 A_KeyInfoCnt, Ptr_GrDbDataChgInfo A_PtrRtChgInfo)
{
	// local -------------------
		__u32	Tv_Result;
		St_GrDbCtrlCmdDataChgInfoMake	Tv_Msg;
	// code --------------------
		Tv_Result							=	0;

		Tv_Msg.Base.Hd.Cmd		=	m_MsgCmd;
		Tv_Msg.Base.Cmd				=	E_GrDbCtrlCmdDataChgInfoMake;
		Tv_Msg.PtrRslt				=	&Tv_Result;

		Tv_Msg.GrpId					=	A_GrpId;
		Tv_Msg.PtrKeyInfos		=	A_PtrKeyInfos;
		Tv_Msg.KeyInfoCnt			=	A_KeyInfoCnt;
		Tv_Msg.PtrRtChgInfo		=	A_PtrRtChgInfo;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;

}
//--------------------------------------------------------------------
void	Cls_GrDbCtrl::ImmDataChgInfoToChgMap(__u32 A_GrpId, Ptr_GrDbDataChgInfo A_PtrChgInfo, __u32 A_InfoCnt, __u32* A_PtrMap)
{
	// local -------------------
	// code --------------------
		// check group
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return;
		}

		// check exist group
		if(NULL == m_ObjGrp[A_GrpId])
		{
			return;
		}

		m_ObjGrp[A_GrpId]->DataChgInfoToChgMap(A_PtrChgInfo, A_InfoCnt, A_PtrMap);

}
//--------------------------------------------------------------------
void	Cls_GrDbCtrl::ReqDataChgInfoToChgMap(__u32 A_GrpId, Ptr_GrDbDataChgInfo A_PtrChgInfo, __u32 A_InfoCnt, __u32* A_PtrMap)
{
	// local -------------------
		St_GrDbCtrlCmdDataChgInfoToChgMap	Tv_Msg;
	// code --------------------

		Tv_Msg.Base.Hd.Cmd		=	m_MsgCmd;
		Tv_Msg.Base.Cmd				=	E_GrDbCtrlCmdDataChgInfoToChgMap;

		Tv_Msg.GrpId					=	A_GrpId;
		Tv_Msg.PtrChgInfo			=	A_PtrChgInfo;
		Tv_Msg.InfoCnt				=	A_InfoCnt;
		Tv_Msg.PtrMap					=	A_PtrMap;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
__u32	Cls_GrDbCtrl::ImmKeyInfoGetByKeyVal(__u32 A_GrpId, void* A_PtrKeyVal, Ptr_GrDbKeyInfo A_PtrRtKeyInfo)
{
	// local -------------------

	// code --------------------
		// check group
		if (E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return	E_GrDbKeyIdNone;
		}

		// check exist group
		if (NULL == m_ObjGrp[A_GrpId])
		{
			return	E_GrDbKeyIdNone;
		}

		return	m_ObjGrp[A_GrpId]->KeyInfoGetByKeyVal(A_PtrKeyVal,A_PtrRtKeyInfo);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::ImmReadBlkDataAndInfoByKeyId(__u32 A_GrpId, __u32 A_KeyId, __u32 A_BlkId, void* A_PtrRtBlkBuf, void* A_PtrRtKeyVal,
	__u32* A_PtrRtChkSum, __u32* A_PtrRtTmUpdt)
{
	// local -------------------
	// code --------------------
		// check group
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return	E_GrDbKeyIdNone;
		}

		// check exist group
		if(NULL == m_ObjGrp[A_GrpId])
		{
			return	E_GrDbKeyIdNone;
		}

		return	m_ObjGrp[A_GrpId]->ReadBlkDataAndInfoByKeyId(A_KeyId, A_BlkId, A_PtrRtBlkBuf, A_PtrRtKeyVal, A_PtrRtChkSum, A_PtrRtTmUpdt);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::ReqReadBlkDataAndInfoByKeyId(__u32 A_GrpId, __u32 A_KeyId, __u32 A_BlkId, void* A_PtrRtBlkBuf, void* A_PtrRtKeyVal,
	__u32* A_PtrRtChkSum, __u32* A_PtrRtTmUpdt)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDbCtrlCmdReadBlkDataAndInfoByKeyId	Tv_Msg;
	// code --------------------

		Tv_Result	=	FALSE;

		Tv_Msg.Base.Hd.Cmd		=	m_MsgCmd;
		Tv_Msg.Base.Cmd				=	E_GrDbCtrlCmdReadBlkDataAndInfoByKeyId;

		Tv_Msg.PtrRslt				=	&Tv_Result;
		Tv_Msg.GrpId					=	A_GrpId;
		Tv_Msg.KeyId					=	A_KeyId;
		Tv_Msg.BlkId					=	A_BlkId;
		Tv_Msg.PtrRtBlkBuf		=	A_PtrRtBlkBuf;
		Tv_Msg.PtrRtKeyVal		=	A_PtrRtKeyVal;
		Tv_Msg.PtrRtChkSum		=	A_PtrRtChkSum;
		Tv_Msg.PtrRtTmUpdt		=	A_PtrRtTmUpdt;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::ImmKeyAtbUpdate(__u32 A_GrpId, Ptr_GrDbKeyAtb A_PtrKeyAtb, BOOL8 A_IsForce)
{
	// local -------------------

	// code --------------------
		// check group
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return	E_GrDbKeyIdNone;
		}

		// check exist group
		if(NULL == m_ObjGrp[A_GrpId])
		{
			return	E_GrDbKeyIdNone;
		}

		return	m_ObjGrp[A_GrpId]->KeyAtbUpdate(A_PtrKeyAtb, A_IsForce);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::ReqKeyAtbUpdate(__u32 A_GrpId, Ptr_GrDbKeyAtb A_PtrKeyAtb, BOOL8 A_IsForce)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDbCtrlCmdKeyAtbUpdate	Tv_Msg;
	// code --------------------
		Tv_Result		=	FALSE;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdKeyAtbUpdate;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.IsForce			=	A_IsForce;
		Tv_Msg.GrpId				=	A_GrpId;
		Tv_Msg.PtrKeyAtb		=	A_PtrKeyAtb;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::ImmWriteBlkAndTimeByKeyVal(__u32 A_GrpId, void* A_PtrKeyVal, __u32 A_BlkId, __u32 A_TmUpdt, void* A_PtrBlkData, BOOL8 A_IsForce)
{
	// local -------------------
	// code --------------------
		// check group
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return	FALSE;
		}

		// check exist group
		if(NULL == m_ObjGrp[A_GrpId])
		{
			return	FALSE;
		}

		return	m_ObjGrp[A_GrpId]->WriteBlkAndTimeByKeyVal(A_PtrKeyVal,A_BlkId,A_TmUpdt,A_PtrBlkData, A_IsForce);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::ReqWriteBlkAndTimeByKeyVal(__u32 A_GrpId, void* A_PtrKeyVal, __u32 A_BlkId, __u32 A_TmUpdt, void* A_PtrBlkData, BOOL8 A_IsForce)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDbCtrlCmdWriteBlkAndTimeByKeyVal	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdWriteBlkAndTimeByKeyVal;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.GrpId				=	A_GrpId;
		Tv_Msg.PtrKeyVal		=	A_PtrKeyVal;
		Tv_Msg.BlkId				=	A_BlkId;
		Tv_Msg.TmUpdt				=	A_TmUpdt;
		Tv_Msg.PtrBlkData		=	A_PtrBlkData;
		Tv_Msg.IsForce			=	A_IsForce;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::ImmKeyAtbGetByKeyId(__u32 A_GrpId, __u32 A_KeyId, Ptr_GrDbKeyAtb A_PtrRtKeyAtb)
{
	// local -------------------
	// code --------------------
		// check group
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return	E_GrDbKeyIdNone;
		}

		// check exist group
		if(NULL == m_ObjGrp[A_GrpId])
		{
			return	E_GrDbKeyIdNone;
		}

		return	m_ObjGrp[A_GrpId]->KeyAtbGetByKeyId(A_KeyId, A_PtrRtKeyAtb);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrDbCtrl::ReqKeyAtbGetByKeyId(__u32 A_GrpId, __u32 A_KeyId, Ptr_GrDbKeyAtb A_PtrRtKeyAtb)
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrDbCtrlCmdKeyAtbGetByKeyId	Tv_Msg;
	// code --------------------
		Tv_Result		=	FALSE;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdKeyAtbGetByKeyId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.GrpId				=	A_GrpId;
		Tv_Msg.KeyId				=	A_KeyId;
		Tv_Msg.PtrRtKeyAtb	=	A_PtrRtKeyAtb;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDbCtrl::ImmSyncSet(BOOL8 A_IsServer, __u32 A_IpSv, __u16 A_Port)
{
	// local -------------------
	// code --------------------
		// release sync
		if(NULL != m_DbSync)
		{
			delete	m_DbSync;
			m_DbSync	=	NULL;
		}

		if((0 != A_Port) && (A_IsServer || (0 != A_IpSv)))
		{
			// create new task
			if(NULL == m_TskSync)
			{
				m_TskSync	=	(Cls_GrTaskSvr*)new Cls_GrTaskSvr(5000);
			}

			// creat sync object
			m_DbSync	=	(Cls_GrTaskCli*)new Cls_GrDbSync(m_TskSync, (void*)this, A_IsServer, A_IpSv, A_Port);
			((Cls_GrDbSync*)m_DbSync)->ReqInit(this);
		}

		if((NULL == m_DbSync) && (NULL != m_TskSync))
		{
			delete	m_TskSync;
			m_TskSync	=	NULL;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDbCtrl::ReqSyncSet(BOOL8 A_IsServer, __u32 A_IpSv, __u16 A_Port)
{
	// local -------------------
		St_GrDbCtrlCmdSyncSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdSyncSet;
		Tv_Msg.IsServer			=	A_IsServer;
		Tv_Msg.IpSv					=	A_IpSv;
		Tv_Msg.Port					=	A_Port;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

}
//--------------------------------------------------------------------
__u8	Cls_GrDbCtrl::ImmDataReadByOfs(__u32 A_GrpId, void* A_PtrKeyVal, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
		__u32	Tv_KeyId;
	// code --------------------
		// check group
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return	E_GrErrBadParam;
		}

		// check exist group
		if(NULL == m_ObjGrp[A_GrpId])
		{
			return	E_GrErrNotFound;
		}

		Tv_KeyId	=	m_ObjGrp[A_GrpId]->KeyIdGetByKeyVal(A_PtrKeyVal);
		if(E_GrDbKeyIdNone == Tv_KeyId)
		{
			return	E_GrErrNotFound;
		}

		return	m_ObjGrp[A_GrpId]->ReadDataByKeyId(Tv_KeyId, A_Ofs,A_Size,A_PtrBuf);

}
//--------------------------------------------------------------------
__u8	Cls_GrDbCtrl::ImmDataWriteByOfs(__u32 A_GrpId, void* A_PtrKeyVal, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
		__u32	Tv_KeyId;
	// code --------------------
		// check group
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return	E_GrErrBadParam;
		}

		// check exist group
		if(NULL == m_ObjGrp[A_GrpId])
		{
			return	E_GrErrNotFound;
		}

		Tv_KeyId	=	m_ObjGrp[A_GrpId]->KeyIdGetByKeyVal(A_PtrKeyVal);
		if(E_GrDbKeyIdNone == Tv_KeyId)
		{
			return	E_GrErrNotFound;
		}

		return	m_ObjGrp[A_GrpId]->WriteDataByKeyId(Tv_KeyId, A_Ofs, A_Size, A_PtrBuf);

}
//--------------------------------------------------------------------
__u8	Cls_GrDbCtrl::ReqDataReadByOfs(__u32 A_GrpId, void* A_PtrKeyVal, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDbCtrlCmdDataReadByOfs	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrErrFail;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdDataReadByOfs;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.GrpId				=	A_GrpId;
		Tv_Msg.PtrKeyVal		=	A_PtrKeyVal;
		Tv_Msg.Ofs					=	A_Ofs;
		Tv_Msg.Size					=	A_Size;
		Tv_Msg.PtrBuf				=	A_PtrBuf;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDbCtrl::ReqDataWriteByOfs(__u32 A_GrpId, void* A_PtrKeyVal, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDbCtrlCmdDataWriteByOfs	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrErrFail;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdDataWriteByOfs;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.GrpId				=	A_GrpId;
		Tv_Msg.PtrKeyVal		=	A_PtrKeyVal;
		Tv_Msg.Ofs					=	A_Ofs;
		Tv_Msg.Size					=	A_Size;
		Tv_Msg.PtrBuf				=	A_PtrBuf;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDbCtrl::ImmKeyCreate(__u32 A_GrpId, void* A_PtrKeyVal)
{
	// local -------------------
		__u32	Tv_KeyId;
	// code --------------------
		// check group
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return	E_GrErrBadParam;
		}

		// check exist group
		if(NULL == m_ObjGrp[A_GrpId])
		{
			return	E_GrErrNotFound;
		}

		return	m_ObjGrp[A_GrpId]->KeyCreate(A_PtrKeyVal, &Tv_KeyId);
}
//--------------------------------------------------------------------
__u8	Cls_GrDbCtrl::ReqKeyCreate(__u32 A_GrpId, void* A_PtrKeyVal)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDbCtrlCmdKeyCreate	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrErrFail;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdKeyCreate;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.GrpId				=	A_GrpId;
		Tv_Msg.PtrKeyVal		=	A_PtrKeyVal;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;

}
//--------------------------------------------------------------------
__u8	Cls_GrDbCtrl::ImmKeyDelByKeyVal(__u32 A_GrpId, void* A_PtrKeyVal)
{
	// local -------------------
		__u32	Tv_KeyId;
	// code --------------------
		// check group
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return	E_GrErrBadParam;
		}

		// check exist group
		if(NULL == m_ObjGrp[A_GrpId])
		{
			return	E_GrErrNotFound;
		}

		return	m_ObjGrp[A_GrpId]->KeyDelByKeyVal(A_PtrKeyVal);
}
//--------------------------------------------------------------------
__u8	Cls_GrDbCtrl::ReqKeyDelByKeyVal(__u32 A_GrpId, void* A_PtrKeyVal)
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDbCtrlCmdKeyDelByKeyVal	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrErrFail;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdKeyDelByKeyVal;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.GrpId				=	A_GrpId;
		Tv_Msg.PtrKeyVal		=	A_PtrKeyVal;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;

}
//--------------------------------------------------------------------
__u32	Cls_GrDbCtrl::ImmKeyFindByValRng(__u32 A_GrpId, void* A_PtrKeySt, void* A_PtrKeyEd, void* A_PtrRtBuf, __u32 A_MaxCnt)
{
	// local -------------------

	// code --------------------
		// check group
		if(E_GrDbGrpMaxCnt <= A_GrpId)
		{
			return	0;
		}

		// check exist group
		if(NULL == m_ObjGrp[A_GrpId])
		{
			return	0;
		}

		return	m_ObjGrp[A_GrpId]->KeyFindByValRng(A_PtrKeySt, A_PtrKeyEd, A_PtrRtBuf, A_MaxCnt);
}
//--------------------------------------------------------------------
__u32	Cls_GrDbCtrl::ReqKeyFindByValRng(__u32 A_GrpId, void* A_PtrKeySt, void* A_PtrKeyEd, void* A_PtrRtBuf, __u32 A_MaxCnt)
{
	// local -------------------
		__u32	Tv_Result;
		St_GrDbCtrlCmdKeyFindByValRng	Tv_Msg;
	// code --------------------
		Tv_Result		=	0;

		Tv_Msg.Base.Hd.Cmd	=	m_MsgCmd;
		Tv_Msg.Base.Cmd			=	E_GrDbCtrlCmdKeyFindByRng;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.GrpId				=	A_GrpId;
		Tv_Msg.PtrKeySt			=	A_PtrKeySt;
		Tv_Msg.PtrKeyEd			=	A_PtrKeyEd;
		Tv_Msg.PtrRtBuf			=	A_PtrRtBuf;
		Tv_Msg.MaxCnt				=	A_MaxCnt;

		m_TskSvr->MsgSend((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
