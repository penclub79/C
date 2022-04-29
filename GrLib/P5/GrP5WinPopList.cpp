/*
Platform 5 UI window popup

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5WinPopList.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5Key.h>
//#include <P5/GrP5UoLi

#ifdef GR_P5_EDITOR

#include <P5/Dvl/GrP5DvlBase.h>

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


#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5WinPopList::Cls_GrP5WinPopList(__u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect, __u16 A_HostObjId, __u32 A_ViewItemIdx) :
	Cls_GrP5WinPopItemBase(A_ConIdx, A_LayerIdx, A_PtrRelRect)
{
	// local -------------------
		//St_GrRect	Tv_RcCon;
		//St_GrRect	Tv_RcWin;
	__u32	Tv_SclPos;
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdWinPopList, NULL, 0, NULL);

		// init
		m_ParaWinBase.IsInputLayerLock	=	FALSE;	// input layer lock

		m_HostObjId	=	A_HostObjId;

		// regist to popup control
		V_GrP5Env.PopCtl.WinCtl[V_GrP5Env.PopCtl.WinCnt].WinObjId	=	m_ObjId;
		V_GrP5Env.PopCtl.WinCnt ++;

		// update control
		LcSkinCtlUpdate();
		
		// default scroll position set
		if(NULL != m_SkinMng)
		{
			Tv_SclPos	=	A_ViewItemIdx;
			if((Tv_SclPos < V_GrP5Env.ItemCnt) && (0 != Tv_SclPos))
			{
				if((Tv_SclPos + m_SkinCtlWinPopItemBase.ViewCnt) > V_GrP5Env.ItemCnt)
				{
					if(V_GrP5Env.ItemCnt <= m_SkinCtlWinPopItemBase.ViewCnt)
					{
						Tv_SclPos		=	0;
					}
					else
					{
						Tv_SclPos		=	V_GrP5Env.ItemCnt - m_SkinCtlWinPopItemBase.ViewCnt;
					}
				}
			}

			// update scroll bar value
			if(0 < m_SkinCtlWinPopItemBase.Sbar.RcArea.right)
			{
				m_SkinMng->CtlCalcSbarVtByValue(this, &m_SkinCtlWinPopItemBase.Sbar, Tv_SclPos);
			}
		}

}
//--------------------------------------------------------------------
Cls_GrP5WinPopList::~Cls_GrP5WinPopList(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5WinPopList::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5WinPopItemBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinPopList::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5WinPopItemBase::DvlParaInfoBuild();
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinPopList::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

	// inherited
	Cls_GrP5WinPopItemBase::DvlParaInfoAction(A_Action);

	// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinPopList::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// not delete

		// delete prepare

		// inherited
		//Cls_GrP5WinPopItemBase::DvlPreDelete();

}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinPopList::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5UoBase*	Tv_EvtHost;
	// code --------------------
		Tv_Result	=	FALSE;

		if(E_GrP5KeyEsc == A_Key)
		{
			if(E_GrP5ObjIdNone != m_HostObjId)
			{
				Tv_EvtHost	=	(Cls_GrP5UoBase*)GrP5UiObjGet(m_HostObjId);
				if(NULL != Tv_EvtHost)
				{
					Tv_EvtHost->EvtPopSelCancel();
				}
			}
			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_Result	=	Cls_GrP5WinPopItemBase::EvtKeyUp(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5WinPopList::EvtClickItem(__u32 A_ItmIdx)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Uo;
	// code --------------------
		// do selected
		Tv_Uo	=	(Cls_GrP5UoBase*)GrP5UiObjGet(m_HostObjId);
		if(NULL == Tv_Uo)
		{
			GrP5PopupCloseAll();
			return;
		}

		// call event
		Tv_Uo->EvtListItemSel(m_HostObjId, A_ItmIdx);

		GrP5PopupCloseAll();
}
//--------------------------------------------------------------------
