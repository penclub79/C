
// MFC10_Interface_DockingPaneEx2View.cpp : CMFC10_Interface_DockingPaneEx2View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC10_Interface_DockingPaneEx2.h"
#endif

#include "MFC10_Interface_DockingPaneEx2Doc.h"
#include "MFC10_Interface_DockingPaneEx2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC10_Interface_DockingPaneEx2View

IMPLEMENT_DYNCREATE(CMFC10_Interface_DockingPaneEx2View, CView)

BEGIN_MESSAGE_MAP(CMFC10_Interface_DockingPaneEx2View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC10_Interface_DockingPaneEx2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFC10_Interface_DockingPaneEx2View 생성/소멸

CMFC10_Interface_DockingPaneEx2View::CMFC10_Interface_DockingPaneEx2View()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFC10_Interface_DockingPaneEx2View::~CMFC10_Interface_DockingPaneEx2View()
{
}

BOOL CMFC10_Interface_DockingPaneEx2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC10_Interface_DockingPaneEx2View 그리기

void CMFC10_Interface_DockingPaneEx2View::OnDraw(CDC* /*pDC*/)
{
	CMFC10_Interface_DockingPaneEx2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMFC10_Interface_DockingPaneEx2View 인쇄


void CMFC10_Interface_DockingPaneEx2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC10_Interface_DockingPaneEx2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC10_Interface_DockingPaneEx2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC10_Interface_DockingPaneEx2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFC10_Interface_DockingPaneEx2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC10_Interface_DockingPaneEx2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC10_Interface_DockingPaneEx2View 진단

#ifdef _DEBUG
void CMFC10_Interface_DockingPaneEx2View::AssertValid() const
{
	CView::AssertValid();
}

void CMFC10_Interface_DockingPaneEx2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC10_Interface_DockingPaneEx2Doc* CMFC10_Interface_DockingPaneEx2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC10_Interface_DockingPaneEx2Doc)));
	return (CMFC10_Interface_DockingPaneEx2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC10_Interface_DockingPaneEx2View 메시지 처리기
