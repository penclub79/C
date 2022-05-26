/*
		platform spin number setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSpinTxtStp.h>
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

#define E_GrP3DoSpinTxtStpParaCnt		6

St_GrP3DvlDoPrInfoData	V_GrP3DoSpinTxtStpParaInfo[E_GrP3DoSpinTxtStpParaCnt]	=	
{
	{	L"ValAdr",			TRUE,		E_GrP3DvlParaTypeAsmStpAdr,				1,	(void*)&((Ptr_GrP3DoSpinTxtStpPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"ValType",			TRUE,		E_GrP3DvlParaTypeUsvalType,				1,	(void*)&((Ptr_GrP3DoSpinTxtStpPara)0)->ValType,NULL,	L"Setup value type"	},
	{	L"IsAdder",			TRUE,		E_GrP3DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP3DoSpinTxtStpPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",		TRUE,		E_GrP3DvlParaTypeAsmMemType,			1,	(void*)&((Ptr_GrP3DoSpinTxtStpPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",		TRUE,		E_GrP3DvlParaTypeDword,						1,	(void*)&((Ptr_GrP3DoSpinTxtStpPara)0)->AdderVal, (void*)&((Ptr_GrP3DoSpinTxtStpPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",			TRUE,		E_GrP3DvlParaTypeWord,						1,	(void*)&((Ptr_GrP3DoSpinTxtStpPara)0)->AddMult,NULL,	L"address adder multipler"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSpinTxtStp::Cls_GrP3DoSpinTxtStp( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoSpinTxtBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassSpinTxtStp;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaSpinTxtStp) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSpinTxtStp, sizeof(m_ParaSpinTxtStp) );

}
//--------------------------------------------------------------------
Cls_GrP3DoSpinTxtStp::~Cls_GrP3DoSpinTxtStp( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinTxtStp::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoSpinTxtBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSpinTxtStp;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSpinTxtStp);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSpinTxtStp, sizeof(m_ParaSpinTxtStp) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSpinTxtStp)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinTxtStp::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoSpinTxtBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSpinTxtStp, sizeof(m_ParaSpinTxtStp) , m_PtrScpt , E_GrP3DoClassSpinTxtStp );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSpinTxtStp::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoSpinTxtBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinTxtStp::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoSpinTxtBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSpinTxtStp, V_GrP3DoSpinTxtStpParaInfo, E_GrP3DoSpinTxtStpParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSpinTxtStp::LcValLoad( void )
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
			Tv_Adder	=	V_GrP3Mng->VmValueGet( m_ParaSpinTxtStp.AdderType, m_ParaSpinTxtStp.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaSpinTxtStp.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			switch ( m_ParaSpinTxtStp.ValType )
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
void	Cls_GrP3DoSpinTxtStp::LcValSave( void )
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
			Tv_Adder	=	V_GrP3Mng->VmValueGet( m_ParaSpinTxtStp.AdderType, m_ParaSpinTxtStp.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaSpinTxtStp.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			switch ( m_ParaSpinTxtStp.ValType )
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


