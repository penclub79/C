/*
	MFC Main form
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrMfcMain
#define	_EPC_GrMfcMain

//====================================================================
// uses
//#include  <stdafx.h>

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include <W32/GrMfcBase.h>
#include <W32/GrMfcForm.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type



//====================================================================
//class

class Cls_GrMfcMain	:	public CDialogEx
{

	protected:
		HICON	m_HndlIcon;				// main icon

		St_GrMfcMng	m_MfcMng;		// manager

		// inherited
		virtual BOOL8 OnInitDialog();
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

		// event
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
	public:
		Cls_GrMfcMain( UINT A_IdTmp, CWnd* A_PtrPrnt = NULL );
		virtual	~Cls_GrMfcMain();

};

//====================================================================
//global function

//====================================================================
//out Environment conditional
#endif

