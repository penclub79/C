/*
		platform grid base

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P4/GrP4DoGridBase.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <P4/GrP4Script.h>
#include <P4/GrP4ConBase.h>
#include <P4/GrP4AsmBase.h>
#include <P4/GrP4Mng.h>
#include <P4/GrP4UiCmd.h>

#ifdef GR_P4_EDITOR
#include <P4/Dvl/GrP4DvlBase.h>
#include <P4/Dvl/GrP4DvlMng.h>
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

#define E_GrP4DoGridBaseParaCnt		14

St_GrP4DvlDoPrInfoData	V_GrP4DoGridBaseParaInfo[E_GrP4DoGridBaseParaCnt]	=	
{
	{	L"PntBgStatDis",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->PntBg[E_GrP4DoGridBgStatDisable],NULL,			L"Paint background status disable"	},
	{	L"PntBgStatNrm",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->PntBg[E_GrP4DoGridBgStatNormal],NULL,			L"Paint background status normal"	},
	{	L"PntBgStatCus",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->PntBg[E_GrP4DoGridBgStatOnCus],NULL,			L"Paint background status on cursor"	},

	{	L"PntCusItem",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->PntCusItem,NULL,			L"Paint cursor on item"	},
	{	L"PntCusVu",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->PntCusVu,NULL,			L"Paint cursor vertical up"	},
	{	L"PntCusVd",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->PntCusVd,NULL,			L"Paint cursor vertical down"	},
	{	L"PntCusHl",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->PntCusHl,NULL,			L"Paint cursor horizontal left"	},
	{	L"PntCusHr",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->PntCusHr,NULL,			L"Paint cursor horizontal right"	},

	{	L"ItemPosX",				TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->ItemPosX,NULL,			L"item area position x"	},
	{	L"ItemPosY",				TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->ItemPosY,NULL,			L"item area position y"	},

	{	L"ItemSizeX",				TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->ItemSizeX,NULL,			L"item unit size x"	},
	{	L"ItemSizeY",				TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->ItemSizeY,NULL,			L"item unit size y"	},

	{	L"IsOnKyeCtrl",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->IsOnKeyCtrl,NULL,			L"key control able at focus"	},
	{	L"IsChSelAble",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoGridBasePara)0)->IsChSelAble,NULL,			L"channel select able"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoGridBase::Cls_GrP4DoGridBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId			=	E_GrP4DoClassGridBase;
		m_ParaSize		=	m_ParaSize + sizeof(m_ParaGridBase) + sizeof(St_GrP4ScptParaHd);
		m_IsKeyAble		=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaGridBase, sizeof(m_ParaGridBase) );
		m_ParaGridBase.ItemSizeX	=	8;
		m_ParaGridBase.ItemSizeY	=	8;

		m_ItemCntX		=	4;
		m_ItemCntY		=	4;

		m_IsCtrlMode	=	FALSE;

		m_CusX				=	0;
		m_CusY				=	0;

}
//--------------------------------------------------------------------
Cls_GrP4DoGridBase::~Cls_GrP4DoGridBase( void )
{
		//
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoGridBase::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassGridBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaGridBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaGridBase, sizeof(m_ParaGridBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaGridBase)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoGridBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaGridBase, sizeof(m_ParaGridBase) , m_PtrScpt , E_GrP4DoClassGridBase );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiItemSizeSet:
				if ( (0 < A_PtrInt[2]) &&  (0 < A_PtrInt[3]) )
				{
					m_ParaGridBase.ItemSizeX	=	(__u16)A_PtrInt[2];
					m_ParaGridBase.ItemSizeY	=	(__u16)A_PtrInt[3];
					ReqDraw();
				}
				break;
			case E_GrP4UpiItemSizeGet:
				A_PtrInt[0]	=	m_ParaGridBase.ItemSizeX;
				A_PtrInt[1]	=	m_ParaGridBase.ItemSizeY;
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridBase::ProcPaint( void )
{
	// local -------------------
		St_GrRect	Tv_RcItem;
		St_GrRect	Tv_RcCus;
		__u8	Tv_Stat;
	// code --------------------

		// init
		Tv_RcItem.left		=	m_PntRcScr.left + (__s32)m_ParaGridBase.ItemPosX + ( (__s32)m_ParaGridBase.ItemSizeX * (__s32)m_CusX );
		Tv_RcItem.top			=	m_PntRcScr.top + (__s32)m_ParaGridBase.ItemPosY + ( (__s32)m_ParaGridBase.ItemSizeY * (__s32)m_CusY );
		Tv_RcItem.right		=	Tv_RcItem.left + (__s32)m_ParaGridBase.ItemSizeX;
		Tv_RcItem.bottom	=	Tv_RcItem.top + (__s32)m_ParaGridBase.ItemSizeY;

		// check channel select able
		if ( m_ParaGridBase.IsChSelAble )
		{
			// draw item cursor
			if ( (m_PntRcScr.right > Tv_RcItem.left) && (m_PntRcScr.bottom > Tv_RcItem.top) && 
				(E_GrP4ScptPstIdNone != m_ParaGridBase.PntCusItem) )
			{
				PaintScriptPush( m_ParaGridBase.PntCusItem, &Tv_RcItem );
			}

			// draw horizontal cursor left
			if ( E_GrP4ScptPstIdNone != m_ParaGridBase.PntCusHl )
			{
				Tv_RcCus.left		=	m_PntRcScr.left;
				Tv_RcCus.top		=	Tv_RcItem.top;
				Tv_RcCus.right	=	Tv_RcItem.left;
				Tv_RcCus.bottom	=	Tv_RcItem.bottom;
				if ( (m_PntRcScr.right > Tv_RcCus.left) && (m_PntRcScr.bottom > Tv_RcCus.top) )
				{
					PaintScriptPush( m_ParaGridBase.PntCusHl, &Tv_RcCus );
				}
			}

			// draw horizontal cursor right
			if ( E_GrP4ScptPstIdNone != m_ParaGridBase.PntCusHr )
			{
				Tv_RcCus.left		=	Tv_RcItem.right;
				Tv_RcCus.top		=	Tv_RcItem.top;
				Tv_RcCus.right	=	m_PntRcScr.right;
				Tv_RcCus.bottom	=	Tv_RcItem.bottom;
				if ( (m_PntRcScr.right > Tv_RcCus.left) && (m_PntRcScr.bottom > Tv_RcCus.top) )
				{
					PaintScriptPush( m_ParaGridBase.PntCusHr, &Tv_RcCus );
				}
			}

			// draw vertical cursor up
			if ( E_GrP4ScptPstIdNone != m_ParaGridBase.PntCusVu )
			{
				Tv_RcCus.left		=	Tv_RcItem.left;
				Tv_RcCus.top		=	m_PntRcScr.top;
				Tv_RcCus.right	=	Tv_RcItem.right;
				Tv_RcCus.bottom	=	Tv_RcItem.top;
				if ( (m_PntRcScr.right > Tv_RcCus.left) && (m_PntRcScr.bottom > Tv_RcCus.top) )
				{
					PaintScriptPush( m_ParaGridBase.PntCusVu, &Tv_RcCus );
				}
			}

			// draw vertical cursor down
			if ( E_GrP4ScptPstIdNone != m_ParaGridBase.PntCusVd )
			{
				Tv_RcCus.left		=	Tv_RcItem.left;
				Tv_RcCus.top		=	Tv_RcItem.bottom;
				Tv_RcCus.right	=	Tv_RcItem.right;
				Tv_RcCus.bottom	=	m_PntRcScr.bottom;
				if ( (m_PntRcScr.right > Tv_RcCus.left) && (m_PntRcScr.bottom > Tv_RcCus.top) )
				{
					PaintScriptPush( m_ParaGridBase.PntCusVd, &Tv_RcCus );
				}
			}

		}
		else
		{
			// draw vertical cursor 
			if ( E_GrP4ScptPstIdNone != m_ParaGridBase.PntCusVu )
			{
				Tv_RcCus.left		=	Tv_RcItem.left;
				Tv_RcCus.top		=	m_PntRcScr.top;
				Tv_RcCus.right	=	Tv_RcItem.right;
				Tv_RcCus.bottom	=	m_PntRcScr.bottom;
				if ( (m_PntRcScr.right > Tv_RcCus.left) && (m_PntRcScr.bottom > Tv_RcCus.top) )
				{
					PaintScriptPush( m_ParaGridBase.PntCusVu, &Tv_RcCus );
				}
			}
		}


		// select area
		LcPaintSelect();
		// paint items
		LcPainItems();
		
		// back ground
		Tv_Stat		=	E_GrP4DoGridBgStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP4DoGridBgStatNormal;
			if ( m_IsOnMouse || IsFocused() )
			{
				Tv_Stat	=	E_GrP4DoGridBgStatOnCus;
			}
		}
		PaintScriptPush( m_ParaGridBase.PntBg[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoGridBase::RtlDvlParaInfoSubDump(void)
{
		// inherite
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaGridBase, V_GrP4DoGridBaseParaInfo, E_GrP4DoGridBaseParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoGridBase::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsCtrlMode	=	FALSE;
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaGridBase.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP4KeyLeft:
					if ( 0 != m_CusX )
					{
						m_CusX --;
						Tv_Result	=	TRUE;
					}
					else if ( m_ScptId == m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxLeft] )
					{
						if ( 0 != m_ItemCntX )
						{
							m_CusX	=	m_ItemCntX - 1;
							Tv_Result	=	TRUE;
						}
					}
					break;
				case E_GrP4KeyRight:
					if ( (0 != m_ItemCntX) && (m_ItemCntX > (m_CusX + 1)) )
					{
						m_CusX ++;
						Tv_Result	=	TRUE;
					}
					else if ( m_ScptId == m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxRight] )
					{
						m_CusX	=	0;
						Tv_Result	=	TRUE;
					}
					break;
				case E_GrP4KeyUp:
					if ( m_ParaGridBase.IsChSelAble )
					{
						if ( 0 != m_CusY )
						{
							m_CusY --;
							Tv_Result	=	TRUE;
						}
						else if ( m_ScptId == m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxUp] )
						{
							if ( 0 != m_ItemCntY )
							{
								m_CusY	=	m_ItemCntY - 1;
								Tv_Result	=	TRUE;
							}
						}
					}
					break;
				case E_GrP4KeyDown:
					if ( m_ParaGridBase.IsChSelAble )
					{
						if ( (0 != m_ItemCntY) && (m_ItemCntY > (m_CusY + 1) ) )
						{
							m_CusY ++;
							Tv_Result	=	TRUE;
						}
						else if ( m_ScptId == m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxDown] )
						{
							m_CusY	=	0;
							Tv_Result	=	TRUE;
						}
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
BOOL8	Cls_GrP4DoGridBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsSel;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_IsSel	=	FALSE;
		if ( E_GrP4KeyEnter == A_Key )
		{
			// check already control mode
			if ( m_ParaGridBase.IsOnKeyCtrl )
			{
				Tv_IsSel	=	TRUE;
			}
			else
			{
				if ( m_IsCtrlMode )
				{
					Tv_IsSel	=	TRUE;
				}
				else
				{
					m_IsCtrlMode	=	TRUE;
				}
			}
			// select mode
			if ( Tv_IsSel )
			{
				LcProcKeyItemSel();
			}
			ReqDraw();
			Tv_Result	=	TRUE;
		}
		else if ( (E_GrP4KeyEsc == A_Key) || (E_GrP4KeyMenu == A_Key) )
		{
			if ( (!m_ParaGridBase.IsOnKeyCtrl) && m_IsCtrlMode )
			{
				m_IsCtrlMode	=	FALSE;
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridBase::ProcEvtFocusDirection( __u8 A_Direction, __u32 A_Atb )
{
	// local -------------------
	// code --------------------
		// set cursor
		switch ( A_Direction )
		{
			case E_GrP4DoFocusMvIdxLeft:
				if ( 0 != m_ItemCntX )
				{
					m_CusX	=	m_ItemCntX - 1;
				}
				break;
			case E_GrP4DoFocusMvIdxRight:
				m_CusX	=	0;
				break;
			case E_GrP4DoFocusMvIdxUp:
				if ( 0 != m_ItemCntY )
				{
					m_CusY	=	m_ItemCntY - 1;
				}
				break;
			case E_GrP4DoFocusMvIdxDown:
				m_CusY	=	0;
				break;
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridBase::LcProcKeyItemSel( void )
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridBase::LcScrPosToItemPos( __u16 A_X, __u16 A_Y, __u16* A_PtrRtX, __u16* A_PtrRtY )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrRect	Tv_RcItem;
		__s32		Tv_PosX;
		__s32		Tv_PosY;
		__s32		Tv_IdxX;
		__s32		Tv_IdxY;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( (0 != m_ItemCntX) && (0 != m_ItemCntY) && (0 != m_ParaGridBase.ItemSizeX) && (0 != m_ParaGridBase.ItemSizeY) )
		{
			// calculate position
			Tv_RcItem.left		=	m_PntRcScr.left + (__s32)m_ParaGridBase.ItemPosX;
			Tv_RcItem.top			=	m_PntRcScr.top + (__s32)m_ParaGridBase.ItemPosY;
			Tv_RcItem.right		=	Tv_RcItem.left + ( (__s32)m_ParaGridBase.ItemSizeX * (__s32)m_ItemCntX );
			Tv_RcItem.bottom	=	Tv_RcItem.top + ( (__s32)m_ParaGridBase.ItemSizeY * (__s32)m_ItemCntY );

			if ( m_PntRcScr.right < Tv_RcItem.right )
			{
				Tv_RcItem.right	=	m_PntRcScr.right;
			}
			if ( m_PntRcScr.bottom < Tv_RcItem.bottom )
			{
				Tv_RcItem.bottom	=	m_PntRcScr.bottom;
			}
			// check able
			if ( (m_PntRcScr.right > Tv_RcItem.left) && (m_PntRcScr.bottom > Tv_RcItem.top) )
			{
				// patch position
				Tv_PosX	=	(__s32)A_X;
				Tv_PosY	=	(__s32)A_Y;
				if ( Tv_RcItem.left > Tv_PosX )
				{
					Tv_PosX	=	Tv_RcItem.left;
				}
				if ( Tv_RcItem.top > Tv_PosY )
				{
					Tv_PosY	=	Tv_RcItem.top;
				}
				// calculate item position
				Tv_IdxX	=	(Tv_PosX - Tv_RcItem.left) / m_ParaGridBase.ItemSizeX;
				Tv_IdxY	=	(Tv_PosY - Tv_RcItem.top) / m_ParaGridBase.ItemSizeY;
				// check size
				if ( m_ItemCntX <= Tv_IdxX )
				{
					Tv_IdxX	=	m_ItemCntX - 1;
				}
				if ( m_ItemCntY <= Tv_IdxY )
				{
					Tv_IdxY	=	m_ItemCntY - 1;
				}
				// update cursor position
				*A_PtrRtX	=	(__u16)Tv_IdxX;
				*A_PtrRtY	=	(__u16)Tv_IdxY;
				Tv_Result	=	TRUE;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridBase::LcPainItems( void )
{
		// 
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridBase::LcPaintSelect( void )
{

}
//--------------------------------------------------------------------
