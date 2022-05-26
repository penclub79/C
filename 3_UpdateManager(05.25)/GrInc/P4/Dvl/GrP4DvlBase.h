/*
	GAUSE Platform developer Base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DvlBase
#define	_EPC_GrP4DvlBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P4/GrP4Script.h>
#include <P4/GrP4ApiBase.h>
#include <P4/GrP4UiCmd.h>

//====================================================================
//constance
#define E_GrP4DvlPrjFcc							Mac_GrMakeFcc('P','3','P','J')
#define E_GrP4DvlPrjVer							5				/* project version */

#define E_GrP4DvlConMaxCnt					2				/* console max count */

#define E_GrP4DvlModelMaxCnt				32			/* max model count */

#define E_GrP4DvlSkinMaxCnt					32			/* max skin count */

#define E_GrP4DvlColorPltCnt				256			/* color pallet count */

#define E_GrP4DvlSlaveMaxCnt				8192		/* slave control max count */
#define E_GrP4DvlSlaveMapShft				5
#define E_GrP4DvlSlaveMapCnt				(E_GrP4DvlSlaveMaxCnt >> E_GrP4DvlSlaveMapShft)		/* slave map count */

#define E_GrP4DvlFolderMaxCnt				256			/* project folder count */

#define E_GrP4DvlFolderIdRoot				0				/* folder id root */
#define E_GrP4DvlFolderIdBase				0x2000	/* folder id base - equal slave base */

#define E_GrP4DvlAdrNameLen					19
#define E_GrP4DvlAdrDescLen					64

#define E_GrP4DvlVarNameLen					19
#define E_GrP4DvlVarDescLen					42

#define E_GrP4DvlSetupUserBase			0x7000
#define E_GrP4DvlSetupUserSize			0x1000

#define E_GrP4DvlSetupSruBase				0x9000
#define E_GrP4DvlSetupSruSize				0x1000

#define E_GrP4DvlGvuNameLen					32

#define E_GrP4DvlUrgstNameLen				32

#define E_GrP4DvlSetupSysCnt				266

#define E_GrP4DvlAsmCodeMax					256			/* assembly code in function */
#define	E_GrP4DvlAsmCodeRmkLen			128

#define	E_GrP4DvlAsmFncCmtLen				128

// old version compatible
#define E_GrP4DvlOldScptRuleMaxCnt		32						/* rule count */

#define	E_GrP4DvlOldScptCfgPrjSize		0x1800
#define	E_GrP4DvlOldScptCfgRtlSize		0x50
#define E_GrP4DvlOldScptCfgMdlSize		0x200

#define E_GrP4DvlOldPrjConfigSize			192
#define E_GrP4DvlOldPrjRtlSize				2560

// IDE message

#define E_GrP4DvlIdeCmdVmFault				1		/* virtual machine fault */
#define E_GrP4DvlIdeCmdVmBrkPnt				2		/* virtual machine break point */

// project data type
#define	E_GrP4DvlPrjDataScptHdOld			1		/* script header V0 */
#define	E_GrP4DvlPrjDataFolderInfo		2		/* project data folder info table */
#define	E_GrP4DvlPrjDataWinDvlTbl			4		/* window develop info table */
#define	E_GrP4DvlPrjDataPstScptTblOld	6		/* PST info whole table */
#define	E_GrP4DvlPrjDataPstDvlTbl			7		/* PST develop info table */
#define	E_GrP4DvlPrjDataSetupVar			12	/* setup variable */
#define	E_GrP4DvlPrjDataSetupSys			13	/* setup system - *not use */
#define	E_GrP4DvlPrjDataGvu						14	/* global variable for user */
#define	E_GrP4DvlPrjDataSetBase				15	/* project enviroment data basic */
#define	E_GrP4DvlPrjDataAsmInfo				16	/* assembly develop info */
#define	E_GrP4DvlPrjDataMdlDebug			19	/* model debug info data */
#define	E_GrP4DvlPrjDataPageDvlTbl		21	/* page develop info table */
#define	E_GrP4DvlPrjDataSdaData				23	/* SDA data */
#define	E_GrP4DvlPrjDataSru						24	/* SRU - setup runtime user */
#define	E_GrP4DvlPrjDataBldFile				25	/* build file info */
#define	E_GrP4DvlPrjDataAsmCmt				26	/* assembly comment data - not use */
#define	E_GrP4DvlPrjDataConfig				128	/* project config */
#define	E_GrP4DvlPrjDataRuntime				129	/* project runtime */
#define	E_GrP4DvlPrjDataModel					130	/* project model */
#define	E_GrP4DvlPrjDataSkin					131	/* project skin */
#define	E_GrP4DvlPrjDataUrgst					132	/* registry user info */
#define	E_GrP4DvlPrjDataRgstRtl				133	/* registry runtime */
#define	E_GrP4DvlPrjDataAsmCodeRmk		134	/* assembly code remark */

// parameter data type
#define E_GrP4DvlParaTypeNone					0		/* not exist */

#define E_GrP4DvlParaTypeChar					1
#define E_GrP4DvlParaTypeShort				2
#define E_GrP4DvlParaTypeInt					3
#define E_GrP4DvlParaTypeByte					4
#define E_GrP4DvlParaTypeWord					5
#define E_GrP4DvlParaTypeDword				6

#define E_GrP4DvlParaTypeBool8				7

#define E_GrP4DvlParaTypeWchar				8			/* text */
#define E_GrP4DvlParaTypeWcOne				9			/* wchar one */

#define E_GrP4DvlParaTypeBitmap0			10		/* bit map (byte unit) type 0 */
#define E_GrP4DvlParaTypeBitmap1			11		/* bit map (byte unit) type 1 */
#define E_GrP4DvlParaTypeRedMark			12		/* red mark */
#define E_GrP4DvlParaTypeDir2d				13		/* direction 2D */

#define E_GrP4DvlParaTypeScptIdAny		20		/* script id any */
#define E_GrP4DvlParaTypeScptIdWin		21		/* script id window */

#define E_GrP4DvlParaTypePntCode			30		/* paint code */
#define E_GrP4DvlParaTypeAsmCode			31		/* assembly code */
#define E_GrP4DvlParaTypeConIdx				32		/* console index */
#define E_GrP4DvlParaTypePosBssLt			33		/* position basis left */
#define E_GrP4DvlParaTypePosBssUp			34		/* position basis top */
#define E_GrP4DvlParaTypePosBssRt			35		/* position basis right */
#define E_GrP4DvlParaTypePosBssDn			36		/* position basis bottom */
#define E_GrP4DvlParaTypeUpiCmd				37		/* UPI command */
#define E_GrP4DvlParaTypeColor				38		/* color */
#define E_GrP4DvlParaTypeKeyCode			39		/* key code */
#define E_GrP4DvlParaTypeApiCmd				40		/* API command */
#define E_GrP4DvlParaTypeTxtId				41		/* text table id */
#define E_GrP4DvlParaTypeQuadMode			42		/* quad mode id */
#define E_GrP4DvlParaTypePopupStyle		43		/* window popup style */

#define E_GrP4DvlParaTypeStdValType		100		/* standard value type - editor only */
#define E_GrP4DvlParaTypeVarAdr				101		/* variable address - editor only */
#define E_GrP4DvlParaTypeKeyEvtType		102		/* key event type - editor only */
#define E_GrP4DvlParaTypeUsvalType		103		/* unsigned value type - editor only */
#define E_GrP4DvlParaTypeRelUpi				104		/* relate UPI - editor only */
#define E_GrP4DvlParaTypeStpCstType		105		/* setup custom value type - editor only */
#define E_GrP4DvlParaTypeStpCstAdr		106		/* setup custom address - editor only */
#define E_GrP4DvlParaTypeSrgstAdr			107		/* system registry address */
#define E_GrP4DvlParaTypeUrgstAdr			108		/* user registry address */
#define E_GrP4DvlParaTypeStrDir				109		/* select directory - editor only */
#define E_GrP4DvlParaTypeStrFn				110		/* select file - editor only */

#define E_GrP4DvlParaTypeAsmOpType		140		/* assembly opcode type */
#define E_GrP4DvlParaTypeAsmFlagIdx		141		/* assembly flag index */
#define E_GrP4DvlParaTypeAsmArgType		142		/* assembly argument type */
#define E_GrP4DvlParaTypeAsmIregIdx		143		/* assembly integer register index */
#define E_GrP4DvlParaTypeAsmGvsAdr		144		/* assembly global variable system address */
#define E_GrP4DvlParaTypeAsmGvuAdr		145		/* assembly global variable user address */
#define E_GrP4DvlParaTypeAsmStpAdr		146		/* assembly setup byte */
#define E_GrP4DvlParaTypeAsmStpLst		147		/* assembly setup list */
#define E_GrP4DvlParaTypeAsmAddType		148		/* assembly adder value type */

#define E_GrP4DvlParaTypeAsmFlagUpdt	150		/* assembly flag update type */
#define E_GrP4DvlParaTypeAsmCmpType		151		/* assembly compare type */
#define E_GrP4DvlParaTypeAsmFlagSet		152		/* assembly flag setting type */
#define E_GrP4DvlParaTypeAsmCalc			153		/* assembly flag calculation type */
#define E_GrP4DvlParaTypeAsmMarkMov		154		/* assembly move mark '=' - not edit */
#define E_GrP4DvlParaTypeAsmNotSet		155		/* assembly not set type - logical , bit */
#define E_GrP4DvlParaTypeAsmJumpCall	156		/* assembly jump type - jump , call */
#define E_GrP4DvlParaTypeAsmJumpDir		157		/* assembly jump direction - down , up */
#define E_GrP4DvlParaTypeAsmTregIdx		158		/* assembly text register index */

// firmware file
//#define E_GrP4DvlBuildItemCnt					16


//====================================================================
//global macro

//====================================================================
//global type

// script parameter info

typedef	struct St_GrP4DvlDoPrInfoData
{
	WCHAR*	StrName;
	BOOL8		IsNewLn;		// new line
	__u8		Type;
	__u8		Cnt;				// item count
	void*		Ptr;
	void*		Ref;
	WCHAR*	StrDesc;
}	*Ptr_GrP4DvlDoPrInfoData;

// UPI info
typedef	struct St_GrP4DvlUpiInfo 
{
	__u16	Cmd;
	WCHAR*	StrName;
}	*Ptr_GrP4DvlUpiInfo;

// API info
typedef	struct St_GrP4DvlApiInfo 
{
	__u16	Cmd;
	WCHAR*	StrName;
}	*Ptr_GrP4DvlApiInfo;

// script base
typedef	struct	St_GrP4PrjHdInfo
{
	__u32	Fcc;		// project FCC code
	__u32	Ver;		// project version
	__u32	_rsvd[30];	// reserved
}	*Ptr_GrP4PrjHdInfo;

//---------------------------
// old script comportable

typedef	struct	St_GrP4DvlOldScptSctRule
{
	__u32	Cnt;
	void*	Pos;
}	*Ptr_GrP4DvlOldScptSctRule;

typedef	struct	St_GrP4DvlOldScptHdInfo
{
	__u32	Fcc;
	__u32	Ver;
	__u32	Atb;
	__u32	Time;		// make time
	__u32	_rsvd[4];
	// rule
	St_GrP4DvlOldScptSctRule	Rule[E_GrP4DvlOldScptRuleMaxCnt];
}	*Ptr_GrP4DvlOldScptHdInfo;

typedef	struct St_GrP4DvlOldScptCfgPrj		// project config
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
	St_GrP4ScptVkbdLang	VkbdTbl[16];		// virtual keyboard table
}	*Ptr_GrP4DvlOldScptCfgPrj;

typedef	struct St_GrP4DvlOldScptCfgRtl
{
	__u32	_rsvd0;
	BOOL8	IsChgConAble;	// can change console focus
	__u8	_rsvd1;				// 
	__u8	_rsvd2;				// 
	__u8	_rsvd3;				// 
	__u32	_rsvd4[14];		// reserved
	St_GrP4ScptCfgSkin	Skin;
}	*Ptr_GrP4DvlOldScptCfgRtl;

typedef	struct	St_GrP4DvlScptHeadOld
{
	St_GrP4DvlOldScptHdInfo	HdInfo;
	St_GrP4DvlOldScptCfgPrj	CfgPrj;		// need realign
	St_GrP4DvlOldScptCfgRtl	CfgRtl;		// need realign
	__u8	CfgMdl[E_GrP4DvlOldScptCfgMdlSize];		// model configuration - decrease copy
}	*Ptr_GrP4DvlScptHeadOld;

typedef	struct St_GrP4PrjOldRtl			// runtime data 
{
	__u8	ModelIdx;				// run model index
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	_rsvd3[127];		// reserved area
	__u32	_rsvd4[256];		// reserved area
	__u32	ClrPltTbl[E_GrP4DvlColorPltCnt];						// color pallet count
}	*Ptr_GrP4PrjOldRtl;

typedef	struct	St_GrP4PrjOldModelCfg
{
	Def_WstrName		StrName;		// name
	__u32	_rsvd4;		// reserved
	__u8	SkinIdx;	// skin index
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	_rsvd3[30];	// model configuration
}	*Ptr_GrP4PrjOldModelCfg;

typedef	struct	St_GrP4PrjOldSkinCfg
{
	Def_WstrName	StrName;
	__u32	ClrCus;			// cursor color
	__u32	_rsvd[31];	// reserved area
}	*Ptr_GrP4PrjOldSkinCfg;


typedef	struct	St_GrP4PrjOldHead
{
	__u8		Config[E_GrP4DvlOldPrjConfigSize];					// decrease copy
	St_GrP4PrjOldRtl	Rtl;															// runtime data - realign
	St_GrP4PrjOldModelCfg	Model[E_GrP4DvlModelMaxCnt];	// realign
	St_GrP4PrjOldSkinCfg	Skin[E_GrP4DvlSkinMaxCnt];		//	decrease copy
	__u8		MdlAtb[E_GrP4DvlModelMaxCnt][E_GrP4DvlOldScptCfgMdlSize];		// decrease copy
}	*Ptr_GrP4PrjOldHead;

//---------------------------
// project file

typedef	struct	St_GrP4PrjConCfg
{
	__u32	ResX;		// resolution x
	__u32	ResY;		// resolution y
	__u32	_rsvd[6];	// reserved
}	*Ptr_GrP4PrjConCfg;

typedef	struct	St_GrP4PrjCfg
{
	BOOL8	IsDualMon;
	BOOL8	IsInputDual;				// keyboard mouse can input second monitor - not use
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u32	_rsvd2[31];					// reserved
	St_GrP4PrjConCfg	ConCfg[E_GrP4DvlConMaxCnt];
}	*Ptr_GrP4PrjCfg;

typedef	struct St_GrP4PrjRtl			// runtime data 
{
	__u8	ModelIdx;				// run model index
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	ClrPltTbl[E_GrP4DvlColorPltCnt];						// color pallet count
}	*Ptr_GrP4PrjRtl;

typedef	struct	St_GrP4PrjModelCfg
{
	Def_WstrName		StrName;		// name
	__u8	SkinIdx;	// skin index
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrP4PrjModelCfg;

typedef	struct	St_GrP4PrjSkinCfg
{
	Def_WstrName	StrName;
	__u32	ClrCus;			// cursor color
}	*Ptr_GrP4PrjSkinCfg;

typedef	struct	St_GrP4PrjFoldInfo
{
	BOOL8	IsExist;	// exist
	BOOL8	IsExpand;	// expand
	__u16	PrntId;		// parent folder id - 0 = root
	Def_WstrName	StrName;	// folder name
}	*Ptr_GrP4PrjFoldInfo;

typedef struct	St_GrP4PrjBldFile
{
	Def_WstrGrFnPath	StrScptOut;			// script out file name
	Def_WstrGrFnPath	StrBaseFile;		// base script file
}	*Ptr_GrP4PrjBldFile;

typedef	struct	St_GrP4PrjWinInfo
{
	__u32	Size;			// script size
	__u16	FoldId;		// folder id
	__u16	_rsvd;
	Def_WstrName	StrName;	// window name
}	*Ptr_GrP4PrjWinInfo;

typedef	struct St_GrP4PrjPstInfo 
{
	Def_WstrName	StrName;
}	*Ptr_GrP4PrjPstInfo;

typedef	struct St_GrP4PrjAdrInfo
{
	__u16	Pos;
	WCHAR	StrName[E_GrP4DvlAdrNameLen];
	WCHAR	StrDesc[E_GrP4DvlAdrDescLen];
}	*Ptr_GrP4PrjAdrInfo;

typedef	struct St_GrP4PrjStpInfo
{
	__u16	Pos;
	__u16	Type;
	__u16	Len;
	__u16	Size;
	WCHAR	StrName[E_GrP4DvlAdrNameLen];
	WCHAR	StrDesc[E_GrP4DvlAdrDescLen];
}	*Ptr_GrP4PrjStpInfo;

typedef	struct St_GrP4PrjVarInfo 
{
	__u8	Type;
	__u8	_rsvd0;
	__u16	Pos;
	__u16	Cnt;
	WCHAR	StrName[E_GrP4DvlVarNameLen];
	WCHAR	StrDesc[E_GrP4DvlVarDescLen];
}	*Ptr_GrP4PrjVarInfo;

typedef	struct	St_GrP4PrjGvuInfo		/* global variable of user */
{
	WCHAR	StrName[E_GrP4DvlGvuNameLen];
}	*Ptr_GrP4PrjGvuInfo;

typedef	struct	St_GrP4PrjUrgstInfo		/* user registry */
{
	WCHAR	StrName[E_GrP4DvlGvuNameLen];
}	*Ptr_GrP4PrjUrgstInfo;

typedef	struct St_GrP4PrjAsmInfo
{
	Def_WstrName	StrName;
}	*Ptr_GrP4PrjAsmInfo;

/*
typedef struct St_GrP4PrjAsmCmt
{
	WCHAR*	StrCmt;
}	*Ptr_GrP4PrjAsmCmt;
*/

typedef struct St_GrP4PrjAsmFncCmt
{
	WCHAR	StrCmt[E_GrP4DvlAsmFncCmtLen];
}	*Ptr_GrP4PrjAsmFncCmt;

typedef struct St_GrP4PrjAsmCodeRmk
{
	WCHAR	StrRmk[E_GrP4DvlAsmCodeMax][E_GrP4DvlAsmCodeRmkLen];
}	*Ptr_GrP4PrjAsmCodeRmk;

typedef	struct	St_GrP4PrjDbgInfo
{
	__u8		VdoMaxCh;		// video max channel
	__u8		AdoMaxCh;		// audio max channel
	__u8		DiChCnt;		// sensor channel count
	__u8		DoChCnt;		// alarm channel count

	__u32		HwVer;			// hardware version
	__u32		NetVer;			// network version

	__u8		DipSw;			// dip switch
	__u8		_rsv0;			// reserved
	__u16		UpiBpObj;		// UPI break point object ID

	BOOL8		ApiBpTbl[E_GrP4ApiTblCnt];		// API break point
	BOOL8		UpiBpTbl[E_GrP4UpiMaxCnt];		// UPI break point
}	*Ptr_GrP4PrjDbgInfo;

typedef	struct	St_GrP4PrjPageInfo
{
	__u32	Size;			// script size
	__u16	FoldId;		// folder id
	__u16	_rsvd;
	Def_WstrName	StrName;	// page name
}	*Ptr_GrP4PrjPageInfo;


//====================================================================
//global function
void	GrP4DvlIdeMsgPost( __u32 A_Msg, __u32 A_Para );		// IDE message post
__u32	GrP4DvlParaTypeUnitSizeGet( __u8 A_Type );

//====================================================================
//out Environment conditional
#endif

