/*
		platform grid motion setup

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P4/GrP4DoGridMtnStp.h>
#include <GrDebug.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <P4/GrP4Script.h>
#include <P4/GrP4ConBase.h>
#include <P4/GrP4AsmBase.h>
#include <P4/GrP4Mng.h>
#include <P4/GrP4UiCmd.h>
#include <P4/GrP4Bios.h>
#include <P4/GrP4Cfg.h>
#include <P4/GrP4ApiBase.h>

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

#define E_GrP4DoGridMtnStpParaCnt		10

St_GrP4DvlDoPrInfoData	V_GrP4DoGridMtnStpParaInfo[E_GrP4DoGridMtnStpParaCnt]	=	
{
	{	L"PntSensNo",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridMtnStpPara)0)->PntSens[0],NULL,		L"Paint Non Sensed"	},
	{	L"PntSensOk",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridMtnStpPara)0)->PntSens[1],NULL,		L"Paint Sensed"	},

	{	L"PntMark",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridMtnStpPara)0)->PntMark,NULL,		L"Paint mark block"	},
	{	L"PntSel",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridMtnStpPara)0)->PntSel,NULL,		L"Paint selected area"	},

	{	L"IsSenseDraw",		TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoGridMtnStpPara)0)->IsSenseDraw, NULL,	L"Sensed area view"	},
	{	L"SwViewDelay",		TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoGridMtnStpPara)0)->SwViewDelay, NULL,	L"software draw delay"	},

	{	L"TxtPrefix",			TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoGridMtnStpPara)0)->TxtPrefix, NULL,	L"sensitivity text prefix"	},
	{	L"TxtSuffix",			TRUE,		E_GrP4DvlParaTypeTxtId,					1,	(void*)&((Ptr_GrP4DoGridMtnStpPara)0)->TxtSuffix, NULL,	L"sensitivity text suffix"	},

	{	L"DispValAdd",		TRUE,		E_GrP4DvlParaTypeInt,						1,	(void*)&((Ptr_GrP4DoGridMtnStpPara)0)->DispValAdd, NULL,	L"display sensitive value adder"	},
	{	L"StpAddress",		TRUE,		E_GrP4DvlParaTypeAsmStpAdr,			1,	(void*)&((Ptr_GrP4DoGridMtnStpPara)0)->StpAddr, NULL,	L"setup address"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoGridMtnStp::Cls_GrP4DoGridMtnStp(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoGridBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
		__s32	Tv_Rint[4];
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassGridMtnStp;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaGridMtnStp) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaGridMtnStp, sizeof(m_ParaGridMtnStp) );
		m_ParaGridBase.IsChSelAble	=	TRUE;

		// get dimension
		GrP4ApiCall(E_GrP4ApiMtnDmsInfo, Tv_Rint, NULL, NULL);
		m_IsSoftView	=	(BOOL8)Tv_Rint[0];
		m_ItemCntX		=	(__u16)Tv_Rint[1];
		m_ItemCntY		=	(__u16)Tv_Rint[2];

		m_Ch				=	0;
		m_IsSel			=	FALSE;
		m_SwViewDelay	=	0;

		m_Value			=	0xFF;
		m_Prefix		=	E_GrP4ScptTxtIdNone;
		m_Suffix		=	E_GrP4ScptTxtIdNone;
		m_PtrSens		= NULL;
		GrDumyZeroMem( m_ChkMtn, sizeof(m_ChkMtn) );

		// change quad
		((Cls_GrP4ConBase*)m_ObjCon)->QuadModeSet( E_GrDvrQuadMode1, m_Ch );
		// update channel environment
		LcChEnvUpdt();
}
//--------------------------------------------------------------------
Cls_GrP4DoGridMtnStp::~Cls_GrP4DoGridMtnStp( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoGridMtnStp::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoGridBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassGridMtnStp;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaGridMtnStp);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaGridMtnStp, sizeof(m_ParaGridMtnStp) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaGridMtnStp)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoGridMtnStp::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoGridBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaGridMtnStp, sizeof(m_ParaGridMtnStp) , m_PtrScpt , E_GrP4DoClassGridMtnStp );

		// update
		LcChEnvUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridMtnStp::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiChGet:
				A_PtrInt[0]	=	(__s32)m_Ch;
				break;
			case E_GrP4UpiChSet:
				m_Ch			=	(__u8)A_PtrInt[1];
				// change quad
				((Cls_GrP4ConBase*)m_ObjCon)->QuadModeSet( E_GrDvrQuadMode1, m_Ch );
				// update channel environment
				LcChEnvUpdt();
				ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoGridBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoGridMtnStp::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoGridBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaGridMtnStp, V_GrP4DoGridMtnStpParaInfo, E_GrP4DoGridMtnStpParaCnt );
}
#endif
//--------------------------------------------------------------------
__u8	Cls_GrP4DoGridMtnStp::LcMapGet( __u16 A_X, __u16 A_Y )
{
	// local -------------------
		__u8	Tv_Result;
		__u32	Tv_Mask;
	// code --------------------
		Tv_Result	=	0;
		Tv_Mask		=	1 << A_X;
		if ( 0 != (Tv_Mask & m_PtrStp[A_Y]) )
		{
			Tv_Result ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridMtnStp::LcProcKeyItemSel( void )
{
	// local -------------------
		BOOL8	Tv_IsVal;
	// code --------------------
		// get target value
		Tv_IsVal	=	(BOOL8)LcMapGet( m_CusX, m_CusY );
		Tv_IsVal	=	!Tv_IsVal;
		LcMapSet( m_CusX, m_CusY, (__u8)Tv_IsVal );

		LcCfgMapUpdt();

		// draw
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridMtnStp::LcMapSet( __u16 A_X, __u16 A_Y, __u8 A_Val )
{
	// local -------------------
		__u32	Tv_Mask;
	// code --------------------
		Tv_Mask	=	1 << A_X;
		if ( 0 != A_Val )
		{
			m_PtrStp[A_Y]	=	m_PtrStp[A_Y] | Tv_Mask;
		}
		else
		{
			m_PtrStp[A_Y]	=	m_PtrStp[A_Y] & (~Tv_Mask);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridMtnStp::LcPainItems( void )
{
	// local -------------------
		__s32		Tv_PosX;
		__s32		Tv_PosY;
		__u16	Tv_IdxX;
		__u16	Tv_IdxY;
		__u16	Tv_PstCode;
		St_GrRect	Tv_RcItem;
		__u32	Tv_Mask;
		__u8	Tv_Val;
	// code --------------------
		// check item drawable
		if ( (0 != m_ItemCntX) && (0 != m_ItemCntY) && (0 != m_ParaGridBase.ItemSizeX) && (0 != m_ParaGridBase.ItemSizeY) )
		{
			// check start position
			if ((m_PntRcScr.right > (m_PntRcScr.left + (__s32)m_ParaGridBase.ItemPosX)) &&
				(m_PntRcScr.bottom > (m_PntRcScr.top + (__s32)m_ParaGridBase.ItemPosY)))
			{
				// draw item
				Tv_PosY	=	m_PntRcScr.top + (__s32)m_ParaGridBase.ItemPosY;
				for (Tv_IdxY=0;Tv_IdxY < m_ItemCntY;Tv_IdxY++)
				{
					// init
					Tv_Mask	=	1;
					Tv_PosX	=	m_PntRcScr.left + (__s32)m_ParaGridBase.ItemPosX;
					for ( Tv_IdxX=0;Tv_IdxX < m_ItemCntX;Tv_IdxX++ )
					{
						// check drawable position
						if ( (m_PntRcScr.right > Tv_PosX) && ( m_PntRcScr.bottom > Tv_PosY ) )
						{
							// check drawing map
							if ( 0 != (m_PtrStp[Tv_IdxY] & Tv_Mask) )
							{

								// draw mark
								Tv_RcItem.left		=	Tv_PosX;
								Tv_RcItem.top			=	Tv_PosY;
								Tv_RcItem.right		=	Tv_PosX + (__s32)m_ParaGridBase.ItemSizeX;
								Tv_RcItem.bottom	=	Tv_PosY + (__s32)m_ParaGridBase.ItemSizeY;
								if ( E_GrP4ScptPstIdNone != m_ParaGridMtnStp.PntMark )
								{
									PaintScriptPush( m_ParaGridMtnStp.PntMark, &Tv_RcItem );
								}

								if(E_GrP4CfgOfsMtnMap == m_ParaGridMtnStp.StpAddr)
								{
									// check software sensed draw
									if ( m_IsSoftView && m_ParaGridMtnStp.IsSenseDraw )
									{
										if(NULL != m_PtrSens)
										{
											// check sensed
											Tv_Val	=	0;
											if ( 0 != (Tv_Mask & m_PtrSens[Tv_IdxY]) )
											{
												Tv_Val ++;
											}
											// draw sensed
											Tv_PstCode	=	m_ParaGridMtnStp.PntSens[Tv_Val];
											if ( E_GrP4ScptPstIdNone != Tv_PstCode )
											{
												PaintScriptPush( Tv_PstCode, &Tv_RcItem );
											}
										}
									}
								}
							}
							
						}
						// next x
						Tv_PosX	=	Tv_PosX + (__s32)m_ParaGridBase.ItemSizeX;
						Tv_Mask	=	Tv_Mask << 1;
					}
					// next y
					Tv_PosY	=	Tv_PosY + (__s32)m_ParaGridBase.ItemSizeY;
				}
			}

		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridMtnStp::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
	// code --------------------
		if ( LcScrPosToItemPos( A_X, A_Y, &m_SelStX, &m_SelStY ) )
		{
			m_SelEdX	=	m_SelStX;
			m_SelEdY	=	m_SelStY;
			m_CusX		=	m_SelEdX;
			m_CusY		=	m_SelEdY;
			m_IsSel		=	TRUE;
			ReqDraw();
		}
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridMtnStp::ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__u16	Tv_WkX;
		__u16	Tv_WkY;
	// code --------------------
		if ( m_IsSel )
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
		
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridMtnStp::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_IsVal;
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

				// change value
				Tv_WkY	=	Tv_StY;
				while ( Tv_WkY <= Tv_EdY )
				{
					Tv_WkX	=	Tv_StX;
					while ( Tv_WkX <= Tv_EdX )
					{
						// toggle value
						Tv_IsVal	=	(BOOL8)LcMapGet( Tv_WkX, Tv_WkY );
						Tv_IsVal	=	!Tv_IsVal;
						LcMapSet( Tv_WkX, Tv_WkY, (__u8)Tv_IsVal );
						// next x
						Tv_WkX ++;
					}
					// next y
					Tv_WkY ++;
				}
				// map update
				LcCfgMapUpdt();
			}
			// draw
			ReqDraw();
		}
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridMtnStp::LcPaintSelect( void )
{
	// local -------------------
		St_GrRect	Tv_RcSel;
		__u16	Tv_StX;
		__u16	Tv_StY;
		__u16	Tv_EdX;
		__u16	Tv_EdY;
	// code --------------------
		// draw select
		if ( m_IsSel && (E_GrP4ScptPstIdNone != m_ParaGridMtnStp.PntSel) )
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

			PaintScriptPush( m_ParaGridMtnStp.PntSel, &Tv_RcSel );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridMtnStp::ProcEvtMseLeave( void )
{
	// local -------------------
	// code --------------------
		m_IsSel		=	FALSE;
		Cls_GrP4DoGridBase::ProcEvtMseLeave();
}
//--------------------------------------------------------------------
__u8	Cls_GrP4DoGridMtnStp::LcSenseGet( void )
{
	// local -------------------
		__s32		Tv_Rint[3];
	// code --------------------
		Tv_Rint[1]	=	(__s32)m_Ch;
		GrP4ApiCall(E_GrP4ApiMtnSenGet, Tv_Rint, NULL, NULL);
		return	(__u8)Tv_Rint[0];
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridMtnStp::LcSenseSet( __u8 A_Sense )
{
	// local -------------------
		__s32		Tv_Rint[3];
	// code --------------------
		Tv_Rint[1]	=	(__s32)m_Ch;
		Tv_Rint[2]	=	(__s32)A_Sense;
		GrP4ApiCall(E_GrP4ApiMtnSenSet, Tv_Rint, NULL, NULL);
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridMtnStp::LcChEnvUpdt( void )
{
	// local -------------------
		__s32		Tv_Rint[2];
	// code --------------------
		// update setup map pointer
		m_PtrStp	=	(__u32*)( (__u32)V_GrP4Mng->SetupDataPtrGet( m_ParaGridMtnStp.StpAddr ) + ( (__u32)m_Ch << 7 ) );
		// sense map update
		Tv_Rint[1]	=	(__s32)m_Ch;
		if(E_GrP4CfgOfsMtnMap == m_ParaGridMtnStp.StpAddr)
		{
			GrP4ApiCall(E_GrP4ApiMtnViewMapGet, Tv_Rint, NULL, NULL);
			m_PtrSens	=	(__u32*)Tv_Rint[0];
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridMtnStp::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		BOOL8	Tv_IsChg;
		__u8	Tv_WkIdx;
		__s32		Tv_Rint[2];
	// code --------------------
		if(E_GrP4CfgOfsMtnMap == m_ParaGridMtnStp.StpAddr)
		{
			// sense draw
			if ( m_IsSoftView && m_ParaGridMtnStp.IsSenseDraw )
			{
				m_SwViewDelay ++;

				if ( m_SwViewDelay > m_ParaGridMtnStp.SwViewDelay )
				{
					// map update
					Tv_Rint[1]	=	(__s32)m_Ch;
					GrP4ApiCall(E_GrP4ApiMtnViewMapUpdt, Tv_Rint, NULL, NULL);
					// check value change
					Tv_IsChg	=	FALSE;

					for ( Tv_WkIdx=0;Tv_WkIdx < m_ItemCntY;Tv_WkIdx++ )
					{
						if(NULL != m_PtrSens)
						{
							if ( m_ChkMtn[Tv_WkIdx] != m_PtrSens[Tv_WkIdx] )
							{
								Tv_IsChg	=	TRUE;
							}
							m_ChkMtn[Tv_WkIdx]	=	m_PtrSens[Tv_WkIdx];
						}
					}

					if ( Tv_IsChg )
					{
						ReqDraw();
					}

					m_SwViewDelay	=	0;	// reset
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridMtnStp::ProcEvtWheelUp( void )
{
	// local -------------------
		__s32		Tv_Rint[3];
	// code --------------------
		if(E_GrP4CfgOfsMtnMap == m_ParaGridMtnStp.StpAddr)
		{
			// update sensitivity value
			Tv_Rint[1]	=	(__s32)m_Ch;
			GrP4ApiCall(E_GrP4ApiMtnSenGet, Tv_Rint, NULL, NULL);
			Tv_Rint[2]	=	Tv_Rint[0] + 1;
			if ( E_GrP4CfgMtnValMax < Tv_Rint[2] )
			{
				Tv_Rint[2]	=	E_GrP4CfgMtnValMax;
			}
			Tv_Rint[1]	=	(__s32)m_Ch;
			GrP4ApiCall(E_GrP4ApiMtnSenSet, Tv_Rint, NULL, NULL);
			ReqDraw();
		}

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridMtnStp::ProcEvtWheelDown( void )
{
	// local -------------------
		__s32		Tv_Rint[3];
	// code --------------------
		if(E_GrP4CfgOfsMtnMap == m_ParaGridMtnStp.StpAddr)
		{
			// update sensitivity value
			Tv_Rint[1]	=	(__s32)m_Ch;
			GrP4ApiCall(E_GrP4ApiMtnSenGet, Tv_Rint, NULL, NULL);
			Tv_Rint[2]	=	Tv_Rint[0] - 1;
			if ( 0 > Tv_Rint[2] )
			{
				Tv_Rint[2]	=	0;
			}
			Tv_Rint[1]	=	(__s32)m_Ch;
			GrP4ApiCall(E_GrP4ApiMtnSenSet, Tv_Rint, NULL, NULL);
			ReqDraw();
		}

		return	TRUE;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoGridMtnStp::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__s32		Tv_Rint[2];
		WCHAR*	Tv_StrWk;
		WCHAR*	Tv_StrScpt;
	// code --------------------
		Tv_Result	=	NULL;
		if ( 0 == A_Idx )
		{
			// check change
			Tv_Rint[1]	=	(__s32)m_Ch;
			GrP4ApiCall(E_GrP4ApiMtnSenGet, Tv_Rint, NULL, NULL);
			if ( ( (__u8)Tv_Rint[0] != m_Value ) || (m_ParaGridMtnStp.TxtPrefix != m_Prefix) || (m_ParaGridMtnStp.TxtSuffix != m_Suffix) )
			{
				// update text
				m_Value		=	(__u8)Tv_Rint[0];
				m_Prefix	=	m_ParaGridMtnStp.TxtPrefix;
				m_Suffix	=	m_ParaGridMtnStp.TxtSuffix;

				// add prefix
				GrStrClear( m_StrMtn );
				Tv_StrWk	=	m_StrMtn;
				if ( E_GrP4ScptTxtIdNone != m_Prefix )
				{
					Tv_StrScpt	=	V_GrP4Mng->TextPtrGet( m_Prefix );
					Tv_StrWk	=	GrStrWcopy( Tv_StrWk, Tv_StrScpt );
				}
				// set value
				GrStrIntToWstr( Tv_StrWk, (__s32)m_Value + m_ParaGridMtnStp.DispValAdd );
				// add suffix
				if ( E_GrP4ScptTxtIdNone != m_Suffix )
				{
					Tv_StrScpt	=	V_GrP4Mng->TextPtrGet( m_Suffix );
					Tv_StrWk	=	GrStrWcat( Tv_StrWk, Tv_StrScpt );
				}
			}

			// sensitivity text
			Tv_Result	=	m_StrMtn;
			
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridMtnStp::LcCfgMapUpdt( void )
{
	// local -------------------
		__s32		Tv_Rtin[2];
	// code --------------------
		Tv_Rtin[1]	=	(__s32)m_Ch;
		if(E_GrP4CfgOfsMtnMap == m_ParaGridMtnStp.StpAddr)
		{
			GrP4ApiCall(E_GrP4ApiCfgMtnMapUpdt, Tv_Rtin, NULL, NULL);
		}
		else
		{
			GrP4ApiCall(E_GrP4ApiCfgPrivacyMapUpdt, Tv_Rtin, NULL, NULL);
		}
}
//--------------------------------------------------------------------
