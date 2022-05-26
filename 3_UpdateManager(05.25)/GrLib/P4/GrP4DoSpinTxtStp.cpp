/*
		platform spin number setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSpinTxtStp.h>
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

#define E_GrP4DoSpinTxtStpParaCnt		6

St_GrP4DvlDoPrInfoData	V_GrP4DoSpinTxtStpParaInfo[E_GrP4DoSpinTxtStpParaCnt]	=	
{
	{	L"ValAdr",			TRUE,		E_GrP4DvlParaTypeAsmStpAdr,				1,	(void*)&((Ptr_GrP4DoSpinTxtStpPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"ValType",			TRUE,		E_GrP4DvlParaTypeUsvalType,				1,	(void*)&((Ptr_GrP4DoSpinTxtStpPara)0)->ValType,NULL,	L"Setup value type"	},
	{	L"IsAdder",			TRUE,		E_GrP4DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP4DoSpinTxtStpPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",		TRUE,		E_GrP4DvlParaTypeAsmAddType,			1,	(void*)&((Ptr_GrP4DoSpinTxtStpPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",		TRUE,		E_GrP4DvlParaTypeDword,						1,	(void*)&((Ptr_GrP4DoSpinTxtStpPara)0)->AdderVal, (void*)&((Ptr_GrP4DoSpinTxtStpPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",			TRUE,		E_GrP4DvlParaTypeWord,						1,	(void*)&((Ptr_GrP4DoSpinTxtStpPara)0)->AddMult,NULL,	L"address adder multipler"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSpinTxtStp::Cls_GrP4DoSpinTxtStp(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoSpinTxtBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSpinTxtStp;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinTxtStp) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSpinTxtStp, sizeof(m_ParaSpinTxtStp) );

}
//--------------------------------------------------------------------
Cls_GrP4DoSpinTxtStp::~Cls_GrP4DoSpinTxtStp( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinTxtStp::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoSpinTxtBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSpinTxtStp;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinTxtStp);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinTxtStp, sizeof(m_ParaSpinTxtStp) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinTxtStp)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtStp::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoSpinTxtBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinTxtStp, sizeof(m_ParaSpinTxtStp) , m_PtrScpt , E_GrP4DoClassSpinTxtStp );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinTxtStp::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoSpinTxtBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinTxtStp::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoSpinTxtBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinTxtStp, V_GrP4DoSpinTxtStpParaInfo, E_GrP4DoSpinTxtStpParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtStp::LcValLoad( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		void*	Tv_PtrVal;
	// code --------------------
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaSpinTxtStp.ValAdr;
		if ( m_ParaSpinTxtStp.IsAdder )
		{
			Tv_Adder	=	V_GrP4Mng->SysValueGet( m_ParaSpinTxtStp.AdderType, m_ParaSpinTxtStp.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaSpinTxtStp.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP4MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	V_GrP4Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			switch ( m_ParaSpinTxtStp.ValType )
			{
				case E_GrP4AsmArgTypeSzByte:
					m_Value	=	(__u32)(*((__u8*)Tv_PtrVal));
					break;
				case E_GrP4AsmArgTypeSzWord:
					if ( E_GrP4MngSdaSize > (Tv_Ofs + 1) )
					{
						m_Value	=	(__u32)(*((__u16*)Tv_PtrVal));
					}
					break;
				case E_GrP4AsmArgTypeSzDword:
					if ( E_GrP4MngSdaSize > (Tv_Ofs + 3) )
					{
						m_Value	=	(__u32)(*((__u32*)Tv_PtrVal));
					}
					break;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinTxtStp::LcValSave( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		void*	Tv_PtrVal;
	// code --------------------
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaSpinTxtStp.ValAdr;
		if ( m_ParaSpinTxtStp.IsAdder )
		{
			Tv_Adder	=	V_GrP4Mng->SysValueGet( m_ParaSpinTxtStp.AdderType, m_ParaSpinTxtStp.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaSpinTxtStp.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP4MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	V_GrP4Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			switch ( m_ParaSpinTxtStp.ValType )
			{
				case E_GrP4AsmArgTypeSzByte:
					*((__u8*)Tv_PtrVal)	=	(__u8)m_Value;
					break;
				case E_GrP4AsmArgTypeSzWord:
					if ( E_GrP4MngSdaSize > (Tv_Ofs + 1) )
					{
						*((__u16*)Tv_PtrVal)	=	(__u16)m_Value;
					}
					break;
				case E_GrP4AsmArgTypeSzDword:
					if ( E_GrP4MngSdaSize > (Tv_Ofs + 3) )
					{
						*((__u32*)Tv_PtrVal)	=	(__u32)m_Value;
					}
					break;
			}
		}
}
//--------------------------------------------------------------------


