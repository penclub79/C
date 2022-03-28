
// MFC21_Text_SearchExDlg.cpp : 구현 파일
//
#define _CRT_SECURE_NO_WARNINGS
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
, m_strBuffer(NULL)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


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
	static TCHAR BASED_CODE szFilter[] = _T("텍스트 파일(*.txt) | *.txt;|모든파일(*.*)|*.*||");
	
	/* 
	CFileDialog 000 (
	1 param : TRUE -> 열기용 , FALSE -> 저장용
	2 param : NULL -> 기본 확장자
	3 param : NULL -> 기본 파일명
	4 param : 플래그
	5 param : 필터링 확장자
	)
	*/
	CFileDialog dlg(TRUE, _T("*.txt"), _T(""), OFN_HIDEREADONLY, szFilter);
	CString strTmp;
	if (IDOK == dlg.DoModal())
	{
		CString strPathName = dlg.GetPathName();
		///////////////////////////////////////////////////////////////////
		char Tv_StrTest[1000];

		memset(Tv_StrTest, 0, 1000);

		char* str = Tv_StrTest;
		int temp = 0;

		for (int i = 0; i < 100; i++)
		{
			*str = 100 - i;
			str++;
		}
		////////////////////////////////////////////////////////////////////
		// 파일 내용 읽어오기
		CString strFileName = dlg.GetFileName();	// 파일명
		char* stText = LPSTR(LPCTSTR(strPathName));	// 형변환 CString -> char
		int fileLoc = 0;
		FILE* p_File = fopen("D:\\C\\MFC\\MFC21_Text_SearchEx\\MFC21_Text_SearchEx\\upload\\test.txt", "r");	// 파일 내용열기
		//m_strBuffer = fgets(m_strBuffer, 1000, p_File);
		//char *test;

		CSearch strSearch;
		char stFindtext[] = { "ls" };
		
		//BOOL CSearch::IsStrFindGet(char* A_PtrFindStr, int A_FindStrSize, char* A_PtrTextStr, int A_TextSize)
		
		//fseek(p_File, 0, SEEK_END);  // 파일 포인터를 파일의 끝으로 이동
		//fileLoc = ftell(p_File);	// 파일 포인터의 현재 위치
		//m_strBuffer = (char*)malloc(fileLoc + 1);		// 파일 크기 + 1바이트(문자열 마지막)만큼 동적 메모리 할당
		//memset(m_strBuffer, 0, fileLoc + 1);			// 파일 크기 + 1바이트만큼 메모리를 0으로 초기화

		//strSearch.IsStrFindGet(stFindtext, sizeof(stFindtext), m_strBuffer, fileLoc);
		//fseek(p_File, 0, SEEK_SET);			// 파일 포인터를 파일의 처음으로 이동
		//
		////m_str = malloc(size);

		////m_obj1 = newn fld; 클래스 사용
		//
		////fread(m_strBuffer, 1, fileLoc, p_File); // 메모리 동적할당 후 사용할 함수
		//
		//CString test;
		//test = (LPSTR)(LPCSTR)m_strBuffer;
		
		SetDlgItemText(IDC_EDIT_IMPORT, strPathName);
		//strFile.Open(_T("test.txt"), CFile::Read);
		//SetDlgItemText(IDC_STATIC_TEXT, test);


		
	}

	if (m_strBuffer)
	{
		free(m_strBuffer);
		m_strBuffer = NULL;
	}
	
}
