#pragma once
#include "afxsock.h"
#include "BasicDefine.h"


// 클라이언트 구조체
struct Client
{
	int iPort;
	CString szUserId;
};

class CBasicSock :
	public CAsyncSocket
{
public:
	CBasicSock();
	~CBasicSock();
	bool m_bIsConnect;

	
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
};

