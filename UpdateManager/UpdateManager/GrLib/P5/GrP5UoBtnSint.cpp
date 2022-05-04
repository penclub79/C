/*
Platform 5 UI object Button setup text

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoBtnSint.h>

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
#define	E_GrP5UoBtnSintParaInfoCnt							10

St_GrP5DvlScptParaInfo	V_GrP5UoBtnSintParaInfo[E_GrP5UoBtnSintParaInfoCnt]	=
{
	{L"Button text id", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBtnSintPara)0)->FontId, TRUE, E_GrP5DvlParaInfoActDefault, L"Font ID"},
	{L"StpAdr", E_GrP5DvlValTypeSetupAdr, 1, (void*)&((Ptr_GrP5UoBtnSintPara)0)->StpAdr, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup address"},
	{L"ValSize", E_GrVarTypeVarSize, 1, (void*)&((Ptr_GrP5UoBtnSintPara)0)->ValSize, TRUE, E_GrP5DvlParaInfoActDefault, L"Value size (1,2,4)"},
	{L"IsPushEdit", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBtnSintPara)0)->IsPushEdit, TRUE, E_GrP5DvlParaInfoActDefault, L"Edit text when press button"},
	{L"IsSign", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBtnSintPara)0)->IsSign, TRUE, E_GrP5DvlParaInfoActDefault, L"Signed variable type"},
	{L"IsHex", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBtnSintPara)0)->IsHex, TRUE, E_GrP5DvlParaInfoActDefault, L"Hex"},
	{L"IsObserve", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoBtnSintPara)0)->IsObserve, TRUE, E_GrP5DvlParaInfoActDefault, L"Observe value"},
	{L"ValMin", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoBtnSintPara)0)->ValMin, TRUE, E_GrP5DvlParaInfoActDefault, L"Value min"},
	{L"ValMax", E_GrVarTypeS32, 1, (void*)&((Ptr_GrP5UoBtnSintPara)0)->ValMax, TRUE, E_GrP5DvlParaInfoActDefault, L"Value max"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoBtnSint::Cls_GrP5UoBtnSint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBtnBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdBtnSint, &m_ParaUoBtnSint, sizeof(m_ParaUoBtnSint), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoBtnSint, sizeof(m_ParaUoBtnSint));

		m_ParaUoBtnSint.TxtPara.Atb					=	E_GrP5FontAtbDrawText;
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoBtnSint.TxtPara);
#endif
		m_ParaUoBtnSint.TxtRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoBtnSint.TxtRc.Bottom.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoBtnSint.ValMax	=	0x7FFFFFFF;

		// reset 
		m_IsHex		=	m_ParaUoBtnSint.IsHex;

		m_SkinCtlBtnBase.Str	=	GrP5TxtBufAlloc(m_ObjId);
		if(NULL != m_SkinCtlBtnBase.Str)
		{
			GrStrClear(m_SkinCtlBtnBase.Str);
			GrStrAddWchar(m_SkinCtlBtnBase.Str, 0x30);
		}

}
//--------------------------------------------------------------------
Cls_GrP5UoBtnSint::~Cls_GrP5UoBtnSint(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnSint::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoBtnSintParaInfo, E_GrP5UoBtnSintParaInfoCnt, &m_ParaUoBtnSint);
		DvlParaInfoTxtParaAdd(L"Button text para", &m_ParaUoBtnSint.TxtPara);
		DvlParaInfoRelPosAdd(L"Button text rect", &m_ParaUoBtnSint.TxtRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnSint::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoAction(A_Action);

		// local action
		LcDispValUpdt(FALSE);
		ReqDraw(FALSE);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnSint::EvtPaintProc(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::EvtPaintProc();

		// update text
		if(!m_ParaUoBtnSint.IsObserve)
		{
			LcDispValUpdt(FALSE);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnSint::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::EvtScptDecDone();

		// update text
		if(LcDispValUpdt(FALSE))
		{
			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoBtnSint::LcDispValUpdt(BOOL8 A_IsForce)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_Val;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Val	=	m_Value;
		switch(m_ParaUoBtnSint.ValSize)
		{
			case E_GrVarSize2byte:
				if(m_ParaUoBtnSint.IsSign && (!m_ParaUoBtnSint.IsHex))
				{
					Tv_Val	=	(__s32)GrP5SetupGetU16(m_ParaUoBtnSint.StpAdr);
				}
				else
				{
					Tv_Val	=	(__s32)((__u32)GrP5SetupGetU16(m_ParaUoBtnSint.StpAdr));
				}
				break;
			case E_GrVarSize4byte:
				Tv_Val	=	(__s32)GrP5SetupGetU32(m_ParaUoBtnSint.StpAdr);
				break;
			default:
				if(m_ParaUoBtnSint.IsSign && (!m_ParaUoBtnSint.IsHex))
				{
					Tv_Val	=	(__s32)GrP5SetupGetU8(m_ParaUoBtnSint.StpAdr);
				}
				else
				{
					Tv_Val	=	(__s32)((__u32)GrP5SetupGetU8(m_ParaUoBtnSint.StpAdr));
				}
				break;
		}
		
		// check change
		if( A_IsForce || (Tv_Val != m_Value) || (m_IsHex != m_ParaUoBtnSint.IsHex))
		{
			// update value
			m_IsHex	=	m_ParaUoBtnSint.IsHex;
			m_Value	=	Tv_Val;
			if(m_IsHex)
			{
				GrStrDwordToHexWstr(m_SkinCtlBtnBase.Str, (__u32)m_Value);
			}
			else
			{
				GrStrIntToWstr(m_SkinCtlBtnBase.Str, m_Value);
			}

			// changed
			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnSint::DoEvtBtnPress(void)
{
	// local -------------------
	// code --------------------
		if(!m_ParaUoBtnSint.IsPushEdit)
		{
			Cls_GrP5UoBtnBase::DoEvtBtnPress();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnSint::DoEvtBtnRelease(void)
{
	// local -------------------
		BOOL8	Tv_IsPw;
	// code --------------------

		if(m_ParaUoBtnSint.IsPushEdit)
		{
			// do edit text
			Tv_IsPw		=	FALSE;
			if(0 != (E_GrP5FontAtbPassword & m_ParaUoBtnSint.TxtPara.Atb))
			{
				Tv_IsPw		=	TRUE;
			}
			if(m_IsHex)
			{
				GrP5VkbdCreate(E_GrP5VkbdTypeHex, this, E_GrP5CtrlIdNone, m_SkinCtlBtnBase.Str, 8, Tv_IsPw,0);
			}
			else
			{
				GrP5VkbdCreate(E_GrP5VkbdTypeDec, this, E_GrP5CtrlIdNone, m_SkinCtlBtnBase.Str, 11, Tv_IsPw,0);
			}
		}
		else
		{
			Cls_GrP5UoBtnBase::DoEvtBtnPress();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnSint::LcSetupValWrite(__s32 A_Val)
{
	// local -------------------
	// code --------------------
		switch(m_ParaUoBtnSint.ValSize)
		{
			case E_GrVarSize2byte:
				GrP5SetupPutU16(m_ParaUoBtnSint.StpAdr, (__u16)A_Val);
				break;
			case E_GrVarSize4byte:
				GrP5SetupPutU32(m_ParaUoBtnSint.StpAdr, (__u32)A_Val);
				break;
			default:
				GrP5SetupPutU8(m_ParaUoBtnSint.StpAdr, (__u8)A_Val);
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnSint::EvtVkbdInput(void)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
		BOOL8	Tv_IsOk;
		__s32	Tv_Val;
	// code --------------------
		// get value
		Tv_IsOk	=	FALSE;
		if(m_IsHex)
		{
			Tv_IsOk	=	GrStrHexWstrToDword(m_SkinCtlBtnBase.Str, (__u32*)&Tv_Val);
		}
		else
		{
			Tv_IsOk	=	GrStrWstrToInt(m_SkinCtlBtnBase.Str, &Tv_Val);
		}

		if(Tv_IsOk)
		{
			// check range
			if((Tv_Val < m_ParaUoBtnSint.ValMin) || (Tv_Val > m_ParaUoBtnSint.ValMax))
			{
				Tv_IsOk	=	FALSE;
			}
		}

		if(Tv_IsOk)
		{
			// write setup value
			m_Value	=	Tv_Val;
			LcSetupValWrite(m_Value);
		}

		LcDispValUpdt(TRUE);

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
void	Cls_GrP5UoBtnSint::SetupAdrSet(__u16 A_Adr)
{
		m_ParaUoBtnSint.StpAdr	=	A_Adr;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoBtnSint::SetupAdrGet(__u16 A_Adr)
{
		return	m_ParaUoBtnSint.StpAdr;
}
//--------------------------------------------------------------------


