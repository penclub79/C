#include "stdafx.h"
#include "ClientSock.h"
#include "MFC19_SocketEx.h"

CBasicSock::CBasicSock()
{
}


CBasicSock::~CBasicSock()
{
}


void CBasicSock::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	((CMFC19_SocketExApp*)AfxGetApp())->ReceiveData();

	CAsyncSocket::OnReceive(nErrorCode);
}


void CBasicSock::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	((CMFC19_SocketExApp*)AfxGetApp())->CloseChild();

	CAsyncSocket::OnClose(nErrorCode);
}
