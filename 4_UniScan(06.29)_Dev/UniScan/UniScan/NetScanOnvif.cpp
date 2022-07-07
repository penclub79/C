#include "./Stdafx.h"
#include "NetScanOnvif.h"
#include <atlconv.h>
#include "xmlite\XMLite.h"

CNetScanOnvif::CNetScanOnvif()
{
	m_bConnected = FALSE;
}

CNetScanOnvif::~CNetScanOnvif(void)
{
	
}

DWORD CNetScanOnvif::thrOnvifScanThread(LPVOID pParam)
{
	CNetScanOnvif* pThis = (CNetScanOnvif*)pParam;
	if (NULL == pThis)
		return 0;

	pThis->thrOnvifReceiver();

	return 0;
}


BOOL CNetScanOnvif::StartScan()
{
	this->StartScanF((LPTHREAD_START_ROUTINE)thrOnvifScanThread);
	return TRUE;
}

void CNetScanOnvif::thrOnvifReceiver()
{
	BOOL		bIsSuccessBind = FALSE;
	SOCKADDR	stSockAddr;
	char*		pszBuff = NULL;
	int			iRecvSock = 0;
	//sockaddr_in	multicast_addr;
	int			iRevLen = sizeof(sockaddr_in);
	//struct		ip_mreq mreq;
	LPXNode		lpHeader = NULL;
	LPXNode		lpBody = NULL;
	LPXNode		lpBodyCommon = NULL;
	LPXNode		lpItemData = NULL;
	XNode		stNode;
	int			iError = 0;
	int			iReuse = 1;
	DWORD		dwLastError = 0;

	
	m_pReceive_buffer = new char[SCAN_INFO_m_pReceive_buffer_SIZE];
	memset(m_pReceive_buffer, 0, sizeof(char)* SCAN_INFO_m_pReceive_buffer_SIZE);

	while (this->m_dwScanThreadID)
	{
		if (NULL == m_hReceiveSock || FALSE == m_bConnected)
		{
			Sleep(100);
			continue;
		}

		if (SOCKET_ERROR == recvfrom(m_hReceiveSock, m_pReceive_buffer, SCAN_INFO_m_pReceive_buffer_SIZE, 0, (SOCKADDR*)&stSockAddr, &iRevLen))
		{
			dwLastError = WSAGetLastError();
			TRACE(_T("recvfrom error=%d\n"), dwLastError);


			if (this->m_hNotifyWnd && dwLastError != WSAEINTR && dwLastError != WSAEINVAL)
			{
				::PostMessage(this->m_hNotifyWnd, this->m_lNotifyMsg, 0, SCAN_ERR_RECV);
				this->ThreadExit();
			}
		}

		::OutputDebugStringA(m_pReceive_buffer);
		::OutputDebugStringA("\n");
	}
	

	return;


	//// UDP 소켓 생성
	//recvSock = socket(AF_INET, SOCK_DGRAM, 0);
	//memset(&multicast_addr, 0, sizeof(multicast_addr));

	//// 멀티 캐스트 주소 정보 초기화
	//multicast_addr.sin_family = AF_INET;
	//multicast_addr.sin_addr.s_addr = inet_addr("192.168.0.90"); // 남는 IP를 자동으로 적용해주는 함수
	//multicast_addr.sin_port = htons(multicast_addr.sin_port);
	//
	//if (SOCKET_ERROR == bind(recvSock, (struct sockaddr*)&multicast_addr, sizeof(multicast_addr)))
	//{
	//	TRACE("bind error\n");
	//}

	//// 가입
	//mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
	//mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	//// 멀티캐스트 그룹 가입
	//if (SOCKET_ERROR == setsockopt(recvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)))
	//{
	//	TRACE("IP_ADD_MEMBERSHIP error\n");
	//}


	////if (bIsSuccessBind)
	////{
	//	m_pReceive_buffer = new char[SCAN_INFO_m_pReceive_buffer_SIZE];
	//	memset(m_pReceive_buffer, 0, sizeof(char)*SCAN_INFO_m_pReceive_buffer_SIZE);

	//	while (this->m_dwScanThreadID)
	//	{
	//		//if (SOCKET_ERROR == recvfrom(recvSock, m_pReceive_buffer, sizeof(pszBuff), 0, (SOCKADDR*)&stSockAddr, &iRevLen))
	//		if (SOCKET_ERROR == recvfrom(recvSock, m_pReceive_buffer, sizeof(SCAN_INFO_m_pReceive_buffer_SIZE), 0, (SOCKADDR*)&other_addr, &iRevLen))
	//		{
	//			TRACE("recvfrom error\n");
	//			this->ThreadExit();
	//			break;
	//		}

	//		
	//		TRACE("recvfrom Success\n");
	//	}
	////}
	//
	//return;
}

BOOL CNetScanOnvif::CreateSocket()
{
	//struct in_addr localInterface = { 0 };
	int			iError = 0;
	char		szEnable = 0;
	struct		ip_mreq mreq;
	struct		in_addr localInterface;
	int			reuse = 0;

	if (NULL == m_hReceiveSock)
	{
		m_hReceiveSock = socket(AF_INET, SOCK_DGRAM, 0);
		
		/*
		* Disable loopback so you do not receive your own datagrams.
		*/
		if (SOCKET_ERROR == setsockopt(m_hReceiveSock, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&szEnable, sizeof(szEnable)))
		{
			iError = WSAGetLastError();
			TRACE(_T("SetSocket Error = %d\n"), iError);
			closesocket(m_hReceiveSock);
			return FALSE;
		}

		/*
		* Set local interface for outbound multicast datagrams.
		* The IP address specified must be associated with a local,
		* multicast-capable interface.
		*/
		localInterface.s_addr = m_ulBindAddress; //inet_addr("192.168.0.40");
		if (SOCKET_ERROR == setsockopt( m_hReceiveSock, IPPROTO_IP, IP_MULTICAST_IF, (char*)&localInterface, sizeof(localInterface)) )
		{
			iError = WSAGetLastError();
			TRACE(_T("SetSocket Error = %d\n"), iError);
			closesocket(m_hReceiveSock);
			return FALSE;
		}

		/*
		* Enable SO_REUSEADDR to allow multiple instances of this
		* application to receive copies of the multicast datagrams.
		*/
		reuse = 1;
		if (SOCKET_ERROR == setsockopt(m_hReceiveSock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)))
		{
			TRACE("getaddrinfo error = %d\n", WSAGetLastError());
			closesocket(m_hReceiveSock);
			m_hReceiveSock = NULL;
			return FALSE;
		}

		/*
		* Join the multicast group 225.1.1.1 on the local 9.5.1.1
		* interface.  Note that this IP_ADD_MEMBERSHIP option must be
		* called for each local interface over which the multicast
		* datagrams are to be received.
		*/
		mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250"); //multi cast group
		mreq.imr_interface.s_addr = m_ulBindAddress; // Local
		//mreq.imr_interface.s_addr = htonl(INADDR_ANY); // INADDR_ANY도 가능

		// 멀티캐스트 그룹 가입
		if (SOCKET_ERROR == setsockopt(m_hReceiveSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)))
		{
			iError = WSAGetLastError();
			TRACE(_T("IP_ADD_MEMBERSHIP error=%d\n"), iError);
			this->ThreadExit();
		}
	}

	return TRUE;
}

BOOL CNetScanOnvif::GenerateMsgID(char* szMessageID, int nBufferLen)
{
	if (szMessageID == NULL || nBufferLen == 0)
		return FALSE;

	GUID guid;
	WCHAR wszGUID[129];

	USES_CONVERSION;
	memset(wszGUID, 0, sizeof(WCHAR)* 129);
	CoCreateGuid(&guid);
	StringFromGUID2(guid, wszGUID, 128); // ex. {D23370B9-3007-47d7-BAEA-30DDD6B0D24B}
	int nGUIDStringLen = wcslen(wszGUID);

	strncpy_s(szMessageID, nBufferLen, W2A(wszGUID) + 1, nGUIDStringLen - 2);

	return TRUE;
}

BOOL CNetScanOnvif::SendScanRequest()
{
	sockaddr_in OnvifSendSock	= { 0 };
	sockaddr_in	multicast_addr	= { 0 };
	SOCKET		hSendSock		= NULL;
	ONVIFINFO*	stOnvifInfo		= NULL;
	DWORD		dwFileSize		= 0;
	const char*	szIpAddr		= "239.255.255.250";
	int			iSize			= 0;
	int			iRevLen = 0;
	char		szMessageID[128] = { 0 };
	
	int			iReuse = 1;
	int			iError = 0;

	enum { ENUM_PROBE_MSG, ENUM_HELLO_MSG };

	const char* g_xmlSchs[2] =
	{
		// probe message
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
			<Envelope xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns=\"http://www.w3.org/2003/05/soap-envelope\">\
			   <Header>\
			      <wsa:MessageID xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">uuid:%s</wsa:MessageID>\
			      <wsa:To xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</ wsa:To>\
			      <wsa:Action xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe</wsa:Action>\
			   </Header>\
			   <Body>\
			      <Probe xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns=  \"http://schemas.xmlsoap.org/ws/2005/04/discovery\">\
			         <Types>dn:NetworkVideoTransmitter</Types>\
			         <Scopes />\
			      </Probe>\
			   </Body>\
			</Envelope>",
		// Hello message
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
		<env:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:env=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\">\
			<env:Header>\
				<wsa:MessageID>uuid:%s</wsa:MessageID>\
				<wsa:To>urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To>\
				<wsa:Action>http://schemas.xmlsoap.org/ws/2005/04/discovery/Hello</wsa:Action>\
				<d:AppSequence InstanceId=\"1077004800\" MessageNumber=\"1\" />\
			</env:Header>\
			<env:Body>\
				<d:Hello>\
					<wsa:EndPointReference>\
					<wsa:Address>uuid:%s</wsa:Address>\
					</wsa:EndPointReference>\
					<d:MetadataVersion>1000</d:MetadataVersion>\
				</d:Hello>\
			</env:Body>\
		</env:Envelope>",
	};
	//pFile = fopen("D:\\C_IPScanner\\IP_Scanner\\Probe.txt", "rb");
	//fseek(pFile, 0, SEEK_END);
	//dwFileSize = ftell(pFile);
	//pszBuff = new char[sizeof(char)*dwFileSize];
	//memset(pszBuff, 0, dwFileSize);
	//fseek(pFile, 0, SEEK_SET);
	//fread(pszBuff, 1, dwFileSize, pFile);
	//stNode.Load(pszBuff);

	if (NULL == m_hReceiveSock)
	{
		CreateSocket();
	}

	////localInterface.s_addr = inet_addr("192.168.0.90");
	//if (SOCKET_ERROR == m_hReceiveSock, IPPROTO_IP, IP_MULTICAST_IF, (char*)&localInterface, sizeof(localInterface))
	//{
	//	iError = WSAGetLastError();
	//	TRACE(_T("SetSocket Error = %d\n", iError));
	//	closesocket(m_hReceiveSock);
	//	return FALSE;
	//}

	if (NULL == m_hReceiveSock)
	{
		return FALSE;
	}


	OnvifSendSock.sin_family = AF_INET;
	OnvifSendSock.sin_port = htons(3702);
	OnvifSendSock.sin_addr.s_addr = inet_addr("239.255.255.250"); // multicast group


	GenerateMsgID(szMessageID, 127);
	char*	pszSendBuffer = new char[4096];
	memset(pszSendBuffer, 0, 4096);

	sprintf_s(pszSendBuffer, 4096, g_xmlSchs[0], szMessageID);

	iSize = strlen(pszSendBuffer);
	if (SOCKET_ERROR == sendto(m_hReceiveSock, pszSendBuffer, iSize, 0, (struct sockaddr*)&OnvifSendSock, sizeof(OnvifSendSock)))
	{
		iError = WSAGetLastError();
		TRACE(_T("sendto Error = %d\n"), iError);
		closesocket(m_hReceiveSock);

		delete[] pszSendBuffer;
		pszSendBuffer = NULL;
		return FALSE;
	}

	m_bConnected = TRUE;

	//socklen_t len = sizeof(OnvifRecvSock);
	//if (0 > getsockname(m_hReceiveSock, (struct sockaddr*)&OnvifRecvSock, &len))
	//{
	//	perror("getSockname Error");
	//}
	//else
	//{
	//	TRACE(_T("%d\n"), ntohs(OnvifRecvSock.sin_port));
	//	m_iRevPort = ntohs(OnvifRecvSock.sin_port);
	//}

	//if (SOCKET_ERROR == setsockopt(hSendSock, SOL_SOCKET, SO_REUSEADDR, (char*)&iReuse, sizeof(iReuse)))
	//{
	//	perror("SO_REUSEADDR error");
	//	this->ThreadExit();
	//}

	//// Local 정보 셋팅
	//OnvifRecvSock.sin_family = AF_INET;
	//OnvifRecvSock.sin_addr.s_addr = INADDR_ANY;
	//OnvifRecvSock.sin_port = htons(m_iRevPort);

	//// 바인드
	//if (SOCKET_ERROR == bind(hSendSock, (struct sockaddr*)&OnvifRecvSock, sizeof(OnvifRecvSock)))
	//{
	//	iError = WSAGetLastError();
	//	TRACE(_T("bind error=%d\n"), iError);
	//	this->ThreadExit();
	//}

	//mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
	//mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	//// 멀티캐스트 그룹 가입
	//if (SOCKET_ERROR == setsockopt(hSendSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)))
	//{
	//	iError = WSAGetLastError();
	//	TRACE(_T("IP_ADD_MEMBERSHIP error=%d\n"), iError);
	//	this->ThreadExit();
	//}

	////if (bIsSuccessBind)
	////{
	//m_pReceive_buffer = new char[SCAN_INFO_m_pReceive_buffer_SIZE];
	//memset(m_pReceive_buffer, 0, sizeof(char)* SCAN_INFO_m_pReceive_buffer_SIZE);

	//while (this->m_dwScanThreadID)
	//{
	//	if (SOCKET_ERROR == recvfrom(hSendSock, m_pReceive_buffer, SCAN_INFO_m_pReceive_buffer_SIZE, 0, (SOCKADDR*)&stSockAddr, &iRevLen))
	//	{
	//		iError = WSAGetLastError();
	//		TRACE(_T("recvfrom error=%d\n"), iError);
	//		this->ThreadExit();
	//	}

	//	TRACE("recvfrom Success\n");
	//}

	delete[] pszSendBuffer;
	pszSendBuffer = NULL;

	return TRUE;
}