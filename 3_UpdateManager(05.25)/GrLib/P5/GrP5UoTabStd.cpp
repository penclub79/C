/*
Platform 5 UI TAB standard

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoTabStd.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5Mng.h>

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

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoTabStd::Cls_GrP5UoTabStd(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoTabBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdTabStd, NULL, 0, NULL);
		

}
//--------------------------------------------------------------------
Cls_GrP5UoTabStd::~Cls_GrP5UoTabStd(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5UoTabStd::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoTabBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
