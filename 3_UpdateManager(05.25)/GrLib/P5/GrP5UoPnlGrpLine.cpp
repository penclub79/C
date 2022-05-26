/*
Platform 5 UI object graph line

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoPnlGrpLine.h>

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

#define	E_GrP5UoPnlGrpLineParaInfoCnt							21

St_GrP5DvlScptParaInfo	V_GrP5UoPnlGrpLineParaInfo[E_GrP5UoPnlGrpLineParaInfoCnt]	=
{
	{L"Pannel graph line", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL},
	{L"GridCnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->GridCnt, TRUE, 0, L"grid count"},
	{L"BoarderWidth", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->BoarderWidth, TRUE, 0, L"boarder width"},
	{L"GridLineWidth", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->GridLineWidth, TRUE, 0, L"grid width"},
	{L"FontGrid", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->FontGrid, TRUE, 0, L"grid font id"},
	{L"FontCol", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->FontCol, TRUE, 0, L"colum font id"},

	{L"GrpWidth", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->GrpWidth, TRUE, 0, L"graph bar width"},
	{L"GridHeight", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->GridHeight, TRUE, 0, L"grid height"},
	{L"ColNameHeight", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->ColNameHeight, TRUE, 0, L"colum name height"},
	{L"GridValWidth", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->GridValWidth, TRUE, 0, L"grid value area width"},
	{L"GridValHeight", E_GrVarTypeU32, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->GridValHeight, TRUE, 0, L"grid value area height"},
	{L"ClrGrid", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->ClrGrid, TRUE, 0, L"color gird line"},
	{L"ClrBoarder", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->ClrBoarder, TRUE, 0, L"color boarder line"},

	{L"ClrGraph0", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->ClrGraph[0], TRUE, 0, L"color graph 0"},
	{L"ClrGraph1", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->ClrGraph[1], TRUE, 0, L"color graph 1"},
	{L"ClrGraph2", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->ClrGraph[2], TRUE, 0, L"color graph 2"},
	{L"ClrGraph3", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->ClrGraph[3], TRUE, 0, L"color graph 3"},
	{L"ClrGraph4", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->ClrGraph[4], TRUE, 0, L"color graph 4"},
	{L"ClrGraph5", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->ClrGraph[5], TRUE, 0, L"color graph 5"},
	{L"ClrGraph6", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->ClrGraph[6], TRUE, 0, L"color graph 6"},
	{L"ClrGraph7", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoPnlGrpLinePara)0)->ClrGraph[7], TRUE, 0, L"color graph 7"},
};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoPnlGrpLine::Cls_GrP5UoPnlGrpLine(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
		WCHAR*	Tv_StrBuf;
		__u32		Tv_Used;
		__u32		Tv_WkIdx;
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdPnlGrpLine, &m_ParaUoPnlGrpLine, sizeof(m_ParaUoPnlGrpLine), &m_SkinCtlPnlGrpLine);

		// default setting
		GrDumyZeroMem(&m_ParaUoPnlGrpLine, sizeof(m_ParaUoPnlGrpLine));
		GrDumyZeroMem(&m_SkinCtlPnlGrpLine, sizeof(m_SkinCtlPnlGrpLine));

		m_ParaUoPnlGrpLine.GridCnt					=	5;
		m_ParaUoPnlGrpLine.BoarderWidth			=	2;
		m_ParaUoPnlGrpLine.GridLineWidth		=	1;
		m_ParaUoPnlGrpLine.GrpWidth					=	1;
		m_ParaUoPnlGrpLine.GridHeight				=	80;
		m_ParaUoPnlGrpLine.ColNameHeight		=	40;
		m_ParaUoPnlGrpLine.GridValWidth			=	100;
		m_ParaUoPnlGrpLine.GridValHeight		=	40;
		m_ParaUoPnlGrpLine.ClrGrid					=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 98, 151, 218), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.ClrBoarder				=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 98, 151, 218), GrP5ScriptPxFmtGet());

		m_ParaUoPnlGrpLine.TxtStyleGrid.Atb	=	E_GrP5FontAtbDrawText;
		m_ParaUoPnlGrpLine.TxtStyleGrid.ColorTxtTbl[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.TxtStyleGrid.ColorTxtTbl[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.TxtStyleGrid.ColorTxtTbl[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.TxtStyleGrid.ColorTxtTbl[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());

		m_ParaUoPnlGrpLine.TxtStyleCol.Atb	=	E_GrP5FontAtbDrawText;
		m_ParaUoPnlGrpLine.TxtStyleCol.ColorTxtTbl[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.TxtStyleCol.ColorTxtTbl[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.TxtStyleCol.ColorTxtTbl[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.TxtStyleCol.ColorTxtTbl[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), GrP5ScriptPxFmtGet());

		m_ParaUoPnlGrpLine.ClrGraph[0]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 0, 0), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.ClrGraph[1]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 127, 39), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.ClrGraph[2]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 242, 0), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.ClrGraph[3]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 34, 177, 76), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.ClrGraph[4]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 162, 231), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.ClrGraph[5]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 63, 72, 204), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.ClrGraph[6]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 163, 73, 164), GrP5ScriptPxFmtGet());
		m_ParaUoPnlGrpLine.ClrGraph[7]			=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 10, 10, 10), GrP5ScriptPxFmtGet());

		// reset
		m_SkinCtlPnlGrpLine.ColCnt		=	7;
		m_SkinCtlPnlGrpLine.GrpCnt		=	3;
		m_ValStep		=	10;

		GrDumyZeroMem(m_GrpTbl, sizeof(m_GrpTbl));

		LcGridValBuild();

		// allocate colum name buffer
		Tv_StrBuf	=	NULL;
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5SkinPnlGrpLineColMaxCnt; Tv_WkIdx++)
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

			m_SkinCtlPnlGrpLine.Cols[Tv_WkIdx].StrName	=	(WCHAR*)((Def_GrCalPtr)Tv_StrBuf + (Def_GrCalPtr)(Tv_Used << 1));
			GrStrClear(m_SkinCtlPnlGrpLine.Cols[Tv_WkIdx].StrName);

			// next
			Tv_Used		=	Tv_Used + (E_GrP5UoPntGrpBarColNameMaxLen + 1);
			if((Tv_Used + E_GrP5UoPntGrpBarColNameMaxLen) > E_GrP5TxtBufMaxLen)
			{
				Tv_StrBuf	=	NULL;
			}
		}

		// allocate grid value buffer
		Tv_StrBuf	=	NULL;
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5SkinPnlGrpLineGridMaxCnt; Tv_WkIdx++)
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

			m_SkinCtlPnlGrpLine.Grids[Tv_WkIdx].StrVal	=	(WCHAR*)((Def_GrCalPtr)Tv_StrBuf + (Def_GrCalPtr)(Tv_Used << 1));
			GrStrClear(m_SkinCtlPnlGrpLine.Grids[Tv_WkIdx].StrVal);

			// next
			Tv_Used		=	Tv_Used + 16;
			if((Tv_Used + 15) > E_GrP5TxtBufMaxLen)
			{
				Tv_StrBuf	=	NULL;
			}
		}



#ifdef GR_P5_EDITOR
		GrStrWcopy(m_SkinCtlPnlGrpLine.Cols[0].StrName, L"A");
		GrStrWcopy(m_SkinCtlPnlGrpLine.Cols[1].StrName, L"B");
		GrStrWcopy(m_SkinCtlPnlGrpLine.Cols[2].StrName, L"C");
		GrStrWcopy(m_SkinCtlPnlGrpLine.Cols[3].StrName, L"D");
		GrStrWcopy(m_SkinCtlPnlGrpLine.Cols[4].StrName, L"E");
		GrStrWcopy(m_SkinCtlPnlGrpLine.Cols[5].StrName, L"F");
		GrStrWcopy(m_SkinCtlPnlGrpLine.Cols[6].StrName, L"G");

		m_SkinCtlPnlGrpLine.Grps[0].IsDraw				=	TRUE;
		m_GrpTbl[0].Cols[0].Val		=	0;
		m_GrpTbl[0].Cols[1].Val		=	0;
		m_GrpTbl[0].Cols[2].Val		=	5;
		m_GrpTbl[0].Cols[3].Val		=	10;
		m_GrpTbl[0].Cols[4].Val		=	15;
		m_GrpTbl[0].Cols[5].Val		=	20;
		m_GrpTbl[0].Cols[6].Val		=	25;

		m_SkinCtlPnlGrpLine.Grps[1].IsDraw				=	TRUE;
		m_GrpTbl[1].Cols[0].Val		=	35;
		m_GrpTbl[1].Cols[1].Val		=	30;
		m_GrpTbl[1].Cols[2].Val		=	25;
		m_GrpTbl[1].Cols[3].Val		=	20;
		m_GrpTbl[1].Cols[4].Val		=	15;
		m_GrpTbl[1].Cols[5].Val		=	10;
		m_GrpTbl[1].Cols[6].Val		=	5;

		m_SkinCtlPnlGrpLine.Grps[2].IsDraw				=	TRUE;
		m_GrpTbl[2].Cols[0].Val		=	10;
		m_GrpTbl[2].Cols[1].Val		=	20;
		m_GrpTbl[2].Cols[2].Val		=	5;
		m_GrpTbl[2].Cols[3].Val		=	30;
		m_GrpTbl[2].Cols[4].Val		=	20;
		m_GrpTbl[2].Cols[5].Val		=	25;
		m_GrpTbl[2].Cols[6].Val		=	15;

#endif

}
//--------------------------------------------------------------------
Cls_GrP5UoPnlGrpLine::~Cls_GrP5UoPnlGrpLine(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpLine::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlGrpLine::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// separator basic info
		DvlParaInfoAdd(V_GrP5UoPnlGrpLineParaInfo, E_GrP5UoPnlGrpLineParaInfoCnt, &m_ParaUoPnlGrpLine);
		DvlParaInfoTxtParaAdd(L"Text style grid value", &m_ParaUoPnlGrpLine.TxtStyleGrid);
		DvlParaInfoTxtParaAdd(L"Text style colum name", &m_ParaUoPnlGrpLine.TxtStyleCol);

		LcGridValBuild();

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlGrpLine::DvlParaInfoAction(__u32 A_Action)
{
	// inherited
	Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

	LcGridValBuild();

	// custom action
	ReqDraw(FALSE);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlGrpLine::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
	// delete prepare


	// inherited
	Cls_GrP5UoBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpLine::LcGrpValBuild(void)
{
	// local -------------------
		__u32	Tv_GrpIdx;
		__u32	Tv_ColIdx;
	// code --------------------
		// graph colum y position
		if((0 == m_SkinCtlPnlGrpLine.GrpCnt) || (0 == m_SkinCtlPnlGrpLine.ColCnt))
		{
			return;
		}

		for(Tv_GrpIdx = 0; Tv_GrpIdx < m_SkinCtlPnlGrpLine.GrpCnt; Tv_GrpIdx++)
		{
			for(Tv_ColIdx = 0; Tv_ColIdx < m_SkinCtlPnlGrpLine.ColCnt; Tv_ColIdx++)
			{
				// calculate value position
				m_SkinCtlPnlGrpLine.Grps[Tv_GrpIdx].Cols[Tv_ColIdx].Pos	=	
					(__s32)(m_ParaUoPnlGrpLine.GridHeight * m_GrpTbl[Tv_GrpIdx].Cols[Tv_ColIdx].Val) / m_ValStep;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpLine::LcGridValBuild(void)
{
	// local -------------------
		__u32	Tv_GridIdx;
		__s32	Tv_Value;
	// code --------------------

		if(0 == m_ParaUoPnlGrpLine.GridCnt)
		{
			return;
		}

		Tv_Value		=	0;

		for(Tv_GridIdx = 0; Tv_GridIdx < (__u32)m_ParaUoPnlGrpLine.GridCnt; Tv_GridIdx++)
		{
			if(NULL != m_SkinCtlPnlGrpLine.Grids[Tv_GridIdx].StrVal)
			{
				GrStrIntToWstr(m_SkinCtlPnlGrpLine.Grids[Tv_GridIdx].StrVal, Tv_Value);
			}

			// next
			if(E_GrP5SkinPnlGrpLineGridMaxCnt <= Tv_GridIdx)
			{
				break;
			}
			Tv_Value	=	Tv_Value + (__s32)m_ValStep;
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoPnlGrpLine::GraphCountGet(void)
{
	// local -------------------
	// code --------------------
		return	m_SkinCtlPnlGrpLine.GrpCnt;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpLine::GraphCountSet(__u32 A_Cnt)
{
	if((0 < A_Cnt) && (E_GrP5SkinPnlGrpLineGrpMaxCnt >= A_Cnt))
	{
		m_SkinCtlPnlGrpLine.GrpCnt	=	A_Cnt;

		LcSkinCtlUpdate();
		LcGrpValBuild();

		ReqDraw(FALSE);
	}
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoPnlGrpLine::GridStepGet(void)
{
	return	m_ValStep;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpLine::GridStepSet(__u32 A_Step)
{
	if(0 != A_Step)
	{
		m_ValStep	=	A_Step;

		LcGridValBuild();
		LcGrpValBuild();

		ReqDraw(FALSE);
	}
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoPnlGrpLine::ColumCountGet(void)
{
	// local -------------------
	// code --------------------
		return	m_SkinCtlPnlGrpLine.ColCnt;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpLine::ColumCountSet(__u32 A_Cnt)
{
	// local -------------------
	// code --------------------
		if((0 < A_Cnt) && (E_GrP5SkinPnlGrpLineColMaxCnt >= A_Cnt))
		{
			m_SkinCtlPnlGrpLine.ColCnt	=	A_Cnt;
			LcSkinCtlUpdate();
			LcGrpValBuild();

			ReqDraw(FALSE);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpLine::ColumNameSet(__u32 A_ColIdx, WCHAR* A_Str)
{
		if(E_GrP5SkinPnlGrpLineColMaxCnt <= A_ColIdx)
		{
			return;
		}

		
		GrStrWcopy(m_SkinCtlPnlGrpLine.Cols[A_ColIdx].StrName, A_Str, E_GrP5UoPntGrpBarColNameMaxLen);

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpLine::GraphValueSet(__u32 A_GrpIdx, __u32 A_ColIdx, __u32 A_Value)
{
		if((E_GrP5SkinPnlGrpLineGrpMaxCnt <= A_GrpIdx) || (E_GrP5SkinPnlGrpLineColMaxCnt <= A_ColIdx))
		{
			return;
		}

		m_GrpTbl[A_GrpIdx].Cols[A_ColIdx].Val			=	A_Value;
		// calculate value position
		m_SkinCtlPnlGrpLine.Grps[A_GrpIdx].Cols[A_ColIdx].Pos	=
			(__s32)(m_ParaUoPnlGrpLine.GridHeight * m_GrpTbl[A_GrpIdx].Cols[A_ColIdx].Val) / m_ValStep;


		if(m_SkinCtlPnlGrpLine.Grps[A_GrpIdx].IsDraw)
		{
			ReqDraw(FALSE);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlGrpLine::GraphVisibleSet(__u32 A_GrpIdx, BOOL8 A_IsVisible)
{
	// local -------------------
	// code --------------------
		if(E_GrP5SkinPnlGrpLineGrpMaxCnt <= A_GrpIdx)
		{
			return;
		}

		m_SkinCtlPnlGrpLine.Grps[A_GrpIdx].IsDraw	=	A_IsVisible;

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
