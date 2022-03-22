// if not define 조건문
#ifndef _BASIC_DEFINE
// #은 컴파일되기 전에 처리하는 선행처리 과정을 거친다.
#define _BASIC_DEFINE

typedef enum{
	PACKET_ID_REQ_WHOAREYOU = 1,
	PACKET_ID_RSP_WHOAREYOU,
};

typedef struct _tagPACKET_HEADER{
	int		iPacketID;
	TCHAR	wszPacketText[64];
}PACKET_HEADER;

#endif