
// MFC12_3_BitMapExView.h : CMFC12_3_BitMapExView Ŭ������ �������̽�
//

#pragma once


class CMFC12_3_BitMapExView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC12_3_BitMapExView();
	DECLARE_DYNCREATE(CMFC12_3_BitMapExView)
	enum{	BUFFER_ID_BACKGROUND, 
			BUFFER_ID_INSERT, 
			BUFFER_ID_MERGE, 
			BUFFER_ID_COUNT};

// Ư���Դϴ�.
public:
	CMFC12_3_BitMapExDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	PBYTE m_apBuffer[2];
	PBYTE m_pBuffer;
	int m_ibx = 0, m_iby = 0;	// ù��°, �ι�° �̹��� ������ ����
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
	virtual ~CMFC12_3_BitMapExView();
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

#ifndef _DEBUG  // MFC12_3_BitMapExView.cpp�� ����� ����
inline CMFC12_3_BitMapExDoc* CMFC12_3_BitMapExView::GetDocument() const
   { return reinterpret_cast<CMFC12_3_BitMapExDoc*>(m_pDocument); }
#endif

