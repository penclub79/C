
// MFC19_SocketExDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MFC19_SocketEx.h"
#include "MFC19_SocketExDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CMFC19_SocketExDlg ��ȭ ����



CMFC19_SocketExDlg::CMFC19_SocketExDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CMFC19_SocketExDlg::IDD, pParent)
// ������� �ʱ�ȭ
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


// CMFC19_SocketExDlg �޽��� ó����

BOOL CMFC19_SocketExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// IP�ּ� ��������
	char name[255];
	PHOSTENT stHostinfo;
	if (gethostname(name, sizeof(name)) == 0)
	{
		if ((stHostinfo = gethostbyname(name)) != NULL)
		{
			m_strMyIP = inet_ntoa(*(struct in_addr*)*stHostinfo->h_addr_list);
		}
	}

	/*��Ʈ�� �ʱ�ȭ*/
	m_IPAddress.SetWindowText(m_strMyIP);
	m_IPAddress.EnableWindow(FALSE);
	// ��Ʈ ���� ��Ȱ��ȭ
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
	// DisConnect ��Ȱ��ȭ
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
	// Send ��Ȱ��ȭ
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	// Connect Ȱ��ȭ
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);

	// Text Static ServerIP�ʱ� ��ǥ��������
	GetDlgItem(IDC_STATIC_SERVER_IP)->GetWindowRect(&m_strInitLoc);
	// ���̾�α� ���� ����� ��ǥ�� ��ȯ
	ScreenToClient(&m_strInitLoc);
	
	CLIENT_INFO stClient = { 0 };
	stClient.iPort = 7777;
	// ��Ʈ ����
	SetDlgItemInt(IDC_EDIT_PORT, stClient.iPort);

	//GetDlgItem(IDC_RADIO_SERVER)->EnableWindow(FALSE);
	SetDlgItemText(IDC_BUTTON_CONNECT, _T("Connect"));

	/*���̾�α� ������ �ʱ�ȭ*/
	RECT stDlgLoc = { 0 };
	GetClientRect(&stDlgLoc);

	ResizeControl((stDlgLoc.right - stDlgLoc.left) - 2, (stDlgLoc.bottom - stDlgLoc.top) + 50);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMFC19_SocketExDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMFC19_SocketExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFC19_SocketExDlg::OnClickedButtonConnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

// �޽��� Send ��ư
void CMFC19_SocketExDlg::OnClickedButtonSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	ResizeControl(cx, cy);
}


void CMFC19_SocketExDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	
}

void CMFC19_SocketExDlg::ResizeControl(int cx, int cy)
{
	if (NULL == this->GetSafeHwnd())
		return;

	/* ��ġ ���� ��������(��Ʈ��) - START */
	// Server IP STATIC
	CWnd* pstrCtl = GetDlgItem(IDC_STATIC_SERVER_IP);
	if (NULL == pstrCtl)
		return;

	int iMarginX = 20;
	int iMarginY = 10;
	int iHeight = 40;

	/* ��ġ ���� ����(��Ʈ��) - START */
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

	/* ��ġ ���� ����(��Ʈ��) - END */
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


//void CMFC19_SocketExDlg::OnClickedButtonClose()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	CString strMessage;
//
//	
//
//}
