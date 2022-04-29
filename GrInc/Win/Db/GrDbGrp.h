/*
DB group
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_GrDbGrp
#define	_GrDbGrp

//====================================================================
// option
#define E_GrDbGrpKeySortUse

#define	E_GrDbGrpKeyOvlpDel

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


//====================================================================
//constance

#define	E_GrDbGrpKeyWriteImmGap				2048

#define	E_GrDbGrpKeyWritePeriod				10

#define	E_GrDbGrpDataCchFlushPeriod		10
#define	E_GrDbGrpDataCacheCnt					512

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDbGrpKeySortItem
{
	__u32	KeyId;
}	*Ptr_GrDbGrpKeySortItem;

typedef struct St_GrDbGrpKeyWriteCtl
{
	__u32	KeyIdSt;		// start id
	__u32	KeyIdEd;		// end id
}	*Ptr_GrDbGrpKeyWriteCtl;

typedef struct St_GrDbGrpDataCchItem
{
	__s64	Fpos;					// file position
	__u32	KeyId;				// data key id
	__u32	BlkId;				// block id
	__u32	Tick;					// tick at access
	__u32	TimerFlush;		// flush timer - dirt is not zero
	__u8	Buf[E_GrDbDatBlkSize];
}	*Ptr_GrDbGrpDataCchItem;

//====================================================================
//class

class Cls_GrDbGrp
{
	protected:
		__u32		m_GrpIdx;
		__u32		m_KeyValCnt;
		__u16		m_KeyValType;

		Def_WstrGrFnPath	m_StrFnKey;			// key file name
		Def_WstrGrFnPath	m_StrFnDat;			// data file name

		St_GrDbKeyInfo		m_KeyTbl[E_GrDbKeyMaxCnt];
#ifdef E_GrDbGrpKeySortUse
		St_GrDbGrpKeySortItem	m_KeySort[E_GrDbKeyMaxCnt];
#endif
		__u32	m_KeyTotal;
		__u32	m_KeyDeleted;
		__u32	m_KeyIdLast;

#ifdef E_GrDbGrpKeySortUse
		__u32	m_KeySrtCnt;		// sort item count
#endif

		__u32	m_Tick;					// work tick
		__u32	m_KeyWriteTimer;

		St_GrDbGrpKeyWriteCtl	m_KeyWt;		// key write control

		St_GrDbGrpDataCchItem	m_DatCch[E_GrDbGrpDataCacheCnt];		// data cache

		Cls_GrFileCtrl*	m_FcKey;
		Cls_GrFileCtrl*	m_FcDat;

		/* compare    result < 0 : source small, result == 0: equal, result > 0 : bigger source */
		__s32	LcCmpVal(void* A_PtrSr, void* A_PtrTg, __u16 A_ValType, __u16 A_ValCnt);		// unsigned big endian
		BOOL8	LcIsKeyValExist(void* A_PtrKeyVal);

		void	LcKeyStaticBuild(void);

		void	LcDataFileCreate(void);

#ifdef E_GrDbGrpKeySortUse
		void	LcKeySortBuild(void);
#endif

#ifdef E_GrDbGrpKeySortUse
		__s32	LcKeySortPosFirstSrGe(void* A_PtrKeyVal, __u32 A_IdxSt, __u32 A_IdxEd);
		__s32	LcKeySortPosFirstSrLe(void* A_PtrKeyVal, __u32 A_IdxSt, __u32 A_IdxEd);
		void	LcKeySortIns(__u32 A_KeyId, __u32 A_SrtIdx);
		void	LcKeySortAdd(__u32 A_KeyId);
#endif

#ifdef E_GrDbGrpKeyOvlpDel
		void	LcKeyOvlpRemove(void);
#endif

		__u32	LcKeyIdFindEmpty(void);

		void	LcKeyWriteFlush(void);
		void	LcKeyWriteAdd(__u32 A_KeyId);

		__s32	LcDatCchFindByKeyId(__u32 A_KeyId, __u32 A_BlkId);
		__s32	LcDatCchFindOldest(void);
		__s32	LcDatCchFindEmpty(void);

		void	LcDatCchFlushByCchIdx(__u32 A_CchIdx);

		__s32	LcDatCchObtain(__u32 A_KeyId, __u32 A_BlkId, BOOL8 A_IsRead);

		__u32	LcBlkChkSumCalc(void* A_PtrBuf);

		BOOL8	LcBlkFillZero(__u32 A_KeyId, __u32 A_BlkId, __u32 A_TimeUpdt);

		__u8	LcDatWrite(__u32 A_KeyId, __u32 A_Ofs, __u32 A_Size, void* A_PtrSr);
		__u8	LcDatRead(__u32 A_KeyId, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf);

	public:
		Cls_GrDbGrp(WCHAR* A_StrPathBase, __u32 A_GrpIdx, __u32 A_KeyValCnt, __u16 A_KeyValType);
		virtual	~Cls_GrDbGrp(void);

		__u32	KeyIdGetByKeyVal(void* A_PtrKeyVal);
		void*	KeyValGetByKeyId(__u32 A_KeyId);

		__u8	KeyCreate(void* A_PtrKeyVal,__u32* A_PtrRtKeyId);
		__u8	KeyDelByKeyVal(void* A_PtrKeyVal);

		__u8	WriteDataByKeyId(__u32 A_KeyId, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf);
		__u8	ReadDataByKeyId(__u32 A_KeyId, __u32 A_Ofs, __u32 A_Size, void* A_PtrBuf);

		__u32	KeyInfosGet(__u32 A_KeyIdSt, __u32 A_MaxCnt, Ptr_GrDbKeyInfo A_PtrBuf, __u32* A_PtrRtKeyIdProc);
		__u32	KeyInfoGetByKeyVal(void* A_PtrKeyVal, Ptr_GrDbKeyInfo A_PtrRtKeyInfo);
		BOOL8	KeyInfoGetByKeyId(__u32 A_KeyId, Ptr_GrDbKeyInfo A_PtrRtBuf);

		BOOL8	KeyAtbUpdate(Ptr_GrDbKeyAtb A_PtrKeyAtb, BOOL8 A_IsForce);
		BOOL8	KeyAtbGetByKeyId(__u32 A_KeyId, Ptr_GrDbKeyAtb A_PtrRtKeyAtb);

		__u32	DataChgInfoMake(Ptr_GrDbKeyInfo A_PtrKeyInfos, __u32 A_KeyInfoCnt,  Ptr_GrDbDataChgInfo A_PtrRtChgInfo);
		void	DataChgInfoToChgMap(Ptr_GrDbDataChgInfo A_PtrChgInfo, __u32 A_InfoCnt, __u32* A_PtrMap);

		BOOL8	ReadBlkDataAndInfoByKeyId(__u32 A_KeyId,__u32 A_BlkId, void* A_PtrRtBlkBuf, void* A_PtrRtKeyVal, 
			__u32* A_PtrRtChkSum, __u32* A_PtrRtTmUpdt);
		
		BOOL8	WriteBlkAndTimeByKeyVal(void* A_PtrKeyVal, __u32 A_BlkId, __u32 A_TmUpdt, void* A_PtrBlkData, BOOL8 A_IsForce);

		__u32	KeyFindByValRng(void* A_PtrKeySt, void* A_PtrKeyEd, void* A_PtrRtBuf, __u32 A_MaxCnt);

		void	RtlTimer(void);
};


//====================================================================
//global function


//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif


