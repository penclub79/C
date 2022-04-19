#pragma once
#include "BasicDefine.h"

class CBasicSock
{

public:
	CBasicSock();
	~CBasicSock();
	
	static DWORD WINAPI ThreadProc(LPVOID _lpParam);
	void Connect(PCSTR _strIP, CString _strUserID, int _iPort);
	void Write(char* _pData, int _iLength);
	void SendPacket(int _iPacketID);
	void ReceivePacket(int _iPacketID, PACKET_RSP_LOGIN* pszPacket);
	void Close();
	
private:
	
	PCSTR m_strIP;
	CString m_strUserID;
	int m_iPort;
	SOCKET	m_uiSocket;
	WSAEVENT m_wsaEvent;
	DWORD m_dwThreadID;
	HANDLE m_hThread;
	char* m_pszBuff;
	int   m_iLength;

	int MainThread();
	

};

