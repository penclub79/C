/*
Platform 5 UI object Button base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoCbxLtSint.h>

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
#define	E_GrP5UoCbxLtSintParaInfoCnt							4

St_GrP5DvlScptParaInfo	V_GrP5UoCbxLtSintParaInfo[E_GrP5UoCbxLtSintParaInfoCnt]	=
{
	{L"Combo local text setup", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"StpAdr", E_GrP5DvlValTypeSetupAdr, 1, (void*)&((Ptr_GrP5UoCbxLtSintPara)0)->StpAdr, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup address"},
	{L"VarSize", E_GrVarTypeVarSize, 1, (void*)&((Ptr_GrP5UoCbxLtSintPara)0)->VarSize, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup variable size"},
	{L"TxtIdOutVal", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoCbxLtSintPara)0)->TxtIdOutVal, TRUE, E_GrP5DvlParaInfoActDefault, L"Text id out of value"},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoCbxLtSint::Cls_GrP5UoCbxLtSint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoCbxLtBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdCbxLtSint, &m_ParaUoCbxLtSint, sizeof(m_ParaUoCbxLtSint), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoCbxLtSint, sizeof(m_ParaUoCbxLtSint));

}
//--------------------------------------------------------------------
Cls_GrP5UoCbxLtSint::~Cls_GrP5UoCbxLtSint(void)
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCbxLtSint::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCbxLtBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoCbxLtSintParaInfo, E_GrP5UoCbxLtSintParaInfoCnt, &m_ParaUoCbxLtSint);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCbxLtSint::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCbxLtBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxLtSint::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------

		// update value
		LcSelIdxUpdtByVal();

		// inherited
		Cls_GrP5UoCbxLtBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxLtSint::EvtListItemSel(__u16 A_ObjId, __s32 A_ItemIdx)
{
	// local -------------------
	// code --------------------
		// update value
		if(0 <= A_ItemIdx)
		{
			switch(m_ParaUoCbxLtSint.VarSize)
			{
				case E_GrVarSize2byte:
					GrP5SetupPutU16(m_ParaUoCbxLtSint.StpAdr, (__u16)m_ItemTbl[A_ItemIdx].UserData);
					break;
				case E_GrVarSize4byte:
					GrP5SetupPutU32(m_ParaUoCbxLtSint.StpAdr, m_ItemTbl[A_ItemIdx].UserData);
					break;
				default:
					GrP5SetupPutU8(m_ParaUoCbxLtSint.StpAdr, (__u8)m_ItemTbl[A_ItemIdx].UserData);
					break;
			}
		}

		LcSelIdxUpdtByVal();
		ReqDraw(FALSE);

		// inherited
		Cls_GrP5UoCbxLtBase::EvtListItemSel(A_ObjId, A_ItemIdx);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxLtSint::SetupAdrSet(__u16 A_Adr)
{
		m_ParaUoCbxLtSint.StpAdr	=	A_Adr;
		LcSelIdxUpdtByVal();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoCbxLtSint::SetupAdrGet(__u16 A_Adr)
{
		return	m_ParaUoCbxLtSint.StpAdr;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCbxLtSint::ItemAdd(WCHAR* A_StrTxt, __u32 A_Data)
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
		LcSelIdxUpdtByVal();
		ReqDraw(FALSE);

		return	TRUE;

}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoCbxLtSint::LcValueGet(void)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result		=	0;

		switch(m_ParaUoCbxLtSint.VarSize)
		{
			case E_GrVarSize2byte:
				Tv_Result	=	(__u32)GrP5SetupGetU16(m_ParaUoCbxLtSint.StpAdr);
				break;
			case E_GrVarSize4byte:
				Tv_Result	=	GrP5SetupGetU32(m_ParaUoCbxLtSint.StpAdr);
				break;
			default:
				Tv_Result	=	(__u32)GrP5SetupGetU8(m_ParaUoCbxLtSint.StpAdr);
				break;
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxLtSint::LcSelIdxUpdtByVal(void)
{
	// local -------------------
		__u32	Tv_Val;
		__u32	Tv_ItmIdx;
		WCHAR*	Tv_Str;
	// code --------------------
		Tv_Val			=	LcValueGet();

		m_SelIdx		=	-1;

		if(0 != m_ItemCnt)
		{
			for(Tv_ItmIdx = 0; Tv_ItmIdx < m_ItemCnt; Tv_ItmIdx++)
			{
				if(Tv_Val == m_ItemTbl[Tv_ItmIdx].UserData)
				{
					m_SelIdx	=	(__s32)Tv_ItmIdx;
					break;
				}
			}
		}

		// update value
		Tv_Str	=	NULL;
		if(0 > m_SelIdx)
		{
			Tv_Str		=	GrP5TextGet(m_ParaUoCbxLtSint.TxtIdOutVal);
		}
		else
		{
			Tv_Str		=	m_ItemTbl[m_SelIdx].StrBuf;
		}

		GrStrWcopy(m_SkinCtlCbxBase.StrDisp,Tv_Str);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxLtSint::ItemClear(void)
{
	// local -------------------
	// code --------------------
		m_ItemCnt		=	0;
		LcSelIdxUpdtByVal();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
