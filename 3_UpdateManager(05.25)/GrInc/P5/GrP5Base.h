/*
	Platform 5 base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5Base
#define	_EPC_GrP5Base

//====================================================================
// option

#define	E_GrP5DbgMsgWarnning				/* warnning message display */


//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

#include <GrStrTool.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>
#include <GrGdibObj.h>

#include <Dvr/GrDvrBase.h>
#include <P5/GrP5Paint.h>

#include <FontV3/GrFontV3DrawBase.h>
#include <P5/GrP5Text.h>

#include <GrLayerData.h>

//====================================================================
//constance

#define	E_GrP5ConMaxCnt							2		/* max console count */

#define	E_GrP5LayerMaxCnt						8		/* max layer count */
#define	E_GrP5WinCntForLayer				8		/* window for layer count */

#define	E_GrP5LayerIndexByScript		0xFF

#define	E_GrP5SlaveObjMaxCnt				1024

#define	E_GrP5CtrlIdMaxCnt					128

#define	E_GrP5KeyMapTblCnt					8

#define	E_GrP5TimerMaxCnt						128

#define	E_GrP5UserIdMaxCnt					1024
#define	E_GrP5UserIdNone						0
#define	E_GrP5userIdBase						1

#define	E_GrP5VkbdShftMax						4

#define	E_GrP5ItemMaxCnt						128
#define	E_GrP5ItemStrBufLen					31

#define	E_GrP5ItemSquadIdNone				0

#define	E_GrP5PopMaxDepth						4

// status
// constant status
#define	E_GrP5StatNormal						0
#define	E_GrP5StatDisable						1
#define	E_GrP5StatPush							2

#define	E_GrP5StatConstMask					0x000000FF

// bitmap status
#define	E_GrP5StatOnFocus						0x00000100
#define	E_GrP5StatOnChecked					0x00000200
#define	E_GrP5StatOnSelected				0x00000400
#define	E_GrP5StatOnCtrlCus					0x00000800

#define	E_GrP5StatBitmapMask				0x0000FF00

// style status
#define	E_GrP5StatStyleLeft					0x00010000
#define	E_GrP5StatStyleUp						0x00020000
#define	E_GrP5StatStyleRight				0x00040000
#define	E_GrP5StatStyleBottom				0x00080000

#define	E_GrP5StatStyleMapMask			0x00FF0000


// paint
#define	E_GrP5PntStackMaxCnt				0x10000
#define	E_GrP5PntClipMaxCnt					1024
#define	E_GrP5PntSctMaxCnt					64

// relate rect
#define	E_GrP5RelRectRatioBaseShft	7
#define	E_GrP5RelRectRatioBaseMax		(1 << E_GrP5RelRectRatioBaseShft)

// script info
#define	E_GrP5ScptWinMaxCnt					128

#define	E_GrP5ScptFontMaxCnt				8

#define	E_GrP5ScptImgMaxCnt					256

#define	E_GrP5ScptTxtUserMaxCnt			1024

#define	E_GrP5ScptLanguageMaxCnt		32

// setup
#define	E_GrP5SetupBufSize					0x10000		/* 64Kbyte */

// console expansion type
#define	E_GrP5ConExpNone						0		/* not expand */
#define	E_GrP5ConExpRight						1		/* right direction expand */
#define	E_GrP5ConExpLeft						2		/* left direction expand */
#define	E_GrP5ConExpUp							3		/* up direction expand */
#define	E_GrP5ConExpDown						4		/* down direction expand */

// control id
#define	E_GrP5CtrlIdNone						0
#define	E_GrP5CtrlIdBase						1

// register id
#define	E_GrP5RegIdNone							0
#define	E_GrP5RegIdBase							1

// script id
#define	E_GrP5ScptIdNone						0

#define	E_GrP5ScptIdWinBase					1

#define	E_GrP5ScptIdSlaveBase				1000

// direction
#define	E_GrP5DirLeft								0
#define	E_GrP5DirUp									1
#define	E_GrP5DirRight							2
#define	E_GrP5DirDown								3
#define	E_GrP5DirCnt								4

// font
#define	E_GrP5FontAlignLeft					E_GrFontV3AlignLeft
#define	E_GrP5FontAlignRight				E_GrFontV3AlignRight
#define	E_GrP5FontAlignCenterHz			E_GrFontV3AlignCenterHz

#define	E_GrP5FontAlignTop					E_GrFontV3AlignTop
#define	E_GrP5FontAlignBottom				E_GrFontV3AlignBottom
#define	E_GrP5FontAlignCenterVt			E_GrFontV3AlignCenterVt

#define	E_GrP5FontAtbDrawText				E_GrFontV3AtbDrawText
#define	E_GrP5FontAtbDrawShadow			E_GrFontV3AtbDrawShadow
#define	E_GrP5FontAtbWordWarp				E_GrFontV3AtbWordWarp
#define	E_GrP5FontAtbFixPitch				E_GrFontV3AtbFixPitch
#define	E_GrP5FontAtbPassword				E_GrFontV3AtbPassword
#define	E_GrP5FontAtbSemiEnter			E_GrFontV3AtbSemiEnter

#define	E_GrP5FontStatNormal				E_GrFontV3StatNormal
#define	E_GrP5FontStatDisable			  E_GrFontV3StatDisable
#define	E_GrP5FontStatOnFocus				E_GrFontV3StatOnFocus	
#define	E_GrP5FontStatOnChecked			E_GrFontV3StatOnChecked
#define	E_GrP5FontStatOnSelected		E_GrFontV3StatOnSelected

#define	E_GrP5FontTxtClrNormal			E_GrFontV3TxtClrNormal
#define	E_GrP5FontTxtClrDisable			E_GrFontV3TxtClrDisable
#define	E_GrP5FontTxtClrFocus				E_GrFontV3TxtClrFocus
#define	E_GrP5FontTxtClrSelect			E_GrFontV3TxtClrSelect

#define	St_GrP5FontDrawStyle				St_GrFontV3DrawStyle
#define	Ptr_GrP5FontDrawStyle				Ptr_GrFontV3DrawStyle

#define	Cls_GrP5Font								Cls_GrFontV3DrawBase

// image
#define	E_GrP5ImgIdNone							0
#define	E_GrP5ImgIdBase							1

#define	E_GrP5ImgUserIdBase					8000

#define	E_GrP5ImgUserMaxCnt					64

// data id
#define	E_GrP5DataIdFont						(0 + E_GrLayerDataIdBase)		/* para 0 : font id */
#define	E_GrP5DataIdImage						(1 + E_GrLayerDataIdBase)		/* para 0 : image id */
#define	E_GrP5DataIdWinScript				(2 + E_GrLayerDataIdBase)		/* para 0 : script id */
#define	E_GrP5DataIdSkinCfg					(3 + E_GrLayerDataIdBase)		/*  */
#define	E_GrP5DataIdText						(4 + E_GrLayerDataIdBase)		/* para 0 : id, para 1 : language */
#define	E_GrP5DataIdVkbdCfg					(5 + E_GrLayerDataIdBase)		/*  */

// object id
#define	E_GrP5ObjIdNone							0

#define	E_GrP5ObjIdBitWindow				0x4000		/* indicate window object id */

#define	E_GrP5ObjIdSlaveIdxBitMask	0x0FFF
#define	E_GrP5ObjIdSlaveIdxBitPos		2
#define	E_GrP5ObjIdSlaveBase				1

#define	E_GrP5ObjIdConIdxBitPos			15
#define	E_GrP5ObjIdConIdxBitMask		0x0001
#define	E_GrP5ObjIdWinLayerBitPos		10
#define	E_GrP5ObjIdWinLayerBitMask	0x000F
#define	E_GrP5ObjIdWinIdxBitPos			2
#define	E_GrP5ObjIdWinIdxBitMask		0x00FF

#define	E_GrP5ObjIdLocalIdMask			0x0003

#define	E_GrP5TblTextMaxLen					63

// table item control type
#define	E_GrP5TblCtlNone						0
#define	E_GrP5TblCtlTextLocal				1
#define	E_GrP5TblCtlImage						2
#define	E_GrP5TblCtlTextId					3
#define	E_GrP5TblCtlValue						4

// key focus para
#define	E_GrP5FocusParaByLeft				0x00000001
#define	E_GrP5FocusParaByUp					0x00000002
#define	E_GrP5FocusParaByRight			0x00000004
#define	E_GrP5FocusParaByDown				0x00000008

#define	E_GrP5FocusParaByKeyMask		0x0000000F

// class id
#define	E_GrP5ClassIdNone						0
#define	E_GrP5ClassIdUoBase					1
#define	E_GrP5ClassIdWinBase				2
#define	E_GrP5ClassIdTabBase				3
#define	E_GrP5ClassIdCbxBase				4
//#define	E_GrP5ClassIdSbarBase				5
#define	E_GrP5ClassIdSpinBase				6


#define	E_GrP5ClassIdWinStd					10
#define	E_GrP5ClassIdWinChild				11
#define	E_GrP5ClassIdWinPopList			12
#define	E_GrP5ClassIdWinVkbd				13
#define	E_GrP5ClassIdWinPopItemBase	14
#define	E_GrP5ClassIdWinPopMenu			15

#define	E_GrP5ClassIdTabTiRel				20
#define	E_GrP5ClassIdTabStd					21

#define	E_GrP5ClassIdLblBase				30
#define	E_GrP5ClassIdLblTid					31
#define	E_GrP5ClassIdLblRef					32
#define	E_GrP5ClassIdLblLt					33
#define	E_GrP5ClassIdLblTimeBase		34
#define	E_GrP5ClassIdLblTimeReg			35
#define	E_GrP5ClassIdLblTimeView		36

#define	E_GrP5ClassIdBtnBase				40
#define	E_GrP5ClassIdBtnTabBase			41
#define	E_GrP5ClassIdBtnTabTid			42
#define	E_GrP5ClassIdBtnTid					43
#define	E_GrP5ClassIdBtnTime				44
#define	E_GrP5ClassIdBtnVkey				45
#define	E_GrP5ClassIdBtnStxt				46
#define	E_GrP5ClassIdBtnImgBase			47
#define	E_GrP5ClassIdBtnImgTid			48
#define	E_GrP5ClassIdBtnSint				49

#define	E_GrP5ClassIdPrgsBase				50
#define	E_GrP5ClassIdPrgsRint				51

#define	E_GrP5ClassIdQuadBase				60
#define	E_GrP5ClassIdQuadT1					61

#if 0  // not use
#define	E_GrP5ClassIdListBase				70
#define	E_GrP5ClassIdListLtxt				71
#define	E_GrP5ClassIdListRef				72
#endif

#define	E_GrP5ClassIdSbarHz					80
#define	E_GrP5ClassIdSbarVt					81

#define	E_GrP5ClassIdCbxTiBase			90
#define	E_GrP5ClassIdCbxTiSint			91
#define	E_GrP5ClassIdCbxLtBase			92
#define	E_GrP5ClassIdCbxLtSint			93
#define	E_GrP5ClassIdCbxLtLint			94

#define	E_GrP5ClassIdSpinNumBase		100
#define	E_GrP5ClassIdSpinNumSint		101
#define	E_GrP5ClassIdSpinStxtBase		102
#define	E_GrP5ClassIdSpinStxtRint		103
#define	E_GrP5ClassIdSpinNumLint		104
#define	E_GrP5ClassIdSpinLtBase			105
#define	E_GrP5ClassIdSpinLtLint			106

#define	E_GrP5ClassIdCalBase				110
#define	E_GrP5ClassIdCalMark				111
#define	E_GrP5ClassIdChkImgLtBase		112
#define	E_GrP5ClassIdChkImgLtLc			113

#define	E_GrP5ClassIdGrpHzBase			120
#define	E_GrP5ClassIdGrpHzDmap			121
#define	E_GrP5ClassIdGrpHzT1				122
#define	E_GrP5ClassIdGrpHzT2				123

#define	E_GrP5ClassIdChkTiBase			130
#define	E_GrP5ClassIdChkTiLc				131
#define	E_GrP5ClassIdChkTiReg				132
#define	E_GrP5ClassIdChkImgTiBase		133
#define	E_GrP5ClassIdChkImgTiLc			134

#define	E_GrP5ClassIdBtnLtxt				140

#define	E_GrP5ClassIdGridBase				150
#define	E_GrP5ClassIdGridSmd				151
#define	E_GrP5ClassIdGridSrscd			152

#define	E_GrP5ClassIdPnlStd					160
#define	E_GrP5ClassIdPnlAim					161
#define	E_GrP5ClassIdPnlDir					162
#define	E_GrP5ClassIdPnlGrpBar			163
#define	E_GrP5ClassIdPnlGrpLine			164
#define	E_GrP5ClassIdPnlDrag				165

#define	E_GrP5ClassIdTblBase				170
#define	E_GrP5ClassIdTblStd					171

#define	E_GrP5ClassIdSlidBase				180
#define	E_GrP5ClassIdSlidHzBase			181
#define	E_GrP5ClassIdSlidHzLint			182
#define	E_GrP5ClassIdSlidHzSint			183

#define	E_GrP5ClassIdArwBase				190
#define	E_GrP5ClassIdArwStd					191


// virtual keyboard
#define	E_GrP5VkbdTypeNormal				0
#define	E_GrP5VkbdTypeDec						1
#define	E_GrP5VkbdTypeHex						2
#define	E_GrP5VkbdTypePassword			3
#define	E_GrP5VkbdTypeUrl						4
#define	E_GrP5VkbdTypeEmail					5

#define	E_GrP5VkbdCnt								6

// text buffer
#define	E_GrP5TxtBufMaxLen					126
#define	E_GrP5TxtBufMaxCnt					512


//====================================================================
//global macro


//====================================================================
//global type

typedef struct St_GrP5PosElm
{
	__u8	Ratio;
	__u8	Para;
	__s16	Val;
}	*Ptr_GrP5PosElm;

typedef struct St_GrP5RelPos
{
	St_GrP5PosElm	X;
	St_GrP5PosElm	Y;
}	*Ptr_GrP5RelPos;

typedef struct St_GrP5RelRect
{
	St_GrP5PosElm	Left;
	St_GrP5PosElm	Top;
	St_GrP5PosElm	Right;
	St_GrP5PosElm	Bottom;
}	*Ptr_GrP5RelRect;

typedef struct St_GrP5SkinPara
{
	__u8	SubType;
	__u8	ParaByte0;
	__u8	ParaByte1;
	__u8	ParaByte2;
	__u16	ParaWord0;
	__u16	ParaWord1;
	__u32	ParaDword;
	__u32	ParaColor;
}	*Ptr_GrP5SkinPara;

typedef struct St_GrP5UoCtrl
{
	void*	UiObj;
	__u8	LocalId;
	__u8	_rsvd;
	__u16	ObjId;
}	*Ptr_GrP5UoCtrl;

typedef struct St_GrP5TimerCtrl
{
	void*	UiObj;
}	*Ptr_GrP5TimerCtrl;

typedef struct St_GrP5ConCtrl
{
	void*	ObjCon;		// console object
	__u8	ConIdx;		// console index
	BOOL8	IsPntNeed;	// paint need
	void*	UiQuad;

	St_GrP5UoCtrl	WinTbl[E_GrP5LayerMaxCnt][E_GrP5WinCntForLayer];

	St_GrP5UoCtrl	SlaveObjTbl[E_GrP5SlaveObjMaxCnt];

}	*Ptr_GrP5ConCtrl;

typedef struct St_GrP5Item
{
	__u16	TxtId;
	__u16	IconId;

	__u16	ChildSquad;		// child squad
	__u16	Squad;				// squad id

	__u32	Stat;

	__u32	UserData;			// user defined data
	WCHAR	StrBuf[E_GrP5ItemStrBufLen + 1];
}*Ptr_GrP5Item;

typedef struct St_GrP5TblItem
{
	__u8		Type;
	__u8		AlignHz;
	__u8		AlignVt;
	__u8		Para;
	__u16		RsrcId;		// resource id, text or image
	__u16		Stat;
	__u32		Value;
	__u32		UserData;
	WCHAR*	Str;		// local text buffer
}	*Ptr_GrP5TblItem;

typedef struct St_GrP5VkbdCfg
{
	__u16	WinIdNormal;
	__u16	WinIdDec;
	__u16	WinIdHex;
	__u16	WinIdPassword;
	__u16	WinIdUrl;
	__u16	WinIdEmail;

}	*Ptr_GrP5VkbdCfg;

typedef struct St_GrP5PntSctItem
{
	__u32		Ip;						// sectin start ip
	__s32		ClipIdx;			// start clip index
	St_GrSwordRect	Rc;
}	*Ptr_GrP5PntSctItem;

typedef struct St_GrP5PntClipItem
{
	__u32		Ip;					// clip command instruction pointer
	BOOL8		IsAct;			// active
	__u8		_rsvd0;
	__u8		_rsvd1;
	__u8		SctIdx;			// section index
	St_GrSwordRect	Rc;
}	*Ptr_GrP5PntClipItem;

typedef struct St_GrP5PntEnv
{
	St_GrP5PntCmdBase	Items[E_GrP5PntStackMaxCnt];
	__u32	InCnt;
	__u32	SctCnt;				// section count
	__u32	ClipTotal;
	St_GrRect	RcClip;		// now clip rect
	St_GrP5PntClipItem	ClipBuf[E_GrP5PntClipMaxCnt];
	St_GrP5PntSctItem		SctBuf[E_GrP5PntSctMaxCnt];
}	*Ptr_GrP5PntEnv;

typedef struct St_GrP5TxtBufItem
{
	__u16	ObjId;
	WCHAR	Str[E_GrP5TxtBufMaxLen + 1];
}	*Ptr_GrP5TxtBufItem;

#ifdef GR_P5_EDITOR

/*
typedef struct St_GrP5EditEnv
{
	
}	*Ptr_GrP5EditEnv;
*/
#endif

typedef struct St_GrP5PopWinCtl
{
	__u16	WinObjId;

}*Ptr_GrP5PopWinCtl;

typedef struct St_GrP5PopCtl
{
	__u8		FontId;
	__u8		WinCnt;
	__u16		_rsvd1;
	__u32		WinWidth;		// popup window width

	St_GrP5FontDrawStyle	TxtStyle;

	St_GrP5PopWinCtl	WinCtl[E_GrP5PopMaxDepth];

}	*Ptr_GrP5PopCtl;

typedef struct St_GrP5Env
{
	Cls_GrTaskSvr*	TskMng;		// main task
	Cls_GrTaskCli*	ObjMng;		// GrP5Mng

	Cls_GrTaskSvr*	TskRnd;		// render task
	Cls_GrTaskCli*	ObjRnd;		// render

	// text
	__u8	Language;						// language

	// mode
	__u8	ConExpType;					// console expantion type
	
	// input
	__u8	MseConIdx;					// mouse console index
	__u8	MseBtnMap;					// mouse button map
	__s32	MseX;
	__s32	MseY;
	__u16	MseObjId;						// mouse object id
	BOOL8	IsMseCap;						// mouse capture
	BOOL8	IsMseCusOn;					// mouse cursor on

	__u16	KeyObjId;		// focused object id
	__u32	KeyMap[E_GrP5KeyMapTblCnt];

	// register
	__s32*	PtrRintBuf;
	__u16		RintCnt;
	__u16		RtxtCnt;
	__u32		RtxtLen;
	void*		PtrRtxtBuf;

	// item
	__u32		ItemCnt;
	St_GrP5Item	ItemTbl[E_GrP5ItemMaxCnt];

	// popup
	St_GrP5PopCtl	PopCtl;		// popup contorl

	// setup
	__u8	SetupBuf[E_GrP5SetupBufSize];	// setup buffer
	__u32	StpMgn;	// setup margin
	// timer
	__u32		TimerCnt;
	St_GrP5TimerCtrl	TimerTbl[E_GrP5TimerMaxCnt];

	// control id
	void*		CtrlEvtTbl[E_GrP5CtrlIdMaxCnt];		// control event table

	// user id
	__u16		UserIdCvtTbl[E_GrP5UserIdMaxCnt];	// user id to object id

	// console
	St_GrP5ConCtrl	ConTbl[E_GrP5ConMaxCnt];

	// paint
	St_GrP5PntEnv	PntEnv;

	// text buffer
	St_GrP5TxtBufItem	TxtBuf[E_GrP5TxtBufMaxCnt];

}	*Ptr_GrP5Env;

//====================================================================
//global class


//====================================================================
//global function

Def_GrErrCode	GrP5Init(Cls_GrTaskSvr* A_TaskMng, Cls_GrTaskSvr* A_TaskRender);
Def_GrErrCode	GrP5Finish(void);

Def_GrErrCode	GrP5TaskRun(void);

void	GrP5RenderRun(BOOL8 A_IsRun);

BOOL8	GrP5ConsoleRegist(void* A_ObjCon, __u8 A_ConIdx);
BOOL8	GrP5ConsoleUnregist(__u8 A_ConIdx);

BOOL8	GrP5RelRectToAbsRect(Ptr_GrP5RelRect A_PtrRelRect, Ptr_GrRect A_PtrRefRect, Ptr_GrRect A_PtrRtRect);
BOOL8	GrP5AbsRectToRelRectFixRatio(Ptr_GrRect A_PtrAbsRect, Ptr_GrRect A_PtrRefRect, Ptr_GrP5RelRect A_PtrRtRelRect );
BOOL8	GrP5RelRectToZeroRatio(Ptr_GrP5RelRect A_PtrRelRect, Ptr_GrRect A_PtrRefRect, Ptr_GrP5RelRect A_PtrRtRect);

BOOL8	GrP5IsWinObjId(__u16 A_ObjId);

__u16	GrP5SlaveObjIdEncode(__u8 A_ConIdx, __u16 A_SlaveObjIdx,__u8 A_LocalId);
BOOL8	GrP5SlaveObjIdDecode(__u16 A_ObjId, __u8* A_PtrRtConIdx, __u16* A_PtrRtSlaveObjIdx, __u8* A_PtrRtLocalId);

__u16	GrP5WinObjIdEncode(__u8 A_ConIdx, __u8 A_Layer, __u8 A_WinIdx, __u8 A_LocalId);
BOOL8	GrP5WinObjIdDecode(__u16 A_ObjId, __u8* A_PtrRtConIdx, __u8* A_PtrRtLayer, __u8* A_PtrRtWinIdx, __u8* A_PtrRtLocalId);

__u8	GrP5ConIdxGetByObjId(__u16 A_ObjId);

__u8	GrP5ConIdxByObjId(__u16 A_ObjId);

void*	GrP5UiObjGetByUserId(__u32 A_UserId);
BOOL8	GrP5UserIdRegist(__u32 A_UserId, __u16 A_ObjId);

__u16	GrP5SlaveObjRegist(__u8 A_ConIdx,void* A_Obj);
__u16	GrP5WinObjRegist(__u8 A_ConIdx, __u8 A_Layer, void* A_Obj);
BOOL8	GrP5ObjUnregist(__u16 A_ObjId);

void*	GrP5UiObjGet(__u16 A_ObjId);
void*	GrP5UiObjFindByPos(__u8 A_ConIdx, __s32 A_PosX, __s32 A_PosY);
void*	GrP5UiObjCanMseFindByPos(__u8 A_ConIdx, __s32 A_PosX, __s32 A_PosY);
void*	GrP5UiObjFindKeyFocusAble(__u8 A_ConIdx);
void*	GrP5UiObjFindKeyFocusByDir(__u16 A_BaseObjId, __u8 A_Dir);

void*	GrP5UiObjCreateByClassId(__u16 A_ClassId, void* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect, __u16 A_ScptId = E_GrP5ScptIdNone);

void	GrP5KeyFocusSet(__u16 A_ObjId, __u32 A_Para);	// use system only
void	GrP5KeyFocusCorrectReq(void);		// use system only
void	GrP5KeyFocusCorrectDo(void);		// use system only
void*	GrP5KeyFocusUiObjGet(void);
void	GrP5KeyFocusReq(__u16 A_ObjId,__u32 A_Para);

BOOL8	GrP5IsMseCursorOn(void);

void	GrP5ConRedraw(__u8 A_ConIdx);
void	GrP5ConWinDelAll(__u8 A_ConIdx);

__u32	GrP5ObjIdFindByScptId(__u8 A_ConIdx, __u16 A_ScptId, __u16* A_PtrRtBuf, __u32 A_MaxCnt);
__u32	GrP5ObjIdFindByCtrlId(__u8 A_ConIdx, __u16 A_CtrlId, __u16* A_PtrRtBuf, __u32 A_MaxCnt);

void*	GrP5WindowCreateByClassId(__u16 A_ClassId, __u8 A_ConIdx, __u8 A_Layer, Ptr_GrP5RelRect A_PtrRelRect, __u16 A_ScptId = E_GrP5ScptIdNone);
void*	GrP5WindowCreateByScptId(__u16 A_ScptId, __u8 A_ConIdx, Ptr_GrP5RelRect A_PtrRelRect = NULL,__u8 A_Layer = E_GrP5LayerIndexByScript );
BOOL8	GrP5WindowCloseByObjId(__u16 A_ObjId);
void*	Grp5WindowPopupByScptId(__u16 A_ScptId, __u8 A_ConIdx, __s32 A_PosX, __s32 A_PosY , __u8 A_Layer = E_GrP5LayerIndexByScript);
void*	GrP5WindowFindByScptId(__u8 A_ConIdx, __u16 A_ScptId);

void*	GrP5ScriptGet(__u16 A_ScptId);
__u32	GrP5ScriptDecSub(void* A_PtrScpt, __u32 A_ScptSize, void* A_ObjPrnt, BOOL8 A_IsScptIdNone = FALSE);
void*	GrP5ScriptDecWin(void* A_PtrScpt, __u8 A_ConIdx, __u32* A_PtrRtDecoded, Ptr_GrP5RelRect A_PtrRelRect = NULL, 
	__u8 A_Layer = E_GrP5LayerIndexByScript, BOOL8 A_IsScptIdNone = FALSE);
void*	GrP5ScriptDecChild(void* A_PtrScpt, void* A_ObjPrnt, __u32* A_PtrRtDecoded, Ptr_GrP5RelRect A_PtrRelRect, BOOL8 A_IsFocusChg = TRUE, BOOL8 A_IsScptIdNone = FALSE);
__u32	GrP5ScriptPxFmtGet(void);
void*	GrP5ScriptParaGet(void* A_PtrScpt, __u16 A_ClassId, __u16* A_PtrRtSize);
BOOL8	GrP5ScriptRelRectGet(__u16 A_ScptId, Ptr_GrP5RelRect A_PtrRtRelRect);

Ptr_GrGdib	GrP5ImageGet(__u16 A_ImgId);
WCHAR*	GrP5TextGet(__u16 A_TxtId, __u8 A_LanguageId = E_GrP5TxtLanguageIdAuto);
void	GrP5LanguageSet(__u8 A_LanguageId);
__u8	GrP5LanguageGet(void);

BOOL8	GrP5MsgPost(void* A_PtrMsg,__u32 A_MsgSize);
BOOL8	GrP5MsgSend(void* A_PtrMsg, __u32 A_MsgSize);

void	GrP5InputKey(__u8 A_DevId, __u8 A_Key, BOOL8 A_IsPush);
void	GrP5InputMsePosRel(__s32 A_X, __s32 A_Y);
void	GrP5InputMsePosAbs(__s32 A_X, __s32 A_Y);
void	GrP5InputMseBtn(__u8 A_BtnIdx, BOOL8 A_IsPush);
void	GrP5InputMseWheel(__u8 A_WheelIdx, BOOL8 A_IsDown);

void	GrP5MseCusPosGet(__s32* A_PtrRtX, __s32* A_PtrRtY);

__u16	GrP5LogTypeToTextId(Ptr_GrDvrLogItm A_PtrLog);
void	GrP5LogContToWstr(Ptr_GrDvrLogItm A_PtrLog, WCHAR* A_StrRt);

void	GrP5RintBufSet(void* A_PtrBuf, __u32 A_Size);
__s32	GrP5RintGet(__u16 A_VarId);
void	GrP5RintSet(__u16 A_VarId, __s32 A_Value);

void	GrP5RtxtBufSet(void* A_PtrBuf, __u32 A_Size,__u32 A_Len);
WCHAR*	GrP5RtxtGet(__u16 A_VarId);
void	GrP5RtxtSet(__u16 A_VarId, WCHAR* A_Str);
__u32	GrP5RtxtMaxLen(void);

void*	GrP5FontGet(__u8 A_FontId);

BOOL8	GrP5ExcScriptSet(void* A_PtrScript);

void*	GrP5CtrlEvtSet(__u16 A_CtrlId,void* A_EvtObj );		// return = privous registed object pointer
void*	GrP5CtrlEvtGet(__u16 A_CtrlId);

BOOL8	GrP5TimerRegist(void* A_UiObj);
BOOL8	GrP5TimerUnregist(void* A_UiObj);

void	GrP5QuadModePageChange(__u8 A_ConIdx, __u8 A_Mode, __u8 A_Page);
void	GrP5QuadModeNext(__u8 A_ConIdx);
void	GrP5QuadModePrev(__u8 A_ConIdx);
void	GrP5QuadPageNext(__u8 A_ConIdx);
void	GrP5QuadPagePrev(__u8 A_ConIdx);
void	GrP5QuadModeExchg(__u8 A_ConIdx, __u8 A_Ch);
void	GrP5QuadModeGet(__u8 A_ConIdx, __u8* A_PtrRtMode, __u8* A_PtrRtPage);
void*	GrP5QuadUiObjGet(__u8 A_ConIdx);

WCHAR*	GrP5TxtBufAlloc(__u16 A_ObjId);
void	GrP5TxtBufFreeByObjId(__u16 A_ObjId);

void*	GrP5SetupPtrGet(void);
void*	GrP5SetupOfsPtrGet(__u16 A_Adr);
__u8	GrP5SetupGetU8(__u16 A_Adr);
__u16	GrP5SetupGetU16(__u16 A_Adr);
__u32	GrP5SetupGetU32(__u16 A_Adr);
void	GrP5SetupPutU8(__u16 A_Adr, __u8 A_Val);
void	GrP5SetupPutU16(__u16 A_Adr, __u16 A_Val);
void	GrP5SetupPutU32(__u16 A_Adr, __u32 A_Val);

void*	GrP5VkbdCreateByScptId(__u16 A_ScptId, __u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect,
	WCHAR* A_StrTxt, __u32 A_MaxLen, BOOL8 A_IsPassword, __u16 A_HostObjId, __u16 A_EvtCtlId, __u32 A_UserData);
BOOL8	GrP5VkbdCreate(__u32 A_Type, void* A_HostObj, __u16 A_CtlEvtId, WCHAR* A_StrTxt, __u32 A_MaxLen, 
	BOOL8 A_IsPassword,__u32 A_UserData);

void	GrP5AutoLogoutSet(__u32 A_ExpireSec);

void	GrP5PopupCloseAll(void);
void	GrP5PopupCloseByDepth(__u8 A_Depth);

void	GrP5ItemReset(void);
__s32	GrP5ItemAdd(__u16 A_TxtId, __u32	A_Stat = E_GrP5StatNormal, __u16	A_IconId = E_GrP5ImgIdNone,
	__u16	A_Squad = 1, __u16 A_ChildSquad = E_GrP5ItemSquadIdNone, __u32 A_UserData = 0, WCHAR* A_Str = NULL);

__s32	GrP5ItemIdGetByUserData(__u32 A_UserData);
BOOL8	GrP5ItemTextSet(__u32 A_ItemId, __u16 A_TxtId, WCHAR* A_Str = NULL);
BOOL8	GrP5ItemStatSet(__u32 A_ItemId, __u32 A_Stat);
BOOL8	GrP5ItemIconSet(__u32 A_ItemId, __u16 A_IconId);

__u32	GrP5ItemCntBySquad(__u16 A_Suqad);

__s32	GrP5PopWinIdxByUo(void* A_UiObj);

BOOL8	GrP5MenuPopByPos(__s32 A_PosX, __s32 A_PosY, __u8 A_ConIdx, __u8 A_Layer, __u8 A_FontId, Ptr_GrP5FontDrawStyle A_PtrTxtStyle,
	__u32 A_Width, __u16 A_Squad, __u16 A_CtrlId);
BOOL8	GrP5MenuSubPop(Ptr_GrRect A_PtrRect, __u8 A_ConIdx, __u8 A_Layer, __u16 A_Squad, __u16 A_CtrlId);

BOOL8	GrP5ListPopByRect(Ptr_GrRect A_PtrRect, __u8 A_ConIdx, __u8 A_Layer, __u8 A_FontId, Ptr_GrP5FontDrawStyle A_PtrTxtStyle, 
	__u32 A_ViewCnt,  __u32 A_ViewIdx, __u16 A_HostObjId );

//====================================================================
//out Environment conditional
#endif

