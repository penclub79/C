/*
 gdib pack mode 

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrGdibPk4a.h>
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
Cls_GrGdibPk4a::Cls_GrGdibPk4a( __u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
																__u32 A_Pitch, void* A_PtrDib, 
																__u32 A_PltBitCnt, void* A_PtrPlt ):
Cls_GrGdibPk4( A_Fcc, A_ResX, A_ResY, A_Pitch, A_PtrDib, A_PltBitCnt, A_PtrPlt )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		m_RmAlphaMix		=	0;

		for ( Tv_WkIdx = 0;Tv_WkIdx < E_GrGdibPk4PxSubCnt;Tv_WkIdx++ )
		{
			m_PxAlpChk[Tv_WkIdx]	=	(__u32)0x01 << m_PtrPxSt->PxSt[Tv_WkIdx][0].Pos;
		}
}
//--------------------------------------------------------------------
Cls_GrGdibPk4a::~Cls_GrGdibPk4a()
{
}
//--------------------------------------------------------------------
void	Cls_GrGdibPk4a::ProcAlphaRemove( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__u32		Tv_WkX;
		__u32		Tv_WkY;
		void*		Tv_PtrTgY;
		__u32		Tv_SubpTg;

		__u32*	Tv_PtrPx;
	// code --------------------
		//init

		//calcluate address
		Tv_PtrTgY	=	(void*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_PtrRect->top) + (2 * ((__u32)A_PtrRect->left & ~1)) );

		//work vertical
		for ( Tv_WkY = (__u32)A_PtrRect->top;Tv_WkY < (__u32)A_PtrRect->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrPx	=	(__u32*)Tv_PtrTgY;
			Tv_SubpTg	=	1 & A_PtrRect->left;
			for ( Tv_WkX	=	(__u32)A_PtrRect->left;Tv_WkX < (__u32)A_PtrRect->right;Tv_WkX++ )
			{
				//remove alpha
				*Tv_PtrPx	=	*Tv_PtrPx	& ( 0xFEFEFEFE | (~m_PxMask[Tv_SubpTg]));	//remove alpha
				//next
				Tv_PtrPx	=	Tv_PtrPx + Tv_SubpTg;
				Tv_SubpTg	=	Tv_SubpTg ^ 1;
			}
			//next
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibPk4a::ProcAlphaPltRemove( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_Plt;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < m_RtlPltCnt;Tv_WkIdx++ )
		{
			Tv_Plt	=	PalleteGet( Tv_WkIdx );
			Tv_Plt	=	Tv_Plt & 0xFEFEFEFE;
			PalleteSet( Tv_WkIdx, Tv_Plt );
		}
}
//--------------------------------------------------------------------
