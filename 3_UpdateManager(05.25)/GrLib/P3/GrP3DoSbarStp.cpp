/*
		platform slider bar setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoSbarStp.h>
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

#define E_GrP3DoSbarStpParaCnt		6

St_GrP3DvlDoPrInfoData	V_GrP3DoSbarStpParaInfo[E_GrP3DoSbarStpParaCnt]	=	
{
	{	L"ValAdr",					TRUE,		E_GrP3DvlParaTypeAsmStpAdr,			1,	(void*)&((Ptr_GrP3DoSbarStpPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"ValType",					TRUE,		E_GrP3DvlParaTypeUsvalType,			1,	(void*)&((Ptr_GrP3DoSbarStpPara)0)->ValType,NULL,	L"Setup value type"	},
	{	L"IsAdder",					TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoSbarStpPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",				TRUE,		E_GrP3DvlParaTypeAsmMemType,		1,	(void*)&((Ptr_GrP3DoSbarStpPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",				TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoSbarStpPara)0)->AdderVal, (void*)&((Ptr_GrP3DoSbarStpPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",					TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoSbarStpPara)0)->AddMult,NULL,	L"address adder multipler"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoSbarStp::Cls_GrP3DoSbarStp( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoSbarBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId			=	E_GrP3DoClassSbarStp;
		m_ParaSize		=	m_ParaSize + sizeof(m_ParaSbarStp) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSbarStp, sizeof(m_ParaSbarStp) );

}
//--------------------------------------------------------------------
Cls_GrP3DoSbarStp::~Cls_GrP3DoSbarStp( void )
{
		//
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarStp::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoSbarBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassSbarStp;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSbarStp);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSbarStp, sizeof(m_ParaSbarStp) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSbarStp)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarStp::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoSbarBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSbarStp, sizeof(m_ParaSbarStp) , m_PtrScpt , E_GrP3DoClassSbarStp );

		LcValLoad();
		LcValDispUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoSbarStp::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoSbarBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarStp::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoSbarBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSbarStp, V_GrP3DoSbarStpParaInfo, E_GrP3DoSbarStpParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoSbarStp::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;

		if ( 0 == A_Idx )
		{
			Tv_Result	=	m_StrVal;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarStp::LcValLoad( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__s32		Tv_Add;
		void*	Tv_PtrVal;
	// code --------------------
		// get address
		Tv_Ofs	=	(__u32)m_ParaSbarStp.ValAdr;
		if ( m_ParaSbarStp.IsAdder )
		{
			Tv_Add	=	V_GrP3Mng->VmValueGet( m_ParaSbarStp.AdderType, m_ParaSbarStp.AdderVal );
			Tv_Add	=	Tv_Ofs * (__s32)((__u32)m_ParaSbarStp.AddMult);
			Tv_Ofs	=	Tv_Ofs + (__u32)Tv_Add;
		}
		Tv_PtrVal	=	V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
		// check able
		switch ( m_ParaSbarStp.ValType )
		{
			case E_GrP3AsmArgTypeSzByte:
				if ( E_GrP3MngSdaSize > Tv_Ofs )
				{
					m_Value	=	(__s32)(*((__u8*)Tv_PtrVal));
				}
				break;
			case E_GrP3AsmArgTypeSzWord:
				if ( E_GrP3MngSdaSize > (Tv_Ofs + 1) )
				{
					m_Value	=	(__s32)(*((__u16*)Tv_PtrVal));
				}
				break;
			case E_GrP3AsmArgTypeSzDword:
				if ( E_GrP3MngSdaSize > (Tv_Ofs + 3) )
				{
					m_Value	=	(__s32)(*((__u32*)Tv_PtrVal));
				}
				break;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3DoSbarStp::LcValSave( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__s32		Tv_Add;
		void*	Tv_PtrVal;
	// code --------------------
		// get address
		Tv_Ofs	=	(__u32)m_ParaSbarStp.ValAdr;
		if ( m_ParaSbarStp.IsAdder )
		{
			Tv_Add	=	V_GrP3Mng->VmValueGet( m_ParaSbarStp.AdderType, m_ParaSbarStp.AdderVal );
			Tv_Add	=	Tv_Ofs * (__s32)((__u32)m_ParaSbarStp.AddMult);
			Tv_Ofs	=	Tv_Ofs + (__u32)Tv_Add;
		}
		Tv_PtrVal	=	V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
		// check able
		switch ( m_ParaSbarStp.ValType )
		{
			case E_GrP3AsmArgTypeSzByte:
				if ( E_GrP3MngSdaSize > Tv_Ofs )
				{
					(*((__u8*)Tv_PtrVal))	=	(__u8)m_Value;
				}
				break;
			case E_GrP3AsmArgTypeSzWord:
				if ( E_GrP3MngSdaSize > (Tv_Ofs + 1) )
				{
					(*((__u16*)Tv_PtrVal))	=	(__u16)m_Value;
				}
				break;
			case E_GrP3AsmArgTypeSzDword:
				if ( E_GrP3MngSdaSize > (Tv_Ofs + 3) )
				{
					(*((__u32*)Tv_PtrVal))	=	(__u32)m_Value;
				}
				break;
		}

}
//--------------------------------------------------------------------
