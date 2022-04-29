/*
Platform 5 UI object Label base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoPnlDrag.h>

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

#if 0
#ifdef GR_P5_EDITOR

#define	E_GrP5UoPnlDragParaInfoCnt							2

St_GrP5DvlScptParaInfo	V_GrP5UoPnlDragParaInfo[E_GrP5UoPnlDragParaInfoCnt]	=
{
	{L"Pannel Aim", E_GrP5DvlValTypeSeparator, 0, NULL, FALSE, 0, NULL},
	{L"IsDiagonal", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoPnlDragPara)0)->IsDiagonal, TRUE, 0, L"Allow diagonal input"},
};

#endif
#endif


//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoPnlDrag::Cls_GrP5UoPnlDrag(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBase(A_ScptId,A_ObjPrnt,A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		//ClassRegist(E_GrP5ClassIdPnlDrag, &m_ParaUoPnlDrag, sizeof(m_ParaUoPnlDrag), &m_SkinCtlPnlDrag);
		ClassRegist(E_GrP5ClassIdPnlDrag, NULL, 0, &m_SkinCtlPnlDrag);

		// default setting
		//GrDumyZeroMem(&m_ParaUoPnlDrag, sizeof(m_ParaUoPnlDrag));
		GrDumyZeroMem(&m_SkinCtlPnlDrag, sizeof(m_SkinCtlPnlDrag));

		// reset
		m_IsMsePush		=	FALSE;
		m_IsDragAble	=	TRUE;

}
//--------------------------------------------------------------------
Cls_GrP5UoPnlDrag::~Cls_GrP5UoPnlDrag(void)
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlDrag::EvtScptDecDone(void)
{
		// inherited
		Cls_GrP5UoBase::EvtScptDecDone();
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlDrag::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5UoBase::DvlParaInfoBuild();

		// separator basic info
		//DvlParaInfoAdd(V_GrP5UoPnlDragParaInfo, E_GrP5UoPnlDragParaInfoCnt, &m_ParaUoPnlDrag);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlDrag::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBase::DvlParaInfoAction(A_Action);

		// custom action
		ReqDraw(FALSE);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoPnlDrag::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// delete prepare
		

		// inherited
		Cls_GrP5UoBase::DvlPreDelete();
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlDrag::IsKeyAble(void)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlDrag::IsMseAble(void)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlDrag::EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32	Tv_X;
		__s32	Tv_Y;
	// code --------------------
		if(m_IsMsePush)
		{
			Tv_X		=	m_RcScr.left + A_X;
			Tv_Y		=	m_RcScr.top + A_Y;

			LcDragRectSet(Tv_X, Tv_Y);
		}

		// inherited
		Cls_GrP5UoBase::EvtMseMove(A_X, A_Y, A_BtnMap);
		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlDrag::EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
	// code --------------------
		// check able
		if(m_IsDragAble)
		{
			// capture
			MseCapture();

			m_IsMsePush	=	TRUE;

			// update start position
			m_StX		=	m_RcScr.left + A_X;
			m_StY		=	m_RcScr.top + A_Y;

			// set by pos
			LcDragRectSet(m_StX, m_StY);

		}
	
		// inherited
		Cls_GrP5UoBase::EvtMseLbtnDown(A_X, A_Y, A_BtnMap);
		return	TRUE;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoPnlDrag::EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		__s32	Tv_X;
		__s32	Tv_Y;
	// code --------------------
		if(m_IsMsePush)
		{
			MseRelese();
			m_IsMsePush	=	FALSE;

			Tv_X		=	m_RcScr.left + A_X;
			Tv_Y		=	m_RcScr.top + A_Y;

			LcDragRectSet(Tv_X, Tv_Y);

			// do event
			LcNotifyDragEvt();

		}
		
		// inherited
		Cls_GrP5UoBase::EvtMseLbtnUp(A_X, A_Y, A_BtnMap);
		return	TRUE;

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlDrag::LcNotifyDragEvt(void)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
	// code --------------------

		// do control event
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_ParaUoBase.CtrlId);
		if(NULL != Tv_Ctrl)
		{
			Tv_Ctrl->EvtDragArea(m_ParaUoBase.CtrlId, m_ObjId, &m_SkinCtlPnlDrag.RcDrag, &m_RcScr);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlDrag::DragAbleSet(BOOL8 A_IsAble)
{
		m_IsDragAble	=	A_IsAble;

		if(!m_IsDragAble)
		{
			if(0 != m_SkinCtlPnlDrag.RcDrag.right)
			{
				LcPntReqByRect(&m_SkinCtlPnlDrag.RcDrag);
			}
			m_SkinCtlPnlDrag.RcDrag.right	=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoPnlDrag::LcDragRectSet(__s32 A_X, __s32 A_Y)
{
	// local -------------------
		St_GrRect	Tv_RcNew;
	// code --------------------
		// req draw previous area
		if(0 < m_SkinCtlPnlDrag.RcDrag.right)
		{
			LcPntReqByRect(&m_SkinCtlPnlDrag.RcDrag);
		}

		// init
		Tv_RcNew.right	=	0;

		// check rect able
		if((m_StX != A_X) && (m_StY != A_Y))
		{
			// calculate new rect
			if(m_StX < A_X)
			{
				Tv_RcNew.left		=	m_StX;
				Tv_RcNew.right	=	A_X + 1;
			}
			else
			{
				Tv_RcNew.left		=	A_X;
				Tv_RcNew.right	=	m_StX + 1;
			}

			if(m_StY < A_Y)
			{
				Tv_RcNew.top		=	m_StY;
				Tv_RcNew.bottom	=	A_Y + 1;
			}
			else
			{
				Tv_RcNew.top		=	A_Y;
				Tv_RcNew.bottom	=	m_StY + 1;
			}

			// patch
			if(Tv_RcNew.left < m_RcScr.left)
			{
				Tv_RcNew.left		=	m_RcScr.left;
			}
			if(Tv_RcNew.right > m_RcScr.right)
			{
				Tv_RcNew.right	=	m_RcScr.right;
			}
			if(Tv_RcNew.top < m_RcScr.top)
			{
				Tv_RcNew.top	=	m_RcScr.top;
			}
			if(Tv_RcNew.bottom > m_RcScr.bottom)
			{
				Tv_RcNew.bottom	=	m_RcScr.bottom;
			}

			if((Tv_RcNew.left == Tv_RcNew.right) || (Tv_RcNew.top == Tv_RcNew.bottom))
			{
				Tv_RcNew.right	=	0;
			}
		}

		// update
		m_SkinCtlPnlDrag.RcDrag	=	Tv_RcNew;
		if(0 != m_SkinCtlPnlDrag.RcDrag.right)
		{
			LcPntReqByRect(&m_SkinCtlPnlDrag.RcDrag);
		}


}
//--------------------------------------------------------------------

