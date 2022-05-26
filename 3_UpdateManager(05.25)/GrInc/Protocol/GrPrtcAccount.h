/*
	protocol DDNS V3

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrPrtcAccount
#define	_EPC_GrPrtcAccount

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

//====================================================================
//constance

#define	E_GrPrtcAccountFcc								Mac_GrMakeFcc('A','C','V','1')

#define	E_GrPrtcAccountAuthSize						16

#define	E_GrPrtcAccountAuth0Health				0x01	/* health service allow */

// result code
#define	E_GrPrtcAccountRsltOk							0
#define	E_GrPrtcAccountRsltFail						1
#define	E_GrPrtcAccountRsltKeyMismatch		2
#define	E_GrPrtcAccountRsltBadId					3
#define	E_GrPrtcAccountRsltBadPasswd			4
#define	E_GrPrtcAccountRsltBadMacAdr			5
#define	E_GrPrtcAccountRsltIdAlreadyExist	6
#define	E_GrPrtcAccountRsltItemNotFound		7
#define	E_GrPrtcAccountRsltBadNewPasswd		8
#define	E_GrPrtcAccountRsltMacExist				9

// command user
#define	E_GrPrtcAccountCmdRegistReq				1
#define	E_GrPrtcAccountCmdRegistRslt			2
#define	E_GrPrtcAccountCmdDelReq					3
#define	E_GrPrtcAccountCmdDelRslt					4
#define	E_GrPrtcAccountCmdPassModReq			5
#define	E_GrPrtcAccountCmdPassModRslt			6
#define	E_GrPrtcAccountCmdPassChkReq			7
#define	E_GrPrtcAccountCmdPassChkRslt			8
#define	E_GrPrtcAccountCmdPassRstReq			9
#define	E_GrPrtcAccountCmdPassRstRslt			10


// command manager
#define	E_GrPrtcAccountCmdMngPassChkReq		100
#define	E_GrPrtcAccountCmdMngPassChkRslt	101
#define	E_GrPrtcAccountCmdMngAuthGetReq		102
#define	E_GrPrtcAccountCmdMngAuthGetRslt	103
#define	E_GrPrtcAccountCmdMngAuthModReq		104
#define	E_GrPrtcAccountCmdMngAuthModRslt	105
#define	E_GrPrtcAccountCmdMngUsrListReq		106
#define	E_GrPrtcAccountCmdMngUsrListRslt	107
#define	E_GrPrtcAccountCmdMngDelOneReq		108
#define	E_GrPrtcAccountCmdMngDelOneRslt		109

// command server
#define	E_GrPrtcAccountCmdSvrDataReq			200
#define	E_GrPrtcAccountCmdSvrDataRslt			201

//====================================================================
//global macro

//====================================================================
//global type

#pragma	pack(1)

typedef struct St_GrPrtcAccountHead
{
	__u32	Fcc;
	__u32	Cmd;
	__u32	PktSize;
}	*Ptr_GrPrtcAccountHead;

typedef struct St_GrPrtcAccountUserRslt
{
	St_GrPrtcAccountHead	Head;
	__u32	RsltCode;
	WCHAR	StrId[32];
}	*Ptr_GrPrtcAccountUserRslt;

typedef struct St_GrPrtcAccountRegistReq
{
	St_GrPrtcAccountHead	Head;
	__u32	_rsvd0;
	WCHAR	StrId[32];
	char	StrPasswd[16];
	__u8	MacAdr[8];
}	*Ptr_GrPrtcAccountRegistReq;

typedef struct St_GrPrtcAccountDelReq
{
	St_GrPrtcAccountHead	Head;
	__u32	_rsvd0;
	WCHAR	StrId[32];
	char	StrPasswd[16];
}	*Ptr_GrPrtcAccountDelReq;

typedef struct St_GrPrtcAccountPassModReq
{
	St_GrPrtcAccountHead	Head;
	__u32	_rsvd0;
	WCHAR	StrId[32];
	char	StrPasswdOrg[16];
	char	StrPasswdNew[16];	
}	*Ptr_GrPrtcAccountPassModReq;

typedef struct St_GrPrtcAccountPassChkReq
{
	St_GrPrtcAccountHead	Head;
	__u32	_rsvd0;
	WCHAR	StrId[32];
	char	StrPasswd[16];
}	*Ptr_GrPrtcAccountPassChkReq;

typedef struct St_GrPrtcAccountPassRstReq
{
	St_GrPrtcAccountHead	Head;
	__u32	_rsvd0;
	WCHAR	StrId[32];
	__u8	MacAdr[8];
	char	StrNewPasswd[16];
}	*Ptr_GrPrtcAccountPassRstReq;

typedef struct St_GrPrtcAccountMngPassChkReq
{
	St_GrPrtcAccountHead	Head;
	__u32	_rsvd0;
	char	StrPasswd[16];
}	*Ptr_GrPrtcAccountMngPassChkReq;

typedef struct St_GrPrtcAccountMngPassChkRslt
{
	St_GrPrtcAccountHead	Head;
	__u32	RsltCode;
}	*Ptr_GrPrtcAccountMngPassChkRslt;

typedef struct St_GrPrtcAccountMngAuthGetReq
{
	St_GrPrtcAccountHead	Head;
	__u32	_rsvd0;
	char	StrPasswd[16];
	WCHAR	StrId[32];
}	*Ptr_GrPrtcAccountMngAuthGetReq;

typedef struct St_GrPrtcAccountMngAuthGetRslt
{
	St_GrPrtcAccountHead	Head;
	__u32	RsltCode;
	WCHAR	StrId[32];
	__u8	Auth[E_GrPrtcAccountAuthSize];
}	*Ptr_GrPrtcAccountMngAuthGetRslt;

typedef struct St_GrPrtcAccountMngAuthModReq
{
	St_GrPrtcAccountHead	Head;
	__u32	_rsvd0;
	char	StrPasswd[16];
	WCHAR	StrId[32];
	__u8	Auth[E_GrPrtcAccountAuthSize];
}	*Ptr_GrPrtcAccountMngAuthModReq;

typedef struct St_GrPrtcAccountMngAuthModRslt
{
	St_GrPrtcAccountHead	Head;
	__u32	RsltCode;
	WCHAR	StrId[32];
}	*Ptr_GrPrtcAccountMngAuthModRslt;

typedef struct St_GrPrtcAccountMngUsrListReq
{
	St_GrPrtcAccountHead	Head;
	__u32	IdxSt;
	char	StrPasswd[16];
	__u32	MaxCnt;
}	*Ptr_GrPrtcAccountMngUsrListReq;

typedef struct St_GrPrtcAccountMngUsrListItem
{
	WCHAR	StrId[32];
	__u32	TimeUpdt;
	__u32	_rsvd0;
	__u8	MacAdr[8];
	__u8	Auth[16];
}	*Ptr_GrPrtcAccountMngUsrListItem;

typedef struct St_GrPrtcAccountMngUsrListRslt
{
	St_GrPrtcAccountHead	Head;
	__u32	RsltCode;
	__u32	IdxSt;
	__u32	ItemCnt;
	BOOL8	IsLast;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	ItemTotal;
}	*Ptr_GrPrtcAccountMngUsrListRslt;

typedef struct St_GrPrtcAccountMngDelOneReq
{
	St_GrPrtcAccountHead	Head;
	__u32	_rsvd0;
	char	StrPasswd[16];
	WCHAR	StrId[32];	
}	*Ptr_GrPrtcAccountMngDelOneReq;

typedef struct St_GrPrtcAccountMngDelOneRslt
{
	St_GrPrtcAccountHead	Head;
	__u32	RsltCode;
	WCHAR	StrId[32];
}	*Ptr_GrPrtcAccountMngDelOneRslt;

typedef struct St_GrPrtcAccountSvrDataReqItem
{
	WCHAR	StrId[32];
}	*Ptr_GrPrtcAccountSvrDataReqItem;

typedef struct St_GrPrtcAccountSvrDataReq
{
	St_GrPrtcAccountHead	Head;
	__u32	ReqCnt;
	char	StrPasswd[16];
}	*Ptr_GrPrtcAccountSvrDataReq;

typedef struct St_GrPrtcAccountSvrDataRsltItem
{
	WCHAR	StrId[32];
	char	StrPasswd[16];
	__u8	Auth[16];
}	*Ptr_GrPrtcAccountSvrDataRsltItem;

typedef struct St_GrPrtcAccountSvrDataRslt
{
	St_GrPrtcAccountHead	Head;
	__u32	ItemCnt;
}	*Ptr_GrPrtcAccountSvrDataRslt;

#pragma	pack()

//====================================================================
//class

	
//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

