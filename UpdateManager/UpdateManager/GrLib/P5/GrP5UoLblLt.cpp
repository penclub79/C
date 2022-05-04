/*
Platform 5 UI object Label Text Id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoLblLt.h>

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


#if 0

WCHAR	V_Cls_GrP5UoLblLtStrParaSepLabelBase[]			=	L"Label by Local text";

#define	E_Cls_GrP5UoLblLtParaInfoCnt							1

St_GrP5DvlScptParaInfo	V_Cls_GrP5UoLblLtParaInfo[E_Cls_GrP5UoLblLtParaInfoCnt]	=
{
	{L"TxtId", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoLblLtPara)0)->TxtId, TRUE, 0, L"Text ID"},
};

#endif

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoLblLt::Cls_GrP5UoLblLt(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoLblBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		//ClassRegist(E_GrP5ClassIdLblLt, &m_ParaUoLblLt, sizeof(m_ParaUoLblLt));
		ClassRegist(E_GrP5ClassIdLblLt, NULL, 0, NULL);
		
		// default setting
		//GrDumyZeroMem(&m_ParaUoLblLt, sizeof(m_ParaUoLblLt));

		m_SkinCtlLblBase.Str	=	GrP5TxtBufAlloc(m_ObjId);
		if(NULL != m_SkinCtlLblBase.Str)
		{
			GrStrClear(m_SkinCtlLblBase.Str);
		}
}
//--------------------------------------------------------------------
Cls_GrP5UoLblLt::~Cls_GrP5UoLblLt(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblLt::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoLblBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblLt::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoLblBase::DvlParaInfoBuild();

		// separator basic info
#if 0
		Tv_ParaInfo.StrName		=	V_Cls_GrP5UoLblLtStrParaSepLabelBase;
		Tv_ParaInfo.StrDesc		=	NULL;
		Tv_ParaInfo.ValType		=	E_GrP5DvlValTypeSeparator;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.Act				=	0;
		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);

		DvlParaInfoAdd(V_Cls_GrP5UoLblLtParaInfo, E_Cls_GrP5UoLblLtParaInfoCnt, &m_ParaUoLblLt);
#endif

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblLt::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoLblBase::DvlParaInfoAction(A_Action);

		// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblLt::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
	// delete prepare


	// inherited
	Cls_GrP5UoLblBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoLblLt::TextSet(WCHAR* A_Str)
{
	// local -------------------
	// code --------------------
		if(NULL != m_SkinCtlLblBase.Str)
		{
			GrStrWcopy(m_SkinCtlLblBase.Str, A_Str, E_GrP5TxtBufMaxLen);
			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------

