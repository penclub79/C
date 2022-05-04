/*
Platform 5 UI object Button base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoCbxTiSint.h>

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
#define	E_GrP5UoCbxTiSintParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoCbxTiSintParaInfo[E_GrP5UoCbxTiSintParaInfoCnt]	=
{
	{L"Combo text id setup", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"StpAdr", E_GrP5DvlValTypeSetupAdr, 1, (void*)&((Ptr_GrP5UoCbxTiSintPara)0)->StpAdr, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup address"},
	{L"VarSize", E_GrVarTypeVarSize, 1, (void*)&((Ptr_GrP5UoCbxTiSintPara)0)->VarSize, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup variable size"},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoCbxTiSint::Cls_GrP5UoCbxTiSint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoCbxTiBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdCbxTiSint, &m_ParaUoCbxTiSint, sizeof(m_ParaUoCbxTiSint), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoCbxTiSint, sizeof(m_ParaUoCbxTiSint));

}
//--------------------------------------------------------------------
Cls_GrP5UoCbxTiSint::~Cls_GrP5UoCbxTiSint(void)
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCbxTiSint::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCbxTiBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoCbxTiSintParaInfo, E_GrP5UoCbxTiSintParaInfoCnt, &m_ParaUoCbxTiSint);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCbxTiSint::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCbxTiBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxTiSint::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------

		// update value
		LcSelIdxUpdtByVal();

		// inherited
		Cls_GrP5UoCbxTiBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxTiSint::EvtListItemSel(__u16 A_ObjId, __s32 A_ItemIdx)
{
	// local -------------------
	// code --------------------
		// update value
		if(0 <= A_ItemIdx)
		{
			switch(m_ParaUoCbxTiSint.VarSize)
			{
				case E_GrVarSize2byte:
					GrP5SetupPutU16(m_ParaUoCbxTiSint.StpAdr, (__u16)m_ParaUoCbxTiBase.ItemInfo[A_ItemIdx].UserData);
					break;
				case E_GrVarSize4byte:
					GrP5SetupPutU32(m_ParaUoCbxTiSint.StpAdr, m_ParaUoCbxTiBase.ItemInfo[A_ItemIdx].UserData);
					break;
				default:
					GrP5SetupPutU8(m_ParaUoCbxTiSint.StpAdr, (__u8)m_ParaUoCbxTiBase.ItemInfo[A_ItemIdx].UserData);
					break;
			}
		}

		LcSelIdxUpdtByVal();
		ReqDraw(FALSE);

		// inherited
		Cls_GrP5UoCbxTiBase::EvtListItemSel(A_ObjId, A_ItemIdx);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxTiSint::SetupAdrSet(__u16 A_Adr)
{
		m_ParaUoCbxTiSint.StpAdr	=	A_Adr;
		LcSelIdxUpdtByVal();
		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoCbxTiSint::SetupAdrGet(__u16 A_Adr)
{
		return	m_ParaUoCbxTiSint.StpAdr;
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoCbxTiSint::LcValueGet(void)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result		=	0;

		switch(m_ParaUoCbxTiSint.VarSize)
		{
			case E_GrVarSize2byte:
				Tv_Result	=	(__u32)GrP5SetupGetU16(m_ParaUoCbxTiSint.StpAdr);
				break;
			case E_GrVarSize4byte:
				Tv_Result	=	GrP5SetupGetU32(m_ParaUoCbxTiSint.StpAdr);
				break;
			default:
				Tv_Result	=	(__u32)GrP5SetupGetU8(m_ParaUoCbxTiSint.StpAdr);
				break;
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxTiSint::LcSelIdxUpdtByVal(void)
{
	// local -------------------
		__u32	Tv_Val;
		__u32	Tv_ItmIdx;
		WCHAR*	Tv_Str;
		__u32	Tv_ItmCnt;
	// code --------------------
		Tv_Val			=	LcValueGet();

		m_SelIdx		=	-1;

		Tv_ItmCnt		=	(__u32)m_ParaUoCbxTiBase.ItemCnt;
		if(E_GrP5UoCbxTiMaxItemCnt <= Tv_ItmCnt)
		{
			Tv_ItmCnt		=	E_GrP5UoCbxTiMaxItemCnt;
		}
		if(0 != Tv_ItmCnt)
		{
			for(Tv_ItmIdx = 0; Tv_ItmIdx < Tv_ItmCnt; Tv_ItmIdx++)
			{
				if(Tv_Val == m_ParaUoCbxTiBase.ItemInfo[Tv_ItmIdx].UserData)
				{
					m_SelIdx	=	(__s32)Tv_ItmIdx;
					break;
				}
			}
		}

		// update value
		Tv_Str	=	NULL;
		if(0 <= m_SelIdx)
		{
			Tv_Str		=	GrP5TextGet(m_ParaUoCbxTiBase.ItemInfo[Tv_ItmIdx].TxtId);
		}

		m_SkinCtlCbxBase.StrDisp	=	Tv_Str;

}
//--------------------------------------------------------------------

