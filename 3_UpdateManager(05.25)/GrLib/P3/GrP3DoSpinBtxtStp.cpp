/*
		platform spin buffer text setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSpinBtxtStp.h>
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

#define E_GrP3DoSpinBtxtStpParaCnt		6

St_GrP3DvlDoPrInfoData	V_GrP3DoSpinBtxtStpParaInfo[E_GrP3DoSpinBtxtStpParaCnt]	=	
{
	{	L"ValAdr",			TRUE,		E_GrP3DvlParaTypeAsmStpAdr,				1,	(void*)&((Ptr_GrP3DoSpinBtxtStpPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"ValType",			TRUE,		E_GrP3DvlParaTypeUsvalType,				1,	(void*)&((Ptr_GrP3DoSpinBtxtStpPara)0)->ValType,NULL,	L"Setup value type"	},
	{	L"IsAdder",			TRUE,		E_GrP3DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP3DoSpinBtxtStpPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",		TRUE,		E_GrP3DvlParaTypeAsmMemType,			1,	(void*)&((Ptr_GrP3DoSpinBtxtStpPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",		TRUE,		E_GrP3DvlParaTypeDword,						1,	(void*)&((Ptr_GrP3DoSpinBtxtStpPara)0)->AdderVal, (void*)&((Ptr_GrP3DoSpinBtxtStpPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",			TRUE,		E_GrP3DvlParaTypeWord,						1,	(void*)&((Ptr_GrP3DoSpinBtxtStpPara)0)->AddMult,NULL,	L"address adder multipler"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSpinBtxtStp::Cls_GrP3DoSpinBtxtStp( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoSpinBtxtBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassSpinBtxtStp;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinBtxtStp) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSpinBtxtStp, sizeof(m_ParaSpinBtxtStp) );

}
//--------------------------------------------------------------------
Cls_GrP3DoSpinBtxtStp::~Cls_GrP3DoSpinBtxtStp( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtStp::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoSpinBtxtBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSpinBtxtStp;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinBtxtStp);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinBtxtStp, sizeof(m_ParaSpinBtxtStp) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinBtxtStp)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtStp::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoSpinBtxtBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinBtxtStp, sizeof(m_ParaSpinBtxtStp) , m_PtrScpt , E_GrP3DoClassSpinBtxtStp );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinBtxtStp::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoSpinBtxtBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtStp::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoSpinBtxtBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinBtxtStp, V_GrP3DoSpinBtxtStpParaInfo, E_GrP3DoSpinBtxtStpParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtStp::LcValLoad( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		void*	Tv_PtrVal;
	// code --------------------
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaSpinBtxtStp.ValAdr;
		if ( m_ParaSpinBtxtStp.IsAdder )
		{
			Tv_Adder	=	V_GrP3Mng->VmValueGet( m_ParaSpinBtxtStp.AdderType, m_ParaSpinBtxtStp.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaSpinBtxtStp.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			switch ( m_ParaSpinBtxtStp.ValType )
			{
				case E_GrP3AsmArgTypeSzByte:
					m_Value	=	(__u32)(*((__u8*)Tv_PtrVal));
					break;
				case E_GrP3AsmArgTypeSzWord:
					if ( E_GrP3MngSdaSize > (Tv_Ofs + 1) )
					{
						m_Value	=	(__u32)(*((__u16*)Tv_PtrVal));
					}
					break;
				case E_GrP3AsmArgTypeSzDword:
					if ( E_GrP3MngSdaSize > (Tv_Ofs + 3) )
					{
						m_Value	=	(__u32)(*((__u32*)Tv_PtrVal));
					}
					break;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinBtxtStp::LcValSave( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		void*	Tv_PtrVal;
	// code --------------------
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaSpinBtxtStp.ValAdr;
		if ( m_ParaSpinBtxtStp.IsAdder )
		{
			Tv_Adder	=	V_GrP3Mng->VmValueGet( m_ParaSpinBtxtStp.AdderType, m_ParaSpinBtxtStp.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaSpinBtxtStp.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			switch ( m_ParaSpinBtxtStp.ValType )
			{
				case E_GrP3AsmArgTypeSzByte:
					*((__u8*)Tv_PtrVal)	=	(__u8)m_Value;
					break;
				case E_GrP3AsmArgTypeSzWord:
					if ( E_GrP3MngSdaSize > (Tv_Ofs + 1) )
					{
						*((__u16*)Tv_PtrVal)	=	(__u16)m_Value;
					}
					break;
				case E_GrP3AsmArgTypeSzDword:
					if ( E_GrP3MngSdaSize > (Tv_Ofs + 3) )
					{
						*((__u32*)Tv_PtrVal)	=	(__u32)m_Value;
					}
					break;
			}
		}
}
//--------------------------------------------------------------------


