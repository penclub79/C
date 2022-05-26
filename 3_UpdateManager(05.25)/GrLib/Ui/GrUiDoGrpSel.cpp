/*
 channel graph

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoGrpSel.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>
#include	<Ui/GrUiDoLbl.h>
#include	<GrStrTool.h>

//====================================================================
//local const
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*				V_GrUiMngObj;

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoGrpSel::Cls_GrUiDoGrpSel( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
	// local -------------------
		__u32	Tv_WkIdx;
#ifdef GR_OSD_EDIT
		__u8	Tv_Tmp;
		__u8*	Tv_PtrVal;
#endif
	// code --------------------
		//init
		m_TypeId			=	E_GrUiDoTypeIdGrpSel;
		m_IsFocusAble	=	TRUE;

		m_ColCnt			=	E_GrUiDoGrpSelDfltColCnt;
		m_LineCnt			=	E_GrUiDoGrpSelDfltLineCnt;

		GrDumyZeroMem( m_CusClrTbl, sizeof(m_CusClrTbl) );

		m_CusColWidth		=	E_GrUiDoGrpSelDfltCusColWidth;
		m_CusLineWidth	=	0;
		m_LineItmSize		=	E_GrUiDoGrpSelDfltLineItmSize;
		m_ColItmSize		=	E_GrUiDoGrpSelDfltColItmSize;
		m_LineGap				=	E_GrUiDoGrpSelDfltLineGap;
		m_BitGroupCnt		=	0;
		m_BitGroupGap		=	0;

		m_RltColLblId		=	E_GrUiObjIdNone;
		m_RltLineLblId	=	E_GrUiObjIdNone;

		m_CusPosCol			=	0;
		m_CusPosLine		=	0;

		m_ColSelClr			=	0;
#ifdef GR_OSD_EDIT
		m_ColSelSt			=	9;
		m_ColSelCnt			=	2;
#else
		m_ColSelSt			=	0;
		m_ColSelCnt			=	0;
#endif

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoGrpSelMaxBitGroupCnt;Tv_WkIdx++ )
		{
			m_GroupBitTbl[Tv_WkIdx]	=	(__u8)Tv_WkIdx;
		}

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoGrpSelMaxColorCnt;Tv_WkIdx++ )
		{
			m_ClrAtbTbl[Tv_WkIdx]	=	0;
		}

#ifdef GR_OSD_EDIT
		Tv_Tmp	=	0;
		Tv_PtrVal	=	m_ItmAtbTbl[0];
		for ( Tv_WkIdx=0;Tv_WkIdx < (E_GrUiDoGrpSelMaxLineCnt*E_GrUiDoGrpSelMaxColCnt);Tv_WkIdx++ )
		{
			*Tv_PtrVal	=	Tv_Tmp;
			Tv_Tmp ++;
			Tv_PtrVal ++;
		}
#else
		GrDumyZeroMem( m_ItmAtbTbl, sizeof(m_ItmAtbTbl) );
#endif

}
//--------------------------------------------------------------------
Cls_GrUiDoGrpSel::~Cls_GrUiDoGrpSel()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::LcDrawItems( void )
{
	// local -------------------
		__s32		Tv_X;
		__s32		Tv_Y;
		St_GrRect	Tv_Rc;
		__u32	Tv_ColIdx;
		__u32	Tv_GroupSubIdx;
		__u32	Tv_GroupIdx;
		__u8	Tv_Atb;
		__u32	Tv_Color;
	// code --------------------

		Tv_GroupIdx			=	0;
		Tv_GroupSubIdx	=	0;
		Tv_Y						=	m_PntRcClient.top;
		while ( (m_PntRcClient.bottom > Tv_Y) && (Tv_GroupIdx < m_LineCnt) )
		{
			Tv_ColIdx	=	0;
			Tv_X	=	m_PntRcClient.left;
			while ( (m_PntRcClient.right > Tv_X) && (Tv_ColIdx < m_ColCnt) )
			{
				//make rect
				Tv_Rc.left		=	Tv_X;
				Tv_Rc.top			=	Tv_Y;
				Tv_Rc.right		=	Tv_Rc.left + (__s32)m_ColItmSize;
				Tv_Rc.bottom	=	Tv_Rc.top + (__s32)m_LineItmSize;
				Tv_Atb	=	m_ItmAtbTbl[Tv_GroupIdx][Tv_ColIdx];
				if ( 0 == m_BitGroupCnt )
				{
					if ( (E_GrUiDoGrpSelMaxColorCnt-1) <= Tv_Atb )
					{
						Tv_Atb	=	0;
					}
				}
				else
				{
					if ( 0 != (Tv_Atb & (1 << m_GroupBitTbl[Tv_GroupSubIdx])) )
					{
						Tv_Atb	=	(__u8)Tv_GroupSubIdx + 1;
					}
					else
					{
						Tv_Atb	=	0;
					}
				}
				Tv_Color	=	m_ClrAtbTbl[Tv_Atb];
				//check selected
				if ( (0 != m_ColSelCnt) && ((__u8)Tv_ColIdx >= m_ColSelSt) && ((__u8)Tv_ColIdx < (m_ColSelSt + m_ColSelCnt) ) )
				{
					if ( 0 != Tv_Atb )
					{
						Tv_Color	=	m_ColSelClr;
					}
				}
				m_GdibScr->FillRect( &Tv_Rc, Tv_Color );

				//next
				Tv_X	=	Tv_X + (__s32)m_ColItmSize;
				Tv_ColIdx ++;
			}
			//next
			Tv_Y	=	Tv_Y + (__s32)m_LineItmSize;
			Tv_GroupSubIdx ++;
			if ( 0 == m_BitGroupCnt )
			{
				Tv_GroupIdx ++;
				Tv_Y	=	Tv_Y + (__s32)m_LineGap;
			}
			else if ( Tv_GroupSubIdx < m_BitGroupCnt )
			{
				Tv_Y	=	Tv_Y + (__s32)m_LineGap;
			}
			else
			{
				Tv_Y	=	Tv_Y + (__s32)m_BitGroupGap;
				Tv_GroupSubIdx	=	0;
				Tv_GroupIdx ++;
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpSel::LcCusStatGet( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrUiDoGrpSelCusClrDisable;

		if ( IsCanEnable() )
		{
			Tv_Result	=	E_GrUiDoGrpSelCusClrNormal;
			if ( IsFocused() || IsOnMouse() )
			{
				Tv_Result	=	E_GrUiDoGrpSelCusClrFocus;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::LcDrawCusCol( __u32 A_CusStat )
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Rc.left	=	m_PntRcClient.left + ((__s32)m_CusPosCol * (__s32)m_ColItmSize);
		if ( Tv_Rc.left < m_PntRcClient.right )
		{
			Tv_Rc.top			=	m_PntRcClient.top;
			Tv_Rc.right		=	Tv_Rc.left + (__s32)m_CusColWidth;
			Tv_Rc.bottom	=	m_PntRcClient.bottom;

			m_GdibScr->FillRect( &Tv_Rc, m_CusClrTbl[A_CusStat] );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::LcDrawCusLine( __u32 A_CusStat )
{
	// local -------------------
		__s32		Tv_Y;
		St_GrRect	Tv_Rc;
		__u32	Tv_GroupSubIdx;
		__u32	Tv_GroupIdx;
		__u32	Tv_GrupTtWidth;
	// code --------------------
		Tv_GroupIdx			=	0;
		Tv_GroupSubIdx	=	0;
		Tv_Y						=	m_PntRcClient.top;
		while ( m_PntRcClient.bottom > Tv_Y )
		{
			//check find
			if ( m_CusPosLine == Tv_GroupIdx )
			{
				//finded
				Tv_GrupTtWidth	=	m_LineItmSize;
				if ( 0 != m_BitGroupCnt )
				{
					Tv_GrupTtWidth	=	(m_LineItmSize * m_BitGroupCnt) + (m_LineGap * (m_BitGroupCnt-1));
				}
				Tv_Rc.left	=	m_PntRcClient.left;
				Tv_Rc.top		=	Tv_Y + ( ( (__s32)Tv_GrupTtWidth - (__s32)m_CusLineWidth ) >> 1 );
				Tv_Rc.right	=	m_PntRcClient.right;
				Tv_Rc.bottom	=	Tv_Rc.top + (__s32)m_CusLineWidth;
				m_GdibScr->FillRect( &Tv_Rc, m_CusClrTbl[A_CusStat] );
				break;
			}
			//next
			Tv_Y	=	Tv_Y + (__s32)m_LineItmSize;
			Tv_GroupSubIdx ++;
			if ( 0 == m_BitGroupCnt )
			{
				Tv_GroupIdx ++;
				Tv_Y	=	Tv_Y + (__s32)m_LineGap;
			}
			else if ( Tv_GroupSubIdx < m_BitGroupCnt )
			{
				Tv_Y	=	Tv_Y + (__s32)m_LineGap;
			}
			else
			{
				Tv_Y	=	Tv_Y + (__s32)m_BitGroupGap;
				Tv_GroupSubIdx	=	0;
				Tv_GroupIdx ++;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::ProcBgSkinTblDraw( void )
{
	// local -------------------
		__u32	Tv_CusStat;
	// code --------------------
		//inheritd
		Cls_GrUiDoSkin::ProcBgSkinTblDraw();

		Tv_CusStat	=	LcCusStatGet();
		//draw line cursor
		if ( 0 != m_CusLineWidth )
		{
			LcDrawCusLine( Tv_CusStat );
		}

		//draw block item
		LcDrawItems();

		//draw colum cursor
		if ( 0 != m_CusColWidth )
		{
			LcDrawCusCol( Tv_CusStat );
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpSel::ColumCountGet( void )
{
		return	m_ColCnt;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpSel::LineCountGet( void )
{
		return	m_LineCnt;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::ColumCountSet( __u8 A_Cnt )
{
		if ( (0 != A_Cnt) && (E_GrUiDoGrpSelMaxColCnt >= A_Cnt) )
		{
			m_ColCnt		=	A_Cnt;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::LineCountSet( __u8 A_Cnt )
{
		if ( (0 != A_Cnt) && (E_GrUiDoGrpSelMaxLineCnt >= A_Cnt) )
		{
			m_LineCnt	=	A_Cnt;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpSel::CusColorGet( __u32 A_Idx )
{
		return	m_CusClrTbl[A_Idx];
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::CusColorSet( __u32 A_Idx, __u32 A_Color )
{
	if ( E_GrUiDoGrpSelCusClrCnt > A_Idx )
	{
		m_CusClrTbl[A_Idx]	=	A_Color;
		ReqDraw();
	}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpSel::AtbColorGet( __u32 A_Idx )
{
		return	m_ClrAtbTbl[A_Idx];
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::AtbColorSet( __u32 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoGrpSelMaxColorCnt > A_Idx )
		{
			m_ClrAtbTbl[A_Idx]	=	A_Color;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpSel::BitGroupPosGet( __u32 A_Idx )
{
		return	m_GroupBitTbl[A_Idx];
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::BitGroupPosSet( __u32 A_Idx, __u8 A_Pos )
{
		if ( E_GrUiDoGrpSelMaxBitGroupCnt > A_Idx )
		{
			m_GroupBitTbl[A_Idx]	=	A_Pos;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpSel::CusColumWidthGet( void )
{
		return	m_CusColWidth;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::CusColumWidthSet( __u32 A_Width )
{
		m_CusColWidth	=	A_Width;
		ReqDraw();
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpSel::CusLineWidthGet( void )
{
		return	m_CusLineWidth;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::CusLineWidthSet( __u32 A_Width )
{
		m_CusLineWidth	=	A_Width;
		ReqDraw();
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpSel::LineItemSizeGet( void )
{
		return	m_LineItmSize;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpSel::ColumItemSizeGet( void )
{
		return	m_ColItmSize;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpSel::LineGapGet( void )
{
		return	m_LineGap;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpSel::BitGroupCountGet( void )
{
		return	m_BitGroupCnt;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpSel::BitGroupGapGet( void )
{
		return	m_BitGroupGap;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoGrpSel::RltColumLableIdGet( void )
{
		return	m_RltColLblId;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoGrpSel::RltLineLableIdGet( void )
{
		return	m_RltLineLblId;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::LineItemSizeSet( __u32 A_Size )
{
		m_LineItmSize	=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::ColumItemSizeSet( __u32 A_Size )
{
		m_ColItmSize	=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::LineGapSet( __u32 A_Gap )
{
		m_LineGap	=	A_Gap;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::BitGroupCountSet( __u8 A_Cnt )
{
		if ( E_GrUiDoGrpSelMaxBitGroupCnt >= A_Cnt )
		{
			m_BitGroupCnt	=	A_Cnt;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::BitGroupGapSet( __u32 A_Gap )
{
		m_BitGroupGap	=	A_Gap;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::RltColumLableIdSet( __u16 A_ScptId )
{
		m_RltColLblId	=	A_ScptId;
		LcCusPosRltCtrlProc();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::RltLineLableIdSet( __u16 A_ScptId )
{
		m_RltLineLblId	=	A_ScptId;
		LcCusPosRltCtrlProc();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::ItemValSet( __u8 A_Colum, __u8 A_Line, __u8 A_Val )
{
		if ( (E_GrUiDoGrpSelMaxColCnt > A_Colum) && (E_GrUiDoGrpSelMaxLineCnt > A_Line) )
		{
			m_ItmAtbTbl[A_Line][A_Colum]	=	A_Val;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpSel::ItemValGet( __u8 A_Colum, __u8 A_Line )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( (E_GrUiDoGrpSelMaxColCnt > A_Colum) && (E_GrUiDoGrpSelMaxLineCnt > A_Line) )
		{
			Tv_Result	=	m_ItmAtbTbl[A_Line][A_Colum];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoGrpSel::ProcEvtKeyDown( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsCusChg;
	// code --------------------
		//inherited
		Tv_Result	=	Cls_GrUiDoSkin::ProcEvtKeyDown( A_Key );
		
		//check key type
		Tv_IsCusChg	=	FALSE;
		switch( A_Key )
		{
			case	E_GrUiKeyDown:
				if ( 0 != m_CusLineWidth )
				{
					if ( m_LineCnt <= (m_CusPosLine + 1) )
					{
						//last position
						if ( 0 == (E_GrUiDoFocusMapDown & m_FocusMoveMap)  )
						{
							LcReqDrawCusLine();
							m_CusPosLine	=	0;
							LcReqDrawCusLine();
							Tv_IsCusChg		=	TRUE;
						}
					}
					else
					{
						LcReqDrawCusLine();
						m_CusPosLine ++;
						LcReqDrawCusLine();
						Tv_IsCusChg		=	TRUE;
					}
				}
				break;
			case	E_GrUiKeyUp:
				if ( 0 != m_CusLineWidth )
				{
					if ( 0 == m_CusPosLine )
					{
						//last position
						if ( 0 == (E_GrUiDoFocusMapUp & m_FocusMoveMap)  )
						{
							LcReqDrawCusLine();
							m_CusPosLine	=	m_LineCnt - 1;
							LcReqDrawCusLine();
							Tv_IsCusChg		=	TRUE;
						}
					}
					else
					{
						LcReqDrawCusLine();
						m_CusPosLine --;
						LcReqDrawCusLine();
						Tv_IsCusChg		=	TRUE;
					}
				}
				break;
			case	E_GrUiKeyLeft:
				if ( 0 != m_CusColWidth )
				{
					if ( 0 == m_CusPosCol )
					{
						//last position
						if ( 0 == (E_GrUiDoFocusMapLeft & m_FocusMoveMap)  )
						{
							LcReqDrawCusCol();
							m_CusPosCol	=	m_ColCnt - 1;
							LcReqDrawCusCol();
							Tv_IsCusChg		=	TRUE;
						}
					}
					else
					{
						LcReqDrawCusCol();
						m_CusPosCol --;
						LcReqDrawCusCol();
						Tv_IsCusChg		=	TRUE;
					}
				}
				break;
			case	E_GrUiKeyRight:
				if ( 0 != m_CusColWidth )
				{
					if ( m_ColCnt <= (m_CusPosCol + 1) )
					{
						//last position
						if ( 0 == (E_GrUiDoFocusMapRight & m_FocusMoveMap)  )
						{
							LcReqDrawCusCol();
							m_CusPosCol	=	0;
							LcReqDrawCusCol();
							Tv_IsCusChg		=	TRUE;
						}
					}
					else
					{
						LcReqDrawCusCol();
						m_CusPosCol ++;
						LcReqDrawCusCol();
						Tv_IsCusChg		=	TRUE;
					}
				}
				break;
		}

		//check cursor change
		if ( Tv_IsCusChg )
		{
			//relate control do
			LcCusPosRltCtrlProc();
			LcMsgMakeCusChg();
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoGrpSel::ProcEvtKeyUp( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//inherited
		Tv_Result	=	Cls_GrUiDoSkin::ProcEvtKeyDown( A_Key );
		if ( E_GrUiKeyEnter == A_Key )
		{
			//do select event
			LcMsgMakeSelect();
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::LcCusPosRltCtrlProc( void )
{
	// local -------------------
		Cls_GrUiDoLbl*	Tv_ObjLbl;
		Def_WstrNum	Tv_StrNum;
	// code --------------------
		//colum
		if ( E_GrUiObjIdNone != m_RltColLblId )
		{
			Tv_ObjLbl	=	(Cls_GrUiDoLbl*)m_ObjWin->FindSlaveObjByObjId( m_RltColLblId );
			if ( NULL != Tv_ObjLbl )
			{
				GrStrIntToWstr( Tv_StrNum, (__s32)m_CusPosCol, 2, TRUE );
				Tv_ObjLbl->TextSet( Tv_StrNum );
			}
		}

		//line
		if ( E_GrUiObjIdNone != m_RltLineLblId )
		{
			Tv_ObjLbl	=	(Cls_GrUiDoLbl*)m_ObjWin->FindSlaveObjByObjId( m_RltLineLblId );
			if ( NULL != Tv_ObjLbl )
			{
				GrStrIntToWstr( Tv_StrNum, (__s32)m_CusPosLine, 2, TRUE );
				Tv_ObjLbl->TextSet( Tv_StrNum );
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::LcMsgMakeSelect( void )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		if ( V_GrUiMngObj->IsUserEventOn() )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdGrpSelItmSel;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::LcMsgMakeCusChg( void )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		if ( V_GrUiMngObj->IsUserEventOn() )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdGrpSelCusChg;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoGrpSel::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_CusX;
		__u32	Tv_CusY;
		__s32		Tv_Y;
		__u32	Tv_GroupSubIdx;
	// code --------------------
		Tv_Result	=	Cls_GrUiDoSkin::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );

		//calculat x cursor position
		Tv_CusX	=	(A_X - (__u32)m_PntRcClient.left) / m_ColItmSize;

		//check current x position
		if ( Tv_CusX < m_ColCnt )
		{
			//calculat y cursor position
			Tv_GroupSubIdx	=	0;
			Tv_CusY					=	0;
			Tv_Y						=	m_PntRcClient.top;
			while ( m_PntRcClient.bottom > Tv_Y )
			{
				//check find
				if ( Tv_Y <= A_Y )
				{
					//finded
					break;
				}
				//next
				Tv_Y	=	Tv_Y + (__s32)m_LineItmSize;
				Tv_GroupSubIdx ++;
				if ( 0 == m_BitGroupCnt )
				{
					Tv_CusY ++;
					Tv_Y	=	Tv_Y + (__s32)m_LineGap;
				}
				else if ( Tv_GroupSubIdx < m_BitGroupCnt )
				{
					Tv_Y	=	Tv_Y + (__s32)m_LineGap;
				}
				else
				{
					Tv_Y	=	Tv_Y + (__s32)m_BitGroupGap;
					Tv_GroupSubIdx	=	0;
					Tv_CusY ++;
				}
			}
			//check y size
			if ( Tv_CusY < m_LineCnt )
			{
				//check match
				if ( (m_CusPosCol == Tv_CusX) && (m_CusPosLine == Tv_CusY) )
				{
					//mke event
					LcMsgMakeSelect();
				}
				else
				{
					LcReqDrawCusCol();
					LcReqDrawCusLine();
					m_CusPosCol		=	Tv_CusX;
					m_CusPosLine	=	Tv_CusY;
					LcReqDrawCusCol();
					LcReqDrawCusLine();
					LcCusPosRltCtrlProc();
					LcMsgMakeCusChg();
				}
			}
		}

		Tv_Result	=	TRUE;
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::ProcEvtScptDecoded( void )
{
		Cls_GrUiDoSkin::ProcEvtScptDecoded();
		LcCusPosRltCtrlProc();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::LcReqDrawCusCol( void )
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		if ( 0 != m_CusColWidth )
		{
			Tv_Rc.left	=	m_PntRcClient.left + ((__s32)m_CusPosCol * (__s32)m_ColItmSize);
			if ( Tv_Rc.left < m_PntRcClient.right )
			{
				Tv_Rc.top			=	m_PntRcClient.top;
				Tv_Rc.right		=	Tv_Rc.left + (__s32)m_CusColWidth;
				Tv_Rc.bottom	=	m_PntRcClient.bottom;
				m_ObjConsole->PaintReq( &Tv_Rc );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::LcReqDrawCusLine( void )
{
	// local -------------------
		__s32		Tv_Y;
		St_GrRect	Tv_Rc;
		__u32	Tv_GroupSubIdx;
		__u32	Tv_GroupIdx;
		__u32	Tv_GrupTtWidth;
	// code --------------------
		if ( 0 != m_CusLineWidth )
		{
			Tv_GroupIdx			=	0;
			Tv_GroupSubIdx	=	0;
			Tv_Y						=	m_PntRcClient.top;
			while ( m_PntRcClient.bottom > Tv_Y )
			{
				//check find
				if ( m_CusPosLine == Tv_GroupIdx )
				{
					//finded
					Tv_GrupTtWidth	=	m_LineItmSize;
					if ( 0 != m_BitGroupCnt )
					{
						Tv_GrupTtWidth	=	(m_LineItmSize * m_BitGroupCnt) + (m_LineGap * (m_BitGroupCnt-1));
					}
					Tv_Rc.left	=	m_PntRcClient.left;
					Tv_Rc.top		=	Tv_Y + ( ( (__s32)Tv_GrupTtWidth - (__s32)m_CusLineWidth ) >> 1 );
					Tv_Rc.right	=	m_PntRcClient.right;
					Tv_Rc.bottom	=	Tv_Rc.top + (__s32)m_CusLineWidth;
					m_ObjConsole->PaintReq( &Tv_Rc );
					break;
				}
				//next
				Tv_Y	=	Tv_Y + (__s32)m_LineItmSize;
				Tv_GroupSubIdx ++;
				if ( 0 == m_BitGroupCnt )
				{
					Tv_GroupIdx ++;
					Tv_Y	=	Tv_Y + (__s32)m_LineGap;
				}
				else if ( Tv_GroupSubIdx < m_BitGroupCnt )
				{
					Tv_Y	=	Tv_Y + (__s32)m_LineGap;
				}
				else
				{
					Tv_Y	=	Tv_Y + (__s32)m_BitGroupGap;
					Tv_GroupSubIdx	=	0;
					Tv_GroupIdx ++;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::ItemValReset( void )
{
		GrDumyZeroMem( m_ItmAtbTbl, sizeof(m_ItmAtbTbl) );
		ReqDraw();
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpSel::CusColPosGet( void )
{
		return	m_CusPosCol;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpSel::CusLinePosGet( void )
{
		return	m_CusPosLine;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::CusPosSet( __u32 A_Colum, __u32 A_Line )
{
		if ( (A_Colum < m_ColCnt) && (A_Line < m_LineCnt) )
		{
			m_CusPosCol		=	A_Colum;
			m_CusPosLine	=	A_Line;

			LcMsgMakeCusChg();
			LcCusPosRltCtrlProc();
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpSel::ColumSelColorGet( void )
{
		return	m_ColSelClr;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::ColumSelColorSet( __u32 A_Color )
{
		m_ColSelClr	=	A_Color;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpSel::ColSelSet( __u8 A_StIdx, __u8 A_Cnt )
{
		m_ColSelSt	=	A_StIdx;
		m_ColSelCnt	=	A_Cnt;
		ReqDraw();
}
//--------------------------------------------------------------------

