/*
		platform virtual keyboard text

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoVkbdNum.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>
#include	<P3/GrP3DoBtnLbl.h>

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
Cls_GrP3DoVkbdNum::Cls_GrP3DoVkbdNum( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_Layer, Cls_GrP3DoBase* A_Host,
	WCHAR* A_StrPtr, __u32 A_MaxLen, BOOL8 A_IsPasswd ):
Cls_GrP3DoVkbdBase( A_ScptId, A_PtrRect, A_Layer, A_Host,A_StrPtr, A_MaxLen, A_IsPasswd )
{
	// local -------------------
	// code --------------------
		//init
		GrStrClear( m_StrPtr );
		GrStrClear( m_StrPass );
		m_CusPos		=	0;
		m_Length		=	0;
}
//--------------------------------------------------------------------
Cls_GrP3DoVkbdNum::~Cls_GrP3DoVkbdNum( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoVkbdNum::ProcVkbdKey( __u32 A_KeyIdx )
{
	// local -------------------
		WCHAR	Tv_Code;
	// code --------------------
		if ( 10 >= A_KeyIdx )
		{
			// text key
			Tv_Code	=	(WCHAR)'0';
			Tv_Code	=	Tv_Code + (WCHAR)(A_KeyIdx - 1);
			VkCodeIns( Tv_Code, TRUE );
		}
		else
		{
			Cls_GrP3DoVkbdBase::ProcVkbdKey(A_KeyIdx);
		}
}
//--------------------------------------------------------------------

