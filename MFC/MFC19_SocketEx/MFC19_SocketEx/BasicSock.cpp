#include "stdafx.h"
#include "BasicSock.h"

#define NUM_THREAD 1
#define CLIENTPORT

CBasicSock::CBasicSock()
{
	m_dwThreadID = 0;
	m_hThread = 0;
	m_iPort = 0;
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


DWORD WINAPI CBasicSock::ThreadProc(LPVOID _lpParam)
{
	// Sync : 동기는 작업이 완료될 때까지 스레드가 멈춘다. 그래서 Blocking소켓이라 불린다.
	CBasicSock*		pBasicSock		= NULL; 
	DWORD			dwEventCheck	= 0;
	int				iCheckSocket	= 0;
	int				iConnResult		= 0;
	
	int				iPort			= 0x1E61;
	PCSTR			iServerIP		= "192.168.0.90";

	pBasicSock = (CBasicSock*)_lpParam;
	
	// Socket 생성
	pBasicSock->m_uiSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 이벤트 객체를 생성함.
	pBasicSock->m_wsaEvent = WSACreateEvent();
	
	SOCKADDR_IN stClientInfo = { 0 };

	// IPv4 인터넷 프로토콜
	stClientInfo.sin_family = AF_INET;
	// htons()함수는 리틀 엔디안에서 빅 엔디안 방식으로 net_port에 저장

	stClientInfo.sin_port = htons(iPort);
	/*
	af : AF_INET 또는 AF_INET6
	src : IPv4 또는 IPv6

	문자열을 프로토콜(IPv4, IPv6 등)에 해당하는 네트워크 데이터(빅엔디언 방식의 2진데이터)로 변경
	※inet_ntoa는 IPv4만 지원
	*/
	inet_pton(stClientInfo.sin_family, iServerIP, &stClientInfo.sin_addr);
	iConnResult = connect(pBasicSock->m_uiSocket, (SOCKADDR*)&stClientInfo, sizeof(stClientInfo));

	// 연결 성공
	if (SOCKET_ERROR == iConnResult)
	{
		AfxMessageBox(_T("Error Connect"));
	}

	
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

		// 소켓 상태 체크
		iCheckSocket = WSAEventSelect(pBasicSock->m_uiSocket, pBasicSock->m_wsaEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

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
	

	return 0xffffffff;
}

void CBasicSock::Connect(CString _strIP, CString _strUserID, int _iPort)
{
	m_strIP = _strIP;
	m_strUserID = _strUserID;
	m_iPort = _iPort;
	//CString _strIP, CString _strUserID, int _iPort

	MainThread();
	

}

void CBasicSock::Write(char* _pData, int _iLength)
{
	PACKET_HEADER stHeader = { 0 };

	stHeader.iMarker = MARKER_CLIENT;
	stHeader.iVersion = VERSION_PACKET_CLIENT_1;
	stHeader.iPacketSize = sizeof(PACKET_REQ_LOGIN);
	stHeader.iPacketID = PACKET_ID_REQ_LOGIN;

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



