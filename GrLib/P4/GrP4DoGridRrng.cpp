/*
		platform grid record time range select

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P4/GrP4DoGridRrng.h>
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

#define E_GrP4DoGridRrngParaCnt		28

St_GrP4DvlDoPrInfoData	V_GrP4DoGridRrngParaInfo[E_GrP4DoGridRrngParaCnt]	=	
{
	{	L"PntVal0",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[0],NULL,			L"Paint item type 0"	},
	{	L"PntVal1",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[1],NULL,			L"Paint item type 1"	},
	{	L"PntVal2",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[2],NULL,			L"Paint item type 2"	},
	{	L"PntVal3",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[3],NULL,			L"Paint item type 3"	},
	{	L"PntVal4",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[4],NULL,			L"Paint item type 4"	},
	{	L"PntVal5",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[5],NULL,			L"Paint item type 5"	},
	{	L"PntVal6",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[6],NULL,			L"Paint item type 6"	},
	{	L"PntVal7",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[7],NULL,			L"Paint item type 7"	},
	{	L"PntVal8",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[8],NULL,			L"Paint item type 8"	},
	{	L"PntVal9",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[9],NULL,			L"Paint item type 9"	},
	{	L"PntVal10",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[10],NULL,			L"Paint item type 10"	},
	{	L"PntVal11",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[11],NULL,			L"Paint item type 11"	},
	{	L"PntVal12",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[12],NULL,			L"Paint item type 12"	},
	{	L"PntVal13",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[13],NULL,			L"Paint item type 13"	},
	{	L"PntVal14",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[14],NULL,			L"Paint item type 14"	},
	{	L"PntVal15",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntVal[15],NULL,			L"Paint item type 15"	},

	{	L"PntSel",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->PntSel, NULL,	L"paint select area"	},

	{	L"ChCnt",					TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->ChCnt, NULL,	L"channel count"	},
	{	L"IsChMerge",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->IsChMerge, NULL,	L"channel merge"	},
	{	L"IsHourUnit",		TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->IsHourUnit, NULL,	L"hour unit, or minute unit"	},
	{	L"RelCtrlId",			TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->RelCtrlId, NULL,	L"relate control id"	},
	{	L"IsFastReact",		TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->IsFastReact, NULL,	L"fast react relate control"	},

	{	L"GvuDev",				TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->GvuDev, NULL,	L"record device select GVU"	},
	{	L"GvuTimeSt",			TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->GvuTimeSt, NULL,	L"selected start time GVU"	},
	{	L"GvuTimeEd",			TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->GvuTimeEd, NULL,	L"selected end time GVU"	},
	{	L"GvuTimeVw",			TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->GvuTimeVw, NULL,	L"selected view time GVU"	},

	{	L"AsmSelSt",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->AsmSelSt, NULL,	L"assembly code for time start select"	},
	{	L"AsmSelEd",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoGridRrngPara)0)->AsmSelEd, NULL,	L"assembly code for time end select"	},

};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoGridRrng::Cls_GrP4DoGridRrng(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoGridBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassGridRrng;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaGridRrng) + sizeof(St_GrP4ScptParaHd);
		
		GrDumyZeroMem( &m_ParaGridRrng, sizeof(m_ParaGridRrng) );
		m_ParaGridRrng.ChCnt	=	4;

		GrDumyZeroMem( m_MinMap, sizeof(m_MinMap) );
		GrDumyZeroMem( m_ViewData, sizeof(m_ViewData) );
		m_ViewTime			=	0;
		m_SelTime[0]		=	0;
		m_SelTime[1]		=	0;
		m_PntSelIdx[0]	=	-1;
		m_PntSelIdx[1]	=	-1;

		m_SelIdx				=	0;
		m_IsUpdtPntSel	=	FALSE;

		m_RecType			=	E_GrDvrRecDevNoraml;

		m_IsCmdUpdt		=	FALSE;

		m_PtrGvu			=	(__s32*)V_GrP4Mng->GvuTblGet();

		LcXyCntUpdt();

}
//--------------------------------------------------------------------
Cls_GrP4DoGridRrng::~Cls_GrP4DoGridRrng( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoGridRrng::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassGridRrng;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaGridRrng);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaGridRrng, sizeof(m_ParaGridRrng) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaGridRrng)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRrng::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoGridBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaGridRrng, sizeof(m_ParaGridRrng) , m_PtrScpt , E_GrP4DoClassGridRrng );

		// update
		LcXyCntUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridRrng::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
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
					if ( E_GrP4ScptIdNone != m_ParaGridRrng.RelCtrlId )
					{
						Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaGridRrng.RelCtrlId );
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
					DbgMsgPrint( "Cls_GrP4DoGridRrng::RtlCommand - UPI Update overlapped.\n" );
				}
				break;
			case E_GrP4UpiMaxValueGet:
				A_PtrInt[0]	=	(__s32)m_ParaGridRrng.ChCnt;
				break;
			case E_GrP4UpiMaxValueSet:
				m_ParaGridRrng.ChCnt	=	(__u8)A_PtrInt[1];
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
void	Cls_GrP4DoGridRrng::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoGridBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaGridRrng, V_GrP4DoGridRrngParaInfo, E_GrP4DoGridRrngParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRrng::LcPainItems( void )
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
							Tv_PstCode	=	m_ParaGridRrng.PntVal[Tv_Val];
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
BOOL8	Cls_GrP4DoGridRrng::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__u16	Tv_CusX;
	// code --------------------
		if ( LcScrPosToItemPos( A_X, A_Y, &Tv_CusX, &m_CusY ) )
		{
			m_CusX	=	Tv_CusX;
			LcProcKeyItemSel();
			ReqDraw();
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRrng::LcPaintSelect( void )
{
	// local -------------------
		St_GrRect	Tv_RcSel;
		__s32		Tv_PosX;
		Def_GrTime	Tv_TimeSize;
	// code --------------------
		// draw select
		if ( E_GrP4ScptPstIdNone != m_ParaGridRrng.PntSel )
		{
			if ( m_IsUpdtPntSel )
			{
				// init
				m_PntSelIdx[0]	=	-1;
				m_PntSelIdx[1]	=	-1;
				// check start time
				if ( 0 != m_SelTime[0] )
				{
					// get time size
					if ( m_ParaGridRrng.IsHourUnit )
					{
						Tv_TimeSize	=	E_GrTimeSecOfDay;
					}
					else
					{
						Tv_TimeSize	=	E_GrTimeSecOfHour;
					}
					// check one area
					if ( 0 != m_SelTime[1] )
					{
						// both exist
						// check in area
						if ( (m_SelTime[0] <= m_ViewTime) && (m_SelTime[1] >= (m_ViewTime + Tv_TimeSize)) )
						{
							// range over - but view area included
							m_PntSelIdx[0]	=	0;
							m_PntSelIdx[1]	=	m_ItemCntX - 1;
						}
						else if ( (m_SelTime[0] >= m_ViewTime) && (m_SelTime[1] < (m_ViewTime + Tv_TimeSize)) )
						{
							// both calculate
							if ( m_ParaGridRrng.IsHourUnit )
							{
								m_PntSelIdx[0]	=	(m_SelTime[0] - m_ViewTime) / E_GrTimeSecOfHour;
								m_PntSelIdx[1]	=	(m_SelTime[1] - m_ViewTime) / E_GrTimeSecOfHour;
							}
							else
							{
								m_PntSelIdx[0]	=	(m_SelTime[0] - m_ViewTime) / E_GrTimeSecOfMin;
								m_PntSelIdx[1]	=	(m_SelTime[1] - m_ViewTime) / E_GrTimeSecOfMin;
							}
						}
						else if ( (m_SelTime[0] >= m_ViewTime) && (m_SelTime[0] < (m_ViewTime + Tv_TimeSize)) )
						{
							// select 0 calculate only
							if ( m_ParaGridRrng.IsHourUnit )
							{
								m_PntSelIdx[0]	=	(m_SelTime[0] - m_ViewTime) / E_GrTimeSecOfHour;
							}
							else
							{
								m_PntSelIdx[0]	=	(m_SelTime[0] - m_ViewTime) / E_GrTimeSecOfMin;
							}
							m_PntSelIdx[1]	=	m_ItemCntX - 1;
						}
						else if ( (m_SelTime[1] >= m_ViewTime) && (m_SelTime[1] < (m_ViewTime + Tv_TimeSize)) )
						{
							// select 1 calculate only
							m_PntSelIdx[0]	=	0;
							if ( m_ParaGridRrng.IsHourUnit )
							{
								m_PntSelIdx[1]	=	(m_SelTime[1] - m_ViewTime) / E_GrTimeSecOfHour;
							}
							else
							{
								m_PntSelIdx[1]	=	(m_SelTime[1] - m_ViewTime) / E_GrTimeSecOfMin;
							}
						}
					}
					else
					{
						// start exist only
						if ( (m_SelTime[0] >= m_ViewTime) && (m_SelTime[0] < (m_ViewTime + Tv_TimeSize)) )
						{
							if ( m_ParaGridRrng.IsHourUnit )
							{
								m_PntSelIdx[0]	=	(m_SelTime[0] - m_ViewTime) / E_GrTimeSecOfHour;
							}
							else
							{
								m_PntSelIdx[0]	=	(m_SelTime[0] - m_ViewTime) / E_GrTimeSecOfMin;
							}
						}
					}
				}	//	if ( 0 != m_SelTime[0] )
				
				// finish
				m_IsUpdtPntSel	=	FALSE;
			}

			if ( 0 <= m_PntSelIdx[0] )
			{
				Tv_RcSel.top		=	m_PntRcScr.top + (__s32)m_ParaGridBase.ItemPosY;
				Tv_RcSel.bottom	=	Tv_RcSel.top + ((__s32)m_ItemCntY * (__s32)m_ParaGridBase.ItemSizeY);
				Tv_RcSel.left		=	m_PntRcScr.left + (__s32)m_ParaGridBase.ItemPosX;
				Tv_RcSel.left		=	Tv_RcSel.left + ( m_PntSelIdx[0] * (__s32)m_ParaGridBase.ItemSizeX );

				if ( 0 <= m_PntSelIdx[1] )
				{
					// both draw
					Tv_PosX	=	m_PntSelIdx[0];
					while ( Tv_PosX <= m_PntSelIdx[1] )
					{
						Tv_RcSel.right	=	Tv_RcSel.left + (__s32)m_ParaGridBase.ItemSizeX;
						PaintScriptPush( m_ParaGridRrng.PntSel, &Tv_RcSel );
						// next
						Tv_RcSel.left		=	Tv_RcSel.right;
						Tv_PosX ++;
					}
				}
				else
				{
					// start draw only
					Tv_RcSel.right	=	Tv_RcSel.left + (__s32)m_ParaGridBase.ItemSizeX;
					PaintScriptPush( m_ParaGridRrng.PntSel, &Tv_RcSel );
				}
				
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRrng::LcXyCntUpdt( void )
{
	// local -------------------
	// code --------------------
		m_ItemCntX	=	60;
		if ( m_ParaGridRrng.IsHourUnit )
		{
			m_ItemCntX	=	24;
		}

		m_ItemCntY	=	(__u16)m_ParaGridRrng.ChCnt;
		if ( m_ParaGridRrng.IsChMerge )
		{
			m_ItemCntY	=	1;
		}
		if ( E_GrDvrMaxChCnt < m_ItemCntY )
		{
			m_ItemCntY	=	E_GrDvrMaxChCnt;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRrng::LcProcKeyItemSel( void )
{
	// local -------------------
	// code --------------------
		LcProcSelTime();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRrng::LcProcSelTime( void )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
		__s32		Tv_Rint[2];
	// code --------------------
		// update time
		if ( m_ParaGridRrng.IsHourUnit )
		{
			m_SelTime[m_SelIdx]	=	m_ViewTime + ( (__u32)m_CusX * E_GrTimeSecOfHour );
		}
		else
		{
			m_SelTime[m_SelIdx]	=	m_ViewTime + ( (__u32)m_CusX * E_GrTimeSecOfMin );
		}

		// patch end time
		if ( 0 == m_SelIdx )
		{
			m_SelTime[1]	=	0;
		}
		else
		{
			LcSelRngCrct();
		}
		// update start time
		if ( E_GrP4MngGvuMaxCnt > m_ParaGridRrng.GvuTimeSt )
		{
			m_PtrGvu[m_ParaGridRrng.GvuTimeSt]	=	m_SelTime[0];
		}
		if ( E_GrP4MngGvuMaxCnt > m_ParaGridRrng.GvuTimeEd )
		{
			m_PtrGvu[m_ParaGridRrng.GvuTimeEd]	=	m_SelTime[1];
		}
		
		// run assembly
		if ( 0 == m_SelIdx )
		{
			if ( E_GrP4AsmIdNone != m_ParaGridRrng.AsmSelSt )
			{
				V_GrP4Mng->VmRunCode( m_ParaGridRrng.AsmSelSt, this );
			}
		}
		else
		{
			if ( E_GrP4AsmIdNone != m_ParaGridRrng.AsmSelEd )
			{
				V_GrP4Mng->VmRunCode( m_ParaGridRrng.AsmSelEd, this );
			}
		}
		
		// relate
		if ( E_GrP4ScptIdNone != m_ParaGridRrng.RelCtrlId )
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaGridRrng.RelCtrlId );
			if ( NULL != Tv_Obj )
			{
				// run command
				Tv_Obj->RtlCommand( E_GrP4UpiUpdate, Tv_Rint, NULL );
			}
		}

		// next
		m_SelIdx ++;
		if ( 2 <= m_SelIdx )
		{
			m_SelIdx	=	0;
		}
		// redraw select area
		m_IsUpdtPntSel	=	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRrng::LcViewDataBuild( void )
{
	// local -------------------
		__s32	Tv_Rint[5];
		__u32	Tv_ItmIdx;
		__u32	Tv_MergeCnt;
		__u32	Tv_ViewIdx;
		__u32	Tv_WkCh;
	// code --------------------

		if ( 0 != m_ParaGridRrng.ChCnt )
		{
			GrDumyZeroMem( m_ViewData, sizeof(m_ViewData) );

			Tv_Rint[1]	=	(__s32)m_RecType;
			Tv_Rint[2]	=	(__s32)m_ViewTime;
			Tv_Rint[3]	=	E_GrTimeMinOfHour;
			if ( m_ParaGridRrng.IsHourUnit )
			{
				Tv_Rint[3]	=	E_GrTimeMinOfDay;
			}
			Tv_Rint[4]	=	(__s32)( (void*)m_MinMap );
			GrP4BiosGroupMinMapGet(Tv_Rint, NULL, NULL);

			// build view data
			if ( m_ParaGridRrng.IsHourUnit )
			{
				// hour unit
				Tv_MergeCnt	=	0;
				Tv_ViewIdx	=	0;
				for ( Tv_ItmIdx=0;Tv_ItmIdx < E_GrTimeMinOfDay;Tv_ItmIdx++ )
				{
					// merger hour unit
					if ( m_ParaGridRrng.IsChMerge )
					{
						for ( Tv_WkCh=0;Tv_WkCh < m_ParaGridRrng.ChCnt;Tv_WkCh++ )
						{
							m_ViewData[Tv_ViewIdx]	=	m_ViewData[Tv_ViewIdx] | m_MinMap[Tv_ItmIdx][Tv_WkCh];
						}
					}
					else
					{
						for ( Tv_WkCh=0;Tv_WkCh < m_ParaGridRrng.ChCnt;Tv_WkCh++ )
						{
							m_ViewData[Tv_ViewIdx + Tv_WkCh]	=	m_ViewData[Tv_ViewIdx + Tv_WkCh] | m_MinMap[Tv_ItmIdx][Tv_WkCh];
						}
					}

					// next
					Tv_MergeCnt ++;
					if ( 60 <= Tv_MergeCnt )
					{
						Tv_MergeCnt	=	0;
						if ( m_ParaGridRrng.IsChMerge )
						{
							Tv_ViewIdx ++;
						}
						else
						{
							Tv_ViewIdx	=	Tv_ViewIdx + (__u32)m_ParaGridRrng.ChCnt;
						}
					}
				}	//	for ( Tv_ItmIdx=0;Tv_ItmIdx < E_GrTimeMinOfDay;Tv_ItmIdx++ )

			}	//	if ( m_ParaGridRrng.IsHourUnit )
			else
			{
				// minute unit
				Tv_ViewIdx	=	0;
				for ( Tv_ItmIdx=0;Tv_ItmIdx < E_GrTimeMinOfHour;Tv_ItmIdx++ )
				{
					// merger hour unit
					if ( 0 != m_MinMap[Tv_ItmIdx] )
					{
						if ( m_ParaGridRrng.IsChMerge )
						{
							for ( Tv_WkCh=0;Tv_WkCh < m_ParaGridRrng.ChCnt;Tv_WkCh++ )
							{
								m_ViewData[Tv_ViewIdx]	=	m_ViewData[Tv_ViewIdx] | m_MinMap[Tv_ItmIdx][Tv_WkCh];
							}
						}
						else
						{
							for ( Tv_WkCh=0;Tv_WkCh < m_ParaGridRrng.ChCnt;Tv_WkCh++ )
							{
								m_ViewData[Tv_ViewIdx + Tv_WkCh]	=	m_ViewData[Tv_ViewIdx + Tv_WkCh] | m_MinMap[Tv_ItmIdx][Tv_WkCh];
							}
						}
					}	//	if ( 0 != m_ThumbTbl[Tv_ItmIdx].HndlRec )

					// next
					if ( m_ParaGridRrng.IsChMerge )
					{
						Tv_ViewIdx ++;
					}
					else
					{
						Tv_ViewIdx	=	Tv_ViewIdx + (__u32)m_ParaGridRrng.ChCnt;
					}

				}	//	for ( Tv_ItmIdx=0;Tv_ItmIdx < E_GrTimeMinOfHour;Tv_ItmIdx++ )

			}	//	if ( m_ParaGridRrng.IsHourUnit )	else
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRrng::LcValueUpdt( void )
{
	// local -------------------
		__u8	Tv_RecDev;
		//__u32	Tv_Mili;
		//BOOL8	Tv_IsSm;
		Def_GrTime	Tv_Time;
	// code --------------------
		// device value update
		if (E_GrP4MngGvuMaxCnt > m_ParaGridRrng.GvuDev)
		{
			Tv_RecDev	=	(__u8)m_PtrGvu[m_ParaGridRrng.GvuDev];
			if ( E_GrDvrRecDevMirror < Tv_RecDev )
			{
				DbgMsgPrint( "Cls_GrP4DoCalRec::LcValueUpdt - bad device type value ( %d ).\n" , Tv_RecDev );
				Tv_RecDev	=	E_GrDvrRecDevNoraml;
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DoGridRrng::LcValueUpdt - ( %d ) bad device GUV address.\n" , m_ScptId );
		}
		m_RecType	=	Tv_RecDev;
		// start time 
		if ( E_GrP4MngGvuMaxCnt > m_ParaGridRrng.GvuTimeSt )
		{
			m_SelTime[0]	=	(Def_GrTime)m_PtrGvu[m_ParaGridRrng.GvuTimeSt];
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DoGridRrng::LcValueUpdt - ( %d ) bad start time GUV address.\n" , m_ScptId );
		}
		// end time 
		if ( E_GrP4MngGvuMaxCnt > m_ParaGridRrng.GvuTimeSt )
		{
			m_SelTime[1]	=	(Def_GrTime)m_PtrGvu[m_ParaGridRrng.GvuTimeEd];
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DoGridRrng::LcValueUpdt - ( %d ) bad end time GUV address.\n" , m_ScptId );
		}
		// view time
		if ( E_GrP4MngGvuMaxCnt > m_ParaGridRrng.GvuTimeVw )
		{
			m_ViewTime	=	(Def_GrTime)m_PtrGvu[m_ParaGridRrng.GvuTimeVw];
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DoGridRrng::LcValueUpdt - ( %d ) bad view time GUV address.\n" , m_ScptId );
		}
		// correct time
		LcSelRngCrct();
		// update view time
		Tv_Time			=	m_ViewTime;
		if ( m_ParaGridRrng.IsHourUnit )
		{
			m_ViewTime	=	Tv_Time - (Tv_Time % E_GrTimeSecOfDay);
			m_CusX			=	(__u8)((Tv_Time - m_ViewTime) / E_GrTimeSecOfHour);
		}
		else
		{
			m_ViewTime	=	Tv_Time - (Tv_Time % E_GrTimeSecOfHour);
			m_CusX			=	(__u8)((Tv_Time - m_ViewTime) / E_GrTimeSecOfMin);
		}
		/*
		Tv_Time	=	m_SelTime[0];
		if ( 0 == Tv_Time )
		{
			Tv_Time	=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
		}
		if ( m_ParaGridRrng.IsHourUnit )
		{
			m_ViewTime	=	Tv_Time - (Tv_Time % E_GrTimeSecOfDay);
			m_CusX			=	(__u8)((Tv_Time - m_ViewTime) / E_GrTimeSecOfHour);
		}
		else
		{
			m_ViewTime	=	Tv_Time - (Tv_Time % E_GrTimeSecOfHour);
			m_CusX			=	(__u8)((Tv_Time - m_ViewTime) / E_GrTimeSecOfMin);
		}
		*/
		// redraw select area
		m_IsUpdtPntSel	=	TRUE;
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridRrng::RtlInit( void )
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
void	Cls_GrP4DoGridRrng::LcSelRngCrct( void )
{
	// local -------------------
		Def_GrTime	Tv_TimeTmp;
	// code --------------------
		if ( (0 != m_SelTime[0]) && (0 != m_SelTime[1]) )
		{
			if ( m_SelTime[0] > m_SelTime[1] )
			{
				Tv_TimeTmp		=	m_SelTime[0];
				m_SelTime[0]	=	m_SelTime[1];
				m_SelTime[1]	=	Tv_TimeTmp;
			}
		}
}
//--------------------------------------------------------------------

