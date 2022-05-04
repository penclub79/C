/*
	protocol DDNS V3

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrPrtcDdnsV3
#define	_EPC_GrPrtcDdnsV3

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

//====================================================================
//constance

#define	E_GrPrtcDdnsV3Fcc								Mac_GrMakeFcc('D','D','V','3')		// 0x44, 0x44, 0x56, 0x33

#define	E_GrPrtcDdnsV3DbPayloadSize			64

#define	E_GrPrtcDdnsV3StatEmergency			0x00000001
#define	E_GrPrtcDdnsV3StatDiskFull			0x00000002
#define	E_GrPrtcDdnsV3StatOverwrite			0x00000004

#define	E_GrPrtcDdnsV3CodeSuccess				0
#define	E_GrPrtcDdnsV3CodeFail					1
#define	E_GrPrtcDdnsV3CodeNotAuth				2

#define	E_GrPrtcDdnsV3ListMaxCnt				128

#define	E_GrPrtcDdnsV3ItemSize					64

#define	E_GrPrtcDdnsV3CmdRegistReq			1
#define	E_GrPrtcDdnsV3CmdRegistAns			2

#define	E_GrPrtcDdnsV3CmdDevAdrReq			3
#define	E_GrPrtcDdnsV3CmdDevAdrAns			4

#define	E_GrPrtcDdnsV3CmdDevDataReq			5
#define	E_GrPrtcDdnsV3CmdDevDataAns			6

//====================================================================
//global macro

//====================================================================
//global type

#pragma	pack(1)

typedef struct St_GrPrtcDdnsV3DbCont
{
	__u32	ModelNum;
	__u32	FirmVer;
	__u32	IpV4;
	__u32	StatFlag;

	__u16	PortTbl[4];		// port number table
	__u32	TimeRecSt;
	__u32	TimeRecLast;

	__u32	HddTotCap;
	__u32	HddTotUse;
	__u8	HddSetMap;
	__u8	HddErrMap;
	__u8	HddTmpMap;
	__u8	_rsvd0;
	__u32	CamNsgMap;

	__u32	UpdtTime;		// server only
	__u32	BootTime;
	__u32	_rsvd1[2];

}	*Ptr_GrPrtcDdnsV3DbCont;

typedef struct St_GrPrtcDdnsV3BaseHd
{
	__u32	Fcc;
	__u32	Cmd;
	__u32	PktSize;		// patcket total size
}	*Ptr_GrPrtcDdnsV3BaseHd;

typedef struct St_GrPrtcDdnsV3RegistReq
{
	St_GrPrtcDdnsV3BaseHd	BaseHd;
	__u32	_rsvd0;
	__u8	MacAdr[6];
	__u8	_rsvd1[2];
	__u32	_rsvd2[2];
	St_GrPrtcDdnsV3DbCont	Data;
}	*Ptr_GrPrtcDdnsV3RegistReq;

typedef struct St_GrPrtcDdnsV3RegistAns
{
	St_GrPrtcDdnsV3BaseHd	BaseHd;
	__u32	AnsCode;

}	*Ptr_GrPrtcDdnsV3RegistAns;

typedef struct St_GrPrtcDdnsV3DevAdrReq
{
	St_GrPrtcDdnsV3BaseHd	BaseHd;
	__u32	_rsvd0;
	__u8	MacAdr[6];
	__u8	_rsvd1[2];
}	*Ptr_GrPrtcDdnsV3DevAdrReq;

typedef struct St_GrPrtcDdnsV3DevAdrAns
{
	St_GrPrtcDdnsV3BaseHd	BaseHd;
	__u32	AnsCode;
	__u32	Ip;
	__u16	Port[4];
	__u32	_rsvd0;
}	*Ptr_GrPrtcDdnsV3DevAdrAns;

typedef struct St_GrPrtcDdnsV3DevDataReq
{
	St_GrPrtcDdnsV3BaseHd	BaseHd;
	__u32	ReqCnt;
	WCHAR	StrId[32];
	char	StrPass[16];
}	*Ptr_GrPrtcDdnsV3DevDataReq;

typedef struct St_GrPrtcDdnsV3DevDataItem
{
	__u8	MacAdr[8];
	__u32	TimeUpdt;

	BOOL8	IsExist;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	St_GrPrtcDdnsV3DbCont	Data;
}	*Ptr_GrPrtcDdnsV3DevDataItem;

typedef struct St_GrPrtcDdnsV3DevDataAns
{
	St_GrPrtcDdnsV3BaseHd	BaseHd;
	__u32	AnsCode;
	__u32	ItemCnt;
	__u32	_rsvd0[3];

}	*Ptr_GrPrtcDdnsV3DevDataAns;


#pragma	pack()

//====================================================================
//class

	
//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

