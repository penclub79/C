
// MFC8_UserInterfaceExView.h : CMFC8_UserInterfaceExView 클래스의 인터페이스
//
#include "atltypes.h"
enum DRAW_MODE { LINE_MODE, ELLIPSE_MODE, POLYGON_MODE };
#pragma once


class CMFC8_UserInterfaceExView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFC8_UserInterfaceExView();
	DECLARE_DYNCREATE(CMFC8_UserInterfaceExView)

// 특성입니다.
public:
	CMFC8_UserInterfaceExDoc* GetDocument() const;

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
	virtual ~CMFC8_UserInterfaceExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLine();
	afx_msg void OnEllipse();
	afx_msg void OnPolygon();
	afx_msg void OnLineColor();
	afx_msg void OnFaceColor();
	afx_msg void OnBdiagonal();
	afx_msg void OnCross();
	afx_msg void OnVertical();
	int m_nDrawMode;
	int m_nHatchStyle;
	afx_msg void OnUpdateLine(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEllipse(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePolygon(CCmdUI *pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
CPoint m_ptStart;
};

#ifndef _DEBUG  // MFC8_UserInterfaceExView.cpp의 디버그 버전
inline CMFC8_UserInterfaceExDoc* CMFC8_UserInterfaceExView::GetDocument() const
   { return reinterpret_cast<CMFC8_UserInterfaceExDoc*>(m_pDocument); }
#endif

