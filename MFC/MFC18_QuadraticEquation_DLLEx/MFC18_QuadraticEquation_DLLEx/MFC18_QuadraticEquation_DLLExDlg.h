
// MFC18_QuadraticEquation_DLLExDlg.h : ��� ����
//

#pragma once
#include "D:\C\MFC\MFC18_QuadraticEquation_DLLEx\RootDll\CalculateRoot.h"


// CMFC18_QuadraticEquation_DLLExDlg ��ȭ ����
class CMFC18_QuadraticEquation_DLLExDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC18_QuadraticEquation_DLLExDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFC18_QUADRATICEQUATION_DLLEX_DIALOG };

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
	CCalculateRoot m_calcRoot;
	afx_msg void OnClickedButtonCalcRoot();
	afx_msg void OnClickedButtonClear();
};
