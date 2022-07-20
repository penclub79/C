#pragma once

#include "./Network/NetScanBase.h"


typedef struct _ONVIFINFO
{
	char szInfo[SCAN_INFO_m_pReceive_buffer_SIZE];
}ONVIFINFO;


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
	BOOL SendAuthentication(char* pszUUID);

	BOOL CreateSocket();
	BOOL GenerateMsgID(char* szMessageID, int nBufferLen);

	BOOL m_bConnected;

private:

};