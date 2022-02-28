
// MFC12_BitMapExView.cpp : CMFC12_BitMapExView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC12_BitMapExView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_IMAGE_ROAD, &CMFC12_BitMapExView::OnImageRoad)
END_MESSAGE_MAP()

// CMFC12_BitMapExView 생성/소멸

CMFC12_BitMapExView::CMFC12_BitMapExView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFC12_BitMapExView::~CMFC12_BitMapExView()
{
}

BOOL CMFC12_BitMapExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC12_BitMapExView 그리기

//void CMFC12_BitMapExView::OnDraw(CDC* /*pDC*/)
//{
//	CMFC12_BitMapExDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	if (!pDoc)
//		return;
//
//	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
//}

void CMFC12_BitMapExView::OnDraw(CDC* /*pDC*/)
{
	CMFC12_BitMapExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

}

// CMFC12_BitMapExView 인쇄


void CMFC12_BitMapExView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC12_BitMapExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC12_BitMapExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC12_BitMapExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
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


// CMFC12_BitMapExView 진단

#ifdef _DEBUG
void CMFC12_BitMapExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC12_BitMapExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC12_BitMapExDoc* CMFC12_BitMapExView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC12_BitMapExDoc)));
	return (CMFC12_BitMapExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC12_BitMapExView 메시지 처리기


//void CMFC12_BitMapExView::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	BITMAPFILEHEADER *fh = NULL;
	BITMAPINFOHEADER *ih;
	int bx, by;
	BYTE *pRaster;
	HDC hdc;
	
	HANDLE hFile;
	DWORD FileSize, dwRead;

	hFile = CreateFile(L"D:\\C\\MFC\\test.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	FileSize = GetFileSize(hFile, NULL);

	if (fh)
	{
		free(fh);
	}
	fh = (BITMAPFILEHEADER *)malloc(FileSize);
	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
	CloseHandle(hFile);

	pRaster = (PBYTE)fh + fh->bfOffBits;
	ih = (BITMAPINFOHEADER *)((PBYTE)fh + sizeof(BITMAPFILEHEADER));
	bx = ih->biWidth;
	by = ih->biHeight;

	CClientDC dc(this);
	SetDIBitsToDevice(dc.m_hDC, 0, 0, bx, by, 0, 0, 0, by, pRaster, (BITMAPINFO *)ih, DIB_RGB_COLORS);
	AfxMessageBox(_T("읽어 왔습니다."));

}
