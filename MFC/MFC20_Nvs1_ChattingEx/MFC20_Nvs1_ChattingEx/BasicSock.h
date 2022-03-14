#pragma once
#include "afxsock.h"
class CBasicSock :
	public CAsyncSocket
{
public:
	CBasicSock();
	~CBasicSock();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);

};

