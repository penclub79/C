/*
	gdib byte unpack mode

*/

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrGdibByt.h>
#include	<GrImgBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrMath.h>
#include	<stdlib.h>

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
Cls_GrGdibByt::Cls_GrGdibByt( __u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
															__u32 A_Pitch, void* A_PtrDib, 
															__u32 A_PltBitCnt, void* A_PtrPlt ):
Cls_GrGdib( A_Fcc, A_ResX, A_ResY, A_Pitch, A_PtrDib, A_PltBitCnt, A_PtrPlt )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrGdibBytPxEleCnt;Tv_WkIdx ++ )
		{
			//byte struct index
			m_PxBpos[Tv_WkIdx]		=	m_PtrPxSt->PxSt[0][Tv_WkIdx].Pos / 8;
		}
		m_PxByteCnt	=	m_Gdib.BitCnt / 8;
}
//--------------------------------------------------------------------
Cls_GrGdibByt::~Cls_GrGdibByt()
{
}
//--------------------------------------------------------------------
void		Cls_GrGdibByt::ProcSetPixelNpt( __s32 A_X, __s32 A_Y, __u32 A_PxVal )
{
	// local -------------------
		__u8*	Tv_PtrPx;		
		__u8*	Tv_PtrVal;
	// code --------------------
		//calculate address
		Tv_PtrPx	=	(__u8*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_Y) + (m_PxByteCnt * (__u32)A_X) );
		//set pixel
		Tv_PtrVal		=	(__u8*)&A_PxVal;
		Tv_PtrPx[m_PxBpos[0]]		=	Tv_PtrVal[m_PxBpos[0]];
		Tv_PtrPx[m_PxBpos[1]]		=	Tv_PtrVal[m_PxBpos[1]];
		Tv_PtrPx[m_PxBpos[2]]		=	Tv_PtrVal[m_PxBpos[2]];
}
//--------------------------------------------------------------------
__u32		Cls_GrGdibByt::ProcGetPixelNpt( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		__u32		Tv_Result;
		__u8*		Tv_PtrPx;		
		__u8*		Tv_PtrRslt;
	// code --------------------
		//init
		Tv_Result		=	0;
		Tv_PtrRslt	=	(__u8*)&Tv_Result;
		//calculate address
		Tv_PtrPx	=	(__u8*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_Y) + (m_PxByteCnt * (__u32)A_X) );
		//get pixel
		Tv_PtrRslt[m_PxBpos[0]]		=	Tv_PtrPx[m_PxBpos[0]];
		Tv_PtrRslt[m_PxBpos[1]]		=	Tv_PtrPx[m_PxBpos[1]];
		Tv_PtrRslt[m_PxBpos[2]]		=	Tv_PtrPx[m_PxBpos[2]];

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		Cls_GrGdibByt::ProcFillRectNpt( Ptr_GrRect A_PtrRect, __u32 A_PxVal )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrWkY;
		__u8*		Tv_PtrPx;
		__u8*		Tv_PtrVal;
	// code --------------------
		//init
		Tv_PtrVal		=	(__u8*)&A_PxVal;

		//calcluate address
		Tv_PtrWkY	=	(void*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_PtrRect->top) + (m_PxByteCnt * (__u32)A_PtrRect->left) );

		//work vertical
		for ( Tv_WkY = (__u32)A_PtrRect->top;Tv_WkY < (__u32)A_PtrRect->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrPx	=	(__u8*)Tv_PtrWkY;
			for ( Tv_WkX = (__u32)A_PtrRect->left;Tv_WkX < (__u32)A_PtrRect->right;Tv_WkX++ )
			{
				//write
				Tv_PtrPx[m_PxBpos[0]]		=	Tv_PtrVal[m_PxBpos[0]];
				Tv_PtrPx[m_PxBpos[1]]		=	Tv_PtrVal[m_PxBpos[1]];
				Tv_PtrPx[m_PxBpos[2]]		=	Tv_PtrVal[m_PxBpos[2]];
				//next
				Tv_PtrPx	=	Tv_PtrPx + m_PxByteCnt;
			}

			//next
			Tv_PtrWkY	=	(void*)( (__u32)Tv_PtrWkY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdibByt::ProcBltNrmNpt2Npt( __s32 A_X, __s32 A_Y,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;
		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													(m_Gdib.Pitch * A_PtrRectSr->top) + 
													(m_PxByteCnt * (__u32)A_PtrRectSr->left) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( m_PxByteCnt * A_X ) );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			for ( Tv_WkX= (__u32)A_PtrRectSr->left;Tv_WkX < (__u32)A_PtrRectSr->right;Tv_WkX++ )
			{
				//copy pixel
				Tv_PtrTgP[m_PxBpos[0]]	=	Tv_PtrSrP[m_PxBpos[0]];
				Tv_PtrTgP[m_PxBpos[1]]	=	Tv_PtrSrP[m_PxBpos[1]];
				Tv_PtrTgP[m_PxBpos[2]]	=	Tv_PtrSrP[m_PxBpos[2]];

				//next
				Tv_PtrSrP	=	Tv_PtrSrP + m_PxByteCnt;
				Tv_PtrTgP	=	Tv_PtrTgP + m_PxByteCnt;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdibByt::ProcBltSrkNpt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;
		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													(m_Gdib.Pitch * A_PtrRectSr->top) + 
													(m_PxByteCnt * (__u32)A_PtrRectSr->left) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( m_PxByteCnt * A_X ) );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			for ( Tv_WkX= (__u32)A_PtrRectSr->left;Tv_WkX < (__u32)A_PtrRectSr->right;Tv_WkX++ )
			{
				//check source key
				if (	(0 != Tv_PtrSrP[m_PxBpos[0]]) || 
							(0 != Tv_PtrSrP[m_PxBpos[1]]) ||		
							(0 != Tv_PtrSrP[m_PxBpos[2]])  )
				{
					//copy pixel
					Tv_PtrTgP[m_PxBpos[0]]	=	Tv_PtrSrP[m_PxBpos[0]];
					Tv_PtrTgP[m_PxBpos[1]]	=	Tv_PtrSrP[m_PxBpos[1]];
					Tv_PtrTgP[m_PxBpos[2]]	=	Tv_PtrSrP[m_PxBpos[2]];
				}

				//next
				Tv_PtrSrP	=	Tv_PtrSrP + m_PxByteCnt;
				Tv_PtrTgP	=	Tv_PtrTgP + m_PxByteCnt;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdibByt::ProcBltNrmPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
	__u32		Tv_WkY;
	__u32		Tv_WkX;
	void*		Tv_PtrSrY;
	void*		Tv_PtrTgY;

	__u32		Tv_SrHzBitPos;
	__u8		Tv_PidxMask;
	__u32*	Tv_PtrPlt;
	__u8*		Tv_PtrWkPlt;
	__u8		Tv_PltIdx;

	__u32		Tv_SubpSr;

	__u8*		Tv_PtrSrP;
	__u8*		Tv_PtrTgP;
	// code --------------------
	//calculate address
	Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
												( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
												( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
	Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
												( m_Gdib.Pitch * A_Y ) +
												( m_PxByteCnt * A_X ) );
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
		for ( Tv_WkX=(__u32)A_PtrRectSr->left;Tv_WkX < (__u32)A_PtrRectSr->right;Tv_WkX++ )
		{
			//get pallete value
			Tv_PltIdx		=	((*Tv_PtrSrP >> Tv_SubpSr) & Tv_PidxMask);
			Tv_PtrWkPlt	=	(__u8*)( &(Tv_PtrPlt[Tv_PltIdx]) );
			//copy pixel
			Tv_PtrTgP[m_PxBpos[0]]	=	Tv_PtrWkPlt[m_PxBpos[0]];
			Tv_PtrTgP[m_PxBpos[1]]	=	Tv_PtrWkPlt[m_PxBpos[1]];
			Tv_PtrTgP[m_PxBpos[2]]	=	Tv_PtrWkPlt[m_PxBpos[2]];

			//next
			Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
			Tv_PtrSrP	=	Tv_PtrSrP + ( (Tv_SubpSr >> 3) & 1 );
			Tv_SubpSr	=	Tv_SubpSr & 0x07;
			Tv_PtrTgP	=	Tv_PtrTgP + m_PxByteCnt;
		}
		//next
		Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
		Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
	}
}
//--------------------------------------------------------------------
void		Cls_GrGdibByt::ProcBltSrkPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SrHzBitPos;
		__u8		Tv_PidxMask;
		__u32*	Tv_PtrPlt;
		__u8*		Tv_PtrWkPlt;
		__u8		Tv_PltIdx;

		__u32		Tv_SubpSr;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
													( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( m_PxByteCnt * A_X ) );
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
			for ( Tv_WkX=(__u32)A_PtrRectSr->left;Tv_WkX < (__u32)A_PtrRectSr->right;Tv_WkX++ )
			{
				//get pallete value
				Tv_PltIdx		=	((*Tv_PtrSrP >> Tv_SubpSr) & Tv_PidxMask);
				Tv_PtrWkPlt	=	(__u8*)( &(Tv_PtrPlt[Tv_PltIdx]) );
				//check source key
				if ( 0 != Tv_PtrPlt[Tv_PltIdx] )
				{
					//copy pixel
					Tv_PtrTgP[m_PxBpos[0]]	=	Tv_PtrWkPlt[m_PxBpos[0]];
					Tv_PtrTgP[m_PxBpos[1]]	=	Tv_PtrWkPlt[m_PxBpos[1]];
					Tv_PtrTgP[m_PxBpos[2]]	=	Tv_PtrWkPlt[m_PxBpos[2]];
				}

				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrP	=	Tv_PtrSrP + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_PtrTgP	=	Tv_PtrTgP + m_PxByteCnt;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibByt::ProcSthNpt2Npt(	Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, 
																		Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

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
													( m_PxByteCnt * A_PtrRcCrntTg->left ) );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRcCrntTg->top;Tv_WkY < (__u32)A_PtrRcCrntTg->bottom;Tv_WkY++ )
		{
			//init
			Tv_SrY	=	A_PtrRcSr->top + ( ((Tv_WkY - A_PtrRcTg->top) * Tv_SrRtY ) / Tv_TgRtY );
			Tv_PtrSrY	=	(void*)(	(__u32)Tv_PtrDibSr + 
														(A_PtrGdibSr->Pitch * Tv_SrY ) );
			Tv_PtrTgP	=	(__u8*)Tv_PtrTgY;
			for ( Tv_WkX=(__u32)A_PtrRcCrntTg->left;Tv_WkX < (__u32)A_PtrRcCrntTg->right;Tv_WkX++ )
			{
				//get source x position
				Tv_SrX	=	A_PtrRcSr->left + ( ((Tv_WkX - A_PtrRcTg->left) * Tv_SrRtX ) / Tv_TgRtX );
				//get source address
				Tv_PtrSrP	=	(__u8*)( (__u32)Tv_PtrSrY + ( m_PxByteCnt * Tv_SrX ) );
				//write
				Tv_PtrTgP[m_PxBpos[0]]	=	Tv_PtrSrP[m_PxBpos[0]];
				Tv_PtrTgP[m_PxBpos[1]]	=	Tv_PtrSrP[m_PxBpos[1]];
				Tv_PtrTgP[m_PxBpos[2]]	=	Tv_PtrSrP[m_PxBpos[2]];
				//next
				Tv_PtrTgP		=	Tv_PtrTgP + m_PxByteCnt;
			}
			//next
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibByt::ProcSthPlt2Npt( Ptr_GrRect A_PtrRcTg,Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRcSr, Ptr_GrRect A_PtrRcCrntTg )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SubpSr;

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
								( m_PxByteCnt * A_PtrRcCrntTg->left ) );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRcCrntTg->top;Tv_WkY < (__u32)A_PtrRcCrntTg->bottom;Tv_WkY++ )
		{
			//get source y position
			Tv_SrY	=	A_PtrRcSr->top + ( ((Tv_WkY - A_PtrRcTg->top) * Tv_SrRtY ) / Tv_TgRtY );
			//get souce y addr
			Tv_PtrSrY	=	(void*)(	(__u32)Tv_PtrDibSr + ( A_PtrGdibSr->Pitch * Tv_SrY ) );
			Tv_PtrTgP	=	(__u8*)Tv_PtrTgY;
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
				Tv_PtrTgP[m_PxBpos[0]]	=	Tv_PtrWkPlt[m_PxBpos[0]];
				Tv_PtrTgP[m_PxBpos[1]]	=	Tv_PtrWkPlt[m_PxBpos[1]];
				Tv_PtrTgP[m_PxBpos[2]]	=	Tv_PtrWkPlt[m_PxBpos[2]];
				//next
				Tv_PtrTgP	=	Tv_PtrTgP + m_PxByteCnt;
			}
			//next
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibByt::ProcFntNrmNpt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SrHzBitPos;
		__u8		Tv_PidxMask;
		__u32*	Tv_PtrPlt;
		__u8*		Tv_PtrWkPlt;
		__u8		Tv_PltIdx;

		__u32		Tv_SubpSr;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
								( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
								( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
								( m_Gdib.Pitch * A_Y ) +
								( m_PxByteCnt * A_X ) );
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
			for ( Tv_WkX=(__u32)A_PtrRectSr->left;Tv_WkX < (__u32)A_PtrRectSr->right;Tv_WkX++ )
			{
				//get pallete value
				Tv_PltIdx		=	((*Tv_PtrSrP >> Tv_SubpSr) & Tv_PidxMask);
				Tv_PtrWkPlt	=	(__u8*)( &(Tv_PtrPlt[Tv_PltIdx]) );
				//check source key
				if ( E_GrGdibFontTextPltIdx == Tv_PltIdx )
				{
					//copy pixel
					Tv_PtrTgP[m_PxBpos[0]]	=	Tv_PtrWkPlt[m_PxBpos[0]];
					Tv_PtrTgP[m_PxBpos[1]]	=	Tv_PtrWkPlt[m_PxBpos[1]];
					Tv_PtrTgP[m_PxBpos[2]]	=	Tv_PtrWkPlt[m_PxBpos[2]];
				}

				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrP	=	Tv_PtrSrP + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_PtrTgP	=	Tv_PtrTgP + m_PxByteCnt;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibByt::ProcFntSdwNpt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u32		Tv_SrHzBitPos;
		__u8		Tv_PidxMask;
		__u32*	Tv_PtrPlt;
		__u8*		Tv_PtrWkPlt;
		__u8		Tv_PltIdx;

		__u32		Tv_SubpSr;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
								( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
								( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
								( m_Gdib.Pitch * A_Y ) +
								( m_PxByteCnt * A_X ) );
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
			for ( Tv_WkX=(__u32)A_PtrRectSr->left;Tv_WkX < (__u32)A_PtrRectSr->right;Tv_WkX++ )
			{
				//get pallete value
				Tv_PltIdx		=	((*Tv_PtrSrP >> Tv_SubpSr) & Tv_PidxMask);
				Tv_PtrWkPlt	=	(__u8*)( &(Tv_PtrPlt[Tv_PltIdx]) );
				//check source key
				if ( 0 != Tv_PltIdx )
				{
					//copy pixel
					Tv_PtrTgP[m_PxBpos[0]]	=	Tv_PtrWkPlt[m_PxBpos[0]];
					Tv_PtrTgP[m_PxBpos[1]]	=	Tv_PtrWkPlt[m_PxBpos[1]];
					Tv_PtrTgP[m_PxBpos[2]]	=	Tv_PtrWkPlt[m_PxBpos[2]];
				}

				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrP	=	Tv_PtrSrP + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_PtrTgP	=	Tv_PtrTgP + m_PxByteCnt;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------

