/*
Font version 3 DIB1 type draw

*/

//--------------------------------------------------------------------
// uses

#include <FontV3/Dib2/GrFontV3Dib2Draw.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>

#include <stdlib.h>

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var


//====================================================================
//--------------------------------------------------------------------
Cls_GrFontV3Dib2Draw::Cls_GrFontV3Dib2Draw(void):
Cls_GrFontV3DrawBase()
{
	// local -------------------
	// code --------------------
		// init
		m_PtrScpt					=	NULL;
		m_PtrDib1Hd				=	NULL;
		m_ChrInfoTbl			=	NULL;
		m_PitchTbl				=	NULL;
		m_PtrData					=	NULL;

		m_ChrCtrlTbl			=	NULL;

		m_ScptSize				=	0;

		m_IsAutoScptFree	=	FALSE;
}
//--------------------------------------------------------------------
Cls_GrFontV3Dib2Draw::~Cls_GrFontV3Dib2Draw(void)
{
	// local -------------------
	// code --------------------

		LcScriptFree();

}
//--------------------------------------------------------------------
void	Cls_GrFontV3Dib2Draw::LcScriptFree(void)
{
	// local -------------------
	// code --------------------
		// release script
		if((NULL != m_PtrScpt) && m_IsAutoScptFree)
		{
			free(m_PtrScpt);
			m_PtrScpt		=	NULL;
		}

		// release char control table
		if(NULL != m_ChrCtrlTbl)
		{
			free(m_ChrCtrlTbl);
			m_ChrCtrlTbl	=	NULL;
		}

		// reset
		m_PtrDib1Hd		=	NULL;
		m_ChrInfoTbl	=	NULL;
		m_PitchTbl		=	NULL;
		m_PtrData			=	NULL;

		m_ChrCtrlTbl	=	NULL;
		m_ScptSize		=	0;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFontV3Dib2Draw::LcCharCtrlTblBuild(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Size;
		__u16	Tv_ChrIdx;
		Def_GrCalPtr	Tv_OfsDat;
	// code --------------------
		Tv_Result	=	FALSE;

		// check exist char contorl table
		if((NULL == m_ChrCtrlTbl) && (NULL != m_PtrScpt))
		{
			// allocate
			Tv_Size	=	sizeof(St_GrFontV3Dib2DrawCharCtrlItem)* (__u32)m_PtrDib1Hd->FontCnt;
			if(0 != Tv_Size)
			{
				m_ChrCtrlTbl	=	(Ptr_GrFontV3Dib2DrawCharCtrlItem)malloc(Tv_Size);
				if(NULL != m_ChrCtrlTbl)
				{
					// build
					m_ScptSize	=	sizeof(St_GrFontV3ScptHead)+ sizeof(St_GrFontV3Dib2ScptHead)
						+ (sizeof(St_GrFontV3Dib2CharInfo)* (__u32)m_PtrDib1Hd->FontCnt)
						+ (__u32)m_PtrDib1Hd->FontCnt;

					Tv_OfsDat		=	(Def_GrCalPtr)m_PtrData;
					for( Tv_ChrIdx = 0; Tv_ChrIdx < m_PtrDib1Hd->FontCnt; Tv_ChrIdx++)
					{
						m_ChrCtrlTbl[Tv_ChrIdx].PtrData	=	(void*)Tv_OfsDat;
						// next
						Tv_Size			=	(__u32)m_ChrInfoTbl[Tv_ChrIdx].BitSize >> 3;
						if(0 != (m_ChrInfoTbl[Tv_ChrIdx].BitSize & 7))
						{
							Tv_Size ++;
						}
						Tv_OfsDat		=	Tv_OfsDat + (Def_GrCalPtr)Tv_Size;
						m_ScptSize	=	m_ScptSize + Tv_Size;
					}
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFontV3Dib2Draw::ScriptAssign(void* A_PtrScpt, __u32 A_Size, BOOL8 A_IsAutoScptFree)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_HdSize;
	// code --------------------
		Tv_Result		=	FALSE;

		LcScriptFree();

		// init
		m_IsAutoScptFree	=	A_IsAutoScptFree;
		// check size
		Tv_HdSize	=	sizeof(St_GrFontV3ScptHead)+ sizeof(St_GrFontV3Dib2ScptHead);
		if(Tv_HdSize < A_Size)
		{
			m_PtrScpt	=	(Ptr_GrFontV3ScptHead)A_PtrScpt;
			if(NULL != m_PtrScpt)
			{
				// check script header
				if((E_GrFontV3ScptFcc == m_PtrScpt->Fcc) && (E_GrFontV3Dib2ClassId == m_PtrScpt->ClassId))
				{
					// check version
					m_PtrDib1Hd	=	(Ptr_GrFontV3Dib2ScptHead)((Def_GrCalPtr)m_PtrScpt + sizeof(St_GrFontV3ScptHead));
					if(E_GrFontV3Dib2Version == m_PtrDib1Hd->Version)
					{
						// check character count
						if(0 != m_PtrDib1Hd->FontCnt)
						{
							// init pointer
							m_ChrInfoTbl	=	(Ptr_GrFontV3Dib2CharInfo)((Def_GrCalPtr)m_PtrDib1Hd + sizeof(St_GrFontV3Dib2ScptHead));
							m_PitchTbl		=	(__u8*)((Def_GrCalPtr)m_ChrInfoTbl + (sizeof(St_GrFontV3Dib2CharInfo)* (Def_GrCalPtr)m_PtrDib1Hd->FontCnt));
							m_PtrData			=	(void*)((Def_GrCalPtr)m_PitchTbl + (Def_GrCalPtr)m_PtrDib1Hd->FontCnt);

							// build map table
							if(LcCharCtrlTblBuild())
							{
								if(m_ScptSize <= A_Size)
								{
									Tv_Result	=	TRUE;
								}
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrFontV3Dib2Draw::ScriptAssign - character not exist\n");
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrFontV3Dib2Draw::ScriptAssign - bad script version (%d)\n", m_PtrDib1Hd->Version);
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrFontV3Dib2Draw::ScriptAssign - bad script base head\n");
				}

				// check faile
				if(!Tv_Result)
				{
					if(m_IsAutoScptFree)
					{
						m_PtrScpt	=	NULL;
					}
					LcScriptFree();
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrFontV3Dib2Draw::ScriptAssign - script size is very small!\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrFontV3Dib2Draw::CodeToCharIdx(__u16 A_Code)
{
	// local -------------------
		__s32	Tv_Result;
		__u16	Tv_Step;
		__u16	Tv_ChrIdx;
		__u16	Tv_AbleCnt;
	// code --------------------
		Tv_Result		=	-1;
		Tv_AbleCnt	=	2;

		Tv_Step		=	m_PtrDib1Hd->FontCnt;
		Tv_ChrIdx	=	0;

		while(0 < Tv_AbleCnt)
		{
			// check now item
			if(A_Code == m_ChrInfoTbl[Tv_ChrIdx].Code)
			{
				// found
				Tv_Result	=	(__s32)((__u32)Tv_ChrIdx);
				break;
			}
			// go next
			Tv_Step	=	(Tv_Step >> 1) + (Tv_Step & 1);
			if(1 == Tv_Step)
			{
				Tv_AbleCnt --;
			}
			if(A_Code < m_ChrInfoTbl[Tv_ChrIdx].Code)
			{
				// go low value
				if(0 == Tv_ChrIdx)
				{
					break;
				}
				if(Tv_Step >= Tv_ChrIdx)
				{
					Tv_ChrIdx	=	0;
				}
				else
				{
					Tv_ChrIdx	=	Tv_ChrIdx - Tv_Step;
				}
			}
			else
			{
				// go higher value
				if((Tv_ChrIdx + 1) >= m_PtrDib1Hd->FontCnt)
				{
					break;
				}
				if((Tv_Step + Tv_ChrIdx) >= m_PtrDib1Hd->FontCnt)
				{
					Tv_ChrIdx	=	m_PtrDib1Hd->FontCnt - 1;
				}
				else
				{
					Tv_ChrIdx	=	Tv_ChrIdx + Tv_Step;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrFontV3Dib2Draw::CharIdxToCode(__s32 A_ChrIdx)
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if((0 <= A_ChrIdx) && ((__u16)A_ChrIdx < m_PtrDib1Hd->FontCnt))
		{
			Tv_Result	=	m_ChrInfoTbl[A_ChrIdx].Code;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrFontV3Dib2Draw::HeightGet(void)
{
		return	m_PtrDib1Hd->FontHeight;
}
//--------------------------------------------------------------------
__u8	Cls_GrFontV3Dib2Draw::WidthGet(void)
{
		return	m_PtrDib1Hd->FontWidth;
}
//--------------------------------------------------------------------
__u16	Cls_GrFontV3Dib2Draw::CharCountGet(void)
{
		return	m_PtrDib1Hd->FontCnt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFontV3Dib2Draw::CharInfoGetByIndxe(__s32 A_ChrIdx, Ptr_GrFontV3DrawCharInfo A_PtrRtInfo)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if((0 <= A_ChrIdx) && ((__u16)A_ChrIdx < m_PtrDib1Hd->FontCnt))
		{
			A_PtrRtInfo->DataSize		=	m_ChrInfoTbl[A_ChrIdx].BitSize >> 3;
			if(0 != (m_ChrInfoTbl[A_ChrIdx].BitSize & 7))
			{
				A_PtrRtInfo->DataSize ++;
			}
			A_PtrRtInfo->Pitch			=	m_PitchTbl[A_ChrIdx];
			A_PtrRtInfo->PtrData		=	m_ChrCtrlTbl[A_ChrIdx].PtrData;

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFontV3Dib2Draw::DrawFont(Cls_GrGdib* A_Gdib, __s32 A_ChrIdx,__s32 A_PosX, __s32 A_PosY)
{
		if((0 <= A_ChrIdx) && ((__u16)A_ChrIdx < m_PtrDib1Hd->FontCnt))
		{
			GrFontV3Dib2Draw(A_Gdib, m_PitchTbl[A_ChrIdx], m_ChrCtrlTbl[A_ChrIdx].PtrData, m_ChrInfoTbl[A_ChrIdx].BitSize,
				A_PosX, A_PosY, m_Ctrl.IsDrawTxt, m_Ctrl.IsDrawShadow, m_ClrTbl);
		}
		
}
//--------------------------------------------------------------------
void*	Cls_GrFontV3Dib2Draw::ScriptGet(__u32* A_PtrRtSize)
{
		*A_PtrRtSize	=	m_ScptSize;
		return	(void*)m_PtrScpt;
}
//--------------------------------------------------------------------
void	Cls_GrFontV3Dib2Draw::DrawCtrlInit(WCHAR* A_StrTxt, Cls_GrGdib* A_Gdib, Ptr_GrRect A_PtrRc, Ptr_GrFontV3DrawStyle A_PtrStyle, __u32 A_Stat)
{
	// local -------------------
		__s32	Tv_ClrTxtR;
		__s32	Tv_ClrTxtG;
		__s32	Tv_ClrTxtB;
		__s32	Tv_ClrTxtA;
		__s32	Tv_ClrTxtY;
		__s32	Tv_ClrTxtU;
		__s32	Tv_ClrTxtV;

		__s32	Tv_ClrShdR;
		__s32	Tv_ClrShdG;
		__s32	Tv_ClrShdB;
		__s32	Tv_ClrShdA;
		__s32	Tv_ClrShdY;
		__s32	Tv_ClrShdU;
		__s32	Tv_ClrShdV;

		__s32	Tv_ClrInpR;
		__s32	Tv_ClrInpG;
		__s32	Tv_ClrInpB;
		//__s32	Tv_ClrInpY;
		//__s32	Tv_ClrInpU;
		//__s32	Tv_ClrInpV;
	// code --------------------
		// inherted
		Cls_GrFontV3DrawBase::DrawCtrlInit(A_StrTxt, A_Gdib, A_PtrRc, A_PtrStyle, A_Stat);

		if(NULL == A_Gdib)
		{
			return;
		}

		// calculate interpolation color
		//m_ClrIntp	=	GrDrawColorMix(A_Gdib->GetFcc(), m_Ctrl.ClrTxt, m_Ctrl.Style.ColorShadow);

		m_ClrTbl[0]		=	m_Ctrl.Style.ColorShadow;
		m_ClrTbl[1]		=	m_Ctrl.ClrTxt;

		GrDrawGetColorElements(A_Gdib->GetFcc(), m_Ctrl.ClrTxt, &Tv_ClrTxtR, &Tv_ClrTxtG, &Tv_ClrTxtB, &Tv_ClrTxtA, &Tv_ClrTxtY, &Tv_ClrTxtU, &Tv_ClrTxtV);
		GrDrawGetColorElements(A_Gdib->GetFcc(), m_Ctrl.Style.ColorShadow, &Tv_ClrShdR, &Tv_ClrShdG, &Tv_ClrShdB, &Tv_ClrShdA, &Tv_ClrShdY, &Tv_ClrShdU, &Tv_ClrShdV);

		// color half
		if(Tv_ClrTxtR < Tv_ClrShdR)
		{
			Tv_ClrInpR	=	Tv_ClrTxtR + ((Tv_ClrShdR - Tv_ClrTxtR) >> 1);
		}
		else
		{
			Tv_ClrInpR	=	Tv_ClrTxtR - ((Tv_ClrTxtR - Tv_ClrShdR) >> 1);
		}

		if(Tv_ClrTxtG < Tv_ClrShdG)
		{
			Tv_ClrInpG	=	Tv_ClrTxtG + ((Tv_ClrShdG - Tv_ClrTxtG) >> 1);
		}
		else
		{
			Tv_ClrInpG	=	Tv_ClrTxtG - ((Tv_ClrTxtG - Tv_ClrShdG) >> 1);
		}

		if(Tv_ClrTxtB < Tv_ClrShdB)
		{
			Tv_ClrInpB	=	Tv_ClrTxtB + ((Tv_ClrShdB - Tv_ClrTxtB) >> 1);
		}
		else
		{
			Tv_ClrInpB	=	Tv_ClrTxtB - ((Tv_ClrTxtB - Tv_ClrShdB) >> 1);
		}
		m_ClrTbl[2]	=	GrDrawMakeColorByElements(A_Gdib->GetFcc(), Tv_ClrInpR, Tv_ClrInpG, Tv_ClrInpB, Tv_ClrTxtA);

		// color quatter
		if(Tv_ClrTxtR < Tv_ClrShdR)
		{
			Tv_ClrInpR	=	Tv_ClrTxtR + ((Tv_ClrShdR - Tv_ClrTxtR) >> 2);
		}
		else
		{
			Tv_ClrInpR	=	Tv_ClrTxtR - ((Tv_ClrTxtR - Tv_ClrShdR) >> 2);
		}

		if(Tv_ClrTxtG < Tv_ClrShdG)
		{
			Tv_ClrInpG	=	Tv_ClrTxtG + ((Tv_ClrShdG - Tv_ClrTxtG) >> 2);
		}
		else
		{
			Tv_ClrInpG	=	Tv_ClrTxtG - ((Tv_ClrTxtG - Tv_ClrShdG) >> 2);
		}

		if(Tv_ClrTxtB < Tv_ClrShdB)
		{
			Tv_ClrInpB	=	Tv_ClrTxtB + ((Tv_ClrShdB - Tv_ClrTxtB) >> 2);
		}
		else
		{
			Tv_ClrInpB	=	Tv_ClrTxtB - ((Tv_ClrTxtB - Tv_ClrShdB) >> 2);
		}
		m_ClrTbl[3]	=	GrDrawMakeColorByElements(A_Gdib->GetFcc(), Tv_ClrInpR, Tv_ClrInpG, Tv_ClrInpB, Tv_ClrTxtA);

		// color 1per8
		if(Tv_ClrTxtR < Tv_ClrShdR)
		{
			Tv_ClrInpR	=	Tv_ClrTxtR + ((Tv_ClrShdR - Tv_ClrTxtR) >> 3);
		}
		else
		{
			Tv_ClrInpR	=	Tv_ClrTxtR - ((Tv_ClrTxtR - Tv_ClrShdR) >> 3);
		}

		if(Tv_ClrTxtG < Tv_ClrShdG)
		{
			Tv_ClrInpG	=	Tv_ClrTxtG + ((Tv_ClrShdG - Tv_ClrTxtG) >> 3);
		}
		else
		{
			Tv_ClrInpG	=	Tv_ClrTxtG - ((Tv_ClrTxtG - Tv_ClrShdG) >> 3);
		}

		if(Tv_ClrTxtB < Tv_ClrShdB)
		{
			Tv_ClrInpB	=	Tv_ClrTxtB + ((Tv_ClrShdB - Tv_ClrTxtB) >> 3);
		}
		else
		{
			Tv_ClrInpB	=	Tv_ClrTxtB - ((Tv_ClrTxtB - Tv_ClrShdB) >> 3);
		}
		m_ClrTbl[4]	=	GrDrawMakeColorByElements(A_Gdib->GetFcc(), Tv_ClrInpR, Tv_ClrInpG, Tv_ClrInpB, Tv_ClrTxtA);

}
//--------------------------------------------------------------------
