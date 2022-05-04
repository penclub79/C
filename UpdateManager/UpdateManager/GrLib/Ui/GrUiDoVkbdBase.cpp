/*
 virtual keyboard base class

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoVkbdBase.h>
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
Cls_GrUiDoVkbdBase::Cls_GrUiDoVkbdBase(	__u16 A_WinId, __u16 A_ObjId, BOOL8 A_IsPasswd, __u32 A_VkId, WCHAR* A_StrEdit, __u32 A_LenMax, BOOL8 A_IsPassChk,
																				__u8 A_ConIdx, BOOL8 A_IsUpdtParm ):
Cls_GrUiDoWin( NULL, E_GrUiDoWinScptLayer, A_ConIdx, FALSE, FALSE )
{
	// local -------------------
		
	// code --------------------
		//init
		m_EdtWinId		=	A_WinId;
		m_EdtObjId		=	A_ObjId;
		m_VkId				=	A_VkId;
		m_IsPasswd		=	A_IsPasswd;
		m_IsPassChk		=	A_IsPassChk;

		m_CusPos			=	0;
		m_EdtMax			=	E_GrUiVkbdEdtMax;
		if ( A_LenMax < m_EdtMax )
		{
			m_EdtMax	=	A_LenMax;
		}

		m_StrEdt	=	A_StrEdit;

		if ( m_IsPasswd )
		{
			GrStrClear(m_StrEdt);
		}
		

		if ( A_IsUpdtParm )
		{
			UpdateWinParam();
		}
}
//--------------------------------------------------------------------
Cls_GrUiDoVkbdBase::~Cls_GrUiDoVkbdBase()
{
	// local -------------------
		St_GrUiMsgVkFinish	Tv_Msg;
	// code --------------------
		//do message finish
		Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdVkFinish;
		Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
		Tv_Msg.Hd.WinId		=	m_EdtWinId;
		Tv_Msg.Hd.ObjId		=	m_EdtObjId;
		Tv_Msg.VkId				=	m_VkId;
		UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoVkbdBase::VkCodeIns( __u16 A_Code, BOOL8 A_IsMovCus )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Len;
		WCHAR		Tv_StrCode[2];
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		//get code len
		Tv_StrCode[0]		=	A_Code;
		Tv_StrCode[1]		=	0;
		Tv_Len		=	GrStrWlen( m_StrEdt );
		//check password mode
		/*
		if (m_EdtMax <= Tv_Len)
		{
			//check password
			if ( m_IsPasswd )
			{
				//reset string
				GrStrClear( m_StrEdt );
				m_CusPos		=	0;
				Tv_Len			=	0;
			}
		}
		*/
		//check len over and cursor position
		if ( (m_EdtMax > Tv_Len) && ((__u32)m_CusPos <= Tv_Len) )
		{
			//insert
			GrStrWins( m_StrEdt, Tv_StrCode, (__u32)m_CusPos );
			//cursor move
			if ( A_IsMovCus )
			{
				VkCusRight();
			}

			//update
			LcLblEdtUpdt( TRUE );
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoVkbdBase::VkBtnCodeGet( Cls_GrUiDoBtn* A_Btn )
{
	// local -------------------
		WCHAR		Tv_StrCode[2];
	// code --------------------
		//get code
		A_Btn->TextGet( Tv_StrCode, 1 );

		return	Tv_StrCode[0];
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::VkCodeOvwt( __u16 A_Code, BOOL8 A_IsMovCus )
{
	// local -------------------
		__u32		Tv_LenStr;
	// code --------------------
		//check able code
		if ( 0 != A_Code )
		{
			Tv_LenStr	=	GrStrWlen( m_StrEdt );
			//check limite
			if ( Tv_LenStr > (__u32)m_CusPos )
			{
				//overwrite
				m_StrEdt[m_CusPos]	=	A_Code;
				//move cursur
				if ( A_IsMovCus )
				{
					VkCusRight();
				}
				//update
				LcLblEdtUpdt( TRUE );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::VkCusLeft( void )
{
		if ( 0 < m_CusPos )
		{
			m_CusPos --;
			LcLblEdtUpdt( FALSE );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::VkCusRight( void )
{
	// local -------------------
		__u32		Tv_LenStr;
	// code --------------------
		Tv_LenStr	=	GrStrWlen( m_StrEdt );
		if ( Tv_LenStr > (__u32)m_CusPos )
		{
			m_CusPos	++;
			LcLblEdtUpdt( FALSE );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::VkCusDel( void )
{
		GrStrWdel( m_StrEdt, (__u32)m_CusPos, 1 );
		LcLblEdtUpdt( TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::VkCusBkspc( void )
{
		if ( 0 != m_CusPos )
		{
			m_CusPos --;
			GrStrWdel( m_StrEdt, (__u32)m_CusPos, 1 );
			LcLblEdtUpdt( TRUE );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::LcTxtNrmProc( __u16 A_Val )
{
		VkCodeIns( A_Val, TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::ProcEvtWinCreate( void )
{
	// local -------------------
		St_GrUiDoTextPara	Tv_TxtPara;
	// code --------------------
		//build script
		V_GrUiMngObj->ScptDecById( m_ObjId, (Cls_GrUiDoBase*)this );
		//inherited
		Cls_GrUiDoWin::ProcEvtWinCreate();
		//find slave frame
		m_LblEdit		=	(Cls_GrUiDoLbl*)FindSlaveObjByObjId( (__u16)m_UserDataTbl[0] );
		if ( NULL != m_LblEdit )
		{
			//init edit label
			m_LblEdit->TextSet( m_StrEdt );
			m_LblEdit->CusOn( TRUE );
			m_LblEdit->CusPosSet( m_CusPos );
			m_LblEdit->TextParaGet( &Tv_TxtPara );
			if ( m_IsPasswd )
			{
				Tv_TxtPara.Atb	=	Tv_TxtPara.Atb | E_GrUiDoTextAtbPasswd;
			}
			else
			{
				Tv_TxtPara.Atb	=	Tv_TxtPara.Atb & ~E_GrUiDoTextAtbPasswd;
			}
			m_LblEdit->TextParaSet( &Tv_TxtPara );
		}

		//currect focus
		m_ObjConsole->FocusObjSet( NULL );
		m_ObjConsole->FocusCorrect();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::ProcEvtBtnUp( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		__u32		Tv_UserData;
		__u16		Tv_Val;
		__u32		Tv_Type;
	// code --------------------
		//check button type
		Tv_UserData		=	A_Obj->UserDataGet();
		Tv_Val	=	(__u16)( E_GrUiDoVkbdUdcCodeMask & Tv_UserData );
		Tv_Type	=	E_GrUiDoVkbdUdcTypeMask & Tv_UserData;
		switch ( Tv_Type )
		{
			case	E_GrUiDoVkbdUdcTypeSpc:
				LcSpcBtnProc( Tv_Val );
				break;
			case	E_GrUiDoVkbdUdcTypeNormal:
				Tv_Val	=	VkBtnCodeGet( (Cls_GrUiDoBtn*)A_Obj );
				LcTxtNrmProc( Tv_Val );
				break;
			default:
				Cls_GrUiDoWin::ProcEvtBtnUp( A_Obj );
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::LcSpcBtnProc( __u16 A_Val )
{
		switch ( A_Val )
		{
			case	E_GrUiDoVkbdUdcEdtLeft:
				VkCusLeft();
				break;
			case	E_GrUiDoVkbdUdcEdtRight:
				VkCusRight();
				break;
			case	E_GrUiDoVkbdUdcEdtDelete:
				VkCusDel();
				break;
			case	E_GrUiDoVkbdUdcEdtClear:
				VkClear();
				break;	
			case	E_GrUiDoVkbdUdcEdtHome:
				VkCusHome();
				break;
			case	E_GrUiDoVkbdUdcEdtEnd:
				VkCusEnd();
				break;
			case	E_GrUiDoVkbdUdcEdtSpace:
				VkCodeIns( 0x20 );		/* space code */
				break;
			case	E_GrUiDoVkbdUdcEdtOk:
				LcProcBtnOk();
				break;
			case	E_GrUiDoVkbdUdcEdtBkspc:
				VkCusBkspc();
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::VkClear( void )
{
		GrStrClear( m_StrEdt );
		m_CusPos		=	0;
		LcLblEdtUpdt( TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::VkCusHome( void )
{
		m_CusPos		=	0;
		LcLblEdtUpdt( FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::VkCusEnd( void )
{
		m_CusPos	=	(__s32)GrStrWlen( m_StrEdt );
		LcLblEdtUpdt( FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::LcLblEdtUpdt( BOOL8 A_IsWithStr )
{
	// local -------------------
		__u8	Tv_PassStat;
	// code --------------------
		//check edit label exist
		if ( NULL != m_LblEdit )
		{
			if ( A_IsWithStr )
			{
				// check password check
				if ( m_IsPassChk && V_GrUiMngObj->IsPasswdChk() )
				{
					Tv_PassStat	=	GrStrRulePasswdWstr( m_StrEdt );
					if (	(E_GrStrPasswdGood != Tv_PassStat) && (E_GrStrPasswdShort != Tv_PassStat) )
					{
						MsgBox( E_GrUiTxtIdNone , V_GrUiMngObj->PasswdErrTxtIdGet( Tv_PassStat ), E_GrUiDoMsgBoxTypeYes );
						GrStrClear( m_StrEdt );
						m_CusPos	=	0;
					}
				}
				m_LblEdit->TextSet( m_StrEdt );
			}
			m_LblEdit->CusPosSet( m_CusPos );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdBase::LcProcBtnOk( void )
{

		//request close
		Close();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoVkbdBase::ProcEvtKeyDown( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Num;
	// code --------------------
		Tv_Result	=	FALSE;

		switch( A_Key )
		{
			case	E_GrUiKeyCh0:
			case	E_GrUiKeyCh1:
			case	E_GrUiKeyCh2:
			case	E_GrUiKeyCh3:
			case	E_GrUiKeyCh4:
			case	E_GrUiKeyCh5:
			case	E_GrUiKeyCh6:
			case	E_GrUiKeyCh7:
			case	E_GrUiKeyCh8:
			case	E_GrUiKeyCh9:
			case	E_GrUiKeyCh10:
				Tv_Num	=	A_Key - E_GrUiKeyCh0;
				if ( 10 <= Tv_Num )
				{
					Tv_Num	=	0;
				}
				Tv_Num	=	Tv_Num + L'0';
				VkCodeIns( Tv_Num );
				Tv_Result	=	TRUE;
				break;
			default:
				Tv_Result	=	Cls_GrUiDoWin::ProcEvtKeyDown( A_Key );
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoVkbdBase::ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrUiDoWin::ProcEvtMseRbtnUp( A_X, A_Y, A_BtnMap	);

		if ( !Tv_Result )
		{
			Close();
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

