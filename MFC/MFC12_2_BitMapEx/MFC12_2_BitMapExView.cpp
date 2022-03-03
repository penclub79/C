
// MFC12_2_BitMapExView.cpp : CMFC12_2_BitMapExView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_IMAGE_ROAD, &CMFC12_2_BitMapExView::OnImageRoad)
END_MESSAGE_MAP()

// CMFC12_2_BitMapExView 생성/소멸

CMFC12_2_BitMapExView::CMFC12_2_BitMapExView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC12_2_BitMapExView 그리기

void CMFC12_2_BitMapExView::OnDraw(CDC* _pDC)
{
	CMFC12_2_BitMapExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	
	// 포인터 변수와 변수 초기화
	BITMAPFILEHEADER *pfh = NULL;
	BITMAPINFOHEADER *pih = NULL;
	BYTE *pRaster = NULL;
	int bx, by = 0;
	HDC hdc = NULL;

	// 버퍼 메모리에 데이터가 있다면
	// 헤더와 래스터의 값을 받아와서 출력한다.
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

// CMFC12_2_BitMapExView 인쇄

BOOL CMFC12_2_BitMapExView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC12_2_BitMapExView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC12_2_BitMapExView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFC12_2_BitMapExView 진단

#ifdef _DEBUG
void CMFC12_2_BitMapExView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC12_2_BitMapExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC12_2_BitMapExDoc* CMFC12_2_BitMapExView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC12_2_BitMapExDoc)));
	return (CMFC12_2_BitMapExDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC12_2_BitMapExView 메시지 처리기


void CMFC12_2_BitMapExView::OnImageRoad()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
	HANDLE ahFile[2] = { INVALID_HANDLE_VALUE };	// 초기화
	DWORD adwFileSize[2] = { 0 };					// 초기화
	DWORD adwRead[2];
	TCHAR aszFileName[2][MAX_PATH] = { 0 };		
	
	memset(&adwRead[0], 0, sizeof(adwRead));		// 초기화 함수

	wsprintf(aszFileName[0], L"D:\\C\\MFC\\MFC12_2_BitMapEx\\background.bmp");
	wsprintf(aszFileName[1], L"D:\\C\\MFC\\MFC12_2_BitMapEx\\insert.bmp");

	for (int i = 0; i < 2; i++) {

		ahFile[i] = CreateFile(aszFileName[i], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (ahFile[i] == INVALID_HANDLE_VALUE)
		{
			break;
		}

		adwFileSize[i] = GetFileSize(ahFile[i], NULL);

		// 버퍼에 값이 있는지 체크
		if (m_apBuffer[i])	
		{
			free(m_apBuffer[i]);
			m_apBuffer[i] = NULL;
		}

		// 버퍼 메모리 할당
		m_apBuffer[i] = (PBYTE)malloc(adwFileSize[i]);	

		// 파일 읽어오기
		ReadFile(ahFile[i], m_apBuffer[i], adwFileSize[i], &adwRead[i], NULL);
		
		// 메모리 해제
		CloseHandle(ahFile[i]);

		// 파일 핸들 닫기
		ahFile[i] = INVALID_HANDLE_VALUE;

	}

	AfxMessageBox(_T("두개의 사진을 불러왔습니다."));

	InvalidateRect(NULL);
	//OnImageRoad2();		// 두번째 DIB정보에 대한 함수
	

}

//void CMFC12_2_BitMapExView::OnImageRoad2()
//{
//	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
//	dwFileSize = GetFileSize(hFile, NULL);   // 함수라 메모리 할당받음 -> 해제 해줘야 함.
//
//	m_pBuffer = (PBYTE)malloc(dwFileSize);		// 메모리 해제
//	ReadFile(hFile, m_pBuffer, dwFileSize, &dwRead, NULL);
//
//	CloseHandle(hFile);
//	hFile = INVALID_HANDLE_VALUE;
//	AfxMessageBox(_T("두번째 사진을 불러왔습니다."));
//
//	InvalidateRect(NULL);
//}
