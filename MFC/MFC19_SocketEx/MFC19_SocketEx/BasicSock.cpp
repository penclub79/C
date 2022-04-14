#include "stdafx.h"
#include "BasicSock.h"

#define NUM_THREAD 1

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
	������ ���� ����
	IpThreadAttributes : �����Ϸ��� �������� ���ȿ� ���õ� ������ ���� �ʿ��� �ɼ�.
	������ ���ؼ� null������(nullptr)�� ����.
	dwStackSize		   : ������ �����ϴ� ���, ��� �޸� ������ ���� ������ ���������� �����ȴ�.
	���� ������, ���� �� �䱸�Ǵ� ������ ũ�⸦ ���ڷ� �����Ѵ�. 0�� ������ ���
	����Ʈ�� �����Ǿ� �ִ� ���±��� ũ�⸦ �Ҵ� �޴´�.
	IpStartAddress	   : �����忡 ���� ȣ��Ǵ� �Լ��� �����͸� ���ڷ� �����Ѵ�.
	IpParameter		   : IpStartAddress�� ����Ű�� �Լ� ȣ�� ��, ������ ���ڸ� ������ �ش�.
	dwCreateionFlags   : ���ο� ������ ���� ���Ŀ� �ٷ� ���� ������ ���°� �ǳ�, �ƴϸ� ��� ���·� �����ĸ�
	�����ϴ� ����̴�. 0�� ������ ��� �ٷ� ���� ������ ���°� �ȴ�. (0�̸� ���)
	IpThreadID		   : ������ ���� �� �������� ID�� ���ϵǴµ�, �̸� �����ϱ� ���� ������ �������̴�.

	�Ű����� �κ��� IpStartAddress �� IpParameter �ΰ��� ���δ�.
	*/

	// this�� �ְ� ������ �����ϸ� LPVOID�� ���޵� ���� ������ ��� �Լ��� ���Ե� Ŭ������ �����ͷ� ����Ǹ� ��static �Լ�ó�� ����ϸ鼭 ������ �Լ��� ��밡��
	m_hThread = CreateThread(nullptr, 0, this->ThreadProc, this, 0, &m_dwThreadID);

	// ������ ����
	if (NULL == m_hThread)
	{
		return -1;
	}
	return 0;
}


DWORD WINAPI CBasicSock::ThreadProc(LPVOID _lpParam)
{
	// Sync : ����� �۾��� �Ϸ�� ������ �����尡 �����. �׷��� Blocking�����̶� �Ҹ���.
	//CBasicSock* pBasicSock = _lpParam;
	CBasicSock*		pBasicSock; 
	DWORD			dwEventCheck	= 0;
	int				iCheckSocket	= 0;
	int				iClientVal		= 0;

	pBasicSock = (CBasicSock*)_lpParam;
	
	// Socket ����
	pBasicSock->m_uiSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// �̺�Ʈ ��ü�� ������.
	pBasicSock->m_wsaEvent = WSACreateEvent();
	
	 
	SOCKADDR_IN stClientInfo = { 0 };
	memset(&stClientInfo, 0, sizeof(stClientInfo));

	// IPv4 ���ͳ� ��������
	stClientInfo.sin_family = AF_INET;
	// htons()�Լ��� short int(2byte)�����͸� ��Ʈ��ũ byte order�� ����.

	/*stClientInfo.sin_port = htons(m_iPort);
	inet_pton(stClientInfo.sin_family, (PCSTR)m_iPort, &stClientInfo.sin_addr);
	iClientVal = connect(pBasicSock->m_uiSocket, (SOCKADDR*)&stClientInfo, sizeof(stClientInfo));*/


	while (true)
	{
		/*
		FD_ACCEPT	:������ Ŭ���̾�Ʈ�� �ִ�
		FD_READ	    :������ ������ �����ϴ�
		FD_WRITE	:������ �۽��� �����ϴ�
		FD_CLOSE	:��밡 ������ �����ߴ�
		FD_CONNECT	:����� ���� ���� ������ ������
		FD_OOB	    :OOB �����Ͱ� �����ߴ�
		*/

		// ���� ���� üũ
		iCheckSocket = WSAEventSelect(pBasicSock->m_uiSocket, pBasicSock->m_wsaEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

		/*
		���ڰ�
		DWORD cEvents : Ȯ���� �̺�Ʈ�� ����
		const WSAEVENT FAR* IphEvents : Ȯ���� �̺�Ʈ�� ��� �迭(���� �ּ�)
		BOOL fWaitAll : �迭�� ��� �̺�Ʈ ��ü�� ���� ��� �̺�Ʈ�� �߻��ߴ��� Ȯ���� ������
		DWORD dwTimeout : �̺�Ʈ �߻��� ����
		*/

		// WSAWaitForMultipleEvents
		// �̺�Ʈ�� �߻��ߴ��� Ȯ��
		dwEventCheck = WSAWaitForMultipleEvents(1, &pBasicSock->m_wsaEvent, TRUE, 1000, TRUE);
	}
	



	return 0;
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



