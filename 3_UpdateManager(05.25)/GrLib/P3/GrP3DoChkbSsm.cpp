/*
		platform check button static setup bitmap

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoChkbSsm.h>
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

#define E_GrP3DoChkbSsmParaCnt		8

St_GrP3DvlDoPrInfoData	V_GrP3DoChkbSsmParaInfo[E_GrP3DoChkbSsmParaCnt]	=	
{
	{	L"TxtIdFalse",	TRUE,		E_GrP3DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP3DoChkbSsmPara)0)->TxtIdFalse,NULL,	L"text id false"	},
	{	L"TxtIdTrue",		TRUE,		E_GrP3DvlParaTypeTxtId,						1,	(void*)&((Ptr_GrP3DoChkbSsmPara)0)->TxtIdTrue,NULL,	L"text id true"	},
	{	L"ValAdr",			TRUE,		E_GrP3DvlParaTypeAsmStpAdr,				1,	(void*)&((Ptr_GrP3DoChkbSsmPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"IsAdder",			TRUE,		E_GrP3DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP3DoChkbSsmPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",		TRUE,		E_GrP3DvlParaTypeAsmMemType,			1,	(void*)&((Ptr_GrP3DoChkbSsmPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",		TRUE,		E_GrP3DvlParaTypeDword,						1,	(void*)&((Ptr_GrP3DoChkbSsmPara)0)->AdderVal, (void*)&((Ptr_GrP3DoChkbSsmPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",			TRUE,		E_GrP3DvlParaTypeWord,						1,	(void*)&((Ptr_GrP3DoChkbSsmPara)0)->AddMult,NULL,	L"address adder multipler"	},
	//{	L"ValType",			TRUE,		E_GrP3DvlParaTypeUsvalType,				1,	(void*)&((Ptr_GrP3DoChkbSsmPara)0)->ValType,NULL,	L"value type"	},
	{	L"BitPos",			TRUE,		E_GrP3DvlParaTypeByte,						1,	(void*)&((Ptr_GrP3DoChkbSsmPara)0)->BitPos,NULL,	L"value bit position"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoChkbSsm::Cls_GrP3DoChkbSsm( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoChkbBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassChkbSsm;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaChkbSsm) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaChkbSsm, sizeof(m_ParaChkbSsm) );

}
//--------------------------------------------------------------------
Cls_GrP3DoChkbSsm::~Cls_GrP3DoChkbSsm( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSsm::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoChkbBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassChkbSsm;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaChkbSsm);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaChkbSsm, sizeof(m_ParaChkbSsm) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaChkbSsm)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSsm::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoChkbBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaChkbSsm, sizeof(m_ParaChkbSsm) , m_PtrScpt , E_GrP3DoClassChkbSsm );

		LcValLoad();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoChkbSsm::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiTextIdGet:
				A_PtrInt[0]	=	(__s32)m_ParaChkbSsm.TxtIdFalse;
				break;
			case E_GrP3UpiTextIdSet:
				m_ParaChkbSsm.TxtIdTrue		=	(__u16)A_PtrInt[1];
				m_ParaChkbSsm.TxtIdFalse	=	(__u16)A_PtrInt[1];
				ReqDraw();
				break;
			case E_GrP3UpiSetupAdrGet:
				A_PtrInt[0]	=	(__s32)m_ParaChkbSsm.ValAdr;
				break;
			case E_GrP3UpiSetupAdrSet:
				m_ParaChkbSsm.ValAdr	=	(__u16)A_PtrInt[1];
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
void	Cls_GrP3DoChkbSsm::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherit
		Cls_GrP3DoChkbBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaChkbSsm, V_GrP3DoChkbSsmParaInfo, E_GrP3DoChkbSsmParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoChkbSsm::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
				Tv_Result	=	Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaChkbSsm.TxtIdTrue );
			}
			else
			{
				Tv_Result	=	Tv_Result	=	V_GrP3Mng->TextPtrGet( m_ParaChkbSsm.TxtIdFalse );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoChkbSsm::LcValLoad( void )
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
			Tv_Adder	=	V_GrP3Mng->VmValueGet( m_ParaChkbSsm.AdderType, m_ParaChkbSsm.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaChkbSsm.AddMult;
		}

		// calculate offset
		Tv_BitPos	=	(__u32)m_ParaChkbSsm.BitPos + Tv_Adder;
		Tv_Ofs		=	Tv_Ofs + (Tv_BitPos >> 3);
		Tv_BitPos	=	Tv_BitPos & 0x07;

		// check range
		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_Mask		=	1 << Tv_BitPos;
			Tv_PtrVal	=	(__u8*)V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
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
void	Cls_GrP3DoChkbSsm::LcValSave( void )
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
			Tv_Adder	=	V_GrP3Mng->VmValueGet( m_ParaChkbSsm.AdderType, m_ParaChkbSsm.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaChkbSsm.AddMult;
		}

		// calculate offset
		Tv_BitPos	=	(__u32)m_ParaChkbSsm.BitPos + Tv_Adder;
		Tv_Ofs		=	Tv_Ofs + (Tv_BitPos >> 3);
		Tv_BitPos	=	Tv_BitPos & 0x07;

		// check range
		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_Mask		=	1 << Tv_BitPos;
			Tv_PtrVal	=	(__u8*)V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
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

