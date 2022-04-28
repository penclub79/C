#pragma once
#include "BasicDefine.h"

#define WM_MESSAGE_SOCKET		WM_USER+100
#define NUM_THREAD 1
#define CLIENTPORT


class CBasicSock
{

public:
	CBasicSock(HWND hParent);
	~CBasicSock();
	
	void Connect(TCHAR* _pszIP, TCHAR* _pszUserID, int _iPort);
	void SendPacket(int _iPacketID, TCHAR* _pData, int _iLength);
	void Close();


private:
	static DWORD WINAPI ThreadProc(LPVOID _lpParam);

	void ReceivePacket(PACKET_HEADER* _pstHeader, char* _pszPacket);
	void ServerExit();
	HWND GetParent();
	int MainThread();
	void SendAliveSignal();

	HWND		m_hParentHandle;

	CString		m_strIP;
	CString		m_strUserID;
	SOCKET		m_uiSocket;
	WSAEVENT	m_wsaEvent;
	DWORD		m_dwThreadID;
	HANDLE		m_hThread;
	int			m_iPort;
	int			m_iConnResult;

	

};

