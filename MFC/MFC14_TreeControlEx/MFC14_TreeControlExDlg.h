
// MFC14_TreeControlExDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"


// CMFC14_TreeControlExDlg ��ȭ ����
class CMFC14_TreeControlExDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC14_TreeControlExDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFC14_TREECONTROLEX_DIALOG };

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
	CTreeCtrl m_treeControl;
	CString m_strNodeText;
	CString m_strSelectedNode;
	HTREEITEM m_hRoot;
	HTREEITEM m_hSelectedNode;
	bool m_bChecked;
	afx_msg void OnSelchangedTreeControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedButtonInsert();
	afx_msg void OnClickedButtonModify();
	afx_msg void OnClickedButtonDelete();
};
