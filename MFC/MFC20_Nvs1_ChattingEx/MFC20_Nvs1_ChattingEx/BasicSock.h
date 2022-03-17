#pragma once
#include "afxsock.h"

enum{
	PACKET_ID_REQ_WHOAREYOU = 1,
	PACKET_ID_RSP_WHOAREYOU,
};

typedef struct _tagPACKET_HEADER{
	int		iPacketID;
	TCHAR	wszPacketText[64];
}PACKET_HEADER;


class CBasicSock :
	public CAsyncSocket
{
public:
	CBasicSock();
	~CBasicSock();

	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);

};

