#pragma once
#include "afxsock.h"
#include "BasicDefine.h"


// Ŭ���̾�Ʈ ����ü
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

