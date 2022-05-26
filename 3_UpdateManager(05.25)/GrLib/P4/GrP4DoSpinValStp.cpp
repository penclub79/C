/*
		platform spin value setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSpinValStp.h>
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

#define E_GrP4DoSpinValStpParaCnt		6

St_GrP4DvlDoPrInfoData	V_GrP4DoSpinValStpParaInfo[E_GrP4DoSpinValStpParaCnt]	=	
{
	{	L"ValAdr",			TRUE,		E_GrP4DvlParaTypeAsmStpAdr,				1,	(void*)&((Ptr_GrP4DoSpinValStpPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"ValType",			TRUE,		E_GrP4DvlParaTypeUsvalType,				1,	(void*)&((Ptr_GrP4DoSpinValStpPara)0)->ValType,NULL,	L"Setup value type"	},
	{	L"IsAdder",			TRUE,		E_GrP4DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP4DoSpinValStpPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",		TRUE,		E_GrP4DvlParaTypeAsmAddType,			1,	(void*)&((Ptr_GrP4DoSpinValStpPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",		TRUE,		E_GrP4DvlParaTypeDword,						1,	(void*)&((Ptr_GrP4DoSpinValStpPara)0)->AdderVal, (void*)&((Ptr_GrP4DoSpinValStpPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",			TRUE,		E_GrP4DvlParaTypeWord,						1,	(void*)&((Ptr_GrP4DoSpinValStpPara)0)->AddMult,NULL,	L"address adder multipler"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSpinValStp::Cls_GrP4DoSpinValStp(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoSpinValBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSpinValStp;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinValStp) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSpinValStp, sizeof(m_ParaSpinValStp) );

}
//--------------------------------------------------------------------
Cls_GrP4DoSpinValStp::~Cls_GrP4DoSpinValStp( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinValStp::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoSpinValBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSpinValStp;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinValStp);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinValStp, sizeof(m_ParaSpinValStp) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinValStp)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinValStp::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoSpinValBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinValStp, sizeof(m_ParaSpinValStp) , m_PtrScpt , E_GrP4DoClassSpinValStp );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinValStp::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoSpinValBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinValStp::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoSpinValBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinValStp, V_GrP4DoSpinValStpParaInfo, E_GrP4DoSpinValStpParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinValStp::LcValLoad( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		void*	Tv_PtrVal;
	// code --------------------
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaSpinValStp.ValAdr;
		if ( m_ParaSpinValStp.IsAdder )
		{
			Tv_Adder	=	V_GrP4Mng->SysValueGet( m_ParaSpinValStp.AdderType, m_ParaSpinValStp.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaSpinValStp.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP4MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	V_GrP4Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			switch ( m_ParaSpinValStp.ValType )
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
void	Cls_GrP4DoSpinValStp::LcValSave( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		void*	Tv_PtrVal;
	// code --------------------
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaSpinValStp.ValAdr;
		if ( m_ParaSpinValStp.IsAdder )
		{
			Tv_Adder	=	V_GrP4Mng->SysValueGet( m_ParaSpinValStp.AdderType, m_ParaSpinValStp.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaSpinValStp.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP4MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	V_GrP4Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			switch ( m_ParaSpinValStp.ValType )
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


