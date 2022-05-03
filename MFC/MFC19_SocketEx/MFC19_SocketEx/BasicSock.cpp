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

HWND CBasicSock::GetParent()
{
	return m_hParentHandle;
}


DWORD WINAPI CBasicSock::ThreadProc(LPVOID _lpParam)
{
	// Sync : ����� �۾��� �Ϸ�� ������ �����尡 �����. �׷��� Blocking�����̶� �Ҹ���.
	
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

	// Socket ����
	if (NULL == pBasicSock->m_uiSocket)
		pBasicSock->m_uiSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	// �̺�Ʈ ��ü ����
	if (NULL == pBasicSock->m_wsaEvent)
		pBasicSock->m_wsaEvent = WSACreateEvent();
	
	SOCKADDR_IN stClientInfo;
	memset(&stClientInfo, 0, sizeof(SOCKADDR_IN));

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
	inet_pton(stClientInfo.sin_family, (PCSTR)pszIPAddress, &stClientInfo.sin_addr);

	// ���� ���� üũ
	iCheckSocket = WSAEventSelect(pBasicSock->m_uiSocket, pBasicSock->m_wsaEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

	pBasicSock->m_iConnResult = connect(pBasicSock->m_uiSocket, (SOCKADDR*)&stClientInfo, sizeof(stClientInfo));

	while (pBasicSock->m_dwThreadID)
	{	
		//// �α��� ������
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
		// �ð� ���� getTickCount �̷��� �и�������� ���� �ð� �˷���. ���� ���� ���� �ð� - ���� �ð� 5�ʸ��� �ѹ��� send�� ��Ŷ ����ִ��� ��� Ȯ�� , �������� �ٷ� ���� ó���ϰ� 
		
		// �α��� ������
		// ��Ʈ ���ú� ������ Ÿ�� �� - ������� ���������� ���� ������ �ð��� 10�� ���� �ƹ����� ������ ������ �ȴ�.
		
		

		
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
				pBasicSock->m_iConnResult = CONNECTED_ERROR;
				break;
			}
			else
			{
				// Connect�� Login�� �Բ� �ϱ� ���ؼ� ���⼭ �Լ��� ȣ���Ѵ�.
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
				//// �����͸� �޾Ƽ� �о��� ��
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
					// �޽���
					if (PACKET_ID_RSP_TEXT == pstHeader->iPacketID)
					{
						pBasicSock->ReceivePacket(pstHeader, pszBuff);
						pBasicSock->m_ulLastTime = GetTickCount();
					}

					// KEEP ALIVE Ȯ��
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

	// �̺�Ʈ ��ü ����
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
	
	// �ؿ� �ڵ�� ExitThread(0xffffffff)�� ����
	return 0xffffffff;
}

void CBasicSock::Connect(TCHAR* _pszIP, TCHAR* _pszUserID, int _iPort)
{	
	m_strIP.Format(_T("%s"), _pszIP);
	m_strUserID.Format(_T("%s"), _pszUserID);
	m_iPort = 7777;

	// ������ ����
	MainThread();

	// settimer �Լ� ȣ�� 
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

		// CString->TCHAR���� ����ȯ
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
		// �ش� Message�� SendMessage�� ó���ϴ� ���� ���� �����̴�.
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
		WaitForSingleObject �Լ��� ���� ����Ǿ����� ���� ���� WAIT_OBJECT_0�� �����ϰ�
		Ÿ�Ӿƿ� �Ǿ �����ϴ� ��쿡�� WAIT_TIMEOUT�� �����Ѵ�.
		*/

		// ������ ������ �Ϸ�� ������ ����Ѵ�.
		m_dwThreadID = 0;
			
		/*
		WaitForSingleObject() : ������ ������Ʈ�� �ñ׳� ���°� �ǰų� Ÿ�Ӿƿ��� �Ǹ� ��� �����ش�.
		������ ������Ʈ�� ��ñ׳� �����̸� �ñ׳� ���°� �ɶ����� ������ Ÿ�Ӿƿ� �ð���ŭ WaitForSingleObject���� ����Ѵ�.
		*/
		if (WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 30000))
		{
			// ���� ���� �Լ�
			TerminateThread(m_hThread, 0xffffffff);
		}

		// ó��
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	
}


