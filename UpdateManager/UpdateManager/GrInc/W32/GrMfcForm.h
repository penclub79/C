/*
	MFC form
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrMfcForm
#define	_EPC_GrMfcForm

//====================================================================
// uses
//#include  <stdafx.h>

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include <W32/GrMfcBase.h>
#include <W32/GrMfcFrame.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type



//====================================================================
//class

class Cls_GrMfcForm	:	public CWnd
{

	protected:
		Ptr_GrMfcMng		m_PtrMfcMng;

		Cls_GrMfcFrame*	m_FrmTbl[E_GrMfcFrameMaxCnt];

		// internal API


		virtual	Cls_GrMfcFrame*	CreateFrame( __u8 A_FrmId );

		// event

		DECLARE_MESSAGE_MAP()

	public:
		Cls_GrMfcForm( Ptr_GrMfcMng A_PtrMng, UINT A_IdTmp, CWnd* A_PtrPrnt = NULL );
		virtual	~Cls_GrMfcForm();

		BOOL8	ViewFrame( __u8 A_FrmId, __u8 A_Mode );

		// runtime frame mode
		BOOL8	RtlFrmModeCreate( __u8 A_FrmId, __u8 A_Mode, Ptr_GrRect A_PtrRc );
};

//====================================================================
//global function

//====================================================================
//out Environment conditional
#endif

