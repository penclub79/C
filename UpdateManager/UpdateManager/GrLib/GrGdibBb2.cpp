/*
	gdib 2 byte in bit

*/

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrGdibBb2.h>
#include	<GrImgBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrMath.h>
#include	<stdlib.h>

#include <FontV3/Dib2/GrFontV3Dib2Base.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_GrGdibBb2::Cls_GrGdibBb2( __u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
															__u32 A_Pitch, void* A_PtrDib, 
															__u32 A_PltBitCnt, void* A_PtrPlt ):
Cls_GrGdib( A_Fcc, A_ResX, A_ResY, A_Pitch, A_PtrDib, A_PltBitCnt, A_PtrPlt )
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
Cls_GrGdibBb2::~Cls_GrGdibBb2()
{
}
//--------------------------------------------------------------------
void		Cls_GrGdibBb2::ProcSetPixelNpt( __s32 A_X, __s32 A_Y, __u32 A_PxVal )
{
	// local -------------------
		__u16*	Tv_PtrPx;		
	// code --------------------
		//calculate address
		Tv_PtrPx	=	(__u16*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_Y) + (2 * (__u32)A_X) );
		//set pixel
		*Tv_PtrPx		=	(__u16)A_PxVal;
}
//--------------------------------------------------------------------
__u32		Cls_GrGdibBb2::ProcGetPixelNpt( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		__u16*		Tv_PtrPx;		
	// code --------------------
		//calculate address
		Tv_PtrPx	=	(__u16*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_Y) + (2 * (__u32)A_X) );
		//get pixel
		return	(__u32)( *Tv_PtrPx );
}
//--------------------------------------------------------------------
void		Cls_GrGdibBb2::ProcFillRectNpt( Ptr_GrRect A_PtrRect, __u32 A_PxVal )
{
	// local -------------------
#ifdef GR_ARM
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrWkY;
		__u32		Tv_Remain;

		__u16*	Tv_PtrPx;
		__u32		Tv_ValDw;
		__u32		Tv_Width;
		__u32		Tv_Height;

		__u32		Tv_Type;
		__u32		Tv_QdCnt;
		__u32		Tv_PitchAdd;

#else
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrWkY;
		__u16*	Tv_PtrPx;
		__u32*	Tv_PtrDw;
		__u64*	Tv_Ptr64;

		__u32		Tv_ValDw;
		__u64		Tv_Val64;
		BOOL8		Tv_IsSt1x;
		BOOL8		Tv_IsEd1x;
		BOOL8		Tv_IsSt2x;
		BOOL8		Tv_IsEd2x;
		__u32		Tv_Remain;
		__u32		Tv_4xCnt;
#endif
	// code --------------------

		//calcluate address
		Tv_PtrWkY	=	(void*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_PtrRect->top) + (2 * (__u32)A_PtrRect->left) );

#ifdef GR_ARM
		//work vertical
		Tv_Width	=	A_PtrRect->right - A_PtrRect->left;
		if(8 > Tv_Width)
		{
			for(Tv_WkY = (__u32)A_PtrRect->top; Tv_WkY < (__u32)A_PtrRect->bottom; Tv_WkY++)
			{
				//init
				Tv_PtrPx	=	(__u16*)Tv_PtrWkY;
				for(Tv_WkX = (__u32)A_PtrRect->left; Tv_WkX < (__u32)A_PtrRect->right; Tv_WkX++)
				{
					//write
					*Tv_PtrPx	=	(__u16)A_PxVal;
					//next
					Tv_PtrPx++;
				}

				//next
				Tv_PtrWkY	=	(void*)((__u32)Tv_PtrWkY + m_Gdib.Pitch);
			}
		}
		else
		{
			if(0 == A_PxVal)
			{
				// zero
				Tv_Remain	=	(__u32)(A_PtrRect->right - A_PtrRect->left) << 1;
				for(Tv_WkY = (__u32)A_PtrRect->top; Tv_WkY < (__u32)A_PtrRect->bottom; Tv_WkY++)
				{
					//init
					GrDumyZeroMem(Tv_PtrWkY, Tv_Remain);
					//next
					Tv_PtrWkY	=	(void*)((__u32)Tv_PtrWkY + m_Gdib.Pitch);
				}
			}
			else
			{
				// none zero
				// check type
				Tv_Type		=	0;		// bit 0: addr 2x , 1: addr 4x , 2:remain 4x, 3 : remain 2x
				Tv_Remain	=	Tv_Width << 1;

				if(0 != ((__u32)Tv_PtrWkY & 2))
				{
					Tv_Type		=	Tv_Type | 1;
					Tv_Remain	=	Tv_Remain - 2;
				}

				if(0 != ((__u32)Tv_PtrWkY & 4))
				{
					Tv_Type		=	Tv_Type | 2;
					Tv_Remain	=	Tv_Remain - 4;
				}

				Tv_QdCnt	=	Tv_Remain >> 3;

				if(0 != (Tv_Remain & 4))
				{
					Tv_Type		=	Tv_Type | 4;
				}

				if(0 != (Tv_Remain & 2))
				{
					Tv_Type		=	Tv_Type | 8;
				}

				Tv_Height		=	(__u32)(A_PtrRect->bottom - A_PtrRect->top);
				Tv_ValDw		=	(A_PxVal & 0xFFFF) | ((A_PxVal & 0xFFFF) << 16);
				Tv_PitchAdd	=	m_Gdib.Pitch - (Tv_Width << 1);

				switch(Tv_Type)
				{
					case 0:
						// quad only
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
							:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);
						break;
					case 1:
						// 2p
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"strh r5 , [r1], #2 \n\t"
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
						:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);

						break;
					case 2:
						// 4p
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"str r5 , [r1], #4 \n\t"
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
						:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);

						break;
					case 3:
						// 2p, 4p
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"strh r5 , [r1], #2 \n\t"
							"str r5 , [r1], #4 \n\t"
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
						:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);

						break;
					case 4:
						// 4c
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"str r5 , [r1], #4 \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
						:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);

						break;
					case 5:
						// 2p, 4c
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"strh r5 , [r1], #2 \n\t"
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"str r5 , [r1], #4 \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
						:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);

						break;
					case 6:
						// 4p, 4c
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"str r5 , [r1], #4 \n\t"
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"str r5 , [r1], #4 \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
						:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);

						break;
					case 7:
						// 2p, 4p, 4c
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"strh r5 , [r1], #2 \n\t"
							"str r5 , [r1], #4 \n\t"
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"str r5 , [r1], #4 \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
						:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);

						break;
					case 8:
						// 2c
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"strh r5 , [r1], #2 \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
						:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);

						break;
					case 9:
						// 2p, 2c
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"strh r5 , [r1], #2 \n\t"
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"strh r5 , [r1], #2 \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
						:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);

						break;
					case 10:
						// 4p, 2c
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"str r5 , [r1], #4 \n\t"
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"strh r5 , [r1], #2 \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
						:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);

						break;
					case 11:
						// 2p, 4p, 2c
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"strh r5 , [r1], #2 \n\t"
							"str r5 , [r1], #4 \n\t"
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"strh r5 , [r1], #2 \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
						:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);

						break;
					case 12:
						// 4c, 2c
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"str r5 , [r1], #4 \n\t"
							"strh r5 , [r1], #2 \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
							:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);

						break;
					case 13:
						// 2p, , 4c, 2c
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"strh r5 , [r1], #2 \n\t"
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"str r5 , [r1], #4 \n\t"
							"strh r5 , [r1], #2 \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
							:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);
						break;
					case 14:
						// 4p, 4c, 2c
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"str r5 , [r1], #4 \n\t"
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"str r5 , [r1], #4 \n\t"
							"strh r5 , [r1], #2 \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
							:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
							);
						break;
					case 15:
						// all
						/* inline assembly
						r1 : pointer
						r2 : y counter
						r3 : quad count
						r4 : pitch adder
						r5 : pixel value
						r6 : pixel value 2
						r7 : quad progress
						*/

						__asm__ __volatile__("ldr r1, %0 \n\t"
							"ldr r2 , %1 \n\t"
							"ldr r3 , %2 \n\t"
							"ldr r4 , %3 \n\t"
							"ldr r5 , %4 \n\t"
							"mov r6 , r5 \n\t"
							"1: \n\t"	/* y loop */
							"strh r5 , [r1], #2 \n\t"
							"str r5 , [r1], #4 \n\t"
							"mov r7, r3 \n\t"
							"2: \n\t"	/* quad loop */
							"stmea r1!, {r5, r6}  \n\t"
							"subs r7 , r7, #1 \n\t"
							"bne 2b \n\t"
							"str r5 , [r1], #4 \n\t"
							"strh r5 , [r1], #2 \n\t"
							"add r1, r1 , r4 \n\t"
							"subs r2 , r2, #1 \n\t"
							"bne 1b \n\t"
							:
							:"m"(Tv_PtrWkY), "m"(Tv_Height), "m"(Tv_QdCnt), "m"(Tv_PitchAdd), "m"(Tv_ValDw)
							:"cc", "r1", "r2", "r3", "r4", "r5" , "r6" , "r7"
							);
						break;
				}

			}	//	if(0 == A_PxVal)
		}	// if(8 > Tv_Width)

#else

		if(8 > (A_PtrRect->right - A_PtrRect->left))
		{
			//work vertical
			for(Tv_WkY = (__u32)A_PtrRect->top; Tv_WkY < (__u32)A_PtrRect->bottom; Tv_WkY++)
			{
				//init
				Tv_PtrPx	=	(__u16*)Tv_PtrWkY;
				for(Tv_WkX = (__u32)A_PtrRect->left; Tv_WkX < (__u32)A_PtrRect->right; Tv_WkX++)
				{
					//write
					*Tv_PtrPx	=	(__u16)A_PxVal;
					//next
					Tv_PtrPx++;
				}

				//next
				Tv_PtrWkY	=	(void*)((__u32)Tv_PtrWkY + m_Gdib.Pitch);
			}
		}	//	if(16 > (A_PtrRect->right - A_PtrRect->left))
		else
		{
			// bigger then 16 pixel
			// check zero
			if(0 == A_PxVal)
			{
				Tv_Remain	=	(__u32)(A_PtrRect->right - A_PtrRect->left) << 1;
				for(Tv_WkY = (__u32)A_PtrRect->top; Tv_WkY < (__u32)A_PtrRect->bottom; Tv_WkY++)
				{
					//init
					GrDumyZeroMem(Tv_PtrWkY, Tv_Remain);
					//next
					Tv_PtrWkY	=	(void*)((__u32)Tv_PtrWkY + m_Gdib.Pitch);
				}
			}
			else
			{
				// other type cpu
				Tv_ValDw	=	(A_PxVal & 0xFFFF) | ((A_PxVal & 0xFFFF) << 16);
				Tv_Val64	=	((__u64)Tv_ValDw << 32) | (__u64)(Tv_ValDw);
				Tv_Remain	=	(__u32)(A_PtrRect->right - A_PtrRect->left);
				Tv_IsSt1x	=	FALSE;
				Tv_IsEd1x	=	FALSE;
				Tv_IsSt2x	=	FALSE;
				Tv_IsEd2x	=	FALSE;
				if(0 != (1 & A_PtrRect->left))
				{
					Tv_IsSt1x	=	TRUE;
					Tv_Remain --;
				}
				if(0 != (1 & Tv_Remain))
				{
					Tv_IsEd1x	=	TRUE;
					Tv_Remain --;
				}
				if(0 != (2 & A_PtrRect->left))
				{
					Tv_IsSt2x	=	TRUE;
					Tv_Remain	=	Tv_Remain - 2;
				}
				if(0 != (2 & Tv_Remain))
				{
					Tv_IsEd2x	=	TRUE;
					Tv_Remain	=	Tv_Remain - 2;
				}

				Tv_4xCnt	=	Tv_Remain >> 2;

				for(Tv_WkY = (__u32)A_PtrRect->top; Tv_WkY < (__u32)A_PtrRect->bottom; Tv_WkY++)
				{
					//init
					Tv_PtrPx	=	(__u16*)Tv_PtrWkY;
					if(Tv_IsSt1x)
					{
						*Tv_PtrPx	=	(__u16)Tv_ValDw;
						Tv_PtrPx ++;
					}

					if(Tv_IsSt2x)
					{
						Tv_PtrDw	=	(__u32*)Tv_PtrPx;
						*Tv_PtrDw	=	Tv_ValDw;
						Tv_PtrPx	=	Tv_PtrPx + 2;
					}

					Tv_Ptr64	=	(__u64*)Tv_PtrPx;
					for( Tv_WkX = 0; Tv_WkX < Tv_4xCnt; Tv_WkX++)
					{
						// write
						*Tv_Ptr64	=	Tv_Val64;
						// next
						Tv_Ptr64 ++;
					}

					Tv_PtrPx	=	(__u16*)Tv_Ptr64;

					if(Tv_IsEd2x)
					{
						Tv_PtrDw	=	(__u32*)Tv_PtrPx;
						*Tv_PtrDw		=	Tv_ValDw;
						Tv_PtrPx		=	Tv_PtrPx + 2;
					}

					if(Tv_IsEd1x)
					{
						*Tv_PtrPx	=	(__u16)Tv_ValDw;
						Tv_PtrPx ++;
					}

					//next
					Tv_PtrWkY	=	(void*)((__u32)Tv_PtrWkY + m_Gdib.Pitch);
				}
			}	// else .. if(0 == A_PxVal)
		}
#endif
}
//--------------------------------------------------------------------
void		Cls_GrGdibBb2::ProcBltNrmNpt2Npt( __s32 A_X, __s32 A_Y,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;
		__u16*		Tv_PtrSrP;
		__u16*		Tv_PtrTgP;
		__u32		Tv_Width;
	// code --------------------
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													(A_PtrGdibSr->Pitch * A_PtrRectSr->top) + 
													( 2 * (__u32)A_PtrRectSr->left) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( 2 * A_X ) );

		Tv_Width	=	(__u32)(A_PtrRectSr->right - A_PtrRectSr->left) * 2;
		if(32 < Tv_Width)
		{
			for(Tv_WkY =(__u32)A_PtrRectSr->top; Tv_WkY < (__u32)A_PtrRectSr->bottom; Tv_WkY++)
			{
				//init
				GrDumyCopyMem(Tv_PtrTgY, Tv_PtrSrY, Tv_Width);
				//next
				Tv_PtrSrY	=	(void*)((__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch);
				Tv_PtrTgY	=	(void*)((__u32)Tv_PtrTgY + m_Gdib.Pitch);
			}
		}
		else
		{
			//work y
			for(Tv_WkY =(__u32)A_PtrRectSr->top; Tv_WkY < (__u32)A_PtrRectSr->bottom; Tv_WkY++)
			{
				//init
				Tv_PtrSrP			=	(__u16*)Tv_PtrSrY;
				Tv_PtrTgP			=	(__u16*)Tv_PtrTgY;
				for(Tv_WkX= (__u32)A_PtrRectSr->left; Tv_WkX < (__u32)A_PtrRectSr->right; Tv_WkX++)
				{
					//copy pixel
					*Tv_PtrTgP	=	*Tv_PtrSrP;

					//next
					Tv_PtrSrP	++;
					Tv_PtrTgP	++;
				}
				//next
				Tv_PtrSrY	=	(void*)((__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch);
				Tv_PtrTgY	=	(void*)((__u32)Tv_PtrTgY + m_Gdib.Pitch);
			}
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdibBb2::ProcBltSrkNpt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;
		__u16*		Tv_PtrSrP;
		__u16*		Tv_PtrTgP;
	// code --------------------
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													(A_PtrGdibSr->Pitch * A_PtrRectSr->top) + 
													( 2 * (__u32)A_PtrRectSr->left) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( 2 * A_X ) );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u16*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u16*)Tv_PtrTgY;
			for ( Tv_WkX= (__u32)A_PtrRectSr->left;Tv_WkX < (__u32)A_PtrRectSr->right;Tv_WkX++ )
			{
				//check source key
				if ( 0 != *Tv_PtrSrP )
				{
					//copy pixel
					*Tv_PtrTgP	=	*Tv_PtrSrP;
				}

				//next
				Tv_PtrSrP	++;
				Tv_PtrTgP	++;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdibBb2::ProcBltNrmPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SrHzBitPos;
		__u8		Tv_PidxMask;
		__u32*	Tv_PtrPlt;
		__u8		Tv_PltIdx;

		__u32		Tv_SubpSr;

		__u8*		Tv_PtrSrP;
		__u16*		Tv_PtrTgP;
	// code --------------------
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
													( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( 2 * A_X ) );
		Tv_SrHzBitPos	=	0x07 & (A_PtrRectSr->left * A_PtrGdibSr->BitCnt);
		Tv_PidxMask		=	(__u8)((__u16)(1 << A_PtrGdibSr->BitCnt) - 1);
		Tv_PtrPlt			=	(__u32*)GrGdibGetPalletPtr( A_PtrGdibSr );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u16*)Tv_PtrTgY;
			Tv_SubpSr			=	Tv_SrHzBitPos;
			for ( Tv_WkX=(__u32)A_PtrRectSr->left;Tv_WkX < (__u32)A_PtrRectSr->right;Tv_WkX++ )
			{
				//get pallete value
				Tv_PltIdx		=	((*Tv_PtrSrP >> Tv_SubpSr) & Tv_PidxMask);
				//copy pixel
				*Tv_PtrTgP	=	(__u16)Tv_PtrPlt[Tv_PltIdx];

				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrP	=	Tv_PtrSrP + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_PtrTgP	++;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdibBb2::ProcBltSrkPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SrHzBitPos;
		__u8		Tv_PidxMask;
		__u32*	Tv_PtrPlt;
		__u8		Tv_PltIdx;

		__u32		Tv_SubpSr;

		__u8*		Tv_PtrSrP;
		__u16*		Tv_PtrTgP;
	// code --------------------
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
													( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( 2 * A_X ) );
		Tv_SrHzBitPos	=	0x07 & (A_PtrRectSr->left * A_PtrGdibSr->BitCnt);
		Tv_PidxMask		=	(__u8)((__u16)(1 << A_PtrGdibSr->BitCnt) - 1);
		Tv_PtrPlt			=	(__u32*)GrGdibGetPalletPtr( A_PtrGdibSr );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u16*)Tv_PtrTgY;
			Tv_SubpSr			=	Tv_SrHzBitPos;
			for ( Tv_WkX=(__u32)A_PtrRectSr->left;Tv_WkX < (__u32)A_PtrRectSr->right;Tv_WkX++ )
			{
				//get pallete value
				Tv_PltIdx		=	((*Tv_PtrSrP >> Tv_SubpSr) & Tv_PidxMask);
				//check source key
				if ( 0 != Tv_PltIdx )
				{
					//copy pixel
					*Tv_PtrTgP	=	(__u16)Tv_PtrPlt[Tv_PltIdx];
				}

				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrP	=	Tv_PtrSrP + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_PtrTgP	++;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibBb2::ProcSthNpt2Npt(	Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, 
																		Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u16*		Tv_PtrSrP;
		__u16*		Tv_PtrTgP;

		__s32			Tv_SrRtX;
		__s32			Tv_SrRtY;
		__s32			Tv_TgRtX;
		__s32			Tv_TgRtY;
		__s32			Tv_SrX;
		__s32			Tv_SrY;
		void*		Tv_PtrDibSr;
	// code --------------------
		//init
		Tv_PtrDibSr	=	GrGdibGetDibPtr( A_PtrGdibSr );
		Tv_SrRtX		=	A_PtrRcSr->right - A_PtrRcSr->left;
		Tv_SrRtY		=	A_PtrRcSr->bottom - A_PtrRcSr->top;
		Tv_TgRtX		=	A_PtrRcTg->right - A_PtrRcTg->left;
		Tv_TgRtY		=	A_PtrRcTg->bottom - A_PtrRcTg->top;

		//calculate address
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_PtrRcCrntTg->top ) +
													( 2 * A_PtrRcCrntTg->left ) );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRcCrntTg->top;Tv_WkY < (__u32)A_PtrRcCrntTg->bottom;Tv_WkY++ )
		{
			//init
			Tv_SrY	=	A_PtrRcSr->top + ( ((Tv_WkY - A_PtrRcTg->top) * Tv_SrRtY ) / Tv_TgRtY );
			Tv_PtrSrY	=	(void*)(	(__u32)Tv_PtrDibSr + 
														(A_PtrGdibSr->Pitch * Tv_SrY ) );
			Tv_PtrTgP	=	(__u16*)Tv_PtrTgY;
			for ( Tv_WkX=(__u32)A_PtrRcCrntTg->left;Tv_WkX < (__u32)A_PtrRcCrntTg->right;Tv_WkX++ )
			{
				//get source x position
				Tv_SrX	=	A_PtrRcSr->left + ( ((Tv_WkX - A_PtrRcTg->left) * Tv_SrRtX ) / Tv_TgRtX );
				//get source address
				Tv_PtrSrP	=	(__u16*)( (__u32)Tv_PtrSrY + ( 2 * Tv_SrX ) );
				//write
				*Tv_PtrTgP	=	*Tv_PtrSrP;
				//next
				Tv_PtrTgP ++;
			}
			//next
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibBb2::ProcSthPlt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SubpSr;

		__u8*		Tv_PtrSrPx;
		__u16*		Tv_PtrTgP;

		__s32			Tv_SrRtX;
		__s32			Tv_SrRtY;
		__s32			Tv_TgRtX;
		__s32			Tv_TgRtY;
		__s32			Tv_SrX;
		__s32			Tv_SrY;
		void*		Tv_PtrDibSr;

		__u8		Tv_PltMask;
		__u8		Tv_PxVal;
		__u32*	Tv_PtrPltSr;
	// code --------------------
		//init
		Tv_SrRtX		=	A_PtrRcSr->right - A_PtrRcSr->left;
		Tv_SrRtY		=	A_PtrRcSr->bottom - A_PtrRcSr->top;
		Tv_TgRtX		=	A_PtrRcTg->right - A_PtrRcTg->left;
		Tv_TgRtY		=	A_PtrRcTg->bottom - A_PtrRcTg->top;

		Tv_PtrDibSr	=	GrGdibGetDibPtr( A_PtrGdibSr );
		Tv_PtrPltSr	=	(__u32*)GrGdibGetPalletPtr( A_PtrGdibSr );
		Tv_PltMask	=	(__u8)( (__u16)(1 << A_PtrGdibSr->BitCnt) -1 );
		//calculate address
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
								( m_Gdib.Pitch * A_PtrRcCrntTg->top ) +
								( 2 * A_PtrRcCrntTg->left ) );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRcCrntTg->top;Tv_WkY < (__u32)A_PtrRcCrntTg->bottom;Tv_WkY++ )
		{
			//get source y position
			Tv_SrY	=	A_PtrRcSr->top + ( ((Tv_WkY - A_PtrRcTg->top) * Tv_SrRtY ) / Tv_TgRtY );
			//get souce y addr
			Tv_PtrSrY	=	(void*)(	(__u32)Tv_PtrDibSr + ( A_PtrGdibSr->Pitch * Tv_SrY ) );
			Tv_PtrTgP	=	(__u16*)Tv_PtrTgY;
			for ( Tv_WkX=(__u32)A_PtrRcCrntTg->left;Tv_WkX < (__u32)A_PtrRcCrntTg->right;Tv_WkX++ )
			{
				//get source x position
				Tv_SrX	=	A_PtrRcSr->left + ( ((Tv_WkX - A_PtrRcTg->left) * Tv_SrRtX ) / Tv_TgRtX );
				//get address source
				Tv_PtrSrPx	=	(__u8*)( (__u32)Tv_PtrSrY + ( ( Tv_SrX * A_PtrGdibSr->BitCnt ) >> 3 ) );
				Tv_SubpSr		=	0x07 & (Tv_SrX * m_Gdib.BitCnt);
				//get pixel value
				Tv_PxVal		=	((*Tv_PtrSrPx >> Tv_SubpSr) & Tv_PltMask);
				//write
				*Tv_PtrTgP	=	(__u16)Tv_PtrPltSr[Tv_PxVal];
				//next
				Tv_PtrTgP	++;
			}
			//next
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdibBb2::DrawLine(__s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY, __u32 A_Val)
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		// check type
		if((A_StX < A_EdX) && (A_StY == A_EdY))
		{
			// horizontal line
			Tv_Rc.left		=	A_StX;
			Tv_Rc.top			=	A_StY;
			Tv_Rc.right		=	A_EdX + 1;
			Tv_Rc.bottom	=	A_EdY + 1;
			FillRect(&Tv_Rc, A_Val);
		}
		else
		{
			Cls_GrGdib::DrawLine(A_StX, A_StY, A_EdX, A_EdY, A_Val);
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrGdibBb2::FontV3Dib2Draw(__u8 A_FontPitch, void* A_PtrData, __u32 A_BitSize,
	__s32 A_PosX, __s32 A_PosY, BOOL8 A_IsDrawTxt, BOOL8 A_IsDrawShadow, __u32* A_PtrClrTbl)
{
	// local -------------------
		__u8*		Tv_PtrCode;
		__u32		Tv_Deced;
		__u32		Tv_Total;

		BOOL8		Tv_IsAble[5];

		__u8		Tv_Bit;
		__u8		Tv_Tmp;

		__u8		Tv_TblIdx;

		__u16		Tv_PxVal;

		__u8		Tv_Cmd;
		__u32		Tv_Val;

		__u32		Tv_VbPos;		// value bit pos

		__s32		Tv_WkX;
		__s32		Tv_WkY;

		__s32		Tv_DwX;
		__s32		Tv_DwLen;

		void*		Tv_PtrWkY;
		__u16*	Tv_PtrDwX;
		
	// code --------------------
		Tv_PtrCode		=	(__u8*)A_PtrData;
		Tv_Deced			=	0;
		Tv_Total			=	A_BitSize;
		Tv_Bit				=	0;

		Tv_IsAble[0]	=	A_IsDrawShadow;
		Tv_IsAble[1]	=	A_IsDrawTxt;
		Tv_IsAble[2]	=	A_IsDrawTxt;
		Tv_IsAble[3]	=	A_IsDrawTxt;
		Tv_IsAble[4]	=	A_IsDrawTxt;

		Tv_WkX				=	A_PosX;
		Tv_WkY				=	A_PosY;

		Tv_PtrWkY			=	(void*)((Def_GrCalPtr)m_Gdib.PtrExtDib + (Def_GrCalPtr)(A_PosY * m_Gdib.Pitch));

		while(Tv_WkY < m_Rect.bottom)
		{
			// check completed
			if(Tv_Deced >= Tv_Total)
			{
				break;
			}
			// get command
			Tv_Tmp	=	((*Tv_PtrCode) >> Tv_Bit) & 0x0F;
			// go next code
			Tv_Deced	=	Tv_Deced + 4;
			Tv_Bit		=	Tv_Bit ^ 4;
			if(0 == Tv_Bit)
			{
				Tv_PtrCode ++;
			}
			Tv_Cmd	=	Tv_Tmp & 7;

			// get value
			Tv_Val		=	0;
			Tv_VbPos	=	0;
			while(0 != (E_GrFontV3Dib2AtbNext & Tv_Tmp))
			{
				// check completed
				if(Tv_Deced >= Tv_Total)
				{
					break;
				}
				// get value
				Tv_Tmp	=	((*Tv_PtrCode) >> Tv_Bit) & 0x0F;
				// go next code
				Tv_Deced	=	Tv_Deced + 4;
				Tv_Bit		=	Tv_Bit ^ 4;
				if(0 == Tv_Bit)
				{
					Tv_PtrCode ++;
				}

				// update value
				Tv_Val	=	Tv_Val | (((__u32)(Tv_Tmp & 7)) << Tv_VbPos);
				// next
				Tv_VbPos	=	Tv_VbPos + 3;
			}
			Tv_Val ++;

			// run command
			if(E_GrFontV3Dib2CmdLnSkip == Tv_Cmd)
			{
				Tv_WkX	=	A_PosX;
				Tv_WkY	=	Tv_WkY + (__s32)Tv_Val;
				Tv_PtrWkY	=	(void*)((Def_GrCalPtr)Tv_PtrWkY + (Def_GrCalPtr)(m_Gdib.Pitch * Tv_Val));
			}
			else if(E_GrFontV3Dib2CmdPxIntp8 >= Tv_Cmd)
			{
				if(E_GrFontV3Dib2CmdPxSkip != Tv_Cmd)
				{
					Tv_TblIdx	=	Tv_Cmd - E_GrFontV3Dib2CmdPxShadow;
					if(Tv_IsAble[Tv_TblIdx] && (Tv_WkY >= m_Rect.top))
					{
						// patch draw position and length
						Tv_DwX		=	Tv_WkX;
						Tv_DwLen	=	(__s32)Tv_Val;
						if(Tv_DwX < m_Rect.left)
						{
							Tv_DwLen	=	Tv_DwLen - (m_Rect.left - Tv_DwX);
							Tv_DwX		=	m_Rect.left;
						}

						if((Tv_DwX + Tv_DwLen) > m_Rect.right)
						{
							Tv_DwLen	=	m_Rect.right - Tv_DwX;
						}

						// check draw able
						Tv_PxVal	=	(__u16)A_PtrClrTbl[Tv_TblIdx];
						Tv_PtrDwX	=	(__u16*)((Def_GrCalPtr)Tv_PtrWkY + (Def_GrCalPtr)(Tv_DwX << 1));
						while(0 < Tv_DwLen)
						{
							*Tv_PtrDwX	=	Tv_PxVal;
							Tv_PtrDwX ++;
							// next
							Tv_DwLen --;
						}
					}
				}
				// go next
				Tv_WkX	=	Tv_WkX + (__s32)Tv_Val;
			}
		}	//	while(Tv_WkY < Tv_RcClip.bottom)

}
//--------------------------------------------------------------------
