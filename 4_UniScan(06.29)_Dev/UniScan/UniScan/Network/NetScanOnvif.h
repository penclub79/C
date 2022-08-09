#pragma once

#include "NetScanBase.h"
#include "../../UniScan/MD5.h"

typedef struct _ONVIFINFO
{
	char szInfo[SCAN_INFO_RECEIVE_BUFFER_SIZE];
}ONVIFINFO;

typedef enum {
	Unauthorized = 401,
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
	BOOL SendDeviceInfo(char* pszIP);
	BOOL SendSSDP();
	BOOL CreateSocket();
	BOOL GenerateMsgID(char* szMessageID, int nBufferLen);
	void DigestConvert(char* pszStr, unsigned char* puszResult);
	void SHA1Encoding(char* pszStr, char* pszResult);
	void Base64Encoding(char* pszStr, int iSize, char* pszResult);
	BOOL m_bConnected;

private:

};