/*
 spin button text

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoSpinTxt.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>

//====================================================================
//local const
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*				V_GrUiMngObj;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoSpinTxt::Cls_GrUiDoSpinTxt( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSpinBase( A_PtrRect, A_ObjPrnt, A_IsIdAlloc )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		m_TypeId				=	E_GrUiDoTypeIdSpbtnTxt;

		m_TxtIdSt			=	E_GrUiTxtIdNone;
		m_TxtIdCnt		=	0;
		m_IsVertical	=	FALSE;
		m_CenterSize	=	0;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoSpinTxtMaxItemCnt;Tv_WkIdx++ )
		{
			GrStrClear( m_StrTbl[Tv_WkIdx] );
		}

		m_ItemCnt			=	0;
		m_SelIdx			=	0;
}
//--------------------------------------------------------------------
Cls_GrUiDoSpinTxt::~Cls_GrUiDoSpinTxt()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinTxt::ProcBgSkinTblDraw( void )
{
	// local -------------------
		WCHAR*		Tv_Str;
	// code --------------------
		//inherited
		Cls_GrUiDoSpinBase::ProcBgSkinTblDraw();
		//set clipper
		if ( PaintClipperSet( TRUE ) )
		{
			//draw text
			Tv_Str	=	 NULL;
			if ( 0 != m_TxtIdCnt )
			{
				Tv_Str	=	V_GrUiMngObj->TextPtrGet( m_TxtIdSt + m_SelIdx );
			}
			else
			{
				if ( m_SelIdx < m_ItemCnt )
				{
					Tv_Str	=	m_StrTbl[m_SelIdx];
				}
			}
			if ( NULL != Tv_Str )
			{
				TextDraw( Tv_Str, &m_TxtPara, m_StatTxtClrTbl[m_SpbStatIdx], &m_PntRcClient );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinTxt::ProcEvtKeyDown( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;
		switch( A_Key )
		{
			case	E_GrUiKeyRight:
				if ( !m_IsVertical )
				{
					Tv_Result	=	LcProcEvtKeyPushUp();
				}
				break;
			case	E_GrUiKeyLeft:
				if ( !m_IsVertical )
				{
					Tv_Result	=	LcProcEvtKeyPushDown();
				}
				break;
			case	E_GrUiKeyUp:
				if ( m_IsVertical )
				{
					Tv_Result	=	LcProcEvtKeyPushUp();
				}
				break;
			case	E_GrUiKeyDown:
				if ( m_IsVertical )
				{
					Tv_Result	=	LcProcEvtKeyPushDown();
				}
				break;
		}
		if ( !Tv_Result )
		{
			Tv_Result	=	Cls_GrUiDoSpinBase::ProcEvtKeyDown( A_Key );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinTxt::ProcEvtKeyUp( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;	
		switch( A_Key )
		{
			case	E_GrUiKeyEnter:
				if ( m_IsOnKeyCtrl && (0 != m_CenterSize) )
				{
					LcNotifyClick();
					Tv_Result	=	TRUE;
				}
				else
				{
					Tv_Result	=	LcProcEvtKeyPullEnter();
				}
				break;
		}
		if ( !Tv_Result )
		{
			Tv_Result	=	Cls_GrUiDoSpinBase::ProcEvtKeyUp( A_Key );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinTxt::LcProcEvtKeyPushUp( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_SelIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		//check able
		if ( m_IsCtrlMode )
		{
			Tv_SelIdx	=	m_SelIdx + 1;
			SelectIdxSet( Tv_SelIdx );
			Tv_Result		=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinTxt::LcProcEvtKeyPushDown( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_SelIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		//check able
		if ( m_IsCtrlMode )
		{
			Tv_SelIdx	=	m_SelIdx - 1;
			SelectIdxSet( Tv_SelIdx );
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinTxt::ProcEvtWheelUp( void )
{
	// local -------------------
		__s32		Tv_SelIdx;
	// code --------------------
		Tv_SelIdx	=	m_SelIdx + 1;
		SelectIdxSet( Tv_SelIdx );

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinTxt::ProcEvtWheelDown( void )
{
	// local -------------------
		__s32		Tv_SelIdx;
	// code --------------------
		Tv_SelIdx	=	m_SelIdx - 1;
		SelectIdxSet( Tv_SelIdx );
		return	TRUE;
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSpinTxt::ItemCountGet( void )
{
		return	m_ItemCnt;
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSpinTxt::SelectIdxGet( void )
{
		return	m_SelIdx;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinTxt::SelectIdxSet( __s32 A_Idx, BOOL8 A_IsNotify )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		if ( ( 0 != m_TxtIdCnt ) || ( 0 != m_ItemCnt ) )
		{
			//update value
			m_SelIdx	=	A_Idx;
			//patch psoition
			if ( 0 != m_TxtIdCnt )
			{
				if ( 0 > A_Idx )
				{
					m_SelIdx	=	m_TxtIdCnt - 1;
				}
				else if ( A_Idx >= m_TxtIdCnt )
				{
					m_SelIdx	=	0;
				}
			}
			else
			{
				if ( 0 > A_Idx )
				{
					m_SelIdx	=	m_ItemCnt - 1;
				}
				else if ( A_Idx >= m_ItemCnt )
				{
					m_SelIdx	=	0;
				}
			}

			//notify message
			if ( A_IsNotify )
			{
				if ( V_GrUiMngObj->IsUserEventOn() )
				{
					Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdSpinValChg;
					Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
					Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
					Tv_Msg.Hd.ObjId		=	m_EvtHostId;
					Tv_Msg.Obj				=	(void*)this;
					UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
				}
			}

			ReqDraw();
		}
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSpinTxt::ItemAdd( WCHAR* A_Str )
{
	// local -------------------
		__s32		Tv_Result;
	// code --------------------
		Tv_Result	=	-1;

		if ( E_GrUiDoSpinTxtMaxItemCnt > m_ItemCnt )
		{
			GrStrWcopy( m_StrTbl[m_ItemCnt] , A_Str );
			Tv_Result	=	m_ItemCnt;
			m_ItemCnt ++;
			ReqDraw();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinTxt::ItemDelete( __s32 A_Idx )
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( (0 <= A_Idx) && (m_ItemCnt > A_Idx) )
		{
			//collect items
			Tv_WkIdx	=	A_Idx + 1;
			while ( Tv_WkIdx < m_ItemCnt )
			{
				GrStrWcopy( m_StrTbl[Tv_WkIdx-1], m_StrTbl[Tv_WkIdx] );
				//next
				Tv_WkIdx ++;
			}
			m_ItemCnt --;
			ReqDraw();
			//success
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinTxt::ItemTextGet( __s32 A_Idx, WCHAR* A_StrRt )
{
	// local -------------------
		BOOL8	Tv_Result;
		WCHAR*	Tv_Str;
	// code --------------------
		Tv_Result	=	FALSE;

		GrStrClear( A_StrRt );

		if ( 0 <= A_Idx )
		{
			if ( 0 != m_TxtIdCnt )
			{
				if ( (__u32)A_Idx < m_TxtIdCnt )
				{
					Tv_Str	=	V_GrUiMngObj->TextPtrGet( m_TxtIdSt + (__u32)A_Idx );
					if ( NULL != Tv_Str )
					{
						GrStrWcopy( A_StrRt, Tv_Str );
					}
					//success
					Tv_Result		=	TRUE;
				}
			}
			else
			{
				if ( m_ItemCnt > A_Idx )
				{
					GrStrWcopy( A_StrRt, m_StrTbl[A_Idx] );
					//success
					Tv_Result		=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinTxt::ItemTextSet( __s32 A_Idx, WCHAR* A_Str )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 == m_TxtIdCnt )
		{
			if ( A_Idx < m_ItemCnt )
			{
				GrStrWcopy( m_StrTbl[A_Idx], A_Str );
				ReqDraw();
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinTxt::ItemReset( void )
{
		m_ItemCnt		=	0;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinTxt::TextIdStartSet( __u32 A_Id )
{
		m_TxtIdSt	=	A_Id;
		if ( 0 != m_TxtIdCnt )
		{
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSpinTxt::TextIdStartGet( void )
{
		return	m_TxtIdSt;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinTxt::TextIdCountSet( __u8 A_Cnt )
{
		if ( E_GrUiDoSpinTxtMaxItemCnt >= A_Cnt )
		{
			m_TxtIdCnt	=	A_Cnt;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoSpinTxt::TextIdCountGet( void )
{
		return	m_TxtIdCnt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinTxt::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect		Tv_Rc;
		__s32		Tv_PosUp;
		__s32		Tv_PosDn;
		__s32		Tv_SelIdx;
	// code --------------------
		//inherited
		Cls_GrUiDoSpinBase::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );

		//check position
		//if ( ScrRectGet( &Tv_Rc ) )
		if ( ClientRectGet( &Tv_Rc ) )
		{
			if ( m_IsVertical )
			{
				//vertical type
				Tv_PosUp		=	((Tv_Rc.bottom - Tv_Rc.top) >> 1) + Tv_Rc.top - (__s32)m_CenterSize;
				Tv_PosDn		=	Tv_PosUp + (__s32)(m_CenterSize << 1);
				if ( Tv_PosDn < A_Y )
				{
					Tv_SelIdx	=	m_SelIdx - 1;
					SelectIdxSet( Tv_SelIdx );
				}
				else if ( Tv_PosUp >= A_Y )
				{
					Tv_SelIdx	=	m_SelIdx + 1;
					SelectIdxSet( Tv_SelIdx );
				}
				else if ( 0 != m_CenterSize )
				{
					//make click event
					LcNotifyClick();
				}
			}
			else
			{
				//horizontal
				Tv_PosUp		=	((Tv_Rc.right - Tv_Rc.left) >> 1) + Tv_Rc.left - (__s32)m_CenterSize;
				Tv_PosDn		=	Tv_PosUp + (__s32)(m_CenterSize << 1);
				if ( Tv_PosUp >= A_X )
				{
					Tv_SelIdx	=	m_SelIdx - 1;
					SelectIdxSet( Tv_SelIdx );
				}
				else if ( Tv_PosDn < A_X )
				{
					Tv_SelIdx	=	m_SelIdx + 1;
					SelectIdxSet( Tv_SelIdx );
				}
				else if (0 != m_CenterSize)
				{
					//make click event
					LcNotifyClick();
				}
			}
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinTxt::VerticalTypeSet( BOOL8 A_IsUse )
{
		m_IsVertical	=	A_IsUse;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinTxt::IsVerticalType( void )
{
		return	m_IsVertical;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSpinTxt::CenterSizeGet( void )
{
		return	m_CenterSize;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinTxt::CenterSizeSet( __u32 A_Size )
{
		m_CenterSize	=	A_Size;
}
//--------------------------------------------------------------------

