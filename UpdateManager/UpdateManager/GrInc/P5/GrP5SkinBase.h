/*
Platform 5 skin base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5SkinBase
#define	_EPC_GrP5SkinBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrGdibObj.h>

#include <P5/GrP5Base.h>

//====================================================================
//constance

#define	E_GrP5SkinArwMaxCnt							4

#define	E_GrP5SkinArwStdRectMaxCnt			8

#define	E_GrP5SkinCalDayLineCnt					6

#define	E_GrP5SkinPnlGrpBarItemMaxCnt		32
#define	E_GrP5SkinPnlGrpBarGridMaxCnt		16

#define	E_GrP5SkinPnlGrpLineColMaxCnt		32
#define	E_GrP5SkinPnlGrpLineGridMaxCnt	16
#define	E_GrP5SkinPnlGrpLineGrpMaxCnt		8

#define	E_GrP5SkinQuadDrawAreaMaxCnt		128

#define	E_GrP5SkinSpinItemCnt						3

#define	E_GrP5SkinTblLineMaxCnt					32
#define	E_GrP5SkinTblColumMaxCnt				16

#define	E_GrP5SkinGrpHzT2RulTxtMaxCnt		84

#define	E_GrP5SkinWinPopMenuItemMaxCnt	32

// spin item index
#define	E_GrP5SkinSpinIdxCont						0		/* contents */
#define	E_GrP5SkinSpinIdxBtnDec					1		/* decrease button */
#define	E_GrP5SkinSpinIdxBtnInc					2		/* increase button */

// dir map
#define	E_GrP5SkinDirMapLeft						0x01
#define	E_GrP5SkinDirMapRight						0x02
#define	E_GrP5SkinDirMapUp							0x04
#define	E_GrP5SkinDirMapDown						0x08

// scroll bar index
#define	E_GrP5SkinSbarIdxTrackInc				0
#define	E_GrP5SkinSbarIdxTrackDec				1
#define	E_GrP5SkinSbarIdxBtnInc					2
#define	E_GrP5SkinSbarIdxBtnDec					3
#define	E_GrP5SkinSbarIdxThumb					4

#ifdef GR_P5_EDITOR

#define	E_GrP5SkinCfgParaCnt						8

#endif

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5SkinCfg
{
	__u8	SkinId;
	__u8	TooltipFontId;
	__u8	_rsvd1;
	__u8	_rsvd2;

	St_GrP5FontDrawStyle	TooltipTxtStyle;

}	*Ptr_GrP5SkinCfg;

typedef struct St_GrP5SkinCtlSbar
{
	St_GrRect	RcCtl;		// total control
	__u32	BtnDecPos;		// up , left
	__u32	BtnIncPos;		// down , right
	__u32	BtnStepSize;	// inc dec button size
	__u32	BtnMovSize;		// mover size
	__u32	BtnMovPos;		// mover position

}	*Ptr_GrP5SkinCtlSbar;

typedef struct St_GrP5SkinArwItem
{
	St_GrSwordPos	St;
	St_GrSwordPos	Ed;
	__u32	DirMap;		// direction map
}	*Ptr_GrP5SkinArwItem;

typedef struct St_GrP5SkinCtlArwBase
{
	// by obj
	BOOL8	IsKeyCvrOn;		// key corver on

	__u32	CellxCnt;
	__u32	CellyCnt;

	__s32	CusIdxX;
	__s32	CusIdxY;

	St_GrP5SkinArwItem	ArwItms[E_GrP5SkinArwMaxCnt];

	// by skin
	__u32	CellSizeX;
	__u32	CellSizeY;


}	*Ptr_GrP5SkinCtlArwBase;

typedef struct St_GrP5SkinCtlArwStd
{
	// by obj
	St_GrRect	RcTbl[E_GrP5SkinArwStdRectMaxCnt];
}	*Ptr_GrP5SkinCtlArwStd;

typedef struct St_GrP5SkinCtlBtnBase
{
	BOOL8	IsPush;
	BOOL8	IsSelect;
	BOOL8	IsChked;		// check button use
	__u8	_rsvd1;

	WCHAR*	Str;
}	*Ptr_GrP5SkinCtlBtnBase;


typedef struct St_GrP5SkinCalDayItem
{
	__u8	Day;			// 0 = not use
	__u8	MarkMap;	// mark map
}	*Ptr_GrP5SkinCalDayItem;

typedef struct St_GrP5SkinCtlCalBase
{

	// by obj
	__u8			ViewMonth;
	__u8			SelMonth;
	__u8			SelDay;
	__u8			_rsvd0;

	__u16			ViewYear;
	__u16			SelYear;

	__s32			CusIdxX;
	__s32			CusIdxY;

	St_GrP5SkinCalDayItem	DayTbl[E_GrP5SkinCalDayLineCnt][E_GrTimeDayOfWeek];

	// by skin
	__u32			DayVwSizeX;		// day view size x
	__u32			DayVwSizeY;		// day view size y

}	*Ptr_GrP5SkinCtlCalBase;

typedef struct St_GrP5SkinCtlLblBase
{
	__u32	RtlStat;
	__u32	ClrCus;
	__s32	CusPos;
	WCHAR*	Str;			// display text

}	*Ptr_GrP5SkinCtlLblBase;

typedef struct St_GrP5SkinCtlBtnVkey
{
	__u32	LangIdx;
	__u32	ShftIdx;
}	*Ptr_GrP5SkinCtlBtnVkey;

typedef struct St_GrP5SkinCtlGridBase
{
	// by rtl
	__u32		CellCntX;
	__u32		CellCntY;

	__s32		CusIdxX;
	__s32		CusIdxY;

	__s32		DragStX;
	__s32		DragStY;
	__s32		DragEdX;
	__s32		DragEdY;

	// by skin
	__u32		CellSizeX;
	__u32		CellSizeY;

}	*Ptr_GrP5SkinCtlGridBase;

typedef struct St_GrP5SkinCtlGrpHzBase
{
	// by obj
	__s32			SelIdxX;
	__s32			SelIdxY;

	__s32			CusIdxX;
	__s32			CusIdxY;

	__u32			CellCntX;		// graph x count
	__u32			CellCntY;		// graph y count

	// by skin

}	*Ptr_GrP5SkinCtlGrpHzBase;

typedef struct St_GrP5SkinCtlGrpHzDmap
{
	__u32	CellSizeX;
	__u32	CellSizeY;
	__u32	VwCntY;
}	*Ptr_GrP5SkinCtlGrpHzDmap;

typedef struct St_GrP5SkinCtlGrpHzT1
{
	BOOL8	IsDayMode;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	CellSizeX;
	__u32	CellSizeY;
	__u32	VwCntX;
	__u32	VwCntY;

	WCHAR*	StrInd;

	St_GrRect	RcContArea;		// contents area

}	*Ptr_GrP5SkinCtlGrpHzT1;

typedef struct St_GrP5SkinGrpHzT2TxtItm
{
	St_GrRect	Rc;
	WCHAR*	Str;
}	*Ptr_GrP5SkinGrpHzT2TxtItm;

typedef struct St_GrP5SkinGrpHzT2ContItm
{
	__u16	EndPos;
	__u8	Val;
	__u8	_rsvd0;
}	*Ptr_GrP5SkinGrpHzT2ContItm;

typedef struct St_GrP5SkinGrpHzT2ContCh
{
	__u32	Cnt;
	St_GrP5SkinGrpHzT2ContItm	Itms[E_GrTimeMinOfDay];
}	*Ptr_GrP5SkinGrpHzT2ContCh;

typedef struct St_GrP5SkinCtlGrpHzT2
{
	__u32	TimeLoad;		// time table loaded - day unit - by use
	__u32	TimeScl;		// time scroll position - by user
	__u32	TimeLen;		// view time length

	__u32	TimeCus;		// cursor time

	__u8	StartCh;		// no merge mode only - view start channel - by user
	__u8	ZoomLvl;		// zoom level - by user
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	PxPerSec;
	__u32	RulPerPx;
	__u32	RulPerSec;
	
	__u32	VwCntX;
	__u32	VwCntY;

	__u32	RulTxtCnt;
	
	St_GrRect	RcContArea;		// contents area
	St_GrRect	RcRule;				// ruler 

	St_GrP5SkinGrpHzT2TxtItm	RulTxtTbl[E_GrP5SkinGrpHzT2RulTxtMaxCnt];		// ruler text table
	St_GrP5SkinGrpHzT2ContCh	ContTbl[E_GrDvrMaxChCnt];

	__u8*	PtrAtbBuf;		// by user

}	*Ptr_GrP5SkinCtlGrpHzT2;


typedef struct St_GrP5SkinGrpBarItem
{
	WCHAR*	StrName;
	__u32	Color;
	__u32	BarHeight;
	WCHAR*	StrVal;
}	*Ptr_GrP5SkinGrpBarItem;

typedef struct St_GrP5SkinGrpBarGrid
{
	WCHAR*	StrVal;
}	*Ptr_GrP5SkinGrpBarGrid;

typedef struct St_GrP5SkinCtlPnlDir
{
	__u32	StatLeft;
	__u32	StatUp;
	__u32	StatRight;
	__u32	StatDown;

	St_GrRect	RcLeft;
	St_GrRect	RcUp;
	St_GrRect	RcRight;
	St_GrRect	RcDown;
}	*Ptr_GrP5SkinCtlPnlDir;

typedef struct St_GrP5SkinCtlPnlGrpBar
{
	__u32		ItemCnt;

	St_GrP5SkinGrpBarItem	Items[E_GrP5SkinPnlGrpBarItemMaxCnt];
	St_GrP5SkinGrpBarGrid	Grids[E_GrP5SkinPnlGrpBarGridMaxCnt];

	// by skin
	__s32		PntItmWidth;

}	*Ptr_GrP5SkinCtlPnlGrpBar;

typedef struct St_GrP5SkinPnlGrpLineCol
{
	__s32	Pos;
	WCHAR*	StrName;
}	*Ptr_GrP5SkinPnlGrpLineCol;

typedef struct St_GrP5SkinPnlGrpLineGrid
{
	WCHAR*	StrVal;
}	*Ptr_GrP5SkinPnlGrpLineGrid;

typedef struct St_GrP5SkinPnlGrpLineItm
{
	__s32	Pos;		// position on screen
}	*Ptr_GrP5SkinPnlGrpLineItm;

typedef struct St_GrP5SkinPnlGrpLineGrp
{
	BOOL8	IsDraw;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	St_GrP5SkinPnlGrpLineItm	Cols[E_GrP5SkinPnlGrpLineColMaxCnt];

}	*Ptr_GrP5SkinPnlGrpLineGrp;

typedef struct St_GrP5SkinCtlPnlGrpLine
{
	__u32		ColCnt;					// colum count
	__u32		GrpCnt;					// graph count

	St_GrP5SkinPnlGrpLineCol	Cols[E_GrP5SkinPnlGrpLineColMaxCnt];
	St_GrP5SkinPnlGrpLineGrid	Grids[E_GrP5SkinPnlGrpLineGridMaxCnt];
	St_GrP5SkinPnlGrpLineGrp	Grps[E_GrP5SkinPnlGrpLineGrpMaxCnt];

	// by skin
	__s32			PntColWidth;	// colum width	

}	*Ptr_GrP5SkinCtlPnlGrpLine;

typedef struct St_GrP5SkinCtlPrgsBase
{
	__u32	ValMax;
	__u32	ValNow;
	WCHAR*	StrTxt;
}	*Ptr_GrP5SkinCtlPrgsBase;

typedef struct St_GrP5SkinQuadDrawInfo
{
	__u32		Cnt;
	St_GrRect	RcTbl[E_GrP5SkinQuadDrawAreaMaxCnt];
}	*Ptr_GrP5SkinQuadDrawInfo;

typedef struct St_GrP5SkinQuadT1View
{
	__s8	Ch;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	St_GrRect	RcView;			// view total area
	St_GrRect	RcInArea;		// except waku
	St_GrRect	RcName;
	St_GrRect	RcIconRec;
	St_GrRect	RcIconMtn;
	St_GrRect	RcIconSen;
	St_GrRect	RcIconIa;
	St_GrRect	RcIconPtz;
	St_GrRect	RcIconAdo;

}	*Ptr_GrP5SkinQuadT1View;

typedef struct St_GrP5SkinCtlQuadT1
{
	BOOL8	IsShowName;
	BOOL8	IsShowRec;
	BOOL8	IsShowEvt;
	BOOL8	IsShowPtz;

	BOOL8	IsShowAudio;
	BOOL8	IsRecEvtShow;

	__u32	RecChMap;
	__u32	MtnChMap;
	__u32	SenChMap;
	__u32	AdoChMap;
	__u32	PtzChMap;
	__u32	IaChMap;

	__u8	RatbTbl[E_GrDvrMaxChCnt];

	__u8	ChToVwTbl[E_GrDvrMaxChCnt];
	__u32	ViewCnt;
	St_GrP5SkinQuadT1View	Views[E_GrDvrMaxChCnt];

	St_GrP5SkinQuadDrawInfo	Dreq;		// draw request info

}	*Ptr_GrP5SkinCtlQuadT1;

typedef struct St_GrP5SkinCtlSpinBase
{
	BOOL8	IsPush;
	BOOL8	IsKeyCoverOn;
	__u8	ItmIdx;			// control item index
	__u8	_rsvd0;
	WCHAR*	StrDisp;		// display text

	// by skin
	St_GrRect	RcText;
	St_GrRect	RcItems[E_GrP5SkinSpinItemCnt];
}	*Ptr_GrP5SkinCtlSpinBase;

typedef struct St_GrP5SkinCtlTabBase
{
	St_GrRect	RcCont;
}	*Ptr_GrP5SkinCtlTabBase;

typedef struct St_GrP5SkinCtlTabTiRel
{
	St_GrRect	RcLblBg;
	St_GrRect	RcLblTxt;
	St_GrRect	RcIcon;
}	*Ptr_GrP5SkinCtlTabTiRel;

typedef struct St_GrP5SkinTblColumCtrl
{
	__s32	Width;
	__s32	Pos;
}	*Ptr_GrP5SkinTblColumCtrl;

typedef struct St_GrP5SkinTblLineCtrl
{
	St_GrP5TblItem	Cols[E_GrP5SkinTblColumMaxCnt];
}	*Ptr_GrP5SkinTblLineCtrl;

typedef struct St_GrP5SkinCtlTblBase
{
	// by obj
	__u32		ItemCnt;

	__s32			CusX;					// display index
	__s32			CusY;

	__s32			SelX;
	__s32			SelY;

	// by skin
	__u32	ViewLineCnt;
	__u32	ViewLineMax;
	__u32	ColumCnt;

	St_GrP5SkinTblColumCtrl	Cols[E_GrP5SkinTblColumMaxCnt];
	St_GrP5SkinTblLineCtrl	Lines[E_GrP5SkinTblLineMaxCnt];

}	*Ptr_GrP5SkinCtlTblBase;

typedef struct St_Grp5SkinSbarCtl
{
	// by obj
	__u32	BtnDecStat;
	__u32	BtnIncStat;
	__u32	ThumbStat;

	__s32	Val;					// value
	__s32	Max;					// max value

	// by skin
	St_GrRect	RcArea;		// scroll bar area
	St_GrRect	RcBtnInc;
	St_GrRect	RcBtnDec;
	St_GrRect	RcTrack;

	__s32		ThumbSize;	// thumbnail size

	__s32		Pos;
	__s32		End;				// end position
}	*Ptr_Grp5SkinSbarCtl;

typedef struct St_GrP5SkinCtlWinPopItemBase
{
	St_Grp5SkinSbarCtl	Sbar;

	// by obj
	__s32		CusMseIdx;		// mouse cursor index
	__u32		CusKeyIdx;		// key cursor index

	// by skin
	__s32	ItemHeight;
	__s32	ItemWidth;
	__s32	ViewCnt;

}	*Ptr_GrP5SkinCtlWinPopItemBase;

typedef struct St_GrP5SkinCtlWinPopMenu
{
	__u32		ItemCnt;
	Ptr_GrP5Item	Items[E_GrP5SkinWinPopMenuItemMaxCnt];

}	*Ptr_GrP5SkinCtlWinPopMenu;

typedef struct St_GrP5SkinCtlCbxBase
{
	BOOL8		IsPush;
	WCHAR*	StrDisp;		// display text

}	*Ptr_GrP5SkinCtlCbxBase;

typedef struct St_GrP5SkinCtlPnlDrag
{
	St_GrRect	RcDrag;
}	*Ptr_GrP5SkinCtlPnlDrag;

typedef struct St_GrP5SkinTooltipCtl
{
	__u16	TxtId;
	__u16	_rsvd0;
	St_GrRect	RcDraw;
}	*Ptr_GrP5SkinTooltipCtl;


//====================================================================
//global class

class Cls_GrP5SkinBase
{
	protected:
		__u8	m_SkinId;

		// draw api
		void	ReqPntGdib(Ptr_GrGdib A_PtrGdib, Ptr_GrRect A_PtrRcScr, __u8 A_AlignHz, __u8 A_AlignVt, BOOL8 A_IsSprite);
		void	ReqPntRectangle(Ptr_GrRect A_PtrRcScr, __u32 A_Width, __u32 A_Color);

	public:
		Cls_GrP5SkinBase(__u8 A_SkinId);
		virtual	~Cls_GrP5SkinBase(void);

		__u8	SkinIdGet(void);

		// implement function

		// control
		virtual	void	CtlUpdateBase(void* A_ObjUi) = 0;

		virtual	void	CtlDrawBase(void* A_ObjUi) = 0;

		virtual	void	CtlGetItemOneRect(void* A_ObjUi, __s32 A_ItmIdxX, __s32 A_ItmIdxY, Ptr_GrRect A_PtrRtRect);
		virtual	void	CtlGetItemRangeRect(void* A_ObjUi, __s32 A_ItmStX, __s32 A_ItmStY, __s32 A_ItmEdX, __s32 A_ItmEdY, Ptr_GrRect A_PtrRtRect) = 0;
		virtual	void	CtlGetItemIdxByPos(void* A_ObjUi, __s32 A_X, __s32 A_Y, __s32* A_PtrRtIdxX, __s32* A_PtrRtIdxY) = 0;

		virtual	void	CtlGetSizeByItemCnt(__u16 A_ClassId, __u32 A_ItemCnt, __u32* A_PtrRtSizeX, __u32* A_PtrRtSizeY);

		virtual	void	CtlCalcSbarVtByValue(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, __u32 A_Value);
		virtual	void	CtlCalcSbarVtByPos(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, __u32 A_Pos);
		virtual	void	CtlGetSbarVtThumbRc(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, Ptr_GrRect A_PtrRtRc);

		virtual	void	CtlCalcSbarHzByValue(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, __u32 A_Value);
		virtual	void	CtlCalcSbarHzByPos(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, __u32 A_Pos);
		virtual	void	CtlGetSbarHzThumbRc(void* A_ObjUi, Ptr_Grp5SkinSbarCtl A_PtrSbarCtl, Ptr_GrRect A_PtrRtRc);

		// tooltip
		virtual	void	TooltipSetup(Ptr_GrRect A_PtrConRect, Ptr_GrRect A_PtrBaseRect, __u16 A_TxtId, Ptr_GrP5SkinTooltipCtl A_PtrRtCtl);	// A_TxtId = 0 - not display
		virtual	void	TooltipDraw(Ptr_GrP5SkinTooltipCtl A_PtrCtl);

};


//====================================================================
//global function

void	GrP5SkinInit(void);
void	GrP5SkinFinish(void);

Cls_GrP5SkinBase*	GrP5SkinGet(void);
void	GrP5SkinSetByCfg(void);

#ifdef GR_P5_EDITOR
__u8	GrP5SkinDvlCountGet(void);
WCHAR*	GrP5SkinDvlNameGet(__u8 A_SkinId);
void	GrP5SkinDvlFontColorSet(Ptr_GrP5FontDrawStyle A_PtrStyle);
#endif

//====================================================================
//out Enviroment conditional
#endif

