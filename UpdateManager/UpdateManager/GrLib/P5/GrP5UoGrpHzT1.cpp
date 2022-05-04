/*
Platform 5 UI object graph horizontal time map type 1

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoGrpHzT1.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>

#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
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

// relate position
#define	E_GrP5UoGrpHzT1ParaInfoCnt							11

St_GrP5DvlScptParaInfo	V_GrP5UoGrpHzT1ParaInfo[E_GrP5UoGrpHzT1ParaInfoCnt]	=
{
	{L"Graph horizontal map Type 1", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"RulerFontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGrpHzT1Para)0)->RulerFontId, TRUE, 0, L"Ruler font id"},
	{L"RulerHeight", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGrpHzT1Para)0)->RulerHeight, TRUE, 0, L"Ruler height"},
	{L"RulerUnitWidth", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoGrpHzT1Para)0)->RulerUnitWidth, TRUE, 0, L"Ruler unit width"},

	{L"IndFontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGrpHzT1Para)0)->IndFontId, TRUE, 0, L"Indicater font id"},
	{L"IndWidth", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoGrpHzT1Para)0)->IndWidth, TRUE, 0, L"Indicater width"},

	{L"CellWidth", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGrpHzT1Para)0)->CellWidth, TRUE, 0, L"Cell width"},
	{L"ViewLineCnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGrpHzT1Para)0)->ViewLineCnt, TRUE, 0, L"View line count"},
	{L"IsDayMode", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoGrpHzT1Para)0)->IsDayMode, TRUE, 0, L"Day or hour mode"},
	{L"RegCusTime", E_GrP5DvlValTypeRintId, 1, (void*)&((Ptr_GrP5UoGrpHzT1Para)0)->RegCusTime, TRUE, 0, L"Cursor time store register"},

	{L"RelHourGrp", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoGrpHzT1Para)0)->RelHourGrp, TRUE, 0, L"Relate hour graph"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoGrpHzT1::Cls_GrP5UoGrpHzT1(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoGrpHzBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
#ifdef GR_P5_EDITOR
		__u32	Tv_WkX;
		__u32	Tv_WkY;
		__u8	Tv_Val;
#endif
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdGrpHzT1, &m_ParaUoGrpHzT1, sizeof(m_ParaUoGrpHzT1), &m_SkinCtlGrpHzT1);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoGrpHzT1, sizeof(m_ParaUoGrpHzT1));
		m_ParaUoGrpHzT1.ViewLineCnt	=	4;
		m_ParaUoGrpHzT1.IsDayMode	=	FALSE;
		m_ParaUoGrpHzT1.CellWidth		=	8;
		m_ParaUoGrpHzT1.RulerTxtStyle.Atb	=	E_GrP5FontAtbDrawText;
		m_ParaUoGrpHzT1.IndTxtStyle.Atb		=	E_GrP5FontAtbDrawText;
		m_ParaUoGrpHzT1.IndWidth					=	40;
		m_ParaUoGrpHzT1.RulerHeight				=	26;
		m_ParaUoGrpHzT1.RulerUnitWidth		=	30;

		m_PtrMap			=	NULL;
		GrDumyZeroMem(m_ViewMap, sizeof(m_ViewMap));

		m_SkinCtlGrpHzT1.IsDayMode		=	m_ParaUoGrpHzT1.IsDayMode;
		m_Hour				=	0;

		m_StartCh			=	0;

		m_RelGrp			=	NULL;
		
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoGrpHzT1.RulerTxtStyle);
		GrP5SkinDvlFontColorSet(&m_ParaUoGrpHzT1.IndTxtStyle);

		Tv_Val	=	1;
		for( Tv_WkY = 0; Tv_WkY < E_GrDvrMaxChCnt; Tv_WkY++)
		{
			for( Tv_WkX = 0; Tv_WkX < E_GrP5UoGrpHzT1HzCellMax; Tv_WkX++)
			{
				// write
				m_ViewMap[Tv_WkY][Tv_WkX]	=	Tv_Val;
				Tv_Val ++;
				Tv_Val	=	Tv_Val & 7;
			}
		}

#endif

		// ruler 
		m_SkinCtlGrpHzT1.StrInd		=	GrP5TxtBufAlloc(m_ObjId);
}
//--------------------------------------------------------------------
Cls_GrP5UoGrpHzT1::~Cls_GrP5UoGrpHzT1(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGrpHzT1::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoGrpHzBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoGrpHzT1ParaInfo, E_GrP5UoGrpHzT1ParaInfoCnt, &m_ParaUoGrpHzT1);
		DvlParaInfoTxtParaAdd(L"Ruler text style", &m_ParaUoGrpHzT1.RulerTxtStyle);
		DvlParaInfoTxtParaAdd(L"Indicater text style", &m_ParaUoGrpHzT1.IndTxtStyle);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGrpHzT1::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		m_SkinCtlGrpHzT1.IsDayMode		=	m_ParaUoGrpHzT1.IsDayMode;
		LcParaValUpdate();

		// inherited
		Cls_GrP5UoGrpHzBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------

		m_SkinCtlGrpHzT1.IsDayMode		=	m_ParaUoGrpHzT1.IsDayMode;
		LcParaValUpdate();

		// inherited
		Cls_GrP5UoGrpHzBase::EvtScptDecDone();


}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGrpHzT1::ItemValueGet(__u32 A_IdxX, __u32 A_IdxY)
{
	// local -------------------
		__u8	Tv_Reuslt;
	// code --------------------
		Tv_Reuslt	=	0;

		if((A_IdxX < m_SkinCtlGrpHzBase.CellCntX) && (A_IdxY < m_SkinCtlGrpHzBase.CellCntY))
		{
			Tv_Reuslt	=	m_ViewMap[A_IdxY][A_IdxX];
		}

		return	Tv_Reuslt;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::MapBufferSet(__u8* A_PtrMap)
{
	// local -------------------
	// code --------------------
		m_PtrMap	=	A_PtrMap;

		// update view map
		LcViewMapUpdate();
}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGrpHzT1::HourGet(void)
{
		return	m_Hour;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::TimeBaseSet(Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Hour;
		__u8	Tv_Min;
		__u8	Tv_Sec;
	// code --------------------
		GrTimeDec(A_Time, &m_Year, &m_Month, &m_Day, &Tv_Hour, &Tv_Min, &Tv_Sec);
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrP5UoGrpHzT1::TimeBaseGet(void)
{
		return	GrTimeEnc(m_Year,m_Month,m_Day,0,0,0);
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrP5UoGrpHzT1::LcTimeCalcByIndex(__u32 A_IdxX)
{
	// local -------------------
		__u32	Tv_Min;
		__u8	Tv_Hour;
	// code --------------------

		Tv_Min		=	0;
		Tv_Hour		=	0;
		if(m_SkinCtlGrpHzT1.IsDayMode)
		{
			Tv_Hour	=	A_IdxX;
		}
		else
		{
			Tv_Min	=	A_IdxX;
			Tv_Hour	=	m_Hour;
		}

		return	GrTimeEnc(m_Year, m_Month, m_Day, Tv_Hour, Tv_Min, 0);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGrpHzT1::EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32	Tv_IdxX;
		__s32	Tv_IdxY;
		__s32	Tv_ScrX;
		__s32	Tv_ScrY;
	// code --------------------
		if(NULL == m_SkinMng)
		{
			return	TRUE;
		}

		// calculate position
		Tv_ScrX		=	A_X + m_RcScr.left;
		Tv_ScrY		=	A_Y + m_RcScr.top;

		m_SkinMng->CtlGetItemIdxByPos(this, Tv_ScrX, Tv_ScrY, &Tv_IdxX, &Tv_IdxY);
		if(0 <= Tv_IdxX)
		{
			// check change cursor
			LcDoCusPosChg(Tv_IdxX,Tv_IdxY);
		}
	
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::LcViewMapUpdate(void)
{
	// local -------------------
	// code --------------------
		if(NULL != m_PtrMap)
		{
			if(m_SkinCtlGrpHzT1.IsDayMode)
			{
				LcSubViewMapDayUpdate();
			}
			else
			{
				LcSubViewMapHourUpdate();
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoGrpHzT1::LcMapIdxCalcBegin(void)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if(!m_SkinCtlGrpHzT1.IsDayMode)
		{
			Tv_Result	=	Tv_Result + (E_GrTimeMinOfHour * E_GrDvrMaxChCnt * (__u32)m_Hour);
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::LcSubViewMapDayUpdate(void)
{
	// local -------------------
		__u32	Tv_MinIdx;
		__u32	Tv_VwX;
		__u32	Tv_ChIdx;
		__u32	Tv_MrgIdx;
	// code --------------------
		// reset
		GrDumyZeroMem(m_ViewMap, sizeof(m_ViewMap));

		// calculate start index
		Tv_MinIdx	=	LcMapIdxCalcBegin();

		for( Tv_VwX = 0; Tv_VwX < m_SkinCtlGrpHzBase.CellCntX; Tv_VwX++)
		{
			for(Tv_MrgIdx = 0; Tv_MrgIdx < E_GrTimeMinOfHour; Tv_MrgIdx++)
			{
				for(Tv_ChIdx = 0; Tv_ChIdx < E_GrDvrMaxChCnt; Tv_ChIdx++)
				{
					m_ViewMap[0][Tv_VwX]	=	m_ViewMap[0][Tv_VwX] | m_PtrMap[Tv_MinIdx + Tv_ChIdx];
				}
				// next
				Tv_MinIdx	=	Tv_MinIdx + E_GrDvrMaxChCnt;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::LcSubViewMapHourUpdate(void)
{
	// local -------------------
		__u32	Tv_MinIdx;
		__u32	Tv_VwX;
		__u32	Tv_ChIdx;
	// code --------------------
		// reset
		//GrDumyZeroMem(m_ViewMap, sizeof(m_ViewMap));

		// calculate start index
		Tv_MinIdx	=	LcMapIdxCalcBegin();

		for(Tv_VwX = 0; Tv_VwX < m_SkinCtlGrpHzBase.CellCntX; Tv_VwX++)
		{
			for(Tv_ChIdx = 0; Tv_ChIdx < m_SkinCtlGrpHzBase.CellCntY; Tv_ChIdx++)
			{
				m_ViewMap[Tv_ChIdx][Tv_VwX]	=	m_PtrMap[Tv_MinIdx + Tv_ChIdx];
			}
			// next
			Tv_MinIdx	=	Tv_MinIdx + E_GrDvrMaxChCnt;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::HourSet(__u8 A_Hour)
{
	// local -------------------
	// code --------------------
		m_Hour	=	A_Hour;

		if(!m_SkinCtlGrpHzT1.IsDayMode)
		{
			LcViewMapUpdate();
			ReqDraw(FALSE);
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::ChannelStartSet(__u8 A_Ch)
{
	// local -------------------
	// code --------------------
		if(E_GrDvrMaxChCnt > A_Ch)
		{
			m_StartCh		=	A_Ch;
			LcViewMapUpdate();
			ReqDraw(FALSE);
		}

}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGrpHzT1::ChannelStartGet(void)
{
		return	m_StartCh;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGrpHzT1::IsDayMode(void)
{
		return	m_SkinCtlGrpHzT1.IsDayMode;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::ModeSet(BOOL8 A_IsDayMode)
{
	// local -------------------
		//__u32	Tv_CntX;
	// code --------------------
		
		m_SkinCtlGrpHzT1.IsDayMode	=	A_IsDayMode;

		LcParaValUpdate();
		LcViewMapUpdate();
		LcSkinCtlUpdate();

		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::LcParaValUpdate(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Obj;
	// code --------------------

		// recalculate count
		if(m_SkinCtlGrpHzT1.IsDayMode)
		{
			m_SkinCtlGrpHzBase.CellCntX	=	E_GrTimeHourOfDay;
			m_SkinCtlGrpHzBase.CellCntY	=	1;
		}
		else
		{
			m_SkinCtlGrpHzBase.CellCntX	=	E_GrTimeMinOfHour;
			// view line count
			m_SkinCtlGrpHzBase.CellCntY	=	1;
			if((0 != m_ParaUoGrpHzT1.ViewLineCnt) && (E_GrDvrMaxChCnt > m_ParaUoGrpHzT1.ViewLineCnt))
			{
				m_SkinCtlGrpHzBase.CellCntY	=	m_ParaUoGrpHzT1.ViewLineCnt;
			}
		}

		// relate object
		m_RelGrp	=	NULL;
		if(E_GrP5ScptIdNone != m_ParaUoGrpHzT1.RelHourGrp)
		{
			Tv_Obj	=	(Cls_GrP5UoBase*)m_ObjWin->RtlUoFindByScptId(m_ParaUoGrpHzT1.RelHourGrp);
			if(NULL != Tv_Obj)
			{
				if(Tv_Obj->IsInheritClass(E_GrP5ClassIdGrpHzT1))
				{
					m_RelGrp	=	Tv_Obj;
				}
			}
		}

		LcSkinCtlUpdate();
	
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrP5UoGrpHzT1::SelectTimeGet(void)
{
	// local -------------------
		Def_GrTime	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if(0 <= m_SkinCtlGrpHzBase.SelIdxX)
		{
			Tv_Result	=	LcTimeCalcByIndex(m_SkinCtlGrpHzBase.SelIdxX);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::SelectTimeSetByHm(__u8 A_Hour, __u8 A_Min)
{
	// local -------------------
		__s32	Tv_IdxX;
	// code --------------------
		if((24 > A_Hour) && (60 > A_Min))
		{
			if(!m_SkinCtlGrpHzT1.IsDayMode)
			{
				// hour mode
				Tv_IdxX		=	(__s32)A_Hour;
			}
			else
			{
				// day mode
				Tv_IdxX		=	A_Hour;
			}

			if(m_SkinCtlGrpHzBase.CusIdxX != Tv_IdxX)
			{
				// darw old
				LcRedrawCusArea((__u32)m_SkinCtlGrpHzBase.CusIdxX, (__u32)m_SkinCtlGrpHzBase.CusIdxY);
				// update
				m_SkinCtlGrpHzBase.CusIdxX		=	Tv_IdxX;
				m_SkinCtlGrpHzBase.SelIdxX		=	Tv_IdxX;
				// darw new
				LcRedrawCusArea((__u32)m_SkinCtlGrpHzBase.CusIdxX, (__u32)m_SkinCtlGrpHzBase.CusIdxY);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::SelectTimeSetByTime(Def_GrTime A_Time)
{
	// local -------------------
		__u16	Tv_Year;
		__u8	Tv_Month;
		__u8	Tv_Day;
		__u8	Tv_Hour;
		__u8	Tv_Min;
		__u8	Tv_Sec;
	// code --------------------

		GrTimeDec(A_Time, &Tv_Year, &Tv_Month, &Tv_Day, &Tv_Hour, &Tv_Min, &Tv_Sec);

		SelectTimeSetByHm(Tv_Hour, Tv_Min);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::LcRedrawCursorArea(__s32 A_Idx)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		if(NULL == m_SkinMng)
		{
			return;
		}

		if((0 <= A_Idx) && ((__s32)m_SkinCtlGrpHzBase.CellCntX > A_Idx))
		{
			// calculate indicater
			m_SkinMng->CtlGetItemOneRect(this, A_Idx, -1, &Tv_Rc);
			if(0 < Tv_Rc.right)
			{
				LcPntReqByRect(&Tv_Rc);
			}

			// calculate item area
			m_SkinMng->CtlGetItemRangeRect(this, A_Idx, 0,A_Idx, m_SkinCtlGrpHzT1.VwCntY - 1, &Tv_Rc);
			LcPntReqByRect(&Tv_Rc);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::LcDoCusPosChg(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		
	// code --------------------
		// check change cursor
		if(A_X != m_SkinCtlGrpHzBase.CusIdxX)
		{
			// redraw
			LcRedrawCursorArea(A_X);
			LcRedrawCursorArea(m_SkinCtlGrpHzBase.CusIdxX);
			// change cursor
			m_SkinCtlGrpHzBase.CusIdxX	=	A_X;
			// check relaste
			if(m_SkinCtlGrpHzT1.IsDayMode )
			{
				if(NULL != m_RelGrp)
				{
					if(!((Cls_GrP5UoGrpHzT1*)m_RelGrp)->IsDayMode())
					{
						((Cls_GrP5UoGrpHzT1*)m_RelGrp)->HourSet(A_X);
					}
				}
			}
			// register change
			if(E_GrP5RegIdNone != m_ParaUoGrpHzT1.RegCusTime)
			{
				GrP5RintSet(m_ParaUoGrpHzT1.RegCusTime, (__s32)LcTimeCalcByIndex(A_X));
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::LineCntSet(__u8 A_Cnt)
{
	// local -------------------
	// code --------------------
		m_ParaUoGrpHzT1.ViewLineCnt	=	A_Cnt;
		LcParaValUpdate();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGrpHzT1::LineCntGet(void)
{
		return	m_ParaUoGrpHzT1.ViewLineCnt;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT1::DoEvtGrpItemClick(void)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
		BOOL8	Tv_IsAble;
		__u8	Tv_WkIdx;
	// code --------------------
		// register change
		if(E_GrP5RegIdNone != m_ParaUoGrpHzT1.RegCusTime)
		{
			GrP5RintSet(m_ParaUoGrpHzT1.RegCusTime, (__s32)LcTimeCalcByIndex(m_SkinCtlGrpHzBase.CusIdxX));
		}
		//Cls_GrP5UoGrpHzBase::DoEvtGrpItemClick();
		// check mode
		Tv_IsAble	=	TRUE;
		if(m_ParaUoGrpHzBase.IsDoExistOnly)
		{
			Tv_IsAble	=	FALSE;
			// check vertical exist
			for( Tv_WkIdx = 0; Tv_WkIdx < m_ParaUoGrpHzT1.ViewLineCnt; Tv_WkIdx++)
			{
				if(0 != ItemValueGet((__u32)m_SkinCtlGrpHzBase.SelIdxX, (__u32)Tv_WkIdx))
				{
					Tv_IsAble	=	TRUE;
					break;
				}
			}
		}
		if(Tv_IsAble)
		{
			// contorl
			Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
			if(NULL != Tv_Ctrl)
			{
				// calculate time
				Tv_Ctrl->EvtGraphItemSel(m_ParaUoBase.CtrlId, m_ObjId, (__u32)m_SkinCtlGrpHzBase.SelIdxX, (__u32)m_SkinCtlGrpHzBase.SelIdxY);
			}
		}

}
//--------------------------------------------------------------------
