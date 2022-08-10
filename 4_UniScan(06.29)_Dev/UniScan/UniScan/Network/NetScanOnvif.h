#pragma once

#include "NetScanBase.h"
#include "../../UniScan/MD5.h"

typedef struct _ONVIFINFO
{
	char szInfo[SCAN_INFO_RECEIVE_BUFFER_SIZE];
}ONVIFINFO;

typedef enum _TCPCODE{
	CONNECT_SUCCESS = 1,
};

typedef enum _HTTPCODE{
	RES_SUCCESS		= 200,
	BAD_REQUEST		= 400,
	UNAUTHORIZED	= 401
};


class CNetScanOnvif : public NetScanBase
{
public:
	CNetScanOnvif();
	~CNetScanOnvif(void);

	virtual BOOL StartScan();
	virtual BOOL SendScanRequest();

protected:
	static DWORD thrOnvifScanThread(LPVOID pParam);
	void thrOnvifReceiver();
	void SendDeviceInfo(char* pszIP, char* pszNonceResult);
	void SendSystemDate(char* pszIP, char* pszDateResult);
	void SendAuthentication(char* pszDigest, char* pszNonceResult, char* pszDateResult);
	BOOL SendSSDP();
	BOOL CreateMultiCastSocket();
	BOOL ConnectTCPSocket(char* pszIP);
	BOOL GenerateMsgID(char* szMessageID, int nBufferLen);
	void DigestConvert(char* pszStr, char* puszResult);
	void SHA1Encoding(char* pszStr, char* pszResult);
	void Base64Encoding(char* pszStr, int iSize, char* pszResult);
	BOOL m_bConnected;
	SOCKET m_TcpSocket;

private:

};