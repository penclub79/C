
// MFC20_Nvs1_ChattingEx.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
// 서버

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include "ServerSock.h"
#include "BasicSock.h"
#include "AcceptSock.h"
#include "afxcoll.h"


// CMFC20_Nvs1_ChattingExApp:
// 이 클래스의 구현에 대해서는 MFC20_Nvs1_ChattingEx.cpp을 참조하십시오.
//

class CMFC20_Nvs1_ChattingExApp : public CWinApp
{
public:
	CMFC20_Nvs1_ChattingExApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

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