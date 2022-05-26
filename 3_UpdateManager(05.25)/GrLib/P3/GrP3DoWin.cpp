/*
		platform window

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoWin.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>
#include <P3/GrP3Cfg.h>

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

#ifdef GR_P3_EDITOR

#define E_GrP3DoWinParaCnt		68

St_GrP3DvlDoPrInfoData	V_GrP3DoWinParaInfo[E_GrP3DoWinParaCnt]	=	
{
	{	L"InputBlock",	TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3DoWinPara)0)->IsInputBlock,NULL,	L"No input any device"	},
	{	L"LayerLock",		TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3DoWinPara)0)->IsLayerLock,NULL,	L"Don't move focus to behind window"	},
	{	L"KeyDirect",		TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3DoWinPara)0)->IsKeyDirect,NULL,	L"Window access key. key can not access sub control"	},
	{	L"EscKeyQuit",	TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3DoWinPara)0)->IsEsckeyQuit,NULL,	L"Close window when press menu key or esc key"	},
	{	L"RbtnQuit",		TRUE,		E_GrP3DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP3DoWinPara)0)->IsRbtnQuit,NULL,	L"Close window when press mouse right button"	},
	{	L"Paint BG",		TRUE,		E_GrP3DvlParaTypePntCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->PntBg,NULL,	L"Paint background code.\n0 = None, 1 = basic code"	},
	{	L"StartFocus",	TRUE,		E_GrP3DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP3DoWinPara)0)->StartFocus,NULL,	L"Start focus object id. you can input this window id."	},
	{	L"EventCreate",	TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->EvtCreate,NULL,	L"When window creat, this code will be run."	},
	{	L"EventClose",	TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->EvtClose,NULL,	L"When window close, this code will be run."	},
	{	L"EvtMseRbtnUp",	TRUE,		E_GrP3DvlParaTypeAsmCode,		1,	(void*)&((Ptr_GrP3DoWinPara)0)->EvtMseRbtnUp,NULL,	L"When mouse right button up, this code will be run."	},
	{	L"EvtReqClose",	TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->EvtReqClose,NULL,	L"When the mouse or key request quit,then this code will be run."	},
	{	L"EvtSysStat",	TRUE,		E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->EvtSysStat,NULL,	L"When system stauts changed,then this code will be run."	},

	{	L"Evt Key0",		TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[0],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key0 code",	FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[0],NULL,	L""	},
	{	L"Evt Key0 asm",	FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[0],NULL,	L""	},

	{	L"Evt Key1",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[1],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key1 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[1],NULL,	L""	},
	{	L"Evt Key1 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[1],NULL,	L""	},

	{	L"Evt Key2",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[2],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key2 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[2],NULL,	L""	},
	{	L"Evt Key2 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[2],NULL,	L""	},

	{	L"Evt Key3",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[3],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key3 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[3],NULL,	L""	},
	{	L"Evt Key3 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[3],NULL,	L""	},

	{	L"Evt Key4",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[4],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key4 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[4],NULL,	L""	},
	{	L"Evt Key4 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[4],NULL,	L""	},

	{	L"Evt Key5",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[5],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key5 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[5],NULL,	L""	},
	{	L"Evt Key5 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[5],NULL,	L""	},

	{	L"Evt Key6",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[6],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key6 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[6],NULL,	L""	},
	{	L"Evt Key6 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[6],NULL,	L""	},

	{	L"Evt Key7",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[7],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key7 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[7],NULL,	L""	},
	{	L"Evt Key7 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[7],NULL,	L""	},

	{	L"Evt Key8",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[8],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key8 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[8],NULL,	L""	},
	{	L"Evt Key8 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[8],NULL,	L""	},

	{	L"Evt Key9",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[9],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key9 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[9],NULL,	L""	},
	{	L"Evt Key9 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[9],NULL,	L""	},

	{	L"Evt Key10",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[10],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key10 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[10],NULL,	L""	},
	{	L"Evt Key10 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[10],NULL,	L""	},

	{	L"Evt Key11",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[11],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key11 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[11],NULL,	L""	},
	{	L"Evt Key11 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[11],NULL,	L""	},

	{	L"Evt Key12",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[12],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key12 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[12],NULL,	L""	},
	{	L"Evt Key12 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[12],NULL,	L""	},

	{	L"Evt Key13",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[13],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key13 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[13],NULL,	L""	},
	{	L"Evt Key13 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[13],NULL,	L""	},

	{	L"Evt Key14",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[14],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key14 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[14],NULL,	L""	},
	{	L"Evt Key14 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[14],NULL,	L""	},

	{	L"Evt Key15",	TRUE,		E_GrP3DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyTypeTbl[15],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key15 code",						FALSE,	E_GrP3DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyCodeTbl[15],NULL,	L""	},
	{	L"Evt Key15 asm",						FALSE,	E_GrP3DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->KeyAsmTbl[15],NULL,	L""	},

	{	L"Timer0 Period",	TRUE,		E_GrP3DvlParaTypeWord,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->TimerPeriod[0],NULL,	L"Timer 0 period. sec unit"	},
	{	L"Event Timer0",	TRUE,		E_GrP3DvlParaTypeAsmCode,		1,	(void*)&((Ptr_GrP3DoWinPara)0)->EvtTimer[0],NULL,	L"Timer 0 event code"	},

	{	L"Timer1 Period",	TRUE,		E_GrP3DvlParaTypeWord,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->TimerPeriod[1],NULL,	L"Timer 1 period. sec unit"	},
	{	L"Event Timer1",	TRUE,		E_GrP3DvlParaTypeAsmCode,		1,	(void*)&((Ptr_GrP3DoWinPara)0)->EvtTimer[1],NULL,	L"Timer 1 event code"	},

	{	L"Timer2 Period",	TRUE,		E_GrP3DvlParaTypeWord,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->TimerPeriod[2],NULL,	L"Timer 2 period. sec unit"	},
	{	L"Event Timer2",	TRUE,		E_GrP3DvlParaTypeAsmCode,		1,	(void*)&((Ptr_GrP3DoWinPara)0)->EvtTimer[2],NULL,	L"Timer 2 event code"	},

	{	L"Timer3 Period",	TRUE,		E_GrP3DvlParaTypeWord,			1,	(void*)&((Ptr_GrP3DoWinPara)0)->TimerPeriod[3],NULL,	L"Timer 3 period. sec unit"	},
	{	L"Event Timer3",	TRUE,		E_GrP3DvlParaTypeAsmCode,		1,	(void*)&((Ptr_GrP3DoWinPara)0)->EvtTimer[3],NULL,	L"Timer 3 event code"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoWin::Cls_GrP3DoWin( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_Layer, void* A_Console ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, NULL )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId	=	E_GrP3DoClassWin;

		m_ParaSize	=	m_ParaSize + sizeof( m_ParaDoWin ) + sizeof(St_GrP3ScptParaHd);
		
		m_Layer			=	A_Layer;

		m_KeyLast			=	E_GrP3KeyNone;
		m_KeyTime			=	0;

		m_IsDoModal			=	FALSE;
		m_PtrMrslt			=	NULL;
		m_ModalRslt			=	0;

		m_ObjCon				=	A_Console;

		m_FocusObj			=	NULL;

		GrDumyZeroMem( &m_ParaDoWin, sizeof(m_ParaDoWin) );
#ifdef GR_P3_EDITOR
		if ( (E_GrP3ScptIdPageBase > A_ScptId) || ((E_GrP3ScptIdPageBase + E_GrP3DvlPageMaxCnt) <= A_ScptId) )
		{
			m_ParaDoWin.PntBg	=	E_GrP3ScptPstIdDflt;
		}
#else
		m_ParaDoWin.PntBg	=	E_GrP3ScptPstIdDflt;
#endif
		GrDumyZeroMem( m_TimerCntTbl, sizeof(m_TimerCntTbl) );

		// register console
		m_WinId	=	((Cls_GrP3ConBase*)m_ObjCon)->RtlWinRegist( this, m_Layer );

		m_LongKeyTime	=	V_GrP3Mng->LongKeyTimeGet() * 10;
		m_KeyLongIdx	=	0xFF;
		m_KeyTime			=	0;

		// paint
		ReqDraw();
}
//--------------------------------------------------------------------
Cls_GrP3DoWin::~Cls_GrP3DoWin( void )
{
	// local -------------------
	// code --------------------
		// close event
		if ( !((Cls_GrP3ConBase*)m_ObjCon)->IsShutDown() )
		{
			if ( E_GrP3ScptIdNone != m_ParaDoWin.EvtClose )
			{
				V_GrP3Mng->VmRunCode( m_ParaDoWin.EvtClose, this );
			}
			// update modal result
			if ( m_IsDoModal && (NULL != m_PtrMrslt) )
			{
				*m_PtrMrslt	=	m_ModalRslt;
			}
		}

}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3DoWin::LastFocusObjGet( void )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Result;
	// code --------------------
		if ( m_ParaDoWin.IsKeyDirect )
		{
			Tv_Result	=	(Cls_GrP3DoBase*)this;
		}
		else
		{
			if ( NULL == m_FocusObj )
			{
				// find key input able object
				m_FocusObj	=	FindFocusableSlave();
			}
			Tv_Result	=	m_FocusObj;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::RtlLastFocusObjSet( Cls_GrP3DoBase* A_Obj )
{
		m_FocusObj	=	A_Obj;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DoWin::WinIdGet( void )
{
		return	m_WinId;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::IsWinInputBlock( void )
{
		return	m_ParaDoWin.IsInputBlock;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::IsWinMouseAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsVisible && m_ParaDoBase.IsEnable )
		{
			Tv_Result	=	!m_ParaDoWin.IsInputBlock;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::IsWinKeyAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsVisible && m_ParaDoBase.IsEnable )
		{
			Tv_Result	=	!m_ParaDoWin.IsInputBlock;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassWin;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoWin);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoWin, sizeof(m_ParaDoWin) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoWin)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::IsWinKeyDirect( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaDoBase.IsEnable && m_IsVisible && m_ParaDoWin.IsKeyDirect )
		{
			Tv_Result	=	TRUE;
			if ( m_ParaDoBase.IsKeyBlock || m_ParaDoWin.IsInputBlock )
			{
				Tv_Result	=	FALSE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		// update
		ParaCopyFromScpt( &m_ParaDoWin, sizeof(m_ParaDoWin) , m_PtrScpt , E_GrP3DoClassWin );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiWinClose:
				WindowClose();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::ProcUiMsg( Ptr_GrP3MsgHd A_PtrMsg )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		//default event handler
		switch( A_PtrMsg->Cmd )
		{
			case E_GrP3MsgCmdBtnDown:
				Tv_Obj	=	FindSlaveObjByScptId( ((Ptr_GrP3MsgObj)A_PtrMsg)->ObjId );
				if ( NULL != Tv_Obj )
				{
					ProcEvtBtnDown( Tv_Obj );
				}
				break;
			case E_GrP3MsgCmdBtnUp:
				Tv_Obj	=	FindSlaveObjByScptId( ((Ptr_GrP3MsgObj)A_PtrMsg)->ObjId );
				if ( NULL != Tv_Obj )
				{
					ProcEvtBtnUp( Tv_Obj );
				}
				break;
			default:
				Cls_GrP3DoBase::ProcUiMsg( A_PtrMsg );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::IsWinLayerLock( void )
{
		return	m_ParaDoWin.IsLayerLock;
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3DoWin::FindMseObjByPos( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Result;
		Cls_GrP3DoBase*	Tv_ObjFind;
	// code --------------------
		Tv_Result	=	NULL;
		// check can mouse input able
		if ( m_IsVisible && m_ParaDoBase.IsEnable && (!m_ParaDoWin.IsInputBlock) )
		{
			// check window rect
			if ( (A_X >= m_Rect.left) && (A_X < m_Rect.right) && (A_Y >= m_Rect.top) && (A_Y < m_Rect.bottom) )
			{
				Tv_Result	=	(Cls_GrP3DoBase*)this;
				// find slave
				Tv_ObjFind	=	RtlFindMseObjByPos( A_X, A_Y, 0, 0 );
				if ( NULL != Tv_ObjFind )
				{
					// update
					Tv_Result	=	Tv_ObjFind;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3DoWin::FindObjByPos( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Result;
		Cls_GrP3DoBase*	Tv_ObjFind;
	// code --------------------
		Tv_Result	=	NULL;
		// check window rect
		if ( (A_X >= m_Rect.left) && (A_X < m_Rect.right) && (A_Y >= m_Rect.top) && (A_Y < m_Rect.bottom) )
		{
			Tv_Result	=	(Cls_GrP3DoBase*)this;
			// find slave
			Tv_ObjFind	=	RtlFindObjByPos( A_X, A_Y, 0, 0 );
			if ( NULL != Tv_ObjFind )
			{
				// update
				Tv_Result	=	Tv_ObjFind;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::WindowClose( void )
{
	// local -------------------
		St_GrP3MsgWinClose	Tv_Msg;
	// code --------------------
		// request to console
		Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdConWinClose;
		Tv_Msg.Hd.ConIdx	=	E_GrP3ConIdxFocus;
		Tv_Msg.Hd.WinId		=	E_GrP3WinIdNone;
		Tv_Msg.Hd.ObjId		=	E_GrP3ScptIdNone;
		Tv_Msg.WinId			=	m_WinId;
		((Cls_GrP3ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::IsDoingModal( void )
{
		return	m_IsDoModal;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::IsEnable( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		if ( !m_ParaDoBase.IsEnable )
		{
			Tv_Result	=	FALSE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::IsCanEnable( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		if ( !m_ParaDoBase.IsEnable )
		{
			Tv_Result	=	FALSE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::RtlModalDoingSet( BOOL8 A_IsDo, __s32* A_PtrRslt )
{
		m_IsDoModal	=	A_IsDo;
		m_PtrMrslt	=	A_PtrRslt;
}
//--------------------------------------------------------------------
Cls_GrP3DoBase*	Cls_GrP3DoWin::FindObjByScptId( __u16 A_ScptId )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		// check self
		if ( A_ScptId == m_ScptId	)
		{
			Tv_Result	=	(Cls_GrP3DoBase*)this;
		}
		else
		{
			// find slave
			Tv_Result	=	FindSlaveObjByScptId( A_ScptId );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrP3DoWin::ModalResultGet( void )
{
		return	m_ModalRslt;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::RtlModalResultSet( __s32 A_Val )
{
		m_ModalRslt		=	A_Val;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DoWin::EvtCloseCodeGet( void )
{
		return	m_ParaDoWin.EvtClose;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DoWin::EvtCreateCodeGet( void )
{
		return	m_ParaDoWin.EvtCreate;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::ProcPaint( void )
{
		PaintScriptPush( m_ParaDoWin.PntBg, &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherite
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoWin, V_GrP3DoWinParaInfo, E_GrP3DoWinParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::RtlInit( void )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		// do slave
		Tv_Obj	=	m_ObjSlave;
		while ( NULL != Tv_Obj )
		{
			// init
			Tv_Obj->RtlInit();
			// next
			Tv_Obj	=	Tv_Obj->NextObjGet();
		}
		// check window focus
		if ( m_ParaDoWin.IsKeyDirect )
		{
			m_FocusObj	=	this;
		}
		// windwo open event
		if ( E_GrP3ScptIdNone != m_ParaDoWin.EvtCreate )
		{
			V_GrP3Mng->VmRunCode( m_ParaDoWin.EvtCreate, this );
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::ProcEvtFocusGet( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::ProcEvtFocusLoss( void )
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::IsCanKey( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	m_ParaDoWin.IsKeyDirect;
		if ( m_ParaDoWin.IsInputBlock )
		{
			Tv_Result	=	FALSE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP3DoWin::LayerGet( void )
{
		return	m_Layer;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		Cls_GrP3DoBase*	Tv_Obj;
		St_GrP3MngVmRpara	Tv_Rpara;
	// code --------------------
		Tv_Result	=	FALSE;

		// check exist key
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DoWinKeyCnt;Tv_WkIdx++ )
		{
			if ( (m_ParaDoWin.KeyCodeTbl[Tv_WkIdx] == A_Key) )
			{
				if ( E_GrP3EvtKeyTypeDown == m_ParaDoWin.KeyTypeTbl[Tv_WkIdx] )
				{
					// event key down
					Tv_Rpara.Cnt	=	1;
					Tv_Rpara.Ridx[0]	=	1;
					Tv_Rpara.Rval[0]	=	(__s32)((__u32)A_Key);
					V_GrP3Mng->VmRunCode( m_ParaDoWin.KeyAsmTbl[Tv_WkIdx], this, &Tv_Rpara );
					if ( 0 != V_GrP3Mng->VmRegIntGet( 0 ) )
					{
						Tv_Result	=	TRUE;
					}
					break;
				}
				if ( E_GrP3EvtKeyTypeLong == m_ParaDoWin.KeyTypeTbl[Tv_WkIdx] )
				{
					// check long key able
					if ( 0 != m_LongKeyTime )
					{
						// update long key
						if ( m_KeyLast != A_Key )
						{
							m_KeyLongIdx	=	Tv_WkIdx;
							m_KeyLast	=	A_Key;
							m_KeyTime	=	0;
						}
					}
					Tv_Result	=	TRUE;
					break;
				}
			}
		}

		// check keep key
		if ( (m_KeyLast != A_Key) || (!Tv_Result) )
		{
			m_KeyLast		=	E_GrP3KeyNone;
			m_KeyTime		=	0;
		}

		// do original function
		if ( !Tv_Result )
		{
			Tv_Obj	=	((Cls_GrP3ConBase*)m_ObjCon)->FocusObjGet();
			if ( NULL != Tv_Obj )
			{
				Tv_Result	=	TRUE;
				switch ( A_Key )
				{
					case E_GrP3KeyLeft:
						Tv_Obj->MoveFocusByDir(E_GrP3DoFocusMvIdxLeft);
						break;
					case E_GrP3KeyUp:
						Tv_Obj->MoveFocusByDir(E_GrP3DoFocusMvIdxUp);
						break;
					case E_GrP3KeyRight:
						Tv_Obj->MoveFocusByDir(E_GrP3DoFocusMvIdxRight);
						break;
					case E_GrP3KeyDown:
						Tv_Obj->MoveFocusByDir(E_GrP3DoFocusMvIdxDown);
						break;
					default:
						Tv_Result	=	FALSE;
						break;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		__u8	Tv_EvtIdx;
		St_GrP3MngVmRpara	Tv_Rpara;
	// code --------------------
		Tv_Result	=	FALSE;

		// check exist key
		Tv_EvtIdx	=	0xFF;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DoWinKeyCnt;Tv_WkIdx++ )
		{
			if ( (m_ParaDoWin.KeyCodeTbl[Tv_WkIdx] == A_Key) )
			{
				if ( E_GrP3EvtKeyTypeUp == m_ParaDoWin.KeyTypeTbl[Tv_WkIdx] )
				{
					Tv_EvtIdx	=	Tv_WkIdx;
				}
				else if ( E_GrP3EvtKeyTypeLong == m_ParaDoWin.KeyTypeTbl[Tv_WkIdx] )
				{
					m_KeyLongIdx	=	0xFF;	// release key long
				}
			}
		}

		if ( 0xFF != Tv_EvtIdx )
		{
			// event key up
			Tv_Rpara.Cnt	=	1;
			Tv_Rpara.Ridx[0]	=	1;
			Tv_Rpara.Rval[0]	=	(__s32)((__u32)A_Key);
			V_GrP3Mng->VmRunCode( m_ParaDoWin.KeyAsmTbl[Tv_EvtIdx], this, &Tv_Rpara );
			if ( 0 != V_GrP3Mng->VmRegIntGet( 0 ) )
			{
				Tv_Result	=	TRUE;
			}
		}

		m_KeyLast		=	E_GrP3KeyNone;
		m_KeyTime		=	0;

		// default key action
		if ( !Tv_Result )
		{
			switch ( A_Key )
			{
				case E_GrP3KeyMenu:
				case E_GrP3KeyEsc:
					if ( m_ParaDoWin.IsEsckeyQuit )
					{
						if ( E_GrP3AsmIdNone != m_ParaDoWin.EvtReqClose )
						{
							V_GrP3Mng->VmRunCode( m_ParaDoWin.EvtReqClose, this );
						}
						else
						{
							WindowClose();
						}
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP3KeyEmergency:
					V_GrP3Mng->RtlTryRecEmgc( !V_GrP3Mng->IsRecEmgc() );
					break;
				case E_GrP3KeyEmgcyOn:
					V_GrP3Mng->RtlTryRecEmgc( TRUE );
					break;
				case E_GrP3KeyEmgcyOff:
					V_GrP3Mng->RtlTryRecEmgc( FALSE );
					break;
				case E_GrP3KeyLock:
					V_GrP3Mng->RtlLogOut();
					break;
			}
			
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::ProcEvtBtnDown( Cls_GrP3DoBase* A_Obj )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::ProcEvtBtnUp( Cls_GrP3DoBase* A_Obj )
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoWin::ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP3AsmIdNone != m_ParaDoWin.EvtMseRbtnUp )
		{
			V_GrP3Mng->VmRunCode( m_ParaDoWin.EvtMseRbtnUp, this );
			Tv_Result	=	TRUE;
		}

		if ( m_ParaDoWin.IsRbtnQuit )
		{
			if ( E_GrP3AsmIdNone != m_ParaDoWin.EvtReqClose )
			{
				V_GrP3Mng->VmRunCode( m_ParaDoWin.EvtReqClose, this );
			}
			else
			{
				WindowClose();
			}
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::ProcEvtSysStat( __u8 A_Type )
{
	// local -------------------
		St_GrP3MngVmRpara	Tv_Rpara;
	// code --------------------
		Cls_GrP3DoBase::ProcEvtSysStat( A_Type );
		if ( E_GrP3AsmIdNone != m_ParaDoWin.EvtSysStat )
		{
			Tv_Rpara.Cnt	=	1;
			Tv_Rpara.Ridx[0]	=	1;
			Tv_Rpara.Rval[0]	=	(__s32)((__u32)A_Type);
			V_GrP3Mng->VmRunCode( m_ParaDoWin.EvtSysStat, this, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoWin::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_KeyIdx;
		St_GrP3MngVmRpara	Tv_Rpara;
	// code --------------------
		// timer run
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DoWinTimerCnt;Tv_WkIdx++ )
		{
			if ( (0 != m_ParaDoWin.TimerPeriod[Tv_WkIdx]) && (E_GrP3AsmIdNone != m_ParaDoWin.EvtTimer[Tv_WkIdx]) )
			{
				// check timer time
				if ( 0 == m_TimerCntTbl[Tv_WkIdx] )
				{
					// run timer
					V_GrP3Mng->VmRunCode( m_ParaDoWin.EvtTimer[Tv_WkIdx], this );
					// update count
					m_TimerCntTbl[Tv_WkIdx]	=	m_ParaDoWin.TimerPeriod[Tv_WkIdx] * 10;
				}
				m_TimerCntTbl[Tv_WkIdx] --;
			}
		}

		// check long key timer
		if ( 0xFF != m_KeyLongIdx )
		{
			if ( m_LongKeyTime <= m_KeyTime )
			{
				// call 
				Tv_KeyIdx			=	m_KeyLongIdx;
				m_KeyLongIdx	=	0xFF;
				m_KeyTime			=	0;
				m_KeyLast			=	E_GrP3KeyNone;
				// run routine
				// event key down
				Tv_Rpara.Cnt	=	1;
				Tv_Rpara.Ridx[0]	=	1;
				Tv_Rpara.Rval[0]	=	(__s32)((__u32)m_KeyLast);
				V_GrP3Mng->VmRunCode( m_ParaDoWin.KeyAsmTbl[Tv_KeyIdx], this );
			}
			else
			{
				m_KeyTime ++;
			}
		}
		
}
//--------------------------------------------------------------------
