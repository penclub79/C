#pragma once
#include "afxsock.h"
#include "BasicSock.h"

class CAcceptSock :
	public CBasicSock
{
public:
	CAcceptSock();
	~CAcceptSock();

	//void OnReceive(int nErrorCode);
	//virtual void OnClose(int nErrorCode);
	void SetUserID(CString i);
	void GetUserID(CString i);
};

