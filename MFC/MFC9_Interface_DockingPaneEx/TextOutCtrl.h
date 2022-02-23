#pragma once


// CTextOutCtrl 대화 상자입니다.

class CTextOutCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CTextOutCtrl)

public:
	CTextOutCtrl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTextOutCtrl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TEXTOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	afx_msg void OnBnClickedButtonTextout();
	afx_msg void OnClickedButtonTextout();
	afx_msg void OnClickedButtonColor();
	afx_msg void OnClickedButtonFont();
	afx_msg void OnEnChangeEditY();
};
