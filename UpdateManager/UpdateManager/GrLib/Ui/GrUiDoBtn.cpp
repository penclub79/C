/*
 osd text draw

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoBtn.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<Ui/GrUiConsole.h>
#include	<GrStrTool.h>
#include	<Ui/GrUiDoWin.h>
#include	<Ui/GrUiMng.h>

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
Cls_GrUiDoBtn::Cls_GrUiDoBtn( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
	// local -------------------
		__u32	Tv_WkIdx;
		__u32	Tv_ClrWhite;
	// code --------------------
		//init
		m_TypeId			=	E_GrUiDoTypeIdButton;

		m_IsFocusAble	=	TRUE;			//focusable control
		m_IsBtnPush		=	FALSE;
		m_BtnType			=	E_GrUiDoBtnTypeNormal;
		m_IsBtnChked	=	FALSE;
		GrDumyZeroMem( &m_TxtPara, sizeof(m_TxtPara) );
		m_TxtPara.ClrShadow	=	m_ObjConsole->ClrBlackGet();
		m_BtnEvtHostId	=	E_GrUiObjIdNone;

		GrStrClear( m_StrText );
		m_TextId			=	E_GrUiTxtIdNone;
		Tv_ClrWhite		=	m_ObjConsole->ClrWhiteGet();
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoBtnImgCnt;Tv_WkIdx++ )
		{
			m_BtnSkinTbl[Tv_WkIdx].Atb		=	0;
			m_BtnSkinTbl[Tv_WkIdx].Id			=	E_GrUiImgIdNone;
			m_BtnClrTbl[Tv_WkIdx]	=	0;
			m_BtnTxtClrTbl[Tv_WkIdx]	=	Tv_ClrWhite;
		}
}
//--------------------------------------------------------------------
Cls_GrUiDoBtn::~Cls_GrUiDoBtn()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::ProcBgColorDraw( void )
{
	// local -------------------
		__u32		Tv_SkinIdx;
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		//check draw background
		Tv_SkinIdx	=	LcBtnGetSkinIdx();
		//select background color
		m_ClrBg		=	m_BtnClrTbl[Tv_SkinIdx];
		//draw background color
		Cls_GrUiDoSkin::ProcBgColorDraw();
		//set clipper
		if ( PaintClipperSet( TRUE ) )
		{
			//button background skin draw
			Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_BtnSkinTbl[Tv_SkinIdx].Id );
			if ( NULL != Tv_PtrGdib )
			{
				//blt
				SkinDrawByPos( m_BtnSkinTbl[Tv_SkinIdx].Atb, Tv_PtrGdib, &m_PntRcClient );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBtn::IsBtnChecked( void )
{
		return	m_IsBtnChked;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::BtnCheckSet( BOOL8 A_IsCheck, BOOL8 A_IsRelate )
{
	// local -------------------
	// code --------------------
		//check type
		if ( E_GrUiDoBtnTypeNormal != m_BtnType )
		{
			m_IsBtnChked	=	A_IsCheck;
			//run event
			if ( (E_GrUiDoBtnTypeToggle == m_BtnType) || m_IsBtnChked )
			{
				LcBtnEventMakeUp();
			}
			//other off
			if ( m_IsBtnChked && A_IsRelate && (E_GrUiDoBtnTypeRadio == m_BtnType) )
			{
				LcBtnRadioOtherOff();
			}
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::LcBtnRadioOtherOff( void )
{
	// local -------------------
		Cls_GrUiDoBase*	Tv_ObjPrnt;
		Cls_GrUiDoBase*	Tv_ObjSlave;
		Cls_GrUiDoBtn*	Tv_ObjBtn;
	// code --------------------
		//find parent 
		Tv_ObjPrnt	=	m_ObjPrnt;
		if ( NULL != Tv_ObjPrnt )
		{
			//find button
			Tv_ObjSlave	=	Tv_ObjPrnt->SlaveObjGet();
			while ( NULL != Tv_ObjSlave )
			{
				//check current object
				if ( (this != Tv_ObjSlave) && (E_GrUiDoTypeIdButton == Tv_ObjSlave->TypeIdGet()) )
				{
					//init
					Tv_ObjBtn	=	(Cls_GrUiDoBtn*)Tv_ObjSlave;
					//check radio button
					if ( E_GrUiDoBtnTypeRadio == Tv_ObjBtn->BtnTypeGet() )
					{
						if ( Tv_ObjBtn->IsBtnChecked() )
						{
							Tv_ObjBtn->BtnCheckSet( FALSE, FALSE );
						}
					}
				}
				//next
				Tv_ObjSlave	=	Tv_ObjSlave->NextObjGet();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::BtnPush( void )
{
	// local -------------------
	// code --------------------
		switch( m_BtnType )
		{
			case	E_GrUiDoBtnTypeToggle:
				//toggle button
				BtnCheckSet( !m_IsBtnChked );				
				break;
			case	E_GrUiDoBtnTypeRadio:
				BtnCheckSet( TRUE );
				break;
			default:
				//normal button
				LcBtnEventMakeUp();
				break;
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBtn::LcBtnGetSkinIdx( void )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	E_GrUiDoBtnImgIdxDisable;
		//check disable
		if ( IsCanEnable()	)
		{
			//normal mode
			Tv_Result		=	E_GrUiDoBtnImgIdxNoraml;	
			//check checked mode
			if ( m_IsBtnChked )
			{
				Tv_Result		=	E_GrUiDoBtnImgIdxCheck;
			}
			//check pushed
			if ( m_IsBtnPush )
			{
				Tv_Result	=	Tv_Result + 2;
			}
			else if ( IsFocused() || IsOnMouse() )
			{
				//check focused
				Tv_Result	++;
			}
		}
		else if ( m_IsBtnChked )
		{ 
			Tv_Result	=	E_GrUiDoBtnImgIdxChkDis;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBtn::BtnTypeGet( void )
{
		return	m_BtnType;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::BtnTypeSet( __u8 A_RunType )
{
		m_BtnType	=	A_RunType;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBtn::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		//push status draw
		m_IsBtnPush		=	TRUE;
		//button down event
		LcBtnEventMakeDown();
		ReqDraw();		
		Cls_GrUiDoSkin::ProcEvtMseLbtnDown( A_X, A_Y, A_BtnMap );
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBtn::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
		//do click event
		m_IsBtnPush		=	FALSE;
		BtnPush();
		ReqDraw();
		Cls_GrUiDoSkin::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBtn::ProcEvtKeyDown( __u8 A_Key )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		//check enter key
		if ( E_GrUiKeyEnter == A_Key )
		{
			//push status draw
			m_IsBtnPush		=	TRUE;
			ReqDraw();
			LcBtnEventMakeDown();
			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_Result	=	Cls_GrUiDoSkin::ProcEvtKeyDown( A_Key );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBtn::ProcEvtKeyUp( __u8 A_Key )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		//check enter key
		if ( E_GrUiKeyEnter == A_Key )
		{
			//push status draw
			m_IsBtnPush		=	FALSE;
			BtnPush();
			ReqDraw();
			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_Result	=	Cls_GrUiDoSkin::ProcEvtKeyUp( A_Key );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::ProcBgSkinTblDraw( void )
{
	// local -------------------
		__u32	Tv_SkinIdx;
	// code --------------------
		Cls_GrUiDoSkin::ProcBgSkinTblDraw();
		//draw text
		if ( PaintClipperSet( TRUE ) )
		{
			//set text color
			Tv_SkinIdx	=	LcBtnGetSkinIdx();
			//draw text
			if (E_GrUiTxtIdNone != m_TextId) 
			{
				RosTextDraw( m_TextId, &m_TxtPara, m_BtnTxtClrTbl[Tv_SkinIdx], &m_PntRcClient );
			}
			else if ( GrStrIsExist(m_StrText) ) 
			{
				TextDraw( m_StrText, &m_TxtPara, m_BtnTxtClrTbl[Tv_SkinIdx], &m_PntRcClient );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::BtnStatImgGet( __u32 A_Idx, Ptr_GrUiDoSkinImg A_PtrRtSkin )
{
	// local -------------------
	// code --------------------
		//init
		A_PtrRtSkin->Id			=	E_GrUiImgIdNone;
		A_PtrRtSkin->Atb		=	0;
		//check able image
		if ( E_GrUiDoBtnImgCnt > A_Idx )
		{
			*A_PtrRtSkin	=	m_BtnSkinTbl[A_Idx];
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::BtnStatImgSet( __u32 A_Idx, Ptr_GrUiDoSkinImg A_PtrSkin )
{
		//check able image
		if ( E_GrUiDoBtnImgCnt > A_Idx )
		{
			m_BtnSkinTbl[A_Idx]		=	*A_PtrSkin;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::ProcEvtMseLeave( void )
{
		//release pushed
		m_IsBtnPush		=	FALSE;
		Cls_GrUiDoSkin::ProcEvtMseLeave();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::TextGet( WCHAR* A_StrRt, __u32 A_LenMax )
{
	// local -------------------
	// code --------------------
		if ( E_GrUiTxtIdNone == m_TextId )
		{
			GrStrWcopy( A_StrRt, m_StrText, A_LenMax );
		}
		else
		{
			V_GrUiMngObj->TextGet( m_TextId, A_StrRt, A_LenMax );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::TextSet( WCHAR* A_Str )
{
		if ( E_GrUiTxtIdNone == m_TextId )
		{
			GrStrWcopy( m_StrText, A_Str, sizeof(m_StrText) - 1 );
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBtn::TextIdGet( void )
{
	// local -------------------
	// code --------------------
		return	m_TextId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::TextIdSet( __u32 A_Id )
{
	// local -------------------
	// code --------------------
		if ( m_TextId != A_Id )
		{
			m_TextId	=	A_Id;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::TextParaSet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( &m_TxtPara, A_PtrPara, sizeof(m_TxtPara) );
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::TextParaGet( Ptr_GrUiDoTextPara A_PtrPara )
{
		GrDumyCopyMem( A_PtrPara, &m_TxtPara, sizeof(m_TxtPara) );
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBtn::BtnStatClrGet( __u32 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result		=	0;
		if ( E_GrUiDoBtnImgCnt > A_Idx )
		{
			Tv_Result	=	m_BtnClrTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::BtnStatClrSet( __u32 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoBtnImgCnt > A_Idx )
		{
			m_BtnClrTbl[A_Idx]	=	A_Color;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBtn::BtnTxtClrGet( __u32 A_Idx )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		Tv_Result		=	0;
		if ( E_GrUiDoBtnImgCnt > A_Idx )
		{
			Tv_Result	=	m_BtnTxtClrTbl[A_Idx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::BtnTxtClrSet( __u32 A_Idx, __u32 A_Color )
{
	if ( E_GrUiDoBtnImgCnt > A_Idx )
	{
		m_BtnTxtClrTbl[A_Idx]	=	A_Color;
		ReqDraw();
	}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::LcBtnEventMakeDown( void )
{
	// local -------------------
		St_GrUiMsgCtrl		Tv_Msg;
	// code --------------------
		//run event
		if ( V_GrUiMngObj->IsUserEventOn() || (E_GrUiObjIdNone != m_BtnEvtHostId) )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdBtnDown;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_BtnEvtHostId;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::LcBtnEventMakeUp( void )
{
	// local -------------------
		St_GrUiMsgCtrl		Tv_Msg;
	// code --------------------
		//run event
		if ( V_GrUiMngObj->IsUserEventOn() || (E_GrUiObjIdNone != m_BtnEvtHostId) )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdBtnUp;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	m_BtnEvtHostId;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBtn::BtnEvtHostIdSet( __u16 A_ObjId )
{
		m_BtnEvtHostId	=	A_ObjId;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoBtn::BtnEvtHostIdGet( void )
{
		return	m_BtnEvtHostId;
}
//--------------------------------------------------------------------


