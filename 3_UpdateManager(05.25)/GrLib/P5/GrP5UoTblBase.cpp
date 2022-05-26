/*
Platform 5 UI list base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoTblBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>
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
#define	E_GrP5UoTblBaseParaInfoCnt							102

St_GrP5DvlScptParaInfo	V_GrP5UoTblBaseParaInfo[E_GrP5UoTblBaseParaInfoCnt]	=
{
	{L"Table base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->FontId, TRUE, E_GrP5DvlParaInfoActDefault, L"Font ID"},
	{L"ItemHeight", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ItemHeight, TRUE, E_GrP5DvlParaInfoActDefault, L"Item Height"},
	{L"ItemMaxCnt", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ItemMaxCnt, TRUE, 0, L"Item max count"},
	{L"ColumCnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColumCnt, TRUE, 0, L"Colum count"},
	{L"IsLineSel", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsLineSel, TRUE, 0, L"Line unit select"},
	{L"ColSize 0", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[0], TRUE, 0, L"Colum width"},
	{L"ColSize 1", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[1], TRUE, 0, L"Colum width"},
	{L"ColSize 2", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[2], TRUE, 0, L"Colum width"},
	{L"ColSize 3", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[3], TRUE, 0, L"Colum width"},
	{L"ColSize 4", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[4], TRUE, 0, L"Colum width"},
	{L"ColSize 5", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[5], TRUE, 0, L"Colum width"},
	{L"ColSize 6", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[6], TRUE, 0, L"Colum width"},
	{L"ColSize 7", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[7], TRUE, 0, L"Colum width"},
	{L"ColSize 8", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[8], TRUE, 0, L"Colum width"},
	{L"ColSize 9", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[9], TRUE, 0, L"Colum width"},
	{L"ColSize 10", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[10], TRUE, 0, L"Colum width"},
	{L"ColSize 11", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[11], TRUE, 0, L"Colum width"},
	{L"ColSize 12", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[12], TRUE, 0, L"Colum width"},
	{L"ColSize 13", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[13], TRUE, 0, L"Colum width"},
	{L"ColSize 14", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[14], TRUE, 0, L"Colum width"},
	{L"ColSize 15", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->ColSize[15], TRUE, 0, L"Colum width"},
	{L"IsFixPitch 0", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[0], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 1", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[1], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 2", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[2], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 3", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[3], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 4", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[4], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 5", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[5], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 6", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[6], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 7", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[7], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 8", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[8], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 9", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[9], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 10", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[10], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 11", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[11], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 12", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[12], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 13", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[13], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 14", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[14], TRUE, 0, L"Fix font pitch"},
	{L"IsFixPitch 15", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->IsFixPitch[15], TRUE, 0, L"Fix font pitch"},

	{L"CharPitch 0", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[0], TRUE, 0, L"char pitch"},
	{L"CharPitch 1", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[1], TRUE, 0, L"char pitch"},
	{L"CharPitch 2", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[2], TRUE, 0, L"char pitch"},
	{L"CharPitch 3", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[3], TRUE, 0, L"char pitch"},
	{L"CharPitch 4", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[4], TRUE, 0, L"char pitch"},
	{L"CharPitch 5", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[5], TRUE, 0, L"char pitch"},
	{L"CharPitch 6", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[6], TRUE, 0, L"char pitch"},
	{L"CharPitch 7", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[7], TRUE, 0, L"char pitch"},
	{L"CharPitch 8", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[8], TRUE, 0, L"char pitch"},
	{L"CharPitch 9", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[9], TRUE, 0, L"char pitch"},
	{L"CharPitch 10", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[10], TRUE, 0, L"char pitch"},
	{L"CharPitch 11", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[11], TRUE, 0, L"char pitch"},
	{L"CharPitch 12", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[12], TRUE, 0, L"char pitch"},
	{L"CharPitch 13", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[13], TRUE, 0, L"char pitch"},
	{L"CharPitch 14", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[14], TRUE, 0, L"char pitch"},
	{L"CharPitch 15", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->CharPitch[15], TRUE, 0, L"char pitch"},

	{L"GapHz 0", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[0], TRUE, 0, L"gap horizontal"},
	{L"GapHz 1", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[1], TRUE, 0, L"gap horizontal"},
	{L"GapHz 2", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[2], TRUE, 0, L"gap horizontal"},
	{L"GapHz 3", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[3], TRUE, 0, L"gap horizontal"},
	{L"GapHz 4", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[4], TRUE, 0, L"gap horizontal"},
	{L"GapHz 5", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[5], TRUE, 0, L"gap horizontal"},
	{L"GapHz 6", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[6], TRUE, 0, L"gap horizontal"},
	{L"GapHz 7", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[7], TRUE, 0, L"gap horizontal"},
	{L"GapHz 8", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[8], TRUE, 0, L"gap horizontal"},
	{L"GapHz 9", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[9], TRUE, 0, L"gap horizontal"},
	{L"GapHz 10", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[10], TRUE, 0, L"gap horizontal"},
	{L"GapHz 11", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[11], TRUE, 0, L"gap horizontal"},
	{L"GapHz 12", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[12], TRUE, 0, L"gap horizontal"},
	{L"GapHz 13", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[13], TRUE, 0, L"gap horizontal"},
	{L"GapHz 14", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[14], TRUE, 0, L"gap horizontal"},
	{L"GapHz 15", E_GrVarTypeS8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->GapHz[15], TRUE, 0, L"gap horizontal"},

	{L"TxtMgnX 0", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[0], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 1", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[1], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 2", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[2], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 3", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[3], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 4", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[4], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 5", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[5], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 6", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[6], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 7", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[7], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 8", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[8], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 9", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[9], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 10", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[10], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 11", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[11], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 12", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[12], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 13", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[13], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 14", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[14], TRUE, 0, L"text margin x"},
	{L"TxtMgnX 15", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnX[15], TRUE, 0, L"text margin x"},

	{L"TxtMgnY 0", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[0], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 1", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[1], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 2", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[2], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 3", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[3], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 4", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[4], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 5", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[5], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 6", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[6], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 7", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[7], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 8", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[8], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 9", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[9], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 10", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[10], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 11", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[11], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 12", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[12], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 13", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[13], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 14", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[14], TRUE, 0, L"text margin y"},
	{L"TxtMgnY 15", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblBasePara)0)->TxtMgnY[15], TRUE, 0, L"text margin y"},
	
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoTblBase::Cls_GrP5UoTblBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdTblBase, &m_ParaUoTblBase, sizeof(m_ParaUoTblBase), &m_SkinCtlTblBase);

		// default setting
		GrDumyZeroMem(&m_ParaUoTblBase, sizeof(m_ParaUoTblBase));
		GrDumyZeroMem(&m_SkinCtlTblBase, sizeof(m_SkinCtlTblBase));
		m_ParaUoTblBase.ItemMaxCnt		=	E_GrP5TblItemDfltMaxCnt;
		m_ParaUoTblBase.ItemHeight		=	E_GrP5TblItemDfltHeight;
		m_ParaUoTblBase.TxtPara.Atb	=	E_GrP5FontAtbDrawText;

		m_SkinCtlTblBase.CusX		=	-1;
		m_SkinCtlTblBase.CusY		=	0;

		m_SkinCtlTblBase.SelX		=	-1;
		m_SkinCtlTblBase.SelY		=	-1;

		m_EvtHostId		=	E_GrP5ObjIdNone;


#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoTblBase.TxtPara);
#endif

		m_ParaUoTblBase.ColumCnt			=	1;

#ifdef GR_P5_EDITOR
		m_SkinCtlTblBase.ItemCnt	=	m_ParaUoTblBase.ItemMaxCnt;
		if(E_GrP5SkinTblLineMaxCnt <= m_SkinCtlTblBase.ItemCnt)
		{
			m_SkinCtlTblBase.ItemCnt	=	E_GrP5SkinTblLineMaxCnt;
		}
#endif

}
//--------------------------------------------------------------------
Cls_GrP5UoTblBase::~Cls_GrP5UoTblBase(void)
{
	// local -------------------
	// code --------------------

		
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoTblBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoTblBaseParaInfo, E_GrP5UoTblBaseParaInfoCnt, &m_ParaUoTblBase);
		DvlParaInfoTxtParaAdd(L"Text parametter", &m_ParaUoTblBase.TxtPara);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoTblBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------
		// style rebuild

#ifdef GR_P5_EDITOR
		m_SkinCtlTblBase.ItemCnt		=	m_ParaUoTblBase.ItemMaxCnt;
#endif

		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		ReqDraw(FALSE);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
#ifdef GR_P5_EDITOR
		m_SkinCtlTblBase.ItemCnt		=	m_ParaUoTblBase.ItemMaxCnt;
#endif

		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::IsKeyAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::EvtKeyFocusGet(__u32 A_Para)
{
		Cls_GrP5UoBase::EvtKeyFocusGet(A_Para);
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::EvtKeyFocusLoss(void)
{
		Cls_GrP5UoBase::EvtKeyFocusLoss();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::EvtMseEnter(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::EvtMseLeave(void)
{
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ItemIdxGetByPos(__s32 A_ScrX, __s32 A_ScrY, __s32* A_PtrRtLine, __s32* A_PtrRtColum)
{
	// local -------------------
	// code --------------------
		if(NULL == m_SkinMng)
		{
			return	FALSE;
		}

		m_SkinMng->CtlGetItemIdxByPos(this, A_ScrX, A_ScrY, A_PtrRtColum, A_PtrRtLine);

		if((0 > (*A_PtrRtLine)) && (0 > (*A_PtrRtColum)))
		{
			return	FALSE;
		}

		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ItemRectGetByIdx(__u32 A_Line, __s32 A_Colum, Ptr_GrRect A_PtrRc)
{
	// local -------------------
		
	// code --------------------
		
		if(NULL == m_SkinMng)
		{
			return	FALSE;
		}
		
		if(0 > A_Colum)
		{
			m_SkinMng->CtlGetItemRangeRect(this, 0, (__s32)A_Line, (__s32)m_ParaUoTblBase.ColumCnt - 1, (__s32)A_Line, A_PtrRc);
		}
		else
		{
			m_SkinMng->CtlGetItemOneRect(this, A_Colum, (__s32)A_Line, A_PtrRc);
		}

		if(0 >= A_PtrRc->right)
		{
			return	FALSE;
		}

		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32	Tv_ScrX;
		__s32	Tv_ScrY;
		__s32	Tv_Line;
		__s32	Tv_Colum;
		__s32	Tv_OldX;
		St_GrRect	Tv_Rc;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::EvtMseMove(A_X, A_Y, A_BtnMap);

		// check item
		Tv_ScrX		=	m_RcScr.left + A_X;
		Tv_ScrY		=	m_RcScr.top + A_Y;
		if(ItemIdxGetByPos(Tv_ScrX, Tv_ScrY, &Tv_Line, &Tv_Colum))
		{
			// check line mode
			Tv_OldX		=	m_SkinCtlTblBase.CusX;
			if(m_ParaUoTblBase.IsLineSel)
			{
				Tv_Colum	=	-1;
				Tv_OldX		=	-1;
			}
			// check changed
			if((m_SkinCtlTblBase.CusX != Tv_Colum) || (m_SkinCtlTblBase.CusY != Tv_Line))
			{
				// redraw old cursor position
				if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.CusY, Tv_OldX, &Tv_Rc))
				{
					LcPntReqByRect(&Tv_Rc);
				}

				// update cursor position
				m_SkinCtlTblBase.CusX		=	Tv_Colum;
				m_SkinCtlTblBase.CusY		=	Tv_Line;

				// draw new cursor position
				if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.CusY, m_SkinCtlTblBase.CusX, &Tv_Rc))
				{
					LcPntReqByRect(&Tv_Rc);
				}
			}
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		//Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
		__s32	Tv_ScrX;
		__s32	Tv_ScrY;
		__s32	Tv_Line;
		__s32	Tv_Colum;
		__s32	Tv_OldX;
		St_GrRect	Tv_Rc;
	// code --------------------
		// check item
		Tv_ScrX		=	m_RcScr.left + A_X;
		Tv_ScrY		=	m_RcScr.top + A_Y;
		if(ItemIdxGetByPos( Tv_ScrX,Tv_ScrY,&Tv_Line,&Tv_Colum ))
		{
			// check line mode
			Tv_OldX		=	m_SkinCtlTblBase.SelX;
			if(m_ParaUoTblBase.IsLineSel)
			{
				Tv_Colum	=	-1;
				Tv_OldX		=	-1;
			}
			// redraw old cursor position
			if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.SelY, Tv_OldX, &Tv_Rc))
			{
				LcPntReqByRect(&Tv_Rc);
			}

			// update cursor position
			m_SkinCtlTblBase.CusX		=	Tv_Colum;
			m_SkinCtlTblBase.CusY		=	Tv_Line;

			// update select
			m_SkinCtlTblBase.SelX		=	m_SkinCtlTblBase.CusX;
			m_SkinCtlTblBase.SelY		=	m_SkinCtlTblBase.CusY;

			// draw new cursor position
			if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.SelY, m_SkinCtlTblBase.SelX, &Tv_Rc))
			{
				LcPntReqByRect(&Tv_Rc);
			}

			NotifyItemSel();
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::ItemClear(void)
{
	// local -------------------
	// code --------------------
		m_SkinCtlTblBase.ItemCnt	=	0;
		m_SkinCtlTblBase.SelX			=	-1;
		m_SkinCtlTblBase.SelY			=	-1;

		LcSkinCtlUpdate();

		// redraw
		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::ItemMaxCountSet(__u32 A_MaxCnt)
{
	// local -------------------
	// code --------------------
		if(0 != A_MaxCnt)
		{
			m_ParaUoTblBase.ItemMaxCnt	=	A_MaxCnt;
			LcSkinCtlUpdate();
			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::NotifyItemSel(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_ObjHost;
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------
		// event
		Tv_ObjHost	=	NULL;
		if(E_GrP5ObjIdNone == m_EvtHostId)
		{
			Tv_ObjHost	=	m_ObjPrnt;
		}
		else
		{
			Tv_ObjHost	=	(Cls_GrP5UoBase*)GrP5UiObjGet(m_EvtHostId);
		}
		if(NULL != Tv_ObjHost)
		{
			//Tv_ObjHost->EvtListItemSel(m_ObjId,m_SelIdx);
		}

		// control event
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			Tv_Ctrl->EvtTblItemSel(m_ParaUoBase.CtrlId, m_ObjId,(__u32)m_SkinCtlTblBase.SelY,m_SkinCtlTblBase.SelX);
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::EvtKeyDown(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		switch(A_Key)
		{
			case E_GrP5KeyUp:
				Tv_Result	=	ProcKeyUp();
				break;
			case E_GrP5KeyDown:
				Tv_Result	=	ProcKeyDown();
				break;
		}

		if(!Tv_Result)
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyDown(A_Key);
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		switch(A_Key)
		{
			case E_GrP5KeyEnter:
				Tv_Result	=	ProcKeyEnter();
				break;
		}

		if(!Tv_Result)
		{
			Tv_Result	=	Cls_GrP5UoBase::EvtKeyUp(A_Key);
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ProcKeyEnter(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;

		if(0 <= m_SkinCtlTblBase.CusY)
		{
			// draw old cursor
			if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.SelY, m_SkinCtlTblBase.SelX,&Tv_Rc))
			{
				LcPntReqByRect(&Tv_Rc);
			}

			// change select
			m_SkinCtlTblBase.SelX		=	m_SkinCtlTblBase.CusX;
			m_SkinCtlTblBase.SelY		=	m_SkinCtlTblBase.CusY;

			if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.SelY, m_SkinCtlTblBase.SelX, &Tv_Rc))
			{
				LcPntReqByRect(&Tv_Rc);
			}

			NotifyItemSel();
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ProcKeyUp(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_CusX;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;

		if(0 < m_SkinCtlTblBase.CusY)
		{
			// draw old
			Tv_CusX	=	m_SkinCtlTblBase.CusX;
			if(m_ParaUoTblBase.IsLineSel)
			{
				Tv_CusX	=	-1;
			}
			if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.CusY,Tv_CusX,&Tv_Rc))
			{
				LcPntReqByRect(&Tv_Rc);
			}
			m_SkinCtlTblBase.CusY --;
			if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.CusY, Tv_CusX, &Tv_Rc))
			{
				LcPntReqByRect(&Tv_Rc);
			}

			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ProcKeyDown(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32	Tv_CusX;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;

		if((0 <= m_SkinCtlTblBase.CusY) && ((__s32)m_SkinCtlTblBase.ItemCnt > (m_SkinCtlTblBase.CusY + 1)))
		{
			// draw old
			Tv_CusX	=	m_SkinCtlTblBase.CusX;
			if(m_ParaUoTblBase.IsLineSel)
			{
				Tv_CusX	=	-1;
			}
			if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.CusY, Tv_CusX, &Tv_Rc))
			{
				LcPntReqByRect(&Tv_Rc);
			}
			m_SkinCtlTblBase.CusY ++;
			if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.CusY, Tv_CusX, &Tv_Rc))
			{
				LcPntReqByRect(&Tv_Rc);
			}

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ProcKeyLeft(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;

		if(!m_ParaUoTblBase.IsLineSel)
		{
			if((0 < m_SkinCtlTblBase.CusX) && (0 <= m_SkinCtlTblBase.CusY))
			{
				// draw old
				if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.CusY, m_SkinCtlTblBase.CusX, &Tv_Rc))
				{
					LcPntReqByRect(&Tv_Rc);
				}
				m_SkinCtlTblBase.CusX --;
				if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.CusY, m_SkinCtlTblBase.CusX, &Tv_Rc))
				{
					LcPntReqByRect(&Tv_Rc);
				}

				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ProcKeyRight(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;

		if(!m_ParaUoTblBase.IsLineSel)
		{
			if((0 <= m_SkinCtlTblBase.CusX) && ((m_SkinCtlTblBase.CusX + 1) < (__s32)m_SkinCtlTblBase.ColumCnt) && (0 <= m_SkinCtlTblBase.CusY))
			{
				// draw old
				if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.CusY, m_SkinCtlTblBase.CusX, &Tv_Rc))
				{
					LcPntReqByRect(&Tv_Rc);
				}
				m_SkinCtlTblBase.CusX ++;
				if(ItemRectGetByIdx((__u32)m_SkinCtlTblBase.CusY, m_SkinCtlTblBase.CusX, &Tv_Rc))
				{
					LcPntReqByRect(&Tv_Rc);
				}

				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::EventHostIdSet(__u16 A_ObjId)
{
		m_EvtHostId	=	A_ObjId;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::CursorPosGet(__s32* A_PtrRtCusX, __s32* A_PtrRtCusY)
{
	// local -------------------
	// code --------------------
		*A_PtrRtCusX		=	m_SkinCtlTblBase.CusX;
		*A_PtrRtCusY		=	m_SkinCtlTblBase.CusY;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::CursorPosSet(__s32 A_CusX, __s32 A_CusY)
{
	// local -------------------
		__s32	Tv_OldX;
		St_GrRect	Tv_Rc;
	// code --------------------
		
		Tv_OldX		=	m_SkinCtlTblBase.CusX;
		if(m_ParaUoTblBase.IsLineSel)
		{
			Tv_OldX		=	-1;
		}

		if(ItemRectGetByIdx((__s32)m_SkinCtlTblBase.CusY,Tv_OldX,&Tv_Rc))
		{
			LcPntReqByRect(&Tv_Rc);
		}

		m_SkinCtlTblBase.CusX		=	A_CusX;
		m_SkinCtlTblBase.CusY		=	A_CusY;
		if(m_ParaUoTblBase.IsLineSel)
		{
			m_SkinCtlTblBase.CusX		=	-1;
		}

		if(ItemRectGetByIdx((__s32)m_SkinCtlTblBase.CusY, m_SkinCtlTblBase.CusX, &Tv_Rc))
		{
			LcPntReqByRect(&Tv_Rc);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::SelectPosGet(__s32* A_PtrRtSelX, __s32* A_PtrRtSelY)
{
	// local -------------------
	// code --------------------
		*A_PtrRtSelX	=	m_SkinCtlTblBase.SelX;
		*A_PtrRtSelY	=	m_SkinCtlTblBase.SelY;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoTblBase::SelectPosSet(__s32 A_SelX, __s32 A_SelY)
{
	// local -------------------
		__s32	Tv_OldX;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_OldX		=	m_SkinCtlTblBase.SelX;
		if(m_ParaUoTblBase.IsLineSel)
		{
			Tv_OldX		=	-1;
		}

		if(ItemRectGetByIdx((__s32)m_SkinCtlTblBase.SelY, Tv_OldX, &Tv_Rc))
		{
			LcPntReqByRect(&Tv_Rc);
		}

		m_SkinCtlTblBase.SelX		=	A_SelX;
		m_SkinCtlTblBase.SelY		=	A_SelY;
		if(m_ParaUoTblBase.IsLineSel)
		{
			m_SkinCtlTblBase.SelX		=	-1;
		}

		if(ItemRectGetByIdx((__s32)m_SkinCtlTblBase.SelY, m_SkinCtlTblBase.SelX, &Tv_Rc))
		{
			LcPntReqByRect(&Tv_Rc);
		}

}
//--------------------------------------------------------------------
__s32	Cls_GrP5UoTblBase::ItemAdd(void)
{
	// local -------------------
		__s32	Tv_Result;
		__u32	Tv_ColIdx;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	-1;

		if(m_ParaUoTblBase.ItemMaxCnt > m_SkinCtlTblBase.ItemCnt)
		{
			// reset
			//GrDumyZeroMem(&m_SkinCtlTblBase.Lines[m_SkinCtlTblBase.ItemCnt], sizeof(St_GrP5UoTblLineCtrl));
			for(Tv_ColIdx = 0; Tv_ColIdx < E_GrP5SkinTblColumMaxCnt; Tv_ColIdx++)
			{
				m_SkinCtlTblBase.Lines[m_SkinCtlTblBase.ItemCnt].Cols[Tv_ColIdx].AlignHz	=	0;
				m_SkinCtlTblBase.Lines[m_SkinCtlTblBase.ItemCnt].Cols[Tv_ColIdx].AlignVt	=	0;
				m_SkinCtlTblBase.Lines[m_SkinCtlTblBase.ItemCnt].Cols[Tv_ColIdx].Para			=	0;
				m_SkinCtlTblBase.Lines[m_SkinCtlTblBase.ItemCnt].Cols[Tv_ColIdx].RsrcId		=	0;
				m_SkinCtlTblBase.Lines[m_SkinCtlTblBase.ItemCnt].Cols[Tv_ColIdx].Stat			=	0;
				m_SkinCtlTblBase.Lines[m_SkinCtlTblBase.ItemCnt].Cols[Tv_ColIdx].Type			=	0;
				m_SkinCtlTblBase.Lines[m_SkinCtlTblBase.ItemCnt].Cols[Tv_ColIdx].UserData	=	0;
				m_SkinCtlTblBase.Lines[m_SkinCtlTblBase.ItemCnt].Cols[Tv_ColIdx].Value		=	0;
			}

			// result 
			Tv_Result	=	(__s32)m_SkinCtlTblBase.ItemCnt;
			// add
			m_SkinCtlTblBase.ItemCnt ++;
			LcSkinCtlUpdate();
			// redraw
			if(ItemRectGetByIdx((__u32)Tv_Result, -1, &Tv_Rc))
			{
				LcPntReqByRect(&Tv_Rc);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ItemSetTextLocal(__u32 A_Line, __u32 A_Colum, WCHAR* A_StrTxt, __u8 A_AlignHz, __u8 A_AlignVt)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;

		if( (m_SkinCtlTblBase.ItemCnt > A_Line) && (m_SkinCtlTblBase.ColumCnt > A_Colum) )
		{
			if(NULL != m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Str)
			{
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Type			=	E_GrP5TblCtlTextLocal;
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].AlignHz		=	A_AlignHz;
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].AlignVt		=	A_AlignVt;
				GrStrWcopy(m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Str, A_StrTxt, E_GrP5TblTextMaxLen);

				// redraw
				if(ItemRectGetByIdx(A_Line, (__s32)A_Colum, &Tv_Rc))
				{
					LcPntReqByRect(&Tv_Rc);
				}

				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ItemSetImage(__u32 A_Line, __u32 A_Colum, __u16 A_ImgId, BOOL8 A_IsSprite, __u8 A_AlignHz, __u8 A_AlignVt)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;

		if((m_SkinCtlTblBase.ItemCnt > A_Line) && (m_SkinCtlTblBase.ColumCnt > A_Colum))
		{
			m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Type			=	E_GrP5TblCtlImage;
			m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].RsrcId		=	A_ImgId;
			m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].AlignHz		=	A_AlignHz;
			m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].AlignVt		=	A_AlignVt;
			m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Para			=	(__u8)A_IsSprite;

			// redraw
			if(ItemRectGetByIdx(A_Line, (__s32)A_Colum, &Tv_Rc))
			{
				LcPntReqByRect(&Tv_Rc);
			}

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ItemSetTextId(__u32 A_Line, __u32 A_Colum, __u16 A_TxtId, __u8 A_AlignHz, __u8 A_AlignVt)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;

		if((m_SkinCtlTblBase.ItemCnt > A_Line) && (m_SkinCtlTblBase.ColumCnt > A_Colum))
		{
			m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Type			=	E_GrP5TblCtlTextId;
			m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].RsrcId		=	A_TxtId;
			m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].AlignHz		=	A_AlignHz;
			m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].AlignVt		=	A_AlignVt;

			// redraw
			if(ItemRectGetByIdx(A_Line, (__s32)A_Colum, &Tv_Rc))
			{
				LcPntReqByRect(&Tv_Rc);
			}

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ItemSetValDec(__u32 A_Line, __u32 A_Colum, __u32 A_Value, __u8 A_Digit, __u8 A_AlignHz, __u8 A_AlignVt)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsFix;
		__u32	Tv_Lim;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;

		if((m_SkinCtlTblBase.ItemCnt > A_Line) && (m_SkinCtlTblBase.ColumCnt > A_Colum))
		{
			if(NULL != m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Str)
			{
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Type			=	E_GrP5TblCtlValue;
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].AlignHz		=	A_AlignHz;
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].AlignVt		=	A_AlignVt;
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Value			=	A_Value;

				Tv_IsFix	=	FALSE;
				Tv_Lim		=	(__u32)A_Digit;
				if(0 != Tv_Lim)
				{
					Tv_IsFix	=	TRUE;
				}
				GrStrIntToWstr(m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Str, (__s32)A_Value, Tv_Lim, Tv_IsFix);

				// redraw
				if(ItemRectGetByIdx(A_Line, (__s32)A_Colum, &Tv_Rc))
				{
					LcPntReqByRect(&Tv_Rc);
				}

				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ItemSetValHex(__u32 A_Line, __u32 A_Colum, __u32 A_Value, __u8 A_Digit, __u8 A_AlignHz, __u8 A_AlignVt)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsFix;
		__u32	Tv_Lim;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;

		if((m_SkinCtlTblBase.ItemCnt > A_Line) && (m_SkinCtlTblBase.ColumCnt > A_Colum))
		{
			if(NULL != m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Str)
			{
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Type			=	E_GrP5TblCtlValue;
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].AlignHz		=	A_AlignHz;
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].AlignVt		=	A_AlignVt;
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Value			=	A_Value;

				Tv_IsFix	=	FALSE;
				Tv_Lim		=	(__u32)A_Digit;
				if(0 != Tv_Lim)
				{
					Tv_IsFix	=	TRUE;
				}
				GrStrDwordToHexWstr(m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Str, A_Value, Tv_Lim, Tv_IsFix);

				// redraw
				if(ItemRectGetByIdx(A_Line, (__s32)A_Colum, &Tv_Rc))
				{
					LcPntReqByRect(&Tv_Rc);
				}

				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoTblBase::ItemSetTime(__u32 A_Line, __u32 A_Colum, Def_GrTime A_Time, BOOL8 A_IsNoTime, BOOL8 A_IsNoDate,
	BOOL8 A_IsNoSec , __u8 A_GapDate , __u8 A_GapTime , __u8 A_DivGap , BOOL8 A_IsDivider ,
	BOOL8 A_IsFix , BOOL8 A_IsApm , WCHAR* A_StrAm , WCHAR* A_StrPm , __u8 A_AlignHz , __u8 A_AlignVt)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Result	=	FALSE;

		if((m_SkinCtlTblBase.ItemCnt > A_Line) && (m_SkinCtlTblBase.ColumCnt > A_Colum))
		{
			if(NULL != m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Str)
			{
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Type			=	E_GrP5TblCtlValue;
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].AlignHz		=	A_AlignHz;
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].AlignVt		=	A_AlignVt;
				m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Value			=	(__u32)A_Time;

				GrStrTimeToWstr(m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Str, A_Time, A_IsNoTime, A_IsNoDate, A_IsNoSec, A_GapDate, A_GapTime,
					A_DivGap, A_IsDivider, A_IsFix, A_IsApm, A_StrAm, A_StrPm);

				// redraw
				if(ItemRectGetByIdx(A_Line, (__s32)A_Colum, &Tv_Rc))
				{
					LcPntReqByRect(&Tv_Rc);
				}

				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP5UoTblBase::ItemGetText(__u32 A_Line, __u32 A_Colum)
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if((m_SkinCtlTblBase.ItemCnt > A_Line) && (m_SkinCtlTblBase.ColumCnt > A_Colum))
		{
			if((E_GrP5TblCtlImage != m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Type) && (E_GrP5TblCtlTextId != m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Type))
			{
				Tv_Result	=	m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Str;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16		Cls_GrP5UoTblBase::ItemGetResourceId(__u32 A_Line, __u32 A_Colum)
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrP5ImgIdNone;

		if((m_SkinCtlTblBase.ItemCnt > A_Line) && (m_SkinCtlTblBase.ColumCnt > A_Colum))
		{
			if((E_GrP5TblCtlImage == m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Type) || (E_GrP5TblCtlTextId == m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Type))
			{
				Tv_Result	=	m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].RsrcId;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrP5UoTblBase::ItemGetValue(__u32 A_Line, __u32 A_Colum, __u32* A_PtrRtVal)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if((m_SkinCtlTblBase.ItemCnt > A_Line) && (m_SkinCtlTblBase.ColumCnt > A_Colum))
		{
			if(E_GrP5TblCtlValue == m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Type)
			{
				*A_PtrRtVal	=	m_SkinCtlTblBase.Lines[A_Line].Cols[A_Colum].Value;
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoTblBase::ItemCountGet(void)
{
		return	m_SkinCtlTblBase.ItemCnt;
}
//--------------------------------------------------------------------
