/*
		platform relate static

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoRelStc.h>
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

#define E_GrP4DoRelStcParaCnt		3

St_GrP4DvlDoPrInfoData	V_GrP4DoRelStcParaInfo[E_GrP4DoRelStcParaCnt]	=	
{
	{	L"Paint Normal",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoRelStcPara)0)->PntStat[E_GrP4DoRelStcStatNormal],NULL,	L"Paint normal status"	},
	{	L"Paint Active",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoRelStcPara)0)->PntStat[E_GrP4DoRelStcStatActive],NULL,	L"Paint active status"	},
	{	L"TxtId",							TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoRelStcPara)0)->TxtId,NULL,	L"Display text script id"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoRelStc::Cls_GrP4DoRelStc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassRelStc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoRelStc) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaDoRelStc, sizeof(m_ParaDoRelStc) );

		m_Stat			=	E_GrP4DoRelStcStatNormal;

}
//--------------------------------------------------------------------
Cls_GrP4DoRelStc::~Cls_GrP4DoRelStc( void )
{

}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoRelStc::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassRelStc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoRelStc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoRelStc, sizeof(m_ParaDoRelStc) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoRelStc)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoRelStc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoRelStc, sizeof(m_ParaDoRelStc) , m_PtrScpt , E_GrP4DoClassRelStc );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoRelStc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch (A_Cmd)
		{
			case E_GrP4UpiTextIdSet:
				m_ParaDoRelStc.TxtId		=	(__u16)A_PtrInt[1];
				break;
			case E_GrP4UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaDoRelStc.TxtId;
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand(A_Cmd, A_PtrInt, A_PtrStr);
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoRelStc::ProcPaint( void )
{
		PaintScriptPush( m_ParaDoRelStc.PntStat[m_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoRelStc::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoRelStc, V_GrP4DoRelStcParaInfo, E_GrP4DoRelStcParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoRelStc::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaDoRelStc.TxtId );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoRelStc::ActiveSet( BOOL8 A_IsActive )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		Tv_Stat	=	E_GrP4DoRelStcStatNormal;
		if ( A_IsActive )
		{
			Tv_Stat	=	E_GrP4DoRelStcStatActive;
		}
		if ( Tv_Stat != m_Stat )
		{
			m_Stat	=	Tv_Stat;
			ReqDraw();
		}
		
}
//--------------------------------------------------------------------
