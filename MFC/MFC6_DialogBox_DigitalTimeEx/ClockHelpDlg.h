#pragma once


// CClockHelpDlg ��ȭ �����Դϴ�.

class CClockHelpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CClockHelpDlg)

public:
	CClockHelpDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CClockHelpDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
