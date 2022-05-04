/*
Platform 5 UI object Button base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoCbxLtBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <P5/GrP5WinPopList.h>
#include <P5/GrP5Key.h>

#ifdef GR_P5_EDITOR

#include <P5\Dvl\GrP5DvlBase.h>

#endif

#ifdef LINUX_APP

#include <stdlib.h>

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

#if 0

#define	E_GrP5UoCbxLtBaseParaInfoCnt							2

St_GrP5DvlScptParaInfo	V_GrP5UoCbxLtBaseParaInfo[E_GrP5UoCbxLtBaseParaInfoCnt]	=
{
	{L"Combo local text base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"ItemMaxCnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoCbxLtBasePara)0)->ItemMaxCnt, TRUE, E_GrP5DvlParaInfoActDefault, L"Item max count"},

};

#endif

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoCbxLtBase::Cls_GrP5UoCbxLtBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoCbxBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		//ClassRegist(E_GrP5ClassIdCbxLtBase, &m_ParaUoCbxLtBase, sizeof(m_ParaUoCbxLtBase));
		ClassRegist(E_GrP5ClassIdCbxLtBase, NULL, 0, NULL);
		
		// default setting
		//GrDumyZeroMem(&m_ParaUoCbxLtBase, sizeof(m_ParaUoCbxLtBase));

		GrDumyZeroMem(m_ItemTbl, sizeof(m_ItemTbl));
		m_ItemCnt		=	0;

		// display text buffer
		m_SkinCtlCbxBase.StrDisp	=	GrP5TxtBufAlloc(m_ObjId);

		GrStrClear(m_SkinCtlCbxBase.StrDisp);

}
//--------------------------------------------------------------------
Cls_GrP5UoCbxLtBase::~Cls_GrP5UoCbxLtBase(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCbxLtBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCbxBase::DvlParaInfoBuild();

		// add
		//DvlParaInfoAdd(V_GrP5UoCbxLtBaseParaInfo, E_GrP5UoCbxLtBaseParaInfoCnt, &m_ParaUoCbxLtBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCbxLtBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCbxBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxLtBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCbxBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP5UoCbxLtBase::ItemTextGet(__u32 A_ItemIdx)
{
	// local -------------------
	// code --------------------
		if(A_ItemIdx >= m_ItemCnt)
		{
			return	NULL;
		}

		return	m_ItemTbl[A_ItemIdx].StrBuf;
}
//--------------------------------------------------------------------
__u32		Cls_GrP5UoCbxLtBase::ItemUserDataGet(__u32 A_ItemIdx)
{
	// local -------------------
	// code --------------------
		if(A_ItemIdx >= m_ItemCnt)
		{
			return	0;
		}

		return	m_ItemTbl[A_ItemIdx].UserData;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxLtBase::PopupItemBuild(void)
{
	// local -------------------
		__u32	Tv_ItmIdx;
		__u32	Tv_Stat;
	// code --------------------
		GrP5ItemReset();

		if(0 == m_ItemCnt)
		{
			return;
		}

		for(Tv_ItmIdx = 0; Tv_ItmIdx < m_ItemCnt; Tv_ItmIdx++)
		{
			Tv_Stat		=	E_GrP5StatNormal;
			if(m_SelIdx == (__s32)Tv_ItmIdx)
			{
				Tv_Stat		=	E_GrP5StatOnSelected;
			}
			GrP5ItemAdd(E_GrP5TxtIdNone, Tv_Stat, 0, 1, 0, m_ItemTbl[Tv_ItmIdx].UserData, m_ItemTbl[Tv_ItmIdx].StrBuf);
		}

}
//--------------------------------------------------------------------

