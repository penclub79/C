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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   // DDX / DDV Support
	DECLARE_MESSAGE_MAP()

private:
	int m_iModelType;
	CComboBox m_CModelType;

	void SetModelName(TCHAR* _pszName);

	
public:
	afx_msg void OnClickedButtonModelMake();
	afx_msg void OnClickedButtonModelCancel();

};

