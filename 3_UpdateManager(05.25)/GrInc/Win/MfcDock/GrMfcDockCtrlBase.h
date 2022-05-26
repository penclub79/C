/*
	Control base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrMfcDockCtrlBase
#define	_EPC_GrMfcDockCtrlBase

//====================================================================
// uses
#include <afxwin.h>

#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrGdibObj.h>


//====================================================================
//constance

#define	E_GrMfcDockCtrlBaseRsrcIdEdit						100
#define	E_GrMfcDockCtrlBaseRsrcIdCombo					101

#define	E_GrMfcDockCtrlBaseMsgValChg						(WM_USER+201)		/* WPARAM = item handle, LPARAM =   */
#define	E_GrMfcDockCtrlBaseMsgClick							(WM_USER+202)		/* WPARAM = item handle, LPARAM =   */
#define	E_GrMfcDockCtrlBaseMsgDblClk						(WM_USER+203)		/* WPARAM = item handle, LPARAM =   */
#define	E_GrMfcDockCtrlBaseMsgPopupMenu					(WM_USER+204)		/* WPARAM = menu id, LPARAM = item handle */

#define	E_GrMfcDockCtrlBaseMsgCbxFree						(WM_USER+210)		/* internal message */

#define	E_GrMfcDockCtrlBaseBitmapBitWidth				12

#define	E_GrMfcDockCtrlBasePopupIdBase					(1)
#define	E_GrMfcDockCtrlBasePopupMaxCnt					16

#define	E_GrMfcDockCtrlBaseComboBoxMaxViewCnt		10

#define	E_GrMfcDockCtrlBaseItemVtMargin					2
#define	E_GrMfcDockCtrlBaseItemTxtHzGap					3
#define	E_GrMfcDockCtrlBaseHeadVtMargin					3

#define	E_GrMfcDockCtrlBaseSplitRng							3
#define	E_GrMfcDockCtrlBaseSizeMin							16

#define E_GrMfcDockCtrlBaseScrlStep							5
#define E_GrMfcDockCtrlBaseScrlPage							20

#define	E_GrMfcDockCtrlBasePosOnNotting					0
#define	E_GrMfcDockCtrlBasePosOnItem						1
#define	E_GrMfcDockCtrlBasePosOnHeader					2
#define	E_GrMfcDockCtrlBasePosOnDescName				3
#define	E_GrMfcDockCtrlBasePosOnDescComment			4
#define	E_GrMfcDockCtrlBasePosOnGridSzLt				5
#define	E_GrMfcDockCtrlBasePosOnGridSzUp				6
#define	E_GrMfcDockCtrlBasePosOnGridSzRt				7
#define	E_GrMfcDockCtrlBasePosOnGridSzDn				8
#define	E_GrMfcDockCtrlBasePosOnFoldMark				9

#define	E_GrMfcDockCtrlBaseGridSizeLt						0
#define	E_GrMfcDockCtrlBaseGridSizeUp						1
#define	E_GrMfcDockCtrlBaseGridSizeRt						2
#define	E_GrMfcDockCtrlBaseGridSizeDn						3

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrMfcDockCtrlBaseValueInfo
{
	BOOL8	IsEdit;
	BOOL8	IsModified;
	__u16	ValCnt;				// length
	__u16	ValType;
	__u32	TxtArrange;
	void*	PtrVal;
}	*Ptr_GrMfcDockCtrlBaseValueInfo;

typedef struct St_GrMfcDockCtrlBasePopupCtrl
{
	__u32	UserId;
}	*Ptr_GrMfcDockCtrlBasePopupCtrl;

//====================================================================
//global class

class Cls_GrMfcDockCtrlBase	: public CWnd
{
	protected:

		Def_WstrTag	m_StrRgtyUiStatKey;
		__u32	m_PxFmt;					// pixel format

		BOOL8	m_IsDblclkEdit;

		BOOL8	m_IsMseLbtnDown;		// check mouse button down

		// cursor
		HCURSOR	m_CusNow;
		HCURSOR	m_CusArrow;
		HCURSOR	m_CusSizeHz;
		HCURSOR	m_CusSizeVt;

		__u32		m_CusItemNow;

		// resource
		HPEN		m_PenSubGrid;

		// popup menu
		CMenu		m_PopupIn;
		CMenu		m_PopupOut;
		int			m_PopupCnt;
		St_GrMfcDockCtrlBasePopupCtrl	m_PopupCtrl[E_GrMfcDockCtrlBasePopupMaxCnt];
		__u32		m_PopupItemHndl;

		// grid size change
		__u8		m_GridSizeChgMode;			// change size mode
		int			m_GridSizeChgBasis;			// change size basis
		int*		m_PtrGridSizeVal;				// change size value

		// scroll
		int		m_ScrlPosHz;
		int		m_ScrlMaxHz;
		int		m_ScrlPosVt;
		int		m_ScrlMaxVt;

		// edit
		CEdit*	m_ObjEdit;
		__u32		m_EditItemHndl;
		BOOL8		m_IsEditAbleChgNoty;

		// combo
		CComboBox*	m_ObjCombo;
		int			m_ComboItemHndl;	// combo item handle
		int			m_ComboFreeReq;

		// method
		void	LcDoEvtValueChg( __u32 A_HndlItem );
		void	LcDoEvtClick(__u32 A_HndlItem);
		void	LcDoEvtDblClk(__u32 A_HndlItem);

		BOOL8	LcCtlEditCreate( RECT* A_PtrRect, __u32 A_HndlItem );
		BOOL8	LcCtlEditFree( void );
		void	LcCtlEditValUpdt( void );

		BOOL8	LcComboCreate( RECT* A_PtrRect, __u32 A_HndlItem, int A_ItemMaxCnt );
		int		LcComboItemAdd( WCHAR* A_StrTxt, DWORD A_Val );
		BOOL8	LcComboSetSel( int A_ComboIdx );
		void	LcComboReqFree( void );
		void	LcComboValUpdt( void );
		void	LcComboDoFree( void );

		void	LcEditValHexUpdt( __u8 A_ValSize );
		void	LcEditValIntUpdt( __u8 A_ValSize );
		void	LcEditValWstrUpdt( void );
		void	LcEditValWcOneUpdt(void);
		void	LcEditValStrUpdt(void);
		void	LcEditValIpV4Updt(void);
		void	LcEditValTimeUpdt(void);
		void	LcEditValMacUpdt(void);

		void	LcDrawValInt( CDC* A_Dc, RECT* A_PtrRc, int A_Value,__u32 A_TxtArrange );
		void	LcDrawValDword(CDC* A_Dc, RECT* A_PtrRc, DWORD A_Value, __u32 A_TxtArrange);
		void	LcDrawValHex( CDC* A_Dc, RECT* A_PtrRc, __u64 A_Value, __u32 A_Digit, __u32 A_TxtArrange );
		void	LcDrawValBool8(CDC* A_Dc, RECT* A_PtrRc, BOOL8 A_IsVal, __u32 A_TxtArrange);
		void	LcDrawValWchar(CDC* A_Dc, RECT* A_PtrRc, WCHAR* A_StrVal, __u16 A_MaxLen, __u32 A_TxtArrange);
		void	LcDrawValColor(CDC* A_Dc, RECT* A_PtrRc, DWORD A_Value, __u32 A_TxtArrange);
		void	LcDrawValPallete(CDC* A_Dc, RECT* A_PtrRc, DWORD A_Value, __u32 A_TxtArrange);
		void	LcDrawValStr(CDC* A_Dc, RECT* A_PtrRc, char* A_StrVal, __u16 A_MaxLen, __u32 A_TxtArrange);
		void	LcDrawValIpV4(CDC* A_Dc, RECT* A_PtrRc, __u32 A_IpV4, __u32 A_TxtArrange);
		void	LcDrawValTime(CDC* A_Dc, RECT* A_PtrRc, __u32 A_Time, __u32 A_TxtArrange);
		void	LcDrawValBitmap(CDC* A_Dc, RECT* A_PtrRc, __u8* A_PtrVal, __u16 A_Cnt, __u32 A_ClrMark);
		void	LcDrawValAlighHz(CDC* A_Dc, RECT* A_PtrRc, __u8 A_Val, __u32 A_TxtArrange);
		void	LcDrawValAlighVt(CDC* A_Dc, RECT* A_PtrRc, __u8 A_Val, __u32 A_TxtArrange);
		void	LcDrawValDirection(CDC* A_Dc, RECT* A_PtrRc, __u8 A_Val, __u32 A_TxtArrange);
		void	LcDrawValVarSize(CDC* A_Dc, RECT* A_PtrRc, __u8 A_Val, __u32 A_TxtArrange);
		void	LcDrawValMacAdr(CDC* A_Dc, RECT* A_PtrRc, __u8* A_PtrVal, __u32 A_TxtArrange);

		void	LcProcLbtnUpValColor(__u32 A_HndlItem, int A_PosX, int A_PosY);
		void	LcProcLbtnUpValPallete(__u32 A_HndlItem, int A_PosX, int A_PosY);
		void	LcProcLbtnUpValNormal(__u32 A_HndlItem, int A_PosX, int A_PosY);
		void	LcProcLbtnUpValBool8(__u32 A_HndlItem, int A_PosX, int A_PosY);
		void	LcProcLbtnUpValBoolBit(__u32 A_HndlItem, int A_PosX, int A_PosY, __u8 A_BitPos);
		void	LcProcLbtnUpValBitmap(__u32 A_HndlItem, int A_PosX, int A_PosY);
		void	LcProcLbtnUpValAlighHz(__u32 A_HndlItem, int A_PosX, int A_PosY);
		void	LcProcLbtnUpValAlighVt(__u32 A_HndlItem, int A_PosX, int A_PosY);
		void	LcProcLbtnUpValDirection(__u32 A_HndlItem, int A_PosX, int A_PosY);
		void	LcProcLbtnUpValVarSize(__u32 A_HndlItem, int A_PosX, int A_PosY);
		void	LcProcLbtnUpValMacAdr(__u32 A_HndlItem, int A_PosX, int A_PosY);

		// api
		void	CtrlRgtySubkeyGet(WCHAR* A_StrRt);
		BOOL8	CtrlRgtyValWrite(__u32 A_ValId, void* A_PtrBuf, __u32 A_Size);
		BOOL8	CtrlRgtyValRead(__u32 A_ValId, void* A_PtrBuf, __u32 A_Size);

		// inheritable
		virtual	void	DrawValue(CDC* A_Dc, RECT* A_PtrRc, __u32 A_HndlItem);	// need inherite
		virtual	BOOL8	EditValue(__u32 A_HndlItem, int A_PosX, int A_PosY);		// need inherite
		virtual	BOOL8	CtlEditSetValue(__u32 A_HndlItem, CEdit* A_CtlEdit);		// not need inherite
		virtual	BOOL8	UpdateValueByCombo(__u32 A_HndlItem, __u32 A_Value);		// not need inherite
		virtual	BOOL8	UpdateValueByCtlEdit(__u32 A_HndlItem, CEdit* A_CtlEdit);	// not need inherite

		// user implement
		virtual	Ptr_GrMfcDockCtrlBaseValueInfo	ValueInfoGetByItemHndl(__u32 A_HndlItem);
		virtual	BOOL8	ItemRectGetByItemHndl(__u32 A_HndlItem, RECT* A_PtrRtRect);
		virtual	__u8	PosInfoGet(int A_X, int A_Y, __u32* A_PtrRtHndlItem, int* A_PtrRtItemPosX, int* A_PtrRtItemPosY);
		virtual	int*	GridSizeValuePtrGet(__u32 A_HndlItem, __u8 A_Mode);

		// event
		virtual	void	OnGridSizeChange(void);		// grid size change event
		virtual	void	OnFoldClick(__u32 A_HndlItem);
		virtual	void	OnItemLbtnDown(__u32 A_HndlItem);
		
		// message
		virtual BOOL PreTranslateMessage( MSG* pMsg );

		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg BOOL OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message );
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg BOOL OnMouseWheel(UINT A_Flag, short A_Delta, CPoint A_Pos); 
		afx_msg void OnEditChange( );
		afx_msg void OnEditKillFocus( );
		afx_msg void OnCbxSelChange( );
		afx_msg void OnCbxKillFocus( );
		afx_msg LRESULT OnCbxFree( WPARAM A_ItemIdx, LPARAM A_Void );
		afx_msg void OnPopmenu(UINT A_MenuId);

		DECLARE_MESSAGE_MAP()
	public:
		Cls_GrMfcDockCtrlBase( void );
		virtual	~Cls_GrMfcDockCtrlBase( void );

		void	DblClkEditModeSet(BOOL8 A_IsSet);

		virtual BOOL Create(LPCTSTR lpszClassName,LPCTSTR lpszWindowName, DWORD dwStyle,
			const RECT& rect,CWnd* pParentWnd, UINT nID,CCreateContext* pContext = NULL);

		BOOL8	ItemSetModifiedOne(__u32 A_HndlItem, BOOL8 A_IsModified);
		BOOL8	IsItemModified(__u32 A_HndlItem);

		int		PopupMenuAdd( WCHAR* A_StrMenu, __u32 A_Id, BOOL8 A_IsItemIn );

		__u32	ColorFormatGet(void);
		void	ColorFormatSet(__u32 A_Fcc);

		void	InvalidateItem(__u32 A_HndlItem);

		// user implement
		virtual	BOOL8	ScrollByItemHndl(__u32 A_HndlItem);

};

//====================================================================
//global function



//====================================================================
//out Enviroment conditional
#endif

