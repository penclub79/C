
// MFC_Message_KeyAndMouseEx_3View.h : CMFC_Message_KeyAndMouseEx_3View 클래스의 인터페이스
//

#pragma once
#include "atltypes.h"


class CMFC_Message_KeyAndMouseEx_3View : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFC_Message_KeyAndMouseEx_3View();
	DECLARE_DYNCREATE(CMFC_Message_KeyAndMouseEx_3View)

// 특성입니다.
public:
	CMFC_Message_KeyAndMouseEx_3Doc* GetDocument() const;

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
	virtual ~CMFC_Message_KeyAndMouseEx_3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	int test;
	CPoint m_ptLocation;
	CString m_strOutput;
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CPoint m_ptClientSize;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFC_Message_KeyAndMouseEx_3View.cpp의 디버그 버전
inline CMFC_Message_KeyAndMouseEx_3Doc* CMFC_Message_KeyAndMouseEx_3View::GetDocument() const
   { return reinterpret_cast<CMFC_Message_KeyAndMouseEx_3Doc*>(m_pDocument); }
#endif

