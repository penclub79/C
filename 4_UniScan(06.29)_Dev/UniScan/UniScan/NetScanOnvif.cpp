#include "./Stdafx.h"
#include "NetScanOnvif.h"
#include <atlconv.h>
#include "xmlite\XMLite.h"

CNetScanOnvif::CNetScanOnvif()
{
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
	SOCKET		recvSock;
	sockaddr_in	multicast_addr, other_addr;
	int			iRevLen = sizeof(other_addr);
	//in_addr		local_addr;
	struct		ip_mreq mreq;
	//ONVIFINFO*	pReceive		= NULL;
	LPXNode lpHeader = NULL;
	LPXNode lpBody = NULL;
	LPXNode lpBodyCommon = NULL;
	LPXNode lpItemData = NULL;
	XNode	stNode;


	recvSock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&multicast_addr, 0, sizeof(multicast_addr));

	multicast_addr.sin_family = AF_INET;
	multicast_addr.sin_addr.s_addr = inet_addr("192.168.0.90"); // 남는 IP를 자동으로 적용해주는 함수
	//multicast_addr.sin_port = htons(0);
	multicast_addr.sin_port = htons(m_iRevPort);

	mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	// 멀티캐스트 그룹 가입
	if (SOCKET_ERROR == setsockopt(recvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)))
	{
		TRACE("IP_ADD_MEMBERSHIP error\n");
	}

	if (SOCKET_ERROR == bind(recvSock, (struct sockaddr*)&multicast_addr, sizeof(multicast_addr)))
	{
		TRACE("bind error\n");
	}
	//bIsSuccessBind = SocketBind();

	//if (bIsSuccessBind)
	//{
	m_pReceive_buffer = new char[SCAN_INFO_m_pReceive_buffer_SIZE];
	//pReceive = (ONVIFINFO*)m_pReceive_buffer;
	memset(m_pReceive_buffer, 0, sizeof(char)* SCAN_INFO_m_pReceive_buffer_SIZE);


	while (this->m_dwScanThreadID)
	{
		//if (SOCKET_ERROR == recvfrom(recvSock, m_pReceive_buffer, sizeof(pszBuff), 0, (SOCKADDR*)&stSockAddr, &iRevLen))
		if (SOCKET_ERROR == recvfrom(recvSock, m_pReceive_buffer, sizeof(SCAN_INFO_m_pReceive_buffer_SIZE), 0, (SOCKADDR*)&stSockAddr, &iRevLen))
		{
			TRACE("recvfrom error\n");
			this->ThreadExit();
			break;
		}
		
			TRACE("recvfrom Success\n");
	}
	//}

	return;


	// UDP 소켓 생성
	recvSock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&multicast_addr, 0, sizeof(multicast_addr));

	// 멀티 캐스트 주소 정보 초기화
	multicast_addr.sin_family = AF_INET;
	multicast_addr.sin_addr.s_addr = inet_addr("192.168.0.90"); // 남는 IP를 자동으로 적용해주는 함수
	multicast_addr.sin_port = htons(multicast_addr.sin_port);
	
	if (SOCKET_ERROR == bind(recvSock, (struct sockaddr*)&multicast_addr, sizeof(multicast_addr)))
	{
		TRACE("bind error\n");
	}

	// 가입
	mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	// 멀티캐스트 그룹 가입
	if (SOCKET_ERROR == setsockopt(recvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)))
	{
		TRACE("IP_ADD_MEMBERSHIP error\n");
	}


	//if (bIsSuccessBind)
	//{
		m_pReceive_buffer = new char[SCAN_INFO_m_pReceive_buffer_SIZE];
		memset(m_pReceive_buffer, 0, sizeof(char)*SCAN_INFO_m_pReceive_buffer_SIZE);

		while (this->m_dwScanThreadID)
		{
			//if (SOCKET_ERROR == recvfrom(recvSock, m_pReceive_buffer, sizeof(pszBuff), 0, (SOCKADDR*)&stSockAddr, &iRevLen))
			if (SOCKET_ERROR == recvfrom(recvSock, m_pReceive_buffer, sizeof(SCAN_INFO_m_pReceive_buffer_SIZE), 0, (SOCKADDR*)&other_addr, &iRevLen))
			{
				TRACE("recvfrom error\n");
				this->ThreadExit();
				break;
			}

			
			TRACE("recvfrom Success\n");
		}
	//}
	
	return;
}

BOOL CNetScanOnvif::SendScanRequest()
{
	sockaddr_in OnvifSendSock = { 0 }, OnvifRecvSock = { 0 };
	int			iEnable			= 1;
	SOCKET		hSendSock		= NULL;
	ONVIFINFO*	stOnvifInfo		= NULL;
	DWORD		dwFileSize		= 0;
	const char*	szIpAddr		= "239.255.255.250";
	int			iSize			= 0;
	int			iRevLen = 0;

	enum { ENUM_PROBE_MSG, ENUM_HELLO_MSG };

	const char* g_xmlSchs[2] =
	{
		// probe message
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
			<Envelope xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns=\"http://www.w3.org/2003/05/soap-envelope\">\
			   <Header>\
			      <wsa:MessageID xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">uuid:5e1cec36-03b9-4d8b-9624-0c5283982a00</wsa:MessageID>\
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
				<wsa:MessageID>uuid:f9d137b3-95d9-45f2-badd-1d70f1c140a2</wsa:MessageID>\
				<wsa:To>urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To>\
				<wsa:Action>http://schemas.xmlsoap.org/ws/2005/04/discovery/Hello</wsa:Action>\
				<d:AppSequence InstanceId=\"1077004800\" MessageNumber=\"1\" />\
			</env:Header>\
			<env:Body>\
				<d:Hello>\
					<wsa:EndPointReference>\
					<wsa:Address>uuid:98190dc2-0890-4ef8-ac9a-5940995e6119</wsa:Address>\
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

	hSendSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (SOCKET_ERROR == setsockopt(hSendSock, SOL_SOCKET, SO_BROADCAST, (const char*)&iEnable, sizeof(int)))
	{
		TRACE(_T("SetSocket Error = %d\n", WSAGetLastError()));
		closesocket(hSendSock);
		return FALSE;
	}

	OnvifSendSock.sin_family = AF_INET;
	OnvifSendSock.sin_port = htons(3702);
	OnvifSendSock.sin_addr.s_addr = inet_addr(szIpAddr);

	iSize = strlen(g_xmlSchs[0]);
	if (SOCKET_ERROR == sendto(hSendSock, g_xmlSchs[0], iSize, 0, (struct sockaddr*)&OnvifSendSock, sizeof(sockaddr_in)))
	{
		TRACE("Send Error");
		closesocket(hSendSock);
		return FALSE;
	}
	socklen_t len = sizeof(OnvifRecvSock);
	if (-1 == getsockname(hSendSock, (struct sockaddr*)&OnvifRecvSock, &len))
	{
		perror("getSockname Error");
	}
	else
	{
		TRACE(_T("%d"), ntohs(OnvifRecvSock.sin_port));
		m_iRevPort = ntohs(OnvifRecvSock.sin_port);
	}


	return TRUE;
}