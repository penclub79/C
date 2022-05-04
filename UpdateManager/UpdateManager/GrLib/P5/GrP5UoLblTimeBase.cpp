/*
Platform 5 UI object Label Text Id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoLblTimeBase.h>

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

WCHAR	V_Cls_GrP5UoLblTimeBaseStrParaSepLabelBase[]			=	L"Label by Local text";

#define	E_Cls_GrP5UoLblTimeBaseParaInfoCnt							5

St_GrP5DvlScptParaInfo	V_Cls_GrP5UoLblTimeBaseParaInfo[E_Cls_GrP5UoLblTimeBaseParaInfoCnt]	=
{
	{L"Label time base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"GapDateTime", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoLblTimeBasePara)0)->GapDateTime, TRUE, 0, L"Gap of time with date"},
	{L"IsNoTime", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoLblTimeBasePara)0)->IsNoTime, TRUE, 0, L"No time display"},
	{L"IsNoDate", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoLblTimeBasePara)0)->IsNoDate, TRUE, 0, L"No date display"},
	{L"IsNoSec", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoLblTimeBasePara)0)->IsNoSec, TRUE, 0, L"No second display"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoLblTimeBase::Cls_GrP5UoLblTimeBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoLblBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdLblTimeBase, &m_ParaUoLblTimeBase, sizeof(m_ParaUoLblTimeBase), NULL);
		//ClassRegist(E_GrP5ClassIdLblTimeBase, NULL, 0);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoLblTimeBase, sizeof(m_ParaUoLblTimeBase));
		m_ParaUoLblTimeBase.GapDateTime	=	4;

		m_IsApmShow	=	FALSE;

		m_SkinCtlLblBase.Str	=	GrP5TxtBufAlloc(m_ObjId);
		if(NULL != m_SkinCtlLblBase.Str)
		{
			GrStrClear(m_SkinCtlLblBase.Str);
		}
}
//--------------------------------------------------------------------
Cls_GrP5UoLblTimeBase::~Cls_GrP5UoLblTimeBase(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblTimeBase::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoLblBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblTimeBase::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoLblBase::DvlParaInfoBuild();

		// separator basic info
		DvlParaInfoAdd(V_Cls_GrP5UoLblTimeBaseParaInfo, E_Cls_GrP5UoLblTimeBaseParaInfoCnt, &m_ParaUoLblTimeBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblTimeBase::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoLblBase::DvlParaInfoAction(A_Action);

		// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblTimeBase::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
	// delete prepare


	// inherited
	Cls_GrP5UoLblBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoLblTimeBase::IsApmShow(void)
{
		return	m_IsApmShow;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblTimeBase::ApmShowSet(BOOL8 A_IsShow)
{
	// local -------------------
	// code --------------------
		m_IsApmShow		=	A_IsShow;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------

