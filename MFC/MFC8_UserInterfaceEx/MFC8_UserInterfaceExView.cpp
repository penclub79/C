
// MFC8_UserInterfaceExView.cpp : CMFC8_UserInterfaceExView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC8_UserInterfaceEx.h"
#endif

#include "MFC8_UserInterfaceExDoc.h"
#include "MFC8_UserInterfaceExView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC8_UserInterfaceExView

IMPLEMENT_DYNCREATE(CMFC8_UserInterfaceExView, CView)

BEGIN_MESSAGE_MAP(CMFC8_UserInterfaceExView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC8_UserInterfaceExView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_LINE, &CMFC8_UserInterfaceExView::OnLine)
	ON_COMMAND(ID_ELLIPSE, &CMFC8_UserInterfaceExView::OnEllipse)
	ON_COMMAND(ID_POLYGON, &CMFC8_UserInterfaceExView::OnPolygon)
	ON_COMMAND(ID_LINE_COLOR, &CMFC8_UserInterfaceExView::OnLineColor)
	ON_COMMAND(ID_FACE_COLOR, &CMFC8_UserInterfaceExView::OnFaceColor)
	ON_COMMAND(ID_BDIAGONAL, &CMFC8_UserInterfaceExView::OnBdiagonal)
	ON_COMMAND(ID_CROSS, &CMFC8_UserInterfaceExView::OnCross)
	ON_COMMAND(ID_VERTICAL, &CMFC8_UserInterfaceExView::OnVertical)
	ON_UPDATE_COMMAND_UI(ID_LINE, &CMFC8_UserInterfaceExView::OnUpdateLine)
	ON_UPDATE_COMMAND_UI(ID_ELLIPSE, &CMFC8_UserInterfaceExView::OnUpdateEllipse)
	ON_UPDATE_COMMAND_UI(ID_POLYGON, &CMFC8_UserInterfaceExView::OnUpdatePolygon)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CMFC8_UserInterfaceExView 생성/소멸

CMFC8_UserInterfaceExView::CMFC8_UserInterfaceExView()
: m_nDrawMode(0)
, m_nHatchStyle(0)
, m_ptStart(0)
, m_ptPrev(0)
, m_bFirst(false)
, m_bLButtonDown(false)
, m_bContextMenu(false)
, m_bHatch(false)
//, m_ptData(0)
, m_nCount(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	
	m_nDrawMode = LINE_MODE;				// 그리기 모드 중에서 펜으로 그리기 초기화
	m_bContextMenu = true;					// 컨텍스트 메뉴를 활성으로 초기화
	m_bFirst = true;						// 처음 그리는 것으로 초기화
	m_bLButtonDown = false;					// 왼쪽 버튼이 눌리지 않은 것으로 초기화
	m_nCount = 0;							// 다각형 점을 저장할 배열의 카운트 초기화
	for (int i = 0; i < 50; i++)			// 다각형의 점을 저장할 배열 초기화
		m_ptData[i] = 0;
	m_bHatch = false;						// 해치 브러쉬를 십자가 빗금으로 초기화
	m_nHatchStyle = HS_CROSS;
	m_colorPen = RGB(0, 0, 0);				// pen color를 검은색으로 초기화
	m_colorBrush = RGB(255, 255, 255);		// brush color를 흰색으로 초기화
}

CMFC8_UserInterfaceExView::~CMFC8_UserInterfaceExView()
{
}

BOOL CMFC8_UserInterfaceExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC8_UserInterfaceExView 그리기

void CMFC8_UserInterfaceExView::OnDraw(CDC* pDC)
{
	CMFC8_UserInterfaceExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	CPen pen, *oldpen;
	pen.CreatePen(PS_SOLID, 1, m_colorPen);				// pen 객체 생성
	oldpen = pDC->SelectObject(&pen);					// pen 객체 등록
	pDC->SetROP2(R2_COPYPEN);							// COPYPEN으로 설정
	CBrush brush, *oldbrush;
	if (m_bHatch)
	{
		brush.CreateSolidBrush(m_colorBrush);			// Hatch brush 객체 생성
	}
	else
	{
		brush.CreateSolidBrush(m_colorBrush);			// Solid brush 객체 생성
	}
	oldbrush = pDC->SelectObject(&brush);				// brush 객체 등록

	switch (m_nDrawMode)
	{
	case LINE_MODE:										// 직선 그리기
		pDC->MoveTo(m_ptStart);
		pDC->LineTo(m_ptPrev);
		break;
	case ELLIPSE_MODE:									// 원 그리기
		pDC->Ellipse(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
		break;
	case POLYGON_MODE:									// 다각형 그리기
		// 배열에 저장한 순서대로 연결해 마지막 다각형 그리기
		pDC->Polygon(m_ptData, m_nCount);
		break;
	}
	pDC->SelectObject(oldpen);							// 이전 pen으로 설정
	pDC->SelectObject(oldbrush);						// 이전 brush으로 설정
	pen.DeleteObject();									// pen 객체 삭제
	brush.DeleteObject();								// brush 객체 삭제
}


// CMFC8_UserInterfaceExView 인쇄


void CMFC8_UserInterfaceExView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC8_UserInterfaceExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC8_UserInterfaceExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC8_UserInterfaceExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFC8_UserInterfaceExView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC8_UserInterfaceExView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	if (m_bContextMenu == true)
	{
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
	}
	m_bContextMenu = true;
#endif
}


// CMFC8_UserInterfaceExView 진단

#ifdef _DEBUG
void CMFC8_UserInterfaceExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC8_UserInterfaceExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC8_UserInterfaceExDoc* CMFC8_UserInterfaceExView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC8_UserInterfaceExDoc)));
	return (CMFC8_UserInterfaceExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC8_UserInterfaceExView 메시지 처리기


void CMFC8_UserInterfaceExView::OnLine()
{	
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 직선그리기 모드 변경
	m_nDrawMode = LINE_MODE;

	// 메인프레임의 포인터 얻음
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	// 상태바에 그리기 모드 출력
	pFrame->m_wndStatusBar.SetWindowText(_T("직선 그리기"));
}


void CMFC8_UserInterfaceExView::OnEllipse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 원 그리기 모드 변경
	m_nDrawMode = ELLIPSE_MODE;

	// 메인프레임의 포인터 얻음
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	// 상태바에 그리기 모드 출력
	pFrame->m_wndStatusBar.SetWindowText(_T("원 그리기"));
}


void CMFC8_UserInterfaceExView::OnPolygon()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 다각형 그리기 모드 변경
	m_nDrawMode = POLYGON_MODE;

	// 메인프레임의 포인터 얻음
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	// 상태바에 그리기 모드 출력
	pFrame->m_wndStatusBar.SetWindowText(_T("다각형 그리기"));
}


void CMFC8_UserInterfaceExView::OnLineColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorDialog dlgColor;						// 색상 대화상자의 인스턴스 생성
	if (dlgColor.DoModal() == IDOK)				// 색상을 선택하고 확인을 누르면
	{
		m_colorPen = dlgColor.GetColor();		// 선택한 색상 값을 얻어 colorPen에 설정
	}
	Invalidate(true);
	
}


void CMFC8_UserInterfaceExView::OnFaceColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CColorDialog dlgColor;
	if (dlgColor.DoModal() == IDOK)
	{
		m_colorBrush = dlgColor.GetColor();
	}
	m_bHatch = false;
	Invalidate(true);
}


void CMFC8_UserInterfaceExView::OnBdiagonal()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 왼쪽 45도 빗금 변경
	AfxMessageBox(_T("왼쪽 45도 패턴으로 변경합니다."));
	m_nHatchStyle = HS_BDIAGONAL;
	m_bHatch = true;
	Invalidate(true);
	
}


void CMFC8_UserInterfaceExView::OnCross()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 십자가 빗금 변경
	AfxMessageBox(_T("십자가 패턴으로 변경합니다."));
	m_nHatchStyle = HS_CROSS;
	m_bHatch = true;
	Invalidate(true);
}


void CMFC8_UserInterfaceExView::OnVertical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 수직 빗금 변경
	AfxMessageBox(_T("수직 패턴으로 변경합니다."));
	m_nHatchStyle = HS_VERTICAL;
	m_bHatch = true;
	Invalidate(true);
}


void CMFC8_UserInterfaceExView::OnUpdateLine(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	
	// 직선 그리기 모드이면 메뉴에 체크 표시
	pCmdUI->SetCheck(m_nDrawMode == LINE_MODE ? 1 : 0);
}


void CMFC8_UserInterfaceExView::OnUpdateEllipse(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

	// 원 그리기 모드이면 메뉴에 체크 표시
	pCmdUI->SetCheck(m_nDrawMode == ELLIPSE_MODE ? 1 : 0);
}



void CMFC8_UserInterfaceExView::OnUpdatePolygon(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

	// 다각형 그리기 모드이면 메뉴에 체크 표시
	pCmdUI->SetCheck(m_nDrawMode == POLYGON_MODE ? 1 : 0);
}


void CMFC8_UserInterfaceExView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 메인프레임의 포인터 얻음
	CClientDC dc(this);
	CPen pen, *oldpen;
	pen.CreatePen(PS_SOLID, 1, m_colorPen);
	oldpen = dc.SelectObject(&pen);

	dc.SetROP2(R2_NOTXORPEN);

	CBrush brush, *oldbrush;
	if (m_bHatch)
	{
		brush.CreateHatchBrush(m_nHatchStyle, m_colorBrush);
	}
	else
	{
		brush.CreateSolidBrush(m_colorBrush);
	}

	oldbrush = dc.SelectObject(&brush);

	switch (m_nDrawMode)
	{
		case LINE_MODE:
			if (m_bLButtonDown)
			{
				dc.MoveTo(m_ptStart);
				dc.LineTo(m_ptPrev);
				dc.MoveTo(m_ptStart);
				dc.LineTo(point);
				m_ptPrev = point;
			}
			break;

		case ELLIPSE_MODE:
			if (m_bLButtonDown)
			{
				dc.Ellipse(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
				dc.Ellipse(m_ptStart.x, m_ptStart.y, point.x, point.y);
				m_ptPrev = point;
			}
			break;

		case POLYGON_MODE:
			if (!m_bFirst)
			{
				dc.MoveTo(m_ptStart);
				dc.LineTo(m_ptPrev);
				dc.MoveTo(m_ptStart);
				dc.LineTo(point);
				m_ptPrev = point;
			}
			break;
	}
	dc.SelectObject(oldpen);
	dc.SelectObject(oldbrush);
	pen.DeleteObject();
	brush.DeleteObject();

	// 메인프레임의 포인터 얻음
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CString strPoint;
	strPoint.Format(_T("마우스 위치 x: %d, y: %d"), point.x, point.y);

	// 새로 추가한 팬에 마우스 위치 출력
	pFrame->m_wndStatusBar.SetPaneText(1, strPoint);


	CView::OnMouseMove(nFlags, point);
}


void CMFC8_UserInterfaceExView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bFirst)
	{
		m_ptStart = m_ptPrev = 0;
		m_bLButtonDown = false;
		m_bFirst = true;
		m_nCount = 0;
		for (int i = 0; i < 50; i++)
			m_ptData[i] = 0;
		Invalidate(true);
	}

	switch (m_nDrawMode)
	{
		case LINE_MODE :							// 직선 그리기
		case ELLIPSE_MODE :							// 원 그리기
			m_bLButtonDown = true;					// 왼쪽 버튼이 눌림
			m_ptStart = m_ptPrev = point;			// 시작점과 이전 점에 현재 점을 저장
			m_bFirst = false;						// 처음 그리는 것 -> false로 전환
			break;
		case POLYGON_MODE :							// 다각형 그리기
			if (m_bFirst == true)					
				m_bFirst = false;					// 처음 그리는 것 -> false로 전환
			m_ptStart = m_ptPrev = point;			// 시작점과 이전 점에 현재 점을 저장
			m_ptData[m_nCount] = point;				// 현재 점을 저장
			m_nCount++;								// 카운트 증가
			break;
	}

	RECT rectClient;								// 구조체 변수 선언
	SetCapture();									// 마우스 캡처
	GetClientRect(&rectClient);						// 클라이언트 영역 받음
	ClientToScreen(&rectClient);					// 스크린 좌표계 변환
	::ClipCursor(&rectClient);						// 마우스 이동범위를 클라이언트 영역으로 제한
	CView::OnLButtonDown(nFlags, point);
}


void CMFC8_UserInterfaceExView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bLButtonDown)
	{
		// 그리기 모드가 직선 그리기, 원 그리기 이면
		if (m_nDrawMode == LINE_MODE || m_nDrawMode == ELLIPSE_MODE)
		{
			m_bLButtonDown = false;
			m_bFirst = true;
			ReleaseCapture();			// 마우스 캡처 해제
			::ClipCursor(NULL);			// 마우스 클릭 해제
			Invalidate(true);			// 화면 갱신
		}
	}

	CView::OnLButtonUp(nFlags, point);
}


void CMFC8_UserInterfaceExView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 다각형 그리기
	if (m_nDrawMode == POLYGON_MODE)
	{
		if (!m_bFirst)			// 처음 그리는 것이 아니면
		{
			m_bContextMenu = FALSE;			// 컨텍스트 메뉴 비활성

			// 배열에 현재 점을 저장하고 카운트 증가
			m_ptData[m_nCount] = point;
			m_nCount++;

			// 체크변수 초기화 => 다시 다각형을 그리기 위해
			m_bFirst = TRUE;
			ReleaseCapture();
			::ClipCursor(NULL);
			Invalidate(TRUE);
		}
	}

	CView::OnRButtonDown(nFlags, point);
}
