/*
Platform 5 UI object arrow base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoArwStd.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5Mng.h>
#include <P5/GrP5Key.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <GrMath.h>

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

#define	E_GrP5UoArwStdParaInfoCnt							9

St_GrP5DvlScptParaInfo	V_GrP5UoArwStdParaInfo[E_GrP5UoArwStdParaInfoCnt]	=
{
	{L"Pannel arrow Standard", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL},
	{L"ClrRect0", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoArwStdPara)0)->ClrRect[0], TRUE, 0, L"color of rect"},
	{L"ClrRect1", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoArwStdPara)0)->ClrRect[1], TRUE, 0, L"color of rect"},
	{L"ClrRect2", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoArwStdPara)0)->ClrRect[2], TRUE, 0, L"color of rect"},
	{L"ClrRect3", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoArwStdPara)0)->ClrRect[3], TRUE, 0, L"color of rect"},
	{L"ClrRect4", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoArwStdPara)0)->ClrRect[4], TRUE, 0, L"color of rect"},
	{L"ClrRect5", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoArwStdPara)0)->ClrRect[5], TRUE, 0, L"color of rect"},
	{L"ClrRect6", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoArwStdPara)0)->ClrRect[6], TRUE, 0, L"color of rect"},
	{L"ClrRect7", E_GrVarTypeColor, 1, (void*)&((Ptr_GrP5UoArwStdPara)0)->ClrRect[7], TRUE, 0, L"color of rect"},
};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoArwStd::Cls_GrP5UoArwStd(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoArwBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
		__u32	Tv_PxFmt;
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdArwStd, &m_ParaUoArwStd, sizeof(m_ParaUoArwStd), &m_SkinCtlArwStd);
		//ClassRegist(E_GrP5ClassIdArwStd, NULL, 0);

		// default setting
		GrDumyZeroMem(&m_ParaUoArwStd, sizeof(m_ParaUoArwStd));
		GrDumyZeroMem(&m_SkinCtlArwStd, sizeof(m_SkinCtlArwStd));

		// reset
		GrDumyZeroMem(&m_RctCtl, sizeof(m_RctCtl));

		Tv_PxFmt	=	((Cls_GrP5Mng*)V_GrP5Env.ObjMng)->PixelFormatGet();

		m_ParaUoArwStd.ClrRect[0]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 0, 0), Tv_PxFmt);
		m_ParaUoArwStd.ClrRect[1]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 0, 255), Tv_PxFmt);
		m_ParaUoArwStd.ClrRect[2]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 0), Tv_PxFmt);
		m_ParaUoArwStd.ClrRect[3]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 0, 255), Tv_PxFmt);
		m_ParaUoArwStd.ClrRect[4]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 0, 255, 255), Tv_PxFmt);
		m_ParaUoArwStd.ClrRect[5]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 255, 255), Tv_PxFmt);
		m_ParaUoArwStd.ClrRect[6]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 192, 192, 192), Tv_PxFmt);
		m_ParaUoArwStd.ClrRect[7]	=	GrDrawCvtColorByFcc(E_GrFccARGB8888, Mac_GrMakeArgb32(255, 255, 192, 192), Tv_PxFmt);
			
		GrP5TimerRegist(this);
}
//--------------------------------------------------------------------
Cls_GrP5UoArwStd::~Cls_GrP5UoArwStd(void)
{
	// local -------------------
	// code --------------------
		GrP5TimerUnregist(this);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoArwStd::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoArwBase::EvtScptDecDone();
		
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoArwStd::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoArwBase::DvlParaInfoBuild();

		// separator basic info
		DvlParaInfoAdd(V_GrP5UoArwStdParaInfo, E_GrP5UoArwStdParaInfoCnt, &m_ParaUoArwStd);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoArwStd::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoArwBase::DvlParaInfoAction(A_Action);

		// custom action
		ReqDraw(FALSE);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoArwStd::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoArwBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoArwStd::EditIndexSet(__s32 A_Idx)
{
	// local -------------------
	// code --------------------
		// check index range
		if(E_GrP5SkinArwStdRectMaxCnt > A_Idx)
		{
			m_EditIdx	=	A_Idx;
			//DbgMsgPrint("[ARW] edit %d set\n", A_Idx);
		}
		
}
//--------------------------------------------------------------------
__s32	Cls_GrP5UoArwStd::EditIndexGet(void)
{
	// local -------------------
	// code --------------------
		return	m_EditIdx;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoArwStd::ItemInfoSet(__u32 A_Idx, __s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_Val)
{
	// local -------------------
	
	// code --------------------
		if(E_GrP5SkinArwStdRectMaxCnt <= A_Idx)
		{
			return	FALSE;
		}

		m_SkinCtlArwBase.ArwItms[A_Idx].St.x		=	(__s16)A_StX;
		m_SkinCtlArwBase.ArwItms[A_Idx].St.y		=	(__s16)A_StY;
		m_SkinCtlArwBase.ArwItms[A_Idx].Ed.x		=	(__s16)A_EdX;
		m_SkinCtlArwBase.ArwItms[A_Idx].Ed.y		=	(__s16)A_EdY;
		m_SkinCtlArwBase.ArwItms[A_Idx].DirMap	=	A_Val;

		LcDrawItemRect(A_Idx);	// redraw

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoArwStd::ItemInfoGet(__u32 A_Idx, __s32* A_PtrRtStX, __s32* A_PtrRtStY, __s32* A_PtrRtEdX, __s32* A_PtrRtEdY,
	__u32* A_PtrRtVal)
{
	// local -------------------
	// code --------------------
		if(E_GrP5SkinArwStdRectMaxCnt <= A_Idx)
		{
			return	FALSE;
		}

		*A_PtrRtStX	=	(__s32)m_SkinCtlArwBase.ArwItms[A_Idx].St.x;
		*A_PtrRtStY	=	(__s32)m_SkinCtlArwBase.ArwItms[A_Idx].St.y;
		*A_PtrRtEdX	=	(__s32)m_SkinCtlArwBase.ArwItms[A_Idx].Ed.x;
		*A_PtrRtEdY	=	(__s32)m_SkinCtlArwBase.ArwItms[A_Idx].Ed.y;
		*A_PtrRtVal	=	m_SkinCtlArwBase.ArwItms[A_Idx].DirMap;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoArwStd::ItemValueSet(__u32 A_Idx, __u32 A_Val)
{
	// local -------------------
	// code --------------------
		if(E_GrP5SkinArwStdRectMaxCnt <= A_Idx)
		{
			return	FALSE;
		}
		m_SkinCtlArwBase.ArwItms[A_Idx].DirMap	=	A_Val;

		LcDrawItemRect(A_Idx);	// redraw

		return	TRUE;
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoArwStd::ItemValueGet(__u32 A_Idx)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result		=	0;

		if(E_GrP5SkinArwStdRectMaxCnt > A_Idx)
		{
			Tv_Result		=	m_SkinCtlArwBase.ArwItms[A_Idx].DirMap;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoArwStd::RectangeListSet(__u8 A_MaxX, __u8 A_MaxY, __u32 A_TblCnt, Ptr_GrByteRect A_PtrTbl)
{
	// local -------------------
	// code --------------------
		m_RctCtl.TblCnt	=	A_TblCnt;
		if(E_GrP5SkinArwStdRectMaxCnt < A_TblCnt)
		{
			m_RctCtl.TblCnt		=	E_GrP5SkinArwStdRectMaxCnt;
		}

		m_RctCtl.MaxX		=	(__s32)((__u32)A_MaxX);
		m_RctCtl.MaxY		=	(__s32)((__u32)A_MaxY);
		m_RctCtl.PtrTbl	=	A_PtrTbl;


}
//--------------------------------------------------------------------
void	Cls_GrP5UoArwStd::EvtTimerTick(void)
{
	// local -------------------
	// code --------------------
		// rectangle display
		LcTmrRectDisp();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoArwStd::LcTmrRectDisp(void)
{
	// local -------------------
		St_GrRect	Tv_RcVw;
		__u32	Tv_ItmIdx;
		BOOL8	Tv_IsExistSr;
		BOOL8	Tv_IsChg;
	// code --------------------

		if(NULL == m_SkinMng)
		{
			return;
		}

		for(Tv_ItmIdx = 0; Tv_ItmIdx < E_GrP5SkinArwStdRectMaxCnt; Tv_ItmIdx++)
		{
			Tv_IsExistSr	=	FALSE;
			Tv_IsChg			=	FALSE;

			// check source
			if(NULL != m_RctCtl.PtrTbl)
			{
				if(0xFF != m_RctCtl.PtrTbl[Tv_ItmIdx].left)
				{
					Tv_IsExistSr	=	TRUE;

					m_SkinMng->CtlGetItemRangeRect(this, (__s32)m_RctCtl.PtrTbl[Tv_ItmIdx].left, (__s32)m_RctCtl.PtrTbl[Tv_ItmIdx].top,
						(__s32)m_RctCtl.PtrTbl[Tv_ItmIdx].right, (__s32)m_RctCtl.PtrTbl[Tv_ItmIdx].bottom, &Tv_RcVw);


#if 0
					DbgMsgPrint("[BR] l:%d t:%d r:%d b:%d\n", m_RctCtl.PtrTbl[Tv_ItmIdx].left, m_RctCtl.PtrTbl[Tv_ItmIdx].top, 
						m_RctCtl.PtrTbl[Tv_ItmIdx].right, m_RctCtl.PtrTbl[Tv_ItmIdx].bottom);
#endif
				}
			}

			// check exist view item
			if(0 < m_SkinCtlArwStd.RcTbl[Tv_ItmIdx].right)
			{
				// exist view item
				if(Tv_IsExistSr)
				{
					// compare 
					if((m_SkinCtlArwStd.RcTbl[Tv_ItmIdx].left != Tv_RcVw.left) || (m_SkinCtlArwStd.RcTbl[Tv_ItmIdx].right != Tv_RcVw.right) ||
						(m_SkinCtlArwStd.RcTbl[Tv_ItmIdx].top != Tv_RcVw.top) || (m_SkinCtlArwStd.RcTbl[Tv_ItmIdx].bottom != Tv_RcVw.bottom))
					{
						Tv_IsChg	=	TRUE;
					}
				}
				else
				{
					Tv_IsChg	=	TRUE;
				}

				// request old rect
				if(Tv_IsChg)
				{
					// draw old rect
					LcPntReqByRect(&m_SkinCtlArwStd.RcTbl[Tv_ItmIdx]);

					// update new rect
					if(Tv_IsExistSr)
					{
						m_SkinCtlArwStd.RcTbl[Tv_ItmIdx]	=	Tv_RcVw;	// update
						// draw new rect
						LcPntReqByRect(&m_SkinCtlArwStd.RcTbl[Tv_ItmIdx]);
					}
					else
					{
						// release view rect
						m_SkinCtlArwStd.RcTbl[Tv_ItmIdx].left			=	-1;
						m_SkinCtlArwStd.RcTbl[Tv_ItmIdx].right		=	-1;
						m_SkinCtlArwStd.RcTbl[Tv_ItmIdx].top			=	-1;
						m_SkinCtlArwStd.RcTbl[Tv_ItmIdx].bottom		=	-1;
					}
				}
			}
			else
			{
				// not exist view item
				if(Tv_IsExistSr)
				{
					m_SkinCtlArwStd.RcTbl[Tv_ItmIdx]	=	Tv_RcVw;	// update
					// draw new rect
					LcPntReqByRect(&m_SkinCtlArwStd.RcTbl[Tv_ItmIdx]);
				}
			}

		}	// for(Tv_ItmIdx = 0; Tv_ItmIdx < E_GrP5UoArwStdRctMaxCnt; Tv_ItmIdx++)
		
}
//--------------------------------------------------------------------
