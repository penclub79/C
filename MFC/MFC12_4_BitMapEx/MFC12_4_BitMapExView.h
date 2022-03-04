
// MFC12_4_BitMapExView.h : CMFC12_4_BitMapExView Ŭ������ �������̽�
//

#pragma once


class CMFC12_4_BitMapExView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC12_4_BitMapExView();
	DECLARE_DYNCREATE(CMFC12_4_BitMapExView)

// Ư���Դϴ�.
public:
	CMFC12_4_BitMapExDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	PBYTE m_apBuffer[2];		// �ʿ��� �̹��� ������ ���� ����
	PBYTE m_pBuffer;			// �׸�1 �׸�2�� ��ģ ����� ���� ����

	CRect m_rect;			// �簢�� ��ǥ ����ϴ� ����
	CPoint m_cpMouse;		// ���� ���콺 ��ǥ ����� ����
	bool m_bClick;			// Ŭ�� ����
	int m_ibx;
	int m_iby;

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
	virtual ~CMFC12_4_BitMapExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnImageRoad();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFC12_4_BitMapExView.cpp�� ����� ����
inline CMFC12_4_BitMapExDoc* CMFC12_4_BitMapExView::GetDocument() const
   { return reinterpret_cast<CMFC12_4_BitMapExDoc*>(m_pDocument); }
#endif

