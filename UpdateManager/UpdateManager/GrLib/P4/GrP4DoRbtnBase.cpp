/*
		platform check radio button static setup bool8

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoRbtnBase.h>
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

#define E_GrP4DoRbtnBaseParaCnt		12

St_GrP4DvlDoPrInfoData	V_GrP4DoRbtnBaseParaInfo[E_GrP4DoRbtnBaseParaCnt]	=	
{
	{	L"Paint NrmDis",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoRbtnBasePara)0)->PntStat[E_GrP4DoRbtnBaseStatNrmDis],  NULL,	L"Paint normal disable status"	},
	{	L"Paint Nrm",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoRbtnBasePara)0)->PntStat[E_GrP4DoRbtnBaseStatNrm],	 NULL,L"Paint normal status"	},
	{	L"Paint NrmFcs",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoRbtnBasePara)0)->PntStat[E_GrP4DoRbtnBaseStatNrmFcs],	 NULL,L"Paint normal focus status"	},
	{	L"Paint NrmPush",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoRbtnBasePara)0)->PntStat[E_GrP4DoRbtnBaseStatNrmPush],	 NULL,L"Paint normal push status"	},
	{	L"Paint SelDis",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoRbtnBasePara)0)->PntStat[E_GrP4DoRbtnBaseStatSelDis],	 NULL,L"Paint select disable status"	},
	{	L"Paint Sel",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoRbtnBasePara)0)->PntStat[E_GrP4DoRbtnBaseStatSel],	 NULL,L"Paint true select status"	},
	{	L"Paint SelFcs",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoRbtnBasePara)0)->PntStat[E_GrP4DoRbtnBaseStatSelFcs],	 NULL,L"Paint select focus status"	},
	{	L"Paint SelPush",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoRbtnBasePara)0)->PntStat[E_GrP4DoRbtnBaseStatSelPush],	 NULL,L"Paint select push status"	},
	{	L"AsmPush",					TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoRbtnBasePara)0)->AsmPush,	 NULL,L"assembly value change event code"	},
	{	L"TxtIdNrm",				TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoRbtnBasePara)0)->TxtIdNrm,	 NULL,L"normal status text id"	},
	{	L"TxtIdSel",				TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoRbtnBasePara)0)->TxtIdSel,	 NULL,L"select status text id"	},
	{	L"Value",						TRUE,		E_GrP4DvlParaTypeInt,						1,	(void*)&((Ptr_GrP4DoRbtnBasePara)0)->Value,	 NULL,L"select value"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoRbtnBase::Cls_GrP4DoRbtnBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassRbtnBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaRbtnBase) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		m_IsPush		=	FALSE;
		m_IsVal			=	FALSE;

		GrDumyZeroMem( &m_ParaRbtnBase, sizeof(m_ParaRbtnBase) );
		//m_ParaRbtnBase.PntBg	=	E_GrP4ScptPstIdDflt;

}
//--------------------------------------------------------------------
Cls_GrP4DoRbtnBase::~Cls_GrP4DoRbtnBase( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoRbtnBase::RtlScptParaSubDump( void )
{
#ifdef GR_P4_EDITOR
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassRbtnBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaRbtnBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaRbtnBase, sizeof(m_ParaRbtnBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaRbtnBase)	+ sizeof(St_GrP4ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP4DoRbtnBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaRbtnBase, sizeof(m_ParaRbtnBase) , m_PtrScpt , E_GrP4DoClassRbtnBase );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoRbtnBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoRbtnBase::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		Tv_Stat	=	E_GrP4DoRbtnBaseStatNrmDis;
		if ( m_IsVal )
		{
			Tv_Stat	=	E_GrP4DoRbtnBaseStatSelDis;
		}
		
		if ( IsCanEnable() )
		{
			Tv_Stat	++;		// normal
			if ( m_IsPush )
			{
				Tv_Stat	=	Tv_Stat + 2;		// push
			}
			else
			{
				if ( m_IsOnMouse || IsFocused() )
				{
					Tv_Stat	++;	// focus
				}
			}
		}
		
		PaintScriptPush( m_ParaRbtnBase.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP4DoRbtnBase::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P4_EDITOR
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaRbtnBase, V_GrP4DoRbtnBaseParaInfo, E_GrP4DoRbtnBaseParaCnt );
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoRbtnBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4KeyEnter == A_Key )
		{
			m_IsPush	=	TRUE;
			ReqDraw();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoRbtnBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4KeyEnter == A_Key )
		{
			m_IsPush	=	FALSE;
			LcDoSelect();
			ReqDraw();
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoRbtnBase::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	TRUE;
		Cls_GrP4DoBase::ProcEvtMseLbtnDown( A_X, A_Y, A_BtnMap );
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoRbtnBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP4DoBase::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		LcDoSelect();
		ReqDraw();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoRbtnBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsPush	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoRbtnBase::LcDoSelect( void )
{
	// local -------------------
	// code --------------------
		m_IsVal		=	TRUE;		// matching
		// do up assembly
		V_GrP4Mng->VmRunCode( m_ParaRbtnBase.AsmPush, this );

}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoRbtnBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			if ( m_IsVal )
			{
				Tv_Result	=	Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaRbtnBase.TxtIdSel );
			}
			else
			{
				Tv_Result	=	Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaRbtnBase.TxtIdNrm );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

