
// MFC12_2_BitMapExView.cpp : CMFC12_2_BitMapExView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC12_2_BitMapEx.h"
#endif

#include "MFC12_2_BitMapExDoc.h"
#include "MFC12_2_BitMapExView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC12_2_BitMapExView

IMPLEMENT_DYNCREATE(CMFC12_2_BitMapExView, CView)

BEGIN_MESSAGE_MAP(CMFC12_2_BitMapExView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_IMAGE_ROAD, &CMFC12_2_BitMapExView::OnImageRoad)
END_MESSAGE_MAP()

// CMFC12_2_BitMapExView 생성/소멸

CMFC12_2_BitMapExView::CMFC12_2_BitMapExView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFC12_2_BitMapExView::~CMFC12_2_BitMapExView()
{
}

BOOL CMFC12_2_BitMapExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC12_2_BitMapExView 그리기

void CMFC12_2_BitMapExView::OnDraw(CDC* /*pDC*/)
{
	CMFC12_2_BitMapExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMFC12_2_BitMapExView 인쇄

BOOL CMFC12_2_BitMapExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC12_2_BitMapExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC12_2_BitMapExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFC12_2_BitMapExView 진단

#ifdef _DEBUG
void CMFC12_2_BitMapExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC12_2_BitMapExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC12_2_BitMapExDoc* CMFC12_2_BitMapExView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC12_2_BitMapExDoc)));
	return (CMFC12_2_BitMapExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC12_2_BitMapExView 메시지 처리기


void CMFC12_2_BitMapExView::OnImageRoad()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxMessageBox(_T("불러왔습니다."));
}
