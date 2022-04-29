/*
 bit grid

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoBitGrid.h>
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

#ifdef GR_OSD_EDIT
	__u8	V_GrUiDoBitGridTestVal[4][32];		//test value
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoBitGrid::Cls_GrUiDoBitGrid( Ptr_GrRect A_PtrRect, Cls_GrUiDoBase* A_ObjPrnt, BOOL8 A_IsIdAlloc ):
Cls_GrUiDoSkin( A_PtrRect, A_ObjPrnt, E_GrUiConIdxFocused, A_IsIdAlloc )
{
	// local -------------------
	// code --------------------
		//init
		m_TypeId			=	E_GrUiDoTypeIdBitGrid;
		m_IsFocusAble	=	TRUE;

		m_CelSizeX		=	E_GrUiDoBitGridDfltCellSizeX;
		m_CelSizeY		=	E_GrUiDoBitGridDfltCellSizeY;
		m_GridCntX		=	E_GrUiDoBitGridDfltGridCntX;
		m_GridCntY		=	E_GrUiDoBitGridDfltGridCntY;
		m_GridWidth		=	E_GrUiDoBitGridDfltGridWidth;
		m_ClrGrid			=	m_ObjConsole->ClrWhiteGet();

		m_ClrMark			=	m_ObjConsole->ClrBlackGet();
		m_MarkWidth		=	E_GrUiDoBitGridDfltMarkWidth;

		m_ClrCus			=	m_ObjConsole->ClrWhiteGet();
		m_CusWidth		=	E_GrUiDoBitGridDfltCusWidth;

		m_ClrDisp			=	0;

		m_CusStX			=	0;
		m_CusStY			=	0;
		m_CusEdX			=	0;
		m_CusEdY			=	0;

		m_IsKeyCusTail	=	FALSE;

		m_IsMosaicUse	= FALSE;//new-120905~mosaic , default, Motion Use

#ifdef GR_OSD_EDIT
		m_PtrValBuf	=	(void*)V_GrUiDoBitGridTestVal;
		m_ValPitch	=	4;
		m_PtrDispBuf	=	(void*)V_GrUiDoBitGridTestVal;
#else
		m_PtrValBuf	=	NULL;
		m_ValPitch	=	0;
		m_PtrDispBuf	=	NULL;
#endif

}
//--------------------------------------------------------------------
Cls_GrUiDoBitGrid::~Cls_GrUiDoBitGrid()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::LcDrawGrid( void )
{
	// local -------------------
		__u32	Tv_Idx;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_Idx				=	0;
		Tv_Rc.top			=	m_PntRcClient.top;
		Tv_Rc.bottom	=	m_PntRcClient.bottom;
		Tv_Rc.left		=	m_PntRcClient.left + (__s32)m_CelSizeX - (__s32)(m_GridWidth >> 1);
		Tv_Rc.right		=	Tv_Rc.left + (__s32)m_GridWidth;
		while ( (Tv_Rc.left < m_PntRcClient.right) && (Tv_Idx < m_GridCntX) )
		{
			//draw grid
			m_GdibScr->FillRect( &Tv_Rc, m_ClrGrid );
			//next
			Tv_Rc.left		=	Tv_Rc.left + (__s32)m_CelSizeX;
			Tv_Rc.right		=	Tv_Rc.left + (__s32)m_GridWidth;
			Tv_Idx ++;
		}
		Tv_Idx	=	0;
		Tv_Rc.left		=	m_PntRcClient.left;
		Tv_Rc.right		=	m_PntRcClient.right;
		Tv_Rc.top			=	m_PntRcClient.top + (__s32)m_CelSizeY - (__s32)(m_GridWidth >> 1);
		Tv_Rc.bottom	=	Tv_Rc.top + (__s32)m_GridWidth;
		while ( (Tv_Rc.top < m_PntRcClient.bottom) && (Tv_Idx < m_GridCntY) )
		{
			//draw grid
			m_GdibScr->FillRect( &Tv_Rc, m_ClrGrid );
			//next
			Tv_Rc.top			=	Tv_Rc.top + (__s32)m_CelSizeY;
			Tv_Rc.bottom	=	Tv_Rc.top + (__s32)m_GridWidth;
			Tv_Idx ++;
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::LcDrawItems( void )
{
	// local -------------------
		__s32		Tv_WkX;
		__s32		Tv_WkY;
		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
		void*	Tv_PtrY;
		__u8*	Tv_PtrVal;
		__u8	Tv_Mask;
		St_GrRect	Tv_Rc;
		__u32	Tv_PosBitY;
		__u32	Tv_PosBitX;
		__u32	Tv_BytePos;
	// code --------------------
		if( m_IsMosaicUse )//new-120905~mosaic, New
		{
			//check exist value
			if ( NULL != m_PtrMosaicBuf )
			{
				Tv_PtrY	=	m_PtrMosaicBuf;
				Tv_WkY	=	m_PntRcClient.top;
				Tv_IdxY	=	0;
				Tv_PosBitY	=	0;
				if ( 0 != m_ValPitch )
				{
					//exist pitch
					while ( (Tv_WkY < m_PntRcClient.bottom) && (Tv_IdxY < m_GridCntY) )
					{
						Tv_PtrVal	=	(__u8*)Tv_PtrY;
						Tv_WkX	=	m_PntRcClient.left;
						Tv_IdxX	=	0;
						Tv_Mask	=	1;
						while ( (Tv_WkX < m_PntRcClient.right) && (Tv_IdxX < m_GridCntX) )
						{
							//check exist item
							if ( 0 != ( (*Tv_PtrVal) & Tv_Mask ) )
							{
								//draw mark top
								Tv_Rc.left			=	Tv_WkX + (__s32)(m_GridWidth - (m_GridWidth >> 1));
								Tv_Rc.top				=	Tv_WkY + (__s32)(m_GridWidth - (m_GridWidth >> 1));
								Tv_Rc.right			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)(m_GridWidth >> 1);
								Tv_Rc.bottom		=	Tv_Rc.top + (__s32)m_MarkWidth;
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
								//draw mark left
								Tv_Rc.right			=	Tv_Rc.left + (__s32)m_MarkWidth;
								Tv_Rc.bottom		=	Tv_WkY + (__s32)m_CelSizeY - (__s32)(m_GridWidth >> 1);
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
								//draw mark right
								Tv_Rc.left			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
								Tv_Rc.right			=	Tv_Rc.left + (__s32)m_GridWidth;
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
								//draw mark bottom
								Tv_Rc.left			=	Tv_WkX + (__s32)(m_GridWidth - (__s32)(m_GridWidth >> 1));
								Tv_Rc.top				=	Tv_WkY + (__s32)m_CelSizeY - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
								Tv_Rc.right			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)(m_GridWidth >> 1);
								Tv_Rc.bottom		=	Tv_Rc.top + (__s32)m_MarkWidth;
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
							}

							//next
							Tv_WkX	=	Tv_WkX + m_CelSizeX;
							Tv_IdxX ++;
							Tv_Mask	=	Tv_Mask << 1;
							if ( 0 == (0x07 & Tv_IdxX) )
							{
								Tv_PtrVal ++;
								Tv_Mask	=	1;
							}
						}
						//next
						Tv_WkY	=	Tv_WkY + m_CelSizeY;
						Tv_IdxY ++;
						Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
					}
				}
				else
				{
					//value pitch zero
					while ( (Tv_WkY < m_PntRcClient.bottom) && (Tv_IdxY < m_GridCntY) )
					{
						Tv_BytePos	=	Tv_PosBitY >> 3;
						Tv_PtrVal		=	(__u8*)( (__u32)m_PtrMosaicBuf + Tv_BytePos );
						Tv_PosBitX	=	Tv_PosBitY & 0x07;
						Tv_WkX	=	m_PntRcClient.left;
						Tv_IdxX	=	0;
						Tv_Mask	=	1 << Tv_PosBitX;
						while ( (Tv_WkX < m_PntRcClient.right) && (Tv_IdxX < m_GridCntX) )
						{
							//check exist item
							if ( 0 != ( (*Tv_PtrVal) & Tv_Mask ) )
							{
								//draw mark top
								Tv_Rc.left			=	Tv_WkX + (__s32)(m_GridWidth - (m_GridWidth >> 1));
								Tv_Rc.top				=	Tv_WkY + (__s32)(m_GridWidth - (m_GridWidth >> 1));
								Tv_Rc.right			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)(m_GridWidth >> 1);
								Tv_Rc.bottom		=	Tv_Rc.top + (__s32)m_MarkWidth;
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
								//draw mark left
								Tv_Rc.right			=	Tv_Rc.left + (__s32)m_MarkWidth;
								Tv_Rc.bottom		=	Tv_WkY + (__s32)m_CelSizeY - (__s32)(m_GridWidth >> 1);
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
								//draw mark right
								Tv_Rc.left			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
								Tv_Rc.right			=	Tv_Rc.left + (__s32)m_GridWidth;
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
								//draw mark bottom
								Tv_Rc.left			=	Tv_WkX + (__s32)(m_GridWidth - (__s32)(m_GridWidth >> 1));
								Tv_Rc.top				=	Tv_WkY + (__s32)m_CelSizeY - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
								Tv_Rc.right			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)(m_GridWidth >> 1);
								Tv_Rc.bottom		=	Tv_Rc.top + (__s32)m_MarkWidth;
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
							}

							//next
							Tv_WkX	=	Tv_WkX + m_CelSizeX;
							Tv_IdxX ++;
							Tv_Mask	=	Tv_Mask << 1;
							Tv_PosBitX ++;
							if ( 8 == Tv_PosBitX )
							{
								Tv_PosBitX	=	0;
								Tv_PtrVal ++;
								Tv_Mask	=	1;
							}
						}
						//next
						Tv_WkY	=	Tv_WkY + m_CelSizeY;
						Tv_IdxY ++;
						Tv_PosBitY	=	Tv_PosBitY + m_GridCntX;
					}
				}
			}
		}
		else//new-120905~mosaic, Old
		{
			//check exist value
			if ( NULL != m_PtrValBuf )
			{
				Tv_PtrY	=	m_PtrValBuf;
				Tv_WkY	=	m_PntRcClient.top;
				Tv_IdxY	=	0;
				Tv_PosBitY	=	0;
				if ( 0 != m_ValPitch )
				{
					//exist pitch
					while ( (Tv_WkY < m_PntRcClient.bottom) && (Tv_IdxY < m_GridCntY) )
					{
						Tv_PtrVal	=	(__u8*)Tv_PtrY;
						Tv_WkX	=	m_PntRcClient.left;
						Tv_IdxX	=	0;
						Tv_Mask	=	1;
						while ( (Tv_WkX < m_PntRcClient.right) && (Tv_IdxX < m_GridCntX) )
						{
							//check exist item
							if ( 0 != ( (*Tv_PtrVal) & Tv_Mask ) )
							{
								//draw mark top
								Tv_Rc.left			=	Tv_WkX + (__s32)(m_GridWidth - (m_GridWidth >> 1));
								Tv_Rc.top				=	Tv_WkY + (__s32)(m_GridWidth - (m_GridWidth >> 1));
								Tv_Rc.right			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)(m_GridWidth >> 1);
								Tv_Rc.bottom		=	Tv_Rc.top + (__s32)m_MarkWidth;
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
								//draw mark left
								Tv_Rc.right			=	Tv_Rc.left + (__s32)m_MarkWidth;
								Tv_Rc.bottom		=	Tv_WkY + (__s32)m_CelSizeY - (__s32)(m_GridWidth >> 1);
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
								//draw mark right
								Tv_Rc.left			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
								Tv_Rc.right			=	Tv_Rc.left + (__s32)m_GridWidth;
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
								//draw mark bottom
								Tv_Rc.left			=	Tv_WkX + (__s32)(m_GridWidth - (__s32)(m_GridWidth >> 1));
								Tv_Rc.top				=	Tv_WkY + (__s32)m_CelSizeY - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
								Tv_Rc.right			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)(m_GridWidth >> 1);
								Tv_Rc.bottom		=	Tv_Rc.top + (__s32)m_MarkWidth;
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
							}

							//next
							Tv_WkX	=	Tv_WkX + m_CelSizeX;
							Tv_IdxX ++;
							Tv_Mask	=	Tv_Mask << 1;
							if ( 0 == (0x07 & Tv_IdxX) )
							{
								Tv_PtrVal ++;
								Tv_Mask	=	1;
							}
						}
						//next
						Tv_WkY	=	Tv_WkY + m_CelSizeY;
						Tv_IdxY ++;
						Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
					}
				}
				else
				{
					//value pitch zero
					while ( (Tv_WkY < m_PntRcClient.bottom) && (Tv_IdxY < m_GridCntY) )
					{
						Tv_BytePos	=	Tv_PosBitY >> 3;
						Tv_PtrVal		=	(__u8*)( (__u32)m_PtrValBuf + Tv_BytePos );
						Tv_PosBitX	=	Tv_PosBitY & 0x07;
						Tv_WkX	=	m_PntRcClient.left;
						Tv_IdxX	=	0;
						Tv_Mask	=	1 << Tv_PosBitX;
						while ( (Tv_WkX < m_PntRcClient.right) && (Tv_IdxX < m_GridCntX) )
						{
							//check exist item
							if ( 0 != ( (*Tv_PtrVal) & Tv_Mask ) )
							{
								//draw mark top
								Tv_Rc.left			=	Tv_WkX + (__s32)(m_GridWidth - (m_GridWidth >> 1));
								Tv_Rc.top				=	Tv_WkY + (__s32)(m_GridWidth - (m_GridWidth >> 1));
								Tv_Rc.right			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)(m_GridWidth >> 1);
								Tv_Rc.bottom		=	Tv_Rc.top + (__s32)m_MarkWidth;
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
								//draw mark left
								Tv_Rc.right			=	Tv_Rc.left + (__s32)m_MarkWidth;
								Tv_Rc.bottom		=	Tv_WkY + (__s32)m_CelSizeY - (__s32)(m_GridWidth >> 1);
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
								//draw mark right
								Tv_Rc.left			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
								Tv_Rc.right			=	Tv_Rc.left + (__s32)m_GridWidth;
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
								//draw mark bottom
								Tv_Rc.left			=	Tv_WkX + (__s32)(m_GridWidth - (__s32)(m_GridWidth >> 1));
								Tv_Rc.top				=	Tv_WkY + (__s32)m_CelSizeY - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
								Tv_Rc.right			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)(m_GridWidth >> 1);
								Tv_Rc.bottom		=	Tv_Rc.top + (__s32)m_MarkWidth;
								m_GdibScr->FillRect( &Tv_Rc , m_ClrMark );
							}

							//next
							Tv_WkX	=	Tv_WkX + m_CelSizeX;
							Tv_IdxX ++;
							Tv_Mask	=	Tv_Mask << 1;
							Tv_PosBitX ++;
							if ( 8 == Tv_PosBitX )
							{
								Tv_PosBitX	=	0;
								Tv_PtrVal ++;
								Tv_Mask	=	1;
							}
						}
						//next
						Tv_WkY	=	Tv_WkY + m_CelSizeY;
						Tv_IdxY ++;
						Tv_PosBitY	=	Tv_PosBitY + m_GridCntX;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::LcDrawDisp( void )
{
	// local -------------------
		__s32		Tv_WkX;
		__s32		Tv_WkY;
		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
		void*	Tv_PtrY;
		__u8*	Tv_PtrVal;
		__u8	Tv_Mask;
		St_GrRect	Tv_Rc;
		__u32	Tv_PosBitY;
		__u32	Tv_PosBitX;
		__u32	Tv_BytePos;
	// code --------------------
		//check exist value
		if ( NULL != m_PtrDispBuf )
		{
			Tv_PtrY	=	m_PtrDispBuf;
			Tv_WkY	=	m_PntRcClient.top;
			Tv_IdxY	=	0;
			Tv_PosBitY	=	0;
			if ( 0 != m_ValPitch )
			{
				//exist pitch
				while ( (Tv_WkY < m_PntRcClient.bottom) && (Tv_IdxY < m_GridCntY) )
				{
					Tv_PtrVal	=	(__u8*)Tv_PtrY;
					Tv_WkX	=	m_PntRcClient.left;
					Tv_IdxX	=	0;
					Tv_Mask	=	1;
					while ( (Tv_WkX < m_PntRcClient.right) && (Tv_IdxX < m_GridCntX) )
					{
						//check exist item
						if ( 0 != ( (*Tv_PtrVal) & Tv_Mask ) )
						{
							//draw display 
							Tv_Rc.left		=	Tv_WkX + (__s32)(m_GridWidth - (m_GridWidth >> 1)) + (__s32)m_MarkWidth;
							Tv_Rc.top			=	Tv_WkY + (__s32)(m_GridWidth - (m_GridWidth >> 1)) + (__s32)m_MarkWidth;
							Tv_Rc.right		=	Tv_WkX + (__s32)m_CelSizeX - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
							Tv_Rc.bottom	=	Tv_WkY + (__s32)m_CelSizeY - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
							m_GdibScr->FillRect( &Tv_Rc , m_ClrDisp );
						}

						//next
						Tv_WkX	=	Tv_WkX + m_CelSizeX;
						Tv_IdxX ++;
						Tv_Mask	=	Tv_Mask << 1;
						if ( 0 == (0x07 & Tv_IdxX) )
						{
							Tv_PtrVal ++;
							Tv_Mask	=	1;
						}
					}
					//next
					Tv_WkY	=	Tv_WkY + m_CelSizeY;
					Tv_IdxY ++;
					Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
				}
			}
			else
			{
				//value pitch zero
				while ( (Tv_WkY < m_PntRcClient.bottom) && (Tv_IdxY < m_GridCntY) )
				{
					Tv_BytePos	=	Tv_PosBitY >> 3;
					Tv_PtrVal		=	(__u8*)( (__u32)m_PtrDispBuf + Tv_BytePos );
					Tv_PosBitX	=	Tv_PosBitY & 0x07;
					Tv_WkX	=	m_PntRcClient.left;
					Tv_IdxX	=	0;
					Tv_Mask	=	1 << Tv_PosBitX;
					while ( (Tv_WkX < m_PntRcClient.right) && (Tv_IdxX < m_GridCntX) )
					{
						//check exist item
						if ( 0 != ( (*Tv_PtrVal) & Tv_Mask ) )
						{
							//draw display 
							Tv_Rc.left		=	Tv_WkX + (__s32)(m_GridWidth - (m_GridWidth >> 1)) + (__s32)m_MarkWidth;
							Tv_Rc.top			=	Tv_WkY + (__s32)(m_GridWidth - (m_GridWidth >> 1)) + (__s32)m_MarkWidth;
							Tv_Rc.right		=	Tv_WkX + (__s32)m_CelSizeX - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
							Tv_Rc.bottom	=	Tv_WkY + (__s32)m_CelSizeY - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
							m_GdibScr->FillRect( &Tv_Rc , m_ClrDisp );
						}

						//next
						Tv_WkX	=	Tv_WkX + m_CelSizeX;
						Tv_IdxX ++;
						Tv_Mask	=	Tv_Mask << 1;
						Tv_PosBitX ++;
						if ( 8 == Tv_PosBitX )
						{
							Tv_PosBitX	=	0;
							Tv_PtrVal ++;
							Tv_Mask	=	1;
						}
					}
					//next
					Tv_WkY	=	Tv_WkY + m_CelSizeY;
					Tv_IdxY ++;
					Tv_PosBitY	=	Tv_PosBitY + m_GridCntX;
				}
			}

		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::LcDrawCus( void )
{
	// local -------------------
		__u32	Tv_StX;
		__u32	Tv_StY;
		__u32	Tv_EdX;
		__u32	Tv_EdY;
		__s32		Tv_WkX;
		__s32		Tv_WkY;
		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
		St_GrRect	Tv_Rc;
	// code --------------------
		//check cursor draw able
		if ( 0 != m_CusWidth )
		{
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

			Tv_WkY	=	m_PntRcClient.top + ( (__s32)m_CelSizeY * Tv_StY );
			Tv_IdxY	=	Tv_StY;
			while ( (Tv_WkY < m_PntRcClient.bottom) && (Tv_IdxY <= Tv_EdY) )
			{
				Tv_WkX	=	m_PntRcClient.left + ( (__s32)m_CelSizeX * Tv_StX ) ;
				Tv_IdxX	=	Tv_StX;
				while ( (Tv_WkX < m_PntRcClient.right) && (Tv_IdxX <= Tv_EdX) )
				{
					//draw mark top
					Tv_Rc.left			=	Tv_WkX + (__s32)m_MarkWidth + (__s32)(m_GridWidth - (m_GridWidth >> 1));
					Tv_Rc.top				=	Tv_WkY + (__s32)m_MarkWidth + (__s32)(m_GridWidth - (m_GridWidth >> 1));
					Tv_Rc.right			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
					Tv_Rc.bottom		=	Tv_Rc.top + (__s32)m_CusWidth;
					m_GdibScr->FillRect( &Tv_Rc , m_ClrCus );
					//draw mark left
					Tv_Rc.right			=	Tv_Rc.left + (__s32)m_CusWidth;
					Tv_Rc.bottom		=	Tv_WkY + (__s32)m_CelSizeY - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
					m_GdibScr->FillRect( &Tv_Rc , m_ClrCus );
					//draw mark right
					Tv_Rc.left			=	Tv_WkX + (__s32)m_CelSizeX -(__s32)m_CusWidth - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
					Tv_Rc.right			=	Tv_Rc.left + (__s32)m_CusWidth;
					m_GdibScr->FillRect( &Tv_Rc , m_ClrCus );
					//draw mark bottom
					Tv_Rc.left			=	Tv_WkX + (__s32)m_MarkWidth + (__s32)(m_GridWidth - (m_GridWidth >> 1));
					Tv_Rc.top				=	Tv_WkY + (__s32)m_CelSizeY -(__s32)m_CusWidth - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
					Tv_Rc.right			=	Tv_WkX + (__s32)m_CelSizeX - (__s32)m_MarkWidth - (__s32)(m_GridWidth >> 1);
					Tv_Rc.bottom		=	Tv_Rc.top + (__s32)m_CusWidth;
					m_GdibScr->FillRect( &Tv_Rc , m_ClrCus );

					//next
					Tv_WkX	=	Tv_WkX + m_CelSizeX;
					Tv_IdxX ++;
				}
				//next
				Tv_WkY	=	Tv_WkY + m_CelSizeY;
				Tv_IdxY ++;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::ProcBgSkinTblDraw( void )
{
		//inherited
		Cls_GrUiDoSkin::ProcBgSkinTblDraw();

		LcDrawGrid();
		LcDrawItems();
		LcDrawDisp();
		LcDrawCus();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBitGrid::CellXsizeGet( void )
{
		return	m_CelSizeX;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::CellXsizeSet( __u8 A_Size )
{
		if ( 0 != A_Size )
		{
			m_CelSizeX	=	A_Size;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBitGrid::CellYsizeGet( void )
{
		return	m_CelSizeY;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::CellYsizeSet( __u8 A_Size )
{
		if ( 0 != A_Size )
		{
			m_CelSizeY	=	A_Size;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBitGrid::GridXcountGet( void )
{
		return	m_GridCntX;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::GridXcountSet( __u8 A_Cnt )
{
		if ( 0 != A_Cnt )
		{
			m_GridCntX	=	A_Cnt;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBitGrid::GridYcountGet( void )
{
		return	m_GridCntY;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::GridYcountSet( __u8 A_Cnt )
{
		if ( 0 != A_Cnt )
		{
			m_GridCntY	=	A_Cnt;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBitGrid::GridWidthGet( void )
{
		return	m_GridWidth;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::GridWidthSet( __u8 A_Size )
{
		m_GridWidth	=	A_Size;
		ReqDraw();
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBitGrid::GridColorGet( void )
{
		return	m_ClrGrid;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::GridColorSet( __u32 A_Color )
{
		m_ClrGrid	=	A_Color;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBitGrid::MarkWidthGet( void )
{
		return	m_MarkWidth;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::MarkWidthSet( __u8 A_Width )
{
		if ( 0 != A_Width )
		{
			m_MarkWidth	=	A_Width;
			ReqDraw();
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBitGrid::MarkColorGet( void )
{
		return	m_ClrMark;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::MarkColorSet( __u32 A_Color )
{
		m_ClrMark	=	A_Color;
		ReqDraw();
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoBitGrid::CursorWidthGet( void )
{
		return	m_CusWidth;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::CursorWidthSet( __u8 A_Width )
{
		m_CusWidth	=	A_Width;
		ReqDraw();
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBitGrid::CursorColorGet( void )
{
		return	m_ClrCus;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::CursorColorSet( __u32 A_Color )
{
		m_ClrCus	=	A_Color;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::ValueBufferSet( void* A_PtrBuf, __u32 A_Pitch )
{
		m_PtrValBuf	=	A_PtrBuf;
		m_ValPitch	=	A_Pitch;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::DisplayBufferSet( void* A_PtrBuf )
{
		m_PtrDispBuf	=	A_PtrBuf;
		ReqDraw();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::CursorAreaGet( Ptr_GrByteRect A_PtrRtRc )
{
		A_PtrRtRc->left			=	m_CusStX;
		A_PtrRtRc->top			=	m_CusStY;
		A_PtrRtRc->right		=	m_CusEdX;
		A_PtrRtRc->bottom		=	m_CusEdY;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::LcReqDrawCus( void )
{
	// local -------------------
		__s32		Tv_StX;
		__s32		Tv_StY;
		__s32		Tv_EdX;
		__s32		Tv_EdY;
		St_GrRect	Tv_Rc;
	// code --------------------
		Tv_StX	=	(__s32)m_CusStX;
		Tv_StY	=	(__s32)m_CusStY;
		Tv_EdX	=	(__s32)m_CusEdX;
		Tv_EdY	=	(__s32)m_CusEdY;
		if ( Tv_StX > Tv_EdX )
		{
			Tv_StX	=	(__s32)m_CusEdX;
			Tv_EdX	=	(__s32)m_CusStX;
		}
		if ( Tv_StY > Tv_EdY )
		{
			Tv_StY	=	(__s32)m_CusEdY;
			Tv_EdY	=	(__s32)m_CusStY;
		}

		Tv_Rc.left		=	m_PntRcClient.left + ( (__s32)m_CelSizeX * Tv_StX );
		Tv_Rc.top			=	m_PntRcClient.top + ( (__s32)m_CelSizeY * Tv_StY );
		Tv_Rc.right		=	m_PntRcClient.left + ( (__s32)m_CelSizeX * (Tv_EdX + 1) );
		Tv_Rc.bottom	=	m_PntRcClient.top + ( (__s32)m_CelSizeY * (Tv_EdY + 1) );
		m_ObjConsole->PaintReq( &Tv_Rc );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::CursorPosSet( __u8 A_X, __u8 A_Y )
{
		//check able position
		if ( (A_X < m_GridCntX) && (A_Y < m_GridCntY) )
		{
			LcReqDrawCus();
			m_CusStX		=	A_X;
			m_CusStY		=	A_Y;
			m_CusEdX		=	A_X;
			m_CusEdY		=	A_Y;
			LcReqDrawCus();
			LcNotifyEvtCusChg();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::CursorTailSet( __u8 A_X, __u8 A_Y )
{
	// local -------------------
		__u8	Tv_X;
		__u8	Tv_Y;
	// code --------------------
		Tv_X		=	A_X;
		if ( Tv_X >= m_GridCntX )
		{
			Tv_X	=	m_GridCntX - 1;
		}
		Tv_Y		=	A_Y;
		if ( Tv_Y >= m_GridCntY )
		{
			Tv_Y	=	m_GridCntY - 1;
		}
		LcReqDrawCus();
		m_CusEdX		=	Tv_X;
		m_CusEdY		=	Tv_Y;
		LcReqDrawCus();
		LcNotifyEvtCusChg();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBitGrid::ProcEvtKeyDown( __u8 A_Key )
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
				if ( m_IsMosaicUse && m_IsKeyCusTail )
				{
					if ( 0 == m_CusEdY )
					{
						if ( 0 == (E_GrUiDoFocusMapUp & m_FocusMoveMap) )
						{
							CursorTailSet( m_CusEdX, m_GridCntY - 1 );
						}
						else
						{
							Tv_Result	=	FALSE;
						}
					}
					else
					{
						CursorTailSet( m_CusEdX, m_CusEdY - 1 );
					}
				}
				else
				{
					if ( 0 == m_CusStY )
					{
						if ( 0 == (E_GrUiDoFocusMapUp & m_FocusMoveMap) )
						{
							CursorPosSet( m_CusStX, m_GridCntY - 1 );
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
				}
				break;
			case	E_GrUiKeyDown:
				if ( m_IsMosaicUse && m_IsKeyCusTail )
				{
					if ( m_GridCntY <= (m_CusEdY + 1) )
					{
						if ( 0 == (E_GrUiDoFocusMapDown & m_FocusMoveMap) )
						{
							CursorTailSet( m_CusEdX, 0 );
						}
						else
						{
							Tv_Result	=	FALSE;
						}
					}
					else
					{
						CursorTailSet( m_CusEdX, m_CusEdY + 1 );
					}
				}
				else
				{
					if ( m_GridCntY <= (m_CusStY + 1) )
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
				}
				break;
			case	E_GrUiKeyLeft:
				if ( m_IsMosaicUse && m_IsKeyCusTail )
				{
					if ( 0 == m_CusEdX )
					{
						if ( 0 == (E_GrUiDoFocusMapLeft & m_FocusMoveMap) )
						{
							CursorTailSet( m_GridCntX - 1, m_CusEdY );
						}
						else
						{
							Tv_Result	=	FALSE;
						}
					}
					else
					{
						CursorTailSet( m_CusEdX -1 , m_CusEdY );
					}
				}
				else
				{
					if ( 0 == m_CusStX )
					{
						if ( 0 == (E_GrUiDoFocusMapLeft & m_FocusMoveMap) )
						{
							CursorPosSet( m_GridCntX - 1, m_CusStY );
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
				}
				break;
			case	E_GrUiKeyRight:
				if ( m_IsMosaicUse && m_IsKeyCusTail )
				{
					if ( m_GridCntX <= (m_CusEdX + 1) )
					{
						if ( 0 == (E_GrUiDoFocusMapRight & m_FocusMoveMap) )
						{
							CursorTailSet( 0, m_CusEdY );
						}
						else
						{
							Tv_Result	=	FALSE;
						}
					}
					else
					{
						CursorTailSet( m_CusEdX + 1, m_CusEdY );
					}
				}
				else
				{
					if ( m_GridCntX <= (m_CusStX + 1) )
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
				}
				break;
			default:
				Tv_Result	=	FALSE;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::ValueChageOnCus( void )
{
	// local -------------------
		__u32	Tv_StX;
		__u32	Tv_StY;
		__u32	Tv_EdX;
		__u32	Tv_EdY;
		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
		void*	Tv_PtrY;
		__u8*	Tv_PtrVal;
		__u8	Tv_Mask;
		__u32	Tv_PosBitY;
		__u32	Tv_PosBitX;
		__u32	Tv_PosByte;
	// code --------------------
		//check change able
		if( m_IsMosaicUse )//new-120905~mosaic
		{
			if ( NULL != m_PtrMosaicBuf )
			{
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

				Tv_IdxY	=	Tv_StY;
				if ( 0 != m_ValPitch )
				{
					Tv_PtrY	=	(void*)( (__u32)m_PtrMosaicBuf + ( Tv_StY * m_ValPitch ) );
					while ( Tv_IdxY <= Tv_EdY )
					{
						Tv_PtrVal	=	(__u8*)( (__u32)Tv_PtrY + (Tv_StX >> 3) );
						Tv_IdxX	=	Tv_StX;
						Tv_Mask		=	1 << (Tv_StX & 7);
						while ( Tv_IdxX <= Tv_EdX )
						{
							//change value
							*Tv_PtrVal	=	(*Tv_PtrVal) ^ Tv_Mask;
							//next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_IdxX ++;
							if ( 0 == (0x07 & Tv_IdxX) )
							{
								Tv_Mask	=	1;
								Tv_PtrVal ++;
							}
						}
						//next
						Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
						Tv_IdxY ++;
					}
				}
				else
				{
					//value pitch zero configuration
					Tv_PosBitY	=	(Tv_StY * m_GridCntX) + Tv_StX;
					while ( Tv_IdxY <= Tv_EdY )
					{
						Tv_PosByte	=	Tv_PosBitY >> 3;
						Tv_PtrVal	=	(__u8*)( (__u32)m_PtrMosaicBuf + Tv_PosByte );
						Tv_IdxX	=	Tv_StX;
						Tv_PosBitX	=	Tv_PosBitY & 0x07;
						Tv_Mask		=	1 << Tv_PosBitX;
						while ( Tv_IdxX <= Tv_EdX )
						{
							//change value
							*Tv_PtrVal	=	(*Tv_PtrVal) ^ Tv_Mask;
							//next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_IdxX ++;
							Tv_PosBitX ++;
							if ( 8 <= Tv_PosBitX )
							{
								Tv_Mask			=	1;
								Tv_PosBitX	=	0;
								Tv_PtrVal ++;
							}
						}
						//next
						Tv_PosBitY	=	Tv_PosBitY + m_GridCntX;
						Tv_IdxY ++;
					}
				}

				//redraw
				LcReqDrawCus();
				LcNotifyEvtValChg();
			}
		}//new-120905~mosaic New
		else
		{
			if ( NULL != m_PtrValBuf )
			{
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

				Tv_IdxY	=	Tv_StY;
				if ( 0 != m_ValPitch )
				{
					Tv_PtrY	=	(void*)( (__u32)m_PtrValBuf + ( Tv_StY * m_ValPitch ) );
					while ( Tv_IdxY <= Tv_EdY )
					{
						Tv_PtrVal	=	(__u8*)( (__u32)Tv_PtrY + (Tv_StX >> 3) );
						Tv_IdxX	=	Tv_StX;
						Tv_Mask		=	1 << (Tv_StX & 7);
						while ( Tv_IdxX <= Tv_EdX )
						{
							//change value
							*Tv_PtrVal	=	(*Tv_PtrVal) ^ Tv_Mask;
							//next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_IdxX ++;
							if ( 0 == (0x07 & Tv_IdxX) )
							{
								Tv_Mask	=	1;
								Tv_PtrVal ++;
							}
						}
						//next
						Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
						Tv_IdxY ++;
					}
				}
				else
				{
					//value pitch zero configuration
					Tv_PosBitY	=	(Tv_StY * m_GridCntX) + Tv_StX;
					while ( Tv_IdxY <= Tv_EdY )
					{
						Tv_PosByte	=	Tv_PosBitY >> 3;
						Tv_PtrVal	=	(__u8*)( (__u32)m_PtrValBuf + Tv_PosByte );
						Tv_IdxX	=	Tv_StX;
						Tv_PosBitX	=	Tv_PosBitY & 0x07;
						Tv_Mask		=	1 << Tv_PosBitX;
						while ( Tv_IdxX <= Tv_EdX )
						{
							//change value
							*Tv_PtrVal	=	(*Tv_PtrVal) ^ Tv_Mask;
							//next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_IdxX ++;
							Tv_PosBitX ++;
							if ( 8 <= Tv_PosBitX )
							{
								Tv_Mask			=	1;
								Tv_PosBitX	=	0;
								Tv_PtrVal ++;
							}
						}
						//next
						Tv_PosBitY	=	Tv_PosBitY + m_GridCntX;
						Tv_IdxY ++;
					}
				}

				//redraw
				LcReqDrawCus();
				LcNotifyEvtValChg();
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::ValueSetOnCus( void )
{
	// local -------------------
		__u32	Tv_StX;
		__u32	Tv_StY;
		__u32	Tv_EdX;
		__u32	Tv_EdY;
		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
		void*	Tv_PtrY;
		__u8*	Tv_PtrVal;
		__u8	Tv_Mask;
		__u32	Tv_PosBitY;
		__u32	Tv_PosBitX;
		__u32	Tv_PosByte;
	// code --------------------
		//check change able
		if ( m_IsMosaicUse )
		{
			if ( NULL != m_PtrMosaicBuf )
			{
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

				Tv_IdxY	=	Tv_StY;
				if ( 0 != m_ValPitch )
				{
					Tv_PtrY	=	(void*)( (__u32)m_PtrMosaicBuf + ( Tv_StY * m_ValPitch ) );
					while ( Tv_IdxY <= Tv_EdY )
					{
						Tv_PtrVal	=	(__u8*)( (__u32)Tv_PtrY + (Tv_StX >> 3) );
						Tv_IdxX	=	Tv_StX;
						Tv_Mask		=	1 << (Tv_StX & 7);
						while ( Tv_IdxX <= Tv_EdX )
						{
							//change value
							*Tv_PtrVal	=	(*Tv_PtrVal) | Tv_Mask;
							//next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_IdxX ++;
							if ( 0 == (0x07 & Tv_IdxX) )
							{
								Tv_Mask	=	1;
								Tv_PtrVal ++;
							}
						}
						//next
						Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
						Tv_IdxY ++;
					}
				}
				else
				{
					//pitch = zero
					Tv_PosBitY	=	(Tv_StY * m_GridCntX) + Tv_StX;
					while ( Tv_IdxY <= Tv_EdY )
					{
						Tv_PosByte	=	Tv_PosBitY >> 3;
						Tv_PtrVal	=	(__u8*)( (__u32)m_PtrMosaicBuf + Tv_PosByte );
						Tv_IdxX	=	Tv_StX;
						Tv_PosBitX	=	Tv_PosBitY & 0x07;
						Tv_Mask		=	1 << Tv_PosBitX;
						while ( Tv_IdxX <= Tv_EdX )
						{
							//change value
							*Tv_PtrVal	=	(*Tv_PtrVal) | Tv_Mask;
							//next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_IdxX ++;
							Tv_PosBitX ++;
							if ( 8 <= Tv_PosBitX )
							{
								Tv_Mask			=	1;
								Tv_PosBitX	=	0;
								Tv_PtrVal ++;
							}
						}
						//next
						Tv_PosBitY	=	Tv_PosBitY + m_GridCntX;
						Tv_IdxY ++;
					}
				}
				//redraw
				LcReqDrawCus();
				LcNotifyEvtValChg();
			}
		}
		else
		{
			if ( NULL != m_PtrValBuf )
			{
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

				Tv_IdxY	=	Tv_StY;
				if ( 0 != m_ValPitch )
				{
					Tv_PtrY	=	(void*)( (__u32)m_PtrValBuf + ( Tv_StY * m_ValPitch ) );
					while ( Tv_IdxY <= Tv_EdY )
					{
						Tv_PtrVal	=	(__u8*)( (__u32)Tv_PtrY + (Tv_StX >> 3) );
						Tv_IdxX	=	Tv_StX;
						Tv_Mask		=	1 << (Tv_StX & 7);
						while ( Tv_IdxX <= Tv_EdX )
						{
							//change value
							*Tv_PtrVal	=	(*Tv_PtrVal) | Tv_Mask;
							//next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_IdxX ++;
							if ( 0 == (0x07 & Tv_IdxX) )
							{
								Tv_Mask	=	1;
								Tv_PtrVal ++;
							}
						}
						//next
						Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
						Tv_IdxY ++;
					}
				}
				else
				{
					//pitch = zero
					Tv_PosBitY	=	(Tv_StY * m_GridCntX) + Tv_StX;
					while ( Tv_IdxY <= Tv_EdY )
					{
						Tv_PosByte	=	Tv_PosBitY >> 3;
						Tv_PtrVal	=	(__u8*)( (__u32)m_PtrValBuf + Tv_PosByte );
						Tv_IdxX	=	Tv_StX;
						Tv_PosBitX	=	Tv_PosBitY & 0x07;
						Tv_Mask		=	1 << Tv_PosBitX;
						while ( Tv_IdxX <= Tv_EdX )
						{
							//change value
							*Tv_PtrVal	=	(*Tv_PtrVal) | Tv_Mask;
							//next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_IdxX ++;
							Tv_PosBitX ++;
							if ( 8 <= Tv_PosBitX )
							{
								Tv_Mask			=	1;
								Tv_PosBitX	=	0;
								Tv_PtrVal ++;
							}
						}
						//next
						Tv_PosBitY	=	Tv_PosBitY + m_GridCntX;
						Tv_IdxY ++;
					}
				}
				//redraw
				LcReqDrawCus();
				LcNotifyEvtValChg();
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::ValueResetOnCus( void )
{
	// local -------------------
		__u32	Tv_StX;
		__u32	Tv_StY;
		__u32	Tv_EdX;
		__u32	Tv_EdY;
		__u32	Tv_IdxX;
		__u32	Tv_IdxY;
		void*	Tv_PtrY;
		__u8*	Tv_PtrVal;
		__u8	Tv_Mask;
		__u32	Tv_PosBitY;
		__u32	Tv_PosBitX;
		__u32	Tv_PosByte;
	// code --------------------
		//check change able
		if ( m_IsMosaicUse )
		{
			if ( NULL != m_PtrMosaicBuf )
			{
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

				Tv_IdxY	=	Tv_StY;
				if ( 0 != m_ValPitch )
				{
					Tv_PtrY	=	(void*)( (__u32)m_PtrMosaicBuf + ( Tv_StY * m_ValPitch ) );
					while ( Tv_IdxY <= Tv_EdY )
					{
						Tv_PtrVal	=	(__u8*)( (__u32)Tv_PtrY + (Tv_StX >> 3) );
						Tv_IdxX	=	Tv_StX;
						Tv_Mask		=	1 << (Tv_StX & 7);
						while ( Tv_IdxX <= Tv_EdX )
						{
							//change value
							*Tv_PtrVal	=	(*Tv_PtrVal) & ~Tv_Mask;
							//next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_IdxX ++;
							if ( 0 == (0x07 & Tv_IdxX) )
							{
								Tv_Mask	=	1;
								Tv_PtrVal ++;
							}
						}
						//next
						Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
						Tv_IdxY ++;
					}
				}
				else
				{
					//pitch = zero
					Tv_PosBitY	=	(Tv_StY * m_GridCntX) + Tv_StX;
					while ( Tv_IdxY <= Tv_EdY )
					{
						Tv_PosByte	=	Tv_PosBitY >> 3;
						Tv_PtrVal	=	(__u8*)( (__u32)m_PtrMosaicBuf + Tv_PosByte );
						Tv_IdxX	=	Tv_StX;
						Tv_PosBitX	=	Tv_PosBitY & 0x07;
						Tv_Mask		=	1 << Tv_PosBitX;
						while ( Tv_IdxX <= Tv_EdX )
						{
							//change value
							*Tv_PtrVal	=	(*Tv_PtrVal) | Tv_Mask;
							//next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_IdxX ++;
							Tv_PosBitX ++;
							if ( 8 <= Tv_PosBitX )
							{
								Tv_Mask			=	1;
								Tv_PosBitX	=	0;
								Tv_PtrVal ++;
							}
						}
						//next
						Tv_PosBitY	=	Tv_PosBitY + m_GridCntX;
						Tv_IdxY ++;
					}
				}

				//redraw
				LcReqDrawCus();
				LcNotifyEvtValChg();
			}
		}
		else
		{
			if ( NULL != m_PtrValBuf )
			{
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

				Tv_IdxY	=	Tv_StY;
				if ( 0 != m_ValPitch )
				{
					Tv_PtrY	=	(void*)( (__u32)m_PtrValBuf + ( Tv_StY * m_ValPitch ) );
					while ( Tv_IdxY <= Tv_EdY )
					{
						Tv_PtrVal	=	(__u8*)( (__u32)Tv_PtrY + (Tv_StX >> 3) );
						Tv_IdxX	=	Tv_StX;
						Tv_Mask		=	1 << (Tv_StX & 7);
						while ( Tv_IdxX <= Tv_EdX )
						{
							//change value
							*Tv_PtrVal	=	(*Tv_PtrVal) & ~Tv_Mask;
							//next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_IdxX ++;
							if ( 0 == (0x07 & Tv_IdxX) )
							{
								Tv_Mask	=	1;
								Tv_PtrVal ++;
							}
						}
						//next
						Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
						Tv_IdxY ++;
					}
				}
				else
				{
					//pitch = zero
					Tv_PosBitY	=	(Tv_StY * m_GridCntX) + Tv_StX;
					while ( Tv_IdxY <= Tv_EdY )
					{
						Tv_PosByte	=	Tv_PosBitY >> 3;
						Tv_PtrVal	=	(__u8*)( (__u32)m_PtrValBuf + Tv_PosByte );
						Tv_IdxX	=	Tv_StX;
						Tv_PosBitX	=	Tv_PosBitY & 0x07;
						Tv_Mask		=	1 << Tv_PosBitX;
						while ( Tv_IdxX <= Tv_EdX )
						{
							//change value
							*Tv_PtrVal	=	(*Tv_PtrVal) | Tv_Mask;
							//next
							Tv_Mask	=	Tv_Mask << 1;
							Tv_IdxX ++;
							Tv_PosBitX ++;
							if ( 8 <= Tv_PosBitX )
							{
								Tv_Mask			=	1;
								Tv_PosBitX	=	0;
								Tv_PtrVal ++;
							}
						}
						//next
						Tv_PosBitY	=	Tv_PosBitY + m_GridCntX;
						Tv_IdxY ++;
					}
				}

				//redraw
				LcReqDrawCus();
				LcNotifyEvtValChg();
			}
		}		
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::ValueSetAll( void )
{
	// local -------------------
		__u32	Tv_BitCnt;
		__u32	Tv_ByteCnt;
		__u8*	Tv_PtrX;
		void*	Tv_PtrY;
		__u32	Tv_WkX;
		__u32	Tv_WkY;
		__u8	Tv_BitMask;
	// code --------------------
		if ( m_IsMosaicUse )
		{
			if ( NULL != m_PtrMosaicBuf )
			{
				if ( 0 != m_ValPitch )
				{
					Tv_PtrY			=	m_PtrMosaicBuf;
					Tv_ByteCnt	=	m_GridCntX >> 3;
					Tv_BitCnt		=	m_GridCntX & 0x07;
					Tv_BitMask	=	(1 << Tv_BitCnt) - 1;
					for ( Tv_WkY=0;Tv_WkY < m_GridCntY;Tv_WkY++ )
					{
						Tv_PtrX	=	(__u8*)Tv_PtrY;
						if ( 0 != Tv_ByteCnt )
						{
							for ( Tv_WkX=0;Tv_WkX < Tv_ByteCnt;Tv_WkX++ )
							{
								*Tv_PtrX	=	0xFF;
								Tv_PtrX ++;
							}
						}
						if ( 0 != Tv_BitCnt )
						{
							*Tv_PtrX	=	Tv_BitMask;
						}
						//next
						Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
					}
				}
				else
				{
					Tv_BitCnt		=	m_GridCntX * m_GridCntY;
					Tv_ByteCnt	=	Tv_BitCnt >> 3;
					Tv_BitCnt		=	Tv_BitCnt & 0x07;
					Tv_BitMask	=	(1 << Tv_BitCnt) - 1;
					Tv_PtrX			=	(__u8*)m_PtrMosaicBuf;
					if ( 0 != Tv_ByteCnt )
					{
						for ( Tv_WkX=0;Tv_WkX < Tv_ByteCnt;Tv_WkX++ )
						{
							*Tv_PtrX	=	0xFF;
							Tv_PtrX ++;
						}
					}
					//last bit
					if ( 0 != Tv_BitCnt )
					{
						*Tv_PtrX	=	Tv_BitMask;
					}
				}
				ReqDraw();
				LcNotifyEvtValChg();
			}
		}
		else
		{
			if ( NULL != m_PtrValBuf )
			{
				if ( 0 != m_ValPitch )
				{
					Tv_PtrY			=	m_PtrValBuf;
					Tv_ByteCnt	=	m_GridCntX >> 3;
					Tv_BitCnt		=	m_GridCntX & 0x07;
					Tv_BitMask	=	(1 << Tv_BitCnt) - 1;
					for ( Tv_WkY=0;Tv_WkY < m_GridCntY;Tv_WkY++ )
					{
						Tv_PtrX	=	(__u8*)Tv_PtrY;
						if ( 0 != Tv_ByteCnt )
						{
							for ( Tv_WkX=0;Tv_WkX < Tv_ByteCnt;Tv_WkX++ )
							{
								*Tv_PtrX	=	0xFF;
								Tv_PtrX ++;
							}
						}
						if ( 0 != Tv_BitCnt )
						{
							*Tv_PtrX	=	Tv_BitMask;
						}
						//next
						Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
					}
				}
				else
				{
					Tv_BitCnt		=	m_GridCntX * m_GridCntY;
					Tv_ByteCnt	=	Tv_BitCnt >> 3;
					Tv_BitCnt		=	Tv_BitCnt & 0x07;
					Tv_BitMask	=	(1 << Tv_BitCnt) - 1;
					Tv_PtrX			=	(__u8*)m_PtrValBuf;
					if ( 0 != Tv_ByteCnt )
					{
						for ( Tv_WkX=0;Tv_WkX < Tv_ByteCnt;Tv_WkX++ )
						{
							*Tv_PtrX	=	0xFF;
							Tv_PtrX ++;
						}
					}
					//last bit
					if ( 0 != Tv_BitCnt )
					{
						*Tv_PtrX	=	Tv_BitMask;
					}
				}
				ReqDraw();
				LcNotifyEvtValChg();
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::ValueResetAll( void )
{
	// local -------------------
		__u32	Tv_BitCnt;
		__u32	Tv_ByteCnt;
		__u8*	Tv_PtrX;
		void*	Tv_PtrY;
		__u32	Tv_WkX;
		__u32	Tv_WkY;
	// code --------------------
		if ( m_IsMosaicUse )
		{
			if ( NULL != m_PtrMosaicBuf )
			{
				if ( 0 != m_ValPitch )
				{
					Tv_PtrY			=	m_PtrMosaicBuf;
					Tv_ByteCnt	=	m_GridCntX >> 3;
					Tv_BitCnt		=	m_GridCntX & 0x07;
					for ( Tv_WkY=0;Tv_WkY < m_GridCntY;Tv_WkY++ )
					{
						Tv_PtrX	=	(__u8*)Tv_PtrY;
						if ( 0 != Tv_ByteCnt )
						{
							for ( Tv_WkX=0;Tv_WkX < Tv_ByteCnt;Tv_WkX++ )
							{
								*Tv_PtrX	=	0;
								Tv_PtrX ++;
							}
						}
						if ( 0 != Tv_BitCnt )
						{
							*Tv_PtrX	=	0;
						}
						//next
						Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
					}
				}
				else
				{
					Tv_BitCnt		=	m_GridCntX * m_GridCntY;
					Tv_ByteCnt	=	Tv_BitCnt >> 3;
					Tv_BitCnt		=	Tv_BitCnt & 0x07;
					Tv_PtrX			=	(__u8*)m_PtrMosaicBuf;
					if ( 0 != Tv_ByteCnt )
					{
						for ( Tv_WkX=0;Tv_WkX < Tv_ByteCnt;Tv_WkX++ )
						{
							*Tv_PtrX	=	0;
							Tv_PtrX ++;
						}
					}
					//last bit
					if ( 0 != Tv_BitCnt )
					{
						*Tv_PtrX	=	0;
					}
				}
				ReqDraw();
				LcNotifyEvtValChg();
			}
		}
		else
		{
			if ( NULL != m_PtrValBuf )
			{
				if ( 0 != m_ValPitch )
				{
					Tv_PtrY			=	m_PtrValBuf;
					Tv_ByteCnt	=	m_GridCntX >> 3;
					Tv_BitCnt		=	m_GridCntX & 0x07;
					for ( Tv_WkY=0;Tv_WkY < m_GridCntY;Tv_WkY++ )
					{
						Tv_PtrX	=	(__u8*)Tv_PtrY;
						if ( 0 != Tv_ByteCnt )
						{
							for ( Tv_WkX=0;Tv_WkX < Tv_ByteCnt;Tv_WkX++ )
							{
								*Tv_PtrX	=	0;
								Tv_PtrX ++;
							}
						}
						if ( 0 != Tv_BitCnt )
						{
							*Tv_PtrX	=	0;
						}
						//next
						Tv_PtrY	=	(void*)( (__u32)Tv_PtrY + m_ValPitch );
					}
				}
				else
				{
					Tv_BitCnt		=	m_GridCntX * m_GridCntY;
					Tv_ByteCnt	=	Tv_BitCnt >> 3;
					Tv_BitCnt		=	Tv_BitCnt & 0x07;
					Tv_PtrX			=	(__u8*)m_PtrValBuf;
					if ( 0 != Tv_ByteCnt )
					{
						for ( Tv_WkX=0;Tv_WkX < Tv_ByteCnt;Tv_WkX++ )
						{
							*Tv_PtrX	=	0;
							Tv_PtrX ++;
						}
					}
					//last bit
					if ( 0 != Tv_BitCnt )
					{
						*Tv_PtrX	=	0;
					}
				}
				ReqDraw();
				LcNotifyEvtValChg();
			}
		}
	
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBitGrid::ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
			Tv_X	=	(__u8)((A_X - m_PntRcClient.left) / m_CelSizeX);
			Tv_Y	=	(__u8)((A_Y - m_PntRcClient.top) / m_CelSizeY );
			//check able position
			if ( (Tv_X < m_GridCntX) && (Tv_Y < m_GridCntY) )
			{
				CursorPosSet( Tv_X, Tv_Y );
			}

			// release key cursor mode
			m_IsKeyCusTail	=	FALSE;

			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBitGrid::ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
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
				Tv_X	=	(__u8)((A_X - m_PntRcClient.left) / m_CelSizeX);
				Tv_Y	=	(__u8)((A_Y - m_PntRcClient.top) / m_CelSizeY );
				//check able position
				if ( Tv_X >= m_GridCntX )
				{
					Tv_X	=	m_GridCntX - 1;
				}
				if ( Tv_Y >= m_GridCntY )
				{
					Tv_Y	=	m_GridCntY - 1;
				}
				//set cursor tail
				CursorTailSet( Tv_X, Tv_Y );
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBitGrid::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_X;
		__u8	Tv_Y;
	// code --------------------
		Tv_Result	=	Cls_GrUiDoSkin::ProcEvtMseLbtnUp( A_X, A_Y, A_BtnMap );

		if ( !Tv_Result )
		{
			if ( m_IsMosaicUse )
			{
				ValueResetAll();
			}
			//calculate position
			Tv_X	=	(__u8)((A_X - m_PntRcClient.left) / m_CelSizeX);
			Tv_Y	=	(__u8)((A_Y - m_PntRcClient.top) / m_CelSizeY );
			//check able position
			if ( Tv_X >= m_GridCntX )
			{
				Tv_X	=	m_GridCntX - 1;
			}
			if ( Tv_Y >= m_GridCntY )
			{
				Tv_Y	=	m_GridCntY - 1;
			}
			//set cursor tail
			CursorTailSet( Tv_X, Tv_Y );
			//change value
			ValueChageOnCus();
			//change cursor position
			CursorPosSet( m_CusStX, m_CusStY );

			// release key cursor mode
			m_IsKeyCusTail	=	FALSE;

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::ProcEvtMseLeave( void )
{
		Cls_GrUiDoSkin::ProcEvtMseLeave();
		//check different cursor startand and end
		if ( (m_CusStX != m_CusEdX) || (m_CusStY != m_CusEdY) )
		{
			CursorPosSet( m_CusStX, m_CusStY );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::LcNotifyEvtCusChg( void )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		if ( V_GrUiMngObj->IsUserEventOn() )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdBitGridCusChg;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::LcNotifyEvtValChg( void )
{
	// local -------------------
		St_GrUiMsgCtrl	Tv_Msg;
	// code --------------------
		if ( V_GrUiMngObj->IsUserEventOn() )
		{
			Tv_Msg.Hd.Cmd			=	E_GrUiMsgCmdBitGridValChg;
			Tv_Msg.Hd.ConIdx	=	E_GrUiConIdxFocused;
			Tv_Msg.Hd.WinId		=	((Cls_GrUiDoWin*)m_ObjWin)->WinIdGet();
			Tv_Msg.Hd.ObjId		=	E_GrUiObjIdNone;
			Tv_Msg.Obj				=	(void*)this;
			UiMsgPost( (Ptr_GrUiMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrUiDoBitGrid::DisplayColorGet( void )
{
		return	m_ClrDisp;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::DisplayColorSet( __u32 A_Color )
{
		m_ClrDisp	=	A_Color;
		ReqDraw();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBitGrid::ProcEvtKeyUp( __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;
		switch( A_Key )
		{
			case	E_GrUiKeyCh1:
				ValueSetAll();
				break;
			case	E_GrUiKeyCh2:
				ValueResetAll();
				break;
			case	E_GrUiKeyEnter:
				if ( m_IsMosaicUse )
				{
					if ( m_IsKeyCusTail )
					{
						ValueResetAll();
						// update value
						ValueChageOnCus();
						// change key cursor
						m_IsKeyCusTail	=	FALSE;
					}
					else
					{
						m_IsKeyCusTail	=	TRUE;
					}
				}
				else
				{
					ValueChageOnCus();
				}
				break;
			default:
				Tv_Result	=	FALSE;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::IsMosaicModeSet( BOOL8 A_Use )
{
	// local -------------------
	// code --------------------
		m_IsMosaicUse = A_Use;
}//new-120905~mosaic
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoBitGrid::IsMosaicModeGet()
{
	// local -------------------
	// code --------------------
		return	m_IsMosaicUse;
}//new-120905~mosaic
//--------------------------------------------------------------------
void	Cls_GrUiDoBitGrid::MosaicBufferSet( void* A_PtrBuf, __u32 A_Pitch )
{
		m_PtrMosaicBuf	=	A_PtrBuf;
		m_ValPitch	=	A_Pitch;
		ReqDraw();
}//new-120905~mosaic
//--------------------------------------------------------------------

