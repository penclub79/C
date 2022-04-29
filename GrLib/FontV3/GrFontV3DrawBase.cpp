/*
Platform 5 UI object base

*/

//--------------------------------------------------------------------
// uses

#include <FontV3/GrFontV3DrawBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>

#include <FontV3/GrFontV3Base.h>

#include <FontV3/Dib1/GrFontV3Dib1Base.h>
#include <FontV3/Dib1/GrFontV3Dib1Draw.h>

#include <FontV3/Dib2/GrFontV3Dib2Base.h>
#include <FontV3/Dib2/GrFontV3Dib2Draw.h>


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
Cls_GrFontV3DrawBase::Cls_GrFontV3DrawBase(void)
{
	// local -------------------
	// code --------------------
		// init

}
//--------------------------------------------------------------------
Cls_GrFontV3DrawBase::~Cls_GrFontV3DrawBase(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFontV3DrawBase::ScriptAssign(void* A_PtrScpt, __u32 A_Size, BOOL8 A_IsAutoScptFree)
{
		return	FALSE;
}
//--------------------------------------------------------------------
__s32	Cls_GrFontV3DrawBase::CodeToCharIdx(__u16 A_Code)
{
		return	-1;
}
//--------------------------------------------------------------------
__u16	Cls_GrFontV3DrawBase::CharIdxToCode(__s32 A_ChrIdx)
{
		return	0;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrFontV3DrawBase::CharInfoGetByIndxe(__s32 A_ChrIdx, Ptr_GrFontV3DrawCharInfo A_PtrRtInfo)
{
		return	FALSE;
}
//--------------------------------------------------------------------
__u8	Cls_GrFontV3DrawBase::HeightGet(void)
{
		return	0;
}
//--------------------------------------------------------------------
__u8	Cls_GrFontV3DrawBase::WidthGet(void)
{
		return	0;
}
//--------------------------------------------------------------------
__u16	Cls_GrFontV3DrawBase::CharCountGet(void)
{
		return	0;
}
//--------------------------------------------------------------------
void	Cls_GrFontV3DrawBase::DrawFont(Cls_GrGdib* A_Gdib, __s32 A_ChrIdx,
	__s32 A_PosX, __s32 A_PosY )
{

}
//--------------------------------------------------------------------
void	Cls_GrFontV3DrawBase::DrawCtrlInit(WCHAR* A_StrTxt, Cls_GrGdib* A_Gdib, 
	Ptr_GrRect A_PtrRc, Ptr_GrFontV3DrawStyle A_PtrStyle, __u32 A_Stat)
{
	// local -------------------
		__u32	Tv_TclrIdx;
	// code --------------------
		m_Ctrl.Gdib			=	A_Gdib;
		m_Ctrl.Style		=	*A_PtrStyle;
		m_Ctrl.RcTarget	=	*A_PtrRc;
		m_Ctrl.StrTxt		=	A_StrTxt;


		Tv_TclrIdx					=	A_Stat & 1;
		if(E_GrFontV3StatDisable != Tv_TclrIdx)
		{
			if(0 != ((E_GrFontV3StatOnChecked | E_GrFontV3StatOnSelected) & A_Stat))
			{
				Tv_TclrIdx					=	E_GrFontV3TxtClrSelect;
			}
			else if(0 != (E_GrFontV3StatOnFocus & A_Stat))
			{
				Tv_TclrIdx					=	E_GrFontV3TxtClrFocus;
			}
		}
		m_Ctrl.ClrTxt		=	m_Ctrl.Style.ColorTxtTbl[Tv_TclrIdx];

		m_Ctrl.SizeX			=	0;
		m_Ctrl.SizeY			=	0;
		m_Ctrl.TxtLen		=	0;

		m_Ctrl.IsDrawTxt		=	FALSE;
		if(0 != (E_GrFontV3AtbDrawText & m_Ctrl.Style.Atb))
		{
			m_Ctrl.IsDrawTxt		=	TRUE;
		}

		m_Ctrl.IsDrawShadow	=	FALSE;
		if(0 != (E_GrFontV3AtbDrawShadow & m_Ctrl.Style.Atb))
		{
			m_Ctrl.IsDrawShadow		=	TRUE;
		}

		m_Ctrl.Height	=	(__s32)((__u32)HeightGet());
		m_Ctrl.Width		=	(__s32)((__u32)WidthGet());


}
//--------------------------------------------------------------------
BOOL8	Cls_GrFontV3DrawBase::DrawCtrlSubLinefeed(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;

		m_Ctrl.SizeY		=	m_Ctrl.SizeY + m_Ctrl.Height;
		if(m_Ctrl.IsGapLine)
		{
			m_Ctrl.SizeY		=	m_Ctrl.SizeY + (__s32)m_Ctrl.Style.GapLine;
		}
		m_Ctrl.DrawY		=	m_Ctrl.DrawY + m_Ctrl.Height + (__s32)m_Ctrl.Style.GapLine;

		// check finish
		if(m_Ctrl.DrawY >= m_Ctrl.RcTarget.bottom)
		{
			Tv_Result	=	FALSE;
		}

		m_Ctrl.SizeX				=	0;
		m_Ctrl.DrawX				=	m_Ctrl.RcTarget.left;

		m_Ctrl.IsGapLine		=	TRUE;
		m_Ctrl.IsGapChr		=	FALSE;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFontV3DrawBase::DrawCtrlRun(BOOL8 A_IsDraw)
{
	// local -------------------
		__s32	Tv_ChrIdx;
		BOOL8	Tv_IsBadChr;
		WCHAR*	Tv_StrTxt;
		St_GrFontV3DrawCharInfo	Tv_ChrInfo;
		__s32	Tv_Space;
		__s32	Tv_DrawX;
		St_GrRect	Tv_Rc;
	// code --------------------
		// init
		m_Ctrl.MaxSizeX		=	0;
		m_Ctrl.SizeX			=	0;
		m_Ctrl.SizeY			=	0;
		m_Ctrl.DrawX			=	m_Ctrl.RcTarget.left;
		m_Ctrl.DrawY			=	m_Ctrl.RcTarget.top;
		m_Ctrl.TxtLen			=	0;
		m_Ctrl.IsGapChr		=	FALSE;
		m_Ctrl.IsGapLine	=	FALSE;

		Tv_StrTxt		=	m_Ctrl.StrTxt;

		while((0 != Tv_StrTxt[m_Ctrl.TxtLen]) || ((__s32)m_Ctrl.TxtLen == m_Ctrl.CusPos))
		{

			// check mode
			if(((0 != (E_GrFontV3AtbSemiEnter & m_Ctrl.Style.Atb)) && (E_FontV3DrawCodeInsteadEnter == Tv_StrTxt[m_Ctrl.TxtLen])) ||
				(0x0A == Tv_StrTxt[m_Ctrl.TxtLen]) || (0x0D == Tv_StrTxt[m_Ctrl.TxtLen]))
			{
				// enter
				if(!DrawCtrlSubLinefeed())
				{
					break;
				}
			}
			else 
			{
				// gap
				if(m_Ctrl.IsGapChr)
				{
					if((0 < m_Ctrl.Style.GapChar) && ((m_Ctrl.DrawX + (__s32)m_Ctrl.Style.GapChar) > m_Ctrl.RcTarget.right))
					{
						if(0 != (E_GrFontV3AtbWordWarp & m_Ctrl.Style.Atb))
						{
							// do line feed
							if(!DrawCtrlSubLinefeed())
							{
								break;
							}
						}
						else
						{
							// finish
							break;
						}
					}
					else
					{
						// make gap
						m_Ctrl.DrawX		=	m_Ctrl.DrawX + (__s32)m_Ctrl.Style.GapChar;
						m_Ctrl.SizeX		=	m_Ctrl.SizeX + (__s32)m_Ctrl.Style.GapChar;
						// update x size
						if(m_Ctrl.SizeX > m_Ctrl.MaxSizeX)
						{
							m_Ctrl.MaxSizeX	=	m_Ctrl.SizeX;
						}
					}
				}

				// do character
				Tv_IsBadChr	=	FALSE;
				Tv_Space		=	m_Ctrl.Width >> 1;
				if(0x3000 == Tv_StrTxt[m_Ctrl.TxtLen])
				{
					Tv_Space		=	m_Ctrl.Width;		// full width sapce for japaness
				}
				else if(0x20 < Tv_StrTxt[m_Ctrl.TxtLen])
				{
					// get character info
					Tv_IsBadChr	=	TRUE;
					if(0 != (E_GrFontV3AtbPassword & m_Ctrl.Style.Atb))
					{
						Tv_ChrIdx	=	CodeToCharIdx(m_Ctrl.Style.CodePw);
					}
					else
					{
						Tv_ChrIdx	=	CodeToCharIdx((__u16)Tv_StrTxt[m_Ctrl.TxtLen]);
					}
					if(0 <= Tv_ChrIdx)
					{
						if(CharInfoGetByIndxe(Tv_ChrIdx, &Tv_ChrInfo))
						{
							Tv_Space		=	(__s32)((__u32)Tv_ChrInfo.Pitch);
							if(0 != (E_GrFontV3AtbFixPitch & m_Ctrl.Style.Atb))
							{
								Tv_Space		=	m_Ctrl.Width;
							}
							Tv_IsBadChr	=	FALSE;;
						}
					}
				}

				if((m_Ctrl.DrawX + Tv_Space) > m_Ctrl.RcTarget.right)
				{
					// check word warp
					if(0 != (E_GrFontV3AtbWordWarp & m_Ctrl.Style.Atb))
					{
						// do line feed
						if(!DrawCtrlSubLinefeed())
						{
							break;
						}
					}
					else
					{
						// finish
						break;
					}
				}

				// draw
				if(A_IsDraw && (0x20 != Tv_StrTxt[m_Ctrl.TxtLen]) && (0x3000 != Tv_StrTxt[m_Ctrl.TxtLen]))
				{
					// draw cursor
					if((__s32)m_Ctrl.TxtLen == m_Ctrl.CusPos)
					{
						// draw cursor
						Tv_Rc.left		=	m_Ctrl.DrawX;
						Tv_Rc.top			=	m_Ctrl.DrawY;
						Tv_Rc.right		=	Tv_Rc.left + Tv_Space;
						Tv_Rc.bottom	=	Tv_Rc.top + m_Ctrl.Height;
						m_Ctrl.Gdib->FillRect(&Tv_Rc, m_Ctrl.ClrCus);
					}

					// draw text
					if(0 != Tv_StrTxt[m_Ctrl.TxtLen])
					{
						if(Tv_IsBadChr)
						{
							// draw bad character mark
							m_Ctrl.Gdib->DrawLine(m_Ctrl.DrawX, m_Ctrl.DrawY, (m_Ctrl.DrawX + Tv_Space - 1), m_Ctrl.DrawY,
								m_Ctrl.ClrTxt);
							m_Ctrl.Gdib->DrawLine(m_Ctrl.DrawX, m_Ctrl.DrawY, m_Ctrl.DrawX, (m_Ctrl.DrawY + m_Ctrl.Height - 1),
								m_Ctrl.ClrTxt);
							m_Ctrl.Gdib->DrawLine(m_Ctrl.DrawX, (m_Ctrl.DrawY + m_Ctrl.Height - 1),
								(m_Ctrl.DrawX + Tv_Space - 1), (m_Ctrl.DrawY + m_Ctrl.Height - 1), m_Ctrl.ClrTxt);
							m_Ctrl.Gdib->DrawLine((m_Ctrl.DrawX + Tv_Space - 1), m_Ctrl.DrawY,
								(m_Ctrl.DrawX + Tv_Space - 1), (m_Ctrl.DrawY + m_Ctrl.Height - 1), m_Ctrl.ClrTxt);
						}
						else
						{
							// draw font
							Tv_DrawX	=	m_Ctrl.DrawX + ((Tv_Space - (__s32)((__u32)Tv_ChrInfo.Pitch)) >> 1);
							DrawFont(m_Ctrl.Gdib, Tv_ChrIdx, Tv_DrawX, m_Ctrl.DrawY);
						}
					}
				}
				
				// go next position
				m_Ctrl.DrawX		=	m_Ctrl.DrawX + Tv_Space;
				m_Ctrl.SizeX		=	m_Ctrl.SizeX + Tv_Space;
				// update x size
				if(m_Ctrl.SizeX > m_Ctrl.MaxSizeX)
				{
					m_Ctrl.MaxSizeX	=	m_Ctrl.SizeX;
				}

				// no first
				m_Ctrl.IsGapChr	=	TRUE;
			}

			// check finish
			if(0 == Tv_StrTxt[m_Ctrl.TxtLen])
			{
				break;
			}

			// next
			m_Ctrl.TxtLen ++;
		}	//while(0 != Tv_StrTxt[m_Ctrl.TxtLen])

		// check exist last line
		if(0 < m_Ctrl.SizeX)
		{
			if(m_Ctrl.IsGapLine)
			{
				m_Ctrl.SizeY		=	m_Ctrl.SizeY + (__s32)m_Ctrl.Style.GapLine;
			}
			m_Ctrl.SizeY		=	m_Ctrl.SizeY + m_Ctrl.Height;
		}

}
//--------------------------------------------------------------------
void	Cls_GrFontV3DrawBase::DrawString(WCHAR* A_StrTxt, Cls_GrGdib* A_Gdib, Ptr_GrRect A_PtrRc, Ptr_GrFontV3DrawStyle A_PtrStyle, __u32 A_Stat,
	__s32 A_CusPos, __u32 A_ClrCus)
{
	// local -------------------
		__u8	Tv_AlignHz;
	// code --------------------
		// check exist string
		if(NULL == A_StrTxt)
		{
			return;
		}

		// init control
		DrawCtrlInit(A_StrTxt, A_Gdib, A_PtrRc, A_PtrStyle, A_Stat);
		m_Ctrl.CusPos		=	A_CusPos;
		m_Ctrl.ClrCus		=	A_ClrCus;

		Tv_AlignHz	=	A_PtrStyle->AlignHz;
		if(0 != (E_GrFontV3AtbWordWarp & A_PtrStyle->Atb))
		{
			Tv_AlignHz	=	E_GrFontV3AlignLeft;
		}

		// check align
		if((E_GrFontV3AlignLeft != Tv_AlignHz) || (E_GrFontV3AlignTop != A_PtrStyle->AlignVt))
		{
			// calculate size
			DrawCtrlRun(FALSE);
			// patch rect
			if(E_GrFontV3AlignRight == Tv_AlignHz)
			{
				m_Ctrl.RcTarget.left	=	m_Ctrl.RcTarget.right - m_Ctrl.MaxSizeX;
			}
			else if(E_GrFontV3AlignCenterHz == Tv_AlignHz)
			{
				m_Ctrl.RcTarget.left		=	(((m_Ctrl.RcTarget.right - m_Ctrl.RcTarget.left) - m_Ctrl.MaxSizeX) >> 1) + 
					m_Ctrl.RcTarget.left;
				m_Ctrl.RcTarget.right	=	m_Ctrl.RcTarget.left + m_Ctrl.MaxSizeX;
			}

			if(E_GrFontV3AlignBottom == m_Ctrl.Style.AlignVt)
			{
				m_Ctrl.RcTarget.top	=	m_Ctrl.RcTarget.bottom - m_Ctrl.SizeY;
			}
			else if(E_GrFontV3AlignCenterVt == m_Ctrl.Style.AlignVt)
			{
				m_Ctrl.RcTarget.top		=	(((m_Ctrl.RcTarget.bottom - m_Ctrl.RcTarget.top) - m_Ctrl.SizeY) >> 1) +
					m_Ctrl.RcTarget.top;
				m_Ctrl.RcTarget.bottom	=	m_Ctrl.RcTarget.top + m_Ctrl.SizeY;
			}
		}

		DrawCtrlRun(TRUE);

}
//--------------------------------------------------------------------
void	Cls_GrFontV3DrawBase::CalcStringDimension(WCHAR* A_StrTxt, Ptr_GrRect A_PtrRcCon, Ptr_GrFontV3DrawStyle A_PtrStyle, __u32 A_Stat, __u32* A_PtrRtWidth, __u32* A_PtrRtHeight)
{
	// local -------------------
		__u8	Tv_AlignHz;
		St_GrRect	Tv_RcBase;
	// code --------------------
		// init
		*A_PtrRtWidth	=	0;
		*A_PtrRtHeight	=	0;

		// check exist string
		if(NULL == A_StrTxt)
		{
			return;
		}

		// init control
		DrawCtrlInit(A_StrTxt, NULL, A_PtrRcCon, A_PtrStyle, A_Stat);
		m_Ctrl.CusPos		=	-1;
		m_Ctrl.ClrCus		=	0;

		// calculate size
		DrawCtrlRun(FALSE);

		*A_PtrRtWidth		=	(__u32)m_Ctrl.MaxSizeX;
		*A_PtrRtHeight	=	(__u32)m_Ctrl.SizeY;

}
//--------------------------------------------------------------------
Cls_GrFontV3DrawBase*	GrFontV3DrawCreateByScript(void* A_PtrScpt, __u32 A_Size, BOOL8 A_IsAutoScptFree)
{
	// local -------------------
		Cls_GrFontV3DrawBase*	Tv_Result;
		Ptr_GrFontV3ScptHead	Tv_PtrBaseHd;
	// code --------------------
		Tv_Result		=	NULL;

		// check header
		Tv_PtrBaseHd	=	(Ptr_GrFontV3ScptHead)A_PtrScpt;
		if(E_GrFontV3ScptFcc == Tv_PtrBaseHd->Fcc)
		{
			// check class
			switch(Tv_PtrBaseHd->ClassId	)
			{
				case	E_GrFontV3Dib1ClassId:
					Tv_Result	=	(Cls_GrFontV3DrawBase*) new Cls_GrFontV3Dib1Draw();
					break;
				case E_GrFontV3Dib2ClassId:
					Tv_Result	=	(Cls_GrFontV3DrawBase*) new Cls_GrFontV3Dib2Draw();
					break;
				default:
					DbgMsgPrint("GrFontV3DrawCreateByBin - unknown font class (%08X)\n",Tv_PtrBaseHd->ClassId);
					break;
			}

			if(NULL != Tv_Result)
			{
				if(!Tv_Result->ScriptAssign(A_PtrScpt, A_Size, A_IsAutoScptFree))
				{
					delete	Tv_Result;
					Tv_Result	=	NULL;
				}
			}

		}
		else
		{
			DbgMsgPrint("GrFontV3DrawCreateByBin - bad font script\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrFontV3DrawBase::ScriptGet(__u32* A_PtrRtSize)
{
		return	NULL;
}
//--------------------------------------------------------------------

