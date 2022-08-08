#include "../Stdafx.h"
#include "NetScanOnvif.h"
#include <atlconv.h>
#include "../xmlite/XMLite.h"
#include <time.h>
#include <sys/timeb.h>
#include <openssl/sha.h>
//#include <openssl/md5.h>
#include <time.h>


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
	BOOL		bIsSuccessBind			= FALSE;
	SOCKADDR	stSockAddr;
	int			iRevLen					= sizeof(sockaddr_in);
	XNode		stNode;
	LPXNode		lpTypeCheck				= NULL;
	LPXNode		lpBody					= NULL;
	LPXNode		lpUUID					= NULL;
	LPXNode		lpIPAddress				= NULL;
	LPXNode		lpScope					= NULL;
	int			iHTTPPort				= 0;
	char		aszUUID[128]			= { 0 };
	char		aszIPData[128]			= { 0 };
	char*		pszData					= NULL;
	char		aszIPAddress[32]		= { 0 };
	char*		pszTestIP				= "192.168.0.135";
	char*		pszSlice				= NULL;
	char*		pszNameSlice			= NULL;
	char*		pszMacSlice				= NULL;
	DWORD		dwLastError				= 0;
	SCAN_INFO*	pScanInfo				= NULL;
	char*		pszProbeMatchType[2]	= { "wsdd:ProbeMatch", "d:ProbeMatch" };
	char*		pszAddressType[2]		= { "wsdd:XAddrs", "d:XAddrs" };

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
				::PostMessage(this->m_hNotifyWnd, this->m_lNotifyMsg, 0, SCAN_ERR_RECV);
			
			this->ThreadExit();
			break;
		}

		if ( 0 < strlen(m_pReceive_buffer) )
		{
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

				lpTypeCheck = ( NULL != stNode.GetChildArg("wsdd:ProbeMatch", NULL) ) ? stNode.GetChildArg("wsdd:ProbeMatch", NULL) : stNode.GetChildArg("d:ProbeMatch", NULL);

				if (NULL != lpTypeCheck)
				{
					lpIPAddress = ( NULL != lpTypeCheck->GetChildArg("wsdd:XAddrs", NULL) ) ? lpTypeCheck->GetChildArg("wsdd:XAddrs", NULL) : lpTypeCheck->GetChildArg("d:XAddrs", NULL);

					// IP Address
					if (NULL != lpIPAddress)
					{
						int iIndex = 0;
						strcpy(&aszIPData[0], lpIPAddress->value);
						pszSlice = strtok(aszIPData, ":");
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
							if (10 > strlen(pszSlice))
							{
								iHTTPPort = atoi(pszSlice);
								pScanInfo->nHTTPPort = iHTTPPort;
							}
						}

						lpIPAddress->value = lpIPAddress->value.Left(20);
						lpIPAddress->value = lpIPAddress->value.Right(13);
						strcpy(&aszIPAddress[0], lpIPAddress->value);

						if (strlen(aszIPAddress) > 0)
						{
							this->WideCopyStringFromAnsi(pScanInfo->szAddr, 32, aszIPAddress);
						}

						SendAuthentication(pszTestIP);
					}

					lpScope = ( NULL != lpTypeCheck->GetChildArg("wsdd:Scopes", NULL) ) ? lpTypeCheck->GetChildArg("wsdd:Scopes", NULL) : lpTypeCheck->GetChildArg("d:Scopes", NULL);

					if (NULL != lpScope)
					{

						pszData = new char[sizeof(char) * lpScope->value.GetLength() + 1];
						strcpy(&pszData[0], lpScope->value);
						::OutputDebugStringA(pszData);
						::OutputDebugStringA("\n");
						pszNameSlice = strstr(pszData, "name"); // return NULL
						pszMacSlice = ( NULL != strstr(pszData, "macaddress" ) ? strstr(pszData, "macaddress") : strstr(pszData, "mac") );

						if (NULL != pszNameSlice)
						{
							pszNameSlice = strtok(pszNameSlice, " ");
							pszNameSlice = strncpy(pszNameSlice, pszNameSlice + 5, 20);

							this->WideCopyStringFromAnsi(pScanInfo->szModelName, 30, pszNameSlice);
						}
						else
							wsprintf(pScanInfo->szModelName, _T("N/A"));

						if (NULL != pszMacSlice)
						{
							char* pszMacType = NULL;

							pszMacSlice = strtok(pszMacSlice, " ");
							pszMacType = strtok(pszMacSlice, "/");
							( 0 == strcmp(pszMacType, "macaddress") ) ? pszMacSlice = strncpy(pszMacSlice, pszMacSlice + 11, 20) : pszMacSlice = strncpy(pszMacSlice, pszMacSlice + 4, 20);

							strlwr(pszMacSlice); // 소문자로 변환

							this->WideCopyStringFromAnsi(pScanInfo->szMAC, 32, pszMacSlice);
						}
						else
							wsprintf(pScanInfo->szMAC, _T("N/A"));

					}
					wsprintf(pScanInfo->szSwVersion, _T("N/A"));
					wsprintf(pScanInfo->szGateWay,	_T("N/A"));
					pScanInfo->iBasePort = 0;
					pScanInfo->iVideoCnt = 0;

					if (this->m_hNotifyWnd)
						::PostMessage(this->m_hNotifyWnd, this->m_lNotifyMsg, (WPARAM)pScanInfo, 0);

				}
			}

			::OutputDebugStringA(m_pReceive_buffer);
			::OutputDebugStringA("\n");
		}

		if (NULL != pszData)
		{
			delete pszData;
			pszData = NULL;
		}
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
	char*		paszOnvifURI[2]		= {"http://www.onvif.org/ver10/network/wsdl", "http://www.onvif.org/ver10/device/wsdl"};
	char*		paszProbeType[3]		= { "dp0:NetworkVideoTransmitter", "dp0:NetworkVideoDisplay", "dp0:Device" };

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

	pszSendBuffer = new char[4096];
	memset(pszSendBuffer, 0, 4096);

	int iTypeIdx = 0;
	int iUriIdx = 0;
	// Probe Type 배열 요소 갯수 만큼 Packet Send
	while (iTypeIdx < sizeof(paszProbeType) / sizeof(paszProbeType[0]))
	{
		// UUID 얻기
		GenerateMsgID(szMessageID, 127);

		if (iTypeIdx > 1)
			iUriIdx++;

		// 얻어온 UUID를 XML에 적용
		sprintf_s(pszSendBuffer, 4096, g_xmlSchs, szMessageID, paszOnvifURI[iUriIdx], paszProbeType[iTypeIdx]);

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
	SOCKET				TcpSock;
	XNode				stNode;
	LPXNode				lpBody				= NULL;
	char*				pszSendBuffer		= NULL;
	char				aszRecvBuffer[4096] = { 0 };
	char				aszSendBuffer[1024] = { 0 };
	char				szMessageID[128]	= { 0 };
	int					iSendDataSize		= 0;
	int					iError				= 0;
	sockaddr_in			HTTPSendSock		= { 0 };
	char				aszTime[32]			= { 0 };
	char*				pszSendAuthBuff		= NULL;
	char*				pszUserID			= "admin" ;
	char*				pszUserPWD			= "111111" ;
	unsigned char		pszDigest[16]		= { 0 };
	char				aszNow[32]			= { 0 };
	char				aszNonce[128]		= { 0 };
	time_t Timer;

	//char aszGetSystemTime[1024] = "POST /onvif/device_service HTTP/1.1\r\nContent-Type: application/soap+xml; charset=utf-8; action=\"http://www.onvif.org/ver10/device/wsdl/GetSystemDateAndTime\"\r\nHost: %s\r\nContent-Length: %d\r\nAccept-Encoding: gzip, deflate\r\nConnection: Close\r\n\r\n";

	char* paszXmlSchs[2] = 
	{
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
		<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\
			<s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
				<GetSystemDateAndTime xmlns=\"http://www.onvif.org/ver10/device/wsdl\"/>\
			</s:Body>\
		</s:Envelope>", // GetSystemDate
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
			<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\
				<s:Header>\
					<Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\">\
						<UsernameToken>\
							<Username>admin</Username>\
							<Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">111111</Password>\
							<Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">AuofUoRWc0GO+tcVcscxpjYBAAAAAA==</Nonce>\
							<Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">2022-08-08T03:26:36.138Z</Created>\
						</UsernameToken>\
					</Security>\
				</s:Header>\
				<s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
					<GetDeviceInformation xmlns=\"http://www.onvif.org/ver10/device/wsdl\"/>\
				</s:Body>\
			</s:Envelope>" // GetDeviceInfomation
	};

	//sprintf_s(aszSendBuffer, 1024, aszGetSystemTime, pszIP, strlen(paszXmlSchs[0]));
	//strcat_s(aszSendBuffer, sizeof(char) * strlen(aszSendBuffer) + strlen(paszXmlSchs[0]) + 1, paszXmlSchs[0]);
	char aszRequest[1024] = "POST /onvif/Events HTTP/1.1\r\nContent-Type: application/soap+xml; charset=utf-8\r\nHost: %s\r\nContent-Length: %d\r\n\r\n";
	sprintf_s(aszSendBuffer, 1024, aszRequest, pszIP, strlen(paszXmlSchs[0]));
	strcat_s(aszSendBuffer, sizeof(char)* strlen(aszSendBuffer) + strlen(paszXmlSchs[0]) + 1, paszXmlSchs[0]);

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

	time(&Timer);
	sprintf_s(aszNow, sizeof(aszNow), "%s", ctime(&Timer)); // \n도 같이 출력된다.
	Base64Encoding(aszNow, strlen(aszNow), &aszNonce[0]); // Nonce 생성 -> Base64[현재 날짜] Encoding
	
	if ( 0 != strlen(aszRecvBuffer) )
	{
		LPXNode		lpaDateType[2]	= { 0 }; // CStringA
		LPXNode		lpaTimeData[6]	= { 0 };
		char*		paiDate[6]		= { 0 };
		char*		paszDateType[2] = { "tt:Date", "tt:Time" };
		char*		paszChild[6]	= { "tt:Year", "tt:Month", "tt:Day", "tt:Hour", "tt:Minute", "tt:Second" };

		stNode.Load(aszRecvBuffer);
		lpBody = stNode.GetChildArg("tt:UTCDateTime", NULL);
		
		if (lpBody)
		{
			int index = 0;
			int iArrLen = sizeof(lpaTimeData) / sizeof(lpaTimeData[0]);

			while (index < iArrLen)
			{
				// 1~3 Date , 4~6 Time
				if (index < iArrLen / 2)
					lpaDateType[index] = lpBody->GetChildArg(paszDateType[0], NULL);
				else
					lpaDateType[index] = lpBody->GetChildArg(paszDateType[1], NULL);
		
				lpaTimeData[index] = lpaDateType[index]->GetChildArg(paszChild[index], NULL);
				
				if (NULL != lpaTimeData[index])
				{
					paiDate[index] = lpaTimeData[index]->value.GetBuffer(0);
				}

				index++;
			}
			sprintf_s(aszTime, sizeof(aszTime), "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ",
					atoi(paiDate[0]),
					atoi(paiDate[1]),
					atoi(paiDate[2]),
					atoi(paiDate[3]),
					atoi(paiDate[4]),
					atoi(paiDate[5]),
					000
					);
		}
	}

	// 다이제스트
	DigestConvert(pszUserPWD, &pszDigest[0]);

	//int BuffSize = strlen(paszXmlSchs[0]) + strlen(pszUserID) + strlen() + strlen() + strlen(aszTime);
	//pszSendAuthBuff = new char[BuffSize + 1];

	//if (NULL != pszSendAuthBuff)
	//{
	//	delete pszSendAuthBuff;
	//	pszSendAuthBuff = NULL;
	//}
	
	return TRUE;
}


void CNetScanOnvif::DigestConvert(char* pszStr, unsigned char* puszResult)
{
	char aszDigest[MAX_PATH] = { 0 };
	char aszOutput[16] = { 0 };

	MD5Encoding(pszStr, strlen(pszStr), aszOutput);
	Base64Encoding(aszOutput, strlen(aszOutput), &aszDigest[0]);
	strcpy((char*)puszResult, aszDigest);
}

void CNetScanOnvif::MD5Encoding(char* pszStr, int iSize, char* pszResult)
{
	SHA_CTX			stSHA1;
	//MD5_CTX			stMD5;
	int				iLen = 0;
	wchar_t			awszUnicode[MAX_PATH]			= { 0 };	// Unicode
	char			aszUtf8[MAX_PATH]				= { 0 };	// UTF-8 
	char			aszSHA1Value[SHA_DIGEST_LENGTH] = { 0 };	// SHA1
	//unsigned char	auszMD5Value[MD5_DIGEST_LENGTH] = { 0 };
	unsigned char	auszHash[SHA_DIGEST_LENGTH]		= { 0 };
	//unsigned char	auszHash[1024]					= { 0 };

	// MultiByte -> Unicode
	iLen = MultiByteToWideChar(CP_ACP, 0, pszStr, strlen(pszStr), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, pszStr, strlen(pszStr), awszUnicode, iLen);

	// Unicode -> UTF-8
	iLen = WideCharToMultiByte(CP_UTF8, 0, awszUnicode, lstrlenW(awszUnicode), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, awszUnicode, lstrlenW(awszUnicode), aszUtf8, iLen, NULL, NULL);

	// SHA1 Library
	SHA1_Init(&stSHA1);
	SHA1_Update(&stSHA1, aszUtf8, strlen(aszUtf8));
	SHA1_Final(auszHash, &stSHA1);

	for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
	{
		sprintf_s(aszSHA1Value, "%02x", aszSHA1Value, auszHash[i]);
	}

	//MD5_Init(&stMD5);
	//MD5_Update(&stMD5, auszHash, strlen(aszUtf8));
	//MD5_Final(auszMD5Value, &stMD5);

	//for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
	//{
	//	sprintf((char*)auszMD5Value, "%02x", auszMD5Value, auszHash[i]);
	//}

	strcpy(pszResult, aszSHA1Value);
	//strcpy(pszResult, (char*)auszMD5Value);
}

void CNetScanOnvif::Base64Encoding(char* pszStr, int iSize, char* pszResult)
{
	static const char MimeBase64[] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3',
		'4', '5', '6', '7', '8', '9', '+', '/'
	};

	unsigned char input[3] = { 0 };
	unsigned char output[4] = { 0 };
	int index = 0;
	int i = 0;
	int j = 0;
	int iDataSize = 0;
	int	iLen = 0;
	char* p = NULL;
	char* plen = NULL;
	//wchar_t strUnicode[256] = { 0 };
	//char strUtf8[256] = { 0 };
	char* pszBuffer = NULL;

	plen = pszStr + iSize - 1;
	iDataSize = (4 * (iSize / 3)) + (iSize % 3 ? 4 : 0) + 1;
	pszBuffer = new char[iDataSize];
	memset(&pszBuffer[0], 0, sizeof(char) * iDataSize);
	//// 유니코드로 변형 
	//iLen = MultiByteToWideChar(CP_ACP, 0, pszResult, strlen(pszResult), NULL, NULL);
	//MultiByteToWideChar(CP_ACP, 0, pszResult, strlen(pszResult), strUnicode, iLen);

	//// 유니코드 -> utf-8
	//iLen = WideCharToMultiByte(CP_UTF8, 0, strUnicode, lstrlenW(strUnicode), NULL, 0, NULL, NULL);
	//WideCharToMultiByte(CP_UTF8, 0, strUnicode, lstrlenW(strUnicode), strUtf8, iLen, NULL, NULL);

	j = 0;
	for (i = 0, p = pszStr; p <= plen; i++, p++) {
		index = i % 3;
		input[index] = *p;
		if (index == 2 || p == plen) {
			output[0] = ((input[0] & 0xFC) >> 2);
			output[1] = ((input[0] & 0x3) << 4) | ((input[1] & 0xF0) >> 4);
			output[2] = ((input[1] & 0xF) << 2) | ((input[2] & 0xC0) >> 6);
			output[3] = (input[2] & 0x3F);
			pszBuffer[j++] = MimeBase64[output[0]];
			pszBuffer[j++] = MimeBase64[output[1]];
			pszBuffer[j++] = index == 0 ? '=' : MimeBase64[output[2]];
			pszBuffer[j++] = index <  2 ? '=' : MimeBase64[output[3]];
			input[0] = input[1] = input[2] = 0;
		}
	}

	pszBuffer[j] = '\0';

	strcpy(pszResult, pszBuffer);

	if (NULL != pszBuffer)
	{
		delete pszBuffer;
		pszBuffer = NULL;
	}
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