#pragma once

#include "NetScanBase.h"
#include "../../UniScan/MD5.h"
#include "../xmlite/XMLite.h"
#include "../../UniScan/XMLform.h"

#define IP_SIZE		32
#define NONCE_SIZE	36
#define SHA_SIZE	41
#define BASE64_SIZE	56
#define DATE_SIZE	56
#define DIGEST_SIZE 64
#define UUID_SIZE	128


typedef struct tagONVIFINFO
{
	char	aszIP[32];
	char	aszMAC[30];
	int		iHttpPort;
	int		iChannelCnt;
	char	aszNonce[36];
	char	aszDate[56];
	char	aszModelName[30];
	char	aszVersion[8];
	char	aszFirmwareVer[32];

}ONVIF_INFO;

typedef enum 
{
	PROBEMATCH = 0,
	SYSTEMDATE,
	DEVICEINFO,
	VERSIONINFO,
	DEVICEINFO_DIGEST,
	NETWORKINFO_DIGEST,
	PROFILEINFO_DIGEST
};

typedef enum _TCPCODE{
	CONNECT_FAIL	= 0,
	CONNECT_SUCCESS,
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
	void SendDeviceInfo(ONVIF_INFO* pstOnvifInfo);
	void SendOnvifVersion(ONVIF_INFO* pstOnvifInfo);
	void SendAuthenticate(ONVIF_INFO* pstOnvifInfo);
	void GetDeviceInfo(ONVIF_INFO* pstOnvifInfo, char* pszNonceResult);
	void GetNetworkInterface(ONVIF_INFO* pstOnvifInfo, char* pszNonceResult);
	void GetIPAndModelName(ONVIF_INFO* pstOnvifInfo);
	void DataParsing(ONVIF_INFO* pstOnvifInfo, SCAN_INFO* pstScanInfo);
	BOOL SendSSDP();
	BOOL CreateMultiCastSocket();
	BOOL ConnectTCPSocket(char* pszIP, int iPort);
	BOOL GenerateMsgID(char* pszMessageID, int iBufferLen);
	void DataPreProcessing(); // ±¸Çö °èÈ¹
	void SoapRequestMessage(int iReqType, int iHttpHeaderSize, int iContentSize);
	void GetDeviceInfo(ONVIF_INFO* pstOnvifInfo);
	void GetNetworkInterface(ONVIF_INFO* pstOnvifInfo);

	char m_aszDigest[DIGEST_SIZE];
	char m_aszBase64[NONCE_SIZE];
	char m_aszDate[DATE_SIZE];
	char m_aszIP[IP_SIZE];
	BOOL m_bConnected;
	BOOL m_bIsRunThr;
	SOCKET m_TcpSocket;

private:
	void Base64Encoding(char* pszStr, int iSize, char* pszResult);
	int	Base64Decoding(char* pszStr, unsigned char* puszResult, int iSize);
	void UtfConvert(char* pszStr, char* pszResult);
	void DigestConvert(char* pszStr, char* puszResult);
	void SHA1Encoding(char* pszStr, char* pszResult);
};