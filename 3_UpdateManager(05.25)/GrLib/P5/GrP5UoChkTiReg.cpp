/*
Platform 5 UI object Button text id

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoChkTiReg.h>

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
#define	E_GrP5UoChkTiRegParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoChkTiRegParaInfo[E_GrP5UoChkTiRegParaInfoCnt]	=
{
	{L"Check Tid Reg", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"RegIdx", E_GrP5DvlValTypeRintId, 1, (void*)&((Ptr_GrP5UoChkTiRegPara)0)->RegIdx, TRUE, E_GrP5DvlParaInfoActDefault, L"Value register index"},
	{L"Bit", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoChkTiRegPara)0)->Bit, TRUE, E_GrP5DvlParaInfoActDefault, L"Value bit position"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoChkTiReg::Cls_GrP5UoChkTiReg(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoChkTiBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdChkTiReg, &m_ParaUoChkTiReg, sizeof(m_ParaUoChkTiReg), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoChkTiReg, sizeof(m_ParaUoChkTiReg));

}
//--------------------------------------------------------------------
Cls_GrP5UoChkTiReg::~Cls_GrP5UoChkTiReg(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoChkTiReg::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoChkTiBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoChkTiRegParaInfo, E_GrP5UoChkTiRegParaInfoCnt, &m_ParaUoChkTiReg);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoChkTiReg::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoChkTiBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoChkTiReg::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoChkTiBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
void	Cls_GrP5UoChkTiReg::LcValueWrite(void)
{
	// local -------------------
		__s32	Tv_Val;
		__s32	Tv_Mask;
	// code --------------------
		Tv_Val	=	GrP5RintGet(m_ParaUoChkTiReg.RegIdx);
		Tv_Mask	=	1 << m_ParaUoChkTiReg.Bit;
		if(m_SkinCtlBtnBase.IsChked)
		{
			Tv_Val	=	Tv_Val | Tv_Mask;
		}
		else
		{
			Tv_Val	=	Tv_Val & (~Tv_Mask);
		}
		GrP5RintSet(m_ParaUoChkTiReg.RegIdx, Tv_Val);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoChkTiReg::LcValueRead(void)
{
	// local -------------------
		__s32	Tv_Val;
		__s32	Tv_Mask;
	// code --------------------
		Tv_Val	=	GrP5RintGet(m_ParaUoChkTiReg.RegIdx);
		Tv_Mask	=	1 << m_ParaUoChkTiReg.Bit;
		if(0 == (Tv_Val & Tv_Mask))
		{
			m_SkinCtlBtnBase.IsChked	=	FALSE;
		}
		else
		{
			m_SkinCtlBtnBase.IsChked	=	TRUE;
		}

}
//--------------------------------------------------------------------

