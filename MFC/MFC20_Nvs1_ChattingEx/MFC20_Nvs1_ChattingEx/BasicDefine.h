#ifndef _BASIC_DEFINE
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