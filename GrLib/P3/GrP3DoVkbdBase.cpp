/*
		platform virtual keyboard base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoVkbdBase.h>
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
Cls_GrP3DoVkbdBase::Cls_GrP3DoVkbdBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_Layer, Cls_GrP3DoBase* A_Host,
	WCHAR* A_StrPtr, __u32 A_MaxLen, BOOL8 A_IsPasswd ):
Cls_GrP3DoWin( A_ScptId, A_PtrRect, A_Layer, A_Host->ConsoleGet() )
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
				DbgMsgPrint( "Cls_GrP3DoVkbdBase::Cls_GrP3DoVkbdBase - password string size over.\n" );
			}
		}
		
		
}
//--------------------------------------------------------------------
Cls_GrP3DoVkbdBase::~Cls_GrP3DoVkbdBase( void )
{
	// local -------------------
		St_GrP3MsgHd	Tv_Msg;
		Cls_GrP3DoWin*	Tv_Win;
	// code --------------------
		// notify virtual keyboard end
		Tv_Msg.Cmd		=	E_GrP3MsgCmdVkbdEnd;
		Tv_Msg.ConIdx	=	((Cls_GrP3ConBase*)m_ObjCon)->IndexGet();
		Tv_Win				=	(Cls_GrP3DoWin*)m_Host->WindowGet();
		Tv_Msg.WinId	=	Tv_Win->WinIdGet();
		Tv_Msg.ObjId	=	m_Host->ScptIdGet();
		((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( &Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoVkbdBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoWin::RtlScptParaSubUpdt();
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DoVkbdBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
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
void	Cls_GrP3DoVkbdBase::ProcEvtBtnUp( Cls_GrP3DoBase* A_Obj )
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
void	Cls_GrP3DoVkbdBase::ProcVkbdKey( __u32 A_KeyIdx )
{
	// local -------------------
	// code --------------------
		switch ( A_KeyIdx )
		{
			case E_GrP3VkeyEnter:
				ProcTxtUpdt();
				WindowClose();
				break;
			case E_GrP3VkeySpace:
				VkCodeIns( 0x20, TRUE );
				break;
			case E_GrP3VkeyDelete:
				VkDelete(); 
				break;
			case E_GrP3VkeyBackSpace:
				VkBackspace();
				break;
			case E_GrP3VkeyClear:
				VkClear();
				break;
			case E_GrP3VkeyHome:
				VkCusHome();
				break;
			case E_GrP3VkeyEnd:
				VkCusEnd();
				break;
			case E_GrP3VkeyCusLeft:
				VkCusLeft();
				break;
			case E_GrP3VkeyCusRight:
				VkCusRight();
				break;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3DoVkbdBase::VkClear( void )
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
void	Cls_GrP3DoVkbdBase::VkCusHome( void )
{
		m_CusPos	=	0;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoVkbdBase::VkCusEnd( void )
{
		m_CusPos	=	(__s32)m_Length;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoVkbdBase::VkCusLeft( void )
{
		if ( 0 < m_CusPos )
		{
			m_CusPos --;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoVkbdBase::VkCusRight( void )
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
void	Cls_GrP3DoVkbdBase::VkCodeIns( WCHAR A_Code, BOOL8 A_IsMoveCus )
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
void	Cls_GrP3DoVkbdBase::VkCodeOvwt( WCHAR A_Code, BOOL8 A_IsMoveCus )
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
void	Cls_GrP3DoVkbdBase::VkBackspace( void )
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
void	Cls_GrP3DoVkbdBase::VkDelete( void )
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
void	Cls_GrP3DoVkbdBase::ProcTxtUpdt( void )
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoVkbdBase::ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		ProcTxtUpdt();
		return Cls_GrP3DoWin::ProcEvtMseRbtnUp(A_X, A_Y, A_BtnMap);
}

//--------------------------------------------------------------------

