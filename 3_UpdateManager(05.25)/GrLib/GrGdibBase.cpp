/*
 osd skin object
*/

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrGdibBase.h>
#include	<GrImgBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<stdlib.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
__u32	V_GrGdibGplt[E_GrGdibBasePltCnt];

//====================================================================
//--------------------------------------------------------------------
__u32		GrGdibCalcSize( __u32 A_Pitch, __u32 A_ResY, __u32 A_PltBitCnt )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------

		Tv_Result	=	sizeof(St_GrGdib) + (A_Pitch * A_ResY);
		if ( 0 != A_PltBitCnt )
		{
			Tv_Result	=	Tv_Result + ( 4 * (1 << A_PltBitCnt) );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrGdibInitHead( Ptr_GrGdib A_PtrGdib,
												__u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
												__u32 A_Pitch, void* A_PtrDib, 
												__u32 A_PltBitCnt, void* A_PtrPlt )
{
	// local -------------------
	// code --------------------
		A_PtrGdib->FccGdib		=	E_GrGdibFcc;
		A_PtrGdib->FccImg			=	A_Fcc;
		A_PtrGdib->ResX				=	A_ResX;
		A_PtrGdib->ResY				=	A_ResY;
		A_PtrGdib->BitCnt			=	A_PltBitCnt;
		A_PtrGdib->IsPlt			=	FALSE;
		A_PtrGdib->PtrExtDib	=	A_PtrDib;
		A_PtrGdib->PtrExtPlt	=	NULL;

		//calculate bit count
		if ( 0 == A_PltBitCnt )
		{
			A_PtrGdib->BitCnt		=	GrDrawGetPixelBitByFcc( A_Fcc );
		}
		else
		{
			A_PtrGdib->IsPlt		=	TRUE;
			A_PtrGdib->PtrExtPlt	=	A_PtrPlt;
		}

		//set pitch
		A_PtrGdib->Pitch			=	A_Pitch;
		if ( E_GrGdibPitchAuto == A_Pitch )
		{
			A_PtrGdib->Pitch	=	GrDrawCalcPitchByBitCnt( A_PtrGdib->BitCnt, A_ResX );
		}
		
		//calculate size
		A_PtrGdib->Size		=	sizeof(St_GrGdib);
		if ( NULL == A_PtrDib )
		{
			A_PtrGdib->Size	=	A_PtrGdib->Size + ( A_PtrGdib->Pitch * A_ResY );
		}
		if ( (NULL == A_PtrPlt) && (0 != A_PltBitCnt) )
		{
			A_PtrGdib->Size	=	A_PtrGdib->Size + ( 4 * (1 << A_PltBitCnt) );
		}
}
//--------------------------------------------------------------------
void*		GrGdibGetDibPtr( Ptr_GrGdib A_PtrGdib )
{
	// local -------------------
		void*		Tv_Result;
	// code --------------------
		Tv_Result	=	A_PtrGdib->PtrExtDib;
		if ( NULL == Tv_Result )
		{
			Tv_Result	=	(void*)( (__u32)A_PtrGdib + sizeof(St_GrGdib) );
			if ( (NULL == A_PtrGdib->PtrExtPlt) && A_PtrGdib->IsPlt )
			{
				Tv_Result	=	(void*)( (__u32)Tv_Result + (4 * (1 << A_PtrGdib->BitCnt)) );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*		GrGdibGetPalletPtr( Ptr_GrGdib A_PtrGdib )
{
	// local -------------------
		void*		Tv_Result;
	// code --------------------
		Tv_Result	=	A_PtrGdib->PtrExtPlt;
		if ( A_PtrGdib->IsPlt )
		{
			if ( NULL == Tv_Result )
			{
				Tv_Result	=	(void*)( (__u32)A_PtrGdib + sizeof(St_GrGdib) );
			}
			else if ( E_GrGdibPltBase == (__u32)Tv_Result )
			{
				Tv_Result	=	GrGdibGetBasePalletePtr();
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrGdibGetBasePalletePtr( void )
{
		return	(void*)V_GrGdibGplt;
}
//--------------------------------------------------------------------
__u32	GrGdibBasePltColorGet( __u32 A_Idx )
{
		return	V_GrGdibGplt[A_Idx];
}
//--------------------------------------------------------------------
void	GrGdibBasePltColorSet( __u32 A_Idx, __u32 A_Color )
{
		if ( E_GrGdibBasePltCnt > A_Idx )
		{
			V_GrGdibGplt[A_Idx]	=	A_Color;
		}
}
//--------------------------------------------------------------------
void	GrGdibAlphaChgArgb8888( Ptr_GrGdib A_PtrGdib, __u8 A_AlphaVal )
{
	// local -------------------
		__u32	Tv_PltCnt;
		__u32*	Tv_PtrClr;
		__u32	Tv_WkX;
		__u32	Tv_WkY;
		void*	Tv_PtrY;
	// code --------------------
		// check match mode
		if ( E_GrFccARGB8888 == A_PtrGdib->FccImg )
		{
			if ( A_PtrGdib->IsPlt )
			{
				// change pallet only
				Tv_PltCnt	=	1 << A_PtrGdib->BitCnt;
				Tv_PtrClr	=	(__u32*)GrGdibGetPalletPtr( A_PtrGdib );

				for ( Tv_WkX=0;Tv_WkX < Tv_PltCnt;Tv_WkX++ )
				{
					if(0xFF != ((Ptr_GrImgFmtARGB8888)Tv_PtrClr)->A)
					{
						((Ptr_GrImgFmtARGB8888)Tv_PtrClr)->A	=	A_AlphaVal;
					}
					// next
					Tv_PtrClr ++;
				}
			}
			else
			{
				// init
				Tv_PtrY		=	GrGdibGetDibPtr( A_PtrGdib );
				for ( Tv_WkY=0;Tv_WkY < A_PtrGdib->ResY;Tv_WkY ++ )
				{
					Tv_PtrClr	=	(__u32*)Tv_PtrY;
					for ( Tv_WkX=0;Tv_WkX < A_PtrGdib->ResX;Tv_WkX ++ )
					{
						// check
						if(0xFF != ((Ptr_GrImgFmtARGB8888)Tv_PtrClr)->A)
						{
							((Ptr_GrImgFmtARGB8888)Tv_PtrClr)->A	=	A_AlphaVal;
						}
						// next
						Tv_PtrClr	++;
					}
					// next
					Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + A_PtrGdib->Pitch );
				}
				
			}
		}
		
}
//--------------------------------------------------------------------
