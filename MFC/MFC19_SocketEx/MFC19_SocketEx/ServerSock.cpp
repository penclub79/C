#include "stdafx.h"
#include "ServerSock.h"
#include "MFC19_SocketEx.h"

CServerSock::CServerSock()
{
}


CServerSock::~CServerSock()
{
}


void CServerSock::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	((CMFC19_SocketExApp*)AfxGetApp())->Accept();

	CAsyncSocket::OnAccept(nErrorCode);
}
