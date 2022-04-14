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
	
	// Sync : ����� �۾��� �Ϸ�� ������ �����尡 �����. �׷��� Blocking�����̶� �Ҹ���.
	SOCKET	stSocket;
	DWORD	dwResult = 0;

	stSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// �̺�Ʈ ��ü�� ������.
	m_wsaEvent = WSACreateEvent();

	/*
	FD_ACCEPT : accept ��û�� ������, accept�� ȣ���Ѵ�.
	FD_READ : �б� ���ۿ� �����Ͱ� ����, recv�� ȣ���Ѵ�.
	FD_CLOSE : ���� ����, closeSocket�� ȣ���Ѵ�.
	*/
	WSAEventSelect(stSocket, m_wsaEvent, FD_ACCEPT);

	/*
	WSAWaitForMultipleEvents
	�̺�Ʈ�� �߻��ߴ��� Ȯ��
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
		WaitForSingleObject �Լ��� ���� ����Ǿ����� ���� ���� WAIT_OBJECT_0�� �����ϰ�
		Ÿ�Ӿƿ� �Ǿ �����ϴ� ��쿡�� WAIT_TIMEOUT�� �����Ѵ�.
		*/

		// ������ ������ �Ϸ�� ������ ����Ѵ�.
		DWORD dwResult = WaitForSingleObject(m_hThread, INFINITE);

		// ������ ���� ����� ����
		if (WAIT_OBJECT_0 == dwResult)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}

		// ������ Ÿ�Ӿƿ��� ����
		if (WAIT_TIMEOUT == dwResult)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
	}

	// �̺�Ʈ ��ü ����
	if (NULL != m_wsaEvent)
	{
		WSACloseEvent(m_wsaEvent);
		m_wsaEvent = NULL;
	}
}



