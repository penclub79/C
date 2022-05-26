/*
		platform grid record schedule setup

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P4/GrP4DoGridRstat.h>
#include <GrDebug.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <P4/GrP4Script.h>
#include <P4/GrP4ConBase.h>
#include <P4/GrP4AsmBase.h>
#include <P4/GrP4Mng.h>
#include <P4/GrP4UiCmd.h>
#include <P4/GrP4ApiBase.h>
#include <P4/GrP4Bios.h>

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

#define E_GrP4DoGridRstatParaCnt		28

St_GrP4DvlDoPrInfoData	V_GrP4DoGridRstatParaInfo[E_GrP4DoGridRstatParaCnt]	=	
{
	{	L"PntVal0",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[0],NULL,			L"Paint item type 0"	},
	{	L"PntVal1",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[1],NULL,			L"Paint item type 1"	},
	{	L"PntVal2",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[2],NULL,			L"Paint item type 2"	},
	{	L"PntVal3",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[3],NULL,			L"Paint item type 3"	},
	{	L"PntVal4",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[4],NULL,			L"Paint item type 4"	},
	{	L"PntVal5",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[5],NULL,			L"Paint item type 5"	},
	{	L"PntVal6",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[6],NULL,			L"Paint item type 6"	},
	{	L"PntVal7",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[7],NULL,			L"Paint item type 7"	},
	{	L"PntVal8",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[8],NULL,			L"Paint item type 8"	},
	{	L"PntVal9",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[9],NULL,			L"Paint item type 9"	},
	{	L"PntVal10",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[10],NULL,			L"Paint item type 10"	},
	{	L"PntVal11",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[11],NULL,			L"Paint item type 11"	},
	{	L"PntVal12",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[12],NULL,			L"Paint item type 12"	},
	{	L"PntVal13",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[13],NULL,			L"Paint item type 13"	},
	{	L"PntVal14",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[14],NULL,			L"Paint item type 14"	},
	{	L"PntVal15",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntVal[15],NULL,			L"Paint item type 15"	},

	{	L"PntSel",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->PntSel, NULL,	L"paint select area"	},

	{	L"ChCnt",					TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->ChCnt, NULL,	L"channel count"	},
	{	L"IsChMerge",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->IsChMerge, NULL,	L"channel merge"	},
	{	L"IsHourUnit",		TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->IsHourUnit, NULL,	L"hour unit, or minute unit"	},
	{	L"RelUpdtId",			TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->RelUpdtId, NULL,	L"relate update action control id"	},
	{	L"IsFastReact",		TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->IsFastReact, NULL,	L"fast react relate control"	},
	{	L"IsRelFcs",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->IsRelFcs, NULL,	L"move focus to relate control"	},
	{	L"IsSelPlay",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->IsSelPlay, NULL,	L"play at select time"	},
	{	L"IsCusSel",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->IsCusSel, NULL,	L"select on cursor"	},

	{	L"GvuDev",				TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->GvuDev, NULL,	L"record device select GVU"	},
	{	L"GvuTime",				TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->GvuTime, NULL,	L"record time select GVU"	},

	{	L"AsmTimeSel",		TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoGridRstatPara)0)->AsmTimeSel, NULL,	L"assembly code for change time select"	},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoGridRstat::Cls_GrP4DoGridRstat(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoGridBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassGridRstat;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaGridRstat) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaGridRstat, sizeof(m_ParaGridRstat) );
		m_ParaGridRstat.ChCnt	=	4;

		GrDumyZeroMem( m_MinMap, sizeof(m_MinMap) );
		GrDumyZeroMem( m_ViewData, sizeof(m_ViewData) );
		m_SelTime			=	0;
		m_ViewTime		=	0;
		m_SelIdx			=	0xFF;

		m_RecType			=	E_GrDvrRecDevNoraml;

		m_IsCmdUpdt		=	FALSE;

		m_PtrGvu			=	(__s32*)V_GrP4Mng->GvuTblGet();

		LcXyCntUpdt();

}
//--------------------------------------------------------------------
Cls_GrP4DoGridRstat::~Cls_GrP4DoGridRstat( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoGridRstat::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassGridRstat;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaGridRstat);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaGridRstat, sizeof(m_ParaGridRstat) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaGridRstat)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRstat::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoGridBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaGridRstat, sizeof(m_ParaGridRstat) , m_PtrScpt , E_GrP4DoClassGridRstat );

		// update
		LcXyCntUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridRstat::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
		__s32		Tv_Rint[2];
	// code --------------------
		Tv_Result	=	FALSE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiUpdate:
				if ( !m_IsCmdUpdt )
				{
					m_IsCmdUpdt	=	TRUE;
					LcValueUpdt();
					LcViewDataBuild();
					ReqDraw();
					// relate
					if ( E_GrP4ScptIdNone != m_ParaGridRstat.RelUpdtId )
					{
						Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaGridRstat.RelUpdtId );
						if ( NULL != Tv_Obj )
						{
							// run command
							Tv_Obj->RtlCommand( E_GrP4UpiUpdate, Tv_Rint, NULL );
						}
					}
					m_IsCmdUpdt	=	FALSE;
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DoGridRstat::RtlCommand - UPI Update overlapped.\n" );
				}
				break;
			case E_GrP4UpiMaxValueGet:
				A_PtrInt[0]	=	(__s32)m_ParaGridRstat.ChCnt;
				break;
			case E_GrP4UpiMaxValueSet:
				m_ParaGridRstat.ChCnt	=	(__u8)A_PtrInt[1];
				LcXyCntUpdt();
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
void	Cls_GrP4DoGridRstat::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoGridBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaGridRstat, V_GrP4DoGridRstatParaInfo, E_GrP4DoGridRstatParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRstat::LcPainItems( void )
{
	// local -------------------
		__s32		Tv_PosX;
		__s32		Tv_PosY;
		__u16	Tv_IdxX;
		__u16	Tv_IdxY;
		__u16	Tv_PstCode;
		St_GrRect	Tv_RcItem;
		__u32	Tv_ItemIdx;
		__u8	Tv_Val;
	// code --------------------
		// check item drawable
		if ( (0 != m_ItemCntX) && (0 != m_ItemCntY) && (0 != m_ParaGridBase.ItemSizeX) && (0 != m_ParaGridBase.ItemSizeY) )
		{
			// check start position
			if ((m_PntRcScr.right > (m_PntRcScr.left + (__s32)m_ParaGridBase.ItemPosX)) &&
				(m_PntRcScr.bottom > (m_PntRcScr.top + (__s32)m_ParaGridBase.ItemPosY)))
			{
				// init
				Tv_ItemIdx	=	0;
				// draw item
				Tv_PosX	=	m_PntRcScr.left + (__s32)m_ParaGridBase.ItemPosX;
				for ( Tv_IdxX=0;Tv_IdxX < m_ItemCntX;Tv_IdxX++ )
				{
					Tv_PosY	=	m_PntRcScr.top + (__s32)m_ParaGridBase.ItemPosY;
					for (Tv_IdxY=0;Tv_IdxY < m_ItemCntY;Tv_IdxY++)
					{
						// check drawable position
						if ( (m_PntRcScr.right > Tv_PosX) && ( m_PntRcScr.bottom > Tv_PosY ) )
						{
							// get value
							Tv_Val	=	m_ViewData[Tv_ItemIdx] & 0x0F;
							Tv_PstCode	=	m_ParaGridRstat.PntVal[Tv_Val];
							if ( E_GrP4ScptPstIdNone != Tv_PstCode )
							{
								Tv_RcItem.left		=	Tv_PosX;
								Tv_RcItem.top			=	Tv_PosY;
								Tv_RcItem.right		=	Tv_PosX + (__s32)m_ParaGridBase.ItemSizeX;
								Tv_RcItem.bottom	=	Tv_PosY + (__s32)m_ParaGridBase.ItemSizeY;
								PaintScriptPush( Tv_PstCode, &Tv_RcItem );
							}
						}
						// next y
						Tv_PosY	=	Tv_PosY + (__s32)m_ParaGridBase.ItemSizeY;
						Tv_ItemIdx ++;
					}
					// next x
					Tv_PosX	=	Tv_PosX + (__s32)m_ParaGridBase.ItemSizeX;
				}

			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridRstat::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__u16	Tv_CusX;
	// code --------------------
		if ( LcScrPosToItemPos( A_X, A_Y, &Tv_CusX, &m_CusY ) )
		{
			if ( m_ParaGridRstat.IsCusSel )
			{
				if ( Tv_CusX == m_CusX )
				{
					m_CusX	=	Tv_CusX;
					LcProcKeyItemSel();
				}
				m_CusX	=	Tv_CusX;
			}
			else
			{
				m_CusX	=	Tv_CusX;
				LcProcKeyItemSel();
			}
			ReqDraw();
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRstat::LcPaintSelect( void )
{
	// local -------------------
		St_GrRect	Tv_RcSel;
	// code --------------------
		// draw select
		if ( (0xFF != m_SelIdx) && (E_GrP4ScptPstIdNone != m_ParaGridRstat.PntSel) )
		{
			Tv_RcSel.top		=	m_PntRcScr.top + (__s32)m_ParaGridBase.ItemPosY;
			Tv_RcSel.bottom	=	Tv_RcSel.top + ((__s32)m_ItemCntY * (__s32)m_ParaGridBase.ItemSizeY);

			Tv_RcSel.left		=	m_PntRcScr.left + (__s32)m_ParaGridBase.ItemPosX;
			Tv_RcSel.left		=	Tv_RcSel.left + ( (__s32)m_SelIdx * (__s32)m_ParaGridBase.ItemSizeX );
			Tv_RcSel.right	=	Tv_RcSel.left + (__s32)m_ParaGridBase.ItemSizeX;

			PaintScriptPush( m_ParaGridRstat.PntSel, &Tv_RcSel );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRstat::LcXyCntUpdt( void )
{
	// local -------------------
	// code --------------------
		m_ItemCntX	=	60;
		if ( m_ParaGridRstat.IsHourUnit )
		{
			m_ItemCntX	=	24;
		}

		m_ItemCntY	=	(__u16)m_ParaGridRstat.ChCnt;
		if ( m_ParaGridRstat.IsChMerge )
		{
			m_ItemCntY	=	1;
		}
		if ( E_GrDvrMaxChCnt < m_ItemCntY )
		{
			m_ItemCntY	=	E_GrDvrMaxChCnt;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRstat::LcProcKeyItemSel( void )
{
	// local -------------------
	// code --------------------
		LcProcSelTime();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRstat::LcProcSelTime( void )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
		__s32		Tv_Rint[2];
	// code --------------------
		m_SelIdx	=	(__u8)m_CusX;
		if ( m_ParaGridRstat.IsHourUnit )
		{
			m_SelTime	=	m_ViewTime + ( (__u32)m_SelIdx * E_GrTimeSecOfHour );
		}
		else
		{
			m_SelTime	=	m_ViewTime + ( (__u32)m_SelIdx * E_GrTimeSecOfMin );
		}
		// update value
		if ( E_GrP4MngGvuMaxCnt > m_ParaGridRstat.GvuTime )
		{
			m_PtrGvu[m_ParaGridRstat.GvuTime]	=	m_SelTime;
		}

		// run assembly
		if ( E_GrP4AsmIdNone != m_ParaGridRstat.AsmTimeSel )
		{
			V_GrP4Mng->VmRunCode( m_ParaGridRstat.AsmTimeSel, this );
		}

		// relate
		if ( E_GrP4ScptIdNone != m_ParaGridRstat.RelUpdtId )
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaGridRstat.RelUpdtId );
			if ( NULL != Tv_Obj )
			{
				// run command
				Tv_Obj->RtlCommand( E_GrP4UpiUpdate, Tv_Rint, NULL );
				// auto change focus
				if ( m_ParaGridRstat.IsRelFcs )
				{
					((Cls_GrP4ConBase*)m_ObjCon)->FocusObjSet( Tv_Obj );
				}
			}
		}

		// select play
		if ( m_ParaGridRstat.IsSelPlay )
		{
			// play group set
			Tv_Rint[1]	=	(__s32)m_RecType;
			GrP4ApiCall(E_GrP4ApiPlayGroupSel, Tv_Rint, NULL, this);
			// play mode set
			Tv_Rint[1]	=	TRUE;
			GrP4ApiCall(E_GrP4ApiPlayModeSet, Tv_Rint, NULL, this);
			// play speed set
			Tv_Rint[1]	=	0;
			GrP4ApiCall(E_GrP4ApiPlaySpeedSet, Tv_Rint, NULL, this);
			// play direction set
			Tv_Rint[1]	=	FALSE;
			GrP4ApiCall(E_GrP4ApiPlayDirSet, Tv_Rint, NULL, this);
			// go time
			Tv_Rint[1]	=	(__s32)m_SelTime;
			GrP4ApiCall(E_GrP4ApiPlayGoTime, Tv_Rint, NULL, this);
			// do play
			GrP4ApiCall(E_GrP4ApiPlayDo, Tv_Rint, NULL, this);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRstat::LcViewDataBuild( void )
{
	// local -------------------
		__s32	Tv_Rint[5];
		__u32	Tv_ItmIdx;
		__u32	Tv_MergeCnt;
		__u32	Tv_ViewIdx;
		__u32	Tv_WkCh;
	// code --------------------

		if ( 0 != m_ParaGridRstat.ChCnt )
		{
			GrDumyZeroMem( m_ViewData, sizeof(m_ViewData) );
			GrDumyZeroMem( m_MinMap, sizeof(m_MinMap) );

			Tv_Rint[1]	=	(__s32)m_RecType;
			Tv_Rint[2]	=	(__s32)m_ViewTime;
			Tv_Rint[3]	=	E_GrTimeMinOfHour;
			if ( m_ParaGridRstat.IsHourUnit )
			{
				Tv_Rint[3]	=	E_GrTimeMinOfDay;
			}
			Tv_Rint[4]	=	(__s32)( (void*)m_MinMap );
			GrP4BiosGroupMinMapGet( Tv_Rint, NULL,this );

			// build view data
			if ( m_ParaGridRstat.IsHourUnit )
			{
				// hour unit
				Tv_MergeCnt	=	0;
				Tv_ViewIdx	=	0;
				for ( Tv_ItmIdx=0;Tv_ItmIdx < E_GrTimeMinOfDay;Tv_ItmIdx++ )
				{
					// merger hour unit
					if ( 0 != m_MinMap[Tv_ItmIdx] )
					{
						if ( m_ParaGridRstat.IsChMerge )
						{
							for ( Tv_WkCh=0;Tv_WkCh < m_ParaGridRstat.ChCnt;Tv_WkCh++ )
							{
								m_ViewData[Tv_ViewIdx]	=	m_ViewData[Tv_ViewIdx] | m_MinMap[Tv_ItmIdx][Tv_WkCh];
							}
						}
						else
						{
							for ( Tv_WkCh=0;Tv_WkCh < m_ParaGridRstat.ChCnt;Tv_WkCh++ )
							{
								m_ViewData[Tv_ViewIdx + Tv_WkCh]	=	m_ViewData[Tv_ViewIdx + Tv_WkCh] | m_MinMap[Tv_ItmIdx][Tv_WkCh];
							}
						}
					}	//	if ( 0 != m_ThumbTbl[Tv_ItmIdx].HndlRec )

					// next
					Tv_MergeCnt ++;
					if ( 60 <= Tv_MergeCnt )
					{
						Tv_MergeCnt	=	0;
						if ( m_ParaGridRstat.IsChMerge )
						{
							Tv_ViewIdx ++;
						}
						else
						{
							Tv_ViewIdx	=	Tv_ViewIdx + (__u32)m_ParaGridRstat.ChCnt;
						}
					}
				}	//	for ( Tv_ItmIdx=0;Tv_ItmIdx < E_GrTimeMinOfDay;Tv_ItmIdx++ )

			}	//	if ( m_ParaGridRstat.IsHourUnit )
			else
			{
				// minute unit
				Tv_ViewIdx	=	0;
				for ( Tv_ItmIdx=0;Tv_ItmIdx < E_GrTimeMinOfHour;Tv_ItmIdx++ )
				{
					// merger hour unit
					if ( 0 != m_MinMap[Tv_ItmIdx] )
					{
						if ( m_ParaGridRstat.IsChMerge )
						{
							for ( Tv_WkCh=0;Tv_WkCh < m_ParaGridRstat.ChCnt;Tv_WkCh++ )
							{
								m_ViewData[Tv_ViewIdx]	=	m_ViewData[Tv_ViewIdx] | m_MinMap[Tv_ItmIdx][Tv_WkCh];
							}
						}
						else
						{
							for ( Tv_WkCh=0;Tv_WkCh < m_ParaGridRstat.ChCnt;Tv_WkCh++ )
							{
								m_ViewData[Tv_ViewIdx + Tv_WkCh]	=	m_ViewData[Tv_ViewIdx + Tv_WkCh] | m_MinMap[Tv_ItmIdx][Tv_WkCh];
							}
						}
					}	//	if ( 0 != m_ThumbTbl[Tv_ItmIdx].HndlRec )

					// next
					if ( m_ParaGridRstat.IsChMerge )
					{
						Tv_ViewIdx ++;
					}
					else
					{
						Tv_ViewIdx	=	Tv_ViewIdx + (__u32)m_ParaGridRstat.ChCnt;
					}

				}	//	for ( Tv_ItmIdx=0;Tv_ItmIdx < E_GrTimeMinOfHour;Tv_ItmIdx++ )

			}	//	if ( m_ParaGridRstat.IsHourUnit )	else
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRstat::LcValueUpdt( void )
{
	// local -------------------
		__u8	Tv_RecDev;
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
	// code --------------------
		if ( (E_GrP4MngGvuMaxCnt > m_ParaGridRstat.GvuDev) && (E_GrP4MngGvuMaxCnt > m_ParaGridRstat.GvuTime) )
		{
			Tv_RecDev	=	(__u8)m_PtrGvu[m_ParaGridRstat.GvuDev];
			if ( E_GrDvrRecDevMirror < Tv_RecDev )
			{
				DbgMsgPrint( "Cls_GrP4DoCalRec::LcValueUpdt - bad device type value ( %d ).\n" , Tv_RecDev );
				Tv_RecDev	=	E_GrDvrRecDevNoraml;
			}
			m_RecType		=	Tv_RecDev;
			m_SelTime		=	(Def_GrTime)m_PtrGvu[m_ParaGridRstat.GvuTime];
			if ( 0 == m_SelTime )
			{
				m_SelTime	=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
			}
			if ( m_ParaGridRstat.IsHourUnit )
			{
				m_ViewTime	=	m_SelTime - (m_SelTime % E_GrTimeSecOfDay);
				m_SelIdx		=	(__u8)((m_SelTime - m_ViewTime) / E_GrTimeSecOfHour);
			}
			else
			{
				m_ViewTime	=	m_SelTime - (m_SelTime % E_GrTimeSecOfHour);
				m_SelIdx		=	(__u8)((m_SelTime - m_ViewTime) / E_GrTimeSecOfMin);
			}
			m_CusX	=	m_SelIdx;
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DoGridRstat::LcValueUpdt - ( %d ) bad GUV address.\n" , m_ScptId );
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRstat::RtlInit( void )
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP4DoGridBase::RtlInit();
		// value update
		LcValueUpdt();
		LcViewDataBuild();
}
//--------------------------------------------------------------------

