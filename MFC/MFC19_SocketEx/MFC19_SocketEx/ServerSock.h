#pragma once
#include "afxsock.h"
#include "ServerSock.h"
#include "MFC19_SocketEx.h"

class CServerSock :
	public CAsyncSocket
{
public:
	CServerSock();
	~CServerSock();
	virtual void OnAccept(int nErrorCode);
};

