/*
		platform spin page text

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoSpinPageTxt.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>
#include	<P4/GrP4DoFrame.h>

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

#define E_GrP4DoSpinPageTxtParaCnt		43

St_GrP4DvlDoPrInfoData	V_GrP4DoSpinPageTxtParaInfo[E_GrP4DoSpinPageTxtParaCnt]	=	
{
	{	L"Paint Disable",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PntStat[E_GrP4DoSpinPageTxtStatDisable],NULL,	L"Paint status disable"	},
	{	L"Paint Normal",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PntStat[E_GrP4DoSpinPageTxtStatNormal],NULL,	L"Paint status normal"	},
	{	L"Paint Focus",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PntStat[E_GrP4DoSpinPageTxtStatFocus],NULL,	L"Paint status focus"	},
	{	L"Paint Control",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PntStat[E_GrP4DoSpinPageTxtStatControl],NULL,	L"Paint status control value"	},
	{	L"IsOnKeyCtrl",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->IsOnKeyCtrl,NULL,	L"anytime value control by key"	},
	{	L"IsVertical",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->IsVertical,NULL,	L"vertical type"	},
	{	L"IsValRotate",				TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->IsValRotate,NULL,	L"value rotate"	},
	{	L"FrameId",						TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->FrameId,NULL,	L"target frame script id"	},
	{	L"FrmLayer",					TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->FrmLayer,NULL,	L"move paget layer"	},
	{	L"IsMoveFcs",					TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->IsMoveFcs,NULL,	L"move focus to slave object when page chage"	},
	{	L"ItmCnt",						TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->ItemCnt,NULL,	L"target item count"	},
	{	L"TxtId0",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[0],NULL,	L"text id 0"	},
	{	L"PageId0",						TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[0],NULL,	L"page id 0"	},
	{	L"TxtId1",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[1],NULL,	L"text id 1"	},
	{	L"PageId1",						TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[1],NULL,	L"page id 1"	},
	{	L"TxtId2",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[2],NULL,	L"text id 2"	},
	{	L"PageId2",						TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[2],NULL,	L"page id 2"	},
	{	L"TxtId3",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[3],NULL,	L"text id 3"	},
	{	L"PageId3",						TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[3],NULL,	L"page id 3"	},
	{	L"TxtId4",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[4],NULL,	L"text id 4"	},
	{	L"PageId4",						TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[4],NULL,	L"page id 4"	},
	{	L"TxtId5",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[5],NULL,	L"text id 5"	},
	{	L"PageId5",						TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[5],NULL,	L"page id 5"	},
	{	L"TxtId6",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[6],NULL,	L"text id 6"	},
	{	L"PageId6",						TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[6],NULL,	L"page id 6"	},
	{	L"TxtId7",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[7],NULL,	L"text id 7"	},
	{	L"PageId7",						TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[7],NULL,	L"page id 7"	},
	{	L"TxtId8",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[8],NULL,	L"text id 8"	},
	{	L"PageId8",						TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[8],NULL,	L"page id 8"	},
	{	L"TxtId9",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[9],NULL,	L"text id 9"	},
	{	L"PageId9",						TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[9],NULL,	L"page id 9"	},
	{	L"TxtId10",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[10],NULL,	L"text id 10"	},
	{	L"PageId10",					TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[10],NULL,	L"page id 10"	},
	{	L"TxtId11",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[11],NULL,	L"text id 11"	},
	{	L"PageId11",					TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[11],NULL,	L"page id 11"	},
	{	L"TxtId12",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[12],NULL,	L"text id 12"	},
	{	L"PageId12",					TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[12],NULL,	L"page id 12"	},
	{	L"TxtId13",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[13],NULL,	L"text id 13"	},
	{	L"PageId13",					TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[13],NULL,	L"page id 13"	},
	{	L"TxtId14",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[14],NULL,	L"text id 14"	},
	{	L"PageId14",					TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[14],NULL,	L"page id 14"	},
	{	L"TxtId15",						TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->TxtTbl[15],NULL,	L"text id 15"	},
	{	L"PageId15",					TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoSpinPageTxtPara)0)->PageTbl[15],NULL,	L"page id 15"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoSpinPageTxt::Cls_GrP4DoSpinPageTxt(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect, A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassSpinPageTxt;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoSpinPageTxt) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaDoSpinPageTxt, sizeof(m_ParaDoSpinPageTxt) );

		m_IsCtrlMode	=	FALSE;
		m_PageIdx			=	0;

}
//--------------------------------------------------------------------
Cls_GrP4DoSpinPageTxt::~Cls_GrP4DoSpinPageTxt( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinPageTxt::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherite
		Cls_GrP4DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassSpinPageTxt;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoSpinPageTxt);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoSpinPageTxt, sizeof(m_ParaDoSpinPageTxt) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoSpinPageTxt)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPageTxt::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoSpinPageTxt, sizeof(m_ParaDoSpinPageTxt) , m_PtrScpt , E_GrP4DoClassSpinPageTxt );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinPageTxt::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPageTxt::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		// update status
		Tv_Stat	=	E_GrP4DoSpinPageTxtStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoSpinPageTxtStatNormal;
			if ( m_IsCtrlMode ||  m_IsOnMouse )
			{
				Tv_Stat	=	E_GrP4DoSpinPageTxtStatControl;
			}
			else
			{
				if ( IsFocused() )
				{
					Tv_Stat	=	E_GrP4DoSpinPageTxtStatFocus;
					if ( m_ParaDoSpinPageTxt.IsOnKeyCtrl )
					{
						Tv_Stat	=	E_GrP4DoSpinPageTxtStatControl;
					}
				}
			}
		}
		PaintScriptPush( m_ParaDoSpinPageTxt.PntStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoSpinPageTxt::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoSpinPageTxt, V_GrP4DoSpinPageTxtParaInfo, E_GrP4DoSpinPageTxtParaCnt );
}
#endif
//--------------------------------------------------------------------
__u8	Cls_GrP4DoSpinPageTxt::PageIdxGet( void )
{
		return	m_PageIdx;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPageTxt::PageIdxSet( __u8 A_PageIdx )
{
	// local -------------------
	// code --------------------
		if ( (E_GrP4DoSpinPageTxtItemMax > A_PageIdx) && (m_ParaDoSpinPageTxt.ItemCnt > A_PageIdx) )
		{
			m_PageIdx	=	A_PageIdx;
			// update page
			LcPageUpdt();
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoSpinPageTxt::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			Tv_Result	=	V_GrP4Mng->TextPtrGet( m_ParaDoSpinPageTxt.TxtTbl[m_PageIdx] );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPageTxt::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPageTxt::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPageTxt::LcPageInc( void )
{
	// local -------------------
		__u8	Tv_PageIdx;
	// code --------------------
		Tv_PageIdx	=	PageIdxGet() + 1;
		if ( (E_GrP4DoSpinPageTxtItemMax <= Tv_PageIdx) || ((__u32)m_ParaDoSpinPageTxt.ItemCnt <= Tv_PageIdx) )
		{
			if ( m_ParaDoSpinPageTxt.IsValRotate )
			{
				Tv_PageIdx	=	0;
			}
			else
			{
				Tv_PageIdx	=	m_ParaDoSpinPageTxt.ItemCnt;
				if ( 0 != Tv_PageIdx )
				{
					Tv_PageIdx --;
					if ( E_GrP4DoSpinPageTxtItemMax <= Tv_PageIdx )
					{
						Tv_PageIdx	=	E_GrP4DoSpinPageTxtItemMax - 1;
					}
				}
			}
		}
		PageIdxSet( Tv_PageIdx );
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPageTxt::LcPageDec( void )
{
	// local -------------------
		__u8	Tv_PageIdx;
	// code --------------------
		Tv_PageIdx	=	PageIdxGet();
		if ( 0 == Tv_PageIdx  )
		{
			if ( m_ParaDoSpinPageTxt.IsValRotate )
			{
				Tv_PageIdx	=	(__u32)m_ParaDoSpinPageTxt.ItemCnt;
				if ( 0 != Tv_PageIdx )
				{
					Tv_PageIdx --;
					if ( E_GrP4DoSpinPageTxtItemMax <= Tv_PageIdx ) 
					{
						Tv_PageIdx	=	E_GrP4DoSpinPageTxtItemMax - 1;
					}
				}
			}
		}
		else
		{
			Tv_PageIdx --;
		}
		PageIdxSet( Tv_PageIdx );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinPageTxt::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaDoSpinPageTxt.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP4KeyLeft:
					if ( !m_ParaDoSpinPageTxt.IsVertical )
					{
						LcPageDec();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyRight:
					if ( !m_ParaDoSpinPageTxt.IsVertical )
					{
						LcPageInc();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyUp:
					if ( m_ParaDoSpinPageTxt.IsVertical )
					{
						LcPageInc();
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyDown:
					if ( m_ParaDoSpinPageTxt.IsVertical )
					{
						LcPageDec();
						Tv_Result	=	TRUE;
					}
					break;

			}
		}

		// change event
		if ( Tv_Result )
		{
			ReqDraw();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinPageTxt::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( !m_ParaDoSpinPageTxt.IsOnKeyCtrl )
		{
			if ( E_GrP4KeyEnter == A_Key )
			{
				m_IsCtrlMode	=	!m_IsCtrlMode;
				ReqDraw();
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinPageTxt::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__s32		Tv_Ct;
	// code --------------------
		// check position
		if ( ScrRectGet( &Tv_Rc ) )
		{
			if ( m_ParaDoSpinPageTxt.IsVertical )
			{
				// vertical area calculate
				Tv_Ct	=	((Tv_Rc.bottom - Tv_Rc.top) >> 1) + Tv_Rc.top;
				if ( Tv_Ct > (__s32)A_Y )
				{
					LcPageInc();
				}
				else
				{
					LcPageDec();
				}
			}
			else
			{
				// horizontal area calculate
				Tv_Ct	=	((Tv_Rc.right - Tv_Rc.left) >> 1) + Tv_Rc.left;
				if ( Tv_Ct > (__s32)A_X )
				{
					LcPageDec();
				}
				else
				{
					LcPageInc();
				}
			}
			ReqDraw();
		}
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinPageTxt::ProcEvtWheelUp( void )
{
	// local -------------------
	// code --------------------
		LcPageInc();
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoSpinPageTxt::ProcEvtWheelDown( void )
{
	// local -------------------
	// code --------------------
		LcPageDec();
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPageTxt::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlInit();
		// LcPageUpdt();
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoSpinPageTxt::LcPageUpdt( void )
{
	// local -------------------
		__u16	Tv_PageId;
		Cls_GrP4DoFrame*	Tv_Frm;
		St_GrP4MsgFrmReqPageChg	Tv_Msg;
	// code --------------------
		Tv_PageId	=	m_ParaDoSpinPageTxt.PageTbl[m_PageIdx];
		if ( E_GrP4ScptIdNone != Tv_PageId )
		{
			if ( E_GrP4ScptIdNone != m_ParaDoSpinPageTxt.FrameId )
			{
				Tv_Frm	=	(Cls_GrP4DoFrame*)m_ObjWin->FindSlaveObjByScptId( m_ParaDoSpinPageTxt.FrameId );
				if ( NULL != Tv_Frm )
				{
					if ( E_GrP4DoClassFrame == Tv_Frm->ClassIdGet() )
					{
						// request page change
						Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdFrameReqPageChg;
						Tv_Msg.Hd.ConIdx	=	E_GrP4ConIdxFocus;
						Tv_Msg.Hd.WinId		=	((Cls_GrP4DoWin*)m_ObjWin)->WinIdGet();
						Tv_Msg.Hd.ObjId		=	m_ParaDoSpinPageTxt.FrameId;
						Tv_Msg.PageId			=	Tv_PageId;
						Tv_Msg.Layer			=	m_ParaDoSpinPageTxt.FrmLayer;
						Tv_Msg.IsBack			=	FALSE;
						Tv_Msg.IsMovFcs		=	m_ParaDoSpinPageTxt.IsMoveFcs;
						((Cls_GrP4ConBase*)m_ObjCon)->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );
					}
				}
			}
		}
}
//--------------------------------------------------------------------

