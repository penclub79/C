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
	LPXNode		lpBody = NULL;
	LPXNode		lpAddress = NULL;
	XNode		stNode;
	char		aszAddress[127] = { 0 };
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

		// 파싱할 데이터
		stNode.Load(m_pReceive_buffer);
		lpBody = stNode.GetChildArg("SOAP-ENV:Body", NULL);
		if (NULL != lpBody)
		{
			lpAddress = lpBody->GetChildArg("wsa:Address", NULL);
			if (NULL != lpAddress)
			{
				// urn:uuid:XXXXXX-XXXXXX-XXXX 에서 urn:uuid: 문자열을 자름
				lpAddress->value = lpAddress->value.Right(36);
				strcpy(&aszAddress[0], lpAddress->value);
			}
		}

		::OutputDebugStringA(m_pReceive_buffer);
		::OutputDebugStringA("\n");
	}
	

	return;
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
	sockaddr_in OnvifSendSock		= { 0 };
	sockaddr_in	multicast_addr		= { 0 };
	SOCKET		hSendSock			= NULL;
	ONVIFINFO*	stOnvifInfo			= NULL;
	DWORD		dwFileSize			= 0;
	int			iSize				= 0;
	char		szMessageID[128]	= { 0 };
	int			iReuse				= 1;
	int			iError				= 0;

	enum { ENUM_PROBE_MSG, ENUM_HELLO_MSG };

	const char* g_xmlSchs[4] =
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
			// resolve
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
			<env:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:env=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\">\
			   <env:Header>\
				  <wsa:MessageID>uuid:%s</wsa:MessageID>\
				  <wsa:To>urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To>\
				  <wsa:Action>http://schemas.xmlsoap.org/ws/2005/04/discovery/Resolve</wsa:Action>\
			   </env:Header>\
			   <env:Body>\
				  <d:Resolve>\
					 <wsa:EndPointReference>\
						<wsa:Address>uuid:%s</wsa:Address>\
					 </wsa:EndPointReference>\
				  </d:Resolve>\
			   </env:Body>\
			</env:Envelope>",
		// GetDeviceInformation
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
			<env:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:env=\"http://www.w3.org/2003/05/soap-envelope\">\
				<env:Header>\
					<wsa:Action>http://www.onvif.org/ver10/device/wsdl/GetDeviceInformation</wsa:Action>\
				</env:Header>\
				<env:Body>\
					<dn:GetDeviceInformation/>\
				</env:Body>\
			</env:Envelope>",
		// Cap
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
			<Envelope xmlns:dn=\"http://www.w3.org/2003/05/soap-envelope\" xmlns : tds =\"http://www.onvif.org/ver10/device/wsdl\">\
			<Body>\
				<tds:GetCapabilities>\
					<tds:Category>Device</tds:Category>\
				</tds:GetCapabilities>\
			</Body>\
			</Envelope>",
	};
	//pFile = fopen("D:\\C_IPScanner\\IP_Scanner\\Probe.txt", "rb");
	//fseek(pFile, 0, SEEK_END);
	//dwFileSize = ftell(pFile);
	//pszBuff = new char[sizeof(char)*dwFileSize];
	//memset(pszBuff, 0, dwFileSize);
	//fseek(pFile, 0, SEEK_SET);
	//fread(pszBuff, 1, dwFileSize, pFile);
	//stNode.Load(pszBuff);


	// 소켓 생성
	if (NULL == m_hReceiveSock)
	{
		CreateSocket();
	}

	if (NULL == m_hReceiveSock)
	{
		return FALSE;
	}

	OnvifSendSock.sin_family = AF_INET;
	OnvifSendSock.sin_port = htons(3702);
	OnvifSendSock.sin_addr.s_addr = inet_addr("239.255.255.250"); // multicast group

	// UUID 얻기
	GenerateMsgID(szMessageID, 127);
	char*	pszSendBuffer = new char[4000];
	memset(pszSendBuffer, 0, 4000);

	// 얻어온 UUID를 XML에 적용
	sprintf_s(pszSendBuffer, 4000, g_xmlSchs[0], szMessageID);

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

	if (NULL != pszSendBuffer)
	{
		delete[] pszSendBuffer;
		pszSendBuffer = NULL;
	}
	
	return TRUE;
}