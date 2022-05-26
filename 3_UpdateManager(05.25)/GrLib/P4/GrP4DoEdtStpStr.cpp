/*
		platform edit setup string

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoEdtStpStr.h>
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

#define E_GrP4DoEdtStpStrParaCnt		5

St_GrP4DvlDoPrInfoData	V_GrP4DoEdtStpStrParaInfo[E_GrP4DoEdtStpStrParaCnt]	=	
{
	{	L"Setup Address",			TRUE,		E_GrP4DvlParaTypeAsmStpAdr,			1,	(void*)&((Ptr_GrP4DoEdtStpStrPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"IsAdder",						TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoEdtStpStrPara)0)->IsAdder,NULL,	L"use address adder"	},
	{	L"Adder Type",				TRUE,		E_GrP4DvlParaTypeAsmAddType,		1,	(void*)&((Ptr_GrP4DoEdtStpStrPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"Adder Value",				TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4DoEdtStpStrPara)0)->AdderVal, (void*)&((Ptr_GrP4DoEdtStpStrPara)0)->AdderType,	L"address adder value"	},
	{	L"Adder Multiplier",	TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoEdtStpStrPara)0)->AddMult,NULL,	L"address adder multipler"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoEdtStpStr::Cls_GrP4DoEdtStpStr(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoEdtBase( A_ScptId, A_PtrRect, A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassEdtStpStr;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaEdtStpStr) + sizeof(St_GrP4ScptParaHd);

		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaEdtStpStr, sizeof(m_ParaEdtStpStr) );
		
}
//--------------------------------------------------------------------
Cls_GrP4DoEdtStpStr::~Cls_GrP4DoEdtStpStr( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoEdtStpStr::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoEdtBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassEdtStpStr;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaEdtStpStr);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaEdtStpStr, sizeof(m_ParaEdtStpStr) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaEdtStpStr)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoEdtStpStr::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoEdtBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaEdtStpStr, sizeof(m_ParaEdtStpStr) , m_PtrScpt , E_GrP4DoClassEdtStpStr );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoEdtStpStr::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoEdtBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoEdtStpStr::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoEdtBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaEdtStpStr, V_GrP4DoEdtStpStrParaInfo, E_GrP4DoEdtStpStrParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoEdtStpStr::LcValAdrGet( void )
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
			Tv_Adder	=	(__u16)V_GrP4Mng->SysValueGet( m_ParaEdtStpStr.AdderType, m_ParaEdtStpStr.AdderVal );
			Tv_Adder	=	Tv_Adder * m_ParaEdtStpStr.AddMult;
			Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		}
		if ( E_GrP4MngSdaSize >= ((__u32)Tv_Ofs + ((__u32)m_ParaEdtBase.MaxLen * 2) ) )
		{
			Tv_Result	=	(WCHAR*)V_GrP4Mng->SetupDataPtrGet( Tv_Ofs );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
