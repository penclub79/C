/*
		platform query window

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoQuery.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>
#include	<P3/GrP3DoBtnLbl.h>

#include	<GrDebug.h>

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


//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoQuery::Cls_GrP3DoQuery( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_Layer, void* A_Console,
	WCHAR* A_StrCont, WCHAR* A_StrTitle ):
Cls_GrP3DoWin( A_ScptId, A_PtrRect, A_Layer, A_Console )
{
	// local -------------------
	// code --------------------
		//init
		GrStrWcopy( m_StrCont, A_StrCont, GrStrMaxLen(m_StrCont) );
		GrStrWcopy( m_StrTitle, A_StrTitle, GrStrMaxLen(m_StrTitle) );
		
}
//--------------------------------------------------------------------
Cls_GrP3DoQuery::~Cls_GrP3DoQuery( void )
{
	// local -------------------
	// code --------------------

}
//--------------------------------------------------------------------
void	Cls_GrP3DoQuery::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoWin::RtlScptParaSubUpdt();
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoQuery::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
void	Cls_GrP3DoQuery::ProcEvtBtnUp( Cls_GrP3DoBase* A_Obj )
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

