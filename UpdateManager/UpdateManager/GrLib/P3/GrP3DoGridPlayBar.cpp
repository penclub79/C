/*
		platform grid play time bar
*/

//====================================================================
// uses
#include <GrLnxApp.h>
#include <P3/GrP3DoGridPlayBar.h>
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
#include <P3/GrP3Bios.h>

#ifdef GR_P3_EDITOR
#include <P3/Dvl/GrP3DvlBase.h>
#include <P3/Dvl/GrP3DvlMng.h>
#endif

//====================================================================
//local const
#define E_GrP3DoGridPlayBarItmXCnt			60
#define E_GrP3DoGridPlayBarItmYCnt			1

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

#define E_GrP3DoGridPlayBarParaCnt		19

St_GrP3DvlDoPrInfoData	V_GrP3DoGridPlayBarParaInfo[E_GrP3DoGridPlayBarParaCnt]	=	
{
	{	L"PntVal0",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->PntVal[0],NULL,			L"Paint item type 0"	},
	{	L"PntVal1",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->PntVal[1],NULL,			L"Paint item type 1"	},
	{	L"PntVal2",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->PntVal[2],NULL,			L"Paint item type 2"	},
	{	L"PntVal3",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->PntVal[3],NULL,			L"Paint item type 3"	},
	{	L"PntVal4",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->PntVal[4],NULL,			L"Paint item type 4"	},
	{	L"PntVal5",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->PntVal[5],NULL,			L"Paint item type 5"	},
	{	L"PntVal6",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->PntVal[6],NULL,			L"Paint item type 6"	},
	{	L"PntVal7",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->PntVal[7],NULL,			L"Paint item type 7"	},

	{	L"ValTbl0",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->ValTbl[0],NULL,			L"item value 0"	},
	{	L"ValTbl1",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->ValTbl[1],NULL,			L"item value 1"	},
	{	L"ValTbl2",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->ValTbl[2],NULL,			L"item value 2"	},
	{	L"ValTbl3",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->ValTbl[3],NULL,			L"item value 3"	},
	{	L"ValTbl4",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->ValTbl[4],NULL,			L"item value 4"	},
	{	L"ValTbl5",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->ValTbl[5],NULL,			L"item value 5"	},
	{	L"ValTbl6",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->ValTbl[6],NULL,			L"item value 6"	},
	{	L"ValTbl7",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->ValTbl[7],NULL,			L"item value 7"	},

	{	L"GvuDev",				TRUE,		E_GrP3DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->GvuDev, NULL,	L"record device select GVU"	},
	{	L"GvuTime",				TRUE,		E_GrP3DvlParaTypeAsmGvuAdr,			1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->GvuTime, NULL,	L"record time select GVU"	},
	{	L"AsmTimeSel",		TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoGridPlayBarPara)0)->AsmTimeSel, NULL,	L"assembly code for change time select"	},
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoGridPlayBar::Cls_GrP3DoGridPlayBar( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoGridBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassGridPlayBar;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaGridPlayBar) + sizeof(St_GrP3ScptParaHd);
		
		GrDumyZeroMem( &m_ParaGridPlayBar, sizeof(m_ParaGridPlayBar) );
		m_ParaGridBase.IsChSelAble	=	TRUE;

		m_ItemCntX		=	E_GrP3DoGridPlayBarItmXCnt;
		m_ItemCntY		=	E_GrP3DoGridPlayBarItmYCnt;

		m_PtrGvu		=	(__s32*)V_GrP3Mng->GvuTblGet();

		GrDumyZeroMem(&m_MinMap, E_GrTimeMinOfHour);
		m_Time			= 0;
		m_GridTime	= 0;
		m_Year			= 0;
		m_Month			= 0;
		m_Day				= 0;
		m_Hour			= 0;
		m_Min				= 0;
}
//--------------------------------------------------------------------
Cls_GrP3DoGridPlayBar::~Cls_GrP3DoGridPlayBar( void )
{
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPlayBar::RtlScptParaSubDump( void )
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
		Tv_PtrHd->ClassId		=	E_GrP3DoClassGridPlayBar;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaGridPlayBar);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaGridPlayBar, sizeof(m_ParaGridPlayBar) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaGridPlayBar)	+ sizeof(St_GrP3ScptParaHd);
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPlayBar::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoGridBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaGridPlayBar, sizeof(m_ParaGridPlayBar) , m_PtrScpt , E_GrP3DoClassGridPlayBar );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoGridPlayBar::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		switch ( A_Cmd )
		{
			case E_GrP3UpiChGet:
				//A_PtrInt[0]	=	(__s32)m_Ch;
				break;
			case E_GrP3UpiChSet:
				//m_Ch	=	(__u8)A_PtrInt[1];
				//ReqDraw();
				break;
			default:
				Tv_Result	=	Cls_GrP3DoGridBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPlayBar::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoGridBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaGridPlayBar, V_GrP3DoGridPlayBarParaInfo, E_GrP3DoGridPlayBarParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPlayBar::LcPainItems( void )
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
		if ( (0 != m_ItemCntY) && (0 != m_ParaGridBase.ItemSizeX) && (0 != m_ParaGridBase.ItemSizeY) )
		{
			// check start position
			if (	(m_Rect.right > (m_Rect.left + (__s32)m_ParaGridBase.ItemPosX)) && 
				(m_Rect.bottom > (m_Rect.top + (__s32)m_ParaGridBase.ItemPosY)) )
			{
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
							if(0 != m_MinMap[Tv_IdxX])
							{
								Tv_PstCode		= m_ParaGridPlayBar.PntVal[1];
							}
							else
							{
								Tv_PstCode		= m_ParaGridPlayBar.PntVal[0];
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
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoGridPlayBar::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__u16	Tv_CusX;
	// code --------------------
		if ( LcScrPosToItemPos( A_X, A_Y, &Tv_CusX, &m_CusY ) )
		{
			LcProcKeyItemSel((__u32)Tv_CusX);
			ReqDraw();
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPlayBar::ProcUiMsg( Ptr_GrP3MsgHd A_PtrMsg )
{
		Cls_GrP3DoBase::ProcUiMsg( A_PtrMsg );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPlayBar::ProcEvtTimeTick( __u32 A_Tick )
{
	// local -------------------
		BOOL8	Tv_IsSm;
		BOOL8	Tv_IsRedraw;
		__s32		Tv_Rint[2];
		__u32	Tv_Time;
	// code --------------------
		Tv_IsRedraw		= FALSE;
		GrP3BiosPlayTimeGet( Tv_Rint, NULL );
		Tv_Time		= Tv_Rint[0];
		Tv_IsSm		= (BOOL8)Tv_Rint[1];

		Tv_IsRedraw = LcTimeUpdate(Tv_Time);

		if(Tv_IsRedraw)
		{
			ReqDraw();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoGridPlayBar::LcTimeUpdate(__u32 A_Time)
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
				m_Min = Tv_Min;
				m_CusX		= Tv_Min;

				Tv_Result		= TRUE;
			}
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPlayBar::LcGetMinMap(__u32 A_Time)
{
	// local -------------------
		__s32			Tv_Rint[5];
		__u8		Tv_RecDev;
		__u8		Tv_MinMap[E_GrTimeMinOfHour][E_GrDvrMaxChCnt];
		__u32		Tv_WkCnt;
		__u32		Tv_WkMin;
	// code --------------------
		Tv_RecDev		= (__u8)m_PtrGvu[m_ParaGridPlayBar.GvuDev];
		GrDumyZeroMem(m_MinMap, sizeof(m_MinMap));
		GrDumyZeroMem(Tv_MinMap, sizeof(Tv_MinMap));

		Tv_Rint[1]	= Tv_RecDev;
		Tv_Rint[2]	=	(__s32)A_Time;
		Tv_Rint[3]	=	E_GrTimeMinOfHour;
		Tv_Rint[4]	=	(__s32)( (void*)Tv_MinMap );
		GrP3BiosGroupMinMapGet(Tv_Rint, NULL);

		for(Tv_WkMin = 0;Tv_WkMin < E_GrTimeMinOfHour;Tv_WkMin++)
		{
			for(Tv_WkCnt = 0;Tv_WkCnt < E_GrDvrMaxChCnt;Tv_WkCnt++)
			{
				m_MinMap[Tv_WkMin] = m_MinMap[Tv_WkMin] | Tv_MinMap[Tv_WkMin][Tv_WkCnt];
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoGridPlayBar::LcProcKeyItemSel(__u32 A_SelTime)
{
	// local -------------------
		__u32		Tv_Time;
	// code --------------------
		Tv_Time		= m_GridTime	+ (A_SelTime * E_GrTimeSecOfMin);

		//set play time
		if(E_GrP3MngGvuMaxCnt > m_ParaGridPlayBar.GvuTime)
		{
			m_PtrGvu[m_ParaGridPlayBar.GvuTime] = Tv_Time;
		}

		//play
		if(E_GrP3AsmIdNone != m_ParaGridPlayBar.AsmTimeSel)
		{
			V_GrP3Mng->VmRunCode(m_ParaGridPlayBar.AsmTimeSel, this);
		}
}
//--------------------------------------------------------------------
