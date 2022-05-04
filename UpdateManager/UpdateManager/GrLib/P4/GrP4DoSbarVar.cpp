/*
		platform slider bar intra variable

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSbarVar.h>
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
#define E_GrP4DoSbarVarParaCnt		24

St_GrP4DvlDoPrInfoData	V_GrP4DoSbarVarParaInfo[E_GrP4DoSbarVarParaCnt]	=	
{
	//{	L"AsmValChg",				TRUE,		E_GrP4DvlParaTypeAsmCode,				(void*)&((Ptr_GrP4DoSbarVarPara)0)->AsmValChg,NULL,			L"assembly code for value change event"	},

};
*/

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSbarVar::Cls_GrP4DoSbarVar(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoSbarBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSbarVar;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSbarVar) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSbarVar, sizeof(m_ParaSbarVar) );
}
//--------------------------------------------------------------------
Cls_GrP4DoSbarVar::~Cls_GrP4DoSbarVar( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSbarVar::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoSbarBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSbarVar;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSbarVar);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSbarVar, sizeof(m_ParaSbarVar) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSbarVar)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarVar::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoSbarBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSbarVar, sizeof(m_ParaSbarVar) , m_PtrScpt , E_GrP4DoClassSbarVar );


}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSbarVar::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoSbarBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSbarVar::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoSbarBase::RtlDvlParaInfoSubDump();
		// dump
		//LcDvlParaInfoSaveAdd( &m_ParaSbarVar, V_GrP4DoSbarVarParaInfo, E_GrP4DoSbarVarParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoSbarVar::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;

		if ( 0 == A_Idx )
		{
			Tv_Result	=	m_StrVal;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
