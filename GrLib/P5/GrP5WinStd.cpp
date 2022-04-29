/*
Platform 5 UI object base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5WinStd.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>

#ifdef GR_P5_EDITOR

#include <P5/Dvl/GrP5DvlBase.h>

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


WCHAR	V_GrP5WinStdStrNameLayer[]	=	L"Layer";

WCHAR	V_GrP5WinStdStrDescLayer[]		=	L"Window layer level";

#define	E_GrP5WinStdParaInfoCnt							1

St_GrP5DvlScptParaInfo	V_GrP5WinStdParaInfo[E_GrP5WinStdParaInfoCnt]	=
{
	{L"Window Standard", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
};


#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5WinStd::Cls_GrP5WinStd(__u16 A_ScptId, __u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect):
Cls_GrP5WinBase(A_ScptId, A_ConIdx, A_LayerIdx, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdWinStd, NULL, 0, NULL);


}
//--------------------------------------------------------------------
Cls_GrP5WinStd::~Cls_GrP5WinStd(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5WinStd::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5WinBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinStd::DvlParaInfoBuild(void)
{
	// local -------------------
		St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5WinBase::DvlParaInfoBuild();

		// add parametter
		DvlParaInfoAdd(V_GrP5WinStdParaInfo, E_GrP5WinStdParaInfoCnt, NULL);

		Tv_ParaInfo.StrName		=	V_GrP5WinStdStrNameLayer;
		Tv_ParaInfo.ValType		=	E_GrVarTypeU8;
		Tv_ParaInfo.ValCnt		=	1;
		Tv_ParaInfo.PtrVal		=	&m_DvlScptHdPara;
		Tv_ParaInfo.IsEdit		=	TRUE;
		Tv_ParaInfo.Act				=	E_GrP5DvlParaInfoActNone;
		Tv_ParaInfo.StrDesc		=	V_GrP5WinStdStrDescLayer;

		DvlParaInfoAdd(&Tv_ParaInfo, 1, NULL);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinStd::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

	// inherited
	Cls_GrP5WinBase::DvlParaInfoAction(A_Action);

	// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinStd::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------

	// delete prepare


	// inherited
	Cls_GrP5WinBase::DvlPreDelete();

}
#endif
//--------------------------------------------------------------------


