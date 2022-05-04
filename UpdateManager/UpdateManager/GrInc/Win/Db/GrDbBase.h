/*

DB base

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_GrDbBase
#define	_GrDbBase

//====================================================================
// uses
#include <afxwin.h>

#include <GrTypeBase.h>
#include <GrTaskBase.h>
#include <GrTaskCli.h>
#include <GrTaskSvr.h>

#include <GrSockV2.h>

#include <GrError.h>
#include <GrStrTool.h>

//====================================================================
//constance


#define	E_GrDbKeySize							64
#define	E_GrDbKeyDwCnt						(E_GrDbKeySize >> 2)
#define	E_GrDbKeyQwCnt						(E_GrDbKeySize >> 3)

#define	E_GrDbKeyMaxCnt						0x20000

#define	E_GrDbGrpMaxCnt						8

#define	E_GrDbDatBlkSize					0x8000
#define	E_GrDbDatBlkShft					15
#define	E_GrDbDatBlkMaskRem				0x7FFF
#define	E_GrDbDatBlkMaxCnt				16
#define	E_GrDbDatBlkIdKeyInfo			0

#define	E_GrDbDatSize							(E_GrDbDatBlkMaxCnt * E_GrDbDatBlkSize)

#define	E_GrDbKeyIdNone						0

#define E_GrDbBaseDebug						1

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDbGrpInfo
{
	__u16	KeyValCnt;
	__u16	KeyValType;
}	*Ptr_GrDbGrpInfo;

typedef struct St_GrDbBlkStat
{
	__u32	TimeUpdt;
	__u32	ChkSum;
}	*Ptr_GrDbBlkStat;

typedef struct St_GrDbKeyAtb
{
	__u64	Key[E_GrDbKeyQwCnt];
	BOOL8	IsDel;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	TimeAtbUpdt;
	__u32	_rsvd3[14];
}	*Ptr_GrDbKeyAtb;

typedef struct St_GrDbKeyInfo
{
	St_GrDbKeyAtb		Atb;
	St_GrDbBlkStat	BlkStat[E_GrDbDatBlkMaxCnt];
}	*Ptr_GrDbKeyInfo;


typedef struct St_GrDbDataChgInfo
{
	__u64	Key[E_GrDbKeyQwCnt];
	__u32	ChgMap;
	__u32	_rsvd0;
}	*Ptr_GrDbDataChgInfo;

//====================================================================
//class

//====================================================================
//global function


//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif


