/*
		platform virtual keyboard base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoVkbdBase.h>
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
Cls_GrP4DoVkbdBase::Cls_GrP4DoVkbdBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, __u8 A_Layer, Cls_GrP4DoBase* A_Host,
	WCHAR* A_StrPtr, __u32 A_MaxLen, BOOL8 A_IsPasswd ):
Cls_GrP4DoWin( A_ScptId, A_PtrRect,A_PosRel, A_Layer, A_Host->ConsoleGet(), E_GrP4ScptIdNone )
{
	// local -------------------
	// code --------------------
		//init
		m_StrPtr	=	A_StrPtr;
		m_MaxLen	=	A_MaxLen;
		m_IsPasswd	=	A_IsPasswd;

		m_Host		=	A_Host;

		m_CusPos	=	0;
		m_Length	=	GrStrWlen( m_StrPtr );
		if ( m_IsPasswd )
		{
			if ( GrStrMaxLen(m_StrPass) > A_MaxLen )
			{
				if ( 0 != m_Length )
				{
					GrDumyFillW( (void*)m_StrPass, m_Length, (__u16)'*' );
				}
				m_StrPass[m_Length]	=	0;
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4DoVkbdBase::Cls_GrP4DoVkbdBase - password string size over.\n" );
			}
		}
		
		
}
//--------------------------------------------------------------------
Cls_GrP4DoVkbdBase::~Cls_GrP4DoVkbdBase( void )
{
	// local -------------------
		St_GrP4MsgHd	Tv_Msg;
		Cls_GrP4DoWin*	Tv_Win;
	// code --------------------
		// notify virtual keyboard end
		Tv_Msg.Cmd		=	E_GrP4MsgCmdVkbdEnd;
		Tv_Msg.ConIdx	=	((Cls_GrP4ConBase*)m_ObjCon)->IndexGet();
		Tv_Win				=	(Cls_GrP4DoWin*)m_Host->WindowGet();
		Tv_Msg.WinId	=	Tv_Win->WinIdGet();
		Tv_Msg.ObjId	=	m_Host->ScptIdGet();
		((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( &Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoWin::RtlScptParaSubUpdt();
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoVkbdBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			*A_PtrRtCus	=	(__s32)m_CusPos;
			if ( m_IsPasswd )
			{
				Tv_Result	=	m_StrPass;
			}
			else
			{
				Tv_Result		=	m_StrPtr;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::ProcEvtBtnUp( Cls_GrP4DoBase* A_Obj )
{
	// local -------------------
		__u32	Tv_KeyIdx;
	// code --------------------
		// check user data
		Tv_KeyIdx	=	A_Obj->UserDataGet( 0 );
		// check exist key index
		if ( 0 != Tv_KeyIdx )
		{
			ProcVkbdKey( Tv_KeyIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::ProcVkbdKey( __u32 A_KeyIdx )
{
	// local -------------------
	// code --------------------
		switch ( A_KeyIdx )
		{
			case E_GrP4VkeyEnter:
				ProcTxtUpdt();
				WindowClose();
				break;
			case E_GrP4VkeySpace:
				VkCodeIns( 0x20, TRUE );
				break;
			case E_GrP4VkeyDelete:
				VkDelete(); 
				break;
			case E_GrP4VkeyBackSpace:
				VkBackspace();
				break;
			case E_GrP4VkeyClear:
				VkClear();
				break;
			case E_GrP4VkeyHome:
				VkCusHome();
				break;
			case E_GrP4VkeyEnd:
				VkCusEnd();
				break;
			case E_GrP4VkeyCusLeft:
				VkCusLeft();
				break;
			case E_GrP4VkeyCusRight:
				VkCusRight();
				break;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::VkClear( void )
{
		GrStrClear( m_StrPtr );
		m_CusPos	=	0;
		if ( m_IsPasswd )
		{
			GrStrClear( m_StrPass );
		}
		m_Length	=	0;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::VkCusHome( void )
{
		m_CusPos	=	0;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::VkCusEnd( void )
{
		m_CusPos	=	(__s32)m_Length;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::VkCusLeft( void )
{
		if ( 0 < m_CusPos )
		{
			m_CusPos --;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::VkCusRight( void )
{
	// local -------------------
	// code --------------------
		if ( m_Length > (__u32)m_CusPos )
		{
			m_CusPos ++;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::VkCodeIns( WCHAR A_Code, BOOL8 A_IsMoveCus )
{
	// local -------------------
		WCHAR	Tv_StrCode[2];
	// code --------------------
		// init
		Tv_StrCode[0]	=	A_Code;
		Tv_StrCode[1]	=	0;
		// check full
		if ( m_MaxLen > m_Length )
		{
			// insert
			GrStrWins( m_StrPtr, Tv_StrCode, (__u32)m_CusPos );
			m_Length ++;
			if ( m_IsPasswd )
			{
				GrStrAddWchar( m_StrPass, 0x2A );		// insert *
			}
			if ( A_IsMoveCus )
			{
				VkCusRight();
			}
			ReqDraw();
		}
		//ProcTxtUpdt();	// text updated
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::VkCodeOvwt( WCHAR A_Code, BOOL8 A_IsMoveCus )
{
	// local -------------------
	// code --------------------
		if ( 0 != A_Code )
		{
			if ( m_Length > (__u32)m_CusPos )
			{
				m_StrPtr[m_CusPos]	=	A_Code;
				if ( A_IsMoveCus )
				{
					VkCusRight();
				}
				ReqDraw();
			}
		}
		//ProcTxtUpdt();	// text updated
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::VkBackspace( void )
{
		if ( 0 != m_CusPos  )
		{
			m_CusPos --;
			GrStrWdel( m_StrPtr, (__u32)m_CusPos, 1 );
			m_Length --;
			if ( m_IsPasswd )
			{
				GrStrWdel( m_StrPass, (__u32)m_CusPos, 1 );
			}
			ReqDraw();
		}
		//ProcTxtUpdt();	// text updated
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::VkDelete( void )
{
		if ( m_Length > (__u32)m_CusPos  )
		{
			GrStrWdel( m_StrPtr, (__u32)m_CusPos, 1 );
			m_Length --;
			ReqDraw();
		}
		//ProcTxtUpdt();	// text updated
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdBase::ProcTxtUpdt( void )
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoVkbdBase::ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		ProcTxtUpdt();
		return Cls_GrP4DoWin::ProcEvtMseRbtnUp(A_X, A_Y, A_BtnMap);
}

//--------------------------------------------------------------------

