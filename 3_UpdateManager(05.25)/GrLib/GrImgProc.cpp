/*
	image process
		
*/

//====================================================================
// uses
#include	<GrImgProc.h>
#include	<GrImgBase.h>

//====================================================================
//local type

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
__u32		GrImpVecter8x8(void * A_PtrBlk1, void* A_PtrBlk2)
{
	// var --------------------
		__s32			Tv_WkCnt;
		__s32			Tv_Result;
		__u8*			Tv_PtrP1;
		__u8*			Tv_PtrP2;
		__s32			Tv_SumT;
		__s32			Tv_SumG;
		__s32			Tv_SqrT;	//square t
		__s32			Tv_SqrG;	//square g
		__s32			Tv_SmTG;
		__s64	Tv_Val1;	//numerator value
		__s64	Tv_Val2;	//denominator value
		__s64	Tv_Ngc;
	// code -------------------
		Tv_Result = (256 * 64);
		Tv_PtrP1	= (__u8*)A_PtrBlk1;
		Tv_PtrP2	= (__u8*)A_PtrBlk2;
		Tv_SumT		= 0;
		Tv_SumG		= 0;
		Tv_SqrT		= 0;
		Tv_SqrG		= 0;
		Tv_SmTG		= 0;
		Tv_Val1		= 0;
		Tv_Val2		= 0;

		//get sigma value
		for(Tv_WkCnt = 0;Tv_WkCnt < 64;Tv_WkCnt++)
		{
			Tv_SumT += *Tv_PtrP1;
			Tv_SumG += *Tv_PtrP2;

			Tv_SqrT += (*Tv_PtrP1 * *Tv_PtrP1);
			Tv_SqrG += (*Tv_PtrP2 * *Tv_PtrP2);

			Tv_SmTG += (*Tv_PtrP1 * *Tv_PtrP2);

			//next
			Tv_PtrP1++;
			Tv_PtrP2++;
		}

		//calculate vector
		Tv_Val1		= ((64 * (__s64)Tv_SmTG) - ((__s64)Tv_SumT * (__s64)Tv_SumG));
		Tv_Val2		= ((64 * (__s64)Tv_SqrG) - ((__s64)Tv_SumG * (__s64)Tv_SumG)) *
			((64 * (__s64)Tv_SqrT) - ((__s64)Tv_SumT * (__s64)Tv_SumT));
		Tv_Val1		=	(Tv_Val1 * Tv_Val1) * 256;

		if(Tv_Val2 != 0)
		{
			Tv_Ngc	=	(Tv_Val1 / Tv_Val2);
			if(Tv_Ngc < 0)
			{
				Tv_Ngc = 0;
			}
			Tv_Result		=	(256 - (__s32)Tv_Ngc) * 64;
		}

		if ( (256 * 64) <= Tv_Result )
		{
			Tv_Result		=	(256 * 64) - 1;
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrImpSad8x8( Ptr_GrImpSmp8x8 A_PtrSmp1, Ptr_GrImpSmp8x8 A_PtrSmp2 )
{
	// local -------------------
		__u32		Tv_Result;
		__s32			Tv_Dif;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	0;

		for ( Tv_WkIdx=0;Tv_WkIdx < 64;Tv_WkIdx++ )
		{
			Tv_Dif		=	(__s32)((__u32)A_PtrSmp1->Smp[Tv_WkIdx]) - (__s32)A_PtrSmp1->Avr 
									- ( (__s32)((__u32)A_PtrSmp2->Smp[Tv_WkIdx]) - (__s32)A_PtrSmp2->Avr );
			if ( 0 > Tv_Dif )
			{
				Tv_Dif		=	-Tv_Dif;
			}
			Tv_Result		=	Tv_Result + (__u32)Tv_Dif;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrImpSmpOneY8x8Uyvy( void* A_PtrDib, __u32 A_X, __u32 A_Y, __u32 A_Pitch, void* A_PtrRtBuf )
{
	// local -------------------
		Ptr_GrImgFmtUYVY	Tv_PtrSr;
		__u32		Tv_WkX;
		__u32		Tv_WkY;
		__u32		Tv_AddPit;
		__u8*		Tv_PtrSmp;
		Ptr_GrImpSmp8x8	Tv_PtrTbl;
	// code --------------------
		//init
		Tv_AddPit		=	A_Pitch - (sizeof(St_GrImgFmtUYVY) * 4);
		Tv_PtrSr		=	(Ptr_GrImgFmtUYVY)( (__u32)A_PtrDib + (A_Y * A_Pitch) + ((A_X & ~1) * 2) );
		Tv_PtrTbl		=	(Ptr_GrImpSmp8x8)A_PtrRtBuf;
		Tv_PtrSmp		=	Tv_PtrTbl->Smp;
		Tv_PtrTbl->Avr		=	0;
		//sampling
		for ( Tv_WkY=0;Tv_WkY < 8;Tv_WkY++ )
		{
			for ( Tv_WkX=0;Tv_WkX < 4;Tv_WkX++ )
			{
				//sample first pixel
				*Tv_PtrSmp	=	Tv_PtrSr->Y1;
				Tv_PtrTbl->Avr	=	Tv_PtrTbl->Avr + (__u32)Tv_PtrSr->Y1;
				Tv_PtrSmp++;
				//sampel second pixel
				*Tv_PtrSmp	=	Tv_PtrSr->Y2;
				Tv_PtrTbl->Avr	=	Tv_PtrTbl->Avr + (__u32)Tv_PtrSr->Y2;
				Tv_PtrSmp++;
				//next
				Tv_PtrSr ++;
			}
			//next
			Tv_PtrSr	=	(Ptr_GrImgFmtUYVY)( (__u32)Tv_PtrSr + Tv_AddPit );
		}
		Tv_PtrTbl->Avr = Tv_PtrTbl->Avr / 64;
}
//--------------------------------------------------------------------
void		GrImpSmpImgY8x8Uyvy( void* A_PtrDib, __u32 A_CntX, __u32 A_CntY, __u32 A_Pitch, void* A_PtrRtBuf )
{
	// local -------------------
		__u32		Tv_WkX;
		__u32		Tv_WkY;
		Ptr_GrImpSmp8x8	Tv_PtrSmp;
		__u32		Tv_X;
		__u32		Tv_Y;
	// code --------------------
		//init
		Tv_PtrSmp		=	(Ptr_GrImpSmp8x8)A_PtrRtBuf;
		//sampling
		Tv_Y				=	0;
		for ( Tv_WkY=0;Tv_WkY < A_CntY;Tv_WkY++ )
		{
			Tv_X				=	0;
			for ( Tv_WkX=0;Tv_WkX < A_CntX;Tv_WkX++ )
			{
				GrImpSmpOneY8x8Uyvy( A_PtrDib, Tv_X, Tv_Y, A_Pitch, (void*)Tv_PtrSmp );
				//next
				Tv_X			=	Tv_X + 8;
				Tv_PtrSmp	++;
			}
			//next
			Tv_Y			=	Tv_Y + 8;
		}
}
//--------------------------------------------------------------------
__u32		GrImpCalc8x8SmpImgSize( __u32 A_BlkCnt )
{
		return	A_BlkCnt * sizeof(St_GrImpSmp8x8);
}
//--------------------------------------------------------------------

