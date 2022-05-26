/*
		platform edit setup string

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoEdtStpStr.h>
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

#define E_GrP3DoEdtStpStrParaCnt		5

St_GrP3DvlDoPrInfoData	V_GrP3DoEdtStpStrParaInfo[E_GrP3DoEdtStpStrParaCnt]	=	
{
	{	L"Setup Address",			TRUE,		E_GrP3DvlParaTypeAsmStpAdr,			1,	(void*)&((Ptr_GrP3DoEdtStpStrPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"IsAdder",						TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoEdtStpStrPara)0)->IsAdder,NULL,	L"use address adder"	},
	{	L"Adder Type",				TRUE,		E_GrP3DvlParaTypeAsmMemType,		1,	(void*)&((Ptr_GrP3DoEdtStpStrPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"Adder Value",				TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoEdtStpStrPara)0)->AdderVal, (void*)&((Ptr_GrP3DoEdtStpStrPara)0)->AdderType,	L"address adder value"	},
	{	L"Adder Multiplier",	TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoEdtStpStrPara)0)->AddMult,NULL,	L"address adder multipler"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoEdtStpStr::Cls_GrP3DoEdtStpStr( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoEdtBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassEdtStpStr;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaEdtStpStr) + sizeof(St_GrP3ScptParaHd);

		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaEdtStpStr, sizeof(m_ParaEdtStpStr) );
		
}
//--------------------------------------------------------------------
Cls_GrP3DoEdtStpStr::~Cls_GrP3DoEdtStpStr( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoEdtStpStr::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoEdtBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassEdtStpStr;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaEdtStpStr);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaEdtStpStr, sizeof(m_ParaEdtStpStr) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaEdtStpStr)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoEdtStpStr::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoEdtBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaEdtStpStr, sizeof(m_ParaEdtStpStr) , m_PtrScpt , E_GrP3DoClassEdtStpStr );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoEdtStpStr::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoEdtBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoEdtStpStr::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoEdtBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaEdtStpStr, V_GrP3DoEdtStpStrParaInfo, E_GrP3DoEdtStpStrParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoEdtStpStr::LcValAdrGet( void )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16		Tv_Ofs;
		__u16		Tv_Adder;
	// code --------------------
		Tv_Result	=	NULL;
		// check offset and length
		Tv_Ofs	=	m_ParaEdtStpStr.ValAdr;
		if ( m_ParaEdtStpStr.IsAdder )
		{
			Tv_Adder	=	(__u16)V_GrP3Mng->VmValueGet( m_ParaEdtStpStr.AdderType, m_ParaEdtStpStr.AdderVal );
			Tv_Adder	=	Tv_Adder * m_ParaEdtStpStr.AddMult;
			Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		}
		if ( E_GrP3MngSdaSize >= ((__u32)Tv_Ofs + ((__u32)m_ParaEdtBase.MaxLen * 2) ) )
		{
			Tv_Result	=	(WCHAR*)V_GrP3Mng->SetupDataPtrGet( Tv_Ofs );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
