
// MFC8_UserInterfaceExView.h : CMFC8_UserInterfaceExView Ŭ������ �������̽�
//

#pragma once


class CMFC8_UserInterfaceExView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC8_UserInterfaceExView();
	DECLARE_DYNCREATE(CMFC8_UserInterfaceExView)

// Ư���Դϴ�.
public:
	CMFC8_UserInterfaceExDoc* GetDocument() const;

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
	virtual ~CMFC8_UserInterfaceExView();
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
	afx_msg void OnLine();
	afx_msg void OnEllipse();
	afx_msg void OnPolygon();
	afx_msg void OnLineColor();
	afx_msg void OnFaceColor();
	afx_msg void OnBdiagonal();
	afx_msg void OnCross();
	afx_msg void OnVertical();
};

#ifndef _DEBUG  // MFC8_UserInterfaceExView.cpp�� ����� ����
inline CMFC8_UserInterfaceExDoc* CMFC8_UserInterfaceExView::GetDocument() const
   { return reinterpret_cast<CMFC8_UserInterfaceExDoc*>(m_pDocument); }
#endif

