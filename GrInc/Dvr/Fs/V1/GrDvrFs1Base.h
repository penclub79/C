/*
	GAUSE DVR file system v1 base

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs1Base
#define	_EPC_GrDvrFs1Base

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <Dvr/GrDvrBase.h>
	
//====================================================================
//constance

#define E_GrDvrFs1MaxDriveCnt					2

#define E_GrDvrFs1TmpBufSize					0x200000		/* 2 MByte */
#define	E_GrDvrFs1GrpBufSize					0x100000		/* 1 MByte */

#define E_GrDvrFs1StrmAlignSize				32
#define E_GrDvrFs1StrmAlignMask				0xFFFFFFE0

#define E_GrDvrFs1HndlSeqPos					48					/* record handle sequence id bit position */
#define E_GrDvrFs1HndlDskIdxMask			3LL					/* group idx mask */
#define E_GrDvrFs1HndlPosMask					0x0000FFFFFFFFFFE0LL					/* position mask */
#define E_GrDvrFs1HndlSeqLimite				0xFFFF			/* record handle sequence limit value */
#define E_GrDvrFs1HndlOldPart					0x0000000000000010LL	/* old part */

#ifdef FS_FCC
#define E_GrDvrFs1StrmFcc							FS_FCC
#else
//#define E_GrDvrFs1StrmFcc							Mac_GrMakeFcc('A','V','H','D')
#define E_GrDvrFs1StrmFcc							Mac_GrMakeFcc('N','D','A','V')
#endif

#define E_GrDvrFs1GrpIntNone					0xFF				/* not group appertain */

#define E_GrDvrFs1RposOk							0
#define E_GrDvrFs1RposBad							1
#define E_GrDvrFs1RposOld							2
#define E_GrDvrFs1RposEnd							3


/*
#define E_GrDvrFs1AdoSyncMax					16	// max audio sync - frame unit

#define E_GrDvrFs1RecIdNone						0
#define E_GrDvrFs1RecIntIdxBad				0xFF
*/

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs1SeekBuf
{
	void*	Ptr;
	__u32	Size;
	__u32	Readed;
	__u32	Prgs;
}	*Ptr_GrDvrFs1SeekBuf;

typedef	struct St_GrDvrFs1SeekCtrl 
{
	St_GrDvrFs1SeekBuf	Buf[2];
	__u64	Hndl;					// disk handle
	BOOL8	IsDblBuf;
	__u8	BufIdx;
	__u8	Stat;
	__u8	_rsvd0;
	//__u32	RecId;
	//__u8	_rsvd2;
}	*Ptr_GrDvrFs1SeekCtrl;


//====================================================================
//class

	
//====================================================================
//global function

void	GrDvrFs1HndlDec( __u64 A_Hndl, __u64* A_PtrRtRpos, __u8* A_PtrRtIntIdx, BOOL8* A_PtrRtIsOldPart, __u16* A_PtrRtOvwtId );
__u64	GrDvrFs1HndlEnc( __u64 A_Rpos, __u8 A_IntIdx, BOOL8 A_IsOldPart, __u16 A_OvwtId );

//====================================================================
//out Environment conditional
#endif


