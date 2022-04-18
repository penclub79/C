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
	
	CBasicSock*			pBasicSock		= NULL; 
	pBasicSock = (CBasicSock*)_lpParam;

	DWORD				dwEventCheck	= 0;
	int					iCheckSocket	= 0;
	int					iConnResult		= 0;
	int					iEventID		= 0;
	BOOL				bCheckPack		= FALSE;

	PCSTR				IpAddr			= "192.168.0.90";
	PACKET_HEADER		pHeader			= { 0 };
	WSANETWORKEVENTS	stNetWorkEvents	= { 0 };
	char*				pszBuff			= new char[1024];
	int					iPort			= pBasicSock->m_iPort;

	memset(pszBuff, 0, 1024);

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
	
	// ���� ���� üũ
	iCheckSocket = WSAEventSelect(pBasicSock->m_uiSocket, pBasicSock->m_wsaEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	
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
		iEventID = WSAEnumNetworkEvents(pBasicSock->m_uiSocket, pBasicSock->m_wsaEvent, &stNetWorkEvents);
		if (stNetWorkEvents.lNetworkEvents & FD_CONNECT)
		{
			if (stNetWorkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
			{
				AfxMessageBox(_T("������ ��Ȱ���� �ʴ�."));
				pBasicSock->Close();
			}
			else
			{
				pBasicSock->SendPacket(NULL, 0, PACKET_ID_REQ_LOGIN);
			}
		}
		
		if (stNetWorkEvents.lNetworkEvents & FD_READ)
		{
			if (stNetWorkEvents.iErrorCode[FD_READ_BIT] != 0)
			{
				AfxMessageBox(_T("�бⰡ ��Ȱ���� �ʴ�."));
				closesocket(pBasicSock->m_uiSocket);
			}
			else
			{	
				// �����͸� �޾Ƽ� �о��� ��
				iCheckSocket = recv(pBasicSock->m_uiSocket, pszBuff, 1024, 0);


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
				//pBasicSock->SendPacket(pszBuff, );
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

	return 0xffffffff;
}

void CBasicSock::Connect(PCSTR _strIP, CString _strUserID, int _iPort)
{	
	m_strIP = _strIP;
	m_iPort = 7777;
	m_strUserID = _strUserID;

	MainThread();
}

//BOOL CBasicSock::IsLogin()
//{
//	// ����� ��Ŷ�� ������ ������.
//	PACKET_HEADER		stHeader	= { 0 };
//	char*				pszBuff		= new char[1024];
//	PACKET_REQ_LOGIN	stReqLogin	= { 0 };
//	PACKET_RSP_LOGIN*	pRspLogin	= NULL;
//	int					iCheckPack	= 0;
//	memset(pszBuff, 0, 1024);
//
//	stReqLogin.stHeader.iMarker = MARKER_CLIENT;
//	stReqLogin.stHeader.iVersion = VERSION_PACKET_CLIENT_1;
//	stReqLogin.stHeader.iPacketID = PACKET_ID_REQ_LOGIN;
//	stReqLogin.stHeader.iPacketSize = sizeof(PACKET_REQ_LOGIN);
//
//	wsprintf(stReqLogin.wszUserID, m_strUserID);
//
//	iCheckPack = send(this->m_uiSocket, (char*)&stReqLogin, sizeof(PACKET_REQ_LOGIN), 0);
//	iCheckPack = recv(this->m_uiSocket, pszBuff, 1024, 0);
//	
//	if (0 != iCheckPack)
//	{
//		// �������� �޾ƿ� iResultCode�� �޾ƿ´�.
//		pRspLogin = (PACKET_RSP_LOGIN*)pszBuff;
//		
//		if (0 != pRspLogin->iResultCode)
//			return FALSE;
//	}
//
//	if (NULL != pszBuff)
//	{
//		delete[] pszBuff;
//		pszBuff = NULL;
//	}
//	return TRUE;
//}

//void CBasicSock::Write(char* _pData, int _iLength)
//{
//	PACKET_HEADER		stHeader	= { 0 };
//	char*				pszBuff		= new char[1024];
//	PACKET_REQ_LOGIN	stReqLogin	= { 0 };
//	int					iLength		= 0;
//
//	memset(pszBuff, 0, 1024);
//
//	stReqLogin.stHeader.iMarker = MARKER_CLIENT;
//	stReqLogin.stHeader.iVersion = VERSION_PACKET_CLIENT_1;
//	stReqLogin.stHeader.iPacketID = PACKET_ID_REQ_TEXT;
//	stReqLogin.stHeader.iPacketSize = sizeof(PACKET_REQ_LOGIN);
//
//	iLength = _iLength;
//	wsprintf(stReqLogin.wszUserID, m_strUserID);
//
//	if (NULL != pszBuff)
//	{
//		delete[] pszBuff;
//		pszBuff = NULL;
//	}
//}

void CBasicSock::SendPacket(char* _pData, int _iLength, int _iPacketCode)
{
	PACKET_HEADER		stHeader		= { 0 };
	char*				pszBuff			= new char[1024];
	PACKET_REQ_LOGIN	stReqLogin		= { 0 };
	int					iLength			= 0;
	int					iCheckPack		= 0;

	memset(pszBuff, 0, 1024);

	switch (_iPacketCode)
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


