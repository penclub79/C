/*
		platform radio button setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoRbtnGvu.h>
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

#define E_GrP3DoRbtnGvuParaCnt		1

St_GrP3DvlDoPrInfoData	V_GrP3DoRbtnGvuParaInfo[E_GrP3DoRbtnGvuParaCnt]	=	
{
	{	L"ValIdx",			TRUE,		E_GrP3DvlParaTypeAsmGvuAdr,	1,	(void*)&((Ptr_GrP3DoRbtnGvuPara)0)->ValIdx,NULL,	L"GVU value index"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoRbtnGvu::Cls_GrP3DoRbtnGvu( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoRbtnBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassRbtnGvu;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaRbtnGvu) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaRbtnGvu, sizeof(m_ParaRbtnGvu) );

		m_PtrGvu	=	(__s32*)V_GrP3Mng->GvuTblGet();

}
//--------------------------------------------------------------------
Cls_GrP3DoRbtnGvu::~Cls_GrP3DoRbtnGvu( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnGvu::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoRbtnBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassRbtnGvu;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaRbtnGvu);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaRbtnGvu, sizeof(m_ParaRbtnGvu) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaRbtnGvu)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnGvu::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoRbtnBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaRbtnGvu, sizeof(m_ParaRbtnGvu) , m_PtrScpt , E_GrP3DoClassRbtnGvu );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoRbtnGvu::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoRbtnBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnGvu::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoRbtnBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaRbtnGvu, V_GrP3DoRbtnGvuParaInfo, E_GrP3DoRbtnGvuParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnGvu::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		BOOL8	Tv_IsMatch;
	// code --------------------
		Tv_IsMatch	=	FALSE;
		if ( E_GrP3MngGvuMaxCnt > m_ParaRbtnGvu.ValIdx )
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
void	Cls_GrP3DoRbtnGvu::LcDoSelect( void )
{
	// local -------------------
	// code --------------------
		if ( E_GrP3MngGvuMaxCnt > m_ParaRbtnGvu.ValIdx )
		{
			m_PtrGvu[m_ParaRbtnGvu.ValIdx]	=	m_ParaRbtnBase.Value;
		}
		
		// inherited
		Cls_GrP3DoRbtnBase::LcDoSelect();

}
//--------------------------------------------------------------------

