#pragma once

#include "./Network/NetScanBase.h"

class CNetScanOnvif : public NetScanBase
{
public:
	CNetScanOnvif();
	~CNetScanOnvif(void);

	virtual BOOL StartScan();
	virtual BOOL SendScanRequest();

protected:
	static DWORD thrOnvifScanThread(LPVOID pParam);
	void thrOnvifGetInfo();

private:

};