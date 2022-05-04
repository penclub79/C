/*
		platform label

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoPrgsBarBase.h>
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

#define E_GrP3DoPrgsBarBaseParaCnt		14

St_GrP3DvlDoPrInfoData	V_GrP3DoPrgsBarBaseParaInfo[E_GrP3DoPrgsBarBaseParaCnt]	=	
{
	{	L"GrpPosLeft",		TRUE,		E_GrP3DvlParaTypePosBssLt,			1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->GrpPosBss,NULL,	L"Graph position left"	},
	{	L"",							FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->GrpLeft,NULL,		L""	},
	{	L"GrpPosTop",			TRUE,		E_GrP3DvlParaTypePosBssUp,			1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->GrpPosBss,NULL,	L"Graph position top"	},
	{	L"",							FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->GrpTop,NULL,			L""	},
	{	L"GrpPosRight",		TRUE,		E_GrP3DvlParaTypePosBssRt,			1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->GrpPosBss,NULL,	L"Graph position right"	},
	{	L"",							FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->GrpRight,NULL,		L""	},
	{	L"GrpPosBottom",	TRUE,		E_GrP3DvlParaTypePosBssDn,			1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->GrpPosBss,NULL,	L"Graph position bottom"	},
	{	L"",							FALSE,	E_GrP3DvlParaTypeShort,					1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->GrpBottom,NULL,	L""	},
	{	L"IsVertical",		TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->IsVertical,NULL,	L"Vertical / horizontal type"	},
	{	L"IsReverse",			TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->IsReverse,NULL,	L"reverse graph direction"	},
	{	L"Paint BG",			TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->PntBg,NULL,			L"Paint background code.\n0 = None, 1 = basic code"	},
	{	L"Paint Graph",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->PntGrp,NULL,			L"Paint graph code.\n0 = None, 1 = basic code"	},
	{	L"Value Min",			TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->ValMin,NULL,			L"Value range minimal"	},
	{	L"Value Max",			TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoPrgsBarBasePara)0)->ValMax,NULL,			L"Value range maximal"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoPrgsBarBase::Cls_GrP3DoPrgsBarBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassPrgsBarBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoPrgsBarBase) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaDoPrgsBarBase, sizeof(m_ParaDoPrgsBarBase) );
		m_ParaDoPrgsBarBase.PntBg		=	E_GrP3ScptPstIdDflt;
		m_ParaDoPrgsBarBase.ValMax	=	100;

#ifdef GR_P3_EDITOR
		m_Value			=	50;
#else
		m_Value			=	0;
#endif
		GrStrClear( m_StrPrgs );
}
//--------------------------------------------------------------------
Cls_GrP3DoPrgsBarBase::~Cls_GrP3DoPrgsBarBase( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPrgsBarBase::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassPrgsBarBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoPrgsBarBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoPrgsBarBase, sizeof(m_ParaDoPrgsBarBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoPrgsBarBase)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPrgsBarBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoPrgsBarBase, sizeof(m_ParaDoPrgsBarBase) , m_PtrScpt , E_GrP3DoClassPrgsBarBase );

		LcPrgsStrUpdt();		// progress string update
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoPrgsBarBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiValueGet:
				LcValLoad();
				A_PtrInt[0]	=	m_Value;
				break;
			case E_GrP3UpiValueSet:
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
			case E_GrP3UpiMinValueGet:
				A_PtrInt[0]	=	m_ParaDoPrgsBarBase.ValMin;
				break;
			case E_GrP3UpiMinValueSet:
				m_ParaDoPrgsBarBase.ValMin	=	A_PtrInt[1];
				if ( m_ParaDoPrgsBarBase.ValMin > m_Value )
				{
					m_Value	=	m_ParaDoPrgsBarBase.ValMin;
					LcPrgsStrUpdt();
					ReqDraw();
				}
				break;
			case E_GrP3UpiMaxValueGet:
				A_PtrInt[0]	=	m_ParaDoPrgsBarBase.ValMax;
				break;
			case E_GrP3UpiMaxValueSet:
				m_ParaDoPrgsBarBase.ValMax	=	A_PtrInt[1];
				if ( m_ParaDoPrgsBarBase.ValMax < m_Value )
				{
					m_Value	=	m_ParaDoPrgsBarBase.ValMax;
					LcPrgsStrUpdt();
					ReqDraw();
				}
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPrgsBarBase::ProcPaint( void )
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
void	Cls_GrP3DoPrgsBarBase::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoPrgsBarBase, V_GrP3DoPrgsBarBaseParaInfo, E_GrP3DoPrgsBarBaseParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPrgsBarBase::LcPrgsStrUpdt( void )
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
WCHAR*	Cls_GrP3DoPrgsBarBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
void	Cls_GrP3DoPrgsBarBase::LcValLoad( void )
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPrgsBarBase::LcValSave( void )
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
