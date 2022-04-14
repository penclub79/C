#include "stdafx.h"
#include "BasicSock.h"

#define NUM_THREAD 1

CBasicSock::CBasicSock()
{
	m_dwThreadID = 0;
	m_hThread = 0;
}


CBasicSock::~CBasicSock()
{
	Close();	
}


DWORD WINAPI ThreadProc(LPVOID _lpParam)
{
	UINT iCount = 0;

	for (UINT i = 0; i < 1000; ++i)
	{
		++iCount;
	}
	return 0;

	// Connect
}

int CBasicSock::MainThread()
{
	//DWORD adwThreadID[NUM_THREAD] = { 0 };
	//HANDLE ahThread[NUM_THREAD] = { 0 };

	m_hThread = CreateThread(nullptr, 0, ThreadProc, nullptr, 0, &m_dwThreadID);

	return 0;
}


void CBasicSock::Connect(CString _strIP, CString _strUserID, int _iPort)
{
	
	// Sync : 동기는 작업이 완료될 때까지 스레드가 멈춘다. 그래서 Blocking소켓이라 불린다.
	SOCKET	stSocket;
	DWORD	dwResult = 0;

	stSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// 이벤트 객체를 생성함.
	m_wsaEvent = WSACreateEvent();

	/*
	FD_ACCEPT : accept 요청이 들어왔음, accept를 호출한다.
	FD_READ : 읽기 버퍼에 데이터가 들어옴, recv를 호출한다.
	FD_CLOSE : 소켓 닫힘, closeSocket을 호출한다.
	*/
	WSAEventSelect(stSocket, m_wsaEvent, FD_ACCEPT);

	/*
	WSAWaitForMultipleEvents
	이벤트가 발생했는지 확인
	*/
	dwResult = WSAWaitForMultipleEvents(1, &m_wsaEvent, TRUE, 1000, TRUE);
	



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
		DWORD dwResult = WaitForSingleObject(m_hThread, INFINITE);

		// 스레드 정상 종료시 해제
		if (WAIT_OBJECT_0 == dwResult)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}

		// 스레드 타임아웃시 해제
		if (WAIT_TIMEOUT == dwResult)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
	}

	// 이벤트 객체 해제
	if (NULL != m_wsaEvent)
	{
		WSACloseEvent(m_wsaEvent);
		m_wsaEvent = NULL;
	}
}



