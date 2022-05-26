/*
	GAUSE MFC form
 */

//====================================================================
// uses

#include <stdafx.h>
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <W32/GrMfcForm.h>
#include <GrDumyTool.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
BEGIN_MESSAGE_MAP(Cls_GrMfcForm, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()
//--------------------------------------------------------------------
Cls_GrMfcForm::Cls_GrMfcForm( Ptr_GrMfcMng A_PtrMng, UINT A_IdTmp, CWnd* A_PtrPrnt ):
CWnd()
{
	// local -------------------
	// code --------------------
		//init
		m_PtrMfcMng		=	A_PtrMng;

		GrDumyZeroMem( m_FrmTbl, sizeof(m_FrmTbl) );

}
//--------------------------------------------------------------------
Cls_GrMfcForm::~Cls_GrMfcForm()
{
}
//--------------------------------------------------------------------
Cls_GrMfcFrame*	Cls_GrMfcForm::CreateFrame( __u8 A_FrmId )
{
		return	NULL;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcForm::ViewFrame( __u8 A_FrmId, __u8 A_Mode )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check frame id
		if ( E_GrMfcFrameMaxCnt > A_FrmId )
		{
			// check view mode

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcForm::RtlFrmModeCreate( __u8 A_FrmId, __u8 A_Mode, Ptr_GrRect A_PtrRc )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check frame id
		if ( E_GrMfcFrameMaxCnt > A_FrmId )
		{
			// check already exist
			if ( NULL == m_FrmTbl[A_FrmId] )
			{
				// create frame

			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------


