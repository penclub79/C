
// MFC19_SocketExDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MFC19_SocketEx.h"
#include "MFC19_SocketExDlg.h"
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
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC19_SocketExDlg 대화 상자



CMFC19_SocketExDlg::CMFC19_SocketExDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CMFC19_SocketExDlg::IDD, pParent)
// 멤버변수 초기화
, m_nChatMode(1)
, m_strMyIP(_T(""))
, m_nOtherIP(_T(""))
, m_strOtherIP(_T(""))
, m_strInitLoc(0)
, m_rectDlg(0)
, m_pClient(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC19_SocketExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_IPAddress);
	DDX_Control(pDX, IDC_LIST_CHAT, m_listChat);
	//  DDX_Control(pDX, IDC_RADIO_SERVER, m_nChatMode);
	//DDX_Radio(pDX, IDC_RADIO_SERVER, m_nChatMode);
}

BEGIN_MESSAGE_MAP(CMFC19_SocketExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMFC19_SocketExDlg::OnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMFC19_SocketExDlg::OnClickedButtonSend)
	ON_WM_SIZE()
	ON_WM_SIZING()
	//ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CMFC19_SocketExDlg::OnClickedButtonClose)
	//ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CMFC19_SocketExDlg::OnClickedButtonDisconnect)
END_MESSAGE_MAP()


// CMFC19_SocketExDlg 메시지 처리기

BOOL CMFC19_SocketExDlg::OnInitDialog()
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
	// IP주소 가져오기
	char name[255];
	PHOSTENT stHostinfo;
	if (gethostname(name, sizeof(name)) == 0)
	{
		if ((stHostinfo = gethostbyname(name)) != NULL)
		{
			m_strMyIP = inet_ntoa(*(struct in_addr*)*stHostinfo->h_addr_list);
		}
	}

	/*컨트롤 초기화*/
	m_IPAddress.SetWindowText(m_strMyIP);
	m_IPAddress.EnableWindow(FALSE);
	// 포트 상자 비활성화
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
	// DisConnect 비활성화
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
	// Send 비활성화
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	// Connect 활성화
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);

	// Text Static ServerIP초기 좌표가져오기
	GetDlgItem(IDC_STATIC_SERVER_IP)->GetWindowRect(&m_strInitLoc);
	// 다이얼로그 내에 상대적 좌표로 변환
	ScreenToClient(&m_strInitLoc);
	
	CLIENT_INFO stClient = { 0 };
	stClient.iPort = 7777;
	// 포트 적용
	SetDlgItemInt(IDC_EDIT_PORT, stClient.iPort);

	//GetDlgItem(IDC_RADIO_SERVER)->EnableWindow(FALSE);
	SetDlgItemText(IDC_BUTTON_CONNECT, _T("Connect"));

	/*다이얼로그 사이즈 초기화*/
	RECT stDlgLoc = { 0 };
	GetClientRect(&stDlgLoc);

	ResizeControl((stDlgLoc.right - stDlgLoc.left) - 2, (stDlgLoc.bottom - stDlgLoc.top) + 50);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFC19_SocketExDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC19_SocketExDlg::OnPaint()
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
HCURSOR CMFC19_SocketExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFC19_SocketExDlg::OnClickedButtonConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	
	CString strIP;
	strIP = m_strMyIP;

	CLIENT_INFO stClient = { 0 };
	stClient.iPort = 7777;

	CString strUserID;
	SetDlgItemText(IDC_EDIT_USERID, strUserID);

	((CBasicSock*)AfxGetMainWnd())->Connect(strIP, strUserID, stClient.iPort);
	//((CBasicSock*)AfxGetMainWnd())->MainThread();
	
}

// 메시지 Send 버튼
void CMFC19_SocketExDlg::OnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strSend;
	CString	strInsert;
	CString strUserID;
	//CString strUserID = ((CMFC19_SocketExApp*)AfxGetApp())->GetUserID();
	
}


void CMFC19_SocketExDlg::ReceiveMessage(CString strReceive, CString strOtherUserID)
{
	
	CString strInsert;
	
}



void CMFC19_SocketExDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ResizeControl(cx, cy);
}


void CMFC19_SocketExDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
}

void CMFC19_SocketExDlg::ResizeControl(int cx, int cy)
{
	if (NULL == this->GetSafeHwnd())
		return;

	/* 위치 정보 가져오기(컨트롤) - START */
	// Server IP STATIC
	CWnd* pstrCtl = GetDlgItem(IDC_STATIC_SERVER_IP);
	if (NULL == pstrCtl)
		return;

	int iMarginX = 20;
	int iMarginY = 10;
	int iHeight = 40;

	/* 위치 정보 적용(컨트롤) - START */
	// Server IP STATIC
	pstrCtl->MoveWindow(iMarginX, 10, 100, iHeight, TRUE);

	// IPADDRESS EDIT
	pstrCtl = GetDlgItem(IDC_IPADDRESS_SERVER);
	pstrCtl->MoveWindow(iMarginX + 100 + iMarginX, 10, (cx / 3), iHeight, TRUE);

	// PORT STATIC
	pstrCtl = GetDlgItem(IDC_STATIC_PORT);
	pstrCtl->MoveWindow(iMarginX + 100 + iMarginX + (cx / 3) + iMarginX, 10, 100, iHeight, TRUE);

	// PORT EDIT
	pstrCtl = GetDlgItem(IDC_EDIT_PORT);
	pstrCtl->MoveWindow(iMarginX + 100 + iMarginX + (cx / 3) + iMarginX + 100 + iMarginX, 10, (cx - ((iMarginX + 100 + iMarginX + (cx / 3) + iMarginX + 100) + iMarginX * 2)), iHeight, TRUE);

	// USER ID STATIC
	pstrCtl = GetDlgItem(IDC_STATIC_USERID);
	pstrCtl->MoveWindow(iMarginX, iMarginY + 39 + 10, 100, iHeight, TRUE);

	// USER ID EDIT
	pstrCtl = GetDlgItem(IDC_EDIT_USERID);
	pstrCtl->MoveWindow(iMarginX + 100 + iMarginX, iMarginY + 39 + 10, cx / 3, iHeight, TRUE);

	// CONNECT BUTTON
	pstrCtl = GetDlgItem(IDC_BUTTON_CONNECT);
	pstrCtl->MoveWindow(iMarginX, iMarginY + 39 + 10 + iHeight + iMarginY, 150, iHeight, TRUE);

	// DISCONNECT BUTTON
	pstrCtl = GetDlgItem(IDC_BUTTON_DISCONNECT);
	pstrCtl->MoveWindow(iMarginX + 150 + iMarginX, iMarginY + 39 + 10 + iHeight + iMarginY, 150, iHeight, TRUE);

	// LISTBOX 
	pstrCtl = GetDlgItem(IDC_LIST_CHAT);
	pstrCtl->MoveWindow(iMarginX, (iMarginY + 39 + 10 + iHeight + iMarginY) + (iHeight + iMarginY), (cx - iMarginX * 2), cy - (iMarginY + 39 + 10 + iHeight + 300));

	// SEND EDIT
	pstrCtl = GetDlgItem(IDC_EDIT_SEND);
	pstrCtl->MoveWindow(iMarginX, (iMarginY + 39 + 10 + iHeight + iMarginY) + (iHeight + iMarginY) + (cy - (iMarginY + 39 + 10 + iHeight + 300)) + iMarginY, cx - (iMarginX + iMarginX + iMarginX + 150), iHeight, TRUE);

	// SEND BUTTON
	pstrCtl = GetDlgItem(IDC_BUTTON_SEND);
	pstrCtl->MoveWindow(cx - (iMarginX + iMarginX + iMarginX + 150) + iMarginX + iMarginX, (iMarginY + 39 + 10 + iHeight + iMarginY) + (iHeight + iMarginY) + (cy - (iMarginY + 39 + 10 + iHeight + 300)) + iMarginY, 150, iHeight, TRUE);

	// CLOSE BUTTON
	pstrCtl = GetDlgItem(IDC_BUTTON_CLOSE);
	pstrCtl->MoveWindow((cx / 2) - (150 / 2), (iMarginY + 39 + 10 + iHeight + iMarginY) + (iHeight + iMarginY) + (cy - (iMarginY + 39 + 10 + iHeight + 300)) + iMarginY + iHeight + iMarginY, 150, iHeight, TRUE);

	/* 위치 정보 적용(컨트롤) - END */
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


//void CMFC19_SocketExDlg::OnClickedButtonClose()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	//AfxGetMainWnd()->PostMessageW(WM_CLOSE);
//}
//
//void CMFC19_SocketExDlg::SetConnectStatus(int iErrorCode)
//{
//	CString strMessage;
//
//	
//
//}
//
//// DISConnect
//void CMFC19_SocketExDlg::OnClickedButtonDisconnect()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CString strMessage;
//
//	
//
//}
