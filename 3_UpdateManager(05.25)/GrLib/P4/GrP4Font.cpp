/*
 osd font object

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P4/GrP4Font.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>

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
Cls_GrP4Font::Cls_GrP4Font( Ptr_GrP4FontHead A_PtrFont )
{
	// local -------------------
	// code --------------------
		//check font
		if ( NULL != A_PtrFont )
		{
			//init
			m_IsCodeTbl		=	FALSE;
			m_Gdib.Pitch	=	A_PtrFont->DibPitch;

			m_Width			=	A_PtrFont->Width;
			m_CharCnt		=	A_PtrFont->CharCnt;

			//calculate mapping table
			m_PtrMapTbl		=	(Ptr_GrP4FontMapItm)( (__u32)A_PtrFont + sizeof(St_GrP4FontHead) );
			//calculate dib 
			m_PtrDib			=	(void*)( (__u32)m_PtrMapTbl + (m_CharCnt * sizeof(St_GrP4FontMapItm) ) );
			//make gdib
			GrGdibInitHead( &m_Gdib, E_GrFccFont, A_PtrFont->DibWidth, A_PtrFont->Height, A_PtrFont->DibPitch,
											m_PtrDib, E_GrGdibFontDibBitCnt, (void*)E_GrGdibPltBase );
		}
}
//--------------------------------------------------------------------
Cls_GrP4Font::~Cls_GrP4Font()
{
}
//--------------------------------------------------------------------
__u32		Cls_GrP4Font::GetWidth( void )
{
		return	m_Width;
}
//--------------------------------------------------------------------
__u32		Cls_GrP4Font::GetHeight(void)
{
		return	m_Gdib.ResY;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4Font::GetCharCnt(void)
{
		return	m_CharCnt;
}
//--------------------------------------------------------------------
Ptr_GrGdib	Cls_GrP4Font::GetGdibPtr( void )
{
		return	&m_Gdib;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Font::GetCharInfo(__u16 A_Code, __u32* A_PtrRtIdx, Ptr_GrRect A_PtrRtRcDib, __s32* A_PtrRtPitch, __s32* A_PtrRtGap)
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrP4FontMapItm	Tv_PtrItm;
		__u32		Tv_WkIdx;
		__u32		Tv_DibPos;
		__u16		Tv_CtblIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check able
		if ( 0 != m_CharCnt )
		{
			if (m_IsCodeTbl)
			{
				if ((E_GrP4FontCharTblBase <= A_Code) && (E_GrP4FontCharTblEnd >= A_Code))
				{
					Tv_CtblIdx	=	A_Code - E_GrP4FontCharTblBase;
					Tv_WkIdx		=	(__u32)m_CdIdxTbl[Tv_CtblIdx];
					Tv_DibPos		=	m_CdPosTbl[Tv_CtblIdx];

					*A_PtrRtIdx						=	Tv_WkIdx;
					A_PtrRtRcDib->left			=	(__s32)Tv_DibPos;
					A_PtrRtRcDib->top			=	0;
					A_PtrRtRcDib->right		=	A_PtrRtRcDib->left + (__s32)m_PtrMapTbl[Tv_WkIdx].Pitch;
					A_PtrRtRcDib->bottom		=	(__s32)m_Gdib.ResY;
					*A_PtrRtPitch					=	(__s32)m_PtrMapTbl[Tv_WkIdx].Pitch;
					*A_PtrRtGap						=	(__s32)m_PtrMapTbl[Tv_WkIdx].StGap;
					Tv_Result	=	TRUE;
				}
			}
			else
			{
				Tv_PtrItm		=	m_PtrMapTbl;
				//find map info
				Tv_DibPos		=	0;
				for (Tv_WkIdx=0; Tv_WkIdx < m_CharCnt; Tv_WkIdx++)
				{
					//check match code
					if (A_Code == Tv_PtrItm->Code)
					{
						*A_PtrRtIdx						=	Tv_WkIdx;
						A_PtrRtRcDib->left			=	(__s32)Tv_DibPos;
						A_PtrRtRcDib->top			=	0;
						A_PtrRtRcDib->right		=	A_PtrRtRcDib->left + (__s32)Tv_PtrItm->Pitch;
						A_PtrRtRcDib->bottom		=	(__s32)m_Gdib.ResY;
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
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4Font::GetItemInfo(__u32 A_Idx, __u16* A_PtrRtCode, Ptr_GrRect A_PtrRtRcDib, __s32* A_PtrRtPitch, __s32* A_PtrRtGap)
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrP4FontMapItm	Tv_PtrItm;
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
BOOL8	Cls_GrP4Font::IsCodeExist(__u16 A_Code)
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrP4FontMapItm	Tv_PtrItm;
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
__u32	GrP4FontCalcSize( Ptr_GrP4FontHead A_PtrFont )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	sizeof(St_GrP4FontHead);
		Tv_Result	=	Tv_Result + A_PtrFont->CharCnt * sizeof(St_GrP4FontMapItm);
		Tv_Result	=	Tv_Result + A_PtrFont->DibPitch * A_PtrFont->Height;

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrP4FontMapItm	Cls_GrP4Font::GetMapTbl( void )
{
		return	m_PtrMapTbl;
}
//--------------------------------------------------------------------
void	Cls_GrP4Font::CodeTblBuild(void)
{
	// local -------------------
		__u32		Tv_MapIdx;
		__u32		Tv_Pos;
		__u16		Tv_TblIdx;
			// code --------------------
		if (0 != m_CharCnt)
		{
			GrDumyZeroMem(m_CdIdxTbl, sizeof(m_CdIdxTbl));
			GrDumyZeroMem(m_CdPosTbl, sizeof(m_CdPosTbl));
			Tv_Pos		=	0;

			for (Tv_MapIdx=0; Tv_MapIdx < m_CharCnt;Tv_MapIdx++)
			{
				if ((E_GrP4FontCharTblBase <= m_PtrMapTbl[Tv_MapIdx].Code) && (E_GrP4FontCharTblEnd >= m_PtrMapTbl[Tv_MapIdx].Code))
				{
					Tv_TblIdx	=	m_PtrMapTbl[Tv_MapIdx].Code - E_GrP4FontCharTblBase;
					m_CdIdxTbl[Tv_TblIdx]	=	(__u16)Tv_MapIdx;
					m_CdPosTbl[Tv_TblIdx]	=	Tv_Pos;
				}
				Tv_Pos	=	Tv_Pos + (__u32)m_PtrMapTbl[Tv_MapIdx].Pitch;
			}

			m_IsCodeTbl	=	TRUE;
		}
		
}
//--------------------------------------------------------------------

