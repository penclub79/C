
// MFC21_Text_SearchExDlg.h : ��� ����
//

#pragma once


// CMFC21_Text_SearchExDlg ��ȭ ����
class CMFC21_Text_SearchExDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC21_Text_SearchExDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFC21_TEXT_SEARCHEX_DIALOG };

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
