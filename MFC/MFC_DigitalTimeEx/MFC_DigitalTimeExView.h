
// MFC_DigitalTimeExView.h : CMFC_DigitalTimeExView 클래스의 인터페이스
//

#pragma once


class CMFC_DigitalTimeExView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFC_DigitalTimeExView();
	DECLARE_DYNCREATE(CMFC_DigitalTimeExView)

// 특성입니다.
public:
	CMFC_DigitalTimeExDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFC_DigitalTimeExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	bool m_bTimerRun;	// true : "년, 월, 일, 시, 분, 초" | false : "시, 분, 초"

public:
	bool m_bTimerType;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CString m_strTimer;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // MFC_DigitalTimeExView.cpp의 디버그 버전
inline CMFC_DigitalTimeExDoc* CMFC_DigitalTimeExView::GetDocument() const
   { return reinterpret_cast<CMFC_DigitalTimeExDoc*>(m_pDocument); }
#endif

