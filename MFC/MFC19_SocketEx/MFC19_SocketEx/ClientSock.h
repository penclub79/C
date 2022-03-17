#pragma once
#include "afxsock.h"
#include "ClientSock.h"

enum{
	PACKET_ID_REQ_WHOAREYOU = 1,
	PACKET_ID_RSP_WHOAREYOU,
};

typedef struct _tagPACKET_HEADER{
	int		iPacketID;
	TCHAR	wszPacketText[64];
}PACKET_HEADER;

class CClientSock :
	public CAsyncSocket
{
public:
	CClientSock();
	~CClientSock();


	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

