
// MFC20_Nvs1_ChattingEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
// ����

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.
#include "ServerSock.h"
#include "BasicSock.h"
#include "afxcoll.h"


// CMFC20_Nvs1_ChattingExApp:
// �� Ŭ������ ������ ���ؼ��� MFC20_Nvs1_ChattingEx.cpp�� �����Ͻʽÿ�.
//

class CMFC20_Nvs1_ChattingExApp : public CWinApp
{
public:
	CMFC20_Nvs1_ChattingExApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:

public:
	CServerSock* m_pServer;
	//CClientSock* m_pClient;
	void InitServer();
	void Accept();
	CBasicSock* m_pAccept;
	void CleanUp();
	//void Connect(CString strIP);
	void ReceiveData(CBasicSock* pClientSock);
	void SendData(CString strData);
	void CloseChild(CBasicSock* pClientSock);
	CObList m_ClientList;
};

extern CMFC20_Nvs1_ChattingExApp theApp;