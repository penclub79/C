#ifndef _BASIC_DEFINE
#define _BASIC_DEFINE

#define	VERSION_PACKET_CLIENT_1	1
#define MAX_LENGTH_USERID		16
#define	MARKER_CLIENT			0xffff1234

// 패킷 번호
typedef enum{
	PACKET_ID_REQ_LOGIN = 1,
	PACKET_ID_RSP_LOGIN,
	PACKET_ID_REQ_TEXT,
	PACKET_ID_RSP_TEXT,
	PACKET_ID_REQ_ALIVE,
	PACKET_ID_RSP_ALIVE,
};

// Success Code
typedef enum
{
	LOGIN_SUCCESS	= 0,
	TEXT_SUCCESS	= 10,
	ALIVE_SUCCESS, 
	DISCONNECTED	= 100,
	CONNECTED_ERROR,
	READ_ERROR,
	WRITE_ERROR,
	CLOSED_ERROR,
	SERVER_CLOSE,
};


typedef struct _tagPACKET_HEADER
{
	int					iMarker;
	unsigned int		iVersion;
	unsigned int		iPacketID;
	unsigned int		iPacketSize;
}PACKET_HEADER;

typedef struct _tagPACKET_REQ_LOGIN
{
	PACKET_HEADER	stHeader;
	TCHAR			wszUserID[MAX_LENGTH_USERID];
}PACKET_REQ_LOGIN;

typedef struct _tagPACKET_RSP_LOGIN
{
	PACKET_HEADER	stHeader;
	int				iResultCode;
}PACKET_RSP_LOGIN;

typedef struct _tagPACKET_REQ_TEXT
{
	PACKET_HEADER	stHeader;
	TCHAR			wszPacketText[64];
}PACKET_REQ_TEXT;

typedef struct _tagPACKET_RSP_TEXT
{
	PACKET_HEADER	stHeader;
	int				iResultCode;
	TCHAR			wszPacketText[64];
	TCHAR			wszSendUserID[MAX_LENGTH_USERID];
}PACKET_RSP_TEXT;

typedef struct _tagPACKET_REQ_KEEPALIVE
{
	PACKET_HEADER	stHeader;
}PACKET_REQ_KEEPALIVE;

typedef struct _tagPACKET_RSP_KEEPALIVE
{
	PACKET_HEADER	stHeader;
}PACKET_RSP_KEEPALIVE;

typedef struct _tagERRORCODE
{
	int iErrorCode;
}ERRORCODE;
#endif