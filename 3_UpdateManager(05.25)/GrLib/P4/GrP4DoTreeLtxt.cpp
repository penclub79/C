/*
		platform tree base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoTreeLtxt.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>
#include <P4/GrP4DoTreeLtxt.h>

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

#define E_GrP4DoTreeLtxtParaCnt		1

St_GrP4DvlDoPrInfoData	V_GrP4DoTreeLtxtParaInfo[E_GrP4DoTreeLtxtParaCnt]	=	
{
	{	L"PntValue",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoTreeLtxtPara)0)->PntValue,NULL,	L"Paint value code.\n0 = None"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoTreeLtxt::Cls_GrP4DoTreeLtxt(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoTreeBase(A_ScptId, A_PtrRect, A_PosRel, A_ObjPrnt)
{
	// local -------------------
		__u8	Tv_ItmIdx;
	// code --------------------
		//init
		m_ClassId				=	E_GrP4DoClassTreeLtxt;
		m_ParaSize			=	m_ParaSize + sizeof(m_ParaTreeLtxt) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaTreeLtxt, sizeof(m_ParaTreeLtxt) );
		m_ParaTreeLtxt.PntValue	=	E_GrP4ScptPstIdDflt;

		GrDumyZeroMem(m_TreeLtxtItemTbl, sizeof(m_TreeLtxtItemTbl));
		for (Tv_ItmIdx=0; Tv_ItmIdx < E_GrP4DoTreeBaseItemMaxCnt;Tv_ItmIdx++)
		{
			m_TreeBaseItemTbl[Tv_ItmIdx].PtrRel	=	&m_TreeLtxtItemTbl[Tv_ItmIdx];
		}
}
//--------------------------------------------------------------------
Cls_GrP4DoTreeLtxt::~Cls_GrP4DoTreeLtxt( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoTreeLtxt::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassTreeLtxt;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaTreeLtxt);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaTreeLtxt, sizeof(m_ParaTreeLtxt) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaTreeLtxt)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeLtxt::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaTreeLtxt, sizeof(m_ParaTreeLtxt) , m_PtrScpt , E_GrP4DoClassTreeLtxt );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoTreeLtxt::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch (A_Cmd)
		{
			case E_GrP4UpiItemTextSet:
				LcProcUpiItemTextSet((__u32)A_PtrInt[1], (__u32)A_PtrInt[2], A_PtrStr[0]);
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoTreeLtxt::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaTreeLtxt, V_GrP4DoTreeLtxtParaInfo, E_GrP4DoTreeLtxtParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeLtxt::LcPntValue(void)
{
	// local -------------------
	// code --------------------
		if (E_GrP4ScptPstIdNone != m_ParaTreeLtxt.PntValue)
		{
			PaintScriptPush(m_ParaTreeLtxt.PntValue, &m_PntRcValue);
		}

}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoTreeLtxt::RtlInTextGet(__u8 A_Idx, __s32* A_PtrRtCus)
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if (0 == A_Idx)
		{
			Tv_Result	=	m_TreeLtxtItemTbl[m_PntItemIdx].StrVal;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoTreeLtxt::LcProcUpiItemTextSet(__u32 A_ItemId, __u32 A_SlotIdx, WCHAR* A_StrVal)
{
	// local -------------------
		__s16	Tv_ItmIdx;
	// code --------------------
		Tv_ItmIdx	=	LcItemFindById(A_ItemId);
		if (0 <= Tv_ItmIdx)
		{
			GrStrWcopy(m_TreeLtxtItemTbl[Tv_ItmIdx].StrVal, A_StrVal, GrStrMaxLen(m_TreeLtxtItemTbl[Tv_ItmIdx].StrVal));
		}
}
//--------------------------------------------------------------------

