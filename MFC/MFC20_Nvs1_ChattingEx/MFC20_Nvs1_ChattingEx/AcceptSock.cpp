#include "stdafx.h"
#include "AcceptSock.h"
#include "MFC20_Nvs1_ChattingEx.h"


CAcceptSock::CAcceptSock()
{

}

CAcceptSock::~CAcceptSock()
{
}

void CAcceptSock::SetUserID(CString strID)
{
	m_strUserID = strID;
}

CString CAcceptSock::GetUserID()
{
	return m_strUserID;
	//((CMFC20_Nvs1_ChattingExApp*)AfxGetApp)->ReceiveData();
}

