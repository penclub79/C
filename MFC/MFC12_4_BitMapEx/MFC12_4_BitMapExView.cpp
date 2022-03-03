
// MFC12_4_BitMapExView.cpp : CMFC12_4_BitMapExView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFC12_4_BitMapEx.h"
#endif

#include "MFC12_4_BitMapExDoc.h"
#include "MFC12_4_BitMapExView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC12_4_BitMapExView

IMPLEMENT_DYNCREATE(CMFC12_4_BitMapExView, CView)

BEGIN_MESSAGE_MAP(CMFC12_4_BitMapExView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_IMAGE_ROAD, &CMFC12_4_BitMapExView::OnImageRoad)
END_MESSAGE_MAP()

// CMFC12_4_BitMapExView ����/�Ҹ�

CMFC12_4_BitMapExView::CMFC12_4_BitMapExView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	for (int i = 0; i < 2; i++)
	{
		if (m_apBuffer[i])
		{
			free(m_apBuffer[i]);
			m_apBuffer[i] = NULL;
		}
	}

	if (m_pBuffer)
	{
		free(m_pBuffer);
		m_pBuffer = NULL;
	}

}

CMFC12_4_BitMapExView::~CMFC12_4_BitMapExView()
{
}

BOOL CMFC12_4_BitMapExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC12_4_BitMapExView �׸���

void CMFC12_4_BitMapExView::OnDraw(CDC* /*pDC*/)
{
	CMFC12_4_BitMapExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMFC12_4_BitMapExView �μ�

BOOL CMFC12_4_BitMapExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC12_4_BitMapExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC12_4_BitMapExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFC12_4_BitMapExView ����

#ifdef _DEBUG
void CMFC12_4_BitMapExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC12_4_BitMapExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC12_4_BitMapExDoc* CMFC12_4_BitMapExView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC12_4_BitMapExDoc)));
	return (CMFC12_4_BitMapExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC12_4_BitMapExView �޽��� ó����


void CMFC12_4_BitMapExView::OnImageRoad()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// 2�� �̹��� ���� �������� ��
	BITMAPFILEHEADER *afh[2] = { NULL };
	BITMAPINFOHEADER *aih[2] = { NULL };
	BYTE *pRaster[2] = { NULL };

	HANDLE ahFile[2] = { INVALID_HANDLE_VALUE };
	DWORD adwFileSize[2] = { 0 };
	DWORD adwRead[2] = { 0 };

	TCHAR aszFileName[2][MAX_PATH] = { 0 };

	wsprintf(aszFileName[0], L"D:\\C\\MFC\\MFC12_4_BitMapEx\\background.bmp");
	wsprintf(aszFileName[1], L"D:\\C\\MFC\\MFC12_4_BitMapEx\\insert.bmp");
	
	ahFile[0] = CreateFile(aszFileName[0], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ahFile[1] = CreateFile(aszFileName[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


}
