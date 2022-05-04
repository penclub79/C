/*
Platform 5 UI object base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>

#include <P5/GrP5Key.h>
#include <P5/GrP5Mng.h>

#include <P5/GrP5Script.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <P5/GrP5WinBase.h>
#include <P5/GrP5Paint.h>

#ifdef GR_P5_EDITOR

#include <P5\Dvl\GrP5DvlBase.h>

#endif

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;

#ifdef GR_P5_EDITOR

extern	St_GrP5DvlEnv	V_GrP5DvlEnv;

// relate position
#define	E_GrP5UoBaseRelPosInfoCnt							8

St_GrP5DvlScptParaInfo	V_GrP5UoBaseRelPosInfo[E_GrP5UoBaseRelPosInfoCnt]	=
{
	{L"Left ratio", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5RelRect)0)->Left.Ratio, TRUE, E_GrP5DvlParaInfoActPosSizeChg, L"Ratio value of parent object (128 base)"},
	{L"Top ratio", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5RelRect)0)->Top.Ratio, TRUE, E_GrP5DvlParaInfoActPosSizeChg, L"Ratio value of parent object (128 base)"},
	{L"Right ratio", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5RelRect)0)->Right.Ratio, TRUE, E_GrP5DvlParaInfoActPosSizeChg, L"Ratio value of parent object (128 base)"},
	{L"Bottom ratio", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5RelRect)0)->Bottom.Ratio, TRUE, E_GrP5DvlParaInfoActPosSizeChg, L"Ratio value of parent object (128 base)"},

	{L"Left value", E_GrVarTypeS16, 1, (void*)&((Ptr_GrP5RelRect)0)->Left.Val, TRUE, E_GrP5DvlParaInfoActPosSizeChg, L"value of parent object"},
	{L"Top value", E_GrVarTypeS16, 1, (void*)&((Ptr_GrP5RelRect)0)->Top.Val, TRUE, E_GrP5DvlParaInfoActPosSizeChg, L"value of parent object"},
	{L"Right value", E_GrVarTypeS16, 1, (void*)&((Ptr_GrP5RelRect)0)->Right.Val, TRUE, E_GrP5DvlParaInfoActPosSizeChg, L"value of parent object"},
	{L"Bottom value", E_GrVarTypeS16, 1, (void*)&((Ptr_GrP5RelRect)0)->Bottom.Val, TRUE, E_GrP5DvlParaInfoActPosSizeChg, L"value of parent object"},
};


// text para

#define	E_GrP5UoBaseTxtParaInfoCnt							16

St_GrP5DvlScptParaInfo	V_GrP5UoBaseTxtParaInfo[E_GrP5UoBaseTxtParaInfoCnt]	=
{
	{L"AlignHz", E_GrVarTypeAlignHz, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->AlignHz, TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Align Horizontal"},
	{L"AlignVt", E_GrVarTypeAlignVt, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->AlignVt, TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Align Vertical"},
	{L"GapChar", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->GapChar, TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Gap of charactor"},
	{L"GapLine", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->GapLine, TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Gap of line"},
	{L"IsDrawText", E_GrVarTypeBitBoolBit0, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->Atb, TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Draw text"},
	{L"IsDrawShadow", E_GrVarTypeBitBoolBit1, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->Atb, TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Draw shadow"},
	{L"IsWordWarp", E_GrVarTypeBitBoolBit2, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->Atb, TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Word warp"},
	{L"IsFixPitch", E_GrVarTypeBitBoolBit3, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->Atb, TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Fix pitch"},
	{L"IsPassword", E_GrVarTypeBitBoolBit4, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->Atb, TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Charactor instead to specific charactor"},
	{L"IsSemiEnter", E_GrVarTypeBitBoolBit5, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->Atb, TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Semicolon instead to line feed"},
	{L"CodePw", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->CodePw, TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Password charactor"},
	{L"ColorTextNormal", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->ColorTxtTbl[0], TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Text color normal"},
	{L"ColorTextDisable", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->ColorTxtTbl[1], TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Text color disable"},
	{L"ColorTextFocus", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->ColorTxtTbl[2], TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Text color focus"},
	{L"ColorTextSelect", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->ColorTxtTbl[3], TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Text color select"},
	{L"ColorShadow", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5FontDrawStyle)0)->ColorShadow, TRUE, E_GrP5DvlParaInfoActFontParaChg, L"Shadow color"},
};


// Uo Base
WCHAR	V_GrP5UoBaseStrParaSepBasicInfo[]			=	L"Basic info";
WCHAR	V_GrP5UoBaseStrParaSepPosition[]			=	L"Position";
WCHAR	V_GrP5UoBaseStrParaSepBase[]					=	L"UoBase";

WCHAR	V_GrP5UoBaseStrParaNameScptId[]				=	L"ScptId";
WCHAR	V_GrP5UoBaseStrParaNameClassId[]			=	L"ClassId";

WCHAR	V_GrP5UoBaseStrParaDescScptId[]				=	L"Script ID";
WCHAR	V_GrP5UoBaseStrParaDescClassId[]			=	L"Class ID";


#define	E_GrP5UoBaseParaInfoCnt							23

St_GrP5DvlScptParaInfo	V_GrP5UoBaseParaInfo[E_GrP5UoBaseParaInfoCnt]	=	
{
	{L"CtrlId", E_GrP5DvlValTypeCtrlId, 1, (void*)&((Ptr_GrP5UoBasePara)0)->CtrlId, TRUE, 0, L"Object control ID"},
	{L"UserData[0]", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoBasePara)0)->UserData[0], TRUE, 0, L"User data 0"},
	{L"UserData[1]", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoBasePara)0)->UserData[1], TRUE, 0, L"User data 1"},
	{L"UserData[2]", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoBasePara)0)->UserData[2], TRUE, 0, L"User data 2"},
	{L"UserData[3]", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoBasePara)0)->UserData[3], TRUE, 0, L"User data 3"},
	{L"IsVisible", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBasePara)0)->IsVisible, TRUE, 0, L"Set Visible"},
	{L"IsEnable", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBasePara)0)->IsEnable, TRUE, 0, L"Set Enable"},
	{L"UserId", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoBasePara)0)->UserId, TRUE, 0, L"User ID"},
	{L"KeyFcsLeft", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBasePara)0)->KeyFcs[0], TRUE, 0, L"Key move focus script id left"},
	{L"KeyFcsUp", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBasePara)0)->KeyFcs[1], TRUE, 0, L"Key move focus script id up"},
	{L"KeyFcsRight", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBasePara)0)->KeyFcs[2], TRUE, 0, L"Key move focus script id right"},
	{L"KeyFcsDown", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBasePara)0)->KeyFcs[3], TRUE, 0, L"Key move focus script id down"},
	{L"IsCtlEvtKey", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBasePara)0)->IsCtlEvtKey, TRUE, 0, L"Control event key use"},
	{L"IsCtlEvtMse", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBasePara)0)->IsCtlEvtMse, TRUE, 0, L"Control event mouse use"},
	{L"TtTxtid", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoBasePara)0)->TtTxtid, TRUE, 0, L"Tooltip text id"},

	{L"SkinSubType", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBasePara)0)->SkinPara.SubType, TRUE, 0, L"Skin Subtype"},
	{L"SkinParaByte0", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBasePara)0)->SkinPara.ParaByte0, TRUE, 0, L"Skin paramter __u8"},
	{L"SkinParaByte1", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBasePara)0)->SkinPara.ParaByte1, TRUE, 0, L"Skin paramter __u8"},
	{L"SkinParaByte2", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBasePara)0)->SkinPara.ParaByte2, TRUE, 0, L"Skin paramter __u8"},
	{L"SkinParaWord0", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBasePara)0)->SkinPara.ParaWord0, TRUE, 0, L"Skin paramter __u16"},
	{L"SkinParaWord1", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBasePara)0)->SkinPara.ParaWord1, TRUE, 0, L"Skin paramter __u16"},
	{L"SkinParaDword", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoBasePara)0)->SkinPara.ParaDword, TRUE, 0, L"Skin paramter __u32"},
	{L"SkinParaColor", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoBasePara)0)->SkinPara.ParaColor, TRUE, 0, L"Skin paramter Color"},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoBase::Cls_GrP5UoBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		GrDumyZeroMem(m_InhHierTbl, sizeof(m_InhHierTbl));
		m_InhHierCnt	=	0;

		ClassRegist(E_GrP5ClassIdUoBase,&m_ParaUoBase,sizeof(m_ParaUoBase), NULL);

		m_ObjPrnt		=	A_ObjPrnt;
		m_ScptId		=	A_ScptId;
		m_ObjId			=	E_GrP5ObjIdNone;

		m_SkinMng		=	NULL;

#ifdef GR_P5_EDITOR
		m_DvlScptHdPara	=	0;
		m_DvlIsNotScpt	=	FALSE;
#endif

		// default setting
		GrDumyZeroMem(&m_ParaUoBase, sizeof(m_ParaUoBase));
		m_ParaUoBase.IsEnable		=	TRUE;
		m_ParaUoBase.IsVisible	=	TRUE;

		// regist object
		m_ObjSlave	=	NULL;
		m_ObjNext		=	NULL;
		if(NULL != m_ObjPrnt)
		{
			m_ObjNext	=	m_ObjPrnt->SlaveObjGet();		// get next object
			m_ObjPrnt->RtlSlaveObjSet(this);
			m_ObjWin	=	m_ObjPrnt->ParentWinGet();
		}

		// rect set
		m_RcRel			=	*A_PtrRelRect;

		// slave object only
		if(NULL != m_ObjPrnt)
		{
			// get console index
			m_ConIdx		=	(__u8)((m_ObjPrnt->ObjIdGet() >>E_GrP5ObjIdConIdxBitPos) & E_GrP5ObjIdConIdxBitMask);

			// build screen rect
			RtlScrRectBld();

#ifdef GR_P5_EDITOR
			// allocate script id
			if(E_GrP5DvlScptIdReqAlloc == A_ScptId)
			{
				m_ScptId	=	GrP5DvlSlaveScptIdAlloc();
			}
#endif

			// allocate object id
			m_ObjId	=	GrP5SlaveObjRegist(m_ConIdx, this);

		}

		// skin control update
		LcSkinCtlUpdate();

}
//--------------------------------------------------------------------
Cls_GrP5UoBase::~Cls_GrP5UoBase(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_ObjPrv;
	// code --------------------
		// release slave
		UoSlaveFreeAll();

		// patch chain
		if(NULL != m_ObjPrnt)
		{
			Tv_ObjPrv	=	PrevObjGet();
			if(NULL != Tv_ObjPrv)
			{
				Tv_ObjPrv->RtlNextObjSet(m_ObjNext);
			}
			else
			{
				m_ObjPrnt->RtlSlaveObjSet(m_ObjNext);
			}
		}

		// key focus release
		if(m_ObjId == V_GrP5Env.KeyObjId)
		{
			// request correct
			GrP5KeyFocusCorrectReq();
		}

		// mouse capture release
		if(V_GrP5Env.MseObjId == m_ObjId)
		{
			V_GrP5Env.MseObjId	=	E_GrP5ObjIdNone;
			V_GrP5Env.IsMseCap	=	FALSE;
			GrP5KeyFocusCorrectReq();
		}

		// unregist timer
		GrP5TimerUnregist(this);

		// unregist object
		GrP5ObjUnregist(m_ObjId);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::ClassRegist(__u16 A_ClassId, void* A_PtrPara, __u32 A_ParaSize, void* A_PtrSkinCtl)
{
	// local -------------------
	// code --------------------
		// check parametter
		if(E_GrP5UoHierarchyMax > m_InhHierCnt)
		{
			if(0x10000 > A_ParaSize)
			{
				if(0 != (3 & A_ParaSize))
				{
					DbgMsgPrint("Cls_GrP5UoBase::ClassIdRegist - bad arrangement parametter size (%d) , classid = %d\n", A_ParaSize, A_ClassId);
				}
				m_InhHierTbl[m_InhHierCnt].ClassId		=	A_ClassId;
				m_InhHierTbl[m_InhHierCnt].PtrPara		=	A_PtrPara;
				m_InhHierTbl[m_InhHierCnt].ParaSize		=	(__u16)A_ParaSize;
				m_InhHierTbl[m_InhHierCnt].PtrSkinCtl	=	A_PtrSkinCtl;

				m_ClassId		=	A_ClassId;		// update last class

				m_InhHierCnt ++;
			}
			else
			{
				DbgMsgPrint("Cls_GrP5UoBase::ClassIdRegist - parametter size (%d) over , classid = %d\n", A_ParaSize, A_ClassId);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP5UoBase::ClassIdRegist - inheritance count overflow , classid = %d\n", A_ClassId);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::UoSlaveFreeAll(void)
{
	// local -------------------
	// code --------------------
		while(NULL != m_ObjSlave)
		{
			delete	m_ObjSlave;
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoBase::ScptIdGet(void)
{
		return	m_ScptId;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoBase::ObjIdGet(void)
{
		return	m_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoBase::CtrlIdGet(void)
{
		return	m_ParaUoBase.CtrlId;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoBase::ClassIdGet(void)
{
		return	m_ClassId;
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoBase::UserIdGet(void)
{
		return	m_ParaUoBase.UserId;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoBase::TooltipTextIdGet(void)
{
		return	m_ParaUoBase.TtTxtid;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::IsCanKeyInput(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	IsKeyAble();

		if(((Cls_GrP5WinBase*)m_ObjWin)->IsKeySkip())
		{
			Tv_Result	=	FALSE;
		}
		if(Tv_Result)
		{
			Tv_Result	=	IsCanEnable();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::IsCanMseInput(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	IsMseAble();

		if(((Cls_GrP5WinBase*)m_ObjWin)->IsMseSkip())
		{
			Tv_Result	=	FALSE;
		}
		if(Tv_Result)
		{
			Tv_Result	=	IsCanEnable();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::IsEnable(void)
{
		return	m_ParaUoBase.IsEnable;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::IsVisible(void)
{
		return	m_ParaUoBase.IsVisible;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::IsKeyAble(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::IsMseAble(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::IsCanEnable(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5UoBase*	Tv_Obj;
	// code --------------------
		//init
		Tv_Result	=	TRUE;
		if((!IsEnable()) || (!IsVisible()))
		{
			Tv_Result	=	FALSE;
		}

		if(Tv_Result)
		{
			Tv_Obj		=	m_ObjPrnt;
			while(NULL != Tv_Obj)
			{
				// get parent
				if((!Tv_Obj->IsEnable()) || (!Tv_Obj->IsVisible()))
				{
					Tv_Result		=	FALSE;
					break;
				}
				//next
				Tv_Obj	=	Tv_Obj->ParentObjGet();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::IsCanVisible(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5UoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	IsVisible();

		if(Tv_Result)
		{
			Tv_Obj		=	this;
			while(NULL != Tv_Obj)
			{
				if(!Tv_Obj->IsVisible())
				{
					Tv_Result		=	FALSE;
					break;
				}
				//next
				Tv_Obj	=	Tv_Obj->ParentObjGet();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::IsOnFocuse(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

#ifdef GR_P5_EDITOR
		if(((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->DvlIsEdit())
		{
			return	FALSE;
		}
#endif

		if(V_GrP5Env.IsMseCusOn)
		{
			if(E_GrP5ObjIdNone != V_GrP5Env.MseObjId)
			{
				if(m_ObjId == V_GrP5Env.MseObjId)
				{
					Tv_Result	=	TRUE;
				}
			}
		}
		else
		{
			if(m_ObjId == V_GrP5Env.KeyObjId)
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::ParentWinGet(void)
{
		return	m_ObjWin;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::ParentObjGet(void)
{
		return	m_ObjPrnt;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::NextObjGet(void)
{
		return	m_ObjNext;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::PrevObjGet(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		Cls_GrP5UoBase*	Tv_ObjNext;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		if(NULL != m_ObjPrnt)
		{
			Tv_Result	=	m_ObjPrnt->SlaveObjGet();
			if(Tv_Result == (Cls_GrP5UoBase*)this)
			{
				Tv_Result	=	NULL;
			}
			else
			{
				while(NULL != Tv_Result)
				{
					//get next
					Tv_ObjNext	=	Tv_Result->NextObjGet();
					if(Tv_ObjNext == (Cls_GrP5UoBase*)this)
					{
						//found
						break;
					}
					//next
					Tv_Result	=	Tv_ObjNext;
				}
			}
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::SlaveObjGet(void)
{
		return	m_ObjSlave;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::RtlNextObjSet(Cls_GrP5UoBase* A_Obj)
{
		m_ObjNext	=	A_Obj;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::RtlSlaveObjSet(Cls_GrP5UoBase* A_Obj)
{
		m_ObjSlave	=	A_Obj;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::ScrRectGet(Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		*A_PtrRtRect	=	m_RcScr;
		if ((m_RcScr.left < m_RcScr.right) && (m_RcScr.top < m_RcScr.bottom))
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::RtlScrRectBld(void)
{
	// local -------------------
		BOOL8	Tv_IsAble;
		St_GrRect	Tv_RcPrnt;
		Cls_GrP5UoBase*	Tv_ObjSlave;
	// code --------------------
		// init
		Tv_IsAble	=	TRUE;
		// get parent screen rect
		if (NULL == m_ObjPrnt)
		{
			// get screen rect
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->ScrRectGet(&Tv_RcPrnt);
		}
		else
		{
			Tv_IsAble	=	m_ObjPrnt->ScrRectGet(&Tv_RcPrnt);
		}

		if (Tv_IsAble)
		{
			// calculate 
			GrP5RelRectToAbsRect(&m_RcRel, &Tv_RcPrnt, &m_RcScr);
		}
		else
		{
			// clear rect
			m_RcScr.left			=	0;
			m_RcScr.top				=	0;
			m_RcScr.right			=	0;
			m_RcScr.bottom		=	0;
		}
		
		// build slave object
		Tv_ObjSlave	=	m_ObjSlave;
		while (NULL != Tv_ObjSlave)
		{
			Tv_ObjSlave->RtlScrRectBld();
			// next
			Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::ReqDraw(BOOL8 A_IsForce)
{
	// local -------------------
		BOOL8	Tv_IsAble;
	// code --------------------

		Tv_IsAble	=	TRUE;
		if (!A_IsForce)
		{
			Tv_IsAble	=	IsCanVisible();
		}

		if (Tv_IsAble)
		{
			// check able
			if ((m_RcScr.left < m_RcScr.right) && (m_RcScr.top < m_RcScr.bottom))
			{
				LcPntReqByRect(&m_RcScr);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtPaintProc(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtBtnPress(__u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtBtnRelease(__u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtListItemSel(__u16 A_ObjId, __s32 A_ItemIdx)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtPopSelCancel(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtSpinValChanged(__u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtSpinClickCont(__u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtVkbdInput(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtVkbdCancel(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::RtlPaintProc(Cls_GrGdib* A_GdibScr, Ptr_GrRect A_PtrRcScr)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_ObjSlv;
		St_GrRect	Tv_RcClipOld;
	// code --------------------
		// check displayable
#ifdef GR_P5_EDITOR
		if(((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->DvlIsEdit() || m_ParaUoBase.IsVisible)
#else
		if(m_ParaUoBase.IsVisible)
#endif
		{
			// clipping
			if(GrP5PntClipSet(&m_RcScr,&Tv_RcClipOld))
			{

#ifdef GR_P5_EDITOR
				if(((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->DvlIsEdit())
				{
					// draw control mark
					GrP5PntReqLine(m_RcScr.left, m_RcScr.top, m_RcScr.right -1, m_RcScr.top, V_GrP5DvlEnv.ClrUoMark1);
					GrP5PntReqLine(m_RcScr.left, m_RcScr.top, m_RcScr.left, m_RcScr.bottom - 1, V_GrP5DvlEnv.ClrUoMark1);
					GrP5PntReqLine(m_RcScr.left, m_RcScr.bottom - 1, m_RcScr.right - 1, m_RcScr.bottom - 1, V_GrP5DvlEnv.ClrUoMark2);
					GrP5PntReqLine(m_RcScr.right - 1, m_RcScr.top, m_RcScr.right - 1, m_RcScr.bottom - 1, V_GrP5DvlEnv.ClrUoMark2);
				}
#endif

#ifdef GR_P5_EDITOR
				if(m_ParaUoBase.IsVisible)
				{
#endif
					// udpate skin object
					LcSkinCtlUpdate();
					if(NULL != m_SkinMng)
					{
						EvtPaintProc();		// pre paint process
						m_SkinMng->CtlDrawBase(this);
					}
#ifdef GR_P5_EDITOR
				}
#endif

				// paint slave
				Tv_ObjSlv	=	m_ObjSlave;
				while(NULL != Tv_ObjSlv)
				{
					// call paint
					Tv_ObjSlv->RtlPaintProc(A_GdibScr, A_PtrRcScr);
					// next
					Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
				}

				// restore clipper
				GrP5PntClipRestore(&Tv_RcClipOld);

			}
		}

}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::RtlUoFindByPos(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		Cls_GrP5UoBase*	Tv_ObjSlv;
		Cls_GrP5UoBase*	Tv_ObjFnd;
	// code --------------------
		Tv_Result	=	NULL;

		// check now position
		if ((A_X >= m_RcScr.left) && (A_X < m_RcScr.right) && (A_Y >= m_RcScr.top) && (A_Y < m_RcScr.bottom))
		{
			// found
			Tv_Result	=	this;

			// find slave object
			Tv_ObjSlv	=	m_ObjSlave;
			while (NULL != Tv_ObjSlv)
			{
				// check
				Tv_ObjFnd	=	Tv_ObjSlv->RtlUoFindByPos(A_X,A_Y);
				if (NULL != Tv_ObjFnd)
				{
					Tv_Result	=	Tv_ObjFnd;
					break;
				}
				// next
				Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::RtlUoCanMseFindByPos(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		Cls_GrP5UoBase*	Tv_ObjSlv;
		Cls_GrP5UoBase*	Tv_ObjFnd;
	// code --------------------
		Tv_Result	=	NULL;

		// check disable
		if(m_ParaUoBase.IsVisible && m_ParaUoBase.IsEnable)
		{
			// check now position
			if((A_X >= m_RcScr.left) && (A_X < m_RcScr.right) && (A_Y >= m_RcScr.top) && (A_Y < m_RcScr.bottom))
			{
				// check mouse able
				if(IsMseAble())
				{
					Tv_Result	=	this;
				}

				// find slave object
				Tv_ObjSlv	=	m_ObjSlave;
				while(NULL != Tv_ObjSlv)
				{
					// check
					Tv_ObjFnd	=	Tv_ObjSlv->RtlUoCanMseFindByPos(A_X, A_Y);
					if(NULL != Tv_ObjFnd)
					{
						Tv_Result	=	Tv_ObjFnd;
						break;
					}
					// next
					Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::RtlUoVisibleFindByPos(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		Cls_GrP5UoBase*	Tv_ObjSlv;
		Cls_GrP5UoBase*	Tv_ObjFnd;
	// code --------------------
		Tv_Result	=	NULL;

		// check disable
		if(m_ParaUoBase.IsVisible && m_ParaUoBase.IsEnable)
		{
			// check now position
			if((A_X >= m_RcScr.left) && (A_X < m_RcScr.right) && (A_Y >= m_RcScr.top) && (A_Y < m_RcScr.bottom))
			{
				Tv_Result	=	this;

				// find slave object
				Tv_ObjSlv	=	m_ObjSlave;
				while(NULL != Tv_ObjSlv)
				{
					// check
					Tv_ObjFnd	=	Tv_ObjSlv->RtlUoVisibleFindByPos(A_X, A_Y);
					if(NULL != Tv_ObjFnd)
					{
						Tv_Result	=	Tv_ObjFnd;
						break;
					}
					// next
					Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
		return	FALSE;
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoBase::UserDataGet(__u32 A_Idx)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if(E_GrP5UoUserDataCnt > A_Idx)
		{
			Tv_Result	=	m_ParaUoBase.UserData[A_Idx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::UserDataSet(__u32 A_Idx, __u32 A_Val)
{
	// local -------------------
	// code --------------------
		if(E_GrP5UoUserDataCnt > A_Idx)
		{
			m_ParaUoBase.UserData[A_Idx]	=	A_Val;
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::IsInheritClass(__u16 A_ClassId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if(0 == m_InhHierCnt)
		{
			DbgMsgPrint("Cls_GrP5UoBase::IsInheritClass - [BUG] Didn't inherit anything.");
		}
		else
		{
			for( Tv_WkIdx = 0; Tv_WkIdx < m_InhHierCnt; Tv_WkIdx++)
			{
				if(A_ClassId == m_InhHierTbl[Tv_WkIdx].ClassId)
				{
					// found
					Tv_Result	=	TRUE;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrP5UoHierarchy	Cls_GrP5UoBase::InheritClassInfoGet(__u16 A_ClassId)
{
	// local -------------------
		Ptr_GrP5UoHierarchy	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		// check count
		if(0 == m_InhHierCnt)
		{
			return	NULL;
		}

		Tv_Result	=	NULL;

		for(Tv_WkIdx = 0; Tv_WkIdx < m_InhHierCnt; Tv_WkIdx++)
		{
			if(A_ClassId == m_InhHierTbl[Tv_WkIdx].ClassId)
			{
				// found
				Tv_Result	=	&m_InhHierTbl[Tv_WkIdx];
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::ParentClassObjectGet(__u16 A_ClassId)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		Cls_GrP5UoBase*	Tv_UoPrnt;
	// code --------------------
		Tv_Result	=	NULL;

		Tv_UoPrnt	=	m_ObjPrnt;
		while(NULL != Tv_UoPrnt)
		{
			if(Tv_UoPrnt->IsInheritClass(A_ClassId))
			{
				// found
				Tv_Result	=	Tv_UoPrnt;
				break;
			}
			// next
			Tv_UoPrnt	=	Tv_UoPrnt->ParentObjGet();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::RtlUoFindKeyFocusAble(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		Cls_GrP5UoBase*	Tv_ObjSlv;
	// code --------------------
		Tv_Result		=	NULL;

		if(m_ParaUoBase.IsEnable && m_ParaUoBase.IsVisible)
		{
			if(IsCanKeyInput())
			{
				Tv_Result	=	this;
			}
			else
			{
				Tv_ObjSlv	=	m_ObjSlave;
				while(NULL != Tv_ObjSlv)
				{
					Tv_Result	=	Tv_ObjSlv->RtlUoFindKeyFocusAble();
					if(NULL != Tv_Result)
					{
						break;
					}
					// next
					Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
				}
			}
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtMseEnter(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtMseLeave(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtQuadChanged(void)
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::EvtKeyDown(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
		__u8	Tv_Dir;
		__u8	Tv_Rvs;
		__u32	Tv_FcsPara;
		Cls_GrP5UoBase*	Tv_ObjFnd;
	// code --------------------
		Tv_Result	=	FALSE;
		// check event
		if(m_ParaUoBase.IsCtlEvtKey)
		{
			Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
			if(NULL != Tv_Ctl)
			{
				Tv_Result	=	Tv_Ctl->EvtKeyDown(m_ParaUoBase.CtrlId, m_ObjId, A_Key);
			}
		}

		if(!Tv_Result)
		{
			Tv_Result	=	TRUE;
			switch(A_Key)
			{
				case E_GrP5KeyLeft:
					Tv_Dir	=	E_GrP5DirLeft;
					Tv_Rvs	=	E_GrP5DirRight;
					Tv_FcsPara	=	E_GrP5FocusParaByLeft;
					break;
				case E_GrP5KeyUp:
					Tv_Dir	=	E_GrP5DirUp;
					Tv_Rvs	=	E_GrP5DirDown;
					Tv_FcsPara	=	E_GrP5FocusParaByUp;
					break;
				case E_GrP5KeyRight:
					Tv_Dir	=	E_GrP5DirRight;
					Tv_Rvs	=	E_GrP5DirLeft;
					Tv_FcsPara	=	E_GrP5FocusParaByRight;
					break;
				case E_GrP5KeyDown:
					Tv_Dir	=	E_GrP5DirDown;
					Tv_Rvs	=	E_GrP5DirUp;
					Tv_FcsPara	=	E_GrP5FocusParaByDown;
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}

			if(Tv_Result)
			{
				Tv_ObjFnd	=	(Cls_GrP5UoBase*)GrP5UiObjFindKeyFocusByDir(m_ObjId, Tv_Dir);
				// find key object
				if(NULL != Tv_ObjFnd)
				{
					// change focus
					GrP5KeyFocusReq(Tv_ObjFnd->ObjIdGet(), Tv_FcsPara);
				}
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		Tv_Result	=	FALSE;
		// check event
		if(m_ParaUoBase.IsCtlEvtKey)
		{
			Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
			if(NULL != Tv_Ctl)
			{
				Tv_Result	=	Tv_Ctl->EvtKeyUp(m_ParaUoBase.CtrlId, m_ObjId, A_Key);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		Tv_Result	=	FALSE;
		// check event
		if(m_ParaUoBase.IsCtlEvtMse)
		{
			Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
			if(NULL != Tv_Ctl)
			{
				Tv_Result	=	Tv_Ctl->EvtMseLbtnDown(m_ParaUoBase.CtrlId, m_ObjId, A_X, A_Y, A_BtnMap);
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		Tv_Result	=	FALSE;
		// check event
		if(m_ParaUoBase.IsCtlEvtMse)
		{
			Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
			if(NULL != Tv_Ctl)
			{
				Tv_Result	=	Tv_Ctl->EvtMseLbtnUp(m_ParaUoBase.CtrlId, m_ObjId, A_X, A_Y, A_BtnMap);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::EvtMseRbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		Tv_Result	=	FALSE;
		// check event
		if(m_ParaUoBase.IsCtlEvtMse)
		{
			Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
			if(NULL != Tv_Ctl)
			{
				Tv_Result	=	Tv_Ctl->EvtMseRbtnDown(m_ParaUoBase.CtrlId, m_ObjId, A_X, A_Y, A_BtnMap);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::EvtMseRbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		Tv_Result	=	FALSE;
		// check event
		if(m_ParaUoBase.IsCtlEvtMse)
		{
			Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
			if(NULL != Tv_Ctl)
			{
				Tv_Result	=	Tv_Ctl->EvtMseRbtnUp(m_ParaUoBase.CtrlId, m_ObjId, A_X, A_Y, A_BtnMap);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::EvtMseMbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		Tv_Result	=	FALSE;
		// check event
		if(m_ParaUoBase.IsCtlEvtMse)
		{
			Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
			if(NULL != Tv_Ctl)
			{
				Tv_Result	=	Tv_Ctl->EvtMseMbtnDown(m_ParaUoBase.CtrlId, m_ObjId, A_X, A_Y, A_BtnMap);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::EvtMseMbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		Tv_Result	=	FALSE;
		// check event
		if(m_ParaUoBase.IsCtlEvtMse)
		{
			Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
			if(NULL != Tv_Ctl)
			{
				Tv_Result	=	Tv_Ctl->EvtMseMbtnUp(m_ParaUoBase.CtrlId, m_ObjId, A_X, A_Y, A_BtnMap);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::EvtMseWheelUp(__u8 A_WheelIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		Tv_Result	=	FALSE;
		// check event
		if(m_ParaUoBase.IsCtlEvtMse)
		{
			Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
			if(NULL != Tv_Ctl)
			{
				Tv_Result	=	Tv_Ctl->EvtMseWheelUp(m_ParaUoBase.CtrlId, m_ObjId, A_WheelIdx);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::EvtMseWheelDown(__u8 A_WheelIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		Tv_Result	=	FALSE;
		// check event
		if(m_ParaUoBase.IsCtlEvtMse)
		{
			Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
			if(NULL != Tv_Ctl)
			{
				Tv_Result	=	Tv_Ctl->EvtMseWheelDown(m_ParaUoBase.CtrlId, m_ObjId, A_WheelIdx);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtKeyFocusGet(__u32 A_Para)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtKeyFocusLoss(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtTimerTick(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtLanguageChg(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtScrlVtPos(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtScrlHzPos(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::RtlScptParaUpdt(void* A_PtrScpt)
{
	// local -------------------
		Ptr_GrP5ScptObjHead	Tv_PtrSoHd;
		Ptr_GrP5ScptParaHead	Tv_PtrPrHd;
		__u32	Tv_Prgs;
		void*	Tv_PtrData;
		__u32	Tv_DataSize;
		__u16	Tv_TblIdx;
		BOOL8	Tv_IsOk;
	// code --------------------
		Tv_PtrSoHd	=	(Ptr_GrP5ScptObjHead)A_PtrScpt;
		// check head
		if(E_GrP5ScptObjHdTcc == Tv_PtrSoHd->Tcc)
		{
			//update header parametter
#ifdef GR_P5_EDITOR
			m_DvlScptHdPara	=	Tv_PtrSoHd->HdPara;
#endif

			Tv_Prgs			=	0;
			Tv_PtrPrHd	=	(Ptr_GrP5ScptParaHead)((Def_GrCalPtr)Tv_PtrSoHd + sizeof(St_GrP5ScptObjHead));
			while(Tv_Prgs < Tv_PtrSoHd->ParaSize)
			{
				// check properly parametter
				if(E_GrP5ScptParaTcc != Tv_PtrPrHd->Tcc)
				{
					DbgMsgPrint("Cls_GrP5UoBase::RtlScptParaUpdt - script (%d) has bad parametter TCC\n", m_ScptId);
					break;
				}
				// do update
				Tv_PtrData	=	(void*)((Def_GrCalPtr)Tv_PtrPrHd + sizeof(St_GrP5ScptParaHead));
				// check exist class
				if(0 != m_InhHierCnt)
				{
					Tv_IsOk	=	FALSE;
					for( Tv_TblIdx = 0; Tv_TblIdx < m_InhHierCnt; Tv_TblIdx++)
					{
						if(Tv_PtrPrHd->ClassId == m_InhHierTbl[Tv_TblIdx].ClassId)
						{
							// found
							Tv_DataSize	=	Tv_PtrPrHd->Size;
							if(Tv_DataSize >= m_InhHierTbl[Tv_TblIdx].ParaSize)
							{
								Tv_DataSize	=	m_InhHierTbl[Tv_TblIdx].ParaSize;
							}
							if(0 != Tv_DataSize)
							{
								GrDumyCopyMem(m_InhHierTbl[Tv_TblIdx].PtrPara, Tv_PtrData, (__u32)Tv_DataSize);
							}
							
							Tv_IsOk	=	TRUE;
							break;
						}
					}
					if(!Tv_IsOk)
					{
						DbgMsgPrint("Cls_GrP5UoBase::RtlScptParaUpdt - script (%d) has unused parametter (%d)\n", m_ScptId, Tv_PtrPrHd->ClassId);
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP5UoBase::RtlScptParaUpdt - script (%d) has not inheritance.\n", m_ScptId);
				}
				// next
				Tv_Prgs			=	Tv_Prgs + sizeof(St_GrP5ScptParaHead) + (__u32)Tv_PtrPrHd->Size;
				Tv_PtrPrHd	=	(Ptr_GrP5ScptParaHead)((Def_GrCalPtr)Tv_PtrData + (Def_GrCalPtr)Tv_PtrPrHd->Size);
			}

			// update user id
			GrP5UserIdRegist(m_ParaUoBase.UserId, m_ObjId);
		}
		else
		{
			DbgMsgPrint("Cls_GrP5UoBase::RtlScptParaUpdt - script (%d) has bad object TCC\n",m_ScptId);
		}
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
__u32	Cls_GrP5UoBase::DvlScptHdParaGet(void)
{
		return	m_DvlScptHdPara;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBase::DvlScptHdParaSet(__u32 A_Val)
{
		m_DvlScptHdPara	=	A_Val;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBase::DvlParaInfoAdd(void* A_PtrAdd, __u32 A_AddCnt, void* A_PtrBasis)
{
	// local -------------------
		Ptr_GrP5DvlScptParaInfo	Tv_PtrSr;
		Ptr_GrP5DvlScptParaInfo	Tv_PtrTg;
		__u32	Tv_WkIdx;
	// code --------------------
		if(0 != A_AddCnt)
		{
			// init
			Tv_PtrTg	=	(Ptr_GrP5DvlScptParaInfo)((Def_GrCalPtr)m_DvlPtrScptBuf + m_DvlScptPrgs);
			Tv_PtrSr	=	(Ptr_GrP5DvlScptParaInfo)A_PtrAdd;

			for( Tv_WkIdx = 0; Tv_WkIdx < A_AddCnt; Tv_WkIdx++)
			{
				Tv_PtrTg->ValType		=	Tv_PtrSr->ValType;
				Tv_PtrTg->ValCnt			=	Tv_PtrSr->ValCnt;
				Tv_PtrTg->StrName		=	Tv_PtrSr->StrName;
				Tv_PtrTg->StrDesc		=	Tv_PtrSr->StrDesc;
				Tv_PtrTg->IsEdit			=	Tv_PtrSr->IsEdit;
				Tv_PtrTg->Act				=	Tv_PtrSr->Act;

				if(NULL != A_PtrBasis)
				{
					Tv_PtrTg->PtrVal			=	(void*)((Def_GrCalPtr)A_PtrBasis + (Def_GrCalPtr)Tv_PtrSr->PtrVal);
				}
				else
				{
					Tv_PtrTg->PtrVal			=	Tv_PtrSr->PtrVal;
				}
				// next
				m_DvlScptPrgs		=	m_DvlScptPrgs + sizeof(St_GrP5DvlScptParaInfo);
				Tv_PtrSr ++;
				Tv_PtrTg ++;
			}

		}
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBase::DvlParaInfoRelPosAdd(WCHAR* A_StrSep, void* A_PtrRelPos)
{
	// local -------------------
		St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// separator
		Tv_ParaInfo.StrName		=	A_StrSep;
		Tv_ParaInfo.StrDesc		=	NULL;
		Tv_ParaInfo.ValType		=	E_GrP5DvlValTypeSeparator;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.Act				=	0;
		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);

		DvlParaInfoAdd(V_GrP5UoBaseRelPosInfo, E_GrP5UoBaseRelPosInfoCnt, A_PtrRelPos);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBase::DvlParaInfoTxtParaAdd(WCHAR* A_StrSep, void* A_PtrTxtPara)
{
	// local -------------------
		St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// separator
		Tv_ParaInfo.StrName		=	A_StrSep;
		Tv_ParaInfo.StrDesc		=	NULL;
		Tv_ParaInfo.ValType		=	E_GrP5DvlValTypeSeparator;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.Act				=	0;
		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);

		DvlParaInfoAdd(V_GrP5UoBaseTxtParaInfo, E_GrP5UoBaseTxtParaInfoCnt, A_PtrTxtPara);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
__u32	Cls_GrP5UoBase::DvlParaInfoGet(void* A_PtrRtBuf)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result				=	0;

		m_DvlScptPrgs		=	0;
		m_DvlPtrScptBuf	=	A_PtrRtBuf;

		DvlParaInfoBuild();

		if(0 != m_DvlScptPrgs)
		{
			Tv_Result	=	m_DvlScptPrgs / sizeof(St_GrP5DvlScptParaInfo);
		}

		return	Tv_Result;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBase::DvlParaInfoBuild(void)
{
	// local -------------------
		St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------

		// common
		// separator basic info
		Tv_ParaInfo.StrName		=	V_GrP5UoBaseStrParaSepBasicInfo;
		Tv_ParaInfo.StrDesc		=	NULL;
		Tv_ParaInfo.ValType		=	E_GrP5DvlValTypeSeparator;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.IsEdit		=	FALSE;
		Tv_ParaInfo.Act				=	0;
		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);

		// script id
		Tv_ParaInfo.StrName		=	V_GrP5UoBaseStrParaNameClassId;
		Tv_ParaInfo.StrDesc		=	V_GrP5UoBaseStrParaDescClassId;
		Tv_ParaInfo.ValType		=	E_GrP5DvlValTypeClassId;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.PtrVal		=	&m_ClassId;
		Tv_ParaInfo.IsEdit		=	FALSE;
		Tv_ParaInfo.Act				=	0;
		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);

		// class type
		Tv_ParaInfo.StrName		=	V_GrP5UoBaseStrParaNameScptId;
		Tv_ParaInfo.StrDesc		=	V_GrP5UoBaseStrParaDescScptId;
		Tv_ParaInfo.ValType		=	E_GrVarTypeU16;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.PtrVal		=	&m_ScptId;
		Tv_ParaInfo.IsEdit		=	FALSE;
		Tv_ParaInfo.Act				=	0;
		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);

		// position --------------------------

		DvlParaInfoRelPosAdd(V_GrP5UoBaseStrParaSepPosition,&m_RcRel);

		// base info --------------------------
		Tv_ParaInfo.StrName		=	V_GrP5UoBaseStrParaSepBase;
		Tv_ParaInfo.StrDesc		=	NULL;
		Tv_ParaInfo.ValType		=	E_GrP5DvlValTypeSeparator;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.Act				=	0;
		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);

			
		DvlParaInfoAdd(V_GrP5UoBaseParaInfo, E_GrP5UoBaseParaInfoCnt, &m_ParaUoBase);
		

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::RtlDoEvtScptDecDone(BOOL8 A_IsFocusUpdt)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_ObjSlv;
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		// do slave
		Tv_ObjSlv	=	m_ObjSlave;
		while (NULL != Tv_ObjSlv)
		{
			// do event
			Tv_ObjSlv->RtlDoEvtScptDecDone();
			// next
			Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
		}

		// do this object evnet
		EvtScptDecDone();
		// do control event
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			Tv_Ctrl->EvtScptDecDone(m_ParaUoBase.CtrlId, m_ObjId);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		LcSkinCtlUpdate();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------
		// control update
		LcSkinCtlUpdate();

		switch (A_Action)
		{
			case E_GrP5DvlParaInfoActPosSizeChg:
				ReqDraw(FALSE);
				RtlScrRectBld();
				EvtPosSizeChanged();
				ReqDraw(FALSE);
				break;
			default:
				ReqDraw(FALSE);
				break;
		}

}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::ParentScrRectGet(Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		// get parent
		if (NULL == m_ObjPrnt)
		{
			// get screen resoultion
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->ScrRectGet(A_PtrRtRect);
			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_Result	=	m_ObjPrnt->ScrRectGet(A_PtrRtRect);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoBase::ConsoleIdxGet(void)
{
	// local -------------------
	// code --------------------
		return	m_ConIdx;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::PosMoveAbsolute(__s32 A_X, __s32 A_Y, BOOL8 A_IsRedraw)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_RcPrnt;
		St_GrRect	Tv_RcTg;
	// code --------------------
		Tv_Result	=	FALSE;
		// get parent rect
		if (ParentScrRectGet(&Tv_RcPrnt))
		{
			// get target position
			Tv_RcTg.left		=	A_X + Tv_RcPrnt.left;
			Tv_RcTg.top			=	A_Y + Tv_RcPrnt.top;
			Tv_RcTg.right		=	Tv_RcTg.left + (m_RcScr.right - m_RcScr.left);
			Tv_RcTg.bottom	=	Tv_RcTg.top + (m_RcScr.bottom - m_RcScr.top);

			// draw previous
			if (A_IsRedraw)
			{
				ReqDraw(FALSE);
			}

			// calculate
			Tv_Result	=	GrP5AbsRectToRelRectFixRatio(&Tv_RcTg, &Tv_RcPrnt, &m_RcRel);

			RtlScrRectBld();
			EvtPosSizeChanged();
			if (Tv_Result && A_IsRedraw)
			{
				ReqDraw(FALSE);
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::PosMoveRelate(__s32 A_X, __s32 A_Y, BOOL8 A_IsRedraw)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_RcPrnt;
		St_GrRect	Tv_RcTg;
	// code --------------------
		Tv_Result	=	FALSE;
		// get parent rect
		if (ParentScrRectGet(&Tv_RcPrnt))
		{
			// get target position
			Tv_RcTg.left		=	A_X + m_RcScr.left;
			Tv_RcTg.top			=	A_Y + m_RcScr.top;
			Tv_RcTg.right		=	Tv_RcTg.left + (m_RcScr.right - m_RcScr.left);
			Tv_RcTg.bottom	=	Tv_RcTg.top + (m_RcScr.bottom - m_RcScr.top);

			// draw previous
			if (A_IsRedraw)
			{
				ReqDraw(FALSE);
			}

			// calculate
			Tv_Result	=	GrP5AbsRectToRelRectFixRatio(&Tv_RcTg, &Tv_RcPrnt, &m_RcRel);

			RtlScrRectBld();
			EvtPosSizeChanged();
			if (Tv_Result && A_IsRedraw)
			{
				ReqDraw(FALSE);
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::SizeChangeAbsolute(__s32 A_X, __s32 A_Y, BOOL8 A_IsRedraw)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_RcPrnt;
		St_GrRect	Tv_RcTg;
	// code --------------------
		Tv_Result	=	FALSE;
		// get parent rect
		if (ParentScrRectGet(&Tv_RcPrnt))
		{
			// get target position
			Tv_RcTg.left		=	m_RcScr.left;
			Tv_RcTg.top			=	m_RcScr.top;
			Tv_RcTg.right		=	Tv_RcTg.left + A_X;
			Tv_RcTg.bottom	=	Tv_RcTg.top + A_Y;

			// draw previous
			if (A_IsRedraw)
			{
				ReqDraw(FALSE);
			}

			// calculate
			Tv_Result	=	GrP5AbsRectToRelRectFixRatio(&Tv_RcTg, &Tv_RcPrnt, &m_RcRel);

			RtlScrRectBld();
			EvtPosSizeChanged();
			if (Tv_Result && A_IsRedraw)
			{
				ReqDraw(FALSE);
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::SizeChangeRelate(__s32 A_X, __s32 A_Y, BOOL8 A_IsRedraw)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_RcPrnt;
		St_GrRect	Tv_RcTg;
	// code --------------------
		Tv_Result	=	FALSE;
		// get parent rect
		if (ParentScrRectGet(&Tv_RcPrnt))
		{
			// get target position
			Tv_RcTg.left		=	m_RcScr.left;
			Tv_RcTg.top			=	m_RcScr.top;
			Tv_RcTg.right		=	Tv_RcTg.left + (m_RcScr.right - m_RcScr.left) + A_X;
			Tv_RcTg.bottom	=	Tv_RcTg.top + (m_RcScr.bottom - m_RcScr.top) + A_Y;

			// draw previous
			if (A_IsRedraw)
			{
				ReqDraw(FALSE);
			}

			// calculate
			Tv_Result	=	GrP5AbsRectToRelRectFixRatio(&Tv_RcTg, &Tv_RcPrnt, &m_RcRel);

			RtlScrRectBld();
			EvtPosSizeChanged();
			if (Tv_Result && A_IsRedraw)
			{
				ReqDraw(FALSE);
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::PosSizeChange(__s32 A_PosX, __s32 A_PosY, BOOL8 A_IsPosRel, __s32 A_SizeX, __s32 A_SizeY, BOOL8 A_IsSizeRel, BOOL8 A_IsRedraw)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_RcPrnt;
		St_GrRect	Tv_RcTg;
	// code --------------------
		Tv_Result	=	FALSE;
		// get parent rect
		if (ParentScrRectGet(&Tv_RcPrnt))
		{
			// get target position
			if (A_IsPosRel)
			{
				Tv_RcTg.left		=	m_RcScr.left + A_PosX;
				Tv_RcTg.top			=	m_RcScr.top + A_PosY;
			}
			else
			{
				Tv_RcTg.left		=	A_PosX;
				Tv_RcTg.top			=	A_PosY;
			}

			if (A_IsSizeRel)
			{
				Tv_RcTg.right		=	Tv_RcTg.left + (m_RcScr.right - m_RcScr.left) + A_SizeX;
				Tv_RcTg.bottom	=	Tv_RcTg.top + (m_RcScr.bottom - m_RcScr.top) + A_SizeY;
			}
			else
			{
				Tv_RcTg.right		=	Tv_RcTg.left + A_SizeX;
				Tv_RcTg.bottom	=	Tv_RcTg.top + A_SizeY;
			}

			// draw previous
			if (A_IsRedraw)
			{
				ReqDraw(FALSE);
			}

			// calculate
			Tv_Result	=	GrP5AbsRectToRelRectFixRatio(&Tv_RcTg, &Tv_RcPrnt, &m_RcRel);

			RtlScrRectBld();
			EvtPosSizeChanged();
			if (Tv_Result && A_IsRedraw)
			{
				ReqDraw(FALSE);
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBase::DvlScptParaAdd(__u16 A_ClassId, void* A_PtrPara, __u16 A_Size)
{
	// local -------------------
		Ptr_GrP5ScptParaHead	Tv_PtrParaHd;
		void*	Tv_PtrData;
	// code --------------------
		Tv_PtrParaHd	=	(Ptr_GrP5ScptParaHead)((Def_GrCalPtr)m_DvlPtrScptBuf + (Def_GrCalPtr)m_DvlScptPrgs);
		Tv_PtrParaHd->Tcc			=	E_GrP5ScptParaTcc;
		Tv_PtrParaHd->_rsvd0		=	0;
		Tv_PtrParaHd->ClassId	=	A_ClassId;
		Tv_PtrParaHd->Size			=	A_Size;

		Tv_PtrData		=	(void*)((Def_GrCalPtr)Tv_PtrParaHd + sizeof(St_GrP5ScptParaHead));

		GrDumyCopyMem(Tv_PtrData, A_PtrPara, A_Size);

		m_DvlScptPrgs	=	m_DvlScptPrgs + sizeof(St_GrP5ScptParaHead) + (__u32)A_Size;

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBase::DvlScptParaEncode(void)
{
	// local -------------------
		__u32	Tv_InhIdx;
	// code --------------------

		if(0 != m_InhHierCnt)
		{
			for(Tv_InhIdx = 0; Tv_InhIdx < m_InhHierCnt;Tv_InhIdx ++)
			{
				if(0 != m_InhHierTbl[Tv_InhIdx].ParaSize)
				{
					DvlScptParaAdd(m_InhHierTbl[Tv_InhIdx].ClassId, m_InhHierTbl[Tv_InhIdx].PtrPara, m_InhHierTbl[Tv_InhIdx].ParaSize);
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP5UoBase::DvlScptParaEncode - inherit class not exist!\n");
		}
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
__u32	Cls_GrP5UoBase::DvlScptEncode(void* A_PtrBuf)
{
	// local -------------------
		Ptr_GrP5ScptObjHead	Tv_PtrObjHd;
		Cls_GrP5UoBase*	Tv_UoSlv;
		void*	Tv_PtrScptSlv;
		Ptr_GrP5ScptObjEnd	Tv_PtrObjEd;
		__u32	Tv_ParaSize;
	// code --------------------

		// init
		m_DvlPtrScptBuf	=	A_PtrBuf;
		m_DvlScptPrgs		=	0;

		// check encode able
		if(DvlIsCanScptEnc())
		{
			// build head
			Tv_PtrObjHd	=	(Ptr_GrP5ScptObjHead)A_PtrBuf;
			Tv_PtrObjHd->Tcc					=	E_GrP5ScptObjHdTcc;
			Tv_PtrObjHd->HdPara			=	m_DvlScptHdPara;
			Tv_PtrObjHd->ClassId			=	m_ClassId;
			Tv_PtrObjHd->ScptId			=	m_ScptId;
			Tv_PtrObjHd->ParaSize		=	0;
			Tv_PtrObjHd->TotalSize		=	0;
			Tv_PtrObjHd->RelRect			=	m_RcRel;

			m_DvlScptPrgs	=	m_DvlScptPrgs + sizeof(St_GrP5ScptObjHead);

			// encode this object parametter
			DvlScptParaEncode();
			Tv_ParaSize							=	m_DvlScptPrgs - sizeof(St_GrP5ScptObjHead);
			if(0x10000 <= Tv_ParaSize)
			{
				DbgMsgPrint("Cls_GrP5UoBase::DvlScptEncode - script (%d) parametter size was big fit (%d)!", m_ScptId, Tv_ParaSize);
			}
			Tv_PtrObjHd->ParaSize		=	(__u16)Tv_ParaSize;

			// encode slave script
			Tv_UoSlv	=	m_ObjSlave;
			while(NULL != Tv_UoSlv)
			{
				Tv_PtrScptSlv	=	(void*)((Def_GrCalPtr)m_DvlPtrScptBuf + (Def_GrCalPtr)m_DvlScptPrgs);
				m_DvlScptPrgs	=	m_DvlScptPrgs + Tv_UoSlv->DvlScptEncode(Tv_PtrScptSlv);
				Tv_UoSlv	=	Tv_UoSlv->NextObjGet();
			}

			// mark end
			Tv_PtrObjEd	=	(Ptr_GrP5ScptObjEnd)((Def_GrCalPtr)m_DvlPtrScptBuf + (Def_GrCalPtr)m_DvlScptPrgs);
			Tv_PtrObjEd->Tcc			=	E_GrP5ScptObjEdTcc;
			Tv_PtrObjEd->ScptId	=	m_ScptId;

			m_DvlScptPrgs	=	m_DvlScptPrgs + sizeof(St_GrP5ScptObjEnd);

			// update total size
			Tv_PtrObjHd->TotalSize		=	m_DvlScptPrgs;
		}

		return	m_DvlScptPrgs;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
BOOL8	Cls_GrP5UoBase::DvlIsNotScpt(void)
{
		return	m_DvlIsNotScpt;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBase::DvlNotScptSet(BOOL8 A_IsSet)
{
		m_DvlIsNotScpt	=	A_IsSet;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
BOOL8	Cls_GrP5UoBase::DvlIsCanScptEnc(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5UoBase*	Tv_ObjChk;
	// code --------------------
		Tv_Result	=	TRUE;

		Tv_ObjChk	=	this;
		while(NULL != Tv_ObjChk)
		{
			if(Tv_ObjChk->DvlIsNotScpt())
			{
				Tv_Result	=	FALSE;
				break;
			}
			// next
			Tv_ObjChk	=	Tv_ObjChk->ParentObjGet();
		}

		return	Tv_Result;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
Cls_GrP5UoBase*	Cls_GrP5UoBase::DvlUoEditableFindByPos(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		Cls_GrP5UoBase*	Tv_ObjSlv;
		Cls_GrP5UoBase*	Tv_ObjFnd;
	// code --------------------
		Tv_Result	=	NULL;

		// check editable
		if(!m_DvlIsNotScpt)
		{
			// check now position
			if((A_X >= m_RcScr.left) && (A_X < m_RcScr.right) && (A_Y >= m_RcScr.top) && (A_Y < m_RcScr.bottom))
			{
				// found
				Tv_Result	=	this;

				// find slave object
				Tv_ObjSlv	=	m_ObjSlave;
				while(NULL != Tv_ObjSlv)
				{
					// check
					Tv_ObjFnd	=	Tv_ObjSlv->DvlUoEditableFindByPos(A_X, A_Y);
					if(NULL != Tv_ObjFnd)
					{
						Tv_Result	=	Tv_ObjFnd;
						break;
					}
					// next
					Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
				}
			}
		}

		return	Tv_Result;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBase::DvlPreDelete(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoSlv;
	// code --------------------
		// delete now
		GrP5DvlSlaveScptIdFree(m_ScptId);

		// call slave
		Tv_UoSlv	=	m_ObjSlave;
		while(NULL != Tv_UoSlv)
		{
			Tv_UoSlv->DvlPreDelete();
			Tv_UoSlv	=	Tv_UoSlv->NextObjGet();
		}

}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::RtlProcUiMsg(Ptr_GrP5MsgUiHd A_PtrMsg)
{
	return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::ReqKeyFocusSet(void)
{
	// local -------------------
		St_GrP5MsgKeyFcsUoSet	Tv_Msg;
	// code --------------------
		// check updateable
		if(IsCanKeyInput())
		{
			// post message
			Tv_Msg.CmdHd.Cmd	=	E_GrP5MsgCmdKeyFcsUoSet;
			Tv_Msg.ObjId			=	m_ObjId;
			
#ifdef GR_TASKMSG_DEBUG
			if(!GrP5MsgPost(&Tv_Msg, sizeof(Tv_Msg)))
			{
				DbgMsgPrint("[DBG] %s MsgPost fail!\n", __FUNCTION__);
			}
#else
			GrP5MsgPost(&Tv_Msg, sizeof(Tv_Msg));
#endif
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBase::IsHasParent(__u16 A_ObjId)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5UoBase*	Tv_ObjPrnt;
	// code --------------------
		Tv_Result	=	FALSE;
		
		Tv_ObjPrnt	=	m_ObjPrnt;
		while(NULL != Tv_ObjPrnt)
		{
			if(A_ObjId == Tv_ObjPrnt->ObjIdGet())
			{
				// found
				Tv_Result	=	TRUE;
				break;
			}
			// go next
			Tv_ObjPrnt	=	Tv_ObjPrnt->ParentObjGet();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::RtlUoFindByScptId(__u16 A_ScptId)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		Cls_GrP5UoBase*	Tv_ObjSlv;
	// code --------------------
		Tv_Result	=	NULL;

		if(A_ScptId == m_ScptId)
		{
			Tv_Result	=	this;
		}
		else
		{
			Tv_ObjSlv	=	m_ObjSlave;
			while(NULL != Tv_ObjSlv)
			{
				Tv_Result	=	Tv_ObjSlv->RtlUoFindByScptId(A_ScptId);
				if(NULL != Tv_Result)
				{
					break;
				}
				// next
				Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::RtlCtrlEvtValChgDo(__u8 A_VarType, __u16 A_VarId)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		// get control event
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			Tv_Ctrl->EvtVarChg(m_ParaUoBase.CtrlId, m_ObjId, A_VarType, A_VarId);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::MseCapture(void)
{
	// local -------------------
	// code --------------------
		if(m_ObjId == V_GrP5Env.MseObjId)
		{
			V_GrP5Env.IsMseCap	=	TRUE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::MseRelese(void)
{
		if(m_ObjId == V_GrP5Env.MseObjId)
		{
			V_GrP5Env.IsMseCap	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EnableSet(BOOL8 A_IsEnable)
{
		m_ParaUoBase.IsEnable	=	A_IsEnable;
		ReqDraw(FALSE);
		if(!m_ParaUoBase.IsEnable)
		{
			LcChkNeedKfcCor();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::VisiableSet(BOOL8 A_IsVisible)
{
		if(m_ParaUoBase.IsVisible != A_IsVisible)
		{
			m_ParaUoBase.IsVisible	=	A_IsVisible;
			ReqDraw(TRUE);
			// rebuild mouse cursor
			((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->ReqMouseObjUpdate();
			// check key foucs
			if(!m_ParaUoBase.IsVisible)
			{
				LcChkNeedKfcCor();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::KeyFocusSet(__u8 A_Dir, __u16 A_ScptId)
{
	// local -------------------
	// code --------------------
		if(E_GrP5DirCnt > A_Dir)
		{
			m_ParaUoBase.KeyFcs[A_Dir]	=	A_ScptId;
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoBase::KeyFocusGet(__u8 A_Dir)
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrP5ScptIdNone;
		if(E_GrP5DirCnt > A_Dir)
		{
			Tv_Result	=	m_ParaUoBase.KeyFcs[A_Dir];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::FindSlaveNearByDir(Ptr_GrRect A_PtrRcBase, __u8 A_Dir, BOOL8 A_IsKeyAbleOnly, __u16 A_ExcpObjId)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		Cls_GrP5UoBase*	Tv_ObjSlv;
	// code --------------------
		Tv_Result	=	NULL;

		if(IsEnable() && IsVisible() && (m_ObjId != A_ExcpObjId) && (!((Cls_GrP5WinBase*)m_ObjWin)->IsKeySkip()))
		{
			Tv_ObjSlv	=	m_ObjSlave;
			while(NULL != Tv_ObjSlv)
			{
				Tv_Result	=	RtlUoFindNearByDir(A_PtrRcBase, A_Dir, A_IsKeyAbleOnly, A_ExcpObjId, Tv_Result);
				// next
				Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::RtlUoFindNearByDir(Ptr_GrRect A_PtrRcBase, __u8 A_Dir, BOOL8 A_IsKeyAbleOnly, __u16 A_ExcpObjId, Cls_GrP5UoBase* A_RsltObj)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		St_GrRect	Tv_RcPrv;
		Cls_GrP5UoBase*	Tv_ObjSlv;
	// code --------------------
		Tv_Result	=	A_RsltObj;

		// check able
		if(m_ParaUoBase.IsVisible && m_ParaUoBase.IsEnable && (m_ObjId != A_ExcpObjId))
		{
			// check this
			if((!A_IsKeyAbleOnly) || IsKeyAble())
			{
				// check range
				if((E_GrP5DirLeft == A_Dir) || (E_GrP5DirRight == A_Dir))
				{
					// check horizontal
					if(((A_PtrRcBase->top <= m_RcScr.top) && (A_PtrRcBase->bottom >= m_RcScr.bottom)) ||
						((A_PtrRcBase->top >= m_RcScr.top) && (A_PtrRcBase->bottom <= m_RcScr.bottom)) ||
						((A_PtrRcBase->top <= m_RcScr.top) && (A_PtrRcBase->bottom > m_RcScr.top)) ||
						((A_PtrRcBase->top < m_RcScr.bottom) && (A_PtrRcBase->bottom >= m_RcScr.bottom)) ||
						((A_PtrRcBase->top >= m_RcScr.top) && (A_PtrRcBase->top < m_RcScr.bottom)) ||
						((A_PtrRcBase->bottom > m_RcScr.top) && (A_PtrRcBase->bottom <= m_RcScr.bottom)))
					{
						// check direction
						if(E_GrP5DirLeft == A_Dir)
						{
							// left
							if(A_PtrRcBase->left >= m_RcScr.right)
							{
								// check exist privious result
								if(NULL != A_RsltObj)
								{
									// check range
									if(A_RsltObj->ScrRectGet(&Tv_RcPrv))
									{
										if(m_RcScr.right > Tv_RcPrv.right)
										{
											// update new result
											Tv_Result	=	this;
										}
									}
								}
								else
								{
									// update new result
									Tv_Result	=	this;
								}
							}
						}
						else
						{
							// right
							if(A_PtrRcBase->right <= m_RcScr.left)
							{
								// check exist privious result
								if(NULL != A_RsltObj)
								{
									// check range
									if(A_RsltObj->ScrRectGet(&Tv_RcPrv))
									{
										if(m_RcScr.left < Tv_RcPrv.left)
										{
											// update new result
											Tv_Result	=	this;
										}
									}
								}
								else
								{
									// update new result
									Tv_Result	=	this;
								}
							}
						}
					}
				}	//	if((E_GrP5DirLeft == A_Dir) || (E_GrP5DirRight == A_Dir))
				else if((E_GrP5DirUp == A_Dir) || (E_GrP5DirDown == A_Dir))
				{
					// check vertical
					if(((A_PtrRcBase->left <= m_RcScr.left) && (A_PtrRcBase->right >= m_RcScr.right)) ||
						((A_PtrRcBase->left >= m_RcScr.left) && (A_PtrRcBase->right <= m_RcScr.right)) ||
						((A_PtrRcBase->left <= m_RcScr.left) && (A_PtrRcBase->right > m_RcScr.left)) ||
						((A_PtrRcBase->left < m_RcScr.right) && (A_PtrRcBase->right >= m_RcScr.right)) ||
						((A_PtrRcBase->left >= m_RcScr.left) && (A_PtrRcBase->left < m_RcScr.right)) ||
						((A_PtrRcBase->right > m_RcScr.left) && (A_PtrRcBase->right <= m_RcScr.right)))
					{
						// check direction
						if(E_GrP5DirUp == A_Dir)
						{
							// up
							if(A_PtrRcBase->top >= m_RcScr.bottom)
							{
								// check exist privious result
								if(NULL != A_RsltObj)
								{
									// check range
									if(A_RsltObj->ScrRectGet(&Tv_RcPrv))
									{
										if(m_RcScr.bottom > Tv_RcPrv.bottom)
										{
											// update new result
											Tv_Result	=	this;
										}
									}
								}
								else
								{
									// update new result
									Tv_Result	=	this;
								}
							}
						}
						else
						{
							// down
							if(A_PtrRcBase->bottom <= m_RcScr.top)
							{
								// check exist privious result
								if(NULL != A_RsltObj)
								{
									// check range
									if(A_RsltObj->ScrRectGet(&Tv_RcPrv))
									{
										if(m_RcScr.top < Tv_RcPrv.top)
										{
											// update new result
											Tv_Result	=	this;
										}
									}
								}
								else
								{
									// update new result
									Tv_Result	=	this;
								}
							}
						}
					}
				}	//	else if((E_GrP5DirUp == A_Dir) || (E_GrP5DirDown == A_Dir))
			}	// if(IsKeyAble())

			// find slave
			Tv_ObjSlv	=	m_ObjSlave;
			while(NULL != Tv_ObjSlv)
			{
				Tv_Result	=	Tv_ObjSlv->RtlUoFindNearByDir(A_PtrRcBase, A_Dir, A_IsKeyAbleOnly, A_ExcpObjId, Tv_Result);
				// next
				Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
			}

		}//	if(m_ParaUoBase.IsVisible && m_ParaUoBase.IsEnable && (m_ObjId != A_ExcpObjId))

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::FindSlaveFarByDir(Ptr_GrRect A_PtrRcBase, __u8 A_Dir, BOOL8 A_IsKeyAbleOnly, __u16 A_ExcpObjId)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		Cls_GrP5UoBase*	Tv_ObjSlv;
	// code --------------------
		Tv_Result	=	NULL;

		if(IsEnable() && IsVisible() && (m_ObjId != A_ExcpObjId) && (!((Cls_GrP5WinBase*)m_ObjWin)->IsKeySkip()))
		{
			Tv_ObjSlv	=	m_ObjSlave;
			while(NULL != Tv_ObjSlv)
			{
				Tv_Result	=	RtlUoFindFarByDir(A_PtrRcBase, A_Dir, A_IsKeyAbleOnly, A_ExcpObjId, Tv_Result);
				// next
				Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBase::RtlUoFindFarByDir(Ptr_GrRect A_PtrRcBase, __u8 A_Dir, BOOL8 A_IsKeyAbleOnly, __u16 A_ExcpObjId, Cls_GrP5UoBase* A_RsltObj)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
		St_GrRect	Tv_RcPrv;
		Cls_GrP5UoBase*	Tv_ObjSlv;
	// code --------------------
		Tv_Result	=	A_RsltObj;

		// check able
		if(m_ParaUoBase.IsVisible && m_ParaUoBase.IsEnable && (m_ObjId != A_ExcpObjId))
		{
			// check this
			if((!A_IsKeyAbleOnly) || IsKeyAble())
			{
				// check range
				if((E_GrP5DirLeft == A_Dir) || (E_GrP5DirRight == A_Dir))
				{
					// check horizontal
					if(((A_PtrRcBase->top <= m_RcScr.top) && (A_PtrRcBase->bottom >= m_RcScr.bottom)) ||
						((A_PtrRcBase->top >= m_RcScr.top) && (A_PtrRcBase->bottom <= m_RcScr.bottom)) ||
						((A_PtrRcBase->top <= m_RcScr.top) && (A_PtrRcBase->bottom > m_RcScr.top)) ||
						((A_PtrRcBase->top < m_RcScr.bottom) && (A_PtrRcBase->bottom >= m_RcScr.bottom)) ||
						((A_PtrRcBase->top >= m_RcScr.top) && (A_PtrRcBase->top < m_RcScr.bottom)) ||
						((A_PtrRcBase->bottom > m_RcScr.top) && (A_PtrRcBase->bottom <= m_RcScr.bottom)))
					{
						// check direction
						if(E_GrP5DirLeft == A_Dir)
						{
							// left
							if(A_PtrRcBase->left >= m_RcScr.right)
							{
								// check exist privious result
								if(NULL != A_RsltObj)
								{
									// check range
									if(A_RsltObj->ScrRectGet(&Tv_RcPrv))
									{
										if(m_RcScr.right < Tv_RcPrv.right)
										{
											// update new result
											Tv_Result	=	this;
										}
									}
								}
								else
								{
									// update new result
									Tv_Result	=	this;
								}
							}
						}
						else
						{
							// right
							if(A_PtrRcBase->right <= m_RcScr.left)
							{
								// check exist privious result
								if(NULL != A_RsltObj)
								{
									// check range
									if(A_RsltObj->ScrRectGet(&Tv_RcPrv))
									{
										if(m_RcScr.left > Tv_RcPrv.left)
										{
											// update new result
											Tv_Result	=	this;
										}
									}
								}
								else
								{
									// update new result
									Tv_Result	=	this;
								}
							}
						}
					}
				}	//	if((E_GrP5DirLeft == A_Dir) || (E_GrP5DirRight == A_Dir))
				else if((E_GrP5DirUp == A_Dir) || (E_GrP5DirDown == A_Dir))
				{
					// check vertical
					if(((A_PtrRcBase->left <= m_RcScr.left) && (A_PtrRcBase->right >= m_RcScr.right)) ||
						((A_PtrRcBase->left >= m_RcScr.left) && (A_PtrRcBase->right <= m_RcScr.right)) ||
						((A_PtrRcBase->left <= m_RcScr.left) && (A_PtrRcBase->right > m_RcScr.left)) ||
						((A_PtrRcBase->left < m_RcScr.right) && (A_PtrRcBase->right >= m_RcScr.right)) ||
						((A_PtrRcBase->left >= m_RcScr.left) && (A_PtrRcBase->left < m_RcScr.right)) ||
						((A_PtrRcBase->right > m_RcScr.left) && (A_PtrRcBase->right <= m_RcScr.right)))
					{
						// check direction
						if(E_GrP5DirUp == A_Dir)
						{
							// up
							if(A_PtrRcBase->top >= m_RcScr.bottom)
							{
								// check exist privious result
								if(NULL != A_RsltObj)
								{
									// check range
									if(A_RsltObj->ScrRectGet(&Tv_RcPrv))
									{
										if(m_RcScr.bottom < Tv_RcPrv.bottom)
										{
											// update new result
											Tv_Result	=	this;
										}
									}
								}
								else
								{
									// update new result
									Tv_Result	=	this;
								}
							}
						}
						else
						{
							// down
							if(A_PtrRcBase->bottom <= m_RcScr.top)
							{
								// check exist privious result
								if(NULL != A_RsltObj)
								{
									// check range
									if(A_RsltObj->ScrRectGet(&Tv_RcPrv))
									{
										if(m_RcScr.top > Tv_RcPrv.top)
										{
											// update new result
											Tv_Result	=	this;
										}
									}
								}
								else
								{
									// update new result
									Tv_Result	=	this;
								}
							}
						}
					}
				}	//	else if((E_GrP5DirUp == A_Dir) || (E_GrP5DirDown == A_Dir))
			}	// if(IsKeyAble())

			// find slave
			Tv_ObjSlv	=	m_ObjSlave;
			while(NULL != Tv_ObjSlv)
			{
				Tv_Result	=	Tv_ObjSlv->RtlUoFindFarByDir(A_PtrRcBase, A_Dir, A_IsKeyAbleOnly, A_ExcpObjId,  Tv_Result);
				// next
				Tv_ObjSlv	=	Tv_ObjSlv->NextObjGet();
			}

		}//	if(m_ParaUoBase.IsVisible && m_ParaUoBase.IsEnable && (m_ObjId != A_ExcpObjId))

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::LcChkNeedKfcCor(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_ObjFcs;
		BOOL8	Tv_IsNeedCrt;
	// code --------------------
		Tv_IsNeedCrt	=	FALSE;
		if(E_GrP5ObjIdNone != V_GrP5Env.KeyObjId)
		{
			Tv_IsNeedCrt	=	TRUE;
			Tv_ObjFcs	=	(Cls_GrP5UoBase*)GrP5UiObjGet(V_GrP5Env.KeyObjId);
			if(NULL != Tv_ObjFcs)
			{
				if(m_ObjId != Tv_ObjFcs->ObjIdGet())
				{
					Tv_IsNeedCrt	=	Tv_ObjFcs->IsHasParent(m_ObjId);
				}
			}

			if(Tv_IsNeedCrt)
			{
				GrP5KeyFocusCorrectReq();
			}
		}


}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::LcPntReqByRect(Ptr_GrRect A_PtrRect)
{
	// local -------------------
	// code --------------------
		// paint request
		((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->PaintReq(A_PtrRect);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::EvtPosSizeChanged(void)
{

		LcSkinCtlUpdate();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBase::LcSkinCtlUpdate(void)
{
	// local -------------------
		Cls_GrP5SkinBase*	Tv_ObjSkin;
	// code --------------------
		Tv_ObjSkin	=	GrP5SkinGet();
		if(NULL == Tv_ObjSkin)
		{
			return;
		}
		// check parametter chaange
#if 0
		if((Tv_ObjSkin == m_SkinMng) && (m_RcSkinBase.left == m_RcScr.left) && (m_RcSkinBase.top == m_RcScr.top) && 
			(m_RcSkinBase.left == m_RcScr.left) && (m_RcSkinBase.right == m_RcScr.right))
		{
			return;
		}
#endif
		// do update skin
		m_SkinMng			=	Tv_ObjSkin;
		m_RcSkinBase	=	m_RcScr;

		m_SkinMng->CtlUpdateBase(this);

}
//--------------------------------------------------------------------
