
// MFC4_StopWatchExView.cpp : CMFC4_StopWatchExView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFC4_StopWatchEx.h"
#endif

#include "MFC4_StopWatchExDoc.h"
#include "MFC4_StopWatchExView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC4_StopWatchExView

IMPLEMENT_DYNCREATE(CMFC4_StopWatchExView, CView)

BEGIN_MESSAGE_MAP(CMFC4_StopWatchExView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFC4_StopWatchExView ����/�Ҹ�

CMFC4_StopWatchExView::CMFC4_StopWatchExView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFC4_StopWatchExView::~CMFC4_StopWatchExView()
{
}

BOOL CMFC4_StopWatchExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC4_StopWatchExView �׸���

void CMFC4_StopWatchExView::OnDraw(CDC* /*pDC*/)
{
	CMFC4_StopWatchExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMFC4_StopWatchExView �μ�

BOOL CMFC4_StopWatchExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC4_StopWatchExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC4_StopWatchExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFC4_StopWatchExView ����

#ifdef _DEBUG
void CMFC4_StopWatchExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC4_StopWatchExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC4_StopWatchExDoc* CMFC4_StopWatchExView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC4_StopWatchExDoc)));
	return (CMFC4_StopWatchExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC4_StopWatchExView �޽��� ó����
