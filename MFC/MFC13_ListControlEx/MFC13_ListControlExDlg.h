
// MFC13_ListControlExDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"


// CMFC13_ListControlExDlg ��ȭ ����
class CMFC13_ListControlExDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC13_ListControlExDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFC13_LISTCONTROLEX_DIALOG };

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
	CListCtrl m_listStudent;
	CString m_strDept;
	CString m_strID;
	CString m_strName;
	CString m_strSeletedItem;
	afx_msg void OnCbnSelchangeComboStyle();
	afx_msg void OnClickedButtonInsert();
	int m_nSelectedItem;
	afx_msg void OnItemchangedListStudent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedButtonModify();
	afx_msg void OnClickedButtonDelete();
	afx_msg void OnClickedButtonReset();
};
