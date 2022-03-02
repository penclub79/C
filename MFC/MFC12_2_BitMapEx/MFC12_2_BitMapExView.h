
// MFC12_2_BitMapExView.h : CMFC12_2_BitMapExView 클래스의 인터페이스
//

#pragma once


class CMFC12_2_BitMapExView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFC12_2_BitMapExView();
	DECLARE_DYNCREATE(CMFC12_2_BitMapExView)

// 특성입니다.
public:
	CMFC12_2_BitMapExDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFC12_2_BitMapExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnImageRoad();
	afx_msg void OnImageRoad2();
};

#ifndef _DEBUG  // MFC12_2_BitMapExView.cpp의 디버그 버전
inline CMFC12_2_BitMapExDoc* CMFC12_2_BitMapExView::GetDocument() const
   { return reinterpret_cast<CMFC12_2_BitMapExDoc*>(m_pDocument); }
#endif

