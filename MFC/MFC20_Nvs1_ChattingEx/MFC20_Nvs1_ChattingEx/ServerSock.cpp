#include "stdafx.h"
#include "ServerSock.h"
#include "MFC20_Nvs1_ChattingEx.h"

CServerSock::CServerSock()
{
}


CServerSock::~CServerSock()
{
}


void CServerSock::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	((CMFC20_Nvs1_ChattingExApp*)AfxGetApp())->Accept();

	CAsyncSocket::OnAccept(nErrorCode);
}
