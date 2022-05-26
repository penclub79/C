/*
Platform 5 UI window popup base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5WinPopBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5Key.h>
//#include <P5/GrP5UoLi

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

#ifdef GR_P5_EDITOR


#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5WinPopBase::Cls_GrP5WinPopBase(__u16 A_ScptId, __u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect, 
	__u16 A_PrntPopupObjId) :
	Cls_GrP5WinBase(A_ScptId, A_ConIdx, A_LayerIdx, A_PtrRelRect)
{
	// local -------------------
		//St_GrRect	Tv_RcCon;
		//St_GrRect	Tv_RcWin;
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdWinPopBase, NULL, 0, NULL);

		// init
		m_PrntPopObjId	=	A_PrntPopupObjId;
		m_SlvPopObjId		=	E_GrP5ObjIdNone;

}
//--------------------------------------------------------------------
Cls_GrP5WinPopBase::~Cls_GrP5WinPopBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5WinPopBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5WinBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinPopBase::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5WinBase::DvlParaInfoBuild();
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinPopBase::DvlParaInfoAction(__u32 A_Action)
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
void	Cls_GrP5WinPopBase::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// not delete

		// delete prepare

		// inherited
		//Cls_GrP5WinBase::DvlPreDelete();

}
#endif
//--------------------------------------------------------------------
__u16	Cls_GrP5WinPopBase::ParentPopupIdGet(void)
{
		return	m_PrntPopObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5WinPopBase::SlavePopupIdGet(void)
{
		return	m_SlvPopObjId;
}
//--------------------------------------------------------------------
