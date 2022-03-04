
// MFC12_4_BitMapExView.h : CMFC12_4_BitMapExView 클래스의 인터페이스
//

#pragma once


class CMFC12_4_BitMapExView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFC12_4_BitMapExView();
	DECLARE_DYNCREATE(CMFC12_4_BitMapExView)

// 특성입니다.
public:
	CMFC12_4_BitMapExDoc* GetDocument() const;

// 작업입니다.
public:
	PBYTE m_apBuffer[2];		// 필요한 이미지 정보를 담을 버퍼
	PBYTE m_pBuffer;			// 그림1 그림2의 합친 결과를 담을 버퍼

	CRect m_rect;			// 사각형 좌표 기억하는 변수
	CPoint m_cpMouse;		// 이전 마우스 좌표 기억할 변수
	bool m_bClick;			// 클릭 여부
	int m_ibx;
	int m_iby;

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
	virtual ~CMFC12_4_BitMapExView();
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFC12_4_BitMapExView.cpp의 디버그 버전
inline CMFC12_4_BitMapExDoc* CMFC12_4_BitMapExView::GetDocument() const
   { return reinterpret_cast<CMFC12_4_BitMapExDoc*>(m_pDocument); }
#endif

