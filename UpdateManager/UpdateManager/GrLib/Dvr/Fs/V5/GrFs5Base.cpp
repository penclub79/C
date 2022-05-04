/*
 GAUSE file system version 5 Base

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

#include <Dvr/Fs/V5/GrFs5Base.h>
#include <Dvr/Fs/V5/GrFs5Hdd.h>
#include <Dvr/Fs/V5/GrFs5IoRaid.h>

#include	<stdlib.h>

#ifdef LINUX_APP

#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

#endif

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

St_GrFs5Env	V_GrFs5Env;

//====================================================================
//--------------------------------------------------------------------
void	GrFs5Prepare(Cls_GrTaskSvr* A_TskSvr, __u64 A_MacAdr, __u32 A_BkupHdSize, __u32 A_DiskMaxCnt,
	__u32 A_CchMinCnt, __u32 A_CchDataCnt)
{
	// local -------------------
		__u32	Tv_GrpIdx;
		__u32	Tv_ItmIdx;
	// code --------------------

		GrDumyZeroMem(&V_GrFs5Env, sizeof(V_GrFs5Env));

		if(E_GrFs5MinuteItemSize != sizeof(St_GrFs5MinuteInfo))
		{
			DbgMsgPrint("GrFs5Prepare - bad St_GrFs5MinuteInfo size %d \n", sizeof(St_GrFs5MinuteInfo));
		}

		if(0 != ((E_GrDvrFrmAlignSize - 1) & sizeof(St_GrFs5FrmLogPool)))
		{
			DbgMsgPrint("GrFs5Prepare - bad frame St_GrFs5FrmLogPool size %d \n", sizeof(St_GrFs5FrmLogPool));
		}

		if(E_GrFs5BlkInfoSize != sizeof(St_GrFs5BlkInfoBase))
		{
			DbgMsgPrint("GrFs5Prepare - bad St_GrFs5BlkInfoBase size %d \n", sizeof(St_GrFs5BlkInfoBase));
		}

		V_GrFs5Env.DiskBaseId	=	A_MacAdr;
		V_GrFs5Env.BkupHdSize	=	A_BkupHdSize;
		V_GrFs5Env.DiskMaxCnt	=	A_DiskMaxCnt;

		// hardware buffer build
		V_GrFs5Env.HwBufs[0].CchMinCnt	=	A_CchMinCnt;
		if (E_GrFs5HwBufMinMaxCnt < V_GrFs5Env.HwBufs[0].CchMinCnt)
		{
			DbgMsgPrint("GrFs5Prepare - min cache count change %d to %d\n", A_CchMinCnt, E_GrFs5HwBufMinMaxCnt);
			V_GrFs5Env.HwBufs[0].CchMinCnt	=	E_GrFs5HwBufMinMaxCnt;
		}
		V_GrFs5Env.HwBufs[0].CchDataCnt	=	A_CchDataCnt;
		if (E_GrFs5HwBufDataMaxCnt < V_GrFs5Env.HwBufs[0].CchDataCnt)
		{
			DbgMsgPrint("GrFs5Prepare - min cache count change %d to %d\n", A_CchDataCnt, E_GrFs5HwBufDataMaxCnt);
			V_GrFs5Env.HwBufs[0].CchDataCnt	=	E_GrFs5HwBufDataMaxCnt;
		}

		V_GrFs5Env.HwBufs[1].CchMinCnt	=	3;
		V_GrFs5Env.HwBufs[1].CchDataCnt	=	3;

		// build data cache
		for(Tv_GrpIdx = 0; Tv_GrpIdx < E_GrFs5RgrpMaxCnt; Tv_GrpIdx++)
		{
			// min cache
			for ( Tv_ItmIdx = 0; Tv_ItmIdx < V_GrFs5Env.HwBufs[Tv_GrpIdx].CchMinCnt; Tv_ItmIdx++)
			{
#ifdef LINUX_APP
				V_GrFs5Env.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx]	=	(void*)mmap(NULL, E_GrFs5MitDaySize, PROT_READ|PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,
					-1, 0);
				if (0xFFFFFFFF == (__u32)V_GrFs5Env.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx])
				{
					DbgMsgPrint("GrFs5Prepare - grp %d min mmap error %d\n", Tv_GrpIdx, errno);
					V_GrFs5Env.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx]	=	NULL;
				}
#else
				V_GrFs5Env.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx]	=	(void*)malloc(E_GrFs5MitDaySize);
#endif
				if (NULL == V_GrFs5Env.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx])
				{
					DbgMsgPrint("GrFs5Prepare - not enough mem, grp %d min buf count %d to %d\n",
						Tv_GrpIdx, V_GrFs5Env.HwBufs[Tv_GrpIdx].CchMinCnt, Tv_ItmIdx);
					V_GrFs5Env.HwBufs[Tv_GrpIdx].CchMinCnt	=	Tv_ItmIdx;
					break;
				}
			}

			// data cache
			for (Tv_ItmIdx = 0; Tv_ItmIdx < V_GrFs5Env.HwBufs[Tv_GrpIdx].CchDataCnt; Tv_ItmIdx++)
			{
#ifdef LINUX_APP
				V_GrFs5Env.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx]	=	(void*)mmap(NULL, E_GrFs5RgrpCchDataBufSize, PROT_READ|PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,
					-1, 0);
				if (0xFFFFFFFF == (__u32)V_GrFs5Env.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx])
				{
					DbgMsgPrint("GrFs5Prepare - grp %d data mmap error %d\n", Tv_GrpIdx, errno);
					V_GrFs5Env.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx]	=	NULL;
				}
#else
				V_GrFs5Env.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx]	=	(void*)malloc(E_GrFs5RgrpCchDataBufSize);
#endif
				if (NULL == V_GrFs5Env.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx])
				{
					DbgMsgPrint("GrFs5Prepare - not enough mem, grp %d data buf count %d to %d\n",
						Tv_GrpIdx, V_GrFs5Env.HwBufs[Tv_GrpIdx].CchDataCnt, Tv_ItmIdx);
					V_GrFs5Env.HwBufs[Tv_GrpIdx].CchDataCnt	=	Tv_ItmIdx;
					break;
				}
			}
		}

		// create manager
		V_GrFs5Env.ObjMng	=	(Cls_GrTaskCli*)new Cls_GrFs5IoRaid(A_TskSvr, A_CchMinCnt,A_CchDataCnt);

		
}
//--------------------------------------------------------------------
void	GrFs5Init(void)
{
		((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqInit();
}
//--------------------------------------------------------------------
void	GrFs5Stop(void)
{
		((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqFinish();
}
//--------------------------------------------------------------------
void	GrFs5Finish(void)
{
	// local -------------------
		__u32	Tv_GrpIdx;
		__u32	Tv_ItmIdx;
	// code --------------------
		// release manager
		delete	V_GrFs5Env.ObjMng;
		V_GrFs5Env.ObjMng	=	NULL;

		// release hw buffer
		for (Tv_GrpIdx = 0; Tv_GrpIdx < E_GrFs5RgrpMaxCnt; Tv_GrpIdx++)
		{
			// min cache
			for (Tv_ItmIdx = 0; Tv_ItmIdx < V_GrFs5Env.HwBufs[Tv_GrpIdx].CchMinCnt; Tv_ItmIdx++)
			{
#ifdef LINUX_APP
				munmap(V_GrFs5Env.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx],E_GrFs5MitDaySize);
#else
				free(V_GrFs5Env.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx]);
#endif
				V_GrFs5Env.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx]	=	NULL;
			}

			// data cache
			for (Tv_ItmIdx = 0; Tv_ItmIdx < V_GrFs5Env.HwBufs[Tv_GrpIdx].CchDataCnt; Tv_ItmIdx++)
			{
#ifdef LINUX_APP
				munmap(V_GrFs5Env.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx],E_GrFs5RgrpCchDataBufSize);
#else
				free(V_GrFs5Env.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx]);
#endif
				V_GrFs5Env.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx]	=	NULL;
			}

		}

}
//--------------------------------------------------------------------
void	GrFs5HndlDec(__u64 A_Hndl, __s32* A_PtrRtDskIdx, __u32* A_PtrRtOffset, __s32* A_PtrRtBlkIdx, __u8* A_PtrRtOvwtId)
{
		*A_PtrRtOffset	=	A_Hndl & E_GrFs5HndlOfsMask;
		*A_PtrRtDskIdx	=	(__s32)(A_Hndl & E_GrFs5HndlGrpIdxMask);
		*A_PtrRtBlkIdx	=	(__s32)((A_Hndl >> E_GrFs5HndlBlkIdxPos) & 0xFFFF);
		*A_PtrRtOvwtId	=	(__u8)((A_Hndl >> E_GrFs5HndlOvwtIdPos) & 0xFF);
}
//--------------------------------------------------------------------
__u64	GrFs5HndlEnc(__s32 A_DskIdx, __u32 A_Offset, __s32 A_BlkIdx, __u8 A_OvwtId)
{
	// local -------------------
	// code --------------------
		return	A_Offset | (__u64)A_DskIdx | ((__u64)A_BlkIdx << E_GrFs5HndlBlkIdxPos) | ((__u64)A_OvwtId << E_GrFs5HndlOvwtIdPos);
}
//--------------------------------------------------------------------
void	GrFs5RecBufAdd(iovec* A_PtrIovs, __u32 A_Cnt)
{
		return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqRecBufAdd(A_PtrIovs,A_Cnt);
}
//--------------------------------------------------------------------
void	GrFs5Setup(__u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir)
{
		((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqSetup(A_NormalMap, A_MirrorMap, A_IsOvwtNrm, A_IsOvwtMir);
}
//--------------------------------------------------------------------
void	GrFs5AutoDelSet(__u32 A_Sec)
{
		((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqAtDelTimeSet(A_Sec);
}
//--------------------------------------------------------------------
BOOL8	GrFs5DiskFormatReq(__u32 A_DiskIdx, BOOL8 A_IsPost)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqDiskFormat((__u8)A_DiskIdx, A_IsPost);
}
//--------------------------------------------------------------------
BOOL8	GrFs5GroupDiskFormatReq(__u8 A_GroupMap, BOOL8 A_IsPost)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqGroupDiskFormat((__u8)A_GroupMap, A_IsPost);
}
//--------------------------------------------------------------------
__u8	GrFs5DiskInstMapGet(__u8* A_PtrRtBadMap)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqDiskInstMapGet(A_PtrRtBadMap);
}
//--------------------------------------------------------------------
__u32	GrFs5MonthDayMapGet(__u8 A_Group, __u32 A_TimeMonth)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqGroupGetMonthDayMap(A_Group,A_TimeMonth);
}
//--------------------------------------------------------------------
BOOL8	GrFs5MinRatbTblGet(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrBuf, BOOL8 A_IsPost, __u32 A_UserData)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqGroupGetMinRatbTbl(A_GroupType, A_TimeSt, A_ItemCnt, A_PtrBuf,A_IsPost,A_UserData);
}
//--------------------------------------------------------------------
__u8	GrFs5DiskRecInfoGet(__u32 A_DiskIdx, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqDiskRecInfoGet((__u8)A_DiskIdx, A_PtrRtTotalSize,A_PtrRtRecSize);
}
//--------------------------------------------------------------------
__u8	GrFs5GroupRecInfoGet(__u8 A_Group, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqGroupRecInfoGet(A_Group, A_PtrRtTotalSize, A_PtrRtRecSize);
}
//--------------------------------------------------------------------
BOOL8	GrFs5DiskModelNameGet(__u32 A_DiskIdx, WCHAR* A_StrRt)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqDiskModelNameGet((__u8)A_DiskIdx, A_StrRt);
}
//--------------------------------------------------------------------
BOOL8	GrFs5GroupRecTimeRngGet(__u8 A_Group, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd, Def_GrTime* A_PtrRtTimeLast)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqGroupGetRecTimeRng(A_Group, A_PtrRtTimeSt,A_PtrRtTimeEd, A_PtrRtTimeLast);
}
//--------------------------------------------------------------------
__u8	GrFs5DiskTemperatureGet(__u8 A_DiskIdx)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->DiskTemperatureGet(A_DiskIdx);
}
//--------------------------------------------------------------------
BOOL8	GrFs5LogAdd(Ptr_GrDvrLogItm A_PtrLog)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(NULL != V_GrFs5Env.ObjMng)
		{
			((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqLogAdd(A_PtrLog);
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	GrFs5GroupRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd)
{
		return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqRecSizeEstmate(A_GroupType, A_ChMap, A_TimeSt, A_TimeEd);
}
//--------------------------------------------------------------------
__s32	GrFs5LogGetOpen(__u8 A_Group, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_MaxCnt,
	__u32 A_ChMask, __u64 A_TypeMask, Ptr_GrDvrLogItm A_PtrRtBuf, BOOL8 A_IsFrmMode)
{
	// local -------------------
	// code --------------------
		return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqLogGetOpen(A_Group, A_TimeSt, A_TimeEd, A_MaxCnt, A_ChMask, A_TypeMask,
			A_PtrRtBuf, A_IsFrmMode);
}
//--------------------------------------------------------------------
BOOL8	GrFs5LogGetChkDone(__s32 A_Hndl, __u32* A_PtrRtCnt)
{
		return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqLogGetChkDone(A_Hndl, A_PtrRtCnt);
}
//--------------------------------------------------------------------
void	GrFs5LogGetClose(__s32 A_Hndl)
{
		((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqLogGetClose(A_Hndl);
}
//--------------------------------------------------------------------
Def_GrErrCode	GrFs5LogGetFrameModeCheck(__s32 A_Hndl, __u32* A_PtrRtCnt)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqLogGetFrameModeCheck(A_Hndl, A_PtrRtCnt);
}
//--------------------------------------------------------------------
BOOL8	GrFs5LogGetFrameModeResume(__s32 A_Hndl)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqLogGetFrameModeResume(A_Hndl);
}
//--------------------------------------------------------------------
__u8	GrFs5RecInfoGet(__u8* A_PtrRtGroup, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize)
{
	return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqRecInfoGet(A_PtrRtGroup, A_PtrRtTotalSize, A_PtrRtRecSize);
}
//--------------------------------------------------------------------
BOOL8	GrFs5SmartRead(__u32 A_DskIdx, Ptr_GrFs5Smart A_PtrRtData)
{
		return	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqDiskSmartRead((__u8)A_DskIdx, A_PtrRtData);
}
//--------------------------------------------------------------------
void	GrFs5GroupMakeFull(__u8 A_Group)
{
	((Cls_GrFs5IoRaid*)V_GrFs5Env.ObjMng)->ReqGroupMakeFull(A_Group);
}
//--------------------------------------------------------------------

