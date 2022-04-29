#pragma once
#include <GrTypeBase.h>
#include "afxwin.h"

// DlgUpdtAdd dialog

class DlgUpdtAdd : public CDialogEx
{
	DECLARE_DYNAMIC(DlgUpdtAdd)

private:
	__s32		m_AddType;

public:
	DlgUpdtAdd(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgUpdtAdd();

// Dialog Data
	enum { IDD = IDD_DLG_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CbType;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	__u32		GetAddType();
};
