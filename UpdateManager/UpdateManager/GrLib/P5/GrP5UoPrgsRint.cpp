/*
Platform 5 UI object Label base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoPrgsRint.h>

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


#ifdef GR_P5_EDITOR

#define	E_GrP5UoPrgsRintParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoPrgsRintParaInfo[E_GrP5UoPrgsRintParaInfoCnt]	=
{
	{ L"Progress Reg Int", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL },
	{L"RintMax", E_GrP5DvlValTypeRintId, 1, (void*)&((Ptr_GrP5UoPrgsRintPara)0)->RintMax, TRUE, 0, L"Register id of max value"},
	{L"RintNow", E_GrP5DvlValTypeRintId, 1, (void*)&((Ptr_GrP5UoPrgsRintPara)0)->RintNow, TRUE, 0, L"Register id of now value"},
};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoPrgsRint::Cls_GrP5UoPrgsRint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoPrgsBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdPrgsRint, &m_ParaUoPrgsRint, sizeof(m_ParaUoPrgsRint), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoPrgsRint, sizeof(m_ParaUoPrgsRint));

		// reset

		// register timer
}
//--------------------------------------------------------------------
Cls_GrP5UoPrgsRint::~Cls_GrP5UoPrgsRint(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPrgsRint::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoPrgsBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPrgsRint::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoPrgsBase::DvlParaInfoBuild();

		DvlParaInfoAdd(V_GrP5UoPrgsRintParaInfo, E_GrP5UoPrgsRintParaInfoCnt, &m_ParaUoPrgsRint);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPrgsRint::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoPrgsBase::DvlParaInfoAction(A_Action);

		// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPrgsRint::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoPrgsBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoPrgsRint::PrgsValGet(__u32* A_PtrRtMax, __u32* A_PtrRtNow)
{
		*A_PtrRtMax	=	GrP5RintGet(m_ParaUoPrgsRint.RintMax);
		*A_PtrRtNow	=	GrP5RintGet(m_ParaUoPrgsRint.RintNow);
}
//--------------------------------------------------------------------
