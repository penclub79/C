/*
		platform slider bar setup

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSbarStp.h>
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

#define E_GrP4DoSbarStpParaCnt		8

St_GrP4DvlDoPrInfoData	V_GrP4DoSbarStpParaInfo[E_GrP4DoSbarStpParaCnt]	=	
{
	{	L"ValAdr",					TRUE,		E_GrP4DvlParaTypeAsmStpAdr,			1,	(void*)&((Ptr_GrP4DoSbarStpPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"ValType",					TRUE,		E_GrP4DvlParaTypeUsvalType,			1,	(void*)&((Ptr_GrP4DoSbarStpPara)0)->ValType,NULL,	L"Setup value type"	},
	{	L"IsAdder",					TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSbarStpPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",				TRUE,		E_GrP4DvlParaTypeAsmAddType,		1,	(void*)&((Ptr_GrP4DoSbarStpPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",				TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4DoSbarStpPara)0)->AdderVal, (void*)&((Ptr_GrP4DoSbarStpPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",					TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoSbarStpPara)0)->AddMult,NULL,	L"address adder multipler"	},
	{ L"Value Min", TRUE, E_GrP4DvlParaTypeInt, 1, (void*)&((Ptr_GrP4DoSbarStpPara)0)->ValMin, NULL, L"Value range minimal" },
	{ L"Value Max", TRUE, E_GrP4DvlParaTypeInt, 1, (void*)&((Ptr_GrP4DoSbarStpPara)0)->ValMax, NULL, L"Value range maximal" },

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSbarStp::Cls_GrP4DoSbarStp(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoSbarBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId			=	E_GrP4DoClassSbarStp;
		m_ParaSize		=	m_ParaSize + sizeof(m_ParaSbarStp) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaSbarStp, sizeof(m_ParaSbarStp) );

		m_ParaSbarStp.ValMax	=	100;
}
//--------------------------------------------------------------------
Cls_GrP4DoSbarStp::~Cls_GrP4DoSbarStp( void )
{
		//
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSbarStp::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoSbarBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSbarStp;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaSbarStp);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaSbarStp, sizeof(m_ParaSbarStp) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaSbarStp)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarStp::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoSbarBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaSbarStp, sizeof(m_ParaSbarStp) , m_PtrScpt , E_GrP4DoClassSbarStp );

		// update min max
		m_ValMin		=	m_ParaSbarStp.ValMin;
		m_ValMax		=	m_ParaSbarStp.ValMax;

		LcValLoad();
		LcValDispUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSbarStp::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoSbarBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSbarStp::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoSbarBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaSbarStp, V_GrP4DoSbarStpParaInfo, E_GrP4DoSbarStpParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoSbarStp::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
void	Cls_GrP4DoSbarStp::LcValLoad( void )
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
			Tv_Add	=	V_GrP4Mng->SysValueGet( m_ParaSbarStp.AdderType, m_ParaSbarStp.AdderVal );
			Tv_Add	=	Tv_Ofs * (__s32)((__u32)m_ParaSbarStp.AddMult);
			Tv_Ofs	=	Tv_Ofs + (__u32)Tv_Add;
		}
		Tv_PtrVal	=	V_GrP4Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
		// check able
		switch ( m_ParaSbarStp.ValType )
		{
			case E_GrP4AsmArgTypeSzByte:
				if ( E_GrP4MngSdaSize > Tv_Ofs )
				{
					m_Value	=	(__s32)(*((__u8*)Tv_PtrVal));
				}
				break;
			case E_GrP4AsmArgTypeSzWord:
				if ( E_GrP4MngSdaSize > (Tv_Ofs + 1) )
				{
					m_Value	=	(__s32)(*((__u16*)Tv_PtrVal));
				}
				break;
			case E_GrP4AsmArgTypeSzDword:
				if ( E_GrP4MngSdaSize > (Tv_Ofs + 3) )
				{
					m_Value	=	(__s32)(*((__u32*)Tv_PtrVal));
				}
				break;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DoSbarStp::LcValSave( void )
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
			Tv_Add	=	V_GrP4Mng->SysValueGet( m_ParaSbarStp.AdderType, m_ParaSbarStp.AdderVal );
			Tv_Add	=	Tv_Ofs * (__s32)((__u32)m_ParaSbarStp.AddMult);
			Tv_Ofs	=	Tv_Ofs + (__u32)Tv_Add;
		}
		Tv_PtrVal	=	V_GrP4Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
		// check able
		switch ( m_ParaSbarStp.ValType )
		{
			case E_GrP4AsmArgTypeSzByte:
				if ( E_GrP4MngSdaSize > Tv_Ofs )
				{
					(*((__u8*)Tv_PtrVal))	=	(__u8)m_Value;
				}
				break;
			case E_GrP4AsmArgTypeSzWord:
				if ( E_GrP4MngSdaSize > (Tv_Ofs + 1) )
				{
					(*((__u16*)Tv_PtrVal))	=	(__u16)m_Value;
				}
				break;
			case E_GrP4AsmArgTypeSzDword:
				if ( E_GrP4MngSdaSize > (Tv_Ofs + 3) )
				{
					(*((__u32*)Tv_PtrVal))	=	(__u32)m_Value;
				}
				break;
		}

}
//--------------------------------------------------------------------
