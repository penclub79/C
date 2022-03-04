
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
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMFC12_4_BitMapExView 생성/소멸

CMFC12_4_BitMapExView::CMFC12_4_BitMapExView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_bClick = FALSE;
	m_ibx = 0;
	m_iby = 0;
	for (int i = 0; i < 2; i++)
	{
		m_apBuffer[i] = NULL;
	}
	m_pBuffer = NULL;

}

CMFC12_4_BitMapExView::~CMFC12_4_BitMapExView()
{
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

BOOL CMFC12_4_BitMapExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC12_4_BitMapExView 그리기

void CMFC12_4_BitMapExView::OnDraw(CDC* _pDC)
{
	CMFC12_4_BitMapExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	BITMAPFILEHEADER *pfh = NULL;
	BITMAPINFOHEADER *pih = NULL;
	BYTE *pRaster = NULL;
	int bx = 0, by = 0;

	HDC hdc = NULL;
	CPaintDC cdc(this);

	// 버퍼에 데이터가 있는지 체크 있다면= 출력, 없다면= 경고
	//for (int i = 2; i < 3; i++)

	if (m_pBuffer)
	{
		pfh = (BITMAPFILEHEADER *)m_pBuffer;
		pih = (BITMAPINFOHEADER *)((PBYTE)m_pBuffer + sizeof(BITMAPFILEHEADER));
		pRaster = (PBYTE)m_pBuffer + pfh->bfOffBits;

		bx = pih->biWidth;
		by = pih->biHeight;
		//m_ibx = pih->biWidth;
		//m_iby = pih->biHeight;

		::SetDIBitsToDevice(
			_pDC->GetSafeHdc(),
			m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
			0, 0, 0, by,
			pRaster,
			(BITMAPINFO *)pih, DIB_RGB_COLORS);
	}
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

	HANDLE ahFile[2] = { INVALID_HANDLE_VALUE };
	DWORD adwFileSize[2] = { 0 };
	DWORD adwRead[2] = { 0 };
	TCHAR aszFileName[2][MAX_PATH] = { 0 };
	int iBigImgID = 0;
	int iSmallImgID = 0;
	int isx = 0, isy = 0;

	// DIB헤더
	BITMAPFILEHEADER *apfh[2] = { NULL };
	BITMAPINFOHEADER *apih[2] = { NULL };
	BYTE *apRaster[2] = { NULL };
	BYTE *pRasterResult = NULL;



	wsprintf(aszFileName[0], L"D:\\C\\MFC\\MFC12_2_BitMapEx\\background.bmp");
	wsprintf(aszFileName[1], L"D:\\C\\MFC\\MFC12_2_BitMapEx\\insert.bmp");

	////////////////////////////////////////////////////////////////////////////////
	// 파일에서 버퍼로 데이터를 담는 곳 /////////////////////////////////////////////

	for (int i = 0; i < 2; i++)
	{
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

	// 파일에서 버퍼로 데이터를 담는 곳 /////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////
	// 읽은 버퍼에서 필요한 데이터를 m_pBuffer에 추출 하는 곳 ///////////////////////
	for (int i = 0; i < 2; i++)
	{
		// 두 버퍼에서 파일 정보 가져온다. 
		if (m_apBuffer[i])
		{
			apfh[i] = (BITMAPFILEHEADER *)m_apBuffer[i];
			apih[i] = (BITMAPINFOHEADER *)((PBYTE)m_apBuffer[i] + sizeof(BITMAPFILEHEADER));
			apRaster[i] = (PBYTE)m_apBuffer[i] + apfh[i]->bfOffBits;
		}
	}

	// 큰 이미지 사이즈 만큼 m_pBuffer 버퍼 할당
	if (adwFileSize[0] > adwFileSize[1])
	{
		iBigImgID = 0;
		iSmallImgID = 1;
	}
	else
	{
		iBigImgID = 1;
		iSmallImgID = 0;
	}

	// m_pBuffer 버퍼에 이미지 Merge 작업 
	m_pBuffer = (PBYTE)malloc(adwFileSize[iBigImgID]);

	// 큰 사이즈의 이미지 정보가 담긴 버퍼의 내용와 사이즈를 복사한다.
	memcpy(m_pBuffer, m_apBuffer[iBigImgID], adwFileSize[iBigImgID]);
	pRasterResult = m_pBuffer + apfh[iBigImgID]->bfOffBits;

	// 헤더 크기 그림1 + 그림2

	int iIndexSrc = 0;
	int iIndexDst = 0;
	int iWidthBitSrc = (apih[iSmallImgID]->biWidth * (apih[iSmallImgID]->biBitCount / 8) + 3) & ~3;
	int iWidthBitDst = (apih[iBigImgID]->biWidth * (apih[iBigImgID]->biBitCount / 8) + 3) & ~3;
	m_ibx = apih[iBigImgID]->biWidth;
	m_iby = apih[iBigImgID]->biHeight;

	for (int ih = 0; ih < apih[iSmallImgID]->biHeight; ih++)
	{
		for (int iw = 0; iw < iWidthBitSrc / 3; iw++)
		{
			iIndexSrc = iw * 3 + 0 + (iWidthBitSrc)*ih;
			iIndexDst = iw * 3 + 0 + (iWidthBitDst)*ih;

			pRasterResult[iIndexDst] = (apRaster[iBigImgID][iIndexDst] + apRaster[iSmallImgID][iIndexSrc]) / 2;	// R
			//pRasterResult[iIndexDst] = (apRaster[iSmallImgID][iIndexSrc]);	// R

			iIndexSrc = iw * 3 + 1 + (iWidthBitSrc)*ih;
			iIndexDst = iw * 3 + 1 + (iWidthBitDst)*ih;

			pRasterResult[iIndexDst] = (apRaster[iBigImgID][iIndexDst] + apRaster[iSmallImgID][iIndexSrc]) / 2; // G
			//pRasterResult[iIndexDst] = (apRaster[iSmallImgID][iIndexSrc]); // G

			iIndexSrc = iw * 3 + 2 + (iWidthBitSrc)*ih;
			iIndexDst = iw * 3 + 2 + (iWidthBitDst)*ih;

			pRasterResult[iIndexDst] = (apRaster[iBigImgID][iIndexDst] + apRaster[iSmallImgID][iIndexSrc]) / 2; // B
			//pRasterResult[iIndexDst] = (apRaster[iSmallImgID][iIndexSrc]); // B
		}
	}
	// 읽은 버퍼에서 필요한 데이터를 m_pBuffer에 추출 하는 곳 ///////////////////////
	////////////////////////////////////////////////////////////////////////////////

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = m_rect.left + apih[iBigImgID]->biWidth;
	m_rect.bottom = m_rect.top + apih[iBigImgID]->biHeight;


	InvalidateRect(NULL);


}


void CMFC12_4_BitMapExView::OnLButtonDown(UINT nFlags, CPoint _point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	m_bClick = TRUE;		// 마우스 클릭 true
	Invalidate(true);	// 화면 갱신
	
	//if (m_rect.PtInRect(_point))
	//{

	// 이동거리 계산을 위한 좌표 기억
	m_cpMouse = _point;
	
	SetCapture();

	CView::OnLButtonDown(nFlags, _point);
}


void CMFC12_4_BitMapExView::OnLButtonUp(UINT nFlags, CPoint _point)
{
	// SetCapture 기능 해제 필요
	ReleaseCapture();
	m_bClick = FALSE;

	CView::OnLButtonUp(nFlags, _point);
}


void CMFC12_4_BitMapExView::OnMouseMove(UINT nFlags, CPoint _point)
{
	CPoint cpDistance;  // 마우스 이동 거리를 계산.
	BITMAPFILEHEADER *pfh = NULL;
	BITMAPINFOHEADER *pih = NULL;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_bClick == TRUE)
	{
		// 마우스가 이동한 거리를 계산.

		int iWidth = m_rect.Width();
		int iHeight = m_rect.Height();

		TRACE(_T("x : %d , y : %d \n"), _point.x, _point.y);
		TRACE(_T("right : %d , bottom : %d \n"), m_rect.right, m_rect.bottom);
		cpDistance.x = _point.x + m_rect.right;		// 0			0 + ? = X
		cpDistance.y = _point.y - m_rect.bottom;	// 0			0 + ? = Y
		
		// 마우스 이동거리만큼 사진을 옮긴다.
		m_rect.left = cpDistance.x;
		m_rect.top = cpDistance.y;
		m_rect.right = m_rect.left + iWidth;
		m_rect.bottom = m_rect.top + iHeight;

		// 화면갱신
		Invalidate();		
	}


	CView::OnMouseMove(nFlags, _point);
}
