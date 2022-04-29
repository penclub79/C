/*
	dib convert

 */

//====================================================================
// uses
#include	<GrDibCvt.h>
#include	<stdlib.h>
#include	<GrImgBase.h>

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
void	GrDibCvtUyvyToRgb15( void* A_PtrDibDst, __s32 A_PitchDst, void* A_PtrDibSrc, __s32 A_PitchSrc, __u32 A_ResX, __u32 A_ResY )
{
	// local -------------------
		__s32			Tv_ValY;
		__s32			Tv_ValU;
		__s32			Tv_ValV;

		__s32			Tv_ValR;
		__s32			Tv_ValG;
		__s32			Tv_ValB;

		__s32			Tv_CntX;
		Ptr_GrImgFmtUYVY	Tv_PtrSrX;
		void*		Tv_PtrSrY;

		__u16*		Tv_PtrTgX;
		void*		Tv_PtrTgY;

		__s32			Tv_WkX;
		__s32			Tv_WkY;
		__s32			Tv_PitSr;
	// code --------------------
		//init
		Tv_CntX		=	A_ResX / 2;
		Tv_PtrTgY	=	A_PtrDibDst;
		Tv_PtrSrY	=	A_PtrDibSrc;
		Tv_PitSr	=	A_PitchSrc;

		for ( Tv_WkY = 0;Tv_WkY < (__s32)A_ResY;Tv_WkY++ )
		{
			Tv_PtrSrX		=	(Ptr_GrImgFmtUYVY)Tv_PtrSrY;
			Tv_PtrTgX		=	(__u16*)Tv_PtrTgY;
			for ( Tv_WkX = 0;Tv_WkX < Tv_CntX;Tv_WkX++ )
			{
				//sampling even pixel
				Tv_ValY		=	(__s32)Tv_PtrSrX->Y1;
				Tv_ValU		=	(__s32)Tv_PtrSrX->U;
				Tv_ValV		=	(__s32)Tv_PtrSrX->V;

				//convert pixle even
				//red
				Tv_ValR		= (1192*((__s32)Tv_ValY - 16) + 1634*((__s32)Tv_ValV - 128)) / 1024;
				if			( 0 > Tv_ValR )
				{
					Tv_ValR		=	0;
				}
				else if	( 255 < Tv_ValR )
				{
					Tv_ValR		=	255;
				}

				//green
				Tv_ValG		=	(1192*((__s32)Tv_ValY - 16) - 401*((__s32)Tv_ValU - 128)
					- 833*((__s32)Tv_ValV - 128)) / 1024;
				if			( 0 > Tv_ValG )
				{
					Tv_ValG		=	0;
				}
				else if	( 255 < Tv_ValG )
				{
					Tv_ValG		=	255;
				}

				//blue
				Tv_ValB   =	(1192*((__s32)Tv_ValY - 16) + 2065*((__s32)Tv_ValU - 128)) / 1024;
				if			( 0 > Tv_ValB )
				{
					Tv_ValB		=	0;
				}
				else if	( 255 < Tv_ValB )
				{
					Tv_ValB		=	255;
				}

				//writing
				*Tv_PtrTgX	=	(__u16)((Tv_ValR & 0xF8) << 7) | (__u16)((Tv_ValG & 0xF8) << 2) |
											(__u16)((Tv_ValB & 0xF8) >> 3);

				//next pixel
				Tv_PtrTgX ++;

				//sampling pixel odd pixel
				Tv_ValY		=	(__s32)Tv_PtrSrX->Y2;

				//convert pixle odd pixel
				//red
				Tv_ValR		= (1192*((__s32)Tv_ValY - 16) + 1634*((__s32)Tv_ValV - 128)) / 1024;
				if			( 0 > Tv_ValR )
				{
					Tv_ValR		=	0;
				}
				else if	( 255 < Tv_ValR )
				{
					Tv_ValR		=	255;
				}

				//green
				Tv_ValG		=	(1192*((__s32)Tv_ValY - 16) - 401*((__s32)Tv_ValU - 128)
					- 833*((__s32)Tv_ValV - 128)) / 1024;
				if			( 0 > Tv_ValG )
				{
					Tv_ValG		=	0;
				}
				else if	( 255 < Tv_ValG )
				{
					Tv_ValG		=	255;
				}

				//blue
				Tv_ValB   =	(1192*((__s32)Tv_ValY - 16) + 2065*((__s32)Tv_ValU - 128)) / 1024;
				if			( 0 > Tv_ValB )
				{
					Tv_ValB		=	0;
				}
				else if	( 255 < Tv_ValB )
				{
					Tv_ValB		=	255;
				}

				//writing
				*Tv_PtrTgX	=	(__u16)((Tv_ValR & 0xF8) << 7) | (__u16)((Tv_ValG & 0xF8) << 2) |
					(__u16)((Tv_ValB & 0xF8) >> 3);

				//next
				Tv_PtrSrX ++;
				Tv_PtrTgX ++;

			}
			//next
			Tv_PtrSrY	=	(void*)( (__s32)Tv_PtrSrY + Tv_PitSr );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + A_PitchDst );

		}
}
//--------------------------------------------------------------------

