#include "stdafx.h"
#include "ClientSock.h"
#include "MFC20_Nvs1_ChattingEx.h"


CBasicSock::CBasicSock()
{
}


CBasicSock::~CBasicSock()
{
}





void CBasicSock::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	((CMFC20_Nvs1_ChattingExApp*)AfxGetApp())->ReceiveData(this);

	CAsyncSocket::OnReceive(nErrorCode);
}


void CBasicSock::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	((CMFC20_Nvs1_ChattingExApp*)AfxGetApp())->CloseChild(this);

	CAsyncSocket::OnClose(nErrorCode);
}
