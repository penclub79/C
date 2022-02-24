
// MFC11_GDIPlusExView.cpp : CMFC11_GDIPlusExView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC11_GDIPlusEx.h"
#endif

#include "MFC11_GDIPlusExDoc.h"
#include "MFC11_GDIPlusExView.h"

#include "PenSizeDlg.h"
#include "EraserSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC11_GDIPlusExView

IMPLEMENT_DYNCREATE(CMFC11_GDIPlusExView, CView)

BEGIN_MESSAGE_MAP(CMFC11_GDIPlusExView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC11_GDIPlusExView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PEN_SIZE, &CMFC11_GDIPlusExView::OnPenSize)
	ON_COMMAND(ID_ERASER_SIZE, &CMFC11_GDIPlusExView::OnEraserSize)
	ON_COMMAND(ID_PEN_COLOR, &CMFC11_GDIPlusExView::OnPenColor)
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFC11_GDIPlusExView 생성/소멸

CMFC11_GDIPlusExView::CMFC11_GDIPlusExView()
: m_nPenSize(0)
, m_nEraserSize(0)
, m_ptPrev(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_nPenSize = 2;
	m_nEraserSize = 4;
	m_colorPen = RGB(0, 0, 0);

}

CMFC11_GDIPlusExView::~CMFC11_GDIPlusExView()
{
}

BOOL CMFC11_GDIPlusExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC11_GDIPlusExView 그리기

void CMFC11_GDIPlusExView::OnDraw(CDC* /*pDC*/)
{
	CMFC11_GDIPlusExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMFC11_GDIPlusExView 인쇄


void CMFC11_GDIPlusExView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC11_GDIPlusExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC11_GDIPlusExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC11_GDIPlusExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFC11_GDIPlusExView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC11_GDIPlusExView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC11_GDIPlusExView 진단

#ifdef _DEBUG
void CMFC11_GDIPlusExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC11_GDIPlusExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC11_GDIPlusExDoc* CMFC11_GDIPlusExView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC11_GDIPlusExDoc)));
	return (CMFC11_GDIPlusExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC11_GDIPlusExView 메시지 처리기


void CMFC11_GDIPlusExView::OnPenSize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPenSizeDlg* pdlgPen = new CPenSizeDlg;
	if (pdlgPen->DoModal() == IDOK)
	{
		m_nPenSize = pdlgPen->m_nPenSize;
	}
}


void CMFC11_GDIPlusExView::OnEraserSize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CEraserSizeDlg* pdlgEraser = new CEraserSizeDlg;
	if (pdlgEraser->DoModal() == IDOK)
	{
		m_nEraserSize = pdlgEraser->m_nEraserSize;
	}
}


void CMFC11_GDIPlusExView::OnPenColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorDialog colorDlg;

	if (colorDlg.DoModal() == IDOK)
	{
		m_colorPen = colorDlg.GetColor();
	}
}


void CMFC11_GDIPlusExView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_ptPrev = point;
	HCURSOR hCursor = AfxGetApp()->LoadCursorW(IDC_CURSOR_ERASER);
	SetCursor(hCursor);

	CView::OnRButtonDown(nFlags, point);
}


void CMFC11_GDIPlusExView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);
	Graphics graphics(dc);

	Gdiplus::Color clr;			// GDI+ 로의 색상변경
	clr.SetFromCOLORREF(m_colorPen);

	if (nFlags == MK_LBUTTON)
	{
		HCURSOR hCursor = AfxGetApp()->LoadCursorW(IDC_CURSOR_PEN);
		SetCursor(hCursor);
		Pen pen(Color(clr), m_nPenSize);
		graphics.DrawLine(&pen, m_ptPrev.x, m_ptPrev.y, point.x, point.y);
		m_ptPrev = point;
	}
	if (nFlags == MK_RBUTTON)
	{
		HCURSOR hCursor = AfxGetApp()->LoadCursorW(IDC_CURSOR_ERASER);
		SetCursor(hCursor);
		Pen pen(Color(255, 255, 255), m_nEraserSize);
		graphics.DrawLine(&pen, m_ptPrev.x, m_ptPrev.y, point.x, point.y);
		m_ptPrev = point;
	}
	CView::OnMouseMove(nFlags, point);
}


void CMFC11_GDIPlusExView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_ptPrev = point;
	HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_PEN);
	SetCursor(hCursor);

	CView::OnLButtonDown(nFlags, point);
}
