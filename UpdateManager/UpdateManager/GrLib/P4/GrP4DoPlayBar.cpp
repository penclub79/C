/*
		platform play time bar
*/

//====================================================================
// uses
#include <GrLnxApp.h>
#include <stdlib.h>
#include <P4/GrP4DoPlayBar.h>
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
#include <GrStrTool.h>

#ifdef GR_P4_EDITOR
#include <P4/Dvl/GrP4DvlBase.h>
#include <P4/Dvl/GrP4DvlMng.h>
#endif

//====================================================================
//local const
#define E_GrP4DoPlayBarHourTimeLine		(60 * 60)
#define E_GrP4DoPlayBarHourTxtCnt			24
#define E_GrP4DoPlayBarMinTxtCnt			12

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

#define E_GrP4DoPlayBarParaCnt		27

St_GrP4DvlDoPrInfoData	V_GrP4DoPlayBarParaInfo[E_GrP4DoPlayBarParaCnt]	=	
{
	{	L"PntVal0",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[0],NULL,		L"Paint item type 0" },
	{	L"PntVal1",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[1],NULL,		L"Paint item type 1" },
	{	L"PntVal2",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[2],NULL,		L"Paint item type 2" },
	{	L"PntVal3",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[3],NULL,		L"Paint item type 3" },
	{	L"PntVal4",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[4],NULL,		L"Paint item type 4" },
	{	L"PntVal5",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[5],NULL,		L"Paint item type 5" },
	{	L"PntVal6",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[6],NULL,		L"Paint item type 6" },
	{	L"PntVal7",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[7],NULL,		L"Paint item type 7" },
	{	L"PntVal8",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[8],NULL,		L"Paint item type 8" },
	{	L"PntVal9",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[9],NULL,		L"Paint item type 9" },
	{	L"PntVal10",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[10],NULL,		L"Paint item type 10"	},
	{	L"PntVal11",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[11],NULL,		L"Paint item type 11"	},
	{	L"PntVal12",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[12],NULL,		L"Paint item type 12"	},
	{	L"PntVal13",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[13],NULL,		L"Paint item type 13"	},
	{	L"PntVal14",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[14],NULL,		L"Paint item type 14"	},
	{	L"PntVal15",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntRecEvt[15],NULL,		L"Paint item type 15"	},

	{	L"PntLine",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntLine,NULL,					L"Paint Line"	},
	{	L"PntCursor",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntCus,NULL,					L"Paint Cursor"	},
	{	L"PntStr",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntStr,NULL,					L"Paint String"	},

	{	L"PntBgTime",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntBgTime,NULL,				L"Paint background time" },
	{	L"PntBgCh",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntBgCh,NULL,					L"Paint background ch title" },
	{	L"PntBgGrid",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->PntBgGrid,NULL,				L"Paint background grid" },

	{	L"TimeHeight",		TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->TimeHeight,NULL,			L"time item area position x" },
	{	L"ChWidth",				TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->ChWidth,NULL,					L"time item area position y" },

	{	L"ListChCnt",			TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->ListChCnt,NULL,				L"channel view list count" },

	{	L"GvuTime",				TRUE,		E_GrP4DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->GvuTime, NULL,				L"record time select GVU"	},
	{	L"AsmTimeSel",		TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoPlayBarPara)0)->AsmTimeSel, NULL,			L"assembly code for change time select"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoPlayBar::Cls_GrP4DoPlayBar(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
		__u8	Tv_WkCnt;
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassPlayBar;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaPlayBar) + sizeof(St_GrP4ScptParaHd);

		GrDumyZeroMem(&m_ParaPlayBar, sizeof(m_ParaPlayBar));
		GrDumyZeroMem(&m_RecMap, sizeof(St_GrP4DoPlayBarMap));
		for (Tv_WkCnt = 0;Tv_WkCnt < E_GrP4DoPlayBarStrChCnt;Tv_WkCnt++)
		{
			GrDumyZeroMem(&m_StrCh[Tv_WkCnt], sizeof(Def_WstrNum));
		}

		for (Tv_WkCnt = 0;Tv_WkCnt < E_GrP4DoPlayBarStrTimeCnt;Tv_WkCnt++)
		{
			GrDumyZeroMem(&m_StrTime[Tv_WkCnt], sizeof(Def_WstrNum));
		}
		
		GrDumyZeroMem(&m_RecMap, sizeof(St_GrP4DoPlayBarMap));

		m_ParaPlayBar.ListChCnt		= 4;

		m_PtrGvu		=	(__s32*)V_GrP4Mng->GvuTblGet();
		
		m_MaxChCnt	= V_GrP4Mng->VdoMaxChCntGet();
		

		m_PlayTime	= 0;
		m_Year			= 0;
		m_Month			= 0;
		m_Day				= 0;
		m_Hour			= 0;
		m_Min				= 0;

		m_CusX			= 0;
		m_CusY			= 0;
}
//--------------------------------------------------------------------
Cls_GrP4DoPlayBar::~Cls_GrP4DoPlayBar( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoPlayBar::RtlScptParaSubDump(void)
{
	// local -------------------
		Ptr_GrP4ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP4DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP4ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP4ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP4DoClassPlayBar;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaPlayBar);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaPlayBar, sizeof(m_ParaPlayBar) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaPlayBar) + sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaPlayBar, sizeof(m_ParaPlayBar) , m_PtrScpt , E_GrP4DoClassPlayBar );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoPlayBar::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result		= TRUE;

		switch (A_Cmd)
		{
			case E_GrP4UpiModeSet:
				//play map mode set
				LcProcPlayModeSet(A_PtrInt[1]);
				break;
			case E_GrP4UpiValueSet:
				//play time set
				LcProcPlayTimeSet(A_PtrInt[1]);
				break;
			case E_GrP4UpiChSet:
				//playbar ch list set
				LcProcChListSet(A_PtrInt[1]);
				break;
			default:
				Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}

		 //play mode
			 //play time set

		return Tv_Result;
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoPlayBar::RtlDvlParaInfoSubDump(void)
{
		// inherited
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaPlayBar, V_GrP4DoPlayBarParaInfo, E_GrP4DoPlayBarParaCnt );
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::LcPaintItemGridHour(void)
{
	// local -------------------
		__u16		Tv_WkCntX;
		__u16		Tv_WkCntY;
		__u16		Tv_PstCode;
		St_GrRect	Tv_RcItem;

		__u8*		Tv_PtrMap;
		__u16		Tv_WkCh;

		__u16		Tv_ItmXCnt;
		__s32		Tv_ItmWidth;
		__s32		Tv_ItmHeight;
	// code -------------------
		Tv_ItmXCnt		= (__u16)((m_RecMap.GridRect.right - m_RecMap.GridRect.left) >> 1);

		Tv_ItmWidth		= (m_RecMap.GridRect.right - m_RecMap.GridRect.left) / Tv_ItmXCnt;
		Tv_ItmHeight	= (m_RecMap.GridRect.bottom - m_RecMap.GridRect.top) / m_RecMap.GridYCnt;

		if (m_IsReqRecMap)
		{
			LcGetHourMap(Tv_ItmXCnt, m_PlayTime);
		}

		Tv_RcItem.left		= m_RecMap.GridRect.left;
		Tv_RcItem.top			= m_RecMap.GridRect.top;
		Tv_RcItem.right		= Tv_RcItem.left + Tv_ItmWidth;
		Tv_RcItem.bottom	= Tv_RcItem.top + Tv_ItmHeight;

		Tv_WkCh		= m_RecMap.ChIdx;
		
		for (Tv_WkCntY = 0;Tv_WkCntY < m_RecMap.GridYCnt;Tv_WkCntY++)
		{
			Tv_PtrMap	= m_RecMap.PtrMap + (Tv_WkCh * m_RecMap.GridXCnt);

			Tv_RcItem.left		= m_RecMap.GridRect.left;
			Tv_RcItem.right		= Tv_RcItem.left + Tv_ItmWidth;
			for (Tv_WkCntX = 0;Tv_WkCntX < m_RecMap.GridXCnt;Tv_WkCntX++)
			{
				if (0xFF != *Tv_PtrMap)
				{
					Tv_PstCode = m_ParaPlayBar.PntRecEvt[(*Tv_PtrMap & 0x0F)];
				}
				else
				{
					//line
					Tv_PstCode = m_ParaPlayBar.PntLine;
				}

				if (E_GrP4ScptPstIdNone != Tv_PstCode)
				{
					PaintScriptPush( Tv_PstCode, &Tv_RcItem );
				}
				else
				{
					Tv_RcItem.left = Tv_RcItem.left;
				}

				//inc
				Tv_RcItem.left	= Tv_RcItem.left + Tv_ItmWidth;
				Tv_RcItem.right	= Tv_RcItem.left + Tv_ItmWidth;
				Tv_PtrMap++;
			}
			//inc
			Tv_WkCh++;
			Tv_RcItem.top			= Tv_RcItem.top + Tv_ItmHeight;
			Tv_RcItem.bottom	= Tv_RcItem.top + Tv_ItmHeight;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::LcPaintItemGridMin(void)
{
	// local -------------------
		__u16		Tv_WkCntX;
		__u16		Tv_WkCntY;
		__u16		Tv_PstCode;
		St_GrRect	Tv_RcItem;

		__u8*		Tv_PtrMap;
		__u16		Tv_WkCh;

		__u16		Tv_ItmXCnt;
		__s32		Tv_ItmWidth;
		__s32		Tv_ItmHeight;
	// code -------------------
		// calculate contents area rect
		Tv_ItmXCnt		= (__u16)((m_RecMap.GridRect.right - m_RecMap.GridRect.left) >> 1);

		Tv_ItmWidth		= (m_RecMap.GridRect.right - m_RecMap.GridRect.left) / Tv_ItmXCnt;
		Tv_ItmHeight	= (m_RecMap.GridRect.bottom - m_RecMap.GridRect.top) / m_RecMap.GridYCnt;

		if (m_IsReqRecMap)
		{
			LcGetMinMap(Tv_ItmXCnt, m_PlayTime);
			m_IsReqRecMap		= FALSE;
		}

		Tv_RcItem.left		= m_RecMap.GridRect.left;
		Tv_RcItem.top			= m_RecMap.GridRect.top;
		Tv_RcItem.right		= Tv_RcItem.left + Tv_ItmWidth;
		Tv_RcItem.bottom	= Tv_RcItem.top + Tv_ItmHeight;

		Tv_WkCh		= m_RecMap.ChIdx;

		for (Tv_WkCntY = 0;Tv_WkCntY < m_RecMap.GridYCnt;Tv_WkCntY++)
		{
			Tv_PtrMap	= m_RecMap.PtrMap + (Tv_WkCh * m_RecMap.GridXCnt);

			Tv_RcItem.left		= m_RecMap.GridRect.left;
			Tv_RcItem.right		= Tv_RcItem.left + Tv_ItmWidth;
			for (Tv_WkCntX = 0;Tv_WkCntX < m_RecMap.GridXCnt;Tv_WkCntX++)
			{
				if (0xFF != *Tv_PtrMap)
				{
					Tv_PstCode = m_ParaPlayBar.PntRecEvt[(*Tv_PtrMap & 0x0F)];
				}
				else
				{
					//line
					Tv_PstCode = m_ParaPlayBar.PntLine;
				}

				if (E_GrP4ScptPstIdNone != Tv_PstCode)
				{
					PaintScriptPush( Tv_PstCode, &Tv_RcItem );
				}
				else
				{
					Tv_RcItem.left = Tv_RcItem.left;
				}

				//inc
				Tv_RcItem.left	= Tv_RcItem.left + Tv_ItmWidth;
				Tv_RcItem.right	= Tv_RcItem.left + Tv_ItmWidth;
				Tv_PtrMap++;
			}
			//inc
			Tv_WkCh++;
			Tv_RcItem.top			= Tv_RcItem.top + Tv_ItmHeight;
			Tv_RcItem.bottom	= Tv_RcItem.top + Tv_ItmHeight;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::LcPaintItemGrid(void)
{
	// local -------------------
	// code --------------------
		if ((0 != m_ParaPlayBar.TimeHeight) && (0 != m_ParaPlayBar.ChWidth) && (0 != m_MaxChCnt))
		{
			//draw cursor
			LcPaintItemGridCursor();

			//draw grid map
			switch (m_RecMap.MapType)
			{
			case E_GrP4DoPlayBarHourMapType:
				LcPaintItemGridHour();
				break;
			case E_GrP4DoPlayBarMinMapType:
				LcPaintItemGridMin();
				break;
			}

			//background paint
			PaintScriptPush(m_ParaPlayBar.PntBgGrid, &m_RecMap.GridRect);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::LcPaintItemTime(void)
{
	// local -------------------
		__u16		Tv_PstCode;
		St_GrRect	Tv_RcItem;
		__u16		Tv_WkStrCnt;
		__u16		Tv_WkCnt;
		__u32		Tv_Width;
		__u16		Tv_TimeGap;
		__u16		Tv_Time;
	// code -------------------
		//if (m_IsReqRecMap)
		{
			if ((0 != m_ParaPlayBar.TimeHeight) && (0 != m_ParaPlayBar.ChWidth))
			{
				Tv_WkStrCnt = E_GrP4DoPlayBarHourTxtCnt;
				Tv_TimeGap	= 1;
				if (E_GrP4DoPlayBarHourMapType != m_RecMap.MapType)
				{
					Tv_TimeGap = 5;
					Tv_WkStrCnt = E_GrP4DoPlayBarMinTxtCnt;
				}

				//init time rect
				Tv_RcItem.left		= m_PntRcScr.left + m_ParaPlayBar.ChWidth;
				Tv_RcItem.top			= m_PntRcScr.top;
				Tv_RcItem.right		= m_PntRcScr.right;
				Tv_RcItem.bottom	= Tv_RcItem.top + m_ParaPlayBar.TimeHeight;

				Tv_Width	= (Tv_RcItem.right - Tv_RcItem.left) / Tv_WkStrCnt;
				Tv_RcItem.left		= Tv_RcItem.left - (Tv_Width >> 1);
				Tv_RcItem.right		= Tv_RcItem.left + Tv_Width;

				Tv_Time		= 0;
				for (Tv_WkCnt = 0;Tv_WkCnt < Tv_WkStrCnt;Tv_WkCnt++)
				{
					m_StrTimeIdx = (__u8)Tv_WkCnt;
					GrStrIntToWstr(m_StrTime[m_StrTimeIdx], Tv_Time);
					Tv_PstCode = m_ParaPlayBar.PntStr;

					if (E_GrP4ScptPstIdNone != Tv_PstCode)
					{
						m_StrType = E_GrP4DoPlayBarStrTypeTime;
						//paint
						PaintScriptPush(Tv_PstCode, &Tv_RcItem);
						m_StrType = E_GrP4DoPlayBarStrTypeNone;
					}
					//inc
					Tv_RcItem.left	= Tv_RcItem.left + Tv_Width;
					Tv_RcItem.right = Tv_RcItem.left + Tv_Width;
					Tv_Time = Tv_Time + Tv_TimeGap;
				}

				//background draw
				Tv_RcItem.left		= m_PntRcScr.left;
				Tv_RcItem.top			= m_PntRcScr.top;
				Tv_RcItem.right		= m_PntRcScr.right;
				Tv_RcItem.bottom	= Tv_RcItem.top + m_ParaPlayBar.TimeHeight;
				PaintScriptPush(m_ParaPlayBar.PntBgTime, &Tv_RcItem);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::LcPaintItemCh(void)
{
	// local -------------------
		__u16		Tv_PstCode;
		St_GrRect	Tv_RcItem;
		__u16		Tv_WkCnt;
		__u32		Tv_Height;
		__u16		Tv_WkCh;
	// code -------------------
		if ((0 != m_ParaPlayBar.TimeHeight) && (0 != m_ParaPlayBar.ChWidth))
		{
			Tv_Height		= (m_PntRcScr.bottom - (m_PntRcScr.top + m_ParaPlayBar.TimeHeight)) / m_ParaPlayBar.ListChCnt;

			//init ch rect
			Tv_RcItem.left		= m_PntRcScr.left;
			Tv_RcItem.top			= m_PntRcScr.top + m_ParaPlayBar.TimeHeight;
			Tv_RcItem.right		= Tv_RcItem.left + m_ParaPlayBar.ChWidth;
			Tv_RcItem.bottom	= Tv_RcItem.top + Tv_Height;

			Tv_WkCh		= m_RecMap.ChIdx;
			for (Tv_WkCnt = 0;Tv_WkCnt < m_ParaPlayBar.ListChCnt;Tv_WkCnt++)
			{
				m_StrChIdx = (__u8)Tv_WkCnt;
				//text set
				GrStrIntToWstr(m_StrCh[m_StrChIdx], Tv_WkCh + 1);
				GrStrAddWchar(m_StrCh[m_StrChIdx], 0x20);
				GrStrAddWchar(m_StrCh[m_StrChIdx], 0x43);
				GrStrAddWchar(m_StrCh[m_StrChIdx], 0x48);

				Tv_PstCode = m_ParaPlayBar.PntStr;
				if (E_GrP4ScptPstIdNone != Tv_PstCode)
				{
					m_StrType = E_GrP4DoPlayBarStrTypeCh;
					//paint
					PaintScriptPush(Tv_PstCode, &Tv_RcItem);
					m_StrType = E_GrP4DoPlayBarStrTypeNone;
				}
				//inc
				Tv_WkCh++;
				Tv_RcItem.top			= Tv_RcItem.top + Tv_Height;
				Tv_RcItem.bottom	= Tv_RcItem.top + Tv_Height;
			}

			//backgroud draw
			Tv_RcItem.left		= m_PntRcScr.left;
			Tv_RcItem.top			= m_PntRcScr.top + m_ParaPlayBar.TimeHeight;
			Tv_RcItem.right		= Tv_RcItem.left + m_ParaPlayBar.ChWidth;
			Tv_RcItem.bottom	= m_PntRcScr.bottom;
			PaintScriptPush(m_ParaPlayBar.PntBgCh, &Tv_RcItem);
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoPlayBar::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__u16	Tv_CusX;
		__u16	Tv_CusY;
	// code --------------------
		if (LcGridPosToCusPos( A_X, A_Y, &Tv_CusX, &Tv_CusY ))
		{
			LcProcKeyItemSel((__u32)Tv_CusX);
			ReqDraw();
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::ProcEvtTimeTick( __u32 A_Tick )
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
BOOL8		Cls_GrP4DoPlayBar::LcTimeUpdate(__u32 A_Time)
{
	// local -------------------
		__u8		Tv_Hour;
		__u8		Tv_Min;
		__u8		Tv_Sec;
		Def_GrTime	Tv_TimeTmp;
		__u16		Tv_CusX;
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result		= FALSE;

		if(m_PlayTime != A_Time)
		{
			m_PlayTime = A_Time;

			Tv_TimeTmp	= m_PlayTime % E_GrTimeSecOfDay;
			Tv_Hour			= (__u8)(Tv_TimeTmp / E_GrTimeSecOfHour);
			Tv_TimeTmp	=	Tv_TimeTmp % E_GrTimeSecOfHour;
			Tv_Min			=	(__u8)(Tv_TimeTmp / E_GrTimeMinOfHour);
			Tv_Sec			= Tv_TimeTmp % E_GrTimeMinOfHour;

			if (E_GrP4DoPlayBarHourMapType == m_RecMap.MapType)
			{
				if ((m_Min != Tv_Min) || (m_Hour != Tv_Hour))
				{
					m_Hour	= Tv_Hour;
					m_Min		= Tv_Min;
					m_CusX = (((m_Hour * 60) + m_Min) * m_RecMap.GridXCnt) / E_GrTimeMinOfDay;
					Tv_Result		= TRUE;
				}
			}
			if (E_GrP4DoPlayBarMinMapType == m_RecMap.MapType)
			{
				m_Hour	= Tv_Hour;
				m_Min		= Tv_Min;
				Tv_CusX = (((m_Min * 60) + Tv_Sec) * m_RecMap.GridXCnt) / (60 * 60);

				if (Tv_CusX != m_CusX)
				{
					m_CusX = Tv_CusX;
					Tv_Result		= TRUE;
				}
			}
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::LcGetMinMap(__u16 A_ItmCnt, __u32 A_Time)
{
	// local -------------------
		__s32		Tv_Rint[5];
		__u8		Tv_RecDev;
		__u8		Tv_MinMap[E_GrTimeMinOfDay][E_GrDvrMaxChCnt];
		__u8*		Tv_PtrMap;

		__u16		Tv_WkCntX;
		__u16		Tv_WkCntY;
		__u16		Tv_MinIdx;
		__u16		Tv_IsDrawLine;
		__u16		Tv_LineMin;

		__u16		Tv_Year;
		__u8		Tv_Month;
		__u8		Tv_Day;
		__u8		Tv_Hour;
		__u8		Tv_Min;
		__u8		Tv_Sec;
	// code --------------------
		Tv_RecDev		= (__u8)m_PtrGvu[m_ParaPlayBar.GvuDev];

		if (NULL == m_RecMap.PtrMap)
		{
			m_RecMap.PtrMap = (__u8*)malloc(A_ItmCnt * E_GrDvrMaxChCnt);
		}

		if (NULL != m_RecMap.PtrMap)
		{
			GrDumyZeroMem(m_RecMap.PtrMap, (A_ItmCnt * E_GrDvrMaxChCnt));
			GrDumyZeroMem(Tv_MinMap, sizeof(Tv_MinMap));

			//build time
			GrTimeDec(A_Time, &Tv_Year, &Tv_Month, &Tv_Day, &Tv_Hour, &Tv_Min, &Tv_Sec);
			Tv_Min		= 0;
			Tv_Sec		= 0;
			m_PlayTime = GrTimeEnc(Tv_Year, Tv_Month, Tv_Day, Tv_Hour, Tv_Min, Tv_Sec);

			Tv_Rint[1]	= Tv_RecDev;
			Tv_Rint[2]	=	(__s32)m_PlayTime;
			Tv_Rint[3]	=	E_GrTimeMinOfHour;
			Tv_Rint[4]	=	(__s32)( (void*)Tv_MinMap );
			GrP4BiosGroupMinMapGet(Tv_Rint, NULL, NULL);

			Tv_IsDrawLine = FALSE;
			for (Tv_WkCntY = 0;Tv_WkCntY < E_GrDvrMaxChCnt;Tv_WkCntY++)
			{
				Tv_PtrMap		= m_RecMap.PtrMap + (Tv_WkCntY * A_ItmCnt);
				Tv_MinIdx		= 0;
				Tv_LineMin	= 0;
				for (Tv_WkCntX = 1;Tv_WkCntX < A_ItmCnt;Tv_WkCntX++)
				{
					Tv_MinIdx = (E_GrTimeMinOfHour * Tv_WkCntX) / A_ItmCnt;

					if (0 == (Tv_MinIdx % 5))
					{
						if (!Tv_IsDrawLine)
						{
							*Tv_PtrMap	= 0xFF;
							Tv_IsDrawLine = TRUE;
						}
					}
					else
					{
						Tv_IsDrawLine = FALSE;
					}
					*Tv_PtrMap = *Tv_PtrMap | Tv_MinMap[Tv_MinIdx][Tv_WkCntX];

					//inc
					Tv_PtrMap++;
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DoPlayBar::LcGetMinMap - min map alloc failed!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::LcGetHourMap(__u16 A_ItmCnt, __u32 A_Time)
{
	// local -------------------
		__s32		Tv_Rint[5];
		__u8		Tv_RecDev;
		__u8		Tv_MinMap[E_GrTimeMinOfDay][E_GrDvrMaxChCnt];
		__u8*		Tv_PtrMap;
		__u32		Tv_MapSize;

		__u16		Tv_WkCntX;
		__u16		Tv_WkCntY;
		__u16		Tv_WkCnt;
		__u16		Tv_MinIdx;
		__u16		Tv_MinCnt;

		__u16		Tv_Year;
		__u8		Tv_Month;
		__u8		Tv_Day;
		__u8		Tv_Hour;
		__u8		Tv_Min;
		__u8		Tv_Sec;
	// code --------------------
		Tv_RecDev		= (__u8)m_PtrGvu[m_ParaPlayBar.GvuDev];

		Tv_MapSize	= A_ItmCnt * E_GrDvrMaxChCnt;

		if (NULL == m_RecMap.PtrMap)
		{
			m_RecMap.PtrMap = (__u8*)malloc(Tv_MapSize);
		}

		if (NULL != m_RecMap.PtrMap)
		{
			GrDumyZeroMem(m_RecMap.PtrMap, Tv_MapSize);
			GrDumyZeroMem(Tv_MinMap, sizeof(Tv_MinMap));

			//build time
			GrTimeDec(A_Time, &Tv_Year, &Tv_Month, &Tv_Day, &Tv_Hour, &Tv_Min, &Tv_Sec);
			Tv_Hour		= 0;
			Tv_Min		= 0;
			Tv_Sec		= 0;
			m_PlayTime = GrTimeEnc(Tv_Year, Tv_Month, Tv_Day, Tv_Hour, Tv_Min, Tv_Sec);

			Tv_Rint[1]	= Tv_RecDev;
			Tv_Rint[2]	=	(__s32)A_Time;
			Tv_Rint[3]	=	E_GrTimeMinOfDay;
			Tv_Rint[4]	=	(__s32)( (void*)Tv_MinMap );
			GrP4BiosGroupMinMapGet(Tv_Rint, NULL, NULL);

			for (Tv_WkCntY = 0;Tv_WkCntY < E_GrDvrMaxChCnt;Tv_WkCntY++)
			{
				Tv_PtrMap		= m_RecMap.PtrMap + (Tv_WkCntY * A_ItmCnt);
				Tv_MinIdx		= 0;
				for (Tv_WkCntX = 0;Tv_WkCntX < A_ItmCnt;Tv_WkCntX++)
				{
					//draw event value
					Tv_MinCnt		= (E_GrTimeMinOfDay * Tv_WkCntX) / A_ItmCnt;
						
					Tv_MinCnt		= Tv_MinCnt - Tv_MinIdx;
					if (0 != Tv_MinCnt)
					{
						for (Tv_WkCnt = 0;Tv_WkCnt < Tv_MinCnt;Tv_WkCnt++)
						{
							if (0 != (Tv_MinIdx % 60))
							{
								*Tv_PtrMap = *Tv_PtrMap | Tv_MinMap[Tv_MinIdx][Tv_WkCntY];
							}
							else
							{
								//line
								*Tv_PtrMap	= 0xFF;
							}

							//inc
							Tv_MinIdx++;
						}
					}
					else
					{
						//line
						*Tv_PtrMap	= 0xFF;
					}
					
					//inc
					Tv_PtrMap++;
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DoPlayBar::LcGetHourMap - hour map alloc failed!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::LcProcKeyItemSel(__u32 A_SelIdx)
{
	// local -------------------
		__u16		Tv_TimeIdx;
		__u32		Tv_Time;
	// code --------------------
		if ((NULL != m_RecMap.PtrMap) && (0 != m_RecMap.GridXCnt))
		{
			switch (m_RecMap.MapType)
			{
			case E_GrP4DoPlayBarHourMapType:
				Tv_TimeIdx = (E_GrTimeMinOfDay * (__u16)A_SelIdx) / m_RecMap.GridXCnt;
				Tv_Time = m_RecMap.MapTime + (60 * 60 * Tv_TimeIdx);
				break;
			case E_GrP4DoPlayBarMinMapType:
				Tv_TimeIdx = (E_GrTimeMinOfDay * (__u16)A_SelIdx) / m_RecMap.GridXCnt;
				Tv_Time = m_RecMap.MapTime + (60 * Tv_TimeIdx);
				break;
			}

			//set play time
			if(E_GrP4MngGvuMaxCnt > m_ParaPlayBar.GvuTime)
			{
				m_PtrGvu[m_ParaPlayBar.GvuTime] = Tv_Time;
			}

			//play
			if(E_GrP4AsmIdNone != m_ParaPlayBar.AsmTimeSel)
			{
				V_GrP4Mng->VmRunCode(m_ParaPlayBar.AsmTimeSel, this);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::RtlInit( void )
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP4DoBase::RtlInit();

		if (0 == m_ParaPlayBar.ListChCnt)
		{
			m_ParaPlayBar.ListChCnt = 4;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::ProcPaint(void)
{
	// local -------------------
		__s32		Tv_WkPosX;
		__s32		Tv_WkPosY;
	// code --------------------
		Tv_WkPosX		= m_PntRcScr.left + m_ParaPlayBar.ChWidth;
		Tv_WkPosY		= m_PntRcScr.top + m_ParaPlayBar.TimeHeight;
		// calculate contents area rect
		if ((m_RecMap.GridRect.left != Tv_WkPosX) || (m_RecMap.GridRect.top != Tv_WkPosY) ||
			(m_RecMap.GridRect.right != m_PntRcScr.right) || (m_RecMap.GridRect.bottom != m_PntRcScr.bottom))
		{
			m_RecMap.GridRect.left		= Tv_WkPosX;
			m_RecMap.GridRect.top		= Tv_WkPosY;
			m_RecMap.GridRect.right	= m_PntRcScr.right;
			m_RecMap.GridRect.bottom	= m_PntRcScr.bottom;

			m_RecMap.GridXCnt		= (__u16)((m_RecMap.GridRect.right - m_RecMap.GridRect.left) >> 1);
			m_RecMap.GridYCnt		= m_ParaPlayBar.ListChCnt;
			m_RecMap.GridXSize	= (__u16)((m_RecMap.GridRect.right - m_RecMap.GridRect.left) / m_RecMap.GridXCnt);
			m_RecMap.GridYSize	= (__u16)((m_RecMap.GridRect.bottom - m_RecMap.GridRect.top) / m_RecMap.GridYCnt);

			m_RecMap.CusSizeX		= m_RecMap.GridXSize;
			m_RecMap.CusSizeY		= (__u16)(m_RecMap.GridRect.bottom - m_RecMap.GridRect.top);

			m_IsReqRecMap		= TRUE;
		}

		//paint time
		LcPaintItemTime();

		//paint ch title
		LcPaintItemCh();

		//paint grid
		LcPaintItemGrid();
		
		if (m_IsReqRecMap)
		{
			m_IsReqRecMap		= FALSE;
		}
}
//--------------------------------------------------------------------
BOOL8		Cls_GrP4DoPlayBar::LcGridPosToCusPos(__u16 A_X, __u16 A_Y, __u16* A_PtrRtX, __u16* A_PtrRtY)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_PosX;
		__s32		Tv_PosY;
		__s32		Tv_IdxX;
		__s32		Tv_IdxY;
	// code --------------------
		Tv_Result		= FALSE;

		if ((0 != m_RecMap.GridRect.left) && (0 != m_RecMap.GridRect.top) && (0 != m_RecMap.GridRect.right) &&
			(0 != m_RecMap.GridRect.bottom))
		{
			Tv_PosX		= A_X;
			Tv_PosY		= A_Y;

			if (m_RecMap.GridRect.left > Tv_PosX)
			{
				Tv_PosX = m_RecMap.GridRect.left;
			}
			if (m_RecMap.GridRect.top > Tv_PosY)
			{
				Tv_PosY = m_RecMap.GridRect.top;
			}

			//calculate grid position
			Tv_IdxX		= (Tv_PosX - m_RecMap.GridRect.left) / m_RecMap.GridXSize;
			Tv_IdxY		= (Tv_PosY - m_RecMap.GridRect.top) / m_RecMap.GridYSize;

			// check size
			if (m_RecMap.GridXCnt <= Tv_IdxX)
			{
				Tv_IdxX	=	m_RecMap.GridXCnt - 1;
			}
			if (m_RecMap.GridYCnt <= Tv_IdxY )
			{
				Tv_IdxY	=	m_RecMap.GridYCnt - 1;
			}

			Tv_Result = TRUE;
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::LcPaintItemGridCursor()
{
	// local -------------------
		St_GrRect	Tv_RcItem;
		__u16		Tv_PstCode;
	// code -------------------
		if ((0 != m_RecMap.GridRect.left) && (0 != m_RecMap.GridRect.top))
		{
			Tv_RcItem.left		= m_RecMap.GridRect.left + (m_RecMap.CusSizeX * m_CusX);
			Tv_RcItem.top			= m_RecMap.GridRect.top;
			Tv_RcItem.right		= Tv_RcItem.left + m_RecMap.CusSizeX;
			Tv_RcItem.bottom	= Tv_RcItem.top + m_RecMap.CusSizeY;

			Tv_PstCode = m_ParaPlayBar.PntCus;
			if (E_GrP4ScptPstIdNone != Tv_PstCode)
			{
				PaintScriptPush( Tv_PstCode, &Tv_RcItem );
			}
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoPlayBar::RtlInTextGet(__u8 A_Idx, __s32* A_PtrRtCus)
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if (0 == A_Idx)
		{
			switch (m_StrType)
			{
			case E_GrP4DoPlayBarStrTypeCh:
				Tv_Result		= m_StrCh[m_StrChIdx];
				break;
			case E_GrP4DoPlayBarStrTypeTime:
				Tv_Result		= m_StrTime[m_StrTimeIdx];
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::LcProcPlayModeSet(__u32 A_Mode)
{
		if ((A_Mode == E_GrP4DoPlayBarHourMapType) || (A_Mode == E_GrP4DoPlayBarMinMapType))
		{
			if (A_Mode != m_RecMap.MapType)
			{
				m_RecMap.MapType	= (__u8)A_Mode;
				m_IsReqRecMap			= TRUE;
				ReqDraw();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::LcProcPlayTimeSet(__u32 A_Time)
{
		if (m_PlayTime	!= A_Time)
		{
			m_PlayTime	= A_Time;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoPlayBar::LcProcChListSet(__u32 A_ListMode)
{
		switch (A_ListMode)
		{
		case 0:
			m_RecMap.ChIdx = m_RecMap.ChIdx - m_ParaPlayBar.ListChCnt;
			break;
		case 1:
			m_RecMap.ChIdx = m_RecMap.ChIdx + m_ParaPlayBar.ListChCnt;
			break;
		}
		
		if (m_MaxChCnt <= m_RecMap.ChIdx )
		{
			m_RecMap.ChIdx = m_MaxChCnt - m_ParaPlayBar.ListChCnt;
		}

		if (0 > m_RecMap.ChIdx)
		{
			m_RecMap.ChIdx	= 0;
		}

		ReqDraw();
}
//--------------------------------------------------------------------