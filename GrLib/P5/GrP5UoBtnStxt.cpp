/*
Platform 5 UI object Button setup text

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoBtnStxt.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5ConBase.h>
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

// relate position
#define	E_GrP5UoBtnStxtParaInfoCnt							6

St_GrP5DvlScptParaInfo	V_GrP5UoBtnStxtParaInfo[E_GrP5UoBtnStxtParaInfoCnt]	=
{
	{L"Button text id", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBtnStxtPara)0)->FontId, TRUE, E_GrP5DvlParaInfoActDefault, L"Font ID"},
	{L"StpAdr", E_GrP5DvlValTypeSetupAdr, 1, (void*)&((Ptr_GrP5UoBtnStxtPara)0)->StpAdr, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup address"},
	{L"TxtLen", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBtnStxtPara)0)->TxtLen, TRUE, E_GrP5DvlParaInfoActDefault, L"Text length"},
	{L"IsPushEdit", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBtnStxtPara)0)->IsPushEdit, TRUE, E_GrP5DvlParaInfoActDefault, L"Edit text when press button"},
	{L"VkbdType", E_GrP5DvlValTypeVkbdType, 1, (void*)&((Ptr_GrP5UoBtnStxtPara)0)->VkbdType, TRUE, E_GrP5DvlParaInfoActDefault, L"Virtual keyboard type"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoBtnStxt::Cls_GrP5UoBtnStxt(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBtnBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdBtnStxt, &m_ParaUoBtnStxt, sizeof(m_ParaUoBtnStxt), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoBtnStxt, sizeof(m_ParaUoBtnStxt));

		m_ParaUoBtnStxt.TxtPara.Atb					=	E_GrP5FontAtbDrawText;
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoBtnStxt.TxtPara);
#endif
		m_ParaUoBtnStxt.TxtRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoBtnStxt.TxtRc.Bottom.Ratio		=	E_GrP5RelRectRatioBaseMax;

		// reset 
		m_SkinCtlBtnBase.Str	=	NULL;
}
//--------------------------------------------------------------------
Cls_GrP5UoBtnStxt::~Cls_GrP5UoBtnStxt(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnStxt::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoBtnStxtParaInfo, E_GrP5UoBtnStxtParaInfoCnt, &m_ParaUoBtnStxt);
		DvlParaInfoTxtParaAdd(L"Button text para", &m_ParaUoBtnStxt.TxtPara);
		DvlParaInfoRelPosAdd(L"Button text rect", &m_ParaUoBtnStxt.TxtRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnStxt::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoAction(A_Action);

		// local action
		LcTxtPtrUpdt();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnStxt::EvtPaintProc(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::EvtPaintProc();

		// draw text
		LcTxtPtrUpdt();

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnStxt::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::EvtScptDecDone();

		// update text
		LcTxtPtrUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnStxt::LcTxtPtrUpdt(void)
{
	// local -------------------
	// code --------------------
		m_SkinCtlBtnBase.Str	=	(WCHAR*)GrP5SetupOfsPtrGet(m_ParaUoBtnStxt.StpAdr);
		
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnStxt::DoEvtBtnPress(void)
{
	// local -------------------
	// code --------------------
		if(!m_ParaUoBtnStxt.IsPushEdit)
		{
			Cls_GrP5UoBtnBase::DoEvtBtnPress();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnStxt::DoEvtBtnRelease(void)
{
	// local -------------------
		BOOL8	Tv_IsPw;
	// code --------------------

		if(m_ParaUoBtnStxt.IsPushEdit)
		{
			// do edit text
			if((NULL != m_SkinCtlBtnBase.Str) && (0 != m_ParaUoBtnStxt.TxtLen))
			{
				Tv_IsPw		=	FALSE;
				if(0 != (E_GrP5FontAtbPassword & m_ParaUoBtnStxt.TxtPara.Atb))
				{
					Tv_IsPw		=	TRUE;
				}
				GrP5VkbdCreate(m_ParaUoBtnStxt.VkbdType, this,E_GrP5CtrlIdNone, m_SkinCtlBtnBase.Str, (__u32)m_ParaUoBtnStxt.TxtLen, Tv_IsPw,0);
			}
		}
		else
		{
			Cls_GrP5UoBtnBase::DoEvtBtnRelease();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnStxt::EvtVkbdInput(void)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		// do value change event
		Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctl)
		{
			Tv_Ctl->EvtBtnValChg(m_ParaUoBase.CtrlId, m_ObjId);
		}
		
		// redraw
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnStxt::SetupAdrSet(__u16 A_Adr)
{
	// local -------------------
	// code --------------------
		m_ParaUoBtnStxt.StpAdr	=	A_Adr;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoBtnStxt::SetupAdrGet(__u16 A_Adr)
{
		return	m_ParaUoBtnStxt.StpAdr;
}
//--------------------------------------------------------------------
