/*
		platform label

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoLbl.h>
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

#define E_GrP4DoLblParaCnt		1

St_GrP4DvlDoPrInfoData	V_GrP4DoLblParaInfo[E_GrP4DoLblParaCnt]	=	
{
	{	L"Paint BG",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoLblPara)0)->PntBg,NULL,	L"Paint background code.\n0 = None, 1 = basic code"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoLbl::Cls_GrP4DoLbl(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassLabel;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoLbl) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaDoLbl, sizeof(m_ParaDoLbl) );
		m_ParaDoLbl.PntBg	=	E_GrP4ScptPstIdDflt;

		GrStrClear(m_StrTxt);

}
//--------------------------------------------------------------------
Cls_GrP4DoLbl::~Cls_GrP4DoLbl( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoLbl::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassLabel;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoLbl);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoLbl, sizeof(m_ParaDoLbl) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoLbl)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoLbl::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoLbl, sizeof(m_ParaDoLbl) , m_PtrScpt , E_GrP4DoClassLabel );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoLbl::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Idx;
		WCHAR*	Tv_StrWk;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiTextGet:
				Tv_Idx		=	(__u8)A_PtrInt[1];
				if ( E_GrP4AsmMaxTxtRegCnt > Tv_Idx )
				{
					GrStrWcopy( A_PtrStr[Tv_Idx], m_StrTxt );
				}
				break;
			case E_GrP4UpiTextSet:
				Tv_Idx		=	(__u8)A_PtrInt[1];
				if ( E_GrP4AsmMaxTxtRegCnt > Tv_Idx )
				{
					GrStrWcopy( m_StrTxt, A_PtrStr[Tv_Idx] );
				}
				ReqDraw();
				break;
			case E_GrP4UpiTextIdSet:
				Tv_StrWk	=	V_GrP4Mng->TextPtrGet((__u16)A_PtrInt[1]);
				if (NULL != Tv_StrWk)
				{
					GrStrWcopy(m_StrTxt, Tv_StrWk, E_GrP4DoLblStrMaxLen - 1);
				}
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoLbl::ProcPaint( void )
{
		PaintScriptPush( m_ParaDoLbl.PntBg, &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoLbl::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoLbl, V_GrP4DoLblParaInfo, E_GrP4DoLblParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoLbl::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
