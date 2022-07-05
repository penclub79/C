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
	m_iRevPort = ONVIF_PORT;
	this->StartScanF((LPTHREAD_START_ROUTINE)thrOnvifScanThread);
	return TRUE;
}

void CNetScanOnvif::thrOnvifReceiver()
{
	BOOL		bIsSuccessBind = FALSE;
	SOCKADDR	stSockAddr;
	int			iRevLen = 0;
	char*		pszBuff = NULL;
	int			iRecvSock = 0;
	SOCKET		recvSock;
	sockaddr_in	multicast_addr;
	//in_addr		local_addr;
	char*			pszIpAddr = "239.255.255.250";
	char*			pszLocalIp = "192.168.0.90";
	struct		ip_mreq mreq;

	recvSock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&multicast_addr, 0, sizeof(multicast_addr));

	multicast_addr.sin_family = AF_INET;
	multicast_addr.sin_addr.s_addr = inet_addr(pszIpAddr); // 남는 IP를 자동으로 적용해주는 함수
	multicast_addr.sin_port = htons(m_iRevPort);

	mreq.imr_multiaddr = multicast_addr.sin_addr;
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
		memset(m_pReceive_buffer, 0, sizeof(char)*SCAN_INFO_m_pReceive_buffer_SIZE);
		m_pReceive_buffer = (char*)pszBuff;

		while (this->m_dwScanThreadID)
		{
			//if (SOCKET_ERROR == recvfrom(recvSock, m_pReceive_buffer, sizeof(pszBuff), 0, (SOCKADDR*)&stSockAddr, &iRevLen))
			if (SOCKET_ERROR == recvfrom(recvSock, m_pReceive_buffer, sizeof(pszBuff), 0, (SOCKADDR*)&stSockAddr, &iRevLen))
			{
				TRACE("recvfrom error\n");
				this->ThreadExit();
				break;
			}

			if (m_pReceive_buffer)
			{
				TRACE("recvfrom Success\n");
				this->ThreadExit();
				break;
			}
		}
	//}
	
	return;
}

BOOL CNetScanOnvif::SendScanRequest()
{
	sockaddr_in OnvifSendSock;
	BOOL		bEnable			= NULL;
	SOCKET		hSendSock		= NULL;
	ONVIFINFO*	stOnvifInfo		= NULL;
	//FILE*		pFile			= NULL;
	DWORD		dwFileSize		= 0;
	const char*	szIpAddr		= "239.255.255.250";
	int			iSize			= 0;

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
	if (SOCKET_ERROR == setsockopt(hSendSock, SOL_SOCKET, SO_BROADCAST, (char*)&bEnable, sizeof(bEnable)))
	{
		TRACE(_T("SetSocket Error = %d\n", WSAGetLastError()));
		return FALSE;
	}

	OnvifSendSock.sin_family = AF_INET;
	OnvifSendSock.sin_port = htons(m_iRevPort);
	OnvifSendSock.sin_addr.s_addr = inet_addr(szIpAddr); //멀티캐스트

	iSize = strlen(g_xmlSchs[0]);
	//if (SOCKET_ERROR == sendto(hSendSock, g_xmlSchs[0], sizeof(pszBuff), 0, (SOCKADDR*)&OnvifSendSock, sizeof(sockaddr_in)))
	if (SOCKET_ERROR == sendto(hSendSock, g_xmlSchs[0], iSize, 0, (SOCKADDR*)&OnvifSendSock, sizeof(sockaddr_in)))
	{
		TRACE("Send Error");
	}

	return TRUE;
}