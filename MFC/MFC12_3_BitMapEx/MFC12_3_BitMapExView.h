
// MFC12_3_BitMapExView.h : CMFC12_3_BitMapExView 클래스의 인터페이스
//

#pragma once


class CMFC12_3_BitMapExView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFC12_3_BitMapExView();
	DECLARE_DYNCREATE(CMFC12_3_BitMapExView)
	enum{	BUFFER_ID_BACKGROUND, 
			BUFFER_ID_INSERT, 
			BUFFER_ID_MERGE, 
			BUFFER_ID_COUNT};

// 특성입니다.
public:
	CMFC12_3_BitMapExDoc* GetDocument() const;

// 작업입니다.
public:
	PBYTE m_apBuffer[2];
	PBYTE m_pBuffer;
	int m_ibx = 0, m_iby = 0;	// 첫번째, 두번째 이미지 사이즈 변수
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
	virtual ~CMFC12_3_BitMapExView();
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

#ifndef _DEBUG  // MFC12_3_BitMapExView.cpp의 디버그 버전
inline CMFC12_3_BitMapExDoc* CMFC12_3_BitMapExView::GetDocument() const
   { return reinterpret_cast<CMFC12_3_BitMapExDoc*>(m_pDocument); }
#endif

