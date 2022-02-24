
// MFC10_Interface_DockingPaneEx2View.h : CMFC10_Interface_DockingPaneEx2View Ŭ������ �������̽�
//

#pragma once


class CMFC10_Interface_DockingPaneEx2View : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC10_Interface_DockingPaneEx2View();
	DECLARE_DYNCREATE(CMFC10_Interface_DockingPaneEx2View)

// Ư���Դϴ�.
public:
	CMFC10_Interface_DockingPaneEx2Doc* GetDocument() const;

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
	virtual ~CMFC10_Interface_DockingPaneEx2View();
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
};

#ifndef _DEBUG  // MFC10_Interface_DockingPaneEx2View.cpp�� ����� ����
inline CMFC10_Interface_DockingPaneEx2Doc* CMFC10_Interface_DockingPaneEx2View::GetDocument() const
   { return reinterpret_cast<CMFC10_Interface_DockingPaneEx2Doc*>(m_pDocument); }
#endif

