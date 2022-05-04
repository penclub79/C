/*
	grid value

*/
#include <Win\MfcDock\GrMfcDockCtrlBase.h>

#include <afxcontrolbars.h>
#include <afxvisualmanager.h>

#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrStrTool.h>
#include <GrDrawTool.h>

#include <Win\MfcDock\GrMfcDockPaneBase.h>


//--------------------------------------------------------------------
// local constant


//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var
//extern	St_P4Base	V_P4Base;


WCHAR	V_GrMfcDockCtrlBaseStrTrue[]	=	L"true";
WCHAR	V_GrMfcDockCtrlBaseStrFalse[]	=	L"false";

WCHAR	V_GrMfcDockCtrlBaseStrLeft[]	=	L"left";
WCHAR	V_GrMfcDockCtrlBaseStrRight[]	=	L"right";
WCHAR	V_GrMfcDockCtrlBaseStrCenter[]	=	L"center";
WCHAR	V_GrMfcDockCtrlBaseStrTop[]	=	L"top";
WCHAR	V_GrMfcDockCtrlBaseStrBottom[]	=	L"bottom";

WCHAR	V_GrMfcDockCtrlBaseStrUp[]	=	L"up";
WCHAR	V_GrMfcDockCtrlBaseStrDown[]	=	L"down";

WCHAR	V_GrMfcDockCtrlBaseStr1byte[]	=	L"1byte";
WCHAR	V_GrMfcDockCtrlBaseStr2byte[]	=	L"2byte";
WCHAR	V_GrMfcDockCtrlBaseStr4byte[]	=	L"4byte";


//====================================================================
//--------------------------------------------------------------------
Cls_GrMfcDockCtrlBase::Cls_GrMfcDockCtrlBase(  ):
CWnd(  )
{
	// local -------------------
	// code --------------------
		// init
		m_IsDblclkEdit	=	FALSE;
		m_CusItemNow		=	0;

		m_IsMseLbtnDown	=	FALSE;

		m_ScrlPosHz			=	0;
		m_ScrlMaxHz			=	0;
		m_ScrlPosVt			=	0;
		m_ScrlMaxVt			=	0;


		m_ObjEdit				=	NULL;
		m_EditItemHndl	=	0;
		m_IsEditAbleChgNoty	=	TRUE;

		m_ObjCombo			=	NULL;
		m_ComboItemHndl	=	0;
		m_ComboFreeReq	=	0;

		m_PopupCnt			=	0;
		m_PopupItemHndl	=	0;
		GrDumyZeroMem(m_PopupCtrl, sizeof(m_PopupCtrl));

		m_PxFmt					=	E_GrFccRGB888;

		m_GridSizeChgMode		=	E_GrMfcDockCtrlBaseGridSizeLt;
		m_GridSizeChgBasis	=	0;
		m_PtrGridSizeVal		=	NULL;

		// cursor 
		m_CusArrow	=	AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		m_CusSizeHz	=	AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
		m_CusSizeVt	=	AfxGetApp()->LoadStandardCursor(IDC_SIZENS);

		m_CusNow		=	m_CusArrow;

		GrStrClear(m_StrRgtyUiStatKey);

		// resource
		m_PenSubGrid	=	CreatePen(PS_SOLID, 1,afxGlobalData.clrInactiveBorder );

		// menu
		m_PopupIn.CreatePopupMenu();
		m_PopupOut.CreatePopupMenu();
}
//--------------------------------------------------------------------
Cls_GrMfcDockCtrlBase::~Cls_GrMfcDockCtrlBase( void )
{
	// local -------------------
	// code --------------------
		LcComboDoFree();
		LcCtlEditFree();

		m_PopupIn.DestroyMenu();
		m_PopupOut.DestroyMenu();

		DeleteObject((HGDIOBJ)m_PenSubGrid);
}
//--------------------------------------------------------------------
BEGIN_MESSAGE_MAP(Cls_GrMfcDockCtrlBase, CWnd)
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_EN_CHANGE( E_GrMfcDockCtrlBaseRsrcIdEdit, Cls_GrMfcDockCtrlBase::OnEditChange )
	ON_EN_KILLFOCUS( E_GrMfcDockCtrlBaseRsrcIdEdit, Cls_GrMfcDockCtrlBase::OnEditKillFocus ) 
	ON_CBN_SELCHANGE( E_GrMfcDockCtrlBaseRsrcIdCombo, Cls_GrMfcDockCtrlBase::OnCbxSelChange )
	ON_CBN_KILLFOCUS( E_GrMfcDockCtrlBaseRsrcIdCombo, Cls_GrMfcDockCtrlBase::OnCbxKillFocus )
	ON_MESSAGE( E_GrMfcDockCtrlBaseMsgCbxFree, Cls_GrMfcDockCtrlBase::OnCbxFree )
	ON_COMMAND_RANGE(E_GrMfcDockCtrlBasePopupIdBase, (E_GrMfcDockCtrlBasePopupIdBase + E_GrMfcDockCtrlBasePopupMaxCnt - 1), &Cls_GrMfcDockCtrlBase::OnPopmenu)
END_MESSAGE_MAP()
//--------------------------------------------------------------------
BOOL Cls_GrMfcDockCtrlBase::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
	const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// local -------------------
		BOOL	Tv_Result;
	// code --------------------
		Tv_Result	=	CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
		CtrlRgtySubkeyGet(m_StrRgtyUiStatKey);
		return	Tv_Result;
}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnSize(UINT nType, int cx, int cy)
{
		CWnd::OnSize(nType, cx, cy);

		LcComboReqFree();
		LcCtlEditFree();

}
//--------------------------------------------------------------------
BOOL Cls_GrMfcDockCtrlBase::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
{
	// local -------------------
	// code --------------------
		SetCursor( m_CusNow );
		return	TRUE;
}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnMouseMove(UINT nFlags, CPoint point)
{
	// local -------------------
		RECT	Tv_RcCli;
		int		Tv_PosX;
		int		Tv_PosY;
		int		Tv_GridSize;
		__u8	Tv_RtCode;
		__u32	Tv_HndlItem;
	// code --------------------
		//CWnd::OnMouseMove(nFlags, point);

		GetClientRect( &Tv_RcCli );
		// check mouse position
		if ((NULL != m_PtrGridSizeVal) && (NULL == m_ObjEdit) && (NULL == m_ObjCombo) )
		{
			// check mode
			Tv_GridSize	=	0;
			switch(m_GridSizeChgMode)
			{
				case	E_GrMfcDockCtrlBaseGridSizeLt:
					Tv_GridSize	=	m_GridSizeChgBasis - point.x;
					break;
				case	E_GrMfcDockCtrlBaseGridSizeUp:
					Tv_GridSize	=	m_GridSizeChgBasis - point.y;
					break;
				case	E_GrMfcDockCtrlBaseGridSizeRt:
					Tv_GridSize	=	point.x - m_GridSizeChgBasis;
					break;
				case	E_GrMfcDockCtrlBaseGridSizeDn:
					Tv_GridSize	=	point.y - m_GridSizeChgBasis;
					break;
			}
			// update value
			if(E_GrMfcDockCtrlBaseSizeMin > Tv_GridSize)
			{
				Tv_GridSize	=	E_GrMfcDockCtrlBaseSizeMin;
			}
			*m_PtrGridSizeVal	=	Tv_GridSize;

			Invalidate();
		}
		else
		{
			// init cursor
			m_CusNow	=	m_CusArrow;

			// check item area
			Tv_RtCode	=	PosInfoGet(point.x, point.y, &Tv_HndlItem, &Tv_PosX, &Tv_PosY);
			if((E_GrMfcDockCtrlBasePosOnGridSzLt == Tv_RtCode) || (E_GrMfcDockCtrlBasePosOnGridSzRt == Tv_RtCode))
			{
				m_CusNow	=	m_CusSizeHz;
			}
			else if((E_GrMfcDockCtrlBasePosOnGridSzUp == Tv_RtCode) || (E_GrMfcDockCtrlBasePosOnGridSzDn == Tv_RtCode))
			{
				m_CusNow	=	m_CusSizeVt;
			}
		}
}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	// local -------------------
		__u32	Tv_HndlItem;
		int		Tv_PosX;
		int		Tv_PosY;
		__u8	Tv_RtCode;
		RECT	Tv_RcItm;
	// code --------------------
		CWnd::OnLButtonDown( nFlags, point );

		m_IsMseLbtnDown	=	TRUE;

		Tv_RtCode	=	PosInfoGet(point.x, point.y, &Tv_HndlItem, &Tv_PosX, &Tv_PosY);
		switch(Tv_RtCode)
		{
			case E_GrMfcDockCtrlBasePosOnGridSzLt:
				if(ItemRectGetByItemHndl(Tv_HndlItem, &Tv_RcItm))
				{
					m_PtrGridSizeVal		=	GridSizeValuePtrGet(Tv_HndlItem, E_GrMfcDockCtrlBaseGridSizeLt);
					if(NULL != m_PtrGridSizeVal)
					{
						LcCtlEditFree();
						LcComboReqFree();
						m_GridSizeChgMode		=	E_GrMfcDockCtrlBaseGridSizeLt;
						m_GridSizeChgBasis	=	(point.x - Tv_PosX) + (Tv_RcItm.right - Tv_RcItm.left);
						SetCapture();
					}
				}
				break;
			case E_GrMfcDockCtrlBasePosOnGridSzUp:
				if(ItemRectGetByItemHndl(Tv_HndlItem, &Tv_RcItm))
				{
					m_PtrGridSizeVal		=	GridSizeValuePtrGet(Tv_HndlItem, E_GrMfcDockCtrlBaseGridSizeUp);
					if(NULL != m_PtrGridSizeVal)
					{
						LcCtlEditFree();
						LcComboReqFree();
						m_GridSizeChgMode		=	E_GrMfcDockCtrlBaseGridSizeUp;
						m_GridSizeChgBasis	=	(point.y - Tv_PosX) + (Tv_RcItm.bottom - Tv_RcItm.top);
						SetCapture();
					}
				}
				break;
			case E_GrMfcDockCtrlBasePosOnGridSzRt:
				m_PtrGridSizeVal		=	GridSizeValuePtrGet(Tv_HndlItem, E_GrMfcDockCtrlBaseGridSizeRt);
				if(NULL != m_PtrGridSizeVal)
				{
					LcCtlEditFree();
					LcComboReqFree();
					m_GridSizeChgMode		=	E_GrMfcDockCtrlBaseGridSizeRt;
					m_GridSizeChgBasis	=	point.x - Tv_PosX;
					SetCapture();
				}
				break;
			case E_GrMfcDockCtrlBasePosOnGridSzDn:
				m_PtrGridSizeVal		=	GridSizeValuePtrGet(Tv_HndlItem, E_GrMfcDockCtrlBaseGridSizeDn);
				if(NULL != m_PtrGridSizeVal)
				{
					LcCtlEditFree();
					LcComboReqFree();
					m_GridSizeChgMode		=	E_GrMfcDockCtrlBaseGridSizeDn;
					m_GridSizeChgBasis	=	point.y - Tv_PosY;
					SetCapture();
				}
				break;
			default:
				m_CusItemNow			=	Tv_HndlItem;
				m_PtrGridSizeVal	=	NULL;
				OnItemLbtnDown(Tv_HndlItem);
				break;
		}

}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	// local -------------------
		__u32	Tv_HndlItem;
		int		Tv_PosX;
		int		Tv_PosY;
		__u8	Tv_RtCode;
	// code --------------------
		CWnd::OnLButtonUp( nFlags, point );

		if(m_IsMseLbtnDown)
		{
			SetFocus();

			if(NULL != m_PtrGridSizeVal)
			{
				ReleaseCapture();
				m_PtrGridSizeVal	=	NULL;
				m_CusNow	=	m_CusArrow;

				// do event
				OnGridSizeChange();
			}
			else
			{
				// item click check
				Tv_RtCode	=	PosInfoGet(point.x, point.y, &Tv_HndlItem, &Tv_PosX, &Tv_PosY);
				if(E_GrMfcDockCtrlBasePosOnItem == Tv_RtCode)
				{
					if(m_IsDblclkEdit)
					{
						LcDoEvtClick(Tv_HndlItem);
					}
					else
					{
						if(!EditValue(Tv_HndlItem, Tv_PosX, Tv_PosY))
						{
							LcDoEvtClick(Tv_HndlItem);
						}
					}
				}
				else if(E_GrMfcDockCtrlBasePosOnFoldMark == Tv_RtCode)
				{
					OnFoldClick(Tv_HndlItem);
				}
			}

			m_IsMseLbtnDown	=	FALSE;
		}
		
}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnRButtonUp(UINT nFlags, CPoint point)
{
	// local -------------------
		__u32	Tv_HndlItem;
		int		Tv_PosX;
		int		Tv_PosY;
		POINT		Tv_PosScr;
		__u8	Tv_RtCode;
	// code --------------------
		//CWnd::OnRButtonUp(nFlags, point);

		GetCursorPos(&Tv_PosScr);

		Tv_RtCode	=	PosInfoGet(point.x, point.y, &Tv_HndlItem, &Tv_PosX, &Tv_PosY);
		if(E_GrMfcDockCtrlBasePosOnItem == Tv_RtCode)
		{
			// in menu
			m_PopupIn.TrackPopupMenu(TPM_LEFTALIGN, Tv_PosScr.x, Tv_PosScr.y, this);
			m_PopupItemHndl	=	Tv_HndlItem;
		}
		else
		{
			// out menu
			m_PopupOut.TrackPopupMenu(TPM_LEFTALIGN, Tv_PosScr.x, Tv_PosScr.y, this);
			m_PopupItemHndl	=	Tv_HndlItem;
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::DrawValue(CDC* A_Dc, RECT* A_PtrRc, __u32 A_HndlItem)
{
	// local -------------------
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrValInfo;
		int		Tv_ValInt;
		DWORD	Tv_ValDword;
		BOOL8	Tv_ValBool8;
		__u8	Tv_BitPos;
		__u64	Tv_ValU64;
		__u32	Tv_Len;
	// code --------------------

		// get value info
		Tv_PtrValInfo	=	ValueInfoGetByItemHndl(A_HndlItem);

		// check value exist
		if(NULL != Tv_PtrValInfo)
		{
			if(NULL != Tv_PtrValInfo->PtrVal)
			{
				// init
				Tv_BitPos		=	0;
				// check value type
				switch(Tv_PtrValInfo->ValType)
				{
					case E_GrVarTypeS8:
						Tv_ValInt	=	(int)(*((char*)Tv_PtrValInfo->PtrVal));
						LcDrawValInt(A_Dc, A_PtrRc, Tv_ValInt, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeS16:
						Tv_ValInt	=	(int)(*((short*)Tv_PtrValInfo->PtrVal));
						LcDrawValInt(A_Dc, A_PtrRc, Tv_ValInt, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeS32:
						Tv_ValInt	=	(*((int*)Tv_PtrValInfo->PtrVal));
						LcDrawValInt(A_Dc, A_PtrRc, Tv_ValInt, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeU8:
						Tv_ValDword	=	(DWORD)(*((__u8*)Tv_PtrValInfo->PtrVal));
						LcDrawValDword(A_Dc, A_PtrRc, Tv_ValDword, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeU16:
						Tv_ValDword	=	(DWORD)(*((WORD*)Tv_PtrValInfo->PtrVal));
						LcDrawValDword(A_Dc, A_PtrRc, Tv_ValDword, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeU32:
						Tv_ValDword	=	(*((DWORD*)Tv_PtrValInfo->PtrVal));
						LcDrawValDword(A_Dc, A_PtrRc, Tv_ValDword, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeHexU8:
						Tv_ValU64	=	(__u64)(*((__u8*)Tv_PtrValInfo->PtrVal));
						LcDrawValHex(A_Dc, A_PtrRc, Tv_ValU64, 2, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeHexU16:
						Tv_ValU64	=	(__u64)(*((__u16*)Tv_PtrValInfo->PtrVal));
						LcDrawValHex(A_Dc, A_PtrRc, Tv_ValU64, 4, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeHexU32:
						Tv_ValU64	=	(__u64)(*((__u32*)Tv_PtrValInfo->PtrVal));
						LcDrawValHex(A_Dc, A_PtrRc, Tv_ValU64, 8, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeHexU64:
						Tv_ValU64	=	(*((__u64*)Tv_PtrValInfo->PtrVal));
						LcDrawValHex(A_Dc, A_PtrRc, Tv_ValU64, 16, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeBitBoolBit7:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit6:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit5:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit4:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit3:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit2:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit1:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit0:
						Tv_ValBool8	=	FALSE;
						if(0 != ((*((__u8*)Tv_PtrValInfo->PtrVal)) & (1 << Tv_BitPos)))
						{
							Tv_ValBool8	=	TRUE;
						}
						LcDrawValBool8(A_Dc, A_PtrRc, Tv_ValBool8, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeBool8:
						Tv_ValBool8	=	*((BOOL8*)Tv_PtrValInfo->PtrVal);
						LcDrawValBool8(A_Dc, A_PtrRc, Tv_ValBool8, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeWstr:
						Tv_Len	=	GrStrWlen((WCHAR*)Tv_PtrValInfo->PtrVal, Tv_PtrValInfo->ValCnt);
						LcDrawValWchar(A_Dc, A_PtrRc, (WCHAR*)Tv_PtrValInfo->PtrVal, (__u16)Tv_Len,
							Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeWchar:
						LcDrawValWchar(A_Dc, A_PtrRc, (WCHAR*)Tv_PtrValInfo->PtrVal, 1, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeColor:
						Tv_ValDword	=	(*((DWORD*)Tv_PtrValInfo->PtrVal));
						LcDrawValColor(A_Dc, A_PtrRc, Tv_ValDword, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypePallete:
						Tv_ValDword	=	(*((DWORD*)Tv_PtrValInfo->PtrVal));
						LcDrawValPallete(A_Dc, A_PtrRc, Tv_ValDword, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeStr:
						LcDrawValStr(A_Dc, A_PtrRc, (char*)Tv_PtrValInfo->PtrVal, Tv_PtrValInfo->ValCnt, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeIpV4:
						Tv_ValDword	=	(*((DWORD*)Tv_PtrValInfo->PtrVal));
						LcDrawValIpV4(A_Dc, A_PtrRc, (__u32)Tv_ValDword, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeTime:
						Tv_ValDword	=	(*((DWORD*)Tv_PtrValInfo->PtrVal));
						LcDrawValTime(A_Dc, A_PtrRc, (__u32)Tv_ValDword, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeBit0U8:
						LcDrawValBitmap(A_Dc, A_PtrRc, (__u8*)Tv_PtrValInfo->PtrVal, Tv_PtrValInfo->ValCnt,afxGlobalData.clrHilite );
						break;
					case E_GrVarTypeBit1U8:
						LcDrawValBitmap(A_Dc, A_PtrRc, (__u8*)Tv_PtrValInfo->PtrVal, Tv_PtrValInfo->ValCnt, afxGlobalData.clrGrayedText);
						break;
					case E_GrVarTypeAlignHz:
						Tv_BitPos	=	*((__u8*)Tv_PtrValInfo->PtrVal);
						LcDrawValAlighHz(A_Dc, A_PtrRc, Tv_BitPos, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeAlignVt:
						Tv_BitPos	=	*((__u8*)Tv_PtrValInfo->PtrVal);
						LcDrawValAlighVt(A_Dc, A_PtrRc, Tv_BitPos, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeDirection:
						Tv_BitPos	=	*((__u8*)Tv_PtrValInfo->PtrVal);
						LcDrawValDirection(A_Dc, A_PtrRc, Tv_BitPos, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeVarSize:
						Tv_BitPos	=	*((__u8*)Tv_PtrValInfo->PtrVal);
						LcDrawValVarSize(A_Dc, A_PtrRc, Tv_BitPos, Tv_PtrValInfo->TxtArrange);
						break;
					case E_GrVarTypeMacAdr:
						LcDrawValMacAdr(A_Dc, A_PtrRc, (__u8*)Tv_PtrValInfo->PtrVal, Tv_PtrValInfo->TxtArrange);
						break;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValInt(CDC* A_Dc, RECT* A_PtrRc, int A_Value, __u32 A_TxtArrange)
{
	// local -------------------
		Def_WstrNum	Tv_StrTxt;
		RECT	Tv_RcTxt;
	// code --------------------
		GrStrIntToWstr( Tv_StrTxt, A_Value );
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right - E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		if ( Tv_RcTxt.left < Tv_RcTxt.right )
		{
			A_Dc->SetTextColor( afxGlobalData.clrWindowText );
			A_Dc->DrawText(Tv_StrTxt, &Tv_RcTxt, DT_SINGLELINE | A_TxtArrange);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValDword(CDC* A_Dc, RECT* A_PtrRc, DWORD A_Value, __u32 A_TxtArrange)
{
	// local -------------------
		Def_WstrNum	Tv_StrTxt;
		RECT	Tv_RcTxt;
	// code --------------------
		GrStrIntToWstr( Tv_StrTxt, (int)A_Value, 0, FALSE, FALSE );
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right - E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		if ( Tv_RcTxt.left < Tv_RcTxt.right )
		{
			A_Dc->SetTextColor( afxGlobalData.clrWindowText );
			A_Dc->DrawText(Tv_StrTxt, &Tv_RcTxt, DT_SINGLELINE | A_TxtArrange);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValHex(CDC* A_Dc, RECT* A_PtrRc, __u64 A_Value, __u32 A_Digit, __u32 A_TxtArrange)
{
	// local -------------------
		Def_WstrTag	Tv_StrTxt;
		RECT	Tv_RcTxt;
	// code --------------------
		GrStrQwordToHexWstr( Tv_StrTxt, A_Value, A_Digit, TRUE );
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right - E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		if ( Tv_RcTxt.left < Tv_RcTxt.right )
		{
			A_Dc->SetTextColor( afxGlobalData.clrWindowText );
			A_Dc->DrawText(Tv_StrTxt, &Tv_RcTxt, DT_SINGLELINE | A_TxtArrange);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValBool8(CDC* A_Dc, RECT* A_PtrRc, BOOL8 A_IsVal, __u32 A_TxtArrange)
{
	// local -------------------
		WCHAR*	Tv_StrTxt;
		RECT	Tv_RcTxt;
	// code --------------------
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right - E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		Tv_StrTxt	=	V_GrMfcDockCtrlBaseStrFalse;
		if ( A_IsVal )
		{
			Tv_StrTxt	=	V_GrMfcDockCtrlBaseStrTrue;
		}

		if ( Tv_RcTxt.left < Tv_RcTxt.right )
		{
			A_Dc->SetTextColor( afxGlobalData.clrWindowText );
			A_Dc->DrawText(Tv_StrTxt, &Tv_RcTxt, DT_SINGLELINE | A_TxtArrange);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValWchar(CDC* A_Dc, RECT* A_PtrRc, WCHAR* A_StrVal, __u16 A_MaxLen, __u32 A_TxtArrange)
{
	// local -------------------
		RECT	Tv_RcTxt;
	// code --------------------
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		if ( (Tv_RcTxt.left < Tv_RcTxt.right) && (GrStrIsExist(A_StrVal)) )
		{
			A_Dc->SetTextColor( afxGlobalData.clrWindowText );
			A_Dc->DrawText(A_StrVal, (int)A_MaxLen, &Tv_RcTxt, DT_SINGLELINE | DT_NOPREFIX | A_TxtArrange);
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValStr(CDC* A_Dc, RECT* A_PtrRc, char* A_StrVal, __u16 A_MaxLen, __u32 A_TxtArrange)
{
	// local -------------------
		RECT	Tv_RcTxt;
		Def_WstrShort	Tv_StrTxt;
		UINT	Tv_Fmt;
	// code --------------------
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		if((Tv_RcTxt.left < Tv_RcTxt.right) && (GrStrIsExist(A_StrVal)))
		{
			GrStrStrToWstr(Tv_StrTxt, A_StrVal);
			A_Dc->SetTextColor(afxGlobalData.clrWindowText);
			Tv_Fmt	=	A_TxtArrange | DT_SINGLELINE | DT_NOPREFIX;
			A_Dc->DrawText(Tv_StrTxt, &Tv_RcTxt, Tv_Fmt);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValIpV4(CDC* A_Dc, RECT* A_PtrRc, __u32 A_IpV4, __u32 A_TxtArrange)
{
	// local -------------------
		RECT	Tv_RcTxt;
		Def_WstrTag	Tv_StrTxt;
	// code --------------------
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right - E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		if(Tv_RcTxt.left < Tv_RcTxt.right)
		{
			GrStrIpToWstr(Tv_StrTxt, A_IpV4);
			A_Dc->SetTextColor(afxGlobalData.clrWindowText);
			A_Dc->DrawText(Tv_StrTxt, &Tv_RcTxt, DT_SINGLELINE | A_TxtArrange);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValTime(CDC* A_Dc, RECT* A_PtrRc, __u32 A_Time, __u32 A_TxtArrange)
{
	// local -------------------
		RECT	Tv_RcTxt;
		Def_WstrTag	Tv_StrTxt;
	// code --------------------
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right - E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		if(Tv_RcTxt.left < Tv_RcTxt.right)
		{
			GrStrTimeToWstr(Tv_StrTxt, (Def_GrTime)A_Time, FALSE, FALSE, FALSE, 0, 0, 1, TRUE, TRUE);
			A_Dc->SetTextColor(afxGlobalData.clrWindowText);
			A_Dc->DrawText(Tv_StrTxt, &Tv_RcTxt, DT_SINGLELINE | A_TxtArrange);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValBitmap(CDC* A_Dc, RECT* A_PtrRc, __u8* A_PtrVal, __u16 A_Cnt, __u32 A_ClrMark)
{
	// local -------------------
		__u8	Tv_ByteIdx;
		__u8	Tv_Mask;
		RECT	Tv_RcPnt;
		__s32	Tv_BitIdx;
		Def_WstrNum	Tv_StrTxt;
	// code --------------------
		if(0 != A_Cnt)
		{

			Tv_StrTxt[1]		=	0;		// string initialize

			Tv_RcPnt.left		=	A_PtrRc->left;
			Tv_RcPnt.top		=	A_PtrRc->top;
			Tv_RcPnt.right	=	A_PtrRc->left + E_GrMfcDockCtrlBaseBitmapBitWidth;
			Tv_RcPnt.bottom	=	A_PtrRc->bottom;

			A_Dc->SelectObject(m_PenSubGrid);
			A_Dc->SetBkMode(TRANSPARENT);

			for(Tv_ByteIdx=0; Tv_ByteIdx < A_Cnt; Tv_ByteIdx++)
			{
				// in byte
				Tv_Mask				=	1;
				Tv_StrTxt[0]	=	L'0';
				for(Tv_BitIdx=0; Tv_BitIdx < 8; Tv_BitIdx++)
				{
					// draw
					if(0 != (A_PtrVal[Tv_ByteIdx] & Tv_Mask))
					{
						// mark
						A_Dc->FillSolidRect(&Tv_RcPnt, (COLORREF)A_ClrMark);
						A_Dc->SetTextColor(RGB(255, 255, 255));
					}
					else
					{
						A_Dc->SetTextColor(afxGlobalData.clrBtnText);
					}
					// draw text
					A_Dc->DrawText(Tv_StrTxt, &Tv_RcPnt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					// grid
					A_Dc->MoveTo(Tv_RcPnt.left, Tv_RcPnt.top);
					A_Dc->LineTo(Tv_RcPnt.left, Tv_RcPnt.bottom);
					// next
					Tv_Mask		=	Tv_Mask << 1;
					Tv_StrTxt[0] ++;
					Tv_RcPnt.left		=	Tv_RcPnt.right;
					Tv_RcPnt.right	=	Tv_RcPnt.right + E_GrMfcDockCtrlBaseBitmapBitWidth;
				}
			}
			// draw outline
			A_Dc->Draw3dRect(A_PtrRc, afxGlobalData.clrActiveCaption, afxGlobalData.clrActiveCaption);
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValAlighHz(CDC* A_Dc, RECT* A_PtrRc, __u8 A_Val, __u32 A_TxtArrange)
{
	// local -------------------
		WCHAR*	Tv_StrTxt;
		RECT	Tv_RcTxt;
	// code --------------------
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right - E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		switch(A_Val)
		{
			case E_GrAlignRight:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStrRight;
				break;
			case E_GrAlignCenter:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStrCenter;
				break;
			default:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStrLeft;
				break;
		}
	
		if(Tv_RcTxt.left < Tv_RcTxt.right)
		{
			A_Dc->SetTextColor(afxGlobalData.clrWindowText);
			A_Dc->DrawText(Tv_StrTxt, &Tv_RcTxt, DT_SINGLELINE | A_TxtArrange);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValAlighVt(CDC* A_Dc, RECT* A_PtrRc, __u8 A_Val, __u32 A_TxtArrange)
{
	// local -------------------
		WCHAR*	Tv_StrTxt;
		RECT	Tv_RcTxt;
	// code --------------------
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right - E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		switch(A_Val)
		{
			case E_GrAlignBottom:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStrBottom;
				break;
			case E_GrAlignCenter:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStrCenter;
				break;
			default:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStrTop;
				break;
		}

		if(Tv_RcTxt.left < Tv_RcTxt.right)
		{
			A_Dc->SetTextColor(afxGlobalData.clrWindowText);
			A_Dc->DrawText(Tv_StrTxt, &Tv_RcTxt, DT_SINGLELINE | A_TxtArrange);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValDirection(CDC* A_Dc, RECT* A_PtrRc, __u8 A_Val, __u32 A_TxtArrange)
{
	// local -------------------
		WCHAR*	Tv_StrTxt;
		RECT	Tv_RcTxt;
	// code --------------------
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right - E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		switch (A_Val)
		{
			case E_GrWayUp:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStrUp;
				break;
			case E_GrWayRight:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStrRight;
				break;
			case E_GrWayDown:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStrDown;
				break;
			default:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStrLeft;
				break;
		}

		if (Tv_RcTxt.left < Tv_RcTxt.right)
		{
			A_Dc->SetTextColor(afxGlobalData.clrWindowText);
			A_Dc->DrawText(Tv_StrTxt, &Tv_RcTxt, DT_SINGLELINE | A_TxtArrange);
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValVarSize(CDC* A_Dc, RECT* A_PtrRc, __u8 A_Val, __u32 A_TxtArrange)
{
	// local -------------------
		WCHAR*	Tv_StrTxt;
		RECT	Tv_RcTxt;
	// code --------------------
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right - E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		switch(A_Val)
		{
			case E_GrVarSize2byte:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStr2byte;
				break;
			case E_GrVarSize4byte:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStr4byte;
				break;
			default:
				Tv_StrTxt	=	V_GrMfcDockCtrlBaseStr1byte;
				break;
		}

		if(Tv_RcTxt.left < Tv_RcTxt.right)
		{
			A_Dc->SetTextColor(afxGlobalData.clrWindowText);
			A_Dc->DrawText(Tv_StrTxt, &Tv_RcTxt, DT_SINGLELINE | A_TxtArrange);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValMacAdr(CDC* A_Dc, RECT* A_PtrRc, __u8* A_PtrVal, __u32 A_TxtArrange)
{
	// local -------------------
		WCHAR	Tv_StrTxt[32];
		RECT	Tv_RcTxt;
	// code --------------------
		// make rect
		Tv_RcTxt.left			=	A_PtrRc->left + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top			=	A_PtrRc->top;
		Tv_RcTxt.right		=	A_PtrRc->right - E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.bottom		=	A_PtrRc->bottom;

		// make text
		GrStrMacToWstr(Tv_StrTxt, A_PtrVal, TRUE, (WCHAR)':');

		// draw text
		if(Tv_RcTxt.left < Tv_RcTxt.right)
		{
			A_Dc->SetTextColor(afxGlobalData.clrWindowText);
			A_Dc->DrawText(Tv_StrTxt, &Tv_RcTxt, DT_SINGLELINE | A_TxtArrange);
		}
}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// local -------------------
		int		Tv_Max;
		SCROLLINFO	Tv_Sinfo;
	// code --------------------
		// do scroll
		switch(nSBCode)
		{
			case SB_LINEUP:
				if(E_GrMfcDockCtrlBaseScrlStep > m_ScrlPosHz)
				{
					m_ScrlPosHz	=	0;
				}
				else
				{
					m_ScrlPosHz	=	m_ScrlPosHz - E_GrMfcDockCtrlBaseScrlStep;
				}
				SetScrollPos(SB_HORZ, m_ScrlPosHz);
				break;
			case SB_LINEDOWN:
				Tv_Max	=	GetScrollLimit(SB_HORZ);
				if((m_ScrlPosHz + E_GrMfcDockCtrlBaseScrlStep) > Tv_Max)
				{
					m_ScrlPosHz	=	Tv_Max;
				}
				else
				{
					m_ScrlPosHz	=	m_ScrlPosHz + E_GrMfcDockCtrlBaseScrlStep;
				}
				SetScrollPos(SB_HORZ, m_ScrlPosHz);
				break;
			case SB_PAGEUP:
				if(E_GrMfcDockCtrlBaseScrlPage > m_ScrlPosHz)
				{
					m_ScrlPosHz	=	0;
				}
				else
				{
					m_ScrlPosHz	=	m_ScrlPosHz - E_GrMfcDockCtrlBaseScrlPage;
				}
				SetScrollPos(SB_HORZ, m_ScrlPosHz);
				break;
			case SB_PAGEDOWN:
				Tv_Max	=	GetScrollLimit(SB_HORZ);
				if((m_ScrlPosHz + E_GrMfcDockCtrlBaseScrlPage) > Tv_Max)
				{
					m_ScrlPosHz	=	Tv_Max;
				}
				else
				{
					m_ScrlPosHz	=	m_ScrlPosHz + E_GrMfcDockCtrlBaseScrlPage;
				}
				SetScrollPos(SB_HORZ, m_ScrlPosHz);
				break;
			case SB_THUMBTRACK:
				Tv_Sinfo.cbSize		=	sizeof(Tv_Sinfo);
				Tv_Sinfo.fMask		=	SIF_TRACKPOS;
				GetScrollInfo(SB_HORZ, &Tv_Sinfo);
				m_ScrlPosHz	=	Tv_Sinfo.nTrackPos;
				SetScrollPos(SB_HORZ, m_ScrlPosHz);
				break;
		}

		LcCtlEditFree();
		LcComboReqFree();
		CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
		Invalidate(FALSE);
}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// local -------------------
		int		Tv_Max;
		SCROLLINFO	Tv_Sinfo;
	// code --------------------
		// do scroll
		switch( nSBCode )
		{
			case SB_LINEUP:
				if ( E_GrMfcDockCtrlBaseScrlStep > m_ScrlPosVt )
				{
					m_ScrlPosVt	=	0;
				}
				else
				{
					m_ScrlPosVt	=	m_ScrlPosVt - E_GrMfcDockCtrlBaseScrlStep;
				}
				SetScrollPos( SB_VERT, m_ScrlPosVt );
				break;
			case SB_LINEDOWN:
				Tv_Max	=	GetScrollLimit( SB_VERT );
				if ( (m_ScrlPosVt + E_GrMfcDockCtrlBaseScrlStep) > Tv_Max )
				{
					m_ScrlPosVt	=	Tv_Max;
				}
				else
				{
					m_ScrlPosVt	=	m_ScrlPosVt + E_GrMfcDockCtrlBaseScrlStep;
				}
				SetScrollPos( SB_VERT, m_ScrlPosVt );
				break;
			case SB_PAGEUP:
				if ( E_GrMfcDockCtrlBaseScrlPage > m_ScrlPosVt )
				{
					m_ScrlPosVt	=	0;
				}
				else
				{
					m_ScrlPosVt	=	m_ScrlPosVt - E_GrMfcDockCtrlBaseScrlPage;
				}
				SetScrollPos( SB_VERT, m_ScrlPosVt );
				break;
			case SB_PAGEDOWN:
				Tv_Max	=	GetScrollLimit( SB_VERT );
				if ( (m_ScrlPosVt + E_GrMfcDockCtrlBaseScrlPage) > Tv_Max )
				{
					m_ScrlPosVt	=	Tv_Max;
				}
				else
				{
					m_ScrlPosVt	=	m_ScrlPosVt + E_GrMfcDockCtrlBaseScrlPage;
				}
				SetScrollPos( SB_VERT, m_ScrlPosVt );
				break;
			case SB_THUMBTRACK:
				Tv_Sinfo.cbSize		=	sizeof(Tv_Sinfo);
				Tv_Sinfo.fMask		=	SIF_TRACKPOS;
				GetScrollInfo(SB_VERT, &Tv_Sinfo);
				m_ScrlPosVt	=	Tv_Sinfo.nTrackPos;
				SetScrollPos( SB_VERT, m_ScrlPosVt );
				break;
		}

		LcCtlEditFree();
		LcComboReqFree();
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
		Invalidate( FALSE );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::EditValue(__u32 A_HndlItem, int A_PosX, int A_PosY)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_BitPos;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrValInfo;
	// code --------------------
		Tv_Result			=	FALSE;
		Tv_PtrValInfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if(NULL != Tv_PtrValInfo)
		{
			if(Tv_PtrValInfo->IsEdit)
			{
				Tv_Result	=	TRUE;
				Tv_BitPos	=	0;
				switch(Tv_PtrValInfo->ValType)
				{
					case E_GrVarTypeColor:
						LcProcLbtnUpValColor(A_HndlItem, A_PosX, A_PosY);
						break;
					case E_GrVarTypePallete:
						LcProcLbtnUpValPallete(A_HndlItem, A_PosX, A_PosY);
						break;
					case E_GrVarTypeBool8:
						LcProcLbtnUpValBool8(A_HndlItem, A_PosX, A_PosY);
						break;
					case E_GrVarTypeBitBoolBit7:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit6:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit5:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit4:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit3:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit2:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit1:
						Tv_BitPos ++;
					case E_GrVarTypeBitBoolBit0:
						LcProcLbtnUpValBoolBit(A_HndlItem, A_PosX, A_PosY, Tv_BitPos);
						break;
					case E_GrVarTypeBit0U8:
					case E_GrVarTypeBit1U8:
						LcProcLbtnUpValBitmap(A_HndlItem, A_PosX, A_PosY);
						break;
					case E_GrVarTypeAlignHz:
						LcProcLbtnUpValAlighHz(A_HndlItem, A_PosX, A_PosY);
						break;
					case E_GrVarTypeAlignVt:
						LcProcLbtnUpValAlighVt(A_HndlItem, A_PosX, A_PosY);
						break;
					case E_GrVarTypeDirection:
						LcProcLbtnUpValDirection(A_HndlItem, A_PosX, A_PosY);
						break;
					case E_GrVarTypeVarSize:
						LcProcLbtnUpValVarSize(A_HndlItem, A_PosX, A_PosY);
						break;
					default:
						LcProcLbtnUpValNormal(A_HndlItem, A_PosX, A_PosY);
						break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL Cls_GrMfcDockCtrlBase::OnEraseBkgnd(CDC* pDC)
{
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL Cls_GrMfcDockCtrlBase::OnMouseWheel(UINT A_Flag, short A_Delta, CPoint A_Pos)
{
		if ( 0 > A_Delta )
		{
			// increase 
			if ( m_ScrlPosVt < m_ScrlMaxVt )
			{
				m_ScrlPosVt ++;
				SetScrollPos( SB_VERT, m_ScrlPosVt );
			}
		}
		else
		{
			// decrease
			if ( 0 < m_ScrlPosVt )
			{
				m_ScrlPosVt --;
				SetScrollPos( SB_VERT, m_ScrlPosVt );
			}
		}
		LcCtlEditFree();
		LcComboDoFree();
		Invalidate( FALSE );
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValColor(CDC* A_Dc, RECT* A_PtrRc, DWORD A_Value, __u32 A_TxtArrange)
{
	// local -------------------
		DWORD	Tv_ClrBg;
		RECT	Tv_RcClr;
		RECT	Tv_RcTxt;
		Def_WstrName	Tv_StrTxt;
		WCHAR*	Tv_StrWk;
	// code --------------------
		// get color value
		Tv_ClrBg	=	GrDrawCvtColorByFcc(m_PxFmt, A_Value, E_GrFccBGR888);
		
		// draw background color
		Tv_RcClr.left		=	A_PtrRc->left;
		Tv_RcClr.top		=	A_PtrRc->top;
		Tv_RcClr.right	=	Tv_RcClr.left + A_PtrRc->bottom - A_PtrRc->top;
		Tv_RcClr.bottom	=	A_PtrRc->bottom;
		A_Dc->FillSolidRect( &Tv_RcClr, (COLORREF)Tv_ClrBg );
		//A_Dc->Draw3dRect( &Tv_RcClr, afxGlobalData.clrActiveBorder, afxGlobalData.clrActiveBorder );
		//A_Dc->DrawEdge( &Tv_RcClr, EDGE_BUMP, BF_RECT );

		// make text
		GrStrDwordToHexWstr( Tv_StrTxt, A_Value );

		if ( 0 == A_Value )
		{
			Tv_StrWk	=	GrStrWcat( Tv_StrTxt, L" (S)" );	// sprite
		}
		else if(GrDrawIsAlphaColor(m_PxFmt, A_Value))
		{
			Tv_StrWk	=	GrStrWcat( Tv_StrTxt, L" (A)" );	// alpha
		}
		// draw color value
		Tv_RcTxt.left		=	Tv_RcClr.right + E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.top		=	Tv_RcClr.top;
		Tv_RcTxt.right	=	A_PtrRc->right - E_GrMfcDockCtrlBaseItemTxtHzGap;
		Tv_RcTxt.bottom	=	Tv_RcClr.bottom;
		if ( Tv_RcTxt.left < Tv_RcTxt.right )
		{
			A_Dc->SetTextColor( afxGlobalData.clrWindowText );
			A_Dc->DrawTextEx(Tv_StrTxt, GrStrWlen(Tv_StrTxt), &Tv_RcTxt, DT_SINGLELINE | A_TxtArrange, NULL);
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDrawValPallete(CDC* A_Dc, RECT* A_PtrRc, DWORD A_Value, __u32 A_TxtArrange)
{
	// local -------------------
		DWORD	Tv_ClrBg;
		RECT	Tv_RcClr;
	// code --------------------
		// get color value
		Tv_ClrBg	=	GrDrawCvtColorByFcc(m_PxFmt, A_Value, E_GrFccBGR888);

		// draw background color
		Tv_RcClr.left		=	A_PtrRc->left + 2;
		Tv_RcClr.top		=	A_PtrRc->top + 2;
		Tv_RcClr.right	=	A_PtrRc->right - 2;
		Tv_RcClr.bottom	=	A_PtrRc->bottom - 2;

		A_Dc->FillSolidRect(&Tv_RcClr, (COLORREF)Tv_ClrBg);
		if (GrDrawIsAlphaColor(m_PxFmt, A_Value))
		{
			Tv_RcClr.left		=	A_PtrRc->left + 4;
			Tv_RcClr.top		=	A_PtrRc->top + 4;
			Tv_RcClr.right	=	A_PtrRc->right - 4;
			Tv_RcClr.bottom	=	A_PtrRc->bottom - 4;

			A_Dc->FillSolidRect(&Tv_RcClr, afxGlobalData.clrWindow);
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcProcLbtnUpValColor(__u32 A_HndlItem, int A_PosX, int A_PosY)
{
	// local -------------------
		CMFCColorDialog*	Tv_DlgClr;
		DWORD	Tv_ClrVal;
		DWORD	Tv_ClrRgb;
		RECT	Tv_RcItm;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		// get value info
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if(NULL != Tv_PtrVinfo)
		{
			// check color button or edit
			if(Tv_PtrVinfo->IsEdit && (NULL != Tv_PtrVinfo->PtrVal))
			{
				if(ItemRectGetByItemHndl(A_HndlItem, &Tv_RcItm))
				{
					if(A_PosX < (Tv_RcItm.bottom - Tv_RcItm.top))
					{
						// color button
						Tv_ClrVal	=	*((DWORD*)Tv_PtrVinfo->PtrVal);
						Tv_ClrRgb	=	GrDrawCvtColorByFcc(m_PxFmt, Tv_ClrVal, E_GrFccRGB888);

						Tv_DlgClr	=	(CMFCColorDialog*) new CMFCColorDialog((COLORREF)Tv_ClrRgb, 0, this, (HPALETTE)0);
						if(IDOK == Tv_DlgClr->DoModal())
						{
							// get color
							Tv_ClrRgb	=	(DWORD)Tv_DlgClr->GetColor();
							Tv_ClrVal	=	GrDrawCvtColorByFcc(E_GrFccBGR888, Tv_ClrRgb, m_PxFmt);
							*((DWORD*)Tv_PtrVinfo->PtrVal)	=	Tv_ClrVal;
							Tv_PtrVinfo->IsModified	=	TRUE;

							LcDoEvtValueChg(A_HndlItem);
							// redraw
							Invalidate();
						}
						delete	Tv_DlgClr;
					}
					else
					{
						// value edit
						if(ScrollByItemHndl(A_HndlItem))
						{
							LcCtlEditCreate(&Tv_RcItm, A_HndlItem);
						}
					}

				}	//	if(ItemRectGetByItemHndl(A_HndlItem, &Tv_RcItm))

			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcProcLbtnUpValPallete(__u32 A_HndlItem, int A_PosX, int A_PosY)
{
	// local -------------------
		CMFCColorDialog*	Tv_DlgClr;
		DWORD	Tv_ClrVal;
		DWORD	Tv_ClrRgb;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		// get value info
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if (NULL != Tv_PtrVinfo)
		{
			// check color button or edit
			if (Tv_PtrVinfo->IsEdit && (NULL != Tv_PtrVinfo->PtrVal))
			{
				Tv_ClrVal	=	*((DWORD*)Tv_PtrVinfo->PtrVal);
				Tv_ClrRgb	=	GrDrawCvtColorByFcc(m_PxFmt, Tv_ClrVal, E_GrFccBGR888);

				Tv_DlgClr	=	(CMFCColorDialog*) new CMFCColorDialog((COLORREF)Tv_ClrRgb, 0, this, (HPALETTE)0);
				if (IDOK == Tv_DlgClr->DoModal())
				{
					// get color
					Tv_ClrRgb	=	(DWORD)Tv_DlgClr->GetColor();
					Tv_ClrVal	=	GrDrawCvtColorByFcc(E_GrFccBGR888, Tv_ClrRgb, m_PxFmt);
					*((DWORD*)Tv_PtrVinfo->PtrVal)	=	Tv_ClrVal;
					Tv_PtrVinfo->IsModified	=	TRUE;

					LcDoEvtValueChg(A_HndlItem);
					// redraw
					Invalidate();
				}
				delete	Tv_DlgClr;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::LcCtlEditCreate(RECT* A_PtrRect, __u32 A_HndlItem)
{
	// local -------------------
		BOOL8	Tv_Result;
		RECT	Tv_Rc;
		int		Tv_Limit;
		Def_WstrTag	Tv_StrTxt;
		//WCHAR*	Tv_StrWk;
		DWORD	Tv_ValDw;
		int		Tv_ValInt;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;
	// code --------------------
		Tv_Result	=	FALSE;
		// check already edit
		if ( NULL == m_ObjEdit )
		{
			// create
			Tv_Rc.left			=	A_PtrRect->left + 1;
			Tv_Rc.top				=	A_PtrRect->top;
			Tv_Rc.right			=	A_PtrRect->right - 1;
			Tv_Rc.bottom		=	A_PtrRect->bottom - 1;

			if ( Tv_Rc.left < Tv_Rc.right )
			{
				m_ObjEdit	=	(CEdit*)new CEdit();
				m_ObjEdit->Create( WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL| ES_LEFT, Tv_Rc, this, E_GrMfcDockCtrlBaseRsrcIdEdit );
				m_ObjEdit->SetFont( &afxGlobalData.fontRegular );

				m_EditItemHndl	=	A_HndlItem;
				Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
				if(NULL != Tv_PtrVinfo)
				{
					if(NULL != Tv_PtrVinfo->PtrVal)
					{
						if(!CtlEditSetValue(A_HndlItem,m_ObjEdit))
						{
							m_IsEditAbleChgNoty	=	FALSE;

							Tv_Limit	=	0;
							switch(Tv_PtrVinfo->ValType)
							{
								case E_GrVarTypeS8:
									Tv_Limit	=	4;
									Tv_ValInt	=	(int)(*((char*)Tv_PtrVinfo->PtrVal));
									GrStrIntToWstr(Tv_StrTxt, Tv_ValInt);
									m_ObjEdit->SetWindowText(Tv_StrTxt);
									break;
								case E_GrVarTypeS16:
									Tv_Limit	=	6;
									Tv_ValInt	=	(int)(*((short*)Tv_PtrVinfo->PtrVal));
									GrStrIntToWstr(Tv_StrTxt, Tv_ValInt);
									m_ObjEdit->SetWindowText(Tv_StrTxt);
									break;
								case E_GrVarTypeS32:
									Tv_Limit	=	11;
									Tv_ValInt	=	(int)(*((short*)Tv_PtrVinfo->PtrVal));
									GrStrIntToWstr(Tv_StrTxt, Tv_ValInt);
									m_ObjEdit->SetWindowText(Tv_StrTxt);
									break;
								case E_GrVarTypeU8:
									Tv_Limit	=	3;
									Tv_ValInt	=	(int)(*((__u8*)Tv_PtrVinfo->PtrVal));
									GrStrIntToWstr(Tv_StrTxt, Tv_ValInt);
									m_ObjEdit->SetWindowText(Tv_StrTxt);
									break;
								case E_GrVarTypeU16:
									Tv_Limit	=	5;
									Tv_ValInt	=	(int)(*((WORD*)Tv_PtrVinfo->PtrVal));
									GrStrIntToWstr(Tv_StrTxt, Tv_ValInt);
									m_ObjEdit->SetWindowText(Tv_StrTxt);
									break;
								case E_GrVarTypeU32:
									Tv_Limit	=	10;
									Tv_ValInt	=	(int)(*((DWORD*)Tv_PtrVinfo->PtrVal));
									GrStrIntToWstr(Tv_StrTxt, Tv_ValInt);
									m_ObjEdit->SetWindowText(Tv_StrTxt);
									break;
								case E_GrVarTypeWstr:
									Tv_Limit	=	(int)Tv_PtrVinfo->ValCnt;
									m_ObjEdit->SetWindowText((WCHAR*)Tv_PtrVinfo->PtrVal);
									break;
								case E_GrVarTypeWchar:
									Tv_Limit	=	1;
									GrStrWcopy(Tv_StrTxt, (WCHAR*)Tv_PtrVinfo->PtrVal, 1);
									m_ObjEdit->SetWindowText(Tv_StrTxt);
									break;
								case E_GrVarTypeColor:
									Tv_Limit	=	8;
									Tv_ValDw	=	*((DWORD*)Tv_PtrVinfo->PtrVal);
									GrStrDwordToHexWstr(Tv_StrTxt, Tv_ValDw);
									m_ObjEdit->SetWindowText(Tv_StrTxt);
									break;
								case E_GrVarTypeTime:
									Tv_Limit	=	40;
									Tv_ValDw	=	*((DWORD*)Tv_PtrVinfo->PtrVal);
									GrStrTimeToWstr(Tv_StrTxt, (Def_GrTime)Tv_ValDw, FALSE, FALSE, FALSE, 0, 0, 1, TRUE, TRUE);
									m_ObjEdit->SetWindowText(Tv_StrTxt);
									break;
								case  E_GrVarTypeMacAdr:
									Tv_Limit	=	17;
									GrStrMacToWstr(Tv_StrTxt, (__u8*)Tv_PtrVinfo->PtrVal, TRUE, L':');
									m_ObjEdit->SetWindowText(Tv_StrTxt);
									break;
							}
							m_ObjEdit->LimitText(Tv_Limit);		// set limit

							m_IsEditAbleChgNoty	=	TRUE;
						}
						m_ObjEdit->SetFocus();
						m_ObjEdit->SetSel(0, -1);

						// success
						Tv_Result	=	TRUE;

					}
				}

			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnEditChange( )
{
		LcCtlEditValUpdt();
}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnEditKillFocus( )
{
		LcCtlEditFree();
}
//--------------------------------------------------------------------
BOOL Cls_GrMfcDockCtrlBase::PreTranslateMessage( MSG* pMsg )
{
	// local -------------------
		BOOL	Tv_Result;
	// code --------------------
		Tv_Result	=	CWnd::PreTranslateMessage( pMsg );

		if ( NULL != m_ObjEdit )
		{
			if ( m_ObjEdit->m_hWnd == pMsg->hwnd )
			{
				if ( (WM_KEYUP == pMsg->message) && (VK_RETURN == pMsg->wParam) )
				{
					// edit finish
					LcCtlEditFree();
					Tv_Result	=	TRUE;
				}
				else if((WM_KEYDOWN == pMsg->message) && (0 != (0x8000 & GetKeyState(VK_CONTROL))))
				{
					if ('V' == pMsg->wParam)
					{
						// past
						m_ObjEdit->Paste();
						Tv_Result	=	TRUE;
					}
					else if('C' == pMsg->wParam)
					{
						// copy
						m_ObjEdit->Copy();
						Tv_Result	=	TRUE;
					}
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::LcCtlEditFree( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_ObjEdit )
		{
			m_ObjEdit->DestroyWindow();
			delete	m_ObjEdit;
			m_ObjEdit				=	NULL;
			m_EditItemHndl	=	0;

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcCtlEditValUpdt( void )
{
	// local -------------------
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;
	// code --------------------


		if(m_IsEditAbleChgNoty)
		{
			Tv_PtrVinfo	=	ValueInfoGetByItemHndl(m_EditItemHndl);
			if(NULL != Tv_PtrVinfo)
			{
				if(NULL != Tv_PtrVinfo->PtrVal)
				{
					if(!UpdateValueByCtlEdit(m_EditItemHndl, m_ObjEdit))
					{
						switch(Tv_PtrVinfo->ValType)
						{
							case E_GrVarTypeColor:
								LcEditValHexUpdt(4);
								break;
							case E_GrVarTypeS8:
							case E_GrVarTypeU8:
								LcEditValIntUpdt(1);
								break;
							case E_GrVarTypeS16:
							case E_GrVarTypeU16:
								LcEditValIntUpdt(2);
								break;
							case E_GrVarTypeS32:
							case E_GrVarTypeU32:
								LcEditValIntUpdt(4);
								break;
							case E_GrVarTypeWstr:
								LcEditValWstrUpdt();
								break;
							case E_GrVarTypeWchar:
								LcEditValWcOneUpdt();
								break;
							case E_GrVarTypeStr:
								LcEditValStrUpdt();
								break;
							case E_GrVarTypeIpV4:
								LcEditValIpV4Updt();
								break;
							case  E_GrVarTypeHexU32:
								LcEditValHexUpdt(4);
								break;
							case E_GrVarTypeHexU64:
								LcEditValHexUpdt(8);
								break;
							case E_GrVarTypeTime:
								LcEditValTimeUpdt();
								break;
							case  E_GrVarTypeMacAdr:
								LcEditValMacUpdt();
								break;
						}
					}
					// after action
					LcDoEvtValueChg(m_EditItemHndl);
				}
			}
		}


}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcEditValHexUpdt( __u8 A_ValSize )
{
	// local -------------------
		Def_WstrNum	Tv_StrNum;
		__u64	Tv_Value;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;
	// code --------------------

		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(m_EditItemHndl);
		if(NULL != Tv_PtrVinfo)
		{
			if(NULL != Tv_PtrVinfo->PtrVal)
			{
				m_ObjEdit->GetWindowText(Tv_StrNum, 16);
				GrStrHexWstrToQword(Tv_StrNum, &Tv_Value);
				switch(A_ValSize)
				{
					case 1:
						if(*((__u8*)Tv_PtrVinfo->PtrVal)	!=	(__u8)(Tv_Value & 0xFF))
						{
							*((__u8*)Tv_PtrVinfo->PtrVal)	=	(__u8)(Tv_Value & 0xFF);
							Tv_PtrVinfo->IsModified	=	TRUE;
						}
						break;
					case 2:
						if(*((__u16*)Tv_PtrVinfo->PtrVal) != (__u16)(Tv_Value & 0xFFFF))
						{
							*((__u16*)Tv_PtrVinfo->PtrVal)	=	(__u16)(Tv_Value & 0xFFFF);
							Tv_PtrVinfo->IsModified	=	TRUE;
						}
						break;
					case 4:
						if(*((__u32*)Tv_PtrVinfo->PtrVal) != (__u32)(Tv_Value & 0xFFFFFFFF))
						{
							*((__u32*)Tv_PtrVinfo->PtrVal)	=	(__u32)(Tv_Value & 0xFFFFFFFF);
							Tv_PtrVinfo->IsModified	=	TRUE;
						}
						break;
					case 8:
						if(*((__u64*)Tv_PtrVinfo->PtrVal) != Tv_Value)
						{
							*((__u64*)Tv_PtrVinfo->PtrVal)	=	Tv_Value;
							Tv_PtrVinfo->IsModified	=	TRUE;
						}
						break;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcProcLbtnUpValNormal(__u32 A_HndlItem, int A_PosX, int A_PosY)
{
	// local -------------------
		RECT	Tv_RcItm;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if(NULL != Tv_PtrVinfo)
		{
			if(Tv_PtrVinfo->IsEdit && (NULL != Tv_PtrVinfo->PtrVal))
			{
				// value edit
				if(ScrollByItemHndl(A_HndlItem))
				{
					if(ItemRectGetByItemHndl(A_HndlItem, &Tv_RcItm))
					{
						LcCtlEditCreate(&Tv_RcItm, A_HndlItem);
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcProcLbtnUpValMacAdr(__u32 A_HndlItem, int A_PosX, int A_PosY)
{
	// local -------------------
		RECT	Tv_RcItm;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if(NULL != Tv_PtrVinfo)
		{
			if(Tv_PtrVinfo->IsEdit && (NULL != Tv_PtrVinfo->PtrVal))
			{
				// value edit
				if(ScrollByItemHndl(A_HndlItem))
				{
					if(ItemRectGetByItemHndl(A_HndlItem, &Tv_RcItm))
					{
						LcCtlEditCreate(&Tv_RcItm, A_HndlItem);
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcProcLbtnUpValBool8(__u32 A_HndlItem, int A_PosX, int A_PosY)
{
	// local -------------------
		int	Tv_SelIdx;
		int	Tv_CmbIdx;
		__u8	Tv_ValNow;
		RECT	Tv_RcItm;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if(NULL != Tv_PtrVinfo)
		{
			if(Tv_PtrVinfo->IsEdit && (NULL != Tv_PtrVinfo->PtrVal))
			{
				// value edit
				if(ScrollByItemHndl(A_HndlItem))
				{
					if(ItemRectGetByItemHndl(A_HndlItem, &Tv_RcItm))
					{
						if(LcComboCreate(&Tv_RcItm, A_HndlItem, 2))
						{
							// get value
							Tv_ValNow = *((__u8*)Tv_PtrVinfo->PtrVal);

							Tv_SelIdx = 0;
							// combo box item add
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrFalse, 0);
							if(0 == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}

							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrTrue, 1);
							if(0 != Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}

							// update select
							LcComboSetSel(Tv_SelIdx);

						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcProcLbtnUpValBoolBit(__u32 A_HndlItem, int A_PosX, int A_PosY, __u8 A_BitPos)
{
	// local -------------------
		int	Tv_SelIdx;
		int	Tv_CmbIdx;
		__u8	Tv_ValNow;
		RECT	Tv_RcItm;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if(NULL != Tv_PtrVinfo)
		{
			if(Tv_PtrVinfo->IsEdit && (NULL != Tv_PtrVinfo->PtrVal))
			{
				if(ScrollByItemHndl(A_HndlItem))
				{
					if(ItemRectGetByItemHndl(A_HndlItem, &Tv_RcItm))
					{
						if(LcComboCreate(&Tv_RcItm, A_HndlItem,2))
						{
							// get value
							Tv_ValNow = (*((__u8*)Tv_PtrVinfo->PtrVal)) & (1 << A_BitPos);

							Tv_SelIdx = 0;
							// combo box item add
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrFalse, 0);
							if(0 == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}

							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrTrue, 1);
							if(0 != Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}

							// update select
							LcComboSetSel(Tv_SelIdx);

						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcProcLbtnUpValBitmap(__u32 A_HndlItem, int A_PosX, int A_PosY)
{
	// local -------------------
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
		RECT	Tv_RcItm;
		__u32	Tv_BitPos;
		__u8*	Tv_PtrVal;
		__u32	Tv_ByteIdx;
		__u32	Tv_BitIdx;
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if(NULL != Tv_PtrVinfo)
		{
			if(Tv_PtrVinfo->IsEdit && (NULL != Tv_PtrVinfo->PtrVal))
			{
				if(ScrollByItemHndl(A_HndlItem))
				{
					if(ItemRectGetByItemHndl(A_HndlItem, &Tv_RcItm))
					{
						// get item position
						Tv_BitPos	=	(__u32)(A_PosX / E_GrMfcDockCtrlBaseBitmapBitWidth);
						if(Tv_BitPos >= ((__u32)Tv_PtrVinfo->ValCnt << 3))
						{
							// patch limite
							Tv_BitPos	=	((__u32)Tv_PtrVinfo->ValCnt << 3) - 1;
						}

						// change value
						Tv_ByteIdx	=	Tv_BitPos >> 3;
						Tv_BitIdx		=	Tv_BitPos	& 0x07;
						Tv_PtrVal		=	(__u8*)Tv_PtrVinfo->PtrVal;

						Tv_PtrVal[Tv_ByteIdx]	=	Tv_PtrVal[Tv_ByteIdx] ^ (1 << Tv_BitPos);

						// modified
						Tv_PtrVinfo->IsModified	=	TRUE;

						// redraw
						InvalidateRect(&Tv_RcItm, FALSE);
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcProcLbtnUpValAlighHz(__u32 A_HndlItem, int A_PosX, int A_PosY)
{
	// local -------------------
		int	Tv_SelIdx;
		int	Tv_CmbIdx;
		__u8	Tv_ValNow;
		RECT	Tv_RcItm;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if(NULL != Tv_PtrVinfo)
		{
			if(Tv_PtrVinfo->IsEdit && (NULL != Tv_PtrVinfo->PtrVal))
			{
				// value edit
				if(ScrollByItemHndl(A_HndlItem))
				{
					if(ItemRectGetByItemHndl(A_HndlItem, &Tv_RcItm))
					{
						if(LcComboCreate(&Tv_RcItm, A_HndlItem, 3))
						{
							// get value
							Tv_ValNow = *((__u8*)Tv_PtrVinfo->PtrVal);

							Tv_SelIdx = 0;
							// combo box item add
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrLeft, E_GrAlignLeft);
							if(E_GrAlignLeft == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrRight, E_GrAlignRight);
							if(E_GrAlignRight == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrCenter, E_GrAlignCenter);
							if(E_GrAlignCenter == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}

							// update select
							LcComboSetSel(Tv_SelIdx);

						}
					}
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcProcLbtnUpValAlighVt(__u32 A_HndlItem, int A_PosX, int A_PosY)
{
	// local -------------------
		int	Tv_SelIdx;
		int	Tv_CmbIdx;
		__u8	Tv_ValNow;
		RECT	Tv_RcItm;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if(NULL != Tv_PtrVinfo)
		{
			if(Tv_PtrVinfo->IsEdit && (NULL != Tv_PtrVinfo->PtrVal))
			{
				// value edit
				if(ScrollByItemHndl(A_HndlItem))
				{
					if(ItemRectGetByItemHndl(A_HndlItem, &Tv_RcItm))
					{
						if(LcComboCreate(&Tv_RcItm, A_HndlItem, 3))
						{
							// get value
							Tv_ValNow = *((__u8*)Tv_PtrVinfo->PtrVal);

							Tv_SelIdx = 0;
							// combo box item add
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrTop, E_GrAlignTop);
							if(E_GrAlignTop == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrBottom, E_GrAlignBottom);
							if(E_GrAlignBottom == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrCenter, E_GrAlignCenter);
							if(E_GrAlignCenter == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}

							// update select
							LcComboSetSel(Tv_SelIdx);

						}
					}
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcProcLbtnUpValDirection(__u32 A_HndlItem, int A_PosX, int A_PosY)
{
	// local -------------------
		int	Tv_SelIdx;
		int	Tv_CmbIdx;
		__u8	Tv_ValNow;
		RECT	Tv_RcItm;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if (NULL != Tv_PtrVinfo)
		{
			if (Tv_PtrVinfo->IsEdit && (NULL != Tv_PtrVinfo->PtrVal))
			{
				// value edit
				if (ScrollByItemHndl(A_HndlItem))
				{
					if (ItemRectGetByItemHndl(A_HndlItem, &Tv_RcItm))
					{
						if (LcComboCreate(&Tv_RcItm, A_HndlItem, 3))
						{
							// get value
							Tv_ValNow = *((__u8*)Tv_PtrVinfo->PtrVal);

							Tv_SelIdx = 0;
							// combo box item add
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrLeft, E_GrWayLeft);
							if (E_GrWayLeft == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrUp, E_GrWayUp);
							if (E_GrWayUp == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrRight, E_GrWayRight);
							if (E_GrWayRight == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStrDown, E_GrWayDown);
							if (E_GrWayDown == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}

							// update select
							LcComboSetSel(Tv_SelIdx);

						}
					}
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcProcLbtnUpValVarSize(__u32 A_HndlItem, int A_PosX, int A_PosY)
{
	// local -------------------
		int	Tv_SelIdx;
		int	Tv_CmbIdx;
		__u8	Tv_ValNow;
		RECT	Tv_RcItm;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if(NULL != Tv_PtrVinfo)
		{
			if(Tv_PtrVinfo->IsEdit && (NULL != Tv_PtrVinfo->PtrVal))
			{
				// value edit
				if(ScrollByItemHndl(A_HndlItem))
				{
					if(ItemRectGetByItemHndl(A_HndlItem, &Tv_RcItm))
					{
						if(LcComboCreate(&Tv_RcItm, A_HndlItem, 3))
						{
							// get value
							Tv_ValNow = *((__u8*)Tv_PtrVinfo->PtrVal);

							Tv_SelIdx = 0;
							// combo box item add
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStr1byte, E_GrVarSize1byte);
							if(E_GrVarSize1byte == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStr2byte, E_GrVarSize2byte);
							if(E_GrVarSize2byte == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}
							Tv_CmbIdx = LcComboItemAdd(V_GrMfcDockCtrlBaseStr4byte, E_GrVarSize4byte);
							if(E_GrVarSize4byte == Tv_ValNow)
							{
								Tv_SelIdx = Tv_CmbIdx;
							}

							// update select
							LcComboSetSel(Tv_SelIdx);

						}
					}
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcEditValIntUpdt( __u8 A_ValSize )
{
	// local -------------------
		Def_WstrNum	Tv_StrNum;
		__s32	Tv_Value;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(m_EditItemHndl);
		if(NULL != Tv_PtrVinfo)
		{
			if(NULL != Tv_PtrVinfo->PtrVal)
			{
				m_ObjEdit->GetWindowText(Tv_StrNum, 15);
				GrStrWstrToInt(Tv_StrNum, &Tv_Value);
				switch(A_ValSize)
				{
					case 1:
						*((__u8*)Tv_PtrVinfo->PtrVal)	=	(__u8)(Tv_Value & 0xFF);
						break;
					case 2:
						*((WORD*)Tv_PtrVinfo->PtrVal)	=	(WORD)(Tv_Value & 0xFFFF);
						break;
					case 4:
						*((DWORD*)Tv_PtrVinfo->PtrVal)	=	(DWORD)Tv_Value;
						break;
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDoEvtValueChg(__u32 A_HndlItem)
{
	// local -------------------
	// code --------------------
		::PostMessage( GetParent()->m_hWnd, E_GrMfcDockCtrlBaseMsgValChg, (WPARAM)A_HndlItem, (LPARAM)0 );
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDoEvtClick(__u32 A_HndlItem)
{
		::PostMessage(GetParent()->m_hWnd, E_GrMfcDockCtrlBaseMsgClick, (WPARAM)A_HndlItem, (LPARAM)0);
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcDoEvtDblClk(__u32 A_HndlItem)
{
		::PostMessage(GetParent()->m_hWnd, E_GrMfcDockCtrlBaseMsgDblClk, (WPARAM)A_HndlItem, (LPARAM)0);
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcEditValWstrUpdt( void )
{
	// local -------------------
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(m_EditItemHndl);
		if(NULL != Tv_PtrVinfo)
		{
			if(NULL != Tv_PtrVinfo->PtrVal)
			{
				m_ObjEdit->GetWindowText((WCHAR*)Tv_PtrVinfo->PtrVal, (int)Tv_PtrVinfo->ValCnt);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcEditValWcOneUpdt(void)
{
	// local -------------------
		Def_WstrNum	Tv_StrTxt;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(m_EditItemHndl);
		if(NULL != Tv_PtrVinfo)
		{
			if(NULL != Tv_PtrVinfo->PtrVal)
			{
				m_ObjEdit->GetWindowText(Tv_StrTxt, 2);
				*(WCHAR*)Tv_PtrVinfo->PtrVal		=	Tv_StrTxt[0];
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcEditValStrUpdt(void)
{
	// local -------------------
		Def_WstrShort	Tv_StrTxt;
		int		Tv_Len;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(m_EditItemHndl);
		if(NULL != Tv_PtrVinfo)
		{
			if(NULL != Tv_PtrVinfo->PtrVal)
			{
				Tv_Len	=	(int)Tv_PtrVinfo->ValCnt;
				if(GrStrMaxLen(Tv_StrTxt) <= Tv_Len)
				{
					Tv_Len	=	GrStrMaxLen(Tv_StrTxt);
				}
				m_ObjEdit->GetWindowText(Tv_StrTxt, Tv_Len);
				GrStrWstrToStr((char*)Tv_PtrVinfo->PtrVal, Tv_StrTxt);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcEditValIpV4Updt(void)
{
	// local -------------------
		Def_WstrTag	Tv_StrTxt;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(m_EditItemHndl);
		if(NULL != Tv_PtrVinfo)
		{
			if(NULL != Tv_PtrVinfo->PtrVal)
			{
				m_ObjEdit->GetWindowText(Tv_StrTxt, GrStrMaxLen(Tv_StrTxt));
				GrStrWstripToIpV4(Tv_StrTxt, Tv_PtrVinfo->PtrVal);
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcEditValTimeUpdt(void)
{
	// local -------------------
		Def_WstrTag	Tv_StrTxt;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
		__u32	Tv_Time;
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(m_EditItemHndl);
		if(NULL != Tv_PtrVinfo)
		{
			if(NULL != Tv_PtrVinfo->PtrVal)
			{
				m_ObjEdit->GetWindowText(Tv_StrTxt, GrStrMaxLen(Tv_StrTxt));
				Tv_Time		=	GrStrWstrToTime(Tv_StrTxt);
				if(0 != Tv_Time)
				{
					*((__u32*)Tv_PtrVinfo->PtrVal)	=	Tv_Time;
					Tv_PtrVinfo->IsModified	=	TRUE;
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcEditValMacUpdt(void)
{
	// local -------------------
		Def_WstrTag	Tv_StrTxt;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;	// value info
	// code --------------------
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(m_EditItemHndl);
		if(NULL != Tv_PtrVinfo)
		{
			if(NULL != Tv_PtrVinfo->PtrVal)
			{
				m_ObjEdit->GetWindowText(Tv_StrTxt, GrStrMaxLen(Tv_StrTxt));
				GrStrWstrToMacAdr((__u8*)Tv_PtrVinfo->PtrVal, Tv_StrTxt);
				Tv_PtrVinfo->IsModified	=	TRUE;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::IsItemModified(__u32 A_HndlItem)
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;
	// code --------------------
		Tv_Result		=	FALSE;

		// get value type
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(A_HndlItem);
		if(NULL != Tv_PtrVinfo)
		{
			Tv_Result	=	Tv_PtrVinfo->IsModified;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::LcComboCreate(RECT* A_PtrRect, __u32 A_HndlItem, int A_ItemMaxCnt)
{
	// local -------------------
		BOOL8	Tv_Result;
		RECT	Tv_Rc;
		int		Tv_Height;
	// code --------------------
		Tv_Result	=	FALSE;
		// check already combobox
		if ( NULL == m_ObjCombo )
		{
			// create
			Tv_Height				=	A_PtrRect->bottom - A_PtrRect->top;
			Tv_Rc.left			=	A_PtrRect->left + 1;
			Tv_Rc.top				=	A_PtrRect->top;
			Tv_Rc.right			=	A_PtrRect->right - 1;
			Tv_Rc.bottom		=	A_PtrRect->bottom - 1 + (Tv_Height* A_ItemMaxCnt);

			if ( Tv_Rc.left < Tv_Rc.right )
			{
				m_ObjCombo	=	(CComboBox*)new CComboBox();
				m_ObjCombo->Create( CBS_SIMPLE | CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL | WS_VISIBLE , Tv_Rc, this, E_GrMfcDockCtrlBaseRsrcIdCombo  );
				m_ObjCombo->SetFont( &afxGlobalData.fontRegular );
				m_ObjCombo->SetItemHeight( -1, A_PtrRect->bottom - A_PtrRect->top - 4 );

				m_ComboItemHndl	=	A_HndlItem;

				// success
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
int	Cls_GrMfcDockCtrlBase::LcComboItemAdd( WCHAR* A_StrTxt, DWORD A_Val )
{
	// local -------------------
		int	Tv_Result;
	// code --------------------
		Tv_Result	=	-1;

		if ( NULL != m_ObjCombo )
		{
			Tv_Result	=	m_ObjCombo->AddString( A_StrTxt );
			if ( 0 <= Tv_Result )
			{
				m_ObjCombo->SetItemData( Tv_Result, A_Val );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::LcComboSetSel( int A_ComboIdx )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_ObjCombo )
		{
			m_ObjCombo->SetCurSel( A_ComboIdx );
			m_ObjCombo->SetFocus();

			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcComboReqFree( void )
{
	// local -------------------
	// code --------------------
		if ( (NULL != m_ObjCombo) && (0 == m_ComboFreeReq) )
		{
			PostMessage( E_GrMfcDockCtrlBaseMsgCbxFree );
			m_ComboFreeReq ++;
		}
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcComboDoFree( void )
{
	// local -------------------
	// code --------------------
		if ( NULL != m_ObjCombo )
		{
			m_ObjCombo->DestroyWindow();
			delete	m_ObjCombo;
			m_ObjCombo			=	NULL;
			m_ComboItemHndl	=	0;

			m_ComboFreeReq	=	0;
		}
}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnCbxSelChange( )
{
	// local -------------------
	// code --------------------
		LcComboValUpdt();
}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnCbxKillFocus( )
{
	// local -------------------
	// code --------------------
		LcComboReqFree();
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::LcComboValUpdt( void )
{
	// local -------------------
		__u32	Tv_Val;
		int		Tv_SelIdx;
		__u8	Tv_BitPos;
		__u8	Tv_BitMask;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;
	// code --------------------
		Tv_SelIdx	=	m_ObjCombo->GetCurSel();
		if ( 0 <= Tv_SelIdx )
		{
			// get value type
			Tv_PtrVinfo	=	ValueInfoGetByItemHndl(m_ComboItemHndl);
			if(NULL != Tv_PtrVinfo)
			{
				if(NULL != Tv_PtrVinfo->PtrVal)
				{
					Tv_BitPos	=	0;
					Tv_Val	=	(__u32)m_ObjCombo->GetItemData(Tv_SelIdx);

					if (!UpdateValueByCombo(m_ComboItemHndl,Tv_Val))
					{
						switch (Tv_PtrVinfo->ValType)
						{
							case E_GrVarTypeBool8:
							case E_GrVarTypeAlignHz:
							case  E_GrVarTypeAlignVt:
							case E_GrVarTypeDirection:
							case E_GrVarTypeVarSize:
								if (*((__u8*)Tv_PtrVinfo->PtrVal) != (__u8)Tv_Val)
								{
									*((__u8*)Tv_PtrVinfo->PtrVal)	=	(__u8)Tv_Val;
									Tv_PtrVinfo->IsModified	=	TRUE;
								}
								break;
							case E_GrVarTypeBitBoolBit7:
								Tv_BitPos ++;
							case E_GrVarTypeBitBoolBit6:
								Tv_BitPos ++;
							case E_GrVarTypeBitBoolBit5:
								Tv_BitPos ++;
							case E_GrVarTypeBitBoolBit4:
								Tv_BitPos ++;
							case E_GrVarTypeBitBoolBit3:
								Tv_BitPos ++;
							case E_GrVarTypeBitBoolBit2:
								Tv_BitPos ++;
							case E_GrVarTypeBitBoolBit1:
								Tv_BitPos ++;
							case E_GrVarTypeBitBoolBit0:
								Tv_BitMask	=	~(1 << Tv_BitPos);
								Tv_Val			=	Tv_Val << Tv_BitPos;
								*((__u8*)Tv_PtrVinfo->PtrVal)	=	*((__u8*)Tv_PtrVinfo->PtrVal) & Tv_BitMask;
								*((__u8*)Tv_PtrVinfo->PtrVal)	=	*((__u8*)Tv_PtrVinfo->PtrVal) | (__u8)Tv_Val;
								Tv_PtrVinfo->IsModified	=	TRUE;
								break;
							default:
								DbgMsgPrint("Cls_GrMfcDockCtrlBase::LcComboValUpdt - unknown value type!\n");
								break;
						}
					}
					LcDoEvtValueChg(m_ComboItemHndl);
				}
			}
		}
		
		// release combo
		LcComboReqFree();
}
//--------------------------------------------------------------------
LRESULT Cls_GrMfcDockCtrlBase::OnCbxFree( WPARAM A_ItemIdx, LPARAM A_Void )
{
	// local -------------------
	// code --------------------
		LcComboDoFree();

		return	0;
}
//--------------------------------------------------------------------
int		Cls_GrMfcDockCtrlBase::PopupMenuAdd(WCHAR* A_StrMenu, __u32 A_Id, BOOL8 A_IsItemIn)
{
	// local -------------------
		int		Tv_Result;
	// code --------------------
		Tv_Result		=	-1;

		if ( E_GrMfcDockCtrlBasePopupMaxCnt > m_PopupCnt )
		{
			m_PopupIn.AppendMenu(MF_STRING, (E_GrMfcDockCtrlBasePopupIdBase + m_PopupCnt), A_StrMenu);
			if (!A_IsItemIn)
			{
				m_PopupOut.AppendMenu(MF_STRING, (E_GrMfcDockCtrlBasePopupIdBase + m_PopupCnt), A_StrMenu);
			}
			m_PopupCtrl[m_PopupCnt].UserId		=	A_Id;
			m_PopupCnt ++;
			Tv_Result	=	m_PopupCnt;
		}
	
		return	Tv_Result;
}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnPopmenu(UINT A_MenuId)
{
	// local -------------------
		WPARAM	Tv_Wparm;
		LPARAM	Tv_Lparm;
		__u32		Tv_PopIdx;
	// code --------------------
		Tv_PopIdx	=	A_MenuId - E_GrMfcDockCtrlBasePopupIdBase;
		Tv_Wparm	=	m_PopupCtrl[Tv_PopIdx].UserId;
		Tv_Lparm	=	m_PopupItemHndl;
		::PostMessage(GetParent()->m_hWnd, E_GrMfcDockCtrlBaseMsgPopupMenu, Tv_Wparm, Tv_Lparm);
}
//--------------------------------------------------------------------
void Cls_GrMfcDockCtrlBase::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// local -------------------
		__u32	Tv_HndlItem;
		int		Tv_PosX;
		int		Tv_PosY;
		__u8	Tv_RtCode;
	// code --------------------
		CWnd::OnLButtonDblClk(nFlags, point);

		Tv_RtCode	=	PosInfoGet(point.x, point.y, &Tv_HndlItem, &Tv_PosX, &Tv_PosY);
		if(E_GrMfcDockCtrlBasePosOnItem == Tv_RtCode)
		{
			if (m_IsDblclkEdit)
			{
				if(!EditValue(Tv_HndlItem, Tv_PosX, Tv_PosY))
				{
					LcDoEvtDblClk(Tv_HndlItem);
				}
			}
			else
			{
				// event
				LcDoEvtDblClk(Tv_HndlItem);
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::ItemSetModifiedOne(__u32 A_HndlItem, BOOL8 A_IsModified)
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrMfcDockCtrlBaseValueInfo	Tv_PtrVinfo;
		RECT	Tv_RcPnt;
	// code --------------------
		Tv_Result	=	FALSE;

		// get value type
		Tv_PtrVinfo	=	ValueInfoGetByItemHndl(m_ComboItemHndl);
		if(NULL != Tv_PtrVinfo)
		{
			Tv_PtrVinfo->IsModified	=	A_IsModified;
			if(ItemRectGetByItemHndl(A_HndlItem, &Tv_RcPnt))
			{
				InvalidateRect(&Tv_RcPnt, FALSE);
			}
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::ScrollByItemHndl(__u32 A_HndlItem)
{
	// local -------------------
	// code --------------------
		
		return	FALSE;
}
//--------------------------------------------------------------------
Ptr_GrMfcDockCtrlBaseValueInfo	Cls_GrMfcDockCtrlBase::ValueInfoGetByItemHndl(__u32 A_HndlItem)
{
		return	NULL;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::ItemRectGetByItemHndl(__u32 A_HndlItem, RECT* A_PtrRtRect)
{
		return	FALSE;
}
//--------------------------------------------------------------------
__u8	Cls_GrMfcDockCtrlBase::PosInfoGet(int A_X, int A_Y, __u32* A_PtrRtHndlItem, int* A_PtrRtItemPosX, int* A_PtrRtItemPosY)
{
		return	E_GrMfcDockCtrlBasePosOnNotting;
}
//--------------------------------------------------------------------
int*	Cls_GrMfcDockCtrlBase::GridSizeValuePtrGet(__u32 A_HndlItem, __u8 A_Mode)
{
		return	NULL;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::OnGridSizeChange(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::OnFoldClick(__u32 A_HndlItem)
{

}
//--------------------------------------------------------------------
__u32	Cls_GrMfcDockCtrlBase::ColorFormatGet(void)
{
		return	m_PxFmt;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::ColorFormatSet(__u32 A_Fcc)
{
		m_PxFmt	=	A_Fcc;
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::OnItemLbtnDown(__u32 A_HndlItem)
{

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::CtrlRgtySubkeyGet(WCHAR* A_StrRt)
{
	// local -------------------
		WCHAR*	Tv_StrWk;
		Cls_GrMfcDockPaneBase*	Tv_Pane;
	// code --------------------
		// get pane id
		Tv_StrWk	=	GrStrWcopy(A_StrRt, L"Workspace\\GRPANE");
		Tv_Pane		=	(Cls_GrMfcDockPaneBase*)GetParent();
		GrStrIntToWstr(Tv_StrWk, (__s32)Tv_Pane->PaneIdGet());
		Tv_StrWk	=	GrStrWcat(Tv_StrWk, L"CTRL");
		GrStrIntToWstr(Tv_StrWk, (__s32)GetDlgCtrlID());
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::CtrlRgtyValWrite(__u32 A_ValId, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
		CRegKey	Tv_Rgst;
		Def_WstrTag	Tv_StrValName;
		WCHAR*	Tv_StrWk;
	// code --------------------
		// init
		Tv_Result		=	FALSE;

		// get registry key
		if(ERROR_SUCCESS == Tv_Rgst.Create(AfxGetApp()->GetAppRegistryKey(), m_StrRgtyUiStatKey))
		{
			// make value name
			Tv_StrWk	=	GrStrWcopy(Tv_StrValName, L"STAT");
			GrStrIntToWstr(Tv_StrWk, (__s32)A_ValId);

			if(ERROR_SUCCESS == Tv_Rgst.SetBinaryValue(Tv_StrValName, A_PtrBuf, (ULONG)A_Size))
			{
				Tv_Result	=	TRUE;
			}

			Tv_Rgst.Close();
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::CtrlRgtyValRead(__u32 A_ValId, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
		CRegKey	Tv_Rgst;
		Def_WstrTag	Tv_StrValName;
		WCHAR*	Tv_StrWk;
		ULONG		Tv_Size;
	// code --------------------
		// init
		Tv_Result		=	FALSE;

		// get registry key
		if(ERROR_SUCCESS == Tv_Rgst.Open(AfxGetApp()->GetAppRegistryKey(), m_StrRgtyUiStatKey))
		{
			// make value name
			Tv_StrWk	=	GrStrWcopy(Tv_StrValName, L"STAT");
			GrStrIntToWstr(Tv_StrWk, (__s32)A_ValId);
			Tv_Size	=	(ULONG)A_Size;
			if(ERROR_SUCCESS == Tv_Rgst.QueryBinaryValue(Tv_StrValName, A_PtrBuf, &Tv_Size))
			{
				Tv_Result	=	TRUE;
			}

			Tv_Rgst.Close();
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::InvalidateItem(__u32 A_HndlItem)
{
	// local -------------------
		RECT	Tv_RcDraw;
	// code --------------------
		if (ItemRectGetByItemHndl(A_HndlItem, &Tv_RcDraw))
		{
			InvalidateRect(&Tv_RcDraw, FALSE);
		}
	
}
//--------------------------------------------------------------------
void	Cls_GrMfcDockCtrlBase::DblClkEditModeSet(BOOL8 A_IsSet)
{
	// local -------------------
	// code --------------------
		m_IsDblclkEdit	=	A_IsSet;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::CtlEditSetValue(__u32 A_HndlItem, CEdit* A_CtlEdit)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::UpdateValueByCombo(__u32 A_HndlItem, __u32 A_Value)
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrMfcDockCtrlBase::UpdateValueByCtlEdit(__u32 A_HndlItem, CEdit* A_CtlEdit)
{
		return	FALSE;
}
//--------------------------------------------------------------------

