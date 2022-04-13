#include "stdafx.h"
#include "BasicSock.h"
#include "MFC19_SocketEx.h"

CBasicSock::CBasicSock()
{
}


CBasicSock::~CBasicSock()
{
}

void CBasicSock::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	((CMFC19_SocketExApp*)AfxGetApp())->ReceiveData();

	CBasicSock::OnReceive(nErrorCode);
}


void CBasicSock::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	((CMFC19_SocketExApp*)AfxGetApp())->CloseChild();

	CBasicSock::OnClose(nErrorCode);
}

void CBasicSock::OnConnect(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	((CMFC19_SocketExApp*)AfxGetApp())->SetConnectStatus(nErrorCode);

	CBasicSock::OnConnect(nErrorCode);
}
