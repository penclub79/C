
// MFC21_Text_SearchExDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"

// CMFC21_Text_SearchExDlg 대화 상자
class CMFC21_Text_SearchExDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFC21_Text_SearchExDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MFC21_TEXT_SEARCHEX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	CEdit m_pFile;

	

	// 생성된 메시지 맵 함수
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
