/*
		platform label

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoStatic.h>
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

#define E_GrP3DoStaticParaCnt		2

St_GrP3DvlDoPrInfoData	V_GrP3DoStaticParaInfo[E_GrP3DoStaticParaCnt]	=	
{
	{	L"Paint BG",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoStaticPara)0)->PntBg,NULL,	L"Paint background code.\n0 = None, 1 = basic code"	},
	{	L"TxtId",					TRUE,		E_GrP3DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP3DoStaticPara)0)->TxtId,NULL,	L"Display text script id"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoStatic::Cls_GrP3DoStatic( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassStatic;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoStatic) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaDoStatic, sizeof(m_ParaDoStatic) );
		m_ParaDoStatic.PntBg	=	E_GrP3ScptPstIdDflt;

}
//--------------------------------------------------------------------
Cls_GrP3DoStatic::~Cls_GrP3DoStatic( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoStatic::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassStatic;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoStatic);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoStatic, sizeof(m_ParaDoStatic) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoStatic)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoStatic::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoStatic, sizeof(m_ParaDoStatic) , m_PtrScpt , E_GrP3DoClassStatic );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoStatic::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch (A_Cmd)
		{
			case E_GrP3UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaDoStatic.TxtId;
				break;
			case E_GrP3UpiTextIdSet:
				m_ParaDoStatic.TxtId	=	(__u16)A_PtrInt[1];
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoStatic::ProcPaint( void )
{
		PaintScriptPush( m_ParaDoStatic.PntBg, &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoStatic::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoStatic, V_GrP3DoStaticParaInfo, E_GrP3DoStaticParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoStatic::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaDoStatic.TxtId );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
