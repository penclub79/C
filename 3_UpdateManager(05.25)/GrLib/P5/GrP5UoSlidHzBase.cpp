/*
Platform 5 UI scroll bar standard

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoSlidHzBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>

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

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoSlidHzBase::Cls_GrP5UoSlidHzBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect ) :
	Cls_GrP5UoSlidBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdSlidHzBase, NULL, 0, NULL);
		

		// default setting
		//GrDumyZeroMem(&m_ParaUoSlidHzBase, sizeof(m_ParaUoSlidHzBase));

}
//--------------------------------------------------------------------
Cls_GrP5UoSlidHzBase::~Cls_GrP5UoSlidHzBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSlidHzBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSlidBase::DvlParaInfoBuild();

		// add
		//DvlParaInfoAdd(V_GrP5UoSlidHzBaseParaInfo, E_GrP5UoSlidHzBaseParaInfoCnt, &m_ParaUoSlidHzBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSlidHzBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------
		// style rebuild
		//BtnStatStyleBuild();

		// inherited
		Cls_GrP5UoSlidBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidHzBase::EvtPaintProc(void)
{
	// local -------------------
	// code --------------------
		LcCalPntDems();

		RtlValueRead();

		// status
		m_PntStat	=	0;
		if(IsCanEnable())
		{
			if(m_IsPush)
			{
				m_PntStat	=	E_GrP5StatPush;
			}
			else if(IsOnFocuse())
			{
				m_PntStat	=	m_PntStat | E_GrP5StatOnFocus;
			}
		}
		else
		{
			m_PntStat	=	E_GrP5StatDisable;
		}

		// paint background
		//m_SkinMng->DrawSlidHzBg(&m_ParaUoBase.SkinPara, &m_RcScr, m_PntStat);

		// paint position button
		if(0 != m_PntRcBtnPos.right)
		{
			//m_SkinMng->DrawSlidHzBtn(&m_ParaUoBase.SkinPara, &m_PntRcBtnPos, m_PntStat);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidHzBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		Cls_GrP5UoSlidBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidHzBase::LcCalPntDems(void)
{
	// local -------------------
	// code --------------------
		// get skin info
		//m_SkinMng->SlidHzInfoGet(&m_ParaUoBase.SkinPara, &m_RcScr, &m_PntInfo);

#if 0
		m_PntPosAreaSize	=	(__u32)(m_RcScr.right - m_RcScr.left) - (m_PntInfo.Margin.left + m_PntInfo.Margin.right);

		// calculate positin button
		m_PntRcBtnPos.right	=	0;
		if(0 != m_PntInfo.PosBtnSize)
		{
			// calculate position
			m_PntRcBtnPos.top			=	m_RcScr.top + m_PntInfo.Margin.top;
			m_PntRcBtnPos.left		=	LcCalcValToPos(m_Value) + m_RcScr.left + m_PntInfo.Margin.left;
			m_PntRcBtnPos.right		=	m_PntRcBtnPos.left + (__s32)m_PntInfo.PosBtnSize;
			m_PntRcBtnPos.bottom	=	m_RcScr.bottom - m_PntInfo.Margin.bottom;
		}
#endif

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidHzBase::EvtSlidBtnPosMove(__s32 A_X, __s32 A_Y)
{
	// local -------------------
	// code --------------------
		// calculate value
		m_Value	=	LcCalcPosToVal(A_X);
		// notify value change
		NotifyValChg();
}
//--------------------------------------------------------------------
