
// MFC9_Interface_DockingPaneExView.h : CMFC9_Interface_DockingPaneExView Ŭ������ �������̽�
//

#pragma once
#include "atltypes.h"


class CMFC9_Interface_DockingPaneExView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC9_Interface_DockingPaneExView();
	DECLARE_DYNCREATE(CMFC9_Interface_DockingPaneExView)

// Ư���Դϴ�.
public:
	CMFC9_Interface_DockingPaneExDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMFC9_Interface_DockingPaneExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CString m_strOutput;
	int m_nTextWidth;
	int m_nTextHeight;
	CPoint m_ptPosition;
	COLORREF m_colorText;
	bool m_bFont;
	LOGFONT m_logFont;
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // MFC9_Interface_DockingPaneExView.cpp�� ����� ����
inline CMFC9_Interface_DockingPaneExDoc* CMFC9_Interface_DockingPaneExView::GetDocument() const
   { return reinterpret_cast<CMFC9_Interface_DockingPaneExDoc*>(m_pDocument); }
#endif

