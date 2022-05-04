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

#include <Dvr/Fs/Nvt/GrFsNvtBase.h>
#include <Dvr/Fs/Nvt/GrFsNvtHdd.h>
#include <Dvr/Fs/Nvt/GrFsNvtIoRaid.h>

#include	<stdlib.h>

#ifdef LINUX_APP

#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

#endif

//====================================================================
//local const

#ifdef DVR_4CH
#define E_GrFsNvtDecMemOffset						0x0D400000
#define E_GrFsNvtDecMemSize							0x02400000	//36Mb
#endif

#ifdef DVR_8CH
#define E_GrFsNvtDecMemOffset						0x0D800000
#define E_GrFsNvtDecMemSize							0x02000000	//32Mb
#endif

#ifdef DVR_16CH
#define E_GrFsNvtDecMemOffset						0x0D800000
#define E_GrFsNvtDecMemSize							0x02000000	//32Mb
#endif

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

St_GrFsNvtEnv	V_GrFsNvtEnv;

//decode memory
St_GrFsNvtDecBuf	V_GrFsNvtDecBuf;

//====================================================================
//--------------------------------------------------------------------
void	GrFsNvtPrepare(Cls_GrTaskSvr* A_TskSvr, __u64 A_MacAdr, __u32 A_BkupHdSize, __u32 A_DiskMaxCnt,
	__u32 A_CchMinCnt, __u32 A_CchDataCnt)
{
	// local -------------------
		__u32	Tv_GrpIdx;
		__u32	Tv_ItmIdx;
		__s32	Tv_DecMemOfs;
		__s32 Tv_DecMemSize;
	// code --------------------

		GrDumyZeroMem(&V_GrFsNvtEnv, sizeof(V_GrFsNvtEnv));

		if(E_GrFsNvtMinuteItemSize != sizeof(St_GrFsNvtMinuteInfo))
		{
			DbgMsgPrint("GrFsNvtPrepare - bad St_GrFsNvtMinuteInfo size %d \n", sizeof(St_GrFsNvtMinuteInfo));
		}

		if(0 != ((E_GrDvrFrmAlignSize - 1) & sizeof(St_GrFsNvtFrmLogPool)))
		{
			DbgMsgPrint("GrFsNvtPrepare - bad frame St_GrFsNvtFrmLogPool size %d \n", sizeof(St_GrFsNvtFrmLogPool));
		}

		if(E_GrFsNvtBlkInfoSize != sizeof(St_GrFsNvtBlkInfoBase))
		{
			DbgMsgPrint("GrFsNvtPrepare - bad St_GrFsNvtBlkInfoBase size %d \n", sizeof(St_GrFsNvtBlkInfoBase));
		}

		V_GrFsNvtEnv.DiskBaseId	=	A_MacAdr;
		V_GrFsNvtEnv.BkupHdSize	=	A_BkupHdSize;
		V_GrFsNvtEnv.DiskMaxCnt	=	A_DiskMaxCnt;

		// hardware buffer build
		V_GrFsNvtEnv.HwBufs[0].CchMinCnt	=	A_CchMinCnt;
		if (E_GrFsNvtHwBufMinMaxCnt < V_GrFsNvtEnv.HwBufs[0].CchMinCnt)
		{
			DbgMsgPrint("GrFsNvtPrepare - min cache count change %d to %d\n", A_CchMinCnt, E_GrFsNvtHwBufMinMaxCnt);
			V_GrFsNvtEnv.HwBufs[0].CchMinCnt	=	E_GrFsNvtHwBufMinMaxCnt;
		}
		V_GrFsNvtEnv.HwBufs[0].CchDataCnt	=	A_CchDataCnt;
		if (E_GrFsNvtHwBufDataMaxCnt < V_GrFsNvtEnv.HwBufs[0].CchDataCnt)
		{
			DbgMsgPrint("GrFsNvtPrepare - min cache count change %d to %d\n", A_CchDataCnt, E_GrFsNvtHwBufDataMaxCnt);
			V_GrFsNvtEnv.HwBufs[0].CchDataCnt	=	E_GrFsNvtHwBufDataMaxCnt;
		}

		V_GrFsNvtEnv.HwBufs[1].CchMinCnt	=	3;
		V_GrFsNvtEnv.HwBufs[1].CchDataCnt	=	3;

		GrFsNvtDecBufInit();

		// build data cache
		for(Tv_GrpIdx = 0; Tv_GrpIdx < E_GrFsNvtRgrpMaxCnt; Tv_GrpIdx++)
		{
			// min cache
			for ( Tv_ItmIdx = 0; Tv_ItmIdx < V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchMinCnt; Tv_ItmIdx++)
			{
#ifdef LINUX_APP
				V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx]	=	(void*)mmap(NULL, E_GrFsNvtMitDaySize, PROT_READ|PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,
					-1, 0);
				if (0xFFFFFFFF == (__u32)V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx])
				{
					DbgMsgPrint("GrFsNvtPrepare - grp %d min mmap error %d\n", Tv_GrpIdx, errno);
					V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx]	=	NULL;
				}
#else
				V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx]	=	(void*)malloc(E_GrFsNvtMitDaySize);
#endif
				if (NULL == V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx])
				{
					DbgMsgPrint("GrFsNvtPrepare - not enough mem, grp %d min buf count %d to %d\n",
						Tv_GrpIdx, V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchMinCnt, Tv_ItmIdx);
					V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchMinCnt	=	Tv_ItmIdx;
					break;
				}
			}

			// data cache
			for (Tv_ItmIdx = 0; Tv_ItmIdx < V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchDataCnt; Tv_ItmIdx++)
			{
#ifdef LINUX_APP
				V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx]	=	(void*)mmap(NULL, E_GrFsNvtRgrpCchDataBufSize, PROT_READ|PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,
					-1, 0);
				if (0xFFFFFFFF == (__u32)V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx])
				{
					DbgMsgPrint("GrFsNvtPrepare - grp %d data mmap error %d\n", Tv_GrpIdx, errno);
					V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx]	=	NULL;
				}
#else
				V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx]	=	(void*)malloc(E_GrFsNvtRgrpCchDataBufSize);
#endif
				if (NULL == V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx])
				{
					DbgMsgPrint("GrFsNvtPrepare - not enough mem, grp %d data buf count %d to %d\n",
						Tv_GrpIdx, V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchDataCnt, Tv_ItmIdx);
					V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchDataCnt	=	Tv_ItmIdx;
					break;
				}
			}
		}

		// create manager
		V_GrFsNvtEnv.ObjMng	=	(Cls_GrTaskCli*)new Cls_GrFsNvtIoRaid(A_TskSvr, A_CchMinCnt,A_CchDataCnt);
}
//--------------------------------------------------------------------
void	GrFsNvtInit(void)
{
		((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqInit();
}
//--------------------------------------------------------------------
void	GrFsNvtStop(void)
{
		((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqFinish();
}
//--------------------------------------------------------------------
void	GrFsNvtFinish(void)
{
	// local -------------------
		__u32	Tv_GrpIdx;
		__u32	Tv_ItmIdx;
	// code --------------------
		// release manager
		delete	V_GrFsNvtEnv.ObjMng;
		V_GrFsNvtEnv.ObjMng	=	NULL;

		// release hw buffer
		for (Tv_GrpIdx = 0; Tv_GrpIdx < E_GrFsNvtRgrpMaxCnt; Tv_GrpIdx++)
		{
			// min cache
			for (Tv_ItmIdx = 0; Tv_ItmIdx < V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchMinCnt; Tv_ItmIdx++)
			{
#ifdef LINUX_APP
				munmap(V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx],E_GrFsNvtMitDaySize);
#else
				free(V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx]);
#endif
				V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchMinBuf[Tv_ItmIdx]	=	NULL;
			}

			// data cache
			for (Tv_ItmIdx = 0; Tv_ItmIdx < V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchDataCnt; Tv_ItmIdx++)
			{
#ifdef LINUX_APP
				munmap(V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx],E_GrFsNvtRgrpCchDataBufSize);
#else
				free(V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx]);
#endif
				V_GrFsNvtEnv.HwBufs[Tv_GrpIdx].CchDataBuf[Tv_ItmIdx]	=	NULL;
			}

		}

}
//--------------------------------------------------------------------
void	GrFsNvtHndlDec(__u64 A_Hndl, __s32* A_PtrRtDskIdx, __u32* A_PtrRtOffset, __s32* A_PtrRtBlkIdx, __u8* A_PtrRtOvwtId)
{
		*A_PtrRtOffset	=	A_Hndl & E_GrFsNvtHndlOfsMask;
		*A_PtrRtDskIdx	=	(__s32)(A_Hndl & E_GrFsNvtHndlGrpIdxMask);
		*A_PtrRtBlkIdx	=	(__s32)((A_Hndl >> E_GrFsNvtHndlBlkIdxPos) & 0xFFFF);
		*A_PtrRtOvwtId	=	(__u8)((A_Hndl >> E_GrFsNvtHndlOvwtIdPos) & 0xFF);
}
//--------------------------------------------------------------------
__u64	GrFsNvtHndlEnc(__s32 A_DskIdx, __u32 A_Offset, __s32 A_BlkIdx, __u8 A_OvwtId)
{
	// local -------------------
	// code --------------------
		return	A_Offset | (__u64)A_DskIdx | ((__u64)A_BlkIdx << E_GrFsNvtHndlBlkIdxPos) | ((__u64)A_OvwtId << E_GrFsNvtHndlOvwtIdPos);
}
//--------------------------------------------------------------------
void	GrFsNvtRecBufAdd(iovec* A_PtrIovs, __u32 A_Cnt)
{
		return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqRecBufAdd(A_PtrIovs,A_Cnt);
}
//--------------------------------------------------------------------
void	GrFsNvtSetup(__u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir)
{
		((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqSetup(A_NormalMap, A_MirrorMap, A_IsOvwtNrm, A_IsOvwtMir);
}
//--------------------------------------------------------------------
void	GrFsNvtAutoDelSet(__u32 A_Sec)
{
		((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqAtDelTimeSet(A_Sec);
}
//--------------------------------------------------------------------
BOOL8	GrFsNvtDiskFormatReq(__u32 A_DiskIdx, BOOL8 A_IsPost)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqDiskFormat((__u8)A_DiskIdx, A_IsPost);
}
//--------------------------------------------------------------------
BOOL8	GrFsNvtGroupDiskFormatReq(__u8 A_GroupMap, BOOL8 A_IsPost)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqGroupDiskFormat((__u8)A_GroupMap, A_IsPost);
}
//--------------------------------------------------------------------
__u8	GrFsNvtDiskInstMapGet(__u8* A_PtrRtBadMap)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqDiskInstMapGet(A_PtrRtBadMap);
}
//--------------------------------------------------------------------
__u32	GrFsNvtMonthDayMapGet(__u8 A_Group, __u32 A_TimeMonth)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqGroupGetMonthDayMap(A_Group,A_TimeMonth);
}
//--------------------------------------------------------------------
BOOL8	GrFsNvtMinRatbTblGet(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrBuf, BOOL8 A_IsPost, __u32 A_UserData)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqGroupGetMinRatbTbl(A_GroupType, A_TimeSt, A_ItemCnt, A_PtrBuf,A_IsPost,A_UserData);
}
//--------------------------------------------------------------------
__u8	GrFsNvtDiskRecInfoGet(__u32 A_DiskIdx, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqDiskRecInfoGet((__u8)A_DiskIdx, A_PtrRtTotalSize,A_PtrRtRecSize);
}
//--------------------------------------------------------------------
__u8	GrFsNvtGroupRecInfoGet(__u8 A_Group, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqGroupRecInfoGet(A_Group, A_PtrRtTotalSize, A_PtrRtRecSize);
}
//--------------------------------------------------------------------
BOOL8	GrFsNvtDiskModelNameGet(__u32 A_DiskIdx, WCHAR* A_StrRt)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqDiskModelNameGet((__u8)A_DiskIdx, A_StrRt);
}
//--------------------------------------------------------------------
BOOL8	GrFsNvtGroupRecTimeRngGet(__u8 A_Group, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd, Def_GrTime* A_PtrRtTimeLast)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqGroupGetRecTimeRng(A_Group, A_PtrRtTimeSt,A_PtrRtTimeEd, A_PtrRtTimeLast);
}
//--------------------------------------------------------------------
__u8	GrFsNvtDiskTemperatureGet(__u8 A_DiskIdx)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->DiskTemperatureGet(A_DiskIdx);
}
//--------------------------------------------------------------------
BOOL8	GrFsNvtLogAdd(Ptr_GrDvrLogItm A_PtrLog)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(NULL != V_GrFsNvtEnv.ObjMng)
		{
			((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqLogAdd(A_PtrLog);
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	GrFsNvtGroupRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd)
{
		return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqRecSizeEstmate(A_GroupType, A_ChMap, A_TimeSt, A_TimeEd);
}
//--------------------------------------------------------------------
__s32	GrFsNvtLogGetOpen(__u8 A_Group, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_MaxCnt,
	__u32 A_ChMask, __u64 A_TypeMask, Ptr_GrDvrLogItm A_PtrRtBuf, BOOL8 A_IsFrmMode)
{
	// local -------------------
	// code --------------------
		return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqLogGetOpen(A_Group, A_TimeSt, A_TimeEd, A_MaxCnt, A_ChMask, A_TypeMask,
			A_PtrRtBuf, A_IsFrmMode);
}
//--------------------------------------------------------------------
BOOL8	GrFsNvtLogGetChkDone(__s32 A_Hndl, __u32* A_PtrRtCnt)
{
		return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqLogGetChkDone(A_Hndl, A_PtrRtCnt);
}
//--------------------------------------------------------------------
void	GrFsNvtLogGetClose(__s32 A_Hndl)
{
		((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqLogGetClose(A_Hndl);
}
//--------------------------------------------------------------------
Def_GrErrCode	GrFsNvtLogGetFrameModeCheck(__s32 A_Hndl, __u32* A_PtrRtCnt)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqLogGetFrameModeCheck(A_Hndl, A_PtrRtCnt);
}
//--------------------------------------------------------------------
BOOL8	GrFsNvtLogGetFrameModeResume(__s32 A_Hndl)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqLogGetFrameModeResume(A_Hndl);
}
//--------------------------------------------------------------------
__u8	GrFsNvtRecInfoGet(__u8* A_PtrRtGroup, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize)
{
	return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqRecInfoGet(A_PtrRtGroup, A_PtrRtTotalSize, A_PtrRtRecSize);
}
//--------------------------------------------------------------------
BOOL8	GrFsNvtSmartRead(__u32 A_DskIdx, Ptr_GrFsNvtSmart A_PtrRtData)
{
		return	((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqDiskSmartRead((__u8)A_DskIdx, A_PtrRtData);
}
//--------------------------------------------------------------------
void	GrFsNvtGroupMakeFull(__u8 A_Group)
{
		((Cls_GrFsNvtIoRaid*)V_GrFsNvtEnv.ObjMng)->ReqGroupMakeFull(A_Group);
}
//--------------------------------------------------------------------
void	GrFsNvtDecBufInit()
{
	// local -------------------
		__s32		Tv_MemOfs;
		__s32		Tv_MemSize;
	// code --------------------
		GrDumyZeroMem(&V_GrFsNvtDecBuf, sizeof(St_GrFsNvtDecBuf));

#ifdef LINUX_APP
		V_GrFsNvtDecBuf.MemHndl	= open("/dev/mem", O_RDWR|O_SYNC);
#else
		V_GrFsNvtDecBuf.MemHndl	= 100;
#endif

		if(0 <= V_GrFsNvtDecBuf.MemHndl)
		{
			Tv_MemOfs		= E_GrFsNvtDecMemOffset;
			V_GrFsNvtDecBuf.BufSize	= E_GrFsNvtDecMemSize;

#ifdef LINUX_APP
			V_GrFsNvtDecBuf.PtrBuf	= (void*)mmap((void*)0, V_GrFsNvtDecBuf.BufSize, PROT_READ|PROT_WRITE, MAP_SHARED, 
				V_GrFsNvtDecBuf.MemHndl, Tv_MemOfs);

			if(0 > V_GrFsNvtDecBuf.PtrBuf)
			{
				DbgMsgPrint("GrFsNvtDecBufInit() - memory mapping error.\n");
			}
#else
			V_GrFsNvtDecBuf.PtrBuf	= (void*)malloc(V_GrFsNvtDecBuf.BufSize);
#endif
		}
		else
		{
			DbgMsgPrint("GrFsNvtDecBufInit() - memory handle error.\n");
		}
}
//--------------------------------------------------------------------
void	GrFsNvtDecBufFinish()
{
#ifdef LINUX_APP
		munmap(V_GrFsNvtDecBuf.PtrBuf, V_GrFsNvtDecBuf.BufSize);
		if(0 <= V_GrFsNvtDecBuf.MemHndl)
		{
			//close(V_GrFsNvtDecBuf.MemHndl);
		}
#else
		free(V_GrFsNvtDecBuf.PtrBuf);
		V_GrFsNvtDecBuf.PtrBuf	= NULL;
		V_GrFsNvtDecBuf.MemHndl	= -1;
#endif
}
//--------------------------------------------------------------------
void*	GrFsNvtDecBufGet(__s32 A_Size, __s32* A_PtrRtDecOfs)
{
	// local -------------------
		void*		Tv_Result;
		__s32		Tv_Ofs;
	// code --------------------
		Tv_Result	= NULL;

		Tv_Ofs	= V_GrFsNvtDecBuf.BufOfs + A_Size;
		if(V_GrFsNvtDecBuf.BufSize > Tv_Ofs)
		{
			*A_PtrRtDecOfs	= V_GrFsNvtDecBuf.BufOfs;
			Tv_Result	= (void*)((__s32)V_GrFsNvtDecBuf.PtrBuf + V_GrFsNvtDecBuf.BufOfs);
			V_GrFsNvtDecBuf.BufOfs	= Tv_Ofs;
		}
		else
		{
			*A_PtrRtDecOfs	= 0;
			Tv_Result		= V_GrFsNvtDecBuf.PtrBuf;
			V_GrFsNvtDecBuf.BufOfs	= A_Size;
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
