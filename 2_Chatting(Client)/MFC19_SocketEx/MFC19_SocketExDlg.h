
// MFC19_SocketExDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"
#include "BasicSock.h"

// CMFC19_SocketExDlg 대화 상자
class CMFC19_SocketExDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFC19_SocketExDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	~CMFC19_SocketExDlg();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_CHATTING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

private:
	CRect m_strInitLoc; // 다이얼로그 화면 server ip static 위치
	CRect m_rectDlg; // 다이얼로그 크기
	int m_iConnResult;

// 구현입니다.
protected:
	HICON m_hIcon;
	int m_iPort;
	
	// 생성된 메시지 맵 함수
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
	void ConnectStatus(int _iResult);
	void SetUserID(CString _strUserID);
	CString GetUserID();

	CBasicSock* m_pClient;

	CIPAddressCtrl m_ctlIPAddress;
	CListBox m_ctlListChat;
	int m_nChatMode;
	CString m_strMyIP;
	CString m_strUserID;
	
	afx_msg void OnBnClickedRadioServer();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

	afx_msg void OnClickedButtonClose();
	afx_msg void OnClickedButtonDisconnect();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
