
// MFC19_SocketEx.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
// 클라이언트

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


// CMFC19_SocketExApp 생성

CMFC19_SocketExApp::CMFC19_SocketExApp()
: m_pClient(NULL)
, m_iConnectCode(0)
, m_strUserID("")

{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CMFC19_SocketExApp 개체입니다.

CMFC19_SocketExApp theApp;


// CMFC19_SocketExApp 초기화

BOOL CMFC19_SocketExApp::InitInstance()
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

	CMFC19_SocketExDlg dlg;
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


void CMFC19_SocketExApp::CleanUp()
{
	if (m_pClient)
	{
		delete m_pClient;
	}
}

// Connect 연결 (IP, Port)
void CMFC19_SocketExApp::Connect(CString strIP, int iPort)
{
	if (strIP && iPort)
	{
		m_pClient = new CBasicSock;
		m_pClient->Create();
		m_pClient->Connect(strIP, iPort);
		
	}
	
}

// Connect 끊기
void CMFC19_SocketExApp::Close()
{
	m_pClient->Close();
}

// Connect 성공/실패 여부 Error Code 리턴
int CMFC19_SocketExApp::GetConnectStatus()
{
	return m_iConnectCode;
}


void CMFC19_SocketExApp::SetConnectStatus(int iErrorCode)
{
	CString strConnectMessage;

	m_iConnectCode = iErrorCode;

	// connect는 성공했다면 다음으로 클라이언트 패킷을 체크
	if (0 == iErrorCode)
	{
		SendReqLoginPacket();
	}
	else  // connect가 실패면 연결 실패 박스를 띄움
	{
		((CMFC19_SocketExDlg*)AfxGetMainWnd())->SetConnectStatus(iErrorCode);
	}
}

void CMFC19_SocketExApp::SendReqLoginPacket()
{
	/*
	웬만하면 (TCHAR*)(LPCTSTR)m_strUserID 쓰지 않는다.
	CString은 클래스이므로 클래스는 다양한 함수와 변수들이 존재하는데.
	메모리 범위를 어떤것으로 잡을지 모른다. 하지만 CString에서만 제공하는 .GetBuffer() 함수 덕분에
	버퍼사이즈를 넘겨줄 수 있다.
	*/
	TCHAR *pstUserID = (TCHAR*)(LPCTSTR)m_strUserID.GetBuffer();
	

	// 헤더 패킷
	PACKET_REQ_LOGIN	stSendLoginHeader = { 0 };
	stSendLoginHeader.stHeader.iPacketID = PACKET_ID_REQ_LOGIN;
	stSendLoginHeader.stHeader.iMarker = MARKER_CLIENT;
	stSendLoginHeader.stHeader.iPacketSize = sizeof(PACKET_REQ_LOGIN);
	stSendLoginHeader.stHeader.iVersion = VERSION_PACKET_CLIENT_1;

	// REQ 로그인 패킷

	// wsprintf는 String의 데이터를 배열에 복사하기 위한 함수이다. 많이 사용함.
	wsprintf(stSendLoginHeader.wszUserID, _T("%s"), pstUserID);

	m_pClient->Send(&stSendLoginHeader, sizeof(PACKET_REQ_LOGIN));
}

void CMFC19_SocketExApp::SendReqMessagePacket(CString strData)
{
	
	//PACKET_HEADER_TEXT	stSendHeader = { 0 };
	//
	//stSendHeader.iPacketID = PACKET_ID_TEXT;

	//TCHAR *pstUserID = (TCHAR*)(LPCTSTR)m_strUserID.GetBuffer();

	//// wsprintf는 String의 데이터를 배열에 복사하기 위한 함수이다. 많이 사용함.
	//wsprintf(stSendHeader.wszPacketText, _T("%s"), strData.GetBuffer());
	////TRACE(_T("%s"), pstUserID);

	//////_tcscpy_s(stSendHeader.wszPacketText, sizeof(stSendHeader.wszPacketText), pstUserID);

	//m_pClient->Send(&stSendHeader, sizeof(stSendHeader));

}

void CMFC19_SocketExApp::SetUserID(CString strUserID)
{
	m_strUserID = strUserID;
}

// 서버에서 받은 데이터
void CMFC19_SocketExApp::ReceiveData()
{
	char*					pBuffer						= new char[1024];
	PACKET_HEADER*			pstHeader					= (PACKET_HEADER*)pBuffer;
	PACKET_RSP_LOGIN*		pstResponseLoginPacket		= NULL;	// 메모리주소 초기화
	PACKET_RSP_TEXT*		pstResponseTextPacket		= NULL;
	
	// 메모리 초기화: memset(초기화할 변수(포인터), Value, Size)
	memset(pBuffer, 0, 1024);

	
	// 헤더 부분
	m_pClient->Receive(pstHeader, sizeof(PACKET_HEADER));

	// 헤더 페이로드(?) 부분
	m_pClient->Receive(&pBuffer[sizeof(PACKET_HEADER)], pstHeader->iPacketSize - sizeof(PACKET_HEADER));

	// 받은 패킷 체크
	// 1. Header Marker Check
	if (MARKER_CLIENT == pstHeader->iMarker)
	{
		// 2. Packet ID Check
		switch (pstHeader->iPacketID)
		{
			case PACKET_ID_RSP_LOGIN:
			{
				((CMFC19_SocketExDlg*)AfxGetMainWnd())->SetConnectStatus(RSP_SUCCESS);
				break;
			}

			case PACKET_ID_RSP_TEXT:
			{
				break;
			}
		}
	}

	//// 버퍼 메모리 해제
	//if (pBuffer)
	//{
	//	delete[] pBuffer;
	//	pBuffer = NULL;
	//}

}


void CMFC19_SocketExApp::CloseChild()
{
	AfxMessageBox(_T("서버 연결 끊김"));
	
	delete m_pClient;
	m_pClient = NULL;
}
