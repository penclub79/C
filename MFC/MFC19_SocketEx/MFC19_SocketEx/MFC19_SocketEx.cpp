
// MFC19_SocketEx.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
// Ŭ���̾�Ʈ

#include "stdafx.h"
#include "MFC19_SocketEx.h"
#include "MFC19_SocketExDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC19_SocketExApp

BEGIN_MESSAGE_MAP(CMFC19_SocketExApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFC19_SocketExApp ����

CMFC19_SocketExApp::CMFC19_SocketExApp()
: m_pClient(NULL)
, m_iConnectCode(0)
, m_strUserID("")

{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CMFC19_SocketExApp ��ü�Դϴ�.

CMFC19_SocketExApp theApp;


// CMFC19_SocketExApp �ʱ�ȭ

BOOL CMFC19_SocketExApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�.
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// ��ȭ ���ڿ� �� Ʈ�� �� �Ǵ�
	// �� ��� �� ��Ʈ���� ���ԵǾ� �ִ� ��� �� �����ڸ� ����ϴ�.
	CShellManager *pShellManager = new CShellManager;

	// MFC ��Ʈ���� �׸��� ����ϱ� ���� "Windows ����" ���־� ������ Ȱ��ȭ
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	CMFC19_SocketExDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ���⿡ [Ȯ��]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ���⿡ [���]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "���: ��ȭ ���ڸ� ������ �������Ƿ� ���� ���α׷��� ����ġ �ʰ� ����˴ϴ�.\n");
		TRACE(traceAppMsg, 0, "���: ��ȭ ���ڿ��� MFC ��Ʈ���� ����ϴ� ��� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS�� ������ �� �����ϴ�.\n");
	}

	// ������ ���� �� �����ڸ� �����մϴ�.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�  ���� ���α׷��� ���� �� �ֵ��� FALSE��
	// ��ȯ�մϴ�.

	return FALSE;
}


void CMFC19_SocketExApp::CleanUp()
{
	if (m_pClient)
	{
		delete m_pClient;
	}
}

// Connect ���� (IP, Port)
void CMFC19_SocketExApp::Connect(CString strIP, int iPort)
{
	if (strIP && iPort)
	{
		m_pClient = new CBasicSock;
		m_pClient->Create();
		m_pClient->Connect(strIP, iPort);
		
	}
	
}

// Connect ����
void CMFC19_SocketExApp::Close()
{
	m_pClient->Close();
}

// Connect ����/���� ���� Error Code ����
int CMFC19_SocketExApp::GetConnectStatus()
{
	return m_iConnectCode;
}


void CMFC19_SocketExApp::SetConnectStatus(int iErrorCode)
{
	CString strConnectMessage;

	m_iConnectCode = iErrorCode;

	// connect�� �����ߴٸ� �������� Ŭ���̾�Ʈ ��Ŷ�� üũ
	if (0 == iErrorCode)
	{
		SendIAm();
	}
	else  // connect�� ���и� ���� ���� �ڽ��� ���
	{
		((CMFC19_SocketExDlg*)AfxGetMainWnd())->SetConnectStatus(iErrorCode);
	}
}

void CMFC19_SocketExApp::SendIAm()
{
	PACKET_HEADER_IAM	stSendHeader = { 0 };
	stSendHeader.iPacketID = PACKET_ID_IAM;

	/*
	�����ϸ� (TCHAR*)(LPCTSTR)m_strUserID ���� �ʴ´�.
	CString�� Ŭ�����̹Ƿ� Ŭ������ �پ��� �Լ��� �������� �����ϴµ�.
	�޸� ������ ������� ������ �𸥴�. ������ CString������ �����ϴ� .GetBuffer() �Լ� ���п�
	���ۻ���� �Ѱ��� �� �ִ�.
	*/
	TCHAR *pstUserID = (TCHAR*)(LPCTSTR)m_strUserID.GetBuffer();

	// wsprintf�� String�� �����͸� �迭�� �����ϱ� ���� �Լ��̴�. ���� �����.
	wsprintf(stSendHeader.wszUserID, _T("%s"), pstUserID);
	TRACE(_T("%s"), pstUserID);

	//_tcscpy_s(stSendHeader.wszPacketText, sizeof(stSendHeader.wszPacketText), pstUserID);

	m_pClient->Send(&stSendHeader, sizeof(stSendHeader));
}


void CMFC19_SocketExApp::SetUserID(CString strUserID)
{
	m_strUserID = strUserID;
}

// �������� ���� ������
void CMFC19_SocketExApp::ReceiveData()
{
	PACKET_HEADER_IAM	stRecvHeader = { 0 };

	m_pClient->Receive(&stRecvHeader, sizeof(stRecvHeader));

	// ��û ��Ŷ�� ���� ����� ��ŶID�� ������
	if (PACKET_ID_IAM == stRecvHeader.iPacketID)
	{
		//((CMFC19_SocketExDlg*)m_pMainWnd)->GetUserID();
		((CMFC19_SocketExDlg*)AfxGetMainWnd())->SetConnectStatus(0);	
	}
}


void CMFC19_SocketExApp::SendText(CString strData)
{	
	PACKET_HEADER_TEXT	stSendHeader = { 0 };
	
	stSendHeader.iPacketID = PACKET_ID_TEXT;

	TCHAR *pstUserID = (TCHAR*)(LPCTSTR)m_strUserID.GetBuffer();

	// wsprintf�� String�� �����͸� �迭�� �����ϱ� ���� �Լ��̴�. ���� �����.
	wsprintf(stSendHeader.wszPacketText, _T("%s"), strData.GetBuffer());
	//TRACE(_T("%s"), pstUserID);


	////_tcscpy_s(stSendHeader.wszPacketText, sizeof(stSendHeader.wszPacketText), pstUserID);

	m_pClient->Send(&stSendHeader, sizeof(stSendHeader));

}


void CMFC19_SocketExApp::CloseChild()
{
	AfxMessageBox(_T("���� ���� ����"));
	
	delete m_pClient;
	m_pClient = NULL;
}
