#include "stdafx.h"
#include "BasicSock.h"
#include "WinInet.h"


CBasicSock::CBasicSock(HWND hParent)
{
	m_dwThreadID	= 0;
	m_hThread		= 0;
	m_iPort			= 0;
	m_iConnResult	= 0;
	m_iAliveStatus	= 0;
	m_hParentHandle = hParent;
	m_uiSocket		= 0;
	m_wsaEvent		= NULL;
	m_ulStartTime	= 0;
	m_ulLastTime	= 0;
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
	
	CBasicSock*				pBasicSock			= NULL; 
	pBasicSock	= (CBasicSock*)_lpParam;

	int						iCheckSocket		= 0;
	int						iEventID			= 0;
	char*					pszIPAddress		= NULL;

	PACKET_HEADER*			pstHeader			= NULL;
	PACKET_RSP_KEEPALIVE*	pstRspKeepAlive		= { 0 };
	PACKET_REQ_KEEPALIVE	pstReqKeepAlive		= { 0 };

	WSANETWORKEVENTS		stNetWorkEvents		= { 0 };
	pBasicSock->m_ulStartTime = GetTickCount();
	pBasicSock->m_ulLastTime = GetTickCount();
	char*					pszBuff				= NULL;
	char*					pszAliveBuff		= NULL;
	int						iPort				= pBasicSock->m_iPort;
	
	//DWORD dwFlag;
	//TCHAR szName[256];
	//BOOL					bInternet			= FALSE;


	pszBuff = new char[1024];
	memset(pszBuff, 0, 1024);

	pszAliveBuff = new char[1024];
	memset(pszAliveBuff, 0, 1024);

	pszIPAddress = new char[pBasicSock->m_strIP.GetLength() + 1];
	memset(pszIPAddress, 0, pBasicSock->m_strIP.GetLength());
	// MBCS->CP_UTF8
	strcpy(pszIPAddress, CT2A(pBasicSock->m_strIP));

	// Socket 생성
	if (NULL == pBasicSock->m_uiSocket)
		pBasicSock->m_uiSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	// 이벤트 객체 생성
	if (NULL == pBasicSock->m_wsaEvent)
		pBasicSock->m_wsaEvent = WSACreateEvent();
	
	SOCKADDR_IN stClientInfo;
	memset(&stClientInfo, 0, sizeof(SOCKADDR_IN));

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
	inet_pton(stClientInfo.sin_family, (PCSTR)pszIPAddress, &stClientInfo.sin_addr);

	// 소켓 상태 체크
	iCheckSocket = WSAEventSelect(pBasicSock->m_uiSocket, pBasicSock->m_wsaEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

	pBasicSock->m_iConnResult = connect(pBasicSock->m_uiSocket, (SOCKADDR*)&stClientInfo, sizeof(stClientInfo));

	while (pBasicSock->m_dwThreadID)
	{	
		//// 로그인 성공시
		//if (LOGIN_SUCCESS == pBasicSock->m_iConnResult)
		//{
		//	if (GetTickCount() - ulStartTime >= 5000)
		//	{
		//		pBasicSock->SendPacket(PACKET_ID_REQ_ALIVE, NULL, sizeof(PACKET_REQ_KEEPALIVE));
		//		ulStartTime = GetTickCount();
		//	}

		//	if (GetTickCount() - ulLastTime >= 10000)
		//	{
		//		pBasicSock->m_iConnResult = SERVER_CLOSE;
		//		break;
		//	}
		//}
		// 시간 관련 getTickCount 이런거 밀리세컨드로 현재 시간 알려줌. 변수 만들어서 현재 시간 - 시작 시간 5초마다 한번씩 send로 패킷 살아있는지 통신 확인 , 서버에서 바로 응답 처리하고 
		
		// 로그인 성공시
		// 라스트 리시브 데이터 타임 비교 - 예를들어 마지막으로 받은 데이터 시간과 10초 동안 아무일이 없으면 끊으면 된다.
		
		

		
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
				pBasicSock->m_iConnResult = CONNECTED_ERROR;
				break;
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
				pBasicSock->m_iConnResult = READ_ERROR;
				break;
			}
			else
			{
				//// 데이터를 받아서 읽었을 때
				recv(pBasicSock->m_uiSocket, pszBuff, 1024, 0);
				pstHeader = (PACKET_HEADER*)pszBuff;
				
				if (PACKET_ID_RSP_LOGIN == pstHeader->iPacketID)
				{	
					pBasicSock->ReceivePacket(pstHeader, pszBuff);
					pBasicSock->m_iConnResult = LOGIN_SUCCESS;
					pBasicSock->m_ulLastTime = GetTickCount();
				}

				if (LOGIN_SUCCESS == pBasicSock->m_iConnResult)
				{
					// 메시지
					if (PACKET_ID_RSP_TEXT == pstHeader->iPacketID)
					{
						pBasicSock->ReceivePacket(pstHeader, pszBuff);
						pBasicSock->m_ulLastTime = GetTickCount();
					}

					// KEEP ALIVE 확인
					if (PACKET_ID_RSP_ALIVE == pstHeader->iPacketID)
					{
						pBasicSock->m_iAliveStatus = ALIVE_SUCCESS;
						pBasicSock->m_ulLastTime = GetTickCount();
					}
				}
								
			}
		}

		if (stNetWorkEvents.lNetworkEvents & FD_WRITE)
		{
			if (stNetWorkEvents.iErrorCode[FD_WRITE_BIT] != 0)
			{
				pBasicSock->m_iConnResult = WRITE_ERROR;
				break;
			}
		}
		
		if (stNetWorkEvents.lNetworkEvents & FD_CLOSE)
		{
			if (stNetWorkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
			{
				pBasicSock->m_iConnResult = CLOSED_ERROR;
				break;
			}
			else
			{
				pBasicSock->m_iConnResult = SERVER_CLOSE;
				break;
			}
		}
		Sleep(0);
	}

	if (NULL != pBasicSock->m_uiSocket)
	{
		closesocket(pBasicSock->m_uiSocket);
		pBasicSock->m_uiSocket = 0;
	}

	// 이벤트 객체 해제
	if (NULL != pBasicSock->m_wsaEvent)
	{
		WSACloseEvent(pBasicSock->m_wsaEvent);
		pBasicSock->m_wsaEvent = NULL;
	}

	if (NULL != pszBuff)
	{
		delete[] pszBuff;
		pszBuff = NULL;
	}

	if (NULL != pszAliveBuff)
	{
		delete[] pszAliveBuff;
		pszAliveBuff = NULL;
	}

	if (NULL != pszIPAddress)
	{
		delete[] pszIPAddress;
		pszIPAddress = NULL;
	}

	if (LOGIN_SUCCESS != pBasicSock->m_iConnResult)
	{
		::PostMessage(pBasicSock->GetParent(), WM_MESSAGE_SOCKET, pBasicSock->m_iConnResult, NULL);
	}
	
	// 밑에 코드는 ExitThread(0xffffffff)와 같다
	return 0xffffffff;
}

void CBasicSock::Connect(TCHAR* _pszIP, TCHAR* _pszUserID, int _iPort)
{	
	m_strIP.Format(_T("%s"), _pszIP);
	m_strUserID.Format(_T("%s"), _pszUserID);
	m_iPort = 7777;

	// 스레드 생성
	MainThread();

	// settimer 함수 호출 
	SetTimer(GetParent(), m_dwThreadID, 5000, TimerProc);
	//SetTimer(GetParent(), m_dwThreadID, 10000, (TIMERPROC)&TimerProc);
}

void CALLBACK CBasicSock::TimerProc(HWND _hWnd, UINT _uiMessage, UINT _uiIDEvent, DWORD _ulTime)
{
	
	TRACE(_T("1 \n"));
	//CBasicSock* pBasicSock = (CBasicSock*)_hWnd;
	CWnd* pWnd = AfxGetMainWnd();
	CBasicSock* pBasicSock = (CBasicSock*)pWnd;
	
	
	if (LOGIN_SUCCESS == pBasicSock->m_iConnResult)
		{
			if (GetTickCount() - pBasicSock->m_ulStartTime >= 5000)
			{
				pBasicSock->SendPacket(PACKET_ID_REQ_ALIVE, NULL, sizeof(PACKET_REQ_KEEPALIVE));
				pBasicSock->m_ulStartTime = GetTickCount();
			}

			if (GetTickCount() - pBasicSock->m_ulLastTime >= 10000)
			{
				pBasicSock->m_iConnResult = SERVER_CLOSE;
				pBasicSock->Close();
			}
		}

	//KillTimer(_hWnd, _uiIDEvent);
}

void CBasicSock::SendPacket(int _iPacketID, TCHAR* _pData, int _iLength)
{
	PACKET_HEADER			stHeader		= { 0 };
	PACKET_REQ_LOGIN		stReqLogin		= { 0 };
	PACKET_REQ_TEXT			stReqText		= { 0 };
	PACKET_REQ_KEEPALIVE	stReqKeepAlive	= { 0 };
	TCHAR*					pszUserID		= NULL;

	switch (_iPacketID)
	{
	case PACKET_ID_REQ_LOGIN:
		stReqLogin.stHeader.iMarker = MARKER_CLIENT;
		stReqLogin.stHeader.iVersion = VERSION_PACKET_CLIENT_1;
		stReqLogin.stHeader.iPacketID = PACKET_ID_REQ_LOGIN;
		stReqLogin.stHeader.iPacketSize = sizeof(PACKET_REQ_LOGIN);

		// CString->TCHAR으로 형변환
		pszUserID = m_strUserID.GetBuffer(m_strUserID.GetLength());
		m_strUserID.ReleaseBuffer();
		
		// TCHAR <-> TCHAR
		wsprintf(stReqLogin.wszUserID, pszUserID);

		send(this->m_uiSocket, (char*)&stReqLogin, sizeof(PACKET_REQ_LOGIN), 0);
		break;

	case PACKET_ID_REQ_TEXT:
		stReqText.stHeader.iMarker = MARKER_CLIENT;
		stReqText.stHeader.iVersion = VERSION_PACKET_CLIENT_1;
		stReqText.stHeader.iPacketID = PACKET_ID_REQ_TEXT;
		stReqText.stHeader.iPacketSize = sizeof(PACKET_REQ_TEXT);

		// TCHAR <-> TCHAR
		wsprintf(stReqText.wszPacketText, _pData);
		send(this->m_uiSocket, (char*)&stReqText, sizeof(PACKET_REQ_TEXT), 0);
		break;

	case PACKET_ID_REQ_ALIVE:
		stReqKeepAlive.stHeader.iMarker = MARKER_CLIENT;
		stReqKeepAlive.stHeader.iVersion = VERSION_PACKET_CLIENT_1;
		stReqKeepAlive.stHeader.iPacketID = PACKET_ID_REQ_ALIVE;
		stReqKeepAlive.stHeader.iPacketSize = sizeof(PACKET_REQ_KEEPALIVE);
		
		send(this->m_uiSocket, (char*)&stReqKeepAlive, sizeof(PACKET_REQ_KEEPALIVE), 0);
	default:
		break;
	}
}

void CBasicSock::ReceivePacket(PACKET_HEADER* _pstHeader, char* _pszPacket)
{
	int					iCheckPack		= 0;

	PACKET_RSP_LOGIN*	pstRspLogin		= NULL;
	PACKET_RSP_TEXT*	pstRspText		= NULL;

	switch (_pstHeader->iPacketID)
	{
	case PACKET_ID_RSP_LOGIN:
		pstRspLogin = (PACKET_RSP_LOGIN*)_pszPacket;
		::PostMessage(GetParent(), WM_MESSAGE_SOCKET, LOGIN_SUCCESS, NULL);
		break;

	case PACKET_ID_RSP_TEXT:
		pstRspText = (PACKET_RSP_TEXT*)_pszPacket;
		// 해당 Message는 SendMessage로 처리하는 것이 좋은 설계이다.
		::PostMessage(GetParent(), WM_MESSAGE_SOCKET, TEXT_SUCCESS, (LPARAM)pstRspText);
		break;
	
	default:
		break;
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
			
		/*
		WaitForSingleObject() : 지정한 오브젝트가 시그널 상태가 되거나 타임아웃이 되면 제어를 돌려준다.
		지정한 오브젝트가 비시그널 상태이면 시그널 상태가 될때까지 설정한 타임아웃 시간만큼 WaitForSingleObject에서 대기한다.
		*/
		if (WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 30000))
		{
			// 강제 종료 함수
			TerminateThread(m_hThread, 0xffffffff);
		}

		// 처리
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	
}


