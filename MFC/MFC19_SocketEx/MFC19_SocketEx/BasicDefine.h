// if not define 조건문
#ifndef _BASIC_DEFINE
// #은 컴파일되기 전에 처리하는 선행처리 과정을 거친다.
#define _BASIC_DEFINE

#define	VERSION_PACKET_CLIENT_1	1
#define MAX_LENGTH_USERID		16
#define	MARKER_CLIENT			0xffff1234

typedef enum{
	RSP_SUCCESS
};

typedef enum{
	PACKET_ID_REQ_LOGIN	 = 1,
	PACKET_ID_RSP_LOGIN		,	
	PACKET_ID_REQ_TEXT		,
	PACKET_ID_RSP_TEXT		,
};

typedef struct _tagPACKET_HEADER{
	/*
	추후 유지보수를 위해서 Marker와 Version을 헤더에서 관리한다.

	Marker : 헤더를 받았을 때, 

	Version : 예를들어 로그인 할때 비밀번호까지 관리하고 싶다면 비밀번호 담을 변수를 추가한다.
			  패킷의 업데이트나 수정/추가로 인해 Version 업이된다면, 해당 버전의 하위호환으로 사용
			  할 수도 있으니 Version 관리를 하면 유지보수하기 수월하다.
	*/
	unsigned int		iMarker;
	unsigned int		iVersion;
	unsigned int		iPacketID;
	unsigned int		iPacketSize;
}PACKET_HEADER;

typedef struct _tagPACKET_REQ_LOGIN{
	PACKET_HEADER	stHeader;
	TCHAR			wszUserID[MAX_LENGTH_USERID];
}PACKET_REQ_LOGIN;

typedef struct _tagPACKET_RSP_LOGIN{
	PACKET_HEADER	stHeader;
	int				iResultCode;
}PACKET_RSP_LOGIN;

typedef struct _tagPACKET_REQ_TEXT{
	PACKET_HEADER	stHeader;
	TCHAR			wszPacketText[64];
}PACKET_REQ_TEXT;

typedef struct _tagPACKET_RSP_TEXT{
	PACKET_HEADER	stHeader;
	int				iResultCode;
	TCHAR			wszPacketText[64];
	TCHAR			wszSendUserID[MAX_LENGTH_USERID];
}PACKET_RSP_TEXT;

// 클라이언트 구조체
typedef struct _tagCLIENT_INFO
{
	int iPort;
	PCSTR szServerIP;
	// 구조체에 클래스는 추천 하지 않음.
	TCHAR szUserId[MAX_LENGTH_USERID];
}CLIENT_INFO;
//
//typedef struct _tagERRORCODE
//{
//	int iErrorCode;
//}ERRORCODE;


#endif