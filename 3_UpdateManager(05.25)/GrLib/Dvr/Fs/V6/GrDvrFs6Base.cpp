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

#include <Dvr/Fs/V6/GrDvrFs6Base.h>
#include <Dvr/Fs/V6/GrDvrFs6Disk.h>
#include <Dvr/Fs/V6/GrDvrFs6Mng.h>

#include	<stdlib.h>

//====================================================================
//local const

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

St_GrDvrFs6Env	V_GrDvrFs6Env;

//====================================================================
//--------------------------------------------------------------------
void	GrDvrFs6Prepare(Cls_GrTaskSvr* A_TskSvr, __u64 A_MacAdr, __u32 A_BkupHdSize, __u32 A_DiskMaxCnt, __u32 A_CacheSize)
{
	// local -------------------
		__u32	Tv_ItmCnt;
		__u32	Tv_BufSize;
	// code --------------------

		GrDumyZeroMem(&V_GrDvrFs6Env, sizeof(V_GrDvrFs6Env));

		if(0 != ((E_GrDvrFrmAlignSize - 1) & sizeof(St_GrDvrFs6FrmDayMap)))
		{
			DbgMsgPrint("GrDvrFs6Init - bad frame St_GrDvrFrmDayMap size %d \n", sizeof(St_GrDvrFs6FrmDayMap));
		}

		if(0 != ((E_GrDvrFrmAlignSize - 1) & sizeof(St_GrDvrFs6FrmTimeMap)))
		{
			DbgMsgPrint("GrDvrFs6Init - bad frame St_GrDvrFrmTimeMap size %d \n", sizeof(St_GrDvrFs6FrmTimeMap));
		}

		if(E_GrDvrFs6BlkHeadSize != sizeof(St_GrDvrFs6BlkBaseHead))
		{
			DbgMsgPrint("GrDvrFs6Init - bad St_GrDvrFsFrmLogPool size %d \n", sizeof(St_GrDvrFs6BlkBaseHead));
		}

		V_GrDvrFs6Env.DiskBaseId	=	A_MacAdr;
		V_GrDvrFs6Env.BkupHdSize	=	A_BkupHdSize;
		V_GrDvrFs6Env.DiskMaxCnt	=	A_DiskMaxCnt;

		// cache create
		Tv_ItmCnt	=	A_CacheSize	>> E_GrDvrFs6CchUnitShft;
		if(E_GrDvrFs6CchItemMaxCnt <= Tv_ItmCnt)
		{
			Tv_ItmCnt		=	E_GrDvrFs6CchItemMaxCnt;
		}
		Tv_BufSize	=	Tv_ItmCnt << E_GrDvrFs6CchUnitShft;

		V_GrDvrFs6Env.Cch.PtrBuf	=	(void*)malloc(Tv_BufSize);
		if(NULL != V_GrDvrFs6Env.Cch.PtrBuf)
		{
			V_GrDvrFs6Env.Cch.ItemCnt	=	Tv_ItmCnt;
		}
		else
		{
			DbgMsgPrint("GrDvrFs6Prepare - [CACHE] not enough memory!\n");
		}

		// create manager
		V_GrDvrFs6Env.ObjMng	=	(Cls_GrTaskCli*)new Cls_GrDvrFs6Mng(A_TskSvr);

		
}
//--------------------------------------------------------------------
void	GrDvrFs6Init(void)
{
		((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqInit();
}
//--------------------------------------------------------------------
void	GrDvrFs6Finish(void)
{
	// local -------------------
	// code --------------------
		// release manager
		delete	V_GrDvrFs6Env.ObjMng;
		V_GrDvrFs6Env.ObjMng	=	NULL;

		// release cache
		free(V_GrDvrFs6Env.Cch.PtrBuf);
		V_GrDvrFs6Env.Cch.PtrBuf	=	NULL;
}
//--------------------------------------------------------------------
void	GrDvrFs6HndlDec(__u64 A_Hndl, __s32* A_PtrRtDskIdx, __u32* A_PtrRtOffset, __s32* A_PtrRtBlkIdx, __u8* A_PtrRtOvwtId)
{
		*A_PtrRtOffset	=	A_Hndl & E_GrDvrFs6HndlOfsMask;
		*A_PtrRtDskIdx	=	(__s32)(A_Hndl & E_GrDvrFs6HndlGrpIdxMask);
		*A_PtrRtBlkIdx	=	(__s32)((A_Hndl >> E_GrDvrFs6HndlBlkIdxPos) & 0xFFFF);
		*A_PtrRtOvwtId	=	(__u8)((A_Hndl >> E_GrDvrFs6HndlOvwtIdPos) & 0xFF);
}
//--------------------------------------------------------------------
__u64	GrDvrFs6HndlEnc(__s32 A_DskIdx, __u32 A_Offset, __s32 A_BlkIdx, __u8 A_OvwtId)
{
	// local -------------------
	// code --------------------
		return	A_Offset | (__u64)A_DskIdx | ((__u64)A_BlkIdx << E_GrDvrFs6HndlBlkIdxPos) | ((__u64)A_OvwtId << E_GrDvrFs6HndlOvwtIdPos);
}
//--------------------------------------------------------------------
BOOL8	GrDvrFs6FrmVdoAdd(Ptr_GrDvrFrmHdVdo A_PtrFrm)
{
		return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ImmFrmVdoAdd(A_PtrFrm);
}
//--------------------------------------------------------------------
BOOL8	GrDvrFs6FrmAdoAdd(Ptr_GrDvrFrmHdAdo A_PtrFrm)
{
		return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ImmFrmAdoAdd(A_PtrFrm);
}
//--------------------------------------------------------------------
void	GrDvrFs6Setup(__u8 A_NormalMap, __u8 A_MirrorMap, BOOL8 A_IsOvwtNrm, BOOL8 A_IsOvwtMir)
{
		((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqSetup(A_NormalMap, A_MirrorMap, A_IsOvwtNrm, A_IsOvwtMir);
}
//--------------------------------------------------------------------
BOOL8	GrDvrFs6DiskFormatReq(__u32 A_DiskIdx, BOOL8 A_IsPost)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDiskFormat((__u8)A_DiskIdx, A_IsPost);
}
//--------------------------------------------------------------------
__u32	GrDvrFs6MonthDayMapGet(__u8 A_Group, __u32 A_TimeMonth)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqGroupGetMonthDayMap(A_Group,A_TimeMonth);
}
//--------------------------------------------------------------------
BOOL8	GrDvrFs6MinRatbTblGet(__u8 A_GroupType, Def_GrTime A_TimeSt, __u32 A_ItemCnt, Ptr_GrDvrRecMinMap A_PtrBuf)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqGroupGetMinRatbTbl(A_GroupType, A_TimeSt, A_ItemCnt, A_PtrBuf);
}
//--------------------------------------------------------------------
__u8	GrDvrFs6DiskRecInfoGet(__u32 A_DiskIdx, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDiskRecInfoGet((__u8)A_DiskIdx, A_PtrRtTotalSize,A_PtrRtRecSize);
}
//--------------------------------------------------------------------
__u8	GrDvrFs6GroupRecInfoGet(__u8 A_Group, __u64* A_PtrRtTotalSize, __u64* A_PtrRtRecSize)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqGroupRecInfoGet(A_Group, A_PtrRtTotalSize, A_PtrRtRecSize);
}
//--------------------------------------------------------------------
BOOL8	GrDvrFs6DiskModelNameGet(__u32 A_DiskIdx, WCHAR* A_StrRt)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDiskModelNameGet((__u8)A_DiskIdx, A_StrRt);
}
//--------------------------------------------------------------------
BOOL8	GrDvrFs6GroupRecTimeRngGet(__u8 A_Group, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqGroupGetRecTimeRng(A_Group, A_PtrRtTimeSt,A_PtrRtTimeEd);
}
//--------------------------------------------------------------------
__u8	GrDvrFs6DiskTemperatureGet(__u8 A_DiskIdx)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->DiskTemperatureGet(A_DiskIdx);
}
//--------------------------------------------------------------------
__u32	GrDvrFs6LogDayGet(__u8 A_Group, Def_GrTime A_Time, __u64 A_TypeMask, Ptr_GrDvrLogItm A_PtrBuf, __u32 A_MaxCnt)
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_ErrCode;
		Ptr_GrDvrFs6LogCtrl Tv_PtrCtrl;
	// code --------------------
		Tv_Result		=	0;
		Tv_PtrCtrl	=	(Ptr_GrDvrFs6LogCtrl)malloc(sizeof(St_GrDvrFs6LogCtrl));
		if(NULL != Tv_PtrCtrl)
		{
			Tv_PtrCtrl->PtrBuf			=	A_PtrBuf;
			Tv_PtrCtrl->MaxCnt			=	A_MaxCnt;
			Tv_PtrCtrl->TimeSt			=	A_Time - (A_Time % E_GrTimeSecOfDay);
			Tv_PtrCtrl->TimeEd			=	Tv_PtrCtrl->TimeSt + E_GrTimeSecOfDay - 1;
			Tv_PtrCtrl->ChMask			=	0xFFFFFFFF;
			Tv_PtrCtrl->TypeMask		=	A_TypeMask;
			Tv_ErrCode	=	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqLogReadOpen(A_Group, Tv_PtrCtrl);
			if(E_GrErrNone == Tv_ErrCode)
			{
				while(!Tv_PtrCtrl->IsFinish)
				{
					Tv_ErrCode	=	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqLogReadNext(A_Group, Tv_PtrCtrl);
					if(E_GrErrNone != Tv_ErrCode)
					{
						DbgMsgPrint("GrDvrFs6LogDayGet - read error (%d)\n", Tv_ErrCode);
					}
				}
				// update count
				Tv_Result	=	Tv_PtrCtrl->InCnt;
			}
			else
			{
				DbgMsgPrint("GrDvrFs6LogDayGet - open error (%d)\n",Tv_ErrCode);
			}
			// release buffer
			free(Tv_PtrCtrl);
		}
		else
		{
			DbgMsgPrint("GrDvrFs6LogDayGet - buffer allocate fail!\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrDvrFs6LogAdvGet(__u8 A_Group, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_ChMask, __u64 A_TypeMask, Ptr_GrDvrLogItm A_PtrBuf, __u32 A_MaxCnt)
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_ErrCode;
		Ptr_GrDvrFs6LogCtrl Tv_PtrCtrl;
	// code --------------------
		Tv_Result		=	0;

		if((A_TimeSt < A_TimeEd) && (0 != A_ChMask) && (0 != A_TypeMask))
		{
			Tv_PtrCtrl	=	(Ptr_GrDvrFs6LogCtrl)malloc(sizeof(St_GrDvrFs6LogCtrl));
			if(NULL != Tv_PtrCtrl)
			{
				Tv_PtrCtrl->PtrBuf			=	A_PtrBuf;
				Tv_PtrCtrl->MaxCnt			=	A_MaxCnt;
				Tv_PtrCtrl->TimeSt			=	A_TimeSt;
				Tv_PtrCtrl->TimeEd			=	A_TimeEd;
				Tv_PtrCtrl->ChMask			=	A_ChMask;
				Tv_PtrCtrl->TypeMask		=	A_TypeMask;
				Tv_ErrCode	=	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqLogReadOpen(A_Group, Tv_PtrCtrl);
				if(E_GrErrNone == Tv_ErrCode)
				{
					while(!Tv_PtrCtrl->IsFinish)
					{
						Tv_ErrCode	=	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqLogReadNext(A_Group, Tv_PtrCtrl);
						if(E_GrErrNone != Tv_ErrCode)
						{
							DbgMsgPrint("GrDvrFs6LogDayGet - read error (%d)\n", Tv_ErrCode);
						}
					}
					// update count
					Tv_Result	=	Tv_PtrCtrl->InCnt;
				}
				else
				{
					DbgMsgPrint("GrDvrFs6LogDayGet - open error (%d)\n", Tv_ErrCode);
				}
				// release buffer
				free(Tv_PtrCtrl);
			}
			else
			{
				DbgMsgPrint("GrDvrFs6LogDayGet - buffer allocate fail!\n");
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrDvrFs6LogAdd(Ptr_GrDvrLogItm A_PtrLog)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(NULL != V_GrDvrFs6Env.ObjMng)
		{
			((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqLogAdd(A_PtrLog);
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	GrDvrFs6GroupRecSizeEstmate(__u8 A_GroupType, __u32 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd)
{
		return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqRecSizeEstmate(A_GroupType, A_ChMap, A_TimeSt, A_TimeEd);
}
//--------------------------------------------------------------------
__s32	GrDvrFs6DmuxNew(__u8 A_MaxChCnt)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDmuxNew(A_MaxChCnt);
}
//--------------------------------------------------------------------
BOOL8	GrDvrFs6DmuxDelete(__s32 A_Hndl)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDmuxDelete(A_Hndl);
}
//--------------------------------------------------------------------
void	GrDvrFs6DmuxRecGroupSet(__s32 A_Hndl, __u8 A_RecGrpType)
{
	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDmuxRecGroupSet(A_Hndl, A_RecGrpType);
}
//--------------------------------------------------------------------
Ptr_GrDvrFs6ReadCtrl	GrDvrFs6DmuxReadCtrlPtrGet(__s32 A_Hndl, __u8 A_Ch)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDmuxReadCtrlPtrGet(A_Hndl, A_Ch);
}
//--------------------------------------------------------------------
__u64	GrDvrFs6DmuxDo(__s32 A_Hndl)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDmuxDo(A_Hndl);
}
//--------------------------------------------------------------------
__u64	GrDvrFs6DmuxReadyFrameRead(__s32 A_Hndl, Ptr_GrDvrFs6ReadFrameItem A_PtrBufTbl, __u32 A_BufCnt)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDmuxReadyFrameRead(A_Hndl, A_PtrBufTbl, A_BufCnt);
}
//--------------------------------------------------------------------
void	GrDvrFs6DmuxReadyFrameDone(__s32 A_Hndl, __u64 A_Map)
{
	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDmuxReadyFrameDone(A_Hndl, A_Map);
}
//--------------------------------------------------------------------
void	GrDvrFs6DmuxGoTime(__s32 A_Hndl, Def_GrTime A_Time)
{
	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDmuxGoTime(A_Hndl, A_Time);
}
//--------------------------------------------------------------------
void	GrDvrFs6DmuxDirSet(__s32 A_Hndl, BOOL8 A_IsDirBack)
{
	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDmuxDirSet(A_Hndl, A_IsDirBack);
}
//--------------------------------------------------------------------
void	GrDvrFs6DmuxVdoChAbleSet(__s32 A_Hndl, __u32 A_Map)
{
	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDmuxVdoChAbleSet(A_Hndl, A_Map);
}
//--------------------------------------------------------------------
void	GrDvrFs6DmuxAdoChAbleSet(__s32 A_Hndl, __u32 A_Map)
{
	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDmuxAdoChAbleSet(A_Hndl, A_Map);
}
//--------------------------------------------------------------------
void	GrDvrFs6DmuxVdoFrmMaskSet(__s32 A_Hndl, __u8 A_Ch, __u8 A_Mask)
{
	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ReqDmuxVdoFrmMaskSet(A_Hndl, A_Ch, A_Mask);
}
//--------------------------------------------------------------------
__u8	GrDvrFs6DmuxRunStatGet(__s32 A_Hndl)
{
	return	((Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng)->ImmDmuxRunStaGet(A_Hndl);
}
//--------------------------------------------------------------------

