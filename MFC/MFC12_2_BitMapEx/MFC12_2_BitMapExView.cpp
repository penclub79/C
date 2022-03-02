
// MFC12_2_BitMapExView.cpp : CMFC12_2_BitMapExView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFC12_2_BitMapEx.h"
#endif

#include "MFC12_2_BitMapExDoc.h"
#include "MFC12_2_BitMapExView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC12_2_BitMapExView

IMPLEMENT_DYNCREATE(CMFC12_2_BitMapExView, CView)

BEGIN_MESSAGE_MAP(CMFC12_2_BitMapExView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_IMAGE_ROAD, &CMFC12_2_BitMapExView::OnImageRoad)
END_MESSAGE_MAP()

// CMFC12_2_BitMapExView ����/�Ҹ�

CMFC12_2_BitMapExView::CMFC12_2_BitMapExView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFC12_2_BitMapExView::~CMFC12_2_BitMapExView()
{
}

BOOL CMFC12_2_BitMapExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC12_2_BitMapExView �׸���

void CMFC12_2_BitMapExView::OnDraw(CDC* /*pDC*/)
{
	CMFC12_2_BitMapExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMFC12_2_BitMapExView �μ�

BOOL CMFC12_2_BitMapExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC12_2_BitMapExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC12_2_BitMapExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFC12_2_BitMapExView ����

#ifdef _DEBUG
void CMFC12_2_BitMapExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC12_2_BitMapExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC12_2_BitMapExDoc* CMFC12_2_BitMapExView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC12_2_BitMapExDoc)));
	return (CMFC12_2_BitMapExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC12_2_BitMapExView �޽��� ó����


void CMFC12_2_BitMapExView::OnImageRoad()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	AfxMessageBox(_T("�ҷ��Խ��ϴ�."));
}
