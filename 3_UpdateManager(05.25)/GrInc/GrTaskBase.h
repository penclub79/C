/*
	Task basic type
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrTaskBase
#define	_EPC_GrTaskBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrProcess.h>
#include <GrError.h>

//====================================================================
//constance

// base
#define E_GrTaskFccNone							0
#define E_GrTaskFccAny							1

#define E_GrTaskMsgCmdTskBase				2					/* task server static message start */

#define	E_GrTaskMsgCmdSysBase				100

#define E_GrTaskMsgCmdUiBase				200				/* UI message start */

#define	E_GrTaskMsgCmdDvrBase				300

#define E_GrTaskMsgCmdAlloc					0x8000		/* client allocate message start */

// server
#define E_GrTaskSvrMaxRegCnt				32		/* max thread regist count */

#define E_GrTaskSvrTmHndlPdPos			16		/* priod positino */

#define E_GrTaskSvrPrioTermStep			10		/* priority term tick step */
#define E_GrTaskSvrPrioQuatStep			40		/* priority quater tick step */
#define E_GrTaskSvrPrioQuatZero			20		/* priority quater zero time */

#define E_GrTaskSvrMsgWaitStep			10		/* message process wait */

#define E_GrTaskSvrMsgFcodeNone			0			/* wait */
#define E_GrTaskSvrMsgFcodeOk				1			/* process success */
#define E_GrTaskSvrMsgFcodeNoCli		2			/* not exist client */

#define E_GrTaskSvrCliTypeMsg				1			/* client type message  */
#define E_GrTaskSvrCliTypeBkg				2			/* client type background  */
#define E_GrTaskSvrCliTypeTimer			3			/* client type timer  */

#define E_GrTaskSvrMaxMsgCliCmdCnt	1024	/* client message count */
#define E_GrTaskSvrMsgCliCmdTblSize	(E_GrTaskSvrMaxMsgCliCmdCnt/8)	/* allocate command table size */	


/// server static message
#define E_GrTaskSvrMsgRegCli				(0 + E_GrTaskMsgCmdTskBase)		/* register client */
#define E_GrTaskSvrMsgUnregCli			(1 + E_GrTaskMsgCmdTskBase)		/* unregister client */

// system message
#define E_GrTaskMsgSysErr						(0	+ E_GrTaskMsgCmdSysBase)		/* system error : St_GrTaskMsgSysErr */
#define E_GrTaskMsgTimeUpdt					(1	+ E_GrTaskMsgCmdSysBase)		/* time update : St_GrTaskMsgTimeUpdt */
#define E_GrTaskMsgIpUpdt						(2	+ E_GrTaskMsgCmdSysBase)		/* network ip set : St_GrTaskMsgNetIf */
#define E_GrTaskMsgDiskFormatCpl		(3	+ E_GrTaskMsgCmdSysBase)		/* disk format completed : St_GrTaskMsgDiskFmtCpl */
#define E_GrTaskMsgDiskFormatPrgs		(4 + E_GrTaskMsgCmdSysBase)			/* disk format progress : St_GrTaskMsgDiskFmtPrgs */
#define	E_GrTaskMsgSysInitCpl				(5	+ E_GrTaskMsgCmdSysBase)		/* system initialized : St_GrTaskMsgHd */
#define	E_GrTaskMsgGroupFsFix				(6	+ E_GrTaskMsgCmdSysBase)		/* group disk file system fix : St_GrTaskMsgGrpDskFsFix */
#define	E_GrTaskMsgHddIoError				(7	+ E_GrTaskMsgCmdSysBase)		/* disk read write error : St_GrTaskMsgDskIoErr */
#define	E_GrTaskMsgNtpRslt					(8	+ E_GrTaskMsgCmdSysBase)		/* NTP result : St_GrTaskMsgNtpRslt */
#define	E_GrTaskMsgDiskFsUpgd				(9	+ E_GrTaskMsgCmdSysBase)		/* disk file system upgrade : St_GrTaskMsgGrpDskFsFix */

// UI message
#define E_GrTaskMsgUiMsg						(0	+ E_GrTaskMsgCmdUiBase)
#define E_GrTaskMsgUiEvt						(1	+ E_GrTaskMsgCmdUiBase)		/* UI event message - not use? */

// DVR message
#define E_GrTaskMsgQuadChgDone			(6	+ E_GrTaskMsgCmdDvrBase)		/* quad mode change done : St_GrTaskMsgQuadChgDone */
#define E_GrTaskMsgBkupStart				(7	+ E_GrTaskMsgCmdDvrBase)		/* backup start : St_GrTaskMsgHd */
#define E_GrTaskMsgBkupEnd					(8	+ E_GrTaskMsgCmdDvrBase)		/* backup end : St_GrTaskMsgBkupEnd */
#define E_GrTaskMsgBkupPrgs					(9	+ E_GrTaskMsgCmdDvrBase)		/* backup progress : St_GrTaskMsgBkupPrgs */
//#define E_GrTaskMsgPlayStatChg			(10	+ E_GrTaskMsgCmdDvrBase)		/* play status change : St_GrTaskMsgHd */
#define E_GrTaskMsgCmsCliChg				(11	+ E_GrTaskMsgCmdDvrBase)		/* CMS client count change : St_GrTaskMsgHd */
#define E_GrTaskMsgLogBkupStart			(12	+ E_GrTaskMsgCmdDvrBase)		/* log backup start : St_GrTaskMsgHd */
#define E_GrTaskMsgLogBkupEnd				(13	+ E_GrTaskMsgCmdDvrBase)		/* log backup end : St_GrTaskMsgHd */
#define E_GrTaskMsgLogBkupError			(14	+ E_GrTaskMsgCmdDvrBase)		/* log backup error : St_GrTaskMsgHd */
#define E_GrTaskMsgBkupSizeEstm			(15	+ E_GrTaskMsgCmdDvrBase)		/* backup size estimate : St_GrTaskMsgBkupSizeEstm */
#define E_GrTaskMsgFwUpgdStart			(16	+ E_GrTaskMsgCmdDvrBase)		/* firmware upgrade start : St_GrTaskMsgHd */
#define E_GrTaskMsgFwUpgdFinish			(17	+ E_GrTaskMsgCmdDvrBase)		/* firmware upgrade finish : St_GrTaskMsgFwUpgdEnd */
#define E_GrTaskMsgHolidayChg				(18	+ E_GrTaskMsgCmdDvrBase)		/* Holiday changed : St_GrTaskMsgHd */
#define E_GrTaskMsgFsStatChg				(19	+ E_GrTaskMsgCmdDvrBase)		/* file system Stat changed : St_GrTaskMsgHd */
#define	E_GrTaskMsgAutoLogout				(20	+ E_GrTaskMsgCmdDvrBase)		/* auto logout : St_GrTaskMsgHd */
#define	E_GrTaskMsgAppNotify				(21	+ E_GrTaskMsgCmdDvrBase)		/* application notify : St_GrTaskMsgAppNotify */
#define	E_GrTaskMsgRecTimeMapDone		(22	+ E_GrTaskMsgCmdDvrBase)		/* record day load done : St_GrTaskMsgRecTimeMapDone */
#define	E_GrTaskMsgEmgcRec					(23	+ E_GrTaskMsgCmdDvrBase)		/* emergency record : St_GrTaskMsgHd */
#define	E_GrTaskMsgDdnsRslt					(24	+ E_GrTaskMsgCmdDvrBase)		/* DDNS test result : St_GrTaskMsgDdnsRslt */
#define	E_GrTaskMsgFsLogGetDone			(25	+ E_GrTaskMsgCmdDvrBase)		/* file system log get done : St_GrTaskMsgFsLogGetDone */
#define	E_GrTaskMsgRecGrpFull				(26	+ E_GrTaskMsgCmdDvrBase)		/* record group is full : St_GrTaskMsgRecGrpFull */
#define E_GrTaskMsgFwUpgdFail				(27	+ E_GrTaskMsgCmdDvrBase)		/* firmware upgrade error : St_GrTaskMsgHd */


//#define E_GrTaskMsgQuadPosCfg				(0x001B	+ E_GrTaskMsgCmdDvrBase)		/* quad position change : St_GrTaskMsgQuadPosChg */

// system error
#define	E_GrTaskSysErrRaidDiskMis		1		/* RAID disk id missmatch , para0 = group id */
#define	E_GrTaskSysErrRaidBroken		2		/* RAID information broken , para0 = group id */

//====================================================================
//global macro


//====================================================================
//global type

typedef struct St_GrTaskSvrCtl
{
	void*	Obj;
	Def_GrPrcThrdId	ThrdId;
	__u32	Fcc;
	__u32	PrgsCnt;
	__u32	PrgsChk;
}	*Ptr_GrTaskSvrCtl;


typedef struct St_GrTaskEnv
{
	__u32	SvrRegCnt;

	St_GrTaskSvrCtl	SvrRegTbl[E_GrTaskSvrMaxRegCnt];

	St_GrPrcCritSct	SvrCrit;

	__u8	CmdAlocTbl[E_GrTaskSvrMsgCliCmdTblSize];

}	*Ptr_GrTaskEnv;

typedef	struct St_GrTaskMsgRtl			/* message result */
{
	__u32		Stat;					//finish status
}	*Ptr_GrTaskMsgRtl;


typedef	struct St_GrTaskMsgHd 
{
	__u32		Cmd;													// message command
	__u32		Size;													// message size
	Ptr_GrTaskMsgRtl	PtrRtl;							// runtime pointer - do not touch user
}	*Ptr_GrTaskMsgHd;

typedef struct St_GrTaskMsgQuadChgDone 
{
	St_GrTaskMsgHd	MsgHd;
	__u8	ConIdx;		// console index
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrTaskMsgQuadChgDone;

typedef struct St_GrTaskMsgNetIf
{
	St_GrTaskMsgHd	MsgHd;
	__u8	IfIdx;		// interface index
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrTaskMsgNetIf;

typedef	struct St_GrTaskMsgSysErr
{
	St_GrTaskMsgHd	MsgHd;
	__u32		ErrCode;
	__u32		ParaD1;
	__u32		ParaD2;
}	*Ptr_GrTaskMsgSysErr;

typedef	struct St_GrTaskMsgDiskFmtCpl
{
	St_GrTaskMsgHd	MsgHd;
	__u8		GroupMap;
	__u8		DiskMap;
	BOOL8		IsSuccess;
	__u8		_rsvd0;
	Def_GrErrCode	ErrCode;
}	*Ptr_GrTaskMsgDiskFmtCpl;

typedef	struct St_GrTaskMsgDiskFmtPrgs
{
	St_GrTaskMsgHd	MsgHd;
	__u8		GroupMap;
	__u8		DiskMap;
	__u16		_rsvd0;
	__u32		Prgs;		// percent
}	*Ptr_GrTaskMsgDiskFmtPrgs;

typedef struct St_GrTaskMsgTimeUpdt
{
	St_GrTaskMsgHd	MsgHd;
	Def_GrTime	Time;
	__u32	Fcc;				// owner device FCC
}	*Ptr_GrTaskMsgTimeUpdt;

typedef struct St_GrTaskMsgFwUpgdEnd
{
	St_GrTaskMsgHd	MsgHd;
	BOOL8	IsReboot;
	__u8	ErrCode;
}	*Ptr_GrTaskMsgFwUpgdEnd;

typedef struct St_GrTaskMsgBkupSizeEstm
{
	St_GrTaskMsgHd	MsgHd;
	__u64	RsltSize;
}	*Ptr_GrTaskMsgBkupSizeEstm;

typedef struct St_GrTaskMsgBkupEnd
{
	St_GrTaskMsgHd	MsgHd;
	__u8	ErrCode;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrTaskMsgBkupEnd;

typedef struct St_GrTaskMsgBkupPrgs
{
	St_GrTaskMsgHd	MsgHd;
	__u8	Prgs;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrTaskMsgBkupPrgs;

typedef struct St_GrTaskMsgGrpDskFsFix
{
	St_GrTaskMsgHd	MsgHd;
	__u8		Type;		// 0 = disk , 1 = group
	__u8		Index;	// index
	__u16		_rsvd1;
}	*Ptr_GrTaskMsgGrpDskFsFix;

typedef struct St_GrTaskMsgDskFsUpgd
{
	St_GrTaskMsgHd	MsgHd;
	__u8		Type;		// 0 = disk , 1 = group
	__u8		Index;	// index
	__u16		_rsvd1;
	__u32		NewVer;
	__u32		OldVer;
}	*Ptr_GrTaskMsgDskFsUpgd;

typedef struct St_GrTaskMsgDskIoErr
{
	St_GrTaskMsgHd	MsgHd;
	__u8		DiskIdx;
	__u8		_rsvd0;
	__u16		_rsvd1;
}	*Ptr_GrTaskMsgDskIoErr;

typedef struct St_GrTaskMsgAppNotify
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Para0;
	__u32	Para1;
}	*Ptr_GrTaskMsgAppNotify;

typedef struct St_GrTaskMsgRecTimeMapDone
{
	St_GrTaskMsgHd	MsgHd;
	__u32	UserData;
}	*Ptr_GrTaskMsgRecTimeMapDone;

typedef struct St_GrTaskMsgDdnsRslt
{
	St_GrTaskMsgHd	MsgHd;
	BOOL8		IsOk;
	__u8		_rsvd0;
	__u8		_rsvd1;
	__u8		_rsvd2;
}	*Ptr_GrTaskMsgDdnsRslt;

typedef struct St_GrTaskMsgFsLogGetDone
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Hndl;
}	*Ptr_GrTaskMsgFsLogGetDone;

typedef struct St_GrTaskMsgRecGrpFull
{
	St_GrTaskMsgHd	MsgHd;
	__u8	GrpType;
	__u8		_rsvd0;
	__u8		_rsvd1;
	__u8		_rsvd2;
}	*Ptr_GrTaskMsgRecGrpFull;

/*
typedef struct St_GrTaskMsgQuadPosChg
{
	St_GrTaskMsgHd	MsgHd;
	St_GrRect	RcQuad;		// quad screen rect
}	*Ptr_GrTaskMsgQuadPosChg;
*/

typedef struct St_GrTaskMsgNtpRslt
{
	St_GrTaskMsgHd	MsgHd;
	BOOL8		IsOk;
	__u8		_rsvd0;
	__u8		_rsvd1;
	__u8		_rsvd2;
}	*Ptr_GrTaskMsgNtpRslt;


//====================================================================
//class

//====================================================================
//global function
	
__s32	GrTaskSvrReg(void* A_Obj);
BOOL8	GrTaskSvrUnreg(void* A_Obj);
void*	GrTaskSvrFindByThreadId(Def_GrPrcThrdId A_Id);

void	GrTaskSvrMngInit(void);
void	GrTaskSvrMngFinish(void);

void*	GrTaskSvrGetByFcc(__u32 A_Fcc);
void*	GrTaskSvrGetByThrdId(Def_GrPrcThrdId A_ThrdId);
BOOL8	GrTaskSvrStepDoThis(void);
__u32	GrTaskSvrMsgCmdAlloc(void);			//return = 0: failure
void	GrTaskSvrMsgCmdFree(__u32 A_Cmd);
void	GrTaskSvrMsgBroadCast(Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_Size, BOOL8 A_IsFast = FALSE);
void	GrTaskSvrMsgSysErrNotify(__u32 A_ErrCode, __u32 A_ParaD1 = 0, __u32 A_ParaD2 = 0);

__s32	GrTaskSvrGetUnstable(__u32* A_PtrRtFcc);

//====================================================================
//out Environment conditional
#endif
