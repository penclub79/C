/*
		platform prograss bar variable

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoPrgsBarGvs.h>
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

#define E_GrP4DoPrgsBarGvsParaCnt		6

St_GrP4DvlDoPrInfoData	V_GrP4DoPrgsBarGvsParaInfo[E_GrP4DoPrgsBarGvsParaCnt]	=	
{
	{	L"ValAdr",					TRUE,		E_GrP4DvlParaTypeAsmGvsAdr,			1,	(void*)&((Ptr_GrP4DoPrgsBarGvsPara)0)->ValAdr,NULL,	L"GVS value address"	},
	{	L"ValType",					TRUE,		E_GrP4DvlParaTypeUsvalType,			1,	(void*)&((Ptr_GrP4DoPrgsBarGvsPara)0)->ValType,NULL,	L"GVS value type"	},
	{	L"IsAdder",					TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoPrgsBarGvsPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",				TRUE,		E_GrP4DvlParaTypeAsmAddType,		1,	(void*)&((Ptr_GrP4DoPrgsBarGvsPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",				TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4DoPrgsBarGvsPara)0)->AdderVal, (void*)&((Ptr_GrP4DoPrgsBarGvsPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",					TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoPrgsBarGvsPara)0)->AddMult,NULL,	L"address adder multipler"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoPrgsBarGvs::Cls_GrP4DoPrgsBarGvs(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoPrgsBarBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassPrgsBarGvs;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaPrgsBarGvs) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaPrgsBarGvs, sizeof(m_ParaPrgsBarGvs) );

		m_PtrGvs		=	V_GrP4Mng->GvsPtrGet();

}
//--------------------------------------------------------------------
Cls_GrP4DoPrgsBarGvs::~Cls_GrP4DoPrgsBarGvs( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoPrgsBarGvs::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoPrgsBarBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassPrgsBarGvs;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaPrgsBarGvs);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaPrgsBarGvs, sizeof(m_ParaPrgsBarGvs) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaPrgsBarGvs)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoPrgsBarGvs::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoPrgsBarBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaPrgsBarGvs, sizeof(m_ParaPrgsBarGvs) , m_PtrScpt , E_GrP4DoClassPrgsBarGvs );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoPrgsBarGvs::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoPrgsBarBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoPrgsBarGvs::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoPrgsBarBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaPrgsBarGvs, V_GrP4DoPrgsBarGvsParaInfo, E_GrP4DoPrgsBarGvsParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoPrgsBarGvs::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		__s32		Tv_Val;
	// code --------------------
#ifdef GR_P4_EDITOR
		if ( !((Cls_GrP4DvlMng*)V_GrP4Mng)->DvlIsDbgRun() )
		{
			return;
		}
#endif
		Tv_Val	=	m_Value;
		LcValLoad();
		if ( m_Value != Tv_Val )
		{
			LcPrgsStrUpdt();
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPrgsBarGvs::LcValLoad( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__s32		Tv_Add;
		void*	Tv_PtrVal;
	// code --------------------

		// get address
		Tv_Ofs	=	(__u32)m_ParaPrgsBarGvs.ValAdr;
		if ( m_ParaPrgsBarGvs.IsAdder )
		{
			Tv_Add	=	V_GrP4Mng->SysValueGet( m_ParaPrgsBarGvs.AdderType, m_ParaPrgsBarGvs.AdderVal );
			Tv_Add	=	Tv_Ofs * (__s32)((__u32)m_ParaPrgsBarGvs.AddMult);
			Tv_Ofs	=	Tv_Ofs + (__u32)Tv_Add;
		}

		Tv_PtrVal	=	(void*)( (__u32)m_PtrGvs + Tv_Ofs );
		// check able
		switch ( m_ParaPrgsBarGvs.ValType )
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
void	Cls_GrP4DoPrgsBarGvs::LcValSave( void )
{
	// local -------------------
	// code --------------------
		// read only
}
//--------------------------------------------------------------------
