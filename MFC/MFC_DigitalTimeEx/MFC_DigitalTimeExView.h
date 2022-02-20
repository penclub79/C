
// MFC_DigitalTimeExView.h : CMFC_DigitalTimeExView Ŭ������ �������̽�
//

#pragma once


class CMFC_DigitalTimeExView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC_DigitalTimeExView();
	DECLARE_DYNCREATE(CMFC_DigitalTimeExView)

// Ư���Դϴ�.
public:
	CMFC_DigitalTimeExDoc* GetDocument() const;

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
	virtual ~CMFC_DigitalTimeExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	bool m_bTimerRun;	// true : "��, ��, ��, ��, ��, ��" | false : "��, ��, ��"

public:
	bool m_bTimerType;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CString m_strTimer;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // MFC_DigitalTimeExView.cpp�� ����� ����
inline CMFC_DigitalTimeExDoc* CMFC_DigitalTimeExView::GetDocument() const
   { return reinterpret_cast<CMFC_DigitalTimeExDoc*>(m_pDocument); }
#endif

