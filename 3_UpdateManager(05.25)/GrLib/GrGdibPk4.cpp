/*
 gdib pack mode 

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrGdibPk4.h>
#include	<GrImgBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrMath.h>
#include	<stdlib.h>
#include	<GrDibCvt.h>

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
Cls_GrGdibPk4::Cls_GrGdibPk4( __u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
															__u32 A_Pitch, void* A_PtrDib, 
															__u32 A_PltBitCnt, void* A_PtrPlt ):
Cls_GrGdib( A_Fcc, A_ResX, A_ResY, A_Pitch, A_PtrDib, A_PltBitCnt, A_PtrPlt )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrGdibPk4PxEleCnt;Tv_WkIdx ++ )
		{
			//byte struct index
			m_PxBpos[0][Tv_WkIdx]		=	m_PtrPxSt->PxSt[0][Tv_WkIdx].Pos / 8;
			m_PxBpos[1][Tv_WkIdx]		=	m_PtrPxSt->PxSt[1][Tv_WkIdx].Pos / 8;
		}
		//pixel mask
		m_PxMask[0]		=	GrDrawGetPxMask( m_PtrPxSt );
		m_PxMask[1]		=	GrDrawGetPxMask( m_PtrPxSt , TRUE );

}
//--------------------------------------------------------------------
Cls_GrGdibPk4::~Cls_GrGdibPk4()
{
}
//--------------------------------------------------------------------
void		Cls_GrGdibPk4::ProcSetPixelNpt( __s32 A_X, __s32 A_Y, __u32 A_PxVal )
{
	// local -------------------
		__u32*	Tv_PtrPx;		
		__u32		Tv_SubPos;
	// code --------------------
		//calculate address
		Tv_PtrPx	=	(__u32*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_Y) + (2 * ((__u32)A_X & ~1)) );
		Tv_SubPos	=	A_X & 1;
		//set pixel
		*Tv_PtrPx	=	( (~m_PxMask[Tv_SubPos]) & *Tv_PtrPx ) | ( m_PxMask[Tv_SubPos] & A_PxVal );
}
//--------------------------------------------------------------------
__u32		Cls_GrGdibPk4::ProcGetPixelNpt( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		__u32		Tv_Result;
		__u8*		Tv_PtrPx;		
		__u32		Tv_SubPos;
		__u8*		Tv_PtrRslt;
	// code --------------------
		//init
		Tv_PtrRslt	=	(__u8*)&Tv_Result;
		//calculate address
		Tv_PtrPx	=	(__u8*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_Y) + (2 * ((__u32)A_X & ~1)) );
		Tv_SubPos	=	A_X & 1;
		//get pixel
		Tv_PtrRslt[m_PxBpos[0][0]]	=	Tv_PtrPx[m_PxBpos[Tv_SubPos][0]];
		Tv_PtrRslt[m_PxBpos[1][0]]	=	Tv_PtrPx[m_PxBpos[Tv_SubPos][0]];
		Tv_PtrRslt[m_PxBpos[0][1]]	=	Tv_PtrPx[m_PxBpos[Tv_SubPos][1]];
		Tv_PtrRslt[m_PxBpos[0][2]]	=	Tv_PtrPx[m_PxBpos[Tv_SubPos][2]];

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		Cls_GrGdibPk4::ProcFillRectNpt( Ptr_GrRect A_PtrRect, __u32 A_PxVal )
{
	// local -------------------
		__u32		Tv_WkY;
		BOOL8		Tv_IsSgHd;		//single head
		BOOL8		Tv_IsSgTl;		//single tail
		__u32		Tv_MidCnt;		//middle pixel count
		__u32*	Tv_PtrPx;
		void*		Tv_PtrPos;
	// code --------------------
		//init
		Tv_IsSgHd		=	FALSE;
		Tv_IsSgTl		=	FALSE;
		Tv_MidCnt		=	A_PtrRect->right - A_PtrRect->left;
		if ( 0 != ( 1 & A_PtrRect->left ) )
		{
			Tv_MidCnt --;
			Tv_IsSgHd	=	TRUE;
		}
		if ( 0 != ( 1 & Tv_MidCnt ) )
		{
			Tv_MidCnt --;
			Tv_IsSgTl	=	TRUE;
		}
		Tv_MidCnt	=	Tv_MidCnt >> 1;

		//calcluate address
		Tv_PtrPos	=	(__u32*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_PtrRect->top) + (2 * ((__u32)A_PtrRect->left & ~1)) );
		
		//work vertical
		for ( Tv_WkY = (__u32)A_PtrRect->top;Tv_WkY < (__u32)A_PtrRect->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrPx	=	(__u32*)Tv_PtrPos;
			//single head
			if ( Tv_IsSgHd )
			{
				*Tv_PtrPx	=	( (~m_PxMask[1]) & *Tv_PtrPx ) | ( m_PxMask[1] & A_PxVal );
				Tv_PtrPx ++;
			}
			//middle 
			if ( 0 != Tv_MidCnt )
			{
				Tv_PtrPx	=	(__u32*)GrDumyFillD( Tv_PtrPx, Tv_MidCnt, A_PxVal );
			}
			//single tail
			if ( Tv_IsSgTl )
			{
				*Tv_PtrPx	=	( (~m_PxMask[0]) & *Tv_PtrPx ) | ( m_PxMask[0] & A_PxVal );
			}
			//next
			Tv_PtrPos	=	(void*)( (__u32)Tv_PtrPos + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdibPk4::ProcBltNrmNpt2Npt( __s32 A_X, __s32 A_Y,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		__u32		Tv_CntHz;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//acc count
		Tv_CntHz		=	A_PtrRectSr->right - A_PtrRectSr->left;
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
			( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
			( 2 * ( A_PtrRectSr->left & ~1 ) ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
			( m_Gdib.Pitch * A_Y ) +
			( 2 * ( A_X & ~1 ) ) );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			Tv_SubpSr			=	1 & A_PtrRectSr->left;
			Tv_SubpTg			=	1 & A_X;
			for ( Tv_WkX=0;Tv_WkX < Tv_CntHz;Tv_WkX++ )
			{
				//copy pixel
				Tv_PtrTgP[m_PxBpos[Tv_SubpTg][0]]	=	Tv_PtrSrP[m_PxBpos[Tv_SubpSr][0]];
				Tv_PtrTgP[m_PxBpos[Tv_SubpTg][1]]	=	Tv_PtrSrP[m_PxBpos[Tv_SubpSr][1]];
				Tv_PtrTgP[m_PxBpos[Tv_SubpTg][2]]	=	Tv_PtrSrP[m_PxBpos[Tv_SubpSr][2]];

				//next
				Tv_PtrSrP	=	Tv_PtrSrP + (Tv_SubpSr * 4);
				Tv_PtrTgP	=	Tv_PtrTgP + (Tv_SubpTg * 4);
				Tv_SubpSr		=	Tv_SubpSr ^ 1;
				Tv_SubpTg		=	Tv_SubpTg ^ 1;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdibPk4::ProcBltSrkNpt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		__u32		Tv_CntHz;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//acc count
		Tv_CntHz		=	A_PtrRectSr->right - A_PtrRectSr->left;
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
													( 2 * ( A_PtrRectSr->left & ~1 ) ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( 2 * ( A_X & ~1 ) ) );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			Tv_SubpSr			=	1 & A_PtrRectSr->left;
			Tv_SubpTg			=	1 & A_X;
			for ( Tv_WkX=0;Tv_WkX < Tv_CntHz;Tv_WkX++ )
			{
				//check source key
				if (	(0 != Tv_PtrSrP[m_PxBpos[Tv_SubpSr][0]]) || 
							(0 != Tv_PtrSrP[m_PxBpos[Tv_SubpSr][1]]) || 
							(0 != Tv_PtrSrP[m_PxBpos[Tv_SubpSr][2]])		)
				{
					Tv_PtrTgP[m_PxBpos[Tv_SubpTg][0]]	=	Tv_PtrSrP[m_PxBpos[Tv_SubpSr][0]];
					Tv_PtrTgP[m_PxBpos[Tv_SubpTg][1]]	=	Tv_PtrSrP[m_PxBpos[Tv_SubpSr][1]];
					Tv_PtrTgP[m_PxBpos[Tv_SubpTg][2]]	=	Tv_PtrSrP[m_PxBpos[Tv_SubpSr][2]];
				}
				
				//next
				Tv_PtrSrP	=	Tv_PtrSrP + (Tv_SubpSr * 4);
				Tv_PtrTgP	=	Tv_PtrTgP + (Tv_SubpTg * 4);
				Tv_SubpSr		=	Tv_SubpSr ^ 1;
				Tv_SubpTg		=	Tv_SubpTg ^ 1;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdibPk4::ProcBltNrmPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		__u32		Tv_CntHz;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SrHzBitPos;
		__u8		Tv_PidxMask;
		__u32*	Tv_PtrPlt;
		__u8*		Tv_PtrWkPlt;
		__u8		Tv_PltIdx;

		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//acc count
		Tv_CntHz		=	A_PtrRectSr->right - A_PtrRectSr->left;
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
													( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( 2 * ( A_X & ~1 ) ) );
		Tv_SrHzBitPos	=	0x07 & (A_PtrRectSr->left * A_PtrGdibSr->BitCnt);
		Tv_PidxMask		=	(__u8)((__u16)(1 << A_PtrGdibSr->BitCnt) - 1);
		Tv_PtrPlt			=	(__u32*)GrGdibGetPalletPtr( A_PtrGdibSr );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			Tv_SubpSr			=	Tv_SrHzBitPos;
			Tv_SubpTg			=	1 & A_X;
			for ( Tv_WkX=0;Tv_WkX < Tv_CntHz;Tv_WkX++ )
			{
				//get pallete value
				Tv_PltIdx		=	((*Tv_PtrSrP >> Tv_SubpSr) & Tv_PidxMask);
				Tv_PtrWkPlt	=	(__u8*)( &(Tv_PtrPlt[Tv_PltIdx]) );
				//copy pixel
				Tv_PtrTgP[m_PxBpos[Tv_SubpTg][0]]	=	Tv_PtrWkPlt[m_PxBpos[0][0]];
				Tv_PtrTgP[m_PxBpos[Tv_SubpTg][1]]	=	Tv_PtrWkPlt[m_PxBpos[0][1]];
				Tv_PtrTgP[m_PxBpos[Tv_SubpTg][2]]	=	Tv_PtrWkPlt[m_PxBpos[0][2]];

				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrP	=	Tv_PtrSrP + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_PtrTgP	=	Tv_PtrTgP + (Tv_SubpTg * 4);
				Tv_SubpTg	=	Tv_SubpTg ^ 1;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdibPk4::ProcBltSrkPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		__u32		Tv_CntHz;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SrHzBitPos;
		__u8		Tv_PidxMask;
		__u32*	Tv_PtrPlt;
		__u8*		Tv_PtrWkPlt;
		__u8		Tv_PltIdx;

		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//acc count
		Tv_CntHz		=	A_PtrRectSr->right - A_PtrRectSr->left;
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
													( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( 2 * ( A_X & ~1 ) ) );
		Tv_SrHzBitPos	=	0x07 & (A_PtrRectSr->left * A_PtrGdibSr->BitCnt);
		Tv_PidxMask		=	(__u8)((__u16)(1 << A_PtrGdibSr->BitCnt) - 1);
		Tv_PtrPlt			=	(__u32*)GrGdibGetPalletPtr( A_PtrGdibSr );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			Tv_SubpSr			=	Tv_SrHzBitPos;
			Tv_SubpTg			=	1 & A_X;
			for ( Tv_WkX=0;Tv_WkX < Tv_CntHz;Tv_WkX++ )
			{
				//get pallete value
				Tv_PltIdx		=	((*Tv_PtrSrP >> Tv_SubpSr) & Tv_PidxMask);
				//check source key
				//if ( Tv_PtrPlt[Tv_PltIdx] != m_ClrKeySrc	)
				if ( 0 != Tv_PltIdx	)
				{
					Tv_PtrWkPlt	=	(__u8*)( &(Tv_PtrPlt[Tv_PltIdx]) );
					//copy pixel
					Tv_PtrTgP[m_PxBpos[Tv_SubpTg][0]]	=	Tv_PtrWkPlt[m_PxBpos[0][0]];
					Tv_PtrTgP[m_PxBpos[Tv_SubpTg][1]]	=	Tv_PtrWkPlt[m_PxBpos[0][1]];
					Tv_PtrTgP[m_PxBpos[Tv_SubpTg][2]]	=	Tv_PtrWkPlt[m_PxBpos[0][2]];
				}
				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrP	=	Tv_PtrSrP + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_PtrTgP	=	Tv_PtrTgP + (Tv_SubpTg * 4);
				Tv_SubpTg	=	Tv_SubpTg ^ 1;
			}

			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibPk4::ProcSthNpt2Npt(	Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, 
																			Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;

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
													( 2 * ( A_PtrRcCrntTg->left & ~1 ) ) );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRcCrntTg->top;Tv_WkY < (__u32)A_PtrRcCrntTg->bottom;Tv_WkY++ )
		{
			//init
			Tv_SrY	=	A_PtrRcSr->top + ( ((Tv_WkY - A_PtrRcTg->top) * Tv_SrRtY ) / Tv_TgRtY );
			Tv_PtrSrY	=	(void*)(	(__u32)Tv_PtrDibSr + 
														( A_PtrGdibSr->Pitch * Tv_SrY ) );
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			Tv_SubpTg			=	1 & A_PtrRcCrntTg->left;
			for ( Tv_WkX=(__u32)A_PtrRcCrntTg->left;Tv_WkX < (__u32)A_PtrRcCrntTg->right;Tv_WkX++ )
			{
				//get source x position
				Tv_SrX	=	A_PtrRcSr->left + ( ((Tv_WkX - A_PtrRcTg->left) * Tv_SrRtX ) / Tv_TgRtX );
				//get source address
				Tv_PtrSrP	=	(__u8*)( (__u32)Tv_PtrSrY + ( 2 * ( Tv_SrX & ~1 ) ) );
				Tv_SubpSr	=	1 & Tv_SrX;
				//write
				Tv_PtrTgP[m_PxBpos[Tv_SubpTg][0]]	=	Tv_PtrSrP[m_PxBpos[Tv_SubpSr][0]];
				Tv_PtrTgP[m_PxBpos[Tv_SubpTg][1]]	=	Tv_PtrSrP[m_PxBpos[Tv_SubpSr][1]];
				Tv_PtrTgP[m_PxBpos[Tv_SubpTg][2]]	=	Tv_PtrSrP[m_PxBpos[Tv_SubpSr][2]];
				//next
				Tv_PtrTgP	=	Tv_PtrTgP + (Tv_SubpTg * 4);
				Tv_SubpTg		=	Tv_SubpTg ^ 1;
			}
			//next
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibPk4::ProcSthPlt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;

		__u8*		Tv_PtrSrPx;
		__u8*		Tv_PtrTgP;

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
		__u8*		Tv_PtrWkPlt;
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
								( 2 * ( A_PtrRcCrntTg->left & ~1 ) ) );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRcCrntTg->top;Tv_WkY < (__u32)A_PtrRcCrntTg->bottom;Tv_WkY++ )
		{
			//get source y position
			Tv_SrY	=	A_PtrRcSr->top + ( ((Tv_WkY - A_PtrRcTg->top) * Tv_SrRtY ) / Tv_TgRtY );
			//get souce y addr
			Tv_PtrSrY	=	(void*)(	(__u32)Tv_PtrDibSr + ( A_PtrGdibSr->Pitch * Tv_SrY ) );
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			Tv_SubpTg			=	1 & A_PtrRcCrntTg->left;
			for ( Tv_WkX=(__u32)A_PtrRcCrntTg->left;Tv_WkX < (__u32)A_PtrRcCrntTg->right;Tv_WkX++ )
			{
				//get source x position
				Tv_SrX	=	A_PtrRcSr->left + ( ((Tv_WkX - A_PtrRcTg->left) * Tv_SrRtX ) / Tv_TgRtX );
				//get address source
				Tv_PtrSrPx	=	(__u8*)( (__u32)Tv_PtrSrY + ( ( Tv_SrX * A_PtrGdibSr->BitCnt ) >> 3 ) );
				Tv_SubpSr		=	0x07 & (Tv_SrX * m_Gdib.BitCnt);
				//get pixel value
				Tv_PxVal		=	((*Tv_PtrSrPx >> Tv_SubpSr) & Tv_PltMask);
				//get pallete address
				Tv_PtrWkPlt	=	(__u8*)( &Tv_PtrPltSr[Tv_PxVal] );
				//write
				Tv_PtrTgP[m_PxBpos[Tv_SubpTg][0]]	=	Tv_PtrWkPlt[m_PxBpos[0][0]];
				Tv_PtrTgP[m_PxBpos[Tv_SubpTg][1]]	=	Tv_PtrWkPlt[m_PxBpos[0][1]];
				Tv_PtrTgP[m_PxBpos[Tv_SubpTg][2]]	=	Tv_PtrWkPlt[m_PxBpos[0][2]];
				//next
				Tv_PtrTgP	=	Tv_PtrTgP + (Tv_SubpTg * 4);
				Tv_SubpTg		=	Tv_SubpTg ^ 1;
			}
			//next
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibPk4::ProcCvtNpt2Npt( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8	Tv_IsDrcCvt;
		void*	Tv_PtrDibSr;
		void*	Tv_PtrDibTg;
		__u32	Tv_CntX;
		__u32	Tv_CntY;
		__u32	Tv_OutFcc;
	// code --------------------
		//check convert
		Tv_IsDrcCvt		=	FALSE;
		if ( (E_GrFccUYVYF == m_Gdib.FccImg) || (E_GrFccUYVY == m_Gdib.FccImg) ) 
		{
			Tv_OutFcc	=	A_ObjGdib->GetFcc();
			if ( (E_GrFccRGB555 == Tv_OutFcc) || (E_GrFccARGB1555 == Tv_OutFcc) )
			{
				//direct converted
				Tv_PtrDibSr	=	(void*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_PtrRect->top) + (2 * (A_PtrRect->left & 0xFFFFFFFE)) );
				Tv_PtrDibTg	=	(void*)( (__u32)A_ObjGdib->GetDibPtr() + (A_ObjGdib->GetDibPitch() * A_PtrRect->top) + (2 * (A_PtrRect->left & 0xFFFFFFFE)) );
				Tv_CntX			=	A_PtrRect->right - A_PtrRect->left;
				Tv_CntY			=	A_PtrRect->bottom - A_PtrRect->top;
				GrDibCvtUyvyToRgb15(	Tv_PtrDibTg, (__s32)A_ObjGdib->GetDibPitch(),
															Tv_PtrDibSr, (__s32)m_Gdib.Pitch, 
															Tv_CntX, Tv_CntY );
				Tv_IsDrcCvt		=	TRUE;
			}
		}
		//not direct converted
		if ( !Tv_IsDrcCvt )
		{
			Cls_GrGdib::ProcCvtNpt2Npt( A_ObjGdib, A_PtrRect );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibPk4::ProcFntNrmNpt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		__u32		Tv_CntHz;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SrHzBitPos;
		__u8		Tv_PidxMask;
		__u32*	Tv_PtrPlt;
		__u8*		Tv_PtrWkPlt;
		__u8		Tv_PltIdx;

		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//acc count
		Tv_CntHz		=	A_PtrRectSr->right - A_PtrRectSr->left;
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
								( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
								( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
								( m_Gdib.Pitch * A_Y ) +
								( 2 * ( A_X & ~1 ) ) );
		Tv_SrHzBitPos	=	0x07 & (A_PtrRectSr->left * A_PtrGdibSr->BitCnt);
		Tv_PidxMask		=	(__u8)((__u16)(1 << A_PtrGdibSr->BitCnt) - 1);
		Tv_PtrPlt			=	(__u32*)GrGdibGetPalletPtr( A_PtrGdibSr );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			Tv_SubpSr			=	Tv_SrHzBitPos;
			Tv_SubpTg			=	1 & A_X;
			for ( Tv_WkX=0;Tv_WkX < Tv_CntHz;Tv_WkX++ )
			{
				//get pallete value
				Tv_PltIdx		=	((*Tv_PtrSrP >> Tv_SubpSr) & Tv_PidxMask);
				//check source key
				if ( E_GrGdibFontTextPltIdx == Tv_PltIdx )
				{
					Tv_PtrWkPlt	=	(__u8*)( &(Tv_PtrPlt[Tv_PltIdx]) );
					//copy pixel
					Tv_PtrTgP[m_PxBpos[Tv_SubpTg][0]]	=	Tv_PtrWkPlt[m_PxBpos[0][0]];
					Tv_PtrTgP[m_PxBpos[Tv_SubpTg][1]]	=	Tv_PtrWkPlt[m_PxBpos[0][1]];
					Tv_PtrTgP[m_PxBpos[Tv_SubpTg][2]]	=	Tv_PtrWkPlt[m_PxBpos[0][2]];
				}
				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrP	=	Tv_PtrSrP + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_PtrTgP	=	Tv_PtrTgP + (Tv_SubpTg * 4);
				Tv_SubpTg	=	Tv_SubpTg ^ 1;
			}

			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibPk4::ProcFntSdwNpt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		__u32		Tv_CntHz;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SrHzBitPos;
		__u8		Tv_PidxMask;
		__u32*	Tv_PtrPlt;
		__u8*		Tv_PtrWkPlt;
		__u8		Tv_PltIdx;

		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//acc count
		Tv_CntHz		=	A_PtrRectSr->right - A_PtrRectSr->left;
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
								( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
								( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
								( m_Gdib.Pitch * A_Y ) +
								( 2 * ( A_X & ~1 ) ) );
		Tv_SrHzBitPos	=	0x07 & (A_PtrRectSr->left * A_PtrGdibSr->BitCnt);
		Tv_PidxMask		=	(__u8)((__u16)(1 << A_PtrGdibSr->BitCnt) - 1);
		Tv_PtrPlt			=	(__u32*)GrGdibGetPalletPtr( A_PtrGdibSr );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			Tv_SubpSr			=	Tv_SrHzBitPos;
			Tv_SubpTg			=	1 & A_X;
			for ( Tv_WkX=0;Tv_WkX < Tv_CntHz;Tv_WkX++ )
			{
				//get pallete value
				Tv_PltIdx		=	((*Tv_PtrSrP >> Tv_SubpSr) & Tv_PidxMask);
				//check source key
				if ( 0 != Tv_PltIdx )
				{
					Tv_PtrWkPlt	=	(__u8*)( &(Tv_PtrPlt[Tv_PltIdx]) );
					//copy pixel
					Tv_PtrTgP[m_PxBpos[Tv_SubpTg][0]]	=	Tv_PtrWkPlt[m_PxBpos[0][0]];
					Tv_PtrTgP[m_PxBpos[Tv_SubpTg][1]]	=	Tv_PtrWkPlt[m_PxBpos[0][1]];
					Tv_PtrTgP[m_PxBpos[Tv_SubpTg][2]]	=	Tv_PtrWkPlt[m_PxBpos[0][2]];
				}
				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrP	=	Tv_PtrSrP + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_PtrTgP	=	Tv_PtrTgP + (Tv_SubpTg * 4);
				Tv_SubpTg	=	Tv_SubpTg ^ 1;
			}

			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------




