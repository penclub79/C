/*
	grid value

*/
#include <Win\MfcDock\GrMfcDockGridVal.h>
#include <Win\MfcDock\GrMfcDockBase.h>

#include <afxcontrolbars.h>
#include <afxvisualmanager.h>

#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <GrDrawTool.h>


//--------------------------------------------------------------------
// local constant


//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var

BOOL8	V_GrMfcDockGridValIsInit			= FALSE;


//====================================================================
//--------------------------------------------------------------------
Cls_GrMfcDockGridVal::Cls_GrMfcDockGridVal(  ):
Cls_GrMfcDockCtrlBase()
{
	// local -------------------
		WNDCLASS Tv_Wcls; 
		//int		Tv_ColIdx;
		//int		Tv_ItmIdx;
	// code --------------------
		// init
		m_IsCanWidthChg	=	TRUE;
		m_IsDrawHead		=	FALSE;
		m_ColumCnt			=	0;
		m_ColumWidthSum	=	0;
		GrDumyZeroMem(m_ColumCtrl, sizeof(m_ColumCtrl));

		GrDumyZeroMem(&m_UiStat, sizeof(m_UiStat));

		m_IsDrawAble		=	TRUE;
		m_ItemMaxCnt		=	0;
		m_ItemCnt				=	0;
		m_PtrItems			=	NULL;

		// register window class
		if ( !V_GrMfcDockGridValIsInit )
		{
			GrDumyZeroMem( &Tv_Wcls, sizeof(Tv_Wcls) );
			Tv_Wcls.style					= CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;  // redraw if size changes 
			Tv_Wcls.lpfnWndProc		= ::DefWindowProc;     // points to window procedure 
			Tv_Wcls.cbClsExtra		= 0;                // no extra class memory 
			Tv_Wcls.cbWndExtra		= 0;                // no extra window memory 
			Tv_Wcls.hInstance			= AfxGetResourceHandle();         // handle to instance 
			Tv_Wcls.hIcon					= NULL;              // predefined app. icon 
			Tv_Wcls.hCursor				= AfxGetApp()->LoadStandardCursor(IDC_ARROW);     // predefined arrow 
			Tv_Wcls.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );                  // white background brush 
			Tv_Wcls.lpszMenuName	= NULL;    // name of menu resource 
			Tv_Wcls.lpszClassName = L"GrGridVal";  // name of window class 

			// Register the window class. 
			if ( AfxRegisterClass( &Tv_Wcls ) )
			{
				V_GrMfcDockGridValIsInit	=	TRUE;
			}
		}

		// pen
		m_PenHdSepLt	=	CreatePen(PS_SOLID, 1, afxGlobalData.clrBtnHilite);
		m_PenHdSepRt	=	CreatePen(PS_SOLID, 1, afxGlobalData.clrBtnDkShadow);
		m_PenCtSepLt	=	CreatePen(PS_SOLID, 1, afxGlobalData.clrActiveCaption);
		m_PenCtSepRt	=	CreatePen(PS_SOLID, 1, afxGlobalData.clrActiveCaption);
		m_PenCtUline	=	CreatePen(PS_SOLID, 1, afxGlobalData.clrActiveCaption);

}
//--------------------------------------------------------------------
Cls_GrMfcDockGridVal::~Cls_GrMfcDockGridVal( void )
{
	// local -------------------
	// code --------------------
		ClearAll();

		DeleteObject((HGDIOBJ)m_PenCtSepLt);
		DeleteObject((HGDIOBJ)m_PenCtSepRt);
		DeleteObject((HGDIOBJ)m_PenCtUline);
		DeleteObject((HGDIOBJ)m_PenHdSepLt);
		DeleteObject((HGDIOBJ)m_PenHdSepRt);

		if(NULL != m_PtrItems)
		{
			free(m_PtrItems);
			m_PtrItems	=	NULL;
		}

		if(m_IsCanWidthChg)
		{
			CtrlRgtyValWrite(E_GrMfcDockGridValUiStatValId, &m_UiStat, sizeof(m_UiStat));
		}
}
//--------------------------------------------------------------------
BEGIN_MESSAGE_MAP(Cls_GrMfcDockGridVal, Cls_GrMfcDockCtrlBase)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()
//--------------------------------------------------------------------
BOOL	Cls_GrMfcDockGridVal::Create(RECT& A_Rect, CWnd* A_WndParent, UINT A_Id, __u32 A_ItemMaxCnt)
{
	// local -------------------
		BOOL	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( V_GrMfcDockGridValIsInit && (NULL != A_WndParent) && (NULL == m_PtrItems) )
		{
			if(Cls_GrMfcDockCtrlBase::Create(L"GrGridVal", NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL, A_Rect, A_WndParent, A_Id))
			{
				if((0 < A_ItemMaxCnt) && (E_GrMfcDockGridValItemAbleMaxCnt >= A_ItemMaxCnt))
				{
					m_PtrItems	=	(Ptr_GrMfcDockGridValItemCtrl)malloc(sizeof(St_GrMfcDockGridValItemCtrl)* A_ItemMaxCnt);
					if(NULL != m_PtrItems)
					{
						m_ItemHeight	=	afxGlobalData.GetTextHeight(TRUE) + (E_GrMfcDockGridValItemVtMargin * 2);
						m_ItemMaxCnt	=	A_ItemMaxCnt;

						// load ui status
						if(m_IsCanWidthChg)
						{
							CtrlRgtyValRead(E_GrMfcDockGridValUiStatValId, &m_UiStat, sizeof(m_UiStat));
						}
						
						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void Cls_GrMfcDockGridVal::OnPaint( )
{
	// local -------------------
		CDC*	Tv_Dc;
		PAINTSTRUCT	Tv_Ps;
		RECT		Tv_RcCli;
	// code --------------------
		Tv_Dc	=	BeginPaint( &Tv_Ps );

		// backup orignal object
		m_FontOld	=	Tv_Dc->SelectObject( afxGlobalData.fontRegular.m_hObject );

		// setup common enviroment
		Tv_Dc->SetBkMode( TRANSPARENT );

		// build dimemsion
		GetClientRect( &Tv_RcCli );
		LcDimensionCalc( &Tv_RcCli );
		
		if (0 != m_HeaderHeight)
		{
			LcDrawHeaders(Tv_Dc);
		}

		LcDrawItems(Tv_Dc);

		// restore orignal object
		Tv_Dc->SelectObject( m_FontOld );


		EndPaint( &Tv_Ps );
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::LcDimensionCalc( RECT* A_PtrRc )
{
	// local -------------------
		int		Tv_TxtHeight;
	// code --------------------
		m_RcItemArea.left			=	A_PtrRc->left;
		m_RcItemArea.top			=	A_PtrRc->top;
		m_RcItemArea.right		=	A_PtrRc->right;
		m_RcItemArea.bottom		=	A_PtrRc->bottom;
		if ( m_RcItemArea.top >= m_RcItemArea.bottom )
		{
			m_RcItemArea.bottom	=	A_PtrRc->bottom;
		}

		m_ItemViewAble	=	0;
		m_HeaderHeight	=	0;
		m_ItemViewCnt		=	0;

		Tv_TxtHeight		=	afxGlobalData.GetTextHeight(TRUE);
		
		if (0 != Tv_TxtHeight)
		{
			if (m_IsDrawHead)
			{
				m_HeaderHeight				=	m_ItemHeight;
			}
		}

		// header rect
		m_RcHeadArea.left				=	m_RcItemArea.left + 1;
		m_RcHeadArea.top				=	m_RcItemArea.top + 1;
		m_RcHeadArea.right			=	m_RcItemArea.right - 1;
		m_RcHeadArea.bottom			=	m_RcItemArea.top + m_HeaderHeight;

		// item contents rect
		m_RcItemCont.left				=	m_RcHeadArea.left;
		m_RcItemCont.top				=	m_RcHeadArea.bottom;
		m_RcItemCont.right			=	m_RcHeadArea.right;
		m_RcItemCont.bottom			=	m_RcItemArea.bottom - 1;

		if(0 != m_ItemHeight)
		{
			m_ItemViewAble				=	(m_RcItemCont.bottom - m_RcItemCont.top) / m_ItemHeight;
			m_ItemViewCnt					=	m_ItemViewAble;
			if((m_ScrlPosVt + m_ItemViewAble) > m_ItemCnt)
			{
				m_ItemViewCnt		=	0;
				if(m_ScrlPosVt < m_ItemCnt)
				{
					m_ItemViewCnt		=	m_ItemCnt - m_ScrlPosVt;
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::LcDrawHeaders(CDC* A_Dc)
{
	// local -------------------
		RECT	Tv_RcItem;
		int		Tv_ColIdx;
		int		Tv_ItmPrgsX;
		int		Tv_HdAreaWidth;
		/*
		RECT	Tv_RcClip;
		CRgn	Tv_RgnClip;
		BOOL8	Tv_IsClip;
		*/
		TRIVERTEX	Tv_Tvtx[2];
		GRADIENT_RECT	Tv_RcGrdt;
	// code --------------------
		// check colum count
		if (0 != m_ColumCnt)
		{
			// check able
			if (m_RcHeadArea.right > m_RcHeadArea.left)
			{
				// draw background
				Tv_Tvtx[0].x			=	m_RcHeadArea.left;
				Tv_Tvtx[0].y			=	m_RcHeadArea.top;
				Tv_Tvtx[0].Red		=	((afxGlobalData.clrBtnLight >> 16) & 0xFF) << 8;
				Tv_Tvtx[0].Green	=	((afxGlobalData.clrBtnLight >> 8) & 0xFF) << 8;
				Tv_Tvtx[0].Blue		=	((afxGlobalData.clrBtnLight >> 0) & 0xFF) << 8;
				Tv_Tvtx[0].Alpha	=	0;

				Tv_Tvtx[1].x			=	m_RcHeadArea.right;
				Tv_Tvtx[1].y			=	m_RcHeadArea.bottom;
				Tv_Tvtx[1].Red		=	((afxGlobalData.clrBtnShadow >> 16) & 0xFF) << 8;
				Tv_Tvtx[1].Green	=	((afxGlobalData.clrBtnShadow >> 8) & 0xFF) << 8;
				Tv_Tvtx[1].Blue		=	((afxGlobalData.clrBtnShadow >> 0) & 0xFF) << 8;
				Tv_Tvtx[1].Alpha	=	0;

				Tv_RcGrdt.UpperLeft		=	0;
				Tv_RcGrdt.LowerRight	=	1;
				A_Dc->GradientFill(Tv_Tvtx, 2, &Tv_RcGrdt, 1, GRADIENT_FILL_RECT_V);

				// init
				Tv_ItmPrgsX			=	0;
				Tv_ColIdx				=	0;
				Tv_HdAreaWidth	=	m_RcHeadArea.right - m_RcHeadArea.left;
				while (Tv_ColIdx < m_ColumCnt)
				{

					// check able
					if ((m_ScrlPosHz < (Tv_ItmPrgsX + (int)m_UiStat.WidthTbl[Tv_ColIdx])) && 
						((m_ScrlPosHz + Tv_HdAreaWidth) > Tv_ItmPrgsX) )
					{
						// init
						//Tv_IsClip	=	FALSE;
						// calculate rect
						Tv_RcItem.left		=	m_RcHeadArea.left + Tv_ItmPrgsX - m_ScrlPosHz;
						Tv_RcItem.top			=	m_RcHeadArea.top;
						Tv_RcItem.right		=	Tv_RcItem.left + (int)m_UiStat.WidthTbl[Tv_ColIdx];
						Tv_RcItem.bottom	=	m_RcHeadArea.bottom;
						// calculate clip
						/*
						Tv_RcClip	=	Tv_RcItem;
						if (Tv_RcClip.left < m_RcHeadArea.left )
						{
							Tv_RcClip.left		=	m_RcHeadArea.left;
							Tv_IsClip	=	TRUE;
						}
						if (Tv_RcClip.right > m_RcHeadArea.right)
						{
							Tv_RcClip.right		=	m_RcHeadArea.right;
							Tv_IsClip	=	TRUE;
						}

						Tv_IsClip		=	FALSE;

						// do clip
						if (Tv_IsClip)
						{
							if (Tv_RgnClip.CreateRectRgn(Tv_RcClip.left, Tv_RcClip.top, Tv_RcClip.right, Tv_RcClip.bottom))
							{
								A_Dc->SelectClipRgn(&Tv_RgnClip);
							}
							else
							{
								Tv_IsClip	=	FALSE;
							}
						}
						*/

						// draw header
						LcDrawHeaderOne(A_Dc, &Tv_RcItem, Tv_ColIdx);
						
						// release clip
						/*
						if (Tv_IsClip)
						{
							A_Dc->SelectClipRgn(NULL);
							Tv_RgnClip.DeleteObject();
						}
						*/
					}
					// next
					Tv_ItmPrgsX	=	Tv_ItmPrgsX + m_UiStat.WidthTbl[Tv_ColIdx];
					Tv_ColIdx ++;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::LcDrawHeaderOne(CDC* A_Dc, RECT* A_PtrRc, int A_Colum)
{
	// local -------------------
		UINT	Tv_Fmt;
		RECT	Tv_RcTxt;
		/*
		TRIVERTEX	Tv_Tvtx[2];
		GRADIENT_RECT	Tv_RcGrdt;
		*/
	// code --------------------
		/*
		// draw background color
		Tv_Tvtx[0].x			=	A_PtrRc->left;
		Tv_Tvtx[0].y			=	A_PtrRc->top;
		Tv_Tvtx[0].Red		=	((afxGlobalData.clrBtnLight >> 16) & 0xFF) << 8;
		Tv_Tvtx[0].Green	=	((afxGlobalData.clrBtnLight >> 8) & 0xFF) << 8;
		Tv_Tvtx[0].Blue		=	((afxGlobalData.clrBtnLight >> 0) & 0xFF) << 8;
		Tv_Tvtx[0].Alpha	=	0;

		Tv_Tvtx[1].x			=	A_PtrRc->right;
		Tv_Tvtx[1].y			=	A_PtrRc->bottom;
		Tv_Tvtx[1].Red		=	((afxGlobalData.clrBtnShadow >> 16) & 0xFF) << 8;
		Tv_Tvtx[1].Green	=	((afxGlobalData.clrBtnShadow >> 8) & 0xFF) << 8;
		Tv_Tvtx[1].Blue		=	((afxGlobalData.clrBtnShadow >> 0) & 0xFF) << 8;
		Tv_Tvtx[1].Alpha	=	0;

		Tv_RcGrdt.UpperLeft		=	0;
		Tv_RcGrdt.LowerRight	=	1;
		A_Dc->GradientFill(Tv_Tvtx, 2, &Tv_RcGrdt, 1, GRADIENT_FILL_RECT_V);
		*/
	
		// draw text
		if (NULL != m_ColumCtrl[A_Colum].StrTxt)
		{
			Tv_RcTxt.left		=	A_PtrRc->left + E_GrMfcDockGridValItemTxtHzGap;
			Tv_RcTxt.top		=	A_PtrRc->top + E_GrMfcDockGridValHeadVtMargin;
			Tv_RcTxt.right	=	A_PtrRc->right - E_GrMfcDockGridValItemTxtHzGap;
			Tv_RcTxt.bottom	=	A_PtrRc->bottom - E_GrMfcDockGridValHeadVtMargin;

			if ((Tv_RcTxt.left < Tv_RcTxt.right) && (Tv_RcTxt.top < Tv_RcTxt.bottom))
			{
				A_Dc->SelectObject(afxGlobalData.fontBold.m_hObject);
				A_Dc->SetTextColor(afxGlobalData.clrWindowText);
				Tv_Fmt		=	(UINT)m_ColumCtrl[A_Colum].TxtArrange | DT_SINGLELINE;
				A_Dc->DrawText(m_ColumCtrl[A_Colum].StrTxt, &Tv_RcTxt, Tv_Fmt);
			}
		}

		// draw left separator
		if (0 != A_Colum)
		{
			A_Dc->SelectObject(m_PenHdSepLt);
			A_Dc->MoveTo(A_PtrRc->left, A_PtrRc->top);
			A_Dc->LineTo(A_PtrRc->left, A_PtrRc->bottom - 1);
		}

		// draw right separator
		if((A_Colum + 1) < m_ColumCnt)
		{
			A_Dc->SelectObject(m_PenHdSepRt);
			A_Dc->MoveTo(A_PtrRc->right - 1, A_PtrRc->top);
			A_Dc->LineTo(A_PtrRc->right - 1, A_PtrRc->bottom - 1);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::LcDrawItems( CDC* A_Dc )
{
	// local -------------------
	// code --------------------
		// item area rect
		LcDrawCont( A_Dc );
		// border line
		A_Dc->Draw3dRect( &m_RcItemArea, afxGlobalData.clrActiveCaption, afxGlobalData.clrActiveCaption );
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::DrawLock( BOOL8 A_IsDrawAble )
{
	// local -------------------
	// code --------------------
		m_IsDrawAble	=	A_IsDrawAble;

		if ( A_IsDrawAble )
		{
			Invalidate();
			LcScrlbarVtUpdt();
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::ClearAll(void)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------

		// release header text
		for (Tv_WkIdx = 0; Tv_WkIdx < E_GrMfcDockGridValColumMaxCnt; Tv_WkIdx++)
		{
			if (NULL != m_ColumCtrl[Tv_WkIdx].StrTxt)
			{
				free(m_ColumCtrl[Tv_WkIdx].StrTxt);
				m_ColumCtrl[Tv_WkIdx].StrTxt	=	NULL;
			}
		}

		// reset
		m_ColumCnt			=	0;
		m_ColumWidthSum	=	0;
		GrDumyZeroMem(m_ColumCtrl, sizeof(m_ColumCtrl));

		m_ScrlPosHz			=	0;
		m_ScrlMaxHz			=	0;

		ClearItems();

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::ClearItems(void)
{
	// local -------------------
		int		Tv_WkItem;
		int		Tv_WkColum;
	// code --------------------
		LcCtlEditFree();
		LcComboReqFree();

		if ((0 != m_ItemCnt) && (NULL != m_PtrItems))
		{
			for (Tv_WkItem=0; Tv_WkItem < m_ItemCnt; Tv_WkItem++)
			{
				for (Tv_WkColum = 0; Tv_WkColum < E_GrMfcDockGridValColumMaxCnt; Tv_WkColum++)
				{
					if (m_PtrItems[Tv_WkItem].Colum[Tv_WkColum].IsLocal)
					{
						if(NULL != m_PtrItems[Tv_WkItem].Colum[Tv_WkColum].ValInfo.PtrVal)
						{
							free(m_PtrItems[Tv_WkItem].Colum[Tv_WkColum].ValInfo.PtrVal);
							m_PtrItems[Tv_WkItem].Colum[Tv_WkColum].ValInfo.PtrVal	=	NULL;
						}
					}
				}
			}
		}

		// reset
		m_ItemCnt				=	0;
		m_ScrlPosVt			=	0;
		m_ScrlMaxVt			=	0;

		LcScrlbarVtUpdt();

}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ColumAdd(int A_Width)
{
	// local -------------------
		BOOL8	Tv_Result;
		int		Tv_Width;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrMfcDockGridValColumMaxCnt > m_ColumCnt )
		{
			if((0 == m_UiStat.WidthTbl[m_ColumCnt]) || (!m_IsCanWidthChg))
			{
				Tv_Width	=	A_Width;
				if(E_GrMfcDockGridValColumMinWidth > Tv_Width)
				{
					Tv_Width	=	E_GrMfcDockGridValColumMinWidth;
				}
				m_UiStat.WidthTbl[m_ColumCnt]	=	Tv_Width;	// update colum size
			}

			m_ColumWidthSum	=	m_ColumWidthSum + m_UiStat.WidthTbl[m_ColumCnt];
			m_ColumCnt ++;		// increase colum count

			LcScrlbarHzUpdt();

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int	Cls_GrMfcDockGridVal::ItemAdd(void)
{
	// local -------------------
		int	Tv_Result;
		int	Tv_ColIdx;
	// code --------------------
		Tv_Result	=	-1;

		if ( (NULL != m_PtrItems) && (m_ItemMaxCnt > m_ItemCnt) )
		{

			for ( Tv_ColIdx = 0; Tv_ColIdx < E_GrMfcDockGridValColumMaxCnt; Tv_ColIdx++)
			{
				m_PtrItems[m_ItemCnt].Colum[Tv_ColIdx].ValInfo.IsEdit			=	FALSE;
				m_PtrItems[m_ItemCnt].Colum[Tv_ColIdx].ValInfo.IsModified	=	FALSE;
				m_PtrItems[m_ItemCnt].Colum[Tv_ColIdx].ValInfo.PtrVal			=	NULL;
				m_PtrItems[m_ItemCnt].Colum[Tv_ColIdx].ValInfo.TxtArrange	=	E_GrMfcDockTxtArrangeCenterVt;
				m_PtrItems[m_ItemCnt].Colum[Tv_ColIdx].ValInfo.ValCnt			=	0;
				m_PtrItems[m_ItemCnt].Colum[Tv_ColIdx].ValInfo.ValType		=	E_GrVarTypeNone;
				m_PtrItems[m_ItemCnt].Colum[Tv_ColIdx].UserData						=	0;
				m_PtrItems[m_ItemCnt].Colum[Tv_ColIdx].IsLocal						=	FALSE;
				m_PtrItems[m_ItemCnt].Colum[Tv_ColIdx].ColorNormal	=	afxGlobalData.clrWindow;
				m_PtrItems[m_ItemCnt].Colum[Tv_ColIdx].IsMark				=	FALSE;
				m_PtrItems[m_ItemCnt].Colum[Tv_ColIdx].ColorMark		=	afxGlobalData.clrBarFace;
			}

			// success
			Tv_Result	=	m_ItemCnt;
			m_ItemCnt ++;
			// redraw
			if ( m_IsDrawAble )
			{
				Invalidate();
			}
			LcScrlbarVtUpdt();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ItemSetValue(int A_ItemIdx, int A_Colum, __u16 A_ValType, void* A_PtrVal, __u16 A_ValCnt, BOOL8 A_IsEdit, __u32 A_TxtArrage)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( (NULL != m_PtrItems) && (0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) )
		{
			if ( (0 <= A_Colum) && (m_ColumCnt > A_Colum) )
			{
				// check exist local value already
				if((m_PtrItems[A_ItemIdx].Colum[A_Colum].IsLocal) && (NULL != m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.PtrVal))
				{
					// release local buffer
					free(m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.PtrVal);
					m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.PtrVal	=	NULL;
					m_PtrItems[A_ItemIdx].Colum[A_Colum].IsLocal				=	FALSE;
					m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.ValCnt	=	0;
				}
				// set item
				m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.IsEdit			=	A_IsEdit;
				m_PtrItems[A_ItemIdx].Colum[A_Colum].IsLocal						=	FALSE;
				m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.IsModified	=	FALSE;
				m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.PtrVal			=	A_PtrVal;
				m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.ValCnt			=	A_ValCnt;
				m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.ValType		=	A_ValType;
				m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.TxtArrange	=	A_TxtArrage;

				// success
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ItemSetLocalText(int A_ItemIdx, int A_Colum, WCHAR* A_StrVal, __u32 A_TxtArrage)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Len;
	// code --------------------
		Tv_Result	=	-1;

		if ((NULL != m_PtrItems) && (0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx))
		{
			if ((0 <= A_Colum) && (m_ColumCnt > A_Colum))
			{
				// check exist local value already
				if((m_PtrItems[A_ItemIdx].Colum[A_Colum].IsLocal) && (NULL != m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.PtrVal))
				{
					// release local buffer
					free(m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.PtrVal);
					m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.PtrVal		=	NULL;
					m_PtrItems[A_ItemIdx].Colum[A_Colum].IsLocal					=	FALSE;
					m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.ValCnt		=	0;
					m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.ValType	=	E_GrVarTypeNone;
				}

				// create new buffer
				Tv_Len	=	GrStrWlen(A_StrVal);
				if (0 != Tv_Len)
				{
					m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.PtrVal	=	malloc((Tv_Len + 1) * 2);
					if(NULL != m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.PtrVal)
					{
						GrStrWcopy((WCHAR*)m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.PtrVal, A_StrVal);
					}
				}
				m_PtrItems[A_ItemIdx].Colum[A_Colum].IsLocal			=	TRUE;
				m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.ValCnt			=	(__u8)Tv_Len;
				m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.ValType		=	E_GrVarTypeWstr;
				m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.IsModified	=	FALSE;
				m_PtrItems[A_ItemIdx].Colum[A_Colum].ValInfo.TxtArrange	=	A_TxtArrage;
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::LcDrawCont( CDC* A_Dc )
{
	// local -------------------
		int		Tv_ItmIdx;
		int		Tv_ViewCnt;
		RECT	Tv_RcItm;
	// code --------------------
		// draw start item
		Tv_ItmIdx					=	m_ScrlPosVt;
		Tv_ViewCnt				=	0;
		Tv_RcItm.left			=	m_RcItemCont.left;
		Tv_RcItm.top			=	m_RcItemCont.top;
		Tv_RcItm.right		=	m_RcItemCont.right;
		Tv_RcItm.bottom		=	Tv_RcItm.top + m_ItemHeight;

		if ((0 != m_ItemCnt) && (0 != m_ColumCnt))
		{
			while ((Tv_ItmIdx < m_ItemCnt) && (Tv_RcItm.bottom <= m_RcItemCont.bottom))
			{
				// normal item
				LcDrawItem(A_Dc, &Tv_RcItm, Tv_ItmIdx);

				// next
				Tv_ItmIdx ++;
				Tv_ViewCnt ++;
				Tv_RcItm.top		=	Tv_RcItm.bottom;
				Tv_RcItm.bottom	=	Tv_RcItm.bottom + m_ItemHeight;
			}
		}

		// draw bottom space
		if(Tv_RcItm.top < m_RcItemCont.bottom)
		{
			Tv_RcItm.bottom	=	m_RcItemCont.bottom;
			A_Dc->FillSolidRect(&Tv_RcItm, afxGlobalData.clrWindow);
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::LcDrawItem( CDC* A_Dc, RECT* A_PtrRc, int A_ItemIdx )
{
	// local -------------------
		int			Tv_ColIdx;
		RECT		Tv_RcCol;
		COLORREF	Tv_ClrBg;
		BOOL8		Tv_IsModified;
		__u32		Tv_HndlItem;
	// code --------------------
		// init
		Tv_IsModified			=	FALSE;
		A_Dc->SelectObject(afxGlobalData.fontRegular.m_hObject);

		// draw colum
		Tv_RcCol.left			=	A_PtrRc->left - m_ScrlPosHz;
		Tv_RcCol.top			=	A_PtrRc->top;
		Tv_RcCol.bottom		=	A_PtrRc->bottom;

		// check draw able
		if(NULL != m_PtrItems)
		{
			for(Tv_ColIdx = 0; Tv_ColIdx < m_ColumCnt; Tv_ColIdx++)
			{
				// set rect right
				Tv_RcCol.right	=	Tv_RcCol.left + (int)m_UiStat.WidthTbl[Tv_ColIdx];
				// check draw able
				if(((Tv_RcCol.left >= A_PtrRc->left) && (Tv_RcCol.left < A_PtrRc->right)) ||
					((Tv_RcCol.right >= A_PtrRc->left) && (Tv_RcCol.right < A_PtrRc->right)) ||
					((Tv_RcCol.left < A_PtrRc->left) && (Tv_RcCol.right > A_PtrRc->right)))
				{
					// draw mark background
					Tv_ClrBg	=	m_PtrItems[A_ItemIdx].Colum[Tv_ColIdx].ColorNormal;
					if(m_PtrItems[A_ItemIdx].Colum[Tv_ColIdx].IsMark)
					{
						Tv_ClrBg	=	m_PtrItems[A_ItemIdx].Colum[Tv_ColIdx].ColorMark;
					}
					A_Dc->FillSolidRect(&Tv_RcCol, Tv_ClrBg);

					// font
					if(Tv_IsModified != m_PtrItems[A_ItemIdx].Colum[Tv_ColIdx].ValInfo.IsModified)
					{
						if(m_PtrItems[A_ItemIdx].Colum[Tv_ColIdx].ValInfo.IsModified)
						{
							A_Dc->SelectObject(afxGlobalData.fontBold.m_hObject);
						}
						else
						{
							A_Dc->SelectObject(afxGlobalData.fontRegular.m_hObject);
						}
						// update 
						Tv_IsModified		=	m_PtrItems[A_ItemIdx].Colum[Tv_ColIdx].ValInfo.IsModified;
					}

					// draw value
					Tv_HndlItem	=	ItemHndlEncode(A_ItemIdx, Tv_ColIdx);
					DrawValue(A_Dc, &Tv_RcCol, Tv_HndlItem);

					// draw seperater right
					A_Dc->SelectObject((HGDIOBJ)m_PenCtSepRt);
					A_Dc->MoveTo(Tv_RcCol.right - 1, A_PtrRc->top);
					A_Dc->LineTo(Tv_RcCol.right - 1, A_PtrRc->bottom - 1);

					// draw selected

				}
				// next
				Tv_RcCol.left		=	Tv_RcCol.right;
			}
		}

		// draw margin
		if((Tv_RcCol.left + 1) < A_PtrRc->right)
		{
			Tv_RcCol.right	=	A_PtrRc->right;
			A_Dc->FillSolidRect(&Tv_RcCol, afxGlobalData.clrWindow);
		}

		// draw under line
		A_Dc->SelectObject((HGDIOBJ)m_PenCtUline);
		A_Dc->MoveTo(A_PtrRc->left, A_PtrRc->bottom - 1);
		A_Dc->LineTo(A_PtrRc->right, A_PtrRc->bottom - 1);

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::LcScrlbarHzUpdt(void)
{
	// local -------------------
		RECT	Tv_RcCli;
		int		Tv_MaxX;
	// code --------------------
		if(NULL == m_hWnd)
		{
			return;
		}

		Tv_MaxX				=	0;
		GetClientRect(&Tv_RcCli);
		LcDimensionCalc(&Tv_RcCli);

		// update max
		Tv_MaxX				=	m_ColumWidthSum - (m_RcHeadArea.right - m_RcHeadArea.left);
		if(0 > Tv_MaxX)
		{
			Tv_MaxX	=	0;
		}
		// update max
		m_ScrlMaxHz		=	Tv_MaxX;
		SetScrollRange(SB_HORZ,0, Tv_MaxX);
		if(Tv_MaxX < m_ScrlPosHz)
		{
			m_ScrlPosHz		=	Tv_MaxX;
			SetScrollPos(SB_HORZ, m_ScrlPosHz);
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::LcScrlbarVtUpdt(void)
{
	// local -------------------
		RECT	Tv_RcCli;
		int		Tv_MaxY;
	// code --------------------
		if(NULL == m_hWnd)
		{
			return;
		}

		Tv_MaxY				=	0;
		GetClientRect( &Tv_RcCli );
		LcDimensionCalc( &Tv_RcCli );

		if ( 0 == m_ItemViewAble )
		{
			m_ItemViewAble ++;
		}
		if ( m_ItemViewAble < m_ItemCnt )
		{
			Tv_MaxY	=	m_ItemCnt - m_ItemViewAble;
		}
		// update max
		SetScrollRange( SB_VERT, 0, Tv_MaxY );
		if(Tv_MaxY < m_ScrlPosVt)
		{
			SetScrollPos( SB_VERT, Tv_MaxY );
			m_ScrlPosVt	=	Tv_MaxY;
		}
		// update max
		m_ScrlMaxVt	=	Tv_MaxY;
		// update position
		m_ScrlPosVt	=	GetScrollPos( SB_VERT );
}
//--------------------------------------------------------------------
void Cls_GrMfcDockGridVal::OnSize(UINT nType, int cx, int cy)
{
	// local -------------------
	// code --------------------
		Cls_GrMfcDockCtrlBase::OnSize(nType, cx, cy);
		LcScrlbarHzUpdt();
		LcScrlbarVtUpdt();
}
//--------------------------------------------------------------------
int		Cls_GrMfcDockGridVal::LcXposToColumIdx(int A_PosX, BOOL8 A_IsChkNearGrid, int* A_PtrRtPosX, BOOL8* A_PtrRtNeerGrid)
{
	// local -------------------
		int		Tv_Result;
		int		Tv_PrgsX;
		int		Tv_IstX;
		int		Tv_IedX;
		int		Tv_ColIdx;
	// code --------------------
		// init
		Tv_Result	=	-1;
		*A_PtrRtPosX			=	0;
		*A_PtrRtNeerGrid	=	FALSE;

		Tv_PrgsX	=	0;
		Tv_ColIdx	=	0;

		while(Tv_ColIdx < m_ColumCnt)
		{
			Tv_IstX		=	Tv_PrgsX - m_ScrlPosHz;
			Tv_IedX		=	Tv_IstX + (int)m_UiStat.WidthTbl[Tv_ColIdx];
			if(A_IsChkNearGrid)
			{
				Tv_IedX		=	Tv_IedX + E_GrMfcDockCtrlBaseSplitRng;
			}
			if( (Tv_IstX <= A_PosX) && (Tv_IedX > A_PosX) )
			{
				// found
				*A_PtrRtPosX	=	A_PosX - Tv_IstX;
				Tv_Result			=	Tv_ColIdx;
				// check approach grid
				if(A_IsChkNearGrid)
				{
					if(((Tv_IedX - (E_GrMfcDockCtrlBaseSplitRng * 2)) <= A_PosX) && (Tv_IedX > A_PosX))
					{
						*A_PtrRtNeerGrid	=	TRUE;
					}
				}
				break;
			}
			// next
			Tv_PrgsX	=	Tv_PrgsX + (int)m_UiStat.WidthTbl[Tv_ColIdx];
			Tv_ColIdx ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrMfcDockGridVal::LcPosInfoGet(int A_X, int A_Y, int* A_PtrRtItem, int* A_PtrRtColum, int* A_PtrRtValPosX, int* A_PtrRtValPosY)
{
	// local -------------------
		__u8	Tv_Result;
		int		Tv_RtValX;	// return value position x, -1 = name click
		int		Tv_RtValY;	// return value position y, -1 = name click
		int		Tv_RtItem;
		int		Tv_RtColum;
		RECT	Tv_RcCli;
		BOOL8	Tv_IsNeerGrid;
	// code --------------------
		// init
		Tv_Result		=	E_GrMfcDockCtrlBasePosOnNotting;

		Tv_RtValX		=	-1;
		Tv_RtValY		=	-1;
		Tv_RtColum	=	-1;
		Tv_RtItem		=	-1;

		// calculate dimemsion
		GetClientRect(&Tv_RcCli);
		LcDimensionCalc(&Tv_RcCli);

		// check header area
		if(((0 != m_HeaderHeight) && (0 != m_ColumWidthSum)) &&
			(m_RcHeadArea.left <= A_X) && (m_RcHeadArea.right > A_X) && (m_RcHeadArea.top <= A_Y) && (m_RcHeadArea.bottom > A_Y))
		{
			// check item index
			Tv_Result		=	E_GrMfcDockCtrlBasePosOnHeader;

			// check colum index
			Tv_IsNeerGrid	=	FALSE;
			Tv_RtColum	=	LcXposToColumIdx(A_X, TRUE, &Tv_RtValX, &Tv_IsNeerGrid);
			
			if(Tv_IsNeerGrid && m_IsCanWidthChg)
			{
				Tv_Result		=	E_GrMfcDockCtrlBasePosOnGridSzRt;
			}
		}
		else if( (m_RcItemCont.left <= A_X) && (m_RcItemCont.right > A_X) && (m_RcItemCont.top <= A_Y) )
		{
			// check item exist area
			if((m_RcItemCont.top + (m_ItemViewCnt * m_ItemHeight)) > A_Y)
			{
				// get item position
				Tv_RtItem		=	m_ScrlPosVt + ((A_Y - m_RcItemCont.top) / m_ItemHeight);
				Tv_RtValY		=	(A_Y - m_RcItemCont.top) % m_ItemHeight;
				Tv_RtColum	=	LcXposToColumIdx(A_X, FALSE, &Tv_RtValX, &Tv_IsNeerGrid);

				Tv_Result		=	E_GrMfcDockCtrlBasePosOnItem;
			}
		}

		// upate
		*A_PtrRtItem		=	Tv_RtItem;
		*A_PtrRtColum		=	Tv_RtColum;
		*A_PtrRtValPosX	=	Tv_RtValX;
		*A_PtrRtValPosY	=	Tv_RtValY;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrMfcDockGridVal::ItemGetUserData(int A_ItemIdx, int A_ItemColum)
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( (0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) && (0 <= A_ItemColum) && (m_ColumCnt > A_ItemColum) && (NULL != m_PtrItems) )
		{
			// get user data
			Tv_Result	=	m_PtrItems[A_ItemIdx].Colum[A_ItemColum].UserData;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ItemSetUserData(int A_ItemIdx, int A_ItemColum, __u64 A_UserData)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) && (0 <= A_ItemColum) && (m_ColumCnt > A_ItemColum) && (NULL != m_PtrItems))
		{
			m_PtrItems[A_ItemIdx].Colum[A_ItemColum].UserData	=	A_UserData;
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ItemSetMarkStatus(int A_ItemIdx, int A_ItemColum, BOOL8 A_IsMark)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) && (0 <= A_ItemColum) && (m_ColumCnt > A_ItemColum) && (NULL != m_PtrItems))
		{
			m_PtrItems[A_ItemIdx].Colum[A_ItemColum].IsMark	=	A_IsMark;
			// redraw item

			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ItemSetMarkColor(int A_ItemIdx, int A_ItemColum, COLORREF A_Color)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) && (0 <= A_ItemColum) && (m_ColumCnt > A_ItemColum) && (NULL != m_PtrItems))
		{
			m_PtrItems[A_ItemIdx].Colum[A_ItemColum].ColorMark	=	A_Color;
			// redraw item

			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ItemSetColor(int A_ItemIdx, int A_ItemColum, COLORREF A_Color)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if ((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) && (0 <= A_ItemColum) && (m_ColumCnt > A_ItemColum) && (NULL != m_PtrItems))
		{
			m_PtrItems[A_ItemIdx].Colum[A_ItemColum].ColorNormal	=	A_Color;
			// redraw item

			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::IsItemMarked(int A_ItemIdx, int A_ItemColum)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) && (0 <= A_ItemColum) && (m_ColumCnt > A_ItemColum) && (NULL != m_PtrItems))
		{
			Tv_Result		=	m_PtrItems[A_ItemIdx].Colum[A_ItemColum].IsMark;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int	Cls_GrMfcDockGridVal::ItemCountGet(void)
{
		return	m_ItemCnt;
}
//--------------------------------------------------------------------
int		Cls_GrMfcDockGridVal::ScrollPosVtGet(void)
{
	// local -------------------
	// code --------------------
		return	m_ScrlPosVt;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::ScrollPosVtSet(int A_ScrlPos)
{
	// local -------------------
		int		Tv_ScrlPos;
	// code --------------------
		if ( 0 <= A_ScrlPos )
		{
			Tv_ScrlPos	=	A_ScrlPos;
			if ( m_ScrlMaxVt < Tv_ScrlPos )
			{
				Tv_ScrlPos	=	m_ScrlMaxVt;
			}
			Invalidate(FALSE);
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ItemDel(int A_ItemIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) && (NULL != m_PtrItems))
		{
			if((1 < m_ItemCnt) && (A_ItemIdx < (m_ItemCnt - 1)))
			{
				GrDumyCopyMem(&m_PtrItems[A_ItemIdx], &m_PtrItems[A_ItemIdx+1], sizeof(St_GrMfcDockGridValItemCtrl)* (m_ItemCnt - A_ItemIdx - 1));
			}
			m_ItemCnt --;
		}

		LcScrlbarVtUpdt();
		Invalidate(FALSE);

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ItemSetModifiedColum(int A_Colum, BOOL8 A_IsModified)
{
	// local -------------------
		BOOL8	Tv_Result;
		int		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if((0 <= A_Colum) && (m_ColumCnt > A_Colum) && (NULL != m_PtrItems))
		{
			if(0 != m_ItemCnt)
			{
				for( Tv_WkIdx = 0; Tv_WkIdx < m_ItemCnt; Tv_WkIdx++)
				{
					m_PtrItems[Tv_WkIdx].Colum[A_Colum].ValInfo.IsModified	=	A_IsModified;
				}
				
				Tv_Result	=	TRUE;
			}
		}

		Invalidate(FALSE);

		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::HeaderSet(int A_Colum, WCHAR* A_StrTxt, __u32 A_TxtArrage)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_LenTxt;
		__u32	Tv_BufSize;
	// code --------------------
		Tv_Result	=	FALSE;

		// check index reange
		if (E_GrMfcDockGridValColumMaxCnt > (__u32)A_Colum)
		{
			// release previous buffer
			if (NULL != m_ColumCtrl[A_Colum].StrTxt)
			{
				// release buffer
				free(m_ColumCtrl[A_Colum].StrTxt);
				m_ColumCtrl[A_Colum].StrTxt	=	NULL;
			}

			// get text length
			Tv_BufSize	=	0;
			Tv_LenTxt		=	GrStrWlen(A_StrTxt);
			if (0 != Tv_LenTxt)
			{
				Tv_BufSize	=	(Tv_LenTxt + 1) * 2;
				m_ColumCtrl[A_Colum].StrTxt	=	(WCHAR*)malloc(Tv_BufSize);
			}

			// copy text
			if (NULL != m_ColumCtrl[A_Colum].StrTxt)
			{
				GrDumyCopyMem(m_ColumCtrl[A_Colum].StrTxt, A_StrTxt, Tv_BufSize);
			}

			// update parametter
			m_ColumCtrl[A_Colum].TxtArrange	=	A_TxtArrage;

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::HeaderShow(BOOL8 A_IsShow)
{
	// local -------------------
	// code --------------------
		m_IsDrawHead	=	A_IsShow;

		Invalidate(FALSE);
}
//--------------------------------------------------------------------
int		Cls_GrMfcDockGridVal::LcColumWidthSumCalc(int A_Colum)
{
	// local -------------------
		int	Tv_Result;
		int	Tv_ColIdx;
	// code --------------------
		Tv_Result	=	0;

		Tv_ColIdx	=	0;
		while(Tv_ColIdx <= A_Colum)
		{
			Tv_Result	=	Tv_Result + (int)m_UiStat.WidthTbl[Tv_ColIdx];
			// next
			Tv_ColIdx ++;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ScrollByIdx(int A_ItemIdx, int A_Colum)
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsScrlVt;
		BOOL8	Tv_IsScrlHz;
		int		Tv_WdtSum;
		int		Tv_PosStX;
		int		Tv_PosEdX;
		RECT	Tv_RcCli;
	// code --------------------
		// init
		Tv_Result		=	FALSE;

		// check able
		if(NULL == m_hWnd)
		{
			return	FALSE;
		}

		// check item range
		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) && (0 <= A_Colum) && (m_ColumCnt > A_Colum))
		{
			// dimension calculate
			GetClientRect(&Tv_RcCli);
			LcDimensionCalc(&Tv_RcCli);

			// check need scroll
			Tv_IsScrlVt	=	FALSE;
			if( (m_ScrlPosVt > A_ItemIdx) || ((m_ScrlPosVt + m_ItemViewCnt) <= A_ItemIdx) )
			{
				Tv_IsScrlVt	=	TRUE;
				// change scroll position
				m_ScrlPosVt	=	A_ItemIdx;
				if((m_ItemCnt - m_ScrlPosVt) < m_ItemViewAble)
				{
					m_ScrlPosVt	=	m_ItemCnt - m_ItemViewAble;
					if(0 > m_ScrlPosVt)
					{
						m_ScrlPosVt	=	0;
					}
				}
				SetScrollPos(SB_VERT, m_ScrlPosVt, TRUE);
			}

			Tv_IsScrlHz	=	FALSE;
			Tv_WdtSum		=	LcColumWidthSumCalc(A_Colum);
			Tv_PosStX		=	Tv_WdtSum - (int)m_UiStat.WidthTbl[A_Colum] - m_ScrlPosHz;
			Tv_PosEdX		=	Tv_WdtSum - m_ScrlPosHz;
			if((m_RcItemCont.left > Tv_PosStX) || (m_RcItemCont.right < Tv_PosEdX))
			{
				Tv_IsScrlHz	=	TRUE;
				m_ScrlPosHz	=	Tv_WdtSum - (int)m_UiStat.WidthTbl[A_Colum];
				if((m_ColumWidthSum - m_ScrlPosHz) < (m_RcItemCont.right - m_RcItemCont.left))
				{
					m_ScrlPosHz	=	m_ColumWidthSum - (m_RcItemCont.right - m_RcItemCont.left);
					if(0 > m_ScrlPosHz)
					{
						m_ScrlPosHz		=	0;
					}
				}
				SetScrollPos(SB_HORZ, m_ScrlPosHz, TRUE);
			}

			// check need redraw
			if(Tv_IsScrlHz || Tv_IsScrlVt)
			{
				Invalidate(FALSE);
			}

			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::LcItemRectGetByIdx(int A_ItemIdx, int A_Colum, RECT* A_PtrRtRect)
{
	// local -------------------
		BOOL8	Tv_Result;
		RECT	Tv_RcCli;
		int		Tv_WdtSum;
		int		Tv_PosStX;
	// code --------------------
		Tv_Result		=	FALSE;
		
		// check item range
		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) && (0 <= A_Colum) && (m_ColumCnt > A_Colum))
		{
			// dimension calculate
			GetClientRect(&Tv_RcCli);
			LcDimensionCalc(&Tv_RcCli);
			// check item range
			if((m_ScrlPosVt <= A_ItemIdx) && ((m_ScrlPosVt + m_ItemViewCnt) > A_ItemIdx))
			{
				// update virtical position
				A_PtrRtRect->top			=	m_RcItemCont.top + (m_ItemHeight * (A_ItemIdx - m_ScrlPosVt));
				A_PtrRtRect->bottom	=	A_PtrRtRect->top + m_ItemHeight;

				// colum range
				Tv_WdtSum		=	LcColumWidthSumCalc(A_Colum);
				Tv_PosStX		=	Tv_WdtSum - (int)m_UiStat.WidthTbl[A_Colum] - m_ScrlPosHz;

				A_PtrRtRect->left		=	Tv_PosStX + m_RcItemCont.left;
				A_PtrRtRect->right		=	A_PtrRtRect->left + (int)m_UiStat.WidthTbl[A_Colum];

				// patch range
				if(m_RcItemCont.left > A_PtrRtRect->left)
				{
					A_PtrRtRect->left	=	m_RcItemCont.left;
				}
				if(m_RcItemCont.right < A_PtrRtRect->left)
				{
					A_PtrRtRect->left	=	m_RcItemCont.right;
				}

				if(m_RcItemCont.left > A_PtrRtRect->right)
				{
					A_PtrRtRect->right		=	m_RcItemCont.left;
				}
				if(m_RcItemCont.right < A_PtrRtRect->right)
				{
					A_PtrRtRect->right		=	m_RcItemCont.right;
				}

				if(A_PtrRtRect->left < A_PtrRtRect->right)
				{
					Tv_Result		=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::LcColumWidthSumUpdate(void)
{
	// local -------------------
		int		Tv_ColIdx;
	// code --------------------
		if(0 != m_ColumCnt)
		{
			m_ColumWidthSum	=	0;
			for(Tv_ColIdx = 0; Tv_ColIdx < m_ColumCnt; Tv_ColIdx++)
			{
				m_ColumWidthSum	=	m_ColumWidthSum + m_UiStat.WidthTbl[Tv_ColIdx];
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::ItemHndlDecode(__u32 A_HndlItem, int* A_PtrRtLine, int* A_PtrRtColum)
{
	// local -------------------
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		// init
		Tv_Line		=	-1;
		Tv_Colum	=	-1;

		if(E_GrMfcDockGridValHndlItemNone != A_HndlItem)
		{
			Tv_Colum	=	(int)((A_HndlItem >> 16) & 0xFFFF) - E_GrMfcDockGridValHndlElemBase;
			if(0 <= Tv_Colum)
			{
				Tv_Line		=	(int)(A_HndlItem & 0xFFFF) - E_GrMfcDockGridValHndlElemBase;
			}
		}

		*A_PtrRtLine	=	Tv_Line;
		*A_PtrRtColum	=	Tv_Colum;
}
//--------------------------------------------------------------------
__u32	Cls_GrMfcDockGridVal::ItemHndlEncode(int A_Line, int A_Colum)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		// init
		Tv_Result	=	E_GrMfcDockGridValHndlItemNone;
		if(0 <= A_Colum)
		{
			Tv_Result	=	Tv_Result | (((__u32)A_Colum + E_GrMfcDockGridValHndlElemBase) << 16);
			if(0 <= A_Line)
			{
				Tv_Result	=	Tv_Result | ((__u32)A_Line + E_GrMfcDockGridValHndlElemBase);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrMfcDockCtrlBaseValueInfo	Cls_GrMfcDockGridVal::ValueInfoGetByItemHndl(__u32 A_HndlItem)
{
	// local -------------------
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_Result;
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		// init
		Tv_Result	=	NULL;

		ItemHndlDecode(A_HndlItem, &Tv_Line, &Tv_Colum);
		if((0<= Tv_Line) && (0 <= Tv_Colum) && (NULL != m_PtrItems))
		{
			// check item count
			if((m_ItemCnt > Tv_Line) && (m_ColumCnt > Tv_Colum))
			{
				Tv_Result	=	&m_PtrItems[Tv_Line].Colum[Tv_Colum].ValInfo;
			}
			else
			{
				DbgMsgPrint("Cls_GrMfcDockGridVal::ValueInfoGetByItemHndl - bad handle value (%d)\n", A_HndlItem);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrMfcDockGridVal::ValueInfoGetByItemHndl - bad handle value (%d)\n", A_HndlItem);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ItemRectGetByItemHndl(__u32 A_HndlItem, RECT* A_PtrRtRect)
{
	// local -------------------
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		ItemHndlDecode(A_HndlItem, &Tv_Line, &Tv_Colum);
		return	LcItemRectGetByIdx(Tv_Line, Tv_Colum, A_PtrRtRect);
}
//--------------------------------------------------------------------
__u8	Cls_GrMfcDockGridVal::PosInfoGet(int A_X, int A_Y, __u32* A_PtrRtHndlItem, int* A_PtrRtItemPosX, int* A_PtrRtItemPosY)
{
	// local -------------------
		__u8	Tv_Result;
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		Tv_Result	=	LcPosInfoGet(A_X, A_Y, &Tv_Line, &Tv_Colum, A_PtrRtItemPosX, A_PtrRtItemPosY);
		*A_PtrRtHndlItem	=	ItemHndlEncode(Tv_Line, Tv_Colum);
		return	Tv_Result;
}
//--------------------------------------------------------------------
int*	Cls_GrMfcDockGridVal::GridSizeValuePtrGet(__u32 A_HndlItem, __u8 A_Mode)
{
	// local -------------------
		int*	Tv_Result;
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		Tv_Result	=	NULL;

		if(E_GrMfcDockCtrlBaseGridSizeRt == A_Mode)
		{
			ItemHndlDecode(A_HndlItem, &Tv_Line, &Tv_Colum);
			if((0 <= Tv_Colum) && (m_ColumCnt > Tv_Colum))
			{
				Tv_Result	=	(int*)&m_UiStat.WidthTbl[Tv_Colum];
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::OnGridSizeChange(void)
{
	// local -------------------
	// code --------------------
		LcColumWidthSumUpdate();
		LcScrlbarHzUpdt();
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ScrollByItemHndl(__u32 A_HndlItem)
{
	// local -------------------
		BOOL8	Tv_Result;
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		Tv_Result	=	FALSE;

		ItemHndlDecode(A_HndlItem, &Tv_Line, &Tv_Colum);
		if((0 <= Tv_Line) && (0 <= Tv_Colum))
		{
			Tv_Result	=	ScrollByIdx(Tv_Line, Tv_Colum);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockGridVal::ItemSetHeight(int A_Height)
{
	// local -------------------
		BOOL8	Tv_Result;
		RECT	Tv_RcCli;
		int		Tv_TxtHeight;
		int		Tv_Height;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Height				=	A_Height;

		Tv_TxtHeight		=	afxGlobalData.GetTextHeight(TRUE);
		if (Tv_Height < (Tv_TxtHeight + (E_GrMfcDockGridValItemVtMargin * 2)))
		{
			Tv_Height	=	(Tv_TxtHeight + (E_GrMfcDockGridValItemVtMargin * 2));
		}
		
		if ((0 < Tv_Height) && (NULL != m_hWnd))
		{
			m_ItemHeight	=	Tv_Height;

			GetClientRect(&Tv_RcCli);
			LcDimensionCalc(&Tv_RcCli);
			//LcScrlbarHzUpdt();
			LcScrlbarVtUpdt();
			Invalidate(FALSE);

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int	Cls_GrMfcDockGridVal::ItemGetHeight(void)
{
		return	m_ItemHeight;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockGridVal::WidthControlSet(BOOL8 A_IsAble)
{
	// local -------------------
	// code --------------------
		m_IsCanWidthChg	=	A_IsAble;
}
//--------------------------------------------------------------------
