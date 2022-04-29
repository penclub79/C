/*
	GAUSE MFC Main form
 */

//====================================================================
// uses

#include  <stdafx.h>
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<W32/GrMfcMain.h>
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
BEGIN_MESSAGE_MAP(Cls_GrMfcMain, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()
//--------------------------------------------------------------------
Cls_GrMfcMain::Cls_GrMfcMain( UINT A_IdTmp, CWnd* A_PtrPrnt ):
CDialogEx( A_IdTmp, A_PtrPrnt )
{
	// local -------------------
	// code --------------------
		//init
		GrDumyZeroMem( &m_MfcMng, sizeof(m_MfcMng) );

}
//--------------------------------------------------------------------
Cls_GrMfcMain::~Cls_GrMfcMain()
{
}
//--------------------------------------------------------------------
void Cls_GrMfcMain::DoDataExchange(CDataExchange* pDX)
{
	// local -------------------
	// code --------------------
		CDialogEx::DoDataExchange(pDX);
}
//--------------------------------------------------------------------
void Cls_GrMfcMain::OnPaint()
{
	// local -------------------
	// code --------------------
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		__s32 cxIcon = GetSystemMetrics(SM_CXICON);
		__s32 cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		__s32 x = (rect.Width() - cxIcon + 1) / 2;
		__s32 y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_HndlIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//--------------------------------------------------------------------
HCURSOR	Cls_GrMfcMain::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_HndlIcon);
}
//--------------------------------------------------------------------
BOOL8 Cls_GrMfcMain::OnInitDialog()
{
	// local -------------------
	// code --------------------
	// set icon
	SetIcon(m_HndlIcon, TRUE);			// Set big icon
	SetIcon(m_HndlIcon, FALSE);			// Set small icon

	// load frame manager

	return	TRUE;
}
//--------------------------------------------------------------------

