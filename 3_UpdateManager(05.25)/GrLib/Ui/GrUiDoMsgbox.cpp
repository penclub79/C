/*
	message box class

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoMsgbox.h>
#include	<GrStrTool.h>
#include	<GrTimeTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiConsole.h>
#include	<Ui/GrUiScptDec.h>

//====================================================================
//local const

#define E_GrUiDoMsgboxUdiLblTitle			0
#define E_GrUiDoMsgboxUdiLblContents	1

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
Cls_GrUiDoMsgbox::Cls_GrUiDoMsgbox(	__u16 A_ScptId, WCHAR* A_StrTitle, WCHAR* A_StrCont,
																		__u8 A_Layer, __u8 A_ConIdx, BOOL8 A_IsUpdtParm ):
Cls_GrUiDoWin( NULL, A_Layer, A_ConIdx, FALSE, FALSE )
{
	// local -------------------
		BOOL8	Tv_IsSlave;
	// code --------------------
		//init
		m_ObjId				=	A_ScptId;

		m_ModalResult	=	E_GrUiDoMsgBoxRsltNo;

		GrStrClear( m_StrTitle );
		if ( NULL != A_StrTitle )
		{
			GrStrWcopy( m_StrTitle, A_StrTitle, GrStrMaxLen(m_StrTitle) );
		}
		GrStrClear( m_StrCont ); 
		if ( NULL != A_StrCont )
		{
			GrStrWcopy( m_StrCont, A_StrCont, GrStrMaxLen(m_StrCont) );
		}

		V_GrUiMngObj->ScptAtbGet( m_ObjId, &m_Layer, &m_Rect, &Tv_IsSlave  );

		if ( A_IsUpdtParm )
		{
			UpdateWinParam();
		}
}	
//--------------------------------------------------------------------
Cls_GrUiDoMsgbox::~Cls_GrUiDoMsgbox()
{
	// local -------------------
	// code --------------------
}
//--------------------------------------------------------------------
void	Cls_GrUiDoMsgbox::ProcEvtWinCreate( void )
{
	// local -------------------
		Cls_GrUiDoLbl*	Tv_LblTitle;
		Cls_GrUiDoLbl*	Tv_LblCont;
	// code --------------------
		
		//build script
		V_GrUiMngObj->ScptDecById( m_ObjId, this );

		//title set
		Tv_LblTitle		=	(Cls_GrUiDoLbl*)FindSlaveObjByObjId( (__u16)m_UserDataTbl[E_GrUiDoMsgboxUdiLblTitle] );
		if ( NULL != Tv_LblTitle )
		{
			//check label
			if ( E_GrUiDoTypeIdLabel == Tv_LblTitle->TypeIdGet() )
			{
				Tv_LblTitle->TextSet( m_StrTitle );
			}
		}
		
		//contents set
		Tv_LblCont		=	(Cls_GrUiDoLbl*)FindSlaveObjByObjId( (__u16)m_UserDataTbl[E_GrUiDoMsgboxUdiLblContents] );
		if ( NULL != Tv_LblCont )
		{
			//check label
			if ( E_GrUiDoTypeIdLabel == Tv_LblCont->TypeIdGet() )
			{
				Tv_LblCont->TextSet( m_StrCont );
			}
		}

		//inherited
		Cls_GrUiDoWin::ProcEvtWinCreate();

		//change focus
		m_ObjConsole->FocusObjSet( NULL );
		m_ObjConsole->FocusCorrect();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoMsgbox::ProcEvtBtnUp( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		__u32		Tv_UserData;
	// code --------------------
		//check button type
		Tv_UserData		=	A_Obj->UserDataGet();
		switch ( Tv_UserData )
		{
			case	E_GrUiDoMsgBoxBudYes:
			case	E_GrUiDoMsgBoxBudNo:
			case	E_GrUiDoMsgBoxBudCancel:
				m_ModalResult	=	(__u8)Tv_UserData;
				Close();
				break;
			default:
				Cls_GrUiDoWin::ProcEvtBtnUp( A_Obj );
				break;
		}
}
//--------------------------------------------------------------------

