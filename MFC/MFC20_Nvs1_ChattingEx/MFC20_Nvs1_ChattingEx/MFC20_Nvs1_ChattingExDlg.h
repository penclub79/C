
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

private:
	CRect m_strInitLoc; // ���̾�α� ȭ�� server ip static ��ġ


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void InitServer();

public:
	afx_msg void OnClickedRadioServer();
	afx_msg void OnRadioClient();
	afx_msg void OnClickedButtonConnect();
	afx_msg void OnClickedButtonSend();
	void ReceiveMessage(CAcceptSock* pAccept, CString strReceive);
	void Accept(CString strSock);
	void ResizeControl(int cx, int cy);
	void EntryClient(CAcceptSock* pAccept);
	void onClose(CAcceptSock* pAccept);

	CListBox m_listChat;
	CIPAddressCtrl m_IPAddress;
	CString m_strMyIP;
	CString m_strOtherIP;
	
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnClickedButtonClose();
};
