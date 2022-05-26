/*
		platform label

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoLbl.h>
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

#define E_GrP3DoLblParaCnt		1

St_GrP3DvlDoPrInfoData	V_GrP3DoLblParaInfo[E_GrP3DoLblParaCnt]	=	
{
	{	L"Paint BG",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLblPara)0)->PntBg,NULL,	L"Paint background code.\n0 = None, 1 = basic code"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoLbl::Cls_GrP3DoLbl( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassLabel;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoLbl) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaDoLbl, sizeof(m_ParaDoLbl) );
		m_ParaDoLbl.PntBg	=	E_GrP3ScptPstIdDflt;

		GrStrClear(m_StrTxt);

}
//--------------------------------------------------------------------
Cls_GrP3DoLbl::~Cls_GrP3DoLbl( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbl::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassLabel;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoLbl);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoLbl, sizeof(m_ParaDoLbl) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoLbl)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbl::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoLbl, sizeof(m_ParaDoLbl) , m_PtrScpt , E_GrP3DoClassLabel );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoLbl::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Idx;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiTextGet:
				Tv_Idx		=	(__u8)A_PtrInt[1];
				if ( E_GrP3AsmMaxTxtRegCnt > Tv_Idx )
				{
					GrStrWcopy( A_PtrStr[Tv_Idx], m_StrTxt );
				}
				break;
			case E_GrP3UpiTextSet:
				Tv_Idx		=	(__u8)A_PtrInt[1];
				if ( E_GrP3AsmMaxTxtRegCnt > Tv_Idx )
				{
					GrStrWcopy( m_StrTxt, A_PtrStr[Tv_Idx] );
				}
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbl::ProcPaint( void )
{
		PaintScriptPush( m_ParaDoLbl.PntBg, &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbl::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoLbl, V_GrP3DoLblParaInfo, E_GrP3DoLblParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoLbl::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	m_StrTxt;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
