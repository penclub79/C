/*
Platform 5 UI window popup

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5WinPopMenu.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5CtrlEvtBase.h>

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
Cls_GrP5WinPopMenu::Cls_GrP5WinPopMenu(__u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect, __u16 A_CtrlId, __u16 A_Squad) :
Cls_GrP5WinPopItemBase(A_ConIdx, A_LayerIdx, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdWinPopMenu, NULL, 0, &m_SkinCtlWinPopMenu);

		m_CtrlId		=	A_CtrlId;

		m_Squad			=	A_Squad;

		GrDumyZeroMem(&m_SkinCtlWinPopMenu, sizeof(m_SkinCtlWinPopMenu));

		// get popup depth
		m_PopDepth		=	V_GrP5Env.PopCtl.WinCnt;

		// regist to popup control
		V_GrP5Env.PopCtl.WinCtl[V_GrP5Env.PopCtl.WinCnt].WinObjId	=	m_ObjId;
		V_GrP5Env.PopCtl.WinCnt ++;

		// build menu
		LcBuildMenu();

}
//--------------------------------------------------------------------
Cls_GrP5WinPopMenu::~Cls_GrP5WinPopMenu(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
void	Cls_GrP5WinPopMenu::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5WinPopItemBase::EvtScptDecDone();

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinPopMenu::DvlParaInfoBuild(void)
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
void	Cls_GrP5WinPopMenu::DvlParaInfoAction(__u32 A_Action)
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
void	Cls_GrP5WinPopMenu::DvlPreDelete(void)
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
void	Cls_GrP5WinPopMenu::EvtWinKeyFocusLoss(void)
{
	// local -------------------
	// code --------------------


}
//--------------------------------------------------------------------
void	Cls_GrP5WinPopMenu::LcBuildMenu(void)
{
	// local -------------------
		__u32	Tv_TblIdx;
	// code --------------------
		m_SkinCtlWinPopMenu.ItemCnt		=	0;

		if(0 == V_GrP5Env.ItemCnt)
		{
			return;
		}

		for( Tv_TblIdx = 0; Tv_TblIdx < V_GrP5Env.ItemCnt; Tv_TblIdx++)
		{
			if(m_Squad == V_GrP5Env.ItemTbl[Tv_TblIdx].Squad)
			{
				m_SkinCtlWinPopMenu.Items[m_SkinCtlWinPopMenu.ItemCnt]	=	&V_GrP5Env.ItemTbl[Tv_TblIdx];
				m_SkinCtlWinPopMenu.ItemCnt ++;
				if(E_GrP5SkinWinPopMenuItemMaxCnt <= m_SkinCtlWinPopMenu.ItemCnt)
				{
					break;
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5WinPopMenu::LcItemSelect(__u32 A_ItemIdx)
{
	// local -------------------
		__u32	Tv_ItmIdx;
		BOOL8	Tv_IsDraw;
		St_GrRect	Tv_Rc;
	// code --------------------
		if(0 == m_SkinCtlWinPopMenu.ItemCnt)
		{
			return;
		}

		for(Tv_ItmIdx = 0; Tv_ItmIdx < m_SkinCtlWinPopMenu.ItemCnt; Tv_ItmIdx++)
		{
			Tv_IsDraw		=	FALSE;
			if(Tv_ItmIdx == A_ItemIdx)
			{
				if(0 == (E_GrP5StatOnSelected & m_SkinCtlWinPopMenu.Items[Tv_ItmIdx]->Stat))
				{
					Tv_IsDraw	=	TRUE;
				}
				m_SkinCtlWinPopMenu.Items[Tv_ItmIdx]->Stat	=	m_SkinCtlWinPopMenu.Items[Tv_ItmIdx]->Stat | E_GrP5StatOnSelected;
			}
			else
			{
				if(0 != (E_GrP5StatOnSelected & m_SkinCtlWinPopMenu.Items[Tv_ItmIdx]->Stat))
				{
					Tv_IsDraw	=	TRUE;
				}
				m_SkinCtlWinPopMenu.Items[Tv_ItmIdx]->Stat	=	m_SkinCtlWinPopMenu.Items[Tv_ItmIdx]->Stat & (~E_GrP5StatOnSelected);
			}

			// draw
			if(Tv_IsDraw && (NULL != m_SkinMng))
			{
				m_SkinMng->CtlGetItemOneRect(this, 0, Tv_ItmIdx, &Tv_Rc);
				if(0 < Tv_Rc.right)
				{
					LcPntReqByRect(&Tv_Rc);
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5WinPopMenu::EvtClickItem(__u32 A_ItmIdx)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		// get skin manager
		if(NULL == m_SkinMng)
		{
			return;
		}

		// close sub depth
		GrP5PopupCloseByDepth(m_PopDepth + 1);

		// check exist item
		if(A_ItmIdx >= m_SkinCtlWinPopMenu.ItemCnt)
		{
			return;
		}

		// mark select
		LcItemSelect(A_ItmIdx);

		// check exist sub squad
		if(E_GrP5ItemSquadIdNone != m_SkinCtlWinPopMenu.Items[A_ItmIdx]->ChildSquad)
		{
			// open child popup
			// get item rect
			m_SkinMng->CtlGetItemOneRect(this, 0, (__s32)A_ItmIdx, &Tv_Rc);
			if(0 > Tv_Rc.right)
			{
				return;
			}

			Tv_Rc.left	=	m_RcScr.left;
			Tv_Rc.right	=	m_RcScr.right;

			GrP5MenuSubPop(&Tv_Rc, m_ConIdx, m_Layer, m_SkinCtlWinPopMenu.Items[A_ItmIdx]->ChildSquad, m_CtrlId);

		}
		else
		{
			// do event
			if(LcDoCtlEvtItemClick(A_ItmIdx))
			{
				// close popup
				GrP5PopupCloseAll();
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinPopMenu::LcDoCtlEvtItemClick(__u32 A_ItemIdx)
{
	// local -------------------
		Cls_GrP5CtrlEvtBase*	Tv_Ctrl;
		__u32	Tv_ItemId;
	// code --------------------
		Tv_Ctrl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_CtrlId);
		if(NULL == Tv_Ctrl)
		{
			return	TRUE;
		}
		if(A_ItemIdx >= m_SkinCtlWinPopMenu.ItemCnt)
		{
			DbgMsgPrint("Cls_GrP5WinPopMenu::LcDoCtlEvtItemClick - [BUG] bad item index (i:%d c:%d) input!\n", A_ItemIdx, m_SkinCtlWinPopMenu.ItemCnt);
			return	TRUE;
		}

		Tv_ItemId	=	((Def_GrCalPtr)m_SkinCtlWinPopMenu.Items[A_ItemIdx] - (Def_GrCalPtr)V_GrP5Env.ItemTbl) / sizeof(St_GrP5Item);
		return	Tv_Ctrl->EvtMenuClick(m_CtrlId, m_ObjId, Tv_ItemId, m_Squad, m_SkinCtlWinPopMenu.Items[A_ItemIdx]->UserData);

}
//--------------------------------------------------------------------
