/*
	gdib pallete 8 bit 

*/

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrGdibPlt8.h>
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
Cls_GrGdibPlt8::Cls_GrGdibPlt8( __u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
															__u32 A_Pitch, void* A_PtrDib, 
															void* A_PtrPlt ):
Cls_GrGdib( A_Fcc, A_ResX, A_ResY, A_Pitch, A_PtrDib, 8, A_PtrPlt )
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
Cls_GrGdibPlt8::~Cls_GrGdibPlt8()
{
}
//--------------------------------------------------------------------
void	Cls_GrGdibPlt8::ProcCvtPlt2Npt( Cls_GrGdib* A_ObjGdib, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		__u32		Tv_PxlByte;		
		__u32		Tv_TgPitch;
		void*		Tv_PtrTgY;
		__u8*		Tv_PtrTgX1;
		__u16*		Tv_PtrTgX2;
		__u32*	Tv_PtrTgX4;
		void*		Tv_PtrSrY;
		__u8*		Tv_PtrSrX;
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		__u32		Tv_TgResY;
		__u32		Tv_TgResX;
		__u32*	Tv_PtrPlt;
		__u32		Tv_FccTg;
		__u32		Tv_PltVal;
	// code --------------------
		//get dest type
		if ( GrDrawIsCoupleFormat( A_ObjGdib->GetFcc() ) )
		{
			//couple foramt
			Cls_GrGdib::ProcCvtPlt2Npt( A_ObjGdib, A_PtrRect );
		}
		else
		{
			//make pallete
			Tv_FccTg	=	A_ObjGdib->GetFcc();
			if ( m_Gdib.FccImg == Tv_FccTg )
			{
				Tv_PtrPlt		=	m_PtrRtlPlt;
			}
			else
			{
				for ( Tv_WkX = 0;Tv_WkX < 256;Tv_WkX++ )
				{
					m_CvtPlt[Tv_WkX]	=	GrDrawCvtColorByFcc( m_Gdib.FccImg, m_PtrRtlPlt[Tv_WkX], Tv_FccTg );
				}
				Tv_PtrPlt	=	m_CvtPlt;
			}
			//get info
			Tv_PxlByte	=	GrDrawGetPixelByteByFcc( Tv_FccTg );
			Tv_PtrSrY		=	m_Gdib.PtrExtDib;
			Tv_PtrTgY		=	A_ObjGdib->GetDibPtr();
			Tv_TgPitch	=	A_ObjGdib->GetDibPitch();
			Tv_TgResX		=	A_ObjGdib->GetResX();
			Tv_TgResY		=	A_ObjGdib->GetResY();
			switch( Tv_PxlByte )
			{
				case	2:
					Tv_WkY	=	0;
					while ( (m_Gdib.ResY > Tv_WkY) && (Tv_TgResY > Tv_WkY) )
					{
						Tv_WkX	=	0;
						Tv_PtrSrX		=	(__u8*)Tv_PtrSrY;
						Tv_PtrTgX2	=	(__u16*)Tv_PtrTgY;
						while ( (m_Gdib.ResX > Tv_WkX) && (Tv_TgResX > Tv_WkX) )
						{
							*Tv_PtrTgX2	=	(__u16)Tv_PtrPlt[*Tv_PtrSrX];
							//next
							Tv_PtrSrX ++;
							Tv_PtrTgX2 ++;
							Tv_WkX ++;
						}
						//next
						Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + m_Gdib.Pitch );
						Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + Tv_TgPitch );
						Tv_WkY ++;
					}
					break;
				case	3:
					Tv_WkY	=	0;
					while ( (m_Gdib.ResY > Tv_WkY) && (Tv_TgResY > Tv_WkY) )
					{
						Tv_WkX	=	0;
						Tv_PtrSrX		=	(__u8*)Tv_PtrSrY;
						Tv_PtrTgX1	=	(__u8*)Tv_PtrTgY;
						while ( (m_Gdib.ResX > Tv_WkX) && (Tv_TgResX > Tv_WkX) )
						{
							Tv_PltVal		=	Tv_PtrPlt[*Tv_PtrSrX];
							Tv_PtrTgX1[0]	=	(__u8)( Tv_PltVal & 0xFF );
							Tv_PtrTgX1[1]	=	(__u8)( (Tv_PltVal >> 8 ) & 0xFF );
							Tv_PtrTgX1[2]	=	(__u8)( (Tv_PltVal >> 16 ) & 0xFF );
							//next
							Tv_PtrSrX ++;
							Tv_PtrTgX1		=	(__u8*)( (__u32)Tv_PtrTgX1 + 3 );
							Tv_WkX ++;
						}
						//next
						Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + m_Gdib.Pitch );
						Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + Tv_TgPitch );
						Tv_WkY ++;
					}
					break;
				case	4:
					Tv_WkY	=	0;
					while ( (m_Gdib.ResY > Tv_WkY) && (Tv_TgResY > Tv_WkY) )
					{
						Tv_WkX	=	0;
						Tv_PtrSrX		=	(__u8*)Tv_PtrSrY;
						Tv_PtrTgX4	=	(__u32*)Tv_PtrTgY;
						while ( (m_Gdib.ResX > Tv_WkX) && (Tv_TgResX > Tv_WkX) )
						{
							*Tv_PtrTgX4	=	Tv_PtrPlt[*Tv_PtrSrX];
							//next
							Tv_PtrSrX ++;
							Tv_PtrTgX4 ++;
							Tv_WkX ++;
						}
						//next
						Tv_PtrSrY	=	(void*)( (__u32)Tv_PtrSrY + m_Gdib.Pitch );
						Tv_PtrTgY	=	(void*)( (__u32)Tv_PtrTgY + Tv_TgPitch );
						Tv_WkY ++;
					}
					break;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibPlt8::ProcBltNrmPlt2Plt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		__u32		Tv_CntHz;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//acc count
		Tv_CntHz		=	A_PtrRectSr->right - A_PtrRectSr->left;
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + ( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) + A_PtrRectSr->left );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + ( m_Gdib.Pitch * A_Y ) + A_X );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			for ( Tv_WkX=0;Tv_WkX < Tv_CntHz;Tv_WkX++ )
			{
				//write pixel
				*Tv_PtrTgP	=	*Tv_PtrSrP;

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
void	Cls_GrGdibPlt8::ProcBltSrkPlt2Plt( __s32 A_X, __s32 A_Y, Ptr_GrGdib A_PtrGdibSr, Ptr_GrRect A_PtrRectSr )
{
	// local -------------------
		__u32		Tv_WkY;
		__u32		Tv_WkX;
		__u32		Tv_CntHz;
		void*		Tv_PtrSrY;
		void*		Tv_PtrTgY;

		__u8*		Tv_PtrSrP;
		__u8*		Tv_PtrTgP;
	// code --------------------
		//acc count
		Tv_CntHz		=	A_PtrRectSr->right - A_PtrRectSr->left;
		//calculate address
		Tv_PtrSrY	=	(void*)(	(__u32)GrGdibGetDibPtr(A_PtrGdibSr) + ( A_PtrGdibSr->Pitch * A_PtrRectSr->top ) + A_PtrRectSr->left );
		Tv_PtrTgY	=	(void*)(	(__u32)m_Gdib.PtrExtDib + ( m_Gdib.Pitch * A_Y ) + A_X );
		//work y
		for ( Tv_WkY =(__u32)A_PtrRectSr->top;Tv_WkY < (__u32)A_PtrRectSr->bottom;Tv_WkY++ )
		{
			//init
			Tv_PtrSrP			=	(__u8*)Tv_PtrSrY;
			Tv_PtrTgP			=	(__u8*)Tv_PtrTgY;
			for ( Tv_WkX=0;Tv_WkX < Tv_CntHz;Tv_WkX++ )
			{
				//write pixel
				//check source key
				if ( 0 != *Tv_PtrSrP )
				{
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
void	Cls_GrGdibPlt8::ProcAlphaPltRemove( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_Plt;
	// code --------------------
		if ( E_GrFccUYVYF == m_Gdib.FccImg )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < m_RtlPltCnt;Tv_WkIdx++ )
			{
				Tv_Plt	=	PalleteGet( Tv_WkIdx );
				Tv_Plt	=	Tv_Plt & 0xFEFFFEFF;
				PalleteSet( Tv_WkIdx, Tv_Plt );
			}
		}
		else
		{
			Cls_GrGdib::ProcAlphaPltRemove();
		}
}
//--------------------------------------------------------------------
void	Cls_GrGdibPlt8::ProcAlphaPltSet( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_Plt;
	// code --------------------
		if ( E_GrFccUYVYF == m_Gdib.FccImg )
		{
			for ( Tv_WkIdx=1;Tv_WkIdx < m_RtlPltCnt;Tv_WkIdx++ )
			{
				Tv_Plt	=	PalleteGet( Tv_WkIdx );
				Tv_Plt	=	Tv_Plt | 0x01000100;
				PalleteSet( Tv_WkIdx, Tv_Plt );
			}
		}
		else
		{
			Cls_GrGdib::ProcAlphaPltSet();
		}
}
//--------------------------------------------------------------------

