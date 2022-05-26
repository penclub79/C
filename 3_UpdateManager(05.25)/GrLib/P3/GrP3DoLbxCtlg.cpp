/*
		platform list box base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoLbxCtlg.h>
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

#define E_GrP3DoLbxCtlgParaCnt		5

St_GrP3DvlDoPrInfoData	V_GrP3DoLbxCtlgParaInfo[E_GrP3DoLbxCtlgParaCnt]	=	
{
	{	L"ElemCode0",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoLbxCtlgPara)0)->ElemCode[0],NULL,	L"element code 0"	},
	{	L"ElemCode1",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoLbxCtlgPara)0)->ElemCode[1],NULL,	L"element code 1"	},
	{	L"ElemCode2",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoLbxCtlgPara)0)->ElemCode[2],NULL,	L"element code 2"	},
	{	L"ElemCode3",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoLbxCtlgPara)0)->ElemCode[3],NULL,	L"element code 3"	},
	{	L"PageSize",				TRUE,		E_GrP3DvlParaTypeDword,					1,	(void*)&((Ptr_GrP3DoLbxCtlgPara)0)->PageSize,		NULL,	L"request page size, 0=system default"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoLbxCtlg::Cls_GrP3DoLbxCtlg( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoLbxBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassLbxCtlg;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaLbxCtlg) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaLbxCtlg, sizeof(m_ParaLbxCtlg) );

		m_CtlgId			=	0;

		GrDumyZeroMem( m_PntStrTbl, sizeof(m_PntStrTbl) );
}
//--------------------------------------------------------------------
Cls_GrP3DoLbxCtlg::~Cls_GrP3DoLbxCtlg( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxCtlg::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoLbxBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassLbxCtlg;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaLbxCtlg);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaLbxCtlg, sizeof(m_ParaLbxCtlg) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaLbxCtlg)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxCtlg::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoLbxBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaLbxCtlg, sizeof(m_ParaLbxCtlg) , m_PtrScpt , E_GrP3DoClassLbxCtlg );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoLbxCtlg::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiValueGet:
				A_PtrInt[0]	=	0;
				if ( (0 != m_ItemCnt) && (0 <= m_SelIdx) )
				{
					// get handle
					A_PtrInt[0]	=	(__s32)GrP3MngMakeCtlgHndl( m_CtlgId, (__u32)m_SelIdx );
				}
				break;
			case E_GrP3UpiCtlgLoad:
				m_CtlgId		=	(__u8)A_PtrInt[1];
				m_ItemCnt		=	0;
				m_ItemTotal	=	0;
				m_PageCnt		=	0;
				m_PagePos		=	0;
				m_SelIdx		=	-1;
				m_CusIdx		=	0;
				A_PtrInt[1]	=	(__s32)m_ParaLbxCtlg.PageSize;
				A_PtrInt[0]	=	(__s32)V_GrP3Mng->CtlgLoad( m_CtlgId, &A_PtrInt[1], &m_ItemCnt, &m_ItemTotal, &m_PageCnt );
				break;
			case E_GrP3UpiPageNext:
				A_PtrInt[0]	=	FALSE;
				if ( E_GrP3CtlgIdNone != m_CtlgId )
				{
					A_PtrInt[0]	=	(__s32)V_GrP3Mng->CtlgPageNext( m_CtlgId, &m_ItemCnt, &m_ItemTotal, &m_PageCnt, &m_PagePos );
				}
				break;
			case E_GrP3UpiPagePrev:
				A_PtrInt[0]	=	FALSE;
				if ( E_GrP3CtlgIdNone != m_CtlgId )
				{
					A_PtrInt[0]	=	(__s32)V_GrP3Mng->CtlgPagePrev( m_CtlgId, &m_ItemCnt, &m_ItemTotal, &m_PageCnt, &m_PagePos );
				}
				break;
			default:
				Tv_Result	=	Cls_GrP3DoLbxBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxCtlg::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoLbxBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaLbxCtlg, V_GrP3DoLbxCtlgParaInfo, E_GrP3DoLbxCtlgParaCnt );
#endif
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoLbxCtlg::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
				Tv_Hndl			=	GrP3MngMakeCtlgHndl( m_CtlgId, (__u32)m_PntItemIdx );
				// check display type
				if ( E_GrP3DoLbxElemDispText == m_ParaLbxBase.ElemView[m_PntElemIdx] )
				{
					if ( V_GrP3Mng->CtlgItemStrGet( Tv_Hndl, Tv_Code, m_ParaLbxBase.ElemValAdd[m_PntElemIdx], m_PntStrTbl[m_PntViewIdx][m_PntElemIdx] ) )
					{
						Tv_Result	=	m_PntStrTbl[m_PntViewIdx][m_PntElemIdx];
					}
				}
			}

		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoLbxCtlg::LcPntElemValGet( __s32* A_PtrRtVal )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Hndl;
		__u8	Tv_Code;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP3CtlgIdNone != m_CtlgId )
		{
			Tv_Hndl	=	GrP3MngMakeCtlgHndl( m_CtlgId, m_PntItemIdx );
			Tv_Code	=	m_ParaLbxCtlg.ElemCode[m_PntElemIdx];
			Tv_Result	=	V_GrP3Mng->CtlgItemValueGet( Tv_Hndl, Tv_Code, m_ParaLbxBase.ElemValAdd[m_PntElemIdx], A_PtrRtVal );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

