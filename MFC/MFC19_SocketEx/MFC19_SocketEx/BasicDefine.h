// if not define 조건문
#ifndef _BASIC_DEFINE
// #은 컴파일되기 전에 처리하는 선행처리 과정을 거친다.
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


// 클라이언트 구조체
typedef struct _tagCLIENT_INFO
{
	int iPort;
	// 구조체에 클래스는 별로 추천하지않음
	TCHAR szUserId[MAX_LENGTH_USERID];
}CLIENT_INFO;

typedef struct _tagERRORCODE
{
	int iErrorCode;
}ERRORCODE;


#endif