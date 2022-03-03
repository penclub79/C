
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
	for (int i = 0; i < 2; i++)
	{
		m_apBuffer[i] = NULL;
	}
	


}

CMFC12_2_BitMapExView::~CMFC12_2_BitMapExView()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_apBuffer[i])
		{
			free(m_apBuffer[i]);
			m_apBuffer[i] = NULL;
		}
	}
	
}

BOOL CMFC12_2_BitMapExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC12_2_BitMapExView �׸���

void CMFC12_2_BitMapExView::OnDraw(CDC* _pDC)
{
	CMFC12_2_BitMapExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	
	// ������ ������ ���� �ʱ�ȭ
	BITMAPFILEHEADER *pfh = NULL;
	BITMAPINFOHEADER *pih = NULL;
	BYTE *pRaster = NULL;
	int bx, by = 0;
	HDC hdc = NULL;

	// ���� �޸𸮿� �����Ͱ� �ִٸ�
	// ����� �������� ���� �޾ƿͼ� ����Ѵ�.
	for (int i = 0; i < 2; i++)
	{
		if (m_apBuffer[i])
		{
			pfh = (BITMAPFILEHEADER*)m_apBuffer[i];
			pih = (BITMAPINFOHEADER*)((PBYTE)m_apBuffer[i] + sizeof(BITMAPFILEHEADER));
			pRaster = (PBYTE)m_apBuffer[i] + pfh->bfOffBits;

			bx = pih->biWidth;
			by = pih->biHeight;

			SetDIBitsToDevice(_pDC->GetSafeHdc(), 0, 0, bx, by, 0, 0, 0, by, pRaster, (BITMAPINFO *)pih, DIB_RGB_COLORS);
		}
	}

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
	
	HANDLE ahFile[2] = { INVALID_HANDLE_VALUE };	// �ʱ�ȭ
	DWORD adwFileSize[2] = { 0 };					// �ʱ�ȭ
	DWORD adwRead[2];
	TCHAR aszFileName[2][MAX_PATH] = { 0 };		
	
	memset(&adwRead[0], 0, sizeof(adwRead));		// �ʱ�ȭ �Լ�

	wsprintf(aszFileName[0], L"D:\\C\\MFC\\MFC12_2_BitMapEx\\background.bmp");
	wsprintf(aszFileName[1], L"D:\\C\\MFC\\MFC12_2_BitMapEx\\insert.bmp");

	for (int i = 0; i < 2; i++) {

		ahFile[i] = CreateFile(aszFileName[i], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (ahFile[i] == INVALID_HANDLE_VALUE)
		{
			break;
		}

		adwFileSize[i] = GetFileSize(ahFile[i], NULL);

		// ���ۿ� ���� �ִ��� üũ
		if (m_apBuffer[i])	
		{
			free(m_apBuffer[i]);
			m_apBuffer[i] = NULL;
		}

		// ���� �޸� �Ҵ�
		m_apBuffer[i] = (PBYTE)malloc(adwFileSize[i]);	

		// ���� �о����
		ReadFile(ahFile[i], m_apBuffer[i], adwFileSize[i], &adwRead[i], NULL);
		
		// �޸� ����
		CloseHandle(ahFile[i]);

		// ���� �ڵ� �ݱ�
		ahFile[i] = INVALID_HANDLE_VALUE;

	}

	AfxMessageBox(_T("�ΰ��� ������ �ҷ��Խ��ϴ�."));

	InvalidateRect(NULL);
	//OnImageRoad2();		// �ι�° DIB������ ���� �Լ�
	

}

//void CMFC12_2_BitMapExView::OnImageRoad2()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//	HANDLE hFile = INVALID_HANDLE_VALUE;
//	DWORD dwFileSize = 0;
//	DWORD dwRead = 0;
//
//	hFile = CreateFile(L"D:\\C\\MFC\\MFC12_2_BitMapEx\\insert.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	if (hFile == INVALID_HANDLE_VALUE)
//	{
//		return;
//	}
//
//	if (m_pBuffer)
//	{
//		free(m_pBuffer);
//		m_pBuffer = NULL;
//	}
//
//	dwFileSize = GetFileSize(hFile, NULL);   // �Լ��� �޸� �Ҵ���� -> ���� ����� ��.
//
//	m_pBuffer = (PBYTE)malloc(dwFileSize);		// �޸� ����
//	ReadFile(hFile, m_pBuffer, dwFileSize, &dwRead, NULL);
//
//	CloseHandle(hFile);
//	hFile = INVALID_HANDLE_VALUE;
//	AfxMessageBox(_T("�ι�° ������ �ҷ��Խ��ϴ�."));
//
//	InvalidateRect(NULL);
//}
