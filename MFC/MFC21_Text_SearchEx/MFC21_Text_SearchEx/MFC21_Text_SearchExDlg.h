
// MFC21_Text_SearchExDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"

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
	CEdit m_pFile;

	

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//char*	m_pszBuffer;
	//char*	ConvertUnicodeToMultybyte(CString strUnicode);


	afx_msg void OnClickedButtonImport();
};
