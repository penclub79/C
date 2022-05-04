/*
		platform radio button setup mask

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoRbtnSsm.h>
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

#define E_GrP4DoRbtnSsmParaCnt		6

St_GrP4DvlDoPrInfoData	V_GrP4DoRbtnSsmParaInfo[E_GrP4DoRbtnSsmParaCnt]	=	
{
	{	L"ValAdr",			TRUE,		E_GrP4DvlParaTypeAsmStpAdr,				1,	(void*)&((Ptr_GrP4DoRbtnSsmPara)0)->ValAdr,NULL,	L"Setup value address"	},
	{	L"IsAdder",			TRUE,		E_GrP4DvlParaTypeBool8,						1,	(void*)&((Ptr_GrP4DoRbtnSsmPara)0)->IsAdder,NULL,	L"address adder use"	},
	{	L"AdderType",		TRUE,		E_GrP4DvlParaTypeAsmAddType,			1,	(void*)&((Ptr_GrP4DoRbtnSsmPara)0)->AdderType,NULL,	L"address adder type"	},
	{	L"AdderVal",		TRUE,		E_GrP4DvlParaTypeDword,						1,	(void*)&((Ptr_GrP4DoRbtnSsmPara)0)->AdderVal, (void*)&((Ptr_GrP4DoRbtnSsmPara)0)->AdderType,	L"address adder value"	},
	{	L"AddMult",			TRUE,		E_GrP4DvlParaTypeWord,						1,	(void*)&((Ptr_GrP4DoRbtnSsmPara)0)->AddMult,NULL,	L"address adder multiplier"	},
	{	L"BitPos",			TRUE,		E_GrP4DvlParaTypeByte,						1,	(void*)&((Ptr_GrP4DoRbtnSsmPara)0)->BitPos,NULL,	L"value bit position"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoRbtnSsm::Cls_GrP4DoRbtnSsm(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoRbtnBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassRbtnSsm;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaRbtnSsm) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaRbtnSsm, sizeof(m_ParaRbtnSsm) );

		m_PtrStp	=	(__u8*)V_GrP4Mng->SetupDataPtrGet( 0 );

}
//--------------------------------------------------------------------
Cls_GrP4DoRbtnSsm::~Cls_GrP4DoRbtnSsm( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoRbtnSsm::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoRbtnBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassRbtnSsm;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaRbtnSsm);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaRbtnSsm, sizeof(m_ParaRbtnSsm) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaRbtnSsm)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoRbtnSsm::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoRbtnBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaRbtnSsm, sizeof(m_ParaRbtnSsm) , m_PtrScpt , E_GrP4DoClassRbtnSsm );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoRbtnSsm::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoRbtnBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoRbtnSsm::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoRbtnBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaRbtnSsm, V_GrP4DoRbtnSsmParaInfo, E_GrP4DoRbtnSsmParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoRbtnSsm::ProcEvtTimeTick( __u32 A_Tick )
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
			Tv_Adder	=	V_GrP4Mng->SysValueGet( m_ParaRbtnSsm.AdderType, m_ParaRbtnSsm.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaRbtnSsm.AddMult;
		}

		Tv_BitPos	=	(__u32)m_ParaRbtnSsm.BitPos + Tv_Adder;
		Tv_Ofs		=	Tv_Ofs + (Tv_BitPos >> 3);
		Tv_BitPos	=	Tv_BitPos & 0x07;

		if ( E_GrP4MngSdaSize > Tv_Ofs )
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
void	Cls_GrP4DoRbtnSsm::LcDoSelect( void )
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
			Tv_Adder	=	V_GrP4Mng->SysValueGet( m_ParaRbtnSsm.AdderType, m_ParaRbtnSsm.AdderVal );
			Tv_Adder	=	Tv_Adder * (__u32)m_ParaRbtnSsm.AddMult;
		}

		Tv_BitPos	=	(__u32)m_ParaRbtnSsm.BitPos + Tv_Adder;
		Tv_Ofs		=	Tv_Ofs + (Tv_BitPos >> 3);
		Tv_BitPos	=	Tv_BitPos & 0x07;

		if ( E_GrP4MngSdaSize > Tv_Ofs )
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
		Cls_GrP4DoRbtnBase::LcDoSelect();

}
//--------------------------------------------------------------------

