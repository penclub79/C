
// MFC11_GDIPlusExView.h : CMFC11_GDIPlusExView 클래스의 인터페이스
//

#pragma once
#include "atltypes.h"


class CMFC11_GDIPlusExView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFC11_GDIPlusExView();
	DECLARE_DYNCREATE(CMFC11_GDIPlusExView)

// 특성입니다.
public:
	CMFC11_GDIPlusExDoc* GetDocument() const;

// 작업입니다.
public:

	void DrawImageFromFile(CDC* pDC);


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
	virtual ~CMFC11_GDIPlusExView();
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
	int m_nPenSize;
	int m_nEraserSize;
	HANDLE m_hFile;
	LPBYTE m_pByte;
	BITMAPFILEHEADER m_bmFileHeader;
	LPBITMAPINFO m_lpBitmapInfo;

	COLORREF m_colorPen;
	CPoint m_ptPrev;
	afx_msg void OnPenSize();
	afx_msg void OnEraserSize();
	afx_msg void OnPenColor();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnImgFileAppend();
};

#ifndef _DEBUG  // MFC11_GDIPlusExView.cpp의 디버그 버전
inline CMFC11_GDIPlusExDoc* CMFC11_GDIPlusExView::GetDocument() const
   { return reinterpret_cast<CMFC11_GDIPlusExDoc*>(m_pDocument); }
#endif

