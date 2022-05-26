/*
	MFC frame
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrMfcFrame
#define	_EPC_GrMfcFrame

//====================================================================
// uses
//#include  <stdafx.h>

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include <W32/GrMfcBase.h>


//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class Cls_GrMfcFrame	:	public CDialogEx
{
	private:
		Ptr_GrMfcMng	m_PtrMfcMng;


	protected:

		// inherited
		virtual BOOL8 OnInitDialog();
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

		// event
		afx_msg void OnPaint();
		DECLARE_MESSAGE_MAP()

	public:
		Cls_GrMfcFrame( Ptr_GrMfcMng A_PtrMng, UINT A_IdTmp, CWnd* A_PtrPrnt = NULL );
		virtual	~Cls_GrMfcFrame();

};

//====================================================================
//global function

//====================================================================
//out Environment conditional
#endif

