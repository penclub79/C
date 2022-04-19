#include "stdafx.h"
#include "BasicSock.h"



CBasicSock::CBasicSock(HWND hParent)
{
	m_dwThreadID = 0;
	m_hThread = 0;
	m_iPort = 0;
	//m_pszBuff = NULL;
	m_hParentHandle = hParent;
	
}


CBasicSock::~CBasicSock()
{
	Close();	
}

int CBasicSock::MainThread()
{
	/*
	스레드 생성 인자
	IpThreadAttributes : 생성하려는 스레드의 보안에 고나련된 설정을 위해 필요한 옵션.
	보안을 위해서 null포인터(nullptr)를 전달.
	dwStackSize		   : 스레드 생성하는 경우, 모든 메모리 공간은 스택 공간은 독립적으로 생성된다.
	따라서 스레드, 생성 시 요구되는 스택의 크기를 인자로 전달한다. 0을 전달할 경우
	디폴트로 설정되어 있는 스태그이 크기를 할당 받는다.
	IpStartAddress	   : 스레드에 의해 호출되는 함수의 포인터를 인자로 전달한다.
	IpParameter		   : IpStartAddress가 가리키는 함수 호출 시, 전달할 인자를 지정해 준다.
	dwCreateionFlags   : 새로운 스레드 생성 이후에 바로 실행 가능한 상태가 되냐, 아니면 대기 상태로 들어가느냐를
	결정하는 요소이다. 0을 전달할 경우 바로 실행 가능한 상태가 된다. (0이면 충분)
	IpThreadID		   : 스레드 생성 시 스레드의 ID가 리턴되는데, 이를 저장하기 위한 변수의 포인터이다.

	신경써야할 부분은 IpStartAddress 와 IpParameter 두가지 뿐인다.
	*/

	// this로 주고 쓰레드 생성하면 LPVOID로 전달된 것을 생성시 사용 함수가 포함된 클래스형 포인터로 변경되며 비static 함수처럼 사용하면서 스레드 함수로 사용가능
	m_hThread = CreateThread(nullptr, 0, this->ThreadProc, this, 0, &m_dwThreadID);

	

	// 생성시 에러
	if (NULL == m_hThread)
	{
		return -1;
	}
	return 0;
}

HWND CBasicSock::GetParent()
{
	return m_hParentHandle;
}


DWORD WINAPI CBasicSock::ThreadProc(LPVOID _lpParam)
{
	// Sync : 동기는 작업이 완료될 때까지 스레드가 멈춘다. 그래서 Blocking소켓이라 불린다.
	
	CBasicSock*			pBasicSock		= NULL; 
	pBasicSock = (CBasicSock*)_lpParam;

	DWORD				dwEventCheck	= 0;
	int					iCheckSocket	= 0;
	int					iConnResult		= 0;
	int					iEventID		= 0;
	BOOL				bCheckPack		= FALSE;

	PCSTR				IpAddr			= "192.168.0.90";
	PACKET_HEADER*		pstHeader		= NULL;
	PACKET_RSP_LOGIN*	pstResLogin		= NULL;
	PACKET_RSP_TEXT*	pstResText		= NULL;
	WSANETWORKEVENTS	stNetWorkEvents	= { 0 };

	char*				pszBuff			= new char[1024];
	int					iPort			= pBasicSock->m_iPort;
	int					iCheckPack		= 0;

	memset(pszBuff, 0, 1024);

	// Socket 생성
	pBasicSock->m_uiSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 이벤트 객체 생성
	pBasicSock->m_wsaEvent = WSACreateEvent();
	
	SOCKADDR_IN stClientInfo = { 0 };

	// IPv4 인터넷 프로토콜
	stClientInfo.sin_family = AF_INET;
	// htons()함수는 리틀 엔디안에서 빅 엔디안 방식으로 net_port에 저장

	stClientInfo.sin_port = htons(pBasicSock->m_iPort);
	/*
	af : AF_INET 또는 AF_INET6
	src : IPv4 또는 IPv6
	문자열을 프로토콜(IPv4, IPv6 등)에 해당하는 네트워크 데이터(빅엔디언 방식의 2진데이터)로 변경
	※inet_ntoa는 IPv4만 지원
	*/
	inet_pton(stClientInfo.sin_family, IpAddr, &stClientInfo.sin_addr);
	iConnResult = connect(pBasicSock->m_uiSocket, (SOCKADDR*)&stClientInfo, sizeof(stClientInfo));
	
	// 소켓 상태 체크
	iCheckSocket = WSAEventSelect(pBasicSock->m_uiSocket, pBasicSock->m_wsaEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	
	while (pBasicSock->m_dwThreadID)
	{
		/*
		FD_ACCEPT	:접속한 클라이언트가 있다
		FD_READ	    :데이터 수신이 가능하다
		FD_WRITE	:데이터 송신이 가능하다
		FD_CLOSE	:상대가 접속을 종료했다
		FD_CONNECT	:통신을 위한 연결 절차가 끝났다
		FD_OOB	    :OOB 데이터가 도착했다
		*/
		iEventID = WSAEnumNetworkEvents(pBasicSock->m_uiSocket, pBasicSock->m_wsaEvent, &stNetWorkEvents);
		if (stNetWorkEvents.lNetworkEvents & FD_CONNECT)
		{
			if (stNetWorkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
			{
				pBasicSock->Close();
			}
			else
			{
				// Connect시 Login도 함께 하기 위해서 여기서 함수를 호출한다.
				pBasicSock->SendPacket(PACKET_ID_REQ_LOGIN, NULL, 0);
			}
		}
		
		if (stNetWorkEvents.lNetworkEvents & FD_READ)
		{
			if (stNetWorkEvents.iErrorCode[FD_READ_BIT] != 0)
			{
				AfxMessageBox(_T("읽기가 원활하지 않다."));
				pBasicSock->Close();
			}
			else
			{
			//	//iCheckPack = recv(pBasicSock->m_uiSocket, pszBuff, 1024, 0);
				//// 데이터를 받아서 읽었을 때
				iCheckPack = recv(pBasicSock->m_uiSocket, pszBuff, 1024, 0);
				pstHeader = (PACKET_HEADER*)pszBuff;
				if (PACKET_ID_RSP_LOGIN == pstHeader->iPacketID)
				{
					pstResLogin = (PACKET_RSP_LOGIN*)pszBuff;
					pBasicSock->ReceivePacket(pstHeader);
				}
				
				if (PACKET_ID_RSP_TEXT == pstHeader->iPacketID)
				{
					//pBasicSock->ReceivePacket(pstHeader, pBasicSock->m_pDlg);
				}
			}
		}
		//iCheckSocket = send(pBasicSock->m_uiSocket, (char*)&stReqLogin, sizeof(PACKET_REQ_LOGIN), 0);
		if (stNetWorkEvents.lNetworkEvents & FD_WRITE)
		{
			if (stNetWorkEvents.iErrorCode[FD_WRITE_BIT] != 0)
			{
				closesocket(pBasicSock->m_uiSocket);
			}
			else
			{
				//pBasicSock->SendPacket(PACKET_ID_REQ_TEXT, NULL, 0);
			}
		}
		
		if (stNetWorkEvents.lNetworkEvents & FD_CLOSE)
		{
			if (stNetWorkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
			{
				closesocket(pBasicSock->m_uiSocket);
			}
		}

		/*
		인자값
		DWORD cEvents : 확인할 이벤트의 개수
		const WSAEVENT FAR* IphEvents : 확인할 이벤트가 담긴 배열(시작 주소)
		BOOL fWaitAll : 배열의 모든 이벤트 객체에 대해 모두 이벤트가 발생했는지 확인할 것인지
		DWORD dwTimeout : 이벤트 발생이 없을
		*/

		// WSAWaitForMultipleEvents
		// 이벤트가 발생했는지 확인
		dwEventCheck = WSAWaitForMultipleEvents(1, &pBasicSock->m_wsaEvent, TRUE, 1000, TRUE);
	}
	closesocket(pBasicSock->m_uiSocket);
	WSACleanup();

	return 0xffffffff;
}

void CBasicSock::Connect(PCSTR _strIP, CString _strUserID, int _iPort)
{	
	m_strIP = _strIP;
	m_iPort = 7777;
	m_strUserID = _strUserID;

	// 스레드 생성
	MainThread();
}

void CBasicSock::SendPacket(int _iPacketID, TCHAR* _pData, int _iLength)
{
	PACKET_HEADER		stHeader		= { 0 };
	PACKET_REQ_LOGIN	stReqLogin		= { 0 };
	PACKET_REQ_TEXT		stReqText		= { 0 };

	int					iCheckPack		= 0;

	switch (_iPacketID)
	{
	case PACKET_ID_REQ_LOGIN:
		stReqLogin.stHeader.iMarker = MARKER_CLIENT;
		stReqLogin.stHeader.iVersion = VERSION_PACKET_CLIENT_1;
		stReqLogin.stHeader.iPacketID = PACKET_ID_REQ_LOGIN;
		stReqLogin.stHeader.iPacketSize = sizeof(PACKET_REQ_LOGIN);
		wsprintf(stReqLogin.wszUserID, m_strUserID);
		iCheckPack = send(this->m_uiSocket, (char*)&stReqLogin, sizeof(PACKET_REQ_LOGIN), 0);
		break;

	case PACKET_ID_REQ_TEXT:
		stReqText.stHeader.iMarker = MARKER_CLIENT;
		stReqText.stHeader.iVersion = VERSION_PACKET_CLIENT_1;
		stReqText.stHeader.iPacketID = PACKET_ID_REQ_TEXT;
		stReqText.stHeader.iPacketSize = sizeof(PACKET_REQ_TEXT);

		wsprintf(stReqText.wszPacketText, _T("%s"), _pData);
		iCheckPack = send(this->m_uiSocket, (char*)&stReqText, sizeof(PACKET_REQ_TEXT), 0);
		break;

	default:
		break;
	}
}

void CBasicSock::ReceivePacket(PACKET_HEADER* _pstHeader)
{
	int					iCheckPack		= 0;
	char*				pszBuff			= NULL;

	iCheckPack = recv(m_uiSocket, pszBuff, 1024, 0);
	pszBuff = new char[1024];
	memset(pszBuff, 0, 1024);

	switch (_pstHeader->iPacketID)
	{
	case PACKET_ID_RSP_LOGIN:
			::PostMessage(GetParent(), WM_MESSAGE_SOCKET, LOGIN_SUCCESS, NULL);
		break;

	case PACKET_ID_RSP_TEXT:
		break;

	default:
		break;
	}

	if (NULL != pszBuff)
	{
		delete[] pszBuff;
		pszBuff = NULL;
	}
}

void CBasicSock::Close()
{
	if (NULL != m_hThread)
	{
		/*
		WaitForSingleObject 함수는 정상 종료되었을시 리턴 값을 WAIT_OBJECT_0을 리턴하고
		타임아웃 되어서 리턴하는 경우에는 WAIT_TIMEOUT을 리턴한다.
		*/

		// 쓰레드 실행이 완료될 때까지 대기한다.
		m_dwThreadID = 0;
		if (WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 30000))
		{
			// 강제 종료 함수
			TerminateThread(m_hThread, 0xffffffff);
		}
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	// 이벤트 객체 해제
	if (NULL != m_wsaEvent)
	{
		WSACloseEvent(m_wsaEvent);
		m_wsaEvent = NULL;
	}
}


