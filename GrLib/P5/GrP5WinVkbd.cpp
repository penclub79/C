/*
Platform 5 UI window virtual keyboard

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5WinVkbd.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5Key.h>
#include <P5/GrP5UoLblRef.h>
#include <P5/GrP5UoBtnVkey.h>
#include <P5/GrP5CtrlEvtBase.h>

#ifdef GR_P5_EDITOR

#include <P5/Dvl/GrP5DvlBase.h>

#endif

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

typedef	struct St_GrP5WinVkbdKorLastDiv
{
	__u16	Last;			//new last
	__u16	First;		//new first
}	*Ptr_GrP5WinVkbdKorLastDiv;

typedef struct St_GrP5WinVkbdJpnCmbInfo
{
	__u16	Std;
	__u16	Cmb;
}	*Ptr_GrP5WinVkbdJpnCmbInfo;

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;

St_GrP5WinVkbdKorLastDiv	V_GrP5DWinVkbdKorLastDivTbl[28]	=
{
	//0			1				2				3				4				5				6				7				8				9
	{0, 0}, {0, 1}, {0, 2}, {1, 10}, {0, 3}, {4, 13}, {4, 19}, {0, 4}, {0, 6}, {8, 1},
	{8, 7}, {8, 8}, {8, 10}, {8, 17}, {8, 18}, {8, 19}, {0, 7}, {0, 8}, {17, 10}, {0, 10},
	{0, 11}, {0, 12}, {0, 13}, {0, 15}, {0, 16}, {0, 17}, {0, 18}, {0, 19}
};

#ifdef GR_P5_EDITOR


#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5WinVkbd::Cls_GrP5WinVkbd(__u16 A_ScptId, __u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect,
	WCHAR* A_StrTxt, __u32 A_MaxLen, BOOL8 A_IsPassword, __u16 A_HostObjId, __u16 A_EvtCtlId, __u32 A_UserData) :
	Cls_GrP5WinBase(A_ScptId, A_ConIdx, A_LayerIdx, A_PtrRelRect)
{
	// local -------------------
		//St_GrRect	Tv_RcCon;
		//St_GrRect	Tv_RcWin;
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdWinVkbd, NULL, 0, NULL);

		// init
		m_HostObjId	=	A_HostObjId;
		m_EvtCtlId	=	A_EvtCtlId;
		m_UserData	=	A_UserData;
		m_StrRslt		=	A_StrTxt;
		m_MaxLen		=	A_MaxLen;
		m_IsPasswd	=	A_IsPassword;

		if(m_MaxLen > E_GrP5WinVkbdMaxTxtLen)
		{
			m_MaxLen	=	E_GrP5WinVkbdMaxTxtLen;
		}

		m_KorComb		=	0;
		m_CusPos		=	0;
		m_IsUpdtRslt	=	0;

		m_LangIdx		=	0;
		m_ShftIdx		=	0;

		// copy orignal text
		m_StrTxt		=	GrP5TxtBufAlloc(m_ObjId);
		if(NULL != m_StrTxt)
		{
			GrStrWcopy(m_StrTxt, m_StrRslt);
			m_Length		=	GrStrWlen(m_StrTxt);
		}

}
//--------------------------------------------------------------------
Cls_GrP5WinVkbd::~Cls_GrP5WinVkbd(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_EvtHost;
		Cls_GrP5CtrlEvtBase*	Tv_EvtCtl;
	// code --------------------
		Tv_EvtHost	=	(Cls_GrP5UoBase*)GrP5UiObjGet(m_HostObjId);
		if(m_IsUpdtRslt)
		{
			// send success
			if(NULL != m_StrTxt)
			{
				GrStrWcopy(m_StrRslt, m_StrTxt);
			}
			if(NULL != Tv_EvtHost)
			{
				Tv_EvtHost->EvtVkbdInput();
			}

			if(E_GrP5CtrlIdNone != m_EvtCtlId)
			{
				Tv_EvtCtl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_EvtCtlId);
				if(NULL != Tv_EvtCtl)
				{
					Tv_EvtCtl->EvtVkbdInput(m_EvtCtlId, m_StrRslt, m_MaxLen,m_UserData);
				}
			}
		}
		else
		{
			// send cancel
			if(NULL != Tv_EvtHost)
			{
				Tv_EvtHost->EvtVkbdCancel();
			}
			if(E_GrP5CtrlIdNone != m_EvtCtlId)
			{
				Tv_EvtCtl	=	(Cls_GrP5CtrlEvtBase*)GrP5CtrlEvtGet(m_EvtCtlId);
				if(NULL != Tv_EvtCtl)
				{
					Tv_EvtCtl->EvtVkbdCancel(m_EvtCtlId, m_StrRslt, m_MaxLen, m_UserData);
				}
			}

		}

		GrP5TxtBufFreeByObjId(m_ObjId);
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::EvtScptDecDone(void)
{
	// local -------------------
		Cls_GrP5UoLblRef*	Tv_Lbl;
	// code --------------------
		// inherited
		Cls_GrP5WinBase::EvtScptDecDone();

		// update layer lock
		m_ParaWinBase.IsInputLayerLock	=	TRUE;	// input layer lock

		// update string
		Tv_Lbl	=	(Cls_GrP5UoLblRef*)RtlUoFindByScptId((__u16)m_ParaUoBase.UserData[3]);
		if(NULL != Tv_Lbl)
		{
			if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblRef))
			{
				if(NULL != m_StrTxt)
				{
					Tv_Lbl->TextPtrSet(m_StrTxt, m_IsPasswd);
					m_CusPos	=	GrStrWlen(m_StrTxt);
					Tv_Lbl->CursorPosSet((__s32)m_CusPos);
				}
			}
		}

		// update language
		LcVkeyLangUpdate();

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinVkbd::DvlParaInfoBuild(void)
{
	// local -------------------
		//St_GrP5DvlScptParaInfo	Tv_ParaInfo;
	// code --------------------
		// inherited
		Cls_GrP5WinBase::DvlParaInfoBuild();
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinVkbd::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

	// inherited
	Cls_GrP5WinBase::DvlParaInfoAction(A_Action);

	// custom action

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5WinVkbd::DvlPreDelete(void)
{
	// local -------------------
	// code --------------------
		// not delete

		// delete prepare

		// inherited
		//Cls_GrP5WinBase::DvlPreDelete();

}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinVkbd::EvtKeyDown(__u8 A_Key)
{
	// local -------------------
	// code --------------------

		return	Cls_GrP5WinBase::EvtKeyDown(A_Key);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinVkbd::EvtKeyUp(__u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP5UoBase*	Tv_EvtHost;
	// code --------------------
		Tv_Result	=	FALSE;

		if(E_GrP5KeyEsc == A_Key)
		{
			if(E_GrP5ObjIdNone != m_HostObjId)
			{
				Tv_EvtHost	=	(Cls_GrP5UoBase*)GrP5UiObjGet(m_HostObjId);
				if(NULL != Tv_EvtHost)
				{
					Tv_EvtHost->EvtPopSelCancel();
				}
			}
			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_Result	=	Cls_GrP5WinBase::EvtKeyUp(A_Key);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::EvtBtnPress(__u16 A_ObjId)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::EvtBtnRelease(__u16 A_ObjId)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_Btn;
		__u32	Tv_Dat3;
		__u32	Tv_Dat2;
	// code --------------------
		Tv_Btn	=	(Cls_GrP5UoBase*)GrP5UiObjGet(A_ObjId);
		if(NULL != Tv_Btn)
		{
			// check user data
			Tv_Dat3	=	Tv_Btn->UserDataGet(3);
			if(0 != Tv_Dat3)
			{
				Tv_Dat2	=	Tv_Btn->UserDataGet(2);
				LcProcSpcKey(Tv_Dat3, Tv_Dat2);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::VkAddChar(WCHAR A_Code)
{
	// local -------------------
		BOOL8	Tv_IsDfltProc;
		__u16	Tv_First;
		__u16	Tv_Mid;
		__u16	Tv_Last;
		__u16	Tv_Ovwt;
		__u16	Tv_Cmb;
		__u16	Tv_Nxt;
	// code --------------------
		if(NULL == m_StrTxt)
		{
			return;
		}
		// init
		Tv_IsDfltProc	=	TRUE;
		// check Korean code
		if(((0x3131 <= A_Code) && (0x3163 >= A_Code)) || ((0xAC00 <= A_Code) && (0xD7A3 >= A_Code)))
		{
			//Korean
			if(E_GrP5WinVkbdKorCombNone != m_KorComb)
			{
				LcKorDivCode(m_StrTxt[m_CusPos], &Tv_First, &Tv_Mid, &Tv_Last);
				switch(m_KorComb)
				{
				case	E_GrP5WinVkbdKorCombMid:
					Tv_Mid	=	LcKorMidCodeGet(A_Code);
					if(0 != Tv_Mid)
					{
						Tv_Ovwt	=	LcKorCombine(Tv_First, Tv_Mid, Tv_Last);
						//combination
						VkCodeOvwt((WCHAR)Tv_Ovwt, FALSE);
						m_KorComb	=	LcKorNextStageGet(A_Code);	//go next stage
					}
					else
					{
						//move next
						VkCusRight();
						VkCodeIns(A_Code, FALSE);
					}
					break;
				case	E_GrP5WinVkbdKorCombDblMid:
					Tv_Cmb	=	LcKorMidCodeGet(A_Code);
					if(0 != Tv_Cmb)
					{
						Tv_Cmb	=	LcKorMidCombine(Tv_Mid, Tv_Cmb);
					}
					if(0 != Tv_Cmb)
					{
						//update code
						Tv_Ovwt	=	LcKorCombine(Tv_First, Tv_Cmb, Tv_Last);
						//combination
						VkCodeOvwt((WCHAR)Tv_Ovwt, FALSE);
						m_KorComb	=	E_GrP5WinVkbdKorCombLast;
					}
					else
					{
						//tail
						Tv_Last	=	LcKorLastCodeGet(A_Code);
						if(0 != Tv_Last)
						{
							//update code
							Tv_Ovwt	=	LcKorCombine(Tv_First, Tv_Mid, Tv_Last);
							//combination
							VkCodeOvwt((WCHAR)Tv_Ovwt, FALSE);
							m_KorComb	=	E_GrP5WinVkbdKorCombLast;
						}
						else
						{
							//move next
							VkCusRight();
							VkCodeIns((WCHAR)A_Code, FALSE);
						}
					}
					break;
				case	E_GrP5WinVkbdKorCombLast:
					//check exist last code
					Tv_Cmb	=	LcKorLastCodeGet(A_Code);
					if(0 != Tv_Cmb)
					{
						//check last code
						if(0 == Tv_Last)
						{
							Tv_Last	=	Tv_Cmb;
							//update code
							Tv_Ovwt	=	LcKorCombine(Tv_First, Tv_Mid, Tv_Last);
							//combination
							VkCodeOvwt((WCHAR)Tv_Ovwt, FALSE);
						}
						else
						{
							//try combination
							Tv_Cmb	=	LcKorLastCombine(Tv_Last, Tv_Cmb);
							if(0 != Tv_Cmb)
							{
								//update code
								Tv_Ovwt	=	LcKorCombine(Tv_First, Tv_Mid, Tv_Cmb);
								//combination
								VkCodeOvwt((WCHAR)Tv_Ovwt, FALSE);
							}
							else
							{
								//write first code
								VkCusRight();
								VkCodeIns((WCHAR)A_Code, FALSE);
								m_KorComb	=	LcKorNextStageGet(A_Code);
							}
						}
					}
					else
					{
						//check middle code
						Tv_Nxt	=	LcKorMidCodeGet(A_Code);
						if(0 != Tv_Nxt)
						{
							//divide last code
							LcKorLastDiv(Tv_Last, &Tv_Last, &Tv_Cmb);
							Tv_Ovwt	=	LcKorCombine(Tv_First, Tv_Mid, Tv_Last);
							VkCodeOvwt(Tv_Ovwt, TRUE);
							Tv_Ovwt	=	LcKorCombine(Tv_Cmb, Tv_Nxt, 0);
							VkCodeIns((WCHAR)Tv_Ovwt, FALSE);
						}
						else
						{
							//go next
							//move next
							VkCusRight();
							VkCodeIns((WCHAR)A_Code, FALSE);
						}
						m_KorComb	=	LcKorNextStageGet(A_Code);
					}
					break;
				}
			}
			else
			{
				//combinational code
				m_KorComb	=	LcKorNextStageGet(A_Code);
				VkCodeIns((WCHAR)A_Code, FALSE);
			}
			Tv_IsDfltProc	=	FALSE;
		}
		else if(0x309B == A_Code)
		{
			// japaness dakuon
			LcJpnProcDakuon();
			Tv_IsDfltProc	=	FALSE;
		}
		else if(0x309C == A_Code)
		{
			// japaness bidakuon
			LcJpnProcBidakuon();
			Tv_IsDfltProc	=	FALSE;
		}
		else if(0 == A_Code)
		{
			Tv_IsDfltProc	=	FALSE;
		}
		else
		{
			// check processed combination stage
			if(E_GrP5WinVkbdKorCombNone != m_KorComb)
			{
				VkCusRight();
				m_KorComb	=	E_GrP5WinVkbdKorCombNone;
			}
		}
		if(Tv_IsDfltProc)
		{
			VkCodeIns((WCHAR)A_Code, TRUE);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::LcKorDivCode(__u16 A_Code, __u16* A_PtrRtFirst, __u16* A_PtrRtMid, __u16* A_PtrRtLast)
{
	// local -------------------
		__u16	Tv_Code;
	// code --------------------
		*A_PtrRtFirst	=	0;
		*A_PtrRtMid		=	0;
		*A_PtrRtLast	=	0;
		if(0xAC00 <= A_Code)
		{
			//johap
			Tv_Code	=	A_Code - 0xAC00;
			*A_PtrRtFirst	=	(Tv_Code / 0x24C) + 1;
			Tv_Code	=	Tv_Code % 0x24C;
			*A_PtrRtMid		=	(Tv_Code / 28) + 1;
			*A_PtrRtLast	=	Tv_Code % 28;
		}
		else
		{
			if(0x314F > A_Code)
			{
				//first only
				Tv_Code	=	A_Code - 0x3131;
				if(0x3134 <= A_Code)
				{
					Tv_Code --;
				}
				if(0x3137 <= A_Code)
				{
					Tv_Code	=	Tv_Code - 2;
				}
				if(0x3141 <= A_Code)
				{
					Tv_Code	=	Tv_Code - 7;
				}
				if(0x3145 <= A_Code)
				{
					Tv_Code	--;
				}
				*A_PtrRtFirst	=	Tv_Code + 1;
			}
			else
			{
				//middle only
				*A_PtrRtMid	=	A_Code - 0x314F + 1;
			}
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP5WinVkbd::LcKorMidCodeGet(__u16 A_Code)
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if((0x314F <= A_Code) && (0x3163 >= A_Code))
		{
			Tv_Result	=	A_Code - 0x314F + 1;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5WinVkbd::LcKorCombine(__u16 A_First, __u16 A_Mid, __u16 A_Last)
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if((0 == A_Last) && ((0 == A_First) || (0 == A_Mid)))
		{
			//jamo mode
			if(0 != A_First)
			{
				//jaum mode
				Tv_Result	=	0x3131 + A_First - 1;
				if(3 <= A_First)
				{
					Tv_Result	++;
				}
				if(4 <= A_First)
				{
					Tv_Result	=	Tv_Result + 2;
				}
				if(7 <= A_First)
				{
					Tv_Result	=	Tv_Result + 7;
				}
				if(10 <= A_First)
				{
					Tv_Result ++;
				}
			}
			else
			{
				//moum mode
				Tv_Result	=	0x314F + A_Mid - 1;
			}
		}
		else
		{
			Tv_Result	=	0xAC00 + ((A_First - 1) * 0x24C) + ((A_Mid - 1) * 28) + A_Last;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP5WinVkbd::LcKorNextStageGet(__u16 A_Code)
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrP5WinVkbdKorCombNone;

		if((0x3131 <= A_Code) && (0x3163 >= A_Code))
		{
			Tv_Result	=	E_GrP5WinVkbdKorCombMid;
			if(0x314F <= A_Code)
			{
				Tv_Result	=	E_GrP5WinVkbdKorCombLast;
				if((0x3157 == A_Code) || (0x315C == A_Code) || (0x3161 == A_Code))
				{
					Tv_Result	=	E_GrP5WinVkbdKorCombDblMid;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5WinVkbd::LcKorMidCombine(__u16 A_Org, __u16 A_Add)
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		//check 
		switch(A_Org)
		{
		case	9:
			if(1 == A_Add)
			{
				Tv_Result	=	10;
			}
			else if(2 == A_Add)
			{
				Tv_Result	=	11;
			}
			else if(21 == A_Add)
			{
				Tv_Result	=	12;
			}
			break;
		case	14:
			if(5 == A_Add)
			{
				Tv_Result	=	15;
			}
			else if(6 == A_Add)
			{
				Tv_Result	=	16;
			}
			else if(21 == A_Add)
			{
				Tv_Result	=	17;
			}
			break;
		case	19:
			if(21 == A_Add)
			{
				Tv_Result	=	20;
			}
			break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5WinVkbd::LcKorLastCodeGet(__u16 A_Code)
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if((0x3131 <= A_Code) && (0x314E >= A_Code) && (0x3138 != A_Code) && (0x3143 != A_Code) && (0x3149 != A_Code))
		{
			Tv_Result	=	A_Code - 0x3131 + 1;
			if(0x3138 < A_Code)
			{
				Tv_Result --;
			}
			if(0x3143 <= A_Code)
			{
				Tv_Result --;
			}
			if(0x3149 < A_Code)
			{
				Tv_Result --;
			}
		}
		return	Tv_Result;

}
//--------------------------------------------------------------------
__u16	Cls_GrP5WinVkbd::LcKorLastCombine(__u16 A_Org, __u16 A_Add)
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		switch(A_Org)
		{
		case	1:
			if(19 == A_Add)
			{
				Tv_Result	=	3;
			}
			break;
		case	4:
			if(22 == A_Add)
			{
				Tv_Result	=	5;
			}
			else if(27 == A_Add)
			{
				Tv_Result	=	6;
			}
			break;
		case	8:
			if(1 == A_Add)
			{
				Tv_Result	=	9;
			}
			else if(16 == A_Add)
			{
				Tv_Result	=	10;
			}
			else if(17 == A_Add)
			{
				Tv_Result	=	11;
			}
			else if(19 == A_Add)
			{
				Tv_Result	=	12;
			}
			else if(25 == A_Add)
			{
				Tv_Result	=	13;
			}
			else if(26 == A_Add)
			{
				Tv_Result	=	14;
			}
			else if(27 == A_Add)
			{
				Tv_Result	=	15;
			}
			break;
		case	17:
			if(19 == A_Add)
			{
				Tv_Result	=	18;
			}
			break;
		}
		return	Tv_Result;

}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::LcKorLastDiv(__u16 A_Org, __u16* A_PtrRtLast, __u16* A_PtrRtFirst)
{
	// local -------------------
	// code --------------------
		*A_PtrRtLast	=	V_GrP5DWinVkbdKorLastDivTbl[A_Org].Last;
		*A_PtrRtFirst	=	V_GrP5DWinVkbdKorLastDivTbl[A_Org].First;
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::VkLabelRedraw(void)
{
	// local -------------------
		Cls_GrP5UoLblRef*	Tv_Lbl;
	// code --------------------
		// get refernce label
		
		Tv_Lbl	=	(Cls_GrP5UoLblRef*)RtlUoFindByScptId((__u16)m_ParaUoBase.UserData[3]);
		if(NULL != Tv_Lbl )
		{
			if(Tv_Lbl->IsInheritClass(E_GrP5ClassIdLblRef))
			{
				Tv_Lbl->CursorPosSet((__s32)m_CusPos);
			}
			Tv_Lbl->ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::VkClear(void)
{
		if(NULL == m_StrTxt)
		{
			return;
		}
		GrStrClear(m_StrTxt);
		m_CusPos	=	0;
		m_Length	=	0;
		// redraw
		VkLabelRedraw();
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::VkCusHome(void)
{
		if(NULL == m_StrTxt)
		{
			return;
		}

		m_CusPos	=	0;
		VkLabelRedraw();
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::VkCusEnd(void)
{
		if(NULL == m_StrTxt)
		{
			return;
		}

		m_CusPos	=	(__s32)m_Length;
		VkLabelRedraw();
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::VkCusLeft(void)
{
		if(NULL == m_StrTxt)
		{
			return;
		}

		if(0 < m_CusPos)
		{
			m_CusPos --;
			VkLabelRedraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::VkCusRight(void)
{
		if(NULL == m_StrTxt)
		{
			return;
		}

		if(m_Length > (__u32)m_CusPos)
		{
			m_CusPos ++;
			VkLabelRedraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::VkCodeIns(WCHAR A_Code, BOOL8 A_IsMoveCus)
{
	// local -------------------
		WCHAR	Tv_StrCode[2];
	// code --------------------
		if(NULL == m_StrTxt)
		{
			return;
		}

		// init
		Tv_StrCode[0]	=	A_Code;
		Tv_StrCode[1]	=	0;
		// check full
		if(m_MaxLen > m_Length)
		{
			// insert
			GrStrWins(m_StrTxt, Tv_StrCode, (__u32)m_CusPos);
			m_Length ++;
			if(A_IsMoveCus)
			{
				VkCusRight();
			}
			VkLabelRedraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::VkCodeOvwt(WCHAR A_Code, BOOL8 A_IsMoveCus)
{
		if(NULL == m_StrTxt)
		{
			return;
		}

		if(0 != A_Code)
		{
			if(m_Length > (__u32)m_CusPos)
			{
				m_StrTxt[m_CusPos]	=	A_Code;
				if(A_IsMoveCus)
				{
					VkCusRight();
				}
				VkLabelRedraw();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::VkBackspace(void)
{
		if(NULL == m_StrTxt)
		{
			return;
		}

		if(0 != m_CusPos)
		{
			m_CusPos --;
			GrStrWdel(m_StrTxt, (__u32)m_CusPos, 1);
			m_Length --;
			VkLabelRedraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::VkDelete(void)
{
		if(NULL == m_StrTxt)
		{
			return;
		}

		if(m_Length > (__u32)m_CusPos)
		{
			GrStrWdel(m_StrTxt, (__u32)m_CusPos, 1);
			m_Length --;
			VkLabelRedraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::LcVkeyLangUpdate(void)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoSlv;
	// code --------------------
		Tv_UoSlv	=	m_ObjSlave;
		while(NULL != Tv_UoSlv)
		{
			LcSubVkeyLangSet(Tv_UoSlv);
			// next
			Tv_UoSlv	=	Tv_UoSlv->NextObjGet();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::LcSubVkeyLangSet(Cls_GrP5UoBase* A_UiObj)
{
	// local -------------------
		Cls_GrP5UoBase*	Tv_UoSlv;
	// code --------------------
		// check object
		if(A_UiObj->IsInheritClass(E_GrP5ClassIdBtnVkey))
		{
			((Cls_GrP5UoBtnVkey*)A_UiObj)->VkeyLanguageSet(m_LangIdx, m_ShftIdx);
		}
		else
		{
			// check slave
			Tv_UoSlv	=	A_UiObj->SlaveObjGet();
			while(NULL != Tv_UoSlv)
			{
				LcSubVkeyLangSet(Tv_UoSlv);
				// next
				Tv_UoSlv	=	Tv_UoSlv->NextObjGet();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::LcProcSpcKey(__u32 A_Cmd, __u32 A_Para)
{
	// local -------------------
	// code --------------------
		switch(A_Cmd)
		{
			case 1:
				// privoius language
				LcLangPrivous();
				break;
			case 2:
				// next langulage
				LcLangNext();
				break;
			case 3:
				// spacific langulage
				LcLangSet(A_Para);
				break;
			case  4:
				// shift
				LcShiftSet();
				break;
			case 5:
				// back space
				VkBackspace();
				break;
			case 6:
				// delete
				VkDelete();
				break;
			case 7:
				// clear
				VkClear();
				break;
			case 8:
				// esc
				CloseRequest();
				break;
			case 9:
				// ok
				m_IsUpdtRslt	=	TRUE;
				CloseRequest();
				break;
			case  10:
				// left
				VkCusLeft();
				break;
			case  11:
				// right
				VkCusRight();
				break;
			case 12:
				VkAddChar(0x20);
				break;
			case 13:
				if(m_LangIdx != V_GrP5Env.Language)
				{
					LcLangSet(V_GrP5Env.Language);
				}
				else
				{
					LcLangSet(0);
				}
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::LcLangPrivous(void)
{
	// local -------------------
		__u32	Tv_LngIdx;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_LngIdx		=	m_LangIdx;
		if(0 == Tv_LngIdx)
		{
			Tv_LngIdx =	E_GrP5ScptLanguageMaxCnt - 1;
		}
		else
		{
			Tv_LngIdx --;
		}
		for( Tv_WkIdx = 0; Tv_WkIdx < E_GrP5ScptLanguageMaxCnt; Tv_WkIdx++)
		{
			if(0 != ((1 << Tv_LngIdx) & m_ParaUoBase.UserData[2]))
			{
				//found
				m_LangIdx	=	Tv_LngIdx;
				break;
			}
			// next
			if(0 == Tv_LngIdx)
			{
				Tv_LngIdx =	E_GrP5ScptLanguageMaxCnt - 1;
			}
			else
			{
				Tv_LngIdx --;
			}
		}

		m_ShftIdx	=	0;	// reset shift

		LcVkeyLangUpdate();
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::LcLangNext(void)
{
	// local -------------------
		__u32	Tv_LngIdx;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_LngIdx	=	m_LangIdx + 1;
		if(E_GrP5ScptLanguageMaxCnt <= Tv_LngIdx)
		{
			Tv_LngIdx =	0;
		}
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5ScptLanguageMaxCnt; Tv_WkIdx++)
		{
			if(0 != ((1 << Tv_LngIdx) & m_ParaUoBase.UserData[2]))
			{
				//found
				m_LangIdx	=	Tv_LngIdx;
				break;
			}
			// next
			Tv_LngIdx ++;
			if(E_GrP5ScptLanguageMaxCnt <= Tv_LngIdx)
			{
				Tv_LngIdx =	0;
			}
		}

		m_ShftIdx	=	0;	// reset shift

		LcVkeyLangUpdate();
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::LcLangSet(__u32 A_LangIdx)
{
	// local -------------------
	// code --------------------
		if(E_GrP5ScptLanguageMaxCnt > A_LangIdx)
		{
			m_LangIdx	=	A_LangIdx;
			m_ShftIdx	=	0;	// reset shift

			LcVkeyLangUpdate();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::LcShiftSet(void)
{
	// local -------------------
	// code --------------------
		m_ShftIdx	++;
		if(E_GrP5VkbdShftMax <= m_ShftIdx)
		{
			m_ShftIdx	=	0;
		}
		LcVkeyLangUpdate();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinVkbd::LcJpnCharDakInfo(__u16 A_Code, __u16* A_PtrRtStd, __u16* A_PtrRtDak)
{
	// local -------------------
		BOOL8	Tv_Reuslt;
		__u16	Tv_Base;
	// code --------------------
		Tv_Reuslt	=	TRUE;

		if((0x304B <= A_Code) && (0x3062 >= A_Code))
		{
			// couple
			if(0 != (1 & A_Code))
			{
				*A_PtrRtStd	=	A_Code;
				*A_PtrRtDak	=	A_Code + 1;
			}
			else
			{
				*A_PtrRtStd	=	A_Code - 1;
				*A_PtrRtDak	=	A_Code;
			}
		}
		else if((0x3064 <= A_Code) && (0x3069 >= A_Code))
		{
			// couple
			if(0 == (1 & A_Code))
			{
				*A_PtrRtStd	=	A_Code;
				*A_PtrRtDak	=	A_Code + 1;
			}
			else
			{
				*A_PtrRtStd	=	A_Code - 1;
				*A_PtrRtDak	=	A_Code;
			}
		}
		else if((0x306F <= A_Code) && (0x307D >= A_Code))
		{
			// with bidakuon
			Tv_Base		=	A_Code - 0x306F;
			Tv_Base		=	Tv_Base - (Tv_Base % 3);
			*A_PtrRtStd	=	Tv_Base + 0x306F;
			*A_PtrRtDak	=	Tv_Base + 0x306F + 1;
		}
		else if((0x30AB <= A_Code) && (0x30C2 >= A_Code))
		{
			// couple
			if(0 != (1 & A_Code))
			{
				*A_PtrRtStd	=	A_Code;
				*A_PtrRtDak	=	A_Code + 1;
			}
			else
			{
				*A_PtrRtStd	=	A_Code - 1;
				*A_PtrRtDak	=	A_Code;
			}
		}
		else if((0x30C4 <= A_Code) && (0x30C9 >= A_Code))
		{
			// couple
			if(0 == (1 & A_Code))
			{
				*A_PtrRtStd	=	A_Code;
				*A_PtrRtDak	=	A_Code + 1;
			}
			else
			{
				*A_PtrRtStd	=	A_Code - 1;
				*A_PtrRtDak	=	A_Code;
			}
		}
		else if((0x30CF <= A_Code) && (0x30DD >= A_Code))
		{
			// with bidakuon
			Tv_Base		=	A_Code - 0x30CF;
			Tv_Base		=	Tv_Base - (Tv_Base % 3);
			*A_PtrRtStd	=	Tv_Base + 0x30CF;
			*A_PtrRtDak	=	Tv_Base + 0x30CF + 1;
		}
		else
		{
			Tv_Reuslt	=	FALSE;
		}

		return	Tv_Reuslt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5WinVkbd::LcJpnCharBidInfo(__u16 A_Code, __u16* A_PtrRtStd, __u16* A_PtrRtBid)
{
	// local -------------------
		BOOL8	Tv_Reuslt;
		__u16	Tv_Base;
	// code --------------------
		Tv_Reuslt	=	TRUE;

		if((0x306F <= A_Code) && (0x307D >= A_Code))
		{
			// with bidakuon
			Tv_Base		=	A_Code - 0x306F;
			Tv_Base		=	Tv_Base - (Tv_Base % 3);
			*A_PtrRtStd	=	Tv_Base + 0x306F;
			*A_PtrRtBid	=	Tv_Base + 0x306F + 2;
		}
		else if((0x30CF <= A_Code) && (0x30DD >= A_Code))
		{
			// with bidakuon
			Tv_Base		=	A_Code - 0x30CF;
			Tv_Base		=	Tv_Base - (Tv_Base % 3);
			*A_PtrRtStd	=	Tv_Base + 0x30CF;
			*A_PtrRtBid	=	Tv_Base + 0x30CF + 2;
		}
		else
		{
			Tv_Reuslt	=	FALSE;
		}

		return	Tv_Reuslt;
}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::LcJpnProcDakuon(void)
{
	// local -------------------
		__u16	Tv_CodeStd;
		__u16	Tv_CodeDak;
	// code --------------------
		if(NULL == m_StrTxt)
		{
			return;
		}

		// get privous charactore
		if(0 != m_CusPos)
		{
			if(LcJpnCharDakInfo(m_StrTxt[m_CusPos - 1],&Tv_CodeStd,&Tv_CodeDak))
			{
				// switch code
				if(m_StrTxt[m_CusPos - 1] == Tv_CodeDak)
				{
					m_StrTxt[m_CusPos - 1]		=	Tv_CodeStd;
				}
				else
				{
					m_StrTxt[m_CusPos - 1]		=	Tv_CodeDak;
				}
				// redraw
				VkLabelRedraw();
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5WinVkbd::LcJpnProcBidakuon(void)
{
	// local -------------------
		__u16	Tv_CodeStd;
		__u16	Tv_CodeBid;
	// code --------------------
		if(NULL == m_StrTxt)
		{
			return;
		}

		// get privous charactore
		if(0 != m_CusPos)
		{
			if(LcJpnCharBidInfo(m_StrTxt[m_CusPos - 1], &Tv_CodeStd, &Tv_CodeBid))
			{
				// switch code
				if(m_StrTxt[m_CusPos - 1] == Tv_CodeBid)
				{
					m_StrTxt[m_CusPos - 1]		=	Tv_CodeStd;
				}
				else
				{
					m_StrTxt[m_CusPos - 1]		=	Tv_CodeBid;
				}
				// redraw
				VkLabelRedraw();
			}
		}
}
//--------------------------------------------------------------------
