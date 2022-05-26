/*
	dumy Tools	-	window version
		
*/

//====================================================================
// uses

#include	<GrDumyTool.h>

//====================================================================
//local type

//====================================================================
//local function

__u8	GrDumyBase64CharToCode(__u8 A_Char);

//====================================================================
//var

extern	char	V_GrStrToolBase64Tbl[65];

//====================================================================
__u32		GrDumyDwGetFirstBitIdx( __u32 A_Mask )
{
	// local -------------------
		__u32				Tv_Result;
		__u32				Tv_WkIdx;
		__u32				Tv_Mask;
	// code --------------------
		//init
		Tv_Result		=	E_GrDumyNotFoundIdx;
		Tv_Mask			=	A_Mask;
		//find
		for (Tv_WkIdx = 0;Tv_WkIdx < 32;Tv_WkIdx ++)
		{
			//check exist
			if ( 0 != (Tv_Mask & 1) ) 
			{
				//found
				Tv_Result		=	Tv_WkIdx;
				break;
			}
			//next
			Tv_Mask		=	Tv_Mask >> 1;
		}
	
		return	( Tv_Result );

}

//--------------------------------------------------------------------
__u32		GrDumyDwClearBit( __u32 A_Mask,__u32 A_BitIdx )
{
	// local -------------------
		__u32				Tv_Result;
		__u32				Tv_Mask;
	// code --------------------
		//init
		Tv_Result		=	A_Mask;
		//check current range
		if ( A_BitIdx < 32 ) 
		{
			//clear bit
			Tv_Mask			=	1 << A_BitIdx;
			Tv_Result		=	Tv_Result & (~Tv_Mask);
		}

		return	( Tv_Result );
}

//--------------------------------------------------------------------
void			GrDumyInsSpace( void* A_PtrBuf, __u32 A_BufSize, __u32 A_InsPos, __u32 A_InsSize )
{
	// local -------------------
		__u8*		Tv_PtrTail;
		__u32		Tv_SizeTail;
		__u8*		Tv_PtrTo;
  // code --------------------
		//init

		//calc tail address
		Tv_PtrTail		=	((__u8*)A_PtrBuf) + A_InsPos;
		Tv_SizeTail		=	A_BufSize - A_InsPos;
		Tv_PtrTo			=	Tv_PtrTail + A_InsSize;
		if ( A_InsPos < A_BufSize )
		{
			GrDumyMoveMem( (void*)Tv_PtrTo	, (void*)Tv_PtrTail, Tv_SizeTail );
		}

}

//--------------------------------------------------------------------
void		GrDumyDelSpace( void* A_PtrBuf, __u32 A_BufSize, __u32 A_DelPos, __u32 A_DelSize )
{
	// local -------------------
	__u8*		Tv_PtrTail;
		__u32		Tv_SizeTail;
		__u8*		Tv_PtrTo;
  // code --------------------
		//init

		//calc tail address
		Tv_PtrTail		=	((__u8*)A_PtrBuf) + A_DelPos + A_DelSize;
		Tv_SizeTail		=	A_BufSize - A_DelPos - A_DelSize;
		Tv_PtrTo			=	((__u8*)A_PtrBuf) + A_DelPos;
		if ( (A_DelPos + A_DelSize) < A_BufSize )
		{
			GrDumyMoveMem( (void*)Tv_PtrTo	, (void*)Tv_PtrTail, Tv_SizeTail );
		}

}

//--------------------------------------------------------------------
void		GrDumyCopyWithSpace( void* A_PtrTg, void* A_PtrSr, __u32 A_SizeSr, __u32 A_DivPos, __u32 A_SizeSpace )
{
	// local -------------------
		__u8*		Tv_PtrTail;
		__u32		Tv_SizeTail;
		__u8*		Tv_PtrTo;
  // code --------------------
		//init

		//copy head
		if ( 0 != A_DivPos )
		{
			GrDumyMoveMem( A_PtrTg, A_PtrSr, A_DivPos );
		}

		//copy tail
		Tv_PtrTail		=	((__u8*)A_PtrSr) + A_DivPos;
		Tv_SizeTail		=	A_SizeSr - A_DivPos;
		Tv_PtrTo			=	((__u8*)A_PtrTg) + A_DivPos + A_SizeSpace;
		if ( A_DivPos < A_SizeSr )
		{
			GrDumyMoveMem( (void*)Tv_PtrTo	, (void*)Tv_PtrTail, Tv_SizeTail );
		}

}

//--------------------------------------------------------------------
void		GrDumyCollectNullDword( void* A_PtrTbl, __s32* A_PtrRtCnt )
{
	// local -------------------
		__s32			Tv_WkIdx;
		__u32*		Tv_PtrChk;		//check pointer
		__u32*		Tv_PtrWt;			//write pointer
		__s32			Tv_WkCnt;
	// code --------------------
		//check workable
		if ( *A_PtrRtCnt != 0 )
		{
			Tv_WkCnt		=	*A_PtrRtCnt;
			Tv_PtrChk		=	(__u32*)A_PtrTbl;
			Tv_PtrWt		=	Tv_PtrChk;
			for ( Tv_WkIdx = 0;Tv_WkIdx < Tv_WkCnt;Tv_WkIdx ++ )
			{
				if ( 0 == *Tv_PtrChk )
				{
					//patch
					(*A_PtrRtCnt)	--;
				}
				else 
				{
					if ( Tv_PtrWt != Tv_PtrChk )
					{
						//align
						*Tv_PtrWt	=	*Tv_PtrChk;
					}
					Tv_PtrWt ++;
				}
				//next
				Tv_PtrChk ++;
			}
		}

}
//--------------------------------------------------------------------
void		GrDumyZeroMem( void* A_PtrTg, __u32 A_Size )
{
	#ifdef LINUX_APP
		memset( A_PtrTg, 0, A_Size );
	#else
		ZeroMemory( A_PtrTg, A_Size );
	#endif
}
//--------------------------------------------------------------------
/*!
 * \brief
 * sampling bits
 * 
 * \param A_PtrSr
 * pointer source
 * 
 * \param A_BitPos
 * bit position of source
 * 
 * \param A_GetBitSize
 * sampling bit size
 * 
 * \param A_GetBitPos
 * bit positon of result
 * 
 * \param A_PtrRtMask
 * pointer of result bit mask
 * 
 * \returns
 * sample data
 * 
 * \remarks
 * "GrDumyGetBits" can have parameter as sum of "A_GetBitSize" and "A_GetBitPos"
 * 
 */
__u32		GrDumyGetBits( void* A_PtrSr, __u32 A_BitPosSr, __u32 A_GetBitSize, __u32 A_GetBitPos, __u32* A_PtrRtMask )
{
	// local -------------------
		__u32		Tv_Result;
		__u32*	Tv_PtrSr;					//source pointer
		__u32		Tv_BpSr;					//bit position source
		__u32		Tv_BsCap;					//captured bit size
		__u32		Tv_BpCap;					//capture bit pos
		__u32		Tv_Mask;					//mask
		__u32		Tv_GapByte;
		__u32		Tv_Smp;						//sample
		__u32		Tv_SftSmp;				//sample shifter
		__u32		Tv_BsSmp;					//bit size sample

	// code --------------------
		//init
		Tv_Result			=	0;
		*A_PtrRtMask	=	0;
		Tv_BsCap			=	0;
		Tv_BpCap			=	A_GetBitPos;

		//check capture able
		if ( 32 >= (A_GetBitPos + A_GetBitSize) )
		{

			Tv_PtrSr			=	(__u32*)A_PtrSr;
			Tv_BpSr				=	A_BitPosSr;
			//patch source position
			Tv_GapByte	=	(Def_GrCalPtr)Tv_PtrSr & 3;
			if ( 0 != Tv_GapByte )
			{
				Tv_PtrSr	=	(__u32*)( (Def_GrCalPtr)Tv_PtrSr & ~3 );
				Tv_BpSr		=	Tv_BpSr + (Tv_GapByte	* 8);
			}

			Tv_GapByte	=	Tv_BpSr >> 5;
			if ( 0 != Tv_GapByte )
			{
				Tv_PtrSr	=	Tv_PtrSr + Tv_GapByte;
			}

			//start sampling
			while ( Tv_BsCap < A_GetBitSize )
			{
				//sampling
				Tv_Smp	=	*Tv_PtrSr;
				Tv_BsSmp		=	32 - Tv_BpSr;
				if ( Tv_BpCap > Tv_BpSr )
				{
					Tv_SftSmp		=	Tv_BpCap - Tv_BpSr;
					Tv_BsSmp		=	Tv_BsSmp - Tv_SftSmp;
					Tv_Smp		=	Tv_Smp << Tv_SftSmp;
				}
				else if ( Tv_BpCap < Tv_BpSr )
				{
					Tv_SftSmp		=	Tv_BpSr - Tv_BpCap;
					Tv_Smp			=	Tv_Smp >> Tv_SftSmp;
				}
				//make mask
				Tv_Mask		=	(1 << Tv_BsSmp) - 1;
				Tv_Mask		=	Tv_Mask << Tv_BpCap;

				//update result
				Tv_Result		=	Tv_Result | (Tv_Smp & Tv_Mask);
				*A_PtrRtMask	=	*A_PtrRtMask | Tv_Mask;

				//next
				Tv_BsCap		=	Tv_BsCap + Tv_BsSmp;
				Tv_BpCap		=	Tv_BpCap + Tv_BsSmp;
				Tv_BpSr			=	Tv_BpSr + Tv_BsSmp;

				//patch source pointer
				Tv_GapByte	=	Tv_BpSr >> 5;
				if ( 0 != Tv_GapByte  )
				{
					Tv_PtrSr	=	Tv_PtrSr + Tv_GapByte;
				}
			}
		}

		return		Tv_Result;

}
//--------------------------------------------------------------------
/*!
 * \brief
 * make bit mask for none zero area
 * 
 * \param A_Value
 * source value
 * 
 * \param A_BitUnit
 * check bit unit
 * 
 * \returns
 * none zero area bit mask
 * 
 * \remarks
 * Bit unit must be 1 , 2 , 4 , 8, 16 , 32
 * 
 */
__u32		GrDumyGetNoneZeroBitMask( __u32 A_Value, __u32 A_BitUnit )
{
	// local -------------------
		__u32		Tv_Result;
		__s32			Tv_LastBit;
		__u32		Tv_Mask;
		__u32		Tv_Smp;
	// code --------------------
		//init
		Tv_Result		=	0;
		Tv_LastBit	=	32;

		if (0 != A_BitUnit)
		{
			Tv_Smp		=	A_Value;
			Tv_Mask		=	(1 << A_BitUnit) - 1;
			do 
			{
				Tv_Result		=	Tv_Result << A_BitUnit;
				if ( 0 != ( Tv_Mask & Tv_Smp) )
				{
					Tv_Result		=	Tv_Result | Tv_Mask;
				}
				//next
				Tv_Smp			=	Tv_Smp >> A_BitUnit;
				Tv_LastBit	=	Tv_LastBit - A_BitUnit;
			} while( 0 < Tv_LastBit );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
/*
inline	__u32		GrDumyDwGetZeroBitCntRight( __u32 A_Val )
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
*/

/*
orignal
// local -------------------
__u32		Tv_Result;
__u32		Tv_WkIdx;
__u32		Tv_Val;
// code --------------------
//init
Tv_Result		=	0;
Tv_Val			=	A_Val;

for ( Tv_WkIdx=0;Tv_WkIdx < 32;Tv_WkIdx ++ )
{
if ( 0 != (Tv_Val & 1) )
{
//found
break;
}
//next
Tv_Val		=	Tv_Val >> 1;
Tv_Result ++;
}

return	Tv_Result;
*/
//--------------------------------------------------------------------
__u32		GrDumyCalcChksumDw( void* A_PtrBuf, __u32 A_Size, __u32 A_PrevChksum )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_LastSize;
		__u32*	Tv_PtrWk;
	// code --------------------
		//init
		Tv_Result		=	A_PrevChksum;
		Tv_LastSize	=	A_Size;
		Tv_PtrWk		=	(__u32*)A_PtrBuf;
		
		while ( 4 <= Tv_LastSize )
		{
			Tv_Result	=	Tv_Result + *Tv_PtrWk;
			//next
			Tv_PtrWk ++;
			Tv_LastSize		=	Tv_LastSize - 4;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrDumyCalcChksumBt( void* A_PtrBuf, __u32 A_Size, __u32 A_PrevChksum )
{
	// local -------------------
		__u32		Tv_Result;
		__u8*		Tv_PtrWk;
		__u32		Tv_Remain;
	// code --------------------
		Tv_Result	=	A_PrevChksum;
		Tv_PtrWk	=	(__u8*)A_PtrBuf;
		Tv_Remain	=	A_Size;
		while ( 0 != Tv_Remain )
		{
			Tv_Result	=	Tv_Result + (__u32)(*Tv_PtrWk);
			// next
			Tv_PtrWk ++;
			Tv_Remain --;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
/*
inline	__u32		GrDumyDwGetLastBitIdx( __u32 A_Mask )
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
*/

/*
// local -------------------
__u32				Tv_Result;
__s32					Tv_WkIdx;
__u32				Tv_Mask;
// code --------------------
//init
Tv_Result		=	E_GrDumyNotFoundIdx;
Tv_Mask			=	1 << 31;
//find
for (Tv_WkIdx = 31;Tv_WkIdx >= 0;Tv_WkIdx --)
{
//check exist
if ( 0 != (Tv_Mask & A_Mask) ) 
{
//found
Tv_Result		=	(__u32)Tv_WkIdx;
break;
}
//next
Tv_Mask		=	Tv_Mask >> 1;
}

return	Tv_Result;
*/

//--------------------------------------------------------------------
/*
inline	void*		GrDumyFillD( void* A_PtrBuf, __u32 A_DwCnt, __u32 A_Val )
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
				stosd
				mov		eax , edi
			}
		#endif
}
*/
//--------------------------------------------------------------------
/*
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
				stosw
				mov		eax , edi
		}
	#endif
}
*/
//--------------------------------------------------------------------
/*
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
			stosb
			mov		eax , edi
		}
	#endif
}
*/
//--------------------------------------------------------------------
void	GrDumySwapByte1Bit( void* A_PtrBuf, __u32 A_ByteCnt )
{
	// local -------------------
		__u8*	Tv_PtrWk;
		__u32	Tv_WkIdx;
		__u8	Tv_Tmp;
		__u32	Tv_WkBit;
	// code --------------------
		Tv_PtrWk	=	(__u8*)A_PtrBuf;
		for ( Tv_WkIdx=0;Tv_WkIdx < A_ByteCnt;Tv_WkIdx++ )
		{
			Tv_Tmp			=	*Tv_PtrWk;
			*Tv_PtrWk		=	0;
			for (Tv_WkBit=0;Tv_WkBit < 8;Tv_WkBit++)
			{
				*Tv_PtrWk	=	*Tv_PtrWk | ((Tv_Tmp & 1) << (7 - Tv_WkBit));
				//next
				Tv_Tmp	=	Tv_Tmp >> 1;
			}
			//next
			Tv_PtrWk ++;
		}
}
//--------------------------------------------------------------------
void	GrDumySwapByte4Bit( void* A_PtrBuf, __u32 A_ByteCnt )
{
	// local -------------------
		__u8*	Tv_PtrWk;
		__u32	Tv_WkIdx;
		__u8	Tv_Tmp;
	// code --------------------
		Tv_PtrWk	=	(__u8*)A_PtrBuf;
		for ( Tv_WkIdx=0;Tv_WkIdx < A_ByteCnt;Tv_WkIdx++ )
		{
			Tv_Tmp			=	*Tv_PtrWk;
			*Tv_PtrWk		=	((Tv_Tmp & 0x0F) << 4) | ((Tv_Tmp & 0xF0) >> 4);
			//next
			Tv_PtrWk ++;
		}
}
//--------------------------------------------------------------------
#ifdef GRACC_DUMYTOOL_NONE
/*
	none accelate code start
*/ 

__u32	GrDumyDwGetZeroBitCntRight( __u32 A_Val )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_WkIdx;
		__u32		Tv_Val;
	// code --------------------
		//init
		Tv_Result		=	0;
		Tv_Val			=	A_Val;

		for ( Tv_WkIdx=0;Tv_WkIdx < 32;Tv_WkIdx ++ )
		{
			if ( 0 != (Tv_Val & 1) )
			{
				//found
				break;
			}
			//next
			Tv_Val		=	Tv_Val >> 1;
			Tv_Result ++;
		}

		return	Tv_Result;
}
__u32	GrDumyDwGetLastBitIdx( __u32 A_Mask )
{
	// local -------------------
		__u32				Tv_Result;
		__s32					Tv_WkIdx;
		__u32				Tv_Mask;
	// code --------------------
		//init
		Tv_Result		=	E_GrDumyNotFoundIdx;
		Tv_Mask			=	1 << 31;
		//find
		for (Tv_WkIdx = 31;Tv_WkIdx >= 0;Tv_WkIdx --)
		{
			//check exist
			if ( 0 != (Tv_Mask & A_Mask) ) 
			{
				//found
				Tv_Result		=	(__u32)Tv_WkIdx;
				break;
			}
			//next
			Tv_Mask		=	Tv_Mask >> 1;
		}

		return	Tv_Result;
}
void*	GrDumyFillD( void* A_PtrBuf, __u32 A_DwCnt, __u32 A_Val )
{
	// local -------------------
		__u32		Tv_WkCnt;
		__u32*	Tv_PtrTg;
	// code --------------------
		Tv_PtrTg	=	(__u32*)A_PtrBuf;
		Tv_WkCnt	=	A_DwCnt;
		while ( 0 != Tv_WkCnt )
		{
			*Tv_PtrTg		=	A_Val;
			//next
			Tv_PtrTg ++;
			Tv_WkCnt --;
		}
		return	Tv_PtrTg;
}
void*	GrDumyFillW( void* A_PtrBuf, __u32 A_WdCnt, __u16 A_Val )
{
	// local -------------------
		__u32		Tv_WkCnt;
		__u16*		Tv_PtrTg;
	// code --------------------
		Tv_PtrTg	=	(__u16*)A_PtrBuf;
		Tv_WkCnt	=	A_WdCnt;
		while ( 0 != Tv_WkCnt )
		{
			*Tv_PtrTg		=	A_Val;
			//next
			Tv_PtrTg ++;
			Tv_WkCnt --;
		}
		return	Tv_PtrTg;
}
void*	GrDumyFillB( void* A_PtrBuf, __u32 A_BtCnt, __u8 A_Val )
{
	// local -------------------
		__u32		Tv_WkCnt;
		__u8*		Tv_PtrTg;
	// code --------------------
		Tv_PtrTg	=	(__u8*)A_PtrBuf;
		Tv_WkCnt	=	A_BtCnt;
		while ( 0 != Tv_WkCnt )
		{
			*Tv_PtrTg		=	A_Val;
			//next
			Tv_PtrTg ++;
			Tv_WkCnt --;
		}
		return	Tv_PtrTg;
}

/*
none accelate code finish
*/ 

#endif
//--------------------------------------------------------------------
BOOL8		GrDumyCmpDwUnit( void* A_PtrSr, void* A_PtrTg, __u32 A_DwCnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkCnt;
		__u32*	Tv_PtrSr;
		__u32*	Tv_PtrTg;
	// code --------------------
		Tv_Result	=	TRUE;

		Tv_PtrSr	=	(__u32*)A_PtrSr;
		Tv_PtrTg	=	(__u32*)A_PtrTg;
		for ( Tv_WkCnt=A_DwCnt;0 != Tv_WkCnt;Tv_WkCnt-- )
		{
			if ( (*Tv_PtrSr) != (*Tv_PtrTg) )
			{
				//differente
				Tv_Result	=	FALSE;
				break;
			}
			//next
			Tv_PtrSr ++;
			Tv_PtrTg ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrDumyEndian4byteSet( void* A_PtrTg, __u32 A_Val )
{
	// local -------------------
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)&A_Val;
		Tv_PtrTg	=	(__u8*)A_PtrTg;

		Tv_PtrTg[3]	=	Tv_PtrSr[0];
		Tv_PtrTg[2]	=	Tv_PtrSr[1];
		Tv_PtrTg[1]	=	Tv_PtrSr[2];
		Tv_PtrTg[0]	=	Tv_PtrSr[3];
}
//--------------------------------------------------------------------
void	GrDumyEndian3byteSet( void* A_PtrTg, __u32 A_Val )
{
	// local -------------------
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)&A_Val;
		Tv_PtrTg	=	(__u8*)A_PtrTg;

		Tv_PtrTg[2]	=	Tv_PtrSr[0];
		Tv_PtrTg[1]	=	Tv_PtrSr[1];
		Tv_PtrTg[0]	=	Tv_PtrSr[2];
}
//--------------------------------------------------------------------
void	GrDumyEndian2byteSet( void* A_PtrTg, __u16 A_Val )
{
	// local -------------------
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)&A_Val;
		Tv_PtrTg	=	(__u8*)A_PtrTg;

		Tv_PtrTg[1]	=	Tv_PtrSr[0];
		Tv_PtrTg[0]	=	Tv_PtrSr[1];
}
//--------------------------------------------------------------------
__u64	GrDumyEndian8byteGet(void* A_PtrSr)
{
	// local -------------------
		__u64	Tv_Result;
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)A_PtrSr;
		Tv_PtrTg	=	(__u8*)&Tv_Result;

		Tv_PtrTg[7]	=	Tv_PtrSr[0];
		Tv_PtrTg[6]	=	Tv_PtrSr[1];
		Tv_PtrTg[5]	=	Tv_PtrSr[2];
		Tv_PtrTg[4]	=	Tv_PtrSr[3];
		Tv_PtrTg[3]	=	Tv_PtrSr[4];
		Tv_PtrTg[2]	=	Tv_PtrSr[5];
		Tv_PtrTg[1]	=	Tv_PtrSr[6];
		Tv_PtrTg[0]	=	Tv_PtrSr[7];

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrDumyEndian4byteGet( void* A_PtrSr )
{
	// local -------------------
		__u32	Tv_Result;
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)A_PtrSr;
		Tv_PtrTg	=	(__u8*)&Tv_Result;

		Tv_PtrTg[3]	=	Tv_PtrSr[0];
		Tv_PtrTg[2]	=	Tv_PtrSr[1];
		Tv_PtrTg[1]	=	Tv_PtrSr[2];
		Tv_PtrTg[0]	=	Tv_PtrSr[3];

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrDumyEndian3byteGet( void* A_PtrSr )
{
	// local -------------------
		__u32	Tv_Result;
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)A_PtrSr;
		Tv_PtrTg	=	(__u8*)&Tv_Result;

		Tv_PtrTg[3]	=	0;
		Tv_PtrTg[2]	=	Tv_PtrSr[0];
		Tv_PtrTg[1]	=	Tv_PtrSr[1];
		Tv_PtrTg[0]	=	Tv_PtrSr[2];

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	GrDumyEndian2byteGet( void* A_PtrSr )
{
	// local -------------------
		__u16	Tv_Result;
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)A_PtrSr;
		Tv_PtrTg	=	(__u8*)&Tv_Result;

		Tv_PtrTg[1]	=	Tv_PtrSr[0];
		Tv_PtrTg[0]	=	Tv_PtrSr[1];

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrDumyWrite4Byte( void* A_PtrTg, __u32 A_Val )
{
	// local -------------------
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)&A_Val;
		Tv_PtrTg	=	(__u8*)A_PtrTg;

		Tv_PtrTg[0]	=	Tv_PtrSr[0];
		Tv_PtrTg[1]	=	Tv_PtrSr[1];
		Tv_PtrTg[2]	=	Tv_PtrSr[2];
		Tv_PtrTg[3]	=	Tv_PtrSr[3];
}
//--------------------------------------------------------------------
void	GrDumyWrite3Byte( void* A_PtrTg, __u32 A_Val )
{
	// local -------------------
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)&A_Val;
		Tv_PtrTg	=	(__u8*)A_PtrTg;

		Tv_PtrTg[0]	=	Tv_PtrSr[0];
		Tv_PtrTg[1]	=	Tv_PtrSr[1];
		Tv_PtrTg[2]	=	Tv_PtrSr[2];
}
//--------------------------------------------------------------------
void	GrDumyWrite2Byte( void* A_PtrTg, __u16 A_Val )
{
	// local -------------------
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)&A_Val;
		Tv_PtrTg	=	(__u8*)A_PtrTg;

		Tv_PtrTg[0]	=	Tv_PtrSr[0];
		Tv_PtrTg[1]	=	Tv_PtrSr[1];
}
//--------------------------------------------------------------------
__u32	GrDumyRead4Byte( void* A_PtrSr )
{
	// local -------------------
		__u32	Tv_Result;
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)A_PtrSr;
		Tv_PtrTg	=	(__u8*)&Tv_Result;

		Tv_PtrTg[3]	=	Tv_PtrSr[3];
		Tv_PtrTg[2]	=	Tv_PtrSr[2];
		Tv_PtrTg[1]	=	Tv_PtrSr[1];
		Tv_PtrTg[0]	=	Tv_PtrSr[0];

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrDumyRead3Byte( void* A_PtrSr )
{
	// local -------------------
		__u32	Tv_Result;
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)A_PtrSr;
		Tv_PtrTg	=	(__u8*)&Tv_Result;

		Tv_PtrTg[3]	=	0;
		Tv_PtrTg[2]	=	Tv_PtrSr[2];
		Tv_PtrTg[1]	=	Tv_PtrSr[1];
		Tv_PtrTg[0]	=	Tv_PtrSr[0];

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	GrDumyRead2Byte( void* A_PtrSr )
{
	// local -------------------
		__u16	Tv_Result;
		__u8*	volatile	Tv_PtrSr;
		__u8*	volatile	Tv_PtrTg;
	// code --------------------
		Tv_PtrSr	=	(__u8*)A_PtrSr;
		Tv_PtrTg	=	(__u8*)&Tv_Result;

		Tv_PtrTg[1]	=	Tv_PtrSr[1];
		Tv_PtrTg[0]	=	Tv_PtrSr[0];

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrDumyEndianTrsBuf( void* A_PtrTg, void* A_PtrSr, __u32 A_Size )
{
	// local -------------------
		__u8*	Tv_PtrSr;
		__u8*	Tv_PtrTg;
		__u32	Tv_Remain;
	// code --------------------
		Tv_Remain	=	A_Size;
		Tv_PtrSr	=	(__u8*)A_PtrSr;
		Tv_PtrTg	=	(__u8*)A_PtrTg;
		while ( 2 <= Tv_Remain ) 
		{
			// swap
			Tv_PtrTg[0]	=	Tv_PtrSr[1];
			Tv_PtrTg[1]	=	Tv_PtrSr[0];
			// next
			Tv_PtrSr	=	Tv_PtrSr + 2;
			Tv_PtrTg	=	Tv_PtrTg + 2;
			Tv_Remain	=	Tv_Remain - 2;
		}
		
}
//--------------------------------------------------------------------
void	GrDumyEndianTrsDwBuf( void* A_PtrTg, void* A_PtrSr, __u32 A_Size )
{
	// local -------------------
		__u8*	Tv_PtrSr;
		__u8*	Tv_PtrTg;
		__u32	Tv_Remain;
		__u8	Tv_Tmp0;
		__u8	Tv_Tmp1;
	// code --------------------
		Tv_Remain	=	A_Size;
		Tv_PtrSr	=	(__u8*)A_PtrSr;
		Tv_PtrTg	=	(__u8*)A_PtrTg;
		while ( 4 <= Tv_Remain ) 
		{
			// swap
			Tv_Tmp0			=	Tv_PtrSr[0];
			Tv_Tmp1			=	Tv_PtrSr[1];
			Tv_PtrTg[1]	=	Tv_PtrSr[2];
			Tv_PtrTg[0]	=	Tv_PtrSr[3];
			Tv_PtrTg[2]	=	Tv_Tmp1;
			Tv_PtrTg[3]	=	Tv_Tmp0;

			// next
			Tv_PtrSr	=	Tv_PtrSr + 4;
			Tv_PtrTg	=	Tv_PtrTg + 4;
			Tv_Remain	=	Tv_Remain - 4;
		}
}
//--------------------------------------------------------------------
BOOL8	GrDumyAbandoneByDw(void* A_PtrBuf, __u32 A_BufSize, __u32 A_Basis, __u32* A_PtrRtSize)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8*	Tv_PtrBasis;
		__u8*	Tv_PtrBuf;
		__u32	Tv_BufIdx;
		__u32	Tv_RtSize;
	// code --------------------
		
		// check size
		if(A_BufSize < 4)
		{
			*A_PtrRtSize	=	A_BufSize;
			return	FALSE;
		}

		// init
		Tv_Result		=	FALSE;
		Tv_PtrBasis	=	(__u8*)&A_Basis;
		Tv_PtrBuf		=	(__u8*)A_PtrBuf;

		Tv_BufIdx		=	0;
		// find data
		while((Tv_BufIdx + 3) < A_BufSize)
		{
			if(Tv_PtrBasis[0] == Tv_PtrBuf[Tv_BufIdx])
			{
				// check remaining data
				if((Tv_PtrBasis[1] == Tv_PtrBuf[Tv_BufIdx + 1]) && (Tv_PtrBasis[2] == Tv_PtrBuf[Tv_BufIdx + 2]) && (Tv_PtrBasis[3] == Tv_PtrBuf[Tv_BufIdx + 3]))
				{
					// found
					Tv_Result	=	TRUE;
				}
			}

			// next
			Tv_BufIdx ++;
		}

		// abondone data
		Tv_RtSize	=	A_BufSize - Tv_BufIdx;
		if(0 != Tv_BufIdx)
		{
			GrDumyMoveMem(A_PtrBuf, &Tv_PtrBuf[Tv_BufIdx], Tv_RtSize);
		}
		*A_PtrRtSize	=	Tv_RtSize;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	GrDumyBitRvsU8(__u8 A_Src)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_Src;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	0;
		Tv_Src		=	A_Src;
		for( Tv_WkIdx = 0; Tv_WkIdx < 8; Tv_WkIdx++)
		{
			Tv_Result	=	Tv_Result << 1;
			Tv_Result	=	Tv_Result | (Tv_Src & 1);
			Tv_Src		=	Tv_Src >> 1;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrDumyDwBitReverse(__u32 A_Src, __u32 A_BitPos, __u32 A_BitCnt, __u32 A_Dst, __u32 A_BitDst)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_WkIdx;
		__u32	Tv_SrMsk;
		__u32	Tv_TgMsk;
	// code --------------------
		Tv_Result	=	A_Dst;

		if(0 != A_BitCnt)
		{
			Tv_SrMsk		=	1 << A_BitPos;
			Tv_TgMsk		=	1 << (A_BitDst + A_BitCnt - 1);
			for( Tv_WkIdx = 0; Tv_WkIdx < A_BitCnt; Tv_WkIdx++)
			{
				if(0 != (A_Src & Tv_SrMsk))
				{
					Tv_Result	=	Tv_Result | Tv_TgMsk;
				}
				else
				{
					Tv_Result	=	Tv_Result & (~Tv_TgMsk);
				}
				// next
				Tv_SrMsk	=	Tv_SrMsk << 1;
				Tv_TgMsk	=	Tv_TgMsk >> 1;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrDumyBase64EncInit(Ptr_GrDumyBase64Enc A_PtrEnv)
{
		A_PtrEnv->RmSize	=	0;
}
//--------------------------------------------------------------------
__u32	GrDumyBase64EncAdd(Ptr_GrDumyBase64Enc A_PtrEnv, void* A_PtrSr, __u32 A_SrSize, void* A_PtrOut)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_Remain;
		__u8*	Tv_PtrSr;
		char*	Tv_PtrEc;
		__u8	Tv_EncIdx;
	// code --------------------
		Tv_Result	=	0;
		Tv_Remain	=	A_SrSize;

		Tv_PtrSr	=	(__u8*)A_PtrSr;
		Tv_PtrEc	=	(char*)A_PtrOut;

		// encode first
		if(0 != A_PtrEnv->RmSize)
		{
			while(0 != Tv_Remain)
			{
				// get one soruce
				A_PtrEnv->RmDat[A_PtrEnv->RmSize]	=	*Tv_PtrSr;
				// next
				Tv_PtrSr ++;
				Tv_Remain --;
				A_PtrEnv->RmSize ++;
				if(3 <= A_PtrEnv->RmSize)
				{
					break;
				}
			}

			// check filled
			if(3 <= A_PtrEnv->RmSize)
			{
				Tv_EncIdx		=	(A_PtrEnv->RmDat[0] >> 2) & 0x3F;
				Tv_PtrEc[0]	=	V_GrStrToolBase64Tbl[Tv_EncIdx];

				Tv_EncIdx		=	((A_PtrEnv->RmDat[0] & 0x03) << 4) | ((A_PtrEnv->RmDat[1] & 0xF0) >> 4);
				Tv_PtrEc[1]	=	V_GrStrToolBase64Tbl[Tv_EncIdx];

				Tv_EncIdx		=	((A_PtrEnv->RmDat[1] & 0x0F) << 2) | ((A_PtrEnv->RmDat[2] & 0xC0) >> 6);
				Tv_PtrEc[2]	=	V_GrStrToolBase64Tbl[Tv_EncIdx];

				Tv_EncIdx		=	(A_PtrEnv->RmDat[2] & 0x3F);
				Tv_PtrEc[3]	=	V_GrStrToolBase64Tbl[Tv_EncIdx];

				Tv_PtrEc		=	Tv_PtrEc + 4;
				Tv_Result		=	Tv_Result + 4;

				A_PtrEnv->RmSize	=	0;
			}
		}

		// encode main stream
		while(3 <= Tv_Remain)
		{
			// do encode
			Tv_EncIdx		=	(Tv_PtrSr[0] >> 2) & 0x3F;
			Tv_PtrEc[0]	=	V_GrStrToolBase64Tbl[Tv_EncIdx];

			Tv_EncIdx		=	((Tv_PtrSr[0] & 0x03) << 4) | ((Tv_PtrSr[1] & 0xF0) >> 4);
			Tv_PtrEc[1]	=	V_GrStrToolBase64Tbl[Tv_EncIdx];

			Tv_EncIdx		=	((Tv_PtrSr[1] & 0x0F) << 2) | ((Tv_PtrSr[2] & 0xC0) >> 6);
			Tv_PtrEc[2]	=	V_GrStrToolBase64Tbl[Tv_EncIdx];

			Tv_EncIdx		=	(Tv_PtrSr[2] & 0x3F);
			Tv_PtrEc[3]	=	V_GrStrToolBase64Tbl[Tv_EncIdx];

			Tv_PtrSr		=	Tv_PtrSr + 3;
			Tv_Remain		=	Tv_Remain - 3;
			Tv_PtrEc		=	Tv_PtrEc + 4;
			Tv_Result		=	Tv_Result + 4;

		}

		// remain soruce process
		while(0 != Tv_Remain)
		{
			A_PtrEnv->RmDat[A_PtrEnv->RmSize]	=	*Tv_PtrSr;
			Tv_PtrSr ++;
			Tv_Remain --;
			A_PtrEnv->RmSize ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrDumyBase64EncFinish(Ptr_GrDumyBase64Enc A_PtrEnv, void* A_PtrOut)
{
	// local -------------------
		char*	Tv_PtrEc;
		__u8	Tv_EncIdx;
	// code --------------------
		// init
		Tv_PtrEc	=	(char*)A_PtrOut;
		// check exist remain
		if(0 == A_PtrEnv->RmSize)
		{
			Tv_PtrEc[0]	=	0;
			return	0;
		}

		Tv_EncIdx		=	(A_PtrEnv->RmDat[0] >> 2) & 0x3F;
		Tv_PtrEc[0]	=	V_GrStrToolBase64Tbl[Tv_EncIdx];

		if(1 == A_PtrEnv->RmSize)
		{

			Tv_EncIdx		=	((A_PtrEnv->RmDat[0] & 0x03) << 4);
			Tv_PtrEc[1]	=	V_GrStrToolBase64Tbl[Tv_EncIdx];

			Tv_PtrEc[2]	=	'=';
			Tv_PtrEc[3]	=	'=';

		}
		else
		{

			Tv_EncIdx		=	((A_PtrEnv->RmDat[0] & 0x03) << 4) | ((A_PtrEnv->RmDat[1] & 0xF0) >> 4);
			Tv_PtrEc[1]	=	V_GrStrToolBase64Tbl[Tv_EncIdx];

			Tv_EncIdx		=	((A_PtrEnv->RmDat[1] & 0x0F) << 2);
			Tv_PtrEc[2]	=	V_GrStrToolBase64Tbl[Tv_EncIdx];

			Tv_PtrEc[3]	=	'=';
		}

		Tv_PtrEc[4]	=	0;		// mark end of text
		A_PtrEnv->RmSize	=	0;
		return	4;

}
//--------------------------------------------------------------------
void	GrDumyBase64DecInit(Ptr_GrDumyBase64Dec A_PtrEnv)
{
		A_PtrEnv->RmSize	=	0;
}
//--------------------------------------------------------------------
__u8	GrDumyBase64CharToCode(char A_Char)
{
	// local -------------------
	// code --------------------
		if((A_Char >= 'A') && (A_Char <= 'Z'))
		{
			return	(__u8)A_Char - (__u8)'A';
		}
		else if((A_Char >= 'a') && (A_Char <= 'z'))
		{
			return	26 + (__u8)A_Char - (__u8)'a';
		}
		else if((A_Char >= '0') && (A_Char <= '9'))
		{
			return	52 + (__u8)A_Char - (__u8)'0';
		}
		else if(A_Char == '+')
		{
			return	62;
		}
		else if(A_Char == '/')
		{
			return	63;
		}

		return	0xFF;
}
//--------------------------------------------------------------------
__u32	GrDumyBase64DecDo(Ptr_GrDumyBase64Dec A_PtrEnv, void* A_PtrSr, __u32 A_SrSize, void* A_PtrOut)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_Remain;
		char*	Tv_PtrSr;
		__u8*	Tv_PtrDc;
	// code --------------------
		Tv_Result	=	0;
		Tv_Remain	=	A_SrSize;

		Tv_PtrSr	=	(char*)A_PtrSr;
		Tv_PtrDc	=	(__u8*)A_PtrOut;

		// check remain exist
		if(0 != A_PtrEnv->RmSize)
		{
			while(0 != Tv_Remain)
			{
				// get one soruce
				A_PtrEnv->RmDat[A_PtrEnv->RmSize]	=	GrDumyBase64CharToCode(*Tv_PtrSr);
				// next
				Tv_PtrSr ++;
				Tv_Remain --;
				A_PtrEnv->RmSize ++;
				if(4 <= A_PtrEnv->RmSize)
				{
					break;
				}
			}

			// check filled
			if(4 <= A_PtrEnv->RmSize)
			{
				// check source size
				if(0xFF == A_PtrEnv->RmDat[2])
				{
					Tv_PtrDc[0]	=	A_PtrEnv->RmDat[0] | (A_PtrEnv->RmDat[1] << 6);

					Tv_PtrDc ++;
					Tv_Result	++;
				}
				else if(0xFF == A_PtrEnv->RmDat[3])
				{
					Tv_PtrDc[0]	=	A_PtrEnv->RmDat[0] | ((A_PtrEnv->RmDat[1] & 3) << 6);
					Tv_PtrDc[1]	=	(A_PtrEnv->RmDat[1] >> 2) | ((A_PtrEnv->RmDat[2] & 0x0F) << 4);

					Tv_PtrDc	=	Tv_PtrDc + 2;
					Tv_Result	=	Tv_Result + 2;
				}
				else
				{
					Tv_PtrDc[0]	=	A_PtrEnv->RmDat[0] | ((A_PtrEnv->RmDat[1] & 3) << 6);
					Tv_PtrDc[1]	=	(A_PtrEnv->RmDat[1] >> 2) | ((A_PtrEnv->RmDat[2] & 0x0F) << 4);
					Tv_PtrDc[2]	=	(A_PtrEnv->RmDat[2] >> 4) | (A_PtrEnv->RmDat[3] << 2);

					Tv_PtrDc	=	Tv_PtrDc + 3;
					Tv_Result	=	Tv_Result + 3;
				}

				A_PtrEnv->RmSize	=	0;
			}
		}

		// do main stream
		while(4 <= Tv_Remain)
		{
			// do decode
			A_PtrEnv->RmDat[0]	=	GrDumyBase64CharToCode(Tv_PtrSr[0]);
			A_PtrEnv->RmDat[1]	=	GrDumyBase64CharToCode(Tv_PtrSr[1]);
			A_PtrEnv->RmDat[2]	=	GrDumyBase64CharToCode(Tv_PtrSr[2]);
			A_PtrEnv->RmDat[3]	=	GrDumyBase64CharToCode(Tv_PtrSr[3]);

			// check source size
			if(0xFF == A_PtrEnv->RmDat[2])
			{
				Tv_PtrDc[0]	=	A_PtrEnv->RmDat[0] | (A_PtrEnv->RmDat[1] << 6);

				Tv_PtrDc ++;
				Tv_Result	++;
			}
			else if(0xFF == A_PtrEnv->RmDat[3])
			{
				Tv_PtrDc[0]	=	A_PtrEnv->RmDat[0] | ((A_PtrEnv->RmDat[1] & 3) << 6);
				Tv_PtrDc[1]	=	(A_PtrEnv->RmDat[1] >> 2) | ((A_PtrEnv->RmDat[2] & 0x0F) << 4);

				Tv_PtrDc	=	Tv_PtrDc + 2;
				Tv_Result	=	Tv_Result + 2;
			}
			else
			{
				Tv_PtrDc[0]	=	A_PtrEnv->RmDat[0] | ((A_PtrEnv->RmDat[1] & 3) << 6);
				Tv_PtrDc[1]	=	(A_PtrEnv->RmDat[1] >> 2) | ((A_PtrEnv->RmDat[2] & 0x0F) << 4);
				Tv_PtrDc[2]	=	(A_PtrEnv->RmDat[2] >> 4) | (A_PtrEnv->RmDat[3] << 2);

				Tv_PtrDc	=	Tv_PtrDc + 3;
				Tv_Result	=	Tv_Result + 3;
			}

			// next
			Tv_PtrSr		=	Tv_PtrSr + 4;
			Tv_Remain		=	Tv_Remain - 4;
		}

		// remain soruce process
		while(0 != Tv_Remain)
		{
			A_PtrEnv->RmDat[A_PtrEnv->RmSize]	=	GrDumyBase64CharToCode(*Tv_PtrSr);
			Tv_PtrSr ++;
			Tv_Remain --;
			A_PtrEnv->RmSize ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrDumyBase64DecFinish(Ptr_GrDumyBase64Dec A_PtrEnv)
{
		A_PtrEnv->RmSize	=	0;
}
//--------------------------------------------------------------------
