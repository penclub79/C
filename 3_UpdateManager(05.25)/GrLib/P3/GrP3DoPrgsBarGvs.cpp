/*
		platform prograss bar variable

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoPrgsBarGvs.h>
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

#define E_GrP3DoPrgsBarGvsParaCnt		6

St_GrP3DvlDoPrInfoData	V_GrP3DoPrgsBarGvsParaInfo[E_GrP3DoPrgsBarGvsParaCnt]	=	
{
	{	L"ValAdr",					TRUE,		E_GrP3DvlParaTypeAsmGvsAdr,			1,	(void*)&((Ptr_GrP3DoPrgsBarGvsPara)0)->ValAdr,NULL,	L"GVS value address"	},
	{	L"ValType",					TRUE,		E_GrP3DvlParaTypeUsvalType,			1,	(void*)&((Ptr_GrP3DoPrgsBarGvsPara)0)->ValType,NULL,	L"GVS value type"	},
	{	L"IsAdder",					TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoPrgsBarGvsPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",				TRUE,		E_GrP3DvlParaTypeAsmMemType,		1,	(void*)&((Ptr_GrP3DoPrgsBarGvsPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",				TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoPrgsBarGvsPara)0)->AdderVal, (void*)&((Ptr_GrP3DoPrgsBarGvsPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",					TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoPrgsBarGvsPara)0)->AddMult,NULL,	L"address adder multipler"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoPrgsBarGvs::Cls_GrP3DoPrgsBarGvs( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoPrgsBarBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassPrgsBarGvs;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaPrgsBarGvs) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaPrgsBarGvs, sizeof(m_ParaPrgsBarGvs) );

		m_PtrGvs		=	V_GrP3Mng->GvsPtrGet();

}
//--------------------------------------------------------------------
Cls_GrP3DoPrgsBarGvs::~Cls_GrP3DoPrgsBarGvs( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPrgsBarGvs::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP3DoPrgsBarBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassPrgsBarGvs;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaPrgsBarGvs);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaPrgsBarGvs, sizeof(m_ParaPrgsBarGvs) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaPrgsBarGvs)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPrgsBarGvs::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoPrgsBarBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaPrgsBarGvs, sizeof(m_ParaPrgsBarGvs) , m_PtrScpt , E_GrP3DoClassPrgsBarGvs );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoPrgsBarGvs::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoPrgsBarBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPrgsBarGvs::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoPrgsBarBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaPrgsBarGvs, V_GrP3DoPrgsBarGvsParaInfo, E_GrP3DoPrgsBarGvsParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoPrgsBarGvs::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		__s32		Tv_Val;
	// code --------------------
#ifdef GR_P3_EDITOR
		if ( !((Cls_GrP3DvlMng*)V_GrP3Mng)->DvlIsDbgRun() )
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
void	Cls_GrP3DoPrgsBarGvs::LcValLoad( void )
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
			Tv_Add	=	V_GrP3Mng->VmValueGet( m_ParaPrgsBarGvs.AdderType, m_ParaPrgsBarGvs.AdderVal );
			Tv_Add	=	Tv_Ofs * (__s32)((__u32)m_ParaPrgsBarGvs.AddMult);
			Tv_Ofs	=	Tv_Ofs + (__u32)Tv_Add;
		}

		Tv_PtrVal	=	(void*)( (__u32)m_PtrGvs + Tv_Ofs );
		// check able
		switch ( m_ParaPrgsBarGvs.ValType )
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
void	Cls_GrP3DoPrgsBarGvs::LcValSave( void )
{
	// local -------------------
	// code --------------------
		// read only
}
//--------------------------------------------------------------------
