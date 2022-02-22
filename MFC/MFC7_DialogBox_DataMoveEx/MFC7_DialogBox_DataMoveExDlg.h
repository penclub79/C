
// MFC7_DialogBox_DataMoveExDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CMFC7_DialogBox_DataMoveExDlg ��ȭ ����
class CMFC7_DialogBox_DataMoveExDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC7_DialogBox_DataMoveExDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFC7_DIALOGBOX_DATAMOVEEX_DIALOG };

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
public:
	afx_msg void OnBnClickedButtonPurchase();
	CListBox m_listLeft;
	CListBox m_listRight;
	CComboBox m_cbMove;
	CString m_strResult;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnLbnSelchangeListLeft();
	afx_msg void OnEnChangeEditResult();
	afx_msg void OnSelchangeComboMove();
};
