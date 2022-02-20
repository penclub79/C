
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
END_MESSAGE_MAP()

// CMFC_DigitalTimeExView ����/�Ҹ�

CMFC_DigitalTimeExView::CMFC_DigitalTimeExView()
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

void CMFC_DigitalTimeExView::OnDraw(CDC* /*pDC*/)
{
	CMFC_DigitalTimeExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
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
