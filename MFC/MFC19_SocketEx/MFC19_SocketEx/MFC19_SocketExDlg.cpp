
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
	, m_nChatMode(1)
	, m_strMyIP(_T(""))
	, m_nOtherIP(_T(""))
	, m_strOtherIP(_T(""))
	, m_strInitLoc(0)
	, m_rectDlg(0)
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
	//ON_BN_CLICKED(IDC_RADIO_SERVER, &CMFC19_SocketExDlg::OnClickedRadioServer)
	//ON_COMMAND(IDC_RADIO_CLIENT, &CMFC19_SocketExDlg::OnRadioClient)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMFC19_SocketExDlg::OnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMFC19_SocketExDlg::OnClickedButtonSend)
	//ON_BN_CLICKED(IDC_RADIO_SERVER, &CMFC19_SocketExDlg::OnBnClickedRadioServer)
	ON_WM_SIZE()
	ON_WM_SIZING()
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
	PHOSTENT hostinfo;
	if (gethostname(name, sizeof(name)) == 0)
	{
		if ((hostinfo = gethostbyname(name)) != NULL)
		{
			m_strMyIP = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
		}
	}

	// ��Ʈ�� �ʱ�ȭ
	m_IPAddress.SetWindowText(m_strMyIP);
	m_IPAddress.EnableWindow(FALSE);


	// Text Static ServerIP�ʱ� ��ǥ��������
	GetDlgItem(IDC_STATIC_SERVER_IP)->GetWindowRect(&m_strInitLoc);
	// ���̾�α� ���� ����� ��ǥ�� ��ȯ
	ScreenToClient(&m_strInitLoc);

	
	this->GetWindowRect(&m_rectDlg);


	//GetDlgItem(IDC_RADIO_SERVER)->EnableWindow(FALSE);
	SetDlgItemText(IDC_BUTTON_CONNECT, _T("Connect"));


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



//void CMFC19_SocketExDlg::OnClickedRadioServer()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	m_IPAddress.SetWindowText(m_strMyIP);
//	m_IPAddress.EnableWindow(FALSE);
//	SetDlgItemText(IDC_BUTTON_CONNECT, _T("Open"));
//}


//void CMFC19_SocketExDlg::OnRadioClient()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//	m_IPAddress.SetWindowText(_T(""));
//	m_IPAddress.EnableWindow(TRUE);
//	SetDlgItemText(IDC_BUTTON_CONNECT, _T("Connect"));
//}


void CMFC19_SocketExDlg::OnClickedButtonConnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();

	CString strIP;
	GetDlgItemText(IDC_IPADDRESS_SERVER, strIP);

	if (strIP != _T("0.0.0.0"))
	{
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
		((CMFC19_SocketExApp*)AfxGetApp())->Connect(strIP);
		m_strOtherIP = strIP;
	}
	else
	{
		AfxMessageBox(_T("������ ������ IP�ּҸ� �Է��ϼ���"));
	}
		
}


void CMFC19_SocketExDlg::OnClickedButtonSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strSend, strInsert;
	GetDlgItemText(IDC_EDIT_SEND, strSend);
	strInsert.Format(_T("Ŭ���̾�Ʈ[%s]:%s"), m_strMyIP, strSend);
	((CMFC19_SocketExApp*)AfxGetApp())->SendData(strSend);

	//int sel = m_listChat.InsertString(-1, strInsert);
	//m_listChat.SetCurSel(sel);
	SetDlgItemText(IDC_EDIT_SEND, _T(""));
}


void CMFC19_SocketExDlg::ReceiveData(CString strReceive)
{
	CString strInsert;
	strInsert.Format(_T("�ٸ�-Ŭ���̾�Ʈ[%s]:%s"), m_strOtherIP, strReceive);
	int sel = m_listChat.InsertString(-1, strInsert);
	m_listChat.SetCurSel(sel);
}


void CMFC19_SocketExDlg::Accept(CString strSock)
{
	m_strOtherIP = strSock;
}


void CMFC19_SocketExDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (NULL == this->GetSafeHwnd())
		return;

	/* ��ġ ���� ��������(��Ʈ��) - START */
	// Server IP STATIC
	CWnd* pstrCtl = GetDlgItem(IDC_STATIC_SERVER_IP);
	//TRACE(_T("left : %d, right : %d, top : %d, bottom : %d,", m_rectDlg.left, m_rectDlg.right, m_rectDlg.top, m_rectDlg.bottom));
	if (NULL == pstrCtl)
		return;

	// IPADDRESS Loc
	RECT stIpAdrEditLoc = { 0 };
	GetDlgItem(IDC_IPADDRESS_SERVER)->GetWindowRect(&stIpAdrEditLoc);
	ScreenToClient(&stIpAdrEditLoc);

	// PORT STATIC Loc
	RECT stPortStaticLoc = { 0 };
	GetDlgItem(IDC_STATIC_PORT)->GetWindowRect(&stPortStaticLoc);
	ScreenToClient(&stPortStaticLoc);

	// PORT EDIT Loc
	RECT stPortEditLoc = { 0 };
	GetDlgItem(IDC_EDIT_PORT)->GetWindowRect(&stPortEditLoc);
	ScreenToClient(&stPortEditLoc);

	// USER ID STATIC Loc
	RECT stUserIdStaticLoc = { 0 };
	GetDlgItem(IDC_STATIC_USERID)->GetWindowRect(&stUserIdStaticLoc);
	ScreenToClient(&stUserIdStaticLoc);

	// USER ID EDIT Loc
	RECT stUserIdEditLoc = { 0 };
	GetDlgItem(IDC_EDIT_USERID)->GetWindowRect(&stUserIdEditLoc);
	ScreenToClient(&stUserIdEditLoc);

	// CONNECT BUTTON Loc
	RECT stConnectButtonLoc = { 0 };
	GetDlgItem(IDC_BUTTON_CONNECT)->GetWindowRect(&stConnectButtonLoc);
	ScreenToClient(&stConnectButtonLoc);

	// LIST BOX Loc
	RECT stListBoxLoc = { 0 };
	GetDlgItem(IDC_LIST_CHAT)->GetWindowRect(&stListBoxLoc);
	ScreenToClient(&stListBoxLoc);

	// SEND EDIT Loc
	RECT stSendEditLoc = { 0 };
	GetDlgItem(IDC_EDIT_SEND)->GetWindowRect(&stSendEditLoc);
	ScreenToClient(&stSendEditLoc);

	// SEND BUTTON Loc
	RECT stSendButtonLoc = { 0 };
	GetDlgItem(IDC_BUTTON_SEND)->GetWindowRect(&stSendButtonLoc);
	ScreenToClient(&stSendButtonLoc);
	/* ��ġ ���� ��������(��Ʈ��) - END */
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* ��ġ ���� ����(��Ʈ��) - START */
	// Server IP STATIC
	pstrCtl->MoveWindow(m_strInitLoc.left, m_strInitLoc.top, 100, 39, TRUE);
	
	// IPADDRESS EDIT
	pstrCtl = GetDlgItem(IDC_IPADDRESS_SERVER);
	pstrCtl->MoveWindow(stIpAdrEditLoc.left, stIpAdrEditLoc.top, (cx / 3), 39, TRUE);

	// PORT STATIC
	pstrCtl = GetDlgItem(IDC_STATIC_PORT);
	pstrCtl->MoveWindow(stIpAdrEditLoc.right + 10, stPortStaticLoc.top, 90, 39, TRUE);

	// PORT EDIT
	pstrCtl = GetDlgItem(IDC_EDIT_PORT);
	//pstrCtl->MoveWindow((stPortStaticLoc.right + 15) + m_stWindowLoc.left, 45, 100, 39, TRUE);
	//pstrCtl->MoveWindow(cx - (stPortStaticLoc.left + stPortStaticLoc.right + 15), 45, 100, 39, TRUE);
	//pstrCtl->MoveWindow(stPortStaticLoc.right + 15, stIpAdrEditLoc.top, (m_rectDlg.left) + stPortEditLoc.right - stPortEditLoc.left, 39, TRUE);
	pstrCtl->MoveWindow(stPortStaticLoc.right + 15, stIpAdrEditLoc.top, cx / 4 , 39, TRUE);

	// USER ID STATIC
	pstrCtl = GetDlgItem(IDC_STATIC_USERID);
	pstrCtl->MoveWindow(stUserIdStaticLoc.left, stUserIdStaticLoc.top, 100, 39, TRUE);

	// USER ID EDIT
	pstrCtl = GetDlgItem(IDC_EDIT_USERID);
	pstrCtl->MoveWindow(stUserIdEditLoc.left, stUserIdEditLoc.top, cx / 4, 39, TRUE);
	
	// CONNECT BUTTON
	pstrCtl = GetDlgItem(IDC_BUTTON_CONNECT);
	pstrCtl->MoveWindow(stUserIdEditLoc.right + 150, stConnectButtonLoc.top, stConnectButtonLoc.right - stConnectButtonLoc.left, stConnectButtonLoc.bottom - stConnectButtonLoc.top);
	
	// LISTBOX 
	pstrCtl = GetDlgItem(IDC_LIST_CHAT);
	pstrCtl->MoveWindow(stListBoxLoc.left, stListBoxLoc.top, cx / 1.1, cy / 1.9);

	// SEND EDIT
	pstrCtl = GetDlgItem(IDC_EDIT_SEND);
	pstrCtl->MoveWindow(stSendEditLoc.left, stListBoxLoc.bottom + 20, cx / 1.5, stSendEditLoc.bottom - stSendEditLoc.top);

	// SEND BUTTON
	pstrCtl = GetDlgItem(IDC_BUTTON_SEND);
	pstrCtl->MoveWindow(stSendEditLoc.right + 20, stListBoxLoc.bottom + 20, stSendButtonLoc.right - stSendButtonLoc.left, stSendButtonLoc.bottom - stSendButtonLoc.top);

	/* ��ġ ���� ����(��Ʈ��) - END */
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

}


void CMFC19_SocketExDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
