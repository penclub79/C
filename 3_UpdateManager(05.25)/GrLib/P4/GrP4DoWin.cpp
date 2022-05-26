/*
		platform window

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P4/GrP4DoWin.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <P4/GrP4Script.h>
#include <P4/GrP4ConBase.h>
#include <P4/GrP4AsmBase.h>
#include <P4/GrP4Mng.h>
#include <P4/GrP4UiCmd.h>
#include <P4/GrP4Cfg.h>
#include <GrDebug.h>

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

#ifdef GR_P4_EDITOR

#define E_GrP4DoWinParaCnt		72

St_GrP4DvlDoPrInfoData	V_GrP4DoWinParaInfo[E_GrP4DoWinParaCnt]	=	
{
	{	L"InputBlock",	TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4DoWinPara)0)->IsInputBlock,NULL,	L"No input any device"	},
	{	L"LayerLock",		TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4DoWinPara)0)->IsLayerLock,NULL,	L"Don't move focus to behind window"	},
	{	L"KeyDirect",		TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4DoWinPara)0)->IsKeyDirect,NULL,	L"Window access key. key can not access sub control"	},
	{	L"EscKeyQuit",	TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4DoWinPara)0)->IsEsckeyQuit,NULL,	L"Close window when press menu key or esc key"	},
	{	L"RbtnQuit",		TRUE,		E_GrP4DvlParaTypeBool8,				1,	(void*)&((Ptr_GrP4DoWinPara)0)->IsRbtnQuit,NULL,	L"Close window when press mouse right button"	},
	{	L"Paint BG",		TRUE,		E_GrP4DvlParaTypePntCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->PntBg,NULL,	L"Paint background code.\n0 = None, 1 = basic code"	},
	{	L"StartFocus",	TRUE,		E_GrP4DvlParaTypeScptIdAny,		1,	(void*)&((Ptr_GrP4DoWinPara)0)->StartFocus,NULL,	L"Start focus object id. you can input this window id."	},
	{	L"EventCreate",	TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->EvtCreate,NULL,	L"When window creat, this code will be run."	},
	{	L"EventClose",	TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->EvtClose,NULL,	L"When window close, this code will be run."	},
	{	L"EvtMseRbtnUp",	TRUE,		E_GrP4DvlParaTypeAsmCode,		1,	(void*)&((Ptr_GrP4DoWinPara)0)->EvtMseRbtnUp,NULL,	L"When mouse right button up, this code will be run."	},
	{	L"EvtReqClose",	TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->EvtReqClose,NULL,	L"When the mouse or key request quit,then this code will be run."	},
	{	L"EvtSysStat",	TRUE,		E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->EvtSysStat,NULL,	L"When system stauts changed,then this code will be run."	},

	{	L"Evt Key0",		TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[0],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key0 code",	FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[0],NULL,	L""	},
	{	L"Evt Key0 asm",	FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[0],NULL,	L""	},

	{	L"Evt Key1",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[1],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key1 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[1],NULL,	L""	},
	{	L"Evt Key1 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[1],NULL,	L""	},

	{	L"Evt Key2",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[2],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key2 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[2],NULL,	L""	},
	{	L"Evt Key2 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[2],NULL,	L""	},

	{	L"Evt Key3",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[3],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key3 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[3],NULL,	L""	},
	{	L"Evt Key3 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[3],NULL,	L""	},

	{	L"Evt Key4",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[4],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key4 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[4],NULL,	L""	},
	{	L"Evt Key4 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[4],NULL,	L""	},

	{	L"Evt Key5",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[5],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key5 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[5],NULL,	L""	},
	{	L"Evt Key5 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[5],NULL,	L""	},

	{	L"Evt Key6",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[6],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key6 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[6],NULL,	L""	},
	{	L"Evt Key6 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[6],NULL,	L""	},

	{	L"Evt Key7",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[7],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key7 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[7],NULL,	L""	},
	{	L"Evt Key7 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[7],NULL,	L""	},

	{	L"Evt Key8",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[8],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key8 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[8],NULL,	L""	},
	{	L"Evt Key8 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[8],NULL,	L""	},

	{	L"Evt Key9",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[9],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key9 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[9],NULL,	L""	},
	{	L"Evt Key9 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[9],NULL,	L""	},

	{	L"Evt Key10",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[10],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key10 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[10],NULL,	L""	},
	{	L"Evt Key10 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[10],NULL,	L""	},

	{	L"Evt Key11",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[11],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key11 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[11],NULL,	L""	},
	{	L"Evt Key11 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[11],NULL,	L""	},

	{	L"Evt Key12",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[12],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key12 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[12],NULL,	L""	},
	{	L"Evt Key12 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[12],NULL,	L""	},

	{	L"Evt Key13",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[13],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key13 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[13],NULL,	L""	},
	{	L"Evt Key13 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[13],NULL,	L""	},

	{	L"Evt Key14",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[14],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key14 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[14],NULL,	L""	},
	{	L"Evt Key14 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[14],NULL,	L""	},

	{	L"Evt Key15",	TRUE,		E_GrP4DvlParaTypeKeyEvtType,	1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyTypeTbl[15],NULL,	L"When key touch, this code will run."	},
	{	L"Evt Key15 code",						FALSE,	E_GrP4DvlParaTypeKeyCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyCodeTbl[15],NULL,	L""	},
	{	L"Evt Key15 asm",						FALSE,	E_GrP4DvlParaTypeAsmCode,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->KeyAsmTbl[15],NULL,	L""	},

	{	L"Timer0 Period",	TRUE,		E_GrP4DvlParaTypeWord,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->TimerPeriod[0],NULL,	L"Timer 0 period. sec unit"	},
	{	L"Event Timer0",	TRUE,		E_GrP4DvlParaTypeAsmCode,		1,	(void*)&((Ptr_GrP4DoWinPara)0)->EvtTimer[0],NULL,	L"Timer 0 event code"	},

	{	L"Timer1 Period",	TRUE,		E_GrP4DvlParaTypeWord,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->TimerPeriod[1],NULL,	L"Timer 1 period. sec unit"	},
	{	L"Event Timer1",	TRUE,		E_GrP4DvlParaTypeAsmCode,		1,	(void*)&((Ptr_GrP4DoWinPara)0)->EvtTimer[1],NULL,	L"Timer 1 event code"	},

	{	L"Timer2 Period",	TRUE,		E_GrP4DvlParaTypeWord,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->TimerPeriod[2],NULL,	L"Timer 2 period. sec unit"	},
	{	L"Event Timer2",	TRUE,		E_GrP4DvlParaTypeAsmCode,		1,	(void*)&((Ptr_GrP4DoWinPara)0)->EvtTimer[2],NULL,	L"Timer 2 event code"	},

	{	L"Timer3 Period",	TRUE,		E_GrP4DvlParaTypeWord,			1,	(void*)&((Ptr_GrP4DoWinPara)0)->TimerPeriod[3],NULL,	L"Timer 3 period. sec unit"	},
	{	L"Event Timer3",	TRUE,		E_GrP4DvlParaTypeAsmCode,		1,	(void*)&((Ptr_GrP4DoWinPara)0)->EvtTimer[3],NULL,	L"Timer 3 event code"	},

	{ L"PopupStyle", TRUE, E_GrP4DvlParaTypePopupStyle, 1, (void*)&((Ptr_GrP4DoWinPara)0)->PopupStyle, NULL, L"popup style" },
	{ L"PopupDir", TRUE, E_GrP4DvlParaTypeDir2d, 1, (void*)&((Ptr_GrP4DoWinPara)0)->PopupDir, NULL, L"popup direction" },
	{ L"PopupTime", TRUE, E_GrP4DvlParaTypeByte, 1, (void*)&((Ptr_GrP4DoWinPara)0)->PopupTime, NULL, L"popup time (0.1 sec unit)" },
	{ L"PopupLen", TRUE, E_GrP4DvlParaTypeShort, 1, (void*)&((Ptr_GrP4DoWinPara)0)->PopupLen, NULL, L"popup length (pixel unit)" },

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoWin::Cls_GrP4DoWin(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, __u8 A_Layer, void* A_Console, __u16 A_RefPosWin) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect, A_PosRel, NULL )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId	=	E_GrP4DoClassWin;

		m_ParaSize	=	m_ParaSize + sizeof( m_ParaDoWin ) + sizeof(St_GrP4ScptParaHd);
		
		m_Layer			=	A_Layer;

		m_RefPosWin		=	A_RefPosWin;

		m_IsPopupRun	=	FALSE;
		m_PopupPrgs		=	0;
		m_PtrWinScptRemain	=	NULL;

		m_KeyLast			=	E_GrP4KeyNone;
		m_KeyTime			=	0;

		m_IsDoModal			=	FALSE;
		m_ModalVm				=	0;
		m_ModalRslt			=	0;

		m_ObjCon				=	A_Console;

		m_FocusObj			=	NULL;

		GrDumyZeroMem( &m_ParaDoWin, sizeof(m_ParaDoWin) );
#ifdef GR_P4_EDITOR
		if ( (E_GrP4ScptIdPageBase > A_ScptId) || ((E_GrP4ScptIdPageBase + E_GrP4DvlPageMaxCnt) <= A_ScptId) )
		{
			m_ParaDoWin.PntBg	=	E_GrP4ScptPstIdDflt;
		}
#else
		m_ParaDoWin.PntBg	=	E_GrP4ScptPstIdDflt;
#endif
		GrDumyZeroMem( m_TimerCntTbl, sizeof(m_TimerCntTbl) );

		// register console
		m_WinId	=	((Cls_GrP4ConBase*)m_ObjCon)->RtlWinRegist( this, m_Layer );

		m_LongKeyTime	=	V_GrP4Mng->LongKeyTimeGet() * 10;
		m_KeyLongIdx	=	0xFF;
		m_KeyTime			=	0;

		// paint
		ReqDraw();
}
//--------------------------------------------------------------------
Cls_GrP4DoWin::~Cls_GrP4DoWin( void )
{
	// local -------------------
		St_GrP4MngVmRpara	Tv_Rpara;
	// code --------------------
		// close event
		if ( !((Cls_GrP4ConBase*)m_ObjCon)->IsShutDown() )
		{
			if ( E_GrP4ScptIdNone != m_ParaDoWin.EvtClose )
			{
				V_GrP4Mng->VmRunCode( m_ParaDoWin.EvtClose, this );
			}

			// update modal result
			if ( m_IsDoModal )
			{
				Tv_Rpara.In.Cnt		=	0;

				Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	0;
				Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	m_ModalRslt;
				Tv_Rpara.In.Cnt ++;

				Tv_Rpara.Out.Cnt	=	0;

				V_GrP4Mng->RtlVmWakeup(m_ModalVm, &Tv_Rpara);
			}
		}

}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoWin::LastFocusObjGet( void )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
	// code --------------------
		if ( m_ParaDoWin.IsKeyDirect )
		{
			Tv_Result	=	(Cls_GrP4DoBase*)this;
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
void	Cls_GrP4DoWin::RtlLastFocusObjSet( Cls_GrP4DoBase* A_Obj )
{
		m_FocusObj	=	A_Obj;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DoWin::WinIdGet( void )
{
		return	m_WinId;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::IsWinInputBlock( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	m_ParaDoWin.IsInputBlock;
		if (m_IsPopupRun)
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::IsWinMouseAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsVisible && m_ParaDoBase.IsEnable && (!m_IsPopupRun) )
		{
			Tv_Result	=	!m_ParaDoWin.IsInputBlock;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::IsWinKeyAble( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if (m_IsVisible && m_ParaDoBase.IsEnable && (!m_IsPopupRun))
		{
			Tv_Result	=	!m_ParaDoWin.IsInputBlock;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoWin::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassWin;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoWin);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoWin, sizeof(m_ParaDoWin) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoWin)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::IsWinKeyDirect( void )
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
void	Cls_GrP4DoWin::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		// update
		ParaCopyFromScpt( &m_ParaDoWin, sizeof(m_ParaDoWin) , m_PtrScpt , E_GrP4DoClassWin );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiWinClose:
				WindowClose();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::ProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		//default event handler
		switch( A_PtrMsg->Cmd )
		{
			case E_GrP4MsgCmdBtnDown:
				Tv_Obj	=	FindSlaveObjByScptId( ((Ptr_GrP4MsgObj)A_PtrMsg)->ObjId );
				if ( NULL != Tv_Obj )
				{
					ProcEvtBtnDown( Tv_Obj );
				}
				break;
			case E_GrP4MsgCmdBtnUp:
				Tv_Obj	=	FindSlaveObjByScptId( ((Ptr_GrP4MsgObj)A_PtrMsg)->ObjId );
				if ( NULL != Tv_Obj )
				{
					ProcEvtBtnUp( Tv_Obj );
				}
				break;
			default:
				Cls_GrP4DoBase::ProcUiMsg( A_PtrMsg );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::IsWinLayerLock( void )
{
		return	m_ParaDoWin.IsLayerLock;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoWin::FindMseObjByPos( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
		Cls_GrP4DoBase*	Tv_ObjFind;
		St_GrRect	Tv_RcBase;
	// code --------------------
		Tv_Result	=	NULL;
		// check can mouse input able
		if ( m_IsVisible && m_ParaDoBase.IsEnable && (!m_ParaDoWin.IsInputBlock) )
		{
			// get console rect
			Tv_RcBase.left		=	0;
			Tv_RcBase.top			=	0;
			Tv_RcBase.right		=	(__s32)((Cls_GrP4ConBase*)m_ObjCon)->GetResX();
			Tv_RcBase.bottom	=	(__s32)((Cls_GrP4ConBase*)m_ObjCon)->GetResY();

			// find slave
			Tv_ObjFind	=	RtlFindMseObjByPos(A_X, A_Y, &Tv_RcBase);
			if (NULL != Tv_ObjFind)
			{
				// update
				Tv_Result	=	Tv_ObjFind;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoWin::FindObjByPos( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
		Cls_GrP4DoBase*	Tv_ObjFind;
		St_GrRect	Tv_RcBase;
	// code --------------------
		Tv_Result	=	NULL;

		// get console rect
		Tv_RcBase.left		=	0;
		Tv_RcBase.top			=	0;
		Tv_RcBase.right		=	(__s32)((Cls_GrP4ConBase*)m_ObjCon)->GetResX();
		Tv_RcBase.bottom	=	(__s32)((Cls_GrP4ConBase*)m_ObjCon)->GetResY();

		Tv_ObjFind	=	RtlFindObjByPos(A_X, A_Y, &Tv_RcBase);
		if (NULL != Tv_ObjFind)
		{
			// update
			Tv_Result	=	Tv_ObjFind;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::WindowClose( void )
{
	// local -------------------
		St_GrP4MsgWinClose	Tv_Msg;
	// code --------------------
		// request to console
		Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdConWinClose;
		Tv_Msg.Hd.ConIdx	=	E_GrP4ConIdxFocus;
		Tv_Msg.Hd.WinId		=	E_GrP4WinIdNone;
		Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdNone;
		Tv_Msg.WinId			=	m_WinId;
		((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::IsDoingModal( void )
{
		return	m_IsDoModal;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::IsEnable( void )
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
BOOL8	Cls_GrP4DoWin::IsCanEnable( void )
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
void	Cls_GrP4DoWin::RtlModalDoingSet(BOOL8 A_IsDo, __u8 A_VmIdx)
{
		m_IsDoModal	=	A_IsDo;
		m_ModalVm		=	A_VmIdx;
}
//--------------------------------------------------------------------
Cls_GrP4DoBase*	Cls_GrP4DoWin::FindObjByScptId( __u16 A_ScptId )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		// check self
		if ( A_ScptId == m_ScptId	)
		{
			Tv_Result	=	(Cls_GrP4DoBase*)this;
		}
		else
		{
			// find slave
			Tv_Result	=	FindSlaveObjByScptId( A_ScptId );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrP4DoWin::ModalResultGet( void )
{
		return	m_ModalRslt;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::RtlModalResultSet( __s32 A_Val )
{
		m_ModalRslt		=	A_Val;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DoWin::EvtCloseCodeGet( void )
{
		return	m_ParaDoWin.EvtClose;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DoWin::EvtCreateCodeGet( void )
{
		return	m_ParaDoWin.EvtCreate;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::ProcPaint( void )
{
		PaintScriptPush( m_ParaDoWin.PntBg, &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoWin::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoWin, V_GrP4DoWinParaInfo, E_GrP4DoWinParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::RtlInit( void )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
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
		if ( E_GrP4ScptIdNone != m_ParaDoWin.EvtCreate )
		{
			V_GrP4Mng->VmRunCode( m_ParaDoWin.EvtCreate, this );
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::ProcEvtFocusGet( void )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::ProcEvtFocusLoss( void )
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::IsCanKey( void )
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
__u8	Cls_GrP4DoWin::LayerGet( void )
{
		return	m_Layer;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		Cls_GrP4DoBase*	Tv_Obj;
		St_GrP4MngVmRpara	Tv_Rpara;
	// code --------------------
		Tv_Result	=	FALSE;

		// check exist key
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DoWinKeyCnt;Tv_WkIdx++ )
		{
			if ( (m_ParaDoWin.KeyCodeTbl[Tv_WkIdx] == A_Key) )
			{
				if ( E_GrP4EvtKeyTypeDown == m_ParaDoWin.KeyTypeTbl[Tv_WkIdx] )
				{
					// event key down
					Tv_Rpara.In.Cnt		=	0;

					Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	1;
					Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	(__s32)((__u32)A_Key);
					Tv_Rpara.In.Cnt ++;

					Tv_Rpara.Out.Cnt	=	0;
					Tv_Rpara.Out.Ridx[Tv_Rpara.In.Cnt]	=	0;
					Tv_Rpara.Out.Rval[Tv_Rpara.In.Cnt]	=	0;
					Tv_Rpara.Out.Cnt ++;

					V_GrP4Mng->VmRunCode(m_ParaDoWin.KeyAsmTbl[Tv_WkIdx], this, &Tv_Rpara, TRUE);
					if (0 != Tv_Rpara.Out.Rval[0] )
					{
						Tv_Result	=	TRUE;
					}
					break;
				}
				if ( E_GrP4EvtKeyTypeLong == m_ParaDoWin.KeyTypeTbl[Tv_WkIdx] )
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
			m_KeyLast		=	E_GrP4KeyNone;
			m_KeyTime		=	0;
		}

		// do original function
		if ( !Tv_Result )
		{
			Tv_Obj	=	((Cls_GrP4ConBase*)m_ObjCon)->FocusObjGet();
			if ( NULL != Tv_Obj )
			{
				Tv_Result	=	TRUE;
				switch ( A_Key )
				{
					case E_GrP4KeyLeft:
						Tv_Obj->MoveFocusByDir(E_GrP4DoFocusMvIdxLeft);
						break;
					case E_GrP4KeyUp:
						Tv_Obj->MoveFocusByDir(E_GrP4DoFocusMvIdxUp);
						break;
					case E_GrP4KeyRight:
						Tv_Obj->MoveFocusByDir(E_GrP4DoFocusMvIdxRight);
						break;
					case E_GrP4KeyDown:
						Tv_Obj->MoveFocusByDir(E_GrP4DoFocusMvIdxDown);
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
BOOL8	Cls_GrP4DoWin::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		__u8	Tv_EvtIdx;
		St_GrP4MngVmRpara	Tv_Rpara;
	// code --------------------
		Tv_Result	=	FALSE;

		// check exist key
		Tv_EvtIdx	=	0xFF;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DoWinKeyCnt;Tv_WkIdx++ )
		{
			if ( (m_ParaDoWin.KeyCodeTbl[Tv_WkIdx] == A_Key) )
			{
				if ( E_GrP4EvtKeyTypeUp == m_ParaDoWin.KeyTypeTbl[Tv_WkIdx] )
				{
					Tv_EvtIdx	=	Tv_WkIdx;
				}
				else if ( E_GrP4EvtKeyTypeLong == m_ParaDoWin.KeyTypeTbl[Tv_WkIdx] )
				{
					m_KeyLongIdx	=	0xFF;	// release key long
				}
			}
		}

		if ( 0xFF != Tv_EvtIdx )
		{
			// event key up
			Tv_Rpara.In.Cnt		=	0;

			Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	1;
			Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	(__s32)((__u32)A_Key);
			Tv_Rpara.In.Cnt ++;

			Tv_Rpara.Out.Cnt	=	0;
			Tv_Rpara.Out.Ridx[Tv_Rpara.In.Cnt]	=	0;
			Tv_Rpara.Out.Rval[Tv_Rpara.In.Cnt]	=	0;
			Tv_Rpara.Out.Cnt ++;

			V_GrP4Mng->VmRunCode( m_ParaDoWin.KeyAsmTbl[Tv_EvtIdx], this, &Tv_Rpara,TRUE );
			if ( 0 != Tv_Rpara.Out.Rval[0] )
			{
				Tv_Result	=	TRUE;
			}
		}

		m_KeyLast		=	E_GrP4KeyNone;
		m_KeyTime		=	0;

		// default key action
		if ( !Tv_Result )
		{
			switch ( A_Key )
			{
				case E_GrP4KeyMenu:
				case E_GrP4KeyEsc:
					if ( m_ParaDoWin.IsEsckeyQuit )
					{
						if ( E_GrP4AsmIdNone != m_ParaDoWin.EvtReqClose )
						{
							V_GrP4Mng->VmRunCode( m_ParaDoWin.EvtReqClose, this );
						}
						else
						{
							WindowClose();
						}
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyEmergency:
					V_GrP4Mng->RtlTryRecEmgc( !V_GrP4Mng->IsRecEmgc() );
					break;
				case E_GrP4KeyEmgcyOn:
					V_GrP4Mng->RtlTryRecEmgc( TRUE );
					break;
				case E_GrP4KeyEmgcyOff:
					V_GrP4Mng->RtlTryRecEmgc( FALSE );
					break;
				case E_GrP4KeyLock:
					V_GrP4Mng->RtlLogOut();
					break;
			}
			
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::ProcEvtBtnDown( Cls_GrP4DoBase* A_Obj )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::ProcEvtBtnUp( Cls_GrP4DoBase* A_Obj )
{

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP4AsmIdNone != m_ParaDoWin.EvtMseRbtnUp )
		{
			V_GrP4Mng->VmRunCode( m_ParaDoWin.EvtMseRbtnUp, this );
			Tv_Result	=	TRUE;
		}

		if ( m_ParaDoWin.IsRbtnQuit )
		{
			if ( E_GrP4AsmIdNone != m_ParaDoWin.EvtReqClose )
			{
				V_GrP4Mng->VmRunCode( m_ParaDoWin.EvtReqClose, this );
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
void	Cls_GrP4DoWin::ProcEvtSysStat( __u8 A_Type )
{
	// local -------------------
		St_GrP4MngVmRpara	Tv_Rpara;
	// code --------------------
		Cls_GrP4DoBase::ProcEvtSysStat( A_Type );
		if ( E_GrP4AsmIdNone != m_ParaDoWin.EvtSysStat )
		{
			Tv_Rpara.In.Cnt		=	0;

			Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	1;
			Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	(__s32)((__u32)A_Type);
			Tv_Rpara.In.Cnt ++;

			Tv_Rpara.Out.Cnt	=	0;

			V_GrP4Mng->VmRunCode( m_ParaDoWin.EvtSysStat, this, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::LcProcTmrWinTimer(void)
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		// timer run
		for (Tv_WkIdx=0; Tv_WkIdx < E_GrP4DoWinTimerCnt; Tv_WkIdx++)
		{
			if ((0 != m_ParaDoWin.TimerPeriod[Tv_WkIdx]) && (E_GrP4AsmIdNone != m_ParaDoWin.EvtTimer[Tv_WkIdx]))
			{
				// check timer time
				if (0 == m_TimerCntTbl[Tv_WkIdx])
				{
					// run timer
					V_GrP4Mng->VmRunCode(m_ParaDoWin.EvtTimer[Tv_WkIdx], this);
					// update count
					m_TimerCntTbl[Tv_WkIdx]	=	m_ParaDoWin.TimerPeriod[Tv_WkIdx] * 10;
				}
				m_TimerCntTbl[Tv_WkIdx] --;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::LcProcTmrLongKey(void)
{
	// local -------------------
		__u8	Tv_KeyIdx;
		St_GrP4MngVmRpara	Tv_Rpara;
	// code --------------------
		// check long key timer
		if (0xFF != m_KeyLongIdx)
		{
			if (m_LongKeyTime <= m_KeyTime)
			{
				// call 
				Tv_KeyIdx			=	m_KeyLongIdx;
				m_KeyLongIdx	=	0xFF;
				m_KeyTime			=	0;
				m_KeyLast			=	E_GrP4KeyNone;
				// run routine
				// event key down
				Tv_Rpara.In.Cnt		=	0;

				Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	1;
				Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	(__s32)((__u32)m_KeyLast);
				Tv_Rpara.In.Cnt ++;

				Tv_Rpara.Out.Cnt	=	0;

				V_GrP4Mng->VmRunCode(m_ParaDoWin.KeyAsmTbl[Tv_KeyIdx], this, &Tv_Rpara);
			}
			else
			{
				m_KeyTime ++;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
	// code --------------------
		if (m_IsPopupRun)
		{
			LcProcTmrPopup();
		}
		else
		{
			LcProcTmrWinTimer();
			LcProcTmrLongKey();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::RtlWinScptDec(void* A_PtrScpt)
{
	// local -------------------
#ifdef GR_P4_EDITOR
		BOOL8	Tv_IsDecNow;
#endif
	// code --------------------
		// init
		m_IsPopupRun	=	FALSE;
		// decode this
		m_PtrWinScptRemain	=	V_GrP4Mng->UiScptDecThis(this, A_PtrScpt);

#ifdef GR_P4_EDITOR
		Tv_IsDecNow	=	TRUE;
		if ( ((Cls_GrP4DvlMng*)V_GrP4Mng)->DvlIsDbgRun() )
		{
			Tv_IsDecNow	=	FALSE;
			if ((E_GrP4PopupStyleImmediate == m_ParaDoWin.PopupStyle) || (0 == m_ParaDoWin.PopupLen))
			{
				Tv_IsDecNow	=	TRUE;
			}
		}
#endif

		// check popup stype
#ifdef GR_P4_EDITOR
		if (Tv_IsDecNow)
#else
		if ((E_GrP4PopupStyleImmediate == m_ParaDoWin.PopupStyle) || (0 == m_ParaDoWin.PopupLen))
#endif
		{
			V_GrP4Mng->UiScptDecRemain(this, m_PtrWinScptRemain);

			// call init
			RtlInit();
			// request focus change
			((Cls_GrP4ConBase*)m_ObjCon)->FocusReset();
			((Cls_GrP4ConBase*)m_ObjCon)->FocusCorrect();
		}
		else
		{
			// init
			m_IsPopupRun	=	TRUE;
			m_PopupPrgs		=	0;
			m_PopupSrRc		=	m_PosRect;
			m_PopupTgRc		=	m_PosRect;
			if (E_GrP4PopupStyleMove == m_ParaDoWin.PopupStyle)
			{
				switch (m_ParaDoWin.PopupDir)
				{
					case E_GrP4Dir2dLeft:
						m_PopupSrRc.left		=	m_PosRect.left + (__s32)((__u32)m_ParaDoWin.PopupLen);
						m_PopupSrRc.right		=	m_PosRect.right + (__s32)((__u32)m_ParaDoWin.PopupLen);
						break;
					case E_GrP4Dir2dRight:
						m_PopupSrRc.left		=	m_PosRect.left - (__s32)((__u32)m_ParaDoWin.PopupLen);
						m_PopupSrRc.right		=	m_PosRect.right - (__s32)((__u32)m_ParaDoWin.PopupLen);
						break;
					case E_GrP4Dir2dUp:
						m_PopupSrRc.top			=	m_PosRect.top + (__s32)((__u32)m_ParaDoWin.PopupLen);
						m_PopupSrRc.bottom	=	m_PosRect.bottom + (__s32)((__u32)m_ParaDoWin.PopupLen);
						break;
					default:
						m_PopupSrRc.top			=	m_PosRect.top - (__s32)((__u32)m_ParaDoWin.PopupLen);
						m_PopupSrRc.bottom	=	m_PosRect.bottom - (__s32)((__u32)m_ParaDoWin.PopupLen);
						break;
				}
			}
			else
			{
				switch (m_ParaDoWin.PopupDir)
				{
					case E_GrP4Dir2dLeft:
						if (0 == (m_PosRel & 1))
						{
							m_PopupSrRc.left		=	m_PosRect.left + (__s32)((__u32)m_ParaDoWin.PopupLen);
						}
						else
						{
							m_PopupSrRc.left		=	m_PosRect.left - (__s32)((__u32)m_ParaDoWin.PopupLen);
						}
						break;
					case E_GrP4Dir2dRight:
						if (0 == (m_PosRel & 0x10))
						{
							m_PopupSrRc.right		=	m_PosRect.right - (__s32)((__u32)m_ParaDoWin.PopupLen);
						}
						else
						{
							m_PopupSrRc.right		=	m_PosRect.right + (__s32)((__u32)m_ParaDoWin.PopupLen);
						}
						break;
					case E_GrP4Dir2dUp:
						if (0 == (m_PosRel & 0x04))
						{
							m_PopupSrRc.top			=	m_PosRect.top + (__s32)((__u32)m_ParaDoWin.PopupLen);
						}
						else
						{
							m_PopupSrRc.top			=	m_PosRect.top - (__s32)((__u32)m_ParaDoWin.PopupLen);
						}
						break;
					default:
						if (0 == (m_PosRel & 0x40))
						{
							m_PopupSrRc.bottom	=	m_PosRect.bottom - (__s32)((__u32)m_ParaDoWin.PopupLen);
						}
						else
						{
							m_PopupSrRc.bottom	=	m_PosRect.bottom + (__s32)((__u32)m_ParaDoWin.PopupLen);
						}
						break;
				}
			}

			// update rect
			m_PosRect		=	m_PopupSrRc;
			ReqDraw();
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DoWin::LcProcTmrPopup(void)
{
	// local -------------------
	// code --------------------
		// draw previous
		ReqDraw();
		// update progress
		m_PopupPrgs ++;
		if (m_PopupPrgs >= m_ParaDoWin.PopupTime)
		{
			m_PosRect			=	m_PopupTgRc;
			m_IsPopupRun	=	FALSE;
			// decode slave
			V_GrP4Mng->UiScptDecRemain(this, m_PtrWinScptRemain);

			// call init
			RtlInit();
			// request focus change
			((Cls_GrP4ConBase*)m_ObjCon)->FocusReset();
			((Cls_GrP4ConBase*)m_ObjCon)->FocusCorrect();
		}
		else
		{
			// calculate
			m_PosRect.left		=	m_PopupSrRc.left + (((m_PopupTgRc.left - m_PopupSrRc.left) * (__s32)((__u32)m_PopupPrgs)) / (__s32)((__u32)m_ParaDoWin.PopupTime));
			m_PosRect.top			=	m_PopupSrRc.top + (((m_PopupTgRc.top - m_PopupSrRc.top) * (__s32)((__u32)m_PopupPrgs)) / (__s32)((__u32)m_ParaDoWin.PopupTime));
			m_PosRect.right		=	m_PopupSrRc.right + (((m_PopupTgRc.right - m_PopupSrRc.right) * (__s32)((__u32)m_PopupPrgs)) / (__s32)((__u32)m_ParaDoWin.PopupTime));
			m_PosRect.bottom	=	m_PopupSrRc.bottom + (((m_PopupTgRc.bottom - m_PopupSrRc.bottom) * (__s32)((__u32)m_PopupPrgs)) / (__s32)((__u32)m_ParaDoWin.PopupTime));
		}

		// draw now
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::RtlKeyDownProc(__u8 A_Dev, __u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	ProcEvtKeyDown(A_Dev, A_Key);
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::RtlKeyUpProc(__u8 A_Dev, __u8 A_Key)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	ProcEvtKeyUp(A_Dev, A_Key);

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::RtlMseRbtnUpProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	ProcEvtMseRbtnUp(A_X, A_Y, A_BtnMap);

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoWin::ScrRectGet(Ptr_GrRect A_PtrRtRect, BOOL8 A_IsForceVisible)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsCon;
		Cls_GrP4DoWin*	Tv_WinRef;
		St_GrRect	Tv_RcBase;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check visible
		if (m_IsVisible || A_IsForceVisible)
		{
			Tv_IsCon	=	TRUE;
			if (E_GrP4ScptIdNone != m_RefPosWin )
			{
				if (m_ScptId != m_RefPosWin)
				{
					// get window
					Tv_WinRef	=	((Cls_GrP4ConBase*)m_ObjCon)->WinFindByScptId(m_RefPosWin);
					if (NULL != Tv_WinRef)
					{
						if (Tv_WinRef->ScrRectGet(&Tv_RcBase, A_IsForceVisible))
						{
							if (BasisRectCalc(&Tv_RcBase, m_PosRel, (__s16)m_PosRect.left, (__s16)m_PosRect.top, (__s16)m_PosRect.right, (__s16)m_PosRect.bottom, A_PtrRtRect))
							{
								Tv_Result	=	TRUE;
								Tv_IsCon	=	FALSE;

								// update
								m_PosRel		=	0;
								m_PosRect		=	*A_PtrRtRect;
								m_RefPosWin	=	E_GrP4ScptIdNone;
							}
						}
					}
				}
				else
				{
					m_RefPosWin	=	E_GrP4ScptIdNone;
					DbgMsgPrint("Cls_GrP4DoWin::ScrRectGet - Window (%d) can not reference myself!\n", m_ScptId);
				}
			}

			if (Tv_IsCon)
			{
				// get console rect
				Tv_RcBase.left		=	0;
				Tv_RcBase.top			=	0;
				Tv_RcBase.right		=	(__s32)((Cls_GrP4ConBase*)m_ObjCon)->GetResX();
				Tv_RcBase.bottom	=	(__s32)((Cls_GrP4ConBase*)m_ObjCon)->GetResY();

				Tv_Result	=	BasisRectCalc(&Tv_RcBase, m_PosRel, (__s16)m_PosRect.left, (__s16)m_PosRect.top, (__s16)m_PosRect.right, (__s16)m_PosRect.bottom, A_PtrRtRect);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
