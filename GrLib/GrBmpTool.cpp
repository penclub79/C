/*
	dumy Tools	-	window version
		
*/

//====================================================================
// uses
#include	<GrBmpTool.h>
#include	<GrImgBase.h>

//====================================================================
//local type

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
BOOL8	GrBmpGetInfo( void* A_PtrBmp, __u32* A_PtrRtResX, __u32* A_PtrRtResY, 
									 __u32* A_PtrRtBit )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrBmpFlHead	Tv_PtrBmp;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		Tv_PtrBmp		=	(Ptr_GrBmpFlHead)A_PtrBmp;

		//check header
		if ( E_GrBmpFileId == Tv_PtrBmp->Id )
		{
			//get resolution
			*A_PtrRtResX		=	Tv_PtrBmp->Bi.Width;
			*A_PtrRtResY		=	Tv_PtrBmp->Bi.Height;
			if ( 0 > *A_PtrRtResY )
			{
				*A_PtrRtResY		=	-Tv_PtrBmp->Bi.Height;
			}
			//get bit count
			*A_PtrRtBit		=	Tv_PtrBmp->Bi.BitCount;
			if ( 16 == Tv_PtrBmp->Bi.BitCount )
			{
				*A_PtrRtBit		=	15;
			}

			if ( 0 != *A_PtrRtBit )
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrBmpGetDibPtr( void* A_PtrBmp )
{
	// local -------------------
		void*		Tv_Result;
		Ptr_GrBmpFlHead	Tv_PtrBmp;
	// code --------------------
		//init
		Tv_Result		=	NULL;
		Tv_PtrBmp		=	(Ptr_GrBmpFlHead)A_PtrBmp;

		//check usable bmp
		if ( E_GrBmpFileId == Tv_PtrBmp->Id )
		{
			Tv_Result		=	(void*)( (__u32)A_PtrBmp + Tv_PtrBmp->OfsDib );
		}

		return		Tv_Result;
}
//--------------------------------------------------------------------
void*	GrBmpGetPalletPtr( void* A_PtrBmp )
{
	// local -------------------
		void*		Tv_Result;
		Ptr_GrBmpFlHead	Tv_PtrBmp;
	// code --------------------
		//init
		Tv_Result		=	NULL;
		Tv_PtrBmp		=	(Ptr_GrBmpFlHead)A_PtrBmp;

		//check usable bmp
		if ( E_GrBmpFileId == Tv_PtrBmp->Id )
		{
			//check pallete
			if ( (0 != Tv_PtrBmp->Bi.BitCount) && (8 >= Tv_PtrBmp->Bi.BitCount) )
			{
				Tv_Result		=	(void*)( (__u32)&Tv_PtrBmp->Bi + Tv_PtrBmp->Bi.Size );
			}
		}

		return		Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrBmpCalcSize( __u32 A_ResX, __u32 A_ResY, __u32 A_Bit )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Pitch;
	// code --------------------
		//init
		Tv_Result		=	0;
		//check resolution
		if ( (0 != A_ResX) && (0 != A_ResY) && (0 != A_Bit) )
		{
			if ( 8 >= A_Bit )
			{
				Tv_Result		=	((__u32)(1 << A_Bit) * sizeof(St_GrImgFmtARGB8888));
			}
			Tv_Result	=	Tv_Result + sizeof(St_GrBmpFlHead);
			Tv_Pitch	=	GrDrawCalcPitchByBitCnt( A_Bit, A_ResX );
			Tv_Result	=	Tv_Result + ( Tv_Pitch * A_ResY );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrBmpSetHeader( void* A_PtrBmp, __u32 A_ResX, __u32 A_ResY, __u32 A_Bit )
{
	// local -------------------
		Ptr_GrBmpFlHead	Tv_PtrBmp;
		__u32		Tv_Pitch;
		__u32		Tv_SzPal;
		__u32		Tv_SzImg;
	// code --------------------
		//check para
		if ( (0 != A_ResX) && (0 != A_ResY) && (0 != A_Bit) )
		{
			//init
			Tv_PtrBmp		=	(Ptr_GrBmpFlHead)A_PtrBmp;

			Tv_Pitch	=	GrDrawCalcPitchByBitCnt( A_Bit, A_ResX );
			Tv_SzPal		=	0;
			if ( 8 >= A_Bit )
			{
				Tv_SzPal		=	((__u32)(1 << A_Bit) * sizeof(St_GrImgFmtARGB8888));
			}
			Tv_SzImg	=	( Tv_Pitch * A_ResY );

			//set file header
			Tv_PtrBmp->Id		=	E_GrBmpFileId;
			Tv_PtrBmp->Size		=	Tv_SzPal + sizeof(St_GrBmpFlHead) + Tv_SzImg;
			Tv_PtrBmp->OfsDib	=	Tv_SzPal + sizeof(St_GrBmpFlHead);

			//set bitmap info header
			Tv_PtrBmp->Bi.Size			=	sizeof( St_GrBmpIfHd );
			Tv_PtrBmp->Bi.Width			=	A_ResX;
			Tv_PtrBmp->Bi.Height		=	A_ResY;
			Tv_PtrBmp->Bi.Planes		=	0;
			Tv_PtrBmp->Bi.BitCount	=	(__u16)A_Bit;
			if ( 15 == A_Bit )
			{
				Tv_PtrBmp->Bi.BitCount	=	16;
			}
			Tv_PtrBmp->Bi.Compression	=	0;
			Tv_PtrBmp->Bi.SizeImage	=	Tv_SzImg;
			Tv_PtrBmp->Bi.XPelsPerMeter	=	0;
			Tv_PtrBmp->Bi.YPelsPerMeter	=	0;
			Tv_PtrBmp->Bi.ClrUsed		=	0;
			if ( 8 >= A_Bit )
			{
				Tv_PtrBmp->Bi.ClrUsed		=	1 << A_Bit;
			}
			Tv_PtrBmp->Bi.ClrImportant	=	0;
	
		}
}
//--------------------------------------------------------------------

