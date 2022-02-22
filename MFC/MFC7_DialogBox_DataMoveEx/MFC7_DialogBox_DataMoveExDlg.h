
// MFC7_DialogBox_DataMoveExDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CMFC7_DialogBox_DataMoveExDlg 대화 상자
class CMFC7_DialogBox_DataMoveExDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFC7_DialogBox_DataMoveExDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MFC7_DIALOGBOX_DATAMOVEEX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
