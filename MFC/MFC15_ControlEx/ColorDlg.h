#pragma once
#include "afxcmn.h"


// CColorDlg 대화 상자입니다.

class CColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorDlg)

public:
	CColorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CColorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderRed;
	CSliderCtrl m_sliderGreen;
	CSliderCtrl m_sliderBlue;
	int m_nRed;
	int m_nGreen;
	int m_nBlue;
	COLORREF m_colorObject;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnSpinSize();
};
