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
		//mreq.imr_interface.s_addr = htonl(INADDR_ANY); // INADDR_ANY�� ����

		// ��Ƽĳ��Ʈ �׷� ����
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
	
	// ���� ����
	if (NULL == m_hReceiveSock)
		CreateMultiCastSocket();

	if (NULL == m_hReceiveSock)
		return FALSE;

	OnvifSendSock.sin_family = AF_INET;
	OnvifSendSock.sin_port = htons(3702);
	OnvifSendSock.sin_addr.s_addr = inet_addr("239.255.255.250"); // multicast group

	int iTypeIdx = 0;
	int iUriIdx = 0;
	// Probe Type �迭 ��� ���� ��ŭ Packet Send
	while (iTypeIdx < sizeof(paszProbeType) / sizeof(paszProbeType[0]))
	{
		// UUID ���
		GenerateMsgID(szMessageID, 127);

		if (iTypeIdx > 1)
			iUriIdx++;

		iSendBufferSize = strlen(aszProbeXml) + strlen(szMessageID)+ strlen(paszOnvifURI[iUriIdx]) + strlen(paszProbeType[iTypeIdx]);
		pszSendBuffer = new char[iSendBufferSize + 1];
		memset(pszSendBuffer, 0, iSendBufferSize + 1);

		// ���� UUID�� XML�� ����
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
	SOCKADDR		stSockAddr;
	XNode			stNode;
	BOOL			bIsSuccessBind			= FALSE;
	int				iRevLen					= sizeof(sockaddr_in);
	LPXNode			lpTypeCheck				= NULL;
	LPXNode			lpBody					= NULL;
	LPXNode			lpUUID					= NULL;
	LPXNode			lpIPAddress				= NULL;
	LPXNode			lpScope					= NULL;
	int				iHTTPPort				= 0;
	char			aszUUID[128]			= { 0 };
	char			aszIPData[128]			= { 0 };
	char*			pszData					= NULL;
	char			aszIPAddress[32]		= { 0 };
	char*			pszSlice				= NULL;
	char*			pszNameSlice			= NULL;
	char*			pszMacSlice				= NULL;
	DWORD			dwLastError				= 0;
	SCAN_INFO*		pScanInfo				= NULL;
	char*			pszProbeMatchType[2]	= { "wsdd:ProbeMatch", "d:ProbeMatch" };
	char*			pszAddressType[2]		= { "wsdd:XAddrs", "d:XAddrs" };
	BOOL			bIsTCPConnect			= FALSE;
	char			aszDigest[48]			= { 0 };
	char			aszNonceResult[36]		= { 0 }; // Base64�� ���ڵ��� 16����Ʈ���� �Ѵ�.
	char			aszDateResult[56]		= { 0 };
	char			aszPwdDigest[64]		= { 0 };
	char			aszPwdShaHash[41]		= { 0 };
	char			aszPwdBaseHash[64]		= { 0 };
	char			aszNonceBase64[36]		= { 0 };
	char			aszUserPwd[16]			= "111111";
	unsigned char	auszNonceDecode[64]		= { 0 };
	char*			pszDeviceInfoData		= NULL;
	char*			pszMacType				= NULL;
	char			aszFirmwareVer[40] = { 0 };

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
			// �Ľ��� ������
			stNode.Load(m_pReceive_buffer);
			lpBody = stNode.GetChildArg("SOAP-ENV:Body", NULL);
			if (NULL != lpBody)
			{
				lpUUID = lpBody->GetChildArg("wsa:Address", NULL);
				if (NULL != lpUUID)
				{
					// urn:uuid:XXXXXX-XXXXXX-XXXX ���� urn:uuid: ���ڿ��� �ڸ�
					lpUUID->value = lpUUID->value.Right(36);
					strcpy(&aszUUID[0], lpUUID->value);
					// �迭�� uuid �װ� �� �׾����� uuid��Resolve Message�� ��Ƽ� �ٽ� Send
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

						pszDeviceInfoData = new char[sizeof(char)* SCAN_INFO_RECEIVE_BUFFER_SIZE];
						memset(&pszDeviceInfoData[0], 0, sizeof(char)* SCAN_INFO_RECEIVE_BUFFER_SIZE);

						//// ProbeMatch���� ���� IP���� ��ŭ �ݺ������� ���� ���� - ������ 1���� �׽�Ʈ ��
						GetAuthenticateData(aszIPAddress, &aszDateResult[0], &aszNonceResult[0], &pszDeviceInfoData[0]); // Device ��¥ ���
						
						if (0 < strlen(aszNonceResult))
						{							
							Base64Encoding(aszNonceResult, strlen(aszNonceResult), &aszNonceBase64[0]); // nonce�� Base64�� ���ڵ�
							//Base64Decoding(aszNonceResult, &auszNonceDecode[0], sizeof(auszNonceDecode));

							sprintf_s(aszPwdDigest, sizeof(aszPwdDigest), "%s%s%s", aszNonceResult, aszDateResult, aszUserPwd);

							SHA1Encoding(aszPwdDigest, &aszPwdShaHash[0]);
							Base64Encoding(aszPwdShaHash, SHA_DIGEST_LENGTH, &aszPwdBaseHash[0]);

							SendAuthentication(aszIPAddress, aszPwdBaseHash, aszNonceBase64, aszDateResult, &pszDeviceInfoData[0]);
						}

						if (0 < strlen(pszDeviceInfoData))
						{
							stNode.Load(pszDeviceInfoData);
							lpBody = stNode.GetChildArg("tds:GetDeviceInformationResponse", NULL);
							lpBody = lpBody->GetChildArg("tds:FirmwareVersion", NULL);

							// FirwareVersion Value Check
							if ( 0 < lpBody->value.GetLength() )
							{
								strcpy(&aszFirmwareVer[0], lpBody->value);
								//this->WideCopyStringFromAnsi(pScanInfo->pExtScanInfos);
							}
						}
					}

					lpScope = ( NULL != lpTypeCheck->GetChildArg("wsdd:Scopes", NULL) ) ? lpTypeCheck->GetChildArg("wsdd:Scopes", NULL) : lpTypeCheck->GetChildArg("d:Scopes", NULL);

					if (NULL != lpScope)
					{
						pszData = new char[sizeof(char) * lpScope->value.GetLength() + 1];
						strcpy(&pszData[0], lpScope->value);

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
							pszMacSlice = strtok(pszMacSlice, " ");
							pszMacType = strtok(pszMacSlice, "/");
							( 0 == strcmp(pszMacType, "macaddress") ) ? pszMacSlice = strncpy(pszMacSlice, pszMacSlice + 11, 20) : pszMacSlice = strncpy(pszMacSlice, pszMacSlice + 4, 20);

							strlwr(pszMacSlice); // �ҹ��ڷ� ��ȯ

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
		}

		if (NULL != pszData)
		{
			delete[] pszData;
			pszData = NULL;
		}

		if (NULL != pszDeviceInfoData)
		{
			delete[] pszDeviceInfoData;
			pszDeviceInfoData = NULL;
		}
	}

	return;
}

//void CNetScanOnvif::UtfConvert(char* pszStr, char* pszResult)
//{
//	//wchar_t awszUnicode[128] = { 0 };
//	wchar_t awszUnicode[2048] = { 0 };
//	int iLen = 0;
//
//	// MultiByte -> Unicode
//	iLen = MultiByteToWideChar(CP_ACP, 0, pszStr, strlen(pszStr), NULL, NULL);
//	MultiByteToWideChar(CP_ACP, 0, pszStr, strlen(pszStr), awszUnicode, iLen);
//
//	// Unicode -> UTF-8
//	iLen = WideCharToMultiByte(CP_UTF8, 0, awszUnicode, lstrlenW(awszUnicode), NULL, 0, NULL, NULL);
//	WideCharToMultiByte(CP_UTF8, 0, awszUnicode, lstrlenW(awszUnicode), pszResult, iLen, NULL, NULL);
//}


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
	// GetSystemDate Send & Recv ����
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
void CNetScanOnvif::SendDeviceInfo(char* pszIP, char* pszNonceResult, char* pszGetData)
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
	int				iHTTPStatus			= 0;
	char*			pszStrCompare		= "nonce";
	BOOL			bIsConnect			= FALSE;
	int				iSendBufferSize		= 0;
	char			aszCopyBuffer[20] = { 0 };

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

	if (0 < strlen(pszNonceResult))
	{
		memset(pszNonceResult, 0, sizeof(char) * 36);
	}

	bIsConnect = ConnectTCPSocket(pszIP);

	if (CONNECT_SUCCESS == bIsConnect)
	{
		iSendBufferSize = strlen(aszGetSystemTime) + strlen(aszDeviceInformation) + strlen(pszIP) + 4;
		
		pszSendBuffer = new char[sizeof(char) * iSendBufferSize + 1];
		memset(&pszSendBuffer[0], 0, iSendBufferSize + 1);

		pszRecvBuffer = new char[SCAN_INFO_RECEIVE_BUFFER_SIZE];
		memset(&pszRecvBuffer[0], 0, SCAN_INFO_RECEIVE_BUFFER_SIZE);

		sprintf_s(pszSendBuffer, sizeof(char) * iSendBufferSize, aszGetSystemTime, pszIP, strlen(aszDeviceInformation));
		strcat_s(pszSendBuffer, sizeof(char) * iSendBufferSize, aszDeviceInformation);

		iSendDataSize = strlen(pszSendBuffer);
		if ( SOCKET_ERROR == send(m_TcpSocket, pszSendBuffer, iSendDataSize, 0) )
		{
			iError = WSAGetLastError();
			TRACE(_T("TCP-HTTP send Error = %d\n"), iError);
			closesocket(m_TcpSocket);
		}

		if ( SOCKET_ERROR == recv(m_TcpSocket, pszRecvBuffer, sizeof(char) * SCAN_INFO_RECEIVE_BUFFER_SIZE, 0) )
		{
			iError = WSAGetLastError();
			TRACE(_T("TCP-HTTP recv Error = %d\n"), iError);
			closesocket(m_TcpSocket);
		}

		::OutputDebugStringA("ONVIF RECEIVE DEVICE DATA -----------------------\n");
		::OutputDebugStringA(pszIP);
		::OutputDebugStringA("\n");
		::OutputDebugStringA(pszRecvBuffer);
		::OutputDebugStringA("\n");

		if (0 < strlen(pszRecvBuffer))
		{
			memcpy(&aszCopyBuffer[0], pszRecvBuffer, 20);

			// 401 Status ã��
			pszSlice = strtok(aszCopyBuffer, " ");
			pszSlice = strtok(NULL, " ");
			iHTTPStatus = atoi(pszSlice);

			// 200 �϶�
			if (RES_SUCCESS == iHTTPStatus)
			{
				strcpy_s(pszGetData, sizeof(char)* SCAN_INFO_RECEIVE_BUFFER_SIZE, pszRecvBuffer);
				pszNonceResult = NULL;
			}

			if (UNAUTHORIZED == iHTTPStatus)
			{
				pszSlice = strtok(pszRecvBuffer, " ");
				pszSlice = strtok(NULL, " ");
				while (NULL != pszRecvBuffer)
				{
					// nonce ã��
					pszSlice = strtok(NULL, ", =");
					if (0 == strcmp(pszSlice, pszStrCompare))
					{
						// Hash �� ���
						pszSliceNonce = strtok(NULL, "\" \"");
						strcat_s(pszNonceResult, sizeof(char)* strlen(pszSliceNonce) + 1, pszSliceNonce);
						pszGetData = NULL;
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

}


// SystemDate Request
void CNetScanOnvif::GetAuthenticateData(char* pszIP, char* pszDateResult, char* pszNonceResult, char* pszGetData)
{
	XNode		stNode;
	LPXNode		lpaDateType[6]		= { 0 }; // CStringA
	LPXNode		lpaTimeData[6]		= { 0 };
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
	BOOL		bIsGetNonce			= FALSE;

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

	iSendBufferSize = strlen(aszGetSystemTime) + strlen(pszIP) + strlen(aszSystemDateXml) + 4;
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
		}

		if (SOCKET_ERROR == recv(m_TcpSocket, pszRecvBuffer, SCAN_INFO_RECEIVE_BUFFER_SIZE, 0))
		{
			iError = WSAGetLastError();
			TRACE(_T("TCP-HTTP recv Error = %d\n"), iError);
			closesocket(m_TcpSocket);
		}

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
			sprintf_s(aszTime, sizeof(aszTime), "%04d-%02d-%02dT%02d:%02d:%02d.000Z",
				atoi(paiDate[0]),
				atoi(paiDate[1]),
				atoi(paiDate[2]),
				atoi(paiDate[3]),
				atoi(paiDate[4]),
				atoi(paiDate[5])
				);

		} // Created ��¥ �������� 
	}

	memcpy(pszDateResult, aszTime, sizeof(char) * strlen(aszTime));

	SendDeviceInfo(pszIP, &pszNonceResult[0], &pszGetData[0]); // Nonce �ؽ� ���

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
}

void CNetScanOnvif::SendAuthentication(char* pszIP, char* pszDigest, char* pszNonceResult, char* pszDateResult, char* pszGetData)
{
	BOOL		bIsConnect			= FALSE;
	char*		pszRecvBuffer		= NULL;
	char*		pszPacketBuffer		= NULL;
	char*		pszSendBuffer		= NULL;
	int			iHeaderSize			= 0;
	int			iBodySize			= 0;
	int			iPacketSize			= 0;
	int			iError				= 0;
	int			iContentLen			= 0;
	char*		pszSlice			= NULL;
	int			iHTTPStatus = 0;
	char		aszCopyBuffer[20]	= { 0 };


	char aszDeviceInfoHeader[] = "POST /onvif/device_service HTTP/1.1\r\nContent-Type: application/soap+xml; charset=utf-8; action=\"http://www.onvif.org/ver10/device/wsdl/GetDeviceInformation\"\r\nHost: %s\r\nContent-Length: %d\r\nAccept-Encoding: gzip, deflate\r\nConnection: Close\r\n\r\n";

	char aszDeviceInformationXML[] = 
	{
		"<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">\
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
				<GetDeviceInformation xmlns=\"http://www.onvif.org/ver10/device/wsdl\"/>\
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

		if (0 < strlen(pszRecvBuffer))
		{
			memcpy(&aszCopyBuffer[0], pszRecvBuffer, 20);
			pszSlice = strstr(aszCopyBuffer, "200");
			if (NULL != pszSlice)
			{
				pszSlice = strtok(pszSlice, " ");
				iHTTPStatus = atoi(pszSlice);

				if (RES_SUCCESS == iHTTPStatus)
				{
					// 200 OK �϶�
					strcpy_s(pszGetData, sizeof(char) * SCAN_INFO_RECEIVE_BUFFER_SIZE, pszRecvBuffer);
				}
			}
		}

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
	unsigned char	auszHash[SHA_DIGEST_LENGTH]			= { 0 };


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

	// ��� copy
	memcpy(pszResult, pszBuffer, iDataSize);

	if (NULL != pszBuffer)
	{
		delete pszBuffer;
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