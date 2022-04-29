/*
	GAUSE Platform API base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3ApiBase
#define	_EPC_GrP3ApiBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3AsmBase.h>

//====================================================================
//constance

#define E_GrP3ApiCnt								118

#define E_GrP3ApiQuit								0			/* system quit - set GVS quit mode   */

#define E_GrP3ApiTimeGet						1			/* time get - reg0 = time */
#define E_GrP3ApiTimeSet						2			/* time set - reg1 = time */
#define E_GrP3ApiCfgLoadFromRom			3			/* configuration load from ROM  */
#define E_GrP3ApiCfgSaveToRom				4			/* configuration save to ROM  */
#define E_GrP3ApiCfgRestore					5			/* configuration restore from template buffer  */
#define E_GrP3ApiCfgAllUpdt					6			/* configuration all update */
#define E_GrP3ApiCfgLoadFromFile		7			/* configuration load from file - reg1 = file name buffer index */
#define E_GrP3ApiCfgSaveToFile			8			/* configuration save to file - ireg0 = result, treg0 = file name */
#define E_GrP3ApiCfgReset						9			/* configuration reset ASM function call - ireg1 : 0 = normal, 1 = factory */
#define E_GrP3ApiCfgLangUpdt				10		/* configuration language update */
#define E_GrP3ApiCfgIsChanged				11		/* configuration check changed */
#define E_GrP3ApiCfgAssign					12		/* configuration assign to system */
#define E_GrP3ApiCfgAlphaUpdt				13		/* configuration alpha update */
#define E_GrP3ApiFwVerGet						14		/* firmware version get */
//#define E_GrP3ApiModelGet						15		/* model number get */
//#define E_GrP3ApiHwVerGet						16		/* hardware version get */
#define E_GrP3ApiCfgDateFmtUpdt			17		/* date display format update */
#define E_GrP3ApiLogIn							18		/* log in */
#define E_GrP3ApiLogOut							19		/* log out */

#define E_GrP3ApiQuadModeNext				20		/* quad mode next - reg1=console index(255=this) */
#define E_GrP3ApiQuadPageNext				21		/* quad page next - reg1=console index(255=this) */
#define E_GrP3ApiQuadModeSet				22		/* quad mode set - reg1=console index(255=this), reg2=mode, reg3=page */
#define E_GrP3ApiQuadModeGet				23		/* quad mode get - in: reg1=console index(255=this), out: reg0=mode, reg1=page */
#define E_GrP3ApiQuadLock						24		/* quad lock */
#define E_GrP3ApiQuadUnlock					25		/* quad unlock */
#define E_GrP3ApiAutoSeqSet					26		/* auto sequence set */
#define E_GrP3ApiDispResTextAdd			27		/* display resolution text add to control */
#define E_GrP3ApiDispResToStr				28		/* display resolution value to string */
#define E_GrP3ApiDispResCntGet			29		/* display resolution cont get */

#define E_GrP3ApiFocusIdGet					30		/* focus object id get - reg1=console index */
#define E_GrP3ApiKeyBlockSet				31		/* key block set - in: 1) block key code */
#define E_GrP3ApiLayerUpClose				32		/* upper layer window close - in: 1) console index 2) layer number */
#define E_GrP3ApiMenuModeSet				33		/* UI menu mode set */
#define E_GrP3ApiLoginModeSet				34		/* UI login mode set */
#define E_GrP3ApiMsgBoxMode					35		/* message box mode set */

#define E_GrP3ApiMacAdrGetGvu				40		/* mac address get */
#define E_GrP3ApiMacAdrSetGvu				41		/* mac address set */
#define E_GrP3ApiWebCodeGet					42		/* web code get */

#define E_GrP3ApiHddFormat					50		/* HDD format */
#define E_GrP3ApiHddRecSizeGet			51		/* HDD record size get */
#define E_GrP3ApiHddStatGet					52		/* HDD status get */
#define E_GrP3ApiGroupRecSizeGet		53		/* group record size get */
#define E_GrP3ApiGroupRecStatGet		54		/* group record status get */
#define E_GrP3ApiGroupDayMapGet			55		/* group record day map get */
#define E_GrP3ApiRecEmgcSet					56		/* record emergency set */
#define E_GrP3ApiGroupTimeRngGet		57		/* group time range get */
#define E_GrP3ApiRecSizeExpect			58		/* record size expect */
#define E_GrP3ApiRecCfgChk					59		/* record configuration check */

#define E_GrP3ApiPlayModeSet				60		/* display play mode set */
#define E_GrP3ApiPlayGroupSel				61		/* play record group set */
#define E_GrP3ApiPlayGoTime					62		/* play go time */
#define E_GrP3ApiPlaySpeedSet				63		/* play speed set */
#define E_GrP3ApiPlayDirSet					64		/* play direction set */
#define E_GrP3ApiPlayDo							65		/* do play */
#define E_GrP3ApiPlayPuase					66		/* pause play */
#define E_GrP3ApiPlayTimeGet				67		/* now play time get */
#define E_GrP3ApiPlayGoFirst				68		/* play go first */
#define E_GrP3ApiPlayGoRecent				69		/* play go recent */
#define E_GrP3ApiPlayNextFrm				70		/* play next frame */
#define E_GrP3ApiPlayPrevFrm				71		/* play previous frame */

#define E_GrP3ApiCamBriSet					80		/* camera brightness set */
#define E_GrP3ApiCamConSet					81		/* camera contrast set */
#define E_GrP3ApiCamHueSet					82		/* camera hue set */
#define E_GrP3ApiCamSatSet					83		/* camera saturation set */
#define E_GrP3ApiCamGainSet					84		/* camera gain set */
#define E_GrP3ApiCamAutoGainSet			85		/* camera auto gain set */
#define E_GrP3ApiCamTypeGet					86		/* camera type get */
#define E_GrP3ApiCamTypeToStr				87		/* camera type to text */

#define E_GrP3ApiCtlgItemValGet			90		/* catalog item value get */
#define E_GrP3ApiCtlgItemStrGet			91		/* catalog item string get */

#define E_GrP3ApiFwUpgdRun					100		/* firmware upgrade run */

#define E_GrP3ApiCfgFileLoad				102		/* configuration file load */
#define E_GrP3ApiCfgFileSave				103		/* configuration file save */

#define E_GrP3ApiPasswdRuleChk			120		/* password rule check */
//#define E_GrP3ApiRtlCfgSave					121		/* runtime configuration save */
#define E_GrP3ApiEvtLogAdd					122		/* event log add add */
//#define E_GrP3ApiHdayDateDel				123		/* holiday date delete */
//#define E_GrP3ApiHdayCtlgDel				124		/* holiday catalog delete */
#define E_GrP3ApiDipSwGet						125		/* Dip switch get */
#define E_GrP3ApiNetModelSet				126		/* network model number set */
#define E_GrP3ApiRmtTmpAbl					127		/* REMOCON temporary input able */
#define E_GrP3ApiRmtIdSet						128		/* REMOCON running ID set */
#define E_GrP3ApiNtpTest						129		/* NTP test request */

#define E_GrP3ApiAdoLiveChSet				130		/* audio live channel set */
#define E_GrP3ApiAdoLiveVolSet			131		/* audio live volume set */
#define E_GrP3ApiAdoPlayChSet				132		/* audio play channel set */
#define E_GrP3ApiAdoPlayVolSet			133		/* audio play volume set */

#define E_GrP3ApiPtzPanTilt					140		/* PTZ pan tilt */
#define E_GrP3ApiPtzZoom						141		/* PTZ zoom */
#define E_GrP3ApiPtzFocus						142		/* PTZ focus */
#define E_GrP3ApiPtzIris						143		/* PTZ IRIS */
#define E_GrP3ApiPtzStop						144		/* PTZ stop */
#define E_GrP3ApiPtzAutoFocus				145		/* PTZ auto focus */
#define E_GrP3ApiPtzMenuCall				146		/* PTZ menu call */
#define E_GrP3ApiPtzMenuExit				147		/* PTZ menu exit */
#define E_GrP3ApiPtzMenuLeft				148		/* PTZ menu left */
#define E_GrP3ApiPtzMenuRight				149		/* PTZ menu right */
#define E_GrP3ApiPtzMenuUp					150		/* PTZ menu up */
#define E_GrP3ApiPtzMenuDown				151		/* PTZ menu down */
#define E_GrP3ApiPtzMenuOk					152		/* PTZ menu ok */
#define E_GrP3ApiPtzMenuCancel			153		/* PTZ menu cancel */
#define E_GrP3ApiPtzPresetSet				154		/* PTZ preset set */
#define E_GrP3ApiPtzPresetMove			155		/* PTZ preset move */
#define E_GrP3ApiPtzTourRun					156		/* PTZ tour run */
#define E_GrP3ApiPtzIdView					157		/* PTZ ID view */
#define E_GrP3ApiPtzIdSet						158		/* PTZ ID set */
#define E_GrP3ApiPtzModelTxtAdd			159		/* PTZ model text add */
#define E_GrP3ApiPtzCtrlLock				160		/* PTZ control lock */
#define E_GrP3ApiCtrlModelTxtAdd		161		/* controller model text add */
#define E_GrP3ApiPtzTourSet					162		/* PTZ tour set */

#define E_GrP3ApiBkupMediaSizeGet		170		/* backup media free size get */		
#define E_GrP3ApiBkupRequest				171		/* backup request */
#define E_GrP3ApiBkupCancel					172		/* backup cancel */

#define E_GrP3ApiLogBkupRequest			175		/* log backup request */
#define E_GrP3ApiLogBkupCancel			176		/* log backup cancel */


#define E_GrP3ApiErrBuzOff					180		/* error buzzer off */
#define E_GrP3ApiDoSet							181		/* Digital output set */

#define E_GrP3ApiMtnSetChk					190		/* motion setting check */
#define E_GrP3ApiBlockageChUpdt			191		/* blockage upate channel */
#define E_GrP3ApiPwrOff							192		/* dvr power off  */

#define E_GrP3ApiGroupHddFormat			200		/* Hdd Group format */
#define E_GrP3ApiEmailTest					201		/* Email test */
#define	E_GrP3ApiVrecStatGet				202		/* video record status get */
#define	E_GrP3ApiVrecFpsTextAdd			203		/* video record speed text add to control */
#define	E_GrP3ApiVrecFpsToStr				204		/* video record speed to text */
#define	E_GrP3ApiVrecFpsCntGet			205		/* video record speed count get */

//====================================================================
//global macro

//====================================================================
//global type

typedef	void	(*Fnc_GrP3Api)	(__s32* A_PtrRint, WCHAR** A_PtrRstr);

typedef	struct	St_GrP3ApiTbl
{
	__u32	Code;
	Fnc_GrP3Api	Fnc;
}	*Ptr_GrP3ApiTbl;

//====================================================================
//global function

void	GrP3ApiTimeGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiCfgLangUpdt( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiCfgReset( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiCfgAssign( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiCfgDateFmtUpdt( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiNetModelSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiRecEmgcSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiLogIn( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiLogOut( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPasswdRuleChk( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiQuadModeNext( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiQuadPageNext( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiQuadModeSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiQuadModeGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiAutoSeqSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiFwVerGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiRmtTmpAble( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiRmtIdSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiFocusIdGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiKeyBlockSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiLayerUpClose( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiPlayModeSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiCtlgItemValGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiCtlgItemStrGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiUiAlphaUpdt( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiUiMenuModeSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiUiLoginModeSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiUiMsgBoxModeSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiAdoLiveChSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiAdoLiveVolSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiAdoPlayChSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiAdoPlayVolSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3ApiPtzPanTilt( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzZoom( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzFocus( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzIris( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzStop( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzAutoFocus( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzMenuCall( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzMenuExit( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzMenuLeft( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzMenuRight( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzMenuUp( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzMenuDown( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzMenuOk( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzMenuCancel( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzPresetSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzPresetMove( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzTourRun( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzIdView( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzIdSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzModelTxtAdd( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzCtrlLock( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiPtzTourSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3ApiCtrlModelTxtAdd( __s32* A_PtrRint, WCHAR** A_PtrRstr );

//====================================================================
//out Environment conditional
#endif

