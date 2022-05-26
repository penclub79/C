/*
Platform 5 UI object Spin setup text base

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoSpinLtBase.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>
#include <P5/GrP5Key.h>

#ifdef GR_P5_EDITOR

#include <P5\Dvl\GrP5DvlBase.h>

#endif

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;

#ifdef GR_P5_EDITOR

// relate position
#define	E_GrP5UoSpinLtBaseParaInfoCnt							2

St_GrP5DvlScptParaInfo	V_GrP5UoSpinLtBaseParaInfo[E_GrP5UoSpinLtBaseParaInfoCnt]	=
{
	{L"Spin Local Text Base", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"IsEdit", E_GrVarTypeBool8, 1, (void*)&((Ptr_GrP5UoSpinLtBasePara)0)->IsEdit, TRUE, E_GrP5DvlParaInfoActDefault, L"String edit able"},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoSpinLtBase::Cls_GrP5UoSpinLtBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoSpinBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
		__u32	Tv_ItmIdx;
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdSpinLtBase, &m_ParaUoSpinLtBase, sizeof(m_ParaUoSpinLtBase), NULL);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoSpinLtBase, sizeof(m_ParaUoSpinLtBase));

		GrDumyZeroMem(m_Items, sizeof(m_Items));

		m_ItemCnt		=	0;
		m_SelIdx		=	0;

		// build text buffer
		for(Tv_ItmIdx = 0; Tv_ItmIdx < E_GrP5SpinLtItemMaxCnt; Tv_ItmIdx++)
		{
			m_Items[Tv_ItmIdx].StrTxt	=	GrP5TxtBufAlloc(m_ObjId);
			if(NULL != m_Items[Tv_ItmIdx].StrTxt)
			{
				GrStrClear(m_Items[Tv_ItmIdx].StrTxt);
			}
		}

}
//--------------------------------------------------------------------
Cls_GrP5UoSpinLtBase::~Cls_GrP5UoSpinLtBase(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);
}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinLtBase::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoSpinLtBaseParaInfo, E_GrP5UoSpinLtBaseParaInfoCnt, &m_ParaUoSpinLtBase);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSpinLtBase::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSpinBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinLtBase::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		Cls_GrP5UoSpinBase::EvtScptDecDone();


}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinLtBase::DoIncrease(void)
{
	// local -------------------
		__u32	Tv_Val;
	// code --------------------
		Tv_Val	=	m_SelIdx + 1;
		if(Tv_Val >= m_ItemCnt)
		{
			Tv_Val	=	0;
			if(m_ParaUoSpinBase.IsNotRotate)
			{
				if(0 != m_ItemCnt)
				{
					Tv_Val	=	m_ItemCnt - 1;
				}
			}
		}
		m_SelIdx	=	Tv_Val;

		ValueWrite();
		DoEvtSpinValChg();

		ReqDraw(FALSE);

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinLtBase::DoDecrease(void)
{
	// local -------------------
		__u32	Tv_Val;
	// code --------------------
		Tv_Val	=	m_SelIdx;
		if(0 == Tv_Val)
		{
			if(!m_ParaUoSpinBase.IsNotRotate)
			{
				if(0 != m_ItemCnt)
				{
					Tv_Val	=	m_ItemCnt - 1;
				}
			}
		}
		else
		{
			Tv_Val --;
		}
		m_SelIdx	=	Tv_Val;

		ValueWrite();
		DoEvtSpinValChg();

		ReqDraw(FALSE);

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinLtBase::ProcSpinClickCont(void)
{
	// local -------------------
		BOOL8	Tv_IsPw;
	// code --------------------
		if(m_ParaUoSpinLtBase.IsEdit)
		{
			// popup virtual keyboard
			ValueRead();
			if(NULL != m_SkinCtlSpinBase.StrDisp)
			{
				Tv_IsPw		=	FALSE;
				if(0 != (E_GrP5FontAtbPassword & m_ParaUoSpinBase.TxtPara.Atb))
				{
					Tv_IsPw	=	TRUE;
				}
				GrP5VkbdCreate(E_GrP5VkbdTypeNormal, this, E_GrP5CtrlIdNone, m_SkinCtlSpinBase.StrDisp, E_GrP5SpinLtItemMaxCnt - 1, Tv_IsPw, 0);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinLtBase::EvtVkbdInput(void)
{
	// local -------------------
	// code --------------------
		ReqKeyFocusSet();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinLtBase::EvtVkbdCancel(void)
{
	// local -------------------
	// code --------------------
		ReqKeyFocusSet();
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
__u32	Cls_GrP5UoSpinLtBase::SelIdxGet(void)
{
		ValueRead();
		return	m_SelIdx;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinLtBase::SelIdxSet(__u32 A_Idx)
{
		if(A_Idx < m_ItemCnt)
		{
			m_SelIdx	=	A_Idx;
			ValueWrite();
			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------
__s32	Cls_GrP5UoSpinLtBase::ItemAdd(WCHAR* A_Str,__u32 A_UserData)
{
	// local -------------------
		__s32	Tv_Result;
	// code --------------------
		Tv_Result	=	-1;

		if(E_GrP5SpinLtItemMaxCnt > m_ItemCnt)
		{
			if(NULL != m_Items[m_ItemCnt].StrTxt)
			{
				GrStrWcopy(m_Items[m_ItemCnt].StrTxt, A_Str, 31);
				m_Items[m_ItemCnt].UserData	=	A_UserData;
				Tv_Result	=	(__s32)m_ItemCnt;
				m_ItemCnt ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinLtBase::ItemClear(void)
{
	// local -------------------
	// code --------------------
		m_ItemCnt		=	0;
		ReqDraw(FALSE);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinLtBase::ItemTextGet(__u32 A_Idx, WCHAR* A_StrRt)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(m_ItemCnt > A_Idx)
		{
			if(NULL != m_Items[A_Idx].StrTxt)
			{
				GrStrWcopy(A_StrRt, m_Items[A_Idx].StrTxt);
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinLtBase::ItemTextSet(__u32 A_Idx, WCHAR* A_Str)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if(m_ItemCnt > A_Idx)
		{
			if(NULL != m_Items[A_Idx].StrTxt)
			{
				GrStrWcopy(m_Items[A_Idx].StrTxt, A_Str, 31);
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinLtBase::ItemUserDataGet(__u32 A_Idx, __u32* A_PtrRtUserData)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if(m_ItemCnt > A_Idx)
		{
			*A_PtrRtUserData	=	m_Items[A_Idx].UserData;
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5UoSpinLtBase::ItemUserDataSet(__u32 A_Idx, __u32 A_UserData)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if(m_ItemCnt > A_Idx)
		{
			m_Items[A_Idx].UserData	=	A_UserData;
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSpinLtBase::ValueRead(void)
{
	// local -------------------
	// code --------------------
		m_SkinCtlSpinBase.StrDisp	=	NULL;

		if(m_SelIdx < m_ItemCnt)
		{
			m_SkinCtlSpinBase.StrDisp	=	m_Items[m_SelIdx].StrTxt;
		}

}
//--------------------------------------------------------------------
