#include "stdafx.h"
#include "BasicSock.h"
#include "MFC19_SocketEx.h"

CBasicSock::CBasicSock()
:m_bIsConnect(0)
{
}


CBasicSock::~CBasicSock()
{
}

void CBasicSock::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	((CMFC19_SocketExApp*)AfxGetApp())->ReceiveData();

	CAsyncSocket::OnReceive(nErrorCode);
}


void CBasicSock::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	((CMFC19_SocketExApp*)AfxGetApp())->CloseChild();

	CAsyncSocket::OnClose(nErrorCode);
}

void CBasicSock::OnConnect(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CString strConnectMessage;

	if (nErrorCode == 0)
	{
		strConnectMessage.Format(_T("연결 성공"));
		AfxMessageBox(strConnectMessage);
		m_bIsConnect = 1;
		((CMFC19_SocketExApp*)AfxGetApp())->Connect(m_bIsConnect);
	}
	else
	{
		strConnectMessage.Format(_T("Error code : %d\n 연결이 거부되었습니다."), nErrorCode);
		AfxMessageBox(strConnectMessage);
		m_bIsConnect = 0;
		((CMFC19_SocketExApp*)AfxGetApp())->Connect(m_bIsConnect);
	}
	
	CAsyncSocket::OnConnect(nErrorCode);
}
