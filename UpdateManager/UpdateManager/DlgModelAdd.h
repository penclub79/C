#pragma once
#include <GrTypeBase.h>
#include "afxwin.h"


class DlgModelAdd : public CDialogEx
{
	DECLARE_DYNAMIC(DlgModelAdd)

public:
	DlgModelAdd(CWnd* pParent = NULL);
	virtual ~DlgModelAdd();
	
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_DIALOG_MODEL_ADD };
	afx_msg void OnSelchangeComboModeltype();
	int		GetModelType();
	BOOL	m_bModalResult;
	int		m_iModelType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   // DDX / DDV Support
	DECLARE_MESSAGE_MAP()

private:
	
	CComboBox m_CModelType;

	void SetModelName(TCHAR* _pszName);

	
public:
	afx_msg void OnClickedButtonModelMake();
	afx_msg void OnClickedButtonModelCancel();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

