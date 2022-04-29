/*
		platform relate static

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoRelStc.h>
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

#define E_GrP3DoRelStcParaCnt		3

St_GrP3DvlDoPrInfoData	V_GrP3DoRelStcParaInfo[E_GrP3DoRelStcParaCnt]	=	
{
	{	L"Paint Normal",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoRelStcPara)0)->PntStat[E_GrP3DoRelStcStatNormal],NULL,	L"Paint normal status"	},
	{	L"Paint Active",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoRelStcPara)0)->PntStat[E_GrP3DoRelStcStatActive],NULL,	L"Paint active status"	},
	{	L"TxtId",							TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoRelStcPara)0)->TxtId,NULL,	L"Display text script id"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoRelStc::Cls_GrP3DoRelStc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassRelStc;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoRelStc) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaDoRelStc, sizeof(m_ParaDoRelStc) );

		m_Stat			=	E_GrP3DoRelStcStatNormal;

}
//--------------------------------------------------------------------
Cls_GrP3DoRelStc::~Cls_GrP3DoRelStc( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoRelStc::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassRelStc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoRelStc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoRelStc, sizeof(m_ParaDoRelStc) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoRelStc)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRelStc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoRelStc, sizeof(m_ParaDoRelStc) , m_PtrScpt , E_GrP3DoClassRelStc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoRelStc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRelStc::ProcPaint( void )
{
		PaintScriptPush( m_ParaDoRelStc.PntStat[m_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRelStc::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoRelStc, V_GrP3DoRelStcParaInfo, E_GrP3DoRelStcParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoRelStc::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaDoRelStc.TxtId );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRelStc::ActiveSet( BOOL8 A_IsActive )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		Tv_Stat	=	E_GrP3DoRelStcStatNormal;
		if ( A_IsActive )
		{
			Tv_Stat	=	E_GrP3DoRelStcStatActive;
		}
		if ( Tv_Stat != m_Stat )
		{
			m_Stat	=	Tv_Stat;
			ReqDraw();
		}
		
}
//--------------------------------------------------------------------
