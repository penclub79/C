#pragma once
#include "afxsock.h"

class CServerSock :
	public CAsyncSocket
{
public:
	CServerSock();
	~CServerSock();
	virtual void OnAccept(int nErrorCode);
};

