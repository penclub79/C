/*
		platform slider bar intra variable

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSbarVar.h>
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
#define E_GrP3DoSbarVarParaCnt		24

St_GrP3DvlDoPrInfoData	V_GrP3DoSbarVarParaInfo[E_GrP3DoSbarVarParaCnt]	=	
{
	//{	L"AsmValChg",				TRUE,		E_GrP3DvlParaTypeAsmCode,				(void*)&((Ptr_GrP3DoSbarVarPara)0)->AsmValChg,NULL,			L"assembly code for value change event"	},

};
*/

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSbarVar::Cls_GrP3DoSbarVar( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoSbarBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassSbarVar;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSbarVar) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSbarVar, sizeof(m_ParaSbarVar) );
}
//--------------------------------------------------------------------
Cls_GrP3DoSbarVar::~Cls_GrP3DoSbarVar( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarVar::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoSbarBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSbarVar;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSbarVar);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSbarVar, sizeof(m_ParaSbarVar) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSbarVar)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarVar::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoSbarBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSbarVar, sizeof(m_ParaSbarVar) , m_PtrScpt , E_GrP3DoClassSbarVar );


}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSbarVar::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoSbarBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarVar::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoSbarBase::RtlDvlParaInfoSubDump();
		// dump
		//LcDvlParaInfoSaveAdd( &m_ParaSbarVar, V_GrP3DoSbarVarParaInfo, E_GrP3DoSbarVarParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoSbarVar::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
