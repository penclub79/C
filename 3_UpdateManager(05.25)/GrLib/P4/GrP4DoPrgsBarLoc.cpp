/*
		platform prograss bar local

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoPrgsBarLoc.h>
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

/*
#define E_GrP4DoPrgsBarLocParaCnt		1

St_GrP4DvlDoPrInfoData	V_GrP4DoPrgsBarLocParaInfo[E_GrP4DoPrgsBarLocParaCnt]	=	
{
	{	L"EvtHostId",			TRUE,		E_GrP4DvlParaTypeScptIdAny,				(void*)&((Ptr_GrP4DoPrgsBarLocPara)0)->EvtHostId,NULL,	L"event host object id"	},
};
*/

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoPrgsBarLoc::Cls_GrP4DoPrgsBarLoc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoPrgsBarBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassPrgsBarLoc;
		//m_ParaSize	=	m_ParaSize + sizeof(m_ParaPrgsBarLoc) + sizeof(St_GrP4ScptParaHd);
		
		//GrDumyZeroMem( &m_ParaPrgsBarLoc, sizeof(m_ParaPrgsBarLoc) );

}
//--------------------------------------------------------------------
Cls_GrP4DoPrgsBarLoc::~Cls_GrP4DoPrgsBarLoc( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoPrgsBarLoc::RtlScptParaSubDump(void)
{
	// local -------------------
		// Ptr_GrP4ScptParaHd	Tv_PtrHd;
		// void*	Tv_PtrCont;
	// code --------------------
		// inherit
		Cls_GrP4DoPrgsBarBase::RtlScptParaSubDump();
		/*
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassPrgsBarLoc;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaPrgsBarLoc);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaPrgsBarLoc, sizeof(m_ParaPrgsBarLoc) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaPrgsBarLoc)	+ sizeof(St_GrP4ScptParaHd);
		*/
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoPrgsBarLoc::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoPrgsBarBase::RtlScptParaSubUpdt();
		//ParaCopyFromScpt( &m_ParaPrgsBarLoc, sizeof(m_ParaPrgsBarLoc) , m_PtrScpt , E_GrP4DoClassPrgsBarLoc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoPrgsBarLoc::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoPrgsBarBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoPrgsBarLoc::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoPrgsBarBase::RtlDvlParaInfoSubDump();
		// dump
		//LcDvlParaInfoSaveAdd( &m_ParaPrgsBarLoc, V_GrP4DoPrgsBarLocParaInfo, E_GrP4DoPrgsBarLocParaCnt );
}
#endif
//--------------------------------------------------------------------

