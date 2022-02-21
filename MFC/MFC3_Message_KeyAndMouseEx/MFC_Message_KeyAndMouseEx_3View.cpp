
// MFC_Message_KeyAndMouseEx_3View.cpp : CMFC_Message_KeyAndMouseEx_3View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC_Message_KeyAndMouseEx_3.h"
#endif

#include "MFC_Message_KeyAndMouseEx_3Doc.h"
#include "MFC_Message_KeyAndMouseEx_3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_Message_KeyAndMouseEx_3View

IMPLEMENT_DYNCREATE(CMFC_Message_KeyAndMouseEx_3View, CView)

BEGIN_MESSAGE_MAP(CMFC_Message_KeyAndMouseEx_3View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CMFC_Message_KeyAndMouseEx_3View 생성/소멸

CMFC_Message_KeyAndMouseEx_3View::CMFC_Message_KeyAndMouseEx_3View()
: m_strOutput(_T(""))
, m_ptLocation(0)
, m_ptClientSize(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFC_Message_KeyAndMouseEx_3View::~CMFC_Message_KeyAndMouseEx_3View()
{
}

BOOL CMFC_Message_KeyAndMouseEx_3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC_Message_KeyAndMouseEx_3View 그리기

void CMFC_Message_KeyAndMouseEx_3View::OnDraw(CDC* pDC)
{
	CMFC_Message_KeyAndMouseEx_3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// 윈도우에 문자열 출력
	pDC->TextOutW(m_ptLocation.x, m_ptLocation.y, m_strOutput);
}


// CMFC_Message_KeyAndMouseEx_3View 인쇄

BOOL CMFC_Message_KeyAndMouseEx_3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC_Message_KeyAndMouseEx_3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC_Message_KeyAndMouseEx_3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFC_Message_KeyAndMouseEx_3View 진단

#ifdef _DEBUG
void CMFC_Message_KeyAndMouseEx_3View::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_Message_KeyAndMouseEx_3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_Message_KeyAndMouseEx_3Doc* CMFC_Message_KeyAndMouseEx_3View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_Message_KeyAndMouseEx_3Doc)));
	return (CMFC_Message_KeyAndMouseEx_3Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_Message_KeyAndMouseEx_3View 메시지 처리기


void CMFC_Message_KeyAndMouseEx_3View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int nCharIndex;								
	nCharIndex = m_strOutput.GetLength();		// 입력된 데이터의 길이를 얻는다.

	if (nChar == VK_BACK)						// 백스페이스가 눌린 경우.
		m_strOutput.Delete(nCharIndex - 1, 1);	// 한 번에 하나씩 지운다.
	else										// 백스페이스 이외의 키가 눌린 경우
		m_strOutput += (WCHAR)nChar;			// 키보드로 입력된 문자를 문자열에 추가

	Invalidate();								// 화면 갱신

	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CMFC_Message_KeyAndMouseEx_3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nChar)								// 가상키 코드 값에 대해
	{
	case VK_LEFT:								// 왼쪽 화살표 키를 누를 때
		m_ptLocation.x--;						// 왼쪽으로 1 이동
		break;
	case VK_RIGHT:								// 오른쪽 화살표 키를 누를 때
		m_ptLocation.x++;						// 오른쪽으로 1 이동
		break;
	case VK_UP:									// 위쪽 화살표 키를 누를 때
		m_ptLocation.y--;						// 위쪽으로 1 이동
		break;
	case VK_DOWN:								// 아래쪽 화살표 키를 누를 때
		m_ptLocation.y++;						// 아래쪽으로 1 이동
		break;
	case VK_PRIOR:								// PageUp키를 누를 때
		m_ptLocation.y -= 50;					// 위쪽으로 50 이동
		break;
	case VK_NEXT:								// PageDown키를 누를 때
		m_ptLocation.y += 50;					// 아래쪽으로 50 이동
		break;
	case VK_HOME:								// Home키를 누를 때
		m_ptLocation = CPoint(0, 0);			// 처음위치로 이동
		break;
	}

	if (m_ptLocation.x < 0)						// X좌표가 0보다 작으면
	{
		m_ptLocation.x = 0;						// m_ptLocation.x = 0으로 초기화
		AfxMessageBox(_T("왼쪽으로 더 이상 이동할 수 없습니다."));
	}
	if (m_ptLocation.y < 0)						// Y좌표가 0보다 작으면
	{
		m_ptLocation.y = 0;						// m_ptLocation.y = 0으로 초기화
		AfxMessageBox(_T("위쪽으로 더 이상 이동할 수 없습니다."));
	}
	if (m_ptLocation.x > m_ptClientSize.x)		// x좌표가 윈도우 사이즈 X 값보다 크면
	{
		m_ptLocation.x = m_ptClientSize.x;		// m_ptLocation.x = 윈도우 X크기로 초기화
		AfxMessageBox(_T("오른쪽으로 더 이상 이동할 수 없습니다."));
	}
	if (m_ptLocation.y > m_ptClientSize.y)		// y좌표가 윈도우 사이즈 Y 값보다 크면
	{
		m_ptLocation.y = m_ptClientSize.y;		// m_ptLocation.y = 윈도우 Y크기로 초기화
		AfxMessageBox(_T("아래쪽으로 더 이상 이동할 수 없습니다."));
	}
	Invalidate();								// 화면 갱신
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMFC_Message_KeyAndMouseEx_3View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_ptClientSize.x = cx;		// 윈도우 영역의 가로(x) 길이를 얻음
	m_ptClientSize.y = cy;		// 윈도우 영역의 세로(y) 길이를 얻음

	Invalidate();				// 화면 갱신
}


void CMFC_Message_KeyAndMouseEx_3View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_ptLocation.x = point.x;
	m_ptLocation.y = point.y;
	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}


void CMFC_Message_KeyAndMouseEx_3View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_strOutput.IsEmpty() == false)
	{
		if (AfxMessageBox(_T("문자를 모두 지우시겠습니까?"),
			MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			m_strOutput.Empty();
		}
	}
	else
	{
		AfxMessageBox(_T("문자열을 입력하시오"));
	}
	Invalidate();
	CView::OnRButtonDown(nFlags, point);
}
