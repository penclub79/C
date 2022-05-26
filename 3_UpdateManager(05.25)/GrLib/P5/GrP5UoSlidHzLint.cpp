/*
Platform 5 UI scroll bar standard

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoSlidHzLint.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>

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

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoSlidHzLint::Cls_GrP5UoSlidHzLint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect ) :
	Cls_GrP5UoSlidHzBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdSlidHzLint, NULL, 0, NULL);
		

		// default setting
		//GrDumyZeroMem(&m_ParaUoSlidHzLint, sizeof(m_ParaUoSlidHzLint));

}
//--------------------------------------------------------------------
Cls_GrP5UoSlidHzLint::~Cls_GrP5UoSlidHzLint(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSlidHzLint::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSlidHzBase::DvlParaInfoBuild();

		// add
		//DvlParaInfoAdd(V_GrP5UoSlidHzLintParaInfo, E_GrP5UoSlidHzLintParaInfoCnt, &m_ParaUoSlidHzLint);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSlidHzLint::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------
		// style rebuild
		//BtnStatStyleBuild();

		// inherited
		Cls_GrP5UoSlidHzBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidHzLint::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoSlidHzBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
