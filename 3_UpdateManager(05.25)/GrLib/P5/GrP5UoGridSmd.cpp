/*
Platform 5 UI object grid base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoGridSmd.h>

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
#define	E_GrP5UoGridSmdParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoGridSmdParaInfo[E_GrP5UoGridSmdParaInfoCnt]	=
{
	{L"Grid Setup motion", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"StpAdr", E_GrP5DvlValTypeSetupAdr, 1, (void*)&((Ptr_GrP5UoGridSmdPara)0)->StpAdr, TRUE, 0, L"Setup address"},
	{L"ChSize", E_GrVarTypeU16, 1, (void*)&((Ptr_GrP5UoGridSmdPara)0)->ChSize, TRUE, 0, L"Setup channel data size"},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoGridSmd::Cls_GrP5UoGridSmd(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoGridBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdGridSmd, &m_ParaUoGridSmd, sizeof(m_ParaUoGridSmd), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoGridSmd, sizeof(m_ParaUoGridSmd));
		m_ParaUoGridSmd.ChSize	=	72;

		m_SkinCtlGridBase.CellCntX	=	22;
		m_SkinCtlGridBase.CellCntY	=	15;

		m_Ch				=	0;
		m_DrawMode	=	E_GrP5UoGridSmdDrawToggle;
}
//--------------------------------------------------------------------
Cls_GrP5UoGridSmd::~Cls_GrP5UoGridSmd(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGridSmd::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoGridBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoGridSmdParaInfo, E_GrP5UoGridSmdParaInfoCnt, &m_ParaUoGridSmd);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoGridSmd::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoGridBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSmd::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoGridSmd::CellValueGet(__u32 A_IdxX, __u32 A_IdxY)
{
	// local -------------------
		__u32	Tv_Result;
		__u16	Tv_Ofs;
		__u32	Tv_ValMap;
	// code --------------------
		Tv_Result	=	0;

		// calculate address
		Tv_Ofs		=	(__u16)(A_IdxY << 2) + (m_ParaUoGridSmd.ChSize * (__u16)m_Ch) + m_ParaUoGridSmd.StpAdr;
		Tv_ValMap	=	GrP5SetupGetU32(Tv_Ofs);
		if(0 != (Tv_ValMap & (1 << A_IdxX)))
		{
			Tv_Result	=	1;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSmd::ChannelSet(__u8 A_Ch)
{
	// local -------------------
	// code --------------------
		m_Ch	=	A_Ch;

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGridSmd::ChannelGet(void)
{
		return	m_Ch;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGridSmd::CellXyCntSet(__u32 A_CntX, __u32 A_CntY)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//if((32 >= A_CntX) && (18 >= A_CntY))
		if((32 >= A_CntX) && (32 >= A_CntY))
		{
			m_SkinCtlGridBase.CellCntX	=	A_CntX;
			m_SkinCtlGridBase.CellCntY	=	A_CntY;

			Tv_Result	=	TRUE;

			LcSkinCtlUpdate();

			ReqDraw(FALSE);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSmd::DoEvtGrpItemOneSel(void)
{
	// local -------------------
	// code --------------------
		if((0 <= m_SkinCtlGridBase.CusIdxX) && (0 <= m_SkinCtlGridBase.CusIdxY))
		{
			CellValueSet(m_SkinCtlGridBase.CusIdxX, m_SkinCtlGridBase.CusIdxY, m_DrawMode);
			// do event
			Cls_GrP5UoGridBase::DoEvtGrpItemOneSel();
			// redraw
			DrawReqItemRect(m_SkinCtlGridBase.CusIdxX, m_SkinCtlGridBase.CusIdxY, m_SkinCtlGridBase.CusIdxX, m_SkinCtlGridBase.CusIdxY);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSmd::DoEvtGrpItemAreaSel(void)
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
					CellValueSet(Tv_WkX, Tv_WkY, m_DrawMode);

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
void	Cls_GrP5UoGridSmd::CellValueSet(__u32 A_X, __u32 A_Y, __u8 A_Mode)
{
	// local -------------------
		__u16	Tv_Ofs;
		__u32	Tv_ValMap;
		__u32	Tv_Mask;
	// code --------------------
		// get orignal value
		Tv_Ofs		=	(__u16)(A_Y << 2) + (m_ParaUoGridSmd.ChSize * (__u16)m_Ch) + m_ParaUoGridSmd.StpAdr;
		Tv_ValMap	=	GrP5SetupGetU32(Tv_Ofs);

		Tv_Mask		=	1 << A_X;
		// update value
		switch(A_Mode)
		{
			case E_GrP5UoGridSmdDrawReset:
				Tv_ValMap	=	Tv_ValMap & (~Tv_Mask);
				break;
			case E_GrP5UoGridSmdDrawSet:
				Tv_ValMap	=	Tv_ValMap | Tv_Mask;
				break;
			default:
				Tv_ValMap	=	Tv_ValMap ^ Tv_Mask;
				break;
		}

		// upate value
		GrP5SetupPutU32(Tv_Ofs, Tv_ValMap);

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoGridSmd::EvtKeyUp(__u8 A_Key)
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
void	Cls_GrP5UoGridSmd::DrawModeSet(__u8 A_Mode)
{
		m_DrawMode	=	A_Mode;
}
//--------------------------------------------------------------------
__u8	Cls_GrP5UoGridSmd::DrawModeGet(void)
{
		return	m_DrawMode;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSmd::GridClear(void)
{
	// local -------------------
		__u16	Tv_Ofs;
		void*	Tv_PtrStp;
	// code --------------------
		if(0 != m_ParaUoGridSmd.ChSize)
		{
			Tv_Ofs		=	(m_ParaUoGridSmd.ChSize * (__u16)m_Ch) + m_ParaUoGridSmd.StpAdr;
			Tv_PtrStp	=	GrP5SetupOfsPtrGet(Tv_Ofs);
			GrDumyFillB(Tv_PtrStp, m_ParaUoGridSmd.ChSize, 0);

			ReqDraw(FALSE);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoGridSmd::GridFill(void)
{
	// local -------------------
		__u16	Tv_Ofs;
		void*	Tv_PtrStp;
	// code --------------------
		if(0 != m_ParaUoGridSmd.ChSize)
		{
			Tv_Ofs		=	(m_ParaUoGridSmd.ChSize * (__u16)m_Ch) + m_ParaUoGridSmd.StpAdr;
			Tv_PtrStp	=	GrP5SetupOfsPtrGet(Tv_Ofs);
			GrDumyFillB(Tv_PtrStp, m_ParaUoGridSmd.ChSize, 0xFF);

			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------
