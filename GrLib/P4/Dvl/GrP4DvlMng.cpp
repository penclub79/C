/*
		platform develop manager

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrDebug.h>
#include	<P4/Dvl/GrP4DvlMng.h>
#include	<GrDumyTool.h>
#include	<P4/GrP4DoLbl.h>
#include	<GrStrTool.h>
#include	<GrDrawTool.h>
#include	<GrFileTool.h>
#include	<GrFileCtrl.h>
#include	<P4/GrP4DoPrgsBarLoc.h>
#include	<P4/GrP4DoQuad.h>
#include <P4/GrP4ApiBase.h>
#include <P4/GrP4Bios.h>
#include <P4/GrP4Cfg.h>

//====================================================================
//local const

#define E_GrP4DvlMngRatbTime			30

//====================================================================
//local type

typedef	struct	St_GrP4DvlTxtNameItm
{
	__u16	Id;
	WCHAR*	StrName;
}	*Ptr_GrP4DvlTxtNameItm;

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrP4Mng*	V_GrP4Mng;

extern	WCHAR*	V_GrP4DvlClassNameTbl[E_GrP4DoClassCnt];

extern	St_GrP4PrjStpInfo	V_GrP4DvlStpSysTbl[E_GrP4DvlSetupSysCnt];

St_GrP4DvlTxtNameItm	V_GrP4DvlTxtSysName[E_GrP4DvlTextSysCnt]	=
{
	{ E_GrP4ScptTxtIdLang0, L"(1) Language 0" },
	{ E_GrP4ScptTxtIdLang1, L"(2) Language 1" },
	{ E_GrP4ScptTxtIdLang2, L"(3) Language 2" },
	{ E_GrP4ScptTxtIdLang3, L"(4) Language 3" },
	{ E_GrP4ScptTxtIdLang4, L"(5) Language 4" },
	{ E_GrP4ScptTxtIdLang5, L"(6) Language 5" },
	{ E_GrP4ScptTxtIdLang6, L"(7) Language 6" },
	{ E_GrP4ScptTxtIdLang7, L"(8) Language 7" },
	{ E_GrP4ScptTxtIdLang8, L"(9) Language 8" },
	{ E_GrP4ScptTxtIdLang9, L"(10) Language 9" },
	{ E_GrP4ScptTxtIdLang10, L"(11) Language 10" },
	{ E_GrP4ScptTxtIdLang11, L"(12) Language 11" },
	{ E_GrP4ScptTxtIdLang12, L"(13) Language 12" },
	{ E_GrP4ScptTxtIdLang13, L"(14) Language 13" },
	{ E_GrP4ScptTxtIdLang14, L"(15) Language 14" },
	{ E_GrP4ScptTxtIdLang15, L"(16) Language 15" },
	{ E_GrP4ScptTxtIdLang16, L"(17) Language 16" },
	{ E_GrP4ScptTxtIdLang17, L"(18) Language 17" },
	{ E_GrP4ScptTxtIdLang18, L"(19) Language 18" },
	{ E_GrP4ScptTxtIdLang19, L"(20) Language 19" },
	{ E_GrP4ScptTxtIdLang20, L"(21) Language 20" },
	{ E_GrP4ScptTxtIdLang21, L"(22) Language 21" },
	{ E_GrP4ScptTxtIdLang22, L"(23) Language 22" },
	{ E_GrP4ScptTxtIdLang23, L"(24) Language 23" },
	{ E_GrP4ScptTxtIdLang24, L"(25) Language 24" },
	{ E_GrP4ScptTxtIdLang25, L"(26) Language 25" },
	{ E_GrP4ScptTxtIdLang26, L"(27) Language 26" },
	{ E_GrP4ScptTxtIdLang27, L"(28) Language 27" },
	{ E_GrP4ScptTxtIdLang28, L"(29) Language 28" },
	{ E_GrP4ScptTxtIdLang29, L"(30) Language 29" },
	{ E_GrP4ScptTxtIdLang30, L"(31) Language 30" },
	{ E_GrP4ScptTxtIdLang31, L"(32) Language 31" },

	{	E_GrP4ScptTxtIdUnitYear,	L"(36) Unit - Year"	},
	{	E_GrP4ScptTxtIdUnitMonth,	L"(37) Unit - Month"	},
	{	E_GrP4ScptTxtIdUnitDay,	L"(38) Unit - Day"	},
	{	E_GrP4ScptTxtIdUnitHour,	L"(39) Unit - Hour"	},
	{	E_GrP4ScptTxtIdUnitMin,	L"(40) Unit - Minute"	},
	{	E_GrP4ScptTxtIdUnitSec,	L"(41) Unit - Sec"	},
	{	E_GrP4ScptTxtIdTimeAm,	L"(42) Time - AM"	},
	{	E_GrP4ScptTxtIdTimePm,	L"(43) Time - PM"	},
	{	E_GrP4ScptTxtIdChannel,	L"(51) Channel"	},
	{	E_GrP4ScptTxtIdDetected,	L"(52) Detected"	},
	{	E_GrP4ScptTxtIdChanged,	L"(53) Changed"	},
	{	E_GrP4ScptTxtIdStart,	L"(54) Start"	},
	{	E_GrP4ScptTxtIdEnd,	L"(55) End"	},
	{	E_GrP4ScptTxtIdResolution,	L"(56) Resolution"	},
	{	E_GrP4ScptTxtIdRecord,	L"(57) Record"	},
	{	E_GrP4ScptTxtIdRecQuility,	L"(58) Quality"	},
	{	E_GrP4ScptTxtIdRecSpeed,	L"(59) Record Speed"	},
	{	E_GrP4ScptTxtIdRecPost,	L"(60) POST-FRAME"	},
	{	E_GrP4ScptTxtIdRecPre,		L"(61 PRE-FRAME"	},
	{	E_GrP4ScptTxtIdOn,		L"(62) On"	},
	{	E_GrP4ScptTxtIdOff,		L"(63) Off"	},
	{	E_GrP4ScptTxtIdMotionMap,		L"(64) Motion Area"	},
	{	E_GrP4ScptTxtIdRecSchd,		L"(65) Record schedule"	},
	{	E_GrP4ScptTxtIdPassword,		L"(66) Password"	},
	{	E_GrP4ScptTxtIdCamera,		L"(67) Camera"	},
	{	E_GrP4ScptTxtIdTitle,		L"(68) Title"	},
	{	E_GrP4ScptTxtIdNetwork,		L"(69) Network"	},
	{	E_GrP4ScptTxtIdDisconnect,		L"(70) Disconnect"	},
	{	E_GrP4ScptTxtIdConnect,		L"(71) Connect"	},
	{	E_GrP4ScptTxtIdSearch,		L"(72) Search"	},
	{	E_GrP4ScptTxtIdBackup,		L"(73) Backup"	},
	{	E_GrP4ScptTxtIdMotion,		L"(74) Motion"	},
	{	E_GrP4ScptTxtIdSensor,		L"(75) Sensor"	},
	{	E_GrP4ScptTxtIdEvtExt1,		L"(76) Event Extension 1"	},
	{	E_GrP4ScptTxtIdEvtExt2,		L"(77) Event Extension 2"	},
	{	E_GrP4ScptTxtIdEvtExt3,		L"(78) Event Extension 3"	},
	{	E_GrP4ScptTxtIdEvtExt4,		L"(79) Event Extension 4"	},
	{	E_GrP4ScptTxtIdEvtExt5,		L"(80) Event Extension 5"	},
	{	E_GrP4ScptTxtIdVdoLoss,		L"(81) Video loss"	},
	{	E_GrP4ScptTxtIdVdoRcvy,		L"(82) Video Recovery"	},
	{	E_GrP4ScptTxtIdPowerOn,		L"(83) Power on"	},
	{	E_GrP4ScptTxtIdPowerOff,		L"(84) Power off"	},
	{	E_GrP4ScptTxtIdLogin,		L"(85) Login"	},
	{	E_GrP4ScptTxtIdLogout,		L"(86) Logout"	},
	{	E_GrP4ScptTxtIdSetupChg,		L"(87) Setup changed"	},
	{	E_GrP4ScptTxtIdHddErr,		L"(88) HDD error"	},
	{	E_GrP4ScptTxtIdSetupIn,		L"(89) Setup menu in"	},
	{	E_GrP4ScptTxtIdSetupOut,		L"(90) Setup menu out"	},
	{	E_GrP4ScptTxtIdHddFormat,		L"(91) HDD formatted"	},
	{	E_GrP4ScptTxtIdRamup,		L"(92) RAMUP"	},
	{	E_GrP4ScptTxtIdEmergency,		L"(93) Emergency"	},
	{	E_GrP4ScptTxtIdMacaddress,		L"(94) Mac address"	},
	{	E_GrP4ScptTxtIdIp,		L"(95) IP"	},
	{	E_GrP4ScptTxtIdPort,		L"(96) Port"	},
	{	E_GrP4ScptTxtIdUse,		L"(97) Use"	},
	{	E_GrP4ScptTxtIdSensitivity,		L"(98) Sensitivity"	},
	{	E_GrP4ScptTxtIdWebcode,		L"(99) WEB-CODE"	},
	{	E_GrP4ScptTxtIdFwVer,		L"(100) Firmware Version"	},
	{	E_GrP4ScptTxtIdNwVer,		L"(101) Network Version"	},
	{	E_GrP4ScptTxtIdModelNum,	L"(102) Model number"	},

	{	E_GrP4ScptTxtIdFileSystem,	L"(103) file system"	},
	{	E_GrP4ScptTxtIdFirmWare,	L"(104) firmware"	},
	{	E_GrP4ScptTxtIdUpgrade,		L"(105) upgrade"	},
	{ E_GrP4ScptTxtIdRtcErr, L"(106) RTC error" },

	{ E_GrP4ScptTxtIdMonth1, L"(107) Month 1" },
	{ E_GrP4ScptTxtIdMonth2, L"(108) Month 2" },
	{ E_GrP4ScptTxtIdMonth3, L"(109) Month 3" },
	{ E_GrP4ScptTxtIdMonth4, L"(110) Month 4" },
	{ E_GrP4ScptTxtIdMonth5, L"(111) Month 5" },
	{ E_GrP4ScptTxtIdMonth6, L"(112) Month 6" },
	{ E_GrP4ScptTxtIdMonth7, L"(113) Month 7" },
	{ E_GrP4ScptTxtIdMonth8, L"(114) Month 8" },
	{ E_GrP4ScptTxtIdMonth9, L"(115) Month 9" },
	{ E_GrP4ScptTxtIdMonth10, L"(116) Month 10" },
	{ E_GrP4ScptTxtIdMonth11, L"(117) Month 11" },
	{ E_GrP4ScptTxtIdMonth12, L"(118) Month 12" },
	{ E_GrP4ScptTxtIdWeekSun, L"(119) Sunday" },
	{ E_GrP4ScptTxtIdWeekMon, L"(120) Monday" },
	{ E_GrP4ScptTxtIdWeekTue, L"(121) Tuesday" },
	{ E_GrP4ScptTxtIdWeekWed, L"(122) Wednesday" },
	{ E_GrP4ScptTxtIdWeekThu, L"(123) Thursday" },
	{ E_GrP4ScptTxtIdWeekFri, L"(124) Friday" },
	{ E_GrP4ScptTxtIdWeekSat, L"(125) Saturday" },

	{ E_GrP4ScptTxtIdHddSysChg, L"(126) Hdd system change" },
	{ E_GrP4ScptTxtIdHdd, L"(127) Hard disk" },

	{	E_GrP4ScptTxtIdNone,	L""	},

};

St_GrP4DvlDoPrInfoData	V_GrP4DvlMdlDbgClstTbl[E_GrP4DvlMdlDbgClstCnt]	=	
{
	{	L"VdoMaxCh",		TRUE,		E_GrP4DvlParaTypeByte,			1,	(void*)&((Ptr_GrP4PrjDbgInfo)0)->VdoMaxCh,	L"Video Channel count"	},
	{	L"AdoMaxCh",		TRUE,		E_GrP4DvlParaTypeByte,			1,	(void*)&((Ptr_GrP4PrjDbgInfo)0)->AdoMaxCh,	L"Audio Channel Count"	},
	{	L"DiChCnt",			TRUE,		E_GrP4DvlParaTypeByte,			1,	(void*)&((Ptr_GrP4PrjDbgInfo)0)->DiChCnt,		L"Sensor count"	},
	{	L"DoChCnt",			TRUE,		E_GrP4DvlParaTypeByte,			1,	(void*)&((Ptr_GrP4PrjDbgInfo)0)->DoChCnt,		L"Alarm count"	},
	{	L"HwVer",				TRUE,		E_GrP4DvlParaTypeDword,			1,	(void*)&((Ptr_GrP4PrjDbgInfo)0)->HwVer,			L"Hardware Version"	},
	{	L"NetVer",			TRUE,		E_GrP4DvlParaTypeDword,			1,	(void*)&((Ptr_GrP4PrjDbgInfo)0)->NetVer,		L"Network Version"	},
	{	L"DipSw",				TRUE,		E_GrP4DvlParaTypeBitmap0,		1,	(void*)&((Ptr_GrP4PrjDbgInfo)0)->DipSw,			L"Dip switch"	},
};

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DvlMng::Cls_GrP4DvlMng( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrP4Mng( A_TaskSvr )
{
	// local -------------------
		__u32	Tv_TmMili;
		BOOL8	Tv_IsSm;
	// code --------------------
		m_DvlMsgAloc	=	GrTaskSvrMsgCmdAlloc();

		// init
		GrDumyZeroMem( &m_DvlPrjConfig, sizeof(m_DvlPrjConfig) );
		GrDumyZeroMem( &m_DvlPrjRtl, sizeof(m_DvlPrjRtl) );
		GrDumyZeroMem( &m_DvlPrjModel, sizeof(m_DvlPrjModel) );
		GrDumyZeroMem( &m_DvlPrjSkin, sizeof(m_DvlPrjSkin) );
		GrDumyZeroMem( &m_DvlPrjMdlAtb, sizeof(m_DvlPrjMdlAtb) );

		// script
		GrDumyZeroMem( &m_DvlScptCfgPrj, sizeof(m_DvlScptCfgPrj) );
		GrDumyZeroMem( &m_DvlScptCfgRtl, sizeof(m_DvlScptCfgRtl) );

		GrDumyZeroMem( m_DvlScptWinTbl, sizeof(m_DvlScptWinTbl) );
		GrDumyZeroMem( m_DvlScptPageTbl, sizeof(m_DvlScptPageTbl) );
		GrDumyZeroMem( m_DvlScptPstTbl, sizeof(m_DvlScptPstTbl) );
		GrDumyZeroMem( m_DvlFoldTbl, sizeof(m_DvlFoldTbl) );		// project folder
		GrDumyZeroMem( m_DvlScptImgTbl, sizeof(m_DvlScptImgTbl) );
		GrDumyZeroMem( m_DvlScptTxtTbl, sizeof(m_DvlScptTxtTbl) );
		GrDumyZeroMem( m_DvlPstInfo, sizeof(m_DvlPstInfo) );
		GrDumyZeroMem( m_DvlScptFontTbl, sizeof(m_DvlScptFontTbl) );
		GrDumyZeroMem( m_DvlStpUsrTbl, sizeof(m_DvlStpUsrTbl) );
		//GrDumyZeroMem( m_DvlStpSysTbl, sizeof(m_DvlStpSysTbl) );
		GrDumyZeroMem( m_DvlGvuTbl, sizeof(m_DvlGvuTbl) );
		GrDumyZeroMem( m_DvlUrgstTbl, sizeof(m_DvlUrgstTbl) );
		GrDumyZeroMem( m_DvlScptAsmTbl, sizeof(m_DvlScptAsmTbl) );
		GrDumyZeroMem( m_DvlAsmInfo, sizeof(m_DvlAsmInfo) );
		GrDumyZeroMem(m_DvlAsmFncCmt, sizeof(m_DvlAsmFncCmt));
		GrDumyZeroMem(m_DvlAsmRmkTbl, sizeof(m_DvlAsmRmkTbl));
		//GrDumyZeroMem( &m_DvlSetBase, sizeof(m_DvlSetBase) );
		GrDumyZeroMem( m_DvlMdlDblTbl, sizeof(m_DvlMdlDblTbl) );

		GrDumyZeroMem( m_DvlBldInfo, sizeof(m_DvlBldInfo) );
		GrDumyZeroMem( m_DvlEdtIdDispTbl, sizeof(m_DvlEdtIdDispTbl) );

		m_DvlIsPrjExist	=	FALSE;

		m_DvlTickId				=	0;

		m_DvlEdtIdDispCnt	=	0;

		m_DvlStpUsrCnt		=	0;

		m_DvlEdtLump			=	NULL;

		m_DvlFc						=	NULL;

		m_DvlPtrStpFl			=	NULL;

		m_DvlDbgMode			=	E_GrP4DvlDbgModeStop;
		m_DvlDbgIsResume	=	FALSE;

		m_DvlRatbTimer		=	0;
		m_DvlLanCblTimer	=	0;
		m_DvlUsbTimer			=	0;
		m_DvlCmsTimer			=	0;

		m_IsDvlBkupErr		=	FALSE;
		m_IsDvlBkupRun		=	FALSE;

		m_IsDvlLogBkupErr	=	FALSE;
		m_IsDvlLogBkupRun	=	FALSE;

		m_DvlImgThumbFcc	=	E_GrP4DvlDfltThumbFcc;
		GrDumyZeroMem(m_DvlImgTmbCch, sizeof(m_DvlImgTmbCch));

		m_IsDvlPlayDo			=	FALSE;
		m_DvlPlayTime			=	GrTimeGet(&Tv_TmMili, &Tv_IsSm);
		m_DvlPlayPrgs			=	0;


		V_GrP4Mng	=	this;			// update normal manager

		m_PtzMng				=	(Cls_GrPtzV2Mng*) new Cls_GrPtzV2Mng( A_TaskSvr, 16 );
		m_SioMng				=	(Cls_GrP4SioMng*) new Cls_GrP4SioMng( A_TaskSvr );

}
//--------------------------------------------------------------------
Cls_GrP4DvlMng::~Cls_GrP4DvlMng( void )
{
	// local -------------------
	// code --------------------
		DvlPrjClose();		// release project

		// relase image thumbnail
		LcDvlImgTmbAllFree();

		GrTaskSvrMsgCmdFree( m_DvlMsgAloc );

		delete	m_PtzMng;
		delete	m_SioMng;

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjRuleBuild( void )
{
	// local -------------------
		__u8	Tv_SkinIdx;
	// code --------------------
		// init
		Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;

		// script initialize
		m_PtrCfgPrj	=	&m_DvlScptCfgPrj;
		m_PtrCfgRtl	=	&m_DvlScptCfgRtl;
		m_PtrCfgMdl	=	&m_DvlPrjMdlAtb[m_DvlPrjRtl.ModelIdx];
		m_IsScptAble	=	TRUE;

		// window
		m_PtrWinTbl	=	m_DvlScptWinTbl;		// window table update
		m_WinCnt	=	E_GrP4DvlWinMaxCnt;

		// page
		m_PtrPageTbl	=	m_DvlScptPageTbl;		// page table update
		m_PageCnt	=	E_GrP4DvlPageMaxCnt;

		// paint script
		m_PtrPstTbl	=	m_DvlScptPstTbl[Tv_SkinIdx];
		m_PstCnt	=	E_GrP4DvlPstMaxCnt;

		// folder 

		// image
		m_PtrImgTbl	=	m_DvlScptImgTbl[Tv_SkinIdx];

		// text
		m_PtrTxtTbl	=	m_DvlScptTxtTbl;

		// font

		// assembly
		m_PtrAsmTbl	=	m_DvlScptAsmTbl;
		m_AsmCnt	=	E_GrP4DvlAsmMaxCnt;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlPrjNew( __u32 A_FccPx, __u32 A_Res1X, __u32 A_Res1Y, __u32 A_Res2X, __u32 A_Res2Y, 
	BOOL8 A_IsDual, BOOL8 A_IsInputDual )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP4DvlMsgPrjNew	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdPrjNew;
		Tv_Msg.FccPx				=	A_FccPx;
		Tv_Msg.Res1X				=	A_Res1X;
		Tv_Msg.Res1Y				=	A_Res1Y;
		Tv_Msg.Res2X				=	A_Res2X;
		Tv_Msg.Res2Y				=	A_Res2Y;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::LcDvlPrjNew( __u32 A_FccPx, __u32 A_Res1X, __u32 A_Res1Y, __u32 A_Res2X, __u32 A_Res2Y, 
	BOOL8 A_IsDual, BOOL8 A_IsInputDual )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
	// code --------------------
		Tv_Result	=	TRUE;
		// delete all object
		DvlPrjClose();
		// update parameter
		GrDumyZeroMem( &m_DvlPrjConfig, sizeof(m_DvlPrjConfig) );
		GrDumyZeroMem( &m_DvlPrjRtl, sizeof(m_DvlPrjRtl) );
		GrDumyZeroMem( &m_DvlPrjModel, sizeof(m_DvlPrjModel) );
		GrDumyZeroMem( &m_DvlPrjSkin, sizeof(m_DvlPrjSkin) );
		GrDumyZeroMem( &m_DvlPrjMdlAtb, sizeof(m_DvlPrjMdlAtb) );

		m_DvlPrjConfig.ConCfg[0].ResX		=	A_Res1X;
		m_DvlPrjConfig.ConCfg[0].ResY		=	A_Res1Y;
		m_DvlPrjConfig.ConCfg[1].ResX		=	A_Res2X;
		m_DvlPrjConfig.ConCfg[1].ResY		=	A_Res2Y;
		m_DvlPrjConfig.IsDualMon		=	A_IsDual;
		m_DvlPrjConfig.IsInputDual	=	A_IsInputDual;

		GrDumyZeroMem( m_DvlFoldTbl, sizeof(m_DvlFoldTbl) );		// project folder

		GrDumyZeroMem( &m_DvlScptCfgPrj, sizeof(m_DvlScptCfgPrj) );
		GrDumyZeroMem( &m_DvlScptCfgRtl, sizeof(m_DvlScptCfgRtl) );
		// head info
		m_Gvs.ScptTime	=	GrTimeGet( &Tv_Mili, &Tv_IsSm );

		// config project
		m_DvlScptCfgPrj.FmtFcc	=	A_FccPx;

		// config runtime
		m_DvlScptCfgRtl.IsChgConAble	=	FALSE;		// console focus change able
		m_DvlScptCfgRtl.Skin.ClrCus		=	GrDrawCvtColorByFcc( E_GrFccRGB24, 0xFFFFFF, A_FccPx );				// cursor color

		// build rule
		LcPrjRuleBuild();
				
		// update setup
		LcDispSetup();

		// font update
		LcFontUpdt();

		// basic build
		LcPrjSkinNameBuild();
		LcPrjPstDfltBuild();
		LcPrjPstNameBuild();

		// project enable
		m_DvlIsPrjExist	=	TRUE;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDispSetup( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4MngConMaxCnt;Tv_WkIdx++)
		{
			// check exist console
			if ( NULL != m_ConTbl[Tv_WkIdx] )
			{
				// setup display
				m_ConTbl[Tv_WkIdx]->DvlDispSetup();
				m_ConTbl[Tv_WkIdx]->RtlScptUpdt();
			}
		}
		
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::DvlConResX( __u8 A_ConIdx )
{
		return	m_DvlPrjConfig.ConCfg[A_ConIdx].ResX;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::DvlConResY( __u8 A_ConIdx )
{
		return	m_DvlPrjConfig.ConCfg[A_ConIdx].ResY;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::LcFindEmptyWinId( void )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlWinMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL == m_DvlScptWinTbl[Tv_WkIdx].Pos )
			{
				// found
				Tv_Result	=	Tv_WkIdx + E_GrP4ScptIdWinBase;
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::LcFindEmptyPageId( void )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlPageMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL == m_DvlScptPageTbl[Tv_WkIdx].Pos )
			{
				// found
				Tv_Result	=	Tv_WkIdx + E_GrP4ScptIdPageBase;
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlWindowNew( void )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP4DvlMsgWindowNew	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdWindowNew;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::LcDvlWindowNew( void )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_ScptId;
		__u16	Tv_SlotIdx;
		Ptr_GrP4ScptObjHd	Tv_PtrHead;
		Ptr_GrP4ScptObjEnd	Tv_PtrTail;
		__u32	Tv_Size;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;

		if ( DvlIsPrjExist() )
		{
			Tv_ScptId	=	LcFindEmptyWinId();
			if ( E_GrP4ScptIdNone != Tv_ScptId )
			{
				Tv_SlotIdx	=	Tv_ScptId - E_GrP4ScptIdWinBase;
				// create buffer
				m_DvlWinInfo[Tv_SlotIdx].Size	=	0;
				m_DvlScptWinTbl[Tv_SlotIdx].Pos	=	(void*)malloc( E_GrP4DvlScptBufMax );
				if ( NULL != m_DvlScptWinTbl[Tv_SlotIdx].Pos )
				{
					// create default window script
					// add header
					Tv_PtrHead	=	(Ptr_GrP4ScptObjHd)m_DvlScptWinTbl[Tv_SlotIdx].Pos;
					Tv_PtrHead->Stx				=	E_GrP4ScptObjStx;
					Tv_PtrHead->ClassId		=	E_GrP4DoClassWin;
					Tv_PtrHead->ScptId		=	Tv_ScptId;
					Tv_PtrHead->ParaSize	=	0;
					Tv_PtrHead->Rect.left			=	E_GrP4DvlDfltWinLeft;
					Tv_PtrHead->Rect.top			=	E_GrP4DvlDfltWinTop;
					Tv_PtrHead->Rect.right		=	E_GrP4DvlDfltWinRight;
					Tv_PtrHead->Rect.bottom		=	E_GrP4DvlDfltWinBottom;
					Tv_Size		=	sizeof(St_GrP4ScptObjHd);
					// add tail
					Tv_PtrTail	=	(Ptr_GrP4ScptObjEnd)( (__u32)m_DvlScptWinTbl[Tv_SlotIdx].Pos + Tv_Size );
					Tv_PtrTail->Edx			=	E_GrP4ScptObjEdx;
					Tv_PtrTail->ScptId	=	Tv_ScptId;
					Tv_Size		=	Tv_Size + sizeof(St_GrP4ScptObjEnd);
					// register
					m_DvlWinInfo[Tv_SlotIdx].Size			=	Tv_Size;
					m_DvlWinInfo[Tv_SlotIdx].FoldId		=	E_GrP4DvlFolderIdRoot;
					GrStrIntToWstr( m_DvlWinInfo[Tv_SlotIdx].StrName, (__s32)Tv_ScptId );
					m_DvlScptWinTbl[Tv_SlotIdx].Layer	=	E_GrP4DvlDfltLayer;
					// success
					Tv_Result	=	Tv_ScptId;
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DvlMng::LcDvlWindowNew - project is not exist.\n");
		}

		return	Tv_Result;		
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlPageNew( void )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP4DvlMsgPageNew	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdPageNew;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::LcDvlPageNew( void )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_ScptId;
		__u16	Tv_SlotIdx;
		Ptr_GrP4ScptObjHd	Tv_PtrHead;
		Ptr_GrP4ScptObjEnd	Tv_PtrTail;
		__u32	Tv_Size;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;
		if ( DvlIsPrjExist() )
		{
			Tv_ScptId	=	LcFindEmptyPageId();
			if ( E_GrP4ScptIdNone != Tv_ScptId )
			{
				Tv_SlotIdx	=	Tv_ScptId - E_GrP4ScptIdPageBase;
				// create buffer
				m_DvlPageInfo[Tv_SlotIdx].Size	=	0;
				m_DvlScptPageTbl[Tv_SlotIdx].Pos	=	(void*)malloc( E_GrP4DvlScptBufMax );
				if ( NULL != m_DvlScptPageTbl[Tv_SlotIdx].Pos )
				{
					// create default page script
					// add header
					Tv_PtrHead	=	(Ptr_GrP4ScptObjHd)m_DvlScptPageTbl[Tv_SlotIdx].Pos;
					Tv_PtrHead->Stx				=	E_GrP4ScptObjStx;
					Tv_PtrHead->ClassId		=	E_GrP4DoClassWin;
					Tv_PtrHead->ScptId		=	Tv_ScptId;
					Tv_PtrHead->ParaSize	=	0;
					Tv_PtrHead->Rect.left			=	0;
					Tv_PtrHead->Rect.top			=	0;
					Tv_PtrHead->Rect.right		=	1920;
					Tv_PtrHead->Rect.bottom		=	1080;
					Tv_Size		=	sizeof(St_GrP4ScptObjHd);
					// add tail
					Tv_PtrTail	=	(Ptr_GrP4ScptObjEnd)( (__u32)m_DvlScptPageTbl[Tv_SlotIdx].Pos + Tv_Size );
					Tv_PtrTail->Edx			=	E_GrP4ScptObjEdx;
					Tv_PtrTail->ScptId	=	Tv_ScptId;
					Tv_Size		=	Tv_Size + sizeof(St_GrP4ScptObjEnd);
					// regist
					m_DvlPageInfo[Tv_SlotIdx].Size			=	Tv_Size;
					m_DvlPageInfo[Tv_SlotIdx].FoldId		=	E_GrP4DvlFolderIdRoot;
					GrStrIntToWstr( m_DvlPageInfo[Tv_SlotIdx].StrName, (__s32)Tv_ScptId );
					m_DvlScptPageTbl[Tv_SlotIdx].AsmOpen	=	E_GrP4AsmIdNone;
					m_DvlScptPageTbl[Tv_SlotIdx].AsmClose	=	E_GrP4AsmIdNone;
					// success
					Tv_Result	=	Tv_ScptId;
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DvlMng::DvlPageNew - project is not exist.\n");
		}

		return	Tv_Result;		
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcEditObjFree( void )
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP4MngConMaxCnt;Tv_ConIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				// not need synchronize and event
				m_ConTbl[Tv_ConIdx]->WindowAllFree();
			}
		}
		m_DvlEdtLump	=	NULL;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::LcEditWindow( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_SlotIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		// check exist window
		Tv_SlotIdx	=	A_ScptId - E_GrP4ScptIdWinBase;
		if ( NULL != m_DvlScptWinTbl[Tv_SlotIdx].Pos )
		{
			// decode
			LcUiDecWindow( A_ScptId, 0, E_GrP4ScptIdNone );
			// update edit lump
			m_DvlEdtLump	=	m_ConTbl[0]->WinFindByScptId( A_ScptId );
			if ( NULL != m_DvlEdtLump )
			{
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::LcEditPage( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_SlotIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		// check exist page
		Tv_SlotIdx	=	A_ScptId - E_GrP4ScptIdPageBase;
		if ( NULL != m_DvlScptPageTbl[Tv_SlotIdx].Pos )
		{
			// decode
			LcDvlUiDecPage( A_ScptId );
			// update edit lump
			m_DvlEdtLump	=	m_ConTbl[0]->WinFindByScptId( A_ScptId );
			if ( NULL != m_DvlEdtLump )
			{
				Tv_Result	=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlEditLumpSet( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP4DvlMsgEditLumpSet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditLumpSet;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.ScptId				=	A_ScptId;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::LcDvlEditLumpSet( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		// check exist project
		if ( DvlIsPrjExist() )
		{
			// release now edit object
			LcEditObjFree();
			// check script type
			if ( (E_GrP4ScptIdWinBase <= A_ScptId) && ((E_GrP4DvlWinMaxCnt + E_GrP4ScptIdWinBase) > A_ScptId) )
			{
				// window
				LcEditWindow( A_ScptId );
			}
			else if ( (E_GrP4ScptIdPageBase <= A_ScptId) && ((E_GrP4DvlPageMaxCnt + E_GrP4ScptIdPageBase) > A_ScptId) )
			{
				// page
				LcEditPage( A_ScptId );
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlEditLumpSet - bad script id ( %d ).\n", A_ScptId );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlEditLumpSet - project is not exist.\n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP4DvlMng::WindowLayerGet( __u16 A_ScptId )
{
	// local -------------------
		__u8	Tv_Result;
		__u16	Tv_SlotIdx;
	// code --------------------
		Tv_Result	=	0;
		// check script id
		if ( (E_GrP4ScptIdWinBase <= A_ScptId) && ((E_GrP4DvlWinMaxCnt + E_GrP4ScptIdWinBase) > A_ScptId) )
		{
			Tv_SlotIdx	=	A_ScptId - E_GrP4ScptIdWinBase;
			// check exist window
			if ( NULL != m_DvlScptWinTbl[Tv_SlotIdx].Pos )
			{
				Tv_Result	=	m_DvlScptWinTbl[Tv_SlotIdx].Layer;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::WindowLayerSet( __u16 A_ScptId, __u8 A_Layer )
{
	// local -------------------
		__u16	Tv_SlotIdx;
	// code --------------------
		// check script id
		if ( (E_GrP4ScptIdWinBase <= A_ScptId) && ((E_GrP4DvlWinMaxCnt + E_GrP4ScptIdWinBase) > A_ScptId) )
		{
			Tv_SlotIdx	=	A_ScptId - E_GrP4ScptIdWinBase;
			// check exist window
			if ( NULL != m_DvlScptWinTbl[Tv_SlotIdx].Pos )
			{
				// check layer range
				if ( E_GrP4ConLayerMaxCnt > A_Layer )
				{
					m_DvlScptWinTbl[Tv_SlotIdx].Layer	=	A_Layer;
				}
			}
		}
}
//--------------------------------------------------------------------
Ptr_GrP4ScptWinInfo	Cls_GrP4DvlMng::DvlWinScptInfoPtrGet( __u16 A_ScptId )
{
	// local -------------------
		Ptr_GrP4ScptWinInfo	Tv_Result;
		__u16	Tv_SlotIdx;
	// code --------------------
		Tv_Result	=	NULL;
		// check script id
		if ( (E_GrP4ScptIdWinBase <= A_ScptId) && ((E_GrP4DvlWinMaxCnt + E_GrP4ScptIdWinBase) > A_ScptId) )
		{
			Tv_SlotIdx	=	A_ScptId - E_GrP4ScptIdWinBase;
			// check exist window
			if ( NULL != m_DvlScptWinTbl[Tv_SlotIdx].Pos )
			{
				Tv_Result	=	&m_DvlScptWinTbl[Tv_SlotIdx];
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrP4ScptPageInfo	Cls_GrP4DvlMng::DvlPageScptInfoPtrGet( __u16 A_ScptId )
{
	// local -------------------
		Ptr_GrP4ScptPageInfo	Tv_Result;
		__u16	Tv_SlotIdx;
	// code --------------------
		Tv_Result	=	NULL;
		// check script id
		if ( (E_GrP4ScptIdPageBase <= A_ScptId) && ((E_GrP4DvlPageMaxCnt + E_GrP4ScptIdPageBase) > A_ScptId) )
		{
			Tv_SlotIdx	=	A_ScptId - E_GrP4ScptIdPageBase;
			// check exist page
			if ( NULL != m_DvlScptPageTbl[Tv_SlotIdx].Pos )
			{
				Tv_Result	=	&m_DvlScptPageTbl[Tv_SlotIdx];
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::RtlVmDoOneCmd(__u8 A_VmIdx)
{
	// local -------------------
		Ptr_GrP4AsmOpBase	Tv_PtrOpBase;
		BOOL8	Tv_IsAble;
		__u8	Tv_Code;
		__u8	Tv_Sp;
		Ptr_GrP4ScptAsmInfo	Tv_CodeInfo;
		__u16	Tv_ScptId;
	// code --------------------

		// check over instruction pointer
		if (m_VmCsLim > m_VmTbl[A_VmIdx].Ip)
		{
			// prepare parse
			Tv_PtrOpBase	=	&m_PtrVmCs[m_VmTbl[A_VmIdx].Ip];
			// check flag
			Tv_IsAble	=	TRUE;
			if (0 != Tv_PtrOpBase->FlagAble)
			{
				if (0 == (m_VmTbl[A_VmIdx].Flag & Tv_PtrOpBase->FlagAble))
				{
					Tv_IsAble	=	FALSE;
				}
			}
			if (0 != (m_VmTbl[A_VmIdx].Flag & Tv_PtrOpBase->FlagBan))
			{
				Tv_IsAble	=	FALSE;
			}
			// check dead command
			if (0 != (E_GrP4AsmOpDeactMask & Tv_PtrOpBase->Code))
			{
				Tv_IsAble	=	FALSE;
			}

			if (Tv_IsAble)
			{
				// check break point set
				if ((!m_DvlDbgIsResume) && (0 != (E_GrP4AsmOpBrkPntMask & Tv_PtrOpBase->Code)))
				{
					// step pause
					m_DvlDbgMode	=	E_GrP4DvlDbgModePause;
					// notify debug stop
					GrP4DvlIdeMsgPost(E_GrP4DvlIdeCmdVmBrkPnt, (__u32)A_VmIdx);
					return;
				}
			}

			if (Tv_IsAble)
			{
				// parse command
				Tv_Code	=	Tv_PtrOpBase->Code & E_GrP4AsmOpCodeMask;
				switch (Tv_Code)
				{
					case E_GrP4AsmOpNop:
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case	E_GrP4AsmOpEnd:
						m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
						if ((E_GrP4DvlDbgModeStep == m_DvlDbgMode) && (A_VmIdx == m_DvlDbgStepVm))
						{
							m_DvlDbgMode	=	E_GrP4DvlDbgModeRun;
						}
						// finish
						break;
					case E_GrP4AsmOpCond:
						LcAsmOpCond(A_VmIdx, (Ptr_GrP4AsmOpCond)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpMove:
						LcAsmOpMove(A_VmIdx, (Ptr_GrP4AsmOpMove)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpFlag:
						LcAsmOpFlag(A_VmIdx, (Ptr_GrP4AsmOpFlag)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpCalc:
						LcAsmOpCalc(A_VmIdx, (Ptr_GrP4AsmOpCalc)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpNot:
						LcAsmOpNot(A_VmIdx, (Ptr_GrP4AsmOpNot)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpDirJump:
						LcAsmOpDirJump(A_VmIdx, (Ptr_GrP4AsmOpDirJump)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpRefJump:
						LcAsmOpRefJump(A_VmIdx, (Ptr_GrP4AsmOpRefJump)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpRelJump:
						LcAsmOpRelJump(A_VmIdx, (Ptr_GrP4AsmOpRelJump)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpOfsJump:
						LcAsmOpOfsJump(A_VmIdx, (Ptr_GrP4AsmOpOfsJump)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpTxtClear:
						LcAsmOpTxtClr(A_VmIdx, (Ptr_GrP4AsmOpTxtClear)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtIdGet:
						LcAsmOpTxtId(A_VmIdx, (Ptr_GrP4AsmOpTxtIdGet)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtDec:
						LcAsmOpTxtDec(A_VmIdx, (Ptr_GrP4AsmOpTxtDec)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtHex:
						LcAsmOpTxtHex(A_VmIdx, (Ptr_GrP4AsmOpTxtHex)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtChar:
						LcAsmOpTxtCharAdd(A_VmIdx, (Ptr_GrP4AsmOpTxtChar)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtCopy:
						LcAsmOpTxtCopy(A_VmIdx, (Ptr_GrP4AsmOpTxtCopy)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtSetStp:
						LcAsmOpTxtSetSetup(A_VmIdx, (Ptr_GrP4AsmOpTxtSetStp)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtGetStp:
						LcAsmOpTxtGetSetup(A_VmIdx, (Ptr_GrP4AsmOpTxtGetStp)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpSetupCopy:
						LcAsmOpSetupCopy(A_VmIdx, (Ptr_GrP4AsmOpSetupCopy)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpSetupFill:
						LcAsmOpSetupFill(A_VmIdx, (Ptr_GrP4AsmOpSetupFill)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpWindow:
						LcAsmOpWindow(A_VmIdx, (Ptr_GrP4AsmOpWindow)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpUpi:
						// check UPI break point
						if (!m_DvlDbgIsResume)
						{
							Tv_ScptId	=	m_DvlMdlDblTbl[m_DvlPrjRtl.ModelIdx].UpiBpObj;
							if (E_GrP4ScptIdNone == Tv_ScptId)
							{
								Tv_ScptId	=	((Ptr_GrP4AsmOpUpi)Tv_PtrOpBase)->ObjScptId;
							}
							if (Tv_ScptId == ((Ptr_GrP4AsmOpUpi)Tv_PtrOpBase)->ObjScptId)
							{
								if (E_GrP4UpiMaxCnt > ((Ptr_GrP4AsmOpUpi)Tv_PtrOpBase)->UpiCmd)
								{
									if (m_DvlMdlDblTbl[m_DvlPrjRtl.ModelIdx].UpiBpTbl[((Ptr_GrP4AsmOpUpi)Tv_PtrOpBase)->UpiCmd])
									{
										// step pause
										m_DvlDbgMode	=	E_GrP4DvlDbgModePause;
										// notify debug stop
										GrP4DvlIdeMsgPost(E_GrP4DvlIdeCmdVmBrkPnt, (__u32)A_VmIdx);
										return;
									}
								}
							}
						}
						LcAsmOpUpi(A_VmIdx, (Ptr_GrP4AsmOpUpi)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpApi:
						// check API break point
						if (!m_DvlDbgIsResume)
						{
							if (E_GrP4ApiTblCnt > ((Ptr_GrP4AsmOpApi)Tv_PtrOpBase)->ApiCmd)
							{
								if (m_DvlMdlDblTbl[m_DvlPrjRtl.ModelIdx].ApiBpTbl[((Ptr_GrP4AsmOpApi)Tv_PtrOpBase)->ApiCmd])
								{
									// step pause
									m_DvlDbgMode	=	E_GrP4DvlDbgModePause;
									// notify debug stop
									GrP4DvlIdeMsgPost(E_GrP4DvlIdeCmdVmBrkPnt, (__u32)A_VmIdx);
									return;
								}
							}
						}
						m_VmTbl[A_VmIdx].Ip	++;
						LcAsmOpApi(A_VmIdx, (Ptr_GrP4AsmOpApi)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpNotify:
						LcAsmOpNotify(A_VmIdx, (Ptr_GrP4AsmOpNotify)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpQuery:
						LcAsmOpQuery(A_VmIdx, (Ptr_GrP4AsmOpQuery)Tv_PtrOpBase);
						//m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtTime:
						LcAsmOpTxtTime(A_VmIdx, (Ptr_GrP4AsmOpTxtTime)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtCmp:
						LcAsmOpTxtCmp(A_VmIdx, (Ptr_GrP4AsmOpTxtCmp)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpTxtLen:
						LcAsmOpTxtLen(A_VmIdx, (Ptr_GrP4AsmOpTxtLen)Tv_PtrOpBase);
						m_VmTbl[A_VmIdx].Ip	++;
						break;
					case E_GrP4AsmOpWait:
						LcAsmOpWait(A_VmIdx, (Ptr_GrP4AsmOpWait)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpLabel:
						LcAsmOpLabel(A_VmIdx, (Ptr_GrP4AsmOpLabel)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpLblJump:
						LcAsmOpLblJump(A_VmIdx, (Ptr_GrP4AsmOpLblJump)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpReturn:
						LcAsmOpReturn(A_VmIdx, Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpDcall:
						LcAsmOpDcall(A_VmIdx, (Ptr_GrP4AsmOpDcall)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpStpGet:
						LcAsmOpStpGet(A_VmIdx, (Ptr_GrP4AsmOpStpGet)Tv_PtrOpBase);
						break;
					case E_GrP4AsmOpStpPut:
						LcAsmOpStpPut(A_VmIdx, (Ptr_GrP4AsmOpStpPut)Tv_PtrOpBase);
						break;

					default:
						// bad opcode
						DbgMsgPrint("Cls_GrP4Mng::RtlVmDoOneCmd - bad opcode (%d) at cs: %d , ip: %d \n", Tv_Code, m_VmTbl[A_VmIdx].Cs, m_VmTbl[A_VmIdx].Ip);
						m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
						if ((E_GrP4DvlDbgModeStep == m_DvlDbgMode) && (A_VmIdx == m_DvlDbgStepVm))
						{
							m_DvlDbgMode	=	E_GrP4DvlDbgModeRun;
						}
						break;
				}
			}
			else
			{
				// no execute
				m_VmTbl[A_VmIdx].Ip	++;
			}
		}

		// check status
		if (E_GrP4AsmRunStatRun == m_VmTbl[A_VmIdx].Rstat)
		{
			// check finish
			if (m_VmTbl[A_VmIdx].Ip >= m_VmCsLim)
			{
				// check stack
				if (0 != m_VmTbl[A_VmIdx].Sp)
				{
					// stack pop
					m_VmTbl[A_VmIdx].Sp --;
					// change pointer
					Tv_Sp	=	m_VmTbl[A_VmIdx].Sp;
					Tv_CodeInfo			=	AsmPtrGet(m_VmTbl[A_VmIdx].CallStack[Tv_Sp].Cs);
					if (NULL != Tv_CodeInfo)
					{
						m_PtrVmCs				=	(Ptr_GrP4AsmOpBase)Tv_CodeInfo->Pos;
						m_VmCsLim				=	Tv_CodeInfo->Cnt;
						m_VmTbl[A_VmIdx].Cs			=	m_VmTbl[A_VmIdx].CallStack[Tv_Sp].Cs;
						m_VmTbl[A_VmIdx].Ip			=	m_VmTbl[A_VmIdx].CallStack[Tv_Sp].Ip;
					}
					else
					{
						m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
						DbgMsgPrint("Cls_GrP4Mng::RtlVmDoOneCmd - VM(%d) stack broken!\n", A_VmIdx);
						if ((E_GrP4DvlDbgModeStep == m_DvlDbgMode) && (A_VmIdx == m_DvlDbgStepVm))
						{
							m_DvlDbgMode	=	E_GrP4DvlDbgModeRun;
						}
					}
				}
				else
				{
					// finish
					m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
					if ((E_GrP4DvlDbgModeStep == m_DvlDbgMode) && (A_VmIdx == m_DvlDbgStepVm))
					{
						m_DvlDbgMode	=	E_GrP4DvlDbgModeRun;
					}

				}
			}

		}

		// check step mode
		if ((E_GrP4DvlDbgModeStep == m_DvlDbgMode)&& (A_VmIdx == m_DvlDbgStepVm))
		{
			// step pause
			m_DvlDbgMode	=	E_GrP4DvlDbgModePause;
			// notify debug stop
			GrP4DvlIdeMsgPost(E_GrP4DvlIdeCmdVmBrkPnt, (__u32)A_VmIdx);
		}
		m_DvlDbgIsResume	=	FALSE;

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::RtlVmDoSlice(__u8 A_VmIdx)
{
	// local -------------------
		Ptr_GrP4ScptAsmInfo	Tv_CodeInfo;
		__u32	Tv_RunCnt;
	// code --------------------
		// check run able
		if ((E_GrP4AsmRunStatRun == m_VmTbl[A_VmIdx].Rstat) && ((E_GrP4DvlDbgModeRun == m_DvlDbgMode) || (E_GrP4DvlDbgModeStep == m_DvlDbgMode)))
		{
			// prepare
			Tv_CodeInfo			=	AsmPtrGet(m_VmTbl[A_VmIdx].Cs);
			if (NULL != Tv_CodeInfo)
			{
				// prepare
				m_PtrVmCs				=	(Ptr_GrP4AsmOpBase)Tv_CodeInfo->Pos;
				m_VmCsLim				=	Tv_CodeInfo->Cnt;
				m_VmHostObj			=	NULL;
				m_VmHostWin			=	NULL;

				if (NULL != m_ConTbl[m_VmTbl[A_VmIdx].ConIdx])
				{
					m_VmHostWin	=	m_ConTbl[m_VmTbl[A_VmIdx].ConIdx]->WinFindByWinId(m_VmTbl[A_VmIdx].HostWinId);
					if (NULL != m_VmHostWin)
					{
						if (E_GrP4ScptIdNone == m_VmTbl[A_VmIdx].HostScptId)
						{
							m_VmHostObj	=	(Cls_GrP4DoBase*)m_VmHostWin;
						}
						else
						{
							m_VmHostObj	=	m_VmHostWin->FindSlaveObjByScptId(m_VmTbl[A_VmIdx].HostScptId);
						}
					}
				}

				// run
				for (Tv_RunCnt=0; Tv_RunCnt <E_GrP4MngVmSliceMax; Tv_RunCnt++)
				{
					RtlVmDoOneCmd(A_VmIdx);
					if ((E_GrP4AsmRunStatRun != m_VmTbl[A_VmIdx].Rstat) || (E_GrP4DvlDbgModeStop == m_DvlDbgMode) || (E_GrP4DvlDbgModePause == m_DvlDbgMode))
					{
						break;
					}
				}
			}
			else
			{
				m_VmTbl[A_VmIdx].Rstat	=	E_GrP4AsmRunStatStop;
				if ((E_GrP4DvlDbgModeStep == m_DvlDbgMode) && (A_VmIdx == m_DvlDbgStepVm))
				{
					m_DvlDbgMode	=	E_GrP4DvlDbgModeRun;
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::RtlVmFault(__u8 A_VmIdx)
{
	// local -------------------
	// code --------------------
		// pause VM running
		m_DvlDbgMode	=	E_GrP4DvlDbgModePause;
		GrP4DvlIdeMsgPost( E_GrP4DvlIdeCmdVmFault,(__u32)A_VmIdx );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlIsPrjExist( void )
{
	// local -------------------
	// code --------------------
		return	m_DvlIsPrjExist;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlSlaveScptIdAlloc( void )
{
	// local -------------------
		__u16	Tv_Result;
		__u32	Tv_Mask;
		__u16	Tv_DwIdx;
		__u16	Tv_BitIdx;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;
		for ( Tv_DwIdx=0;Tv_DwIdx < E_GrP4DvlSlaveMapCnt;Tv_DwIdx++ )
		{
			// check exist dw
			if ( 0xFFFFFFFF != m_DvlSlvMap[Tv_DwIdx] )
			{
				// find bit position
				Tv_Mask	=	1;
				for ( Tv_BitIdx=0;Tv_BitIdx < 32;Tv_BitIdx++ )
				{
					if ( 0 == (Tv_Mask & m_DvlSlvMap[Tv_DwIdx]) )
					{
						// found
						m_DvlSlvMap[Tv_DwIdx]	=	m_DvlSlvMap[Tv_DwIdx] | Tv_Mask;
						Tv_Result	=	(Tv_DwIdx << E_GrP4DvlSlaveMapShft) + Tv_BitIdx + E_GrP4ScptIdSlaveBase;
						break;
					}
					// next
					Tv_Mask	=	Tv_Mask << 1;
				}
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlSlaveScptIdFree( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_DwIdx;
		__u16	Tv_BitIdx;
		__u16	Tv_Mask;
	// code --------------------
		// check in range
		if ( (E_GrP4ScptIdSlaveBase <= A_ScptId) && ((E_GrP4ScptIdSlaveBase + E_GrP4DvlSlaveMaxCnt) > A_ScptId) )
		{
			Tv_DwIdx	=	(A_ScptId - E_GrP4ScptIdSlaveBase) >> E_GrP4DvlSlaveMapShft;
			Tv_BitIdx	=	(A_ScptId - E_GrP4ScptIdSlaveBase) & ((1 << E_GrP4DvlSlaveMapShft) - 1);
			Tv_Mask		=	1 << Tv_BitIdx;
			if ( 0 != (Tv_Mask & m_DvlSlvMap[Tv_DwIdx]) )
			{
				// remove
				m_DvlSlvMap[Tv_DwIdx]	=	m_DvlSlvMap[Tv_DwIdx] & (~Tv_Mask);
			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::DvlSlaveScptIdFree - Script id (%d) is already removed.\n", A_ScptId );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DvlMng::DvlSlaveScptIdFree - Script id (%d) is not slave id.\n", A_ScptId );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DvlMng::DvlClassNameGet( __u8 A_ClassId )
{
		return	V_GrP4DvlClassNameTbl[A_ClassId];
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlIsPageWindow( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( (E_GrP4ScptIdPageBase <= A_ScptId) && ((E_GrP4ScptIdPageBase + E_GrP4DvlPageMaxCnt) > A_ScptId) )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlPrjClose( void )
{
	// local -------------------
		//St_GrP4DvlMsgBase	Tv_Msg;
	// code --------------------
		/*
		Tv_Msg.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Cmd				=	E_GrP4DvlMcmdPrjClose;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		*/
		LcDvlPrjClose();
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlPrjClose( void )
{
	// local -------------------
		__s32		Tv_WkIdx;
		__u8	Tv_SkinIdx;
		__u8	Tv_LangIdx;
	// code --------------------
		// release window
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlWinMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_DvlScptWinTbl[Tv_WkIdx].Pos )
			{
				free( m_DvlScptWinTbl[Tv_WkIdx].Pos );
				m_DvlScptWinTbl[Tv_WkIdx].Pos	=	NULL;
			}
		}
		GrDumyZeroMem( m_DvlPageInfo, sizeof(m_DvlPageInfo) );		// clear info
		// release page
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlPageMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_DvlScptPageTbl[Tv_WkIdx].Pos )
			{
				free( m_DvlScptPageTbl[Tv_WkIdx].Pos );
				m_DvlScptPageTbl[Tv_WkIdx].Pos	=	NULL;
			}
		}
		GrDumyZeroMem( m_DvlPageInfo, sizeof(m_DvlPageInfo) );		// clear info
		// release paint script
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP4DvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlPstMaxCnt;Tv_WkIdx++ )
			{
				if ( NULL != m_DvlScptPstTbl[Tv_SkinIdx][Tv_WkIdx].Pos )
				{
					free( m_DvlScptPstTbl[Tv_SkinIdx][Tv_WkIdx].Pos );
					m_DvlScptPstTbl[Tv_SkinIdx][Tv_WkIdx].Pos	=	NULL;
				}
			}
		}
		GrDumyZeroMem( m_DvlPstInfo, sizeof(m_DvlPstInfo) );
		// release image
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP4DvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4MngImgScptMaxCnt;Tv_WkIdx++ )
			{
				if ( NULL != m_DvlScptImgTbl[Tv_SkinIdx][Tv_WkIdx] )
				{
					free( m_DvlScptImgTbl[Tv_SkinIdx][Tv_WkIdx] );
					m_DvlScptImgTbl[Tv_SkinIdx][Tv_WkIdx]	=	NULL;
				}
			}
		}
		// release text
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP4MngTextScptMaxCnt;Tv_WkIdx++)
		{
			for ( Tv_LangIdx=0;Tv_LangIdx < E_GrP4ScptLangMaxCnt;Tv_LangIdx++ )
			{
				if ( NULL != m_DvlScptTxtTbl[Tv_WkIdx].LngTbl[Tv_LangIdx] )
				{
					free(m_DvlScptTxtTbl[Tv_WkIdx].LngTbl[Tv_LangIdx]);
					m_DvlScptTxtTbl[Tv_WkIdx].LngTbl[Tv_LangIdx]	=	NULL;
				}
			}
		}
		// release font
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP4DvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4MngFontMaxCnt;Tv_WkIdx++ )
			{
				if ( NULL != m_DvlScptFontTbl[Tv_SkinIdx][Tv_WkIdx] )
				{
					free( m_DvlScptFontTbl[Tv_SkinIdx][Tv_WkIdx] );
					m_DvlScptFontTbl[Tv_SkinIdx][Tv_WkIdx]	=	NULL;
				}
			}
		}
		// release assembly
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlAsmMaxCnt;Tv_WkIdx++ )
		{
			// release code
			if ( NULL != m_DvlScptAsmTbl[Tv_WkIdx].Pos )
			{
				free( m_DvlScptAsmTbl[Tv_WkIdx].Pos );
				m_DvlScptAsmTbl[Tv_WkIdx].Pos	=	NULL;
			}
		}
		GrDumyZeroMem( m_DvlAsmInfo, sizeof(m_DvlAsmInfo) );
		GrDumyZeroMem(m_DvlAsmRmkTbl, sizeof(m_DvlAsmRmkTbl));
		GrDumyZeroMem(m_DvlAsmFncCmt, sizeof(m_DvlAsmFncCmt));

		// clear folder info
		GrDumyZeroMem( m_DvlFoldTbl, sizeof(m_DvlFoldTbl) );
		GrDumyZeroMem( m_DvlWinInfo, sizeof(m_DvlWinInfo) );		// develop table clear
		GrDumyZeroMem( m_DvlPageInfo, sizeof(m_DvlPageInfo) );		// develop table clear
		GrDumyZeroMem( m_DvlSlvMap, sizeof(m_DvlSlvMap) );			// slave id map clear

		// clear setup variable table
		m_DvlStpUsrCnt	=	0;
		GrDumyZeroMem( m_DvlStpUsrTbl, sizeof(m_DvlStpUsrTbl) );

		// clear global variable user type 
		GrDumyZeroMem( m_DvlGvuTbl, sizeof(m_DvlGvuTbl) );

		// clear user registry info table
		GrDumyZeroMem( m_DvlUrgstTbl, sizeof(m_DvlUrgstTbl) );

		// clear setting
		//GrDumyZeroMem( &m_DvlSetBase, sizeof(m_DvlSetBase) );

		// clear debug
		GrDumyZeroMem( m_DvlMdlDblTbl, sizeof(m_DvlMdlDblTbl) );

		// clear file make
		GrDumyZeroMem( m_DvlBldInfo, sizeof(m_DvlBldInfo) );

		// release console all
		LcWinAllFree();
			
		// release edit object
		m_DvlEdtLump	=	NULL;

		// project not exist
		m_DvlIsPrjExist	=	FALSE;

		// relase image thumbnail
		LcDvlImgTmbAllFree();
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcGetEditObjRect(Ptr_GrRect A_PtrRcRslt, Ptr_GrRect A_PtrRcScrPrnt,
	__s32 A_CtX, __s32 A_CtY, __s32 A_SizeX, __s32 A_SizeY )
{
	// local -------------------
	// code --------------------
		A_PtrRcRslt->left		=	A_CtX - (A_SizeX >>1);
		A_PtrRcRslt->top		=	A_CtY - (A_SizeY >>1);
		A_PtrRcRslt->right	=	A_PtrRcRslt->left + A_SizeX;
		A_PtrRcRslt->bottom	=	A_PtrRcRslt->top + A_SizeY;
		if (0 > A_PtrRcRslt->left)
		{
			A_PtrRcRslt->left	=	0;
		}
		if ((A_PtrRcScrPrnt->right - A_PtrRcScrPrnt->left) < A_PtrRcRslt->right)
		{
			A_PtrRcRslt->right	=	A_PtrRcScrPrnt->right - A_PtrRcScrPrnt->left;
		}
		if (0 > A_PtrRcRslt->top)
		{
			A_PtrRcRslt->top	=	0;
		}
		if ((A_PtrRcScrPrnt->bottom - A_PtrRcScrPrnt->top) < A_PtrRcRslt->right)
		{
			A_PtrRcRslt->bottom	=	A_PtrRcScrPrnt->bottom - A_PtrRcScrPrnt->top;
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlEditObjCreate( __s32 A_ScrX, __s32 A_ScrY, __u8 A_ClassId )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP4DvlMsgEditObjCreate	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;
		
		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditObjCreate;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.ScrX					=	A_ScrX;
		Tv_Msg.ScrY					=	A_ScrY;
		Tv_Msg.ClassId			=	A_ClassId;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::LcDvlEditObjCreate( __s32 A_ScrX, __s32 A_ScrY, __u8 A_ClassId )
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrP4DoBase*	Tv_ObjNew;
		Cls_GrP4DoBase*	Tv_ObjPrnt;
		St_GrRect	Tv_RcPrnt;
		__s32		Tv_X;
		__s32		Tv_Y;
		St_GrRect	Tv_RcObj;
	// code --------------------
		Tv_Result		=	E_GrP4ScptIdNone;
		if ( DvlIsPrjExist() )
		{
			Tv_ObjPrnt	=	m_ConTbl[0]->ObjFindByPos( A_ScrX, A_ScrY );
			if ( NULL != Tv_ObjPrnt )
			{
				// check edit create able
				if ( NULL == Tv_ObjPrnt->FrameGet() )
				{
					// get parent screen rect
					if ( Tv_ObjPrnt->ScrRectGet( &Tv_RcPrnt, TRUE ) )
					{
						// object center position
						Tv_X	=	A_ScrX - Tv_RcPrnt.left;
						Tv_Y	=	A_ScrY - Tv_RcPrnt.top;
						// make
						LcGetEditObjRect( &Tv_RcObj, &Tv_RcPrnt, Tv_X, Tv_Y, E_GrP4DvlDfltObjSizeX, E_GrP4DvlDfltObjSizeY );
						Tv_ObjNew	=	LcUiObjCreate( A_ClassId, E_GrP4ScptIdNone, &Tv_RcObj, 0, Tv_ObjPrnt );
						if ( NULL != Tv_ObjNew )
						{
							Tv_Result	=	Tv_ObjNew->ScptIdGet();
							Tv_ObjNew->ReqDraw();
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlEditObjCreate - Can not get screen position.\n" );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlEditObjCreate - Can not create object under frame.\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlEditObjCreate - No parent object!" );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlFolderInfoGet( __u16 A_Id, WCHAR* A_StrRtName, __u16* A_PtrRtPrntId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( DvlIsPrjExist() )
		{
			if ( (E_GrP4DvlFolderIdBase <= A_Id) && ((E_GrP4DvlFolderIdBase + E_GrP4DvlFolderMaxCnt) > A_Id) )
			{
				Tv_Idx		=	A_Id - E_GrP4DvlFolderIdBase;
				if ( m_DvlFoldTbl[Tv_Idx].IsExist )
				{
					GrStrWcopy( A_StrRtName, m_DvlFoldTbl[Tv_Idx].StrName );
					*A_PtrRtPrntId	=	m_DvlFoldTbl[Tv_Idx].PrntId;
					Tv_Result	=	TRUE;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlFolderInfoSet( __u16 A_Id, WCHAR* A_StrName, __u16 A_PrntId )
{
	// local -------------------
		__u16	Tv_Idx;
	// code --------------------
		if ( DvlIsPrjExist() )
		{
			if ( (E_GrP4DvlFolderIdBase <= A_Id) && ((E_GrP4DvlFolderIdBase + E_GrP4DvlFolderMaxCnt) > A_Id) )
			{
				Tv_Idx		=	A_Id - E_GrP4DvlFolderIdBase;
				m_DvlFoldTbl[Tv_Idx].IsExist	=	TRUE;
				GrStrWcopy( m_DvlFoldTbl[Tv_Idx].StrName, A_StrName, 31 );
				m_DvlFoldTbl[Tv_Idx].PrntId	=	A_PrntId;
			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::DvlFolderInfoSet - bad foder id( %d ).\n", A_Id );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DvlMng::DvlFolderInfoSet - project not exist.\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlFolderNameSet( __u16 A_Id, WCHAR* A_StrName )
{
	// local -------------------
		__u16	Tv_Idx;
	// code --------------------
		if ( (E_GrP4DvlFolderIdBase <= A_Id) && ((E_GrP4DvlFolderIdBase + E_GrP4DvlFolderMaxCnt) > A_Id) )
		{
			Tv_Idx		=	A_Id - E_GrP4DvlFolderIdBase;
			GrStrWcopy( m_DvlFoldTbl[Tv_Idx].StrName, A_StrName, 31 );
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DvlMng::DvlFolderNameSet - bad foder id( %d ).\n", A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlFolderPrntIdSet( __u16 A_Id, __u16 A_PrntId )
{
	// local -------------------
		__u16	Tv_Idx;
	// code --------------------
		if ( (E_GrP4DvlFolderIdBase <= A_Id) && ((E_GrP4DvlFolderIdBase + E_GrP4DvlFolderMaxCnt) > A_Id) )
		{
			// check parent id
			if (	(E_GrP4DvlFolderIdRoot == A_PrntId) || 
						((E_GrP4DvlFolderIdBase <= A_PrntId) && ((E_GrP4DvlFolderIdBase + E_GrP4DvlFolderMaxCnt) > A_PrntId)) 
					)
			{
				Tv_Idx		=	A_Id - E_GrP4DvlFolderIdBase;
				m_DvlFoldTbl[Tv_Idx].PrntId	=	A_PrntId;
			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::DvlFolderPrntIdSet - bad parent foder id( %d ).\n", A_PrntId );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DvlMng::DvlFolderPrntIdSet - bad foder id( %d ).\n", A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlFolderExpandSet( __u16 A_Id, BOOL8 A_IsExpand )
{
	// local -------------------
		__u16	Tv_Idx;
	// code --------------------
		if ( (E_GrP4DvlFolderIdBase <= A_Id) && ((E_GrP4DvlFolderIdBase + E_GrP4DvlFolderMaxCnt) > A_Id) )
		{
			Tv_Idx		=	A_Id - E_GrP4DvlFolderIdBase;
			m_DvlFoldTbl[Tv_Idx].IsExpand	=	A_IsExpand;
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DvlMng::DvlFolderExpandSet - bad foder id( %d ).\n", A_Id );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlFolderIsExpand( __u16 A_Id )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( (E_GrP4DvlFolderIdBase <= A_Id) && ((E_GrP4DvlFolderIdBase + E_GrP4DvlFolderMaxCnt) > A_Id) )
		{
			Tv_Idx		=	A_Id - E_GrP4DvlFolderIdBase;
			Tv_Result	=	m_DvlFoldTbl[Tv_Idx].IsExpand;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlFolderDelete( __u16 A_Id )
{
	// local -------------------
		__u16	Tv_Idx;
		__u16	Tv_ScptId;
	// code --------------------
		if ( DvlIsPrjExist() )
		{
			if ( (E_GrP4DvlFolderIdBase <= A_Id) && ((E_GrP4DvlFolderIdBase + E_GrP4DvlFolderMaxCnt) > A_Id) )
			{
				Tv_Idx		=	A_Id - E_GrP4DvlFolderIdBase;
				m_DvlFoldTbl[Tv_Idx].IsExist	=	FALSE;
				m_DvlFoldTbl[Tv_Idx].IsExpand	=	FALSE;
				// delete window
				Tv_ScptId	=	DvlWinFindByPrntFolder( A_Id );
				while ( E_GrP4ScptIdNone != Tv_ScptId )
				{
					// delete window
					DvlWindowDel( Tv_ScptId );
					// next
					Tv_ScptId	=	DvlWinFindByPrntFolder( A_Id );
				}
				// delete page
				Tv_ScptId	=	DvlPageFindByPrntFolder( A_Id );
				while ( E_GrP4ScptIdNone != Tv_ScptId )
				{
					// delete window
					DvlPageDel( Tv_ScptId );
					// next
					Tv_ScptId	=	DvlPageFindByPrntFolder( A_Id );
				}

			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::DvlFolderDelete - bad foder id( %d ).\n", A_Id );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DvlMng::DvlFolderDelete - project not exist.\n");
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DvlMng::DvlWinNameGet( __u16 A_ScptId )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16	Tv_WinIdx;
	// code --------------------
		Tv_Result	=	NULL;
		if ( (E_GrP4ScptIdWinBase <= A_ScptId) && ((E_GrP4ScptIdWinBase + E_GrP4DvlWinMaxCnt) > A_ScptId) )
		{
			Tv_WinIdx	=	A_ScptId - E_GrP4ScptIdWinBase;
			// check exist window
			if ( NULL != m_PtrWinTbl[Tv_WinIdx].Pos )
			{
				Tv_Result	=	m_DvlWinInfo[Tv_WinIdx].StrName;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DvlMng::DvlPageNameGet( __u16 A_ScptId )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16	Tv_PageIdx;
	// code --------------------
		Tv_Result	=	NULL;
		if ( (E_GrP4ScptIdPageBase <= A_ScptId) && ((E_GrP4ScptIdPageBase + E_GrP4DvlPageMaxCnt) > A_ScptId) )
		{
			Tv_PageIdx	=	A_ScptId - E_GrP4ScptIdPageBase;
			// check exist page
			if ( NULL != m_PtrPageTbl[Tv_PageIdx].Pos )
			{
				Tv_Result	=	m_DvlPageInfo[Tv_PageIdx].StrName;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlWinNameSet( __u16 A_ScptId, WCHAR* A_StrName )
{
	// local -------------------
		__u16	Tv_WinIdx;
	// code --------------------
		if ( (E_GrP4ScptIdWinBase <= A_ScptId) && ((E_GrP4ScptIdWinBase + E_GrP4DvlWinMaxCnt) > A_ScptId) )
		{
			Tv_WinIdx	=	A_ScptId - E_GrP4ScptIdWinBase;
			// check exist window
			if ( NULL != m_PtrWinTbl[Tv_WinIdx].Pos )
			{
				GrStrWcopy( m_DvlWinInfo[Tv_WinIdx].StrName, A_StrName, 31 );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlPageNameSet( __u16 A_ScptId, WCHAR* A_StrName )
{
	// local -------------------
		__u16	Tv_PgIdx;
	// code --------------------
		if ( (E_GrP4ScptIdPageBase <= A_ScptId) && ((E_GrP4ScptIdPageBase + E_GrP4DvlPageMaxCnt) > A_ScptId) )
		{
			Tv_PgIdx	=	A_ScptId - E_GrP4ScptIdPageBase;
			// check exist window
			if ( NULL != m_PtrPageTbl[Tv_PgIdx].Pos )
			{
				GrStrWcopy( m_DvlPageInfo[Tv_PgIdx].StrName, A_StrName, 31 );
			}
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlWinPrntFolderGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_WinIdx;
	// code --------------------
		Tv_Result	=	E_GrP4DvlFolderIdRoot;
		if ( (E_GrP4ScptIdWinBase <= A_ScptId) && ((E_GrP4ScptIdWinBase + E_GrP4DvlWinMaxCnt) > A_ScptId) )
		{
			Tv_WinIdx	=	A_ScptId - E_GrP4ScptIdWinBase;
			// check exist window
			if ( NULL != m_PtrWinTbl[Tv_WinIdx].Pos )
			{
				Tv_Result	=	m_DvlWinInfo[Tv_WinIdx].FoldId;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlPagePrntFolderGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_PageIdx;
	// code --------------------
		Tv_Result	=	E_GrP4DvlFolderIdRoot;
		if ( (E_GrP4ScptIdPageBase <= A_ScptId) && ((E_GrP4ScptIdPageBase + E_GrP4DvlPageMaxCnt) > A_ScptId) )
		{
			Tv_PageIdx	=	A_ScptId - E_GrP4ScptIdPageBase;
			// check exist page
			if ( NULL != m_PtrPageTbl[Tv_PageIdx].Pos )
			{
				Tv_Result	=	m_DvlPageInfo[Tv_PageIdx].FoldId;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlWinPrntFolderSet( __u16 A_ScptId, __u16 A_FolderId )
{
	// local -------------------
		__u16	Tv_WinIdx;
	// code --------------------
		if ( (E_GrP4ScptIdWinBase <= A_ScptId) && ((E_GrP4ScptIdWinBase + E_GrP4DvlWinMaxCnt) > A_ScptId) )
		{
			Tv_WinIdx	=	A_ScptId - E_GrP4ScptIdWinBase;
			if ( NULL != m_PtrWinTbl[Tv_WinIdx].Pos )
			{
				m_DvlWinInfo[Tv_WinIdx].FoldId	=	A_FolderId;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlPagePrntFolderSet( __u16 A_ScptId, __u16 A_FolderId )
{
	// local -------------------
		__u16	Tv_PageIdx;
	// code --------------------
		if ( (E_GrP4ScptIdPageBase <= A_ScptId) && ((E_GrP4ScptIdPageBase + E_GrP4DvlPageMaxCnt) > A_ScptId) )
		{
			Tv_PageIdx	=	A_ScptId - E_GrP4ScptIdPageBase;
			if ( NULL != m_PtrPageTbl[Tv_PageIdx].Pos )
			{
				m_DvlPageInfo[Tv_PageIdx].FoldId	=	A_FolderId;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlWinIsExist( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_WinIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( DvlIsPrjExist() )
		{
			if ( (E_GrP4ScptIdWinBase <= A_ScptId) && ((E_GrP4ScptIdWinBase + E_GrP4DvlWinMaxCnt) > A_ScptId) )
			{
				Tv_WinIdx	=	A_ScptId - E_GrP4ScptIdWinBase;
				if ( NULL != m_PtrWinTbl[Tv_WinIdx].Pos )
				{
					Tv_Result	=	TRUE;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlPageIsExist( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_PageIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( DvlIsPrjExist() )
		{
			if ( (E_GrP4ScptIdPageBase <= A_ScptId) && ((E_GrP4ScptIdPageBase + E_GrP4DvlPageMaxCnt) > A_ScptId) )
			{
				Tv_PageIdx	=	A_ScptId - E_GrP4ScptIdPageBase;
				if ( NULL != m_PtrPageTbl[Tv_PageIdx].Pos )
				{
					Tv_Result	=	TRUE;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlEditLumpScptUpdt(void)
{
	// local -------------------
		__u16	Tv_ScptId;
		__u16	Tv_Idx;
		void*	Tv_PtrScpt;
		__u32	Tv_ScptSize;
	// code --------------------
		if (!DvlIsDbgRun())
		{
			if (NULL != m_DvlEdtLump)
			{
				Tv_ScptId	=	m_DvlEdtLump->ScptIdGet();
				// check page
				if (DvlIsPageWindow(Tv_ScptId))
				{
					// page
					// get script buffer
					Tv_Idx			=	Tv_ScptId - E_GrP4ScptIdPageBase;
					Tv_PtrScpt	=	m_DvlScptPageTbl[Tv_Idx].Pos;
					if (NULL != Tv_PtrScpt)
					{
						Tv_ScptSize	=	m_DvlEdtLump->RtlScptDump(Tv_PtrScpt, FALSE);
						m_DvlPageInfo[Tv_Idx].Size		=	Tv_ScptSize;
						if (E_GrP4DvlScptBufMax < Tv_ScptSize)
						{
							DbgMsgPrint("Cls_GrP4DvlMng::LcEdtLumpScptUpdt - page ( %d ) script size ( %d ) over!\n", Tv_ScptId, Tv_ScptSize);
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrP4DvlMng::LcEdtLumpScptUpdt - page ( %d ) script buffer not allocated!\n", Tv_ScptId);
					}
				}
				else
				{
					// widow
					// get script buffer
					Tv_Idx			=	Tv_ScptId - E_GrP4ScptIdWinBase;
					Tv_PtrScpt	=	m_DvlScptWinTbl[Tv_Idx].Pos;
					if (NULL != Tv_PtrScpt)
					{
						Tv_ScptSize	=	m_DvlEdtLump->RtlScptDump(Tv_PtrScpt, FALSE);
						m_DvlWinInfo[Tv_Idx].Size		=	Tv_ScptSize;
						if (E_GrP4DvlScptBufMax < Tv_ScptSize)
						{
							DbgMsgPrint("Cls_GrP4DvlMng::LcEdtLumpScptUpdt - window ( %d ) script size ( %d ) over!\n", Tv_ScptId, Tv_ScptSize);
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrP4DvlMng::LcEdtLumpScptUpdt - window ( %d ) script buffer not allocated!\n", Tv_ScptId);
					}
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DvlMng::LcEdtLumpScptUpdt - In debug mode.\n");
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlEditLumpGet(void)
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;
		if ( NULL != m_DvlEdtLump )
		{
			Tv_Result	=	m_DvlEdtLump->ScptIdGet();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjPstDfltBuild( void )
{
	// local -------------------
		__u16	Tv_Idx;
		__u8	Tv_SkinIdx;
		Ptr_GrP4ScptPstData	Tv_PtrPst;
	// code --------------------
		Tv_Idx	=	E_GrP4ScptPstIdDflt - E_GrP4ScptPstIdBase;
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP4DvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			if ( NULL == m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos )
			{
				m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos	=	malloc( E_GrP4DvlPstBufMax );
				if ( NULL != m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos )
				{
					Tv_PtrPst	=	(Ptr_GrP4ScptPstData)m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos;

					Tv_PtrPst[0].Left		=	0;
					Tv_PtrPst[0].Top		=	0;
					Tv_PtrPst[0].Right	=	4;
					Tv_PtrPst[0].Bottom	=	0;
					Tv_PtrPst[0].Basis	=	E_GrP4PosBasisSt | (E_GrP4PosBasisSt << 2) | (E_GrP4PosBasisSt << 4) | (E_GrP4PosBasisEd << 6);
					Tv_PtrPst[0].Type		=	E_GrP4ScptPstTypeLine;
					Tv_PtrPst[0].Para0	=	1;
					Tv_PtrPst[0].Para1	=	0x11;
					Tv_PtrPst[0].Value	=	GrDrawCvtColorByFcc( E_GrFccRGB24, 0xD0D0D0 , m_PtrCfgPrj->FmtFcc );

					Tv_PtrPst[1].Left		=	0;
					Tv_PtrPst[1].Top		=	0;
					Tv_PtrPst[1].Right	=	0;
					Tv_PtrPst[1].Bottom	=	4;
					Tv_PtrPst[1].Basis	=	E_GrP4PosBasisSt | (E_GrP4PosBasisSt << 2) | (E_GrP4PosBasisEd << 4) | (E_GrP4PosBasisSt << 6);
					Tv_PtrPst[1].Type		=	E_GrP4ScptPstTypeLine;
					Tv_PtrPst[1].Para0	=	0;
					Tv_PtrPst[1].Para1	=	0x11;
					Tv_PtrPst[1].Value	=	GrDrawCvtColorByFcc( E_GrFccRGB24, 0xF0F0F0 , m_PtrCfgPrj->FmtFcc );

					Tv_PtrPst[2].Left		=	4;
					Tv_PtrPst[2].Top		=	0;
					Tv_PtrPst[2].Right	=	0;
					Tv_PtrPst[2].Bottom	=	0;
					Tv_PtrPst[2].Basis	=	E_GrP4PosBasisEd | (E_GrP4PosBasisSt << 2) | (E_GrP4PosBasisEd << 4) | (E_GrP4PosBasisEd << 6);
					Tv_PtrPst[2].Type		=	E_GrP4ScptPstTypeLine;
					Tv_PtrPst[2].Para0	=	1;
					Tv_PtrPst[2].Para1	=	0x22;
					Tv_PtrPst[2].Value	=	GrDrawCvtColorByFcc( E_GrFccRGB24, 0x404040 , m_PtrCfgPrj->FmtFcc );

					Tv_PtrPst[3].Left		=	0;
					Tv_PtrPst[3].Top		=	4;
					Tv_PtrPst[3].Right	=	0;
					Tv_PtrPst[3].Bottom	=	0;
					Tv_PtrPst[3].Basis	=	E_GrP4PosBasisSt | (E_GrP4PosBasisEd << 2) | (E_GrP4PosBasisEd << 4) | (E_GrP4PosBasisEd << 6);
					Tv_PtrPst[3].Type		=	E_GrP4ScptPstTypeLine;
					Tv_PtrPst[3].Para0	=	0;
					Tv_PtrPst[3].Para1	=	0x22;
					Tv_PtrPst[3].Value	=	GrDrawCvtColorByFcc( E_GrFccRGB24, 0x202020 , m_PtrCfgPrj->FmtFcc );

					Tv_PtrPst[4].Left		=	0;
					Tv_PtrPst[4].Top		=	0;
					Tv_PtrPst[4].Right	=	0;
					Tv_PtrPst[4].Bottom	=	0;
					Tv_PtrPst[4].Basis	=	E_GrP4PosBasisSt | (E_GrP4PosBasisSt << 2) | (E_GrP4PosBasisEd << 4) | (E_GrP4PosBasisEd << 6);
					Tv_PtrPst[4].Type		=	E_GrP4ScptPstTypeClrFill;
					Tv_PtrPst[4].Para0	=	0;
					Tv_PtrPst[4].Para1	=	0;
					Tv_PtrPst[4].Value	=	GrDrawCvtColorByFcc( E_GrFccRGB24, 0xB0B0B0 , m_PtrCfgPrj->FmtFcc );

					// update script count
					m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Cnt		=	5;

				}
				else
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjPstDfltBuild - mamory allocate failed.\n");
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjPstDfltBuild - paint script buffer not initate.\n");
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlEditSkinSet( __u8 A_SkinIdx )
{
	// local -------------------
		St_GrP4DvlMsgEditSkinSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditSkinSet;
		Tv_Msg.SkinIdx			=	A_SkinIdx;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlEditSkinSet( __u8 A_SkinIdx )
{
	// local -------------------
	// code --------------------
		if ( DvlIsPrjExist() )
		{
			if ( E_GrP4DvlSkinMaxCnt > A_SkinIdx )
			{
				// set skin index
				m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx	=	A_SkinIdx;
				// update table
				m_DvlScptCfgRtl.Skin.ClrCus	=	m_DvlPrjSkin[A_SkinIdx].ClrCus;		// update cursor color

				// rebuild rule
				LcPrjRuleBuild();
				// update font
				LcFontUpdt();
				// redraw all control
				m_ConTbl[0]->PaintAllReq();		// paint all request
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSkinNameBuild( void )
{
	// local -------------------
		__s32		Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlSkinMaxCnt;Tv_WkIdx++ )
		{
			GrStrIntToWstr( m_DvlPrjSkin[Tv_WkIdx].StrName, Tv_WkIdx );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DvlMng::DvlSkinNameGet( __u8 A_SkinIdx )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( DvlIsPrjExist() )
		{
			if ( E_GrP4DvlSkinMaxCnt > A_SkinIdx )
			{
				Tv_Result	=	m_DvlPrjSkin[A_SkinIdx].StrName;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlSkinNameSet( __u8 A_SkinIdx, WCHAR* A_StrName )
{
	// local -------------------
	// code --------------------
		if ( DvlIsPrjExist() )
		{
			if ( E_GrP4DvlSkinMaxCnt > A_SkinIdx )
			{
				GrStrWcopy( m_DvlPrjSkin[A_SkinIdx].StrName, A_StrName );
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrP4DvlMng::DvlEditSkinGet( void )
{
		return	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
}
//--------------------------------------------------------------------
Ptr_GrP4ScptPstData	Cls_GrP4DvlMng::DvlPstCreate( __u16 A_Id )
{
	// local -------------------
		Ptr_GrP4ScptPstData	Tv_Result;
		__u16	Tv_Idx;
		__u8	Tv_SkinIdx;
	// code --------------------
		// check already exist
		Tv_Result	=	NULL;
		if ( (E_GrP4ScptPstIdBase <= A_Id) && ((E_GrP4ScptPstIdBase + E_GrP4DvlPstMaxCnt) > A_Id ) )
		{
			Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
			Tv_Idx	=	A_Id - E_GrP4ScptPstIdBase;
			if ( NULL == m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos )
			{
				m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Cnt	=	0;		// count reset
				m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos	=	malloc( E_GrP4DvlPstBufMax );
				if ( NULL == m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos )
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::DvlPstCreate - pst ( %d ) memory allocate error.\n", A_Id );
				}
				Tv_Result	=	(Ptr_GrP4ScptPstData)m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos;
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4DvlMng::DvlPstCreate - pst ( %d )  is exist.\n", A_Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DvlMng::DvlPstCreate - bad id ( %d ) input.\n", A_Id );
		}
		return	Tv_Result;		
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlPstCodeCountSet( __u16 A_Id, __u8 A_Cnt )
{
	// local -------------------
		__u16	Tv_Idx;
		__u8	Tv_SkinIdx;
	// code --------------------
		if ( E_GrP4DvlPstCodeMax >= A_Cnt )
		{
			if ( (E_GrP4ScptPstIdBase <= A_Id) && ((E_GrP4ScptPstIdBase + E_GrP4DvlPstMaxCnt) > A_Id ) )
			{
				Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
				Tv_Idx			=	A_Id - E_GrP4ScptPstIdBase;
				if ( NULL != m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos )
				{
					m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Cnt	=	A_Cnt;		// update count
					if ( 0 == A_Cnt )
					{
						// release
						free( m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos );
						m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos	=	NULL;
					}
				}
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DvlMng::DvlPstCodeCountSet - try over count ( %d ) set to pst ( %d ).\n" , A_Cnt, A_Id );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DvlMng::DvlPstCodeNameGet( __u16 A_Id )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16		Tv_Idx;
	// code --------------------
		Tv_Result	=	NULL;
		if ( (E_GrP4ScptPstIdBase <= A_Id) && ((E_GrP4ScptPstIdBase + E_GrP4DvlPstMaxCnt) > A_Id ) )
		{
			Tv_Idx	=	A_Id - E_GrP4ScptPstIdBase;
			Tv_Result	=	m_DvlPstInfo[Tv_Idx].StrName;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlPstCodeNameSet( __u16 A_Id, WCHAR* A_StrName )
{
	// local -------------------
		__u16		Tv_Idx;
	// code --------------------
		if ( (E_GrP4ScptPstIdBase <= A_Id) && ((E_GrP4ScptPstIdBase + E_GrP4DvlPstMaxCnt) > A_Id ) )
		{
			Tv_Idx	=	A_Id - E_GrP4ScptPstIdBase;
			GrStrWcopy( m_DvlPstInfo[Tv_Idx].StrName, A_StrName, 31 );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlImgLoadBmp( __u16 A_Id, WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_SkinIdx;
		Cls_GrGdib*		Tv_GdibBmp;
		Cls_GrGdib*		Tv_GdibCvt;
		__u16	Tv_SlotIdx;
		__u32	Tv_SizeGdib;
	// code --------------------
		Tv_Result	=	FALSE;
		// check current id
		if ( (E_GrP4ImgIdBase <= A_Id) && ((E_GrP4ImgIdBase + E_GrP4MngImgScptMaxCnt) > A_Id) )
		{
			//load bmp
			Tv_GdibBmp	=	GrGdibLoadBmp( A_StrFn );
			if ( NULL != Tv_GdibBmp )
			{
				//convert GDIB
				Tv_GdibCvt	=	NULL;
				if ( m_DvlScptCfgPrj.FmtFcc != Tv_GdibBmp->GetFcc() )
				{
					if ( Tv_GdibBmp->IsPalleteUse() )
					{
						Tv_GdibBmp->ChgPalletForamt( m_DvlScptCfgPrj.FmtFcc );
						Tv_GdibCvt	=	Tv_GdibBmp;
					}
					else
					{
						Tv_GdibCvt	=	GrGdibCreate( m_DvlScptCfgPrj.FmtFcc, 
							Tv_GdibBmp->GetResX(), Tv_GdibBmp->GetResY() );
						if ( NULL != Tv_GdibCvt )
						{
							Tv_GdibBmp->CvtTo( Tv_GdibCvt );
						}
						delete	Tv_GdibBmp;
					}
				}
				else
				{
					Tv_GdibCvt	=	Tv_GdibBmp;
				}

				Tv_GdibBmp	=	NULL;			//not use mark
				//find bitmap slot
				if ( NULL != Tv_GdibCvt )
				{
					Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
					Tv_SlotIdx	=	A_Id - E_GrP4ImgIdBase;
					// check already exist image
					if ( NULL != m_DvlScptImgTbl[Tv_SkinIdx][Tv_SlotIdx] )
					{
						// release
						free( m_DvlScptImgTbl[Tv_SkinIdx][Tv_SlotIdx] );
						m_DvlScptImgTbl[Tv_SkinIdx][Tv_SlotIdx]	=	NULL;
					}
					// create
					Tv_SizeGdib	=	Tv_GdibCvt->SaveToMem( NULL );
					m_DvlScptImgTbl[Tv_SkinIdx][Tv_SlotIdx]	=	(Ptr_GrGdib)malloc( Tv_SizeGdib );
					Tv_GdibCvt->SaveToMem( (void*)m_DvlScptImgTbl[Tv_SkinIdx][Tv_SlotIdx] );
					Tv_Result	=	TRUE;
				}
				//release converted gdib
				delete	Tv_GdibCvt;
				// relase image thumbnail
				LcDvlImgTmbFreeById((__u16)A_Id);
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlImgDelete( __u16 A_Id )
{
	// local -------------------
		__u16	Tv_SlotIdx;
		__u8	Tv_SkinIdx;
	// code --------------------
		// check current id
		if ( (E_GrP4ImgIdBase <= A_Id) && ((E_GrP4ImgIdBase + E_GrP4MngImgScptMaxCnt) > A_Id) )
		{
			Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
			Tv_SlotIdx	=	A_Id - E_GrP4ImgIdBase;
			// check exist
			if ( NULL != m_DvlScptImgTbl[Tv_SkinIdx][Tv_SlotIdx] )
			{
				// release
				free( m_DvlScptImgTbl[Tv_SkinIdx][Tv_SlotIdx] );
				m_DvlScptImgTbl[Tv_SkinIdx][Tv_SlotIdx]	=	NULL;
			}
			// release thumbnail
			LcDvlImgTmbFreeById((__u16)A_Id);
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::DvlClrPltGet( __u8 A_ClrIdx )
{
		return	m_DvlPrjRtl.ClrPltTbl[A_ClrIdx];
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlClrPltSet( __u8 A_ClrIdx, __u32 A_Color )
{
		m_DvlPrjRtl.ClrPltTbl[A_ClrIdx]	=	A_Color;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlTxtNameGet( __u16 A_Id, WCHAR* A_StrRt )
{
	// local -------------------
		__u16	Tv_TxtIdx;
		BOOL8	Tv_Isfound;
		__u16	Tv_WkIdx;
		WCHAR*	Tv_StrWk;
	// code --------------------
		// clear string
		GrStrClear( A_StrRt );
		// check id
		if ( E_GrP4ScptTxtIdBase <= A_Id )
		{
			Tv_TxtIdx	=	A_Id - E_GrP4ScptTxtIdBase;
			// check text count
			if ( E_GrP4MngTextScptMaxCnt > Tv_TxtIdx )
			{
				// check system text
				if ( E_GrP4DvlTextSysCnt > Tv_TxtIdx )
				{
					// find system text
					Tv_Isfound	=	FALSE;
					Tv_WkIdx	=	0;
					while( E_GrP4ScptTxtIdNone != V_GrP4DvlTxtSysName[Tv_WkIdx].Id )
					{
						// check text id
						if ( A_Id == V_GrP4DvlTxtSysName[Tv_WkIdx].Id )
						{
							GrStrWcopy( A_StrRt, V_GrP4DvlTxtSysName[Tv_WkIdx].StrName );
							Tv_Isfound	=	TRUE;
							break;
						}
						// next
						Tv_WkIdx ++;
					}
					// check found
					if ( !Tv_Isfound )
					{
						// convert number
						Tv_StrWk	=	GrStrWcopy( A_StrRt, L"[ reserved " );
						GrStrIntToWstr( Tv_StrWk, (__s32)A_Id );
						GrStrWcat( Tv_StrWk, L" ]" );
					}
				}
				else
				{
					// set user text
					Tv_StrWk	=	GrStrWcopy( A_StrRt, L"( " );
					GrStrIntToWstr( Tv_StrWk, (__s32)A_Id );
					GrStrWcat( Tv_StrWk, L" )" );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlTxtSet( __u16 A_Id, __u8 A_LangIdx, WCHAR* A_Str )
{
	// local -------------------
		__u16	Tv_TxtIdx;

	// code --------------------
		// check able id
		if ( (E_GrP4ScptTxtIdBase <= A_Id) && (E_GrP4ScptLangMaxCnt > A_LangIdx) )
		{
			Tv_TxtIdx	=	A_Id - E_GrP4ScptTxtIdBase;
			if ( E_GrP4MngTextScptMaxCnt > Tv_TxtIdx )
			{
				// check exist text
				if ( GrStrIsExist(A_Str) )
				{
					// check exist
					if ( NULL == m_DvlScptTxtTbl[Tv_TxtIdx].LngTbl[A_LangIdx] )
					{
						// allocate
						m_DvlScptTxtTbl[Tv_TxtIdx].LngTbl[A_LangIdx]	=	(WCHAR*)malloc( E_GrP4DvlTextBufSize );
					}
					if ( NULL != m_DvlScptTxtTbl[Tv_TxtIdx].LngTbl[A_LangIdx] )
					{
						GrStrWcopy( m_DvlScptTxtTbl[Tv_TxtIdx].LngTbl[A_LangIdx] , A_Str, E_GrP4DvlTextMaxLen );
					}
					else
					{
						DbgMsgPrint( "Cls_GrP4DvlMng::DvlTxtSet - text ( %d ) buffer allocate error.\n\n", A_Id );
					}
				}
				else
				{
					// release
					if ( NULL != m_DvlScptTxtTbl[Tv_TxtIdx].LngTbl[A_LangIdx] )
					{
						free( m_DvlScptTxtTbl[Tv_TxtIdx].LngTbl[A_LangIdx] );
						m_DvlScptTxtTbl[Tv_TxtIdx].LngTbl[A_LangIdx]	=	NULL;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjPstNameBuild( void )
{
	// local -------------------
		__s32		Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlPstMaxCnt;Tv_WkIdx++)
		{
			GrStrIntToWstr( m_DvlPstInfo[Tv_WkIdx].StrName, Tv_WkIdx + E_GrP4ScptPstIdBase );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlFontDelete( __u8 A_Id )
{
	// local -------------------
		__u8	Tv_SkinIdx;
	// code --------------------
		// check id
		if ( E_GrP4MngFontMaxCnt > A_Id )
		{
			Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
			// release already exist font
			if ( NULL != m_DvlScptFontTbl[Tv_SkinIdx][A_Id] )
			{
				// release
				free( m_DvlScptFontTbl[Tv_SkinIdx][A_Id] );
				m_DvlScptFontTbl[Tv_SkinIdx][A_Id]	=	NULL;
				// update font
				LcFontUpdt();
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlFontLoadFile( __u8 A_Id, WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_SkinIdx;
		__u32	Tv_FileSize;
		Ptr_GrP4FontHead	Tv_PtrFont;
	// code --------------------
		// init
		Tv_Result	=	FALSE;

		// check id
		if ( E_GrP4MngFontMaxCnt > A_Id )
		{
			// init
			Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
			// release already exist font
			if ( NULL != m_DvlScptFontTbl[Tv_SkinIdx][A_Id] )
			{
				// release
				free( m_DvlScptFontTbl[Tv_SkinIdx][A_Id] );
				m_DvlScptFontTbl[Tv_SkinIdx][A_Id]	=	NULL;
			}
			// file load
			Tv_FileSize	=	(__u32)GrFileGetSize( A_StrFn, TRUE );
			if ( 0 != Tv_FileSize )
			{
				// load file
				Tv_PtrFont	=	(Ptr_GrP4FontHead) malloc( Tv_FileSize );
				if ( NULL != Tv_PtrFont )
				{
					// load
					if ( Tv_FileSize == GrFileCtrlRead( A_StrFn, Tv_PtrFont, Tv_FileSize, TRUE ) )
					{
						// check fcc
						if ( E_GrP4FontVer2Id == Tv_PtrFont->Fcc )
						{
							// check size
							if ( GrP4FontCalcSize( Tv_PtrFont ) <= Tv_FileSize )
							{
								// update font
								m_DvlScptFontTbl[Tv_SkinIdx][A_Id]	=	Tv_PtrFont;
								Tv_Result	=	TRUE;
							}
						}
					}
					if ( !Tv_Result )
					{
						// release font
						free( Tv_PtrFont );
					}
				}
			}

			// update 
			LcFontUpdt();
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP4DvlMng::DvlEditModelIdxGet( void )
{
		return	m_DvlPrjRtl.ModelIdx;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlEditModelIdxSet( __u8 A_ModelIdx )
{
	// local -------------------
		St_GrP4DvlMsgEditModelIdxSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditModelIdxSet;
		Tv_Msg.ModelIdx			=	A_ModelIdx;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlEditModelIdxSet( __u8 A_ModelIdx )
{
	// local -------------------
		__u8	Tv_SkinIdx;
	// code --------------------
		// check index over
		if ( E_GrP4DvlModelMaxCnt > A_ModelIdx )
		{
			m_DvlPrjRtl.ModelIdx	=	A_ModelIdx;
			// init
			Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
			// update table
			m_DvlScptCfgRtl.Skin.ClrCus	=	m_DvlPrjSkin[Tv_SkinIdx].ClrCus;		// update cursor color
			// update configuration
			m_PtrCfgMdl	=	&m_DvlPrjMdlAtb[m_DvlPrjRtl.ModelIdx];
			// rebuild rule
			LcPrjRuleBuild();
			// update font
			LcFontUpdt();
			// redraw all control
			m_ConTbl[0]->PaintAllReq();		// paint all request
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlModelCopyTo( __u8 A_TargetIdx )
{
	// local -------------------
	// code --------------------
		// project exist
		if ( DvlIsPrjExist() )
		{
			// check target model number
			if ( E_GrP4DvlModelMaxCnt > A_TargetIdx )
			{
				// check different with edit model
				if ( m_DvlPrjRtl.ModelIdx != A_TargetIdx )
				{
					// copy skin index
					m_DvlPrjModel[A_TargetIdx].SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
					// copy model attribute
					GrDumyCopyMem( &m_DvlPrjMdlAtb[A_TargetIdx], &m_DvlPrjMdlAtb[m_DvlPrjRtl.ModelIdx] , sizeof(St_GrP4ScptCfgMdl) );
					DbgMsgPrint( "Cls_GrP4DvlMng::DvlModelCopyTo - model copy done.\n" );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::DvlModelCopyTo - model can not copy self.\n" );
				}
			}
			else 
			{
				DbgMsgPrint( "Cls_GrP4DvlMng::DvlModelCopyTo - bad target model number.\n" );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DvlMng::DvlModelCopyTo - project not exist\n" );
		}
}
//--------------------------------------------------------------------
void*	Cls_GrP4DvlMng::DvlModelAtbGet( __u8 A_ModelIdx )
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( E_GrP4DvlModelMaxCnt > A_ModelIdx )
		{
			Tv_Result	=	&m_DvlPrjMdlAtb[A_ModelIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrP4DvlMng::DvlModelDbgGet( __u8 A_ModelIdx )
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( E_GrP4DvlModelMaxCnt > A_ModelIdx )
		{
			Tv_Result	=	&m_DvlMdlDblTbl[A_ModelIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DvlMng::DvlModelNameGet( __u8 A_ModelIdx )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( E_GrP4DvlModelMaxCnt > A_ModelIdx )
		{
			Tv_Result	=	m_DvlPrjModel[A_ModelIdx].StrName;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlModelNameSet( __u8 A_ModelIdx, WCHAR* A_StrName )
{
		if ( E_GrP4DvlModelMaxCnt > A_ModelIdx )
		{
			GrStrWcopy( m_DvlPrjModel[A_ModelIdx].StrName, A_StrName, 31 );
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlModelNumberGet( __u8 A_ModelIdx )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrP4DvlModelMaxCnt > A_ModelIdx )
		{
			Tv_Result	=	m_DvlPrjMdlAtb[A_ModelIdx].ModelNum;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlModelNumberSet( __u8 A_ModelIdx, __u16 A_Number )
{
		if ( E_GrP4DvlModelMaxCnt > A_ModelIdx )
		{
			m_DvlPrjMdlAtb[A_ModelIdx].ModelNum	=	A_Number;
		}
}
//-------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveWinDvlInfo( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		// window developed table write
		Tv_DataHd.Type	=	E_GrP4DvlPrjDataWinDvlTbl;
		Tv_DataHd.Size	=	sizeof(m_DvlWinInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlWinInfo, Tv_DataHd.Size );	// write scripte head
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSavePageDvlInfo( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		// page develope table write
		Tv_DataHd.Type	=	E_GrP4DvlPrjDataPageDvlTbl;
		Tv_DataHd.Size	=	sizeof(m_DvlPageInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlPageInfo, Tv_DataHd.Size );	// write scripte head
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveSdaData( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		// SDA data write
		Tv_DataHd.Type	=	E_GrP4DvlPrjDataSdaData;
		Tv_DataHd.Size	=	sizeof(m_SdaBuf);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_SdaBuf, Tv_DataHd.Size );	// write data
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSavePstDvlInfo( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		// PST develop table write
		Tv_DataHd.Type	=	E_GrP4DvlPrjDataPstDvlTbl;
		Tv_DataHd.Size	=	sizeof(m_DvlPstInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlPstInfo, Tv_DataHd.Size );	// write scripte head
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveAsmRmkTbl(void)
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
		__u16	Tv_WkIdx;
	// code --------------------
		// write assembly comment
		Tv_DataHd.Type	=	E_GrP4DvlPrjDataAsmCodeRmk;
		Tv_DataHd.SubIdx	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlAsmMaxCnt;Tv_WkIdx++ )
		{
			// write header
			Tv_DataHd.Id		=	Tv_WkIdx;
			Tv_DataHd.Size	=	sizeof(St_GrP4PrjAsmCodeRmk);
			m_DvlSaved		=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
			m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
			m_DvlSaved		=	m_DvlSaved + m_DvlFc->Write( m_DvlAsmRmkTbl[Tv_WkIdx].StrRmk, Tv_DataHd.Size );	// write script data
			m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveAsmFncCmt(void)
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
		__u16	Tv_WkIdx;
	// code --------------------
		// write assembly function comment
		Tv_DataHd.Type	=	E_GrP4DvlPrjDataAsmCmt;
		Tv_DataHd.SubIdx	=	0;
		for (Tv_WkIdx=0; Tv_WkIdx < E_GrP4DvlAsmMaxCnt; Tv_WkIdx++)
		{
			// write header
			Tv_DataHd.Id		=	Tv_WkIdx;
			Tv_DataHd.Size	=	sizeof(St_GrP4PrjAsmFncCmt);
			m_DvlSaved		=	m_DvlSaved + m_DvlFc->Write(&Tv_DataHd, sizeof(Tv_DataHd));	// write data type
			m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
			m_DvlSaved		=	m_DvlSaved + m_DvlFc->Write(m_DvlAsmFncCmt[Tv_WkIdx].StrCmt, Tv_DataHd.Size);	// write script data
			m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveMdlDbg( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
		__u16	Tv_WkIdx;
	// code --------------------
		// write data
		Tv_DataHd.Type	=	E_GrP4DvlPrjDataMdlDebug;
		Tv_DataHd.SubIdx	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlModelMaxCnt;Tv_WkIdx++ )
		{
			// write header
			Tv_DataHd.Id		=	Tv_WkIdx;
			Tv_DataHd.Size	=	sizeof(St_GrP4PrjDbgInfo);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
			m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlMdlDblTbl[Tv_WkIdx], Tv_DataHd.Size );	// write data
			m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveBldFile( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4DvlPrjDataBldFile;
		Tv_DataHd.SubIdx	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlModelMaxCnt;Tv_WkIdx++ )
		{
			// write header
			Tv_DataHd.Id		=	Tv_WkIdx;
			Tv_DataHd.Size	=	sizeof(St_GrP4PrjBldFile);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
			m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlBldInfo[Tv_WkIdx], Tv_DataHd.Size );	// write data
			m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveImage( void )
{
	// local -------------------
		__u8	Tv_SkinIdx;
	// code --------------------
		// script data write
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP4DvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			LcScptSaveImgData( Tv_SkinIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveFont( void )
{
	// local -------------------
		__u8	Tv_SkinIdx;
	// code --------------------
		// script data write
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP4DvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			LcScptSaveFontData( Tv_SkinIdx );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlPrjSave( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP4DvlMsgPrjSave	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdPrjSave;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.StrFn				=	A_StrFn;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSavePrjCfg( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4DvlPrjDataConfig;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlPrjConfig);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlPrjConfig, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSavePrjHead( void )
{
	// local -------------------
		St_GrP4PrjHdInfo	Tv_Hd;
	// code --------------------
		Tv_Hd.Fcc		=	E_GrP4DvlPrjFcc;
		Tv_Hd.Ver		=	E_GrP4DvlPrjVer;

		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_Hd, sizeof(Tv_Hd) );
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_Hd);
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSavePrjRtl( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4DvlPrjDataRuntime;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlPrjRtl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlPrjRtl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSavePrjModel( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4DvlPrjDataModel;
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlModelMaxCnt;Tv_WkIdx++ )
		{
			Tv_DataHd.Id			=	Tv_WkIdx;
			Tv_DataHd.SubIdx	=	0;
			Tv_DataHd.Size		=	sizeof(St_GrP4PrjModelCfg);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
			m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlPrjModel[Tv_WkIdx], Tv_DataHd.Size );	// write script
			m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSavePrjSkin( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4DvlPrjDataSkin;
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlSkinMaxCnt;Tv_WkIdx++ )
		{
			Tv_DataHd.Id			=	Tv_WkIdx;
			Tv_DataHd.SubIdx	=	0;
			Tv_DataHd.Size		=	sizeof(St_GrP4PrjSkinCfg);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
			m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlPrjSkin[Tv_WkIdx], Tv_DataHd.Size );	// write script
			m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSavePrjMdlAtb( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlModelMaxCnt;Tv_WkIdx++)
		{
			LcScptSaveMdlAtb( Tv_WkIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveFolderInfo( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type	=	E_GrP4DvlPrjDataFolderInfo;
		Tv_DataHd.Size	=	sizeof(m_DvlFoldTbl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlFoldTbl, Tv_DataHd.Size );	// write script head
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSavePstTable( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlSkinMaxCnt;Tv_WkIdx++)
		{
			LcScptSavePstTbl( Tv_WkIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSavePstCode( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlSkinMaxCnt;Tv_WkIdx++)
		{
			LcScptSavePstScpt( Tv_WkIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveAsmDvlInfo( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type	=	E_GrP4DvlPrjDataAsmInfo;
		Tv_DataHd.Size	=	sizeof(m_DvlAsmInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlAsmInfo, Tv_DataHd.Size );	// write script head
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::LcDvlPrjSave( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		m_DvlSaved		=	0;
		m_DvlSaveReq	=	0;
		// check project exist
		if ( DvlIsPrjExist() )
		{
			// delete exist file
			GrFileDelete(A_StrFn);
			// open file
			m_DvlFc	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, TRUE, TRUE, FALSE, TRUE );
			if ( m_DvlFc->IsOpened() )		// check file open success
			{
				// write project header
				LcPrjSavePrjHead();
				// project config
				LcPrjSavePrjCfg();
				// proejct runtime
				LcPrjSavePrjRtl();
				// project model
				LcPrjSavePrjModel();
				// project skin
				LcPrjSavePrjSkin();
				// project model attribute
				LcPrjSavePrjMdlAtb();
				// write script config project
				LcScptSaveCfgPrj();
				// write script config runtime
				LcScptSaveCfgRtl();
				// write folder info
				LcPrjSaveFolderInfo();
				// basic setting
				LcPrjSaveSetBase();
				// setup variable write
				LcPrjSaveSetupVar();
				// global variable user write
				LcPrjSaveGvu();
				// model debug data write
				LcPrjSaveMdlDbg();
				// build file
				LcPrjSaveBldFile();
				// SDA data
				LcPrjSaveSdaData();
				// window information table
				LcScptSaveWinInfo();
				// window develop info table
				LcPrjSaveWinDvlInfo();
				// window script write
				LcScptSaveWinScpt();
				// page script info
				LcScptSavePageInfo();
				// page develop info
				LcPrjSavePageDvlInfo();
				// page script
				LcScptSavePageScpt();
				// PST info
				LcPrjSavePstTable();
				// paint develop info
				LcPrjSavePstDvlInfo();
				// paint script
				LcPrjSavePstCode();
				// image
				LcPrjSaveImage();
				// font
				LcPrjSaveFont();
				// ASM Devlop Info
				LcPrjSaveAsmDvlInfo();
				// ASM info
				LcScptSaveAsmTbl();
				// ASM Script code
				LcScptSaveAsmScpt();
				// ASM comment
				LcPrjSaveAsmFncCmt();
				// ASM remark
				LcPrjSaveAsmRmkTbl();
				// text
				LcScptSaveTxtData();
				// user registry
				LcPrjSaveUrgst();
				// registry runtime
				LcPrjSaveRgstRtl();

				// check success
				if ( m_DvlSaved == m_DvlSaveReq )
				{
					Tv_Result	=	TRUE;
					DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlPrjSave - project file %d bytes saved.\n" , m_DvlSaved );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlPrjSave - project file save requested as %d bytes, but %d bytes saved.\n", m_DvlSaveReq, m_DvlSaved );
				}
				
			}
			delete	m_DvlFc;
			m_DvlFc	=	NULL;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadOldScptHd( __u32 A_Size )
{
	// local -------------------
		Ptr_GrP4DvlScptHeadOld	Tv_PtrHd;
	// code --------------------

		if ( sizeof(St_GrP4DvlScptHeadOld) >= A_Size )
		{
			if ( sizeof(St_GrP4DvlScptHeadOld) != A_Size )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadOldScptHd - (warnning) size miss match !\n");
			}

			// memory allocate
			Tv_PtrHd	=	(Ptr_GrP4DvlScptHeadOld)malloc( sizeof(St_GrP4DvlScptHeadOld) );
			if ( NULL != Tv_PtrHd )
			{
				// read
				if ( sizeof(St_GrP4DvlScptHeadOld) == m_DvlFc->Read( Tv_PtrHd, sizeof(St_GrP4DvlScptHeadOld) ) )
				{
					// script project realign
					m_DvlScptCfgPrj.FmtFcc		=	Tv_PtrHd->CfgPrj.FmtFcc;
					m_DvlScptCfgPrj.CusImgId	=	Tv_PtrHd->CfgPrj.CusImgId;
					//m_DvlScptCfgPrj.HwType		=	Tv_PtrHd->CfgPrj.HwType;
					m_DvlScptCfgPrj.UartCnt		=	Tv_PtrHd->CfgPrj.UartCnt;
					//m_DvlScptCfgPrj.UiResId		=	Tv_PtrHd->CfgPrj.UiResId;
					m_DvlScptCfgPrj.FwVer			=	Tv_PtrHd->CfgPrj.FwVer;
					GrDumyCopyMem( m_DvlScptCfgPrj.VkbdTbl, Tv_PtrHd->CfgPrj.VkbdTbl, (sizeof(St_GrP4ScptVkbdLang) * E_GrP4ScptLangMaxCnt) );

					// runtime realign
					m_DvlScptCfgRtl.IsChgConAble	=	Tv_PtrHd->CfgRtl.IsChgConAble;
					GrDumyCopyMem( &m_DvlScptCfgRtl.Skin, &Tv_PtrHd->CfgRtl.Skin, sizeof(St_GrP4ScptCfgSkin) );
				}
				else
				{
					m_DvlIsLoading	=	FALSE;
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadOldScptHd - read file error!\n");
				}
				// release buffer
				free( Tv_PtrHd );
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadOldScptHd - work memory allocate fail!\n");
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadScpt - script header size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadFolderTbl( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlFoldTbl) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlFoldTbl, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadFolderTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadFolderTbl - folder table size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadWinScptTbl( __u32 A_Size )
{
	// local -------------------
		__u16	Tv_WkIdx;
	// code --------------------
		if ( sizeof(m_DvlScptWinTbl) >= A_Size )
		{
			if ( A_Size == m_DvlFc->Read( m_DvlScptWinTbl, A_Size ) )
			{
				// patch table
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlWinMaxCnt;Tv_WkIdx++ )
				{
					m_DvlScptWinTbl[Tv_WkIdx].Pos		=	NULL;		// reset pointer
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadWinScptTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadWinScptTbl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPageScptTbl( __u32 A_Size )
{
	// local -------------------
		__u16	Tv_WkIdx;
	// code --------------------
		if ( sizeof(m_DvlScptPageTbl) >= A_Size )
		{
			if ( A_Size == m_DvlFc->Read( m_DvlScptPageTbl, A_Size ) )
			{
				// patch table
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlPageMaxCnt;Tv_WkIdx++ )
				{
					m_DvlScptPageTbl[Tv_WkIdx].Pos		=	NULL;		// reset pointer
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPageScptTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPageScptTbl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadWinDvlTbl( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlWinInfo) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlWinInfo, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadWinDvlTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadWinDvlTbl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPageDvlTbl( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlPageInfo) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlPageInfo, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPageDvlTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPageDvlTbl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadSdaData( __u32 A_Size )
{
	// local -------------------
		__u32	Tv_Size;
		__u64	Tv_Pos;
	// code --------------------
		Tv_Size	=	sizeof(m_SdaBuf);
		if ( Tv_Size != A_Size )
		{
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadSdaData - data size mismatch!\n");
			if ( Tv_Size > A_Size )
			{
				Tv_Size	=	A_Size;
			}
			else
			{
				// save position
				Tv_Pos	=	(__u64)m_DvlFc->Seek( 0, E_GrFileCtrlPosNow );
			}
		}
		// read SDA data
		if ( Tv_Size != m_DvlFc->Read( m_SdaBuf, Tv_Size ) )
		{
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadSdaData - read file error!\n");
			m_DvlIsLoading	=	FALSE;
		}
		// remain data load
		if ( sizeof(m_SdaBuf) < A_Size )
		{
			Tv_Pos	=	Tv_Pos + (__u64)A_Size;
			m_DvlFc->Seek( Tv_Pos );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadWinScptData( __u32 A_Size, __u16 A_Id )
{
	// local -------------------
	// code --------------------
		// check data size
		if ( E_GrP4DvlScptBufMax >= A_Size )
		{
			// check id range
			if ( E_GrP4DvlWinMaxCnt > A_Id )
			{
				// check already allocated
				if ( NULL == m_DvlScptWinTbl[A_Id].Pos )
				{
					// allocate script buffer
					m_DvlScptWinTbl[A_Id].Pos	=	(void*)malloc( E_GrP4DvlScptBufMax );
					if ( NULL != m_DvlScptWinTbl[A_Id].Pos )
					{
						// read
						if ( A_Size == m_DvlFc->Read( m_DvlScptWinTbl[A_Id].Pos, A_Size ) )
						{
							// check size missmatch
							if ( A_Size != m_DvlWinInfo[A_Id].Size )
							{
								DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadWinScptData - warnning - script ( %d ) size mismatch!\n", A_Id + E_GrP4ScptIdWinBase );
								m_DvlWinInfo[A_Id].Size	=	A_Size;
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadWinScptData - read file error!\n");
							m_DvlIsLoading	=	FALSE;
						}
						// check failure
						if ( !m_DvlIsLoading )
						{
							// release memory
							free( m_DvlScptWinTbl[A_Id].Pos );
							m_DvlScptWinTbl[A_Id].Pos	=	NULL;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadWinScptData - script buffer allocate error.\n");
						m_DvlIsLoading	=	FALSE;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadWinScptData - script ( %d ) overrun.\n" , A_Id + E_GrP4ScptIdWinBase );
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadWinScptData - bad input id ( %d )!\n", A_Id + E_GrP4ScptIdWinBase );
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadWinScptData - script( %d ) was big!\n", A_Id + E_GrP4ScptIdWinBase );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPageScptData( __u32 A_Size, __u16 A_Id )
{
	// local -------------------
	// code --------------------
		// check data size
		if ( E_GrP4DvlScptBufMax >= A_Size )
		{
			// check id range
			if ( E_GrP4DvlPageMaxCnt > A_Id )
			{
				// check already allocated
				if ( NULL == m_DvlScptPageTbl[A_Id].Pos )
				{
					// allocate script buffer
					m_DvlScptPageTbl[A_Id].Pos	=	(void*)malloc( E_GrP4DvlScptBufMax );
					if ( NULL != m_DvlScptPageTbl[A_Id].Pos )
					{
						// read
						if ( A_Size == m_DvlFc->Read( m_DvlScptPageTbl[A_Id].Pos, A_Size ) )
						{
							// check size mismatch
							if ( A_Size != m_DvlPageInfo[A_Id].Size )
							{
								DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPageScptData - warnning - script ( %d ) size mismatch!\n", A_Id + E_GrP4ScptIdPageBase );
								m_DvlPageInfo[A_Id].Size	=	A_Size;
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPageScptData - read file error!\n");
							m_DvlIsLoading	=	FALSE;
						}
						// check failure
						if ( !m_DvlIsLoading )
						{
							// release memory
							free( m_DvlScptPageTbl[A_Id].Pos );
							m_DvlScptPageTbl[A_Id].Pos	=	NULL;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPageScptData - script buffer allocate error.\n");
						m_DvlIsLoading	=	FALSE;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPageScptData - script ( %d ) overrun.\n" , A_Id + E_GrP4ScptIdPageBase );
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPageScptData - bad input id ( %d )!\n", A_Id + E_GrP4ScptIdPageBase );
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPageScptData - script( %d ) was big!\n", A_Id + E_GrP4ScptIdPageBase );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPstScptTblWhole( __u32 A_Size )
{
	// local -------------------
		__u16	Tv_WkIdx;
		__u8	Tv_SkinIdx;
	// code --------------------
		if ( sizeof(m_DvlScptPstTbl) >= A_Size )
		{
			if ( A_Size == m_DvlFc->Read( m_DvlScptPstTbl, A_Size ) )
			{
				for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP4DvlSkinMaxCnt;Tv_SkinIdx++ )
				{
					for( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlPstMaxCnt;Tv_WkIdx++ )
					{
						m_DvlScptPstTbl[Tv_SkinIdx][Tv_WkIdx].Pos	=	NULL;		// clear pointer
					}
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstScptTblWhole - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstScptTblWhole - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPstScptTblOne( __u32 A_Size, __u16 A_SkinIdx )
{
	// local -------------------
		__u16	Tv_WkIdx;
	// code --------------------
		if ( (sizeof(St_GrP4ScptPstItm) * E_GrP4DvlPstMaxCnt) >= A_Size )
		{
			if ( E_GrP4DvlSkinMaxCnt > A_SkinIdx )
			{
				if ( A_Size == m_DvlFc->Read( m_DvlScptPstTbl[A_SkinIdx], A_Size ) )
				{
					// pointer reset
					for( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlPstMaxCnt;Tv_WkIdx++ )
					{
						m_DvlScptPstTbl[A_SkinIdx][Tv_WkIdx].Pos	=	NULL;		// clear pointer
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstScptTblOne - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstScptTblOne - bad skin index ( %d )!\n", A_SkinIdx);
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstScptTblOne - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadAsmScptTbl( __u32 A_Size )
{
	// local -------------------
		__u16	Tv_WkIdx;
	// code --------------------
		if ( sizeof(m_DvlScptAsmTbl) >= A_Size )
		{
			if ( A_Size == m_DvlFc->Read( m_DvlScptAsmTbl, A_Size ) )
			{
				for( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlAsmMaxCnt;Tv_WkIdx++ )
				{
					m_DvlScptAsmTbl[Tv_WkIdx].Pos	=	NULL;		// clear pointer
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmScptTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmScptTbl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPstDvlTbl( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlPstInfo) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlPstInfo, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstDvlTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstDvlTbl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadAsmInfo( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlAsmInfo) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlAsmInfo, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmInfo - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmInfo - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPstScptData( __u32 A_Size, __u16 A_Id, __u8 A_SkinIdx )
{
	// local -------------------
	// code --------------------
		// check data size
		if ( E_GrP4DvlPstBufMax >= A_Size )
		{
			// check id range
			if ( E_GrP4DvlPstMaxCnt > A_Id )
			{
				// check skin range
				if ( E_GrP4DvlSkinMaxCnt > A_SkinIdx )
				{
					// check already allocated
					if ( NULL == m_DvlScptPstTbl[A_SkinIdx][A_Id].Pos )
					{
						// allocate script buffer
						m_DvlScptPstTbl[A_SkinIdx][A_Id].Pos	=	(void*)malloc( E_GrP4DvlPstBufMax );
						if ( NULL != m_DvlScptPstTbl[A_SkinIdx][A_Id].Pos )
						{
							// read
							if ( A_Size == m_DvlFc->Read( m_DvlScptPstTbl[A_SkinIdx][A_Id].Pos, A_Size ) )
							{
								// check size missmatch
								if ( A_Size != ( sizeof(St_GrP4ScptPstData) * m_DvlScptPstTbl[A_SkinIdx][A_Id].Cnt ) )
								{
									DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstScptData - warnning - pst ( %d ) size mismatch!\n", A_Id + E_GrP4ScptPstIdBase );
								}
							}
							else
							{
								DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstScptData - read file error!\n");
								m_DvlIsLoading	=	FALSE;
							}
							// check failure
							if ( !m_DvlIsLoading )
							{
								// release memory
								free( m_DvlScptPstTbl[A_SkinIdx][A_Id].Pos );
								m_DvlScptPstTbl[A_SkinIdx][A_Id].Pos	=	NULL;
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstScptData - pst buffer allocate error.\n");
							m_DvlIsLoading	=	FALSE;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstScptData - pst ( %d ) overrun.\n" , A_Id + E_GrP4ScptPstIdBase );
						m_DvlIsLoading	=	FALSE;
					}


				}
				else
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstScptData - skin index ( %d ) of script ( %d ) is bad.\n" , A_SkinIdx , A_Id + E_GrP4ScptPstIdBase );
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstScptData - bad input id ( %d )!\n", A_Id + E_GrP4ScptPstIdBase );
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPstScptData - pst ( %d ) was big!\n", A_Id + E_GrP4ScptPstIdBase );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadAsmData( __u32 A_Size, __u16 A_Id )
{
	// local -------------------
	// code --------------------
		// check data size
		if ( E_GrP4DvlAsmBufMax >= A_Size )
		{
			// check id range
			if ( E_GrP4DvlAsmMaxCnt > A_Id )
			{
				// check already allocated
				if ( NULL == m_DvlScptAsmTbl[A_Id].Pos )
				{
					// allocate assembly buffer
					m_DvlScptAsmTbl[A_Id].Pos	=	(void*)malloc( E_GrP4DvlAsmBufMax );
					if ( NULL != m_DvlScptAsmTbl[A_Id].Pos )
					{
						// read
						if ( A_Size == m_DvlFc->Read( m_DvlScptAsmTbl[A_Id].Pos, A_Size ) )
						{
							// check size missmatch
							if ( A_Size != ( E_GrP4AsmOpUnitSize * m_DvlScptAsmTbl[A_Id].Cnt ) )
							{
								DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmData - warnning - assembly ( %d ) size mismatch!\n", A_Id + E_GrP4AsmIdBase );
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmData - read file error!\n");
							m_DvlIsLoading	=	FALSE;
						}
						// check failure
						if ( !m_DvlIsLoading )
						{
							// release memory
							free( m_DvlScptAsmTbl[A_Id].Pos );
							m_DvlScptAsmTbl[A_Id].Pos	=	NULL;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmData - assembl buffer allocate error.\n");
						m_DvlIsLoading	=	FALSE;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmData - assembly data ( %d ) overrun.\n" , A_Id + E_GrP4AsmIdBase );
					m_DvlIsLoading	=	FALSE;
				}


			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmData - bad input id ( %d )!\n", A_Id + E_GrP4AsmIdBase );
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmData - asm ( %d ) was big!\n", A_Id + E_GrP4AsmIdBase );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadMdlDbg( __u32 A_Size, __u16 A_Id )
{
	// local -------------------
	// code --------------------
		// check size
		if ( sizeof(St_GrP4PrjDbgInfo) >= A_Size )
		{
			// check model index
			if ( E_GrP4DvlModelMaxCnt > A_Id )
			{
				// read
				if ( A_Size != m_DvlFc->Read( &m_DvlMdlDblTbl[A_Id], A_Size ) )
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadMdlDbg - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadMdlDbg - bad model index ( %d )!\n", A_Id );
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadMdlDbg - model debug ( %d ) was big!\n", A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadBldFile( __u32 A_Size, __u16 A_Id )
{
	// local -------------------
		__u32	Tv_SkipSize;
		__u32	Tv_ReadSize;
	// code --------------------
		Tv_SkipSize	=	0;
		Tv_ReadSize	=	A_Size;
		// check size
		if (sizeof(St_GrP4PrjBldFile) < Tv_ReadSize)
		{
			Tv_ReadSize	=	sizeof(St_GrP4PrjBldFile);
			Tv_SkipSize	=	A_Size - sizeof(St_GrP4PrjBldFile);
		}

		// check model index
		if (E_GrP4DvlModelMaxCnt > A_Id)
		{
			// read
			if (Tv_ReadSize != m_DvlFc->Read(&m_DvlBldInfo[A_Id], Tv_ReadSize))
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadBldFile - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadBldFile - bad model index ( %d )!\n", A_Id);
		}

		if (m_DvlIsLoading && (0 != Tv_SkipSize))
		{
			m_DvlFc->Seek((__s64)Tv_SkipSize, E_GrFileCtrlPosNow);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadAsmCmt( __u32 A_Size, __u16 A_Id )
{
	// local -------------------
	// code --------------------
		// check size
		if (sizeof(St_GrP4PrjAsmFncCmt) >= A_Size)
		{
			// check model index
			if (E_GrP4DvlAsmMaxCnt > A_Id)
			{
				// read
				if (A_Size != m_DvlFc->Read(m_DvlAsmFncCmt[A_Id].StrCmt, A_Size))
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmCmt - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmCmt - bad code index ( %d )!\n", A_Id + E_GrP4AsmIdBase);
			}
		}
		else
		{
			//m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadAsmCmt - assembly remark ( %d ) was big!maybe old data\n", A_Id);
			m_DvlFc->Seek((__u64)A_Size, E_GrFileCtrlPosNow);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPrjAsmRmkTbl(__u32 A_Size, __u16 A_Id)
{
	// local -------------------
	// code --------------------

		// check size
		if (sizeof(St_GrP4PrjAsmCodeRmk) >= A_Size)
		{
			// check model index
			if (E_GrP4DvlAsmMaxCnt > A_Id)
			{
				// read
				if (A_Size != m_DvlFc->Read(m_DvlAsmRmkTbl[A_Id].StrRmk, A_Size))
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjAsmRmkTbl - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjAsmRmkTbl - bad code index ( %d )!\n", A_Id + E_GrP4AsmIdBase);
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjAsmRmkTbl - assembly remark ( %d ) was big!\n", A_Id);
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadImgData( __u32 A_Size, __u16 A_Id, __u8 A_SkinIdx )
{
	// local -------------------
	// code --------------------
		// check id range
		if ( E_GrP4MngImgScptMaxCnt > A_Id )
		{
			// check skin range
			if ( E_GrP4DvlSkinMaxCnt > A_SkinIdx )
			{
				// check already allocated
				if ( NULL == m_DvlScptImgTbl[A_SkinIdx][A_Id] )
				{
					// allocate script buffer
					m_DvlScptImgTbl[A_SkinIdx][A_Id]	=	(Ptr_GrGdib)malloc( A_Size );
					if ( NULL != m_DvlScptImgTbl[A_SkinIdx][A_Id] )
					{
						// read
						if ( A_Size == m_DvlFc->Read( m_DvlScptImgTbl[A_SkinIdx][A_Id], A_Size ) )
						{
							// check size missmatch
							if ( A_Size !=  m_DvlScptImgTbl[A_SkinIdx][A_Id]->Size )
							{
								DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadImgData - image ( %d ) size mismatch!\n", A_Id + E_GrP4ImgIdBase );
								m_DvlIsLoading	=	FALSE;
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadImgData - read file error!\n");
							m_DvlIsLoading	=	FALSE;
						}
						// check failure
						if ( !m_DvlIsLoading )
						{
							// release memory
							free( m_DvlScptImgTbl[A_SkinIdx][A_Id] );
							m_DvlScptImgTbl[A_SkinIdx][A_Id]	=	NULL;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadImgData - image buffer allocate error.\n");
						m_DvlIsLoading	=	FALSE;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadImgData - image ( %d ) overrun.\n" , A_Id + E_GrP4ImgIdBase );
					m_DvlIsLoading	=	FALSE;
				}


			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadImgData - skin index ( %d ) of script ( %d ) is bad.\n" , A_SkinIdx , A_Id + E_GrP4ImgIdBase );
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadImgData - bad input id ( %d )!\n", A_Id + E_GrP4ImgIdBase );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadFontData( __u32 A_Size, __u16 A_Id, __u8 A_SkinIdx )
{
	// local -------------------
	// code --------------------
		// check id range
		if ( E_GrP4MngFontMaxCnt > A_Id )
		{
			// check skin range
			if ( E_GrP4DvlSkinMaxCnt > A_SkinIdx )
			{
				// check already allocated
				if ( NULL == m_DvlScptFontTbl[A_SkinIdx][A_Id] )
				{
					// allocate script buffer
					m_DvlScptFontTbl[A_SkinIdx][A_Id]	=	(Ptr_GrP4FontHead)malloc( A_Size );
					if ( NULL != m_DvlScptFontTbl[A_SkinIdx][A_Id] )
					{
						// read
						if ( A_Size == m_DvlFc->Read( m_DvlScptFontTbl[A_SkinIdx][A_Id], A_Size ) )
						{
							// check size missmatch
							if ( A_Size !=  GrP4FontCalcSize( m_DvlScptFontTbl[A_SkinIdx][A_Id] ) )
							{
								DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadFontData - font ( %d ) size mismatch!\n", A_Id );
								m_DvlIsLoading	=	FALSE;
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadFontData - read file error!\n");
							m_DvlIsLoading	=	FALSE;
						}
						// check failure
						if ( !m_DvlIsLoading )
						{
							// release memory
							free( m_DvlScptFontTbl[A_SkinIdx][A_Id] );
							m_DvlScptFontTbl[A_SkinIdx][A_Id]	=	NULL;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadFontData - font buffer allocate error.\n");
						m_DvlIsLoading	=	FALSE;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadFontData - font ( %d ) overrun.\n" , A_Id );
					m_DvlIsLoading	=	FALSE;
				}


			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadFontData - skin index ( %d ) of font ( %d ) is bad.\n" , A_SkinIdx , A_Id );
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadFontData - bad input id ( %d )!\n", A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadTextData( __u32 A_Size, __u16 A_Id, __u8 A_LngIdx )
{
	// local -------------------
		WCHAR*	Tv_StrWk;
	// code --------------------
		// check size
		if ( E_GrP4DvlTextBufSize >= (A_Size + 2) )
		{
			// check id range
			if ( E_GrP4MngTextScptMaxCnt > A_Id )
			{
				// check language range
				if ( E_GrP4ScptLangMaxCnt > A_LngIdx )
				{
					// check already allocated
					if ( NULL == m_DvlScptTxtTbl[A_Id].LngTbl[A_LngIdx] )
					{
						// allocate script buffer
						m_DvlScptTxtTbl[A_Id].LngTbl[A_LngIdx]	=	(WCHAR*)malloc( E_GrP4DvlTextBufSize );
						if ( NULL != m_DvlScptTxtTbl[A_Id].LngTbl[A_LngIdx] )
						{
							// read
							if ( A_Size == m_DvlFc->Read( m_DvlScptTxtTbl[A_Id].LngTbl[A_LngIdx], A_Size ) )
							{
								// mark eol
								Tv_StrWk	=	m_DvlScptTxtTbl[A_Id].LngTbl[A_LngIdx];
								Tv_StrWk[ A_Size >> 1 ]	=	0;
							}
							else
							{
								DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadTextData - read file error!\n");
								m_DvlIsLoading	=	FALSE;
							}
							// check failure
							if ( !m_DvlIsLoading )
							{
								// release memory
								free( m_DvlScptTxtTbl[A_Id].LngTbl[A_LngIdx] );
								m_DvlScptTxtTbl[A_Id].LngTbl[A_LngIdx]	=	NULL;
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadTextData - text buffer allocate error.\n");
							m_DvlIsLoading	=	FALSE;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadTextData - text ( %d ) of language ( %d ) overrun.\n" , A_Id + E_GrP4ScptTxtIdBase, A_LngIdx  );
						m_DvlIsLoading	=	FALSE;
					}

				}
				else
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadTextData - language index ( %d ) of text ( %d ) is bad.\n" , A_LngIdx , A_Id + E_GrP4ScptTxtIdBase );
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadTextData - bad input id ( %d )!\n", A_Id + E_GrP4ScptTxtIdBase );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadTextData - text ( %d ) size was big.\n" , A_Id + E_GrP4ScptTxtIdBase );
			m_DvlIsLoading	=	FALSE;
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlPrjLoad( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP4DvlMsgPrjLoad	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdPrjLoad;
		Tv_Msg.StrFn				=	A_StrFn;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::LcPrjLoadOldPrjHead( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		Ptr_GrP4PrjOldHead	Tv_PtrHd;
		__u32	Tv_CpSize;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrHd	=	(Ptr_GrP4PrjOldHead)malloc( sizeof(St_GrP4PrjOldHead) );
		if ( NULL != Tv_PtrHd )
		{
			// load
			if ( sizeof(St_GrP4PrjOldHead) == m_DvlFc->Read( Tv_PtrHd, sizeof(St_GrP4PrjOldHead) ) )
			{
				// config copy
				Tv_CpSize	=	sizeof(m_DvlPrjConfig);
				if ( sizeof(St_GrP4PrjOldHead) < Tv_CpSize  )
				{
					Tv_CpSize	=	sizeof(St_GrP4PrjOldHead);
				}
				GrDumyCopyMem( &m_DvlPrjConfig, &Tv_PtrHd->Config, Tv_CpSize );

				// runtime copy
				m_DvlPrjRtl.ModelIdx	=	Tv_PtrHd->Rtl.ModelIdx;
				GrDumyCopyMem( m_DvlPrjRtl.ClrPltTbl, Tv_PtrHd->Rtl.ClrPltTbl, E_GrP4DvlColorPltCnt << 2 );

				// model
				for (Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlModelMaxCnt;Tv_WkIdx++)
				{
					GrStrWcopy( m_DvlPrjModel[Tv_WkIdx].StrName, Tv_PtrHd->Model[Tv_WkIdx].StrName );
					m_DvlPrjModel[Tv_WkIdx].SkinIdx	=	Tv_PtrHd->Model[Tv_WkIdx].SkinIdx;
				}

				// skin
				Tv_CpSize	=	sizeof(St_GrP4PrjSkinCfg);
				if ( sizeof(St_GrP4PrjOldSkinCfg) < Tv_CpSize  )
				{
					Tv_CpSize	=	sizeof(St_GrP4PrjOldSkinCfg);
				}
				for (Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlSkinMaxCnt;Tv_WkIdx++)
				{
					GrDumyCopyMem( &m_DvlPrjSkin[Tv_WkIdx], &Tv_PtrHd->Skin[Tv_WkIdx], Tv_CpSize );
				}

				// model attribute
				Tv_CpSize	=	sizeof(St_GrP4ScptCfgMdl);
				if ( E_GrP4DvlOldScptCfgMdlSize < Tv_CpSize  )
				{
					Tv_CpSize	=	E_GrP4DvlOldScptCfgMdlSize;
				}
				for (Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlModelMaxCnt;Tv_WkIdx++)
				{
					GrDumyCopyMem( &m_DvlPrjMdlAtb[Tv_WkIdx], Tv_PtrHd->MdlAtb[Tv_WkIdx], Tv_CpSize );
				}

				// success
				Tv_Result	=	TRUE;
			}

			// release
			free( Tv_PtrHd );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadScptCfgPrj( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlScptCfgPrj) >= A_Size )
		{
			GrDumyZeroMem(&m_DvlScptCfgPrj, sizeof(m_DvlScptCfgPrj));
			if ( A_Size != m_DvlFc->Read( &m_DvlScptCfgPrj, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadScptCfgPrj - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadScptCfgPrj - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadScptCfgRtl( __u32 A_Size )
{
		if ( sizeof(m_DvlScptCfgRtl) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( &m_DvlScptCfgRtl, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadScptCfgRtl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadScptCfgRtl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadMdlAtb( __u32 A_Size, __u16 A_ModelIdx )
{
	// local -------------------
	// code --------------------
		if ( E_GrP4DvlModelMaxCnt > A_ModelIdx )
		{
			if ( sizeof(St_GrP4ScptCfgMdl) >= A_Size )
			{
				if ( A_Size != m_DvlFc->Read( &m_DvlPrjMdlAtb[A_ModelIdx], A_Size ) )
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadMdlAtb - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadMdlAtb - data size was big!\n");
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadMdlAtb - bad model index ( %d )!\n" , A_ModelIdx);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPrjConfig( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlPrjConfig) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( &m_DvlPrjConfig, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjConfig - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjConfig - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPrjRuntime( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlPrjRtl) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( &m_DvlPrjRtl, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjRuntime - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjRuntime - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPrjModel( __u32 A_Size, __u16 A_ModelIdx )
{
	// local -------------------
	// code --------------------
		if ( E_GrP4DvlModelMaxCnt > A_ModelIdx )
		{
			if ( sizeof(St_GrP4PrjModelCfg) >= A_Size )
			{
				if ( A_Size != m_DvlFc->Read( &m_DvlPrjModel[A_ModelIdx], A_Size ) )
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjModel - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjModel - data size was big!\n");
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjModel - bad model index ( %d )!\n" , A_ModelIdx);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPrjSkin( __u32 A_Size, __u16 A_SkinIdx )
{
	// local -------------------
	// code --------------------
		if ( E_GrP4DvlSkinMaxCnt > A_SkinIdx )
		{
			if ( sizeof(St_GrP4PrjSkinCfg) >= A_Size )
			{
				if ( A_Size != m_DvlFc->Read( &m_DvlPrjSkin[A_SkinIdx], A_Size ) )
				{
					DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjSkin - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjSkin - data size was big!\n");
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjSkin - bad skin index ( %d )!\n" , A_SkinIdx);
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::LcDvlPrjLoad( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP4ScptDataHd	Tv_DataHd;
		St_GrP4PrjHdInfo	Tv_PrjHdInfo;
	// code --------------------
		Tv_Result	=	FALSE;
		// project close
		DvlPrjClose();
		// open file
		m_DvlFc	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE, FALSE, TRUE );
		if ( m_DvlFc->IsOpened() )		// check file open success
		{
			// load project header
			m_DvlFc->Read( &Tv_PrjHdInfo, sizeof(Tv_PrjHdInfo) );	// read header info
			// check header
			if ( (E_GrP4DvlPrjFcc == Tv_PrjHdInfo.Fcc) && (0 != Tv_PrjHdInfo.Ver) && (E_GrP4DvlPrjVer >= Tv_PrjHdInfo.Ver) )
			{
				// init
				m_DvlIsLoading	=	TRUE;
				// patch version
				if ( 4 == Tv_PrjHdInfo.Ver )
				{
					// old version conversion
					DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlPrjLoad - load old version ( 4 ) header.\n" );
					m_DvlIsLoading	=	LcPrjLoadOldPrjHead();
				}

				// check able
				if ( m_DvlIsLoading )
				{
					// parsing file
					while ( sizeof(Tv_DataHd) == m_DvlFc->Read( &Tv_DataHd, sizeof(Tv_DataHd) ) )
					{
						// parsing data
						switch ( Tv_DataHd.Type )
						{
							case E_GrP4DvlPrjDataScptHdOld:
								LcPrjLoadOldScptHd( Tv_DataHd.Size );
								break;
							case E_GrP4DvlPrjDataFolderInfo:
								LcPrjLoadFolderTbl( Tv_DataHd.Size );
								break;
							case E_GrP4ScptDataWinScptTbl:
								LcPrjLoadWinScptTbl( Tv_DataHd.Size );
								break;
							case E_GrP4DvlPrjDataWinDvlTbl:
								LcPrjLoadWinDvlTbl( Tv_DataHd.Size );
								break;
							case E_GrP4ScptDataWinScptData:
								LcPrjLoadWinScptData( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP4DvlPrjDataPstScptTblOld:
								LcPrjLoadPstScptTblWhole( Tv_DataHd.Size );
								break;
							case E_GrP4ScptDataPstTbl:
								LcPrjLoadPstScptTblOne( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP4DvlPrjDataPstDvlTbl:
								LcPrjLoadPstDvlTbl( Tv_DataHd.Size );
								break;
							case E_GrP4ScptDataPstScptData:
								LcPrjLoadPstScptData( Tv_DataHd.Size, Tv_DataHd.Id, Tv_DataHd.SubIdx );
								break;
							case E_GrP4ScptDataImageData:
								LcPrjLoadImgData( Tv_DataHd.Size, Tv_DataHd.Id, Tv_DataHd.SubIdx );
								break;
							case E_GrP4ScptDataFontData:
								LcPrjLoadFontData( Tv_DataHd.Size, Tv_DataHd.Id, Tv_DataHd.SubIdx );
								break;
							case E_GrP4ScptDataTextData:
								LcPrjLoadTextData( Tv_DataHd.Size, Tv_DataHd.Id, Tv_DataHd.SubIdx );
								break;
							case E_GrP4DvlPrjDataSetupVar:
								LcPrjLoadSetupVar( Tv_DataHd.Size );
								break;
							case E_GrP4DvlPrjDataSetupSys:
								LcPrjLoadSetupSys( Tv_DataHd.Size );
								break;
							case E_GrP4DvlPrjDataGvu:
								LcPrjLoadGvu( Tv_DataHd.Size );
								break;
							case E_GrP4DvlPrjDataSetBase:
								LcPrjLoadSetBase( Tv_DataHd.Size );
								break;
							case E_GrP4DvlPrjDataAsmInfo:
								LcPrjLoadAsmInfo( Tv_DataHd.Size );
								break;
							case E_GrP4ScptDataAsmTbl:
								LcPrjLoadAsmScptTbl( Tv_DataHd.Size );
								break;
							case E_GrP4ScptDataAsmData:
								LcPrjLoadAsmData( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP4DvlPrjDataMdlDebug:
								LcPrjLoadMdlDbg( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP4ScptDataPageScptTbl:
								LcPrjLoadPageScptTbl( Tv_DataHd.Size );
								break;
							case E_GrP4DvlPrjDataPageDvlTbl:
								LcPrjLoadPageDvlTbl( Tv_DataHd.Size );
								break;
							case E_GrP4ScptDataPageScptData:
								LcPrjLoadPageScptData( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP4DvlPrjDataSdaData:
								LcPrjLoadSdaData( Tv_DataHd.Size );
								break;
							case E_GrP4DvlPrjDataSru:
								LcPrjLoadSru( Tv_DataHd.Size );
								break;
							case E_GrP4DvlPrjDataBldFile:
								LcPrjLoadBldFile( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP4DvlPrjDataAsmCmt:
								LcPrjLoadAsmCmt( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP4ScptDataCfgPrj:
								LcPrjLoadScptCfgPrj( Tv_DataHd.Size );
								break;
							case E_GrP4ScptDataCfgRtl:
								LcPrjLoadScptCfgRtl( Tv_DataHd.Size );
								break;
							case E_GrP4ScptDataMdlAtb:
								LcPrjLoadMdlAtb( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP4DvlPrjDataConfig:
								LcPrjLoadPrjConfig( Tv_DataHd.Size );
								break;
							case E_GrP4DvlPrjDataRuntime:
								LcPrjLoadPrjRuntime( Tv_DataHd.Size );
								break;
							case  E_GrP4DvlPrjDataModel:
								LcPrjLoadPrjModel( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP4DvlPrjDataSkin:
								LcPrjLoadPrjSkin( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP4DvlPrjDataUrgst:
								LcPrjLoadPrjUrgst( Tv_DataHd.Size );
								break;
							case E_GrP4DvlPrjDataRgstRtl:
								LcPrjLoadPrjRgstRtl( Tv_DataHd.Size );
								break;
							case E_GrP4DvlPrjDataAsmCodeRmk:
								LcPrjLoadPrjAsmRmkTbl(Tv_DataHd.Size, Tv_DataHd.Id);
								break;

							default:
								DbgMsgPrint( "Cls_GrP4DvlMng::DvlPrjLoad - bad data block (%d) found. \n" , Tv_DataHd.Type );
								m_DvlIsLoading	=	FALSE;
								break;
						}
						// check error
						if ( !m_DvlIsLoading )
						{
							break;
						}
					}
				}
				// check success
				if ( m_DvlIsLoading )
				{
					Tv_Result	=	TRUE;
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::DvlPrjLoad - bad file header.\n");
			}
		}
		delete	m_DvlFc;
		m_DvlFc	=	NULL;

		// check failure
		if ( Tv_Result )
		{
			// update
			// build rule
			LcPrjRuleBuild();
			// update setup
			LcDispSetup();
			// font update
			LcFontUpdt();
			// update model
			DvlEditModelIdxSet( m_DvlPrjRtl.ModelIdx );
			// update slave id allocation
			LcSlaveMapRebuild();

			// project enable
			m_DvlIsPrjExist	=	TRUE;
		}
		else
		{
			DvlPrjClose();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlCursorColorSet( __u32 A_Color )
{
	// local -------------------
		__u8	Tv_SkinIdx;
	// code --------------------
		if ( DvlIsPrjExist() )
		{
			Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
			m_DvlPrjSkin[Tv_SkinIdx].ClrCus	=	A_Color;
			// update cursor color
			m_DvlScptCfgRtl.Skin.ClrCus	=	A_Color;
		}
}
//--------------------------------------------------------------------
void*	Cls_GrP4DvlMng::DvlSetupUsrTblGet( void )
{
		return	(void*)m_DvlStpUsrTbl;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveSetupVar( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4DvlPrjDataSetupVar;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	m_DvlStpUsrCnt * sizeof(St_GrP4PrjVarInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlStpUsrTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveSetupSys( void )
{
	// local -------------------
		//St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		/*
		Tv_DataHd.Type		=	E_GrP4DvlPrjDataSetupSys;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	m_DvlStpSysCnt * sizeof(St_GrP4PrjAdrInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlStpSysTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		*/
		DbgMsgPrint( "Cls_GrP4DvlMng::LcPrjSaveSetupSys - Error!!! this function must be remove.\n" );
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveGvu( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4DvlPrjDataGvu;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlGvuTbl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlGvuTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveUrgst( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4DvlPrjDataUrgst;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlUrgstTbl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlUrgstTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveRgstRtl( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4DvlPrjDataRgstRtl;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_RgstBuf);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_RgstBuf, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjSaveSetBase( void )
{
	// local -------------------
		//St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		/*
		Tv_DataHd.Type		=	E_GrP4DvlPrjDataSetBase;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlSetBase);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlSetBase, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		*/
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadSetupVar( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlStpUsrTbl) >= A_Size )
		{
			if ( A_Size == m_DvlFc->Read( m_DvlStpUsrTbl, A_Size ) )
			{
				m_DvlStpUsrCnt	=	A_Size / sizeof(St_GrP4PrjVarInfo);
			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadSetupVar - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadSetupVar - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPrjUrgst( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlUrgstTbl) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlUrgstTbl, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjUrgst - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjUrgst - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadPrjRgstRtl( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_RgstBuf) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_RgstBuf, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjRgstRtl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadPrjRgstRtl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadSru( __u32 A_Size )
{
	// local -------------------
	// code --------------------

		m_DvlFc->Seek( (__u64)A_Size, E_GrFileCtrlPosNow );

		DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadSru - skip this data.\n");

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadSetupSys( __u32 A_Size )
{
	// local -------------------
	// code --------------------

		m_DvlFc->Seek( (__u64)A_Size, E_GrFileCtrlPosNow );

		DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadSetupSys - skip this data.\n");
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadGvu( __u32 A_Size )
{
		if ( sizeof(m_DvlGvuTbl) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlGvuTbl, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadGvu - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadGvu - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcPrjLoadSetBase( __u32 A_Size )
{
		// skip
		m_DvlFc->Seek((__u64)A_Size, E_GrFileCtrlPosNow);
		DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadSetBase - skip this section\n");
		/*
		if ( sizeof(m_DvlSetBase) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( &m_DvlSetBase, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadSetBase - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP4DvlMng::LcPrjLoadSetBase - data size was big!\n");
		}
		*/
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::DvlSetupUsrCntGet( void )
{
		return	m_DvlStpUsrCnt;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlSetupUsrCntSet( __u32 A_Cnt )
{
		if ( E_GrP4DvlSetupUsrMaxCnt > A_Cnt )
		{
			m_DvlStpUsrCnt	=	A_Cnt;
		}
}
//--------------------------------------------------------------------
void*	Cls_GrP4DvlMng::DvlSetupSysTblGet( void )
{
		return	(void*)V_GrP4DvlStpSysTbl;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::DvlSetupSysCntGet( void )
{
		return	E_GrP4DvlSetupSysCnt;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcStpParsSysAdd(char* A_StrName, __u32 A_NameLen, __u32 A_Addr, char* A_StrDesc, __u32 A_DescLen)
{
	// local -------------------
		//__u32	Tv_Len;
		//Def_StrTag	Tv_StrTmp;
	// code --------------------
		/*
		// check full
		if ( E_GrP4DvlSetupUsrMaxCnt > m_DvlStpSysCnt )
		{
			// get name
			Tv_Len	=	A_NameLen;
			if ( E_GrP4DvlAdrNameLen <= Tv_Len )
			{
				Tv_Len	=	E_GrP4DvlAdrNameLen - 1;
			}
			GrStrCopy( Tv_StrTmp, A_StrName, Tv_Len );
			GrStrStrToWstr( m_DvlStpSysTbl[m_DvlStpSysCnt].StrName, Tv_StrTmp );
			// get address
			m_DvlStpSysTbl[m_DvlStpSysCnt].Pos	=	(__u16)A_Addr;
			// get descriptor
			if ( (NULL != A_StrDesc) && (0 != A_DescLen) )
			{
				Tv_Len	=	A_DescLen;
				if ( E_GrP4DvlAdrDescLen <= Tv_Len )
				{
					Tv_Len	=	E_GrP4DvlAdrDescLen - 1;
				}
				GrStrCopy( Tv_StrTmp, A_StrDesc, Tv_Len );
				GrStrStrToWstr( m_DvlStpSysTbl[m_DvlStpSysCnt].StrDesc, Tv_StrTmp );
			}
			else
			{
				GrStrClear( m_DvlStpSysTbl[m_DvlStpSysCnt].StrDesc );
			}
			// success
			m_DvlStpSysCnt ++;
		}
		else
		{
			m_DvlIsStpPars	=	FALSE;	// finish
		}
		*/
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcStpParsGetWordNrm( void )
{
	// local -------------------
		char	Tv_Char;
		BOOL8	Tv_IsRun;
	// code --------------------
		// init
		m_DvlStpPsType		=	E_GrP4DvlStpPsTypeReady;
		Tv_IsRun	=	TRUE;

		m_DvlPtrStpWord	=	NULL;
		m_DvlStpWordLen	=	0;

		while( Tv_IsRun && (m_DvlStpPsPrs < m_DvlStpFlSize) )
		{
			Tv_Char	=	m_DvlPtrStpFl[m_DvlStpPsPrs];
			switch ( m_DvlStpPsType )
			{
				case E_GrP4DvlStpPsTypeReady:
					// check type
					if (	(('A' <= Tv_Char) && ('Z' >= Tv_Char)) || (('a' <= Tv_Char) && ('z' >= Tv_Char)) ||
								( '_' == Tv_Char ) )
					{
						// normal text type
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeText;
						m_DvlPtrStpWord	=	&m_DvlPtrStpFl[m_DvlStpPsPrs];
						m_DvlStpWordLen	=	1;
					}
					else if ('0' == Tv_Char)
					{
						// zero number
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeNumZero;
						m_DvlPtrStpWord	=	&m_DvlPtrStpFl[m_DvlStpPsPrs];
						m_DvlStpWordLen	=	1;
					}
					else if ( ('1' <= Tv_Char) && ('9' >= Tv_Char) )
					{
						// decimal number
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeNumDec;
						m_DvlPtrStpWord	=	&m_DvlPtrStpFl[m_DvlStpPsPrs];
						m_DvlStpWordLen	=	1;
					}
					else if ( '#' == Tv_Char )
					{
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeDirect;
						Tv_IsRun	=	FALSE;
						break;
					}
					else if ( '/' == Tv_Char)
					{
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeRemPref;
						m_DvlPtrStpWord	=	&m_DvlPtrStpFl[m_DvlStpPsPrs];
						m_DvlStpWordLen	=	1;
					}
					else if ( 0x0A == Tv_Char )
					{
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeLineEnd;
						Tv_IsRun	=	FALSE;
					}
					else if ( 0x0 == Tv_Char )
					{
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeLineEnd;
						Tv_IsRun	=	FALSE;
						m_DvlIsStpPars	=	FALSE;		// finish
					}
					else if ( 0x20 < Tv_Char )
					{
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeUnknown;
						m_DvlPtrStpWord	=	&m_DvlPtrStpFl[m_DvlStpPsPrs];
						m_DvlStpWordLen	=	1;
					}
					break;
				case E_GrP4DvlStpPsTypeText:
					if (	(('A' <= Tv_Char) && ('Z' >= Tv_Char)) || (('a' <= Tv_Char) && ('z' >= Tv_Char)) ||
								( '_' == Tv_Char ) || (('0' <= Tv_Char) && ('9' >= Tv_Char)) )
					{
						// keep
						m_DvlStpWordLen ++;
					}
					else
					{
						// finish
						m_DvlStpPsPrs --;	// back
						Tv_IsRun	=	FALSE;
					}
					break;
				case E_GrP4DvlStpPsTypeNumZero:
					if ( ('x'== Tv_Char) || ('X'== Tv_Char) )
					{
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeNumHpx;
					}
					else if ( ('0' <= Tv_Char) && ('9' >= Tv_Char) )
					{
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeNumDec;
						m_DvlStpWordLen ++;
					}
					else
					{
						// finish
						m_DvlStpPsPrs --;	// back
						Tv_IsRun	=	FALSE;
					}
					break;
				case E_GrP4DvlStpPsTypeNumHpx:
					if (	(('0' <= Tv_Char) && ('9' >= Tv_Char)) || (('A' <= Tv_Char) && ('F' >= Tv_Char)) ||
								(('a' <= Tv_Char) && ('f' >= Tv_Char))	)
					{
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeNumHex;
						// skip prefix
						m_DvlPtrStpWord ++;
						m_DvlPtrStpWord ++;
						m_DvlStpWordLen	=	1;
					}
					else
					{
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeNumDec;
						// finish
						m_DvlStpPsPrs =	m_DvlStpPsPrs - 2;	// back to number
						Tv_IsRun	=	FALSE;
					}
					break;
				case E_GrP4DvlStpPsTypeNumDec:
					if ( ('0' <= Tv_Char) && ('9' >= Tv_Char) )
					{
						m_DvlStpWordLen ++;
					}
					else
					{
						// finish
						m_DvlStpPsPrs --;	// back
						Tv_IsRun	=	FALSE;
					}
					break;
				case E_GrP4DvlStpPsTypeNumHex:
					if (	(('0' <= Tv_Char) && ('9' >= Tv_Char)) || (('A' <= Tv_Char) && ('F' >= Tv_Char)) ||
								(('a' <= Tv_Char) && ('f' >= Tv_Char))	)
					{
						m_DvlStpWordLen ++;
					}
					else
					{
						// finish
						m_DvlStpPsPrs --;	// back
						Tv_IsRun	=	FALSE;
					}
					break;
				case E_GrP4DvlStpPsTypeRemPref:
					if ( '*' == Tv_Char )
					{
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeRemBlk;
						Tv_IsRun	=	FALSE;
					}
					else if ( '/' == Tv_Char )
					{
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeRemLine;
						Tv_IsRun	=	FALSE;
					}
					else
					{
						m_DvlStpPsType	=	E_GrP4DvlStpPsTypeUnknown;
						m_DvlStpWordLen ++;
					}
					break;
				case E_GrP4DvlStpPsTypeUnknown:
					if ( ('/' == Tv_Char) || ( 0x20 >= Tv_Char ) )
					{
						m_DvlStpPsPrs --;
						Tv_IsRun	=	FALSE;
					}
					else 
					{
						m_DvlStpWordLen	++;
					}
					break;
			}
			// next
			m_DvlStpPsPrs ++;
		}
		// patch
		if ( E_GrP4DvlStpPsTypeNumZero == m_DvlStpPsType )
		{
			m_DvlStpPsType	=	E_GrP4DvlStpPsTypeNumDec;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcStpParsModeReady( void )
{
	// local -------------------
	// code --------------------
		// init
		m_DvlIsStpMult	=	FALSE;
		LcStpParsGetWordNrm();		// get first word
		// check first word type
		switch ( m_DvlStpPsType )
		{
			case E_GrP4DvlStpPsTypeDirect:
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeDefine;	// define wait mode
				break;
			case E_GrP4DvlStpPsTypeLineEnd:
				// keep ready
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	
				break;
			case E_GrP4DvlStpPsTypeRemLine:
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeRemLine;	
				break;
			case E_GrP4DvlStpPsModeRemBlk:
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeRemBlk;	
				break;
			default:
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeLineEnd;	// not need line mode
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcStpParsModeDefine( void )
{
	// local -------------------
		BOOL8	Tv_IsOk;
	// code --------------------
		LcStpParsGetWordNrm();		// get secound word
		switch ( m_DvlStpPsType )
		{
			case E_GrP4DvlStpPsTypeText:
				// check name size
				Tv_IsOk	=	FALSE;
				if ( 6 == m_DvlStpWordLen )
				{
					if ( 0 ==	GrStrCmp( m_DvlPtrStpWord, "define", TRUE, 6 ) )
					{
						Tv_IsOk	=	TRUE;
						m_DvlStpPsMode	=	E_GrP4DvlStpPsModeName;	// reset to ready
					}
				}
				if ( !Tv_IsOk )
				{
					m_DvlStpPsMode	=	E_GrP4DvlStpPsModeLineEnd;	// not need line mode
				}
				break;
			case E_GrP4DvlStpPsTypeLineEnd:
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	// reset to ready
				break;
			default:
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeLineEnd;	// not need line mode
				break;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcStpParsModeName( void )
{
	// local -------------------
		BOOL8	Tv_IsOk;
	// code --------------------
		LcStpParsGetWordNrm();		// get secound word
		switch ( m_DvlStpPsType )
		{
			case E_GrP4DvlStpPsTypeText:
				// check name size
				Tv_IsOk	=	FALSE;
				if ( E_GrP4DvlStpDefNameMin < m_DvlStpWordLen )
				{
					if ( 0 ==	GrStrCmp( m_DvlPtrStpWord, "E_GrP4CfgOfs", TRUE, E_GrP4DvlStpDefNameMin ) )
					{
						Tv_IsOk	=	TRUE;
						m_DvlStpStrName	=	(char*)( (__u32)m_DvlPtrStpWord + E_GrP4DvlStpDefNameMin );
						m_DvlStpNameLen	=	m_DvlStpWordLen - E_GrP4DvlStpDefNameMin;
						m_DvlStpPsMode	=	E_GrP4DvlStpPsModeAddr;	
					}
				}
				if ( !Tv_IsOk )
				{
					m_DvlStpPsMode	=	E_GrP4DvlStpPsModeLineEnd;	// not need line mode
				}
				break;
			case E_GrP4DvlStpPsTypeLineEnd:
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	// reset to ready
				break;
			default:
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeLineEnd;	// not need line mode
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcStpParsModeAddr( void )
{
	// local -------------------
	// code --------------------
		LcStpParsGetWordNrm();		// get secound word
		switch ( m_DvlStpPsType )
		{
			case E_GrP4DvlStpPsTypeNumDec:
				if ( GrStrStrToInt( m_DvlPtrStpWord, (__s32*)&m_DvlStpAdr, m_DvlStpWordLen ) )
				{
					m_DvlStpPsMode	=	E_GrP4DvlStpPsModeDescChk;	
				}
				break;
			case E_GrP4DvlStpPsTypeNumHex:
				if ( GrStrHexStrToDword( m_DvlPtrStpWord, &m_DvlStpAdr, m_DvlStpWordLen ) )
				{
					m_DvlStpPsMode	=	E_GrP4DvlStpPsModeDescChk;	
				}
				break;
			case E_GrP4DvlStpPsTypeLineEnd:
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	// reset to ready
				break;
			default:
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeLineEnd;	// not need line mode
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcStpParsModeDescChk( void )
{
	// local -------------------
		BOOL8	Tv_IsUpdtVal;
	// code --------------------
		Tv_IsUpdtVal	=	FALSE;
		LcStpParsGetWordNrm();		// get secound word
		switch ( m_DvlStpPsType )
		{
			case	E_GrP4DvlStpPsTypeRemLine:
				m_DvlStpStrDesc	=	(char*)&m_DvlPtrStpFl[m_DvlStpPsPrs];
				m_DvlStpDescLen	=	0;
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeDescLine;	
				break;
			case E_GrP4DvlStpPsTypeRemBlk:
				m_DvlStpStrDesc	=	(char*)&m_DvlPtrStpFl[m_DvlStpPsPrs];
				m_DvlStpDescLen	=	0;
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeDescBlk;	
				m_DvlIsStpMult	=	FALSE;
				break;
			case E_GrP4DvlStpPsTypeLineEnd:
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	// reset to ready
				Tv_IsUpdtVal		=	TRUE;
				break;
			default:
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeLineEnd;	
				Tv_IsUpdtVal		=	TRUE;
				break;
		}

		// update value
		if ( Tv_IsUpdtVal )
		{
			LcStpParsSysAdd( m_DvlStpStrName, m_DvlStpNameLen, m_DvlStpAdr, NULL, 0 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcStpParsModeDescLine( void )
{
	// local -------------------
		__s8	Tv_Char;
		BOOL8	Tv_IsUpdtVal;
	// code --------------------
		Tv_IsUpdtVal	=	FALSE;
		// get one __s8
		Tv_Char	=	m_DvlPtrStpFl[m_DvlStpPsPrs];
		if ( 0x0D == Tv_Char )
		{
			m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	
			Tv_IsUpdtVal		=	TRUE;
			m_DvlStpPsPrs ++;
		}
		else if ( 0x0A == Tv_Char )
		{
			m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;
			Tv_IsUpdtVal		=	TRUE;
			m_DvlStpPsPrs ++;
		}
		else if ( 0x00 == Tv_Char )
		{
			m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	
			Tv_IsUpdtVal		=	TRUE;
			m_DvlIsStpPars	=	FALSE;	// parsing finish
		}
		else
		{
			m_DvlStpDescLen ++;
			m_DvlStpPsPrs ++;
		}
			 
		if ( Tv_IsUpdtVal )
		{
			LcStpParsSysAdd( m_DvlStpStrName, m_DvlStpNameLen, m_DvlStpAdr, m_DvlStpStrDesc, m_DvlStpDescLen );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcStpParsModeDescBlk( void )
{
	// local -------------------
		__s8	Tv_Char;
		BOOL8	Tv_IsUpdtVal;
	// code --------------------
		Tv_IsUpdtVal	=	FALSE;
		// get one __s8
		Tv_Char	=	m_DvlPtrStpFl[m_DvlStpPsPrs];
		if ( (0x0D == Tv_Char) || ( 0x0A == Tv_Char ) )
		{
			m_DvlPtrStpFl[m_DvlStpPsPrs]	=	0x20;
			m_DvlStpPsPrs ++;
			m_DvlIsStpMult	=	FALSE;
		}
		else if ( '*' == Tv_Char )
		{
			m_DvlIsStpMult	=	TRUE;
			m_DvlStpDescLen ++;
			m_DvlStpPsPrs ++;
		}
		else if ( '/' == Tv_Char )
		{
			if ( m_DvlIsStpMult )
			{
				// finish remark
				m_DvlStpDescLen --;
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	
				Tv_IsUpdtVal		=	TRUE;
			}
			else
			{
				m_DvlStpDescLen ++;
			}
			m_DvlIsStpMult	=	FALSE;
			m_DvlStpPsPrs ++;
		}
		else if ( 0x00 == Tv_Char )
		{
			m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	
			Tv_IsUpdtVal		=	TRUE;
			m_DvlIsStpPars	=	FALSE;	// parsing finish
			m_DvlIsStpMult	=	FALSE;
		}
		else
		{
			m_DvlStpDescLen ++;
			m_DvlStpPsPrs ++;
			m_DvlIsStpMult	=	FALSE;
		}

		if ( Tv_IsUpdtVal )
		{
			LcStpParsSysAdd( m_DvlStpStrName, m_DvlStpNameLen, m_DvlStpAdr, m_DvlStpStrDesc, m_DvlStpDescLen );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcStpParsModeRemLine( void )
{
	// local -------------------
		__s8	Tv_Char;
	// code --------------------
		// get one __s8
		Tv_Char	=	m_DvlPtrStpFl[m_DvlStpPsPrs];
		if ( (0x0D == Tv_Char) || (0x0A == Tv_Char) )
		{
			m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	
			m_DvlStpPsPrs ++;
		}
		else if ( 0x00 == Tv_Char )
		{
			m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	
			m_DvlIsStpPars	=	FALSE;	// parsing finish
		}
		else
		{
			m_DvlStpPsPrs ++;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcStpParsModeRemBlk( void )
{
	// local -------------------
		__s8	Tv_Char;
	// code --------------------
		// get one __s8
		Tv_Char	=	m_DvlPtrStpFl[m_DvlStpPsPrs];
		if ( (0x0D == Tv_Char) || ( 0x0A == Tv_Char ) )
		{
			m_DvlStpPsPrs ++;
			m_DvlIsStpMult	=	FALSE;
		}
		else if ( '*' == Tv_Char )
		{
			m_DvlIsStpMult	=	TRUE;
			m_DvlStpPsPrs ++;
		}
		else if ( '/' == Tv_Char )
		{
			if ( m_DvlIsStpMult )
			{
				// finish remark
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	
			}
			m_DvlIsStpMult	=	FALSE;
			m_DvlStpPsPrs ++;
		}
		else if ( 0x00 == Tv_Char )
		{
			m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	
			m_DvlIsStpPars	=	FALSE;	// parsing finish
			m_DvlIsStpMult	=	FALSE;
		}
		else
		{
			m_DvlStpPsPrs ++;
			m_DvlIsStpMult	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcStpParsModeEndLine( void )
{
	// local -------------------
		__s8	Tv_Char;
	// code --------------------
		// get one __s8
		Tv_Char	=	m_DvlPtrStpFl[m_DvlStpPsPrs];
		if ( (0x0D == Tv_Char) || ( 0x0A == Tv_Char ) )
		{
			m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	
			m_DvlStpPsPrs ++;
			m_DvlIsStpMult	=	FALSE;
		}
		else if ( '/' == Tv_Char )
		{
			m_DvlIsStpMult	=	TRUE;
			m_DvlStpPsPrs ++;
		}
		else if ( '*' == Tv_Char )
		{
			if ( m_DvlIsStpMult )
			{
				// start remark
				m_DvlStpPsMode	=	E_GrP4DvlStpPsModeRemBlk;	
			}
			m_DvlIsStpMult	=	FALSE;
			m_DvlStpPsPrs ++;
		}
		else if ( 0x00 == Tv_Char )
		{
			m_DvlStpPsMode	=	E_GrP4DvlStpPsModeReady;	
			m_DvlIsStpPars	=	FALSE;	// parsing finish
			m_DvlIsStpMult	=	FALSE;
		}
		else
		{
			m_DvlStpPsPrs ++;
			m_DvlIsStpMult	=	FALSE;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlWindowDel( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP4DvlMsgWindowDel	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdWindowDel;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::LcDvlWindowDel( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result	=	FALSE;
		// check script id range
		if ( (E_GrP4ScptIdWinBase <= A_ScptId) && ((E_GrP4ScptIdWinBase + E_GrP4DvlWinMaxCnt) > A_ScptId ) )
		{
			Tv_Idx	=	A_ScptId - E_GrP4ScptIdWinBase;
			if ( NULL != m_DvlScptWinTbl[Tv_Idx].Pos )
			{
				// check match edit window
				if ( NULL != m_DvlEdtLump )
				{
					if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
					{
						// delete 
						LcEditObjFree();
					}
				}
				// delete
				free( m_DvlScptWinTbl[Tv_Idx].Pos );
				m_DvlScptWinTbl[Tv_Idx].Pos	=	NULL;
				m_DvlScptWinTbl[Tv_Idx].Layer		=	0;
				m_DvlWinInfo[Tv_Idx].FoldId			=	E_GrP4DvlFolderIdRoot;
				m_DvlWinInfo[Tv_Idx].Size				=	0;
				GrStrClear( m_DvlWinInfo[Tv_Idx].StrName ) ;
				// slave id rebuild
				LcSlaveMapRebuild();
				// success
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlPageDel( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP4DvlMsgPageDel	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdPageDel;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::LcDvlPageDel( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result	=	FALSE;
		// check script id range
		if ( (E_GrP4ScptIdPageBase <= A_ScptId) && ((E_GrP4ScptIdPageBase + E_GrP4DvlPageMaxCnt) > A_ScptId ) )
		{
			Tv_Idx	=	A_ScptId - E_GrP4ScptIdPageBase;
			if ( NULL != m_DvlScptPageTbl[Tv_Idx].Pos )
			{
				// check match edit page
				if ( NULL != m_DvlEdtLump )
				{
					if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
					{
						// delete 
						LcEditObjFree();
					}
				}
				// delete
				free( m_DvlScptPageTbl[Tv_Idx].Pos );
				m_DvlScptPageTbl[Tv_Idx].Pos	=	NULL;
				m_DvlScptPageTbl[Tv_Idx].AsmOpen	=	E_GrP4AsmIdNone;
				m_DvlScptPageTbl[Tv_Idx].AsmClose	=	E_GrP4AsmIdNone;
				m_DvlPageInfo[Tv_Idx].FoldId			=	E_GrP4DvlFolderIdRoot;
				m_DvlPageInfo[Tv_Idx].Size				=	0;
				GrStrClear( m_DvlPageInfo[Tv_Idx].StrName ) ;
				// slave id rebuild
				LcSlaveMapRebuild();
				// success
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlWinFindByPrntFolder( __u16 A_FolderId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlWinMaxCnt;Tv_WkIdx++ )
		{
			// check exist
			if ( NULL != m_DvlScptWinTbl[Tv_WkIdx].Pos )
			{
				// check parent folder id
				if ( A_FolderId == m_DvlWinInfo[Tv_WkIdx].FoldId )
				{
					// found
					Tv_Result	=	E_GrP4ScptIdWinBase + Tv_WkIdx;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlPageFindByPrntFolder( __u16 A_FolderId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlPageMaxCnt;Tv_WkIdx++ )
		{
			// check exist
			if ( NULL != m_DvlScptPageTbl[Tv_WkIdx].Pos )
			{
				// check parent folder id
				if ( A_FolderId == m_DvlPageInfo[Tv_WkIdx].FoldId )
				{
					// found
					Tv_Result	=	E_GrP4ScptIdPageBase + Tv_WkIdx;
					break;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrP4DvlMng::DvlGvuInfoTblGet( void )
{
		return	(void*)m_DvlGvuTbl;
}
//--------------------------------------------------------------------
void*	Cls_GrP4DvlMng::DvlUrgstInfoTblGet( void )
{
		return	(void*)m_DvlUrgstTbl;
}
//--------------------------------------------------------------------
void*	Cls_GrP4DvlMng::DvlAsmCreate( __u16 A_Id )
{
	// local -------------------
		void*	Tv_Result;
		__u16	Tv_AsmIdx;
	// code --------------------
		Tv_Result	=	NULL;
		// check id
		if ( (E_GrP4AsmIdBase <= A_Id) && ((E_GrP4AsmIdBase + E_GrP4DvlAsmMaxCnt) > A_Id) )
		{
			// get index
			Tv_AsmIdx	=	A_Id - E_GrP4AsmIdBase;
			// check exist
			if ( NULL == m_DvlScptAsmTbl[Tv_AsmIdx].Pos )
			{
				// allocate buffer
				m_DvlScptAsmTbl[Tv_AsmIdx].Pos	=	(void*)malloc( E_GrP4DvlAsmBufMax );
				if ( NULL != m_DvlScptAsmTbl[Tv_AsmIdx].Pos )
				{
					m_DvlScptAsmTbl[Tv_AsmIdx].Cnt	=	0;		// count reset
					// GrStrClear( m_DvlAsmInfo[Tv_AsmIdx].StrName );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::DvlAsmCreate - assembly code ( %d ) allocate failure.\n" , A_Id );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4DvlMng::DvlAsmCreate - assembly code ( %d ) already exist.\n" , A_Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DvlMng::DvlAsmCreate - bad assembly code id ( %d ).\n" , A_Id );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlAsmCodeCountSet( __u16 A_Id, __u16 A_Cnt )
{
	// local -------------------
		__u16	Tv_AsmIdx;
	// code --------------------
		// check count over
		if ( E_GrP4DvlAsmCodeMax >= A_Cnt )
		{
			// check id
			if ( (E_GrP4AsmIdBase <= A_Id) && ((E_GrP4AsmIdBase + E_GrP4DvlAsmMaxCnt) > A_Id) )
			{
				// get index
				Tv_AsmIdx	=	A_Id - E_GrP4AsmIdBase;
				// check exist
				if ( NULL != m_DvlScptAsmTbl[Tv_AsmIdx].Pos )
				{
					// set
					m_DvlScptAsmTbl[Tv_AsmIdx].Cnt	=	A_Cnt;
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::DvlAsmCodeCountSet - assembly code ( %d ) not exist.\n" , A_Id );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4DvlMng::DvlAsmCodeCountSet - bad assembly code id ( %d ).\n" , A_Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DvlMng::DvlAsmCodeCountSet - try over count ( %d ) set to code id ( %d ).\n" , A_Cnt, A_Id );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP4DvlMng::DvlAsmFuncNameGet( __u16 A_Id )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16	Tv_AsmIdx;
	// code --------------------
		Tv_Result	=	NULL;
		if ( (E_GrP4AsmIdBase <= A_Id) && ((E_GrP4AsmIdBase + E_GrP4DvlAsmMaxCnt) > A_Id) )
		{
			// get index
			Tv_AsmIdx	=	A_Id - E_GrP4AsmIdBase;
			// set
			Tv_Result	=	m_DvlAsmInfo[Tv_AsmIdx].StrName;
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DvlMng::DvlAsmCodeNameGet - bad assembly code id ( %d ).\n" , A_Id );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlAsmFuncNameSet( __u16 A_Id, WCHAR* A_StrName )
{
	// local -------------------
		__u16	Tv_AsmIdx;
	// code --------------------
		if ( (E_GrP4AsmIdBase <= A_Id) && ((E_GrP4AsmIdBase + E_GrP4DvlAsmMaxCnt) > A_Id) )
		{
			// get index
			Tv_AsmIdx	=	A_Id - E_GrP4AsmIdBase;
			// set
			GrStrWcopy( m_DvlAsmInfo[Tv_AsmIdx].StrName, A_StrName, 31 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DvlMng::DvlAsmCodeNameSet - bad assembly code id ( %d ).\n" , A_Id );
		}
}
//--------------------------------------------------------------------
Ptr_GrP4PrjAsmCodeRmk	Cls_GrP4DvlMng::DvlAsmRmkPtrGet(__u16 A_Id)
{
	// local -------------------
		Ptr_GrP4PrjAsmCodeRmk	Tv_Result;
		__u16	Tv_AsmIdx;
	// code --------------------
		Tv_Result	=	NULL;
		if ( (E_GrP4AsmIdBase <= A_Id) && ((E_GrP4AsmIdBase + E_GrP4DvlAsmMaxCnt) > A_Id) )
		{
			// get index
			Tv_AsmIdx	=	A_Id - E_GrP4AsmIdBase;
			// set
			Tv_Result	=	&m_DvlAsmRmkTbl[Tv_AsmIdx];
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DvlMng::DvlAsmRmkPtrGet - bad assembly code id ( %d ).\n" , A_Id );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrP4PrjAsmFncCmt	Cls_GrP4DvlMng::DvlAsmFncCmtPtrGet(__u16 A_Id)
{
	// local -------------------
		Ptr_GrP4PrjAsmFncCmt	Tv_Result;
		__u16	Tv_AsmIdx;
	// code --------------------
		Tv_Result	=	NULL;
		if ((E_GrP4AsmIdBase <= A_Id) && ((E_GrP4AsmIdBase + E_GrP4DvlAsmMaxCnt) > A_Id))
		{
			// get index
			Tv_AsmIdx	=	A_Id - E_GrP4AsmIdBase;
			// set
			Tv_Result	=	&m_DvlAsmFncCmt[Tv_AsmIdx];
		}
		else
		{
			DbgMsgPrint("Cls_GrP4DvlMng::DvlAsmFncCmtPtrGet - bad assembly code id ( %d ).\n", A_Id);
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDbgGvsInit( void )
{
	// local -------------------
	// code --------------------
		m_Gvs.VdoMaxCh	=	m_DvlMdlDblTbl[m_DvlPrjRtl.ModelIdx].VdoMaxCh;
		m_Gvs.AdoMaxCh	=	m_DvlMdlDblTbl[m_DvlPrjRtl.ModelIdx].AdoMaxCh;
		m_Gvs.DiChCnt		=	m_DvlMdlDblTbl[m_DvlPrjRtl.ModelIdx].DiChCnt;
		m_Gvs.DoChCnt		=	m_DvlMdlDblTbl[m_DvlPrjRtl.ModelIdx].DoChCnt;

		m_Gvs.HwVer			=	m_DvlMdlDblTbl[m_DvlPrjRtl.ModelIdx].HwVer;
		m_Gvs.NetVer		=	m_DvlMdlDblTbl[m_DvlPrjRtl.ModelIdx].NetVer;

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDbgConInit( void )
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP4MngConMaxCnt;Tv_ConIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				m_ConTbl[Tv_ConIdx]->DvlDebugPrepare();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlDbgStart( void )
{
	// local -------------------
		St_GrP4DvlMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Cmd				=	E_GrP4DvlMcmdDbgStart;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlDbgStart( void )
{
	// local -------------------
	// code --------------------
		if (!DvlIsDbgRun())
		{
			// init
			m_DvlDbgIsResume	=	FALSE;
			m_DvlDbgMode			=	E_GrP4DvlDbgModeRun;

			// release edit window
			LcEditObjFree();
			DvlEditModelIdxSet(m_DvlPrjRtl.ModelIdx);
			LcDbgGvsInit();
			LcDbgConInit();
			// setup assign
			SysBuildEnv();
			SysInit();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlDbgFinish( void )
{
	// local -------------------
		//St_GrP4DvlMsgBase	Tv_Msg;
	// code --------------------
		m_DvlDbgMode			=	E_GrP4DvlDbgModeStop;

		LcWinAllFree();

		m_IsDvlBkupRun	=	FALSE;

		m_SysStat	=	E_GrP4MngSysStatNone;

		/*
		Tv_Msg.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Cmd				=	E_GrP4DvlMcmdDbgFinish;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		*/
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlDbgFinish( void )
{
	// local -------------------
	// code --------------------
		m_DvlDbgMode	=	E_GrP4DvlDbgModeStop;
		LcWinAllFree();
		
		m_IsDvlBkupRun	=	FALSE;

		m_SysStat	=	E_GrP4MngSysStatNone;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
		St_GrTaskMsgFwUpgdEnd	Tv_MsgFwUpEnd;
	// code --------------------
		Cls_GrP4Mng::RtlTimer( A_Hndl );

		// tick id increase
		m_DvlTickId ++;

		// debug
		if ((E_GrP4DvlDbgModeRun == m_DvlDbgMode) || (E_GrP4DvlDbgModeStep == m_DvlDbgMode))
		{
			// do system status
			switch ( m_SysStat )
			{
				case E_GrP4MngSysStatInit:
					LcTimerEmuInit();
					break;
				case E_GrP4MngSysStatFwUpgd:
					if ( 100 <= m_Gvs.PrgsFwUp )
					{
						// finish
						m_SysStat	=	E_GrP4MngSysStatStart;
						// notify finish
						Tv_MsgFwUpEnd.MsgHd.Cmd	=	E_GrTaskMsgFwUpgdFinish;
						Tv_MsgFwUpEnd.ErrCode		=	0;
						m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_MsgFwUpEnd, sizeof(Tv_MsgFwUpEnd) );
					}
					else
					{
						m_Gvs.PrgsFwUp	=	m_Gvs.PrgsFwUp + 5;
					}
					break;
			}
			LcDvlRatbUpdt();			// record attribute update
			LcDvlLanCblUpdt();		// LAN cable emulation
			LcDvrUsbUpdt();
			LcDvrCmsUpdt();
			LcDvrBkupUpdt();
			LcDvrLogBkupUpdt();
			LcDvlPlayUpdt();
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_DvlMsgAloc == A_PtrMsg->Cmd )
		{
			switch ( ((Ptr_GrP4DvlMsgBase)A_PtrMsg)->Cmd )
			{
				case E_GrP4DvlMcmdPrjNew:
					*((Ptr_GrP4DvlMsgPrjNew)A_PtrMsg)->PtrRslt	=	LcDvlPrjNew( ((Ptr_GrP4DvlMsgPrjNew)A_PtrMsg)->FccPx,
						((Ptr_GrP4DvlMsgPrjNew)A_PtrMsg)->Res1X, ((Ptr_GrP4DvlMsgPrjNew)A_PtrMsg)->Res1Y,
						((Ptr_GrP4DvlMsgPrjNew)A_PtrMsg)->Res2X, ((Ptr_GrP4DvlMsgPrjNew)A_PtrMsg)->Res2Y,
						((Ptr_GrP4DvlMsgPrjNew)A_PtrMsg)->IsDual, ((Ptr_GrP4DvlMsgPrjNew)A_PtrMsg)->IsInputDual );
					break;
				case E_GrP4DvlMcmdPrjClose:
					LcDvlPrjClose();
					break;
				case E_GrP4DvlMcmdPrjSave:
					*((Ptr_GrP4DvlMsgPrjSave)A_PtrMsg)->PtrRslt	=	LcDvlPrjSave( ((Ptr_GrP4DvlMsgPrjSave)A_PtrMsg)->StrFn );
					break;
				case E_GrP4DvlMcmdPrjLoad:
					*((Ptr_GrP4DvlMsgPrjLoad)A_PtrMsg)->PtrRslt	=	LcDvlPrjLoad( ((Ptr_GrP4DvlMsgPrjLoad)A_PtrMsg)->StrFn );
					break;
				case E_GrP4DvlMcmdWindowNew:
					*((Ptr_GrP4DvlMsgWindowNew)A_PtrMsg)->PtrRslt	=	LcDvlWindowNew();
					break;
				case E_GrP4DvlMcmdPageNew:
					*((Ptr_GrP4DvlMsgPageNew)A_PtrMsg)->PtrRslt	=	LcDvlPageNew();
					break;
				case E_GrP4DvlMcmdWindowDel:
					*((Ptr_GrP4DvlMsgWindowDel)A_PtrMsg)->PtrRslt	=	LcDvlWindowDel( ((Ptr_GrP4DvlMsgWindowDel)A_PtrMsg)->ScptId );
					break;
				case E_GrP4DvlMcmdPageDel:
					*((Ptr_GrP4DvlMsgPageDel)A_PtrMsg)->PtrRslt	=	LcDvlPageDel( ((Ptr_GrP4DvlMsgPageDel)A_PtrMsg)->ScptId );
					break;
				case E_GrP4DvlMcmdScriptDec:
					*((Ptr_GrP4DvlMsgScriptDec)A_PtrMsg)->PtrRslt	=	LcDvlScriptDec( ((Ptr_GrP4DvlMsgScriptDec)A_PtrMsg)->PrntId,
						((Ptr_GrP4DvlMsgScriptDec)A_PtrMsg)->PtrScpt );
					break;
				case E_GrP4DvlMcmdEditSkinSet:
					LcDvlEditSkinSet( ((Ptr_GrP4DvlMsgEditSkinSet)A_PtrMsg)->SkinIdx );
					break;
				case E_GrP4DvlMcmdEditModelIdxSet:
					LcDvlEditModelIdxSet( ((Ptr_GrP4DvlMsgEditModelIdxSet)A_PtrMsg)->ModelIdx );
					break;
				case E_GrP4DvlMcmdEditObjCreate:
					*((Ptr_GrP4DvlMsgEditObjCreate)A_PtrMsg)->PtrRslt	=	LcDvlEditObjCreate( ((Ptr_GrP4DvlMsgEditObjCreate)A_PtrMsg)->ScrX,
						((Ptr_GrP4DvlMsgEditObjCreate)A_PtrMsg)->ScrY, ((Ptr_GrP4DvlMsgEditObjCreate)A_PtrMsg)->ClassId );
					break;
				case E_GrP4DvlMcmdEditLumpSet:
					*((Ptr_GrP4DvlMsgEditLumpSet)A_PtrMsg)->PtrRslt	=	LcDvlEditLumpSet( ((Ptr_GrP4DvlMsgEditLumpSet)A_PtrMsg)->ScptId );
					break;
				case E_GrP4DvlMcmdEditObjClassIdGet:
					*((Ptr_GrP4DvlMsgEditObjClassIdGet)A_PtrMsg)->PtrRslt	=	LcDvlEditObjClassIdGet( ((Ptr_GrP4DvlMsgEditObjClassIdGet)A_PtrMsg)->ScptId );
					break;
				case E_GrP4DvlMcmdEditObjScptDump:
					*((Ptr_GrP4DvlMsgEditObjScptDump)A_PtrMsg)->PtrRslt	=	LcDvlEditObjScptDump( ((Ptr_GrP4DvlMsgEditObjScptDump)A_PtrMsg)->ScptId,
						((Ptr_GrP4DvlMsgEditObjScptDump)A_PtrMsg)->PtrBuf, ((Ptr_GrP4DvlMsgEditObjScptDump)A_PtrMsg)->IsCopyMode );
					break;
				case E_GrP4DvlMcmdEditObjFrameGet:
					*((Ptr_GrP4DvlMsgEditObjFrameGet)A_PtrMsg)->PtrRslt	=	LcDvlEditObjFrameGet( ((Ptr_GrP4DvlMsgEditObjFrameGet)A_PtrMsg)->ScptId );
					break;
				case E_GrP4DvlMcmdEditObjRedraw:
					LcDvlEditObjRedraw( ((Ptr_GrP4DvlMsgEditObjRedraw)A_PtrMsg)->ScptId );
					break;
				case E_GrP4DvlMcmdEditObjListByPos:
					*((Ptr_GrP4DvlMsgEditObjListByPos)A_PtrMsg)->PtrRslt	=	LcDvlEditObjListByPos( ((Ptr_GrP4DvlMsgEditObjListByPos)A_PtrMsg)->X,
						((Ptr_GrP4DvlMsgEditObjListByPos)A_PtrMsg)->Y, ((Ptr_GrP4DvlMsgEditObjListByPos)A_PtrMsg)->PtrBuf );
					break;
				case E_GrP4DvlMcmdEditObjSlaveObjGet:
					*((Ptr_GrP4DvlMsgEditObjSlaveObjGet)A_PtrMsg)->PtrRslt	=	LcDvlEditObjSlaveObjGet( ((Ptr_GrP4DvlMsgEditObjSlaveObjGet)A_PtrMsg)->ScptId );
					break;
				case E_GrP4DvlMcmdEditObjPrntObjGet:
					*((Ptr_GrP4DvlMsgEditObjPrntObjGet)A_PtrMsg)->PtrRslt	=	LcDvlEditObjPrntObjGet( ((Ptr_GrP4DvlMsgEditObjPrntObjGet)A_PtrMsg)->ScptId );
					break;
				case E_GrP4DvlMcmdEditObjIsWindow:
					*((Ptr_GrP4DvlMsgEditObjIsWindow)A_PtrMsg)->PtrRslt	=	LcDvlEditObjIsWindow( ((Ptr_GrP4DvlMsgEditObjIsWindow)A_PtrMsg)->ScptId );
					break;
				case E_GrP4DvlMcmdEditObjDelete:
					LcDvlEditObjDelete( ((Ptr_GrP4DvlMsgEditObjDelete)A_PtrMsg)->ScptId );
					break;
				case E_GrP4DvlMcmdEditObjParaUpdt:
					LcDvlEditObjScptParaUpdt( ((Ptr_GrP4DvlMsgEditObjParaUpdt)A_PtrMsg)->ScptId, 
						((Ptr_GrP4DvlMsgEditObjParaUpdt)A_PtrMsg)->PtrScpt, ((Ptr_GrP4DvlMsgEditObjParaUpdt)A_PtrMsg)->IsNoCopy );
					break;
				case E_GrP4DvlMcmdDbgStart:
					LcDvlDbgStart();
					break;
				case E_GrP4DvlMcmdDbgFinish:
					LcDvlDbgFinish();
					break;
				case E_GrP4DvlMcmdIsDbgPause:
					*((Ptr_GrP4DvlMsgIsDbgPause)A_PtrMsg)->PtrRslt	=	LcDvlIsDbgPause();
					break;

				default:
					DbgMsgPrint( "Cls_GrP4DvlMng::RtlMsgProc - bad devlop message ( %d ).\n", ((Ptr_GrP4DvlMsgBase)A_PtrMsg)->Cmd );
					break;
			}
			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_Result	=	Cls_GrP4Mng::RtlMsgProc( A_PtrMsg );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcTimerEmuInit( void )
{
	// local -------------------
	// code --------------------
		m_Gvs.PrgsInit	=	m_Gvs.PrgsInit + 2;
		if ( 100 <= m_Gvs.PrgsInit )
		{
			// next mode
			SysStart();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlIsDbgRun( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrP4DvlDbgModeStop != m_DvlDbgMode)
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::VmRunCode(__u16 A_CodeId, Cls_GrP4DoBase* A_ObjHost, Ptr_GrP4MngVmRpara A_PtrRpara, BOOL8 A_IsRunNow)
{
	// local -------------------
		BOOL8	Tv_Result;		
	// code --------------------
		Tv_Result	=	FALSE;
		if (DvlIsDbgRun())
		{
			Tv_Result	=	Cls_GrP4Mng::VmRunCode( A_CodeId, A_ObjHost, A_PtrRpara );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::LcDvlUiDecPage( __u16 A_ScptId )
{
	// local -------------------
		__u32	Tv_Result;
		__u16	Tv_SlotIdx;
		Cls_GrP4DoWin*	Tv_Win;
		Ptr_GrP4ScptObjHd	Tv_PtrScpt;
		St_GrRect	Tv_RcObj;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrP4ScptIdPageBase <= A_ScptId )
		{
			Tv_SlotIdx	=	A_ScptId - E_GrP4ScptIdPageBase;
			if ( m_PageCnt > Tv_SlotIdx )
			{
				// get script
				if ( NULL != m_PtrPageTbl[Tv_SlotIdx].Pos )
				{
					// script decode
					Tv_PtrScpt	=	(Ptr_GrP4ScptObjHd)m_PtrPageTbl[Tv_SlotIdx].Pos;
					// check script head
					if ( E_GrP4ScptObjStx == Tv_PtrScpt->Stx )
					{
						// check first page class type
						if ( E_GrP4DoClassWin == Tv_PtrScpt->ClassId )
						{
							// check script id
							if ( A_ScptId == Tv_PtrScpt->ScptId )
							{
								// check already exist
								if ( NULL == m_ConTbl[0]->WinFindByScptId( A_ScptId ) )
								{
									// create window
									Tv_RcObj.left		=	0;
									Tv_RcObj.top		=	0;
									Tv_RcObj.right	=	(__s32)m_ConTbl[0]->GetResX();
									Tv_RcObj.bottom	=	(__s32)m_ConTbl[0]->GetResY();
									Tv_Win	=	(Cls_GrP4DoWin*)new Cls_GrP4DoWin( A_ScptId, &Tv_RcObj,0, 0,
										m_ConTbl[0], E_GrP4ScptIdNone );
									// decode other option
									Tv_Result	=	LcUiDecSlave( (Cls_GrP4DoBase*)Tv_Win, Tv_PtrScpt, TRUE );
									// call init
									Tv_Win->RtlInit();
									// request focus change
									m_ConTbl[0]->FocusReset();
									m_ConTbl[0]->FocusCorrect();
									DbgMsgPrint("Page ( %d ) decoded! size = %d byte.\n" , A_ScptId, Tv_Result );

								}
								else
								{
									DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlUiDecPage - page ( %d ) already exist!\n", A_ScptId );
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlUiDecPage - buffer script id  ( %d )  is mismatch orignal id ( %d ) !\n", Tv_PtrScpt->ScptId , A_ScptId  );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlUiDecPage - bad script ( %d )  - class type ( %d ) missmatch!\n" , A_ScptId, Tv_PtrScpt->ClassId );
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlUiDecPage - script ( %d ) error! : bad stx.\n" , A_ScptId );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlUiDecPage - script ( %d ) not exist .\n" , A_ScptId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlUiDecPage - page Id ( %d ) is out of range.\n" , A_ScptId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlVkbdTxtPageCntSet( __u8 A_Lang , __u8 A_Cnt )
{
	// local -------------------
	// code --------------------
		m_PtrCfgPrj->VkbdTbl[A_Lang].PageCnt	=	A_Cnt;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlVkbdTxtKeyCodeSet( __u8 A_Page, __u8 A_Key, __u8 A_Lang, WCHAR A_KeyCode )
{
	// local -------------------
	// code --------------------
		m_PtrCfgPrj->VkbdTbl[A_Lang].KeyTbl[A_Page][A_Key]	=	A_KeyCode;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::DvlScriptDec( __u16 A_PrntId, void* A_PtrScpt )
{
	// local -------------------
		__u32	Tv_Result;
		St_GrP4DvlMsgScriptDec	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdScriptDec;
		Tv_Msg.PrntId				=	A_PrntId;
		Tv_Msg.PtrScpt			=	A_PtrScpt;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::LcDvlScriptDec( __u16 A_PrntId, void* A_PtrScpt )
{
	// local -------------------
		__u32	Tv_Result;
		Ptr_GrP4ScptObjHd	Tv_PtrHd;
		BOOL8	Tv_IsWithPrnt;
		Cls_GrP4DoBase*	Tv_ObjPrnt;
	// code --------------------
		// init
		Tv_Result			=	0;
		Tv_IsWithPrnt	=	FALSE;
		Tv_PtrHd			=	(Ptr_GrP4ScptObjHd)A_PtrScpt;

		// get parent object
		if ( NULL != m_DvlEdtLump )
		{
			Tv_ObjPrnt	=	m_DvlEdtLump->FindObjByScptId( A_PrntId );
			if ( NULL != Tv_ObjPrnt )
			{
				if ( E_GrP4DoClassWin == Tv_PtrHd->ClassId )
				{
					// check parent is window
					if ( Tv_ObjPrnt->IsWindow() )
					{
						Tv_IsWithPrnt	=	TRUE;
					}
				}
				Tv_Result	=	LcUiDecSlave( Tv_ObjPrnt, A_PtrScpt, Tv_IsWithPrnt );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcSlaveIdMarkByScpt( void* A_PtrScpt )
{
	// local -------------------
		__u8*	Tv_PtrScpt;
		__u16	Tv_ScptId;
		__u32	Tv_DwIdx;
		__u32	Tv_BitIdx;
		__u32	Tv_Mask;
		__s32		Tv_Depth;
	// code --------------------
		Tv_Depth		=	0;
		Tv_PtrScpt	=	(__u8*)A_PtrScpt;

		do 
		{
			if ( E_GrP4ScptObjStx == *Tv_PtrScpt )
			{
				Tv_Depth ++;
				// get script id
				Tv_ScptId	=	((Ptr_GrP4ScptObjHd)Tv_PtrScpt)->ScptId;
				// check slave range
				if ( (E_GrP4ScptIdSlaveBase <= Tv_ScptId) && ((E_GrP4ScptIdSlaveBase + E_GrP4DvlSlaveMaxCnt) > Tv_ScptId) )
				{
					// mark slave
					Tv_DwIdx	=	(Tv_ScptId - E_GrP4ScptIdSlaveBase) >> E_GrP4DvlSlaveMapShft;
					Tv_BitIdx	=	(Tv_ScptId - E_GrP4ScptIdSlaveBase) & ((1 << E_GrP4DvlSlaveMapShft) - 1);
					Tv_Mask		=	1 << Tv_BitIdx;
					// add
					m_DvlSlvMap[Tv_DwIdx]	=	m_DvlSlvMap[Tv_DwIdx] | Tv_Mask;
				}
				// go next object
				Tv_PtrScpt	=	(__u8*)( (__u32)Tv_PtrScpt + sizeof(St_GrP4ScptObjHd) + (__u32)((Ptr_GrP4ScptObjHd)Tv_PtrScpt)->ParaSize ) ;
			}
			else if ( E_GrP4ScptObjEdx == *Tv_PtrScpt )
			{
				Tv_Depth --;
				// go next object
				Tv_PtrScpt	=	(__u8*)( (__u32)Tv_PtrScpt + sizeof(St_GrP4ScptObjEnd) ) ;
			}
			else
			{
				DbgMsgPrint("Cls_GrP4DvlMng::LcSlaveIdMarkByScpt - appear bad script depth.\n");
				break;
			}

		} while ( 0 != Tv_Depth );
		
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcSlaveMapRebuild( void )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		// clear slave map
		GrDumyZeroMem( m_DvlSlvMap, sizeof(m_DvlSlvMap) );
		// check window lump
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlWinMaxCnt;Tv_WkIdx++ )
		{
			if ( (NULL != m_DvlScptWinTbl[Tv_WkIdx].Pos) && (0 != m_DvlWinInfo[Tv_WkIdx].Size) )
			{
				// update
				LcSlaveIdMarkByScpt( m_DvlScptWinTbl[Tv_WkIdx].Pos );
			}
		}
		
		// check page lump
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlPageMaxCnt;Tv_WkIdx++ )
		{
			if ( (NULL != m_DvlScptPageTbl[Tv_WkIdx].Pos) && (0 != m_DvlPageInfo[Tv_WkIdx].Size) )
			{
				// update
				LcSlaveIdMarkByScpt( m_DvlScptPageTbl[Tv_WkIdx].Pos );
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSaveHead( void )
{
	// local -------------------
		St_GrP4ScptHdInfo	Tv_Hd;
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
	// code --------------------
		// init
		Tv_Hd.Fcc			=	E_GrP4ScptFcc;
		Tv_Hd.Ver			=	E_GrP4ScptVer;
		Tv_Hd.Time		=	GrTimeGet( &Tv_Mili, &Tv_IsSm );

		m_DvlSaved		=	m_DvlSaved + m_DvlFc->Write( &Tv_Hd, sizeof(Tv_Hd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_Hd);

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSaveCfgPrj( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4ScptDataCfgPrj;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlScptCfgPrj);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlScptCfgPrj, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSaveCfgRtl( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4ScptDataCfgRtl;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlScptCfgRtl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlScptCfgRtl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSaveMdlAtb( __u8 A_MdlIdx )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4ScptDataMdlAtb;
		Tv_DataHd.Id			=	A_MdlIdx;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(St_GrP4ScptCfgMdl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlPrjMdlAtb[A_MdlIdx], Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSaveWinInfo( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4ScptDataWinScptTbl;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlScptWinTbl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptWinTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSaveWinScpt( void )
{
	// local -------------------
		__u16	Tv_WkIdx;
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------

		// script data write
		Tv_DataHd.Type		=	E_GrP4ScptDataWinScptData;
		Tv_DataHd.SubIdx	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlWinMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_DvlScptWinTbl[Tv_WkIdx].Pos )
			{
				Tv_DataHd.Id		=	Tv_WkIdx;
				Tv_DataHd.Size	=	m_DvlWinInfo[Tv_WkIdx].Size;
				m_DvlSaved			=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
				m_DvlSaveReq		=	m_DvlSaveReq + sizeof(Tv_DataHd);
				m_DvlSaved			=	m_DvlSaved + m_DvlFc->Write( m_DvlScptWinTbl[Tv_WkIdx].Pos, Tv_DataHd.Size );	// write scripte
				m_DvlSaveReq		=	m_DvlSaveReq + Tv_DataHd.Size;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSavePageInfo( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4ScptDataPageScptTbl;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlScptPageTbl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptPageTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSavePageScpt( void )
{
	// local -------------------
		__u16	Tv_WkIdx;
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4ScptDataPageScptData;
		Tv_DataHd.SubIdx	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlPageMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_DvlScptPageTbl[Tv_WkIdx].Pos )
			{
				Tv_DataHd.Id		=	Tv_WkIdx;
				Tv_DataHd.Size	=	m_DvlPageInfo[Tv_WkIdx].Size;
				m_DvlSaved			=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
				m_DvlSaveReq		=	m_DvlSaveReq + sizeof(Tv_DataHd);
				m_DvlSaved			=	m_DvlSaved + m_DvlFc->Write( m_DvlScptPageTbl[Tv_WkIdx].Pos, Tv_DataHd.Size );	// write scripte
				m_DvlSaveReq		=	m_DvlSaveReq + Tv_DataHd.Size;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSaveAsmTbl( void )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4ScptDataAsmTbl;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlScptAsmTbl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptAsmTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSaveAsmScpt( void )
{
	// local -------------------
		__u16	Tv_WkIdx;
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		// write assembly code
		Tv_DataHd.Type	=	E_GrP4ScptDataAsmData;
		Tv_DataHd.SubIdx	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlAsmMaxCnt;Tv_WkIdx++ )
		{
			// check exist
			if ( (NULL != m_DvlScptAsmTbl[Tv_WkIdx].Pos) && (0 != m_DvlScptAsmTbl[Tv_WkIdx].Cnt) )
			{
				// write header
				Tv_DataHd.Id		=	Tv_WkIdx;
				Tv_DataHd.Size	=	E_GrP4AsmOpUnitSize * (__u32)m_DvlScptAsmTbl[Tv_WkIdx].Cnt;
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
				m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptAsmTbl[Tv_WkIdx].Pos, Tv_DataHd.Size );	// write script head
				m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSavePstTbl( __u8 A_Skin )
{
	// local -------------------
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP4ScptDataPstTbl;
		Tv_DataHd.Id			=	A_Skin;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(St_GrP4ScptPstItm) * E_GrP4DvlPstMaxCnt;
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptPstTbl[A_Skin], Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSavePstScpt( __u8 A_Skin )
{
	// local -------------------
		__u16	Tv_WkIdx;
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		// script data write
		Tv_DataHd.Type		=	E_GrP4ScptDataPstScptData;
		Tv_DataHd.SubIdx	=	A_Skin;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4DvlPstMaxCnt;Tv_WkIdx++ )
		{
			if ( (NULL != m_DvlScptPstTbl[A_Skin][Tv_WkIdx].Pos) && (0 != m_DvlScptPstTbl[A_Skin][Tv_WkIdx].Cnt) )
			{
				Tv_DataHd.Id		=	Tv_WkIdx;
				Tv_DataHd.Size	=	sizeof(St_GrP4ScptPstData) * m_DvlScptPstTbl[A_Skin][Tv_WkIdx].Cnt;
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
				m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptPstTbl[A_Skin][Tv_WkIdx].Pos, Tv_DataHd.Size );	// write scripte
				m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSaveFontData( __u8 A_Skin )
{
	// local -------------------
		__u16	Tv_WkIdx;
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		// script data write
		Tv_DataHd.Type		=	E_GrP4ScptDataFontData;
		Tv_DataHd.SubIdx	=	A_Skin;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4MngFontMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_DvlScptFontTbl[A_Skin][Tv_WkIdx] ) 
			{
				Tv_DataHd.Id		=	Tv_WkIdx;
				Tv_DataHd.Size	=	GrP4FontCalcSize( m_DvlScptFontTbl[A_Skin][Tv_WkIdx] );
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
				m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptFontTbl[A_Skin][Tv_WkIdx], Tv_DataHd.Size );	// write scripte
				m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSaveImgData( __u8 A_Skin )
{
	// local -------------------
		__u16	Tv_WkIdx;
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		// script data write
		Tv_DataHd.Type		=	E_GrP4ScptDataImageData;
		Tv_DataHd.SubIdx	=	A_Skin;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4MngImgScptMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_DvlScptImgTbl[A_Skin][Tv_WkIdx] ) 
			{
				Tv_DataHd.Id		=	Tv_WkIdx;
				Tv_DataHd.Size	=	m_DvlScptImgTbl[A_Skin][Tv_WkIdx]->Size;
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
				m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptImgTbl[A_Skin][Tv_WkIdx], Tv_DataHd.Size );	// write scripte
				m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcScptSaveTxtData( void )
{
	// local -------------------
		__u16	Tv_WkIdx;
		__u8	Tv_LngIdx;
		St_GrP4ScptDataHd	Tv_DataHd;
	// code --------------------
		// script data write
		Tv_DataHd.Type	=	E_GrP4ScptDataTextData;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4MngTextScptMaxCnt;Tv_WkIdx++ )
		{
			Tv_DataHd.Id		=	Tv_WkIdx;
			for ( Tv_LngIdx=0;Tv_LngIdx < E_GrP4ScptLangMaxCnt;Tv_LngIdx++ )
			{
				if ( NULL != m_DvlScptTxtTbl[Tv_WkIdx].LngTbl[Tv_LngIdx] )
				{
					Tv_DataHd.SubIdx	=	Tv_LngIdx;
					Tv_DataHd.Size	=	GrStrWlen( m_DvlScptTxtTbl[Tv_WkIdx].LngTbl[Tv_LngIdx] );
					if ( 0 != Tv_DataHd.Size )
					{
						Tv_DataHd.Size	=	((Tv_DataHd.Size + 2) & 0xFFFFFFFE) << 1;	// patch data size
						m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
						m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
						m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptTxtTbl[Tv_WkIdx].LngTbl[Tv_LngIdx], Tv_DataHd.Size );	// write scripte
						m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
					}
				}
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlScptExport(WCHAR* A_StrFn, __u8 A_Model, BOOL8 A_IsAdd)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_ModelIdx;
		__u8	Tv_SkinIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		m_DvlSaved		=	0;
		m_DvlSaveReq	=	0;
		Tv_ModelIdx		=	A_Model;
		if ( E_GrP4DvlModelThis == A_Model )
		{
			Tv_ModelIdx		=	m_DvlPrjRtl.ModelIdx;
		}
		Tv_SkinIdx			=	m_DvlPrjModel[Tv_ModelIdx].SkinIdx;
		
		// check project exist
		if ( DvlIsPrjExist() )
		{
			// open file
			m_DvlFc	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, TRUE, TRUE, FALSE, TRUE );
			if ( m_DvlFc->IsOpened() )		// check file open success
			{
				if ( A_IsAdd )
				{
					m_DvlFc->Seek(0, E_GrFileCtrlPosEnd);
				}

				// script header
				LcScptSaveHead();

				// script config project
				LcScptSaveCfgPrj();

				// script config runtime
				LcScptSaveCfgRtl();

				// script config model attribute
				LcScptSaveMdlAtb( Tv_ModelIdx );

				// window write
				LcScptSaveWinInfo();
				LcScptSaveWinScpt();

				// page write
				LcScptSavePageInfo();
				LcScptSavePageScpt();

				// assembly write
				LcScptSaveAsmTbl();
				LcScptSaveAsmScpt();

				// paint script write
				LcScptSavePstTbl( Tv_SkinIdx );
				LcScptSavePstScpt( Tv_SkinIdx );

				// font write
				LcScptSaveFontData( Tv_SkinIdx );

				// image write
				LcScptSaveImgData( Tv_SkinIdx );

				// text write
				LcScptSaveTxtData();

				// update header
				/*
				m_DvlFc->Seek( 0 );
				m_DvlFc->Write( &Tv_ScptHd, sizeof(Tv_ScptHd) );
				*/

				// check success
				if ( m_DvlSaved == m_DvlSaveReq )
				{
					Tv_Result	=	TRUE;
					DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlScptExport - script file %d bytes saved.\n" , m_DvlSaved );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::LcDvlScptExport - script file save requested as %d bytes, but %d bytes saved.\n", m_DvlSaveReq, m_DvlSaved );
				}

			}
			delete	m_DvlFc;
			m_DvlFc	=	NULL;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::KeyInput( __u8 A_Key, BOOL8 A_IsPush, __u8 A_DevType )
{
		if ((E_GrP4DvlDbgModeRun == m_DvlDbgMode) || (E_GrP4DvlDbgModeStep == m_DvlDbgMode))
		{
			Cls_GrP4Mng::KeyInput( A_Key, A_IsPush, A_DevType );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::MousePosInput( __s32 A_X, __s32 A_Y, BOOL8 A_IsAbs )
{
		if ((E_GrP4DvlDbgModeRun == m_DvlDbgMode) || (E_GrP4DvlDbgModeStep == m_DvlDbgMode))
		{
			Cls_GrP4Mng::MousePosInput( A_X, A_Y, A_IsAbs );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::MouseBtnInput( __u8 A_BtnIdx, BOOL8 A_IsPush )
{
		if ((E_GrP4DvlDbgModeRun == m_DvlDbgMode) || (E_GrP4DvlDbgModeStep == m_DvlDbgMode))
		{
			Cls_GrP4Mng::MouseBtnInput( A_BtnIdx, A_IsPush );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::MouseWheelInput( BOOL8 A_IsDown )
{
		if ((E_GrP4DvlDbgModeRun == m_DvlDbgMode) || (E_GrP4DvlDbgModeStep == m_DvlDbgMode))
		{
			Cls_GrP4Mng::MouseWheelInput( A_IsDown );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlEditObjScrRectGet(__u16 A_ScptId, Ptr_GrRect A_PtrRtRect, BOOL8 A_IsNoChk)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Result	=	Tv_Obj->ScrRectGet( A_PtrRtRect, A_IsNoChk );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlEditObjCliRectGet(__u16 A_ScptId, Ptr_GrRect A_PtrRtRect, BOOL8 A_IsNoChk)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrP4ScptIdNone != A_ScptId)
		{
			// find object
			if (NULL != m_DvlEdtLump)
			{
				if (A_ScptId == m_DvlEdtLump->ScptIdGet())
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId(A_ScptId);
				}
				if (NULL != Tv_Obj)
				{
					Tv_Result	=	Tv_Obj->CliRectGet(A_PtrRtRect, A_IsNoChk);
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlEditObjPosSet(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Obj->PosSet( A_PtrRect,A_PosRel );
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlEditObjPosGet(__u16 A_ScptId, Ptr_GrRect A_PtrRtRect, __u8* A_PtrRtPosRel)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Obj->PosGet( A_PtrRtRect,A_PtrRtPosRel );
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlEditObjPosMoveAbs(__u16 A_ScptId, __s32 A_X, __s32 A_Y)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrP4ScptIdNone != A_ScptId)
		{
			// find object
			if (NULL != m_DvlEdtLump)
			{
				if (A_ScptId == m_DvlEdtLump->ScptIdGet())
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId(A_ScptId);
				}
				if (NULL != Tv_Obj)
				{
					Tv_Obj->PosMoveAbs(A_X, A_Y);
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlEditObjPosMoveRel(__u16 A_ScptId, __s32 A_X, __s32 A_Y)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrP4ScptIdNone != A_ScptId)
		{
			// find object
			if (NULL != m_DvlEdtLump)
			{
				if (A_ScptId == m_DvlEdtLump->ScptIdGet())
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId(A_ScptId);
				}
				if (NULL != Tv_Obj)
				{
					Tv_Obj->PosMoveRel(A_X, A_Y);
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlEditObjSizeSetAbs(__u16 A_ScptId, __s32 A_X, __s32 A_Y)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrP4ScptIdNone != A_ScptId)
		{
			// find object
			if (NULL != m_DvlEdtLump)
			{
				if (A_ScptId == m_DvlEdtLump->ScptIdGet())
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId(A_ScptId);
				}
				if (NULL != Tv_Obj)
				{
					Tv_Obj->SizeSetAbs(A_X, A_Y);
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlEditObjSizeSetRel(__u16 A_ScptId, __s32 A_X, __s32 A_Y)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrP4ScptIdNone != A_ScptId)
		{
			// find object
			if (NULL != m_DvlEdtLump)
			{
				if (A_ScptId == m_DvlEdtLump->ScptIdGet())
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId(A_ScptId);
				}
				if (NULL != Tv_Obj)
				{
					Tv_Obj->SizeSetRel(A_X, A_Y);
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlEditObjFindByPos(__s32 A_X, __s32 A_Y, BOOL8 A_IsExcFrm)
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
		Cls_GrP4DoBase*	Tv_Frm;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;

		// find object
		if ( NULL != m_DvlEdtLump )
		{
			Tv_Obj	=	m_DvlEdtLump->FindObjByPos(A_X, A_Y);
			if (NULL != Tv_Obj)
			{
				Tv_Result	=	Tv_Obj->ScptIdGet();
				// check frame slave
				if (A_IsExcFrm)
				{
					while (NULL != Tv_Obj)
					{
						// check frame
						Tv_Frm	=	Tv_Obj->FrameGet();
						if (NULL == Tv_Frm)
						{
							break;
						}

						// update
						Tv_Result	=	Tv_Frm->ScptIdGet();
						Tv_Obj		=	Tv_Frm->ParentObjGet();
					}
				}
				
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP4DvlMng::DvlEditObjClassIdGet( __u16 A_ScptId )
{
	// local -------------------
		__u8	Tv_Result;
		St_GrP4DvlMsgEditObjClassIdGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP4DoClassNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditObjClassIdGet;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP4DvlMng::LcDvlEditObjClassIdGet( __u16 A_ScptId )
{
	// local -------------------
		__u8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	E_GrP4DoClassNone;

		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Result	=	E_GrP4DoClassWin;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
					if ( NULL != Tv_Obj )
					{
						Tv_Result	=	Tv_Obj->ClassIdGet();
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::DvlEditObjScptDump( __u16 A_ScptId, void* A_PtrBuf, BOOL8 A_IsCopyMode )
{
	// local -------------------
		__u32	Tv_Result;
		St_GrP4DvlMsgEditObjScptDump	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditObjScptDump;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrBuf				=	A_PtrBuf;
		Tv_Msg.IsCopyMode		=	A_IsCopyMode;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::LcDvlEditObjScptDump( __u16 A_ScptId, void* A_PtrBuf, BOOL8 A_IsCopyMode )
{
	// local -------------------
		__u32	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	0;

		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Result	=	Tv_Obj->RtlScptDump( A_PtrBuf, A_IsCopyMode );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlEditObjFrameGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP4DvlMsgEditObjFrameGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditObjFrameGet;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::LcDvlEditObjFrameGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
		Cls_GrP4DoBase*	Tv_Frm;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;

		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	NULL;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Frm		=	Tv_Obj->FrameGet();
					if ( NULL != Tv_Frm )
					{
						Tv_Result	=	Tv_Frm->ScptIdGet();
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlEditObjRedraw( __u16 A_ScptId )
{
	// local -------------------
		St_GrP4DvlMsgEditObjRedraw	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditObjRedraw;
		Tv_Msg.ScptId				=	A_ScptId;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlEditObjRedraw( __u16 A_ScptId )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Obj->ReqDraw();
				}
			}
		}

}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::DvlEditObjListByPos( __s32 A_X, __s32 A_Y, __u16* A_PtrBuf )
{
	// local -------------------
		__u32	Tv_Result;
		St_GrP4DvlMsgEditObjListByPos	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditObjListByPos;
		Tv_Msg.X						=	A_X;
		Tv_Msg.Y						=	A_Y;
		Tv_Msg.PtrBuf				=	A_PtrBuf;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::LcDvlEditObjListByPos( __s32 A_X, __s32 A_Y, __u16* A_PtrBuf )
{
	// local -------------------
	__u32	Tv_Result;
	__u32	Tv_ListTbl[32];
	__u32	Tv_WkIdx;
	// code --------------------
	Tv_Result	=	0;

	// find object
	if ( NULL != m_ConTbl[0] )
	{
		Tv_Result	=	m_ConTbl[0]->ObjListByPos( A_X, A_Y, Tv_ListTbl );
		if ( 0 != Tv_Result )
		{
			for ( Tv_WkIdx=0;Tv_WkIdx < Tv_Result;Tv_WkIdx++ )
			{
				A_PtrBuf[Tv_WkIdx]	=	E_GrP4ScptIdNone;
				if ( 0 != Tv_ListTbl[Tv_WkIdx] )
				{
					A_PtrBuf[Tv_WkIdx]	=	((Cls_GrP4DoBase*)Tv_ListTbl[Tv_WkIdx])->ScptIdGet();
				}
			}
		}
	}

	return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlEditObjSlaveObjGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP4DvlMsgEditObjSlaveObjGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditObjSlaveObjGet;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::LcDvlEditObjSlaveObjGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
		Cls_GrP4DoBase*	Tv_Slave;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;

		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Slave	=	Tv_Obj->SlaveObjGet();
					if ( NULL != Tv_Slave )
					{
						Tv_Result	=	Tv_Slave->ScptIdGet();
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlEditObjNextObjGet(__u16 A_ScptId)
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
		Cls_GrP4DoBase*	Tv_Next;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;

		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Next	=	Tv_Obj->NextObjGet();
					if ( NULL != Tv_Next )
					{
						Tv_Result	=	Tv_Next->ScptIdGet();
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::DvlEditObjPrntObjGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP4DvlMsgEditObjPrntObjGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditObjPrntObjGet;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DvlMng::LcDvlEditObjPrntObjGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
		Cls_GrP4DoBase*	Tv_Prnt;
	// code --------------------
		Tv_Result	=	E_GrP4ScptIdNone;

		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	NULL;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Prnt	=	Tv_Obj->NextObjGet();
					if ( NULL != Tv_Prnt )
					{
						Tv_Result	=	Tv_Prnt->ScptIdGet();
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlEditObjIsWindow( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP4DvlMsgEditObjIsWindow	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditObjIsWindow;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::LcDvlEditObjIsWindow( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			if ( NULL != m_DvlEdtLump )
			{
				// find object
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Result	=	TRUE;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
					if ( NULL != Tv_Obj )
					{
						Tv_Result	=	Tv_Obj->IsWindow();
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlEditObjDelete( __u16 A_ScptId )
{
	// local -------------------
		St_GrP4DvlMsgEditObjDelete	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditObjDelete;
		Tv_Msg.ScptId				=	A_ScptId;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlEditObjDelete( __u16 A_ScptId )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				if ( NULL != Tv_Obj )
				{
					Tv_Obj->RtlScptDelMode();
					delete	Tv_Obj;
				}
			}
		}

}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::DvlEditObjParaInfoDump( __u16 A_ScptId, void* A_PtrBuf )
{
	// local -------------------
		__u32	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	0;

		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Result	=	Tv_Obj->RtlParaInfoDump( A_PtrBuf );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlEditObjScptParaUpdt( __u16 A_ScptId, void* A_PtrScpt, BOOL8 A_IsNoCopy )
{
	// local -------------------
		St_GrP4DvlMsgEditObjParaUpdt	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdEditObjParaUpdt;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrScpt			=	A_PtrScpt;
		Tv_Msg.IsNoCopy			=	A_IsNoCopy;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlEditObjScptParaUpdt( __u16 A_ScptId, void* A_PtrScpt, BOOL8 A_IsNoCopy )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Obj->RtlScptParaUpdt( A_PtrScpt, A_IsNoCopy );
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlIsDbgPause( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		//St_GrP4DvlMsgIsDbgPause	Tv_Msg;
	// code --------------------
		/*
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4DvlMcmdIsDbgPause;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		*/
		Tv_Result	=	FALSE;
		if ( E_GrP4DvlDbgModePause == m_DvlDbgMode )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::LcDvlIsDbgPause( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4DvlDbgModePause == m_DvlDbgMode )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlDbgResume( void )
{
		if (E_GrP4DvlDbgModePause == m_DvlDbgMode)
		{
			m_DvlDbgIsResume	=	TRUE;
			m_DvlDbgMode	=	E_GrP4DvlDbgModeRun;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlDbgStep(__u8 A_VmIdx)
{
		if (E_GrP4DvlDbgModePause == m_DvlDbgMode)
		{
			m_DvlDbgStepVm		=	A_VmIdx;
			m_DvlDbgIsResume	=	TRUE;
			m_DvlDbgMode			=	E_GrP4DvlDbgModeStep;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlEditObjInfoT1(__u16 A_ScptId, __u8* A_PtrRtClassType, Ptr_GrRect A_PtrRtRect, __u8* A_PtrRtPosRel, __u16* A_PtrRtSlaveId,
	__u16* A_PtrRtNextId )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP4DoBase*	Tv_Obj;
		Cls_GrP4DoBase*	Tv_Ref;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP4ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP4DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					*A_PtrRtClassType	=	Tv_Obj->ClassIdGet();
					Tv_Obj->PosGet( A_PtrRtRect, A_PtrRtPosRel );

					*A_PtrRtSlaveId		=	E_GrP4ScptIdNone;
					Tv_Ref	=	Tv_Obj->SlaveObjGet();
					if ( NULL != Tv_Ref )
					{
						*A_PtrRtSlaveId		=	Tv_Ref->ScptIdGet();
					}
					
					*A_PtrRtNextId		=	E_GrP4ScptIdNone;
					Tv_Ref	=	Tv_Obj->NextObjGet();
					if ( NULL != Tv_Ref )
					{
						*A_PtrRtNextId		=	Tv_Ref->ScptIdGet();
					}

					Tv_Result	=	TRUE;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlRatbUpdt( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		m_DvlRatbTimer ++;
		if ( E_GrP4DvlMngRatbTime <= m_DvlRatbTimer )
		{
			// change attribute
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
			{
				if ( DvlIsDbgRun() )
				{
					m_LiveAtbTbl[Tv_WkIdx] ++;
					if ( 8 == m_LiveAtbTbl[Tv_WkIdx] )
					{
						m_Gvs.VlossMap	=	m_Gvs.VlossMap | (1 << Tv_WkIdx);
					}
					else if ( 8 < m_LiveAtbTbl[Tv_WkIdx] )
					{
						m_LiveAtbTbl[Tv_WkIdx]	=	0;
						m_Gvs.VlossMap	=	m_Gvs.VlossMap & ~(1 << Tv_WkIdx);
					}

					m_PlayAtbTbl[Tv_WkIdx] ++;
					if ( 8 <= m_PlayAtbTbl[Tv_WkIdx] )
					{
						m_PlayAtbTbl[Tv_WkIdx]	=	0;
					}
				}
				else
				{
					m_LiveAtbTbl[Tv_WkIdx]	=	0xFF;
					m_PlayAtbTbl[Tv_WkIdx]	=	0xFF;
				}
				
			}

			// rest 
			m_DvlRatbTimer	=	0;
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlAsmBreakPointToggle( __u16 A_Id, __u16 A_Ip )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrP4ScptAsmInfo	Tv_PtrInfo;
		Ptr_GrP4AsmOpBase	Tv_PtrOp;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrInfo	=	AsmPtrGet( A_Id );
		if ( NULL != Tv_PtrInfo )
		{
			if ( (NULL != Tv_PtrInfo->Pos) && (A_Ip < Tv_PtrInfo->Cnt) )
			{
				Tv_PtrOp	=	(Ptr_GrP4AsmOpBase)Tv_PtrInfo->Pos;
				Tv_PtrOp[A_Ip].Code	=	Tv_PtrOp[A_Ip].Code ^ E_GrP4AsmOpBrkPntMask;
				Tv_Result	=	TRUE;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlAsmActiveToggle( __u16 A_Id, __u16 A_Ip )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrP4ScptAsmInfo	Tv_PtrInfo;
		Ptr_GrP4AsmOpBase	Tv_PtrOp;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrInfo	=	AsmPtrGet( A_Id );
		if ( NULL != Tv_PtrInfo )
		{
			if ( (NULL != Tv_PtrInfo->Pos) && (A_Ip < Tv_PtrInfo->Cnt) )
			{
				Tv_PtrOp	=	(Ptr_GrP4AsmOpBase)Tv_PtrInfo->Pos;
				Tv_PtrOp[A_Ip].Code	=	Tv_PtrOp[A_Ip].Code ^ E_GrP4AsmOpDeactMask;
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlFwUpgdRun( __u32 A_CtlgHndl )
{
	// local -------------------
	// code --------------------
		// message
		DbgMsgPrint( "Firmware Upgrade : catalog id %d\n", A_CtlgHndl );

		// request upgrade
		m_SysStat	=	E_GrP4MngSysStatFwUpgd;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDispIdTblBuild( void )
{
	// local -------------------
	// code --------------------
		m_DvlEdtIdDispCnt	=	0;
		GrDumyZeroMem( m_DvlEdtIdDispTbl, sizeof(m_DvlEdtIdDispTbl) );
		if ( NULL != m_DvlEdtLump )
		{
			LcDispIdSlaveAdd( m_DvlEdtLump );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDispIdSlaveAdd( Cls_GrP4DoBase* A_Obj )
{
	// local -------------------
		Cls_GrP4DoBase*	Tv_Obj;
	// code --------------------
		if ( E_GrP4DvlLumpSlvMax >  m_DvlEdtIdDispCnt  )
		{
			// get info
			Tv_Obj	=	A_Obj->SlaveObjGet();
			while ( NULL != Tv_Obj )
			{
				// add now information
				if ( Tv_Obj->ScrRectGet( &m_DvlEdtIdDispTbl[m_DvlEdtIdDispCnt].Rc ) )
				{
					// add information
					m_DvlEdtIdDispTbl[m_DvlEdtIdDispCnt].Id	=	Tv_Obj->ScptIdGet();
					m_DvlEdtIdDispCnt ++;
					// slave add
					LcDispIdSlaveAdd( Tv_Obj );
				}
				// next
				Tv_Obj	=	Tv_Obj->NextObjGet();
			}
		}
}
//--------------------------------------------------------------------
Ptr_GrP4DvlObjInfo	Cls_GrP4DvlMng::DvlDispIdTblGet( void )
{
		return	m_DvlEdtIdDispTbl;
}
//--------------------------------------------------------------------
Ptr_GrP4PrjBldFile	Cls_GrP4DvlMng::DvlBldFwInfoGet(__u8 A_ModelIdx)
{
		return	&m_DvlBldInfo[A_ModelIdx];
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlLanCblUpdt( void )
{
	// local -------------------
		BOOL8	Tv_IsLanCbl;
		St_GrTaskMsgHd	Tv_Msg;
	// code --------------------
		m_DvlLanCblTimer ++;
		Tv_IsLanCbl	=	TRUE;
		if ( (300 < m_DvlLanCblTimer) && (600 > m_DvlLanCblTimer) )
		{
			Tv_IsLanCbl	=	FALSE;
		}
		else if ( 600 <= m_DvlLanCblTimer )
		{
			m_DvlLanCblTimer	=	0;
		}

		// check change status
		if ( m_Gvs.IsLanOn != Tv_IsLanCbl )
		{
			// update
			m_Gvs.IsLanOn	=	Tv_IsLanCbl;
			// notify message
			Tv_Msg.Cmd		=	E_GrTaskMsgNetOff;
			if ( Tv_IsLanCbl )
			{
				Tv_Msg.Cmd		=	E_GrTaskMsgNetOn;
			}
			m_TaskSvr->MsgPost( &Tv_Msg, sizeof(Tv_Msg) );
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvrUsbUpdt( void )
{
	// local -------------------
		St_GrTaskMsgHd	Tv_Msg;
	// code --------------------
		m_DvlUsbTimer ++;
		if ( 60 <= m_DvlUsbTimer )
		{
			m_Gvs.UsbMemCnt	=	m_Gvs.UsbMemCnt ^ 1;
			m_DvlUsbTimer	=	0;

			Tv_Msg.Cmd	=	E_GrTaskMsgUsbChg;
			m_TaskSvr->MsgPost( &Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvrCmsUpdt( void )
{
	// local -------------------
		St_GrTaskMsgHd	Tv_Msg;
	// code --------------------
		m_DvlCmsTimer ++;
		if ( 40 <= m_DvlCmsTimer )
		{
			m_Gvs.CmsCliCnt	=	m_Gvs.CmsCliCnt ^ 1;
			m_DvlCmsTimer	=	0;

			Tv_Msg.Cmd	=	E_GrTaskMsgCmsCliChg;
			m_TaskSvr->MsgPost( &Tv_Msg, sizeof(Tv_Msg) );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvrBkupUpdt( void )
{
	// local -------------------
		St_GrTaskMsgHd	Tv_Msg;
	// code --------------------
		if ( m_IsDvlBkupRun )
		{
			m_Gvs.BkupTimePrgs	=	m_Gvs.BkupTimePrgs + 120;
			if ( m_Gvs.BkupTimePrgs > m_Gvs.BkupTimeEd )
			{
				// finished
				if ( m_IsDvlBkupErr || (5 == m_Gvs.BkupErrCode) )
				{
					if ( 5 != m_Gvs.BkupErrCode )
					{
						m_Gvs.BkupErrCode	=	3;
					}
					Tv_Msg.Cmd	=	E_GrTaskMsgBkupError;
					GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );

				}

				if ( 0 == m_Gvs.BkupErrCode )
				{
					Tv_Msg.Cmd	=	E_GrTaskMsgBkupEnd;
					GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );
				}
				
				// update
				m_IsDvlBkupErr	=	!m_IsDvlBkupErr;
				m_IsDvlBkupRun	=	FALSE;
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvrLogBkupUpdt( void )
{
	// local -------------------
		St_GrTaskMsgHd	Tv_Msg;
	// code --------------------
		if ( m_IsDvlLogBkupRun )
		{
			m_Gvs.LogPrgs	++;
			if ( m_Gvs.LogPrgs >= m_Gvs.LogTotal )
			{
				// finished
				if ( m_IsDvlLogBkupErr || (5 == m_Gvs.LogErrCode) )
				{
					if ( 5 != m_Gvs.LogErrCode )
					{
						m_Gvs.LogErrCode	=	3;
					}
					Tv_Msg.Cmd	=	E_GrTaskMsgLogBkupError;
					GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );

				}

				if ( 0 == m_Gvs.LogErrCode )
				{
					Tv_Msg.Cmd	=	E_GrTaskMsgLogBkupEnd;
					GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );
				}

				// update
				m_IsDvlLogBkupErr	=	!m_IsDvlLogBkupErr;
				m_IsDvlLogBkupRun	=	FALSE;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlBkupReq( __u8 A_Media, __u8 A_RecDev, __u16 A_ChMap,
	Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, BOOL8 A_IsCmbViewer )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrTaskMsgHd	Tv_Msg;
		Def_StrName	Tv_StrMedia;
		Def_StrTag	Tv_StrTmSt;
		Def_StrTag	Tv_StrTmEd;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrRecDevMirror >= A_RecDev )
		{
			if ( 0 != A_ChMap )
			{
				if ( A_TimeSt <= A_TimeEd )
				{
					if ( !m_IsDvlBkupRun )
					{
						m_Gvs.BkupMedia		=	A_Media;
						m_Gvs.BkupRecDev	=	A_RecDev;
						m_Gvs.BkupTimeSt	=	A_TimeSt;
						m_Gvs.BkupTimeEd	=	A_TimeEd;
						m_Gvs.BkupChMap		=	A_ChMap;
						m_Gvs.IsBkupCmbViewer	=	A_IsCmbViewer;

						m_Gvs.BkupTimePrgs	=	A_TimeSt;
						m_Gvs.BkupErrCode	=	0;

						if ( 0 == A_Media )
						{
							GrStrCopy( Tv_StrMedia, "USB" );
						}
						else
						{
							GrStrCopy( Tv_StrMedia, "ODD" );
						}
						
						GrStrTimeToStr( Tv_StrTmSt, A_TimeSt );
						GrStrTimeToStr( Tv_StrTmEd, A_TimeEd );

						DbgMsgPrint( "Cls_GrP4DvlMng::DvlBkupReq - try backup %s dev : %d ch : %04X  time : %s ~ %s .\n", 
							Tv_StrMedia, A_RecDev, A_ChMap, Tv_StrTmSt, Tv_StrTmEd );

						// notify backup start
						Tv_Msg.Cmd	=	E_GrTaskMsgBkupStart;
						GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );

						m_IsDvlBkupRun		=	TRUE;

						Tv_Result	=	TRUE;
					}
					else
					{
						DbgMsgPrint( "Cls_GrP4DvlMng::DvlBkupReq - backup already progress!\n" );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::DvlBkupReq - bad time range!\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4DvlMng::DvlBkupReq - channel map is zero!\n" );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DvlMng::DvlBkupReq - record device input bad!\n" );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlBkupCancel( void )
{
		if ( m_IsDvlBkupRun )
		{
			m_Gvs.BkupErrCode		=	5;
			m_Gvs.BkupTimePrgs	=	m_Gvs.BkupTimeEd;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4DvlMng::DvlLogBkupReq( __u8 A_RecDev, __u16 A_ChMap, __u16 A_EvtMap,
	Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrTaskMsgHd	Tv_Msg;
		Def_StrTag	Tv_StrFn;
		Def_StrTag	Tv_StrTmSt;
		Def_StrTag	Tv_StrTmEd;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrRecDevMirror >= A_RecDev )
		{
			if ( 0 != A_ChMap )
			{
				if ( A_TimeSt <= A_TimeEd )
				{
					if ( !m_IsDvlLogBkupRun )
					{
						m_Gvs.LogRecDev	=	A_RecDev;
						m_Gvs.LogTimeSt	=	A_TimeSt;
						m_Gvs.LogTimeEd	=	A_TimeEd;
						m_Gvs.LogChMap	=	A_ChMap;
						m_Gvs.LogEvtMap	=	A_EvtMap;

						m_Gvs.LogPrgs			=	0;
						m_Gvs.LogTotal		=	30;
						m_Gvs.LogErrCode	=	0;

						GrStrTimeToStr( Tv_StrTmSt, A_TimeSt );
						GrStrTimeToStr( Tv_StrTmEd, A_TimeEd );

						GrStrWstrToStr( Tv_StrFn, A_StrFn );

						DbgMsgPrint( "Cls_GrP4DvlMng::DvlBkupReq - try log backup %s dev : %d ch : %04X  time : %s ~ %s .\n", 
							Tv_StrFn, A_RecDev, A_ChMap, Tv_StrTmSt, Tv_StrTmEd );

						// notify backup start
						Tv_Msg.Cmd	=	E_GrTaskMsgLogBkupStart;
						GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );

						m_IsDvlLogBkupRun		=	TRUE;

						Tv_Result	=	TRUE;
					}
					else
					{
						DbgMsgPrint( "Cls_GrP4DvlMng::DvlLogBkupReq - log backup already progress!\n" );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::DvlLogBkupReq - bad time range!\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP4DvlMng::DvlLogBkupReq - channel map is zero!\n" );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP4DvlMng::DvlLogBkupReq - record device input bad!\n" );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlLogBkupCancel( void )
{
		if ( m_IsDvlLogBkupRun )
		{
			m_Gvs.LogErrCode		=	5;
			m_Gvs.LogPrgs				=	m_Gvs.LogTotal;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::RtlAtLockProc( void )
{
	// local -------------------
	// code --------------------
	if ((E_GrP4DvlDbgModeRun == m_DvlDbgMode) || (E_GrP4DvlDbgModeStep == m_DvlDbgMode))
		{
			Cls_GrP4Mng::RtlAtLockProc();
		}
		else
		{
			m_IsAtLock	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlSkinCopy( __u8 A_TgIdx, __u8 A_SrIdx )
{
	// local -------------------
		__u32	Tv_WkIdx;
		__u32	Tv_Size;
	// code --------------------
		// check source and target index
	if ( (A_TgIdx != A_SrIdx) && (E_GrP4DvlSkinMaxCnt > A_TgIdx) && (E_GrP4DvlSkinMaxCnt > A_SrIdx) )
	{
		// copy
		// script
		for (Tv_WkIdx=0;Tv_WkIdx <E_GrP4DvlPstMaxCnt;Tv_WkIdx++ )
		{
			// remove target skin
			if ( NULL != m_DvlScptPstTbl[A_TgIdx][Tv_WkIdx].Pos )
			{
				free( m_DvlScptPstTbl[A_TgIdx][Tv_WkIdx].Pos );
				m_DvlScptPstTbl[A_TgIdx][Tv_WkIdx].Pos	=	NULL;
			}
			m_DvlScptPstTbl[A_TgIdx][Tv_WkIdx].Cnt	=	0;
			// check exist source
			if ( ( NULL != m_DvlScptPstTbl[A_SrIdx][Tv_WkIdx].Pos ) && (0 != m_DvlScptPstTbl[A_SrIdx][Tv_WkIdx].Cnt)  )
			{
				// copy
				m_DvlScptPstTbl[A_TgIdx][Tv_WkIdx].Pos	=	malloc( E_GrP4DvlPstBufMax );
				if ( NULL != m_DvlScptPstTbl[A_TgIdx][Tv_WkIdx].Pos )
				{
					m_DvlScptPstTbl[A_TgIdx][Tv_WkIdx].Cnt	=	m_DvlScptPstTbl[A_SrIdx][Tv_WkIdx].Cnt;
					GrDumyCopyMem( m_DvlScptPstTbl[A_TgIdx][Tv_WkIdx].Pos, m_DvlScptPstTbl[A_SrIdx][Tv_WkIdx].Pos, E_GrP4DvlPstBufMax );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::DvlSkinCopy - Paint scipt copy error!\n" );
				}
			}
		}

		// image
		for (Tv_WkIdx=0;Tv_WkIdx <E_GrP4MngImgScptMaxCnt;Tv_WkIdx++ )
		{
			// remove target image
			if ( NULL != m_DvlScptImgTbl[A_TgIdx][Tv_WkIdx] )
			{
				free( m_DvlScptImgTbl[A_TgIdx][Tv_WkIdx] );
				m_DvlScptImgTbl[A_TgIdx][Tv_WkIdx]	=	NULL;
			}
			// check exist source
			if ( NULL != m_DvlScptImgTbl[A_SrIdx][Tv_WkIdx] )
			{
				// copy
				m_DvlScptImgTbl[A_TgIdx][Tv_WkIdx]	=	(Ptr_GrGdib)malloc( m_DvlScptImgTbl[A_SrIdx][Tv_WkIdx]->Size );
				if ( NULL != m_DvlScptImgTbl[A_TgIdx][Tv_WkIdx] )
				{
					GrDumyCopyMem( m_DvlScptImgTbl[A_TgIdx][Tv_WkIdx], m_DvlScptImgTbl[A_SrIdx][Tv_WkIdx], m_DvlScptImgTbl[A_SrIdx][Tv_WkIdx]->Size );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::DvlSkinCopy - image copy error!\n" );
				}
			}
		}

		// font
		for (Tv_WkIdx=0;Tv_WkIdx <E_GrP4MngFontMaxCnt;Tv_WkIdx++ )
		{
			// remove target image
			if ( NULL != m_DvlScptFontTbl[A_TgIdx][Tv_WkIdx] )
			{
				free( m_DvlScptFontTbl[A_TgIdx][Tv_WkIdx] );
				m_DvlScptFontTbl[A_TgIdx][Tv_WkIdx]	=	NULL;
			}
			// check exist source
			if ( NULL != m_DvlScptFontTbl[A_SrIdx][Tv_WkIdx] )
			{
				// copy
				Tv_Size	=	GrP4FontCalcSize( m_DvlScptFontTbl[A_SrIdx][Tv_WkIdx] );
				m_DvlScptFontTbl[A_TgIdx][Tv_WkIdx]	=	(Ptr_GrP4FontHead)malloc( Tv_Size );
				if ( NULL != m_DvlScptFontTbl[A_TgIdx][Tv_WkIdx] )
				{
					GrDumyCopyMem( m_DvlScptFontTbl[A_TgIdx][Tv_WkIdx], m_DvlScptFontTbl[A_SrIdx][Tv_WkIdx], Tv_Size );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP4DvlMng::DvlSkinCopy - font copy error!\n" );
				}
			}
		}

	}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcFontUpdt( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_SkinIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP4MngFontMaxCnt;Tv_WkIdx++ )
		{
			// release font
			if ( NULL != m_FontMng[Tv_WkIdx] )
			{
				delete	m_FontMng[Tv_WkIdx];
				m_FontMng[Tv_WkIdx]	=	NULL;
			}
			// check exist font
			if ( E_GrP4MngFontMaxCnt > Tv_WkIdx )
			{
				Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
				if ( NULL != m_DvlScptFontTbl[Tv_SkinIdx][Tv_WkIdx] )
				{
					m_FontMng[Tv_WkIdx]	=	(Cls_GrP4Font*)new Cls_GrP4Font( (Ptr_GrP4FontHead)m_DvlScptFontTbl[Tv_SkinIdx][Tv_WkIdx] );
				}
			}
		}
}
//--------------------------------------------------------------------
Ptr_GrP4PrjCfg	Cls_GrP4DvlMng::DvlConfigPtrGet( void )
{
		return	&m_DvlPrjConfig;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlImgThumbFccSet(__u32 A_Fcc)
{
	// local -------------------
	// code --------------------
		m_DvlImgThumbFcc	=	A_Fcc;

		// rebuild image cache
		LcDvlImgTmbAllFree();
}
//--------------------------------------------------------------------
__u32	Cls_GrP4DvlMng::DvlImgThumbFccGet(void)
{
		return	m_DvlImgThumbFcc;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlImgTmbAllFree(void)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx = 0; Tv_WkIdx < E_GrP4DvlImgThumbCchCnt; Tv_WkIdx++)
		{
			if ( NULL != m_DvlImgTmbCch[Tv_WkIdx].Gdib )
			{
				LcDvlImgTmbFreeByCch(Tv_WkIdx);
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlImgTmbFreeByCch(__u32 A_CchIdx)
{
	// local -------------------
	// code --------------------
		if (E_GrP4DvlImgThumbCchCnt > A_CchIdx)
		{
			delete	m_DvlImgTmbCch[A_CchIdx].Gdib;
			m_DvlImgTmbCch[A_CchIdx].Gdib		=	NULL;
			m_DvlImgTmbCch[A_CchIdx].ImgId	=	E_GrP4ImgIdNone;
			m_DvlImgTmbCch[A_CchIdx].Tick		=	0;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlImgTmbFreeById(__u16 A_ImgId)
{
	// local -------------------
		__s32	Tv_CchIdx;
	// code --------------------
		Tv_CchIdx	=	LcDvlImgTmbFindId(A_ImgId);
		if ( 0 <= Tv_CchIdx )
		{
			LcDvlImgTmbFreeByCch((__u32)Tv_CchIdx);
		}

}
//--------------------------------------------------------------------
__s32	Cls_GrP4DvlMng::LcDvlImgTmbFindId(__u16 A_ImgId)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for (Tv_WkIdx = 0; Tv_WkIdx < E_GrP4DvlImgThumbCchCnt; Tv_WkIdx++)
		{
			if ((NULL != m_DvlImgTmbCch[Tv_WkIdx].Gdib) && (A_ImgId == m_DvlImgTmbCch[Tv_WkIdx].ImgId))
			{
				Tv_Result	=	Tv_WkIdx;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrP4DvlMng::LcDvlImgTmbFindEmpty(void)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for (Tv_WkIdx = 0; Tv_WkIdx < E_GrP4DvlImgThumbCchCnt; Tv_WkIdx++)
		{
			if (NULL == m_DvlImgTmbCch[Tv_WkIdx].Gdib)
			{
				Tv_Result	=	Tv_WkIdx;
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	Cls_GrP4DvlMng::LcDvlImgTmbFindOldest(void)
{
	// local -------------------
		__s32	Tv_Result;
		__s32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	-1;

		for (Tv_WkIdx = 0; Tv_WkIdx < E_GrP4DvlImgThumbCchCnt; Tv_WkIdx++)
		{
			if (NULL != m_DvlImgTmbCch[Tv_WkIdx].Gdib)
			{
				if ( -1 == Tv_Result )
				{
					// set first
					Tv_Result	=	Tv_WkIdx;
				}
				else
				{
					// check tick
					if (m_DvlImgTmbCch[Tv_Result].Tick > m_DvlImgTmbCch[Tv_WkIdx].Tick)
					{
						Tv_Result	=	Tv_WkIdx;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrP4DvlImgTmbCch	Cls_GrP4DvlMng::DvlImgThumbGet(__u16 A_ImgId)
{
	// local -------------------
		Ptr_GrP4DvlImgTmbCch	Tv_Result;
		__s32		Tv_CchIdx;
		Ptr_GrGdib	Tv_PtrGdib;
		Cls_GrGdib*	Tv_GdibOrg;
		__u32		Tv_PxFmt;
		BOOL8		Tv_IsCvt;
	// code --------------------
		Tv_Result	=	NULL;

		// check exist thumb
		Tv_CchIdx	=	LcDvlImgTmbFindId(A_ImgId);
		if (0 > Tv_CchIdx)
		{
			// not exist
			// check exist id
			Tv_PtrGdib	=	ImgGet(A_ImgId);
			if ( NULL != Tv_PtrGdib )
			{
				// check empty slot
				Tv_CchIdx	=	LcDvlImgTmbFindEmpty();
				if (0 > Tv_CchIdx)
				{
					// delete oldest thumb
					Tv_CchIdx	=	LcDvlImgTmbFindOldest();
					if (0 <= Tv_CchIdx)
					{
						LcDvlImgTmbFreeByCch((__u32)Tv_CchIdx);
					}
				}
				// make thumbnail image
				Tv_GdibOrg	=	GrGdibOvlp(Tv_PtrGdib);
				if ( NULL != Tv_GdibOrg )
				{
					Tv_IsCvt		=	FALSE;

					// check need convert
					Tv_PxFmt	=	Tv_GdibOrg->GetFcc();
					if (Tv_GdibOrg->IsPalleteUse())
					{
						// convert 
						m_DvlImgTmbCch[Tv_CchIdx].Gdib	=	GrGdibCvt(Tv_GdibOrg, m_DvlImgThumbFcc);
						Tv_IsCvt	=	TRUE;
					}
					else
					{
						// check rgb format
						if (GrDrawIsFccTypeYUV(Tv_PxFmt))
						{
							// convert
							m_DvlImgTmbCch[Tv_CchIdx].Gdib	=	GrGdibCvt(Tv_GdibOrg, m_DvlImgThumbFcc);
							Tv_IsCvt	=	TRUE;
						}
						else
						{
							// direct
							m_DvlImgTmbCch[Tv_CchIdx].Gdib	=	Tv_GdibOrg;
						}
					}
					// check convert failure
					if (Tv_IsCvt && (NULL == m_DvlImgTmbCch[Tv_CchIdx].Gdib))
					{
						DbgMsgPrint("Cls_GrP4DvlMng::DvlImgThumbGet - convert gdib create failure.\n");
					}

					if (NULL != m_DvlImgTmbCch[Tv_CchIdx].Gdib)
					{
						GrDumyZeroMem(&m_DvlImgTmbCch[Tv_CchIdx].Bmhd, sizeof(BITMAPV4HEADER));
						// update bitmap info header
						m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4Size			=	sizeof(BITMAPV4HEADER);
						m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4Width			=	(LONG)m_DvlImgTmbCch[Tv_CchIdx].Gdib->GetResX();
						m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4Height		=	-(LONG)m_DvlImgTmbCch[Tv_CchIdx].Gdib->GetResY();
						m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4Planes		=	1;
						m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4BitCount	=	(__u16)m_DvlImgTmbCch[Tv_CchIdx].Gdib->GetPixelBit();
						if (15 == m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4BitCount)
						{
							m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4BitCount ++;		// patch 15 bit 
						}
						Tv_PxFmt	=	m_DvlImgTmbCch[Tv_CchIdx].Gdib->GetFcc();
						if (E_GrFccARGB4444 == Tv_PxFmt)
						{
							m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4V4Compression	=	BI_BITFIELDS;
							m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4RedMask				=	0x0F00;
							m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4GreenMask			=	0x00F0;
							m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4BlueMask			=	0x000F;
						}
						else
						{
							m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4V4Compression	=	BI_RGB;
							m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4RedMask				=	0;
							m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4GreenMask			=	0;
							m_DvlImgTmbCch[Tv_CchIdx].Bmhd.bV4BlueMask			=	0;
						}

					}	//	if (NULL != m_DvlImgTmbCch[Tv_CchIdx].Gdib)
					else
					{
						Tv_CchIdx	=	-1;
					}

					// check convert then release
					if (Tv_IsCvt && (NULL != Tv_GdibOrg))
					{
						// release orignal
						delete	Tv_GdibOrg;
					}

				}	//	if ( NULL != Tv_GdibOrg )

			}	//	if ( NULL != Tv_PtrGdib )
		}	//	if (0 > Tv_CchIdx)

		// update result
		if (0 <= Tv_CchIdx)
		{
			m_DvlImgTmbCch[Tv_CchIdx].Tick	=	m_DvlTickId;		// update tick id
			Tv_Result	=	&m_DvlImgTmbCch[Tv_CchIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlDbgVmRposGet(__u8 A_VmIdx, __u16* A_PtrRtCs, __u16* A_PtrRtIp)
{
	// local -------------------
	// code --------------------
		// init
		*A_PtrRtCs	=	E_GrP4AsmIdNone;
		*A_PtrRtIp	=	0;

		if (E_GrP4MngVmCnt > A_VmIdx)
		{
			*A_PtrRtCs	=	m_VmTbl[A_VmIdx].Cs;
			*A_PtrRtIp	=	m_VmTbl[A_VmIdx].Ip;
		}

}
//--------------------------------------------------------------------
Ptr_GrP4AsmVm	Cls_GrP4DvlMng::DvlVmPtrGet(__u8 A_VmIdx)
{
	// local -------------------
		Ptr_GrP4AsmVm	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if (E_GrP4MngVmCnt > A_VmIdx)
		{
			Tv_Result	=	&m_VmTbl[A_VmIdx];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlPlayTimeSet(Def_GrTime A_Time)
{
		m_DvlPlayTime	=	A_Time;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlPlayDo(BOOL8 A_IsDo)
{
	// local -------------------
	// code --------------------
		m_IsDvlPlayDo		=	A_IsDo;
		m_Gvs.PlayStat	=	E_GrDvrPlayStatStop;
		if (A_IsDo)
		{
			m_Gvs.PlayStat	=	E_GrDvrPlayStatPlay;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlPlayDirSet(BOOL8 A_IsBack)
{
	// local -------------------
	// code --------------------
		m_Gvs.IsPlayBack	=	A_IsBack;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::DvlPlaySpeedSet(__s32 A_Speed)
{
		m_Gvs.PlaySpd		=	A_Speed;
		m_DvlPlayPrgs		=	0;
}
//--------------------------------------------------------------------
void Cls_GrP4DvlMng::DvlPlayTimeGet(Def_GrTime* A_PtrRtSec, BOOL8* A_PtrRtIsSm)
{
		*A_PtrRtSec		=	m_DvlPlayTime;
		*A_PtrRtIsSm	=	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrP4DvlMng::LcDvlPlayUpdt(void)
{
	// local -------------------
		__u32	Tv_SecCnt;
	// code --------------------
		if (m_IsDvlPlayDo)
		{
			Tv_SecCnt	=	0;
			if (0 <= m_Gvs.PlaySpd)
			{
				m_DvlPlayPrgs		=	m_DvlPlayPrgs + (1 << m_Gvs.PlaySpd);
				Tv_SecCnt				=	m_DvlPlayPrgs / 10;
				m_DvlPlayPrgs		=	m_DvlPlayPrgs % 10;
			}
			else
			{
				m_DvlPlayPrgs ++;
				if ((__s32)m_DvlPlayPrgs >= (1 << (-m_Gvs.PlaySpd)))
				{
					Tv_SecCnt	++;
					m_DvlPlayPrgs	=	0;
				}
			}
			
			if (m_Gvs.IsPlayBack)
			{
				m_DvlPlayTime	=	m_DvlPlayTime - Tv_SecCnt;
			}
			else
			{
				m_DvlPlayTime	=	m_DvlPlayTime + Tv_SecCnt;
			}
		}
}
//--------------------------------------------------------------------
