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
	static void CALLBACK TimerProc(HWND _hWnd, UINT _uiMessage, UINT _uiIDEvent, DWORD _ulTime);

	void ReceivePacket(PACKET_HEADER* _pstHeader, char* _pszPacket);
	void ServerExit();
	HWND GetParent();
	int MainThread();
	void SendAliveSignal();
	void KeepAlive();

	HWND		m_hParentHandle;

	CString		m_strIP;
	CString		m_strUserID;
	SOCKET		m_uiSocket;
	WSAEVENT	m_wsaEvent;
	DWORD		m_dwThreadID;
	HANDLE		m_hThread;
	DWORD		m_ulStartTime;
	DWORD		m_ulLastTime;
	int			m_iPort;
	int			m_iConnResult;
	int			m_iAliveStatus;

	

};

