
// MFC12_4_BitMapExView.cpp : CMFC12_4_BitMapExView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_IMAGE_ROAD, &CMFC12_4_BitMapExView::OnImageRoad)
END_MESSAGE_MAP()

// CMFC12_4_BitMapExView 생성/소멸

CMFC12_4_BitMapExView::CMFC12_4_BitMapExView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC12_4_BitMapExView 그리기

void CMFC12_4_BitMapExView::OnDraw(CDC* /*pDC*/)
{
	CMFC12_4_BitMapExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMFC12_4_BitMapExView 인쇄

BOOL CMFC12_4_BitMapExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC12_4_BitMapExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC12_4_BitMapExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFC12_4_BitMapExView 진단

#ifdef _DEBUG
void CMFC12_4_BitMapExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC12_4_BitMapExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC12_4_BitMapExDoc* CMFC12_4_BitMapExView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC12_4_BitMapExDoc)));
	return (CMFC12_4_BitMapExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC12_4_BitMapExView 메시지 처리기


void CMFC12_4_BitMapExView::OnImageRoad()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 2개 이미지 파일 가져오는 것
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
