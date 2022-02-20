
// MFC_Message_KeyAndMouseEx_3View.h : CMFC_Message_KeyAndMouseEx_3View Ŭ������ �������̽�
//

#pragma once
#include "atltypes.h"


class CMFC_Message_KeyAndMouseEx_3View : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC_Message_KeyAndMouseEx_3View();
	DECLARE_DYNCREATE(CMFC_Message_KeyAndMouseEx_3View)

// Ư���Դϴ�.
public:
	CMFC_Message_KeyAndMouseEx_3Doc* GetDocument() const;

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
	virtual ~CMFC_Message_KeyAndMouseEx_3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
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

#ifndef _DEBUG  // MFC_Message_KeyAndMouseEx_3View.cpp�� ����� ����
inline CMFC_Message_KeyAndMouseEx_3Doc* CMFC_Message_KeyAndMouseEx_3View::GetDocument() const
   { return reinterpret_cast<CMFC_Message_KeyAndMouseEx_3Doc*>(m_pDocument); }
#endif

