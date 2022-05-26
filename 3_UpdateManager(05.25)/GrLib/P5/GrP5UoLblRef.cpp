/*
Platform 5 UI object Label reference

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoLblRef.h>

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

#define	E_Cls_GrP5UoLblRefParaInfoCnt							2

St_GrP5DvlScptParaInfo	V_Cls_GrP5UoLblRefParaInfo[E_Cls_GrP5UoLblRefParaInfoCnt]	=
{
	{L"Label Reference", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL},
	{L"ClrCus", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoLblRefPara)0)->ClrCus, TRUE, 0, L"Color cursor"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoLblRef::Cls_GrP5UoLblRef(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoLblBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdLblRef, &m_ParaUoLblRef, sizeof(m_ParaUoLblRef), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoLblRef, sizeof(m_ParaUoLblRef));
		m_ParaUoLblRef.ClrCus	=	GrDrawCvtColorByFcc(E_GrFccABGR8888, Mac_GrMakeArgb32(255, 128, 128, 128), GrP5ScriptPxFmtGet());

}
//--------------------------------------------------------------------
Cls_GrP5UoLblRef::~Cls_GrP5UoLblRef(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblRef::EvtScptDecDone(void)
{
		// update
		m_SkinCtlLblBase.ClrCus	=	m_ParaUoLblRef.ClrCus;

		// inherited
		Cls_GrP5UoLblBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblRef::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoLblBase::DvlParaInfoBuild();

		DvlParaInfoAdd(V_Cls_GrP5UoLblRefParaInfo, E_Cls_GrP5UoLblRefParaInfoCnt, &m_ParaUoLblRef);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblRef::DvlParaInfoAction(__u32 A_Action)
{
		// custom action


		// inherited
		Cls_GrP5UoLblBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoLblRef::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare


		// inherited
		Cls_GrP5UoLblBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoLblRef::TextPtrSet(WCHAR* A_StrText, BOOL8 A_IsPasswd)
{
	// local -------------------
	// code --------------------
		m_SkinCtlLblBase.Str	=	A_StrText;
		if(A_IsPasswd)
		{
			m_ParaUoLblBase.TxtPara.Atb	=	m_ParaUoLblBase.TxtPara.Atb | E_GrP5FontAtbPassword;
		}
		else
		{
			m_ParaUoLblBase.TxtPara.Atb	=	m_ParaUoLblBase.TxtPara.Atb & (~E_GrP5FontAtbPassword);
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP5UoLblRef::CursorPosSet(__s32 A_Pos)
{
		m_SkinCtlLblBase.CusPos	=	A_Pos;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------

