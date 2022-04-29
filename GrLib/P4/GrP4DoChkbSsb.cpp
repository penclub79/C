/*
		platform spin number setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoChkbSsb.h>
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

#define E_GrP4DoChkbSsbParaCnt		7

St_GrP4DvlDoPrInfoData	V_GrP4DoChkbSsbParaInfo[E_GrP4DoChkbSsbParaCnt]	=	
{
	{	L"TxtIdFalse",	TRUE,		E_GrP4DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP4DoChkbSsbPara)0)->TxtIdFalse,NULL,	L"text id false"	},
	{	L"TxtIdTrue",		TRUE,		E_GrP4DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP4DoChkbSsbPara)0)->TxtIdTrue,NULL,	L"text id true"	},
	{	L"ValAdr",			TRUE,		E_GrP4DvlParaTypeAsmStpAdr,				1,	(void*)&((Ptr_GrP4DoChkbSsbPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"IsAdder",			TRUE,		E_GrP4DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP4DoChkbSsbPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",		TRUE,		E_GrP4DvlParaTypeAsmAddType,			1,	(void*)&((Ptr_GrP4DoChkbSsbPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",		TRUE,		E_GrP4DvlParaTypeDword,						1,	(void*)&((Ptr_GrP4DoChkbSsbPara)0)->AdderVal, (void*)&((Ptr_GrP4DoChkbSsbPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",			TRUE,		E_GrP4DvlParaTypeWord,						1,	(void*)&((Ptr_GrP4DoChkbSsbPara)0)->AddMult,NULL,	L"address adder multipler"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoChkbSsb::Cls_GrP4DoChkbSsb(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoChkbBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassChkbSsb;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaChkbSsb) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaChkbSsb, sizeof(m_ParaChkbSsb) );

}
//--------------------------------------------------------------------
Cls_GrP4DoChkbSsb::~Cls_GrP4DoChkbSsb( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoChkbSsb::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoChkbBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassChkbSsb;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaChkbSsb);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaChkbSsb, sizeof(m_ParaChkbSsb) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaChkbSsb)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbSsb::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoChkbBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaChkbSsb, sizeof(m_ParaChkbSsb) , m_PtrScpt , E_GrP4DoClassChkbSsb );

		LcValLoad();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoChkbSsb::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaChkbSsb.TxtIdFalse;
				break;
			case E_GrP4UpiTextIdSet:
				m_ParaChkbSsb.TxtIdTrue		=	(__u16)A_PtrInt[1];
				m_ParaChkbSsb.TxtIdFalse	=	(__u16)A_PtrInt[1];
				ReqDraw();
				break;
			case E_GrP4UpiSetupAdrGet:
				A_PtrInt[0]	=	(__s32)m_ParaChkbSsb.ValAdr;
				break;
			case E_GrP4UpiSetupAdrSet:
				m_ParaChkbSsb.ValAdr	=	(__u16)A_PtrInt[1];
				LcValLoad();
				ReqDraw();
				break;

			default:
				Tv_Result	=	Cls_GrP4DoChkbBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoChkbSsb::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoChkbBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaChkbSsb, V_GrP4DoChkbSsbParaInfo, E_GrP4DoChkbSsbParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoChkbSsb::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
				Tv_Result	=	Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaChkbSsb.TxtIdTrue );
			}
			else
			{
				Tv_Result	=	Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaChkbSsb.TxtIdFalse );
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbSsb::LcValLoad( void )
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
			Tv_Adder	=	V_GrP4Mng->SysValueGet( m_ParaChkbSsb.AdderType, m_ParaChkbSsb.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaChkbSsb.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP4MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	(BOOL8*)V_GrP4Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			m_IsVal	=	*Tv_PtrVal;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbSsb::LcValSave( void )
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
			Tv_Adder	=	V_GrP4Mng->SysValueGet(m_ParaChkbSsb.AdderType, m_ParaChkbSsb.AdderVal);
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaChkbSsb.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP4MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	(BOOL8*)V_GrP4Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			*Tv_PtrVal	=	m_IsVal;
		}
}
//--------------------------------------------------------------------

