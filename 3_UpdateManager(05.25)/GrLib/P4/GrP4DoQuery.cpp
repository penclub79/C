/*
		platform query window

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoQuery.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>
#include	<P4/GrP4DoBtnLbl.h>

#include	<GrDebug.h>

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


//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoQuery::Cls_GrP4DoQuery(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, __u8 A_Layer, void* A_Console,
	WCHAR* A_StrCont, WCHAR* A_StrTitle, __u16 A_RefPosWin) :
Cls_GrP4DoWin( A_ScptId, A_PtrRect,A_PosRel, A_Layer, A_Console, A_RefPosWin )
{
	// local -------------------
	// code --------------------
		//init
		GrStrWcopy( m_StrCont, A_StrCont, GrStrMaxLen(m_StrCont) );
		GrStrWcopy( m_StrTitle, A_StrTitle, GrStrMaxLen(m_StrTitle) );
		
}
//--------------------------------------------------------------------
Cls_GrP4DoQuery::~Cls_GrP4DoQuery( void )
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
void	Cls_GrP4DoQuery::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoWin::RtlScptParaSubUpdt();
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoQuery::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	m_StrCont;
		}
		else if ( 1 == A_Idx )
		{
			Tv_Result	=	m_StrTitle;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoQuery::ProcEvtBtnUp( Cls_GrP4DoBase* A_Obj )
{
	// local -------------------
		__u32	Tv_KeyIdx;
	// code --------------------
		// check user data
		Tv_KeyIdx	=	A_Obj->UserDataGet( 0 );
		// check exist key index
		if ( 0 != Tv_KeyIdx )
		{
			m_ModalRslt	=	(__s32)Tv_KeyIdx;
			WindowClose();
		}
}
//--------------------------------------------------------------------

