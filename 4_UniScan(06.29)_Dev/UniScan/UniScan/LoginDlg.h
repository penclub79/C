#pragma once
#include "resource.h"
#include "afxwin.h"


// CLoginDlg ��ȭ �����Դϴ�.

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLoginDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_LOGIN };

private:
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUsername;
	CString m_strPassword;

	CEdit m_EditUserName;
	CEdit m_EditPassword;
	afx_msg void OnClickedIdok();
};
