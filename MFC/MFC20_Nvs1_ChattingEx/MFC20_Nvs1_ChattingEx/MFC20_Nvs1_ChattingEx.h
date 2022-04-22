
// MFC20_Nvs1_ChattingEx.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
// ����

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.
#include "ServerSock.h"
#include "BasicSock.h"
#include "AcceptSock.h"
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
	void ReceiveData(CAcceptSock* pClientSock);
	void CloseChild(CAcceptSock* pClientSock);
	void CloseServer(CAcceptSock* pClientSock);

	void CleanUp();
	void Accept();
	void InitServer();

private:
	CServerSock* m_pServer;

	CObList m_AcceptSocketList;
};

extern CMFC20_Nvs1_ChattingExApp theApp;