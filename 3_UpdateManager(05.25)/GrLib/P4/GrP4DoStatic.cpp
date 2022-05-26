/*
		platform label

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoStatic.h>
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

#define E_GrP4DoStaticParaCnt		2

St_GrP4DvlDoPrInfoData	V_GrP4DoStaticParaInfo[E_GrP4DoStaticParaCnt]	=	
{
	{	L"Paint BG",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoStaticPara)0)->PntBg,NULL,	L"Paint background code.\n0 = None, 1 = basic code"	},
	{	L"TxtId",					TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoStaticPara)0)->TxtId,NULL,	L"Display text script id"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoStatic::Cls_GrP4DoStatic(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassStatic;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoStatic) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaDoStatic, sizeof(m_ParaDoStatic) );
		m_ParaDoStatic.PntBg	=	E_GrP4ScptPstIdDflt;

}
//--------------------------------------------------------------------
Cls_GrP4DoStatic::~Cls_GrP4DoStatic( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoStatic::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassStatic;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoStatic);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoStatic, sizeof(m_ParaDoStatic) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoStatic)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoStatic::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoStatic, sizeof(m_ParaDoStatic) , m_PtrScpt , E_GrP4DoClassStatic );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoStatic::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch (A_Cmd)
		{
			case E_GrP4UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaDoStatic.TxtId;
				break;
			case E_GrP4UpiTextIdSet:
				m_ParaDoStatic.TxtId	=	(__u16)A_PtrInt[1];
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoStatic::ProcPaint( void )
{
		PaintScriptPush( m_ParaDoStatic.PntBg, &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoStatic::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoStatic, V_GrP4DoStaticParaInfo, E_GrP4DoStaticParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoStatic::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaDoStatic.TxtId );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
