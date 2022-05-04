/*
Platform 5 UI object grid base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoGridSrscd.h>

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
#define	E_GrP5UoGridSrscdParaInfoCnt							6

St_GrP5DvlScptParaInfo	V_GrP5UoGridSrscdParaInfo[E_GrP5UoGridSrscdParaInfoCnt]	=
{
	{L"Grid Setup record schedule", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"StpAdr", E_GrP5DvlValTypeSetupAdr, 1, (void*)&((Ptr_GrP5UoGridSrscdPara)0)->StpAdr, TRUE, 0, L"Setup address"},
	{L"IsHourUnit", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoGridSrscdPara)0)->IsHourUnit, TRUE, 0, L"Hour unit control mode"},
	{L"HolydayCnt", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoGridSrscdPara)0)->HolydayCnt, TRUE, 0, L"Holyday count, 0 ~ 2"},
	{L"IsHzWeek", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoGridSrscdPara)0)->IsHzWeek, TRUE, 0, L"horizontal week mode"},
	{L"IsHolydayOnly", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoGridSrscdPara)0)->IsHolydayOnly, TRUE, 0, L"holyday only mode"},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoGridSrscd::Cls_GrP5UoGridSrscd(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoGridBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdGridSrscd, &m_ParaUoGridSrscd, sizeof(m_ParaUoGridSrscd), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoGridSrscd, sizeof(m_ParaUoGridSrscd));

		m_SkinCtlGridBase.CellCntX	=	E_GrTimeHourOfDay * 2;
		m_SkinCtlGridBase.CellCntY	=	E_GrTimeDayOfWeek;

		m_Ch				=	0;
		m_DrawVal		=	0;
}
//--------------------------------------------------------------------
Cls_GrP5UoGridSrscd::~Cls_GrP5UoGridSrscd(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGridSrscd::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoGridBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoGridSrscdParaInfo, E_GrP5UoGridSrscdParaInfoCnt, &m_ParaUoGridSrscd);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGridSrscd::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoGridBase::DvlParaInfoAction(A_Action);

		LcCellCntUpdate();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSrscd::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		Cls_GrP5UoGridBase::EvtScptDecDone();

		LcCellCntUpdate();
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoGridSrscd::CellValueGet(__u32 A_IdxX, __u32 A_IdxY)
{
	// local -------------------
		__u32	Tv_Result;
		__u16	Tv_Ofs;
		__u32	Tv_HrIdx;
		__u32	Tv_WeIdx;
	// code --------------------
		Tv_Result	=	0;

		Tv_WeIdx		=	A_IdxY;
		Tv_HrIdx		=	A_IdxX;
		if(m_ParaUoGridSrscd.IsHzWeek)
		{
			Tv_WeIdx		=	A_IdxX;
			Tv_HrIdx		=	A_IdxY;
		}

		if(m_ParaUoGridSrscd.IsHolydayOnly)
		{
			Tv_WeIdx	=	Tv_WeIdx + 7;
		}

		// calculate address
		Tv_Ofs		=	(__u16)(Tv_WeIdx * E_GrTimeHourOfDay) + (E_GrP5UoGridSrscdChSize * (__u16)m_Ch) + m_ParaUoGridSrscd.StpAdr;
		if(m_ParaUoGridSrscd.IsHourUnit)
		{
			Tv_Ofs	=	Tv_Ofs + (__u16)Tv_HrIdx;
		}
		else
		{
			Tv_Ofs	=	Tv_Ofs + (__u16)(Tv_HrIdx >> 1);
		}
		Tv_Result		=	(__u32)GrP5SetupGetU8(Tv_Ofs);

		if(!m_ParaUoGridSrscd.IsHourUnit)
		{
			if(0 == (1 & Tv_HrIdx))
			{
				Tv_Result	=	Tv_Result & 0x0F;
			}
			else
			{
				Tv_Result	=	(Tv_Result >> 4) & 0x0F;
			}
		}

#ifdef GR_P5_EDITOR
		Tv_Result	=	A_IdxX & 0x07;
#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSrscd::ChannelSet(__u8 A_Ch)
{
	// local -------------------
	// code --------------------
		m_Ch	=	A_Ch;

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGridSrscd::ChannelGet(void)
{
		return	m_Ch;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGridSrscd::CellXyCntSet(__u32 A_CntX, __u32 A_CntY)
{
#if 1
		return	FALSE;
#else
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		return	FALSE;


		Tv_Result	=	FALSE;

		if(m_ParaUoGridSrscd.IsHzWeek)
		{
			if(18 >= A_CntX)
			{
				m_SkinCtlGridBase.CellCntX	=	A_CntX;

				Tv_Result	=	TRUE;

				LcSkinCtlUpdate();

				ReqDraw(FALSE);
			}
		}
		else
		{
			if(18 >= A_CntY)
			{
				m_SkinCtlGridBase.CellCntY	=	A_CntY;

				LcSkinCtlUpdate();

				Tv_Result	=	TRUE;

				ReqDraw(FALSE);
			}
		}

		return	Tv_Result;
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSrscd::DoEvtGrpItemOneSel(void)
{
	// local -------------------
	// code --------------------
		if((0 <= m_SkinCtlGridBase.CusIdxX) && (0 <= m_SkinCtlGridBase.CusIdxY))
		{
			CellValueSet(m_SkinCtlGridBase.CusIdxX, m_SkinCtlGridBase.CusIdxY, m_DrawVal);
			// do event
			Cls_GrP5UoGridBase::DoEvtGrpItemOneSel();
			// redraw
			DrawReqItemRect(m_SkinCtlGridBase.CusIdxX, m_SkinCtlGridBase.CusIdxY, m_SkinCtlGridBase.CusIdxX, m_SkinCtlGridBase.CusIdxY);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSrscd::DoEvtGrpItemAreaSel(void)
{
	// local -------------------
		__u32	Tv_StX;
		__u32	Tv_EdX;
		__u32	Tv_StY;
		__u32	Tv_EdY;
		__u32	Tv_WkX;
		__u32	Tv_WkY;
	// code --------------------
		if((0 <= m_SkinCtlGridBase.DragStX) && (0 <= m_SkinCtlGridBase.DragStY) && (0 <= m_SkinCtlGridBase.DragEdX) && (0 <= m_SkinCtlGridBase.DragEdY))
		{
			// update range
			Tv_StX	=	m_SkinCtlGridBase.DragStX;
			Tv_EdX	=	m_SkinCtlGridBase.DragEdX;
			if(Tv_StX > Tv_EdX)
			{
				Tv_StX	=	m_SkinCtlGridBase.DragEdX;
				Tv_EdX	=	m_SkinCtlGridBase.DragStX;
			}
			Tv_StY	=	m_SkinCtlGridBase.DragStY;
			Tv_EdY	=	m_SkinCtlGridBase.DragEdY;
			if(Tv_StY > Tv_EdY)
			{
				Tv_StY	=	m_SkinCtlGridBase.DragEdY;
				Tv_EdY	=	m_SkinCtlGridBase.DragStY;
			}

			Tv_WkY		=	Tv_StY;
			while(Tv_WkY <= Tv_EdY)
			{
				// x work
				Tv_WkX		=	Tv_StX;
				while(Tv_WkX <= Tv_EdX)
				{
					CellValueSet(Tv_WkX, Tv_WkY, m_DrawVal);

					// next
					Tv_WkX ++;
				}

				// next
				Tv_WkY ++;
			}

			// do event
			Cls_GrP5UoGridBase::DoEvtGrpItemAreaSel();
			// redraw
			DrawReqItemRect(m_SkinCtlGridBase.DragStX, m_SkinCtlGridBase.DragStY, m_SkinCtlGridBase.DragEdX, m_SkinCtlGridBase.DragEdY);
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSrscd::CellValueSet(__u32 A_X, __u32 A_Y, __u8 A_Val)
{
	// local -------------------
		__u16	Tv_Ofs;
		__u8	Tv_Val;
		__u32	Tv_HrIdx;
		__u32	Tv_WeIdx;
	// code --------------------
		// get index
		Tv_WeIdx		=	A_Y;
		Tv_HrIdx		=	A_X;
		if(m_ParaUoGridSrscd.IsHzWeek)
		{
			Tv_WeIdx		=	A_X;
			Tv_HrIdx		=	A_Y;
		}

		if(m_ParaUoGridSrscd.IsHolydayOnly)
		{
			Tv_WeIdx	=	Tv_WeIdx + 7;
		}

		// get orignal value
		Tv_Ofs		=	(__u16)(Tv_WeIdx * E_GrTimeHourOfDay) + (E_GrP5UoGridSrscdChSize * (__u16)m_Ch) + m_ParaUoGridSrscd.StpAdr;
		if(m_ParaUoGridSrscd.IsHourUnit)
		{
			Tv_Ofs	=	Tv_Ofs + (__u16)Tv_HrIdx;
		}
		else
		{
			Tv_Ofs	=	Tv_Ofs + (__u16)(Tv_HrIdx >> 1);
		}

		if(m_ParaUoGridSrscd.IsHourUnit)
		{
			Tv_Val	=	A_Val;
		}
		else
		{
			Tv_Val		=	GrP5SetupGetU8(Tv_Ofs);
			if(0 == (1 & Tv_HrIdx))
			{
				Tv_Val	=	(Tv_Val & 0xF0) | (A_Val & 0x0F);
			}
			else
			{
				Tv_Val	=	(Tv_Val & 0x0F) | (A_Val << 4);
			}
		}

		// write
		GrP5SetupPutU8(Tv_Ofs, Tv_Val);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGridSrscd::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(E_GrP5KeyEnter == A_Key)
		{
			// change value
			if((0 <= m_SkinCtlGridBase.CusIdxX) && (0 <= m_SkinCtlGridBase.CusIdxY))
			{
				DoEvtGrpItemOneSel();
			}
			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_Result	=	Cls_GrP5UoGridBase::EvtKeyUp(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSrscd::DrawValueSet(__u8 A_Val)
{
		m_DrawVal	=	A_Val;
}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGridSrscd::DrawValueGet(void)
{
		return	m_DrawVal;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSrscd::GridClear(void)
{
	// local -------------------
		__u16	Tv_Ofs;
		void*	Tv_PtrStp;
	// code --------------------

		Tv_Ofs		=	(E_GrP5UoGridSrscdChSize * (__u16)m_Ch) + m_ParaUoGridSrscd.StpAdr;
		Tv_PtrStp	=	GrP5SetupOfsPtrGet(Tv_Ofs);
		GrDumyFillB(Tv_PtrStp, E_GrP5UoGridSrscdChSize, 0);

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSrscd::GridFill(void)
{
	// local -------------------
		__u8	Tv_Val;
		__u16	Tv_Ofs;
		void*	Tv_PtrStp;
	// code --------------------
		Tv_Ofs		=	(E_GrP5UoGridSrscdChSize * (__u16)m_Ch) + m_ParaUoGridSrscd.StpAdr;
		Tv_PtrStp	=	GrP5SetupOfsPtrGet(Tv_Ofs);
		Tv_Val		=	m_DrawVal;
		if(!m_ParaUoGridSrscd.IsHourUnit)
		{
			Tv_Val	=	m_DrawVal | (m_DrawVal << 4);
		}
		GrDumyFillB(Tv_PtrStp, E_GrP5UoGridSrscdChSize, Tv_Val);

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSrscd::LcCellCntUpdate(void)
{
	// local -------------------
		__u32	Tv_HolyCnt;
	// code --------------------
		Tv_HolyCnt	=	(__u32)m_ParaUoGridSrscd.HolydayCnt;
		if(E_GrP5UoGridSrscdHolydayCnt < Tv_HolyCnt)
		{
			Tv_HolyCnt	=	E_GrP5UoGridSrscdHolydayCnt;
		}

		if(m_ParaUoGridSrscd.IsHzWeek)
		{
			m_SkinCtlGridBase.CellCntX		=	Tv_HolyCnt;
			if(!m_ParaUoGridSrscd.IsHolydayOnly)
			{
				m_SkinCtlGridBase.CellCntX		=	m_SkinCtlGridBase.CellCntX + E_GrTimeDayOfWeek;
			}

			if(m_ParaUoGridSrscd.IsHourUnit)
			{
				m_SkinCtlGridBase.CellCntY	=	E_GrTimeHourOfDay;
			}
			else
			{
				m_SkinCtlGridBase.CellCntY	=	E_GrTimeHourOfDay * 2;
			}
		}
		else
		{
			m_SkinCtlGridBase.CellCntY		=	Tv_HolyCnt;
			if(!m_ParaUoGridSrscd.IsHolydayOnly)
			{
				m_SkinCtlGridBase.CellCntY		=	m_SkinCtlGridBase.CellCntY + E_GrTimeDayOfWeek;
			}

			if(m_ParaUoGridSrscd.IsHourUnit)
			{
				m_SkinCtlGridBase.CellCntX	=	E_GrTimeHourOfDay;
			}
			else
			{
				m_SkinCtlGridBase.CellCntX	=	E_GrTimeHourOfDay * 2;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSrscd::DrawReqItemDrag(__u32 A_IdxStX, __u32 A_IdxStY, __u32 A_IdxEdX, __u32 A_IdxEdY)
{
	// local -------------------
		St_GrRect	Tv_RcBase;
		St_GrRect	Tv_RcTbl[4];
		__u32	Tv_RcIdx;
		__u32	Tv_RcCnt;
	// code --------------------
		if(NULL == m_SkinMng)
		{
			return;
		}

		m_SkinMng->CtlGetItemRangeRect(this, (__s32)A_IdxStX, (__s32)A_IdxStY, (__s32)A_IdxEdX, (__s32)A_IdxEdY, &Tv_RcBase);
		if(0 < Tv_RcBase.right)
		{
			Tv_RcCnt	=	0;
			// top
			Tv_RcTbl[Tv_RcCnt].left		=	Tv_RcBase.left;
			Tv_RcTbl[Tv_RcCnt].top		=	Tv_RcBase.top;
			Tv_RcTbl[Tv_RcCnt].right	=	Tv_RcBase.right;
			Tv_RcTbl[Tv_RcCnt].bottom	=	Tv_RcBase.top + (__s32)m_SkinCtlGridBase.CellSizeY;
			Tv_RcCnt ++;

			// bottom
			if(A_IdxStY != A_IdxEdY)
			{
				Tv_RcTbl[Tv_RcCnt].left		=	Tv_RcBase.left;
				Tv_RcTbl[Tv_RcCnt].top		=	Tv_RcBase.bottom - (__s32)m_SkinCtlGridBase.CellSizeY;
				Tv_RcTbl[Tv_RcCnt].right	=	Tv_RcBase.right;
				Tv_RcTbl[Tv_RcCnt].bottom	=	Tv_RcBase.bottom;
				Tv_RcCnt ++;

				if((Tv_RcBase.top + (__s32)m_SkinCtlGridBase.CellSizeY) < Tv_RcBase.bottom)
				{
					// left
					Tv_RcTbl[Tv_RcCnt].left		=	Tv_RcBase.left;
					Tv_RcTbl[Tv_RcCnt].top		=	Tv_RcBase.top + (__s32)m_SkinCtlGridBase.CellSizeY;
					Tv_RcTbl[Tv_RcCnt].right	=	Tv_RcBase.left + (__s32)m_SkinCtlGridBase.CellSizeX;
					Tv_RcTbl[Tv_RcCnt].bottom	=	Tv_RcBase.bottom - (__s32)m_SkinCtlGridBase.CellSizeY;
					Tv_RcCnt ++;

					if((Tv_RcBase.left + (__s32)m_SkinCtlGridBase.CellSizeX) < Tv_RcBase.right)
					{
						// right
						Tv_RcTbl[Tv_RcCnt].left		=	Tv_RcBase.right - (__s32)m_SkinCtlGridBase.CellSizeX;
						Tv_RcTbl[Tv_RcCnt].top		=	Tv_RcBase.top + (__s32)m_SkinCtlGridBase.CellSizeY;
						Tv_RcTbl[Tv_RcCnt].right	=	Tv_RcBase.right;
						Tv_RcTbl[Tv_RcCnt].bottom	=	Tv_RcBase.bottom - (__s32)m_SkinCtlGridBase.CellSizeY;
						Tv_RcCnt ++;
					}
				}

			}

			for(Tv_RcIdx = 0; Tv_RcIdx < Tv_RcCnt; Tv_RcIdx++)
			{
				LcPntReqByRect(&Tv_RcTbl[Tv_RcIdx]);
			}
		}

}
//--------------------------------------------------------------------
