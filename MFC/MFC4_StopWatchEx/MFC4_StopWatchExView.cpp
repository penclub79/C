
// MFC4_StopWatchExView.cpp : CMFC4_StopWatchExView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC4_StopWatchEx.h"
#endif

#include "MFC4_StopWatchExDoc.h"
#include "MFC4_StopWatchExView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC4_StopWatchExView

IMPLEMENT_DYNCREATE(CMFC4_StopWatchExView, CView)

BEGIN_MESSAGE_MAP(CMFC4_StopWatchExView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFC4_StopWatchExView 생성/소멸

CMFC4_StopWatchExView::CMFC4_StopWatchExView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFC4_StopWatchExView::~CMFC4_StopWatchExView()
{
}

BOOL CMFC4_StopWatchExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC4_StopWatchExView 그리기

void CMFC4_StopWatchExView::OnDraw(CDC* /*pDC*/)
{
	CMFC4_StopWatchExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMFC4_StopWatchExView 인쇄

BOOL CMFC4_StopWatchExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC4_StopWatchExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC4_StopWatchExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFC4_StopWatchExView 진단

#ifdef _DEBUG
void CMFC4_StopWatchExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC4_StopWatchExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC4_StopWatchExDoc* CMFC4_StopWatchExView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC4_StopWatchExDoc)));
	return (CMFC4_StopWatchExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC4_StopWatchExView 메시지 처리기
