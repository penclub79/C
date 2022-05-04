/*
Platform 5 UI object graph horizontal time map type 2

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoGrpHzT2.h>

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
#define	E_GrP5UoGrpHzT2ParaInfoCnt							8

St_GrP5DvlScptParaInfo	V_GrP5UoGrpHzT2ParaInfo[E_GrP5UoGrpHzT2ParaInfoCnt]	=
{
	{L"Graph horizontal map Type 1", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"RulerFontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGrpHzT2Para)0)->RulerFontId, TRUE, 0, L"Ruler font id"},
	{L"RulerHeight", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGrpHzT2Para)0)->RulerHeight, TRUE, 0, L"Ruler height"},
	{L"IsMergeCh", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoGrpHzT2Para)0)->IsMergeCh, TRUE, 0, L"Merge all channel"},
	{L"IsAutoCus", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoGrpHzT2Para)0)->IsAutoCus, TRUE, 0, L"Auto cursor position"},
	{L"RulerTxtWidth", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGrpHzT2Para)0)->RulerTxtWidth, TRUE, 0, L"Ruler text width"},
	{L"ViewLineCnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGrpHzT2Para)0)->ViewLineCnt, TRUE, 0, L"View line count"},
	{L"RegCusTime", E_GrP5DvlValTypeRintId, 1, (void*)&((Ptr_GrP5UoGrpHzT2Para)0)->RegCusTime, TRUE, 0, L"Cursor time store register"},

};

extern	St_GrDvrRecMinMap	V_GrP5DvlTestRecMap[E_GrTimeMinOfDay];

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoGrpHzT2::Cls_GrP5UoGrpHzT2(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoGrpHzBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdGrpHzT2, &m_ParaUoGrpHzT2, sizeof(m_ParaUoGrpHzT2), &m_SkinCtlGrpHzT2);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoGrpHzT2, sizeof(m_ParaUoGrpHzT2));
		m_ParaUoGrpHzT2.ViewLineCnt	=	4;
		m_ParaUoGrpHzT2.RulerTxtStyle.Atb	=	E_GrP5FontAtbDrawText;
		m_ParaUoGrpHzT2.RulerHeight				=	26;

		GrDumyZeroMem(&m_SkinCtlGrpHzT2, sizeof(m_SkinCtlGrpHzT2));
		GrDumyZeroMem(m_TxtBufs, sizeof(m_TxtBufs));

		m_TimeLastOvEvt	=	0xFFFFFFFF;

		// skin control init

		LcTxtBufInit();
		
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoGrpHzT2.RulerTxtStyle);


		m_SkinCtlGrpHzT2.PtrAtbBuf	=	(__u8*)&V_GrP5DvlTestRecMap;
#endif

		// timer
		GrP5TimerRegist(this);



}
//--------------------------------------------------------------------
Cls_GrP5UoGrpHzT2::~Cls_GrP5UoGrpHzT2(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);

		GrP5TimerUnregist(this);
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGrpHzT2::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoGrpHzBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoGrpHzT2ParaInfo, E_GrP5UoGrpHzT2ParaInfoCnt, &m_ParaUoGrpHzT2);
		DvlParaInfoTxtParaAdd(L"Ruler text style", &m_ParaUoGrpHzT2.RulerTxtStyle);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGrpHzT2::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoGrpHzBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoGrpHzBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::MapBufferSet(__u8* A_PtrMap, __u32 A_TimeDay)
{
	// local -------------------
	// code --------------------
		m_SkinCtlGrpHzT2.PtrAtbBuf	=	A_PtrMap;
		m_SkinCtlGrpHzT2.TimeLoad		=	A_TimeDay - (A_TimeDay % E_GrTimeSecOfDay);

		// update view map
		LcSkinCtlUpdate();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::TimeSet(Def_GrTime A_Time)
{
	// local -------------------
	// code --------------------
		GrP5RintSet(m_ParaUoGrpHzT2.RegCusTime, (__s32)A_Time);
		if(!m_ParaUoGrpHzT2.IsAutoCus)
		{
			// direct cursor change
			LcCusTimeSet(A_Time);
		}
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrP5UoGrpHzT2::TimeGet(void)
{
		return	m_SkinCtlGrpHzT2.TimeCus;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::ChannelStartSet(__u8 A_Ch)
{
	// local -------------------
	// code --------------------
		if(E_GrDvrMaxChCnt > A_Ch)
		{
			m_SkinCtlGrpHzT2.StartCh	=	A_Ch;
			LcSkinCtlUpdate();
			ReqDraw(FALSE);
		}

}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGrpHzT2::ChannelStartGet(void)
{
		return	m_SkinCtlGrpHzT2.StartCh;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::ZoomLvlSet(__u8 A_Level)
{
	// local -------------------
		//__u32	Tv_CntX;
	// code --------------------
		
		m_SkinCtlGrpHzT2.ZoomLvl	=	A_Level;

		LcSkinCtlUpdate();

		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGrpHzT2::ZoomLvlGet(void)
{
		return	m_SkinCtlGrpHzT2.ZoomLvl;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::LcRedrawCus(void)
{
	// local -------------------
		St_GrRect	Tv_RcDraw;
		__u32	Tv_PxCus;

	// code --------------------
		if(NULL == m_SkinMng)
		{
			return;
		}

		if((m_SkinCtlGrpHzT2.TimeCus < m_SkinCtlGrpHzT2.TimeScl) || (m_SkinCtlGrpHzT2.TimeCus >= (m_SkinCtlGrpHzT2.TimeScl + m_SkinCtlGrpHzT2.TimeLen)))
		{
			return;
		}

		// redraw
		Tv_PxCus	=	(m_SkinCtlGrpHzT2.TimeCus - m_SkinCtlGrpHzT2.TimeScl) / m_SkinCtlGrpHzT2.PxPerSec;
		Tv_RcDraw.left		=	m_SkinCtlGrpHzT2.RcContArea.left + (__s32)Tv_PxCus;
		Tv_RcDraw.right		=	Tv_RcDraw.left + 1;
		Tv_RcDraw.top			=	m_SkinCtlGrpHzT2.RcRule.top;
		Tv_RcDraw.bottom	=	m_SkinCtlGrpHzT2.RcContArea.bottom;

		LcPntReqByRect(&Tv_RcDraw);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::LcCusTimeSet(Def_GrTime A_Time)
{
	// local -------------------
	// code --------------------

		if(m_SkinCtlGrpHzT2.TimeCus == A_Time)
		{
			return;
		}

		// draw previous position
		LcRedrawCus();

		// update new position
		m_SkinCtlGrpHzT2.TimeCus	=	A_Time;

		LcRedrawCus();

}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::LineCntSet(__u8 A_Cnt)
{
	// local -------------------
	// code --------------------
		m_ParaUoGrpHzT2.ViewLineCnt	=	A_Cnt;
		LcSkinCtlUpdate();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGrpHzT2::LineCntGet(void)
{
		return	m_ParaUoGrpHzT2.ViewLineCnt;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::LcTxtBufInit(void)
{
	// local -------------------
		__u32	Tv_ItmIdx;
		__u32	Tv_BufIdx;
		__u32	Tv_WkIdx;
		WCHAR*	Tv_StrWk;
	// code --------------------
		Tv_ItmIdx	=	0;
		for(Tv_BufIdx = 0; Tv_BufIdx < E_GrP5UoGrpHzT2TxtBufCnt; Tv_BufIdx++)
		{
			m_TxtBufs[Tv_BufIdx]	=	GrP5TxtBufAlloc(m_ObjId);
			if(NULL == m_TxtBufs[Tv_BufIdx])
			{
				break;
			}

			Tv_StrWk	=	m_TxtBufs[Tv_BufIdx];

			for( Tv_WkIdx = 0; Tv_WkIdx < 21; Tv_WkIdx++)
			{
				m_SkinCtlGrpHzT2.RulTxtTbl[Tv_ItmIdx].Str	=	Tv_StrWk;
				// next
				Tv_StrWk	=	(WCHAR*)((Def_GrCalPtr)Tv_StrWk + 12);
				Tv_ItmIdx ++;
			}

		}
		
}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGrpHzT2::ItemValueGet(__u32 A_IdxX, __u32 A_IdxY)
{
	return	0;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGrpHzT2::IsKeyAble(void)
{
	return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGrpHzT2::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32	Tv_ScrX;
		__s32	Tv_ScrY;
		__u32	Tv_TimeSel;
	// code --------------------
		if(NULL == m_SkinMng)
		{
			return	TRUE;
		}

		if(0 == m_SkinCtlGrpHzT2.PxPerSec)
		{
			return	TRUE;
		}

		// get position
		Tv_ScrX	=	A_X + m_RcScr.left;
		Tv_ScrY	=	A_Y + m_RcScr.top;

		// calculate position
		if((Tv_ScrX < m_SkinCtlGrpHzT2.RcContArea.left) || (Tv_ScrX >= m_SkinCtlGrpHzT2.RcContArea.right) || 
			(Tv_ScrY < m_SkinCtlGrpHzT2.RcContArea.top) || (Tv_ScrY >= m_SkinCtlGrpHzT2.RcContArea.bottom))
		{
			return	TRUE;
		}
		
		// calculate time
		Tv_TimeSel	=	((__u32)(Tv_ScrX - m_SkinCtlGrpHzT2.RcContArea.left) * m_SkinCtlGrpHzT2.PxPerSec) + m_SkinCtlGrpHzT2.TimeScl;
		m_SkinCtlGrpHzBase.SelIdxX	=	(__s32)Tv_TimeSel;
		m_SkinCtlGrpHzBase.SelIdxY	=	-1;

		if(!m_ParaUoGrpHzT2.IsMergeCh)
		{
			// calculate channel
			m_SkinCtlGrpHzBase.SelIdxY	=	(__s32)((__u32)m_SkinCtlGrpHzT2.StartCh + ((__u32)(Tv_ScrY - m_SkinCtlGrpHzT2.RcContArea.top) / (__u32)m_ParaUoGrpHzBase.LineHeight));
			if(E_GrDvrMaxChCnt <= m_SkinCtlGrpHzBase.SelIdxY)
			{
				m_SkinCtlGrpHzBase.SelIdxY	=	-1;
			}
		}

		// do event
		DoEvtGrpItemClick();
		
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::EvtTimerTick(void)
{
	// local -------------------
		__u32	Tv_TimeVal;
		__u32	Tv_Dir;
	// code --------------------
		if(m_ParaUoGrpHzT2.IsAutoCus)
		{
			Tv_TimeVal	=	(__u32)GrP5RintGet(m_ParaUoGrpHzT2.RegCusTime);
			LcCusTimeSet(Tv_TimeVal);

			if(m_ParaUoGrpHzBase.IsEvtOutRange)
			{
				if((Tv_TimeVal < m_SkinCtlGrpHzT2.TimeLoad) || (Tv_TimeVal >= (m_SkinCtlGrpHzT2.TimeLoad + E_GrTimeSecOfDay)))
				{
					if(m_TimeLastOvEvt != Tv_TimeVal)
					{
						Tv_Dir		=	1;
						if(Tv_TimeVal < m_SkinCtlGrpHzT2.TimeLoad)
						{
							Tv_Dir		=	0;
						}
						LcDoEvtCusOver(Tv_Dir);
						// update last over
						m_TimeLastOvEvt	=	Tv_TimeVal;
					}
				}
				else
				{
					// in range
					m_TimeLastOvEvt	=	0xFFFFFFFF;
				}
			}

		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::ScrollLeft(__u32 A_Step)
{
	// local -------------------
		__u32	Tv_TimeMov;
	// code --------------------
		if(NULL == m_SkinMng)
		{
			return;
		}

		if(0 == m_SkinCtlGrpHzT2.PxPerSec)
		{
			return;
		}

		Tv_TimeMov	=	m_SkinCtlGrpHzT2.RulPerSec * A_Step;
		if((m_SkinCtlGrpHzT2.TimeScl - m_SkinCtlGrpHzT2.TimeLoad) <= Tv_TimeMov)
		{
			m_SkinCtlGrpHzT2.TimeScl	=	m_SkinCtlGrpHzT2.TimeLoad;
		}
		else
		{
			m_SkinCtlGrpHzT2.TimeScl	=	m_SkinCtlGrpHzT2.TimeScl - Tv_TimeMov;
		}
		
		LcSkinCtlUpdate();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::ScrollRight(__u32 A_Step)
{
	// local -------------------
		__u32	Tv_TimeMov;
	// code --------------------
		if(NULL == m_SkinMng)
		{
			return;
		}

		if(0 == m_SkinCtlGrpHzT2.PxPerSec)
		{
			return;
		}

		Tv_TimeMov	=	m_SkinCtlGrpHzT2.RulPerSec * A_Step;
		if((m_SkinCtlGrpHzT2.TimeScl + m_SkinCtlGrpHzT2.TimeLen + Tv_TimeMov) >= (m_SkinCtlGrpHzT2.TimeLoad + E_GrTimeSecOfDay))
		{
			m_SkinCtlGrpHzT2.TimeScl	=	m_SkinCtlGrpHzT2.TimeLoad + E_GrTimeSecOfDay - m_SkinCtlGrpHzT2.TimeLen;
		}
		else
		{
			m_SkinCtlGrpHzT2.TimeScl	=	m_SkinCtlGrpHzT2.TimeScl + Tv_TimeMov;
		}

		LcSkinCtlUpdate();
		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoGrpHzT2::ScrollByTime(Def_GrTime A_Time)
{
	// local -------------------
	// code --------------------
		if(NULL == m_SkinMng)
		{
			return;
		}

		if(0 == m_SkinCtlGrpHzT2.PxPerSec)
		{
			return;
		}

		if((A_Time < m_SkinCtlGrpHzT2.TimeLoad) || (A_Time >= (m_SkinCtlGrpHzT2.TimeLoad + E_GrTimeSecOfDay)))
		{
			return;
		}

		m_SkinCtlGrpHzT2.TimeScl	=	A_Time - (m_SkinCtlGrpHzT2.TimeLen >> 1);

		LcSkinCtlUpdate();
		ReqDraw(FALSE);

}
//--------------------------------------------------------------------
