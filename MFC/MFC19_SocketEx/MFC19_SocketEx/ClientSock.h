#pragma once
#include "afxsock.h"
#include "ClientSock.h"

class CClientSock :
	public CAsyncSocket
{
public:
	CClientSock();
	~CClientSock();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

