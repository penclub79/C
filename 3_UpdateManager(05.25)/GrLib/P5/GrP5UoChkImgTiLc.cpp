/*
Platform 5 UI object checkbox text id local value

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoChkImgTiLc.h>

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
#define	E_GrP5UoChkImgTiLcParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoChkImgTiLcParaInfo[E_GrP5UoChkImgTiLcParaInfoCnt]	=
{
	{L"Button text id", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"TxtId", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoChkImgTiLcPara)0)->TxtId, TRUE, E_GrP5DvlParaInfoActDefault, L"Text ID"},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoChkImgTiLcPara)0)->FontId, TRUE, E_GrP5DvlParaInfoActDefault, L"Font ID"},
};

#endif

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoChkImgTiLc::Cls_GrP5UoChkImgTiLc(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoChkImgTiBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		//ClassRegist(E_GrP5ClassIdChkImgTiLc, &m_ParaUoChkImgTiLc, sizeof(m_ParaUoChkImgTiLc));
		ClassRegist(E_GrP5ClassIdChkImgTiLc, NULL, 0, NULL);
		
}
//--------------------------------------------------------------------
Cls_GrP5UoChkImgTiLc::~Cls_GrP5UoChkImgTiLc(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoChkImgTiLc::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoChkImgTiBase::DvlParaInfoBuild();

		// add
#if 0
		DvlParaInfoAdd(V_GrP5UoChkImgTiLcParaInfo, E_GrP5UoChkImgTiLcParaInfoCnt, &m_ParaUoChkImgTiLc);
		DvlParaInfoTxtParaAdd(L"Button text para", &m_ParaUoChkImgTiLc.TxtPara);
		DvlParaInfoRelPosAdd(L"Button text rect", &m_ParaUoChkImgTiLc.TxtRc);
#endif
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoChkImgTiLc::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoChkImgTiBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoChkImgTiLc::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoChkImgTiBase::EvtScptDecDone();

}
//--------------------------------------------------------------------

