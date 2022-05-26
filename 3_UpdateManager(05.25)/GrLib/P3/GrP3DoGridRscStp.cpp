/*
		platform grid record schedule setup

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P3/GrP3DoGridRscStp.h>
#include <GrDebug.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <P3/GrP3Script.h>
#include <P3/GrP3ConBase.h>
#include <P3/GrP3AsmBase.h>
#include <P3/GrP3Mng.h>
#include <P3/GrP3UiCmd.h>
#include <P3/GrP3Cfg.h>

#ifdef GR_P3_EDITOR
#include <P3/Dvl/GrP3DvlBase.h>
#include <P3/Dvl/GrP3DvlMng.h>
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

#define E_GrP3DoGridRscStpParaCnt		21

St_GrP3DvlDoPrInfoData	V_GrP3DoGridRscStpParaInfo[E_GrP3DoGridRscStpParaCnt]	=	
{
	{	L"PntVal0",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->PntVal[0],NULL,			L"Paint item type 0"	},
	{	L"PntVal1",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->PntVal[1],NULL,			L"Paint item type 1"	},
	{	L"PntVal2",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->PntVal[2],NULL,			L"Paint item type 2"	},
	{	L"PntVal3",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->PntVal[3],NULL,			L"Paint item type 3"	},
	{	L"PntVal4",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->PntVal[4],NULL,			L"Paint item type 4"	},
	{	L"PntVal5",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->PntVal[5],NULL,			L"Paint item type 5"	},
	{	L"PntVal6",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->PntVal[6],NULL,			L"Paint item type 6"	},
	{	L"PntVal7",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->PntVal[7],NULL,			L"Paint item type 7"	},

	{	L"ValTbl0",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->ValTbl[0],NULL,			L"item value 0"	},
	{	L"ValTbl1",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->ValTbl[1],NULL,			L"item value 1"	},
	{	L"ValTbl2",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->ValTbl[2],NULL,			L"item value 2"	},
	{	L"ValTbl3",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->ValTbl[3],NULL,			L"item value 3"	},
	{	L"ValTbl4",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->ValTbl[4],NULL,			L"item value 4"	},
	{	L"ValTbl5",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->ValTbl[5],NULL,			L"item value 5"	},
	{	L"ValTbl6",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->ValTbl[6],NULL,			L"item value 6"	},
	{	L"ValTbl7",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->ValTbl[7],NULL,			L"item value 7"	},

	{	L"WeekCnt",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->WeekCnt, NULL,	L"Week count"	},
	//{	L"TimeCnt",				TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->TimeCnt,NULL,	L"time count"	},
	{	L"ValCnt",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->ValCnt, NULL,	L"value count"	},
	{	L"IsAfterChg",		TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->IsAfterChg, NULL,	L"when click do change value after area"	},

	{	L"PntSel",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->PntSel, NULL,	L"paint select area"	},

	{	L"MseValIdx",			TRUE,		E_GrP3DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->MseValIdx,NULL,	L"GVU index of mouse draw value"	},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoGridRscStp::Cls_GrP3DoGridRscStp( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoGridBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassGridRscStp;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaGridRscStp) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaGridRscStp, sizeof(m_ParaGridRscStp) );
		m_ParaGridBase.IsChSelAble	=	TRUE;

		m_ItemCntX		=	E_GrP3CfgRschdAtbDayCnt;
		m_ItemCntY		=	(__u16)m_ParaGridRscStp.WeekCnt;

		m_Ch				=	0;
		m_IsSel			=	FALSE;
		
		m_PtrGvu		=	(__s32*)V_GrP3Mng->GvuTblGet();

}
//--------------------------------------------------------------------
Cls_GrP3DoGridRscStp::~Cls_GrP3DoGridRscStp( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridRscStp::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoGridBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassGridRscStp;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaGridRscStp);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaGridRscStp, sizeof(m_ParaGridRscStp) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaGridRscStp)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridRscStp::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoGridBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaGridRscStp, sizeof(m_ParaGridRscStp) , m_PtrScpt , E_GrP3DoClassGridRscStp );

		// update
		m_ItemCntY		=	(__u16)m_ParaGridRscStp.WeekCnt;
		if ( (7 + E_GrP3CfgHoliSchdCnt) < m_ItemCntY )
		{
			m_ItemCntY	=	7 + E_GrP3CfgHoliSchdCnt;
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoGridRscStp::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiChGet:
				A_PtrInt[0]	=	(__s32)m_Ch;
				break;
			case E_GrP3UpiChSet:
				m_Ch	=	(__u8)A_PtrInt[1];
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoGridBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridRscStp::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoGridBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaGridRscStp, V_GrP3DoGridRscStpParaInfo, E_GrP3DoGridRscStpParaCnt );
#endif
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DoGridRscStp::LcValueToPst( __u8 A_Value )
{
	// local -------------------
		__u8	Tv_ItmIdx;
	// code --------------------
		Tv_ItmIdx	=	LcValueToItemIdx( A_Value );
		return	m_ParaGridRscStp.PntVal[Tv_ItmIdx];
}
//--------------------------------------------------------------------
__u8	Cls_GrP3DoGridRscStp::LcValueGet( __u16 A_X, __u16 A_Y )
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_Ofs;
		__u8*	Tv_PtrVal;
	// code --------------------
		Tv_Result	=	0;

		if ( (A_X < E_GrP3CfgRschdAtbDayCnt) && (A_Y < m_ItemCntY) )
		{
			Tv_Ofs	=	( (E_GrP3CfgRschdAtbDayCnt * (7 + E_GrP3CfgHoliSchdCnt)) * (__u32)m_Ch ) + 
				(E_GrP3CfgRschdAtbDayCnt * A_Y) + (__u32)A_X;
			Tv_Ofs	=	Tv_Ofs + E_GrP3CfgOfsRecSchd;
			if ( E_GrP3MngSdaSize > Tv_Ofs )
			{
				Tv_PtrVal	=	(__u8*)V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
				Tv_Result	=	*Tv_PtrVal;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridRscStp::LcProcKeyItemSel( void )
{
	// local -------------------
		__u8	Tv_Val;
		__u16	Tv_Idx;
		__u16	Tv_Cnt;
	// code --------------------
		// get target value
		Tv_Val	=	LcValueGet( m_CusX, m_CusY );
		Tv_Idx	=	(__u8)LcValueToItemIdx( Tv_Val );
		Tv_Idx ++;	// change index
		if ( m_ParaGridRscStp.ValCnt <= (__u8)Tv_Idx )
		{
			Tv_Idx	=	0;
		}
		Tv_Val	=	LcItemIdxToValue( (__u8)Tv_Idx );

		Tv_Cnt	=	1;
		if ( m_ParaGridRscStp.IsAfterChg )
		{
			Tv_Cnt	=	E_GrP3CfgRschdAtbDayCnt - m_CusX;
		}
		for ( Tv_Idx=0;Tv_Idx < Tv_Cnt;Tv_Idx++)
		{
			LcValueSet( m_CusX + Tv_Idx, m_CusY, Tv_Val );
		}
		// draw
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridRscStp::LcValueSet( __u16 A_X, __u16 A_Y, __u8 A_Val )
{
	// local -------------------
		__u32	Tv_Ofs;
		__u8*	Tv_PtrVal;
	// code --------------------
		Tv_Ofs	=	( (__u32)m_Ch * (E_GrP3CfgRschdAtbDayCnt * (7 + E_GrP3CfgHoliSchdCnt)) ) + 
			( E_GrP3CfgRschdAtbDayCnt * (__u32)A_Y ) + (__u32)A_X;
		Tv_Ofs	=	Tv_Ofs + E_GrP3CfgOfsRecSchd;
		// check over
		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal		=	(__u8*)V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			*Tv_PtrVal	=	A_Val;
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DoGridRscStp::LcValueSet - value offset is over.\n" );
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrP3DoGridRscStp::LcValueToItemIdx( __u8 A_Value )
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_ValCnt;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	0;
		if ( 0 != m_ParaGridRscStp.ValCnt )
		{
			Tv_ValCnt	=	m_ParaGridRscStp.ValCnt;
			if ( E_GrP3DoGridRscStpItemCnt < Tv_ValCnt )
			{
				Tv_ValCnt	=	E_GrP3DoGridRscStpItemCnt;
			}
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_ValCnt;Tv_WkIdx++ )
			{
				if ( A_Value == m_ParaGridRscStp.ValTbl[Tv_WkIdx] )
				{
					Tv_Result	=	Tv_WkIdx;
					break;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP3DoGridRscStp::LcItemIdxToValue( __u8 A_ItemIdx )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		
		if ( (A_ItemIdx < m_ParaGridRscStp.ValCnt) && (A_ItemIdx < E_GrP3DoGridRscStpItemCnt) )
		{
			Tv_Result	=	m_ParaGridRscStp.ValTbl[A_ItemIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridRscStp::LcPainItems( void )
{
	// local -------------------
		__s32		Tv_PosX;
		__s32		Tv_PosY;
		__u16	Tv_IdxX;
		__u16	Tv_IdxY;
		__u16	Tv_PstCode;
		__u32	Tv_Ofs;
		St_GrRect	Tv_RcItem;
		__u8*	Tv_PtrVal;
	// code --------------------
		// check item drawable
		if ( (0 != m_ItemCntY) && (0 != m_ParaGridBase.ItemSizeX) && (0 != m_ParaGridBase.ItemSizeY) )
		{
			// check start position
			if (	(m_Rect.right > (m_Rect.left + (__s32)m_ParaGridBase.ItemPosX)) && 
				(m_Rect.bottom > (m_Rect.top + (__s32)m_ParaGridBase.ItemPosY)) )
			{
				// get item pointer
				Tv_Ofs		=	(E_GrP3CfgRschdAtbDayCnt * (7 + E_GrP3CfgHoliSchdCnt)) * (__u32)m_Ch + E_GrP3CfgOfsRecSchd;
				if ( E_GrP3MngSdaSize >= (Tv_Ofs + (E_GrP3CfgRschdAtbDayCnt * (7 + E_GrP3CfgHoliSchdCnt))) )
				{
					// init
					Tv_PtrVal	=	(__u8*)V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
					// draw item
					Tv_PosY	=	m_PntRcScr.top + (__s32)m_ParaGridBase.ItemPosY;
					for (Tv_IdxY=0;Tv_IdxY < m_ItemCntY;Tv_IdxY++)
					{
						Tv_PosX	=	m_PntRcScr.left + (__s32)m_ParaGridBase.ItemPosX;
						for ( Tv_IdxX=0;Tv_IdxX < E_GrP3CfgRschdAtbDayCnt;Tv_IdxX++ )
						{
							// check drawable position
							if ( (m_PntRcScr.right > Tv_PosX) && ( m_PntRcScr.bottom > Tv_PosY ) )
							{
								// get value
								Tv_PstCode	=	LcValueToPst( *Tv_PtrVal );
								if ( E_GrP3ScptPstIdNone != Tv_PstCode )
								{
									Tv_RcItem.left		=	Tv_PosX;
									Tv_RcItem.top			=	Tv_PosY;
									Tv_RcItem.right		=	Tv_PosX + (__s32)m_ParaGridBase.ItemSizeX;
									Tv_RcItem.bottom	=	Tv_PosY + (__s32)m_ParaGridBase.ItemSizeY;
									PaintScriptPush( Tv_PstCode, &Tv_RcItem );
								}
							}
							// next x
							Tv_PosX	=	Tv_PosX + (__s32)m_ParaGridBase.ItemSizeX;
							Tv_PtrVal ++;
						}
						// next y
						Tv_PosY	=	Tv_PosY + (__s32)m_ParaGridBase.ItemSizeY;
					}

				}	// if ( E_GrP3MngSdaSize >= (Tv_Ofs + Tv_ChSize) )

			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoGridRscStp::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		if ( LcScrPosToItemPos( A_X, A_Y, &m_SelStX, &m_SelStY ) )
		{
			// update mouse position
			m_SelEdX	=	m_SelStX;
			m_SelEdY	=	m_SelStY;
			m_CusX		=	m_SelEdX;
			m_CusY		=	m_SelEdY;
			m_IsSel		=	TRUE;
			ReqDraw();
			// set mouse capture
			((Cls_GrP3ConBase*)m_ObjCon)->MouseCapture();
		}
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoGridRscStp::ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__u16	Tv_WkX;
		__u16	Tv_WkY;
	// code --------------------
		if ( m_IsSel )
		{
			// check mouse button
			if ( 0 != (E_GrP3MouseBmkkLeft & A_BtnMap) )
			{
				if ( LcScrPosToItemPos( A_X, A_Y, &Tv_WkX, &Tv_WkY ) )
				{
					// check different
					if ( (Tv_WkX != m_SelEdX) || (Tv_WkY != m_SelEdY) )
					{
						m_SelEdX	=	Tv_WkX;
						m_SelEdY	=	Tv_WkY;
						ReqDraw();
					}
				}
			}
			else
			{
				m_IsSel	=	FALSE;
			}
		}
		
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoGridRscStp::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__u8	Tv_Val;
		__u8	Tv_Itm;
		__u16	Tv_StX;
		__u16	Tv_StY;
		__u16	Tv_EdX;
		__u16	Tv_EdY;
		__u16	Tv_WkX;
		__u16	Tv_WkY;
	// code --------------------
		if ( m_IsSel )
		{
			// select off
			m_IsSel	=	FALSE;

			if ( LcScrPosToItemPos( A_X, A_Y, &m_SelEdX, &m_SelEdY ) )
			{
				m_CusX	=	m_SelEdX;
				m_CusY	=	m_SelEdY;
				// check equal position
				if ( (m_SelStX == m_SelEdX) && (m_SelStY == m_SelEdY) && m_ParaGridRscStp.IsAfterChg )
				{
					// get target value
					Tv_Val	=	LcValueGet( m_SelEdX, m_SelEdY );
					Tv_Itm	=	LcValueToItemIdx( Tv_Val );
					Tv_Itm ++;	// change index
					if ( m_ParaGridRscStp.ValCnt <= Tv_Itm )
					{
						Tv_Itm	=	0;
					}
					Tv_Val	=	LcItemIdxToValue( Tv_Itm );
					Tv_StX	=	m_SelStX;
					Tv_StY	=	m_SelStY;
					Tv_EdX	=	E_GrP3CfgRschdAtbDayCnt - 1;
					Tv_EdY	=	Tv_StY;
				}
				else
				{
					if ( m_SelStX < m_SelEdX )
					{
						Tv_StX	=	m_SelStX;
						Tv_EdX	=	m_SelEdX;
					}
					else
					{
						Tv_StX	=	m_SelEdX;
						Tv_EdX	=	m_SelStX;
					}
					if ( m_SelStY < m_SelEdY )
					{
						Tv_StY	=	m_SelStY;
						Tv_EdY	=	m_SelEdY;
					}
					else
					{
						Tv_StY	=	m_SelEdY;
						Tv_EdY	=	m_SelStY;
					}

					Tv_Val	=	0;
					if ( E_GrP3MngGvuMaxCnt > m_ParaGridRscStp.MseValIdx )
					{
						Tv_Val	=	(__u8)m_PtrGvu[m_ParaGridRscStp.MseValIdx];
					}
					
				}

				// change value
				Tv_WkY	=	Tv_StY;
				while ( Tv_WkY <= Tv_EdY )
				{
					Tv_WkX	=	Tv_StX;
					while ( Tv_WkX <= Tv_EdX )
					{
						LcValueSet( Tv_WkX, Tv_WkY, Tv_Val );
						// next x
						Tv_WkX ++;
					}
					// next y
					Tv_WkY ++;
				}
			}
			// draw
			ReqDraw();
		}

		((Cls_GrP3ConBase*)m_ObjCon)->MouseRelease();

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridRscStp::LcPaintSelect( void )
{
	// local -------------------
		St_GrRect	Tv_RcSel;
		__u16	Tv_StX;
		__u16	Tv_StY;
		__u16	Tv_EdX;
		__u16	Tv_EdY;
	// code --------------------
		// draw select
		
		if ( m_IsSel && (E_GrP3ScptPstIdNone != m_ParaGridRscStp.PntSel) && (IsOnMouse()) )
		{
			// align
			if ( m_SelStX < m_SelEdX )
			{
				Tv_StX	=	m_SelStX;
				Tv_EdX	=	m_SelEdX;
			}
			else
			{
				Tv_StX	=	m_SelEdX;
				Tv_EdX	=	m_SelStX;
			}
			if ( m_SelStY < m_SelEdY )
			{
				Tv_StY	=	m_SelStY;
				Tv_EdY	=	m_SelEdY;
			}
			else
			{
				Tv_StY	=	m_SelEdY;
				Tv_EdY	=	m_SelStY;
			}

			Tv_RcSel.left		=	m_PntRcScr.left + (__s32)m_ParaGridBase.ItemPosX;
			Tv_RcSel.top		=	m_PntRcScr.top + (__s32)m_ParaGridBase.ItemPosY;
			Tv_RcSel.right	=	Tv_RcSel.left;
			Tv_RcSel.bottom	=	Tv_RcSel.top;

			Tv_RcSel.left		=	Tv_RcSel.left + ( (__s32)Tv_StX * (__s32)m_ParaGridBase.ItemSizeX );
			Tv_RcSel.top		=	Tv_RcSel.top + ( (__s32)Tv_StY * (__s32)m_ParaGridBase.ItemSizeY );
			Tv_RcSel.right	=	Tv_RcSel.right + ( (__s32)Tv_EdX * (__s32)m_ParaGridBase.ItemSizeX ) + (__s32)m_ParaGridBase.ItemSizeX;
			Tv_RcSel.bottom	=	Tv_RcSel.bottom + ( (__s32)Tv_EdY * (__s32)m_ParaGridBase.ItemSizeY ) + (__s32)m_ParaGridBase.ItemSizeY;

			PaintScriptPush( m_ParaGridRscStp.PntSel, &Tv_RcSel );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridRscStp::ProcUiMsg( Ptr_GrP3MsgHd A_PtrMsg )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		//default event handler
		switch( A_PtrMsg->Cmd )
		{
			case E_GrP3MsgCmdSpinValChg:
				Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( ((Ptr_GrP3MsgObj)A_PtrMsg)->ObjId );
				if ( NULL != Tv_Obj )
				{
					ProcEvtSpinValChg( Tv_Obj );
				}
				break;
			default:
				Cls_GrP3DoBase::ProcUiMsg( A_PtrMsg );
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridRscStp::ProcEvtSpinValChg( Cls_GrP3DoBase* A_Obj )
{
	// local -------------------
		__s32	Tv_Rint[2];
	// code --------------------
		Tv_Rint[0]	=	0;
		A_Obj->RtlCommand( E_GrP3UpiValueGet, Tv_Rint, NULL );
		//if ( (__u8)Tv_Rint[0] < m_ParaGridRscStp.ChCnt )
		{
			m_Ch	=	(__u8)Tv_Rint[0];
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridRscStp::ProcEvtMseLeave( void )
{
	// local -------------------
	// code --------------------
		//m_IsSel		=	FALSE;
		Cls_GrP3DoGridBase::ProcEvtMseLeave();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridRscStp::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsSel	=	FALSE;
		Cls_GrP3DoGridBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
