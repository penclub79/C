
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


// ������
CMFC19_SocketExDlg::CMFC19_SocketExDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CMFC19_SocketExDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ������� �ʱ�ȭ
	m_strInitLoc	= 0;
	m_rectDlg		= 0;
	m_iPort			= 0;
	m_bIsConnect	= FALSE;
}

// �Ҹ���
CMFC19_SocketExDlg::~CMFC19_SocketExDlg()
{
	if (NULL != m_pClient)
	{
		delete m_pClient;
		m_pClient = NULL;
	}
}

void CMFC19_SocketExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_ctlIPAddress);
	DDX_Control(pDX, IDC_LIST_CHAT, m_ctlListChat);
	
}

BEGIN_MESSAGE_MAP(CMFC19_SocketExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMFC19_SocketExDlg::OnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMFC19_SocketExDlg::OnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CMFC19_SocketExDlg::OnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CMFC19_SocketExDlg::OnClickedButtonDisconnect)
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
	PHOSTENT stHostinfo;
	if (gethostname(name, sizeof(name)) == 0)
	{
		if ((stHostinfo = gethostbyname(name)) != NULL)
		{
			m_strMyIP = inet_ntoa(*(struct in_addr*)*stHostinfo->h_addr_list);
		}
	}

	/*��Ʈ�� �ʱ�ȭ*/
	m_ctlIPAddress.SetWindowText(m_strMyIP);

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
	
	m_iPort = 7777;

	// ��Ʈ ����
	SetDlgItemInt(IDC_EDIT_PORT, m_iPort);

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
	else if (nID == SC_CLOSE)
	{
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		if (m_pClient)
			m_pClient->Close();
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

	if (NULL == m_pClient)
	{
		m_pClient = new CBasicSock(this->GetSafeHwnd());
		CString strUserID;
		CString strIP;
		BOOL bResult = 0;
		TCHAR* pszIPBuff = NULL;
		TCHAR* pszUserIdBuff = NULL;

		m_iPort = 7777;
		GetDlgItemText(IDC_EDIT_USERID, strUserID);
		GetDlgItemText(IDC_IPADDRESS_SERVER, strIP);

		SetUserID(strUserID);

		pszIPBuff = new TCHAR[strIP.GetLength() + 1];
		memset(pszIPBuff, 0, strIP.GetLength());
		_tcscpy(pszIPBuff, strIP.GetBuffer(0));

		pszUserIdBuff = new TCHAR[strUserID.GetLength() + 1];
		memset(pszUserIdBuff, 0, strUserID.GetLength());
		_tcscpy(pszUserIdBuff, strUserID.GetBuffer(0));

		if (NULL != m_pClient)
			m_pClient->Connect(pszIPBuff, pszUserIdBuff, m_iPort);

		if (NULL != pszIPBuff)
		{
			delete[] pszIPBuff;
			pszIPBuff = NULL;
		}

		if (NULL != pszUserIdBuff)
		{
			delete[] pszUserIdBuff;
			pszUserIdBuff = NULL;
		}
	}
}

void CMFC19_SocketExDlg::ConnectStatus(int _iResult)
{
	if (LOGIN_SUCCESS == _iResult)
	{
		//AfxMessageBox(_T("���� �Ϸ�."));
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_IPADDRESS_SERVER)->EnableWindow(FALSE);
	}
	else if (DISCONNECT == _iResult)
	{
		AfxMessageBox(_T("������ ������ϴ�."));

		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_IPADDRESS_SERVER)->EnableWindow(TRUE);
	}
	else
	{
		m_bIsConnect = FALSE;
		AfxMessageBox(_T("������ ��Ȱ���� ����."));
	}
		
}

// �޽��� Send ��ư
void CMFC19_SocketExDlg::OnClickedButtonSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strMessage;
	CString	strInsert;
	CString strUserID;
	int		sel			= 0;
	int		iLength		= 0;
	TCHAR*	pszBuff		= NULL;

	GetDlgItemText(IDC_EDIT_SEND, strMessage);
	iLength = strMessage.GetLength();

	pszBuff = new TCHAR[iLength + 1];
	memset(pszBuff, 0, iLength);
	_tcscpy(pszBuff, strMessage.GetBuffer(0));

	m_pClient->SendPacket(PACKET_ID_REQ_TEXT, pszBuff, iLength);

	strUserID = GetUserID();
	strInsert.Format(_T("Ŭ���̾�Ʈ[%s]:%s"), strUserID, strMessage);
	sel = m_ctlListChat.InsertString(-1, strInsert);
	m_ctlListChat.SetCurSel(sel);

	SetDlgItemText(IDC_EDIT_SEND, _T(""));

	if (NULL != pszBuff)
	{
		delete[] pszBuff;
		pszBuff = NULL;
	}
}

void CMFC19_SocketExDlg::ReceiveMessage(CString strReceive, CString strOtherUserID)
{
	CString strInsert;
	int sel = 0;
	int iLength = 0;

	strInsert.Format(_T("Ŭ���̾�Ʈ[%s]:%s"), strOtherUserID, strReceive);
	sel = m_ctlListChat.InsertString(-1, strInsert);
	m_ctlListChat.SetCurSel(sel);
}

void CMFC19_SocketExDlg::SetUserID(CString _strUserID)
{
	m_strUserID = _strUserID;
}

CString CMFC19_SocketExDlg::GetUserID()
{
	return m_strUserID;
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


void CMFC19_SocketExDlg::OnClickedButtonClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
	if (m_pClient )
		m_pClient->Close();
}

// DISConnect
void CMFC19_SocketExDlg::OnClickedButtonDisconnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strMessage;
	if (m_pClient)
		m_pClient->DisConnect();
}

// �����͸� ������ WindowProc���� ó���Ѵ�.
LRESULT CMFC19_SocketExDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	PACKET_RSP_TEXT* pstRspText = NULL;

	switch (message)
	{
	case WM_MESSAGE_SOCKET:
		if (LOGIN_SUCCESS == wParam)
		{
			m_bIsConnect = TRUE;
			ConnectStatus(wParam);
		}
		else
		{
			m_bIsConnect = FALSE;
			ConnectStatus(wParam);
			
			if (NULL != m_pClient)
			{
				delete m_pClient;
				m_pClient = NULL;
			}
		}
		break;

	case WM_USER:
		if (TEXT_SUCCESS == wParam)
		{
			pstRspText = (PACKET_RSP_TEXT*)lParam;
			ReceiveMessage(pstRspText->wszPacketText, pstRspText->wszSendUserID);
		}
		break;
	}
	
	return CDialogEx::WindowProc(message, wParam, lParam);
}
