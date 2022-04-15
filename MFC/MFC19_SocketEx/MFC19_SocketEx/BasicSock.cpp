#include "stdafx.h"
#include "BasicSock.h"
#include "MFC19_SocketExDlg.h"

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
	
	CBasicSock*		pBasicSock		= NULL; 
	DWORD			dwEventCheck	= 0;
	int				iCheckSocket	= 0;
	int				iConnResult		= 0;
	PCSTR			IpAddr = "192.168.0.90";
	//PACKET_HEADER	pHeader = { 0 };
	
	pBasicSock = (CBasicSock*)_lpParam;
	int iPort = pBasicSock->m_iPort;

	
	// Socket ����
	pBasicSock->m_uiSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// �̺�Ʈ ��ü ����
	pBasicSock->m_wsaEvent = WSACreateEvent();
	
	SOCKADDR_IN stClientInfo = { 0 };

	// IPv4 ���ͳ� ��������
	stClientInfo.sin_family = AF_INET;
	// htons()�Լ��� ��Ʋ ����ȿ��� �� ����� ������� net_port�� ����

	stClientInfo.sin_port = htons(pBasicSock->m_iPort);
	/*
	af : AF_INET �Ǵ� AF_INET6
	src : IPv4 �Ǵ� IPv6

	���ڿ��� ��������(IPv4, IPv6 ��)�� �ش��ϴ� ��Ʈ��ũ ������(�򿣵�� ����� 2��������)�� ����
	��inet_ntoa�� IPv4�� ����
	*/
	inet_pton(stClientInfo.sin_family, IpAddr, &stClientInfo.sin_addr);
	iConnResult = connect(pBasicSock->m_uiSocket, (SOCKADDR*)&stClientInfo, sizeof(stClientInfo));

	// ���� ����
	if (SOCKET_ERROR != iConnResult)
	{
		PACKET_REQ_LOGIN stReqLogin = { 0 };
		int iLength = 0;
		//AfxMessageBox(_T("Connect"));

		stReqLogin.stHeader.iMarker = MARKER_CLIENT;
		stReqLogin.stHeader.iVersion = VERSION_PACKET_CLIENT_1;
		stReqLogin.stHeader.iPacketID = PACKET_ID_REQ_LOGIN;
		stReqLogin.stHeader.iPacketSize = sizeof(PACKET_REQ_LOGIN);
		
		iLength = pBasicSock->m_strUserID.GetLength();
		wsprintf(stReqLogin.wszUserID, pBasicSock->m_strUserID);

		iCheckSocket = send(pBasicSock->m_uiSocket, (char*)&stReqLogin, sizeof(PACKET_REQ_LOGIN), 0);
		if (SOCKET_ERROR == iCheckSocket)
		{
			//AfxMessageBox(_T("Send Success"));
		}

		//iCheckSocket = recv(pBasicSock->m_uiSocket, (char*)&)


		while (pBasicSock->m_dwThreadID)
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

		closesocket(pBasicSock->m_uiSocket);
		WSACleanup();
	}
	else
	{
		AfxMessageBox(_T("Connect Fail"));
	}

	return 0xffffffff;
}

void CBasicSock::Connect(PCSTR _strIP, CString _strUserID, int _iPort)
{	
	m_strIP = _strIP;
	m_iPort = 7777;
	m_strUserID = _strUserID;

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
		m_dwThreadID = 0;
		if (WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 30000))
		{
			// ���� ���� �Լ�
			TerminateThread(m_hThread, 0xffffffff);
		}

		CloseHandle(m_hThread);
		m_hThread = NULL;

	}

	// �̺�Ʈ ��ü ����
	if (NULL != m_wsaEvent)
	{
		WSACloseEvent(m_wsaEvent);
		m_wsaEvent = NULL;
	}
}


