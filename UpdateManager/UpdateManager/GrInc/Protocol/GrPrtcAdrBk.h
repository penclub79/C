/*
	protocol address book

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrPrtcAdrBk
#define	_EPC_GrPrtcAdrBk

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

//====================================================================
//constance

#define	E_GrPrtcAdrBkFcc								Mac_GrMakeFcc('A','D','R','B')
#define	E_GrPrtcAdrBkPrtcVer						1

// command
#define	E_GrPrtcAdrBkCmdSvrInfo					1
#define	E_GrPrtcAdrBkCmdLogin						2
#define	E_GrPrtcAdrBkCmdResult					3
#define	E_GrPrtcAdrBkCmdAccountNew			4
#define	E_GrPrtcAdrBkCmdAccountDel			5
#define	E_GrPrtcAdrBkCmdBookUpload			6
#define	E_GrPrtcAdrBkCmdBookReqAll			7
#define	E_GrPrtcAdrBkCmdBookDownload		8
#define	E_GrPrtcAdrBkCmdBookAdd					9
#define	E_GrPrtcAdrBkCmdBookDelItem			10

// result code
#define	E_GrPrtcAdrBkCodeOk							0
#define	E_GrPrtcAdrBkBadId							1
#define	E_GrPrtcAdrBkBadPassword				2
#define	E_GrPrtcAdrBkNeedLogin					3
#define	E_GrPrtcAdrBkIdExist						4
#define	E_GrPrtcAdrBkIdCreateFail				5
#define	E_GrPrtcAdrBkIdDelFail					6
#define	E_GrPrtcAdrBkUploadFail					7
#define	E_GrPrtcAdrBkAddFail						8
#define	E_GrPrtcAdrBkDelFail						9
#define	E_GrPrtcAdrBkLoginAlready				10
#define	E_GrPrtcAdrBkDownloadFail				11


//====================================================================
//global macro

//====================================================================
//global type

#pragma	pack(1)

typedef struct St_GrPrtcAdrBkGroupInfo
{
	__u16	GrpId;
	__u16	_rsvd0;
	__u32	UserData;
	WCHAR	StrName[32];
}	*Ptr_GrPrtcAdrBkGroupInfo;

typedef struct St_GrPrtcAdrBkDevInfo
{
	char	StrId[64];
	char	StrPw[32];
	__u16	Port[4];
	__u16	GrpId;
	__u16	DevType;
	__u32	_rsvd0[5];
	char	StrUrl[128];
	WCHAR	StrName[32];
	__u32	_rsvd1[16];
}	*Ptr_GrPrtcAdrBkDevInfo;

typedef struct St_GrPrtcAdrBkBaseHd
{
	__u32	Fcc;
	__u32	Size;
	__u32	Cmd;
}	*Ptr_GrPrtcAdrBkBaseHd;

typedef struct St_GrPrtcAdrBkSvrInfo
{
	St_GrPrtcAdrBkBaseHd	BaseHd;

	__u32	Ver;
}	*Ptr_GrPrtcAdrBkSvrInfo;

typedef struct St_GrPrtcAdrBkLogin
{
	St_GrPrtcAdrBkBaseHd	BaseHd;

	WCHAR	StrId[32];
	char	StrPw[16];
}	*Ptr_GrPrtcAdrBkLogin;

typedef struct St_GrPrtcAdrBkResult
{
	St_GrPrtcAdrBkBaseHd	BaseHd;
	__u32	Code;
	__u32	ReqCmd;
}	*Ptr_GrPrtcAdrBkResult;

typedef struct St_GrPrtcAdrAccountNew
{
	St_GrPrtcAdrBkBaseHd	BaseHd;

	WCHAR	StrId[32];
	char	StrPw[16];
}	*Ptr_GrPrtcAdrAccountNew;

typedef struct St_GrPrtcAdrAccountDel
{
	St_GrPrtcAdrBkBaseHd	BaseHd;

	WCHAR	StrId[32];
	char	StrPw[16];
}	*Ptr_GrPrtcAdrAccountDel;

typedef struct St_GrPrtcAdrBkBookUpload
{
	St_GrPrtcAdrBkBaseHd	BaseHd;

	__u32	GroupCnt;
	__u32	DevCnt;
}	*Ptr_GrPrtcAdrBkBookUpload;

typedef struct St_GrPrtcAdrBkBookDownload
{
	St_GrPrtcAdrBkBaseHd	BaseHd;

	__u32	GroupCnt;
	__u32	DevCnt;
}	*Ptr_GrPrtcAdrBkBookDownload;

typedef struct St_GrPrtcAdrBkBookAdd
{
	St_GrPrtcAdrBkBaseHd	BaseHd;

	__u32	GroupCnt;
	__u32	DevCnt;
}	*Ptr_GrPrtcAdrBkBookAdd;

typedef struct St_GrPrtcAdrBkBookDelItem
{
	St_GrPrtcAdrBkBaseHd	BaseHd;
	__u32	DevCnt;
}	*Ptr_GrPrtcAdrBkBookDelItem;


#pragma	pack()

//====================================================================
//class

	
//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

