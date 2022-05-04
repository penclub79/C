/*
DB control
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_GrDbCtrl
#define	_GrDbCtrl

//====================================================================
// uses
#include <afxwin.h>

#include <GrTypeBase.h>
#include <GrTaskBase.h>
#include <GrTaskCli.h>
#include <GrTaskSvr.h>

#include <GrError.h>
#include <GrStrTool.h>
#include <GrFileCtrl.h>

#include <Win/Db/GrDbBase.h>
#include <Win/Db/GrDbGrp.h>
#include <Win/Db/GrDbSync.h>


//====================================================================
//constance

#define	E_GrDbCtrlTimerPeriod							1000		/* 1 sec */


#define	E_GrDbCtrlUpdtTimeGap							30

#define	E_GrDbCtrlFlushPeriod							10

// command
#define	E_GrDbCtrlCmdInit												1
#define	E_GrDbCtrlCmdFinish											2

#define	E_GrDbCtrlCmdGrpInfoGet									3		/* group info get */
#define	E_GrDbCtrlCmdKeyInfosGet								4		/* key info list get */
#define	E_GrDbCtrlCmdDataChgInfoMake						5		/* data change info make */
#define	E_GrDbCtrlCmdDataChgInfoToChgMap				6		/* data change info to change map */
#define	E_GrDbCtrlCmdKeyInfoGetByKeyVal					7		/* key info get by key value */
#define	E_GrDbCtrlCmdKeyInfoGetByKeyId					8		/* key info get by key id */
#define	E_GrDbCtrlCmdKeyAtbUpdate								9		/* key attribute upadate */
#define	E_GrDbCtrlCmdWriteBlkAndTimeByKeyVal		10	/* write block data and update time by key value */
#define	E_GrDbCtrlCmdKeyAtbGetByKeyId						11	/* key attribute get by key id */
#define	E_GrDbCtrlCmdReadBlkDataAndInfoByKeyId	12	/* read block data and info by key id */
#define	E_GrDbCtrlCmdSyncSet										13	/* sync set */
#define	E_GrDbCtrlCmdDataReadByOfs							14	/* data read by offset */
#define	E_GrDbCtrlCmdDataWriteByOfs							15	/* data write by offset */
#define	E_GrDbCtrlCmdKeyCreate									16	/* key create */
#define	E_GrDbCtrlCmdKeyFindByRng								17	/* key find by range */
#define	E_GrDbCtrlCmdKeyDelByKeyVal							18	/* key delete by key value */

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDbCtrlCmdBase
{
	St_GrTaskMsgHd	Hd;
	__u32		Cmd;
}	*Ptr_GrDbCtrlCmdBase;


typedef struct St_GrDbCtrlCmdInit
{
	St_GrDbCtrlCmdBase	Base;
}	*Ptr_GrDbCtrlCmdInit;

typedef struct St_GrDbCtrlCmdFinish
{
	St_GrDbCtrlCmdBase	Base;
}	*Ptr_GrDbCtrlCmdFinish;

typedef struct St_GrDbCtrlCmdGrpInfoGet
{
	St_GrDbCtrlCmdBase	Base;
	Ptr_GrDbGrpInfo			PtrRtBuf;
}	*Ptr_GrDbCtrlCmdGrpInfoGet;

typedef struct St_GrDbCtrlCmdKeyInfosGet
{
	St_GrDbCtrlCmdBase	Base;
	__u32*	PtrRslt;
	__u32		GrpId;
	__u32		KeyIdSt;
	__u32		MaxCnt;
	Ptr_GrDbKeyInfo		PtrBuf;
	__u32*	PtrRtKeyIdProc;
}	*Ptr_GrDbCtrlCmdKeyInfosGet;

typedef struct St_GrDbCtrlCmdDataChgInfoMake
{
	St_GrDbCtrlCmdBase	Base;
	__u32*	PtrRslt;
	__u32		GrpId;
	Ptr_GrDbKeyInfo			PtrKeyInfos;
	__u32		KeyInfoCnt;
	Ptr_GrDbDataChgInfo	PtrRtChgInfo;
}	*Ptr_GrDbCtrlCmdDataChgInfoMake;

typedef struct St_GrDbCtrlCmdDataChgInfoToChgMap
{
	St_GrDbCtrlCmdBase	Base;
	__u32		GrpId;
	Ptr_GrDbDataChgInfo	PtrChgInfo;
	__u32		InfoCnt;
	__u32*	PtrMap;
}	*Ptr_GrDbCtrlCmdDataChgInfoToChgMap;

typedef struct St_GrDbCtrlCmdKeyInfoGetByKeyVal
{
	St_GrDbCtrlCmdBase	Base;
	__u32*	PtrRslt;
	__u32		GrpId;
	void*		PtrKeyVal;
	Ptr_GrDbKeyInfo	PtrRtInfo;
}	*Ptr_GrDbCtrlCmdKeyInfoGetByKeyVal;

typedef struct St_GrDbCtrlCmdKeyInfoGetByKeyId
{
	St_GrDbCtrlCmdBase	Base;
	BOOL8*	PtrRslt;
	__u32		GrpId;
	__u32		KeyId;
	Ptr_GrDbKeyInfo	PtrRtBuf;
}	*Ptr_GrDbCtrlCmdKeyInfoGetByKeyId;

typedef struct St_GrDbCtrlCmdReadBlkDataAndInfoByKeyId
{
	St_GrDbCtrlCmdBase	Base;
	BOOL8*	PtrRslt;
	__u32		GrpId;
	__u32		KeyId;
	__u32		BlkId;
	void*		PtrRtBlkBuf;
	void*		PtrRtKeyVal;
	__u32*	PtrRtChkSum;
	__u32*	PtrRtTmUpdt;
}	*Ptr_GrDbCtrlCmdReadBlkDataAndInfoByKeyId;

typedef struct St_GrDbCtrlCmdKeyAtbUpdate
{
	St_GrDbCtrlCmdBase	Base;
	BOOL8*	PtrRslt;
	BOOL8		IsForce;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32		GrpId;
	Ptr_GrDbKeyAtb	PtrKeyAtb;
}	*Ptr_GrDbCtrlCmdKeyAtbUpdate;

typedef struct St_GrDbCtrlCmdWriteBlkAndTimeByKeyVal
{
	St_GrDbCtrlCmdBase	Base;
	BOOL8*	PtrRslt;
	BOOL8		IsForce;
	__u8		_rsvd0;
	__u8		_rsvd1;
	__u8		_rsvd2;
	__u32		GrpId;
	void*		PtrKeyVal;
	__u32		BlkId;
	__u32		TmUpdt;
	void*		PtrBlkData;
}	*Ptr_GrDbCtrlCmdWriteBlkAndTimeByKeyVal;

typedef struct St_GrDbCtrlCmdKeyAtbGetByKeyId
{
	St_GrDbCtrlCmdBase	Base;
	BOOL8*	PtrRslt;
	__u32		GrpId;
	__u32		KeyId;
	Ptr_GrDbKeyAtb	PtrRtKeyAtb;
}	*Ptr_GrDbCtrlCmdKeyAtbGetByKeyId;

typedef struct St_GrDbCtrlCmdSyncSet
{
	St_GrDbCtrlCmdBase	Base;
	BOOL8		IsServer;
	__u16		Port;
	__u32		IpSv;
	Ptr_GrDbKeyAtb	PtrRtKeyAtb;
}	*Ptr_GrDbCtrlCmdSyncSet;

typedef struct St_GrDbCtrlCmdDataReadByOfs
{
	St_GrDbCtrlCmdBase	Base;
	__u8*		PtrRslt;
	__u32		GrpId;
	__u32		Ofs;
	__u32		Size;
	void*		PtrKeyVal;
	void*		PtrBuf;
}	*Ptr_GrDbCtrlCmdDataReadByOfs;

typedef struct St_GrDbCtrlCmdDataWriteByOfs
{
	St_GrDbCtrlCmdBase	Base;
	__u8*		PtrRslt;
	__u32		GrpId;
	__u32		Ofs;
	__u32		Size;
	void*		PtrKeyVal;
	void*		PtrBuf;
}	*Ptr_GrDbCtrlCmdDataWriteByOfs;

typedef struct St_GrDbCtrlCmdKeyCreate
{
	St_GrDbCtrlCmdBase	Base;
	__u8*		PtrRslt;
	__u32		GrpId;
	void*		PtrKeyVal;
}	*Ptr_GrDbCtrlCmdKeyCreate;

typedef struct St_GrDbCtrlCmdKeyFindByValRng
{
	St_GrDbCtrlCmdBase	Base;
	__u32*	PtrRslt;
	__u32		GrpId;
	__u32		MaxCnt;
	void*		PtrKeySt;
	void*		PtrKeyEd;
	void*		PtrRtBuf;
}	*Ptr_GrDbCtrlCmdKeyFindByValRng;

typedef struct St_GrDbCtrlCmdKeyDelByKeyVal
{
	St_GrDbCtrlCmdBase	Base;
	__u8*		PtrRslt;
	__u32		GrpId;
	void*		PtrKeyVal;
}	*Ptr_GrDbCtrlCmdKeyDelByKeyVal;


//====================================================================
//class

class Cls_GrDbCtrl : public Cls_GrTaskCli
{
	protected:
		Cls_GrTaskSvr*		m_TskSvr;
		__u32							m_MsgCmd;
		__u32							m_TmrSec;

		// local
		Def_WstrGrFnPath	m_StrPath;

		Cls_GrTaskSvr*		m_TskSync;
		Cls_GrTaskCli*		m_DbSync;

		St_GrDbGrpInfo		m_GrpInfo[E_GrDbGrpMaxCnt];
		Cls_GrDbGrp*			m_ObjGrp[E_GrDbGrpMaxCnt];


		// api

		// control

		// event

		// background work

		// timer work

		// inherited
		virtual	BOOL8	RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg);		/* return = message processed */
		virtual	BOOL8	RtlBkgProc(void);												/* return = background processed */
		virtual	void	RtlTimer(__u32 A_Hndl);

	public:
		Cls_GrDbCtrl(Cls_GrTaskSvr* A_TaskSvr, WCHAR* A_StrDbPath, Ptr_GrDbGrpInfo A_PtrGrpInfo);
		virtual	~Cls_GrDbCtrl(void);

		// immidatly process
		void	ImmInit(void);
		void	ImmFinish(void);

		void	ImmGrpInfoGet(Ptr_GrDbGrpInfo A_PtrBuf);
		__u32	ImmKeyInfosGet(__u32 A_GrpId, __u32 A_KeyIdSt, __u32 A_MaxCnt, Ptr_GrDbKeyInfo A_PtrBuf, __u32* A_PtrRtKeyIdProc);
		__u32	ImmDataChgInfoMake(__u32 A_GrpId, Ptr_GrDbKeyInfo A_PtrKeyInfos, __u32 A_KeyInfoCnt, Ptr_GrDbDataChgInfo A_PtrRtChgInfo);
		void	ImmDataChgInfoToChgMap(__u32 A_GrpId, Ptr_GrDbDataChgInfo A_PtrChgInfo, __u32 A_InfoCnt, __u32* A_PtrMap);
		__u32	ImmKeyInfoGetByKeyVal(__u32 A_GrpId, void* A_PtrKeyVal, Ptr_GrDbKeyInfo A_PtrRtKeyInfo);
		BOOL8	ImmKeyInfoGetByKeyId(__u32 A_GrpId, __u32 A_KeyId, Ptr_GrDbKeyInfo A_PtrRtBuf);
		BOOL8	ImmReadBlkDataAndInfoByKeyId(__u32 A_GrpId, __u32 A_KeyId, __u32 A_BlkId, void* A_PtrRtBlkBuf, void* A_PtrRtKeyVal,
			__u32* A_PtrRtChkSum, __u32* A_PtrRtTmUpdt);
		BOOL8	ImmKeyAtbUpdate(__u32 A_GrpId, Ptr_GrDbKeyAtb A_PtrKeyAtb, BOOL8 A_IsForce);
		BOOL8	ImmWriteBlkAndTimeByKeyVal(__u32 A_GrpId, void* A_PtrKeyVal, __u32 A_BlkId, __u32 A_TmUpdt, void* A_PtrBlkData,BOOL8 A_IsForce);
		BOOL8	ImmKeyAtbGetByKeyId(__u32 A_GrpId, __u32 A_KeyId, Ptr_GrDbKeyAtb A_PtrRtKeyAtb);

		void	ImmSyncSet(BOOL8 A_IsServer, __u32 A_IpSv, __u16 A_Port);

		__u8	ImmDataReadByOfs(__u32 A_GrpId, void* A_PtrKeyVal, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf);
		__u8	ImmDataWriteByOfs(__u32 A_GrpId, void* A_PtrKeyVal, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf);

		__u8	ImmKeyCreate(__u32 A_GrpId, void* A_PtrKeyVal);
		__u8	ImmKeyDelByKeyVal(__u32 A_GrpId, void* A_PtrKeyVal);

		__u32	ImmKeyFindByValRng(__u32 A_GrpId, void* A_PtrKeySt, void* A_PtrKeyEd, void* A_PtrRtBuf, __u32 A_MaxCnt);

		// thead safe process
		BOOL8	ReqInit(void);
		BOOL8	ReqFinish(void);

		void	ReqGrpInfoGet(Ptr_GrDbGrpInfo A_PtrBuf);
		__u32	ReqKeyInfosGet(__u32 A_GrpId, __u32 A_KeyIdSt, __u32 A_MaxCnt, Ptr_GrDbKeyInfo A_PtrBuf, __u32* A_PtrRtKeyIdProc);
		__u32	ReqDataChgInfoMake(__u32 A_GrpId, Ptr_GrDbKeyInfo A_PtrKeyInfos, __u32 A_KeyInfoCnt, Ptr_GrDbDataChgInfo A_PtrRtChgInfo);
		void	ReqDataChgInfoToChgMap(__u32 A_GrpId, Ptr_GrDbDataChgInfo A_PtrChgInfo, __u32 A_InfoCnt, __u32* A_PtrMap);
		__u32	ReqKeyInfoGetByKeyVal(__u32 A_GrpId, void* A_PtrKeyVal, Ptr_GrDbKeyInfo A_PtrRtKeyInfo);
		BOOL8	ReqKeyInfoGetByKeyId(__u32 A_GrpId, __u32 A_KeyId, Ptr_GrDbKeyInfo A_PtrRtBuf);
		BOOL8	ReqReadBlkDataAndInfoByKeyId(__u32 A_GrpId, __u32 A_KeyId, __u32 A_BlkId, void* A_PtrRtBlkBuf, void* A_PtrRtKeyVal,
			__u32* A_PtrRtChkSum, __u32* A_PtrRtTmUpdt);
		BOOL8	ReqKeyAtbUpdate(__u32 A_GrpId, Ptr_GrDbKeyAtb A_PtrKeyAtb, BOOL8 A_IsForce);
		BOOL8	ReqWriteBlkAndTimeByKeyVal(__u32 A_GrpId, void* A_PtrKeyVal, __u32 A_BlkId, __u32 A_TmUpdt, void* A_PtrBlkData,BOOL8 A_IsForce);
		BOOL8	ReqKeyAtbGetByKeyId(__u32 A_GrpId, __u32 A_KeyId, Ptr_GrDbKeyAtb A_PtrRtKeyAtb);

		void	ReqSyncSet(BOOL8 A_IsServer, __u32 A_IpSv, __u16 A_Port);

		__u8	ReqDataReadByOfs(__u32 A_GrpId, void* A_PtrKeyVal, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf);
		__u8	ReqDataWriteByOfs(__u32 A_GrpId, void* A_PtrKeyVal, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf);

		__u8	ReqKeyCreate(__u32 A_GrpId, void* A_PtrKeyVal);
		__u8	ReqKeyDelByKeyVal(__u32 A_GrpId, void* A_PtrKeyVal);

		__u32	ReqKeyFindByValRng(__u32 A_GrpId, void* A_PtrKeySt, void* A_PtrKeyEd, void* A_PtrRtBuf, __u32 A_MaxCnt);

};


//====================================================================
//global function


//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif


