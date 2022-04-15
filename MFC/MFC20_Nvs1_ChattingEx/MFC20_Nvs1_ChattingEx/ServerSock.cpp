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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	((CMFC20_Nvs1_ChattingExApp*)AfxGetApp())->Accept();

	CAsyncSocket::OnAccept(nErrorCode);
}
