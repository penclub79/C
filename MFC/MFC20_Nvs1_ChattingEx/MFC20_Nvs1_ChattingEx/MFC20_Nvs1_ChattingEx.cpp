
// MFC20_Nvs1_ChattingEx.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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


// CMFC20_Nvs1_ChattingExApp 생성

CMFC20_Nvs1_ChattingExApp::CMFC20_Nvs1_ChattingExApp()
: m_pServer(NULL)
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CMFC20_Nvs1_ChattingExApp 개체입니다.

CMFC20_Nvs1_ChattingExApp theApp;


// CMFC20_Nvs1_ChattingExApp 초기화

BOOL CMFC20_Nvs1_ChattingExApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	CMFC20_Nvs1_ChattingExDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 응용 프로그램이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}

// 서버 생성
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
	// 클라이언트로 부터 받은 메시지
	pAcceptSock->Receive(&pBuffer[0], sizeof(PACKET_HEADER)); // 헤더사이즈를 버퍼에 Receive한다.
	pAcceptSock->Receive(&pBuffer[sizeof(PACKET_HEADER)], pstHeader->iPacketSize - sizeof(PACKET_HEADER));// 헤더사이즈 만큼 버퍼에서 건너뛰고 페이로드를 Receive한다.
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// Parsing //////////////////////////////////////////////////////
	// Parsing : 패킷 데이터를 열어 본다. ////////////////////////////
	if (MARKER_CLIENT == pstHeader->iMarker)
	{
		switch (pstHeader->iPacketID)
		{
			case PACKET_ID_REQ_LOGIN:
			{
				pstReceiveUserHeader = (PACKET_REQ_LOGIN*)pBuffer;

				// UserID 부여
				pAcceptSock->SetUserID(pstReceiveUserHeader->wszUserID);

				// UserHeader Send to Client
				// Code 
				stRSPLogin.stHeader.iMarker = MARKER_CLIENT;
				stRSPLogin.stHeader.iVersion = VERSION_PACKET_CLIENT_1;
				stRSPLogin.stHeader.iPacketID = PACKET_ID_RSP_LOGIN;
				stRSPLogin.stHeader.iPacketSize = sizeof(PACKET_RSP_LOGIN);
				// sizeof는 구조체를 넣는 걸로 규칙을 정함 -> 변수랑 포인터 변수랑 헤깔린다.
				stRSPLogin.iResultCode = LOGIN_SUCCESS;

				pAcceptSock->Send(&stRSPLogin, sizeof(PACKET_RSP_LOGIN));

				// 클라이언트 검증 후 방에 들어왔다는 것을 표시
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
					// pAccept소켓을 담아야한다.
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

	// 메모리 해제
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


