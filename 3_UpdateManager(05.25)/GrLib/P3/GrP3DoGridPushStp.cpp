/*
		platform grid push-server schedule setup

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P3/GrP3DoGridPushStp.h>
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
#define E_GrP3GridPushStpHourCnt			24

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

#define E_GrP3DoGridPushStpParaCnt		22

St_GrP3DvlDoPrInfoData	V_GrP3DoGridPushStpParaInfo[E_GrP3DoGridPushStpParaCnt]	=	
{
	{	L"PntVal0",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->PntVal[0],NULL,			L"Paint item type 0"	},
	{	L"PntVal1",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->PntVal[1],NULL,			L"Paint item type 1"	},
	{	L"PntVal2",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->PntVal[2],NULL,			L"Paint item type 2"	},
	{	L"PntVal3",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->PntVal[3],NULL,			L"Paint item type 3"	},
	{	L"PntVal4",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->PntVal[4],NULL,			L"Paint item type 4"	},
	{	L"PntVal5",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->PntVal[5],NULL,			L"Paint item type 5"	},
	{	L"PntVal6",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->PntVal[6],NULL,			L"Paint item type 6"	},
	{	L"PntVal7",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->PntVal[7],NULL,			L"Paint item type 7"	},

	{	L"ValTbl0",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->ValTbl[0],NULL,			L"item value 0"	},
	{	L"ValTbl1",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->ValTbl[1],NULL,			L"item value 1"	},
	{	L"ValTbl2",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->ValTbl[2],NULL,			L"item value 2"	},
	{	L"ValTbl3",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->ValTbl[3],NULL,			L"item value 3"	},
	{	L"ValTbl4",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->ValTbl[4],NULL,			L"item value 4"	},
	{	L"ValTbl5",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->ValTbl[5],NULL,			L"item value 5"	},
	{	L"ValTbl6",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->ValTbl[6],NULL,			L"item value 6"	},
	{	L"ValTbl7",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->ValTbl[7],NULL,			L"item value 7"	},

	{	L"WeekCnt",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->WeekCnt, NULL,	L"Week count"	},
	//{	L"TimeCnt",				TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoGridRscStpPara)0)->TimeCnt,NULL,	L"time count"	},
	{	L"ValCnt",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->ValCnt, NULL,	L"value count"	},
	{	L"IsAfterChg",		TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->IsAfterChg, NULL,	L"when click do change value after area"	},

	{	L"PntSel",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->PntSel, NULL,	L"paint select area"	},

	{	L"MseValIdx",			TRUE,		E_GrP3DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->MseValIdx, NULL,	L"GVU index of mouse draw value"	},
	{	L"AsmChgVal",			TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoGridPushStpPara)0)->AsmChgVal, NULL,	L"assembly value change event code"	},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoGridPushStp::Cls_GrP3DoGridPushStp( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoGridBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassGridPushStp;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaGridPushStp) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaGridPushStp, sizeof(m_ParaGridPushStp) );
		m_ParaGridBase.IsChSelAble	=	TRUE;

		m_ItemCntX		=	E_GrP3GridPushStpHourCnt;
		m_ItemCntY		=	(__u16)m_ParaGridPushStp.WeekCnt;

		m_Ch				=	0;
		m_IsSel			=	FALSE;
		
		m_PtrGvu		=	(__s32*)V_GrP3Mng->GvuTblGet();

}
//--------------------------------------------------------------------
Cls_GrP3DoGridPushStp::~Cls_GrP3DoGridPushStp( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPushStp::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassGridPushStp;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaGridPushStp);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaGridPushStp, sizeof(m_ParaGridPushStp) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaGridPushStp)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPushStp::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoGridBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaGridPushStp, sizeof(m_ParaGridPushStp) , m_PtrScpt , E_GrP3DoClassGridPushStp );

		// update
		m_ItemCntY		=	(__u16)m_ParaGridPushStp.WeekCnt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoGridPushStp::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
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
void	Cls_GrP3DoGridPushStp::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoGridBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaGridPushStp, V_GrP3DoGridPushStpParaInfo, E_GrP3DoGridPushStpParaCnt );
#endif
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DoGridPushStp::LcSchdGet( __u16 A_X, __u16 A_Y )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_Ofs;
		PDWORD	Tv_PtrVal;
	// code --------------------
		Tv_Result	=	0;

		if ( (A_X < E_GrP3GridPushStpHourCnt) && (A_Y < m_ItemCntY) )
		{
			Tv_Ofs	=	((__u32)m_Ch * sizeof(__u32)) + E_GrP3CfgOfsPushScd;
			if ( E_GrP3MngSdaSize > Tv_Ofs )
			{
				Tv_PtrVal	=	(__u32*)V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
				Tv_Result		= *Tv_PtrVal;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPushStp::LcProcKeyItemSel( void )
{
	// local -------------------
		__u16	Tv_Idx;
		__u16	Tv_Cnt;
		__u32	Tv_SchdMsk;
	// code --------------------
		Tv_SchdMsk	= LcSchdGet(m_CusX, m_CusY);
		if(0 != (Tv_SchdMsk & (1 << m_CusX)))
		{
			Tv_SchdMsk = Tv_SchdMsk & ~(1 << m_CusX);
		}
		else
		{
			Tv_SchdMsk = Tv_SchdMsk | (1 << m_CusX);
		}

		Tv_Cnt	=	1;
		if ( m_ParaGridPushStp.IsAfterChg )
		{
			Tv_Cnt	=	E_GrP3GridPushStpHourCnt - m_CusX;
		}
		for ( Tv_Idx=0;Tv_Idx < Tv_Cnt;Tv_Idx++)
		{
			LcSchdSet( m_CusX + Tv_Idx, m_CusY, Tv_SchdMsk );
		}
		// draw
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPushStp::LcSchdSet( __u16 A_X, __u16 A_Y, __u32 A_SchdMsk )
{
	// local -------------------
		__u32		Tv_Ofs;
		PDWORD	Tv_PtrVal;
	// code --------------------
		Tv_Ofs	=	((__u32)m_Ch * sizeof(__u32)) + E_GrP3CfgOfsPushScd;
		// check over
		if ( E_GrP3MngSdaSize > Tv_Ofs )
		{
			Tv_PtrVal		=	(PDWORD)V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs );
			*Tv_PtrVal	= A_SchdMsk;
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DoGridPushStp::LcValueSet - value offset is over.\n" );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPushStp::LcPainItems( void )
{
	// local -------------------
		__s32		Tv_PosX;
		__s32		Tv_PosY;
		__u16	Tv_IdxX;
		__u16	Tv_IdxY;
		__u16	Tv_PstCode;
		__u32	Tv_Ofs;
		St_GrRect	Tv_RcItem;
		__u32		Tv_WkVal;
	// code --------------------
		// check item drawable
		if ( (0 != m_ItemCntY) && (0 != m_ParaGridBase.ItemSizeX) && (0 != m_ParaGridBase.ItemSizeY) )
		{
			// check start position
			if (	(m_Rect.right > (m_Rect.left + (__s32)m_ParaGridBase.ItemPosX)) && 
				(m_Rect.bottom > (m_Rect.top + (__s32)m_ParaGridBase.ItemPosY)) )
			{
				// get item pointer
				Tv_Ofs		=	((__u32)m_Ch * sizeof(__u32)) + E_GrP3CfgOfsPushScd;
				if(E_GrP3MngSdaSize >= Tv_Ofs)
				{
					// init
					Tv_WkVal	=	*((PDWORD)V_GrP3Mng->SetupDataPtrGet( (__u16)Tv_Ofs ));
					// draw item
					Tv_PosY	=	m_PntRcScr.top + (__s32)m_ParaGridBase.ItemPosY;
					for (Tv_IdxY=0;Tv_IdxY < m_ItemCntY;Tv_IdxY++)
					{
						Tv_PosX	=	m_PntRcScr.left + (__s32)m_ParaGridBase.ItemPosX;
						for ( Tv_IdxX=0;Tv_IdxX < m_ItemCntX;Tv_IdxX++ )
						{
							// check drawable position
							if ( (m_PntRcScr.right > Tv_PosX) && ( m_PntRcScr.bottom > Tv_PosY ) )
							{
								if(0 != (Tv_WkVal & (1 << Tv_IdxX)))
								{
									Tv_PstCode		= m_ParaGridPushStp.PntVal[1];
								}
								else
								{
									Tv_PstCode		= m_ParaGridPushStp.PntVal[0];
								}

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
						}
						// next y
						Tv_PosY	=	Tv_PosY + (__s32)m_ParaGridBase.ItemSizeY;
					}

				}	// if ( E_GrP3MngSdaSize >= (Tv_Ofs + Tv_ChSize) )

			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoGridPushStp::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
BOOL8	Cls_GrP3DoGridPushStp::ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
BOOL8	Cls_GrP3DoGridPushStp::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__u8	Tv_Val;
		BOOL8	Tv_IsDrag;
		__u16	Tv_StX;
		__u16	Tv_StY;
		__u16	Tv_EdX;
		__u16	Tv_EdY;
		__u16	Tv_WkX;
		__u16	Tv_WkY;
		__u32	Tv_SchdMsk;
	// code --------------------
		Tv_IsDrag		= FALSE;

		if ( m_IsSel )
		{
			// select off
			m_IsSel	=	FALSE;

			if ( LcScrPosToItemPos( A_X, A_Y, &m_SelEdX, &m_SelEdY ) )
			{
				m_CusX	=	m_SelEdX;
				m_CusY	=	m_SelEdY;
				// check equal position
				if ( (m_SelStX == m_SelEdX) && (m_SelStY == m_SelEdY) && m_ParaGridPushStp.IsAfterChg )
				{
					Tv_SchdMsk	= LcSchdGet(m_SelEdX, m_SelEdY);
					if(0 != (Tv_SchdMsk & (1 << m_SelEdX)))
					{
						Tv_SchdMsk = Tv_SchdMsk & ~(1 << m_SelEdX);
					}
					else
					{
						Tv_SchdMsk = Tv_SchdMsk | (1 << m_SelEdX);
					}

					Tv_StX	=	m_SelStX;
					Tv_StY	=	m_SelStY;
					Tv_EdX	=	E_GrP3GridPushStpHourCnt - 1;
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
					Tv_IsDrag		= TRUE;
				}

				// change value
				Tv_WkY	=	Tv_StY;
				while ( Tv_WkY <= Tv_EdY )
				{
					Tv_WkX	=	Tv_StX;
					while ( Tv_WkX <= Tv_EdX )
					{
						if(Tv_IsDrag)
						{
							Tv_SchdMsk	= LcSchdGet(Tv_WkX, Tv_WkY);
							if(0 != (Tv_SchdMsk & (1 << m_SelEdX)))
							{
								Tv_SchdMsk = Tv_SchdMsk & ~(1 << Tv_WkX);
							}
							else
							{
								Tv_SchdMsk = Tv_SchdMsk | (1 << Tv_WkX);
							}
						}

						LcSchdSet( Tv_WkX, Tv_WkY, Tv_SchdMsk );
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
void	Cls_GrP3DoGridPushStp::LcPaintSelect( void )
{
	// local -------------------
		St_GrRect	Tv_RcSel;
		__u16	Tv_StX;
		__u16	Tv_StY;
		__u16	Tv_EdX;
		__u16	Tv_EdY;
	// code --------------------
		// draw select
		
		if ( m_IsSel && (E_GrP3ScptPstIdNone != m_ParaGridPushStp.PntSel) && (IsOnMouse()) )
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

			PaintScriptPush( m_ParaGridPushStp.PntSel, &Tv_RcSel );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPushStp::ProcUiMsg( Ptr_GrP3MsgHd A_PtrMsg )
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
void	Cls_GrP3DoGridPushStp::ProcEvtSpinValChg( Cls_GrP3DoBase* A_Obj )
{
	// local -------------------
		__s32	Tv_Rint[2];
	// code --------------------
		Tv_Rint[0]	=	0;
		A_Obj->RtlCommand( E_GrP3UpiValueGet, Tv_Rint, NULL );
		//if ( (__u8)Tv_Rint[0] < m_ParaGridRscStp.ChCnt )
		{
			m_Ch	=	(__u8)Tv_Rint[0];
			V_GrP3Mng->VmRunCode( m_ParaGridPushStp.AsmChgVal, this );
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPushStp::ProcEvtMseLeave( void )
{
	// local -------------------
	// code --------------------
		//m_IsSel		=	FALSE;
		Cls_GrP3DoGridBase::ProcEvtMseLeave();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPushStp::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		m_IsSel	=	FALSE;
		Cls_GrP3DoGridBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
