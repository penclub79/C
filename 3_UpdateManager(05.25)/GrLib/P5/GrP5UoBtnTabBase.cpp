/*
Platform 5 UI object Button standard

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoBtnTabBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>

#include <P5/GrP5UoTabBase.h>
#include <P5/GrP5WinBase.h>

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

// relate position
#define	E_GrP5UoBtnTabBaseParaInfoCnt							4

St_GrP5DvlScptParaInfo	V_GrP5UoBtnTabBaseParaInfo[E_GrP5UoBtnTabBaseParaInfoCnt]	=
{
	{L"Tab button base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"TabScptId", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnTabBasePara)0)->TabScptId, TRUE, 0, L"TAB script ID, if 0 then instead parent tab control"},
	{L"PageScptId", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoBtnTabBasePara)0)->PageScptId, TRUE, 0, L"Page script ID"},
	{L"IsPush", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBtnTabBasePara)0)->IsPush, TRUE, 0, L"Page push"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoBtnTabBase::Cls_GrP5UoBtnTabBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBtnBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdBtnTabBase, &m_ParaUoBtnTabBase, sizeof(m_ParaUoBtnTabBase), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoBtnTabBase, sizeof(m_ParaUoBtnTabBase));

}
//--------------------------------------------------------------------
Cls_GrP5UoBtnTabBase::~Cls_GrP5UoBtnTabBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnTabBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoBtnTabBaseParaInfo, E_GrP5UoBtnTabBaseParaInfoCnt, &m_ParaUoBtnTabBase);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnTabBase::DoTabCommand(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoTab;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoBtnTabBase.PageScptId)
		{
			// get tab object
			Tv_UoTab	=	HostTabCtrlGet();
			if(NULL != Tv_UoTab)
			{
				((Cls_GrP5UoTabBase*)Tv_UoTab)->TabPageSet(m_ParaUoBtnTabBase.PageScptId,m_ParaUoBtnTabBase.IsPush);
			}
		}

}
//--------------------------------------------------------------------
Cls_GrP5UoBase*	Cls_GrP5UoBtnTabBase::HostTabCtrlGet(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		// get tab object
		if(E_GrP5ScptIdNone == m_ParaUoBtnTabBase.TabScptId)
		{
			Tv_Result	=	ParentClassObjectGet(E_GrP5ClassIdTabBase);
		}
		else
		{
			Tv_Result	=	m_ObjWin->RtlUoFindByScptId(m_ParaUoBtnTabBase.TabScptId);
			if(NULL != Tv_Result)
			{
				if(!Tv_Result->IsInheritClass(E_GrP5ClassIdTabBase))
				{
					Tv_Result	=	NULL;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBtnTabBase::IsTabSelected(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5UoBase*	Tv_UoTab;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_UoTab	=	HostTabCtrlGet();
		if(NULL != Tv_UoTab)
		{
			// get now selected page script
			if(((Cls_GrP5UoTabBase*)Tv_UoTab)->TabNowPageScptIdGet() == m_ParaUoBtnTabBase.PageScptId)
			{
				// selected
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnTabBase::DoEvtBtnRelease(void)
{
	// local -------------------
	// code --------------------
		DoTabCommand();
}
//--------------------------------------------------------------------
