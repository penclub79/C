/*
 osd font object

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Ui/GrUiFontObj.h>
#include	<GrDrawTool.h>

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
Cls_GrUiFontObj::Cls_GrUiFontObj( Ptr_GrUiFontV2Head A_PtrFont )
{
	// local -------------------
	// code --------------------
		//check font
		if ( NULL != A_PtrFont )
		{
			//init
			m_Gdib.Pitch	=	A_PtrFont->DibPitch;

			m_Width			=	A_PtrFont->Width;
			m_CharCnt		=	A_PtrFont->CharCnt;

			//calculate mapping table
			m_PtrMapTbl		=	(Ptr_GrUiFontV2MapItm)( (__u32)A_PtrFont + sizeof(St_GrUiFontV2Head) );
			//calculate dib 
			m_PtrDib			=	(void*)( (__u32)m_PtrMapTbl + (m_CharCnt * sizeof(St_GrUiFontV2MapItm) ) );
			//make gdib
			GrGdibInitHead( &m_Gdib, E_GrFccFont, A_PtrFont->DibWidth, A_PtrFont->Height, A_PtrFont->DibPitch,
											m_PtrDib, E_GrUiFontDibBitCnt, (void*)E_GrGdibPltBase );
		}
}
//--------------------------------------------------------------------
Cls_GrUiFontObj::~Cls_GrUiFontObj()
{
}
//--------------------------------------------------------------------
__u32		Cls_GrUiFontObj::GetWidth( void )
{
		return	m_Width;
}
//--------------------------------------------------------------------
__u32		Cls_GrUiFontObj::GetHeight( void )
{
		return	m_Gdib.ResY;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiFontObj::GetCharCnt( void )
{
		return	m_CharCnt;
}
//--------------------------------------------------------------------
Ptr_GrGdib	Cls_GrUiFontObj::GetGdibPtr( void )
{
		return	&m_Gdib;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiFontObj::GetCharInfo( __u16 A_Code, __u32* A_PtrRtIdx, Ptr_GrRect A_PtrRtRcDib, __s32* A_PtrRtPitch, __s32* A_PtrRtGap )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrUiFontV2MapItm	Tv_PtrItm;
		__u32		Tv_WkIdx;
		__u32		Tv_DibPos;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check able
		if ( 0 != m_CharCnt )
		{
			Tv_PtrItm		=	m_PtrMapTbl;
			//find map info
			Tv_DibPos		=	0;
			for ( Tv_WkIdx=0;Tv_WkIdx < m_CharCnt;Tv_WkIdx++ )
			{
				//check match code
				if ( A_Code == Tv_PtrItm->Code )
				{
					*A_PtrRtIdx			=	Tv_WkIdx;
					A_PtrRtRcDib->left		=	(__s32)Tv_DibPos;
					A_PtrRtRcDib->top			=	0;
					A_PtrRtRcDib->right		=	A_PtrRtRcDib->left + (__s32)Tv_PtrItm->Pitch;
					A_PtrRtRcDib->bottom	=	(__s32)m_Gdib.ResY;
					*A_PtrRtPitch					=	(__s32)Tv_PtrItm->Pitch;
					*A_PtrRtGap						=	(__s32)Tv_PtrItm->StGap;
					Tv_Result	=	TRUE;
					break;
				}
				//next
				Tv_DibPos	=	Tv_DibPos + Tv_PtrItm->Pitch;
				Tv_PtrItm ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiFontObj::GetItemInfo( __u32 A_Idx, __u16* A_PtrRtCode, Ptr_GrRect A_PtrRtRcDib, __s32* A_PtrRtPitch, __s32* A_PtrRtGap )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrUiFontV2MapItm	Tv_PtrItm;
		__u32		Tv_WkIdx;
		__u32		Tv_DibPos;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check item info
		if ( m_CharCnt > A_Idx )
		{
			Tv_PtrItm		=	m_PtrMapTbl;
			//find map info
			Tv_DibPos		=	0;
			for ( Tv_WkIdx=0;Tv_WkIdx < m_CharCnt;Tv_WkIdx++ )
			{
				//check match code
				if ( Tv_WkIdx == A_Idx )
				{
					*A_PtrRtCode					=	Tv_PtrItm->Code;
					A_PtrRtRcDib->left		=	(__s32)Tv_DibPos;
					A_PtrRtRcDib->top			=	0;
					A_PtrRtRcDib->right		=	A_PtrRtRcDib->left + (__s32)Tv_PtrItm->Pitch;
					A_PtrRtRcDib->bottom	=	(__s32)m_Gdib.ResY;
					*A_PtrRtPitch					=	(__s32)Tv_PtrItm->Pitch;
					*A_PtrRtGap						=	(__s32)Tv_PtrItm->StGap;
					Tv_Result	=	TRUE;
					break;
				}
				//next
				Tv_DibPos	=	Tv_DibPos + Tv_PtrItm->Pitch;
				Tv_PtrItm ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiFontObj::IsCodeExist( __u16 A_Code )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrUiFontV2MapItm	Tv_PtrItm;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check item info
		if ( 0 != m_CharCnt )
		{
			Tv_PtrItm		=	m_PtrMapTbl;
			//find map info
			for ( Tv_WkIdx=0;Tv_WkIdx < m_CharCnt;Tv_WkIdx++ )
			{
				//check match code
				if ( A_Code == Tv_PtrItm->Code )
				{
					Tv_Result	=	TRUE;
					break;
				}
				//next
				Tv_PtrItm ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUiFontCalcSize( Ptr_GrUiFontV2Head A_PtrFont )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	sizeof(St_GrUiFontV2Head);
		Tv_Result	=	Tv_Result + A_PtrFont->CharCnt * sizeof(St_GrUiFontV2MapItm);
		Tv_Result	=	Tv_Result + A_PtrFont->DibPitch * A_PtrFont->Height;

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrUiFontV2MapItm	Cls_GrUiFontObj::GetMapTbl( void )
{
		return	m_PtrMapTbl;
}
//--------------------------------------------------------------------


