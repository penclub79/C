#include "stdafx.h"
#include "BasicSock.h"
#include "MFC20_Nvs1_ChattingEx.h"


CBasicSock::CBasicSock()
{

}


CBasicSock::~CBasicSock()
{
}





void CBasicSock::OnReceive(int nErrorCode)
{
	
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	((CMFC20_Nvs1_ChattingExApp*)AfxGetApp())->ReceiveData((CAcceptSock*)this);

	CAsyncSocket::OnReceive(nErrorCode);
}


void CBasicSock::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	((CMFC20_Nvs1_ChattingExApp*)AfxGetApp())->CloseChild((CAcceptSock*)this);

	CAsyncSocket::OnClose(nErrorCode);
}
