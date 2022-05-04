/*
	GAUSE DVR file system v3 base

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs3Base
#define	_EPC_GrDvrFs3Base

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <Dvr/GrDvrBase.h>
	
//====================================================================
//constance

#define E_GrDvrFs3MaxDriveCnt					8

#define E_GrDvrFs3TmpBufSize					0x200000		/* 2 MByte */

#define E_GrDvrFs3StrmAlignSize				32
#define E_GrDvrFs3StrmAlignMask				0xFFFFFFE0

#define E_GrDvrFs3HndlOvwtIdPos				48					/* record handle sequence id bit position */
#define E_GrDvrFs3HndlGrpIdxMask			0x0FLL			/* disk idx mask */
#define E_GrDvrFs3HndlMarkBit					0x10LL			/* handle mark bit */
#define E_GrDvrFs3HndlRdaPosMask			0x0000FFFFFFFFFFE0LL					/* RDA position mask */

#ifdef FS_FCC
#define E_GrDvrFs3StrmFcc							FS_FCC
#else
#define E_GrDvrFs3StrmFcc							Mac_GrMakeFcc('A','V','H','D')
#endif

#define E_GrDvrFs3GrpIntNone					0xFF				/* not group appertain */

#define E_GrDvrFs3AdoSyncMax					16	// max audio sync - frame unit

#define E_GrDvrFs3RecIdNone						0
#define E_GrDvrFs3RecIntIdxBad				0xFF

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs3BlkAddInfo
{
	Def_GrTime TimeSt;
	Def_GrTime TimeEd;
	//Def_GrTime TimeBase;
	__u32	_rsvd0;
	__u8 AtbTbl[E_GrDvrMaxChCnt];
	__u32	ChSize[E_GrDvrMaxChCnt];
}	*Ptr_GrDvrFs3BlkAddInfo;

typedef struct St_GrDvrFs3SeekBuf
{
	void*	Ptr;
	__u32	Size;
	__u32	Readed;
	__u32	Prgs;
}	*Ptr_GrDvrFs3SeekBuf;

typedef	struct St_GrDvrFs3SeekCtrl 
{
	St_GrDvrFs3SeekBuf	Buf[2];
	__u64	Hndl;					// disk handle
	BOOL8	IsDblBuf;
	__u8	BufIdx;
	__u8	Stat;
	__u8	_rsvd0;
	__u32	RecId;
	//__u8	_rsvd2;
}	*Ptr_GrDvrFs3SeekCtrl;

typedef struct St_GrDvrFs3RecSizeReq
{
	__u8	DevType;				// input
	__u8	DskIdx;					// progress disk
	__u8	_rsvd1;					
	__u8	_rsvd0;
	Def_GrTime	TimeSt;		// input
	Def_GrTime	TimeEd;		// input
	__u16	ChMask;					// input
	__u16	BcsIdx;					// progress BCS index
	__u64	Rslt;
}	*Ptr_GrDvrFs3RecSizeReq;


//====================================================================
//class

	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


