/*
Platform 5 UI object Button text id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoChkImgLtLc.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5ConBase.h>

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
#define	E_GrP5UoChkImgLtLcInfoCnt							1

St_GrP5DvlScptParaInfo	V_GrP5UoChkImgLtLcInfo[E_GrP5UoChkImgLtLcInfoCnt]	=
{
	{L"Check text id base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
};
#endif

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoChkImgLtLc::Cls_GrP5UoChkImgLtLc(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoChkImgLtBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		//ClassRegist(E_GrP5ClassIdChkImgLtLc, &m_ParaUoChkImgLtLc, sizeof(m_ParaUoChkImgLtLc));
		ClassRegist(E_GrP5ClassIdChkImgLtLc, NULL, 0, NULL);
		
		// default setting
		//GrDumyZeroMem(&m_ParaUoChkImgLtLc, sizeof(m_ParaUoChkImgLtLc));

}
//--------------------------------------------------------------------
Cls_GrP5UoChkImgLtLc::~Cls_GrP5UoChkImgLtLc(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoChkImgLtLc::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoChkImgLtBase::DvlParaInfoBuild();

		// add
#if 0
		DvlParaInfoAdd(V_GrP5UoChkImgLtLcInfo, E_GrP5UoChkImgLtLcInfoCnt, &m_ParaUoChkImgLtLc);
		DvlParaInfoRelPosAdd(L"Check mark rect", &m_ParaUoChkImgLtLc.MarkRc);
#endif

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoChkImgLtLc::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoChkImgLtBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoChkImgLtLc::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoChkImgLtBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
