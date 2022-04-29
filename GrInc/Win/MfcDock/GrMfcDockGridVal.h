/*
	Grid value

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrMfcDockGridVal
#define	_EPC_GrMfcDockGridVal

//====================================================================
// uses
#include <afxwin.h>
#include <Win\MfcDock\GrMfcDockCtrlBase.h>

#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrGdibObj.h>


//====================================================================
//constance

#define	E_GrMfcDockGridValUiStatValId					1

#define	E_GrMfcDockGridValColumMaxCnt					64
#define	E_GrMfcDockGridValColumMinWidth				10

#define	E_GrMfcDockGridValItemVtMargin				2

#define	E_GrMfcDockGridValItemTxtHzGap				3

#define	E_GrMfcDockGridValHeadVtMargin				3

#define	E_GrMfcDockGridValItemDfltMax					4096
#define	E_GrMfcDockGridValItemAbleMaxCnt			0xFFF0

#define	E_GrMfcDockGridValHndlItemNone				0
#define	E_GrMfcDockGridValHndlElemBase				1

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrMfcDockGridValColumCtrl
{
	__u32		TxtArrange;
	WCHAR*	StrTxt;
}	*Ptr_GrMfcDockGridValColumCtrl;

typedef struct St_GrMfcDockGridValUiStat
{
	__u32		WidthTbl[E_GrMfcDockGridValColumMaxCnt];
}	*Ptr_GrMfcDockGridValUiStat;


typedef struct St_GrMfcDockGridValItemColum
{
	St_GrMfcDockCtrlBaseValueInfo	ValInfo;
	BOOL8	IsLocal;			// local value, control must be release value buffer
	BOOL8	IsMark;				// mark back ground
	COLORREF	ColorNormal;
	COLORREF	ColorMark;	// mark color
	__u64	UserData;

}	*Ptr_GrMfcDockGridValItemColum;

typedef struct St_GrMfcDockGridValItemCtrl
{
	St_GrMfcDockGridValItemColum	Colum[E_GrMfcDockGridValColumMaxCnt];
}	*Ptr_GrMfcDockGridValItemCtrl;

//====================================================================
//global class

class Cls_GrMfcDockGridVal : public Cls_GrMfcDockCtrlBase
{
	protected:

		BOOL8	m_IsDrawHead;
		BOOL8	m_IsDrawAble;			// draw on item add

		BOOL8	m_IsCanWidthChg;	// can width change

		St_GrMfcDockGridValUiStat			m_UiStat;

		int		m_ColumCnt;
		int		m_ColumWidthSum;
		St_GrMfcDockGridValColumCtrl	m_ColumCtrl[E_GrMfcDockGridValColumMaxCnt];

		int		m_ItemMaxCnt;
		int		m_ItemCnt;			// line count
		Ptr_GrMfcDockGridValItemCtrl		m_PtrItems;

		// rect
		RECT	m_RcItemArea;
		RECT	m_RcHeadArea;
		RECT	m_RcItemCont;
		int		m_ItemHeight;
		int		m_ItemViewAble;
		int		m_HeaderHeight;
		int		m_ItemViewCnt;		// now display item count

		// resource
		HPEN	m_PenHdSepLt;
		HPEN	m_PenHdSepRt;
		HPEN	m_PenCtSepLt;
		HPEN	m_PenCtSepRt;
		HPEN	m_PenCtUline;

		HGDIOBJ	m_FontOld;


		void	LcDimensionCalc( RECT* A_PtrRc );
		__u8	LcPosInfoGet(int A_X, int A_Y, int* A_PtrRtItem, int* A_PtrRtColum, int* A_PtrRtValPosX, int* A_PtrRtValPosY);
		int		LcXposToColumIdx(int A_PosX, BOOL8 A_IsChkNearGrid, int* A_PtrRtPosX, BOOL8* A_PtrRtNeerGrid);
		int		LcColumWidthSumCalc(int A_Colum);
		BOOL8	LcItemRectGetByIdx(int A_ItemIdx, int A_Colum, RECT* A_PtrRtRect);
		void	LcColumWidthSumUpdate(void);

		void	LcScrlbarHzUpdt(void);
		void	LcScrlbarVtUpdt( void );

		void	LcDrawHeaders(CDC* A_Dc);
		void	LcDrawHeaderOne(CDC* A_Dc, RECT* A_PtrRc, int A_Colum);
		void	LcDrawItems( CDC* A_Dc );
		void	LcDrawCont( CDC* A_Dc );
		void	LcDrawItem( CDC* A_Dc, RECT* A_PtrRc, int A_ItemIdx );

		// inherited
		virtual	Ptr_GrMfcDockCtrlBaseValueInfo	ValueInfoGetByItemHndl(__u32 A_HndlItem);
		virtual	BOOL8	ItemRectGetByItemHndl(__u32 A_HndlItem, RECT* A_PtrRtRect);
		virtual	__u8	PosInfoGet(int A_X, int A_Y, __u32* A_PtrRtHndlItem, int* A_PtrRtItemPosX, int* A_PtrRtItemPosY);
		virtual	int*	GridSizeValuePtrGet(__u32 A_HndlItem, __u8 A_Mode);
		virtual	void	OnGridSizeChange(void);		// grid size change event

		// message

		afx_msg void OnPaint( );
		afx_msg void OnSize(UINT nType, int cx, int cy);

		DECLARE_MESSAGE_MAP()
	public:
		Cls_GrMfcDockGridVal( void );
		virtual	~Cls_GrMfcDockGridVal( void );

		void	WidthControlSet(BOOL8 A_IsAble);

		BOOL	Create(RECT& A_Rect, CWnd* A_WndParent, UINT A_Id, __u32 A_ItemMaxCnt = E_GrMfcDockGridValItemDfltMax);
		void	ClearAll( void );
		void	ClearItems(void);

		BOOL8	HeaderSet(int A_Colum, WCHAR* A_StrTxt, __u32 A_TxtArrage = 0);
		void	HeaderShow(BOOL8 A_IsShow);

		BOOL8	ColumAdd(int A_Width);

		void	DrawLock( BOOL8 A_IsDrawAble );
		int		ItemAdd( void );
		BOOL8	ItemSetValue(int A_ItemIdx, int A_Colum, __u16 A_ValType, void* A_PtrVal, __u16 A_ValCnt, BOOL8 A_IsEdit, __u32 A_TxtArrage = 0);
		BOOL8	ItemSetLocalText(int A_ItemIdx, int A_Colum, WCHAR* A_StrVal, __u32 A_TxtArrage = 0);
		__u64	ItemGetUserData( int A_ItemIdx, int A_ItemColum );
		BOOL8	ItemSetUserData(int A_ItemIdx, int A_ItemColum, __u64 A_UserData);

		BOOL8	ItemDel(int A_ItemIdx);

		BOOL8	ItemSetColor(int A_ItemIdx, int A_ItemColum, COLORREF A_Color);
		BOOL8	ItemSetMarkStatus(int A_ItemIdx, int A_ItemColum, BOOL8 A_IsMark);
		BOOL8	ItemSetMarkColor(int A_ItemIdx, int A_ItemColum, COLORREF A_Color);
		BOOL8	IsItemMarked(int A_ItemIdx, int A_ItemColum);

		BOOL8	ItemSetModifiedColum(int A_Colum, BOOL8 A_IsModified);

		void	ItemHndlDecode(__u32 A_HndlItem, int* A_PtrRtLine, int* A_PtrRtColum);
		__u32	ItemHndlEncode(int A_Line, int A_Colum);

		int		ItemCountGet(void);
		int		ScrollPosVtGet(void);
		void	ScrollPosVtSet(int A_ScrlPos);

		BOOL8	ScrollByIdx(int A_ItemIdx, int A_Colum);

		virtual	BOOL8	ItemSetHeight(int A_Height);		// inheritable
		int		ItemGetHeight(void);

		// user implement
		virtual	BOOL8	ScrollByItemHndl(__u32 A_HndlItem);
};

//====================================================================
//global function



//====================================================================
//out Enviroment conditional
#endif

