#pragma once
#include "afxsock.h"
#include "BasicSock.h"

class CAcceptSock :
	public CBasicSock
{
public:
	CAcceptSock();
	~CAcceptSock();

	//int iIndex;
	//void OnReceive(int nErrorCode);
	//virtual void OnClose(int nErrorCode);
protected:
	int iIndex;

public:

	void SendWhoAreYou();

	void SetUserID(int iID);
	int GetUserID();
};

