/*
Platform 5 window child

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5WinChild.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>

#ifdef GR_P5_EDITOR

#include <P5/Dvl/GrP5DvlBase.h>

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
Cls_GrP5WinChild::Cls_GrP5WinChild(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5WinBase(A_ScptId, 0,0, A_PtrRelRect,A_ObjPrnt)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdWinChild, NULL, 0, NULL);

		// default setting
		//GrDumyZeroMem(&m_ParaWinChild, sizeof(m_ParaWinChild));

}
//--------------------------------------------------------------------
Cls_GrP5WinChild::~Cls_GrP5WinChild(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinChild::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

	// inherited
	Cls_GrP5WinBase::DvlParaInfoAction(A_Action);

	// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinChild::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------

	// delete prepare


	// inherited
	Cls_GrP5WinBase::DvlPreDelete();

}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinChild::CloseRequest(void)
{
		DbgMsgPrint("Cls_GrP5WinChild::CloseRequest - child window can not close\n");
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP5WinChild::EvtPaintProc(void)
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
