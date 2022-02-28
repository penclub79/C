
// MFC12_BitMapExView.h : CMFC12_BitMapExView Ŭ������ �������̽�
//

#pragma once


class CMFC12_BitMapExView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC12_BitMapExView();
	DECLARE_DYNCREATE(CMFC12_BitMapExView)

// Ư���Դϴ�.
public:
	CMFC12_BitMapExDoc* GetDocument() const;

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
	virtual ~CMFC12_BitMapExView();
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
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnImageRoad();
};

#ifndef _DEBUG  // MFC12_BitMapExView.cpp�� ����� ����
inline CMFC12_BitMapExDoc* CMFC12_BitMapExView::GetDocument() const
   { return reinterpret_cast<CMFC12_BitMapExDoc*>(m_pDocument); }
#endif

