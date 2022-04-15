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
	void Close();
	
private:
	PCSTR m_strIP;
	CString m_strUserID;
	int m_iPort;

	int GetPort();
	int MainThread();
	SOCKET	m_uiSocket;
	WSAEVENT m_wsaEvent;
	DWORD m_dwThreadID;
	HANDLE m_hThread;
};

