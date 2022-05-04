/*
Platform 5 UI object Quad base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoQuadBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5Mng.h>
#include <P5/GrP5Key.h>
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

#define	E_GrP5UoQuadBaseParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoQuadBaseParaInfo[E_GrP5UoQuadBaseParaInfoCnt]	=
{
	{ L"Quad base", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL },
	{ L"IsDrawLine", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoQuadBasePara)0)->IsDrawLine, TRUE, 0, L"draw divide line" },
	{L"ClrLine", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoQuadBasePara)0)->ClrLine, TRUE, 0, L"Color line"},
};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoQuadBase::Cls_GrP5UoQuadBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdQuadBase, &m_ParaUoQuadBase, sizeof(m_ParaUoQuadBase), NULL);

		// regist 
		V_GrP5Env.ConTbl[m_ConIdx].UiQuad	=	(void*)this;
		
		// default setting
		GrDumyZeroMem(&m_ParaUoQuadBase, sizeof(m_ParaUoQuadBase));
		m_ParaUoQuadBase.ClrLine	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		// reset

}
//--------------------------------------------------------------------
Cls_GrP5UoQuadBase::~Cls_GrP5UoQuadBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadBase::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoQuadBase::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		DvlParaInfoAdd(V_GrP5UoQuadBaseParaInfo, E_GrP5UoQuadBaseParaInfoCnt, &m_ParaUoQuadBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoQuadBase::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoQuadBase::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoQuadBase::IsKeyAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoQuadBase::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoQuadBase::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_ScrX;
		__s32	Tv_ScrY;
		__s8	Tv_Ch;
	// code --------------------
		Tv_Result	=	FALSE;

		// get channel
		Tv_ScrX		=	m_RcScr.left + A_X;
		Tv_ScrY		=	m_RcScr.top + A_Y;

		Tv_Ch	=	((Cls_GrP5ConBase*)V_GrP5Env.ConTbl[m_ConIdx].ObjCon)->QuadChGetByPos(Tv_ScrX, Tv_ScrY);
		if(0 <= Tv_Ch)
		{
			GrP5QuadModeExchg(m_ConIdx, (__u8)Tv_Ch);

			LcDoEvtQuadChgByUser();

			Tv_Result	=	TRUE;
		}

		if(Cls_GrP5UoBase::EvtMseLbtnUp(A_X,A_Y,A_BtnMap))
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadBase::EvtQuadChanged(void)
{
		//ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadBase::LcDoEvtQuadChgByUser(void)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			Tv_Ctrl->EvtQuadChgByUser(m_ParaUoBase.CtrlId, m_ObjId);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoQuadBase::PntQaudAddQuadView(__u8 A_Ch, Ptr_GrRect A_PtrRc)
{

}
//--------------------------------------------------------------------
