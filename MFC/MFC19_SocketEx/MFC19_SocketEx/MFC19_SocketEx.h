
// MFC19_SocketEx.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include "BasicSock.h"

// CMFC19_SocketExApp:
// 이 클래스의 구현에 대해서는 MFC19_SocketEx.cpp을 참조하십시오.
//

class CMFC19_SocketExApp : public CWinApp
{
public:
	CMFC19_SocketExApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.
	DECLARE_MESSAGE_MAP()

private:
	// 클래스
	CBasicSock* m_pClient;

	// 자료형
	int m_iConnectCode;


public:
	// 함수
	void CleanUp(void);

	// 연결
	void Connect(CString strIP, int iPort);

	// 연결 끊기
	void Close();

	// 연결 상태
	int GetConnectStatus();
	void SetConnectStatus(int iErrorCode);

	// 사용자ID 받기
	void SetUserID(CString strUserID);

	// 데이터 받기
	void ReceiveData();

	// 데이터 보내기
	void SendData(CString strData);

	// 서버가 끊겼을 때
	void CloseChild();
};

extern CMFC19_SocketExApp theApp;