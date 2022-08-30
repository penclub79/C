#pragma once

#include "NetScanBase.h"
#include "../../UniScan/MD5.h"
#include "../xmlite/XMLite.h"
#include "../../UniScan/XMLform.h"

#define NOT_DATA	0
#define SHA_SIZE	41
#define BASE64_SIZE	56
#define UUID_SIZE	128
#define DIGEST_SIZE 64

typedef struct tagONVIFINFO
{
	char	aszIP[24];
	char	aszMAC[30];
	int		iPort;
	char	aszDigest[DIGEST_SIZE];
	char	aszNonce[36];
	char	aszDate[56];
	char*	pszGetData;
	char   aszVersion[8];

}ONVIF_INFO;

typedef struct tagNODECHILD
{
	LPXNode lpBody;
	LPXNode	lpDeviceInfoBody;
	LPXNode lpUUID;
	LPXNode lpIPAddress;
	LPXNode lpMAC;
	LPXNode lpMajor;
	LPXNode lpMinor;
}NODECHILD;

typedef struct tagNODE
{
	XNode	stNode;
	XNode	stDeviceInfoNode;
	XNode	stNetworkInfoNode;
	XNode	stOnvifVersionNode;
	NODECHILD stChild;
}NODE;

typedef enum 
{
	PROBEMATCH = 0,
	DEVICEINFO,
	VERSIONINFO,
	NETWORKINFO,
	PROFILEINFO
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
	void SendDeviceInfo(char* pszIP, int iPort, char* pszNonceResult, char* pszGetData);
	void GetOnvifVersion(char* pszIP, int iPort, char* pszGetData);
	void GetAuthenticateData(char* pszIP, int iPort, char* pszDateResult, char* pszNonceResult, char* pszGetData);
	void GetDeviceInfo(char* pszIP, int iPort, char* pszDigest, char* pszNonceResult, char* pszDateResult, char* pszGetData);
	void GetNetworkInterface(char* pszIP, int iPort, char* pszDigest, char* pszNonceResult, char* pszDateResult, char* pszGetData);
	//void GetProfile(char* pszIP, int iPort, char* pszDigest, char* pszNonceResult, char* pszDateResult, char* pszGetData);
	BOOL SendSSDP();
	BOOL CreateMultiCastSocket();
	BOOL ConnectTCPSocket(char* pszIP, int iPort);
	BOOL GenerateMsgID(char* szMessageID, int nBufferLen);
	void UtfConvert(char* pszStr, char* pszResult);
	void DigestConvert(char* pszStr, char* puszResult);
	void SHA1Encoding(char* pszStr, char* pszResult);
	void Base64Encoding(char* pszStr, int iSize, char* pszResult);
	int	Base64Decoding(char* pszStr, unsigned char* puszResult, int iSize);
	//void RecvStatus(char* pszRecvData, int iLabel);						// Pre-treat's Status Value
	//void RecvDataParsing(NODE* pNode, char* pszRecvData, int iLabel);			// Recieve Data Pre-treat Func
	//void RecvFault(int iLabel, int iHTTPStatus);

	BOOL m_bConnected;
	BOOL m_bIsProbeRev;
	SOCKET m_TcpSocket;


private:

};