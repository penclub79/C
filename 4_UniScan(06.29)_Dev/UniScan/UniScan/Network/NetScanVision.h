//
// Copyright (C) Since 2022 DAYOU. All rights reserved.
// 
//
#pragma once

//enum VERSION_PROTOCAL {
//	VERSION_1 = 0x00,
//	VERSION_2
//};
//
//const UINT32 SCAN_INFO_RECEIVE_BUFFER_SIZE = 64 * 1024; // 64 Kbytes temp buffer for receive temporary data
//const UINT32 SCAN_ERR_NONE = 0x00000000;
//const UINT32 SCAN_ERR_SOCKET_OPT = 0x00000001;
//const UINT32 SCAN_ERR_BIND = 0x00000002;
//const UINT32 SCAN_ERR_MEMORY = 0x00000003;
//const UINT32 SCAN_ERR_RECV = 0x00000004;

#include "NetScanBase.h"

//
// Copyright (C) Since 2022 DAYOU. All rights reserved.
// 
// Description: Network Define second version
//
#pragma once

const USHORT MAGIC2_CODE = 0x7668;

const UINT NET_BUF2_SIZE = 1024 * 1024;

enum VERSION_PROTOCAL
{
	VERSION_1 = 0x00,
	VERSION_2
};

#pragma pack(push, 1)

// protocol_types
const BYTE PROTOCOL_TYPE_EVENT_INFO = 0x03;
const BYTE PROTOCOL_TYPE_STREAM = 0x05;
const BYTE PROTOCOL_TYPE_EMERGENCY = 0x06;
const BYTE PROTOCOL_TYPE_ALIVE_PING = 0x08;

const DWORD PROTOCOL_MODE_EVENT_INFO = 0x01;
const DWORD PROTOCOL_MODE_EMERGENCY_EVENT = 0x01;
const DWORD PROTOCOL_MODE_ALIVE_REQUEST = 0x0F;
const DWORD PROTOCOL_MODE_ALIVE_RESPONSE = 0xF0;

const BYTE PROTOCOL_TYPE_IPUTILITY = 0x01;

const DWORD PROTOCOL_MODE_REQ_GET_IPINFO = 0x01;
const DWORD PROTOCOL_MODE_REQ_SET_IPINFO = 0x02;
const DWORD PROTOCOL_MODE_REQ_GET_IPINFO_EXT = 0x03; // IP util extension

const DWORD PROTOCOL_MODE_RSP_GET_IPINFO = 0x100;
const DWORD PROTOCOL_MODE_RSP_GET_IPINFO_EXT = 0x101;// IP util extension


// protocol_modes
// -- Requests
const DWORD _STREAM_REQ_AUTH = 0x00000001;
const DWORD _STREAM_REQ_GET_DEVICE_INFO = 0x00000002;
const DWORD _STREAM_REQ_LOGOFF = 0x00000003;
const DWORD _STREAM_REQ_START_ALL = 0x00000004;
const DWORD _STREAM_REQ_START_VIDEO = 0x00000005;
const DWORD _STREAM_REQ_START_AUDIO = 0x00000006;
const DWORD _STREAM_REQ_STOP_ALL = 0x00000007;
const DWORD _STREAM_REQ_STOP_VIDEO = 0x00000008;
const DWORD _STREAM_REQ_STOP_AUDIO = 0x00000009;

// -- Responses
const DWORD _STREAM_RSP_RESULT = 0x00000101;
const DWORD _STREAM_RSP_AUTH = 0x00000102;
const DWORD _STREAM_RSP_SERVERINFO = 0x00000103;
const DWORD _STREAM_RSP_DATA = 0x00000104;

typedef struct tagHEADER2 {
	WORD	magic; // 0x7668
	BYTE	protocol_type; // PROTOCOL_TYPE_STREAM 0x5
	DWORD	protocol_mode; // command
	DWORD	body_size;	   // body size
} HEADER2, FAR * PHEADER2;

const UINT HEADER2_SIZE = (UINT)sizeof(HEADER2);

typedef struct tagNETBUF2 {
	BYTE	byCmd;	// command?
	BYTE	byMode;	// 이녀석이 command 처럼 쓰인다
	BYTE	byType;	// header body, none,stream등을 구별하기 위해서 사용됨
	BYTE	byError;
	UINT	nRecvTotalSize;	// ReceiveTotal Size
	UINT	nPos;

	HEADER2 header;
	BYTE	abyBodyBuf[NET_BUF2_SIZE];

} NETBUF2, FAR *PNETBUF2;

// AUTH_DATA
const DWORD _ENCRYPT_MODE_MD5 = 0x0001;
const DWORD _ENCRYPT_MODE_SHA1 = 0x0002;
const DWORD _ENCRYPT_MODE_CRC32 = 0x0003;

typedef struct tagAUTH_DATA {
	char	id[32];
	char	password[32];
	WORD	enc_mode; // 0x01 = MD5, 0x02 = SHA1, 0x03 = CRC32
} AUTH_DATA, FAR * PAUTH_DATA;

// STREAM_RSP
const DWORD _RSP_AUTH_SUCCESS = 0x00000000;
const DWORD _RSP_AUTH_INVALID = 0x00000001;

typedef struct tagSTREAM_RSP {
	DWORD	message;
} STREAM_RSP, FAR* PSTREAM_RSP;

// SERVER_INFO
typedef struct tagSERVER_INFO
{
	//union DWORD mac
	//{
	//	DWORD macaddress:20;
	//};
	char	macaddress[20]; // fix me 
	char	serverid[32];
	DWORD	streamcount;
	DWORD	alarmincount;
	DWORD	alarmoutcount;
	BYTE	isaudio;
} SERVER_INFO, FAR *PSERVER_INFO;

// FSH structure
typedef struct tagFSHSTURCT
{
	DWORD startcode; // 0x000001ff; //?
	DWORD version : 24;
	DWORD channel_sequence : 8; // channel sequence(0~255)

	DWORD v_ch_id_left : 4;    // video channel ID
	DWORD v_ch_id_right : 4;   // video channel ID
	DWORD video_resolution : 4;// if video_resolution == 0, then extended_resolution 참조
	DWORD fsh_length : 20;	 // fsh header + video length + audio length
	DWORD time;
	DWORD prev_fsh_length : 20;
	DWORD field_type : 1; // 0:ODD, 1:EVEN
	DWORD codec_type : 2; // 0:JPEG, 1:H.264, 2:MPEG
	DWORD video_format : 1; // 0: NTSC, 1:PAL
	DWORD prev_skip_cnt : 8; // pre skip count;
	DWORD video_length : 20;  // video stream length;
	DWORD audio_length : 12;
	DWORD audio_link_1 : 16; // audio channel link info
	DWORD millisecond : 16;
	DWORD VOP_type : 2;	// 0: P-VOP, 1:I-VOP, 2:B-VOP
	DWORD fps : 5;		// frame rate
	DWORD ncd : 1;		// not coded;
	DWORD Blank_L : 1;	// blank
	DWORD Blank_R : 1;	// blank
	DWORD field_cnt : 6;	// field count
	DWORD cif_pos : 1;    // cif position 0:right, 1:left
	DWORD field_split : 1;// 0:split,1:NONE
	DWORD sched : 4;		// reserved;
	DWORD audio : 1;		// reserved
	DWORD net_skip : 1;	// reserved
	DWORD reserved1 : 8;	// reserved1
	DWORD extended_resolution : 32;	// video_resolution == 0 일때 사용됨 ( width 16 bit | height 16 bit )
	DWORD reserved3 : 32; // reserved3
} FSHSTURCT, FAR* PFSHSTRUCT;

typedef struct tagEmItem {
	ULONG		nSAddr;
	DWORD		nPort;
	DWORD		nEventType;
	DWORD		nAlarmInIndex;
	DWORD		nMotionIndex;
	DWORD		nDwellTime;
	time_t		tTime;

	tagEmItem()
	{
		nPort =
			nSAddr =
			nEventType =
			nAlarmInIndex =
			nMotionIndex =
			nDwellTime = 0L;

		tTime = 0i64;
	}

} EMITEM, FAR *PEMITEM;

const UINT EMITEM_SIZE = sizeof(EMITEM);


typedef struct tagEvItem {
	int   nEventType;
	int   nIndex;
} EVITEM, FAR* PEVITEM;

typedef struct tagPTZCMD {
	int   nCommand;
	int   nIndex;
	int   nSpeed;
	int   nPan;
	int   nTilt;
	int   nZoom;
	int   nBufferLen; // extra data length
	void* pBuffer;    // extra data
	tagPTZCMD()
	{
		nCommand = 0;
		nIndex = 0;
		nSpeed = 3;
		nPan = 0;
		nTilt = 0;
		nZoom = 0;
		nBufferLen = 0;
		pBuffer = NULL;
	}
	~tagPTZCMD()
	{
		nCommand = 0;
		nBufferLen = 0;
		free(pBuffer);
		pBuffer = NULL;
	}
} PTZCMDITEM, FAR* PPTZCMDITEM;

#pragma pack(pop)

const USHORT VH_BIND_PORT_EMERGENCY = 12000;
const USHORT VH_EVENT_INFO_PORT = 2300;

//------------------------------------------------------------------------------------------

typedef struct tagCAPTION_HEADER
{
	char aszCaption[32];
	int  nDataLen;
}CAPTION_HEADER, *LPCAPTION_HEADER;

#pragma pack(push, 1)
typedef struct tagIPUTIL_INFO
{
	char	szIPAddress[16];
	char	szGatewayIP[16];
	char	szMACAddress[20];
	DWORD   dwStreamPort;
	DWORD	dwHTTPPort;
}IPUTIL_INFO, *LPIPUTIL_INFO;

typedef struct tagIPUTIL_INFO2
{
	char	szIPAddress[16];
	char	szGatewayIP[16];
	char	szMACAddress[20];
	DWORD   dwStreamPort;
	DWORD	dwHTTPPort;
	char	cIsDHCP;              // 0 static, 1 DHCP, IPUTIL INFO version 2
	char	szSubnetmask[16];
}IPUTIL_INFO2, *LPIPUTIL_INFO2;

typedef struct tagIPUTIL_AUTH
{
	char    ID[32];			// User ID
	char	Password[32];	// Password
	SHORT   EncMode;		// Encription mode(default 0 : no encription)
}IPUTIL_AUTH, *LPIPUTIL_AUTH;

class CNetScanVision : public NetScanBase
{

public:
	CNetScanVision();
	~CNetScanVision(void);
	
	virtual BOOL StartScan();
	virtual BOOL SendScanRequest();

	//static BOOL RequestIPChange(WCHAR* strTargetServerMAC, WCHAR* strNewIP, WCHAR* strNewGateWay, int nStreamPort = 2700, int nHTTPPort = 80);
	//static BOOL RequestIPChange2(WCHAR* strTargetServerMAC, WCHAR* strNewIP, WCHAR* strNewGateWay, int nStreamPort = 2700, int nHTTPPort = 80, int cIsDHCP = 0, WCHAR* strNewsubnetMask = L"255.255.255.0", WCHAR* szID = L"", WCHAR* szPass = L"", int nEncMode = 0);

	//BOOL	SendScanRequest();
	//BOOL SendScanRequestExt(); // 확장 scan 정보 전송
protected:
	void thrReceiver();
	static DWORD thrScanThread(LPVOID pParam);

};
