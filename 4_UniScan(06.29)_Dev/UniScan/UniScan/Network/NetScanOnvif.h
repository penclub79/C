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
	void SendDeviceInfo(char* pszIP, int iPort, char* pszNonceResult, char* pszGetData);
	void GetOnvifVersion(char* pszIP, int iPort, char* pszGetData);
	void GetAuthenticateData(char* pszIP, int iPort, char* pszDateResult, char* pszNonceResult, char* pszGetData);
	void GetDeviceInfo(char* pszIP, int iPort, char* pszDigest, char* pszNonceResult, char* pszDateResult, char* pszGetData);
	void GetNetworkInterface(char* pszIP, int iPort, char* pszDigest, char* pszNonceResult, char* pszDateResult, char* pszGetData);
	BOOL SendSSDP();
	BOOL CreateMultiCastSocket();
	BOOL ConnectTCPSocket(char* pszIP, int iPort);
	BOOL GenerateMsgID(char* szMessageID, int nBufferLen);
	void UtfConvert(char* pszStr, char* pszResult);
	void DigestConvert(char* pszStr, char* puszResult);
	void SHA1Encoding(char* pszStr, char* pszResult);
	void Base64Encoding(char* pszStr, int iSize, char* pszResult);
	int Base64Decoding(char* pszStr, unsigned char* puszResult, int iSize);
	BOOL m_bConnected;
	SOCKET m_TcpSocket;


private:

};