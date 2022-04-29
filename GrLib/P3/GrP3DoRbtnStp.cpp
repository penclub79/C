/*
		platform radio button setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoRbtnStp.h>
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

#define E_GrP3DoRbtnStpParaCnt		6

St_GrP3DvlDoPrInfoData	V_GrP3DoRbtnStpParaInfo[E_GrP3DoRbtnStpParaCnt]	=	
{
	{	L"ValAdr",			TRUE,		E_GrP3DvlParaTypeAsmStpAdr,				1,	(void*)&((Ptr_GrP3DoRbtnStpPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"ValType",			TRUE,		E_GrP3DvlParaTypeUsvalType,				1,	(void*)&((Ptr_GrP3DoRbtnStpPara)0)->ValType,NULL,	L"Setup value type"	},
	{	L"IsAdder",			TRUE,		E_GrP3DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP3DoRbtnStpPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",		TRUE,		E_GrP3DvlParaTypeAsmMemType,			1,	(void*)&((Ptr_GrP3DoRbtnStpPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",		TRUE,		E_GrP3DvlParaTypeDword,						1,	(void*)&((Ptr_GrP3DoRbtnStpPara)0)->AdderVal, (void*)&((Ptr_GrP3DoRbtnStpPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",			TRUE,		E_GrP3DvlParaTypeWord,						1,	(void*)&((Ptr_GrP3DoRbtnStpPara)0)->AddMult,NULL,	L"address adder multipler"	},
};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoRbtnStp::Cls_GrP3DoRbtnStp( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoRbtnBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassRbtnStp;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaRbtnStp) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaRbtnStp, sizeof(m_ParaRbtnStp) );

}
//--------------------------------------------------------------------
Cls_GrP3DoRbtnStp::~Cls_GrP3DoRbtnStp( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnStp::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoRbtnBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassRbtnStp;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaRbtnStp);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaRbtnStp, sizeof(m_ParaRbtnStp) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaRbtnStp)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnStp::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoRbtnBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaRbtnStp, sizeof(m_ParaRbtnStp) , m_PtrScpt , E_GrP3DoClassRbtnStp );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoRbtnStp::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoRbtnBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnStp::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoRbtnBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaRbtnStp, V_GrP3DoRbtnStpParaInfo, E_GrP3DoRbtnStpParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnStp::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		void*	Tv_PtrVal;
		__s32		Tv_Val;
		BOOL8	Tv_IsMatch;
	// code --------------------
		Tv_Val		=	0;
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaRbtnStp.ValAdr;
		if ( m_ParaRbtnStp.IsAdder )
		{
			Tv_Adder	=	V_GrP3Mng->VmValueGet( m_ParaRbtnStp.AdderType, m_ParaRbtnStp.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaRbtnStp.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			switch ( m_ParaRbtnStp.ValType )
			{
				case E_GrP3AsmArgTypeSzByte:
					Tv_Val	=	(__s32)(*((__u8*)Tv_PtrVal));
					break;
				case E_GrP3AsmArgTypeSzWord:
					if ( E_GrP3MngSdaSize > (Tv_Ofs + 1) )
					{
						Tv_Val	=	(__s32)(*((__u16*)Tv_PtrVal));
					}
					break;
				case E_GrP3AsmArgTypeSzDword:
					if ( E_GrP3MngSdaSize > (Tv_Ofs + 3) )
					{
						Tv_Val	=	(__s32)(*((__u32*)Tv_PtrVal));
					}
					break;
			}
		}
		// check match
		Tv_IsMatch	=	FALSE;
		if ( Tv_Val == m_ParaRbtnBase.Value )
		{
			Tv_IsMatch	=	TRUE;
		}
		// check need repaint
		if ( m_IsVal != Tv_IsMatch )
		{
			m_IsVal	=	Tv_IsMatch;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnStp::LcDoSelect( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		void*	Tv_PtrVal;
	// code --------------------
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaRbtnStp.ValAdr;
		if ( m_ParaRbtnStp.IsAdder )
		{
			Tv_Adder	=	V_GrP3Mng->VmValueGet( m_ParaRbtnStp.AdderType, m_ParaRbtnStp.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaRbtnStp.AddMult;
		}
		Tv_Ofs	=	Tv_Ofs + Tv_Adder;
		// check range
		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal	=	V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			switch ( m_ParaRbtnStp.ValType )
			{
			case E_GrP3AsmArgTypeSzByte:
				*((__u8*)Tv_PtrVal)	=	(__u8)m_ParaRbtnBase.Value;
				break;
			case E_GrP3AsmArgTypeSzWord:
				if ( E_GrP3MngSdaSize > (Tv_Ofs + 1) )
				{
					*((__u16*)Tv_PtrVal)	=	(__u16)m_ParaRbtnBase.Value;
				}
				break;
			case E_GrP3AsmArgTypeSzDword:
				if ( E_GrP3MngSdaSize > (Tv_Ofs + 3) )
				{
					*((__u32*)Tv_PtrVal)	=	(__u32)m_ParaRbtnBase.Value;
				}
				break;
			}
		}
		
		// inherited
		Cls_GrP3DoRbtnBase::LcDoSelect();

}
//--------------------------------------------------------------------

