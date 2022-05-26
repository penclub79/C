/*
Platform 5 UI object Label base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoPnlDir.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5Mng.h>
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

#define	E_GrP5UoPnlDirParaInfoCnt							2

St_GrP5DvlScptParaInfo	V_GrP5UoPnlDirParaInfo[E_GrP5UoPnlDirParaInfoCnt]	=
{
	{L"Pannel Aim", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL},
	{L"IsDiagonal", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoPnlDirPara)0)->IsDiagonal, TRUE, 0, L"Allow diagonal input"},
};

#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoPnlDir::Cls_GrP5UoPnlDir(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdPnlDir, &m_ParaUoPnlDir, sizeof(m_ParaUoPnlDir), &m_SkinCtlPnlDir);

		// default setting
		GrDumyZeroMem(&m_ParaUoPnlDir, sizeof(m_ParaUoPnlDir));
		GrDumyZeroMem(&m_SkinCtlPnlDir, sizeof(m_SkinCtlPnlDir));

		// reset
		m_UpdtStep	=	0;
		m_IsMsePush	=	FALSE;

		// request timer
		GrP5TimerRegist(this);
}
//--------------------------------------------------------------------
Cls_GrP5UoPnlDir::~Cls_GrP5UoPnlDir(void)
{
	// local -------------------
	// code --------------------
		
		//unregist timer
		GrP5TimerUnregist(this);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlDir::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlDir::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// separator basic info
		DvlParaInfoAdd(V_GrP5UoPnlDirParaInfo, E_GrP5UoPnlDirParaInfoCnt, &m_ParaUoPnlDir);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlDir::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		// custom action
		ReqDraw(FALSE);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlDir::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlDir::IsKeyAble(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlDir::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlDir::EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__u32	Tv_DirMap;
		__u32	Tv_DirLeft;
		__u32	Tv_DirRight;
		__u32	Tv_DirUp;
		__u32	Tv_DirDown;
		__u32	Tv_Value;
	// code --------------------
		Tv_DirMap	=	LcDirCalcByPos(A_X, A_Y);
		Tv_DirLeft		=	E_GrP5StatNormal;
		Tv_DirRight		=	E_GrP5StatNormal;
		Tv_DirUp			=	E_GrP5StatNormal;
		Tv_DirDown		=	E_GrP5StatNormal;

		Tv_Value			=	E_GrP5StatOnFocus;
		if(m_IsMsePush)
		{
			Tv_Value		=	E_GrP5StatPush;
		}

		if(0 != (Tv_DirMap & E_GrP5UoPnlDirMaskLeft))
		{
			Tv_DirLeft	=	Tv_Value;
		}
		if(0 != (Tv_DirMap & E_GrP5UoPnlDirMaskRight))
		{
			Tv_DirRight	=	Tv_Value;
		}
		if(0 != (Tv_DirMap & E_GrP5UoPnlDirMaskUp))
		{
			Tv_DirUp	=	Tv_Value;
		}
		if(0 != (Tv_DirMap & E_GrP5UoPnlDirMaskDown))
		{
			Tv_DirDown	=	Tv_Value;
		}

		LcDirSet(Tv_DirLeft, Tv_DirRight, Tv_DirUp, Tv_DirDown);

		LcNotifyEvtByMseStat();

		// inherited
		Cls_GrP5UoBase::EvtMseMove(A_X, A_Y, A_BtnMap);
		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlDir::EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__u32	Tv_DirMap;
		__u32	Tv_DirLeft;
		__u32	Tv_DirRight;
		__u32	Tv_DirUp;
		__u32	Tv_DirDown;
		__u32	Tv_Value;
	// code --------------------
		m_IsMsePush	=	TRUE;

		Tv_DirMap	=	LcDirCalcByPos(A_X, A_Y);
		Tv_DirLeft		=	E_GrP5StatNormal;
		Tv_DirRight		=	E_GrP5StatNormal;
		Tv_DirUp			=	E_GrP5StatNormal;
		Tv_DirDown		=	E_GrP5StatNormal;

		Tv_Value			=	E_GrP5StatOnFocus;
		if(m_IsMsePush)
		{
			Tv_Value		=	E_GrP5StatPush;
		}

		if(0 != (Tv_DirMap & E_GrP5UoPnlDirMaskLeft))
		{
			Tv_DirLeft	=	Tv_Value;
		}
		if(0 != (Tv_DirMap & E_GrP5UoPnlDirMaskRight))
		{
			Tv_DirRight	=	Tv_Value;
		}
		if(0 != (Tv_DirMap & E_GrP5UoPnlDirMaskUp))
		{
			Tv_DirUp	=	Tv_Value;
		}
		if(0 != (Tv_DirMap & E_GrP5UoPnlDirMaskDown))
		{
			Tv_DirDown	=	Tv_Value;
		}

		LcDirSet(Tv_DirLeft, Tv_DirRight, Tv_DirUp, Tv_DirDown);

		m_UpdtStep	=	0;
		LcNotifyEvtByMseStat();
		
		// inherited
		Cls_GrP5UoBase::EvtMseLbtnDown(A_X, A_Y, A_BtnMap);
		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlDir::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__u32	Tv_DirMap;
		__u32	Tv_DirLeft;
		__u32	Tv_DirRight;
		__u32	Tv_DirUp;
		__u32	Tv_DirDown;
		__u32	Tv_Value;
	// code --------------------
		m_IsMsePush	=	FALSE;

		Tv_DirMap	=	LcDirCalcByPos(A_X, A_Y);
		Tv_DirLeft		=	E_GrP5StatNormal;
		Tv_DirRight		=	E_GrP5StatNormal;
		Tv_DirUp			=	E_GrP5StatNormal;
		Tv_DirDown		=	E_GrP5StatNormal;

		Tv_Value			=	E_GrP5StatOnFocus;
		if(m_IsMsePush)
		{
			Tv_Value		=	E_GrP5StatPush;
		}

		if(0 != (Tv_DirMap & E_GrP5UoPnlDirMaskLeft))
		{
			Tv_DirLeft	=	Tv_Value;
		}
		if(0 != (Tv_DirMap & E_GrP5UoPnlDirMaskRight))
		{
			Tv_DirRight	=	Tv_Value;
		}
		if(0 != (Tv_DirMap & E_GrP5UoPnlDirMaskUp))
		{
			Tv_DirUp	=	Tv_Value;
		}
		if(0 != (Tv_DirMap & E_GrP5UoPnlDirMaskDown))
		{
			Tv_DirDown	=	Tv_Value;
		}

		LcDirSet(Tv_DirLeft, Tv_DirRight, Tv_DirUp, Tv_DirDown);

		// inherited
		Cls_GrP5UoBase::EvtMseLbtnUp(A_X, A_Y, A_BtnMap);
		return	TRUE;

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlDir::EvtMseLeave(void)
{
	// local -------------------
	// code --------------------
		m_IsMsePush	=	FALSE;
	
		// reset direction
		LcDirSet(E_GrP5StatNormal, E_GrP5StatNormal, E_GrP5StatNormal, E_GrP5StatNormal);

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlDir::EvtTimerTick(void)
{
	// local -------------------
	// code --------------------
		m_UpdtStep ++;
		if(E_GrP5UoPnlDirUpdtTimeStep <= m_UpdtStep)
		{
			if(m_IsMsePush)
			{
				LcNotifyEvtByMseStat();
			}
			// reset
			m_UpdtStep	=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlDir::LcNotifyDirEvt(BOOL8 A_IsLeft, BOOL8 A_IsRigth, BOOL8 A_IsUp, BOOL8 A_IsDown)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------

		// do control event
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			Tv_Ctrl->EvtDir(m_ParaUoBase.CtrlId, m_ObjId, A_IsLeft,A_IsRigth,A_IsUp,A_IsDown);
		}

}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoPnlDir::LcDirCalcByPos(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		__u32	Tv_Result;
		__s32	Tv_SizeX;
		__s32	Tv_SizeY;
		__s32	Tv_CtX;
		__s32	Tv_CtY;
	// code --------------------
		// init
		Tv_Result				=	0;

		// calculate x
		Tv_SizeX		=	m_RcScr.right - m_RcScr.left;
		Tv_CtX			=	Tv_SizeX >> 1;
		if(A_X < Tv_CtX)
		{
			if((Tv_CtX - A_X) >= (Tv_SizeX >> 3))
			{
				Tv_Result		=	Tv_Result | E_GrP5UoPnlDirMaskLeft;
			}
		}
		else
		{
			if((A_X - Tv_CtX) >= (Tv_SizeX >> 3))
			{
				Tv_Result		=	Tv_Result | E_GrP5UoPnlDirMaskRight;
			}
		}

		// calculate y
		Tv_SizeY		=	m_RcScr.bottom - m_RcScr.top;
		Tv_CtY			=	Tv_SizeY >> 1;

		if(A_Y < Tv_CtY)
		{
			if((Tv_CtY - A_Y) >= (Tv_SizeY >> 3))
			{
				if(!m_ParaUoPnlDir.IsDiagonal)
				{
					Tv_Result		=	0;
				}
				Tv_Result		=	Tv_Result | E_GrP5UoPnlDirMaskUp;
			}
		}
		else
		{
			if((A_Y - Tv_CtY) >= (Tv_SizeY >> 3))
			{
				if(!m_ParaUoPnlDir.IsDiagonal)
				{
					Tv_Result		=	0;
				}
				Tv_Result		=	Tv_Result | E_GrP5UoPnlDirMaskDown;
			}
		}


		return	Tv_Result;
		
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlDir::LcDirSet(__u32 A_Left, __u32 A_Right, __u32 A_Up, __u32 A_Down)
{
	// local -------------------
	// code --------------------

	
		if(m_SkinCtlPnlDir.StatLeft != A_Left)
		{
			m_SkinCtlPnlDir.StatLeft		=	A_Left;

			// request draw
			LcPntReqByRect(&m_SkinCtlPnlDir.RcLeft);
		}

		if(m_SkinCtlPnlDir.StatRight != A_Right )
		{
			m_SkinCtlPnlDir.StatRight		=	A_Right;

			// request draw
			LcPntReqByRect(&m_SkinCtlPnlDir.RcRight);
		}
	
		if(m_SkinCtlPnlDir.StatUp != A_Up)
		{
			m_SkinCtlPnlDir.StatUp		=	A_Up;

			// request draw
			LcPntReqByRect(&m_SkinCtlPnlDir.RcUp);
		}

		if(m_SkinCtlPnlDir.StatDown != A_Down)
		{
			m_SkinCtlPnlDir.StatDown		=	A_Down;

			// request draw
			LcPntReqByRect(&m_SkinCtlPnlDir.RcDown);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlDir::LcNotifyEvtByMseStat(void)
{
	// local -------------------
		BOOL8	Tv_IsLeft;
		BOOL8	Tv_IsRight;
		BOOL8	Tv_IsUp;
		BOOL8	Tv_IsDown;
	// code --------------------
		Tv_IsLeft			=	FALSE;
		Tv_IsRight		=	FALSE;
		Tv_IsUp				=	FALSE;
		Tv_IsDown			=	FALSE;

		if(E_GrP5StatPush == m_SkinCtlPnlDir.StatLeft)
		{
			Tv_IsLeft		=	TRUE;
		}

		if(E_GrP5StatPush == m_SkinCtlPnlDir.StatRight)
		{
			Tv_IsRight		=	TRUE;
		}

		if(E_GrP5StatPush == m_SkinCtlPnlDir.StatUp)
		{
			Tv_IsUp		=	TRUE;
		}

		if(E_GrP5StatPush == m_SkinCtlPnlDir.StatDown)
		{
			Tv_IsDown		=	TRUE;
		}

		if(Tv_IsLeft || Tv_IsRight || Tv_IsUp || Tv_IsDown)
		{
			LcNotifyDirEvt(Tv_IsLeft, Tv_IsRight, Tv_IsUp, Tv_IsDown);
		}

}
//--------------------------------------------------------------------

