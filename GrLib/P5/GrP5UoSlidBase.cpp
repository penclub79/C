/*
Platform 5 UI scroll bar standard

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoSlidBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <P5/GrP5UoLblBase.h>

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
#define	E_GrP5UoSlidBaseParaInfoCnt							5

St_GrP5DvlScptParaInfo	V_GrP5UoSlidBaseParaInfo[E_GrP5UoSlidBaseParaInfoCnt]	=
{
	{L"Slider base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"ValMin", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoSlidBasePara)0)->ValMin, TRUE, E_GrP5DvlParaInfoActDefault, L"Value minimum"},
	{L"ValMax", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoSlidBasePara)0)->ValMax, TRUE, E_GrP5DvlParaInfoActDefault, L"Value maximum"},
	{L"ValStep", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoSlidBasePara)0)->ValStep, TRUE, E_GrP5DvlParaInfoActDefault, L"Value step"},
	{L"RelLbl", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoSlidBasePara)0)->RelLbl, TRUE, E_GrP5DvlParaInfoActDefault, L"Relate label"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoSlidBase::Cls_GrP5UoSlidBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdSlidBase, NULL, 0, NULL);
	
		m_Value					=	0;
		m_IsPush				=	FALSE;

		// default setting
		GrDumyZeroMem(&m_ParaUoSlidBase, sizeof(m_ParaUoSlidBase));
		m_ParaUoSlidBase.ValMin		=	0;
		m_ParaUoSlidBase.ValMax		=	100;
		m_ParaUoSlidBase.ValStep	=	1;

}
//--------------------------------------------------------------------
Cls_GrP5UoSlidBase::~Cls_GrP5UoSlidBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSlidBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoSlidBaseParaInfo, E_GrP5UoSlidBaseParaInfoCnt, &m_ParaUoSlidBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSlidBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------
		LcValueCorrect();

		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();

		LcValueCorrect();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSlidBase::IsKeyAble(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSlidBase::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidBase::EvtMseEnter(void)
{
	// local -------------------
		Cls_GrP5UoLblBase*	Tv_Lbl;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoSlidBase.RelLbl)
		{
			Tv_Lbl	=	(Cls_GrP5UoLblBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoSlidBase.RelLbl);
			if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblBase))
			{
				if(NULL != Tv_Lbl)
				{
					Tv_Lbl->RtlStatSet(E_GrP5StatOnFocus);
				}
#ifdef GR_P5_EDITOR
				else
				{
					DbgMsgPrint("Cls_GrP5UoSlidBase::EvtMseEnter - script id (%d) is not label!\n", m_ParaUoSlidBase.RelLbl);
				}
#endif
			}
#ifdef GR_P5_EDITOR
			else
			{
				DbgMsgPrint("Cls_GrP5UoSlidBase::EvtMseEnter - bad relate lable script id (%d)!\n", m_ParaUoSlidBase.RelLbl);
			}
#endif
		}

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidBase::EvtMseLeave(void)
{
	// local -------------------
		Cls_GrP5UoLblBase*	Tv_Lbl;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoSlidBase.RelLbl)
		{
			Tv_Lbl	=	(Cls_GrP5UoLblBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoSlidBase.RelLbl);
			if(NULL != Tv_Lbl)
			{
				if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblBase))
				{
					Tv_Lbl->RtlStatSet(0);
				}
#ifdef GR_P5_EDITOR
				else
				{
					DbgMsgPrint("Cls_GrP5UoSlidBase::EvtMseLeave - script id (%d) is not label!\n", m_ParaUoSlidBase.RelLbl);
				}
#endif
			}
#ifdef GR_P5_EDITOR
			else
			{
				DbgMsgPrint("Cls_GrP5UoSlidBase::EvtMseLeave - bad relate lable script id (%d)!\n", m_ParaUoSlidBase.RelLbl);
			}
#endif
		}

		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidBase::EvtKeyFocusGet(__u32 A_Para)
{
	// local -------------------
		Cls_GrP5UoLblBase*	Tv_Lbl;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoSlidBase.RelLbl)
		{
			Tv_Lbl	=	(Cls_GrP5UoLblBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoSlidBase.RelLbl);
			if(NULL != Tv_Lbl)
			{
				if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblBase))
				{
					Tv_Lbl->RtlStatSet(E_GrP5StatOnFocus);
				}
#ifdef GR_P5_EDITOR
				else
				{
					DbgMsgPrint("Cls_GrP5UoSlidBase::EvtKeyFocusGet - script id (%d) is not label!\n", m_ParaUoSlidBase.RelLbl);
				}
#endif
			}
#ifdef GR_P5_EDITOR
			else
			{
				DbgMsgPrint("Cls_GrP5UoSlidBase::EvtKeyFocusGet - bad relate lable script id (%d)!\n", m_ParaUoSlidBase.RelLbl);
			}
#endif
		}

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidBase::EvtKeyFocusLoss(void)
{
	// local -------------------
		Cls_GrP5UoLblBase*	Tv_Lbl;
	// code --------------------
		if(E_GrP5ScptIdNone != m_ParaUoSlidBase.RelLbl)
		{
			Tv_Lbl	=	(Cls_GrP5UoLblBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoSlidBase.RelLbl);
			if(NULL != Tv_Lbl)
			{
				if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblBase))
				{
					Tv_Lbl->RtlStatSet(0);
				}
#ifdef GR_P5_EDITOR
				else
				{
					DbgMsgPrint("Cls_GrP5UoSlidBase::EvtKeyFocusLoss - script id (%d) is not label!\n", m_ParaUoSlidBase.RelLbl);
				}
#endif
			}
#ifdef GR_P5_EDITOR
			else
			{
				DbgMsgPrint("Cls_GrP5UoSlidBase::EvtKeyFocusLoss - bad relate lable script id (%d)!\n", m_ParaUoSlidBase.RelLbl);
			}
#endif
		}

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSlidBase::EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;

		if(m_IsPush)
		{
			// check mode
			EvtSlidBtnPosMove(A_X, A_Y);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSlidBase::EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		//Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		MseCapture();

		m_IsPush	=	TRUE;

		EvtSlidBtnPosMove(A_X, A_Y);

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSlidBase::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		//Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------

		MseRelese();

		m_IsPush	=	FALSE;

		ReqDraw(FALSE);

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSlidBase::ValueRangeSet(__s32 A_Begin, __s32 A_End)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check value range
		if(A_Begin < A_End)
		{
			m_ParaUoSlidBase.ValMin	=	A_Begin;
			m_ParaUoSlidBase.ValMax	=	A_End;
			LcValueCorrect();

			ReqDraw(FALSE);

			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSlidBase::ValueSet(__s32 A_Val)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if((m_ParaUoSlidBase.ValMin <= A_Val) && (m_ParaUoSlidBase.ValMax >= A_Val))
		{
			m_Value		=	A_Val;

			RtlValueWrite();

			ReqDraw(FALSE);

			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrP5UoSlidBase::LcCalcValToPos(__s32 A_Val)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_ValSize;
	// code --------------------
		Tv_Result	=	0;

#if 0
		if(m_ParaUoSlidBase.ValMin < m_ParaUoSlidBase.ValMax)
		{
			if((m_ParaUoSlidBase.ValMin <= A_Val) && (m_ParaUoSlidBase.ValMax >= A_Val))
			{
				Tv_ValSize	=	m_ParaUoSlidBase.ValMax - m_ParaUoSlidBase.ValMin;
				Tv_Result		=	((A_Val *  (m_PntPosAreaSize - (__s32)m_PntInfo.PosBtnSize)) / Tv_ValSize);
			}
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrP5UoSlidBase::LcCalcPosToVal(__s32 A_Pos)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_Pos;
		__s32	Tv_ValSize;
	// code --------------------
		Tv_Result	=	m_ParaUoSlidBase.ValMin;

#if 0
		if(0 != m_PntInfo.PosBtnSize)
		{
			Tv_Pos	=	A_Pos - m_PntInfo.Margin.left - ((__s32)m_PntInfo.PosBtnSize >> 1);
			if(0 > Tv_Pos)
			{
				Tv_Pos	=	0;
			}
			if((m_PntPosAreaSize - (__s32)m_PntInfo.PosBtnSize) < Tv_Pos)
			{
				Tv_Pos	=	(m_PntPosAreaSize - m_PntInfo.PosBtnSize);
			}

			Tv_ValSize	=	m_ParaUoSlidBase.ValMax - m_ParaUoSlidBase.ValMin;
			Tv_Result	=	((Tv_Pos * Tv_ValSize) / (m_PntPosAreaSize - m_PntInfo.PosBtnSize)) +  m_ParaUoSlidBase.ValMin;
		}
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidBase::EvtSlidBtnPosMove(__s32 A_X, __s32 A_Y)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidBase::RtlValueRead(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidBase::RtlValueWrite(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidBase::NotifyValChg(void)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctl)
		{
			Tv_Ctl->EvtSlidValChanged(m_ParaUoBase.CtrlId, m_ObjId);
		}
}
//--------------------------------------------------------------------
__s32	Cls_GrP5UoSlidBase::ValueGet(void)
{
	// local -------------------
	// code --------------------
		return	m_Value;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidBase::LcValueCorrect(void)
{
	// local -------------------
		BOOL8	Tv_IsChg;
	// code --------------------
		Tv_IsChg		=	FALSE;

		if(m_ParaUoSlidBase.ValMin < m_ParaUoSlidBase.ValMax)
		{
			if(m_Value < m_ParaUoSlidBase.ValMin)
			{
				m_Value			=	m_ParaUoSlidBase.ValMin;
				Tv_IsChg		=	TRUE;
			}
			if(m_Value > m_ParaUoSlidBase.ValMax)
			{
				m_Value			=	m_ParaUoSlidBase.ValMax;
				Tv_IsChg		=	TRUE;
			}

			if(Tv_IsChg)
			{
				RtlValueWrite();
				NotifyValChg();
			}

		}

}
//--------------------------------------------------------------------
