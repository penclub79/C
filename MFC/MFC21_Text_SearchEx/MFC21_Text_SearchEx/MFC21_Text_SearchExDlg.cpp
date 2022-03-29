
// MFC21_Text_SearchExDlg.cpp : 구현 파일
//
//#define _CRT_SECURE_NO_WARNINGS
//#define MAX 1000


#include "stdafx.h"
#include "MFC21_Text_SearchEx.h"
#include "MFC21_Text_SearchExDlg.h"
#include "Search.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnButtonImport();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//ON_COMMAND(IDC_BUTTON_IMPORT, &CAboutDlg::OnButtonImport)
END_MESSAGE_MAP()


// CMFC21_Text_SearchExDlg 대화 상자



CMFC21_Text_SearchExDlg::CMFC21_Text_SearchExDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CMFC21_Text_SearchExDlg::IDD, pParent)
//, m_pszBuffer(NULL)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	CSearch* pSearch = new CSearch;
	St_Test stResult = { 0 };

	int* piBuffer = NULL;				// 메모리주소 변수
	int iSum = 0;
	

	// 동적 메모리 할당
	//pszBuffer = (char*)malloc((sizeof(char))* (sizeof(iArr) / sizeof(int)));
	piBuffer = (int*)malloc(1000 * sizeof(int));
	
	// memset
	pSearch->InputValue(piBuffer, 1000, 0);

	// 오름차순 정렬
	pSearch->ValueAsc(piBuffer, 1000);

	// 3의 배수 합 구하기
	iSum = pSearch->SumResult(piBuffer, 1000);

	// 버퍼 몇번째의 Value가져오기
	pSearch->GetValue(piBuffer, 1000, &stResult);// &랑 * 헤깔리면 안된다.

	// 메모리 해제
	if (NULL != piBuffer)
	{
		free(piBuffer);
		piBuffer = NULL;
	}
	if (NULL != pSearch)
	{
		delete pSearch;
		pSearch = NULL;
	}

	/////////////////////////////////////////////////////////////////////
	//char Tv_StrTest[1000];

	//memset(Tv_StrTest, 0, 1000);

	//char* str = Tv_StrTest;
	//int temp = 0;
	//

	//for (int i = 0; i < 100; i++)
	//{
	//	temp = 100 - i;
	//	*str = temp;
	//	str++;
	//}

}

void CMFC21_Text_SearchExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFC21_Text_SearchExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CMFC21_Text_SearchExDlg::OnClickedButtonImport)
END_MESSAGE_MAP()


// CMFC21_Text_SearchExDlg 메시지 처리기

BOOL CMFC21_Text_SearchExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFC21_Text_SearchExDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFC21_Text_SearchExDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFC21_Text_SearchExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// LoadFile 버튼
void CMFC21_Text_SearchExDlg::OnClickedButtonImport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.



	/*CString strPathName = _T("");
	static TCHAR BASED_CODE szFilter[] = _T("|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.txt"), _T("image"), OFN_HIDEREADONLY, szFilter);*/

	/*
	
	if (IDOK == dlg.DoModal())
	{
		strPathName = dlg.GetPathName();
	}
	
	
	
	*/

	
	//ConvertUnicodeToMultybyte(strPathName);
	

	// 파일 오픈 - 메모리 할당
	//FILE* pFile = fopen(strPathName, "r");

	// 파일 오픈 - 메모리 해제

	// 파일 사이즈 구하기

	// 텍스트를 읽어올 버퍼 동적 할당

	// 버퍼 - 메모리 해제
	
	// 파일 읽기

}

//
//char* CMFC21_Text_SearchExDlg::ConvertUnicodeToMultybyte(CString strUnicode)
//{
//	// 유니코드 -> 멀티바이트
//	int nLen = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
//	char* pMultibyte = new char[nLen];
//	memset(pMultibyte, 0x00, (nLen)*sizeof(char));
//
//	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, pMultibyte, nLen, NULL, NULL);
//
//	return pMultibyte;
//}
