/*
Platform 5 UI object spin number local integer

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoSpinNumLint.h>

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

#if 0

// relate position
#define	E_GrP5UoSpinNumLintParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoSpinNumLintParaInfo[E_GrP5UoSpinNumLintParaInfoCnt]	=
{
	{L"Spin number setup integer", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"StpAdr", E_GrP5DvlValTypeSetupAdr, 1, (void*)&((Ptr_GrP5UoSpinNumLintPara)0)->StpAdr, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup address"},
	{L"VarSize", E_GrVarTypeVarSize, 1, (void*)&((Ptr_GrP5UoSpinNumLintPara)0)->VarSize, TRUE, E_GrP5DvlParaInfoActDefault, L"Variable size"},
};

#endif

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoSpinNumLint::Cls_GrP5UoSpinNumLint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoSpinNumBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		//ClassRegist(E_GrP5ClassIdSpinNumLint, &m_ParaUoSpinNumLint, sizeof(m_ParaUoSpinNumLint));
		ClassRegist(E_GrP5ClassIdSpinNumLint, NULL, 0, NULL);
		
		// default setting
		//GrDumyZeroMem(&m_ParaUoSpinNumLint, sizeof(m_ParaUoSpinNumLint));


}
//--------------------------------------------------------------------
Cls_GrP5UoSpinNumLint::~Cls_GrP5UoSpinNumLint(void)
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinNumLint::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinNumBase::DvlParaInfoBuild();

		// add
		//DvlParaInfoAdd(V_GrP5UoSpinNumLintParaInfo, E_GrP5UoSpinNumLintParaInfoCnt, &m_ParaUoSpinNumLint);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinNumLint::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinNumBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinNumLint::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		Cls_GrP5UoSpinNumBase::EvtScptDecDone();


}
//--------------------------------------------------------------------
__s32	Cls_GrP5UoSpinNumLint::ValueGet(void)
{
	// local -------------------
	// code --------------------
		return	m_Value;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinNumLint::ValueSet(__s32 A_Value)
{
	// local -------------------
	// code --------------------
		m_Value	=	A_Value;

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

