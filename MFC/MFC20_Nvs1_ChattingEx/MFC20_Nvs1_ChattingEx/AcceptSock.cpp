#include "stdafx.h"
#include "AcceptSock.h"
#include "MFC20_Nvs1_ChattingEx.h"


CAcceptSock::CAcceptSock()
{

}


CAcceptSock::~CAcceptSock()
{
}

void CAcceptSock::SendWhoAreYou()
{
	PACKET_HEADER	stWhoAreYou = { 0 };
	
	stWhoAreYou.iPacketID = PACKET_ID_REQ_WHOAREYOU;

	Send((LPCTSTR)&stWhoAreYou, sizeof(stWhoAreYou));
}


//void CAcceptSock::OnReceive(int nErrorCode)
//{
//
//	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//	((CMFC20_Nvs1_ChattingExApp*)AfxGetApp())->ReceiveData(this);
//
//	CAsyncSocket::OnReceive(nErrorCode);
//}

void CAcceptSock::SetUserID(int iID)
{
	iIndex = iID;
}

int CAcceptSock::GetUserID()
{
	return iIndex;
	//((CMFC20_Nvs1_ChattingExApp*)AfxGetApp)->ReceiveData();
}

