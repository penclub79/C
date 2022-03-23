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
	CString m_strUserID;

public:

	//void SendWhoAreYou();

	void SetUserID(CString strID);
	CString GetUserID();
};

