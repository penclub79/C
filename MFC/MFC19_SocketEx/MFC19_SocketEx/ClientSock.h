#pragma once
#include "afxsock.h"
#include "ClientSock.h"
#include "MFC19_SocketEx.h"

class CClientSock :
	public CAsyncSocket
{
public:
	CClientSock();
	~CClientSock();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

