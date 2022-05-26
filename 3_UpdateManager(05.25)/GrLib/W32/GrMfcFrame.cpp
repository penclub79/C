/*
	GAUSE MFC frame
 */

//====================================================================
// uses

#include  <stdafx.h>
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<W32/GrMfcFrame.h>
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
BEGIN_MESSAGE_MAP(Cls_GrMfcFrame, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()
//--------------------------------------------------------------------
Cls_GrMfcFrame::Cls_GrMfcFrame( Ptr_GrMfcMng A_PtrMng, UINT A_IdTmp, CWnd* A_PtrPrnt ):
CDialogEx( A_IdTmp, A_PtrPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_PtrMfcMng	=	A_PtrMng;
}
//--------------------------------------------------------------------
Cls_GrMfcFrame::~Cls_GrMfcFrame()
{
}
//--------------------------------------------------------------------
void Cls_GrMfcFrame::DoDataExchange(CDataExchange* pDX)
{
	// local -------------------
	// code --------------------
		CDialogEx::DoDataExchange(pDX);
}
//--------------------------------------------------------------------
void Cls_GrMfcFrame::OnPaint()
{
	// local -------------------
	// code --------------------
		CDialogEx::OnPaint();
}
//--------------------------------------------------------------------
BOOL8 Cls_GrMfcFrame::OnInitDialog()
{
	// local -------------------
	// code --------------------


		return	TRUE;
}
//--------------------------------------------------------------------

