/*
MFC docking toolbar

*/

//--------------------------------------------------------------------
// uses

#include <Win\MfcDock\GrMfcDockToolbar.h>

#include <GrDebug.h>

#include <afxtoolbareditboxbutton.h>
#include <afxtoolbarcomboboxbutton.h>

/*
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5\GrP5ConBase.h>
*/


//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var


//====================================================================
//--------------------------------------------------------------------
Cls_GrMfcDockToolbar::Cls_GrMfcDockToolbar() :
CMFCToolBar()
{
		m_IsShowOnList		=	TRUE;
		m_IsCmdUiToOwner	=	FALSE;

		m_IsCanUseState		=	TRUE;
}
//--------------------------------------------------------------------
Cls_GrMfcDockToolbar::~Cls_GrMfcDockToolbar()
{

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	// local -------------------
	// code --------------------
		if (m_IsCmdUiToOwner)
		{
			//CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
			CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), FALSE);
		}
		else
		{
			//CMFCToolBar::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);
			CMFCToolBar::OnUpdateCmdUI(pTarget, FALSE);
		}
}
//--------------------------------------------------------------------
BOOL	Cls_GrMfcDockToolbar::AllowShowOnList(void) const
{
		return	m_IsShowOnList;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::ShowOnListSet(BOOL A_IsOn)
{
		m_IsShowOnList	=	A_IsOn;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::CommandUiSetMode(BOOL8 A_IsOwner)
{
	// local -------------------
	// code --------------------
		m_IsCmdUiToOwner	=	A_IsOwner;
}
//--------------------------------------------------------------------
int	Cls_GrMfcDockToolbar::ButtonIndexGetById(UINT A_Id)
{
#if 0
	// local -------------------
		int	Tv_Result;
		int	Tv_Cnt;
		int	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		Tv_Cnt	=	(__u32)GetCount();
		if (0 != Tv_Cnt)
		{
			for (Tv_WkIdx=0; Tv_WkIdx < Tv_Cnt; Tv_WkIdx++)
			{
				if (A_Id == GetItemID(Tv_WkIdx))
				{
					Tv_Result	=	Tv_WkIdx;
					break;
				}
			}
		}

		return	Tv_Result;
#else
		return	CommandToIndex(A_Id);
#endif
}
//--------------------------------------------------------------------
CMFCToolBarButton*	Cls_GrMfcDockToolbar::ButtonGetById(UINT A_Id)
{
	// local -------------------
		CMFCToolBarButton*	Tv_Result;
		__u32	Tv_Index;
	// code --------------------
		Tv_Result	=	NULL;

		Tv_Index	=	ButtonIndexGetById(A_Id);
		if (0 <= Tv_Index)
		{
			Tv_Result	=	GetButton(Tv_Index);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::ButtonSetText(UINT A_Id, WCHAR* A_Str, BOOL8 A_IsImageView)
{
	// local -------------------
		int	Tv_BtnIdx;
	// code --------------------
		Tv_BtnIdx	=	ButtonIndexGetById(A_Id);
		if (0 <= Tv_BtnIdx)
		{
			SetToolBarBtnText(Tv_BtnIdx, A_Str, TRUE, (BOOL)A_IsImageView);
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::EditSetup(UINT A_Id, int A_Width, BOOL A_IsFlat, UINT A_Style)
{
	// local -------------------
		
	// code --------------------

		CMFCToolBarEditBoxButton	Tv_ObjEdt(A_Id, GetCmdMgr()->GetCmdImage(A_Id, FALSE), ES_AUTOHSCROLL, A_Width);
		Tv_ObjEdt.CanBeStretched();
		Tv_ObjEdt.HaveHotBorder();
		Tv_ObjEdt.SetFlatMode(A_IsFlat);
		Tv_ObjEdt.SetStyle(A_Style);
		ReplaceButton(A_Id, Tv_ObjEdt);

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::EditSetText(UINT A_Id, WCHAR* A_Str)
{
	// local -------------------
		CMFCToolBarEditBoxButton*	Tv_ObjEdt;
	// code --------------------
		Tv_ObjEdt	=	(CMFCToolBarEditBoxButton*)ButtonGetById(A_Id);
		if(NULL != Tv_ObjEdt)
		{
			Tv_ObjEdt->SetContents(A_Str);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::EditGetText(UINT A_Id, WCHAR* A_Str, int A_MaxLen)
{
	// local -------------------
		CMFCToolBarEditBoxButton*	Tv_ObjEdt;
		CEdit*	Tv_Edt;
	// code --------------------
		Tv_ObjEdt	=	(CMFCToolBarEditBoxButton*)ButtonGetById(A_Id);
		if(NULL != Tv_ObjEdt)
		{
			Tv_Edt	=	Tv_ObjEdt->GetEditBox();
			if(NULL != Tv_Edt)
			{
				Tv_Edt->GetWindowText(A_Str, A_MaxLen);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::ButtonSetEnable(UINT A_Id, BOOL8 A_IsEnable)
{
	// local -------------------
		int	Tv_BtnIdx;
		UINT	Tv_Style;
	// code --------------------
		Tv_BtnIdx	=	ButtonIndexGetById(A_Id);
		if(0 <= Tv_BtnIdx)
		{
			Tv_Style	=	GetButtonStyle(Tv_BtnIdx);
			if(A_IsEnable)
			{
				Tv_Style	=	Tv_Style & (~TBBS_DISABLED);
			}
			else
			{
				Tv_Style	=	Tv_Style | TBBS_DISABLED;
			}
			SetButtonStyle(Tv_BtnIdx, Tv_Style);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::ButtonSetCheckbox(UINT A_Id, BOOL8 A_IsUse)
{
	// local -------------------
		int	Tv_BtnIdx;
	// code --------------------
		Tv_BtnIdx	=	ButtonIndexGetById(A_Id);
		if (0 <= Tv_BtnIdx)
		{
			if (A_IsUse)
			{
				SetButtonStyle(Tv_BtnIdx, TBBS_CHECKBOX);
			}
			else
			{
				SetButtonStyle(Tv_BtnIdx, TBBS_BUTTON);
			}

			InvalidateButton(Tv_BtnIdx);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::ButtonSetChecked(UINT A_Id, BOOL8 A_IsChkecked)
{
	// local -------------------
		int	Tv_BtnIdx;
		UINT	Tv_Style;
	// code --------------------
		Tv_BtnIdx	=	ButtonIndexGetById(A_Id);
		if(0 <= Tv_BtnIdx)
		{
			Tv_Style	=	GetButtonStyle(Tv_BtnIdx);
			if(A_IsChkecked)
			{
				Tv_Style	=	Tv_Style | TBBS_CHECKED;
			}
			else
			{
				Tv_Style	=	Tv_Style & (~TBBS_CHECKED);
			}

			SetButtonStyle(Tv_BtnIdx, Tv_Style);

			InvalidateButton(Tv_BtnIdx);
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockToolbar::IsButtonChecked(UINT A_Id)
{
	// local -------------------
		BOOL8	Tv_Result;
		int		Tv_BtnIdx;
		UINT	Tv_Style;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_BtnIdx	=	ButtonIndexGetById(A_Id);
		if(0 <= Tv_BtnIdx)
		{
			Tv_Style	=	GetButtonStyle(Tv_BtnIdx);
			if(0 != (Tv_Style & TBBS_CHECKED))
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::ButtonHideAfterIndex(int A_Index)
{
	// local -------------------
		int	Tv_Index;
		int	Tv_Count;
		CMFCToolBarButton*	Tv_Btn;
	// code --------------------
		Tv_Index	=	A_Index;
		Tv_Count	=	GetCount();
		if((0 <= Tv_Index) && (Tv_Count > Tv_Index))
		{
			while(Tv_Index < Tv_Count)
			{
				Tv_Btn	=	GetButton(Tv_Index);
				if(NULL != Tv_Btn)
				{
					Tv_Btn->SetVisible(FALSE);
				}
				// next
				Tv_Index ++;
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::ButtonImageSet(UINT A_Id, int A_ImgIdx)
{
	// local -------------------
		CMFCToolBarButton*	Tv_Btn;
	// code --------------------
		Tv_Btn	=	ButtonGetById(A_Id);
		if(NULL != Tv_Btn)
		{
			Tv_Btn->SetImage(A_ImgIdx);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::ComboboxSetup(UINT A_Id, int A_Width, BOOL A_IsFlat, DWORD A_Style, int A_MaxHeight)
{
		CMFCToolBarComboBoxButton	Tv_ObjCbx(A_Id, GetCmdMgr()->GetCmdImage(A_Id, FALSE), A_Style, A_Width);
		Tv_ObjCbx.EnableWindow(TRUE);
		Tv_ObjCbx.SetDropDownHeight(A_MaxHeight);
		Tv_ObjCbx.SetCenterVert();
		if(A_IsFlat)
		{
			Tv_ObjCbx.SetFlatMode();
		}
		if(0 == ReplaceButton(A_Id, Tv_ObjCbx))
		{
			DbgMsgPrint("Cls_GrMfcDockToolbar::ComboboxSetup - button (%d) not found.\n",A_Id);
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::ComboboxItemRemoveAll(UINT A_Id)
{
	// local -------------------
		CMFCToolBarComboBoxButton*	Tv_Cbx;
	// code --------------------
		Tv_Cbx	=	(CMFCToolBarComboBoxButton*)ButtonGetById(A_Id);
		if(NULL != Tv_Cbx)
		{
			Tv_Cbx->RemoveAllItems();
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::ComboboxItemAdd(UINT A_Id, WCHAR* A_StrTxt, __u32 A_UserData)
{
	// local -------------------
		CMFCToolBarComboBoxButton*	Tv_Cbx;
	// code --------------------
		Tv_Cbx	=	(CMFCToolBarComboBoxButton*)ButtonGetById(A_Id);
		if(NULL != Tv_Cbx)
		{
			Tv_Cbx->AddItem(A_StrTxt, (DWORD_PTR)A_UserData);
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrMfcDockToolbar::ComboboxItemGetSelected(UINT A_Id)
{
	// local -------------------
		__u32	Tv_Result;
		int		Tv_CurSel;
		CMFCToolBarComboBoxButton*	Tv_Cbx;
	// code --------------------
		Tv_Result	=	0;

		Tv_Cbx	=	(CMFCToolBarComboBoxButton*)ButtonGetById(A_Id);
		if(NULL != Tv_Cbx)
		{
			Tv_CurSel	=	Tv_Cbx->GetCurSel();
			if(0 <= Tv_CurSel)
			{
				Tv_Result	=	(__u32)Tv_Cbx->GetItemData(Tv_CurSel);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::ComboboxItemSelect(UINT A_Id, __u32 A_UserData)
{
	// local -------------------
		CMFCToolBarComboBoxButton*	Tv_Cbx;
	// code --------------------
		Tv_Cbx	=	(CMFCToolBarComboBoxButton*)ButtonGetById(A_Id);
		if(NULL != Tv_Cbx)
		{
			Tv_Cbx->SelectItem((DWORD_PTR)A_UserData);
		}
}
//--------------------------------------------------------------------
BOOL Cls_GrMfcDockToolbar::LoadState(LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{
	if (m_IsCanUseState)
	{
		return	CMFCToolBar::LoadState(lpszProfileName, nIndex, uiID);
	}
	else
	{
		return	FALSE;
	}
}
//--------------------------------------------------------------------
BOOL Cls_GrMfcDockToolbar::SaveState(LPCTSTR lpszProfileName, int nIndex, UINT uiID)
{
	if (m_IsCanUseState)
	{
		return	CMFCToolBar::SaveState(lpszProfileName, nIndex, uiID);
	}
	else
	{
		return	FALSE;
	}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockToolbar::StateUseSet(BOOL8 A_IsUse)
{
	m_IsCanUseState	=	A_IsUse;
}
//--------------------------------------------------------------------
