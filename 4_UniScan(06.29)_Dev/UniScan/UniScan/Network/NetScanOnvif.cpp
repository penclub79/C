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
	m_TcpSocket = NULL;
	m_aszIP[IP_SIZE] = { 0 };
	m_aszDigest[DIGEST_SIZE] = { 0 };
	m_aszBase64[NONCE_SIZE] = { 0 };
	m_aszDate[DATE_SIZE] = { 0 };
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
	int			iError = 0;
	char		szEnable = 0;
	int			iReuse = 0;
	struct		ip_mreq mreq;
	struct		in_addr localInterface;

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
		iReuse = 1;
		if (SOCKET_ERROR == setsockopt(m_hReceiveSock, SOL_SOCKET, SO_REUSEADDR, (char *)&iReuse, sizeof(iReuse)))
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
		}
	}

	return TRUE;
}

// Probe Message Request
BOOL CNetScanOnvif::SendScanRequest()
{
	sockaddr_in OnvifSendSock = { 0 };
	sockaddr_in	multicast_addr = { 0 };
	SOCKET		hSendSock = NULL;
	DWORD		dwFileSize = 0;
	int			iSize = 0;
	char		szMessageID[128] = { 0 };
	int			iError = 0;
	int			iSendBufferSize = 0;
	char*		pszProbeSendBuffer = NULL;
	char*		pszResolveSendBuffer = NULL;
	char*		paszOnvifURI[2] = { "http://www.onvif.org/ver10/network/wsdl", "http://www.onvif.org/ver10/device/wsdl" };
	char*		paszProbeType[3] = { "dp0:Device", "dp0:NetworkVideoDisplay", "dp0:NetworkVideoTransmitter" };

	// 소켓 생성
	if (NULL == m_hReceiveSock)
		CreateMultiCastSocket();

	if (NULL == m_hReceiveSock)
		return FALSE;

	OnvifSendSock.sin_family = AF_INET;
	OnvifSendSock.sin_port = htons(MULTICAST_PORT);
	OnvifSendSock.sin_addr.s_addr = inet_addr("239.255.255.250"); // multicast group

	int iTypeIdx = 0;
	int iUriIdx = 0;

	// Probe Type 배열 요소 갯수 만큼 Packet Send
	while (iTypeIdx < sizeof(paszProbeType) / sizeof(paszProbeType[0]))
	{
		// UUID 얻기
		GenerateMsgID(szMessageID, UUID_SIZE);
		strlwr(szMessageID);

		if (iTypeIdx > 1)
			iUriIdx++;

		iSendBufferSize = strlen(__aszProbeXml) + strlen(szMessageID) + strlen(paszOnvifURI[iUriIdx]) + strlen(paszProbeType[iTypeIdx]);
		pszProbeSendBuffer = new char[iSendBufferSize + 1];
		memset(pszProbeSendBuffer, 0, iSendBufferSize + 1);

		// 얻어온 UUID를 XML에 적용
		sprintf_s(pszProbeSendBuffer, iSendBufferSize, __aszProbeXml, szMessageID, paszOnvifURI[iUriIdx], paszProbeType[iTypeIdx]);

		iSize = strlen(pszProbeSendBuffer);
		if (SOCKET_ERROR == sendto(m_hReceiveSock, pszProbeSendBuffer, iSize, 0, (struct sockaddr*)&OnvifSendSock, sizeof(OnvifSendSock)))
		{
			iError = WSAGetLastError();
			TRACE(_T("sendto Error = %d\n"), iError);
			closesocket(m_hReceiveSock);

			delete[] pszProbeSendBuffer;
			pszProbeSendBuffer = NULL;
			return FALSE;
		}

		if (NULL != pszProbeSendBuffer)
		{
			delete[] pszProbeSendBuffer;
			pszProbeSendBuffer = NULL;
		}

		iTypeIdx++;
	}

	m_bConnected = TRUE;

	if (NULL != pszResolveSendBuffer)
	{
		delete[] pszResolveSendBuffer;
		pszResolveSendBuffer = NULL;
	}

	return TRUE;
}

void CNetScanOnvif::thrOnvifReceiver()
{
	SOCKADDR		stSockAddr;
	int				iRevLen = sizeof(sockaddr_in);
	DWORD			dwLastError = 0;
	SCAN_INFO*		pScanInfo = NULL;
	ONVIF_INFO*		pOnvifInfo = NULL;
	//char			aszNonceBase64[BASE64_SIZE] = { 0 };


	while (this->m_dwScanThreadID)
	{
		if (TRUE == this->m_bUserCancel)
			goto FINAL;

		if (NULL == m_hReceiveSock || FALSE == m_bConnected)
		{
			Sleep(100);
			continue;
		}

		m_aszDigest[DIGEST_SIZE] = { 0 };
		m_aszBase64[BASE64_SIZE] = { 0 };
		m_aszDate[DATE_SIZE] = { 0 };

		m_pReceive_buffer = new char[SCAN_INFO_RECEIVE_BUFFER_SIZE]; // 65255 size
		memset(m_pReceive_buffer, 0, sizeof(char)* SCAN_INFO_RECEIVE_BUFFER_SIZE);

		if (SOCKET_ERROR == recvfrom(m_hReceiveSock, m_pReceive_buffer, SCAN_INFO_RECEIVE_BUFFER_SIZE, 0, (SOCKADDR*)&stSockAddr, &iRevLen))
		{
			dwLastError = WSAGetLastError();
			TRACE(_T("recvfrom error=%d\n"), dwLastError);

			if (this->m_hNotifyWnd && dwLastError != WSAEINTR && dwLastError != WSAEINVAL)
				::SendMessage(this->m_hNotifyWnd, this->m_lNotifyMsg, 0, SCAN_ERR_RECV);

			break;
		}

		if (NULL != m_pReceive_buffer)
		{
			pScanInfo = new SCAN_INFO; // 472 size
			memset(pScanInfo, 0, sizeof(SCAN_INFO));

			pOnvifInfo = new ONVIF_INFO; // 300 size
			memset(pOnvifInfo, 0, sizeof(ONVIF_INFO));

			GetIPAndModelName(pOnvifInfo);

			SendTcpOnvif(pOnvifInfo);

			DataParsing(pOnvifInfo, pScanInfo);

			if (0 != m_dwScanThreadID)
			{
				if (this->m_hNotifyWnd)
				{
					if (FALSE == m_bUserCancel)
					{
						//::SendMessageTimeout(this->m_hNotifyWnd, this->m_lNotifyMsg, (WPARAM)pScanInfo, 0, SMTO_NORMAL, 10, NULL);
						::SendMessage(this->m_hNotifyWnd, this->m_lNotifyMsg, (WPARAM)pScanInfo, 0);
					}
				}
			}

			if (NULL != pOnvifInfo)
			{
				delete pOnvifInfo;
				pOnvifInfo = NULL;
			}
			this->DelBuff();
		}

		if (m_TcpSocket)
		{
			closesocket(m_TcpSocket);
			m_TcpSocket = NULL;
		}
	}

	return;

FINAL:
	if (NULL != pScanInfo)
	{
		delete pScanInfo;
		pScanInfo = NULL;
	}

	if (NULL != pOnvifInfo)
	{
		delete pOnvifInfo;
		pOnvifInfo = NULL;
	}
	this->DelBuff();
}

void CNetScanOnvif::DataParsing(const ONVIF_INFO* pstOnvifInfo, const SCAN_INFO* pstScanInfo)
{
	SCAN_INFO*		pScanInfo = (SCAN_INFO*)pstScanInfo;
	ONVIF_INFO*		pOnvifInfo = (ONVIF_INFO*)pstOnvifInfo;

	if (0 < strlen(m_aszIP))
		this->WideCopyStringFromAnsi(pScanInfo->szAddr, 32, m_aszIP);
	else
		wsprintf(pScanInfo->szAddr, _T("N/A"));

	if (0 < strlen(pOnvifInfo->aszVersion))
		this->WideCopyStringFromAnsi(pScanInfo->szSwVersion, 30, pOnvifInfo->aszVersion);
	else
		wsprintf(pScanInfo->szSwVersion, _T("N/A"));

	if (0 < strlen(pOnvifInfo->aszMAC))
		this->WideCopyStringFromAnsi(pScanInfo->szMAC, 32, pOnvifInfo->aszMAC);
	else
		wsprintf(pScanInfo->szMAC, _T("N/A"));

	if (0 < strlen(pOnvifInfo->aszFirmwareVer))
		this->WideCopyStringFromAnsi(pScanInfo->szFirmwareVer, 32, pOnvifInfo->aszFirmwareVer);
	else
		wsprintf(pScanInfo->szFirmwareVer, _T("N/A"));

	if (0 < strlen(pOnvifInfo->aszModelName))
		this->WideCopyStringFromAnsi(pScanInfo->szModelName, 30, pOnvifInfo->aszModelName);
	else
		wsprintf(pScanInfo->szModelName, _T("N/A"));

	wsprintf(pScanInfo->szGateWay, _T("N/A"));

	pScanInfo->iVideoCnt = pOnvifInfo->iChannelCnt;
	pScanInfo->nHTTPPort = pOnvifInfo->iHttpPort;

	return;
}

void CNetScanOnvif::SoapRequestMessage(int iReqType, int iHttpHeaderSize, int iContentSize)
{
	char* pszSendBuffer = NULL;
	char* pszCopyBuffer = NULL;
	int iError = 0;
	int iSendDataSize = 0;

	iSendDataSize = iHttpHeaderSize + iContentSize;

	pszSendBuffer = new char[sizeof(char)* iSendDataSize + 1];
	memset(&pszSendBuffer[0], 0, iSendDataSize + 1);

	switch (iReqType)
	{
	case SYSTEMDATE:
		sprintf_s(pszSendBuffer, sizeof(char)* iSendDataSize, __aszDeviceHeader, m_aszIP, strlen(__aszSystemDateXml));
		strcat_s(pszSendBuffer, sizeof(char)* iSendDataSize, __aszSystemDateXml);
		break;

	case DEVICEINFO:
		sprintf_s(pszSendBuffer, sizeof(char)* iSendDataSize, __aszDeviceHeader, m_aszIP, strlen(__aszDeviceInfoXml));
		strcat_s(pszSendBuffer, sizeof(char)* iSendDataSize, __aszDeviceInfoXml);
		break;

	case VERSIONINFO:
		sprintf_s(pszSendBuffer, sizeof(char)* iSendDataSize, __aszDeviceHeader, m_aszIP, strlen(__aszVersionXml));
		strcat_s(pszSendBuffer, sizeof(char)* iSendDataSize, __aszVersionXml);
		break;

	case DEVICEINFO_DIGEST:
		pszCopyBuffer = new char[iSendDataSize + 1];
		memset(&pszCopyBuffer[0], 0, iSendDataSize + 1);

		sprintf_s(pszCopyBuffer, sizeof(char)* iHttpHeaderSize, "%s", __aszDeviceHeader);
		strcat_s(pszCopyBuffer, sizeof(char)* iSendDataSize, __aszDeviceInfoDigestXml);
		sprintf_s(pszSendBuffer, sizeof(char)* iSendDataSize, pszCopyBuffer, m_aszIP, iContentSize, m_aszUserName, m_aszDigest, m_aszBase64, m_aszDate);

		if (NULL != pszCopyBuffer)
		{
			delete[] pszCopyBuffer;
			pszCopyBuffer = NULL;
		}

		break;

	case NETWORKINFO_DIGEST:
		pszCopyBuffer = new char[iSendDataSize + 1];
		memset(&pszCopyBuffer[0], 0, iSendDataSize + 1);

		sprintf_s(pszCopyBuffer, sizeof(char)* iHttpHeaderSize, "%s", __aszNetworkHeader);
		strcat_s(pszCopyBuffer, sizeof(char)* iSendDataSize, __aszNetworkInfoDigestXml);
		sprintf_s(pszSendBuffer, sizeof(char)* iSendDataSize, pszCopyBuffer, m_aszIP, iContentSize, m_aszUserName, m_aszDigest, m_aszBase64, m_aszDate);

		if (NULL != pszCopyBuffer)
		{
			delete[] pszCopyBuffer;
			pszCopyBuffer = NULL;
		}

		break;
	}

	if (SOCKET_ERROR == send(m_TcpSocket, pszSendBuffer, iSendDataSize, 0))
	{
		iError = WSAGetLastError();
		TRACE(_T("TCP-HTTP send Error = %d\n"), iError);
	}

	if (SOCKET_ERROR == recv(m_TcpSocket, m_pReceive_buffer, SCAN_INFO_RECEIVE_BUFFER_SIZE, 0))
	{
		iError = WSAGetLastError();
		TRACE(_T("TCP-HTTP recv Error = %d\n"), iError);
	}

	if (NULL != pszSendBuffer)
	{
		delete[] pszSendBuffer;
		pszSendBuffer = NULL;
	}
}

void CNetScanOnvif::SendTcpOnvif(ONVIF_INFO* pstOnvifInfo)
{
	/*CNetScanOnvif	Cthis;*/
	ONVIF_INFO*		pOnvifInfo = (ONVIF_INFO*)pstOnvifInfo;
	char			aszPwdDigest[DIGEST_SIZE] = { 0 };
	char			aszPwdShaHash[41] = { 0 };
	BOOL			bIsConnect = FALSE;
	int				iCnt = 0;
	void(CNetScanOnvif::*pSendFuncArr[4])(ONVIF_INFO*) = { 0 };

	pSendFuncArr[0] = &CNetScanOnvif::SendAuthenticate;
	pSendFuncArr[1] = &CNetScanOnvif::SendOnvifVersion;
	pSendFuncArr[2] = &CNetScanOnvif::GetDeviceInfo;
	pSendFuncArr[3] = &CNetScanOnvif::GetNetworkInterface;

	while (iCnt < sizeof(pSendFuncArr) / sizeof(pSendFuncArr[0]))
	{
		if (0 < strlen(m_pReceive_buffer))
			memset(&m_pReceive_buffer[0], 0, sizeof(SCAN_INFO_RECEIVE_BUFFER_SIZE));

		bIsConnect = ConnectTCPSocket(m_aszIP, pOnvifInfo->iHttpPort);

		if (TRUE == bIsConnect)
		{
			(this->*pSendFuncArr[iCnt])(pOnvifInfo);

			if (0 == iCnt)
			{
				SendDeviceInfo(pOnvifInfo);

				// 없으면, Client 난수 생성
				if (0 == strlen(pOnvifInfo->aszNonce))
					strcpy(pOnvifInfo->aszNonce, __aszNonce);

				// 인증 로직
				if (0 < strlen(pOnvifInfo->aszNonce))
				{
					Base64Encoding(pOnvifInfo->aszNonce, strlen(pOnvifInfo->aszNonce), &m_aszBase64[0]); // nonce값 Base64로 인코딩
					//Base64Decoding(m_aszBase64, &auszNonceDecode[0], sizeof(auszNonceDecode));

					sprintf_s(aszPwdDigest, sizeof(aszPwdDigest), "%s%s%s", pOnvifInfo->aszNonce, m_aszDate, m_aszPassword);

					SHA1Encoding(aszPwdDigest, &aszPwdShaHash[0]);
					Base64Encoding(aszPwdShaHash, SHA_DIGEST_LENGTH, &m_aszDigest[0]);
				}
			}
		}
		iCnt++;
	}
}

void CNetScanOnvif::GetIPAndModelName(ONVIF_INFO* pstOnvifInfo)
{
	XNode			stNode;
	LPXNode			lpBody = NULL;
	LPXNode			lpUUID = NULL;
	LPXNode			lpTypeCheck = NULL;
	LPXNode			lpScope = NULL;
	LPXNode			lpIPAddress = NULL;
	char			aszUUID[UUID_SIZE] = { 0 };
	char			aszIPData[128] = { 0 };
	char			aszIPSlice[MAX_PATH] = { 0 };
	char*			pszSlice = NULL;
	char*			pszNameSlice = NULL;
	char*			pszBuffer = NULL;
	ONVIF_INFO*		pOnvifInfo = (ONVIF_INFO*)pstOnvifInfo;

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

	// IP Parsing
	lpTypeCheck = (NULL != stNode.GetChildArg("wsdd:ProbeMatch", NULL)) ? stNode.GetChildArg("wsdd:ProbeMatch", NULL) : stNode.GetChildArg("d:ProbeMatch", NULL);

	if (NULL != lpTypeCheck)
	{
		lpIPAddress = (NULL != lpTypeCheck->GetChildArg("wsdd:XAddrs", NULL)) ? lpTypeCheck->GetChildArg("wsdd:XAddrs", NULL) : lpTypeCheck->GetChildArg("d:XAddrs", NULL);

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
					pOnvifInfo->iHttpPort = atoi(pszSlice);
				}
			}
			else
				pOnvifInfo->iHttpPort = 80;

			strcpy(&aszIPSlice[0], lpIPAddress->value);
			pszSlice = strtok(aszIPSlice, ":");
			pszSlice = strtok(NULL, ":");
			pszSlice = strtok(pszSlice, "//");
			strcpy(&m_aszIP[0], pszSlice);
		}

		// ModelName
		lpScope = (NULL != lpTypeCheck->GetChildArg("wsdd:Scopes", NULL)) ? lpTypeCheck->GetChildArg("wsdd:Scopes", NULL) : lpTypeCheck->GetChildArg("d:Scopes", NULL);

		if (NULL != lpScope)
		{
			pszBuffer = new char[sizeof(char)* lpScope->value.GetLength() + 1];
			strcpy(&pszBuffer[0], lpScope->value);

			pszNameSlice = strstr(pszBuffer, "name"); // return NULL

			if (NULL != pszNameSlice)
			{
				pszNameSlice = strtok(pszNameSlice, " ");
				pszNameSlice = strncpy(pszNameSlice, pszNameSlice + 5, 20);
				strcpy_s(pOnvifInfo->aszModelName, 30, pszNameSlice);
			}
		}
	}

	if (NULL != pszBuffer)
	{
		delete[] pszBuffer;
		pszBuffer = NULL;
	}

}


BOOL CNetScanOnvif::GenerateMsgID(char* pszMessageID, int iBufferLen)
{
	if (pszMessageID == NULL || iBufferLen == 0)
		return FALSE;

	GUID guid;
	WCHAR wszGUID[129];

	USES_CONVERSION;
	memset(wszGUID, 0, sizeof(WCHAR)* 129);
	CoCreateGuid(&guid);
	StringFromGUID2(guid, wszGUID, 128); // ex. {D23370B9-3007-47d7-BAEA-30DDD6B0D24B}
	int nGUIDStringLen = wcslen(wszGUID);

	strncpy_s(pszMessageID, iBufferLen, W2A(wszGUID) + 1, nGUIDStringLen - 2);

	return TRUE;
}


BOOL CNetScanOnvif::ConnectTCPSocket(char* pszIP, int iPort)
{
	sockaddr_in		HTTPSendSock = { 0 };
	int				iError = 0;
	BOOL			bOptval = TRUE;
	DWORD			dwAliveVal = 0;

	m_TcpSocket = socket(PF_INET, SOCK_STREAM, 0);

	HTTPSendSock.sin_family = AF_INET;
	HTTPSendSock.sin_port = htons(iPort);
	HTTPSendSock.sin_addr.s_addr = inet_addr(pszIP);

	// --------------------------------------------------------------------------------
	// GetSystemDate Send & Recv 로직

	dwAliveVal = setsockopt(m_TcpSocket, SOL_SOCKET, SO_KEEPALIVE, (const char*)&bOptval, sizeof(BOOL));
	if (dwAliveVal < 0)
	{
		iError = WSAGetLastError();
		closesocket(m_TcpSocket);
		TRACE(_T("TCP-HTTP Connect Error = %d\n"), iError);

		return FALSE;
	}

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
void CNetScanOnvif::SendDeviceInfo(ONVIF_INFO* pstOnvifInfo)
{
	sockaddr_in		HTTPSendSock = { 0 };
	char*			pszSlice = NULL;
	char*			pszSliceNonce = NULL;
	int				iHTTPStatus = 0;
	char			aszStrCompare[6] = "nonce";
	int				iHttpHeaderSize = 0;
	int				iContentSize = 0;
	char			aszCopyBuffer[20] = { 0 };
	ONVIF_INFO*		pOnvifInfo = (ONVIF_INFO*)pstOnvifInfo;

	iHttpHeaderSize = strlen(__aszDeviceHeader) + strlen(m_aszIP) + 4;
	iContentSize = strlen(__aszDeviceInfoXml);
	SoapRequestMessage(DEVICEINFO, iHttpHeaderSize, iContentSize);

	if (NULL != m_pReceive_buffer)
	{
		if (0 < strlen(m_pReceive_buffer))
		{
			memcpy(&aszCopyBuffer[0], m_pReceive_buffer, 20);

			// 401 Status 찾기
			pszSlice = strtok(aszCopyBuffer, " ");
			pszSlice = strtok(NULL, " ");
			iHTTPStatus = atoi(pszSlice);

			//// 200 일때
			//if (RES_SUCCESS == iHTTPStatus)
			//{
			//	stNode.Load(m_pReceive_buffer);
			//	lpBody = stNode.GetChildArg("SOAP-ENV:Fault", NULL);

			//	if (NULL != lpBody)
			//		sprintf_s(m_pReceive_buffer, sizeof(int), "%d", UNAUTHORIZED);

			//	else
			//		strcpy_s(m_pReceive_buffer, sizeof(char)* SCAN_INFO_RECEIVE_BUFFER_SIZE, m_pReceive_buffer);

			//	//pszNonceResult = NULL;
			//}

			if (UNAUTHORIZED == iHTTPStatus)
			{
				pszSlice = strtok(m_pReceive_buffer, " ");
				pszSlice = strtok(NULL, " ");
				while (NULL != m_pReceive_buffer)
				{
					// nonce 찾기
					pszSlice = strtok(NULL, ", =");
					if (0 == strcmp(pszSlice, aszStrCompare))
					{
						// Hash 값 얻기
						pszSliceNonce = strtok(NULL, "\" \"");
						strcat_s(pOnvifInfo->aszNonce, sizeof(char)* strlen(pszSliceNonce) + 1, pszSliceNonce);
						break;
					}
				}
			}
		}
	}
}

void CNetScanOnvif::SendOnvifVersion(ONVIF_INFO* pstOnvifInfo)
{
	XNode			stNode;
	LPXNode			lpSupportedVersions = NULL;
	LPXNode			lpMajor = NULL;
	LPXNode			lpMinor = NULL;
	char*			pszSlice = NULL;
	int				iHTTPStatus = 0;
	int				iHttpHeaderSize = 0;
	int				iContentSize = 0;
	char			aszCopyBuffer[20] = { 0 };
	ONVIF_INFO*		pOnvifInfo = (ONVIF_INFO*)pstOnvifInfo;

	iHttpHeaderSize = strlen(__aszDeviceHeader) + +strlen(m_aszIP) + 4;
	iContentSize = strlen(__aszVersionXml);
	SoapRequestMessage(VERSIONINFO, iHttpHeaderSize, iContentSize);

	if (NULL != m_pReceive_buffer)
	{
		memcpy(&aszCopyBuffer[0], m_pReceive_buffer, 20);

		// 401 Status 찾기
		pszSlice = strtok(aszCopyBuffer, " ");
		pszSlice = strtok(NULL, " ");
		iHTTPStatus = atoi(pszSlice);

		if (RES_SUCCESS == iHTTPStatus)
		{
			stNode.Load(m_pReceive_buffer);
			lpSupportedVersions = stNode.GetChildArg("tt:SupportedVersions", NULL);
			char aszMajor[4] = { 0 };
			char aszMinor[4] = { 0 };
			char aszVersion[8] = { 0 };
			if (NULL != lpSupportedVersions)
			{
				lpMajor = lpSupportedVersions->GetChildArg("tt:Major", NULL);
				if (NULL != lpMajor)
					strcpy(aszMajor, lpMajor->value);

				lpMinor = lpSupportedVersions->GetChildArg("tt:Minor", NULL);
				if (NULL != lpMinor)
					strcpy(aszMinor, lpMinor->value);
			}
			sprintf_s(&pOnvifInfo->aszVersion[0], sizeof(char)* 8, "%d.%d", atoi(aszMajor), atoi(aszMinor));
		}
	}
}


// SystemDate Request
void CNetScanOnvif::SendAuthenticate(ONVIF_INFO* pstOnvifInfo)
{
	XNode		stNode;
	LPXNode		lpaDateType[6] = { 0 }; // CStringA
	LPXNode		lpaTimeData[6] = { 0 };
	LPXNode		lpBody = NULL;
	char*		paszDate[6] = { 0 };
	char*		paszDateType[2] = { "tt:Date", "tt:Time" };
	char*		paszChild[6] = { "tt:Year", "tt:Month", "tt:Day", "tt:Hour", "tt:Minute", "tt:Second" };
	char		aszTime[32] = { 0 };
	char*		pszSendBuffer = NULL;
	int			iHttpHeaderSize = 0;
	int			iContentSize = 0;
	BOOL		bIsGetNonce = FALSE;
	ONVIF_INFO* pOnvifInfo = (ONVIF_INFO*)pstOnvifInfo;

	iHttpHeaderSize = strlen(__aszDeviceHeader) + strlen(m_aszIP) + 4;
	iContentSize = strlen(__aszSystemDateXml);
	SoapRequestMessage(SYSTEMDATE, iHttpHeaderSize, iContentSize);

	if (NULL != m_pReceive_buffer)
	{
		stNode.Load(m_pReceive_buffer);
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
					paszDate[index] = lpaTimeData[index]->value.GetBuffer(0);

				index++;
			}
			sprintf_s(m_aszDate, sizeof(aszTime), "%04d-%02d-%02dT%02d:%02d:%02d.000Z",
				atoi(paszDate[0]),
				atoi(paszDate[1]),
				atoi(paszDate[2]),
				atoi(paszDate[3]),
				atoi(paszDate[4]),
				atoi(paszDate[5])
				);

		} // Created 날짜 가져오기 
	}
}

void CNetScanOnvif::GetDeviceInfo(ONVIF_INFO* pstOnvifInfo)
{
	XNode			stNode;
	LPXNode			lpDeviceInfoBody = NULL;
	BOOL			bIsConnect = FALSE;
	char*			pszPacketBuffer = NULL;
	char*			pszSendBuffer = NULL;
	char*			pszSlice = NULL;
	int				iHeaderSize = 0;
	int				iBodySize = 0;
	int				iHttpHeaderSize = 0;
	int				iContentSize = 0;
	int				iError = 0;
	int				iContentLen = 0;
	int				iHTTPStatus = 0;
	char			aszCopyBuffer[20] = { 0 };
	char*			pszDeviceInfoData = NULL;
	ONVIF_INFO*		pOnvifInfo = (ONVIF_INFO*)pstOnvifInfo;

	iHttpHeaderSize = strlen(__aszDeviceHeader) + strlen(m_aszIP) + 3;
	iContentSize = strlen(__aszDeviceInfoDigestXml) + strlen(m_aszDigest) + strlen(m_aszBase64) + strlen(m_aszDate) + strlen(m_aszUserName);

	SoapRequestMessage(DEVICEINFO_DIGEST, iHttpHeaderSize, iContentSize);

	if (NULL != m_pReceive_buffer)
	{
		memcpy(&aszCopyBuffer[0], m_pReceive_buffer, 20);
		pszSlice = strstr(aszCopyBuffer, " ");
		if (NULL != pszSlice)
		{
			pszSlice = strtok(pszSlice, " ");
			iHTTPStatus = atoi(pszSlice);

			switch (iHTTPStatus)
			{
			case RES_SUCCESS:
				stNode.Load(m_pReceive_buffer);
				lpDeviceInfoBody = stNode.GetChildArg("tds:GetDeviceInformationResponse", NULL);

				if (NULL != lpDeviceInfoBody)
					lpDeviceInfoBody = lpDeviceInfoBody->GetChildArg("tds:FirmwareVersion", NULL);

				// FirwareVersion Value Check
				if (NULL != lpDeviceInfoBody)
				{
					if (1 < lpDeviceInfoBody->value.GetLength())
					{
						strcpy(&pOnvifInfo->aszFirmwareVer[0], lpDeviceInfoBody->value);
					}
				}
				break;

			case BAD_REQUEST:
				sprintf_s(pOnvifInfo->aszFirmwareVer, sizeof(int), "%s", "N/A");
				break;

			case UNAUTHORIZED:
				sprintf_s(pOnvifInfo->aszFirmwareVer, sizeof(int), "%s", "N/A");
				break;
			}
		}
	}
}


void CNetScanOnvif::GetNetworkInterface(ONVIF_INFO* pstOnvifInfo)
{
	XNode			stNode;
	LPXNode			lpMAC = NULL;
	BOOL			bIsConnect = FALSE;
	char*			pszPacketBuffer = NULL;
	char*			pszSendBuffer = NULL;
	char*			pszSlice = NULL;
	int				iHttpHeaderSize = 0;
	int				iContentSize = 0;
	int				iError = 0;
	int				iHTTPStatus = 0;
	char			aszCopyBuffer[20] = { 0 };
	ONVIF_INFO*		pOnvifInfo = (ONVIF_INFO*)pstOnvifInfo;

	iHttpHeaderSize = strlen(__aszNetworkHeader) + strlen(m_aszIP) + 3;
	iContentSize = strlen(__aszNetworkInfoDigestXml) + strlen(m_aszDigest) + strlen(m_aszBase64) + strlen(m_aszDate) + strlen(m_aszUserName);

	SoapRequestMessage(NETWORKINFO_DIGEST, iHttpHeaderSize, iContentSize);

	if (NULL != m_pReceive_buffer)
	{
		memcpy(&aszCopyBuffer[0], m_pReceive_buffer, 20);
		pszSlice = strstr(aszCopyBuffer, " ");
		if (NULL != pszSlice)
		{
			pszSlice = strtok(pszSlice, " ");
			iHTTPStatus = atoi(pszSlice);

			switch (iHTTPStatus)
			{
			case RES_SUCCESS:
				stNode.Load(m_pReceive_buffer);
				lpMAC = stNode.GetChildArg("tt:HwAddress", NULL);
				if (NULL != lpMAC)
				{
					if (1 < lpMAC->value.GetLength())
					{
						strcpy(&pOnvifInfo->aszMAC[0], lpMAC->value);

						for (int i = 0; i < strlen(pOnvifInfo->aszMAC); i++)
						{
							if (pOnvifInfo->aszMAC[i] >= 'A' && pOnvifInfo->aszMAC[i] <= 'Z')
							{
								pOnvifInfo->aszMAC[i] += 32;
							}
						}
					}
				}
				break;

			case BAD_REQUEST:
				sprintf_s(pOnvifInfo->aszMAC, sizeof(int), "%s", "N/A");
				break;

			case UNAUTHORIZED:
				sprintf_s(pOnvifInfo->aszMAC, sizeof(int), "%s", "N/A");
				break;
			}
		}
	}

}


void CNetScanOnvif::DigestConvert(char* pszStr, char* puszResult)
{
	char aszDigest[MAX_PATH] = { 0 };
	char aszOutput[SHA_DIGEST_LENGTH * 2 + 1] = { 0 };

	SHA1Encoding(pszStr, aszOutput);
	Base64Encoding(aszOutput, strlen(aszOutput), &aszDigest[0]);
	memcpy(puszResult, aszDigest, sizeof(char)* strlen(aszDigest));

}

void CNetScanOnvif::SHA1Encoding(char* pszStr, char* pszResult)
{
	SHA_CTX			stSHA1;
	int				iLen = 0;
	unsigned char	auszHash[SHA_DIGEST_LENGTH] = { 0 };


	// SHA1 Library
	SHA1_Init(&stSHA1);
	SHA1_Update(&stSHA1, pszStr, strlen(pszStr));
	SHA1_Final(auszHash, &stSHA1);

	memcpy(pszResult, auszHash, sizeof(char)* (SHA_DIGEST_LENGTH));
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

	unsigned char input[3] = { 0 };
	unsigned char output[4] = { 0 };
	int index = 0;
	int i = 0;
	int j = 0;
	int iDataSize = 0;
	int	iLen = 0;
	char* p = NULL;
	char* plen = NULL;
	wchar_t strUnicode[MAX_PATH] = { 0 };
	char strUtf8[MAX_PATH] = { 0 };
	char* pszBuffer = NULL;

	plen = pszStr + iSize - 1;
	iDataSize = (4 * (iSize / 3)) + (iSize % 3 ? 4 : 0) + 1;

	pszBuffer = new char[iDataSize];
	memset(&pszBuffer[0], 0, sizeof(char)* iDataSize);

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

	// 결과 copy
	memcpy(pszResult, pszBuffer, iDataSize);

	if (NULL != pszBuffer)
	{
		delete[] pszBuffer;
		pszBuffer = NULL;
	}
}

int CNetScanOnvif::Base64Decoding(char* pszStr, unsigned char* puszResult, int iSize)
{
	/*------ Base64 Decoding Table ------*/
	static int DecodeMimeBase64[256] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 00-0F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 10-1F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,  /* 20-2F */
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,  /* 30-3F */
		-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,  /* 40-4F */
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,  /* 50-5F */
		-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,  /* 60-6F */
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,  /* 70-7F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 80-8F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* 90-9F */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* A0-AF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* B0-BF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* C0-CF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* D0-DF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  /* E0-EF */
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1   /* F0-FF */
	};

	const char* cp;
	int space_idx = 0, phase;
	int d, prev_d = 0;
	unsigned char c;
	space_idx = 0;
	phase = 0;
	for (cp = pszStr; *cp != '\0'; ++cp) {
		d = DecodeMimeBase64[(int)*cp];
		if (d != -1) {
			switch (phase) {
			case 0:
				++phase;
				break;
			case 1:
				c = ((prev_d << 2) | ((d & 0x30) >> 4));
				if (space_idx < iSize)
					puszResult[space_idx++] = c;
				++phase;
				break;
			case 2:
				c = (((prev_d & 0xf) << 4) | ((d & 0x3c) >> 2));
				if (space_idx < iSize)
					puszResult[space_idx++] = c;
				++phase;
				break;
			case 3:
				c = (((prev_d & 0x03) << 6) | d);
				if (space_idx < iSize)
					puszResult[space_idx++] = c;
				phase = 0;
				break;
			}
			prev_d = d;
		}
	}
	return space_idx;
}