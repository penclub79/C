/*
		platform calendar base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoCalBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>

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

#define E_GrP4DoCalBaseParaCnt		27

St_GrP4DvlDoPrInfoData	V_GrP4DoCalBaseParaInfo[E_GrP4DoCalBaseParaCnt]	=	
{
	{	L"PntBg Disable",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntBg[E_GrP4DoCalPntIdxDisable],NULL,	L"Paint background disable"	},
	{	L"PntBg Normal",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntBg[E_GrP4DoCalPntIdxNormal],NULL,	L"Paint background normal"	},
	{	L"PntItem Disable",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntItem[E_GrP4DoCalPntIdxDisable],NULL,	L"Paint item disable"	},
	{	L"PntItem Normal",		TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntItem[E_GrP4DoCalPntIdxNormal],NULL,	L"Paint item normal"	},
	{	L"PntItem OnCus",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntItem[E_GrP4DoCalPntIdxOnCus],NULL,	L"Paint item on cursor"	},
	{	L"PntItem Mark",			TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntItem[E_GrP4DoCalPntIdxMark],NULL,	L"Paint item marked"	},
	{	L"PntItem Cus&Mark",	TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntItem[E_GrP4DoCalPntIdxCusAndMark],NULL,	L"Paint item cusor and marked"	},
	{	L"Pnt cursor",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntCus,NULL,	L"Paint cursor"	},
	{	L"Pnt level 0",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntLvl[0],NULL,	L"Paint mark level 0"	},
	{	L"Pnt level 1",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntLvl[1],NULL,	L"Paint mark level 1"	},
	{	L"Pnt level 2",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntLvl[2],NULL,	L"Paint mark level 2"	},
	{	L"Pnt level 3",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntLvl[3],NULL,	L"Paint mark level 3"	},
	{	L"Pnt level 4",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntLvl[4],NULL,	L"Paint mark level 4"	},
	{	L"Pnt level 5",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntLvl[5],NULL,	L"Paint mark level 5"	},
	{	L"Pnt level 6",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntLvl[6],NULL,	L"Paint mark level 6"	},
	{	L"Pnt level 7",				TRUE,		E_GrP4DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntLvl[7],NULL,	L"Paint mark level 7"	},

	{	L"Is align week",			TRUE,		E_GrP4DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->IsAlgWeek,NULL,	L"align by week"	},
	{	L"paint type",				TRUE,		E_GrP4DvlParaTypeByte,					1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->PntType,NULL,	L"paint sequence type"	},
	{	L"item start x",			TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->StartX,NULL,	L"item start position x"	},
	{	L"item start y",			TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->StartY,NULL,	L"item start position y"	},
	{	L"item size x",				TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->SizeX,NULL,	L"item start size x"	},
	{	L"item size y",				TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->SizeY,NULL,	L"item start size y"	},
	{	L"item gap x",				TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->GapX,NULL,	L"item gap size x"	},
	{	L"item gap y",				TRUE,		E_GrP4DvlParaTypeWord,					1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->GapY,NULL,	L"item gap size y"	},

	{	L"Year CtrlId",				TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->DateCtrl[E_GrP4DoCalCtrlIdxYear],NULL,	L"Year control id"	},
	{	L"Month CtrlId",			TRUE,		E_GrP4DvlParaTypeScptIdAny,			1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->DateCtrl[E_GrP4DoCalCtrlIdxMonth],NULL,	L"Month control id"	},

	{	L"AsmEvtMarkChg",			TRUE,		E_GrP4DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP4DoCalBasePara)0)->AsmEvtMarkChg,NULL,	L"event code when mark change"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoCalBase::Cls_GrP4DoCalBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt) :
Cls_GrP4DoBase( A_ScptId, A_PtrRect,A_PosRel, A_ObjPrnt )
{
	// local -------------------
		//__u32	Tv_Mili;
		//BOOL8	Tv_IsSm;
		__s32		Tv_WkIdx;
		//__u32	Tv_Time;
	// code --------------------
		//init
		m_ClassId		=	E_GrP4DoClassCalBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaDoCalBase) + sizeof(St_GrP4ScptParaHd);
		
		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaDoCalBase, sizeof(m_ParaDoCalBase) );
		m_ParaDoCalBase.IsAlgWeek		=	TRUE;
		m_ParaDoCalBase.SizeX				=	18;
		m_ParaDoCalBase.SizeY				=	18;

		GrDumyZeroMem( m_ElemTime, sizeof(m_ElemTime) );

		// update now time
		/*
		Tv_Time	=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
		GrTimeDec( Tv_Time, &m_ElemTime[E_GrP4DoCalTimeIdxYear], (__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxMonth],
			(__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxDay], (__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxHour],
			(__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxMin], (__u8*)&m_ElemTime[E_GrP4DoCalTimeIdxSec] );
			*/

		m_PtrMap		=	NULL;
		m_CusDay		=	1;

		// make date string
		for ( Tv_WkIdx=0;Tv_WkIdx < 31;Tv_WkIdx++ )
		{
			GrStrIntToWstr( m_StrTxt[Tv_WkIdx], Tv_WkIdx + 1 );
		}

}
//--------------------------------------------------------------------
Cls_GrP4DoCalBase::~Cls_GrP4DoCalBase( void )
{
		
}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoCalBase::RtlScptParaSubDump(void)
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
		Tv_PtrHd->ClassId		=	E_GrP4DoClassCalBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaDoCalBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP4ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaDoCalBase, sizeof(m_ParaDoCalBase) );
		//update prograss
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaDoCalBase)	+ sizeof(St_GrP4ScptParaHd);
}
#endif
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaDoCalBase, sizeof(m_ParaDoCalBase) , m_PtrScpt , E_GrP4DoClassCalBase );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoCalBase::RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		/*
		Tv_Result	=	TRUE;
		switch ( A_Cmd )
		{
		case E_GrP4UpiValueGet:
		A_PtrInt[0]	=	(__s32)m_SelTime;
		break;
		default:
		Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		break;
		}
		*/
		Tv_Result	=	Cls_GrP4DoBase::RtlCommand( A_Cmd, A_PtrInt, A_PtrStr );
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP4DoCalBase::LcMarkValueGet( __u8 A_Day )
{
		return	0;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::LcPntText( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		Tv_Stat	=	E_GrP4DoCalPntIdxDisable;
		if ( m_IsPntStatEn )
		{
			Tv_Stat	=	E_GrP4DoCalPntIdxNormal;
			// check on cursor
			if ( m_PntDay == m_CusDay )
			{
				Tv_Stat	=	E_GrP4DoCalPntIdxOnCus;
			}
			// check marked
			if ( 0 != LcMarkValueGet( m_PntDay ) )
			{
				Tv_Stat	=	Tv_Stat + 2;
			}
		}
		PaintScriptPush( m_ParaDoCalBase.PntItem[Tv_Stat], &m_PntRcItem );
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::LcPntMark( void )
{
	// local -------------------
		__u8	Tv_Val;
		__u8	Tv_LvlIdx;
		__u8	Tv_Mask;
	// code --------------------
		Tv_Val	=	LcMarkValueGet( m_PntDay );
		if ( 0 != Tv_Val )
		{
			if ( m_IsBitMode )
			{
				Tv_Mask	=	1;
				for ( Tv_LvlIdx=0;Tv_LvlIdx < m_LvlCnt;Tv_LvlIdx++ )
				{
					if ( 0 != (Tv_Mask & Tv_Val) )
					{
						PaintScriptPush( m_ParaDoCalBase.PntLvl[Tv_LvlIdx], &m_PntRcItem );
					}
				}
			}
			else
			{
				if ( Tv_Val <= m_LvlCnt )
				{
					PaintScriptPush( m_ParaDoCalBase.PntLvl[Tv_Val - 1], &m_PntRcItem );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::LcPntCus( void )
{
	// local -------------------
	// code --------------------
		if ( m_PntDay == m_CusDay )
		{
			PaintScriptPush( m_ParaDoCalBase.PntCus, &m_PntRcItem );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::ProcPaint( void )
{
	// local -------------------
		__u8	Tv_Stat;
		__u8	Tv_WeekIdx;
		__s32		Tv_X;
		__s32		Tv_Y;
	// code --------------------
		// init
		m_PntDay		=	0;
		// background
		Tv_Stat				=	E_GrP4DoCalPntIdxDisable;
		m_IsPntStatEn	=	FALSE;
		if ( IsCanEnable() )
		{
			Tv_Stat				=	E_GrP4DoCalPntIdxNormal;
			m_IsPntStatEn	=	TRUE;
		}
		PaintScriptPush( m_ParaDoCalBase.PntBg[Tv_Stat], &m_PntRcScr );

		// calculate start position
		Tv_X		=	m_PntRcScr.left + (__s32)m_ParaDoCalBase.StartX;
		Tv_Y		=	m_PntRcScr.top + (__s32)m_ParaDoCalBase.StartY;
		Tv_WeekIdx	=	m_StWeek;

		if ( m_ParaDoCalBase.IsAlgWeek )
		{
			Tv_X	=	Tv_X + ( ((__s32)m_ParaDoCalBase.SizeX + (__s32)m_ParaDoCalBase.GapX) * (__s32)m_StWeek );
		}

		// draw item
		for ( m_PntDay=1; m_PntDay <= m_DayCnt;m_PntDay ++ )
		{
			// make rect
			m_PntRcItem.left		=	Tv_X;
			m_PntRcItem.top			=	Tv_Y;
			m_PntRcItem.right		=	Tv_X + (__s32)m_ParaDoCalBase.SizeX;
			m_PntRcItem.bottom	=	Tv_Y + (__s32)m_ParaDoCalBase.SizeY;

			// paint
			switch ( m_ParaDoCalBase.PntType )
			{
				case E_GrP4DoCalPntTypeMTC:
					LcPntMark();
					LcPntText();
					LcPntCus();
					break;
				case E_GrP4DoCalPntTypeMCT:
					LcPntMark();
					LcPntCus();
					LcPntText();
					break;
				case E_GrP4DoCalPntTypeCMT:
					LcPntCus();
					LcPntMark();
					LcPntText();
					break;
				default:
					LcPntText();
					LcPntMark();
					LcPntCus();
					break;
			}

			// next
			Tv_X	=	Tv_X + (__s32)m_ParaDoCalBase.SizeX + (__s32)m_ParaDoCalBase.GapX;
			if ( m_ParaDoCalBase.IsAlgWeek )
			{
				Tv_WeekIdx ++;
				if ( 7 <= Tv_WeekIdx )
				{
					Tv_Y	=	Tv_Y + (__s32)m_ParaDoCalBase.SizeY + (__s32)m_ParaDoCalBase.GapY;
					Tv_X	=	m_PntRcScr.left + (__s32)m_ParaDoCalBase.StartX;
					Tv_WeekIdx	=	0;
				}
			}
		}
		

}
//--------------------------------------------------------------------
#ifdef GR_P4_EDITOR
void	Cls_GrP4DoCalBase::RtlDvlParaInfoSubDump(void)
{
		// inherit
		Cls_GrP4DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaDoCalBase, V_GrP4DoCalBaseParaInfo, E_GrP4DoCalBaseParaCnt );
}
#endif
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DoCalBase::RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result		=	NULL;
		*A_PtrRtCus	=	-1;
		if ( 0 == A_Idx )
		{
			if ( (0 != m_PntDay) && (m_DayCnt >= m_PntDay) )
			{
				Tv_Result	=	m_StrTxt[m_PntDay - 1];
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::LcNotifyMarkChg( void )
{
	// local -------------------
		//St_GrP4MsgObj	Tv_Msg;
	// code --------------------
		// relate event

		// user event
		if ( V_GrP4Mng->IsUiEventAble() )
		{
			// run event assembly
			V_GrP4Mng->VmRunCode( m_ParaDoCalBase.AsmEvtMarkChg, this );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::ProcEvtFocusGet( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::ProcEvtFocusLoss( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoCalBase::ProcEvtFocusDirection( __u8 A_Direction, __u32 A_Atb )
{
	// local -------------------
		__u8	Tv_Pos;
	// code --------------------
		if ( m_ParaDoCalBase.IsAlgWeek )
		{
			// change cursor position
			switch ( A_Direction )
			{
				case E_GrP4DoFocusMvIdxLeft:
					Tv_Pos	=	0;
					if ( 6 != m_StWeek )
					{
						Tv_Pos	=	6 - m_StWeek;	
					}
					m_CusDay	=	Tv_Pos + 1;
					break;
				case E_GrP4DoFocusMvIdxRight:
					Tv_Pos	=	0;
					if ( 0 != m_StWeek )
					{
						Tv_Pos	=	7 - m_StWeek;
					}
					m_CusDay	=	Tv_Pos + 1;
					break;
				case E_GrP4DoFocusMvIdxUp:
					m_CusDay	=	m_DayCnt;
					break;
				case E_GrP4DoFocusMvIdxDown:
					m_CusDay	=	1;
					break;
			}
		}
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoCalBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Pos;
		__u8	Tv_Gap;
	// code --------------------
		Tv_Result	=	TRUE;

		switch ( A_Key )
		{
			case E_GrP4KeyLeft:
				if ( E_GrP4ScptIdNone != m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxLeft] )
				{
					// check left first
					Tv_Pos	=	(m_StWeek + m_CusDay - 1) % 7;
					if ( 0 == Tv_Pos )
					{
						Tv_Result	=	FALSE;
					}
				}
				if ( Tv_Result )
				{
					m_CusDay	=	m_CusDay - 1;
				}
				break;
			case E_GrP4KeyRight:
				if ( E_GrP4ScptIdNone != m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxRight] )
				{
					// check right
					Tv_Pos	=	(m_StWeek + m_CusDay - 1) % 7;
					if ( 6 == Tv_Pos )
					{
						Tv_Result	=	FALSE;
					}
				}
				if ( Tv_Result )
				{
					m_CusDay	=	m_CusDay + 1;
				}
				break;
			case E_GrP4KeyUp:
				if ( E_GrP4ScptIdNone != m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxUp] )
				{
					// check right
					Tv_Pos	=	m_StWeek + m_CusDay - 1;
					if ( 7 > Tv_Pos )
					{
						Tv_Result	=	FALSE;
					}
				}
				if ( Tv_Result )
				{
					if ( 7 >= m_CusDay  )
					{
						m_CusDay	=	m_DayCnt;
					}
					else
					{
						m_CusDay	=	m_CusDay - 7;
					}
				}
				break;
			case E_GrP4KeyDown:
				if ( E_GrP4ScptIdNone != m_ParaDoBase.FcsMvTbl[E_GrP4DoFocusMvIdxDown] )
				{
					// check down
					Tv_Gap	=	(m_StWeek + m_DayCnt) % 7;
					if ( 0 == Tv_Gap )
					{
						Tv_Gap	=	7;
					}
						
					if ( Tv_Gap > (m_DayCnt - m_CusDay) )
					{
						Tv_Result	=	FALSE;
					}
				}
				if ( Tv_Result )
				{
					if ( m_DayCnt < (m_CusDay + 7) )
					{
						m_CusDay	=	1;
					}
					else
					{
						m_CusDay	=	m_CusDay + 7;
					}
				}
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		// change event
		if ( Tv_Result )
		{
			ReqDraw();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoCalBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		St_GrRect	Tv_Rc;
		__s32		Tv_X;
		__s32		Tv_Y;
		__u8	Tv_WeekIdx;
		__u8	Tv_WkDay;
		__u8	Tv_ClkDay;
	// code --------------------
		// check position
		if ( ScrRectGet( &Tv_Rc ) )
		{
			// calculate start position
			Tv_X		=	Tv_Rc.left + (__s32)m_ParaDoCalBase.StartX;
			Tv_Y		=	Tv_Rc.top + (__s32)m_ParaDoCalBase.StartY;
			Tv_WeekIdx	=	m_StWeek;

			if ( m_ParaDoCalBase.IsAlgWeek )
			{
				Tv_X	=	Tv_X + ( ((__s32)m_ParaDoCalBase.SizeX + (__s32)m_ParaDoCalBase.GapX) * (__s32)m_StWeek );
			}

			// find item
			Tv_ClkDay	=	0xFF;
			for ( Tv_WkDay=1; Tv_WkDay <= m_DayCnt;Tv_WkDay ++ )
			{
				// check rect
				if (	(A_X >= Tv_X) && (A_X < (Tv_X + (__s32)m_ParaDoCalBase.SizeX) ) && 
							(A_Y >= Tv_Y) && (A_Y < (Tv_Y + (__s32)m_ParaDoCalBase.SizeY) )
						)
				{
					// found
					Tv_ClkDay	=	Tv_WkDay;
					break;
				}

				// next
				Tv_X	=	Tv_X + (__s32)m_ParaDoCalBase.SizeX + (__s32)m_ParaDoCalBase.GapX;
				if ( m_ParaDoCalBase.IsAlgWeek )
				{
					Tv_WeekIdx ++;
					if ( 7 <= Tv_WeekIdx )
					{
						Tv_Y	=	Tv_Y + (__s32)m_ParaDoCalBase.SizeY + (__s32)m_ParaDoCalBase.GapY;
						Tv_X	=	m_PntRcScr.left + (__s32)m_ParaDoCalBase.StartX;
						Tv_WeekIdx	=	0;
					}
				}
			}

			if ( 0xFF != Tv_ClkDay )
			{
				m_CusDay	=	Tv_ClkDay;
				LcProcTouchDay();
			}

			ReqDraw();
		}
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoBase::RtlInit();
		LcCtrlUpdtByElemTm();			// control rebuild
		LcCalRebuild();
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::ProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_IsRun;
	// code --------------------
		Tv_IsRun	=	FALSE;
		if ( E_GrP4MsgCmdSpinValChg == A_PtrMsg->Cmd )
		{
			Tv_IsRun	=	LcProcEvtSpinValChg( ((Ptr_GrP4MsgObj)A_PtrMsg)->ObjId );
		}
		if ( !Tv_IsRun )
		{
			Cls_GrP4DoBase::ProcUiMsg( A_PtrMsg );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoCalBase::LcProcEvtSpinValChg( __u16 A_ObjId )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
		__s32		Tv_Rint;
		__u8	Tv_TmIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		// check time type
		if (	(A_ObjId == m_ParaDoCalBase.DateCtrl[E_GrP4DoCalCtrlIdxYear]) || 
			(A_ObjId == m_ParaDoCalBase.DateCtrl[E_GrP4DoCalCtrlIdxMonth]) )
		{
			Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( A_ObjId );
			if ( NULL != Tv_Obj )
			{
				Tv_TmIdx	=	E_GrP4DoCalCtrlIdxYear;
				if ( A_ObjId == m_ParaDoCalBase.DateCtrl[E_GrP4DoCalCtrlIdxMonth] )
				{
					Tv_TmIdx ++;
				}
				Tv_Rint	=	0;
				Tv_Obj->RtlCommand( E_GrP4UpiValueGet, &Tv_Rint, NULL );
				if ( 0 != Tv_Rint )
				{
					// update
					m_ElemTime[Tv_TmIdx]	=	(__u16)Tv_Rint;
					LcViewTimeChg();		// call view time change
					LcCalRebuild();
					ReqDraw();
				}
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::LcCalRebuild( void )
{
	// local -------------------
	// code --------------------
		// update day count
		m_DayCnt	=	GrTimeGetMonthMaxDay( m_ElemTime[E_GrP4DoCalTimeIdxYear], (__u8)m_ElemTime[E_GrP4DoCalTimeIdxMonth] );
		// update start week
		m_StWeek	=	GrTimeGetWeekByDate( m_ElemTime[E_GrP4DoCalTimeIdxYear], 
			(__u8)m_ElemTime[E_GrP4DoCalTimeIdxMonth], 1 );

}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::LcProcTouchDay( void )
{
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DoCalBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		if ( E_GrP4KeyEnter == A_Key )
		{
			LcProcTouchDay();
		}
		else
		{
			Tv_Result	=	Cls_GrP4DoBase::ProcEvtKeyUp( A_Dev, A_Key );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::LcElemTmUpdtByCtrl( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		Cls_GrP4DoBase*	Tv_Obj;
		__s32		Tv_Rint[2];
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DoCalCtrlCnt;Tv_WkIdx++ )
		{
			if ( E_GrP4ScptIdNone != m_ParaDoCalBase.DateCtrl[Tv_WkIdx] )
			{
				Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaDoCalBase.DateCtrl[Tv_WkIdx] );
				if ( NULL != Tv_Obj )
				{
					Tv_Rint[0]	=	0;
					Tv_Obj->RtlCommand( E_GrP4UpiValueGet, Tv_Rint, NULL );
					m_ElemTime[Tv_WkIdx]	=	(__u16)Tv_Rint[0];
				}
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::LcCtrlUpdtByElemTm( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		Cls_GrP4DoBase*	Tv_Obj;
		__s32		Tv_Rint[2];
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DoCalCtrlCnt;Tv_WkIdx++ )
		{
			if ( E_GrP4ScptIdNone != m_ParaDoCalBase.DateCtrl[Tv_WkIdx] )
			{
				Tv_Obj	=	m_ObjWin->FindSlaveObjByScptId( m_ParaDoCalBase.DateCtrl[Tv_WkIdx] );
				if ( NULL != Tv_Obj )
				{
					Tv_Rint[1]	=	(__s32)m_ElemTime[Tv_WkIdx];
					Tv_Obj->RtlCommand( E_GrP4UpiValueSet, Tv_Rint, NULL );
				}
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DoCalBase::LcViewTimeChg( void )
{
	// local -------------------
	// code --------------------
		
}
//--------------------------------------------------------------------
