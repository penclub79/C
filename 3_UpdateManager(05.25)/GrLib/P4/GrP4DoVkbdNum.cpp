/*
		platform virtual keyboard text

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoVkbdNum.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>
#include	<P4/GrP4DoBtnLbl.h>

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
Cls_GrP4DoVkbdNum::Cls_GrP4DoVkbdNum(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, __u8 A_Layer, Cls_GrP4DoBase* A_Host,
	WCHAR* A_StrPtr, __u32 A_MaxLen, BOOL8 A_IsPasswd ):
Cls_GrP4DoVkbdBase( A_ScptId, A_PtrRect, A_PosRel, A_Layer, A_Host,A_StrPtr, A_MaxLen, A_IsPasswd )
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
Cls_GrP4DoVkbdNum::~Cls_GrP4DoVkbdNum( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdNum::ProcVkbdKey( __u32 A_KeyIdx )
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
			Cls_GrP4DoVkbdBase::ProcVkbdKey(A_KeyIdx);
		}
}
//--------------------------------------------------------------------

