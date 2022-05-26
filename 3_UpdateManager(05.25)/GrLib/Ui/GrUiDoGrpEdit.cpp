/*
 channel graph edit

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoGrpEdit.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiScript.h>
#include	<Ui/GrUiDoLbl.h>
#include	<GrStrTool.h>

//====================================================================
//local const
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

#define E_GrUiDoGrpEditKeyCodeDelete	0x100

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*				V_GrUiMngObj;

#ifdef GR_OSD_EDIT
	__u8	V_GrUiDoGrpEditTestVal[32][60];		//test value
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoGrpEdit::Cls_GrUiDoGrpEdit( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		//init
		m_TypeId			=	E_GrUiDoTypeIdGrpEdit;
		m_IsFocusAble	=	TRUE;

		m_ColCnt			=	E_GrUiDoGrpEditDfltColCnt;
		m_LineCnt			=	E_GrUiDoGrpEditDfltLineCnt;
		m_ValCnt			=	E_GrUiDoGrpEditDfltValCnt;
		m_IsBitMode		=	FALSE;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrUiDoGrpEditMaxBitCnt;Tv_WkIdx++ )
		{
			m_BitPosTbl[Tv_WkIdx]	=	(__u8)Tv_WkIdx;
		}
		
		GrDumyZeroMem( m_ClrAtbTbl, sizeof(m_ClrAtbTbl) );

		m_ClrGrid			=	m_ObjConsole->ClrWhiteGet();
		m_GridWidth		=	E_GrUiDoGrpEditDfltGridWidth;
		m_ClrCus			=	m_ObjConsole->ClrBlackGet();
		m_CusWidth		=	E_GrUiDoGrpEditDfltCusWidth;
		m_LineItmSize	=	E_GrUiDoGrpEditDfltLineItmSize;
		m_ColItmSize	=	E_GrUiDoGrpEditDfltColItmSize;
		m_LineGap			=	0;

		m_CusStX			=	0;
		m_CusStY			=	0;
		m_CusEdX			=	0;
		m_CusEdY			=	0;

		m_WriteVal		=	1;

		m_RltLblColId		=	E_GrUiObjIdNone;
		m_RltLblLineId	=	E_GrUiObjIdNone;

#ifdef GR_OSD_EDIT
		m_PtrValBuf	=	(void*)V_GrUiDoGrpEditTestVal;
#else
		m_PtrValBuf	=	NULL;
#endif

}
//--------------------------------------------------------------------
Cls_GrUiDoGrpEdit::~Cls_GrUiDoGrpEdit()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::LcDrawItems( void )
{
	// local -------------------
		__u32		Tv_X;
		__u32		Tv_Y;
		__u8		Tv_LineIdx;
		__u8		Tv_ColIdx;
		St_GrRect	Tv_Rc;
		void*		Tv_PtrLineVal;
		__u8*		Tv_PtrVal;
		__u8		Tv_Val;
	// code --------------------
		//check exist value buffer
		if ( NULL != m_PtrValBuf )
		{
			Tv_Y		=	(__u32)m_PntRcClient.top;
			Tv_LineIdx	=	0;
			Tv_PtrLineVal	=	(__u8*)m_PtrValBuf;
			while ( m_PntRcClient.bottom > (__s32)Tv_Y )
			{
				//check item count
				if ( (m_LineCnt <= Tv_LineIdx)  || ( m_IsBitMode && ( E_GrUiDoGrpEditMaxBitCnt <= Tv_LineIdx ) ) )
				{
					break;
				}
				//draw x direction
				Tv_X	=	(__u32)m_PntRcClient.left;
				Tv_ColIdx	=	0;
				Tv_PtrVal	=	(__u8*)Tv_PtrLineVal;
				while ( m_PntRcClient.right > (__s32)Tv_X )
				{
					//check colum index
					if ( m_ColCnt <= Tv_ColIdx )
					{
						break;
					}
					//get index
					Tv_Val	=	0;
					if ( m_IsBitMode )
					{
						if ( 0 != ( (*Tv_PtrVal) & (1 << m_BitPosTbl[Tv_LineIdx]) ) )
						{
							Tv_Val	=	Tv_LineIdx + 1;
						}
					}
					else
					{
						Tv_Val	=	*Tv_PtrVal;
						Tv_Val	=	LcValToIdx( Tv_Val );
					}
					//draw
					Tv_Rc.top			=	(__s32)Tv_Y;
					Tv_Rc.left		=	(__s32)Tv_X;
					Tv_Rc.right		=	Tv_Rc.left + (__s32)m_ColItmSize;
					Tv_Rc.bottom	=	Tv_Rc.top + (__s32)m_LineItmSize;
					m_GdibScr->FillRect( &Tv_Rc, m_ClrAtbTbl[Tv_Val] );
					//next
					Tv_PtrVal ++;
					Tv_X	=	Tv_X + (__u32)m_ColItmSize;
					Tv_ColIdx ++;
				}
				//next
				if ( !m_IsBitMode )
				{
					Tv_PtrLineVal	=	(void*)( (__u32)Tv_PtrLineVal + (__u32)m_ColCnt );
				}
				Tv_LineIdx ++;
				Tv_Y	=	Tv_Y + (__u32)m_LineItmSize + (__u32)m_LineGap;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::LcDrawOneCus( __u8 A_X, __u8 A_Y )
{
	// local -------------------
		__s32		Tv_StX;
		__s32		Tv_StY;
		__s32		Tv_EdX;
		__s32		Tv_EdY;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_StX	=	m_PntRcClient.left + (((__s32)m_ColItmSize * (__s32)A_X) + (__s32)(m_GridWidth >> 1));
		Tv_StY	=	m_PntRcClient.top + (( ( (__s32)m_LineItmSize + (__s32)m_LineGap ) * (__s32)A_Y ) + (__s32)(m_GridWidth >> 1));
		Tv_EdX	=	Tv_StX + (__s32)m_ColItmSize -(__s32)m_GridWidth;
		Tv_EdY	=	Tv_StY + (__s32)m_LineItmSize - (__s32)m_GridWidth;
		//draw top cursor
		Tv_Rc.left		=	Tv_StX;
		Tv_Rc.top			=	Tv_StY;
		Tv_Rc.right		=	Tv_EdX;
		Tv_Rc.bottom	=	Tv_Rc.top + (__s32)m_CusWidth;
		m_GdibScr->FillRect( &Tv_Rc, m_ClrCus );
		//draw left cursor
		Tv_Rc.left		=	Tv_StX;
		Tv_Rc.top			=	Tv_StY;
		Tv_Rc.right		=	Tv_Rc.left + (__s32)m_CusWidth;
		Tv_Rc.bottom	=	Tv_EdY;
		m_GdibScr->FillRect( &Tv_Rc, m_ClrCus );
		//draw bottom
		Tv_Rc.left		=	Tv_StX;
		Tv_Rc.top			=	Tv_EdY - (__s32)m_CusWidth;
		Tv_Rc.right		=	Tv_EdX;
		Tv_Rc.bottom	=	Tv_EdY;
		m_GdibScr->FillRect( &Tv_Rc, m_ClrCus );
		//draw right
		Tv_Rc.left		=	Tv_EdX - (__s32)m_CusWidth;
		Tv_Rc.top			=	Tv_StY;
		Tv_Rc.right		=	Tv_EdX;
		Tv_Rc.bottom	=	Tv_EdY;
		m_GdibScr->FillRect( &Tv_Rc, m_ClrCus );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::LcDrawCus( void )
{
	// local -------------------
		__u8	Tv_StX;
		__u8	Tv_StY;
		__u8	Tv_EdX;
		__u8	Tv_EdY;
		__u8	Tv_WkX;
		__u8	Tv_WkY;
	// code --------------------
		if ( IsFocused() || IsOnMouse() )
		{
			if ( 0 != m_CusWidth )
			{
				//align position
				Tv_StX	=	m_CusStX;
				Tv_StY	=	m_CusStY;
				Tv_EdX	=	m_CusEdX;
				Tv_EdY	=	m_CusEdY;
				if ( Tv_StX > Tv_EdX )
				{
					Tv_StX	=	m_CusEdX;
					Tv_EdX	=	m_CusStX;
				}
				if ( Tv_StY > Tv_EdY )
				{
					Tv_StY	=	m_CusEdY;
					Tv_EdY	=	m_CusStY;
				}
				
				//draw
				Tv_WkY	=	Tv_StY;
				while ( Tv_WkY <= Tv_EdY  )
				{
					Tv_WkX	=	Tv_StX;
					while ( Tv_WkX <= Tv_EdX )
					{
						LcDrawOneCus( Tv_WkX, Tv_WkY );
						//next
						Tv_WkX ++;
					}
					//next
					Tv_WkY ++;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::LcDrawGrid( void )
{
	// local -------------------
		__u32		Tv_X;
		__u32		Tv_Y;
		__u8		Tv_LineIdx;
		__u8		Tv_ColIdx;
		St_GrRect	Tv_Rc;
	// code --------------------
		//check exist value buffer
		if ( 0 != m_GridWidth )
		{
			//horizontal line
			Tv_Y		=	(__u32)m_PntRcClient.top + (__u32)m_LineItmSize + (__u32)m_LineGap;
			Tv_LineIdx	=	0;
			Tv_Rc.left		=	m_PntRcClient.left;
			Tv_Rc.right		=	m_PntRcClient.right;
			while ( m_PntRcClient.bottom > (__s32)Tv_Y )
			{
				//check item count
				if ( (m_LineCnt <= (Tv_LineIdx + 1) )  || ( m_IsBitMode && ( E_GrUiDoGrpEditMaxBitCnt <= (Tv_LineIdx + 1) ) ) )
				{
					break;
				}
				//draw line
				Tv_Rc.top			=	(__s32)Tv_Y - (__s32)( m_GridWidth >> 1);
				Tv_Rc.bottom	=	Tv_Rc.top + (__s32)m_GridWidth;
				m_GdibScr->FillRect( &Tv_Rc, m_ClrGrid );
				//next
				Tv_LineIdx ++;
				Tv_Y	=	Tv_Y + (__u32)m_LineItmSize + (__u32)m_LineGap;
			}

			//vertical line
			Tv_X	=	(__u32)m_PntRcClient.left + (__u32)m_ColItmSize;
			Tv_ColIdx	=	0;
			Tv_Rc.top			=	m_PntRcClient.top;
			Tv_Rc.bottom	=	m_PntRcClient.bottom;
			while ( m_PntRcClient.right > (__s32)Tv_X )
			{
				//check colum index
				if ( m_ColCnt <= (Tv_ColIdx + 1) )
				{
					break;
				}
				//draw
				Tv_Rc.left		=	(__s32)Tv_X - (__s32)( m_GridWidth >> 1);
				Tv_Rc.right		=	Tv_Rc.left + (__s32)m_GridWidth;
				m_GdibScr->FillRect( &Tv_Rc, m_ClrGrid );
				//next
				Tv_X	=	Tv_X + (__u32)m_ColItmSize;
				Tv_ColIdx ++;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ProcBgSkinTblDraw( void )
{
	// local -------------------
	// code --------------------
		//inherited
		Cls_GrUiDoSkin::ProcBgSkinTblDraw();

		//item blocks
		LcDrawItems();

		//cursor
		LcDrawCus();

		//grid line
		LcDrawGrid();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoGrpEdit::ProcEvtKeyDown( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	Cls_GrUiDoSkin::ProcEvtKeyDown( A_Key );

		if ( !Tv_Result )
		{
			Tv_Result	=	TRUE;
			switch( A_Key )
			{
				case	E_GrUiKeyUp:
					if ( 0 == m_CusStY )
					{
						if ( 0 == (E_GrUiDoFocusMapUp & m_FocusMoveMap) )
						{
							CursorPosSet( m_CusStX, m_LineCnt - 1 );
						}
						else
						{
							Tv_Result	=	FALSE;
						}
					}
					else
					{
						CursorPosSet( m_CusStX, m_CusStY - 1 );
					}
					break;
				case	E_GrUiKeyDown:
					if ( m_LineCnt <= (m_CusStY + 1) )
					{
						if ( 0 == (E_GrUiDoFocusMapDown & m_FocusMoveMap) )
						{
							CursorPosSet( m_CusStX, 0 );
						}
						else
						{
							Tv_Result	=	FALSE;
						}
					}
					else
					{
						CursorPosSet( m_CusStX, m_CusStY + 1 );
					}
					break;
				case	E_GrUiKeyLeft:
					if ( 0 == m_CusStX )
					{
						if ( 0 == (E_GrUiDoFocusMapLeft & m_FocusMoveMap) )
						{
							CursorPosSet( m_ColCnt - 1, m_CusStY );
						}
						else
						{
							Tv_Result	=	FALSE;
						}
					}
					else
					{
						CursorPosSet( m_CusStX -1 , m_CusStY );
					}
					break;
				case	E_GrUiKeyRight:
					if ( m_ColCnt <= (m_CusStX + 1) )
					{
						if ( 0 == (E_GrUiDoFocusMapRight & m_FocusMoveMap) )
						{
							CursorPosSet( 0, m_CusStY );
						}
						else
						{
							Tv_Result	=	FALSE;
						}
					}
					else
					{
						CursorPosSet( m_CusStX + 1, m_CusStY );
					}
					break;
				case	E_GrUiKeyEnter:
					ChangeValAfterCursor();
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoGrpEdit::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_X;
		__u8	Tv_Y;
	// code --------------------
		Tv_Result	=	Cls_GrUiDoSkin::ProcEvtMseLbtnDown( A_X, A_Y, A_BtnMap );
		//if ( !Tv_Result  )
		{
			//calculate position
			Tv_X	=	(__u8)((A_X - m_PntRcClient.left) / m_ColItmSize);
			Tv_Y	=	(__u8)((A_Y - m_PntRcClient.top) / (m_LineItmSize + m_LineGap) );
			//check able position
			if ( (Tv_X < m_ColCnt) && (Tv_Y < m_LineCnt) )
			{
				CursorPosSet( Tv_X, Tv_Y );
			}
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoGrpEdit::ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_X;
		__u8	Tv_Y;
	// code --------------------
		Tv_Result	=	Cls_GrUiDoSkin::ProcEvtMseMove( A_X, A_Y, A_BtnMap );

		if ( !Tv_Result )
		{
			//check left buffon pushed
			if ( 0 != ( E_GrUiMouseBmkkLeft & A_BtnMap) )
			{
				//calculate position
				Tv_X	=	(__u8)((A_X - m_PntRcClient.left) / m_ColItmSize);
				Tv_Y	=	(__u8)((A_Y - m_PntRcClient.top) / (m_LineItmSize + m_LineGap) );
				//check able position
				if ( Tv_X >= m_ColCnt )
				{
					Tv_X	=	m_ColCnt - 1;
				}
				if ( Tv_Y >= m_LineCnt )
				{
					Tv_Y	=	m_LineCnt - 1;
				}
				//set cursor tail
				CursorTailSet( Tv_X, Tv_Y );
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoGrpEdit::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_X;
		__u8	Tv_Y;
	// code --------------------
		Tv_Result	=	Cls_GrUiDoSkin::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );

		if ( !Tv_Result )
		{
			//calculate position
			Tv_X	=	(__u8)((A_X - m_PntRcClient.left) / m_ColItmSize);
			Tv_Y	=	(__u8)((A_Y - m_PntRcClient.top) / (m_LineItmSize + m_LineGap) );
			//check able position
			if ( Tv_X >= m_ColCnt )
			{
				Tv_X	=	m_ColCnt - 1;
			}
			if ( Tv_Y >= m_LineCnt )
			{
				Tv_Y	=	m_LineCnt - 1;
			}
			//set cursor tail
			CursorTailSet( Tv_X, Tv_Y );
			//change value
			if ( (m_CusStX == m_CusEdX) && (m_CusStY == m_CusEdY) )
			{
				ChangeValAfterCursor();
			}
			else
			{
				ChangeValOnCursor();
			}
			
			//change cursor position
			CursorPosSet( m_CusStX, m_CusStY );
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpEdit::ColumCountGet( void )
{
		return	m_ColCnt;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpEdit::LineCountGet( void )
{
		return	m_LineCnt;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoGrpEdit::IsBitMode( void )
{
		return	m_IsBitMode;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpEdit::BitPosGet( __u8 A_Idx )
{
		return	m_BitPosTbl[A_Idx];
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpEdit::ClrAtbGet( __u8 A_Idx )
{
		return	m_ClrAtbTbl[A_Idx];
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpEdit::ClrGridGet( void )
{
		return	m_ClrGrid;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpEdit::GridWidthGet( void )
{
		return	m_GridWidth;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpEdit::ClrCursorGet( void )
{
		return	m_ClrCus;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpEdit::CursorWidthGet( void )
{
		return	m_CusWidth;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpEdit::LineItemSizeGet( void )
{
		return	m_LineItmSize;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpEdit::ColumItemSizeGet( void )
{
		return	m_ColItmSize;
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoGrpEdit::LineGapGet( void )
{
		return	m_LineGap;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ColumCountSet( __u8 A_Cnt )
{
		m_ColCnt	=	A_Cnt;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::LineCountSet( __u8 A_Cnt )
{
		m_LineCnt	=	A_Cnt;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::BitModeSet( BOOL8 A_IsBitMode )
{
		m_IsBitMode	=	A_IsBitMode;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::BitPosSet( __u8 A_Idx, __u8 A_Pos )
{
		if ( E_GrUiDoGrpEditMaxBitCnt > A_Idx )
		{
			m_BitPosTbl[A_Idx]	=	A_Pos;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ClrAtbSet( __u8 A_Idx, __u32 A_Color )
{
		if ( E_GrUiDoGrpEditAtbClrCnt > A_Idx )
		{
			m_ClrAtbTbl[A_Idx]	=	A_Color;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ClrGridSet( __u32 A_Color )
{
		m_ClrGrid	=	A_Color;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::GridWidthSet( __u8 A_Width )
{
		m_GridWidth	=	A_Width;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ClrCursorSet( __u32 A_Color )
{
		m_ClrCus	=	A_Color;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::CursorWidthSet( __u8 A_Width )
{
		m_CusWidth	=	A_Width;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::LineItemSizeSet( __u32 A_Size )
{
		m_LineItmSize	=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ColumItemSizeSet( __u32 A_Size )
{
		m_ColItmSize	=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::LineGapSet( __u32 A_Size )
{
		m_LineGap	=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ValueBufferSet( void* A_PtrBuf )
{
		m_PtrValBuf	=	A_PtrBuf;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpEdit::CursorPosColGet( void )
{
		return	m_CusStX;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpEdit::CursorPosLineGet( void )
{
		return	m_CusStY;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpEdit::CursorTailColGet( void )
{
		return	m_CusEdX;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpEdit::CursorTailLineGet( void )
{
		return	m_CusEdY;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::LcReqPaintCusArea( void )
{
	// local -------------------
		St_GrRect	Tv_Rc;
	// code --------------------
		//align position
		Tv_Rc.left		=	(__s32)m_CusStX;
		Tv_Rc.top			=	(__s32)m_CusStY;
		Tv_Rc.right		=	(__s32)m_CusEdX;
		Tv_Rc.bottom	=	(__s32)m_CusEdY;
		if ( Tv_Rc.left > Tv_Rc.right )
		{
			Tv_Rc.left	=	(__s32)m_CusEdX;
			Tv_Rc.right	=	(__s32)m_CusStX;
		}
		if ( Tv_Rc.top > Tv_Rc.bottom )
		{
			Tv_Rc.top			=	(__s32)m_CusEdY;
			Tv_Rc.bottom	=	(__s32)m_CusStY;
		}

		//calculat position
		Tv_Rc.left		=	m_PntRcClient.left + (Tv_Rc.left * (__s32)m_ColItmSize);
		Tv_Rc.top			=	m_PntRcClient.top + (Tv_Rc.top * (__s32)(m_LineItmSize + m_LineGap));
		Tv_Rc.right		=	m_PntRcClient.left + ( (Tv_Rc.right + 1) * m_ColItmSize );
		Tv_Rc.bottom	=	m_PntRcClient.top + ( (Tv_Rc.bottom + 1) * (__s32)(m_LineItmSize + m_LineGap));
		m_ObjConsole->PaintReq( &Tv_Rc );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::CursorPosSet( __u8 A_Colum, __u8 A_Line )
{
		//check range
		if ( (m_ColCnt > A_Colum) && (m_LineCnt > A_Line) )
		{
			//reqdraw previous cussor position
			LcReqPaintCusArea();
			m_CusStX	=	A_Colum;
			m_CusStY	=	A_Line;
			m_CusEdX	=	A_Colum;
			m_CusEdY	=	A_Line;
			LcReqPaintCusArea();
			LcRltCusLblUpdt();
			LcNotifyEvtCusChg();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::CursorTailSet( __u8 A_Colum, __u8 A_Line )
{
		//check range
		if ( (m_ColCnt > A_Colum) && (m_LineCnt > A_Line) )
		{
			//reqdraw previous cussor position
			LcReqPaintCusArea();
			m_CusEdX	=	A_Colum;
			m_CusEdY	=	A_Line;
			LcReqPaintCusArea();
			LcNotifyEvtCusChg();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::FillAll( __u8 A_Val )
{
	// local -------------------
		__u32	Tv_Size;
	// code --------------------
		if ( NULL != m_PtrValBuf )
		{
			Tv_Size	=	m_ColCnt;
			if ( !m_IsBitMode )
			{
				Tv_Size	=	m_ColCnt * m_LineCnt;
			}
			if ( 0 != Tv_Size )
			{
				GrDumyFillB( m_PtrValBuf, Tv_Size, A_Val );
				ReqDraw();
				LcNotifyEvtValChg();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ChangeValOnCursor( void )
{
	// local -------------------
		__u8	Tv_StX;
		__u8	Tv_StY;
		__u8	Tv_EdX;
		__u8	Tv_EdY;
		__u8	Tv_WkX;
		__u8	Tv_WkY;
		__u8*	Tv_PtrVal;
	// code --------------------
		//check exist valeu buffer
		if ( NULL != m_PtrValBuf )
		{
			//align position
			Tv_StX	=	m_CusStX;
			Tv_StY	=	m_CusStY;
			Tv_EdX	=	m_CusEdX;
			Tv_EdY	=	m_CusEdY;
			if ( Tv_StX > Tv_EdX )
			{
				Tv_StX	=	m_CusEdX;
				Tv_EdX	=	m_CusStX;
			}
			if ( Tv_StY > Tv_EdY )
			{
				Tv_StY	=	m_CusEdY;
				Tv_EdY	=	m_CusStY;
			}

			//draw
			Tv_WkY	=	Tv_StY;
			while ( Tv_WkY <= Tv_EdY  )
			{
				Tv_WkX	=	Tv_StX;
				while ( Tv_WkX <= Tv_EdX )
				{
					//change value
					if ( m_IsBitMode )
					{
						Tv_PtrVal	=	(__u8*)( (__u32)m_PtrValBuf + (__u32)Tv_WkX );
						if ( E_GrUiDoGrpEditMaxBitCnt > Tv_WkY )
						{
							*Tv_PtrVal	=	(*Tv_PtrVal) ^ (1 << m_BitPosTbl[Tv_WkY]);
						}
					}
					else
					{
						Tv_PtrVal	=	(__u8*)( (__u32)m_PtrValBuf + ( (__u32)Tv_WkY * (__u32)m_ColCnt) + (__u32)Tv_WkX );
						*Tv_PtrVal	=	m_WriteVal;
					}
					//next
					Tv_WkX ++;
				}
				//next
				Tv_WkY ++;
			}
			LcReqPaintCusArea();
			LcNotifyEvtValChg();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ChangeValAfterCursor( void )
{
	// local -------------------
		__u8	Tv_WkX;
		__u8*	Tv_PtrVal;
		__u8	Tv_ValFirst;
		__u8	Tv_ValIdx;
	// code --------------------
		//check exist valeu buffer
		if ( NULL != m_PtrValBuf )
		{
			//get first value
			if ( m_IsBitMode )
			{
				Tv_PtrVal	=	(__u8*)( (__u32)m_PtrValBuf + (__u32)m_CusStX );
				Tv_ValFirst	=	*Tv_PtrVal;
				if ( E_GrUiDoGrpEditMaxBitCnt > m_CusStY )
				{
					if ( 0 != ( Tv_ValFirst & (1 << m_BitPosTbl[m_CusStY])) )
					{
						Tv_ValFirst	=	0;
					}
					else
					{
						Tv_ValFirst	=	1;
					}
				}
			}
			else
			{
				Tv_PtrVal	=	(__u8*)( (__u32)m_PtrValBuf + ( (__u32)m_CusStY * (__u32)m_ColCnt) + (__u32)m_CusStX );
				Tv_ValFirst	=	*Tv_PtrVal;
				Tv_ValIdx	=	LcValToIdx( Tv_ValFirst );
				Tv_ValIdx ++;
				if ( Tv_ValIdx >= m_ValCnt )
				{
					Tv_ValIdx	=	0;
				}
				Tv_ValFirst	=	m_BitPosTbl[Tv_ValIdx];		//get value
			}
			//align position
			Tv_WkX	=	m_CusStX;
			while ( Tv_WkX < m_ColCnt )
			{
				//change value
				if ( m_IsBitMode )
				{
					if ( E_GrUiDoGrpEditMaxBitCnt > m_CusStY )
					{
						if ( Tv_ValFirst )
						{
							*Tv_PtrVal	=	(*Tv_PtrVal) | (1 << m_BitPosTbl[m_CusStY]);
						}
						else
						{
							*Tv_PtrVal	=	(*Tv_PtrVal) & ~(1 << m_BitPosTbl[m_CusStY]);
						}
					}
				}
				else
				{
					*Tv_PtrVal	=	Tv_ValFirst;
				}
				//next
				Tv_WkX ++;
				Tv_PtrVal ++;
			}
			ReqDraw();
			LcNotifyEvtValChg();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::WriteValueSet( __u8 A_Val )
{
		m_WriteVal	=	A_Val;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ProcEvtMseLeave( void )
{
		Cls_GrUiDoSkin::ProcEvtMseLeave();
		//check different cursor startand and end
		if ( (m_CusStX != m_CusEdX) || (m_CusStY != m_CusEdY) )
		{
			CursorPosSet( m_CusStX, m_CusStY );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::LcRltCusLblUpdt( void )
{
	// local -------------------
		Cls_GrUiDoLbl*	Tv_ObjLbl;
		Def_WstrNum		Tv_StrNum;
	// code --------------------
		//colum label
		if ( E_GrUiObjIdNone != m_RltLblColId )
		{
			//find label
			Tv_ObjLbl	=	(Cls_GrUiDoLbl*)m_ObjWin->FindSlaveObjByObjId( m_RltLblColId );
			if ( NULL != Tv_ObjLbl )
			{
				GrStrIntToWstr( Tv_StrNum, (__s32)m_CusStX );
				Tv_ObjLbl->TextSet( Tv_StrNum );
			}
		}
		//line label
		if ( E_GrUiObjIdNone != m_RltLblLineId )
		{
			//find label
			Tv_ObjLbl	=	(Cls_GrUiDoLbl*)m_ObjWin->FindSlaveObjByObjId( m_RltLblLineId );
			if ( NULL != Tv_ObjLbl )
			{
				GrStrIntToWstr( Tv_StrNum, (__s32)m_CusStY );
				Tv_ObjLbl->TextSet( Tv_StrNum );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ProcEvtScptDecoded( void )
{
		Cls_GrUiDoSkin::ProcEvtScptDecoded();
		LcRltCusLblUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::RltLblColumIdSet( __u16 A_ObjId )
{
		m_RltLblColId		=	A_ObjId;
		LcRltCusLblUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::RltLblLineIdSet( __u16 A_ObjId )
{
		m_RltLblLineId	=	A_ObjId;
		LcRltCusLblUpdt();
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoGrpEdit::RltLblColumIdGet( void )
{
		return	m_RltLblColId;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoGrpEdit::RltLblLineIdGet( void )
{
		return	m_RltLblLineId;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpEdit::LcValToIdx( __u8 A_Val )
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	0;

		for ( Tv_WkIdx=0;Tv_WkIdx < m_ValCnt;Tv_WkIdx++ )
		{
			if ( A_Val == m_BitPosTbl[Tv_WkIdx] )
			{
				Tv_Result	=	Tv_WkIdx;
				break;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoGrpEdit::ValueCountGet( void )
{
		return	m_ValCnt;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ValueCountSet( __u8 A_Cnt )
{
		if ( (0 < A_Cnt) && (E_GrUiDoGrpEditMaxBitCnt >= A_Cnt) )
		{
			m_ValCnt	=	A_Cnt;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::LcNotifyEvtCusChg( void )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		if ( V_GrUiMngObj->IsUserEventOn() )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdGrpEditCusChg;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::LcNotifyEvtValChg( void )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		if ( V_GrUiMngObj->IsUserEventOn() )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdGrpEditValChg;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::LcProcEvtBtnUp( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		__u32	Tv_Val;
	// code --------------------
		Tv_Val	=	A_Obj->UserDataGet();
		if ( 0 != (E_GrUiDoGrpEditKeyCodeDelete & Tv_Val) )
		{
			//fill all
			FillAll( (__u8)(0xFF & Tv_Val) );
		}
		else
		{
			m_WriteVal	=	(__u8)Tv_Val;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoGrpEdit::ProcUiMsg( Ptr_GrUiMsgHd A_PtrMsg )
{
	//default event handler
	switch( A_PtrMsg->Cmd )
	{
		case	E_GrUiMsgCmdBtnUp:
			LcProcEvtBtnUp( (Cls_GrUiDoBase*)((Ptr_GrUiMsgCtrl)A_PtrMsg)->Obj );
			break;
		default:
			Cls_GrUiDoSkin::ProcUiMsg( A_PtrMsg );
			break;
	}
}
//--------------------------------------------------------------------

