
// MFC19_SocketExDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "BasicSock.h"

// CMFC19_SocketExDlg ��ȭ ����
class CMFC19_SocketExDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC19_SocketExDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CHATTING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

private:
	CRect m_strInitLoc; // ���̾�α� ȭ�� server ip static ��ġ
	CRect m_rectDlg; // ���̾�α� ũ��

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
	afx_msg void OnClickedButtonConnect();
	afx_msg void OnClickedButtonSend();
	void ReceiveMessage(CString strReceive, CString strOtherUserID);
	void ResizeControl(int cx, int cy);
	void SetConnectStatus(int iErrorCode);

	CBasicSock* m_pClient;

	CIPAddressCtrl m_IPAddress;
	CListBox m_listChat;
	int m_nChatMode;
	CString m_strMyIP;
	CString m_nOtherIP;
	CString m_strOtherIP;
	afx_msg void OnBnClickedRadioServer();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//afx_msg void OnClickedButtonClose();
	//afx_msg void OnClickedButtonDisconnect();
};
