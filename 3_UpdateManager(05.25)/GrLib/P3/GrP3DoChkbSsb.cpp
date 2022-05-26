/*
		platform spin number setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoChkbSsb.h>
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

#define E_GrP3DoChkbSsbParaCnt		7

St_GrP3DvlDoPrInfoData	V_GrP3DoChkbSsbParaInfo[E_GrP3DoChkbSsbParaCnt]	=	
{
	{	L"TxtIdFalse",	TRUE,		E_GrP3DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP3DoChkbSsbPara)0)->TxtIdFalse,NULL,	L"text id false"	},
	{	L"TxtIdTrue",		TRUE,		E_GrP3DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP3DoChkbSsbPara)0)->TxtIdTrue,NULL,	L"text id true"	},
	{	L"ValAdr",			TRUE,		E_GrP3DvlParaTypeAsmStpAdr,				1,	(void*)&((Ptr_GrP3DoChkbSsbPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"IsAdder",			TRUE,		E_GrP3DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP3DoChkbSsbPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",		TRUE,		E_GrP3DvlParaTypeAsmMemType,			1,	(void*)&((Ptr_GrP3DoChkbSsbPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",		TRUE,		E_GrP3DvlParaTypeDword,						1,	(void*)&((Ptr_GrP3DoChkbSsbPara)0)->AdderVal, (void*)&((Ptr_GrP3DoChkbSsbPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",			TRUE,		E_GrP3DvlParaTypeWord,						1,	(void*)&((Ptr_GrP3DoChkbSsbPara)0)->AddMult,NULL,	L"address adder multipler"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoChkbSsb::Cls_GrP3DoChkbSsb( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoChkbBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassChkbSsb;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaChkbSsb) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaChkbSsb, sizeof(m_ParaChkbSsb) );

}
//--------------------------------------------------------------------
Cls_GrP3DoChkbSsb::~Cls_GrP3DoChkbSsb( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSsb::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoChkbBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassChkbSsb;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaChkbSsb);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaChkbSsb, sizeof(m_ParaChkbSsb) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaChkbSsb)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSsb::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoChkbBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaChkbSsb, sizeof(m_ParaChkbSsb) , m_PtrScpt , E_GrP3DoClassChkbSsb );

		LcValLoad();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoChkbSsb::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaChkbSsb.TxtIdFalse;
				break;
			case E_GrP3UpiTextIdSet:
				m_ParaChkbSsb.TxtIdTrue		=	(__u16)A_PtrInt[1];
				m_ParaChkbSsb.TxtIdFalse	=	(__u16)A_PtrInt[1];
				ReqDraw();
				break;
			case E_GrP3UpiSetupAdrGet:
				A_PtrInt[0]	=	(__s32)m_ParaChkbSsb.ValAdr;
				break;
			case E_GrP3UpiSetupAdrSet:
				m_ParaChkbSsb.ValAdr	=	(__u16)A_PtrInt[1];
				LcValLoad();
				ReqDraw();
				break;

			default:
				Tv_Result	=	Cls_GrP3DoChkbBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSsb::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoChkbBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaChkbSsb, V_GrP3DoChkbSsbParaInfo, E_GrP3DoChkbSsbParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoChkbSsb::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
				Tv_Result	=	Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaChkbSsb.TxtIdTrue );
			}
			else
			{
				Tv_Result	=	Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaChkbSsb.TxtIdFalse );
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSsb::LcValLoad( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		BOOL8*	Tv_PtrVal;
	// code --------------------
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaChkbSsb.ValAdr;
		if ( m_ParaChkbSsb.IsAdder )
		{
			Tv_Adder	=	V_GrP3Mng->VmValueGet( m_ParaChkbSsb.AdderType, m_ParaChkbSsb.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaChkbSsb.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	(BOOL8*)V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			m_IsVal	=	*Tv_PtrVal;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSsb::LcValSave( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		BOOL8*	Tv_PtrVal;
	// code --------------------
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaChkbSsb.ValAdr;
		if ( m_ParaChkbSsb.IsAdder )
		{
			Tv_Adder	=	V_GrP3Mng->VmValueGet( m_ParaChkbSsb.AdderType, m_ParaChkbSsb.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaChkbSsb.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	(BOOL8*)V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			*Tv_PtrVal	=	m_IsVal;
		}
}
//--------------------------------------------------------------------

