#pragma once
#include "afxsock.h"
#include "BasicDefine.h"



class CBasicSock
{
public:
	CBasicSock();
	~CBasicSock();
	
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
};

