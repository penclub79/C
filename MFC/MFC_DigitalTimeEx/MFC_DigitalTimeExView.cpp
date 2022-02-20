
// MFC_DigitalTimeExView.cpp : CMFC_DigitalTimeExView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC_DigitalTimeEx.h"
#endif

#include "MFC_DigitalTimeExDoc.h"
#include "MFC_DigitalTimeExView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_DigitalTimeExView

IMPLEMENT_DYNCREATE(CMFC_DigitalTimeExView, CView)

BEGIN_MESSAGE_MAP(CMFC_DigitalTimeExView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMFC_DigitalTimeExView 생성/소멸

CMFC_DigitalTimeExView::CMFC_DigitalTimeExView()
: m_bTimerRun(false)
, m_bTimerType(true)
, m_strTimer(_T(""))
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFC_DigitalTimeExView::~CMFC_DigitalTimeExView()
{
}

BOOL CMFC_DigitalTimeExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC_DigitalTimeExView 그리기

void CMFC_DigitalTimeExView::OnDraw(CDC* pDC)
{
	CMFC_DigitalTimeExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	CRect rect;
	GetClientRect(&rect);			// 윈도우 클라이언트 영역을 얻는다.
	// 윈도우의 중앙에 타이머를 출력
	pDC->DrawText(m_strTimer, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}


// CMFC_DigitalTimeExView 인쇄

BOOL CMFC_DigitalTimeExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC_DigitalTimeExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC_DigitalTimeExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFC_DigitalTimeExView 진단

#ifdef _DEBUG
void CMFC_DigitalTimeExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_DigitalTimeExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_DigitalTimeExDoc* CMFC_DigitalTimeExView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_DigitalTimeExDoc)));
	return (CMFC_DigitalTimeExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_DigitalTimeExView 메시지 처리기


int CMFC_DigitalTimeExView::OnCreate(LPCREATESTRUCT lpCreateStruct)		// 윈도우가 생성될 때 타이머를 설정한다.
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	SetTimer(0, 1000, NULL);		// 타이머 설정
	m_bTimerRun = true;				// 타이머 동작

	return 0;
}


void CMFC_DigitalTimeExView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int hour;
	CString str;
	CTime timer;
	timer = CTime::GetCurrentTime();

	if (m_bTimerType)
	{
		m_strTimer.Format(_T("현재는 %d년 %d월 %d일 %d시 %d분 %d초"),
			timer.GetYear(), timer.GetMonth(), timer.GetDay(),
			timer.GetHour(), timer.GetMinute(), timer.GetSecond());
	}
	else
	{
		hour = timer.GetHour();
		if (hour >= 12)
		{
			str = _T("PM");
			if (hour >= 13)
				hour = hour - 12;
		}
		else
		{
			str = _T("AM");
		}
		m_strTimer.Format(_T("지금은 %s %d시 %d분 %d초"), str, hour, timer.GetMinute(), timer.GetSecond());
	}
	Invalidate();
	CView::OnTimer(nIDEvent);
}


void CMFC_DigitalTimeExView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bTimerType)		// 년, 월, 일, 시, 분, 초 형태로 출력 조건
	{
		if (AfxMessageBox(_T("시, 분, 초 형태로 표시하시겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			m_bTimerType = false;
		}
	}
	else					// 시, 분, 초 형태로 출력 조건
	{
		if (AfxMessageBox(_T("년, 월, 일, 시, 분, 초 형태로 표시하시겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			m_bTimerType = true;
		}
	}
	CView::OnLButtonDown(nFlags, point);
}


void CMFC_DigitalTimeExView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bTimerRun == false)			// 타이머가 동작 안 할 때 메시지 박스 출력
	{
		if (AfxMessageBox(_T("디지털시계를 동작시키겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			SetTimer(0, 1000, NULL);	// 타이머 설정
			m_bTimerRun = true;			// 타이머 동작 => true
		}
	}
	else								// 타이머가 동작 중일 때 메시지 박스 출력
	{
		if (AfxMessageBox(_T("정말로 디지털시계 동작을 멈추시겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			KillTimer(0);				// 타이머 해제
			m_bTimerRun = false;		// 타이머 동작 => false
		}
	}


	CView::OnRButtonDown(nFlags, point);
}


void CMFC_DigitalTimeExView::OnDestroy()
{	
	if (m_bTimerRun)
	{
		KillTimer(0);	// 타이머 해제
	}
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
