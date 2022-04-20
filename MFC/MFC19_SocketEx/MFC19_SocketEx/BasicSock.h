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
	
	static DWORD WINAPI ThreadProc(LPVOID _lpParam);
	void Connect(PCSTR _strIP, CString _strUserID, int _iPort);
	//void Write(TCHAR* _pData, int _iLength);
	void SendPacket(int _iPacketID, TCHAR* _pData, int _iLength);
	void ReceivePacket(PACKET_HEADER* _pstHeader, char* _pszPacket);
	void DisConnect();
	void Close();
	HWND		GetParent();
	

private:
	HWND		m_hParentHandle;

	PCSTR		m_strIP;
	CString		m_strUserID;
	int			m_iPort;
	SOCKET		m_uiSocket;
	WSAEVENT	m_wsaEvent;
	DWORD		m_dwThreadID;
	HANDLE		m_hThread;

	/*char* m_pszBuff;
	int   m_iLength;*/
	
	int MainThread();
	

};

