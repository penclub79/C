#include "stdafx.h"
#include "ClientSock.h"
#include "MFC19_SocketEx.h"

CClientSock::CClientSock()
{
}


CClientSock::~CClientSock()
{
}


void CClientSock::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	((CMFC19_SocketExApp*)AfxGetApp())->ReceiveData();

	CAsyncSocket::OnReceive(nErrorCode);
}


void CClientSock::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	((CMFC19_SocketExApp*)AfxGetApp())->CloseChild();

	CAsyncSocket::OnClose(nErrorCode);
}
