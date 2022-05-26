/*
	GAUSE DVR file system v2 base

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs2Base
#define	_EPC_GrDvrFs2Base

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <Dvr/GrDvrBase.h>
	
//====================================================================
//constance

#define E_GrDvrFs2MaxDriveCnt					2

#define E_GrDvrFs2TmpBufSize					0x100000		/* 1 MByte */

#define E_GrDvrFs2StrmAlignSize				32
#define E_GrDvrFs2StrmAlignMask				0xFFFFFFE0

#define E_GrDvrFs2HndlOvwtIdPos				48					/* record handle sequence id bit position */
#define E_GrDvrFs2HndlGrpIdxMask			0x1FLL				/* disk idx mask */
#define E_GrDvrFs2HndlPosMask					0x0000FFFFFFFFFFE0LL					/* position mask */

#define E_GrDvrFs2StrmFcc							Mac_GrMakeFcc('A','V','H','D')

#define E_GrDvrFs2GrpIntNone					0xFF				/* not group appertain */

#define E_GrDvrFs2AdoSyncMax					16	// max audio sync - frame unit

#define E_GrDvrFs2RecIdNone						0
#define E_GrDvrFs2RecIntIdxBad				0xFF

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs2BlkAddInfo
{
	Def_GrTime TimeSt;
	Def_GrTime TimeEd;
	//Def_GrTime TimeBase;
	__u32	_rsvd0;
	__u8 AtbTbl[E_GrDvrMaxChCnt];
}	*Ptr_GrDvrFs2BlkAddInfo;

typedef struct St_GrDvrFs2SeekBuf
{
	void*	Ptr;
	__u32	Size;
	__u32	Readed;
	__u32	Prgs;
}	*Ptr_GrDvrFs2SeekBuf;

typedef	struct St_GrDvrFs2SeekCtrl 
{
	St_GrDvrFs2SeekBuf	Buf[2];
	__u64	Hndl;					// disk handle
	BOOL8	IsDblBuf;
	__u8	BufIdx;
	__u8	Stat;
	__u8	_rsvd0;
	__u32	RecId;
	//__u8	_rsvd2;
}	*Ptr_GrDvrFs2SeekCtrl;


//====================================================================
//class

	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


