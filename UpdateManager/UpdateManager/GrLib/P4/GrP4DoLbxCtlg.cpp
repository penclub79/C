/*
		platform list box base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoLbxCtlg.h>
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

#define E_GrP4DoLbxCtlgParaCnt		5

St_GrP4DvlDoPrInfoData	V_GrP4DoLbxCtlgParaInfo[E_GrP4DoLbxCtlgParaCnt]	=	
{
	{	L"ElemCode0",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoLbxCtlgPara)0)->ElemCode[0],NULL,	L"element code 0"	},
	{	L"ElemCode1",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoLbxCtlgPara)0)->ElemCode[1],NULL,	L"element code 1"	},
	{	L"ElemCode2",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoLbxCtlgPara)0)->ElemCode[2],NULL,	L"element code 2"	},
	{	L"ElemCode3",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoLbxCtlgPara)0)->ElemCode[3],NULL,	L"element code 3"	},
	{	L"PageSize",				TRUE,		E_GrP4DvlParaTypeDword,					1,	(void*)&((Ptr_GrP4DoLbxCtlgPara)0)->PageSize,		NULL,	L"request page size, 0=system default"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoLbxCtlg::Cls_GrP4DoLbxCtlg(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoLbxBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassLbxCtlg;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaLbxCtlg) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaLbxCtlg, sizeof(m_ParaLbxCtlg) );

		m_CtlgId			=	0;

		GrDumyZeroMem( m_PntStrTbl, sizeof(m_PntStrTbl) );
}
//--------------------------------------------------------------------
Cls_GrP4DoLbxCtlg::~Cls_GrP4DoLbxCtlg( void )
{
		//update modal result
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoLbxCtlg::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoLbxBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassLbxCtlg;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaLbxCtlg);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaLbxCtlg, sizeof(m_ParaLbxCtlg) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaLbxCtlg)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoLbxCtlg::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoLbxBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaLbxCtlg, sizeof(m_ParaLbxCtlg) , m_PtrScpt , E_GrP4DoClassLbxCtlg );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoLbxCtlg::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiValueGet:
				A_PtrInt[0]	=	0;
				if ( (0 != m_ItemCnt) && (0 <= m_SelIdx) )
				{
					// get handle
					A_PtrInt[0]	=	(__s32)GrP4MngMakeCtlgHndl( m_CtlgId, (__u32)m_SelIdx );
				}
				break;
			case E_GrP4UpiCtlgLoad:
				m_CtlgId		=	(__u8)A_PtrInt[1];
				m_ItemCnt		=	0;
				m_ItemTotal	=	0;
				m_PageCnt		=	0;
				m_PagePos		=	0;
				m_SelIdx		=	-1;
				m_CusIdx		=	0;
				A_PtrInt[1]	=	(__s32)m_ParaLbxCtlg.PageSize;
				A_PtrInt[0]	=	(__s32)V_GrP4Mng->CtlgLoad( m_CtlgId, &A_PtrInt[1], &m_ItemCnt, &m_ItemTotal, &m_PageCnt );
				break;
			case E_GrP4UpiPageNext:
				A_PtrInt[0]	=	FALSE;
				if ( E_GrP4CtlgIdNone != m_CtlgId )
				{
					A_PtrInt[0]	=	(__s32)V_GrP4Mng->CtlgPageNext( m_CtlgId, &m_ItemCnt, &m_ItemTotal, &m_PageCnt, &m_PagePos );
				}
				break;
			case E_GrP4UpiPagePrev:
				A_PtrInt[0]	=	FALSE;
				if ( E_GrP4CtlgIdNone != m_CtlgId )
				{
					A_PtrInt[0]	=	(__s32)V_GrP4Mng->CtlgPagePrev( m_CtlgId, &m_ItemCnt, &m_ItemTotal, &m_PageCnt, &m_PagePos );
				}
				break;
			default:
				Tv_Result	=	Cls_GrP4DoLbxBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoLbxCtlg::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoLbxBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaLbxCtlg, V_GrP4DoLbxCtlgParaInfo, E_GrP4DoLbxCtlgParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoLbxCtlg::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u32	Tv_Hndl;
		__u8	Tv_Code;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			if ( m_IsPntText )
			{
				// get code
				Tv_Code			=	m_ParaLbxCtlg.ElemCode[m_PntElemIdx];
				// element value get
				Tv_Hndl			=	GrP4MngMakeCtlgHndl( m_CtlgId, (__u32)m_PntItemIdx );
				// check display type
				if ( E_GrP4DoLbxElemDispText == m_ParaLbxBase.ElemView[m_PntElemIdx] )
				{
					if ( V_GrP4Mng->CtlgItemStrGet( Tv_Hndl, Tv_Code, m_ParaLbxBase.ElemValAdd[m_PntElemIdx], m_PntStrTbl[m_PntViewIdx][m_PntElemIdx] ) )
					{
						Tv_Result	=	m_PntStrTbl[m_PntViewIdx][m_PntElemIdx];
					}
				}
			}

		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoLbxCtlg::LcPntElemValGet( __s32* A_PtrRtVal )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Hndl;
		__u8	Tv_Code;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP4CtlgIdNone != m_CtlgId )
		{
			Tv_Hndl	=	GrP4MngMakeCtlgHndl( m_CtlgId, m_PntItemIdx );
			Tv_Code	=	m_ParaLbxCtlg.ElemCode[m_PntElemIdx];
			Tv_Result	=	V_GrP4Mng->CtlgItemValueGet( Tv_Hndl, Tv_Code, m_ParaLbxBase.ElemValAdd[m_PntElemIdx], A_PtrRtVal );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

