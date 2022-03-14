#pragma once
#include "afxsock.h"

class CBasicSock :
	public CAsyncSocket
{
public:
	CBasicSock();
	~CBasicSock();
	virtual void OnAccept(int nErrorCode);
};

