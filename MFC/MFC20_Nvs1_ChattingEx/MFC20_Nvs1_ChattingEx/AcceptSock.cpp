#include "stdafx.h"
#include "AcceptSock.h"


CAcceptSock::CAcceptSock()
{
}


CAcceptSock::~CAcceptSock()
{
}


void CAcceptSock::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CAsyncSocket::OnAccept(nErrorCode);
}
