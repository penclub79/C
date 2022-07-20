#pragma once
#include "resource.h"
#include "afxwin.h"


// CLoginDlg 대화 상자입니다.

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLoginDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LOGIN };

private:
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUsername;
	CString m_strPassword;

	CEdit m_EditUserName;
	CEdit m_EditPassword;
	afx_msg void OnClickedIdok();
};
