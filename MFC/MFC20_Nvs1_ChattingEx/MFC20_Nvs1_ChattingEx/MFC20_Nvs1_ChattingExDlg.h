
// MFC20_Nvs1_ChattingExDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMFC20_Nvs1_ChattingExDlg ��ȭ ����
class CMFC20_Nvs1_ChattingExDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC20_Nvs1_ChattingExDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFC20_NVS1_CHATTINGEX_DIALOG };

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
	CListBox m_listChat;
	CIPAddressCtrl m_IPAddress;
};
