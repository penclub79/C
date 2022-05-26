/*
		platform tree base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoTreeBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>

#include <P4/GrP4DoSbarBase.h>

#include <GrDebug.h>

#ifdef GR_P4_EDITOR
#include	<P4/Dvl/GrP4DvlBase.h>
#include	<P4/Dvl/GrP4DvlMng.h>
#endif

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrP4Mng*	V_GrP4Mng;


#ifdef GR_P4_EDITOR

#define E_GrP4DoTreeBaseParaCnt		1

St_GrP4DvlDoPrInfoData	V_GrP4DoTreeBaseParaInfo[E_GrP4DoTreeBaseParaCnt]	=	
{
	{	L"Paint BG",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTreeBasePara)0)->PntBg,NULL,	L"Paint background code.\n0 = None, 1 = basic code"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoTreeBase::Cls_GrP4DoTreeBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
		__u8	Tv_ItmIdx;
	// code --------------------
		//init
		m_ClassId				=	E_GrP4DoClassTreeBase;
		m_ParaSize			=	m_ParaSize + sizeof(m_ParaTreeBase) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaTreeBase, sizeof(m_ParaTreeBase) );
		m_ParaTreeBase.PntBg	=	E_GrP4ScptPstIdDflt;

		GrDumyZeroMem(m_TreeBaseItemTbl, sizeof(m_TreeBaseItemTbl));
		GrDumyZeroMem(m_DispTbl, sizeof(m_DispTbl));

		m_DispItemCnt		=	0;
		m_DispAbleCnt		=	0;
		m_DispPos				=	0;
		m_ScrlMax				=	0;
		m_DispAreaCnt		=	0;
		m_IsDispBldReq	=	FALSE;

		for (Tv_ItmIdx=0; Tv_ItmIdx < E_GrP4DoTreeBaseItemMaxCnt;Tv_ItmIdx++)
		{
			m_TreeBaseItemTbl[Tv_ItmIdx].ItmIdx	=	Tv_ItmIdx;
		}

}
//--------------------------------------------------------------------
Cls_GrP4DoTreeBase::~Cls_GrP4DoTreeBase( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoTreeBase::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassTreeBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaTreeBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaTreeBase, sizeof(m_ParaTreeBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaTreeBase)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaTreeBase, sizeof(m_ParaTreeBase) , m_PtrScpt , E_GrP4DoClassTreeBase );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoTreeBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ItmIdx;
	// code --------------------
		Tv_Result	=	TRUE;
		switch (A_Cmd)
		{
			case E_GrP4UpiItemAdd:
				LcItemAdd((__u32)A_PtrInt[1], (__u32)A_PtrInt[2], (__u32)A_PtrInt[3], &Tv_ItmIdx);
				break;
			case E_GrP4UpiItemDel:
				LcItemDel((__u32)A_PtrInt[1]);
				break;
			case E_GrP4UpiItemIconSet:
				LcItemIconSet((__u32)A_PtrInt[1], (__u8)A_PtrInt[3]);
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeBase::ProcPaint( void )
{
		// paint item
		LcPntCont();

		PaintScriptPush( m_ParaTreeBase.PntBg, &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoTreeBase::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaTreeBase, V_GrP4DoTreeBaseParaInfo, E_GrP4DoTreeBaseParaCnt );
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoTreeBase::LcItemAdd(__u32 A_Id, __u32 A_PrntId, __u32 A_UserData, __u8* A_PtrRtItmIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ItmIdx;
		__s16	Tv_PrntIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrP4DoTreeItemIdNone != A_Id)
		{
			Tv_Result		=	TRUE;
			Tv_PrntIdx	=	-1;

			// check exist ID
			for (Tv_ItmIdx=0; Tv_ItmIdx <E_GrP4DoTreeBaseItemMaxCnt; Tv_ItmIdx++)
			{
				if (A_Id == m_TreeBaseItemTbl[Tv_ItmIdx].Id)
				{
					// found
					Tv_Result	=	FALSE;
					DbgMsgPrint("Cls_GrP4DoTreeBase::LcItemAdd - ID(%d) already exist\n", A_Id);
					break;
				}
			}

			if (Tv_Result)
			{
				// find parent
				if (E_GrP4DoTreeItemIdNone != A_PrntId)
				{
					Tv_Result	=	FALSE;
					for (Tv_ItmIdx=0; Tv_ItmIdx <E_GrP4DoTreeBaseItemMaxCnt; Tv_ItmIdx++)
					{
						if (A_PrntId == m_TreeBaseItemTbl[Tv_ItmIdx].Id)
						{
							// found
							Tv_PrntIdx	=	(__s16)Tv_ItmIdx;
							Tv_Result		=	TRUE;
							break;
						}
					}
				}
			}

			if (Tv_Result)
			{
				Tv_Result	=	FALSE;
				// find empty slot
				for (Tv_ItmIdx=0; Tv_ItmIdx <E_GrP4DoTreeBaseItemMaxCnt; Tv_ItmIdx++)
				{
					if (E_GrP4DoTreeItemIdNone == m_TreeBaseItemTbl[Tv_ItmIdx].Id)
					{
						// found
						m_TreeBaseItemTbl[Tv_ItmIdx].Id					=	A_Id;
						m_TreeBaseItemTbl[Tv_ItmIdx].PrntIdx		=	Tv_PrntIdx;
						m_TreeBaseItemTbl[Tv_ItmIdx].IsChildExp	=	TRUE;
						m_TreeBaseItemTbl[Tv_ItmIdx].UserData		=	A_UserData;
						m_TreeBaseItemTbl[Tv_ItmIdx].IconIdx		=	0;

						*A_PtrRtItmIdx	=	Tv_ItmIdx;

						m_IsDispBldReq	=	TRUE;

						Tv_Result	=	TRUE;
						break;
					}
				}

			}
		}


		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeBase::LcDispAddSlave(__s16 A_PrntIdx, __u16 A_IdentPos)
{
	// local -------------------
		__s16	Tv_ItmIdx;
	// code --------------------
		for (Tv_ItmIdx=0; Tv_ItmIdx < E_GrP4DoTreeBaseItemMaxCnt; Tv_ItmIdx++)
		{
			if ((E_GrP4DoTreeItemIdNone != m_TreeBaseItemTbl[Tv_ItmIdx].Id) && (A_PrntIdx == m_TreeBaseItemTbl[Tv_ItmIdx].PrntIdx))
			{
				// find add
				m_DispTbl[m_DispItemCnt]	=	&m_TreeBaseItemTbl[Tv_ItmIdx];
				m_TreeBaseItemTbl[Tv_ItmIdx].IdenPos	=	A_IdentPos;
				m_DispItemCnt ++;
				// find slave
				if (m_TreeBaseItemTbl[Tv_ItmIdx].IsChildExp)
				{
					LcDispAddSlave(Tv_ItmIdx, A_IdentPos + m_ParaTreeBase.IdentSize);
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeBase::LcDispTblBld(void)
{
	// local -------------------
		__s16	Tv_ItmIdx;
	// code --------------------
		m_DispItemCnt	=	0;
		// find top items
		for (Tv_ItmIdx=0; Tv_ItmIdx < E_GrP4DoTreeBaseItemMaxCnt;Tv_ItmIdx++)
		{
			if ((E_GrP4DoTreeItemIdNone != m_TreeBaseItemTbl[Tv_ItmIdx].Id) && (0 > m_TreeBaseItemTbl[Tv_ItmIdx].PrntIdx))
			{
				// found
				m_DispTbl[m_DispItemCnt]	=	&m_TreeBaseItemTbl[Tv_ItmIdx];
				m_TreeBaseItemTbl[Tv_ItmIdx].IdenPos	=	0;
				m_DispItemCnt ++;

				// find slave item
				if (m_TreeBaseItemTbl[Tv_ItmIdx].IsChildExp)
				{
					LcDispAddSlave(Tv_ItmIdx,m_ParaTreeBase.IdentSize);
				}
			}
		}

		
		if (0 != m_DispAreaCnt)
		{
			// calculate scroll max
			m_ScrlMax	=	0;
			if (m_DispItemCnt > m_DispAreaCnt)
			{
				m_ScrlMax	=	m_DispItemCnt - (__u8)m_DispAreaCnt;
			}
			// update scroll bar
			if (E_GrP4ScptIdNone != m_ParaTreeBase.RelSbarId)
			{
				// calculate 
				V_GrP4Mng->DpcUpiCall(((Cls_GrP4ConBase*)m_ObjCon)->IndexGet(), m_ObjWin->ScptIdGet(), m_ParaTreeBase.RelSbarId, E_GrP4UpiMaxValueSet, (__s32)m_ScrlMax);
			}
		}

		m_IsDispBldReq	=	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeBase::LcSubItemDel(__s16 A_PrntIdx)
{
	// local -------------------
		__s16	Tv_ItmIdx;
	// code --------------------
		for (Tv_ItmIdx=0; Tv_ItmIdx < E_GrP4DoTreeBaseItemMaxCnt; Tv_ItmIdx++)
		{
			if ((E_GrP4DoTreeItemIdNone != m_TreeBaseItemTbl[Tv_ItmIdx].Id) && (A_PrntIdx == m_TreeBaseItemTbl[Tv_ItmIdx].PrntIdx))
			{
				// delete sub
				LcSubItemDel(Tv_ItmIdx);
				// delete this
				m_TreeBaseItemTbl[Tv_ItmIdx].Id				=	E_GrP4DoTreeItemIdNone;
				m_TreeBaseItemTbl[Tv_ItmIdx].PrntIdx	=	-1;
				m_TreeBaseItemTbl[Tv_ItmIdx].UserData	=	0;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoTreeBase::LcItemDel(__u32 A_Id)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s16	Tv_ItmIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		// find item
		for (Tv_ItmIdx=0; Tv_ItmIdx < E_GrP4DoTreeBaseItemMaxCnt; Tv_ItmIdx++)
		{
			if ((E_GrP4DoTreeItemIdNone != m_TreeBaseItemTbl[Tv_ItmIdx].Id) && (A_Id == m_TreeBaseItemTbl[Tv_ItmIdx].Id))
			{
				// found
				// delete slave items
				LcSubItemDel(Tv_ItmIdx);

				// delete item now
				m_TreeBaseItemTbl[Tv_ItmIdx].Id				=	E_GrP4DoTreeItemIdNone;
				m_TreeBaseItemTbl[Tv_ItmIdx].PrntIdx	=	-1;
				m_TreeBaseItemTbl[Tv_ItmIdx].UserData	=	0;

				m_IsDispBldReq	=	TRUE;
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s16	Cls_GrP4DoTreeBase::LcItemFindById(__u32 A_Id)
{
	// local -------------------
		__s16	Tv_Result;
		__s16	Tv_ItmIdx;
	// code --------------------
		Tv_Result	=	-1;

		for (Tv_ItmIdx=0; Tv_ItmIdx < E_GrP4DoTreeBaseItemMaxCnt; Tv_ItmIdx++)
		{
			if ((E_GrP4DoTreeItemIdNone != m_TreeBaseItemTbl[Tv_ItmIdx].Id) && (A_Id == m_TreeBaseItemTbl[Tv_ItmIdx].Id))
			{
				// found
				Tv_Result	=	Tv_ItmIdx;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeBase::LcPntCont(void)
{
	// local -------------------
		__u8	Tv_DispIdx;
	// code --------------------
		// calculate contents area rect
		m_PntRcCont.left		=	m_PntRcScr.left + (__s32)m_ParaTreeBase.GapLeft;
		m_PntRcCont.top			=	m_PntRcScr.top + (__s32)m_ParaTreeBase.GapTop;
		m_PntRcCont.right		=	m_PntRcScr.right - (__s32)m_ParaTreeBase.GapRight;
		m_PntRcCont.bottom	=	m_PntRcScr.bottom - (__s32)m_ParaTreeBase.GapBottom;

		// check draw able
		if ((m_PntRcCont.left < m_PntRcCont.right) && (m_PntRcCont.top < m_PntRcCont.bottom))
		{
			// calculate view able item
			if (0 == m_DispAreaCnt)
			{
				m_DispAreaCnt	=	(m_PntRcCont.bottom - m_PntRcCont.top) / (__s32)m_ParaTreeBase.ItemHeight;
			}
			if (0 != m_DispAreaCnt)
			{
				// check need rebuild item
				if (m_IsDispBldReq)
				{
					LcDispTblBld();
					if (((__s32)m_DispPos + m_DispAreaCnt) > (__s32)m_DispItemCnt)
					{
						if ((__s32)m_DispAreaCnt >= m_DispItemCnt)
						{
							m_DispPos	=	0;
						}
						else
						{
							m_DispPos	=	m_DispItemCnt - (__u8)m_DispAreaCnt;
						}
					}
				}

				// calculate display count
				m_DispAbleCnt	=	m_DispItemCnt - m_DispPos;
				if ((__s32)m_DispAbleCnt >= m_DispAreaCnt)
				{
					m_DispAbleCnt	=	(__u8)m_DispAreaCnt;
				}

				// check display item
				if (0 != m_DispAbleCnt)
				{
					m_PntRcItem.left		=	m_PntRcCont.left;
					m_PntRcItem.top			=	m_PntRcCont.top;
					m_PntRcItem.right		=	m_PntRcCont.right;
					m_PntRcItem.bottom	=	m_PntRcItem.top + (__s32)m_ParaTreeBase.ItemHeight;
					
					for (Tv_DispIdx=0; Tv_DispIdx < m_DispAbleCnt;Tv_DispIdx++)
					{
						m_PntItemIdx				=	m_DispTbl[m_DispPos + Tv_DispIdx]->ItmIdx;
						// paint item
						LcPntItem();
					}

				}

			}

		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeBase::LcPntValue(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeBase::LcPntItem(void)
{
	// local -------------------
		St_GrRect	Tv_RcIcon;
		__u8	Tv_IconIdx;
	// code --------------------
		// draw item
		// draw icon
		// calculate icon rect
		Tv_RcIcon.left		=	m_PntRcItem.left + m_ParaTreeBase.ItemGapLeft;
		Tv_RcIcon.top			=	m_PntRcItem.top;
		Tv_RcIcon.right		=	Tv_RcIcon.left + m_ParaTreeBase.IconWidth;
		Tv_RcIcon.bottom	=	m_PntRcItem.bottom;
		if (0 != m_ParaTreeBase.IconWidth)
		{
			if (Tv_RcIcon.right >= m_PntRcItem.right)
			{
				Tv_RcIcon.right	=	m_PntRcItem.right;
			}
			if ((Tv_RcIcon.left < Tv_RcIcon.right) && (Tv_RcIcon.top < Tv_RcIcon.bottom))
			{
				// draw icon
				Tv_IconIdx	=	m_TreeBaseItemTbl[m_PntItemIdx].IconIdx;
				PaintScriptPush(m_ParaTreeBase.PntIcon[Tv_IconIdx], &Tv_RcIcon);
			}
		}

		// draw value
		m_PntRcValue.left		=	Tv_RcIcon.right + (__s32)m_ParaTreeBase.IconGap;
		m_PntRcValue.top		=	m_PntRcItem.top;
		m_PntRcValue.right	=	m_PntRcItem.right - m_ParaTreeBase.ItemGapRight;
		m_PntRcValue.bottom	=	m_PntRcItem.bottom;
		if ((m_PntRcValue.left < m_PntRcValue.right) && (m_PntRcValue.top < m_PntRcValue.bottom))
		{
			LcPntValue();
		}

		// draw item background
		PaintScriptPush(m_ParaTreeBase.PntItemBg, &m_PntRcItem);
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeBase::ProcUiMsg(Ptr_GrP4MsgHd A_PtrMsg)
{
	// local -------------------
	// code --------------------
		if (E_GrP4MsgCmdSbarValChg == A_PtrMsg->Cmd)
		{
			LcProcEvtSbarValChg(((Ptr_GrP4MsgObj)A_PtrMsg)->ObjId);
		}
		else
		{
			Cls_GrP4DoBase::ProcUiMsg(A_PtrMsg);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeBase::LcProcEvtSbarValChg(__u16 A_ObjId)
{
	// local -------------------
		Cls_GrP4DoSbarBase*	Tv_ObjSbar;
	// code --------------------
		// check exist object
		Tv_ObjSbar	=	(Cls_GrP4DoSbarBase*)m_ObjWin->FindSlaveObjByScptId(A_ObjId);
		if (NULL != Tv_ObjSbar)
		{
			m_DispPos	=	(__u8)Tv_ObjSbar->ValueGet();
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoTreeBase::LcItemIconSet(__u32 A_Id, __u8 A_IconIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s16	Tv_ItmIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrP4DoTreeBaseIconCnt > A_IconIdx)
		{
			Tv_ItmIdx	=	LcItemFindById(A_Id);
			if (0 <= Tv_ItmIdx)
			{
				m_TreeBaseItemTbl[Tv_ItmIdx].IconIdx	=	A_IconIdx;
				ReqDraw();
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoTreeBase::ProcEvtMseLbtnUp(__u16 A_X, __u16 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ItmIdx;
		__u16	Tv_DispIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		// check item area
		if ((0 != m_DispAbleCnt) && (0 != m_ParaTreeBase.ItemHeight))
		{
			// check click item index
			if (m_ParaTreeBase.GapTop <= A_Y)
			{
				Tv_DispIdx	=	(A_Y - m_ParaTreeBase.GapTop) / m_ParaTreeBase.ItemHeight;
				if (Tv_DispIdx < (__u16)m_DispAbleCnt)
				{
					Tv_DispIdx	=	Tv_DispIdx + (__u16)m_DispPos;
					// get item index
					Tv_ItmIdx		=	m_DispTbl[Tv_DispIdx]->ItmIdx;
					// change expand
					m_TreeBaseItemTbl[Tv_ItmIdx].IsChildExp	=	!m_TreeBaseItemTbl[Tv_ItmIdx].IsChildExp;
					// rebuild display table
					m_IsDispBldReq	=	TRUE;
					ReqDraw();
					// success
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
