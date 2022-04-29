/*
Pane class select

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrMfcDockToolbar
#define	_EPC_GrMfcDockToolbar

//====================================================================
// uses

#include <afxtoolbar.h>
#include <GrLnxApp.h>
#include <GrTypeBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//global class

class Cls_GrMfcDockToolbar : public CMFCToolBar
{
	private:
		BOOL	m_IsShowOnList;
		BOOL8	m_IsCmdUiToOwner;
		BOOL8	m_IsCanUseState;

	public:
		Cls_GrMfcDockToolbar(void);
		virtual	~Cls_GrMfcDockToolbar(void);

		virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
		virtual	BOOL	AllowShowOnList(void) const;

		void	ShowOnListSet(BOOL A_IsOn);
		void	CommandUiSetMode(BOOL8 A_IsOwner);
		void	StateUseSet(BOOL8 A_IsUse);

		int	ButtonIndexGetById(UINT A_Id);
		CMFCToolBarButton*	ButtonGetById(UINT A_Id);
		void	ButtonSetText(UINT A_Id,WCHAR* A_Str,BOOL8 A_IsImageView);
		void	ButtonSetEnable(UINT A_Id, BOOL8 A_IsEnable);
		void	ButtonSetCheckbox(UINT A_Id, BOOL8 A_IsUse);
		void	ButtonSetChecked(UINT A_Id, BOOL8 A_IsChkecked);
		BOOL8	IsButtonChecked(UINT A_Id);
		void	ButtonHideAfterIndex(int A_Index);
		void	ButtonImageSet(UINT A_Id, int A_ImgIdx);

		void	EditSetup(UINT A_Id, int A_Width, BOOL A_IsFlat = TRUE, UINT A_Style = TBBS_PRESSED);
		void	EditSetText(UINT A_Id, WCHAR* A_Str);
		void	EditGetText(UINT A_Id, WCHAR* A_Str, int A_MaxLen);

		void	ComboboxSetup(UINT A_Id, int A_Width, BOOL A_IsFlat = TRUE, DWORD A_Style = CBS_DROPDOWNLIST, int A_MaxHeight = 600);
		void	ComboboxItemRemoveAll(UINT A_Id);
		void	ComboboxItemAdd(UINT A_Id, WCHAR* A_StrTxt, __u32 A_UserData);
		__u32	ComboboxItemGetSelected(UINT A_Id);
		void	ComboboxItemSelect(UINT A_Id, __u32 A_UserData);

		// inherited
		virtual BOOL LoadState( LPCTSTR lpszProfileName=NULL, int nIndex=-1,UINT uiID=(UINT)-1);
		virtual BOOL SaveState( LPCTSTR lpszProfileName=NULL, int nIndex=-1, UINT uiID=(UINT)-1 );


};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

