/*
 gdib pack mode 

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrGdibUyvy.h>
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
Cls_GrGdibUyvy::Cls_GrGdibUyvy( __u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
																__u32 A_Pitch, void* A_PtrDib, 
																__u32 A_PltBitCnt, void* A_PtrPlt ):
Cls_GrGdibPk4( A_Fcc, A_ResX, A_ResY, A_Pitch, A_PtrDib, A_PltBitCnt, A_PtrPlt )
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
Cls_GrGdibUyvy::~Cls_GrGdibUyvy()
{
}
//--------------------------------------------------------------------
void		Cls_GrGdibUyvy::ProcBltNrmPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_CntX;
		__u32		Tv_CntY;
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;
		__u32		Tv_SrHzBitPos;
		__u8		Tv_PidxMask;
		__u16*		Tv_PtrPlt;
		__u8*		Tv_PtrSrX;
		__u16*		Tv_PtrTgX;
		__u32		Tv_PltIdx;
		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;
	// code --------------------
		//calculate count
		Tv_CntX			=	A_PtrRectSr->right - A_PtrRectSr->left;
		Tv_CntY			=	A_PtrRectSr->bottom - A_PtrRectSr->top;
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
													( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( A_X << 1 ) );
		Tv_SrHzBitPos	=	0x07 & (A_PtrRectSr->left * A_PtrGdibSr->BitCnt);
		Tv_PidxMask		=	(__u8)((__u16)(1 << A_PtrGdibSr->BitCnt) - 1);
		Tv_PtrPlt			=	(__u16*)GrGdibGetPalletPtr( A_PtrGdibSr );

		//work y
		for ( Tv_WkY =Tv_CntY;0 != Tv_WkY;Tv_WkY-- )
		{
			//init
			Tv_PtrSrX			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgX			=	(__u16*)Tv_PtrTgY;
			Tv_SubpSr			=	Tv_SrHzBitPos;
			Tv_SubpTg			=	1 & A_X;
			for ( Tv_WkX=Tv_CntX;0 != Tv_WkX;Tv_WkX-- )
			{
				//get pallete value
				Tv_PltIdx		=	(__u32)((*Tv_PtrSrX >> Tv_SubpSr) & Tv_PidxMask);
				Tv_PltIdx		=	( (Tv_PltIdx << 1) + Tv_SubpTg );
				//copy pixel
				*Tv_PtrTgX	=	Tv_PtrPlt[Tv_PltIdx];
				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrX	=	Tv_PtrSrX + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_PtrTgX	++;
				Tv_SubpTg	=	Tv_SubpTg ^ 1;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void		Cls_GrGdibUyvy::ProcBltSrkPlt2Npt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_CntX;
		__u32		Tv_CntY;
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;
		__u32		Tv_SrHzBitPos;
		__u8		Tv_PidxMask;
		__u16*		Tv_PtrPlt;
		__u8*		Tv_PtrSrX;
		__u16*		Tv_PtrTgX;
		__u32		Tv_PltIdx;
		__u32		Tv_SubpSr;
		__u32		Tv_SubpTg;
	// code --------------------
		//calculate count
		Tv_CntX			=	A_PtrRectSr->right - A_PtrRectSr->left;
		Tv_CntY			=	A_PtrRectSr->bottom - A_PtrRectSr->top;
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + 
													( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) +
													( (A_PtrRectSr->left * A_PtrGdibSr->BitCnt) >> 3 ) );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + 
													( m_Gdib.Pitch * A_Y ) +
													( A_X << 1 ) );
		Tv_SrHzBitPos	=	0x07 & (A_PtrRectSr->left * A_PtrGdibSr->BitCnt);
		Tv_PidxMask		=	(__u8)((__u16)(1 << A_PtrGdibSr->BitCnt) - 1);
		Tv_PtrPlt			=	(__u16*)GrGdibGetPalletPtr( A_PtrGdibSr );

		//work y
		for ( Tv_WkY =Tv_CntY;0 != Tv_WkY;Tv_WkY-- )
		{
			//init
			Tv_PtrSrX			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgX			=	(__u16*)Tv_PtrTgY;
			Tv_SubpSr			=	Tv_SrHzBitPos;
			Tv_SubpTg			=	1 & A_X;
			for ( Tv_WkX=Tv_CntX;0 != Tv_WkX;Tv_WkX-- )
			{
				//get pallete value
				Tv_PltIdx		=	(__u32)((*Tv_PtrSrX >> Tv_SubpSr) & Tv_PidxMask);
				//check source key
				if ( 0 != Tv_PltIdx )
				{
					//patch pallete index
					Tv_PltIdx		=	( (Tv_PltIdx << 1) + Tv_SubpTg );
					//copy pixel
					*Tv_PtrTgX	=	Tv_PtrPlt[Tv_PltIdx];
				}
				
				//next
				Tv_SubpSr	=	Tv_SubpSr + A_PtrGdibSr->BitCnt;
				Tv_PtrSrX	=	Tv_PtrSrX + ( (Tv_SubpSr >> 3) & 1 );
				Tv_SubpSr	=	Tv_SubpSr & 0x07;
				Tv_PtrTgX	++;
				Tv_SubpTg	=	Tv_SubpTg ^ 1;
			}
			//next
			Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibUyvy::ProcFntNrm( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, __u32 A_ClrTxt, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		void*		Tv_PtrFtY;
		__u8*		Tv_PtrFtX;
		__u8		Tv_BposInit;
		__s32			Tv_WkX;
		__s32			Tv_WkY;
		__u8		Tv_BitPos;
		__u8		Tv_ClrIdx;
		void*		Tv_PtrTgY;
		__u16*		Tv_PtrTgX;
		__u32		Tv_SubPos;
		__u32		Tv_WkSub;
		__u16*		Tv_PtrClr;
	// code --------------------
		//calculate start address
		Tv_PtrFtY		=	(void*)( (__u32)GrGdibGetDibPtr( A_PtrGdibSr ) + (A_PtrRectSr->top * A_PtrGdibSr->Pitch ) );
		Tv_PtrFtY		=	(void*)( (__u32)Tv_PtrFtY + ( A_PtrRectSr->left >> 2 ) );
		Tv_BposInit	=	(A_PtrRectSr->left & 3) << 1;
		Tv_PtrTgY		=	(void*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * (__u32)A_Y) + ((__u32)A_X << 1) );
		Tv_SubPos		=	A_X & 1;
		Tv_PtrClr		=	(__u16*)&A_ClrTxt;
		for ( Tv_WkY = A_PtrRectSr->top;Tv_WkY < A_PtrRectSr->bottom;Tv_WkY++ )
		{
			Tv_BitPos	=	Tv_BposInit;
			Tv_PtrFtX	=	(__u8*)Tv_PtrFtY;
			Tv_PtrTgX	=	(__u16*)Tv_PtrTgY;
			Tv_WkSub	=	Tv_SubPos;
			for ( Tv_WkX = A_PtrRectSr->left;Tv_WkX < A_PtrRectSr->right;Tv_WkX ++ )
			{
				//get pixel
				Tv_ClrIdx	=	( *Tv_PtrFtX >> Tv_BitPos ) & 3;
				if ( 1 == Tv_ClrIdx )
				{
					//draw
					*Tv_PtrTgX	=	Tv_PtrClr[Tv_WkSub];
				}
				//next
				Tv_BitPos	=	Tv_BitPos + 2;
				Tv_PtrFtX	=	Tv_PtrFtX + ((Tv_BitPos >> 3) & 1);
				Tv_BitPos	=	Tv_BitPos & 7;
				Tv_WkSub	=	Tv_WkSub ^ 1;
				Tv_PtrTgX ++;
			}
			//next
			Tv_PtrFtY	=	(void*)( (__u32)Tv_PtrFtY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibUyvy::ProcFntSdw( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, __u32 A_ClrTxt, __u32 A_ClrSdw, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		void*		Tv_PtrFtY;
		__u8*		Tv_PtrFtX;
		__u8		Tv_BposInit;
		__s32			Tv_WkX;
		__s32			Tv_WkY;
		__u8		Tv_BitPos;
		__u8		Tv_ClrIdx;
		void*		Tv_PtrTgY;
		__u16*		Tv_PtrTgX;
		__u32		Tv_SubPos;
		__u32		Tv_WkSub;
		__u16*		Tv_PtrClrTxt;
		__u16*		Tv_PtrClrSdw;
	// code --------------------
		//calculate start address
		Tv_PtrFtY		=	(void*)( (__u32)GrGdibGetDibPtr( A_PtrGdibSr ) + (A_PtrRectSr->top * A_PtrGdibSr->Pitch ) );
		Tv_PtrFtY		=	(void*)( (__u32)Tv_PtrFtY + ( A_PtrRectSr->left >> 2 ) );
		Tv_BposInit	=	(A_PtrRectSr->left & 3) << 1;
		Tv_PtrTgY		=	(void*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * (__u32)A_Y) + ((__u32)A_X << 1) );
		Tv_SubPos		=	A_X & 1;
		Tv_PtrClrTxt		=	(__u16*)&A_ClrTxt;
		Tv_PtrClrSdw		=	(__u16*)&A_ClrSdw;
		for ( Tv_WkY = A_PtrRectSr->top;Tv_WkY < A_PtrRectSr->bottom;Tv_WkY++ )
		{
			Tv_BitPos	=	Tv_BposInit;
			Tv_PtrFtX	=	(__u8*)Tv_PtrFtY;
			Tv_PtrTgX	=	(__u16*)Tv_PtrTgY;
			Tv_WkSub	=	Tv_SubPos;
			for ( Tv_WkX = A_PtrRectSr->left;Tv_WkX < A_PtrRectSr->right;Tv_WkX ++ )
			{
				//get pixel
				Tv_ClrIdx	=	( *Tv_PtrFtX >> Tv_BitPos ) & 3;
				if ( 1 == Tv_ClrIdx )
				{
					//draw
					*Tv_PtrTgX	=	Tv_PtrClrTxt[Tv_WkSub];
				}
				else if ( 2 == Tv_ClrIdx )
				{
					//draw
					*Tv_PtrTgX	=	Tv_PtrClrSdw[Tv_WkSub];
				}
				
				//next
				Tv_BitPos	=	Tv_BitPos + 2;
				Tv_PtrFtX	=	Tv_PtrFtX + ((Tv_BitPos >> 3) & 1);
				Tv_BitPos	=	Tv_BitPos & 7;
				Tv_WkSub	=	Tv_WkSub ^ 1;
				Tv_PtrTgX ++;
			}
			//next
			Tv_PtrFtY	=	(void*)( (__u32)Tv_PtrFtY + A_PtrGdibSr->Pitch );
			Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibUyvy::ProcSetPixelNpt( __s32 A_X, __s32 A_Y, volatile __u32 A_PxVal )
{
	// local -------------------
		__u16*		Tv_PtrPx;		
		__u32		Tv_SubPos;
		__u16*		Tv_PtrSr;
	// code --------------------
		//calculate address
		Tv_PtrPx	=	(__u16*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_Y) + (2 * (__u32)A_X ) );
		Tv_PtrSr	=	(__u16*)&A_PxVal;
		Tv_SubPos	=	A_X & 1;
		//set pixel
		*Tv_PtrPx	=	Tv_PtrSr[Tv_SubPos];
}
//--------------------------------------------------------------------
void	Cls_GrGdibUyvy::ProcFillRectNpt( Ptr_GrRect A_PtrRect, __u32 A_PxVal )
{
	// local -------------------
		__u32		Tv_WkY;
		BOOL8		Tv_IsSgHd;		//single head
		BOOL8		Tv_IsSgTl;		//single tail
		__u32		Tv_MidCnt;		//middle pixel count
		__u32*	Tv_PtrPx;
		void*		Tv_PtrPos;
		__u16*		Tv_PtrSr;
		__u32		Tv_WkIdx;
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
		Tv_PtrSr	=	(__u16*)&A_PxVal;

		//calcluate address
		Tv_PtrPos	=	(__u32*)( (__u32)m_Gdib.PtrExtDib + (m_Gdib.Pitch * A_PtrRect->top) + ((__u32)A_PtrRect->left << 1 ) );

		//work vertical
		for ( Tv_WkY = (__u32)A_PtrRect->top;Tv_WkY < (__u32)A_PtrRect->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrPx	=	(__u32*)Tv_PtrPos;
			//single head
			if ( Tv_IsSgHd )
			{
				*((__u16*)Tv_PtrPx)	=	Tv_PtrSr[1];
				Tv_PtrPx	=	(__u32*)( (__u32)Tv_PtrPx + 2 );
			}
			//middle 
			if ( 0 != Tv_MidCnt )
			{
				for ( Tv_WkIdx=Tv_MidCnt;0 != Tv_WkIdx;Tv_WkIdx-- )
				{
					*Tv_PtrPx	=	A_PxVal;
					Tv_PtrPx ++;
				}
			}
			//single tail
			if ( Tv_IsSgTl )
			{
				*((__u16*)Tv_PtrPx)	=	Tv_PtrSr[0];
			}
			//next
			Tv_PtrPos	=	(void*)( (__u32)Tv_PtrPos + m_Gdib.Pitch );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibUyvy::ProcAlphaPltRemove( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_Plt;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < m_RtlPltCnt;Tv_WkIdx++ )
		{
			Tv_Plt	=	PalleteGet( Tv_WkIdx );
			Tv_Plt	=	Tv_Plt & 0xFEFFFEFF;
			PalleteSet( Tv_WkIdx, Tv_Plt );
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibUyvy::ProcAlphaPltSet( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_Plt;
	// code --------------------
		for ( Tv_WkIdx=1;Tv_WkIdx < m_RtlPltCnt;Tv_WkIdx++ )
		{
			Tv_Plt	=	PalleteGet( Tv_WkIdx );
			Tv_Plt	=	Tv_Plt | 0x01000100;
			PalleteSet( Tv_WkIdx, Tv_Plt );
		}
}
//--------------------------------------------------------------------

