/*
		platform check button static gvu bitmap

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoChkbSgm.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>

#ifdef GR_P3_EDITOR
#include	<P3/Dvl/GrP3DvlBase.h>
#include	<P3/Dvl/GrP3DvlMng.h>
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
extern	Cls_GrP3Mng*	V_GrP3Mng;

#ifdef GR_P3_EDITOR

#define E_GrP3DoChkbSgmParaCnt		4

St_GrP3DvlDoPrInfoData	V_GrP3DoChkbSgmParaInfo[E_GrP3DoChkbSgmParaCnt]	=	
{
	{	L"TxtIdFalse",	TRUE,		E_GrP3DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP3DoChkbSgmPara)0)->TxtIdFalse,NULL,	L"text id false"	},
	{	L"TxtIdTrue",		TRUE,		E_GrP3DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP3DoChkbSgmPara)0)->TxtIdTrue,NULL,	L"text id true"	},
	{	L"ValIdx",			TRUE,		E_GrP3DvlParaTypeAsmGvuAdr,				1,	(void*)&((Ptr_GrP3DoChkbSgmPara)0)->ValIdx,NULL,	L"GVU value address"	},
	{	L"BitPos",			TRUE,		E_GrP3DvlParaTypeByte,						1,	(void*)&((Ptr_GrP3DoChkbSgmPara)0)->BitPos,NULL,	L"value bit position"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoChkbSgm::Cls_GrP3DoChkbSgm( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoChkbBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassChkbSgm;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaChkbSgm) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaChkbSgm, sizeof(m_ParaChkbSgm) );

}
//--------------------------------------------------------------------
Cls_GrP3DoChkbSgm::~Cls_GrP3DoChkbSgm( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSgm::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoChkbBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassChkbSgm;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaChkbSgm);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaChkbSgm, sizeof(m_ParaChkbSgm) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaChkbSgm)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSgm::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoChkbBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaChkbSgm, sizeof(m_ParaChkbSgm) , m_PtrScpt , E_GrP3DoClassChkbSgm );

		LcValLoad();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoChkbSgm::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaChkbSgm.TxtIdFalse;
				break;
			case E_GrP3UpiTextIdSet:
				m_ParaChkbSgm.TxtIdTrue		=	(__u16)A_PtrInt[1];
				m_ParaChkbSgm.TxtIdFalse	=	(__u16)A_PtrInt[1];
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoChkbBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSgm::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoChkbBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaChkbSgm, V_GrP3DoChkbSgmParaInfo, E_GrP3DoChkbSgmParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSgm::LcValLoad( void )
{
	// local -------------------
		__u32	Tv_BitMask;
		__u32	Tv_Val;
	// code --------------------
		Tv_Val	=	(__u32)V_GrP3Mng->VmValueGet( E_GrP3AsmArgTypeAtbGvu, m_ParaChkbSgm.ValIdx );
		Tv_BitMask	=	1 << (__u32)m_ParaChkbSgm.BitPos;
		m_IsVal	=	FALSE;
		if ( 0 != ( Tv_BitMask & Tv_Val ) )
		{
			m_IsVal	=	TRUE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSgm::LcValSave( void )
{
	// local -------------------
		__u32	Tv_BitMask;
		__u32	Tv_Val;
	// code --------------------
		Tv_Val	=	(__u32)V_GrP3Mng->VmValueGet( E_GrP3AsmArgTypeAtbGvu, m_ParaChkbSgm.ValIdx );
		Tv_BitMask	=	1 << (__u32)m_ParaChkbSgm.BitPos;
		if ( m_IsVal )
		{
			Tv_Val	=	Tv_Val | Tv_BitMask;
		}
		else
		{
			Tv_Val	=	Tv_Val & ~Tv_BitMask;
		}
		V_GrP3Mng->VmValueSet( E_GrP3AsmArgTypeAtbGvu, m_ParaChkbSgm.ValIdx, (__s32)Tv_Val );
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoChkbSgm::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
				Tv_Result	=	Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaChkbSgm.TxtIdTrue );
			}
			else
			{
				Tv_Result	=	Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaChkbSgm.TxtIdFalse );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
