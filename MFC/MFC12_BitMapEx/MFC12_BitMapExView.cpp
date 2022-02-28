
// MFC12_BitMapExView.cpp : CMFC12_BitMapExView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFC12_BitMapEx.h"
#endif

#include "MFC12_BitMapExDoc.h"
#include "MFC12_BitMapExView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC12_BitMapExView

IMPLEMENT_DYNCREATE(CMFC12_BitMapExView, CView)

BEGIN_MESSAGE_MAP(CMFC12_BitMapExView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC12_BitMapExView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_IMAGE_ROAD, &CMFC12_BitMapExView::OnImageRoad)
END_MESSAGE_MAP()

// CMFC12_BitMapExView ����/�Ҹ�

CMFC12_BitMapExView::CMFC12_BitMapExView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_pBuffer = NULL;
}

CMFC12_BitMapExView::~CMFC12_BitMapExView()
{
	if (m_pBuffer)
	{
		free(m_pBuffer);
		m_pBuffer = NULL;
	}
}

BOOL CMFC12_BitMapExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC12_BitMapExView �׸���

//void CMFC12_BitMapExView::OnDraw(CDC* /*pDC*/)
//{
//	CMFC12_BitMapExDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	if (!pDoc)
//		return;
//
//	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
//}

void CMFC12_BitMapExView::OnDraw(CDC* _pDC)
{
	CMFC12_BitMapExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	BYTE *pFileData = NULL;
	BITMAPFILEHEADER *pfh = NULL;
	BITMAPINFOHEADER *pih = NULL;
	int bx, by = 0;
	BYTE *pRaster = NULL;
	HDC hdc = NULL;

	if (m_pBuffer)
	{
		pfh = (BITMAPFILEHEADER*)m_pBuffer;
		pih = (BITMAPINFOHEADER *)((PBYTE)m_pBuffer + sizeof(BITMAPFILEHEADER));
		pRaster = (PBYTE)m_pBuffer + pfh->bfOffBits;


		bx = pih->biWidth;
		by = pih->biHeight;

		//CClientDC dc(this);
		SetDIBitsToDevice(_pDC->GetSafeHdc(), 0, 0, bx, by, 0, 0, 0, by, pRaster, (BITMAPINFO *)pih, DIB_RGB_COLORS);

	}
}

// CMFC12_BitMapExView �μ�


void CMFC12_BitMapExView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC12_BitMapExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC12_BitMapExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC12_BitMapExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CMFC12_BitMapExView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC12_BitMapExView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC12_BitMapExView ����

#ifdef _DEBUG
void CMFC12_BitMapExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC12_BitMapExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC12_BitMapExDoc* CMFC12_BitMapExView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC12_BitMapExDoc)));
	return (CMFC12_BitMapExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC12_BitMapExView �޽��� ó����


//void CMFC12_BitMapExView::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
//	BITMAPFILEHEADER *fh = NULL;
//
//	
//	
//	
//
//	CView::OnLButtonDown(nFlags, point);
//}


void CMFC12_BitMapExView::OnImageRoad()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwFileSize = 0;
	DWORD dwRead = 0;

	hFile = CreateFile(L"D:\\C\\MFC\\MFC12_BitMapEx\\background.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	if (m_pBuffer)
	{
		free(m_pBuffer);
		m_pBuffer = NULL;
	}

	AfxMessageBox(_T("�о� �Խ��ϴ�."));

	dwFileSize = GetFileSize(hFile, NULL);

	m_pBuffer = (PBYTE)malloc(dwFileSize);
	ReadFile(hFile, m_pBuffer, dwFileSize, &dwRead, NULL);

	CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;



	//hFile = CreateFile(L"D:\\C\\MFC\\MFC12_BitMapEx\\insert.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//if (hFile == INVALID_HANDLE_VALUE)
	//{
	//	return;
	//}

	//if (m_pBuffer)
	//{
	//	free(m_pBuffer);
	//	m_pBuffer = NULL;
	//}

	//AfxMessageBox(_T("�о� �Խ��ϴ�."));

	//dwFileSize = GetFileSize(hFile, NULL);

	//m_pBuffer = (PBYTE)malloc(dwFileSize);
	//ReadFile(hFile, m_pBuffer, dwFileSize, &dwRead, NULL);

	//CloseHandle(hFile);
	//hFile = INVALID_HANDLE_VALUE;

	InvalidateRect(NULL);
}
