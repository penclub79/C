// if not define ���ǹ�
#ifndef _BASIC_DEFINE
// #�� �����ϵǱ� ���� ó���ϴ� ����ó�� ������ ��ģ��.
#define _BASIC_DEFINE



#define MAX_LENGTH_USERID	 16


typedef enum{
	PACKET_ID_REQ_WHOAREYOU = 1,
	PACKET_ID_RSP_WHOAREYOU,
};

typedef struct _tagPACKET_HEADER{
	int		iPacketID;
	TCHAR	wszPacketText[64];
}PACKET_HEADER;


// Ŭ���̾�Ʈ ����ü
typedef struct _tagCLIENT_INFO
{
	int iPort;
	// ����ü�� Ŭ������ ���� ��õ��������
	TCHAR szUserId[MAX_LENGTH_USERID];
}CLIENT_INFO;

typedef struct _tagERRORCODE
{
	int iErrorCode;
}ERRORCODE;


#endif