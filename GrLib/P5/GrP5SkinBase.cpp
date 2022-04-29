/*
Platform 5 skin base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5SkinBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>

#include <P5/GrP5SkinT1.h>
#include <P5/GrP5SkinT2.h>

#ifdef GR_P5_EDITOR

#include <P5\Dvl\GrP5DvlBase.h>

#endif

//--------------------------------------------------------------------
// local constant

#define	E_GrP5SkinCnt				2


//--------------------------------------------------------------------
// local type


//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;

St_GrP5SkinCfg	V_GrP5SkinCfg;

Cls_GrP5SkinBase*	V_GrP5SkinMng	=	NULL;

#ifdef GR_P5_EDITOR
Def_WstrName	V_GrP5SkinNameTbl[E_GrP5SkinCnt]	=	
{
	{L"OEM-T2"},
	{L"UDR-T1"},
};


St_GrP5DvlScptParaInfo	V_GrP5SkinCfgParaInfo[E_GrP5SkinCfgParaCnt]	=	
{
	{L"SkinId", E_GrP5DvlValTypeSkinId, 1, (void*)&V_GrP5SkinCfg.SkinId, TRUE, 0, L"Skin ID"},
	{L"TooltipFontId", E_GrVarTypeU8, 1, (void*)&V_GrP5SkinCfg.TooltipFontId, TRUE, 0, L"Tooltip font id"},
	{L"TooltipTxtGapChar", E_GrVarTypeU8, 1, (void*)&V_GrP5SkinCfg.TooltipTxtStyle.GapChar, TRUE, 0, L"Tooltip text gap"},
	{L"TooltipTxtDrawTxt", E_GrVarTypeBitBoolBit0, 1, (void*)&V_GrP5SkinCfg.TooltipTxtStyle.Atb, TRUE, 0, L"Tooltip draw text"},
	{L"TooltipTxtDrawShadow", E_GrVarTypeBitBoolBit1, 1, (void*)&V_GrP5SkinCfg.TooltipTxtStyle.Atb, TRUE, 0, L"Tooltip draw shadow"},
	{L"TooltipTxtFixPitch", E_GrVarTypeBitBoolBit3, 1, (void*)&V_GrP5SkinCfg.TooltipTxtStyle.Atb, TRUE, 0, L"Tooltip pitch fix"},
	{L"TooltipTxtClrTxt", E_GrVarTypeColor, 1, (void*)&V_GrP5SkinCfg.TooltipTxtStyle.ColorTxtTbl[0], TRUE, 0, L"Tooltip color text"},
	{L"TooltipTxtClrShadow", E_GrVarTypeColor, 1, (void*)&V_GrP5SkinCfg.TooltipTxtStyle.ColorShadow, TRUE, 0, L"Tooltip color shadow"},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5SkinBase::Cls_GrP5SkinBase(__u8 A_SkinId)
{
	// local -------------------
	// code --------------------
		// init
		m_SkinId		=	A_SkinId;

}
//--------------------------------------------------------------------
Cls_GrP5SkinBase::~Cls_GrP5SkinBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
__u8	Cls_GrP5SkinBase::SkinIdGet(void)
{	
		return	m_SkinId;
}
//--------------------------------------------------------------------
void	GrP5SkinInit(void)
{
		GrDumyZeroMem(&V_GrP5SkinCfg, sizeof(V_GrP5SkinCfg));
		V_GrP5SkinCfg.TooltipTxtStyle.Atb	=	E_GrP5FontAtbDrawText;
		//V_GrP5SkinCfg.TooltipTxtStyle.ColorTxtTbl[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
}
//--------------------------------------------------------------------
void	GrP5SkinFinish(void)
{
	// local -------------------
	// code --------------------
		if(NULL != V_GrP5SkinMng)
		{
			delete	V_GrP5SkinMng;
			V_GrP5SkinMng	=	NULL;
		}
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
__u8	GrP5SkinDvlCountGet(void)
{
		return	E_GrP5SkinCnt;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
WCHAR*	GrP5SkinDvlNameGet(__u8 A_SkinId)
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;

		if (E_GrP5SkinCnt > A_SkinId)
		{
			Tv_Result	=	V_GrP5SkinNameTbl[A_SkinId];
		}

		return	Tv_Result;
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	GrP5SkinDvlFontColorSet(Ptr_GrP5FontDrawStyle A_PtrStyle)
{
	// local -------------------
	// code --------------------
		A_PtrStyle->ColorShadow			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 0), GrP5ScriptPxFmtGet());
		A_PtrStyle->ColorTxtTbl[0]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		A_PtrStyle->ColorTxtTbl[1]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 150, 150, 150), GrP5ScriptPxFmtGet());
		A_PtrStyle->ColorTxtTbl[2]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 230, 230, 0), GrP5ScriptPxFmtGet());
		A_PtrStyle->ColorTxtTbl[3]		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 230, 230, 0), GrP5ScriptPxFmtGet());
}
#endif
//--------------------------------------------------------------------
void	GrP5SkinSetByCfg(void)
{
	// local -------------------
		
	// code --------------------
		// init
		if(NULL != V_GrP5SkinMng)
		{
			delete	V_GrP5SkinMng;
			V_GrP5SkinMng	=	NULL;
		}

		switch(V_GrP5SkinCfg.SkinId)
		{
			case 0:
				V_GrP5SkinMng	=	(Cls_GrP5SkinBase*)new Cls_GrP5SkinT2(V_GrP5SkinCfg.SkinId);
				break;
			case 1:
				V_GrP5SkinMng	=	(Cls_GrP5SkinBase*)new Cls_GrP5SkinT1(V_GrP5SkinCfg.SkinId);
				break;
			default:
				V_GrP5SkinMng	=	(Cls_GrP5SkinBase*)new Cls_GrP5SkinT1(V_GrP5SkinCfg.SkinId);
				DbgMsgPrint("GrP5SkinSetByCfg - bad skin id (%d)\n", V_GrP5SkinCfg.SkinId);
				break;
		}
}
//--------------------------------------------------------------------
Cls_GrP5SkinBase*	GrP5SkinGet(void)
{
		return	V_GrP5SkinMng;
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinBase::CtlGetItemOneRect(void* A_ObjUi, __s32 A_ItmIdxX, __s32 A_ItmIdxY, Ptr_GrRect A_PtrRtRect)
{
	// local -------------------
	// code --------------------
		CtlGetItemRangeRect(A_ObjUi, A_ItmIdxX, A_ItmIdxY, A_ItmIdxX, A_ItmIdxY, A_PtrRtRect);
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinBase::ReqPntGdib(Ptr_GrGdib A_PtrGdib, Ptr_GrRect A_PtrRcScr, __u8 A_AlignHz, __u8 A_AlignVt, BOOL8 A_IsSprite)
{
	// local -------------------
		__s32	Tv_DrawX;
		__s32	Tv_DrawY;
	// code --------------------
		// calculate draw position
		Tv_DrawX	=	A_PtrRcScr->left;
		Tv_DrawY	=	A_PtrRcScr->top;

		if(E_GrAlignCenter == A_AlignHz)
		{
			Tv_DrawX	=	(((A_PtrRcScr->right - A_PtrRcScr->left) - (__s32)A_PtrGdib->ResX) >> 1) + A_PtrRcScr->left;
		}
		else if(E_GrAlignRight == A_AlignHz)
		{
			Tv_DrawX	=	A_PtrRcScr->right - (__s32)A_PtrGdib->ResX;
		}

		if(E_GrAlignCenter == A_AlignVt)
		{
			Tv_DrawY	=	(((A_PtrRcScr->bottom - A_PtrRcScr->top) - (__s32)A_PtrGdib->ResY) >> 1) + A_PtrRcScr->top;
		}
		else if(E_GrAlignBottom == A_AlignHz)
		{
			Tv_DrawY	=	A_PtrRcScr->bottom - (__s32)A_PtrGdib->ResY;
		}

		// draw
		GrP5PntReqImage(A_PtrGdib, Tv_DrawX, Tv_DrawY, A_IsSprite);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinBase::ReqPntRectangle(Ptr_GrRect A_PtrRcScr, __u32 A_Width, __u32 A_Color)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		// check able
		if(0 == A_Width)
		{
			return;
		}
		// top
		Tv_Rc.left		=	A_PtrRcScr->left;
		Tv_Rc.top			=	A_PtrRcScr->top;
		Tv_Rc.right		=	A_PtrRcScr->right;
		Tv_Rc.bottom	=	A_PtrRcScr->top + (__s32)A_Width;
		GrP5PntReqFillRect(&Tv_Rc, A_Color);

		// bottom
		Tv_Rc.top			=	A_PtrRcScr->bottom - (__s32)A_Width;
		Tv_Rc.bottom	=	A_PtrRcScr->bottom;
		GrP5PntReqFillRect(&Tv_Rc, A_Color);

		// left
		Tv_Rc.left		=	A_PtrRcScr->left;
		Tv_Rc.top			=	A_PtrRcScr->top + (__s32)A_Width;
		Tv_Rc.right		=	A_PtrRcScr->left + (__s32)A_Width;
		Tv_Rc.bottom	=	A_PtrRcScr->bottom - (__s32)A_Width;
		if(Tv_Rc.top >= Tv_Rc.bottom)
		{
			return;
		}
		GrP5PntReqFillRect(&Tv_Rc, A_Color);

		// right
		Tv_Rc.left		=	A_PtrRcScr->right - (__s32)A_Width;
		Tv_Rc.right		=	A_PtrRcScr->right;
		GrP5PntReqFillRect(&Tv_Rc, A_Color);

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinBase::CtlGetSizeByItemCnt(__u16 A_ClassId, __u32 A_ItemCnt, __u32* A_PtrRtSizeX, __u32* A_PtrRtSizeY)
{
		*A_PtrRtSizeX	=	0;
		*A_PtrRtSizeY	=	0;
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinBase::CtlCalcSbarVtByValue(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, __u32 A_Value)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinBase::CtlCalcSbarVtByPos(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, __u32 A_Pos)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinBase::CtlGetSbarVtThumbRc(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, Ptr_GrRect A_PtrRtRc)
{
		A_PtrRtRc->right	=	0;
		if(0 >= A_PtrSbarCtl->RcTrack.right)
		{
			return;
		}

		A_PtrRtRc->left		=	A_PtrSbarCtl->RcTrack.left;
		A_PtrRtRc->right	=	A_PtrSbarCtl->RcTrack.right;
		A_PtrRtRc->top		=	A_PtrSbarCtl->RcTrack.top + A_PtrSbarCtl->Pos;
		A_PtrRtRc->bottom	=	A_PtrRtRc->top + A_PtrSbarCtl->ThumbSize;
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinBase::CtlCalcSbarHzByValue(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, __u32 A_Value)
{
	
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinBase::CtlCalcSbarHzByPos(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, __u32 A_Pos)
{
	
}
//--------------------------------------------------------------------
void	Cls_GrP5SkinBase::CtlGetSbarHzThumbRc(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, Ptr_GrRect A_PtrRtRc)
{
		A_PtrRtRc->right	=	0;
		if(0 >= A_PtrSbarCtl->RcTrack.right)
		{
			return;
		}

		A_PtrRtRc->top		=	A_PtrSbarCtl->RcTrack.top;
		A_PtrRtRc->bottom	=	A_PtrSbarCtl->RcTrack.bottom;
		A_PtrRtRc->left		=	A_PtrSbarCtl->RcTrack.left + A_PtrSbarCtl->Pos;
		A_PtrRtRc->right	=	A_PtrRtRc->left + A_PtrSbarCtl->ThumbSize;

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinBase::TooltipSetup(Ptr_GrRect A_PtrConRect, Ptr_GrRect A_PtrBaseRect, __u16 A_TxtId, Ptr_GrP5SkinTooltipCtl A_PtrRtCtl)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5SkinBase::TooltipDraw(Ptr_GrP5SkinTooltipCtl A_PtrCtl)
{

}
//--------------------------------------------------------------------
