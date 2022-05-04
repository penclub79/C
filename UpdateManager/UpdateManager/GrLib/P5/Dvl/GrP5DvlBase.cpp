/*
	Platform 5

*/

//--------------------------------------------------------------------
// uses

#include <P5/Dvl/GrP5DvlBase.h>

#include <P5/GrP5Base.h>

#include <P5/Dvl/GrP5DvlMng.h>
#include <P5/GrP5WinBase.h>
#include <P5/Dvl/GrP5DvlRefPrs.h>
#include <P5/GrP5ConBase.h>

#include <P5/GrP5Render.h>

#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <GrImgBase.h>
#include <GrDebug.h>

//--------------------------------------------------------------------
// local constant


//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;

St_GrP5DvlEnv	V_GrP5DvlEnv;

// class id name
St_GrP5DvlInfoIdName	V_GrP5DvlClassInfoTbl[]	=	
{
	{E_GrP5ClassIdWinStd, L"Window (WinStd)"},
	{E_GrP5ClassIdWinChild, L"Child (WinChild)"},
	{E_GrP5ClassIdLblLt, L"Label Local text (LblLt)"},
	{E_GrP5ClassIdLblTid, L"Label TextId (LblTid)"},
	{E_GrP5ClassIdLblRef, L"Label Reference (LblRef)"},
	{E_GrP5ClassIdLblTimeReg, L"Label time reg (LblTimeReg)"},
	{E_GrP5ClassIdLblTimeView, L"Label time View (LblTimeView)"},
	{E_GrP5ClassIdBtnImgTid, L"Button Image TextId (BtnImgTid)"},
	{E_GrP5ClassIdBtnLtxt, L"Button L-Text (BtnLtxt)"},
	{E_GrP5ClassIdBtnTid, L"Button TextId (BtnTid)"},
	{E_GrP5ClassIdBtnTabTid, L"Button Tab TextId (BtnTabTid)"},
	{E_GrP5ClassIdBtnTime, L"Button Time (BtnTime)"},
	{E_GrP5ClassIdBtnSint, L"Button Setup Number (BtnSint)"},
	{E_GrP5ClassIdBtnStxt, L"Button Setup Text (BtnStxt)"},
	{E_GrP5ClassIdBtnVkey, L"Button Virtual key (BtnVkey)"},
	{E_GrP5ClassIdChkImgLtLc, L"Check Img L-Val (ChkImgLtLc)"},
	{E_GrP5ClassIdChkImgTiLc, L"Check Img Ti L-Val (ChkImgTiLc)"},
	{E_GrP5ClassIdChkTiLc, L"Check TxtId L-Val (ChkTiLc)"},
	{E_GrP5ClassIdChkTiReg, L"Check TxtId reg (ChkTiReg)"},
	{E_GrP5ClassIdCalMark, L"Calendar Mark (CalMark)"},
	{E_GrP5ClassIdCbxLtLint, L"Combo L-Text L-Val (CbxLtLint)"},
	{E_GrP5ClassIdCbxLtSint, L"Combo L-Text Setup (CbxLtSint)"},
	{E_GrP5ClassIdCbxTiSint, L"Combo TextId Setup (CbxTiSint)"},
	{E_GrP5ClassIdGridSmd, L"Grid Setup Motion (GridSmd)"},
	{E_GrP5ClassIdGridSrscd, L"Grid Setup schedule (GridSrscd)"},
	{E_GrP5ClassIdGrpHzDmap, L"Graph Hz Day map (GrpHzDmap)"},
	{E_GrP5ClassIdGrpHzT1, L"Graph Hz Type 1 (GrpHzT1)"},
	{E_GrP5ClassIdGrpHzT2, L"Graph Hz Type 2 (GrpHzT2)"},
	{E_GrP5ClassIdSpinNumLint, L"Spin Number L-val (SpinNumLint)"},
	{E_GrP5ClassIdSpinNumSint, L"Spin Number Setup (SpinNumSint)"},
	{E_GrP5ClassIdSpinLtLint, L"Spin L-Text L-Val (SpintLtLint)"},
	{E_GrP5ClassIdSpinStxtRint, L"Spin S-Text Reg (SpintStxtRint)"},
	{E_GrP5ClassIdSlidHzLint, L"Slid Hz L-val (SlidHzLint)"},
	{E_GrP5ClassIdSlidHzSint, L"Slid Hz S-val (SlidHzSint)"},
	{E_GrP5ClassIdPnlAim, L"Pannel Aim (PnlAim)"},
	{E_GrP5ClassIdPnlStd, L"Pannel Standard (PnlStd)"},
	{E_GrP5ClassIdPnlDir, L"Pannel direction (PnlDir)"},
	{E_GrP5ClassIdPnlDrag, L"Pannel drag (PnlDrag)"},
	{E_GrP5ClassIdPnlGrpBar, L"Pannel graph bar (PnlGrpBar)"},
	{E_GrP5ClassIdPnlGrpLine, L"Pannel graph line (PnlGrpLine)"},
	{E_GrP5ClassIdPrgsRint, L"Progress Reg (PrgsRint)"},
	{E_GrP5ClassIdArwStd, L"Arrow standard (ArwStd)"},
	{E_GrP5ClassIdTblStd, L"Table standard (TblStd)"},
	{E_GrP5ClassIdQuadT1, L"Quad Type 1 (QuadT1)"},
	{E_GrP5ClassIdTabStd, L"Tab Standard (TabStd)"},
	{E_GrP5ClassIdTabTiRel, L"Tab text icon relate (TabTiRel)"},
	{E_GrP5ClassIdNone, L""}
};

// virtual keyboard type name
Def_WstrName	V_GrP5DvlVkbdNameTbl[E_GrP5VkbdCnt]	=	
{
	{L"Normal"},
	{L"Decimal"},
	{L"Hexadecimal"},
	{L"Password"},
	{L"URL"},
	{L"Mail Address"},
};

St_GrDvrRecMinMap	V_GrP5DvlTestRecMap[E_GrTimeMinOfDay];

//====================================================================
//--------------------------------------------------------------------
Def_GrErrCode	GrP5DvlInit(Cls_GrTaskSvr* A_TaskSvr)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_WkCh;
		__u8	Tv_TmpVal;
	// code --------------------
		Tv_Result	=	E_GrErrInitFail;

		// reset P5 env
		GrDumyZeroMem(&V_GrP5Env, sizeof(V_GrP5Env));

		Tv_TmpVal	=	0;
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrTimeMinOfDay; Tv_WkIdx++)
		{

			for( Tv_WkCh = 0; Tv_WkCh < E_GrDvrMaxChCnt; Tv_WkCh++)
			{
				V_GrP5DvlTestRecMap[Tv_WkIdx].ChAtb[Tv_WkCh]	=	Tv_TmpVal;
				Tv_TmpVal	 ++;
				Tv_TmpVal	=	Tv_TmpVal & 7;
			}
		}

		// reset DVL env
		GrDumyZeroMem(&V_GrP5DvlEnv, sizeof(V_GrP5DvlEnv));

		// skin initialize
		GrP5SkinInit();

		// create object
		V_GrP5Env.TskMng	=	A_TaskSvr;
		V_GrP5Env.ObjMng	=	(Cls_GrTaskCli*)new Cls_GrP5DvlMng(A_TaskSvr);
		if (NULL != V_GrP5Env.ObjMng)
		{
			// create renderer
			V_GrP5Env.TskRnd		=	(Cls_GrTaskSvr*) new Cls_GrTaskSvr(7777, TRUE);
			V_GrP5Env.ObjRnd		=	(Cls_GrTaskCli*) new Cls_GrP5Render(V_GrP5Env.TskRnd);

			// create reference parser
			V_GrP5DvlEnv.TaskRefPrs	=	(Cls_GrTaskSvr*) new Cls_GrTaskSvr(100, TRUE);
			if(NULL != V_GrP5DvlEnv.TaskRefPrs)
			{
				// create parser
				V_GrP5DvlEnv.ObjRefPrs	=	(Cls_GrTaskCli*)new Cls_GrP5DvlRefPrs(V_GrP5DvlEnv.TaskRefPrs);
			}

			GrP5RintBufSet(V_GrP5DvlEnv.Debug.RintTbl, 4 * E_GrP5DvlRintMaxCnt);
			GrP5RtxtBufSet(V_GrP5DvlEnv.Debug.RtxtTbl, sizeof(Def_WstrTag) * E_GrP5DvlRtxtMaxCnt, 64);

			GrP5RenderRun(TRUE);

			GrP5DvlPrjNew(E_GrFccARGB8888);

			Tv_Result	=	E_GrErrNone;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	GrP5DvlFinish(void)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
	// code --------------------
		GrP5RenderRun(FALSE);

		Tv_Result	=	GrP5Finish();

		// develop enviroment release
		if(NULL != V_GrP5DvlEnv.ObjRefPrs)
		{
			delete	V_GrP5DvlEnv.ObjRefPrs;
			V_GrP5DvlEnv.ObjRefPrs	=	NULL;
		}

		if(NULL != V_GrP5Env.ObjRnd)
		{
			delete	V_GrP5Env.ObjRnd;
			V_GrP5Env.ObjRnd	=	NULL;
		}

		// task release
		if(NULL != V_GrP5DvlEnv.TaskRefPrs)
		{
			delete	V_GrP5DvlEnv.TaskRefPrs;
			V_GrP5DvlEnv.TaskRefPrs	=	NULL;
		}

		if(NULL != V_GrP5Env.TskRnd)
		{
			delete	V_GrP5Env.TskRnd;
			V_GrP5Env.TskRnd	=	NULL;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	GrP5DvlClassNameGet(__u16 A_ClassId)
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		Tv_Result		=	NULL;

		Tv_WkIdx	=	0;
		while(E_GrP5ClassIdNone != V_GrP5DvlClassInfoTbl[Tv_WkIdx].Id)
		{
			// check match
			if(A_ClassId == V_GrP5DvlClassInfoTbl[Tv_WkIdx].Id)
			{
				// found
				Tv_Result	=	V_GrP5DvlClassInfoTbl[Tv_WkIdx].StrName;
				break;
			}
			// next
			Tv_WkIdx ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrP5DvlScptWindowBuild(void* A_PtrBuf, __u16 A_ScptId, __u8 A_Layer, Ptr_GrP5RelRect A_PtrRelRect, __u16 A_ClassId)
{
	// local -------------------
		__u32	Tv_Result;
		Ptr_GrP5ScptObjHead	Tv_PtrSoHd;
		Ptr_GrP5ScptObjEnd	Tv_PtrSoEd;
	// code --------------------
		// build head
		if (NULL != A_PtrBuf)
		{
			Tv_PtrSoHd	=	(Ptr_GrP5ScptObjHead)A_PtrBuf;
			Tv_PtrSoHd->Tcc				=	E_GrP5ScptObjHdTcc;
			Tv_PtrSoHd->ClassId		=	A_ClassId;
			Tv_PtrSoHd->HdPara			=	A_Layer;
			Tv_PtrSoHd->ParaSize		=	0;
			Tv_PtrSoHd->RelRect		=	*A_PtrRelRect;
			Tv_PtrSoHd->ScptId			=	A_ScptId;
		}
		Tv_Result		=	sizeof(St_GrP5ScptObjHead);

		// build para

		// build tail
		if (NULL != A_PtrBuf)
		{
			Tv_PtrSoEd	=	(Ptr_GrP5ScptObjEnd)((Def_GrCalPtr)A_PtrBuf + (Def_GrCalPtr)Tv_Result);
			Tv_PtrSoEd->Tcc				=	E_GrP5ScptObjEdTcc;
			Tv_PtrSoEd->ScptId			=	A_ScptId;
		}
		Tv_Result		=	Tv_Result + sizeof(St_GrP5ScptObjEnd);

		// update heder total size
		if (NULL != A_PtrBuf)
		{
			Tv_PtrSoHd->TotalSize	=	Tv_Result;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5DvlScptWindowCreate(__u16 A_ScptId, __u16 A_ClassId)
{
		return	((Cls_GrP5DvlMng*)V_GrP5Env.ObjMng)->DvlWindowScriptCreate(A_ScptId, A_ClassId);
}
//--------------------------------------------------------------------
BOOL8	GrP5DvlScptWindowDelete(__u16 A_ScptId)
{
		return	((Cls_GrP5DvlMng*)V_GrP5Env.ObjMng)->DvlWindowScriptDelete(A_ScptId);
}
//--------------------------------------------------------------------
BOOL8	GrP5DvlScptWindowSet(__u16 A_ScptId, void* A_PtrScpt, __u32 A_ScptSize)
{
		return	((Cls_GrP5DvlMng*)V_GrP5Env.ObjMng)->DvlWindowScriptSet(A_ScptId, A_PtrScpt,A_ScptSize);
}
//--------------------------------------------------------------------
__u16	GrP5DvlWindowScptDecode(__u16 A_ScptId, __u8 A_ConIdx)
{
	// local -------------------
		__u16	Tv_Result;
		void*	Tv_PtrScpt;
		Cls_GrP5WinBase*	Tv_UoWin;
		__u32	Tv_Decoded;
	// code --------------------
		Tv_Result	=	E_GrP5ObjIdNone;

		// check script id range
		if ((E_GrP5ScptIdWinBase <= A_ScptId) && ((E_GrP5ScptIdWinBase + E_GrP5ScptWinMaxCnt) > A_ScptId))
		{
			// get script
			Tv_PtrScpt	=	GrP5ScriptGet(A_ScptId);
			if (NULL != Tv_PtrScpt)
			{
				Tv_UoWin	=	(Cls_GrP5WinBase*)GrP5ScriptDecWin(Tv_PtrScpt, A_ConIdx, &Tv_Decoded);
				if (NULL != Tv_UoWin)
				{
					// update result
					Tv_Result	=	Tv_UoWin->ObjIdGet();
				}
			}
			else
			{
				DbgMsgPrint("GrP5DvlWindowScptDecode - script (%d) not found\n", A_ScptId);
			}
		}
		else
		{
			DbgMsgPrint("GrP5DvlWindowScptDecode - bad script id (%d)\n", A_ScptId);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrP5DvlWinInfo	GrP5DvlWindowInfoPtrGet(__u16 A_ScptId)
{
	// local -------------------
		Ptr_GrP5DvlWinInfo	Tv_Result;
		__u16	Tv_TblIdx;
	// code --------------------
		Tv_Result	=	NULL;

		// check script id range
		if ((E_GrP5ScptIdWinBase <= A_ScptId) && ((E_GrP5ScptIdWinBase + E_GrP5ScptWinMaxCnt) > A_ScptId))
		{
			Tv_TblIdx	=	A_ScptId - E_GrP5ScptIdWinBase;
			Tv_Result	=	&V_GrP5DvlEnv.PrjCtrl.WinInfoTbl[Tv_TblIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrP5DvlCtrlIdInfo	GrP5DvlCtrlIdInfoPtrGet(__u16 A_CtrlId)
{
	// local -------------------
		Ptr_GrP5DvlCtrlIdInfo	Tv_Result;
		__u16	Tv_TblIdx;
	// code --------------------
		Tv_Result	=	NULL;

		// check script id range
		if((E_GrP5CtrlIdBase <= A_CtrlId) && ((E_GrP5CtrlIdBase + E_GrP5CtrlIdMaxCnt) > A_CtrlId))
		{
			Tv_TblIdx	=	A_CtrlId - E_GrP5CtrlIdBase;
			Tv_Result	=	&V_GrP5DvlEnv.PrjCtrl.CtrlIdInfoTbl[Tv_TblIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrP5DvlVarIdInfo	GrP5DvlRintInfoPtrGet(__u16 A_VarId)
{
	// local -------------------
		Ptr_GrP5DvlVarIdInfo	Tv_Result;
		__u16	Tv_VarIdx;
	// code --------------------
		Tv_Result	=	NULL;

		if ((E_GrP5RegIdBase <= A_VarId) && ((E_GrP5RegIdBase + E_GrP5DvlRintMaxCnt) > A_VarId))
		{
			Tv_VarIdx	=	A_VarId - E_GrP5RegIdBase;
			Tv_Result	=	&V_GrP5DvlEnv.PrjCtrl.RintInfoTbl[Tv_VarIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrP5DvlVarIdInfo	GrP5DvlRtxtInfoPtrGet(__u16 A_VarId)
{
	// local -------------------
		Ptr_GrP5DvlVarIdInfo	Tv_Result;
		__u16	Tv_VarIdx;
	// code --------------------
		Tv_Result	=	NULL;

		if ((E_GrP5RegIdBase <= A_VarId) && ((E_GrP5RegIdBase + E_GrP5DvlRtxtMaxCnt) > A_VarId))
		{
			Tv_VarIdx	=	A_VarId - E_GrP5RegIdBase;
			Tv_Result	=	&V_GrP5DvlEnv.PrjCtrl.RtxtInfoTbl[Tv_VarIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	GrP5DvlSlaveScptIdAlloc(void)
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_BitPos;
		__u32*	Tv_PtrChkU32;
		__u8*		Tv_PtrChkU8;
		__u32	Tv_U8Idx;
		__u32	Tv_BitIdx;
		__u8	Tv_Mask;
	// code --------------------
		Tv_Result		=	0;
		// find empty id
		Tv_BitPos		=	0;
		Tv_PtrChkU32	=	(__u32*)V_GrP5DvlEnv.PrjCtrl.ScptSlvIdMap;

		while(Tv_BitPos < E_GrP5DvlScptSlaveMaxCnt)
		{
			// check empty
			if(0xFFFFFFFF != (*Tv_PtrChkU32))
			{
				// found
				Tv_PtrChkU8	=	(__u8*)((Def_GrCalPtr)Tv_PtrChkU32);
				for(Tv_U8Idx = 0; Tv_U8Idx < 4; Tv_U8Idx++)
				{
					if(0xFF != (*Tv_PtrChkU8))
					{
						// find first bit
						Tv_Mask	=	1;
						for(Tv_BitIdx = 0; Tv_BitIdx < 8; Tv_BitIdx++)
						{
							if(0 == (Tv_Mask & (*Tv_PtrChkU8)))
							{
								// found
								Tv_Result		=	Tv_BitPos + E_GrP5ScptIdSlaveBase;
								*Tv_PtrChkU8	=	(*Tv_PtrChkU8) | Tv_Mask;
								break;
							}
							// next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_BitPos ++;
						}
						break;
					}
					// next
					Tv_BitPos		=	Tv_BitPos + 8;
					Tv_PtrChkU8 ++;
				}

				break;
			}

			// next
			Tv_BitPos		=	Tv_BitPos + 32;
			Tv_PtrChkU32	++;
		}
		
		if(0 == Tv_Result)
		{
			DbgMsgPrint("GrP5DvlSlaveScptIdAlloc - slave script id is full!\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrP5DvlSlaveScptIdFree(__u16 A_ScptId)
{
	// local -------------------
		__u16	Tv_U8Idx;
		__u8	Tv_U8Mask;
	// code --------------------
		// check script id range
		if((E_GrP5ScptIdSlaveBase <= A_ScptId) && ((E_GrP5ScptIdSlaveBase + E_GrP5DvlScptSlaveMaxCnt) > A_ScptId))
		{
			// make address
			Tv_U8Idx	=	(A_ScptId - E_GrP5ScptIdSlaveBase) >> 3;
			Tv_U8Mask	=	1 << ((A_ScptId - E_GrP5ScptIdSlaveBase) & 7);
			// check allocated
			if(0 != (Tv_U8Mask & V_GrP5DvlEnv.PrjCtrl.ScptSlvIdMap[Tv_U8Idx]))
			{
				V_GrP5DvlEnv.PrjCtrl.ScptSlvIdMap[Tv_U8Idx]	=	V_GrP5DvlEnv.PrjCtrl.ScptSlvIdMap[Tv_U8Idx] & (~Tv_U8Mask);
			}
			else
			{
				DbgMsgPrint("GrP5DvlSlaveScptIdFree - slave script id(%d) is not exist!\n", A_ScptId);
			}
		}
		else
		{
			DbgMsgPrint("GrP5DvlSlaveScptIdFree - bad slave script id(%d)!\n", A_ScptId);
		}
}
//--------------------------------------------------------------------
void	GrP5DvlScptSlaveIdFree(void* A_PtrScpt, __u32 A_ScptSize)
{
	// local -------------------
		__u32	Tv_Progress;
		__u16*	Tv_PtrTcc;

		Ptr_GrP5ScptObjHead	Tv_PtrSoHd;
	// code --------------------
		Tv_Progress	=	0;
		while ((Tv_Progress + 2) < A_ScptSize)
		{
			Tv_PtrTcc	=	(__u16*)((Def_GrCalPtr)A_PtrScpt + (Def_GrCalPtr)Tv_Progress);
			// check header type
			if (E_GrP5ScptObjHdTcc == (*Tv_PtrTcc))
			{
				// script header
				Tv_PtrSoHd	=	(Ptr_GrP5ScptObjHead)Tv_PtrTcc;

				// check script id range
				if (E_GrP5ScptIdSlaveBase <= Tv_PtrSoHd->ScptId)
				{
					if ((E_GrP5ScptIdSlaveBase + E_GrP5DvlScptSlaveMaxCnt) > Tv_PtrSoHd->ScptId)
					{
						// delete script id
						GrP5DvlSlaveScptIdFree(Tv_PtrSoHd->ScptId);
					}
					else
					{
						DbgMsgPrint("GrP5DvlScptSlaveIdFree - bad script id (%d)! offset is %d\n", Tv_PtrSoHd->ScptId, Tv_Progress);
					}
				}

				// go next
				Tv_Progress	=	Tv_Progress + sizeof(St_GrP5ScptObjHead)+ (__u32)Tv_PtrSoHd->ParaSize;
			}
			else if (E_GrP5ScptObjEdTcc == (*Tv_PtrTcc))
			{
				// tail header
				Tv_Progress	=	Tv_Progress + sizeof(St_GrP5ScptObjEnd);
			}
			else
			{
				// unknown
				DbgMsgPrint("GrP5DvlScptSlaveIdFree - bad script tcc! offset is %d\n",Tv_Progress);
				break;
			}
		}

}
//--------------------------------------------------------------------
void	GrP5DvlPrjNew(__u32 A_PxFmt)
{
	// local -------------------
	// code --------------------

		// release object
		if(NULL != V_GrP5Env.ConTbl[0].ObjCon)
		{
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[0].ObjCon)->RtlUiObjReleaseAll();
		}
		if(NULL != V_GrP5Env.ConTbl[1].ObjCon)
		{
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[1].ObjCon)->RtlUiObjReleaseAll();
		}

		((Cls_GrP5DvlMng*)V_GrP5Env.ObjMng)->DvlPrjReset();
		V_GrP5DvlEnv.PrjCtrl.UiInfoBasic.FccScr		=	A_PxFmt;

		// update color
		V_GrP5DvlEnv.ClrUoMark1	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 192, 192, 192), V_GrP5DvlEnv.PrjCtrl.UiInfoBasic.FccScr);
		V_GrP5DvlEnv.ClrUoMark2	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 64, 64, 64), V_GrP5DvlEnv.PrjCtrl.UiInfoBasic.FccScr);
}
//--------------------------------------------------------------------
BOOL8	GrP5DvlPrjSave(WCHAR* A_StrFn, BOOL8 A_IsPrjInc)
{
		return	((Cls_GrP5DvlMng*)V_GrP5Env.ObjMng)->DvlPrjSave(A_StrFn, A_IsPrjInc);
}
//--------------------------------------------------------------------
BOOL8	GrP5DvlPrjLoad(WCHAR* A_StrFn)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		// release object
		if(NULL != V_GrP5Env.ConTbl[0].ObjCon)
		{
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[0].ObjCon)->RtlUiObjReleaseAll();
		}
		if(NULL != V_GrP5Env.ConTbl[1].ObjCon)
		{
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[1].ObjCon)->RtlUiObjReleaseAll();
		}

		Tv_Result	=	((Cls_GrP5DvlMng*)V_GrP5Env.ObjMng)->DvlPrjLoad(A_StrFn);
		if(Tv_Result)
		{
			// update color
			V_GrP5DvlEnv.ClrUoMark1	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 192, 192, 192), V_GrP5DvlEnv.PrjCtrl.UiInfoBasic.FccScr);
			V_GrP5DvlEnv.ClrUoMark2	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 64, 64, 64), V_GrP5DvlEnv.PrjCtrl.UiInfoBasic.FccScr);
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrP5DvlDbgRegChgEvtSet(Fnc_GrP5DvlRegChgEvt A_Fnc)
{
		V_GrP5DvlEnv.Debug.FncEvtRegChg	=	A_Fnc;
}
//--------------------------------------------------------------------
void	GrP5DvlDbgRegChgNotify(BOOL8 A_IsText, __u16 A_VarId)
{
		if (NULL != V_GrP5DvlEnv.Debug.FncEvtRegChg)
		{
			V_GrP5DvlEnv.Debug.FncEvtRegChg(A_IsText, A_VarId);
		}
}
//--------------------------------------------------------------------
void	GrP5DvlRefHeaderChgEvtSet(Fnc_GrP5DvlRefChgEvt A_Fnc)
{
		V_GrP5DvlEnv.FncEvtRefChg	=	A_Fnc;
}
//--------------------------------------------------------------------
void	GrP5DvlRefHeaderChgNotify(void)
{
		if (NULL != V_GrP5DvlEnv.FncEvtRefChg)
		{
			V_GrP5DvlEnv.FncEvtRefChg();
		}

}
//--------------------------------------------------------------------
void*	GrP5DvlUiObjEditableFindByPos(__u8 A_ConIdx, __s32 A_PosX, __s32 A_PosY)
{
	// local -------------------
		void*	Tv_Result;
		__u8	Tv_LayIdx;
		__u8	Tv_WinIdx;
		Ptr_GrP5ConCtrl	Tv_PtrConCtl;
	// code --------------------
		Tv_Result	=	NULL;

		Tv_PtrConCtl	=	&V_GrP5Env.ConTbl[A_ConIdx];

		// find window
		Tv_LayIdx	=	E_GrP5LayerMaxCnt;
		do
		{
			Tv_LayIdx --;

			// check window
			for(Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
			{
				if(NULL != Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)
				{
					// check position
					Tv_Result	=	(void*)((Cls_GrP5UoBase*)Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)->DvlUoEditableFindByPos(A_PosX, A_PosY);
					if(NULL != Tv_Result)
					{
						break;
					}
				}
			}
		} while(0 != Tv_LayIdx);

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5DvlScriptDecSlave(void* A_PtrScpt, void* A_ObjPrnt, __u32* A_PtrRtDecoded, Ptr_GrP5RelRect A_PtrRelRect, BOOL8 A_IsScptIdNone)
{
	// local -------------------
		void*	Tv_Result;
		__u32	Tv_TtSize;
		__u32	Tv_DecSize;
		Ptr_GrP5ScptObjHead	Tv_PtrSoHd;
		__u16	Tv_ScptId;
		Cls_GrP5UoBase*	Tv_UoThis;
		Ptr_GrP5ScptObjEnd	Tv_PtrSoEd;
		void*	Tv_PtrSlvScpt;
		__u32	Tv_SlvScptSize;
		Ptr_GrP5RelRect	Tv_PtrRc;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtDecoded	=	0;
		Tv_PtrSoHd	=	(Ptr_GrP5ScptObjHead)A_PtrScpt;
		// check header
		if(E_GrP5ScptObjHdTcc == Tv_PtrSoHd->Tcc)
		{
			// get info
			Tv_TtSize		=	Tv_PtrSoHd->TotalSize;
			Tv_ScptId		=	Tv_PtrSoHd->ScptId;
			if(A_IsScptIdNone)
			{
#ifdef GR_P5_EDITOR
				Tv_ScptId	=	E_GrP5DvlScptIdReqAlloc;
#else
				Tv_ScptId	=	E_GrP5ScptIdNone;
#endif
			}

			Tv_PtrRc	=	&Tv_PtrSoHd->RelRect;
			if(NULL != A_PtrRelRect)
			{
				Tv_PtrRc		=	A_PtrRelRect;
			}

			// init
			Tv_UoThis	=	(Cls_GrP5UoBase*)GrP5UiObjCreateByClassId(Tv_PtrSoHd->ClassId, (Cls_GrP5UoBase*)A_ObjPrnt, Tv_PtrRc, Tv_ScptId);
			Tv_Result	=	(void*)Tv_UoThis;

			if(NULL != Tv_UoThis)
			{
				// decode parametter
				Tv_UoThis->RtlScptParaUpdt(Tv_PtrSoHd);

				// decode sub
				Tv_PtrSlvScpt		=	(void*)((Def_GrCalPtr)A_PtrScpt + sizeof(St_GrP5ScptObjHead)+ (Def_GrCalPtr)Tv_PtrSoHd->ParaSize);
				Tv_SlvScptSize	=	Tv_PtrSoHd->TotalSize - sizeof(St_GrP5ScptObjHead)- (__u32)Tv_PtrSoHd->ParaSize - sizeof(St_GrP5ScptObjEnd);
				if(0 != Tv_SlvScptSize)
				{
					Tv_DecSize	=	GrP5ScriptDecSub(Tv_PtrSlvScpt, Tv_SlvScptSize, (void*)Tv_UoThis, A_IsScptIdNone);
					if(Tv_DecSize != Tv_SlvScptSize)
					{
						DbgMsgPrint("GrP5DvlScriptDecSlave - bad sub script size at script (%d).\n", Tv_ScptId);
						Tv_Result	=	NULL;	// fail
					}
				}

				// check end script
				Tv_PtrSoEd	=	(Ptr_GrP5ScptObjEnd)((Def_GrCalPtr)A_PtrScpt + (Def_GrCalPtr)Tv_PtrSoHd->TotalSize - sizeof(St_GrP5ScptObjEnd));
				if((E_GrP5ScptObjEdTcc != Tv_PtrSoEd->Tcc) || (Tv_PtrSoHd->ScptId != Tv_PtrSoEd->ScptId))
				{
					DbgMsgPrint("GrP5DvlScriptDecSlave - bad slave script end at script (%d).\n", Tv_ScptId);
					Tv_Result	=	NULL;	// fail
				}
			}

			// check success
			if(NULL != Tv_Result)
			{
				// create success
				Tv_UoThis->RtlDoEvtScptDecDone(FALSE);

				// update decoded size
				*A_PtrRtDecoded	=	Tv_PtrSoHd->TotalSize;
			}
			else
			{
				// create fail
				// release
				delete	Tv_UoThis;
				Tv_UoThis	=	NULL;
			}

		}
		else
		{
			DbgMsgPrint("GrP5DvlScriptDecSlave - bad script header.\n");
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
WCHAR*	GrP5DvlWorkEnvPathGet(__u8 A_Idx)
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if (E_GrP5DvlWorkEnvPathCnt > A_Idx)
		{
			Tv_Result	=	V_GrP5DvlEnv.PrjCtrl.WorkEnv.StrPathTbl[A_Idx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrP5DvlSetupInfoGet(__u16 A_Adr, WCHAR* A_StrRt)
{
	// local -------------------
		WCHAR*	Tv_StrWk;
		__u32	Tv_AdrCnt;
		__s32	Tv_AdrIdx;
		__u32	Tv_WkIdx;
		__u16	Tv_Base;
	// code --------------------
		GrStrClear(A_StrRt);
		Tv_StrWk	=	A_StrRt;

		// find address
		Tv_Base		=	0;
		Tv_AdrIdx	=	-1;
		Tv_AdrCnt	=	V_GrP5DvlEnv.PrjCtrl.StpAdrCnt;
		if(0 != Tv_AdrCnt)
		{
			for( Tv_WkIdx = 0; Tv_WkIdx < Tv_AdrCnt; Tv_WkIdx++)
			{
				if(A_Adr >= V_GrP5DvlEnv.PrjCtrl.StpAdrTbl[Tv_WkIdx].Ofs)
				{
					if(0 > Tv_AdrIdx)
					{
						// update
						Tv_AdrIdx	=	(__u32)Tv_WkIdx;
					}
					else
					{
						// compare address
						if(V_GrP5DvlEnv.PrjCtrl.StpAdrTbl[Tv_AdrIdx].Ofs < V_GrP5DvlEnv.PrjCtrl.StpAdrTbl[Tv_WkIdx].Ofs)
						{
							// update
							Tv_AdrIdx	=	(__u32)Tv_WkIdx;
						}
					}
				}
			}
		}

		if(0 <= Tv_AdrIdx)
		{
			// copy name
			Tv_StrWk	=	GrStrWcopy(Tv_StrWk, V_GrP5DvlEnv.PrjCtrl.StpAdrTbl[Tv_AdrIdx].StrName);
			Tv_Base		=	V_GrP5DvlEnv.PrjCtrl.StpAdrTbl[Tv_AdrIdx].Ofs;
		}

		Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x5B);
		GrStrDwordToHexWstr(Tv_StrWk, (__u32)A_Adr,4,TRUE);
		Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x5D);

}
//--------------------------------------------------------------------
