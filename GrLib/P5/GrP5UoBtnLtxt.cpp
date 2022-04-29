/*
Platform 5 UI object Button setup text

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoBtnLtxt.h>

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
#define	E_GrP5UoBtnLtxtParaInfoCnt							5

St_GrP5DvlScptParaInfo	V_GrP5UoBtnLtxtParaInfo[E_GrP5UoBtnLtxtParaInfoCnt]	=
{
	{L"Button local text", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBtnLtxtPara)0)->FontId, TRUE, E_GrP5DvlParaInfoActDefault, L"Font ID"},
	{L"TxtLen", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBtnLtxtPara)0)->TxtLen, TRUE, E_GrP5DvlParaInfoActDefault, L"Text length"},
	{L"IsPushEdit", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBtnLtxtPara)0)->IsPushEdit, TRUE, E_GrP5DvlParaInfoActDefault, L"Edit text when press button"},
	{L"VkbdType", E_GrP5DvlValTypeVkbdType, 1, (void*)&((Ptr_GrP5UoBtnLtxtPara)0)->VkbdType, TRUE, E_GrP5DvlParaInfoActDefault, L"Virtual keyboard type"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoBtnLtxt::Cls_GrP5UoBtnLtxt(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBtnBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdBtnLtxt, &m_ParaUoBtnLtxt, sizeof(m_ParaUoBtnLtxt), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoBtnLtxt, sizeof(m_ParaUoBtnLtxt));

		m_ParaUoBtnLtxt.TxtPara.Atb					=	E_GrP5FontAtbDrawText;
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoBtnLtxt.TxtPara);
#endif
		m_ParaUoBtnLtxt.TxtRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoBtnLtxt.TxtRc.Bottom.Ratio		=	E_GrP5RelRectRatioBaseMax;

		// allocate text buffer
		m_SkinCtlBtnBase.Str	=	GrP5TxtBufAlloc(m_ObjId);
		if(NULL != m_SkinCtlBtnBase.Str)
		{
			// reset 
			GrStrClear(m_SkinCtlBtnBase.Str);
		}
}
//--------------------------------------------------------------------
Cls_GrP5UoBtnLtxt::~Cls_GrP5UoBtnLtxt(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnLtxt::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoBtnLtxtParaInfo, E_GrP5UoBtnLtxtParaInfoCnt, &m_ParaUoBtnLtxt);
		DvlParaInfoTxtParaAdd(L"Button text para", &m_ParaUoBtnLtxt.TxtPara);
		DvlParaInfoRelPosAdd(L"Button text rect", &m_ParaUoBtnLtxt.TxtRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnLtxt::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnLtxt::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::EvtScptDecDone();

		// check text max len
		if(E_GrP5TxtBufMaxLen < m_ParaUoBtnLtxt.TxtLen)
		{
			DbgMsgPrint("Cls_GrP5UoBtnLtxt::EvtScptDecDone - text size was big!\n");
			m_ParaUoBtnLtxt.TxtLen	=	E_GrP5TxtBufMaxLen;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnLtxt::DoEvtBtnPress(void)
{
	// local -------------------
	// code --------------------
		if(!m_ParaUoBtnLtxt.IsPushEdit)
		{
			Cls_GrP5UoBtnBase::DoEvtBtnPress();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnLtxt::DoEvtBtnRelease(void)
{
	// local -------------------
		BOOL8	Tv_IsPw;
	// code --------------------

		if(m_ParaUoBtnLtxt.IsPushEdit)
		{
			// do edit text
			if((NULL != m_SkinCtlBtnBase.Str) && (0 != m_ParaUoBtnLtxt.TxtLen))
			{
				Tv_IsPw		=	FALSE;
				if(0 != (E_GrP5FontAtbPassword & m_ParaUoBtnLtxt.TxtPara.Atb))
				{
					Tv_IsPw		=	TRUE;
				}
				GrP5VkbdCreate(m_ParaUoBtnLtxt.VkbdType, this, E_GrP5CtrlIdNone, m_SkinCtlBtnBase.Str, (__u32)m_ParaUoBtnLtxt.TxtLen, Tv_IsPw,0);
			}
		}
		else
		{
			Cls_GrP5UoBtnBase::DoEvtBtnRelease();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnLtxt::EvtVkbdInput(void)
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
void	Cls_GrP5UoBtnLtxt::TextGet(WCHAR* A_StrRtTxt)
{
	// local -------------------
	// code --------------------
		GrStrWcopy(A_StrRtTxt, m_SkinCtlBtnBase.Str);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnLtxt::TextSet(WCHAR* A_StrTxt)
{
		if(0 != m_ParaUoBtnLtxt.TxtLen)
		{
			GrStrWcopy(m_SkinCtlBtnBase.Str, A_StrTxt, (__u32)m_ParaUoBtnLtxt.TxtLen);
			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------

