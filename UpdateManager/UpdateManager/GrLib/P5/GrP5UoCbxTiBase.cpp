/*
Platform 5 UI object Button base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoCbxTiBase.h>

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

// relate position
#define	E_GrP5UoCbxTiBaseParaInfoCnt							66

St_GrP5DvlScptParaInfo	V_GrP5UoCbxTiBaseParaInfo[E_GrP5UoCbxTiBaseParaInfoCnt]	=
{
	{L"Combo text id base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"ItemCnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemCnt, TRUE, E_GrP5DvlParaInfoActDefault, L"Item count"},
	{L"TxtId[0]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[0].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[0]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[0].UserData, TRUE, 0, L"User data"},
	{L"TxtId[1]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[1].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[1]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[1].UserData, TRUE, 0, L"User data"},
	{L"TxtId[2]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[2].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[2]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[2].UserData, TRUE, 0, L"User data"},
	{L"TxtId[3]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[3].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[3]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[3].UserData, TRUE, 0, L"User data"},
	{L"TxtId[4]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[4].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[4]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[4].UserData, TRUE, 0, L"User data"},
	{L"TxtId[5]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[5].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[5]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[5].UserData, TRUE, 0, L"User data"},
	{L"TxtId[6]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[6].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[6]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[6].UserData, TRUE, 0, L"User data"},
	{L"TxtId[7]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[7].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[7]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[7].UserData, TRUE, 0, L"User data"},
	{L"TxtId[8]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[8].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[8]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[8].UserData, TRUE, 0, L"User data"},
	{L"TxtId[9]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[9].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[9]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[9].UserData, TRUE, 0, L"User data"},
	{L"TxtId[10]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[10].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[10]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[10].UserData, TRUE, 0, L"User data"},
	{L"TxtId[11]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[11].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[11]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[11].UserData, TRUE, 0, L"User data"},
	{L"TxtId[12]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[12].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[12]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[12].UserData, TRUE, 0, L"User data"},
	{L"TxtId[13]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[13].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[13]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[13].UserData, TRUE, 0, L"User data"},
	{L"TxtId[14]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[14].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[14]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[14].UserData, TRUE, 0, L"User data"},
	{L"TxtId[15]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[15].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[15]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[15].UserData, TRUE, 0, L"User data"},
	{L"TxtId[16]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[16].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[16]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[16].UserData, TRUE, 0, L"User data"},
	{L"TxtId[17]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[17].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[17]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[17].UserData, TRUE, 0, L"User data"},
	{L"TxtId[18]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[18].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[18]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[18].UserData, TRUE, 0, L"User data"},
	{L"TxtId[19]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[19].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[19]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[19].UserData, TRUE, 0, L"User data"},
	{L"TxtId[20]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[20].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[20]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[20].UserData, TRUE, 0, L"User data"},
	{L"TxtId[21]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[21].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[21]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[21].UserData, TRUE, 0, L"User data"},
	{L"TxtId[22]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[22].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[22]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[22].UserData, TRUE, 0, L"User data"},
	{L"TxtId[23]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[23].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[23]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[23].UserData, TRUE, 0, L"User data"},
	{L"TxtId[24]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[24].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[24]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[24].UserData, TRUE, 0, L"User data"},
	{L"TxtId[25]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[25].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[25]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[25].UserData, TRUE, 0, L"User data"},
	{L"TxtId[26]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[26].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[26]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[26].UserData, TRUE, 0, L"User data"},
	{L"TxtId[27]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[27].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[27]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[27].UserData, TRUE, 0, L"User data"},
	{L"TxtId[28]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[28].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[28]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[28].UserData, TRUE, 0, L"User data"},
	{L"TxtId[29]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[29].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[29]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[29].UserData, TRUE, 0, L"User data"},
	{L"TxtId[30]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[30].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[30]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[30].UserData, TRUE, 0, L"User data"},
	{L"TxtId[31]", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[31].TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id"},
	{L"UserData[31]", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoCbxTiBasePara)0)->ItemInfo[31].UserData, TRUE, 0, L"User data"},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoCbxTiBase::Cls_GrP5UoCbxTiBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoCbxBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdCbxTiBase, &m_ParaUoCbxTiBase, sizeof(m_ParaUoCbxTiBase), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoCbxTiBase, sizeof(m_ParaUoCbxTiBase));

}
//--------------------------------------------------------------------
Cls_GrP5UoCbxTiBase::~Cls_GrP5UoCbxTiBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCbxTiBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCbxBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoCbxTiBaseParaInfo, E_GrP5UoCbxTiBaseParaInfoCnt, &m_ParaUoCbxTiBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCbxTiBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCbxBase::DvlParaInfoAction(A_Action);

		//CbxItemCntUpdt();
		//CbxItemContUpdt();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxTiBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCbxBase::EvtScptDecDone();

		// update

}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxTiBase::PopupItemBuild(void)
{
	// local -------------------
		__u32	Tv_ItmIdx;
		__u32	Tv_Stat;
		__u32	Tv_ItmCnt;
		WCHAR*	Tv_Str;
	// code --------------------
		Tv_ItmCnt		=	(__u32)m_ParaUoCbxTiBase.ItemCnt;
		if(E_GrP5UoCbxTiMaxItemCnt <= Tv_ItmCnt)
		{
			Tv_ItmCnt		=	E_GrP5UoCbxTiMaxItemCnt;
		}

		GrP5ItemReset();

		if(0 == Tv_ItmCnt)
		{
			return;
		}

		for(Tv_ItmIdx = 0; Tv_ItmIdx < Tv_ItmCnt; Tv_ItmIdx++)
		{
			Tv_Stat		=	E_GrP5StatNormal;
			if(m_SelIdx == (__s32)Tv_ItmIdx)
			{
				Tv_Stat		=	E_GrP5StatOnSelected;
			}

			Tv_Str		=	GrP5TextGet(m_ParaUoCbxTiBase.ItemInfo[Tv_ItmIdx].TxtId);
			GrP5ItemAdd(E_GrP5TxtIdNone, Tv_Stat, 0, 1, 0, m_ParaUoCbxTiBase.ItemInfo[Tv_ItmIdx].UserData, Tv_Str);
		}

}
//--------------------------------------------------------------------

