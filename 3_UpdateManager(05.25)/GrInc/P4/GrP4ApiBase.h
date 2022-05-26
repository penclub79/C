/*
	GAUSE Platform API base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4ApiBase
#define	_EPC_GrP4ApiBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4AsmBase.h>

//====================================================================
//constance

#define	E_GrP4ApiTblCnt							256

#define E_GrP4ApiCnt								145

#define E_GrP4ApiQuit								0			/* system quit - set GVS quit mode   */

#define E_GrP4ApiTimeGet						1			/* time get - reg0 = time */
#define E_GrP4ApiTimeSet						2			/* time set - reg1 = time */
#define E_GrP4ApiCfgLoadFromRom			3			/* configuration load from ROM  */
#define E_GrP4ApiCfgSaveToRom				4			/* configuration save to ROM  */
#define E_GrP4ApiCfgRestore					5			/* configuration restore from template buffer  */
#define E_GrP4ApiCfgMtnMapUpdt			6			/* motion map update */
#define E_GrP4ApiCfgPrivacyMapUpdt	7			/* privacy map update */
//#define E_GrP4ApiCfgSaveToFile			8			/* configuration save to file - ireg0 = result, treg0 = file name */
#define E_GrP4ApiCfgReset						9			/* configuration reset ASM function call - ireg1 : 0 = normal, 1 = factory */
#define E_GrP4ApiCfgLangUpdt				10		/* configuration language update */
#define E_GrP4ApiCfgIsChanged				11		/* configuration check changed */
#define E_GrP4ApiCfgAssign					12		/* configuration assign to system */
#define E_GrP4ApiCfgAlphaUpdt				13		/* configuration alpha update */
#define E_GrP4ApiFwVerGet						14		/* firmware version get */
//#define E_GrP4ApiModelGet						15		/* model number get */
//#define E_GrP4ApiHwVerGet						16		/* hardware version get */
#define E_GrP4ApiCfgDateFmtUpdt			17		/* date display format update */
#define E_GrP4ApiLogIn							18		/* log in */
#define E_GrP4ApiLogOut							19		/* log out */

#define E_GrP4ApiQuadModeNext				20		/* quad mode next - reg1=console index(255=this) */
#define E_GrP4ApiQuadPageNext				21		/* quad page next - reg1=console index(255=this) */
#define E_GrP4ApiQuadModeSet				22		/* quad mode set - reg1=console index(255=this), reg2=mode, reg3=page */
#define E_GrP4ApiQuadModeGet				23		/* quad mode get - in: reg1=console index(255=this), out: reg0=mode, reg1=page */
#define E_GrP4ApiQuadLock						24		/* quad lock */
#define E_GrP4ApiQuadUnlock					25		/* quad unlock */
#define E_GrP4ApiAutoSeqSet					26		/* auto sequence set */
#define E_GrP4ApiDispResTextAdd			27		/* display resolution text add to control */
#define E_GrP4ApiDispResToStr				28		/* display resolution value to string */
#define E_GrP4ApiDispResCntGet			29		/* display resolution cont get */

#define E_GrP4ApiFocusIdGet					30		/* focus object id get - reg1=console index */
#define E_GrP4ApiKeyBlockSet				31		/* key block set - in: 1) block key code */
#define E_GrP4ApiLayerUpClose				32		/* upper layer window close - in: 1) console index 2) layer number */
#define E_GrP4ApiMenuModeSet				33		/* UI menu mode set */
#define E_GrP4ApiLoginModeSet				34		/* UI login mode set */
#define E_GrP4ApiMsgBoxMode					35		/* message box mode set - will intend other function */

#define E_GrP4ApiMacAdrGetGvu				40		/* mac address get - !!!other function intend */
#define E_GrP4ApiMacAdrSetGvu				41		/* mac address set */
#define E_GrP4ApiWebCodeGet					42		/* web code get */
#define E_GrP4ApiLanCableStat				43		/* get lan cable status */

#define E_GrP4ApiHddFormat					50		/* HDD format */
#define E_GrP4ApiHddRecSizeGet			51		/* HDD record size get */
#define E_GrP4ApiHddStatGet					52		/* HDD status get */
#define E_GrP4ApiGroupRecSizeGet		53		/* group record size get */
#define E_GrP4ApiGroupRecStatGet		54		/* group record status get */
#define E_GrP4ApiGroupDayMapGet			55		/* group record day map get */
#define E_GrP4ApiRecEmgcSet					56		/* record emergency set */
#define E_GrP4ApiGroupTimeRngGet		57		/* group time range get */
#define E_GrP4ApiRecSizeExpect			58		/* record size expect */
#define E_GrP4ApiRecCfgChk					59		/* record configuration check */

#define E_GrP4ApiPlayModeSet				60		/* display play mode set */
#define E_GrP4ApiPlayGroupSel				61		/* play record group set */
#define E_GrP4ApiPlayGoTime					62		/* play go time */
#define E_GrP4ApiPlaySpeedSet				63		/* play speed set */
#define E_GrP4ApiPlayDirSet					64		/* play direction set */
#define E_GrP4ApiPlayDo							65		/* do play */
#define E_GrP4ApiPlayPuase					66		/* pause play */
#define E_GrP4ApiPlayTimeGet				67		/* now play time get */
#define E_GrP4ApiPlayGoFirst				68		/* play go first */
#define E_GrP4ApiPlayGoRecent				69		/* play go recent */
#define E_GrP4ApiPlayNextFrm				70		/* play next frame */
#define E_GrP4ApiPlayPrevFrm				71		/* play previous frame */

#define E_GrP4ApiCamBriSet					80		/* camera brightness set */
#define E_GrP4ApiCamConSet					81		/* camera contrast set */
#define E_GrP4ApiCamHueSet					82		/* camera hue set */
#define E_GrP4ApiCamSatSet					83		/* camera saturation set */
#define E_GrP4ApiCamGainSet					84		/* camera gain set */
#define E_GrP4ApiCamAutoGainSet			85		/* camera auto gain set */
#define E_GrP4ApiCamTypeGet					86		/* camera type get */
#define E_GrP4ApiCamTypeToStr				87		/* camera type to text */

#define E_GrP4ApiCtlgItemValGet			90		/* catalog item value get - !!!will instead other function */
#define E_GrP4ApiCtlgItemStrGet			91		/* catalog item string get - !!!will instead other function */

#define E_GrP4ApiFwUpgdRun					100		/* firmware upgrade run */
#define E_GrP4ApiMtnViewMapUpdt			101		/* motion view map update */
#define E_GrP4ApiCfgFileLoad				102		/* configuration file load */
#define E_GrP4ApiCfgFileSave				103		/* configuration file save */
#define E_GrP4ApiFwUpgdSpcRun				104		/* firmware upgrade special run */

#define E_GrP4ApiPasswdRuleChk			120		/* password rule check */
//#define E_GrP4ApiRtlCfgSave					121		/* runtime configuration save */
#define E_GrP4ApiEvtLogAdd					122		/* event log add add */
//#define E_GrP4ApiHdayDateDel				123		/* holiday date delete */
//#define E_GrP4ApiHdayCtlgDel				124		/* holiday catalog delete */
//#define E_GrP4ApiDipSwGet						125		/* Dip switch get */
#define E_GrP4ApiNetModelSet				126		/* network model number set - !!!will move to model setting */
#define E_GrP4ApiRmtTmpAbl					127		/* REMOCON temporary input able */
#define E_GrP4ApiRmtIdSet						128		/* REMOCON running ID set */
#define E_GrP4ApiNtpTest						129		/* NTP test request */

#define E_GrP4ApiAdoLiveChSet				130		/* audio live channel set */
#define E_GrP4ApiAdoLiveVolSet			131		/* audio live volume set */
#define E_GrP4ApiAdoPlayChSet				132		/* audio play channel set */
#define E_GrP4ApiAdoPlayVolSet			133		/* audio play volume set */

#define E_GrP4ApiPtzPanTilt					140		/* PTZ pan tilt */
#define E_GrP4ApiPtzZoom						141		/* PTZ zoom */
#define E_GrP4ApiPtzFocus						142		/* PTZ focus */
#define E_GrP4ApiPtzIris						143		/* PTZ IRIS */
#define E_GrP4ApiPtzStop						144		/* PTZ stop */
#define E_GrP4ApiPtzAutoFocus				145		/* PTZ auto focus */
#define E_GrP4ApiPtzMenuCall				146		/* PTZ menu call */
#define E_GrP4ApiPtzMenuExit				147		/* PTZ menu exit */
#define E_GrP4ApiPtzMenuLeft				148		/* PTZ menu left */
#define E_GrP4ApiPtzMenuRight				149		/* PTZ menu right */
#define E_GrP4ApiPtzMenuUp					150		/* PTZ menu up */
#define E_GrP4ApiPtzMenuDown				151		/* PTZ menu down */
#define E_GrP4ApiPtzMenuOk					152		/* PTZ menu ok */
#define E_GrP4ApiPtzMenuCancel			153		/* PTZ menu cancel */
#define E_GrP4ApiPtzPresetSet				154		/* PTZ preset set */
#define E_GrP4ApiPtzPresetMove			155		/* PTZ preset move */
#define E_GrP4ApiPtzTourRun					156		/* PTZ tour run */
#define E_GrP4ApiPtzIdView					157		/* PTZ ID view */
#define E_GrP4ApiPtzIdSet						158		/* PTZ ID set */
#define E_GrP4ApiPtzModelTxtAdd			159		/* PTZ model text add */
#define E_GrP4ApiPtzCtrlLock				160		/* PTZ control lock */
#define E_GrP4ApiCtrlModelTxtAdd		161		/* controller model text add */
#define E_GrP4ApiPtzTourSet					162		/* PTZ tour set */

#define E_GrP4ApiBkupMediaSizeGet		170		/* backup media free size get */		
#define E_GrP4ApiBkupRequest				171		/* backup request */
#define E_GrP4ApiBkupCancel					172		/* backup cancel */

#define E_GrP4ApiLogBkupRequest			175		/* log backup request */
#define E_GrP4ApiLogBkupCancel			176		/* log backup cancel */


#define E_GrP4ApiErrBuzOff					180		/* error buzzer off */
#define E_GrP4ApiDoSet							181		/* Digital output set */
#define E_GrP4ApiBuzRun							182		/* buzzer run */

#define E_GrP4ApiMtnSetChk					190		/* motion setting check */
#define E_GrP4ApiPrivacyChUpdt			191		/* Privacy upate channel */
#define E_GrP4ApiPwrOff							192		/* dvr power off  */
#define E_GrP4ApiRgstLoad						193		/* registry load */
#define E_GrP4ApiRgstSave						194		/* registry save */
#define E_GrP4ApiMtnDmsInfo					196		/* motion demansion info */
#define E_GrP4ApiMtnSenGet					197		/* motion sense get */
#define E_GrP4ApiMtnSenSet					198		/* motion sense set */
#define E_GrP4ApiMtnViewMapGet			199		/* motion view map pointer get */

#define E_GrP4ApiGroupHddFormat			200		/* Hdd Group format */
#define E_GrP4ApiEmailTest					201		/* Email test */
#define	E_GrP4ApiVrecStatGet				202		/* video record status get */
#define	E_GrP4ApiVrecFpsTextAdd			203		/* video record speed text add to control */
#define	E_GrP4ApiVrecFpsToStr				204		/* video record speed to text */
#define	E_GrP4ApiVrecFpsCntGet			205		/* video record speed count get */

#define	E_GrP4ApiCtlgFwUpgdLoad			210		/* catalog firmware list load */
#define	E_GrP4ApiCtlgFwUpgdItemValGet	211		/* catalog firmware list value get */
#define	E_GrP4ApiCtlgFwUpgdItemStrGet	212		/* catalog firmware list string get */
#define	E_GrP4ApiCtlgFwUpgdNext			213		/* catalog firmware list page next */
#define	E_GrP4ApiCtlgFwUpgdPrev			214		/* catalog firmware list page previous */
#define	E_GrP4ApiCtlgCfgLoad				215		/* catalog cfg file list load */
#define	E_GrP4ApiCtlgCfgItemValGet	216		/* catalog cfg file list value get */
#define	E_GrP4ApiCtlgCfgItemStrGet	217		/* catalog cfg file list string get */
#define	E_GrP4ApiCtlgCfgNext				218		/* catalog cfg file list next */
#define	E_GrP4ApiCtlgCfgPrev				219		/* catalog cfg file list previous */
#define	E_GrP4ApiCtlgLogLoad				220		/* catalog event log list load */
#define	E_GrP4ApiCtlgLogItemValGet	221		/* catalog event log list value get */
#define	E_GrP4ApiCtlgLogItemStrGet	222		/* catalog event log list string get */
#define	E_GrP4ApiCtlgLogNext				223		/* catalog event log list next */
#define	E_GrP4ApiCtlgLogPrev				224		/* catalog event log list previous */

#define	E_GrP4ApiModeSet					225		/* mode set */

//====================================================================
//global macro

//====================================================================
//global type

typedef	void	(*Fnc_GrP4Api)	(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

//====================================================================
//global function

BOOL8	GrP4ApiCall(__u16 A_ApiId, __s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
BOOL8	GrP4ApiRegist( __u16 A_ApiId, Fnc_GrP4Api A_FncPtr );
Fnc_GrP4Api	GrP4ApiPtrGet( __u16 A_ApiId );


//====================================================================
//out Environment conditional
#endif

