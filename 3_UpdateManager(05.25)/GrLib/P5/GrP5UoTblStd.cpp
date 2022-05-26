/*
Platform 5 UI list base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoTblStd.h>

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

#if 0


// relate position
#define	E_GrP5UoTblStdParaInfoCnt							38

St_GrP5DvlScptParaInfo	V_GrP5UoTblStdParaInfo[E_GrP5UoTblStdParaInfoCnt]	=
{
	{L"Table base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblStdPara)0)->FontId, TRUE, E_GrP5DvlParaInfoActDefault, L"Font ID"},
	{L"ItemHeight", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblStdPara)0)->ItemHeight, TRUE, E_GrP5DvlParaInfoActDefault, L"Item Height"},
	{L"ItemMaxCnt", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblStdPara)0)->ItemMaxCnt, TRUE, 0, L"Item max count"},
	{L"ColumCnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoTblStdPara)0)->ColumCnt, TRUE, 0, L"Colum count"},
	{L"IsLineSel", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoTblStdPara)0)->IsLineSel, TRUE, 0, L"Line unit select"},
	{L"ColSize 0", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoTblStdPara)0)->ColSize[0], TRUE, 0, L"Colum width"},
	
};

#endif

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoTblStd::Cls_GrP5UoTblStd(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoTblBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		//ClassRegist(E_GrP5ClassIdTblStd, &m_ParaUoTblStd, sizeof(m_ParaUoTblStd));
		ClassRegist(E_GrP5ClassIdTblStd, NULL, 0, NULL);

		// default setting
		//GrDumyZeroMem(&m_ParaUoTblStd, sizeof(m_ParaUoTblStd));


}
//--------------------------------------------------------------------
Cls_GrP5UoTblStd::~Cls_GrP5UoTblStd(void)
{
	// local -------------------
	// code --------------------

		GrP5TxtBufFreeByObjId(m_ObjId);
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoTblStd::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoTblBase::DvlParaInfoBuild();

		// add
		//DvlParaInfoAdd(V_GrP5UoTblStdParaInfo, E_GrP5UoTblStdParaInfoCnt, &m_ParaUoTblStd);
		//DvlParaInfoTxtParaAdd(L"Text parametter", &m_ParaUoTblStd.TxtPara);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoTblStd::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------
		// style rebuild

		// inherited
		Cls_GrP5UoTblBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoTblStd::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoTblBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
__s32	Cls_GrP5UoTblStd::ItemAdd(void)
{
	// local -------------------
		__s32		Tv_Result;
		__u32		Tv_ColIdx;
		__u32		Tv_ColCnt;
	// code --------------------
		Tv_Result	=	Cls_GrP5UoTblBase::ItemAdd();
		if(0 > Tv_Result)
		{
			return	Tv_Result;
		}

		// check allocated text buffer
		Tv_ColCnt	=	m_ParaUoTblBase.ColumCnt;
		if(E_GrP5SkinTblColumMaxCnt <= Tv_ColCnt)
		{
			Tv_ColCnt		=	E_GrP5SkinTblColumMaxCnt;
		}

		for( Tv_ColIdx = 0; Tv_ColIdx < Tv_ColCnt; Tv_ColIdx++)
		{
			if(NULL == m_SkinCtlTblBase.Lines[Tv_Result].Cols[Tv_ColIdx].Str)
			{
				m_SkinCtlTblBase.Lines[Tv_Result].Cols[Tv_ColIdx].Str	=	GrP5TxtBufAlloc(m_ObjId);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
