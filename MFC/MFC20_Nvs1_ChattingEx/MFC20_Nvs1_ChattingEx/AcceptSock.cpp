#include "stdafx.h"
#include "AcceptSock.h"
#include "MFC20_Nvs1_ChattingEx.h"


CAcceptSock::CAcceptSock()
{

}


CAcceptSock::~CAcceptSock()
{
}

//void CAcceptSock::SendWhoAreYou()
//{
//	PACKET_HEADER	stWhoAreYou = { 0 };
//	
//	stWhoAreYou.iPacketID = PACKET_ID_REQ_WHOAREYOU;
//
//	Send((LPCTSTR)&stWhoAreYou, sizeof(stWhoAreYou));
//}


//void CAcceptSock::OnReceive(int nErrorCode)
//{
//
//	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//	((CMFC20_Nvs1_ChattingExApp*)AfxGetApp())->ReceiveData(this);
//
//	CAsyncSocket::OnReceive(nErrorCode);
//}

void CAcceptSock::SetUserID(CString strID)
{
	m_strUserID = strID;
}

CString CAcceptSock::GetUserID()
{
	return m_strUserID;
	//((CMFC20_Nvs1_ChattingExApp*)AfxGetApp)->ReceiveData();
}

