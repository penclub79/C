/*
Platform 5 UI object calendar base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoCalMark.h>

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
#define	E_GrP5UoCalMarkParaInfoCnt							2

St_GrP5DvlScptParaInfo	V_GrP5UoCalMarkParaInfo[E_GrP5UoCalMarkParaInfoCnt]	=
{
	{L"Calendar Mark", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"IsCanSelOnMark", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoCalMarkPara)0)->IsCanSelOnMark, TRUE, 0, L"You can select on checked item only."},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoCalMark::Cls_GrP5UoCalMark(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoCalBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdCalMark, &m_ParaUoCalMark, sizeof(m_ParaUoCalMark), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoCalMark, sizeof(m_ParaUoCalMark));
		m_ParaUoCalMark.IsCanSelOnMark	=	FALSE;

}
//--------------------------------------------------------------------
Cls_GrP5UoCalMark::~Cls_GrP5UoCalMark(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCalMark::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCalBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoCalMarkParaInfo, E_GrP5UoCalMarkParaInfoCnt, &m_ParaUoCalMark);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoCalMark::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoCalBase::DvlParaInfoAction(A_Action);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoCalMark::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoCalBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
void	Cls_GrP5UoCalMark::ValueMapSet(__u32 A_Map)
{
	// local -------------------
		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
		__u8	Tv_DayIdx;
		__u32	Tv_ChkMask;
	// code --------------------
		if((0 != m_SkinCtlCalBase.ViewYear) && (0 != m_SkinCtlCalBase.ViewMonth))
		{
			Tv_IdxX	=	(__u32)m_StartWeek;
			Tv_IdxY	=	0;
			Tv_ChkMask	=	1;
			for( Tv_DayIdx = 0; Tv_DayIdx < m_DayCnt; Tv_DayIdx++)
			{
				if(0 != m_SkinCtlCalBase.DayTbl[Tv_IdxY][Tv_IdxX].Day)
				{
					if(0 == (Tv_ChkMask & A_Map))
					{
						m_SkinCtlCalBase.DayTbl[Tv_IdxY][Tv_IdxX].MarkMap	=	0;
					}
					else
					{
						m_SkinCtlCalBase.DayTbl[Tv_IdxY][Tv_IdxX].MarkMap	=	1;
					}
				}

				// next
				Tv_ChkMask	=	Tv_ChkMask << 1;

				Tv_IdxX ++;
				if(E_GrTimeDayOfWeek <= Tv_IdxX)
				{
					Tv_IdxX		=	0;
					Tv_IdxY		++;
				}

			}
		}

		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoCalMark::ValueMapGet(void)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
		__u8	Tv_DayIdx;
		__u32	Tv_ChkMask;
	// code --------------------
		Tv_Result	=	0;

		if((0 != m_SkinCtlCalBase.ViewYear) && (0 != m_SkinCtlCalBase.ViewMonth))
		{
			Tv_IdxX	=	(__u32)m_StartWeek;
			Tv_IdxY	=	0;
			Tv_ChkMask	=	1;
			for(Tv_DayIdx = 0; Tv_DayIdx < m_DayCnt; Tv_DayIdx++)
			{
				if(0 != m_SkinCtlCalBase.DayTbl[Tv_IdxY][Tv_IdxX].Day)
				{
					if(0 != m_SkinCtlCalBase.DayTbl[Tv_IdxY][Tv_IdxX].MarkMap)
					{
						Tv_Result	=	Tv_Result | Tv_ChkMask;
					}
				}

				// next
				Tv_ChkMask	=	Tv_ChkMask << 1;

				Tv_IdxX ++;
				if(E_GrTimeDayOfWeek <= Tv_IdxX)
				{
					Tv_IdxX		=	0;
					Tv_IdxY		++;
				}

			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
