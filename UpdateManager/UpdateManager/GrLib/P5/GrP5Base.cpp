/*
	Platform 5

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5Base.h>

#include <P5/GrP5Mng.h>
#include <P5/GrP5Render.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5WinBase.h>
#include <P5/GrP5SkinBase.h>

#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <GrDebug.h>

#include <P5/GrP5WinStd.h>
#include <P5/GrP5WinChild.h>
#include <P5/GrP5UoLblTid.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5UoTabStd.h>
#include <P5/GrP5UoTabTiRel.h>
#include <P5/GrP5UoBtnTabTid.h>
#include <P5/GrP5UoPrgsRint.h>
#include <P5/GrP5UoBtnTid.h>
#include <P5/GrP5UoQuadT1.h>
#include <P5/GrP5UoCbxTiSint.h>
#include <P5/GrP5UoSpinNumSint.h>
#include <P5/GrP5UoBtnTime.h>
#include <P5/GrP5UoLblRef.h>
#include <P5/GrP5UoBtnVkey.h>
#include <P5/GrP5UoSpinStxtRint.h>
#include <P5/GrP5UoBtnStxt.h>
#include <P5/GrP5UoCalMark.h>
#include <P5/GrP5UoGrpHzDmap.h>
#include <P5/GrP5UoCbxLtSint.h>
#include <P5/GrP5UoLblLt.h>
#include <P5/GrP5UoBtnImgTid.h>
#include <P5/GrP5UoLblTimeReg.h>
#include <P5/GrP5UoBtnSint.h>
#include <P5/GrP5UoSpinNumLint.h>
#include <P5/GrP5UoChkTiLc.h>
#include <P5/GrP5UoChkTiReg.h>
#include <P5/GrP5UoBtnLtxt.h>
#include <P5/GrP5UoCbxLtLint.h>
#include <P5/GrP5UoSpinLtLint.h>
#include <P5/GrP5UoGridSmd.h>
#include <P5/GrP5UoLblTimeView.h>
#include <P5/GrP5UoPnlStd.h>
#include <P5/GrP5UoGridSrscd.h>
#include <P5/GrP5UoChkImgLtLc.h>
#include <P5/GrP5UoChkImgTiLc.h>
#include <P5/GrP5UoTblStd.h>
#include <P5/GrP5UoPnlAim.h>
#include <P5/GrP5UoSlidHzLint.h>
#include <P5/GrP5UoSlidHzSint.h>
#include <P5/GrP5UoGrpHzT1.h>
#include <P5/GrP5UoArwStd.h>
#include <P5/GrP5UoPnlGrpBar.h>
#include <P5/GrP5UoPnlGrpLine.h>
#include <P5/GrP5UoPnlDir.h>
#include <P5/GrP5UoPnlDrag.h>
#include <P5/GrP5UoGrpHzT2.h>

#include <P5/GrP5Msg.h>
#include <P5/GrP5Script.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <P5/GrP5WinVkbd.h>
#include <P5/GrP5WinPopMenu.h>
#include <P5/GrP5WinPopList.h>

#ifdef GR_P5_EDITOR

#include <P5/Dvl/GrP5DvlBase.h>

#endif

//--------------------------------------------------------------------
// local constant


//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

St_GrP5Env	V_GrP5Env	={NULL, };

__u16	V_GrP5LogTypeToTxtIdTbl[E_GrDvrLogTypeCnt] =
{
	E_GrP5TxtIdLgtMotion, E_GrP5TxtIdLgtSensor, E_GrP5TxtIdLgtVdoloss, E_GrP5TxtIdLgtVdoRcvy, E_GrP5TxtIdLgtPowerOn,	/* 1 */
	E_GrP5TxtIdLgtPowerOff, E_GrP5TxtIdLgtLogin, E_GrP5TxtIdLgtLogout, E_GrP5TxtIdLgtSetupIn, E_GrP5TxtIdLgtSetupOut,	/* 6 */
	E_GrP5TxtIdLgtBkupSt, E_GrP5TxtIdLgtBkupEd, E_GrP5TxtIdLgtHddErr, E_GrP5TxtIdLgtHddFull, E_GrP5TxtIdLgtHddFormat,	/* 11 */
	E_GrP5TxtIdLgtFsFix, E_GrP5TxtIdLgtFwUpgd, E_GrP5TxtIdLgtRtcErr, E_GrP5TxtIdLgtNetLogin, E_GrP5TxtIdLgtNetDiscon,	/* 16 */
	E_GrP5TxtIdLgtNetSetup, E_GrP5TxtIdLgtEmgcSt, E_GrP5TxtIdLgtEmgyEd, E_GrP5TxtIdLgtCfgRecPara, E_GrP5TxtIdLgtCfgMotion,	/* 21 */
	E_GrP5TxtIdLgtCfgIdPass, E_GrP5TxtIdLgtCfgRecSchd, E_GrP5TxtIdLgtCfgHdd, E_GrP5TxtIdLgtCfgChg, E_GrP5TxtIdLgtReboot,			/* 26 */
	E_GrP5TxtIdLgtChgTime, E_GrP5TxtIdCfgInit, E_GrP5TxtIdCfgPtz, E_GrP5TxtIdCfgNet, E_GrP5TxtIdCfgCamSeq, /* 31 */
	E_GrP5TxtIdCfgSpot, E_GrP5TxtIdCfgLoad, E_GrP5TxtIdCfgSave, E_GrP5TxtIdCfgNetFrm, E_GrP5TxtIdCfgDispRes, /* 36 */
	E_GrP5TxtIdMailSendErr, E_GrP5TxtIdWdogVdo, E_GrP5TxtIdWdogAdo, E_GrP5TxtILogSpcSen, E_GrP5TxtILogSoftErr, /* 41 */
	E_GrP5TxtILogFsUpgrade, E_GrP5TxtIdHddTemp /* 47 */
};

//====================================================================
//--------------------------------------------------------------------
Def_GrErrCode	GrP5Init(Cls_GrTaskSvr* A_TaskMng, Cls_GrTaskSvr* A_TaskRender)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrInitFail;

		if(NULL == V_GrP5Env.TskMng)
		{
			GrP5SkinInit();

			GrDumyZeroMem(&V_GrP5Env, sizeof(V_GrP5Env));

			V_GrP5Env.TskMng	=	A_TaskMng;
			V_GrP5Env.ObjMng	=	(Cls_GrTaskCli*)new Cls_GrP5Mng(A_TaskMng);
			if (NULL != V_GrP5Env.ObjMng)
			{
				V_GrP5Env.TskRnd	=	A_TaskRender;
				V_GrP5Env.ObjRnd	=	(Cls_GrTaskCli*) new Cls_GrP5Render(A_TaskRender);
				if(NULL != V_GrP5Env.ObjRnd)
				{
					Tv_Result	=	E_GrErrNone;
				}
				else
				{
					delete	V_GrP5Env.ObjMng;
					V_GrP5Env.ObjMng	=	NULL;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	GrP5Finish(void)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		__u8	Tv_ConIdx;
		__u8	Tv_Layer;
		__u8	Tv_WinIdx;
		Cls_GrP5UoBase*	Tv_UiObj;
	// code --------------------
		// stop rendering
		GrP5RenderRun(FALSE);

		Tv_Result	=	E_GrErrInitFail;

		if(NULL != V_GrP5Env.TskMng)
		{
			// release object direct
			for( Tv_ConIdx = 0; Tv_ConIdx < E_GrP5ConMaxCnt; Tv_ConIdx++)
			{
				if(NULL != V_GrP5Env.ConTbl[Tv_ConIdx].ObjCon)
				{
					for(Tv_Layer = 0; Tv_Layer < E_GrP5LayerMaxCnt; Tv_Layer++)
					{
						for(Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
						{
							if(NULL != V_GrP5Env.ConTbl[Tv_ConIdx].WinTbl[Tv_Layer][Tv_WinIdx].UiObj)
							{
								// release direct
								Tv_UiObj	=	(Cls_GrP5UoBase*)V_GrP5Env.ConTbl[Tv_ConIdx].WinTbl[Tv_Layer][Tv_WinIdx].UiObj;
								delete	Tv_UiObj;
								V_GrP5Env.ConTbl[Tv_ConIdx].WinTbl[Tv_Layer][Tv_WinIdx].UiObj	=	NULL;
							}
						}
					}
				}
			}

			delete	V_GrP5Env.ObjMng;
			V_GrP5Env.ObjMng	=	NULL;

			delete	V_GrP5Env.ObjRnd;
			V_GrP5Env.ObjRnd	=	NULL;

			GrP5SkinFinish();

			Tv_Result	=	E_GrErrNone;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	GrP5TaskRun(void)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;	
	// code --------------------
		Tv_Result	=	E_GrErrInitFail;

		if(NULL != V_GrP5Env.TskMng)
		{
			V_GrP5Env.TskMng->Run();

			Tv_Result	=	E_GrErrNone;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5ConsoleRegist(void* A_ObjCon, __u8 A_ConIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP5MsgConRegist	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.PtrRslt		=	&Tv_Result;
		Tv_Msg.CmdHd.Cmd	=	E_GrP5MsgCmdConRegist;
		Tv_Msg.ConIdx			=	A_ConIdx;
		Tv_Msg.ObjCon			=	A_ObjCon;

		GrP5MsgSend(&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5ConsoleUnregist(__u8 A_ConIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP5MsgConUnregist	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.PtrRslt		=	&Tv_Result;
		Tv_Msg.CmdHd.Cmd	=	E_GrP5MsgCmdConUnregist;
		Tv_Msg.ConIdx			=	A_ConIdx;

		GrP5MsgSend(&Tv_Msg, sizeof(Tv_Msg));

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	GrP5SlaveObjIdEncode(__u8 A_ConIdx, __u16 A_SlaveObjIdx, __u8 A_LocalId)
{
		return	((__u16)A_ConIdx << E_GrP5ObjIdConIdxBitPos) | ((A_SlaveObjIdx + E_GrP5ObjIdSlaveBase) << E_GrP5ObjIdSlaveIdxBitPos) |
			(__u16)A_LocalId;
}
//--------------------------------------------------------------------
BOOL8	GrP5SlaveObjIdDecode(__u16 A_ObjId, __u8* A_PtrRtConIdx, __u16* A_PtrRtSlaveObjIdx, __u8* A_PtrRtLocalId)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(0 == (E_GrP5ObjIdBitWindow & A_ObjId))
		{
			*A_PtrRtSlaveObjIdx	=	((A_ObjId >> E_GrP5ObjIdSlaveIdxBitPos) & E_GrP5ObjIdSlaveIdxBitMask);
			if(0 != (*A_PtrRtSlaveObjIdx))
			{
				*A_PtrRtSlaveObjIdx	=	(*A_PtrRtSlaveObjIdx) - E_GrP5ObjIdSlaveBase;
				if(E_GrP5SlaveObjMaxCnt > (*A_PtrRtSlaveObjIdx))
				{
					*A_PtrRtConIdx		=	(__u8)((A_ObjId >> E_GrP5ObjIdConIdxBitPos) & E_GrP5ObjIdConIdxBitMask);
					*A_PtrRtLocalId		=	(__u8)(A_ObjId & E_GrP5ObjIdLocalIdMask);
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	GrP5WinObjIdEncode(__u8 A_ConIdx, __u8 A_Layer, __u8 A_WinIdx, __u8 A_LocalId)
{
	return	(((__u16)A_ConIdx << E_GrP5ObjIdConIdxBitPos) |	((__u16)A_Layer  << E_GrP5ObjIdWinLayerBitPos) |
		((__u16)A_WinIdx << E_GrP5ObjIdWinIdxBitPos) | (__u16)A_LocalId |
		E_GrP5ObjIdBitWindow);
}
//--------------------------------------------------------------------
BOOL8	GrP5WinObjIdDecode(__u16 A_ObjId, __u8* A_PtrRtConIdx, __u8* A_PtrRtLayer, __u8* A_PtrRtWinIdx, __u8* A_PtrRtLocalId)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (0 != (E_GrP5ObjIdBitWindow & A_ObjId))
		{
			*A_PtrRtWinIdx	=	(__u8)((A_ObjId >> E_GrP5ObjIdWinIdxBitPos) & E_GrP5ObjIdWinIdxBitMask);
			if (E_GrP5WinCntForLayer > (*A_PtrRtWinIdx))
			{
				*A_PtrRtLayer	=	(__u8)((A_ObjId >> E_GrP5ObjIdWinLayerBitPos) & E_GrP5ObjIdWinLayerBitMask);
				if (E_GrP5LayerMaxCnt > (*A_PtrRtLayer))
				{
					*A_PtrRtConIdx	=	(__u8)((A_ObjId >> E_GrP5ObjIdConIdxBitPos) & E_GrP5ObjIdConIdxBitMask);
					if (E_GrP5ConMaxCnt > (*A_PtrRtConIdx))
					{
						*A_PtrRtLocalId		=	(__u8)(A_ObjId & E_GrP5ObjIdLocalIdMask);
						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	GrP5ConIdxGetByObjId(__u16 A_ObjId)
{
		return	(__u8)((A_ObjId >> E_GrP5ObjIdConIdxBitPos) & E_GrP5ObjIdConIdxBitMask);
}
//--------------------------------------------------------------------
__u16	GrP5SlaveObjRegist(__u8 A_ConIdx, void* A_Obj)
{
	// local -------------------
	__u16	Tv_Result;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrP5ObjIdNone;

		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5SlaveObjMaxCnt; Tv_WkIdx++)
		{
			if(NULL == V_GrP5Env.ConTbl[A_ConIdx].SlaveObjTbl[Tv_WkIdx].UiObj)
			{
				// space found
				V_GrP5Env.ConTbl[A_ConIdx].SlaveObjTbl[Tv_WkIdx].UiObj	=	A_Obj;
				Tv_Result	=	GrP5SlaveObjIdEncode(A_ConIdx, Tv_WkIdx, V_GrP5Env.ConTbl[A_ConIdx].SlaveObjTbl[Tv_WkIdx].LocalId);
				V_GrP5Env.ConTbl[A_ConIdx].SlaveObjTbl[Tv_WkIdx].ObjId	=	Tv_Result;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	GrP5WinObjRegist(__u8 A_ConIdx, __u8 A_Layer, void* A_Obj)
{
	// local -------------------
		__u16	Tv_Result;
		__u8	Tv_WinIdx;
	// code --------------------
		Tv_Result	=	E_GrP5ObjIdNone;

		if ((E_GrP5ConMaxCnt > A_ConIdx) && (E_GrP5LayerMaxCnt > A_Layer) )
		{
			// find empty slot
			for (Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
			{
				if (NULL == V_GrP5Env.ConTbl[A_ConIdx].WinTbl[A_Layer][Tv_WinIdx].UiObj)
				{
					// found
					V_GrP5Env.ConTbl[A_ConIdx].WinTbl[A_Layer][Tv_WinIdx].UiObj	=	A_Obj;
					Tv_Result	=	GrP5WinObjIdEncode(A_ConIdx, A_Layer, Tv_WinIdx, V_GrP5Env.ConTbl[A_ConIdx].WinTbl[A_Layer][Tv_WinIdx].LocalId);
					V_GrP5Env.ConTbl[A_ConIdx].WinTbl[A_Layer][Tv_WinIdx].ObjId	=	Tv_Result;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5ObjUnregist(__u16 A_ObjId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ConIdx;
		__u8	Tv_LayIdx;
		__u8	Tv_WinIdx;
		__u8	Tv_LocalId;
		__u16	Tv_SlvIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if (0 != (E_GrP5ObjIdBitWindow & A_ObjId))
		{
			// window
			if (GrP5WinObjIdDecode(A_ObjId, &Tv_ConIdx, &Tv_LayIdx, &Tv_WinIdx, &Tv_LocalId))
			{
				V_GrP5Env.ConTbl[Tv_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj	=	NULL;
				V_GrP5Env.ConTbl[Tv_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].ObjId	=	E_GrP5ObjIdNone;
				V_GrP5Env.ConTbl[Tv_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].LocalId	=
					(V_GrP5Env.ConTbl[Tv_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].LocalId + 1) & E_GrP5ObjIdLocalIdMask;
				Tv_Result	=	TRUE;
			}
		}
		else
		{
			// slave object
			if (GrP5SlaveObjIdDecode(A_ObjId, &Tv_ConIdx, &Tv_SlvIdx,&Tv_LocalId))
			{
				V_GrP5Env.ConTbl[Tv_ConIdx].SlaveObjTbl[Tv_SlvIdx].UiObj	=	NULL;
				V_GrP5Env.ConTbl[Tv_ConIdx].SlaveObjTbl[Tv_SlvIdx].ObjId	=	E_GrP5ObjIdNone;
				V_GrP5Env.ConTbl[Tv_ConIdx].SlaveObjTbl[Tv_SlvIdx].LocalId	=
					(V_GrP5Env.ConTbl[Tv_ConIdx].SlaveObjTbl[Tv_SlvIdx].LocalId + 1) & E_GrP5ObjIdLocalIdMask;
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5RelRectToAbsRect(Ptr_GrP5RelRect A_PtrRelRect, Ptr_GrRect A_PtrRefRect, Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_RefSzX;
		__s32	Tv_RefSzY;
	// code --------------------
		Tv_Result	=	TRUE;

		Tv_RefSzX		=	A_PtrRefRect->right - A_PtrRefRect->left;
		Tv_RefSzY		=	A_PtrRefRect->bottom - A_PtrRefRect->top;

		if ((0 >= Tv_RefSzX) || (0 >= Tv_RefSzY) )
		{
			A_PtrRtRect->left		=	0;
			A_PtrRtRect->top			=	0;
			A_PtrRtRect->right		=	0;
			A_PtrRtRect->bottom	=	0;
			Tv_Result	=	FALSE;
		}
		else
		{
			A_PtrRtRect->left		=	A_PtrRefRect->left + ((Tv_RefSzX * (__s32)A_PtrRelRect->Left.Ratio) >> E_GrP5RelRectRatioBaseShft) 
				+ (__s32)A_PtrRelRect->Left.Val;
			A_PtrRtRect->top			=	A_PtrRefRect->top + ((Tv_RefSzY * (__s32)A_PtrRelRect->Top.Ratio) >> E_GrP5RelRectRatioBaseShft) 
				+ (__s32)A_PtrRelRect->Top.Val;
			A_PtrRtRect->right		=	A_PtrRefRect->left + ((Tv_RefSzX * (__s32)A_PtrRelRect->Right.Ratio) >> E_GrP5RelRectRatioBaseShft) 
				+ (__s32)A_PtrRelRect->Right.Val;
			A_PtrRtRect->bottom	=	A_PtrRefRect->top + ((Tv_RefSzY * (__s32)A_PtrRelRect->Bottom.Ratio) >> E_GrP5RelRectRatioBaseShft) 
				+ (__s32)A_PtrRelRect->Bottom.Val;

			// check value
			if ((A_PtrRtRect->left >= A_PtrRtRect->right) || (A_PtrRtRect->top >= A_PtrRtRect->bottom))
			{
				A_PtrRtRect->left		=	0;
				A_PtrRtRect->top			=	0;
				A_PtrRtRect->right		=	0;
				A_PtrRtRect->bottom	=	0;
				Tv_Result	=	FALSE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5AbsRectToRelRectFixRatio(Ptr_GrRect A_PtrAbsRect, Ptr_GrRect A_PtrRefRect, Ptr_GrP5RelRect A_PtrRtRelRect)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_RefSzX;
		__s32	Tv_RefSzY;
	// code --------------------
		Tv_Result	=	TRUE;

		Tv_RefSzX		=	A_PtrRefRect->right - A_PtrRefRect->left;
		Tv_RefSzY		=	A_PtrRefRect->bottom - A_PtrRefRect->top;

		// left
		A_PtrRtRelRect->Left.Val		=	(__s16)(A_PtrAbsRect->left
			- (A_PtrRefRect->left + ((Tv_RefSzX * (__s32)A_PtrRtRelRect->Left.Ratio) >> E_GrP5RelRectRatioBaseShft)));
		// top
		A_PtrRtRelRect->Top.Val		=	(__s16)(A_PtrAbsRect->top
			- (A_PtrRefRect->top + ((Tv_RefSzY * (__s32)A_PtrRtRelRect->Top.Ratio) >> E_GrP5RelRectRatioBaseShft)));
		// right
		A_PtrRtRelRect->Right.Val	=	(__s16)(A_PtrAbsRect->right
			- (A_PtrRefRect->left + ((Tv_RefSzX * (__s32)A_PtrRtRelRect->Right.Ratio) >> E_GrP5RelRectRatioBaseShft)));
		// bottom
		A_PtrRtRelRect->Bottom.Val	=	(__s16)(A_PtrAbsRect->bottom
			- (A_PtrRefRect->top + ((Tv_RefSzY * (__s32)A_PtrRtRelRect->Bottom.Ratio) >> E_GrP5RelRectRatioBaseShft)));

		// check value
		if ((A_PtrAbsRect->left >= A_PtrAbsRect->right) || (A_PtrAbsRect->top >= A_PtrAbsRect->bottom))
		{
			Tv_Result	=	FALSE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5RelRectToZeroRatio(Ptr_GrP5RelRect A_PtrRelRect, Ptr_GrRect A_PtrRefRect, Ptr_GrP5RelRect A_PtrRtRect)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_RefSzX;
		__s32	Tv_RefSzY;
	// code --------------------
		Tv_Result		=	TRUE;

		Tv_RefSzX		=	A_PtrRefRect->right - A_PtrRefRect->left;
		Tv_RefSzY		=	A_PtrRefRect->bottom - A_PtrRefRect->top;

		A_PtrRtRect->Left.Ratio		=	0;
		A_PtrRtRect->Top.Ratio			=	0;
		A_PtrRtRect->Right.Ratio		=	0;
		A_PtrRtRect->Bottom.Ratio	=	0;

		if((0 >= Tv_RefSzX) || (0 >= Tv_RefSzY))
		{
			A_PtrRtRect->Left.Val			=	0;
			A_PtrRtRect->Top.Val				=	0;
			A_PtrRtRect->Right.Val			=	0;
			A_PtrRtRect->Bottom.Val		=	0;
			Tv_Result	=	FALSE;
		}
		else
		{
			A_PtrRtRect->Left.Val		=	A_PtrRefRect->left + ((Tv_RefSzX * (__s32)A_PtrRelRect->Left.Ratio) >> 7) + (__s32)A_PtrRelRect->Left.Val;
			A_PtrRtRect->Top.Val			=	A_PtrRefRect->top + ((Tv_RefSzY * (__s32)A_PtrRelRect->Top.Ratio) >> 7) + (__s32)A_PtrRelRect->Top.Val;
			A_PtrRtRect->Right	.Val	=	A_PtrRefRect->left + ((Tv_RefSzX * (__s32)A_PtrRelRect->Right.Ratio) >> 7) + (__s32)A_PtrRelRect->Right.Val;
			A_PtrRtRect->Bottom.Val	=	A_PtrRefRect->top + ((Tv_RefSzY * (__s32)A_PtrRelRect->Bottom.Ratio) >> 7) + (__s32)A_PtrRelRect->Bottom.Val;

			// check value
			if((A_PtrRtRect->Left.Val >= A_PtrRtRect->Right.Val) || (A_PtrRtRect->Top.Val >= A_PtrRtRect->Bottom.Val))
			{
				A_PtrRtRect->Left.Val			=	0;
				A_PtrRtRect->Top.Val				=	0;
				A_PtrRtRect->Right.Val			=	0;
				A_PtrRtRect->Bottom.Val		=	0;
				Tv_Result	=	FALSE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5UiObjGet(__u16 A_ObjId)
{
	// local -------------------
		void*	Tv_Result;
		__u8	Tv_ConIdx;
		__u8	Tv_LayIdx;
		__u8	Tv_WinIdx;
		__u8	Tv_LocalId;
		__u16	Tv_SlvIdx;
	// code --------------------
		// init
		Tv_Result	=	NULL;
		// check object type
		if (0 != (E_GrP5ObjIdBitWindow & A_ObjId))
		{
			// window
			if (GrP5WinObjIdDecode(A_ObjId, &Tv_ConIdx, &Tv_LayIdx, &Tv_WinIdx, &Tv_LocalId))
			{
				if (Tv_LocalId == V_GrP5Env.ConTbl[Tv_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].LocalId)
				{
					Tv_Result	=	V_GrP5Env.ConTbl[Tv_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj;
				}
			}
		}
		else
		{
			// slave object
			if (GrP5SlaveObjIdDecode(A_ObjId, &Tv_ConIdx, &Tv_SlvIdx, &Tv_LocalId))
			{
				if (Tv_LocalId == V_GrP5Env.ConTbl[Tv_ConIdx].SlaveObjTbl[Tv_SlvIdx].LocalId)
				{
					Tv_Result	=	V_GrP5Env.ConTbl[Tv_ConIdx].SlaveObjTbl[Tv_SlvIdx].UiObj;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrP5ObjIdFindByScptId(__u8 A_ConIdx, __u16 A_ScptId, __u16* A_PtrRtBuf, __u32 A_MaxCnt)
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_LayIdx;
		__u8	Tv_WinIdx;
		__u16	Tv_SlvIdx;
		Ptr_GrP5ConCtrl	Tv_PtrConCtl;
	// code --------------------
		Tv_Result	=	0;

		Tv_PtrConCtl	=	&V_GrP5Env.ConTbl[A_ConIdx];

		// find window
		for (Tv_LayIdx = 0; Tv_LayIdx < E_GrP5LayerMaxCnt; Tv_LayIdx++)
		{
			for (Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
			{
				// check exist
				if ( NULL != Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj )
				{
					// check script id
					if (A_ScptId == ((Cls_GrP5UoBase*)Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)->ScptIdGet())
					{
						// found
						A_PtrRtBuf[Tv_Result]	=	((Cls_GrP5UoBase*)Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)->ObjIdGet();
						Tv_Result ++;
						if (Tv_Result >= A_MaxCnt)
						{
							break;
						}
					}
				}
			}
		}

		// find slave object
		if (Tv_Result < A_MaxCnt)
		{
			for (Tv_SlvIdx = 0; Tv_SlvIdx < E_GrP5SlaveObjMaxCnt; Tv_SlvIdx++)
			{
				// check exist
				if (NULL != Tv_PtrConCtl->SlaveObjTbl[Tv_SlvIdx].UiObj)
				{
					// check script id
					if (A_ScptId == ((Cls_GrP5UoBase*)Tv_PtrConCtl->SlaveObjTbl[Tv_SlvIdx].UiObj)->ScptIdGet())
					{
						// found
						A_PtrRtBuf[Tv_Result]	=	((Cls_GrP5UoBase*)Tv_PtrConCtl->SlaveObjTbl[Tv_SlvIdx].UiObj)->ObjIdGet();
						Tv_Result ++;
						if (Tv_Result >= A_MaxCnt)
						{
							break;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrP5ObjIdFindByCtrlId(__u8 A_ConIdx, __u16 A_CtrlId, __u16* A_PtrRtBuf, __u32 A_MaxCnt)
{
	// local -------------------
		__u32	Tv_Result;
		__u8	Tv_LayIdx;
		__u8	Tv_WinIdx;
		__u16	Tv_SlvIdx;
		Ptr_GrP5ConCtrl	Tv_PtrConCtl;
	// code --------------------
		Tv_Result	=	0;

		Tv_PtrConCtl	=	&V_GrP5Env.ConTbl[A_ConIdx];

		// find window
		for (Tv_LayIdx = 0; Tv_LayIdx < E_GrP5LayerMaxCnt; Tv_LayIdx++)
		{
			for (Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
			{
				// check exist
				if (NULL != Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)
				{
					// check script id
					if (A_CtrlId == ((Cls_GrP5UoBase*)Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)->CtrlIdGet())
					{
						// found
						A_PtrRtBuf[Tv_Result]	=	((Cls_GrP5UoBase*)Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)->ObjIdGet();
						Tv_Result ++;
						if (Tv_Result >= A_MaxCnt)
						{
							break;
						}
					}
				}
			}
		}

		// find slave object
		if (Tv_Result < A_MaxCnt)
		{
			for (Tv_SlvIdx = 0; Tv_SlvIdx < E_GrP5SlaveObjMaxCnt; Tv_SlvIdx++)
			{
				// check exist
				if (NULL != Tv_PtrConCtl->SlaveObjTbl[Tv_SlvIdx].UiObj)
				{
					// check script id
					if (A_CtrlId == ((Cls_GrP5UoBase*)Tv_PtrConCtl->SlaveObjTbl[Tv_SlvIdx].UiObj)->CtrlIdGet())
					{
						// found
						A_PtrRtBuf[Tv_Result]	=	((Cls_GrP5UoBase*)Tv_PtrConCtl->SlaveObjTbl[Tv_SlvIdx].UiObj)->ObjIdGet();
						Tv_Result ++;
						if (Tv_Result >= A_MaxCnt)
						{
							break;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5UiObjFindByPos(__u8 A_ConIdx, __s32 A_PosX, __s32 A_PosY)
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
			for (Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
			{
				if (NULL != Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)
				{
					// check position
					Tv_Result	=	(void*)((Cls_GrP5UoBase*)Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)->RtlUoFindByPos(A_PosX, A_PosY);
					if (NULL != Tv_Result)
					{
						break;
					}
				}
			}
		} while (0 != Tv_LayIdx);

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5UiObjCanMseFindByPos(__u8 A_ConIdx, __s32 A_PosX, __s32 A_PosY)
{
	// local -------------------
		void*	Tv_Result;
		__u8	Tv_LayIdx;
		__u8	Tv_WinIdx;
		BOOL8	Tv_IsLayLock;
		Ptr_GrP5ConCtrl	Tv_PtrConCtl;
		Cls_GrP5WinBase*	Tv_ObjWin;
	// code --------------------
		Tv_Result	=	NULL;

		Tv_IsLayLock	=	FALSE;
		Tv_PtrConCtl	=	&V_GrP5Env.ConTbl[A_ConIdx];

		// find window
		Tv_LayIdx	=	E_GrP5LayerMaxCnt;
		do
		{
			Tv_LayIdx --;

			// check window
			for (Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
			{
				if (NULL != Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)
				{
					// check visible
					Tv_ObjWin	=	(Cls_GrP5WinBase*)Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj;
					if(Tv_ObjWin->IsVisible())
					{
						// check mouse skip
						if(!Tv_ObjWin->IsMseSkip())
						{
							// check position
							Tv_Result	=	(void*)((Cls_GrP5UoBase*)Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)->RtlUoCanMseFindByPos(A_PosX, A_PosY);
							if(NULL != Tv_Result)
							{
								break;
							}
						}
						// check layer lock
						if(((Cls_GrP5WinBase*)Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)->IsInputLayerLock())
						{
							Tv_IsLayLock	=	TRUE;
						}
					}
				}
			}
			// next layer
			if(Tv_IsLayLock)
			{
				break;
			}
			// found
			if(NULL != Tv_Result)
			{
				break;
			}
		} while (0 != Tv_LayIdx);

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5UiObjFindKeyFocusAble(__u8 A_ConIdx)
{
	// local -------------------
		void*	Tv_Result;
		__u8	Tv_LayIdx;
		__u8	Tv_WinIdx;
		BOOL8	Tv_IsLayLock;
		Ptr_GrP5ConCtrl	Tv_PtrConCtl;
	// code --------------------
		Tv_Result	=	NULL;

		Tv_IsLayLock	=	FALSE;
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
					// get foucus able control
					Tv_Result	=	(void*)((Cls_GrP5WinBase*)Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)->RtlUoFindKeyFocusAble();
					if(NULL != Tv_Result)
					{
						break;
					}
					// check layer lock
					if(((Cls_GrP5WinBase*)Tv_PtrConCtl->WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)->IsInputLayerLock())
					{
						Tv_IsLayLock	=	TRUE;
					}
				}
			}
			// check found
			if(NULL != Tv_Result )
			{
				break;
			}
			// next layer
			if(Tv_IsLayLock)
			{
				break;
			}
		} while(0 != Tv_LayIdx);

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	GrP5ConIdxByObjId(__u16 A_ObjId)
{
		return	(__u8)((A_ObjId >> E_GrP5ObjIdConIdxBitPos) & E_GrP5ObjIdConIdxBitMask);
}
//--------------------------------------------------------------------
void	GrP5KeyFocusSet(__u16 A_ObjId, __u32 A_Para)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoPre;
		Cls_GrP5UoBase*	Tv_UoNow;
		Cls_GrP5WinBase*	Tv_UoWin;
		Cls_GrP5WinBase*	Tv_OldWin;
		//__u8	Tv_ConIdx;
	// code --------------------
		// check equal
		if(V_GrP5Env.KeyObjId != A_ObjId)
		{
			Tv_OldWin	=	NULL;
			// release previous focus object
			Tv_UoPre	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.KeyObjId);
			if(NULL != Tv_UoPre)
			{
				// event focus loss
				Tv_UoPre->EvtKeyFocusLoss();
				Tv_OldWin	=	(Cls_GrP5WinBase*)Tv_UoPre->ParentWinGet();
			}
			// clear key object id
			V_GrP5Env.KeyObjId	=	E_GrP5ObjIdNone;

			// update new
			Tv_UoNow	=	(Cls_GrP5UoBase*)GrP5UiObjGet(A_ObjId);
			if(NULL != Tv_UoNow)
			{
				if(Tv_UoNow->IsCanKeyInput())
				{
					// set new object
					V_GrP5Env.KeyObjId	=	Tv_UoNow->ObjIdGet();
					Tv_UoWin	=	(Cls_GrP5WinBase*)Tv_UoNow->ParentWinGet();

					// release old focus
					if(Tv_OldWin != Tv_UoWin)
					{
						if(NULL != Tv_OldWin)
						{
							Tv_OldWin->EvtWinKeyFocusLoss();
						}
					}
					// new focus set
					if(NULL != Tv_UoWin)
					{
						Tv_UoWin->RtlWinKeyFocusSlaveSet(V_GrP5Env.KeyObjId);
					}
					// get event
					Tv_UoNow->EvtKeyFocusGet(A_Para);
				}
			}
		}
		
}
//--------------------------------------------------------------------
void	GrP5KeyFocusCorrectReq(void)
{
	// local -------------------
		St_GrP5MsgHd	Tv_Msg;
	// code --------------------
		Tv_Msg.Cmd		=	E_GrP5MsgCmdKeyFcsCorrect;
		GrP5MsgPost(&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
void	GrP5KeyFocusCorrectDo(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoFcs;
		__u8	Tv_ConIdx;
		__u8	Tv_ConCnt;
	// code --------------------
		// check now able
		//if (E_GrP5ObjIdNone == V_GrP5Env.KeyObjId)
		{
			Tv_ConCnt	=	1;
			if((E_GrP5ConExpNone != V_GrP5Env.ConExpType) && (NULL != V_GrP5Env.ConTbl[1].ObjCon))
			{
				Tv_ConCnt ++;
			}
			for( Tv_ConIdx = 0; Tv_ConIdx < Tv_ConCnt; Tv_ConIdx++)
			{
				Tv_UoFcs	=	(Cls_GrP5UoBase*)GrP5UiObjFindKeyFocusAble(Tv_ConIdx);
				if(NULL != Tv_UoFcs)
				{
					GrP5KeyFocusSet(Tv_UoFcs->ObjIdGet(),0);
				}
				else
				{
					GrP5KeyFocusSet(E_GrP5ObjIdNone, 0);
				}
			}
		}
}
//--------------------------------------------------------------------
void*	GrP5KeyFocusUiObjGet(void)
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	GrP5UiObjGet(V_GrP5Env.KeyObjId);
		if (NULL == Tv_Result)
		{
			// reset
			V_GrP5Env.KeyObjId	=	E_GrP5ObjIdNone;
			// request focus correct
			GrP5KeyFocusCorrectReq();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5WindowCreateByClassId(__u16 A_ClassId, __u8 A_ConIdx, __u8 A_Layer, Ptr_GrP5RelRect A_PtrRelRect, __u16 A_ScptId)
{
	// local -------------------
		Cls_GrP5WinBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		// check console index and layer range
		if ((E_GrP5ConMaxCnt > A_ConIdx) && (E_GrP5LayerMaxCnt > A_Layer))
		{
			// check exist console
			if (NULL != V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
			{
				// check script range
				if((E_GrP5ScptIdNone == A_ScptId) || 
					((E_GrP5ScptIdWinBase <= A_ScptId) && ((E_GrP5ScptIdWinBase + E_GrP5ScptWinMaxCnt) > A_ScptId)))
				{
					// create object
					switch(A_ClassId)
					{
						case E_GrP5ClassIdWinStd:
							Tv_Result	=	(Cls_GrP5WinBase*) new Cls_GrP5WinStd(A_ScptId, A_ConIdx, A_Layer, A_PtrRelRect);
							break;
						default:
							DbgMsgPrint("GrP5WindowCreate - bad class type (%d) \n", A_ClassId);
							break;
					}

					// check success
					if(NULL != Tv_Result)
					{
						if(E_GrP5ObjIdNone == Tv_Result->ObjIdGet())
						{
							// release
							delete	Tv_Result;
							Tv_Result	=	NULL;
						}

					}
				}
				else
				{
					DbgMsgPrint("GrP5WindowCreateByClassId - bad script (%d) id value \n", A_ScptId);
				}
			}
			else
			{
				DbgMsgPrint("GrP5WindowCreateByClassId - bad console (%d) not exist \n", A_ConIdx);
			}
		}
		else
		{
			DbgMsgPrint("GrP5WindowCreateByClassId - bad console (%d) or layer (%d) index \n", A_ConIdx, A_Layer);
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5WindowCreateByScptId(__u16 A_ScptId, __u8 A_ConIdx, Ptr_GrP5RelRect A_PtrRelRect, __u8 A_Layer)
{
	// local -------------------
		void*	Tv_Result;
		Ptr_GrP5ScptObjHead	Tv_PtrSoHead;
		__u32	Tv_DecSize;
	// code --------------------
		Tv_Result	=	NULL;

		// check script id
		if((E_GrP5ScptIdWinBase <= A_ScptId) && ((E_GrP5ScptIdWinBase + E_GrP5ScptWinMaxCnt) > A_ScptId))
		{
			// get script
			Tv_PtrSoHead	=	(Ptr_GrP5ScptObjHead)GrP5ScriptGet(A_ScptId);
			if(NULL != Tv_PtrSoHead)
			{
				// check script id
				if (A_ScptId != Tv_PtrSoHead->ScptId)
				{
					DbgMsgPrint("GrP5WindowCreateByScptId - [WARNNING] script id (%d : %d) not match\n", A_ScptId,Tv_PtrSoHead->ScptId	);
				}
				// decode
				Tv_Result	=	GrP5ScriptDecWin(Tv_PtrSoHead, A_ConIdx, &Tv_DecSize, A_PtrRelRect,A_Layer );
				// focus currect
				if(NULL != Tv_Result)
				{
					GrP5KeyFocusCorrectReq();
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5ScriptGet(__u16 A_ScptId)
{
		return	((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->ScriptGet(A_ScptId);
}
//--------------------------------------------------------------------
BOOL8	GrP5WindowCloseByObjId(__u16 A_ObjId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Layer;
		__u8	Tv_WinIdx;
		__u8	Tv_LocalId;
		St_GrP5MsgConWinCloseReq	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		// check window
		if(GrP5IsWinObjId(A_ObjId))
		{
			// decode
			if(GrP5WinObjIdDecode(A_ObjId, &Tv_Msg.ConHd.ConIdx, &Tv_Layer, &Tv_WinIdx, &Tv_LocalId))
			{
				// fill message
				Tv_Msg.ConHd.CmdHd.Cmd	=	E_GrP5MsgCmdConWinCloseReq;
				Tv_Msg.ObjId		=	A_ObjId;

				// post message
				Tv_Result	=	GrP5MsgPost(&Tv_Msg, sizeof(Tv_Msg));
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Grp5WindowPopupByScptId(__u16 A_ScptId, __u8 A_ConIdx, __s32 A_PosX, __s32 A_PosY, __u8 A_Layer)
{
	// local -------------------
		void*	Tv_Result;
		St_GrP5RelRect	Tv_RelRc;
		St_GrRect	Tv_RcScr;
		St_GrRect	Tv_RcNow;
		__s32	Tv_MovX;
		__s32	Tv_MovY;
	// code --------------------
		Tv_Result	=	NULL;
		// get relate rect
		if(GrP5ScriptRelRectGet(A_ScptId,&Tv_RelRc))
		{
			// calculate screen rect
			if(E_GrP5ConMaxCnt > A_ConIdx)
			{
				if(NULL != V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
				{
					// get screen rect
					((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon)->ScrRectGet(&Tv_RcScr);
					// calculate relate rect
					if(GrP5RelRectToAbsRect(&Tv_RelRc, &Tv_RcScr, &Tv_RcNow))
					{
						// calculate position
						Tv_MovX	=	A_PosX - Tv_RcNow.left;
						Tv_MovY	=	A_PosY - Tv_RcNow.top;

						// patch move position
						if(0 > A_PosX)
						{
							Tv_MovX	=	Tv_MovX - A_PosX;
						}
						if(0 > A_PosY)
						{
							Tv_MovY	=	Tv_MovY - A_PosY;
						}
						if( (Tv_MovX + Tv_RcNow.right) > Tv_RcScr.right )
						{
							Tv_MovX	=	Tv_MovX - ((Tv_MovX + Tv_RcNow.right) - Tv_RcScr.right);
						}
						if((Tv_MovY + Tv_RcNow.bottom) > Tv_RcScr.bottom)
						{
							Tv_MovY	=	Tv_MovY - ((Tv_MovY + Tv_RcNow.bottom) - Tv_RcScr.bottom);
						}

						// make relate rect
						Tv_RcNow.left		=	Tv_RcNow.left + Tv_MovX;
						Tv_RcNow.top		=	Tv_RcNow.top + Tv_MovY;
						Tv_RcNow.right	=	Tv_RcNow.right + Tv_MovX;
						Tv_RcNow.bottom	=	Tv_RcNow.bottom + Tv_MovY;

						if(GrP5AbsRectToRelRectFixRatio(&Tv_RcNow,&Tv_RcScr,&Tv_RelRc))
						{
							// create window
							Tv_Result	=	GrP5WindowCreateByScptId(A_ScptId, A_ConIdx, &Tv_RelRc, A_Layer);
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5MsgPost(void* A_PtrMsg, __u32 A_MsgSize)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(NULL != V_GrP5Env.ObjMng)
		{
			Tv_Result	=	((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->UiMsgPost((Ptr_GrTaskMsgHd)A_PtrMsg, A_MsgSize);
#ifdef GR_TASKMSG_DEBUG
			if(!Tv_Result)
			{
				DbgMsgPrint("[DBG] %s MsgPost fail!\n", __FUNCTION__);
			}
#endif
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5MsgSend(void* A_PtrMsg, __u32 A_MsgSize)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(NULL != V_GrP5Env.ObjMng)
		{
			Tv_Result	=	((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->UiMsgSend((Ptr_GrTaskMsgHd)A_PtrMsg, A_MsgSize);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5IsWinObjId(__u16 A_ObjId)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(0 != (E_GrP5ObjIdBitWindow & A_ObjId))
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5UiObjCreateByClassId(__u16 A_ClassId, void* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect, __u16 A_ScptId)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;

		switch (A_ClassId)
		{
			case E_GrP5ClassIdLblTid:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoLblTid(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdBtnTabTid:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoBtnTabTid(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdTabStd:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoTabStd(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdTabTiRel:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoTabTiRel(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdPrgsRint:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoPrgsRint(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdBtnTid:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoBtnTid(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdQuadT1:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoQuadT1(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdCbxTiSint:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoCbxTiSint(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdSpinNumSint:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoSpinNumSint(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdBtnTime:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoBtnTime(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdLblRef:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoLblRef(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdBtnVkey:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoBtnVkey(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdSpinStxtRint:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoSpinStxtRint(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdBtnStxt:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoBtnStxt(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdCalMark:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoCalMark(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdGrpHzDmap:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoGrpHzDmap(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case  E_GrP5ClassIdCbxLtSint:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoCbxLtSint(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case  E_GrP5ClassIdLblLt:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoLblLt(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdBtnImgTid:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoBtnImgTid(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdLblTimeReg:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoLblTimeReg(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdBtnSint:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoBtnSint(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdSpinNumLint:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoSpinNumLint(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdChkTiLc:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoChkTiLc(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdChkTiReg:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoChkTiReg(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdBtnLtxt:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoBtnLtxt(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdCbxLtLint:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoCbxLtLint(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdSpinLtLint:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoSpinLtLint(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdGridSmd:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoGridSmd(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdLblTimeView:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoLblTimeView(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdPnlStd:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoPnlStd(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdGridSrscd:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoGridSrscd(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdChkImgLtLc:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoChkImgLtLc(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdChkImgTiLc:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoChkImgTiLc(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case  E_GrP5ClassIdTblStd:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoTblStd(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdPnlAim:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoPnlAim(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdSlidHzLint:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoSlidHzLint(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdSlidHzSint:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoSlidHzSint(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case  E_GrP5ClassIdGrpHzT1:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoGrpHzT1(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case  E_GrP5ClassIdArwStd:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoArwStd(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdPnlDir:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoPnlDir(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case  E_GrP5ClassIdPnlGrpBar:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoPnlGrpBar(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdPnlGrpLine:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoPnlGrpLine(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdPnlDrag:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoPnlDrag(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			case E_GrP5ClassIdGrpHzT2:
				Tv_Result	=	(Cls_GrP5UoBase*)new Cls_GrP5UoGrpHzT2(A_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
				break;
			default:
				DbgMsgPrint("GrP5UiObjCreateByClassId - unknown class type id (%d).",A_ClassId);
				break;
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrP5ScriptDecSub(void* A_PtrScpt, __u32 A_ScptSize, void* A_ObjPrnt, BOOL8 A_IsScptIdNone)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_TtSize;
		__u32	Tv_DecSize;
		__u16	Tv_ScptId;
		Ptr_GrP5ScptObjHead	Tv_PtrSoHd;
		Cls_GrP5UoBase*	Tv_UoThis;
		Ptr_GrP5ScptObjEnd	Tv_PtrSoEd;
		void*	Tv_PtrSlvScpt;
		__u32	Tv_SlvScptSize;
	// code --------------------
		Tv_Result		=	0;

		while(Tv_Result < A_ScptSize)
		{
			Tv_PtrSoHd	=	(Ptr_GrP5ScptObjHead)((Def_GrCalPtr)A_PtrScpt + (Def_GrCalPtr)Tv_Result);
			// check header
			if(E_GrP5ScptObjHdTcc != Tv_PtrSoHd->Tcc)
			{
				// finish
				break;
			}
			// update header info
			Tv_TtSize	=	Tv_PtrSoHd->TotalSize;
			Tv_ScptId	=	Tv_PtrSoHd->ScptId;
			// check size
			if((Tv_TtSize + Tv_Result) > A_ScptSize)
			{
				// finish
				break;
			}
			if(A_IsScptIdNone)
			{
#ifdef GR_P5_EDITOR
				Tv_ScptId	=	E_GrP5DvlScptIdReqAlloc;
#else
				Tv_ScptId	=	E_GrP5ScptIdNone;
#endif
			}
			Tv_UoThis		=	(Cls_GrP5UoBase*)GrP5UiObjCreateByClassId(Tv_PtrSoHd->ClassId, A_ObjPrnt, &Tv_PtrSoHd->RelRect, Tv_ScptId);
			if(NULL == Tv_UoThis)
			{
				DbgMsgPrint("GrP5ScriptDecSub - Ui object create fail at script (%d) offset (%d).\n", Tv_ScptId, Tv_Result);
				break;
			}

			Tv_UoThis->RtlScptParaUpdt(Tv_PtrSoHd);

			// decode slave
			Tv_PtrSlvScpt		=	(void*)((Def_GrCalPtr)A_PtrScpt + (Def_GrCalPtr)Tv_Result + sizeof(St_GrP5ScptObjHead) + (Def_GrCalPtr)Tv_PtrSoHd->ParaSize);
			Tv_SlvScptSize	=	Tv_PtrSoHd->TotalSize - sizeof(St_GrP5ScptObjHead)- (__u32)Tv_PtrSoHd->ParaSize - sizeof(St_GrP5ScptObjEnd);
			if(0 != Tv_SlvScptSize)
			{
				Tv_DecSize	=	GrP5ScriptDecSub(Tv_PtrSlvScpt, Tv_SlvScptSize, (void*)Tv_UoThis, A_IsScptIdNone);
				if(Tv_DecSize != Tv_SlvScptSize)
				{
					DbgMsgPrint("GrP5ScriptDecSub - bad slave script size at script (%d) offset (%d).\n", Tv_ScptId, Tv_Result);
				}
			}

			// check end
			Tv_PtrSoEd	=	(Ptr_GrP5ScptObjEnd)((Def_GrCalPtr)A_PtrScpt + (Def_GrCalPtr)Tv_Result + (Def_GrCalPtr)Tv_PtrSoHd->TotalSize - sizeof(St_GrP5ScptObjEnd));
			if((E_GrP5ScptObjEdTcc != Tv_PtrSoEd->Tcc) || (Tv_PtrSoHd->ScptId != Tv_PtrSoEd->ScptId))
			{
				DbgMsgPrint("GrP5ScriptDecSub - bad slave script end at script (%d) offset (%d).\n", Tv_ScptId, Tv_Result);
				break;
			}

			// next
			Tv_Result	=	Tv_Result + Tv_PtrSoHd->TotalSize;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5ScriptDecWin(void* A_PtrScpt, __u8 A_ConIdx, __u32* A_PtrRtDecoded, Ptr_GrP5RelRect A_PtrRelRect, __u8 A_Layer,
	BOOL8 A_IsScptIdNone)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		__u32	Tv_TtSize;
		__u32	Tv_DecSize;
		Ptr_GrP5ScptObjHead	Tv_PtrSoHd;
		__u16	Tv_ScptId;
		__u8	Tv_Layer;
		Ptr_GrP5RelRect	Tv_PtrRcWin;
		Cls_GrP5UoBase*	Tv_UoThis;
		Ptr_GrP5ScptObjEnd	Tv_PtrSoEd;
		void*	Tv_PtrSlvScpt;
		__u32	Tv_SlvScptSize;
	// code --------------------
		Tv_Result		=	NULL;
		Tv_UoThis		=	NULL;
		Tv_Layer		=	A_Layer;
		Tv_PtrSoHd	=	(Ptr_GrP5ScptObjHead)A_PtrScpt;
		*A_PtrRtDecoded	=	0;
		// check header
		if(E_GrP5ScptObjHdTcc == Tv_PtrSoHd->Tcc)
		{
			// get info
			Tv_TtSize		=	Tv_PtrSoHd->TotalSize;
			Tv_ScptId		=	Tv_PtrSoHd->ScptId;
			if (A_IsScptIdNone)
			{
#ifdef GR_P5_EDITOR
				Tv_ScptId	=	E_GrP5DvlScptIdReqAlloc;
#else
				Tv_ScptId	=	E_GrP5ScptIdNone;
#endif
			}

			if(E_GrP5LayerMaxCnt <= Tv_Layer)
			{
				Tv_Layer	=	(__u8)(Tv_PtrSoHd->HdPara & 0xFF);
			}

			Tv_PtrRcWin	=	A_PtrRelRect;
			if(NULL == Tv_PtrRcWin)
			{
				Tv_PtrRcWin	=	&Tv_PtrSoHd->RelRect;
			}
			// create window
			Tv_UoThis	=	(Cls_GrP5UoBase*)GrP5WindowCreateByClassId(Tv_PtrSoHd->ClassId, A_ConIdx, Tv_Layer, Tv_PtrRcWin, Tv_ScptId);
			if (NULL != Tv_UoThis)
			{
				// init
				Tv_Result	=	Tv_UoThis;
				// decode parametter
				Tv_UoThis->RtlScptParaUpdt(Tv_PtrSoHd);

				Tv_PtrSlvScpt		=	(void*)((Def_GrCalPtr)A_PtrScpt + sizeof(St_GrP5ScptObjHead)+ (Def_GrCalPtr)Tv_PtrSoHd->ParaSize);
				Tv_SlvScptSize	=	Tv_PtrSoHd->TotalSize - sizeof(St_GrP5ScptObjHead)- (__u32)Tv_PtrSoHd->ParaSize - sizeof(St_GrP5ScptObjEnd);
				if(0 != Tv_SlvScptSize)
				{
					Tv_DecSize	=	GrP5ScriptDecSub(Tv_PtrSlvScpt, Tv_SlvScptSize, (void*)Tv_UoThis, A_IsScptIdNone);
					if(Tv_DecSize != Tv_SlvScptSize)
					{
						DbgMsgPrint("GrP5ScriptDecWin - bad sub script size at script (%d).\n", Tv_ScptId);
						Tv_Result	=	NULL;	// fail
					}
				}

				// check end script
				Tv_PtrSoEd	=	(Ptr_GrP5ScptObjEnd)((Def_GrCalPtr)A_PtrScpt + (Def_GrCalPtr)Tv_PtrSoHd->TotalSize - sizeof(St_GrP5ScptObjEnd));
				if((E_GrP5ScptObjEdTcc != Tv_PtrSoEd->Tcc) || (Tv_PtrSoHd->ScptId != Tv_PtrSoEd->ScptId))
				{
					DbgMsgPrint("GrP5ScriptDecWin - bad sub script end at script (%d).\n", Tv_ScptId);
					Tv_Result	=	NULL;	// fail
				}

				// check success
				if (NULL != Tv_Result)
				{
					// create success
					// do create event
					Tv_UoThis->RtlDoEvtScptDecDone();

					// request draw
					Tv_UoThis->ReqDraw(FALSE);
					
					// update decoded size
					*A_PtrRtDecoded	=	Tv_PtrSoHd->TotalSize;
				}
				else
				{
					// create fail
					// request close
					if (!((Cls_GrP5WinBase*)Tv_UoThis)->CloseRequest())
					{
						DbgMsgPrint("GrP5ScriptDecWin - script (%d) can not correct fail resource.\n", Tv_ScptId);
					}
				}
			}		//	if (NULL != Tv_UoThis)

		}
		else
		{
			DbgMsgPrint("GrP5ScriptDecWin - bad script header.\n");
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5ScriptDecChild(void* A_PtrScpt, void* A_ObjPrnt, __u32* A_PtrRtDecoded, Ptr_GrP5RelRect A_PtrRelRect, BOOL8 A_IsFocusChg, BOOL8 A_IsScptIdNone)
{
	// local -------------------
		void*	Tv_Result;
		__u32	Tv_TtSize;
		__u32	Tv_DecSize;
		Ptr_GrP5ScptObjHead	Tv_PtrSoHd;
		__u16	Tv_ScptId;
		Cls_GrP5WinChild*	Tv_UoThis;
		Ptr_GrP5ScptObjEnd	Tv_PtrSoEd;
		void*	Tv_PtrSlvScpt;
		__u32	Tv_SlvScptSize;
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

			// init
			Tv_UoThis	=	(Cls_GrP5WinChild*)new Cls_GrP5WinChild(Tv_ScptId, (Cls_GrP5UoBase*)A_ObjPrnt, A_PtrRelRect);
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
						DbgMsgPrint("GrP5ScriptDecSlave - bad sub script size at script (%d).\n", Tv_ScptId);
						Tv_Result	=	NULL;	// fail
					}
				}

				// check end script
				Tv_PtrSoEd	=	(Ptr_GrP5ScptObjEnd)((Def_GrCalPtr)A_PtrScpt + (Def_GrCalPtr)Tv_PtrSoHd->TotalSize - sizeof(St_GrP5ScptObjEnd));
				if((E_GrP5ScptObjEdTcc != Tv_PtrSoEd->Tcc) || (Tv_PtrSoHd->ScptId != Tv_PtrSoEd->ScptId))
				{
					DbgMsgPrint("GrP5ScriptDecSlave - bad slave script end at script (%d).\n", Tv_ScptId);
					Tv_Result	=	NULL;	// fail
				}
			}

			// check success
			if(NULL != Tv_Result)
			{
				// create success
				Tv_UoThis->RtlDoEvtScptDecDone(A_IsFocusChg);

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
			DbgMsgPrint("GrP5ScriptDecTab - bad script header.\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrGdib	GrP5ImageGet(__u16 A_ImgId)
{
	// local -------------------
	// code --------------------
		return	((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->ImageGet(A_ImgId);
}
//--------------------------------------------------------------------
WCHAR*	GrP5TextGet(__u16 A_TxtId, __u8 A_LanguageId)
{
		return	((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->TextGet(A_TxtId, A_LanguageId);
}
//--------------------------------------------------------------------
void	GrP5LanguageSet(__u8 A_LanguageId)
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------

		if(E_GrP5ScptLanguageMaxCnt > A_LanguageId)
		{
			V_GrP5Env.Language	=	A_LanguageId;

			// redraw screen
			for( Tv_ConIdx = 0; Tv_ConIdx < E_GrP5ConMaxCnt; Tv_ConIdx++)
			{
				GrP5ConRedraw(Tv_ConIdx);
			}
		}
}
//--------------------------------------------------------------------
__u8	GrP5LanguageGet(void)
{
		return	V_GrP5Env.Language;
}
//--------------------------------------------------------------------
void	GrP5InputKey(__u8 A_DevId, __u8 A_Key, BOOL8 A_IsPush)
{
		if(NULL != V_GrP5Env.ObjMng)
		{
			((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->InputKey(A_DevId, A_Key, A_IsPush);
		}
}
//--------------------------------------------------------------------
void	GrP5InputMsePosRel(__s32 A_X, __s32 A_Y)
{
		if(NULL != V_GrP5Env.ObjMng)
		{
			((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->InputMsePosRel(A_X, A_Y);
		}
}
//--------------------------------------------------------------------
void	GrP5InputMsePosAbs(__s32 A_X, __s32 A_Y)
{
		if(NULL != V_GrP5Env.ObjMng)
		{
			((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->InputMsePosAbs(A_X, A_Y);
		}
}
//--------------------------------------------------------------------
void	GrP5InputMseBtn(__u8 A_BtnIdx, BOOL8 A_IsPush)
{
		if(NULL != V_GrP5Env.ObjMng)
		{
			((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->InputMseBtn(A_BtnIdx,A_IsPush);
		}
}
//--------------------------------------------------------------------
void	GrP5InputMseWheel(__u8 A_WheelIdx, BOOL8 A_IsDown)
{
		if(NULL != V_GrP5Env.ObjMng)
		{
			((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->InputMseWheel(A_WheelIdx, A_IsDown);
		}
}
//--------------------------------------------------------------------
void	GrP5ConRedraw(__u8 A_ConIdx)
{
	// local -------------------
	// code --------------------
		if(E_GrP5ConMaxCnt > A_ConIdx)
		{
			if(NULL != V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
			{
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon)->PaintAllReq();
			}
		}
}
//--------------------------------------------------------------------
void	GrP5ConWinDelAll(__u8 A_ConIdx)
{
	// local -------------------
		__u16		Tv_LayIdx;
		__u16		Tv_WinIdx;
	// code --------------------
		if(E_GrP5ConMaxCnt > A_ConIdx)
		{
			if(NULL != V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
			{
				for(Tv_LayIdx = 0; Tv_LayIdx < E_GrP5LayerMaxCnt; Tv_LayIdx++)
				{
					for(Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
					{
						if(NULL != V_GrP5Env.ConTbl[A_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)
						{
							((Cls_GrP5WinBase*)V_GrP5Env.ConTbl[A_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj)->CloseRequest();
						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------
__u32	GrP5ScriptPxFmtGet(void)
{
	// local -------------------
	// code --------------------
		return	((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->PixelFormatGet();
}
//--------------------------------------------------------------------
void*	GrP5ScriptParaGet(void* A_PtrScpt, __u16 A_ClassId, __u16* A_PtrRtSize)
{
	// local -------------------
		void*	Tv_Result;
		Ptr_GrP5ScptObjHead	Tv_PtrSoHd;
		Ptr_GrP5ScptParaHead	Tv_PtrPrHd;
		__u32	Tv_Prgs;
	// code --------------------
		Tv_Result	=	NULL;

		Tv_PtrSoHd	=	(Ptr_GrP5ScptObjHead)A_PtrScpt;
		// check header
		if(E_GrP5ScptObjHdTcc == Tv_PtrSoHd->Tcc)
		{
			Tv_Prgs			=	0;
			Tv_PtrPrHd	=	(Ptr_GrP5ScptParaHead)((Def_GrCalPtr)Tv_PtrSoHd + sizeof(St_GrP5ScptObjHead));
			while(Tv_Prgs < Tv_PtrSoHd->ParaSize)
			{
				// check properly parametter
				if(E_GrP5ScptParaTcc != Tv_PtrPrHd->Tcc)
				{
					DbgMsgPrint("GrP5ScriptParaGet - script has bad parametter TCC\n");
					break;
				}
				// check class
				if(A_ClassId == Tv_PtrPrHd->ClassId)
				{
					// found
					Tv_Result			=	(void*)((Def_GrCalPtr)Tv_PtrPrHd + sizeof(St_GrP5ScptParaHead));
					*A_PtrRtSize	=	Tv_PtrPrHd->Size;
					break;
				}

				// next
				Tv_Prgs			=	Tv_Prgs + sizeof(St_GrP5ScptParaHead)+ (__u32)Tv_PtrPrHd->Size;
				Tv_PtrPrHd	=	(Ptr_GrP5ScptParaHead)((Def_GrCalPtr)Tv_PtrPrHd + sizeof(St_GrP5ScptParaHead) + (Def_GrCalPtr)Tv_PtrPrHd->Size);
			}
		}
		else
		{
			DbgMsgPrint("GrP5ScriptParaGet - bad script TCC\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5ScriptRelRectGet(__u16 A_ScptId, Ptr_GrP5RelRect A_PtrRtRelRect)
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrP5ScptObjHead	Tv_PtrSoHd;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrSoHd	=	(Ptr_GrP5ScptObjHead)GrP5ScriptGet(A_ScptId);
		if(NULL != Tv_PtrSoHd)
		{
			// update result
			*A_PtrRtRelRect	=	Tv_PtrSoHd->RelRect;
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrP5RintBufSet(void* A_PtrBuf, __u32 A_Size)
{
		V_GrP5Env.PtrRintBuf	=	(__s32*)A_PtrBuf;
		V_GrP5Env.RintCnt			=	A_Size / 4;
}
//--------------------------------------------------------------------
__s32	GrP5RintGet(__u16 A_VarId)
{
	// local -------------------
		__s32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ((E_GrP5RegIdBase <= A_VarId) && ((E_GrP5RegIdBase + V_GrP5Env.RintCnt) > A_VarId))
		{
			Tv_Result	=	V_GrP5Env.PtrRintBuf[A_VarId - E_GrP5RegIdBase];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrP5RintSet(__u16 A_VarId, __s32 A_Value)
{
		if ((E_GrP5RegIdBase <= A_VarId) && ((E_GrP5RegIdBase + V_GrP5Env.RintCnt) > A_VarId))
		{
#ifdef GR_P5_EDITOR
			if (V_GrP5Env.PtrRintBuf[A_VarId - E_GrP5RegIdBase] != A_Value)
			{
				V_GrP5Env.PtrRintBuf[A_VarId - E_GrP5RegIdBase]	=	A_Value;
				GrP5DvlDbgRegChgNotify(FALSE, A_VarId);
			}
#else
			V_GrP5Env.PtrRintBuf[A_VarId - E_GrP5RegIdBase]	=	A_Value;
#endif
			
		}
}
//--------------------------------------------------------------------
void	GrP5RtxtBufSet(void* A_PtrBuf, __u32 A_Size, __u32 A_Len)
{
		V_GrP5Env.PtrRtxtBuf		=	A_PtrBuf;
		V_GrP5Env.RtxtLen				=	A_Len;
		V_GrP5Env.RtxtCnt				=	(__u16)(A_Size / (A_Len * 2));
}
//--------------------------------------------------------------------
WCHAR*	GrP5RtxtGet(__u16 A_VarId)
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ((E_GrP5RegIdBase <= A_VarId) && ((E_GrP5RegIdBase + V_GrP5Env.RtxtCnt) > A_VarId))
		{
			Tv_Result	=	(WCHAR*)((Def_GrCalPtr)V_GrP5Env.PtrRtxtBuf + (Def_GrCalPtr)((__u32)(A_VarId - E_GrP5RegIdBase) * (V_GrP5Env.RtxtLen << 1)));
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrP5RtxtSet(__u16 A_VarId, WCHAR* A_Str)
{
	// local -------------------
		WCHAR*	Tv_StrTxt;
	// code --------------------
		Tv_StrTxt	=	GrP5RtxtGet(A_VarId);
		if (NULL != Tv_StrTxt)
		{
#ifdef GR_P5_EDITOR
			if (0 != GrStrWcmp(A_Str, Tv_StrTxt, TRUE))
			{
				GrStrWcopy(Tv_StrTxt, A_Str, V_GrP5Env.RtxtLen - 1);
				GrP5DvlDbgRegChgNotify(TRUE, A_VarId);
			}
#else
			GrStrWcopy(Tv_StrTxt, A_Str,V_GrP5Env.RtxtLen - 1);
#endif
		}
}
//--------------------------------------------------------------------
__u32	GrP5RtxtMaxLen(void)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if(0 != V_GrP5Env.RtxtLen)
		{
			Tv_Result	=	V_GrP5Env.RtxtLen -1;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5CtrlEvtSet(__u16 A_CtrlId, void* A_EvtObj)
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if((E_GrP5CtrlIdBase <= A_CtrlId) && ((E_GrP5CtrlIdBase + E_GrP5CtrlIdMaxCnt) > A_CtrlId))
		{
			Tv_Result	=	V_GrP5Env.CtrlEvtTbl[A_CtrlId - E_GrP5CtrlIdBase];
			V_GrP5Env.CtrlEvtTbl[A_CtrlId - E_GrP5CtrlIdBase]	=	A_EvtObj;
		}
		else
		{
			DbgMsgPrint("GrP5CtrlEvtSet - bad CtrlId (%d)\n",A_CtrlId);
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5CtrlEvtGet(__u16 A_CtrlId)
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if((E_GrP5CtrlIdBase <= A_CtrlId) && ((E_GrP5CtrlIdBase + E_GrP5CtrlIdMaxCnt) > A_CtrlId))
		{
			Tv_Result	=	V_GrP5Env.CtrlEvtTbl[A_CtrlId - E_GrP5CtrlIdBase];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5FontGet(__u8 A_FontId)
{
	// local -------------------
		Cls_GrP5Font*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if(NULL != V_GrP5Env.ObjMng)
		{
			Tv_Result	=	((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->FontGet(A_FontId);
		}

		return	(void*)Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5ExcScriptSet(void* A_PtrScript)
{
	return	((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->ExcScriptSet(A_PtrScript);
}
//--------------------------------------------------------------------
BOOL8	GrP5TimerRegist(void* A_UiObj)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_TmrIdx;
	// code --------------------
		Tv_Result	=	TRUE;

		// check already exist
		if(0 != V_GrP5Env.TimerCnt)
		{
			for(Tv_TmrIdx = 0; Tv_TmrIdx < V_GrP5Env.TimerCnt; Tv_TmrIdx++)
			{
				if(A_UiObj == V_GrP5Env.TimerTbl[Tv_TmrIdx].UiObj)
				{
					// found, already registed
					DbgMsgPrint("GrP5TimerRegist - object existed already.\n");
					Tv_Result	=	FALSE;
					break;
				}
			}
		}

		if(Tv_Result)
		{
			// find empty slot
			Tv_Result	=	FALSE;
			for(Tv_TmrIdx = 0; Tv_TmrIdx < V_GrP5Env.TimerCnt; Tv_TmrIdx++)
			{
				if(NULL == V_GrP5Env.TimerTbl[Tv_TmrIdx].UiObj)
				{
					// found , regist
					V_GrP5Env.TimerTbl[Tv_TmrIdx].UiObj	=	A_UiObj;
					Tv_Result	=	TRUE;
					break;
				}
			}

			if(!Tv_Result)
			{
				if(E_GrP5TimerMaxCnt > V_GrP5Env.TimerCnt)
				{
					V_GrP5Env.TimerTbl[V_GrP5Env.TimerCnt].UiObj	=	A_UiObj;
					V_GrP5Env.TimerCnt ++;
					Tv_Result	=	TRUE;
				}
				else
				{
					DbgMsgPrint("GrP5TimerRegist - timer slot is full.\n");
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5TimerUnregist(void* A_UiObj)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsCollect;
		__u32	Tv_TmrIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_IsCollect	=	FALSE;

		if(0 != V_GrP5Env.TimerCnt)
		{
			for(Tv_TmrIdx = 0; Tv_TmrIdx < V_GrP5Env.TimerCnt; Tv_TmrIdx++)
			{
				if(A_UiObj == V_GrP5Env.TimerTbl[Tv_TmrIdx].UiObj)
				{
					// found 
					V_GrP5Env.TimerTbl[Tv_TmrIdx].UiObj	=	NULL;
					Tv_Result	=	TRUE;
					if((Tv_TmrIdx + 1) == V_GrP5Env.TimerCnt)
					{
						Tv_IsCollect	=	TRUE;
					}
					break;
				}
			}

			// timer count collect
			if(Tv_IsCollect)
			{
				Tv_TmrIdx	=	V_GrP5Env.TimerCnt;
				while(0 < Tv_TmrIdx)
				{
					Tv_TmrIdx --;

					if(NULL != V_GrP5Env.TimerTbl[Tv_TmrIdx].UiObj)
					{
						break;
					}
					// update count
					V_GrP5Env.TimerCnt	=	Tv_TmrIdx;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrP5MseCusPosGet(__s32* A_PtrRtX, __s32* A_PtrRtY)
{
		*A_PtrRtX	=	V_GrP5Env.MseX;
		*A_PtrRtY	=	V_GrP5Env.MseY;
}
//--------------------------------------------------------------------
void	GrP5QuadModePageChange(__u8 A_ConIdx, __u8 A_Mode, __u8 A_Page)
{
	// local -------------------
	// code --------------------
		if(E_GrP5ConMaxCnt > A_ConIdx)
		{
			if(NULL != V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
			{
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon)->QuadModeSet(A_Mode, A_Page);
			}
		}
}
//--------------------------------------------------------------------
void	GrP5QuadModeNext(__u8 A_ConIdx)
{
	// local -------------------
	// code --------------------
		if(E_GrP5ConMaxCnt > A_ConIdx)
		{
			if(NULL != V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
			{
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon)->QuadModeNext();
			}
		}

}
//--------------------------------------------------------------------
void	GrP5QuadModePrev(__u8 A_ConIdx)
{
	// local -------------------
	// code --------------------
		if(E_GrP5ConMaxCnt > A_ConIdx)
		{
			if(NULL != V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
			{
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon)->QuadModePrev();
			}
		}
}
//--------------------------------------------------------------------
void	GrP5QuadPageNext(__u8 A_ConIdx)
{
	// local -------------------
	// code --------------------
		if(E_GrP5ConMaxCnt > A_ConIdx)
		{
			if(NULL != V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
			{
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon)->QuadPageNext();
			}
		}

}
//--------------------------------------------------------------------
void	GrP5QuadPagePrev(__u8 A_ConIdx)
{
	// local -------------------
	// code --------------------
		if(E_GrP5ConMaxCnt > A_ConIdx)
		{
			if(NULL != V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
			{
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon)->QuadPagePrev();
			}
		}

}
//--------------------------------------------------------------------
void	GrP5QuadModeExchg(__u8 A_ConIdx, __u8 A_Ch)
{
	// local -------------------
	// code --------------------
		if(E_GrP5ConMaxCnt > A_ConIdx)
		{
			if(NULL != V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
			{
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon)->QuadModeExchange(A_Ch	);
			}
		}

}
//--------------------------------------------------------------------
void	GrP5QuadModeGet(__u8 A_ConIdx, __u8* A_PtrRtMode, __u8* A_PtrRtPage)
{
		if(E_GrP5ConMaxCnt > A_ConIdx)
		{
			if(NULL != V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
			{
				((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon)->QuadModeGet(A_PtrRtMode, A_PtrRtPage);
			}
		}
}
//--------------------------------------------------------------------
void*	GrP5SetupPtrGet(void)
{
		return	(void*)V_GrP5Env.SetupBuf;
}
//--------------------------------------------------------------------
void*	GrP5SetupOfsPtrGet(__u16 A_Adr)
{
		return	(void*)((Def_GrCalPtr)V_GrP5Env.SetupBuf + (Def_GrCalPtr)A_Adr);
}
//--------------------------------------------------------------------
__u8	GrP5SetupGetU8(__u16 A_Adr)
{
	// local -------------------
		__u8*	Tv_Ptr;
	// code --------------------
		Tv_Ptr	=	(__u8*)((Def_GrCalPtr)V_GrP5Env.SetupBuf + (Def_GrCalPtr)A_Adr);
		return	*Tv_Ptr;
}
//--------------------------------------------------------------------
__u16	GrP5SetupGetU16(__u16 A_Adr)
{
	// local -------------------
		__u16*	Tv_Ptr;
	// code --------------------
		Tv_Ptr	=	(__u16*)((Def_GrCalPtr)V_GrP5Env.SetupBuf + (Def_GrCalPtr)A_Adr);
		return	*Tv_Ptr;
}
//--------------------------------------------------------------------
__u32	GrP5SetupGetU32(__u16 A_Adr)
{
	// local -------------------
		__u32*	Tv_Ptr;
	// code --------------------
		Tv_Ptr	=	(__u32*)((Def_GrCalPtr)V_GrP5Env.SetupBuf + (Def_GrCalPtr)A_Adr);
		return	*Tv_Ptr;
}
//--------------------------------------------------------------------
void	GrP5SetupPutU8(__u16 A_Adr, __u8 A_Val)
{
	// local -------------------
		__u8*	Tv_Ptr;
	// code --------------------
		Tv_Ptr	=	(__u8*)((Def_GrCalPtr)V_GrP5Env.SetupBuf + (Def_GrCalPtr)A_Adr);
		*Tv_Ptr	=	A_Val;
}
//--------------------------------------------------------------------
void	GrP5SetupPutU16(__u16 A_Adr, __u16 A_Val)
{
	// local -------------------
		__u16*	Tv_Ptr;
	// code --------------------
		Tv_Ptr	=	(__u16*)((Def_GrCalPtr)V_GrP5Env.SetupBuf + (Def_GrCalPtr)A_Adr);
		*Tv_Ptr	=	A_Val;
}
//--------------------------------------------------------------------
void	GrP5SetupPutU32(__u16 A_Adr, __u32 A_Val)
{
	// local -------------------
		__u32*	Tv_Ptr;
	// code --------------------
		Tv_Ptr	=	(__u32*)((Def_GrCalPtr)V_GrP5Env.SetupBuf + (Def_GrCalPtr)A_Adr);
		*Tv_Ptr	=	A_Val;
}
//--------------------------------------------------------------------
void	GrP5KeyFocusReq(__u16 A_ObjId, __u32 A_Para)
{
	// local -------------------
		St_GrP5MsgKeyFcsUoSet	Tv_Msg;
	// code --------------------
		// post message
		Tv_Msg.CmdHd.Cmd	=	E_GrP5MsgCmdKeyFcsUoSet;
		Tv_Msg.ObjId			=	A_ObjId;
		Tv_Msg.Para				=	A_Para;

#ifdef GR_TASKMSG_DEBUG
		if(!GrP5MsgPost(&Tv_Msg, sizeof(Tv_Msg)))
		{
			DbgMsgPrint("[DBG] %s MsgPost fail!\n", __FUNCTION__);
		}
#else
		GrP5MsgPost(&Tv_Msg, sizeof(Tv_Msg));
#endif

}
//--------------------------------------------------------------------
void*	GrP5VkbdCreateByScptId(__u16 A_ScptId, __u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect,
	WCHAR* A_StrTxt, __u32 A_MaxLen, BOOL8 A_IsPassword, __u16 A_HostObjId, __u16 A_EvtCtlId, __u32 A_UserData)
{
	// local -------------------
		void*	Tv_Result;
		Cls_GrP5WinVkbd*	Tv_UoThis;
		Ptr_GrP5ScptObjHead	Tv_PtrSoHd;
		__u8	Tv_Layer;
		__u16	Tv_ScptId;
		Ptr_GrP5ScptObjEnd	Tv_PtrSoEd;
		__u32	Tv_TtSize;
		Ptr_GrP5RelRect	Tv_PtrRcWin;
		void*	Tv_PtrSlvScpt;
		__u32	Tv_SlvScptSize;
		__u32	Tv_DecSize;
	// code --------------------

		Tv_Result	=	NULL;
		Tv_UoThis		=	NULL;
		Tv_Layer		=	A_LayerIdx;

		// check script id
		if((E_GrP5ScptIdWinBase <= A_ScptId) && ((E_GrP5ScptIdWinBase + E_GrP5ScptWinMaxCnt) > A_ScptId))
		{
			// get script
			Tv_PtrSoHd	=	(Ptr_GrP5ScptObjHead)GrP5ScriptGet(A_ScptId);
			if(NULL != Tv_PtrSoHd)
			{
				// check script id
				if(A_ScptId != Tv_PtrSoHd->ScptId)
				{
					DbgMsgPrint("GrP5VkbdCreateByScptId - [WARNNING] script id (%d : %d) not match\n", A_ScptId, Tv_PtrSoHd->ScptId);
				}
				// decode
				// check header
				if(E_GrP5ScptObjHdTcc == Tv_PtrSoHd->Tcc)
				{
					// get info
					Tv_TtSize		=	Tv_PtrSoHd->TotalSize;
					Tv_ScptId		=	Tv_PtrSoHd->ScptId;
					if(E_GrP5LayerMaxCnt <= Tv_Layer)
					{
						Tv_Layer	=	(__u8)(Tv_PtrSoHd->HdPara & 0xFF);
					}

					Tv_PtrRcWin	=	A_PtrRelRect;
					if(NULL == Tv_PtrRcWin)
					{
						Tv_PtrRcWin	=	&Tv_PtrSoHd->RelRect;
					}
					// create window
					Tv_UoThis	=	(Cls_GrP5WinVkbd*)new Cls_GrP5WinVkbd(Tv_ScptId, A_ConIdx, Tv_Layer, Tv_PtrRcWin, A_StrTxt,A_MaxLen,
						A_IsPassword,A_HostObjId,A_EvtCtlId,A_UserData);
					if(NULL != Tv_UoThis)
					{
						// init
						Tv_Result	=	Tv_UoThis;
						// decode parametter
						Tv_UoThis->RtlScptParaUpdt(Tv_PtrSoHd);

						Tv_PtrSlvScpt		=	(void*)((Def_GrCalPtr)Tv_PtrSoHd + sizeof(St_GrP5ScptObjHead)+ (Def_GrCalPtr)Tv_PtrSoHd->ParaSize);
						Tv_SlvScptSize	=	Tv_PtrSoHd->TotalSize - sizeof(St_GrP5ScptObjHead)- (__u32)Tv_PtrSoHd->ParaSize - sizeof(St_GrP5ScptObjEnd);
						if(0 != Tv_SlvScptSize)
						{
							Tv_DecSize	=	GrP5ScriptDecSub(Tv_PtrSlvScpt, Tv_SlvScptSize, (void*)Tv_UoThis);
							if(Tv_DecSize != Tv_SlvScptSize)
							{
								DbgMsgPrint("GrP5VkbdCreateByScptId - bad sub script size at script (%d).\n", Tv_ScptId);
								Tv_Result	=	NULL;	// fail
							}
						}

						// check end script
						Tv_PtrSoEd	=	(Ptr_GrP5ScptObjEnd)((Def_GrCalPtr)Tv_PtrSoHd + (Def_GrCalPtr)Tv_PtrSoHd->TotalSize - sizeof(St_GrP5ScptObjEnd));
						if((E_GrP5ScptObjEdTcc != Tv_PtrSoEd->Tcc) || (Tv_PtrSoHd->ScptId != Tv_PtrSoEd->ScptId))
						{
							DbgMsgPrint("GrP5VkbdCreateByScptId - bad sub script end at script (%d).\n", Tv_ScptId);
							Tv_Result	=	NULL;	// fail
						}

						// check success
						if(NULL != Tv_Result)
						{
							// create success
							// do create event
							Tv_UoThis->RtlDoEvtScptDecDone();
							
							V_GrP5Env.MseObjId	=	E_GrP5ObjIdNone;

							// request draw
							Tv_UoThis->ReqDraw(FALSE);
						}
						else
						{
							// create fail
							// request close
							if(!((Cls_GrP5WinBase*)Tv_UoThis)->CloseRequest())
							{
								DbgMsgPrint("GrP5VkbdCreateByScptId - script (%d) can not correct fail resource.\n", Tv_ScptId);
							}
						}
					}		//	if (NULL != Tv_UoThis)

				}
				else
				{
					DbgMsgPrint("GrP5VkbdCreateByScptId - bad script header.\n");
				}


			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5VkbdCreate(__u32 A_Type, void* A_HostObj, __u16 A_CtlEvtId, WCHAR* A_StrTxt, __u32 A_MaxLen, BOOL8 A_IsPassword, __u32 A_UserData)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_ScptId;
		__u8	Tv_LayIdx;
		Cls_GrP5WinBase*	Tv_Win;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_ScptId	=	((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->VkbdGetScptId(A_Type);
		if(E_GrP5ScptIdNone != Tv_ScptId)
		{
			//
			Tv_Win	=	(Cls_GrP5WinBase*)((Cls_GrP5UoBase*)A_HostObj)->ParentWinGet();
			Tv_LayIdx	=	Tv_Win->LayerGet();
			if((Tv_LayIdx + 1) < E_GrP5LayerMaxCnt)
			{
				Tv_LayIdx	++;
			}
			
			if(NULL != GrP5VkbdCreateByScptId(Tv_ScptId, ((Cls_GrP5UoBase*)A_HostObj)->ConsoleIdxGet(), Tv_LayIdx, NULL,
				A_StrTxt, A_MaxLen, A_IsPassword, ((Cls_GrP5UoBase*)A_HostObj)->ObjIdGet(),A_CtlEvtId,A_UserData))
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5WindowFindByScptId(__u8 A_ConIdx, __u16 A_ScptId)
{
	// local -------------------
		void*	Tv_Result;
		__u8	Tv_LayIdx;
		__u8	Tv_WinIdx;
		Cls_GrP5UoBase*	Tv_UiObj;
	// code --------------------
		Tv_Result	=	NULL;

		// check console
		if(E_GrP5ConMaxCnt > A_ConIdx)
		{
			// find window table
			for( Tv_LayIdx = 0; Tv_LayIdx < E_GrP5LayerMaxCnt; Tv_LayIdx++)
			{
				for( Tv_WinIdx = 0; Tv_WinIdx < E_GrP5WinCntForLayer; Tv_WinIdx++)
				{
					Tv_UiObj	=	(Cls_GrP5UoBase*)V_GrP5Env.ConTbl[A_ConIdx].WinTbl[Tv_LayIdx][Tv_WinIdx].UiObj;
					if(NULL != Tv_UiObj)
					{
						if(A_ScptId == Tv_UiObj->ScptIdGet())
						{
							// found
							Tv_Result	=	(void*)Tv_UiObj;
							break;
						}
					}
				}
				if(NULL != Tv_Result)
				{
					break;
				}
			}
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
void*	GrP5UiObjGetByUserId(__u32 A_UserId)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		__u32	Tv_IdIdx;
		__u16	Tv_ObjId;
	// code --------------------
		Tv_Result	=	NULL;

		// check range
		if((0 != A_UserId) && ((E_GrP5UserIdMaxCnt + E_GrP5userIdBase) > A_UserId))
		{
			// get object id
			Tv_IdIdx	=	A_UserId - E_GrP5userIdBase;
			Tv_ObjId	=	V_GrP5Env.UserIdCvtTbl[Tv_IdIdx];
			Tv_Result	=	(Cls_GrP5UoBase*)GrP5UiObjGet(Tv_ObjId);
			if(NULL != Tv_Result)
			{
				// check valid user id
				if(A_UserId != Tv_Result->UserIdGet())
				{
					Tv_Result	=	NULL;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5UserIdRegist(__u32 A_UserId, __u16 A_ObjId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_IdIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		// check range
		if((0 != A_UserId) && ((E_GrP5UserIdMaxCnt + E_GrP5userIdBase) > A_UserId))
		{
			Tv_IdIdx	=	A_UserId - E_GrP5userIdBase;
			V_GrP5Env.UserIdCvtTbl[Tv_IdIdx]	=	A_ObjId;
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrP5UiObjFindKeyFocusByDir(__u16 A_BaseObjId, __u8 A_Dir)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		__u16	Tv_ScptId;
		__u8	Tv_Dir;
		Cls_GrP5UoBase*		Tv_UoBase;
		Cls_GrP5WinBase*	Tv_Win;
		St_GrRect	Tv_RcBase;
	// code --------------------
		Tv_Result	=	NULL;
		// get window
		Tv_UoBase	=	(Cls_GrP5UoBase*)GrP5UiObjGet(A_BaseObjId);
		if(NULL != Tv_UoBase)
		{
			// get window
			Tv_Win	=	(Cls_GrP5WinBase*)Tv_UoBase->ParentWinGet();
			if(NULL != Tv_Win)
			{
				// check static focus
				Tv_ScptId	=	Tv_UoBase->KeyFocusGet(A_Dir);
				if(E_GrP5ScptIdNone != Tv_ScptId)
				{
					Tv_Result	=	Tv_Win->RtlUoFindByScptId(Tv_ScptId);
					if(NULL != Tv_Result)
					{
						if(!Tv_Result->IsCanKeyInput())
						{
							Tv_Result	=	NULL;
						}
					}
				}
				
				if(NULL == Tv_Result)
				{
					// get rect
					if(Tv_UoBase->ScrRectGet(&Tv_RcBase))
					{
						Tv_Result	=	Tv_Win->FindSlaveNearByDir(&Tv_RcBase, A_Dir, TRUE, A_BaseObjId);
						if(NULL == Tv_Result)
						{
							Tv_Dir		=	A_Dir;
							switch(A_Dir)
							{
								case E_GrP5DirLeft:
									Tv_Dir	=	E_GrP5DirRight;
									break;
								case E_GrP5DirUp:
									Tv_Dir	=	E_GrP5DirDown;
									break;
								case E_GrP5DirRight:
									Tv_Dir	=	E_GrP5DirLeft;
									break;
								case E_GrP5DirDown:
									Tv_Dir	=	E_GrP5DirUp;
									break;
							}
							Tv_Result	=	Tv_Win->FindSlaveFarByDir(&Tv_RcBase, Tv_Dir,TRUE, A_BaseObjId);
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5IsMseCursorOn(void)
{
		return	V_GrP5Env.IsMseCusOn;
}
//--------------------------------------------------------------------
__u16	GrP5LogTypeToTextId(Ptr_GrDvrLogItm A_PtrLog)
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrP5TxtIdNone;

		if((E_GrDvrLogTypeNone != A_PtrLog->LogType) && (E_GrDvrLogTypeCnt >= A_PtrLog->LogType))
		{
			Tv_Result	=	V_GrP5LogTypeToTxtIdTbl[A_PtrLog->LogType - 1];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrP5LogContToWstr(Ptr_GrDvrLogItm A_PtrLog, WCHAR* A_StrRt)
{
	// local -------------------
		WCHAR*	Tv_StrWk;
		WCHAR*	Tv_StrTg;
		__u16		Tv_TxtId;
	// code --------------------
		// init
		GrStrClear(A_StrRt);
		// get title
		Tv_StrTg	=	A_StrRt;

		Tv_TxtId	=	GrP5LogTypeToTextId(A_PtrLog);
		if(E_GrP5TxtIdNone == Tv_TxtId)
		{
			return;
		}
		Tv_StrWk	=	GrP5TextGet(Tv_TxtId);
		if(NULL == Tv_StrWk)
		{
			return;
		}
		// copy title
		Tv_StrTg	=	GrStrWcopy(Tv_StrTg, Tv_StrWk);

		// copy parametter
		switch(A_PtrLog->LogType)
		{
			case E_GrDvrLogTypeMotion:
				// add channel
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, 0x20);
				Tv_StrTg	=	GrStrWcopy(Tv_StrTg, GrP5TextGet(E_GrP5TxtIdChannel));
				GrStrIntToWstr(Tv_StrTg, A_PtrLog->Ch + 1);
				break;
			case E_GrDvrLogTypeSensor:
				// add channel
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, 0x20);
				Tv_StrTg	=	GrStrWcopy(Tv_StrTg, GrP5TextGet(E_GrP5TxtIdChannel));
				GrStrIntToWstr(Tv_StrTg, A_PtrLog->Ch + 1);
				break;
			case E_GrDvrLogTypeVdoLoss:
				// add channel
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, 0x20);
				Tv_StrTg	=	GrStrWcopy(Tv_StrTg, GrP5TextGet(E_GrP5TxtIdChannel));
				GrStrIntToWstr(Tv_StrTg, A_PtrLog->Ch + 1);
				break;
			case E_GrDvrLogTypeVdoRcvy:
				// add channel
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, 0x20);
				Tv_StrTg	=	GrStrWcopy(Tv_StrTg, GrP5TextGet(E_GrP5TxtIdChannel));
				GrStrIntToWstr(Tv_StrTg, A_PtrLog->Ch + 1);
				break;
			case E_GrDvrLogTypeNetLogin:
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, 0x20);
				GrStrIpToWstr(Tv_StrTg, A_PtrLog->ParaDw0);
				break;
			case E_GrDvrLogTypeNetDiscon:
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, 0x20);
				GrStrIpToWstr(Tv_StrTg, A_PtrLog->ParaDw0);
				break;
			case E_GrDvrLogTypeSpcSen:
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, 0x20);
				Tv_StrTg	=	GrStrWcopy(Tv_StrTg, GrP5TextGet(E_GrP5TxtIdChannel));
				GrStrIntToWstr(Tv_StrTg, A_PtrLog->Ch + 1);
				break;
			case E_GrDvrLogTypeHddErr:
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, 0x20);
				GrStrIntToWstr(Tv_StrTg, A_PtrLog->ParaBt0 + 1);
				break;
			case E_GrDvrLogTypeSoftErr:
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, 0x20);
				GrStrIntToWstr(Tv_StrTg, A_PtrLog->ParaDw0 + 1);
				break;
			case  E_GrDvrLogTypeFsFix:
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, 0x20);
				GrStrIntToWstr(Tv_StrTg, A_PtrLog->ParaBt0);
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, ':');
				GrStrIntToWstr(Tv_StrTg, A_PtrLog->ParaWd0);
				break;
			case E_GrDvrLogTypeFsUpgrade:
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, 0x20);
				GrStrIntToWstr(Tv_StrTg, A_PtrLog->ParaDw1);
				Tv_StrTg	=	GrStrAddWchar(Tv_StrTg, '>');
				GrStrIntToWstr(Tv_StrTg, A_PtrLog->ParaDw0);
				break;
			case E_GrDvrLogTypeHddTemp:

				break;
		}
}
//--------------------------------------------------------------------
void	GrP5AutoLogoutSet(__u32 A_ExpireSec)
{
	// local -------------------
	// code --------------------
		((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->AutoLogoutReq(A_ExpireSec);
}
//--------------------------------------------------------------------
void*	GrP5QuadUiObjGet(__u8 A_ConIdx)
{
		return	V_GrP5Env.ConTbl[A_ConIdx].UiQuad;
}
//--------------------------------------------------------------------
WCHAR*	GrP5TxtBufAlloc(__u16 A_ObjId)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		for( Tv_WkIdx = 0; Tv_WkIdx < E_GrP5TxtBufMaxCnt; Tv_WkIdx++)
		{
			if(E_GrP5ObjIdNone == V_GrP5Env.TxtBuf[Tv_WkIdx].ObjId)
			{
				// found
				V_GrP5Env.TxtBuf[Tv_WkIdx].ObjId	=	A_ObjId;
				return	V_GrP5Env.TxtBuf[Tv_WkIdx].Str;
			}
		}

#ifdef E_GrP5DbgMsgWarnning
		DbgMsgPrint("GrP5TxtBufAlloc - text buffer is full!\n");
#endif

		return	NULL;
}
//--------------------------------------------------------------------
void	GrP5TxtBufFreeByObjId(__u16 A_ObjId)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5TxtBufMaxCnt; Tv_WkIdx++)
		{
			if(A_ObjId == V_GrP5Env.TxtBuf[Tv_WkIdx].ObjId)
			{
				// found
				V_GrP5Env.TxtBuf[Tv_WkIdx].ObjId	=	E_GrP5ObjIdNone;
			}
		}
}
//--------------------------------------------------------------------
void	GrP5RenderRun(BOOL8 A_IsRun)
{
		((Cls_GrP5Render*)V_GrP5Env.ObjRnd)->ReqRenderRunSet(A_IsRun);
}
//--------------------------------------------------------------------
void	GrP5PopupCloseAll(void)
{
	// local -------------------
		__u8	Tv_WinIdx;
	// code --------------------
		// check exist popup
		if(0 == V_GrP5Env.PopCtl.WinCnt)
		{
			return;
		}

		// request close
		for(Tv_WinIdx = 0; Tv_WinIdx < V_GrP5Env.PopCtl.WinCnt; Tv_WinIdx++)
		{
			GrP5WindowCloseByObjId(V_GrP5Env.PopCtl.WinCtl[Tv_WinIdx].WinObjId);
			V_GrP5Env.PopCtl.WinCtl[Tv_WinIdx].WinObjId	=	E_GrP5ObjIdNone;
		}

		V_GrP5Env.PopCtl.WinCnt	=	0;
}
//--------------------------------------------------------------------
void	GrP5PopupCloseByDepth(__u8 A_Depth)
{
	// local -------------------
		__u8	Tv_WinIdx;
	// code --------------------
		if(A_Depth >= V_GrP5Env.PopCtl.WinCnt)
		{
			return;
		}

		Tv_WinIdx	=	A_Depth;
		while(Tv_WinIdx < V_GrP5Env.PopCtl.WinCnt)
		{
			GrP5WindowCloseByObjId(V_GrP5Env.PopCtl.WinCtl[Tv_WinIdx].WinObjId);
			V_GrP5Env.PopCtl.WinCtl[Tv_WinIdx].WinObjId	=	E_GrP5ObjIdNone;
			// next
			Tv_WinIdx ++;
		}

		V_GrP5Env.PopCtl.WinCnt	=	A_Depth;
}
//--------------------------------------------------------------------
void	GrP5ItemReset(void)
{
	// local -------------------
	// code --------------------
		V_GrP5Env.ItemCnt		=	0;
}
//--------------------------------------------------------------------
__s32	GrP5ItemAdd(__u16 A_TxtId, __u32 A_Stat, __u16	A_IconId, __u16	A_Squad, __u16 A_ChildSquad,
	__u32 A_UserData, WCHAR* A_Str)
{
	// local -------------------
	// code --------------------
		// check full
		if(E_GrP5ItemMaxCnt <= V_GrP5Env.ItemCnt)
		{
			return	-1;
		}

		// check squad
		if(E_GrP5ItemSquadIdNone == A_Squad)
		{
			return	-1;
		}

		V_GrP5Env.ItemTbl[V_GrP5Env.ItemCnt].Stat				=	A_Stat;
		V_GrP5Env.ItemTbl[V_GrP5Env.ItemCnt].TxtId			=	A_TxtId;
		V_GrP5Env.ItemTbl[V_GrP5Env.ItemCnt].IconId			=	A_IconId;
		V_GrP5Env.ItemTbl[V_GrP5Env.ItemCnt].Squad			=	A_Squad;
		V_GrP5Env.ItemTbl[V_GrP5Env.ItemCnt].ChildSquad	=	A_ChildSquad;
		V_GrP5Env.ItemTbl[V_GrP5Env.ItemCnt].UserData		=	A_UserData;

		GrStrClear(V_GrP5Env.ItemTbl[V_GrP5Env.ItemCnt].StrBuf);
		if(NULL != A_Str)
		{
			GrStrWcopy(V_GrP5Env.ItemTbl[V_GrP5Env.ItemCnt].StrBuf, A_Str, E_GrP5ItemStrBufLen);
		}

		V_GrP5Env.ItemCnt ++;

		return	(__s32)(V_GrP5Env.ItemCnt - 1);
}
//--------------------------------------------------------------------
__s32	GrP5ItemIdGetByUserData(__u32 A_UserData)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		// check item count
		if(0 == V_GrP5Env.ItemCnt)
		{
			return	-1;
		}

		for(Tv_WkIdx = 0; Tv_WkIdx < V_GrP5Env.ItemCnt; Tv_WkIdx++)
		{
			if(A_UserData == V_GrP5Env.ItemTbl[Tv_WkIdx].UserData)
			{
				// found
				return	(__s32)Tv_WkIdx;
			}
		}

		return	-1;
}
//--------------------------------------------------------------------
BOOL8	GrP5ItemTextSet(__u32 A_ItemId, __u16 A_TxtId, WCHAR* A_Str)
{
	// local -------------------
	// code --------------------
		// check id
		if(V_GrP5Env.ItemCnt <= A_ItemId)
		{
			return	FALSE;
		}

		// update text
		V_GrP5Env.ItemTbl[A_ItemId].TxtId		=	A_TxtId;

		GrStrClear(V_GrP5Env.ItemTbl[A_ItemId].StrBuf);
		if(NULL != A_Str)
		{
			GrStrWcopy(V_GrP5Env.ItemTbl[A_ItemId].StrBuf, A_Str, E_GrP5ItemStrBufLen);
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	GrP5ItemStatSet(__u32 A_ItemId, __u32 A_Stat)
{
		// check id
		if(V_GrP5Env.ItemCnt <= A_ItemId)
		{
			return	FALSE;
		}

		// udpate attribute
		V_GrP5Env.ItemTbl[A_ItemId].Stat	=	A_Stat;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	GrP5ItemIconSet(__u32 A_ItemId, __u16 A_IconId)
{
		// check id
		if(V_GrP5Env.ItemCnt <= A_ItemId)
		{
			return	FALSE;
		}

		// udpate attribute
		V_GrP5Env.ItemTbl[A_ItemId].IconId	=	A_IconId;

		return	TRUE;

}
//--------------------------------------------------------------------
__u32	GrP5ItemCntBySquad(__u16 A_Suqad)
{
	// local -------------------
		__u32	Tv_WkIdx;
		__u32	Tv_Result;
	// code --------------------
		// check item exist
		if(0 == V_GrP5Env.ItemCnt)
		{
			return	0;
		}

		Tv_Result		=	0;
		for(Tv_WkIdx = 0; Tv_WkIdx < V_GrP5Env.ItemCnt; Tv_WkIdx++)
		{
			if(A_Suqad == V_GrP5Env.ItemTbl[Tv_WkIdx].Squad)
			{
				Tv_Result ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP5MenuPopByPos(__s32 A_PosX, __s32 A_PosY, __u8 A_ConIdx, __u8 A_Layer, __u8 A_FontId, Ptr_GrP5FontDrawStyle A_PtrTxtStyle,
	__u32 A_Width, __u16 A_Squad, __u16 A_CtrlId)
{
	// local -------------------
		Cls_GrP5SkinBase*	Tv_Skin;
		__u32	Tv_WinSizeX;
		__u32	Tv_WinSizeY;
		__u32	Tv_ItmCnt;
		St_GrP5RelRect	Tv_RelRcPop;
		Cls_GrP5ConBase*	Tv_Con;
		St_GrRect	Tv_RcScr;
		Cls_GrP5WinPopMenu*	Tv_PopMenu;
	// code --------------------

		if(E_GrP5ConMaxCnt <= A_ConIdx)
		{
			return	FALSE;
		}

		if(NULL == V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
		{
			return	FALSE;
		}

		if(E_GrP5PopMaxDepth <= V_GrP5Env.PopCtl.WinCnt)
		{
			GrP5PopupCloseAll();
		}

		Tv_Skin	=	GrP5SkinGet();
		if(NULL == Tv_Skin)
		{
			return	FALSE;
		}

		// check parametter
		if((0 == A_Width) || (E_GrP5ItemSquadIdNone == A_Squad))
		{
			return	FALSE;
		}

		Tv_ItmCnt	=	GrP5ItemCntBySquad(A_Squad);
		if(0 == Tv_ItmCnt)
		{
			return	FALSE;
		}

		// update popup environment
		V_GrP5Env.PopCtl.WinWidth	=	A_Width;
		V_GrP5Env.PopCtl.FontId		=	A_FontId;
		GrDumyCopyMem(&V_GrP5Env.PopCtl.TxtStyle, A_PtrTxtStyle, sizeof(St_GrP5FontDrawStyle));

		// get window size
		Tv_Skin->CtlGetSizeByItemCnt(E_GrP5ClassIdWinPopMenu, Tv_ItmCnt, &Tv_WinSizeX, &Tv_WinSizeY);
		if((0 == Tv_WinSizeX) || (0 == Tv_WinSizeY))
		{
			return	FALSE;
		}

		// calculate rect
		Tv_Con		=	(Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon;
		Tv_Con->ScrRectGet(&Tv_RcScr);

		// build normal rect
		Tv_RelRcPop.Left.Ratio		=	0;
		Tv_RelRcPop.Top.Ratio			=	0;
		Tv_RelRcPop.Right.Ratio		=	0;
		Tv_RelRcPop.Bottom.Ratio	=	0;

		// check horizontal
		if((A_PosX + (__s32)Tv_WinSizeX) <= Tv_RcScr.right)
		{
			Tv_RelRcPop.Left.Val	=	(__s16)A_PosX;
			Tv_RelRcPop.Right.Val	=	(__s16)(A_PosX + (__s32)Tv_WinSizeX);
		}
		else
		{
			Tv_RelRcPop.Left.Val	=	(__s16)(A_PosX - (__s32)Tv_WinSizeX);
			Tv_RelRcPop.Right.Val	=	(__s16)A_PosX;
		}

		// check vertical
		if((A_PosY + (__s32)Tv_WinSizeY) <= Tv_RcScr.bottom)
		{
			Tv_RelRcPop.Top.Val			=	(__s16)A_PosY;
			Tv_RelRcPop.Bottom.Val	=	(__s16)(A_PosY + (__s32)Tv_WinSizeY);
		}
		else
		{
			Tv_RelRcPop.Top.Val			=	(__s16)(A_PosY - (__s32)Tv_WinSizeY);
			Tv_RelRcPop.Bottom.Val	=	(__s16)A_PosY;
		}

		// do create
		Tv_PopMenu	=	(Cls_GrP5WinPopMenu*)new Cls_GrP5WinPopMenu(A_ConIdx, A_Layer, &Tv_RelRcPop, A_CtrlId, A_Squad);

		return	TRUE;
}
//--------------------------------------------------------------------
__s32	GrP5PopWinIdxByUo(void* A_UiObj)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_ObjWin;
		__u16	Tv_ObjId;
		__u8	Tv_WinIdx;
	// code --------------------
		// check null
		if(NULL == A_UiObj)
		{
			return	-1;
		}

		// check popup window conut
		if(0 == V_GrP5Env.PopCtl.WinCnt)
		{
			return	-1;
		}

		Tv_ObjWin	=	((Cls_GrP5UoBase*)A_UiObj)->ParentWinGet();
		Tv_ObjId	=	Tv_ObjWin->ObjIdGet();

		for( Tv_WinIdx = 0; Tv_WinIdx < V_GrP5Env.PopCtl.WinCnt; Tv_WinIdx++)
		{
			if(Tv_ObjId == V_GrP5Env.PopCtl.WinCtl[Tv_WinIdx].WinObjId)
			{
				// found
				return	(__s32)Tv_WinIdx;
			}
		}

		return	-1;
}
//--------------------------------------------------------------------
BOOL8	GrP5MenuSubPop(Ptr_GrRect A_PtrRect, __u8 A_ConIdx, __u8 A_Layer, __u16 A_Squad, __u16 A_CtrlId)
{
	// local -------------------
		Cls_GrP5SkinBase*	Tv_Skin;
		__u32	Tv_WinSizeX;
		__u32	Tv_WinSizeY;
		__u32	Tv_ItmCnt;
		St_GrP5RelRect	Tv_RelRcPop;
		Cls_GrP5ConBase*	Tv_Con;
		St_GrRect	Tv_RcScr;
		Cls_GrP5WinPopMenu*	Tv_PopMenu;

	// code --------------------
		if(E_GrP5ConMaxCnt <= A_ConIdx)
		{
			return	FALSE;
		}

		if(NULL == V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
		{
			return	FALSE;
		}

		if(E_GrP5PopMaxDepth <= V_GrP5Env.PopCtl.WinCnt)
		{
			return	FALSE;
		}

		Tv_Skin	=	GrP5SkinGet();
		if(NULL == Tv_Skin)
		{
			return	FALSE;
		}

		// check item exist
		if(0 == V_GrP5Env.ItemCnt)
		{
			return	FALSE;
		}

		// check parametter
		if((0 == V_GrP5Env.PopCtl.WinWidth) || (E_GrP5ItemSquadIdNone == A_Squad))
		{
			return	FALSE;
		}

		Tv_ItmCnt	=	GrP5ItemCntBySquad(A_Squad);
		if(0 == Tv_ItmCnt)
		{
			return	FALSE;
		}

		// get window size
		Tv_Skin->CtlGetSizeByItemCnt(E_GrP5ClassIdWinPopMenu, Tv_ItmCnt, &Tv_WinSizeX, &Tv_WinSizeY);
		if((0 == Tv_WinSizeX) || (0 == Tv_WinSizeY))
		{
			return	FALSE;
		}

		// calculate rect
		Tv_Con		=	(Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon;
		Tv_Con->ScrRectGet(&Tv_RcScr);

		// build normal rect
		Tv_RelRcPop.Left.Ratio		=	0;
		Tv_RelRcPop.Top.Ratio			=	0;
		Tv_RelRcPop.Right.Ratio		=	0;
		Tv_RelRcPop.Bottom.Ratio	=	0;

		// check horizontal
		if((A_PtrRect->right + (__s32)Tv_WinSizeX) <= Tv_RcScr.right)
		{
			Tv_RelRcPop.Left.Val	=	(__s16)A_PtrRect->right;
			Tv_RelRcPop.Right.Val	=	(__s16)(A_PtrRect->right + (__s32)Tv_WinSizeX);
		}
		else
		{
			Tv_RelRcPop.Left.Val	=	(__s16)(A_PtrRect->left - (__s32)Tv_WinSizeX);
			Tv_RelRcPop.Right.Val	=	(__s16)A_PtrRect->left;
		}

		// check vertical
		if((A_PtrRect->top + (__s32)Tv_WinSizeY) <= Tv_RcScr.bottom)
		{
			Tv_RelRcPop.Top.Val			=	(__s16)A_PtrRect->top;
			Tv_RelRcPop.Bottom.Val	=	(__s16)(A_PtrRect->top + (__s32)Tv_WinSizeY);
		}
		else
		{
			Tv_RelRcPop.Top.Val			=	(__s16)(A_PtrRect->bottom - (__s32)Tv_WinSizeY);
			Tv_RelRcPop.Bottom.Val	=	(__s16)A_PtrRect->bottom;
		}

		// do create
		Tv_PopMenu	=	(Cls_GrP5WinPopMenu*)new Cls_GrP5WinPopMenu(A_ConIdx, A_Layer, &Tv_RelRcPop, A_CtrlId, A_Squad);
		GrP5KeyFocusReq(Tv_PopMenu->ObjIdGet(), 0);

		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	GrP5ListPopByRect(Ptr_GrRect A_PtrRect, __u8 A_ConIdx, __u8 A_Layer, __u8 A_FontId, Ptr_GrP5FontDrawStyle A_PtrTxtStyle, 
	__u32 A_ViewCnt, __u32 A_ViewIdx, __u16 A_HostObjId)
{
	// local -------------------
		Cls_GrP5SkinBase*	Tv_Skin;
		__u32	Tv_WinSizeX;
		__u32	Tv_WinSizeY;
		Cls_GrP5ConBase*	Tv_Con;
		St_GrP5RelRect	Tv_RelRcPop;
		St_GrRect	Tv_RcScr;
		Cls_GrP5WinPopList*	Tv_PopList;
	// code --------------------
		if(E_GrP5ConMaxCnt <= A_ConIdx)
		{
			return	FALSE;
		}

		if(NULL == V_GrP5Env.ConTbl[A_ConIdx].ObjCon)
		{
			return	FALSE;
		}

		if(0 != V_GrP5Env.PopCtl.WinCnt)
		{
			GrP5PopupCloseAll();
		}

		Tv_Skin	=	GrP5SkinGet();
		if(NULL == Tv_Skin)
		{
			return	FALSE;
		}

		// check item exist
		if(0 == V_GrP5Env.ItemCnt)
		{
			return	FALSE;
		}

		// update popup environment
		V_GrP5Env.PopCtl.WinWidth	=	(__u32)(A_PtrRect->right - A_PtrRect->left);
		V_GrP5Env.PopCtl.FontId		=	A_FontId;
		GrDumyCopyMem(&V_GrP5Env.PopCtl.TxtStyle, A_PtrTxtStyle, sizeof(St_GrP5FontDrawStyle));

		// get window size
		Tv_Skin->CtlGetSizeByItemCnt(E_GrP5ClassIdWinPopList, A_ViewCnt, &Tv_WinSizeX, &Tv_WinSizeY);
		if((0 == Tv_WinSizeX) || (0 == Tv_WinSizeY))
		{
			return	FALSE;
		}

		// calculate rect
		Tv_Con		=	(Cls_GrP5ConBase*)V_GrP5Env.ConTbl[A_ConIdx].ObjCon;
		Tv_Con->ScrRectGet(&Tv_RcScr);

		// build normal rect
		Tv_RelRcPop.Left.Ratio		=	0;
		Tv_RelRcPop.Top.Ratio			=	0;
		Tv_RelRcPop.Right.Ratio		=	0;
		Tv_RelRcPop.Bottom.Ratio	=	0;

		Tv_RelRcPop.Left.Val			=	A_PtrRect->left;
		Tv_RelRcPop.Right.Val			=	A_PtrRect->right;

		// check up point
		if((A_PtrRect->bottom + (__s32)Tv_WinSizeY) <= Tv_RcScr.bottom)
		{
			// down direction
			Tv_RelRcPop.Top.Val			=	A_PtrRect->bottom;
			Tv_RelRcPop.Bottom.Val	=	(__s16)(A_PtrRect->bottom + (__s32)Tv_WinSizeY);
		}
		else
		{
			// up direction
			Tv_RelRcPop.Bottom.Val	=	(__s16)A_PtrRect->top;
			Tv_RelRcPop.Top.Val			=	(__s16)(A_PtrRect->top - (__s32)Tv_WinSizeY);
		}

		// do create
		Tv_PopList	=	(Cls_GrP5WinPopList*)new Cls_GrP5WinPopList(A_ConIdx, A_Layer, &Tv_RelRcPop, A_HostObjId,A_ViewIdx);
		GrP5KeyFocusReq(Tv_PopList->ObjIdGet(), 0);

		return	TRUE;

}
//--------------------------------------------------------------------


