/*
		platform prograss bar local

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoPrgsBarLoc.h>
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

/*
#define E_GrP3DoPrgsBarLocParaCnt		1

St_GrP3DvlDoPrInfoData	V_GrP3DoPrgsBarLocParaInfo[E_GrP3DoPrgsBarLocParaCnt]	=	
{
	{	L"EvtHostId",			TRUE,		E_GrP3DvlParaTypeScptIdAny,				(void*)&((Ptr_GrP3DoPrgsBarLocPara)0)->EvtHostId,NULL,	L"event host object id"	},
};
*/

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoPrgsBarLoc::Cls_GrP3DoPrgsBarLoc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoPrgsBarBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassPrgsBarLoc;
		//m_ParaSize	=	m_ParaSize + sizeof(m_ParaPrgsBarLoc) + sizeof(St_GrP3ScptParaHd);
		
		//GrDumyZeroMem( &m_ParaPrgsBarLoc, sizeof(m_ParaPrgsBarLoc) );

}
//--------------------------------------------------------------------
Cls_GrP3DoPrgsBarLoc::~Cls_GrP3DoPrgsBarLoc( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPrgsBarLoc::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		// Ptr_GrP3ScptParaHd	Tv_PtrHd;
		// void*	Tv_PtrCont;
	// code --------------------
		// inherit
		Cls_GrP3DoPrgsBarBase::RtlScptParaSubDump();
		/*
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassPrgsBarLoc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaPrgsBarLoc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaPrgsBarLoc, sizeof(m_ParaPrgsBarLoc) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaPrgsBarLoc)	+ sizeof(St_GrP3ScptParaHd);
		*/
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPrgsBarLoc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoPrgsBarBase::RtlScptParaSubUpdt();
		//ParaCopyFromScpt( &m_ParaPrgsBarLoc, sizeof(m_ParaPrgsBarLoc) , m_PtrScpt , E_GrP3DoClassPrgsBarLoc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoPrgsBarLoc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoPrgsBarBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPrgsBarLoc::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoPrgsBarBase::RtlDvlParaInfoSubDump();
		// dump
		//LcDvlParaInfoSaveAdd( &m_ParaPrgsBarLoc, V_GrP3DoPrgsBarLocParaInfo, E_GrP3DoPrgsBarLocParaCnt );
#endif
}
//--------------------------------------------------------------------

