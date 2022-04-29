/*
	GAUSE Platform API base

*/

#include	<P3/GrP3ApiBase.h>
#include	<P3/GrP3Bios.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3Cfg.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3DoWin.h>
#include	<GrDebug.h>
#include <Dvr/GrDvrBase.h>
#include <GrDumyTool.h>

#ifdef GR_P3_EDITOR
#include <P3/Dvl/GrP3DvlBase.h>
#endif

#include <GrStrTool.h>
#include <Dvr/PtzV2/GrPtzV2Base.h>
#include <Dvr/PtzV2/GrPtzV2Mng.h>

#include <P3/GrP3UiCmd.h>

#include <P3/Sio/GrP3SioMng.h>

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var
extern	Cls_GrP3Mng*	V_GrP3Mng;
extern	__u32	V_GrP3UartSpeedTbl[E_GrP3CfgUartSpeedCnt];

St_GrP3ApiTbl	V_GrP3ApiCodeTbl[E_GrP3ApiCnt]	=	
{
	{	E_GrP3ApiQuit,						GrP3BiosQuit },
	{	E_GrP3ApiTimeGet,					GrP3ApiTimeGet },
	{	E_GrP3ApiTimeSet,					GrP3BiosTimeSet },
	{	E_GrP3ApiQuadModeNext,		GrP3ApiQuadModeNext },
	{	E_GrP3ApiCfgReset,				GrP3ApiCfgReset },
	{	E_GrP3ApiCfgSaveToRom,		GrP3BiosCfgSaveToRom },
	{	E_GrP3ApiCfgRestore,			GrP3BiosCfgRestore },
	{	E_GrP3ApiCfgLangUpdt,			GrP3ApiCfgLangUpdt },
	{	E_GrP3ApiCfgIsChanged,		GrP3BiosCfgIsChanged },
	{	E_GrP3ApiCfgAssign,				GrP3ApiCfgAssign },
	{	E_GrP3ApiNetModelSet,			GrP3ApiNetModelSet },
	{	E_GrP3ApiRmtTmpAbl,				GrP3ApiRmtTmpAble },
	{	E_GrP3ApiRmtIdSet,				GrP3ApiRmtIdSet },
	{	E_GrP3ApiEvtLogAdd,				GrP3BiosEvtLogAdd },
	{	E_GrP3ApiDipSwGet,				GrP3BiosDipSwGet },

	{	E_GrP3ApiFocusIdGet,			GrP3ApiFocusIdGet },
	{	E_GrP3ApiKeyBlockSet,			GrP3ApiKeyBlockSet },
	{	E_GrP3ApiLayerUpClose,		GrP3ApiLayerUpClose },
	{	E_GrP3ApiMenuModeSet,			GrP3ApiUiMenuModeSet },
	{	E_GrP3ApiLoginModeSet,		GrP3ApiUiLoginModeSet },
	{	E_GrP3ApiMsgBoxMode,			GrP3ApiUiMsgBoxModeSet },

	{	E_GrP3ApiMacAdrGetGvu,		GrP3BiosMacAdrGetGvu },
	{	E_GrP3ApiMacAdrSetGvu,		GrP3BiosMacAdrSetGvu },
	{	E_GrP3ApiWebCodeGet,			GrP3BiosWebCodeGet },
	{	E_GrP3ApiHddFormat,				GrP3BiosHddFormat },
	{	E_GrP3ApiHddRecSizeGet,		GrP3BiosHddRecSizeGet },
	{	E_GrP3ApiHddStatGet,			GrP3BiosHddStatGet },
	{ E_GrP3ApiGroupHddFormat,	GrP3BiosHddGroupFormat },
	{	E_GrP3ApiGroupRecSizeGet,	GrP3BiosGroupRecSizeGet },
	{	E_GrP3ApiGroupRecStatGet,	GrP3BiosGroupRecStatGet },
	{	E_GrP3ApiGroupDayMapGet,	GrP3BiosGroupDayMapGet },
	{	E_GrP3ApiGroupTimeRngGet,	GrP3BiosGroupTimeRngGet },
	{	E_GrP3ApiRecSizeExpect,		GrP3BiosRecSizeExpect },
	{	E_GrP3ApiRecCfgChk,				GrP3BiosRecCfgChk },
	{	E_GrP3ApiVrecStatGet,			GrP3BiosVrecStatGet },
	{	E_GrP3ApiVrecFpsTextAdd,	GrP3BiosVrecFpsTextAdd },
	{	E_GrP3ApiVrecFpsToStr,		GrP3BiosVrecFpsToStr },
	{	E_GrP3ApiVrecFpsCntGet,		GrP3BiosVrecFpsCntGet },

	{ E_GrP3ApiCamAutoGainSet,	GrP3BiosCamAutoGainSet },
	{ E_GrP3ApiCamGainSet,			GrP3BiosCamGainSet },
	{ E_GrP3ApiCamBriSet,				GrP3BiosCamBrightSet },
	{ E_GrP3ApiCamConSet,				GrP3BiosCamConstSet },
	{ E_GrP3ApiCamHueSet,				GrP3BiosCamHueSet },
	{ E_GrP3ApiCamSatSet,				GrP3BiosCamSatSet },
	{ E_GrP3ApiCamTypeGet,			GrP3BiosCamTypeGet },
	{ E_GrP3ApiCamTypeToStr,		GrP3BiosCamTypeToStr },

	{ E_GrP3ApiPlayModeSet,			GrP3ApiPlayModeSet },
	{ E_GrP3ApiPlayGroupSel,		GrP3BiosPlayGroupSet },
	{ E_GrP3ApiPlayGoTime,			GrP3BiosPlayGoTime },
	{ E_GrP3ApiPlaySpeedSet,		GrP3BiosPlaySpeedSet },
	{ E_GrP3ApiPlayDirSet,			GrP3BiosPlayDirSet },
	{ E_GrP3ApiPlayDo,					GrP3BiosPlayDo },
	{ E_GrP3ApiPlayPuase,				GrP3BiosPlayPause },
	{ E_GrP3ApiPlayTimeGet,			GrP3BiosPlayTimeGet },
	{ E_GrP3ApiPlayGoFirst,			GrP3BiosPlayGoFirst },
	{ E_GrP3ApiPlayGoRecent,		GrP3BiosPlayGoRecent },
	{ E_GrP3ApiPlayNextFrm,			GrP3BiosPlayNextFrm },
	{ E_GrP3ApiPlayPrevFrm,			GrP3BiosPlayPrevFrm },

	{ E_GrP3ApiCtlgItemValGet,	GrP3ApiCtlgItemValGet },
	{ E_GrP3ApiCtlgItemStrGet,	GrP3ApiCtlgItemStrGet },
	{ E_GrP3ApiCfgAlphaUpdt,		GrP3ApiUiAlphaUpdt },
	{ E_GrP3ApiFwVerGet,				GrP3ApiFwVerGet },
	//{ E_GrP3ApiModelGet,				GrP3ApiModelGet },
	{ E_GrP3ApiCfgDateFmtUpdt,	GrP3ApiCfgDateFmtUpdt },
	{ E_GrP3ApiQuadPageNext,		GrP3ApiQuadPageNext },
	{ E_GrP3ApiQuadModeSet,			GrP3ApiQuadModeSet },
	{ E_GrP3ApiQuadModeGet,			GrP3ApiQuadModeGet },
	{ E_GrP3ApiQuadLock,				GrP3BiosQuadLock },
	{ E_GrP3ApiQuadUnlock,			GrP3BiosQuadUnlock },
	{ E_GrP3ApiAutoSeqSet,			GrP3ApiAutoSeqSet },
	{ E_GrP3ApiDispResTextAdd,	GrP3BiosDispResTextAdd },
	{ E_GrP3ApiDispResToStr,		GrP3BiosDispResToStr },
	{ E_GrP3ApiDispResCntGet,		GrP3BiosDispResCntGet },

	{ E_GrP3ApiAdoLiveChSet,		GrP3ApiAdoLiveChSet },
	{ E_GrP3ApiAdoLiveVolSet,		GrP3ApiAdoLiveVolSet },
	{ E_GrP3ApiAdoPlayChSet,		GrP3ApiAdoPlayChSet },
	{ E_GrP3ApiAdoPlayVolSet,		GrP3ApiAdoPlayVolSet },

	{ E_GrP3ApiFwUpgdRun,				GrP3BiosFwUpgdRun },
	{ E_GrP3ApiCfgFileLoad,			GrP3BiosCfgFileLoad },
	{ E_GrP3ApiCfgFileSave,			GrP3BiosCfgFileSave },
	
	{ E_GrP3ApiLogIn,						GrP3ApiLogIn },
	{ E_GrP3ApiLogOut,					GrP3ApiLogOut },
	{ E_GrP3ApiPasswdRuleChk,		GrP3ApiPasswdRuleChk },

	{ E_GrP3ApiRecEmgcSet,			GrP3ApiRecEmgcSet },

	{ E_GrP3ApiPtzPanTilt,			GrP3ApiPtzPanTilt },
	{ E_GrP3ApiPtzZoom,					GrP3ApiPtzZoom },
	{ E_GrP3ApiPtzFocus,				GrP3ApiPtzFocus },
	{ E_GrP3ApiPtzIris,					GrP3ApiPtzIris },
	{ E_GrP3ApiPtzStop,					GrP3ApiPtzStop },
	{ E_GrP3ApiPtzAutoFocus,		GrP3ApiPtzAutoFocus },
	{ E_GrP3ApiPtzMenuCall,			GrP3ApiPtzMenuCall },
	{ E_GrP3ApiPtzMenuExit,			GrP3ApiPtzMenuExit },
	{ E_GrP3ApiPtzMenuLeft,			GrP3ApiPtzMenuLeft },
	{ E_GrP3ApiPtzMenuRight,		GrP3ApiPtzMenuRight },
	{ E_GrP3ApiPtzMenuUp,				GrP3ApiPtzMenuUp },
	{ E_GrP3ApiPtzMenuDown,			GrP3ApiPtzMenuDown },
	{ E_GrP3ApiPtzMenuOk,				GrP3ApiPtzMenuOk },
	{ E_GrP3ApiPtzMenuCancel,		GrP3ApiPtzMenuCancel },
	{ E_GrP3ApiPtzPresetSet,		GrP3ApiPtzPresetSet },
	{ E_GrP3ApiPtzPresetMove,		GrP3ApiPtzPresetMove },
	{ E_GrP3ApiPtzTourRun,			GrP3ApiPtzTourRun },
	{ E_GrP3ApiPtzTourSet,			GrP3ApiPtzTourSet },
	{ E_GrP3ApiPtzIdView,				GrP3ApiPtzIdView },
	{ E_GrP3ApiPtzIdSet,				GrP3ApiPtzIdSet },
	{ E_GrP3ApiPtzModelTxtAdd,	GrP3ApiPtzModelTxtAdd },
	{ E_GrP3ApiPtzCtrlLock,			GrP3ApiPtzCtrlLock },
	{ E_GrP3ApiCtrlModelTxtAdd,	GrP3ApiCtrlModelTxtAdd }, //96

	{ E_GrP3ApiBkupMediaSizeGet,	GrP3BiosBkupMediaSizeGet },
	{ E_GrP3ApiBkupRequest,			GrP3BiosBkupRequest },
	{ E_GrP3ApiBkupCancel,			GrP3BiosBkupCancel },

	{ E_GrP3ApiErrBuzOff,				GrP3BiosErrBuzOff },
	{ E_GrP3ApiNtpTest,					GrP3BiosNtpTest },
	{ E_GrP3ApiDoSet,						GrP3BiosDoSet },

	{ E_GrP3ApiLogBkupRequest,	GrP3BiosLogBkupRequest },
	{ E_GrP3ApiLogBkupCancel,		GrP3BiosLogBkupCancel },

	{ E_GrP3ApiMtnSetChk,				GrP3BiosCfgMtnSetChk },
	{ E_GrP3ApiBlockageChUpdt,	GrP3BiosCfgBlkageMapUpdt },
	{ E_GrP3ApiPwrOff,					GrP3BiosPwrOff },

	{ E_GrP3ApiEmailTest,				GrP3BiosEmailTest },
};

#ifdef GR_P3_EDITOR

St_GrP3DvlApiInfo	V_GrP3ApiInfoTbl[E_GrP3ApiCnt]	=	
{
	{	E_GrP3ApiQuit,						L"Quit :: " },
	{	E_GrP3ApiTimeGet,					L"TimeGet :: out : (0) time" },
	{	E_GrP3ApiTimeSet,					L"TimeSet :: in : (1) time" },
	{ E_GrP3ApiFwVerGet,				L"FwVerGet :: out : (0) firmware version" },
	//{ E_GrP3ApiModelGet,				L"ModelGet :: out : (0) model number" },
	{ E_GrP3ApiNetModelSet,			L"Network Model Set :: in : (1) network model number" },
	{	E_GrP3ApiRmtTmpAbl,				L"REMOCON temporary input able :: in : (1) 0 = disable, 1 = able" },
	{	E_GrP3ApiRmtIdSet,				L"REMOCON running ID set :: in : (1) ID number" },
	{	E_GrP3ApiEvtLogAdd,				L"EvtLogAdd :: in : (1)type (2)channel mask (3)para0 (4) para1" },
	{	E_GrP3ApiDipSwGet,				L"Dip switch get :: out : (0) dip switch bit map" },

	{ E_GrP3ApiLogIn,						L"LogIn :: in : (1) id index (2) password treg index (3) dual password treg index :: out : (0) 0=fail,1=success" },
	{ E_GrP3ApiLogOut,					L"LogOut :: " },
	{ E_GrP3ApiPasswdRuleChk,		L"PasswdRuleChk :: out : (0) o=bad, 1=good" },
	{ E_GrP3ApiErrBuzOff,				L"ErrBuzOff :: " },

	{	E_GrP3ApiQuadModeNext,		L"QuadModeNext :: in : (1) console index " },
	{	E_GrP3ApiQuadPageNext,		L"QuadPageNext :: in : (1) console index " },
	{	E_GrP3ApiQuadModeSet,			L"QuadModeSet :: in : (1) console index, (2) mode, (3) page" },
	{	E_GrP3ApiQuadModeGet,			L"QuadModeGet :: in : (1) console index :: out : (0) mode, (1) page" },
	{	E_GrP3ApiQuadLock,				L"QuadLock :: " },
	{	E_GrP3ApiQuadUnlock,			L"QuadUnlock :: " },

	{	E_GrP3ApiAutoSeqSet,			L"AutoSeqSet :: in : (1) 0 = off, 1= on" },
	{	E_GrP3ApiDispResTextAdd,	L"DispResTextAdd :: in : (1) consol index (2) window id (3) control id" },
	{	E_GrP3ApiDispResToStr,		L"DispResToStr :: in : (1) resolution index :: out : (T0) text" },
	{	E_GrP3ApiDispResCntGet,		L"DispResCntGet :: out : (0) count" },

	{	E_GrP3ApiCfgReset,				L"CfgReset :: in : (1) 0 = normal , 1 = factroy default " },
	{	E_GrP3ApiCfgSaveToRom,		L"CfgSaveToRom :: " },
	{	E_GrP3ApiCfgRestore,			L"CfgRestore :: in : " },
	{	E_GrP3ApiCfgLangUpdt,			L"CfgLangUpdt :: " },
	{	E_GrP3ApiCfgIsChanged,		L"CfgIsChanged :: out : (0) 1=changed" },
	{	E_GrP3ApiCfgAlphaUpdt,		L"CfgAlphaUpdt :: " },
	{	E_GrP3ApiCfgDateFmtUpdt,	L"CfgDateFmtUpdt :: " },

	{	E_GrP3ApiFocusIdGet,			L"FocusIdGet :: in : (1) console index :: out : (0) script id " },
	{	E_GrP3ApiKeyBlockSet,			L"KeyBlockSet :: in : (1) key code" },
	{	E_GrP3ApiLayerUpClose,		L"LayerUpClose :: in : (1) console index (2) start layer" },
	{	E_GrP3ApiMenuModeSet,			L"MenuModeSet :: in : (1) menu mode" },
	{	E_GrP3ApiLoginModeSet,		L"LoginModeSet :: in : (1) login mode" },
	{	E_GrP3ApiMsgBoxMode,			L"MsgBoxModeSet :: in : (1) message box mode" },

	{	E_GrP3ApiCfgAssign,				L"CfgAssign :: " },

	{	E_GrP3ApiMacAdrGetGvu,		L"MacAdrGetGvu :: in : (1) recive GVU var index" },
	{	E_GrP3ApiMacAdrSetGvu,		L"MacAdrSetGvu :: in : (1) store GVU var index" },
	{	E_GrP3ApiWebCodeGet,			L"WebCodeGet :: in : (1) recive text register index" },
	{	E_GrP3ApiHddFormat,				L"HddFormat :: in : (1) hdd index, (2) 1=logformat" },
	{ E_GrP3ApiGroupHddFormat,	L"HddGroupFormat :: in : (1) group type, (2) 1=logformat" },
	{	E_GrP3ApiHddRecSizeGet,		L"HddRecSizeGet :: in : (1) hdd index :: out : (2) disk size, (3) record size" },
	{	E_GrP3ApiHddStatGet,			L"HddStatGet :: in : (1) hdd index :: out : (0) disk record status" },
	{	E_GrP3ApiGroupRecSizeGet,	L"GroupRecSizeGet :: in : (1) group index :: out : (2) total disk size, (3) total record size" },
	{	E_GrP3ApiGroupRecStatGet,	L"GroupRecStatGet :: in : (1) group index :: out : (0) group record status" },
	{	E_GrP3ApiGroupDayMapGet,	L"GroupRecDayMapGet :: in : (1) group index, (2) time, (3) day count :: out : (0) record day map" },
	{	E_GrP3ApiGroupTimeRngGet,	L"GroupTimeRngGet :: in : (1) group index :: out : (0) start time (1) end time" },
	{ E_GrP3ApiRecEmgcSet,			L"RecEmgcSet :: in : (1) 0=off,1=on" },
	{ E_GrP3ApiRecSizeExpect,		L"RecSizeExpect :: in : (1) group index (2) channel mask (3) start time (4) end time :: out (0) MByte unit size" },
	{	E_GrP3ApiRecCfgChk,				L"RecCfgChk :: out : (0) 0=good,1=bad" },
	{	E_GrP3ApiVrecStatGet,			L"VrecStatGet :: in : (1) channel :: out : (0) status" },
	{	E_GrP3ApiVrecFpsTextAdd,	L"VrecFpsTextAdd :: in : (1) console (2) window id (3) control id (4) channel" },
	{	E_GrP3ApiVrecFpsToStr,		L"VrecFpsToStr :: in : (1) channel (2) speed value :: out : (T0) speed text" },
	{	E_GrP3ApiVrecFpsCntGet,		L"VrecFpsCntGet :: (1) channel :: out : (0) count" },

	{ E_GrP3ApiCamAutoGainSet,	L"CamAutoGainSet :: in : (1) channel index, (2) 0=Manual,1=Auto " },
	{ E_GrP3ApiCamGainSet,			L"CamGainSet :: in : (1) channel index, (2) value" },
	{ E_GrP3ApiCamBriSet,				L"CamBriSet :: in : (1) channel index, (2) value" },
	{ E_GrP3ApiCamConSet,				L"CamConSet :: in : (1) channel index, (2) value" },
	{ E_GrP3ApiCamHueSet,				L"CamHueSet :: in : (1) channel index, (2) value" },
	{ E_GrP3ApiCamSatSet,				L"CamSatSet :: in : (1) channel index, (2) value" },
	{ E_GrP3ApiCamTypeGet,			L"CamTypeGet :: in : (1) channel index :: out : (0) camera type" },
	{ E_GrP3ApiCamTypeToStr,		L"CamTypeToStr :: in : (1) camera type :: out (T0) camera type text" },

	{ E_GrP3ApiPlayModeSet,			L"PlayModeSet :: in : (1) 0=live,1=play" },
	{ E_GrP3ApiPlayGroupSel,		L"PlayGroupSel :: in : (1) 0=normal,1=mirror" },
	{ E_GrP3ApiPlayGoTime,			L"PlayGoTime :: in : (1) time" },
	{ E_GrP3ApiPlaySpeedSet,		L"PlaySpeedSet :: in : (1) speed" },
	{ E_GrP3ApiPlayDirSet,			L"PlayDirSet :: in : (1) 0=normal,1=back" },
	{ E_GrP3ApiPlayDo,					L"PlayDo :: " },
	{ E_GrP3ApiPlayPuase,				L"PlayPause :: " },
	{ E_GrP3ApiPlayTimeGet,			L"PlayTimeGet :: out : (0) time (1) summer time" },
	{ E_GrP3ApiPlayGoFirst,			L"PlayGoFirst :: " },
	{ E_GrP3ApiPlayGoRecent,		L"PlayGoRecent :: " },
	{ E_GrP3ApiPlayNextFrm,			L"PlayNextFrm :: " },
	{ E_GrP3ApiPlayPrevFrm,			L"PlayPrevFrm :: " },

	{ E_GrP3ApiAdoLiveChSet,		L"Live audio channel set :: in : (1) channel, 254=off, 255=auto" },
	{ E_GrP3ApiAdoLiveVolSet,		L"Live audio volume set :: in : (1) channel (2) volume 0 ~ 100" },
	{ E_GrP3ApiAdoPlayChSet,		L"Play audio channel set :: in : (1) channel, 254=off, 255=auto" },
	{ E_GrP3ApiAdoPlayVolSet,		L"Play audio volume set :: in : (1) channel (2) volume 0 ~ 100" },

	{ E_GrP3ApiCtlgItemValGet,	L"CtlgItemValGet :: in : (1) handle, (2) element (3) add value :: out : (0) success , (1) result value" },
	{ E_GrP3ApiCtlgItemStrGet,	L"CtlgItemStrGet :: in : (1) handle, (2) element (3) add value :: out : (0) success , (T0) result string" },

	{ E_GrP3ApiFwUpgdRun,				L"FwUpgdRun :: in : (1) Catalog Handle :: out : (0) success" },

	{ E_GrP3ApiCfgFileLoad,			L"CfgFileLoad :: in : (1) Catalog Handle :: out : (0) success" },
	{ E_GrP3ApiCfgFileSave,			L"CfgFileSave :: in : (T0) file name :: out : (0) success" },

	{ E_GrP3ApiPtzPanTilt,			L"PtzPanTilt :: in : (1) channel (2) left (3) right (4) up (5) down  " },
	{ E_GrP3ApiPtzZoom,					L"PtzZoom :: in : (1) channel (2) 0=tele,1=wide " },
	{ E_GrP3ApiPtzFocus,				L"PtzFocus :: in : (1) channel (2) 0=near,1=far " },
	{ E_GrP3ApiPtzIris,					L"PtzIris :: in : (1) channel (2) 0=close,1=open " },
	{ E_GrP3ApiPtzStop,					L"PtzStop :: in : (1) channel" },
	{ E_GrP3ApiPtzAutoFocus,		L"PtzAutoFocus :: in : (1) channel" },
	{ E_GrP3ApiPtzMenuCall,			L"PtzMenuCall :: in : (1) channel" },
	{ E_GrP3ApiPtzMenuExit,			L"PtzMenuExit :: in : (1) channel" },
	{ E_GrP3ApiPtzMenuLeft,			L"PtzMenuLeft :: in : (1) channel" },
	{ E_GrP3ApiPtzMenuRight,		L"PtzMenuRight :: in : (1) channel" },
	{ E_GrP3ApiPtzMenuUp,				L"PtzMenuUp :: in : (1) channel" },
	{ E_GrP3ApiPtzMenuDown,			L"PtzMenuDown :: in : (1) channel" },
	{ E_GrP3ApiPtzMenuOk,				L"PtzMenuOk :: in : (1) channel" },
	{ E_GrP3ApiPtzMenuCancel,		L"PtzMenuCancel :: in : (1) channel" },
	{ E_GrP3ApiPtzPresetSet,		L"PtzPresetSet :: in : (1) channel (2) id" },
	{ E_GrP3ApiPtzPresetMove,		L"PtzPresetMove :: in : (1) channel (2) id" },
	{ E_GrP3ApiPtzTourRun,			L"PtzTourRun :: in : (1) channel (2) 0=stop,1=run" },
	{ E_GrP3ApiPtzTourSet,			L"PtzTourSet :: in : (1) channel " },
	{ E_GrP3ApiPtzIdView,				L"PtzIdView :: in : (1) channel (2) 0=off,1=on" },
	{ E_GrP3ApiPtzIdSet,				L"PtzIdSet :: in : (1) channel (2) new id" },
	{ E_GrP3ApiPtzModelTxtAdd,	L"PtzModelTxtAdd :: in : (1) console index (2) window id (3) control id" },
	{ E_GrP3ApiPtzCtrlLock,			L"PtzCtrlLock :: in : (1) 0=unlock, 1=lock" },
	{ E_GrP3ApiCtrlModelTxtAdd,	L"CtrlModelTxtAdd :: in : (1) console index (2) window id (3) control id" },

	{ E_GrP3ApiBkupMediaSizeGet,	L"BkupMediaSizeGet :: in : (1) 0=usb, 1=odd :: out : (0) Mbyte unit size" },
	{ E_GrP3ApiBkupRequest,			L"BkupRequest :: in : (1) group index (2) channel map (3) time start (4) time end (5) 0=usb 1=odd (6) viewer combine (T0) password :: out : (0) 0= fail 1= success" },
	{ E_GrP3ApiBkupCancel,			L"BkupCancel :: " },

	{ E_GrP3ApiNtpTest,					L"NtpTest :: " },

	{ E_GrP3ApiLogBkupRequest,	L"LogRequest :: in : (1) group index (2) channel map (3) event map (4) time start (5) time end (T0) file name :: out : (0) 0= fail 1= success" },
	{ E_GrP3ApiLogBkupCancel,		L"LogCancel :: " },

	{ E_GrP3ApiDoSet,						L"DoSet :: in : (1) do index (2) 0=auto, 1=off, 2= on " },
	{ E_GrP3ApiMtnSetChk,				L"motion set check :: in : (1) channel :: out : (0) motion set " },
	{ E_GrP3ApiBlockageChUpdt,	L"blockage ch update :: in : (1) channel " },
	{ E_GrP3ApiPwrOff,					L"Dvr Pwr Off" },

	
	{ E_GrP3ApiEmailTest,				L"Email Test :: " },
};

#endif



//====================================================================
//--------------------------------------------------------------------
void	GrP3ApiCfgLangUpdt( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		__u8*	Tv_PtrBt;
	// code --------------------
		Tv_PtrBt	=	(__u8*)V_GrP3Mng->SetupDataPtrGet( E_GrP3CfgOfsLang );
		V_GrP3Mng->LangSelSet( *Tv_PtrBt );
}
//--------------------------------------------------------------------
void	GrP3ApiCfgReset( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Ptr_GrP3ScptCfgMdl	Tv_PtrCfgMdl;
		void*		Tv_PtrCfg;
		__u32*	Tv_PtrDw;
		__u16*		Tv_PtrWd;
		St_GrP3MngVmRpara	Tv_Rpara;
	// code --------------------
		// init
		Tv_PtrCfg	=	(__u32*)V_GrP3Mng->SetupDataPtrGet( 0 );
		// check factory default
		if ( 0 != A_PtrRint[1] )
		{
			// reset all data
			GrDumyZeroMem( Tv_PtrCfg, E_GrP3MngSdaSize );
		}

		// build header
		// make FCC
		Tv_PtrDw	=	(__u32*)( (__u32)Tv_PtrCfg + E_GrP3CfgOfsFcc );
		*Tv_PtrDw	=	E_GrP3CfgFccId;
		// set firmware version
		Tv_PtrWd	=	(__u16*)( (__u32)Tv_PtrCfg + E_GrP3CfgOfsCfgVer );
		*Tv_PtrWd	=	(__u16)V_GrP3Mng->FirmwareVerGet();
		// model number 
		Tv_PtrWd	=	(__u16*)( (__u32)Tv_PtrCfg + E_GrP3CfgOfsModel );
		*Tv_PtrWd	=	(__u16)V_GrP3Mng->FirmwareVerGet();

		// quad map reset
		GrDvrQuadMapReset( V_GrP3Mng->SetupDataPtrGet( E_GrP3CfgOfsQuadMap), V_GrP3Mng->VdoMaxChCntGet() );

		// ADMIN privilege
		Tv_PtrWd	=	(__u16*)V_GrP3Mng->SetupDataPtrGet( E_GrP3CfgOfsUserAuth );
		*Tv_PtrWd	=	0xFFFF;

		// run configuration reset code
		Tv_PtrCfgMdl	=	V_GrP3Mng->ConfigMdlAtbGet();
		if ( E_GrP3AsmIdNone != Tv_PtrCfgMdl->AsmCfgReset )
		{
			Tv_Rpara.Cnt	=	1;
			Tv_Rpara.Ridx[0]	=	1;
			Tv_Rpara.Rval[0]	=	A_PtrRint[1];
			V_GrP3Mng->VmRunCode( Tv_PtrCfgMdl->AsmCfgReset, NULL, &Tv_Rpara );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiQuadModeNext( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		__u8	Tv_ConIdx;
		Cls_GrP3DoBase*	Tv_HostObj;
		Cls_GrP3DoWin*	Tv_HostWin;
		Cls_GrP3ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_ObjCon	=	NULL;

		Tv_ConIdx	=	(__u8)A_PtrRint[1];
		if ( E_GrP3MngConMaxCnt <= Tv_ConIdx )
		{
			Tv_HostObj	=	V_GrP3Mng->VmHostGet();
			if ( NULL != Tv_HostObj )
			{
				Tv_HostWin	=	(Cls_GrP3DoWin*)Tv_HostObj->WindowGet();
				Tv_ObjCon		=	(Cls_GrP3ConBase*)Tv_HostWin->ConsoleGet();
			}
		}
		else
		{
			Tv_ObjCon	=	V_GrP3Mng->ConsoleGet( Tv_ConIdx );
		}

		// check exist console
		if ( NULL != Tv_ObjCon )
		{
			Tv_ObjCon->QuadModeNext();
		}
		else
		{
			DbgMsgPrint( "GrP3ApiQuadModeNext - bad console index ( %d ).\n " , Tv_ConIdx );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiQuadModeSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		__u8	Tv_ConIdx;
		Cls_GrP3DoBase*	Tv_HostObj;
		Cls_GrP3DoWin*	Tv_HostWin;
		Cls_GrP3ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_ObjCon	=	NULL;

		Tv_ConIdx	=	(__u8)A_PtrRint[1];
		if ( E_GrP3MngConMaxCnt <= Tv_ConIdx )
		{
			Tv_HostObj	=	V_GrP3Mng->VmHostGet();
			if ( NULL != Tv_HostObj )
			{
				Tv_HostWin	=	(Cls_GrP3DoWin*)Tv_HostObj->WindowGet();
				Tv_ObjCon		=	(Cls_GrP3ConBase*)Tv_HostWin->ConsoleGet();
			}
		}
		else
		{
			Tv_ObjCon	=	V_GrP3Mng->ConsoleGet( Tv_ConIdx );
		}

		// check exist console
		if ( NULL != Tv_ObjCon )
		{
			Tv_ObjCon->QuadModeSet( (__u8)A_PtrRint[2], (__u8)A_PtrRint[3] );
		}
		else
		{
			DbgMsgPrint( "GrP3ApiQuadModeSet - bad console index ( %d ).\n " , Tv_ConIdx );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiQuadModeGet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		__u8	Tv_ConIdx;
		__u8	Tv_Mode;
		__u8	Tv_Page;
		Cls_GrP3DoBase*	Tv_HostObj;
		Cls_GrP3DoWin*	Tv_HostWin;
		Cls_GrP3ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_ObjCon	=	NULL;

		Tv_ConIdx	=	(__u8)A_PtrRint[1];
		if ( E_GrP3MngConMaxCnt <= Tv_ConIdx )
		{
			Tv_HostObj	=	V_GrP3Mng->VmHostGet();
			if ( NULL != Tv_HostObj )
			{
				Tv_HostWin	=	(Cls_GrP3DoWin*)Tv_HostObj->WindowGet();
				Tv_ObjCon		=	(Cls_GrP3ConBase*)Tv_HostWin->ConsoleGet();
			}
		}
		else
		{
			Tv_ObjCon	=	V_GrP3Mng->ConsoleGet( Tv_ConIdx );
		}

		// check exist console
		if ( NULL != Tv_ObjCon )
		{
			Tv_ObjCon->QuadModeGet( &Tv_Mode, &Tv_Page );
			A_PtrRint[0]	=	(__s32)Tv_Mode;
			A_PtrRint[1]	=	(__s32)Tv_Page;
		}
		else
		{
			DbgMsgPrint( "GrP3ApiQuadModeGet - bad console index ( %d ).\n " , Tv_ConIdx );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPlayModeSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
	// code --------------------
		V_GrP3Mng->RtlPlayModeSet( (BOOL8)A_PtrRint[1] );
}
//--------------------------------------------------------------------
void	GrP3ApiCfgAssign( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
		// call BIOS
		GrP3BiosCfgAssign( A_PtrRint, A_PtrRstr );
		// assign setup to p3 manager
		V_GrP3Mng->RtlCfgUpdate();
}
//--------------------------------------------------------------------
void	GrP3ApiFocusIdGet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrP3ConBase*	Tv_Con;
		Cls_GrP3DoBase*		Tv_Obj;
	// code --------------------
		A_PtrRint[0]	=	E_GrP3ScptIdNone;

		Tv_Con	=	V_GrP3Mng->ConsoleGet( (__u8)A_PtrRint[1] );
		if ( NULL != Tv_Con )
		{
			Tv_Obj	=	Tv_Con->FocusObjGet();
			if ( NULL != Tv_Obj )
			{
				A_PtrRint[0]	=	(__s32)Tv_Obj->ScptIdGet();
			}
		}
		
}
//--------------------------------------------------------------------
void	GrP3ApiCtlgItemValGet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
		A_PtrRint[0]	=	(__s32)V_GrP3Mng->CtlgItemValueGet( (__u32)A_PtrRint[1], (__u8)A_PtrRint[2], A_PtrRint[3], &A_PtrRint[1] );
}
//--------------------------------------------------------------------
void	GrP3ApiCtlgItemStrGet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
		A_PtrRint[0]	=	(__s32)V_GrP3Mng->CtlgItemStrGet( (__u32)A_PtrRint[1], (__u8)A_PtrRint[2], A_PtrRint[3], A_PtrRstr[0] );
}
//--------------------------------------------------------------------
void	GrP3ApiTimeGet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
	// code --------------------
		A_PtrRint[0]	=	(__s32)GrTimeGet( (__u32*)&A_PtrRint[1], (BOOL8*)&A_PtrRint[2] );
}
//--------------------------------------------------------------------
void	GrP3ApiUiAlphaUpdt( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		__u8*	Tv_PtrVal;
		__s32		Tv_Rint[2];
	// code --------------------
		Tv_PtrVal	=	(__u8*)V_GrP3Mng->SetupDataPtrGet( E_GrP3CfgOfsOsdAlpha );
#ifndef GR_P3_EDITOR
		V_GrP3Mng->UiAlpahSet( *Tv_PtrVal );
#endif
		Tv_Rint[1]	=	(__s32)*Tv_PtrVal;
		GrP3BiosHwAlphaUpdt( Tv_Rint, NULL );
}
//--------------------------------------------------------------------
void	GrP3ApiFwVerGet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
		A_PtrRint[0]	=	V_GrP3Mng->FirmwareVerGet();
}
//--------------------------------------------------------------------
void	GrP3ApiCfgDateFmtUpdt( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
		V_GrP3Mng->RtlDateDispFmtUpdt();
}
//--------------------------------------------------------------------
void	GrP3ApiQuadPageNext( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		__u8		Tv_ConIdx;
		Cls_GrP3DoBase*	Tv_HostObj;
		Cls_GrP3DoWin*	Tv_HostWin;
		Cls_GrP3ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_ObjCon		= NULL;
		Tv_ConIdx		= (__u8)A_PtrRint[1];

		if ( E_GrP3MngConMaxCnt <= Tv_ConIdx )
		{
			Tv_HostObj	=	V_GrP3Mng->VmHostGet();
			if ( NULL != Tv_HostObj )
			{
				Tv_HostWin	=	(Cls_GrP3DoWin*)Tv_HostObj->WindowGet();
				Tv_ObjCon		=	(Cls_GrP3ConBase*)Tv_HostWin->ConsoleGet();
			}
		}
		else
		{
			Tv_ObjCon	=	V_GrP3Mng->ConsoleGet( Tv_ConIdx );
		}

		if(NULL != Tv_ObjCon)
		{
			Tv_ObjCon->QuadPageNext();
		}
		else
		{
			DbgMsgPrint( "GrP3ApiQuadPageNext - bad console index ( %d ).\n " , Tv_ConIdx );
		}

}
//--------------------------------------------------------------------
void	GrP3ApiLogIn( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		__u32	Tv_Tidx1;
		__u32	Tv_Tidx2;
	// code --------------------
		A_PtrRint[0]	=	FALSE;
		Tv_Tidx1	=	(__u32)A_PtrRint[2];
		Tv_Tidx2	=	(__u32)A_PtrRint[3];
		if ( (E_GrP3AsmMaxTxtRegCnt > Tv_Tidx1) && (E_GrP3AsmMaxTxtRegCnt > Tv_Tidx2) )
		{
			if ( V_GrP3Mng->RtlLogIn( (__u8)A_PtrRint[1], A_PtrRstr[Tv_Tidx1], A_PtrRstr[Tv_Tidx2] ) )
			{
				A_PtrRint[0]	=	TRUE;
			}
		}
		else
		{
			DbgMsgPrint( "GrP3ApiLogIn - text register index over.\n" );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiLogOut( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
	// code --------------------
		V_GrP3Mng->RtlLogOut();
}
//--------------------------------------------------------------------
void	GrP3ApiPasswdRuleChk( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
	// code --------------------
		A_PtrRint[0]	=	FALSE;
		if ( V_GrP3Mng->RtlPasswdRuleChk() )
		{
			A_PtrRint[0]	=	TRUE;
		}
}
//--------------------------------------------------------------------
void	GrP3ApiRecEmgcSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
		V_GrP3Mng->RtlTryRecEmgc( (BOOL8)A_PtrRint[1] );
}
//--------------------------------------------------------------------
void	GrP3ApiNetModelSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Ptr_GrP3MngGvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet();
		Tv_PtrGvs->NetModel	=	(__u32)A_PtrRint[1];
}
//--------------------------------------------------------------------
void	GrP3ApiKeyBlockSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
		V_GrP3Mng->KeyBlockSet( (__u8)A_PtrRint[1] );
}
//--------------------------------------------------------------------
void	GrP3ApiLayerUpClose( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		St_GrP3MsgLayer	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Cmd			=	E_GrP3MsgCmdConLayUpClose;
		Tv_Msg.Hd.ConIdx	=	(__u8)A_PtrRint[1];
		Tv_Msg.Hd.WinId		=	E_GrP3WinIdNone;
		Tv_Msg.Hd.ObjId		=	E_GrP3ScptIdNone;
		Tv_Msg.Layer			=	(__u8)A_PtrRint[2];
		V_GrP3Mng->UiMsgPost( (Ptr_GrP3MsgHd)&Tv_Msg, sizeof(Tv_Msg) );

}
//--------------------------------------------------------------------
void	GrP3ApiAdoLiveChSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Ptr_GrP3MngGvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet();
		Tv_PtrGvs->AdoLiveCh	=	(__u8)A_PtrRint[1];
		GrP3BiosAdoLiveChSet( A_PtrRint, A_PtrRstr );
}
//--------------------------------------------------------------------
void	GrP3ApiAdoLiveVolSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Ptr_GrP3MngGvs	Tv_PtrGvs;
		__u8	Tv_Ch;
		__u8	Tv_Vol;
	// code --------------------
		Tv_Ch	=	(__u8)A_PtrRint[1];
		if ( E_GrDvrMaxChCnt > Tv_Ch )
		{
			Tv_Vol	=	(__u8)A_PtrRint[2];
			if ( 100 >= Tv_Vol )
			{
				Tv_PtrGvs	=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet();
				Tv_PtrGvs->AdoLiveVol[Tv_Ch]	=	Tv_Vol;
				GrP3BiosAdoLiveVolSet( A_PtrRint, A_PtrRstr );
			}
			else
			{
				DbgMsgPrint( "GrP3ApiAdoLiveVolSet - bad volume value %d \n", A_PtrRint[2] );
			}
		}
		else
		{
			DbgMsgPrint( "GrP3ApiAdoLiveVolSet - bad channel number %d \n", A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiAdoPlayChSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Ptr_GrP3MngGvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet();
		Tv_PtrGvs->AdoPlayCh	=	(__u8)A_PtrRint[1];
		GrP3BiosAdoPlayChSet( A_PtrRint, A_PtrRstr );
}
//--------------------------------------------------------------------
void	GrP3ApiAdoPlayVolSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Ptr_GrP3MngGvs	Tv_PtrGvs;
		__u8	Tv_Ch;
		__u8	Tv_Vol;
	// code --------------------
		Tv_Ch	=	(__u8)A_PtrRint[1];
		if ( E_GrDvrMaxChCnt > Tv_Ch )
		{
			Tv_Vol	=	(__u8)A_PtrRint[2];
			if ( 100 >= Tv_Vol )
			{
				Tv_PtrGvs	=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet();
				Tv_PtrGvs->AdoPlayVol[Tv_Ch]	=	Tv_Vol;
				GrP3BiosAdoPlayVolSet( A_PtrRint, A_PtrRstr );
			}
			else
			{
				DbgMsgPrint( "GrP3ApiAdoPlayVolSet - bad volume value %d \n", A_PtrRint[2] );
			}
		}
		else
		{
			DbgMsgPrint( "GrP3ApiAdoPlayVolSet - bad channel number %d \n", A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzPanTilt( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoPanTilt( (__u8)A_PtrRint[1], (BOOL8)A_PtrRint[2], (BOOL8)A_PtrRint[3], (BOOL8)A_PtrRint[4], (BOOL8)A_PtrRint[5],
				E_GrPtzV2DfltSpeed, E_GrPtzV2DfltSpeed, 0 );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzZoom( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoZoom( (__u8)A_PtrRint[1], (BOOL8)A_PtrRint[2], E_GrPtzV2DfltSpeed, 0 );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzFocus( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoFocus( (__u8)A_PtrRint[1], (BOOL8)A_PtrRint[2], E_GrPtzV2DfltSpeed, 0 );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzIris( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoIris( (__u8)A_PtrRint[1], (BOOL8)A_PtrRint[2], E_GrPtzV2DfltSpeed, 0 );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzStop( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoStop( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzAutoFocus( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoAutoFocus( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzMenuCall( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuCall( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzMenuExit( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuExit( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzMenuLeft( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuLeft( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzMenuRight( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuRight( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzMenuUp( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuUp( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzMenuDown( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuDown( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzMenuOk( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuOk( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzMenuCancel( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuCancel( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzPresetSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoPresetSet( (__u8)A_PtrRint[1], (__u8)A_PtrRint[2] );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzPresetMove( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoPresetMove( (__u8)A_PtrRint[1], (__u8)A_PtrRint[2], E_GrPtzV2DfltSpeed );
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzTourRun( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
		Ptr_GrP3MngGvs	Tv_PtrGvs;
		__u16		Tv_Msk;
		__u16		Tv_TourMsk;
		BOOL8		Tv_IsSaveMode;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		Tv_PtrGvs	=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet();

		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeTouringRun( (__u8)A_PtrRint[1], (__u8)A_PtrRint[2] );

			Tv_TourMsk		= Tv_PtrGvs->PtzTourMsk;
			Tv_IsSaveMode	= TRUE;
			if(0 != (__u8)A_PtrRint[2])
			{
				Tv_Msk = 1 << ((__u8)A_PtrRint[1]);
				Tv_TourMsk = Tv_PtrGvs->PtzTourMsk | Tv_Msk;
			}
			else
			{
				Tv_Msk = ~(1 << ((__u8)A_PtrRint[1]));
				Tv_TourMsk = Tv_PtrGvs->PtzTourMsk & Tv_Msk;

				if(Tv_TourMsk == Tv_PtrGvs->PtzTourMsk)
				{
					Tv_IsSaveMode		= FALSE;
				}
			}
			Tv_PtrGvs->PtzTourMsk = Tv_TourMsk;

			if(Tv_IsSaveMode)
			{
				GrP3BiosRgstSave(NULL, NULL);
			}
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzTourSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
		void*	Tv_PtrStp;
		__u8	Tv_Ch;
		__u16	Tv_Ofs;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_Ch	=	(__u8)A_PtrRint[1];
			if ( Tv_Ch < V_GrP3Mng->VdoMaxChCntGet() )
			{
				// calculate channel
				Tv_Ofs		=	( ((__u16)Tv_Ch) * 12 * 4) + E_GrP3CfgOfsPtzTourCfg;
				Tv_PtrStp	=	V_GrP3Mng->SetupDataPtrGet( Tv_Ofs );

				Tv_PtzMng->SafeTourSave( Tv_Ch, (Ptr_GrPtzV2TourItm)Tv_PtrStp, 12 );
			}
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzIdView( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
		__u32	Tv_Ch;
		__u8*	Tv_PtrStp;
		__u32	Tv_Baud;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_Ch			=	(__u32)A_PtrRint[1];
			if ( E_GrDvrMaxChCnt > Tv_Ch )
			{
				Tv_PtrStp	=	(__u8*)V_GrP3Mng->SetupDataPtrGet(0);
				Tv_Baud		=	9600;
				if ( E_GrP3CfgUartSpeedCnt > Tv_PtrStp[E_GrP3CfgOfsPtzBaud + Tv_Ch] )
				{
					Tv_Baud	=	V_GrP3UartSpeedTbl[Tv_PtrStp[E_GrP3CfgOfsPtzBaud + Tv_Ch]];
				}
				Tv_PtzMng->SafeExIdView( Tv_PtrStp[E_GrP3CfgOfsPtzType + Tv_Ch], Tv_PtrStp[E_GrP3CfgOfsPtzId + Tv_Ch],
					Tv_PtrStp[E_GrP3CfgOfsPtzPort + Tv_Ch], Tv_Baud, Tv_PtrStp[E_GrP3CfgOfsPtzDataBit + Tv_Ch],
					Tv_PtrStp[E_GrP3CfgOfsPtzParity + Tv_Ch], Tv_PtrStp[E_GrP3CfgOfsPtzStopBit + Tv_Ch], 
					(BOOL8)A_PtrRint[2] );
			}
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzIdSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
		__u32	Tv_Ch;
		__u8*	Tv_PtrStp;
		__u32	Tv_Baud;
	// code --------------------
		Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_Ch			=	(__u32)A_PtrRint[1];
			if ( E_GrDvrMaxChCnt > Tv_Ch )
			{
				Tv_PtrStp	=	(__u8*)V_GrP3Mng->SetupDataPtrGet(0);
				Tv_Baud		=	9600;
				if ( E_GrP3CfgUartSpeedCnt > Tv_PtrStp[E_GrP3CfgOfsPtzBaud + Tv_Ch] )
				{
					Tv_Baud	=	V_GrP3UartSpeedTbl[Tv_PtrStp[E_GrP3CfgOfsPtzBaud + Tv_Ch]];
				}
				Tv_PtzMng->SafeExIdSet( Tv_PtrStp[E_GrP3CfgOfsPtzType + Tv_Ch], Tv_PtrStp[E_GrP3CfgOfsPtzId + Tv_Ch],
					Tv_PtrStp[E_GrP3CfgOfsPtzPort + Tv_Ch], Tv_Baud, Tv_PtrStp[E_GrP3CfgOfsPtzDataBit + Tv_Ch],
					Tv_PtrStp[E_GrP3CfgOfsPtzParity + Tv_Ch], Tv_PtrStp[E_GrP3CfgOfsPtzStopBit + Tv_Ch], 
					(__u8)A_PtrRint[2] );
			}
		}
}
//--------------------------------------------------------------------
void	GrP3ApiPtzModelTxtAdd( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrP3ConBase*	Tv_Con;
		Cls_GrP3DoWin*		Tv_Win;
		Cls_GrP3DoBase*		Tv_Obj;
		WCHAR*	Tv_Rstr[2];
		__u8	Tv_ModelCnt;
		__u8	Tv_MdlIdx;
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_Con	=	V_GrP3Mng->ConsoleGet( (__u8)A_PtrRint[1] );
		if ( NULL != Tv_Con )
		{
			Tv_Win	=	Tv_Con->WinFindByScptId( (__u16)A_PtrRint[2] );
			if ( NULL != Tv_Win )
			{
				Tv_Obj	=	Tv_Win->FindSlaveObjByScptId( (__u16)A_PtrRint[3] );
				if ( NULL != Tv_Obj )
				{
					Tv_PtzMng	=	V_GrP3Mng->PtzMngGet();
					if ( NULL != Tv_PtzMng )
					{
						// update
						Tv_ModelCnt	=	Tv_PtzMng->ModelCntGet();
						for ( Tv_MdlIdx=0;Tv_MdlIdx <= Tv_ModelCnt;Tv_MdlIdx++ )
						{
							Tv_Rstr[0]	=	Tv_PtzMng->ModelNameGet( Tv_MdlIdx );
							Tv_Obj->RtlCommand( E_GrP3UpiTextAdd, A_PtrRint, Tv_Rstr );
						}
					}
				}
				else
				{
					DbgMsgPrint( "GrP3ApiPtzModelTxtAdd - bad object script id.\n", A_PtrRint[3] );
				}
			}
			else
			{
				DbgMsgPrint( "GrP3ApiPtzModelTxtAdd - bad window script id.\n", A_PtrRint[2] );
			}
		}
		else
		{
			DbgMsgPrint( "GrP3ApiPtzModelTxtAdd - bad console index.\n", A_PtrRint[1] );
		}

}
//--------------------------------------------------------------------
void	GrP3ApiCtrlModelTxtAdd( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Cls_GrP3ConBase*	Tv_Con;
		Cls_GrP3DoWin*		Tv_Win;
		Cls_GrP3DoBase*		Tv_Obj;
		WCHAR*	Tv_Rstr[2];
		Def_WstrTag	Tv_StrTmp;
		__u8	Tv_ModelCnt;
		__u8	Tv_MdlIdx;
		Cls_GrP3SioMng*	Tv_SioMng;
	// code --------------------
		Tv_Con	=	V_GrP3Mng->ConsoleGet( (__u8)A_PtrRint[1] );
		if ( NULL != Tv_Con )
		{
			Tv_Win	=	Tv_Con->WinFindByScptId( (__u16)A_PtrRint[2] );
			if ( NULL != Tv_Win )
			{
				Tv_Obj	=	Tv_Win->FindSlaveObjByScptId( (__u16)A_PtrRint[3] );
				if ( NULL != Tv_Obj )
				{
					Tv_SioMng	=	V_GrP3Mng->SioMngGet();
					if ( NULL != Tv_SioMng )
					{
						// update
						Tv_Rstr[0]	=	Tv_StrTmp;
						Tv_Rstr[1]	=	Tv_StrTmp;
						Tv_ModelCnt	=	Tv_SioMng->ModelCntGet();
						for ( Tv_MdlIdx=0;Tv_MdlIdx < Tv_ModelCnt;Tv_MdlIdx++ )
						{
							Tv_SioMng->ModelNameGet( Tv_MdlIdx, Tv_Rstr[0] );
							Tv_Obj->RtlCommand( E_GrP3UpiTextAdd, A_PtrRint, Tv_Rstr );
						}
					}
				}
				else
				{
					DbgMsgPrint( "GrP3ApiCtrlModelTxtAdd - bad object script id.\n", A_PtrRint[3] );
				}
			}
			else
			{
				DbgMsgPrint( "GrP3ApiCtrlModelTxtAdd - bad window script id.\n", A_PtrRint[2] );
			}
		}
		else
		{
			DbgMsgPrint( "GrP3ApiCtrlModelTxtAdd - bad console index.\n", A_PtrRint[1] );
		}

}
//--------------------------------------------------------------------
void	GrP3ApiRmtTmpAble( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Ptr_GrP3MngGvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet();
		Tv_PtrGvs->IsRmtTmpAble	=	(BOOL8)A_PtrRint[1];
}
//--------------------------------------------------------------------
void	GrP3ApiRmtIdSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Ptr_GrP3MngGvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet();
		Tv_PtrGvs->RunRmtId	=	(__u8)A_PtrRint[1];
}
//--------------------------------------------------------------------
void	GrP3ApiAutoSeqSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
		V_GrP3Mng->RtlDispAutoSeqSet( (BOOL8)A_PtrRint[1] );
#ifdef GR_P3_EDITOR
		if ( 0 == A_PtrRint[1] )
		{
			DbgMsgPrint( "Auto sequence off!\n" );
		}
		else
		{
			DbgMsgPrint( "Auto sequence on!\n" );
		}
#endif
}
//--------------------------------------------------------------------
void	GrP3ApiPtzCtrlLock( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Ptr_GrP3MngGvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet();
		
		Tv_PtrGvs->IsPtzCtrlLock	=	(BOOL8)A_PtrRint[1];
}
//--------------------------------------------------------------------
void	GrP3ApiUiMenuModeSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Ptr_GrP3MngGvs	Tv_PtrGvs;
		__s32		Tv_Rint[5];
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet();

		// check enter setup mode
		if ( (1 != Tv_PtrGvs->MenuMode) && (1 == A_PtrRint[1]) )
		{
			Tv_Rint[1]	=	E_GrDvrLogTypeInSetup;
			Tv_Rint[2]	=	0xFFFF;
			Tv_Rint[3]	=	(__s32)Tv_PtrGvs->LoginIdx;
			Tv_Rint[4]	=	0;
			GrP3BiosEvtLogAdd( Tv_Rint, NULL );
		}
		// check leave setup mode
		if ( (1 == Tv_PtrGvs->MenuMode) && (1 != A_PtrRint[1]) )
		{
			Tv_Rint[1]	=	E_GrDvrLogTypeOutSetup;
			Tv_Rint[2]	=	0xFFFF;
			Tv_Rint[3]	=	(__s32)Tv_PtrGvs->LoginIdx;
			Tv_Rint[4]	=	0;
			GrP3BiosEvtLogAdd( Tv_Rint, NULL );
		}

		// update menu mode
		Tv_PtrGvs->MenuMode	=	(__u8)A_PtrRint[1];
		DbgMsgPrint( "MenuMode = %d\n", Tv_PtrGvs->MenuMode );
}
//--------------------------------------------------------------------
void	GrP3ApiUiLoginModeSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Ptr_GrP3MngGvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet();

		Tv_PtrGvs->IsLoginMenu	=	(BOOL8)A_PtrRint[1];

		DbgMsgPrint( "LoginMode = %d\n", Tv_PtrGvs->IsLoginMenu );
}
//--------------------------------------------------------------------
void	GrP3ApiUiMsgBoxModeSet( __s32* A_PtrRint, WCHAR** A_PtrRstr )
{
	// local -------------------
		Ptr_GrP3MngGvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP3MngGvs)V_GrP3Mng->GvsPtrGet();

		Tv_PtrGvs->MsgBoxMode	=	(__u8)A_PtrRint[1];

		DbgMsgPrint( "MsgBoxMode = %d\n", Tv_PtrGvs->MsgBoxMode );
}
//--------------------------------------------------------------------

