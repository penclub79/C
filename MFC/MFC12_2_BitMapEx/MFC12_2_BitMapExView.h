
// MFC12_2_BitMapExView.h : CMFC12_2_BitMapExView Ŭ������ �������̽�
//

#pragma once


class CMFC12_2_BitMapExView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC12_2_BitMapExView();
	DECLARE_DYNCREATE(CMFC12_2_BitMapExView)

// Ư���Դϴ�.
public:
	CMFC12_2_BitMapExDoc* GetDocument() const;

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
	virtual ~CMFC12_2_BitMapExView();
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
	afx_msg void OnImageRoad2();
};

#ifndef _DEBUG  // MFC12_2_BitMapExView.cpp�� ����� ����
inline CMFC12_2_BitMapExDoc* CMFC12_2_BitMapExView::GetDocument() const
   { return reinterpret_cast<CMFC12_2_BitMapExDoc*>(m_pDocument); }
#endif

