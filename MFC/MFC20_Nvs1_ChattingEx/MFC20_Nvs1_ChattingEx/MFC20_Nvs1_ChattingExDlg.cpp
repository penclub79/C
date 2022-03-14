
// MFC20_Nvs1_ChattingExDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MFC20_Nvs1_ChattingEx.h"
#include "MFC20_Nvs1_ChattingExDlg.h"
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


// CMFC20_Nvs1_ChattingExDlg ��ȭ ����



CMFC20_Nvs1_ChattingExDlg::CMFC20_Nvs1_ChattingExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC20_Nvs1_ChattingExDlg::IDD, pParent)
	/*, m_nChat(0)
	, m_nChatMode(0)*/
	, m_nChatMode(0)
	, m_strMyIP(_T(""))
	, m_strOtherIP(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC20_Nvs1_ChattingExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_IPAddress(pDX, IDC_IPADDRESS_SERVER, m_nChat);
	//  DDX_IPAddress(pDX, IDC_IPADDRESS_SERVER, m_nChatMode);
	DDX_Control(pDX, IDC_LIST_CHAT, m_listChat);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_IPAddress);
	DDX_Radio(pDX, IDC_RADIO_SERVER, m_nChatMode);
}

BEGIN_MESSAGE_MAP(CMFC20_Nvs1_ChattingExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO_SERVER, &CMFC20_Nvs1_ChattingExDlg::OnClickedRadioServer)
	//ON_COMMAND(IDC_RADIO_CLIENT, &CMFC20_Nvs1_ChattingExDlg::OnRadioClient)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMFC20_Nvs1_ChattingExDlg::OnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMFC20_Nvs1_ChattingExDlg::OnClickedButtonSend)
END_MESSAGE_MAP()


// CMFC20_Nvs1_ChattingExDlg �޽��� ó����

BOOL CMFC20_Nvs1_ChattingExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ���� ������ �־�� �մϴ�.
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
	char szName[255];
	PHOSTENT stHostinfo;
	if (gethostname(szName, sizeof(szName)) == 0)
	{
		if ((stHostinfo = gethostbyname(szName)) != NULL)
		{
			m_strMyIP = inet_ntoa (*(struct in_addr *)*stHostinfo->h_addr_list);
		}
	}

	// ��Ʈ�� �ʱ�ȭ
	m_IPAddress.SetWindowTextW(m_strMyIP);	// ipaddress ���ڿ� ip ���� �־��ش�.
	m_IPAddress.EnableWindow(FALSE);  // ipaddress ���ڸ� ��Ȱ��ȭ ��Ų��.
	GetDlgItem(IDC_RADIO_SERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_CLIENT)->EnableWindow(FALSE);
	SetDlgItemText(IDC_BUTTON_CONNECT, _T("Open"));


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CMFC20_Nvs1_ChattingExDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC20_Nvs1_ChattingExDlg::OnPaint()
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
HCURSOR CMFC20_Nvs1_ChattingExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC20_Nvs1_ChattingExDlg::OnClickedRadioServer()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_IPAddress.SetWindowTextW(m_strMyIP);
	m_IPAddress.EnableWindow(FALSE);
	SetDlgItemText(IDC_BUTTON_CONNECT, _T("Open"));
}


void CMFC20_Nvs1_ChattingExDlg::OnRadioClient()
{
	// TODO: ���⿡ ���� ó���� �ڵ带 �߰��մϴ�.
	m_IPAddress.SetWindowTextW(_T(""));
	m_IPAddress.EnableWindow(TRUE);
	SetDlgItemText(IDC_BUTTON_CONNECT, _T("Connect"));
}


void CMFC20_Nvs1_ChattingExDlg::OnClickedButtonConnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
	((CMFC20_Nvs1_ChattingExApp*)AfxGetApp())->InitServer();
	GetDlgItem(IDC_RADIO_SERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_CLIENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	
	CString strIP;
	GetDlgItemText(IDC_IPADDRESS_SERVER, strIP);
	
}


void CMFC20_Nvs1_ChattingExDlg::OnClickedButtonSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strSend, strInsert;
	GetDlgItemText(IDC_EDIT_SEND, strSend);
	strInsert.Format(_T("����[%s]:%s"), m_strMyIP, strSend);
	theApp.SendData(strSend);
	int sel = m_listChat.InsertString(-1, strInsert);
	m_listChat.SetCurSel(sel);
	SetDlgItemText(IDC_EDIT_SEND, _T(""));
}


void CMFC20_Nvs1_ChattingExDlg::ReceiveData(CString strReceive)
{
	CString strInsert;
	strInsert.Format(_T("Ŭ���̾�Ʈ[%s]:%s"), m_strOtherIP, strReceive);
	int sel = m_listChat.InsertString(-1, strInsert);
}


void CMFC20_Nvs1_ChattingExDlg::Accept(CString strSock)
{
	m_strOtherIP = strSock;
}