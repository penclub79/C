/*
Platform 5 UI object Spin setup text base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoSpinLtLint.h>

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
#define	E_GrP5UoSpinLtLintParaInfoCnt							10

St_GrP5DvlScptParaInfo	V_GrP5UoSpinLtLintParaInfo[E_GrP5UoSpinLtLintParaInfoCnt]	=
{
	{L"Spin Setup Text Base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"StpAdr", E_GrP5DvlValTypeSetupAdr, 1, (void*)&((Ptr_GrP5UoSpinLtLintPara)0)->StpAdr, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup address"},
	{L"Len", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoSpinLtLintPara)0)->Len, TRUE, E_GrP5DvlParaInfoActDefault, L"String Length"},
	{L"Cnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoSpinLtLintPara)0)->Cnt, TRUE, E_GrP5DvlParaInfoActDefault, L"String Count"},
	{L"IsEdit", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoSpinLtLintPara)0)->IsEdit, TRUE, E_GrP5DvlParaInfoActDefault, L"String edit able"},
	{L"IsCalc", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoSpinLtLintPara)0)->IsCalc, TRUE, E_GrP5DvlParaInfoActDefault, L"Calculate address"},
	{L"IsZeroText", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoSpinLtLintPara)0)->IsZeroText, TRUE, E_GrP5DvlParaInfoActDefault, L"Add zero index text"},
	{L"MulVal", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoSpinLtLintPara)0)->MulVal, TRUE, E_GrP5DvlParaInfoActDefault, L"Multiple value"},
	{L"AddVal", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoSpinLtLintPara)0)->AddVal, TRUE, E_GrP5DvlParaInfoActDefault, L"Add value"},
	{L"TextIdZero", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoSpinLtLintPara)0)->TextIdZero, TRUE, E_GrP5DvlParaInfoActDefault, L"ID of zero index text"},
};
#endif

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoSpinLtLint::Cls_GrP5UoSpinLtLint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoSpinLtBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		//ClassRegist(E_GrP5ClassIdSpinLtLint, &m_ParaUoSpinLtLint, sizeof(m_ParaUoSpinLtLint));
		ClassRegist(E_GrP5ClassIdSpinLtLint, NULL, 0, NULL);
		
		// default setting
		//GrDumyZeroMem(&m_ParaUoSpinLtLint, sizeof(m_ParaUoSpinLtLint));

}
//--------------------------------------------------------------------
Cls_GrP5UoSpinLtLint::~Cls_GrP5UoSpinLtLint(void)
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinLtLint::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinBase::DvlParaInfoBuild();

		// add
		//DvlParaInfoAdd(V_GrP5UoSpinLtLintParaInfo, E_GrP5UoSpinLtLintParaInfoCnt, &m_ParaUoSpinLtLint);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinLtLint::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinLtLint::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		Cls_GrP5UoSpinBase::EvtScptDecDone();


}
//--------------------------------------------------------------------

