/*
 osd object

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoVkbdNum.h>
#include	<GrStrTool.h>
#include	<GrTimeTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiConsole.h>
#include	<Ui/GrUiScptDec.h>

//====================================================================
//local const

//====================================================================
//local type


//====================================================================
//local macro
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*				V_GrUiMngObj;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoVkbdNum::Cls_GrUiDoVkbdNum(	__u16 A_WinId, __u16 A_ObjId, BOOL8 A_IsPasswd, __u32 A_VkId, WCHAR* A_StrEdit, __u32 A_LenMax, 
																			__u8 A_ConIdx, BOOL8 A_IsUpdtParm ):
Cls_GrUiDoVkbdBase( A_WinId, A_ObjId, A_IsPasswd, A_VkId, A_StrEdit, A_LenMax, FALSE, A_ConIdx, FALSE )
{
	// local -------------------
		BOOL8		Tv_IsSlave;
	// code --------------------
		//init
		m_ObjId		=	E_GrUiObjIdWinVkbdNum;

		V_GrUiMngObj->ScptAtbGet( m_ObjId, &m_Layer, &m_Rect, &Tv_IsSlave  );

		if ( A_IsUpdtParm )
		{
			UpdateWinParam();
		}
}
//--------------------------------------------------------------------
Cls_GrUiDoVkbdNum::~Cls_GrUiDoVkbdNum()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdNum::ProcEvtWinCreate( void )
{
	// local -------------------
	// code --------------------
		//do notify create
		Cls_GrUiDoVkbdBase::ProcEvtWinCreate();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoVkbdNum::VkCodeIns( __u16 A_Code, BOOL8 A_IsMovCus )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Len;
	// code --------------------
		//check data full
		Tv_Len		=	GrStrWlen( m_StrEdt );
		//check password mode
		if (m_EdtMax <= Tv_Len)
		{
			GrStrClear( m_StrEdt );
			m_CusPos	=	0;
		}
		Tv_Result	=	Cls_GrUiDoVkbdBase::VkCodeIns( A_Code, A_IsMovCus );

		return	Tv_Result;
}
//--------------------------------------------------------------------

