
// MFC17_Calendar_DLLExDlg.h : ��� ����
//

#pragma once


// CMFC17_Calendar_DLLExDlg ��ȭ ����
class CMFC17_Calendar_DLLExDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC17_Calendar_DLLExDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFC17_CALENDAR_DLLEX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
