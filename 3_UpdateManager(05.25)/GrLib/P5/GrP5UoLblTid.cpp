/*
Platform 5 UI object Label Text Id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoLblTid.h>

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

WCHAR	V_Cls_GrP5UoLblTidStrParaSepLabelBase[]			=	L"Label by Text ID";

#define	E_Cls_GrP5UoLblTidParaInfoCnt							1

St_GrP5DvlScptParaInfo	V_Cls_GrP5UoLblTidParaInfo[E_Cls_GrP5UoLblTidParaInfoCnt]	=
{
	{L"TxtId", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoLblTidPara)0)->TxtId, TRUE, 0, L"Text ID"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoLblTid::Cls_GrP5UoLblTid(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoLblBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdLblTid, &m_ParaUoLblTid, sizeof(m_ParaUoLblTid),NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoLblTid, sizeof(m_ParaUoLblTid));

}
//--------------------------------------------------------------------
Cls_GrP5UoLblTid::~Cls_GrP5UoLblTid(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblTid::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoLblBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblTid::DvlParaInfoBuild(void)
{
	// local -------------------
		St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoLblBase::DvlParaInfoBuild();

		// separator basic info
		Tv_ParaInfo.StrName		=	V_Cls_GrP5UoLblTidStrParaSepLabelBase;
		Tv_ParaInfo.StrDesc		=	NULL;
		Tv_ParaInfo.ValType		=	E_GrP5DvlValTypeSeparator;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.Act				=	0;
		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);

		DvlParaInfoAdd(V_Cls_GrP5UoLblTidParaInfo, E_Cls_GrP5UoLblTidParaInfoCnt, &m_ParaUoLblTid);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblTid::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoLblBase::DvlParaInfoAction(A_Action);

		// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblTid::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
	// delete prepare


	// inherited
	Cls_GrP5UoLblBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
__u16	Cls_GrP5UoLblTid::TextIdGet(void)
{
		return	m_ParaUoLblTid.TxtId;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblTid::TextIdSet(__u16 A_TxtId)
{
		m_ParaUoLblTid.TxtId	=	A_TxtId;
		LcSkinCtlUpdate();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------

