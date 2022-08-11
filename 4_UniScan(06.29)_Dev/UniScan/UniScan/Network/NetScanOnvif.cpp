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

	if (m_TcpSocket)
	{
		closesocket(m_TcpSocket);
		m_TcpSocket = NULL;
	}

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

BOOL CNetScanOnvif::CreateMultiCastSocket()
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

// Probe Message Request
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
	int			iSendBufferSize		= 0;
	char*		pszSendBuffer		= NULL;
	char*		paszOnvifURI[2]		= {"http://www.onvif.org/ver10/network/wsdl", "http://www.onvif.org/ver10/device/wsdl"};
	char*		paszProbeType[3]	= { "dp0:NetworkVideoTransmitter", "dp0:NetworkVideoDisplay", "dp0:Device" };

	// probe message
	char aszProbeXml[] =
	{
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
						<d:Types xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\"xmlns:dp0=%s>%s</Types>\
					</Probe>\
				</s:Body>\
			</Envelope>"
	};
	
	// 소켓 생성
	if (NULL == m_hReceiveSock)
		CreateMultiCastSocket();

	if (NULL == m_hReceiveSock)
		return FALSE;

	OnvifSendSock.sin_family = AF_INET;
	OnvifSendSock.sin_port = htons(3702);
	OnvifSendSock.sin_addr.s_addr = inet_addr("239.255.255.250"); // multicast group

	int iTypeIdx = 0;
	int iUriIdx = 0;
	// Probe Type 배열 요소 갯수 만큼 Packet Send
	while (iTypeIdx < sizeof(paszProbeType) / sizeof(paszProbeType[0]))
	{
		// UUID 얻기
		GenerateMsgID(szMessageID, 127);

		if (iTypeIdx > 1)
			iUriIdx++;

		iSendBufferSize = strlen(aszProbeXml) + strlen(szMessageID)+ strlen(paszOnvifURI[iUriIdx]) + strlen(paszProbeType[iTypeIdx]);
		pszSendBuffer = new char[iSendBufferSize + 1];
		memset(pszSendBuffer, 0, iSendBufferSize + 1);

		// 얻어온 UUID를 XML에 적용
		sprintf_s(pszSendBuffer, iSendBufferSize, aszProbeXml, szMessageID, paszOnvifURI[iUriIdx], paszProbeType[iTypeIdx]);

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

		if (NULL != pszSendBuffer)
		{
			delete[] pszSendBuffer;
			pszSendBuffer = NULL;
		}

		iTypeIdx++;
	}

	m_bConnected = TRUE;
	
	return TRUE;
}

void CNetScanOnvif::thrOnvifReceiver()
{
	SOCKADDR	stSockAddr;
	XNode		stNode;
	BOOL		bIsSuccessBind			= FALSE;
	int			iRevLen					= sizeof(sockaddr_in);
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
	char*		pszSlice				= NULL;
	char*		pszNameSlice			= NULL;
	char*		pszMacSlice				= NULL;
	DWORD		dwLastError				= 0;
	SCAN_INFO*	pScanInfo				= NULL;
	char*		pszProbeMatchType[2]	= { "wsdd:ProbeMatch", "d:ProbeMatch" };
	char*		pszAddressType[2]		= { "wsdd:XAddrs", "d:XAddrs" };
	BOOL		bIsTCPConnect			= FALSE;
	char		aszTestIP[24]			= "192.168.0.134";
	char		aszUserPWD[16]			= "111111" ;
	char		aszDigest[48]			= { 0 };
	char		aszNonceResult[128]		= { 0 }; // Base64로 인코딩된 16바이트여야 한다.
	char		aszDateResult[56]		= { 0 };
	char		aszPwdDigest[128]		= { 0 };
	char		aszPwdShaHash[56]		= { 0 };
	char		aszPwdBaseHash[128]		= { 0 };
	BOOL		bIsGetAuthData			= FALSE;

	m_pReceive_buffer = new char[SCAN_INFO_RECEIVE_BUFFER_SIZE];
	memset(m_pReceive_buffer, 0, sizeof(char) * SCAN_INFO_RECEIVE_BUFFER_SIZE);

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

						unsigned int uiSum = 0;
						unsigned int uiBinary = 0;
						//// ProbeMatch에서 받은 IP갯수 만큼 반복문으로 돌릴 예정 - 지금은 1개로 테스트 중
						bIsGetAuthData = GetAuthenticateData(aszTestIP, &aszDateResult[0], &aszNonceResult[0]); // Device 날짜 얻기
						if (TRUE == bIsGetAuthData)
						{
							sprintf_s(aszPwdDigest, sizeof(aszPwdDigest), "%d", aszNonceResult);
							uiSum += atoi(aszPwdDigest);
							sprintf_s(aszPwdDigest, sizeof(aszPwdDigest), "%d", aszDateResult);
							uiSum += atoi(aszPwdDigest);
							sprintf_s(aszPwdDigest, sizeof(aszPwdDigest), "%d", aszUserPWD);
							uiSum += atoi(aszPwdDigest);

							uiBinary = binaryConvert(uiSum);

							SHA1Encoding(aszPwdDigest, &aszPwdShaHash[0]);
							Base64Encoding(aszPwdShaHash, strlen(aszPwdShaHash), &aszPwdBaseHash[0]);

							SendAuthentication(aszTestIP, aszPwdBaseHash, aszNonceResult, aszDateResult);
						}

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

int CNetScanOnvif::binaryConvert(unsigned int uiData)
{
	int aiBinary[128] = { 0 };
	int iResult = 0;

	int idx = 0;
	while (1)
	{
		aiBinary[idx] = uiData % 2;
		uiData = uiData / 2;

		idx++;

		if (uiData == 0)
			break;
	}

	for (int i = idx - 1; i >= 0; i--)
	{
		TRACE(_T("%d"), aiBinary[i]);
	}
	//if (uiData < 2)
	//	sprintf_s(szBinary, sizeof(szBinary), (char)uiData);
	//else
	//{
	//	binaryConvert(uiData / 2);
	//	sprintf_s(szBinary, sizeof(szBinary), "%d", uiData % 2);
	//}
	//iResult = atoi(szBinary);

	return iResult;
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



BOOL CNetScanOnvif::ConnectTCPSocket(char* pszIP)
{
	sockaddr_in		HTTPSendSock	= { 0 };
	int				iError			= 0;

	m_TcpSocket = socket(PF_INET, SOCK_STREAM, 0);

	HTTPSendSock.sin_family = AF_INET;
	HTTPSendSock.sin_port = htons(80);
	HTTPSendSock.sin_addr.s_addr = inet_addr(pszIP);

	// --------------------------------------------------------------------------------
	// GetSystemDate Send & Recv 로직
	if (SOCKET_ERROR == connect(m_TcpSocket, (SOCKADDR*)&HTTPSendSock, sizeof(SOCKADDR)))
	{
		iError = WSAGetLastError();
		TRACE(_T("TCP-HTTP Connect Error = %d\n"), iError);
		closesocket(m_TcpSocket);

		return FALSE;
	}

	return TRUE;
}


// Device Info Request
BOOL CNetScanOnvif::SendDeviceInfo(char* pszIP, char* pszNonceResult)
{
	XNode			stNode;
	LPXNode			lpBody				= NULL;
	char*			pszRecvBuffer		= NULL;
	char*			pszSendBuffer		= NULL;
	int				iSendDataSize		= 0;
	int				iError				= 0;
	sockaddr_in		HTTPSendSock		= { 0 };
	char*			pszSendAuthBuff		= NULL;
	char*			pszSlice			= NULL;
	char*			pszSliceNonce		= NULL;
	char			aszNonceBase64[128] = { 0 };
	int				iHTTPStatus			= 0;
	char*			pszStrCompare		= "nonce";
	BOOL			bIsConnect			= FALSE;
	int				iSendBufferSize		= 0;

	char aszGetSystemTime[] = "POST /onvif/device_service HTTP/1.1\r\nContent-Type: application/soap+xml; charset=utf-8; action=\"http://www.onvif.org/ver10/device/wsdl/GetSystemDateAndTime\"\r\nHost: %s\r\nContent-Length: %d\r\nAccept-Encoding: gzip, deflate\r\nConnection: Close\r\n\r\n";

	char aszDeviceInformation[] = 
	{
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
			<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\">\
				<SOAP-ENV:Body>\
					<tds:GetDeviceInformation/>\
				</SOAP-ENV:Body>\
			</SOAP-ENV:Envelope>" // UserToken Header - GetDeviceInformation Body
	};

	bIsConnect = ConnectTCPSocket(pszIP);

	if (CONNECT_SUCCESS == bIsConnect)
	{
		iSendBufferSize = strlen(aszGetSystemTime) + strlen(aszDeviceInformation) + strlen(pszIP);
		
		pszSendBuffer = new char[sizeof(char) * iSendBufferSize + 1];
		memset(&pszSendBuffer[0], 0, iSendBufferSize + 1);

		pszRecvBuffer = new char[SCAN_INFO_RECEIVE_BUFFER_SIZE];
		memset(&pszRecvBuffer[0], 0, SCAN_INFO_RECEIVE_BUFFER_SIZE);

		sprintf_s(pszSendBuffer, sizeof(char) * iSendBufferSize, aszGetSystemTime, pszIP, strlen(aszDeviceInformation));
		strcat_s(pszSendBuffer, sizeof(char) * iSendBufferSize, aszDeviceInformation);

		iSendDataSize = strlen(pszSendBuffer);
		if (SOCKET_ERROR == send(m_TcpSocket, pszSendBuffer, iSendDataSize, 0))
		{
			iError = WSAGetLastError();
			TRACE(_T("TCP-HTTP send Error = %d\n"), iError);
			closesocket(m_TcpSocket);
			return FALSE;
		}

		if (SOCKET_ERROR == recv(m_TcpSocket, pszRecvBuffer, sizeof(char) * SCAN_INFO_RECEIVE_BUFFER_SIZE, 0))
		{
			iError = WSAGetLastError();
			TRACE(_T("TCP-HTTP recv Error = %d\n"), iError);
			closesocket(m_TcpSocket);
			return FALSE;
		}
		::OutputDebugStringA("ONVIF DEVICE DATA -----------------------\n");
		::OutputDebugStringA(pszIP);
		::OutputDebugStringA("\n");
		::OutputDebugStringA(pszRecvBuffer);
		::OutputDebugStringA("\n");

		if (0 < strlen(pszRecvBuffer))
		{
			stNode.Load(pszRecvBuffer);

			// 401 Status 찾기
			pszSlice = strtok(pszRecvBuffer, " ");
			pszSlice = strtok(NULL, " ");
			iHTTPStatus = atoi(pszSlice);

			if (UNAUTHORIZED == iHTTPStatus)
			{
				while (NULL != pszRecvBuffer)
				{
					// nonce 찾기
					pszSlice = strtok(NULL, ",%s=");
					if (0 == strcmp(pszSlice, pszStrCompare))
					{
						// Hash 값 얻기
						pszSliceNonce = strtok(NULL, "\" \"");
						Base64Encoding(pszSliceNonce, strlen(pszSliceNonce), &aszNonceBase64[0]); // nonce값 Base64로 인코딩
						memcpy(pszNonceResult, aszNonceBase64, sizeof(char)* strlen(aszNonceBase64));
						break;
					}
				}
			}
		}
	}

	if (NULL != pszSendBuffer)
	{
		delete[] pszSendBuffer;
		pszSendBuffer = NULL;
	}

	if (NULL != pszRecvBuffer)
	{
		delete[] pszRecvBuffer;
		pszRecvBuffer = NULL;
	}

	if (NULL != m_TcpSocket)
	{
		closesocket(m_TcpSocket);
		m_TcpSocket = NULL;
	}

	return TRUE;
}


// SystemDate Request
BOOL CNetScanOnvif::GetAuthenticateData(char* pszIP, char* pszDateResult, char* pszNonceResult)
{
	LPXNode		lpaDateType[6]		= { 0 }; // CStringA
	LPXNode		lpaTimeData[6]		= { 0 };
	XNode		stNode;
	LPXNode		lpBody				= NULL;
	char*		paiDate[6]			= { 0 };
	char*		paszDateType[2]		= { "tt:Date", "tt:Time" };
	char*		paszChild[6]		= { "tt:Year", "tt:Month", "tt:Day", "tt:Hour", "tt:Minute", "tt:Second" };
	char		aszTime[32]			= { 0 };
	char*		pszRecvBuffer		= NULL;
	char*		pszSendBuffer		= NULL;
	BOOL		bIsConnect			= FALSE;
	int			iSendBufferSize		= 0;
	int			iError				= 0;
	BOOL		bIsGetNonce = FALSE;

	char aszGetSystemTime[] = "POST /onvif/device_service HTTP/1.1\r\nContent-Type: application/soap+xml; charset=utf-8; action=\"http://www.onvif.org/ver10/device/wsdl/GetSystemDateAndTime\"\r\nHost: %s\r\nContent-Length: %d\r\nAccept-Encoding: gzip, deflate\r\nConnection: Close\r\n\r\n";

	char aszSystemDateXml[] =
	{
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
		<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\
			<s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
				<GetSystemDateAndTime xmlns=\"http://www.onvif.org/ver10/device/wsdl\"/>\
			</s:Body>\
		</s:Envelope>"
	};
		
	bIsConnect = ConnectTCPSocket(pszIP);

	iSendBufferSize = strlen(aszGetSystemTime) + strlen(pszIP) + strlen(aszSystemDateXml);
	pszSendBuffer = new char[iSendBufferSize + 1];
	memset(&pszSendBuffer[0], 0, iSendBufferSize + 1);

	pszRecvBuffer = new char[SCAN_INFO_RECEIVE_BUFFER_SIZE];
	memset(&pszRecvBuffer[0], 0, SCAN_INFO_RECEIVE_BUFFER_SIZE);

	sprintf_s(pszSendBuffer, sizeof(char)* iSendBufferSize, aszGetSystemTime, pszIP, strlen(aszSystemDateXml));
	strcat_s(pszSendBuffer, sizeof(char)* iSendBufferSize, aszSystemDateXml);

	if (CONNECT_SUCCESS == bIsConnect)
	{
		
		if (SOCKET_ERROR == send(m_TcpSocket, pszSendBuffer, iSendBufferSize, 0))
		{
			iError = WSAGetLastError();
			TRACE(_T("TCP-HTTP send Error = %d\n"), iError);
			closesocket(m_TcpSocket);
			return FALSE;
		}

		if (SOCKET_ERROR == recv(m_TcpSocket, pszRecvBuffer, SCAN_INFO_RECEIVE_BUFFER_SIZE, 0))
		{
			iError = WSAGetLastError();
			TRACE(_T("TCP-HTTP recv Error = %d\n"), iError);
			closesocket(m_TcpSocket);
			return FALSE;
		}
		::OutputDebugStringA("ONVIF DEVICE DATA -----------------------\n");
		::OutputDebugStringA(pszIP);
		::OutputDebugStringA("\n");
		::OutputDebugStringA(pszRecvBuffer);
		::OutputDebugStringA("\n");

		stNode.Load(pszRecvBuffer);
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

		} // Created 날짜 가져오기 
	}

	memcpy(pszDateResult, aszTime, sizeof(char) * strlen(aszTime));

	bIsGetNonce = SendDeviceInfo(pszIP, &pszNonceResult[0]); // Nonce 해쉬 얻기
	if (FALSE == bIsGetNonce)
		return FALSE;

	
	
	if (NULL != pszSendBuffer)
	{
		delete[] pszSendBuffer;
		pszSendBuffer = NULL;
	}

	if (NULL != pszRecvBuffer)
	{
		delete[] pszRecvBuffer;
		pszRecvBuffer = NULL;
	}

	if (NULL != m_TcpSocket)
	{
		closesocket(m_TcpSocket);
		m_TcpSocket = NULL;
	}

	return TRUE;
}

void CNetScanOnvif::SendAuthentication(char* pszIP, char* pszDigest, char* pszNonceResult, char* pszDateResult)
{

	BOOL	bIsConnect = FALSE;
	char*	pszRecvBuffer = NULL;
	char*	pszPacketBuffer = NULL;
	char*	pszSendBuffer = NULL;
	int		iHeaderSize = 0;
	int		iBodySize = 0;
	int		iPacketSize = 0;
	int		iError = 0;
	int		iContentLen = 0;
	

	char aszDeviceInfoHeader[] = "POST /onvif/device_service HTTP/1.1\r\nContent-Type: application/soap+xml; charset=utf-8; action=\"http://www.onvif.org/ver10/device/wsdl/GetZeroConfiguration\"\r\nHost: %s\r\nContent-Length: %d\r\nAccept-Encoding: gzip, deflate\r\nConnection: Close\r\n\r\n";

	char aszDeviceInformationXML[] = 
	{
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
			<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\
				<s:Header>\
					<Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\">\
						<UsernameToken>\
							<Username>admin</Username>\
							<Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">%s</Password>\
							<Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\">%s</Nonce>\
							<Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">%s</Created>\
						</UsernameToken>\
					</Security>\
				</s:Header>\
				<s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\
					<GetZeroConfiguration xmlns=\"http://www.onvif.org/ver10/device/wsdl\"/>\
				</s:Body>\
			</s:Envelope>" // UserToken Header - GetDeviceInformation Body
	};
	
	bIsConnect = ConnectTCPSocket(pszIP);

	iHeaderSize = strlen(aszDeviceInfoHeader) + strlen(pszIP) + 3;
	iBodySize = strlen(aszDeviceInformationXML) + strlen(pszDigest) + strlen(pszNonceResult) + strlen(pszDateResult);
	iPacketSize = iHeaderSize + iBodySize;
	iContentLen = iBodySize;

	pszPacketBuffer = new char[iPacketSize + 1];
	memset(&pszPacketBuffer[0], 0, iPacketSize + 1);

	sprintf_s(pszPacketBuffer, sizeof(char)* iHeaderSize, "%s", aszDeviceInfoHeader);
	strcat_s(pszPacketBuffer, sizeof(char)* iPacketSize, aszDeviceInformationXML);

	pszSendBuffer = new char[iPacketSize + 1];
	memset(&pszSendBuffer[0], 0, iPacketSize + 1);
	sprintf_s(pszSendBuffer, sizeof(char)* iPacketSize, pszPacketBuffer, pszIP, iContentLen, pszDigest, pszNonceResult, pszDateResult);
	//sprintf_s(pszSendBuffer, sizeof(char)* iSendBufferSize, aszDeviceInformationXML, pszDigest, pszNonceResult, pszDateResult);

	::OutputDebugStringA(pszSendBuffer);
	::OutputDebugStringA("\n");

	pszRecvBuffer = new char[SCAN_INFO_RECEIVE_BUFFER_SIZE];
	memset(&pszRecvBuffer[0], 0, SCAN_INFO_RECEIVE_BUFFER_SIZE);


	if (CONNECT_SUCCESS == bIsConnect)
	{

		if (SOCKET_ERROR == send(m_TcpSocket, pszSendBuffer, iPacketSize, 0))
		{
			iError = WSAGetLastError();
			TRACE(_T("TCP-HTTP send Error = %d\n"), iError);
			closesocket(m_TcpSocket);
		}

		if (SOCKET_ERROR == recv(m_TcpSocket, pszRecvBuffer, SCAN_INFO_RECEIVE_BUFFER_SIZE, 0))
		{
			iError = WSAGetLastError();
			TRACE(_T("TCP-HTTP recv Error = %d\n"), iError);
			closesocket(m_TcpSocket);
		}
 		::OutputDebugStringA("ONVIF DEVICE DATA -----------------------\n");
		::OutputDebugStringA(pszIP);
		::OutputDebugStringA("\n");
		::OutputDebugStringA(pszRecvBuffer);
		::OutputDebugStringA("\n");
	}

	if (NULL != pszPacketBuffer)
	{
		delete[] pszPacketBuffer;
		pszPacketBuffer = NULL;
	}

	if (NULL != pszSendBuffer)
	{
		delete[] pszSendBuffer;
		pszSendBuffer = NULL;
	}

	if (NULL != pszRecvBuffer)
	{
		delete[] pszRecvBuffer;
		pszRecvBuffer = NULL;
	}
}

void CNetScanOnvif::DigestConvert(char* pszStr, char* puszResult)
{
	char aszDigest[MAX_PATH] = { 0 };
	char aszOutput[SHA_DIGEST_LENGTH * 2 + 1] = { 0 };

	SHA1Encoding(pszStr, aszOutput);
	Base64Encoding(aszOutput, strlen(aszOutput), &aszDigest[0]);
	memcpy( puszResult, aszDigest, sizeof(char) * strlen(aszDigest));

}

void CNetScanOnvif::SHA1Encoding(char* pszStr, char* pszResult)
{
	SHA_CTX			stSHA1;
	int				iLen = 0;
	wchar_t			awszUnicode[MAX_PATH]		= { 0 };	// Unicode
	char			aszUtf8[MAX_PATH]			= { 0 };	// UTF-8 
	char			aszSHA1Value[MAX_PATH]		= { 0 };	// SHA1
	unsigned char	auszHash[SHA_DIGEST_LENGTH]	= { 0 };

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
		sprintf_s(&aszSHA1Value[i * 2], SHA_DIGEST_LENGTH * 2 + 1, "%02x", auszHash[i]);
	}

	//strcpy_s(pszResult, SHA_DIGEST_LENGTH * 2 + 1, aszSHA1Value);
	memcpy( pszResult, aszSHA1Value, sizeof(char) * (SHA_DIGEST_LENGTH * 2 + 1) );
}

// 128bit 
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

	unsigned char input[3]	= { 0 };
	unsigned char output[4] = { 0 };
	int index				= 0;
	int i					= 0;
	int j					= 0;
	int iDataSize			= 0;
	int	iLen				= 0;
	char* p					= NULL;
	char* plen				= NULL;
	wchar_t strUnicode[MAX_PATH] = { 0 };
	char strUtf8[MAX_PATH] = { 0 };
	char* pszBuffer			= NULL;

	plen = pszStr + iSize - 1;
	iDataSize = (4 * (iSize / 3)) + (iSize % 3 ? 4 : 0) + 1;
	pszBuffer = new char[iDataSize];
	memset(&pszBuffer[0], 0, sizeof(char) * iDataSize);
	
	// 유니코드로 변형 
	iLen = MultiByteToWideChar(CP_ACP, 0, pszStr, strlen(pszStr), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, pszStr, strlen(pszStr), strUnicode, iLen);

	// 유니코드 -> utf-8
	iLen = WideCharToMultiByte(CP_UTF8, 0, strUnicode, lstrlenW(strUnicode), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, strUnicode, lstrlenW(strUnicode), strUtf8, iLen, NULL, NULL);

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