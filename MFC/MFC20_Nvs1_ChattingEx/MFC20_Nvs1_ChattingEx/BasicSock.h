#pragma once
#include "SyncSocket.h"
#include "BasicDefine.h"


class CBasicSock :
	public CSyncSocket
{
public:
	CBasicSock();
	~CBasicSock();

	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);

};

