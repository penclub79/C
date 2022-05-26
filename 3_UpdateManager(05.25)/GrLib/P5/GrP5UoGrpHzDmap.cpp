/*
Platform 5 UI object graph horizontal day map

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoGrpHzDmap.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>

#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <P5/GrP5Key.h>

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
#define	E_GrP5UoGrpHzDmapParaInfoCnt							4

St_GrP5DvlScptParaInfo	V_GrP5UoGrpHzDmapParaInfo[E_GrP5UoGrpHzDmapParaInfoCnt]	=
{
	{L"Graph horizontal day map", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"ViewLineCnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGrpHzDmapPara)0)->ViewLineCnt, TRUE, 0, L"View line count"},
	{L"DayMapUnit", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGrpHzDmapPara)0)->DayMapUnit, TRUE, 0, L"0 = 3min,1= 4min, 2= 6min,3=12min,other=60min"},
	{L"RegCusTime", E_GrP5DvlValTypeRintId, 1, (void*)&((Ptr_GrP5UoGrpHzDmapPara)0)->RegCusTime, TRUE, 0, L"Cursor time store register"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoGrpHzDmap::Cls_GrP5UoGrpHzDmap(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
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
		ClassRegist(E_GrP5ClassIdGrpHzDmap, &m_ParaUoGrpHzDmap, sizeof(m_ParaUoGrpHzDmap), &m_SkinCtlGrpHzDmap);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoGrpHzDmap, sizeof(m_ParaUoGrpHzDmap));
		GrDumyZeroMem(&m_SkinCtlGrpHzDmap, sizeof(m_SkinCtlGrpHzDmap));
		m_ParaUoGrpHzDmap.ViewLineCnt	=	4;
		m_ParaUoGrpHzDmap.DayMapUnit	=	2;

		m_PtrMap	=	NULL;
		GrDumyZeroMem(m_ViewMap, sizeof(m_ViewMap));

		m_Mode		=	E_GrP5UoGrpHzDmapModeDayView;
		m_Hour		=	0;

		m_StartCh	=	0;
		m_DivDayUnit	=	6;
		
#ifdef GR_P5_EDITOR
		Tv_Val	=	1;
		for( Tv_WkY = 0; Tv_WkY < E_GrDvrMaxChCnt; Tv_WkY++)
		{
			for( Tv_WkX = 0; Tv_WkX < E_GrP5UoGrpHzDmapHzCellMax; Tv_WkX++)
			{
				// write
				m_ViewMap[Tv_WkY][Tv_WkX]	=	Tv_Val;
				Tv_Val ++;
				Tv_Val	=	Tv_Val & 7;
			}
		}

#endif

}
//--------------------------------------------------------------------
Cls_GrP5UoGrpHzDmap::~Cls_GrP5UoGrpHzDmap(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGrpHzDmap::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoGrpHzBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoGrpHzDmapParaInfo, E_GrP5UoGrpHzDmapParaInfoCnt, &m_ParaUoGrpHzDmap);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGrpHzDmap::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		LcParaValUpdate();

		// inherited
		Cls_GrP5UoGrpHzBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzDmap::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------

		LcParaValUpdate();

		// inherited
		Cls_GrP5UoGrpHzBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGrpHzDmap::ItemValueGet(__u32 A_IdxX, __u32 A_IdxY)
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
void	Cls_GrP5UoGrpHzDmap::MapBufferSet(__u8* A_PtrMap)
{
	// local -------------------
	// code --------------------
		m_PtrMap	=	A_PtrMap;

		// update view map
		LcViewMapUpdate();
}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGrpHzDmap::HourGet(void)
{
		return	m_Hour;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzDmap::TimeBaseSet(Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Hour;
		__u8	Tv_Min;
		__u8	Tv_Sec;
	// code --------------------
		GrTimeDec(A_Time, &m_Year, &m_Month, &m_Day, &Tv_Hour, &Tv_Min, &Tv_Sec);
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrP5UoGrpHzDmap::TimeBaseGet(void)
{
		return	GrTimeEnc(m_Year,m_Month,m_Day,0,0,0);
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrP5UoGrpHzDmap::LcTimeCalcByIndex(__u32 A_IdxX)
{
	// local -------------------
		__u32	Tv_Min;
		__u8	Tv_Hour;
	// code --------------------

		Tv_Min		=	0;
		Tv_Hour		=	0;
		if(E_GrP5UoGrpHzDmapModeHourView == m_Mode)
		{
			Tv_Hour	=	m_Hour;
		}
		else
		{
			Tv_Min	=	A_IdxX * m_DivDayUnit;
			Tv_Hour	=	(__u8)(Tv_Min / 60);
			Tv_Min	=	Tv_Min % 60;
		}

		return	GrTimeEnc(m_Year, m_Month, m_Day, Tv_Hour, Tv_Min, 0);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGrpHzDmap::EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
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

		// register
		if(E_GrP5RegIdNone != m_ParaUoGrpHzDmap.RegCusTime)
		{
			// calculate position
			Tv_ScrX		=	A_X + m_RcScr.left;
			Tv_ScrY		=	A_Y + m_RcScr.top;

			m_SkinMng->CtlGetItemIdxByPos(this, Tv_ScrX, Tv_ScrY, &Tv_IdxX, &Tv_IdxY);
			if((0 <= Tv_IdxX) && (0 <= Tv_IdxY))
			{
				GrP5RintSet(m_ParaUoGrpHzDmap.RegCusTime, (__s32)LcTimeCalcByIndex(Tv_IdxX));
			}
		}
	
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzDmap::LcViewMapUpdate(void)
{
	// local -------------------
	// code --------------------
		if(NULL != m_PtrMap)
		{
			switch(m_Mode)
			{
				case E_GrP5UoGrpHzDmapModeDayView:
					LcSubViewMapDayUpdate();
					break;
				case E_GrP5UoGrpHzDmapModeHourView:
					LcSubViewMapHourUpdate();
					break;
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoGrpHzDmap::LcMapIdxCalcBegin(void)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if(E_GrP5UoGrpHzDmapModeHourView == m_Mode)
		{
			Tv_Result	=	Tv_Result + (E_GrTimeMinOfHour * E_GrDvrMaxChCnt * (__u32)m_Hour);
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzDmap::LcSubViewMapDayUpdate(void)
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
			for(Tv_MrgIdx = 0; Tv_MrgIdx < m_DivDayUnit; Tv_MrgIdx++)
			{
				for(Tv_ChIdx = 0; Tv_ChIdx < m_SkinCtlGrpHzBase.CellCntY; Tv_ChIdx++)
				{
					m_ViewMap[Tv_ChIdx][Tv_VwX]	=	m_ViewMap[Tv_ChIdx][Tv_VwX] | m_PtrMap[Tv_MinIdx + Tv_ChIdx];
				}
				// next
				Tv_MinIdx	=	Tv_MinIdx + E_GrDvrMaxChCnt;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzDmap::LcSubViewMapHourUpdate(void)
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
void	Cls_GrP5UoGrpHzDmap::HourSet(__u8 A_Hour)
{
	// local -------------------
	// code --------------------
		m_Hour	=	A_Hour;

		if(E_GrP5UoGrpHzDmapModeHourView == m_Mode)
		{
			LcViewMapUpdate();
			ReqDraw(FALSE);
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzDmap::ChannelStartSet(__u8 A_Ch)
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
__u8	Cls_GrP5UoGrpHzDmap::ChannelStartGet(void)
{
		return	m_StartCh;
}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGrpHzDmap::ModeGet(void)
{
		return	m_Mode;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzDmap::ModeSet(__u8 A_Mode)
{
	// local -------------------
		__u32	Tv_CntX;
	// code --------------------
		
		switch(A_Mode)
		{
			case E_GrP5UoGrpHzDmapModeDayView:
				Tv_CntX	=	E_GrTimeMinOfDay / m_DivDayUnit;
				break;
			case E_GrP5UoGrpHzDmapModeHourView:
				Tv_CntX	=	E_GrTimeMinOfHour;
				break;
			default:
				Tv_CntX		=	0;
				break;
		}

		if(0 != Tv_CntX)
		{
			m_SkinCtlGrpHzBase.CellCntX			=	Tv_CntX;
			m_Mode			=	A_Mode;

			LcViewMapUpdate();

			ReqDraw(FALSE);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzDmap::LcParaValUpdate(void)
{
	// local -------------------
	// code --------------------
		// view line count
		if((0 != m_ParaUoGrpHzDmap.ViewLineCnt) && (E_GrDvrMaxChCnt > m_ParaUoGrpHzDmap.ViewLineCnt))
		{
			m_SkinCtlGrpHzBase.CellCntY	=	m_ParaUoGrpHzDmap.ViewLineCnt;
		}

		// day map unit
		switch(m_ParaUoGrpHzDmap.DayMapUnit)
		{
			case 0:
				m_DivDayUnit	=	3;
				break;
			case 1:
				m_DivDayUnit	=	4;
				break;
			case 2:
				m_DivDayUnit	=	6;
				break;
			case 3:
				m_DivDayUnit	=	12;
				break;
			default:
				m_DivDayUnit	=	60;
				break;
		}

		// recalculate count
		switch(m_Mode)
		{
			case E_GrP5UoGrpHzDmapModeDayView:
				m_SkinCtlGrpHzBase.CellCntX	=	E_GrTimeMinOfDay / m_DivDayUnit;
				break;
			case E_GrP5UoGrpHzDmapModeHourView:
				m_SkinCtlGrpHzBase.CellCntX	=	E_GrTimeMinOfHour;
				break;
		}

		
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrP5UoGrpHzDmap::SelectTimeGet(void)
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
void	Cls_GrP5UoGrpHzDmap::SelectTimeSetByHm(__u8 A_Hour, __u8 A_Min)
{
	// local -------------------
		__s32	Tv_IdxX;
		__u32	Tv_Min;
	// code --------------------
		if((24 > A_Hour) && (60 > A_Min))
		{
			if(E_GrP5UoGrpHzDmapModeHourView == m_Mode)
			{
				// hour mode
				Tv_IdxX		=	(__s32)A_Hour;
			}
			else
			{
				// day mode
				Tv_Min		=	((__u32)A_Hour * E_GrTimeMinOfHour) + (__u32)A_Min;
				Tv_IdxX		=	(__s32)(Tv_Min / m_DivDayUnit);
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
void	Cls_GrP5UoGrpHzDmap::SelectTimeSetByTime(Def_GrTime A_Time)
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
