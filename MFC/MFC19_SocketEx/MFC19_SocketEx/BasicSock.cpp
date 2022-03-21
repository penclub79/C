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

void CBasicSock::OnConnect(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CString strConnectMessage;

	if (nErrorCode == 0)
	{
		strConnectMessage.Format(_T("���� ����"));
		AfxMessageBox(strConnectMessage);
		m_bIsConnect = 1;
		((CMFC19_SocketExApp*)AfxGetApp())->Connect(m_bIsConnect);
	}
	else
	{
		strConnectMessage.Format(_T("Error code : %d\n ������ �źεǾ����ϴ�."), nErrorCode);
		AfxMessageBox(strConnectMessage);
		m_bIsConnect = 0;
		((CMFC19_SocketExApp*)AfxGetApp())->Connect(m_bIsConnect);
	}
	
	CAsyncSocket::OnConnect(nErrorCode);
}
