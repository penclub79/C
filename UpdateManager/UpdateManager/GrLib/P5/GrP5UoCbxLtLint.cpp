/*
Platform 5 UI object combobox local text local integer

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoCbxLtLint.h>

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

// relate position
#define	E_GrP5UoCbxLtLintParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoCbxLtLintParaInfo[E_GrP5UoCbxLtLintParaInfoCnt]	=
{
	{L"Combo local text setup", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"StpAdr", E_GrP5DvlValTypeSetupAdr, 1, (void*)&((Ptr_GrP5UoCbxLtLintPara)0)->StpAdr, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup address"},
	{L"VarSize", E_GrVarTypeVarSize, 1, (void*)&((Ptr_GrP5UoCbxLtLintPara)0)->VarSize, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup variable size"},

};

#endif

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoCbxLtLint::Cls_GrP5UoCbxLtLint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoCbxLtBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		//ClassRegist(E_GrP5ClassIdCbxLtLint, &m_ParaUoCbxLtLint, sizeof(m_ParaUoCbxLtLint));
		ClassRegist(E_GrP5ClassIdCbxLtLint, NULL, 0, NULL);

		// default setting
		//GrDumyZeroMem(&m_ParaUoCbxLtLint, sizeof(m_ParaUoCbxLtLint));


#ifdef GR_P5_EDITOR

		// add item
		ItemAdd(L"1", 1);
		ItemAdd(L"2", 2);
		ItemAdd(L"3", 3);
		ItemAdd(L"4", 4);
		ItemAdd(L"5", 5);
		ItemAdd(L"6", 6);
		ItemAdd(L"7", 7);
		ItemAdd(L"8", 8);
		ItemAdd(L"9", 9);
		ItemAdd(L"10", 10);

#endif

}
//--------------------------------------------------------------------
Cls_GrP5UoCbxLtLint::~Cls_GrP5UoCbxLtLint(void)
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCbxLtLint::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCbxLtBase::DvlParaInfoBuild();

		// add
		//DvlParaInfoAdd(V_GrP5UoCbxLtLintParaInfo, E_GrP5UoCbxLtLintParaInfoCnt, &m_ParaUoCbxLtLint);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCbxLtLint::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCbxLtBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxLtLint::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoCbxLtBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCbxLtLint::ItemAdd(WCHAR* A_StrTxt, __u32 A_Data)
{
	// local -------------------
	// code --------------------
		if(E_GrP5UoCbxLtMaxItemCnt <= m_ItemCnt)
		{
			return	FALSE;
		}

		// insert
		GrStrWcopy(m_ItemTbl[m_ItemCnt].StrBuf, A_StrTxt, E_GrP5ItemStrBufLen);
		m_ItemTbl[m_ItemCnt].UserData	=	A_Data;
		m_ItemCnt ++;

		// select auto update
		if(0 > m_SelIdx)
		{
			SelectIdxSet((__s32)(m_ItemCnt - 1));
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxLtLint::ItemClear(void)
{
	// local -------------------
	// code --------------------
		if(0 <= m_SelIdx)
		{
			SelectIdxSet(-1);
		}
		m_ItemCnt	=	0;

}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxLtLint::SelectIdxSet(__s32 A_Idx)
{
	// local -------------------
	// code --------------------
		// check index
		if((0 > A_Idx) ||(A_Idx >= (__s32)m_ItemCnt))
		{
			m_SelIdx		=	-1;
			GrStrClear(m_SkinCtlCbxBase.StrDisp);
			ReqDraw(FALSE);
			return;
		}

		m_SelIdx	=	A_Idx;
		GrStrWcopy(m_SkinCtlCbxBase.StrDisp, m_ItemTbl[m_SelIdx].StrBuf);
		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxLtLint::EvtListItemSel(__u16 A_ObjId, __s32 A_ItemIdx)
{
	// local -------------------
	// code --------------------
		SelectIdxSet(A_ItemIdx);

		// inherited
		Cls_GrP5UoCbxLtBase::EvtListItemSel(A_ObjId, A_ItemIdx);
}
//--------------------------------------------------------------------
