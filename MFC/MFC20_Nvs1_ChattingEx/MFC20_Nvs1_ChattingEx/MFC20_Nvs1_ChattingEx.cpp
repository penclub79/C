
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
	CAcceptSock* pAccept = new CAcceptSock;
	
	if (m_pServer->Accept(*pAccept))
	{
		CString strSock;
		UINT nPort;
		pAccept->GetPeerName(strSock, nPort);
		
		m_AcceptSocketList.AddTail(pAccept);

		((CMFC20_Nvs1_ChattingExDlg*)m_pMainWnd)->Accept(strSock);

	}
	

#endif
}

void CMFC20_Nvs1_ChattingExApp::CleanUp()
{
	if (m_pServer)
	{
		delete m_pServer;
		m_pServer = NULL;
	}
	CAcceptSock* pAccept;
	POSITION pos = m_AcceptSocketList.GetHeadPosition();
	while (pos != NULL)
	{
		pAccept = (CAcceptSock*)m_AcceptSocketList.GetAt(pos);
		m_AcceptSocketList.GetNext(pos);
		delete pAccept;
		pAccept = NULL;
	}
	m_AcceptSocketList.RemoveAll();
}

void CMFC20_Nvs1_ChattingExApp::ReceiveData(CAcceptSock* pAcceptSock)
{
	char*					pBuffer					= new char[1024];
	PACKET_HEADER*			pstHeader				= (PACKET_HEADER*)pBuffer;
	PACKET_REQ_LOGIN*		pstReceiveUserHeader	= NULL;
	PACKET_REQ_TEXT*		pstReceiveTextHeader	= NULL;
	

	PACKET_RSP_TEXT			stRSPText				= { 0 };
	PACKET_RSP_LOGIN		stRSPLogin				= { 0 };
	PACKET_RSP_KEEPALIVE	stRSPKeepAlive			= { 0 };

	memset(pBuffer, 0, 1024);

	/////////////////////////////////////////////////////////////////
	// Receive //////////////////////////////////////////////////////
	// wchar_t temp[MAX_PATH];
	// Ŭ���̾�Ʈ�� ���� ���� �޽���
	pAcceptSock->Receive(&pBuffer[0], sizeof(PACKET_HEADER)); // �������� ���ۿ� Receive�Ѵ�.
	pAcceptSock->Receive(&pBuffer[sizeof(PACKET_HEADER)], pstHeader->iPacketSize - sizeof(PACKET_HEADER));// ��������� ��ŭ ���ۿ��� �ǳʶٰ� ���̷ε带 Receive�Ѵ�.
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// Parsing //////////////////////////////////////////////////////
	// Parsing : ��Ŷ �����͸� ���� ����. ////////////////////////////
	if (MARKER_CLIENT == pstHeader->iMarker)
	{
		switch (pstHeader->iPacketID)
		{
			case PACKET_ID_REQ_LOGIN:
			{
				pstReceiveUserHeader = (PACKET_REQ_LOGIN*)pBuffer;

				// UserID �ο�
				pAcceptSock->SetUserID(pstReceiveUserHeader->wszUserID);

				// UserHeader Send to Client
				// Code 
				stRSPLogin.stHeader.iMarker = MARKER_CLIENT;
				stRSPLogin.stHeader.iVersion = VERSION_PACKET_CLIENT_1;
				stRSPLogin.stHeader.iPacketID = PACKET_ID_RSP_LOGIN;
				stRSPLogin.stHeader.iPacketSize = sizeof(PACKET_RSP_LOGIN);
				// sizeof�� ����ü�� �ִ� �ɷ� ��Ģ�� ���� -> ������ ������ ������ ��򸰴�.
				stRSPLogin.iResultCode = LOGIN_SUCCESS;

				pAcceptSock->Send(&stRSPLogin, sizeof(PACKET_RSP_LOGIN));

				// Ŭ���̾�Ʈ ���� �� �濡 ���Դٴ� ���� ǥ��
				((CMFC20_Nvs1_ChattingExDlg*)AfxGetMainWnd())->EntryClient(pAcceptSock);
				//((CMFC20_Nvs1_ChattingExDlg*)m_pMainWnd)->ReceiveData(pClientSock, stRSPLogin.wszPacketText);

				break;
			}

			case PACKET_ID_REQ_TEXT:
			{
				POSITION pos;
				CString strOtherUserID;
				strOtherUserID = pAcceptSock->GetUserID();
				pstReceiveTextHeader = (PACKET_REQ_TEXT*)pBuffer;
				
				stRSPText.stHeader.iMarker = MARKER_CLIENT;
				stRSPText.stHeader.iVersion = VERSION_PACKET_CLIENT_1;
				stRSPText.stHeader.iPacketID = PACKET_ID_RSP_TEXT;
				stRSPText.stHeader.iPacketSize = sizeof(PACKET_RSP_TEXT);
				stRSPText.iResultCode = TEXT_SUCCESS;
				wsprintf(stRSPText.wszPacketText, _T("%s"), pstReceiveTextHeader->wszPacketText);
				wsprintf(stRSPText.wszSendUserID, _T("%s"), strOtherUserID);

				//wsprintf(stRSPText.wszSendUserID, _T("%s"), ((CAcceptSock*)AfxGetMainWnd())->GetUserID());
				
				pos = m_AcceptSocketList.GetHeadPosition();
				CAcceptSock* pAcceptSockTemp = NULL;
				while (pos != NULL)
				{
					// pAccept������ ��ƾ��Ѵ�.
					pAcceptSockTemp = (CAcceptSock*)m_AcceptSocketList.GetAt(pos);
					
					if (pAcceptSockTemp != pAcceptSock)
						pAcceptSockTemp->Send(&stRSPText, sizeof(PACKET_RSP_TEXT));

					(CAcceptSock*)m_AcceptSocketList.GetNext(pos);
				}
				break;
			}

			case PACKET_ID_REQ_ALIVE:
			{
				stRSPKeepAlive.stHeader.iMarker			= MARKER_CLIENT;
				stRSPKeepAlive.stHeader.iVersion		= VERSION_PACKET_CLIENT_1;
				stRSPKeepAlive.stHeader.iPacketID		= PACKET_ID_RSP_ALIVE;
				stRSPKeepAlive.stHeader.iPacketSize		= sizeof(PACKET_RSP_KEEPALIVE);
				
				pAcceptSock->Send(&stRSPKeepAlive, sizeof(PACKET_RSP_KEEPALIVE));

				TRACE(_T("Send PACKET_RSP_KEEPALIVE \n"));
				break;
			}
			
			default:
				break;
		}
	}

	// �޸� ����
	if (pBuffer)
	{
		delete [] pBuffer;
		pBuffer = NULL;
	}
}

void CMFC20_Nvs1_ChattingExApp::CloseChild(CAcceptSock* pClientSock)
{
	POSITION pos = m_AcceptSocketList.Find(pClientSock);

	if (pos != NULL)
	{
		((CMFC20_Nvs1_ChattingExDlg*)AfxGetMainWnd())->onClose(pClientSock);
		pClientSock = (CAcceptSock*)m_AcceptSocketList.GetAt(pos);
		m_AcceptSocketList.RemoveAt(pos);
		delete pClientSock;
		pClientSock = NULL;
	}
}


