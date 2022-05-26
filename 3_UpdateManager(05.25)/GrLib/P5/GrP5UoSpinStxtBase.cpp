/*
Platform 5 UI object Spin setup text base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoSpinStxtBase.h>

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
#define	E_GrP5UoSpinStxtBaseParaInfoCnt							10

St_GrP5DvlScptParaInfo	V_GrP5UoSpinStxtBaseParaInfo[E_GrP5UoSpinStxtBaseParaInfoCnt]	=
{
	{L"Spin Setup Text Base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"StpAdr", E_GrP5DvlValTypeSetupAdr, 1, (void*)&((Ptr_GrP5UoSpinStxtBasePara)0)->StpAdr, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup address"},
	{L"Len", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoSpinStxtBasePara)0)->Len, TRUE, E_GrP5DvlParaInfoActDefault, L"String Length"},
	{L"Cnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoSpinStxtBasePara)0)->Cnt, TRUE, E_GrP5DvlParaInfoActDefault, L"String Count"},
	{L"IsEdit", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoSpinStxtBasePara)0)->IsEdit, TRUE, E_GrP5DvlParaInfoActDefault, L"String edit able"},
	{L"IsCalc", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoSpinStxtBasePara)0)->IsCalc, TRUE, E_GrP5DvlParaInfoActDefault, L"Calculate address"},
	{L"IsZeroText", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoSpinStxtBasePara)0)->IsZeroText, TRUE, E_GrP5DvlParaInfoActDefault, L"Add zero index text"},
	{L"MulVal", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoSpinStxtBasePara)0)->MulVal, TRUE, E_GrP5DvlParaInfoActDefault, L"Multiple value"},
	{L"AddVal", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoSpinStxtBasePara)0)->AddVal, TRUE, E_GrP5DvlParaInfoActDefault, L"Add value"},
	{L"TextIdZero", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoSpinStxtBasePara)0)->TextIdZero, TRUE, E_GrP5DvlParaInfoActDefault, L"ID of zero index text"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoSpinStxtBase::Cls_GrP5UoSpinStxtBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoSpinBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdSpinStxtBase, &m_ParaUoSpinStxtBase, sizeof(m_ParaUoSpinStxtBase), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoSpinStxtBase, sizeof(m_ParaUoSpinStxtBase));


		m_ValIdx		=	0;

}
//--------------------------------------------------------------------
Cls_GrP5UoSpinStxtBase::~Cls_GrP5UoSpinStxtBase(void)
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinStxtBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoSpinStxtBaseParaInfo, E_GrP5UoSpinStxtBaseParaInfoCnt, &m_ParaUoSpinStxtBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinStxtBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinStxtBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		Cls_GrP5UoSpinBase::EvtScptDecDone();


}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinStxtBase::ValueRead(void)
{
	// local -------------------
		__u32	Tv_Sadr;
		__u32	Tv_Size;
		__u32	Tv_ValAdd;

	// code --------------------
		// inherited
		Cls_GrP5UoSpinBase::ValueRead();

		m_SkinCtlSpinBase.StrDisp		=	NULL;
		if((0 == m_ValIdx) && m_ParaUoSpinStxtBase.IsZeroText)
		{
			m_SkinCtlSpinBase.StrDisp	=	GrP5TextGet(m_ParaUoSpinStxtBase.TextIdZero);
		}
		else if((0 != m_ParaUoSpinStxtBase.Len) && (0 != m_ParaUoSpinStxtBase.Cnt) && (m_ValIdx < (__u32)m_ParaUoSpinStxtBase.Cnt))
		{
			Tv_ValAdd	=	0;
			if(m_ParaUoSpinStxtBase.IsZeroText)
			{
				Tv_ValAdd ++;
			}
			Tv_Size	=	(__u32)m_ParaUoSpinStxtBase.Len * 2;
			// calculate address
			if(m_ParaUoSpinStxtBase.IsCalc)
			{
				Tv_Sadr	=	(__u32)m_ParaUoSpinStxtBase.StpAdr + ((m_ValIdx + Tv_ValAdd) * m_ParaUoSpinStxtBase.MulVal) + m_ParaUoSpinStxtBase.AddVal;
			}
			else
			{
				Tv_Sadr	=	(__u32)m_ParaUoSpinStxtBase.StpAdr + ((m_ValIdx + Tv_ValAdd) * Tv_Size);
			}

			// check address
			if((Tv_Sadr + Tv_Size) <= E_GrP5SetupBufSize)
			{
				m_SkinCtlSpinBase.StrDisp	=	(WCHAR*)GrP5SetupOfsPtrGet((__u16)Tv_Sadr);
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinStxtBase::DoIncrease(void)
{
	// local -------------------
		__u32	Tv_Val;
	// code --------------------
		Tv_Val	=	m_ValIdx + 1;
		if(Tv_Val >= (__u32)m_ParaUoSpinStxtBase.Cnt)
		{
			Tv_Val	=	0;
			if(m_ParaUoSpinBase.IsNotRotate)
			{
				if(0 != m_ParaUoSpinStxtBase.Cnt)
				{
					Tv_Val	=	m_ParaUoSpinStxtBase.Cnt - 1;
				}
			}
		}
		m_ValIdx	=	Tv_Val;

		ValueWrite();
		DoEvtSpinValChg();

		ReqDraw(FALSE);

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinStxtBase::DoDecrease(void)
{
	// local -------------------
		__u32	Tv_Val;
	// code --------------------
		Tv_Val	=	m_ValIdx;
		if(0 == Tv_Val)
		{
			if(!m_ParaUoSpinBase.IsNotRotate)
			{
				if(0 != m_ParaUoSpinStxtBase.Cnt)
				{
					Tv_Val	=	(__u32)m_ParaUoSpinStxtBase.Cnt - 1;
				}
			}
		}
		else
		{
			Tv_Val --;
		}
		m_ValIdx	=	Tv_Val;

		ValueWrite();
		DoEvtSpinValChg();

		ReqDraw(FALSE);

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinStxtBase::ProcSpinClickCont(void)
{
	// local -------------------
		BOOL8	Tv_IsPw;
	// code --------------------
		if(!m_ParaUoSpinStxtBase.IsEdit)
		{
			return;
		}

		ValueRead();
		// check setup text
		if((0 == m_ValIdx) && m_ParaUoSpinStxtBase.IsZeroText)
		{
			return;
		}
		// popup virtual keyboard
		if(0 == m_ParaUoSpinStxtBase.Len)
		{
			return;
		}
		if(NULL != m_SkinCtlSpinBase.StrDisp)
		{
			Tv_IsPw		=	FALSE;
			if(0 != (E_GrP5FontAtbPassword & m_ParaUoSpinBase.TxtPara.Atb))
			{
				Tv_IsPw	=	TRUE;
			}
			GrP5VkbdCreate(E_GrP5VkbdTypeNormal, this, E_GrP5CtrlIdNone, m_SkinCtlSpinBase.StrDisp, (__u32)m_ParaUoSpinStxtBase.Len, Tv_IsPw, 0);
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinStxtBase::EvtVkbdInput(void)
{
	// local -------------------
	// code --------------------
		ReqKeyFocusSet();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinStxtBase::EvtVkbdCancel(void)
{
	// local -------------------
	// code --------------------
		ReqKeyFocusSet();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------


