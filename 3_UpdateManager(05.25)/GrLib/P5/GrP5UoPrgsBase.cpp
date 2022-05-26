/*
Platform 5 UI object Label base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoPrgsBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5Mng.h>

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

#define	E_GrP5UoPrgsBaseParaInfoCnt							7

St_GrP5DvlScptParaInfo	V_GrP5UoPrgsBaseParaInfo[E_GrP5UoPrgsBaseParaInfoCnt]	=
{
	{ L"Progress base", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL },
	{ L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPrgsBasePara)0)->FontId, TRUE, 0, L"font id" },
	{L"IsDrawTxt", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoPrgsBasePara)0)->IsDrawTxt, TRUE, 0, L"draw text"},
	{L"IsPercentage", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoPrgsBasePara)0)->IsPercentage, TRUE, 0, L"text type is percentage"},
	{L"IsShowTotal", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoPrgsBasePara)0)->IsShowTotal, TRUE, 0, L"show total value"},
	{L"Direction", E_GrVarTypeDirection, 1, (void*)&((Ptr_GrP5UoPrgsBasePara)0)->Direction, TRUE, 0, L"Direction"},
	{L"SuffixTxtid", E_GrP5DvlValTypeTextId, 1, (void*)&((Ptr_GrP5UoPrgsBasePara)0)->SuffixTxtid, TRUE, 0, L"suffix text id"},
};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoPrgsBase::Cls_GrP5UoPrgsBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdPrgsBase, &m_ParaUoPrgsBase, sizeof(m_ParaUoPrgsBase), &m_SkinCtlPrgsBase);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoPrgsBase, sizeof(m_ParaUoPrgsBase));
		GrDumyZeroMem(&m_SkinCtlPrgsBase, sizeof(m_SkinCtlPrgsBase));

		m_ParaUoPrgsBase.TxtPara.Atb	=	E_GrP5FontAtbDrawText;
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoPrgsBase.TxtPara);
#endif

		// reset
		m_SkinCtlPrgsBase.ValMax		=	0;
		m_SkinCtlPrgsBase.ValNow		=	0;

		m_SkinCtlPrgsBase.StrTxt		=	GrP5TxtBufAlloc(m_ObjId);
		if(NULL != m_SkinCtlPrgsBase.StrTxt)
		{
			GrStrClear(m_SkinCtlPrgsBase.StrTxt);
		}

		// register timer
		GrP5TimerRegist(this);
}
//--------------------------------------------------------------------
Cls_GrP5UoPrgsBase::~Cls_GrP5UoPrgsBase(void)
{
	// local -------------------
	// code --------------------
		
		GrP5TxtBufFreeByObjId(m_ObjId);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPrgsBase::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPrgsBase::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		DvlParaInfoAdd(V_GrP5UoPrgsBaseParaInfo, E_GrP5UoPrgsBaseParaInfoCnt, &m_ParaUoPrgsBase);

		DvlParaInfoTxtParaAdd(L"Progress text para", &m_ParaUoPrgsBase.TxtPara);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPrgsBase::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPrgsBase::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoPrgsBase::PrgsValGet(__u32* A_PtrRtMax, __u32* A_PtrRtNow)
{
		*A_PtrRtMax	=	0;
		*A_PtrRtNow	=	0;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPrgsBase::PrgsTxtUpdate(void)
{
	// local -------------------
		__u32	Tv_Val;
		WCHAR*	Tv_StrWk;
	// code --------------------
		if(NULL == m_SkinCtlPrgsBase.StrTxt)
		{
			return;
		}

		GrStrClear(m_SkinCtlPrgsBase.StrTxt);
		
		// check max value
		if(0 != m_SkinCtlPrgsBase.ValMax)
		{
			if(m_ParaUoPrgsBase.IsPercentage)
			{
				// percentage display
				Tv_Val	=	(m_SkinCtlPrgsBase.ValNow * 100) / m_SkinCtlPrgsBase.ValMax;
				GrStrIntToWstr(m_SkinCtlPrgsBase.StrTxt, (__s32)Tv_Val);
				Tv_StrWk	=	GrStrAddWchar(m_SkinCtlPrgsBase.StrTxt,0x20 );
				Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x25);
			}
			else
			{
				GrStrIntToWstr(m_SkinCtlPrgsBase.StrTxt, (__s32)m_SkinCtlPrgsBase.ValNow);
				if(m_ParaUoPrgsBase.IsShowTotal)
				{
					Tv_StrWk	=	GrStrAddWchar(m_SkinCtlPrgsBase.StrTxt, 0x20);
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x2F);
					Tv_StrWk	=	GrStrAddWchar(Tv_StrWk, 0x20);
				}
				if(E_GrP5TxtIdNone != m_ParaUoPrgsBase.SuffixTxtid)
				{
					Tv_StrWk	=	GrStrAddWchar(m_SkinCtlPrgsBase.StrTxt, 0x20);
					Tv_StrWk	=	GrStrWcopy(Tv_StrWk, GrP5TextGet(m_ParaUoPrgsBase.SuffixTxtid),40);
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPrgsBase::EvtTimerTick(void)
{
	// local -------------------
		__u32	Tv_ValMax;
		__u32	Tv_ValNow;
	// code --------------------
		PrgsValGet(&Tv_ValMax, &Tv_ValNow);
		if((Tv_ValMax != m_SkinCtlPrgsBase.ValMax) || (Tv_ValNow != m_SkinCtlPrgsBase.ValNow))
		{
			// update value
			m_SkinCtlPrgsBase.ValMax	=	Tv_ValMax;
			m_SkinCtlPrgsBase.ValNow	=	Tv_ValNow;

			// update text
			PrgsTxtUpdate();
			// redraw
			ReqDraw(FALSE);
		}

}
//--------------------------------------------------------------------

