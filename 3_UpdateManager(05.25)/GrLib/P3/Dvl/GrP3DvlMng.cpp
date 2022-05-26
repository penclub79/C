/*
		platform develop manager

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrDebug.h>
#include	<P3/Dvl/GrP3DvlMng.h>
#include	<GrDumyTool.h>
#include	<P3/GrP3DoLbl.h>
#include	<GrStrTool.h>
#include	<GrDrawTool.h>
#include	<GrFileTool.h>
#include	<GrFileCtrl.h>
#include	<P3/GrP3DoPrgsBarLoc.h>
#include	<P3/GrP3DoQuad.h>
#include <P3/GrP3ApiBase.h>
#include <P3/GrP3Bios.h>
#include <P3/GrP3Cfg.h>

//====================================================================
//local const

#define E_GrP3DvlMngRatbTime			30

//====================================================================
//local type

typedef	struct	St_GrP3DvlTxtNameItm
{
	__u16	Id;
	WCHAR*	StrName;
}	*Ptr_GrP3DvlTxtNameItm;

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrP3Mng*	V_GrP3Mng;

extern	WCHAR*	V_GrP3DvlClassNameTbl[E_GrP3DoClassCnt];

extern	St_GrP3PrjStpInfo	V_GrP3DvlStpSysTbl[E_GrP3DvlSetupSysCnt];

St_GrP3DvlTxtNameItm	V_GrP3DvlTxtSysName[E_GrP3DvlTextSysCnt]	=
{
	{	E_GrP3ScptTxtIdLang0,	L"(1) Language 0"	},
	{	E_GrP3ScptTxtIdLang1,	L"(2) Language 1"	},
	{	E_GrP3ScptTxtIdLang2,	L"(3) Language 2"	},
	{	E_GrP3ScptTxtIdLang3,	L"(4) Language 3"	},
	{	E_GrP3ScptTxtIdLang4,	L"(5) Language 4"	},
	{	E_GrP3ScptTxtIdLang5,	L"(6) Language 5"	},
	{	E_GrP3ScptTxtIdLang6,	L"(7) Language 6"	},
	{	E_GrP3ScptTxtIdLang7,	L"(8) Language 7"	},
	{	E_GrP3ScptTxtIdLang8,	L"(9) Language 8"	},
	{	E_GrP3ScptTxtIdLang9,	L"(10) Language 9"	},
	{	E_GrP3ScptTxtIdLang10,	L"(11) Language 10"	},
	{	E_GrP3ScptTxtIdLang11,	L"(12) Language 11"	},
	{	E_GrP3ScptTxtIdLang12,	L"(13) Language 12"	},
	{	E_GrP3ScptTxtIdLang13,	L"(14) Language 13"	},
	{	E_GrP3ScptTxtIdLang14,	L"(15) Language 14"	},
	{	E_GrP3ScptTxtIdLang15,	L"(16) Language 15"	},
	{	E_GrP3ScptTxtIdMonth1,	L"(17) Month 1"	},
	{	E_GrP3ScptTxtIdMonth2,	L"(18) Month 2"	},
	{	E_GrP3ScptTxtIdMonth3,	L"(19) Month 3"	},
	{	E_GrP3ScptTxtIdMonth4,	L"(20) Month 4"	},
	{	E_GrP3ScptTxtIdMonth5,	L"(21) Month 5"	},
	{	E_GrP3ScptTxtIdMonth6,	L"(22) Month 6"	},
	{	E_GrP3ScptTxtIdMonth7,	L"(23) Month 7"	},
	{	E_GrP3ScptTxtIdMonth8,	L"(24) Month 8"	},
	{	E_GrP3ScptTxtIdMonth9,	L"(25) Month 9"	},
	{	E_GrP3ScptTxtIdMonth10,	L"(26) Month 10"	},
	{	E_GrP3ScptTxtIdMonth11,	L"(27) Month 11"	},
	{	E_GrP3ScptTxtIdMonth12,	L"(28) Month 12"	},
	{	E_GrP3ScptTxtIdWeekSun,	L"(29) Sunday"	},
	{	E_GrP3ScptTxtIdWeekMon,	L"(30) Monday"	},
	{	E_GrP3ScptTxtIdWeekTue,	L"(31) Tuesday"	},
	{	E_GrP3ScptTxtIdWeekWed,	L"(32) Wednesday"	},
	{	E_GrP3ScptTxtIdWeekThu,	L"(33) Thursday"	},
	{	E_GrP3ScptTxtIdWeekFri,	L"(34) Friday"	},
	{	E_GrP3ScptTxtIdWeekSat,	L"(35) Saturday"	},
	{	E_GrP3ScptTxtIdUnitYear,	L"(36) Unit - Year"	},
	{	E_GrP3ScptTxtIdUnitMonth,	L"(37) Unit - Month"	},
	{	E_GrP3ScptTxtIdUnitDay,	L"(38) Unit - Day"	},
	{	E_GrP3ScptTxtIdUnitHour,	L"(39) Unit - Hour"	},
	{	E_GrP3ScptTxtIdUnitMin,	L"(40) Unit - Minute"	},
	{	E_GrP3ScptTxtIdUnitSec,	L"(41) Unit - Sec"	},
	{	E_GrP3ScptTxtIdTimeAm,	L"(42) Time - AM"	},
	{	E_GrP3ScptTxtIdTimePm,	L"(43) Time - PM"	},
	{	E_GrP3ScptTxtIdChannel,	L"(51) Channel"	},
	{	E_GrP3ScptTxtIdDetected,	L"(52) Detected"	},
	{	E_GrP3ScptTxtIdChanged,	L"(53) Changed"	},
	{	E_GrP3ScptTxtIdStart,	L"(54) Start"	},
	{	E_GrP3ScptTxtIdEnd,	L"(55) End"	},
	{	E_GrP3ScptTxtIdResolution,	L"(56) Resolution"	},
	{	E_GrP3ScptTxtIdRecord,	L"(57) Record"	},
	{	E_GrP3ScptTxtIdRecQuility,	L"(58) Quality"	},
	{	E_GrP3ScptTxtIdRecSpeed,	L"(59) Record Speed"	},
	{	E_GrP3ScptTxtIdRecPost,	L"(60) POST-FRAME"	},
	{	E_GrP3ScptTxtIdRecPre,		L"(61 PRE-FRAME"	},
	{	E_GrP3ScptTxtIdOn,		L"(62) On"	},
	{	E_GrP3ScptTxtIdOff,		L"(63) Off"	},
	{	E_GrP3ScptTxtIdMotionMap,		L"(64) Motion Area"	},
	{	E_GrP3ScptTxtIdRecSchd,		L"(65) Record schedule"	},
	{	E_GrP3ScptTxtIdPassword,		L"(66) Password"	},
	{	E_GrP3ScptTxtIdCamera,		L"(67) Camera"	},
	{	E_GrP3ScptTxtIdTitle,		L"(68) Title"	},
	{	E_GrP3ScptTxtIdNetwork,		L"(69) Network"	},
	{	E_GrP3ScptTxtIdDisconnect,		L"(70) Disconnect"	},
	{	E_GrP3ScptTxtIdConnect,		L"(71) Connect"	},
	{	E_GrP3ScptTxtIdSearch,		L"(72) Search"	},
	{	E_GrP3ScptTxtIdBackup,		L"(73) Backup"	},
	{	E_GrP3ScptTxtIdMotion,		L"(74) Motion"	},
	{	E_GrP3ScptTxtIdSensor,		L"(75) Sensor"	},
	{	E_GrP3ScptTxtIdEvtExt1,		L"(76) Event Extension 1"	},
	{	E_GrP3ScptTxtIdEvtExt2,		L"(77) Event Extension 2"	},
	{	E_GrP3ScptTxtIdEvtExt3,		L"(78) Event Extension 3"	},
	{	E_GrP3ScptTxtIdEvtExt4,		L"(79) Event Extension 4"	},
	{	E_GrP3ScptTxtIdEvtExt5,		L"(80) Event Extension 5"	},
	{	E_GrP3ScptTxtIdVdoLoss,		L"(81) Video loss"	},
	{	E_GrP3ScptTxtIdVdoRcvy,		L"(82) Video Recovery"	},
	{	E_GrP3ScptTxtIdPowerOn,		L"(83) Power on"	},
	{	E_GrP3ScptTxtIdPowerOff,		L"(84) Power off"	},
	{	E_GrP3ScptTxtIdLogin,		L"(85) Login"	},
	{	E_GrP3ScptTxtIdLogout,		L"(86) Logout"	},
	{	E_GrP3ScptTxtIdSetupChg,		L"(87) Setup changed"	},
	{	E_GrP3ScptTxtIdHddErr,		L"(88) HDD error"	},
	{	E_GrP3ScptTxtIdSetupIn,		L"(89) Setup menu in"	},
	{	E_GrP3ScptTxtIdSetupOut,		L"(90) Setup menu out"	},
	{	E_GrP3ScptTxtIdHddFormat,		L"(91) HDD formatted"	},
	{	E_GrP3ScptTxtIdRamup,		L"(92) RAMUP"	},
	{	E_GrP3ScptTxtIdEmergency,		L"(93) Emergency"	},
	{	E_GrP3ScptTxtIdMacaddress,		L"(94) Mac address"	},
	{	E_GrP3ScptTxtIdIp,		L"(95) IP"	},
	{	E_GrP3ScptTxtIdPort,		L"(96) Port"	},
	{	E_GrP3ScptTxtIdUse,		L"(97) Use"	},
	{	E_GrP3ScptTxtIdSensitivity,		L"(98) Sensitivity"	},
	{	E_GrP3ScptTxtIdWebcode,		L"(99) WEB-CODE"	},
	{	E_GrP3ScptTxtIdFwVer,		L"(100) Firmware Version"	},
	{	E_GrP3ScptTxtIdNwVer,		L"(101) Network Version"	},
	{	E_GrP3ScptTxtIdModelNum,	L"(102) Model number"	},

	{	E_GrP3ScptTxtIdFileSystem,	L"(103) file system"	},
	{	E_GrP3ScptTxtIdFirmWare,	L"(104) firmware"	},
	{	E_GrP3ScptTxtIdUpgrade,		L"(105) upgrade"	},
	{	E_GrP3ScptTxtIdBatteryErr,	L"(106) battery error"	},

	{	E_GrP3ScptTxtIdNone,	L""	},

};

St_GrP3DvlDoPrInfoData	V_GrP3DvlMdlDbgClstTbl[E_GrP3DvlMdlDbgClstCnt]	=	
{
	{	L"VdoMaxCh",		TRUE,		E_GrP3DvlParaTypeByte,			1,	(void*)&((Ptr_GrP3PrjDbgInfo)0)->VdoMaxCh,	L"Video Channel count"	},
	{	L"AdoMaxCh",		TRUE,		E_GrP3DvlParaTypeByte,			1,	(void*)&((Ptr_GrP3PrjDbgInfo)0)->AdoMaxCh,	L"Audio Channel Count"	},
	{	L"DiChCnt",			TRUE,		E_GrP3DvlParaTypeByte,			1,	(void*)&((Ptr_GrP3PrjDbgInfo)0)->DiChCnt,		L"Sensor count"	},
	{	L"DoChCnt",			TRUE,		E_GrP3DvlParaTypeByte,			1,	(void*)&((Ptr_GrP3PrjDbgInfo)0)->DoChCnt,		L"Alarm count"	},
	{	L"HwVer",				TRUE,		E_GrP3DvlParaTypeDword,			1,	(void*)&((Ptr_GrP3PrjDbgInfo)0)->HwVer,			L"Hardware Version"	},
	{	L"NetVer",			TRUE,		E_GrP3DvlParaTypeDword,			1,	(void*)&((Ptr_GrP3PrjDbgInfo)0)->NetVer,		L"Network Version"	},
	{	L"DipSw",				TRUE,		E_GrP3DvlParaTypeBitmap0,		1,	(void*)&((Ptr_GrP3PrjDbgInfo)0)->DipSw,			L"Dip switch"	},
};

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DvlMng::Cls_GrP3DvlMng( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrP3Mng( A_TaskSvr )
{
	// local -------------------
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
		GrDumyZeroMem( m_DvlSruTbl, sizeof(m_DvlSruTbl) );
		//GrDumyZeroMem( m_DvlStpSysTbl, sizeof(m_DvlStpSysTbl) );
		GrDumyZeroMem( m_DvlGvuTbl, sizeof(m_DvlGvuTbl) );
		GrDumyZeroMem( m_DvlScptAsmTbl, sizeof(m_DvlScptAsmTbl) );
		GrDumyZeroMem( m_DvlAsmInfo, sizeof(m_DvlAsmInfo) );
		GrDumyZeroMem( m_DvlAsmCmt, sizeof(m_DvlAsmCmt) );
		GrDumyZeroMem( &m_DvlSetBase, sizeof(m_DvlSetBase) );
		GrDumyZeroMem( m_DvlMdlDblTbl, sizeof(m_DvlMdlDblTbl) );

		GrDumyZeroMem( m_DvlBldInfo, sizeof(m_DvlBldInfo) );
		GrDumyZeroMem( m_DvlEdtIdDispTbl, sizeof(m_DvlEdtIdDispTbl) );

		m_DvlIsPrjExist	=	FALSE;

		m_DvlEdtIdDispCnt	=	0;

		m_DvlStpUsrCnt	=	0;
		m_DvlSruCnt			=	0;

		m_DvlEdtLump	=	NULL;

		m_DvlFc				=	NULL;

		m_DvlPtrStpFl	=	NULL;

		m_DvlIsDbgRun	=	FALSE;
		m_DvlDbgMode	=	E_GrP3DvlDbgModeRun;

		m_DvlRatbTimer	=	0;
		m_DvlLanCblTimer	=	0;
		m_DvlUsbTimer		=	0;
		m_DvlCmsTimer		=	0;

		m_IsDvlBkupErr	=	FALSE;
		m_IsDvlBkupRun	=	FALSE;

		m_IsDvlLogBkupErr	=	FALSE;
		m_IsDvlLogBkupRun	=	FALSE;

		V_GrP3Mng	=	this;			// update normal manager

		m_PtzMng				=	(Cls_GrPtzV2Mng*) new Cls_GrPtzV2Mng( A_TaskSvr, 16 );
		m_SioMng				=	(Cls_GrP3SioMng*) new Cls_GrP3SioMng( A_TaskSvr );

}
//--------------------------------------------------------------------
Cls_GrP3DvlMng::~Cls_GrP3DvlMng( void )
{
	// local -------------------
	// code --------------------
		DvlPrjClose();		// release project

		GrTaskSvrMsgCmdFree( m_DvlMsgAloc );

		delete	m_PtzMng;
		delete	m_SioMng;

}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjRuleBuild( void )
{
	// local -------------------
		__u8	Tv_SkinIdx;
	// code --------------------
		// init
		Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;

		// script initialize
		m_PtrCfgPrj	=	&m_DvlScptCfgPrj;
		m_PtrCfgRtl	=	&m_DvlScptCfgRtl;
		m_PtrCfgMdl	=	&m_DvlPrjMdlAtb[0];
		m_IsScptAble	=	TRUE;

		// window
		m_PtrWinTbl	=	m_DvlScptWinTbl;		// window table update
		m_WinCnt	=	E_GrP3DvlWinMaxCnt;

		// page
		m_PtrPageTbl	=	m_DvlScptPageTbl;		// page table update
		m_PageCnt	=	E_GrP3DvlPageMaxCnt;

		// paint script
		m_PtrPstTbl	=	m_DvlScptPstTbl[Tv_SkinIdx];
		m_PstCnt	=	E_GrP3DvlPstMaxCnt;

		// folder 

		// image
		m_PtrImgTbl	=	m_DvlScptImgTbl[Tv_SkinIdx];

		// text
		m_PtrTxtTbl	=	m_DvlScptTxtTbl;

		// font

		// assembly
		m_PtrAsmTbl	=	m_DvlScptAsmTbl;
		m_AsmCnt	=	E_GrP3DvlAsmMaxCnt;

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlPrjNew( __u32 A_FccPx, __u32 A_Res1X, __u32 A_Res1Y, __u32 A_Res2X, __u32 A_Res2Y, 
	BOOL8 A_IsDual, BOOL8 A_IsInputDual )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3DvlMsgPrjNew	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdPrjNew;
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
BOOL8	Cls_GrP3DvlMng::LcDvlPrjNew( __u32 A_FccPx, __u32 A_Res1X, __u32 A_Res1Y, __u32 A_Res2X, __u32 A_Res2Y, 
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
void	Cls_GrP3DvlMng::LcDispSetup( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3MngConMaxCnt;Tv_WkIdx++)
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
__u32	Cls_GrP3DvlMng::DvlConResX( __u8 A_ConIdx )
{
		return	m_DvlPrjConfig.ConCfg[A_ConIdx].ResX;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DvlMng::DvlConResY( __u8 A_ConIdx )
{
		return	m_DvlPrjConfig.ConCfg[A_ConIdx].ResY;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcFindEmptyWinId( void )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlWinMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL == m_DvlScptWinTbl[Tv_WkIdx].Pos )
			{
				// found
				Tv_Result	=	Tv_WkIdx + E_GrP3ScptIdWinBase;
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcFindEmptyPageId( void )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlPageMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL == m_DvlScptPageTbl[Tv_WkIdx].Pos )
			{
				// found
				Tv_Result	=	Tv_WkIdx + E_GrP3ScptIdPageBase;
				break;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlWindowNew( void )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP3DvlMsgWindowNew	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdWindowNew;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcDvlWindowNew( void )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_ScptId;
		__u16	Tv_SlotIdx;
		Ptr_GrP3ScptObjHd	Tv_PtrHead;
		Ptr_GrP3ScptObjEnd	Tv_PtrTail;
		__u32	Tv_Size;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		if ( DvlIsPrjExist() )
		{
			Tv_ScptId	=	LcFindEmptyWinId();
			if ( E_GrP3ScptIdNone != Tv_ScptId )
			{
				Tv_SlotIdx	=	Tv_ScptId - E_GrP3ScptIdWinBase;
				// create buffer
				m_DvlWinInfo[Tv_SlotIdx].Size	=	0;
				m_DvlScptWinTbl[Tv_SlotIdx].Pos	=	(void*)malloc( E_GrP3DvlScptBufMax );
				if ( NULL != m_DvlScptWinTbl[Tv_SlotIdx].Pos )
				{
					// create default window script
					// add header
					Tv_PtrHead	=	(Ptr_GrP3ScptObjHd)m_DvlScptWinTbl[Tv_SlotIdx].Pos;
					Tv_PtrHead->Stx				=	E_GrP3ScptObjStx;
					Tv_PtrHead->ClassId		=	E_GrP3DoClassWin;
					Tv_PtrHead->ScptId		=	Tv_ScptId;
					Tv_PtrHead->ParaSize	=	0;
					Tv_PtrHead->Rect.left			=	E_GrP3DvlDfltWinLeft;
					Tv_PtrHead->Rect.top			=	E_GrP3DvlDfltWinTop;
					Tv_PtrHead->Rect.right		=	E_GrP3DvlDfltWinRight;
					Tv_PtrHead->Rect.bottom		=	E_GrP3DvlDfltWinBottom;
					Tv_Size		=	sizeof(St_GrP3ScptObjHd);
					// add tail
					Tv_PtrTail	=	(Ptr_GrP3ScptObjEnd)( (__u32)m_DvlScptWinTbl[Tv_SlotIdx].Pos + Tv_Size );
					Tv_PtrTail->Edx			=	E_GrP3ScptObjEdx;
					Tv_PtrTail->ScptId	=	Tv_ScptId;
					Tv_Size		=	Tv_Size + sizeof(St_GrP3ScptObjEnd);
					// register
					m_DvlWinInfo[Tv_SlotIdx].Size			=	Tv_Size;
					m_DvlWinInfo[Tv_SlotIdx].FoldId		=	E_GrP3DvlFolderIdRoot;
					GrStrIntToWstr( m_DvlWinInfo[Tv_SlotIdx].StrName, (__s32)Tv_ScptId );
					m_DvlScptWinTbl[Tv_SlotIdx].Layer	=	E_GrP3DvlDfltLayer;
					// success
					Tv_Result	=	Tv_ScptId;
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP3DvlMng::LcDvlWindowNew - project is not exist.\n");
		}

		return	Tv_Result;		
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlPageNew( void )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP3DvlMsgPageNew	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdPageNew;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcDvlPageNew( void )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_ScptId;
		__u16	Tv_SlotIdx;
		Ptr_GrP3ScptObjHd	Tv_PtrHead;
		Ptr_GrP3ScptObjEnd	Tv_PtrTail;
		__u32	Tv_Size;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;
		if ( DvlIsPrjExist() )
		{
			Tv_ScptId	=	LcFindEmptyPageId();
			if ( E_GrP3ScptIdNone != Tv_ScptId )
			{
				Tv_SlotIdx	=	Tv_ScptId - E_GrP3ScptIdPageBase;
				// create buffer
				m_DvlPageInfo[Tv_SlotIdx].Size	=	0;
				m_DvlScptPageTbl[Tv_SlotIdx].Pos	=	(void*)malloc( E_GrP3DvlScptBufMax );
				if ( NULL != m_DvlScptPageTbl[Tv_SlotIdx].Pos )
				{
					// create default page script
					// add header
					Tv_PtrHead	=	(Ptr_GrP3ScptObjHd)m_DvlScptPageTbl[Tv_SlotIdx].Pos;
					Tv_PtrHead->Stx				=	E_GrP3ScptObjStx;
					Tv_PtrHead->ClassId		=	E_GrP3DoClassWin;
					Tv_PtrHead->ScptId		=	Tv_ScptId;
					Tv_PtrHead->ParaSize	=	0;
					Tv_PtrHead->Rect.left			=	0;
					Tv_PtrHead->Rect.top			=	0;
					Tv_PtrHead->Rect.right		=	1920;
					Tv_PtrHead->Rect.bottom		=	1080;
					Tv_Size		=	sizeof(St_GrP3ScptObjHd);
					// add tail
					Tv_PtrTail	=	(Ptr_GrP3ScptObjEnd)( (__u32)m_DvlScptPageTbl[Tv_SlotIdx].Pos + Tv_Size );
					Tv_PtrTail->Edx			=	E_GrP3ScptObjEdx;
					Tv_PtrTail->ScptId	=	Tv_ScptId;
					Tv_Size		=	Tv_Size + sizeof(St_GrP3ScptObjEnd);
					// regist
					m_DvlPageInfo[Tv_SlotIdx].Size			=	Tv_Size;
					m_DvlPageInfo[Tv_SlotIdx].FoldId		=	E_GrP3DvlFolderIdRoot;
					GrStrIntToWstr( m_DvlPageInfo[Tv_SlotIdx].StrName, (__s32)Tv_ScptId );
					m_DvlScptPageTbl[Tv_SlotIdx].AsmOpen	=	E_GrP3AsmIdNone;
					m_DvlScptPageTbl[Tv_SlotIdx].AsmClose	=	E_GrP3AsmIdNone;
					// success
					Tv_Result	=	Tv_ScptId;
				}
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP3DvlMng::DvlPageNew - project is not exist.\n");
		}

		return	Tv_Result;		
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcEditObjFree( void )
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP3MngConMaxCnt;Tv_ConIdx++ )
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
BOOL8	Cls_GrP3DvlMng::LcEditWindow( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_SlotIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		// check exist window
		Tv_SlotIdx	=	A_ScptId - E_GrP3ScptIdWinBase;
		if ( NULL != m_DvlScptWinTbl[Tv_SlotIdx].Pos )
		{
			// decode
			LcUiDecWindow( A_ScptId, 0 );
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
BOOL8	Cls_GrP3DvlMng::LcEditPage( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_SlotIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		// check exist page
		Tv_SlotIdx	=	A_ScptId - E_GrP3ScptIdPageBase;
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
BOOL8	Cls_GrP3DvlMng::DvlEditLumpSet( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3DvlMsgEditLumpSet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditLumpSet;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.ScptId				=	A_ScptId;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcDvlEditLumpSet( __u16 A_ScptId )
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
			if ( (E_GrP3ScptIdWinBase <= A_ScptId) && ((E_GrP3DvlWinMaxCnt + E_GrP3ScptIdWinBase) > A_ScptId) )
			{
				// window
				LcEditWindow( A_ScptId );
			}
			else if ( (E_GrP3ScptIdPageBase <= A_ScptId) && ((E_GrP3DvlPageMaxCnt + E_GrP3ScptIdPageBase) > A_ScptId) )
			{
				// page
				LcEditPage( A_ScptId );
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlEditLumpSet - bad script id ( %d ).\n", A_ScptId );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlEditLumpSet - project is not exist.\n" );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP3DvlMng::WindowLayerGet( __u16 A_ScptId )
{
	// local -------------------
		__u8	Tv_Result;
		__u16	Tv_SlotIdx;
	// code --------------------
		Tv_Result	=	0;
		// check script id
		if ( (E_GrP3ScptIdWinBase <= A_ScptId) && ((E_GrP3DvlWinMaxCnt + E_GrP3ScptIdWinBase) > A_ScptId) )
		{
			Tv_SlotIdx	=	A_ScptId - E_GrP3ScptIdWinBase;
			// check exist window
			if ( NULL != m_DvlScptWinTbl[Tv_SlotIdx].Pos )
			{
				Tv_Result	=	m_DvlScptWinTbl[Tv_SlotIdx].Layer;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::WindowLayerSet( __u16 A_ScptId, __u8 A_Layer )
{
	// local -------------------
		__u16	Tv_SlotIdx;
	// code --------------------
		// check script id
		if ( (E_GrP3ScptIdWinBase <= A_ScptId) && ((E_GrP3DvlWinMaxCnt + E_GrP3ScptIdWinBase) > A_ScptId) )
		{
			Tv_SlotIdx	=	A_ScptId - E_GrP3ScptIdWinBase;
			// check exist window
			if ( NULL != m_DvlScptWinTbl[Tv_SlotIdx].Pos )
			{
				// check layer range
				if ( E_GrP3ConLayerMaxCnt > A_Layer )
				{
					m_DvlScptWinTbl[Tv_SlotIdx].Layer	=	A_Layer;
				}
			}
		}
}
//--------------------------------------------------------------------
Ptr_GrP3ScptWinInfo	Cls_GrP3DvlMng::DvlWinScptInfoPtrGet( __u16 A_ScptId )
{
	// local -------------------
		Ptr_GrP3ScptWinInfo	Tv_Result;
		__u16	Tv_SlotIdx;
	// code --------------------
		Tv_Result	=	NULL;
		// check script id
		if ( (E_GrP3ScptIdWinBase <= A_ScptId) && ((E_GrP3DvlWinMaxCnt + E_GrP3ScptIdWinBase) > A_ScptId) )
		{
			Tv_SlotIdx	=	A_ScptId - E_GrP3ScptIdWinBase;
			// check exist window
			if ( NULL != m_DvlScptWinTbl[Tv_SlotIdx].Pos )
			{
				Tv_Result	=	&m_DvlScptWinTbl[Tv_SlotIdx];
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrP3ScptPageInfo	Cls_GrP3DvlMng::DvlPageScptInfoPtrGet( __u16 A_ScptId )
{
	// local -------------------
		Ptr_GrP3ScptPageInfo	Tv_Result;
		__u16	Tv_SlotIdx;
	// code --------------------
		Tv_Result	=	NULL;
		// check script id
		if ( (E_GrP3ScptIdPageBase <= A_ScptId) && ((E_GrP3DvlPageMaxCnt + E_GrP3ScptIdPageBase) > A_ScptId) )
		{
			Tv_SlotIdx	=	A_ScptId - E_GrP3ScptIdPageBase;
			// check exist page
			if ( NULL != m_DvlScptPageTbl[Tv_SlotIdx].Pos )
			{
				Tv_Result	=	&m_DvlScptPageTbl[Tv_SlotIdx];
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::RtlVmRun( void )
{
	// local -------------------
		Ptr_GrP3AsmOpBase	Tv_PtrOpBase;
		BOOL8	Tv_IsFgAble;
		BOOL8	Tv_IsDoCmd;
	// code --------------------
		do 
		{
			// check run able
			if ( m_VmTbl[m_VmIdx].IsRun )
			{
				// prepare parse
				Tv_PtrOpBase	=	&m_PtrVmCs[m_VmTbl[m_VmIdx].Ip];
				// check flag
				Tv_IsFgAble	=	TRUE;
				if ( 0 != Tv_PtrOpBase->FlagAble )
				{
					if ( 0 == (m_VmTbl[m_VmIdx].Flag & Tv_PtrOpBase->FlagAble ) )
					{
						Tv_IsFgAble	=	FALSE;
					}
				}
				if ( 0 != (m_VmTbl[m_VmIdx].Flag & Tv_PtrOpBase->FlagBan) )
				{
					Tv_IsFgAble	=	FALSE;
				}
				// run
				Tv_IsDoCmd	=	TRUE;

				switch ( m_DvlDbgMode )
				{
					case E_GrP3DvlDbgModePause:
						Tv_IsDoCmd	=	FALSE;
						break;
					case E_GrP3DvlDbgModeRun:
						// check break point mask
						if ( Tv_IsFgAble && ( 0 != (E_GrP3AsmOpBrkPntMask & Tv_PtrOpBase->Code) ) )
						{
							Tv_IsDoCmd		=	FALSE;
							m_DvlDbgMode	=	E_GrP3DvlDbgModePause;
							GrP3DvlIdeMsgPost( E_GrP3DvlIdeCmdVmBrkPnt, 0 );
						}
						break;
					case E_GrP3DvlDbgModeResume:
						m_DvlDbgMode	=	E_GrP3DvlDbgModeRun;
						break;
					case E_GrP3DvlDbgModeStep:
						m_DvlDbgMode	=	E_GrP3DvlDbgModePause;
						GrP3DvlIdeMsgPost( E_GrP3DvlIdeCmdVmBrkPnt, 0 );
						break;
				}
				
				// do command
				if ( Tv_IsDoCmd )
				{
					LcVmDoOneCmd();
				}
			}
			// check step mode or break point
			while ( E_GrP3DvlDbgModePause == m_DvlDbgMode )
			{
				GrPrcSleep( 10 );
				if ( !m_DvlIsDbgRun )
				{
					m_DvlDbgMode	=	E_GrP3DvlDbgModeRun;
					break;
				}
			}
			// check debug run mode
			if ( !m_DvlIsDbgRun )
			{
				break;
			}
			// check op wait
			if ( m_VmTbl[m_VmIdx].IsOpWait )
			{
				break;
			}
			// check modal wait
			if ( m_VmTbl[m_VmIdx].IsModalWait )
			{
				break;
			}
			// check finish code
			if ( (!m_VmTbl[m_VmIdx].IsRun) && (0 != m_VmIdx) )
			{
				m_VmIdx --;
				LcVmPrepare( NULL );
				if ( m_VmTbl[m_VmIdx].IsModalWait )
				{
					break;
				}
			}
		} while ( m_DvlIsDbgRun && m_VmTbl[m_VmIdx].IsRun );

}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::RtlVmFault( void )
{
	// local -------------------
	// code --------------------
		// pause VM running
		m_DvlDbgMode	=	E_GrP3DvlDbgModePause;
		GrP3DvlIdeMsgPost( E_GrP3DvlIdeCmdVmFault, 0 );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlIsPrjExist( void )
{
	// local -------------------
	// code --------------------
		return	m_DvlIsPrjExist;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlSlaveScptIdAlloc( void )
{
	// local -------------------
		__u16	Tv_Result;
		__u32	Tv_Mask;
		__u16	Tv_DwIdx;
		__u16	Tv_BitIdx;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;
		for ( Tv_DwIdx=0;Tv_DwIdx < E_GrP3DvlSlaveMapCnt;Tv_DwIdx++ )
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
						Tv_Result	=	(Tv_DwIdx << E_GrP3DvlSlaveMapShft) + Tv_BitIdx + E_GrP3ScptIdSlaveBase;
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
void	Cls_GrP3DvlMng::DvlSlaveScptIdFree( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_DwIdx;
		__u16	Tv_BitIdx;
		__u16	Tv_Mask;
	// code --------------------
		// check in range
		if ( (E_GrP3ScptIdSlaveBase <= A_ScptId) && ((E_GrP3ScptIdSlaveBase + E_GrP3DvlSlaveMaxCnt) > A_ScptId) )
		{
			Tv_DwIdx	=	(A_ScptId - E_GrP3ScptIdSlaveBase) >> E_GrP3DvlSlaveMapShft;
			Tv_BitIdx	=	(A_ScptId - E_GrP3ScptIdSlaveBase) & ((1 << E_GrP3DvlSlaveMapShft) - 1);
			Tv_Mask		=	1 << Tv_BitIdx;
			if ( 0 != (Tv_Mask & m_DvlSlvMap[Tv_DwIdx]) )
			{
				// remove
				m_DvlSlvMap[Tv_DwIdx]	=	m_DvlSlvMap[Tv_DwIdx] & (~Tv_Mask);
			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::DvlSlaveScptIdFree - Script id (%d) is already removed.\n", A_ScptId );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP3DvlMng::DvlSlaveScptIdFree - Script id (%d) is not slave id.\n", A_ScptId );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlClassNameGet( __u8 A_ClassId )
{
		return	V_GrP3DvlClassNameTbl[A_ClassId];
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlIsPageWindow( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( (E_GrP3ScptIdPageBase <= A_ScptId) && ((E_GrP3ScptIdPageBase + E_GrP3DvlPageMaxCnt) > A_ScptId) )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlPrjClose( void )
{
	// local -------------------
		//St_GrP3DvlMsgBase	Tv_Msg;
	// code --------------------
		/*
		Tv_Msg.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Cmd				=	E_GrP3DvlMcmdPrjClose;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		*/
		LcDvlPrjClose();
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcDvlPrjClose( void )
{
	// local -------------------
		__s32		Tv_WkIdx;
		__u8	Tv_SkinIdx;
		__u8	Tv_LangIdx;
	// code --------------------
		if ( DvlIsPrjExist() )
		{
			// release window
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlWinMaxCnt;Tv_WkIdx++ )
			{
				if ( NULL != m_DvlScptWinTbl[Tv_WkIdx].Pos )
				{
					free( m_DvlScptWinTbl[Tv_WkIdx].Pos );
					m_DvlScptWinTbl[Tv_WkIdx].Pos	=	NULL;
				}
			}
			GrDumyZeroMem( m_DvlPageInfo, sizeof(m_DvlPageInfo) );		// clear info
			// release page
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlPageMaxCnt;Tv_WkIdx++ )
			{
				if ( NULL != m_DvlScptPageTbl[Tv_WkIdx].Pos )
				{
					free( m_DvlScptPageTbl[Tv_WkIdx].Pos );
					m_DvlScptPageTbl[Tv_WkIdx].Pos	=	NULL;
				}
			}
			GrDumyZeroMem( m_DvlPageInfo, sizeof(m_DvlPageInfo) );		// clear info
			// release paint script
			for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP3DvlSkinMaxCnt;Tv_SkinIdx++ )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlPstMaxCnt;Tv_WkIdx++ )
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
			for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP3DvlSkinMaxCnt;Tv_SkinIdx++ )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3MngImgScptMaxCnt;Tv_WkIdx++ )
				{
					if ( NULL != m_DvlScptImgTbl[Tv_SkinIdx][Tv_WkIdx] )
					{
						free( m_DvlScptImgTbl[Tv_SkinIdx][Tv_WkIdx] );
						m_DvlScptImgTbl[Tv_SkinIdx][Tv_WkIdx]	=	NULL;
					}
				}
			}
			// release text
			for (Tv_WkIdx=0;Tv_WkIdx < E_GrP3MngTextScptMaxCnt;Tv_WkIdx++)
			{
				for ( Tv_LangIdx=0;Tv_LangIdx < E_GrP3ScptLangMaxCnt;Tv_LangIdx++ )
				{
					if ( NULL != m_DvlScptTxtTbl[Tv_WkIdx].LngTbl[Tv_LangIdx] )
					{
						free(m_DvlScptTxtTbl[Tv_WkIdx].LngTbl[Tv_LangIdx]);
						m_DvlScptTxtTbl[Tv_WkIdx].LngTbl[Tv_LangIdx]	=	NULL;
					}
				}
			}
			// release font
			for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP3DvlSkinMaxCnt;Tv_SkinIdx++ )
			{
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3MngFontMaxCnt;Tv_WkIdx++ )
				{
					if ( NULL != m_DvlScptFontTbl[Tv_SkinIdx][Tv_WkIdx] )
					{
						free( m_DvlScptFontTbl[Tv_SkinIdx][Tv_WkIdx] );
						m_DvlScptFontTbl[Tv_SkinIdx][Tv_WkIdx]	=	NULL;
					}
				}
			}
			// release assembly
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlAsmMaxCnt;Tv_WkIdx++ )
			{
				// release code
				if ( NULL != m_DvlScptAsmTbl[Tv_WkIdx].Pos )
				{
					free( m_DvlScptAsmTbl[Tv_WkIdx].Pos );
					m_DvlScptAsmTbl[Tv_WkIdx].Pos	=	NULL;
				}
				// release comment
				if ( NULL != m_DvlAsmCmt[Tv_WkIdx].StrCmt )
				{
					free( m_DvlAsmCmt[Tv_WkIdx].StrCmt );
					m_DvlAsmCmt[Tv_WkIdx].StrCmt	=	NULL;
				}
			}
			GrDumyZeroMem( m_DvlAsmInfo, sizeof(m_DvlAsmInfo) );

			// clear folder info
			GrDumyZeroMem( m_DvlFoldTbl, sizeof(m_DvlFoldTbl) );
			GrDumyZeroMem( m_DvlWinInfo, sizeof(m_DvlWinInfo) );		// develop table clear
			GrDumyZeroMem( m_DvlPageInfo, sizeof(m_DvlPageInfo) );		// develop table clear
			GrDumyZeroMem( m_DvlSlvMap, sizeof(m_DvlSlvMap) );			// slave id map clear

			// clear setup variable table
			m_DvlStpUsrCnt	=	0;
			GrDumyZeroMem( m_DvlStpUsrTbl, sizeof(m_DvlStpUsrTbl) );

			// clear setup runtime user table
			m_DvlSruCnt			=	0;
			GrDumyZeroMem( m_DvlSruTbl, sizeof(m_DvlSruTbl) );

			// clear global variable user type 
			GrDumyZeroMem( m_DvlGvuTbl, sizeof(m_DvlGvuTbl) );

			// clear setting
			GrDumyZeroMem( &m_DvlSetBase, sizeof(m_DvlSetBase) );

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

		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcGetEditObjRect( Ptr_GrRect A_PtrRcRslt, Ptr_GrRect A_PtrRcPrnt, 
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
		if ( (A_PtrRcPrnt->right - A_PtrRcPrnt->left) < A_PtrRcRslt->right )
		{
			A_PtrRcRslt->right	=	A_PtrRcPrnt->right - A_PtrRcPrnt->left;
		}
		if (0 > A_PtrRcRslt->top)
		{
			A_PtrRcRslt->top	=	0;
		}
		if ( (A_PtrRcPrnt->bottom - A_PtrRcPrnt->top) < A_PtrRcRslt->right )
		{
			A_PtrRcRslt->bottom	=	A_PtrRcPrnt->bottom - A_PtrRcPrnt->top;
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlEditObjCreate( __s32 A_ScrX, __s32 A_ScrY, __u8 A_ClassId )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP3DvlMsgEditObjCreate	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;
		
		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjCreate;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.ScrX					=	A_ScrX;
		Tv_Msg.ScrY					=	A_ScrY;
		Tv_Msg.ClassId			=	A_ClassId;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcDvlEditObjCreate( __s32 A_ScrX, __s32 A_ScrY, __u8 A_ClassId )
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrP3DoBase*	Tv_ObjNew;
		Cls_GrP3DoBase*	Tv_ObjPrnt;
		St_GrRect	Tv_RcPrnt;
		__s32		Tv_X;
		__s32		Tv_Y;
		St_GrRect	Tv_RcObj;
	// code --------------------
		Tv_Result		=	E_GrP3ScptIdNone;
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
						LcGetEditObjRect( &Tv_RcObj, &Tv_RcPrnt, Tv_X, Tv_Y, E_GrP3DvlDfltObjSizeX, E_GrP3DvlDfltObjSizeY );
						Tv_ObjNew	=	LcUiObjCreate( A_ClassId, E_GrP3ScptIdNone, &Tv_RcObj, Tv_ObjPrnt );
						if ( NULL != Tv_ObjNew )
						{
							Tv_Result	=	Tv_ObjNew->ScptIdGet();
							Tv_ObjNew->ReqDraw();
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlEditObjCreate - Can not get screen position.\n" );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlEditObjCreate - Can not create object under frame.\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlEditObjCreate - No parent object!" );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlFolderInfoGet( __u16 A_Id, WCHAR* A_StrRtName, __u16* A_PtrRtPrntId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( DvlIsPrjExist() )
		{
			if ( (E_GrP3DvlFolderIdBase <= A_Id) && ((E_GrP3DvlFolderIdBase + E_GrP3DvlFolderMaxCnt) > A_Id) )
			{
				Tv_Idx		=	A_Id - E_GrP3DvlFolderIdBase;
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
void	Cls_GrP3DvlMng::DvlFolderInfoSet( __u16 A_Id, WCHAR* A_StrName, __u16 A_PrntId )
{
	// local -------------------
		__u16	Tv_Idx;
	// code --------------------
		if ( DvlIsPrjExist() )
		{
			if ( (E_GrP3DvlFolderIdBase <= A_Id) && ((E_GrP3DvlFolderIdBase + E_GrP3DvlFolderMaxCnt) > A_Id) )
			{
				Tv_Idx		=	A_Id - E_GrP3DvlFolderIdBase;
				m_DvlFoldTbl[Tv_Idx].IsExist	=	TRUE;
				GrStrWcopy( m_DvlFoldTbl[Tv_Idx].StrName, A_StrName, 31 );
				m_DvlFoldTbl[Tv_Idx].PrntId	=	A_PrntId;
			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::DvlFolderInfoSet - bad foder id( %d ).\n", A_Id );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP3DvlMng::DvlFolderInfoSet - project not exist.\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlFolderNameSet( __u16 A_Id, WCHAR* A_StrName )
{
	// local -------------------
		__u16	Tv_Idx;
	// code --------------------
		if ( (E_GrP3DvlFolderIdBase <= A_Id) && ((E_GrP3DvlFolderIdBase + E_GrP3DvlFolderMaxCnt) > A_Id) )
		{
			Tv_Idx		=	A_Id - E_GrP3DvlFolderIdBase;
			GrStrWcopy( m_DvlFoldTbl[Tv_Idx].StrName, A_StrName, 31 );
		}
		else
		{
			DbgMsgPrint("Cls_GrP3DvlMng::DvlFolderNameSet - bad foder id( %d ).\n", A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlFolderPrntIdSet( __u16 A_Id, __u16 A_PrntId )
{
	// local -------------------
		__u16	Tv_Idx;
	// code --------------------
		if ( (E_GrP3DvlFolderIdBase <= A_Id) && ((E_GrP3DvlFolderIdBase + E_GrP3DvlFolderMaxCnt) > A_Id) )
		{
			// check parent id
			if (	(E_GrP3DvlFolderIdRoot == A_PrntId) || 
						((E_GrP3DvlFolderIdBase <= A_PrntId) && ((E_GrP3DvlFolderIdBase + E_GrP3DvlFolderMaxCnt) > A_PrntId)) 
					)
			{
				Tv_Idx		=	A_Id - E_GrP3DvlFolderIdBase;
				m_DvlFoldTbl[Tv_Idx].PrntId	=	A_PrntId;
			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::DvlFolderPrntIdSet - bad parent foder id( %d ).\n", A_PrntId );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP3DvlMng::DvlFolderPrntIdSet - bad foder id( %d ).\n", A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlFolderExpandSet( __u16 A_Id, BOOL8 A_IsExpand )
{
	// local -------------------
		__u16	Tv_Idx;
	// code --------------------
		if ( (E_GrP3DvlFolderIdBase <= A_Id) && ((E_GrP3DvlFolderIdBase + E_GrP3DvlFolderMaxCnt) > A_Id) )
		{
			Tv_Idx		=	A_Id - E_GrP3DvlFolderIdBase;
			m_DvlFoldTbl[Tv_Idx].IsExpand	=	A_IsExpand;
		}
		else
		{
			DbgMsgPrint("Cls_GrP3DvlMng::DvlFolderExpandSet - bad foder id( %d ).\n", A_Id );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlFolderIsExpand( __u16 A_Id )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( (E_GrP3DvlFolderIdBase <= A_Id) && ((E_GrP3DvlFolderIdBase + E_GrP3DvlFolderMaxCnt) > A_Id) )
		{
			Tv_Idx		=	A_Id - E_GrP3DvlFolderIdBase;
			Tv_Result	=	m_DvlFoldTbl[Tv_Idx].IsExpand;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlFolderDelete( __u16 A_Id )
{
	// local -------------------
		__u16	Tv_Idx;
		__u16	Tv_ScptId;
	// code --------------------
		if ( DvlIsPrjExist() )
		{
			if ( (E_GrP3DvlFolderIdBase <= A_Id) && ((E_GrP3DvlFolderIdBase + E_GrP3DvlFolderMaxCnt) > A_Id) )
			{
				Tv_Idx		=	A_Id - E_GrP3DvlFolderIdBase;
				m_DvlFoldTbl[Tv_Idx].IsExist	=	FALSE;
				m_DvlFoldTbl[Tv_Idx].IsExpand	=	FALSE;
				// delete window
				Tv_ScptId	=	DvlWinFindByPrntFolder( A_Id );
				while ( E_GrP3ScptIdNone != Tv_ScptId )
				{
					// delete window
					DvlWindowDel( Tv_ScptId );
					// next
					Tv_ScptId	=	DvlWinFindByPrntFolder( A_Id );
				}
				// delete page
				Tv_ScptId	=	DvlPageFindByPrntFolder( A_Id );
				while ( E_GrP3ScptIdNone != Tv_ScptId )
				{
					// delete window
					DvlPageDel( Tv_ScptId );
					// next
					Tv_ScptId	=	DvlPageFindByPrntFolder( A_Id );
				}

			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::DvlFolderDelete - bad foder id( %d ).\n", A_Id );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP3DvlMng::DvlFolderDelete - project not exist.\n");
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlWinNameGet( __u16 A_ScptId )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16	Tv_WinIdx;
	// code --------------------
		Tv_Result	=	NULL;
		if ( (E_GrP3ScptIdWinBase <= A_ScptId) && ((E_GrP3ScptIdWinBase + E_GrP3DvlWinMaxCnt) > A_ScptId) )
		{
			Tv_WinIdx	=	A_ScptId - E_GrP3ScptIdWinBase;
			// check exist window
			if ( NULL != m_PtrWinTbl[Tv_WinIdx].Pos )
			{
				Tv_Result	=	m_DvlWinInfo[Tv_WinIdx].StrName;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlPageNameGet( __u16 A_ScptId )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16	Tv_PageIdx;
	// code --------------------
		Tv_Result	=	NULL;
		if ( (E_GrP3ScptIdPageBase <= A_ScptId) && ((E_GrP3ScptIdPageBase + E_GrP3DvlPageMaxCnt) > A_ScptId) )
		{
			Tv_PageIdx	=	A_ScptId - E_GrP3ScptIdPageBase;
			// check exist page
			if ( NULL != m_PtrPageTbl[Tv_PageIdx].Pos )
			{
				Tv_Result	=	m_DvlPageInfo[Tv_PageIdx].StrName;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlWinNameSet( __u16 A_ScptId, WCHAR* A_StrName )
{
	// local -------------------
		__u16	Tv_WinIdx;
	// code --------------------
		if ( (E_GrP3ScptIdWinBase <= A_ScptId) && ((E_GrP3ScptIdWinBase + E_GrP3DvlWinMaxCnt) > A_ScptId) )
		{
			Tv_WinIdx	=	A_ScptId - E_GrP3ScptIdWinBase;
			// check exist window
			if ( NULL != m_PtrWinTbl[Tv_WinIdx].Pos )
			{
				GrStrWcopy( m_DvlWinInfo[Tv_WinIdx].StrName, A_StrName, 31 );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlPageNameSet( __u16 A_ScptId, WCHAR* A_StrName )
{
	// local -------------------
		__u16	Tv_PgIdx;
	// code --------------------
		if ( (E_GrP3ScptIdPageBase <= A_ScptId) && ((E_GrP3ScptIdPageBase + E_GrP3DvlPageMaxCnt) > A_ScptId) )
		{
			Tv_PgIdx	=	A_ScptId - E_GrP3ScptIdPageBase;
			// check exist window
			if ( NULL != m_PtrPageTbl[Tv_PgIdx].Pos )
			{
				GrStrWcopy( m_DvlPageInfo[Tv_PgIdx].StrName, A_StrName, 31 );
			}
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlWinPrntFolderGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_WinIdx;
	// code --------------------
		Tv_Result	=	E_GrP3DvlFolderIdRoot;
		if ( (E_GrP3ScptIdWinBase <= A_ScptId) && ((E_GrP3ScptIdWinBase + E_GrP3DvlWinMaxCnt) > A_ScptId) )
		{
			Tv_WinIdx	=	A_ScptId - E_GrP3ScptIdWinBase;
			// check exist window
			if ( NULL != m_PtrWinTbl[Tv_WinIdx].Pos )
			{
				Tv_Result	=	m_DvlWinInfo[Tv_WinIdx].FoldId;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlPagePrntFolderGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_PageIdx;
	// code --------------------
		Tv_Result	=	E_GrP3DvlFolderIdRoot;
		if ( (E_GrP3ScptIdPageBase <= A_ScptId) && ((E_GrP3ScptIdPageBase + E_GrP3DvlPageMaxCnt) > A_ScptId) )
		{
			Tv_PageIdx	=	A_ScptId - E_GrP3ScptIdPageBase;
			// check exist page
			if ( NULL != m_PtrPageTbl[Tv_PageIdx].Pos )
			{
				Tv_Result	=	m_DvlPageInfo[Tv_PageIdx].FoldId;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlWinPrntFolderSet( __u16 A_ScptId, __u16 A_FolderId )
{
	// local -------------------
		__u16	Tv_WinIdx;
	// code --------------------
		if ( (E_GrP3ScptIdWinBase <= A_ScptId) && ((E_GrP3ScptIdWinBase + E_GrP3DvlWinMaxCnt) > A_ScptId) )
		{
			Tv_WinIdx	=	A_ScptId - E_GrP3ScptIdWinBase;
			if ( NULL != m_PtrWinTbl[Tv_WinIdx].Pos )
			{
				m_DvlWinInfo[Tv_WinIdx].FoldId	=	A_FolderId;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlPagePrntFolderSet( __u16 A_ScptId, __u16 A_FolderId )
{
	// local -------------------
		__u16	Tv_PageIdx;
	// code --------------------
		if ( (E_GrP3ScptIdPageBase <= A_ScptId) && ((E_GrP3ScptIdPageBase + E_GrP3DvlPageMaxCnt) > A_ScptId) )
		{
			Tv_PageIdx	=	A_ScptId - E_GrP3ScptIdPageBase;
			if ( NULL != m_PtrPageTbl[Tv_PageIdx].Pos )
			{
				m_DvlPageInfo[Tv_PageIdx].FoldId	=	A_FolderId;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlWinIsExist( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_WinIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( DvlIsPrjExist() )
		{
			if ( (E_GrP3ScptIdWinBase <= A_ScptId) && ((E_GrP3ScptIdWinBase + E_GrP3DvlWinMaxCnt) > A_ScptId) )
			{
				Tv_WinIdx	=	A_ScptId - E_GrP3ScptIdWinBase;
				if ( NULL != m_PtrWinTbl[Tv_WinIdx].Pos )
				{
					Tv_Result	=	TRUE;
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlPageIsExist( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_PageIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( DvlIsPrjExist() )
		{
			if ( (E_GrP3ScptIdPageBase <= A_ScptId) && ((E_GrP3ScptIdPageBase + E_GrP3DvlPageMaxCnt) > A_ScptId) )
			{
				Tv_PageIdx	=	A_ScptId - E_GrP3ScptIdPageBase;
				if ( NULL != m_PtrPageTbl[Tv_PageIdx].Pos )
				{
					Tv_Result	=	TRUE;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcDvlEdtLumpScptUpdt( void )
{
	// local -------------------
		__u16	Tv_ScptId;
		__u16	Tv_Idx;
		void*	Tv_PtrScpt;
		__u32	Tv_ScptSize;
	// code --------------------
		if ( NULL != m_DvlEdtLump )
		{
			Tv_ScptId	=	m_DvlEdtLump->ScptIdGet();
			// check page
			if ( DvlIsPageWindow( Tv_ScptId ) )
			{
				// page
				// get script buffer
				Tv_Idx			=	Tv_ScptId - E_GrP3ScptIdPageBase;
				Tv_PtrScpt	=	m_DvlScptPageTbl[Tv_Idx].Pos;
				if ( NULL != Tv_PtrScpt )
				{
					Tv_ScptSize	=	m_DvlEdtLump->RtlScptDump( Tv_PtrScpt, FALSE );
					m_DvlPageInfo[Tv_Idx].Size		=	Tv_ScptSize;
					if ( E_GrP3DvlScptBufMax < Tv_ScptSize )
					{
						DbgMsgPrint("Cls_GrP3DvlMng::LcEdtLumpScptUpdt - page ( %d ) script size ( %d ) over!", Tv_ScptId, Tv_ScptSize );
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcEdtLumpScptUpdt - page ( %d ) script buffer not allocated!", Tv_ScptId );
				}
			}
			else
			{
				// widow
				// get script buffer
				Tv_Idx			=	Tv_ScptId - E_GrP3ScptIdWinBase;
				Tv_PtrScpt	=	m_DvlScptWinTbl[Tv_Idx].Pos;
				if ( NULL != Tv_PtrScpt )
				{
					Tv_ScptSize	=	m_DvlEdtLump->RtlScptDump( Tv_PtrScpt, FALSE );
					m_DvlWinInfo[Tv_Idx].Size		=	Tv_ScptSize;
					if ( E_GrP3DvlScptBufMax < Tv_ScptSize )
					{
						DbgMsgPrint("Cls_GrP3DvlMng::LcEdtLumpScptUpdt - window ( %d ) script size ( %d ) over!", Tv_ScptId, Tv_ScptSize );
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcEdtLumpScptUpdt - window ( %d ) script buffer not allocated!", Tv_ScptId );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlEditLumpScptUpdt( void )
{
	// local -------------------
		St_GrP3DvlMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Cmd				=	E_GrP3DvlMcmdEdtLumpScptUpdt;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlEditLumpGet( void )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP3DvlMsgEditLumpGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditLumpGet;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcDvlEditLumpGet( void )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;
		if ( NULL != m_DvlEdtLump )
		{
			Tv_Result	=	m_DvlEdtLump->ScptIdGet();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjPstDfltBuild( void )
{
	// local -------------------
		__u16	Tv_Idx;
		__u8	Tv_SkinIdx;
		Ptr_GrP3ScptPstData	Tv_PtrPst;
	// code --------------------
		Tv_Idx	=	E_GrP3ScptPstIdDflt - E_GrP3ScptPstIdBase;
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP3DvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			if ( NULL == m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos )
			{
				m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos	=	malloc( E_GrP3DvlPstBufMax );
				if ( NULL != m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos )
				{
					Tv_PtrPst	=	(Ptr_GrP3ScptPstData)m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos;

					Tv_PtrPst[0].Left		=	0;
					Tv_PtrPst[0].Top		=	0;
					Tv_PtrPst[0].Right	=	4;
					Tv_PtrPst[0].Bottom	=	0;
					Tv_PtrPst[0].Basis	=	E_GrP3ScptPstBasisSt | (E_GrP3ScptPstBasisSt << 2) | (E_GrP3ScptPstBasisSt << 4) | (E_GrP3ScptPstBasisEd << 6);
					Tv_PtrPst[0].Type		=	E_GrP3ScptPstTypeLine;
					Tv_PtrPst[0].Para0	=	1;
					Tv_PtrPst[0].Para1	=	0x11;
					Tv_PtrPst[0].Value	=	GrDrawCvtColorByFcc( E_GrFccRGB24, 0xD0D0D0 , m_PtrCfgPrj->FmtFcc );

					Tv_PtrPst[1].Left		=	0;
					Tv_PtrPst[1].Top		=	0;
					Tv_PtrPst[1].Right	=	0;
					Tv_PtrPst[1].Bottom	=	4;
					Tv_PtrPst[1].Basis	=	E_GrP3ScptPstBasisSt | (E_GrP3ScptPstBasisSt << 2) | (E_GrP3ScptPstBasisEd << 4) | (E_GrP3ScptPstBasisSt << 6);
					Tv_PtrPst[1].Type		=	E_GrP3ScptPstTypeLine;
					Tv_PtrPst[1].Para0	=	0;
					Tv_PtrPst[1].Para1	=	0x11;
					Tv_PtrPst[1].Value	=	GrDrawCvtColorByFcc( E_GrFccRGB24, 0xF0F0F0 , m_PtrCfgPrj->FmtFcc );

					Tv_PtrPst[2].Left		=	4;
					Tv_PtrPst[2].Top		=	0;
					Tv_PtrPst[2].Right	=	0;
					Tv_PtrPst[2].Bottom	=	0;
					Tv_PtrPst[2].Basis	=	E_GrP3ScptPstBasisEd | (E_GrP3ScptPstBasisSt << 2) | (E_GrP3ScptPstBasisEd << 4) | (E_GrP3ScptPstBasisEd << 6);
					Tv_PtrPst[2].Type		=	E_GrP3ScptPstTypeLine;
					Tv_PtrPst[2].Para0	=	1;
					Tv_PtrPst[2].Para1	=	0x22;
					Tv_PtrPst[2].Value	=	GrDrawCvtColorByFcc( E_GrFccRGB24, 0x404040 , m_PtrCfgPrj->FmtFcc );

					Tv_PtrPst[3].Left		=	0;
					Tv_PtrPst[3].Top		=	4;
					Tv_PtrPst[3].Right	=	0;
					Tv_PtrPst[3].Bottom	=	0;
					Tv_PtrPst[3].Basis	=	E_GrP3ScptPstBasisSt | (E_GrP3ScptPstBasisEd << 2) | (E_GrP3ScptPstBasisEd << 4) | (E_GrP3ScptPstBasisEd << 6);
					Tv_PtrPst[3].Type		=	E_GrP3ScptPstTypeLine;
					Tv_PtrPst[3].Para0	=	0;
					Tv_PtrPst[3].Para1	=	0x22;
					Tv_PtrPst[3].Value	=	GrDrawCvtColorByFcc( E_GrFccRGB24, 0x202020 , m_PtrCfgPrj->FmtFcc );

					Tv_PtrPst[4].Left		=	0;
					Tv_PtrPst[4].Top		=	0;
					Tv_PtrPst[4].Right	=	0;
					Tv_PtrPst[4].Bottom	=	0;
					Tv_PtrPst[4].Basis	=	E_GrP3ScptPstBasisSt | (E_GrP3ScptPstBasisSt << 2) | (E_GrP3ScptPstBasisEd << 4) | (E_GrP3ScptPstBasisEd << 6);
					Tv_PtrPst[4].Type		=	E_GrP3ScptPstTypeClrFill;
					Tv_PtrPst[4].Para0	=	0;
					Tv_PtrPst[4].Para1	=	0;
					Tv_PtrPst[4].Value	=	GrDrawCvtColorByFcc( E_GrFccRGB24, 0xB0B0B0 , m_PtrCfgPrj->FmtFcc );

					// update script count
					m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Cnt		=	5;

				}
				else
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjPstDfltBuild - mamory allocate failed.\n");
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjPstDfltBuild - paint script buffer not initate.\n");
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlEditSkinSet( __u8 A_SkinIdx )
{
	// local -------------------
		St_GrP3DvlMsgEditSkinSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditSkinSet;
		Tv_Msg.SkinIdx			=	A_SkinIdx;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcDvlEditSkinSet( __u8 A_SkinIdx )
{
	// local -------------------
	// code --------------------
		if ( DvlIsPrjExist() )
		{
			if ( E_GrP3DvlSkinMaxCnt > A_SkinIdx )
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
void	Cls_GrP3DvlMng::LcPrjSkinNameBuild( void )
{
	// local -------------------
		__s32		Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlSkinMaxCnt;Tv_WkIdx++ )
		{
			GrStrIntToWstr( m_DvlPrjSkin[Tv_WkIdx].StrName, Tv_WkIdx );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlSkinNameGet( __u8 A_SkinIdx )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( DvlIsPrjExist() )
		{
			if ( E_GrP3DvlSkinMaxCnt > A_SkinIdx )
			{
				Tv_Result	=	m_DvlPrjSkin[A_SkinIdx].StrName;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlSkinNameSet( __u8 A_SkinIdx, WCHAR* A_StrName )
{
	// local -------------------
	// code --------------------
		if ( DvlIsPrjExist() )
		{
			if ( E_GrP3DvlSkinMaxCnt > A_SkinIdx )
			{
				GrStrWcopy( m_DvlPrjSkin[A_SkinIdx].StrName, A_StrName );
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrP3DvlMng::DvlEditSkinGet( void )
{
		return	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
}
//--------------------------------------------------------------------
Ptr_GrP3ScptPstData	Cls_GrP3DvlMng::DvlPstCreate( __u16 A_Id )
{
	// local -------------------
		Ptr_GrP3ScptPstData	Tv_Result;
		__u16	Tv_Idx;
		__u8	Tv_SkinIdx;
	// code --------------------
		// check already exist
		Tv_Result	=	NULL;
		if ( (E_GrP3ScptPstIdBase <= A_Id) && ((E_GrP3ScptPstIdBase + E_GrP3DvlPstMaxCnt) > A_Id ) )
		{
			Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
			Tv_Idx	=	A_Id - E_GrP3ScptPstIdBase;
			if ( NULL == m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos )
			{
				m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Cnt	=	0;		// count reset
				m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos	=	malloc( E_GrP3DvlPstBufMax );
				if ( NULL == m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos )
				{
					DbgMsgPrint( "Cls_GrP3DvlMng::DvlPstCreate - pst ( %d ) memory allocate error.\n", A_Id );
				}
				Tv_Result	=	(Ptr_GrP3ScptPstData)m_DvlScptPstTbl[Tv_SkinIdx][Tv_Idx].Pos;
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3DvlMng::DvlPstCreate - pst ( %d )  is exist.\n", A_Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DvlMng::DvlPstCreate - bad id ( %d ) input.\n", A_Id );
		}
		return	Tv_Result;		
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlPstCodeCountSet( __u16 A_Id, __u8 A_Cnt )
{
	// local -------------------
		__u16	Tv_Idx;
		__u8	Tv_SkinIdx;
	// code --------------------
		if ( E_GrP3DvlPstCodeMax >= A_Cnt )
		{
			if ( (E_GrP3ScptPstIdBase <= A_Id) && ((E_GrP3ScptPstIdBase + E_GrP3DvlPstMaxCnt) > A_Id ) )
			{
				Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
				Tv_Idx			=	A_Id - E_GrP3ScptPstIdBase;
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
			DbgMsgPrint( "Cls_GrP3DvlMng::DvlPstCodeCountSet - try over count ( %d ) set to pst ( %d ).\n" , A_Cnt, A_Id );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlPstCodeNameGet( __u16 A_Id )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16		Tv_Idx;
	// code --------------------
		Tv_Result	=	NULL;
		if ( (E_GrP3ScptPstIdBase <= A_Id) && ((E_GrP3ScptPstIdBase + E_GrP3DvlPstMaxCnt) > A_Id ) )
		{
			Tv_Idx	=	A_Id - E_GrP3ScptPstIdBase;
			Tv_Result	=	m_DvlPstInfo[Tv_Idx].StrName;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlPstCodeNameSet( __u16 A_Id, WCHAR* A_StrName )
{
	// local -------------------
		__u16		Tv_Idx;
	// code --------------------
		if ( (E_GrP3ScptPstIdBase <= A_Id) && ((E_GrP3ScptPstIdBase + E_GrP3DvlPstMaxCnt) > A_Id ) )
		{
			Tv_Idx	=	A_Id - E_GrP3ScptPstIdBase;
			GrStrWcopy( m_DvlPstInfo[Tv_Idx].StrName, A_StrName, 31 );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlImgLoadBmp( __u16 A_Id, WCHAR* A_StrFn )
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
		if ( (E_GrP3ImgIdBase <= A_Id) && ((E_GrP3ImgIdBase + E_GrP3MngImgScptMaxCnt) > A_Id) )
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
					Tv_SlotIdx	=	A_Id - E_GrP3ImgIdBase;
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
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlImgDelete( __u16 A_Id )
{
	// local -------------------
		__u16	Tv_SlotIdx;
		__u8	Tv_SkinIdx;
	// code --------------------
		// check current id
		if ( (E_GrP3ImgIdBase <= A_Id) && ((E_GrP3ImgIdBase + E_GrP3MngImgScptMaxCnt) > A_Id) )
		{
			Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
			Tv_SlotIdx	=	A_Id - E_GrP3ImgIdBase;
			// check exist
			if ( NULL != m_DvlScptImgTbl[Tv_SkinIdx][Tv_SlotIdx] )
			{
				// release
				free( m_DvlScptImgTbl[Tv_SkinIdx][Tv_SlotIdx] );
				m_DvlScptImgTbl[Tv_SkinIdx][Tv_SlotIdx]	=	NULL;
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DvlMng::DvlClrPltGet( __u8 A_ClrIdx )
{
		return	m_DvlPrjRtl.ClrPltTbl[A_ClrIdx];
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlClrPltSet( __u8 A_ClrIdx, __u32 A_Color )
{
		m_DvlPrjRtl.ClrPltTbl[A_ClrIdx]	=	A_Color;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlTxtNameGet( __u16 A_Id, WCHAR* A_StrRt )
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
		if ( E_GrP3ScptTxtIdBase <= A_Id )
		{
			Tv_TxtIdx	=	A_Id - E_GrP3ScptTxtIdBase;
			// check text count
			if ( E_GrP3MngTextScptMaxCnt > Tv_TxtIdx )
			{
				// check system text
				if ( E_GrP3DvlTextSysCnt > Tv_TxtIdx )
				{
					// find system text
					Tv_Isfound	=	FALSE;
					Tv_WkIdx	=	0;
					while( E_GrP3ScptTxtIdNone != V_GrP3DvlTxtSysName[Tv_WkIdx].Id )
					{
						// check text id
						if ( A_Id == V_GrP3DvlTxtSysName[Tv_WkIdx].Id )
						{
							GrStrWcopy( A_StrRt, V_GrP3DvlTxtSysName[Tv_WkIdx].StrName );
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
void	Cls_GrP3DvlMng::DvlTxtSet( __u16 A_Id, __u8 A_LangIdx, WCHAR* A_Str )
{
	// local -------------------
		__u16	Tv_TxtIdx;

	// code --------------------
		// check able id
		if ( (E_GrP3ScptTxtIdBase <= A_Id) && (E_GrP3ScptLangMaxCnt > A_LangIdx) )
		{
			Tv_TxtIdx	=	A_Id - E_GrP3ScptTxtIdBase;
			if ( E_GrP3MngTextScptMaxCnt > Tv_TxtIdx )
			{
				// check exist text
				if ( GrStrIsExist(A_Str) )
				{
					// check exist
					if ( NULL == m_DvlScptTxtTbl[Tv_TxtIdx].LngTbl[A_LangIdx] )
					{
						// allocate
						m_DvlScptTxtTbl[Tv_TxtIdx].LngTbl[A_LangIdx]	=	(WCHAR*)malloc( E_GrP3DvlTextBufSize );
					}
					if ( NULL != m_DvlScptTxtTbl[Tv_TxtIdx].LngTbl[A_LangIdx] )
					{
						GrStrWcopy( m_DvlScptTxtTbl[Tv_TxtIdx].LngTbl[A_LangIdx] , A_Str, E_GrP3DvlTextMaxLen );
					}
					else
					{
						DbgMsgPrint( "Cls_GrP3DvlMng::DvlTxtSet - text ( %d ) buffer allocate error.\n\n", A_Id );
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
void	Cls_GrP3DvlMng::LcPrjPstNameBuild( void )
{
	// local -------------------
		__s32		Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlPstMaxCnt;Tv_WkIdx++)
		{
			GrStrIntToWstr( m_DvlPstInfo[Tv_WkIdx].StrName, Tv_WkIdx + E_GrP3ScptPstIdBase );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlFontDelete( __u8 A_Id )
{
	// local -------------------
		__u8	Tv_SkinIdx;
	// code --------------------
		// check id
		if ( E_GrP3MngFontMaxCnt > A_Id )
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
BOOL8	Cls_GrP3DvlMng::DvlFontLoadFile( __u8 A_Id, WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_SkinIdx;
		__u32	Tv_FileSize;
		Ptr_GrP3FontHead	Tv_PtrFont;
	// code --------------------
		// init
		Tv_Result	=	FALSE;

		// check id
		if ( E_GrP3MngFontMaxCnt > A_Id )
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
				Tv_PtrFont	=	(Ptr_GrP3FontHead) malloc( Tv_FileSize );
				if ( NULL != Tv_PtrFont )
				{
					// load
					if ( Tv_FileSize == GrFileCtrlRead( A_StrFn, Tv_PtrFont, Tv_FileSize, TRUE ) )
					{
						// check fcc
						if ( E_GrP3FontVer2Id == Tv_PtrFont->Fcc )
						{
							// check size
							if ( GrP3FontCalcSize( Tv_PtrFont ) <= Tv_FileSize )
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
__u8	Cls_GrP3DvlMng::DvlEditModelIdxGet( void )
{
		return	m_DvlPrjRtl.ModelIdx;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlEditModelIdxSet( __u8 A_ModelIdx )
{
	// local -------------------
		St_GrP3DvlMsgEditModelIdxSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditModelIdxSet;
		Tv_Msg.ModelIdx			=	A_ModelIdx;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcDvlEditModelIdxSet( __u8 A_ModelIdx )
{
	// local -------------------
		__u8	Tv_SkinIdx;
	// code --------------------
		// check index over
		if ( E_GrP3DvlModelMaxCnt > A_ModelIdx )
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
void	Cls_GrP3DvlMng::DvlModelCopyTo( __u8 A_TargetIdx )
{
	// local -------------------
	// code --------------------
		// project exist
		if ( DvlIsPrjExist() )
		{
			// check target model number
			if ( E_GrP3DvlModelMaxCnt > A_TargetIdx )
			{
				// check different with edit model
				if ( m_DvlPrjRtl.ModelIdx != A_TargetIdx )
				{
					// copy skin index
					m_DvlPrjModel[A_TargetIdx].SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
					// copy model attribute
					GrDumyCopyMem( &m_DvlPrjMdlAtb[A_TargetIdx], &m_DvlPrjMdlAtb[m_DvlPrjRtl.ModelIdx] , sizeof(St_GrP3ScptCfgMdl) );
					DbgMsgPrint( "Cls_GrP3DvlMng::DvlModelCopyTo - model copy done.\n" );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3DvlMng::DvlModelCopyTo - model can not copy self.\n" );
				}
			}
			else 
			{
				DbgMsgPrint( "Cls_GrP3DvlMng::DvlModelCopyTo - bad target model number.\n" );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DvlMng::DvlModelCopyTo - project not exist\n" );
		}
}
//--------------------------------------------------------------------
void*	Cls_GrP3DvlMng::DvlModelAtbGet( __u8 A_ModelIdx )
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( E_GrP3DvlModelMaxCnt > A_ModelIdx )
		{
			Tv_Result	=	&m_DvlPrjMdlAtb[A_ModelIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrP3DvlMng::DvlModelDbgGet( __u8 A_ModelIdx )
{
	// local -------------------
		void*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( E_GrP3DvlModelMaxCnt > A_ModelIdx )
		{
			Tv_Result	=	&m_DvlMdlDblTbl[A_ModelIdx];
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlModelNameGet( __u8 A_ModelIdx )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( E_GrP3DvlModelMaxCnt > A_ModelIdx )
		{
			Tv_Result	=	m_DvlPrjModel[A_ModelIdx].StrName;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlModelNameSet( __u8 A_ModelIdx, WCHAR* A_StrName )
{
		if ( E_GrP3DvlModelMaxCnt > A_ModelIdx )
		{
			GrStrWcopy( m_DvlPrjModel[A_ModelIdx].StrName, A_StrName, 31 );
		}
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlModelNumberGet( __u8 A_ModelIdx )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrP3DvlModelMaxCnt > A_ModelIdx )
		{
			Tv_Result	=	m_DvlPrjMdlAtb[A_ModelIdx].ModelNum;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlModelNumberSet( __u8 A_ModelIdx, __u16 A_Number )
{
		if ( E_GrP3DvlModelMaxCnt > A_ModelIdx )
		{
			m_DvlPrjMdlAtb[A_ModelIdx].ModelNum	=	A_Number;
		}
}
//-------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveWinDvlInfo( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		// window developed table write
		Tv_DataHd.Type	=	E_GrP3DvlPrjDataWinDvlTbl;
		Tv_DataHd.Size	=	sizeof(m_DvlWinInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlWinInfo, Tv_DataHd.Size );	// write scripte head
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSavePageDvlInfo( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		// page develope table write
		Tv_DataHd.Type	=	E_GrP3DvlPrjDataPageDvlTbl;
		Tv_DataHd.Size	=	sizeof(m_DvlPageInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlPageInfo, Tv_DataHd.Size );	// write scripte head
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveSdaData( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		// SDA data write
		Tv_DataHd.Type	=	E_GrP3DvlPrjDataSdaData;
		Tv_DataHd.Size	=	sizeof(m_SdaBuf);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_SdaBuf, Tv_DataHd.Size );	// write data
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;

}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSavePstDvlInfo( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		// PST develop table write
		Tv_DataHd.Type	=	E_GrP3DvlPrjDataPstDvlTbl;
		Tv_DataHd.Size	=	sizeof(m_DvlPstInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlPstInfo, Tv_DataHd.Size );	// write scripte head
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveAsmComment( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
		__u16	Tv_WkIdx;
	// code --------------------
		// write assembly comment
		Tv_DataHd.Type	=	E_GrP3DvlPrjDataAsmCmt;
		Tv_DataHd.SubIdx	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlAsmMaxCnt;Tv_WkIdx++ )
		{
			// check exist
			if ( NULL != m_DvlAsmCmt[Tv_WkIdx].StrCmt )
			{
				if ( GrStrIsExist(m_DvlAsmCmt[Tv_WkIdx].StrCmt) )
				{
					// write header
					Tv_DataHd.Id		=	Tv_WkIdx;
					Tv_DataHd.Size	=	GrStrWlen( m_DvlAsmCmt[Tv_WkIdx].StrCmt );
					if ( 0 != Tv_DataHd.Size )
					{
						Tv_DataHd.Size	=	 ((Tv_DataHd.Size + 2) & 0xFFFFFFFE) * 2;	// patch size
						m_DvlSaved		=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
						m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
						m_DvlSaved		=	m_DvlSaved + m_DvlFc->Write( m_DvlAsmCmt[Tv_WkIdx].StrCmt, Tv_DataHd.Size );	// write script head
						m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
					}
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveMdlDbg( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
		__u16	Tv_WkIdx;
	// code --------------------
		// write data
		Tv_DataHd.Type	=	E_GrP3DvlPrjDataMdlDebug;
		Tv_DataHd.SubIdx	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlModelMaxCnt;Tv_WkIdx++ )
		{
			// write header
			Tv_DataHd.Id		=	Tv_WkIdx;
			Tv_DataHd.Size	=	sizeof(St_GrP3PrjDbgInfo);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
			m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlMdlDblTbl[Tv_WkIdx], Tv_DataHd.Size );	// write data
			m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveBldFile( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3DvlPrjDataBldFile;
		Tv_DataHd.SubIdx	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlModelMaxCnt;Tv_WkIdx++ )
		{
			// write header
			Tv_DataHd.Id		=	Tv_WkIdx;
			Tv_DataHd.Size	=	sizeof(St_GrP3PrjBldFile);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
			m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlBldInfo[Tv_WkIdx], Tv_DataHd.Size );	// write data
			m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveImage( void )
{
	// local -------------------
		__u8	Tv_SkinIdx;
	// code --------------------
		// script data write
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP3DvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			LcScptSaveImgData( Tv_SkinIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveFont( void )
{
	// local -------------------
		__u8	Tv_SkinIdx;
	// code --------------------
		// script data write
		for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP3DvlSkinMaxCnt;Tv_SkinIdx++ )
		{
			LcScptSaveFontData( Tv_SkinIdx );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlPrjSave( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3DvlMsgPrjSave	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdPrjSave;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.StrFn				=	A_StrFn;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSavePrjCfg( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3DvlPrjConfig;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlPrjConfig);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlPrjConfig, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSavePrjHead( void )
{
	// local -------------------
		St_GrP3PrjHdInfo	Tv_Hd;
	// code --------------------
		Tv_Hd.Fcc		=	E_GrP3DvlPrjFcc;
		Tv_Hd.Ver		=	E_GrP3DvlPrjVer;

		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_Hd, sizeof(Tv_Hd) );
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_Hd);
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSavePrjRtl( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3DvlPrjRuntime;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlPrjRtl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlPrjRtl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSavePrjModel( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3DvlPrjModel;
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlModelMaxCnt;Tv_WkIdx++ )
		{
			Tv_DataHd.Id			=	Tv_WkIdx;
			Tv_DataHd.SubIdx	=	0;
			Tv_DataHd.Size		=	sizeof(St_GrP3PrjModelCfg);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
			m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlPrjModel[Tv_WkIdx], Tv_DataHd.Size );	// write script
			m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSavePrjSkin( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3DvlPrjSkin;
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlSkinMaxCnt;Tv_WkIdx++ )
		{
			Tv_DataHd.Id			=	Tv_WkIdx;
			Tv_DataHd.SubIdx	=	0;
			Tv_DataHd.Size		=	sizeof(St_GrP3PrjSkinCfg);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
			m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
			m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlPrjSkin[Tv_WkIdx], Tv_DataHd.Size );	// write script
			m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSavePrjMdlAtb( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlModelMaxCnt;Tv_WkIdx++)
		{
			LcScptSaveMdlAtb( Tv_WkIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveFolderInfo( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type	=	E_GrP3DvlPrjDataFolderInfo;
		Tv_DataHd.Size	=	sizeof(m_DvlFoldTbl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlFoldTbl, Tv_DataHd.Size );	// write script head
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSavePstTable( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlSkinMaxCnt;Tv_WkIdx++)
		{
			LcScptSavePstTbl( Tv_WkIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSavePstCode( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for (Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlSkinMaxCnt;Tv_WkIdx++)
		{
			LcScptSavePstScpt( Tv_WkIdx );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveAsmDvlInfo( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type	=	E_GrP3DvlPrjDataAsmInfo;
		Tv_DataHd.Size	=	sizeof(m_DvlAsmInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlAsmInfo, Tv_DataHd.Size );	// write script head
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcDvlPrjSave( WCHAR* A_StrFn )
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
				// setup runtime user
				LcPrjSaveSru();
				// setup system write
				//LcPrjSaveSetupSys(); - not use
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
				LcPrjSaveAsmComment();
				// text
				LcScptSaveTxtData();

				// check success
				if ( m_DvlSaved == m_DvlSaveReq )
				{
					Tv_Result	=	TRUE;
					DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlPrjSave - project file %d bytes saved.\n" , m_DvlSaved );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlPrjSave - project file save requested as %d bytes, but %d bytes saved.\n", m_DvlSaveReq, m_DvlSaved );
				}
				
			}
			delete	m_DvlFc;
			m_DvlFc	=	NULL;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadOldScptHd( __u32 A_Size )
{
	// local -------------------
		Ptr_GrP3DvlScptHeadOld	Tv_PtrHd;
	// code --------------------

		if ( sizeof(St_GrP3DvlScptHeadOld) >= A_Size )
		{
			if ( sizeof(St_GrP3DvlScptHeadOld) != A_Size )
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadOldScptHd - (warnning) size miss match !\n");
			}

			// memory allocate
			Tv_PtrHd	=	(Ptr_GrP3DvlScptHeadOld)malloc( sizeof(St_GrP3DvlScptHeadOld) );
			if ( NULL != Tv_PtrHd )
			{
				// read
				if ( sizeof(St_GrP3DvlScptHeadOld) == m_DvlFc->Read( Tv_PtrHd, sizeof(St_GrP3DvlScptHeadOld) ) )
				{
					// script project realign
					m_DvlScptCfgPrj.FmtFcc		=	Tv_PtrHd->CfgPrj.FmtFcc;
					m_DvlScptCfgPrj.CusImgId	=	Tv_PtrHd->CfgPrj.CusImgId;
					m_DvlScptCfgPrj.HwType		=	Tv_PtrHd->CfgPrj.HwType;
					m_DvlScptCfgPrj.UartCnt		=	Tv_PtrHd->CfgPrj.UartCnt;
					m_DvlScptCfgPrj.UiResId		=	Tv_PtrHd->CfgPrj.UiResId;
					m_DvlScptCfgPrj.FwVer			=	Tv_PtrHd->CfgPrj.FwVer;
					GrDumyCopyMem( m_DvlScptCfgPrj.VkbdTbl, Tv_PtrHd->CfgPrj.VkbdTbl, (sizeof(St_GrP3ScptVkbdLang) * E_GrP3ScptLangMaxCnt) );

					// runtime realign
					m_DvlScptCfgRtl.IsChgConAble	=	Tv_PtrHd->CfgRtl.IsChgConAble;
					GrDumyCopyMem( &m_DvlScptCfgRtl.Skin, &Tv_PtrHd->CfgRtl.Skin, sizeof(St_GrP3ScptCfgSkin) );
				}
				else
				{
					m_DvlIsLoading	=	FALSE;
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadOldScptHd - read file error!\n");
				}
				// release buffer
				free( Tv_PtrHd );
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadOldScptHd - work memory allocate fail!\n");
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadScpt - script header size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadFolderTbl( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlFoldTbl) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlFoldTbl, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadFolderTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadFolderTbl - folder table size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadWinScptTbl( __u32 A_Size )
{
	// local -------------------
		__u16	Tv_WkIdx;
	// code --------------------
		if ( sizeof(m_DvlScptWinTbl) >= A_Size )
		{
			if ( A_Size == m_DvlFc->Read( m_DvlScptWinTbl, A_Size ) )
			{
				// patch table
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlWinMaxCnt;Tv_WkIdx++ )
				{
					m_DvlScptWinTbl[Tv_WkIdx].Pos		=	NULL;		// reset pointer
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadWinScptTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadWinScptTbl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadPageScptTbl( __u32 A_Size )
{
	// local -------------------
		__u16	Tv_WkIdx;
	// code --------------------
		if ( sizeof(m_DvlScptPageTbl) >= A_Size )
		{
			if ( A_Size == m_DvlFc->Read( m_DvlScptPageTbl, A_Size ) )
			{
				// patch table
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlPageMaxCnt;Tv_WkIdx++ )
				{
					m_DvlScptPageTbl[Tv_WkIdx].Pos		=	NULL;		// reset pointer
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPageScptTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPageScptTbl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadWinDvlTbl( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlWinInfo) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlWinInfo, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadWinDvlTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadWinDvlTbl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadPageDvlTbl( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlPageInfo) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlPageInfo, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPageDvlTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPageDvlTbl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadSdaData( __u32 A_Size )
{
	// local -------------------
		__u32	Tv_Size;
		__u64	Tv_Pos;
	// code --------------------
		Tv_Size	=	sizeof(m_SdaBuf);
		if ( Tv_Size != A_Size )
		{
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadSdaData - data size mismatch!\n");
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
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadSdaData - read file error!\n");
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
void	Cls_GrP3DvlMng::LcPrjLoadWinScptData( __u32 A_Size, __u16 A_Id )
{
	// local -------------------
	// code --------------------
		// check data size
		if ( E_GrP3DvlScptBufMax >= A_Size )
		{
			// check id range
			if ( E_GrP3DvlWinMaxCnt > A_Id )
			{
				// check already allocated
				if ( NULL == m_DvlScptWinTbl[A_Id].Pos )
				{
					// allocate script buffer
					m_DvlScptWinTbl[A_Id].Pos	=	(void*)malloc( E_GrP3DvlScptBufMax );
					if ( NULL != m_DvlScptWinTbl[A_Id].Pos )
					{
						// read
						if ( A_Size == m_DvlFc->Read( m_DvlScptWinTbl[A_Id].Pos, A_Size ) )
						{
							// check size missmatch
							if ( A_Size != m_DvlWinInfo[A_Id].Size )
							{
								DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadWinScptData - warnning - script ( %d ) size mismatch!\n", A_Id + E_GrP3ScptIdWinBase );
								m_DvlWinInfo[A_Id].Size	=	A_Size;
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadWinScptData - read file error!\n");
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
						DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadWinScptData - script buffer allocate error.\n");
						m_DvlIsLoading	=	FALSE;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadWinScptData - script ( %d ) overrun.\n" , A_Id + E_GrP3ScptIdWinBase );
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadWinScptData - bad input id ( %d )!\n", A_Id + E_GrP3ScptIdWinBase );
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadWinScptData - script( %d ) was big!\n", A_Id + E_GrP3ScptIdWinBase );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadPageScptData( __u32 A_Size, __u16 A_Id )
{
	// local -------------------
	// code --------------------
		// check data size
		if ( E_GrP3DvlScptBufMax >= A_Size )
		{
			// check id range
			if ( E_GrP3DvlPageMaxCnt > A_Id )
			{
				// check already allocated
				if ( NULL == m_DvlScptPageTbl[A_Id].Pos )
				{
					// allocate script buffer
					m_DvlScptPageTbl[A_Id].Pos	=	(void*)malloc( E_GrP3DvlScptBufMax );
					if ( NULL != m_DvlScptPageTbl[A_Id].Pos )
					{
						// read
						if ( A_Size == m_DvlFc->Read( m_DvlScptPageTbl[A_Id].Pos, A_Size ) )
						{
							// check size mismatch
							if ( A_Size != m_DvlPageInfo[A_Id].Size )
							{
								DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPageScptData - warnning - script ( %d ) size mismatch!\n", A_Id + E_GrP3ScptIdPageBase );
								m_DvlPageInfo[A_Id].Size	=	A_Size;
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPageScptData - read file error!\n");
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
						DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPageScptData - script buffer allocate error.\n");
						m_DvlIsLoading	=	FALSE;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPageScptData - script ( %d ) overrun.\n" , A_Id + E_GrP3ScptIdPageBase );
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPageScptData - bad input id ( %d )!\n", A_Id + E_GrP3ScptIdPageBase );
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPageScptData - script( %d ) was big!\n", A_Id + E_GrP3ScptIdPageBase );
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadPstScptTblWhole( __u32 A_Size )
{
	// local -------------------
		__u16	Tv_WkIdx;
		__u8	Tv_SkinIdx;
	// code --------------------
		if ( sizeof(m_DvlScptPstTbl) >= A_Size )
		{
			if ( A_Size == m_DvlFc->Read( m_DvlScptPstTbl, A_Size ) )
			{
				for ( Tv_SkinIdx=0;Tv_SkinIdx < E_GrP3DvlSkinMaxCnt;Tv_SkinIdx++ )
				{
					for( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlPstMaxCnt;Tv_WkIdx++ )
					{
						m_DvlScptPstTbl[Tv_SkinIdx][Tv_WkIdx].Pos	=	NULL;		// clear pointer
					}
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstScptTblWhole - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstScptTblWhole - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadPstScptTblOne( __u32 A_Size, __u16 A_SkinIdx )
{
	// local -------------------
		__u16	Tv_WkIdx;
	// code --------------------
		if ( (sizeof(St_GrP3ScptPstItm) * E_GrP3DvlPstMaxCnt) >= A_Size )
		{
			if ( E_GrP3DvlSkinMaxCnt > A_SkinIdx )
			{
				if ( A_Size == m_DvlFc->Read( m_DvlScptPstTbl[A_SkinIdx], A_Size ) )
				{
					// pointer reset
					for( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlPstMaxCnt;Tv_WkIdx++ )
					{
						m_DvlScptPstTbl[A_SkinIdx][Tv_WkIdx].Pos	=	NULL;		// clear pointer
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstScptTblOne - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstScptTblOne - bad skin index ( %d )!\n", A_SkinIdx);
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstScptTblOne - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadAsmScptTbl( __u32 A_Size )
{
	// local -------------------
		__u16	Tv_WkIdx;
	// code --------------------
		if ( sizeof(m_DvlScptAsmTbl) >= A_Size )
		{
			if ( A_Size == m_DvlFc->Read( m_DvlScptAsmTbl, A_Size ) )
			{
				for( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlAsmMaxCnt;Tv_WkIdx++ )
				{
					m_DvlScptAsmTbl[Tv_WkIdx].Pos	=	NULL;		// clear pointer
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmScptTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmScptTbl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadPstDvlTbl( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlPstInfo) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlPstInfo, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstDvlTbl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstDvlTbl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadAsmInfo( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlAsmInfo) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlAsmInfo, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmInfo - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmInfo - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadPstScptData( __u32 A_Size, __u16 A_Id, __u8 A_SkinIdx )
{
	// local -------------------
	// code --------------------
		// check data size
		if ( E_GrP3DvlPstBufMax >= A_Size )
		{
			// check id range
			if ( E_GrP3DvlPstMaxCnt > A_Id )
			{
				// check skin range
				if ( E_GrP3DvlSkinMaxCnt > A_SkinIdx )
				{
					// check already allocated
					if ( NULL == m_DvlScptPstTbl[A_SkinIdx][A_Id].Pos )
					{
						// allocate script buffer
						m_DvlScptPstTbl[A_SkinIdx][A_Id].Pos	=	(void*)malloc( E_GrP3DvlPstBufMax );
						if ( NULL != m_DvlScptPstTbl[A_SkinIdx][A_Id].Pos )
						{
							// read
							if ( A_Size == m_DvlFc->Read( m_DvlScptPstTbl[A_SkinIdx][A_Id].Pos, A_Size ) )
							{
								// check size missmatch
								if ( A_Size != ( sizeof(St_GrP3ScptPstData) * m_DvlScptPstTbl[A_SkinIdx][A_Id].Cnt ) )
								{
									DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstScptData - warnning - pst ( %d ) size mismatch!\n", A_Id + E_GrP3ScptPstIdBase );
								}
							}
							else
							{
								DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstScptData - read file error!\n");
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
							DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstScptData - pst buffer allocate error.\n");
							m_DvlIsLoading	=	FALSE;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstScptData - pst ( %d ) overrun.\n" , A_Id + E_GrP3ScptPstIdBase );
						m_DvlIsLoading	=	FALSE;
					}


				}
				else
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstScptData - skin index ( %d ) of script ( %d ) is bad.\n" , A_SkinIdx , A_Id + E_GrP3ScptPstIdBase );
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstScptData - bad input id ( %d )!\n", A_Id + E_GrP3ScptPstIdBase );
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPstScptData - pst ( %d ) was big!\n", A_Id + E_GrP3ScptPstIdBase );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadAsmData( __u32 A_Size, __u16 A_Id )
{
	// local -------------------
	// code --------------------
		// check data size
		if ( E_GrP3DvlAsmBufMax >= A_Size )
		{
			// check id range
			if ( E_GrP3DvlAsmMaxCnt > A_Id )
			{
				// check already allocated
				if ( NULL == m_DvlScptAsmTbl[A_Id].Pos )
				{
					// allocate assembly buffer
					m_DvlScptAsmTbl[A_Id].Pos	=	(void*)malloc( E_GrP3DvlAsmBufMax );
					if ( NULL != m_DvlScptAsmTbl[A_Id].Pos )
					{
						// read
						if ( A_Size == m_DvlFc->Read( m_DvlScptAsmTbl[A_Id].Pos, A_Size ) )
						{
							// check size missmatch
							if ( A_Size != ( E_GrP3AsmOpUnitSize * m_DvlScptAsmTbl[A_Id].Cnt ) )
							{
								DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmData - warnning - assembly ( %d ) size mismatch!\n", A_Id + E_GrP3AsmIdBase );
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmData - read file error!\n");
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
						DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmData - assembl buffer allocate error.\n");
						m_DvlIsLoading	=	FALSE;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmData - assembly data ( %d ) overrun.\n" , A_Id + E_GrP3AsmIdBase );
					m_DvlIsLoading	=	FALSE;
				}


			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmData - bad input id ( %d )!\n", A_Id + E_GrP3AsmIdBase );
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmData - asm ( %d ) was big!\n", A_Id + E_GrP3AsmIdBase );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadMdlDbg( __u32 A_Size, __u16 A_Id )
{
	// local -------------------
	// code --------------------
		// check size
		if ( sizeof(St_GrP3PrjDbgInfo) >= A_Size )
		{
			// check model index
			if ( E_GrP3DvlModelMaxCnt > A_Id )
			{
				// read
				if ( A_Size != m_DvlFc->Read( &m_DvlMdlDblTbl[A_Id], A_Size ) )
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadMdlDbg - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadMdlDbg - bad model index ( %d )!\n", A_Id );
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadMdlDbg - model debug ( %d ) was big!\n", A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadBldFile( __u32 A_Size, __u16 A_Id )
{
		// check size
		if ( sizeof(St_GrP3PrjBldFile) >= A_Size )
		{
			// check model index
			if ( E_GrP3DvlModelMaxCnt > A_Id )
			{
				// read
				if ( A_Size != m_DvlFc->Read( &m_DvlBldInfo[A_Id], A_Size ) )
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadBldFile - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadBldFile - bad model index ( %d )!\n", A_Id );
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadBldFile - model build file ( %d ) was big!\n", A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadAsmCmt( __u32 A_Size, __u16 A_Id )
{
	// local -------------------
	// code --------------------
		// check size
		if ( E_GrP3DvlAsmRmkBufSize >= A_Size )
		{
			// check model index
			if ( E_GrP3DvlAsmMaxCnt > A_Id )
			{
				// read
				if ( NULL == m_DvlAsmCmt[A_Id].StrCmt )
				{
					m_DvlAsmCmt[A_Id].StrCmt	=	(WCHAR*)malloc( E_GrP3DvlAsmRmkBufSize );
					GrStrClear( m_DvlAsmCmt[A_Id].StrCmt );
				}
				
				if ( NULL != m_DvlAsmCmt[A_Id].StrCmt )
				{
					if ( A_Size != m_DvlFc->Read( m_DvlAsmCmt[A_Id].StrCmt, A_Size ) )
					{
						DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmCmt - read file error!\n");
						m_DvlIsLoading	=	FALSE;
					}
				}
				else
				{
					m_DvlIsLoading	=	FALSE;
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmCmt - memory allocate error ( %d )!\n", A_Id + E_GrP3AsmIdBase );
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmCmt - bad code index ( %d )!\n", A_Id + E_GrP3AsmIdBase );
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadAsmCmt - assembly comment ( %d ) was big!\n", A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadImgData( __u32 A_Size, __u16 A_Id, __u8 A_SkinIdx )
{
	// local -------------------
	// code --------------------
		// check id range
		if ( E_GrP3MngImgScptMaxCnt > A_Id )
		{
			// check skin range
			if ( E_GrP3DvlSkinMaxCnt > A_SkinIdx )
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
								DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadImgData - image ( %d ) size mismatch!\n", A_Id + E_GrP3ImgIdBase );
								m_DvlIsLoading	=	FALSE;
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadImgData - read file error!\n");
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
						DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadImgData - image buffer allocate error.\n");
						m_DvlIsLoading	=	FALSE;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadImgData - image ( %d ) overrun.\n" , A_Id + E_GrP3ImgIdBase );
					m_DvlIsLoading	=	FALSE;
				}


			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadImgData - skin index ( %d ) of script ( %d ) is bad.\n" , A_SkinIdx , A_Id + E_GrP3ImgIdBase );
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadImgData - bad input id ( %d )!\n", A_Id + E_GrP3ImgIdBase );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadFontData( __u32 A_Size, __u16 A_Id, __u8 A_SkinIdx )
{
	// local -------------------
	// code --------------------
		// check id range
		if ( E_GrP3MngFontMaxCnt > A_Id )
		{
			// check skin range
			if ( E_GrP3DvlSkinMaxCnt > A_SkinIdx )
			{
				// check already allocated
				if ( NULL == m_DvlScptFontTbl[A_SkinIdx][A_Id] )
				{
					// allocate script buffer
					m_DvlScptFontTbl[A_SkinIdx][A_Id]	=	(Ptr_GrP3FontHead)malloc( A_Size );
					if ( NULL != m_DvlScptFontTbl[A_SkinIdx][A_Id] )
					{
						// read
						if ( A_Size == m_DvlFc->Read( m_DvlScptFontTbl[A_SkinIdx][A_Id], A_Size ) )
						{
							// check size missmatch
							if ( A_Size !=  GrP3FontCalcSize( m_DvlScptFontTbl[A_SkinIdx][A_Id] ) )
							{
								DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadFontData - font ( %d ) size mismatch!\n", A_Id );
								m_DvlIsLoading	=	FALSE;
							}
						}
						else
						{
							DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadFontData - read file error!\n");
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
						DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadFontData - font buffer allocate error.\n");
						m_DvlIsLoading	=	FALSE;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadFontData - font ( %d ) overrun.\n" , A_Id );
					m_DvlIsLoading	=	FALSE;
				}


			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadFontData - skin index ( %d ) of font ( %d ) is bad.\n" , A_SkinIdx , A_Id );
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadFontData - bad input id ( %d )!\n", A_Id );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadTextData( __u32 A_Size, __u16 A_Id, __u8 A_LngIdx )
{
	// local -------------------
		WCHAR*	Tv_StrWk;
	// code --------------------
		// check size
		if ( E_GrP3DvlTextBufSize >= (A_Size + 2) )
		{
			// check id range
			if ( E_GrP3MngTextScptMaxCnt > A_Id )
			{
				// check language range
				if ( E_GrP3ScptLangMaxCnt > A_LngIdx )
				{
					// check already allocated
					if ( NULL == m_DvlScptTxtTbl[A_Id].LngTbl[A_LngIdx] )
					{
						// allocate script buffer
						m_DvlScptTxtTbl[A_Id].LngTbl[A_LngIdx]	=	(WCHAR*)malloc( E_GrP3DvlTextBufSize );
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
								DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadTextData - read file error!\n");
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
							DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadTextData - text buffer allocate error.\n");
							m_DvlIsLoading	=	FALSE;
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadTextData - text ( %d ) of language ( %d ) overrun.\n" , A_Id + E_GrP3ScptTxtIdBase, A_LngIdx  );
						m_DvlIsLoading	=	FALSE;
					}

				}
				else
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadTextData - language index ( %d ) of text ( %d ) is bad.\n" , A_LngIdx , A_Id + E_GrP3ScptTxtIdBase );
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadTextData - bad input id ( %d )!\n", A_Id + E_GrP3ScptTxtIdBase );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadTextData - text ( %d ) size was big.\n" , A_Id + E_GrP3ScptTxtIdBase );
			m_DvlIsLoading	=	FALSE;
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlPrjLoad( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3DvlMsgPrjLoad	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdPrjLoad;
		Tv_Msg.StrFn				=	A_StrFn;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcPrjLoadOldPrjHead( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_WkIdx;
		Ptr_GrP3PrjOldHead	Tv_PtrHd;
		__u32	Tv_CpSize;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrHd	=	(Ptr_GrP3PrjOldHead)malloc( sizeof(St_GrP3PrjOldHead) );
		if ( NULL != Tv_PtrHd )
		{
			// load
			if ( sizeof(St_GrP3PrjOldHead) == m_DvlFc->Read( Tv_PtrHd, sizeof(St_GrP3PrjOldHead) ) )
			{
				// config copy
				Tv_CpSize	=	sizeof(m_DvlPrjConfig);
				if ( sizeof(St_GrP3PrjOldHead) < Tv_CpSize  )
				{
					Tv_CpSize	=	sizeof(St_GrP3PrjOldHead);
				}
				GrDumyCopyMem( &m_DvlPrjConfig, &Tv_PtrHd->Config, Tv_CpSize );

				// runtime copy
				m_DvlPrjRtl.ModelIdx	=	Tv_PtrHd->Rtl.ModelIdx;
				GrDumyCopyMem( m_DvlPrjRtl.ClrPltTbl, Tv_PtrHd->Rtl.ClrPltTbl, E_GrP3DvlColorPltCnt << 2 );

				// model
				for (Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlModelMaxCnt;Tv_WkIdx++)
				{
					GrStrWcopy( m_DvlPrjModel[Tv_WkIdx].StrName, Tv_PtrHd->Model[Tv_WkIdx].StrName );
					m_DvlPrjModel[Tv_WkIdx].SkinIdx	=	Tv_PtrHd->Model[Tv_WkIdx].SkinIdx;
				}

				// skin
				Tv_CpSize	=	sizeof(St_GrP3PrjSkinCfg);
				if ( sizeof(St_GrP3PrjOldSkinCfg) < Tv_CpSize  )
				{
					Tv_CpSize	=	sizeof(St_GrP3PrjOldSkinCfg);
				}
				for (Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlSkinMaxCnt;Tv_WkIdx++)
				{
					GrDumyCopyMem( &m_DvlPrjSkin[Tv_WkIdx], &Tv_PtrHd->Skin[Tv_WkIdx], Tv_CpSize );
				}

				// model attribute
				Tv_CpSize	=	sizeof(St_GrP3ScptCfgMdl);
				if ( E_GrP3DvlOldScptCfgMdlSize < Tv_CpSize  )
				{
					Tv_CpSize	=	E_GrP3DvlOldScptCfgMdlSize;
				}
				for (Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlModelMaxCnt;Tv_WkIdx++)
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
void	Cls_GrP3DvlMng::LcPrjLoadScptCfgPrj( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlScptCfgPrj) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( &m_DvlScptCfgPrj, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadScptCfgPrj - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadScptCfgPrj - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadScptCfgRtl( __u32 A_Size )
{
		if ( sizeof(m_DvlScptCfgRtl) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( &m_DvlScptCfgRtl, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadScptCfgRtl - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadScptCfgRtl - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadMdlAtb( __u32 A_Size, __u16 A_ModelIdx )
{
	// local -------------------
	// code --------------------
		if ( E_GrP3DvlModelMaxCnt > A_ModelIdx )
		{
			if ( sizeof(St_GrP3ScptCfgMdl) >= A_Size )
			{
				if ( A_Size != m_DvlFc->Read( &m_DvlPrjMdlAtb[A_ModelIdx], A_Size ) )
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadMdlAtb - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadMdlAtb - data size was big!\n");
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadMdlAtb - bad model index ( %d )!\n" , A_ModelIdx);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadPrjConfig( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlPrjConfig) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( &m_DvlPrjConfig, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPrjConfig - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPrjConfig - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadPrjRuntime( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlPrjRtl) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( &m_DvlPrjRtl, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPrjRuntime - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPrjRuntime - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadPrjModel( __u32 A_Size, __u16 A_ModelIdx )
{
	// local -------------------
	// code --------------------
		if ( E_GrP3DvlModelMaxCnt > A_ModelIdx )
		{
			if ( sizeof(St_GrP3PrjModelCfg) >= A_Size )
			{
				if ( A_Size != m_DvlFc->Read( &m_DvlPrjModel[A_ModelIdx], A_Size ) )
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPrjModel - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPrjModel - data size was big!\n");
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPrjModel - bad model index ( %d )!\n" , A_ModelIdx);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadPrjSkin( __u32 A_Size, __u16 A_SkinIdx )
{
	// local -------------------
	// code --------------------
		if ( E_GrP3DvlSkinMaxCnt > A_SkinIdx )
		{
			if ( sizeof(St_GrP3PrjSkinCfg) >= A_Size )
			{
				if ( A_Size != m_DvlFc->Read( &m_DvlPrjSkin[A_SkinIdx], A_Size ) )
				{
					DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPrjSkin - read file error!\n");
					m_DvlIsLoading	=	FALSE;
				}
			}
			else
			{
				m_DvlIsLoading	=	FALSE;
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPrjSkin - data size was big!\n");
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadPrjSkin - bad skin index ( %d )!\n" , A_SkinIdx);
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcDvlPrjLoad( WCHAR* A_StrFn )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3ScptDataHd	Tv_DataHd;
		St_GrP3PrjHdInfo	Tv_PrjHdInfo;
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
			if ( (E_GrP3DvlPrjFcc == Tv_PrjHdInfo.Fcc) && (0 != Tv_PrjHdInfo.Ver) && (E_GrP3DvlPrjVer >= Tv_PrjHdInfo.Ver) )
			{
				// init
				m_DvlIsLoading	=	TRUE;
				// patch version
				if ( 4 == Tv_PrjHdInfo.Ver )
				{
					// old version conversion
					DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlPrjLoad - load old version ( 4 ) header.\n" );
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
							case E_GrP3DvlPrjDataScptHdOld:
								LcPrjLoadOldScptHd( Tv_DataHd.Size );
								break;
							case E_GrP3DvlPrjDataFolderInfo:
								LcPrjLoadFolderTbl( Tv_DataHd.Size );
								break;
							case E_GrP3ScptDataWinScptTbl:
								LcPrjLoadWinScptTbl( Tv_DataHd.Size );
								break;
							case E_GrP3DvlPrjDataWinDvlTbl:
								LcPrjLoadWinDvlTbl( Tv_DataHd.Size );
								break;
							case E_GrP3ScptDataWinScptData:
								LcPrjLoadWinScptData( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP3DvlPrjDataPstScptTblOld:
								LcPrjLoadPstScptTblWhole( Tv_DataHd.Size );
								break;
							case E_GrP3ScptDataPstTbl:
								LcPrjLoadPstScptTblOne( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP3DvlPrjDataPstDvlTbl:
								LcPrjLoadPstDvlTbl( Tv_DataHd.Size );
								break;
							case E_GrP3ScptDataPstScptData:
								LcPrjLoadPstScptData( Tv_DataHd.Size, Tv_DataHd.Id, Tv_DataHd.SubIdx );
								break;
							case E_GrP3ScptDataImageData:
								LcPrjLoadImgData( Tv_DataHd.Size, Tv_DataHd.Id, Tv_DataHd.SubIdx );
								break;
							case E_GrP3ScptDataFontData:
								LcPrjLoadFontData( Tv_DataHd.Size, Tv_DataHd.Id, Tv_DataHd.SubIdx );
								break;
							case E_GrP3ScptDataTextData:
								LcPrjLoadTextData( Tv_DataHd.Size, Tv_DataHd.Id, Tv_DataHd.SubIdx );
								break;
							case E_GrP3DvlPrjDataSetupVar:
								LcPrjLoadSetupVar( Tv_DataHd.Size );
								break;
							case E_GrP3DvlPrjDataSetupSys:
								LcPrjLoadSetupSys( Tv_DataHd.Size );
								break;
							case E_GrP3DvlPrjDataGvu:
								LcPrjLoadGvu( Tv_DataHd.Size );
								break;
							case E_GrP3DvlPrjDataSetBase:
								LcPrjLoadSetBase( Tv_DataHd.Size );
								break;
							case E_GrP3DvlPrjDataAsmInfo:
								LcPrjLoadAsmInfo( Tv_DataHd.Size );
								break;
							case E_GrP3ScptDataAsmTbl:
								LcPrjLoadAsmScptTbl( Tv_DataHd.Size );
								break;
							case E_GrP3ScptDataAsmData:
								LcPrjLoadAsmData( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP3DvlPrjDataMdlDebug:
								LcPrjLoadMdlDbg( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP3ScptDataPageScptTbl:
								LcPrjLoadPageScptTbl( Tv_DataHd.Size );
								break;
							case E_GrP3DvlPrjDataPageDvlTbl:
								LcPrjLoadPageDvlTbl( Tv_DataHd.Size );
								break;
							case E_GrP3ScptDataPageScptData:
								LcPrjLoadPageScptData( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP3DvlPrjDataSdaData:
								LcPrjLoadSdaData( Tv_DataHd.Size );
								break;
							case E_GrP3DvlPrjDataSru:
								LcPrjLoadSru( Tv_DataHd.Size );
								break;
							case E_GrP3DvlPrjDataBldFile:
								LcPrjLoadBldFile( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP3DvlPrjDataAsmCmt:
								LcPrjLoadAsmCmt( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP3ScptDataCfgPrj:
								LcPrjLoadScptCfgPrj( Tv_DataHd.Size );
								break;
							case E_GrP3ScptDataCfgRtl:
								LcPrjLoadScptCfgRtl( Tv_DataHd.Size );
								break;
							case E_GrP3ScptDataMdlAtb:
								LcPrjLoadMdlAtb( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP3DvlPrjConfig:
								LcPrjLoadPrjConfig( Tv_DataHd.Size );
								break;
							case E_GrP3DvlPrjRuntime:
								LcPrjLoadPrjRuntime( Tv_DataHd.Size );
								break;
							case  E_GrP3DvlPrjModel:
								LcPrjLoadPrjModel( Tv_DataHd.Size, Tv_DataHd.Id );
								break;
							case E_GrP3DvlPrjSkin:
								LcPrjLoadPrjSkin( Tv_DataHd.Size, Tv_DataHd.Id );
								break;

							default:
								DbgMsgPrint( "Cls_GrP3DvlMng::DvlPrjLoad - bad data block (%d) found. \n" , Tv_DataHd.Type );
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
				DbgMsgPrint("Cls_GrP3DvlMng::DvlPrjLoad - bad file header.\n");
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
void	Cls_GrP3DvlMng::DvlCursorColorSet( __u32 A_Color )
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
void*	Cls_GrP3DvlMng::DvlSetupUsrTblGet( void )
{
		return	(void*)m_DvlStpUsrTbl;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveSetupVar( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3DvlPrjDataSetupVar;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	m_DvlStpUsrCnt * sizeof(St_GrP3PrjVarInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlStpUsrTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveSru( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3DvlPrjDataSru;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	m_DvlSruCnt * sizeof(St_GrP3PrjVarInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlSruTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveSetupSys( void )
{
	// local -------------------
		//St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		/*
		Tv_DataHd.Type		=	E_GrP3DvlPrjDataSetupSys;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	m_DvlStpSysCnt * sizeof(St_GrP3PrjAdrInfo);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlStpSysTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
		*/
		DbgMsgPrint( "Cls_GrP3DvlMng::LcPrjSaveSetupSys - Error!!! this function must be remove.\n" );
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveGvu( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3DvlPrjDataGvu;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlGvuTbl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlGvuTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjSaveSetBase( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3DvlPrjDataSetBase;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlSetBase);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlSetBase, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadSetupVar( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlStpUsrTbl) >= A_Size )
		{
			if ( A_Size == m_DvlFc->Read( m_DvlStpUsrTbl, A_Size ) )
			{
				m_DvlStpUsrCnt	=	A_Size / sizeof(St_GrP3PrjVarInfo);
			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadSetupVar - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadSetupVar - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadSru( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		if ( sizeof(m_DvlSruTbl) >= A_Size )
		{
			if ( A_Size == m_DvlFc->Read( m_DvlSruTbl, A_Size ) )
			{
				m_DvlSruCnt	=	A_Size / sizeof(St_GrP3PrjVarInfo);
			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadSru - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadSru - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadSetupSys( __u32 A_Size )
{
	// local -------------------
	// code --------------------
		/*
		if ( sizeof(m_DvlStpSysTbl) >= A_Size )
		{
			if ( A_Size == m_DvlFc->Read( m_DvlStpSysTbl, A_Size ) )
			{
				m_DvlStpSysCnt	=	A_Size / sizeof(St_GrP3PrjAdrInfo);
			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadSetupSys - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadSetupSys - data size was big!\n");
		}
		*/

		m_DvlFc->Seek( (__u64)A_Size, E_GrFileCtrlPosNow );

		DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadSetupSys - skip this data.\n");
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadGvu( __u32 A_Size )
{
		if ( sizeof(m_DvlGvuTbl) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( m_DvlGvuTbl, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadGvu - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadGvu - data size was big!\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcPrjLoadSetBase( __u32 A_Size )
{
		if ( sizeof(m_DvlSetBase) >= A_Size )
		{
			if ( A_Size != m_DvlFc->Read( &m_DvlSetBase, A_Size ) )
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadSetBase - read file error!\n");
				m_DvlIsLoading	=	FALSE;
			}
		}
		else
		{
			m_DvlIsLoading	=	FALSE;
			DbgMsgPrint("Cls_GrP3DvlMng::LcPrjLoadSetBase - data size was big!\n");
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DvlMng::DvlSetupUsrCntGet( void )
{
		return	m_DvlStpUsrCnt;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlSetupUsrCntSet( __u32 A_Cnt )
{
		if ( E_GrP3DvlSetupUsrMaxCnt > A_Cnt )
		{
			m_DvlStpUsrCnt	=	A_Cnt;
		}
}
//--------------------------------------------------------------------
void*	Cls_GrP3DvlMng::DvlSruTblGet( void )
{
		return	(void*)m_DvlSruTbl;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DvlMng::DvlSruCntGet( void )
{
		return	m_DvlSruCnt;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlSruCntSet( __u32 A_Cnt )
{
		if ( E_GrP3DvlSruMaxCnt > A_Cnt )
		{
			m_DvlSruCnt	=	A_Cnt;
		}
}
//--------------------------------------------------------------------
void*	Cls_GrP3DvlMng::DvlSetupSysTblGet( void )
{
		return	(void*)V_GrP3DvlStpSysTbl;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DvlMng::DvlSetupSysCntGet( void )
{
		return	E_GrP3DvlSetupSysCnt;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcStpParsSysAdd( __s8* A_StrName, __u32 A_NameLen, __u32 A_Addr, __s8* A_StrDesc, __u32 A_DescLen )
{
	// local -------------------
		//__u32	Tv_Len;
		//Def_StrTag	Tv_StrTmp;
	// code --------------------
		/*
		// check full
		if ( E_GrP3DvlSetupUsrMaxCnt > m_DvlStpSysCnt )
		{
			// get name
			Tv_Len	=	A_NameLen;
			if ( E_GrP3DvlAdrNameLen <= Tv_Len )
			{
				Tv_Len	=	E_GrP3DvlAdrNameLen - 1;
			}
			GrStrCopy( Tv_StrTmp, A_StrName, Tv_Len );
			GrStrStrToWstr( m_DvlStpSysTbl[m_DvlStpSysCnt].StrName, Tv_StrTmp );
			// get address
			m_DvlStpSysTbl[m_DvlStpSysCnt].Pos	=	(__u16)A_Addr;
			// get descriptor
			if ( (NULL != A_StrDesc) && (0 != A_DescLen) )
			{
				Tv_Len	=	A_DescLen;
				if ( E_GrP3DvlAdrDescLen <= Tv_Len )
				{
					Tv_Len	=	E_GrP3DvlAdrDescLen - 1;
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
void	Cls_GrP3DvlMng::LcStpParsGetWordNrm( void )
{
	// local -------------------
		__u8	Tv_Char;
		BOOL8	Tv_IsRun;
	// code --------------------
		// init
		m_DvlStpPsType		=	E_GrP3DvlStpPsTypeReady;
		Tv_IsRun	=	TRUE;

		m_DvlPtrStpWord	=	NULL;
		m_DvlStpWordLen	=	0;

		while( Tv_IsRun && (m_DvlStpPsPrs < m_DvlStpFlSize) )
		{
			Tv_Char	=	m_DvlPtrStpFl[m_DvlStpPsPrs];
			switch ( m_DvlStpPsType )
			{
				case E_GrP3DvlStpPsTypeReady:
					// check type
					if (	(('A' <= Tv_Char) && ('Z' >= Tv_Char)) || (('a' <= Tv_Char) && ('z' >= Tv_Char)) ||
								( '_' == Tv_Char ) )
					{
						// normal text type
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeText;
						m_DvlPtrStpWord	=	&m_DvlPtrStpFl[m_DvlStpPsPrs];
						m_DvlStpWordLen	=	1;
					}
					else if ('0' == Tv_Char)
					{
						// zero number
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeNumZero;
						m_DvlPtrStpWord	=	&m_DvlPtrStpFl[m_DvlStpPsPrs];
						m_DvlStpWordLen	=	1;
					}
					else if ( ('1' <= Tv_Char) && ('9' >= Tv_Char) )
					{
						// decimal number
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeNumDec;
						m_DvlPtrStpWord	=	&m_DvlPtrStpFl[m_DvlStpPsPrs];
						m_DvlStpWordLen	=	1;
					}
					else if ( '#' == Tv_Char )
					{
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeDirect;
						Tv_IsRun	=	FALSE;
						break;
					}
					else if ( '/' == Tv_Char)
					{
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeRemPref;
						m_DvlPtrStpWord	=	&m_DvlPtrStpFl[m_DvlStpPsPrs];
						m_DvlStpWordLen	=	1;
					}
					else if ( 0x0A == Tv_Char )
					{
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeLineEnd;
						Tv_IsRun	=	FALSE;
					}
					else if ( 0x0 == Tv_Char )
					{
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeLineEnd;
						Tv_IsRun	=	FALSE;
						m_DvlIsStpPars	=	FALSE;		// finish
					}
					else if ( 0x20 < Tv_Char )
					{
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeUnknown;
						m_DvlPtrStpWord	=	&m_DvlPtrStpFl[m_DvlStpPsPrs];
						m_DvlStpWordLen	=	1;
					}
					break;
				case E_GrP3DvlStpPsTypeText:
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
				case E_GrP3DvlStpPsTypeNumZero:
					if ( ('x'== Tv_Char) || ('X'== Tv_Char) )
					{
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeNumHpx;
					}
					else if ( ('0' <= Tv_Char) && ('9' >= Tv_Char) )
					{
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeNumDec;
						m_DvlStpWordLen ++;
					}
					else
					{
						// finish
						m_DvlStpPsPrs --;	// back
						Tv_IsRun	=	FALSE;
					}
					break;
				case E_GrP3DvlStpPsTypeNumHpx:
					if (	(('0' <= Tv_Char) && ('9' >= Tv_Char)) || (('A' <= Tv_Char) && ('F' >= Tv_Char)) ||
								(('a' <= Tv_Char) && ('f' >= Tv_Char))	)
					{
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeNumHex;
						// skip prefix
						m_DvlPtrStpWord ++;
						m_DvlPtrStpWord ++;
						m_DvlStpWordLen	=	1;
					}
					else
					{
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeNumDec;
						// finish
						m_DvlStpPsPrs =	m_DvlStpPsPrs - 2;	// back to number
						Tv_IsRun	=	FALSE;
					}
					break;
				case E_GrP3DvlStpPsTypeNumDec:
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
				case E_GrP3DvlStpPsTypeNumHex:
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
				case E_GrP3DvlStpPsTypeRemPref:
					if ( '*' == Tv_Char )
					{
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeRemBlk;
						Tv_IsRun	=	FALSE;
					}
					else if ( '/' == Tv_Char )
					{
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeRemLine;
						Tv_IsRun	=	FALSE;
					}
					else
					{
						m_DvlStpPsType	=	E_GrP3DvlStpPsTypeUnknown;
						m_DvlStpWordLen ++;
					}
					break;
				case E_GrP3DvlStpPsTypeUnknown:
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
		if ( E_GrP3DvlStpPsTypeNumZero == m_DvlStpPsType )
		{
			m_DvlStpPsType	=	E_GrP3DvlStpPsTypeNumDec;
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcStpParsModeReady( void )
{
	// local -------------------
	// code --------------------
		// init
		m_DvlIsStpMult	=	FALSE;
		LcStpParsGetWordNrm();		// get first word
		// check first word type
		switch ( m_DvlStpPsType )
		{
			case E_GrP3DvlStpPsTypeDirect:
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeDefine;	// define wait mode
				break;
			case E_GrP3DvlStpPsTypeLineEnd:
				// keep ready
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	
				break;
			case E_GrP3DvlStpPsTypeRemLine:
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeRemLine;	
				break;
			case E_GrP3DvlStpPsModeRemBlk:
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeRemBlk;	
				break;
			default:
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeLineEnd;	// not need line mode
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcStpParsModeDefine( void )
{
	// local -------------------
		BOOL8	Tv_IsOk;
	// code --------------------
		LcStpParsGetWordNrm();		// get secound word
		switch ( m_DvlStpPsType )
		{
			case E_GrP3DvlStpPsTypeText:
				// check name size
				Tv_IsOk	=	FALSE;
				if ( 6 == m_DvlStpWordLen )
				{
					if ( 0 ==	GrStrCmp( m_DvlPtrStpWord, "define", TRUE, 6 ) )
					{
						Tv_IsOk	=	TRUE;
						m_DvlStpPsMode	=	E_GrP3DvlStpPsModeName;	// reset to ready
					}
				}
				if ( !Tv_IsOk )
				{
					m_DvlStpPsMode	=	E_GrP3DvlStpPsModeLineEnd;	// not need line mode
				}
				break;
			case E_GrP3DvlStpPsTypeLineEnd:
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	// reset to ready
				break;
			default:
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeLineEnd;	// not need line mode
				break;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcStpParsModeName( void )
{
	// local -------------------
		BOOL8	Tv_IsOk;
	// code --------------------
		LcStpParsGetWordNrm();		// get secound word
		switch ( m_DvlStpPsType )
		{
			case E_GrP3DvlStpPsTypeText:
				// check name size
				Tv_IsOk	=	FALSE;
				if ( E_GrP3DvlStpDefNameMin < m_DvlStpWordLen )
				{
					if ( 0 ==	GrStrCmp( m_DvlPtrStpWord, "E_GrP3CfgOfs", TRUE, E_GrP3DvlStpDefNameMin ) )
					{
						Tv_IsOk	=	TRUE;
						m_DvlStpStrName	=	(__s8*)( (__u32)m_DvlPtrStpWord + E_GrP3DvlStpDefNameMin );
						m_DvlStpNameLen	=	m_DvlStpWordLen - E_GrP3DvlStpDefNameMin;
						m_DvlStpPsMode	=	E_GrP3DvlStpPsModeAddr;	
					}
				}
				if ( !Tv_IsOk )
				{
					m_DvlStpPsMode	=	E_GrP3DvlStpPsModeLineEnd;	// not need line mode
				}
				break;
			case E_GrP3DvlStpPsTypeLineEnd:
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	// reset to ready
				break;
			default:
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeLineEnd;	// not need line mode
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcStpParsModeAddr( void )
{
	// local -------------------
	// code --------------------
		LcStpParsGetWordNrm();		// get secound word
		switch ( m_DvlStpPsType )
		{
			case E_GrP3DvlStpPsTypeNumDec:
				if ( GrStrStrToInt( m_DvlPtrStpWord, (__s32*)&m_DvlStpAdr, m_DvlStpWordLen ) )
				{
					m_DvlStpPsMode	=	E_GrP3DvlStpPsModeDescChk;	
				}
				break;
			case E_GrP3DvlStpPsTypeNumHex:
				if ( GrStrHexStrToDword( m_DvlPtrStpWord, &m_DvlStpAdr, m_DvlStpWordLen ) )
				{
					m_DvlStpPsMode	=	E_GrP3DvlStpPsModeDescChk;	
				}
				break;
			case E_GrP3DvlStpPsTypeLineEnd:
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	// reset to ready
				break;
			default:
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeLineEnd;	// not need line mode
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcStpParsModeDescChk( void )
{
	// local -------------------
		BOOL8	Tv_IsUpdtVal;
	// code --------------------
		Tv_IsUpdtVal	=	FALSE;
		LcStpParsGetWordNrm();		// get secound word
		switch ( m_DvlStpPsType )
		{
			case	E_GrP3DvlStpPsTypeRemLine:
				m_DvlStpStrDesc	=	(__s8*)&m_DvlPtrStpFl[m_DvlStpPsPrs];
				m_DvlStpDescLen	=	0;
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeDescLine;	
				break;
			case E_GrP3DvlStpPsTypeRemBlk:
				m_DvlStpStrDesc	=	(__s8*)&m_DvlPtrStpFl[m_DvlStpPsPrs];
				m_DvlStpDescLen	=	0;
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeDescBlk;	
				m_DvlIsStpMult	=	FALSE;
				break;
			case E_GrP3DvlStpPsTypeLineEnd:
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	// reset to ready
				Tv_IsUpdtVal		=	TRUE;
				break;
			default:
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeLineEnd;	
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
void	Cls_GrP3DvlMng::LcStpParsModeDescLine( void )
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
			m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	
			Tv_IsUpdtVal		=	TRUE;
			m_DvlStpPsPrs ++;
		}
		else if ( 0x0A == Tv_Char )
		{
			m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;
			Tv_IsUpdtVal		=	TRUE;
			m_DvlStpPsPrs ++;
		}
		else if ( 0x00 == Tv_Char )
		{
			m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	
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
void	Cls_GrP3DvlMng::LcStpParsModeDescBlk( void )
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
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	
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
			m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	
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
void	Cls_GrP3DvlMng::LcStpParsModeRemLine( void )
{
	// local -------------------
		__s8	Tv_Char;
	// code --------------------
		// get one __s8
		Tv_Char	=	m_DvlPtrStpFl[m_DvlStpPsPrs];
		if ( (0x0D == Tv_Char) || (0x0A == Tv_Char) )
		{
			m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	
			m_DvlStpPsPrs ++;
		}
		else if ( 0x00 == Tv_Char )
		{
			m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	
			m_DvlIsStpPars	=	FALSE;	// parsing finish
		}
		else
		{
			m_DvlStpPsPrs ++;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcStpParsModeRemBlk( void )
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
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	
			}
			m_DvlIsStpMult	=	FALSE;
			m_DvlStpPsPrs ++;
		}
		else if ( 0x00 == Tv_Char )
		{
			m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	
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
void	Cls_GrP3DvlMng::LcStpParsModeEndLine( void )
{
	// local -------------------
		__s8	Tv_Char;
	// code --------------------
		// get one __s8
		Tv_Char	=	m_DvlPtrStpFl[m_DvlStpPsPrs];
		if ( (0x0D == Tv_Char) || ( 0x0A == Tv_Char ) )
		{
			m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	
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
				m_DvlStpPsMode	=	E_GrP3DvlStpPsModeRemBlk;	
			}
			m_DvlIsStpMult	=	FALSE;
			m_DvlStpPsPrs ++;
		}
		else if ( 0x00 == Tv_Char )
		{
			m_DvlStpPsMode	=	E_GrP3DvlStpPsModeReady;	
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
BOOL8	Cls_GrP3DvlMng::DvlWindowDel( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3DvlMsgWindowDel	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdWindowDel;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcDvlWindowDel( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result	=	FALSE;
		// check script id range
		if ( (E_GrP3ScptIdWinBase <= A_ScptId) && ((E_GrP3ScptIdWinBase + E_GrP3DvlWinMaxCnt) > A_ScptId ) )
		{
			Tv_Idx	=	A_ScptId - E_GrP3ScptIdWinBase;
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
				m_DvlWinInfo[Tv_Idx].FoldId			=	E_GrP3DvlFolderIdRoot;
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
BOOL8	Cls_GrP3DvlMng::DvlPageDel( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3DvlMsgPageDel	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdPageDel;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcDvlPageDel( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_Idx;
	// code --------------------
		Tv_Result	=	FALSE;
		// check script id range
		if ( (E_GrP3ScptIdPageBase <= A_ScptId) && ((E_GrP3ScptIdPageBase + E_GrP3DvlPageMaxCnt) > A_ScptId ) )
		{
			Tv_Idx	=	A_ScptId - E_GrP3ScptIdPageBase;
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
				m_DvlScptPageTbl[Tv_Idx].AsmOpen	=	E_GrP3AsmIdNone;
				m_DvlScptPageTbl[Tv_Idx].AsmClose	=	E_GrP3AsmIdNone;
				m_DvlPageInfo[Tv_Idx].FoldId			=	E_GrP3DvlFolderIdRoot;
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
__u16	Cls_GrP3DvlMng::DvlWinFindByPrntFolder( __u16 A_FolderId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlWinMaxCnt;Tv_WkIdx++ )
		{
			// check exist
			if ( NULL != m_DvlScptWinTbl[Tv_WkIdx].Pos )
			{
				// check parent folder id
				if ( A_FolderId == m_DvlWinInfo[Tv_WkIdx].FoldId )
				{
					// found
					Tv_Result	=	E_GrP3ScptIdWinBase + Tv_WkIdx;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlPageFindByPrntFolder( __u16 A_FolderId )
{
	// local -------------------
		__u16	Tv_Result;
		__u16	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlPageMaxCnt;Tv_WkIdx++ )
		{
			// check exist
			if ( NULL != m_DvlScptPageTbl[Tv_WkIdx].Pos )
			{
				// check parent folder id
				if ( A_FolderId == m_DvlPageInfo[Tv_WkIdx].FoldId )
				{
					// found
					Tv_Result	=	E_GrP3ScptIdPageBase + Tv_WkIdx;
					break;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	Cls_GrP3DvlMng::DvlGvuInfoTblGet( void )
{
		return	(void*)m_DvlGvuTbl;
}
//--------------------------------------------------------------------
void*	Cls_GrP3DvlMng::DvlAsmCreate( __u16 A_Id )
{
	// local -------------------
		void*	Tv_Result;
		__u16	Tv_AsmIdx;
	// code --------------------
		Tv_Result	=	NULL;
		// check id
		if ( (E_GrP3AsmIdBase <= A_Id) && ((E_GrP3AsmIdBase + E_GrP3DvlAsmMaxCnt) > A_Id) )
		{
			// get index
			Tv_AsmIdx	=	A_Id - E_GrP3AsmIdBase;
			// check exist
			if ( NULL == m_DvlScptAsmTbl[Tv_AsmIdx].Pos )
			{
				// allocate buffer
				m_DvlScptAsmTbl[Tv_AsmIdx].Pos	=	(void*)malloc( E_GrP3DvlAsmBufMax );
				if ( NULL != m_DvlScptAsmTbl[Tv_AsmIdx].Pos )
				{
					m_DvlScptAsmTbl[Tv_AsmIdx].Cnt	=	0;		// count reset
					// GrStrClear( m_DvlAsmInfo[Tv_AsmIdx].StrName );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3DvlMng::DvlAsmCreate - assembly code ( %d ) allocate failure.\n" , A_Id );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3DvlMng::DvlAsmCreate - assembly code ( %d ) already exist.\n" , A_Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DvlMng::DvlAsmCreate - bad assembly code id ( %d ).\n" , A_Id );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlAsmCodeCountSet( __u16 A_Id, __u16 A_Cnt )
{
	// local -------------------
		__u16	Tv_AsmIdx;
	// code --------------------
		// check count over
		if ( E_GrP3DvlAsmCodeMax >= A_Cnt )
		{
			// check id
			if ( (E_GrP3AsmIdBase <= A_Id) && ((E_GrP3AsmIdBase + E_GrP3DvlAsmMaxCnt) > A_Id) )
			{
				// get index
				Tv_AsmIdx	=	A_Id - E_GrP3AsmIdBase;
				// check exist
				if ( NULL != m_DvlScptAsmTbl[Tv_AsmIdx].Pos )
				{
					// set
					m_DvlScptAsmTbl[Tv_AsmIdx].Cnt	=	A_Cnt;
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3DvlMng::DvlAsmCodeCountSet - assembly code ( %d ) not exist.\n" , A_Id );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3DvlMng::DvlAsmCodeCountSet - bad assembly code id ( %d ).\n" , A_Id );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DvlMng::DvlAsmCodeCountSet - try over count ( %d ) set to code id ( %d ).\n" , A_Cnt, A_Id );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlAsmFuncNameGet( __u16 A_Id )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16	Tv_AsmIdx;
	// code --------------------
		Tv_Result	=	NULL;
		if ( (E_GrP3AsmIdBase <= A_Id) && ((E_GrP3AsmIdBase + E_GrP3DvlAsmMaxCnt) > A_Id) )
		{
			// get index
			Tv_AsmIdx	=	A_Id - E_GrP3AsmIdBase;
			// set
			Tv_Result	=	m_DvlAsmInfo[Tv_AsmIdx].StrName;
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DvlMng::DvlAsmCodeNameGet - bad assembly code id ( %d ).\n" , A_Id );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlAsmFuncNameSet( __u16 A_Id, WCHAR* A_StrName )
{
	// local -------------------
		__u16	Tv_AsmIdx;
	// code --------------------
		if ( (E_GrP3AsmIdBase <= A_Id) && ((E_GrP3AsmIdBase + E_GrP3DvlAsmMaxCnt) > A_Id) )
		{
			// get index
			Tv_AsmIdx	=	A_Id - E_GrP3AsmIdBase;
			// set
			GrStrWcopy( m_DvlAsmInfo[Tv_AsmIdx].StrName, A_StrName, 31 );
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DvlMng::DvlAsmCodeNameSet - bad assembly code id ( %d ).\n" , A_Id );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlAsmFuncCmtGet( __u16 A_Id )
{
	// local -------------------
		WCHAR*	Tv_Result;
		__u16	Tv_AsmIdx;
	// code --------------------
		Tv_Result	=	NULL;
		if ( (E_GrP3AsmIdBase <= A_Id) && ((E_GrP3AsmIdBase + E_GrP3DvlAsmMaxCnt) > A_Id) )
		{
			// get index
			Tv_AsmIdx	=	A_Id - E_GrP3AsmIdBase;
			// set
			Tv_Result	=	m_DvlAsmCmt[Tv_AsmIdx].StrCmt;
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DvlMng::DvlAsmFuncCmtGet - bad assembly code id ( %d ).\n" , A_Id );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlAsmFuncCmtSet( __u16 A_Id, WCHAR* A_StrCmt )
{
	// local -------------------
		__u16	Tv_AsmIdx;
	// code --------------------
		if ( (E_GrP3AsmIdBase <= A_Id) && ((E_GrP3AsmIdBase + E_GrP3DvlAsmMaxCnt) > A_Id) )
		{
			// get index
			Tv_AsmIdx	=	A_Id - E_GrP3AsmIdBase;

			// check exist
			if ( NULL == m_DvlAsmCmt[Tv_AsmIdx].StrCmt )
			{
				m_DvlAsmCmt[Tv_AsmIdx].StrCmt	=	(WCHAR*)malloc( E_GrP3DvlAsmRmkBufSize );
				GrStrClear( m_DvlAsmCmt[Tv_AsmIdx].StrCmt );
			}
			if ( NULL != m_DvlAsmCmt[Tv_AsmIdx].StrCmt )
			{
				// set
				GrStrWcopy( m_DvlAsmCmt[Tv_AsmIdx].StrCmt, A_StrCmt, 2047 );
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3DvlMng::DvlAsmFuncCmtSet - memory allocate fail!\n" );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DvlMng::DvlAsmFuncCmtSet - bad assembly code id ( %d ).\n" , A_Id );
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlPathImageGet( void )
{
		return	m_DvlSetBase.StrPathImg;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlPathFontGet( void )
{
		return	m_DvlSetBase.StrPathFont;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlPathBldSrGet( void )
{
	// local -------------------
	// code --------------------
		return	m_DvlSetBase.StrPathBldSr;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlPathBldTgGet( void )
{
		return	m_DvlSetBase.StrPathBldTg;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlPathHexpGet( void )
{
		return	m_DvlSetBase.StrPathHexp;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlPathTextGet( void )
{
		return	m_DvlSetBase.StrPathText;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrP3DvlMng::DvlPathScriptGet( void )
{
		return	m_DvlSetBase.StrPathScpt;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcDbgGvsInit( void )
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
void	Cls_GrP3DvlMng::LcDbgConInit( void )
{
	// local -------------------
		__u8	Tv_ConIdx;
	// code --------------------
		for ( Tv_ConIdx=0;Tv_ConIdx < E_GrP3MngConMaxCnt;Tv_ConIdx++ )
		{
			if ( NULL != m_ConTbl[Tv_ConIdx] )
			{
				m_ConTbl[Tv_ConIdx]->DvlDebugPrepare();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlDbgStart( void )
{
	// local -------------------
		St_GrP3DvlMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Cmd				=	E_GrP3DvlMcmdDbgStart;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcDvlDbgStart( void )
{
	// local -------------------
	// code --------------------
		// release edit window
		LcEditObjFree();
		DvlEditModelIdxSet( m_DvlPrjRtl.ModelIdx );
		LcDbgGvsInit();
		LcDbgConInit();
		// setup assign
		SysBuildEnv();
		SysInit();

		m_DvlIsDbgRun	=	TRUE;
		m_DvlDbgMode	=	E_GrP3DvlDbgModeRun;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlDbgFinish( void )
{
	// local -------------------
		St_GrP3DvlMsgBase	Tv_Msg;
	// code --------------------
		m_DvlIsDbgRun	=	FALSE;
		m_DvlDbgMode	=	E_GrP3DvlDbgModeRun;

		Tv_Msg.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Cmd				=	E_GrP3DvlMcmdDbgFinish;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcDvlDbgFinish( void )
{
	// local -------------------
	// code --------------------
		m_DvlIsDbgRun	=	FALSE;
		m_DvlDbgMode	=	E_GrP3DvlDbgModeRun;
		LcWinAllFree();
		m_VmIdx		=	0;		// virtual machine reset
		GrDumyZeroMem( m_VmTbl, sizeof(m_VmTbl) );
		
		m_IsDvlBkupRun	=	FALSE;

		m_SysStat	=	E_GrP3MngSysStatNone;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
		St_GrTaskMsgFwUpgdEnd	Tv_MsgFwUpEnd;
	// code --------------------
		Cls_GrP3Mng::RtlTimer( A_Hndl );

		// debug
		if ( m_DvlIsDbgRun )
		{
			// do system status
			switch ( m_SysStat )
			{
				case E_GrP3MngSysStatInit:
					LcTimerEmuInit();
					break;
				case E_GrP3MngSysStatFwUpgd:
					if ( 100 <= m_Gvs.PrgsFwUp )
					{
						// finish
						m_SysStat	=	E_GrP3MngSysStatStart;
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
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_DvlMsgAloc == A_PtrMsg->Cmd )
		{
			switch ( ((Ptr_GrP3DvlMsgBase)A_PtrMsg)->Cmd )
			{
				case E_GrP3DvlMcmdPrjNew:
					*((Ptr_GrP3DvlMsgPrjNew)A_PtrMsg)->PtrRslt	=	LcDvlPrjNew( ((Ptr_GrP3DvlMsgPrjNew)A_PtrMsg)->FccPx,
						((Ptr_GrP3DvlMsgPrjNew)A_PtrMsg)->Res1X, ((Ptr_GrP3DvlMsgPrjNew)A_PtrMsg)->Res1Y,
						((Ptr_GrP3DvlMsgPrjNew)A_PtrMsg)->Res2X, ((Ptr_GrP3DvlMsgPrjNew)A_PtrMsg)->Res2Y,
						((Ptr_GrP3DvlMsgPrjNew)A_PtrMsg)->IsDual, ((Ptr_GrP3DvlMsgPrjNew)A_PtrMsg)->IsInputDual );
					break;
				case E_GrP3DvlMcmdPrjClose:
					LcDvlPrjClose();
					break;
				case E_GrP3DvlMcmdPrjSave:
					*((Ptr_GrP3DvlMsgPrjSave)A_PtrMsg)->PtrRslt	=	LcDvlPrjSave( ((Ptr_GrP3DvlMsgPrjSave)A_PtrMsg)->StrFn );
					break;
				case E_GrP3DvlMcmdPrjLoad:
					*((Ptr_GrP3DvlMsgPrjLoad)A_PtrMsg)->PtrRslt	=	LcDvlPrjLoad( ((Ptr_GrP3DvlMsgPrjLoad)A_PtrMsg)->StrFn );
					break;
				case E_GrP3DvlMcmdScptExport:
					*((Ptr_GrP3DvlMsgScptExport)A_PtrMsg)->PtrRslt	=	LcDvlScptExport( ((Ptr_GrP3DvlMsgScptExport)A_PtrMsg)->StrFn,
						((Ptr_GrP3DvlMsgScptExport)A_PtrMsg)->Model );
					break;
				case E_GrP3DvlMcmdWindowNew:
					*((Ptr_GrP3DvlMsgWindowNew)A_PtrMsg)->PtrRslt	=	LcDvlWindowNew();
					break;
				case E_GrP3DvlMcmdPageNew:
					*((Ptr_GrP3DvlMsgPageNew)A_PtrMsg)->PtrRslt	=	LcDvlPageNew();
					break;
				case E_GrP3DvlMcmdWindowDel:
					*((Ptr_GrP3DvlMsgWindowDel)A_PtrMsg)->PtrRslt	=	LcDvlWindowDel( ((Ptr_GrP3DvlMsgWindowDel)A_PtrMsg)->ScptId );
					break;
				case E_GrP3DvlMcmdPageDel:
					*((Ptr_GrP3DvlMsgPageDel)A_PtrMsg)->PtrRslt	=	LcDvlPageDel( ((Ptr_GrP3DvlMsgPageDel)A_PtrMsg)->ScptId );
					break;
				case E_GrP3DvlMcmdScriptDec:
					*((Ptr_GrP3DvlMsgScriptDec)A_PtrMsg)->PtrRslt	=	LcDvlScriptDec( ((Ptr_GrP3DvlMsgScriptDec)A_PtrMsg)->PrntId,
						((Ptr_GrP3DvlMsgScriptDec)A_PtrMsg)->PtrScpt );
					break;
				case E_GrP3DvlMcmdEditSkinSet:
					LcDvlEditSkinSet( ((Ptr_GrP3DvlMsgEditSkinSet)A_PtrMsg)->SkinIdx );
					break;
				case E_GrP3DvlMcmdEditModelIdxSet:
					LcDvlEditModelIdxSet( ((Ptr_GrP3DvlMsgEditModelIdxSet)A_PtrMsg)->ModelIdx );
					break;
				case E_GrP3DvlMcmdEditObjCreate:
					*((Ptr_GrP3DvlMsgEditObjCreate)A_PtrMsg)->PtrRslt	=	LcDvlEditObjCreate( ((Ptr_GrP3DvlMsgEditObjCreate)A_PtrMsg)->ScrX,
						((Ptr_GrP3DvlMsgEditObjCreate)A_PtrMsg)->ScrY, ((Ptr_GrP3DvlMsgEditObjCreate)A_PtrMsg)->ClassId );
					break;
				case E_GrP3DvlMcmdEdtLumpScptUpdt:
					LcDvlEdtLumpScptUpdt();
					break;
				case E_GrP3DvlMcmdEditLumpGet:
					*((Ptr_GrP3DvlMsgEditLumpGet)A_PtrMsg)->PtrRslt	=	LcDvlEditLumpGet();
					break;
				case E_GrP3DvlMcmdEditLumpSet:
					*((Ptr_GrP3DvlMsgEditLumpSet)A_PtrMsg)->PtrRslt	=	LcDvlEditLumpSet( ((Ptr_GrP3DvlMsgEditLumpSet)A_PtrMsg)->ScptId );
					break;
				case E_GrP3DvlMcmdEditObjScrRectGet:
					*((Ptr_GrP3DvlMsgEditObjScrRectGet)A_PtrMsg)->PtrRslt	=	LcDvlEditObjScrRectGet( ((Ptr_GrP3DvlMsgEditObjScrRectGet)A_PtrMsg)->ScptId,
						((Ptr_GrP3DvlMsgEditObjScrRectGet)A_PtrMsg)->PtrRtRect, ((Ptr_GrP3DvlMsgEditObjScrRectGet)A_PtrMsg)->IsNoChk );
					break;
				case E_GrP3DvlMcmdEditObjRectSet:
					*((Ptr_GrP3DvlMsgEditObjRectSet)A_PtrMsg)->PtrRslt	=	LcDvlEditObjRectSet( ((Ptr_GrP3DvlMsgEditObjRectSet)A_PtrMsg)->ScptId,
						((Ptr_GrP3DvlMsgEditObjRectSet)A_PtrMsg)->PtrRect );
					break;
				case E_GrP3DvlMcmdEditObjRectGet:
					*((Ptr_GrP3DvlMsgEditObjRectGet)A_PtrMsg)->PtrRslt	=	LcDvlEditObjRectGet( ((Ptr_GrP3DvlMsgEditObjRectGet)A_PtrMsg)->ScptId,
						((Ptr_GrP3DvlMsgEditObjRectGet)A_PtrMsg)->PtrRtRect );
					break;
				case E_GrP3DvlMcmdEditObjFindByPos:
					*((Ptr_GrP3DvlMsgEditObjFindByPos)A_PtrMsg)->PtrRslt	=	LcDvlEditObjFindByPos( ((Ptr_GrP3DvlMsgEditObjFindByPos)A_PtrMsg)->PosX,
						((Ptr_GrP3DvlMsgEditObjFindByPos)A_PtrMsg)->PosY );
					break;
				case E_GrP3DvlMcmdEditObjClassIdGet:
					*((Ptr_GrP3DvlMsgEditObjClassIdGet)A_PtrMsg)->PtrRslt	=	LcDvlEditObjClassIdGet( ((Ptr_GrP3DvlMsgEditObjClassIdGet)A_PtrMsg)->ScptId );
					break;
				case E_GrP3DvlMcmdEditObjScptDump:
					*((Ptr_GrP3DvlMsgEditObjScptDump)A_PtrMsg)->PtrRslt	=	LcDvlEditObjScptDump( ((Ptr_GrP3DvlMsgEditObjScptDump)A_PtrMsg)->ScptId,
						((Ptr_GrP3DvlMsgEditObjScptDump)A_PtrMsg)->PtrBuf, ((Ptr_GrP3DvlMsgEditObjScptDump)A_PtrMsg)->IsCopyMode );
					break;
				case E_GrP3DvlMcmdEditObjFrameGet:
					*((Ptr_GrP3DvlMsgEditObjFrameGet)A_PtrMsg)->PtrRslt	=	LcDvlEditObjFrameGet( ((Ptr_GrP3DvlMsgEditObjFrameGet)A_PtrMsg)->ScptId );
					break;
				case E_GrP3DvlMcmdEditObjRedraw:
					LcDvlEditObjRedraw( ((Ptr_GrP3DvlMsgEditObjRedraw)A_PtrMsg)->ScptId );
					break;
				case E_GrP3DvlMcmdEditObjListByPos:
					*((Ptr_GrP3DvlMsgEditObjListByPos)A_PtrMsg)->PtrRslt	=	LcDvlEditObjListByPos( ((Ptr_GrP3DvlMsgEditObjListByPos)A_PtrMsg)->X,
						((Ptr_GrP3DvlMsgEditObjListByPos)A_PtrMsg)->Y, ((Ptr_GrP3DvlMsgEditObjListByPos)A_PtrMsg)->PtrBuf );
					break;
				case E_GrP3DvlMcmdEditObjSlaveObjGet:
					*((Ptr_GrP3DvlMsgEditObjSlaveObjGet)A_PtrMsg)->PtrRslt	=	LcDvlEditObjSlaveObjGet( ((Ptr_GrP3DvlMsgEditObjSlaveObjGet)A_PtrMsg)->ScptId );
					break;
				case E_GrP3DvlMcmdEditObjNextObjGet:
					*((Ptr_GrP3DvlMsgEditObjNextObjGet)A_PtrMsg)->PtrRslt	=	LcDvlEditObjNextObjGet( ((Ptr_GrP3DvlMsgEditObjNextObjGet)A_PtrMsg)->ScptId );
					break;
				case E_GrP3DvlMcmdEditObjPrntObjGet:
					*((Ptr_GrP3DvlMsgEditObjPrntObjGet)A_PtrMsg)->PtrRslt	=	LcDvlEditObjPrntObjGet( ((Ptr_GrP3DvlMsgEditObjPrntObjGet)A_PtrMsg)->ScptId );
					break;
				case E_GrP3DvlMcmdEditObjIsWindow:
					*((Ptr_GrP3DvlMsgEditObjIsWindow)A_PtrMsg)->PtrRslt	=	LcDvlEditObjIsWindow( ((Ptr_GrP3DvlMsgEditObjIsWindow)A_PtrMsg)->ScptId );
					break;
				case E_GrP3DvlMcmdEditObjDelete:
					LcDvlEditObjDelete( ((Ptr_GrP3DvlMsgEditObjDelete)A_PtrMsg)->ScptId );
					break;
				case E_GrP3DvlMcmdEditObjParaInfoDump:
					*((Ptr_GrP3DvlMsgEditObjParaInfoDump)A_PtrMsg)->PtrRslt	=	LcDvlEditObjParaInfoDump( ((Ptr_GrP3DvlMsgEditObjParaInfoDump)A_PtrMsg)->ScptId,
						((Ptr_GrP3DvlMsgEditObjParaInfoDump)A_PtrMsg)->PtrBuf );
					break;
				case E_GrP3DvlMcmdEditObjParaUpdt:
					LcDvlEditObjScptParaUpdt( ((Ptr_GrP3DvlMsgEditObjParaUpdt)A_PtrMsg)->ScptId, 
						((Ptr_GrP3DvlMsgEditObjParaUpdt)A_PtrMsg)->PtrScpt, ((Ptr_GrP3DvlMsgEditObjParaUpdt)A_PtrMsg)->IsNoCopy );
					break;
				case E_GrP3DvlMcmdDbgStart:
					LcDvlDbgStart();
					break;
				case E_GrP3DvlMcmdDbgFinish:
					LcDvlDbgFinish();
					break;
				case E_GrP3DvlMcmdIsDbgPause:
					*((Ptr_GrP3DvlMsgIsDbgPause)A_PtrMsg)->PtrRslt	=	LcDvlIsDbgPause();
					break;
				case E_GrP3DvlMcmdDbgVmCsGet:
					*((Ptr_GrP3DvlMsgDbgVmCsGet)A_PtrMsg)->PtrRslt	=	LcDvlDbgVmCsGet();
					break;
				case E_GrP3DvlMcmdDbgVmIpGet:
					*((Ptr_GrP3DvlMsgDbgVmIpGet)A_PtrMsg)->PtrRslt	=	LcDvlDbgVmIpGet();
					break;
				case E_GrP3DvlMcmdEditObjInfoT1:
					*((Ptr_GrP3DvlMsgEditObjInfoT1)A_PtrMsg)->PtrRslt	=	LcDvlEditObjInfoT1( ((Ptr_GrP3DvlMsgEditObjInfoT1)A_PtrMsg)->ScptId,
						((Ptr_GrP3DvlMsgEditObjInfoT1)A_PtrMsg)->PtrRtClassType, ((Ptr_GrP3DvlMsgEditObjInfoT1)A_PtrMsg)->PtrRtRect,
						((Ptr_GrP3DvlMsgEditObjInfoT1)A_PtrMsg)->PtrRtSlaveId, ((Ptr_GrP3DvlMsgEditObjInfoT1)A_PtrMsg)->PtrRtNextId );
					break;


				default:
					DbgMsgPrint( "Cls_GrP3DvlMng::RtlMsgProc - bad devlop message ( %d ).\n", ((Ptr_GrP3DvlMsgBase)A_PtrMsg)->Cmd );
					break;
			}
			Tv_Result	=	TRUE;
		}
		else
		{
			Tv_Result	=	Cls_GrP3Mng::RtlMsgProc( A_PtrMsg );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcTimerEmuInit( void )
{
	// local -------------------
	// code --------------------
		m_Gvs.PrgsInit	=	m_Gvs.PrgsInit + 5;
		if ( 100 <= m_Gvs.PrgsInit )
		{
			// next mode
			SysStart();
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlIsDbgRun( void )
{
		return	m_DvlIsDbgRun;		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::VmRunCode( __u16 A_CodeId, Cls_GrP3DoBase* A_ObjHost, Ptr_GrP3MngVmRpara A_PtrRpara )
{
	// local -------------------
		BOOL8	Tv_Result;		
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_DvlIsDbgRun )
		{
			Tv_Result	=	Cls_GrP3Mng::VmRunCode( A_CodeId, A_ObjHost, A_PtrRpara );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DvlMng::LcDvlUiDecPage( __u16 A_ScptId )
{
	// local -------------------
		__u32	Tv_Result;
		__u16	Tv_SlotIdx;
		Cls_GrP3DoWin*	Tv_Win;
		Ptr_GrP3ScptObjHd	Tv_PtrScpt;
		St_GrRect	Tv_RcObj;
	// code --------------------
		Tv_Result	=	0;
		if ( E_GrP3ScptIdPageBase <= A_ScptId )
		{
			Tv_SlotIdx	=	A_ScptId - E_GrP3ScptIdPageBase;
			if ( m_PageCnt > Tv_SlotIdx )
			{
				// get script
				if ( NULL != m_PtrPageTbl[Tv_SlotIdx].Pos )
				{
					// script decode
					Tv_PtrScpt	=	(Ptr_GrP3ScptObjHd)m_PtrPageTbl[Tv_SlotIdx].Pos;
					// check script head
					if ( E_GrP3ScptObjStx == Tv_PtrScpt->Stx )
					{
						// check first page class type
						if ( E_GrP3DoClassWin == Tv_PtrScpt->ClassId )
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
									Tv_Win	=	(Cls_GrP3DoWin*)new Cls_GrP3DoWin( A_ScptId, &Tv_RcObj, 0,
										m_ConTbl[0] );
									// decode other option
									Tv_Result	=	LcUiDecSlave( (Cls_GrP3DoBase*)Tv_Win, Tv_PtrScpt, TRUE );
									// call init
									Tv_Win->RtlInit();
									// request focus change
									m_ConTbl[0]->FocusReset();
									m_ConTbl[0]->FocusCorrect();
									DbgMsgPrint("Page ( %d ) decoded! size = %d byte.\n" , A_ScptId, Tv_Result );

								}
								else
								{
									DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlUiDecPage - page ( %d ) already exist!\n", A_ScptId );
								}
							}
							else
							{
								DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlUiDecPage - buffer script id  ( %d )  is mismatch orignal id ( %d ) !\n", Tv_PtrScpt->ScptId , A_ScptId  );
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlUiDecPage - bad script ( %d )  - class type ( %d ) missmatch!\n" , A_ScptId, Tv_PtrScpt->ClassId );
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlUiDecPage - script ( %d ) error! : bad stx.\n" , A_ScptId );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlUiDecPage - script ( %d ) not exist .\n" , A_ScptId );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlUiDecPage - page Id ( %d ) is out of range.\n" , A_ScptId );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlVkbdTxtPageCntSet( __u8 A_Lang , __u8 A_Cnt )
{
	// local -------------------
	// code --------------------
		m_PtrCfgPrj->VkbdTbl[A_Lang].PageCnt	=	A_Cnt;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlVkbdTxtKeyCodeSet( __u8 A_Page, __u8 A_Key, __u8 A_Lang, WCHAR A_KeyCode )
{
	// local -------------------
	// code --------------------
		m_PtrCfgPrj->VkbdTbl[A_Lang].KeyTbl[A_Page][A_Key]	=	A_KeyCode;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DvlMng::DvlScriptDec( __u16 A_PrntId, void* A_PtrScpt )
{
	// local -------------------
		__u32	Tv_Result;
		St_GrP3DvlMsgScriptDec	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdScriptDec;
		Tv_Msg.PrntId				=	A_PrntId;
		Tv_Msg.PtrScpt			=	A_PtrScpt;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DvlMng::LcDvlScriptDec( __u16 A_PrntId, void* A_PtrScpt )
{
	// local -------------------
		__u32	Tv_Result;
		Ptr_GrP3ScptObjHd	Tv_PtrHd;
		BOOL8	Tv_IsWithPrnt;
		Cls_GrP3DoBase*	Tv_ObjPrnt;
	// code --------------------
		// init
		Tv_Result			=	0;
		Tv_IsWithPrnt	=	FALSE;
		Tv_PtrHd			=	(Ptr_GrP3ScptObjHd)A_PtrScpt;

		// get parent object
		if ( NULL != m_DvlEdtLump )
		{
			Tv_ObjPrnt	=	m_DvlEdtLump->FindObjByScptId( A_PrntId );
			if ( NULL != Tv_ObjPrnt )
			{
				if ( E_GrP3DoClassWin == Tv_PtrHd->ClassId )
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
void	Cls_GrP3DvlMng::LcSlaveIdMarkByScpt( void* A_PtrScpt )
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
			if ( E_GrP3ScptObjStx == *Tv_PtrScpt )
			{
				Tv_Depth ++;
				// get script id
				Tv_ScptId	=	((Ptr_GrP3ScptObjHd)Tv_PtrScpt)->ScptId;
				// check slave range
				if ( (E_GrP3ScptIdSlaveBase <= Tv_ScptId) && ((E_GrP3ScptIdSlaveBase + E_GrP3DvlSlaveMaxCnt) > Tv_ScptId) )
				{
					// mark slave
					Tv_DwIdx	=	(Tv_ScptId - E_GrP3ScptIdSlaveBase) >> E_GrP3DvlSlaveMapShft;
					Tv_BitIdx	=	(Tv_ScptId - E_GrP3ScptIdSlaveBase) & ((1 << E_GrP3DvlSlaveMapShft) - 1);
					Tv_Mask		=	1 << Tv_BitIdx;
					// add
					m_DvlSlvMap[Tv_DwIdx]	=	m_DvlSlvMap[Tv_DwIdx] | Tv_Mask;
				}
				// go next object
				Tv_PtrScpt	=	(__u8*)( (__u32)Tv_PtrScpt + sizeof(St_GrP3ScptObjHd) + (__u32)((Ptr_GrP3ScptObjHd)Tv_PtrScpt)->ParaSize ) ;
			}
			else if ( E_GrP3ScptObjEdx == *Tv_PtrScpt )
			{
				Tv_Depth --;
				// go next object
				Tv_PtrScpt	=	(__u8*)( (__u32)Tv_PtrScpt + sizeof(St_GrP3ScptObjEnd) ) ;
			}
			else
			{
				DbgMsgPrint("Cls_GrP3DvlMng::LcSlaveIdMarkByScpt - appear bad script depth.\n");
				break;
			}

		} while ( 0 != Tv_Depth );
		
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcSlaveMapRebuild( void )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		// clear slave map
		GrDumyZeroMem( m_DvlSlvMap, sizeof(m_DvlSlvMap) );
		// check window lump
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlWinMaxCnt;Tv_WkIdx++ )
		{
			if ( (NULL != m_DvlScptWinTbl[Tv_WkIdx].Pos) && (0 != m_DvlWinInfo[Tv_WkIdx].Size) )
			{
				// update
				LcSlaveIdMarkByScpt( m_DvlScptWinTbl[Tv_WkIdx].Pos );
			}
		}
		
		// check page lump
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlPageMaxCnt;Tv_WkIdx++ )
		{
			if ( (NULL != m_DvlScptPageTbl[Tv_WkIdx].Pos) && (0 != m_DvlPageInfo[Tv_WkIdx].Size) )
			{
				// update
				LcSlaveIdMarkByScpt( m_DvlScptPageTbl[Tv_WkIdx].Pos );
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcScptSaveHead( void )
{
	// local -------------------
		St_GrP3ScptHdInfo	Tv_Hd;
		__u32	Tv_Mili;
		BOOL8	Tv_IsSm;
	// code --------------------
		// init
		Tv_Hd.Fcc			=	E_GrP3ScptFcc;
		Tv_Hd.Ver			=	E_GrP3ScptVer;
		Tv_Hd.Time		=	GrTimeGet( &Tv_Mili, &Tv_IsSm );

		m_DvlSaved		=	m_DvlSaved + m_DvlFc->Write( &Tv_Hd, sizeof(Tv_Hd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_Hd);

}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcScptSaveCfgPrj( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3ScptDataCfgPrj;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlScptCfgPrj);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlScptCfgPrj, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcScptSaveCfgRtl( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3ScptDataCfgRtl;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlScptCfgRtl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlScptCfgRtl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcScptSaveMdlAtb( __u8 A_MdlIdx )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3ScptDataMdlAtb;
		Tv_DataHd.Id			=	A_MdlIdx;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(St_GrP3ScptCfgMdl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &m_DvlPrjMdlAtb[A_MdlIdx], Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcScptSaveWinInfo( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3ScptDataWinScptTbl;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlScptWinTbl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptWinTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcScptSaveWinScpt( void )
{
	// local -------------------
		__u16	Tv_WkIdx;
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------

		// script data write
		Tv_DataHd.Type		=	E_GrP3ScptDataWinScptData;
		Tv_DataHd.SubIdx	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlWinMaxCnt;Tv_WkIdx++ )
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
void	Cls_GrP3DvlMng::LcScptSavePageInfo( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3ScptDataPageScptTbl;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlScptPageTbl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptPageTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcScptSavePageScpt( void )
{
	// local -------------------
		__u16	Tv_WkIdx;
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3ScptDataPageScptData;
		Tv_DataHd.SubIdx	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlPageMaxCnt;Tv_WkIdx++ )
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
void	Cls_GrP3DvlMng::LcScptSaveAsmTbl( void )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3ScptDataAsmTbl;
		Tv_DataHd.Id			=	0;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(m_DvlScptAsmTbl);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptAsmTbl, Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcScptSaveAsmScpt( void )
{
	// local -------------------
		__u16	Tv_WkIdx;
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		// write assembly code
		Tv_DataHd.Type	=	E_GrP3ScptDataAsmData;
		Tv_DataHd.SubIdx	=	0;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlAsmMaxCnt;Tv_WkIdx++ )
		{
			// check exist
			if ( (NULL != m_DvlScptAsmTbl[Tv_WkIdx].Pos) && (0 != m_DvlScptAsmTbl[Tv_WkIdx].Cnt) )
			{
				// write header
				Tv_DataHd.Id		=	Tv_WkIdx;
				Tv_DataHd.Size	=	E_GrP3AsmOpUnitSize * (__u32)m_DvlScptAsmTbl[Tv_WkIdx].Cnt;
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
				m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptAsmTbl[Tv_WkIdx].Pos, Tv_DataHd.Size );	// write script head
				m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcScptSavePstTbl( __u8 A_Skin )
{
	// local -------------------
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		Tv_DataHd.Type		=	E_GrP3ScptDataPstTbl;
		Tv_DataHd.Id			=	A_Skin;
		Tv_DataHd.SubIdx	=	0;
		Tv_DataHd.Size		=	sizeof(St_GrP3ScptPstItm) * E_GrP3DvlPstMaxCnt;
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
		m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
		m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptPstTbl[A_Skin], Tv_DataHd.Size );	// write script
		m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcScptSavePstScpt( __u8 A_Skin )
{
	// local -------------------
		__u16	Tv_WkIdx;
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		// script data write
		Tv_DataHd.Type		=	E_GrP3ScptDataPstScptData;
		Tv_DataHd.SubIdx	=	A_Skin;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DvlPstMaxCnt;Tv_WkIdx++ )
		{
			if ( (NULL != m_DvlScptPstTbl[A_Skin][Tv_WkIdx].Pos) && (0 != m_DvlScptPstTbl[A_Skin][Tv_WkIdx].Cnt) )
			{
				Tv_DataHd.Id		=	Tv_WkIdx;
				Tv_DataHd.Size	=	sizeof(St_GrP3ScptPstData) * m_DvlScptPstTbl[A_Skin][Tv_WkIdx].Cnt;
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
				m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptPstTbl[A_Skin][Tv_WkIdx].Pos, Tv_DataHd.Size );	// write scripte
				m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcScptSaveFontData( __u8 A_Skin )
{
	// local -------------------
		__u16	Tv_WkIdx;
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		// script data write
		Tv_DataHd.Type		=	E_GrP3ScptDataFontData;
		Tv_DataHd.SubIdx	=	A_Skin;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3MngFontMaxCnt;Tv_WkIdx++ )
		{
			if ( NULL != m_DvlScptFontTbl[A_Skin][Tv_WkIdx] ) 
			{
				Tv_DataHd.Id		=	Tv_WkIdx;
				Tv_DataHd.Size	=	GrP3FontCalcSize( m_DvlScptFontTbl[A_Skin][Tv_WkIdx] );
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( &Tv_DataHd, sizeof(Tv_DataHd) );	// write data type
				m_DvlSaveReq	=	m_DvlSaveReq + sizeof(Tv_DataHd);
				m_DvlSaved	=	m_DvlSaved + m_DvlFc->Write( m_DvlScptFontTbl[A_Skin][Tv_WkIdx], Tv_DataHd.Size );	// write scripte
				m_DvlSaveReq	=	m_DvlSaveReq + Tv_DataHd.Size;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcScptSaveImgData( __u8 A_Skin )
{
	// local -------------------
		__u16	Tv_WkIdx;
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		// script data write
		Tv_DataHd.Type		=	E_GrP3ScptDataImageData;
		Tv_DataHd.SubIdx	=	A_Skin;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3MngImgScptMaxCnt;Tv_WkIdx++ )
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
void	Cls_GrP3DvlMng::LcScptSaveTxtData( void )
{
	// local -------------------
		__u16	Tv_WkIdx;
		__u8	Tv_LngIdx;
		St_GrP3ScptDataHd	Tv_DataHd;
	// code --------------------
		// script data write
		Tv_DataHd.Type	=	E_GrP3ScptDataTextData;
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3MngTextScptMaxCnt;Tv_WkIdx++ )
		{
			Tv_DataHd.Id		=	Tv_WkIdx;
			for ( Tv_LngIdx=0;Tv_LngIdx < E_GrP3ScptLangMaxCnt;Tv_LngIdx++ )
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
BOOL8	Cls_GrP3DvlMng::DvlScptExport( WCHAR* A_StrFn, __u8 A_Model )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3DvlMsgScptExport	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdScptExport;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		Tv_Msg.StrFn				=	A_StrFn;
		Tv_Msg.Model				=	A_Model;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcDvlScptExport( WCHAR* A_StrFn, __u8 A_Model )
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
		if ( E_GrP3DvlModelThis == A_Model )
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
					DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlScptExport - script file %d bytes saved.\n" , m_DvlSaved );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3DvlMng::LcDvlScptExport - script file save requested as %d bytes, but %d bytes saved.\n", m_DvlSaveReq, m_DvlSaved );
				}

			}
			delete	m_DvlFc;
			m_DvlFc	=	NULL;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::KeyInput( __u8 A_Key, BOOL8 A_IsPush, __u8 A_DevType )
{
		if ( m_DvlIsDbgRun && ( E_GrP3DvlDbgModeRun == m_DvlDbgMode ) )
		{
			Cls_GrP3Mng::KeyInput( A_Key, A_IsPush, A_DevType );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::MousePosInput( __s32 A_X, __s32 A_Y, BOOL8 A_IsAbs )
{
		if ( m_DvlIsDbgRun && ( E_GrP3DvlDbgModeRun == m_DvlDbgMode )  )
		{
			Cls_GrP3Mng::MousePosInput( A_X, A_Y, A_IsAbs );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::MouseBtnInput( __u8 A_BtnIdx, BOOL8 A_IsPush )
{
		if ( m_DvlIsDbgRun && ( E_GrP3DvlDbgModeRun == m_DvlDbgMode )  )
		{
			Cls_GrP3Mng::MouseBtnInput( A_BtnIdx, A_IsPush );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::MouseWheelInput( BOOL8 A_IsDown )
{
		if ( m_DvlIsDbgRun && ( E_GrP3DvlDbgModeRun == m_DvlDbgMode )  )
		{
			Cls_GrP3Mng::MouseWheelInput( A_IsDown );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlEditObjScrRectGet( __u16 A_ScptId, Ptr_GrRect A_PtrRtRect, BOOL8 A_IsNoChk )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3DvlMsgEditObjScrRectGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjScrRectGet;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRtRect		=	A_PtrRtRect;
		Tv_Msg.IsNoChk			=	A_IsNoChk;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcDvlEditObjScrRectGet( __u16 A_ScptId, Ptr_GrRect A_PtrRtRect, BOOL8 A_IsNoChk )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP3ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP3DoBase*)m_DvlEdtLump;
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
BOOL8	Cls_GrP3DvlMng::DvlEditObjRectSet( __u16 A_ScptId, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3DvlMsgEditObjRectSet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjRectSet;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRect			=	A_PtrRect;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcDvlEditObjRectSet( __u16 A_ScptId, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP3ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP3DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Obj->RectSet( A_PtrRect );
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlEditObjRectGet( __u16 A_ScptId, Ptr_GrRect A_PtrRtRect )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3DvlMsgEditObjRectGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjRectGet;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRtRect		=	A_PtrRtRect;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcDvlEditObjRectGet( __u16 A_ScptId, Ptr_GrRect A_PtrRtRect )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP3ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP3DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					Tv_Obj->RectGet( A_PtrRtRect );
					Tv_Result	=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlEditObjFindByPos( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP3DvlMsgEditObjFindByPos	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjFindByPos;
		Tv_Msg.PosX					=	A_X;
		Tv_Msg.PosY					=	A_Y;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcDvlEditObjFindByPos( __s32 A_X, __s32 A_Y )
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		// find object
		if ( NULL != m_DvlEdtLump )
		{
			Tv_Obj	=	m_DvlEdtLump->FindObjByPos( A_X, A_Y );
			if ( NULL != Tv_Obj )
			{
				Tv_Result	=	Tv_Obj->ScptIdGet();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP3DvlMng::DvlEditObjClassIdGet( __u16 A_ScptId )
{
	// local -------------------
		__u8	Tv_Result;
		St_GrP3DvlMsgEditObjClassIdGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP3DoClassNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjClassIdGet;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrP3DvlMng::LcDvlEditObjClassIdGet( __u16 A_ScptId )
{
	// local -------------------
		__u8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	E_GrP3DoClassNone;

		if ( E_GrP3ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Result	=	E_GrP3DoClassWin;
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
__u32	Cls_GrP3DvlMng::DvlEditObjScptDump( __u16 A_ScptId, void* A_PtrBuf, BOOL8 A_IsCopyMode )
{
	// local -------------------
		__u32	Tv_Result;
		St_GrP3DvlMsgEditObjScptDump	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjScptDump;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrBuf				=	A_PtrBuf;
		Tv_Msg.IsCopyMode		=	A_IsCopyMode;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DvlMng::LcDvlEditObjScptDump( __u16 A_ScptId, void* A_PtrBuf, BOOL8 A_IsCopyMode )
{
	// local -------------------
		__u32	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	0;

		if ( E_GrP3ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP3DoBase*)m_DvlEdtLump;
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
__u16	Cls_GrP3DvlMng::DvlEditObjFrameGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP3DvlMsgEditObjFrameGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjFrameGet;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcDvlEditObjFrameGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
		Cls_GrP3DoBase*	Tv_Frm;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		if ( E_GrP3ScptIdNone != A_ScptId )
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
void	Cls_GrP3DvlMng::DvlEditObjRedraw( __u16 A_ScptId )
{
	// local -------------------
		St_GrP3DvlMsgEditObjRedraw	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjRedraw;
		Tv_Msg.ScptId				=	A_ScptId;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcDvlEditObjRedraw( __u16 A_ScptId )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		if ( E_GrP3ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP3DoBase*)m_DvlEdtLump;
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
__u32	Cls_GrP3DvlMng::DvlEditObjListByPos( __s32 A_X, __s32 A_Y, __u16* A_PtrBuf )
{
	// local -------------------
		__u32	Tv_Result;
		St_GrP3DvlMsgEditObjListByPos	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjListByPos;
		Tv_Msg.X						=	A_X;
		Tv_Msg.Y						=	A_Y;
		Tv_Msg.PtrBuf				=	A_PtrBuf;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DvlMng::LcDvlEditObjListByPos( __s32 A_X, __s32 A_Y, __u16* A_PtrBuf )
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
				A_PtrBuf[Tv_WkIdx]	=	E_GrP3ScptIdNone;
				if ( 0 != Tv_ListTbl[Tv_WkIdx] )
				{
					A_PtrBuf[Tv_WkIdx]	=	((Cls_GrP3DoBase*)Tv_ListTbl[Tv_WkIdx])->ScptIdGet();
				}
			}
		}
	}

	return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlEditObjSlaveObjGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP3DvlMsgEditObjSlaveObjGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjSlaveObjGet;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcDvlEditObjSlaveObjGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
		Cls_GrP3DoBase*	Tv_Slave;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		if ( E_GrP3ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP3DoBase*)m_DvlEdtLump;
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
__u16	Cls_GrP3DvlMng::DvlEditObjNextObjGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP3DvlMsgEditObjNextObjGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjNextObjGet;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcDvlEditObjNextObjGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
		Cls_GrP3DoBase*	Tv_Next;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		if ( E_GrP3ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP3DoBase*)m_DvlEdtLump;
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
__u16	Cls_GrP3DvlMng::DvlEditObjPrntObjGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		St_GrP3DvlMsgEditObjPrntObjGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjPrntObjGet;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcDvlEditObjPrntObjGet( __u16 A_ScptId )
{
	// local -------------------
		__u16	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
		Cls_GrP3DoBase*	Tv_Prnt;
	// code --------------------
		Tv_Result	=	E_GrP3ScptIdNone;

		if ( E_GrP3ScptIdNone != A_ScptId )
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
BOOL8	Cls_GrP3DvlMng::DvlEditObjIsWindow( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrP3DvlMsgEditObjIsWindow	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjIsWindow;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcDvlEditObjIsWindow( __u16 A_ScptId )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP3ScptIdNone != A_ScptId )
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
void	Cls_GrP3DvlMng::DvlEditObjDelete( __u16 A_ScptId )
{
	// local -------------------
		St_GrP3DvlMsgEditObjDelete	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjDelete;
		Tv_Msg.ScptId				=	A_ScptId;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcDvlEditObjDelete( __u16 A_ScptId )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		if ( E_GrP3ScptIdNone != A_ScptId )
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
__u32	Cls_GrP3DvlMng::DvlEditObjParaInfoDump( __u16 A_ScptId, void* A_PtrBuf )
{
	// local -------------------
		__u32	Tv_Result;
		St_GrP3DvlMsgEditObjParaInfoDump	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjParaInfoDump;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrBuf				=	A_PtrBuf;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrP3DvlMng::LcDvlEditObjParaInfoDump( __u16 A_ScptId, void* A_PtrBuf )
{
	// local -------------------
		__u32	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		Tv_Result	=	0;

		if ( E_GrP3ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP3DoBase*)m_DvlEdtLump;
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
void	Cls_GrP3DvlMng::DvlEditObjScptParaUpdt( __u16 A_ScptId, void* A_PtrScpt, BOOL8 A_IsNoCopy )
{
	// local -------------------
		St_GrP3DvlMsgEditObjParaUpdt	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdEditObjParaUpdt;
		Tv_Msg.ScptId				=	A_ScptId;
		Tv_Msg.PtrScpt			=	A_PtrScpt;
		Tv_Msg.IsNoCopy			=	A_IsNoCopy;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcDvlEditObjScptParaUpdt( __u16 A_ScptId, void* A_PtrScpt, BOOL8 A_IsNoCopy )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		if ( E_GrP3ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP3DoBase*)m_DvlEdtLump;
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
BOOL8	Cls_GrP3DvlMng::DvlIsDbgPause( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		//St_GrP3DvlMsgIsDbgPause	Tv_Msg;
	// code --------------------
		/*
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdIsDbgPause;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		*/
		Tv_Result	=	FALSE;
		if ( E_GrP3DvlDbgModePause == m_DvlDbgMode )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcDvlIsDbgPause( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3DvlDbgModePause == m_DvlDbgMode )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlDbgVmCsGet( void )
{
	// local -------------------
		__u16	Tv_Result;
		//St_GrP3DvlMsgDbgVmCsGet	Tv_Msg;
	// code --------------------
		/*
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdDbgVmCsGet;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		*/
		Tv_Result	=	m_VmTbl[m_VmIdx].Cs;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcDvlDbgVmCsGet( void )
{
		return	m_VmTbl[m_VmIdx].Cs;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::DvlDbgVmIpGet( void )
{
	// local -------------------
		__u16	Tv_Result;
		//St_GrP3DvlMsgDbgVmIpGet	Tv_Msg;
	// code --------------------
		/*
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3DvlMcmdDbgVmIpGet;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		*/
		Tv_Result	=	m_VmTbl[m_VmIdx].Ip;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP3DvlMng::LcDvlDbgVmIpGet( void )
{
		return	m_VmTbl[m_VmIdx].Ip;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlDbgResume( void )
{
		m_DvlDbgMode	=	E_GrP3DvlDbgModeResume;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlDbgStep( void )
{
		m_DvlDbgMode	=	E_GrP3DvlDbgModeStep;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlEditObjInfoT1( __u16 A_ScptId, __u8* A_PtrRtClassType, Ptr_GrRect A_PtrRtRect, __u16* A_PtrRtSlaveId,
	__u16* A_PtrRtNextId )
{
	// local -------------------
		BOOL8	Tv_Result;
		//St_GrP3DvlMsgEditObjInfoT1	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		/*
		Tv_Msg.Hd.MsgHd.Cmd		=	m_DvlMsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrP3DvlMcmdEditObjInfoT1;
		Tv_Msg.PtrRslt				=	&Tv_Result;
		Tv_Msg.ScptId					=	A_ScptId;
		Tv_Msg.PtrRtClassType	=	A_PtrRtClassType;
		Tv_Msg.PtrRtRect			=	A_PtrRtRect;
		Tv_Msg.PtrRtSlaveId		=	A_PtrRtSlaveId;
		Tv_Msg.PtrRtNextId		=	A_PtrRtNextId;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		*/

		Tv_Result	=	LcDvlEditObjInfoT1( A_ScptId, A_PtrRtClassType, A_PtrRtRect, A_PtrRtSlaveId, A_PtrRtNextId );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::LcDvlEditObjInfoT1( __u16 A_ScptId, __u8* A_PtrRtClassType, Ptr_GrRect A_PtrRtRect, __u16* A_PtrRtSlaveId,
	__u16* A_PtrRtNextId )
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrP3DoBase*	Tv_Obj;
		Cls_GrP3DoBase*	Tv_Ref;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrP3ScptIdNone != A_ScptId )
		{
			// find object
			if ( NULL != m_DvlEdtLump )
			{
				if ( A_ScptId == m_DvlEdtLump->ScptIdGet() )
				{
					Tv_Obj	=	(Cls_GrP3DoBase*)m_DvlEdtLump;
				}
				else
				{
					Tv_Obj	=	m_DvlEdtLump->FindSlaveObjByScptId( A_ScptId );
				}
				if ( NULL != Tv_Obj )
				{
					*A_PtrRtClassType	=	Tv_Obj->ClassIdGet();
					Tv_Obj->RectGet( A_PtrRtRect );

					*A_PtrRtSlaveId		=	E_GrP3ScptIdNone;
					Tv_Ref	=	Tv_Obj->SlaveObjGet();
					if ( NULL != Tv_Ref )
					{
						*A_PtrRtSlaveId		=	Tv_Ref->ScptIdGet();
					}
					
					*A_PtrRtNextId		=	E_GrP3ScptIdNone;
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
void	Cls_GrP3DvlMng::LcDvlRatbUpdt( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		m_DvlRatbTimer ++;
		if ( E_GrP3DvlMngRatbTime <= m_DvlRatbTimer )
		{
			// change attribute
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
			{
				if ( m_DvlIsDbgRun )
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
BOOL8	Cls_GrP3DvlMng::DvlAsmBreakPointToggle( __u16 A_Id, __u16 A_Ip )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrP3ScptAsmInfo	Tv_PtrInfo;
		Ptr_GrP3AsmOpBase	Tv_PtrOp;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrInfo	=	AsmPtrGet( A_Id );
		if ( NULL != Tv_PtrInfo )
		{
			if ( (NULL != Tv_PtrInfo->Pos) && (A_Ip < Tv_PtrInfo->Cnt) )
			{
				Tv_PtrOp	=	(Ptr_GrP3AsmOpBase)Tv_PtrInfo->Pos;
				Tv_PtrOp[A_Ip].Code	=	Tv_PtrOp[A_Ip].Code ^ E_GrP3AsmOpBrkPntMask;
				Tv_Result	=	TRUE;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlAsmActiveToggle( __u16 A_Id, __u16 A_Ip )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrP3ScptAsmInfo	Tv_PtrInfo;
		Ptr_GrP3AsmOpBase	Tv_PtrOp;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrInfo	=	AsmPtrGet( A_Id );
		if ( NULL != Tv_PtrInfo )
		{
			if ( (NULL != Tv_PtrInfo->Pos) && (A_Ip < Tv_PtrInfo->Cnt) )
			{
				Tv_PtrOp	=	(Ptr_GrP3AsmOpBase)Tv_PtrInfo->Pos;
				Tv_PtrOp[A_Ip].Code	=	Tv_PtrOp[A_Ip].Code ^ E_GrP3AsmOpDeactMask;
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlFwUpgdRun( __u32 A_CtlgHndl )
{
	// local -------------------
	// code --------------------
		// message
		DbgMsgPrint( "Firmware Upgrade : catalog id %d\n", A_CtlgHndl );

		// request upgrade
		m_SysStat	=	E_GrP3MngSysStatFwUpgd;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcDispIdTblBuild( void )
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
void	Cls_GrP3DvlMng::LcDispIdSlaveAdd( Cls_GrP3DoBase* A_Obj )
{
	// local -------------------
		Cls_GrP3DoBase*	Tv_Obj;
	// code --------------------
		if ( E_GrP3DvlLumpSlvMax >  m_DvlEdtIdDispCnt  )
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
Ptr_GrP3DvlObjInfo	Cls_GrP3DvlMng::DvlDispIdTblGet( void )
{
		return	m_DvlEdtIdDispTbl;
}
//--------------------------------------------------------------------
Ptr_GrP3PrjBldFile	Cls_GrP3DvlMng::DvlBldFileInfoGet( __u8 A_ModelIdx )
{
		return	&m_DvlBldInfo[A_ModelIdx];
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcDvlLanCblUpdt( void )
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
void	Cls_GrP3DvlMng::LcDvrUsbUpdt( void )
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
void	Cls_GrP3DvlMng::LcDvrCmsUpdt( void )
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
void	Cls_GrP3DvlMng::LcDvrBkupUpdt( void )
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
void	Cls_GrP3DvlMng::LcDvrLogBkupUpdt( void )
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
BOOL8	Cls_GrP3DvlMng::DvlBkupReq( __u8 A_Media, __u8 A_RecDev, __u16 A_ChMap,
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

						DbgMsgPrint( "Cls_GrP3DvlMng::DvlBkupReq - try backup %s dev : %d ch : %04X  time : %s ~ %s .\n", 
							Tv_StrMedia, A_RecDev, A_ChMap, Tv_StrTmSt, Tv_StrTmEd );

						// notify backup start
						Tv_Msg.Cmd	=	E_GrTaskMsgBkupStart;
						GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );

						m_IsDvlBkupRun		=	TRUE;

						Tv_Result	=	TRUE;
					}
					else
					{
						DbgMsgPrint( "Cls_GrP3DvlMng::DvlBkupReq - backup already progress!\n" );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3DvlMng::DvlBkupReq - bad time range!\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3DvlMng::DvlBkupReq - channel map is zero!\n" );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DvlMng::DvlBkupReq - record device input bad!\n" );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlBkupCancel( void )
{
		if ( m_IsDvlBkupRun )
		{
			m_Gvs.BkupErrCode		=	5;
			m_Gvs.BkupTimePrgs	=	m_Gvs.BkupTimeEd;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DvlMng::DvlLogBkupReq( __u8 A_RecDev, __u16 A_ChMap, __u16 A_EvtMap,
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

						DbgMsgPrint( "Cls_GrP3DvlMng::DvlBkupReq - try log backup %s dev : %d ch : %04X  time : %s ~ %s .\n", 
							Tv_StrFn, A_RecDev, A_ChMap, Tv_StrTmSt, Tv_StrTmEd );

						// notify backup start
						Tv_Msg.Cmd	=	E_GrTaskMsgLogBkupStart;
						GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );

						m_IsDvlLogBkupRun		=	TRUE;

						Tv_Result	=	TRUE;
					}
					else
					{
						DbgMsgPrint( "Cls_GrP3DvlMng::DvlLogBkupReq - log backup already progress!\n" );
					}
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3DvlMng::DvlLogBkupReq - bad time range!\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrP3DvlMng::DvlLogBkupReq - channel map is zero!\n" );
			}
		}
		else
		{
			DbgMsgPrint( "Cls_GrP3DvlMng::DvlLogBkupReq - record device input bad!\n" );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlLogBkupCancel( void )
{
		if ( m_IsDvlLogBkupRun )
		{
			m_Gvs.LogErrCode		=	5;
			m_Gvs.LogPrgs				=	m_Gvs.LogTotal;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::RtlAtLockProc( void )
{
	// local -------------------
	// code --------------------
		if ( m_DvlIsDbgRun )
		{
			Cls_GrP3Mng::RtlAtLockProc();
		}
		else
		{
			m_IsAtLock	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::DvlSkinCopy( __u8 A_TgIdx, __u8 A_SrIdx )
{
	// local -------------------
		__u32	Tv_WkIdx;
		__u32	Tv_Size;
	// code --------------------
		// check source and target index
	if ( (A_TgIdx != A_SrIdx) && (E_GrP3DvlSkinMaxCnt > A_TgIdx) && (E_GrP3DvlSkinMaxCnt > A_SrIdx) )
	{
		// copy
		// script
		for (Tv_WkIdx=0;Tv_WkIdx <E_GrP3DvlPstMaxCnt;Tv_WkIdx++ )
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
				m_DvlScptPstTbl[A_TgIdx][Tv_WkIdx].Pos	=	malloc( E_GrP3DvlPstBufMax );
				if ( NULL != m_DvlScptPstTbl[A_TgIdx][Tv_WkIdx].Pos )
				{
					m_DvlScptPstTbl[A_TgIdx][Tv_WkIdx].Cnt	=	m_DvlScptPstTbl[A_SrIdx][Tv_WkIdx].Cnt;
					GrDumyCopyMem( m_DvlScptPstTbl[A_TgIdx][Tv_WkIdx].Pos, m_DvlScptPstTbl[A_SrIdx][Tv_WkIdx].Pos, E_GrP3DvlPstBufMax );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3DvlMng::DvlSkinCopy - Paint scipt copy error!\n" );
				}
			}
		}

		// image
		for (Tv_WkIdx=0;Tv_WkIdx <E_GrP3MngImgScptMaxCnt;Tv_WkIdx++ )
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
					DbgMsgPrint( "Cls_GrP3DvlMng::DvlSkinCopy - image copy error!\n" );
				}
			}
		}

		// font
		for (Tv_WkIdx=0;Tv_WkIdx <E_GrP3MngFontMaxCnt;Tv_WkIdx++ )
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
				Tv_Size	=	GrP3FontCalcSize( m_DvlScptFontTbl[A_SrIdx][Tv_WkIdx] );
				m_DvlScptFontTbl[A_TgIdx][Tv_WkIdx]	=	(Ptr_GrP3FontHead)malloc( Tv_Size );
				if ( NULL != m_DvlScptFontTbl[A_TgIdx][Tv_WkIdx] )
				{
					GrDumyCopyMem( m_DvlScptFontTbl[A_TgIdx][Tv_WkIdx], m_DvlScptFontTbl[A_SrIdx][Tv_WkIdx], Tv_Size );
				}
				else
				{
					DbgMsgPrint( "Cls_GrP3DvlMng::DvlSkinCopy - font copy error!\n" );
				}
			}
		}

	}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3DvlMng::LcFontUpdt( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_SkinIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3MngFontMaxCnt;Tv_WkIdx++ )
		{
			// release font
			if ( NULL != m_FontMng[Tv_WkIdx] )
			{
				delete	m_FontMng[Tv_WkIdx];
				m_FontMng[Tv_WkIdx]	=	NULL;
			}
			// check exist font
			if ( E_GrP3MngFontMaxCnt > Tv_WkIdx )
			{
				Tv_SkinIdx	=	m_DvlPrjModel[m_DvlPrjRtl.ModelIdx].SkinIdx;
				if ( NULL != m_DvlScptFontTbl[Tv_SkinIdx][Tv_WkIdx] )
				{
					m_FontMng[Tv_WkIdx]	=	(Cls_GrP3Font*)new Cls_GrP3Font( (Ptr_GrP3FontHead)m_DvlScptFontTbl[Tv_SkinIdx][Tv_WkIdx] );
				}
			}
		}
}
//--------------------------------------------------------------------
Ptr_GrP3PrjCfg	Cls_GrP3DvlMng::DvlConfigPtrGet( void )
{
		return	&m_DvlPrjConfig;
}
//--------------------------------------------------------------------
