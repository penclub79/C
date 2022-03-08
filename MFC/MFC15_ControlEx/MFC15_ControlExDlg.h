
// MFC15_ControlExDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "ObjectDlg.h"
#include "ColorDlg.h"


// CMFC15_ControlExDlg ��ȭ ����
class CMFC15_ControlExDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC15_ControlExDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFC15_CONTROLEX_DIALOG };

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
	CTabCtrl m_tabSelection;
	int m_nSize;
	CSpinButtonCtrl m_spinSize;
	CObjectDlg m_dlgObject;
	CColorDlg m_dlgColor;
	void UpdateDrawing();
	afx_msg void OnSelchangeTabSelection(NMHDR *pNMHDR, LRESULT *pResult);
};
