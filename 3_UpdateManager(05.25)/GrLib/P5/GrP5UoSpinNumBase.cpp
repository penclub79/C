/*
Platform 5 UI object Button base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoSpinNumBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>
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
#define	E_GrP5UoSpinNumBaseParaInfoCnt							14

St_GrP5DvlScptParaInfo	V_GrP5UoSpinNumBaseParaInfo[E_GrP5UoSpinNumBaseParaInfoCnt]	=
{
	{L"Spin number base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"IsHex", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->IsHex, TRUE, E_GrP5DvlParaInfoActDefault, L"HEX display"},
	{L"IsFixDigit", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->IsFixDigit, TRUE, E_GrP5DvlParaInfoActDefault, L"Fix digit"},
	{L"MaxDigit", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->MaxDigit, TRUE, E_GrP5DvlParaInfoActDefault, L"Max digit count"},
	{L"ValMin", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->ValMin, TRUE, E_GrP5DvlParaInfoActDefault, L"Min value"},
	{L"ValMax", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->ValMax, TRUE, E_GrP5DvlParaInfoActDefault, L"Max value"},
	{L"ValStep", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->ValStep, TRUE, E_GrP5DvlParaInfoActDefault, L"Value stpe"},
	{L"ValAdder", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->ValAdder, TRUE, E_GrP5DvlParaInfoActDefault, L"Value display adder"},
	{L"RplTxtId 0", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->RplTxtId[0], TRUE, E_GrP5DvlParaInfoActDefault, L"Replace text id 0"},
	{L"RplVal 0", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->RplVal[0], TRUE, E_GrP5DvlParaInfoActDefault, L"Replace value 0"},
	{L"RplTxtId 1", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->RplTxtId[1], TRUE, E_GrP5DvlParaInfoActDefault, L"Replace text id 1"},
	{L"RplVal 1", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->RplVal[1], TRUE, E_GrP5DvlParaInfoActDefault, L"Replace value 1"},
	{L"PrefixTxtId", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->PrefixTxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Prefix text id 1"},
	{L"SurfixTxtId", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoSpinNumBasePara)0)->SurfixTxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Surfix text id 1"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoSpinNumBase::Cls_GrP5UoSpinNumBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoSpinBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdSpinNumBase, &m_ParaUoSpinNumBase, sizeof(m_ParaUoSpinNumBase), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoSpinNumBase, sizeof(m_ParaUoSpinNumBase));

		m_ParaUoSpinNumBase.ValMin		=	0;
		m_ParaUoSpinNumBase.ValMax		=	100;
		m_ParaUoSpinNumBase.ValStep		=	1;
		m_ParaUoSpinNumBase.ValAdder	=	0;

		m_Value		=	0;

		m_StrTxt	=	GrP5TxtBufAlloc(m_ObjId);
		if(NULL != m_StrTxt)
		{
			GrStrClear(m_StrTxt);
		}
}
//--------------------------------------------------------------------
Cls_GrP5UoSpinNumBase::~Cls_GrP5UoSpinNumBase(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinNumBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoSpinNumBaseParaInfo, E_GrP5UoSpinNumBaseParaInfoCnt, &m_ParaUoSpinNumBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinNumBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinBase::DvlParaInfoAction(A_Action);

		ValueRead();

		// check value
		if((m_Value + m_ParaUoSpinNumBase.ValAdder) < m_ParaUoSpinNumBase.ValMin)
		{
			m_Value	=	m_ParaUoSpinNumBase.ValMin - m_ParaUoSpinNumBase.ValAdder;
			ValueWrite();
		}

		if((m_Value + m_ParaUoSpinNumBase.ValAdder) > m_ParaUoSpinNumBase.ValMax)
		{
			m_Value	=	m_ParaUoSpinNumBase.ValMax - m_ParaUoSpinNumBase.ValAdder;
			ValueWrite();
		}

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinNumBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		Cls_GrP5UoSpinBase::EvtScptDecDone();

		ValueRead();

		// check value
		if((m_Value + m_ParaUoSpinNumBase.ValAdder) < m_ParaUoSpinNumBase.ValMin)
		{
			m_Value	=	m_ParaUoSpinNumBase.ValMin - m_ParaUoSpinNumBase.ValAdder;
			ValueWrite();
		}

		if((m_Value + m_ParaUoSpinNumBase.ValAdder) > m_ParaUoSpinNumBase.ValMax)
		{
			m_Value	=	m_ParaUoSpinNumBase.ValMax - m_ParaUoSpinNumBase.ValAdder;
			ValueWrite();
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinNumBase::ValueRead(void)
{
	// local -------------------
		__s32	Tv_Value;
		WCHAR*	Tv_StrWk;
		WCHAR*	Tv_StrTmp;
	// code --------------------
		// inherited
		Cls_GrP5UoSpinBase::ValueRead();

		// do make string
		m_SkinCtlSpinBase.StrDisp	=	NULL;
		// check replace value 0
		if((E_GrP5TxtIdNone != m_ParaUoSpinNumBase.RplTxtId[0]) && (m_Value == m_ParaUoSpinNumBase.RplVal[0]))
		{
			m_SkinCtlSpinBase.StrDisp	=	GrP5TextGet(m_ParaUoSpinNumBase.RplTxtId[0]);
		}
		else if((E_GrP5TxtIdNone != m_ParaUoSpinNumBase.RplTxtId[1]) && (m_Value == m_ParaUoSpinNumBase.RplVal[1]))
		{
			m_SkinCtlSpinBase.StrDisp	=	GrP5TextGet(m_ParaUoSpinNumBase.RplTxtId[1]);
		}
		else
		{
			if(NULL != m_StrTxt)
			{
				GrStrClear(m_StrTxt);

				// add prefix
				Tv_StrWk		=	m_StrTxt;
				Tv_StrTmp		=	GrP5TextGet(m_ParaUoSpinNumBase.PrefixTxtId);
				if(NULL != Tv_StrTmp)
				{
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, Tv_StrTmp);
				}

				// add contents
				Tv_Value	=	m_Value + m_ParaUoSpinNumBase.ValAdder;
				if(m_ParaUoSpinNumBase.IsHex)
				{
					GrStrDwordToHexWstr(Tv_StrWk, (__u32)Tv_Value, (__u32)m_ParaUoSpinNumBase.MaxDigit, m_ParaUoSpinNumBase.IsFixDigit);
				}
				else
				{
					GrStrIntToWstr(Tv_StrWk, Tv_Value, (__u32)m_ParaUoSpinNumBase.MaxDigit, m_ParaUoSpinNumBase.IsFixDigit);
				}

				// add surfix
				Tv_StrWk		=	m_StrTxt;
				Tv_StrTmp		=	GrP5TextGet(m_ParaUoSpinNumBase.SurfixTxtId);
				if(NULL != Tv_StrTmp)
				{
					Tv_StrWk	=	GrStrWcat(Tv_StrWk, Tv_StrTmp);
				}
			}

			m_SkinCtlSpinBase.StrDisp	=	m_StrTxt;
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinNumBase::DoIncrease(void)
{
	// local -------------------
		__s32	Tv_Val;
	// code --------------------

		Tv_Val	=	m_Value + m_ParaUoSpinNumBase.ValStep;
		if((Tv_Val + m_ParaUoSpinNumBase.ValAdder) > m_ParaUoSpinNumBase.ValMax)
		{
			if(m_ParaUoSpinBase.IsNotRotate)
			{
				Tv_Val		=	m_ParaUoSpinNumBase.ValMax - m_ParaUoSpinNumBase.ValAdder;
			}
			else
			{
				Tv_Val		=	m_ParaUoSpinNumBase.ValMin - m_ParaUoSpinNumBase.ValAdder;
			}
		}
		m_Value	=	Tv_Val;

		ValueWrite();
		DoEvtSpinValChg();

		ReqDraw(FALSE);

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinNumBase::DoDecrease(void)
{
	// local -------------------
		__s32	Tv_Val;
	// code --------------------
		Tv_Val	=	m_Value - m_ParaUoSpinNumBase.ValStep;
		if((Tv_Val + m_ParaUoSpinNumBase.ValAdder) < m_ParaUoSpinNumBase.ValMin)
		{
			if(m_ParaUoSpinBase.IsNotRotate)
			{
				Tv_Val		=	m_ParaUoSpinNumBase.ValMin - m_ParaUoSpinNumBase.ValAdder;
			}
			else
			{
				Tv_Val		=	m_ParaUoSpinNumBase.ValMax - m_ParaUoSpinNumBase.ValAdder;
			}
		}
		m_Value	=	Tv_Val;

		ValueWrite();
		DoEvtSpinValChg();

		ReqDraw(FALSE);

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinNumBase::ValueMinSet(__s32 A_Value)
{
	// local -------------------
	// code --------------------
		m_ParaUoSpinNumBase.ValMin	=	A_Value;

		ValueRead();

		// check value
		if((m_Value + m_ParaUoSpinNumBase.ValAdder) < m_ParaUoSpinNumBase.ValMin)
		{
			m_Value	=	m_ParaUoSpinNumBase.ValMin - m_ParaUoSpinNumBase.ValAdder;
			ValueWrite();
		}

		if((m_Value + m_ParaUoSpinNumBase.ValAdder) > m_ParaUoSpinNumBase.ValMax)
		{
			m_Value	=	m_ParaUoSpinNumBase.ValMax - m_ParaUoSpinNumBase.ValAdder;
			ValueWrite();
		}

		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinNumBase::ValueMaxSet(__s32 A_Value)
{
	// local -------------------
	// code --------------------
		m_ParaUoSpinNumBase.ValMax	=	A_Value;

		ValueRead();

		// check value
		if((m_Value + m_ParaUoSpinNumBase.ValAdder) < m_ParaUoSpinNumBase.ValMin)
		{
			m_Value	=	m_ParaUoSpinNumBase.ValMin - m_ParaUoSpinNumBase.ValAdder;
			ValueWrite();
		}

		if((m_Value + m_ParaUoSpinNumBase.ValAdder) > m_ParaUoSpinNumBase.ValMax)
		{
			m_Value	=	m_ParaUoSpinNumBase.ValMax - m_ParaUoSpinNumBase.ValAdder;
			ValueWrite();
		}

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
__s32	Cls_GrP5UoSpinNumBase::ValueMinGet(void)
{
	// local -------------------
	// code --------------------
		return	m_ParaUoSpinNumBase.ValMin;
}
//--------------------------------------------------------------------
__s32	Cls_GrP5UoSpinNumBase::ValueMaxGet(void)
{
		return	m_ParaUoSpinNumBase.ValMax;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinNumBase::ValueRelpaceTxtGet(__u32 A_Idx, __s32* A_PtrRtVal, __u16* A_PtrRtTxtId)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(E_GrP5UoSpinNumValRplTxtMax > A_Idx)
		{
			*A_PtrRtVal		=	m_ParaUoSpinNumBase.RplVal[A_Idx];
			*A_PtrRtTxtId	=	m_ParaUoSpinNumBase.RplTxtId[A_Idx];

			Tv_Result			=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinNumBase::ValueRelpaceTxtSet(__u32 A_Idx, __s32 A_Val, __u16 A_TxtId)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(E_GrP5UoSpinNumValRplTxtMax > A_Idx)
		{
			m_ParaUoSpinNumBase.RplVal[A_Idx]		=	A_Val;
			m_ParaUoSpinNumBase.RplTxtId[A_Idx]	=	A_TxtId;

			ReqDraw(FALSE);

			Tv_Result			=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

