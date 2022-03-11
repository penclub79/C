#pragma once
#include "afxsock.h"
class CServerSock :
	public CAsyncSocket
{
public:
	CServerSock();
	~CServerSock();
};

