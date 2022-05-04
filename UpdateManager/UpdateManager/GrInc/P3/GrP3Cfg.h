/*
	GAUSE Platform system configuration

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3Cfg
#define	_EPC_GrP3Cfg

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance

#define E_GrP3CfgHolyDayTime				(3840 * 24 * 60 * 60)
#define E_GrP3CfgHoliSchdCnt				4					/* max holiday schedule count */
#define E_GrP3CfgHoliDayCnt					256
#define E_GrP3CfgRschdUnitMin				30				/* 30 minute unit */
#define E_GrP3CfgRschdAtbDayCnt			48
#define E_GrP3CfgMtnSchdUnit				24
#define E_GrP3CfgMtnValMax					9

#define E_GrP3CfgUartMax						4					/* UART max count */
#define E_GrP3CfgUartSpeedCnt				5

#define E_GrP3CfgFirewallCnt				3					/* firewall item count */

#define E_GrP3CfgCamTitleLen				16

#define E_GrP3CfgAuthSetup					0x0001		/* authority setup  */
#define E_GrP3CfgAuthHdd						0x0002		/* authority HDD  */
#define E_GrP3CfgAuthSearch					0x0004		/* authority search */
#define E_GrP3CfgAuthBackup					0x0008		/* authority backup */
#define E_GrP3CfgAuthPtzCtrl				0x0010		/* authority PTZ */
#define E_GrP3CfgAuthReckey					0x0020		/* authority record key */
#define E_GrP3CfgAuthPtzOsd					0x0040		/* authority PTZ OSD */
#define E_GrP3CfgAuthNetwork				0x0080		/* authority network */

#define E_GrP3CfgHdayBaseYear				2012
#define E_GrP3CfgHdayYearCnt				4					/* holiday year count */

#define E_GrP3CfgFccId							Mac_GrMakeFcc('P','3','C','F')

#define E_GrP3CfgOfsFcc							0x0000		/* __u32	::	fourcc code, check current configuration */
#define E_GrP3CfgOfsCfgVer					0x0004		/* __u16	::	firmware version */
#define E_GrP3CfgOfsModel						0x0006		/* __u16	::	model number */

#define E_GrP3CfgOfsVdoType					0x0008		/* __u8	::	signal type , ntsc , pal */
#define E_GrP3CfgOfsRmtId						0x0009		/* __u8	::	remocon id */
#define E_GrP3CfgOfsLang						0x000A		/* __u8	::	language */
#define E_GrP3CfgOfsMseRes					0x000B		/* __u8	::	mouse resolution */
#define E_GrP3CfgOfsDateFmt					0x000C		/* __u8	::	date format */
#define E_GrP3CfgOfsIsDst						0x000D		/* BOOL8	::	daylight saving time use */
#define E_GrP3CfgOfsDstStMon				0x000E		/* __u8	::	daylight saving start month */
#define E_GrP3CfgOfsDstStWkTh				0x000F		/* __u8	::	daylight saving start week th */
#define E_GrP3CfgOfsDstStWkDay			0x0010		/* __u8	::	daylight saving start week day */
#define E_GrP3CfgOfsDstStHour				0x0011		/* __u8	::	daylight saving start hour */
#define E_GrP3CfgOfsDstEdMon				0x0012		/* __u8	::	daylight saving end month */
#define E_GrP3CfgOfsDstEdWkTh				0x0013		/* __u8	::	daylight saving end week th */
#define E_GrP3CfgOfsDstEdWkDay			0x0014		/* __u8	::	daylight saving end week day */
#define E_GrP3CfgOfsDstEdHour				0x0015		/* __u8	::	daylight saving end hour */
#define E_GrP3CfgOfsSysTmSvrType		0x0016		/* __u8	::	time server type */
#define E_GrP3CfgOfsSysTmSvrSync		0x0017		/* __u8	::	time server Sync cycle - hour unit */
#define E_GrP3CfgOfsSysTmSvrGmt			0x0018		/* __u8	::	time server gmt time value , index */
#define E_GrP3CfgOfsOsdAlpha				0x0019		/* __u8	::	display osd alpha value 0 ~ 100 */
#define E_GrP3CfgOfsDispRes					0x001A		/* __u8	::	display resolution index */

/* reserved 0x001B ~ 0x001F */

#define E_GrP3CfgOfsUserId					0x0020		/* WCHAR[8][16]	::	user id */
#define E_GrP3CfgOfsUserPass				0x0120		/* WCHAR[2][8][16]	::	password - dual password support */
#define E_GrP3CfgOfsUserGroup				0x0320		/* __u8[8]	::	authority group */

#define E_GrP3CfgOfsBtnBeep					0x0328		/* BOOL8	::	button setting beep use */
#define E_GrP3CfgOfsBtnAtLock				0x0329		/* __u8	::	button setting auto lock key time index */
#define E_GrP3CfgOfsBtnEmgcLock			0x032A		/* BOOL8	::	button setting emergency lock key use */
/* reserved 0x032B ~ 0x032F */

#define E_GrP3CfgOfsUserAuth				0x0330		/* __u16[8]	::	authority map */
#define E_GrP3CfgOfsUserCovert			0x0340		/* __u16[8]	::	covert map */
#define E_GrP3CfgOfsUserLevel				0x0350		/* __u8[8]	::	user level */
#define E_GrP3CfgOfsCmsFwallV4			0x0358		/* __u32[3][2] :: firewall start, end */
#define E_GrP3CfgOfsCmsFwallUse			0x0370		/* BOOL8 :: CMS firewall use */
/* reserved 0x0371 ~ 0x037F */

#define E_GrP3CfgOfsCamType					0x0380		/* __u8[16]	::	cammera type , 0 = auto , 1 = D1, 2 = 960, 3 = HD */
#define E_GrP3CfgOfsCamUseMap				0x0390		/* __u16	::	cammera use map */
#define E_GrP3CfgOfsCamCvtMap				0x0392		/* __u16	::	cammera global corvert map */
#define E_GrP3CfgOfsCamAgcMap				0x0394		/* __u16	::	cammera auto gain use map */

/* reserved 0x0396 ~ 0x039F */

#define E_GrP3CfgOfsCamGain					0x03A0		/* __u8[16]	::	cammera gain value table */
#define E_GrP3CfgOfsCamBright				0x03B0		/* __u8[16]	::	cammera bright value table */
#define E_GrP3CfgOfsCamContrast			0x03C0		/* __u8[16]	::	cammera contrast value table */
#define E_GrP3CfgOfsCamHue					0x03D0		/* __u8[16]	::	cammera hue value table */

#define E_GrP3CfgOfsCamTitle				0x03E0		/* WCHAR[16][16]	::	cammera title */

#define E_GrP3CfgOfsQuadMap					0x05E0		/* __u8[0xA0] :: quad map */

#define E_GrP3CfgOfsHddType					0x0680		/* __u8[8] :: disk type, 0 = main, 1 = mirror, 2 = none,  */
#define E_GrP3CfgOfsHddOverWt				0x0688		/* BOOL8 : HDD overwrite on/off */
#define E_GrP3CfgOfsHddAtDel				0x0689		/* __u8 : HDD auto delete */

/* reserved 0x068A ~ 0x068F */

#define E_GrP3CfgOfsDiUseMap				0x0690		/* __u16 :: sensor use bitmap */
#define E_GrP3CfgOfsDiTypeMap				0x0692		/* __u16 :: sensor type bitmap , 0 = normal close, 1 = normal open */
#define E_GrP3CfgOfsRecAdoMap				0x0694		/* __u16 :: audio record bitmap */
#define E_GrP3CfgOfsRecVdoMap				0x0696		/* __u16 :: video record bitmap */

#define E_GrP3CfgOfsHolyBaseYear		0x0698		/* __u16 :: holyday base year */

/* reserved 0x069A ~ 0x069F */

#define E_GrP3CfgOfsDiCamMap				0x06A0		/* __u16[16] :: sensor relate cam map */
#define E_GrP3CfgOfsRecPost					0x06E0		/* __u8[16] :: record post time - 10 sec unit */
#define E_GrP3CfgOfsCamSat					0x06F0		/* __u8[16]	::	cammera saturation value table */

#define E_GrP3CfgOfsRecSchd					0x0700		/* __u8[0x2100] :: record schedule */
#define E_GrP3CfgOfsRecHoly					0x2800		/* __u8[0x300] :: holyday map */

#define E_GrP3CfgOfsEprsNrmPres			0x2B00		/* __u8[16] :: normal preset position */
#define E_GrP3CfgOfsEprs0Type				0x2B10		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs0Idx				0x2B20		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs0Preset			0x2B30		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs0Dwell			0x2B40		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs1Type				0x2B50		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs1Idx				0x2B60		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs1Preset			0x2B70		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs1Dwell			0x2B80		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs2Type				0x2B90		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs2Idx				0x2BA0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs2Preset			0x2BB0		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs2Dwell			0x2BC0		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs3Type				0x2BD0		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs3Idx				0x2BE0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs3Preset			0x2BF0		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs3Dwell			0x2C00		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs4Type				0x2C10		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs4Idx				0x2C20		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs4Preset			0x2C30		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs4Dwell			0x2C40		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs5Type				0x2C50		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs5Idx				0x2C60		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs5Preset			0x2C70		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs5Dwell			0x2C80		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs6Type				0x2C90		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs6Idx				0x2CA0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs6Preset			0x2CB0		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs6Dwell			0x2CC0		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs7Type				0x2CD0		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs7Idx				0x2CE0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs7Preset			0x2CF0		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs7Dwell			0x2D00		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs8Type				0x2D10		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs8Idx				0x2D20		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs8Preset			0x2D30		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs8Dwell			0x2D40		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs9Type				0x2D50		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs9Idx				0x2D60		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs9Preset			0x2D70		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs9Dwell			0x2D80		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs10Type			0x2D90		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs10Idx				0x2DA0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs10Preset		0x2DB0		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs10Dwell			0x2DC0		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs11Type			0x2DD0		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs11Idx				0x2DE0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs11Preset		0x2DF0		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs11Dwell			0x2E00		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs12Type			0x2E10		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs12Idx				0x2E20		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs12Preset		0x2E30		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs12Dwell			0x2E40		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs13Type			0x2E50		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs13Idx				0x2E60		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs13Preset		0x2E70		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs13Dwell			0x2E80		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs14Type			0x2E90		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs14Idx				0x2EA0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs14Preset		0x2EB0		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs14Dwell			0x2EC0		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP3CfgOfsEprs15Type			0x2ED0		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP3CfgOfsEprs15Idx				0x2EE0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP3CfgOfsEprs15Preset		0x2EF0		/* __u8[16] :: PTZ event preset number */
#define E_GrP3CfgOfsEprs15Dwell			0x2F00		/* __u8[16] :: PTZ event preset dwell time */

/* reserved 0x2F10 ~ 0x2F80 */

#define E_GrP3CfgOfsSysTmSvrAddr		0x2F80		/* WCHAR[64]	::	time server address */

#define E_GrP3CfgOfsRecRes					0x3000		/* __u8[16]	::	record resolution */
#define E_GrP3CfgOfsRecFps					0x3010		/* __u8[16]	::	record fps value */
#define E_GrP3CfgOfsRecQua					0x3020		/* __u8[16]	::	record frame quality */
#define E_GrP3CfgOfsRecRuFps				0x3030		/* __u8[16]	::	record ram up fps value */
#define E_GrP3CfgOfsRecRuQua				0x3040		/* __u8[16]	::	record ram up frame quality */
#define E_GrP3CfgOfsRecRuRes				0x3050		/* __u8[16]	::	record ram up resolution */
#define E_GrP3CfgOfsNetFps					0x3060		/* __u8[16]	::	network fps value */
#define E_GrP3CfgOfsNetQua					0x3070		/* __u8[16]	::	network frame quality */
#define E_GrP3CfgOfsNetRes					0x3080		/* __u8[16]	::	network resolution */

#define E_GrP3CfgOfsDisSeqCh				0x3090		/* __u8[16] : auto sequence split 1 channel time */
#define E_GrP3CfgOfsDisSeqSplit4		0x30A0		/* __u8 : auto sequence split 4 */
#define E_GrP3CfgOfsDisSeqSplit6		0x30A1		/* __u8 : auto sequence split 6 */
#define E_GrP3CfgOfsDisSeqSplit8		0x30A2		/* __u8 : auto sequence split 8 */
#define E_GrP3CfgOfsDisSeqSplit9		0x30A3		/* __u8 : auto sequence split 9 */
#define E_GrP3CfgOfsDisSeqSplit10		0x30A4		/* __u8 : auto sequence split 10 */
#define E_GrP3CfgOfsDisSeqSplit13		0x30A5		/* __u8 : auto sequence split 13 */
#define E_GrP3CfgOfsDisSeqLoss			0x30A6		/* BOOL8 : video loss channel skip */
#define E_GrP3CfgOfsDisSplitMode		0x30A7		/* __u8 : split mode */

#define E_GrP3CfgOfsRecPreUse				0x30A8		/* BOOL8 : preframe recording use */
#define E_GrP3CfgOfsRecRamUse				0x30A9		/* BOOL8 : ram-up recording(event recording) use */

/* reserved 0x30A8 ~ 0x30AF */

#define E_GrP3CfgOfsSpotChTime			0x30B0		/* __u8[16] : spot dwell time for each channel */
#define E_GrP3CfgOfsSpotGroupTime		0x30C0		/* __u8[4] : spot dwell time for each group */
#define E_GrP3CfgOfsSpotQuadMode		0x30C4		/* __u8 : spot quad mode */
#define E_GrP3CfgOfsSpotEvtMode			0x30C5		/* __u8 : spot event mode */
#define E_GrP3CfgOfsSpotFixCh				0x30C6		/* __u8 : spot fix channel */

/* reserved 0x30C7 ~ 0x30CD */
#define E_GrP3CfgOfsDoHddErrUseMap	0x30CE		/* __u16					: alarm HDD error map */
#define E_GrP3CfgOfsDoMtnOutTimes		0x30D0		/* __u8[16][16] : alarm motion alarm time - sec unit */
#define E_GrP3CfgOfsDoSenOutTimes		0x31D0		/* __u8[16][16] : alarm sensor alarm time - sec unit */
#define E_GrP3CfgOfsDoVdlOutTimes		0x32D0		/* __u8[16][16] : alarm video loss alarm time - sec unit */
#define E_GrP3CfgOfsDoHddErrDur			0x33D0		/* __u8[16]			: alarm HDD error duration */

#define E_GrP3CfgOfsNetEmailTitle		0x33E0		/* WCHAR[64] : email title */
#define E_GrP3CfgOfsNetSmtpAdr			0x3460		/* WCHAR[64] : SMTP server address */
#define E_GrP3CfgOfsNetSmtpPort			0x34E0		/* __u16 : SMTP port */
#define E_GrP3CfgOfsNetEmailEvt			0x34E2		/* __u16 : email notify event - event log mask */
#define E_GrP3CfgOfsNetEmailInter		0x34E4		/* __u8 : email interval - sec table index */
#define E_GrP3CfgOfsNetSmtpSecu			0x34E5		/* __u8 : email security type */
/* reserved 0x34E6 ~ 0x3523 */
#define E_GrP3CfgOfsDualStrmUse			0x3524		/* BOOL8 : dual stream */
#define E_GrP3CfgOfsDualStrmBand		0x3525		/* __u8 : dual stream band width mode */
#define E_GrP3CfgOfsCmsPort					0x3526		/* __u16 : CMS port */
#define E_GrP3CfgOfsWebPort					0x3528		/* __u16 : web port */
#define E_GrP3CfgOfsNetChMap				0x352A		/* __u16 : network frame channel map */
/* reserved 0x352C ~ 0x352F */

#define E_GrP3CfgOfsNetFdnsUrl			0x3530		/* WCHAR[64] : free DNS URL */
#define E_GrP3CfgOfsNetFdnsId				0x35B0		/* WCHAR[64] : free DNS id */
#define E_GrP3CfgOfsNetFdnsPass			0x3630		/* WCHAR[32] : free DNS password */

#define E_GrP3CfgOfsNetIsDhcp				0x3670		/* BOOL8 :	DHCP use */
#define E_GrP3CfgOfsNetFdnsType			0x3671		/* __u8 : free DNS server 0=none,1=DynDns,2=no-ip */

/* reserved 0x3672 ~ 0x367F */
#define E_GrP3CfgOfsNetIpv4					0x3680		/* __u8[4] : network ip v4 */
#define E_GrP3CfgOfsNetGateway4			0x3684		/* __u8[4] : network gateway v4 */
#define E_GrP3CfgOfsSysDns4					0x3688		/* __u8[4] : network DNS address v4 */
#define E_GrP3CfgOfsNetMask4				0x368C		/* __u8[4] :	NETMASK ipv4 */

/* reserved 0x3690 ~ 0x369F */

#define E_GrP3CfgOfsNetIpv6					0x36A0		/* __u16[8] : network ip v6 */
#define E_GrP3CfgOfsNetGatewayv6		0x36B0		/* __u16[8] : network gateway v6 */
#define E_GrP3CfgOfsSysDnsv6				0x36C0		/* __u16[8] : network DNS address v6 */
#define E_GrP3CfgOfsNetMask6				0x36D0		/* __u16		 : NETMASK ipv6 */

/* reserved 0x36D2 ~ 0x36DF */

#define E_GrP3CfgOfsBuzMtnTimes			0x36E0		/* __u8[16] :	internal buzzer motion time - sec unit */
#define E_GrP3CfgOfsBuzSenTimes			0x36F0		/* __u8[16] :	internal buzzer sensor time - sec unit */
#define E_GrP3CfgOfsBuzLossTimes		0x3700		/* __u8[16] :	internal buzzer video loss time - sec unit */
#define E_GrP3CfgOfsBuzHddErr				0x3710		/* __u8			:	internal buzzer hard disk error - sec unit */
#define E_GrP3CfgOfsBuzHddTemp			0x3711		/* __u8			: HDD temperature */

/* reserved 0x3712 ~ 0x371F */

#define E_GrP3CfgOfsUartSpeed				0x3720		/* __u8[4] :	UART port speed, 0=1200, 1=2400, 2=4800, 3=9600, 4=19200, 5=38400, 6=115200 */
#define E_GrP3CfgOfsUartDataBit			0x3724		/* __u8[4] :	data bit size, 0=8bit, 1=7bit */
#define E_GrP3CfgOfsUartStopBit			0x3728		/* __u8[4] :	stop bit, 0=1bit, 1=1.5bit, 2=2bit */
#define E_GrP3CfgOfsUartParity			0x372C		/* __u8[4] :	parity bit, 0= none, 1=even, 2=odd  */

#define E_GrP3CfgOfsCtrlModel				0x3730		/* __u8 : controller model */
#define E_GrP3CfgOfsCtrlId					0x3731		/* __u8 : controller id */
#define E_GrP3CfgOfsCtrlUart				0x3732		/* __u8 : controller UART port */
/* reserved 0x3733 ~ 0x373F */

#define E_GrP3CfgOfsNetDdnsAdr			0x3740		/* WCHAR[64] : DDNS Address */

#define E_GrP3CfgOfsMtnMap					0x37C0		/* __u32[16][32] : motion sense map */
#define E_GrP3CfgOfsMtnSensi				0x3FC0		/* __u8[16][24] : motion sensitivity schedule table */

#define E_GrP3CfgOfsEvtPpEvt				0x4140		/* __u8[16] : event pop up event mask */
#define E_GrP3CfgOfsEvtPpDur				0x4150		/* __u8[16] : event pop up duration */

#define E_GrP3CfgOfsSmtpId					0x4160		/* WCHAR[64] : SMTP server id */
#define E_GrP3CfgOfsSmtpPass				0x41E0    /* WCHAR[32] : SMTP server password */
#define E_GrP3CfgOfsEmailSender			0x4220		/* WCHAR[64] : email sender */
#define E_GrP3CfgOfsEmailTo1				0x42A0		/* WCHAR[64] : email to address 1 */
#define E_GrP3CfgOfsEmailTo2				0x4320		/* WCHAR[64] : email to address 2 */
#define E_GrP3CfgOfsEmailTo3				0x43A0		/* WCHAR[64] : email to address 3 */

#define E_GrP3CfgOfsPtzType					0x4420		/* __u8[16] : PTZ camera type */
#define E_GrP3CfgOfsPtzId						0x4430		/* __u8[16] : PTZ camera ID */
#define E_GrP3CfgOfsPtzPort					0x4440		/* __u8[16] : PTZ serial - 0 = 485, 1 = 232 */
#define E_GrP3CfgOfsPtzBaud					0x4450		/* __u8[16] : PTZ baud rate */
#define E_GrP3CfgOfsPtzDataBit			0x4460		/* __u8[16] : PTZ data bit - 0=8bit, 1=7bit */
#define E_GrP3CfgOfsPtzStopBit			0x4470		/* __u8[16] : PTZ stop bit - 0=1bit, 1=1.5bit, 2=2bit */
#define E_GrP3CfgOfsPtzParity				0x4480		/* __u8[16] : PTZ parity bit - 0= none, 1=even, 2=odd */
#define E_GrP3CfgOfsPtzAtb					0x4490		/* __u8[16] : PTZ attribute, bit 0 = pan reverse, bit1= tilt reverse */
#define E_GrP3CfgOfsPtzTourType			0x44A0		/* __u8[16] : PTZ tour type 0 = DVR, 1 = camera */
#define E_GrP3CfgOfsPtzSpdPan				0x44B0		/* __u8[16] : PTZ pan speed */
#define E_GrP3CfgOfsPtzSpdTilt			0x44C0		/* __u8[16] : PTZ tilt speed */
#define E_GrP3CfgOfsPtzSpdZoom			0x44D0		/* __u8[16] : PTZ zoom speed */
#define E_GrP3CfgOfsPtzSpdFocus			0x44E0		/* __u8[16] : PTZ focus speed */
#define E_GrP3CfgOfsPtzTourCfg			0x44F0		/* __u32[16][12] : PTZ tour: preset, speed, maintain */

#define E_GrP3CfgOfsBlkageMap				0x47F0		/* __u32[16][32] : blockage map */
#define E_GrP3CfgOfsBlkageVisi			0x4FF0		/* __u8 : blockage visible */

#define E_GrP3CfgOfsSmtpSSL					0x4FF1		/* BOOL8 : SMTP use openssl */
/* 0x4FF2 ~ */

#define E_GrP3CfgOfsPushEnable			0x5000		/* BOOL8 : push-server enable */
#define E_GrP3CfgOfsPushSvrIpv4			0x5001		/* __u8[4] : push-server network ip v4 */
#define E_GrP3CfgOfsPushSvrIpv6			0x5005		/* __u16[8] : push-server network ip v6 */
#define E_GrP3CfgOfsPushSvrPort			0x5015		/* __u16 : push-server network port */
#define E_GrP3CfgOfsPushDvrTitle		0x5017		/* WCHAR[16] : push-server dvr title */
#define E_GrP3CfgOfsPushIsDvrEvt		0x5037		/* BOOL8 : push-server event duration time */

#define E_GrP3CfgOfsPushEvtTime			0x5038		/* __u8 : push-server event duration time */
#define E_GrP3CfgOfsPushEvt					0x5039		/* __u8 : push-server event */
#define E_GrP3CfgOfsPushHoly1Evt		0x503A		/* __u8 : push-server holyday1 event */
#define E_GrP3CfgOfsPushHoly2Evt		0x503B		/* __u8 : push-server holyday2 event */
#define E_GrP3CfgOfsPushHoly3Evt		0x503C		/* __u8 : push-server holyday3 event */
#define E_GrP3CfgOfsPushHoly4Evt		0x503D		/* __u8 : push-server holyday4 event */

/* 0x503E ~ 0x503F reserved */
#define E_GrP3CfgOfsPushScd					0x5040		/* __u32 : push-server schedule */
#define E_GrP3CfgOfsPushScdHoly1		0x5044		/* __u32 : push-server holyday1 schedule */
#define E_GrP3CfgOfsPushScdHoly2		0x5048		/* __u32 : push-server holyday2 schedule */
#define E_GrP3CfgOfsPushScdHoly3		0x504C		/* __u32 : push-server holyday3 schedule */
#define E_GrP3CfgOfsPushScdHoly4		0x5050		/* __u32 : push-server holyday4 schedule */

#define E_GrP3CfgOfsPushSvrUrl			0x5054		/* WCHAR[64] : push-server url address */
/* 0x50D4 ~ 0x50FF reserved */

/* 0x5100 ~ reserved */

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global function

//====================================================================
//out Environment conditional
#endif

