/*
		platform label

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoPrgsBarBase.h>
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

#define E_GrP4DoPrgsBarBaseParaCnt		14

St_GrP4DvlDoPrInfoData	V_GrP4DoPrgsBarBaseParaInfo[E_GrP4DoPrgsBarBaseParaCnt]	=	
{
	{	L"GrpPosLeft",		TRUE,		E_GrP4DvlParaTypePosBssLt,			1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->GrpPosBss,NULL,	L"Graph position left"	},
	{	L"",							FALSE,	E_GrP4DvlParaTypeShort,					1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->GrpLeft,NULL,		L""	},
	{	L"GrpPosTop",			TRUE,		E_GrP4DvlParaTypePosBssUp,			1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->GrpPosBss,NULL,	L"Graph position top"	},
	{	L"",							FALSE,	E_GrP4DvlParaTypeShort,					1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->GrpTop,NULL,			L""	},
	{	L"GrpPosRight",		TRUE,		E_GrP4DvlParaTypePosBssRt,			1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->GrpPosBss,NULL,	L"Graph position right"	},
	{	L"",							FALSE,	E_GrP4DvlParaTypeShort,					1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->GrpRight,NULL,		L""	},
	{	L"GrpPosBottom",	TRUE,		E_GrP4DvlParaTypePosBssDn,			1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->GrpPosBss,NULL,	L"Graph position bottom"	},
	{	L"",							FALSE,	E_GrP4DvlParaTypeShort,					1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->GrpBottom,NULL,	L""	},
	{	L"IsVertical",		TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->IsVertical,NULL,	L"Vertical / horizontal type"	},
	{	L"IsReverse",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->IsReverse,NULL,	L"reverse graph direction"	},
	{	L"Paint BG",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->PntBg,NULL,			L"Paint background code.\n0 = None, 1 = basic code"	},
	{	L"Paint Graph",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->PntGrp,NULL,			L"Paint graph code.\n0 = None, 1 = basic code"	},
	{	L"Value Min",			TRUE,		E_GrP4DvlParaTypeInt,						1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->ValMin,NULL,			L"Value range minimal"	},
	{	L"Value Max",			TRUE,		E_GrP4DvlParaTypeInt,						1,	(void*)&((Ptr_GrP4DoPrgsBarBasePara)0)->ValMax,NULL,			L"Value range maximal"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoPrgsBarBase::Cls_GrP4DoPrgsBarBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt ):
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassPrgsBarBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoPrgsBarBase) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaDoPrgsBarBase, sizeof(m_ParaDoPrgsBarBase) );
		m_ParaDoPrgsBarBase.PntBg		=	E_GrP4ScptPstIdDflt;
		m_ParaDoPrgsBarBase.ValMax	=	100;

#ifdef GR_P4_EDITOR
		m_Value			=	50;
#else
		m_Value			=	0;
#endif
		GrStrClear( m_StrPrgs );
}
//--------------------------------------------------------------------
Cls_GrP4DoPrgsBarBase::~Cls_GrP4DoPrgsBarBase( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPrgsBarBase::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassPrgsBarBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoPrgsBarBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoPrgsBarBase, sizeof(m_ParaDoPrgsBarBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoPrgsBarBase)	+ sizeof(St_GrP4ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPrgsBarBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoPrgsBarBase, sizeof(m_ParaDoPrgsBarBase) , m_PtrScpt , E_GrP4DoClassPrgsBarBase );

		LcPrgsStrUpdt();		// progress string update
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoPrgsBarBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiValueGet:
				LcValLoad();
				A_PtrInt[0]	=	m_Value;
				break;
			case E_GrP4UpiValueSet:
				m_Value	=	A_PtrInt[1];
				if ( m_ParaDoPrgsBarBase.ValMin > m_Value )
				{
					m_Value	=	m_ParaDoPrgsBarBase.ValMin;
				}
				if ( m_ParaDoPrgsBarBase.ValMax < m_Value )
				{
					m_Value	=	m_ParaDoPrgsBarBase.ValMax;
				}
				LcPrgsStrUpdt();
				ReqDraw();
				break;
			case E_GrP4UpiMinValueGet:
				A_PtrInt[0]	=	m_ParaDoPrgsBarBase.ValMin;
				break;
			case E_GrP4UpiMinValueSet:
				m_ParaDoPrgsBarBase.ValMin	=	A_PtrInt[1];
				if ( m_ParaDoPrgsBarBase.ValMin > m_Value )
				{
					m_Value	=	m_ParaDoPrgsBarBase.ValMin;
					LcPrgsStrUpdt();
					ReqDraw();
				}
				break;
			case E_GrP4UpiMaxValueGet:
				A_PtrInt[0]	=	m_ParaDoPrgsBarBase.ValMax;
				break;
			case E_GrP4UpiMaxValueSet:
				m_ParaDoPrgsBarBase.ValMax	=	A_PtrInt[1];
				if ( m_ParaDoPrgsBarBase.ValMax < m_Value )
				{
					m_Value	=	m_ParaDoPrgsBarBase.ValMax;
					LcPrgsStrUpdt();
					ReqDraw();
				}
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPrgsBarBase::ProcPaint( void )
{
	// local -------------------
		St_GrRect	Tv_RcGrp;
		__s32		Tv_Len;
	// code --------------------
		// graph
		if ( m_ParaDoPrgsBarBase.ValMax > m_ParaDoPrgsBarBase.ValMin )
		{
			if ( BasisRectCalc( &m_PntRcScr, m_ParaDoPrgsBarBase.GrpPosBss, 
						m_ParaDoPrgsBarBase.GrpLeft, m_ParaDoPrgsBarBase.GrpTop, m_ParaDoPrgsBarBase.GrpRight, m_ParaDoPrgsBarBase.GrpBottom, 
						&Tv_RcGrp ) )
			{
				if ( m_ParaDoPrgsBarBase.IsVertical )
				{
					Tv_Len	=	((m_Value - m_ParaDoPrgsBarBase.ValMin) * (Tv_RcGrp.bottom - Tv_RcGrp.top)) / (m_ParaDoPrgsBarBase.ValMax - m_ParaDoPrgsBarBase.ValMin);
					if ( m_ParaDoPrgsBarBase.IsReverse )
					{
						Tv_RcGrp.bottom	=	Tv_RcGrp.top + Tv_Len;
					}
					else
					{
						Tv_RcGrp.top	=	Tv_RcGrp.bottom - Tv_Len;
					}
				}
				else
				{
					Tv_Len	=	((m_Value - m_ParaDoPrgsBarBase.ValMin) * (Tv_RcGrp.right - Tv_RcGrp.left)) / (m_ParaDoPrgsBarBase.ValMax - m_ParaDoPrgsBarBase.ValMin);
					if ( m_ParaDoPrgsBarBase.IsReverse )
					{
						Tv_RcGrp.left		=	Tv_RcGrp.right - Tv_Len;
					}
					else
					{
						Tv_RcGrp.right	=	Tv_RcGrp.left + Tv_Len;
					}
				}
				PaintScriptPush( m_ParaDoPrgsBarBase.PntGrp, &Tv_RcGrp );
			}
		}
		
		// back ground
		PaintScriptPush( m_ParaDoPrgsBarBase.PntBg, &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPrgsBarBase::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P4_EDITOR
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoPrgsBarBase, V_GrP4DoPrgsBarBaseParaInfo, E_GrP4DoPrgsBarBaseParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPrgsBarBase::LcPrgsStrUpdt( void )
{
	// local -------------------
		__s32		Tv_Prgs;
	// code --------------------
		if ( m_ParaDoPrgsBarBase.ValMax > m_ParaDoPrgsBarBase.ValMin )
		{
			Tv_Prgs	=	((m_Value - m_ParaDoPrgsBarBase.ValMin) * 100) / (m_ParaDoPrgsBarBase.ValMax - m_ParaDoPrgsBarBase.ValMin);
			GrStrIntToWstr( m_StrPrgs, Tv_Prgs );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoPrgsBarBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;

		if ( 0 == A_Idx )
		{
			Tv_Result	=	m_StrPrgs;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPrgsBarBase::LcValLoad( void )
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPrgsBarBase::LcValSave( void )
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
