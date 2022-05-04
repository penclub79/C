/*
Platform 5 UI object Button base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoCbxBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <P5/GrP5WinPopList.h>
#include <P5/GrP5Key.h>

#ifdef LINUX_APP

#include <stdlib.h>

#endif

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
#define	E_GrP5UoCbxBaseParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoCbxBaseParaInfo[E_GrP5UoCbxBaseParaInfoCnt]	=
{
	{L"Combo base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"CbxFontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoCbxBasePara)0)->CbxFontId, TRUE, E_GrP5DvlParaInfoActDefault, L"Combo font id"},
	{L"PopDispCnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoCbxBasePara)0)->PopDispCnt, TRUE, E_GrP5DvlParaInfoActDefault, L"Popup display line count"},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoCbxBase::Cls_GrP5UoCbxBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdCbxBase, &m_ParaUoCbxBase, sizeof(m_ParaUoCbxBase), &m_SkinCtlCbxBase);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoCbxBase, sizeof(m_ParaUoCbxBase));
		GrDumyZeroMem(&m_SkinCtlCbxBase, sizeof(m_SkinCtlCbxBase));
		m_ParaUoCbxBase.CbxTxtRc.Left.Ratio		=	0;
		m_ParaUoCbxBase.CbxTxtRc.Left.Val			=	0;
		m_ParaUoCbxBase.CbxTxtRc.Top.Ratio		=	0;
		m_ParaUoCbxBase.CbxTxtRc.Top.Val			=	0;
		m_ParaUoCbxBase.CbxTxtRc.Right.Ratio	=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoCbxBase.CbxTxtRc.Right.Val		=	-((__s32)(__u32)m_ParaUoBase.SkinPara.ParaByte0);
		m_ParaUoCbxBase.CbxTxtRc.Bottom.Ratio	=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoCbxBase.CbxTxtRc.Bottom.Val		=	0;

		m_ParaUoCbxBase.CbxTpara.Atb		=	E_GrP5FontAtbDrawText;
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoCbxBase.CbxTpara);
#endif
		m_ParaUoCbxBase.CbxTpara.ColorShadow	=	GrDrawCvtColorByFcc(E_GrFccABGR8888, Mac_GrMakeArgb32(255, 0, 0, 0), GrP5ScriptPxFmtGet());

		m_SelIdx			=	-1;

}
//--------------------------------------------------------------------
Cls_GrP5UoCbxBase::~Cls_GrP5UoCbxBase(void)
{
	// local -------------------
	// code --------------------
		// block popup
		
		// close popup
		GrP5PopupCloseAll();

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCbxBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoCbxBaseParaInfo, E_GrP5UoCbxBaseParaInfoCnt, &m_ParaUoCbxBase);
		DvlParaInfoTxtParaAdd(L"Combo Tpara", &m_ParaUoCbxBase.CbxTpara);
		DvlParaInfoRelPosAdd(L"Combo Text Rect", &m_ParaUoCbxBase.CbxTxtRc);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCbxBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		Cls_GrP5UoBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCbxBase::IsKeyAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCbxBase::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxBase::EvtKeyFocusGet(__u32 A_Para)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxBase::EvtKeyFocusLoss(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxBase::EvtMseEnter(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxBase::EvtMseLeave(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCbxBase::EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		//Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		MseCapture();
		m_SkinCtlCbxBase.IsPush		=	TRUE;
		ReqDraw(FALSE);

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCbxBase::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		//Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------

		MseRelese();
		m_SkinCtlCbxBase.IsPush		=	FALSE;
		ReqDraw(FALSE);

		// do popup
		LcDoPopup();

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxBase::EvtListItemSel(__u16 A_ObjId, __s32 A_ItemIdx)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctl;
	// code --------------------
		// change foucs
		ReqKeyFocusSet();

		// do combobox event
		Tv_Ctl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctl)
		{
			Tv_Ctl->EvtCbxItemSel(m_ParaUoBase.CtrlId, m_ObjId);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxBase::EvtPopSelCancel(void)
{
		// change foucs
		ReqKeyFocusSet();

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoCbxBase::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		
		if(E_GrP5KeyEnter == A_Key)
		{
			LcDoPopup();
			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyUp(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrP5UoCbxBase::SelectIdxGet(void)
{
		return	m_SelIdx;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCbxBase::LcDoPopup(void)
{
	// local -------------------
		St_GrP5FontDrawStyle	Tv_TxtStyle;
		__u32	Tv_ViewIdx;
		__u32	Tv_PopDispCnt;
	// code --------------------
		// popup item build
		PopupItemBuild();

		// check item exist
		if(0 == V_GrP5Env.ItemCnt)
		{
			return;
		}

		// do popup
		GrDumyCopyMem(&Tv_TxtStyle, &m_ParaUoCbxBase.CbxTpara, sizeof(Tv_TxtStyle));
		//Tv_TxtStyle.AlignHz		=	E_GrP5FontAlignLeft;
		//Tv_TxtStyle.AlignVt		=	E_GrP5FontAlignCenterVt;

		Tv_ViewIdx	=	0;
		if(0 <= m_SelIdx)
		{
			Tv_ViewIdx	=	(__u32)m_SelIdx;
		}

		Tv_PopDispCnt	=	(__u32)m_ParaUoCbxBase.PopDispCnt;
		if(V_GrP5Env.ItemCnt <= Tv_PopDispCnt)
		{
			Tv_PopDispCnt	=	V_GrP5Env.ItemCnt;
		}

		GrP5ListPopByRect(&m_RcScr, m_ConIdx, E_GrP5UoCbxPopLayer, m_ParaUoCbxBase.CbxFontId, &Tv_TxtStyle,
			Tv_PopDispCnt, Tv_ViewIdx, m_ObjId);

}
//--------------------------------------------------------------------
