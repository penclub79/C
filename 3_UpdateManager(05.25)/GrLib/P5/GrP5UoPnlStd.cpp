/*
Platform 5 UI object Label base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoPnlStd.h>

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

#define	E_GrP5UoPnlStdParaInfoCnt							7

St_GrP5DvlScptParaInfo	V_GrP5UoPnlStdParaInfo[E_GrP5UoPnlStdParaInfoCnt]	=
{
	{L"Pannel Standard", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL},
	{L"Color", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlStdPara)0)->Color, TRUE, 0, L"Color"},
	{L"IsDrawBg", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoPnlStdPara)0)->IsDrawBg, TRUE, 0, L"Drawing Background"},
	{L"IsSprite", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoPnlStdPara)0)->IsSprite, TRUE, 0, L"Sprite image"},
	{L"ImgAlignHz", E_GrVarTypeAlignHz, 1, (void*)&((Ptr_GrP5UoPnlStdPara)0)->ImgAlignHz, TRUE, 0, L"Image align horizontal"},
	{L"ImgAlignVt", E_GrVarTypeAlignVt, 1, (void*)&((Ptr_GrP5UoPnlStdPara)0)->ImgAlignVt, TRUE, 0, L"Image align vertical"},
	{L"ImgId", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoPnlStdPara)0)->ImgId, TRUE, 0, L"Image ID"},
};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoPnlStd::Cls_GrP5UoPnlStd(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdPnlStd, &m_ParaUoPnlStd, sizeof(m_ParaUoPnlStd), NULL);

		// default setting
		GrDumyZeroMem(&m_ParaUoPnlStd, sizeof(m_ParaUoPnlStd));

		m_ParaUoPnlStd.Color		=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlStd.IsDrawBg	=	TRUE;

		m_ParaUoPnlStd.ImgRc.Left.Ratio		=	0;
		m_ParaUoPnlStd.ImgRc.Top.Ratio			=	0;
		m_ParaUoPnlStd.ImgRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoPnlStd.ImgRc.Bottom.Ratio	=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoPnlStd.ImgRc.Left.Val			=	0;
		m_ParaUoPnlStd.ImgRc.Top.Val				=	0;
		m_ParaUoPnlStd.ImgRc.Right.Val			=	0;
		m_ParaUoPnlStd.ImgRc.Bottom.Val		=	0;

		// reset

}
//--------------------------------------------------------------------
Cls_GrP5UoPnlStd::~Cls_GrP5UoPnlStd(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlStd::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlStd::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// separator basic info
		DvlParaInfoAdd(V_GrP5UoPnlStdParaInfo, E_GrP5UoPnlStdParaInfoCnt, &m_ParaUoPnlStd);

		DvlParaInfoRelPosAdd(L"Image Rect", &m_ParaUoPnlStd.ImgRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlStd::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		// custom action
		ReqDraw(FALSE);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlStd::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
__u16	Cls_GrP5UoPnlStd::ImageIdGet(void)
{
		return	m_ParaUoPnlStd.ImgId;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlStd::ImageIdSet(__u16 A_ImgId)
{
	// local -------------------
	// code --------------------
		if(m_ParaUoPnlStd.ImgId != A_ImgId)
		{
			m_ParaUoPnlStd.ImgId	=	A_ImgId;
			ReqDraw(FALSE);
		}
		
}
//--------------------------------------------------------------------

