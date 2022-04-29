/*
		platform grid play time bar
*/

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P4/GrP4DoGridPlayBar.h>
#include <GrDebug.h>
#include <GrDrawTool.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <P4/GrP4Script.h>
#include <P4/GrP4ConBase.h>
#include <P4/GrP4AsmBase.h>
#include <P4/GrP4Mng.h>
#include <P4/GrP4UiCmd.h>
#include <P4/GrP4Cfg.h>
#include <P4/GrP4ApiBase.h>
#include <P4/GrP4Bios.h>

#ifdef GR_P4_EDITOR
#include <P4/Dvl/GrP4DvlBase.h>
#include <P4/Dvl/GrP4DvlMng.h>
#endif

//====================================================================
//local const
#define E_GrP4DoGridPlayBarItmXCnt			60

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

#define E_GrP4DoGridPlayBarParaCnt		19

St_GrP4DvlDoPrInfoData	V_GrP4DoGridPlayBarParaInfo[E_GrP4DoGridPlayBarParaCnt]	=	
{
	{	L"PntVal0",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->PntVal[0],NULL,			L"Paint item type 0"	},
	{	L"PntVal1",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->PntVal[1],NULL,			L"Paint item type 1"	},
	{	L"PntVal2",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->PntVal[2],NULL,			L"Paint item type 2"	},
	{	L"PntVal3",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->PntVal[3],NULL,			L"Paint item type 3"	},
	{	L"PntVal4",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->PntVal[4],NULL,			L"Paint item type 4"	},
	{	L"PntVal5",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->PntVal[5],NULL,			L"Paint item type 5"	},
	{	L"PntVal6",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->PntVal[6],NULL,			L"Paint item type 6"	},
	{	L"PntVal7",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->PntVal[7],NULL,			L"Paint item type 7"	},

	{	L"ValTbl0",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->ValTbl[0],NULL,			L"item value 0"	},
	{	L"ValTbl1",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->ValTbl[1],NULL,			L"item value 1"	},
	{	L"ValTbl2",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->ValTbl[2],NULL,			L"item value 2"	},
	{	L"ValTbl3",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->ValTbl[3],NULL,			L"item value 3"	},
	{	L"ValTbl4",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->ValTbl[4],NULL,			L"item value 4"	},
	{	L"ValTbl5",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->ValTbl[5],NULL,			L"item value 5"	},
	{	L"ValTbl6",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->ValTbl[6],NULL,			L"item value 6"	},
	{	L"ValTbl7",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->ValTbl[7],NULL,			L"item value 7"	},

	{	L"GvuDev",				TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->GvuDev, NULL,	L"record device select GVU"	},
	{	L"GvuTime",				TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->GvuTime, NULL,	L"record time select GVU"	},
	{	L"AsmTimeSel",		TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoGridPlayBarPara)0)->AsmTimeSel, NULL,	L"assembly code for change time select"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoGridPlayBar::Cls_GrP4DoGridPlayBar(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoGridBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassGridPlayBar;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaGridPlayBar) + sizeof(St_GrP4ScptParaHd);

		GrDumyZeroMem( &m_ParaGridPlayBar, sizeof(m_ParaGridPlayBar) );
		m_ParaGridBase.IsChSelAble	=	TRUE;

		GrDumyZeroMem(&m_BarInfo, sizeof(St_GrP4DoGridPlayBarInfo));

		m_PtrGvu		=	(__s32*)V_GrP4Mng->GvuTblGet();
		//m_BarInfo.ChCnt = V_GrP4Mng->VdoMaxChCntGet();
		m_BarInfo.ChCnt = 16;
		
		m_ItemCntX		=	E_GrP4DoGridPlayBarItmXCnt;
		m_ItemCntY		=	m_BarInfo.ChCnt;

		m_Time			= 0;
		m_GridTime	= 0;
		m_Year			= 0;
		m_Month			= 0;
		m_Day				= 0;
		m_Hour			= 0;
		m_Min				= 0;
}
//--------------------------------------------------------------------
Cls_GrP4DoGridPlayBar::~Cls_GrP4DoGridPlayBar( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoGridPlayBar::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassGridPlayBar;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaGridPlayBar);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaGridPlayBar, sizeof(m_ParaGridPlayBar) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaGridPlayBar)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoGridPlayBar::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoGridBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaGridPlayBar, sizeof(m_ParaGridPlayBar) , m_PtrScpt , E_GrP4DoClassGridPlayBar );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridPlayBar::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		switch ( A_Cmd )
		{
			case E_GrP4UpiChGet:
				//A_PtrInt[0]	=	(__s32)m_Ch;
				break;
			case E_GrP4UpiChSet:
				//m_Ch	=	(__u8)A_PtrInt[1];
				//ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP4DoGridBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoGridPlayBar::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoGridBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaGridPlayBar, V_GrP4DoGridPlayBarParaInfo, E_GrP4DoGridPlayBarParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoGridPlayBar::LcPainItems( void )
{
	// local -------------------
		__s32		Tv_PosX;
		__s32		Tv_PosY;
		__u16	Tv_IdxX;
		__u16	Tv_IdxY;
		__u16	Tv_PstCode;
		St_GrRect	Tv_RcItem;
	// code --------------------
		// check item drawable
		if ( (0 != m_ItemCntX) && (0 != m_ItemCntY) && (0 != m_ParaGridBase.ItemSizeX) && (0 != m_ParaGridBase.ItemSizeY) )
		{
			// check start position
			if ((m_PntRcScr.right > (m_PntRcScr.left + (__s32)m_ParaGridBase.ItemPosX)) &&
				(m_PntRcScr.bottom > (m_PntRcScr.top + (__s32)m_ParaGridBase.ItemPosY)))
			{
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
#if 0
							/*if(0 != m_BarInfo.MinMap[Tv_IdxX])
							{
								Tv_PstCode		= m_ParaGridPlayBar.PntVal[1];
								//don't work - event value : jeff
							}
							else
							{
								Tv_PstCode		= m_ParaGridPlayBar.PntVal[0];
							}*/
#endif
							if (0 != (Tv_IdxX & 1))
							{
								if (0 != (Tv_IdxX & 1))
								{
									Tv_PstCode		= m_ParaGridPlayBar.PntVal[1];
								}
								else
								{
									Tv_PstCode		= m_ParaGridPlayBar.PntVal[3];
								}
							}
							else
							{
								if (0 != (Tv_IdxX & 1))
								{
									Tv_PstCode		= m_ParaGridPlayBar.PntVal[2];
								}
								else
								{
									Tv_PstCode		= m_ParaGridPlayBar.PntVal[4];
								}
							}

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
					}
					// next x
					Tv_PosX	=	Tv_PosX + (__s32)m_ParaGridBase.ItemSizeX;
				}

			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridPlayBar::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__u16	Tv_CusX;
		__u16	Tv_CusY;
	// code --------------------
		if ( LcScrPosToItemPos( A_X, A_Y, &Tv_CusX, &Tv_CusY ) )
		{
			LcProcKeyItemSel((__u32)Tv_CusX);
			ReqDraw();
		}

		return	TRUE;
}
//--------------------------------------------------------------------
/*void	Cls_GrP4DoGridPlayBar::ProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg )
{
		Cls_GrP4DoBase::ProcUiMsg( A_PtrMsg );
}*/
//--------------------------------------------------------------------
void	Cls_GrP4DoGridPlayBar::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		BOOL8	Tv_IsSm;
		BOOL8	Tv_IsRedraw;
		__s32		Tv_Rint[2];
		__u32	Tv_Time;
	// code --------------------
		Tv_IsRedraw		= FALSE;
		GrP4ApiCall(E_GrP4ApiPlayTimeGet, Tv_Rint, NULL, NULL);
		Tv_Time		= Tv_Rint[0];
		Tv_IsSm		= (BOOL8)Tv_Rint[1];

		Tv_IsRedraw = LcTimeUpdate(Tv_Time);

		if(Tv_IsRedraw)
		{
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoGridPlayBar::LcTimeUpdate(__u32 A_Time)
{
	// local -------------------
		__u16		Tv_Year;
		__u8		Tv_Month;
		__u8		Tv_Day;
		__u8		Tv_Hour;
		__u8		Tv_Min;
		__u8		Tv_Sec;
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result		= FALSE;

		if(m_Time != A_Time)
		{
			m_Time = A_Time;

			GrTimeDec(m_Time, &Tv_Year, &Tv_Month, &Tv_Day, &Tv_Hour, &Tv_Min, &Tv_Sec);

			if((m_Year != Tv_Year) || (m_Month != Tv_Month) || (m_Day != Tv_Day) || (m_Hour != Tv_Hour))
			{
				//get time record data
				m_GridTime = GrTimeEnc(Tv_Year, Tv_Month, Tv_Day, Tv_Hour, 0, 0);
				LcGetMinMap(m_GridTime);

				m_Year		= Tv_Year;
				m_Month		= Tv_Month;
				m_Day			= Tv_Day;
				m_Hour		= Tv_Hour;

				Tv_Result		= TRUE;
			}

			if(m_Min != Tv_Min)
			{
				m_Min		= Tv_Min;
				m_CusX	= Tv_Min;

				Tv_Result		= TRUE;
			}
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridPlayBar::LcGetMinMap(__u32 A_Time)
{
	// local -------------------
		__s32			Tv_Rint[5];
		__u8		Tv_RecDev;
		__u8		Tv_MinMap[E_GrTimeMinOfHour][E_GrDvrMaxChCnt];
		__u8*		Tv_TmpMap;
		__u32		Tv_WkCh;
		__u32		Tv_WkMin;
	// code --------------------
		Tv_RecDev		= (__u8)m_PtrGvu[m_ParaGridPlayBar.GvuDev];
		GrDumyZeroMem(m_BarInfo.MinMap, sizeof(m_BarInfo.MinMap));
		GrDumyZeroMem(Tv_MinMap, sizeof(Tv_MinMap));

		Tv_Rint[1]	= Tv_RecDev;
		Tv_Rint[2]	=	(__s32)A_Time;
		Tv_Rint[3]	=	E_GrTimeMinOfHour;
		Tv_Rint[4]	=	(__s32)( (void*)Tv_MinMap );
		GrP4BiosGroupMinMapGet(Tv_Rint, NULL, NULL);

		for(Tv_WkMin = 0;Tv_WkMin < E_GrTimeMinOfHour;Tv_WkMin++)
		{
			Tv_TmpMap = Tv_MinMap[Tv_WkMin];
			for(Tv_WkCh = 0;Tv_WkCh < E_GrDvrMaxChCnt;Tv_WkCh++)
			{
				m_BarInfo.MinMap[Tv_WkMin][Tv_WkCh] = *Tv_TmpMap;
				Tv_TmpMap++;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridPlayBar::LcProcKeyItemSel(__u32 A_SelTime)
{
	// local -------------------
		__u32		Tv_Time;
	// code --------------------
		Tv_Time		= m_GridTime	+ (A_SelTime * E_GrTimeSecOfMin);

		//set play time
		if(E_GrP4MngGvuMaxCnt > m_ParaGridPlayBar.GvuTime)
		{
			m_PtrGvu[m_ParaGridPlayBar.GvuTime] = Tv_Time;
		}

		//play
		if(E_GrP4AsmIdNone != m_ParaGridPlayBar.AsmTimeSel)
		{
			V_GrP4Mng->VmRunCode(m_ParaGridPlayBar.AsmTimeSel, this);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoGridPlayBar::RtlInit( void )
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP4DoGridBase::RtlInit();
}
//--------------------------------------------------------------------