
// MFC_DigitalTimeExView.cpp : CMFC_DigitalTimeExView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMFC_DigitalTimeExView ����/�Ҹ�

CMFC_DigitalTimeExView::CMFC_DigitalTimeExView()
: m_bTimerRun(false)
, m_bTimerType(true)
, m_strTimer(_T(""))
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFC_DigitalTimeExView::~CMFC_DigitalTimeExView()
{
}

BOOL CMFC_DigitalTimeExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC_DigitalTimeExView �׸���

void CMFC_DigitalTimeExView::OnDraw(CDC* pDC)
{
	CMFC_DigitalTimeExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	CRect rect;
	GetClientRect(&rect);			// ������ Ŭ���̾�Ʈ ������ ��´�.
	// �������� �߾ӿ� Ÿ�̸Ӹ� ���
	pDC->DrawText(m_strTimer, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}


// CMFC_DigitalTimeExView �μ�

BOOL CMFC_DigitalTimeExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC_DigitalTimeExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC_DigitalTimeExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFC_DigitalTimeExView ����

#ifdef _DEBUG
void CMFC_DigitalTimeExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_DigitalTimeExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_DigitalTimeExDoc* CMFC_DigitalTimeExView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_DigitalTimeExDoc)));
	return (CMFC_DigitalTimeExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_DigitalTimeExView �޽��� ó����


int CMFC_DigitalTimeExView::OnCreate(LPCREATESTRUCT lpCreateStruct)		// �����찡 ������ �� Ÿ�̸Ӹ� �����Ѵ�.
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	SetTimer(0, 1000, NULL);		// Ÿ�̸� ����
	m_bTimerRun = true;				// Ÿ�̸� ����

	return 0;
}


void CMFC_DigitalTimeExView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	int hour;
	CString str;
	CTime timer;
	timer = CTime::GetCurrentTime();

	if (m_bTimerType)
	{
		m_strTimer.Format(_T("����� %d�� %d�� %d�� %d�� %d�� %d��"),
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
		m_strTimer.Format(_T("������ %s %d�� %d�� %d��"), str, hour, timer.GetMinute(), timer.GetSecond());
	}
	Invalidate();
	CView::OnTimer(nIDEvent);
}


void CMFC_DigitalTimeExView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_bTimerType)		// ��, ��, ��, ��, ��, �� ���·� ��� ����
	{
		if (AfxMessageBox(_T("��, ��, �� ���·� ǥ���Ͻðڽ��ϱ�?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			m_bTimerType = false;
		}
	}
	else					// ��, ��, �� ���·� ��� ����
	{
		if (AfxMessageBox(_T("��, ��, ��, ��, ��, �� ���·� ǥ���Ͻðڽ��ϱ�?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			m_bTimerType = true;
		}
	}
	CView::OnLButtonDown(nFlags, point);
}


void CMFC_DigitalTimeExView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_bTimerRun == false)			// Ÿ�̸Ӱ� ���� �� �� �� �޽��� �ڽ� ���
	{
		if (AfxMessageBox(_T("�����нð踦 ���۽�Ű�ڽ��ϱ�?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			SetTimer(0, 1000, NULL);	// Ÿ�̸� ����
			m_bTimerRun = true;			// Ÿ�̸� ���� => true
		}
	}
	else								// Ÿ�̸Ӱ� ���� ���� �� �޽��� �ڽ� ���
	{
		if (AfxMessageBox(_T("������ �����нð� ������ ���߽ðڽ��ϱ�?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			KillTimer(0);				// Ÿ�̸� ����
			m_bTimerRun = false;		// Ÿ�̸� ���� => false
		}
	}


	CView::OnRButtonDown(nFlags, point);
}


void CMFC_DigitalTimeExView::OnDestroy()
{	
	if (m_bTimerRun)
	{
		KillTimer(0);	// Ÿ�̸� ����
	}
	CView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
