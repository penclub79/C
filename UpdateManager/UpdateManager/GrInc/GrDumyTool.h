//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDumyTool
#define	_EPC_GrDumyTool

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#ifdef LINUX_APP
#include	<string.h>
#endif
//====================================================================
//constance

#define 	E_GrDumyNotFoundIdx		0xFFFFFFFF

//====================================================================
//global macro
#define		GrDumyMemCmp	memcmp
#define		GrDumyMoveMem	memmove
#define		GrDumyCopyMem	memmove

#ifdef LINUX_APP
#define	GrDumyDwGetNoneZeroBitCount	(__u32)__builtin_popcount
#else
#define	GrDumyDwGetNoneZeroBitCount	(__u32)__popcnt
#endif 


//====================================================================
//global type

typedef struct St_GrDumyBase64Enc
{
	__u8	RmDat[3];
	__u8	RmSize;
}	*Ptr_GrDumyBase64Enc;

typedef struct St_GrDumyBase64Dec
{
	__u8	RmDat[4];
	__u32	RmSize;
}	*Ptr_GrDumyBase64Dec;


//====================================================================
//global function
	
	void		GrDumyZeroMem( void* A_PtrTg, __u32 A_Size );		//zero memory	
	
	__u32		GrDumyDwGetFirstBitIdx( __u32 A_Mask );
	__u32		GrDumyDwClearBit( __u32 A_Mask,__u32 A_BitIdx );
	void		GrDumyInsSpace( void* A_PtrBuf, __u32 A_BufSize, __u32 A_InsPos, __u32 A_InsSize );
	void		GrDumyDelSpace( void* A_PtrBuf, __u32 A_BufSize, __u32 A_DelPos, __u32 A_DelSize );
	void		GrDumyCopyWithSpace( void* A_PtrTg, void* A_PtrSr, __u32 A_SizeSr, __u32 A_DivPos, __u32 A_SizeSpace );

					//input	-		A_PtrTbl	:	dword table pointer
					//					A_PtrRtCnt	:	table item counter variable pointer - store table item count
					//output	-	A_PtrRtCnt	:	store collected item count
	void		GrDumyCollectNullDword( void* A_PtrTbl, __s32* A_PtrRtCnt );		//collect null item dword unit

	__u32		GrDumyGetBits( void* A_PtrSr, __u32 A_BitPosSr, __u32 A_GetBitSize, __u32 A_GetBitPos, __u32* A_PtrRtMask );
	__u32		GrDumyGetNoneZeroBitMask( __u32 A_Value, __u32 A_BitUnit );

	BOOL8		GrDumyCmpDwUnit( void* A_PtrSr, void* A_PtrTg, __u32 A_DwCnt );

#ifdef GRACC_X86
	//----------------------------------------
	// x86 accelat - start
	//----------------------------------------

	inline	__u32	GrDumyDwGetZeroBitCntRight( __u32 A_Val )
	{
#ifdef LINUX_APP
		// local -------------------
		__u32		Tv_Result;
		// code --------------------
		//gcc
		__asm__ __volatile__(
			"movl	32 , %0	\n\t"
			"bsfl	%1 , %0										\n\t"
			:"=r" (Tv_Result) 
			:"rm" (A_Val)
			);
		return	Tv_Result;
#else
		//vcc
		__asm
		{
			mov		eax , 32
				bsf		eax , A_Val
		}
#endif
	}

	inline	__u32	GrDumyDwGetLastBitIdx( __u32 A_Mask )
	{
#ifdef LINUX_APP
		// local -------------------
		__u32		Tv_Result;
		// code --------------------
		//gcc
		__asm__ __volatile__(
			"movl	E_GrDumyNotFoundIdx , %0	\n\t"
			"bsrl	%1 , %0										\n\t"
			:"=r" (Tv_Result) 
			:"rm" (A_Mask)
			);
		return	Tv_Result;
#else
		//vcc
		__asm
		{
			mov		eax , E_GrDumyNotFoundIdx
				bsr		eax , A_Mask
		}
#endif
	}

	inline	void*	GrDumyFillD( void* A_PtrBuf, __u32 A_DwCnt, __u32 A_Val )
	{
#ifdef LINUX_APP
		//gcc
		// local -------------------
		void*		Tv_Result;
		// code --------------------
		__asm__ __volatile__(
			"movl	E_GrDumyNotFoundIdx , %0	\n\t"
			"rep				\n\t"
			"stosl			\n\t"
			:"=D"(Tv_Result)
			:"a"(A_Val),"c"(A_DwCnt), "D"(A_PtrBuf)
			);
		return	Tv_Result;
#else
		//vcc
		__asm
		{
			mov		eax , A_Val
				mov		edi , A_PtrBuf
				mov		ecx , A_DwCnt
				rep	stosd
				mov		eax , edi
		}
#endif
	}

	inline	void*	GrDumyFillW( void* A_PtrBuf, __u32 A_WdCnt, __u16 A_Val )
	{
#ifdef LINUX_APP
		//gcc
		// local -------------------
		void*		Tv_Result;
		// code --------------------
		__asm__ __volatile__(
			"movl	E_GrDumyNotFoundIdx , %0	\n\t"
			"rep				\n\t"
			"stosw			\n\t"
			:"=D"(Tv_Result)
			:"a"(A_Val),"c"(A_WdCnt), "D"(A_PtrBuf)
			);
		return	Tv_Result;
#else
		//vcc
		__asm
		{
			movzx	eax , A_Val
				mov		edi , A_PtrBuf
				mov		ecx , A_WdCnt
				rep	stosw
				mov		eax , edi
		}
#endif
	}

	inline	void*	GrDumyFillB( void* A_PtrBuf, __u32 A_BtCnt, __u8 A_Val )
	{
#ifdef LINUX_APP
		//gcc
		// local -------------------
		void*		Tv_Result;
		// code --------------------
		__asm__ __volatile__(
			"movl	E_GrDumyNotFoundIdx , %0	\n\t"
			"rep				\n\t"
			"stosw			\n\t"
			:"=D"(Tv_Result)
			:"a"(A_Val),"c"(A_BtCnt), "D"(A_PtrBuf)
			);
		return	Tv_Result;
#else
		//vcc
		__asm
		{
			movzx	eax , A_Val
				mov		edi , A_PtrBuf
				mov		ecx , A_BtCnt
				rep	stosb
				mov		eax , edi
		}
#endif
	}
	//----------------------------------------
	//defatul = x86 archtecture - finish
	//----------------------------------------
#else
//----------------------------------------
//not accelate - start
//----------------------------------------
#define GRACC_DUMYTOOL_NONE

__u32	GrDumyDwGetZeroBitCntRight( __u32 A_Val );
__u32	GrDumyDwGetLastBitIdx( __u32 A_Mask );
void*	GrDumyFillD( void* A_PtrBuf, __u32 A_DwCnt, __u32 A_Val );
void*	GrDumyFillW( void* A_PtrBuf, __u32 A_WdCnt, __u16 A_Val );
void*	GrDumyFillB( void* A_PtrBuf, __u32 A_BtCnt, __u8 A_Val );

__u8	GrDumyBitRvsU8(__u8 A_Src);
__u32	GrDumyDwBitReverse(__u32 A_Src, __u32 A_BitPos, __u32 A_BitCnt, __u32 A_Dst, __u32 A_BitDst);

//----------------------------------------
//not accelerate - finish
//----------------------------------------
#endif

__u32		GrDumyCalcChksumBt(void* A_PtrBuf, __u32 A_Size, __u32 A_PrevChksum = 0);		//calculate checksum byte unit
__u32		GrDumyCalcChksumDw(void* A_PtrBuf, __u32 A_Size, __u32 A_PrevChksum = 0);		//calculate checksum dword unit

	void	GrDumySwapByte1Bit( void* A_PtrBuf, __u32 A_ByteCnt );
	void	GrDumySwapByte4Bit( void* A_PtrBuf, __u32 A_ByteCnt );

	BOOL8	GrDumyAbandoneByDw(void* A_PtrBuf, __u32 A_BufSize, __u32 A_Basis, __u32* A_PtrRtSize);

	//endian
	void	GrDumyEndian4byteSet( void* A_PtrTg, __u32 A_Val );
	void	GrDumyEndian3byteSet( void* A_PtrTg, __u32 A_Val );
	void	GrDumyEndian2byteSet( void* A_PtrTg, __u16 A_Val );
	__u64	GrDumyEndian8byteGet(void* A_PtrSr);
	__u32	GrDumyEndian4byteGet( void* A_PtrSr );
	__u32	GrDumyEndian3byteGet( void* A_PtrSr );
	__u16	GrDumyEndian2byteGet( void* A_PtrSr );
	void	GrDumyWrite4Byte( void* A_PtrTg, __u32 A_Val );
	void	GrDumyWrite3Byte( void* A_PtrTg, __u32 A_Val );
	void	GrDumyWrite2Byte( void* A_PtrTg, __u16 A_Val );
	__u32	GrDumyRead4Byte( void* A_PtrSr );
	__u32	GrDumyRead3Byte( void* A_PtrSr );
	__u16	GrDumyRead2Byte( void* A_PtrSr );
	void	GrDumyEndianTrsBuf( void* A_PtrTg, void* A_PtrSr, __u32 A_Size );
	void	GrDumyEndianTrsDwBuf( void* A_PtrTg, void* A_PtrSr, __u32 A_Size );

	// basd 64
	void	GrDumyBase64EncInit(Ptr_GrDumyBase64Enc A_PtrEnv);
	__u32	GrDumyBase64EncAdd(Ptr_GrDumyBase64Enc A_PtrEnv, void* A_PtrSr, __u32 A_SrSize, void* A_PtrOut);
	__u32	GrDumyBase64EncFinish(Ptr_GrDumyBase64Enc A_PtrEnv, void* A_PtrOut);

	void	GrDumyBase64DecInit(Ptr_GrDumyBase64Dec A_PtrEnv);
	__u32	GrDumyBase64DecDo(Ptr_GrDumyBase64Dec A_PtrEnv, void* A_PtrSr, __u32 A_SrSize, void* A_PtrOut);
	void	GrDumyBase64DecFinish(Ptr_GrDumyBase64Dec A_PtrEnv);


//====================================================================
//out Environment conditional
#endif

