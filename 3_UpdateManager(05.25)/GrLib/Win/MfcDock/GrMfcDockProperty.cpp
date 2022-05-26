/*
	grid value

*/
#include <Win\MfcDock\GrMfcDockProperty.h>

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

BOOL8	V_GrMfcDockPropertyIsInit			= FALSE;


//====================================================================
//--------------------------------------------------------------------
Cls_GrMfcDockProperty::Cls_GrMfcDockProperty(  ):
Cls_GrMfcDockCtrlBase()
{
	// local -------------------
		WNDCLASS Tv_Wcls; 
		int		Tv_ItmIdx;
	// code --------------------
		// init
		m_IsDrawAble	=	TRUE;

		// ui status
		GrDumyZeroMem(&m_UiStat, sizeof(m_UiStat));
		m_UiStat.NameWidth	=	E_GrMfcDockPropertyNameDfltWidth;
		m_UiStat.DescHeight	=	E_GrMfcDockPropertyDescDfltHeight;

		// item
		m_ItemCnt			=	0;

		GrDumyZeroMem( m_ItemTbl, sizeof(m_ItemTbl) );
		for(Tv_ItmIdx = 0; Tv_ItmIdx < E_GrMfcDockPropertyItemMaxCnt; Tv_ItmIdx++)
		{
			m_ItemTbl[Tv_ItmIdx].Line				=	Tv_ItmIdx;
			m_ItemTbl[Tv_ItmIdx].ColorMark	=	afxGlobalData.clrBarFace;
		}

		// display
		GrDumyZeroMem(m_ViewTbl, sizeof(m_ViewTbl));
		m_ViewAbleCnt	=	0;
		m_ViewNowCnt	=	0;
		m_ViewCnt			=	0;

		// desc
		m_DescNameTxtArrange	=	0;
		m_DescCmtTxtArrange		=	0;

		// register window class
		if ( !V_GrMfcDockPropertyIsInit )
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
			Tv_Wcls.lpszClassName = L"GrProperty";  // name of window class 

			// Register the window class. 
			if ( AfxRegisterClass( &Tv_Wcls ) )
			{
				V_GrMfcDockPropertyIsInit	=	TRUE;
			}
		}

		// resource
		m_PenFoldMark	=	CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		m_PenCtUline	=	CreatePen(PS_SOLID, 1, afxGlobalData.clrActiveCaption);
		m_PenCtSplit	=	CreatePen(PS_SOLID, 1, afxGlobalData.clrActiveCaption);
}
//--------------------------------------------------------------------
Cls_GrMfcDockProperty::~Cls_GrMfcDockProperty( void )
{
	// local -------------------
	// code --------------------
		ClearAll();

		DeleteObject((HGDIOBJ)m_PenFoldMark);
		DeleteObject((HGDIOBJ)m_PenCtUline);
		DeleteObject((HGDIOBJ)m_PenCtSplit);

		CtrlRgtyValWrite(E_GrMfcDockPropertyUiStatValId, &m_UiStat, sizeof(m_UiStat));
}
//--------------------------------------------------------------------
BEGIN_MESSAGE_MAP(Cls_GrMfcDockProperty, Cls_GrMfcDockCtrlBase)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()
//--------------------------------------------------------------------
BOOL	Cls_GrMfcDockProperty::Create(RECT& A_Rect, CWnd* A_WndParent, UINT A_Id)
{
	// local -------------------
		BOOL	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( V_GrMfcDockPropertyIsInit && (NULL != A_WndParent) )
		{
			Tv_Result	=	Cls_GrMfcDockCtrlBase::Create(L"GrProperty", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL, A_Rect, A_WndParent, A_Id);
			CtrlRgtyValRead(E_GrMfcDockPropertyUiStatValId, &m_UiStat, sizeof(m_UiStat));
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void Cls_GrMfcDockProperty::OnPaint( )
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
		
		LcDrawItems(Tv_Dc);

		LcDrawDescs(Tv_Dc);
		// restore orignal object
		Tv_Dc->SelectObject( m_FontOld );


		EndPaint( &Tv_Ps );
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcDimensionCalc( RECT* A_PtrRc )
{
	// local -------------------
		int		Tv_TxtHeight;
	// code --------------------
		m_RcItemArea.left			=	A_PtrRc->left;
		m_RcItemArea.top			=	A_PtrRc->top;
		m_RcItemArea.right		=	A_PtrRc->right;
		m_RcItemArea.bottom		=	A_PtrRc->bottom - m_UiStat.DescHeight;
		if ( m_RcItemArea.top >= m_RcItemArea.bottom )
		{
			m_RcItemArea.bottom	=	A_PtrRc->bottom;
		}

		m_RcDescArea.top			=	m_RcItemArea.bottom;
		m_RcDescArea.left			=	A_PtrRc->left;
		m_RcDescArea.right		=	A_PtrRc->right;
		m_RcDescArea.bottom		=	A_PtrRc->bottom;

		m_ItemHeight		=	0;
		m_ViewAbleCnt		=	0;

		Tv_TxtHeight		=	afxGlobalData.GetTextHeight(TRUE);
		
		if (0 != Tv_TxtHeight)
		{
			m_ItemHeight					=	Tv_TxtHeight + (E_GrMfcDockPropertyItemVtMargin * 2);
		}

		// item contents rect
		m_RcItemCont.left				=	m_RcItemArea.left + 1;
		m_RcItemCont.top				=	m_RcItemArea.top + 1;
		m_RcItemCont.right			=	m_RcItemArea.right - 1;
		m_RcItemCont.bottom			=	m_RcItemArea.bottom - 1;

		if(0 != m_ItemHeight)
		{
			m_ViewAbleCnt					=	(m_RcItemCont.bottom - m_RcItemCont.top) / m_ItemHeight;
#if 0
			if(0 != ((m_RcItemCont.bottom - m_RcItemCont.top) % m_ItemHeight))
			{
				m_ViewAbleCnt ++;
			}
#endif
			m_ViewNowCnt					=	m_ViewAbleCnt;
			if((m_ScrlPosVt + m_ViewAbleCnt) > m_ViewCnt)
			{
				m_ViewNowCnt		=	0;
				if (m_ScrlPosVt < m_ViewCnt)
				{
					m_ViewNowCnt		=	m_ViewCnt - m_ScrlPosVt;
				}
			}
		}

		// desc name area
		m_RcDescName.left		=	m_RcDescArea.left + 1;
		m_RcDescName.top		=	m_RcDescArea.top + 1;
		m_RcDescName.right	=	m_RcDescArea.right - 1;
		m_RcDescName.bottom	=	m_RcDescName.top;
		if(m_RcDescName.top < (m_RcDescArea.bottom - 1))
		{
			m_RcDescName.bottom	=	m_RcDescName.bottom + m_ItemHeight;
		}

		// desc comment area
		m_RcDescCmt.left		=	m_RcDescName.left;
		m_RcDescCmt.top			=	m_RcDescName.bottom;
		m_RcDescCmt.right		=	m_RcDescName.right;
		m_RcDescCmt.bottom	=	m_RcDescArea.bottom - 1;

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcDrawItems( CDC* A_Dc )
{
	// local -------------------
	// code --------------------
		// item area rect
		LcDrawCont( A_Dc );
		// border line
		A_Dc->Draw3dRect( &m_RcItemArea, afxGlobalData.clrActiveCaption, afxGlobalData.clrActiveCaption );
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::DrawLock( BOOL8 A_IsDrawAble )
{
	// local -------------------
	// code --------------------
		m_IsDrawAble	=	A_IsDrawAble;

		if ( A_IsDrawAble )
		{
			LcViewBuild();
			Invalidate();
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::ClearAll(void)
{
	// local -------------------
	// code --------------------

		// reset
		ClearItems();

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::ClearItems(void)
{
	// local -------------------
		int		Tv_ItmIdx;
	// code --------------------
		LcCtlEditFree();
		LcComboReqFree();

		if (0 != m_ItemCnt)
		{
			for(Tv_ItmIdx=0; Tv_ItmIdx < m_ItemCnt; Tv_ItmIdx++)
			{
				if(m_ItemTbl[Tv_ItmIdx].IsLocal)
				{
					if(NULL != m_ItemTbl[Tv_ItmIdx].ValInfo.PtrVal)
					{
						free(m_ItemTbl[Tv_ItmIdx].ValInfo.PtrVal);
						m_ItemTbl[Tv_ItmIdx].ValInfo.PtrVal	=	NULL;
					}
					if(NULL != m_ItemTbl[Tv_ItmIdx].StrName)
					{
						free(m_ItemTbl[Tv_ItmIdx].StrName);
						m_ItemTbl[Tv_ItmIdx].StrName	=	NULL;
					}
					if(NULL != m_ItemTbl[Tv_ItmIdx].StrDesc)
					{
						free(m_ItemTbl[Tv_ItmIdx].StrDesc);
						m_ItemTbl[Tv_ItmIdx].StrDesc	=	NULL;
					}
				}
			}
		}

		// reset
		m_ItemCnt				=	0;
		m_ScrlPosVt			=	0;
		m_ScrlMaxVt			=	0;
		GrDumyZeroMem(m_ItemTbl, sizeof(m_ItemTbl));
		for(Tv_ItmIdx = 0; Tv_ItmIdx < E_GrMfcDockPropertyItemMaxCnt; Tv_ItmIdx++)
		{
			m_ItemTbl[Tv_ItmIdx].Line				=	Tv_ItmIdx;
			m_ItemTbl[Tv_ItmIdx].ColorMark	=	afxGlobalData.clrBarFace;
		}

		LcViewBuild();

}
//--------------------------------------------------------------------
int	Cls_GrMfcDockProperty::ItemAddValue(WCHAR* A_StrName, __u16 A_ValType, void* A_PtrVal, __u16 A_ValCnt, BOOL8 A_IsEdit, WCHAR* A_StrDesc, __u32 A_TxtArrage, __u32 A_NameArrage, __u64 A_UserData)
{
	// local -------------------
		int	Tv_Result;
	// code --------------------
		Tv_Result	=	-1;

		if ( E_GrMfcDockPropertyItemMaxCnt > m_ItemCnt )
		{
			// reset item
			m_ItemTbl[m_ItemCnt].ValInfo.IsEdit			=	A_IsEdit;
			m_ItemTbl[m_ItemCnt].IsLocal						=	FALSE;
			m_ItemTbl[m_ItemCnt].ValInfo.IsModified	=	FALSE;
			m_ItemTbl[m_ItemCnt].ValInfo.PtrVal			=	A_PtrVal;
			m_ItemTbl[m_ItemCnt].UserData						=	0;
			m_ItemTbl[m_ItemCnt].ValInfo.ValCnt			=	A_ValCnt;
			m_ItemTbl[m_ItemCnt].ValInfo.ValType		=	A_ValType;
			m_ItemTbl[m_ItemCnt].IsMark				=	FALSE;
			m_ItemTbl[m_ItemCnt].ColorMark		=	afxGlobalData.clrBarFace;
			m_ItemTbl[m_ItemCnt].StrName			=	A_StrName;
			m_ItemTbl[m_ItemCnt].StrDesc			=	A_StrDesc;
			m_ItemTbl[m_ItemCnt].NameTxtArrange	=	A_NameArrage;
			m_ItemTbl[m_ItemCnt].IsSeparator				=	FALSE;
			m_ItemTbl[m_ItemCnt].IsFold							=	FALSE;
			m_ItemTbl[m_ItemCnt].UserData			=	A_UserData;

			// success
			Tv_Result	=	m_ItemCnt;
			m_ItemCnt ++;
			// redraw
			if ( m_IsDrawAble )
			{
				LcViewBuild();
				Invalidate();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int	Cls_GrMfcDockProperty::ItemAddLocalText(WCHAR* A_StrName, WCHAR* A_StrVal, WCHAR* A_StrDesc, __u32 A_TxtArrage, __u32 A_NameArrage)
{
	// local -------------------
		int	Tv_Result;
		__u32	Tv_LenTxt;
	// code --------------------
		Tv_Result	=	-1;

		if(E_GrMfcDockPropertyItemMaxCnt > m_ItemCnt)
		{
			// reset item
			m_ItemTbl[m_ItemCnt].ValInfo.IsEdit			=	FALSE;
			m_ItemTbl[m_ItemCnt].IsLocal						=	TRUE;
			m_ItemTbl[m_ItemCnt].ValInfo.IsModified	=	FALSE;
			m_ItemTbl[m_ItemCnt].ValInfo.PtrVal			=	NULL;
			m_ItemTbl[m_ItemCnt].UserData						=	0;
			m_ItemTbl[m_ItemCnt].ValInfo.ValCnt			=	0;
			m_ItemTbl[m_ItemCnt].ValInfo.ValType		=	E_GrVarTypeWstr;
			m_ItemTbl[m_ItemCnt].IsMark				=	FALSE;
			m_ItemTbl[m_ItemCnt].ColorMark		=	afxGlobalData.clrBarFace;
			m_ItemTbl[m_ItemCnt].NameTxtArrange	=	A_NameArrage;
			m_ItemTbl[m_ItemCnt].IsSeparator				=	FALSE;
			m_ItemTbl[m_ItemCnt].IsFold							=	FALSE;

			// name
			if(NULL != A_StrName)
			{
				// get length name
				Tv_LenTxt	=	GrStrWlen(A_StrName);
				if(0 != Tv_LenTxt)
				{
					m_ItemTbl[m_ItemCnt].StrName	=	(WCHAR*)malloc((Tv_LenTxt + 1) * 2);
					GrStrWcopy(m_ItemTbl[m_ItemCnt].StrName, A_StrName);
				}
			}
			// value
			if(NULL != A_StrVal)
			{
				// get length name
				Tv_LenTxt	=	GrStrWlen(A_StrVal);
				if(0 != Tv_LenTxt)
				{
					m_ItemTbl[m_ItemCnt].ValInfo.PtrVal	=	(void*)malloc((Tv_LenTxt + 1) * 2);
					GrStrWcopy((WCHAR*)m_ItemTbl[m_ItemCnt].ValInfo.PtrVal, A_StrVal);
					m_ItemTbl[m_ItemCnt].ValInfo.ValCnt	=	(__u16)Tv_LenTxt;
				}
			}
			// desc
			if(NULL != A_StrDesc)
			{
				// get length name
				Tv_LenTxt	=	GrStrWlen(A_StrDesc);
				if(0 != Tv_LenTxt)
				{
					m_ItemTbl[m_ItemCnt].StrDesc	=	(WCHAR*)malloc((Tv_LenTxt + 1) * 2);
					GrStrWcopy(m_ItemTbl[m_ItemCnt].StrDesc, A_StrDesc);
				}
			}

			// success
			Tv_Result	=	m_ItemCnt;
			m_ItemCnt ++;
			// redraw
			if(m_IsDrawAble)
			{
				LcViewBuild();
				Invalidate();
			}
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
int	Cls_GrMfcDockProperty::ItemAddSeparator(WCHAR* A_StrName, WCHAR* A_StrDesc, __u32 A_NameArrage)
{
	// local -------------------
		int	Tv_Result;
		__u32	Tv_LenTxt;
	// code --------------------
		Tv_Result	=	-1;

		if(E_GrMfcDockPropertyItemMaxCnt > m_ItemCnt)
		{
			// reset item
			m_ItemTbl[m_ItemCnt].ValInfo.IsEdit			=	FALSE;
			m_ItemTbl[m_ItemCnt].IsLocal						=	TRUE;
			m_ItemTbl[m_ItemCnt].ValInfo.IsModified	=	FALSE;
			m_ItemTbl[m_ItemCnt].ValInfo.PtrVal			=	NULL;
			m_ItemTbl[m_ItemCnt].UserData						=	0;
			m_ItemTbl[m_ItemCnt].ValInfo.ValCnt			=	0;
			m_ItemTbl[m_ItemCnt].ValInfo.ValType		=	E_GrVarTypeWstr;
			m_ItemTbl[m_ItemCnt].IsMark							=	FALSE;
			m_ItemTbl[m_ItemCnt].ColorMark					=	afxGlobalData.clrBarFace;
			m_ItemTbl[m_ItemCnt].NameTxtArrange			=	A_NameArrage;
			m_ItemTbl[m_ItemCnt].IsSeparator				=	TRUE;
			m_ItemTbl[m_ItemCnt].IsFold							=	FALSE;

			// name
			if(NULL != A_StrName)
			{
				// get length name
				Tv_LenTxt	=	GrStrWlen(A_StrName);
				if(0 != Tv_LenTxt)
				{
					m_ItemTbl[m_ItemCnt].StrName	=	(WCHAR*)malloc((Tv_LenTxt + 1) * 2);
					GrStrWcopy(m_ItemTbl[m_ItemCnt].StrName, A_StrName);
				}
			}
			// desc
			if(NULL != A_StrDesc)
			{
				// get length name
				Tv_LenTxt	=	GrStrWlen(A_StrDesc);
				if(0 != Tv_LenTxt)
				{
					m_ItemTbl[m_ItemCnt].StrDesc	=	(WCHAR*)malloc((Tv_LenTxt + 1) * 2);
					GrStrWcopy(m_ItemTbl[m_ItemCnt].StrDesc, A_StrDesc);
				}
			}

			// success
			Tv_Result	=	m_ItemCnt;
			m_ItemCnt ++;
			// redraw
			if(m_IsDrawAble)
			{
				LcViewBuild();
				Invalidate();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcDrawCont( CDC* A_Dc )
{
	// local -------------------
		int		Tv_ViewIdx;
		int		Tv_VsbIdx;
		RECT	Tv_RcItm;
	// code --------------------
		// draw start item
		Tv_ViewIdx				=	m_ScrlPosVt;
		Tv_RcItm.left			=	m_RcItemCont.left;
		Tv_RcItm.top			=	m_RcItemCont.top;
		Tv_RcItm.right		=	m_RcItemCont.right;
		Tv_RcItm.bottom		=	Tv_RcItm.top + m_ItemHeight;

		if (0 != m_ViewNowCnt)
		{
			for(Tv_VsbIdx=0; Tv_VsbIdx < m_ViewNowCnt; Tv_VsbIdx ++)
			{
				if(Tv_ViewIdx < m_ViewCnt)
				{
					// check draw able
					if(NULL != m_ViewTbl[Tv_ViewIdx])
					{
						LcDrawItem(A_Dc, &Tv_RcItm, m_ViewTbl[Tv_ViewIdx]->Line);
					}
				}
				// next
				Tv_ViewIdx ++;
				Tv_RcItm.top		=	Tv_RcItm.bottom;
				Tv_RcItm.bottom	=	Tv_RcItm.bottom + m_ItemHeight;
			}
		}

		// draw bottom space
		if(Tv_RcItm.top < m_RcItemCont.bottom)
		{
			Tv_RcItm.bottom	=	m_RcItemCont.bottom;
			if(m_ViewCnt > Tv_ViewIdx)
			{
				A_Dc->FillSolidRect(&Tv_RcItm, afxGlobalData.clrBtnShadow);
			}
			else
			{
				A_Dc->FillSolidRect(&Tv_RcItm, afxGlobalData.clrWindow);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcDrawItem( CDC* A_Dc, RECT* A_PtrRc, int A_ItemIdx )
{
	// local -------------------
		RECT		Tv_RcDraw;
		BOOL8		Tv_IsModified;
	// code --------------------
		// init
		Tv_IsModified			=	FALSE;
		A_Dc->SelectObject(afxGlobalData.fontRegular.m_hObject);

		// draw left gap
		Tv_RcDraw.left		=	A_PtrRc->left;
		Tv_RcDraw.top			=	A_PtrRc->top;
		Tv_RcDraw.right		=	A_PtrRc->left + E_GrMfcDockPropertyLeftGap;
		Tv_RcDraw.bottom	=	A_PtrRc->bottom;
		LcDrawLeftGap(A_Dc,&Tv_RcDraw,A_ItemIdx);
		
		// check separator
		if(m_ItemTbl[A_ItemIdx].IsSeparator)
		{
			// draw separator
			Tv_RcDraw.left		=	A_PtrRc->left + E_GrMfcDockPropertyLeftGap;
			Tv_RcDraw.top			=	A_PtrRc->top;
			Tv_RcDraw.right		=	A_PtrRc->right;
			Tv_RcDraw.bottom	=	A_PtrRc->bottom;
			LcDrawSeparator(A_Dc, &Tv_RcDraw,A_ItemIdx);
		}
		else
		{
			// item name
			Tv_RcDraw.left		=	A_PtrRc->left + E_GrMfcDockPropertyLeftGap;
			Tv_RcDraw.top			=	A_PtrRc->top;
			Tv_RcDraw.right		=	Tv_RcDraw.left + m_UiStat.NameWidth;
			Tv_RcDraw.bottom	=	A_PtrRc->bottom;
			if(Tv_RcDraw.left < Tv_RcDraw.right)
			{
				LcDrawName(A_Dc, &Tv_RcDraw, A_ItemIdx);
			}

			// item value
			Tv_RcDraw.left		=	A_PtrRc->left + E_GrMfcDockPropertyLeftGap + m_UiStat.NameWidth;
			Tv_RcDraw.top			=	A_PtrRc->top;
			Tv_RcDraw.right		=	A_PtrRc->right;
			Tv_RcDraw.bottom	=	A_PtrRc->bottom;
			if(Tv_RcDraw.left < Tv_RcDraw.right)
			{
				LcDrawValue(A_Dc, &Tv_RcDraw, A_ItemIdx);
			}

			// draw grid
			A_Dc->SelectObject((HGDIOBJ)m_PenCtSplit);
			A_Dc->MoveTo(A_PtrRc->left + E_GrMfcDockPropertyLeftGap + m_UiStat.NameWidth, A_PtrRc->top);
			A_Dc->LineTo(A_PtrRc->left + E_GrMfcDockPropertyLeftGap + m_UiStat.NameWidth, A_PtrRc->bottom - 1);

			// draw underline
			A_Dc->SelectObject((HGDIOBJ)m_PenCtUline);
			A_Dc->MoveTo(A_PtrRc->left + E_GrMfcDockPropertyLeftGap, A_PtrRc->bottom - 1);
			A_Dc->LineTo(A_PtrRc->right + E_GrMfcDockPropertyLeftGap, A_PtrRc->bottom - 1);
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcDrawLeftGap(CDC* A_Dc, RECT* A_PtrRc, int A_ItemIdx)
{
	// local -------------------
		int		Tv_Up;
		int		Tv_Left;
	// code --------------------
		// draw background
		A_Dc->FillSolidRect(A_PtrRc, afxGlobalData.clrBtnShadow);
		// check separator
		if(m_ItemTbl[A_ItemIdx].IsSeparator)
		{
			Tv_Left	=	A_PtrRc->left + 2;
			Tv_Up		=	A_PtrRc->top + (((A_PtrRc->bottom - A_PtrRc->top) - E_GrMfcDockPropertyFoldMarkSize) >> 1);
			// draw basic fold marker
			A_Dc->SelectObject(m_PenFoldMark);

			A_Dc->MoveTo(Tv_Left, Tv_Up);
			A_Dc->LineTo(Tv_Left + E_GrMfcDockPropertyFoldMarkSize - 1, Tv_Up);
			A_Dc->LineTo(Tv_Left + E_GrMfcDockPropertyFoldMarkSize - 1, Tv_Up + E_GrMfcDockPropertyFoldMarkSize -1);
			A_Dc->LineTo(Tv_Left, Tv_Up + E_GrMfcDockPropertyFoldMarkSize -1);
			A_Dc->LineTo(Tv_Left, Tv_Up);

			A_Dc->MoveTo(Tv_Left + 2, Tv_Up + (E_GrMfcDockPropertyFoldMarkSize >> 1));
			A_Dc->LineTo(Tv_Left + E_GrMfcDockPropertyFoldMarkSize - 2, Tv_Up + (E_GrMfcDockPropertyFoldMarkSize >> 1));

			// check folded
			if(m_ItemTbl[A_ItemIdx].IsFold)
			{
				A_Dc->MoveTo(Tv_Left + (E_GrMfcDockPropertyFoldMarkSize >> 1), Tv_Up + 2);
				A_Dc->LineTo(Tv_Left + (E_GrMfcDockPropertyFoldMarkSize >> 1), Tv_Up + E_GrMfcDockPropertyFoldMarkSize - 2);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcDrawSeparator(CDC* A_Dc, RECT* A_PtrRc, int A_ItemIdx)
{
	// local -------------------
	// code --------------------
		// draw background
		A_Dc->FillSolidRect(A_PtrRc, afxGlobalData.clrBtnShadow);

		A_Dc->SelectObject(afxGlobalData.fontBold.m_hObject);
		A_Dc->SetTextColor(afxGlobalData.clrBtnText);
		A_Dc->DrawText(m_ItemTbl[A_ItemIdx].StrName, A_PtrRc, m_ItemTbl[A_ItemIdx].NameTxtArrange | DT_SINGLELINE);

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcDrawName(CDC* A_Dc, RECT* A_PtrRc, int A_ItemIdx)
{
	// local -------------------
		RECT	Tv_RcTxt;
	// code --------------------
		// draw background
		A_Dc->FillSolidRect(A_PtrRc, afxGlobalData.clrBarFace);

		if(NULL != m_ItemTbl[A_ItemIdx].StrName)
		{
			Tv_RcTxt.left		=	A_PtrRc->left + E_GrMfcDockPropertyItemTxtHzGap;
			Tv_RcTxt.top		=	A_PtrRc->top;
			Tv_RcTxt.right	=	A_PtrRc->right - E_GrMfcDockPropertyItemTxtHzGap;
			Tv_RcTxt.bottom	=	A_PtrRc->bottom;
			A_Dc->SelectObject(afxGlobalData.fontRegular.m_hObject);
			A_Dc->SetTextColor(afxGlobalData.clrBarText);
			A_Dc->DrawText(m_ItemTbl[A_ItemIdx].StrName, &Tv_RcTxt, m_ItemTbl[A_ItemIdx].NameTxtArrange | DT_SINGLELINE);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcDrawValue(CDC* A_Dc, RECT* A_PtrRc, int A_ItemIdx)
{
	// local -------------------
		__u32	Tv_HndlItem;
		RECT	Tv_RcTxt;
	// code --------------------
		A_Dc->FillSolidRect(A_PtrRc, afxGlobalData.clrWindow);

		if(m_ItemTbl[A_ItemIdx].ValInfo.IsModified)
		{
			A_Dc->SelectObject(afxGlobalData.fontBold.m_hObject);
		}
		else
		{
			A_Dc->SelectObject(afxGlobalData.fontRegular.m_hObject);
		}
		A_Dc->SetTextColor(afxGlobalData.clrWindowText);

		Tv_RcTxt.left		=	A_PtrRc->left + E_GrMfcDockPropertyItemTxtHzGap;
		Tv_RcTxt.top		=	A_PtrRc->top;
		Tv_RcTxt.right	=	A_PtrRc->right - E_GrMfcDockPropertyItemTxtHzGap;
		Tv_RcTxt.bottom	=	A_PtrRc->bottom;

		Tv_HndlItem	=	ItemHndlEncode(A_ItemIdx, 1);
		DrawValue(A_Dc, &Tv_RcTxt, Tv_HndlItem);
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcDrawDescs(CDC* A_Dc)
{
	// local -------------------
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		// check draw able
		if( (m_RcDescArea.left < m_RcDescArea.right) && (m_RcDescArea.top < m_RcDescArea.bottom))
		{
			// get item info
			ItemHndlDecode(m_CusItemNow, &Tv_Line, &Tv_Colum);
			// draw name
			LcDrawDescName(A_Dc, Tv_Line);
			// draw comment
			LcDrawDescCmt(A_Dc, Tv_Line);
			// border line
			A_Dc->Draw3dRect(&m_RcDescArea, afxGlobalData.clrBtnShadow, afxGlobalData.clrBtnShadow);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcDrawDescName(CDC* A_Dc, int A_ItemIdx)
{
	// local -------------------
	// code --------------------
		// check draw able
		if((m_RcDescName.left < m_RcDescName.right) && (m_RcDescName.top < m_RcDescName.bottom))
		{
			// draw background
			A_Dc->FillSolidRect(&m_RcDescName, afxGlobalData.clrActiveCaption);

			if(0 <= A_ItemIdx)
			{
				if(NULL != m_ItemTbl[A_ItemIdx].StrName)
				{
					A_Dc->SelectObject(afxGlobalData.fontBold.m_hObject);
					A_Dc->SetTextColor(afxGlobalData.clrWindowText);
					A_Dc->DrawText(m_ItemTbl[A_ItemIdx].StrName, &m_RcDescName, m_DescNameTxtArrange | DT_SINGLELINE);
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcDrawDescCmt(CDC* A_Dc, int A_ItemIdx)
{
	// local -------------------
	// code --------------------
		// check draw able
		if((m_RcDescCmt.left < m_RcDescCmt.right) && (m_RcDescCmt.top < m_RcDescCmt.bottom))
		{
			// draw background
			A_Dc->FillSolidRect(&m_RcDescCmt, afxGlobalData.clrActiveCaption);

			if(0 <= A_ItemIdx)
			{
				if(NULL != m_ItemTbl[A_ItemIdx].StrDesc)
				{
					A_Dc->SelectObject(afxGlobalData.fontRegular.m_hObject);
					A_Dc->SetTextColor(afxGlobalData.clrWindowText);
					A_Dc->DrawText(m_ItemTbl[A_ItemIdx].StrDesc, &m_RcDescCmt, m_DescCmtTxtArrange | DT_WORDBREAK);
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcViewBuild(void)
{
	// local -------------------
		int		Tv_ItmIdx;
		BOOL8	Tv_IsFoldSkip;
	// code --------------------
		m_ViewCnt		=	0;

		if(0 != m_ItemCnt)
		{
			Tv_IsFoldSkip	=	FALSE;
			for( Tv_ItmIdx = 0; Tv_ItmIdx < m_ItemCnt; Tv_ItmIdx++)
			{
				// check fold skip
				if((!Tv_IsFoldSkip) || m_ItemTbl[Tv_ItmIdx].IsSeparator )
				{
					// add item
					m_ViewTbl[m_ViewCnt]	=	&m_ItemTbl[Tv_ItmIdx];
					m_ViewCnt ++;
					// check separator
					if(m_ItemTbl[Tv_ItmIdx].IsSeparator)
					{
						Tv_IsFoldSkip	=	m_ItemTbl[Tv_ItmIdx].IsFold;
					}
				}
			}
		}

		LcScrlbarVtUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcScrlbarVtUpdt(void)
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

		if ( 0 == m_ViewAbleCnt )
		{
			m_ViewAbleCnt ++;
		}
		if(m_ViewAbleCnt < m_ViewCnt)
		{
			Tv_MaxY	=	m_ViewCnt - m_ViewAbleCnt;
		}
		// update max
		SetScrollRange( SB_VERT, 0, Tv_MaxY );
		if ( Tv_MaxY < GetScrollPos( SB_VERT ) )
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
void Cls_GrMfcDockProperty::OnSize(UINT nType, int cx, int cy)
{
	// local -------------------
	// code --------------------
		Cls_GrMfcDockCtrlBase::OnSize(nType, cx, cy);
		LcScrlbarVtUpdt();
}
//--------------------------------------------------------------------
__u8	Cls_GrMfcDockProperty::LcPosInfoGet(int A_X, int A_Y, int* A_PtrRtItem, int* A_PtrRtColum, int* A_PtrRtValPosX, int* A_PtrRtValPosY)
{
	// local -------------------
		__u8	Tv_Result;
		int		Tv_RtValX;	// return value position x, -1 = name click
		int		Tv_RtValY;	// return value position y, -1 = name click
		int		Tv_RtLine;
		int		Tv_RtColum;
		int		Tv_VwIdx;
		RECT	Tv_RcCli;
		Ptr_GrMfcDockPropertyItemCtrl	Tv_PtrItm;
	// code --------------------
		// init
		Tv_Result		=	E_GrMfcDockCtrlBasePosOnNotting;

		Tv_RtValX		=	-1;
		Tv_RtValY		=	-1;
		Tv_RtColum	=	-1;
		Tv_RtLine		=	-1;

		// calculate dimemsion
		GetClientRect(&Tv_RcCli);
		LcDimensionCalc(&Tv_RcCli);

		// check area
		if((m_RcItemCont.top <= A_Y) && ((m_RcItemCont.bottom - E_GrMfcDockCtrlBaseSplitRng) >= A_Y))
		{
			if((0 != m_ItemHeight) && (m_RcItemCont.top < m_RcItemCont.bottom))
			{
				// check horizontal grid
				if(((m_RcItemCont.left + E_GrMfcDockPropertyLeftGap + m_UiStat.NameWidth - E_GrMfcDockCtrlBaseSplitRng) < A_X) &&
					((m_RcItemCont.left + E_GrMfcDockPropertyLeftGap + m_UiStat.NameWidth + E_GrMfcDockCtrlBaseSplitRng) > A_X))
				{
					Tv_RtValX		=	A_X - (m_RcItemCont.left + E_GrMfcDockPropertyLeftGap);
					Tv_Result		=	E_GrMfcDockCtrlBasePosOnGridSzRt;
				}

				// checkitem
				if(E_GrMfcDockCtrlBasePosOnNotting == Tv_Result)
				{
					// get view index
					Tv_VwIdx	=	(A_Y - m_RcItemCont.top) / m_ItemHeight;
					Tv_VwIdx	=	Tv_VwIdx + m_ScrlPosVt;
					// get view item
					if(m_ViewCnt > Tv_VwIdx)
					{
						Tv_PtrItm	=	m_ViewTbl[Tv_VwIdx];
						if(NULL != Tv_PtrItm)
						{
							Tv_RtLine	=	Tv_PtrItm->Line;
							// check separator
							if(Tv_PtrItm->IsSeparator)
							{
								// check fold mark
								if( (E_GrMfcDockPropertyLeftGap + m_RcItemCont.left ) > A_X)
								{
									Tv_Result		=	E_GrMfcDockCtrlBasePosOnFoldMark;
								}
								else
								{
									Tv_Result		=	E_GrMfcDockCtrlBasePosOnHeader;
								}
							}
							else
							{
								// get colum
								Tv_RtColum	=	0;
								if((m_UiStat.NameWidth + E_GrMfcDockPropertyLeftGap + m_RcItemCont.left) > A_X)
								{
									Tv_RtValX		=	A_X - (E_GrMfcDockPropertyLeftGap + m_RcItemCont.left);
								}
								else
								{
									Tv_RtValX		=	A_X - (E_GrMfcDockPropertyLeftGap + m_RcItemCont.left + m_UiStat.NameWidth);
									Tv_RtColum ++;
								}
								Tv_RtValY		=	(A_Y - m_RcItemCont.top) % m_ItemHeight;
								// success
								Tv_Result		=	E_GrMfcDockCtrlBasePosOnItem;
							}
						}
					}
				}
			}
		}

		// check desc grid
		if(E_GrMfcDockCtrlBasePosOnNotting == Tv_Result)
		{
			if(((m_RcDescArea.top - E_GrMfcDockCtrlBaseSplitRng) < A_Y) && ((m_RcDescArea.top + E_GrMfcDockCtrlBaseSplitRng) > A_Y))
			{
				Tv_RtValY		=	A_Y - m_RcDescArea.top;
				Tv_RtLine		=	E_GrMfcDockPropertyDescLineVal;
				Tv_Result		=	E_GrMfcDockCtrlBasePosOnGridSzUp;
			}
		}

		// check desc name
		if(E_GrMfcDockCtrlBasePosOnNotting == Tv_Result)
		{
			// check name
			if((m_RcDescName.top < m_RcDescName.bottom) && (m_RcDescName.top <= A_Y) && (m_RcDescName.bottom > A_Y) )
			{
				Tv_Result		=	E_GrMfcDockCtrlBasePosOnDescName;
			}
		}

		// check desc comment
		if(E_GrMfcDockCtrlBasePosOnNotting == Tv_Result)
		{
			// check name
			if((m_RcDescCmt.top < m_RcDescCmt.bottom) && (m_RcDescCmt.top <= A_Y) && (m_RcDescCmt.bottom > A_Y))
			{
				Tv_Result		=	E_GrMfcDockCtrlBasePosOnDescComment;
			}
		}

		// upate
		*A_PtrRtItem		=	Tv_RtLine;
		*A_PtrRtColum		=	Tv_RtColum;
		*A_PtrRtValPosX	=	Tv_RtValX;
		*A_PtrRtValPosY	=	Tv_RtValY;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrMfcDockProperty::ItemUserDataGetByIdx(int A_ItemIdx)
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( (0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) )
		{
			// get user data
			Tv_Result	=	m_ItemTbl[A_ItemIdx].UserData;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::ItemUserDataSetByIdx(int A_ItemIdx, __u64 A_UserData)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) )
		{
			m_ItemTbl[A_ItemIdx].UserData	=	A_UserData;
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrMfcDockProperty::ItemUserDataGetByHndl(__u32 A_HndlItem)
{
	// local -------------------
		__u64	Tv_Result;
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		Tv_Result	=	0;

		ItemHndlDecode(A_HndlItem, &Tv_Line, &Tv_Colum);
		if (0 <= Tv_Line)
		{
			Tv_Result	=	ItemUserDataGetByIdx(Tv_Line);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::ItemUserDataSetByHndl(__u32 A_HndlItem, __u64 A_UserData)
{
	// local -------------------
		BOOL8	Tv_Result;
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		Tv_Result	=	FALSE;

		ItemHndlDecode(A_HndlItem, &Tv_Line, &Tv_Colum);
		if (0 <= Tv_Line)
		{
			Tv_Result	=	ItemUserDataSetByIdx(Tv_Line, A_UserData);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::ItemSetMarkStatus(int A_ItemIdx, BOOL8 A_IsMark)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) )
		{
			m_ItemTbl[A_ItemIdx].IsMark	=	A_IsMark;
			// redraw item

			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::ItemSetMarkColor(int A_ItemIdx, COLORREF A_Color)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx))
		{
			m_ItemTbl[A_ItemIdx].ColorMark	=	A_Color;
			// redraw item

			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::IsItemMarked(int A_ItemIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx))
		{
			Tv_Result		=	m_ItemTbl[A_ItemIdx].IsMark;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int	Cls_GrMfcDockProperty::ItemCountGet(void)
{
		return	m_ItemCnt;
}
//--------------------------------------------------------------------
int		Cls_GrMfcDockProperty::ScrollPosVtGet(void)
{
	// local -------------------
	// code --------------------
		return	m_ScrlPosVt;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::ScrollPosVtSet(int A_ScrlPos)
{
	// local -------------------
	// code --------------------
		if ( 0 <= A_ScrlPos )
		{
			m_ScrlPosVt	=	A_ScrlPos;
			if(m_ScrlMaxVt < m_ScrlPosVt)
			{
				m_ScrlPosVt	=	m_ScrlMaxVt;
			}
			SetScrollPos(SB_VERT, m_ScrlPosVt);
			Invalidate(FALSE);
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::ItemDel(int A_ItemIdx)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx))
		{
			if((1 < m_ItemCnt) && (A_ItemIdx < (m_ItemCnt - 1)))
			{
				GrDumyCopyMem(&m_ItemTbl[A_ItemIdx], &m_ItemTbl[A_ItemIdx+1], sizeof(St_GrMfcDockPropertyItemCtrl)* (m_ItemCnt - A_ItemIdx - 1));
			}
			m_ItemCnt --;
		}

		LcViewBuild();
		Invalidate(FALSE);

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::ItemSetModifiedAll(BOOL8 A_IsModified)
{
	// local -------------------
		BOOL8	Tv_Result;
		int		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if(0 != m_ItemCnt)
		{
			for( Tv_WkIdx = 0; Tv_WkIdx < m_ItemCnt; Tv_WkIdx++)
			{
				m_ItemTbl[Tv_WkIdx].ValInfo.IsModified	=	A_IsModified;
			}
				
			Tv_Result	=	TRUE;
		}

		Invalidate(FALSE);

		return	Tv_Result;

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::LcUnfoldByItemIdx(int A_ItemIdx)
{
	// local -------------------
		int	Tv_SptIdx;
		int	Tv_ItmIdx;
		BOOL8	Tv_IsFold;
	// code --------------------
		if((0 <= A_ItemIdx) && (A_ItemIdx < m_ItemCnt))
		{
			// init
			Tv_IsFold	=	FALSE;
			Tv_SptIdx	=	-1;
			for( Tv_ItmIdx = 0; Tv_ItmIdx < m_ItemCnt; Tv_ItmIdx++)
			{
				// check now item
				if(A_ItemIdx == Tv_ItmIdx)
				{
					// found
					if(!m_ItemTbl[Tv_ItmIdx].IsSeparator)
					{
						if(Tv_IsFold && (0 <= Tv_SptIdx))
						{
							// do unfold
							m_ItemTbl[Tv_SptIdx].IsFold	=	FALSE;
							// redraw
							LcViewBuild();
						}
					}
					// finish
					break;
				}
				// next
				if(m_ItemTbl[Tv_ItmIdx].IsSeparator)
				{
					// update 
					Tv_IsFold	=	m_ItemTbl[Tv_ItmIdx].IsFold;
					Tv_SptIdx	=	Tv_ItmIdx;
				}
			}
		}

}
//--------------------------------------------------------------------
int	Cls_GrMfcDockProperty::LcViewIdxGetByItemIdx(int A_ItemIdx)
{
	// local -------------------
		int	Tv_Result;
		int	Tv_VwIdx;
	// code --------------------
		Tv_Result	=	-1;

		if(0 != m_ViewCnt)
		{
			for(Tv_VwIdx = 0; Tv_VwIdx < m_ViewCnt; Tv_VwIdx++)
			{
				if(NULL != m_ViewTbl[Tv_VwIdx])
				{
					if(m_ViewTbl[Tv_VwIdx]->Line == A_ItemIdx)
					{
						// found
						Tv_Result	=	Tv_VwIdx;
						break;
					}
					else if(m_ViewTbl[Tv_VwIdx]->Line > A_ItemIdx)
					{
						// not found
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::ScrollByIdx(int A_ItemIdx, BOOL8 A_IsForceUnfold)
{
	// local -------------------
		BOOL8	Tv_Result;
		int		Tv_VwIdx;			// view index
		RECT	Tv_RcCli;
	// code --------------------
		// init
		Tv_Result		=	FALSE;

		// check item range
		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx))
		{
			// dimension calculate
			GetClientRect(&Tv_RcCli);
			LcDimensionCalc(&Tv_RcCli);

			// force unfold
			if(A_IsForceUnfold)
			{
				LcUnfoldByItemIdx(A_ItemIdx);
			}

			// get view index
			Tv_VwIdx	=	LcViewIdxGetByItemIdx(A_ItemIdx);
			if(0 <= Tv_VwIdx)
			{
				// check scroll range
				if((m_ScrlPosVt > Tv_VwIdx) || ((m_ScrlPosVt + m_ViewAbleCnt) <= Tv_VwIdx))
				{
					// change scroll
					ScrollPosVtSet(Tv_VwIdx);
				}
				else if( (Tv_VwIdx + 1) == (m_ViewAbleCnt + m_ScrlPosVt) )
				{
					// check clipping
					if( 0 != ((m_RcItemCont.bottom - m_RcItemCont.top) % m_ItemHeight))
					{
						ScrollPosVtSet(m_ScrlPosVt + 1);
					}
				}
			}

			Tv_Result	=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
int	Cls_GrMfcDockProperty::LcVisibleIdxGetByItemIdx(int A_ItemIdx)
{
	// local -------------------
		int		Tv_Result;
		int		Tv_WkIdx;
		int		Tv_VwIdx;
	// code --------------------
		Tv_Result	=	-1;

		if(0 != m_ViewNowCnt)
		{
			Tv_VwIdx	=	m_ScrlPosVt;
			for( Tv_WkIdx = 0; Tv_WkIdx < m_ViewNowCnt; Tv_WkIdx++)
			{
				// check item exist
				if(NULL != m_ViewTbl[Tv_VwIdx])
				{
					if(A_ItemIdx == m_ViewTbl[Tv_VwIdx]->Line)
					{
						// found
						Tv_Result	=	Tv_WkIdx;
						break;
					}
				}
				// next
				Tv_VwIdx ++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::LcItemRectGetByIdx(int A_ItemIdx, int A_Colum, RECT* A_PtrRtRect)
{
	// local -------------------
		BOOL8	Tv_Result;
		RECT	Tv_RcCli;
		int		Tv_VsbIdx;			// visible index
	// code --------------------
		Tv_Result		=	FALSE;
		
		// check item range
		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx) && (0 <= A_Colum) && (2 > A_Colum))
		{
			// dimension calculate
			GetClientRect(&Tv_RcCli);
			LcDimensionCalc(&Tv_RcCli);
			// check item range
			Tv_VsbIdx	=	LcVisibleIdxGetByItemIdx(A_ItemIdx);
			if(0 <= Tv_VsbIdx)
			{
				// update virtical position
				A_PtrRtRect->top			=	m_RcItemCont.top + (m_ItemHeight * Tv_VsbIdx);
				A_PtrRtRect->bottom	=	A_PtrRtRect->top + m_ItemHeight;

				// get colum rect
				A_PtrRtRect->left		=	m_RcItemCont.left + E_GrMfcDockPropertyLeftGap;
				A_PtrRtRect->right		=	m_RcItemCont.right;
				if(0 == A_Colum)
				{
					A_PtrRtRect->right		=	m_RcItemCont.left + m_UiStat.NameWidth + E_GrMfcDockPropertyLeftGap;
				}
				else if(1 == A_Colum)
				{
					A_PtrRtRect->left		=	m_RcItemCont.left + m_UiStat.NameWidth + E_GrMfcDockPropertyLeftGap;
				}

				Tv_Result	=	TRUE;
			}
		}
		else if(E_GrMfcDockPropertyDescLineVal == A_ItemIdx)
		{
			// desc area
			*A_PtrRtRect	=	m_RcDescArea;
			Tv_Result			=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::ItemHndlDecode(__u32 A_HndlItem, int* A_PtrRtLine, int* A_PtrRtColum)
{
	// local -------------------
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		// init
		Tv_Line		=	-1;
		Tv_Colum	=	-1;

		if(E_GrMfcDockPropertyHndlItemNone != A_HndlItem)
		{
			Tv_Colum	=	(int)((A_HndlItem >> 16) & 0xFFFF) - E_GrMfcDockPropertyHndlElemBase;
			Tv_Line		=	(int)(A_HndlItem & 0xFFFF) - E_GrMfcDockPropertyHndlElemBase;
		}

		*A_PtrRtLine	=	Tv_Line;
		*A_PtrRtColum	=	Tv_Colum;
}
//--------------------------------------------------------------------
__u32	Cls_GrMfcDockProperty::ItemHndlEncode(int A_Line, int A_Colum)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		// init
		Tv_Result	=	E_GrMfcDockPropertyHndlItemNone;
		if(0 <= A_Line)
		{
			Tv_Result	=	Tv_Result | ((__u32)A_Line + E_GrMfcDockPropertyHndlElemBase);
			if(0 <= A_Colum)
			{
				Tv_Result	=	Tv_Result | (((__u32)A_Colum + E_GrMfcDockPropertyHndlElemBase) << 16);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrMfcDockCtrlBaseValueInfo	Cls_GrMfcDockProperty::ValueInfoGetByItemHndl(__u32 A_HndlItem)
{
	// local -------------------
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_Result;
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		// init
		Tv_Result	=	NULL;

		ItemHndlDecode(A_HndlItem, &Tv_Line, &Tv_Colum);
		if((0<= Tv_Line) && (1 == Tv_Colum))
		{
			// check item count
			if(m_ItemCnt > Tv_Line)
			{
				Tv_Result	=	&m_ItemTbl[Tv_Line].ValInfo;
			}
			/*
			else
			{
				DbgMsgPrint("Cls_GrMfcDockProperty::ValueInfoGetByItemHndl - bad handle value (%d)\n", A_HndlItem);
			}
			*/
		}
		/*
		else
		{
			DbgMsgPrint("Cls_GrMfcDockProperty::ValueInfoGetByItemHndl - bad handle value (%d)\n", A_HndlItem);
		}
		*/

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::ItemRectGetByItemHndl(__u32 A_HndlItem, RECT* A_PtrRtRect)
{
	// local -------------------
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		ItemHndlDecode(A_HndlItem, &Tv_Line, &Tv_Colum);
		return	LcItemRectGetByIdx(Tv_Line, Tv_Colum, A_PtrRtRect);
}
//--------------------------------------------------------------------
__u8	Cls_GrMfcDockProperty::PosInfoGet(int A_X, int A_Y, __u32* A_PtrRtHndlItem, int* A_PtrRtItemPosX, int* A_PtrRtItemPosY)
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
int*	Cls_GrMfcDockProperty::GridSizeValuePtrGet(__u32 A_HndlItem, __u8 A_Mode)
{
	// local -------------------
		int*	Tv_Result;
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		Tv_Result	=	NULL;

		ItemHndlDecode(A_HndlItem, &Tv_Line, &Tv_Colum);
		if(E_GrMfcDockCtrlBaseGridSizeUp == A_Mode)
		{
			Tv_Result	=	&m_UiStat.DescHeight;
		}
		else if(E_GrMfcDockCtrlBaseGridSizeRt == A_Mode)
		{
			Tv_Result	=	&m_UiStat.NameWidth;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::OnGridSizeChange(void)
{
	// local -------------------
		BOOL8	Tv_IsRedraw;
	// code --------------------
		// init
		Tv_IsRedraw		=	FALSE;
		// check change value
		if(30 > m_UiStat.NameWidth)
		{
			m_UiStat.NameWidth	=	30;
			Tv_IsRedraw		=	TRUE;
		}
		if(20 > m_UiStat.DescHeight)
		{
			m_UiStat.DescHeight	=	20;
			Tv_IsRedraw		=	TRUE;
		}
		if((m_RcItemArea.bottom - m_RcItemArea.top) < (m_UiStat.DescHeight + 15))
		{
			m_UiStat.DescHeight	=	(m_RcItemArea.bottom - m_RcItemArea.top) - 15;
			if(0 >= m_UiStat.DescHeight)
			{
				m_UiStat.DescHeight	=	1;
			}
			Tv_IsRedraw		=	TRUE;
		}

		LcScrlbarVtUpdt();

		if(Tv_IsRedraw)
		{
			Invalidate(FALSE);
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::ScrollByItemHndl(__u32 A_HndlItem)
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
			Tv_Result	=	ScrollByIdx(Tv_Line, FALSE);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::OnFoldClick(__u32 A_HndlItem)
{
	// local -------------------
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		ItemHndlDecode(A_HndlItem, &Tv_Line, &Tv_Colum);
		if(0 <= Tv_Line)
		{
			m_ItemTbl[Tv_Line].IsFold		=	!m_ItemTbl[Tv_Line].IsFold;
			LcViewBuild();
			Invalidate(FALSE);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockProperty::OnItemLbtnDown(__u32 A_HndlItem)
{
	// local -------------------
	// code --------------------
		// redraw description
		if(E_GrMfcDockPropertyHndlItemNone != A_HndlItem)
		{
			InvalidateRect(&m_RcDescArea);
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::ItemValTypeSetByIdx(int A_ItemIdx, __u16 A_ValType, __u16 A_ValCnt)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if((0 <= A_ItemIdx) && (m_ItemCnt > A_ItemIdx))
		{
			m_ItemTbl[A_ItemIdx].ValInfo.ValType	=	A_ValType;
			m_ItemTbl[A_ItemIdx].ValInfo.ValCnt		=	A_ValCnt;

			Tv_Result		=	TRUE;

			// redraw
			if(m_IsDrawAble)
			{
				LcViewBuild();
				Invalidate();
			}
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockProperty::ItemValTypeSetByHndl(__u32 A_HndlItem, __u16 A_ValType, __u16 A_ValCnt)
{
	// local -------------------
		BOOL8	Tv_Result;
		int		Tv_Line;
		int		Tv_Colum;
	// code --------------------
		Tv_Result	=	FALSE;

		ItemHndlDecode(A_HndlItem, &Tv_Line, &Tv_Colum);
		if(0 <= Tv_Line)
		{
			Tv_Result	=	ItemValTypeSetByIdx(Tv_Line, A_ValType,A_ValCnt);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
