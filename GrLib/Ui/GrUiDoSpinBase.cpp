/*
 spin button base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoSpinBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>
#include	<Ui/GrUiDoInpTime.h>

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
Cls_GrUiDoSpinBase::Cls_GrUiDoSpinBase( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_ClrWhite;
	// code --------------------
		//init
		m_IsFocusAble		=	TRUE;

		m_IsOnKeyCtrl		=	FALSE;

		GrDumyZeroMem( &m_TxtPara, sizeof(m_TxtPara) );
		GrDumyZeroMem( m_StatClrTbl, sizeof(m_StatClrTbl) );
		GrDumyZeroMem( m_StatSkinTbl, sizeof(m_StatSkinTbl) );
		GrDumyZeroMem( m_StatTxtClrTbl, sizeof(m_StatTxtClrTbl) );
		m_TxtPara.ClrShadow	=	m_ObjConsole->ClrBlackGet();
		m_EvtHostId		=	E_GrUiObjIdNone;
		m_TxtPara.ClrShadow	=	m_ObjConsole->ClrBlackGet();
		Tv_ClrWhite		=	m_ObjConsole->ClrWhiteGet();
		for ( Tv_WkIdx = 0;Tv_WkIdx < E_GrUiDoSpinStatCnt;Tv_WkIdx++ )
		{
			m_StatTxtClrTbl[Tv_WkIdx]	=	Tv_ClrWhite;
		}

		m_IsCtrlMode	=	FALSE;
}
//--------------------------------------------------------------------
Cls_GrUiDoSpinBase::~Cls_GrUiDoSpinBase()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::TextParaSet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( &m_TxtPara, A_PtrPara, sizeof(St_GrUiDoTextPara) );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::TextParaGet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( A_PtrPara, &m_TxtPara, sizeof(St_GrUiDoTextPara) );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::ProcEvtMseLeave( void )
{
		m_IsCtrlMode	=	FALSE;
		Cls_GrUiDoSkin::ProcEvtMseLeave();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::LcSpbUpdtStat( void )
{
		m_SpbStatIdx	=	E_GrUiDoSpinStatDisable;	
		if ( IsCanEnable() )
		{
			m_SpbStatIdx	=	E_GrUiDoSpinStatNormal;
			//check control mode
			if ( m_IsCtrlMode || IsOnMouse() )
			{
				m_SpbStatIdx	=	E_GrUiDoSpinStatControl;
			}
			else
			{
				//check focus
				if ( IsFocused() )
				{
					if ( m_IsOnKeyCtrl )
					{
						m_SpbStatIdx	=	E_GrUiDoSpinStatControl;
					}
					else
					{
						m_SpbStatIdx	=	E_GrUiDoSpinStatFocus;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::ProcBgColorDraw( void )
{
		//update background color
		LcSpbUpdtStat();
		//update background color
		m_ClrBg		=	m_StatClrTbl[m_SpbStatIdx];
		Cls_GrUiDoSkin::ProcBgColorDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::ProcBgSkinTblDraw( void )
{
	// local -------------------
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		//inherited
		Cls_GrUiDoSkin::ProcBgSkinTblDraw();
		//update image
		//set clipper
		if ( PaintClipperSet( TRUE ) )
		{
			//button background skin draw
			Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_StatSkinTbl[m_SpbStatIdx].Id );
			if ( NULL != Tv_PtrGdib )
			{
				//blt
				SkinDrawByPos( m_StatSkinTbl[m_SpbStatIdx].Atb, Tv_PtrGdib, &m_PntRcClient );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinBase::ProcEvtKeyUp( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;
		switch( A_Key )
		{
			case	E_GrUiKeyEnter:
				Tv_Result	=	LcProcEvtKeyPullEnter();
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinBase::LcProcEvtKeyPullEnter( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( !m_IsOnKeyCtrl )
		{
			m_IsCtrlMode	=	!m_IsCtrlMode;
			Tv_Result			=	TRUE;
			ReqDraw();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::EvtHostIdSet( __u16 A_Id )
{
		m_EvtHostId	=	 A_Id;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoSpinBase::EvtHostIdGet( void )
{
		return	m_EvtHostId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::StatClrSet( __u8 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoSpinStatCnt > A_Idx )
		{
			m_StatClrTbl[A_Idx]		=	A_Color;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSpinBase::StatClrGet( __u8 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( E_GrUiDoSpinStatCnt > A_Idx )
		{
			Tv_Result		=	m_StatClrTbl[A_Idx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::StatTxtClrSet( __u8 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoSpinStatCnt > A_Idx )
		{
			m_StatTxtClrTbl[A_Idx]		=	A_Color;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSpinBase::StatTxtClrGet( __u8 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( E_GrUiDoSpinStatCnt > A_Idx )
		{
			Tv_Result		=	m_StatTxtClrTbl[A_Idx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::StatSkinSet( __u8 A_Idx, Ptr_GrUiDoSkinImg A_PtrSkin )
{
		if ( E_GrUiDoSpinStatCnt > A_Idx )
		{
			m_StatSkinTbl[A_Idx]	=	*A_PtrSkin;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::StatSkinGet( __u8 A_Idx, Ptr_GrUiDoSkinImg A_PtrRtSkin )
{
		A_PtrRtSkin->Id		=	E_GrUiImgIdNone;
		A_PtrRtSkin->Atb	=	0;
		if ( E_GrUiDoSpinStatCnt > A_Idx )
		{
			*A_PtrRtSkin	=	m_StatSkinTbl[A_Idx];
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::ProcEvtFocusGet( void )
{
		if ( m_IsOnKeyCtrl )
		{
			m_IsCtrlMode	=	TRUE;
		}
		Cls_GrUiDoSkin::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::ProcEvtFocusLoss( void )
{
		m_IsCtrlMode	=	FALSE;
		Cls_GrUiDoSkin::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::LcNotifyValChg( void )
{
	// local -------------------
		BOOL8	Tv_IsAble;
		St_GrUiMsgCtrl	Tv_Msg;
		Cls_GrUiDoBase*	Tv_Obj;
		__u8	Tv_TypeId;
	// code --------------------
		Tv_IsAble	=	TRUE;
		if ( !V_GrUiMngObj->IsUserEventOn() )
		{
			Tv_IsAble	=	FALSE;
			if ( E_GrUiObjIdNone != m_EvtHostId )
			{
				Tv_Obj	=	m_ObjWin->FindSlaveObjByObjId( m_EvtHostId );
				if ( NULL != Tv_Obj )
				{
					Tv_IsAble	=	TRUE;
					Tv_TypeId	=	Tv_Obj->TypeIdGet();
					switch( Tv_TypeId )
					{
						case	E_GrUiDoTypeIdInpTime:
						case	E_GrUiDoTypeIdCalendar:
							Tv_IsAble	=	FALSE;
							break;
					}
				}
			}
		}

		if ( Tv_IsAble )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdSpinValChg;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_EvtHostId;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::LcNotifyClick( void )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		if ( V_GrUiMngObj->IsUserEventOn() || (E_GrUiObjIdNone != m_EvtHostId) )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdSpinClick;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_EvtHostId;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinBase::IsOnKeyCtrl( void )
{
		return	m_IsOnKeyCtrl;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::OnKeyCtrlSet( BOOL8 A_IsUse )
{
		m_IsOnKeyCtrl	=	A_IsUse;
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinBase::IsCtrlMode( void )
{
		return	m_IsCtrlMode;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinBase::CtrlModeSet( BOOL8 A_IsOn )
{
		m_IsCtrlMode	=	A_IsOn;
}
//--------------------------------------------------------------------

