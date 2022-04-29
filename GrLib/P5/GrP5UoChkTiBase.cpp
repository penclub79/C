/*
Platform 5 UI object Button text id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoChkTiBase.h>

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

// relate position
#define	E_GrP5UoChkTiBaseParaInfoCnt							1

St_GrP5DvlScptParaInfo	V_GrP5UoChkTiBaseParaInfo[E_GrP5UoChkTiBaseParaInfoCnt]	=
{
	{L"Check text id base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoChkTiBase::Cls_GrP5UoChkTiBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBtnTid(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdChkTiBase, &m_ParaUoChkTiBase, sizeof(m_ParaUoChkTiBase), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoChkTiBase, sizeof(m_ParaUoChkTiBase));

		m_ParaUoChkTiBase.MarkRc.Right.Val		=	16;
		m_ParaUoChkTiBase.MarkRc.Bottom.Val		=	12;


		m_SkinCtlBtnBase.IsChked		=	FALSE;
}
//--------------------------------------------------------------------
Cls_GrP5UoChkTiBase::~Cls_GrP5UoChkTiBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoChkTiBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnTid::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoChkTiBaseParaInfo, E_GrP5UoChkTiBaseParaInfoCnt, &m_ParaUoChkTiBase);
		DvlParaInfoRelPosAdd(L"Check mark rect", &m_ParaUoChkTiBase.MarkRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoChkTiBase::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBtnTid::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoChkTiBase::EvtPaintProc(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnTid::EvtPaintProc();

		// update checked
		LcValueRead();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoChkTiBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnTid::EvtScptDecDone();

}
//--------------------------------------------------------------------
void	Cls_GrP5UoChkTiBase::DoEvtBtnRelease(void)
{
	// local -------------------
	// code --------------------
		// 
	m_SkinCtlBtnBase.IsChked	=	!m_SkinCtlBtnBase.IsChked;
		LcValueWrite();
		// inherited
		Cls_GrP5UoBtnTid::DoEvtBtnRelease();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoChkTiBase::ValueSet(BOOL8 A_IsSet)
{
		m_SkinCtlBtnBase.IsChked	=	A_IsSet;
		LcValueWrite();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoChkTiBase::ValueGet(void)
{
		LcValueRead();
		return	m_SkinCtlBtnBase.IsChked;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoChkTiBase::LcValueWrite(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoChkTiBase::LcValueRead(void)
{

}
//--------------------------------------------------------------------

