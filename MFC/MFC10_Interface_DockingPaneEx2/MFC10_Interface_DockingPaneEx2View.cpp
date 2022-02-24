
// MFC10_Interface_DockingPaneEx2View.cpp : CMFC10_Interface_DockingPaneEx2View Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC10_Interface_DockingPaneEx2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFC10_Interface_DockingPaneEx2View ����/�Ҹ�

CMFC10_Interface_DockingPaneEx2View::CMFC10_Interface_DockingPaneEx2View()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFC10_Interface_DockingPaneEx2View::~CMFC10_Interface_DockingPaneEx2View()
{
}

BOOL CMFC10_Interface_DockingPaneEx2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC10_Interface_DockingPaneEx2View �׸���

void CMFC10_Interface_DockingPaneEx2View::OnDraw(CDC* /*pDC*/)
{
	CMFC10_Interface_DockingPaneEx2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMFC10_Interface_DockingPaneEx2View �μ�


void CMFC10_Interface_DockingPaneEx2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC10_Interface_DockingPaneEx2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC10_Interface_DockingPaneEx2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC10_Interface_DockingPaneEx2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CMFC10_Interface_DockingPaneEx2View ����

#ifdef _DEBUG
void CMFC10_Interface_DockingPaneEx2View::AssertValid() const
{
	CView::AssertValid();
}

void CMFC10_Interface_DockingPaneEx2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC10_Interface_DockingPaneEx2Doc* CMFC10_Interface_DockingPaneEx2View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC10_Interface_DockingPaneEx2Doc)));
	return (CMFC10_Interface_DockingPaneEx2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC10_Interface_DockingPaneEx2View �޽��� ó����
