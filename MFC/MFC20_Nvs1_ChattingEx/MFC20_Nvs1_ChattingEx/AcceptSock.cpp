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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CAsyncSocket::OnAccept(nErrorCode);
}
