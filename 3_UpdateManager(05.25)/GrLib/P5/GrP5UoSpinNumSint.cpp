/*
Platform 5 UI object Button base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoSpinNumSint.h>

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
#define	E_GrP5UoSpinNumSintParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoSpinNumSintParaInfo[E_GrP5UoSpinNumSintParaInfoCnt]	=
{
	{L"Spin number setup integer", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"StpAdr", E_GrP5DvlValTypeSetupAdr, 1, (void*)&((Ptr_GrP5UoSpinNumSintPara)0)->StpAdr, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup address"},
	{L"VarSize", E_GrVarTypeVarSize, 1, (void*)&((Ptr_GrP5UoSpinNumSintPara)0)->VarSize, TRUE, E_GrP5DvlParaInfoActDefault, L"Variable size"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoSpinNumSint::Cls_GrP5UoSpinNumSint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoSpinNumBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdSpinNumSint, &m_ParaUoSpinNumSint, sizeof(m_ParaUoSpinNumSint), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoSpinNumSint, sizeof(m_ParaUoSpinNumSint));


}
//--------------------------------------------------------------------
Cls_GrP5UoSpinNumSint::~Cls_GrP5UoSpinNumSint(void)
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinNumSint::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinNumBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoSpinNumSintParaInfo, E_GrP5UoSpinNumSintParaInfoCnt, &m_ParaUoSpinNumSint);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinNumSint::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinNumBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinNumSint::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		Cls_GrP5UoSpinNumBase::EvtScptDecDone();


}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinNumSint::ValueRead(void)
{
	// local -------------------
		__s32	Tv_Val;
	// code --------------------
		switch(m_ParaUoSpinNumSint.VarSize)
		{
			case E_GrVarSize2byte:
				Tv_Val	=	(__s32)GrP5SetupGetU16(m_ParaUoSpinNumSint.StpAdr);
				break;
			case E_GrVarSize4byte:
				Tv_Val	=	(__s32)GrP5SetupGetU32(m_ParaUoSpinNumSint.StpAdr);
				break;
			default:
				Tv_Val	=	(__s32)GrP5SetupGetU8(m_ParaUoSpinNumSint.StpAdr);
				break;
		}
		// check value range
		if((Tv_Val + m_ParaUoSpinNumBase.ValAdder) < m_ParaUoSpinNumBase.ValMin)
		{
			Tv_Val	=	m_ParaUoSpinNumBase.ValMin - m_ParaUoSpinNumBase.ValAdder;
		}
		if((Tv_Val + m_ParaUoSpinNumBase.ValAdder) > m_ParaUoSpinNumBase.ValMax)
		{
			Tv_Val	=	m_ParaUoSpinNumBase.ValMax - m_ParaUoSpinNumBase.ValAdder;
		}

		m_Value		=	Tv_Val;

		Cls_GrP5UoSpinNumBase::ValueRead();

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinNumSint::ValueWrite(void)
{
	// local -------------------
	// code --------------------
		switch(m_ParaUoSpinNumSint.VarSize)
		{
			case E_GrVarSize2byte:
				GrP5SetupPutU16(m_ParaUoSpinNumSint.StpAdr,(__u16)m_Value);
				break;
			case E_GrVarSize4byte:
				GrP5SetupPutU32(m_ParaUoSpinNumSint.StpAdr,(__u32)m_Value);
				break;
			default:
				GrP5SetupPutU8(m_ParaUoSpinNumSint.StpAdr,(__u8)m_Value);
				break;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinNumSint::SetupAdrSet(__u16 A_Adr)
{
		m_ParaUoSpinNumSint.StpAdr	=	A_Adr;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoSpinNumSint::SetupAdrGet(__u16 A_Adr)
{
		return	m_ParaUoSpinNumSint.StpAdr;
}
//--------------------------------------------------------------------

