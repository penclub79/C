
// MFC6_DialogBox_DigitalTimeExDlg.h : ��� ����
//

#pragma once


// CMFC6_DialogBox_DigitalTimeExDlg ��ȭ ����
class CMFC6_DialogBox_DigitalTimeExDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC6_DialogBox_DigitalTimeExDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFC6_DIALOGBOX_DIGITALTIMEEX_DIALOG };

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
