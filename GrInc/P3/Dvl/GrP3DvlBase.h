/*
	GAUSE Platform developer Base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DvlBase
#define	_EPC_GrP3DvlBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Script.h>

//====================================================================
//constance
#define E_GrP3DvlPrjFcc							Mac_GrMakeFcc('P','3','P','J')
#define E_GrP3DvlPrjVer							5				/* project version */

#define E_GrP3DvlConMaxCnt					2				/* console max count */

#define E_GrP3DvlModelMaxCnt				32			/* max model count */

#define E_GrP3DvlSkinMaxCnt					32			/* max skin count */

#define E_GrP3DvlColorPltCnt				256			/* color pallet count */

#define E_GrP3DvlSlaveMaxCnt				8192		/* slave control max count */
#define E_GrP3DvlSlaveMapShft				5
#define E_GrP3DvlSlaveMapCnt				(E_GrP3DvlSlaveMaxCnt >> E_GrP3DvlSlaveMapShft)		/* slave map count */

#define E_GrP3DvlFolderMaxCnt				256			/* project folder count */

#define E_GrP3DvlFolderIdRoot				0				/* folder id root */
#define E_GrP3DvlFolderIdBase				0x2000	/* folder id base - equal slave base */

#define E_GrP3DvlAdrNameLen					19
#define E_GrP3DvlAdrDescLen					64

#define E_GrP3DvlVarNameLen					19
#define E_GrP3DvlVarDescLen					42

#define E_GrP3DvlSetupUserBase			0x7000
#define E_GrP3DvlSetupUserSize			0x1000

#define E_GrP3DvlSetupSruBase				0x9000
#define E_GrP3DvlSetupSruSize				0x1000

#define E_GrP3DvlGvuNameLen					32

#define E_GrP3DvlSetupSysCnt				234

// old version compatible
#define E_GrP3DvlOldScptRuleMaxCnt		32						/* rule count */

#define	E_GrP3DvlOldScptCfgPrjSize		0x1800
#define	E_GrP3DvlOldScptCfgRtlSize		0x50
#define E_GrP3DvlOldScptCfgMdlSize		0x200

#define E_GrP3DvlOldPrjConfigSize			192
#define E_GrP3DvlOldPrjRtlSize				2560

// IDE message

#define E_GrP3DvlIdeCmdVmFault				1		/* virtual machine fault */
#define E_GrP3DvlIdeCmdVmBrkPnt				2		/* virtual machine break point */

// project data type
#define	E_GrP3DvlPrjDataScptHdOld			1		/* script header V0 */
#define	E_GrP3DvlPrjDataFolderInfo		2		/* project data folder info table */
#define	E_GrP3DvlPrjDataWinDvlTbl			4		/* window develop info table */
#define	E_GrP3DvlPrjDataPstScptTblOld	6		/* PST info whole table */
#define	E_GrP3DvlPrjDataPstDvlTbl			7		/* PST develop info table */
#define	E_GrP3DvlPrjDataSetupVar			12	/* setup variable */
#define	E_GrP3DvlPrjDataSetupSys			13	/* setup system */
#define	E_GrP3DvlPrjDataGvu						14	/* global variable for user */
#define	E_GrP3DvlPrjDataSetBase				15	/* project set data basic */
#define	E_GrP3DvlPrjDataAsmInfo				16	/* assembly develop info */
#define	E_GrP3DvlPrjDataMdlDebug			19	/* model debug info data */
#define	E_GrP3DvlPrjDataPageDvlTbl		21	/* page develop info table */
#define	E_GrP3DvlPrjDataSdaData				23	/* SDA data */
#define	E_GrP3DvlPrjDataSru						24	/* SRU - setup runtime user */
#define	E_GrP3DvlPrjDataBldFile				25	/* build file info */
#define	E_GrP3DvlPrjDataAsmCmt				26	/* assembly comment data */
#define	E_GrP3DvlPrjConfig						128	/* project config */
#define	E_GrP3DvlPrjRuntime						129	/* project runtime */
#define	E_GrP3DvlPrjModel							130	/* project model */
#define	E_GrP3DvlPrjSkin							131	/* project skin */

// parameter data type
#define E_GrP3DvlParaTypeNone					0		/* not exist */

#define E_GrP3DvlParaTypeChar					1
#define E_GrP3DvlParaTypeShort				2
#define E_GrP3DvlParaTypeInt					3
#define E_GrP3DvlParaTypeByte					4
#define E_GrP3DvlParaTypeWord					5
#define E_GrP3DvlParaTypeDword				6

#define E_GrP3DvlParaTypeBool8				7

#define E_GrP3DvlParaTypeWhcar				8			/* text */

#define E_GrP3DvlParaTypeBitmap0			10		/* bit map (byte unit) type 0 */
#define E_GrP3DvlParaTypeBitmap1			11		/* bit map (byte unit) type 1 */

#define E_GrP3DvlParaTypeScptIdAny		20		/* script id any */
#define E_GrP3DvlParaTypeScptIdWin		21		/* script id window */

#define E_GrP3DvlParaTypePntCode			30		/* paint code */
#define E_GrP3DvlParaTypeAsmCode			31		/* assembly code */
#define E_GrP3DvlParaTypeConIdx				32		/* console index */
#define E_GrP3DvlParaTypePosBssLt			33		/* position basis left */
#define E_GrP3DvlParaTypePosBssUp			34		/* position basis top */
#define E_GrP3DvlParaTypePosBssRt			35		/* position basis right */
#define E_GrP3DvlParaTypePosBssDn			36		/* position basis bottom */
#define E_GrP3DvlParaTypeUpiCmd				37		/* UPI command */
#define E_GrP3DvlParaTypeColor				38		/* color */
#define E_GrP3DvlParaTypeKeyCode			39		/* key code */
#define E_GrP3DvlParaTypeApiCmd				40		/* API command */
#define E_GrP3DvlParaTypeTxtId				41		/* text table id */
#define E_GrP3DvlParaTypeQuadMode			42		/* quad mode id */

#define E_GrP3DvlParaTypeStdValType		100		/* standard value type - editor only */
#define E_GrP3DvlParaTypeVarAdr				101		/* variable address - editor only */
#define E_GrP3DvlParaTypeKeyEvtType		102		/* key event type - editor only */
#define E_GrP3DvlParaTypeUsvalType		103		/* unsigned value type - editor only */
#define E_GrP3DvlParaTypeRelUpi				104		/* relate UPI - editor only */

#define E_GrP3DvlParaTypeAsmOpType		140		/* assembly opcode type */
#define E_GrP3DvlParaTypeAsmFlagIdx		141		/* assembly flag index */
#define E_GrP3DvlParaTypeAsmArgType		142		/* assembly argument type */
#define E_GrP3DvlParaTypeAsmIregIdx		143		/* assembly integer register index */
#define E_GrP3DvlParaTypeAsmGvsAdr		144		/* assembly global variable system address */
#define E_GrP3DvlParaTypeAsmGvuAdr		145		/* assembly global variable user address */
#define E_GrP3DvlParaTypeAsmStpAdr		146		/* assembly setup byte */
#define E_GrP3DvlParaTypeAsmStpLst		147		/* assembly setup list */
#define E_GrP3DvlParaTypeAsmMemType		148		/* assembly memory value type */

#define E_GrP3DvlParaTypeAsmFlagUpdt	150		/* assembly flag update type */
#define E_GrP3DvlParaTypeAsmCmpType		151		/* assembly compare type */
#define E_GrP3DvlParaTypeAsmFlagSet		152		/* assembly flag setting type */
#define E_GrP3DvlParaTypeAsmCalc			153		/* assembly flag calculation type */
#define E_GrP3DvlParaTypeAsmMarkMov		154		/* assembly move mark '=' - not edit */
#define E_GrP3DvlParaTypeAsmNotSet		155		/* assembly not set type - logical , bit */
#define E_GrP3DvlParaTypeAsmJumpCall	156		/* assembly jump type - jump , call */
#define E_GrP3DvlParaTypeAsmJumpDir		157		/* assembly jump direction - down , up */
#define E_GrP3DvlParaTypeAsmTregIdx		158		/* assembly text register index */

// firmware file
#define E_GrP3DvlBuildItemCnt					16


//====================================================================
//global macro

//====================================================================
//global type

// script parameter info

typedef	struct St_GrP3DvlDoPrInfoData
{
	WCHAR*	StrName;
	BOOL8		IsNewLn;		// new line
	__u8		Type;
	__u8		Cnt;				// item count
	void*		Ptr;
	void*		Ref;
	WCHAR*	StrDesc;
}	*Ptr_GrP3DvlDoPrInfoData;

// UPI info
typedef	struct St_GrP3DvlUpiInfo 
{
	__u16	Cmd;
	WCHAR*	StrName;
}	*Ptr_GrP3DvlUpiInfo;

// API info
typedef	struct St_GrP3DvlApiInfo 
{
	__u16	Cmd;
	WCHAR*	StrName;
}	*Ptr_GrP3DvlApiInfo;

// script base
typedef	struct	St_GrP3PrjHdInfo
{
	__u32	Fcc;		// project FCC code
	__u32	Ver;		// project version
	__u32	_rsvd[30];	// reserved
}	*Ptr_GrP3PrjHdInfo;

//---------------------------
// old script comportable

typedef	struct	St_GrP3DvlOldScptSctRule
{
	__u32	Cnt;
	void*	Pos;
}	*Ptr_GrP3DvlOldScptSctRule;

typedef	struct	St_GrP3DvlOldScptHdInfo
{
	__u32	Fcc;
	__u32	Ver;
	__u32	Atb;
	__u32	Time;		// make time
	__u32	_rsvd[4];
	// rule
	St_GrP3DvlOldScptSctRule	Rule[E_GrP3DvlOldScptRuleMaxCnt];
}	*Ptr_GrP3DvlOldScptHdInfo;

typedef	struct St_GrP3DvlOldScptCfgPrj		// project config
{
	__u32	FmtFcc;				// format FCC
	__u16	CusImgId;			// software cursor image id
	__u16	_rsvd4;				// reserved
	__u32	HwType;				// hardware type
	__u8	UartCnt;			// UART count
	__u8	UiResId;			// UI resolution ID
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	FwVer;				// firmware version
	__u32	_rsvd3[11];		// reserved
	St_GrP3ScptVkbdLang	VkbdTbl[E_GrP3ScptLangMaxCnt];		// virtual keyboard table
}	*Ptr_GrP3DvlOldScptCfgPrj;

typedef	struct St_GrP3DvlOldScptCfgRtl
{
	__u32	_rsvd0;
	BOOL8	IsChgConAble;	// can change console focus
	__u8	_rsvd1;				// 
	__u8	_rsvd2;				// 
	__u8	_rsvd3;				// 
	__u32	_rsvd4[14];		// reserved
	St_GrP3ScptCfgSkin	Skin;
}	*Ptr_GrP3DvlOldScptCfgRtl;

typedef	struct	St_GrP3DvlScptHeadOld
{
	St_GrP3DvlOldScptHdInfo	HdInfo;
	St_GrP3DvlOldScptCfgPrj	CfgPrj;		// need realign
	St_GrP3DvlOldScptCfgRtl	CfgRtl;		// need realign
	__u8	CfgMdl[E_GrP3DvlOldScptCfgMdlSize];		// model configuration - decrease copy
}	*Ptr_GrP3DvlScptHeadOld;

typedef	struct St_GrP3PrjOldRtl			// runtime data 
{
	__u8	ModelIdx;				// run model index
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	_rsvd3[127];		// reserved area
	__u32	_rsvd4[256];		// reserved area
	__u32	ClrPltTbl[E_GrP3DvlColorPltCnt];						// color pallet count
}	*Ptr_GrP3PrjOldRtl;

typedef	struct	St_GrP3PrjOldModelCfg
{
	Def_WstrName		StrName;		// name
	__u32	_rsvd4;		// reserved
	__u8	SkinIdx;	// skin index
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	_rsvd3[30];	// model configuration
}	*Ptr_GrP3PrjOldModelCfg;

typedef	struct	St_GrP3PrjOldSkinCfg
{
	Def_WstrName	StrName;
	__u32	ClrCus;			// cursor color
	__u32	_rsvd[31];	// reserved area
}	*Ptr_GrP3PrjOldSkinCfg;


typedef	struct	St_GrP3PrjOldHead
{
	__u8		Config[E_GrP3DvlOldPrjConfigSize];					// decrease copy
	St_GrP3PrjOldRtl	Rtl;															// runtime data - realign
	St_GrP3PrjOldModelCfg	Model[E_GrP3DvlModelMaxCnt];	// realign
	St_GrP3PrjOldSkinCfg	Skin[E_GrP3DvlSkinMaxCnt];		//	decrease copy
	__u8		MdlAtb[E_GrP3DvlModelMaxCnt][E_GrP3DvlOldScptCfgMdlSize];		// decrease copy
}	*Ptr_GrP3PrjOldHead;

//---------------------------
// project file

typedef	struct	St_GrP3PrjConCfg
{
	__u32	ResX;		// resolution x
	__u32	ResY;		// resolution y
	__u32	_rsvd[6];	// reserved
}	*Ptr_GrP3PrjConCfg;

typedef	struct	St_GrP3PrjCfg
{
	BOOL8	IsDualMon;
	BOOL8	IsInputDual;				// keyboard mouse can input second monitor - not use
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u32	_rsvd2[31];					// reserved
	St_GrP3PrjConCfg	ConCfg[E_GrP3DvlConMaxCnt];
}	*Ptr_GrP3PrjCfg;

typedef	struct St_GrP3PrjRtl			// runtime data 
{
	__u8	ModelIdx;				// run model index
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	ClrPltTbl[E_GrP3DvlColorPltCnt];						// color pallet count
}	*Ptr_GrP3PrjRtl;

typedef	struct	St_GrP3PrjModelCfg
{
	Def_WstrName		StrName;		// name
	__u8	SkinIdx;	// skin index
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrP3PrjModelCfg;

typedef	struct	St_GrP3PrjSkinCfg
{
	Def_WstrName	StrName;
	__u32	ClrCus;			// cursor color
}	*Ptr_GrP3PrjSkinCfg;

typedef	struct	St_GrP3PrjFoldInfo
{
	BOOL8	IsExist;	// exist
	BOOL8	IsExpand;	// expand
	__u16	PrntId;		// parent folder id - 0 = root
	Def_WstrName	StrName;	// folder name
}	*Ptr_GrP3PrjFoldInfo;

typedef	struct	St_GrP3PrjSetBase
{
	Def_WstrGrFnPath	StrPathImg;			// image
	Def_WstrGrFnPath	StrPathFont;		// font 
	Def_WstrGrFnPath	StrPathBldTg;		// build target
	Def_WstrGrFnPath	StrPathHexp;		// header file export
	Def_WstrGrFnPath	StrPathText;		// EXEL file export
	Def_WstrGrFnPath	StrPathScpt;		// script export
	Def_WstrGrFnPath	StrPathBldSr;		// build source
}	*Ptr_GrP3PrjSetBase;

typedef struct St_GrP3PrjBldItem
{
	__u8	Type;
	__u8	Atb;
	__u16	_rsvd0;
	__u32	_rsvd1;
	Def_WstrGrFnPath	StrFn;
}	*Ptr_GrP3PrjBldItem;

typedef struct	St_GrP3PrjBldFile
{
	Def_WstrGrFnPath	StrScptOut;			// script out file name
	Def_WstrGrFnPath	StrRunCmd;			// script build command
	__u8	_rsvd0[2048];
	St_GrP3PrjBldItem	ItemTbl[E_GrP3DvlBuildItemCnt];
}	*Ptr_GrP3PrjBldFile;

typedef	struct	St_GrP3PrjWinInfo
{
	__u32	Size;			// script size
	__u16	FoldId;		// folder id
	__u16	_rsvd;
	Def_WstrName	StrName;	// window name
}	*Ptr_GrP3PrjWinInfo;

typedef	struct St_GrP3PrjPstInfo 
{
	Def_WstrName	StrName;
}	*Ptr_GrP3PrjPstInfo;

typedef	struct St_GrP3PrjAdrInfo
{
	__u16	Pos;
	WCHAR	StrName[E_GrP3DvlAdrNameLen];
	WCHAR	StrDesc[E_GrP3DvlAdrDescLen];
}	*Ptr_GrP3PrjAdrInfo;

typedef	struct St_GrP3PrjStpInfo
{
	__u16	Pos;
	__u16	Type;
	__u16	Len;
	__u16	Size;
	WCHAR	StrName[E_GrP3DvlAdrNameLen];
	WCHAR	StrDesc[E_GrP3DvlAdrDescLen];
}	*Ptr_GrP3PrjStpInfo;

typedef	struct St_GrP3PrjVarInfo 
{
	__u8	Type;
	__u8	_rsvd0;
	__u16	Pos;
	__u16	Cnt;
	WCHAR	StrName[E_GrP3DvlVarNameLen];
	WCHAR	StrDesc[E_GrP3DvlVarDescLen];
}	*Ptr_GrP3PrjVarInfo;

typedef	struct	St_GrP3PrjGvuInfo		/* global variable of user */
{
	WCHAR	StrName[E_GrP3DvlGvuNameLen];
}	*Ptr_GrP3PrjGvuInfo;

typedef	struct St_GrP3PrjAsmInfo
{
	Def_WstrName	StrName;
}	*Ptr_GrP3PrjAsmInfo;

typedef struct St_GrP3PrjAsmCmt
{
	WCHAR*	StrCmt;
}	*Ptr_GrP3PrjAsmCmt;

typedef	struct	St_GrP3PrjDbgInfo
{
	__u8		VdoMaxCh;		// video max channel
	__u8		AdoMaxCh;		// audio max channel
	__u8		DiChCnt;		// sensor channel count
	__u8		DoChCnt;		// alarm channel count
	__u32		HwVer;			// hardware version
	__u32		NetVer;			// network version
	__u8		DipSw;			// dip switch
	__u8		_rsv0;			// reserved
	__u8		_rsv1;			// reserved
	__u8		_rsv2;			// reserved
}	*Ptr_GrP3PrjDbgInfo;

typedef	struct	St_GrP3PrjPageInfo
{
	__u32	Size;			// script size
	__u16	FoldId;		// folder id
	__u16	_rsvd;
	Def_WstrName	StrName;	// page name
}	*Ptr_GrP3PrjPageInfo;


//====================================================================
//global function
void	GrP3DvlIdeMsgPost( __u32 A_Msg, __u32 A_Para );		// IDE message post
__u32	GrP3DvlParaTypeUnitSizeGet( __u8 A_Type );

//====================================================================
//out Environment conditional
#endif

