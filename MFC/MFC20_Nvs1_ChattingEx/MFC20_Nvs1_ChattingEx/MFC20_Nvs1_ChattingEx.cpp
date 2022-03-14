
// MFC20_Nvs1_ChattingEx.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "MFC20_Nvs1_ChattingEx.h"
#include "MFC20_Nvs1_ChattingExDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC20_Nvs1_ChattingExApp

BEGIN_MESSAGE_MAP(CMFC20_Nvs1_ChattingExApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFC20_Nvs1_ChattingExApp ����

CMFC20_Nvs1_ChattingExApp::CMFC20_Nvs1_ChattingExApp()
: m_pServer(NULL)
, m_pAccept(NULL)
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CMFC20_Nvs1_ChattingExApp ��ü�Դϴ�.

CMFC20_Nvs1_ChattingExApp theApp;


// CMFC20_Nvs1_ChattingExApp �ʱ�ȭ

BOOL CMFC20_Nvs1_ChattingExApp::InitInstance()
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

	CMFC20_Nvs1_ChattingExDlg dlg;
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


// ���� ����
void CMFC20_Nvs1_ChattingExApp::InitServer()
{
	m_pServer = new CServerSock;
	m_pServer->Create(7777);
	m_pServer->Listen();
}


void CMFC20_Nvs1_ChattingExApp::Accept()
{

#if (0)

	if (m_pAccept == NULL)
	{
		m_pAccept = new CClientSock;
		m_pServer->Accept(*m_pAccept);
		CString strSock;
		UINT nPort;
		m_pAccept->GetPeerName(strSock, nPort);
		((CMFC20_Nvs1_ChattingExDlg*)m_pMainWnd)->Accept(strSock);
	}

#else

	m_pAccept = new CBasicSock;
	if (m_pServer->Accept(*m_pAccept))
	{
		CString strSock;
		UINT nPort;
		m_pAccept->GetPeerName(strSock, nPort);

		m_ClientList.AddTail(m_pAccept);
		((CMFC20_Nvs1_ChattingExDlg*)m_pMainWnd)->Accept(strSock);
	}

#endif

}


void CMFC20_Nvs1_ChattingExApp::CleanUp()
{
	if (m_pServer)
	{
		delete m_pServer;
	}
	if (m_pAccept)
	{
		delete m_pAccept;
	}
}


void CMFC20_Nvs1_ChattingExApp::ReceiveData(CBasicSock* pClientSock)
{
	wchar_t temp[MAX_PATH];
	POSITION pos = m_ClientList.GetHeadPosition();
	while (pos != NULL)
	{
		pClientSock = (CBasicSock*)m_ClientList.GetAt(pos);
		m_ClientList.GetNext(pos);
	}
	pClientSock->Receive(temp, sizeof(temp));
	((CMFC20_Nvs1_ChattingExDlg*)m_pMainWnd)->ReceiveData(temp);


}


void CMFC20_Nvs1_ChattingExApp::SendData(CString strData)
{
	if (m_pAccept)
	{
		m_pAccept->Send((LPCTSTR)strData, sizeof(TCHAR)*(strData.GetLength() + 1));
	}
}


void CMFC20_Nvs1_ChattingExApp::CloseChild(CBasicSock* pClientSock)
{
	/*CClientSock* pClient;
	POSITION pos = m_ClientList.Find(pSock);
	if (pos != NULL)
	{
		pClient = (CClientSock*)m_ClientList.GetAt(pos);
		m_ClientList.RemoveAt(pos);
		delete pClient;
	}*/
	/*delete m_pClient;
	m_pClient = NULL;*/

	POSITION pos = m_ClientList.Find(pClientSock);
	if (pos != NULL)
	{
		AfxMessageBox(_T("Ŭ���̾�Ʈ ���� ����"));
		pClientSock = (CBasicSock*)m_ClientList.GetAt(pos);
		m_ClientList.RemoveAt(pos);
		delete pClientSock;
		pClientSock = NULL;
	}
	
}