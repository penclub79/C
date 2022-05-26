/*
 user interface manager

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiMng.h>
#include	<GrDebug.h>
#include	<stdlib.h>
#include	<Ui/GrUiBase.h>
#include	<GrTimeTool.h>
#include	<Ui/GrUiDoBase.h>
#include	<Ui/GrUiDoWin.h>
#include	<GrDumyTool.h>

//====================================================================
//local const
#define E_GrUiMngTickEvtPeriod	100		/* 1/10 sec */
#define E_GrUiMngTickRtmBtnUp		300		/* remote button up gap */
#define E_GrUiMngTickMseCusOff	10000		/* mouse auto cusor off time */

#define E_GrUiMngMsgRestTime		10

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

Cls_GrUiMng*		V_GrUiMngObj	=	NULL;
BOOL8					V_IsGrUiMngPalMode;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiMng::Cls_GrUiMng( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		m_TaskSvr				=	A_TaskSvr;
		m_FocusConIdx		=	0;
		m_MseBtnMap			=	0;
		m_MseX					=	0;
		m_MseY					=	0;
		m_IsFcsAtChg		=	FALSE;
		m_Language			=	0;				//language index
		//m_IsUsrEvtAble	=	TRUE;			//user envent able
		m_PtrLangKeyTbl	=	NULL;
		m_LangKeyCnt		=	0;

		m_UsrEvtLvl			=	0;
		V_IsGrUiMngPalMode	=	FALSE;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiMngMaxConsoleCnt;Tv_WkIdx++ )
		{
			m_ConTbl[Tv_WkIdx]	=	NULL;
		}
		GrDumyZeroMem( m_ImgAlocTbl, sizeof(m_ImgAlocTbl) );
		GrDumyZeroMem( m_GlbVarTbl, sizeof(m_GlbVarTbl) );

		m_IsKeyOneSkip	=	FALSE;

		m_IsInputAble		=	TRUE;

		m_IsPasswdChk		=	FALSE;
		GrDumyZeroMem( m_PassErrTxt, sizeof(m_PassErrTxt) );

		//script decoder
		m_ScptDec	=	(Cls_GrUiScptDec*) new Cls_GrUiScptDec();

		//regist server
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrUiMngTickEvtPeriod, &m_HndlTimer );
}
//--------------------------------------------------------------------
Cls_GrUiMng::~Cls_GrUiMng()
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		//unregist server
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );
		m_TaskSvr->UnregistMsgClient( this );

		//release console
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiMngMaxConsoleCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_WkIdx] )
			{
				m_ConTbl[Tv_WkIdx]	=	NULL;
			}
		}

		//release script decoder
		delete	m_ScptDec;
}
//--------------------------------------------------------------------
BOOL8	GrUiMngInit( Cls_GrTaskSvr* A_TaskSvr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( NULL == V_GrUiMngObj )
		{
			V_GrUiMngObj	=	(Cls_GrUiMng*)new Cls_GrUiMng( A_TaskSvr );
			Tv_Result			=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrUiMngRelease( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( NULL != V_GrUiMngObj )
		{
			delete	V_GrUiMngObj;
			V_GrUiMngObj	=	NULL;
			Tv_Result			=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrUiConsole*	Cls_GrUiMng::GetConsoleObj( __u8 A_Idx )
{
	// local -------------------
		Cls_GrUiConsole*	Tv_Result;
		__u32		Tv_Idx;
	// code --------------------
		//init
		Tv_Result	=	NULL;	

		//patch index
		Tv_Idx		=	A_Idx;
		if ( E_GrUiConIdxFocused == A_Idx )
		{
			Tv_Idx	=	m_FocusConIdx;
		}

		//check range
		if ( E_GrUiMngMaxConsoleCnt > Tv_Idx )
		{
			//get
			Tv_Result		=	m_ConTbl[Tv_Idx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::ConsoleRegist( Cls_GrUiConsole* A_ObjCon, __u8 A_Idx )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check index
		if ( E_GrUiMngMaxConsoleCnt > A_Idx )
		{
			m_ConTbl[A_Idx]	=	A_ObjCon;
			m_ConTbl[A_Idx]->IndexSet( A_Idx );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::ConsoleUnregist( Cls_GrUiConsole* A_ObjCon )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check index
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiMngMaxConsoleCnt;Tv_WkIdx++ )
		{
			if ( A_ObjCon == m_ConTbl[Tv_WkIdx] )
			{
				//remove
				m_ConTbl[Tv_WkIdx]	=	NULL;
				Tv_Result	=	TRUE;
				break;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8		Tv_Result;
		void*		Tv_PtrPara;
	// code --------------------
		//init
		Tv_Result		=	TRUE;
		Tv_PtrPara	=	(void*)( (__u32)A_PtrMsg + sizeof(St_GrTaskMsgHd) );
		switch( A_PtrMsg->Cmd )
		{
			case	E_GrTaskMsgUiMsg:
				LcProcMsgUiMsg( (Ptr_GrUiMsgHd)Tv_PtrPara );
				break;
			case	E_GrTaskMsgQuadChgReq:
				LcProcMsgQuadChg( (Ptr_GrTaskMsgQuadChgReq)A_PtrMsg );
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::LcTickEvtBrdcst( __u32 A_Tick )
{
	// local -------------------
		St_GrUiMsgTimeTick	Tv_UiMsg;
	// code --------------------
		//broadcast tick
		Tv_UiMsg.Hd.Cmd				=	E_GrUiMsgCmdTickPeriod;
		Tv_UiMsg.Hd.WinId			=	E_GrUiWinIdAll;
		Tv_UiMsg.Hd.ObjId			=	E_GrUiObjIdBroadcast;
		Tv_UiMsg.Tick					=	A_Tick;
		LcUiMsgBrdcstCon( (Ptr_GrUiMsgHd)&Tv_UiMsg );
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
		__u32		Tv_Tick;
	// code --------------------
		//get tick
		Tv_Tick	=	GrTimeGetTick();
		//broadcast tick
		LcTickEvtBrdcst( Tv_Tick );
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::ConsoleSetFocus( __u8 A_Idx )
{
	// local -------------------
		__u8		Tv_WkIdx;
		St_GrUiMsgConFcsChg	Tv_UiMsg;
	// code --------------------
		if ( E_GrUiMngMaxConsoleCnt > A_Idx )
		{
			//check exist console
			if ( NULL != m_ConTbl[A_Idx] )
			{
				//set focus
				m_FocusConIdx	=	A_Idx;
				Tv_UiMsg.Hd.Cmd			=	E_GrUiMsgCmdConChgConFcs;
				Tv_UiMsg.Hd.WinId		=	E_GrUiWinIdNone;				//no id
				Tv_UiMsg.Hd.ObjId		=	E_GrUiObjIdNone;
				//change focus message broadcast
				for (Tv_WkIdx=0;Tv_WkIdx < E_GrUiMngMaxConsoleCnt;Tv_WkIdx++)
				{
					//check exist console
					if ( NULL != m_ConTbl[Tv_WkIdx] )
					{
						Tv_UiMsg.Hd.ConIdx	=	Tv_WkIdx;
						Tv_UiMsg.IsFocus		=	FALSE;
						if ( m_FocusConIdx == Tv_WkIdx )
						{
							Tv_UiMsg.IsFocus	=	TRUE;
						}
						UiMsgPost( (Ptr_GrUiMsgHd)&Tv_UiMsg, sizeof(Tv_UiMsg) );
					}
				}
				
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::ConsoleChgFocusNext( BOOL8 A_IsRotate )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_FcsIdx;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		Tv_FcsIdx	=	(__s32)m_FocusConIdx;
		Tv_FcsIdx ++;
		//patch console focus index
		for ( Tv_WkIdx=0;Tv_WkIdx < (E_GrUiMngMaxConsoleCnt - 1);Tv_WkIdx++ )
		{
			//patch
			if ( E_GrUiMngMaxConsoleCnt <= Tv_FcsIdx )
			{
				Tv_FcsIdx		=	0;
				if ( !A_IsRotate )
				{
					break;
				}
			}
			//check console exist
			if ( NULL != m_ConTbl[Tv_FcsIdx] )
			{
				//finded
				ConsoleSetFocus( Tv_FcsIdx );
				Tv_Result			=	TRUE;
				break;
			}
			//next
			Tv_FcsIdx ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::ConsoleChgFocusPrev( BOOL8 A_IsRotate )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_FcsIdx;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		Tv_FcsIdx	=	(__s32)m_FocusConIdx;
		Tv_FcsIdx --;
		//patch console focus index
		for ( Tv_WkIdx=0;Tv_WkIdx < (E_GrUiMngMaxConsoleCnt - 1);Tv_WkIdx++ )
		{
			//patch focus index
			if ( 0 > Tv_FcsIdx )
			{
				Tv_FcsIdx	=	E_GrUiMngMaxConsoleCnt - 1;
				if ( !A_IsRotate )
				{
					break;
				}
			}
			//check console exist
			if ( NULL != m_ConTbl[Tv_FcsIdx] )
			{
				//finded
				ConsoleSetFocus( Tv_FcsIdx );
				Tv_Result			=	TRUE;
				break;
			}
			//next
			Tv_FcsIdx --;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::InputMousePos( __s32 A_DtX, __s32 A_DtY, BOOL8 A_IsAbs )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
		BOOL8	Tv_IsChgFcs;
		St_GrUiMsgMouse		Tv_UiMsg;
	// code --------------------

		//check able
		if ( (NULL != m_ConTbl[m_FocusConIdx]) && m_IsInputAble )
		{
			Tv_X	=	A_DtX;
			Tv_Y	=	A_DtY;
			if ( !A_IsAbs )
			{
				Tv_X	=	Tv_X + m_MseX;
				Tv_Y	=	Tv_Y + m_MseY;
			}
			Tv_IsChgFcs	=	FALSE;
			//check now focus console
			if ( 0 > Tv_X )
			{
				//auto focus change
				if ( m_IsFcsAtChg && (0 == m_MseBtnMap) )
				{
					Tv_IsChgFcs	=	ConsoleChgFocusPrev( FALSE );
				}
				//change focus
				if ( Tv_IsChgFcs )
				{
					Tv_X	=	(__s32)m_ConTbl[m_FocusConIdx]->GetResX() - 1;
				}
				else
				{
					Tv_X	=	0;
				}
			}
			else if ( Tv_X >= (__s32)m_ConTbl[m_FocusConIdx]->GetResX() )
			{
				//auto focus change
				if ( m_IsFcsAtChg && (0 == m_MseBtnMap) )
				{
					Tv_IsChgFcs	=	ConsoleChgFocusNext( FALSE );
				}
				//change focus
				if ( Tv_IsChgFcs )
				{
					Tv_X	=	0;
				}
				else
				{
					Tv_X	=	(__s32)m_ConTbl[m_FocusConIdx]->GetResX() - 1;
				}
			}
			//patch positon y
			if ( Tv_Y < 0 )
			{
				Tv_Y		=	0;
			}
			else if ( Tv_Y >= (__s32)m_ConTbl[m_FocusConIdx]->GetResY()  )
			{
				Tv_Y	=	(__s32)m_ConTbl[m_FocusConIdx]->GetResY() - 1;
			}
			//check change position
			if ( (m_MseX != (__u16)Tv_X) || (m_MseY != (__u16)Tv_Y ) )
			{
				//update cus pos
				m_MseX		=	(__u16)Tv_X;
				m_MseY		=	(__u16)Tv_Y;
				//make message
				Tv_UiMsg.Hd.Cmd			=	E_GrUiMsgCmdConInpMouse;
				Tv_UiMsg.Hd.ConIdx	=	m_FocusConIdx;
				Tv_UiMsg.Hd.WinId		=	E_GrUiWinIdNone;		//console message
				Tv_UiMsg.Hd.ObjId		=	E_GrUiObjIdNone;
				Tv_UiMsg.BtnMap			=	m_MseBtnMap;
				Tv_UiMsg.X					=	m_MseX;
				Tv_UiMsg.Y					=	m_MseY;
				Tv_UiMsg.Atb				=	E_GrUiMouseAtbPosChg;

				UiMsgPost( (Ptr_GrUiMsgHd)&Tv_UiMsg, sizeof(Tv_UiMsg) );
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrUiMng::InputMouseBtn( __u8 A_BtnIdx, BOOL8 A_IsPush )
{
	// local -------------------
		St_GrUiMsgMouse		Tv_UiMsg;
	// code --------------------
		if ( m_IsInputAble )
		{
			//update button status
			if ( A_IsPush )
			{
				m_MseBtnMap	=	m_MseBtnMap | (1 << A_BtnIdx);
			}
			else
			{
				m_MseBtnMap	=	m_MseBtnMap & ~(1 << A_BtnIdx);
			}

			//make message
			Tv_UiMsg.Hd.Cmd			=	E_GrUiMsgCmdConInpMouse;
			Tv_UiMsg.Hd.ConIdx	=	m_FocusConIdx;
			Tv_UiMsg.Hd.WinId		=	E_GrUiWinIdNone;		//console message
			Tv_UiMsg.Hd.ObjId		=	E_GrUiObjIdNone;
			Tv_UiMsg.BtnMap			=	m_MseBtnMap;
			Tv_UiMsg.X					=	m_MseX;
			Tv_UiMsg.Y					=	m_MseY;
			Tv_UiMsg.Atb				=	1 << A_BtnIdx;

			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_UiMsg, sizeof(Tv_UiMsg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::InputMouseWheel( BOOL8 A_IsDown )
{
	// local -------------------
		St_GrUiMsgWheel		Tv_UiMsg;
	// code --------------------
		if ( (NULL != m_ConTbl[m_FocusConIdx]) && m_IsInputAble )
		{
			Tv_UiMsg.Hd.Cmd			=	E_GrUiMsgCmdConInpWheel;
			Tv_UiMsg.Hd.ConIdx	=	m_FocusConIdx;
			Tv_UiMsg.Hd.WinId		=	E_GrUiWinIdNone;
			Tv_UiMsg.Hd.ObjId		=	E_GrUiObjIdNone;
			Tv_UiMsg.IsDown			=	A_IsDown;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_UiMsg, sizeof(Tv_UiMsg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::InputKey( __u8 A_Key, BOOL8 A_IsPush )
{
	// local -------------------
		St_GrUiMsgKey		Tv_UiMsg;
	// code --------------------
		//check able key
		if ( (0 != A_Key) && m_IsInputAble )
		{
			//manager key process
			switch ( A_Key )
			{
				case E_GrUiKeyDisplay:
					if ( m_IsFcsAtChg && A_IsPush )
					{
						ConsoleChgFocusNext( TRUE );
					}
					break;
				default:
					//check request skip
					if ( m_IsKeyOneSkip )
					{
						if ( !A_IsPush )
						{
							//release skip
							m_IsKeyOneSkip	=	FALSE;
						}
					}
					else
					{
						if ( NULL != m_ConTbl[m_FocusConIdx] )
						{
							//make message
							Tv_UiMsg.Hd.Cmd			=	E_GrUiMsgCmdConInpKey;
							Tv_UiMsg.Hd.WinId		=	E_GrUiWinIdNone;				//console command mode
							Tv_UiMsg.Hd.ObjId		=	E_GrUiObjIdNone;
							Tv_UiMsg.Hd.ConIdx	=	m_FocusConIdx;
							Tv_UiMsg.IsPush			=	A_IsPush;
							Tv_UiMsg.Key				=	A_Key;
							UiMsgPost( (Ptr_GrUiMsgHd)&Tv_UiMsg, sizeof(Tv_UiMsg) );
						}
					}
					break;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::MousePosGet( __u16* A_PtrRtX, __u16* A_PtrRtY )
{
		*A_PtrRtX		=	m_MseX;
		*A_PtrRtY		=	m_MseY;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiMng::MouseBtnMapGet( void )
{
		return	m_MseBtnMap;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::TextGet( __u32 A_Id, WCHAR* A_StrRt, __u32 A_LenMax, __u32 A_Language )
{
	// local -------------------
		__u32		Tv_Lng;
	// code --------------------
		Tv_Lng		=	A_Language;
		if ( E_GrUiLangIdAuto == Tv_Lng )
		{
			Tv_Lng		=	m_Language;
		}
		return	m_ScptDec->TextGet( Tv_Lng, A_Id, A_StrRt, A_LenMax );
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrUiMng::TextPtrGet( __u32 A_Id, __u32 A_Language )
{
	// local -------------------
		__u32		Tv_Lng;
	// code --------------------
		Tv_Lng		=	A_Language;
		if ( E_GrUiLangIdAuto == Tv_Lng )
		{
			Tv_Lng		=	m_Language;
		}
		return	m_ScptDec->TextPtrGet( Tv_Lng, A_Id );
}
//--------------------------------------------------------------------
Cls_GrUiFontObj*	Cls_GrUiMng::FontGet( __u8 A_FontId )
{
		return	m_ScptDec->FontGet( A_FontId );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::ScptSet( void* A_PtrScpt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	m_ScptDec->ScriptSet( (Ptr_GrUiScptHd)A_PtrScpt );
		if ( Tv_Result )
		{
			//update pallete
			if ( m_ScptDec->IsDispPltMode() )
			{
				GrDumyCopyMem( GrGdibGetBasePalletePtr(), m_ScptDec->PalleteTblGet(), E_GrUiScptPltCnt * 4 );
			}
		}
		//update cursor color
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiMngMaxConsoleCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_WkIdx] )
			{
				m_ConTbl[Tv_WkIdx]->CursColorSet( m_ScptDec->CursorColorGet() );
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrUiScptDec*	Cls_GrUiMng::ScptDecGet( void )
{
		return	m_ScptDec;
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::LcUiMsgBrdcstCon( Ptr_GrUiMsgHd A_PtrMsg )
{
	// local -------------------
		__u8		Tv_ConIdx;
	// code --------------------
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrUiMngMaxConsoleCnt;Tv_ConIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				A_PtrMsg->ConIdx		=	Tv_ConIdx;
				m_ConTbl[Tv_ConIdx]->RtlUiMsgProc( A_PtrMsg );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::LanguageSet( __u32 A_Lang )
{
	// local -------------------
		St_GrUiMsgStatChg		Tv_UiMsg;
	// code --------------------
		//check accept language
		if ( E_GrUiLanguageCnt > A_Lang )
		{
			//check change language
			if ( A_Lang != m_Language )
			{
				m_Language	=	A_Lang;
				//notify change language
				Tv_UiMsg.Hd.Cmd			=	E_GrUiMsgCmdStatChg;
				Tv_UiMsg.Hd.ConIdx	=	E_GrUiConIdxAll;
				Tv_UiMsg.Hd.WinId	=	E_GrUiWinIdAll;
				Tv_UiMsg.Hd.ObjId	=	E_GrUiObjIdBroadcast;
				Tv_UiMsg.Type			=	E_GrUiChgLanguage;
				UiMsgPost( (Ptr_GrUiMsgHd)&Tv_UiMsg, sizeof(Tv_UiMsg) );
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiMng::LanguageGet( void )
{
		return	m_Language;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::IsScptExist( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( NULL != m_ScptDec )
		{
			if ( NULL != m_ScptDec->ObjScriptGet( A_ScptId ) )
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::ScptAtbGet(	__u16 A_ScptId, __u8* A_PtrRtLayer, Ptr_GrRect A_PtrRtRect, BOOL8* A_PtrRtIsSlave )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( NULL != m_ScptDec )
		{
			Tv_Result	=	m_ScptDec->ObjAtbGet( A_ScptId, A_PtrRtLayer, A_PtrRtRect, A_PtrRtIsSlave );
		}
	
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::ScptDecById( __u16 A_ScptId, Cls_GrUiDoBase* A_ObjBase )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		if ( NULL != m_ScptDec )
		{
			UserEventLock();
			Tv_Result	=	m_ScptDec->DecById( A_ObjBase, A_ScptId );
			UserEventUnlock();
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::ScptDecByPtr( void* A_PtrScpt, Cls_GrUiDoBase* A_ObjBase, BOOL8 A_IsChgId )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		UserEventLock();
		Tv_Result	=	m_ScptDec->DecScript( A_ObjBase, A_PtrScpt, A_IsChgId );
		UserEventUnlock();

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::ReqConPaint( __u8 A_ConIdx )
{
	// local -------------------
		St_GrUiMsgHd	Tv_Msg;
	// code --------------------
		//init
		Tv_Msg.Cmd			=	E_GrUiMsgCmdConPaint;
		Tv_Msg.ConIdx		=	A_ConIdx;
		Tv_Msg.WinId		=	E_GrUiWinIdNone;
		Tv_Msg.ObjId		=	E_GrUiObjIdNone;
		return	UiMsgPost( &Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
Ptr_GrGdib	Cls_GrUiMng::ImgGet( __u16 A_ImgId )
{
	// local -------------------
		Ptr_GrGdib	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		//check range
		if ( (E_GrUiImgIdUserStart <= A_ImgId) && ((E_GrUiImgIdUserStart + E_GrUiMngMaxAllocImgCnt) > A_ImgId) )
		{
			//user regist image
			Tv_Result	=	m_ImgAlocTbl[A_ImgId - E_GrUiImgIdUserStart];
		}
		else if ( E_GrUiImgIdUserStart > A_ImgId )
		{
			Tv_Result	=	m_ScptDec->ImgGet( A_ImgId );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::UiMsgPost( Ptr_GrUiMsgHd A_PtrMsg, __u32 A_Size )
{
		return	m_TaskSvr->MsgExPost( E_GrTaskMsgUiMsg, (void*)A_PtrMsg, A_Size );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::UiMsgPump( void )
{
		return	m_TaskSvr->DoStep();
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::UiMsgRest( void )
{
		GrPrcSleep( E_GrUiMngMsgRestTime );
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::LcProcMsgUiMsg( Ptr_GrUiMsgHd A_PtrMsg )
{
	// local -------------------
	// code --------------------
		//check broad cast console
		if ( E_GrUiConIdxAll == A_PtrMsg->ConIdx )
		{
			LcUiMsgBrdcstCon( A_PtrMsg );
		}
		else
		{
			if ( NULL != m_ConTbl[A_PtrMsg->ConIdx] )
			{
				m_ConTbl[A_PtrMsg->ConIdx]->RtlUiMsgProc( A_PtrMsg );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::IsOsdExp( void )
{
		return	V_IsGrUiMngPalMode;
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::PalModeSet( BOOL8 A_IsSet )
{
		V_IsGrUiMngPalMode	=	A_IsSet;
		LcConNotifyGbStatAll( E_GrUiChgOsdExp );
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::LcConRedrawAll( void )
{
	// local -------------------
		__u8	Tv_ConIdx;
		St_GrRect	Tv_Rc;
	// code --------------------
		for ( Tv_ConIdx = 0;Tv_ConIdx < E_GrUiMngMaxConsoleCnt;Tv_ConIdx++ )
		{
			//check exist console
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				//request redraw
				Tv_Rc.left		=	0;
				Tv_Rc.top			=	0;
				Tv_Rc.right		=	m_ConTbl[Tv_ConIdx]->GetResX();
				Tv_Rc.bottom	=	m_ConTbl[Tv_ConIdx]->GetResY();
				m_ConTbl[Tv_ConIdx]->PaintReq( &Tv_Rc );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::LcConNotifyGbStatAll( __u8 A_Type )
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------
		for ( Tv_ConIdx = 0;Tv_ConIdx < E_GrUiMngMaxConsoleCnt;Tv_ConIdx++ )
		{
			//check exist console
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				//request redraw
				m_ConTbl[Tv_ConIdx]->NotifyGbStatChg( A_Type );
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiMng::UserVersionGet( void )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result		=	0;

		if ( NULL != m_ScptDec )
		{
			Tv_Result	=	m_ScptDec->UserVersionGet();
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiMng::UserImgRegist( Ptr_GrGdib A_PtrGdib )
{
	// local -------------------
		__u16		Tv_Result;
		__u16		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrUiImgIdNone;

		//find empty slot
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiMngMaxAllocImgCnt;Tv_WkIdx++ )
		{
			if ( NULL == m_ImgAlocTbl[Tv_WkIdx] )
			{
				//find
				Tv_Result		=	E_GrUiImgIdUserStart + Tv_WkIdx;
				m_ImgAlocTbl[Tv_WkIdx]	=	A_PtrGdib;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::UserImgUnregist( __u16 A_Id )
{
	// local -------------------
	// code --------------------
		//check range
		if ( (E_GrUiImgIdUserStart <= A_Id) && ((E_GrUiImgIdUserStart + E_GrUiMngMaxAllocImgCnt) > A_Id) )
		{
			//unregister
			m_ImgAlocTbl[A_Id - E_GrUiImgIdUserStart]	=	NULL;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::LcProcMsgQuadChg( Ptr_GrTaskMsgQuadChgReq A_PtrMsg )
{
	// local -------------------
		St_GrUiMsgStatChg	Tv_UiMsg;
	// code --------------------
		Tv_UiMsg.Hd.Cmd			=	E_GrUiMsgCmdStatChg;
		Tv_UiMsg.Hd.ConIdx	=	A_PtrMsg->ConIdx;
		Tv_UiMsg.Hd.WinId		=	E_GrUiWinIdAll;
		Tv_UiMsg.Hd.ObjId		=	E_GrUiObjIdBroadcast;
		Tv_UiMsg.Type				=	E_GrUiChgQuad;
		LcProcMsgUiMsg( (Ptr_GrUiMsgHd)&Tv_UiMsg );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::IsUserEventOn( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 >= m_UsrEvtLvl )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::UserEventLock( void )
{
#ifndef LINUX_APP
		if ( 0 == m_UsrEvtLvl )
		{
			DbgMsgPrint( "Cls_GrUiMng::UserEventLock - Locked.\n" );
		}
#endif
		m_UsrEvtLvl ++;
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::UserEventUnlock( void )
{
#ifndef LINUX_APP
		if ( 1 == m_UsrEvtLvl )
		{
			DbgMsgPrint( "Cls_GrUiMng::UserEventUnlock - Unlocked.\n" );
		}
#endif
		m_UsrEvtLvl --;
		if ( 0 > m_UsrEvtLvl )
		{
			DbgMsgPrint( "Cls_GrUiMng::UserEventUnlock - user event level break.\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::KeyOneSkip( void )
{
		m_IsKeyOneSkip	=	TRUE;	
}
//--------------------------------------------------------------------
__u8	Cls_GrUiMng::FocusConsoleIdx( void )
{
		return	m_FocusConIdx;
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::GlbVarSet( __u8 A_Idx, __u32 A_Val )
{
#ifndef LINUX_APP
		if ( E_GrUiMngGlobalValCnt <= A_Idx )
		{
			DbgMsgPrint( "Cls_GrUiMng::GlbVarSet - over value index.\n" );
		}
#endif
		m_GlbVarTbl[A_Idx]	=	A_Val;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiMng::GlbVarGet( __u8 A_Idx )
{
#ifndef LINUX_APP
		if ( E_GrUiMngGlobalValCnt <= A_Idx )
		{
			DbgMsgPrint( "Cls_GrUiMng::GlbVarSet - over value index.\n" );
		}
#endif
		return	m_GlbVarTbl[A_Idx];
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::PasswdChkUseSet( BOOL8 A_IsOn )
{
		m_IsPasswdChk	=	A_IsOn;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiMng::IsPasswdChk( void )
{
		return	m_IsPasswdChk;
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::PasswdErrTxtIdSet( __u8 A_ErrCode, __u16 A_TxtId )
{
		if ( (0 != A_ErrCode) && (E_GrStrPasswdErrCnt >= A_ErrCode) )
		{
			m_PassErrTxt[A_ErrCode - 1]	=	A_TxtId;
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrUiMng::PasswdErrTxtIdGet( __u8 A_ErrCode )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrUiTxtIdNone;

		if ( (0 != A_ErrCode) && (E_GrStrPasswdErrCnt >= A_ErrCode) )
		{
			Tv_Result	=	m_PassErrTxt[A_ErrCode - 1];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::LangKeyTblSet( __u16* A_PtrKeyTbl, __u32 A_Cnt )
{
	// local -------------------
	// code --------------------
		m_PtrLangKeyTbl	=	A_PtrKeyTbl;
		m_LangKeyCnt		=	A_Cnt;
}
//--------------------------------------------------------------------
__u16*	Cls_GrUiMng::LangKeyTblGet( void )
{
	// local -------------------
	// code --------------------
		return	m_PtrLangKeyTbl;
}
//--------------------------------------------------------------------
__u32		Cls_GrUiMng::LangKeyCnt( void )
{
	// local -------------------
	// code --------------------
		return	m_LangKeyCnt;
}
//--------------------------------------------------------------------
void	Cls_GrUiMng::InputAbleSet( BOOL8 A_IsAble )
{
	// local -------------------
	// code --------------------
		m_IsInputAble	=	A_IsAble;
}
//--------------------------------------------------------------------

