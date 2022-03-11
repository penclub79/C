#pragma once
#include "afxsock.h"
class CClientSock :
	public CAsyncSocket
{
public:
	CClientSock();
	~CClientSock();
};

