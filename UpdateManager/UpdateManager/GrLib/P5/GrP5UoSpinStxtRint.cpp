/*
Platform 5 UI object Spin setup text register integer

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoSpinStxtRint.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <P5/GrP5Key.h>

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
#define	E_GrP5UoSpinStxtRintParaInfoCnt							2

St_GrP5DvlScptParaInfo	V_GrP5UoSpinStxtRintParaInfo[E_GrP5UoSpinStxtRintParaInfoCnt]	=
{
	{L"Spin Setup Text By Register", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"RintVal", E_GrP5DvlValTypeRintId, 1, (void*)&((Ptr_GrP5UoSpinStxtRintPara)0)->RintVal, TRUE, E_GrP5DvlParaInfoActDefault, L"Register index"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoSpinStxtRint::Cls_GrP5UoSpinStxtRint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoSpinStxtBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdSpinStxtRint, &m_ParaUoSpinStxtRint, sizeof(m_ParaUoSpinStxtRint), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoSpinStxtRint, sizeof(m_ParaUoSpinStxtRint));


		m_ValIdx		=	0;

}
//--------------------------------------------------------------------
Cls_GrP5UoSpinStxtRint::~Cls_GrP5UoSpinStxtRint(void)
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinStxtRint::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinStxtBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoSpinStxtRintParaInfo, E_GrP5UoSpinStxtRintParaInfoCnt, &m_ParaUoSpinStxtRint);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinStxtRint::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinStxtBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinStxtRint::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		Cls_GrP5UoSpinStxtBase::EvtScptDecDone();


}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinStxtRint::ValueRead(void)
{
	// local -------------------
	// code --------------------
		if(0 != m_ParaUoSpinStxtBase.Cnt)
		{
			m_ValIdx	=	GrP5RintGet(m_ParaUoSpinStxtRint.RintVal);
			if(m_ValIdx > (__u32)m_ParaUoSpinStxtBase.Cnt)
			{
				m_ValIdx	=	(__u32)m_ParaUoSpinStxtBase.Cnt - 1;
			}
		}
		else
		{
			m_ValIdx	=	0;
		}
		// inherited
		Cls_GrP5UoSpinStxtBase::ValueRead();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinStxtRint::ValueWrite(void)
{
	// local -------------------
	// code --------------------
		if(0 != m_ParaUoSpinStxtBase.Cnt)
		{
			if(m_ValIdx > (__u32)m_ParaUoSpinStxtBase.Cnt)
			{
				m_ValIdx	=	(__u32)m_ParaUoSpinStxtBase.Cnt - 1;
			}
			// write
			GrP5RintSet(m_ParaUoSpinStxtRint.RintVal, (__s32)m_ValIdx);
		}
		else
		{
			m_ValIdx	=	0;
		}
}
//--------------------------------------------------------------------
