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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	((CMFC19_SocketExApp*)AfxGetApp())->ReceiveData();

	CAsyncSocket::OnReceive(nErrorCode);
}


void CClientSock::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	((CMFC19_SocketExApp*)AfxGetApp())->CloseChild();

	CAsyncSocket::OnClose(nErrorCode);
}

void CClientSock::OnConnect(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CString str;

	if (nErrorCode == 0)
	{
		str.Format(_T("연결 성공"));
		AfxMessageBox(str);
	}
	else
	{
		str.Format(_T("Error code : %d\n 연결이 거부되었습니다."), nErrorCode);
		AfxMessageBox(str);
	}
	
	CAsyncSocket::OnConnect(nErrorCode);
}
