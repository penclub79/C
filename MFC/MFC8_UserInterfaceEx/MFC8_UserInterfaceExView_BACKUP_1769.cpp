
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
<<<<<<< HEAD
	ON_WM_MOUSEMOVE()
=======
>>>>>>> 9abe66b48605c633308cb4c163889a4a4a8d420d
END_MESSAGE_MAP()

// CMFC8_UserInterfaceExView ����/�Ҹ�

CMFC8_UserInterfaceExView::CMFC8_UserInterfaceExView()
: m_nDrawMode(0)
, m_nHatchStyle(0)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

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

void CMFC8_UserInterfaceExView::OnDraw(CDC* /*pDC*/)
{
	CMFC8_UserInterfaceExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
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
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
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
<<<<<<< HEAD

	// ������������ ������ ����
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	// ���¹ٿ� �׸��� ��� ���
	pFrame->m_wndStatusBar.SetWindowText(_T("���� �׸���"));
=======
>>>>>>> 9abe66b48605c633308cb4c163889a4a4a8d420d
}


void CMFC8_UserInterfaceExView::OnEllipse()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// �� �׸��� ��� ����
<<<<<<< HEAD
	m_nDrawMode = ELLIPSE_MODE;

	// ������������ ������ ����
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	// ���¹ٿ� �׸��� ��� ���
	pFrame->m_wndStatusBar.SetWindowText(_T("�� �׸���"));
=======
	AfxMessageBox(_T("���� �׸��ϴ�."));
	m_nDrawMode = ELLIPSE_MODE;
>>>>>>> 9abe66b48605c633308cb4c163889a4a4a8d420d
}


void CMFC8_UserInterfaceExView::OnPolygon()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// �ٰ��� �׸��� ��� ����
<<<<<<< HEAD
	m_nDrawMode = POLYGON_MODE;

	// ������������ ������ ����
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	// ���¹ٿ� �׸��� ��� ���
	pFrame->m_wndStatusBar.SetWindowText(_T("�ٰ��� �׸���"));
=======
	AfxMessageBox(_T("�ٰ����� �׸��ϴ�."));
	m_nDrawMode = POLYGON_MODE;
>>>>>>> 9abe66b48605c633308cb4c163889a4a4a8d420d
}


void CMFC8_UserInterfaceExView::OnLineColor()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// �� ���� �޴��� �������� �� �޽��� ���
	AfxMessageBox(_T("���� ������ �����մϴ�."));
	
}


void CMFC8_UserInterfaceExView::OnFaceColor()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// �� ���� �޴��� �������� �� �޽��� ���
	AfxMessageBox(_T("���� ������ �����մϴ�."));
	
}


void CMFC8_UserInterfaceExView::OnBdiagonal()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� 45�� ���� ����
	AfxMessageBox(_T("���� 45�� �������� �����մϴ�."));
	m_nHatchStyle = HS_BDIAGONAL;
	
}


void CMFC8_UserInterfaceExView::OnCross()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���ڰ� ���� ����
	AfxMessageBox(_T("���ڰ� �������� �����մϴ�."));
	m_nHatchStyle = HS_CROSS;
}


void CMFC8_UserInterfaceExView::OnVertical()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� ���� ����
	AfxMessageBox(_T("���� �������� �����մϴ�."));
	m_nHatchStyle = HS_VERTICAL;
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
<<<<<<< HEAD
}


void CMFC8_UserInterfaceExView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	// ������������ ������ ����
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	CString strPoint;
	strPoint.Format(_T("���콺 ��ġ x : %d, y : %d"), point.x, point.y);

	// ���� �߰��� �ҿ� ���콺 ��ġ ���
	pFrame->m_wndStatusBar.SetPaneText(1, strPoint);

	CView::OnMouseMove(nFlags, point);
=======
>>>>>>> 9abe66b48605c633308cb4c163889a4a4a8d420d
}
