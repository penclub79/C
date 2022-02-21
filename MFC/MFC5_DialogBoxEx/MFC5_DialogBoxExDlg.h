
// MFC5_DialogBoxExDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CMFC5_DialogBoxExDlg 대화 상자
class CMFC5_DialogBoxExDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFC5_DialogBoxExDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MFC5_DIALOGBOXEX_DIALOG };

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
	CString m_strEdit;
	CListBox m_listbox;
	CComboBox m_cbListItem;
	void UpdateComboBox();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	bool m_bChecked[2];
	afx_msg void OnClickedCheck1();
	afx_msg void OnClickedCheck2();
	afx_msg void OnClickedButtonAdd();
	afx_msg void OnClickedButtonInsert();
	afx_msg void OnClickedButtonDelete();
};
