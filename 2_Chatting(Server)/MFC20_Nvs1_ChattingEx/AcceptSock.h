#pragma once
#include "afxsock.h"
#include "BasicSock.h"

class CAcceptSock :
	public CBasicSock
{
public:
	CAcceptSock();
	~CAcceptSock();

protected:
	CString m_strUserID;

public:

	//void SendWhoAreYou();

	void SetUserID(CString strID);
	CString GetUserID();
};

