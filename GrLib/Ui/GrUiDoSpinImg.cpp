/*
 spin button image

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoSpinImg.h>
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
Cls_GrUiDoSpinImg::Cls_GrUiDoSpinImg( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSpinBase( A_PtrRect, A_ObjPrnt, A_IsIdAlloc )
{
	// local -------------------
	// code --------------------
		//init
		m_TypeId				=	E_GrUiDoTypeIdSpbtnImg;

		m_IsVertical	=	FALSE;

		GrDumyZeroMem( &m_ImageTbl, sizeof(m_ImageTbl) );

		m_SelIdx			=	0;
		m_ImageCnt		=	E_GrUiDoSpinImgDfltImgCnt;
		m_TxtIdSt			=	E_GrUiTxtIdNone;

}
//--------------------------------------------------------------------
Cls_GrUiDoSpinImg::~Cls_GrUiDoSpinImg()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinImg::ProcBgSkinTblDraw( void )
{
	// local -------------------
		Ptr_GrGdib	Tv_PtrGdib;
		WCHAR*			Tv_Str;
	// code --------------------
		//inherited
		Cls_GrUiDoSpinBase::ProcBgSkinTblDraw();
		//set clipper
		if ( PaintClipperSet( TRUE ) )
		{
			//draw image
			Tv_PtrGdib	=	V_GrUiMngObj->ImgGet( m_ImageTbl[m_SelIdx].Id );
			if ( NULL != Tv_PtrGdib )
			{
				SkinDrawByPos( m_ImageTbl[m_SelIdx].Atb, Tv_PtrGdib, &m_PntRcClient );
			}
			//draw text
			if ( E_GrUiTxtIdNone != m_TxtIdSt )
			{
				Tv_Str	=	V_GrUiMngObj->TextPtrGet( m_TxtIdSt + m_SelIdx );
				if ( NULL != Tv_Str )
				{
					TextDraw( Tv_Str, &m_TxtPara, m_StatTxtClrTbl[m_SpbStatIdx], &m_PntRcClient );
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinImg::ProcEvtKeyDown( __u8 A_Key )
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

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinImg::ProcEvtKeyUp( __u8 A_Key )
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
BOOL8	Cls_GrUiDoSpinImg::LcProcEvtKeyPushUp( void )
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
BOOL8	Cls_GrUiDoSpinImg::LcProcEvtKeyPushDown( void )
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
BOOL8	Cls_GrUiDoSpinImg::ProcEvtWheelUp( void )
{
	// local -------------------
		__s32		Tv_SelIdx;
	// code --------------------
		Tv_SelIdx	=	m_SelIdx + 1;
		SelectIdxSet( Tv_SelIdx );

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinImg::ProcEvtWheelDown( void )
{
	// local -------------------
		__s32		Tv_SelIdx;
	// code --------------------
		Tv_SelIdx	=	m_SelIdx - 1;
		SelectIdxSet( Tv_SelIdx );
		return	TRUE;
}
//--------------------------------------------------------------------
__s32		Cls_GrUiDoSpinImg::SelectIdxGet( void )
{
		return	m_SelIdx;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinImg::SelectIdxSet( __s32 A_Idx, BOOL8 A_IsNotify )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		//update value
		m_SelIdx	=	A_Idx;
		//patch psoition
		if ( 0 > A_Idx )
		{
			m_SelIdx	=	(__s32)m_ImageCnt - 1;
		}
		else if ( A_Idx >= (__s32)m_ImageCnt )
		{
			m_SelIdx	=	0;
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
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinImg::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect		Tv_Rc;
		__s32		Tv_PosHalf;
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
				Tv_PosHalf	=	((Tv_Rc.bottom - Tv_Rc.top) >> 1) + Tv_Rc.top;
				if ( Tv_PosHalf < A_Y )
				{
					Tv_SelIdx	=	m_SelIdx - 1;
					SelectIdxSet( Tv_SelIdx );
				}
				else
				{
					Tv_SelIdx	=	m_SelIdx + 1;
					SelectIdxSet( Tv_SelIdx );
				}
			}
			else
			{
				//horizontal
				Tv_PosHalf	=	((Tv_Rc.right - Tv_Rc.left) >> 1) + Tv_Rc.left;
				if ( Tv_PosHalf > A_X )
				{
					Tv_SelIdx	=	m_SelIdx - 1;
					SelectIdxSet( Tv_SelIdx );
				}
				else
				{
					Tv_SelIdx	=	m_SelIdx + 1;
					SelectIdxSet( Tv_SelIdx );
				}
			}
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinImg::VerticalTypeSet( BOOL8 A_IsUse )
{
		m_IsVertical	=	A_IsUse;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoSpinImg::IsVerticalType( void )
{
		return	m_IsVertical;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinImg::ImageGet( __u8 A_Idx, Ptr_GrUiDoSkinImg A_PtrRtSkin )
{
		A_PtrRtSkin->Id		=	E_GrUiImgIdNone;
		A_PtrRtSkin->Atb	=	0;
		if ( E_GrUiDoSpinImgItemCnt > A_Idx )
		{
			*A_PtrRtSkin	=	m_ImageTbl[A_Idx];
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinImg::ImageSet( __u8 A_Idx, Ptr_GrUiDoSkinImg A_PtrSkin )
{
		if ( E_GrUiDoSpinImgItemCnt > A_Idx )
		{
			m_ImageTbl[A_Idx]	=	*A_PtrSkin;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoSpinImg::ImageCountGet( void )
{
		return	m_ImageCnt;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinImg::ImageCountSet( __u8 A_Cnt )
{
		if ( (0 != A_Cnt) && (E_GrUiDoSpinImgItemCnt >= A_Cnt) )
		{
			m_ImageCnt	=	A_Cnt;
			if ( (__u8)m_SelIdx >= m_ImageCnt )
			{
				SelectIdxSet( 0 );
			}
			else
			{
				ReqDraw();
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoSpinImg::TextStartIdGet( void )
{
		return	m_TxtIdSt;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoSpinImg::TextStartIdSet( __u32 A_Id )
{
		m_TxtIdSt	=	A_Id;
		ReqDraw();
}
//--------------------------------------------------------------------


