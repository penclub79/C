#pragma once
#include "afxsock.h"

class CAcceptSock :
	public CAsyncSocket
{
public:
	CAcceptSock();
	~CAcceptSock();
	virtual void OnAccept(int nErrorCode);
};

