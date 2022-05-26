/*
	property

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrMfcDockProperty
#define	_EPC_GrMfcDockProperty

//====================================================================
// uses
#include <afxwin.h>
#include <Win\MfcDock\GrMfcDockCtrlBase.h>

#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrGdibObj.h>

#include <GrStrTool.h>

//====================================================================
//constance

#define	E_GrMfcDockPropertyUiStatValId					1

#define	E_GrMfcDockPropertyItemVtMargin				2

#define	E_GrMfcDockPropertyItemTxtHzGap				3

#define	E_GrMfcDockPropertyHeadVtMargin				3

#define	E_GrMfcDockPropertyLeftGap						14
#define	E_GrMfcDockPropertyFoldMarkSize				9

#define	E_GrMfcDockPropertyNameDfltWidth			150
#define	E_GrMfcDockPropertyDescDfltHeight			30

#define	E_GrMfcDockPropertyItemMaxCnt					4096

#define	E_GrMfcDockPropertyDescLineVal				30000

#define	E_GrMfcDockPropertyHndlItemNone				0
#define	E_GrMfcDockPropertyHndlElemBase				1

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrMfcDockPropertyItemCtrl
{
	WCHAR*	StrName;
	St_GrMfcDockCtrlBaseValueInfo	ValInfo;
	BOOL8	IsLocal;			// local value, control must be release value buffer
	BOOL8	IsMark;				// mark back ground
	BOOL8	IsSeparator;
	BOOL8	IsFold;
	__u32	NameTxtArrange;
	int		Line;			// item line number
	COLORREF	ColorMark;	// mark color
	__u64	UserData;
	WCHAR*	StrDesc;			// descript

}	*Ptr_GrMfcDockPropertyItemCtrl;

typedef struct St_GrMfcDockPropertyUiStat
{
	int		NameWidth;
	int		DescHeight;
}	*Ptr_GrMfcDockPropertyUiStat;

//====================================================================
//global class

class Cls_GrMfcDockProperty : public Cls_GrMfcDockCtrlBase
{
	private:

		BOOL8	m_IsDrawAble;			// draw on item add

		St_GrMfcDockPropertyUiStat	m_UiStat;

		int		m_ItemHeight;

		int		m_ItemCnt;				// line count
		St_GrMfcDockPropertyItemCtrl		m_ItemTbl[E_GrMfcDockPropertyItemMaxCnt];

		// display
		Ptr_GrMfcDockPropertyItemCtrl		m_ViewTbl[E_GrMfcDockPropertyItemMaxCnt];
		int		m_ViewCnt;			// view item count
		int		m_ViewAbleCnt;
		int		m_ViewNowCnt;		// now display item count
		__u32	m_DescNameTxtArrange;
		__u32	m_DescCmtTxtArrange;

		// rect
		RECT	m_RcItemArea;
		RECT	m_RcItemCont;
		RECT	m_RcDescArea;
		RECT	m_RcDescName;
		RECT	m_RcDescCmt;

		// resource
		HPEN	m_PenFoldMark;
		HPEN	m_PenCtUline;
		HPEN	m_PenCtSplit;
		HGDIOBJ	m_FontOld;

	protected:
		void	LcDimensionCalc( RECT* A_PtrRc );
		__u8	LcPosInfoGet(int A_X, int A_Y, int* A_PtrRtItem, int* A_PtrRtColum, int* A_PtrRtValPosX, int* A_PtrRtValPosY);
		int		LcVisibleIdxGetByItemIdx(int A_ItemIdx);
		BOOL8	LcItemRectGetByIdx(int A_ItemIdx, int A_Colum, RECT* A_PtrRtRect);
		void	LcUnfoldByItemIdx(int A_ItemIdx);
		int		LcViewIdxGetByItemIdx(int A_ItemIdx);

		void	LcScrlbarVtUpdt( void );

		void	LcDrawItems( CDC* A_Dc );
		void	LcDrawCont( CDC* A_Dc );
		void	LcDrawItem( CDC* A_Dc, RECT* A_PtrRc, int A_ItemIdx );
		void	LcDrawLeftGap(CDC* A_Dc, RECT* A_PtrRc, int A_ItemIdx);
		void	LcDrawSeparator(CDC* A_Dc, RECT* A_PtrRc, int A_ItemIdx);
		void	LcDrawName(CDC* A_Dc, RECT* A_PtrRc, int A_ItemIdx);
		void	LcDrawValue(CDC* A_Dc, RECT* A_PtrRc, int A_ItemIdx);
		void	LcDrawDescs(CDC* A_Dc);
		void	LcDrawDescName(CDC* A_Dc, int A_ItemIdx);
		void	LcDrawDescCmt(CDC* A_Dc, int A_ItemIdx);

		void	LcViewBuild(void);
		
		// inherited
		virtual	Ptr_GrMfcDockCtrlBaseValueInfo	ValueInfoGetByItemHndl(__u32 A_HndlItem);
		virtual	BOOL8	ItemRectGetByItemHndl(__u32 A_HndlItem, RECT* A_PtrRtRect);
		virtual	__u8	PosInfoGet(int A_X, int A_Y, __u32* A_PtrRtHndlItem, int* A_PtrRtItemPosX, int* A_PtrRtItemPosY);
		virtual	int*	GridSizeValuePtrGet(__u32 A_HndlItem, __u8 A_Mode);

		// event
		virtual	void	OnGridSizeChange(void);		// grid size change event
		virtual	void	OnFoldClick(__u32 A_HndlItem);
		virtual	void	OnItemLbtnDown(__u32 A_HndlItem);

		// message

		afx_msg void OnPaint( );
		afx_msg void OnSize(UINT nType, int cx, int cy);

		DECLARE_MESSAGE_MAP()
	public:
		Cls_GrMfcDockProperty( void );
		virtual	~Cls_GrMfcDockProperty( void );

		BOOL	Create(RECT& A_Rect, CWnd* A_WndParent, UINT A_Id );
		void	ClearAll( void );
		void	ClearItems(void);

		void	DrawLock( BOOL8 A_IsDrawAble );
		int		ItemAddValue(WCHAR* A_StrName, __u16 A_ValType, void* A_PtrVal, __u16 A_ValCnt, BOOL8 A_IsEdit, WCHAR* A_StrDesc, __u32 A_TxtArrage = 0, __u32 A_NameArrage = 0, __u64 A_UserData = 0);
		int		ItemAddLocalText(WCHAR* A_StrName, WCHAR* A_StrVal, WCHAR* A_StrDesc, __u32 A_TxtArrage = 0, __u32 A_NameArrage = 0);
		int		ItemAddSeparator(WCHAR* A_StrName, WCHAR* A_StrDesc, __u32 A_NameArrage = 0);

		__u64	ItemUserDataGetByIdx( int A_ItemIdx );
		BOOL8	ItemUserDataSetByIdx(int A_ItemIdx, __u64 A_UserData);
		__u64	ItemUserDataGetByHndl(__u32 A_HndlItem);
		BOOL8	ItemUserDataSetByHndl(__u32 A_HndlItem, __u64 A_UserData);

		BOOL8	ItemValTypeSetByIdx(int A_ItemIdx, __u16 A_ValType, __u16 A_ValCnt);
		BOOL8	ItemValTypeSetByHndl(__u32 A_HndlItem, __u16 A_ValType, __u16 A_ValCnt);
		
		BOOL8	ItemDel(int A_ItemIdx);

		BOOL8	ItemSetMarkStatus(int A_ItemIdx, BOOL8 A_IsMark);
		BOOL8	ItemSetMarkColor(int A_ItemIdx, COLORREF A_Color);
		BOOL8	IsItemMarked(int A_ItemIdx);

		BOOL8	ItemSetModifiedAll(BOOL8 A_IsModified);

		void	ItemHndlDecode(__u32 A_HndlItem, int* A_PtrRtLine, int* A_PtrRtColum);
		__u32	ItemHndlEncode(int A_Line, int A_Colum);

		int		ItemCountGet(void);
		int		ScrollPosVtGet(void);
		void	ScrollPosVtSet(int A_ScrlPos);

		BOOL8	ScrollByIdx(int A_ItemIdx, BOOL8 A_IsForceUnfold);

		// ui status
		__u32	UiStatSizeGet(void);
		void	UiStatGet(void* A_PtrBuf);
		BOOL8	UiStatLoad(void* A_PtrBuf, __u32 A_Size);

		// user implement
		virtual	BOOL8	ScrollByItemHndl(__u32 A_HndlItem);
};

//====================================================================
//global function



//====================================================================
//out Enviroment conditional
#endif

