
// MFC4_StopWatchExView.h : CMFC4_StopWatchExView Ŭ������ �������̽�
//

#pragma once


class CMFC4_StopWatchExView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC4_StopWatchExView();
	DECLARE_DYNCREATE(CMFC4_StopWatchExView)

// Ư���Դϴ�.
public:
	CMFC4_StopWatchExDoc* GetDocument() const;

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
	virtual ~CMFC4_StopWatchExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MFC4_StopWatchExView.cpp�� ����� ����
inline CMFC4_StopWatchExDoc* CMFC4_StopWatchExView::GetDocument() const
   { return reinterpret_cast<CMFC4_StopWatchExDoc*>(m_pDocument); }
#endif

