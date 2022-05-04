/*
Platform 5 UI object Label base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoPnlGrpBar.h>

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

#define	E_GrP5UoPnlGrpBarParaInfoCnt							16

St_GrP5DvlScptParaInfo	V_GrP5UoPnlGrpBarParaInfo[E_GrP5UoPnlGrpBarParaInfoCnt]	=
{
	{L"Pannel graph bar", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL},
	{L"GridCnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->GridCnt, TRUE, 0, L"grid count"},
	{L"BoarderWidth", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->BoarderWidth, TRUE, 0, L"boarder width"},
	{L"GridLineWidth", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->GridLineWidth, TRUE, 0, L"grid width"},
	{L"FontGrid", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->FontGrid, TRUE, 0, L"grid font id"},
	{L"FontItem", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->FontItem, TRUE, 0, L"item font id"},
	{L"FontVal", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->FontVal, TRUE, 0, L"value font id"},

	{L"IsBarValView", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->IsBarValView, TRUE, 0, L"bar value visible"},

	{L"BarWidth", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->BarWidth, TRUE, 0, L"graph bar width"},
	{L"GridHeight", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->GridHeight, TRUE, 0, L"grid height"},
	{L"ItemNameHeight", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->ItemNameHeight, TRUE, 0, L"item name height"},
	{L"GridValWidth", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->GridValWidth, TRUE, 0, L"grid value area width"},
	{L"GridValHeight", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->GridValHeight, TRUE, 0, L"grid value area height"},
	{L"BarValHeight", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->BarValHeight, TRUE, 0, L"bar value height"},

	{L"ClrGrid", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->ClrGrid, TRUE, 0, L"color gird line"},
	{L"ClrBoarder", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlGrpBarPara)0)->ClrBoarder, TRUE, 0, L"color boarder line"},

};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoPnlGrpBar::Cls_GrP5UoPnlGrpBar(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
		WCHAR*	Tv_StrBuf;
		__u32		Tv_Used;
		__u32		Tv_WkIdx;
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdPnlGrpBar, &m_ParaUoPnlGrpBar, sizeof(m_ParaUoPnlGrpBar), &m_SkinCtlPnlGrpBar);

		// default setting
		GrDumyZeroMem(&m_ParaUoPnlGrpBar, sizeof(m_ParaUoPnlGrpBar));
		GrDumyZeroMem(&m_SkinCtlPnlGrpBar, sizeof(m_SkinCtlPnlGrpBar));

		m_ParaUoPnlGrpBar.GridCnt					=	5;
		m_ParaUoPnlGrpBar.BoarderWidth		=	2;
		m_ParaUoPnlGrpBar.GridLineWidth		=	1;
		m_ParaUoPnlGrpBar.BarWidth				=	10;
		m_ParaUoPnlGrpBar.GridHeight			=	80;
		m_ParaUoPnlGrpBar.ItemNameHeight	=	40;
		m_ParaUoPnlGrpBar.GridValWidth		=	100;
		m_ParaUoPnlGrpBar.GridValHeight		=	40;
		m_ParaUoPnlGrpBar.BarValHeight		=	10;
		m_ParaUoPnlGrpBar.ClrGrid					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 98, 151, 218), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpBar.ClrBoarder			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 98, 151, 218), GrP5ScriptPxFmtGet());

		m_ParaUoPnlGrpBar.TxtStyleGrid.Atb	=	E_GrP5FontAtbDrawText;
		m_ParaUoPnlGrpBar.TxtStyleGrid.ColorTxtTbl[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpBar.TxtStyleGrid.ColorTxtTbl[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpBar.TxtStyleGrid.ColorTxtTbl[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpBar.TxtStyleGrid.ColorTxtTbl[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());

		m_ParaUoPnlGrpBar.TxtStyleItem.Atb	=	E_GrP5FontAtbDrawText;
		m_ParaUoPnlGrpBar.TxtStyleItem.ColorTxtTbl[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpBar.TxtStyleItem.ColorTxtTbl[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpBar.TxtStyleItem.ColorTxtTbl[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpBar.TxtStyleItem.ColorTxtTbl[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());

		m_ParaUoPnlGrpBar.TxtStyleVal.Atb	=	E_GrP5FontAtbDrawText;
		m_ParaUoPnlGrpBar.TxtStyleVal.ColorTxtTbl[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpBar.TxtStyleVal.ColorTxtTbl[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpBar.TxtStyleVal.ColorTxtTbl[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpBar.TxtStyleVal.ColorTxtTbl[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());

		// reset
		m_SkinCtlPnlGrpBar.ItemCnt		=	7;
		m_ValStep		=	10;

		GrDumyZeroMem(m_ItemTbl, sizeof(m_ItemTbl));

		// allocate name buffer
		Tv_StrBuf	=	NULL;
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5SkinPnlGrpBarItemMaxCnt; Tv_WkIdx++)
		{
			if(NULL == Tv_StrBuf)
			{
				Tv_StrBuf	=	GrP5TxtBufAlloc(m_ObjId);
				Tv_Used		=	0;
				if(NULL == Tv_StrBuf)
				{
					break;
				}
			}

			m_SkinCtlPnlGrpBar.Items[Tv_WkIdx].StrName	=	(WCHAR*)((Def_GrCalPtr)Tv_StrBuf + (Def_GrCalPtr)(Tv_Used << 1));
			GrStrClear(m_SkinCtlPnlGrpBar.Items[Tv_WkIdx].StrName);

			// next
			Tv_Used		=	Tv_Used + (E_GrP5UoPntGrpBarItemNameMaxLen + 1);
			if((Tv_Used + E_GrP5UoPntGrpBarItemNameMaxLen) > E_GrP5TxtBufMaxLen)
			{
				Tv_StrBuf	=	NULL;
			}
		}

		// allocate item value string
		Tv_StrBuf	=	NULL;
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5SkinPnlGrpBarItemMaxCnt; Tv_WkIdx++)
		{
			if(NULL == Tv_StrBuf)
			{
				Tv_StrBuf	=	GrP5TxtBufAlloc(m_ObjId);
				Tv_Used		=	0;
				if(NULL == Tv_StrBuf)
				{
					break;
				}
			}

			m_SkinCtlPnlGrpBar.Items[Tv_WkIdx].StrVal	=	(WCHAR*)((Def_GrCalPtr)Tv_StrBuf + (Def_GrCalPtr)(Tv_Used << 1));
			GrStrClear(m_SkinCtlPnlGrpBar.Items[Tv_WkIdx].StrVal);

			// next
			Tv_Used		=	Tv_Used + 16;
			if((Tv_Used + 15) > E_GrP5TxtBufMaxLen)
			{
				Tv_StrBuf	=	NULL;
			}
		}

		// allocate grid value buffer
		Tv_StrBuf	=	NULL;
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5SkinPnlGrpBarGridMaxCnt; Tv_WkIdx++)
		{
			if(NULL == Tv_StrBuf)
			{
				Tv_StrBuf	=	GrP5TxtBufAlloc(m_ObjId);
				Tv_Used		=	0;
				if(NULL == Tv_StrBuf)
				{
					break;
				}
			}

			m_SkinCtlPnlGrpBar.Grids[Tv_WkIdx].StrVal	=	(WCHAR*)((Def_GrCalPtr)Tv_StrBuf + (Def_GrCalPtr)(Tv_Used << 1));
			GrStrClear(m_SkinCtlPnlGrpBar.Grids[Tv_WkIdx].StrVal);

			// next
			Tv_Used		=	Tv_Used + 16;
			if((Tv_Used + 15) > E_GrP5TxtBufMaxLen)
			{
				Tv_StrBuf	=	NULL;
			}
		}

		LcGridValBuild();

#ifdef GR_P5_EDITOR
		m_SkinCtlPnlGrpBar.Items[0].Color			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 0, 0), GrP5ScriptPxFmtGet());
		m_SkinCtlPnlGrpBar.Items[1].Color			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 255, 0), GrP5ScriptPxFmtGet());
		m_SkinCtlPnlGrpBar.Items[2].Color			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 255), GrP5ScriptPxFmtGet());
		m_SkinCtlPnlGrpBar.Items[3].Color			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 0), GrP5ScriptPxFmtGet());
		m_SkinCtlPnlGrpBar.Items[4].Color			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 255, 255), GrP5ScriptPxFmtGet());

		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[0].StrName, L"A");
		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[1].StrName, L"B");
		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[2].StrName, L"C");
		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[3].StrName, L"D");
		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[4].StrName, L"E");
		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[5].StrName, L"F");
		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[6].StrName, L"G");

		m_ItemTbl[0].Val			=	11;
		m_ItemTbl[1].Val			=	22;
		m_ItemTbl[2].Val			=	33;
		m_ItemTbl[3].Val			=	44;
		m_ItemTbl[4].Val			=	55;
		m_ItemTbl[5].Val			=	66;
		m_ItemTbl[6].Val			=	77;

		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[0].StrVal, L"11");
		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[1].StrVal, L"22");
		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[2].StrVal, L"33");
		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[3].StrVal, L"44");
		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[4].StrVal, L"55");
		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[5].StrVal, L"66");
		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[6].StrVal, L"77");

#endif

}
//--------------------------------------------------------------------
Cls_GrP5UoPnlGrpBar::~Cls_GrP5UoPnlGrpBar(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpBar::EvtScptDecDone(void)
{
		LcGridValBuild();

		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlGrpBar::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// separator basic info
		DvlParaInfoAdd(V_GrP5UoPnlGrpBarParaInfo, E_GrP5UoPnlGrpBarParaInfoCnt, &m_ParaUoPnlGrpBar);
		DvlParaInfoTxtParaAdd(L"Text style grid value", &m_ParaUoPnlGrpBar.TxtStyleGrid);
		DvlParaInfoTxtParaAdd(L"Text style item name", &m_ParaUoPnlGrpBar.TxtStyleItem);
		DvlParaInfoTxtParaAdd(L"Text style bar value", &m_ParaUoPnlGrpBar.TxtStyleVal);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlGrpBar::DvlParaInfoAction(__u32 A_Action)
{

		LcGridValBuild();

		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		// custom action
		ReqDraw(FALSE);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlGrpBar::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpBar::LcGridValBuild(void)
{
	// local -------------------
		__u32	Tv_GridIdx;
		__s32	Tv_Value;
		__u32	Tv_ItmIdx;
	// code --------------------

		if(0 == m_ParaUoPnlGrpBar.GridCnt)
		{
			return;
		}

		Tv_Value		=	0;

		for( Tv_GridIdx = 0; Tv_GridIdx < (__u32)m_ParaUoPnlGrpBar.GridCnt; Tv_GridIdx++)
		{
			GrStrIntToWstr(m_SkinCtlPnlGrpBar.Grids[Tv_GridIdx].StrVal, Tv_Value);
			
			// next
			if(E_GrP5SkinPnlGrpBarGridMaxCnt <= Tv_GridIdx)
			{
				break;
			}
			Tv_Value	=	Tv_Value + (__s32)m_ValStep;
		}

		// bar height
		if(0 != m_SkinCtlPnlGrpBar.ItemCnt)
		{
			for(Tv_ItmIdx = 0; Tv_ItmIdx < m_SkinCtlPnlGrpBar.ItemCnt; Tv_ItmIdx++)
			{
				m_SkinCtlPnlGrpBar.Items[Tv_ItmIdx].BarHeight	=	(m_ParaUoPnlGrpBar.GridHeight * m_ItemTbl[Tv_ItmIdx].Val) / m_ValStep;
			}
		}

		LcSkinCtlUpdate();
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoPnlGrpBar::ItemCountGet(void)
{
	// local -------------------
	// code --------------------
		return	m_SkinCtlPnlGrpBar.ItemCnt;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpBar::ItemCountSet(__u32 A_Cnt)
{
		if((0 < A_Cnt) && (E_GrP5SkinPnlGrpBarItemMaxCnt >= A_Cnt))
		{
			m_SkinCtlPnlGrpBar.ItemCnt	=	A_Cnt;
			LcSkinCtlUpdate();
			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoPnlGrpBar::GridStepGet(void)
{
		return	m_ValStep;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpBar::GridStepSet(__u32 A_Step)
{
		if(0 != A_Step)
		{
			m_ValStep	=	A_Step;

			LcGridValBuild();

			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpBar::ItemNameSet(__u32 A_ItmIdx, WCHAR* A_Str)
{
		if(E_GrP5SkinPnlGrpBarItemMaxCnt <= A_ItmIdx )
		{
			return;
		}

		GrStrWcopy(m_SkinCtlPnlGrpBar.Items[A_ItmIdx].StrName, A_Str, E_GrP5UoPntGrpBarItemNameMaxLen);
		
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpBar::ItemDataSet(__u32 A_ItmIdx, __u32 A_Color, __u32 A_Value)
{
		if(E_GrP5SkinPnlGrpBarItemMaxCnt <= A_ItmIdx)
		{
			return;
		}

		m_SkinCtlPnlGrpBar.Items[A_ItmIdx].Color		=	A_Color;
		m_ItemTbl[A_ItmIdx].Val			=	A_Value;
		GrStrIntToWstr(m_SkinCtlPnlGrpBar.Items[A_ItmIdx].StrVal, A_Value);

		m_SkinCtlPnlGrpBar.Items[A_ItmIdx].BarHeight	=	(m_ParaUoPnlGrpBar.GridHeight * m_ItemTbl[A_ItmIdx].Val) / m_ValStep;

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpBar::ItemColorSet(__u32 A_ItmIdx, __u32 A_Color)
{
	// local -------------------

	// code --------------------
		if(E_GrP5SkinPnlGrpBarItemMaxCnt <= A_ItmIdx)
		{
			return;
		}

		m_SkinCtlPnlGrpBar.Items[A_ItmIdx].Color		=	A_Color;

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpBar::ItemValueSet(__u32 A_ItmIdx, __u32 A_Value)
{
		if(E_GrP5SkinPnlGrpBarItemMaxCnt <= A_ItmIdx)
		{
			return;
		}

		m_ItemTbl[A_ItmIdx].Val			=	A_Value;
		GrStrIntToWstr(m_SkinCtlPnlGrpBar.Items[A_ItmIdx].StrVal, A_Value);

		m_SkinCtlPnlGrpBar.Items[A_ItmIdx].BarHeight	=	(m_ParaUoPnlGrpBar.GridHeight * m_ItemTbl[A_ItmIdx].Val) / m_ValStep;

		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
