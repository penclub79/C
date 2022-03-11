
// MFC19_SocketEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.
#include "ServerSock.h"
#include "ClientSock.h"

// CMFC19_SocketExApp:
// �� Ŭ������ ������ ���ؼ��� MFC19_SocketEx.cpp�� �����Ͻʽÿ�.
//

class CMFC19_SocketExApp : public CWinApp
{
public:
	CMFC19_SocketExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	DECLARE_MESSAGE_MAP()
	
public:
	CServerSock* m_pServer;
	CClientSock* m_pClient;
	void InitServer(void);
	void Accept(void);
	void CleanUp(void);
	void Connect(CString strIP);
	void ReceiveData(void);
	void SendData(CString strData);
	void CloseChild(void);
};

extern CMFC19_SocketExApp theApp;