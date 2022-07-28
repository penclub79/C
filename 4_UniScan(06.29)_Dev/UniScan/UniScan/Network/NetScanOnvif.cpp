#include "../Stdafx.h"
#include "NetScanOnvif.h"
#include <atlconv.h>
#include "../xmlite/XMLite.h"
#include <time.h>
#include <sys/timeb.h>

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
		if (SOCKET_ERROR == setsockopt(m_hReceiveSock, IPPROTO_IP, IP_MULTICAST_IF, (char*)&localInterface, sizeof(localInterface)))
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

void CNetScanOnvif::thrOnvifReceiver()
{
	BOOL		bIsSuccessBind		= FALSE;
	SOCKADDR	stSockAddr;
	int			iRevLen				= sizeof(sockaddr_in);
	XNode		stNode;
	LPXNode		lpBody				= NULL;
	LPXNode		lpUUID				= NULL;
	LPXNode		lpIPAddress			= NULL;
	int			iHTTPPort			= 0;
	char		aszUUID[128]		= { 0 };
	char		aszData[128]		= { 0 };
	char		aszIPAddress[32]	= { 0 };
	char*		pszSlice			= NULL;
	DWORD		dwLastError			= 0;
	SCAN_INFO*	pScanInfo			= NULL;

	m_pReceive_buffer = new char[SCAN_INFO_RECEIVE_BUFFER_SIZE];
	memset(m_pReceive_buffer, 0, sizeof(char)* SCAN_INFO_RECEIVE_BUFFER_SIZE);

	while (this->m_dwScanThreadID)
	{
		if (NULL == m_hReceiveSock || FALSE == m_bConnected)
		{
			Sleep(100);
			continue;
		}
		
		if (SOCKET_ERROR == recvfrom(m_hReceiveSock, m_pReceive_buffer, SCAN_INFO_RECEIVE_BUFFER_SIZE, 0, (SOCKADDR*)&stSockAddr, &iRevLen))
		{
			dwLastError = WSAGetLastError();
			TRACE(_T("recvfrom error=%d\n"), dwLastError);

			if (this->m_hNotifyWnd && dwLastError != WSAEINTR && dwLastError != WSAEINVAL)
			{
				::PostMessage(this->m_hNotifyWnd, this->m_lNotifyMsg, 0, SCAN_ERR_RECV);
			}

			this->ThreadExit();
			break;
		}

		// 파싱할 데이터
		stNode.Load(m_pReceive_buffer);
		lpBody = stNode.GetChildArg("SOAP-ENV:Body", NULL);
		if (NULL != lpBody)
		{
			lpUUID = lpBody->GetChildArg("wsa:Address", NULL);
			if (NULL != lpUUID)
			{
				// urn:uuid:XXXXXX-XXXXXX-XXXX 에서 urn:uuid: 문자열을 자름
				lpUUID->value = lpUUID->value.Right(36);
				strcpy(&aszUUID[0], lpUUID->value);
				// 배열에 uuid 쌓고 다 쌓았으면 uuid를Resolve Message에 담아서 다시 Send
			}
		}

		// table input Data
		pScanInfo = new SCAN_INFO;
		if (pScanInfo)
		{
			memset(pScanInfo, 0, sizeof(SCAN_INFO));
			// IP Parsing
			//lpBody = stNode.GetChildArg("wsdd:ProbeMatch", NULL);
			lpBody = stNode.GetChildArg("d:ProbeMatch", NULL);
			if (NULL != lpBody)
			{
				//lpIPAddress = lpBody->GetChildArg("wsdd:XAddrs", NULL);
				lpIPAddress = lpBody->GetChildArg("d:XAddrs", NULL);

				if (NULL != lpIPAddress)
				{
					int iIndex = 0;
					strcpy(&aszData[0], lpIPAddress->value);
					pszSlice = strtok(aszData, ":");
					while (NULL != pszSlice)
					{
						pszSlice = strtok(NULL, ":");
						if (1 == iIndex)
							break;
						
						iIndex++;
					}
					if (pszSlice)
					{
						pszSlice = strtok(pszSlice, "/");
						iHTTPPort = atoi(pszSlice);
						pScanInfo->nHTTPPort = iHTTPPort;
					}

					lpIPAddress->value = lpIPAddress->value.Left(20);
					lpIPAddress->value = lpIPAddress->value.Right(13);
					strcpy(&aszIPAddress[0], lpIPAddress->value);
					
					this->WideCopyStringFromAnsi(pScanInfo->szAddr, 32, aszIPAddress);
					wsprintf(pScanInfo->szGateWay,		_T("N/A"));
					wsprintf(pScanInfo->szMAC,			_T("N/A"));
					wsprintf(pScanInfo->szModelName,	_T("N/A"));
					wsprintf(pScanInfo->szSwVersion,	_T("N/A"));
					pScanInfo->iBasePort = 0;
					pScanInfo->iVideoCnt = 0;
					
					SendAuthentication(aszIPAddress);

					if (this->m_hNotifyWnd)
					{
						::PostMessage(this->m_hNotifyWnd, this->m_lNotifyMsg, (WPARAM)pScanInfo, 0);
					}
				}
			}
		}

		//::OutputDebugStringA(m_pReceive_buffer);
		//::OutputDebugStringA("\n");

	}
	
	if (NULL != pScanInfo)
	{
		delete pScanInfo;
		pScanInfo = NULL;
	}

	return;
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
	int			iError				= 0;
	char*		pszSendBuffer		= NULL;
	char*		pszOnvifURI[2]		= {"http://www.onvif.org/ver10/network/wsdl", "http://www.onvif.org/ver10/device/wsdl"};
	char*		pszProbeType[3]		= { "dp0:NetworkVideoTransmitter", "dp0:NetworkVideoDisplay", "dp0:Device" };

	// probe message
	const char* g_xmlSchs =
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
			<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:a=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">\
				<s:Header>\
					<a:Action s:mustUnderstand=\"1\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe</a:Action>\
					<wsa:MessageID xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">uuid:%s</wsa:MessageID>\
					<a:ReplyTo>\
						<a:Address>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</a:Address>\
					</a:ReplyTo>\
					<a:To s:mustUnderstand=\"1\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</a:To>\
				</s:Header>\
				<s:Body>\
					<Probe xmlns=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\">\
						<d:Types xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:dp0=%s>%s</Types>\
					</Probe>\
				</s:Body>\
			</Envelope>";
	
	// 소켓 생성
	if (NULL == m_hReceiveSock)
		CreateSocket();

	if (NULL == m_hReceiveSock)
		return FALSE;

	OnvifSendSock.sin_family = AF_INET;
	OnvifSendSock.sin_port = htons(3702);
	OnvifSendSock.sin_addr.s_addr = inet_addr("239.255.255.250"); // multicast group

	pszSendBuffer = new char[4000];
	memset(pszSendBuffer, 0, 4000);

	int iTypeIdx = 0;
	int iUriIdx = 0;
	// Probe Type 배열 요소 갯수 만큼 Packet Send
	while (iTypeIdx < sizeof(pszProbeType) / sizeof(pszProbeType[0]))
	{
		// UUID 얻기
		GenerateMsgID(szMessageID, 127);

		if (iTypeIdx > 1)
			iUriIdx++;

		// 얻어온 UUID를 XML에 적용
		sprintf_s(pszSendBuffer, 4000, g_xmlSchs, szMessageID, pszOnvifURI[iUriIdx], pszProbeType[iTypeIdx]);

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
		
		iTypeIdx++;
	}
	

	m_bConnected = TRUE;

	//SendSSDP();

	if (NULL != pszSendBuffer)
	{
		delete[] pszSendBuffer;
		pszSendBuffer = NULL;
	}
	
	return TRUE;
}

BOOL CNetScanOnvif::SendAuthentication(char* pszIP)
{
	char*		pszSendBuffer		= NULL;
	char		aszRecvBuffer[4096] = { 0 };
	char		aszSendBuffer[1024] = { 0 };
	char		szMessageID[128]	= { 0 };
	int			iSendDataSize		= 0;
	int			iError				= 0;
	sockaddr_in HTTPSendSock		= { 0 };
	SOCKET		TcpSock;
	XNode		stNode;
	LPXNode		lpBody				= NULL;
	char		aszTime[32]			= { 0 };
	
	char aszReqHTTP[1024] = "POST /onvif/device_service HTTP/1.1\r\n\Content-Type: application/soap+xml; charset=utf-8; action=\"http://www.onvif.org/ver10/device/wsdl/GetSystemDateAndTime\"\r\nHost: %s\r\nContent-Length: %d\r\nAccept-Encoding: gzip, deflate\r\nConnection: Close\r\n\r\n";

	char aszXmlSchs[500] = 
	{
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
		<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\
			<s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
				<GetSystemDateAndTime xmlns=\"http://www.onvif.org/ver10/device/wsdl\"/>\
			</s:Body>\
		</s:Envelope>"
	};

	sprintf_s(aszSendBuffer, 1024, aszReqHTTP, pszIP, strlen(aszXmlSchs));
	strcat(aszSendBuffer, aszXmlSchs);
	
	TcpSock = socket(PF_INET, SOCK_STREAM, 0);

	HTTPSendSock.sin_family = AF_INET;
	HTTPSendSock.sin_port = htons(80);
	HTTPSendSock.sin_addr.s_addr = inet_addr(pszIP);

	if (SOCKET_ERROR == connect(TcpSock, (SOCKADDR*)&HTTPSendSock, sizeof(SOCKADDR)))
	{
		iError = WSAGetLastError();
		TRACE(_T("TCP-HTTP Connect Error = %d\n"), iError);
		closesocket(TcpSock);

		return FALSE;
	}

	iSendDataSize = strlen(aszSendBuffer);
	if (SOCKET_ERROR == send(TcpSock, aszSendBuffer, iSendDataSize, 0))
	{
		iError = WSAGetLastError();
		TRACE(_T("TCP-HTTP send Error = %d\n"), iError);
		closesocket(TcpSock);

		return FALSE;
	}

	if (SOCKET_ERROR == recv(TcpSock, aszRecvBuffer, sizeof(char) * 4096, 0))
	{
		iError = WSAGetLastError();
		TRACE(_T("TCP-HTTP recv Error = %d\n"), iError);
		closesocket(TcpSock);

		return FALSE;
	}
	::OutputDebugStringA("ONVIF DEVICE DATA -----------------------\n");
	::OutputDebugStringA(pszIP);
	::OutputDebugStringA("\n");
	::OutputDebugStringA(aszRecvBuffer);
	::OutputDebugStringA("\n");

	if ( 0 != strlen(aszRecvBuffer) )
	{
		LPXNode		lpTime;
		LPXNode		lpDate;
		LPXNode		lpYear;
		LPXNode		lpMonth;
		LPXNode		lpDay;
		LPXNode		lpHour;
		LPXNode		lpMinute;
		LPXNode		lpSecond;

		stNode.Load(aszRecvBuffer);
		lpBody = stNode.GetChildArg("tt:UTCDateTime", NULL);
		
		if (lpBody)
		{
			lpDate = lpBody->GetChildArg("tt:Date", NULL);
			lpYear = lpDate->GetChildArg("tt:Year", NULL);
			lpMonth = lpDate->GetChildArg("tt:Month", NULL);
			lpDay = lpDate->GetChildArg("tt:Day", NULL);

			lpTime = lpBody->GetChildArg("tt:Time", NULL);
			lpHour = lpTime->GetChildArg("tt:Hour", NULL);
			lpMinute = lpTime->GetChildArg("tt:Minute", NULL);
			lpSecond = lpTime->GetChildArg("tt:Second", NULL);

			sprintf_s(aszTime, sizeof(aszTime), "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ",
				_ttoi(lpYear->value.GetBuffer),
				_ttoi(lpMonth->value.GetBuffer),
				_ttoi(lpDay->value.GetBuffer),
				_ttoi(lpHour->value.GetBuffer),
				_ttoi(lpMinute->value.GetBuffer),
				_ttoi(lpSecond->value.GetBuffer),
				000
				);
		}
		
		

	}

	//struct timeb stMilliTime;
	//struct tm* pstTime;
	//char aszTime[32] = { 0 };

	//ftime(&stMilliTime);
	//pstTime = localtime(&stMilliTime.time);

	////stTime = time(NULL);
	////pstGlobalTime = gmtime(&stTime);
	////
	//sprintf_s(aszTime, sizeof(aszTime), "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ",
	//	pstTime->tm_year + 1900,
	//	pstTime->tm_mon + 1,
	//	pstTime->tm_mday,
	//	pstTime->tm_hour - 9,
	//	pstTime->tm_min,
	//	pstTime->tm_sec,
	//	stMilliTime.millitm / 10
	//	);
	

	return TRUE;
}


//BOOL CNetScanOnvif::SendSSDP()
//{
//	sockaddr_in HTTPSendSock		= { 0 };
//	SOCKET		TcpSock;
//	SOCKADDR	stSockAddr;
//	int			iRevLen				= sizeof(sockaddr_in);
//	char*		pszHost				= "239.255.255.250";
//	//char*		pszHost				= "192.168.0.199";
//	int			iPort				= 1900;
//	int			iError				= 0;
//	int			iSendDataSize		= 0;
//	char		aszRecvBuffer[4096] = { 0 };
//	char		aszSendBuffer[1024]	= { 0 };
//
//	char* aszReqSSDP = "M-SEARCH * HTTP/1.1\r\n\HOST: %s:%d\r\nMAN: \"ssdp:discover\"\r\nMX: 1\r\nST: urn:dial-multiscreen-org:service:dial:1\r\nUSER-AGENT: Google Chrome/103.0.5060.134 Windows\r\n\r\n";
//
//	sprintf_s(aszSendBuffer, sizeof(char) * 1024, aszReqSSDP, pszHost, iPort);
//
//	TcpSock = socket(AF_INET, SOCK_DGRAM, 0);
//	HTTPSendSock.sin_family = AF_INET;
//	HTTPSendSock.sin_port = htons(3702);
//	HTTPSendSock.sin_addr.s_addr = inet_addr(pszHost);
//
//	iSendDataSize = strlen(aszSendBuffer);
//	if (SOCKET_ERROR == sendto(TcpSock, aszSendBuffer, iSendDataSize, 0, (struct sockaddr*)&HTTPSendSock, sizeof(HTTPSendSock)))
//	{
//		iError = WSAGetLastError();
//		TRACE(_T("TCP-HTTP send Error = %d\n"), iError);
//		closesocket(TcpSock);
//
//		return FALSE;
//	}
//
//	while (this->m_dwScanThreadID)
//	{
//		if (SOCKET_ERROR == recvfrom(TcpSock, aszRecvBuffer, sizeof(char)* 4096, 0, (SOCKADDR*)&stSockAddr, &iRevLen))
//		{
//			iError = WSAGetLastError();
//			TRACE(_T("TCP-HTTP recv Error = %d\n"), iError);
//			closesocket(TcpSock);
//
//			return FALSE;
//		}
//
//		::OutputDebugStringA(aszRecvBuffer);
//		::OutputDebugStringA("\n");
//	}
//	return TRUE;
//}