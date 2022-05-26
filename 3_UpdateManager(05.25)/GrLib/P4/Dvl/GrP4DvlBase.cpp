/*
	GAUSE Platform develop base

*/

#include <P4/Dvl/GrP4DvlBase.h>
#include <P4/GrP4Base.h>
#include <P4/GrP4UiCmd.h>

#include <P4/GrP4Cfg.h>

//--------------------------------------------------------------------
// local constant


//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var

WCHAR*	V_GrP4DvlClassNameTbl[E_GrP4DoClassCnt]	=	
{
	L"None",
	L"Base",
	L"Window",
	L"Frame",
	L"Label",
	L"Prograsss Bar Local",
	L"Quad",
	L"Btn Page",
	L"Static",
	L"EditSetupStr",
	L"Btn Label",
	L"Spin Number Base",
	L"Spin Number Setup",
	L"Spin Text Base",
	L"Spin Text Setup",
	L"Relate Static",
	L"Spin Page Text ",
	L"Spin Dec Digit Base ",
	L"Spin Dec Digit Setup ",
	L"Btn Static",
	L"Check Base",
	L"Chk Static Setup Bool8",
	L"Chk Static Setup Bit",
	L"Sbar Variable",
	L"Sbar Setup",
	L"Radio Base",
	L"Radio Setup",
	L"Chk Static GVU Bool8",
	L"Chk Static GVU Bit",
	L"Spin Paint",
	L"Spin Number Local",
	L"Spin Dec Digit Local",
	L"Spin Text Local",
	L"Time Control",
	L"Calendar Base",
	L"Calendar Record",
	L"Sbar Base",
	L"Progress bar Base",
	L"Progress bar Memory",
	L"Grid Base",
	L"Grid Record Setup",
	L"Btn Relate Static",
	L"Grid Record View (GridRstat)",
	L"Time label",
	L"ListBox Base",
	L"ListBox Catalog",
	L"Spin Quad Setup",
	L"Radio GVU value",
	L"Radio Setup Bit",
	L"Chk Static Local",
	L"Edit Base",
	L"Edit Local",
	L"Spin Id",
	L"Grid Motion",
	L"Calendar Holi",
	L"Panel PTZ",
	L"Spin BufTxt Base",
	L"Spin BufTxt Setup",
	L"Grid Record Select Range",
	L"Spin value base",
	L"Spin value setup",
	L"Grid Play time bar",
	L"Tree base",
	L"Tree Local text",
	L"Play Bar",
};

WCHAR*	V_GrP4DvlRelUpiNameTbl[E_GrP4RelUpiCnt]	=	
{
	L"None",
	L"Value",
	L"RecType",
	L"Channel",
};

St_GrP4DvlUpiInfo	V_GrP4DvlUpiTbl[E_GrP4UpiCnt]	=	
{
	{	E_GrP4UpiClassIdGet,			L"ClassIdGet :: out: (0)ClassId" },
	{	E_GrP4UpiScptIdGet,				L"ScriptIdGet :: out: (0)ScriptId" },
	{	E_GrP4UpiPosGet,					L"PosGet :: out: (1)x,(2)y" },
	{ E_GrP4UpiPosMovAbs,				L"PosMoveAbs :: in: (1)x,(2)y" },
	{ E_GrP4UpiPosMovRel,				L"PosMoveRel :: in: (1)x,(2)y" },
	{	E_GrP4UpiSizeGet,					L"SizeGet :: out: (1)size x,(2)size y" },
	{ E_GrP4UpiSizeSetAbs,			L"SizeSetAbs :: in: (1)size x,(2)size y" },
	{ E_GrP4UpiSizeSetRel,			L"SizeSetRel :: in: (1)size x,(2)size y" },
	{	E_GrP4UpiVisibleGet,			L"VisibleGet :: out: (0)0 = behind, 1= show" },
	{	E_GrP4UpiVisibleSet,			L"VisibleSet :: in: (1)0 = behind, 1= show" },
	{	E_GrP4UpiEnableGet,				L"EnableGet :: out: (0)0 = false, 1= true" },
	{	E_GrP4UpiEnableSet,				L"EnableSet :: in: (1)0 = false, 1= true" },
	{	E_GrP4UpiFcsMvTblGet,			L"FcsMvTblGet :: in: (1)left, (2)right, (3)up, (4)down " },
	{	E_GrP4UpiFcsMvTblSet,			L"FcsMvTblSet :: out: (1)left, (2)right, (3)up, (4)down" },
	{	E_GrP4UpiUserDataGet,			L"UserDataGet :: in: (1)index :: out: (0)value " },
	{	E_GrP4UpiUserDataSet,			L"UserDataSet :: in: (1)index, (2)value " },
	{	E_GrP4UpiReqDraw,					L"ReqDraw :: in: (1) 0=normal,1=forceless draw " },
	{	E_GrP4UpiWinClose,				L"WinClose ::  " },
	{	E_GrP4UpiValueGet,				L"ValueGet ::  out: (0)value" },
	{	E_GrP4UpiValueSet,				L"ValueSet ::  in: (1)value" },
	{	E_GrP4UpiTextGet,					L"TextGet :: in: (1)text register index" },
	{	E_GrP4UpiTextSet,					L"TextSet :: in: (1)text register index" },
	{	E_GrP4UpiMinValueGet,			L"MinValueGet :: out: (0)value" },
	{	E_GrP4UpiMinValueSet,			L"MinValueSet :: in: (1)value" },
	{	E_GrP4UpiMaxValueGet,			L"MaxValueGet :: out: (0)value" },
	{	E_GrP4UpiMaxValueSet,			L"MaxValueSet :: in: (1)value" },
	{	E_GrP4UpiEvtHostIdGet,		L"EvtHostIdGet :: out: (0)host script id" },
	{	E_GrP4UpiEvtHostIdSet,		L"EvtHostIdSet :: in: (1)host script id" },
	{	E_GrP4UpiTextIdGet,				L"TextIdGet :: out: (0)text id" },
	{	E_GrP4UpiTextIdSet,				L"TextIdSet :: in: (1)text id" },
	{	E_GrP4UpiRecTypeGet,			L"RecTypeGet :: out: (0)record type" },
	{	E_GrP4UpiRecTypeSet,			L"RecTypeSet :: in: (1)record type" },
	{	E_GrP4UpiFrmPageSet,			L"FrmPageSet :: in: (1)PageId, (2)Layer, (3)0=none,1=focus move" },
	{	E_GrP4UpiFrmPageBack,			L"FrmPageBack :: in: (1)0=none,1=focus move" },
	{	E_GrP4UpiChGet,						L"ChGet :: out: (0)channel number" },
	{	E_GrP4UpiChSet,						L"ChSet :: in: (1)channel number" },
	{	E_GrP4UpiSubShowGet,			L"SubShowGet :: in: (1)control number :: out: (0)0=off,1=show" },
	{	E_GrP4UpiSubShowSet,			L"SubShowSet :: in: (1)control number (2)0=off,1=show" },
	{	E_GrP4UpiFocusIdSet,			L"FocusIdSet :: " },
	{	E_GrP4UpiCtlgLoad,				L"CtlgLoad :: in: (1)Catalog ID, (2~6)Parameter :: out: (0)0=failure,1=success" },
	{	E_GrP4UpiPageNext,				L"PageNext :: " },
	{	E_GrP4UpiPagePrev,				L"PagePrev :: " },
	{	E_GrP4UpiSetupAdrGet,			L"SetupAdrGet :: out: (0)setup address" },
	{	E_GrP4UpiSetupAdrSet,			L"SetupAdrSet :: in: (1)setup address" },
	{	E_GrP4UpiUpdate,					L"Update data :: " },
	{	E_GrP4UpiTextAdd,					L"TextAdd :: in: (T0) add text" },
	{	E_GrP4UpiItemCntGet,			L"ItemCntGet :: out: (0) item count" },
	{	E_GrP4UpiItemCntSet,			L"ItemCntSet :: in: (1) item count" },
	{	E_GrP4UpiItemTxtIdSet,		L"ItemTxtIdSet :: in: (1) item index (2) text id" },
	{	E_GrP4UpiItemSizeSet,			L"ItemSizeSet :: in: (1) item index (2) size x (3) size y" },
	{	E_GrP4UpiItemSizeGet,			L"ItemSizeSet :: in: (1) item index :: out: (0) size x (1) size y" },
	{ E_GrP4UpiItemAdd,					L"ItemAdd :: in: (1) item ID (2) item parent ID (3) user data" },
	{ E_GrP4UpiItemDel,					L"ItemDel :: in: (1) item ID" },
	{ E_GrP4UpiItemIconSet,			L"ItemIconSet :: in: (1) item ID (2)icon slot index (3) icon image index" },
	{ E_GrP4UpiItemTextSet,			L"ItemTextSet :: in: (1) item ID (2)text slot index (T0) text" },
	{ E_GrP4UpiModeSet,					L"ModeSet :: in: (1) mode set index" },
};

St_GrP4PrjStpInfo	V_GrP4DvlStpSysTbl[E_GrP4DvlSetupSysCnt]	=	
{
	{	E_GrP4CfgOfsVdoType,				E_GrP4DvlParaTypeByte,	1,	1,	L"VdoType"				,	L"__u8	::	signal type , ntsc , pal , sdi"	},
	{	E_GrP4CfgOfsRmtId,					E_GrP4DvlParaTypeByte,	1,	1,	L"RmtId"					,	L"__u8	::	remocon id"	},
	{	E_GrP4CfgOfsLang,						E_GrP4DvlParaTypeByte,	1,	1,	L"Lang"						,	L"__u8	::	language"	},
	{	E_GrP4CfgOfsMseRes,					E_GrP4DvlParaTypeByte,	1,	1,	L"MseRes"					,	L"__u8	::	mouse resolution"	},
	{	E_GrP4CfgOfsDateFmt,				E_GrP4DvlParaTypeByte,	1,	1,	L"DateFmt"				,	L"__u8	::	date format"	},
	{	E_GrP4CfgOfsIsDst,					E_GrP4DvlParaTypeBool8,	1,	1,	L"IsDst"					,	L"BOOL8	::	daylight saving time use"	},
	{	E_GrP4CfgOfsDstStMon,				E_GrP4DvlParaTypeByte,	1,	1,	L"DstStMon"				,	L"__u8	::	daylight saving start month"	},
	{	E_GrP4CfgOfsDstStWkTh,			E_GrP4DvlParaTypeByte,	1,	1,	L"DstStWkTh"			,	L"__u8	::	daylight saving start week th"	},
	{	E_GrP4CfgOfsDstStWkDay,			E_GrP4DvlParaTypeByte,	1,	1,	L"DstStWkDay"			,	L"__u8	::	daylight saving start week day"	},
	{	E_GrP4CfgOfsDstStHour,			E_GrP4DvlParaTypeByte,	1,	1,	L"DstStHour"			,	L"__u8	::	daylight saving start hour"	},
	{	E_GrP4CfgOfsDstEdMon,				E_GrP4DvlParaTypeByte,	1,	1,	L"DstEdMon"				,	L"__u8	::	daylight saving end month"	},
	{	E_GrP4CfgOfsDstEdWkTh,			E_GrP4DvlParaTypeByte,	1,	1,	L"DstEdWkTh"			,	L"__u8	::	daylight saving end week th"	},
	{	E_GrP4CfgOfsDstEdWkDay,			E_GrP4DvlParaTypeByte,	1,	1,	L"DstEdWkDay"			,	L"__u8	::	daylight saving end week day"	},
	{	E_GrP4CfgOfsDstEdHour,			E_GrP4DvlParaTypeByte,	1,	1,	L"DstEdHour"			,	L"__u8	::	daylight saving end hour"	},
	{	E_GrP4CfgOfsSysTmSvrSync,		E_GrP4DvlParaTypeByte,	1,	1,	L"SysTmSvrSync"		,	L"__u8	::	time server Sync cycle - hour unit"	},
	{	E_GrP4CfgOfsSysTmSvrGmt,		E_GrP4DvlParaTypeByte,	1,	1,	L"SysTmSvrGmt"		,	L"__u8	::	time server gmt time value , index"	},
	{	E_GrP4CfgOfsOsdAlpha,				E_GrP4DvlParaTypeByte,	1,	1,	L"OsdAlpha"				,	L"__u8	::	display osd alpha value 0 ~ 100"	},
	{	E_GrP4CfgOfsDispRes,				E_GrP4DvlParaTypeByte,	1,	1,	L"DispRes"				,	L"__u8	::	display resolution index"	},
	{	E_GrP4CfgOfsUserId,					E_GrP4DvlParaTypeWchar,	15,	1024,	L"UserId"				,	L"WCHAR[32][16]	::	user id"	},
	{	E_GrP4CfgOfsUserPass,				E_GrP4DvlParaTypeWchar,	15,	2048,	L"UserPass"			,	L"WCHAR[2][32][16]	::	password - dual password support"	},
	{	E_GrP4CfgOfsUserGroup,			E_GrP4DvlParaTypeByte,	32,	32,	L"UserGroup"			,	L"__u8[32]	::	authority group"	},
	{	E_GrP4CfgOfsBtnBeep,				E_GrP4DvlParaTypeBool8,	1,	1,	L"BtnBeep"				,	L"BOOL8	::	button setting beep use"	},
	{	E_GrP4CfgOfsBtnAtLock,			E_GrP4DvlParaTypeByte,	1,	1,	L"BtnAtLock"			,	L"__u8	::	button setting auto lock key time index"	},
	{	E_GrP4CfgOfsBtnEmgcLock,		E_GrP4DvlParaTypeBool8,	1,	1,	L"BtnEmgcLock"		,	L"BOOL8	::	button setting emergency lock key use"	},

	{	E_GrP4CfgOfsUserAuth,				E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth0"		,	L"__u16	::	authority map 0"	},
	{	E_GrP4CfgOfsUserAuth + 2,		E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth1"		,	L"__u16	::	authority map 1"	},
	{	E_GrP4CfgOfsUserAuth + 4,		E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth2"		,	L"__u16	::	authority map 2"	},
	{	E_GrP4CfgOfsUserAuth + 6,		E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth3"		,	L"__u16	::	authority map 3"	},
	{	E_GrP4CfgOfsUserAuth + 8,		E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth4"		,	L"__u16	::	authority map 4"	},
	{	E_GrP4CfgOfsUserAuth + 10,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth5"		,	L"__u16	::	authority map 5"	},
	{	E_GrP4CfgOfsUserAuth + 12,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth6"		,	L"__u16	::	authority map 6"	},
	{	E_GrP4CfgOfsUserAuth + 14,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth7"		,	L"__u16	::	authority map 7"	},
	{	E_GrP4CfgOfsUserAuth + 16,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth8"		,	L"__u16	::	authority map 8"	},
	{	E_GrP4CfgOfsUserAuth + 18,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth9"		,	L"__u16	::	authority map 9"	},
	{	E_GrP4CfgOfsUserAuth + 20,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth10"		,	L"__u16	::	authority map 10"	},
	{	E_GrP4CfgOfsUserAuth + 22,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth11"		,	L"__u16	::	authority map 11"	},
	{	E_GrP4CfgOfsUserAuth + 24,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth12"		,	L"__u16	::	authority map 12"	},
	{	E_GrP4CfgOfsUserAuth + 26,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth13"		,	L"__u16	::	authority map 13"	},
	{	E_GrP4CfgOfsUserAuth + 28,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth14"		,	L"__u16	::	authority map 14"	},
	{	E_GrP4CfgOfsUserAuth + 30,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth15"		,	L"__u16	::	authority map 15"	},
	{	E_GrP4CfgOfsUserAuth + 32,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth16"		,	L"__u16	::	authority map 16"	},
	{	E_GrP4CfgOfsUserAuth + 34,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth17"		,	L"__u16	::	authority map 17"	},
	{	E_GrP4CfgOfsUserAuth + 36,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth18"		,	L"__u16	::	authority map 18"	},
	{	E_GrP4CfgOfsUserAuth + 38,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth19"		,	L"__u16	::	authority map 19"	},
	{	E_GrP4CfgOfsUserAuth + 40,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth20"		,	L"__u16	::	authority map 20"	},
	{	E_GrP4CfgOfsUserAuth + 42,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth21"		,	L"__u16	::	authority map 21"	},
	{	E_GrP4CfgOfsUserAuth + 44,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth22"		,	L"__u16	::	authority map 22"	},
	{	E_GrP4CfgOfsUserAuth + 46,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth23"		,	L"__u16	::	authority map 23"	},
	{	E_GrP4CfgOfsUserAuth + 48,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth24"		,	L"__u16	::	authority map 24"	},
	{	E_GrP4CfgOfsUserAuth + 50,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth25"		,	L"__u16	::	authority map 25"	},
	{	E_GrP4CfgOfsUserAuth + 52,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth26"		,	L"__u16	::	authority map 26"	},
	{	E_GrP4CfgOfsUserAuth + 54,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth27"		,	L"__u16	::	authority map 27"	},
	{	E_GrP4CfgOfsUserAuth + 56,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth28"		,	L"__u16	::	authority map 28"	},
	{	E_GrP4CfgOfsUserAuth + 58,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth29"		,	L"__u16	::	authority map 29"	},
	{	E_GrP4CfgOfsUserAuth + 60,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth30"		,	L"__u16	::	authority map 30"	},
	{	E_GrP4CfgOfsUserAuth + 62,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserAuth31"		,	L"__u16	::	authority map 31"	},

	{	E_GrP4CfgOfsUserCovert,				E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert0"	,	L"__u16	::	user covert map 0"	},
	{	E_GrP4CfgOfsUserCovert + 2,		E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert1"	,	L"__u16	::	user covert map 1"	},
	{	E_GrP4CfgOfsUserCovert + 4,		E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert2"	,	L"__u16	::	user covert map 2"	},
	{	E_GrP4CfgOfsUserCovert + 6,		E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert3"	,	L"__u16	::	user covert map 3"	},
	{	E_GrP4CfgOfsUserCovert + 8,		E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert4"	,	L"__u16	::	user covert map 4"	},
	{	E_GrP4CfgOfsUserCovert + 10,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert5"	,	L"__u16	::	user covert map 5"	},
	{	E_GrP4CfgOfsUserCovert + 12,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert6"	,	L"__u16	::	user covert map 6"	},
	{	E_GrP4CfgOfsUserCovert + 14,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert7"	,	L"__u16	::	user covert map 7"	},
	{	E_GrP4CfgOfsUserCovert + 16,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert8"	,	L"__u16	::	user covert map 8"	},
	{	E_GrP4CfgOfsUserCovert + 18,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert9"	,	L"__u16	::	user covert map 9"	},
	{	E_GrP4CfgOfsUserCovert + 20,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert10"	,	L"__u16	::	user covert map 10"	},
	{	E_GrP4CfgOfsUserCovert + 22,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert11"	,	L"__u16	::	user covert map 11"	},
	{	E_GrP4CfgOfsUserCovert + 24,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert12"	,	L"__u16	::	user covert map 12"	},
	{	E_GrP4CfgOfsUserCovert + 26,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert13"	,	L"__u16	::	user covert map 13"	},
	{	E_GrP4CfgOfsUserCovert + 28,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert14"	,	L"__u16	::	user covert map 14"	},
	{	E_GrP4CfgOfsUserCovert + 30,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert15"	,	L"__u16	::	user covert map 15"	},
	{	E_GrP4CfgOfsUserCovert + 32,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert16"	,	L"__u16	::	user covert map 16"	},
	{	E_GrP4CfgOfsUserCovert + 34,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert17"	,	L"__u16	::	user covert map 17"	},
	{	E_GrP4CfgOfsUserCovert + 36,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert18"	,	L"__u16	::	user covert map 18"	},
	{	E_GrP4CfgOfsUserCovert + 38,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert19"	,	L"__u16	::	user covert map 19"	},
	{	E_GrP4CfgOfsUserCovert + 40,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert20"	,	L"__u16	::	user covert map 20"	},
	{	E_GrP4CfgOfsUserCovert + 42,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert21"	,	L"__u16	::	user covert map 21"	},
	{	E_GrP4CfgOfsUserCovert + 44,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert22"	,	L"__u16	::	user covert map 22"	},
	{	E_GrP4CfgOfsUserCovert + 46,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert23"	,	L"__u16	::	user covert map 23"	},
	{	E_GrP4CfgOfsUserCovert + 48,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert24"	,	L"__u16	::	user covert map 24"	},
	{	E_GrP4CfgOfsUserCovert + 50,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert25"	,	L"__u16	::	user covert map 25"	},
	{	E_GrP4CfgOfsUserCovert + 52,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert26"	,	L"__u16	::	user covert map 26"	},
	{	E_GrP4CfgOfsUserCovert + 54,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert27"	,	L"__u16	::	user covert map 27"	},
	{	E_GrP4CfgOfsUserCovert + 56,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert28"	,	L"__u16	::	user covert map 28"	},
	{	E_GrP4CfgOfsUserCovert + 58,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert29"	,	L"__u16	::	user covert map 29"	},
	{	E_GrP4CfgOfsUserCovert + 60,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert30"	,	L"__u16	::	user covert map 30"	},
	{	E_GrP4CfgOfsUserCovert + 62,	E_GrP4DvlParaTypeBitmap0,	2,	2,	L"UserCovert31"	,	L"__u16	::	user covert map 31"	},


	{	E_GrP4CfgOfsNetIpv4,				E_GrP4DvlParaTypeByte,	4,	4,	L"SysIp4"					,	L"__u8[4] system ip v4"	},
	{	E_GrP4CfgOfsNetGateway4,		E_GrP4DvlParaTypeByte,	4,	4,	L"SysGateway4"		,	L"__u8[4] system gateway v4"	},
	{	E_GrP4CfgOfsSysDns4,				E_GrP4DvlParaTypeByte,	4,	4,	L"SysDns4"				,	L"__u8[4] system DNS v4"	},
	{	E_GrP4CfgOfsNetMask4,				E_GrP4DvlParaTypeByte,	4,	4,	L"NetMask4"				,	L"__u8[4]	::	netmask v4"	},
	{	E_GrP4CfgOfsNetIsDhcp,			E_GrP4DvlParaTypeBool8,	1,	1,	L"IsDhcp"					,	L"BOOL8	::	dhcp use"	},
	{	E_GrP4CfgOfsCmsPort,				E_GrP4DvlParaTypeWord,	1,	2,	L"CmsPort"				,	L"__u16	::	CMS port"	},
	{	E_GrP4CfgOfsWebPort,				E_GrP4DvlParaTypeWord,	1,	2,	L"WebPort"				,	L"__u16	::	WEB port"	},
	{	E_GrP4CfgOfsDualStrmUse,		E_GrP4DvlParaTypeBool8,	1,	1,	L"DualStrmUse"		,	L"BOOL8	::	dual stream use"	},

	{	E_GrP4CfgOfsCamUseMap,			E_GrP4DvlParaTypeBitmap0,	2,	2,	L"CamUseMap"			,	L"__u16	::	cammera use map"	},
	{	E_GrP4CfgOfsCamCvtMap,			E_GrP4DvlParaTypeBitmap0,	2,	2,	L"CamCvtMap"			,	L"__u16	::	cammera global corvert map"	},
	{	E_GrP4CfgOfsCamAgcMap,			E_GrP4DvlParaTypeBitmap0,	2,	2,	L"CamAgcMap"			,	L"__u16	::	cammera auto gain use map"	},
	{	E_GrP4CfgOfsBuzMtnTimes,		E_GrP4DvlParaTypeByte,	16,	16,	L"BuzMtn"					,	L"__u8[16]	::	internal buzzer motion time"	},
	{	E_GrP4CfgOfsBuzSenTimes,		E_GrP4DvlParaTypeByte,	16,	16,	L"BuzSen"					,	L"__u8[16]	::	internal buzzer sensor time"	},
	{	E_GrP4CfgOfsBuzLossTimes,		E_GrP4DvlParaTypeByte,	16,	16,	L"BuzLoss"				,	L"__u8[16]	::	internal buzzer vdo loss time"	},
	{	E_GrP4CfgOfsBuzHddErr,			E_GrP4DvlParaTypeByte,	1,	1,	L"BuzHddErr"			,	L"__u8	::	internal buzzer HDD error"	},
	{	E_GrP4CfgOfsBuzHddTemp,			E_GrP4DvlParaTypeByte,	1,	1,	L"BuzHddTemp"			,	L"__u8	::	internal buzzer HDD temperature"	},

	{	E_GrP4CfgOfsCamGain,				E_GrP4DvlParaTypeByte,	16,	16,	L"CamGain"				,	L"__u8[16]	::	cammera gain value table"	},
	{	E_GrP4CfgOfsCamBright,			E_GrP4DvlParaTypeByte,	16,	16,	L"CamBright"			,	L"__u8[16]	::	cammera bright value table"	},
	{	E_GrP4CfgOfsCamContrast,		E_GrP4DvlParaTypeByte,	16,	16,	L"CamContrast"		,	L"__u8[16]	::	cammera contrast value table"	},
	{	E_GrP4CfgOfsCamHue,					E_GrP4DvlParaTypeByte,	16,	16,	L"CamHue"					,	L"__u8[16]	::	cammera hue value table"	},
	{	E_GrP4CfgOfsCamTitle,				E_GrP4DvlParaTypeNone,	1,	512,	L"CamTitle"				,	L"WCHAR[16][16]	::	cammera title"	},
	{	E_GrP4CfgOfsQuadMap,				E_GrP4DvlParaTypeNone,	1,	0xA0,	L"QuadMap"				,	L"__u8[0xA0] :: quad map"	},
	{	E_GrP4CfgOfsHddType,				E_GrP4DvlParaTypeByte,	8,	8,	L"HddType"				,	L"__u8[8] :: disk type, 0 = main, 1 = mirror, 2 = none,"	},
	{	E_GrP4CfgOfsHddOverWt,			E_GrP4DvlParaTypeBool8,	1,	1,	L"HddOverWt"			,	L"BOOL8 : record overwrite"	},
	{	E_GrP4CfgOfsHddAtDel,				E_GrP4DvlParaTypeWord,	1,	2,	L"HddAtDel"				,	L"__u16 : auto delete - day unit"	},
	
	{	E_GrP4CfgOfsDiUseMap,				E_GrP4DvlParaTypeBitmap0,	2,	2,	L"DiUseMap"				,	L"__u16 :: sensor use bitmap"	},
	{	E_GrP4CfgOfsDiTypeMap,			E_GrP4DvlParaTypeBitmap0,	2,	2,	L"DiTypeMap"			,	L"__u16 :: sensor type bitmap , 0 = normal close, 1 = normal open"	},
	{	E_GrP4CfgOfsRecAdoMap,			E_GrP4DvlParaTypeBitmap0,	2,	2,	L"RecAdoMap"			,	L"__u16 :: audio record bitmap"	},
	{	E_GrP4CfgOfsRecVdoMap,			E_GrP4DvlParaTypeBitmap0,	2,	2,	L"RecVdoMap"			,	L"__u16 :: video record bitmap"	},
	{	E_GrP4CfgOfsHolyBaseYear,		E_GrP4DvlParaTypeWord,		1,	2,	L"HolyBaseYear"		,	L"__u16 :: holiday base year - 2012 year base"	},

	{	E_GrP4CfgOfsDiCamMap,				E_GrP4DvlParaTypeNone,	1,	32,	L"DiCamMap"				,	L"__u16[16] :: sensor relate cam map"	},
	{	E_GrP4CfgOfsRecPost,				E_GrP4DvlParaTypeByte,	16,	16,	L"RecPost"				,	L"__u8[16] :: record post time"	},
	{	E_GrP4CfgOfsCamSat,					E_GrP4DvlParaTypeByte,	1,	16,	L"CamSat"					,	L"__u8[16]	::	cammera saturation value table"	},
	{	E_GrP4CfgOfsRecSchd,				E_GrP4DvlParaTypeNone,	1,	0x2100,	L"RecSchd"		,	L"__u8[0x2100] :: record schedule"	},
	{	E_GrP4CfgOfsRecHoly,				E_GrP4DvlParaTypeNone,	1,	0x300,	L"RecHoly"		,	L"__u8[0x300] :: holiday map, 4bit , 4 year capable"	},
	{	E_GrP4CfgOfsRecPreUse,			E_GrP4DvlParaTypeBool8,	1,	1,	L"PreRec"					,	L"BOOL8 : preframe recording use"	},

	{	E_GrP4CfgOfsEprsNrmPres,		E_GrP4DvlParaTypeByte,	16,	16,	L"EprsNrmPres"		,	L"__u8[16] :: normal preset position"	},
	{	E_GrP4CfgOfsEprs0Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs0Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs0Idx,				E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs0Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs0Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs0Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs0Dwell,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs0Dwell"			,	L"__u8[16] :: event preset dwell time"	},

	{	E_GrP4CfgOfsEprs1Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs1Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs1Idx,				E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs1Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs1Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs1Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs1Dwell,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs1Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs2Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs2Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs2Idx,				E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs2Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs2Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs2Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs2Dwell,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs2Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs3Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs3Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs3Idx,				E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs3Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs3Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs3Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs3Dwell,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs3Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs4Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs4Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs4Idx,				E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs4Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs4Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs4Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs4Dwell,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs4Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs5Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs5Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs5Idx,				E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs5Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs5Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs5Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs5Dwell,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs5Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs6Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs6Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs6Idx,				E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs6Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs6Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs6Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs6Dwell,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs6Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs7Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs7Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs7Idx,				E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs7Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs7Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs7Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs7Dwell,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs7Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs8Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs8Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs8Idx,				E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs8Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs8Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs8Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs8Dwell,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs8Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs9Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs9Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs9Idx,				E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs9Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs9Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs9Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs9Dwell,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs9Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs10Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs10Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs10Idx,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs10Idx"			,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs10Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs10Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs10Dwell,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs10Dwell"		,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs11Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs11Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs11Idx,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs11Idx"			,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs11Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs11Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs11Dwell,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs11Dwell"		,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs12Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs12Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs12Idx,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs12Idx"			,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs12Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs12Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs12Dwell,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs12Dwell"		,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs13Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs13Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs13Idx,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs13Idx"			,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs13Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs13Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs13Dwell,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs13Dwell"		,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs14Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs14Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs14Idx,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs14Idx"			,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs14Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs14Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs14Dwell,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs14Dwell"		,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP4CfgOfsEprs15Type,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs15Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP4CfgOfsEprs15Idx,			E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs15Idx"			,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP4CfgOfsEprs15Preset,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs15Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP4CfgOfsEprs15Dwell,		E_GrP4DvlParaTypeByte,	16,	16,	L"Eprs15Dwell"		,	L"__u8[16] :: event preset dwell time"	},

	{	E_GrP4CfgOfsSysTmSvrAddr,		E_GrP4DvlParaTypeNone,	1,	128,	L"SysTmSvrAddr"		,	L"WCHAR[64]	::	time server address"	},

	{	E_GrP4CfgOfsRecRes,					E_GrP4DvlParaTypeByte,	16,	16,	L"RecRes"					,	L"__u8[16]	::	record resolution"	},
	{	E_GrP4CfgOfsRecFps,					E_GrP4DvlParaTypeByte,	16,	16,	L"RecFps"					,	L"__u8[16]	::	record fps value"	},
	{	E_GrP4CfgOfsRecQua,					E_GrP4DvlParaTypeByte,	16,	16,	L"RecQua"					,	L"__u8[16]	::	record frame quality"	},
	{	E_GrP4CfgOfsRecRuFps,				E_GrP4DvlParaTypeByte,	16,	16,	L"RecRuFps"				,	L"__u8[16]	::	record ram up fps value"	},
	{	E_GrP4CfgOfsRecRuQua,				E_GrP4DvlParaTypeByte,	16,	16,	L"RecRuQua"				,	L"__u8[16]	::	record ram up frame quality"	},
	{	E_GrP4CfgOfsRecRuRes,				E_GrP4DvlParaTypeByte,	16,	16,	L"RecRuRes"				,	L"__u8[16]	::	record ram up resolution"	},
	{	E_GrP4CfgOfsNetFps,					E_GrP4DvlParaTypeByte,	16,	16,	L"NetFps"					,	L"__u8[16]	::	network fps value"	},
	{	E_GrP4CfgOfsNetQua,					E_GrP4DvlParaTypeByte,	16,	16,	L"NetQua"					,	L"__u8[16]	::	network frame quality"	},
	{	E_GrP4CfgOfsNetRes,					E_GrP4DvlParaTypeByte,	16,	16,	L"NetRes"					,	L"__u8[16]	::	network resolution"	},
	{	E_GrP4CfgOfsNetChMap,				E_GrP4DvlParaTypeBitmap0,	2,	2,	L"NetChMap"			,	L"__u16	::	network channel map"	},
	{	E_GrP4CfgOfsCmsAlmKeep,			E_GrP4DvlParaTypeByte,	1,	1,	L"CmsKeepAlm"			,	L"BOOL8	::	cms alarm use keeping time"	},

	{	E_GrP4CfgOfsDisSeqCh,				E_GrP4DvlParaTypeByte,	16,	16,	L"DisSeqCh"				,	L"__u8[16] :: auto sequence split 1 channel time"	},
	{	E_GrP4CfgOfsDisSeqSplit4,		E_GrP4DvlParaTypeByte,	1,	1,	L"DisSeqSplit4"		,	L"__u8 :: auto sequence split 4"	},
	{	E_GrP4CfgOfsDisSeqSplit6,		E_GrP4DvlParaTypeByte,	1,	1,	L"DisSeqSplit6"		,	L"__u8 :: auto sequence split 6"	},
	{	E_GrP4CfgOfsDisSeqSplit8,		E_GrP4DvlParaTypeByte,	1,	1,	L"DisSeqSplit8"		,	L"__u8 :: auto sequence split 8"	},
	{	E_GrP4CfgOfsDisSeqSplit9,		E_GrP4DvlParaTypeByte,	1,	1,	L"DisSeqSplit9"		,	L"__u8 :: auto sequence split 9"	},
	{	E_GrP4CfgOfsDisSeqSplit10,	E_GrP4DvlParaTypeByte,	1,	1,	L"DisSeqSplit10"	,	L"__u8 :: auto sequence split 10"	},
	{	E_GrP4CfgOfsDisSeqSplit13,	E_GrP4DvlParaTypeByte,	1,	1,	L"DisSeqSplit13"	,	L"__u8 :: auto sequence split 13"	},
	{	E_GrP4CfgOfsDisSeqLoss,			E_GrP4DvlParaTypeBool8,	1,	1,	L"DisSeqLoss"			,	L"BOOL8 :: video loss channel skip"	},
	{	E_GrP4CfgOfsDisSplitMode,		E_GrP4DvlParaTypeByte,	1,	1,	L"DisSplitMode"		,	L"__u8 :: split mode"	},

	{	E_GrP4CfgOfsSpotChTime,			E_GrP4DvlParaTypeByte,	16,	16,	L"SpotChTime"	,			L"__u8[16] :: spot channel time"	},
	{	E_GrP4CfgOfsSpotGroupTime,	E_GrP4DvlParaTypeByte,	4,	4,	L"SpotGroupTime"	,	L"__u8[4] :: spot quad 4 dwell time"	},
	{	E_GrP4CfgOfsSpotQuadMode,		E_GrP4DvlParaTypeByte,	1,	1,	L"SpotQuadMode"		,	L"__u8 :: spot quad mode"	},
	{	E_GrP4CfgOfsSpotEvtMode,		E_GrP4DvlParaTypeByte,	1,	1,	L"SpotEvtMode"		,	L"__u8 :: spot event mode"	},
	{	E_GrP4CfgOfsSpotFixCh,			E_GrP4DvlParaTypeByte,	1,	1,	L"SpotFixCh"			,	L"__u8 :: spot fix channel"	},

	{	E_GrP4CfgOfsDoMtnOutTimes,	E_GrP4DvlParaTypeNone,	1,	256,	L"DoMtnOutTimes"	,	L"__u8[16][16]	:: alarm motion alram time - sec unit"	},
	{	E_GrP4CfgOfsDoSenOutTimes,	E_GrP4DvlParaTypeNone,	1,	256,	L"DoSenOutTimes"	,	L"__u8[16][16]	:: alarm sensor alram time - sec unit"	},
	{	E_GrP4CfgOfsDoVdlOutTimes,	E_GrP4DvlParaTypeNone,	1,	256,	L"DoVdlOutTimes"	,	L"__u8[16][16]	:: alarm video loss alram time - sec unit"	},
	{	E_GrP4CfgOfsDoHddErrUseMap,	E_GrP4DvlParaTypeBitmap0,	2,	2,		L"DoHddErrUseMap"	,	L"__u16					:: hdd error alram use alarm channel map"	},
	{	E_GrP4CfgOfsDoHddErrDur,		E_GrP4DvlParaTypeByte,	16,	16,		L"DoHddErrDur"	,		L"__u8[16]			:: hdd error alram duration"	},

	{	E_GrP4CfgOfsUartSpeed,			E_GrP4DvlParaTypeByte,	4,	4,	L"UartSpeed"	,	L"__u8[4] :	UART port speed"	},
	{	E_GrP4CfgOfsUartDataBit,		E_GrP4DvlParaTypeByte,	4,	4,	L"UartDataBit"	,	L"__u8[4] :	data bit size, 0=8bit, 1=7bit"	},
	{	E_GrP4CfgOfsUartStopBit,		E_GrP4DvlParaTypeByte,	4,	4,	L"UartStopBit"	,	L"__u8[4] :	stop bit, 0=1bit, 1=1.5bit, 2=2bit"	},
	{	E_GrP4CfgOfsUartParity,			E_GrP4DvlParaTypeByte,	4,	4,	L"UartParityBit"	,	L"__u8[4] :	parity bit, 0= none, 1=even, 2=odd"	},

	{	E_GrP4CfgOfsNetDdnsAdr,			E_GrP4DvlParaTypeNone,	1,	128,	L"DdnsAddr"	,	L"WCHAR[64] : DDNS server Address"	},

	{	E_GrP4CfgOfsNetFdnsType,		E_GrP4DvlParaTypeByte,	1,	1,	L"FdnsServer"	,	L"__u8 : free DNS server 0=none,1=DynDns,2=no-ip"	},
	{	E_GrP4CfgOfsNetFdnsUrl,			E_GrP4DvlParaTypeNone,	1,	128,	L"FdnsUrl"	,	L"WCHAR[64] : free DNS URL"	},
	{	E_GrP4CfgOfsNetFdnsId,			E_GrP4DvlParaTypeNone,	1,	128,	L"FdnsId"	,	L"WCHAR[64] : free DNS id"	},
	{	E_GrP4CfgOfsNetFdnsPass,		E_GrP4DvlParaTypeNone,	1,	64,	L"FdnsPass"	,	L"WCHAR[32] : free DNS password"	},

	{	E_GrP4CfgOfsMtnMap,					E_GrP4DvlParaTypeNone,	1,	2048,	L"MtnMap"	,		L"__u32[16][32] : motion sense map"	},
	{	E_GrP4CfgOfsMtnSensi,				E_GrP4DvlParaTypeNone,	1,	384,	L"MtnSensi"	,	L"__u8[16][24] : motion sensitivity schedule table"	},

	{	E_GrP4CfgOfsEvtPpEvt,				E_GrP4DvlParaTypeByte,	16,	16,	L"EvtPpEvt"	,	L"__u8[16] : event pop up event mask"	},
	{	E_GrP4CfgOfsEvtPpDur,				E_GrP4DvlParaTypeByte,	16,	16,	L"EvtPpDur",	L"__u8[16] : event pop up duration"	},

	{	E_GrP4CfgOfsPtzType,				E_GrP4DvlParaTypeByte,	16,	16,	L"PtzModel"	,	L"__u8[16] : PTZ model"	},
	{	E_GrP4CfgOfsPtzId,					E_GrP4DvlParaTypeByte,	16,	16,	L"PtzId"	,	L"__u8[16] : PTZ ID"	},
	{	E_GrP4CfgOfsPtzPort,				E_GrP4DvlParaTypeByte,	16,	16,	L"PtzUart"	,	L"__u8[16] : PTZ UART port index"	},
	{	E_GrP4CfgOfsPtzBaud,				E_GrP4DvlParaTypeByte,	16,	16,	L"PtzBaud"	,	L"__u8[16] : PTZ UART baud rate"	},
	{	E_GrP4CfgOfsPtzDataBit,			E_GrP4DvlParaTypeByte,	16,	16,	L"PtzDataBit"	,	L"__u8[16] : PTZ UART data bit"	},
	{	E_GrP4CfgOfsPtzStopBit,			E_GrP4DvlParaTypeByte,	16,	16,	L"PtzStopBit"	,	L"__u8[16] : PTZ UART stop bit"	},
	{	E_GrP4CfgOfsPtzParity,			E_GrP4DvlParaTypeByte,	16,	16,	L"PtzParity"	,	L"__u8[16] : PTZ UART parity bit"	},
	{	E_GrP4CfgOfsPtzAtb,					E_GrP4DvlParaTypeByte,	16,	16,	L"PtzAtb"	,	L"__u8[16] : PTZ attribute bit0=pan reverse, bit1=tilt reverse"	},
	{	E_GrP4CfgOfsPtzTourType,		E_GrP4DvlParaTypeByte,	16,	16,	L"PtzTourType"	,	L"__u8[16] : PTZ tour type 0=DVR, 1=CAMERA"	},
	{	E_GrP4CfgOfsPtzSpdPan,			E_GrP4DvlParaTypeByte,	16,	16,	L"PtzSpdPan"	,	L"__u8[16] : PTZ pan speed"	},
	{	E_GrP4CfgOfsPtzSpdTilt,			E_GrP4DvlParaTypeByte,	16,	16,	L"PtzSpdTilt"	,	L"__u8[16] : PTZ tilt speed"	},
	{	E_GrP4CfgOfsPtzSpdZoom,			E_GrP4DvlParaTypeByte,	16,	16,	L"PtzSpdZoom"	,	L"__u8[16] : PTZ zoom speed"	},
	{	E_GrP4CfgOfsPtzSpdFocus,		E_GrP4DvlParaTypeByte,	16,	16,	L"PtzSpdFocus"	,	L"__u8[16] : PTZ focus speed"	},
	{	E_GrP4CfgOfsPtzTourCfg,			E_GrP4DvlParaTypeNone,	1,	768,	L"PtzTourCfg"	,	L"__u32[16][12] : PTZ tour-refer document"	},

	{	E_GrP4CfgOfsNetSmtpAdr,			E_GrP4DvlParaTypeNone,	1,	128,	L"SmtpAdr"	,	L"WCHAR[64] : email server address"	},
	{	E_GrP4CfgOfsNetSmtpPort,		E_GrP4DvlParaTypeWord,	1,	2,		L"SmtpPort"	,	L"__u16 : email server port"	},
	{	E_GrP4CfgOfsSmtpId,					E_GrP4DvlParaTypeNone,	1,	128,	L"SmtpId"	,		L"WCHAR[64] : SMTP server id"	},
	{	E_GrP4CfgOfsSmtpPass,				E_GrP4DvlParaTypeNone,	1,	64,		L"SmtpPass"	,	L"WCHAR[32] : SMTP server password"	},
	{	E_GrP4CfgOfsNetEmailTitle,	E_GrP4DvlParaTypeNone,	1,	128,	L"EmailTitle"	,	L"WCHAR[64] : email title"	},
	{	E_GrP4CfgOfsEmailSender,		E_GrP4DvlParaTypeNone,	1,	128,	L"EmailSender"	,	L"WCHAR[64] : email sender"	},
	{	E_GrP4CfgOfsEmailTo1,				E_GrP4DvlParaTypeNone,	1,	128,	L"EmailTo1"	,	L"WCHAR[64] : email to address 1"	},
	{	E_GrP4CfgOfsEmailTo2,				E_GrP4DvlParaTypeNone,	1,	128,	L"EmailTo2"	,	L"WCHAR[64] : email to address 2"	},
	{	E_GrP4CfgOfsEmailTo3,				E_GrP4DvlParaTypeNone,	1,	128,	L"EmailTo3"	,	L"WCHAR[64] : email to address 3"	},
	{ E_GrP4CfgOfsEmailTo4,				E_GrP4DvlParaTypeNone,	1,	128,	L"EmailTo4",	L"WCHAR[64] : email to address 4" },

	{	E_GrP4CfgOfsNetEmailEvt,		E_GrP4DvlParaTypeWord,	1,	2,		L"EmailEvt"	,	L"__u16 : email notify event - event log mask"	},
	{	E_GrP4CfgOfsNetEmailInter,	E_GrP4DvlParaTypeByte,	1,	1,		L"EmailInter"	,	L"__u8 : email interval - sec table index"	},
	{	E_GrP4CfgOfsNetSmtpSecu,		E_GrP4DvlParaTypeByte,	1,	1,		L"SmtpSecu"	,	L"__u8 : email security type"	},

	{	E_GrP4CfgOfsCtrlModel,			E_GrP4DvlParaTypeByte,	1,	1,		L"CtrlModel"	,	L"__u8 : controller model"	},
	{	E_GrP4CfgOfsCtrlId,					E_GrP4DvlParaTypeByte,	1,	1,		L"CtrlId"			,	L"__u8 : controller ID"	},
	{	E_GrP4CfgOfsCtrlUart,				E_GrP4DvlParaTypeByte,	1,	1,		L"CtrlUart"		,	L"__u8 : controller UART port"	},

	{ E_GrP4CfgOfsPrivacyMap,			E_GrP4DvlParaTypeAsmStpAdr, 1, 1, L"PrivacyMap", L"__u16 : Privacy Cfg Map address" },
	{ E_GrP4CfgOfsPrivacyEnable,	E_GrP4DvlParaTypeBool8, 1, 1,			L"PrivacyEnable", L"BOOL8 : Privacy Enable" },

	{ E_GrP4CfgOfsCamSharp,				E_GrP4DvlParaTypeByte, 16, 16,		L"CamSharp", L"__u8[16]	::	cammera sharpness value table" },
};

//====================================================================
//--------------------------------------------------------------------
__u32	GrP4DvlParaTypeUnitSizeGet( __u8 A_Type )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		switch ( A_Type )
		{
			case E_GrP4DvlParaTypeChar:
			case E_GrP4DvlParaTypeByte:
			case E_GrP4DvlParaTypeBool8:
			case E_GrP4DvlParaTypeStdValType:
			case E_GrP4DvlParaTypeAsmOpType:
			case E_GrP4DvlParaTypeAsmFlagIdx:
			case E_GrP4DvlParaTypeAsmArgType:
			case E_GrP4DvlParaTypeAsmIregIdx:
			case E_GrP4DvlParaTypeAsmCmpType:
			case E_GrP4DvlParaTypeAsmFlagSet:
			case E_GrP4DvlParaTypeAsmCalc:
			case E_GrP4DvlParaTypeAsmNotSet:
			case E_GrP4DvlParaTypeAsmJumpCall:
			case E_GrP4DvlParaTypeAsmJumpDir:
			case E_GrP4DvlParaTypeAsmTregIdx:
			case E_GrP4DvlParaTypeConIdx:
			case E_GrP4DvlParaTypePosBssLt:
			case E_GrP4DvlParaTypePosBssUp:
			case E_GrP4DvlParaTypePosBssRt:
			case E_GrP4DvlParaTypePosBssDn:
			case E_GrP4DvlParaTypeAsmAddType:
			case E_GrP4DvlParaTypeKeyEvtType:
			case E_GrP4DvlParaTypeKeyCode:
			case E_GrP4DvlParaTypeUsvalType:
			case E_GrP4DvlParaTypeRelUpi:
				Tv_Result	=	1;
				break;
			case E_GrP4DvlParaTypeShort:
			case E_GrP4DvlParaTypeWord:
			case E_GrP4DvlParaTypeWchar:
			case E_GrP4DvlParaTypeScptIdAny:
			case E_GrP4DvlParaTypePntCode:
			case E_GrP4DvlParaTypeAsmCode:
			case E_GrP4DvlParaTypeVarAdr:
			case E_GrP4DvlParaTypeAsmGvsAdr:
			case E_GrP4DvlParaTypeAsmGvuAdr:
			case E_GrP4DvlParaTypeAsmStpAdr:
			case E_GrP4DvlParaTypeUpiCmd:
			case E_GrP4DvlParaTypeApiCmd:
			case E_GrP4DvlParaTypeTxtId:
			case E_GrP4DvlParaTypeScptIdWin:
				Tv_Result	=	2;
				break;
			case E_GrP4DvlParaTypeInt:
			case E_GrP4DvlParaTypeDword:
			case E_GrP4DvlParaTypeColor:
				Tv_Result	=	4;
				break;

			default:
				Tv_Result	=	0;
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
