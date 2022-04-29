/*
		platform spin number setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoChkbSgb.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>

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

#define E_GrP4DoChkbSgbParaCnt		3

St_GrP4DvlDoPrInfoData	V_GrP4DoChkbSgbParaInfo[E_GrP4DoChkbSgbParaCnt]	=	
{
	{	L"TxtIdFalse",	TRUE,		E_GrP4DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP4DoChkbSgbPara)0)->TxtIdFalse,NULL,	L"text id false"	},
	{	L"TxtIdTrue",		TRUE,		E_GrP4DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP4DoChkbSgbPara)0)->TxtIdTrue,NULL,	L"text id true"	},
	{	L"ValIdx",			TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,				1,	(void*)&((Ptr_GrP4DoChkbSgbPara)0)->ValIdx,NULL,	L"GVU value address"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoChkbSgb::Cls_GrP4DoChkbSgb(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoChkbBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassChkbSgb;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaChkbSgb) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaChkbSgb, sizeof(m_ParaChkbSgb) );

}
//--------------------------------------------------------------------
Cls_GrP4DoChkbSgb::~Cls_GrP4DoChkbSgb( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoChkbSgb::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoChkbBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassChkbSgb;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaChkbSgb);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaChkbSgb, sizeof(m_ParaChkbSgb) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaChkbSgb)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbSgb::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoChkbBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaChkbSgb, sizeof(m_ParaChkbSgb) , m_PtrScpt , E_GrP4DoClassChkbSgb );

		LcValLoad();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoChkbSgb::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaChkbSgb.TxtIdFalse;
				break;
			case E_GrP4UpiTextIdSet:
				m_ParaChkbSgb.TxtIdTrue		=	(__u16)A_PtrInt[1];
				m_ParaChkbSgb.TxtIdFalse	=	(__u16)A_PtrInt[1];
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoChkbBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoChkbSgb::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoChkbBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaChkbSgb, V_GrP4DoChkbSgbParaInfo, E_GrP4DoChkbSgbParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbSgb::LcValLoad( void )
{
	// local -------------------
		__s32		Tv_Val;
	// code --------------------
		Tv_Val	=	V_GrP4Mng->SysValueGet(E_GrP4AsmArgTypeGvu, (__u32)m_ParaChkbSgb.ValIdx);
		if ( 0 == Tv_Val )
		{
			m_IsVal	=	FALSE;
		}
		else
		{
			m_IsVal	=	TRUE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbSgb::LcValSave( void )
{
		V_GrP4Mng->SysValueSet( E_GrP4AsmArgTypeGvu, (__u32)m_ParaChkbSgb.ValIdx, (__s32)m_IsVal );
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoChkbSgb::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			if ( m_IsVal )
			{
				Tv_Result	=	Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaChkbSgb.TxtIdTrue );
			}
			else
			{
				Tv_Result	=	Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaChkbSgb.TxtIdFalse );
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
