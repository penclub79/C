/*
	GAUSE Platform script

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3Script
#define	_EPC_GrP3Script

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<GrGdibBase.h>
#include <Dvr/GrDvrBase.h>

//====================================================================
//constance

// script version
#define E_GrP3ScptFcc									Mac_GrMakeFcc('P','3','S','0')
#define E_GrP3ScptVer									1							/* script version */

#define E_GrP3ScptConMaxCnt						2							/* console max count */

//#define E_GrP3ScptRuleMaxCnt					32						/* rule count */

#define	E_GrP3VencResTypeCnt					4							/* resolution type count */
#define	E_GrP3VencResSubCnt						4							/* resolution sub type count */

#if 0
#define E_GrP3ScptRuleWinInfo					0							/* window info */
#define E_GrP3ScptRulePageInfo				1							/* page info */
#define E_GrP3ScptRuleAsmInfo					2							/* assembly info */
#define E_GrP3ScptRulePstInfo					3							/* paint script info */
#define E_GrP3ScptRuleFontInfo				4							/* font info */
#define E_GrP3ScptRuleImgInfo					5							/* image info */
#define E_GrP3ScptRuleTextInfo				6							/* text info */
#endif

// data script type
/* 1 ~ 2 : use by DVL */
#define	E_GrP3ScptDataWinScptTbl			3		/* window script info table */
/* 4 : use by DVL */
#define	E_GrP3ScptDataWinScptData			5		/* window script data */
/* 6 ~ 7 : use by DVL */
#define	E_GrP3ScptDataPstScptData			8		/* PST script data */
#define	E_GrP3ScptDataImageData				9		/* image data */
#define	E_GrP3ScptDataFontData				10	/* font data */
#define	E_GrP3ScptDataTextData				11	/* text data */
/* 12 ~ 16 : use by DVL */
#define	E_GrP3ScptDataAsmTbl					17	/* assembly script table */
#define	E_GrP3ScptDataAsmData					18	/* assembly code data */
/* 19 : use by DVL */
#define	E_GrP3ScptDataPageScptTbl			20	/* page script info table */
/* 21 : use by DVL */
#define	E_GrP3ScptDataPageScptData		22	/* page script data */
/* 23 ~ 26 : use by DVL */
#define	E_GrP3ScptDataCfgPrj					27	/* configuration project */
#define	E_GrP3ScptDataCfgRtl					28	/* configuration runtime */
#define	E_GrP3ScptDataMdlAtb					29	/* model attribute */
#define	E_GrP3ScptDataPstTbl					30	/* PST single skin script  */

// script model attribute
#define E_GrP3ScptMdlAtbClstCnt				215						/* script model attribute control list item count */

// script attribute
//#define E_GrP3ScptAtbAdrReloc					0x00000001		/* address relocated */


// text
#define E_GrP3ScptLangMaxCnt					16						/* max language count */
#define E_GrP3ScptTxtIdNone						0
#define E_GrP3ScptTxtIdBase						1
#define E_GrP3ScptTxtIdCtrl						30000					/* control text id */
#define E_GrP3ScptTxtCtrlCnt					16						/* max control text count */
// basic text id
#define E_GrP3ScptTxtIdLang0					(0 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang1					(1 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang2					(2 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang3					(3 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang4					(4 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang5					(5 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang6					(6 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang7					(7 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang8					(8 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang9					(9 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang10					(10 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang11					(11 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang12					(12 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang13					(13 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang14					(14 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdLang15					(15 + E_GrP3ScptTxtIdBase) 

#define E_GrP3ScptTxtIdMonth1					(16 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdMonth2					(17 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdMonth3					(18 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdMonth4					(19 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdMonth5					(20 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdMonth6					(21 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdMonth7					(22 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdMonth8					(23 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdMonth9					(24 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdMonth10				(25 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdMonth11				(26 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdMonth12				(27 + E_GrP3ScptTxtIdBase) 

#define E_GrP3ScptTxtIdWeekSun				(28 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdWeekMon				(29 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdWeekTue				(30 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdWeekWed				(31 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdWeekThu				(32 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdWeekFri				(33 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdWeekSat				(34 + E_GrP3ScptTxtIdBase) 

#define E_GrP3ScptTxtIdUnitYear				(35 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdUnitMonth			(36 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdUnitDay				(37 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdUnitHour				(38 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdUnitMin				(39 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdUnitSec				(40 + E_GrP3ScptTxtIdBase) 

#define E_GrP3ScptTxtIdTimeAm					(41 + E_GrP3ScptTxtIdBase) 
#define E_GrP3ScptTxtIdTimePm					(42 + E_GrP3ScptTxtIdBase) 

#define E_GrP3ScptTxtIdChannel				(50 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdDetected				(51 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdChanged				(52 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdStart					(53 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdEnd						(54 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdResolution			(55 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdRecord					(56 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdRecQuility			(57 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdRecSpeed				(58 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdRecPost				(59 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdRecPre					(60 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdOn							(61 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdOff						(62 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdMotionMap			(63 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdRecSchd				(64 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdPassword				(65 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdCamera					(66 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdTitle					(67 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdNetwork				(68 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdDisconnect			(69 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdConnect				(70 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdSearch					(71 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdBackup					(72 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdMotion					(73 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdSensor					(74 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdEvtExt1				(75 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdEvtExt2				(76 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdEvtExt3				(77 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdEvtExt4				(78 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdEvtExt5				(79 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdVdoLoss				(80 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdVdoRcvy				(81 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdPowerOn				(82 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdPowerOff				(83 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdLogin					(84 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdLogout					(85 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdSetupChg				(86 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdHddErr					(87 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdSetupIn				(88 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdSetupOut				(89 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdHddFormat			(90 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdRamup					(91 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdEmergency			(92 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdMacaddress			(93 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdIp							(94 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdPort						(95 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdUse						(96 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdSensitivity		(97 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdWebcode				(98 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdFwVer					(99 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdNwVer					(100 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdModelNum				(101 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdFileSystem			(102 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdFirmWare				(103 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdUpgrade				(104 + E_GrP3ScptTxtIdBase)
#define E_GrP3ScptTxtIdBatteryErr			(105 + E_GrP3ScptTxtIdBase)

// STX
#define E_GrP3ScptObjStx							0x4F
#define E_GrP3ScptParaStx							0x50
#define E_GrP3ScptObjEdx							0x45

// script id
#define E_GrP3ScptIdNone							0				/* none script */
#define E_GrP3ScptIdCrnt							0xFFFE	/* current script id - event ... */
#define E_GrP3ScptIdAll								0xFFFF	/* all script */

#define E_GrP3ScptIdWinBase						1				/* window base script id */
#define E_GrP3ScptIdPageBase					1000		/* page base script id */
#define E_GrP3ScptIdSlaveBase					4000		/* slave object base script id */

// paint script id
#define E_GrP3ScptPstIdNone						0				/* paint script id */
#define E_GrP3ScptPstIdBase						1				/* paint script id base */
#define E_GrP3ScptPstIdDflt						1				/* paint script id default */

#define E_GrP3ScptPstBasisSt					0				/* basis start */
#define E_GrP3ScptPstBasisEd					1				/* basis end */
#define E_GrP3ScptPstBasisCtSt				2				/* basis center start */
#define E_GrP3ScptPstBasisCtEd				3				/* basis center end */

#define E_GrP3ScptPstTypeNone					0
#define E_GrP3ScptPstTypeClrFill			1				/* color fill */
#define E_GrP3ScptPstTypeLine					2				/* line draw */
#define E_GrP3ScptPstTypeImage				3				/* image */
#define E_GrP3ScptPstTypeText					4				/* text */
#define E_GrP3ScptPstTypeFont					5				/* font */
#define E_GrP3ScptPstTypeStyle				6				/* text style */

#define E_GrP3ScptPstTypeDisable			0x80		/* disable mask */

#define E_GrP3ScptPstImgAtbFill				0x01		/* image fill */
#define E_GrP3ScptPstImgAtbSprite			0x02		/* sprite image */

#define E_GrP3ScptPstTxtAtbFixPitch				(1 << 0)	/* fixed pitch */
#define E_GrP3ScptPstTxtAtbDrawShadow			(1 << 1)	/* shadow draw */
#define E_GrP3ScptPstTxtAtbMultiLine			(1 << 2)	/* multi line */
#define E_GrP3ScptPstTxtAtbWarp						(1 << 3)	/* warp */
#define E_GrP3ScptPstTxtAtbSemiEnter			(1 << 4)	/* semicolon enter */
#define E_GrP3ScptPstTxtAtbPasswd					(1 << 5)	/* password mode */

#define E_GrP3ScptPstTxtAlignHzCenter			0
#define E_GrP3ScptPstTxtAlignHzLeft				1
#define E_GrP3ScptPstTxtAlignHzRight			2
#define E_GrP3ScptPstTxtAlignVtCenter			0
#define E_GrP3ScptPstTxtAlignVtTop				1
#define E_GrP3ScptPstTxtAlignVtBottom			2

// para type id
#define E_GrP3ScptParaTypeChar				0				/* __s8 */
#define E_GrP3ScptParaTypeShort				1				/* __s16 */
#define E_GrP3ScptParaTypeInt					2				/* __s32 */
#define E_GrP3ScptParaTypeBool8				3				/* bool8 */
#define E_GrP3ScptParaTypeByteRect		4				/* St_GrByteRect */
#define E_GrP3ScptParaTypeWordRect		5				/* St_GrWordRect */
#define E_GrP3ScptParaTypeIntRect			6				/* St_GrRect */
#define E_GrP3ScptParaTypeScptId			7				/* script id */
#define E_GrP3ScptParaTypeColor				8				/* color */
#define E_GrP3ScptParaTypeSkin				9				/* skin image */
#define E_GrP3ScptParaTypeBrdMap			10			/* border map */

#define E_GrP3ScptParaTypeTextId			20			/* text id */


//====================================================================
//global macro

//====================================================================
//global type

typedef	struct	St_GrP3ScptDataHd
{
	__u8	Type;			// data type
	__u8	SubIdx;		// skin index or language index
	__u16	Id;				// script id
	__u32	Size;			// data size
}	*Ptr_GrP3ScptDataHd;

typedef	struct St_GrP3ScptObjHd 
{
	__u8	Stx;			// O
	__u8	ClassId;	// class id
	__u16	ScptId;		// script id
	__u16	_rsvd0;
	__u16	ParaSize;			// parameter size
	St_GrSwordRect	Rect;
}	*Ptr_GrP3ScptObjHd;

typedef	struct	St_GrP3ScptParaHd
{
	__u8	Stx;			// P
	__u8	ClassId;	// class id
	__u16	ParaSize;	// parameter size
}	*Ptr_GrP3ScptParaHd;

typedef	struct St_GrP3ScptObjEnd 
{
	__u8	Edx;		// E
	__u8	_rsvd;
	__u16	ScptId;
}	*Ptr_GrP3ScptObjEnd;

typedef	struct	St_GrP3ScptAsmInfo
{
	void*	Pos;
	__u16	Cnt;		// code count
	__u16	_rsvd;
}	*Ptr_GrP3ScptAsmInfo;

typedef	struct	St_GrP3ScptTxtItm
{
	WCHAR*	LngTbl[E_GrP3ScptLangMaxCnt];
}	*Ptr_GrP3ScptTxtItm;


typedef struct St_GrP3ScptPstItm 
{
	void*	Pos;
	__u8	Cnt;
	__u8	_rsvd0;
	__u16	_rsvd1;
}	*Ptr_GrP3ScptPstItm;

typedef	struct	St_GrP3ScptPstData
{
	__s16	Left;
	__s16	Top;
	__s16	Right;
	__s16	Bottom;
	__u8	Basis;
	__u8	Type;
	__u8	Para0;
	__u8	Para1;
	__u32	Value;
}	*Ptr_GrP3ScptPstData;

typedef	struct	St_GrP3ScptWinInfo
{
	void*	Pos;
	__u8	Layer;
	__u8	_rsvd0;
	__u16	Privilege;
}	*Ptr_GrP3ScptWinInfo;

typedef	struct	St_GrP3ScptPageInfo
{
	void*	Pos;
	__u16	AsmOpen;			// open code id
	__u16	AsmClose;			// close code id
	__u16	EscKeyFcsId;	// exit focus id
	__u16	_rsvd1;
	__u32	_rsvd[1];			// reserved
}	*Ptr_GrP3ScptPageInfo;

typedef	struct	St_GrP3ScptCfgSkin
{
	__u32	ClrCus;		// cursor color
	__u32	_rsvd[3];	
}	*Ptr_GrP3ScptCfgSkin;

typedef	struct	St_GrP3ScptVkbdLang
{
	__u8	PageCnt;
	__u8	_rsvd0;
	__u16	_rsvd1;
	WCHAR	KeyTbl[E_GrP3VkeyShftCnt][E_GrP3VkeyTxtCnt];
}	*Ptr_GrP3ScptVkbdLang;

typedef	struct St_GrP3ScptCfgPrj		// project config
{
	__u32	FmtFcc;				// format FCC
	__u16	CusImgId;			// software cursor image id
	__u8	UartCnt;			// UART count
	__u8	UiResId;			// UI resolution ID
	__u32	HwType;				// hardware type
	__u32	FwVer;				// firmware version
	St_GrP3ScptVkbdLang	VkbdTbl[E_GrP3ScptLangMaxCnt];		// virtual keyboard table
}	*Ptr_GrP3ScptCfgPrj;

typedef	struct St_GrP3ScptCfgRtl
{
	BOOL8	IsChgConAble;	// can change console focus
	__u8	_rsvd1;				// 
	__u8	_rsvd2;				// 
	__u8	_rsvd3;				// 
	St_GrP3ScptCfgSkin	Skin;
}	*Ptr_GrP3ScptCfgRtl;

typedef	struct	St_GrP3ScptCfgMdl		// model config
{
	__u16	ModelNum;			// [0000] model number
	__u16	AsmInit;			// [0002] assembly init
	__u16	AsmStart;			// [0004] assembly start
	__u16	AsmCfgReset;	// [0006] assembly configuration reset
	__u16	AsmCfgPatch;	// [0008] assembly configuration patch
	__u16	AsmFormat;		// [000A] assembly HDD format
	__u16	AsmLivePlay;	// [000C] assembly live / play mode change
	__u16	AsmFwUpDone;	// [000E] firmware upgrade finished
	__u8	VsstType;			// [0010] 0 = none , 1 ...
	BOOL8	IsIoBox;			// [0011] extended IO box able
	BOOL8	IsEvtPreset;	// [0012] event preset
	BOOL8	IsFreeDdns;		// [0013] free DDNS
	BOOL8	IsPtzKbd;			// [0014] pan-tilt keyboard
	__u8	DfltVgaRes;		// [0015] default VGA resolution
	__u8	DfltIconPos;	// [0016] default quad icon position
	__u8	DfltLang;			// [0017] default language
	__u8	DfltSchd;			// [0018] default record schedule
	BOOL8	IsBkvwLater;	// [0019] backup viewer extract later
	BOOL8	IsRamup;			// [001A] ram-up use
	BOOL8	IsSmart;			// [001B] HDD smart use
	BOOL8	IsQuadSave;		// [001C] last quad save
	BOOL8	IsLoginSave;	// [001D] last login save
	__u8	MaxHddCnt;		// [001E] usable HDD map - bitmap
	__u8	IsPassChk;		// [001F] password check 
	BOOL8	IsEnvSenor;		// [0020] environment sensor use
	__u8	LongKeyTime;	// [0021] long key time
	__u16	TxtVkbdId;		// [0x22] text virtual keyboard window id
	__u16	NumVkbdId;		// [0x24] number virtual keyboard window id
	__u16	Notify0Id;		// [0x26] notify window 0 id
	__u16	QryYn0Id;			// [0x28] query yes , no window 0 id
	__u16	QryYnc0Id;		// [0x2A] query yes , no , cancel window 0 id

	__u16	AsmLogEnd;		// [0x2C] log backup end event
	__u16	AsmLogErr;		// [0x2E] log backup error event

	__u16	Notify1Id;		// [0x30] notify window 1 id
	__u16	Notify2Id;		// [0x32] notify window 2 id
	__u16	Notify3Id;		// [0x34] notify window 3 id
	__u16	QryYn1Id;			// [0x36] query yes , no window 1 id
	__u16	QryYn2Id;			// [0x38] query yes , no window 2 id
	__u16	QryYn3Id;			// [0x3A] query yes , no window 3 id
	__u16	QryYnc1Id;		// [0x3C] query yes , no , cancel window 1 id
	__u16	QryYnc2Id;		// [0x3E] query yes , no , cancel window 2 id
	__u16	QryYnc3Id;		// [0x40] query yes , no , cancel window 3 id

	__u16	_rsvd1;				// [0x42] reserved

	__u32	_rsvd2[4];		// [0x44] reserved

	__u16	PassRuleTitle;	// [0x54] bad password rule message title text id
	__u16	PassRuleCont;		// [0x56] bad password rule message contents text id

	__u8	UserIdCnt;			// [0x58] authority id count
	BOOL8	IsPassDual;			// [0x59] dual password
	BOOL8	IsEmgcLock;			// [0x5A] emergency key lock usable
	__u8	IsMtnSchd;			// [0x5B] motion schedule

	WCHAR	MasterPass[16];	// [0x5C] master password

	__u16	AsmDenEmgc;			// [0x7C] emergency denial event
	__u16	AsmWinPriv;			// [0x7E] window has not privilege

	// version 4 expansion
	__u8	StartLogin;			// [0x80] start login type , 0=ADMIN,1=by setup
	__u8	IsLogRec;				// [0x81] event log record only
	__u16	AsmLanOnOff;		// [0x82]	LAN on off status

	BOOL8	IsCmsFireWall;	// [0x84] CMS firewall use
	BOOL8	IsCvtUsr;				// [0x85] user covert active
	__u16	AsmPlayStat;		// [0x86] play status change

	__u16	AsmUsbChg;			// [0x88] USB change event
	__u16	AsmCmsChg;			// [0x8A] CMS client count change event

	__u16	AsmQuad0Chg;		// [0x8C] console 0 QUAD change
	__u16	AsmQuad1Chg;		// [0x8C] console 1 QUAD change

	__u16	AsmBkupStart;		// [0x90] backup start
	__u16	AsmBkupEnd;			// [0x92] backup end
	__u16	AsmBkupErr;			// [0x94] backup error
	BOOL8	IsDmaxFront;		// [0x96] D-Max front use
	__u8	WebLogo;				// [0x97] web logo

	__u16	AsmErrOcu;			// [0x98] error occur
	__u16	AsmUserChg;			// [0x9A] log in log out status change

	__u16	AsmNtpTest;			// [0x9C] NTP Test finish event
	__u16	AsmLogStart;		// [0x9E] log backup start

	__u16	AsmFwUpStart;		// [0xA0] firmware upgrade start
	__u16	_rsvd3;					// [0xA2] reserved

	__u16	VrecQt[E_GrP3VencResTypeCnt][E_GrP3VencResSubCnt][E_GrP3VencQtyCnt];	// [0xA4]
	__u16	VnetQt[E_GrP3VencResTypeCnt][E_GrP3VencResSubCnt][E_GrP3VencQtyCnt];	// [0x144]

	__u16	_rsvd4;					// [0x1E4] reserved
	__u16	_rsvd5;					// [0x1E6] reserved

}	*Ptr_GrP3ScptCfgMdl;

typedef	struct	St_GrP3ScptHdInfo
{
	__u32	Fcc;
	__u32	Ver;
	Def_GrTime	Time;		// make time
	__u32	_rsvd[5];
}	*Ptr_GrP3ScptHdInfo;

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

