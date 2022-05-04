/*
		platform radio button setup mask

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoRbtnSsm.h>
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

#define E_GrP3DoRbtnSsmParaCnt		6

St_GrP3DvlDoPrInfoData	V_GrP3DoRbtnSsmParaInfo[E_GrP3DoRbtnSsmParaCnt]	=	
{
	{	L"ValAdr",			TRUE,		E_GrP3DvlParaTypeAsmStpAdr,				1,	(void*)&((Ptr_GrP3DoRbtnSsmPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"IsAdder",			TRUE,		E_GrP3DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP3DoRbtnSsmPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",		TRUE,		E_GrP3DvlParaTypeAsmMemType,			1,	(void*)&((Ptr_GrP3DoRbtnSsmPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",		TRUE,		E_GrP3DvlParaTypeDword,						1,	(void*)&((Ptr_GrP3DoRbtnSsmPara)0)->AdderVal, (void*)&((Ptr_GrP3DoRbtnSsmPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",			TRUE,		E_GrP3DvlParaTypeWord,						1,	(void*)&((Ptr_GrP3DoRbtnSsmPara)0)->AddMult,NULL,	L"address adder multiplier"	},
	{	L"BitPos",			TRUE,		E_GrP3DvlParaTypeByte,						1,	(void*)&((Ptr_GrP3DoRbtnSsmPara)0)->BitPos,NULL,	L"value bit position"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoRbtnSsm::Cls_GrP3DoRbtnSsm( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoRbtnBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassRbtnSsm;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaRbtnSsm) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaRbtnSsm, sizeof(m_ParaRbtnSsm) );

		m_PtrStp	=	(__u8*)V_GrP3Mng->SetupDataPtrGet( 0 );

}
//--------------------------------------------------------------------
Cls_GrP3DoRbtnSsm::~Cls_GrP3DoRbtnSsm( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnSsm::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoRbtnBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassRbtnSsm;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaRbtnSsm);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaRbtnSsm, sizeof(m_ParaRbtnSsm) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaRbtnSsm)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnSsm::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoRbtnBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaRbtnSsm, sizeof(m_ParaRbtnSsm) , m_PtrScpt , E_GrP3DoClassRbtnSsm );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoRbtnSsm::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP3DoRbtnBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnSsm::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoRbtnBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaRbtnSsm, V_GrP3DoRbtnSsmParaInfo, E_GrP3DoRbtnSsmParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoRbtnSsm::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		BOOL8	Tv_IsMatch;
		__s32		Tv_Val;
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		__u32	Tv_BitPos;
		__u8	Tv_Mask;
	// code --------------------
		// init
		Tv_IsMatch	=	FALSE;
		Tv_Val			=	0;
		// get address
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaRbtnSsm.ValAdr;
		if ( m_ParaRbtnSsm.IsAdder )
		{
			Tv_Adder	=	V_GrP3Mng->VmValueGet( m_ParaRbtnSsm.AdderType, m_ParaRbtnSsm.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaRbtnSsm.AddMult;
		}

		Tv_BitPos	=	(__u32)m_ParaRbtnSsm.BitPos + Tv_Adder;
		Tv_Ofs		=	Tv_Ofs + (Tv_BitPos >> 3);
		Tv_BitPos	=	Tv_BitPos & 0x07;

		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_Mask	=	1 << Tv_BitPos;
			if ( 0 != (Tv_Mask & m_PtrStp[Tv_Ofs]) )
			{
				Tv_Val	=	1;
			}
		}

		if ( m_ParaRbtnBase.Value == Tv_Val )
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
void	Cls_GrP3DoRbtnSsm::LcDoSelect( void )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u32	Tv_Adder;
		__u32	Tv_BitPos;
		__u8	Tv_Mask;
	// code --------------------
		// get address
		Tv_Adder	=	0;
		Tv_Ofs		=	(__u32)m_ParaRbtnSsm.ValAdr;
		if ( m_ParaRbtnSsm.IsAdder )
		{
			Tv_Adder	=	V_GrP3Mng->VmValueGet( m_ParaRbtnSsm.AdderType, m_ParaRbtnSsm.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaRbtnSsm.AddMult;
		}

		Tv_BitPos	=	(__u32)m_ParaRbtnSsm.BitPos + Tv_Adder;
		Tv_Ofs		=	Tv_Ofs + (Tv_BitPos >> 3);
		Tv_BitPos	=	Tv_BitPos & 0x07;

		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_Mask	=	1 << Tv_BitPos;
			if ( 0 == m_ParaRbtnBase.Value )
			{
				m_PtrStp[Tv_Ofs]	=	m_PtrStp[Tv_Ofs] & (~Tv_Mask);
			}
			else
			{
				m_PtrStp[Tv_Ofs]	=	m_PtrStp[Tv_Ofs] | Tv_Mask;
			}
		}
		
		// inherited
		Cls_GrP3DoRbtnBase::LcDoSelect();

}
//--------------------------------------------------------------------

