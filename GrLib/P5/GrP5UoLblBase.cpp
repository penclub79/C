/*
Platform 5 UI object Label base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoLblBase.h>

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

WCHAR	V_GrP5UoLblBaseStrParaSepLabelBase[]			=	L"Label base";

WCHAR	V_GrP5UoLblBaseStrParaSepTxtPara[]				=	L"Text Parametter";

#define	E_GrP5UoLblBaseParaInfoCnt							1

St_GrP5DvlScptParaInfo	V_GrP5UoLblBaseParaInfo[E_GrP5UoLblBaseParaInfoCnt]	=
{
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoLblBasePara)0)->FontId, TRUE, 0, L"Font ID"},
};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoLblBase::Cls_GrP5UoLblBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdLblBase, &m_ParaUoLblBase, sizeof(m_ParaUoLblBase), &m_SkinCtlLblBase);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoLblBase, sizeof(m_ParaUoLblBase));
		GrDumyZeroMem(&m_SkinCtlLblBase, sizeof(m_SkinCtlLblBase));

		m_ParaUoLblBase.TxtPara.Atb		=	E_GrP5FontAtbDrawText;
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoLblBase.TxtPara);
#endif

		m_ParaUoLblBase.TxtRc.Left.Ratio		=	0;
		m_ParaUoLblBase.TxtRc.Top.Ratio			=	0;
		m_ParaUoLblBase.TxtRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoLblBase.TxtRc.Bottom.Ratio	=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoLblBase.TxtRc.Left.Val			=	0;
		m_ParaUoLblBase.TxtRc.Top.Val				=	0;
		m_ParaUoLblBase.TxtRc.Right.Val			=	0;
		m_ParaUoLblBase.TxtRc.Bottom.Val		=	0;

		// reset
		m_SkinCtlLblBase.CusPos	=	-1;

}
//--------------------------------------------------------------------
Cls_GrP5UoLblBase::~Cls_GrP5UoLblBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblBase::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblBase::DvlParaInfoBuild(void)
{
	// local -------------------
		St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// separator basic info
		Tv_ParaInfo.StrName		=	V_GrP5UoLblBaseStrParaSepLabelBase;
		Tv_ParaInfo.StrDesc		=	NULL;
		Tv_ParaInfo.ValType		=	E_GrP5DvlValTypeSeparator;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.Act				=	0;
		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);

		DvlParaInfoAdd(V_GrP5UoLblBaseParaInfo, E_GrP5UoLblBaseParaInfoCnt, &m_ParaUoLblBase);

		// font parametter
		DvlParaInfoTxtParaAdd(V_GrP5UoLblBaseStrParaSepTxtPara, &m_ParaUoLblBase.TxtPara);

		DvlParaInfoRelPosAdd(L"Text Rect", &m_ParaUoLblBase.TxtRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblBase::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblBase::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoLblBase::RtlStatSet(__u32 A_Stat)
{
	// local -------------------
	// code --------------------
		if(m_SkinCtlLblBase.RtlStat != A_Stat)
		{
			m_SkinCtlLblBase.RtlStat	=	A_Stat;
			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------
