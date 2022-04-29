/*
		platform radio button setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoRbtnGvu.h>
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

#define E_GrP4DoRbtnGvuParaCnt		1

St_GrP4DvlDoPrInfoData	V_GrP4DoRbtnGvuParaInfo[E_GrP4DoRbtnGvuParaCnt]	=	
{
	{	L"ValIdx",			TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,	1,	(void*)&((Ptr_GrP4DoRbtnGvuPara)0)->ValIdx,NULL,	L"GVU value index"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoRbtnGvu::Cls_GrP4DoRbtnGvu(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoRbtnBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassRbtnGvu;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaRbtnGvu) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaRbtnGvu, sizeof(m_ParaRbtnGvu) );

		m_PtrGvu	=	(__s32*)V_GrP4Mng->GvuTblGet();

}
//--------------------------------------------------------------------
Cls_GrP4DoRbtnGvu::~Cls_GrP4DoRbtnGvu( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoRbtnGvu::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoRbtnBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassRbtnGvu;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaRbtnGvu);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaRbtnGvu, sizeof(m_ParaRbtnGvu) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaRbtnGvu)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoRbtnGvu::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoRbtnBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaRbtnGvu, sizeof(m_ParaRbtnGvu) , m_PtrScpt , E_GrP4DoClassRbtnGvu );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoRbtnGvu::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoRbtnBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoRbtnGvu::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoRbtnBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaRbtnGvu, V_GrP4DoRbtnGvuParaInfo, E_GrP4DoRbtnGvuParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoRbtnGvu::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		BOOL8	Tv_IsMatch;
	// code --------------------
		Tv_IsMatch	=	FALSE;
		if ( E_GrP4MngGvuMaxCnt > m_ParaRbtnGvu.ValIdx )
		{
			if ( m_ParaRbtnBase.Value == m_PtrGvu[m_ParaRbtnGvu.ValIdx] )
			{
				Tv_IsMatch	=	TRUE;
			}
		}
		// check need repaint
		if ( m_IsVal != Tv_IsMatch )
		{
			m_IsVal	=	Tv_IsMatch;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoRbtnGvu::LcDoSelect( void )
{
	// local -------------------
	// code --------------------
		if ( E_GrP4MngGvuMaxCnt > m_ParaRbtnGvu.ValIdx )
		{
			m_PtrGvu[m_ParaRbtnGvu.ValIdx]	=	m_ParaRbtnBase.Value;
		}
		
		// inherited
		Cls_GrP4DoRbtnBase::LcDoSelect();

}
//--------------------------------------------------------------------

