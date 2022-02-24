
// MFC8_UserInterfaceExView.cpp : CMFC8_UserInterfaceExView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
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

// CMFC8_UserInterfaceExView ����/�Ҹ�

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
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	
	m_nDrawMode = LINE_MODE;				// �׸��� ��� �߿��� ������ �׸��� �ʱ�ȭ
	m_bContextMenu = true;					// ���ؽ�Ʈ �޴��� Ȱ������ �ʱ�ȭ
	m_bFirst = true;						// ó�� �׸��� ������ �ʱ�ȭ
	m_bLButtonDown = false;					// ���� ��ư�� ������ ���� ������ �ʱ�ȭ
	m_nCount = 0;							// �ٰ��� ���� ������ �迭�� ī��Ʈ �ʱ�ȭ
	for (int i = 0; i < 50; i++)			// �ٰ����� ���� ������ �迭 �ʱ�ȭ
		m_ptData[i] = 0;
	m_bHatch = false;						// ��ġ �귯���� ���ڰ� �������� �ʱ�ȭ
	m_nHatchStyle = HS_CROSS;
	m_colorPen = RGB(0, 0, 0);				// pen color�� ���������� �ʱ�ȭ
	m_colorBrush = RGB(255, 255, 255);		// brush color�� ������� �ʱ�ȭ
}

CMFC8_UserInterfaceExView::~CMFC8_UserInterfaceExView()
{
}

BOOL CMFC8_UserInterfaceExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC8_UserInterfaceExView �׸���

void CMFC8_UserInterfaceExView::OnDraw(CDC* pDC)
{
	CMFC8_UserInterfaceExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	CPen pen, *oldpen;
	pen.CreatePen(PS_SOLID, 1, m_colorPen);				// pen ��ü ����
	oldpen = pDC->SelectObject(&pen);					// pen ��ü ���
	pDC->SetROP2(R2_COPYPEN);							// COPYPEN���� ����
	CBrush brush, *oldbrush;
	if (m_bHatch)
	{
		brush.CreateSolidBrush(m_colorBrush);			// Hatch brush ��ü ����
	}
	else
	{
		brush.CreateSolidBrush(m_colorBrush);			// Solid brush ��ü ����
	}
	oldbrush = pDC->SelectObject(&brush);				// brush ��ü ���

	switch (m_nDrawMode)
	{
	case LINE_MODE:										// ���� �׸���
		pDC->MoveTo(m_ptStart);
		pDC->LineTo(m_ptPrev);
		break;
	case ELLIPSE_MODE:									// �� �׸���
		pDC->Ellipse(m_ptStart.x, m_ptStart.y, m_ptPrev.x, m_ptPrev.y);
		break;
	case POLYGON_MODE:									// �ٰ��� �׸���
		// �迭�� ������ ������� ������ ������ �ٰ��� �׸���
		pDC->Polygon(m_ptData, m_nCount);
		break;
	}
	pDC->SelectObject(oldpen);							// ���� pen���� ����
	pDC->SelectObject(oldbrush);						// ���� brush���� ����
	pen.DeleteObject();									// pen ��ü ����
	brush.DeleteObject();								// brush ��ü ����
}


// CMFC8_UserInterfaceExView �μ�


void CMFC8_UserInterfaceExView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC8_UserInterfaceExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC8_UserInterfaceExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC8_UserInterfaceExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CMFC8_UserInterfaceExView ����

#ifdef _DEBUG
void CMFC8_UserInterfaceExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC8_UserInterfaceExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC8_UserInterfaceExDoc* CMFC8_UserInterfaceExView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC8_UserInterfaceExDoc)));
	return (CMFC8_UserInterfaceExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC8_UserInterfaceExView �޽��� ó����


void CMFC8_UserInterfaceExView::OnLine()
{	
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// �����׸��� ��� ����
	m_nDrawMode = LINE_MODE;

	// ������������ ������ ����
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	// ���¹ٿ� �׸��� ��� ���
	pFrame->m_wndStatusBar.SetWindowText(_T("���� �׸���"));
}


void CMFC8_UserInterfaceExView::OnEllipse()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// �� �׸��� ��� ����
	m_nDrawMode = ELLIPSE_MODE;

	// ������������ ������ ����
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	// ���¹ٿ� �׸��� ��� ���
	pFrame->m_wndStatusBar.SetWindowText(_T("�� �׸���"));
}


void CMFC8_UserInterfaceExView::OnPolygon()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// �ٰ��� �׸��� ��� ����
	m_nDrawMode = POLYGON_MODE;

	// ������������ ������ ����
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	// ���¹ٿ� �׸��� ��� ���
	pFrame->m_wndStatusBar.SetWindowText(_T("�ٰ��� �׸���"));
}


void CMFC8_UserInterfaceExView::OnLineColor()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CColorDialog dlgColor;						// ���� ��ȭ������ �ν��Ͻ� ����
	if (dlgColor.DoModal() == IDOK)				// ������ �����ϰ� Ȯ���� ������
	{
		m_colorPen = dlgColor.GetColor();		// ������ ���� ���� ��� colorPen�� ����
	}
	Invalidate(true);
	
}


void CMFC8_UserInterfaceExView::OnFaceColor()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� 45�� ���� ����
	AfxMessageBox(_T("���� 45�� �������� �����մϴ�."));
	m_nHatchStyle = HS_BDIAGONAL;
	m_bHatch = true;
	Invalidate(true);
	
}


void CMFC8_UserInterfaceExView::OnCross()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���ڰ� ���� ����
	AfxMessageBox(_T("���ڰ� �������� �����մϴ�."));
	m_nHatchStyle = HS_CROSS;
	m_bHatch = true;
	Invalidate(true);
}


void CMFC8_UserInterfaceExView::OnVertical()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� ���� ����
	AfxMessageBox(_T("���� �������� �����մϴ�."));
	m_nHatchStyle = HS_VERTICAL;
	m_bHatch = true;
	Invalidate(true);
}


void CMFC8_UserInterfaceExView::OnUpdateLine(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	
	// ���� �׸��� ����̸� �޴��� üũ ǥ��
	pCmdUI->SetCheck(m_nDrawMode == LINE_MODE ? 1 : 0);
}


void CMFC8_UserInterfaceExView::OnUpdateEllipse(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.

	// �� �׸��� ����̸� �޴��� üũ ǥ��
	pCmdUI->SetCheck(m_nDrawMode == ELLIPSE_MODE ? 1 : 0);
}



void CMFC8_UserInterfaceExView::OnUpdatePolygon(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.

	// �ٰ��� �׸��� ����̸� �޴��� üũ ǥ��
	pCmdUI->SetCheck(m_nDrawMode == POLYGON_MODE ? 1 : 0);
}


void CMFC8_UserInterfaceExView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	// ������������ ������ ����
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

	// ������������ ������ ����
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CString strPoint;
	strPoint.Format(_T("���콺 ��ġ x: %d, y: %d"), point.x, point.y);

	// ���� �߰��� �ҿ� ���콺 ��ġ ���
	pFrame->m_wndStatusBar.SetPaneText(1, strPoint);


	CView::OnMouseMove(nFlags, point);
}


void CMFC8_UserInterfaceExView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
		case LINE_MODE :							// ���� �׸���
		case ELLIPSE_MODE :							// �� �׸���
			m_bLButtonDown = true;					// ���� ��ư�� ����
			m_ptStart = m_ptPrev = point;			// �������� ���� ���� ���� ���� ����
			m_bFirst = false;						// ó�� �׸��� �� -> false�� ��ȯ
			break;
		case POLYGON_MODE :							// �ٰ��� �׸���
			if (m_bFirst == true)					
				m_bFirst = false;					// ó�� �׸��� �� -> false�� ��ȯ
			m_ptStart = m_ptPrev = point;			// �������� ���� ���� ���� ���� ����
			m_ptData[m_nCount] = point;				// ���� ���� ����
			m_nCount++;								// ī��Ʈ ����
			break;
	}

	RECT rectClient;								// ����ü ���� ����
	SetCapture();									// ���콺 ĸó
	GetClientRect(&rectClient);						// Ŭ���̾�Ʈ ���� ����
	ClientToScreen(&rectClient);					// ��ũ�� ��ǥ�� ��ȯ
	::ClipCursor(&rectClient);						// ���콺 �̵������� Ŭ���̾�Ʈ �������� ����
	CView::OnLButtonDown(nFlags, point);
}


void CMFC8_UserInterfaceExView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_bLButtonDown)
	{
		// �׸��� ��尡 ���� �׸���, �� �׸��� �̸�
		if (m_nDrawMode == LINE_MODE || m_nDrawMode == ELLIPSE_MODE)
		{
			m_bLButtonDown = false;
			m_bFirst = true;
			ReleaseCapture();			// ���콺 ĸó ����
			::ClipCursor(NULL);			// ���콺 Ŭ�� ����
			Invalidate(true);			// ȭ�� ����
		}
	}

	CView::OnLButtonUp(nFlags, point);
}


void CMFC8_UserInterfaceExView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	// �ٰ��� �׸���
	if (m_nDrawMode == POLYGON_MODE)
	{
		if (!m_bFirst)			// ó�� �׸��� ���� �ƴϸ�
		{
			m_bContextMenu = FALSE;			// ���ؽ�Ʈ �޴� ��Ȱ��

			// �迭�� ���� ���� �����ϰ� ī��Ʈ ����
			m_ptData[m_nCount] = point;
			m_nCount++;

			// üũ���� �ʱ�ȭ => �ٽ� �ٰ����� �׸��� ����
			m_bFirst = TRUE;
			ReleaseCapture();
			::ClipCursor(NULL);
			Invalidate(TRUE);
		}
	}

	CView::OnRButtonDown(nFlags, point);
}
