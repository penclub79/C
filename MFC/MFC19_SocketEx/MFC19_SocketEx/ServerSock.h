#pragma once
#include "afxsock.h"
#include "ServerSock.h"

class CServerSock :
	public CAsyncSocket
{
public:
	CServerSock();
	~CServerSock();
	virtual void OnAccept(int nErrorCode);
};

