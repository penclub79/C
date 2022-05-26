/*
	GAUSE Platform system configuration

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4Cfg
#define	_EPC_GrP4Cfg

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance

#define E_GrP4CfgHolyDayTime				(3840 * 24 * 60 * 60)
#define E_GrP4CfgHoliSchdCnt				4					/* max holiday schedule count */
#define E_GrP4CfgHoliDayCnt					256
#define E_GrP4CfgRschdUnitMin				30				/* 30 minute unit */
#define E_GrP4CfgRschdAtbDayCnt			48
#define E_GrP4CfgMtnSchdUnit				24
#define E_GrP4CfgMtnValMax					9

#define E_GrP4CfgUartMax						4					/* UART max count */
#define E_GrP4CfgUartSpeedCnt				5

#define E_GrP4CfgFirewallCnt				3					/* firewall item count */

#define E_GrP4CfgCamTitleLen				16

#define E_GrP4CfgAuthSetup					0x0001		/* authority setup  */
#define E_GrP4CfgAuthHdd						0x0002		/* authority HDD  */
#define E_GrP4CfgAuthSearch					0x0004		/* authority search */
#define E_GrP4CfgAuthBackup					0x0008		/* authority backup */
#define E_GrP4CfgAuthPtzCtrl				0x0010		/* authority PTZ */
#define E_GrP4CfgAuthReckey					0x0020		/* authority record key */
#define E_GrP4CfgAuthPtzOsd					0x0040		/* authority PTZ OSD */
#define E_GrP4CfgAuthNetwork				0x0080		/* authority network */

#define E_GrP4CfgHdayBaseYear				2012
#define E_GrP4CfgHdayYearCnt				4					/* holiday year count */

#define E_GrP4CfgFccId							Mac_GrMakeFcc('P','4','C','F')

#define E_GrP4CfgOfsFcc							0x0000		/* __u32	::	fourcc code, check current configuration */
#define E_GrP4CfgOfsCfgVer					0x0004		/* __u16	::	firmware version */
#define E_GrP4CfgOfsModel						0x0006		/* __u16	::	model number */

#define E_GrP4CfgOfsVdoType					0x0008		/* __u8	::	signal type , ntsc , pal */
#define E_GrP4CfgOfsRmtId						0x0009		/* __u8	::	remocon id */
#define E_GrP4CfgOfsLang						0x000A		/* __u8	::	language */
#define E_GrP4CfgOfsMseRes					0x000B		/* __u8	::	mouse resolution */
#define E_GrP4CfgOfsDateFmt					0x000C		/* __u8	::	date format */
#define E_GrP4CfgOfsIsDst						0x000D		/* BOOL8	::	daylight saving time use */
#define E_GrP4CfgOfsDstStMon				0x000E		/* __u8	::	daylight saving start month */
#define E_GrP4CfgOfsDstStWkTh				0x000F		/* __u8	::	daylight saving start week th */
#define E_GrP4CfgOfsDstStWkDay			0x0010		/* __u8	::	daylight saving start week day */
#define E_GrP4CfgOfsDstStHour				0x0011		/* __u8	::	daylight saving start hour */
#define E_GrP4CfgOfsDstEdMon				0x0012		/* __u8	::	daylight saving end month */
#define E_GrP4CfgOfsDstEdWkTh				0x0013		/* __u8	::	daylight saving end week th */
#define E_GrP4CfgOfsDstEdWkDay			0x0014		/* __u8	::	daylight saving end week day */
#define E_GrP4CfgOfsDstEdHour				0x0015		/* __u8	::	daylight saving end hour */

/* reserved 0x0016 ~ 0x0016 */

#define E_GrP4CfgOfsSysTmSvrSync		0x0017		/* __u8	::	time server Sync cycle - hour unit */
#define E_GrP4CfgOfsSysTmSvrGmt			0x0018		/* __u8	::	time server gmt time value , index */
#define E_GrP4CfgOfsOsdAlpha				0x0019		/* __u8	::	display osd alpha value 0 ~ 100 */
#define E_GrP4CfgOfsDispRes					0x001A		/* __u8	::	display resolution index */

/* reserved 0x001B ~ 0x0327 */

#define E_GrP4CfgOfsBtnBeep					0x0328		/* BOOL8	::	button setting beep use */
#define E_GrP4CfgOfsBtnAtLock				0x0329		/* __u8	::	button setting auto lock key time index */
#define E_GrP4CfgOfsBtnEmgcLock			0x032A		/* BOOL8	::	button setting emergency lock key use */

/* reserved 0x032B ~ 0x0357 */

#define E_GrP4CfgOfsCmsFwallV4			0x0358		/* __u32[3][2] :: firewall start, end */
#define E_GrP4CfgOfsCmsFwallUse			0x0370		/* BOOL8 :: CMS firewall use */
/* reserved 0x0371 ~ 0x038F */

#define E_GrP4CfgOfsCamUseMap				0x0390		/* __u16	::	cammera use map */
#define E_GrP4CfgOfsCamCvtMap				0x0392		/* __u16	::	cammera global corvert map */
#define E_GrP4CfgOfsCamAgcMap				0x0394		/* __u16	::	cammera auto gain use map */

/* reserved 0x0396 ~ 0x039F */
#define E_GrP4CfgOfsCamGain					0x03A0		/* __u8[16]	::	cammera gain value table */
#define E_GrP4CfgOfsCamBright				0x03B0		/* __u8[16]	::	cammera bright value table */
#define E_GrP4CfgOfsCamContrast			0x03C0		/* __u8[16]	::	cammera contrast value table */
#define E_GrP4CfgOfsCamHue					0x03D0		/* __u8[16]	::	cammera hue value table */

#define E_GrP4CfgOfsCamTitle				0x03E0		/* WCHAR[16][16]	::	cammera title */

/* reserved 0x05E0 ~ 0x067F */

#define E_GrP4CfgOfsHddType					0x0680		/* __u8[8] :: disk type, 0 = main, 1 = mirror, 2 = none,  */
#define E_GrP4CfgOfsHddOverWt				0x0688		/* BOOL8 : HDD overwrite on/off */
#define E_GrP4CfgOfsHddAtDel				0x0689		/* __u16 : HDD auto delete */

/* reserved 0x068B ~ 0x068F */

#define E_GrP4CfgOfsDiUseMap				0x0690		/* __u16 :: sensor use bitmap */
#define E_GrP4CfgOfsDiTypeMap				0x0692		/* __u16 :: sensor type bitmap , 0 = normal close, 1 = normal open */
#define E_GrP4CfgOfsRecAdoMap				0x0694		/* __u16 :: audio record bitmap */
#define E_GrP4CfgOfsRecVdoMap				0x0696		/* __u16 :: video record bitmap */

#define E_GrP4CfgOfsHolyBaseYear		0x0698		/* __u16 :: holyday base year */

/* reserved 0x069A ~ 0x069F */

#define E_GrP4CfgOfsDiCamMap				0x06A0		/* __u16[16] :: sensor relate cam map */
#define E_GrP4CfgOfsRecPost					0x06E0		/* __u8[16] :: record post time - 10 sec unit */
#define E_GrP4CfgOfsCamSat					0x06F0		/* __u8[16]	::	cammera saturation value table */

#define E_GrP4CfgOfsRecSchd					0x0700		/* __u8[0x2100] :: record schedule */
#define E_GrP4CfgOfsRecHoly					0x2800		/* __u8[0x300] :: holyday map */

#define E_GrP4CfgOfsEprsNrmPres			0x2B00		/* __u8[16] :: normal preset position */
#define E_GrP4CfgOfsEprs0Type				0x2B10		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs0Idx				0x2B20		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs0Preset			0x2B30		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs0Dwell			0x2B40		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs1Type				0x2B50		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs1Idx				0x2B60		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs1Preset			0x2B70		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs1Dwell			0x2B80		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs2Type				0x2B90		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs2Idx				0x2BA0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs2Preset			0x2BB0		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs2Dwell			0x2BC0		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs3Type				0x2BD0		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs3Idx				0x2BE0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs3Preset			0x2BF0		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs3Dwell			0x2C00		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs4Type				0x2C10		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs4Idx				0x2C20		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs4Preset			0x2C30		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs4Dwell			0x2C40		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs5Type				0x2C50		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs5Idx				0x2C60		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs5Preset			0x2C70		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs5Dwell			0x2C80		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs6Type				0x2C90		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs6Idx				0x2CA0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs6Preset			0x2CB0		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs6Dwell			0x2CC0		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs7Type				0x2CD0		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs7Idx				0x2CE0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs7Preset			0x2CF0		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs7Dwell			0x2D00		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs8Type				0x2D10		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs8Idx				0x2D20		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs8Preset			0x2D30		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs8Dwell			0x2D40		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs9Type				0x2D50		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs9Idx				0x2D60		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs9Preset			0x2D70		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs9Dwell			0x2D80		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs10Type			0x2D90		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs10Idx				0x2DA0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs10Preset		0x2DB0		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs10Dwell			0x2DC0		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs11Type			0x2DD0		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs11Idx				0x2DE0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs11Preset		0x2DF0		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs11Dwell			0x2E00		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs12Type			0x2E10		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs12Idx				0x2E20		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs12Preset		0x2E30		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs12Dwell			0x2E40		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs13Type			0x2E50		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs13Idx				0x2E60		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs13Preset		0x2E70		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs13Dwell			0x2E80		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs14Type			0x2E90		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs14Idx				0x2EA0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs14Preset		0x2EB0		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs14Dwell			0x2EC0		/* __u8[16] :: PTZ event preset dwell time */
#define E_GrP4CfgOfsEprs15Type			0x2ED0		/* __u8[16] :: PTZ event type, 0 = none 1 = motion 2 = sensor */
#define E_GrP4CfgOfsEprs15Idx				0x2EE0		/* __u8[16] :: PTZ event index, motion = channel , sensor = pin number */
#define E_GrP4CfgOfsEprs15Preset		0x2EF0		/* __u8[16] :: PTZ event preset number */
#define E_GrP4CfgOfsEprs15Dwell			0x2F00		/* __u8[16] :: PTZ event preset dwell time */

/* reserved 0x2F10 ~ 0x2F80 */

#define E_GrP4CfgOfsSysTmSvrAddr		0x2F80		/* WCHAR[64]	::	time server address */

#define E_GrP4CfgOfsRecRes					0x3000		/* __u8[16]	::	record resolution */
#define E_GrP4CfgOfsRecFps					0x3010		/* __u8[16]	::	record fps value */
#define E_GrP4CfgOfsRecQua					0x3020		/* __u8[16]	::	record frame quality */
#define E_GrP4CfgOfsRecRuFps				0x3030		/* __u8[16]	::	record ram up fps value */
#define E_GrP4CfgOfsRecRuQua				0x3040		/* __u8[16]	::	record ram up frame quality */
#define E_GrP4CfgOfsRecRuRes				0x3050		/* __u8[16]	::	record ram up resolution */
#define E_GrP4CfgOfsNetFps					0x3060		/* __u8[16]	::	network fps value */
#define E_GrP4CfgOfsNetQua					0x3070		/* __u8[16]	::	network frame quality */
#define E_GrP4CfgOfsNetRes					0x3080		/* __u8[16]	::	network resolution */

#define E_GrP4CfgOfsDisSeqCh				0x3090		/* __u8[16] : auto sequence split 1 channel time */
#define E_GrP4CfgOfsDisSeqSplit4		0x30A0		/* __u8 : auto sequence split 4 */
#define E_GrP4CfgOfsDisSeqSplit6		0x30A1		/* __u8 : auto sequence split 6 */
#define E_GrP4CfgOfsDisSeqSplit8		0x30A2		/* __u8 : auto sequence split 8 */
#define E_GrP4CfgOfsDisSeqSplit9		0x30A3		/* __u8 : auto sequence split 9 */
#define E_GrP4CfgOfsDisSeqSplit10		0x30A4		/* __u8 : auto sequence split 10 */
#define E_GrP4CfgOfsDisSeqSplit13		0x30A5		/* __u8 : auto sequence split 13 */
#define E_GrP4CfgOfsDisSeqLoss			0x30A6		/* BOOL8 : video loss channel skip */
#define E_GrP4CfgOfsDisSplitMode		0x30A7		/* __u8 : split mode */

#define E_GrP4CfgOfsRecPreUse				0x30A8		/* BOOL8 : preframe recording use */
#define E_GrP4CfgOfsRecRamUse				0x30A9		/* BOOL8 : ram-up recording(event recording) use */

/* reserved 0x30A8 ~ 0x30AF */

#define E_GrP4CfgOfsSpotChTime			0x30B0		/* __u8[16] : spot dwell time for each channel */
#define E_GrP4CfgOfsSpotGroupTime		0x30C0		/* __u8[4] : spot dwell time for each group */
#define E_GrP4CfgOfsSpotQuadMode		0x30C4		/* __u8 : spot quad mode */
#define E_GrP4CfgOfsSpotEvtMode			0x30C5		/* __u8 : spot event mode */
#define E_GrP4CfgOfsSpotFixCh				0x30C6		/* __u8 : spot fix channel */

/* reserved 0x30C7 ~ 0x30CD */
#define E_GrP4CfgOfsDoHddErrUseMap	0x30CE		/* __u16				: alarm HDD error map */
#define E_GrP4CfgOfsDoMtnOutTimes		0x30D0		/* __u8[16][16] : alarm motion alarm time - sec unit */
#define E_GrP4CfgOfsDoSenOutTimes		0x31D0		/* __u8[16][16] : alarm sensor alarm time - sec unit */
#define E_GrP4CfgOfsDoVdlOutTimes		0x32D0		/* __u8[16][16] : alarm video loss alarm time - sec unit */
#define E_GrP4CfgOfsDoHddErrDur			0x33D0		/* __u8[16]			: alarm HDD error duration */

#define E_GrP4CfgOfsNetEmailTitle		0x33E0		/* WCHAR[64] : email title */
#define E_GrP4CfgOfsNetSmtpAdr			0x3460		/* WCHAR[64] : SMTP server address */
#define E_GrP4CfgOfsNetSmtpPort			0x34E0		/* __u16 : SMTP port */
#define E_GrP4CfgOfsNetEmailEvt			0x34E2		/* __u16 : email notify event - event log mask */
#define E_GrP4CfgOfsNetEmailInter		0x34E4		/* __u8 : email interval - sec table index */
#define E_GrP4CfgOfsNetSmtpSecu			0x34E5		/* __u8 : email security type */

/* reserved 0x34E6 ~ 0x3523 */
#define E_GrP4CfgOfsDualStrmUse			0x3524		/* BOOL8 : dual stream */
#define E_GrP4CfgOfsDualStrmBand		0x3525		/* __u8 : dual stream band width mode */
#define E_GrP4CfgOfsCmsPort					0x3526		/* __u16 : CMS port */
#define E_GrP4CfgOfsWebPort					0x3528		/* __u16 : web port */
#define E_GrP4CfgOfsNetChMap				0x352A		/* __u16 : network frame channel map */
#define E_GrP4CfgOfsCmsAlmKeep			0x352C		/* __u8 : cms alarm keep time */
#define E_GrP4CfgOfsCmsAlmRsvd0			0x352D		/* __u8 : cms alarm keep time - reserved */
#define E_GrP4CfgOfsCmsAlmRsvd1			0x352E		/* __u8 : cms alarm keep time - reserved */
#define E_GrP4CfgOfsCmsAlmRsvd2			0x352F		/* __u8 : cms alarm keep time - reserved */

#define E_GrP4CfgOfsNetFdnsUrl			0x3530		/* WCHAR[64] : free DNS URL */
#define E_GrP4CfgOfsNetFdnsId				0x35B0		/* WCHAR[64] : free DNS id */
#define E_GrP4CfgOfsNetFdnsPass			0x3630		/* WCHAR[32] : free DNS password */

#define E_GrP4CfgOfsNetIsDhcp				0x3670		/* BOOL8 :	DHCP use */
#define E_GrP4CfgOfsNetFdnsType			0x3671		/* __u8 : free DNS server 0=none,1=DynDns,2=no-ip */

/* reserved 0x3672 ~ 0x367F */
#define E_GrP4CfgOfsNetIpv4					0x3680		/* __u8[4] : network ip v4 */
#define E_GrP4CfgOfsNetGateway4			0x3684		/* __u8[4] : network gateway v4 */
#define E_GrP4CfgOfsSysDns4					0x3688		/* __u8[4] : network DNS address v4 */
#define E_GrP4CfgOfsNetMask4				0x368C		/* __u8[4] :	NETMASK ipv4 */

/* reserved 0x3690 ~ 0x369F */

#define E_GrP4CfgOfsNetIpv6					0x36A0		/* __u16[8] : network ip v6 */
#define E_GrP4CfgOfsNetGatewayv6		0x36B0		/* __u16[8] : network gateway v6 */
#define E_GrP4CfgOfsSysDnsv6				0x36C0		/* __u16[8] : network DNS address v6 */
#define E_GrP4CfgOfsNetMask6				0x36D0		/* __u16		 : NETMASK ipv6 */

/* reserved 0x36D2 ~ 0x36DF */

#define E_GrP4CfgOfsBuzMtnTimes			0x36E0		/* __u8[16] :	internal buzzer motion time - sec unit */
#define E_GrP4CfgOfsBuzSenTimes			0x36F0		/* __u8[16] :	internal buzzer sensor time - sec unit */
#define E_GrP4CfgOfsBuzLossTimes		0x3700		/* __u8[16] :	internal buzzer video loss time - sec unit */
#define E_GrP4CfgOfsBuzHddErr				0x3710		/* __u8			:	internal buzzer hard disk error - sec unit */
#define E_GrP4CfgOfsBuzHddTemp			0x3711		/* __u8			: HDD temperature */

/* reserved 0x3712 ~ 0x371F */

#define E_GrP4CfgOfsUartSpeed				0x3720		/* __u8[4] :	UART port speed, 0=1200, 1=2400, 2=4800, 3=9600, 4=19200, 5=38400, 6=115200 */
#define E_GrP4CfgOfsUartDataBit			0x3724		/* __u8[4] :	data bit size, 0=8bit, 1=7bit */
#define E_GrP4CfgOfsUartStopBit			0x3728		/* __u8[4] :	stop bit, 0=1bit, 1=1.5bit, 2=2bit */
#define E_GrP4CfgOfsUartParity			0x372C		/* __u8[4] :	parity bit, 0= none, 1=even, 2=odd  */

#define E_GrP4CfgOfsCtrlModel				0x3730		/* __u8 : controller model */
#define E_GrP4CfgOfsCtrlId					0x3731		/* __u8 : controller id */
#define E_GrP4CfgOfsCtrlUart				0x3732		/* __u8 : controller UART port */
/* reserved 0x3733 ~ 0x373F */

#define E_GrP4CfgOfsNetDdnsAdr			0x3740		/* WCHAR[64] : DDNS Address */

#define E_GrP4CfgOfsMtnMap					0x37C0		/* __u32[16][32] : motion sense map */
#define E_GrP4CfgOfsMtnSensi				0x3FC0		/* __u8[16][24] : motion sensitivity schedule table */

#define E_GrP4CfgOfsEvtPpEvt				0x4140		/* __u8[16] : event pop up event mask */
#define E_GrP4CfgOfsEvtPpDur				0x4150		/* __u8[16] : event pop up duration */

/* reseerved 0x4160 ~ 0x441F */

#define E_GrP4CfgOfsPtzType					0x4420		/* __u8[16] : PTZ camera type */
#define E_GrP4CfgOfsPtzId						0x4430		/* __u8[16] : PTZ camera ID */
#define E_GrP4CfgOfsPtzPort					0x4440		/* __u8[16] : PTZ serial - 0 = 485, 1 = 232 */
#define E_GrP4CfgOfsPtzBaud					0x4450		/* __u8[16] : PTZ baud rate */
#define E_GrP4CfgOfsPtzDataBit			0x4460		/* __u8[16] : PTZ data bit - 0=8bit, 1=7bit */
#define E_GrP4CfgOfsPtzStopBit			0x4470		/* __u8[16] : PTZ stop bit - 0=1bit, 1=1.5bit, 2=2bit */
#define E_GrP4CfgOfsPtzParity				0x4480		/* __u8[16] : PTZ parity bit - 0= none, 1=even, 2=odd */
#define E_GrP4CfgOfsPtzAtb					0x4490		/* __u8[16] : PTZ attribute, bit 0 = pan reverse, bit1= tilt reverse */
#define E_GrP4CfgOfsPtzTourType			0x44A0		/* __u8[16] : PTZ tour type 0 = DVR, 1 = camera */
#define E_GrP4CfgOfsPtzSpdPan				0x44B0		/* __u8[16] : PTZ pan speed */
#define E_GrP4CfgOfsPtzSpdTilt			0x44C0		/* __u8[16] : PTZ tilt speed */
#define E_GrP4CfgOfsPtzSpdZoom			0x44D0		/* __u8[16] : PTZ zoom speed */
#define E_GrP4CfgOfsPtzSpdFocus			0x44E0		/* __u8[16] : PTZ focus speed */
#define E_GrP4CfgOfsPtzTourCfg			0x44F0		/* __u32[16][12] : PTZ tour: preset, speed, maintain */

#define E_GrP4CfgOfsPrivacyMap			0x47F0		/* __u32[16][32] : Privacy map */
#define E_GrP4CfgOfsPrivacyEnable		0x4FF0		/* __u8 : Privacy visible */
#define E_GrP4CfgOfsSmtpSSL					0x4FF1		/* BOOL8 : SMTP use openssl */

/* 0x4FF2 ~ 0x4FFF*/

#define E_GrP4CfgOfsUserId					0x5000		/* WCHAR[32][16]	::	user id */
#define E_GrP4CfgOfsUserPass				0x5400		/* WCHAR[2][32][16]	::	password - dual password support */
#define E_GrP4CfgOfsUserGroup				0x5C00		/* __u8[32]	::	authority group */

#define E_GrP4CfgOfsUserAuth				0x5C20		/* __u16[32]	::	authority map */
#define E_GrP4CfgOfsUserCovert			0x5C60		/* __u16[32]	::	covert map */
#define E_GrP4CfgOfsUserLevel				0x5CA0		/* __u8[32]	::	user level */

#define E_GrP4CfgOfsSmtpId					0x5CC0		/* WCHAR[64] : SMTP server id */
#define E_GrP4CfgOfsSmtpPass				0x5D40    /* WCHAR[32] : SMTP server password */
#define E_GrP4CfgOfsEmailSender			0x5D80		/* WCHAR[64] : email sender */
#define E_GrP4CfgOfsEmailTo1				0x5E00		/* WCHAR[64] : email to address 1 */
#define E_GrP4CfgOfsEmailTo2				0x5E80		/* WCHAR[64] : email to address 2 */
#define E_GrP4CfgOfsEmailTo3				0x5F00		/* WCHAR[64] : email to address 3 */
#define E_GrP4CfgOfsEmailTo4				0x5F80		/* WCHAR[64] : email to address 4 */

#define E_GrP4CfgOfsCamSharp				0x6000		/* __u8[16]	::	cammera sharpness value table */

#define E_GrP4CfgOfsQuadMap					0x6010		/* __u8[0x200] :: quad map */

/* 0x6210 ~ 0x6FFF reserved */


//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global function

//====================================================================
//out Environment conditional
#endif

