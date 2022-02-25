
// MFC11_GDIPlusExView.cpp : CMFC11_GDIPlusExView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFC11_GDIPlusEx.h"
#endif

#include "MFC11_GDIPlusExDoc.h"
#include "MFC11_GDIPlusExView.h"

#include "PenSizeDlg.h"
#include "EraserSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC11_GDIPlusExView

IMPLEMENT_DYNCREATE(CMFC11_GDIPlusExView, CView)

BEGIN_MESSAGE_MAP(CMFC11_GDIPlusExView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC11_GDIPlusExView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PEN_SIZE, &CMFC11_GDIPlusExView::OnPenSize)
	ON_COMMAND(ID_ERASER_SIZE, &CMFC11_GDIPlusExView::OnEraserSize)
	ON_COMMAND(ID_PEN_COLOR, &CMFC11_GDIPlusExView::OnPenColor)
	ON_COMMAND(ID_IMG_FILE_APPEND, &CMFC11_GDIPlusExView::OnImgFileAppend)
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFC11_GDIPlusExView ����/�Ҹ�

CMFC11_GDIPlusExView::CMFC11_GDIPlusExView()
: m_nPenSize(0)
, m_nEraserSize(0)
, m_ptPrev(0)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_hFile = INVALID_HANDLE_VALUE;
	m_nPenSize = 2;
	m_nEraserSize = 4;
	m_colorPen = RGB(0, 0, 0);

	m_pByte = NULL;
	memset(&m_bmFileHeader, 0, sizeof(m_bmFileHeader));
	m_lpBitmapInfo = NULL;

}

CMFC11_GDIPlusExView::~CMFC11_GDIPlusExView()
{
	if (NULL != m_pByte)
	{
		delete[] m_pByte;
		m_pByte = NULL;
	}

	if (m_hFile) { CloseHandle(m_hFile); m_hFile = INVALID_HANDLE_VALUE; }

}

BOOL CMFC11_GDIPlusExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC11_GDIPlusExView �׸���

void CMFC11_GDIPlusExView::OnDraw(CDC* pDC)
{
	CMFC11_GDIPlusExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	if (INVALID_HANDLE_VALUE != m_hFile)
	{
		DrawImageFromFile(pDC);
	}

}

void CMFC11_GDIPlusExView::DrawImageFromFile(CDC* pDC)
{
	HDC hdc = pDC->GetSafeHdc();

	TRACE(_T("DrawImageFromFile \n"));
}

// CMFC11_GDIPlusExView �μ�


void CMFC11_GDIPlusExView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC11_GDIPlusExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC11_GDIPlusExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC11_GDIPlusExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CMFC11_GDIPlusExView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC11_GDIPlusExView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC11_GDIPlusExView ����

#ifdef _DEBUG
void CMFC11_GDIPlusExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC11_GDIPlusExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC11_GDIPlusExDoc* CMFC11_GDIPlusExView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC11_GDIPlusExDoc)));
	return (CMFC11_GDIPlusExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC11_GDIPlusExView �޽��� ó����


void CMFC11_GDIPlusExView::OnPenSize()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CPenSizeDlg* pdlgPen = new CPenSizeDlg;
	if (pdlgPen->DoModal() == IDOK)
	{
		m_nPenSize = pdlgPen->m_nPenSize;
	}
}


void CMFC11_GDIPlusExView::OnEraserSize()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CEraserSizeDlg* pdlgEraser = new CEraserSizeDlg;
	if (pdlgEraser->DoModal() == IDOK)
	{
		m_nEraserSize = pdlgEraser->m_nEraserSize;
	}
}


void CMFC11_GDIPlusExView::OnPenColor()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CColorDialog colorDlg;

	if (colorDlg.DoModal() == IDOK)
	{
		m_colorPen = colorDlg.GetColor();
	}
}


void CMFC11_GDIPlusExView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_ptPrev = point;
	HCURSOR hCursor = AfxGetApp()->LoadCursorW(IDC_CURSOR_ERASER);
	SetCursor(hCursor);

	CView::OnRButtonDown(nFlags, point);
}


void CMFC11_GDIPlusExView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CClientDC dc(this);
	Graphics graphics(dc);

	Gdiplus::Color clr;			// GDI+ ���� ���󺯰�
	clr.SetFromCOLORREF(m_colorPen);

	if (nFlags == MK_LBUTTON)
	{
		HCURSOR hCursor = AfxGetApp()->LoadCursorW(IDC_CURSOR_PEN);
		SetCursor(hCursor);
		Pen pen(Color(clr), m_nPenSize);
		graphics.DrawLine(&pen, m_ptPrev.x, m_ptPrev.y, point.x, point.y);
		m_ptPrev = point;
	}
	if (nFlags == MK_RBUTTON)
	{
		HCURSOR hCursor = AfxGetApp()->LoadCursorW(IDC_CURSOR_ERASER);
		SetCursor(hCursor);
		Pen pen(Color(255, 255, 255), m_nEraserSize);
		graphics.DrawLine(&pen, m_ptPrev.x, m_ptPrev.y, point.x, point.y);
		m_ptPrev = point;
	}
	CView::OnMouseMove(nFlags, point);
}


void CMFC11_GDIPlusExView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_ptPrev = point;
	HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURSOR_PEN);
	SetCursor(hCursor);

	CView::OnLButtonDown(nFlags, point);
}


void CMFC11_GDIPlusExView::OnImgFileAppend()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	///*HWND hwnd = GetDesktopWindow();*/
	LPVOID lpDIBits = NULL;

	if (INVALID_HANDLE_VALUE != m_hFile)
		return;

	m_hFile = CreateFile(_T("D:\\C\\MFC\\UserImages.bmp"), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == m_hFile)
	{
		return;
	}

	DWORD dwReadBytes	= 0;

	// 1. BITMAPFILEHEADER ����ü ��ŭ ���Ͽ��� �б�
	ReadFile(m_hFile, &m_bmFileHeader, sizeof(BITMAPFILEHEADER), &dwReadBytes, NULL);

	// 2. DIB�� ����� BITMAPINFO ����ü�� ���Ͽ��� �б�
	// BITMAPINFO�� BITMAPINFOHEADER + RGBQUAD�� ����ü
	int iBitmapInfoSize = m_bmFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
	m_pByte = new BYTE[iBitmapInfoSize];
	ReadFile(m_hFile, m_pByte, iBitmapInfoSize, &dwReadBytes, NULL);

	//// 3. pixel Data�� �޾Ƽ� DIB�� ����
	//HDC hdc = GetDC(hwnd);
	//lpBitmapInfo = (LPBITMAPINFO)pByte;
	//HBITMAP hDib = CreateDIBSection(hdc, lpBitmapInfo, DIB_RGB_COLORS, &lpDIBits, NULL, 0);
	//ReadFile(hFile, lpDIBits, lpBitmapInfo->bmiHeader.biSizeImage, &dwReadBytes, NULL);
	//ReleaseDC(hwnd, hdc);

	//// 4. ������
	//if (pByte) { delete[] pByte; pByte = 0; }

	InvalidateRect(NULL, TRUE);
}
