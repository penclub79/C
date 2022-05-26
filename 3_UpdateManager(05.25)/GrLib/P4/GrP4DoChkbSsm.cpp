/*
		platform check button static setup bitmap

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoChkbSsm.h>
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

#define E_GrP4DoChkbSsmParaCnt		8

St_GrP4DvlDoPrInfoData	V_GrP4DoChkbSsmParaInfo[E_GrP4DoChkbSsmParaCnt]	=	
{
	{	L"TxtIdFalse",	TRUE,		E_GrP4DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP4DoChkbSsmPara)0)->TxtIdFalse,NULL,	L"text id false"	},
	{	L"TxtIdTrue",		TRUE,		E_GrP4DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP4DoChkbSsmPara)0)->TxtIdTrue,NULL,	L"text id true"	},
	{	L"ValAdr",			TRUE,		E_GrP4DvlParaTypeAsmStpAdr,				1,	(void*)&((Ptr_GrP4DoChkbSsmPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"IsAdder",			TRUE,		E_GrP4DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP4DoChkbSsmPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",		TRUE,		E_GrP4DvlParaTypeAsmAddType,			1,	(void*)&((Ptr_GrP4DoChkbSsmPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",		TRUE,		E_GrP4DvlParaTypeDword,						1,	(void*)&((Ptr_GrP4DoChkbSsmPara)0)->AdderVal, (void*)&((Ptr_GrP4DoChkbSsmPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",			TRUE,		E_GrP4DvlParaTypeWord,						1,	(void*)&((Ptr_GrP4DoChkbSsmPara)0)->AddMult,NULL,	L"address adder multipler"	},
	//{	L"ValType",			TRUE,		E_GrP4DvlParaTypeUsvalType,				1,	(void*)&((Ptr_GrP4DoChkbSsmPara)0)->ValType,NULL,	L"value type"	},
	{	L"BitPos",			TRUE,		E_GrP4DvlParaTypeByte,						1,	(void*)&((Ptr_GrP4DoChkbSsmPara)0)->BitPos,NULL,	L"value bit position"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoChkbSsm::Cls_GrP4DoChkbSsm(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoChkbBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassChkbSsm;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaChkbSsm) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaChkbSsm, sizeof(m_ParaChkbSsm) );

}
//--------------------------------------------------------------------
Cls_GrP4DoChkbSsm::~Cls_GrP4DoChkbSsm( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoChkbSsm::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoChkbBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassChkbSsm;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaChkbSsm);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaChkbSsm, sizeof(m_ParaChkbSsm) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaChkbSsm)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbSsm::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoChkbBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaChkbSsm, sizeof(m_ParaChkbSsm) , m_PtrScpt , E_GrP4DoClassChkbSsm );

		LcValLoad();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoChkbSsm::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaChkbSsm.TxtIdFalse;
				break;
			case E_GrP4UpiTextIdSet:
				m_ParaChkbSsm.TxtIdTrue		=	(__u16)A_PtrInt[1];
				m_ParaChkbSsm.TxtIdFalse	=	(__u16)A_PtrInt[1];
				ReqDraw();
				break;
			case E_GrP4UpiSetupAdrGet:
				A_PtrInt[0]	=	(__s32)m_ParaChkbSsm.ValAdr;
				break;
			case E_GrP4UpiSetupAdrSet:
				m_ParaChkbSsm.ValAdr	=	(__u16)A_PtrInt[1];
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
void	Cls_GrP4DoChkbSsm::RtlDvlParaInfoSubDump(void)
{
		// inherit
		Cls_GrP4DoChkbBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaChkbSsm, V_GrP4DoChkbSsmParaInfo, E_GrP4DoChkbSsmParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoChkbSsm::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
				Tv_Result	=	Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaChkbSsm.TxtIdTrue );
			}
			else
			{
				Tv_Result	=	Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaChkbSsm.TxtIdFalse );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbSsm::LcValLoad( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		__u32	Tv_BitPos;
		__u8	Tv_Mask;
		__u8*	Tv_PtrVal;
		__u8	Tv_Val;
	// code --------------------
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaChkbSsm.ValAdr;
		if ( m_ParaChkbSsm.IsAdder )
		{
			Tv_Adder	=	V_GrP4Mng->SysValueGet( m_ParaChkbSsm.AdderType, m_ParaChkbSsm.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaChkbSsm.AddMult;
		}

		// calculate offset
		Tv_BitPos	=	(__u32)m_ParaChkbSsm.BitPos + Tv_Adder;
		Tv_Ofs		=	Tv_Ofs + (Tv_BitPos >> 3);
		Tv_BitPos	=	Tv_BitPos & 0x07;

		// check range
		if ( E_GrP4MngSdaSize > Tv_Ofs )
		{
			Tv_Mask		=	1 << Tv_BitPos;
			Tv_PtrVal	=	(__u8*)V_GrP4Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			Tv_Val		=	*Tv_PtrVal;
			if ( 0 != (Tv_Mask & Tv_Val) )
			{
				m_IsVal	=	TRUE;
			}
			else
			{
				m_IsVal	=	FALSE;
			}
		}
		else
		{
			m_IsVal	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoChkbSsm::LcValSave( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		__u32	Tv_BitPos;
		__u8	Tv_Mask;
		__u8*	Tv_PtrVal;
		__u8	Tv_Val;
	// code --------------------
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaChkbSsm.ValAdr;
		if ( m_ParaChkbSsm.IsAdder )
		{
			Tv_Adder	=	V_GrP4Mng->SysValueGet( m_ParaChkbSsm.AdderType, m_ParaChkbSsm.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaChkbSsm.AddMult;
		}

		// calculate offset
		Tv_BitPos	=	(__u32)m_ParaChkbSsm.BitPos + Tv_Adder;
		Tv_Ofs		=	Tv_Ofs + (Tv_BitPos >> 3);
		Tv_BitPos	=	Tv_BitPos & 0x07;

		// check range
		if ( E_GrP4MngSdaSize > Tv_Ofs )
		{
			Tv_Mask		=	1 << Tv_BitPos;
			Tv_PtrVal	=	(__u8*)V_GrP4Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			Tv_Val		=	*Tv_PtrVal;
			if ( m_IsVal )
			{
				Tv_Val	=	Tv_Val | Tv_Mask;
			}
			else
			{
				Tv_Val	=	Tv_Val & (~Tv_Mask);
			}
			*Tv_PtrVal	=	Tv_Val;		// update value
		}
}
//--------------------------------------------------------------------

