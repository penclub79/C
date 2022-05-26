/*
	GAUSE Platform develop base

*/

#include <P3/Dvl/GrP3DvlBase.h>
#include <P3/GrP3Base.h>
#include <P3/GrP3UiCmd.h>

#include <P3/GrP3Cfg.h>

//--------------------------------------------------------------------
// local constant


//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function


//--------------------------------------------------------------------
//global var

WCHAR*	V_GrP3DvlClassNameTbl[E_GrP3DoClassCnt]	=	
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
	L"Grid Push Setup",
	L"Grid Play time bar",
};

WCHAR*	V_GrP3DvlRelUpiNameTbl[E_GrP3RelUpiCnt]	=	
{
	L"None",
	L"Value",
	L"RecType",
	L"Channel",
};

St_GrP3DvlUpiInfo	V_GrP3DvlUpiTbl[E_GrP3UpiCnt]	=	
{
	{	E_GrP3UpiClassIdGet,			L"ClassIdGet :: out: (0)ClassId" },
	{	E_GrP3UpiScptIdGet,				L"ScriptIdGet :: out: (0)ScriptId" },
	{	E_GrP3UpiPosGet,					L"PosGet :: out: (1)x,(2)y" },
	{	E_GrP3UpiPosSet,					L"PosSet :: in: (1)x,(2)y" },
	{	E_GrP3UpiSizeGet,					L"SizeGet :: out: (1)size x,(2)size y" },
	{	E_GrP3UpiSizeSet,					L"SizeSet :: in: (1)size x,(2)size y" },
	{	E_GrP3UpiVisibleGet,			L"VisibleGet :: out: (0)0 = behind, 1= show" },
	{	E_GrP3UpiVisibleSet,			L"VisibleSet :: in: (1)0 = behind, 1= show" },
	{	E_GrP3UpiEnableGet,				L"EnableGet :: out: (0)0 = false, 1= true" },
	{	E_GrP3UpiEnableSet,				L"EnableSet :: in: (1)0 = false, 1= true" },
	{	E_GrP3UpiFcsMvTblGet,			L"FcsMvTblGet :: in: (1)left, (2)right, (3)up, (4)down " },
	{	E_GrP3UpiFcsMvTblSet,			L"FcsMvTblSet :: out: (1)left, (2)right, (3)up, (4)down" },
	{	E_GrP3UpiUserDataGet,			L"UserDataGet :: in: (1)index :: out: (0)value " },
	{	E_GrP3UpiUserDataSet,			L"UserDataSet :: in: (1)index, (2)value " },
	{	E_GrP3UpiReqDraw,					L"ReqDraw :: in: (1) 0=normal,1=forceless draw " },
	{	E_GrP3UpiWinClose,				L"WinClose ::  " },
	{	E_GrP3UpiValueGet,				L"ValueGet ::  out: (0)value" },
	{	E_GrP3UpiValueSet,				L"ValueSet ::  in: (1)value" },
	{	E_GrP3UpiTextGet,					L"TextGet :: in: (1)text register index" },
	{	E_GrP3UpiTextSet,					L"TextSet :: in: (1)text register index" },
	{	E_GrP3UpiMinValueGet,			L"MinValueGet :: out: (0)value" },
	{	E_GrP3UpiMinValueSet,			L"MinValueSet :: in: (1)value" },
	{	E_GrP3UpiMaxValueGet,			L"MaxValueGet :: out: (0)value" },
	{	E_GrP3UpiMaxValueSet,			L"MaxValueSet :: in: (1)value" },
	{	E_GrP3UpiEvtHostIdGet,		L"EvtHostIdGet :: out: (0)host script id" },
	{	E_GrP3UpiEvtHostIdSet,		L"EvtHostIdSet :: in: (1)host script id" },
	{	E_GrP3UpiTextIdGet,				L"TextIdGet :: out: (0)text id" },
	{	E_GrP3UpiTextIdSet,				L"TextIdSet :: in: (1)text id" },
	{	E_GrP3UpiRecTypeGet,			L"RecTypeGet :: out: (0)record type" },
	{	E_GrP3UpiRecTypeSet,			L"RecTypeSet :: in: (1)record type" },
	{	E_GrP3UpiFrmPageSet,			L"FrmPageSet :: in: (1)PageId, (2)Layer, (3)0=none,1=focus move" },
	{	E_GrP3UpiFrmPageBack,			L"FrmPageBack :: in: (1)0=none,1=focus move" },
	{	E_GrP3UpiChGet,						L"ChGet :: out: (0)channel number" },
	{	E_GrP3UpiChSet,						L"ChSet :: in: (1)channel number" },
	{	E_GrP3UpiSubShowGet,			L"SubShowGet :: in: (1)control number :: out: (0)0=off,1=show" },
	{	E_GrP3UpiSubShowSet,			L"SubShowSet :: in: (1)control number (2)0=off,1=show" },
	{	E_GrP3UpiFocusIdSet,			L"FocusIdSet :: " },
	{	E_GrP3UpiCtlgLoad,				L"CtlgLoad :: in: (1)Catalog ID, (2~6)Parameter :: out: (0)0=failure,1=success" },
	{	E_GrP3UpiPageNext,				L"PageNext :: " },
	{	E_GrP3UpiPagePrev,				L"PagePrev :: " },
	{	E_GrP3UpiSetupAdrGet,			L"SetupAdrGet :: out: (0)setup address" },
	{	E_GrP3UpiSetupAdrSet,			L"SetupAdrSet :: in: (1)setup address" },
	{	E_GrP3UpiUpdate,					L"Update data :: " },
	{	E_GrP3UpiTextAdd,					L"TextAdd :: in: (T0) add text" },
	{	E_GrP3UpiItemCntGet,			L"ItemCntGet :: out: (0) item count" },
	{	E_GrP3UpiItemTxtIdSet,		L"ItemTxtIdSet :: in: (1) item index (2) text id" },
	{	E_GrP3UpiItemSizeSet,			L"ItemSizeSet :: in: (1) item index (2) size x (3) size y" },
	{	E_GrP3UpiItemSizeGet,			L"ItemSizeSet :: in: (1) item index :: out: (0) size x (1) size y" },
	
};

St_GrP3PrjStpInfo	V_GrP3DvlStpSysTbl[E_GrP3DvlSetupSysCnt]	=	
{
	{	E_GrP3CfgOfsVdoType,				E_GrP3DvlParaTypeByte,	1,	1,	L"VdoType"				,	L"__u8	::	signal type , ntsc , pal , sdi"	},
	{	E_GrP3CfgOfsRmtId,					E_GrP3DvlParaTypeByte,	1,	1,	L"RmtId"					,	L"__u8	::	remocon id"	},
	{	E_GrP3CfgOfsLang,						E_GrP3DvlParaTypeByte,	1,	1,	L"Lang"						,	L"__u8	::	language"	},
	{	E_GrP3CfgOfsMseRes,					E_GrP3DvlParaTypeByte,	1,	1,	L"MseRes"					,	L"__u8	::	mouse resolution"	},
	{	E_GrP3CfgOfsDateFmt,				E_GrP3DvlParaTypeByte,	1,	1,	L"DateFmt"				,	L"__u8	::	date format"	},
	{	E_GrP3CfgOfsIsDst,					E_GrP3DvlParaTypeBool8,	1,	1,	L"IsDst"					,	L"BOOL8	::	daylight saving time use"	},
	{	E_GrP3CfgOfsDstStMon,				E_GrP3DvlParaTypeByte,	1,	1,	L"DstStMon"				,	L"__u8	::	daylight saving start month"	},
	{	E_GrP3CfgOfsDstStWkTh,			E_GrP3DvlParaTypeByte,	1,	1,	L"DstStWkTh"			,	L"__u8	::	daylight saving start week th"	},
	{	E_GrP3CfgOfsDstStWkDay,			E_GrP3DvlParaTypeByte,	1,	1,	L"DstStWkDay"			,	L"__u8	::	daylight saving start week day"	},
	{	E_GrP3CfgOfsDstStHour,			E_GrP3DvlParaTypeByte,	1,	1,	L"DstStHour"			,	L"__u8	::	daylight saving start hour"	},
	{	E_GrP3CfgOfsDstEdMon,				E_GrP3DvlParaTypeByte,	1,	1,	L"DstEdMon"				,	L"__u8	::	daylight saving end month"	},
	{	E_GrP3CfgOfsDstEdWkTh,			E_GrP3DvlParaTypeByte,	1,	1,	L"DstEdWkTh"			,	L"__u8	::	daylight saving end week th"	},
	{	E_GrP3CfgOfsDstEdWkDay,			E_GrP3DvlParaTypeByte,	1,	1,	L"DstEdWkDay"			,	L"__u8	::	daylight saving end week day"	},
	{	E_GrP3CfgOfsDstEdHour,			E_GrP3DvlParaTypeByte,	1,	1,	L"DstEdHour"			,	L"__u8	::	daylight saving end hour"	},
	{	E_GrP3CfgOfsSysTmSvrType,		E_GrP3DvlParaTypeByte,	1,	1,	L"SysTmSvrType"		,	L"__u8	::	time server type"	},
	{	E_GrP3CfgOfsSysTmSvrSync,		E_GrP3DvlParaTypeByte,	1,	1,	L"SysTmSvrSync"		,	L"__u8	::	time server Sync cycle - hour unit"	},
	{	E_GrP3CfgOfsSysTmSvrGmt,		E_GrP3DvlParaTypeByte,	1,	1,	L"SysTmSvrGmt"		,	L"__u8	::	time server gmt time value , index"	},
	{	E_GrP3CfgOfsOsdAlpha,				E_GrP3DvlParaTypeByte,	1,	1,	L"OsdAlpha"				,	L"__u8	::	display osd alpha value 0 ~ 100"	},
	{	E_GrP3CfgOfsDispRes,				E_GrP3DvlParaTypeByte,	1,	1,	L"DispRes"				,	L"__u8	::	display resolution index"	},
	{	E_GrP3CfgOfsUserId,					E_GrP3DvlParaTypeNone,	1,	256,	L"UserId"					,	L"WCHAR[8][16]	::	user id"	},
	{	E_GrP3CfgOfsUserPass,				E_GrP3DvlParaTypeNone,	1,	512,	L"UserPass"				,	L"WCHAR[2][8][16]	::	password - dual password support"	},
	{	E_GrP3CfgOfsUserGroup,			E_GrP3DvlParaTypeByte,	8,	8,	L"UserGroup"			,	L"__u8[8]	::	authority group"	},
	{	E_GrP3CfgOfsBtnBeep,				E_GrP3DvlParaTypeBool8,	1,	1,	L"BtnBeep"				,	L"BOOL8	::	button setting beep use"	},
	{	E_GrP3CfgOfsBtnAtLock,			E_GrP3DvlParaTypeByte,	1,	1,	L"BtnAtLock"			,	L"__u8	::	button setting auto lock key time index"	},
	{	E_GrP3CfgOfsBtnEmgcLock,		E_GrP3DvlParaTypeBool8,	1,	1,	L"BtnEmgcLock"		,	L"BOOL8	::	button setting emergency lock key use"	},

	{	E_GrP3CfgOfsUserAuth,				E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserAuth0"		,	L"__u16	::	authority map 0"	},
	{	E_GrP3CfgOfsUserAuth + 2,		E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserAuth1"		,	L"__u16	::	authority map 1"	},
	{	E_GrP3CfgOfsUserAuth + 4,		E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserAuth2"		,	L"__u16	::	authority map 2"	},
	{	E_GrP3CfgOfsUserAuth + 6,		E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserAuth3"		,	L"__u16	::	authority map 3"	},
	{	E_GrP3CfgOfsUserAuth + 8,		E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserAuth4"		,	L"__u16	::	authority map 4"	},
	{	E_GrP3CfgOfsUserAuth + 10,	E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserAuth5"		,	L"__u16	::	authority map 5"	},
	{	E_GrP3CfgOfsUserAuth + 12,	E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserAuth6"		,	L"__u16	::	authority map 6"	},
	{	E_GrP3CfgOfsUserAuth + 14,	E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserAuth7"		,	L"__u16	::	authority map 7"	},

	{	E_GrP3CfgOfsUserCovert,			E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserCovert0"	,	L"__u16	::	user covert map 0"	},
	{	E_GrP3CfgOfsUserCovert + 2,		E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserCovert1"	,	L"__u16	::	user covert map 1"	},
	{	E_GrP3CfgOfsUserCovert + 4,		E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserCovert2"	,	L"__u16	::	user covert map 2"	},
	{	E_GrP3CfgOfsUserCovert + 6,		E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserCovert3"	,	L"__u16	::	user covert map 3"	},
	{	E_GrP3CfgOfsUserCovert + 8,		E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserCovert4"	,	L"__u16	::	user covert map 4"	},
	{	E_GrP3CfgOfsUserCovert + 10,	E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserCovert5"	,	L"__u16	::	user covert map 5"	},
	{	E_GrP3CfgOfsUserCovert + 12,	E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserCovert6"	,	L"__u16	::	user covert map 6"	},
	{	E_GrP3CfgOfsUserCovert + 14,	E_GrP3DvlParaTypeBitmap0,	2,	2,	L"UserCovert7"	,	L"__u16	::	user covert map 7"	},

	{	E_GrP3CfgOfsNetIpv4,				E_GrP3DvlParaTypeByte,	4,	4,	L"SysIp4"					,	L"__u8[4] system ip v4"	},
	{	E_GrP3CfgOfsNetGateway4,		E_GrP3DvlParaTypeByte,	4,	4,	L"SysGateway4"		,	L"__u8[4] system gateway v4"	},
	{	E_GrP3CfgOfsSysDns4,				E_GrP3DvlParaTypeByte,	4,	4,	L"SysDns4"				,	L"__u8[4] system DNS v4"	},
	{	E_GrP3CfgOfsNetMask4,				E_GrP3DvlParaTypeByte,	4,	4,	L"NetMask4"				,	L"__u8[4]	::	netmask v4"	},
	{	E_GrP3CfgOfsNetIsDhcp,			E_GrP3DvlParaTypeBool8,	1,	1,	L"IsDhcp"					,	L"BOOL8	::	dhcp use"	},
	{	E_GrP3CfgOfsCmsPort,				E_GrP3DvlParaTypeWord,	1,	2,	L"CmsPort"				,	L"__u16	::	CMS port"	},
	{	E_GrP3CfgOfsWebPort,				E_GrP3DvlParaTypeWord,	1,	2,	L"WebPort"				,	L"__u16	::	WEB port"	},
	{	E_GrP3CfgOfsDualStrmUse,		E_GrP3DvlParaTypeBool8,	1,	1,	L"DualStrmUse"		,	L"BOOL8	::	dual stream use"	},

	{	E_GrP3CfgOfsCamUseMap,			E_GrP3DvlParaTypeBitmap0,	2,	2,	L"CamUseMap"			,	L"__u16	::	cammera use map"	},
	{	E_GrP3CfgOfsCamCvtMap,			E_GrP3DvlParaTypeBitmap0,	2,	2,	L"CamCvtMap"			,	L"__u16	::	cammera global corvert map"	},
	{	E_GrP3CfgOfsCamAgcMap,			E_GrP3DvlParaTypeBitmap0,	2,	2,	L"CamAgcMap"			,	L"__u16	::	cammera auto gain use map"	},
	{	E_GrP3CfgOfsBuzMtnTimes,		E_GrP3DvlParaTypeByte,	16,	16,	L"BuzMtn"					,	L"__u8[16]	::	internal buzzer motion time"	},
	{	E_GrP3CfgOfsBuzSenTimes,		E_GrP3DvlParaTypeByte,	16,	16,	L"BuzSen"					,	L"__u8[16]	::	internal buzzer sensor time"	},
	{	E_GrP3CfgOfsBuzLossTimes,		E_GrP3DvlParaTypeByte,	16,	16,	L"BuzLoss"				,	L"__u8[16]	::	internal buzzer vdo loss time"	},
	{	E_GrP3CfgOfsBuzHddErr,			E_GrP3DvlParaTypeByte,	1,	1,	L"BuzHddErr"			,	L"__u8	::	internal buzzer HDD error"	},
	{	E_GrP3CfgOfsBuzHddTemp,			E_GrP3DvlParaTypeByte,	1,	1,	L"BuzHddTemp"			,	L"__u8	::	internal buzzer HDD temperature"	},

	{	E_GrP3CfgOfsCamGain,				E_GrP3DvlParaTypeByte,	16,	16,	L"CamGain"				,	L"__u8[16]	::	cammera gain value table"	},
	{	E_GrP3CfgOfsCamBright,			E_GrP3DvlParaTypeByte,	16,	16,	L"CamBright"			,	L"__u8[16]	::	cammera bright value table"	},
	{	E_GrP3CfgOfsCamContrast,		E_GrP3DvlParaTypeByte,	16,	16,	L"CamContrast"		,	L"__u8[16]	::	cammera contrast value table"	},
	{	E_GrP3CfgOfsCamHue,					E_GrP3DvlParaTypeByte,	16,	16,	L"CamHue"					,	L"__u8[16]	::	cammera hue value table"	},
	{	E_GrP3CfgOfsCamTitle,				E_GrP3DvlParaTypeNone,	1,	512,	L"CamTitle"				,	L"WCHAR[16][16]	::	cammera title"	},
	{	E_GrP3CfgOfsQuadMap,				E_GrP3DvlParaTypeNone,	1,	0xA0,	L"QuadMap"				,	L"__u8[0xA0] :: quad map"	},
	{	E_GrP3CfgOfsHddType,				E_GrP3DvlParaTypeByte,	8,	8,	L"HddType"				,	L"__u8[8] :: disk type, 0 = main, 1 = mirror, 2 = none,"	},
	{	E_GrP3CfgOfsHddOverWt,			E_GrP3DvlParaTypeBool8,	1,	1,	L"HddOverWt"			,	L"BOOL8 : record overwrite"	},
	{	E_GrP3CfgOfsHddAtDel,				E_GrP3DvlParaTypeByte,	1,	1,	L"HddAtDel"				,	L"__u8 : auto delete - day unit"	},
	
	{	E_GrP3CfgOfsDiUseMap,				E_GrP3DvlParaTypeBitmap0,	2,	2,	L"DiUseMap"				,	L"__u16 :: sensor use bitmap"	},
	{	E_GrP3CfgOfsDiTypeMap,			E_GrP3DvlParaTypeBitmap0,	2,	2,	L"DiTypeMap"			,	L"__u16 :: sensor type bitmap , 0 = normal close, 1 = normal open"	},
	{	E_GrP3CfgOfsRecAdoMap,			E_GrP3DvlParaTypeBitmap0,	2,	2,	L"RecAdoMap"			,	L"__u16 :: audio record bitmap"	},
	{	E_GrP3CfgOfsRecVdoMap,			E_GrP3DvlParaTypeBitmap0,	2,	2,	L"RecVdoMap"			,	L"__u16 :: video record bitmap"	},
	{	E_GrP3CfgOfsHolyBaseYear,		E_GrP3DvlParaTypeWord,		1,	2,	L"HolyBaseYear"		,	L"__u16 :: holiday base year - 2012 year base"	},

	{	E_GrP3CfgOfsDiCamMap,				E_GrP3DvlParaTypeNone,	1,	32,	L"DiCamMap"				,	L"__u16[16] :: sensor relate cam map"	},
	{	E_GrP3CfgOfsRecPost,				E_GrP3DvlParaTypeByte,	16,	16,	L"RecPost"				,	L"__u8[16] :: record post time"	},
	{	E_GrP3CfgOfsCamSat,					E_GrP3DvlParaTypeByte,	1,	16,	L"CamSat"					,	L"__u8[16]	::	cammera saturation value table"	},
	{	E_GrP3CfgOfsRecSchd,				E_GrP3DvlParaTypeNone,	1,	0x2100,	L"RecSchd"		,	L"__u8[0x2100] :: record schedule"	},
	{	E_GrP3CfgOfsRecHoly,				E_GrP3DvlParaTypeNone,	1,	0x300,	L"RecHoly"		,	L"__u8[0x300] :: holiday map, 4bit , 4 year capable"	},
	{	E_GrP3CfgOfsRecPreUse,			E_GrP3DvlParaTypeBool8,	1,	1,	L"PreRec"					,	L"BOOL8 : preframe recording use"	},

	{	E_GrP3CfgOfsEprsNrmPres,		E_GrP3DvlParaTypeByte,	16,	16,	L"EprsNrmPres"		,	L"__u8[16] :: normal preset position"	},
	{	E_GrP3CfgOfsEprs0Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs0Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs0Idx,				E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs0Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs0Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs0Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs0Dwell,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs0Dwell"			,	L"__u8[16] :: event preset dwell time"	},

	{	E_GrP3CfgOfsEprs1Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs1Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs1Idx,				E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs1Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs1Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs1Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs1Dwell,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs1Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs2Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs2Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs2Idx,				E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs2Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs2Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs2Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs2Dwell,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs2Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs3Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs3Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs3Idx,				E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs3Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs3Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs3Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs3Dwell,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs3Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs4Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs4Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs4Idx,				E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs4Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs4Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs4Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs4Dwell,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs4Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs5Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs5Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs5Idx,				E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs5Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs5Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs5Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs5Dwell,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs5Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs6Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs6Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs6Idx,				E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs6Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs6Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs6Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs6Dwell,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs6Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs7Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs7Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs7Idx,				E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs7Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs7Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs7Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs7Dwell,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs7Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs8Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs8Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs8Idx,				E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs8Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs8Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs8Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs8Dwell,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs8Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs9Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs9Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs9Idx,				E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs9Idx"				,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs9Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs9Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs9Dwell,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs9Dwell"			,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs10Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs10Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs10Idx,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs10Idx"			,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs10Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs10Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs10Dwell,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs10Dwell"		,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs11Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs11Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs11Idx,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs11Idx"			,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs11Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs11Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs11Dwell,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs11Dwell"		,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs12Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs12Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs12Idx,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs12Idx"			,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs12Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs12Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs12Dwell,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs12Dwell"		,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs13Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs13Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs13Idx,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs13Idx"			,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs13Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs13Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs13Dwell,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs13Dwell"		,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs14Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs14Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs14Idx,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs14Idx"			,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs14Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs14Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs14Dwell,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs14Dwell"		,	L"__u8[16] :: event preset dwell time"	},
	{	E_GrP3CfgOfsEprs15Type,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs15Type"			,	L"__u8[16] :: event type, 0 = none 1 = motion 2 = sensor"	},
	{	E_GrP3CfgOfsEprs15Idx,			E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs15Idx"			,	L"__u8[16] :: event index, motion = channel , sensor = pin number"	},
	{	E_GrP3CfgOfsEprs15Preset,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs15Preset"		,	L"__u8[16] :: event preset number"	},
	{	E_GrP3CfgOfsEprs15Dwell,		E_GrP3DvlParaTypeByte,	16,	16,	L"Eprs15Dwell"		,	L"__u8[16] :: event preset dwell time"	},

	{	E_GrP3CfgOfsSysTmSvrAddr,		E_GrP3DvlParaTypeNone,	1,	128,	L"SysTmSvrAddr"		,	L"WCHAR[64]	::	time server address"	},

	{	E_GrP3CfgOfsRecRes,					E_GrP3DvlParaTypeByte,	16,	16,	L"RecRes"					,	L"__u8[16]	::	record resolution"	},
	{	E_GrP3CfgOfsRecFps,					E_GrP3DvlParaTypeByte,	16,	16,	L"RecFps"					,	L"__u8[16]	::	record fps value"	},
	{	E_GrP3CfgOfsRecQua,					E_GrP3DvlParaTypeByte,	16,	16,	L"RecQua"					,	L"__u8[16]	::	record frame quality"	},
	{	E_GrP3CfgOfsRecRuFps,				E_GrP3DvlParaTypeByte,	16,	16,	L"RecRuFps"				,	L"__u8[16]	::	record ram up fps value"	},
	{	E_GrP3CfgOfsRecRuQua,				E_GrP3DvlParaTypeByte,	16,	16,	L"RecRuQua"				,	L"__u8[16]	::	record ram up frame quality"	},
	{	E_GrP3CfgOfsRecRuRes,				E_GrP3DvlParaTypeByte,	16,	16,	L"RecRuRes"				,	L"__u8[16]	::	record ram up resolution"	},
	{	E_GrP3CfgOfsNetFps,					E_GrP3DvlParaTypeByte,	16,	16,	L"NetFps"					,	L"__u8[16]	::	network fps value"	},
	{	E_GrP3CfgOfsNetQua,					E_GrP3DvlParaTypeByte,	16,	16,	L"NetQua"					,	L"__u8[16]	::	network frame quality"	},
	{	E_GrP3CfgOfsNetRes,					E_GrP3DvlParaTypeByte,	16,	16,	L"NetRes"					,	L"__u8[16]	::	network resolution"	},
	{	E_GrP3CfgOfsNetChMap,				E_GrP3DvlParaTypeBitmap0,	2,	2,	L"NetChMap"				,	L"__u16	::	network channel map"	},

	{	E_GrP3CfgOfsDisSeqCh,				E_GrP3DvlParaTypeByte,	16,	16,	L"DisSeqCh"				,	L"__u8[16] :: auto sequence split 1 channel time"	},
	{	E_GrP3CfgOfsDisSeqSplit4,		E_GrP3DvlParaTypeByte,	1,	1,	L"DisSeqSplit4"		,	L"__u8 :: auto sequence split 4"	},
	{	E_GrP3CfgOfsDisSeqSplit6,		E_GrP3DvlParaTypeByte,	1,	1,	L"DisSeqSplit6"		,	L"__u8 :: auto sequence split 6"	},
	{	E_GrP3CfgOfsDisSeqSplit8,		E_GrP3DvlParaTypeByte,	1,	1,	L"DisSeqSplit8"		,	L"__u8 :: auto sequence split 8"	},
	{	E_GrP3CfgOfsDisSeqSplit9,		E_GrP3DvlParaTypeByte,	1,	1,	L"DisSeqSplit9"		,	L"__u8 :: auto sequence split 9"	},
	{	E_GrP3CfgOfsDisSeqSplit10,	E_GrP3DvlParaTypeByte,	1,	1,	L"DisSeqSplit10"	,	L"__u8 :: auto sequence split 10"	},
	{	E_GrP3CfgOfsDisSeqSplit13,	E_GrP3DvlParaTypeByte,	1,	1,	L"DisSeqSplit13"	,	L"__u8 :: auto sequence split 13"	},
	{	E_GrP3CfgOfsDisSeqLoss,			E_GrP3DvlParaTypeBool8,	1,	1,	L"DisSeqLoss"			,	L"BOOL8 :: video loss channel skip"	},
	{	E_GrP3CfgOfsDisSplitMode,		E_GrP3DvlParaTypeByte,	1,	1,	L"DisSplitMode"		,	L"__u8 :: split mode"	},

	{	E_GrP3CfgOfsSpotChTime,			E_GrP3DvlParaTypeByte,	16,	16,	L"SpotChTime"	,			L"__u8[16] :: spot channel time"	},
	{	E_GrP3CfgOfsSpotGroupTime,	E_GrP3DvlParaTypeByte,	4,	4,	L"SpotGroupTime"	,	L"__u8[4] :: spot quad 4 dwell time"	},
	{	E_GrP3CfgOfsSpotQuadMode,		E_GrP3DvlParaTypeByte,	1,	1,	L"SpotQuadMode"		,	L"__u8 :: spot quad mode"	},
	{	E_GrP3CfgOfsSpotEvtMode,		E_GrP3DvlParaTypeByte,	1,	1,	L"SpotEvtMode"		,	L"__u8 :: spot event mode"	},
	{	E_GrP3CfgOfsSpotFixCh,			E_GrP3DvlParaTypeByte,	1,	1,	L"SpotFixCh"			,	L"__u8 :: spot fix channel"	},

	{	E_GrP3CfgOfsDoMtnOutTimes,	E_GrP3DvlParaTypeNone,	1,	256,	L"DoMtnOutTimes"	,	L"__u8[16][16]	:: alarm motion alram time - sec unit"	},
	{	E_GrP3CfgOfsDoSenOutTimes,	E_GrP3DvlParaTypeNone,	1,	256,	L"DoSenOutTimes"	,	L"__u8[16][16]	:: alarm sensor alram time - sec unit"	},
	{	E_GrP3CfgOfsDoVdlOutTimes,	E_GrP3DvlParaTypeNone,	1,	256,	L"DoVdlOutTimes"	,	L"__u8[16][16]	:: alarm video loss alram time - sec unit"	},
	{	E_GrP3CfgOfsDoHddErrUseMap,	E_GrP3DvlParaTypeBitmap0,	2,	2,		L"DoHddErrUseMap"	,	L"__u16					:: hdd error alram use alarm channel map"	},
	{	E_GrP3CfgOfsDoHddErrDur,		E_GrP3DvlParaTypeByte,	16,	16,		L"DoHddErrDur"	,		L"__u8[16]			:: hdd error alram duration"	},

	{	E_GrP3CfgOfsUartSpeed,			E_GrP3DvlParaTypeByte,	4,	4,	L"UartSpeed"	,	L"__u8[4] :	UART port speed"	},
	{	E_GrP3CfgOfsUartDataBit,		E_GrP3DvlParaTypeByte,	4,	4,	L"UartDataBit"	,	L"__u8[4] :	data bit size, 0=8bit, 1=7bit"	},
	{	E_GrP3CfgOfsUartStopBit,		E_GrP3DvlParaTypeByte,	4,	4,	L"UartStopBit"	,	L"__u8[4] :	stop bit, 0=1bit, 1=1.5bit, 2=2bit"	},
	{	E_GrP3CfgOfsUartParity,			E_GrP3DvlParaTypeByte,	4,	4,	L"UartParityBit"	,	L"__u8[4] :	parity bit, 0= none, 1=even, 2=odd"	},

	{	E_GrP3CfgOfsNetDdnsAdr,			E_GrP3DvlParaTypeNone,	1,	128,	L"DdnsAddr"	,	L"WCHAR[64] : DDNS server Address"	},

	{	E_GrP3CfgOfsNetFdnsType,		E_GrP3DvlParaTypeByte,	1,	1,	L"FdnsServer"	,	L"__u8 : free DNS server 0=none,1=DynDns,2=no-ip"	},
	{	E_GrP3CfgOfsNetFdnsUrl,			E_GrP3DvlParaTypeNone,	1,	128,	L"FdnsUrl"	,	L"WCHAR[64] : free DNS URL"	},
	{	E_GrP3CfgOfsNetFdnsId,			E_GrP3DvlParaTypeNone,	1,	128,	L"FdnsId"	,	L"WCHAR[64] : free DNS id"	},
	{	E_GrP3CfgOfsNetFdnsPass,		E_GrP3DvlParaTypeNone,	1,	64,	L"FdnsPass"	,	L"WCHAR[32] : free DNS password"	},

	{	E_GrP3CfgOfsMtnMap,					E_GrP3DvlParaTypeNone,	1,	2048,	L"MtnMap"	,		L"__u32[16][32] : motion sense map"	},
	{	E_GrP3CfgOfsMtnSensi,				E_GrP3DvlParaTypeNone,	1,	384,	L"MtnSensi"	,	L"__u8[16][24] : motion sensitivity schedule table"	},

	{	E_GrP3CfgOfsEvtPpEvt,				E_GrP3DvlParaTypeByte,	16,	16,	L"EvtPpEvt"	,	L"__u8[16] : event pop up event mask"	},
	{	E_GrP3CfgOfsEvtPpDur,				E_GrP3DvlParaTypeByte,	16,	16,	L"EvtPpDur",	L"__u8[16] : event pop up duration"	},

	{	E_GrP3CfgOfsPtzType,				E_GrP3DvlParaTypeByte,	16,	16,	L"PtzModel"	,	L"__u8[16] : PTZ model"	},
	{	E_GrP3CfgOfsPtzId,					E_GrP3DvlParaTypeByte,	16,	16,	L"PtzId"	,	L"__u8[16] : PTZ ID"	},
	{	E_GrP3CfgOfsPtzPort,				E_GrP3DvlParaTypeByte,	16,	16,	L"PtzUart"	,	L"__u8[16] : PTZ UART port index"	},
	{	E_GrP3CfgOfsPtzBaud,				E_GrP3DvlParaTypeByte,	16,	16,	L"PtzBaud"	,	L"__u8[16] : PTZ UART baud rate"	},
	{	E_GrP3CfgOfsPtzDataBit,			E_GrP3DvlParaTypeByte,	16,	16,	L"PtzDataBit"	,	L"__u8[16] : PTZ UART data bit"	},
	{	E_GrP3CfgOfsPtzStopBit,			E_GrP3DvlParaTypeByte,	16,	16,	L"PtzStopBit"	,	L"__u8[16] : PTZ UART stop bit"	},
	{	E_GrP3CfgOfsPtzParity,			E_GrP3DvlParaTypeByte,	16,	16,	L"PtzParity"	,	L"__u8[16] : PTZ UART parity bit"	},
	{	E_GrP3CfgOfsPtzAtb,					E_GrP3DvlParaTypeByte,	16,	16,	L"PtzAtb"	,	L"__u8[16] : PTZ attribute bit0=pan reverse, bit1=tilt reverse"	},
	{	E_GrP3CfgOfsPtzTourType,		E_GrP3DvlParaTypeByte,	16,	16,	L"PtzTourType"	,	L"__u8[16] : PTZ tour type 0=DVR, 1=CAMERA"	},
	{	E_GrP3CfgOfsPtzSpdPan,			E_GrP3DvlParaTypeByte,	16,	16,	L"PtzSpdPan"	,	L"__u8[16] : PTZ pan speed"	},
	{	E_GrP3CfgOfsPtzSpdTilt,			E_GrP3DvlParaTypeByte,	16,	16,	L"PtzSpdTilt"	,	L"__u8[16] : PTZ tilt speed"	},
	{	E_GrP3CfgOfsPtzSpdZoom,			E_GrP3DvlParaTypeByte,	16,	16,	L"PtzSpdZoom"	,	L"__u8[16] : PTZ zoom speed"	},
	{	E_GrP3CfgOfsPtzSpdFocus,		E_GrP3DvlParaTypeByte,	16,	16,	L"PtzSpdFocus"	,	L"__u8[16] : PTZ focus speed"	},
	{	E_GrP3CfgOfsPtzTourCfg,			E_GrP3DvlParaTypeNone,	1,	768,	L"PtzTourCfg"	,	L"__u32[16][12] : PTZ tour-refer document"	},

	{	E_GrP3CfgOfsNetSmtpAdr,			E_GrP3DvlParaTypeNone,	1,	128,	L"SmtpAdr"	,	L"WCHAR[64] : email server address"	},
	{	E_GrP3CfgOfsNetSmtpPort,		E_GrP3DvlParaTypeWord,	1,	2,	L"SmtpPort"	,	L"__u16 : email server port"	},
	{	E_GrP3CfgOfsSmtpId,					E_GrP3DvlParaTypeNone,	1,	128,	L"SmtpId"	,		L"WCHAR[64] : SMTP server id"	},
	{	E_GrP3CfgOfsSmtpPass,				E_GrP3DvlParaTypeNone,	1,	64,		L"SmtpPass"	,	L"WCHAR[32] : SMTP server password"	},
	{	E_GrP3CfgOfsNetEmailTitle,	E_GrP3DvlParaTypeNone,	1,	128,	L"EmailTitle"	,	L"WCHAR[64] : email title"	},
	{	E_GrP3CfgOfsEmailSender,		E_GrP3DvlParaTypeNone,	1,	128,	L"EmailSender"	,	L"WCHAR[64] : email sender"	},
	{	E_GrP3CfgOfsEmailTo1,				E_GrP3DvlParaTypeNone,	1,	128,	L"EmailTo1"	,	L"WCHAR[64] : email to address 1"	},
	{	E_GrP3CfgOfsEmailTo2,				E_GrP3DvlParaTypeNone,	1,	128,	L"EmailTo2"	,	L"WCHAR[64] : email to address 2"	},
	{	E_GrP3CfgOfsEmailTo3,				E_GrP3DvlParaTypeNone,	1,	128,	L"EmailTo3"	,	L"WCHAR[64] : email to address 3"	},

	{	E_GrP3CfgOfsNetEmailEvt,		E_GrP3DvlParaTypeWord,	1,	2,	L"EmailEvt"	,	L"__u16 : email notify event - event log mask"	},
	{	E_GrP3CfgOfsNetEmailInter,	E_GrP3DvlParaTypeByte,	1,	1,	L"EmailInter"	,	L"__u8 : email interval - sec table index"	},
	{	E_GrP3CfgOfsNetSmtpSecu,		E_GrP3DvlParaTypeByte,	1,	1,	L"SmtpSecu"	,	L"__u8 : email security type"	},

	{	E_GrP3CfgOfsCtrlModel,			E_GrP3DvlParaTypeByte,	1,	1,		L"CtrlModel"	,	L"__u8 : controller model"	},
	{	E_GrP3CfgOfsCtrlId,					E_GrP3DvlParaTypeByte,	1,	1,		L"CtrlId"			,	L"__u8 : controller ID"	},
	{	E_GrP3CfgOfsCtrlUart,				E_GrP3DvlParaTypeByte,	1,	1,		L"CtrlUart"		,	L"__u8 : controller UART port"	},

	{	E_GrP3CfgOfsBlkageMap,			E_GrP3DvlParaTypeAsmStpAdr,	1,	1,	L"BlockageMap"	,	L"__u16 : Blockage Cfg Map address"	},
	{	E_GrP3CfgOfsBlkageVisi,			E_GrP3DvlParaTypeBool8,	1,	1,		L"BlockageVisi"		,	L"BOOL8 : Blockage visible"	},

	{	E_GrP3CfgOfsPushEnable,			E_GrP3DvlParaTypeBool8,	1,	1,		L"PushEnable"			,	L"BOOL8 : push-server Enable"	},
	{	E_GrP3CfgOfsPushSvrIpv4,		E_GrP3DvlParaTypeByte,	4,	4,		L"PushSvrIpV4"		,	L"__u8[4] : pushpush-server ip v4"	},
	{	E_GrP3CfgOfsPushSvrIpv6,		E_GrP3DvlParaTypeWord,	2,	16,		L"PushSvrIpV6"		,	L"__u16[8] : push-server ip v6"	},
	{	E_GrP3CfgOfsPushSvrPort,		E_GrP3DvlParaTypeWord,	2,	2,		L"PushSvrPort"		,	L"__u16 : push-server network port"	},
	{	E_GrP3CfgOfsPushDvrTitle,		E_GrP3DvlParaTypeWhcar,	1,	32,		L"PushTitle"			,	L"WCHAR[6] : push-server dvr title"	},
	{	E_GrP3CfgOfsPushIsDvrEvt,		E_GrP3DvlParaTypeBool8,	1,	1,		L"PushIsDvrEvt"		,	L"BOOL8 : push-server dvr event"	},

	{	E_GrP3CfgOfsPushEvtTime,		E_GrP3DvlParaTypeByte,	1,	1,		L"PushEvtTime"		,	L"__u8 : push-server event duration time"	},
	{	E_GrP3CfgOfsPushEvt,				E_GrP3DvlParaTypeByte,	1,	1,		L"PushEvt"				,	L"__u8 : push-server event"	},
	{	E_GrP3CfgOfsPushHoly1Evt,		E_GrP3DvlParaTypeByte,	1,	1,		L"PushHolyEvt1"		,	L"__u8 : push-server holyday1 event"	},
	{	E_GrP3CfgOfsPushHoly2Evt,		E_GrP3DvlParaTypeByte,	1,	1,		L"PushHolyEvt2"		,	L"__u8 : push-server holyday2 event"	},
	{	E_GrP3CfgOfsPushHoly3Evt,		E_GrP3DvlParaTypeByte,	1,	1,		L"PushHolyEvt3"		,	L"__u8 : push-server holyday3 event"	},
	{	E_GrP3CfgOfsPushHoly4Evt,		E_GrP3DvlParaTypeByte,	1,	1,		L"PushHolyEvt4"		,	L"__u8 : push-server holyday4 event"	},

	{	E_GrP3CfgOfsPushScd,				E_GrP3DvlParaTypeDword,	4,	4,		L"PushSchedule"		,	L"__u32 : push-server schedule"	},
	{	E_GrP3CfgOfsPushScdHoly1,		E_GrP3DvlParaTypeDword,	4,	4,		L"PushHolySchedule1"	,	L"__u32 : push-server holyday1 schedule"	},
	{	E_GrP3CfgOfsPushScdHoly2,		E_GrP3DvlParaTypeDword,	4,	4,		L"PushHolySchedule2"	,	L"__u32 : push-server holyday2 schedule"	},
	{	E_GrP3CfgOfsPushScdHoly3,		E_GrP3DvlParaTypeDword,	4,	4,		L"PushHolySchedule3"	,	L"__u32 : push-server holyday3 schedule"	},
	{	E_GrP3CfgOfsPushScdHoly4,		E_GrP3DvlParaTypeDword,	4,	4,		L"PushHolySchedule4"	,	L"__u32 : push-server holyday4 schedule"	},

	{	E_GrP3CfgOfsPushSvrUrl,			E_GrP3DvlParaTypeNone,	1,	128,	L"PushServerURL"	,	L"WCHAR[64] : push-server url address"	},
};

//====================================================================
//--------------------------------------------------------------------
__u32	GrP3DvlParaTypeUnitSizeGet( __u8 A_Type )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		switch ( A_Type )
		{
			case E_GrP3DvlParaTypeChar:
			case E_GrP3DvlParaTypeByte:
			case E_GrP3DvlParaTypeBool8:
			case E_GrP3DvlParaTypeStdValType:
			case E_GrP3DvlParaTypeAsmOpType:
			case E_GrP3DvlParaTypeAsmFlagIdx:
			case E_GrP3DvlParaTypeAsmArgType:
			case E_GrP3DvlParaTypeAsmIregIdx:
			case E_GrP3DvlParaTypeAsmCmpType:
			case E_GrP3DvlParaTypeAsmFlagSet:
			case E_GrP3DvlParaTypeAsmCalc:
			case E_GrP3DvlParaTypeAsmNotSet:
			case E_GrP3DvlParaTypeAsmJumpCall:
			case E_GrP3DvlParaTypeAsmJumpDir:
			case E_GrP3DvlParaTypeAsmTregIdx:
			case E_GrP3DvlParaTypeConIdx:
			case E_GrP3DvlParaTypePosBssLt:
			case E_GrP3DvlParaTypePosBssUp:
			case E_GrP3DvlParaTypePosBssRt:
			case E_GrP3DvlParaTypePosBssDn:
			case E_GrP3DvlParaTypeAsmMemType:
			case E_GrP3DvlParaTypeKeyEvtType:
			case E_GrP3DvlParaTypeKeyCode:
			case E_GrP3DvlParaTypeUsvalType:
			case E_GrP3DvlParaTypeRelUpi:
				Tv_Result	=	1;
				break;
			case E_GrP3DvlParaTypeShort:
			case E_GrP3DvlParaTypeWord:
			case E_GrP3DvlParaTypeWhcar:
			case E_GrP3DvlParaTypeScptIdAny:
			case E_GrP3DvlParaTypePntCode:
			case E_GrP3DvlParaTypeAsmCode:
			case E_GrP3DvlParaTypeVarAdr:
			case E_GrP3DvlParaTypeAsmGvsAdr:
			case E_GrP3DvlParaTypeAsmGvuAdr:
			case E_GrP3DvlParaTypeAsmStpAdr:
			case E_GrP3DvlParaTypeUpiCmd:
			case E_GrP3DvlParaTypeApiCmd:
			case E_GrP3DvlParaTypeTxtId:
			case E_GrP3DvlParaTypeScptIdWin:
				Tv_Result	=	2;
				break;
			case E_GrP3DvlParaTypeInt:
			case E_GrP3DvlParaTypeDword:
			case E_GrP3DvlParaTypeColor:
				Tv_Result	=	4;
				break;

			default:
				Tv_Result	=	0;
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
