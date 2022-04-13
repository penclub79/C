#pragma once
#include "BasicDefine.h"

class CBasicSock
{
public:
	CBasicSock();
	~CBasicSock();
	
	int MainThread();

	void Connect(CString _strIP, CString _strUserID, int _iPort);
	void Write(char* _pData, int _iLength);
	void Close();

private:
	DWORD m_dwThreadID;
	HANDLE m_hThread;
};

