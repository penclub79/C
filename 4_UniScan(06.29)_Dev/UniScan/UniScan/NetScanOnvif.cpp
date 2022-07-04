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

	pThis->thrOnvifGetInfo();

	return 0;
}


BOOL CNetScanOnvif::StartScan()
{
	this->StartScanF((LPTHREAD_START_ROUTINE)thrOnvifScanThread);
	return TRUE;
}

void CNetScanOnvif::thrOnvifGetInfo()
{
	BOOL				bIsSuccessBind = FALSE;
	SOCKADDR			stSockAddr;
	int iRevLen = 0;
	char* pszBuff = NULL;


	bIsSuccessBind = SocketBind();

	if (bIsSuccessBind)
	{
		m_pReceive_buffer = new char[SCAN_INFO_m_pReceive_buffer_SIZE];
		memset(m_pReceive_buffer, 0, sizeof(char)*SCAN_INFO_m_pReceive_buffer_SIZE);
		m_pReceive_buffer = (char*)pszBuff;


		while (this->m_dwScanThreadID)
		{
			if (SOCKET_ERROR == recvfrom(this->m_hReceiveSock, m_pReceive_buffer, sizeof(pszBuff), 0, (SOCKADDR*)&stSockAddr, &iRevLen))
			{
				TRACE("recvfrom error\n");
				this->ThreadExit();
				break;
			}

			if (m_pReceive_buffer)
			{
				TRACE("recvfrom Success\n");
				break;
			}
		}
		
	}
}

BOOL CNetScanOnvif::SendScanRequest()
{
	sockaddr_in OnvifSendSock;
	BOOL		bEnable			= NULL;
	SOCKET		hSendSock		= NULL;
	char*		pszBuff			= NULL;
	FILE*		pFile			= NULL;
	DWORD		dwFileSize		= 0;
	XNode		stNode;
	char*		szIpAddr		= "239.255.255.250";


	pFile = fopen("D:\\C_IPScanner\\IP_Scanner\\Probe.txt", "rb");
	fseek(pFile, 0, SEEK_END);
	dwFileSize = ftell(pFile);

	pszBuff = new char[sizeof(char)*dwFileSize];
	memset(pszBuff, 0, dwFileSize);

	fseek(pFile, 0, SEEK_SET);
	fread(pszBuff, 1, dwFileSize, pFile);

	stNode.Load(pszBuff);

	hSendSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (SOCKET_ERROR == setsockopt(hSendSock, SOL_SOCKET, SO_BROADCAST, (char*)&bEnable, sizeof(bEnable)))
	{
		TRACE(_T("SetSocket Error = %d\n", WSAGetLastError()));
	}

	OnvifSendSock.sin_family = AF_INET;
	OnvifSendSock.sin_port = htons(3702);
	OnvifSendSock.sin_addr.s_addr = inet_addr(szIpAddr); //멀티캐스트
	//OnvifSendSock.sin_addr.s_addr = INADDR_BROADCAST; //Send 오류 발생

	if (SOCKET_ERROR == sendto(hSendSock, pszBuff, sizeof(pszBuff), 0, (SOCKADDR*)&OnvifSendSock, sizeof(sockaddr_in)))
	{
		TRACE("Send Error");
	}

	if (NULL != pszBuff)
	{
		delete[] pszBuff;
		pszBuff = NULL;
	}

	return TRUE;
}